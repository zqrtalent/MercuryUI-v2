#pragma once
#include "..\Array\StringToPtrArray.h"
#include "..\Array\AutoSortedArrayInt64.h"
#include "..\SafePointer\SafePointer.h"

class GameServer;
class ClientConnection;
class Session : public SafePointer
{
public:
	Session(GameServer* pServer, ClientConnection* pConn = NULL);
	virtual ~Session();

	ClientConnection*				GetConnection			(bool bRetain, bool bNullWhenInactive);	
	void							SetConnection			(ClientConnection* pConn, bool bDestroyOldConn);
	ClientConnection*				GetTableConnection		(int nTableId, int nTournamentId, bool bRetain, bool bNullWhenInactive);	
	void							SetTableConnection		(int nTableId, int nTournamentId, ClientConnection* pConn, bool bDestroyOldConn);

	void*							Get						(const _String sKeyName);
	bool							Set						(const _String sKeyName, void* lpValue);
	bool							Remove					(const _String sKeyName, void** lppValue = NULL);
	int								RemoveAll				(bool bDestoryObjects);
	int								RemoveAllGetValues		(CPtrVector& m_arrValues);
	__int64							GetSessionId			(){return m_sessionId;};
	bool							IsActive				(){return m_bActive;};
	GameServer*						GetServer				(){return m_pServer;};

	virtual int						GetUserId				(){return 0;};
	virtual bool					IsChatBlocked			(){return false;};
	virtual bool					IsAvatarBlocked			(){return false;};
	
public:
	static Session*					CreateNewSession		(GameServer* pServer, ClientConnection* pConn, bool bRetain);
	static Session*					RestoreInactiveSession	(__int64 nSessionId, ClientConnection* pConn, bool bRetain);	
	static bool						DestroySession			(Session* pSess);
	static Session*					GetSessionById			(__int64 nSessionId, bool bRetain);
	static void						DestroyAllSessions		();

protected:
	void							SetActive				(bool bActive){m_bActive = bActive;};
	int								ReleaseTableConnections	();

protected:
	static __int64					GenerateNewSessionId	(Session* pSess);
	static AutoSortedArrayInt64*	GetArraySessionIds		(){
		static AutoSortedArrayInt64 arrSessionIds; // <Session Id><Session*>
		arrSessionIds.OptimizeForLargeData(1);
		return &arrSessionIds;
		}

	static CCriticalSection*		GetGlobalLock(){
		static CCriticalSection lockSection;
		return &lockSection;
		}

	static CCriticalSection*		GetTableConnectionLock(){
		static CCriticalSection lockSection;
		return &lockSection;
		}

	static void						LockGlobalSession		(){GetGlobalLock()->Lock();}	
	static void						UnLockGlobalSession		(){GetGlobalLock()->Unlock();}	

	static bool						RemoveSessionIdFromList	(__int64 nSessionId);
	static bool						AddSessionIdIntoList	(__int64 nSessionId, Session* pSess);

	static bool						OnSessionDeleting		(Session* pSess);

protected:
	__int64												m_sessionId;
	ClientConnection*									m_pConnection;				// Main connection.
	AutoSortedArrayTempl<__int64, ClientConnection*>	m_listTableConns;
	GameServer*											m_pServer;
	StringToPtrArray									m_mapKeyValue;
	COleDateTime										m_dTimeLastCommandRecv;		// Last command receive time.
	volatile bool										m_bActive;
};