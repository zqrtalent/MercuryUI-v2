#include "stdafx.h"
#include "PokerServer.h"
#include "../GameServer/ClientConnection.h"
#include "HoldemWinnerCalculator.h"
#include "TournamentTableController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LRESULT __stdcall
PokerCashTableController::GThread_Entry(WinThread* pGameThread){
	PokerCashTableController*	pGameController		= (PokerCashTableController*)pGameThread->GetThreadParameter();
	PokerServer*				pServer				= pGameController->m_pServer;
	BOOL						bStartAtOnce		= (BOOL)pGameThread->GetParam(_T("START_AT_ONCE"));		
	CEvent*						pSyncStart			= (CEvent*)pGameThread->GetParam(_T("SYNC_START_EVENT"));
	// Wait milliseconds after sync event become signaled.
	int*						pSyncStartWaitMS	= (int*)pGameThread->GetParam(_T("SYNC_START_WAIT_MS"));
	int							nHandStartDelayMS	= pGameController->m_nHandStartDelay*1000;
	if( !bStartAtOnce )
		pGameThread->PauseThread();

	while( true ){
top_label:
		// Wait while paused.
		if( pGameThread->IsPauseEventSignaled() ){
			if( pGameThread->IsStopEventSignaled() )
				break; // Stop thread.
			// Hand start delay.
			nHandStartDelayMS = pGameController->m_nHandStartDelay*1000;
			pGameThread->WaitForContinueEvent();
			}
		else{
			if( pGameThread->IsStopEventSignaled() )
				break; // Stop thread.
			}

		// Wait hand start delay seconds.
		if( !pGameController->GThread_Sleep(nHandStartDelayMS) )
			break; // Stop thread event signaled.
		nHandStartDelayMS = 0;

start_hand_label:
		// Start hand.
		if( !pGameController->GThread_StartHand() ){
			if( pGameThread->IsStopEventSignaled() )
				break; // Stop thread.

			bool bPauseThread = false;
			pGameController->OnGThread_Finishing(bPauseThread);

			// Turn into paused state.
			if( bPauseThread ){
				pGameThread->PauseThread();
				continue;
				}
			else
				break; // Finish thread.
			}

		bool bHandFinished = false, bTerminate = false;
		while( !bHandFinished ){
			// Wait round start delay seconds.
			if( !pGameController->GThread_Sleep(pGameController->m_nStartRoundDelay*1000) )
				break; // Stop thread event signaled.

			// Start round.
			bool	bRoundEnded = false, bOpenCards	= false;
			int		nSeatAsked	= -1, nPrevSeat	= -1;
			while( !bRoundEnded ){
				if( !pGameController->GThread_AskForAction(nPrevSeat, nSeatAsked, bRoundEnded, bOpenCards) ){
					// Turn into paused state.
					pGameThread->PauseThread();
					goto top_label;
					}
				nPrevSeat = nSeatAsked;
				}
			
			// Finish round.
			if( !pGameController->GThread_FinishRound(bHandFinished) ){
				// Turn into paused state.
				pGameThread->PauseThread();
				goto top_label;
				}
			}

		// Finish hand. {{
		int		nPotCt	= 0;
		bool	bRet	= pGameController->GThread_FinishHand(bTerminate, nPotCt);
		if( bTerminate ){
			bool bPauseThread = false;
			pGameController->OnGThread_Finishing(bPauseThread);
			// Turn into paused state.
			if( bPauseThread ){
				pGameThread->PauseThread();
				continue;
				}
			else
				break; // Finish thread.
			}
		else
		if( !bRet ){
			// Turn into paused state.
			pGameThread->PauseThread();
			goto top_label;
			}
		// }}

		// Wait for sync start event while not signaled.
		if( pSyncStart ){
			if( ::WaitForSingleObject(*pSyncStart, 0) != WAIT_OBJECT_0 ){
				// Clear seat cards while waiting for hand start.
				pGameController->ClearSeatCards();

				// Wait for sync start signal.
				pGameController->m_bSyncState = true;
				::WaitForSingleObject(*pSyncStart, INFINITE);
				pGameController->m_bSyncState = false;

				// Check for stop event.
				if( pGameThread->IsStopEventSignaled() )
					break; // Stop thread.

				if( pSyncStartWaitMS && *pSyncStartWaitMS > 0 ){
					if( !pGameController->GThread_Sleep(*pSyncStartWaitMS) )
						break; // Stop thread event signaled.
					// Check for stop event.
					if( pGameThread->IsStopEventSignaled() )
						break; // Stop thread.
					}

				// Start hand immediately.
				goto start_hand_label;
				}
			}
		}

	if( pGameController )
		pGameController->OnGThread_Terminated();
	return 0L;
	}

void
PokerCashTableController::GThread_Start(bool bSuspended /*= false*/){
	if( !m_gameThread.IsThreadStopped() )
		return;
	// Game thread starting event.
	OnGThread_Starting();
	// Start at once or stat paused.
	m_gameThread.AddParam(_T("START_AT_ONCE"), (void*)(bSuspended ? FALSE : TRUE));
	m_gameThread.StartThread((LPTHREAD_START_ROUTINE)&PokerCashTableController::GThread_Entry, this);
	}

void
PokerCashTableController::GThread_Stop(){
	m_gameThread.StopThread();
	}

bool
PokerCashTableController::GThread_StartHand(){
	if( !m_gameThread.IsCurrentThread() )
		return false;

	CDWordArray	arrSitOutSeats, arrEmptySeats, arrPayBBSeats;
	if( m_info.m_gameStatus != PokerGameStatus::NotStarted )
		return false;

	// Hand starting.
	HandStarting();

//	AutoSortedArray	arrSeatsPostBBAsked;

try_again:
	m_lockObjects.Lock(); // Lock
	// Empty offline seats.
	EmptyOfflineSeats();
	// False pre-actions.
	m_info.FalsePreActions();
	// Clear pot amounts.
	m_info.ClearPotAmounts();

	if( m_info.GetSittedCt() < 2 ){ // Insafficient count of ready seats.
		m_info.SetSeatStatus(PokerSeatStatus::PayBBAndPlay,				PokerSeatStatus::Sitted);
		m_info.SetSeatStatus(PokerSeatStatus::WaitingForBB,				PokerSeatStatus::Sitted);
		m_info.SetSeatStatus(PokerSeatStatus::CheckForPositionToStart,	PokerSeatStatus::Sitted);

		m_info.m_cSBSeat		= -1;
		m_info.m_cBBSeat		= -1;
		m_info.m_cActiveSeat	= -1;
		m_info.m_cDealerSeat	= -1;

		if( m_info.GetSittedCt() < 2 ){
			m_info.ClearStatInfo();
			m_info.m_gameStatus	= PokerGameStatus::NotStarted; // Change state.	
			m_lockObjects.Unlock(); // Unlock
			return false;
			}
		}
	
	m_info.m_gameStatus					= PokerGameStatus::Starting; // Change to starting state.	
	int					nBBSeat			= -1, nSBSeat = -1, nSBSeatRefused = -1, nDealer = -1, nDealerPrev	= m_info.m_cDealerSeat;;
	PokerTableSeatInfo*	pDSeat			= m_info.GetDealerSeat(&nDealer);
	bool				bSBSeat			= false, bBBSeat = false, bElapsed = false;
	PokerTableSeatInfo* pSBSeatInfo		= m_info.GetSBSeat(nDealer, &nSBSeat);
	BasePacket*			pPacketReceived	= NULL;
	// Pay ante amount. // Moved to method DealCardsAndStartHand
	//m_info.PayAnte();
	m_lockObjects.Unlock(); // Unlock

	BasePacket*			pPacketSent		= NULL;
	PokerActionPacket*	pPacketRecv		= new PokerActionPacket();
	bool				bTimeElapsed	= false, bSitOutNextBB	= false;
	int					nSecWasted		= 0;
	ClientConnection*	pConnSend		= NULL;			
	
	// Find out SB seat.
	if( nSBSeat != -1 ){
		m_lockObjects.Lock(); // Lock
		pConnSend = pSBSeatInfo->GetConnection();
		m_lockObjects.Unlock(); // Unlock

		bSBSeat = false;
		if( !GThread_AskToPostBlind(nSBSeat, true, pConnSend, pPacketRecv, bSBSeat) ){
			delete pPacketRecv;
			return false; 
			}
		
		if( !bSBSeat )
			nSBSeatRefused = nSBSeat;
		}
	else
		nSBSeatRefused = (m_info.m_cBBSeat == -1) ? -1 : m_info.m_cBBSeat;
	// }}

	// Find out BB seat. {{
	CDWordArray arrBBAskSeats;
	m_info.GetBBAskSeats(nSBSeat, nDealer, &arrBBAskSeats);
	for(int i=0; i<arrBBAskSeats.GetCount(); i++){
		int				nSeat			= arrBBAskSeats.GetAt(i);
		m_lockObjects.Lock();	// Lock
		PokerTableSeatInfo* pBBSeatInfo	=  m_info.GetSeatInfo(nSeat);
		if( !pBBSeatInfo || 
			(pBBSeatInfo->GetStatus() != PokerSeatStatus::Sitted &&
			pBBSeatInfo->GetStatus() != PokerSeatStatus::WaitingForBB &&
			pBBSeatInfo->GetStatus() != PokerSeatStatus::PayBBAndPlay &&
			pBBSeatInfo->GetStatus() != PokerSeatStatus::CheckForPositionToStart &&
			pBBSeatInfo->GetStatus() != PokerSeatStatus::All_In &&
			(m_info.m_bTournamentTable && pBBSeatInfo->GetStatus() != PokerSeatStatus::SittedOut))
			){
			if( !m_info.m_bTournamentTable && pBBSeatInfo->GetStatus() == PokerSeatStatus::SittedOut )
				pBBSeatInfo->SetMissedBB(true);

			m_lockObjects.Unlock();	// Unlock
			continue;
			}

		pConnSend		= pBBSeatInfo->GetConnection();
		bSitOutNextBB	= pBBSeatInfo->GetSitOutNextBB();
		m_lockObjects.Unlock();		// Unlock

		if( bSitOutNextBB ){
			// Sit out seat.
			SitOutBySeat(nSeat);
			continue;
			}

		bBBSeat = false;
		if( !GThread_AskToPostBlind(nSeat, false, pConnSend, pPacketRecv, bBBSeat) ){
			delete pPacketRecv;
			return false; 
			}

		if( bBBSeat ){
			nBBSeat = nSeat;
			break;
			}
		}
	// }}

	if( !bBBSeat && bSBSeat ){
		// Check if dealer exists and ask to pay BB ?
		if( nDealer != nSBSeat ){
			m_lockObjects.Lock();	// Lock
			PokerTableSeatInfo* pSeatInfoBB = m_info.GetSeatInfo(nDealer);
			pConnSend	= (pSeatInfoBB ? pSeatInfoBB->GetConnection() : NULL);
			m_lockObjects.Unlock(); // Unlock

			bBBSeat		= false;
			if( !GThread_AskToPostBlind(nDealer, false, pConnSend, pPacketRecv, bBBSeat) ){
				delete pPacketRecv;
				return false; 
				}

			if( bBBSeat ){
				nDealer	= nSBSeat;
				nBBSeat = nDealer;
				}
			}
		}

	/*
	// Find out BB seat.
	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo* pBBSeatInfo	=  m_info.GetBBSeat(nSBSeat, nDealer, &nBBSeat);
	pConnSend						= pBBSeatInfo ? pBBSeatInfo->GetConnection() : NULL;
	bSitOutNextBB					= pBBSeatInfo ? pBBSeatInfo->GetSitOutNextBB() : false;
	m_lockObjects.Unlock();	// Unlock

	while( nBBSeat != -1 ){
		if( bSitOutNextBB ){
			// Sit out seat.
			SitOutBySeat(nBBSeat);

			if( !m_info.m_bTournamentTable ){
				m_lockObjects.Lock(); // Lock
				pBBSeatInfo		= m_info.GetBBSeat(nSBSeat, nDealer, &nBBSeat);
				if( pBBSeatInfo ){
					pConnSend		= pBBSeatInfo->GetConnection();
					bSitOutNextBB	= pBBSeatInfo->GetSitOutNextBB();
					}
				m_lockObjects.Unlock(); // Unlock
				continue;
				}
			}

		// Check if asked for post BB once!
		if( arrSeatsPostBBAsked.IndexOf((void*)nBBSeat) == -1 )
			arrSeatsPostBBAsked.Add((void*)nBBSeat);
		else
			break;

		bBBSeat = false;
		if( !GThread_AskToPostBlind(nBBSeat, false, pConnSend, pPacketRecv, bBBSeat) ){
			delete pPacketRecv;
			return false; 
			}
		
		m_lockObjects.Lock(); // Lock
		pBBSeatInfo		= m_info.GetBBSeat(nSBSeat, nDealer, &nBBSeat);
		if( pBBSeatInfo ){
			pConnSend		= pBBSeatInfo->GetConnection();
			bSitOutNextBB	= pBBSeatInfo->GetSitOutNextBB();
			}
		m_lockObjects.Unlock(); // Unlock
		}
	// }}
	*/

	// Deal cards and start hand. {{
	int nDealerPos	= nDealer;
	int nSBPos		= (bSBSeat ? nSBSeat : -1);
	int nBBPos		= (bBBSeat ? nBBSeat : -1);
	/*
	m_lockObjects.Lock();	// Lock
	int nSittedCt	= m_info.GetSittedCt(), nSeatCtMissingHand = 0;
	CPtrArray arrSeatsMissingHand;
	// Count seats between dealer and smallblind seats.
	if( nSBPos != -1 && nSBPos != nBBPos ){
		nSeatCtMissingHand = m_info.GetSeatBetweenSeatsByStatusClockwise(&arrSeatsMissingHand, nDealerPos, nSBPos, PokerSeatStatus::Sitted);
		if( m_info.m_bTournamentTable )
			nSeatCtMissingHand += m_info.GetSeatBetweenSeatsByStatusClockwise(&arrSeatsMissingHand, nDealerPos, nSBPos, PokerSeatStatus::SittedOut);
		}
	m_lockObjects.Unlock(); // Unlock

	// Remain dealer position same as in previous hand.
	if( nDealerPos == nSBPos && bBBSeat && (nSittedCt - nSeatCtMissingHand) > 2 && nDealerPrev != -1 ){
		nDealerPos = nDealerPrev;
		}
	else{
		// Everyone has refused to post bb.
		if( (nSittedCt - nSeatCtMissingHand) == 2 && !bBBSeat && bSBSeat ){
			nDealerPos	= nSBPos;
			m_lockObjects.Lock();	// Lock
			PokerTableSeatInfo* pSeatInfoBB = m_info.GetNextSeatInfoByStatus(nDealerPos, PokerSeatStatus::Sitted, &nBBPos);
			pConnSend	= (pSeatInfoBB ? pSeatInfoBB->GetConnection() : NULL);
			m_lockObjects.Unlock(); // Unlock

			bBBSeat		= false;
			if( !GThread_AskToPostBlind(nBBPos, false, pConnSend, pPacketRecv, bBBSeat) ){
				delete pPacketRecv;
				return false; 
				}

			// Check if asked for post BB once!
			if( arrSeatsPostBBAsked.IndexOf((void*)nBBPos) == -1 )
				arrSeatsPostBBAsked.Add((void*)nBBPos);

			if( !bBBSeat )
				nBBPos = -1;
			}
		}*/

	int nTournamentPlayersCt = 0, nTournamentTablesCt = 0;
	if(  m_info.m_bTournamentTable ){
		nTournamentPlayersCt	= ((TournamentTableController*)this)->GetTournamentPlayersCount();
		nTournamentTablesCt		= ((TournamentTableController*)this)->GetTournamentTablesCount();
		}

	// Deal with CheckForPositionToStart status seats.
	m_lockObjects.Lock(); // Lock
	CPtrArray	arrSeatsToCheckPosition;
	if( bBBSeat && nSBPos != nDealerPos ){ // Not in heads-up.
		m_info.GetSeatBetweenSeatsByStatusClockwise(&arrSeatsToCheckPosition, nBBPos, nDealerPos, PokerSeatStatus::CheckForPositionToStart);
		for(int i=0; i<arrSeatsToCheckPosition.GetCount(); i++){
			PokerTableSeatInfo* pSeatCheck = (PokerTableSeatInfo*)arrSeatsToCheckPosition.GetAt(i);
			if( pSeatCheck ){
				int		nSeat		= pSeatCheck->GetSeat();
				bool	bMissedSB	= pSeatCheck->HasMissedSB(), bMissedBB = pSeatCheck->HasMissedBB();
				if( bMissedBB || bMissedSB ){
					pConnSend		= pSeatCheck->GetConnection();
					m_lockObjects.Unlock(); // Unock

					bool bSuccess = false;
					if( !GThread_AskToPostBlind(nSeat, false, pConnSend, pPacketRecv, bSuccess, true) ){
						delete pPacketRecv;
						return false; 
						}

					m_lockObjects.Lock(); // Lock
					}
				else
					m_info.SetSeatStatus(nSeat, PokerSeatStatus::Sitted);
				}
			}
		m_info.GetPayBBAndPlaySeats	(nBBPos, nDealerPos, arrPayBBSeats);
		}

	bool bDealerRemainTheSame = (m_info.m_cSBSeat == m_info.m_cDealerSeat && m_info.m_cSBSeat != -1) && (m_info.m_cDealerSeat !=  m_info.m_cBBSeat && m_info.m_cBBSeat != -1);
	m_lockObjects.Unlock(); // Unlock

	bool bRet = false;
	if( nSBPos != -1 || nBBPos != -1){
		if( bDealerRemainTheSame )
			nDealerPos = nDealerPrev;
		bRet = DealCardsAndStartHand(nDealerPos, nSBPos, nBBPos, nSBSeatRefused, arrPayBBSeats, nTournamentPlayersCt, nTournamentTablesCt);
		}

	if( bRet ){
		HandStarted();
		}
	else{
		/*
		if( nBBPos == -1 ){
			m_lockObjects.Lock();	// Lock
			nSittedCt = m_info.GetSittedCt();
			if( nSittedCt > 1 ){
				int nTemp	= nSBPos;
				nSBPos		= nDealerPos;
				nBBPos		= nTemp;
				}
			m_lockObjects.Unlock();	// Unlock
			}*/

		m_lockObjects.Lock();	// Lock
		m_info.m_cSBSeat		= -1;
		m_info.m_cBBSeat		= -1;
		m_info.m_cActiveSeat	= -1;
		m_info.m_cDealerSeat	= -1;
		m_lockObjects.Unlock();	// Unlock

		delete pPacketRecv;
		pPacketRecv = NULL;
		goto try_again;
		}
	// }}

	delete pPacketRecv;
	return bRet;
	}

bool
PokerCashTableController::GThread_FinishHand(bool& bTerminate, int& nPotCt){
	m_lockObjects.Lock();	// Lock
	PokerGameStatus status = m_info.m_gameStatus;
	m_lockObjects.Unlock(); // Unlock
	if( status != PokerGameStatus::Started )
		return false;

	m_lockObjects.Lock();	// Lock
	List<PokerHandWinnerInfo> winners;
	EnumerableObject<PokerSeatAmountInfo> seatAmounts;
	m_info.GetSeatAmounts(&seatAmounts, true);

	HandWinnerCalculator*	pCalc				= m_info.GetHandWinnerCalculator();
	UINT					nRakeAmount			= 0;
	float					fRakePercent		= m_info.m_fRakePercent;
	INT_CHIPS				nTotalPotAmount		= 0;
	int						nHandStartMS		= m_nHandStartDelay * 1000;
	bool					bOpenWinnersCards	= false, bTableCardsIsTheBestComb = false;
	_String					sPlayersInfo, sTableCards;
	int						nArrHandCards[]		= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	
	bool					bRet				= pCalc ? pCalc->CalculateWinners(&m_info, &winners, fRakePercent, nRakeAmount, nTotalPotAmount, bOpenWinnersCards, bTableCardsIsTheBestComb, nArrHandCards) : false; 
	int						nTableId			= m_info.m_nTableId;	
	int						nTournamentId		= m_info.m_nTournamentId;	
	CDWordArray				arrAskMuckShowCards;
	AutoSortedArray			arrHandCardsInfoBySeat, arrWinnerSeats;
	PokerHandFinishedPacket* p					= NULL;

	// Prepare 'handfinished' packet.
	if( bRet ){
		// Build players info string.
		int			nLoop			= 0, nCt = (int)m_info.m_cMaxPlayersCt;
		INT_CHIPS	nSeatBalanceNew = 0, nSeatBalanceOld = 0;
		INT_CHIPS	nWonAmount		= 0, nBalance	= 0, nBetAmount	= 0;
		_String sTemp;
		// Fill pot count.
		nPotCt					= m_info.m_listPots.GetCount();

		// Table cards.
		sTableCards.Format(_T("%s%s%s%s%s"), 
			PokerHandCards::GetCardNameByIndex(m_info.GetTableCard1()).GetBuffer(),
			PokerHandCards::GetCardNameByIndex(m_info.GetTableCard2()).GetBuffer(),
			PokerHandCards::GetCardNameByIndex(m_info.GetTableCard3()).GetBuffer(),
			PokerHandCards::GetCardNameByIndex(m_info.GetTableCard4()).GetBuffer(),
			PokerHandCards::GetCardNameByIndex(m_info.GetTableCard5()).GetBuffer());

		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nLoop);
			if( !pSeatInfo || (pSeatInfo->GetStatus() != PokerSeatStatus::All_In && pSeatInfo->GetStatus() != PokerSeatStatus::InPlay) ){
				// Call seat balance change callback.
				HandPlayersInfoStruct* pHandPLayerInfo = m_info.GetHandPlayerInfo(nLoop);
				if( pHandPLayerInfo && pHandPLayerInfo->status == PokerSeatStatus::Folded /*pSeatInfo && pSeatInfo->GetStatus() == PokerSeatStatus::Folded*/ ){
					/*
					nBalance		= pSeatInfo->GetBalance();
					nBetAmount		= pSeatInfo->GetTotalPot();
					nSeatBalanceNew = nBalance;
					nSeatBalanceOld = nBalance + nBetAmount;

					if( pSeatInfo )
						m_pGameBase->PlayerSeatBalanceChanged((PokerSession*)pSeatInfo->GetSession(false), nTableId, m_nCurrencyId, pSeatInfo->GetSeat(),  nSeatBalanceNew, nSeatBalanceOld);

					// Folded seat info.
					sTemp.Format(_T("%d,%d.%02d,%d.%02d,%d.%02d,%d,%s%s%s%s,"), pSeatInfo->GetUserId(), 
						nBalance/100, nBalance%100, nBetAmount/100, nBetAmount%100, 0, 0, 0, 
						HandWinnerCalculator::GetCardNameByIndex(pSeatInfo->GetCard1()).GetBuffer(),
						HandWinnerCalculator::GetCardNameByIndex(pSeatInfo->GetCard2()).GetBuffer(),
						HandWinnerCalculator::GetCardNameByIndex(pSeatInfo->GetCard3()).GetBuffer(),
						HandWinnerCalculator::GetCardNameByIndex(pSeatInfo->GetCard4()).GetBuffer()
						);*/
					nBalance		= pHandPLayerInfo->nBalanceAmount;
					nBetAmount		= pHandPLayerInfo->nTotalPotAmount;
					nSeatBalanceNew = nBalance;
					nSeatBalanceOld = nBalance + nBetAmount;

					if( pSeatInfo )
						m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, (PokerSession*)pSeatInfo->GetSession(false), nTableId, m_nCurrencyId, pSeatInfo->GetSeat(),  nSeatBalanceNew, nSeatBalanceOld);

					// Folded seat info.
					sTemp.Format(_T("%d,%d.%02d,%d.%02d,%d.%02d,%d,%s%s%s%s,"), pHandPLayerInfo->nUserId, 
						(UINT)(nBalance/100), (UINT)(nBalance%100), (UINT)(nBetAmount/100), (UINT)(nBetAmount%100), 0, 0, 0, 
						PokerHandCards::GetCardNameByIndex((int)pHandPLayerInfo->btCard1).GetBuffer(),
						PokerHandCards::GetCardNameByIndex((int)pHandPLayerInfo->btCard2).GetBuffer(),
						PokerHandCards::GetCardNameByIndex((int)pHandPLayerInfo->btCard3).GetBuffer(),
						PokerHandCards::GetCardNameByIndex((int)pHandPLayerInfo->btCard4).GetBuffer()
						);

					sPlayersInfo += sTemp;
					}

				nLoop ++;
				continue;
				}

			// Find win amount by seat index (Should be improved in future :- Zack ). {{
			int nLoop1		= 0;
			int nCt1		= winners.GetCount();
			nWonAmount		= 0;
			nBalance		= pSeatInfo->GetBalance();
			nBetAmount		= pSeatInfo->GetTotalPot();

			while( nLoop1 < nCt1 ){
				if( winners[nLoop1] && winners[nLoop1]->m_bWinner && (int)(winners[nLoop1]->m_cSeat) == pSeatInfo->GetSeat() ){
					nWonAmount = winners[nLoop1]->m_nWonAmount;
					break;
					}
				nLoop1 ++;
				}
			// }}

			// Call seat balance change callback.
			PokerSeatAmountInfo* pSeatAmountInfo = seatAmounts[nLoop];
			nSeatBalanceNew = pSeatInfo->GetBalance(); // Seat already awarded.
			// Old (lead to mistake of in play balance amount)
			//nSeatBalanceOld = pSeatAmountInfo ? (pSeatAmountInfo->m_nBalance + pSeatAmountInfo->m_nPotAmount) : 0;
			nSeatBalanceOld = pSeatAmountInfo ? (pSeatAmountInfo->m_nBalance + pSeatAmountInfo->m_nTotalPotAmount) : 0;
			m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, (PokerSession*)pSeatInfo->GetSession(false), nTableId, m_nCurrencyId, pSeatInfo->GetSeat(),  nSeatBalanceNew, nSeatBalanceOld);
			// (UserId,Balance,BetAmount,WonAmount),,,,
			if( nBalance < 0 )	{nBalance = 0;}
			if( nWonAmount < 0 ){nWonAmount = 0;}
			sTemp.Format(_T("%d,%d.%02d,%d.%02d,%d.%02d,%d,%s%s%s%s,"), pSeatInfo->GetUserId(), 
				(UINT)(nBalance/100), (UINT)(nBalance%100), (UINT)(nBetAmount/100), (UINT)(nBetAmount%100), (UINT)(nWonAmount/100), (UINT)(nWonAmount%100), 
				nArrHandCards[pSeatInfo->GetSeat()], 
				PokerHandCards::GetCardNameByIndex(pSeatInfo->GetCard1()).GetBuffer(),
				PokerHandCards::GetCardNameByIndex(pSeatInfo->GetCard2()).GetBuffer(),
				PokerHandCards::GetCardNameByIndex(pSeatInfo->GetCard3()).GetBuffer(),
				PokerHandCards::GetCardNameByIndex(pSeatInfo->GetCard4()).GetBuffer()
				);
			sPlayersInfo += sTemp;
			nLoop ++;
			}
		// ==

		p						= new PokerHandFinishedPacket();
		p->m_dtAction			= COleDateTime::GetCurrentTime();
		p->m_nTableId			= nTableId;
		p->m_nTournamentId		= nTournamentId;
		p->m_nHandId			= m_nHandId;
		p->m_cHandStartSec		= (char)m_nHandStartDelay;
		p->m_wRakeAmount		= (WORD)nRakeAmount;
		p->m_bTableCardsBestComb= bTableCardsIsTheBestComb;

		// Update rake amount.
		m_info.SetRakeAmount(nRakeAmount);
		// Retrieve detailed pots info.
		m_info.GetDetailedPots(&p->m_listPotAmounts);

		// Build winners array.
		for(int i=0; i<winners.GetCount(); i++){
			PokerHandWinnerInfo* pWinnerInfo = winners.GetAt(i);
			if( pWinnerInfo ){
				arrWinnerSeats.Add((void*)pWinnerInfo->m_cSeat);
				p->m_winners.Add(pWinnerInfo);
				}
			}

		if( bOpenWinnersCards )
			m_info.GetSeatsHandCards(&p->m_handCards, &arrAskMuckShowCards, &arrWinnerSeats);
		else{
			if( winners.GetCount() == 1 ){
				PokerHandWinnerInfo* pWinnerInfo = winners.GetAt(0);
				if( pWinnerInfo ){
					PokerHandCards* pHand = new PokerHandCards();
					m_info.GetSeatHandCards((int)pWinnerInfo->m_cSeat, pHand);
					p->m_handCards.Add(pHand);
					arrAskMuckShowCards.Add((int)pWinnerInfo->m_cSeat);
					}
				}
			}

		// Build hand cards array.
		for(int i=0; i<p->m_handCards.GetCount(); i++){
			PokerHandCards* pHand = p->m_handCards.GetAt(i);
			if( pHand ){
				int nSeat = (int)pHand->m_btSeat;
				arrHandCardsInfoBySeat.Add((void*)nSeat, pHand);
				}
			}
		p->m_handCards.RemoveAll(false);

		// Add only winners hand cards to open at first.
		if( bOpenWinnersCards ){
			for(int i=0; i<winners.GetCount(); i++){
				PokerHandWinnerInfo* pWinnerInfo = winners.GetAt(i);
				if( pWinnerInfo){
					int nIndex = arrHandCardsInfoBySeat.IndexOf((void*)pWinnerInfo->m_cSeat);
					if( nIndex != -1 && arrHandCardsInfoBySeat.GetData(nIndex) ){
						PokerHandCards* pHand = (PokerHandCards*)arrHandCardsInfoBySeat.GetData(nIndex);
						p->m_handCards.Add(pHand);
						// Add card show action in hand history.
						m_handHistory.ActionWithHandCard(pWinnerInfo->m_cSeat, PokerActionTypes::ShowCards, 0, pWinnerInfo->m_nBalance, 0, 0, 0, pHand);
						}
					}
				}
			}
		}

	// Unlock
	m_lockObjects.Unlock();
	// Finish hand.
	if( bOpenWinnersCards && p ){
		// Hand history entries.
		m_handHistory.FinishHand(&winners, &m_info, p->m_cHandStartSec);
		// Copy hand history.
		m_handHistory.Copy(&p->m_handHistory);

		p->m_bShowdown = true;
		m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, p);
		p->m_handCards.RemoveAll(false);
		p->m_winners.RemoveAll(false); // winners destroy automatically.
		delete p;
		p = NULL;
		}
	
	// Muck/Show Cards packet. {{
	for(int i=0; i<arrAskMuckShowCards.GetCount(); i++){
		int							nSeatIndex		= (int)arrAskMuckShowCards.GetAt(i);
		int							nWinnerIndex	= arrWinnerSeats.IndexOf((void*)nSeatIndex);
		int							nHandCardsIdx	= arrHandCardsInfoBySeat.IndexOf((void*)nSeatIndex);
		PokerHandCards*				pHandCards		= nHandCardsIdx != -1 ? (PokerHandCards*)arrHandCardsInfoBySeat.GetData(nHandCardsIdx) : NULL;

		// Lock
		m_lockObjects.Lock();
		// Get seat status
		PokerSeatStatus status = m_info.GetSeatStatus(nSeatIndex);
		// Unlock
		m_lockObjects.Unlock();

		if( nWinnerIndex != -1 && bOpenWinnersCards ){
			// Send seat action.
			SendSeatActionPacket(nSeatIndex, PokerActionTypes::ShowCards, status, 0);
			continue;
			}

		int							nSecWasted		= 0;
		bool						bResult			= false, bTimeElapsed = false;
		BasePacket*					pPacketSent		= NULL;
		PokerShowMuckCardsPacket	packetRecv;
		ClientConnection*			pConnSend		= NULL;
		
		if( SendShowMuckCardsRequest(nSeatIndex, bResult) && !bResult ){
			pPacketSent = (BasePacket*)m_gameThread.GetParam(_T("PACKET"));
			pConnSend	= (ClientConnection*)m_gameThread.GetParam(_T("CONN"));

			if( !GThread_WaitForResponse(pConnSend, pPacketSent, m_nShowMuckDelay, &packetRecv, bTimeElapsed, NULL)  ){
				// Delete sent packet object.
				if( pPacketSent )
					delete pPacketSent;
				return false;
				}
				
			// Delete sent packet object.
			if( pPacketSent )
				delete pPacketSent;

			if( bTimeElapsed )
				SendSeatActionPacket(nSeatIndex, PokerActionTypes::Muck, status, 0);
			else{
				if( p && packetRecv.m_bShowCards )
					p->m_handCards.Add(pHandCards);
				SendSeatActionPacket(nSeatIndex, packetRecv.m_bShowCards ? PokerActionTypes::ShowCards : PokerActionTypes::Muck, status, 0);
				}
			}
		}
	// }}

	// Finish hand.
	if( !bOpenWinnersCards && p ){
		// Hand history entries.
		m_handHistory.FinishHand(&winners, &m_info, p->m_cHandStartSec);
		// Copy hand history.
		m_handHistory.Copy(&p->m_handHistory);

		p->m_bShowdown = false;
		m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, p);
		p->m_handCards.RemoveAll(false);
		p->m_winners.RemoveAll(false); // winners destroy automatically.
		delete p;
		}

	// Destroy hand cards. {{
	for(UINT i=0; i<arrHandCardsInfoBySeat.GetCount(); i++){
		PokerHandCards* pHand = (PokerHandCards*)arrHandCardsInfoBySeat.GetData(i);
		if( pHand ) delete pHand;
		}
	// }}

	// Finish hand.
	int nAvgHandDuration = 0;
	m_lockObjects.Lock();	// Lock
	m_info.FinishHand(bOpenWinnersCards);
	nAvgHandDuration = m_info.m_nAvgHandDurationSec;
	// Clear seat cards.
	m_info.ClearSeatCards();
	m_lockObjects.Unlock();	// Unlock
	
	// Change hand start delay by pot count.
	// It takes time to animate pot motion.
	if( nPotCt > 1 ){
		const int nSecondsPerPot	= 1;
		nHandStartMS				+= ((nPotCt - 1)*nSecondsPerPot)*1000;
		}

	// Finish hand animation delay.
	GThread_Sleep(nHandStartMS);

	CDWordArray		arrSitOutSeats, arrEmptyBalanceSeats;
	AutoSortedArray arrSeated;

	m_lockObjects.Lock();	// Lock
	// Retrieve sit out seats.
	m_info.GetSitOutNextHandSeats(arrSitOutSeats, true);
	// Zero balance seats.
	m_info.GetEmptyBalanceSeats(arrEmptyBalanceSeats, true);
	//old one
	//m_info.GetEmptyBalanceSeats(arrEmptyBalanceSeats);
	m_lockObjects.Unlock();	// Lock

	// Perform seats auto rebuy.
	if( nTournamentId > 0 )
		((TournamentTableController*)this)->PerformAutoRebuySeats();

	// Let users to make buyin / rebuy.
	int nEmptySeatCt = 0, nSecWaitToPerformBuyinRebuy = 5;
	if( arrEmptyBalanceSeats.GetCount() > 0 ){
		// Tournament table check for rebuy permission.
		if( nTournamentId <= 0 || ((TournamentTableController*)this)->IsRebuyAllowed() ){
			// Send wait for players to make rebuy. {{
			PokerWaitForPacket p;
			p.m_nTournamentId		= nTournamentId;
			p.m_nTableId			= nTableId;
			p.m_cWaitFor			= PokerTableWaitingState::WaitingForPlayersToMakeRebuy;
			p.m_shWaitDurationSec	= (short)nSecWaitToPerformBuyinRebuy;
			m_pServer->SendPacketByTable(nTableId, nTournamentId, &p, false);
			// }}

			// Perform buy/rebuy chips action.
			if( PerformAddChipsAction() > 0 ){
				if( !GThread_Sleep(1000) ) // Make add chips action be visible for any one.
					return true; // Thread stopped!!!
				}

			// Empty seats.
			arrEmptyBalanceSeats.RemoveAll();
			m_lockObjects.Lock();	// Lock
			m_info.GetEmptyBalanceSeats(arrEmptyBalanceSeats);
			m_lockObjects.Unlock();	// Unlock

			// Show add chips window.
			if( arrEmptyBalanceSeats.GetCount() > 0 && nTournamentId <= 0 ){
				PokerAddChipsPacket p;
				p.ZeroPacketVariables();
				p.m_nTableId		= m_info.m_nTableId;
				p.m_bPrepareToAdd	= true;				

				for(int i=0; i<arrEmptyBalanceSeats.GetCount(); i++){
					int nSeat = (int)arrEmptyBalanceSeats[i];
					m_lockObjects.Lock();	// Lock
					PokerTableSeatInfo* pSeatInfo	= m_info.GetSeatInfo(nSeat);
					m_lockObjects.Unlock();	// Unlock

					if( !pSeatInfo ) continue;
					PokerSession*		pSeatSess	= (PokerSession*)pSeatInfo->GetSession(true);
					ClientConnection*	pConnSeat	= pSeatInfo->GetConnection();
					if( !pSeatSess ) continue;

					p.m_cSeat			= (char)nSeat;
					p.m_nBalance		= 0;
					p.m_nMin			= 0;
					p.m_nMax			= 0;
					p.m_nSeatSessionId	= 0;
					p.m_nSessionId		= pSeatSess->GetSessionId();
					OnAddChips(pSeatSess, pConnSeat, &p);

					SafePointer::Release(pSeatSess); // Release
					}
				}

			int nLoop = 0;
			while( nLoop < nSecWaitToPerformBuyinRebuy ){
				if( !GThread_Sleep(1000) )
					return true; // Thread stopped!!!

				// Empty seats.
				arrEmptyBalanceSeats.RemoveAll();
				m_lockObjects.Lock();	// Lock
				m_info.GetEmptyBalanceSeats(arrEmptyBalanceSeats);
				m_lockObjects.Unlock();	// Unlock

				if( !arrEmptyBalanceSeats.GetCount() )
					break; // No more empty balance seats.

				nLoop ++;
				}

			/* Wasting time when everyone adds chips in less time then time we waiting for them.
			if( !GThread_Sleep(nSecWaitToPerformBuyinRebuy*1000) )
				return true; // Thread stopped!!!
			*/

			// Perform buy/rebuy chips action.
			if( PerformAddChipsAction() > 0 ){
				if( !GThread_Sleep(1000) ) // Make add chips action be visible for any one.
					return true; // Thread stopped!!!
				}

			// Empty seats.
			arrEmptyBalanceSeats.RemoveAll();
			m_lockObjects.Lock();	// Lock
			m_info.GetEmptyBalanceSeats(arrEmptyBalanceSeats);
			m_lockObjects.Unlock();	// Unlock
			}
		}
	else{
		// Perform buy/rebuy chips action.
		if( PerformAddChipsAction() > 0 ){
			if( !GThread_Sleep(1000) ) // Make add chips action be visible for every one.
				return true; // Thread stopped!!!
			}
		}

	if( nTournamentId > 0 ){
		// Seat out seats.
		for( int i=0; i<arrSitOutSeats.GetCount(); i++ )
			SitOutBySeat((int)arrSitOutSeats[i]);
		// Empty seats zero balanced seats.
		for( int i=0; i<arrEmptyBalanceSeats.GetCount(); i++ ){
			EmptySeat((int)arrEmptyBalanceSeats[i]);
			nEmptySeatCt ++;
			}
		}
	else{
		// Empty offline seats.
		EmptyOfflineSeats();

		// Force seats to seat out.
		for( int i=0; i<arrSitOutSeats.GetCount(); i++ )
			SitOutBySeat((int)arrSitOutSeats[i]);
		for( int i=0; i<arrEmptyBalanceSeats.GetCount(); i++ ){
			SitOutBySeat((int)arrEmptyBalanceSeats[i], false);
			}

		// Seat wait list users, before empty seat appears to enyone.
		SeatWaitListUsers(arrSeated);
		}

	// Hand finished.
	HandFinished(bTerminate, nAvgHandDuration);
	// Call hand finished callback.
	m_pGameBase->HandFinished(this, m_pParam, &m_info, m_nHandId, nTotalPotAmount, nRakeAmount, &m_handHistory, sPlayersInfo, sTableCards);
	return true;
	}

bool
PokerCashTableController::GThread_FinishRound(bool& bHandFinished){
	m_lockObjects.Lock();	// Lock
	PokerRoundType	round			= m_info.m_round;
	int				nInPlaySeatCt	= m_info.CountSeatsByStatus(PokerSeatStatus::InPlay) + (m_info.m_bTournamentTable ? m_info.CountSeatsByStatus(PokerSeatStatus::SittedOut) : m_info.CountSitoutButInPlaySeats());
	int				nAllInSeatCt	= m_info.CountSeatsByStatus(PokerSeatStatus::All_In);
	bool			bAllIn			= ((nInPlaySeatCt <= 1) && (nAllInSeatCt > 0));
	bHandFinished					= false;
	m_lockObjects.Unlock(); // Unlock

	if( round == PokerRoundType::No )
		return false;

	if( (nInPlaySeatCt + nAllInSeatCt) == 1 ){
		bHandFinished = true;
		return true; // Finish hand
		}

	// Show seat's hand cards when allin.
	m_info.ShowSeatCards(bAllIn);

	INT_CHIPS nTotalPot = 0, nRoundPot = 0;
	UINT nRakeAmount = 0;
	PokerRoundType roundStartForHistory = PokerRoundType::No;
	switch(round){
		case PokerRoundType::PreFlop:{
			m_lockObjects.Lock();	// Lock
			m_info.StartRound(PokerRoundType::Flop);
			// Hand history entries. {{
			if( m_info.GetRoundAmounts(PokerRoundType::PreFlop, nTotalPot, nRoundPot, nRakeAmount) ){
				m_handHistory.FinishRound	(nTotalPot, nRoundPot, nRakeAmount);
				roundStartForHistory = PokerRoundType::Flop;
				}
			// }}
			m_lockObjects.Unlock(); // Unlock
			break;
			}
		case PokerRoundType::Flop:{
			m_lockObjects.Lock();	// Lock
			m_info.StartRound(PokerRoundType::Turn);
			// Hand history entries. {{
			if( m_info.GetRoundAmounts(PokerRoundType::Flop, nTotalPot, nRoundPot, nRakeAmount) ){
				m_handHistory.FinishRound	(nTotalPot, nRoundPot, nRakeAmount);
				roundStartForHistory = PokerRoundType::Turn;
				}
			// }}
			m_lockObjects.Unlock(); // Unlock
			break;
			}
		case PokerRoundType::Turn:{
			m_lockObjects.Lock();	// Lock
			// Hand history entries. {{
			if( m_info.GetRoundAmounts(PokerRoundType::Turn, nTotalPot, nRoundPot, nRakeAmount) ){
				m_handHistory.FinishRound	(nTotalPot, nRoundPot, nRakeAmount);
				roundStartForHistory = PokerRoundType::River;
				}
			// }}
			m_info.StartRound(PokerRoundType::River);
			m_lockObjects.Unlock(); // Unlock
			break;
			}
		case PokerRoundType::River:{
			// Hand history entries. {{
			if( m_info.GetRoundAmounts(PokerRoundType::River, nTotalPot, nRoundPot, nRakeAmount) ){
				m_handHistory.FinishRound	(nTotalPot, nRoundPot, nRakeAmount);
				m_handHistory.StartRound	(PokerRoundType::Showdown, NULL, NULL, nTotalPot, nRakeAmount);
				}
			// }}

			bHandFinished = true;
			return true;
			}
		};

	m_lockObjects.Lock();	// Lock
	PokerRoundStartedPacket p;
	p.ZeroPacketVariables();
	m_info.m_cActiveSeat			= -1; // Deactivate recently active seat.
	p.m_dtAction					= COleDateTime::GetCurrentTime();
	p.m_nTableId					= m_info.m_nTableId;
	p.m_nTournamentId				= m_info.m_nTournamentId;
	p.m_nHandId						= m_nHandId;
	p.m_cRoundType					= (char)m_info.m_round;
	p.m_nMainPotAmount				= m_info.GetTotalPotAmount();
	p.m_nTotalPotAmount				= m_info.GetTotalPotAmount();
	p.m_wRakeAmount					= (WORD)m_info.CalculateRakeAmount(p.m_nTotalPotAmount);
	p.m_cRoundStartSec				= (char)m_nStartRoundDelay;
	p.m_btTableCard1				= (BYTE)m_info.m_cCard1;
	p.m_btTableCard2				= (BYTE)m_info.m_cCard2;
	p.m_btTableCard3				= (BYTE)m_info.m_cCard3;
	p.m_btTableCard4				= (BYTE)m_info.m_cCard4;
	p.m_btTableCard5				= (BYTE)m_info.m_cCard5;

	// Retrieve detailed pots info.
	m_info.GetDetailedPots(&p.m_listPotAmounts);

	int					nHandCardsClass				= 0;
	int					nHandCardClasses[]			= {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	PokerHandCards*		ppHandCardClasses[]			= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HandWinnerCalculator*	pCalc	= m_info.GetHandWinnerCalculator();

	// When seat hand cards are shown.
	if( m_info.m_bCardsShown ){
		// Calculate seat best hand card classes.
		m_info.GetSeatsHandCards(&p.m_listHandCards, NULL);
		for( int i=0; i<p.m_listHandCards.GetCount(); i++ ){
			int nSeat = (int)p.m_listHandCards[i]->m_btSeat;
			nHandCardClasses[nSeat] = (int)p.m_listHandCards[i]->m_cBestCardsClass;
			ppHandCardClasses[nSeat] = p.m_listHandCards[i];
			}
		}
	
	for( int i=0; i<m_info.m_cMaxPlayersCt; i++ ){
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		if( !pSeat || !pSeat->GetSession(false) || pSeat->IsOffline() ) continue;
		Session* pSess = pSeat->GetSession(false);

		// In-play seat.
		if( m_info.IsInPlayByStatus(i, pSeat->GetStatus()) /*pSeat->GetStatus() == PokerSeatStatus::All_In || pSeat->GetStatus() == PokerSeatStatus::InPlay*/ ){	
			p.m_nReceiverSession	= pSess->GetSessionId();
			p.m_cReceiverSeat		= (char)pSeat->GetSeat();

			// Calculate best hand value. {{
			if( nHandCardClasses[i] == -1 && pCalc ){
				CDWordArray arrCardsIdx;
				pCalc->GetBestHandCardValue(pSeat->GetCard1(), pSeat->GetCard2(), pSeat->GetCard3(), pSeat->GetCard4(), 
					m_info.m_cCard1, m_info.m_cCard2, m_info.m_cCard3, m_info.m_cCard4, m_info.m_cCard5, (PokerHandCardsClass&)nHandCardsClass, arrCardsIdx);

				if( arrCardsIdx.GetCount() == 5 ){
					p.m_cBestCard1 = (char)arrCardsIdx.GetAt(0);
					p.m_cBestCard2 = (char)arrCardsIdx.GetAt(1);
					p.m_cBestCard3 = (char)arrCardsIdx.GetAt(2);
					p.m_cBestCard4 = (char)arrCardsIdx.GetAt(3);
					p.m_cBestCard5 = (char)arrCardsIdx.GetAt(4);
					}

				nHandCardClasses[i]		= nHandCardsClass;
				p.m_cBestCardsClass	= (char)nHandCardsClass;
				}
			// Already calculated !!!
			else{
				p.m_cBestCardsClass	= (char)nHandCardClasses[i];
				if( ppHandCardClasses[i] != NULL ){
					p.m_cBestCard1 = (char)ppHandCardClasses[i]->m_cBestCard1;
					p.m_cBestCard2 = (char)ppHandCardClasses[i]->m_cBestCard2;
					p.m_cBestCard3 = (char)ppHandCardClasses[i]->m_cBestCard3;
					p.m_cBestCard4 = (char)ppHandCardClasses[i]->m_cBestCard4;
					p.m_cBestCard5 = (char)ppHandCardClasses[i]->m_cBestCard5;
					}
				else
					p.m_cBestCard1 = p.m_cBestCard2 = p.m_cBestCard3	= p.m_cBestCard4 = p.m_cBestCard5	= -1;
				}
			// }}

			// Build pre-actions list. {{
			if( !bAllIn )
				m_info.GetSeatPreActions(i, &p.m_listPreActions);
			// }}
			}
		else{
			p.m_cReceiverSeat		= (char)-1;
			p.m_nReceiverSession	= 0;
			p.m_cBestCardsClass		= PokerHandCardsClass::CardsClass_None;
			}

		// Send packet.
		m_lockObjects.Unlock(); // Unlock
		m_pServer->SendPacket(pSeat->GetConnection(), &p);
		// Remove pre-actions
		p.m_listPreActions.RemoveAll(); 
		m_lockObjects.Lock(); // Unlock
		}

	// Start hand history round.
	m_handHistory.StartRound(roundStartForHistory, &p.m_listHandCards, &p.m_listPotAmounts, p.m_nTotalPotAmount, (int)p.m_wRakeAmount);
	m_lockObjects.Unlock(); // Unlock

	// Send packet table observers.
	p.m_cReceiverSeat		= (char)-1;
	p.m_nReceiverSession	= 0;
	p.m_cBestCardsClass		= PokerHandCardsClass::CardsClass_None;
	m_pServer->SendPacketByTable(p.m_nTableId, m_info.m_nTournamentId, &p, true);
	return true;
	}

bool
PokerCashTableController::GThread_Sleep(int nMSecSleep, int nMSecSingleSleep /*= 250*/){
	if( !m_gameThread.IsCurrentThread() )
		return false;
	int nMSec = nMSecSingleSleep, nLoop = 0, nLoopCt = (nMSecSleep / nMSec);
	while( nLoop < nLoopCt ){
		if( m_gameThread.IsStopEventSignaled() )
			return false;
		Sleep(nMSec);
		nLoop ++;
		}
	return true;
	}

bool
PokerCashTableController::GThread_WaitForResponse(ClientConnection* pConn, BasePacket* pSent, int nSecWait, BasePacket* pPacketReceived,  bool& bElapsed, int* pSecWasted){
	if( !m_gameThread.IsCurrentThread() )
		return false;

	// Wait for reply.
	int nMSecSleep	= 250;
	int nLoop		= 0;
	int	nLoopCt		= (nSecWait * 1000) / nMSecSleep;
	int nMSecPassed	= 0;
	int nMSecWait	= nSecWait*1000;
	bElapsed		= true;

	// Add thread params.
	m_gameThread.AddParam(_T("CONN"),			pConn);
	m_gameThread.AddParam(_T("PACKET"),			pSent);
	m_gameThread.AddParam(_T("RECV"),			pPacketReceived);
	m_gameThread.AddParam(_T("MSEC_PASSED"),	&nMSecPassed);
	m_gameThread.AddParam(_T("MSEC_WAIT"),		&nMSecWait);

	// Reset sync event.
	m_gameThread.SetSyncEvent(false);

	while( nLoop < nLoopCt ){
		if( m_gameThread.IsStopEventSignaled() )
			return false;
		if( m_gameThread.IsSyncEventSignaled() ){
			bElapsed = false;
			break; // Reply received.
			}
		// Sleep some time.
		Sleep(nMSecSleep);

		nMSecPassed += nMSecSleep;
		nLoop		++;
		}

	// Reset sync event.
	m_gameThread.SetSyncEvent(false);
	if( pSecWasted ){
		*pSecWasted = (nLoop*nMSecSleep) / 1000;
		}

	// Remove thread params.
	m_gameThread.RemoveAllParams();
	return true;
	}

bool
PokerCashTableController::GThread_ActionResponseRecieved(PokerActionPacket* p, int nSecElapsed, PokerSeatStatus statusSet /*= -1*/){
	int		nTableId = m_info.m_nTableId, nSeat	= (int)p->m_cSeat;
	// Set time bank seconds.
	if( nSecElapsed > (m_nRegularTimeSec + m_nExtraTimeSec) ){
		int nTimeBankLeft = m_info.GetTimeBankSeconds(nSeat) - (nSecElapsed - (m_nRegularTimeSec + m_nExtraTimeSec));
		m_info.SetTimeBankSeconds(nSeat, nTimeBankLeft);
		}

	INT_CHIPS nAmount			= p->m_nAmount; // Amount value is validated.
	switch( p->m_cActionType ){
		case PokerActionTypes::PostBB:{
			nAmount			= m_info.GetBBAmount();
			m_nBigBlind		= nSeat;

			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat) - nAmount;
			p->m_nPotAmount	= nAmount;
			m_info.SetBalance	(nSeat, p->m_nBalance);
			m_info.AddPotAmount	(nSeat, nAmount);
			m_lockObjects.Unlock(); // Unlock
			// }}
			// Post PostBlind {{
			SendSeatActionPacket(nSeat, (PokerActionTypes)p->m_cActionType, (statusSet == -1 ? PokerSeatStatus::Sitted : statusSet), nAmount, false, nSecElapsed);
			// }}
			break;
			}
		case PokerActionTypes::PostBB_Plus_SB:{
			nAmount			= m_info.GetBBAmount();
			int nFineAmount	= m_info.GetSBAmount();
			m_nBigBlind		= nSeat;

			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat) - (nAmount + nFineAmount);
			p->m_nPotAmount	= nAmount;
			m_info.SetBalance		(nSeat, p->m_nBalance);
			m_info.AddPotAmount		(nSeat, nAmount);
			m_info.AddFineAmount	(nFineAmount);
			m_lockObjects.Unlock(); // Unlock
			// }}
			// Post PostBlind {{
			SendSeatActionPacket(nSeat, (PokerActionTypes)p->m_cActionType, (statusSet == -1 ? PokerSeatStatus::Sitted : statusSet), nAmount, false, nSecElapsed);
			// }}
			break;
			}
		case PokerActionTypes::PostSB:{
			nAmount			= m_info.GetSBAmount();
			m_nSmallBlind	= nSeat;

			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat) - nAmount;
			p->m_nPotAmount	= nAmount;
			m_info.SetBalance	(nSeat, p->m_nBalance);
			m_info.AddPotAmount	(nSeat, nAmount);
			m_lockObjects.Unlock(); // Unlock
			// }}

			// Post PostBlind.
			SendSeatActionPacket(nSeat, (PokerActionTypes)p->m_cActionType, (statusSet == -1 ? PokerSeatStatus::Sitted : statusSet), nAmount, false, nSecElapsed);
			break;
			}
		case PokerActionTypes::SitOut:{
			/*
			m_lockObjects.Lock();	// Lock
			if( m_info.m_gameStatus == PokerGameStatus::Starting )
				m_info.m_gameStatus = PokerGameStatus::NotStarted;
			m_lockObjects.Unlock();	// Unlock
			*/
			SitOutBySeat(nSeat); // Seat out.
			break;
			}
		case PokerActionTypes::Call:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat) - nAmount;
			m_info.SetBalance	(nSeat, p->m_nBalance);
			m_info.AddPotAmount	(nSeat, nAmount);
			p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
			m_lockObjects.Unlock(); // Unlock
			// }}

			SendSeatActionPacket(nSeat, PokerActionTypes::Call, (statusSet == -1 ? PokerSeatStatus::InPlay : statusSet), nAmount, false, nSecElapsed);
			break;
			}
		case PokerActionTypes::Bet:
		case PokerActionTypes::Raise:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			if( p->m_cActionType == PokerActionTypes::Raise ){
				INT_CHIPS nPotAmount	= m_info.GetPotAmount(nSeat);
				p->m_nBalance			= m_info.GetBalance(nSeat) - nAmount + nPotAmount;
				m_info.SetBalance	(nSeat, p->m_nBalance);
				m_info.AddPotAmount	(nSeat, nAmount - nPotAmount);
				p->m_nPotAmount			= m_info.GetPotAmount(nSeat);
				}
			else{
				p->m_nBalance	= m_info.GetBalance(nSeat) - nAmount;
				m_info.SetBalance	(nSeat, p->m_nBalance);
				m_info.AddPotAmount	(nSeat, nAmount);
				p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
				}
			m_lockObjects.Unlock(); // Unlock
			// }}

			bool bAllIn	= (p->m_nBalance <= 0);
			if( bAllIn )
				SendSeatActionPacket(nSeat, PokerActionTypes::AllIn, PokerSeatStatus::All_In, nAmount, false, nSecElapsed);
			else
				SendSeatActionPacket(nSeat, (PokerActionTypes)p->m_cActionType, (statusSet == -1 ? PokerSeatStatus::InPlay : statusSet), nAmount, false, nSecElapsed);
			break;
			}
	    case PokerActionTypes::AllIn:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			nAmount			= m_info.GetBalance(nSeat);
			p->m_nBalance	= 0;
			m_info.SetBalance	(nSeat, p->m_nBalance);
			m_info.AddPotAmount	(nSeat, nAmount);
			p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
			m_lockObjects.Unlock(); // Unlock
			// }}
			SendSeatActionPacket(nSeat, PokerActionTypes::AllIn, PokerSeatStatus::All_In, nAmount, false, nSecElapsed);
			break;
			}
		case PokerActionTypes::Check:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat);
			p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
			m_lockObjects.Unlock(); // Unlock
			// }}
			SendSeatActionPacket(nSeat, PokerActionTypes::Check, (statusSet == -1 ? PokerSeatStatus::InPlay : statusSet), 0, false, nSecElapsed);		// Act check
			break;
			}
	    case PokerActionTypes::Fold:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat);
			p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
			m_lockObjects.Unlock(); // Unlock
			// }}
			SendSeatActionPacket(nSeat, PokerActionTypes::Fold, PokerSeatStatus::Folded, 0, false, nSecElapsed);		// Act fold
			break;
			}
		default:
			return false;
		};
	return true;
	}

bool
PokerCashTableController::GThread_ActTimerElapsed(PokerActPacket* pSent, ClientConnection* pConn, int& nSeatFrom, int nElapsedTimeSec){
	if( !pSent /*|| !pConn*/ ) return false;
	bool bSitOut	= false;
	bool bCheck		= false;
	bool bFold		= false;
	int  nSeat		= (int)pSent->m_cSeat;
	nSeatFrom		= nSeat;

	for(int i=0; i<pSent->m_actions.GetCount(); i++){
		PokerAction* pAction = pSent->m_actions.GetAt(i);
		if( !pAction ) continue;

		if( pAction->m_cActionType == PokerActionTypes::Check )
			bCheck = true;
		else
		if( pAction->m_cActionType == PokerActionTypes::Fold )
			bFold = true;
		else
		if( pAction->m_cActionType == PokerActionTypes::SitOut )
			bSitOut = true;
		}

	// Empty Time bank seconds.
	m_info.SetTimeBankSeconds	(nSeat, 0);
	m_info.SetCollectedTimeBank	(nSeat, 0.0f);

	if( bCheck ){
		//Old code
		//SendSeatActionPacket(nSeat, PokerActionTypes::Check, PokerSeatStatus::InPlay, 0, false, nElapsedTimeSec);// Act check
		SendSeatActionPacket(nSeat, PokerActionTypes::Check, PokerSeatStatus::SittedOut, 0, true, nElapsedTimeSec);// Act check
		// Set sitout by inplay state.
		m_info.SetSitOutButInPlay(nSeat, true);
		}
	else
	if( bFold )
		SendSeatActionPacket(nSeat, PokerActionTypes::Fold, PokerSeatStatus::Folded, 0, true, nElapsedTimeSec);// Act fold
	else
	if( bSitOut )
		SitOut(nSeat, pConn); // Sit out
	return true;
	}

bool
PokerCashTableController::GThread_AskToPostBlind(int nSeat, bool bSmallBlind, ClientConnection* pConn, PokerActionPacket* pPacketRecv, bool& bSuccess, bool bCheckForAutoPostFlag /*= true*/, bool bAllowedToFine /*= false*/){
	int		nSecWasted = 0;
	bool	bResult = false, bTimeElapsed = false;
	bSuccess		= false;

	SendPostBlindRequest(nSeat, bSmallBlind, bResult, bCheckForAutoPostFlag, bAllowedToFine);
	if( !bResult ){
		BasePacket* pPacketSent = (BasePacket*)m_gameThread.GetParam(_T("PACKET"));
		if( !GThread_WaitForResponse(pConn, pPacketSent, m_nPostBlindDelay, pPacketRecv, bTimeElapsed, &nSecWasted) ){
			// Delete sent packet object.
			if( pPacketSent )
				delete pPacketSent;
			return false; 
			}

		// Delete sent packet object.
		if( pPacketSent )
			delete pPacketSent;

		if( bTimeElapsed )
			SitOutBySeat(nSeat, false, (bSmallBlind ? true : false), (!bSmallBlind ? true : false));// Sit out.
		else{
			// Validate received packet.
			if( pPacketRecv->m_nCode == PacketCodes::_PokerAction ){
				if( pPacketRecv->m_cActionType == PokerActionTypes::PostSB || 
					pPacketRecv->m_cActionType == PokerActionTypes::PostBB ||
					pPacketRecv->m_cActionType == PokerActionTypes::PostBB_Plus_SB ){
					GThread_ActionResponseRecieved(pPacketRecv, nSecWasted);
					bSuccess = true;
					}
				else
					SitOutBySeat(nSeat, false, (bSmallBlind ? true : false), (!bSmallBlind ? true : false));// Sit out.
				}
			}
		}
	else
		bSuccess = true;
	return true;
	}


#define ALLOW_TEST_BOTS

bool
PokerCashTableController::GThread_AskForAction(int nSeatPrev, int& nSeatAsked, bool& bRoundEnded, bool& bOpenCards){
	m_lockObjects.Lock();	// Lock
	PokerGameStatus status	= m_info.m_gameStatus;
	PokerRoundType round	= m_info.m_round;
	int nTableId			= m_info.m_nTableId;
	int nTournamentId		= m_info.m_nTournamentId;
	int nBigBlind			= m_nBigBlind;
	int nDealer				= m_nDealer;
	m_lockObjects.Unlock(); // Unlock

	if( status != PokerGameStatus::Started || round == PokerRoundType::No )
		return false;

	m_lockObjects.Lock();	// Lock
	int					nSeatAsk	= -1;
	PokerTableSeatInfo* pSeat		= m_info.GetNextActiveSeat(nSeatPrev, nSeatAsk, bRoundEnded, bOpenCards);
	bool				bPreFlop	= (round == PokerRoundType::PreFlop);
	nSeatAsked						= nSeatAsk;
	m_lockObjects.Unlock(); // Unlock

	if( bRoundEnded ) return true;

	if( pSeat && nSeatAsk > -1 ){
		m_lockObjects.Lock();	// Lock
		INT_CHIPS		nCallAmount		= m_info.GetCallAmount(nSeatAsk);
		INT_CHIPS		nMinBetAmount	= 0;
		INT_CHIPS		nMaxBetAmount	= 0;
		INT_CHIPS		nBalance		= pSeat->GetBalance();
		int				nSeat			= pSeat->GetSeat();
		INT_CHIPS		nPotAmount		= pSeat->GetPotAmount();
		int				nTimeBankSec	= pSeat->GetTimeBankSeconds();
		float			fCollectedSec	= pSeat->GetCollectedTimeBank();

		m_info.GetBetAmounts(nSeatAsk, nMinBetAmount, nMaxBetAmount);
		m_lockObjects.Unlock();	// Unlock

		PokerActPacket actPacket;
		PokerActPacket* p		= &actPacket;
		p->m_nTableId			= nTableId;
		p->m_nTournamentId		= nTournamentId;
		p->m_nHandId			= m_nHandId;
		p->m_cSeat				= (char)nSeat;
		p->m_cActionTime		= (char)(m_nRegularTimeSec + m_nExtraTimeSec + nTimeBankSec);

		if( nCallAmount == 0 ){ // Fold - Check - [Bet/Raise]
			PokerAction* pAction	= new PokerAction();
			pAction->m_cActionType	= PokerActionTypes::Fold;
			pAction->m_nAmountMin	= 0;
			pAction->m_nAmountMax	= 0;
			p->m_actions.Add(pAction);

			pAction	= new PokerAction();
			pAction->m_cActionType	= PokerActionTypes::Check;
			pAction->m_nAmountMin	= 0;
			pAction->m_nAmountMax	= 0;
			p->m_actions.Add(pAction);

			if( nBalance > 0 ){
				if( nMinBetAmount >= nBalance ){	
					pAction	= new PokerAction(); // All-In
					pAction->m_cActionType	= PokerActionTypes::AllIn;
					pAction->m_nAmountMin	= nBalance;
					pAction->m_nAmountMax	= nBalance;
					p->m_actions.Add(pAction);
					}
				else{
					pAction	= new PokerAction();
					if( nPotAmount > 0 ){ // Raise To
						pAction->m_cActionType	= PokerActionTypes::Raise;
						pAction->m_nAmountMin	= nMinBetAmount + nPotAmount;
						pAction->m_nAmountMax	= min(nMaxBetAmount, nBalance) + nPotAmount;
						}
					else{// Bet
						pAction->m_cActionType	= PokerActionTypes::Bet;
						pAction->m_nAmountMin	= nMinBetAmount;
						pAction->m_nAmountMax	= min(nMaxBetAmount, nBalance);
						}
					p->m_actions.Add(pAction);
					}
				}
			}
		else{	// Fold - Call - [Bet/Raise]
			PokerAction* pAction	= new PokerAction(); // Fold
			pAction->m_cActionType	= PokerActionTypes::Fold;
			pAction->m_nAmountMin	= 0;
			pAction->m_nAmountMax	= 0;
			p->m_actions.Add(pAction);

			if( nBalance > 0 ){
				if( nCallAmount >= nBalance ){
					pAction	= new PokerAction(); // All-In
					pAction->m_cActionType	= PokerActionTypes::AllIn;
					pAction->m_nAmountMin	= nBalance;
					pAction->m_nAmountMax	= nBalance;
					p->m_actions.Add(pAction);
					}
				else{
					pAction	= new PokerAction(); // Call
					pAction->m_cActionType	= PokerActionTypes::Call;
					pAction->m_nAmountMin	= nCallAmount;
					pAction->m_nAmountMax	= nCallAmount;
					p->m_actions.Add(pAction);

					if( nMinBetAmount > 0 ){
						if( nMinBetAmount >= nBalance ){
							pAction	= new PokerAction(); // All-In
							pAction->m_cActionType	= PokerActionTypes::AllIn;
							pAction->m_nAmountMin	= nBalance;
							pAction->m_nAmountMax	= nBalance;
							p->m_actions.Add(pAction);
							}
						else{
							pAction	= new PokerAction();
							if( nPotAmount > 0 ){ // Raise To
								pAction->m_cActionType	= PokerActionTypes::Raise;
								pAction->m_nAmountMin	= nMinBetAmount + nPotAmount;
								pAction->m_nAmountMax	= min(nMaxBetAmount, nBalance) + nPotAmount;
								}
							else{// Bet
								pAction->m_cActionType	= PokerActionTypes::Bet;
								pAction->m_nAmountMin	= nMinBetAmount;
								pAction->m_nAmountMax	= min(nMaxBetAmount, nBalance);
								}
							p->m_actions.Add(pAction);
							}
						}
					}
				}
			}

		if( nSeatAsk != -1 ){
			// Analyze pre-actions. {{
			PokerActionPacket	autoActionPacket;
			PokerActionPacket* packet	= &autoActionPacket;
			packet->m_nTableId			= p->m_nTableId;
			packet->m_nTournamentId		= p->m_nTournamentId;
			packet->m_cSeat				= (char)nSeat;
			packet->m_cSeatStatus		= (char)pSeat->GetStatus();
			packet->m_cActionType		= PokerActionTypes::NoAction;
			packet->m_nAmount			= 0;

			// Activate check/fold on tournament table when seat is offline or in sitout state. {{
			if( /*m_info.m_bTournamentTable &&*/ (pSeat->IsOffline() || pSeat->GetStatus() == PokerSeatStatus::SittedOut)){
#ifndef ALLOW_TEST_BOTS
// Original code.
				pSeat->m_bPreAction_CheckFold = true;
				}
#else				
				// Temp
				if( pSeat->GetStatus() != PokerSeatStatus::SittedOut ){
					pSeat->m_bPreAction_CallAnyCheck	= true;
	//				pSeat->m_bPreAction_CheckFold		= true;
					if( nCallAmount >= pSeat->GetBalance() )
						//packet->m_cActionType = (char)PokerActionTypes::Call;
						packet->m_cActionType = (char)PokerActionTypes::AllIn;
					}
				// Temp
				}
			//Temp
			else{
				if( !pSeat->GetConnection() ){
					pSeat->m_bPreAction_CallAnyCheck	= true;
					if( nCallAmount >= pSeat->GetBalance() )
						packet->m_cActionType = (char)PokerActionTypes::AllIn;
					else
						packet->m_cActionType = (char)PokerActionTypes::Call;
					}
				}
			// Temp
#endif
			// }}

			PokerAction* pActionCall = NULL, *pActionCheck = NULL, *pActionFold = NULL, *pActionAllin = NULL;			
			for( int i=0; i<p->m_actions.GetCount(); i++ ){
				PokerAction* pAction = p->m_actions[i];
				if( !pAction ) continue;
				switch( pAction->m_cActionType ){
					case PokerActionTypes::AllIn:{
						pActionAllin = pAction;
						break;
						}
					case PokerActionTypes::Call:{
						pActionCall = pAction;
						break;
						}
					case PokerActionTypes::Fold:{
						pActionFold = pAction;
						break;
						}
					case PokerActionTypes::Check:{
						pActionCheck = pAction;
						break;
						}
					};
				}

			bool bAutoActionChosen	= false;
			bool bAutoCallAny_IsOn	= (pSeat->m_bPreAction_Call || pSeat->m_bPreAction_CallAny || pSeat->m_bPreAction_CallAnyCheck);
			bool bAutoCall_IsOn		= (pSeat->m_bPreAction_Call);
			bool bAutoFold_IsOn		= (pSeat->m_bPreAction_Fold || pSeat->m_bPreAction_CheckFold);
			bool bAutoCheck_IsOn	= (pSeat->m_bPreAction_Check || pSeat->m_bPreAction_CheckFold || pSeat->m_bPreAction_CallAnyCheck);
			// Auto Call action.
			if( pActionCall ){
				if( bAutoCallAny_IsOn ){
#ifndef ALLOW_TEST_BOTS
// Original
					packet->m_cActionType	= PokerActionTypes::Call;
#else					
					// Temp {{
					packet->m_cActionType	= PokerActionTypes::Call;
					//packet->m_cActionType	= PokerActionTypes::AllIn;
					// }}
#endif					
					packet->m_nAmount		= pActionCall->m_nAmountMin;
					bAutoActionChosen		= true;
					}
				else
				if( bAutoCall_IsOn && pActionCall->m_nAmountMin == pSeat->m_nPreAction_CallAmount ){
					packet->m_cActionType	= PokerActionTypes::Call;
					packet->m_nAmount		= pActionCall->m_nAmountMin;
					bAutoActionChosen		= true;
					}
				}
			else{
				if( pActionAllin &&  bAutoCallAny_IsOn ){
					packet->m_cActionType	= PokerActionTypes::AllIn;
					packet->m_nAmount		= pActionAllin->m_nAmountMin;
					bAutoActionChosen		= true;
					}
				}

			// Auto Check action.
			if( pActionCheck && bAutoCheck_IsOn && !bAutoActionChosen ){
				packet->m_cActionType	= PokerActionTypes::Check;
				packet->m_nAmount		= 0;
				bAutoActionChosen		= true;
				}

			// Auto Fold action.
			if( pActionFold && bAutoFold_IsOn && !bAutoActionChosen ){
				packet->m_cActionType	= PokerActionTypes::Fold;
				packet->m_nAmount		= 0;
				bAutoActionChosen		= true;
				}
			// }}

			// False seat pre-actions.
			pSeat->FalsePreActions();

			int nSeatFrom = 0;
			if( packet->m_cActionType != PokerActionTypes::NoAction ){
				// Action predicted by pre-action.
				if( pSeat->GetStatus() == PokerSeatStatus::SittedOut && packet->m_cActionType != PokerActionTypes::Fold )
					GThread_ActionResponseRecieved(packet, 0, PokerSeatStatus::SittedOut);
				else
					GThread_ActionResponseRecieved(packet, 0);
				
				// Sleep 1000 milisecond for auto action.
				if( !GThread_Sleep(1000) ){
					// Thread stop event is fired.
					return false;
					}
				}
			else{
				// Calculate time bank. {{
				int	nThinkTimeSec	= m_nRegularTimeSec + m_nExtraTimeSec;
				if( nTimeBankSec < m_nTimeBankSec ){
					nTimeBankSec	+= (int)fCollectedSec;
					fCollectedSec	-= (float)((int)fCollectedSec);
					nTimeBankSec	= min(m_nTimeBankSec, nTimeBankSec);
					pSeat->SetTimeBankSeconds	(nTimeBankSec);
					pSeat->SetCollectedTimeBank	(fCollectedSec);
					}
				else
					pSeat->SetCollectedTimeBank(0.0f);
				nThinkTimeSec += nTimeBankSec;
				// }}

				ClientConnection* pConnSend = pSeat->GetConnection();
				// Send act packet.
				m_pServer->SendPacket(pConnSend, p);
				// Send active seat packet.
				SendSetActiveSeat(nSeatAsk, m_nRegularTimeSec, m_nExtraTimeSec, nTimeBankSec);
				// Wait for reply from client. {{
				int					nSecWasted	= 0;
				bool				bElapsed	= false;
				PokerActionPacket*	pRecv		= new PokerActionPacket();
				if( !GThread_WaitForResponse(pConnSend, p, nThinkTimeSec, pRecv, bElapsed, &nSecWasted) ){
					delete pRecv;
					return false;
					}
				else{
					if( !bElapsed ){
						if( nSecWasted <= 1 ){
							// Sleep 250 milisecond for action.
							if( !GThread_Sleep(250) ){
								delete pRecv;
								// Thread stop event is fired.
								return false;
								}
							}
						GThread_ActionResponseRecieved(pRecv, nSecWasted);
						}
					else
						GThread_ActTimerElapsed(p, pConnSend, nSeatFrom, nSecWasted);
					}
				delete pRecv;
				// }}
				}
			}
		return true;
		}
	return false;
	}