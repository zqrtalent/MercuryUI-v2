#include "stdafx.h"
#include "..\Protocol\BasePacket.h"
#include "GameClient.h"
#include "..\Utility\Timers.h"
//#include <Af_irda.h>

///////////////////////////////////////////////////////////
///
///	class GameClient
///
///////////////////////////////////////////////////////////

GameClient::GameClient(HWND hOwnerWnd, _String sIpAddress /*= _T("127.0.0.1")*/, int nPort /*= 20001*/,  int nReconnectTime /*= 5000*/) : m_memIncompleteData(2048, 1024, true) {
	m_hwndOwner					= hOwnerWnd;
	m_nConnDisMessageId			= 0L;
	m_sIpAddress				= sIpAddress;
	m_nPort						= nPort;
	m_bAutoDelete				= false;
	m_pSockCtx					= NULL;

	m_bNotifyFriendConns		= true;

	m_threadDataManager			= NULL;
	m_dwDataManagerThreadId		= 0L;
	m_pStopDataManager			= NULL;
	m_pResumeDataManager		= NULL;
	m_nSessionId				= 0;
	m_bUseSSL					= false;

	m_bKeepAlive				= true;
	m_nKeepAliveTime			= 1000*60*10;	//10 minutes.
	m_nKeepAliveInterval		= 5000;			//5 seconds.

	m_bLoggedIn					= false;
	m_nUserId					= 0;
	m_bChatIsBlocked			= false;
	m_bAvatarIsBlocked			= false;

	m_nClientVersion			= 1100;
	m_pPacketHandler			= NULL;
	m_pEventWaitForResponse		= NULL;
	m_nIncompletePacketSize		= 0;

	m_pPacketHandler			= new PacketHandler();
	m_pPacketHandler->AddPacketObject(PacketCodes::_HandShake,				new HandShakePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_Login,					new LoginPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_Logout,					new LogoutPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_RegisterUser,			new RegisterUserPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_AcquireTableAccess,		new AcquireTableAccessPacket());

	// Initialize packet listener methods. ###############################
	m_pPacketHandler->AddPacketHandler(PacketCodes::_HandShake,				this, (PacketListenerMethod)&GameClient::OnHandShake);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Login,					this, (PacketListenerMethod)&GameClient::OnLogin);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Logout,				this, (PacketListenerMethod)&GameClient::OnLogout);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_AcquireTableAccess,	this, (PacketListenerMethod)&GameClient::OnAcquireTableAccess);
	// ###################################################################
	} 

GameClient::~GameClient(){
	RemoveConnectionFromFriendsList(this);

	// Disconnect connection.
	Disconnect();
	
	// Delete wait for response event.
	if( m_pEventWaitForResponse ){
		delete m_pEventWaitForResponse;
		m_pEventWaitForResponse = 0L;
		}

	// Destroy packet handler.
	if( m_pPacketHandler != NULL ){
		delete m_pPacketHandler;
		m_pPacketHandler = NULL;
		}
	}

///////////////////////////////////////////////////////////
///
///	Public Methods
///
///////////////////////////////////////////////////////////

void
GameClient::SafeDestroy(){
	if( GetCurrentThreadId() != m_dwDataManagerThreadId ){	
		Disconnect();
		delete this;
		}
	else{ // When destroy class object called from DataManager thread.
		m_bAutoDelete = true;
		Disconnect();
		}
	}

bool
GameClient::SetKeepAlive(bool bKeepAlive, int nKeepAliveTime, int nKeepAliveInterval){
	if( IsConnected() ){
		if( !SocketContext::SetKeepAliveOption(m_pSockCtx->GetSocket(), bKeepAlive, nKeepAliveTime, nKeepAliveInterval) )
			return false;
		m_bKeepAlive			= bKeepAlive;
		m_nKeepAliveTime		= nKeepAliveTime;
		m_nKeepAliveInterval	= nKeepAliveInterval;
		return true;
		}
	m_bKeepAlive			= bKeepAlive;
	m_nKeepAliveTime		= nKeepAliveTime;
	m_nKeepAliveInterval	= nKeepAliveInterval;
	return true;
	}

void
GameClient::UseSSL(bool bUseSSL, const _String sClientCert){
	m_bUseSSL			= bUseSSL;
	m_sClientSSLCert	= sClientCert;
	}

bool
GameClient::IsConnected(){
	if( !m_pSockCtx ) 
		return false;
	return m_pSockCtx->IsConnected();
	}

bool
GameClient::Connect(SOCKET* pCopySocket /*= NULL*/, HANDLE* pEventConnect /*= NULL*/){
	if( IsConnected() ) 
		return false;
	HANDLE hConnectEvent	= CreateEvent(NULL, TRUE, FALSE, NULL);
	if( !m_pSockCtx ){
		m_pSockCtx			= new SSLSocketContext();
		m_pSockCtx->SetIOEventHandler(this);
		// Use SSL.
		if( m_bUseSSL )
			m_pSockCtx->InitSSL(m_sClientSSLCert.IsEmpty() ? NULL : m_sClientSSLCert.GetBuffer());
		}

	if( pEventConnect )
		*pEventConnect = hConnectEvent;

	bool bRet = m_pSockCtx->Connect(m_sIpAddress.GetBuffer(), m_nPort, m_bKeepAlive, m_nKeepAliveTime, m_nKeepAliveInterval, pCopySocket, hConnectEvent);
	if( bRet ){
		// Start data manager.
		StartDataManager();
		// Wait for connection established.
		WaitForSingleObject(hConnectEvent, INFINITE);
		// Update connected status.
		bRet = m_pSockCtx->IsConnected();
		}
	CloseHandle(hConnectEvent);
	return bRet;
	}

bool
GameClient::Disconnect(){
	if( m_pSockCtx ){
		m_pSockCtx->SafeClose();
		delete m_pSockCtx;
		}
	m_pSockCtx = NULL;
	return true;
	}

bool
GameClient::AddPacketHandler(PacketCodes code, void* pClass, PacketListenerMethod method){
	return m_pPacketHandler ? m_pPacketHandler->AddPacketHandler(code, pClass, method) : false;
	}

bool
GameClient::CallPacketHandler(BasePacket* p, void* pParam){
	return m_pPacketHandler ? m_pPacketHandler->HandlePacket(p, pParam) : false;
	}

///////////////////////////////////////////////////////////
///
///	Virtual events.
///
///////////////////////////////////////////////////////////

void
GameClient::OnAcceptConnection(SOCKET sock, const _String sRemoteIp, int nRemotePort, bool& bAccept){
	bAccept = true;
	}

void
GameClient::OnConnected(SOCKET sock, ConnectedSocketInfo* pSockInfo){
	}

void
GameClient::OnDataReceived(SocketContext* pConn, BYTE* pData, DWORD dwSize, bool bIsFirstData){
	AddReceivedDataInQueue(pData, dwSize);
	}

void
GameClient::OnDisconnected(SocketContext* pConn, DWORD dwLastError){
	/*&& dwLastError == ERROR_SEM_TIMEOUT*/ //KeepAlive timeout
	if( m_hwndOwner && IsWindow(m_hwndOwner) ){
		if( GetCurrentThreadId() == GetWindowThreadProcessId(m_hwndOwner, NULL) )
			::SendMessage(m_hwndOwner, m_nConnDisMessageId, (WPARAM)0, 0L); // 0 means disconnected state
		else
			::PostMessage(m_hwndOwner, m_nConnDisMessageId, (WPARAM)0, 0L); // 0 means disconnected state
		}

	// Keep session id when connection dropped.
	if( GetCurrentThreadId() != m_dwDataManagerThreadId )
		m_nSessionId = 0;
	}

///////////////////////////////////////////////////////////
///
///	Public server operations.
///
///////////////////////////////////////////////////////////

void		
GameClient::Login(_String sUserName, _String sPassword){
	LoginPacket* packet = new LoginPacket();
	packet->ZeroPacketVariables();
	packet->m_sPassword = sPassword;
	packet->m_sUserName = sUserName;
	SendPacket(packet);
//	SendPacket(packet);
//	SendPacket(packet);
	delete packet;
	}

void
GameClient::LoginByWebSession(const _String sWebSession){
	LoginPacket* packet		= new LoginPacket();
	packet->ZeroPacketVariables();
	packet->m_sWebSessionId = sWebSession;
	SendPacket(packet);
	delete packet;
	}

void
GameClient::Logout(){
	LogoutPacket* packet = new LogoutPacket();
	SendPacket(packet);
	delete packet;
	}

void
GameClient::HandShake(){
	SYSTEM_INFO info;
	::GetSystemInfo(&info);

	HandShakePacket* packet			= new HandShakePacket();
	packet->ZeroPacketVariables();
	packet->m_nClientVer			= m_nClientVersion;
	packet->m_nProtocolVer			= PROTOCOL_VER;
	packet->m_bPacketCompressionZ	= false;
	packet->m_sPCName				= _T("");
	packet->m_sOS					= _T("Windows OS");
	packet->m_sProcessor			= _T("");
	SendPacket(packet);
	delete packet;
	}

void
GameClient::AcquireTableAccess(__int64 nSessionId, int nTableId, int nTournamentId){
	AcquireTableAccessPacket* packet	= new AcquireTableAccessPacket();
	packet->ZeroPacketVariables();
	packet->m_nSessionIdAcquire			= nSessionId;
	packet->m_nTableId					= nTableId;
	packet->m_nTournamentId				= nTournamentId;
	packet->m_nUserId					= 0;
	packet->m_bChatIsBlocked			= false;
	packet->m_bAvatarIsBlocked			= false;
	packet->m_nUserId					= 0;
	SendPacket(packet);
	delete packet;
	}

void
GameClient::RegisterUser(const _String sUserName, const _String sPwd, const _String sPwdRepeat, const _String sEmailAddress, const _String sPhoneNumber, const _String sNickName){
	RegisterUserPacket* packet	= new RegisterUserPacket();
	packet->ZeroPacketVariables();
	packet->m_sUserName		= sUserName;
	packet->m_sPwd			= sPwd;
	packet->m_sPwdRepeat	= sPwdRepeat;
	packet->m_sEmailAddress	= sEmailAddress;
	packet->m_sPhoneNumber	= sPhoneNumber;
	packet->m_sNickName		= sNickName;
	SendPacket(packet);
	delete packet;
	}

bool
GameClient::SendPacket(BasePacket* packet){
	if( !IsConnected() )
		return false;

	bool bRet = false;
	m_secLock1.Lock(); // Lock
	packet->m_nSessionId = m_nSessionId;
	m_memSerialize.ZeroUsedMemory();
	if( packet->Serialize(&m_memSerialize) ){
		// Push socket data.
		bRet = m_pSockCtx->Send(m_memSerialize.GetBufferPtr(), m_memSerialize.GetUsedBufferSize());
		}
	m_secLock1.Unlock(); // Unlock
	return bRet;
	}

///////////////////////////////////////////////////////////
///
///	Thread methods.
///
///////////////////////////////////////////////////////////

UINT  __stdcall 
GameClient::DataManage_Thread(GameClient* pClient){
	ASSERT(pClient);
	CEvent*				pStop		= pClient->m_pStopDataManager;
	CEvent*				pResume		= pClient->m_pResumeDataManager;
	CCriticalSection*	lock		= &pClient->m_lockDataManager;
	List<WSABUF>*		pListData	= &pClient->m_listReceivedData;

	BYTE*				lpData		= NULL;
	int					nSize		= 0;

	while( TRUE ){
		// Wait while event is not signaled.
		WaitForSingleObject(*pResume, INFINITE);
		// Check if stop event is signaled. {{
		if( WaitForSingleObject(*pStop, 0) == WAIT_OBJECT_0 ) break;

		// Manage received data. {{
		lock->Lock();			// Lock
		WSABUF* pData	= pListData->GetAt(0);
		if( pData ){
			lpData		= (BYTE*)pData->buf;
			nSize		= (int)pData->len;
			}
		pListData->RemoveAt(0); // Remove first element.
		lock->Unlock();			// Unlock

		if( lpData && nSize > 0 ){
			pClient->HandleReceivedData(lpData, nSize);
			delete [] lpData;
			lpData	= NULL;
			nSize	= 0;
			}
		// }}

		lock->Lock();// Lock
		if( !pListData->GetCount() )
			pResume->ResetEvent(); // Nothing to do.
		lock->Unlock();// Unlock
		}
	return 0L;
	}

///////////////////////////////////////////////////////////
///
///	Protected methods.
///
///////////////////////////////////////////////////////////

bool
GameClient::StartDataManager(){
	if( !m_threadDataManager ){
		m_pStopDataManager		= new CEvent(FALSE, TRUE);
		m_pResumeDataManager	= new CEvent(FALSE, TRUE);
		m_threadDataManager		= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)GameClient::DataManage_Thread, this, 0L, &m_dwDataManagerThreadId);
		return true;
		}
	return false;
	}

bool
GameClient::StopDataManager(){
	bool bRet = false;
	if( m_threadDataManager ){
		m_pStopDataManager		->SetEvent();
		m_pResumeDataManager	->SetEvent();
		WaitForSingleObject(m_threadDataManager, INFINITE);

		delete m_pStopDataManager;
		delete m_pResumeDataManager;
		CloseHandle(m_threadDataManager);

		m_pStopDataManager	= m_pResumeDataManager = NULL;
		m_threadDataManager = NULL;

		// Delete list of received data. {{
		int nLoop = 0;
		while( nLoop < m_listReceivedData.GetCount() ){
			WSABUF* pBuf = m_listReceivedData[nLoop];
			if( pBuf ) delete [] pBuf->buf;
			nLoop ++;
			}
		m_listReceivedData.RemoveAll();
		// }}
		bRet = true;
		}

	if( m_bAutoDelete )
		delete this;
	return bRet;
	}

bool
GameClient::AddReceivedDataInQueue(BYTE* lpData, int nSize){
	if( nSize <= 0 ) return false;
	if( m_threadDataManager ){
		WSABUF* pBuff	= new WSABUF;
		pBuff->buf		= (char*)new BYTE[nSize];
		memcpy(pBuff->buf, lpData, nSize);
		pBuff->len		= nSize;
		
		m_lockDataManager.Lock();	// Lock
		m_listReceivedData.Add(pBuff);
		m_lockDataManager.Unlock(); //Unlock

		m_pResumeDataManager->SetEvent();
		return true;
		}
	return false;
	}

void
GameClient::HandleReceivedData(BYTE* lpData, int nSize){
	// Handle received packet(s). {{
	GrowableMemory	mem(0, 0x400, false);
	int				nOffset	= 0, nBufferSize = nSize, nSerializedBytes = 0;
	BYTE*			lpDeserialize = lpData;
	BasePacket		packet;
	#define			basePacketLimitSize 64

	if( m_memIncompleteData.GetUsedBufferSize() > 0 ){
		// Add chunk data bytes.
		m_memIncompleteData.AddBytes(lpData, nSize);
		lpDeserialize	= m_memIncompleteData.GetBufferPtr();
		nBufferSize		= m_memIncompleteData.GetUsedBufferSize();
		}

	while( TRUE ){
		mem.SetReadonlyBuffer(&lpDeserialize[nOffset], nBufferSize);	
		packet.m_nCode			= 0;
		packet.m_nPacketSize	= 0;
		if( packet.Deserialize(&mem) ){ // Success
			if( packet.m_nPacketSize > nBufferSize ){
				// Add first chunk data bytes.
				if( m_memIncompleteData.GetUsedBufferSize() == 0 )
					m_memIncompleteData.AddBytes(&lpDeserialize[nOffset], nBufferSize);
				m_nIncompletePacketSize = packet.m_nPacketSize;
				break;
				}

			if( !m_pPacketHandler->HandlePacketByCode(packet.m_nCode, this, &lpDeserialize[nOffset], nBufferSize, &nSerializedBytes) || 
				nSerializedBytes >= nBufferSize ){
				m_memIncompleteData.ZeroUsedMemory();
				m_nIncompletePacketSize = 0;
				break;
				}

			if( m_memIncompleteData.GetUsedBufferSize() > 0 ){
				m_memIncompleteData.RemoveBytes(0, nSerializedBytes);
				lpDeserialize			= m_memIncompleteData.GetBufferPtr();
				nBufferSize				= m_memIncompleteData.GetUsedBufferSize();
				nOffset					= 0;
				m_nIncompletePacketSize = 0;
				}
		
			nBufferSize -= nSerializedBytes;
			nOffset		+= nSerializedBytes;
			}
		else{
			if( m_nIncompletePacketSize == 0 ){
				if( m_memIncompleteData.GetUsedBufferSize() > basePacketLimitSize ){
					m_memIncompleteData.ZeroUsedMemory();
					m_nIncompletePacketSize = 0;
					}
				else{ // Add first chunk data bytes.
					if( m_memIncompleteData.GetUsedBufferSize() == 0 )
						m_memIncompleteData.AddBytes(&lpDeserialize[nOffset], nBufferSize);
					}
				}
			break;
			}
		}
	}

///////////////////////////////////////////////////////////
///
///	Packet receive listener methods.
///
///////////////////////////////////////////////////////////
void
GameClient::OnHandShake(HandShakePacket* p, void* pParam){
	if( m_bLoggedIn && m_nSessionId != p->m_nSessionId ){
		m_bLoggedIn = false;
		m_nUserId	= 0;
		m_sUserName	= _T("");
		}
	m_nSessionId = p->m_nSessionId; // Save session id.
	}

void
GameClient::OnAcquireTableAccess(AcquireTableAccessPacket* p, void* pParam){
	if( p->m_nErrorCode == ES_ERROR_SUCCESS ){
		m_nSessionId		= p->m_nSessionId;
		m_nUserId			= p->m_nUserId;
		m_bLoggedIn			= (p->m_nUserId > 0);
		m_bChatIsBlocked	= p->m_bChatIsBlocked;
		m_bAvatarIsBlocked	= p->m_bAvatarIsBlocked;
		}
	}

void	
GameClient::OnLogin(LoginPacket* p, void* pParam){
	if( p->m_nErrorCode != ES_ERROR_SUCCESS )
		return;
	m_bLoggedIn				= true;
	m_sUserName				= p->m_sUserName;
	m_nUserId				= p->m_nUserId;
	m_bChatIsBlocked		= p->m_bChatIsBlocked;
	m_bAvatarIsBlocked		= p->m_bAvatarIsBlocked;

	if( m_bNotifyFriendConns ){
		// Call packet handlers of friend list connections.
		GetFriendConnectionsLock()->Lock();		// Lock
		AutoSortedArray* pArrList = GetFriendConnectionsArray();
		for(int i=0; i<pArrList->GetCount(); i++){
			GameClient* pConn = (GameClient*)pArrList->Get(i);
			if( pConn == this )
				continue;
			pConn->m_bNotifyFriendConns = false;
			pConn->CallPacketHandler(p, pConn);
			pConn->m_bNotifyFriendConns = true;
			}
		GetFriendConnectionsLock()->Unlock();	// Unlock
		}
	}

void	
GameClient::OnLogout(LogoutPacket* p, void* pParam){
	if( p->m_nErrorCode == ES_ERROR_SUCCESS )
		m_nSessionId	= p->m_nSessionId;
	m_bLoggedIn				= false;
	m_sUserName				= _T("");
	m_nUserId				= 0;
	m_bChatIsBlocked		= false;
	m_bAvatarIsBlocked		= false;

	if( m_bNotifyFriendConns ){
		// Call packet handlers of friend list connections.
		GetFriendConnectionsLock()->Lock();		// Lock
		AutoSortedArray* pArrList = GetFriendConnectionsArray();
		for(int i=0; i<pArrList->GetCount(); i++){
			GameClient* pConn = (GameClient*)pArrList->Get(i);
			if( pConn == this )
				continue;
			pConn->m_bNotifyFriendConns = false;
			pConn->CallPacketHandler(p, pConn);
			pConn->m_bNotifyFriendConns = true;
			}
		GetFriendConnectionsLock()->Unlock();	// Unlock
		}
	}

bool
GameClient::WaitForResponse(int nPacketCode, BasePacket** packetResponse, int nWaitSeconds, CEvent* pEventCancel /*= NULL*/){
	m_pEventWaitForResponse = new CEvent(FALSE, TRUE);
	m_pPacketHandler->SetPacketWaitingFor(m_pEventWaitForResponse, nPacketCode, packetResponse);

	if( pEventCancel ){
		int nWaitOnce = 200, nLoop = 0, nLoopCt = (nWaitSeconds*1000)/nWaitOnce;
		while(nLoop < nLoopCt){
			if( ::WaitForSingleObject(*m_pEventWaitForResponse, (DWORD)(nWaitOnce)) == WAIT_OBJECT_0 ){
				m_pPacketHandler->SetPacketWaitingFor(NULL, 0L, NULL);
				delete m_pEventWaitForResponse;
				m_pEventWaitForResponse = NULL;
				return true;
				}
			// Check for cancel event.
			if( ::WaitForSingleObject(*pEventCancel, 0) == WAIT_OBJECT_0 ){
				delete m_pEventWaitForResponse;
				m_pEventWaitForResponse = NULL;
				return false;
				}
			nLoop ++;
			}
		}
	else{
		if( ::WaitForSingleObject(*m_pEventWaitForResponse, (DWORD)(nWaitSeconds*1000)) == WAIT_OBJECT_0 ){
			m_pPacketHandler->SetPacketWaitingFor(NULL, 0L, NULL);
			delete m_pEventWaitForResponse;
			m_pEventWaitForResponse = NULL;
			return true;
			}
		}

	m_pPacketHandler->SetPacketWaitingFor(NULL, 0L, NULL);
	if( AfxIsValidAddress(m_pEventWaitForResponse, sizeof(void*)) )
		delete m_pEventWaitForResponse;
	m_pEventWaitForResponse = NULL;
	return false;
	}
