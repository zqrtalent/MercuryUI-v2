#pragma once
#include <afxsock.h>            // MFC socket extensions
#include <afxmt.h>
#include "..\Array\AutoSortedArray.h"
#include "..\Array\List.h"
#include "..\SafePointer\SafePointer.h"

class Session;
class SocketContext;
class SocketServer;
struct ConnectedSocketInfo;
class GrowableMemory;

class ClientConnection : public SafePointer
{
public:
	friend class GameServer;

	ClientConnection(ConnectedSocketInfo* pSockInfo, GameServer* pServer, SocketContext* pSockConn, SocketServer* pSockServer);
	virtual ~ClientConnection();

	void				SetSession				(Session* pSession);
	Session*			GetSession				(bool bRetain);
	__int64				GetSessionId			();

	void				SetAuthorized			(bool bAuthorized){m_bAuthorized = bAuthorized;};
	bool				IsAuthorized			() {return m_bAuthorized;};
	void				SetSendingStatus		(bool bStatus){m_bSending = bStatus;};
	bool				GetSendingStatus		(){return m_bSending;};
	SOCKET				GetSock					(){return m_sock;};
	SocketServer*		GetSocketServer			(){return m_pSockServer;};

	bool				Send					(GrowableMemory* pMem);
	bool				SendBytes				(BYTE* lpBytes, UINT nSize);

	bool				IsConnected				(){return m_bConnected;};
	void				SetConnected			(bool bConnected) {m_bConnected = bConnected;};

	_String				GetRemoteIpAddress		(){return m_sIpRemote;};
	int					GetRemotePort			(){return m_nPortRemote;};
	_String				GetLocalIpAddress		(){return m_sIpLocal;};
	int					GetLocalPort			(){return m_nPortLocal;};

	ClientConnection*	GetCopy					(bool bKeepSession = false);
	DWORD				GetRemoteIp				(){return m_dwIpRemote;};

	void				SetTableConnection		(int nTableId, int nTournamentId)	{ m_nTableId = nTableId; m_nTournamentId = nTournamentId; m_bTableConnection = true; };
	bool				IsTableConnection		()									{ return m_bTableConnection; };
	int					GetTableId				()									{ return m_nTableId; };
	int					GetTournamentId			()									{ return m_nTournamentId; };

protected:
	bool				Initialize				(ConnectedSocketInfo* pSockInfo, GameServer* pServer);

protected:
	__int64				m_nConnectionId;	// Unique identifier of connection object.
	bool				m_bConnected;		// Connected status.
	bool				m_bSending;			// Packet sending status.

	SOCKET				m_sock;
	GameServer*			m_pServer;
	Session*			m_pSession;
	SocketContext*		m_pSockConn;
	SocketServer*		m_pSockServer;

	DWORD				m_dwIpRemote;
	_String				m_sIpRemote;
	int					m_nPortRemote;
	_String				m_sIpLocal;
	int					m_nPortLocal;

	volatile bool		m_bAuthorized;
	volatile bool		m_bTableConnection;	// Table connection status.
	int					m_nTableId;
	int					m_nTournamentId;
};