#pragma once
#include "..\SocketLibrary\SocketLibraryDef.h"
#include "..\SocketLibrary\SSLSocketContext.h"
#include "..\Protocol\PacketHandler.h"
#include "..\Array\AutoSortedArray.h"
#include "..\Array\List.h"
#include "..\Protocol\BasePacket.h"
#include "..\Utility\GrowableMemory.h"

class GameClient : public SocketIOEventHandler
{
public:
	GameClient(HWND hOwnerWnd, std::string sIpAddress = _T("127.0.0.1"), int nPort = 20001, int nReconnectTime = 5000);
	virtual ~GameClient();

public:
	// Set/Get owner window.
	void				SetOwner				(HWND hOwnerWnd)	{m_hwndOwner = hOwnerWnd;};
	HWND				GetOwner				()					{return m_hwndOwner;};
	// Set Connected/Disconnected window message identifier.
	void				SetConnDisMessageId		(UINT nMessageId)	{m_nConnDisMessageId = nMessageId;};
	// Set Ip addres to connect to.
	void				SetIpAddress			(std::string sIpAddress){m_sIpAddress = sIpAddress;};
	// Set port number to connect to.
	void				SetPort					(int nPort)			{m_nPort = nPort;};
	// Register packet receive listener method.
	bool				AddPacketHandler		(PacketCodes code, void* pClass, PacketListenerMethod method);
	// Call that method only from main thread.
	bool				WaitForResponse			(int nPacketCode, BasePacket** packetResponse, int nWaitSeconds, Event* pEventCancel = NULL);
	// Check for connected state.
	bool				IsConnected				();
	// Retrieve session id.
	__int64				GetSessionId			(){return m_nSessionId;};
	// Safely destroies class object.
	void				SafeDestroy				();
	// Set keep alive params.
	bool				SetKeepAlive			(bool bKeepAlive, int nKeepAliveTime = 0, int nKeepAliveInterval = 0);
	// Set SSL parameters.
	void				UseSSL					(bool bUseSSL, const std::string sClientCert);

	SOCKET				GetSock					(){return m_pSockCtx ? m_pSockCtx->GetSocket() : 0;};
	bool				CallPacketHandler		(BasePacket* p, void* pParam);
		
public: // Operations.
	virtual bool		Connect					(SOCKET* pCopySocket = NULL, HANDLE* pEventConnect = NULL);
	virtual bool		Disconnect				();
	virtual void		Login					(std::string sUserName, std::string sPassword);
	virtual void		LoginByWebSession		(const std::string sWebSession);
	virtual void		Logout					();
	virtual void		RegisterUser			(const std::string sUserName, const std::string sPwd, const std::string sPwdRepeat, const std::string sEmailAddress, const std::string sPhoneNumber, const std::string sNickName);
	virtual void		HandShake				();
	virtual bool		SendPacket				(BasePacket* packet);

protected: 
	virtual void		OnAcceptConnection		(SOCKET sock, const std::string sRemoteIp, int nRemotePort, bool& bAccept);
	virtual void		OnConnected				(SOCKET sock, ConnectedSocketInfo* pSockInfo);
	virtual void		OnDataReceived			(SocketContext* pConn, BYTE* pData, DWORD dwSize, bool bIsFirstData);
	virtual void		OnDisconnected			(SocketContext* pConn, DWORD dwLastError);

	bool				StartDataManager		();
	bool				StopDataManager			();
	bool				AddReceivedDataInQueue	(BYTE* lpData, int nSize);
	void				HandleReceivedData		(BYTE* lpData, int nSize);

protected: // Packet receive listener methods.
	void				OnHandShake				(HandShakePacket* p, void* pParam);
	void				OnLogin					(LoginPacket* p, void* pParam);
	void				OnLogout				(LogoutPacket* p, void* pParam);

protected: // Static thread procs.
	static UINT  __stdcall DataManage_Thread	(GameClient* pClient);

protected:
	SSLSocketContext*		m_pSockCtx;					// Socket context.
	HWND					m_hwndOwner;				// Owner window.
	UINT					m_nConnDisMessageId;		// Connected/Disconnected message identifier.
	int						m_nPort;					// Port number.
	std::string				m_sIpAddress;				// IP address listen to.
	bool					m_bUseSSL;					// SSL usage.
	std::string				m_sClientSSLCert;			// Client SSL certificate file.
	bool					m_bAutoDelete;				// Auto delete.

	bool					m_bKeepAlive;				// Keep-alive
	int						m_nKeepAliveTime;			// Keep-alive time (milliseconds).
	int						m_nKeepAliveInterval;		// Keep-alive interval time (milliseconds).

	PacketHandler*			m_pPacketHandler;			// Packet handler.
	Event*					m_pEventWaitForResponse;	// Event for waiting response.

	// Received data manager thread. {{
	HANDLE					m_threadDataManager;
	DWORD					m_dwDataManagerThreadId;
	Event					*m_pStopDataManager;
	Event					*m_pResumeDataManager;
	List<WSABUF>			m_listReceivedData;			// Queue of received data.
	CriticalSection			m_lockDataManager;
	// }}

	CriticalSection			m_secLock1;
	GrowableMemory			m_memSerialize;

	// Game client attributes.
	__int64					m_nSessionId;				// Session identifier.
	int						m_nClientVersion;			// Client version.
	std::string				m_sUserName;				// Logged in user name.
	bool					m_bLoggedIn;				// User's authorization status.
};
