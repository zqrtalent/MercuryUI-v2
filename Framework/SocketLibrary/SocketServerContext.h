#pragma once
#include "SocketContext.h"

class SocketServer;
class SocketServerContext : public SocketContext
{
public:
	SocketServerContext(SocketServer* pSockServer, UINT nSendBuffSize = 8192, UINT nReceiveBuffSize = 8192);
	virtual ~SocketServerContext();

protected:
	virtual	void		OnInitialize						();
	virtual void		OnDisconnect						(DWORD dwLastError);
	virtual void		OnDataReceived						(DWORD dwSize);
	virtual void		OnDataSent							(DWORD dwSize);

protected:
	SocketServer*		m_pSockServer;
};
