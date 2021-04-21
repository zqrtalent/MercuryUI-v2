#include "stdafx.h"
#include "PokerServer.h"
#include "PokerGameBase.h"
#include "../GameServer/ClientConnection.h"
#include "HoldemWinnerCalculator.h"
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

PokerCashTableController::PokerCashTableController(PokerServer* pServer /*= NULL*/, PokerGameBase* pGameBase /*= NULL*/) : 
	PokerTableController(pServer, pGameBase)
	{
	if( !pServer ){ m_pServer = NULL; return; }
	// Create instance.
	HoldemWinnerCalculator::GetInstance();

	m_pServer					= pServer;
	m_pGameBase					= pGameBase;
	m_bSyncState				= false;

	m_nHandStartDelay			= 3;
	m_nPostBlindDelay			= 5;
	m_nStartRoundDelay			= 3;
	m_nRegularTimeSec			= 10;
	m_nExtraTimeSec				= 3;
	m_nShowMuckDelay			= 5;
	m_nReserveSeatDelay			= 5;
	m_nLeaveSitoutedDelay		= 30;
	m_nLeaveZeroBalancedSeatDelay= 10;
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
	m_nHandId					= -1;

	// Not destroy objects when remove element from array.
	m_listWaitingList.SetDestroyObjects(false);

	memset(m_arrSeatReserveTimerIds, 0, sizeof(m_arrSeatReserveTimerIds));
	memset(m_arrLeaveSeatTimerIds, 0, sizeof(m_arrLeaveSeatTimerIds));

	// Start game thread.
	// Start only when it will be neccessary.
	//GThread_Start();
	} 

BEGIN_DECLARE_VAR(PokerCashTableController, Serializable)
	DECLARE_VAR(_T("PokerCashTableController"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableInfo"),			_T(""),		VariableType::VT_None, offsetof(PokerCashTableController,m_info), false, true, false)
END_DECLARE_VAR()

PokerCashTableController::~PokerCashTableController(){
	if( !m_pServer ) return;
	// Stop game thread.
	GThread_Stop();
	// Kill all timers.
	KillAllTimers();
	// Destroy waiting list.
	m_listWaitingList.RemoveAll(false);
	}

void
PokerCashTableController::SendStopSignal(){
	PokerTableController::SendStopSignal();
	// Send stop event to thread.
	m_gameThread.SetStopEvent(true);
	}

void
PokerCashTableController::OnOpenTable(PokerSession* pSess, ClientConnection* pConn, PokerOpenTablePacket* p){
	ASSERT( pConn );
	ASSERT(p->m_nTableId == m_info.m_nTableId);
	// Allow observer open table. {{
	int			nUserId		= pSess->GetUserId();
	_String		sUserName	= pSess->GetUserName();
	// }}

	if( m_bTableIsClosed ){
		// Send table closed packet only.
		PokerTableClosedPacket packet;
		packet.m_nTableId		= m_info.m_nTableId;
		packet.m_nTournamentId	= m_info.m_nTournamentId;
		m_pServer->SendPacket(pConn, &packet);
		return;
		}

	m_lockObjects.Lock();	// Lock
	int nSeatOccupy = -1;
	bool bSeatOrJoin = false;
	if( p->m_bSeatOrJoinWaitList ){
		if( p->m_cSeat == (char)-1 )
			nSeatOccupy = m_info.GetFirstEmptySeat();
		else
			nSeatOccupy = (m_info.GetSeatStatus((int)p->m_cSeat) == PokerSeatStatus::Empty) ? p->m_cSeat : -1;
		bSeatOrJoin = true;
		}
	p->m_nHandId		= m_nHandId;
	p->m_cSeat			= (char)-1;
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

		int nSeat					= pSeatInfo->GetSeat();
		p->m_cSeat					= (char)nSeat;
		p->m_bSeatOrJoinWaitList	= false;
		nSeatOccupy					= -1;
		bSeatOrJoin					= false;
		// Bring seat online.
		if( pSeatInfo->IsOffline() ){
			SetSeatOffline(nSeat, true);
			}
		// Set seat session.
		pSeatInfo->SetSession(pConn->GetSession(false), p->m_nTableId, 0);
		// Attach connection to seat.
		pSess->SeatAt(p->m_nTableId, nSeat, 0);
		}
	else{
		p->m_cSeat					= (char)-1;
		p->m_bSeatOrJoinWaitList	= false;
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

	// Retrieve statistic info.
	m_pGameBase->GetTableStatInfoByUser(nUserId, p->m_nTableId, p);
	// Send reply.
	m_pServer->SendPacket		(pConn, p);
	p->m_info.RemoveAll				(false);
	if( bSeatOrJoin && nUserId > 0 ){
		// Reserve seat.
		if( nSeatOccupy != -1 ){
			PokerReserveSeatPacket* packet	= new PokerReserveSeatPacket();
			packet->m_nSessionId			= p->m_nSessionId;
			packet->m_nTableId				= p->m_nTableId;
			packet->m_cSeat					= (char)nSeatOccupy;
			packet->m_sUserName				= sUserName;
			OnReserveSeat(pSess, pConn, packet);
			delete packet;
			}
		else{ // join waiting list.
			PokerJoinWaitingListPacket* packet = new PokerJoinWaitingListPacket;
			packet->m_nSessionId			= p->m_nSessionId;
			packet->m_nTableId				= p->m_nTableId;
			OnJoinWaitingList(pSess, pConn, packet);
			delete packet;
			}
		}

	// Repeat last sent action packet.
	if( pPacketRepeat ){
		m_pServer->SendPacket(pConn, pPacketRepeat);
		delete pPacketRepeat;
		}
	}

void
PokerCashTableController::OnLeaveTable(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTablePacket* p){
	// If seat is taken by connection. {{
	m_lockObjects.Lock(); // Lock
	bool bOpened = false;
	int nSeatIndex = pSess->GetSeatIndex(m_info.m_nTableId, bOpened, m_info.m_nTournamentId);
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
	m_pServer->SendPacket(pConn, p);
	}

void
PokerCashTableController::OnSeatAt(PokerSession* pSess, ClientConnection* pConn, PokerSeatAtTablePacket* p){
//	ASSERT( pConn );
	int nSeatIndex = (int)p->m_cSeatIndex;
	if( !p || (nSeatIndex < 0 || nSeatIndex >= m_info.m_cMaxPlayersCt) )
		return;

	// Check for user.
	// Check for amount.
	// Check seat status if already taken seat.
	UserInfo user;
	if( !pSess->CopyUserInfo(user) ) return;

	PokerAvatar*		pAvatarInfo = user.GetAvatarInfo();
	m_lockObjects.Lock(); // Lock
	bool				bReserved	= pConn ? IsSeatReservedByConn(nSeatIndex, pConn) : false;
	bool				bSuccess	= false;
	PokerTableSeatInfo* pSeatInfo	= m_info.GetSeatInfo(nSeatIndex);
	PokerSeatStatus		status		= pSeatInfo ? pSeatInfo->GetStatus() : PokerSeatStatus::Empty;

	if( !pSeatInfo || (pSeatInfo && (status == PokerSeatStatus::Empty || (status == PokerSeatStatus::Reserved && bReserved))) ){
		if( !pSeatInfo )
			pSeatInfo = new PokerTableSeatInfo();

		// Kill seat reservation timer. {{
		if( m_arrSeatReserveTimerIds[nSeatIndex] ){
			Timers::GetInstance()->KillTimer(m_arrSeatReserveTimerIds[nSeatIndex]);
			m_arrSeatReserveTimerIds[nSeatIndex] = 0;
			}
		// }}
		
		pSeatInfo->SetIpAddress		(pConn ? pConn->GetRemoteIp() : 0);
		pSeatInfo->SetSeat			(nSeatIndex);
		pSeatInfo->SetBalance		(p->m_nChipAmount);
		pSeatInfo->SetAddedAmount	(p->m_nChipAmount);
		pSeatInfo->SetCards			( -1, -1, -1, -1);
		pSeatInfo->SetRankLevel		(user.m_shRankLevel);
//		pSeatInfo->SetRankName		(_T("Bronze"));
		pSeatInfo->SetUserId		(user.m_nUserId);

		if( !user.m_bAvatarIsBlocked && user.m_nAvatarId > 0 ){
			pSeatInfo->SetAvatarId(user.m_nAvatarId);
			p->m_sAvatarFileName = user.m_sAvatarFileName;
			}
		else{
			pSeatInfo->SetAvatarId(0);
			p->m_sAvatarFileName.Empty();
			}
		
		pSeatInfo->SetUserName		(user.m_sUserName);
		pSeatInfo->SetActionName	(_T(""));

		if( (m_info.m_gameStatus == PokerGameStatus::NotStarted && m_info.GetSittedCt() > 1) ||
			(m_info.m_gameStatus != PokerGameStatus::NotStarted)
			)
			pSeatInfo->SetStatus(PokerSeatStatus::WaitingForBB);
		else
			pSeatInfo->SetStatus(PokerSeatStatus::Sitted);

		pSeatInfo->SetTimeBankSeconds	(m_nTimeBankSec);
		pSeatInfo->SetSession			(pConn ? pConn->GetSession(false) : pSess, p->m_nTableId, p->m_nTournamentId);
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
			if( !m_pGameBase->PlayerSeated(this, m_pParam, &m_info, nSeatIndex, p->m_nChipAmount, pSeatInfo) ){
				m_info.EmptySeat(nSeatIndex);
				m_lockObjects.Unlock(); // Unlock
				// Send seat status change.
				SendSeatStatusChange(nSeatIndex, PokerSeatStatus::Empty);
				return;
				}
			
			// Call seat balance change callback
			m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, pSess, m_info.m_nTableId, m_nCurrencyId, nSeatIndex, p->m_nChipAmount, 0);
			}
		}
	m_lockObjects.Unlock(); // Unlock

	if( bSuccess ){
		ASSERT(m_pServer);
//		p->m_avatarData.m_nId							= pAvatarInfo->m_nId;
//		p->m_avatarData.m_recStatus						= 0;
//		if( pAvatarInfo ){
//			p->m_avatarData.m_sAvatarFileName			= pAvatarInfo->m_sAvatarFileName;
////			p->m_avatarData.m_binImage.m_dwDataLength	= pAvatarInfo->m_binImage.m_dwDataLength;
////			p->m_avatarData.m_binImage.m_hData			= pAvatarInfo->m_binImage.m_hData;
//			p->m_avatarData.m_binImage.m_dwDataLength	= 0L;
//			p->m_avatarData.m_binImage.m_hData			= NULL;
//			}
//		else{
//			p->m_avatarData.m_binImage.m_dwDataLength	= 0L;
//			p->m_avatarData.m_binImage.m_hData			= NULL;
//			}

		p->m_nTournamentId = m_info.m_nTournamentId;
		// Send seat at packet to everyone.
		m_pServer->SendPacketByTable(m_info.m_nTableId, m_info.m_nTournamentId, p);
		// Zero avatar binary data.
		//p->m_avatarData.m_binImage.m_dwDataLength	= 0L;
		//p->m_avatarData.m_binImage.m_hData			= NULL;

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
PokerCashTableController::OnReserveSeat(PokerSession* pSess, ClientConnection* pConn, PokerReserveSeatPacket* p){
	ASSERT( pConn );
	UserInfo user;
	if( !pSess->CopyUserInfo(user) ) return;
	int		nUserId		= user.m_nUserId;
	_String	sUserName	= user.m_sUserName;
	if( nUserId <= 0 ) return;

	DWORD dwRemoteIp = pConn->GetRemoteIp();
	m_lockObjects.Lock(); // Lock
	int nCurrencyId = (int)m_info.m_cCurrencyId;
	UINT nMinBuyIn	= m_info.m_nMinBuyIn;
	UINT nMaxBuyIn	= m_info.m_nMaxBuyIn;

	int nSeatIp = -1;
	if( m_info.GetSeatWithIp(dwRemoteIp, nSeatIp) ){
		if( !m_pServer->IsAllowedMultiUserIp(dwRemoteIp) && !m_pServer->IsDontRestrictIPUser(sUserName) ){
			m_lockObjects.Unlock(); // Unlock
			p->m_nErrorCode = ES_ERROR_IP_ALREADY_EXISTS;
			// Send reply.
			m_pServer->SendPacket(pConn, p);
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
	pInfo->SetUserId			(nUserId);
	pInfo->SetUserName			(sUserName);
	pInfo->SetActionName		(_T(""));
	pInfo->SetStatus			(PokerSeatStatus::Reserved);
	pInfo->SetTimeBankSeconds	(m_nTimeBankSec);
	pInfo->SetSession			(pConn->GetSession(false), p->m_nTableId, 0);
	m_info.SetSeatInfo			(nSeat, pInfo);

	// Min buy-in.
	p->m_nAmountMin				= m_info.m_nMinBuyIn;
	// Max buy-in
	p->m_nAmountMax				= m_info.m_nMaxBuyIn;
	m_lockObjects.Unlock(); // Unlock

	// Retrieve min buy in for current table. {{
	INT_CHIPS nBalance = -1;
	if( m_pGameBase->GetSeatMinBuyIn(nUserId, p->m_nTableId, nCurrencyId, p->m_nAmountMin, nBalance) ){
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
			m_pServer->SendPacket(pConn, p);
			return;
			}

		p->m_nBalance		= nBalance;
		p->m_nAmountMax		= max(p->m_nAmountMin, min(nMaxBuyIn,nBalance));
		}
	else{
		if( nBalance == -1 )
			nBalance		= pSess->GetBalance();
		p->m_nBalance		= nBalance;
		p->m_nAmountMin		= m_info.m_nMinBuyIn;
		p->m_nAmountMax		= m_info.m_nMaxBuyIn;
		p->m_nErrorCode		= ES_ERROR_INSUFFICIENT_AMOUNT;
		m_info.SetSeatInfo(nSeat, NULL);
		m_lockObjects.Unlock(); // Unlock
		m_pServer->SendPacket(pConn, p);
		return;
		}
	// }}

	// Send response to seat reserver. {{
	p->m_sUserName				= sUserName;
	p->m_nBalance				= p->m_nBalance;
	p->m_shReservationTimeSec	= (short)m_nReserveSeatDelay;
	p->m_bReservationExpired	= false;

	// Set avatar info.
	m_info.SetAvatarInfo(nSeat, user.m_pAvatarInfo);
	m_pServer->SendPacket(pConn, p);
	// }}

	// Send seat seat status packet. {{
	PokerSeatStatusChangedPacket p1;
	p1.m_nTableId		= p->m_nTableId;
	p1.m_nTournamentId	= 0;
	p1.m_cSeat			= (char)nSeat;
	p1.m_sUserName		= sUserName;
	p1.m_status			= (char)PokerSeatStatus::Reserved;
	if( user.m_pAvatarInfo )
		p1.m_seatAvatar.Add(user.m_pAvatarInfo);
	m_pServer->SendPacketByTable(p1.m_nTableId, m_info.m_nTournamentId, &p1, false);
	p1.m_seatAvatar.RemoveAll(false);
	// }}

	m_lockObjects.Lock();	// Lock
	// Start reservation timer.
	StartSeatReservationTimer(nSeat);

	// Call event method.
	m_pGameBase->PlayerReservedSeat(this, m_pParam, pSess, p->m_nTableId, nSeat);
	m_lockObjects.Unlock(); // Unlock
	}

void
PokerCashTableController::OnCancelReservation(PokerSession* pSess, ClientConnection* pConn, PokerCancelReservationPacket* p){
	m_lockObjects.Lock();		// Lock
	int nSeat		= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nSeat);
	if( !pSeatInfo || pSeatInfo->GetStatus() != PokerSeatStatus::Reserved ){
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	_String sUserName	= pSeatInfo->GetUserName(); 
	int		nUserId		= pSeatInfo->GetUserId();
	// Empty seat.
	m_info.EmptySeat(nSeat);
	// Call event method.
	m_pGameBase->PlayerCanceledReservation(this, m_pParam, pSess, nUserId, p->m_nTableId, nSeat);
	m_lockObjects.Unlock(); // Unlock

	// Send seat status change packet to everyone. {{
	PokerSeatStatusChangedPacket seatStatusChanged;
	seatStatusChanged.m_nTableId		= p->m_nTableId;
	seatStatusChanged.m_nTournamentId	= 0;
	seatStatusChanged.m_cSeat			= (char)nSeat;
	seatStatusChanged.m_status			= (char)PokerSeatStatus::Empty;
	seatStatusChanged.m_sUserName		= sUserName; 
	m_pServer->SendPacketByTable(p->m_nTableId, m_info.m_nTournamentId, &seatStatusChanged);
	// }}
	}

void
PokerCashTableController::OnJoinWaitingList(PokerSession* pSess, ClientConnection* pConn, PokerJoinWaitingListPacket* p){
	int		nUserId		= pSess->GetUserId();
	_String	sUserName	= pSess->GetUserName();
	if( nUserId <= 0 ) return;

	m_lockObjects.Lock();	// Lock
	int nSeatEmpty = m_info.GetFirstEmptySeat();
	if( nSeatEmpty != -1 ){
		PokerReserveSeatPacket packet;
		packet.m_nSessionId			= p->m_nSessionId;
		packet.m_nTableId			= p->m_nTableId;
		packet.m_cSeat				= (char)nSeatEmpty;
		packet.m_sUserName			= sUserName;
		m_lockObjects.Unlock(); // Unlock

		OnReserveSeat(pSess, pConn, &packet);
		return;
		}
	else{
		// Wait index number.
		p->m_shIndex = AddInWaitList(pConn);
		}
	m_lockObjects.Unlock(); // Unlock
	// Send reply packet.
	m_pServer->SendPacket(pConn, p);
	}

void
PokerCashTableController::OnAddChips(PokerSession* pSess, ClientConnection* pConn, PokerAddChipsPacket* p){
	int		nUserId		= pSess->GetUserId();
	_String	sUserName	= pSess->GetUserName();
	if( nUserId <= 0 ) return;

	int					nSeat	= (int)p->m_cSeat;
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty )
		return;

	if( p->m_bPrepareToAdd ){
		INT_CHIPS nSeatBalance	= pSeat->GetBalance();
		INT_CHIPS nBalance		= 0;

		// Play money or real money.
		nBalance			= (m_moneyType == MoneyType::Real) ? pSess->GetBalance() : pSess->GetPlayBalance();
		p->m_nBalance		= nBalance;
		p->m_nMin			= min(m_info.m_nMinBuyIn, nBalance);
		p->m_nMax			= min(m_info.m_nMaxBuyIn, nBalance);

		INT_CHIPS nMaxAllowed		= (m_info.m_nMaxBuyIn - nSeatBalance);
		INT_CHIPS	nMinAllowed		= (m_info.m_nMinBuyIn - nSeatBalance);
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
		m_pServer->SendPacket(pConn, p);
		}
	else{
		if( pSeat->GetStatus() == PokerSeatStatus::Reserved )
			return;
		INT_CHIPS nSeatBalance = AddChips(nSeat, p->m_nAmount);
		if( nSeatBalance > 0 ){
			SendSeatActionPacket(nSeat, PokerActionTypes::AddChips, PokerSeatStatus::Sitted, p->m_nAmount);
			}
		else{
			if( nSeatBalance < 0 ){
				p->m_nErrorCode	= ES_ERROR_CHIPS_WILL_ADD_AFTER_HANDFINISH;
				m_pServer->SendPacket(pConn, p); // Send reply to notify abbout chips add status.
				}
			}
		}
	}

void
PokerCashTableController::OnAction(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p, bool bPreAction /*= false*/){
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

	bool bValidAction = false;
	if( pSent ){
		if( pSent->m_nCode == PacketCodes::_PokerAct ){
			for( int i=0; i<pSent->m_actions.GetCount(); i++){
				PokerAction* pAction = pSent->m_actions.GetAt(i);
				if( pAction && pAction->m_cActionType == p->m_cActionType ){
					// Validate received amount.
					if( p->m_nAmount < pAction->m_nAmountMin || p->m_nAmount > pAction->m_nAmountMax ){
						m_gameThread.Unlock();
						return;
						}
					bValidAction = true;
					break;
					}
				}
			}
		else// Validate response of post blind request.
			if( pSent->m_nCode == PacketCodes::_PokerPostBlind ){
				bValidAction = (p->m_cActionType == PokerActionTypes::PostSB || 
								p->m_cActionType == PokerActionTypes::PostBB || 
								p->m_cActionType == PokerActionTypes::PostBB_Plus_SB || 
								p->m_cActionType == PokerActionTypes::SitOut);
				}
		}
	

	if( bValidAction && pRecv && pRecv->m_nCode == p->m_nCode ){
		// Copy received packet.
		p->Copy(pRecv);
		// Packet received.
		m_gameThread.SetSyncEvent(true);
		}
	m_gameThread.Unlock();
	}

void
PokerCashTableController::OnShowMuckCards(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p){
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
PokerCashTableController::OnTableInfo(PokerSession* pSess, ClientConnection* pConn, PokerTableInfoPacket* p){
	ASSERT(pConn);
	ASSERT(p->m_nTableId == m_info.m_nTableId);

	p->m_nHandId = m_nHandId;
	p->m_info.RemoveAll	();
	m_lockObjects.Lock(); // Lock
	p->m_info.Add		(&m_info);

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
	p->m_info.RemoveAll		(false);
	}

void
PokerCashTableController::OnSetOption(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p){
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
					((pSeat->GetStatus() == PokerSeatStatus::Sitted && m_info.m_gameStatus == PokerGameStatus::NotStarted) ||
					((pSeat->GetStatus() == PokerSeatStatus::WaitingForBB || 
					pSeat->GetStatus() == PokerSeatStatus::PayBBAndPlay || 
					pSeat->GetStatus() == PokerSeatStatus::CheckForPositionToStart) && m_info.m_gameStatus == PokerGameStatus::Started))
					){
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
				pSeat->FalsePreActions();
				pSeat->m_bPreAction_Call		= p->m_bValue;
				pSeat->m_nPreAction_CallAmount	= p->m_nValue;
				break;
				}
			
			case PokerTableOption::Option_CallAny:{
				pSeat->FalsePreActions();
				pSeat->m_bPreAction_CallAny = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_CallAnyCheck:{
				pSeat->FalsePreActions();
				pSeat->m_bPreAction_CallAnyCheck = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_Check:{
				pSeat->FalsePreActions();
				pSeat->m_bPreAction_Check = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_CheckFold:{
				pSeat->FalsePreActions();
				pSeat->m_bPreAction_CheckFold = p->m_bValue;
				break;
				}
			case PokerTableOption::Option_Fold:{
				pSeat->FalsePreActions();
				pSeat->m_bPreAction_Fold = p->m_bValue;
				break;
				}
			};
		}
	m_lockObjects.Unlock();		// Unlock
	}

void
PokerCashTableController::OnChatMessage(PokerSession* pSess, ClientConnection* pConn, PokerChatMessagePacket* p){
	// Send chat message.
	p->m_message.m_dtTime = COleDateTime::GetCurrentTime();
	m_pServer->SendPacketByTable(p->m_nTableId, m_info.m_nTournamentId, p);
	}

void
PokerCashTableController::OnReturnBack(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p){
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
		packetAddChips.m_bPrepareToAdd	= true;
		packetAddChips.m_cSeat			= (char)nSeat;
		packetAddChips.m_nAmount		= 0;
		packetAddChips.m_nSessionId		= pSess->GetSessionId();
		packetAddChips.m_nTableId		= p->m_nTableId;
		OnAddChips(pSess, pConn, &packetAddChips);
		m_lockObjects.Unlock(); // Unlock
		return;
		}

	bool			bStart			= (m_info.m_gameStatus == PokerGameStatus::NotStarted && (m_info.GetSittedCt() >= 1 || (GThread_IsPaused() || !GThread_IsStarted())));
	PokerSeatStatus statusBefore	= !bStart ? pSeat->GetStatusBeforeSitout() : PokerSeatStatus::Empty;
	PokerSeatStatus status			= pSeat->GetStatus();
	bool			bSuccess		= false;
	bool			bSitOutNextHand	= pSeat->GetSitOutNextHand();
	bool			bSitOutNextBB	= pSeat->GetSitOutNextBB();

	if( status == PokerSeatStatus::SittedOut ){
		if( pSeat->GetSitoutButInPlayFlag() )
			pSeat->SetStatus(PokerSeatStatus::InPlay);
		else{
			if( statusBefore == PokerSeatStatus::WaitingForBB )
				pSeat->SetStatus(PokerSeatStatus::WaitingForBB);
			else
				if( statusBefore == PokerSeatStatus::PayBBAndPlay )
					pSeat->SetStatus(PokerSeatStatus::PayBBAndPlay);
				else{
					pSeat->SetStatus(PokerSeatStatus::CheckForPositionToStart/*bStart ? PokerSeatStatus::Sitted : PokerSeatStatus::CheckForPositionToStart*/);
					}
			}
		bSuccess = true;
		// Clear SitOutNextHand option.
		if( bSitOutNextHand )
			pSeat->SetSitOutNextHand(false);

		// Clear SitOutNextHand / SitOutNextBB option.
		if( bSitOutNextHand )
			pSeat->SetSitOutNextHand(false);
		if( bSitOutNextBB )
			pSeat->SetSitOutNextBB(false);
		}
	else
		bStart = false;
	
	if( bSuccess ){
		// Cancel leave seat timer.
		StopLeaveSeatTimer(nSeat);
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

		if( bSitOutNextBB ){
			// Clear SitOutNextBB option.
			packet1.m_nTableId		= m_info.m_nTableId;
			packet1.m_nTournamentId	= m_info.m_nTournamentId;
			packet1.m_cSeat			= (char)nSeat;
			packet1.m_cOption		= (char)PokerTableOption::Option_SitOutNextBB;
			packet1.m_bValue		= false;
			packet1.m_nValue		= 0;
			m_pServer->SendPacket(pConn, &packet1);
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
PokerCashTableController::OnConnectionDropped(PokerSession* pSess, ClientConnection* pConn, int nSeat){
	if( !pConn || !pSess ) return;
	m_lockObjects.Lock(); // Lock
	if( nSeat > -1 ){
		if( IsSeatReservedByConn(nSeat, pConn) )
			OnReservationTimerElapsed(nSeat);
		else{
			if( (m_info.m_gameStatus != PokerGameStatus::NotStarted) ){
				// When game thread is started and not paused(waiting for start game event).
				if( GThread_IsStarted() && !GThread_IsPaused() )
					SetSeatOffline(nSeat, false);
				else
					ForceSeatToLeaveTable(nSeat);
				}
			else
				ForceSeatToLeaveTable(nSeat);
			}
		}
	else
		RemoveFromWaitList(pConn);
	m_lockObjects.Unlock(); // Unlock
	}

void
PokerCashTableController::OnGameSessionExpired(PokerSession* pSess, ClientConnection* pConn,  bool& bCancel){
	if( !pConn || !pSess ) return; 
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatBySession(pSess);
	if( pSeat ){
		if( (!m_info.m_bTournamentTable) && (m_info.m_gameStatus != PokerGameStatus::NotStarted) ){
			// When game thread is started and not paused(waiting for start game event).
			if( GThread_IsStarted() && !GThread_IsPaused() )
				bCancel = true; // Cancel session expiration.
			}
		else
		if( m_info.m_bTournamentTable )
			bCancel = true;

		if( !bCancel )
			EmptySeat(pSeat->GetSeat());
		}
	m_lockObjects.Unlock(); // Unlock
	}

bool
PokerCashTableController::OnGameSessionRestored(PokerSession* pSess, ClientConnection* pConn, int nUserId){
	/* Let client side open tables which was previously opened by the same session.
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
	p->m_nTournamentId			= 0;
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
	m_pServer->SendPacket(pConn, p);
	p->m_info.RemoveAll(true);

	// Reply last sent message.
	if( pPacketRepeat ){
		m_pServer->SendPacket(pConn, pPacketRepeat);
		delete pPacketRepeat;
		}*/
	return true;
	}

void
PokerCashTableController::OnGThread_Terminated(){
	m_bTableIsClosed		= true;

	// Send table closed packet.
	PokerTableClosedPacket packet;
	packet.m_nTableId		= m_info.m_nTableId;
	packet.m_nTournamentId	= m_info.m_nTournamentId;
	m_pServer->SendPacketByTable(packet.m_nTableId, packet.m_nTournamentId, &packet);
	}

int
PokerCashTableController::PerformAddChipsAction(){
	m_lockObjects.Lock();		// Lock
	int nLoop = 0, nCt = m_arrAddChips.GetCount(), nSuccessCt = 0;
	AutoSortedArrayInt64 arrAddChips;
	arrAddChips.Copy(&m_arrAddChips);
	m_arrAddChips.DeleteAll();  // Clear Add chips array.
	m_lockObjects.Unlock();		// Unlock

	int nSeatVal = 0, nSeat = 0, nChipsType = 0;
	while( nLoop < nCt ){
		nSeatVal		= (int)arrAddChips.Get(nLoop);
		nSeat			= (nSeatVal & 0xFF);
		nChipsType		= ((nSeatVal >> 8) & 0xFF);
		int nAmount		= (int)arrAddChips.GetData(nLoop);
		if( AddChips(nSeat, nAmount, (PokerTableController::AddChipsType)nChipsType) > 0 ){
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

INT_CHIPS
PokerCashTableController::AddChips(int nSeat, INT_CHIPS nAmount, AddChipsType chipsType /*= AddChipsType::AddChips_BalanceAmount*/){
	m_lockObjects.Lock();		// Lock
	if( nSeat < 0 || nSeat >= m_info.m_cMaxPlayersCt ){
		m_lockObjects.Unlock();		// Unlock
		return 0;
		}

	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat || (!pSeat->GetConnection() && chipsType == AddChipsType::AddChips_BalanceAmount) || pSeat->GetStatus() == PokerSeatStatus::Empty ){
		m_lockObjects.Unlock();		// Unlock
		return 0;
		}

	if( m_info.m_gameStatus == PokerGameStatus::NotStarted || pSeat->GetStatus() == PokerSeatStatus::SittedOut ){
		ClientConnection*	pConn	= pSeat->GetConnection();
		PokerSession*		pSess	= pConn ? (PokerSession*)pConn->GetSession(true) : NULL;
		int					nUserId	= 0, nTableId = m_info.m_nTableId;
		_String				sUserName;

		// Validate session.
		if( !pSess ){
			m_lockObjects.Unlock();		// Unlock
			return 0;
			}

		nUserId	= pSess->GetUserId();
		if( nUserId <= 0 ){
			if( pSess ) SafePointer::Release(pSess);
			m_lockObjects.Unlock();		// Unlock
			return 0;
			}

		// Perform chips adding by balance amount.
		if( chipsType == AddChipsType::AddChips_BalanceAmount ){
			if( !pConn ){
				if( pSess ) SafePointer::Release(pSess);
				m_lockObjects.Unlock();		// Unlock
				return 0;
				}

			// Set amount value to maximum acceptable.
			INT_CHIPS nAmountAddMax	= (m_info.m_nMaxBuyIn - pSeat->GetBalance());
			nAmount					= min(nAmount, nAmountAddMax);

			// Validate add amount.
			if( nAmount <= 0 ){
				if( pSess ) SafePointer::Release(pSess);
				m_lockObjects.Unlock();		// Unlock
				return 0;
				}

			// Stop leave seat timer.
			StopLeaveSeatTimer(nSeat);
			m_lockObjects.Unlock();		// Unlock

			// User add chips by balance.
			if( m_pGameBase->UserAddChips(pSess, this, m_pParam, nTableId, nSeat, nAmount, (int)m_info.m_cCurrencyId) ){
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
				m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, pSess, nTableId, m_nCurrencyId, nSeat, pSeat->GetBalance() + nAmount, pSeat->GetBalance());

				if( pSess ) SafePointer::Release(pSess);
				pSeat->Add_AddedAmount	(nAmount);
				pSeat->SetBalance		(pSeat->GetBalance() + nAmount);
				INT_CHIPS nBalance = pSeat->GetBalance();
				m_lockObjects.Unlock();		// Unlock
				return nBalance;
				}
			}
		else
		// Add rebuy chips.
		if( chipsType == AddChipsType::AddChips_Rebuy || chipsType == AddChipsType::AddChips_RebuyVPPoints ){
			m_lockObjects.Unlock();	// Unlock

			// User add rebuy chips.
			if( m_pGameBase->UserAddRebuyChips(pSess, this, m_pParam, nTableId, pSeat, nAmount, (chipsType == AddChipsType::AddChips_RebuyVPPoints)) ){
				m_lockObjects.Lock();		// Lock
				// Call seat balance change callback
				m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, pSess, nTableId, m_nCurrencyId, nSeat, (pSeat->GetBalance() + nAmount), pSeat->GetBalance());

				if( pSess ) SafePointer::Release(pSess);
				pSeat->Add_AddedAmount	(nAmount);
				pSeat->SetBalance		(pSeat->GetBalance() + nAmount);
				INT_CHIPS nBalance = pSeat->GetBalance();
				m_lockObjects.Unlock();		// Unlock
				return nBalance;
				}
			}
		else
		// Add addon chips.
		if( chipsType == AddChipsType::AddChips_Addon || chipsType == AddChipsType::AddChips_AddonVPPoints ){
			m_lockObjects.Unlock();	// Unlock

			// User add addon chips.
			if( m_pGameBase->UserAddAddonChips(pSess, this, m_pParam, nTableId, pSeat, nAmount, (chipsType == AddChipsType::AddChips_AddonVPPoints)) ){
				m_lockObjects.Lock();		// Lock
				// Call seat balance change callback
				m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, pSess, nTableId, m_nCurrencyId, nSeat, (pSeat->GetBalance() + nAmount), pSeat->GetBalance());
				if( pSess ) SafePointer::Release(pSess);
				pSeat->Add_AddedAmount	(nAmount);
				pSeat->SetBalance		(pSeat->GetBalance() + nAmount);
				INT_CHIPS nBalance = pSeat->GetBalance();
				m_lockObjects.Unlock();		// Unlock
				return nBalance;
				}
			}
		else
			m_lockObjects.Unlock();	// Unlock
		if( pSess ) SafePointer::Release(pSess);
		}
	else{
		int nSeatVal = (nSeat & 0xFF) | (int)((chipsType & 0xFF) << 8);
		// Register for future action. (When hand ends.)
		int nIndex = m_arrAddChips.IndexOf((__int64)nSeatVal);
		if( nIndex == -1 )
			m_arrAddChips.Add((__int64)nSeatVal, nAmount);
		else
			m_arrAddChips.Set(nIndex, (__int64)nSeatVal, nAmount);
		m_lockObjects.Unlock();		// Unlock
		return -1; // Chips will be added when hand finishes.
		}
	return 0;
	}

bool
PokerCashTableController::DealCardsAndStartHand(int nDealer, int nSBSeat, int nBBSeat, int nSeatRefusedToPaySB, CDWordArray& arrPayBBSeats, int nTournamentPlayersCt /*= 0*/, int nTournamentTablesCt /*= 0*/){
	CPtrArray listInPlaySeats;
	m_lockObjects.Lock();		// Lock
	int nDealerOld = m_nDealer, nSBSeatOld = m_nSmallBlind, nBBSeatOld = m_nBigBlind;

	// Pay BB seats.
	for( int i=0; i<arrPayBBSeats.GetCount(); i++ ){
		int				nSeat		= (int)arrPayBBSeats[i];
		PokerSeatStatus status		= m_info.GetSeatStatus(nSeat); 
		INT_CHIPS		nBalance	= m_info.GetBalance(nSeat) - m_nBBAmount;
		if( nBalance >= 0 ){
			m_info.SetBalance	(nSeat, nBalance);
			m_info.AddPotAmount	(nSeat, m_nBBAmount);
			// Post pay BB action.
			SendSeatActionPacket(nSeat, PokerActionTypes::PostBB, PokerSeatStatus::Sitted, m_nBBAmount);
			}
		else
			SitOutBySeat(nSeat);
		}

	// Only small blind should be dealer.
	if( nBBSeat == nDealer && nDealer != -1 && nSBSeat != -1 )
		nDealer = nSBSeat;

	m_nDealer		= nDealer;
	m_nSmallBlind	= nSBSeat;
	m_nBigBlind		= nBBSeat;

	int nSBAmount	= m_info.GetSBAmount(), nBBAmount	= m_info.GetBBAmount();
	// Start hand will fail of insufficient players count.
	m_info.GetSeatCtByStatus(PokerSeatStatus::Sitted, &listInPlaySeats);
	m_info.GetSeatCtByStatus(PokerSeatStatus::All_In, &listInPlaySeats);

	// Start tournament hand to sitout users.
	if( m_info.m_bTournamentTable )
		m_info.GetSeatCtByStatus(PokerSeatStatus::SittedOut, &listInPlaySeats);

	// Hand start will fail without BB seat or insufficient players count.
	if( listInPlaySeats.GetCount() < 2 || nBBSeat == -1 ){
		m_info.m_gameStatus = PokerGameStatus::NotStarted;
		if( m_nBigBlind != -1 ){
			INT_CHIPS nSeatTotalPot		= m_info.GetSeatTotalPot(m_nBigBlind);
			INT_CHIPS nBalanceNew		= m_info.GetBalance(m_nBigBlind) + nSeatTotalPot;
			m_info.SetBalance(m_nBigBlind, nBalanceNew);

			PokerActionPacket p;
			p.m_nTableId		= m_info.m_nTableId;
			p.m_nTournamentId	= m_info.m_nTournamentId;
			p.m_nBalance		= nBalanceNew;
			p.m_nPotAmount		= 0;
			p.m_nSeatTotalPot	= 0;
			p.m_nTotalPot		= 0;
			p.m_cSeat			= (char)m_nBigBlind;
			p.m_cSeatReceiver	= (char)-1;
			p.m_cSeatStatus		= (char)m_info.GetSeatStatus(m_nBigBlind);
			p.m_cActionType		= (char)PokerActionTypes::NoAction;
			p.m_nAmount			= 0;
			m_info.ClearPotAmounts();
			m_info.SetTotalPotAmount(0);
			m_lockObjects.Unlock();	// Unlock
			m_pServer->SendPacketByTable(p.m_nTableId, p.m_nTournamentId, &p);
			}
		else
		if( m_nSmallBlind != -1 ){
			INT_CHIPS nSeatTotalPot		= m_info.GetSeatTotalPot(m_nSmallBlind);
			INT_CHIPS nBalanceNew		= m_info.GetBalance(m_nSmallBlind) + nSeatTotalPot;
			m_info.SetBalance(m_nSmallBlind, nBalanceNew);

			PokerActionPacket p;
			p.m_nTableId		= m_info.m_nTableId;
			p.m_nTournamentId	= m_info.m_nTournamentId;
			p.m_nBalance		= nBalanceNew;
			p.m_nPotAmount		= 0;
			p.m_nSeatTotalPot	= 0;
			p.m_nTotalPot		= 0;
			p.m_cSeat			= (char)m_nSmallBlind;
			p.m_cSeatReceiver	= (char)-1;
			p.m_cSeatStatus		= (char)m_info.GetSeatStatus(m_nSmallBlind);
			p.m_cActionType		= (char)PokerActionTypes::NoAction;
			p.m_nAmount			= 0;
			m_info.ClearPotAmounts();
			m_info.SetTotalPotAmount(0);
			m_lockObjects.Unlock();	// Unlock
			m_pServer->SendPacketByTable(p.m_nTableId, p.m_nTournamentId, &p);
			}
		else
			m_lockObjects.Unlock();	// Unlock

		// Restore old positions.
		m_nDealer		= nDealerOld;
		m_nSmallBlind	= nSBSeatOld;
		m_nBigBlind		= nBBSeatOld;
		return false;
		}

	// Pay ante amount.
	int			nAnteSeatCt			= m_info.PayAnte();
	INT_CHIPS	nTotalPotHistory	= (nAnteSeatCt*m_info.m_nAnteAmount);

	// Turn seat status to inplay mode. {{ 
	CPtrArray listSittedOut;
	for(int i=0; i<listInPlaySeats.GetCount(); i++){
		PokerTableSeatInfo* pSeatInPlay = (PokerTableSeatInfo*)listInPlaySeats.GetAt(i);
		ASSERT(pSeatInPlay);
		if( pSeatInPlay->GetStatus() == PokerSeatStatus::SittedOut ){
			pSeatInPlay->SetStatus	((pSeatInPlay->GetBalance() == 0) ? PokerSeatStatus::All_In : PokerSeatStatus::InPlay);
			listSittedOut.Add(pSeatInPlay);
			}
		else
			pSeatInPlay->SetStatus	((pSeatInPlay->GetBalance() == 0) ? PokerSeatStatus::All_In : PokerSeatStatus::InPlay);
		pSeatInPlay->SetLastAction	(PokerActionTypes::NoAction);
		}
	// }}
	
	// Start hand, shuffle and deal cards.
	m_info.StartHand(m_nDealer, m_nSmallBlind, m_nBigBlind, nSeatRefusedToPaySB);
	// Retrieve hand id.
	m_pGameBase->HandStarted(this, m_pParam, &m_info, m_nHandId, m_info.m_nTableId, listInPlaySeats.GetCount(), m_nSBAmount, m_nBBAmount, m_nAnteAmount);

	// Turn back sittedout status.
	for(int i=0; i<listSittedOut.GetCount(); i++){
		PokerTableSeatInfo* pSeattedout = (PokerTableSeatInfo*)listSittedOut.GetAt(i);
		if( pSeattedout )
			pSeattedout->SetStatus(PokerSeatStatus::SittedOut);
		}

	m_info.m_cDealerSeat	= (char)m_nDealer;
	m_info.m_cBBSeat		= (char)m_nBigBlind;
	m_info.m_cSBSeat		= (char)m_nSmallBlind;

	bool bTournamentTable	= (m_info.m_nTournamentId > 0);
	PokerHandStartedPacket* p = new PokerHandStartedPacket();
	p->m_dtAction			= COleDateTime::GetCurrentTime();
	p->m_nTableId			= m_info.m_nTableId;
	p->m_nTournamentId		= m_info.m_nTournamentId;
	p->m_nHandId			= m_nHandId;
	p->m_cBigBlind			= m_info.m_cBBSeat;
	p->m_cSmallBlind		= m_info.m_cSBSeat;
	p->m_cDealer			= m_info.m_cDealerSeat;
	p->m_nTotalPotAmount	= m_info.m_nTotalPotAmount;
	p->m_nMainPotAmount		= m_info.m_nMainPotAmount;
	p->m_nRankIndex			= 0;
	p->m_nPlayersCt			= 0;
	p->m_shTablesCt			= 0;

	// Retrieve seat statuses.
	m_info.GetSeatStatuses(&p->m_listSeatStatus);

	if( m_info.m_nAnteAmount > 0 )
		m_info.GetSeatAmounts(&p->m_seatAmounts);

	int nLoop = 0, nCt = m_info.m_cMaxPlayersCt;
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
			if( bTournamentTable ){
				p->m_nRankIndex		= pSeat->GetUserPlaceIndex();
				p->m_nPlayersCt		= nTournamentPlayersCt;
				p->m_shTablesCt		= (WORD)nTournamentTablesCt;
				}

			m_pServer->SendPacket(pSeat->GetConnection(), p);
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
	if( bTournamentTable ){
		p->m_nRankIndex		= 0;
		p->m_nPlayersCt		= nTournamentPlayersCt;
		p->m_shTablesCt		= (WORD)nTournamentTablesCt;
		}
	else{
		p->m_nRankIndex	= p->m_nPlayersCt = p->m_shTablesCt	= 0;
		}

	m_pServer->SendPacketByTable(p->m_nTableId, m_info.m_nTournamentId, p, true);
	// }}

	m_lockObjects.Lock(); // Lock
	// Hand history entries. {{
	m_handHistory.StartHand		(&m_info, m_nHandId, (int)(m_info.m_fRakePercent*100), m_nHandStartDelay, m_nStartRoundDelay);
	m_handHistory.StartRound	(PokerRoundType::PreFlop, NULL, NULL, p->m_nMainPotAmount, 0);
	if( ((int)p->m_cSmallBlind) != -1 ){
		nTotalPotHistory	+= nSBAmount;
		m_handHistory.Action	((int)p->m_cSmallBlind,	PokerActionTypes::PostSB, PokerSeatStatus::Sitted, 
								nSBAmount, m_info.GetBalance((int)p->m_cSmallBlind), nSBAmount, nTotalPotHistory, 0);
		}
	nTotalPotHistory += nBBAmount;
	m_handHistory.Action		((int)p->m_cBigBlind,	PokerActionTypes::PostBB, PokerSeatStatus::Sitted, nBBAmount, m_info.GetBalance((int)p->m_cBigBlind), nBBAmount, nTotalPotHistory, 0);

	// Pay BB and play seats.
	for( int i=0; i<arrPayBBSeats.GetCount(); i++ ){
		nTotalPotHistory += nBBAmount;
		m_handHistory.Action(arrPayBBSeats[i],	PokerActionTypes::PostBB, PokerSeatStatus::Sitted, nBBAmount, m_info.GetBalance(i), nBBAmount, nTotalPotHistory, 0);
		}
	// }}

	m_info.m_gameStatus		= PokerGameStatus::Started;
	m_lockObjects.Unlock(); // Unlock
	delete p;
	return true;
	}

void
PokerCashTableController::ClearSeatCards(){
	m_lockObjects.Lock();		// Lock
	m_info.ClearSeatCards();
	m_lockObjects.Unlock();		// Unlock
	}

bool
PokerCashTableController::SitOut(int nSeat, ClientConnection* pConn){
	m_lockObjects.Lock();		// Lock
	int nTableId						= m_info.m_nTableId;
	PokerSeatStatus		status			= m_info.GetSeatStatus(nSeat);
	_String				sUserName;
	PokerTableSeatInfo* pSeat			= m_info.GetSeatInfo(nSeat);
	bool				bZeroBalanced	= ((pSeat && pSeat->GetBalance() > 0) ? true : false);
	if( pSeat )
		sUserName = pSeat->GetUserName();
	m_info.SitOut(nSeat);
	m_lockObjects.Unlock();		// Unlock

	if( status == PokerSeatStatus::Empty )
		return false;

	PokerSitOutPacket packet1;
	packet1.m_nSeat				= nSeat;
	packet1.m_nTableId			= nTableId;
	packet1.m_nTournamentId		= m_info.m_nTournamentId;
	packet1.m_bSitOut			= true;
	m_pServer->SendPacket(pConn, &packet1);

	// Send SetSeatStatus packet. {{
	PokerSeatStatusChangedPacket packet2;
	packet2.m_nTableId			= nTableId;
	packet2.m_nTournamentId		= m_info.m_nTournamentId;
	packet2.m_cSeat				= (char)nSeat;
	packet2.m_sUserName			= sUserName;
	packet2.m_status			= (char)PokerSeatStatus::SittedOut;
	m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, &packet2);
	// }}

	m_lockObjects.Lock();		// Lock
	// Start leave seat timer if not tournament table.
	if( !m_info.m_bTournamentTable )
		StartLeaveSeatTimer(nSeat, bZeroBalanced);
	m_lockObjects.Unlock();		// Unlock
	return true;
	}

bool
PokerCashTableController::SitOutBySeat(int nSeat, bool bSendAddChips /*= false*/, bool bMissedSB /*= false*/, bool bMisedBB /*= false*/){
	m_lockObjects.Lock();		// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock(); // Unlock
		return false;
		}
	ClientConnection* pConn = pSeat->GetConnection();
	if( bMissedSB )
		pSeat->SetMissedSB(bMissedSB);
	if( bMisedBB )
		pSeat->SetMissedBB(bMisedBB);
	m_lockObjects.Unlock();		// Unlock

	if( SitOut(nSeat, pConn) ){
		if( bSendAddChips && pSeat ){
			PokerSession* pSess	= (PokerSession*)pSeat->GetSession(true);
			if( pSess ){
				// Show add chips window.
				PokerAddChipsPacket p;
				p.ZeroPacketVariables();
				p.m_nTableId		= m_info.m_nTableId;
				p.m_nAmount			= 0;
				p.m_bPrepareToAdd	= true;
				p.m_cSeat			= (char)nSeat;
				p.m_nBalance		= 0;
				p.m_nMin			= 0;
				p.m_nMax			= 0;
				p.m_nSessionId		= pSess->GetSessionId();
				p.m_nSeatSessionId	= 0;
				OnAddChips(pSess, pConn, &p);

				SafePointer::Release(pSess); // Release
				}
			}
		return true;
		}
	return false;
	}

bool
PokerCashTableController::SetSeatOffline(int nSeat, bool bOnline /*= false*/,  PokerSession* pSess /*= NULL*/){
	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(nSeat);
	if( !pSeat ){
		m_lockObjects.Unlock();	// Unlock
		return false;
		}

	int	nTableId = m_info.m_nTableId;
	// Set seat session.
	if( pSess )
		pSeat->SetSession(pSess, nTableId, 0);
	// Set offline status.
	pSeat->SetOffline(!bOnline);	
	m_lockObjects.Unlock();	// Unlock

	// Send SeatOnlineStatusChanged packet. {{
	PokerSeatOnlineChangedPacket packet;
	packet.m_nTableId			= nTableId;
	packet.m_nTournamentId		= m_info.m_nTournamentId;
	packet.m_cSeat				= (char)nSeat;
	packet.m_bOnline			= bOnline;
	m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, &packet);
	// }}
	return true;
	}

bool
PokerCashTableController::EmptySeat(int nSeat, bool bSendCallbacks /*= true*/){
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
	_String			sUserName	= pSeat->GetUserName();
	int				nUserId		= pSeat->GetUserId();

	if( status == PokerSeatStatus::Reserved ){
		if( CancelSeatReservation(nSeat) )
			m_pGameBase->PlayerCanceledReservation(this, m_pParam, pSess, nUserId, m_info.m_nTableId, nSeat);
		// Empty seat.
		m_info.EmptySeat(nSeat);
		bEmptySeat = true;
		}
	else{
		if( pSess ){
			// If hand is not started or not in play state, free seat at once.	
			if( m_info.CanEmptySeatWhenLeave(nSeat) ){
				// Call seat balance change callback.
				m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, pSess, nTableId, m_nCurrencyId, nSeat, 0, pSeat->GetBalance());
				// Empty seat and keep seat info object pointer.
				m_info.EmptySeat(nSeat, false);
				// Call seat left callback.
				m_pGameBase->PlayerLeftSeat(this, m_pParam, &m_info, nSeat, pSeat);
				delete pSeat; // Delete seat info object pointer.
				bEmptySeat = true;
				}
			else{// Sessionless seat will empty when hand finishes.
				// Remove from wait list.
				RemoveFromWaitList(pSeat->GetConnection());
				// Set seat offline
				SetSeatOffline(nSeat, false);
				}
			}
		else{
			if( pSeat && status != PokerSeatStatus::Empty ){
				bEmptySeat = true;
				// Call seat balance change callback.
				m_pGameBase->PlayerSeatBalanceChanged(this, m_pParam, pSess, nTableId, m_nCurrencyId, nSeat, 0, pSeat->GetBalance());
				// Call seat left callback.
				m_pGameBase->PlayerLeftSeat(this, m_pParam, &m_info, nSeat, pSeat);
				}
			// Empty seat.
			m_info.EmptySeat(nSeat);
			}
		}
	m_lockObjects.Unlock();	// Unlock

	if( bEmptySeat ){
		// Send SetSeatStatus packet. {{
		PokerSeatStatusChangedPacket p;
		p.m_nTableId		= nTableId;
		p.m_nTournamentId	= m_info.m_nTournamentId;
		p.m_cSeat			= (char)nSeat;
		p.m_sUserName		= sUserName;
		p.m_status			= (char)PokerSeatStatus::Empty;
		m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, &p);
		// }}
		}

	if( pSess ) SafePointer::Release(pSess);
	return true;
	}

int
PokerCashTableController::EmptyOfflineSeats(){
	m_lockObjects.Lock(); // Lock
	if( m_info.m_bTournamentTable ){
		m_lockObjects.Unlock(); // Unlock
		return 0;
		}

	CDWordArray arrEmptySeats;
	m_info.EmptyOfflineSeats(false, &arrEmptySeats);

	PokerSeatStatusChangedPacket* p	= new PokerSeatStatusChangedPacket();
	p->m_nTableId				= m_info.m_nTableId;
	p->m_nTournamentId			= m_info.m_nTournamentId;
	p->m_status					= (char)PokerSeatStatus::Empty;

	CStringVector arrUserNames;
	for( int i=0; i<arrEmptySeats.GetCount(); i++ ){
		int nSeat = (int)arrEmptySeats[i];
		arrUserNames.push_back(m_info.GetSeatUserName(nSeat)); 
		// Call seat left callback.
		m_pGameBase->PlayerLeftSeat(this, m_pParam, &m_info, nSeat, m_info.GetSeatInfo(nSeat));
		// Empty seat info.
		m_info.EmptySeat(nSeat);
		}
	m_lockObjects.Unlock(); // Unlock

	// Send empty seat packet.
	for( int i=0; i<arrEmptySeats.GetCount(); i++ ){
		p->m_cSeat		= (char)arrEmptySeats[i];
		p->m_sUserName	= arrUserNames[i];
		m_pServer->SendPacketByTable(p->m_nTableId, m_info.m_nTournamentId, p);
		}
	
	delete p;
	return (int)arrEmptySeats.GetCount();
	}

bool
PokerCashTableController::ForceSeatToLeaveTable(int nSeat){
	m_lockObjects.Lock();			// Lock
	PokerTableSeatInfo* pSeatInfo = m_info.GetSeatInfo(nSeat);
	if( !pSeatInfo || pSeatInfo->GetStatus() == PokerSeatStatus::Empty || !pSeatInfo->GetSession(false) ){
		m_lockObjects.Unlock();		// Unlock
		return false;
		}

	Session*			pSes		= pSeatInfo->GetSession(true);
	if( !pSes ){
		m_lockObjects.Unlock();		// Unlock
		return false;
		}
	ClientConnection*	pConn		= pSeatInfo->GetConnection();
	m_lockObjects.Unlock();		// Unlock
	// Empty seat and not close table to player.
	EmptySeat(nSeat);

	/* Old code.
	PokerLeaveTablePacket packet;
	packet.m_bIsRequest				= true;
	packet.m_nSessionId				= pSes->GetSessionId();
	packet.m_nTableId				= m_info.m_nTableId;
	bool				bOffline	= pSeatInfo->IsOffline();
	
	if( bOffline || !pConn ){
		EmptySeat(nSeat);
		}
	else// Call OnLeaveTable handler implicitly
		m_pServer->HandlePacket(&packet, (void*)pConn);
	*/
	SafePointer::Release(pSes); // Release session.
	return true;
	}

bool
PokerCashTableController::GetEmptySeat(int& nSeat, int nStartFrom /*= 0*/){
	m_lockObjects.Lock(); // Lock
	bool bRet = false;
	for(int i=nStartFrom; i<m_info.m_cMaxPlayersCt; i++){
		PokerTableSeatInfo* pSeat = m_info.GetSeatInfo(i);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty ){
			nSeat = i;
			bRet = true;
			break;
			}
		}
	m_lockObjects.Unlock(); // Unlock
	return bRet;
	}

int
PokerCashTableController::GetSeatIndexByUserId(int nUserId, INT_CHIPS* pUserBalance /*= NULL*/){
	if( nUserId <= 0 )
		return -1;
	int nSeatIndex = -1;
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeat = m_info.GetSeatByUserId(nUserId);
	if( pSeat && pSeat->GetStatus() != PokerSeatStatus::Empty ){
		nSeatIndex = pSeat->GetSeat();
		if( pUserBalance )
			*pUserBalance = pSeat->GetBalance();
		}
	m_lockObjects.Unlock(); // Unlock
	return nSeatIndex;
	}

bool
PokerCashTableController::SendPostBlindRequest(int nSeat, bool bSmallBlind, bool& bResult, bool bCheckForAutoFlag /*= true*/, bool bAllowedToFine /*= false*/){
	bResult = false;
	if( bSmallBlind ){
		int					nSBSeat		= nSeat;
		int					nSBAmount	= m_info.GetSBAmount();
		m_lockObjects.Lock(); // Lock
		PokerTableSeatInfo* pSBSeatInfo = m_info.GetSeatInfo(nSeat);
		if( pSBSeatInfo ){
			// Check for seat balance amount. {{
			PokerSeatStatus statusOld	= pSBSeatInfo->GetStatus();
			INT_CHIPS		nBalance	= pSBSeatInfo->GetBalance();
			if( nBalance < nSBAmount ){				
				pSBSeatInfo->SetBalance(0);
				if( !nBalance ){
					pSBSeatInfo->SetStatus(PokerSeatStatus::All_In);
					m_lockObjects.Unlock(); // Unlock
					}
				else{
					m_info.AddPotAmount	(nSeat, nBalance);
					m_lockObjects.Unlock(); // Unlock
					// Post seat action.
					SendSeatActionPacket(nSeat, PokerActionTypes::PostSB, PokerSeatStatus::All_In, nBalance);
					}
				bResult = true;
				return true;
				}
			// }}

			// Ask seat for action.
			if( !bCheckForAutoFlag || pSBSeatInfo->GetAutoPostBlind() || m_info.m_bTournamentTable ){
				nBalance -= nSBAmount;
				m_info.SetBalance	(nSeat, nBalance);
				m_info.AddPotAmount	(nSeat, nSBAmount);
				m_lockObjects.Unlock(); // Unlock
				
				// Post seat action.
				SendSeatActionPacket(nSeat, PokerActionTypes::PostSB, PokerSeatStatus::Sitted, nSBAmount);
				bResult = true;
				return true;
				}

			// Send PostSB packet.
			PokerPostBlindPacket* p		= new PokerPostBlindPacket();
			p->m_nTableId				= m_info.m_nTableId;
			p->m_cPostBlind				= 0; // Post small blind
			p->m_cSeat					= (char)nSBSeat;
			p->m_shActionSec			= (short)m_nPostBlindDelay;
//			p->m_nAmount				= nSBAmount;

			ClientConnection* pConn = pSBSeatInfo->GetConnection();
			m_lockObjects.Unlock(); // Unlock

			// Add parameters.
			m_gameThread.AddParam(_T("PACKET"), p);
			m_gameThread.AddParam(_T("CONN"),	pConn);
			// Reset sync event.
			m_gameThread.SetSyncEvent(false);
			// Send packet.
			m_pServer->SendPacket(pConn, p);
			// Send active seat packet.
			SendSetActiveSeat(nSBSeat, m_nPostBlindDelay);
			return true;
			}
		m_lockObjects.Unlock(); // Unlock
		}
	else{
		int					nBBSeat		= nSeat;
		INT_CHIPS			nBBAmount	= (INT_CHIPS)m_info.GetBBAmount();
		m_lockObjects.Lock(); // Lock
		PokerTableSeatInfo* pBBSeatInfo = m_info.GetSeatInfo(nSeat);
		if( pBBSeatInfo ){
			// Check for seat balance amount. {{
			PokerSeatStatus statusOld	= pBBSeatInfo->GetStatus();
			INT_CHIPS		nBalance	= pBBSeatInfo->GetBalance();
			UINT			nFineAmount	= 0;
			PokerActionTypes actionType = PokerActionTypes::PostBB;
			if( bAllowedToFine && pBBSeatInfo->HasMissedSB() ){
				nFineAmount	= m_info.GetSBAmount();
				actionType	= PokerActionTypes::PostBB_Plus_SB;
				}

			if( nBalance < (nBBAmount + nFineAmount) ){			
				pBBSeatInfo->SetBalance(0);
				if( !nBalance ){
					pBBSeatInfo->SetStatus(PokerSeatStatus::All_In);
					m_lockObjects.Unlock(); // Unlock
					}
				else{
					if( nFineAmount > 0 ){
						if( nBalance > nBBAmount ){
							nFineAmount = (UINT)(nBalance - nBBAmount);
							m_info.AddFineAmount(nFineAmount);
							m_info.AddPotAmount	(nSeat, nBalance);
							}
						else
							m_info.AddPotAmount	(nSeat, nBalance);
						}
					else
						m_info.AddPotAmount	(nSeat, nBalance);
					m_lockObjects.Unlock(); // Unlock
					// Post seat action.
					SendSeatActionPacket(nSeat, actionType, PokerSeatStatus::All_In, (nBalance - nFineAmount));
					}
				bResult = true;
				return true;
				}
			// }}

			if( !bCheckForAutoFlag || pBBSeatInfo->GetAutoPostBlind() || m_info.m_bTournamentTable ){
				if( nFineAmount > 0 ){
					m_info.AddFineAmount(nFineAmount);
					nBalance -= nFineAmount;
					}

				nBalance -= nBBAmount;
				m_info.SetBalance	(nSeat, nBalance);
				m_info.AddPotAmount (nSeat, nBBAmount);
				m_lockObjects.Unlock(); // Unlock

				// Send seat action.
				SendSeatActionPacket(nSeat,  actionType, PokerSeatStatus::Sitted, (nBBAmount /*+ nFineAmount*/));
				bResult = true;
				return true;
				}

			// Send PostBB packet.
			PokerPostBlindPacket* p		= new PokerPostBlindPacket();
			p->m_nTableId				= m_info.m_nTableId;
			p->m_cPostBlind				= ((bAllowedToFine && pBBSeatInfo->HasMissedSB()) ? 2 /*Post BB + SB*/ : 1/*Post big blind*/);
			p->m_cSeat					= (char)nBBSeat;
			p->m_shActionSec			= (short)m_nPostBlindDelay;
//			p->m_nAmount				= nBBAmount;

			ClientConnection* pConn = pBBSeatInfo->GetConnection();
			m_lockObjects.Unlock(); // Unlock

			m_pServer->SendPacket(pConn, p);

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
PokerCashTableController::SendShowMuckCardsRequest(int nSeat, bool& bResult){
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
		p->m_nTableId				= m_info.m_nTableId;
		p->m_nTournamentId			= m_info.m_nTournamentId;
		p->m_cSeat					= (char)nSeat;
		p->m_shActionSec			= (short)m_nShowMuckDelay;

		ClientConnection* pConn		= pSeatInfo->GetConnection();
		m_lockObjects.Unlock(); // Unlock

		// Add parameters.
		m_gameThread.AddParam(_T("PACKET"), p);
		m_gameThread.AddParam(_T("CONN"),	pConn);
		// Reset sync event.
		m_gameThread.SetSyncEvent(false);
		// Send packet.
		m_pServer->SendPacket(pConn, p);
		// Send active seat packet.
		SendSetActiveSeat(nSeat, m_nShowMuckDelay);
		return true;
		}
	m_lockObjects.Unlock(); // Unlock
	return false;
	}

bool
PokerCashTableController::SendSeatStatusChange(int nSeat, PokerSeatStatus status){
	m_lockObjects.Lock(); // Lock
	PokerTableSeatInfo* pSeatInfo	= m_info.GetSeatInfo(nSeat);
	if( pSeatInfo ){
		pSeatInfo->SetStatus(status);
		}

	// Send SetSeatStatus packet.
	PokerSeatStatusChangedPacket packet;
	packet.m_nTableId				= m_info.m_nTableId;
	packet.m_nTournamentId			= m_info.m_nTournamentId;
	packet.m_cSeat					= (char)nSeat;
	packet.m_status					= (char)status;
	m_lockObjects.Unlock(); // Unlock

	// Send packet.
	m_pServer->SendPacketByTable(packet.m_nTableId, m_info.m_nTournamentId, &packet);
	return true;
	}

bool
PokerCashTableController::SendSetActiveSeat(int nActiveSeat, int nRegularTimeSec, int nExtraTimeSec /*= 0*/, int nTimeBankSec /*= 0*/){
	m_lockObjects.Lock();	// Lock
	int nTableId = m_info.m_nTableId;
	m_info.m_cActiveSeat = (char)nActiveSeat;
	m_lockObjects.Unlock(); // Unlock

	// Send set active packet. {{
	PokerSetActivePacket packet;
	packet.m_cSeat			= (char)nActiveSeat;
	packet.m_nTableId		= nTableId;
	packet.m_nTournamentId	= m_info.m_nTournamentId;
	packet.m_cRegularTimeSec= (BYTE)nRegularTimeSec;
	packet.m_cExtraTimeSec	= (BYTE)nExtraTimeSec;
	packet.m_cTimeBankSec	= (BYTE)nTimeBankSec;
	m_pServer->SendPacketByTable(nTableId, m_info.m_nTournamentId, &packet);
	// }}
	return true;
	}

bool
PokerCashTableController::SendSeatActionPacket(int nSeat, PokerActionTypes action, PokerSeatStatus status, INT_CHIPS nAmount, bool bSetSitOutNextHand /*= false*/, int nActionWaitTimeSec /*= 0*/){
	// Send action packet. {{
	PokerActionPacket packet;
	m_lockObjects.Lock();	// Lock
	bool	bSendPreActions		= (m_info.GetSeatStatus(nSeat) == PokerSeatStatus::InPlay && m_info.GetSeatCtByStatus(PokerSeatStatus::InPlay) > 1 && 
		(action == PokerActionTypes::Bet || action == PokerActionTypes::Raise || action == PokerActionTypes::Call || 
		action == PokerActionTypes::Check || action == PokerActionTypes::AllIn));
	bool	bSitOutNextHand		= m_info.GetSitOutNextHand(nSeat);

	m_info.SetLastAction(nSeat, action);	// Remember last action.
	if( status != -1 )
		m_info.SetSeatStatus(nSeat, status);// Set seat status.
	packet.m_nTableId			= m_info.m_nTableId;
	packet.m_nTournamentId		= m_info.m_nTournamentId;
	packet.m_nBalance			= m_info.GetBalance			(nSeat);
	packet.m_nPotAmount			= m_info.GetPotAmount		(nSeat);
	packet.m_nSeatTotalPot		= m_info.GetSeatTotalPot	(nSeat);
	packet.m_nTotalPot			= m_info.GetTotalPotAmount	();
	packet.m_bSetPreActions		= bSendPreActions;

	// Seat action callback.
	m_info.OnSeatAction(nSeat, action, nAmount);

	if( bSetSitOutNextHand )
		m_info.SetSitOutNextHand(nSeat, true);

	packet.m_cSeat				= (char)nSeat;
	packet.m_cSeatStatus		= (char)m_info.GetSeatStatus(nSeat);
	packet.m_cSeatReceiver		= (char)-1;
	packet.m_cActionType		= action;
	packet.m_nAmount			= nAmount;
	packet.m_dtAction			= COleDateTime::GetCurrentTime();
	packet.m_bRoundFinished		= false;

	// Retrieve in-play seats to send packet one-by-one.
	CPtrArray arrSeats;
	m_info.GetSeats(arrSeats);
	// Add chat message.
	m_info.AddChatMessage	(packet.m_dtAction, nSeat, (PokerActionTypes)packet.m_cActionType, packet.m_nAmount);
	m_lockObjects.Unlock(); // Unlock

	// Determine hand cards open status. {{
	if( m_info.m_round > PokerRoundType::No ){
		if( action == PokerActionTypes::ShowCards ){
			PokerHandCards* pHandCards = new PokerHandCards();
			if( m_info.GetSeatHandCards(nSeat, pHandCards) )
				packet.m_listCardsShown.Add(pHandCards);

			// Add action.
			m_handHistory.Action(nSeat,	action, status, nAmount, packet.m_nBalance, packet.m_nPotAmount, packet.m_nTotalPot, nActionWaitTimeSec, &packet.m_listCardsShown);

			// Send packet to everyone.
			packet.m_cSeatReceiver	= (char)-1;
			m_pServer->SendPacketByTable(packet.m_nTableId, m_info.m_nTournamentId, &packet, false);
			// }}
			return true;
			}
		else
		if( action == PokerActionTypes::Muck ){
			// Add action.
			m_handHistory.Action(nSeat,	action, status, nAmount, packet.m_nBalance, packet.m_nPotAmount, packet.m_nTotalPot, nActionWaitTimeSec, &packet.m_listCardsShown);
			// Send packet to everyone.
			packet.m_cSeatReceiver	= (char)-1;
			m_pServer->SendPacketByTable(packet.m_nTableId, m_info.m_nTournamentId, &packet, false);
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
				m_info.GetSeatsHandCards(&packet.m_listCardsShown, NULL);
				}

			if( bRoundEnded )
				m_info.GetNextActiveSeat(nSeat, nSeatAsk, bRoundEnded, bOpenCards);

			packet.m_bRoundFinished = bRoundEnded;
			}
		}
	// }}

	// Normalize seat pot amounts.
	if( packet.m_bRoundFinished ){
		m_lockObjects.Lock();	// Lock
		int nRet = m_info.NormalizeSeatPots(&packet.m_listSeatChangedAmounts);
		m_lockObjects.Unlock(); // Unlock

		if( nRet > 0 ){
			for(int i=0; i<packet.m_listSeatChangedAmounts.GetCount(); i++){
				PokerSeatAmountInfo* pSeatAmount = packet.m_listSeatChangedAmounts[i];
				if( !pSeatAmount ) continue;

				if( pSeatAmount->m_cSeat == (char)nSeat ){
					INT_CHIPS nAmountDifference	= (pSeatAmount->m_nBalance - packet.m_nBalance);
					nAmount						-= nAmountDifference;

					packet.m_nBalance			= pSeatAmount->m_nBalance;
					packet.m_nAmount			= nAmount;
					packet.m_nBalance			= pSeatAmount->m_nBalance;
					packet.m_nPotAmount			= pSeatAmount->m_nPotAmount;
					packet.m_nSeatTotalPot		= m_info.GetSeatTotalPot(nSeat);
					//break;
					}
				else{
					// Change hand histories action.
					m_handHistory.ChangeLastActionAmounts(pSeatAmount->m_cSeat, pSeatAmount->m_nPotAmount, pSeatAmount->m_nBalance, pSeatAmount->m_nPotAmount, pSeatAmount->m_nTotalPotAmount);
					}
				}

			// Change total pot amount.
			packet.m_nTotalPot			= m_info.GetTotalPotAmount();
			}
		}

	// Add action.
	m_handHistory.Action(nSeat,	action, status, nAmount, packet.m_nBalance, packet.m_nPotAmount, packet.m_nTotalPot, nActionWaitTimeSec, &packet.m_listCardsShown);

	if( arrSeats.GetCount() > 0 ){
		bSendPreActions = bSendPreActions ? (!packet.m_bRoundFinished) : bSendPreActions;
		// Send packet one by one.
		for(int i=0; i<arrSeats.GetCount(); i++){
			PokerTableSeatInfo* pSeatInfo = (PokerTableSeatInfo*)arrSeats.GetAt(i);
			if( !pSeatInfo || !pSeatInfo->GetConnection() ) continue;

			m_lockObjects.Lock(); // Lock
			// Setup pre-actions for each seat.
			if( bSendPreActions )
				m_info.GetSeatPreActions(pSeatInfo->GetSeat(), &packet.m_listPreActions);
			packet.m_cSeatReceiver = (char)pSeatInfo->GetSeat();
			m_lockObjects.Unlock(); // Unlock

			m_pServer->SendPacket(pSeatInfo->GetConnection(), &packet);
			packet.m_listPreActions.RemoveAll(); // Remove pre-actions
			}

		// Send only to observers.
		packet.m_cSeatReceiver	= (char)-1;
		m_pServer->SendPacketByTable(packet.m_nTableId, m_info.m_nTournamentId, &packet, true);
		// }}
		}
	else{
		// Send packet to everyone.
		packet.m_cSeatReceiver	= (char)-1;
		m_pServer->SendPacketByTable(packet.m_nTableId, m_info.m_nTournamentId, &packet, false);
		}

	// Sitout player.
	if( action == PokerActionTypes::Fold && bSitOutNextHand )
		SitOutBySeat(nSeat, false);

	if( bSetSitOutNextHand ){
		m_lockObjects.Lock();	// Lock

		// Set option packet.
		PokerSetOptionPacket packet1;
		packet1.m_nTableId		= m_info.m_nTableId;
		packet1.m_nTournamentId	= m_info.m_nTournamentId;
		packet1.m_cSeat			= (char)nSeat;
		packet1.m_cOption		= (char)PokerTableOption::Option_SitOutNextHand;
		packet1.m_bValue		= true;
		packet1.m_nValue		= 0;
		
		ClientConnection* pConn = m_info.GetConnection(nSeat);
		m_lockObjects.Unlock(); // Unlock
		m_pServer->SendPacket(pConn, &packet1);
		}
	return true;
	}

bool
PokerCashTableController::IsSeatReservedByConn(int nSeat, ClientConnection* pConn){
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
PokerCashTableController::CancelSeatReservation(int nSeat){
	m_lockObjects.Lock(); // Lock
	int		nTimerId	= m_arrSeatReserveTimerIds[nSeat];
	bool	bRet		= Timers::GetInstance()->KillTimer(nTimerId);
	m_arrSeatReserveTimerIds[nSeat] = 0;
	m_lockObjects.Unlock(); // Unlock
	return bRet;
	}

void
PokerCashTableController::SeatWaitListUsers(AutoSortedArray& arrSeated){
	m_lockObjects.Lock(); // Lock
	int nLoop = 0, nCt = m_listWaitingList.GetCount(), nSeat = -1;

	while( nLoop < nCt ){
		nSeat = m_info.GetFirstEmptySeat();
		if( nSeat == -1 ) break;
		ClientConnection* pConn = m_listWaitingList.GetAt(0);
		if( !pConn ) break;

		PokerSession* pSess = (PokerSession*)pConn->GetSession(false);
		if( !pSess ){
			RemoveFromWaitList(pConn);
			nCt --;
			}

		int		nUserId		= pSess->GetUserId();
		_String sUserName	= pSess->GetUserName();
		if( nUserId <= 0 ) break;

		// Remove from wait list.
		RemoveFromWaitList(pConn);
		// Add seated seat index.
		arrSeated.AddAsLong(nSeat);

		// Pseudo reserve seat packet.
		PokerReserveSeatPacket packet;
		packet.m_nSessionId			= pSess->GetSessionId();
		packet.m_nTableId			= m_info.m_nTableId;
		packet.m_cSeat				= (char)nSeat;
		packet.m_sUserName			= sUserName;
		OnReserveSeat(pSess, pConn, &packet);
		}
	m_lockObjects.Unlock(); // Unlock
	}

int
PokerCashTableController::AddInWaitList(ClientConnection* pConn){
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
		PokerSession* pSess = (PokerSession*)pConn->GetSession(true);
		if( pSess ){
			m_pGameBase->PlayerAddedInWaitList(this, m_pParam, pSess, nTableId);
			SafePointer::Release(pSess); // Release session.
			}
		}
	m_lockObjects.Unlock(); // Unlock
	return nIndex;
	}

bool
PokerCashTableController::RemoveFromWaitList(ClientConnection* pConn){
	m_lockObjects.Lock();		// Lock
	int nIndex = m_arrWaitingListConn.IndexOf((__int64)pConn);
	if( nIndex != -1 ){ // Connection already exists in waiting list.
		if( pConn ){
			PokerSession* pSess = (PokerSession*)pConn->GetSession(true);
			if( pSess ){
				m_pGameBase->PlayerRemovedFromWaitList(this, m_pParam, pSess, m_info.m_nTableId);
				SafePointer::Release(pSess); // Release session.
				}
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
PokerCashTableController::IsInWaitList(ClientConnection* pConn){
	m_lockObjects.Lock(); // Lock
	int nIndex = m_arrWaitingListConn.IndexOf((__int64)pConn);
	m_lockObjects.Unlock(); // Unlock
	return ( nIndex != -1 ); // Connection already exists in waiting list.
	}

int
PokerCashTableController::GetWaitListUsersCt(){
	m_lockObjects.Lock(); // Lock
	int nCt = (int)m_arrWaitingListConn.GetCount();
	m_lockObjects.Unlock(); // Unlock
	return nCt;
	}

bool
PokerCashTableController::CloseTable(){
	if( m_bCloseTable || m_bTableIsClosed )
		return true;
	m_bCloseTable = true;
	if( !GThread_IsStarted() ){
		if( GThread_IsPaused() )
			GThread_Resume();
		}
	else{
		m_bTableIsClosed = true;
		// Send table closed packet.
		PokerTableClosedPacket packet;
		packet.m_nTableId		= m_info.m_nTableId;
		packet.m_nTournamentId	= m_info.m_nTournamentId;
		m_pServer->SendPacketByTable(packet.m_nTableId, packet.m_nTournamentId, &packet, false);
		}
	return true;
	}

bool
PokerCashTableController::GetSeatInfo(int nSeat, PokerTableSeatInfo* pSeatInfo){
	m_lockObjects.Lock();	// Lock
	bool				bRet	= false;
	PokerTableSeatInfo* pSeat	= m_info.GetSeatInfo(nSeat);
	if( pSeat ){
		pSeat->Copy(pSeatInfo);
		bRet = true;
		}
	m_lockObjects.Unlock(); // Unlock
	return bRet;
	}

bool
PokerCashTableController::SetSeatInfo(int nSeat, PokerTableSeatInfo* pSeatInfo){
	m_lockObjects.Lock();	// Lock
	PokerTableSeatInfo* pSeat = NULL;
	if( pSeatInfo ){
		pSeat	= new PokerTableSeatInfo();
		pSeatInfo->Copy(pSeat);
		}
	bool				bRet	= m_info.SetSeatInfo(nSeat, pSeat);
	if( !bRet && pSeat ){
		delete pSeat;
		m_lockObjects.Unlock(); // Unlock
		ASSERT(FALSE);
		return false;
		}

	PokerSession* pSess = (PokerSession*)pSeat->GetSession(true);
	m_lockObjects.Unlock(); // Unlock

	// Seat session.
	if( pSess ){
		pSess->SeatAt(m_info.m_nTableId, nSeat, m_info.m_nTournamentId);
		SafePointer::Release(pSess); // Release session pointer.
		}
	return bRet;
	}

void
PokerCashTableController::GetSeatBalanceStat(INT_CHIPS& nBalanceSmallest, INT_CHIPS& nBalanceLargest){
	m_lockObjects.Lock();	// Lock
	nBalanceLargest		= 0;
	nBalanceSmallest	= MAXINT64;
	bool bSuccess = false;
	for(int i=0; i<(int)m_info.m_cMaxPlayersCt; i++){
		INT_CHIPS nAmount = m_info.GetBalance(i);
		if( nAmount > 0 ){
			bSuccess = true;
			nBalanceSmallest	= min(nBalanceSmallest, nAmount);
			nBalanceLargest		= max(nBalanceLargest, nAmount);
			}
		}
	m_lockObjects.Unlock(); // Unlock
	if( !bSuccess ){
		nBalanceSmallest = nBalanceLargest = 0;
		}
	}

INT_CHIPS
PokerCashTableController::GetSeatBalance(int nSeat){
	m_lockObjects.Lock();	// Lock
	INT_CHIPS nBalance = m_info.GetBalance(nSeat);
	m_lockObjects.Unlock(); // Unlock
	return nBalance;
	}