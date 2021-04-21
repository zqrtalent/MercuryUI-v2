#include "stdafx.h"
#include "ESSocket.h"
#include "ESSocketMan.h"

/////////////////////////////////////////
//	class: CSocketListen
//	programmer:	ZqrTalent
//	Date:	30/6/2009
/////////////////////////////////////////

struct WaitForDataThreadStruct{
	ESSocket*			pSocket;
	SOCKET				sock;
	CCriticalSection*	pCritSection;
	HANDLE				hEventStop;
	BOOL*				pThreadActive;
	};

UINT 
WaitForDataThread(WaitForDataThreadStruct* pData){
	CCriticalSection* pSect = pData->pCritSection;
	ASSERT( pSect );

	pSect->Lock();		// Lock
	CString		sError;
	timeval		timeout		= {5000, 0};
	ESSocket*	pSock		= pData->pSocket;
	SOCKET		sock		= pData->sock;
	SOCKET		sockAccept	= 0;
	HANDLE		hEvent		= pData->hEventStop;
	BOOL*		bThread		= pData->pThreadActive;
	fd_set		sel_recv;
	*bThread				= TRUE;
	pSect->Unlock();	// Unlock

	while( TRUE ){ 
		// Stop thread when event signaled.
		if( WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0 ){
			*bThread = FALSE;
			delete pData;
			return 0L;
			}

		sel_recv.fd_count = 0;
		// add listen sock to select list.
		FD_SET(sock, &sel_recv);

		int nSockNum = select(0, &sel_recv, NULL, NULL, &timeout);
		if( nSockNum == SOCKET_ERROR )
			continue;

		// see if we have a some data to read.
		if( FD_ISSET(sock, &sel_recv) ){
			// Determine read data size in bytes. {{
			u_long lReadSz = 0;
			ioctlsocket(sock, FIONREAD, &lReadSz);
			if( lReadSz <= 0 )
				continue;
			// }}

			// Receive binary data. {{
			BYTE* pDataRecv = new BYTE[lReadSz];
			if( recv(sock, (char*)pDataRecv, lReadSz, 0) != lReadSz ){
				delete [] pDataRecv; // Fail to read full of recevied data.
				continue;
				}
			// }}

			// Call receive data notify. {{
			ESSocketMan::GetInstance()->OnReceiveData(pSock, pDataRecv, lReadSz);
			delete [] pDataRecv;
			// }}
			}
		}
	
	*bThread = FALSE;
	delete pData;
	return 0L;
	}

struct WaitForConnectionThreadStruct{
	ESSocket*			pSocket;
	SOCKET				sock;
	CCriticalSection*	pCritSection;
	HANDLE				hEventStop;
	BOOL*				pThreadActive;
	};

UINT
WaitForConnectionThread(WaitForConnectionThreadStruct* pData){
	CCriticalSection* pSect = pData->pCritSection;
	ASSERT( pSect );

	pSect->Lock();		// Lock
	CString		sError;
	timeval		timeout		= {5, 0};
	ESSocket*	pSock		= pData->pSocket;
	SOCKET		sock		= pData->sock;
	SOCKET		sockAccept	= 0;
	HANDLE		hEvent		= pData->hEventStop;
	BOOL*		bThread		= pData->pThreadActive;
	fd_set		sel_recv;
	*bThread				= TRUE;
	pSect->Unlock();	// Unlock

	while( TRUE ){ 
		// Stop thread when event signaled.
		if( WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0 ){
			*bThread = FALSE;
			delete pData;
			return 0L;
			}

		sel_recv.fd_count = 0;
		// add listen sock to select list.
		FD_SET(sock, &sel_recv);

		int nSockNum = select(0, &sel_recv, NULL, NULL, &timeout);
		if( nSockNum == SOCKET_ERROR ){
			continue;
			}

		// see if we have a new connection request.
		if( FD_ISSET(sock, &sel_recv) ){
			// Accept connection. #################
			pSect->Lock();	// Lock

			// Accept connection socket. #########
			struct	sockaddr_in	sa;
			int		sasize	= sizeof(struct sockaddr_in);
			sockAccept		= accept(sock, (struct sockaddr * )&sa, &sasize);
			if( sasize != sizeof(struct sockaddr_in)){
				sockAccept = 0;
				pSect->Unlock();	// Unlock
				continue;
				}
			// ###################################

			// Set every socket to non-blocking.
			int		option	= TRUE;
			int		err		= ioctlsocket((int)sockAccept, FIONBIO, (u_long *)&option);
			// #################################

			// Ask for accept recently connected.
			if( !ESSocketMan::GetInstance()->OnAccept(pSock, sockAccept) ){
				closesocket(sockAccept);
				sockAccept = 0;
				pSect->Unlock();	// Unlock
				continue;
				}
			// #################################

			// Create accept wait event. #######
			//HANDLE eventAcceptWait = ::CreateEvent(NULL, FALSE, FALSE, "AcceptWait");
			//if( !eventAcceptWait ){
			//	closesocket(sockAccept);
			//	sockAccept = 0;
			//	continue;
			//	}
			// #################################

			/*
			m_bAcceptSocket = FALSE;
			if( AfxGetApp()->PostThreadMessage(m_nAcceptNotify, (WPARAM)&m_bAcceptSocket, (LPARAM)this) ){
				if( WaitForSingleObject(m_hEventSync, m_nAcceptWaitTime) != WAIT_OBJECT_0 ) // Time out.
					m_bAcceptSocket = FALSE;
				}*/

			// Close event. ###############
			//::CloseHandle(eventAcceptWait);
			// ############################

			/*
			if( !m_bAcceptSocket ){	// Not accepted.
				closesocket(m_acceptSock);
				SetLastError("Socket not accepted !!!");
				return FALSE;
				}
			else{					// Accepted.
				}
			*/
			pSect->Unlock();	// Unlock
			// ####################################
			}
		}
	
	*bThread = FALSE;
	delete pData;
	return 0L;
	}

ESSocket::ESSocket(){
	m_sock				= 0;			// Listen socket.
	m_sockStat			= Closed;		// Socket status.
	m_nMaxPending		= 10;			// Pending connections count.
	m_hThread			= 0;			// Handle to the listen thread.
	m_hEventStop		= 0;			// Thread stop event.
	m_dwThreadId		= 0L;			// Thread Identifier.
	m_bThread			= FALSE;		// Thread is working.
	m_nAcceptWaitTime	= 5000;			// Accept wait time.

	m_procReceiveData		= NULL;
	m_lpClassObjReceiveData	= NULL;
	m_procAccept			= NULL;
	m_lpClassObjAccept		= NULL;
	m_procDisconnected		= NULL;
	m_lpClassObjDisconnected= NULL;
	}

ESSocket::~ESSocket(){
	// Destroy everything.
	Destroy();
	}

BOOL
ESSocket::Connect(short shPort, const CString sIpAddress){
	if( !IsClosed() ) return FALSE;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( sock == INVALID_SOCKET ) {
		return FALSE;
		}

	sockaddr_in clientService;
	clientService.sin_family		= AF_INET;
	clientService.sin_addr.s_addr	= inet_addr(sIpAddress);
	clientService.sin_port			= htons(shPort);

	int nResult = connect(sock, (SOCKADDR *) & clientService, sizeof (clientService));
	if( nResult == SOCKET_ERROR) {
		closesocket(sock);
		return FALSE;
		}

	m_sock		= sock;
	m_sockStat	= Connected;
	
	CString sEventName;
	sEventName.Format("WaitForDataThread_STOP%d", (int)m_sock);

	WaitForDataThreadStruct* pThreadData = new WaitForDataThreadStruct;
	pThreadData->pSocket		= this;
	pThreadData->sock			= m_sock;
	pThreadData->pCritSection	= &m_critSection;
	pThreadData->pThreadActive	= &m_bThread;
	m_hEventStop				= ::CreateEvent	(NULL, FALSE, FALSE, sEventName.GetBuffer()); 
	pThreadData->hEventStop		= m_hEventStop;
	m_hThread					= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)WaitForDataThread, pThreadData, 0L, &m_dwThreadId);
	return TRUE;
	}

BOOL
ESSocket::Bind(short shPort, CString sIpAddress /*= _T("")*/){
	struct sockaddr_in	wi_sin;
	int					error;

	// Create the "listen" socket.
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( m_sock == INVALID_SOCKET ){
		m_sLastError = _T("Error create socket.");
		return FALSE;
		}

	wi_sin.sin_family		= AF_INET;
	wi_sin.sin_addr.s_addr	= (!sIpAddress.IsEmpty()) ? inet_addr(sIpAddress.GetBuffer()) : INADDR_ANY;
	wi_sin.sin_port			= htons(shPort);
	error					= bind(m_sock, (struct sockaddr*)&wi_sin, sizeof(sockaddr_in));
	if( error != 0 ){
		error = WSAGetLastError();
		if( error == WSAEADDRNOTAVAIL )
			m_sLastError = _T("bind error. Address not available !!!");
		else
		if( error == WSAEADDRINUSE )
			m_sLastError = _T("bind error. Address in use !!!");
		else
			m_sLastError.Format(_T("Error %d bind !!!"), error);
		return FALSE;
		}

	error = listen(m_sock, m_nMaxPending);
	if( error != 0 ){
		error = WSAGetLastError();
		m_sLastError.Format("Error %d listen", error);
		return FALSE;
		} 

	// Save socket state.
	m_sockStat					= Binded;

	WaitForConnectionThreadStruct* pThreadData = new WaitForConnectionThreadStruct;
	pThreadData->pSocket		= this;
	pThreadData->sock			= m_sock;
	pThreadData->pCritSection	= &m_critSection;
	pThreadData->pThreadActive	= &m_bThread;

	m_hEventStop				= ::CreateEvent	(NULL, FALSE, FALSE, _T("WaitForConnectionThread_STOP")); 
	pThreadData->hEventStop		= m_hEventStop;
	m_hThread					= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)WaitForConnectionThread, pThreadData, 0L, &m_dwThreadId);
	return TRUE;
	}

BOOL
ESSocket::Accept(SOCKET sockAccept){ 
	if( m_sockStat != Closed )
		return FALSE;

	m_sock		= sockAccept;
	m_sockStat	= Connected;
	m_bThread	= FALSE;

	CString sEventName;
	sEventName.Format("WaitForDataThread_STOP%d", (int)m_sock);

	WaitForDataThreadStruct* pThreadData = new WaitForDataThreadStruct;
	pThreadData->pSocket		= this;
	pThreadData->sock			= m_sock;
	pThreadData->pCritSection	= &m_critSection;
	pThreadData->pThreadActive	= &m_bThread;
	m_hEventStop				= ::CreateEvent	(NULL, FALSE, FALSE, sEventName.GetBuffer()); 
	pThreadData->hEventStop		= m_hEventStop;
	m_hThread					= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)WaitForDataThread, pThreadData, 0L, &m_dwThreadId);
	return TRUE;
	}

int
ESSocket::Send(BYTE* lpBytesSend, UINT nSize){
	if( !IsConnected() ) return 0;
	int nBytesSent = send(m_sock, (char*)lpBytesSend, (int)nSize, 0);
	return nBytesSent;
	}

void
ESSocket::Disconect(){
	// Disconnect socket. {{
	if( m_sockStat == Connected ){
		closesocket(m_sock);
		m_sock = 0;
		m_sockStat = Closed;
		}
	// }}
	}

void
ESSocket::Destroy(){
	// Destroy thread. {{
	if( m_hThread ){
		::SetEvent			(m_hEventStop);
		WaitForSingleObject	(m_hThread, INFINITE);
		CloseHandle			(m_hThread);
		CloseHandle			(m_hEventStop);

		m_hThread			= NULL;
		m_hEventStop		= NULL;
		m_dwThreadId		= 0L;
		}
	// }}

	// Close socket. {{
	if( m_sockStat != Closed ){
		closesocket(m_sock);
		m_sock = 0;
		m_sockStat = Closed;
		}
	// }}
	}

void
ESSocket::SetReceiveDataNotifier(ReceiveDataProc proc, void* pClassObject){
	m_procReceiveData		= proc;
	m_lpClassObjReceiveData	= pClassObject;
	}

void
ESSocket::SetAcceptNotifier(AcceptProc proc, void* pClassObject){
	m_procAccept			= proc;
	m_lpClassObjAccept		= pClassObject;
	}

void
ESSocket::SetDisconnectedNotifier(DisconnectedProc proc, void* pClassObject){
	m_procDisconnected		= proc;
	m_lpClassObjDisconnected= pClassObject;
	}

bool
ESSocket::Call_ReceiveDataNotifier(ESSocket* pSockFrom, BYTE* lpData, UINT nSize){
	if( m_procReceiveData != NULL ){
		ReceiveDataProc proc = m_procReceiveData;
		void* obj = m_lpClassObjReceiveData;
		__asm{
			push ecx;
			mov eax, proc;
			mov ecx, obj;
			push nSize;
			push lpData;
			push pSockFrom;
			call eax;
			pop ecx;
			};

		return true;
		}
	return false;
	}

bool
ESSocket::Call_AcceptNotifier(ESSocket* pAccepted, BOOL& bAccept){
	if( m_procAccept != NULL ){
		AcceptProc proc = m_procAccept;
		void* obj = m_lpClassObjAccept;
		__asm{
			push ecx;
			mov eax, dword ptr proc;
			mov ecx, obj;
			push bAccept;
			push pAccepted;
			call eax;
			pop ecx;
			};
		return true;
		}
	return false;
	}

bool
ESSocket::Call_DisconnectedNotifier(ESSocket* pSock){
	if( m_procDisconnected != NULL ){
		DisconnectedProc proc = m_procDisconnected;
		void* obj = m_lpClassObjDisconnected;
		__asm{
			push ecx;
			mov eax, proc;
			mov ecx, obj;
			push pSock;
			call eax;
			pop ecx;
			};
		return true;
		}
	return false;
	}
