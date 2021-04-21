#pragma once

enum SockIOType{
	IOInitialize = 1,
	IORead,
	IOWrite,
	IODisconnect
	};

class OVERLAPPEDEX{
	public:
		OVERLAPPED			m_ol;
		SockIOType			m_ioType;

		OVERLAPPEDEX(SockIOType ioType){
			ZeroMemory(&m_ol, sizeof(OVERLAPPEDEX));
			m_ioType = ioType;
			}
	};

class SocketIOQueue
{
protected:
	SocketIOQueue();
	virtual ~SocketIOQueue();

public:
	static SocketIOQueue* GetInstance(){
		static SocketIOQueue ioQueue;
		return &ioQueue;
		}

	bool	Create					();
	void	Free					();

	bool	BindSocket				(SOCKET sock, ULONG_PTR key);
	bool	GetQueuedEvent			(LPDWORD lpBytesLen, ULONG_PTR* lpKeyObject, LPOVERLAPPED* lpOverlapped);
	bool	PostInitializeEvent		(ULONG_PTR keyObject);
	bool	PostDisconnectEvent		(ULONG_PTR keyObject);
	bool	PostEvent				(OVERLAPPEDEX* lpOverlapped, ULONG_PTR keyObject);
	void	PostTerminationSignal	();

private:
	HANDLE			m_hIOCP;	// IO completition port handle.

	class DefaultKey{
		public:
			SOCKET _sock;
		};

	DefaultKey*		m_pKey;	
};

