#include "stdafx.h"
#include "ClientConnection.h"
#include "../Protocol/BasePacket.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"
#include "GameServer.h"
#include "../SocketLibrary/SocketProtocolFactory.h"
#include "Log/Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class GameServer
///
///////////////////////////////////////////////////////////

GameServer::GameServer() {
	m_pConfig					= NULL;
	m_bStarted					= false;

	m_pPacketHandler			= new PacketHandler(false);
	m_pPacketHandler->AddPacketObject(PacketCodes::_HandShake,			new HandShakePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_AcquireTableAccess,	new AcquireTableAccessPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_Login,				new LoginPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_Logout,				new LogoutPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_RegisterUser,		new RegisterUserPacket());
	
	// Initialize packet listener methods. ###############################
	m_pPacketHandler->AddPacketHandler(PacketCodes::_HandShake,				this, (PacketListenerMethod)&GameServer::OnHandShake);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_AcquireTableAccess,	this, (PacketListenerMethod)&GameServer::OnAcquireTableAccess);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Login,					this, (PacketListenerMethod)&GameServer::OnLogin);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Logout,				this, (PacketListenerMethod)&GameServer::OnLogout);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_RegisterUser,			this, (PacketListenerMethod)&GameServer::OnRegisterUser);
	// ###################################################################

	m_arrVariableOffset.Add((void*)offsetof(BasePacket, m_nPacketSize), NULL);

	/*
	m_sIpAddress				= sIpAddress;
	m_nPort						= nPort;
	m_bStarted					= false;
	m_nReceiveDataSizeMax		= 16*1024;	// 16 KB

	m_nConnectionCt				= 0;
	m_nBytesReceived			= 0;
	m_nBytesSent				= 0;
	
	m_arrAcceptedSocks			.OptimizeForLargeData(TRUE);
	m_arrBlockedIps				.OptimizeForLargeData(TRUE);
	m_pMem						= new GrowableMemory(0x400, 0x400, true);

	m_pSockSendComplete			= new CEvent(FALSE, TRUE);

	m_threadPacketSender		= NULL;
	m_dwPacketSenderThreadId	= 0L;
	m_pStopPacketSender			= NULL;
	m_pResumePacketSender		= NULL;

	m_pPacketHandler			= new PacketHandler(false);
	m_pPacketHandler->AddPacketObject(PacketCodes::_HandShake,		new HandShakePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_CreateSession,	new CreateSessionPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_DestroySession,	new DestroySessionPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_Login,			new LoginPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_Logout,			new LogoutPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_RegisterUser,	new RegisterUserPacket());
	
	// Initialize packet listener methods. ###############################
	m_pPacketHandler->AddPacketHandler(PacketCodes::_HandShake, this, (PacketListenerMethod)&GameServer::OnHandShake);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Login, this, (PacketListenerMethod)&GameServer::OnLogin);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Logout, this, (PacketListenerMethod)&GameServer::OnLogout);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_CreateSession, this, (PacketListenerMethod)&GameServer::OnCreateSession);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_DestroySession, this, (PacketListenerMethod)&GameServer::OnDestroySession);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_RegisterUser, this, (PacketListenerMethod)&GameServer::OnRegisterUser);
	// ###################################################################

	// Assign packets to main thread. {{
	DataManageThread* pManageThread = new DataManageThread(this);
	m_listDataManageThreads.Add(pManageThread);
	m_arrDataManThreadByPacket.Add((__int64)PacketCodes::_HandShake, (__int64)pManageThread);
//	m_arrDataManThreadByPacket.Add((__int64)PacketCodes::_Login, (__int64)pManageThread);
//	m_arrDataManThreadByPacket.Add((__int64)PacketCodes::_Logout, (__int64)pManageThread);
	m_arrDataManThreadByPacket.Add((__int64)PacketCodes::_CreateSession, (__int64)pManageThread);
	m_arrDataManThreadByPacket.Add((__int64)PacketCodes::_DestroySession, (__int64)pManageThread);
	m_arrDataManThreadByPacket.Add((__int64)PacketCodes::_RegisterUser, (__int64)pManageThread);
	// }}
	*/
	} 

GameServer::~GameServer(){
	Stop();

	// Destroy packet handler. {{
	if( m_pPacketHandler ){
		delete m_pPacketHandler;
		m_pPacketHandler = NULL;
		}
	// }}

	/*
	Stop();

	delete m_pSockSendComplete;
	m_pSockSendComplete = NULL;

	// Destroy packet handler. {{
	if( m_pPacketHandler ){
		delete m_pPacketHandler;
		m_pPacketHandler = NULL;
		}
	// }}

	// Destroy growable memory. {{
	if( m_pMem ){
		delete m_pMem;
		m_pMem = NULL;
		}
	// }}

	// Destroy event handler object. {{
	if( m_pPacketHandler ){
		delete m_pPacketHandler;
		m_pPacketHandler = NULL;
		}
	// }}
	*/
	}

///////////////////////////////////////////////////////////
///
///	Public Methods
///
///////////////////////////////////////////////////////////

bool
GameServer::Start(){
	if( m_bStarted ){
		return false; // Already started.
		}

	GameServerConfig* pConfig = LoadConfiguration();
	if( !pConfig ){
		ASSERT(pConfig);
		AfxMessageBox(_T("Error loading configuration file !!!"));
		return false;
		}

	// Initialize logging.
	if( pConfig->m_bLog )
		Logger::InitLogging(pConfig->m_sLogDir, pConfig->m_nMaxLogFileSize);
	
	
	// Dump packets source code in AS3. {{
	int nArrPacketsDump [] = {
		_HandShake,
		_Login,
		_Logout,
		_AcquireTableAccess,
		_PokerTableInfo,					
		_PokerOpenTable,
		_PokerLeaveTable,
		_PokerTableClosed,
		_PokerReserveSeat,
		_PokerCancelReservation,
		_PokerSeatAt,
		_PokerJoinWaitingList,
		_PokerAddChips,
		_PokerPostBlind,
		_PokerShowMuckCards,
		_PokerSetOption,
		_PokerSitOut,
		_PokerSeatStatusChanged,
		_PokerReturnBack,
		_PokerSeatOnlineChanged,
		_PokerSetActive,
		_PokerAction,
		_PokerAct,
		_PokerHandStarted,
		_PokerHandFinished,
		_PokerRoundStarted,
		_PokerChatMessage,
		_PokerSetAvatar,
		_PokerUploadAvatar,
		_PokerDownloadAvatar,
		_PokerOpenRandomTable,
		_PokerGetHandHistoryPacket,
		_PokerGetLobbyInfo,
		_PokerMoneyTransfer,
		_PokerTableClosed,
		_PokerReturnBack
		};
	/*
	for(int i=0; i<sizeof(nArrPacketsDump)/sizeof(int); i++){
		GeneratePacketSourceCode_AS3(nArrPacketsDump[i], _T("D:\\dump"), _T("Poker.Net.Packets"), _T("Poker.Serialization"));
		}*/
	// }}
	
	/*
	_String s;
	GeneratePacketSourceCode_CSharp(_PokerCreateTournament, s);
	GeneratePacketSourceCode_CSharp(_PokerTournamentCustomInfo, s);
	*/

	// Start socket server.
	if( !m_sockServer.Start(&pConfig->m_sockServerOpt.m_listenOpt, this, 
		(pConfig->m_sockServerOpt.m_listenOpt.m_bUseSSL ? pConfig->m_sockServerOpt.m_listenOpt.m_sSSLPemFile.GetBuffer() : NULL), SocketProtocolFactory::GetProtocolByName(_T("websocket1"))) ){
		Logger::CreateLogEntry(_T("Error: starting socket server !!!"));
		return false; // Socket server start error.
		}

	// Start admin socket server.
	if( !m_adminServer.Start(&pConfig->m_adminSocketOpt, this, 
		(pConfig->m_adminSocketOpt.m_bUseSSL ? pConfig->m_adminSocketOpt.m_sSSLPemFile.GetBuffer() : NULL)) ){
		Logger::CreateLogEntry(_T("Error: starting admin socket server !!!"));
		return false; // Socket server start error.
		}

	// Start policy server.
	if( !pConfig->m_policySocketOpt.m_sName.IsEmpty() ){
		CString sReplyXML;
		sReplyXML.Format(_T("<cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"%d\" /></cross-domain-policy>"), pConfig->m_sockServerOpt.m_listenOpt.m_nPort);
		m_policyServer.SetPolicyXML(sReplyXML);
		if( !m_policyServer.Start(&pConfig->m_policySocketOpt, this, NULL) ){
			Logger::CreateLogEntry(_T("Error: starting flash policy socket server !!!"));
			return false; // Socket server start error.
			}
		}

	m_bStarted	= true;
	m_pConfig	= pConfig;
	return true;
	}

bool
GameServer::Stop(){
	if( !m_bStarted )
		return true;

	// Stop socket server.
	m_sockServer.Stop();
	// Stop admin socket server.
	m_adminServer.Stop();
	// Stop policy server.
	m_policyServer.Stop();

	// Delete configuration.
	delete m_pConfig;
	m_pConfig = NULL;
	// Destroy sessions.
	Session::DestroyAllSessions();
	// Destroy connections.
	DestroyAllConnections();
	// Server is stopped.
	m_bStarted = false;
	// Destroy logging.
	Logger::DestroyLogging();
	return true;
	}

///////////////////////////////////////////////////////////
///
///	Protected Methods
///
///////////////////////////////////////////////////////////

ClientConnection*
GameServer::CreateClientConnection(ConnectedSocketInfo* pSockInfo, SocketServer* pSockServer){
	if( !pSockInfo ) return NULL;
	ClientConnection*	pConnNew = new ClientConnection(pSockInfo, this, pSockInfo->m_pSockConn, pSockServer);

	m_lockConnections.Lock();		// Lock
	pConnNew = (ClientConnection*)SafePointer::Retain(pConnNew);		// Retain connection pointer.
	m_arrAcceptedSocks.Add((void*)pSockInfo->m_sock, (void*)pConnNew);
	m_lockConnections.Unlock();	// Unlock 

	OnConnected(pConnNew, pSockInfo);
	return pConnNew;
	}

bool
GameServer::DestroyClientConnection(SOCKET sock, DWORD dwErrorCode){
	if( !sock ) return false;
	bool bRet = true;
	m_lockConnections.Lock(); // Lock
	int nIndex = m_arrAcceptedSocks.FindValue((void*)sock);
	if( nIndex != -1 ){
		ClientConnection* pConn = (ClientConnection*)m_arrAcceptedSocks.GetData(nIndex);
		if( !pConn ) {
			m_arrAcceptedSocks.Delete(nIndex);		// Remove socket from list.
			m_lockConnections.Unlock();				// Unlock
			return true;
			}

		bool bTableConn = pConn->IsTableConnection();
		// Set disconnected state.
		Session* pSess = pConn->GetSession(true);
		pConn->SetConnected(false); 
		OnDisconnected(pConn, dwErrorCode);

		if( pSess ){
			// Make session connectionless.
			if( bTableConn )
				pSess->SetTableConnection(pConn->GetTableId(), pConn->GetTournamentId(), NULL, true);
			else
				pSess->SetConnection(NULL, true);	
			// Release session pointer.
			SafePointer::Release(pSess);
			}

		// Release pointer.
		SafePointer::Release(pConn);				
		// Destroy session.
		if( !bTableConn )
			Session::DestroySession(pSess);
		// Remove conection info.
		m_arrAcceptedSocks.Delete(nIndex);		
		}
	else
		bRet = false;
	m_lockConnections.Unlock();		// Unlock
	return bRet;
	}

Session*
GameServer::CreateOrRestoreSession(ClientConnection* pConn, __int64 nSessionId /*= 0*/){
	// Create connectionless active session.
	if( !pConn ){
		Session* pSess = Session::CreateNewSession(this, pConn, true);
		return pSess;
		}

	// Sessionless connection.
	if( pConn->GetSession(false) == NULL ){
		Session*			pSessRestored	= nSessionId ? Session::RestoreInactiveSession(nSessionId, pConn, true) : NULL;
		Session*			pSessRet		= NULL;
		ClientConnection*	pConnOld		= NULL;

		if( pSessRestored ){
			pSessRestored->SetConnection(pConn, false);
			pConn->SetSession(pSessRestored);
			OnSessionRestored(pSessRestored, pConn);
			pSessRet = pSessRestored;
			}
		else{
			Session* pSess = Session::CreateNewSession(this, pConn, true);
			pSessRet = pSess;
			}
		return pSessRet;
		}
	return NULL;
	}

bool
GameServer::DestroySession(Session* pSess){
	return Session::DestroySession(pSess);
	}

int
GameServer::DestroyAllConnections(){
	m_lockConnections.Lock(); // Lock
	int nResult = 0, nLoop	= 0, nCt = m_arrAcceptedSocks.GetCount();
	while( nLoop < nCt ){
		ClientConnection* pConn = (ClientConnection*)m_arrAcceptedSocks.GetData(nLoop);
		if( pConn ){
			SafePointer::Release(pConn);
			nResult ++;
			}
		nLoop ++;
		}
	m_arrAcceptedSocks		.DeleteAll();
	m_lockConnections.Unlock(); // Unlock
	return nResult;
	}

bool
GameServer::DispatchReceivedData(SocketContext* pSockConn, BYTE* pData, DWORD dwSize, bool bIsFirstData){
	ASSERT(m_pPacketHandler && pData && dwSize > 0);
	if( !pData || !dwSize )
		return false;
	ClientConnection* pConn = (ClientConnection*)pSockConn->GetParam();

	GrowableMemory mem(0, 0, false);
	mem.SetReadonlyBuffer(pData, dwSize);
	if( !pConn )
		return false;

	BasePacket	basePacket;
	int			nOffset				= 0;
	int			nBufferSize			= dwSize;
	int			nSerializedBytes	= 0;

	while( TRUE ){
		if( basePacket.Deserialize(&mem) ){ // Success
			// Handshake must be the only one command accepted while session is not created. 
			if( (basePacket.m_nCode != PacketCodes::_HandShake && basePacket.m_nCode != PacketCodes::_AcquireTableAccess) && pConn->GetSession(false) == NULL ) 
				return false;
			BasePacket* packet	= m_pPacketHandler->CreatePacketByCode(basePacket.m_nCode);
			ASSERT( packet );

#ifndef _DEBUG
		if( packet == NULL )
			return false;
#endif
			// Deserialize received data into known packet.
			if( !mem.SetCurrentOffset(nOffset) )
				return false;
			if( !packet->Deserialize(&mem) )
				return false;

			// Handle received packet.
			OnPacketReceived(pConn, packet);
			delete packet;
		
			nOffset	= mem.GetCurrentOffset();
			if( nOffset == nBufferSize )
				break;
			}
		else
			break;
		}

	return true;
	}

/////////////////////////////////////////////////////////
//
//	Socket server event methods.
//
/////////////////////////////////////////////////////////

void
GameServer::Event_OnConnected(SocketServer* pSockServer, SOCKET sock, ConnectedSocketInfo* pSockInfo){
	if( !sock || !pSockInfo->m_pSockConn ) return;
	ClientConnection* pConn = CreateClientConnection(pSockInfo, pSockServer);
	}

void
GameServer::Event_OnAcceptConnection(SocketServer* pSockServer, SOCKET	sock, const CString sRemoteIp, int nRemotePort, bool& bAccept){
	// Check IP in blacklist existance.
	bAccept = true;
	}

void
GameServer::Event_OnDataReceived(SocketServer* pSockServer, SocketContext* pSockConn, BYTE* pData, DWORD dwSize, bool bIsFirstData){
	// Dispatch received data.
	DispatchReceivedData(pSockConn, pData, dwSize, bIsFirstData);
	}

void
GameServer::Event_OnDisconnected(SocketServer* pSockServer, SocketContext* pSockConn, SOCKET sockDisposed, DWORD dwLastError){
	DestroyClientConnection(sockDisposed, dwLastError);
	}

///////////////////////////////////////////////////////////
///
///	Virtual events.
///
///////////////////////////////////////////////////////////

void
GameServer::OnDisconnected(ClientConnection* pConn, DWORD dwLastError){
	pConn->SetConnected(false); // Disconnected
	}

void
GameServer::OnConnected(ClientConnection* pConn, ConnectedSocketInfo* pSockInfo){
	pConn->SetConnected(true); // Connected
	}

void
GameServer::OnSessionExpired(Session* pSess){}

void
GameServer::OnSessionRestored(Session* pSess, ClientConnection* pConn){}

void
GameServer::OnPacketReceived(ClientConnection* pConn, BasePacket* packet){
	HandlePacket(packet, pConn);
	}

bool
GameServer::SendPacketByConnectionId(__int64 nConnectionId, BasePacket* p, bool bDontCheckSessionId /*= false*/){
	return false;
	}

bool
GameServer::SendPacket(ClientConnection* pConn, BasePacket* p, bool bDontCheckSessionId /*= false*/){
	if( !pConn ) return false;
	ASSERT(p);
	Session* pSess		= pConn->GetSession(false);
	if( pSess ){
		p->m_nSessionId = pSess->GetSessionId();	// Session Id
		}
	else{
		if( !bDontCheckSessionId )
			return false;
		p->m_nSessionId = 0;						// Session Id
		}
//	m_lockSendPacket.Lock(); // Lock
	pConn->SetSendingStatus(true); // Set connection sending status

	bool bRet			= false;
	m_serializeMem.ZeroUsedMemory();
	p->m_nPacketSize	= 0xFFFF;
	if( p->Serialize(&m_serializeMem, &m_arrVariableOffset) ){
		// Update packet size value in buffer.{{
		void*	lpBuffer								= m_serializeMem.GetBufferPtr();
		int		nOffsetPacketSize						= (int)m_arrVariableOffset.GetData(0);
		*(int*)(&((BYTE*)lpBuffer)[nOffsetPacketSize])	= m_serializeMem.GetUsedBufferSize();
		// }}
		bRet = pConn->Send(&m_serializeMem);
		}
	pConn->SetSendingStatus(false);
//	m_lockSendPacket.Unlock(); // Unlock
	return bRet;
	}

///////////////////////////////////////////////////////////
///
///	Event listener methods.
///
///////////////////////////////////////////////////////////

void
GameServer::OnHandShake(HandShakePacket* p, void* pParam){
	ClientConnection*	pConn	= (ClientConnection*)pParam;
	if( p->m_nProtocolVer != PROTOCOL_VER ){
		p->m_nErrorCode			= ES_ERROR_PROTOCOL_VERSION_MISMATCH;
		p->m_nSessionId			= 0;
		p->m_sError				= _T("");
		SendPacket(pConn, p, true); // Reply packet.
		return;
		}
	
	Session*			pSess	= CreateOrRestoreSession(pConn, p->m_nSessionId);
	if( !pSess ) return;
	p->m_nSessionId				= pSess->GetSessionId();
	p->m_sError					= _T("");
	p->m_nErrorCode				= 0; // Success

	SendPacket(pConn, p, true);		// Reply packet.
	SafePointer::Release(pSess);	// Release session.
	}

void
GameServer::OnAcquireTableAccess(AcquireTableAccessPacket* p, void* pParam){
	if( p->m_nSessionId != 0 || p->m_nSessionIdAcquire == 0 || p->m_nTableId <= 0 || p->m_nTournamentId < 0 )
		return;

	ClientConnection*	pConn		= (ClientConnection*)pParam;
	Session*			pSess		= Session::GetSessionById(p->m_nSessionIdAcquire, true);
	bool				bSuccess	= false;
	if( pConn->IsTableConnection() )
		return;

	if( pSess && pSess->IsActive() ){
		int	 nUserId = pSess->GetUserId();
		pSess->SetTableConnection(p->m_nTableId, p->m_nTournamentId, pConn, true);
		pConn->SetTableConnection(p->m_nTableId, p->m_nTournamentId);

		if( nUserId > 0 )
			pConn->SetAuthorized(true);

		p->m_bAvatarIsBlocked	= pSess->IsAvatarBlocked();
		p->m_bChatIsBlocked		= pSess->IsChatBlocked();
		p->m_nUserId			= nUserId;
		bSuccess				= true;
		SafePointer::Release(pSess);
		}

	// Send reply.
	if( bSuccess ){
		//pConn->SetAuthorized(
		pConn->SetSession(pSess);
		p->m_nErrorCode = ES_ERROR_SUCCESS;
		}
	else{
		p->m_nUserId	= 0;
		p->m_nErrorCode = ES_ERROR_UNKNOWN;
		}
	SendPacket(pConn, p, true);
	}

void	
GameServer::OnLogin(LoginPacket* p, void* pParam){}

void
GameServer::OnLogout(LogoutPacket* p, void* pParam){}

void
GameServer::OnRegisterUser(RegisterUserPacket* p, void* pParam){
	ClientConnection* pConn = (ClientConnection*)pParam;
	if( p->m_nSessionId == 0 ) return; // Handshake first.
	}

///////////////////////////////////////////////////////////
///
///	Packet source code generation.
///
///////////////////////////////////////////////////////////

bool
GameServer::GeneratePacketSourceCode_CSharp(int packetCode, _String& strSource){
	BasePacket* packet	= m_pPacketHandler->CreatePacketByCode(packetCode);
	if( !packet ) return false;
	bool bRet = packet->GenerateSourceCodeInCSharp(strSource, true);
	delete packet;
	return bRet;
	}

bool
GameServer::GeneratePacketSourceCode_AS3(int packetCode, const _String sDumpDir, const TCHAR* pszPackage, const TCHAR* pszSerializeBasePackage){
	BasePacket* packet	= m_pPacketHandler->CreatePacketByCode(packetCode);
	if( !packet ) 
		return false;

	_String sSaveDir = sDumpDir;
	Path::BuildFullPath(sSaveDir);

	StringToStringArray arrGenPackages;
	if( !packet->GenerateSourceCodeInAS3(arrGenPackages, pszPackage, pszSerializeBasePackage) ){
		delete packet;
		return false;
		}

	if( sSaveDir.GetAt(sSaveDir.GetLength() - 1) != '\\' )
		sSaveDir += _T("\\");

	_String sPackage, sSourceCode, sFile;
	for(int i=0; i<arrGenPackages.GetCount(); i++){
		sPackage	= arrGenPackages.GetKey(i);
		sSourceCode = arrGenPackages.GetValueByIndex(i);
		sFile.Format(_T("%s%s.as"),sSaveDir.GetBuffer(), sPackage.GetBuffer());
		Path::SaveAsFile(sSourceCode.GetBuffer(), sSourceCode.GetLength(), sFile);
		}
	return true;
	}

bool
GameServer::GeneratePacketSourceCode_ObjectiveC(int packetCode, _String& strSource){
	return false;
	}

