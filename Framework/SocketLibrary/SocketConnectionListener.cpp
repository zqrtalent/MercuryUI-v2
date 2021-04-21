#include "stdafx.h"
#include "SocketConnectionListener.h"
#include "SocketServerContext.h"
#include "SocketServer.h"
#include "SocketIOCP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SocketConnectionListener.
//
/////////////////////////////////////////////////////////////////////

SocketConnectionListener::SocketConnectionListener(SockListenOptions* pListenerInfo, SocketServer* pSockServer){
	ASSERT(pListenerInfo);
	if( pListenerInfo ){
		m_listenerInfo.ZeroInit();
		pListenerInfo->Copy(&m_listenerInfo);
		}
	
	m_hEventListeningClosed	= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_bStarted				= false;
	m_pSockServer			= pSockServer;
	m_pSSLContext			= NULL;
	m_sockListen			= INVALID_SOCKET;
	::InitializeCriticalSection(&m_lock);
	} 

SocketConnectionListener::~SocketConnectionListener(){
	if( m_bStarted )
		Stop();

	CloseHandle(m_hEventListeningClosed);
	m_hEventListeningClosed = NULL;
	// Free critical section.
	::DeleteCriticalSection(&m_lock);
	}

bool
SocketConnectionListener::Start(SSL_CTX* pSSLContext /*= NULL*/){
	if( m_bStarted ) return false;
	int		nRetCode = 0;
	CString sError;

	// Create the socket.
    SOCKET sockListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if( sockListen == INVALID_SOCKET )
        return false;

	// Prepare address to bind.
	char* szIpAddress		= m_listenerInfo.m_sIpListen.GetBuffer();
	struct	sockaddr_in	wi_sin;
	wi_sin.sin_family		= AF_INET;
	wi_sin.sin_port			= htons((short)m_listenerInfo.m_nPort);
	if( !strcmp(szIpAddress, _T("localhost")) || !strcmp(szIpAddress, _T("127.0.0.1")) )
		wi_sin.sin_addr.s_addr	= INADDR_LOOPBACK;
	else
		wi_sin.sin_addr.s_addr	= (!m_listenerInfo.m_sIpListen.IsEmpty()) ? inet_addr(szIpAddress) : INADDR_ANY;

	// Associate socket handle.
	SocketIOCP::GetInstance()->AssociateSocketHandle(sockListen, (ULONG_PTR)this);

    // Bind the socket.
    nRetCode = bind(sockListen, (LPSOCKADDR)&wi_sin, sizeof(struct sockaddr));
    if( nRetCode == SOCKET_ERROR ){
		int nErrorCode = WSAGetLastError();
		if( nErrorCode == WSAEADDRNOTAVAIL )
			sError = _T("bind error: address not available !!!");
		else
		if( nErrorCode == WSAEADDRINUSE )
			sError = _T("bind error: address in use !!!");
		else
			sError.Format(_T("Error %d bind !!!"), nErrorCode);
        closesocket(sockListen);
        return false;
		}

	listen(sockListen, m_listenerInfo.m_nListenBackLog);
	::EnterCriticalSection(&m_lock); // Lock
	m_pSSLContext		= pSSLContext;
	m_sockListen		= sockListen;
	m_bStarted			= true;
	m_pSocketAccepting = SocketContext::BeginAccept(this, m_pSockServer, m_sockListen, m_pSSLContext, m_listenerInfo.m_nSendBufferSize, m_listenerInfo.m_nReceiveBufferSize,
		m_listenerInfo.m_bKeepAlive, m_listenerInfo.m_nKeepAliveTime, m_listenerInfo.m_nKeepAliveInterval);
	::LeaveCriticalSection(&m_lock); // Unlock
	return true;
	}

bool
SocketConnectionListener::Stop(){
	if( !m_bStarted ) return true;
	::EnterCriticalSection(&m_lock); // Lock
	if( m_sockListen != INVALID_SOCKET ){
		closesocket(m_sockListen);
		m_sockListen	= INVALID_SOCKET;
		m_bStarted		= false;
		}

	if( m_pSocketAccepting ){
		bool bStarted = SocketIOCP::GetInstance()->IsStarted();
		ResetEvent(m_hEventListeningClosed);
		m_pSocketAccepting->SafeClose();
		if( bStarted ){
			::LeaveCriticalSection(&m_lock); // Unlock
			::WaitForSingleObject(m_hEventListeningClosed, INFINITE);
			delete m_pSocketAccepting;
			m_pSocketAccepting = NULL;
			return true;
			}
		delete m_pSocketAccepting;
		m_pSocketAccepting = NULL;
		}
	::LeaveCriticalSection(&m_lock); // Unlock
	return true;
	}

bool
SocketConnectionListener::OnAccept(){
	::EnterCriticalSection(&m_lock); // Lock
	if( !m_pSocketAccepting ){
		::LeaveCriticalSection(&m_lock); // Unlock
		return false;
		}

	// Set socket IO event handler.
	m_pSocketAccepting->SetIOEventHandler(m_pSockServer);
	// Set socket keep alive options.
	SocketContext::SetKeepAliveOption(m_pSocketAccepting->GetSocket(), m_listenerInfo.m_bKeepAlive, m_listenerInfo.m_nKeepAliveTime, m_listenerInfo.m_nKeepAliveInterval);

	// Accept sock connection.
	if( m_pSocketAccepting->OnAccept() ){
		// Begin receive bytes.
		m_pSocketAccepting->ReceiveBytes();
		}

	// Start acception next one.
	m_pSocketAccepting = SocketContext::BeginAccept(this, m_pSockServer, m_sockListen, m_pSSLContext, m_listenerInfo.m_nSendBufferSize, m_listenerInfo.m_nReceiveBufferSize,
		m_listenerInfo.m_bKeepAlive, m_listenerInfo.m_nKeepAliveTime, m_listenerInfo.m_nKeepAliveInterval);
	::LeaveCriticalSection(&m_lock); // Unlock
	return true;
	}

void
SocketConnectionListener::OnDisconnect(DWORD dwLastError){
	// Listening socket closed.
	// Signal event.
	::EnterCriticalSection(&m_lock); // Lock
	::SetEvent(m_hEventListeningClosed);
	::LeaveCriticalSection(&m_lock); // Unlock
	}