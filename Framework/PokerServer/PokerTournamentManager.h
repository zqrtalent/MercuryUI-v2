#pragma once
#include "TournamentTableController.h"
#include "Data/Tables/TournamentInfo.h"
#include "TournamentTablesMan.h"

enum MethodResult{
	Result_Success = 0,
	Result_Stopped,
	Result_TournamentCompleted,
	Result_CreateGameTablesFail,
	Result_InsufficientTournamentRegistrants,
	Result_BlindStructureNotExists
	};

class PokerTournaments;

class PokerTournamentManager : public Serializable
{
public:
	friend PokerTournaments;
	friend class PokerTournamentData;
	friend TournamentTableController;
	friend TournamentTablesManager;

	PokerTournamentManager			(PokerServer* pServer = NULL, PokerTournaments* pOwner = NULL);
	virtual ~PokerTournamentManager	();

protected:
	bool						Start						(bool bUseSyncBreak = true);
	void						Stop						(bool bSaveState);
	virtual void				SendStopSignal				();

	void						SetParam					(void* pParam)	{ m_pParam = pParam; };
	void*						GetParam					()				{ return m_pParam; };

	bool						CancelTournament			(bool bChangeStatus);
	bool						LateRegisterPlayer			(TournamentPlayerInfo* pPlayer, int nUserId, PokerSession* pSession);

	TournamentTableController*	CreateTableController		(int nTableId, bool bInitializeTableSeatsWithNull = false);
	TournamentTableController*	GetTableController			(int nTableId);
	CEvent*						GetSyncGameStartEvent		(){return m_pSyncGameStartEvent;};

	bool						IsRegisterUnregisterAllowed	(bool bRegisterAction = true);
	void						OnUserRegister				(int nUserId, _String sUserName, PokerSession* pSession, bool bUnregister = false);
	bool						UserRebuyAction				(int nUserId);
	bool						UserAddonAction				(int nUserId);

	void						SetHandForHand				(bool bHandForHand);
	void						SetFinishTournament			(bool bFinish);

	bool						IsRebuyAllowed				(PokerSession* pUserSession, int& nRebuyCtLeft, int nRebuyCtAtOnce = 1);
	bool						IsAddonAllowed				(PokerSession* pUserSession, int& nAddonCtLeft);
	void						IsRebuyAddonAllowed			(PokerSession* pUserSession, bool& bRebuyAllowed, bool& bAddonAllowed, int* pRebuyCtLeft = NULL, int* pAddonCtLeft = NULL);

	// Packet handler methods.
	void						OnOpenTable					(PokerSession* pSess, ClientConnection* pConn, PokerOpenTable_TournamentPacket* p);
	void						OnLeaveTable				(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTable_TournamentPacket* p);

	// Game table packets.
	void						OnAction					(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p);
	void						OnReturnBack				(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p);
	void						OnSitOut					(PokerSession* pSess, ClientConnection* pConn, PokerSitOutPacket* p);
	void						OnPostBlind					(PokerSession* pSess, ClientConnection* pConn, PokerPostBlindPacket* p);
	void						OnShowMuckCards				(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p);
	void						OnSetOption					(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p);
	void						OnChatMessage				(PokerSession* pSess, ClientConnection* pConn, PokerChatMessagePacket* p);

	void						OnRebuy						(PokerSession* pSess, ClientConnection* pConn, PokerRebuyPacket* p);
	void						OnAddon						(PokerSession* pSess, ClientConnection* pConn, PokerAddonPacket* p);

	// Tournament manager thread. #######################################
private:
	static LRESULT __stdcall	TournamentThread_Entry		(WinThread* pThread);
	MethodResult				PerformStartCountdown		(int nReturnBeforeStartSec = 0);
	MethodResult				SeatPlayers					(int& nElapsedTimeMS, int nStartTournamentTimeSec);
	MethodResult				PerformLevel				(int nLevelIndex);
	MethodResult				PerformBreak				(bool bAddonBreak, bool bKeepGameThreadsPaused);
	MethodResult				FinishCompetition			();
	MethodResult				SleepAndWait				(int nSecWait, int nMSecCheckStopEvent = 250);
	MethodResult				SleepMSAndWait				(int nMSecWait, int nMSecCheckStopEvent = 250);
	MethodResult				SyncStartPausedGameThreads	(int nWaitMSecInitial, int nWaitMSecInc);
	// ##################################################################

protected:
	// Tournament table controllers event methods.#######################
	void						OnHandStarted				(TournamentTableController* pTable);
	void						OnHandFinished				(TournamentTableController* pTable, bool& bTerminateGame, int nHandAvgDurationSec);
	void						OnSeatEmpty					(TournamentTableController* pTable, int nSeat, int nTableId, int nUserId);
	void						OnSeatTaken					(TournamentTableController* pTable, int nSeat, int nTableId, int nUserId);
	void						OnUserReseated				(TournamentTableController* pTableDest, int nTableIdDest, int nSeatDest, TournamentTableController* pTableFrom, int nTableIdFrom, int nSeatFrom, int nUserId);
	void						OnGameTableCreated			(TournamentTableController* pTable);
	void						OnGameTableClosed			(TournamentTableController* pTable);
	// ##################################################################

public:
	int							GetTournamentId				(){ return m_info.m_nId;};
	const TournamentInfo*		GetTournamentInfo			(){ return &m_info; };
	int							GetPlayersCount				(){ return m_nPlayersCt; };
	int							GetTablesCount				(){ return m_nTableCt; };
	bool						GetLevelStakes				(int nLevel, UINT& nSB, UINT& nBB, UINT& nAnte);
	int							GetCurrentLevelIndex		(){ return m_nLevelIndex; };
	bool						IsRebuyAllowed				(){ return m_bRebuyIsAllowed; };
	bool						IsAddonAllowed				(){ return (m_bIsBreak && m_bAddonBreak); };
	bool						isTournamentInProgress		(){ return m_bTournamentIsInProgress; };
	int							GetRebuyCtMax				(){ return (int)m_info.m_shRebuyCt; };
	int							GetRebuyChipsCt				(){ return m_info.m_nRebuyChips; };
	UINT						GetRebuyAmount				(){ return (UINT)Serializable::ConvertMoney(m_info.m_dRebuy); };
	UINT						GetRebuyVPPAmount			(){ return (UINT)Serializable::ConvertMoney(m_info.m_dRebuyVPP); };

	int							GetAddonCtMax				(){ return (int)m_info.m_shAddonCt; };
	int							GetAddonChipsCt				(){ return m_info.m_nAddonChips; };
	UINT						GetAddonAmount				(){ return (UINT)Serializable::ConvertMoney(m_info.m_dAddon); };
	UINT						GetAddonVPPAmount			(){ return (UINT)Serializable::ConvertMoney(m_info.m_dAddonVPP); };

	int							GetLevelDurationSec			(){ return m_info.m_shLevelDuration*60; };
	int							GetBreakDurationSec			(){ return m_info.m_shBreakDurationMin*60; };
	int							GetBreakPerLevel			(){ return m_info.m_shBreakPerLevel; };
	UINT						GetStartingChips			(){ return m_info.m_nBuyInChips; };
	TournamentStatus			GetTournamentStatus			(){ return m_tournamentStatus; };

private:
	void						SetTournamentStatus			(TournamentStatus statusNew);
	void						SetTournamentRegStatus		(bool bRegIsOpen);
	int							GetSecondsUntilRegWillClose	();
	void						SetTournamentVisibility		(bool bVisible);
	int							SendBreakPacketToAllTables	(bool bAddonBreak);
	int							PerformAutoAddonOnAllTables	();

	static clock_t				StartCountdown				();
	static int					EndCountdown				(clock_t start); // Returns elapsed time in MS.

protected:
	PokerServer*						m_pServer;
	PokerTournaments*					m_pOwner;
	void*								m_pParam;						// Param object.

	List<TournamentTableController>		m_listTables;					// Tournament tables.
	AutoSortedArray						m_arrGameTableById;				// <table id><TournamentTableController*>
	CCriticalSection					m_lockGameTables;
	WinThread							m_tournamentThread;				// Tournament manager thread.

	TournamentTablesManager				m_tablesMan;					// Tournament tables manager.
	bool								m_bVisible;						// Tournament visibility.
	bool								m_bLateRegistration;			// Tournament late registration option.
	TournamentInfo						m_info;							// Tournament info.

	TournamentBlindStructure			m_nextLevel;					// Tournament's next level stakes.
	TournamentBlindStructure			m_currLevel;					// Tournament's current level stakes.

	volatile bool						m_bTournamentIsInProgress;		// Tournament status.	
	volatile bool						m_bRegistrationStatus;			// Tournament registration status.
	volatile int						m_nLevelIndex;					// Current level index.
	volatile bool						m_bAddonBreak;					// Addon break.
	volatile bool						m_bWaitingToStartBreak;			// Waiting for tables to start break.
	volatile bool						m_bIsBreak;						// Tournament break time.
	bool								m_bStartBreakSyncronously;		// Start break simultaneously.

	volatile int						m_nTournamentStartSecondsLeft;	// Tournament start seconds remaining.
	volatile int						m_nBreakSecondsLeft;			// Break seconds remaining.
	volatile int						m_nLevelSecondsLeft;			// Level seconds remaining.
	volatile int						m_nNextBreakSecondsLeft;		// Next break seconds remaining.
	volatile bool						m_bHandForHand;					// Hand for hand competion is active.

	volatile int						m_nTableCt;						// Game table count.
	volatile bool						m_bRebuyIsAllowed;				// Rebuy is allowed.

	// Playing user ids by table id.
	AutoSortedArray						m_arrUsersPlaying;				// <User Id><Table Id>
	CCriticalSection					m_lockUsersPlaying;

	volatile int						m_nPlayersCt;					// Players count.
	volatile int						m_nRegisteredCt;				// Registered players count.

	volatile INT_CHIPS					m_nStakeHighest;				// Highest stake.
	volatile INT_CHIPS					m_nStakeAverage;				// Average stake.
	volatile INT_CHIPS					m_nStakeLowest;					// Lowest stake.
	volatile TournamentStatus			m_tournamentStatus;				// Tournament status.

	CCriticalSection					m_lockGameTablesSync;			// Game tables syncronization lock.
	CEvent*								m_pSyncGameStartEvent;			// Sync game start event.
	AutoSortedArray						m_arrHandInProgressGameTableIds;// Game table ids whose hand is in progress.

	volatile int						m_nStartedTableCt;				// Started game table count.
	volatile int						m_nFinishedTableCt;				// Finished game table count.

private:
	volatile bool						m_bCancelling;					// Cancelling tournament.
	volatile bool						m_bDestroying;					// Destroying tournament manager.
	volatile bool						m_bFinishTournament;			// Finish tournament.

protected: // Serialization.	
	virtual Serializable* CreateObject(){return new TournamentTableController();};
	INIT_RUNTIME_VARIABLE()

};
