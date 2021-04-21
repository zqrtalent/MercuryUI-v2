#include "stdafx.h"
#include "PokerServer.h"
#include "..\GameServer\ClientConnection.h"
#include "HoldemWinnerCalculator.h"
#include "GameController.h"
#include "..\Utility\Timers.h"

///////////////////////////////////////////////////////////
///
///	class PokerGameController
///
///////////////////////////////////////////////////////////

UINT	_stdcall	
PokerGameController::StartHandThreadProc(PokerGameController* pController){
	/*
	pController->m_lockStartHandThread.Lock();		// Lock
	CEvent*				pEventStop		= pController->m_eventStopHandStartThread;
	CEvent**			ppEventStop		= &pController->m_eventStopHandStartThread;
	HANDLE*				pThread			= &pController->m_hStartHandThread;
	HANDLE				hThread			= *pThread;
	CCriticalSection*	pLockObjects	= &pController->m_lockObjects;
	PokerTableInfo*		pTableInfo		= &pController->m_info;
	PokerServer*		pServer			= pController->m_pServer;
	CDWordArray			arrSitOutSeats;
	CDWordArray			arrEmptySeats;
	CDWordArray			arrPayBBSeats;
	pController->m_lockStartHandThread.Unlock();	// Unlock

	pLockObjects->Lock(); //Lock
	if( pTableInfo->m_gameStatus != PokerGameStatus::NotStarted ){
		pLockObjects->Unlock(); // Unlock
		goto finish_thread;
		}

	// Empty sessionless seats.
	pController->EmptySessionlessSeats();
	// False pre-actions.
	pTableInfo->FalsePreActions();

	if( pTableInfo->GetSittedCt() < 2 ){ // Insafficient count of ready seats.
		pTableInfo->SetSeatStatus(PokerSeatStatus::PayBBAndPlay, PokerSeatStatus::Sitted);
		pTableInfo->SetSeatStatus(PokerSeatStatus::WaitingForBB, PokerSeatStatus::Sitted);
		pLockObjects->Unlock(); // Unlock
		goto finish_thread;		// Hand start canceled.
		}

	pTableInfo->m_gameStatus			= PokerGameStatus::Starting; // Change to starting state.	
	int					nDealer			= -1;
	PokerTableSeatInfo*	pDSeat			= pTableInfo->GetDealerSeat(&nDealer);
	int					nBBSeat			= -1;
	int					nSBSeat			= -1;
	bool				bSBSeat			= false;
	bool				bBBSeat			= false;
	PokerTableSeatInfo* pSBSeatInfo		= pTableInfo->GetSBSeat(&nSBSeat);
	CEvent*				pComplete		= new CEvent(FALSE, TRUE);
	bool				bElapsed		= false;
	BasePacket*			pPacketReceived	= NULL;
	pLockObjects->Unlock(); // Unlock

	// Send PostSB request. {{
	if( nSBSeat != -1 ){
		pController->SendPostBlindRequest(nSBSeat, true, pComplete, &bElapsed, &pPacketReceived);
		while( TRUE ){
			// Check for signaled event.
			if( WaitForSingleObject(pComplete->m_hObject, 200) == WAIT_OBJECT_0 ) break;
			// Stop thread.
			if( WaitForSingleObject(pEventStop->m_hObject, 0) == WAIT_OBJECT_0 )  goto finish_thread;
			}

		pComplete->ResetEvent();
		if( bElapsed ){ // Request timer elapsed.
			pController->SitOutBySeat(nSBSeat, true);
			}
		else{			
			if( pPacketReceived && (pPacketReceived->m_nCode == PacketCodes::_PokerAction) ){
				PokerActionPacket* pAction = (PokerActionPacket*)pPacketReceived;
				if( pAction->m_cActionType == PokerActionTypes::PostSB ){
					nSBSeat		= (int)pAction->m_cSeat;
					bSBSeat		= true;
					}
				}

			// Destroy received packet.
			if( pPacketReceived ){
				delete pPacketReceived;
				pPacketReceived = NULL;
				}
			}
		}
	// }}

	// Set PostBB request.
	pLockObjects->Lock();	// Lock
	PokerTableSeatInfo* pBBSeatInfo	=  pTableInfo->GetBBSeat(nSBSeat, &nBBSeat);
	pLockObjects->Unlock(); // Unlock
	while( nBBSeat != -1 ){
		if( nBBSeat == nDealer ){
			nBBSeat = -1;
			continue; // Hand start failed.
			}

		if( pBBSeatInfo->GetSitOutNextBB() ){
			pController->SitOutBySeat(nBBSeat, true);
			pLockObjects->Lock();	// Lock
			pBBSeatInfo	= pTableInfo->GetBBSeat(nSBSeat, &nBBSeat);
			pLockObjects->Unlock(); // Unlock
			continue;
			}

		pController->SendPostBlindRequest(nBBSeat, false, pComplete, &bElapsed, &pPacketReceived);
		while( TRUE ){
			// Check for signaled event.
			if( WaitForSingleObject(pComplete->m_hObject, 200) == WAIT_OBJECT_0 ) break;
			// Stop thread.
			if( WaitForSingleObject(pEventStop->m_hObject, 0) == WAIT_OBJECT_0 ) goto finish_thread;
			}

		pComplete->ResetEvent();
		if( bElapsed ){ // Request timer elapsed.
			pController->SitOutBySeat(nBBSeat, true);
			}
		else{		
			if( pPacketReceived && pPacketReceived->m_nCode == PacketCodes::_PokerAction ){
				PokerActionPacket* pAction = (PokerActionPacket*)pPacketReceived;
				if( pAction->m_cActionType == PokerActionTypes::PostBB ){
					nBBSeat		= (int)pAction->m_cSeat;
					bBBSeat		= true;
					// Destroy received packet.
					if( pPacketReceived ){
						delete pPacketReceived;
						pPacketReceived = NULL;
						}
					break;
					}
				else{
					// Destroy received packet.
					if( pPacketReceived ){
						delete pPacketReceived;
						pPacketReceived = NULL;
						}
					pController->SitOutBySeat(nBBSeat, true);
					}
				}
			}

		pLockObjects->Lock();	// Lock
		pBBSeatInfo	= pTableInfo->GetBBSeat(nSBSeat, &nBBSeat);
		pLockObjects->Unlock(); // Unlock
		}

	// Deal cards and start hand. {{
	int nDealerPos				= nDealer;
	int nSBPos					= bSBSeat ? nSBSeat : -1;
	int nBBPos					= bBBSeat ? nBBSeat : -1;

	pTableInfo->GetPayBBAndPlaySeats(nBBPos, nDealerPos, arrPayBBSeats);
	if( !pController->DealCardsAndStartHand(nDealerPos, nSBPos, nBBPos, arrPayBBSeats) ){
		}
	// }}

	if( pComplete )
		delete pComplete;

finish_thread:
	
	pController->m_lockStartHandThread.Lock();		// Lock
	CloseHandle(hThread);
	*pThread		= NULL;
	if( pEventStop )
		delete pEventStop;
	*ppEventStop	= NULL;
	pController->m_lockStartHandThread.Unlock();	// Unlock
	*/
	return 0L;
	}

bool
PokerGameController::IsHandStartTimerStarted(){
	/*
	m_lockStartHandThread.Lock();	// Lock
	HANDLE hThread	= m_hStartHandThread;	
	m_lockStartHandThread.Unlock(); // Unlock
	return (hThread != NULL);
	*/ return false;
	}

bool
PokerGameController::StartHandThread(){
	/*
	DWORD	dwThreadId	= 0L;
	HANDLE	hThread		= NULL;

	m_lockStartHandThread.Lock();	// Lock
	hThread				= m_hStartHandThread;	

	if( hThread ){ // Hand start thread is already started.
		m_lockStartHandThread.Unlock(); // Unlock
		return false;
		}

	dwThreadId	= 0L;
	hThread		= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)PokerGameController::StartHandThreadProc, this, 0L, &dwThreadId);
	if( !dwThreadId ){
		m_lockStartHandThread.Unlock(); // Unlock
		return false;
		}

	m_hStartHandThread			= hThread;
	m_dwStartHandThreadId		= dwThreadId;
	m_eventStopHandStartThread	= new CEvent(FALSE, TRUE);
	m_lockStartHandThread.Unlock(); // Unlock
	*/
	return true;
	}

bool
PokerGameController::StopHandThread(){
	/*
	DWORD	dwThreadId	= 0L;
	HANDLE	hThread		= NULL;
	HANDLE	hEventStop	= NULL;

	m_lockStartHandThread.Lock();	// Lock
	hThread				= m_hStartHandThread;
	hEventStop			= m_eventStopHandStartThread ? m_eventStopHandStartThread->m_hObject : NULL;
	m_lockStartHandThread.Unlock(); // Unlock

	if( hThread != NULL ){ 
		::SetEvent(hEventStop);
		::WaitForSingleObject(hThread, INFINITE);
		return true;
		}*/
	return false;
	}