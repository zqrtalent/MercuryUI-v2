#pragma once
#include "SocketContext.h"
#include "SSLContext.h"

class SSLSocketContext : public SocketContext
{
public:
	SSLSocketContext(UINT nSendBuffSize = 8192, UINT nReceiveBuffSize = 8192);
	virtual ~SSLSocketContext();

public:
	bool			InitSSL		(LPCTSTR lpszClientCertFile = NULL);
	virtual bool	Connect		(TCHAR* pszIp, int nPort, bool bKeepAlive = false, int nKeepAliveTime = 0, int nKeepAliveInterval = 0, SOCKET* pCopySocket = NULL, HANDLE hConnectEvent = NULL);	
	virtual bool	Close		(HANDLE hEventClosed);

protected:
	virtual	void	OnConnect	();

protected:
	SSLContext*		m_pSSLCtx;
};
