#pragma once
#include "../Serialize/Serializable.h"
#include "../Array/AutoSortedArray.h"

typedef struct GameTableSeatsTag{
	int							nTableId;
	char						arrSeats[10];				// Seats occupied by index.
	INT_CHIPS					arrBalances[10];			// Seats balances.
	int							nEmptySeatCt;				// Empty seat count.
	int							nOccupiedSeatCt;			// Occupied seat count.
	
	INT_CHIPS					nAvgBalance;				// Avarage balance of table.
	INT_CHIPS					nBalanceHighest;			// Highest balance amount of table.
	INT_CHIPS					nBalanceLowest;				// Lowest balance amount of table.
	INT_CHIPS					nSumChips;					// Sum of all chips on the table.

	bool						bHandStarted;				// Hand progress status.
	bool						bWaitingForPlayersToStart;	// Game thread is resumed and waiting for sufficient players count to start.
	int							nDealer;					// Dealer position.
	int							nBB;						// Bigblind position.
	int							nSB;						// Smallblind position.
	TournamentTableController*	pGameTable;
	} GameTableSeats;

class PokerTournamentManager;
class TournamentTablesManager : public Serializable
{
public:
	TournamentTablesManager();
	virtual ~TournamentTablesManager();

	// Retrieves array of integers  which represents occoupied seats count of each table. 
	static int*		SeatPlayersOnTables			(int nPlayersCt, int nMaxSeatCt, int& nTableCt, int& nAvgPlayerCtPerTable);

	void			Initialize					(PokerTournamentManager* pMan, List<TournamentTableController>* pListTables, int nMaxSeatCt);

	bool			SeatLatelyRegisteredPlayer	(PokerTournamentManager* pMan, TournamentPlayerInfo* pPlayer, int nUserId, PokerSession* pSess);
		
	void			HandStarted					(int nTableId, TournamentTableController* pTable);
	void			HandFinished				(int nTableId, TournamentTableController* pTable, int& nTableCt);

	void			PlayerAddedChips			(int nUserId, INT_CHIPS nChipsOld, INT_CHIPS nChipsNew);
	bool			IsTableActive				(int nTableId);

	void			GetStakesStat				(INT_CHIPS* pStakeHighest, INT_CHIPS* pStakeLowest, INT_CHIPS* pStakeAvg){
		ASSERT(pStakeHighest && pStakeLowest && pStakeAvg);
		*pStakeHighest	= m_nStakeHighest;
		*pStakeLowest	= m_nStakeLowest;
		*pStakeAvg		= m_nStakeAverage;
		}

protected:
	void			OptimizeTablesAndSeats		(GameTableSeats* p, int &nAvgPlayerCtPerTable);

	int				FindSeatClosestToBalance	(GameTableSeats* p, INT_CHIPS nBalanceCloseTo);
	int				FindEmptySeat				(GameTableSeats* pDest, GameTableSeats* pSrc, int nSeatSrc, bool& bSkipBB);
	bool			FindEmptySeatOnAllTables	(int nIndexStart, int nMaxOccupiedSeatCt, int nSeatCtWithoutTable, int& nTableIndex, int& nSeatIndex, bool bRandomTables = false);
	int				FindOccupiedSeat			(GameTableSeats* p);
	void			RecalcStakesStatistic		(GameTableSeats* p);

	GameTableSeats*	GetTableSeatsInfo			(int nTableId);
	bool			DeleteTableSeatsInfo		(int nTableId, bool bDestroyObject, bool bReducePlayersCount);

	bool			AddTableWaitingForPlayers	(int nTableId);
	bool			RemoveTableWaitingForPlayers(int nTableId);

protected:
	CCriticalSection			m_lock;
	PokerTournamentManager*		m_pMan;
	List<GameTableSeatsTag>		m_listTableSeats;
	AutoSortedArray				m_arrSeatStructByTableId;		// <table id><GameTableSeatsTag*>
	AutoSortedArray				m_arrTablesWaitingForPlayers;	// <table id><not used> Ids of tables paused and waiting for players to resume.
	int							m_nPlayersCt;					// Players count.
	int							m_nMaxSeatCt;					// Max seat count of each table.
	int							m_nTableCt;						// Game table count.

	int							m_nTableIdNew;					// GAme table id for new table.

	// Stakes statistic.
	int							m_nHighestStakeTableId;
	int							m_nLowestStakeTableId;
	volatile INT_CHIPS			m_nStakeHighest;
	volatile INT_CHIPS			m_nStakeLowest;
	volatile INT_CHIPS			m_nStakeAverage;
	volatile INT_CHIPS			m_nSumChips;

protected: // Serialization.	
	virtual Serializable* CreateObject(){return new TournamentTablesManager();};
	INIT_RUNTIME_VARIABLE()

};
