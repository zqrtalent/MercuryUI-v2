#include "stdafx.h"
#include "PokerServer.h"
#include "../GameServer/ClientConnection.h"
#include "HoldemWinnerCalculator.h"
#include <Mmsystem.h>
#include "PokerCashTableController.h"
#include "../Utility/Timers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerCashTableController
///
///////////////////////////////////////////////////////////

void	__stdcall 
PokerCashTableController::OnTimerEvent(int nIdEvent, void* lpParam){
	PokerCashTableController* pController = (PokerCashTableController*)lpParam;
	if( pController ){
		pController->OnTimerEventElapsed(nIdEvent);
		}
	}

void
PokerCashTableController::OnTimerEventElapsed(int nIdEvent){
	int nIdEventReservationMin = TIMER_SEAT_RESERVATION;
	int nIdEventReservationMax = TIMER_SEAT_RESERVATION + m_info.m_cMaxPlayersCt;
	if( nIdEvent >= nIdEventReservationMin && nIdEvent <= nIdEventReservationMax ){
		int nSeat = nIdEvent - nIdEventReservationMin;
		m_arrSeatReserveTimerIds[nSeat] = 0;
		OnReservationTimerElapsed(nSeat);
		return;
		}

	int nIdEventLeaveSeatMin = TIMER_LEAVE_SEAT;
	int nIdEventLeaveSeatMax = TIMER_LEAVE_SEAT + m_info.m_cMaxPlayersCt;
	if( nIdEvent >= nIdEventLeaveSeatMin && nIdEvent <= nIdEventLeaveSeatMax ){
		int nSeat = nIdEvent - nIdEventLeaveSeatMin;
		m_arrLeaveSeatTimerIds[nSeat] = 0;
		OnLeaveSeatTimerElapsed(nSeat);
		return;
		}
	}

void
PokerCashTableController::OnReservationTimerElapsed(int nSeat){
	m_lockObjects.Lock();			// Lock
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nSeat);
	if( !pSeatInfo || pSeatInfo->GetStatus() != PokerSeatStatus::Reserved ){
		m_lockObjects.Unlock();		// Unlock
		return;
		}

	CString				sUserName	= pSeatInfo->GetUserName();
	int					nUserId		= pSeatInfo->GetUserId();
	int					nTableId	= m_info.m_nTableId;
	ClientConnection*	pSeatConn	= pSeatInfo->GetConnection();
	Session*			pSess		= pSeatConn ? pSeatConn->GetSession(true) : NULL;

	// Empty seat.
	m_info.SetSeatInfo(nSeat, NULL);
	m_lockObjects.Unlock(); // Unlock

	// Send reservation canceled packet to reservation performer. {{
	if( pSeatConn ){
		PokerReserveSeatPacket p;
		p.ZeroPacketVariables();
		p.m_nTableId				= nTableId;
		p.m_bReservationExpired		= true;
		p.m_cSeat					= (char)nSeat;
		p.m_nAmountMax				= 0;
		p.m_nAmountMin				= 0;
		p.m_nBalance				= 0;
		m_pServer->SendPacket(pSeatConn, &p);
		}
	// }}

	// Send seat status change packet to everyone. {{
	PokerSeatStatusChangedPacket p;
	p.ZeroPacketVariables();
	p.m_nTableId		= nTableId;
	p.m_nTournamentId	= m_info.m_nTournamentId;
	p.m_cSeat			= (char)nSeat;
	p.m_status			= (char)PokerSeatStatus::Empty;
	p.m_sUserName		= sUserName;
	m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, &p);
	// }}

	// Call event method.
	m_pGameBase->PlayerCanceledReservation(this, m_pParam, (PokerSession*)pSess, nUserId, nTableId, nSeat);
	if( pSess ) SafePointer::Release(pSess); // Release session.
	}

void
PokerCashTableController::OnLeaveSeatTimerElapsed(int nSeat){
	ForceSeatToLeaveTable(nSeat);
	}

bool
PokerCashTableController::StartSeatReservationTimer(int nSeat){
	int nIdEvent = TIMER_SEAT_RESERVATION + nSeat;
	if( m_arrSeatReserveTimerIds[nSeat] ){
		StopSeatReservationTimer(nSeat);
		}
	int nTimerId = Timers::GetInstance()->SetTimer(nIdEvent, m_nReserveSeatDelay, OnTimerEvent, this, true);
	m_arrSeatReserveTimerIds[nSeat] = nTimerId;
	return true;
	}

bool
PokerCashTableController::StopSeatReservationTimer(int nSeat){
	int nTimerId = m_arrSeatReserveTimerIds[nSeat];
	Timers::GetInstance()->KillTimer(nTimerId);
	return true;
	}

bool
PokerCashTableController::StartLeaveSeatTimer(int nSeat, bool bSeatIsZeroBalanced /*= false*/){
	int nIdEvent = TIMER_LEAVE_SEAT + nSeat;
	if( m_arrLeaveSeatTimerIds[nSeat] )
		StopLeaveSeatTimer(nSeat);
	int nTimerId = Timers::GetInstance()->SetTimer(nIdEvent, (bSeatIsZeroBalanced ? m_nLeaveSitoutedDelay : m_nLeaveZeroBalancedSeatDelay), OnTimerEvent, this, true);
	m_arrLeaveSeatTimerIds[nSeat] = nTimerId;
	return true;
	}

bool
PokerCashTableController::StopLeaveSeatTimer(int nSeat){
	int nTimerId = m_arrLeaveSeatTimerIds[nSeat];
	Timers::GetInstance()->KillTimer(nTimerId);
	return true;
	}

void
PokerCashTableController::KillAllTimers(){
	int nLoop	= 0;
	int nCt		= sizeof(m_arrSeatReserveTimerIds) / sizeof(int);

	// Kill seat timers. {{
	Timers* pTimers = Timers::GetInstance();
	while( nLoop < nCt ){
		if( m_arrSeatReserveTimerIds[nLoop] ){
			pTimers->KillTimer(m_arrSeatReserveTimerIds[nLoop]);
			}
		if( m_arrLeaveSeatTimerIds[nLoop] ){
			pTimers->KillTimer(m_arrLeaveSeatTimerIds[nLoop]);
			}
		nLoop ++;
		}
	// }}
	}