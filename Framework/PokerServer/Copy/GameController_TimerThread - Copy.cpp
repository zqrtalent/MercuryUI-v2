#include "stdafx.h"
#include "PokerServer.h"
#include "..\GameServer\ClientConnection.h"
#include "HoldemWinnerCalculator.h"
#include "GameController.h"

///////////////////////////////////////////////////////////
///
///	Timer thread methods.
///
///////////////////////////////////////////////////////////

UINT _stdcall 
PokerGameController::TimerThreadProc(PokerGameController* pController){
	CCriticalSection*	pLock					= &pController->m_lockTimerObjects;
	ThreadTimerInfo*	pThreadTimerInfo		= &pController->m_ThreadTimerInfo;
	PokerServer*		pServer					= pThreadTimerInfo->pServer;
	CEvent*				pEventStop				= pThreadTimerInfo->pEventStop;
	CEvent*				pEventResume			= pThreadTimerInfo->pEventResume;
	CEvent*				pEventCancel			= pThreadTimerInfo->pEventCancel;
	CEvent*				pEventCanceled			= pThreadTimerInfo->pEventCanceled;

	while( TRUE ){
		// Wait while event is not signaled.
		WaitForSingleObject(*pEventResume, INFINITE);

		// Check if stop event is signaled. {{
		if( WaitForSingleObject(*pEventStop, 0) == WAIT_OBJECT_0 ){
			break;
			}
		// }}

		if( pThreadTimerInfo->pServer ){
			pLock->Lock();			// Lock
			int		nLoop						= 0;
			int		nCt							= pThreadTimerInfo->nDelaySec*5;
			bool	bElapsed					= true;
			pThreadTimerInfo->nDelaySecPassed	= 0;
			pThreadTimerInfo->bIsWorking		= true;
			pLock->Unlock();		// Unlock

			// Send packet.
			if( pThreadTimerInfo->pConn && pThreadTimerInfo->pPacketSend ){
				pServer->SendPacketSheduled(pThreadTimerInfo->pConn, pThreadTimerInfo->pPacketSend);
				}

			while( nLoop < nCt ){
				Sleep(200);

				// Check for signaled state. {{
				if( WaitForSingleObject(*pEventCancel, 0) == WAIT_OBJECT_0 ){
					pLock->Lock();			// Lock
					pEventCancel->ResetEvent();
					pEventCanceled->SetEvent();
					bElapsed						= false;
					pThreadTimerInfo->bIsWorking	= false;
					// Delete sent packet. {{
					if( pThreadTimerInfo->pPacketSend ){
						delete pThreadTimerInfo->pPacketSend;
						pThreadTimerInfo->pPacketSend = NULL;
						}
					// }}
					pLock->Unlock();		// Unlock
					break;
					}
				// }}
				nLoop ++;

				// Every second.
				if( !(nLoop%5) ){
					pLock->Lock();	// Lock
					pThreadTimerInfo->nDelaySecPassed ++;
					pLock->Unlock();// Unlock
					}
				}
			if( !bElapsed ) continue;
			pLock->Lock(); // Lock

			// Signal event. {{
			if( pThreadTimerInfo->pSignal ){
				if( pThreadTimerInfo->pElapsed )
					*pThreadTimerInfo->pElapsed = true;
				pThreadTimerInfo->pSignal->SetEvent();
				}
			// }}

			if( pThreadTimerInfo->procElapsed ){
				OnTimerElapsed proc = pThreadTimerInfo->procElapsed;
				__asm{
					push eax;
					push ecx;
					mov eax, pController;
					mov ecx, eax;
					mov eax, proc;
					call eax;
					pop ecx;
					pop eax;
					};
				}

			if( pThreadTimerInfo->bContinue ){
				pThreadTimerInfo->bContinue = false;
				pLock->Unlock();		// Unlock
				continue;
				}

			// Delete sent packet. {{
			if( pThreadTimerInfo->pPacketSend ){
				delete pThreadTimerInfo->pPacketSend;
				pThreadTimerInfo->pPacketSend = NULL;
				}
			// }}
			pThreadTimerInfo->bIsWorking	= false;
			pLock->Unlock();// Unlock
			}
		else{
			pEventResume->ResetEvent();
			pEventStop	->ResetEvent();
			}
		}
	return 0L;
	}

bool
PokerGameController::InitializeTimer(ClientConnection* pConn, BasePacket* pPacketSend, 
									 int nDelaySec, OnTimerElapsed elapsedMethod,
									 CEvent* pEventSignal/*= NULL*/, bool* pElapsedStatus /*= NULL*/,
									 BasePacket** ppPacketReceived /*= NULL*/){
	if( !m_hTimerThread )
		return false; // Timer thread is not started.

	if( GetCurrentThreadId() == m_dwTimerThreadId ){ // Called from the timer thread.
		m_lockTimerObjects.Lock(); // Lock
		// Delete sent packet object. {{
		if( m_ThreadTimerInfo.pPacketSend ){
			delete m_ThreadTimerInfo.pPacketSend;
			}
		// }}

		// Initialize timer parameters. {{
		m_ThreadTimerInfo.nDelaySec		= nDelaySec;
		m_ThreadTimerInfo.pConn			= pConn;
		m_ThreadTimerInfo.procElapsed	= elapsedMethod;
		m_ThreadTimerInfo.pPacketSend	= pPacketSend;
		m_ThreadTimerInfo.bIsWorking	= false;
		m_ThreadTimerInfo.bContinue		= true;
		m_ThreadTimerInfo.pSignal		= pEventSignal;
		m_ThreadTimerInfo.pElapsed		= pElapsedStatus;
		m_ThreadTimerInfo.packetReceived= ppPacketReceived;
		// }}
		HANDLE hEventStop	= m_ThreadTimerInfo.pEventStop->m_hObject;
		HANDLE hEventResume = m_ThreadTimerInfo.pEventResume->m_hObject;
		m_lockTimerObjects.Unlock(); // Unlock

		// Start timer. {{
		::ResetEvent(hEventStop);
		::SetEvent	(hEventResume);
		// }}
		}
	else{
		CancelTimer(); // Cancel privious timer.

		m_lockTimerObjects.Lock(); // Lock
		// Initialize timer parameters. {{
		m_ThreadTimerInfo.nDelaySec		= nDelaySec;
		m_ThreadTimerInfo.pConn			= pConn;
		m_ThreadTimerInfo.procElapsed	= elapsedMethod;
		m_ThreadTimerInfo.pPacketSend	= pPacketSend;
		m_ThreadTimerInfo.bIsWorking	= false;
		m_ThreadTimerInfo.pSignal		= pEventSignal;
		m_ThreadTimerInfo.pElapsed		= pElapsedStatus;
		m_ThreadTimerInfo.packetReceived= ppPacketReceived;
		// }}

		HANDLE hEventStop	= m_ThreadTimerInfo.pEventStop->m_hObject;
		HANDLE hEventResume = m_ThreadTimerInfo.pEventResume->m_hObject;
		m_lockTimerObjects.Unlock(); // Unlock

		// Start timer. {{
		::ResetEvent(hEventStop);
		::SetEvent	(hEventResume);
		// }}
		}
	return true;
	}

bool
PokerGameController::CancelTimer(int* pSecondsElapsed){
	if( IsTimerWorking() ){
		m_lockTimerObjects.Lock(); // Lock
		HANDLE hEvent = *m_ThreadTimerInfo.pEventCanceled;
		m_ThreadTimerInfo.pEventCanceled->ResetEvent();
		m_ThreadTimerInfo.pEventCancel->SetEvent();
		if( pSecondsElapsed )
			*pSecondsElapsed = m_ThreadTimerInfo.nDelaySecPassed;
		m_lockTimerObjects.Unlock(); // Unlock

		WaitForSingleObject(hEvent, 300); // Wait till timer cancel.
		bool bRet = !IsTimerWorking();

		// Delete packet object. {{
		if( m_ThreadTimerInfo.pPacketSend ){
			delete m_ThreadTimerInfo.pPacketSend;
			m_ThreadTimerInfo.pPacketSend = NULL;
			}
		// }}
		return bRet;
		}
	if( pSecondsElapsed )
		*pSecondsElapsed = 0;
	return false;
	}

bool
PokerGameController::IsTimerWorking(){
	bool bRet = false;
	if( !m_hTimerThread ) // Timer thread is not created.
		return bRet;
	m_lockTimerObjects.Lock();		// Lock
	bRet = m_ThreadTimerInfo.bIsWorking;
	m_lockTimerObjects.Unlock();	// Unlock
	return bRet;
	}

bool
PokerGameController::StartTimerThread(){
	if( !m_hTimerThread ){
		m_ThreadTimerInfo.pEventStop	->ResetEvent();
		m_ThreadTimerInfo.pEventResume	->ResetEvent();
		m_ThreadTimerInfo.pEventCancel	->ResetEvent();
		m_ThreadTimerInfo.pEventCanceled->ResetEvent();
		m_ThreadTimerInfo.bIsWorking	= false;

		m_hTimerThread	= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)PokerGameController::TimerThreadProc, this, 0L, &m_dwTimerThreadId);
		::SetThreadPriority(m_hTimerThread, THREAD_PRIORITY_ABOVE_NORMAL);
		return true;
		}
	return false;
	}

bool
PokerGameController::StopTimerThread(){
	if( m_hTimerThread ){
		m_ThreadTimerInfo.pEventStop	->SetEvent();
		m_ThreadTimerInfo.pEventResume	->SetEvent();
		CancelTimer();
		WaitForSingleObject(m_hTimerThread, INFINITE);

		// Delete packet sent to client. {{
		if( m_ThreadTimerInfo.pPacketSend ){
			delete m_ThreadTimerInfo.pPacketSend;
			m_ThreadTimerInfo.pPacketSend = NULL;
			}
		// }}

		CloseHandle(m_hTimerThread);
		m_hTimerThread		= NULL;
		m_dwTimerThreadId	= 0L;
		return true;
		}
	return false;
	}

///////////////////////////////////////////////////////////
///
///	Timer thread elapse methods.
///
///////////////////////////////////////////////////////////

void
PokerGameController::OnActTimerElapsed(){
	m_lockTimerObjects.Lock();		// Lock
	ClientConnection*	pConn		= m_ThreadTimerInfo.pConn;
    PokerActPacket*		pActPacket	= (PokerActPacket*)m_ThreadTimerInfo.pPacketSend;
	ASSERT( pActPacket );

	bool bSitOut	= false;
	bool bCheck		= false;
	bool bFold		= false;

	for(int i=0; i<pActPacket->m_actions.GetCount(); i++){
		PokerAction* pAction = pActPacket->m_actions.GetAt(i);
		if( !pAction ) continue;

		if( pAction->m_cActionType == PokerActionTypes::Check ){
			bCheck = true;
			}
		else
		if( pAction->m_cActionType == PokerActionTypes::Fold ){
			bFold = true;
			}
		else
		if( pAction->m_cActionType == PokerActionTypes::SitOut ){
			bSitOut = true;
			}
		}

	// Empty Time bank seconds. {{
	m_info.SetTimeBankSeconds	((int)pActPacket->m_cSeat, 0);
	m_info.SetCollectedTimeBank	((int)pActPacket->m_cSeat, 0.0f);
	// }}

	m_lockTimerObjects.Unlock();	// Unlock

	if( bCheck ){
		PostSeatActionPacket((int)pActPacket->m_cSeat, PokerActionTypes::Check, PokerSeatStatus::InPlay, 0);// Act check
		}
	else
	if( bFold ){
		PostSeatActionPacket((int)pActPacket->m_cSeat, PokerActionTypes::Fold, PokerSeatStatus::Folded, 0);// Act fold
		}
	else
	if( bSitOut ){
		SitOut((int)pActPacket->m_cSeat, pConn); // Sit out
		}

	bool bRoundEnded = false;
	bool bOpenCards = false;
	AskForAction((int)pActPacket->m_cSeat, bRoundEnded, bOpenCards);
	if( bRoundEnded ){
		EndRound();
		}
	}

void
PokerGameController::OnPostBigBlindTimerElapsed(){
	PokerPostBlindPacket* pSent = (PokerPostBlindPacket*)m_ThreadTimerInfo.pPacketSend;
	SitOut((int)pSent->m_cSeat, m_ThreadTimerInfo.pConn);

	/*
	m_lockObjects.Lock();	// Lock
	if( m_info.m_gameStatus == PokerGameStatus::Starting ){
		m_info.m_gameStatus = PokerGameStatus::NotStarted;
		}
	m_lockObjects.Unlock(); // Unlock
	*/
	}

void
PokerGameController::OnPostSmallBlindTimerElapsed(){
	PokerPostBlindPacket* pSent = (PokerPostBlindPacket*)m_ThreadTimerInfo.pPacketSend;
	SitOut((int)pSent->m_cSeat, m_ThreadTimerInfo.pConn);

	/*
	m_lockObjects.Lock();	// Lock
	m_nSmallBlind					= -1;
	int					nBBSeat		= (int)pSent->m_cSeat;
	PokerTableSeatInfo* pBBSeatInfo = m_info.GetBBSeat(&nBBSeat);
	m_lockObjects.Unlock();	// Unlock
	if( pBBSeatInfo ){
		SendPostBlind(nBBSeat, false);
		}*/
	}

void	
PokerGameController::OnHandStartTimerElapsed(){
	StartHand();

	m_lockObjects.Lock();	// Lock
	m_bHandStartTimerActive = false;
	m_lockObjects.Unlock(); // Unlock
	}

void
PokerGameController::OnStartRoundTimerElapsed(){
	bool bRoundEnded	= false;
	bool bOpenCards		= false;
	AskForAction(-1, bRoundEnded, bOpenCards);
	if( bRoundEnded ){
		EndRound();
		}
	}

void
PokerGameController::OnShowMuckCardsTimerElapsed(){
	PokerShowMuckCardsPacket* packetSent = (PokerShowMuckCardsPacket*)m_ThreadTimerInfo.pPacketSend;
	if( packetSent ){
		m_lockObjects.Lock(); // Lock
		PokerSeatStatus status = m_info.GetSeatStatus((int)packetSent->m_cSeat);
		m_lockObjects.Unlock(); // Lock
		// Send seat action packet.
		PostSeatActionPacket(packetSent->m_cSeat, PokerActionTypes::Muck, status, 0);
		}

	// Activate hand start timer. {{
	OnTimerElapsed method = (OnTimerElapsed)&PokerGameController::OnHandStartTimerElapsed;
	InitializeTimer(NULL, NULL, m_nHandStartDelay, method);
	m_bHandStartTimerActive = true;
	// }}
	}