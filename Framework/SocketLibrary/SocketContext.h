#pragma once

#include "../Array/List.h"
#include "../Utility/GrowableMemory.h"
#include "SocketIOCP.h"

class SocketContext : public SocketContextBase
{
public:
	friend SocketIOCP;
	SocketContext(UINT nSendBuffSize = 8192, UINT nReceiveBuffSize = 8192);
	virtual ~SocketContext();

	friend class SocketConnectionListener;
	friend class WebSocketProtocol;

public:
	static bool				SetKeepAliveOption					(SOCKET sock, bool bKeepAlive, int nKeepAliveTime, int nKeepAliveInterval);
	static SocketContext*	BeginAccept							(SocketConnectionListener* pConnListener, SocketServer* pServer, SOCKET sockListen, SSL_CTX* pSSLContext,
																int nSendBuffSize, int nReceiveBuffSize, bool bKeepAlive = false, int nKeepAliveTime = 0, int nKeepAliveInterval = 0);
public:
	virtual bool			Connect								(TCHAR* pszIp, int nPort, bool bKeepAlive = false, int nKeepAliveTime = 0, int nKeepAliveInterval = 0, SOCKET* pCopySocket = NULL, HANDLE hConnectEvent = NULL);
	virtual bool			ConnectSync							(TCHAR* pszIp, int nPort, bool bKeepAlive = false, int nKeepAliveTime = 0, int nKeepAliveInterval = 0, SOCKET* pCopySocket = NULL);

	virtual bool			Send								(BYTE* lpData, int nSize);

	virtual bool			Close								(HANDLE hEventClosed);
	virtual void			SafeClose							(int nWaitMS = -1);

	int						GetLocalPort						();
	int						GetRemotePort						();
	bool					GetLocalIp							(char* pszIp, int nLen, DWORD* pIpAsDWORD = NULL);
	bool					GetRemoteIp							(char* pszIp, int nLen, DWORD* pIpAsDWORD = NULL);

	bool					IsConnected							()								{return ((m_status&SockStatus_Connected) == SockStatus_Connected);};
	void					SetIOEventHandler					(SocketIOEventHandler* pHandler){m_pEventHandler = pHandler;};
	SOCKET					GetSocket							()								{return m_sock;};
	void					SetParam							(void* pParam)					{m_pParam = pParam;};
	void*					GetParam							()								{return m_pParam;};
	int						GetLastError						()								{return m_nLastError;};

	DWORD					GetSizeBytesSent					()				{return m_dwBytesSent;};
	DWORD					GetSizeBytesReceived				()				{return m_dwBytesReceived;};
	void					SetAutoDeleteOnDisconnect			(bool bOn)		{m_bAutoDeleteOnDisconnect = bOn;};
	int						GetStatus							()				{return m_status;};

protected:
	bool					SendBytes							();
	bool					ReceiveBytes						();

	bool					ReadReceivedBytes					(DWORD dwSize, bool& bCloseSocket);
	bool					PushSendData						(BYTE* lpData, int nDataSize, bool bSSLEncrypted);
	BYTE*					PopSendData							(int& nDataSize, bool& bIsPartial, bool& bSSLEncrypted, BYTE*& lpFullData);

	bool					IsDataPending						();

	void					IncreaseIOWorkersReferenceCounter	();
	void					DecreaseIOWorkersReferenceCounter	();

protected:// Socket IO event handlers.
	virtual void			OnHandshakeComplete					();
	virtual	bool			OnAccept							();
	virtual	void			OnConnect							();
	virtual void			OnDisconnect						(DWORD dwLastError);
	virtual void			OnDataReceived						(DWORD dwSize);
	virtual void			OnDataSent							(DWORD dwSize);

protected:
	SOCKET					m_sock;
	SocketIOCP*				m_pSockIOCP;
	SocketIOEventHandler*	m_pEventHandler;			// Socket IO event handler.
	void*					m_pParam;

	volatile int			m_status;					// SocketStatus
	volatile bool			m_bSending;					// Sending bytes.
	volatile bool			m_bReceiving;				// Receiving bytes.
	int						m_nLastError;				// Last Sock IO error.
	bool					m_bAutoDeleteOnDisconnect;	// Auto delete on disconnect.

	HANDLE					m_hEventConnected;			// Socket connected event.
	HANDLE					m_hEventClosed;				// Socket closed event.

	sockaddr_storage		m_addresses[2];				// local and remote address

	UINT					m_nSendBufferSize;
	UINT					m_nRecvBufferSize;

	WSABUF					m_buffSend;
	WSABUF					m_buffRecv;

	// SSL
	SSL*					m_pSSL;
	BIO*					m_BIORecv;
	BIO*					m_BIOSend;
	WSABUF					m_buffSendSSL;
	WSABUF					m_buffRecvSSL;

	OVERLAPPEDEX			m_overlSend;
	OVERLAPPEDEX			m_overlRecv;
	OVERLAPPEDEX			m_overlConn;

	struct SendDataBuffer{
		int		_nOffset;
		BYTE*	_pszBuffer;
		int		_nLen;
		bool	_bSSLEncrypted;
		};

	List<SendDataBuffer>	m_listSendData;
	GrowableMemory			m_receivedData;

	CRITICAL_SECTION		m_lock;
	CRITICAL_SECTION		m_lockSend;

	volatile ULONG			m_nIOWorkersReferenceCounter;
	volatile DWORD			m_dwBytesSent;
	volatile DWORD			m_dwBytesReceived;
};
