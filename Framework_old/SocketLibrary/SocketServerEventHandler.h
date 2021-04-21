#pragma once
#include "..\_platformCompat\PlatformCompat.h"

struct ConnectedSocketInfo;
class SocketServer;
class SocketContext;
class GrowableMemory;

class SocketServerEventHandler
{
public:
	SocketServerEventHandler(){};
	~SocketServerEventHandler(){};

public:
	virtual void	Event_OnConnected			(SocketServer* pSockServer, SOCKET sock, ConnectedSocketInfo* pSockInfo){};
	virtual void	Event_OnAcceptConnection	(SocketServer* pSockServer, SOCKET	sock, const std::string sRemoteIp, int nRemotePort, bool& bAccept){bAccept = true;};
	virtual void	Event_OnDataReceived		(SocketServer* pSockServer, SocketContext* pSockConn, BYTE* pData, DWORD dwSize, bool bIsFirstData){};
	virtual void	Event_OnDisconnected		(SocketServer* pSockServer, SocketContext* pSockConn, SOCKET sockDisposed, DWORD dwLastError){};
};