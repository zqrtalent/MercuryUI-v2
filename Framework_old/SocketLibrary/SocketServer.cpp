#include "SocketServer.h"
#include "..\Utility\GrowableMemory.h"
#include "..\Path\Path.h"

/////////////////////////////////////////////////////////////////////
//
//	SocketServer.
//
/////////////////////////////////////////////////////////////////////

SocketServer::SocketServer(){
	m_pHandler		= NULL;
	m_pConnListener	= NULL;
	m_bStarted		= false;
	m_pSSL_CTX		= nullptr;
	m_nConnectionCt	= 0;
	/*
	m_pszServerCertPem = "Bag Attributes\n"
"    localKeyID: D4 99 A5 A4 0A 78 18 E6 AC EC 55 1D B4 FE 4A 83 27 82 05 DE \n"
"subject=/C=US/ST=CA/L=LA/O=Test Company/OU=IT/CN=www.testserver.com\n"
"issuer=/C=US/ST=CA/L=LA/O=Test Root CA/OU=IT/CN=www.testrootca.com\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIDRDCCAiwCAf8wDQYJKoZIhvcNAQEFBQAwaDELMAkGA1UEBhMCVVMxCzAJBgNV\n"
"BAgMAkNBMQswCQYDVQQHDAJMQTEVMBMGA1UECgwMVGVzdCBSb290IENBMQswCQYD\n"
"VQQLDAJJVDEbMBkGA1UEAwwSd3d3LnRlc3Ryb290Y2EuY29tMB4XDTEzMDIyNzE4\n"
"MjE1NloXDTIzMDIyNTE4MjE1NlowaDELMAkGA1UEBhMCVVMxCzAJBgNVBAgMAkNB\n"
"MQswCQYDVQQHDAJMQTEVMBMGA1UECgwMVGVzdCBDb21wYW55MQswCQYDVQQLDAJJ\n"
"VDEbMBkGA1UEAwwSd3d3LnRlc3RzZXJ2ZXIuY29tMIIBIjANBgkqhkiG9w0BAQEF\n"
"AAOCAQ8AMIIBCgKCAQEAzkHv+S30g5Dc+F1RJ1PUq9Hbh1YkEUJdYEj7ti+UfONV\n"
"NOT24hXzg8zaNSVO2Bhm+l8vzOVYMnjK9xcGSq5R5I633+lEeFdxURfsSJv9Vymq\n"
"tHUj5eNkmjzWBVrf4HvnZTJtRJljs941zYUgyJT9tkQXaerGFKJ6sfdXYfhGrkuK\n"
"gA1e71TwpRFYcfyYbQ3htENTh2CFBv7l5gjrITcmEJwpcU3U4nx4ZTr0IPLmV2kr\n"
"K8IJysY4dqgRcmduEI5ZgbYGkdG8L7QjggFXA6QNDPu8DfmXeeqS0gIffEm22bk7\n"
"b2fMnPfnFsJLsDdyhgrdYktnWhtZNij0y80tV4YCTwIDAQABMA0GCSqGSIb3DQEB\n"
"BQUAA4IBAQDMLn9VnUQt6BWx73J1lExYO/LWulMOnMR/WSVFy9dSwry+E807ekMY\n"
"WC8b3gpgDIqfkZjmttE9VtAdss2Baten+oBW+K13339sxHvcn30OxOs/Bln0yvaZ\n"
"Be+Zir7iE450b1IdYI98PMTSKgrK2e3vx/uUOCgG2yvs6/1v5rz5er/M1SQNzdMS\n"
"blelHWRQ1/ExwoUWBfIBkx/A4lTPmLgoC9fnXSiLhHKbZdfCJD8KLzEV0Se+ocn/\n"
"vl+6tlcUznap0TsRQpC67T/NGUimxdAhb6G1/U6z9bq0QQIuDxpOIpvwIgLvfRFx\n"
"qZQxmxOcK28fejHngmek7ZJNYKQbNewP\n"
"-----END CERTIFICATE-----\n"
"Bag Attributes\n"
"    localKeyID: D4 99 A5 A4 0A 78 18 E6 AC EC 55 1D B4 FE 4A 83 27 82 05 DE \n"
"Key Attributes: <No Attributes>\n"
"-----BEGIN PRIVATE KEY-----\n"
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDOQe/5LfSDkNz4\n"
"XVEnU9Sr0duHViQRQl1gSPu2L5R841U05PbiFfODzNo1JU7YGGb6Xy/M5VgyeMr3\n"
"FwZKrlHkjrff6UR4V3FRF+xIm/1XKaq0dSPl42SaPNYFWt/ge+dlMm1EmWOz3jXN\n"
"hSDIlP22RBdp6sYUonqx91dh+EauS4qADV7vVPClEVhx/JhtDeG0Q1OHYIUG/uXm\n"
"COshNyYQnClxTdTifHhlOvQg8uZXaSsrwgnKxjh2qBFyZ24QjlmBtgaR0bwvtCOC\n"
"AVcDpA0M+7wN+Zd56pLSAh98SbbZuTtvZ8yc9+cWwkuwN3KGCt1iS2daG1k2KPTL\n"
"zS1XhgJPAgMBAAECggEAIT83s27Y7yw2skI4hqJYsamOPW6BOdb8vjyFdoSM5uSu\n"
"I2yU7zSioCgxNEfjQaoNT2ZwihKd+OTHsrSfawJWaQUoVot/YfaWaX/1sm6Sk64/\n"
"uf733mKdIM+VoB9Z3xGZ5xIN0vT2wVOcUJiZBDwf+XVYYNZbP5BBPtaj20LuAcIZ\n"
"OmW9uigdXQkQ1dylUkRPitjJ92bbysrTr621JTBSmvKnF7ctcF/Ql6VfS5RcqzYI\n"
"6U1vozoFkjmUnExlYZHC6qKCFG73Z+IcC7ojdMpzMp4/EqiveV/9EVdFlLRB1YAa\n"
"tND93xU9mo7L26XQzy79Xf2dWRUgUvaJ/7EvLA1RoQKBgQD2ZhJ9ogqfQ0ahq0D6\n"
"5neZo6bPbckEKshv1GKR5ixnYpPp1kCIxM8oIzb9fOvTX4MOMeRzPJyrJNwhVgfY\n"
"otWLrvkNviGHXN0frmkdj/Y/WSWh7clzzwXmGbB/8NPG4yzREvQ8vhKBkAmZln6K\n"
"ICl8J5NxOxF6GgYJ793GcsfZVQKBgQDWS3DYMVQ3eRgFajkQ/8+Gacgdu+8/SyM1\n"
"WptHOlPvKfqg3nZYPlAjMnVmk0Q7l/d2EtFBPP07/Jz0IvC/pMz0S8XfW/NigcRn\n"
"0R5Nci3BXbmQEjxNGt0m0sX4C4/Bx8ei8pugipX96OemT/bWP05RskL6tWsofGsb\n"
"8zgIQcldEwKBgCyx90iyzBp3qahJ2E+q3qcP+IJH9965pAIlFHxCtGtMhmg0ZSBq\n"
"EunE+YSh1GVTPgKlKjt9Ey44UXX6lRHG99WOt762bn6Pac0FZivmoVR8Z0coSxKm\n"
"yvsiTdHnbYL2UnraZVNfZxv5dMRXeDy1+NB8nVI81L7BWbcTu7bzuyzBAoGAY0j4\n"
"s3HHbxwvwPKCFhovcDs6eGxGYLDTUzjzkIC5uqlccYQgmKnmPyh1tFyu1F2ITbBS\n"
"O0OioFRd887sdB5KxzUELIRRs2YkNWVyALfR8zEVdGa+gYrcw8wL5OyWYlXJbPmy\n"
"mSMcc1OhYDDUUFdsVfWdisLbLxrWFVEOuOSiAvkCgYEA2viHsxoFxOrhnZQOhaLT\n"
"RPrgaSojv9pooHQ6fJwplewt91tb1OchDIeZk9Sl1hqPAXB0167of43GDOw2vfnq\n"
"Ust7RtiyJhQhSkz0qp4aH4P9l+dZJIWnpgjcyWkcz893br9gEuVnQgh13V/lcxOn\n"
"JtpaCFuHNTU3PcFiuQW+cN0=\n"
"-----END PRIVATE KEY-----\n";
	*/
	} 

SocketServer::~SocketServer(){
	Stop();
	}

bool
SocketServer::Start(SockListenOptions* pInfo, SocketServerEventHandler* pHandler, LPCTSTR lpszPemFile /*= NULL*/){
	m_lock.Lock(); // Lock
	if( m_bStarted ){
		m_lock.Unlock(); // Unlock
		return false; // Already started !!!
		}
	
	// Set server pem key certificate.
	if( lpszPemFile ){
		BYTE*		lpData		= NULL;
		int			nFileSize	= 0;
		std::string sPemFile	= lpszPemFile;
		Path::BuildFullPath(sPemFile);

		if( Path::ReadFileContent(sPemFile.c_str(), lpData, nFileSize) ){
			BIO *bio_cert	= BIO_new_mem_buf((void*)lpData, nFileSize);
			X509 *cert		= PEM_read_bio_X509(bio_cert, nullptr, nullptr, nullptr);
			EVP_PKEY *pkey	= PEM_read_bio_PrivateKey(bio_cert, 0, 0, 0);

			m_pSSL_CTX		= new SSLContext();
			m_pSSL_CTX->Init(cert, pkey);

			X509_free		(cert);
			EVP_PKEY_free	(pkey);
			BIO_free		(bio_cert);
			delete [] lpData;
			}
		}

	int nRet = SocketServer::AddSocketServer(this);
	// Start socket IOCP.
	if( nRet == 1 )
		SocketIOCP::GetInstance()->Start(-1);

	SocketConnectionListener* pListener = new SocketConnectionListener(pInfo, this);
	if( !pListener->Start(m_pSSL_CTX ? m_pSSL_CTX->GetSSLContextHandle() : NULL) ){
		// Stop socket IOCP.
		if( nRet == 1 )
			SocketIOCP::GetInstance()->Stop();
		m_lock.Unlock(); // Unlock
		return false;
		}

	m_pConnListener = pListener;
	m_pHandler		= pHandler;
	m_bStarted		= true;
	pInfo->Copy(&m_options);
	m_lock.Unlock(); // Unlock
	return m_bStarted;
	}

bool
SocketServer::Stop(){
	if( !m_bStarted )
		return false;

	m_lock.Lock(); // Lock
	SocketServerEventHandler* pEventHandler = m_pHandler;

	// Stop socket IOCP.
	if( !SocketServer::RemoveSocketServer(this) )
		SocketIOCP::GetInstance()->Stop();

	if( m_pConnListener ){
		m_pConnListener->Stop();
		delete m_pConnListener;
		m_pConnListener	= NULL;
		}
	m_pHandler		= NULL;
	m_bStarted		= false;

	// Destroy SSL context.
	if( m_pSSL_CTX ){
		delete m_pSSL_CTX;
		m_pSSL_CTX = nullptr;
		}
	m_lock.Unlock(); // Unlock
	
	m_lockListOfSockets.Lock(); // Lock
	HANDLE hEventClosed = CreateEvent(NULL, TRUE, FALSE, NULL);
	for(int i=0; i<m_arrSocketInfoBySocket.GetCount(); i++){
		ConnectedSocketInfo* pSockInfo = (ConnectedSocketInfo*)m_arrSocketInfoBySocket.GetData(i);
		if( pSockInfo ){
			if( pSockInfo->m_pSockConn ){
				ResetEvent(hEventClosed);
				pSockInfo->m_pSockConn->SetIOEventHandler(NULL);
				if( !pSockInfo->m_pSockConn->Close(hEventClosed) )
					WaitForSingleObject(hEventClosed, INFINITE);
				// Call disconnect event handler.
				if( pEventHandler )
					pEventHandler->Event_OnDisconnected(this, pSockInfo->m_pSockConn, pSockInfo->m_sock, ERROR_CONNECTION_ABORTED);
				delete pSockInfo->m_pSockConn;
				}
			else{
				SOCKET sock = (SOCKET)m_arrSocketInfoBySocket.Get(i);
				closesocket(sock);
				}
			delete pSockInfo;
			}
		}
	::CloseHandle(hEventClosed);
	m_arrSocketInfoBySocket.DeleteAll();
	m_lockListOfSockets.Unlock(); // Unlock
	return true;
	}

void
SocketServer::AddConnectedSocketInfo(SOCKET sock, ConnectedSocketInfo* pSockInfo){
	m_lockListOfSockets.Lock(); // Lock
	int	nIndex	= m_arrSocketInfoBySocket.IndexOf((void*)sock);
	if( nIndex == -1 )
		m_arrSocketInfoBySocket.Add((void*)sock, (void*)pSockInfo);
	m_nConnectionCt = (int)m_arrSocketInfoBySocket.GetCount();
	m_lockListOfSockets.Unlock(); // Unlock
	}

bool
SocketServer::DeleteConnectedSocketInfo(SOCKET sock){
	if( !sock ) return false;
	ConnectedSocketInfo* pSockInfo = NULL;
	m_lockListOfSockets.Lock(); // Lock
	int		nIndex	= m_arrSocketInfoBySocket.IndexOf((void*)sock);
	if( nIndex != -1 ){
		pSockInfo = (ConnectedSocketInfo*)m_arrSocketInfoBySocket.GetData(nIndex);
		m_arrSocketInfoBySocket.Delete(nIndex);
		m_nConnectionCt = (int)m_arrSocketInfoBySocket.GetCount();
		}
	m_lockListOfSockets.Unlock(); // Unlock

	if( pSockInfo ){
		if( pSockInfo->m_pSockConn )
			pSockInfo->m_pSockConn->Close(0);
		delete pSockInfo;
		}
	return true;
	}

void
SocketServer::OnAcceptConnection(SOCKET	sock, const std::string sRemoteIp, int nRemotePort, bool& bAccept){
	// Call event handler.
	if( m_pHandler ){
		m_pHandler->Event_OnAcceptConnection(this, sock, sRemoteIp, nRemotePort, bAccept);
		if( !bAccept ) return;
		}
	bAccept = (GetConnectionCt() < m_options.m_nMaxConnection);
	}

void
SocketServer::OnConnected(SOCKET sock, ConnectedSocketInfo* pSockInfo){
	ASSERT(sock && pSockInfo);
	// Add socket info.
	AddConnectedSocketInfo(sock, pSockInfo);
	// Call event handler.
	if( m_pHandler )
		m_pHandler->Event_OnConnected(this, sock, pSockInfo);
	}

void
SocketServer::OnDataReceived(SocketContext* pConn, BYTE* pData, DWORD dwSize, bool bIsFirstData){
	if( m_options.m_nPort == 843 ){
		const char* pcszPlicyReq	= _T("<policy-file-request/>");
		int			nLenPolicyReq	= 23;//strlen(pcszPlicyReq);
		if( dwSize == nLenPolicyReq ){
			pData[nLenPolicyReq] = 0;
			if( !_strcmpi((const char*)pData, pcszPlicyReq) ){
				//CString sReplyXML;
				//sReplyXML.Format(_T("<cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"%d\" /></cross-domain-policy>"), 20001);
				pConn->Send((BYTE*)m_sPolicyXML.c_str(), m_sPolicyXML.length() + 1);
				// Reply with policy xml content.
				return;
				}
			}
		pConn->Close(NULL);
		return;
		}

	// Call event handler.
	if( m_pHandler )
		m_pHandler->Event_OnDataReceived(this, pConn, pData, dwSize, bIsFirstData);
	}

void
SocketServer::OnDisconnected(SocketContext* pConn, DWORD dwLastError){
	SOCKET socketDisposed = pConn->GetSocket();
	// Call event handler.
	if( m_pHandler )
		m_pHandler->Event_OnDisconnected(this, pConn, socketDisposed, dwLastError);

	// Destroy socket info object.
	DeleteConnectedSocketInfo(socketDisposed);
	// Make socket context autodeletable on disconnect.
	pConn->SetAutoDeleteOnDisconnect(true);
	}

bool
SocketServer::GetIpAddress(SOCKET sock, std::string& sIpLocal, int& nPortLocal, std::string& sIpRemote, int& nPortRemote,  DWORD* pIpRemote){
	sockaddr addr_local, addr_remote;
	int nLen = sizeof(sockaddr);
	getsockname(sock, &addr_local, &nLen);
	nLen = sizeof(sockaddr);
	getpeername(sock, &addr_remote, &nLen);

	// getsockname, getsockpeer
	nPortLocal				= htons(*(short*)&addr_local.sa_data[0]);
	nPortRemote				= htons(*(short*)&addr_remote.sa_data[0]);
	BYTE*	arrIpLocal		= (BYTE*)&addr_local.sa_data[2];
	BYTE*	arrIpRemote		= (BYTE*)&addr_remote.sa_data[2];
	stringEx::Format(sIpLocal, _T("%d.%d.%d.%d"), arrIpLocal[0], arrIpLocal[1], arrIpLocal[2], arrIpLocal[3]);
	stringEx::Format(sIpRemote, _T("%d.%d.%d.%d"), arrIpRemote[0], arrIpRemote[1], arrIpRemote[2], arrIpRemote[3]);

	if( pIpRemote )
		*pIpRemote = ((((DWORD)arrIpRemote[0]) << 24) & 0xFF000000) | ((((DWORD)arrIpRemote[1]) << 16) & 0x00FF0000) | ((((DWORD)arrIpRemote[2]) << 8) & 0x0000FF00) | (((DWORD)arrIpRemote[3]) & 0x000000FF);
	return true;
	}

ConnectedSocketInfo*
SocketServer::GetConnectionInfos(int& nCt){
	m_lockListOfSockets.Lock(); // Lock
	nCt = GetConnectionCt();
	if( nCt <=0 ){
		m_lockListOfSockets.Unlock(); // Unlock
		return NULL;
		}

	SocketContext*			pSockConn	= NULL;
	ConnectedSocketInfo*	pArrConn	= new ConnectedSocketInfo[nCt];
	for(int i=0; i<nCt; i++){
		ConnectedSocketInfo* pConn = (ConnectedSocketInfo*)m_arrSocketInfoBySocket.GetData(i);
		if( pConn ){
			pArrConn[i].m_dtTimeConnected	= pConn->m_dtTimeConnected;
			pArrConn[i].m_nLocalPort		= pConn->m_nLocalPort;
			pArrConn[i].m_dwRemoteIp		= pConn->m_dwRemoteIp;
			pArrConn[i].m_nRemotePort		= pConn->m_nRemotePort;
			pArrConn[i].m_pSockConn			= pConn->m_pSockConn;
			pArrConn[i].m_pServer			= pConn->m_pServer;
			pArrConn[i].m_sLocalIp			= pConn->m_sLocalIp;
			pArrConn[i].m_sRemoteIp			= pConn->m_sRemoteIp;
			pArrConn[i].m_sock				= pConn->m_sock;

			pSockConn = pConn->m_pSockConn;
			if( pSockConn ){
				pArrConn[i].m_dwBytesSent		= pSockConn->GetSizeBytesSent();
				pArrConn[i].m_dwBytesReceived	= pSockConn->GetSizeBytesReceived();
				}
			else{
				pArrConn[i].m_dwBytesSent		= 0;
				pArrConn[i].m_dwBytesReceived	= 0;
				}
			}
		}
	m_lockListOfSockets.Unlock(); // Unlock
	return pArrConn;
	}