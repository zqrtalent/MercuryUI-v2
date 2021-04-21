#pragma once

/////////////////////////////////////////////////////////////////////////////
// LobbyInfo class

#include <afxmt.h>
#include "../../Serialize/Serializable.h"
#include "../../UI/Poker/PokerGUI.h"
#include "../../Protocol/_PokerDefines.h"
#include "../../Protocol/PokerPacket.h"
#include "../../Array/AutoSortedArrayInt64.h"

#include "LobbyCashTableInfo.h"
#include "LobbyTournamentInfo.h"

#include "../Data/Tables/PokerGameSpeed.h"
#include "../Data/Tables/PokerGameType.h"
#include "../Data/Tables/PokerGameSubType.h"
#include "../Data/Tables/PokerGameTable.h"
#include "../Data/Tables/TournamentInfo.h"

class PokerTableInfo;
class PokerTableSeatInfo;

class LobbyInfo : public Serializable
{
public:
	LobbyInfo();
	~LobbyInfo();

	friend class PokerServer;
	friend class PokerTournaments;
	friend class PokerCashGames;
	friend class PokerTournamentData;

public:
	bool		SendLobbyInfoByRequest			(ClientConnection* pConn, PokerGetLobbyInfoPacket* p);
	int			GetTablesCt						() { return m_nTablesCt; };
	int			GetTournamentsCt				() { return m_nTournamentsCt; };
	int			GetPlayersCt					() { return m_nPlayersCt; };
	void		SetPlayersCt					(int nPlayersCt) { m_nPlayersCt = nPlayersCt; };

protected:
	// Poker cash game. {{
	bool		SetTable						(PokerGameTable* pTable);
	bool		UpdateTableInfo					(PokerTableInfo* pTableInfo);

	int			SerializeCashTables				(GrowableMemory* pMemTableStaticInfo, GrowableMemory* pMemTableDynamicInfo, GameType type, GameSubType subType, int& nTablesCt);

//	int			CopyCashTablePlayers			(EnumerableObject<LobbyCashTablePlayerInfo>& pCopyInto, int nTableId, bool bCopyObjects);
	int			SerializeTournaments			(GrowableMemory* pMemInto, GameType type, GameSubType subType);
	int			SerializeTournamentDynamicInfos	(GrowableMemory* pMemInto, GameType type, GameSubType subType);

	int			GetTableId						(int nGameType, int nGameSubType, bool bIgnoreFullTable, AutoSortedArray& notIntableIds);
	bool		SeatTablePlayer					(int nTableId, int nSeat, PokerTableSeatInfo* pSeatInfo);
	bool		UnseatTablePlayer				(int nTableId, int nUserId);
	bool		SetTableDynamicInfo				(int nTableId, int nWaitedPlayersCt = -1, int nAvaragePotAmount = -1, int nHandPerHour = -1);
	bool		SetTableSeatAmount				(int nTableId, int nSeat, INT_CHIPS nAmount);
	bool		GetTableAttributes				(int nTableId, int& nPlayersCt, int& nWaitingPlayersCt, float& fAvgPot, int& nHandPerHour);
	// }}

	// Poker tournament game. {{
	bool		SetTournamentInfo				(TournamentInfo* pInfo);
	bool		SetTournamentTable				(int nTournamentId, PokerTableInfo* pTableInfo);
	bool		SetTournamentParticipant		(int nTournamentId, TournamentPlayerInfo* pPlayerInfo);
	bool		SetTournamentStatus				(int nTournamentId, TournamentStatus statusNew);
	bool		SetTournamentPlayersCt			(int nTournamentId, int nPlayersCt, int nRegistrantsCt);

	bool		PlayerFinishedTournament		(int nTournamentId, int nUserId, int nPlace, INT_CHIPS nWinAmount);
	int			GetPlayersPlaceInTournament		(int nTournamentId, int nUserId);

	bool		SetTournamentRegistrationStatus (int nTournamentId, bool bOpenStatus);

	bool		OnTournamentRebuy				(TournamentInfo* pInfo, int nUserId);
	bool		OnTournamentAddon				(TournamentInfo* pInfo, int nUserId);

	bool		FillCustomInfoPacket			(PokerTournamentCustomInfoPacket* p);
	bool		RebuildTournamentPayouts		(TournamentInfo* pInfo);

	bool		RemoveTournamentInfo			(int nTournamentId);
	bool		RemoveTournamentTable			(int nTournamentId, int nTableId);
	bool		RemoveTournamentParticipant		(int nTournamentId, int nUserId);
	// }}

	bool		FindPlayerTables				(AutoSortedArray* pArrTableIds, EnumerableObject<FindPlayerTableInfo> *pFindPlayerInfo);
	bool		FindTournament					(int nTournamentId, LobbyTournamentInfo* pFindInfo);
	void		EmptyInfo						();

private:
	volatile int										m_nPlayersCt;						// Players count.
	volatile int										m_nTablesCt;						// Tables count.
	volatile int										m_nTournamentsCt;					// Tournament table count.
	DateTime											m_dtLastUpdate;						// Last update datetime.

	CCriticalSection									m_lockLobbyInfoSend;				// Lobby info send lock.
	GrowableMemory*										m_pMemLobbyInfoSerializer;			// Lobby info serializer.

	// Cash game.
	CCriticalSection									m_lockCashGames;					// Lock critical section.
	EnumerableObject<LobbyCashTableStaticInfo>			m_listTableStaticInfos;				// LobbyCashTableStaticInfo objects list.
	EnumerableObject<LobbyCashTableDynamicInfo>			m_listTableDynamicInfos;			// LobbyCashTableDynamicInfo objects list.
	AutoSortedArrayInt64								m_arrStaticTableInfoByGameType;		// <[(int)gameType][(int)gameSubType]><LobbyCashTableStaticInfo*>

	// Tournament games.
	CCriticalSection									m_lockTournaments;					// Lock critical section.
	EnumerableObject<LobbyTournamentInfo>				m_listTournaments;					// LobbyTournamentInfo objects list.
	AutoSortedArrayInt64								m_arrLobbyTournamentInfoByGameType;	// <[(int)gameType][(int)gameSubType]><LobbyTournamentInfo*>

public:
	virtual Serializable* CreateSerializableObject(){return new LobbyInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};