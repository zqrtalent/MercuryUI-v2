#include "stdafx.h"
#include "SSLSocketContext.h"
#include <Mswsock.h>
#include <Ws2tcpip.h>
#include <Mstcpip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SSLSocketContext.
//
/////////////////////////////////////////////////////////////////////

SSLSocketContext::SSLSocketContext(UINT nSendBuffSize /*= 8192*/, UINT nReceiveBuffSize /*= 8192*/) :
	SocketContext(nSendBuffSize, nReceiveBuffSize){
	m_pSSLCtx = nullptr;
	}

SSLSocketContext::~SSLSocketContext(){
	// Destroy SSL context structure. 
	if( m_pSSLCtx ){
		delete m_pSSLCtx;
		m_pSSLCtx = NULL;
		}
	}

bool
SSLSocketContext::InitSSL(LPCTSTR lpszClientCertFile){
	if( m_pSSLCtx )
		return true;
	m_pSSLCtx = new SSLContext();
	if( !m_pSSLCtx->Init(NULL, NULL) ){
		delete m_pSSLCtx;
		m_pSSLCtx = NULL;
		return false;
		}
	return true;
	}

bool
SSLSocketContext::Connect(TCHAR* pszIp, int nPort, bool bKeepAlive /*= false*/, int nKeepAliveTime /*= 0*/, int nKeepAliveInterval /*= 0*/, SOCKET* pCopySocket /*= NULL*/, HANDLE hConnectEvent /*= NULL*/){
	bool bRet = SocketContext::Connect(pszIp, nPort, bKeepAlive, nKeepAliveTime, nKeepAliveInterval, pCopySocket, hConnectEvent);
	if( bRet ){
		}
	return bRet;
	}

bool
SSLSocketContext::Close(HANDLE hEventClosed){
	return SocketContext::Close(hEventClosed);
	}

void
SSLSocketContext::OnConnect(){
	::EnterCriticalSection(&m_lock); // Lock
	if( m_pSSLCtx && !m_pSSL ){
		m_pSSL				= SSL_new(m_pSSLCtx->GetSSLContextHandle());
		m_BIOSend			= BIO_new(BIO_s_mem());
		m_BIORecv			= BIO_new(BIO_s_mem());
		SSL_set_bio(m_pSSL, m_BIORecv, m_BIOSend);

		m_buffRecvSSL.len	= m_nRecvBufferSize;
		m_buffRecvSSL.buf	= (CHAR*)new BYTE[m_nRecvBufferSize];
		memset(m_buffRecvSSL.buf, 0, m_nRecvBufferSize);
		m_buffSendSSL.len	= 0;
		m_buffSendSSL.buf	= (CHAR*)new BYTE[m_nSendBufferSize];
		memset(m_buffSendSSL.buf, 0, m_nSendBufferSize);
		}
	::LeaveCriticalSection(&m_lock); // Unlock

	SocketContext::OnConnect();
	}