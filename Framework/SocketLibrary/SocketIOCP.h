#pragma once
#include "SocketLibraryDef.h"

class SocketContext;
class SocketIOCP
{
protected:
	SocketIOCP();
	virtual ~SocketIOCP();

public:
	static SocketIOCP* GetInstance(){
		static SocketIOCP ioMan;
		return &ioMan;
		}

	bool	Start					(int nWorkerThreadCt = 1);
	void	Stop					();
	bool	IsStarted				(){return (m_hIOCP != NULL);};

	bool	AssociateSocketHandle	(SOCKET sock, ULONG_PTR keyObject);
	bool	GetQueuedEvent			(LPDWORD lpBytesLen, SocketContextBase** pSockCtxBase, LPOVERLAPPED* lpOverlapped);
	bool	PostDisconnectEvent		(ULONG_PTR keyObject, DWORD dwLastError);
	bool	PostEvent				(OVERLAPPEDEX* lpOverlapped, ULONG_PTR keyObject);
	void	PostTerminationSignal	();

protected:
	static unsigned __stdcall		_MainThreadProc		(void* pIOMan);
	void							DoMainThreadJob		();

private:
	HANDLE*					m_phThread;
	DWORD*					m_pdwThreadIds;
	HANDLE					m_hStopEvent;
	UINT					m_nThreadCt;

	HANDLE					m_hIOCP;	// IO completition port handle.
	class DefaultKey{
		public:
			SOCKET _sock;
		};
	DefaultKey*		m_pKey;	
};

