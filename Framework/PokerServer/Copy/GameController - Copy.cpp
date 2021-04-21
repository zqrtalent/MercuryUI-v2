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

PokerGameController::PokerGameController(PokerServer* pServer /*= NULL*/){
	if( !pServer ){ m_pServer = NULL; return; }

	// Create instance.
	HoldemWinnerCalculator::GetInstance();

	m_pServer					= pServer;
	m_hTimerThread				= 0L;
	m_dwTimerThreadId			= 0L;

	m_nHandStartDelay			= 3;
	m_nPostBlindDelay			= 5;
	m_nStartRoundDelay			= 3;
	m_nRegularTimeSec			= 10;
	m_nExtraTimeSec				= 3;
	m_nShowMuckDelay			= 5;
	m_nReserveSeatDelay			= 5;
	m_nLeaveSitoutedDelay		= 30;
	m_nTimeBankSec				= 60;
	m_nTimeBankInc				= 0;
	m_nTimeBankIncHands			= 0;

	m_nSBAmount					= 0;
	m_nBBAmount					= 0;
	m_nAnteAmount				= 0;

	m_nMinBuyIn					= 0;
	m_nMaxBuyIn					= 0;

	m_nDealer					= -1;
	m_nSmallBlind				= -1;
	m_nBigBlind					= -1;

	m_bHandStartTimerActive		= false;

	m_nHandId					= -1;
	m_hStartHandThread			= NULL;
	m_dwStartHandThreadId		= 0L;
	m_eventStopHandStartThread	= NULL;

	m_ThreadTimerInfo.pServer			= pServer;
	m_ThreadTimerInfo.pConn				= NULL;
	m_ThreadTimerInfo.pPacketSend		= NULL;
	m_ThreadTimerInfo.nDelaySec			= 0;
	m_ThreadTimerInfo.procElapsed		= NULL;
	m_ThreadTimerInfo.pEventCancel		= new CEvent(FALSE, FALSE);
	m_ThreadTimerInfo.pEventStop		= new CEvent(FALSE, FALSE);
	m_ThreadTimerInfo.pEventResume		= new CEvent(FALSE, FALSE);
	m_ThreadTimerInfo.pEventCanceled	= new CEvent(FALSE, FALSE);
	m_ThreadTimerInfo.bIsWorking		= false;
	m_ThreadTimerInfo.bContinue			= false;
	m_ThreadTimerInfo.pSignal			= NULL;
	m_ThreadTimerInfo.pElapsed			= NULL;

	// Start timer thread.
	StartTimerThread();

	// Not destroy objects when remove element from array.
	m_listWaitingList.DestroyObjects(false);

	memset(m_arrSeatReserveTimerIds, 0, sizeof(m_arrSeatReserveTimerIds));
	memset(m_arrLeaveSeatTimerIds, 0, sizeof(m_arrLeaveSeatTimerIds));
	} 

BEGIN_DECLARE_VAR(PokerGameController, Serializable)
	DECLARE_VAR(_T("PokerGameController"),	_T(""),		VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("TableInfo"),			_T(""),		VariableType::None, offsetof(PokerGameController,m_info), false, true, false)
END_DECLARE_VAR()

PokerGameController::~PokerGameController(){
	if( !m_pServer ) return;

	// Stop hand start thread.
	StopHandThread();

	// Kill all timers.
	KillAllTimers();

	// Stop timer thread.
	StopTimerThread();

	// Destroy waiting list.
	m_listWaitingList.RemoveAll(false);

	// Delete event objects.
	delete m_ThreadTimerInfo.pEventCancel;
	delete m_ThreadTimerInfo.pEventStop;
	delete m_ThreadTimerInfo.pEventResume;
	delete m_ThreadTimerInfo.pEventCanceled;
	}

void
PokerGameController::OnOpenTable(ClientConnection* pConn, PokerOpenTablePacket* p){
	ASSERT( pConn );
	ASSERT(p->m_nTableId == m_info.m_nTableId);

	Session* pSess	= pConn->GetSession();
	ASSERT( pSess );
	UserInfo* pUser = (UserInfo*)pSess->Get(_T("User"));
	if( !pUser ) return;

	m_lockObjects.Lock();	// Lock
	int nSeatEmpty	= m_info.GetFirstEmptySeat();
	p->m_nHandId	= m_nHandId;
	p->m_info.RemoveAll				();
	p->m_info.Add					(&m_info);

	if( IsTimerWorking() ){
		m_lockTimerObjects.Lock();		// Lock
		p->m_shActiveSeatTimeFull	= m_ThreadTimerInfo.nDelaySec;
		p->m_shActiveSeatTimePassed = m_ThreadTimerInfo.nDelaySecPassed;
		m_lockTimerObjects.Unlock();	// Unlock
		}
	else{
		p->m_shActiveSeatTimeFull	= 0;
		p->m_shActiveSeatTimePassed = 0;
		}

	m_pServer->SendPacketSheduled	(pConn, p);
	p->m_info.RemoveAll				(false);
	m_lockObjects.Unlock(); // Unlock

	if( p->m_bSeatOrJoinWaitList ){
		// Reserve seat.
		if( nSeatEmpty != -1 ){
			PokerReserveSeatPacket* packet	= new PokerReserveSeatPacket();
			packet->m_nSessionId			= p->m_nSessionId;
			packet->m_nTableId				= p->m_nTableId;
			packet->m_bIsRequest			= true;
			packet->m_cSeat					= (char)nSeatEmpty;
			packet->m_sUserName				= pUser->m_sUserName;
			OnReserveSeat(pConn, packet);
			delete packet;
			}
		else{ // join waiting list.
			PokerJoinWaitingListPacket* packet = new PokerJoinWaitingListPacket;
			packet->m_bIsRequest			= true;
			packet->m_nSessionId			= p->m_nSessionId;
			packet->m_nTableId				= p->m_nTableId;
			packet->m_bIsRequest			= true;
			OnJoinWaitingList(pConn, packet);
			delete packet;
			}
		}
	}

void
PokerGameController::OnLeaveTable(ClientConnection* pConn, PokerLeaveTablePacket* p){
	m_lockObjects.Lock();	// Lock
	// If seat is taken by connection. {{
	int nSeatIndex = pConn->GetSeatIndex(m_info.m_nTableId);
	if( nSeatIndex != -1 ){
		EmptySeat(nSeatIndex);
		}
	// }}
	p->m_cSeat = (char)nSeatIndex;
	m_lockObjects.Unlock(); // Unlock

	// Send reply.
	m_pServer->SendPacketSheduled(pConn, p);
	}

void
PokerGameController::OnSeatAt(ClientConnection* pConn, PokerSeatAtTablePacket* p){
	ASSERT( pConn );
	int nSeatIndex = (int)p->m_cSeatIndex;
	if( !p || (nSeatIndex < 0 || nSeatIndex >= m_info.m_nMaxPlayersCt) )
		return;

	// Check for user.
	// Check for amount.
	// Check seat status if already taken seat.

	Session* pSess	= pConn->GetSession();
	ASSERT( pSess );
	UserInfo* pUser = (UserInfo*)pSess->Get(_T("User"));
	if( !pUser )	return;

	PokerAvatar*		pAvatarInfo = pUser->m_pAvatarInfo;
	m_lockObjects.Lock(); // Lock
	bool				bReserved	= IsSeatReservedByConn(nSeatIndex, pConn);
	bool				bSuccess	= false;
	PokerTableSeatInfo* pSeatInfo	= m_info.GetSeatInfo(nSeatIndex);
	PokerSeatStatus		status		= pSeatInfo ? pSeatInfo->GetStatus() : PokerSeatStatus::Empty;

	if( !pSeatInfo || (pSeatInfo && (status == PokerSeatStatus::Empty || (status == PokerSeatStatus::Reserved && bReserved))) ){
		if( !pSeatInfo ){
			pSeatInfo = new PokerTableSeatInfo();
			}

		// Kill seat reservation timer. {{
		if( m_arrSeatReserveTimerIds[nSeatIndex] ){
			Timers::GetInstance()->KillTimer(m_arrSeatReserveTimerIds[nSeatIndex]);
			m_arrSeatReserveTimerIds[nSeatIndex] = 0;
			}
		// }}

		pSeatInfo->SetSeat				(nSeatIndex);
		pSeatInfo->SetBalance			(p->m_nChipAmount);
		pSeatInfo->SetCards				( -1, -1, -1, -1);
		pSeatInfo->SetRankLevel			(pUser->m_shRankLevel);
//		pSeatInfo->SetRankName			(_T("Bronze"));
		pSeatInfo->SetUserId			(pUser->m_nUserId);
		pSeatInfo->SetUserName			(pUser->m_sUserName);
		pSeatInfo->SetActionName		(_T(""));

		if( m_info.m_gameStatus == PokerGameStatus::NotStarted )
			pSeatInfo->SetStatus(PokerSeatStatus::Sitted);
		else
			pSeatInfo->SetStatus(PokerSeatStatus::WaitingForBB);

		pSeatInfo->SetTimeBankSeconds	(m_nTimeBankSec);
		pSeatInfo->SetSession			(pConn->GetSession());

		// Set avatar info.
		m_info.SetAvatarInfo(nSeatIndex, pAvatarInfo);

		// Set seat info.
		bSuccess = m_info.SetSeatInfo(nSeatIndex, pSeatInfo);
		if( bSuccess ){
			// Copy seat info.
			pSeatInfo->Copy(&p->m_seatInfo);	

			// Set user's session id.
			p->m_nSeatSessionId = pSess->GetSessionId();

			// Player seated event.
			m_pServer->PlayerSeated(m_info.m_nTableId, nSeatIndex, p->m_nChipAmount, pSeatInfo);
			}
		}
	m_lockObjects.Unlock(); // Unlock

	if( bSuccess ){
		ASSERT(m_pServer);
		
		if( pAvatarInfo ){
			p->m_avatarData.m_nId						= pAvatarInfo->m_nId;
			p->m_avatarData.m_recStatus					= 0;
			p->m_avatarData.m_binData.m_dwDataLength	= pAvatarInfo->m_binData.m_dwDataLength;
			p->m_avatarData.m_binData.m_hData			= pAvatarInfo->m_binData.m_hData;
			}
		else{
			p->m_avatarData.m_nId						= 0;
			p->m_avatarData.m_recStatus					= 0;
			p->m_avatarData.m_binData.m_dwDataLength	= 0L;
			p->m_avatarData.m_binData.m_hData			= NULL;
			}

		// Send seat at packet to everyone.
		m_pServer->SendPacketByTable(m_info.m_nTableId, p);
		// Zero avatar binary data.
		p->m_avatarData.m_binData.m_dwDataLength	= 0L;
		p->m_avatarData.m_binData.m_hData			= NULL;

		// Start hand start timer if game is not active. {{
		m_lockObjects.Lock();		// Lock
		if( m_info.m_gameStatus == PokerGameStatus::NotStarted && m_info.GetSittedCt() >= 2 && !m_bHandStartTimerActive ){
			m_info.ClearSeatCards();
			m_lockObjects.Unlock(); // Unlock
			
			// Activate hand start timer. {{
			OnTimerElapsed method	= (OnTimerElapsed)&PokerGameController::OnHandStartTimerElapsed;
			InitializeTimer(NULL, NULL, m_nHandStartDelay, method);
			m_bHandStartTimerActive = true;
			// }}
			}
		else
			m_lockObjects.Unlock(); // Unlock
		// }}
		}
	}

void
PokerGameController::OnReserveSeat(ClientConnection* pConn, PokerReserveSeatPacket* p){
	ASSERT( pConn );
	Session* pSess	= pConn->GetSession();
	ASSERT( pSess );
	UserInfo* pUser = (UserInfo*)pSess->Get(_T("User"));
	if( !pUser ) return;
	
	m_lockObjects.Lock(); // Lock
	int nSeat = (int)p->m_cSeat;
	PokerTableSeatInfo* pInfo = m_info.GetSeatInfo(nSeat);
	if( pInfo && pInfo->GetStatus() != PokerSeatStatus::Empty ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	pInfo = pInfo ? pInfo : new PokerTableSeatInfo();
	pInfo->SetSeat				(nSeat);
	pInfo->SetBalance			(0);
	pInfo->SetCards				(-1, -1, -1, -1);
	pInfo->SetUserName			(pUser->m_sUserName);
	pInfo->SetActionName		(_T(""));
	pInfo->SetStatus			(PokerSeatStatus::Reserved);
	pInfo->SetTimeBankSeconds	(m_nTimeBankSec);
	pInfo->SetSession			(pConn->GetSession());
	m_info.SetSeatInfo			(nSeat, pInfo);
	m_lockObjects.Unlock(); // Unlock

	// Send response to seat reserver. {{
	p->m_sUserName				= pUser->m_sUserName;
	p->m_shReservationTimeSec	= (short)m_nReserveSeatDelay;
	p->m_bReservationExpired	= false;
	m_pServer->SendPacketSheduled(pConn, p);
	// }}

	// Send seat seat status packet. {{
	PokerSetSeatStatusPacket* p1 = new PokerSetSeatStatusPacket();
	p1->m_nTableId	= p->m_nTableId;
	p1->m_cSeat		= (char)nSeat;
	p1->m_sUserName = pUser->m_sUserName;
	p1->m_status	= (char)PokerSeatStatus::Reserved;
	m_pServer->SendPacketByTable(p1->m_nTableId, p1, false);
	delete p1;
	// }}

	m_lockObjects.Lock();	// Lock
	// Start reservation timer.
	StartSeatReservationTimer(nSeat);
	m_lockObjects.Unlock(); // Unlock
	}

void
PokerGameController::OnCancelReservation(ClientConnection* pConn, PokerCancelReservationPacket* p){
	m_lockObjects.Lock();		// Lock
	int nSeat		= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nSeat);
	if( !pSeatInfo || pSeatInfo->GetStatus() != PokerSeatStatus::Reserved ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	// Empty seat.
	m_info.EmptySeat(nSeat);
	m_lockObjects.Unlock(); // Unlock

	// Send seat status change packet to everyone. {{
	PokerSetSeatStatusPacket* pSeatStatus = new PokerSetSeatStatusPacket();
	pSeatStatus->m_nTableId		= p->m_nTableId;
	pSeatStatus->m_cSeat		= (char)nSeat;
	pSeatStatus->m_status		= (char)PokerSeatStatus::Empty;
	m_pServer->SendPacketByTable(p->m_nTableId, pSeatStatus);
	delete pSeatStatus;
	// }}
	}

void
PokerGameController::OnJoinWaitingList(ClientConnection* pConn, PokerJoinWaitingListPacket* p){
	Session* pSess	= pConn->GetSession();
	ASSERT( pSess );
	UserInfo* pUser = (UserInfo*)pSess->Get(_T("User"));
	if( !pUser ) return;

	m_lockObjects.Lock();	// Lock
	int nSeatEmpty = m_info.GetFirstEmptySeat();
	if( nSeatEmpty != -1 ){
		PokerReserveSeatPacket* packet	= new PokerReserveSeatPacket();
		packet->m_nSessionId			= p->m_nSessionId;
		packet->m_nTableId				= p->m_nTableId;
		packet->m_bIsRequest			= true;
		packet->m_cSeat					= (char)nSeatEmpty;
		packet->m_sUserName				= pUser->m_sUserName;
		m_lockObjects.Unlock(); // Unlock

		OnReserveSeat(pConn, packet);
		delete packet;
		return;
		}
	else{
		// Wait index number.
		p->m_shIndex = AddWaitList(pConn);
		}
	m_lockObjects.Unlock(); // Unlock

	// Send reply packet.
	m_pServer->SendPacketSheduled(pConn, p);
	}

void
PokerGameController::OnAddChips(ClientConnection* pConn, PokerAddChipsPacket* p){
	}

void
PokerGameController::OnAction(ClientConnection* pConn, PokerActionPacket* p, bool bPreAction /*= false*/){
	int				nTableId			= m_info.m_nTableId;
	CEvent*			pSignal				= NULL;
	bool*			pElapsed			= NULL;
	BasePacket**	ppPacketReceived	= NULL;
	int				nSeat				= (int)p->m_cSeat;

	if( !bPreAction ){
		m_lockTimerObjects.Lock(); // Lock
		// Accept action packet from destination client {{
		if( pConn != m_ThreadTimerInfo.pConn ){
			m_lockTimerObjects.Unlock(); // Unlock
			return;
			}
		// }}

		// Validate received action and it's parameters. {{
		bool bBreak = false;
		PokerActPacket* pPacketSent = (PokerActPacket*)m_ThreadTimerInfo.pPacketSend;
		if( pPacketSent ) {
			if( pPacketSent->m_nCode == PacketCodes::_PokerAct ){
				for( int i=0; i<pPacketSent->m_actions.GetCount(); i++){
					PokerAction* pAction = pPacketSent->m_actions.GetAt(i);
					if( pAction && pAction->m_cActionType == p->m_cActionType ){
						if( p->m_nAmount < pAction->m_nAmountMin && 
							p->m_nAmount > pAction->m_nAmountMax ){ // Invalid amount value received from client.
							bBreak = true;
							}
						break;
						}
					}
				}
			// }}

			// Signal event params. {{
			pSignal				= m_ThreadTimerInfo.pSignal;
			pElapsed			= m_ThreadTimerInfo.pElapsed;
			ppPacketReceived	= m_ThreadTimerInfo.packetReceived;
			// }}
			}

		m_lockTimerObjects.Unlock(); // Unlock
		if( bBreak ) return;
		}
	else{
		}

	int nSecondsElapsed = 0;
	CancelTimer(&nSecondsElapsed); // Cancel timer.
	// Set time bank seconds. {{
	if( nSecondsElapsed > (m_nRegularTimeSec + m_nExtraTimeSec) ){
		int nTimeBankLeft = m_info.GetTimeBankSeconds(nSeat) - (nSecondsElapsed - (m_nRegularTimeSec + m_nExtraTimeSec));
		m_info.SetTimeBankSeconds(nSeat, nTimeBankLeft);
		}
	// }}

	int		nAmount		= p->m_nAmount; // Amount value is validated.
	bool	bAskNext	= false;
	switch( p->m_cActionType ){
		case PokerActionTypes::PostBB:{
			nAmount			= m_nBBAmount;
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
			PostSeatActionPacket(nSeat, (PokerActionTypes)p->m_cActionType, PokerSeatStatus::Sitted, nAmount);
			// }}
			break;
			}
		case PokerActionTypes::PostSB:{
			nAmount			= m_nSBAmount;
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
			PostSeatActionPacket(nSeat, (PokerActionTypes)p->m_cActionType, PokerSeatStatus::Sitted, nAmount);
			break;
			}
		case PokerActionTypes::SitOut:{
			m_lockObjects.Lock();	// Lock
			if( m_info.m_gameStatus == PokerGameStatus::Starting ){
				m_info.m_gameStatus = PokerGameStatus::NotStarted;
				}
			m_lockObjects.Unlock();	// Unlock
			SitOut(nSeat, pConn);	// Seat out.
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

			PostSeatActionPacket(nSeat, PokerActionTypes::Call, PokerSeatStatus::InPlay, nAmount);
			bAskNext = true;
			break;
			}
		case PokerActionTypes::Bet:
		case PokerActionTypes::Raise:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat) - nAmount;
			m_info.SetBalance	(nSeat, p->m_nBalance);
			m_info.AddPotAmount	(nSeat, nAmount);
			p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
			m_lockObjects.Unlock(); // Unlock
			// }}

			bool bAllIn	= (p->m_nBalance <= 0);
			if( bAllIn )
				PostSeatActionPacket(nSeat, PokerActionTypes::AllIn, PokerSeatStatus::All_In, nAmount);
			else
				PostSeatActionPacket(nSeat, (PokerActionTypes)p->m_cActionType, PokerSeatStatus::InPlay, nAmount);
			bAskNext = true;
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

			PostSeatActionPacket(nSeat, PokerActionTypes::AllIn, PokerSeatStatus::All_In, nAmount);
			bAskNext = true;
			break;
			}

		case PokerActionTypes::Check:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat);
			p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
			m_lockObjects.Unlock(); // Unlock
			// }}

			PostSeatActionPacket(nSeat, PokerActionTypes::Check, PokerSeatStatus::InPlay, 0);		// Act check
			bAskNext = true;
			break;
			}
	    case PokerActionTypes::Fold:{
			// Balance amount. {{
			m_lockObjects.Lock();	// Lock
			p->m_nBalance	= m_info.GetBalance(nSeat);
			p->m_nPotAmount	= m_info.GetPotAmount(nSeat);
			m_lockObjects.Unlock(); // Unlock
			// }}

			PostSeatActionPacket(nSeat, PokerActionTypes::Fold, PokerSeatStatus::Folded, 0);		// Act fold
			bAskNext = true;
			break;
			}
		};

	// Signal event. {{
	if( ppPacketReceived ){
		BasePacket* pReceived = new PokerActionPacket();
		p->Copy(pReceived);
		*ppPacketReceived = pReceived;
		}
	if( pSignal ){
		if( pElapsed )
			*pElapsed = false;
		pSignal->SetEvent();
		}
	// }}

	if( bAskNext ){
		bool bRoundEnded = false;
		bool bOpenCards = false;
		AskForAction(nSeat, bRoundEnded, bOpenCards);
		if( bRoundEnded ){
			EndRound();
			}
		}
	}

void
PokerGameController::OnTableInfo(ClientConnection* pConn, PokerTableInfoPacket* p){
	ASSERT( pConn );
	ASSERT(p->m_nTableId == m_info.m_nTableId);

	p->m_nHandId = m_nHandId;
	p->m_info.RemoveAll				();
	m_lockObjects.Lock(); // Lock
	p->m_info.Add					(&m_info);

	if( IsTimerWorking() ){
		m_lockTimerObjects.Lock();		// Lock
		p->m_shActiveSeatTimeFull	= m_ThreadTimerInfo.nDelaySec;
		p->m_shActiveSeatTimePassed = m_ThreadTimerInfo.nDelaySecPassed;
		m_lockTimerObjects.Unlock();	// Unlock
		}
	else{
		p->m_shActiveSeatTimeFull	= 0;
		p->m_shActiveSeatTimePassed = 0;
		}

	m_lockObjects.Unlock(); // Unlock

	m_pServer->SendPacketSheduled	(pConn, p);
	p->m_info.RemoveAll				(false);
	}

void
PokerGameController::OnShowMuckCards(ClientConnection* pConn, PokerShowMuckCardsPacket* p){
	int nTableId = m_info.m_nTableId;
	m_lockTimerObjects.Lock(); // Lock
	// Check for acception action packet from destination client {{
	if( pConn != m_ThreadTimerInfo.pConn ){
		m_lockTimerObjects.Unlock(); // Unlock
		return;
		}
	// }}

	// Validate received action and it's parameters. {{
	bool bBreak = false;
	PokerShowMuckCardsPacket* pPacketSent = (PokerShowMuckCardsPacket*)m_ThreadTimerInfo.pPacketSend;
	ASSERT(pPacketSent);
	if( pPacketSent->m_nCode != PacketCodes::_PokerShowMuckCards ){
		bBreak = true;
		}
	// }}
	m_lockTimerObjects.Unlock(); // Unlock
	if( bBreak ) return;
	CancelTimer(); // Cancel timer.

	// Send action packet. {{
	int nSeat	= (int)p->m_cSeat;
	PokerActionPacket* packetAction	= new PokerActionPacket();
	packetAction->m_nTableId		= p->m_nTableId;
	packetAction->m_nBalance		= 0;
	packetAction->m_nPotAmount		= 0;
	packetAction->m_nSeatTotalPot	= 0;
	packetAction->m_nTotalPot		= 0;
	packetAction->m_cSeat			= (char)nSeat;
	packetAction->m_cSeatReceiver	= (char)-1;
	packetAction->m_nAmount			= 0;

	if( p->m_bShowCards ){	// Show cards to everyone.
		packetAction->m_cActionType	= PokerActionTypes::ShowCards;
		m_lockObjects.Lock();	// Lock
		PokerTableSeatInfo* pSeatInfo	= m_info.GetSeatInfo(nSeat);
		if( pSeatInfo ){
			PokerHandCards*	pHandCards	= new PokerHandCards();
			pHandCards->m_btSeat		= nSeat;
			pHandCards->m_btCard1		= (BYTE)pSeatInfo->GetCard1();
			pHandCards->m_btCard2		= (BYTE)pSeatInfo->GetCard2();
			pHandCards->m_btCard3		= (BYTE)pSeatInfo->GetCard3();
			pHandCards->m_btCard4		= (BYTE)pSeatInfo->GetCard4();
			packetAction->m_listCardsShown.Add(pHandCards);
			}
		m_lockObjects.Unlock();	// Unlock
		}
	else{					// Muck cards.
		packetAction->m_cActionType		= PokerActionTypes::Muck;
		}

	m_pServer->SendPacketByTable(p->m_nTableId, packetAction);
	delete packetAction;
	// }}

	m_lockObjects.Lock(); // Lock
	m_info.ClearSeatCards();
	m_lockObjects.Unlock(); // Unlock

	// Activate hand start timer. {{
	OnTimerElapsed method = (OnTimerElapsed)&PokerGameController::OnHandStartTimerElapsed;
	InitializeTimer(NULL, NULL, m_nHandStartDelay, method);
	m_bHandStartTimerActive = true;
	// }}
	}

void
PokerGameController::OnSetOption(ClientConnection* pConn, PokerSetOptionPacket* p){
	int nSeat = (int)p->m_cSeat;
	m_lockObjects.Lock();		// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( pSeat ){
		switch( p->m_cOption ){
			case PokerTableOption::Option_AutoMuck:
				{
					pSeat->SetAutoMuck(p->m_bValue);
					break;
				}
			case PokerTableOption::Option_AutoPostBB:
				{
					pSeat->SetAutoPostBlind(p->m_bValue);
					break;
				}
			case PokerTableOption::Option_SitOutNextBB:
				{
					pSeat->SetSitOutNextBB(p->m_bValue);
					break;
				}
			case PokerTableOption::Option_SitOutNextHand:
				{
					pSeat->SetSitOutNextHand(p->m_bValue);
					break;
				}
			case PokerTableOption::Option_PayBB:
				{
					if( pSeat->GetStatus() == PokerSeatStatus::WaitingForBB )
						pSeat->SetStatus(PokerSeatStatus::PayBBAndPlay);
					break;
				}
			case PokerTableOption::Option_WaitingForBB:
				{
					if( pSeat->GetStatus() == PokerSeatStatus::PayBBAndPlay )
						pSeat->SetStatus(PokerSeatStatus::WaitingForBB);
					break;
				}
			case PokerTableOption::Option_Call:
				{
					pSeat->m_bPreAction_Call		= p->m_bValue;
					pSeat->m_nPreAction_CallAmount	= p->m_nValue;
					break;
				}
			
			case PokerTableOption::Option_CallAny:
				{
					pSeat->m_bPreAction_CallAny = p->m_bValue;
					break;
				}
			case PokerTableOption::Option_CallAnyCheck:
				{
					pSeat->m_bPreAction_CallAnyCheck = p->m_bValue;
					break;
				}
			case PokerTableOption::Option_Check:
				{
					pSeat->m_bPreAction_Check = p->m_bValue;
					break;
				}
			case PokerTableOption::Option_CheckFold:
				{
					pSeat->m_bPreAction_CheckFold = p->m_bValue;
					break;
				}
			case PokerTableOption::Option_Fold:
				{
					pSeat->m_bPreAction_Fold = p->m_bValue;
					break;
				}
			};
		}
	m_lockObjects.Unlock();		// Unlock
	}

void
PokerGameController::OnReturnBack(ClientConnection* pConn, PokerReturnBackPacket* p){
	m_lockObjects.Lock();		// Lock
	int					nSeat	= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	PokerSeatStatus status		= pSeat->GetStatus();
	bool			bSuccess	= false;
	if( status == PokerSeatStatus::SittedOut ){
		if( m_info.m_gameStatus == PokerGameStatus::NotStarted )
			pSeat->SetStatus(PokerSeatStatus::Sitted);
		else
			pSeat->SetStatus(PokerSeatStatus::WaitingForBB);
		bSuccess = true;
		}
	m_lockObjects.Unlock();		// Unlock

	if( bSuccess ){
		// Cancel leave seat timer.
		StopLeaveSeatTimer(nSeat);
		// Send seat action.
		PostSeatActionPacket(nSeat, PokerActionTypes::ReturnBack, PokerSeatStatus::Sitted, 0);
		}

	// Start hand start timer if game is not active. {{
	m_lockObjects.Lock();		// Lock
	if( m_info.m_gameStatus == PokerGameStatus::NotStarted && m_info.GetSittedCt() >= 2 && !m_bHandStartTimerActive ){
		m_info.ClearSeatCards();
		m_lockObjects.Unlock(); // Unlock
		
		// Activate hand start timer. {{
		OnTimerElapsed method	= (OnTimerElapsed)&PokerGameController::OnHandStartTimerElapsed;
		InitializeTimer(NULL, NULL, m_nHandStartDelay, method);
		m_bHandStartTimerActive = true;
		// }}
		}
	else
		m_lockObjects.Unlock(); // Unlock
	// }}
	}

void
PokerGameController::OnConnectionDropped(ClientConnection* pConn, int nSeat){
	if( !pConn ) return;
	if( nSeat != -1 ){
		SetSeatOffline(nSeat, false);
		}
	else{
		m_lockObjects.Lock(); // Lock
		RemoveFromWaitList(pConn);
		m_lockObjects.Unlock(); // Unlock
		}
	}

bool
PokerGameController::StartHand(){
	return StartHandThread();
	}

bool
PokerGameController::FinishHand(){
	m_lockObjects.Lock();	// Lock
	PokerGameStatus status = m_info.m_gameStatus;
	m_lockObjects.Unlock(); // Unlock
	if( status != PokerGameStatus::Started ){
		return false;
		}

	m_lockObjects.Lock();	// Lock
	List<PokerHandWinnerInfo> winners;
	int			nRakeAmount		= 0;
	float		fRakePercent	= m_info.m_fRakePercent;
	int			nTotalPotAmount	= 0;
	bool		bOpenWinnersCards = false;
	bool		bRet			= HoldemWinnerCalculator::GetInstance()->Holdem_CalculateWinners(&m_info, &winners, 
									fRakePercent, nRakeAmount, nTotalPotAmount, bOpenWinnersCards); 
	int			nTableId		= m_info.m_nTableId;	
	CDWordArray arrSitOutSeats;
	PokerHandFinishedPacket* p	= NULL;

	// Prepare 'handfinished' packet.
	if( bRet ){
		p					= new PokerHandFinishedPacket();
		p->m_nTableId		= nTableId;
		p->m_nHandId		= m_nHandId;
		p->m_nHandStartSec	= m_nHandStartDelay;

		if( bOpenWinnersCards ){
			m_info.GetSeatHandCards(&p->m_handCards);
			}

		for(int i=0; i<winners.GetCount(); i++){
			PokerHandWinnerInfo* pWinnerInfo = winners.GetAt(i);
			p->m_winners.Add(pWinnerInfo);
			}
		m_lockObjects.Unlock(); // Unlock
		}
	else
		m_lockObjects.Unlock(); // Unlock

	// Hand history entries.
	m_handHistory.FinishHand(&winners, &m_info);
	// Finish hand.
	m_info.FinishHand();
	// Call hand finished method.
	m_pServer->HandFinished(&m_info, m_nHandId, nTotalPotAmount, nRakeAmount, &m_handHistory);
	
	// Send finish hand packet.
	if( p ){
		m_pServer->SendPacketByTable(nTableId, p);
		p->m_winners.RemoveAll(false);
		delete p;
		}

	// Empty sessionless seats.
	EmptySessionlessSeats();
	// Collect sit out seats.
	m_info.GetSitOutSeats(arrSitOutSeats);
	// Seat wait list users, before empty seat appears to enyone.
	AutoSortedArray arrSeated;
	SeatWaitListUsers(arrSeated);

	// Force seats to seat out.
	for( int i=0; i<arrSitOutSeats.GetCount(); i++ )
		SitOutBySeat((int)arrSitOutSeats[i]);

	// Muck/Show Cards packet send to winner. {{
	if( !bOpenWinnersCards && winners.GetCount() == 1 ){
		PokerHandWinnerInfo* pWinnerInfo = winners.GetAt(0);
		if( pWinnerInfo ){
			int					nSeatIndex	= (int)pWinnerInfo->m_cSeat;
			m_lockObjects.Lock();	// Lock
			PokerTableSeatInfo* pSeatWinner = m_info.GetSeatInfo(nSeatIndex);
			m_lockObjects.Unlock(); // Unlock

			if( pSeatWinner && !pSeatWinner->GetAutoMuck() ){				
				PokerShowMuckCardsPacket* p1	= new PokerShowMuckCardsPacket();
				p1->m_nTableId					= nTableId;
				p1->m_cSeat						= (char)nSeatIndex;
				p1->m_shActionSec				= (short)m_nShowMuckDelay;

				OnTimerElapsed method = (OnTimerElapsed)&PokerGameController::OnShowMuckCardsTimerElapsed;
				InitializeTimer(pSeatWinner->GetConnection(), p1, m_nShowMuckDelay, method);
				// Activate seat with timer.
				SendSetActiveSeat(nSeatIndex, m_nShowMuckDelay, 0, 0);
				return true;
				}
			}
		}
	// }}

	m_lockObjects.Lock(); // Lock
	m_info.ClearSeatCards();
	m_lockObjects.Unlock(); // Unlock

	// Activate hand start timer. {{
	OnTimerElapsed method = (OnTimerElapsed)&PokerGameController::OnHandStartTimerElapsed;
	InitializeTimer(NULL, NULL, m_nHandStartDelay, method);
	m_bHandStartTimerActive = true;
	// }}
	return true;
	}

bool
PokerGameController::EndRound(){
	m_lockObjects.Lock();	// Lock
	PokerRoundType	round			= m_info.m_round;
	int				nInPlaySeatCt	= m_info.CountSeatsByStatus(PokerSeatStatus::InPlay);
	int				nAllInSeatCt	= m_info.CountSeatsByStatus(PokerSeatStatus::All_In);
	m_lockObjects.Unlock(); // Unlock

	if( round == PokerRoundType::No )
		return false;

	if( (nInPlaySeatCt + nAllInSeatCt) == 1 ){
		FinishHand();
		return true; // Finish hand
		}

	int nTotalPot = 0, nRoundPot = 0, nRakeAmount = 0;
	switch(round){
		case PokerRoundType::PreFlop:{
			m_lockObjects.Lock();	// Lock
			m_info.StartRound(PokerRoundType::Flop);

			// Hand history entries. {{
			if( m_info.GetRoundAmounts(PokerRoundType::PreFlop, nTotalPot, nRoundPot, nRakeAmount) ){
				m_handHistory.FinishRound	(nTotalPot, nRoundPot, nRakeAmount);
				m_handHistory.StartRound	(PokerRoundType::Flop);
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
				m_handHistory.StartRound	(PokerRoundType::Turn);
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
				m_handHistory.StartRound	(PokerRoundType::River);
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
				m_handHistory.StartRound	(PokerRoundType::Showdown);
				}
			// }}

			FinishHand();
			return true;
			}
		};

	m_lockObjects.Lock();	// Lock
	PokerRoundStartedPacket* p	= new PokerRoundStartedPacket();
	m_info.m_nActiveSeat		= -1; // Deactivate recently active seat.
	p->m_nTableId				= m_info.m_nTableId;
	p->m_nHandId				= m_nHandId;
	p->m_cRoundType				= (char)m_info.m_round;
	p->m_nMainPotAmount			= m_info.GetTotalPotAmount();
	p->m_nTotalPotAmount		= m_info.GetTotalPotAmount();
	p->m_shRoundStartSec		= (short)m_nStartRoundDelay;
	p->m_btTableCard1			= (BYTE)m_info.m_nCard1;
	p->m_btTableCard2			= (BYTE)m_info.m_nCard2;
	p->m_btTableCard3			= (BYTE)m_info.m_nCard3;
	p->m_btTableCard4			= (BYTE)m_info.m_nCard4;
	p->m_btTableCard5			= (BYTE)m_info.m_nCard5;

	for( int i=0; i<m_info.m_nMaxPlayersCt; i++ ){
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		if( !pSeat || !pSeat->GetSession() ) continue;
		Session* pSess = pSeat->GetSession();

		// In-play seat.
		if( pSeat->GetStatus() == PokerSeatStatus::All_In || pSeat->GetStatus() == PokerSeatStatus::InPlay ){	
			p->m_nReceiverSession	= pSess->GetSessionId();

			// Calculate best hand value. {{
			HoldemWinnerCalculator::GetInstance()->GetBestHandCardValue(
				pSeat->GetCard1(), pSeat->GetCard2(), m_info.m_nCard1, m_info.m_nCard2, 
				m_info.m_nCard3, m_info.m_nCard4, m_info.m_nCard5, (PokerHandCardsClass&)p->m_cBestCardsClass);
			// }}

			// Build pre-actions list. {{
			int				nCallAmount = m_info.GetCallAmount(pSeat->GetSeat());
			PokerPreAction* pPreAction	= NULL;
			if( nCallAmount == 0 ){
				pPreAction					= new PokerPreAction();
				pPreAction->m_cActionType	= PokerTableOption::Option_Check;
				pPreAction->m_bValue		= pSeat->m_bPreAction_Check;
				pPreAction->m_nValue		= 0;
				p->m_listPreActions.Add(pPreAction);

				pPreAction					= new PokerPreAction();
				pPreAction->m_cActionType	= PokerTableOption::Option_CallAnyCheck;
				pPreAction->m_bValue		= pSeat->m_bPreAction_CallAnyCheck;
				pPreAction->m_nValue		= 0;
				p->m_listPreActions.Add(pPreAction);

				pPreAction					= new PokerPreAction();
				pPreAction->m_cActionType	= PokerTableOption::Option_CheckFold;
				pPreAction->m_bValue		= pSeat->m_bPreAction_CheckFold;
				pPreAction->m_nValue		= 0;
				p->m_listPreActions.Add(pPreAction);
				}
			else{
				pPreAction					= new PokerPreAction();
				pPreAction->m_cActionType	= PokerTableOption::Option_Fold;
				pPreAction->m_bValue		= pSeat->m_bPreAction_Fold;
				pPreAction->m_nValue		= 0;
				p->m_listPreActions.Add(pPreAction);

				if( nCallAmount <= pSeat->GetBalance() ){
					pSeat->m_bPreAction_Call		= (pSeat->m_nPreAction_CallAmount == nCallAmount);
					pPreAction						= new PokerPreAction();
					pPreAction->m_cActionType		= PokerTableOption::Option_Call;
					pPreAction->m_bValue			= pSeat->m_bPreAction_Call;
					pPreAction->m_nValue			= nCallAmount;
					p->m_listPreActions.Add(pPreAction);
					}
				else
					pSeat->m_bPreAction_Call = false;

				pPreAction					= new PokerPreAction();
				pPreAction->m_cActionType	= PokerTableOption::Option_CallAny;
				pPreAction->m_bValue		= pSeat->m_bPreAction_CallAny;
				pPreAction->m_nValue		= 0;
				p->m_listPreActions.Add(pPreAction);
				}
			// }}
			}
		else{
			p->m_nReceiverSession	= 0;
			p->m_cBestCardsClass	= PokerHandCardsClass::CardsClass_None;
			}

		// Send packet.
		m_pServer->SendPacketSheduled(pSess->GetConnection(), p);
		// Remove pre-actions
		p->m_listPreActions.RemoveAll(); 
		}
	m_lockObjects.Unlock(); // Unlock

	// Send packet table observers.
	p->m_nReceiverSession	= 0;
	p->m_cBestCardsClass	= PokerHandCardsClass::CardsClass_None;
	m_pServer->SendPacketByTable(p->m_nTableId, p, true);
	delete p;

	// Wait while new round starts. {{
	OnTimerElapsed method		= (OnTimerElapsed)&PokerGameController::OnStartRoundTimerElapsed;
	InitializeTimer(NULL, NULL, m_nStartRoundDelay, method);
	// }}
	return true;
	}

bool
PokerGameController::DealCardsAndStartHand(int nDealer, int SBSeat, int BBSeat, CDWordArray& arrPayBBSeats){
	CPtrArray listInPlaySeats;
	m_lockObjects.Lock();		// Lock

	int nDealerOld = m_nDealer;
	int nSBSeatOld = m_nSmallBlind;
	int nBBSeatOld = m_nBigBlind;

	// Pay BB seats.
	for( int i=0; i<arrPayBBSeats.GetCount(); i++ ){
		// Post pay BB action.
		PostSeatActionPacket((int)arrPayBBSeats[i], PokerActionTypes::PostBB, PokerSeatStatus::Sitted, m_nBBAmount);
		}

	// Start hand will fail of insufficient players count.
	m_info.GetSeatCtByStatus(PokerSeatStatus::Sitted, &listInPlaySeats);
	if( listInPlaySeats.GetCount() < 2 ){
		m_info.m_gameStatus = PokerGameStatus::NotStarted;
		m_lockObjects.Unlock();	// Unlock
		return false;
		}

	m_nDealer		= nDealer;
	m_nSmallBlind	= SBSeat;
	m_nBigBlind		= BBSeat;

	if( m_nBigBlind == -1 ){ // Hand start will fail without BB seat.
		m_info.m_gameStatus = PokerGameStatus::NotStarted;
		if( m_nSmallBlind != -1 ){
			PokerActionPacket* p	= new PokerActionPacket();
			p->m_nTableId			= m_info.m_nTableId;
			p->m_nBalance			= m_info.GetBalance			(m_nSmallBlind) + m_nSBAmount;
			p->m_nPotAmount			= m_info.GetPotAmount		(m_nSmallBlind) - m_nSBAmount;
			p->m_nSeatTotalPot		= m_info.GetSeatTotalPot	(m_nSmallBlind) - m_nSBAmount;
			p->m_nTotalPot			= m_info.GetTotalPotAmount	() - m_nSBAmount;
			p->m_cSeat				= (char)m_nSmallBlind;
			p->m_cSeatReceiver		= (char)-1;
			p->m_cActionType		= (char)PokerActionTypes::NoAction;
			p->m_nAmount			= 0;
			m_info.ClearPotAmounts();
			m_lockObjects.Unlock();	// Unlock

			m_pServer->SendPacketByTable(p->m_nTableId, p);
			}
		else
			m_lockObjects.Unlock();	// Unlock

		// Restore old positions.
		m_nDealer		= nDealerOld;
		m_nSmallBlind	= nSBSeatOld;
		m_nBigBlind		= nBBSeatOld;
		return false;
		}

	// Who's sitting should have inplay state. {{ 
	for(int i=0; i<listInPlaySeats.GetCount(); i++){
		PokerTableSeatInfo* pSeatInPlay = (PokerTableSeatInfo*)listInPlaySeats.GetAt(i);
		ASSERT(pSeatInPlay);
		pSeatInPlay->SetStatus		(PokerSeatStatus::InPlay);
		pSeatInPlay->SetLastAction	(PokerActionTypes::NoAction);
		}
	// }}
	
	// Start hand, shuffle and deal cards.
	m_info.StartHand(m_nDealer, m_nSmallBlind, m_nBigBlind);

	// Retrieve hand id.
	m_pServer->HandStarted(m_nHandId, m_info.m_nTableId, listInPlaySeats.GetCount());

	m_info.m_nDealerSeat	= m_nDealer;
	m_info.m_nBBSeat		= m_nBigBlind;
	m_info.m_nSBSeat		= m_nSmallBlind;

	PokerHandStartedPacket* p = new PokerHandStartedPacket();
	p->m_nTableId			= m_info.m_nTableId;
	p->m_nHandId			= m_nHandId;
	p->m_cBigBlind			= (char)m_info.m_nBBSeat;
	p->m_cSmallBlind		= (char)m_info.m_nSBSeat;
	p->m_cDealer			= (char)m_info.m_nDealerSeat;

	int nLoop				= 0;
	int nCt					= m_info.m_nMaxPlayersCt;
	m_lockObjects.Unlock();		// Unlock

	float fTBankSecPerHand	= (float)(m_nTimeBankInc / ((float)m_nTimeBankIncHands));
	while( nLoop < nCt ){ 
		m_lockObjects.Lock();		// Lock
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nLoop);
		m_lockObjects.Unlock();		// Unlock
		if( !pSeat ){
			nLoop ++;
			continue;
			}

		// Increase collected time.
		pSeat->SetCollectedTimeBank(pSeat->GetCollectedTimeBank() + fTBankSecPerHand);

		p->m_cSeatReceiver  = (char)nLoop;
		// Send hand started packet. {{
		if( pSeat->GetConnection() ){
			p->m_handCards.RemoveAll();
			for(int i=0; i<listInPlaySeats.GetCount(); i++){
				PokerTableSeatInfo* pSeatInPlay = (PokerTableSeatInfo*)listInPlaySeats.GetAt(i);
				PokerHandCards*		pCards		= new PokerHandCards();
				
				if( pSeatInPlay == pSeat ){
					pCards->m_btSeat	= (BYTE)pSeatInPlay->GetSeat();
					pCards->m_btCard1	= (BYTE)pSeatInPlay->GetCard1();
					pCards->m_btCard2	= (BYTE)pSeatInPlay->GetCard2();
					pCards->m_btCard3	= (BYTE)pSeatInPlay->GetCard3();
					pCards->m_btCard4	= (BYTE)pSeatInPlay->GetCard4();
					}
				else{
					pCards->m_btSeat	= (BYTE)pSeatInPlay->GetSeat();
					pCards->m_btCard1	= -2;	// Hidden card.
					pCards->m_btCard2	= -2;	// Hidden card.
					pCards->m_btCard3	= pSeatInPlay->GetCard3() == -1 ? -1 : -2;
					pCards->m_btCard4	= pSeatInPlay->GetCard4() == -1 ? -1 : -2;
					}
				/*
				pCards->m_btSeat	= (BYTE)pSeatInPlay->GetSeat();
				pCards->m_btCard1	= (BYTE)pSeatInPlay->GetCard1();
				pCards->m_btCard2	= (BYTE)pSeatInPlay->GetCard2();
				pCards->m_btCard3	= (BYTE)pSeatInPlay->GetCard3();
				pCards->m_btCard4	= (BYTE)pSeatInPlay->GetCard4();
				*/
				p->m_handCards.Add(pCards);
				}
			// Send hand start packet. {{
			m_pServer->SendPacketSheduled(pSeat->GetConnection(), p);
			// }}
			}
		// }}
		nLoop ++;
		}

	// Send hand start packet only to observers. {{
	p->m_handCards.RemoveAll();
	for(int i=0; i<listInPlaySeats.GetCount(); i++){
		PokerTableSeatInfo* pSeatInPlay = (PokerTableSeatInfo*)listInPlaySeats.GetAt(i);
		PokerHandCards*		pCards		= new PokerHandCards();
	/*
		pCards->m_btSeat	= (BYTE)pSeatInPlay->GetSeat();
		pCards->m_btCard1	= (BYTE)pSeatInPlay->GetCard1();
		pCards->m_btCard2	= (BYTE)pSeatInPlay->GetCard2();
		pCards->m_btCard3	= (BYTE)pSeatInPlay->GetCard3();
		pCards->m_btCard4	= (BYTE)pSeatInPlay->GetCard4();
	*/
		pCards->m_btSeat	= (BYTE)pSeatInPlay->GetSeat();
		pCards->m_btCard1	= (BYTE)-2;	// Hidden card
		pCards->m_btCard1	= (BYTE)-2;	// Hidden card
		pCards->m_btCard2	= (BYTE)-2;	// Hidden card.
		pCards->m_btCard3	= pSeatInPlay->GetCard3() == -1 ? -1 : -2;
		pCards->m_btCard4	= pSeatInPlay->GetCard4() == -1 ? -1 : -2;

		p->m_handCards.Add(pCards);
		}
	p->m_cSeatReceiver = -1;
	m_pServer->SendPacketByTable(p->m_nTableId, p, true);
	// }}

	m_lockObjects.Lock(); // Lock
	// Hand history entries. {{
	m_handHistory.StartHand		(&m_info, m_nHandId, 0, m_nSBAmount, m_nBBAmount, m_nAnteAmount);
	m_handHistory.StartRound	(PokerRoundType::PreFlop);
	if( ((int)p->m_cSmallBlind) != -1 )
		m_handHistory.Action	((int)p->m_cSmallBlind,	PokerActionTypes::PostSB, m_nSBAmount, m_info.GetBalance((int)p->m_cSmallBlind));
	m_handHistory.Action		((int)p->m_cBigBlind,	PokerActionTypes::PostBB, m_nBBAmount, m_info.GetBalance((int)p->m_cBigBlind));

	// Pay BB and play seats.
	for( int i=0; i<arrPayBBSeats.GetCount(); i++ ){
		m_handHistory.Action(arrPayBBSeats[i],	PokerActionTypes::PostBB, m_nBBAmount, m_info.GetBalance(i));
		}
	// }}

	m_info.m_round			= PokerRoundType::PreFlop;
	m_info.m_gameStatus		= PokerGameStatus::Started;
	m_lockObjects.Unlock(); // Unlock
	delete p;

	// Start round start timer. {{
	OnTimerElapsed method		= (OnTimerElapsed)&PokerGameController::OnStartRoundTimerElapsed;
	InitializeTimer(NULL, NULL, m_nStartRoundDelay, method);
	// }}
	return true;
	}

bool
PokerGameController::SitOut(int nSeat, ClientConnection* pConn){
	m_lockObjects.Lock();		// Lock
	int nTableId				= m_info.m_nTableId;
	PokerSeatStatus status		= m_info.GetSeatStatus(nSeat);
	m_info.SitOut(nSeat);
	m_lockObjects.Unlock();		// Unlock

	if( status == PokerSeatStatus::Empty )
		return false;

	PokerSitOutPacket* p		= new PokerSitOutPacket();
	p->m_nSeat					= nSeat;
	p->m_nTableId				= nTableId;
	p->m_bSitOut				= true;
	m_pServer->SendPacketSheduled(pConn, p);
	delete p; // Delete packet.

	// Send SetSeatStatus packet. {{
	PokerSetSeatStatusPacket* p2= new PokerSetSeatStatusPacket();
	p2->m_nTableId				= nTableId;
	p2->m_cSeat					= (char)nSeat;
	p2->m_status				= (char)PokerSeatStatus::SittedOut;
	m_pServer->SendPacketByTable(nTableId, p2);
	delete p2;
	// }}

	// Start leave seat timer.
	StartLeaveSeatTimer(nSeat);
	return true;
	}

bool
PokerGameController::SitOutBySeat(int nSeat, bool bSetWaitingBB /*= false*/){
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock(); // Unlock
		return false;
		}
	m_lockObjects.Unlock(); // Unlock
	return SitOut(pSeat->GetSeat(), pSeat->GetConnection());
	}

bool
PokerGameController::AskForAction(int nSeatPrev, bool& bRoundEnded, bool& bOpenCards){
	m_lockObjects.Lock();	// Lock
	PokerGameStatus status	= m_info.m_gameStatus;
	PokerRoundType round	= m_info.m_round;
	int nTableId			= m_info.m_nTableId;
	int nBigBlind			= m_nBigBlind;
	int nDealer				= m_nDealer;
	m_lockObjects.Unlock(); // Unlock

	if( status != PokerGameStatus::Started || round == PokerRoundType::No ){
		return false;
		}

	m_lockObjects.Lock();	// Lock
	int					nSeatAsk		= -1;
	PokerTableSeatInfo* pSeat			= m_info.GetNextActiveSeat(nSeatPrev, nSeatAsk, bRoundEnded, bOpenCards);
	bool				bPreFlop		= (round == PokerRoundType::PreFlop);
	m_lockObjects.Unlock(); // Unlock

	if( pSeat && nSeatAsk > -1 ){
		m_lockObjects.Lock();	// Lock
		int nCallAmount		= m_info.GetCallAmount(nSeatAsk);
		int nMinBetAmount	= 0;
		int nMaxBetAmount	= 0;
		int nBalance		= pSeat->GetBalance();
		m_info.GetBetAmounts(nSeatAsk, nMinBetAmount, nMaxBetAmount);
		m_lockObjects.Unlock();	// Unlock

		PokerActPacket* p	= new PokerActPacket();
		p->m_nTableId		= nTableId;
		p->m_nHandId		= m_nHandId;
		p->m_cSeat			= (char)pSeat->GetSeat();
		p->m_cActionTime	= (char)(m_nRegularTimeSec + m_nExtraTimeSec + pSeat->GetTimeBankSeconds());

		if( nCallAmount == 0 ){ // Fold - Check - Bet/Raise
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
					pAction	= new PokerAction(); // Bet/Raise
					pAction->m_cActionType	= bPreFlop ? PokerActionTypes::Raise : PokerActionTypes::Bet;
					pAction->m_nAmountMin	= nMinBetAmount;
					pAction->m_nAmountMax	= min(nMaxBetAmount, nBalance);
					p->m_actions.Add(pAction);
					}
				}
			}
		else{	// Fold - Call - Bet/Raise
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

					if( nMinBetAmount >= nBalance ){
						pAction	= new PokerAction(); // All-In
						pAction->m_cActionType	= PokerActionTypes::AllIn;
						pAction->m_nAmountMin	= nBalance;
						pAction->m_nAmountMax	= nBalance;
						p->m_actions.Add(pAction);
						}
					else{
						pAction	= new PokerAction(); // Bet/Raise
						pAction->m_cActionType	= bPreFlop ? PokerActionTypes::Raise : PokerActionTypes::Bet;
						pAction->m_nAmountMin	= nMinBetAmount;
						pAction->m_nAmountMax	= min(nMaxBetAmount, nBalance);
						p->m_actions.Add(pAction);
						}
					}
				}
			}

		if( nSeatAsk != -1 ){
			// Analyze pre-actions. {{
			PokerActionPacket	autoActionPacket;
			PokerActionPacket* packet	= &autoActionPacket;
			packet->m_nTableId			= p->m_nTableId;
			packet->m_cSeat				= (char)pSeat->GetSeat();
			packet->m_cActionType		= PokerActionTypes::NoAction;
			packet->m_nAmount			= 0;
			
			for( int i=0; i<p->m_actions.GetCount(); i++ ){
				PokerAction* pAction = p->m_actions[i];
				if( !pAction ) continue;

				switch( pAction->m_cActionType ){
					case PokerActionTypes::AllIn:
						{
							if( pSeat->m_bPreAction_CallAny || pSeat->m_bPreAction_CallAnyCheck ){
								packet->m_cActionType				= PokerActionTypes::AllIn;
								packet->m_nAmount					= pAction->m_nAmountMin;
								pSeat->m_bPreAction_CallAny			= false; // Un-check preaction.
								pSeat->m_bPreAction_CallAnyCheck	= false; // Un-check preaction.
								}
							break;
						}
					case PokerActionTypes::Call:
						{
							if( (pSeat->m_bPreAction_Call && pAction->m_nAmountMin == pSeat->m_nPreAction_CallAmount) ||
								pSeat->m_bPreAction_CallAny || pSeat->m_bPreAction_CallAnyCheck ){
								packet->m_cActionType				= PokerActionTypes::Call;
								packet->m_nAmount					= pAction->m_nAmountMin;
								pSeat->m_bPreAction_Call			= false; // Un-check preaction.
								pSeat->m_bPreAction_CallAny			= false; // Un-check preaction.
								pSeat->m_bPreAction_CallAnyCheck	= false; // Un-check preaction.
								}
							break;
						}
					case PokerActionTypes::Fold:
						{	
							if( pSeat->m_bPreAction_Fold || pSeat->m_bPreAction_CheckFold ){
								packet->m_cActionType	= PokerActionTypes::Fold;
								packet->m_nAmount		= 0;
								pSeat->m_bPreAction_Fold = false; // Un-check preaction.
								}
							break;
						}
					case PokerActionTypes::Check:
						{
							if( pSeat->m_bPreAction_Check || pSeat->m_bPreAction_CheckFold || pSeat->m_bPreAction_CallAnyCheck ){
								packet->m_cActionType				= PokerActionTypes::Check;
								packet->m_nAmount					= 0;
								pSeat->m_bPreAction_Check			= false; // Un-check preaction.
								pSeat->m_bPreAction_CheckFold		= false; // Un-check preaction.
								pSeat->m_bPreAction_CallAnyCheck	= false; // Un-check preaction.
								}
							break;
						}
					};
				}
			// }}

			if( packet->m_cActionType != PokerActionTypes::NoAction ){
				// Action predicted by pre-action.
				OnAction(pSeat->GetConnection(), packet, true);
				}
			else{
				// Calculate time bank. {{
				int		nThinkTimeSec	= m_nRegularTimeSec + m_nExtraTimeSec;
				int		nTimeBankSec	= pSeat->GetTimeBankSeconds();
				float	fCollectedSec	= pSeat->GetCollectedTimeBank();
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

				OnTimerElapsed method = (OnTimerElapsed)&PokerGameController::OnActTimerElapsed;
				InitializeTimer(pSeat->GetConnection(), p, nThinkTimeSec, method);
				// Activate seat.
				SendSetActiveSeat(nSeatAsk, m_nRegularTimeSec, m_nExtraTimeSec, pSeat->GetTimeBankSeconds());
				}
			}
		return true;
		}
	return false;
	}

bool
PokerGameController::SetSeatOffline(int nSeat, bool bOnline /*= false*/){
	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock();	// Unlock
		return false;
		}

	int	nTableId = m_info.m_nTableId;
	// Set offline status.
	pSeat->SetOffline(!bOnline);	
	m_lockObjects.Unlock();	// Unlock

	// Send SeatOnlineStatusChanged packet. {{
	PokerSeatOnlineChangedPacket* p = new PokerSeatOnlineChangedPacket();
	p->m_nTableId					= nTableId;
	p->m_cSeat						= (char)nSeat;
	p->m_bOnline					= bOnline;
	m_pServer->SendPacketByTable(nTableId, p);
	delete p;
	// }}
	return true;
	}

bool
PokerGameController::EmptySeat(int nSeat){
	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock();	// Unlock
		return false;
		}

	bool			bEmptySeat	= false;
	int				nTableId	= m_info.m_nTableId;
	PokerSeatStatus status		= pSeat->GetStatus();

	// If hand is not started empty seat at once.	
	if( (m_info.m_gameStatus == PokerGameStatus::NotStarted) || 
		(status == PokerSeatStatus::SittedOut || status == PokerSeatStatus::Empty) ){
		// Call callback method.
		m_pServer->PlayerLeftSeat(nTableId, nSeat, m_info.GetSeatInfo(nSeat));
		// Empty seat.
		m_info.EmptySeat(nSeat);
		bEmptySeat = true;
		}
	else{// Sessionless seat will empty on hand finish.
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
		if( pSeat ){
			// Empty session.
			pSeat->SetSession(NULL);
			// Remove from wait list.
			RemoveFromWaitList(pSeat->GetConnection());
			}
		else // seat is already empty ???
			bEmptySeat = true;
		}
	m_lockObjects.Unlock();	// Unlock

	if( bEmptySeat ){
		// Send SetSeatStatus packet. {{
		PokerSetSeatStatusPacket* p	= new PokerSetSeatStatusPacket();
		p->m_nTableId				= nTableId;
		p->m_cSeat					= (char)nSeat;
		p->m_status					= (char)PokerSeatStatus::Empty;
		m_pServer->SendPacketByTable(nTableId, p);
		delete p;
		// }}
		// Seat wait list users, before empty seat appears to enyone.
		AutoSortedArray arrSeated;
		SeatWaitListUsers(arrSeated);
		}
	return true;
	}

int
PokerGameController::EmptySessionlessSeats(){
	m_lockObjects.Lock(); // Lock
	CDWordArray arrEmptySeats;
	m_info.EmptySessionlessSeats(false, &arrEmptySeats);

	PokerSetSeatStatusPacket* p	= new PokerSetSeatStatusPacket();
	p->m_nTableId				= m_info.m_nTableId;
	p->m_status					= (char)PokerSeatStatus::Empty;

	for( int i=0; i<arrEmptySeats.GetCount(); i++ ){
		int nSeat = (int)arrEmptySeats[i];
		// Call callback method.
		m_pServer->PlayerLeftSeat(p->m_nTableId, nSeat, m_info.GetSeatInfo(nSeat));
		// Empty seat info.
		m_info.EmptySeat(nSeat);
		}
	m_lockObjects.Unlock(); // Unlock

	// Send empty seat packet.
	for( int i=0; i<arrEmptySeats.GetCount(); i++ ){
		p->m_cSeat	= (char)arrEmptySeats[i];
		m_pServer->SendPacketByTable(p->m_nTableId, p);
		}

	delete p;
	return (int)arrEmptySeats.GetCount();
	}

bool
PokerGameController::ForceSeatToLeaveTable(int nSeat){
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nSeat);
	if( !pSeatInfo || 
		pSeatInfo->GetStatus() == PokerSeatStatus::Empty || 
		!pSeatInfo->GetSession() ){
		m_lockObjects.Unlock(); // Unlock
		return false;
		}

	PokerLeaveTablePacket packet;
	packet.m_bIsRequest = true;
	packet.m_nSessionId = pSeatInfo->GetSession()->GetSessionId();
	packet.m_nTableId	= m_info.m_nTableId;
	m_lockObjects.Unlock(); // Unlock

	m_pServer->OnLeaveTable(&packet, (void*)pSeatInfo->GetConnection());
	return true;
	}

bool
PokerGameController::SendPostBlindRequest(int nSeat, bool bSmallBlind, CEvent* pSignal /*= NULL*/, bool* pElapsed /*= NULL*/,
										  BasePacket** ppPacketReceived /*= NULL*/){
	if( bSmallBlind ){
		int					nSBSeat		= nSeat;
		PokerTableSeatInfo* pSBSeatInfo = m_info.GetSeatInfo(nSeat);
		if( pSBSeatInfo ){
			if( pSBSeatInfo->GetAutoPostBlind() ){
				m_lockObjects.Lock(); // Lock
				int nBalance = m_info.GetBalance(nSeat) - m_nSBAmount;
				m_info.SetBalance	(nSeat, nBalance);
				m_info.AddPotAmount(nSeat, m_nSBAmount);
				m_lockObjects.Unlock(); // Unlock
				// Post seat action.
				PostSeatActionPacket(nSeat, PokerActionTypes::PostSB, PokerSeatStatus::Sitted, m_nSBAmount);

				if( pElapsed )
					*pElapsed = false;
				if( pSignal )
					pSignal->SetEvent();
				if( ppPacketReceived ){
					PokerActionPacket* pPacket	= new PokerActionPacket();
					pPacket->m_cSeat			= (char)nSeat;
					pPacket->m_cSeatReceiver	= (char)-1;
					pPacket->m_cActionType		= PokerActionTypes::PostSB;
					*ppPacketReceived			= pPacket;
					}
				return true;
				}

			PokerPostBlindPacket* p		= new PokerPostBlindPacket();
			m_lockObjects.Lock();	// Lock
			p->m_nTableId				= m_info.m_nTableId;
			m_lockObjects.Unlock();	// Unlock
			p->m_bPostBB				= false; // Post BigBlind
			p->m_cSeat					= (char)nSBSeat;
			p->m_shActionSec			= (short)m_nPostBlindDelay;

			if( pSignal ){
				InitializeTimer(pSBSeatInfo->GetConnection(), p, m_nPostBlindDelay, NULL, pSignal, pElapsed, ppPacketReceived);
				}
			else{
				OnTimerElapsed method		= (OnTimerElapsed)&PokerGameController::OnPostSmallBlindTimerElapsed;
				InitializeTimer(pSBSeatInfo->GetConnection(), p, m_nPostBlindDelay, method, pSignal, pElapsed, ppPacketReceived);
				}

			SendSetActiveSeat(nSBSeat, m_nPostBlindDelay);
			return true;
			}
		}
	else{
		int					nBBSeat		= nSeat;
		PokerTableSeatInfo* pBBSeatInfo = m_info.GetSeatInfo(nSeat);
		if( pBBSeatInfo ){
			if( pBBSeatInfo->GetAutoPostBlind() ){
				m_lockObjects.Lock(); // Lock
				int nBalance = m_info.GetBalance(nSeat) - m_nBBAmount;
				m_info.SetBalance	(nSeat, nBalance);
				m_info.AddPotAmount (nSeat, m_nBBAmount);
				m_lockObjects.Unlock(); // Unlock
				// Post seat action.
				PostSeatActionPacket(nSeat, PokerActionTypes::PostBB, PokerSeatStatus::Sitted, m_nBBAmount);

				if( pElapsed )
					*pElapsed = false;
				if( pSignal )
					pSignal->SetEvent();
				if( ppPacketReceived ){
					PokerActionPacket* pPacket	= new PokerActionPacket();
					pPacket->m_cSeat			= (char)nSeat;
					pPacket->m_cSeatReceiver	= (char)-1;
					pPacket->m_cActionType		= PokerActionTypes::PostBB;
					*ppPacketReceived			= pPacket;
					}
				return true;
				}

			PokerPostBlindPacket* p		= new PokerPostBlindPacket();
			m_lockObjects.Lock();	// Lock
			p->m_nTableId				= m_info.m_nTableId;
			m_lockObjects.Unlock();	// Unlock
			p->m_bPostBB				= true; // Post BigBlind
			p->m_cSeat					= (char)nBBSeat;
			p->m_shActionSec				= (short)m_nPostBlindDelay;
			if( pSignal ){
				InitializeTimer(pBBSeatInfo->GetConnection(), p, m_nPostBlindDelay, NULL, pSignal, pElapsed, ppPacketReceived);
				}
			else{
				OnTimerElapsed method		= (OnTimerElapsed)&PokerGameController::OnPostBigBlindTimerElapsed;
				InitializeTimer(pBBSeatInfo->GetConnection(), p, m_nPostBlindDelay, method, pSignal, pElapsed, ppPacketReceived);
				}
		
			SendSetActiveSeat(nBBSeat, m_nPostBlindDelay);
			return true;
			}
		}
	return false;
	}

bool
PokerGameController::SendSetActiveSeat(int nActiveSeat, int nRegularTimeSec, int nExtraTimeSec /*= 0*/, int nTimeBankSec /*= 0*/){
	m_lockObjects.Lock();	// Lock
	int nTableId = m_info.m_nTableId;
	m_info.m_nActiveSeat = nActiveSeat;
	m_lockObjects.Unlock(); // Unlock

	// Send set active packet. {{
	PokerSetActivePacket* p		= new PokerSetActivePacket();
	p->m_cSeat					= (char)nActiveSeat;
	p->m_nTableId				= nTableId;
	p->m_shRegularTimeSec		= (short)nRegularTimeSec;
	p->m_shExtraTimeSec			= (short)nExtraTimeSec;
	p->m_shTimeBankSec			= (short)nTimeBankSec;
	m_pServer->SendPacketByTable(nTableId, p);
	delete p;
	// }}
	return true;
	}

bool
PokerGameController::PostSeatActionPacket(int nSeat, PokerActionTypes action, PokerSeatStatus status, int nAmount){
	// Send action packet. {{
	PokerActionPacket* packet	= new PokerActionPacket();
	m_lockObjects.Lock();	// Lock
	bool bSendPreActions		= (m_info.GetSeatStatus(nSeat) == PokerSeatStatus::InPlay);
	m_info.SetLastAction(nSeat, action);	// Remember last action.
	m_info.SetSeatStatus(nSeat, status);	// Set seat status.
	packet->m_nTableId			= m_info.m_nTableId;
	packet->m_nBalance			= m_info.GetBalance			(nSeat);
	packet->m_nPotAmount		= m_info.GetPotAmount		(nSeat);
	packet->m_nSeatTotalPot		= m_info.GetSeatTotalPot	(nSeat);
	packet->m_nTotalPot			= m_info.GetTotalPotAmount	();
	packet->m_bSetPreActions	= bSendPreActions;

	// Retrieve in-play seats to send packet one-by-one.
	CPtrArray arrSeats;
	m_info.GetSeats(arrSeats);

	m_lockObjects.Unlock(); // Unlock
	packet->m_cSeat				= (char)nSeat;
	packet->m_cSeatReceiver		= (char)-1;
	packet->m_cActionType		= action;
	packet->m_nAmount			= nAmount;

	// Determine hand cards open status. {{
	if( m_info.m_round > PokerRoundType::No ){
		int nSeatAsk = -1;
		bool bRoundEnded = false, bOpenCards = false;
		m_info.GetNextActiveSeat(nSeat, nSeatAsk, bRoundEnded, bOpenCards);
		if( bOpenCards ){
			m_info.GetSeatHandCards(&packet->m_listCardsShown);
			}
		}
	// }}
	if( arrSeats.GetCount() > 0 ){
		// Send packet one by one.
		for(int i=0; i<arrSeats.GetCount(); i++){
			PokerTableSeatInfo* pSeatInfo = (PokerTableSeatInfo*)arrSeats.GetAt(i);
			if( !pSeatInfo ) continue;

			m_lockObjects.Lock(); // Lock
			// Setup pre-actions for each seat.
			if( bSendPreActions && pSeatInfo->GetStatus() == PokerSeatStatus::InPlay ){
				int				nCallAmount = m_info.GetCallAmount(pSeatInfo->GetSeat());
				PokerPreAction* pPreAction	= NULL;
				if( nCallAmount == 0 ){
					pPreAction					= new PokerPreAction();
					pPreAction->m_cActionType	= PokerTableOption::Option_Check;
					pPreAction->m_bValue		= pSeatInfo->m_bPreAction_Check;
					pPreAction->m_nValue		= 0;
					packet->m_listPreActions.Add(pPreAction);

					pPreAction					= new PokerPreAction();
					pPreAction->m_cActionType	= PokerTableOption::Option_CallAnyCheck;
					pPreAction->m_bValue		= pSeatInfo->m_bPreAction_CallAnyCheck;
					pPreAction->m_nValue		= 0;
					packet->m_listPreActions.Add(pPreAction);

					pPreAction					= new PokerPreAction();
					pPreAction->m_cActionType	= PokerTableOption::Option_CheckFold;
					pPreAction->m_bValue		= pSeatInfo->m_bPreAction_CheckFold;
					pPreAction->m_nValue		= 0;
					packet->m_listPreActions.Add(pPreAction);
					}
				else{
					pPreAction					= new PokerPreAction();
					pPreAction->m_cActionType	= PokerTableOption::Option_Fold;
					pPreAction->m_bValue		= pSeatInfo->m_bPreAction_Fold;
					pPreAction->m_nValue		= 0;
					packet->m_listPreActions.Add(pPreAction);

					if( nCallAmount <= pSeatInfo->GetBalance() ){
						pSeatInfo->m_bPreAction_Call	= (pSeatInfo->m_nPreAction_CallAmount == nCallAmount);
						pPreAction						= new PokerPreAction();
						pPreAction->m_cActionType		= PokerTableOption::Option_Call;
						pPreAction->m_bValue			= pSeatInfo->m_bPreAction_Call;
						pPreAction->m_nValue			= nCallAmount;
						packet->m_listPreActions.Add(pPreAction);
						}
					else
						pSeatInfo->m_bPreAction_Call = false;

					pPreAction					= new PokerPreAction();
					pPreAction->m_cActionType	= PokerTableOption::Option_CallAny;
					pPreAction->m_bValue		= pSeatInfo->m_bPreAction_CallAny;
					pPreAction->m_nValue		= 0;
					packet->m_listPreActions.Add(pPreAction);
					}
				}

			packet->m_cSeatReceiver = (char)pSeatInfo->GetSeat();
			m_lockObjects.Unlock(); // Unlock

			m_pServer->SendPacketSheduled(pSeatInfo->GetConnection(), packet);
			packet->m_listPreActions.RemoveAll(); // Remove pre-actions
			}

		// Send only to observers.
		packet->m_cSeatReceiver	= (char)-1;
		m_pServer->SendPacketByTable(packet->m_nTableId, packet, true);
		delete packet;
		// }}
		}
	else{
		// Send packet to everyone.
		packet->m_cSeatReceiver	= (char)-1;
		m_pServer->SendPacketByTable(packet->m_nTableId, packet, false);
		delete packet;
		}
	return true;
	}

bool
PokerGameController::IsSeatReservedByConn(int nSeat, ClientConnection* pConn){
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat || pSeat->GetStatus() != PokerSeatStatus::Reserved ){
		return false;
		}
	bool bRet = (pSeat->GetConnection() == pConn);
	return bRet;
	}

bool
PokerGameController::CancelSeatReservation(int nSeat){
	int		nTimerId	= m_arrSeatReserveTimerIds[nSeat];
	bool	bRet		= Timers::GetInstance()->KillTimer(nTimerId);
	m_arrSeatReserveTimerIds[nSeat] = 0;
	return bRet;
	}

void
PokerGameController::SeatWaitListUsers(AutoSortedArray& arrSeated){
	m_lockObjects.Lock(); // Lock
	int nLoop	= 0;
	int nCt		= m_listWaitingList.GetCount();
	int nSeat	= -1;

	while( nLoop < nCt ){
		nSeat = m_info.GetFirstEmptySeat();
		if( nSeat == -1 ) break;

		ClientConnection* pConn = m_listWaitingList.GetAt(0);
		if( !pConn ) break;

		Session* pSess = pConn->GetSession();
		if( !pSess ){
			RemoveFromWaitList(pConn);
			nCt --;
			}

		UserInfo* pUser = (UserInfo*)pSess->Get(_T("User"));
		if( !pUser ) break;

		// Remove from wait list.
		RemoveFromWaitList(pConn);

		// Add seated seat index.
		arrSeated.Add(nSeat);

		// Pseudo reserve seat packet.
		PokerReserveSeatPacket* packet	= new PokerReserveSeatPacket();
		packet->m_nSessionId			= pSess->GetSessionId();
		packet->m_nTableId				= m_info.m_nTableId;
		packet->m_bIsRequest			= true;
		packet->m_cSeat					= (char)nSeat;
		packet->m_sUserName				= pUser->m_sUserName;
		OnReserveSeat(pConn, packet);
		delete packet;
		}
	m_lockObjects.Unlock(); // Unlock
	}

int
PokerGameController::AddWaitList(ClientConnection* pConn){
	m_lockObjects.Lock(); // Lock
	int nIndex = m_arrWaitingListConn.IndexOf((__int64)pConn);
	if( nIndex != -1 ){ // Connection already exists into waiting list.
		m_lockObjects.Unlock(); // Unlock
		return nIndex;
		}
	// Add connection to wait list.
	m_listWaitingList.Add(pConn);
	// Add connection into array.
	m_arrWaitingListConn.Add((__int64)pConn);

	nIndex = m_arrWaitingListConn.GetCount() - 1;
	m_lockObjects.Unlock(); // Unlock
	return nIndex;
	}

bool
PokerGameController::RemoveFromWaitList(ClientConnection* pConn){
	m_lockObjects.Lock(); // Lock
	int nIndex = m_arrWaitingListConn.IndexOf((__int64)pConn);
	if( nIndex != -1 ){ // Connection already exists in waiting list.
		m_arrWaitingListConn.Delete(nIndex);
		m_listWaitingList	.Remove(pConn, false);
		m_lockObjects.Unlock(); // Unlock
		return true;
		}
	m_lockObjects.Unlock(); // Unlock
	return false;
	}

bool
PokerGameController::IsInWaitList(ClientConnection* pConn){
	m_lockObjects.Lock(); // Lock
	int nIndex = m_arrWaitingListConn.IndexOf((__int64)pConn);
	m_lockObjects.Unlock(); // Unlock
	if( nIndex != -1 ){ // Connection already exists in waiting list.
		return true;
		}
	return false;
	}