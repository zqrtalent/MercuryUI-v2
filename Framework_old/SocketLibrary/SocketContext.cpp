#include "SocketContext.h"

/////////////////////////////////////////////////////////////////////
//
//	SocketContext.
//
/////////////////////////////////////////////////////////////////////

SocketContext::SocketContext(UINT nSendBuffSize /*= 8192*/, UINT nReceiveBuffSize /*= 8192*/) :
	m_overlSend(SockIOType::IOSend), m_overlRecv(SockIOType::IORecv), m_overlConn(SockIOType::IOConnect){
	m_sock							= INVALID_SOCKET;
	m_pEventHandler					= NULL;
	m_pParam						= NULL;
	m_dwBytesSent					= 0L;
	m_dwBytesReceived				= 0L;
	m_pSockIOCP						= SocketIOCP::GetInstance();
	m_status						= SocketStatus::SockStatus_None;
	m_nLastError					= 0;
	m_nIOWorkersReferenceCounter	= 0;
	m_hEventConnected				= NULL;
	m_hEventClosed					= NULL;
	m_bAutoDeleteOnDisconnect		= false;

	m_nSendBufferSize				= nSendBuffSize;
	m_nRecvBufferSize				= nReceiveBuffSize;
	m_buffRecv.len					= m_nRecvBufferSize;
	m_buffRecv.buf					= (CHAR*)new BYTE[m_buffRecv.len];
	m_buffSend.len					= m_nSendBufferSize;
	m_buffSend.buf					= (CHAR*)new BYTE[m_buffSend.len];

	m_pSSL							= NULL;
	m_BIORecv						= NULL;
	m_BIOSend						= NULL;
	m_buffSendSSL.buf				= NULL;
	m_buffSendSSL.len				= 0;
	m_buffRecvSSL.buf				= NULL;
	m_buffRecvSSL.len				= 0;
	} 

SocketContext::~SocketContext(){
	if( m_sock != INVALID_SOCKET )
		Close(NULL);

	// Delete memory buffers.
	WSABUF* pArrBuf[] = {&m_buffRecv, &m_buffSend, &m_buffSendSSL, &m_buffRecvSSL};
	for(int i=0; i<sizeof(pArrBuf)/sizeof(WSABUF*); i++){
		if( pArrBuf[i]->len > 0 && pArrBuf[i]->buf != NULL ){
			delete [] pArrBuf[i]->buf;
			pArrBuf[i]->len = 0;
			pArrBuf[i]->buf = NULL;
			}
		}
	}

bool
SocketContext::Connect(const TCHAR* pszIp, int nPort, bool bKeepAlive /*= false*/, int nKeepAliveTime /*= 0*/, int nKeepAliveInterval /*= 0*/, SOCKET* pCopySocket /*= NULL*/, HANDLE hConnectEvent /*= NULL*/){
	if( m_sock != INVALID_SOCKET && (m_status == SocketStatus::SockStatus_None || m_status == SockStatus_Closed) )
		return false;

	// Create the socket.
	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if( sock == INVALID_SOCKET )
		return false;

	if( pCopySocket )
		*pCopySocket = sock;

	// Set the socket options. {{
	const int dontLinger	= 1; 
	const int linger		= 1; 
	const int one			= 1;
	// Dont wait for data when closesocket.
	setsockopt(sock, SOL_SOCKET, SO_DONTLINGER, (char *)&dontLinger, sizeof(int));
//	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof(int));
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&one, sizeof(int));

	// getsockname and getpeername is valid after setting that option.
	setsockopt(sock, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
	// Set keep-alive.
	SetKeepAliveOption(sock, bKeepAlive, nKeepAliveTime, nKeepAliveInterval);

	SocketIOCP* pIOCP = SocketIOCP::GetInstance();
	if( !pIOCP->IsStarted() )
		pIOCP->Start();
	pIOCP->AssociateSocketHandle(sock, (ULONG_PTR)this);

	addrinfo hints				= {0};
	hints.ai_family				= PF_INET;
	hints.ai_protocol			= IPPROTO_TCP;
	hints.ai_socktype			= SOCK_STREAM;
	hints.ai_flags				= AI_PASSIVE; // Allow socket to bind.

	sockaddr_in sin_remote;
	sin_remote.sin_family		= AF_INET;
	sin_remote.sin_addr.s_addr	= inet_addr((const char*)pszIp);
	sin_remote.sin_port			= htons(nPort);

	addrinfo* paddrinfo = nullptr;
	getaddrinfo(0, _T(""), &hints, &paddrinfo);
	if( paddrinfo != NULL ){
		bind(sock, paddrinfo->ai_addr, paddrinfo->ai_addrlen);
		freeaddrinfo(paddrinfo);
		paddrinfo = nullptr;
		}

	m_hEventConnected = hConnectEvent;
	if( m_hEventConnected )
		::ResetEvent(m_hEventConnected);

	LPFN_CONNECTEX pfn_ConnectEx = NULL;
	if( !pfn_ConnectEx ){
		DWORD	dwRetBytes	= 0;
		GUID	guid		= WSAID_CONNECTEX;
		WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, (void*)&guid, sizeof(guid), (void*)&pfn_ConnectEx, sizeof(pfn_ConnectEx), &dwRetBytes, NULL, NULL);
		}

	bool bRet	= true;
	m_status	= SockStatus_Connecting;
	m_sock		= sock;

	(*pfn_ConnectEx)(sock, (sockaddr*)&sin_remote, sizeof(sockaddr_storage), 0, 0, 0, &m_overlConn.m_ol);
	m_overlConn.m_dwLastError = WSAGetLastError();
	if( m_overlConn.m_dwLastError != WSA_IO_PENDING ){
		m_status &= ~SockStatus_Connecting;
		Close(NULL);
		bRet = false;
		}
	return bRet;
	}

bool
SocketContext::ConnectSync(const TCHAR* pszIp, int nPort, bool bKeepAlive, int nKeepAliveTime, int nKeepAliveInterval, SOCKET* pCopySocket){
	return false;
	}

bool
SocketContext::Send(BYTE* lpData, int nSize){
	bool bRet	= false;
	if( m_sock == INVALID_SOCKET || (m_status&SockStatus_Connected) != SockStatus_Connected )
		return false;
	bRet = PushSendData(lpData, nSize, false);
	return bRet;
	}

bool
SocketContext::Close(HANDLE hEventClosed){
	m_lock.Lock(); // Lock
	bool	bSocketIOCP = (m_pSockIOCP && m_pSockIOCP->IsStarted());
	if( (m_status&(SockStatus_Accepting/*|SockStatus_Connecting*/)) )
		m_status = SockStatus_Closed;
	else{
		if( m_nIOWorkersReferenceCounter > 0 || !(m_status&SockStatus_Closed) )
			m_status |= SockStatus_Closing;
		else
			m_status = SockStatus_Closed;
		}

	int nSockError = 0;
	if( m_sock != INVALID_SOCKET ){
		// Finalize socket shutdown.
		if( m_status&SockStatus_Closed ){
			::CancelIo		((HANDLE)m_sock);
			::closesocket	(m_sock);
			m_sock = INVALID_SOCKET;
			}
		else{
			// Send shutdown event.
			if( shutdown(m_sock, SD_BOTH) == SOCKET_ERROR ){
				nSockError	= WSAGetLastError();
				closesocket(m_sock);
				m_sock		= INVALID_SOCKET;
				}
			}
		}

	if( bSocketIOCP && (m_status&SockStatus_Closed) != SockStatus_Closed ){
		m_hEventClosed = hEventClosed;
		m_lock.Unlock(); // Unlock
		return false; // Wait for Disconnect method calling from socket IOCP
		}

	// Clean up SSL attributes.
	if( m_pSSL ){
		SSL_shutdown(m_pSSL);
		SSL_free	(m_pSSL);
		m_pSSL		= nullptr;
		m_BIORecv	= m_BIOSend	= nullptr;
		}
	
	// Free memory buffers.
	for(int i=0; i<m_listSendData.GetCount(); i++){
		SendDataBuffer* pBuf = m_listSendData.GetAt(i);
		if( pBuf ){
			delete [] pBuf->_pszBuffer;
			delete pBuf;
			}
		}

	// Free send/receive memory buffers.
	m_listSendData.RemoveAll	(false);
	m_receivedData.SetBufferSize(0);

	HANDLE hEventConnected	= m_hEventConnected;
	m_hEventConnected		= NULL;
	m_hEventClosed			= NULL;
	m_lock.Unlock(); // Unlock

	m_dwBytesSent			= 0L;
	m_dwBytesReceived		= 0L;

	// Signal connected event.
	if( hEventConnected )
		SetEvent(hEventConnected);
	// Signal closed event.
	if( hEventClosed )
		SetEvent(hEventClosed);
	return true;
	}

void
SocketContext::SafeClose(int nWaitMS /*= -1*/){
	HANDLE hEventStopped = CreateEvent(NULL, TRUE, FALSE, NULL);
	if( !Close(hEventStopped) )
		::WaitForSingleObject(hEventStopped, (nWaitMS == -1) ? INFINITE : nWaitMS);
	::CloseHandle(hEventStopped);
	}

int
SocketContext::GetLocalPort(){
	if( (m_status&SockStatus_Connected) != SockStatus_Connected )
		return 0;
	sockaddr addr_local;
	int nLen = sizeof(sockaddr);
	getsockname(m_sock, &addr_local, &nLen);
	return htons(*(short*)&addr_local.sa_data[0]);
	}

int
SocketContext::GetRemotePort(){
	if( (m_status&SockStatus_Connected) != SockStatus_Connected )
		return 0;
	sockaddr addr_remote;
	int nLen = sizeof(sockaddr);
	getpeername(m_sock, &addr_remote, &nLen);
	return htons(*(short*)&addr_remote.sa_data[0]);
	}

bool
SocketContext::GetLocalIp(char* pszIp, int nLen, DWORD* pIpAsDWORD /*= NULL*/){
	if( !pszIp || nLen <= 0 || (m_status&SockStatus_Connected) != SockStatus_Connected )
		return false;
	sockaddr addr;
	int nAddrLen = sizeof(sockaddr);
	getsockname(m_sock, &addr, &nAddrLen);

	BYTE* arrIp = (BYTE*)&addr.sa_data[2];
	sprintf_s(pszIp, nLen, _T("%d.%d.%d.%d"), arrIp[0], arrIp[1], arrIp[2], arrIp[3]);
	if( pIpAsDWORD )
		*pIpAsDWORD = ((((DWORD)arrIp[0]) << 24) & 0xFF000000) | ((((DWORD)arrIp[1]) << 16) & 0x00FF0000) | ((((DWORD)arrIp[2]) << 8) & 0x0000FF00) | (((DWORD)arrIp[3]) & 0x000000FF);
	return true;
	}

bool
SocketContext::GetRemoteIp(char* pszIp, int nLen, DWORD* pIpAsDWORD /*= NULL*/){
	if( !pszIp || nLen <= 0 || (m_status&SockStatus_Connected) != SockStatus_Connected )
		return false;
	sockaddr addr;
	int nAddrLen = sizeof(sockaddr);
	getpeername(m_sock, &addr, &nAddrLen);

	BYTE* arrIp = (BYTE*)&addr.sa_data[2];
	sprintf_s(pszIp, nLen, _T("%d.%d.%d.%d"), arrIp[0], arrIp[1], arrIp[2], arrIp[3]);

	if( pIpAsDWORD )
		*pIpAsDWORD = ((((DWORD)arrIp[0]) << 24) & 0xFF000000) | ((((DWORD)arrIp[1]) << 16) & 0x00FF0000) | ((((DWORD)arrIp[2]) << 8) & 0x0000FF00) | (((DWORD)arrIp[3]) & 0x000000FF);
	return true;
	}

bool
SocketContext::SendBytes(){
	if( (m_status&(SockStatus_Connected|SockStatus_Handshaking)) == 0 )
		return false; // Socket is not connected.

	DWORD	dwSent			= 0;
	int		nSize			= 0;
	bool	bIsPartial		= false;
	bool	bIsSSLEncrypted = false;
	m_lockSend.Lock(); // Lock
	BYTE*	lpData			= PopSendData(nSize, bIsPartial, bIsSSLEncrypted);
	m_lockSend.Unlock(); // Unlock

	if( !lpData ){
		return false;
		}

	m_status				|= SockStatus_Sending;
	if( m_pSSL && !bIsSSLEncrypted ){
		bool	bFatalError = false;
		int		nBytes		= SSL_write(m_pSSL, lpData, nSize);
		int		ssl_error	= ssl_get_error(m_pSSL, nBytes, bFatalError);
		if( !bFatalError && nBytes == nSize ){
			nBytes			= BIO_read(m_BIOSend, m_buffSendSSL.buf, m_nSendBufferSize);
			ssl_error		= ssl_get_error(m_pSSL, nBytes, bFatalError);
			if( !bFatalError && nBytes > 0 ){
				memcpy(m_buffSend.buf, (BYTE*)m_buffSendSSL.buf, nBytes);
				nSize = nBytes;
				}
			}

		if( bFatalError ){
			m_status &= ~SockStatus_Sending;
			Close(m_hEventClosed);
			return false;
			}
		}
	else
		memcpy(m_buffSend.buf, lpData, nSize);

#ifdef _SERVER
	CString sLogEntry;
	sLogEntry.Format(_T("Thread %08x Socket %08x Sending... %d bytes"), GetCurrentThreadId(), (int)m_sock, nSize);
	TRACE(_T("Thread %08x Socket %08x Sending... %d bytes\r\n"), GetCurrentThreadId(), (int)m_sock, nSize);
	Logger::CreateLogEntry(sLogEntry.GetBuffer());
#endif

	ULONG	ulFlags = bIsPartial ? MSG_PARTIAL : 0;
	m_buffSend.len	= nSize;
	UINT nRetVal	= WSASend(m_sock, &m_buffSend, 1, NULL, ulFlags, &m_overlSend.m_ol, NULL);
	if( nRetVal == SOCKET_ERROR ){
		int nLastError = WSAGetLastError();
		if( nLastError != WSA_IO_PENDING ){
			m_status		= SockStatus_Closed;
			m_nLastError	= nLastError;
			// Destroy sent data.
			delete [] lpData;
			return false;
			}
		}

	// Destroy sent data.
	delete [] lpData;
	IncreaseIOWorkersReferenceCounter();
    return true;
	}

bool
SocketContext::ReceiveBytes(){
	if( (m_status&(SockStatus_Connected|SockStatus_Handshaking)) == 0 )
		return false; // Socket is not connected.
	if( m_status&SockStatus_Receiving )
		return true; // Socket is already receiving.

    ULONG	ulFlags = MSG_PARTIAL;
	UINT	nRetVal	= WSARecv(m_sock, &m_buffRecv, 1, NULL, &ulFlags, &m_overlRecv.m_ol, NULL);
	if( nRetVal == SOCKET_ERROR ){
		int nLastError = WSAGetLastError();
		if( nLastError != WSA_IO_PENDING ){
			m_status		= SockStatus_Closed;
			m_nLastError	= nLastError;
			return false;
			}
		}
	m_status |= SockStatus_Receiving;
	IncreaseIOWorkersReferenceCounter();
	return true;
	}

void
SocketContext::IncreaseIOWorkersReferenceCounter(){
	::InterlockedIncrement(&m_nIOWorkersReferenceCounter);
	}

void
SocketContext::DecreaseIOWorkersReferenceCounter(){
	::InterlockedDecrement(&m_nIOWorkersReferenceCounter);
	}

bool
SocketContext::ReadReceivedBytes(DWORD dwSize){
	m_dwBytesReceived += dwSize;
	return m_receivedData.AddBytes((BYTE*)m_buffRecv.buf, dwSize);
	}

bool
SocketContext::PushSendData(BYTE* lpData, int nDataSize, bool bSSLEncrypted){
	if( !lpData || nDataSize <= 0 )
		return false;

	SendDataBuffer* pBuff	= new SendDataBuffer;
	pBuff->_nOffset			= 0;
	pBuff->_pszBuffer		= new BYTE[nDataSize];
	pBuff->_nLen			= nDataSize;
	pBuff->_bSSLEncrypted	= m_pSSL ? bSSLEncrypted : false;
	memcpy(pBuff->_pszBuffer, lpData, nDataSize);
	m_lockSend.Lock(); // Lock
	m_listSendData.Add(pBuff);
	m_lockSend.Unlock(); // Unlock

	bool bRet = true;
	if( !(m_status&SockStatus_Sending) )
		bRet = SendBytes();
	return true;
	}

BYTE*
SocketContext::PopSendData(int& nDataSize, bool& bIsPartial, bool& bSSLEncrypted){
	if( !m_listSendData.GetCount() )
		return NULL;
	SendDataBuffer* pBuff	= m_listSendData.GetAt(0);
	BYTE*			pResult;
	if( m_nSendBufferSize < (pBuff->_nLen - pBuff->_nOffset) ){
		bIsPartial		= true;
		bSSLEncrypted	= pBuff->_bSSLEncrypted;
		pResult			= &pBuff->_pszBuffer[pBuff->_nOffset];
		nDataSize		= m_buffSend.len;
		// Change offset.
		pBuff->_nOffset += m_buffSend.len;
		}
	else{
		bIsPartial		= false;
		bSSLEncrypted	= pBuff->_bSSLEncrypted;
		pResult			= &pBuff->_pszBuffer[pBuff->_nOffset];
		nDataSize		= (pBuff->_nLen - pBuff->_nOffset);
		delete pBuff;
		m_listSendData.RemoveAt(0, false);
		}
	return pResult;
	}

bool
SocketContext::IsDataPending(){
	if( (m_status&SockStatus_Closed) != 0 )
		return false;
	if( (m_status&SockStatus_Sending) == SockStatus_Sending )
		return true;
	if( m_pSSL && (BIO_pending(m_BIORecv) > 0 || BIO_pending(m_BIOSend) > 0) )
		return true;
	return false;
	}

/////////////////////////////////////////////////////////////////////////
//
//	Socket IOCP events.
//
bool
SocketContext::OnAccept(){
	m_lock.Lock(); // Lock
	if( (m_status&SockStatus_Closed) == SockStatus_Closed || (m_status&SockStatus_Accepting) != SockStatus_Accepting ){
		m_lock.Unlock(); // Unlock
		return false;
		}

	m_status					&= ~SockStatus_Accepting;
	int			nAddrslen[2]	= {0, 0};
	sockaddr *	pAddrs[2]		= {0, 0};
	GetAcceptExSockaddrs(m_addresses, 0, sizeof(sockaddr_storage), sizeof(sockaddr_storage), &pAddrs[0], &nAddrslen[0], &pAddrs[1], &nAddrslen[1]);
	memcpy_s(&m_addresses[0], sizeof(sockaddr_storage), pAddrs[0], nAddrslen[0]);
	memcpy_s(&m_addresses[1], sizeof(sockaddr_storage), pAddrs[1], nAddrslen[1]);
	// getsockname and getpeername functions are valid after updating accept context.
	setsockopt(m_sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_sock, sizeof(SOCKET));

	if( m_pSSL ){
		m_status				&= ~SockStatus_Accepting;
		m_status				|= SockStatus_Handshaking;
		// Set accept state.
		SSL_set_accept_state(m_pSSL);
		}
	else{
		char szIpAddress[32];
		std::string sRemoteIp;
		if( GetRemoteIp(szIpAddress, 32) )
			sRemoteIp = szIpAddress;
		int nRemotePort = GetRemotePort();
		bool bAccept = true;
		if( m_pEventHandler )
			m_pEventHandler->OnAcceptConnection(m_sock, sRemoteIp, nRemotePort, bAccept);

		if( bAccept )
			OnConnect();
		else
			OnDisconnect(0);
		}
	m_lock.Unlock(); // Unlock
	return true;
	}

void
SocketContext::OnConnect(){
	bool	bFatalErrorOccured	= false;
	m_lock.Lock(); // Lock
	if( (m_status&SockStatus_Closed) == SockStatus_Closed ){
		m_lock.Unlock(); // Unlock
		return;
		}

	if( (m_status&SockStatus_Connecting) == SockStatus_Connecting ){
		m_status &= ~(SockStatus_Connecting);
		if( m_pSSL ){
			// Set SSL connect state.
			SSL_set_connect_state(m_pSSL);
			// Start handshaking.
			m_status			|= SockStatus_Handshaking;
			int		nBytes		= SSL_read		(m_pSSL, m_buffRecvSSL.buf, m_nRecvBufferSize);
			int		ssl_error	= ssl_get_error	(m_pSSL, nBytes, bFatalErrorOccured);
			int		nBytesSend	= 0;
			if( !bFatalErrorOccured ){
				nBytesSend		= BIO_read		(m_BIOSend, m_buffSend.buf, m_nSendBufferSize);
				ssl_error		= ssl_get_error	(m_pSSL, nBytesSend, bFatalErrorOccured);
				}
			
			if( !bFatalErrorOccured && nBytesSend > 0 )
				PushSendData((BYTE*)m_buffSend.buf, nBytesSend, true);
			else
				Close(m_hEventClosed);
			}
		else{
			m_status |= SockStatus_Connected;
			if( m_pEventHandler ){
				char szIpAddress[32];
				ConnectedSocketInfo* pSockInfo	= new ConnectedSocketInfo();
				pSockInfo->m_dtTimeConnected	= time(NULL);
				pSockInfo->m_nLocalPort			= GetLocalPort();
				pSockInfo->m_nRemotePort		= GetRemotePort();
				if( GetLocalIp(szIpAddress, 32) )
					pSockInfo->m_sLocalIp		= szIpAddress;
				pSockInfo->m_dwRemoteIp			= 0L;
				if( GetRemoteIp(szIpAddress, 32, &pSockInfo->m_dwRemoteIp) )
					pSockInfo->m_sRemoteIp		= szIpAddress;
				pSockInfo->m_sock				= GetSocket();
				pSockInfo->m_pServer			= NULL;
				pSockInfo->m_pSockConn			= this;
				m_pEventHandler->OnConnected(pSockInfo->m_sock, pSockInfo);
				}
			}
		}
	else{// connect event after handshake finishing.
		m_status &= ~(SockStatus_Connecting|SockStatus_Accepting|SockStatus_Handshaking);
		m_status |= SockStatus_Connected;

		if( m_pEventHandler ){
			char szIpAddress[32];
			ConnectedSocketInfo* pSockInfo	= new ConnectedSocketInfo();
			pSockInfo->m_dtTimeConnected	= time(NULL);
			pSockInfo->m_nLocalPort			= GetLocalPort();
			pSockInfo->m_nRemotePort		= GetRemotePort();
			if( GetLocalIp(szIpAddress, 32) )
				pSockInfo->m_sLocalIp		= szIpAddress;
			pSockInfo->m_dwRemoteIp			= 0L;
			if( GetRemoteIp(szIpAddress, 32, &pSockInfo->m_dwRemoteIp) )
				pSockInfo->m_sRemoteIp		= szIpAddress;
			pSockInfo->m_sock				= GetSocket();
			pSockInfo->m_pServer			= NULL;
			pSockInfo->m_pSockConn			= this;
			m_pEventHandler->OnConnected(pSockInfo->m_sock, pSockInfo);
			}
		}

	bool bConnected = ((m_status&SockStatus_Connected) == SockStatus_Connected);
	if( bFatalErrorOccured ){
		m_lock.Unlock(); // Unlock
		Close(m_hEventClosed);
		return;
		}
	else{
		// Start receiving bytes.
		ReceiveBytes();
		// Fire connect event.
		if( bConnected && m_hEventConnected ){
			::SetEvent(m_hEventConnected);
			m_hEventConnected = NULL;
			}
		}
	m_lock.Unlock(); // Unlock
	}

void
SocketContext::OnDisconnect(DWORD dwLastError){
	m_lock.Lock(); // Lock
	if( m_status&SockStatus_Closed ){
		bool bConnecting	= ((m_status&SockStatus_Connecting) == SockStatus_Connecting);
		m_status			&= ~SockStatus_Connecting;
		m_lock.Unlock(); // Unlock
		// Signal connected event.
		if( bConnecting ){
			if( m_hEventConnected ){
				SetEvent(m_hEventConnected);
				m_hEventConnected = 0L;
				}
			}
		return;
		}
	bool bCallHandler	= ((m_status&SockStatus_Accepting) == 0);
	m_status			= SockStatus_Closed;
	m_lock.Unlock(); // Unlock

	if( m_pEventHandler && bCallHandler )
		m_pEventHandler->OnDisconnected(this, dwLastError);

	bool bAutoDeleteOnDisconnect = m_bAutoDeleteOnDisconnect;
	Close(m_hEventClosed);

	// Auto delete self.
	if( bAutoDeleteOnDisconnect )
		delete this;
	}

void
SocketContext::OnDataReceived(DWORD dwSize){
	m_status &= ~SockStatus_Receiving;
	DecreaseIOWorkersReferenceCounter();

	m_lock.Lock(); // Lock
	if( m_pSSL ){
		bool	bFatalErrorOccurred = false, bSSLConnectionEstablished = false;
		int		ssl_error			= 0;
		if( ReadReceivedBytes(dwSize) ){
			int nBytes		= BIO_write		(m_BIORecv, m_receivedData.GetBufferPtr(), m_receivedData.GetUsedBufferSize());
			ssl_error		= ssl_get_error	(m_pSSL, nBytes, bFatalErrorOccurred);
			if( !bFatalErrorOccurred && m_receivedData.GetUsedBufferSize() == nBytes ){
				nBytes		= SSL_read		(m_pSSL, m_buffRecvSSL.buf, m_nRecvBufferSize);
				ssl_error	= ssl_get_error	(m_pSSL, nBytes, bFatalErrorOccurred);
				if( !bFatalErrorOccurred && ((m_status&SockStatus_Handshaking) == SockStatus_Handshaking) ){
					if( SSL_is_init_finished(m_pSSL) ){
						m_status &= ~SockStatus_Handshaking;
						m_status |= SockStatus_Connected;
						bSSLConnectionEstablished = true;
						}
					
					if( BIO_pending(m_BIOSend) ){
						int nBytesSend	= BIO_read		(m_BIOSend, m_buffSend.buf, m_nSendBufferSize);
						ssl_error		= ssl_get_error	(m_pSSL, nBytesSend, bFatalErrorOccurred);
						if( !bFatalErrorOccurred && nBytesSend > 0 )
							PushSendData((BYTE*)m_buffSend.buf, nBytesSend, true);
						}
					}

				if( !bFatalErrorOccurred && nBytes > 0 ){
					if( m_pEventHandler )
						m_pEventHandler->OnDataReceived(this, (BYTE*)m_buffRecvSSL.buf, (DWORD)nBytes, (m_dwBytesReceived == 0));
					m_dwBytesReceived += nBytes;
					}
				}
			else 
			if( !BIO_should_retry(m_BIORecv) )
				bFatalErrorOccurred = true;
			// Zero used memory for received data bytes.
			m_receivedData.ZeroUsedMemory();
			}

		if( bFatalErrorOccurred ){
			m_lock.Unlock(); // Unlock
			Close(m_hEventClosed);
			return;
			}
		else{
			// SSL connection established.
			if( bSSLConnectionEstablished )
				OnConnect();
			}
		}
	else{
		// Gracefull close
		if( !dwSize ){
			m_lock.Unlock(); // Unlock
			Close(m_hEventClosed);
			return;
			}

		DWORD dwBytesReceived =  m_dwBytesReceived;
		if( ReadReceivedBytes(dwSize) ){
			if( m_pEventHandler )
				m_pEventHandler->OnDataReceived(this, (BYTE*)m_receivedData.GetBufferPtr(), (DWORD)m_receivedData.GetUsedBufferSize(), (dwBytesReceived == 0));
			// Zero used memory fore received data bytes.
			m_receivedData.ZeroUsedMemory();
			}
		}

	// Start receiving data.
	ReceiveBytes();
	m_lock.Unlock(); // Unlock
	}

void
SocketContext::OnDataSent(DWORD dwSize){
	m_dwBytesSent	+= dwSize;
	DecreaseIOWorkersReferenceCounter();
#ifdef _SERVER
	CString sLogEntry;
	sLogEntry.Format(_T("Thread %08x %08x Sent %d bytes"), GetCurrentThreadId(), (int)m_sock, dwSize);
	TRACE(_T("Thread %08x %08x Sent %d bytes\r\n"), GetCurrentThreadId(), (int)m_sock, dwSize);
	Logger::CreateLogEntry(sLogEntry.GetBuffer());
#endif
	if( m_pSSL ){
		/*
		bool bFatalErrorOccurred = false;
		if( BIO_pending(m_BIOSend) ){
			int nBytesSend	= BIO_read		(m_BIOSend, m_buffSend.buf, m_nSendBufferSize);
			int ssl_error	= ssl_get_error	(m_pSSL, nBytesSend);
			if( nBytesSend > 0 )
				PushSendData((BYTE*)m_buffSend.buf, nBytesSend);
			else 
				bFatalErrorOccurred = ssl_is_fatal_error(ssl_error);
			}/

		/*
		if( dwSize > 0 ){
			bool	 bFatalErrorOccurred	= false;
			int		nBytes					= SSL_write(m_pSSL, m_buffSendSSL.buf, m_nSendBufferSize);
			int		ssl_error				= ssl_get_error(m_pSSL, nBytes);
			if( nBytes == dwSize ){
				m_buffSend.len	= 0;
				}
			else
				bFatalErrorOccurred = ssl_is_fatal_error(ssl_error);
			}*/
		}
	else{
		}

	// Send new bytes.
	// Change status of senging only if data is not available to send.
	if( !SendBytes() )
		m_status &= ~SockStatus_Sending;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Static methods.
//

bool
SocketContext::SetKeepAliveOption(SOCKET sock, bool bKeepAlive, int nKeepAliveTime, int nKeepAliveInterval){
	bool bRet = false;
	if( sock ){
		// Set keep alive timer.
		struct tcp_keepalive {
			u_long  onoff;
			u_long  keepalivetime;
			u_long  keepaliveinterval;
			};

		tcp_keepalive kalive;
		kalive.onoff				= bKeepAlive ? 1 : 0;
		kalive.keepalivetime		= nKeepAliveTime;
		kalive.keepaliveinterval	= nKeepAliveInterval;

		DWORD dwReturned			= 0;
		if( WSAIoctl(sock, SIO_KEEPALIVE_VALS, &kalive, sizeof(kalive), NULL, 0, &dwReturned, NULL, NULL) != SOCKET_ERROR )
			bRet = true;
		}
	return bRet;
	}

SocketContext*
SocketContext::BeginAccept(	SocketConnectionListener* pConnListener, SOCKET sockListen, SSL_CTX* pSSLContext, int nSendBuffSize, int nReceiveBuffSize, bool bKeepAlive /*=false*/, int nKeepAliveTime /*=0*/, int nKeepAliveInterval /*=0*/ ){
	if( sockListen == INVALID_SOCKET )
		return NULL;

	SocketContext* pSockContxt			= new SocketContext(nSendBuffSize, nReceiveBuffSize);
	if( pSSLContext ){
		pSockContxt->m_pSSL				= SSL_new(pSSLContext);
		pSockContxt->m_BIOSend			= BIO_new(BIO_s_mem());
		pSockContxt->m_BIORecv			= BIO_new(BIO_s_mem());
		SSL_set_bio(pSockContxt->m_pSSL, pSockContxt->m_BIORecv, pSockContxt->m_BIOSend);

		pSockContxt->m_buffRecvSSL.len	= pSockContxt->m_nRecvBufferSize;
		pSockContxt->m_buffRecvSSL.buf	= (CHAR*)new BYTE[pSockContxt->m_nRecvBufferSize];
		memset(pSockContxt->m_buffRecvSSL.buf, 0, pSockContxt->m_nRecvBufferSize);
		pSockContxt->m_buffSendSSL.len	= 0;
		pSockContxt->m_buffSendSSL.buf	= (CHAR*)new BYTE[pSockContxt->m_nSendBufferSize];
		memset(pSockContxt->m_buffSendSSL.buf, 0, pSockContxt->m_nSendBufferSize);
		}

	SOCKET	sock			= WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	// Set non-blocking state.
	int		option = 0, err	= ioctlsocket((int)sock, FIONBIO, (u_long *)&option);

	// Set the socket options. {{
	const int dontLinger	= 0; 
	const int one			= 1;
	// Dont wait for data when closesocket.
	setsockopt(sock, SOL_SOCKET, SO_DONTLINGER, (char *)&dontLinger, sizeof(int));
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&one, sizeof(int));
	// }}

	// Associate socket to IOCP.
	SocketIOCP::GetInstance()->AssociateSocketHandle(sock, (ULONG_PTR)pSockContxt);

	pSockContxt->m_sock		= sock;
	pSockContxt->m_status	|= SockStatus_Accepting;
	pSockContxt->m_overlConn.m_bConnecting = false; // Accepting state.
	BOOL accepted			= AcceptEx(sockListen, sock, pSockContxt->m_addresses, 0, sizeof(sockaddr_storage), sizeof(sockaddr_storage), 0, &pSockContxt->m_overlConn.m_ol);
	int nLastError			= WSAGetLastError();

	if( nLastError != WSA_IO_PENDING ){
		pSockContxt->m_overlConn.m_dwLastError = nLastError;
		delete pSockContxt;
		pSockContxt = NULL;
		}
	return pSockContxt;
	}