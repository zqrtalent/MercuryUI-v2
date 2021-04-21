#pragma once
#include <afxsock.h>            // MFC socket extensions
#include <afxmt.h>
#include "Session.h"
#include "..\Protocol\PacketHandler.h"
#include "..\Array\AutoSortedArray.h"
#include "..\Array\AutoSortedArrayInt64.h"
#include "..\Array\List.h"
#include "..\SocketLibrary\SocketServer.h"
#include "ClientConnection.h"
#include "GameServerConfig.h"

class GrowableMemory;
class GameServer : public SocketServerEventHandler
{
public:
	GameServer();
	virtual ~GameServer();

	friend ClientConnection;
	friend Session;
 
public:
	virtual bool				Start						();
	virtual bool				Stop						();

	int							GetBytesSent				();
	int							GetBytesRead				();
	int							GetConnectionCt				() { return m_sockServer.GetConnectionCt(); };

	bool						IsServerStarted				(){return m_bStarted;};
	int							GetLastErrorCode			(){return m_nLastErrorCode;};
	CString						GetLastError				(){return m_sLastError;};

	bool						SendPacketByConnectionId	(__int64 nConnectionId, BasePacket* p, bool bDontCheckSessionId = false);
	bool						SendPacket					(ClientConnection* pConn, BasePacket* p, bool bDontCheckSessionId = false);

	bool						HandlePacket				(BasePacket* pPacket, void* pParam){return m_pPacketHandler ? m_pPacketHandler->HandlePacket(pPacket, pParam) : NULL;};
	BasePacket*					CreatePacketByCode			(int nCode){ return m_pPacketHandler ? m_pPacketHandler->CreatePacketByCode(nCode) : NULL;};

protected:
	// Client connection. ############################
	ClientConnection*			CreateClientConnection		(ConnectedSocketInfo* pSockInfo, SocketServer* pSockServer);
	bool						DestroyClientConnection		(SOCKET sock, DWORD dwErrorCode);
	// ###############################################

protected:
	// Create or restore session.
	Session*					CreateOrRestoreSession		(ClientConnection* pConn, __int64 nSessionId = 0);

	// The way destroy session.
	bool						DestroySession				(Session* pSess);
	int							DestroyAllConnections		();

	bool						DispatchReceivedData		(SocketContext* pSockConn, BYTE* pData, DWORD dwSize, bool bIsFirstData);

protected: // Virtual methods.
	virtual GameServerConfig*	LoadConfiguration			(){
		GameServerConfig* pConf = new GameServerConfig();
		if( !pConf->DeserializeAsXML(_T("Config.xml")) ){
			delete pConf;
			return NULL;
			}
		return pConf;
		};

	virtual Session*			CreateSession				(ClientConnection* pConn){return new Session(this, pConn);};
	virtual void				OnDisconnected				(ClientConnection* pConn, DWORD dwLastError);
	virtual void				OnConnected					(ClientConnection* pConn, ConnectedSocketInfo* pSockInfo);
	virtual void				OnPacketReceived			(ClientConnection* pConn, BasePacket* packet);
	virtual void				OnSessionExpired			(Session* pSess);
	virtual	void				OnSessionRestored			(Session* pSess, ClientConnection* pConn);

	// Socket server events.
	virtual void				Event_OnConnected			(SocketServer* pSockServer, SOCKET sock, ConnectedSocketInfo* pSockInfo);
	virtual void				Event_OnAcceptConnection	(SocketServer* pSockServer, SOCKET	sock, const CString sRemoteIp, int nRemotePort, bool& bAccept);
	virtual void				Event_OnDataReceived		(SocketServer* pSockServer, SocketContext* pSockConn, BYTE* pData, DWORD dwSize, bool bIsFirstData);
	virtual void				Event_OnDisconnected		(SocketServer* pSockServer, SocketContext* pSockConn, SOCKET sockDisposed, DWORD dwLastError);

protected: // Packet receive listener methods.
	void						OnHandShake					(HandShakePacket* p, void* pParam);
	void						OnAcquireTableAccess		(AcquireTableAccessPacket* p, void* pParam);
	void						OnLogin						(LoginPacket* p, void* pParam);
	void						OnLogout					(LogoutPacket* p, void* pParam);
	void						OnRegisterUser				(RegisterUserPacket* p, void* pParam);

	// Packet code generation.
	bool						GeneratePacketSourceCode_CSharp		(int packetCode, _String& strSource);
	bool						GeneratePacketSourceCode_AS3		(int packetCode, const _String sDumpDir, const TCHAR* pszPackage, const TCHAR* pszSerializeBasePackage);
	bool						GeneratePacketSourceCode_ObjectiveC	(int packetCode, _String& strSource);

protected:
	bool					m_bStarted;					// Server is started.
	SocketServer			m_sockServer;				// Socket server.
	SocketServer			m_adminServer;				// Admin server.
	SocketServer			m_policyServer;				// Policy server.
	GameServerConfig*		m_pConfig;					// Game server configurations.
	
	CCriticalSection		m_lockSendPacket;	
	GrowableMemory			m_serializeMem;				// Memory for packet serialization.

	PacketHandler*			m_pPacketHandler;			// Packet handler.

	CCriticalSection		m_lockConnections;			
	AutoSortedArray			m_arrAcceptedSocks;			// Array of accepted sockets. <SOCKET><ClientConnection*>

	// Usage: When we are sending packet to client. it is naccessary to update packet size in serialized memory, so we should found out offset of variable in memory.
	AutoSortedArray			m_arrVariableOffset;		// Array of accepted sockets. <SOCKET><ClientConnection*>

	// Last error occured.
	int						m_nLastErrorCode;
	_String					m_sLastError;
};

