#include "stdafx.h"
#include "PokerTournamentManager.h"
#include "TournamentTablesMan.h"
#include "Log/Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class TournamentSeatsArranger
///
///////////////////////////////////////////////////////////

TournamentTablesManager::TournamentTablesManager() {
	m_pMan						= NULL;
	m_nMaxSeatCt				= 0;
	m_nPlayersCt				= 0;
	m_nTableCt					= 0;
	m_nStakeHighest				= m_nStakeLowest = m_nStakeAverage = 0;
	m_nSumChips					= m_nLowestStakeTableId	= m_nHighestStakeTableId = 0;
	m_nTableIdNew				= 0;
	m_arrSeatStructByTableId	.OptimizeForLargeData(1);
	m_arrTablesWaitingForPlayers.SetSize(100);
	}

BEGIN_DECLARE_VAR(TournamentTablesManager, Serializable)
	DECLARE_VAR(_T("TournamentTablesManager"),	_T(""),		VariableType::VT_None, 0, false, true, false)
END_DECLARE_VAR()

TournamentTablesManager::~TournamentTablesManager(){
	m_listTableSeats		.RemoveAll();
	m_arrSeatStructByTableId.DeleteAll();
	}

int*
TournamentTablesManager::SeatPlayersOnTables(int nPlayersCt, int nMaxSeatCt, int& nTableCt, int& nAvgPlayerCtPerTable){
	nTableCt						= nPlayersCt/nMaxSeatCt + (nPlayersCt%nMaxSeatCt > 0 ? 1 : 0);
	nAvgPlayerCtPerTable			= (nPlayersCt / nTableCt);
	int		nPlayersOutOfTable		= nPlayersCt - nAvgPlayerCtPerTable*nTableCt;
	int*	pArrPlayersPerTable		= new int[nTableCt];

	if( nPlayersOutOfTable > 0 ){
		for( int i=0; i<nPlayersOutOfTable; i++ )
			pArrPlayersPerTable[i]=nAvgPlayerCtPerTable + 1;
		for( int i=nPlayersOutOfTable; i<nTableCt; i++ )
			pArrPlayersPerTable[i]=nAvgPlayerCtPerTable;
		}
	else{
		for( int i=0; i<nTableCt; i++ )
			pArrPlayersPerTable[i]=nAvgPlayerCtPerTable;
		}
	return pArrPlayersPerTable;
	}

void
TournamentTablesManager::Initialize(PokerTournamentManager* pMan, List<TournamentTableController>* pListTables, int nMaxSeatCt){
	ASSERT(pListTables);
	m_lock.Lock(); // Lock
	m_pMan				= pMan;
	m_nMaxSeatCt		= nMaxSeatCt;
	m_nTableCt			= 0;
	m_nPlayersCt		= 0;
	m_listTableSeats		.RemoveAll();
	m_arrSeatStructByTableId.DeleteAll();

	for(int i=0; i<pListTables->GetCount(); i++){
		TournamentTableController* pTable = pListTables->GetAt(i);
		if( pTable ){
			GameTableSeatsTag* pStruct			= new GameTableSeatsTag;
			pStruct->nOccupiedSeatCt			= pTable->GetSeatsAvailability((bool*)pStruct->arrSeats, pStruct->arrBalances);
			pStruct->nEmptySeatCt				= (nMaxSeatCt - pStruct->nOccupiedSeatCt);
			pStruct->nTableId					= pTable->GetTableId();
			pStruct->pGameTable					= pTable;

			pStruct->nAvgBalance				= 0;
			pStruct->nBalanceHighest			= 0;
			pStruct->nBalanceLowest				= 0;
			pStruct->nSumChips					= 0;

			pStruct->bHandStarted				= false;
			pStruct->bWaitingForPlayersToStart	= false;
			pStruct->nDealer					= 0;
			pStruct->nBB						= 0;
			pStruct->nSB						= 0;

			m_nPlayersCt	+= (nMaxSeatCt - pStruct->nEmptySeatCt);
			m_nTableCt		++;

			RecalcStakesStatistic(pStruct);
			m_listTableSeats.Add(pStruct);
			m_arrSeatStructByTableId.Add((void*)pStruct->nTableId, (void*)pStruct);
			}
		}
	m_nTableIdNew = (m_nTableCt + 1);
	m_lock.Unlock(); // Unlock
	}

bool
TournamentTablesManager::SeatLatelyRegisteredPlayer(PokerTournamentManager* pMan, TournamentPlayerInfo* pPlayer, int nUserId, PokerSession* pSess){
	if( !pMan || !pPlayer || nUserId <= 0 || !pSess )
		return false;

	m_lock.Lock();		// Lock
	int				nAvgPlayerPerTable	= (m_nPlayersCt / m_nTableCt);
	int				nTableIndex			= 0;
	int				nSeatIndex			= 0;
	INT_CHIPS		nBalance			= pPlayer->m_nChips;
	bool			bRet				= false;
	bool			bGameTableCreated	= false;
	GameTableSeats* pStruct				= NULL;

	// Find empty seat and seat recently registered player.
	if( nAvgPlayerPerTable != m_nMaxSeatCt && FindEmptySeatOnAllTables(0, nAvgPlayerPerTable + 1, 0, nTableIndex, nSeatIndex, true) ){
		pStruct = m_listTableSeats.GetAt(nTableIndex);
		ASSERT(pStruct);
		if( !pStruct ){
			m_lock.Unlock();	// Unlock
			return false;
			}
		}
	// Create new tournament game table controller and seat registered player there.
	else{
		TournamentTableController* pTable = pMan->CreateTableController(m_nTableIdNew, true);
		ASSERT( pTable );
		if( !pTable ){
			m_lock.Unlock();	// Unlock
			return false;
			}

		pStruct								= new GameTableSeatsTag;
		pStruct->nOccupiedSeatCt			= 0;
		pStruct->nEmptySeatCt				= m_nMaxSeatCt;
		pStruct->nTableId					= pTable->GetTableId();
		pStruct->pGameTable					= pTable;

		memset(pStruct->arrBalances, 0, sizeof(pStruct->arrBalances));
		memset(pStruct->arrSeats, 0, sizeof(pStruct->arrSeats));

		pStruct->nAvgBalance				= 0;
		pStruct->nBalanceHighest			= 0;
		pStruct->nBalanceLowest				= 0;
		pStruct->nSumChips					= 0;

		pStruct->bWaitingForPlayersToStart	= true;
		pStruct->bHandStarted				= false;
		pStruct->nDealer					= 0;
		pStruct->nBB						= 0;
		pStruct->nSB						= 0;

		m_nTableCt					++;
		m_listTableSeats		.Add(pStruct);
		m_arrSeatStructByTableId.Add((void*)pStruct->nTableId, (void*)pStruct);
		RecalcStakesStatistic(pStruct);

		AddTableWaitingForPlayers(pStruct->nTableId);

		// Seat player index.
		nSeatIndex							= 0;
		// Increase new table id.
		m_nTableIdNew						++;
		bGameTableCreated					= true;
		}

	ASSERT(pStruct->pGameTable);
	if( pStruct->pGameTable ){
		PokerTableSeatInfo* pSeat = new PokerTableSeatInfo();
		pSeat->SetUserId	(nUserId);
		pSeat->SetBalance	(nBalance);
		pSeat->SetUserName	(pPlayer->m_sUserName);

		TournamentTableController* pGameTable = pStruct->pGameTable;
		if( !pGameTable->SeatSessionAtTable(pSeat, pSess, nSeatIndex) ){
			delete pSeat;
			}
		else{
			pStruct->nOccupiedSeatCt			++;
			pStruct->nEmptySeatCt				--;
			pStruct->arrSeats[nSeatIndex]		= true;
			pStruct->arrBalances[nSeatIndex]	= nBalance;
			m_nPlayersCt						++;
			m_nSumChips							+= nBalance;
			RecalcStakesStatistic(pStruct);

			bRet = true;
			}

		// Game table created !!!
		if( bGameTableCreated )
			pMan->OnGameTableCreated(pGameTable);

		// Start hand on game table which was waiting for players to start.
		if( pStruct->bWaitingForPlayersToStart && pStruct->nOccupiedSeatCt >= nAvgPlayerPerTable ){
			pStruct->bWaitingForPlayersToStart	= false;
			RemoveTableWaitingForPlayers(pStruct->nTableId);
			pStruct->bHandStarted				= true;
			pStruct->pGameTable->GThread_Resume();
			}
		}

	m_lock.Unlock();	// Unlock
	return bRet;
	}

void
TournamentTablesManager::HandStarted(int nTableId, TournamentTableController* pTable){
	m_lock.Lock();		// Lock
	GameTableSeats* pStruct = GetTableSeatsInfo(nTableId);
	if( pStruct ){
		pStruct->bHandStarted				= true;
		pStruct->bWaitingForPlayersToStart	= false;
		RecalcStakesStatistic(pStruct);
		}
	m_lock.Unlock();	// Unlock
	}

void
TournamentTablesManager::HandFinished(int nTableId, TournamentTableController* pTable, int& nTableCt){
	m_lock.Lock();		// Lock
	GameTableSeats* p		= GetTableSeatsInfo(nTableId);
	if( p ){
		p->bHandStarted		= false;
		int nSeatsCtPrev	=  (m_nMaxSeatCt - p->nEmptySeatCt);
		int nSeatsCt		= pTable->GetSeatsAvailability((bool*)p->arrSeats, p->arrBalances);
		int nSeatsLeft		= (nSeatsCtPrev - nSeatsCt);
		p->nEmptySeatCt		= (m_nMaxSeatCt - nSeatsCt);
		p->nOccupiedSeatCt	= nSeatsCt;
		m_nPlayersCt		-= nSeatsLeft;
		RecalcStakesStatistic(p);

		// Optimize tables and seats.
		int nAvgPerTable = 0;
		OptimizeTablesAndSeats(p, nAvgPerTable);
		/*
		_String sLog;
		sLog.Format(_T("TournamentTablesManager::HandFinished tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d-5, isWaiting=%d, avg=%d, threadId=%d"), p->nTableId, p->nEmptySeatCt, 
			p->nOccupiedSeatCt, (int)p->bWaitingForPlayersToStart, nAvgPerTable, GetCurrentThreadId());
			Logger::CreateLogEntry(sLog.GetBuffer());

		if( nAvgPerTable > 1 && p->nOccupiedSeatCt < nAvgPerTable && !p->bWaitingForPlayersToStart ){
			bool bAdded = AddTableWaitingForPlayers(p->nTableId);
			p->bWaitingForPlayersToStart = true;
			p->pGameTable->GThread_Pause();
					
			sLog.Format(_T("Pause game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d-5 added=%d, threadId=%d"), p->nTableId, p->nEmptySeatCt, 
				p->nOccupiedSeatCt, (int)bAdded, GetCurrentThreadId());
			Logger::CreateLogEntry(sLog.GetBuffer());
			}*/
		}
	nTableCt			= m_nTableCt;
	m_lock.Unlock();	// Unlock
	}

void
TournamentTablesManager::PlayerAddedChips(int nUserId, INT_CHIPS nChipsOld, INT_CHIPS nChipsNew){
	// Update sum of all chips.
	m_nSumChips += (nChipsNew - nChipsOld);
	}

bool
TournamentTablesManager::IsTableActive(int nTableId){
	m_lock.Lock();		// Lock
	GameTableSeats* p		= GetTableSeatsInfo(nTableId);
	m_lock.Unlock();	// Unlock
	return (p != NULL);
	}

void
TournamentTablesManager::OptimizeTablesAndSeats(GameTableSeats* p, int &nAvgPlayerCtPerTable){
	ASSERT(p);
	int						nTableCtOpt					= m_nPlayersCt/m_nMaxSeatCt + (m_nPlayersCt%m_nMaxSeatCt > 0 ? 1 : 0);
	int						nAvgPlayerPerTable			= max(2, (m_nPlayersCt / nTableCtOpt));
//	int						nPlayersOutOfTable			= m_nPlayersCt - nAvgPlayerPerTable*nTableCtOpt;
	int						nSeatCtAvailableToReseat	= 0;
	nAvgPlayerCtPerTable								= nAvgPlayerPerTable;

	AutoSortedArray			arrMoreThanAvgSeatTables;
	AutoSortedArray			arrLessThanAvgSeatTables;
	AutoSortedArray			arrFinishedTablesByOccoupiedSeatsCt;
	AutoSortedArray			arrCloseTables;
	/*
	TRACE("Optimize tableId=%d EmptySeats=%d OccupiedSeats=%d\r\n", p->nTableId, p->nEmptySeatCt, p->nOccupiedSeatCt);
	TRACE("AverageSeatct=%d TableCtOld=%d TableCTNew=%d\r\n", nAvgPlayerPerTable, m_nTableCt, nTableCtOpt);

	_String sLog;
	sLog.Format(_T("OptimizeTablesAndSeats tableId=%d, seat1Balance=%d, seat2Balance=%d, threadId=%d"), 
		p->nTableId, (UINT)p->arrBalances[0], (UINT)p->arrBalances[1], GetCurrentThreadId());
	Logger::CreateLogEntry(sLog.GetBuffer());
	*/
	// No need to change table count we just only balance count of players on tables.
	if( nTableCtOpt == m_nTableCt ){
		for(int i=0; i<m_nTableCt; i++){
			GameTableSeats* p1 = m_listTableSeats.GetAt(i);
			if( !p1 ) continue;
			if( p1->nOccupiedSeatCt < nAvgPlayerPerTable )
				arrLessThanAvgSeatTables.Add((void*)i);
			else{
				if( p1->nOccupiedSeatCt > nAvgPlayerPerTable && !p1->bHandStarted ){
					arrMoreThanAvgSeatTables.Add((void*)i);
					nSeatCtAvailableToReseat += (p1->nOccupiedSeatCt - nAvgPlayerPerTable);
					}
				}
			}

		for(UINT i=0; i<arrLessThanAvgSeatTables.GetCount(); i++){
			GameTableSeats* pTableSeatIn	= m_listTableSeats.GetAt((int)arrLessThanAvgSeatTables.Get(i));
			ASSERT( pTableSeatIn );
			int				nNeedSeatCt		= (nAvgPlayerPerTable - pTableSeatIn->nOccupiedSeatCt);
			int				nLoop			= 0;
			int				nCt				= min(nNeedSeatCt, nSeatCtAvailableToReseat);

			while( nLoop < nCt ){
				if( arrMoreThanAvgSeatTables.GetCount() == 0 )
					break;
				GameTableSeats*				pTableSeatFrom		= m_listTableSeats.GetAt((int)arrMoreThanAvgSeatTables.Get(0));
				ASSERT(pTableSeatFrom);
				TournamentTableController*	pGameTableSeatIn	= pTableSeatIn->pGameTable;
				TournamentTableController*	pGameTableSeatFrom	= pTableSeatFrom->pGameTable;

				bool						bSkipBB				= false;
				int							nSeatSrc			= FindSeatClosestToBalance	(pTableSeatFrom, pTableSeatIn->nAvgBalance);
				int							nSeatDest			= FindEmptySeat				(pTableSeatIn, pTableSeatFrom, nSeatSrc, bSkipBB);
				int							nBalanceSrcSeat		= 0;

				if( nSeatSrc > -1 && nSeatDest > -1 && pGameTableSeatIn->SeatFromAnotherTable(nSeatDest, pGameTableSeatFrom, nSeatSrc) ){
					pTableSeatIn->arrSeats[nSeatDest]		= 1;
					pTableSeatIn->arrBalances[nSeatDest]	= pTableSeatFrom->arrBalances[nSeatSrc];
					pTableSeatIn->nEmptySeatCt				--;
					pTableSeatIn->nOccupiedSeatCt			++;

					pTableSeatFrom->arrSeats[nSeatSrc]		= 0;
					pTableSeatFrom->arrBalances[nSeatSrc]	= 0;
					pTableSeatFrom->nEmptySeatCt			++;
					pTableSeatFrom->nOccupiedSeatCt			--;

					RecalcStakesStatistic(pTableSeatIn);
					RecalcStakesStatistic(pTableSeatFrom);
					/*
					_String sLog;
					sLog.Format(_T("SeatFromAnotherTable tableIdFrom=%d, tableIdTo=%d, occoupiedNew=%d, threadId=%d"), 
						pTableSeatFrom->nTableId, pTableSeatIn->nTableId, pTableSeatIn->nOccupiedSeatCt, GetCurrentThreadId());
					Logger::CreateLogEntry(sLog.GetBuffer());
					*/
//					nPlayersOutOfTable						--;
					nSeatCtAvailableToReseat				--;
					}

				if( pTableSeatFrom->nOccupiedSeatCt == nAvgPlayerPerTable ){
					arrMoreThanAvgSeatTables.Delete(0);
					/*if( arrMoreThanAvgSeatTables.GetCount() == 0 )
						return;*/
					}
				nLoop ++;
				}

			// Pause / Resume table controller thread.
			if( pTableSeatIn->nOccupiedSeatCt > 0 && pTableSeatIn->pGameTable ){
				if( pTableSeatIn->nOccupiedSeatCt < nAvgPlayerPerTable ){
					//TRACE("Pause game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d", pTableSeatIn->nTableId, pTableSeatIn->nEmptySeatCt, pTableSeatIn->nOccupiedSeatCt);
					bool bAdded = AddTableWaitingForPlayers(pTableSeatIn->nTableId);
					pTableSeatIn->bWaitingForPlayersToStart = true;
					pTableSeatIn->pGameTable->GThread_Pause();
					/*
					_String sLog;
					sLog.Format(_T("Pause game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d-1 added=%d, threadId=%d"), pTableSeatIn->nTableId, pTableSeatIn->nEmptySeatCt, 
						pTableSeatIn->nOccupiedSeatCt, (int)bAdded, GetCurrentThreadId());
					Logger::CreateLogEntry(sLog.GetBuffer());*/
					}
				else{
					//TRACE("Resume game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d", pTableSeatIn->nTableId, pTableSeatIn->nEmptySeatCt, pTableSeatIn->nOccupiedSeatCt);
					RemoveTableWaitingForPlayers(pTableSeatIn->nTableId);
					pTableSeatIn->bWaitingForPlayersToStart = false;
					pTableSeatIn->pGameTable->GThread_Resume();
					/*
					_String sLog;
					sLog.Format(_T("Resume game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d-2, threadId=%d"), pTableSeatIn->nTableId, pTableSeatIn->nEmptySeatCt, 
						pTableSeatIn->nOccupiedSeatCt, GetCurrentThreadId());
					Logger::CreateLogEntry(sLog.GetBuffer());*/
					}
				}
			}
		}
	else{
		for(int i=0; i<m_nTableCt; i++){
			GameTableSeats* p1 = m_listTableSeats.GetAt(i);
			if( !p1 ) continue;
			if( !p1->bHandStarted )
				arrFinishedTablesByOccoupiedSeatsCt.Add((void*)p1->nOccupiedSeatCt, (void*)i);
			}

		// Gather closing tables. {{
		int nTableCloseCt		= min((m_nTableCt - nTableCtOpt), (int)arrFinishedTablesByOccoupiedSeatsCt.GetCount());
		int nReseatPlayersCt	= 0;

		for(int i=0; i<nTableCloseCt; i++){
			GameTableSeats* p1 = m_listTableSeats.GetAt((int)arrFinishedTablesByOccoupiedSeatsCt.GetData(i));
			ASSERT(p1);
			if( DeleteTableSeatsInfo(p1->nTableId, false, false) ){
				arrCloseTables.Add((void*)p1);
				nReseatPlayersCt += p1->nOccupiedSeatCt;
				}
			}
		// }}

		int							nMaxOccoupiedSeatCt		= nAvgPlayerPerTable + (m_nPlayersCt%m_nTableCt > 0 ? 1 : 0);
		// Array of game tables stat object pointers whose waited for players to start and now they are about to resume game.
		AutoSortedArray arrGameTablesResumeHand;
		for(UINT i=0; i<arrCloseTables.GetCount(); i++){
			GameTableSeats*				pTableSeatFrom		= (GameTableSeats*)arrCloseTables.Get(i);
			TournamentTableController*	pGameTableSeatFrom	= pTableSeatFrom->pGameTable;

			int	nLoop		= 0, nCt = pTableSeatFrom->nOccupiedSeatCt;
			int	nTableIndex	= 0, nTableIndexDest = 0, nSeatIndexDest = 0;
			
			while( nLoop < nCt ){
				int nSeatSrc = FindOccupiedSeat(pTableSeatFrom);
				ASSERT(nSeatSrc > -1);

				// Find empty seat on all tables. first looking for on tables with max occoupied seat less than nAvgPlayerPerTable after less than nAvgPlayerPerTable + 1. {{
				if( !FindEmptySeatOnAllTables(nTableIndex, nMaxOccoupiedSeatCt, nReseatPlayersCt, nTableIndexDest, nSeatIndexDest) ){
					ASSERT(FALSE);
					delete pTableSeatFrom;
					pTableSeatFrom = NULL;
					break;
					}
				nTableIndex										= nTableIndexDest;
				// }}

				GameTableSeats*				pTableSeatIn		= m_listTableSeats.GetAt(nTableIndexDest);
				ASSERT(pTableSeatIn);
				TournamentTableController*	pGameTableSeatIn	= pTableSeatIn->pGameTable;
	
				if( nSeatIndexDest > -1 && pGameTableSeatIn->SeatFromAnotherTable(nSeatIndexDest, pGameTableSeatFrom, nSeatSrc) ){
					pTableSeatIn->arrSeats[nSeatIndexDest]		= 1;
					pTableSeatIn->arrBalances[nSeatIndexDest]	= pTableSeatFrom->arrBalances[nSeatSrc];
					pTableSeatIn->nEmptySeatCt					--;
					pTableSeatIn->nOccupiedSeatCt				++;
					RecalcStakesStatistic(pTableSeatIn);

					pTableSeatFrom->arrSeats[nSeatSrc]			= 0;
					pTableSeatFrom->nEmptySeatCt				++;
					pTableSeatFrom->nOccupiedSeatCt				--;
					RecalcStakesStatistic(pTableSeatFrom);

					/*
					_String sLog;
					sLog.Format(_T("SeatFromAnotherTable tableIdFrom=%d, tableIdTo=%d, occoupiedNew=%d, seatInWaiting=%d, avg=%d isInWaitingList=%d -2, threadId=%d"), 
						pTableSeatFrom->nTableId, pTableSeatIn->nTableId, pTableSeatIn->nOccupiedSeatCt, (int)pTableSeatIn->bWaitingForPlayersToStart, nAvgPlayerPerTable, 
						m_arrTablesWaitingForPlayers.IndexOf((void*)pTableSeatIn->nTableId), GetCurrentThreadId());
					Logger::CreateLogEntry(sLog.GetBuffer());*/
					}

				nLoop ++;
				}

			if( pTableSeatFrom ){
				RemoveTableWaitingForPlayers(pTableSeatFrom->nTableId);
				pTableSeatFrom->bWaitingForPlayersToStart = false;
				pTableSeatFrom->pGameTable->GThread_Resume();
				/*
				_String sLog;
				sLog.Format(_T("Resume game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d-3, threadId=%d"), pTableSeatFrom->nTableId, 
					pTableSeatFrom->nEmptySeatCt, pTableSeatFrom->nOccupiedSeatCt, GetCurrentThreadId());
				Logger::CreateLogEntry(sLog.GetBuffer());
				*/
				delete pTableSeatFrom;
				pTableSeatFrom = NULL;
				}
			}
		}

	// Resume game table threads.
	for(UINT i = 0; i< m_arrTablesWaitingForPlayers.GetCount(); i++){
		GameTableSeats* pSeatResume = GetTableSeatsInfo((int)m_arrTablesWaitingForPlayers.Get(i));
		if( pSeatResume && pSeatResume->bWaitingForPlayersToStart && pSeatResume->nOccupiedSeatCt >= nAvgPlayerPerTable ){
			RemoveTableWaitingForPlayers(pSeatResume->nTableId);
			pSeatResume->bWaitingForPlayersToStart = false;
			/*
			_String sLog;
			sLog.Format(_T("Resume game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d-4, threadId=%d"), pSeatResume->nTableId, 
				pSeatResume->nEmptySeatCt, pSeatResume->nOccupiedSeatCt, GetCurrentThreadId());
			Logger::CreateLogEntry(sLog.GetBuffer());
			TRACE("Resume_ game thread tableId=%d, emptySeatCt=%d, OccupiedSeatCt=%d, threadId=%d", pSeatResume->nTableId, 
				pSeatResume->nEmptySeatCt, pSeatResume->nOccupiedSeatCt, GetCurrentThreadId());
				*/
			ASSERT(pSeatResume->pGameTable);
			pSeatResume->pGameTable->GThread_Resume();
			}
		}
	}

int	
TournamentTablesManager::FindSeatClosestToBalance(GameTableSeats* p, INT_CHIPS nBalanceCloseTo){
	ASSERT(p && p->nOccupiedSeatCt > 0);
	TournamentTableController*	pController		= p->pGameTable;
	int							nSeatClosest	= -1;
	INT_CHIPS					nClosestAmount	= MAXINT64;

	for(int i=0; i<m_nMaxSeatCt; i++){
		if( p->arrSeats[i] == 1 ){
			INT_CHIPS nBalance = p->arrBalances[i];
			if( nClosestAmount	> abs(nBalanceCloseTo - nBalance) ){
				nClosestAmount	= abs(nBalanceCloseTo - nBalance);
				nSeatClosest	= i;
				}
			}
		}
	return nSeatClosest;
	}

int
TournamentTablesManager::FindEmptySeat(GameTableSeats* pDest, GameTableSeats* pSrc, int nSeatSrc, bool& bSkipBB){
	ASSERT(pDest && pSrc && nSeatSrc > -1);

	if( pDest->nEmptySeatCt <= 0 ||
		pSrc->nOccupiedSeatCt <= 0 ||
		pSrc->arrSeats[nSeatSrc] == 0 ) 
		return -1;

	for(int i=0; i<m_nMaxSeatCt; i++){
		if( pDest->arrSeats[i] == 0 ){ // Empty
			return i;
			}
		}
	return -1;
	}

bool
TournamentTablesManager::FindEmptySeatOnAllTables(int nIndexStart, int nMaxOccupiedSeatCt, int nSeatCtWithoutTable, int& nTableIndex, int& nSeatIndex, bool bRandomTables /*= false*/){
	if( nMaxOccupiedSeatCt <= 0 )
		return false;

	// There's no empty seat.
	if( ((m_nPlayersCt - nSeatCtWithoutTable) / m_nTableCt) == m_nMaxSeatCt )
		return false;

	if( bRandomTables ){
		AutoSortedArray arrRandomTables; // <Random number><Table index>
		arrRandomTables.SetSize((UINT)m_nTableCt);

		srand(time(NULL));
		for(int i=nIndexStart; i<m_nTableCt; i++)
			arrRandomTables.Add((void*)rand(), (void*)i);

		for(UINT i=0; i<arrRandomTables.GetCount(); i++){
			int				nIndex = (int)arrRandomTables.GetData(i);
			GameTableSeats* pSeats = m_listTableSeats.GetAt(nIndex);
			ASSERT( pSeats );

			if( pSeats->nOccupiedSeatCt < nMaxOccupiedSeatCt ){
				nTableIndex = nIndex;
				nSeatIndex	= -1;
				for(int j=0; j<m_nMaxSeatCt; j++){
					if( pSeats->arrSeats[j] == 0 ){ // Empty
						nSeatIndex = j;
						break;
						}
					}
				return true;
				}
			}
		return false;
		}

	while( nIndexStart < m_nTableCt ){
		GameTableSeats* pSeats = m_listTableSeats.GetAt(nIndexStart);
		ASSERT( pSeats );

		if( pSeats->nOccupiedSeatCt < nMaxOccupiedSeatCt ){
			nTableIndex = nIndexStart;
			nSeatIndex	= -1;
			for(int i=0; i<m_nMaxSeatCt; i++){
				if( pSeats->arrSeats[i] == 0 ){ // Empty
					nSeatIndex = i;
					break;
					}
				}
			return true;
			}
		nIndexStart ++;
		}
	return false;
	}

int
TournamentTablesManager::FindOccupiedSeat(GameTableSeats* p){
	ASSERT(p);
	for(int i=0; i<m_nMaxSeatCt; i++){
		if( p->arrSeats[i] == 1 ) // Occoupied
			return i;
		}
	return -1;
	}

void
TournamentTablesManager::RecalcStakesStatistic(GameTableSeats* p){
	ASSERT(p);
	TournamentTableController*	pController		= p->pGameTable;
	INT_CHIPS					nSumStakes		= 0, nStakeAvg = 0, nStakeLowest = MAXINT64, nStakeHighest	= 0;
	int							nSeatCt			= 0;

	for(int i=0; i<m_nMaxSeatCt; i++){
		if( p->arrSeats[i] == 1 ){
			if( p->arrBalances[i] > 0 ){
				nStakeLowest	= min(nStakeLowest, p->arrBalances[i]);
				nStakeHighest	= max(nStakeHighest, p->arrBalances[i]);
				}
			nSumStakes	+= p->arrBalances[i];
			nSeatCt		++;
			}
		else
			p->arrBalances[i] = 0;
		}

	if( nStakeLowest == MAXINT64 )
		nStakeLowest = 0;

	// Update sum of chips.
	m_nSumChips -= (p->nSumChips/*old*/ - nSumStakes/*new*/);

	p->nBalanceHighest	= nStakeHighest;
	p->nBalanceLowest	= nStakeLowest;
	p->nAvgBalance		= ((nSeatCt <= 0) ? 0 : (nSumStakes / nSeatCt));
	p->nSumChips		= nSumStakes;

	bool bRecalcStakes = false;
	if( m_nHighestStakeTableId != p->nTableId ){
		if( m_nStakeHighest < nStakeHighest ){
			m_nHighestStakeTableId	= p->nTableId;
			m_nStakeHighest			= nStakeHighest;
			}
		}
	else
		bRecalcStakes = true;

	if( m_nLowestStakeTableId != p->nTableId ){
		if( m_nStakeLowest > nStakeLowest || (!m_nStakeLowest && nStakeLowest > 0) ){
			m_nLowestStakeTableId	= p->nTableId;
			m_nStakeLowest			= nStakeLowest;
			}
		}
	else
		bRecalcStakes = true;

	// Update avg stake count.
	INT_CHIPS nAvgStake		=  (m_nSumChips / m_nPlayersCt);
	m_nStakeAverage			= nAvgStake;

	if( bRecalcStakes ){
		int nHighestStakesTableId = 0, nLowestStakesTableId = 0, nOccupiedSeatsCt = 0;
		nStakeHighest	= 0;
		nStakeLowest	= MAXINT64;
		nSumStakes		= 0;

		for(int i=0; i<m_listTableSeats.GetCount(); i++){
			GameTableSeatsTag* pTableSeats = m_listTableSeats[i];
			if( pTableSeats->nOccupiedSeatCt > 0 ){
				if( pTableSeats->nBalanceHighest > nStakeHighest ){
					nStakeHighest			= pTableSeats->nBalanceHighest;
					nHighestStakesTableId	= pTableSeats->nTableId;
					}

				if( pTableSeats->nBalanceLowest < nStakeLowest ){
					nStakeLowest			= pTableSeats->nBalanceLowest;
					nLowestStakesTableId	= pTableSeats->nTableId;
					}
				nSumStakes			+= pTableSeats->nSumChips;
				nOccupiedSeatsCt	+= pTableSeats->nOccupiedSeatCt;
				}
			}

		if( nStakeLowest == MAXINT64 )
			nStakeLowest = 0;

//		int nAvgStake			=  (nSumStakes / nOccupiedSeatsCt);
		// Finally replace original values.
		m_nHighestStakeTableId	= nHighestStakesTableId;
		m_nLowestStakeTableId	= nLowestStakesTableId;
		m_nStakeHighest			= nStakeHighest;
		m_nStakeLowest			= nStakeLowest;
//		m_nStakeAverage			= nAvgStake;
		}
	}

GameTableSeats*
TournamentTablesManager::GetTableSeatsInfo(int nTableId){
	int nIndex = m_arrSeatStructByTableId.IndexOf((void*)nTableId);
	if( nIndex == -1 ) return NULL;
	GameTableSeats* pSeats = (GameTableSeats*)m_arrSeatStructByTableId.GetData(nIndex);
	return pSeats;
	}

bool
TournamentTablesManager::DeleteTableSeatsInfo(int nTableId, bool bDestroyObject, bool bReducePlayersCount){
	bool bRet = false;
	int nIndex = m_arrSeatStructByTableId.IndexOf((void*)nTableId);
	if( nIndex != -1 ){
		GameTableSeats* pSeats = (GameTableSeats*)m_arrSeatStructByTableId.GetData(nIndex);
		m_arrSeatStructByTableId.Delete(nIndex);
		if( bReducePlayersCount )
			m_nPlayersCt	-= (m_nMaxSeatCt - pSeats->nEmptySeatCt);
		m_nTableCt			--;
		m_listTableSeats.Remove(pSeats, bDestroyObject);
		bRet = true;
		}
	RemoveTableWaitingForPlayers(nTableId);
	return bRet;
	}

bool
TournamentTablesManager::AddTableWaitingForPlayers(int nTableId){
	if( m_arrTablesWaitingForPlayers.IndexOf((void*)nTableId) == -1 ){
		m_arrTablesWaitingForPlayers.Add((void*)nTableId);
		return true;
		}
	return false;
	}

bool
TournamentTablesManager::RemoveTableWaitingForPlayers(int nTableId){
	int nIndex = m_arrTablesWaitingForPlayers.IndexOf((void*)nTableId);
	if( nIndex != -1 ){
		m_arrTablesWaitingForPlayers.Delete(nIndex);
		return true;
		}
	return false;
	}

