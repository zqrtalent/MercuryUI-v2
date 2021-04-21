#pragma once
#include <afxsock.h>            // MFC socket extensions
#include <afxmt.h>
#include "..\Protocol\PacketHandler.h"
#include "..\Array\AutoSortedArray.h"
#include "..\Array\AutoSortedArrayInt64.h"
#include "..\Array\List.h"

class ClientConnection;
class GameServer;

struct ReceivedPacket{
	ClientConnection*	pConn;
	SOCKET				sockFrom;
	BasePacket*			packet;
};

class DataManageThread
{
public:
	DataManageThread(GameServer* pServer);
	virtual ~DataManageThread();
 
public:
	virtual bool		Start						();
	virtual bool		Stop						();

public:
	bool				ManageReceivedPacket		(ClientConnection* pConn, SOCKET sockFrom, BasePacket* packet);

protected: // Static thread procs.
	static UINT  __stdcall DataManage_Thread		(DataManageThread* pMan);

protected:
	GameServer*				m_pServer;
	HANDLE					m_threadDataManager;
	DWORD					m_dwDataManagerThreadId;
	CEvent					*m_pStopDataManager;
	CEvent					*m_pResumeDataManager;
	List<ReceivedPacket>	m_listReceivedData;			// Queue of received data list.
	CCriticalSection		m_lockDataManager;
};

