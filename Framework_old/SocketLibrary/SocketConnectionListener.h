#pragma once
#include "SocketLibraryDef.h"
#include "SocketServerOptions.h"
#include "..\Array\AutoSortedArrayInt64.h"

#include "..\_platformCompat\PlatformCriticalSection.h"
#include "..\_platformCompat\PlatformEvent.h"

class SocketServer;
class SocketContext;

class SocketConnectionListener : public SocketContextBase
{
public:
	SocketConnectionListener(SockListenOptions* pListenerInfo, SocketServer* pSockServer);
	virtual ~SocketConnectionListener();

	friend class SocketIOCP;

public:
	virtual bool	Start				(SSL_CTX* pSSLContext = NULL);
	virtual bool	Stop				();

	int				GetSendBufferSize	(){return m_listenerInfo.m_nSendBufferSize;};
	int				GetReceiveBufferSize(){return m_listenerInfo.m_nReceiveBufferSize;};

	// Socket IOCP events.
protected:
	virtual bool	OnAccept			();
	virtual void	OnDisconnect		(DWORD dwLastError);

protected:
	bool					m_bStarted;
	SockListenOptions		m_listenerInfo;
	SocketServer*			m_pSockServer;

	SSL_CTX*				m_pSSLContext;			// SSL context.

	SocketContext*			m_pSocketAccepting;		// Socket context in accepting state.
	SOCKET					m_sockListen;
	Event					m_eventListeningClosed;
	CriticalSection			m_lock;
};
