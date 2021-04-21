#include "stdafx.h"
#include "PokerServer.h"
#include "..\GameServer\ClientConnection.h"
#include "HoldemWinnerCalculator.h"
#include "PokerGameController.h"
#include "..\Utility\Timers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

	m_bIsTournamentGame			= false;

	m_nDealer					= -1;
	m_nSmallBlind				= -1;
	m_nBigBlind					= -1;
	m_nHandId					= -1;

	// Not destroy objects when remove element from array.
	m_listWaitingList.DestroyObjects(false);

	memset(m_arrSeatReserveTimerIds, 0, sizeof(m_arrSeatReserveTimerIds));
	memset(m_arrLeaveSeatTimerIds, 0, sizeof(m_arrLeaveSeatTimerIds));

	// Start game thread.
	// Start only when it will be neccessary.
	//GThread_Start();
	} 

BEGIN_DECLARE_VAR(PokerGameController, Serializable)
	DECLARE_VAR(_T("PokerGameController"),	_T(""),		VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("TableInfo"),			_T(""),		VariableType::None, offsetof(PokerGameController,m_info), false, true, false)
END_DECLARE_VAR()

PokerGameController::~PokerGameController(){
	if( !m_pServer ) return;

	// Stop game thread.
	GThread_Stop();

	// Kill all timers.
	KillAllTimers();

	// Destroy waiting list.
	m_listWaitingList.RemoveAll(false);
	}

void
PokerGameController::OnOpenTable(PokerSession* pSess, ClientConnection* pConn, PokerOpenTablePacket* p){
	ASSERT( pConn );
	ASSERT(p->m_nTableId == m_info.m_nTableId);
	// Allow observer open table. {{
	UserInfo*	pUser	= (UserInfo*)pSess->GetUserInfo();
	int			nUserId	= pUser ? pUser->m_nUserId : 0;
	// }}

	m_lockObjects.Lock();	// Lock
	int nSeatEmpty		= m_info.GetFirstEmptySeat();
	p->m_nHandId		= m_nHandId;
	p->m_cSeat			= (char)-1;
	p->m_info.RemoveAll	();
	p->m_info.Add		(&m_info);

	// Reconnect seat from another session. {{
	PokerTableSeatInfo*	pSeatInfo = NULL;
	if( pUser && pUser->m_nUserId > 0 )
		pSeatInfo = m_info.GetSeatByUserId(pUser->m_nUserId);

	if( pSeatInfo ){
		int nSeat					= pSeatInfo->GetSeat();
		p->m_cSeat					= (char)nSeat;
		p->m_bSeatOrJoinWaitList	= false;
		// Bring seat online.
		if( pSeatInfo->IsOffline() ){
			SetSeatOffline(nSeat, true);
			}
		// Set seat session.
		pSeatInfo->SetSession(pConn->GetSession());
		// Attach connection to seat.
		pSess->SeatAt(p->m_nTableId, nSeat);
		}
	// }}

	BasePacket* pPacketRepeat = NULL;
	// Retrieve seat action wait process time info. <<
	m_gameThread.Lock();
//	ClientConnection*	pConnOld	= (ClientConnection*)m_gameThread.GetParam(_T("CONN"));
	BasePacket*			pPacketSent	= (BasePacket*)m_gameThread.GetParam(_T("PACKET"));
	int*				pMSecPassed	= (int*)m_gameThread.GetParam(_T("MSEC_PASSED"));
	int*				pMSecWait	= (int*)m_gameThread.GetParam(_T("MSEC_WAIT"));
	int					nMSecPassed = pMSecPassed ? *pMSecPassed : 0;
	int					nMSecWait	= pMSecWait ? *pMSecWait : 0;
	m_gameThread.Unlock();
	p->m_shActiveSeatTimeFull	= nMSecWait / 1000;
	p->m_shActiveSeatTimePassed = nMSecPassed / 1000;
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

	// Retrieve statistic info.
	m_pServer->GetTableStatInfoByUser(nUserId, p->m_nTableId, p);
	// Send reply.
	m_pServer->SendPacketQueued		(pConn, p);
	p->m_info.RemoveAll				(false);
	if( p->m_bSeatOrJoinWaitList && pUser ){
		// Reserve seat.
		if( nSeatEmpty != -1 ){
			PokerReserveSeatPacket* packet	= new PokerReserveSeatPacket();
			packet->m_nSessionId			= p->m_nSessionId;
			packet->m_nTableId				= p->m_nTableId;
			packet->m_bIsRequest			= true;
			packet->m_cSeat					= (char)nSeatEmpty;
			packet->m_sUserName				= pUser->m_sUserName;
			OnReserveSeat(pSess, pConn, packet);
			delete packet;
			}
		else{ // join waiting list.
			PokerJoinWaitingListPacket* packet = new PokerJoinWaitingListPacket;
			packet->m_bIsRequest			= true;
			packet->m_nSessionId			= p->m_nSessionId;
			packet->m_nTableId				= p->m_nTableId;
			packet->m_bIsRequest			= true;
			OnJoinWaitingList(pSess, pConn, packet);
			delete packet;
			}
		}

	// Repeat last sent action packet.
	if( pPacketRepeat ){
		m_pServer->SendPacketQueued(pConn, pPacketRepeat);
		delete pPacketRepeat;
		}
	}

void
PokerGameController::OnLeaveTable(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTablePacket* p){
	// If seat is taken by connection. {{
	m_lockObjects.Lock(); // Lock
	bool bOpened = false;
	int nSeatIndex = pSess->GetSeatIndex(m_info.m_nTableId, bOpened);
	if( bOpened ){
		if( nSeatIndex != -1 ){
			// Kill leave seat timer.
			StopLeaveSeatTimer(nSeatIndex);
			// Empty seat
			EmptySeat(nSeatIndex);
			}
		else{
			// Remove from wait list.
			RemoveFromWaitList(pConn);	
			}
		// }}
		p->m_cSeat = (char)nSeatIndex;
		m_lockObjects.Unlock(); // Unlock
		// Send reply.
		m_pServer->SendPacketQueued(pConn, p);
		}
	else
		m_lockObjects.Unlock(); // Unlock
	}

void
PokerGameController::OnSeatAt(PokerSession* pSess, ClientConnection* pConn, PokerSeatAtTablePacket* p){
	ASSERT( pConn );
	int nSeatIndex = (int)p->m_cSeatIndex;
	if( !p || (nSeatIndex < 0 || nSeatIndex >= m_info.m_cMaxPlayersCt) )
		return;

	// Check for user.
	// Check for amount.
	// Check seat status if already taken seat.
	UserInfo* pUser = pSess->GetUserInfo();
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
		pSeatInfo->SetIpAddress			(pConn->GetRemoteIp());
		pSeatInfo->SetSeat				(nSeatIndex);
		pSeatInfo->SetBalance			(p->m_nChipAmount);
		pSeatInfo->SetAddedAmount		(p->m_nChipAmount);
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
			if( !m_pServer->PlayerSeated(m_info.m_nTableId, m_nCurrencyId, nSeatIndex, p->m_nChipAmount, pSeatInfo) ){
				m_info.EmptySeat(nSeatIndex);
				m_lockObjects.Unlock(); // Unlock
				// Send seat status change.
				SendSeatStatusChange(nSeatIndex, PokerSeatStatus::Empty);
				return;
				}
			
			// Call seat balance change callback
			m_pServer->PlayerSeatBalanceChanged((PokerSession*)pSeatInfo->GetSession(), m_info.m_nTableId, nSeatIndex, p->m_nChipAmount, 0);
			}
		}
	m_lockObjects.Unlock(); // Unlock

	if( bSuccess ){
		ASSERT(m_pServer);
		if( pAvatarInfo ){
			p->m_avatarData.m_nId						= pAvatarInfo->m_nId;
			p->m_avatarData.m_recStatus					= 0;
			p->m_avatarData.m_sAvatarFileName			= pAvatarInfo->m_sAvatarFileName;
//			p->m_avatarData.m_binImage.m_dwDataLength	= pAvatarInfo->m_binImage.m_dwDataLength;
//			p->m_avatarData.m_binImage.m_hData			= pAvatarInfo->m_binImage.m_hData;
			p->m_avatarData.m_binImage.m_dwDataLength	= 0L;
			p->m_avatarData.m_binImage.m_hData			= NULL;
			}
		else{
			p->m_avatarData.m_nId						= 0;
			p->m_avatarData.m_recStatus					= 0;
			p->m_avatarData.m_binImage.m_dwDataLength	= 0L;
			p->m_avatarData.m_binImage.m_hData			= NULL;
			}

		// Send seat at packet to everyone.
		m_pServer->SendPacketByTable(m_info.m_nTableId, p);
		// Zero avatar binary data.
		p->m_avatarData.m_binImage.m_dwDataLength	= 0L;
		p->m_avatarData.m_binImage.m_hData			= NULL;

		// Start hand start timer if game is not active. {{
		m_lockObjects.Lock();		// Lock
		if( m_info.m_gameStatus == PokerGameStatus::NotStarted && m_info.GetSittedCt() >= 2 && (GThread_IsPaused() || !GThread_IsStarted()) ){
			m_info.ClearSeatCards();
			m_lockObjects.Unlock(); // Unlock

			// Resume game thread to start hand.
			GThread_Resume();
			}
		else
			m_lockObjects.Unlock(); // Unlock
		// }}
		}
	}

void
PokerGameController::OnReserveSeat(PokerSession* pSess, ClientConnection* pConn, PokerReserveSeatPacket* p){
	ASSERT( pConn );
	UserInfo* pUser = pSess->GetUserInfo();
	if( !pUser ) return;

	DWORD dwRemoteIp = pConn->GetRemoteIp();
	m_lockObjects.Lock(); // Lock
	int nCurrencyId = m_info.m_nCurrencyId;
	int nMinBuyIn	= m_info.m_nMinBuyIn;
	int nMaxBuyIn	= m_info.m_nMaxBuyIn;

	int nSeatIp = -1;
	if( m_info.GetSeatWithIp(dwRemoteIp, nSeatIp) ){
		if( !m_pServer->IsAllowedMultiUserIp(dwRemoteIp) && !m_pServer->IsDontRestrictIPUser(pUser->m_sUserName) ){
			m_lockObjects.Unlock(); // Unlock
			p->m_nErrorCode = ES_ERROR_IP_ALREADY_EXISTS;
			// Send reply.
			m_pServer->SendPacketQueued(pConn, p);
			return;
			}
		}

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

	// Min buy-in.
	p->m_nAmountMin				= m_info.m_nMinBuyIn;
	// Max buy-in
	p->m_nAmountMax				= m_info.m_nMaxBuyIn;
	m_lockObjects.Unlock(); // Unlock

	// Retrieve min buy in for current table. {{
	int nBalance = -1;
	if( m_pServer->GetSeatMinBuyIn(pUser->m_nUserId, p->m_nTableId, nCurrencyId, p->m_nAmountMin, nBalance) ){
		if( p->m_nAmountMin < nMinBuyIn )
			p->m_nAmountMin = nMinBuyIn;

		// Insufficient balance amount.
		if( nBalance < p->m_nAmountMin ){
			p->m_nBalance		= nBalance;
			p->m_nAmountMin		= m_info.m_nMinBuyIn;
			p->m_nAmountMax		= m_info.m_nMaxBuyIn;
			p->m_nErrorCode		= ES_ERROR_INSUFFICIENT_AMOUNT;
			m_info.SetSeatInfo(nSeat, NULL);
			m_lockObjects.Unlock(); // Unlock
			m_pServer->SendPacketQueued(pConn, p);
			return;
			}

		p->m_nBalance		= nBalance;
		p->m_nAmountMax		= min(nMaxBuyIn, p->m_nAmountMax);
		}
	else{
		if( nBalance == -1 )
			nBalance		= m_pServer->GetUserBalance(pUser);
		p->m_nBalance		= nBalance;
		p->m_nAmountMin		= m_info.m_nMinBuyIn;
		p->m_nAmountMax		= m_info.m_nMaxBuyIn;
		p->m_nErrorCode		= ES_ERROR_INSUFFICIENT_AMOUNT;
		m_info.SetSeatInfo(nSeat, NULL);
		m_lockObjects.Unlock(); // Unlock
		m_pServer->SendPacketQueued(pConn, p);
		return;
		}
	// }}

	// Send response to seat reserver. {{
	p->m_sUserName				= pInfo->GetUserName();
	p->m_nBalance				= p->m_nBalance;
	p->m_shReservationTimeSec	= (short)m_nReserveSeatDelay;
	p->m_bReservationExpired	= false;
	m_pServer->SendPacketQueued(pConn, p);
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

	// Call event method.
	m_pServer->PlayerReservedSeat(pSess, p->m_nTableId, nSeat);
	m_lockObjects.Unlock(); // Unlock
	}

void
PokerGameController::OnCancelReservation(PokerSession* pSess, ClientConnection* pConn, PokerCancelReservationPacket* p){
	m_lockObjects.Lock();		// Lock
	int nSeat		= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nSeat);
	if( !pSeatInfo || pSeatInfo->GetStatus() != PokerSeatStatus::Reserved ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	CString sUserName = m_info.GetSeatUserName(nSeat); 
	// Empty seat.
	m_info.EmptySeat(nSeat);
	// Call event method.
	m_pServer->PlayerCanceledReservation(pSess, p->m_nTableId, nSeat);
	m_lockObjects.Unlock(); // Unlock

	// Send seat status change packet to everyone. {{
	PokerSetSeatStatusPacket* pSeatStatus = new PokerSetSeatStatusPacket();
	pSeatStatus->m_nTableId		= p->m_nTableId;
	pSeatStatus->m_cSeat		= (char)nSeat;
	pSeatStatus->m_status		= (char)PokerSeatStatus::Empty;
	pSeatStatus->m_sUserName	= sUserName; 
	m_pServer->SendPacketByTable(p->m_nTableId, pSeatStatus);
	delete pSeatStatus;
	// }}
	}

void
PokerGameController::OnJoinWaitingList(PokerSession* pSess, ClientConnection* pConn, PokerJoinWaitingListPacket* p){
	UserInfo* pUser = (UserInfo*)pSess->GetUserInfo();
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

		OnReserveSeat(pSess, pConn, packet);
		delete packet;
		return;
		}
	else{
		// Wait index number.
		p->m_shIndex = AddInWaitList(pConn);
		}
	m_lockObjects.Unlock(); // Unlock
	// Send reply packet.
	m_pServer->SendPacketQueued(pConn, p);
	}

void
PokerGameController::OnAddChips(PokerSession* pSess, ClientConnection* pConn, PokerAddChipsPacket* p){
	UserInfo* pUser = (UserInfo*)pSess->GetUserInfo();
	if( !pUser ) return;

	int					nSeat	= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty )
		return;

	if( p->m_bPrepareToAdd ){
		int nSeatBalance	= pSeat->GetBalance();
		int nBalance		= 0;

		nBalance			= m_pServer->GetUserBalance(pUser);
		p->m_nBalance		= nBalance;
		p->m_nMin			= min(m_info.m_nMinBuyIn, nBalance);
		p->m_nMax			= min(m_info.m_nMaxBuyIn, nBalance);

		int nMaxAllowed		= (m_info.m_nMaxBuyIn - nSeatBalance);
		int	nMinAllowed		= (m_info.m_nMinBuyIn - nSeatBalance);
		if( nMinAllowed <= 0 )
			nMinAllowed		= 1;

		if( nMaxAllowed <= 0 ){
			p->m_nMin = 0;
			p->m_nMax = 0;
			}
		else{
			p->m_nMax = min(nMaxAllowed, nBalance);
			p->m_nMin = min(nMinAllowed, p->m_nMax);
			}
		
		p->m_nSeatSessionId = pSess->GetSessionId();
		m_pServer->SendPacketQueued(pConn, p);
		}
	else{
		if( pSeat->GetStatus() == PokerSeatStatus::Reserved )
			return;
		int nSeatBalance = AddChips(nSeat, p->m_nAmount);
		if( nSeatBalance > 0 ){
			SendSeatActionPacket(nSeat, PokerActionTypes::AddChips, PokerSeatStatus::Sitted, p->m_nAmount);
			}
		}
	}

void
PokerGameController::OnAction(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p, bool bPreAction /*= false*/){
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
PokerGameController::OnShowMuckCards(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p){
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
PokerGameController::OnTableInfo(PokerSession* pSess, ClientConnection* pConn, PokerTableInfoPacket* p){
	ASSERT( pConn );
	ASSERT(p->m_nTableId == m_info.m_nTableId);

	p->m_nHandId = m_nHandId;
	p->m_info.RemoveAll			();
	m_lockObjects.Lock(); // Lock
	p->m_info.Add				(&m_info);

	// Retrieve seat action wait process time info. <<
	m_gameThread.Lock();
	int*	pMSecPassed	= (int*)m_gameThread.GetParam(_T("MSEC_PASSED"));
	int*	pMSecWait	= (int*)m_gameThread.GetParam(_T("MSEC_WAIT"));
	int		nMSecPassed = pMSecPassed ? *pMSecPassed : 0;
	int		nMSecWait	= pMSecWait ? *pMSecWait : 0;
	m_gameThread.Unlock();
	p->m_shActiveSeatTimeFull	= nMSecWait / 1000;
	p->m_shActiveSeatTimePassed = nMSecPassed / 1000;
	// >>
	m_lockObjects.Unlock(); // Unlock

	m_pServer->SendPacketQueued	(pConn, p);
	p->m_info.RemoveAll				(false);
	}

void
PokerGameController::OnSetOption(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p){
	int nSeat = (int)p->m_cSeat;
	m_lockObjects.Lock();		// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( pSeat ){
		switch( p->m_cOption ){
			case PokerTableOption::Option_AutoMuck:{
				pSeat->SetAutoMuck(p->m_bValue);
				break;
				}
			case PokerTableOption::Option_AutoPostBB:{
				pSeat->SetAutoPostBlind(p->m_bValue);
				break;
				}
			case PokerTableOption::Option_SitOutNextBB:{
				pSeat->SetSitOutNextBB(p->m_bValue);
				break;
				}
			case PokerTableOption::Option_SitOutNextHand:{
				pSeat->SetSitOutNextHand(p->m_bValue);

				// If hand not started sit out at once.
				if( p->m_bValue && 
					pSeat->GetStatus() == PokerSeatStatus::Sitted &&
					m_info.m_gameStatus == PokerGameStatus::NotStarted ){
					SitOut(nSeat, pConn);
					}
				break;
				}
			case PokerTableOption::Option_PayBB:{
				if( pSeat->GetStatus() == PokerSeatStatus::WaitingForBB )
					pSeat->SetStatus(PokerSeatStatus::PayBBAndPlay);
				break;
				}
			case PokerTableOption::Option_WaitingForBB:{
				if( pSeat->GetStatus() == PokerSeatStatus::PayBBAndPlay )
					pSeat->SetStatus(PokerSeatStatus::WaitingForBB);
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
PokerGameController::OnChatMessage(PokerSession* pSess, ClientConnection* pConn, PokerChatMessagePacket* p){
	// Send chat message.
	p->m_message.m_dtTime = COleDateTime::GetCurrentTime();
	m_pServer->SendPacketByTable(p->m_nTableId, p);
	}

void
PokerGameController::OnReturnBack(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p){
	m_lockObjects.Lock();		// Lock
	int					nSeat	= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( !pSeat || pSeat->GetStatus() != PokerSeatStatus::SittedOut ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	if( pSeat->GetBalance() < m_info.m_nBigBlindAmount  ){
		// Show add chips window.
		PokerAddChipsPacket packetAddChips;
		packetAddChips.m_bIsRequest		= true;
		packetAddChips.m_bPrepareToAdd	= true;
		packetAddChips.m_cSeat			= (char)nSeat;
		packetAddChips.m_nAmount		= 0;
		packetAddChips.m_nSessionId		= pSess->GetSessionId();
		packetAddChips.m_nTableId		= p->m_nTableId;
		OnAddChips(pSess, pConn, &packetAddChips);
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	bool			bStart			= (m_info.m_gameStatus == PokerGameStatus::NotStarted && m_info.GetSittedCt() >= 1 && (GThread_IsPaused() || !GThread_IsStarted()));
	PokerSeatStatus status			= pSeat->GetStatus();
	bool			bSuccess		= false;
	bool			bSitOutNextHand	= pSeat->GetSitOutNextHand();
	if( status == PokerSeatStatus::SittedOut ){
		if( bStart )
			pSeat->SetStatus(PokerSeatStatus::Sitted);
		else
			pSeat->SetStatus(PokerSeatStatus::WaitingForBB);
		bSuccess = true;
		// Clear SitOutNextHand option.
		if( bSitOutNextHand ){
			pSeat->SetSitOutNextHand(false);
			}
		}
	else
		bStart = false;
	
	if( bSuccess ){
		// Cancel leave seat timer.
		StopLeaveSeatTimer(nSeat);
		m_lockObjects.Unlock();		// Unlock

		// Send seat action.
		SendSeatActionPacket(nSeat, PokerActionTypes::ReturnBack, pSeat->GetStatus(), 0);

		if( bSitOutNextHand ){
			// Clear SitOutNextHand option.
			PokerSetOptionPacket packet1;
			packet1.m_nTableId	= m_info.m_nTableId;
			packet1.m_cSeat		= (char)nSeat;
			packet1.m_cOption	= (char)PokerTableOption::Option_SitOutNextHand;
			packet1.m_bValue	= false;
			packet1.m_nValue	= 0;
			m_pServer->SendPacketQueued(pConn, &packet1);
			}
		}
	else
		m_lockObjects.Unlock();		// Unlock

	// Start hand start timer if game is not active. {{
	if( bStart ){
		m_lockObjects.Lock();	// Lock
		m_info.ClearSeatCards();
		m_lockObjects.Unlock(); // Unlock

		// Resume game thread to start hand.
		GThread_Resume();
		}
	// }}
	}

void
PokerGameController::OnConnectionDropped(PokerSession* pSess, ClientConnection* pConn, int nSeat){
	if( !pConn || !pSess ) return;
	m_lockObjects.Lock(); // Lock
	if( nSeat > -1 ){
		if( IsSeatReservedByConn(nSeat, pConn) )
			OnReservationTimerElapsed(nSeat);
		else
			SetSeatOffline(nSeat, false);
		}
	else
		RemoveFromWaitList(pConn);
	m_lockObjects.Unlock(); // Unlock
	}

void
PokerGameController::OnGameSessionExpired(PokerSession* pSess, ClientConnection* pConn,  bool& bCancel){
	bCancel = false;
	if( !pConn || !pSess ) return; 
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatBySession(pSess);
	if( pSeat ){
		if( (!m_bIsTournamentGame) && (m_info.m_gameStatus != PokerGameStatus::NotStarted) ){
			// When game thread is started and not paused(waiting for start game event).
			if( GThread_IsStarted() && !GThread_IsPaused() )
				bCancel = true; // Cancel session expiration.
			}
		else
		if( m_bIsTournamentGame )
			bCancel = true;

		if( !bCancel )
			EmptySeat(pSeat->GetSeat());
		}
	m_lockObjects.Unlock(); // Unlock
	}

bool
PokerGameController::OnGameSessionRestored(PokerSession* pSess, ClientConnection* pConn, int nUserId){
	if( !pSess ) return false;
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatByUserId(nUserId);
	if( !pSeatInfo ) {
		m_lockObjects.Unlock(); // Unlock
		return false;
		}
	pSeatInfo->SetSession(pSess);
	int nSeat = pSeatInfo->GetSeat();
	m_lockObjects.Unlock(); // Unlock

	if( nSeat > -1 )
		SetSeatOffline(nSeat, true);

	// Send table info packet to restored session.
	PokerOpenTablePacket tableInfo;
	PokerOpenTablePacket* p = &tableInfo;
	PokerTableInfo* pTableInfo = new PokerTableInfo();

	m_lockObjects.Lock();		// Lock
	p->m_nHandId				= m_nHandId;
	p->m_nSessionId				= pSess->GetSessionId();
	p->m_nTableId				= m_info.m_nTableId;
	p->m_cSeat					= (char)nSeat;
	p->m_bSeatOrJoinWaitList	= false;
	p->m_nErrorCode				= 0;

	m_info.Copy(pTableInfo);
	p->m_info.Add(pTableInfo);

	BasePacket* pPacketRepeat = NULL;
	// Retrieve seat action wait process time info. <<
	m_gameThread.Lock();
	BasePacket*			pPacketSent	= (BasePacket*)m_gameThread.GetParam(_T("PACKET"));
	int*				pMSecPassed	= (int*)m_gameThread.GetParam(_T("MSEC_PASSED"));
	int*				pMSecWait	= (int*)m_gameThread.GetParam(_T("MSEC_WAIT"));
	int					nMSecPassed = pMSecPassed ? *pMSecPassed : 0;
	int					nMSecWait	= pMSecWait ? *pMSecWait : 0;

	if( pSeatInfo && pPacketSent && m_info.m_cActiveSeat == nSeat ){
		// Change connection for waiting action.
		m_gameThread.SetParam(_T("CONN"), pConn);
		pPacketRepeat = m_pServer->CreatePacketByCode(pPacketSent->m_nCode);
		if( pPacketRepeat ){
			pPacketSent->Copy(pPacketRepeat);
			} 
		}

	m_gameThread.Unlock();
	p->m_shActiveSeatTimeFull	= nMSecWait / 1000;
	p->m_shActiveSeatTimePassed = nMSecPassed / 1000;
	// >>
	m_lockObjects.Unlock(); // Unlock

	// Send open table packet.
	m_pServer->SendPacketQueued(pConn, p);
	p->m_info.RemoveAll(true);

	// Reply last sent message.
	if( pPacketRepeat ){
		m_pServer->SendPacketQueued(pConn, pPacketRepeat);
		delete pPacketRepeat;
		}
	return true;
	}

int
PokerGameController::PerformAddChipsAction(){
	m_lockObjects.Lock();		// Lock
	int nLoop		= 0;
	int nCt			= m_arrAddChips.GetCount();
	int nSuccessCt	= 0;

	AutoSortedArray arrAddChips;
	arrAddChips.Copy(&m_arrAddChips);
	m_arrAddChips.DeleteAll();  // Clear Add chips array.
	m_lockObjects.Unlock();		// Unlock

	while( nLoop < nCt ){
		int nSeat = (int)arrAddChips.Get(nLoop);
		int nAmount = (int)arrAddChips.GetData(nLoop);
		if( AddChips(nSeat, nAmount) > 0 ){
			m_lockObjects.Lock();		// Lock
			PokerSeatStatus status = m_info.GetSeatStatus(nSeat);
			m_lockObjects.Unlock();		// Unlock

			// Send add chips state change.
			SendSeatActionPacket(nSeat, PokerActionTypes::AddChips, status, nAmount);
			nSuccessCt ++;
			}
		nLoop ++;
		}

	return nSuccessCt;
	}

int
PokerGameController::AddChips(int nSeat, int nAmount){
	m_lockObjects.Lock();		// Lock
	if( nSeat < 0 || nSeat >= m_info.m_cMaxPlayersCt ){
		m_lockObjects.Unlock();		// Unlock
		return 0;
		}

	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ||
		!pSeat->GetConnection() || 
		pSeat->GetStatus() == PokerSeatStatus::Empty ){
		m_lockObjects.Unlock();		// Unlock
		return 0;
		}

	if( m_info.m_gameStatus == PokerGameStatus::NotStarted || pSeat->GetStatus() == PokerSeatStatus::SittedOut ){
		ClientConnection*	pConn	= pSeat->GetConnection();
		PokerSession*		pSess	= (PokerSession*)pConn->GetSession();
		if( !pSess || !pConn ){
			m_lockObjects.Unlock();		// Unlock
			return 0;
			}

		UserInfo* pUser = (UserInfo*)pSess->GetUserInfo();
		if( !pUser ){
			m_lockObjects.Unlock();		// Unlock
			return 0;
			}

		// Not allowed to add such amount.
		if( (pSeat->GetBalance() + nAmount) > m_info.m_nMaxBuyIn){
			m_lockObjects.Unlock();		// Unlock
			return 0;
			}

		int nTableId = m_info.m_nTableId;
		// Stop leave seat timer.
		StopLeaveSeatTimer(nSeat);
		m_lockObjects.Unlock();		// Unlock

		// ******************
		// User add chips.
		if( m_pServer->UserAddChips(pUser, nTableId, nSeat, nAmount, m_info.m_nCurrencyId) ){
			m_lockObjects.Lock();		// Lock
			// Change seat status.
			if( pSeat->GetStatus() == PokerSeatStatus::SittedOut ){
				if( m_info.m_gameStatus != PokerGameStatus::NotStarted )
					pSeat->SetStatus(PokerSeatStatus::WaitingForBB);
				else
					pSeat->SetStatus(PokerSeatStatus::Sitted);

				// Start hand start timer if game is not active. {{
				if( m_info.m_gameStatus == PokerGameStatus::NotStarted && m_info.GetSittedCt() >= 2 && (GThread_IsPaused() || !GThread_IsStarted())){
					m_info.ClearSeatCards();
					// Resume game thread to start hand.
					GThread_Resume();
					}
				// }}
				}

			// Call seat balance change callback
			m_pServer->PlayerSeatBalanceChanged((PokerSession*)pSeat->GetSession(), nTableId, nSeat, pSeat->GetBalance() + nAmount, pSeat->GetBalance());

			pSeat->Add_AddedAmount	(nAmount);
			pSeat->SetBalance		(pSeat->GetBalance() + nAmount);
			int nBalance = pSeat->GetBalance();
			m_lockObjects.Unlock();		// Unlock
			return nBalance;
			}
		// ******************
		}
	else{
		// Register for future action. (When hand ends.)
		int nIndex = m_arrAddChips.IndexOf(nSeat);
		if( nIndex == -1 )
			m_arrAddChips.Add(nSeat, nAmount);
		else
			m_arrAddChips.Set(nIndex, nSeat, nAmount);
		m_lockObjects.Unlock();		// Unlock
		}
	return 0;
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
		int nSeat		= (int)arrPayBBSeats[i];
		int nBalance	= m_info.GetBalance(nSeat) - m_nBBAmount;
		if( nBalance >= 0 ){
			m_info.SetBalance	(nSeat, nBalance);
			m_info.AddPotAmount	(nSeat, m_nBBAmount);
			// Post pay BB action.
			SendSeatActionPacket(nSeat, PokerActionTypes::PostBB, PokerSeatStatus::Sitted, m_nBBAmount);
			}
		else
			SitOutBySeat(nSeat);
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
		if( pSeatInPlay->GetBalance() == 0 )
			pSeatInPlay->SetStatus(PokerSeatStatus::All_In);
		else
			pSeatInPlay->SetStatus	(PokerSeatStatus::InPlay);
		pSeatInPlay->SetLastAction	(PokerActionTypes::NoAction);
		}
	// }}
	
	// Start hand, shuffle and deal cards.
	m_info.StartHand(m_nDealer, m_nSmallBlind, m_nBigBlind);
	// Retrieve hand id.
	m_pServer->HandStarted(m_nHandId, m_info.m_nTableId, listInPlaySeats.GetCount());

	m_info.m_cDealerSeat	= (char)m_nDealer;
	m_info.m_cBBSeat		= (char)m_nBigBlind;
	m_info.m_cSBSeat		= (char)m_nSmallBlind;

	PokerHandStartedPacket* p = new PokerHandStartedPacket();
	p->m_dtAction			= COleDateTime::GetCurrentTime();
	p->m_nTableId			= m_info.m_nTableId;
	p->m_nHandId			= m_nHandId;
	p->m_cBigBlind			= m_info.m_cBBSeat;
	p->m_cSmallBlind		= m_info.m_cSBSeat;
	p->m_cDealer			= m_info.m_cDealerSeat;

	int nLoop				= 0;
	int nCt					= m_info.m_cMaxPlayersCt;
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
				pCards->m_cBestCardsClass		= PokerHandCardsClass::CardsClass_None;
				
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
			m_pServer->SendPacketQueued(pSeat->GetConnection(), p);
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
	return true;
	}

bool
PokerGameController::SitOut(int nSeat, ClientConnection* pConn){
	m_lockObjects.Lock();		// Lock
	int nTableId				= m_info.m_nTableId;
	PokerSeatStatus status		= m_info.GetSeatStatus(nSeat);
	CString			sUserName;
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( pSeat )
		sUserName = pSeat->GetUserName();
	m_info.SitOut(nSeat);
	m_lockObjects.Unlock();		// Unlock

	if( status == PokerSeatStatus::Empty )
		return false;

	PokerSitOutPacket* p		= new PokerSitOutPacket();
	p->m_nSeat					= nSeat;
	p->m_nTableId				= nTableId;
	p->m_bSitOut				= true;
	m_pServer->SendPacketQueued(pConn, p);
	delete p; // Delete packet.

	// Send SetSeatStatus packet. {{
	PokerSetSeatStatusPacket* p2= new PokerSetSeatStatusPacket();
	p2->m_nTableId				= nTableId;
	p2->m_cSeat					= (char)nSeat;
	p2->m_sUserName				= sUserName;
	p2->m_status				= (char)PokerSeatStatus::SittedOut;
	m_pServer->SendPacketByTable(nTableId, p2);
	delete p2;
	// }}

	m_lockObjects.Lock();		// Lock
	// Start leave seat timer if not tournament table.
	if( !m_bIsTournamentGame )
		StartLeaveSeatTimer(nSeat);
	m_lockObjects.Unlock();		// Unlock
	return true;
	}

bool
PokerGameController::SitOutBySeat(int nSeat){
	m_lockObjects.Lock();		// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock(); // Unlock
		return false;
		}
	
	bool bRet = SitOut(pSeat->GetSeat(), pSeat->GetConnection());
	m_lockObjects.Unlock();		// Unlock
	return bRet;
	}

bool
PokerGameController::SetSeatOffline(int nSeat, bool bOnline /*= false*/,  PokerSession* pSess /*= NULL*/){
	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock();	// Unlock
		return false;
		}

	int	nTableId = m_info.m_nTableId;
	// Set seat session.
	if( pSess )
		pSeat->SetSession(pSess);
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
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock();	// Unlock
		return false;
		}

	bool			bEmptySeat	= false;
	int				nTableId	= m_info.m_nTableId;
	PokerSeatStatus status		= pSeat->GetStatus();
	PokerSession*	pSess		= (PokerSession*)pSeat->GetSession();
	CString			sUserName	= pSeat->GetUserName();

	if( status == PokerSeatStatus::Reserved ){
		if( CancelSeatReservation(nSeat) )
			m_pServer->PlayerCanceledReservation(pSess, m_info.m_nTableId, nSeat);
		// Empty seat.
		m_info.EmptySeat(nSeat);
		bEmptySeat = true;
		}
	else{
		UserInfo* pUser	= NULL;
		if( pSess ){
			pUser = pSess->GetUserInfo();
			// If hand is not started or not in play state, free seat at once.	
			if( (m_info.m_gameStatus == PokerGameStatus::NotStarted) || 
				(status == PokerSeatStatus::SittedOut || status == PokerSeatStatus::Empty) || 
				(m_info.m_gameStatus != PokerGameStatus::NotStarted && status != PokerSeatStatus::InPlay && status != PokerSeatStatus::All_In) ){
				// Call seat balance change callback.
				m_pServer->PlayerSeatBalanceChanged(pSess, nTableId, nSeat, 0, pSeat->GetBalance());
				// Call seat left callback.
				m_pServer->PlayerLeftSeat(nTableId, m_nCurrencyId, nSeat, pSeat);
				// Empty seat.
				m_info.EmptySeat(nSeat);
				bEmptySeat = true;
				}
			else{// Sessionless seat will empty when hand finishes.
				// Remove from wait list.
				RemoveFromWaitList(pSeat->GetConnection());
				// Set seat offline
				SetSeatOffline(nSeat, false);
				// Zero seat session.
				pSeat->SetSession(NULL);
				}
			}
		else{
			if( pSeat && status != PokerSeatStatus::Empty ){
				bEmptySeat = true;
				// Call seat balance change callback.
				m_pServer->PlayerSeatBalanceChanged(pSess, nTableId, nSeat, 0, pSeat->GetBalance());
				// Call seat left callback.
				m_pServer->PlayerLeftSeat(nTableId, m_nCurrencyId, nSeat, pSeat);
				}
			// Empty seat.
			m_info.EmptySeat(nSeat);
			}
		}
	m_lockObjects.Unlock();	// Unlock

	if( bEmptySeat ){
		// Send SetSeatStatus packet. {{
		PokerSetSeatStatusPacket* p	= new PokerSetSeatStatusPacket();
		p->m_nTableId				= nTableId;
		p->m_cSeat					= (char)nSeat;
		p->m_sUserName				= sUserName;
		p->m_status					= (char)PokerSeatStatus::Empty;
		m_pServer->SendPacketByTable(nTableId, p);
		delete p;
		// }}
		}
	return true;
	}

int
PokerGameController::EmptySessionlessSeats(){
	m_lockObjects.Lock(); // Lock
	if( m_bIsTournamentGame ){
		m_lockObjects.Unlock(); // Unlock
		return 0;
		}

	CDWordArray arrEmptySeats;
	m_info.EmptySessionlessSeats(false, &arrEmptySeats);

	PokerSetSeatStatusPacket* p	= new PokerSetSeatStatusPacket();
	p->m_nTableId				= m_info.m_nTableId;
	p->m_status					= (char)PokerSeatStatus::Empty;

	CStringArray arrUserNames;
	for( int i=0; i<arrEmptySeats.GetCount(); i++ ){
		int nSeat = (int)arrEmptySeats[i];
		arrUserNames.Add(m_info.GetSeatUserName(nSeat)); 
		// Call seat left callback.
		m_pServer->PlayerLeftSeat(p->m_nTableId, m_nCurrencyId, nSeat, m_info.GetSeatInfo(nSeat));
		// Empty seat info.
		m_info.EmptySeat(nSeat);
		}
	m_lockObjects.Unlock(); // Unlock

	// Send empty seat packet.
	for( int i=0; i<arrEmptySeats.GetCount(); i++ ){
		p->m_cSeat		= (char)arrEmptySeats[i];
		p->m_sUserName	= arrUserNames[i];
		m_pServer->SendPacketByTable(p->m_nTableId, p);
		}
	
	delete p;
	return (int)arrEmptySeats.GetCount();
	}

bool
PokerGameController::ForceSeatToLeaveTable(int nSeat){
	m_lockObjects.Lock();			// Lock
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nSeat);
	if( !pSeatInfo || pSeatInfo->GetStatus() == PokerSeatStatus::Empty || !pSeatInfo->GetSession() ){
		m_lockObjects.Unlock();		// Unlock
		return false;
		}

	PokerLeaveTablePacket packet;
	packet.m_bIsRequest				= true;
	packet.m_nSessionId				= pSeatInfo->GetSession()->GetSessionId();
	packet.m_nTableId				= m_info.m_nTableId;
	bool				bOffline	= pSeatInfo->IsOffline();
	ClientConnection*	pConn		= pSeatInfo->GetConnection();
	
	if( bOffline || !pConn ){
		EmptySeat(nSeat);
		}
	else
		m_pServer->OnLeaveTable(&packet, (void*)pConn);

	m_lockObjects.Unlock();		// Unlock
	return true;
	}

bool
PokerGameController::GetEmptySeat(int& nSeat){
	m_lockObjects.Lock(); // Lock
	bool bRet = false;
	nSeat = m_info.GetFirstEmptySeat();
	if( nSeat != -1 )
		bRet = true;
	m_lockObjects.Unlock(); // Unlock
	return bRet;
	}

bool
PokerGameController::SendPostBlindRequest(int nSeat, bool bSmallBlind, bool& bResult){
	bResult = false;
	if( bSmallBlind ){
		int					nSBSeat		= nSeat;
		m_lockObjects.Lock(); // Lock
		PokerTableSeatInfo* pSBSeatInfo = m_info.GetSeatInfo(nSeat);
		if( pSBSeatInfo ){
			if( pSBSeatInfo->GetAutoPostBlind() ){
				int nBalance = m_info.GetBalance(nSeat) - m_nSBAmount;
				m_info.SetBalance	(nSeat, nBalance);
				m_info.AddPotAmount	(nSeat, m_nSBAmount);
				m_lockObjects.Unlock(); // Unlock
				
				// Post seat action.
				SendSeatActionPacket(nSeat, PokerActionTypes::PostSB, PokerSeatStatus::Sitted, m_nSBAmount);
				bResult = true;
				return true;
				}

			// Send PostSB packet.
			PokerPostBlindPacket* p		= new PokerPostBlindPacket();
			p->m_nTableId				= m_info.m_nTableId;
			p->m_bPostBB				= false; // Post BigBlind
			p->m_cSeat					= (char)nSBSeat;
			p->m_shActionSec			= (short)m_nPostBlindDelay;

			ClientConnection* pConn = pSBSeatInfo->GetConnection();
			m_lockObjects.Unlock(); // Unlock

			// Add parameters.
			m_gameThread.AddParam(_T("PACKET"), p);
			m_gameThread.AddParam(_T("CONN"),	pConn);
			// Reset sync event.
			m_gameThread.SetSyncEvent(false);
			// Send packet.
			m_pServer->SendPacketQueued(pConn, p);
			// Send active seat packet.
			SendSetActiveSeat(nSBSeat, m_nPostBlindDelay);
			return true;
			}
		m_lockObjects.Unlock(); // Unlock
		}
	else{
		int					nBBSeat		= nSeat;
		m_lockObjects.Lock(); // Lock
		PokerTableSeatInfo* pBBSeatInfo = m_info.GetSeatInfo(nSeat);
		if( pBBSeatInfo ){
			if( pBBSeatInfo->GetAutoPostBlind() ){
				int nBalance = m_info.GetBalance(nSeat) - m_nBBAmount;
				m_info.SetBalance	(nSeat, nBalance);
				m_info.AddPotAmount (nSeat, m_nBBAmount);
				m_lockObjects.Unlock(); // Unlock

				// Send seat action.
				SendSeatActionPacket(nSeat, PokerActionTypes::PostBB, PokerSeatStatus::Sitted, m_nBBAmount);
				bResult = true;
				return true;
				}

			// Send PostSB packet.
			PokerPostBlindPacket* p		= new PokerPostBlindPacket();
			p->m_nTableId				= m_info.m_nTableId;
			p->m_bPostBB				= true; // Post BigBlind
			p->m_cSeat					= (char)nBBSeat;
			p->m_shActionSec			= (short)m_nPostBlindDelay;

			ClientConnection* pConn = pBBSeatInfo->GetConnection();
			m_lockObjects.Unlock(); // Unlock

			m_pServer->SendPacketQueued(pConn, p);

			// Add parameters.
			m_gameThread.AddParam(_T("PACKET"), p);
			m_gameThread.AddParam(_T("CONN"),	pConn);
			// Send active seat packet.
			SendSetActiveSeat(nBBSeat, m_nPostBlindDelay);
			// Reset sync event.
			m_gameThread.SetSyncEvent(false);
			return true;
			}
		m_lockObjects.Unlock(); // Unlock
		}
	return false;
	}

bool
PokerGameController::SendShowMuckCardsRequest(int nSeat, bool& bResult){
	bResult							= false;
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeatInfo	= m_info.GetSeatInfo(nSeat);
	if( pSeatInfo ){
		if( pSeatInfo->GetAutoMuck() || pSeatInfo->GetCardsShowStatus() ){
			m_lockObjects.Unlock(); // Unlock
			bResult = true;
			return true;
			}

		// Send ShowMuckCards packet.
		PokerShowMuckCardsPacket* p	= new PokerShowMuckCardsPacket();
		p->m_nTableId					= m_info.m_nTableId;
		p->m_cSeat						= (char)nSeat;
		p->m_shActionSec				= (short)m_nShowMuckDelay;

		ClientConnection* pConn = pSeatInfo->GetConnection();
		m_lockObjects.Unlock(); // Unlock

		// Add parameters.
		m_gameThread.AddParam(_T("PACKET"), p);
		m_gameThread.AddParam(_T("CONN"),	pConn);
		// Reset sync event.
		m_gameThread.SetSyncEvent(false);
		// Send packet.
		m_pServer->SendPacketQueued(pConn, p);
		// Send active seat packet.
		SendSetActiveSeat(nSeat, m_nShowMuckDelay);
		return true;
		}
	m_lockObjects.Unlock(); // Unlock
	return false;
	}

bool
PokerGameController::SendSeatStatusChange(int nSeat, PokerSeatStatus status){
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeatInfo	= m_info.GetSeatInfo(nSeat);
	if( pSeatInfo ){
		pSeatInfo->SetStatus(status);
		}

	// Send SetSeatStatus packet.
	PokerSetSeatStatusPacket packet;
	packet.m_nTableId				= m_info.m_nTableId;
	packet.m_cSeat					= (char)nSeat;
	packet.m_status					= (char)status;
	m_lockObjects.Unlock(); // Unlock

	// Send packet.
	m_pServer->SendPacketByTable(packet.m_nTableId, &packet);
	return true;
	}

bool
PokerGameController::SendSetActiveSeat(int nActiveSeat, int nRegularTimeSec, int nExtraTimeSec /*= 0*/, int nTimeBankSec /*= 0*/){
	m_lockObjects.Lock();	// Lock
	int nTableId = m_info.m_nTableId;
	m_info.m_cActiveSeat = (char)nActiveSeat;
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
PokerGameController::SendSeatActionPacket(int nSeat, PokerActionTypes action, PokerSeatStatus status, int nAmount, bool bSetSitOutNextHand /*= false*/){
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

	if( bSetSitOutNextHand ){
		m_info.SetSitOutNextHand(nSeat, true);
		}

	packet->m_cSeat				= (char)nSeat;
	packet->m_cSeatReceiver		= (char)-1;
	packet->m_cActionType		= action;
	packet->m_nAmount			= nAmount;
	packet->m_dtAction			= COleDateTime::GetCurrentTime();
	packet->m_bRoundFinished	= false;

	// Retrieve in-play seats to send packet one-by-one.
	CPtrArray arrSeats;
	m_info.GetSeats(arrSeats);
	// Add chat message.
	m_info.AddChatMessage	(packet->m_dtAction, nSeat, (PokerActionTypes)packet->m_cActionType, packet->m_nAmount);
	m_lockObjects.Unlock(); // Unlock

	// Determine hand cards open status. {{
	if( m_info.m_round > PokerRoundType::No ){
		if( action == PokerActionTypes::ShowCards ){
			PokerHandCards* pHandCards = new PokerHandCards();
			if( m_info.GetSeatHandCards(nSeat, pHandCards) )
				packet->m_listCardsShown.Add(pHandCards);

			// Send packet to everyone.
			packet->m_cSeatReceiver	= (char)-1;
			m_pServer->SendPacketByTable(packet->m_nTableId, packet, false);
			delete packet;
			// }}
			return true;
			}
		else
		if( action == PokerActionTypes::Muck ){
			// Send packet to everyone.
			packet->m_cSeatReceiver	= (char)-1;
			m_pServer->SendPacketByTable(packet->m_nTableId, packet, false);
			delete packet;
			// }}
			return true;
			}
		else{
			int nSeatAsk = -1;
			bool bRoundEnded = false, bOpenCards = false;
			m_info.GetNextActiveSeat(nSeat, nSeatAsk, bRoundEnded, bOpenCards);
			if( bOpenCards ){
				// Show seat's hand cards.
				m_info.ShowSeatCards(true);
				// Calculate seat best hand card classes.
				m_info.GetSeatsHandCards(&packet->m_listCardsShown, NULL);
				}
			packet->m_bRoundFinished = bRoundEnded;
			}
		}
	// }}
	if( arrSeats.GetCount() > 0 ){
		bSendPreActions = bSendPreActions ? (!packet->m_bRoundFinished) : bSendPreActions;
		// Send packet one by one.
		for(int i=0; i<arrSeats.GetCount(); i++){
			PokerTableSeatInfo* pSeatInfo = (PokerTableSeatInfo*)arrSeats.GetAt(i);
			if( !pSeatInfo || !pSeatInfo->GetConnection() ) continue;

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

					// False unused pre-actions.
					pSeatInfo->m_bPreAction_Call	= false;
					pSeatInfo->m_bPreAction_Fold	= false;
					pSeatInfo->m_bPreAction_CallAny = false;
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

					// False unused pre-actions.
					pSeatInfo->m_bPreAction_Check			= false;
					pSeatInfo->m_bPreAction_CallAnyCheck	= false;
					pSeatInfo->m_bPreAction_CheckFold		= false;
					}
				}

			packet->m_cSeatReceiver = (char)pSeatInfo->GetSeat();
			m_lockObjects.Unlock(); // Unlock

			m_pServer->SendPacketQueued(pSeatInfo->GetConnection(), packet);
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

	if( bSetSitOutNextHand ){
		m_lockObjects.Lock();	// Lock

		// Set option packet.
		PokerSetOptionPacket packet1;
		packet1.m_nTableId	= m_info.m_nTableId;
		packet1.m_cSeat		= (char)nSeat;
		packet1.m_cOption	= (char)PokerTableOption::Option_SitOutNextHand;
		packet1.m_bValue	= true;
		packet1.m_nValue	= 0;
		
		ClientConnection* pConn = m_info.GetConnection(nSeat);
		m_lockObjects.Unlock(); // Unlock
		m_pServer->SendPacketQueued(pConn, &packet1);
		}
	return true;
	}

bool
PokerGameController::IsSeatReservedByConn(int nSeat, ClientConnection* pConn){
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat || pSeat->GetStatus() != PokerSeatStatus::Reserved ){
		m_lockObjects.Unlock(); // Unlock
		return false;
		}
	bool bRet = (pSeat->GetConnection() == pConn);
	m_lockObjects.Unlock(); // Unlock
	return bRet;
	}

bool
PokerGameController::CancelSeatReservation(int nSeat){
	m_lockObjects.Lock(); // Lock
	int		nTimerId	= m_arrSeatReserveTimerIds[nSeat];
	bool	bRet		= Timers::GetInstance()->KillTimer(nTimerId);
	m_arrSeatReserveTimerIds[nSeat] = 0;
	m_lockObjects.Unlock(); // Unlock
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

		PokerSession* pSess = (PokerSession*)pConn->GetSession();
		if( !pSess ){
			RemoveFromWaitList(pConn);
			nCt --;
			}

		UserInfo* pUser = (UserInfo*)pSess->GetUserInfo();
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
		OnReserveSeat(pSess, pConn, packet);
		delete packet;
		}
	m_lockObjects.Unlock(); // Unlock
	}

int
PokerGameController::AddInWaitList(ClientConnection* pConn){
	m_lockObjects.Lock(); // Lock
	int nTableId	= m_info.m_nTableId;
	int nIndex		= m_arrWaitingListConn.IndexOf((__int64)pConn);
	if( nIndex != -1 ){ // Connection already exists into waiting list.
		m_lockObjects.Unlock(); // Unlock
		return nIndex;
		}
	// Add connection to wait list.
	m_listWaitingList.Add(pConn);
	// Add connection into array.
	m_arrWaitingListConn.Add((__int64)pConn);
	nIndex = m_arrWaitingListConn.GetCount() - 1;

	if( pConn ){
		PokerSession* pSess = (PokerSession*)pConn->GetSession();
		if( pSess )
			m_pServer->PlayerAddedInWaitList(pSess, nTableId);
		}
	m_lockObjects.Unlock(); // Unlock
	return nIndex;
	}

bool
PokerGameController::RemoveFromWaitList(ClientConnection* pConn){
	m_lockObjects.Lock();		// Lock
	int nIndex = m_arrWaitingListConn.IndexOf((__int64)pConn);
	if( nIndex != -1 ){ // Connection already exists in waiting list.
		if( pConn ){
			PokerSession* pSess = (PokerSession*)pConn->GetSession();
			if( pSess )
				m_pServer->PlayerRemovedFromWaitList(pSess, m_info.m_nTableId);
			}

		m_arrWaitingListConn.Delete(nIndex);
		m_listWaitingList	.Remove(pConn, false);
		m_lockObjects.Unlock(); // Unlock
		return true;
		}
	m_lockObjects.Unlock();		// Unlock
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