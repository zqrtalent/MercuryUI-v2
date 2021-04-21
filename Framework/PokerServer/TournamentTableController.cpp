#include "stdafx.h"
#include "PokerServer.h"
#include "../GameServer/ClientConnection.h"
#include "HoldemWinnerCalculator.h"
#include "TournamentTableController.h"
#include "../Utility/Timers.h"
#include "PokerTournamentManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class TournamentGameController
///
///////////////////////////////////////////////////////////

TournamentTableController::TournamentTableController(PokerServer* pServer /*= NULL*/, PokerGameBase* pGameBase /*= NULL*/, PokerTournamentManager* pMan /*= NULL*/) : 
PokerCashTableController(pServer, pGameBase){
	if( !pServer ){ m_pServer = NULL; return; }
	m_pTournamentMan	= pMan;
	m_nTimeBankIncLevel = 0;
	m_bChangeStakes		= false;
	m_bCanceled			= false;

	m_nSBAmountNew		= m_nBBAmountNew = m_nAnteAmountNew = 0;
	m_nSBAmountNext		= m_nBBAmountNext = m_nAnteAmountNext = 0;
	} 

BEGIN_DECLARE_VAR(TournamentTableController, Serializable)
	DECLARE_VAR(_T("TournamentTableController"),	_T(""),		VariableType::VT_None, 0, false, true, false)
END_DECLARE_VAR()

TournamentTableController::~TournamentTableController(){
	if( !m_pServer ) return;
	}

void
TournamentTableController::SendStopSignal(){
	PokerCashTableController::SendStopSignal();
	}

void
TournamentTableController::OnOpenTable(PokerSession* pSess, ClientConnection* pConn, PokerOpenTablePacket* p){
	}

void
TournamentTableController::OnLeaveTable(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTablePacket* p){
	}

void
TournamentTableController::OnOpenTable_Tournament(PokerSession* pSess, ClientConnection* pConn, PokerOpenTable_TournamentPacket* p){
	ASSERT( pConn );
	ASSERT(p->m_nTableId == m_info.m_nTableId);
	int			nUserId		= pSess->GetUserId();

	// Next level's stakes.
	p->m_nSmallBlind_Next	= m_nSBAmountNext;
	p->m_nBigBlind_Next		= m_nBBAmountNext;
	p->m_nAnte_Next			= m_nAnteAmountNext;

	m_lockObjects.Lock();	// Lock
	p->m_nHandId			= m_nHandId;
	p->m_cSeat				= (char)-1;
	p->m_info.RemoveAll	();
	p->m_info.Add		(&m_info);

	// Reconnect seat from another session. {{
	PokerTableSeatInfo*	pSeatInfo = NULL;
	if( nUserId > 0 )
		pSeatInfo = m_info.GetSeatByUserId(nUserId);

	if( pSeatInfo ){
		p->m_nHandsDealed			= pSeatInfo->GetHandsDealed();
		p->m_nHandsPlayed			= pSeatInfo->GetHandsPlayed();
		p->m_nHandsWon				= pSeatInfo->GetHandsWon();
		p->m_nHandsWonPreflop		= pSeatInfo->GetHandsPreflop();
		p->m_nHandsWonPostflop		= pSeatInfo->GetHandsPostflop();
		p->m_nHandsWonShowdown		= pSeatInfo->GetHandsShowdown();

		// User's place index in tournament.
		p->m_nRankIndex				= pSeatInfo->GetUserPlaceIndex();

		int nSeat					= pSeatInfo->GetSeat();
		p->m_cSeat					= (char)nSeat;
		// Bring seat online.
		if( pSeatInfo->IsOffline() )
			SetSeatOffline(nSeat, true);
		// Set seat session.
		pSeatInfo->SetSession(pConn->GetSession(false), p->m_nTableId, p->m_nTournamentId);
		// Attach connection to seat.
		pSess->SeatAt(p->m_nTableId, nSeat, m_info.m_nTournamentId);
		}
	else{
		// User's place index in tournament.
		p->m_nRankIndex				= -1;
		}
	// }}

	BasePacket* pPacketRepeat = NULL;
	// Retrieve seat action wait process time info. <<
	m_gameThread.Lock();
	BasePacket*			pPacketSent	= (BasePacket*)m_gameThread.GetParam(_T("PACKET"));
	int*				pMSecPassed	= (int*)m_gameThread.GetParam(_T("MSEC_PASSED"));
	int*				pMSecWait	= (int*)m_gameThread.GetParam(_T("MSEC_WAIT"));
	int					nMSecPassed = pMSecPassed ? *pMSecPassed : 0;
	int					nMSecWait	= pMSecWait ? *pMSecWait : 0;
	m_gameThread.Unlock();
	p->m_cActiveSeatTimeFull	= nMSecWait / 1000;
	p->m_cActiveSeatTimePassed	= nMSecPassed / 1000;
	// >>

	if( pSeatInfo && pPacketSent && m_info.m_cActiveSeat == pSeatInfo->GetSeat() ){
		pPacketRepeat = m_pServer->CreatePacketByCode(pPacketSent->m_nCode);
		if( pPacketRepeat ){
			// Change waiting connection to accept action packet from new connection instance.
			m_gameThread.Lock();
			m_gameThread.SetParam(_T("CONN"), pConn);
			m_gameThread.Unlock();

			pPacketSent->Copy(pPacketRepeat);
			} 
		}
	m_lockObjects.Unlock(); // Unlock

	// Send reply.
	m_pServer->SendPacket	(pConn, p);
	p->m_info.RemoveAll		(false);

	// Repeat last sent action packet.
	if( pPacketRepeat ){
		m_pServer->SendPacket(pConn, pPacketRepeat);
		delete pPacketRepeat;
		}
	}

void
TournamentTableController::OnLeaveTable_Tournament(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTable_TournamentPacket* p){
	// If seat is taken by connection. {{
	m_lockObjects.Lock(); // Lock
	bool bOpened = false;
	int nSeatIndex = pSess->GetSeatIndex(m_info.m_nTableId, bOpened, m_info.m_nTournamentId);
	if( bOpened ){
		if( nSeatIndex != -1 )
			SetSeatOffline(nSeatIndex, true);
		// }}
//		p->m_cSeat = (char)nSeatIndex;
		m_lockObjects.Unlock(); // Unlock
		}
	else
		m_lockObjects.Unlock(); // Unlock
	}

void
TournamentTableController::OnSeatAt(PokerSession* pSess, ClientConnection* pConn, PokerSeatAtTablePacket* p){
	}

void
TournamentTableController::OnReserveSeat(PokerSession* pSess, ClientConnection* pConn, PokerReserveSeatPacket* p){
	}

void
TournamentTableController::OnCancelReservation(PokerSession* pSess, ClientConnection* pConn, PokerCancelReservationPacket* p){
	}

void
TournamentTableController::OnJoinWaitingList(PokerSession* pSess, ClientConnection* pConn, PokerJoinWaitingListPacket* p){
	}

void
TournamentTableController::OnAction(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p, bool bPreAction /*= false*/){
	m_gameThread.Lock();
	ClientConnection*	pConnSent	= (ClientConnection*)m_gameThread.GetParam(_T("CONN"));
	PokerActPacket*		pSent		= (PokerActPacket*)m_gameThread.GetParam(_T("PACKET"));
	BasePacket*			pRecv		= (BasePacket*)m_gameThread.GetParam(_T("RECV"));
	if( !pConnSent ){
		m_gameThread.Unlock(); // Unlock
		return;
		}

	if( pConnSent != pConn ){
		m_gameThread.Unlock(); // Unlock
		return;
		}

	if( pSent && pSent->m_nCode == PacketCodes::_PokerAct ){
		for( int i=0; i<pSent->m_actions.GetCount(); i++){
			PokerAction* pAction = pSent->m_actions.GetAt(i);
			if( pAction && pAction->m_cActionType == p->m_cActionType ){
				if( p->m_nAmount < pAction->m_nAmountMin || 
					p->m_nAmount > pAction->m_nAmountMax ){ // Invalid amount value received from client.
					m_gameThread.Unlock();
					return;
					}
				break;
				}
			}
		}

	if( pRecv && 
		pRecv->m_nCode == p->m_nCode ){
		// Copy received packet.
		p->Copy(pRecv);
		// Packet received.
		m_gameThread.SetSyncEvent(true);
		}
	m_gameThread.Unlock();
	}

void
TournamentTableController::OnShowMuckCards(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p){
	m_gameThread.Lock(); // Lock
	ClientConnection*	pConnSent	= (ClientConnection*)m_gameThread.GetParam(_T("CONN"));
	PokerActPacket*		pSent		= (PokerActPacket*)m_gameThread.GetParam(_T("PACKET"));
	BasePacket*			pRecv		= (BasePacket*)m_gameThread.GetParam(_T("RECV"));
	if( !pConnSent ){
		m_gameThread.Unlock(); // Unlock
		return;
		}

	if( pConnSent != pConn ){
		m_gameThread.Unlock(); // Unlock
		return;
		}

	if( pRecv && 
		pRecv->m_nCode == p->m_nCode ){
		// Copy received packet.
		p->Copy(pRecv);
		// Packet received.
		m_gameThread.SetSyncEvent(true);
		}
	m_gameThread.Unlock();
	}

void
TournamentTableController::OnTableInfo(PokerSession* pSess, ClientConnection* pConn, PokerTableInfoPacket* p){
	ASSERT( pConn );
	ASSERT(p->m_nTableId == m_info.m_nTableId);

	p->m_nHandId = m_nHandId;
	p->m_info.RemoveAll		();
	m_lockObjects.Lock(); // Lock
	p->m_info.Add			(&m_info);

	// Retrieve seat action wait process time info. <<
	m_gameThread.Lock();
	int*	pMSecPassed	= (int*)m_gameThread.GetParam(_T("MSEC_PASSED"));
	int*	pMSecWait	= (int*)m_gameThread.GetParam(_T("MSEC_WAIT"));
	int		nMSecPassed = pMSecPassed ? *pMSecPassed : 0;
	int		nMSecWait	= pMSecWait ? *pMSecWait : 0;
	m_gameThread.Unlock();
	p->m_cActiveSeatTimeFull	= nMSecWait / 1000;
	p->m_cActiveSeatTimePassed	= nMSecPassed / 1000;
	// >>
	m_lockObjects.Unlock(); // Unlock

	m_pServer->SendPacket	(pConn, p);
	p->m_info.RemoveAll				(false);
	}

void
TournamentTableController::OnSetOption(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p){
	int nSeat = (int)p->m_cSeat;
	m_lockObjects.Lock();		// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( pSeat ){
		switch( p->m_cOption ){
			case PokerTableOption::Option_AutoMuck:{
				pSeat->SetAutoMuck(p->m_bValue);
				break;
				}
			case PokerTableOption::Option_AutoRebuy:{
				pSeat->SetAutoRebuy(p->m_bValue);
				break;
				}
			case PokerTableOption::Option_AutoAddon:{
				pSeat->SetAutoAddon(p->m_bValue);
				break;
				}
			case PokerTableOption::Option_SitOutNextBB:{
				//Removed.
				//pSeat->SetSitOutNextBB(p->m_bValue);
				break;
				}
			case PokerTableOption::Option_SitOutNextHand:{
				pSeat->SetSitOutNextHand(p->m_bValue);
				/*
				// If hand is not started sit out at once.
				if( p->m_bValue && 
					pSeat->GetStatus() == PokerSeatStatus::Sitted &&
					m_info.m_gameStatus == PokerGameStatus::NotStarted ){
					SitOut(nSeat, pConn);
					}*/
				break;
				}
			case PokerTableOption::Option_Call:{
				pSeat->m_bPreAction_Call		= p->m_bValue;
				pSeat->m_nPreAction_CallAmount	= p->m_nValue;
				break;
				}
			
			case PokerTableOption::Option_CallAny:{
				pSeat->m_bPreAction_CallAny = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_CallAnyCheck:{
				pSeat->m_bPreAction_CallAnyCheck = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_Check:{
				pSeat->m_bPreAction_Check = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_CheckFold:{
				pSeat->m_bPreAction_CheckFold = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_Fold:{
				pSeat->m_bPreAction_Fold = p->m_bValue;
				break;
				}
			};
		}
	m_lockObjects.Unlock();		// Unlock
	}

void
TournamentTableController::OnReturnBack(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p){
	m_lockObjects.Lock();		// Lock
	int					nSeat	= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( !pSeat || pSeat->GetStatus() != PokerSeatStatus::SittedOut ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	if( pSeat->GetBalance() < m_info.m_nBigBlindAmount  ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	bool			bReturnInGame	= ((m_info.m_gameStatus == PokerGameStatus::Started || m_info.m_gameStatus == PokerGameStatus::Starting)  && m_info.GetSittedCt() >= 1 && GThread_IsStarted());
	PokerSeatStatus status			= pSeat->GetStatus();
	bool			bSuccess		= false;
	bool			bSitOutNextHand	= pSeat->GetSitOutNextHand();
	bool			bSitOutNextBB	= pSeat->GetSitOutNextBB();
	if( status == PokerSeatStatus::SittedOut ){
		pSeat->SetStatus((bReturnInGame ? PokerSeatStatus::InPlay : PokerSeatStatus::Sitted));

		bSuccess = true;
		// Clear SitOutNextHand / SitOutNextBB option.
		if( bSitOutNextHand )
			pSeat->SetSitOutNextHand(false);
		/*
		if( bSitOutNextBB )
			pSeat->SetSitOutNextBB(false);
			*/
		}
	
	if( bSuccess ){
		m_lockObjects.Unlock();		// Unlock
		// Send seat action.
		SendSeatActionPacket(nSeat, PokerActionTypes::ReturnBack, pSeat->GetStatus(), 0);

		PokerSetOptionPacket packet1;
		if( bSitOutNextHand ){
			// Clear SitOutNextHand option.
			packet1.m_nTableId		= m_info.m_nTableId;
			packet1.m_nTournamentId	= m_info.m_nTournamentId;
			packet1.m_cSeat			= (char)nSeat;
			packet1.m_cOption		= (char)PokerTableOption::Option_SitOutNextHand;
			packet1.m_bValue		= false;
			packet1.m_nValue		= 0;
			m_pServer->SendPacket(pConn, &packet1);
			}
		/*
		if( bSitOutNextBB ){
			// Clear SitOutNextHand option.
			packet1.m_nTableId		= m_info.m_nTableId;
			packet1.m_nTournamentId	= m_info.m_nTournamentId;
			packet1.m_cSeat			= (char)nSeat;
			packet1.m_cOption		= (char)PokerTableOption::Option_SitOutNextBB;
			packet1.m_bValue		= false;
			packet1.m_nValue		= 0;
			m_pServer->SendPacket(pConn, &packet1);
			}*/
		}
	else
		m_lockObjects.Unlock();		// Unlock
	}

void
TournamentTableController::OnConnectionDropped(PokerSession* pSess, ClientConnection* pConn, int nSeat){
	SetSeatOffline(nSeat, false);
	}

void
TournamentTableController::OnGameSessionExpired(PokerSession* pSess, ClientConnection* pConn,  bool& bCancel){
	if( !pConn || !pSess ) return; 
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatBySession(pSess);
	if( pSeat )
		bCancel = true;
	m_lockObjects.Unlock(); // Unlock
	}

bool
TournamentTableController::OnGameSessionRestored(PokerSession* pSess, ClientConnection* pConn, int nUserId){
	return true;
	}

void
TournamentTableController::HandStarting(){
	if( m_bChangeStakes ){
		m_bChangeStakes = false;
		m_lockObjects.Lock();	// Lock
		m_nSBAmount					= m_nSBAmountNew;
		m_nBBAmount					= m_nBBAmountNew;
		m_nAnteAmount				= m_nAnteAmountNew;
		m_info.m_nAnteAmount		= m_nAnteAmount;
		m_info.m_nBigBlindAmount	= m_nBBAmount;
		m_info.m_nSmallBlindAmount	= m_nSBAmount;
		m_lockObjects.Unlock(); // Unlock

		// Send level changed packet.
		PokerLevelChangedPacket packet;
		packet.m_nTournamentId		= m_info.m_nTournamentId;
		packet.m_nTableId			= m_info.m_nTableId;
		packet.m_cLevelIndex		= (char)m_pTournamentMan->m_nLevelIndex;
		packet.m_shLevelSecLeft		= (short)m_pTournamentMan->m_nLevelSecondsLeft;
		packet.m_shNextBreakSecLeft	= (short)m_pTournamentMan->m_nNextBreakSecondsLeft;
		packet.m_bRebuyAllowed		= m_pTournamentMan->IsRebuyAllowed();
		packet.m_nSmallBlind		= m_nSBAmount;
		packet.m_nBigBlind			= m_nBBAmount;
		packet.m_nAnte				= m_nAnteAmount;
		packet.m_nSmallBlind_Next	= m_nSBAmountNext;
		packet.m_nBigBlind_Next		= m_nBBAmountNext;
		packet.m_nAnte_Next			= m_nAnteAmountNext;
		m_pServer->SendPacketByTable(m_info.m_nTableId, m_info.m_nTournamentId, &packet);

		// Sleep for a while.
		GThread_Sleep(1000);
		}
	}

void
TournamentTableController::HandStarted(){
	m_pTournamentMan->OnHandStarted(this);
	}

void
TournamentTableController::HandFinished(bool& bTerminateGame, int nHandAvgDurationSec){
	m_pTournamentMan->OnHandFinished(this, bTerminateGame, nHandAvgDurationSec);
	}

bool
TournamentTableController::EmptySeat(int nSeat, bool bSendCallbacks /*= true*/){
	// In case when table belongs to tournamenent, we should empty seat at once.
	if( !m_info.m_bTournamentTable ) return false;

	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock();	// Unlock
		return false;
		}

	bool			bEmptySeat	= false;
	int				nTableId	= m_info.m_nTableId;
	PokerSeatStatus status		= pSeat->GetStatus();
	PokerSession*	pSess		= (PokerSession*)pSeat->GetSession(true);
	CString			sUserName	= pSeat->GetUserName();
	int				nUserId		= pSeat->GetUserId();
	
	if( bSendCallbacks ){
		// Call seat left callback.
		m_pGameBase->PlayerLeftSeat(this, m_pParam, &m_info, nSeat, pSeat);
		// Call tournament manager callback.
		m_pTournamentMan->OnSeatEmpty(this, nSeat, nTableId, nUserId);
		}

	// Empty seat.
	m_info.EmptySeat(nSeat);
	m_lockObjects.Unlock();	// Unlock

	// Send SetSeatStatus packet. {{
	PokerSeatStatusChangedPacket* p	= new PokerSeatStatusChangedPacket();
	p->m_nTableId				= nTableId;
	p->m_nTournamentId			= m_info.m_nTournamentId;
	p->m_cSeat					= (char)nSeat;
	p->m_sUserName				= sUserName;
	p->m_status					= (char)PokerSeatStatus::Empty;
	m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, p);
	delete p;
	// }}

	if( pSess ) SafePointer::Release(pSess); // Release session.
	return true;
	}

void
TournamentTableController::OnGThread_Starting(){
	PokerCashTableController::OnGThread_Starting();
	ASSERT(m_pTournamentMan);
	m_gameThread.AddParam(_T("SYNC_START_EVENT"), m_pTournamentMan->GetSyncGameStartEvent());
	m_gameThread.AddParam(_T("SYNC_START_WAIT_MS"), &m_nSyncStartWaitMS);
	}

void
TournamentTableController::OnGThread_Finishing(bool& bTurnInPauseMode){
	bTurnInPauseMode = false;
	}

void
TournamentTableController::OnGThread_Terminated(){
	PokerCashTableController::OnGThread_Terminated();
	// Call game table closed callback.
	m_pTournamentMan->OnGameTableClosed(this);
	}

PokerSession*
TournamentTableController::GetSeatSession(int nSeat, bool bRetain){
	PokerSession* pSess = NULL;
	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo*	pSeat = m_info.GetSeatInfo(nSeat);
	if( pSeat )
		pSess = (PokerSession*)pSeat->GetSession(bRetain);
	m_lockObjects.Unlock(); // Unlock
	return pSess;
	}

void
TournamentTableController::SetBlindStakes(TournamentBlindStructure* pCurrLevelStakes, TournamentBlindStructure* pNextLevelStakes){
	ASSERT(pCurrLevelStakes );
	if( pNextLevelStakes ){
		m_nSBAmountNext		= (UINT)Serializable::ConvertMoney(pNextLevelStakes->m_dSmallBlind);
		m_nBBAmountNext		= (UINT)Serializable::ConvertMoney(pNextLevelStakes->m_dBigBlind);
		m_nAnteAmountNext	= (UINT)Serializable::ConvertMoney(pNextLevelStakes->m_dAnte);
		}
	else
		m_nSBAmountNext = m_nBBAmountNext = m_nAnteAmountNext = 0;

	if( GThread_IsStarted() ){
		m_nSBAmountNew		= (UINT)Serializable::ConvertMoney(pCurrLevelStakes->m_dSmallBlind);
		m_nBBAmountNew		= (UINT)Serializable::ConvertMoney(pCurrLevelStakes->m_dBigBlind);
		m_nAnteAmountNew	= (UINT)Serializable::ConvertMoney(pCurrLevelStakes->m_dAnte);
		m_bChangeStakes		= true;
		}
	else{
		m_lockObjects.Lock();	// Lock
		m_nSBAmount					= (UINT)Serializable::ConvertMoney(pCurrLevelStakes->m_dSmallBlind);
		m_nBBAmount					= (UINT)Serializable::ConvertMoney(pCurrLevelStakes->m_dBigBlind);
		m_nAnteAmount				= (UINT)Serializable::ConvertMoney(pCurrLevelStakes->m_dAnte);
		m_info.m_nAnteAmount		= m_nAnteAmount;
		m_info.m_nBigBlindAmount	= m_nBBAmount;
		m_info.m_nSmallBlindAmount	= m_nSBAmount;
		m_lockObjects.Unlock(); // Unlock
		}
	}

INT_CHIPS
TournamentTableController::GetAverageBalanceAmount(){
	INT_CHIPS nAvgBalance = 0;
	int nSeatCt = 0; 
	m_lockObjects.Lock(); // Lock
	for(int i=0; i<m_info.m_cMaxPlayersCt; i++){
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		if( pSeat ){
			nSeatCt		++;
			nAvgBalance += pSeat->GetBalance();
			}
		}
	m_lockObjects.Unlock(); // Unlock

	if( nSeatCt > 0 )
		nAvgBalance /= nSeatCt;
	return nAvgBalance;
	}

int
TournamentTableController::GetSeatsAvailability(bool* pSeats, INT_CHIPS* nArrBalances){
	int nSeatCt	= 0; 
	m_lockObjects.Lock();	// Lock
	for(int i=0; i<m_info.m_cMaxPlayersCt; i++){
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		if( pSeat ){
			pSeats[i]		= true;
			nArrBalances[i] = pSeat->GetBalance();
			nSeatCt			++;
			}
		else{
			pSeats[i]		= false;
			nArrBalances[i]	= 0;
			}
		}
	m_lockObjects.Unlock(); // Unlock
	return nSeatCt;			// Returns available seats count.
	}

int
TournamentTableController::GetEmptySeatCt(){
	int nSeatCt	= 0; 
	m_lockObjects.Lock();	// Lock
	for(int i=0; i<m_info.m_cMaxPlayersCt; i++){
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty )
			nSeatCt			++;
		}
	m_lockObjects.Unlock(); // Unlock
	return nSeatCt;
	}

bool
TournamentTableController::SeatFromAnotherTable(int nSeatDest, TournamentTableController* pTableSrc, int nSeatSrc){
	if( !pTableSrc || nSeatSrc < 0 || nSeatDest < 0 ) return false;

	m_lockObjects.Lock();		// Lock
	PokerTableSeatInfo* pSeatDest = m_info.GetSeatInfo(nSeatDest);
	if( pSeatDest != NULL && pSeatDest->GetStatus() != PokerSeatStatus::Empty ){
		m_lockObjects.Unlock(); // Unlock
		return false;
		}

	PokerTableSeatInfo* pSeat = new PokerTableSeatInfo();
	if( !pTableSrc->GetSeatInfo(nSeatSrc, pSeat) ){
		m_lockObjects.Unlock(); // Unlock
		delete pSeat;
		return false;
		}

	PokerSession*	pSess		= pTableSrc->GetSeatSession(nSeatSrc, true);
	PokerAvatar*	pAvatar		= NULL;
	pTableSrc->EmptySeat		(nSeatSrc, false);
	pSeat->SetStatus			(PokerSeatStatus::CheckForPositionToStart);
	pSeat->SetSeat				(nSeatDest);
	pSeat->SetLastAction		(PokerActionTypes::NoAction);
	pSeat->SetCardsShowStatus	(false);
	pSeat->SetHandCardsClass	(PokerHandCardsClass::CardsClass_None);
	SetSeatInfo					(nSeatDest, pSeat);
	
	// Seat at packet.
	PokerSeatAtTablePacket	packet1;
	packet1.m_nTableId			= m_info.m_nTableId;
	packet1.m_nTournamentId		= m_info.m_nTournamentId;
	packet1.m_cSeatIndex		= (char)nSeatDest;
	packet1.m_nChipAmount		= pSeat->GetBalance();
	packet1.m_nSeatSessionId	= pSess->GetSessionId();
	pSeat->Copy(&packet1.m_seatInfo);

	int nUserId					= pSeat->GetUserId();
	
	// Set avatar info.
	if( pSess ){
		UserInfo* pUser = pSess->GetUserInfo();
		if( pUser ){
			packet1.m_sAvatarFileName = pUser->m_sAvatarFileName;
			m_info.SetAvatarInfo(nSeatDest, pUser->m_pAvatarInfo);
			}
		}

	delete pSeat;
	m_lockObjects.Unlock(); // Unlock

	// Call user reseated notify method.
	m_pTournamentMan->OnUserReseated(this, packet1.m_nTableId, nSeatDest, pTableSrc, pTableSrc->GetTableId(), nSeatSrc, nUserId);
	// Send seat at packet.
	m_pServer->SendPacketByTable(m_info.m_nTableId, m_info.m_nTournamentId, &packet1, false);

	// Send redirect packet.
	if( pSess ){
		// Unseat session from old table.
		pSess->TableCloseAndUnseat(pTableSrc->GetTableId(), m_info.m_nTournamentId);
		// Seat session to a new table.
		pSess->SeatAt(m_info.m_nTableId, nSeatDest, m_info.m_nTournamentId);

		// Redirect client connection to another table.
		//ClientConnection* pConn = pSess->GetConnection(true, true);
		ClientConnection* pTableConn		= pSess->GetTableConnection(pTableSrc->GetTableId(), m_info.m_nTournamentId, true, true);
		ClientConnection* pTableConnClose	= pSess->GetTableConnection(m_info.m_nTableId, m_info.m_nTournamentId, true, true);

		if( pTableConnClose ){
			// Remove table connection.
			m_pServer->RemoveTableConnection(m_info.m_nTableId, m_info.m_nTournamentId, pTableConnClose, this);
			SafePointer::Release(pTableConnClose);
			}

		if( pTableConn ){
			// Remove table connection.
			//m_pServer->RemoveTableConnection(m_info.m_nTableId, m_info.m_nTournamentId, pTableConn, this);

			PokerRedirectPacket packet;
			packet.m_nTableId				= pTableSrc->GetTableId();
			packet.m_nTournamentId			= pTableSrc->GetTournamentId();
			packet.m_nTableId_Redirect		= m_info.m_nTableId;
			packet.m_nTournamentId_Redirect = m_info.m_nTournamentId;
			m_pServer->SendPacket(pTableConn, &packet);
			SafePointer::Release(pTableConn);
			}
		SafePointer::Release(pSess);
		}
	return true;
	}

bool
TournamentTableController::SeatSessionAtTable(PokerTableSeatInfo* pSeat, PokerSession* pSession,  int nSeatDest){
	if( !pSeat || !pSession || nSeatDest < 0 )
		return false;

	m_lockObjects.Lock();		// Lock
	if( m_info.GetSeatStatus(nSeatDest) != PokerSeatStatus::Empty ){
		m_lockObjects.Unlock(); // Unlock
		return false;
		}

	PokerAvatar*	pAvatar			= NULL;
	if( pSeat->GetSession(false) != pSession )
		pSeat->SetSession(pSession, m_info.m_nTableId, m_info.m_nTournamentId);

	pSeat->SetStatus			(PokerSeatStatus::CheckForPositionToStart);
	pSeat->SetSeat				(nSeatDest);
	pSeat->SetLastAction		(PokerActionTypes::NoAction);
	pSeat->SetCardsShowStatus	(false);
	pSeat->SetHandCardsClass	(PokerHandCardsClass::CardsClass_None);
	SetSeatInfo					(nSeatDest, pSeat);
	
	// Seat at packet.
	PokerSeatAtTablePacket	packet1;
	packet1.m_nTableId			= m_info.m_nTableId;
	packet1.m_nTournamentId		= m_info.m_nTournamentId;
	packet1.m_cSeatIndex		= (char)nSeatDest;
	packet1.m_nChipAmount		= pSeat->GetBalance();
	packet1.m_nSeatSessionId	= pSession->GetSessionId();
	pSeat->Copy(&packet1.m_seatInfo);

	int nUserId					= pSeat->GetUserId();
	// Set avatar info.
	if( pSession ){
		UserInfo* pUser = pSession->GetUserInfo();
		if( pUser ){
			packet1.m_sAvatarFileName = pUser->m_sAvatarFileName;
			m_info.SetAvatarInfo(nSeatDest, pUser->m_pAvatarInfo);
			}
		}

	delete pSeat;
	m_lockObjects.Unlock(); // Unlock

	// Call seat taken notify method.
	m_pTournamentMan->OnSeatTaken(this, nSeatDest, packet1.m_nTableId, nUserId);
	// Send seat at packet.
	m_pServer->SendPacketByTable(m_info.m_nTableId, m_info.m_nTournamentId, &packet1, false);

	// Send open table packet.
	if( pSession ){
		// Unseat session from old table.
		pSession->TableCloseAndUnseat(packet1.m_nTableId, m_info.m_nTournamentId);
		// Seat session to the new table.
		pSession->SeatAt(packet1.m_nTableId, nSeatDest, m_info.m_nTournamentId);

		// Retrieve stake amounts.
		UINT nSB_Next = 0, nBB_Next = 0, nAnte_Next = 0;
		m_pTournamentMan->GetLevelStakes(m_pTournamentMan->GetCurrentLevelIndex() + 1, nSB_Next, nBB_Next, nAnte_Next);

		// Send open tournament table packet.
		SendOpenTournamentTableToSeats(nSeatDest, nSB_Next, nBB_Next, nAnte_Next, 0);
		}
	return true;
	}

int
TournamentTableController::EmptyAllSeats(){
	int nRet = 0;
	for(int i=0; i<m_info.m_cMaxPlayersCt; i++){
		if( EmptySeat(i, true) )
			nRet ++;
		}
	return nRet;
	}

int
TournamentTableController::PerformAutoRebuySeats(){
	if( !m_pTournamentMan || !IsRebuyAllowed() )
		return 0;
	int nRet = 0, nMaxSeatCt = m_info.m_cMaxPlayersCt, nStartingChips = (m_pTournamentMan->GetStartingChips()*100), nRebuyChipsAmount = m_pTournamentMan->GetAddonChipsCt()*100;
	for(int i=0; i<nMaxSeatCt; i++){
		m_lockObjects.Lock();	// Lock
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		bool bPerform = (pSeat && pSeat->GetStatus() != PokerSeatStatus::Empty && pSeat->GetAutoRebuy() && pSeat->GetBalance() == 0);
		/* Old one
		bool bPerform = (pSeat && pSeat->GetStatus() != PokerSeatStatus::Empty && pSeat->GetAutoRebuy() && pSeat->GetBalance() < nStartingChips);*/
		m_lockObjects.Unlock(); // Unlock

		if( bPerform ){
			INT_CHIPS nBalanceNew = AddChips(i, nRebuyChipsAmount, PokerTableController::AddChipsType::AddChips_Rebuy);
			if( nBalanceNew > 0 ){
				// Addd rebuy chips.
				SendSeatActionPacket(i, PokerActionTypes::AddChips, (PokerSeatStatus)-1, nBalanceNew);
				nRet ++;
				}
			}
		}
	return nRet;
	}

int
TournamentTableController::PerformAutoAddonSeats(){
	if( !m_pTournamentMan && !IsAddonAllowed() )
		return 0;
	int nRet = 0, nMaxSeatCt = m_info.m_cMaxPlayersCt, nAddonChipsAmount = m_pTournamentMan->GetAddonChipsCt()*100;
	for(int i=0; i<nMaxSeatCt; i++){
		m_lockObjects.Lock();	// Lock
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		bool bPerform = (pSeat && pSeat->GetStatus() != PokerSeatStatus::Empty && pSeat->GetAutoAddon());
		m_lockObjects.Unlock(); // Unlock

		if( bPerform ){
			INT_CHIPS nBalanceNew = AddChips(i, nAddonChipsAmount, PokerTableController::AddChipsType::AddChips_Addon);
			if( nBalanceNew > 0 ){
				// Addd rebuy chips.
				SendSeatActionPacket(i, PokerActionTypes::AddChips, (PokerSeatStatus)-1, nBalanceNew);
				nRet ++;
				}
			}
		}
	return nRet;
	}

int
TournamentTableController::GetTournamentPlayersCount(){
	if( !m_pTournamentMan )
		return 0;
	return m_pTournamentMan->GetPlayersCount();
	}

int
TournamentTableController::GetTournamentTablesCount(){
	if( !m_pTournamentMan )
		return 0;
	return m_pTournamentMan->GetTablesCount();
	}

bool
TournamentTableController::IsRebuyAllowed(){
	if( !m_pTournamentMan )
		return false;
	return m_pTournamentMan->IsRebuyAllowed();
	}

bool
TournamentTableController::IsAddonAllowed(){
	if( !m_pTournamentMan )
		return false;
	return m_pTournamentMan->IsAddonAllowed();
	}

int
TournamentTableController::SendOpenTournamentTableToSeats(int nTargetSeat, UINT nSB_Next, UINT nBB_Next, UINT nAnte_Next, int nStartTournamentTimeSec){
	PokerOpenTable_TournamentPacket packet;
	packet.ZeroInit();

	packet.m_nCode					= PacketCodes::_PokerOpenTable_Tournament;
	packet.m_nTableId				= m_info.m_nTableId;
	packet.m_cSeat					= 0;
	packet.m_nTournamentId			= m_info.m_nTournamentId;
	packet.m_btTableWaitingState	= PokerTableWaitingState::WaitingForStartHandToStartTournament;
	packet.m_btWaitSec				= (BYTE)nStartTournamentTimeSec;
	packet.m_shLevelSecLeft			= m_pTournamentMan->GetLevelDurationSec(); // Level countdown starts when hand starts.
	packet.m_shBreakSecLeft			= m_pTournamentMan->GetBreakPerLevel()*packet.m_shLevelSecLeft; // Break starting countdown.
	packet.m_bRebuyAllowed			= m_pTournamentMan->IsRebuyAllowed();
	packet.m_btRebuyCtLeft			= (BYTE)m_pTournamentMan->GetRebuyCtMax();
	packet.m_bAddonAllowed			= false;
	packet.m_btAddonCtLeft			= (BYTE)m_pTournamentMan->GetAddonCtMax();

	packet.m_nRebuyChipsCt			= m_pTournamentMan->GetRebuyChipsCt()*100;
	packet.m_nAddonChipsCt			= m_pTournamentMan->GetAddonChipsCt()*100;
	packet.m_nRebuyAmount			= m_pTournamentMan->GetRebuyAmount();
	packet.m_nRebuyVPPAmount		= m_pTournamentMan->GetRebuyVPPAmount();
	packet.m_nAddonAmount			= m_pTournamentMan->GetAddonAmount();
	packet.m_nAddonVPPAmount		= m_pTournamentMan->GetAddonVPPAmount();

	packet.m_nRankIndex				= -1;
	packet.m_shTablesCt				= (short)m_pTournamentMan->GetTablesCount();
	packet.m_nPlayersCt				= m_pTournamentMan->GetPlayersCount();

	packet.m_nSmallBlind_Next		= nSB_Next;
	packet.m_nBigBlind_Next			= nBB_Next;
	packet.m_nAnte_Next				= nAnte_Next;

	PokerTableInfo* pTableInfo		= new PokerTableInfo();
	m_info.Copy(pTableInfo);
	packet.m_info.Add(pTableInfo);
	
	int	nResult = 0;
	if( nTargetSeat == -1 ){
		for(int i=0; i<(int)m_info.m_cMaxPlayersCt; i++){
			PokerSession*	pSess	= GetSeatSession(i, true);
			if( pSess ){
				ClientConnection* pConn = pSess->GetConnection(true, true);
				if( pConn ){
					bool	bOpened		= false;
					int		nSeatIndex	= pSess->GetSeatIndex(packet.m_nTableId, bOpened, packet.m_nTournamentId);
					if( !bOpened ){
						// Add table connection.
						m_pServer->AddTableConnection(packet.m_nTableId, packet.m_nTournamentId, pConn, this);
						// Attach connection to seat.
						pSess->SeatAt(packet.m_nTableId, i, packet.m_nTournamentId);
						/*
						// Set seat online status. {{
						PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
						if( pSeat && pSeat->IsOffline() )
							pSeat->SetOffline(false);
						// }}
						*/
						}

					packet.m_cSeat		= (char)i;
					m_pServer->SendPacket(pConn, &packet);
					SafePointer::Release(pConn);
					nResult ++;
					}
				SafePointer::Release(pSess);
				}
			}
		}
	else{
		int				i		= nTargetSeat;
		PokerSession*	pSess	= GetSeatSession(i, true);
		if( pSess ){
			ClientConnection* pConn = pSess->GetConnection(true, true);
			if( pConn ){
				bool	bOpened		= false;
				int		nSeatIndex	= pSess->GetSeatIndex(packet.m_nTableId, bOpened, packet.m_nTournamentId);
				if( !bOpened ){
					// Add table connection.
					m_pServer->AddTableConnection(packet.m_nTableId, packet.m_nTournamentId, pConn, this);
					// Attach connection to seat.
					pSess->SeatAt(packet.m_nTableId, i, packet.m_nTournamentId);
					/*
					// Set seat online status. {{
					PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
					if( pSeat && pSeat->IsOffline() )
						pSeat->SetOffline(false);
					// }}
					*/
					}

				packet.m_cSeat		= (char)i;
				m_pServer->SendPacket(pConn, &packet);
				SafePointer::Release(pConn);
				nResult ++;
				}
			SafePointer::Release(pSess);
			}
		}
	return nResult;
	}