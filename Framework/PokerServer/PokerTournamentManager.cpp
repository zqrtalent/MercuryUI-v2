#include "stdafx.h"
#include "PokerServer.h"
#include "PokerTournaments.h"
#include "Data/PokerTournamentData.h"
#include "PokerTournamentManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerTournamentManager
///
///////////////////////////////////////////////////////////

PokerTournamentManager::PokerTournamentManager(PokerServer* pServer /*= NULL*/, PokerTournaments* pOwner /*= NULL*/) {
	if( !pServer || !pOwner ){
		m_pServer				= NULL;
		m_pOwner				= NULL;
		m_pSyncGameStartEvent	= NULL;
		return;
		}

	m_info.ZeroInit();
	m_pServer					= pServer;
	m_pOwner					= pOwner;
	m_bVisible					= false;
	m_bLateRegistration			= false;
	m_bTournamentIsInProgress	= false;
	m_bRegistrationStatus		= false;
	m_info.ZeroInit();
	m_nextLevel.ZeroInit();
	m_currLevel.ZeroInit();

	m_pParam					= NULL;
	m_nLevelIndex				= -1;
	m_bAddonBreak				= false;
	m_bIsBreak					= false;
	m_bStartBreakSyncronously	= true;
	m_bWaitingToStartBreak		= false;
	m_nBreakSecondsLeft			= 0;
	m_nLevelSecondsLeft			= 0;
	m_bHandForHand				= false;
	m_pSyncGameStartEvent		= new CEvent(TRUE, TRUE); // Create signaled and manually reseting event.

	m_nStakeHighest				= 0;
	m_nStakeAverage				= 0;
	m_nStakeLowest				= 0;

	m_tournamentStatus			= TournamentStatus::Tournament_NA;
	m_bRebuyIsAllowed			= false;

	m_nTableCt					= 0;
	m_nPlayersCt				= 0;
	m_nRegisteredCt				= 0;
	m_nFinishedTableCt			= 0;
	m_nStartedTableCt			= 0;

	m_nTournamentStartSecondsLeft = 0;

	m_bCancelling				= false;
	m_bDestroying				= false;
	m_bFinishTournament			= false;

	m_arrUsersPlaying				.OptimizeForLargeData(1);
	m_arrHandInProgressGameTableIds	.OptimizeForLargeData(1);
	} 

BEGIN_DECLARE_VAR(PokerTournamentManager, Serializable)
	DECLARE_VAR(_T("PokerTournamentManager"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""),		VariableType::VT_Int32, offsetof(PokerTournamentManager,m_info.m_nId), false, false, false)
END_DECLARE_VAR()

PokerTournamentManager::~PokerTournamentManager(){
	Stop(true);
	delete m_pSyncGameStartEvent;
	}

bool
PokerTournamentManager::Start(bool bUseSyncBreak /*= true*/){
	if( !m_tournamentThread.IsThreadStopped() )
		return false;
	ASSERT(m_info.m_nId > 0);
	
	m_nBreakSecondsLeft		= m_info.m_shBreakDurationMin*60;
	m_nLevelSecondsLeft		= m_info.m_shLevelDuration*60;
	m_bLateRegistration		= false;
	m_tournamentStatus		= (TournamentStatus)m_info.m_nStatus;
	m_nRegisteredCt			= m_info.m_nEntriesCt;

	if(	m_info.m_nStatus == TournamentStatus::Tournament_Completed || 
		m_info.m_nStatus == TournamentStatus::Tournament_Canceled) {
		SetTournamentVisibility(true); // Make finished/canceled tournament visible.
		return false;
		}

	// Sit & Go tournament doesn't requires time check.
	if( (m_info.m_nSubTypeId&((int)GameSubType::Tournament_SitNGo)) > 0 ){
		if( m_info.m_nMaxPlayers <= 0 )
			return false;
		if( m_info.m_nStatus == TournamentStatus::Tournament_InProgress )
			return false;

		m_bRegistrationStatus = true;
		m_info.m_dtRegStartTime.SetCurrentTime();
		// Make tournament visible.
		SetTournamentVisibility(true);
		return true;
		}

	//COleDateTime dtCurrent	= COleDateTime::GetCurrentTime();
	DateTime dtCurrent(true);
	// Temporary removed
	if( dtCurrent > m_info.m_dtStartTime ) 
		return false; // Tournament finished or failed on start.

	/* Temp 
	if( m_info.m_nId != 93 ) return false;
	m_info.m_dtStartTime = dtCurrent;
	COleDateTimeSpan span(0, 0, 6, 0);
	m_info.m_dtRegEndTime = COleDateTime::GetCurrentTime() + span;
	Temp */

	// Break start setting.
	m_bStartBreakSyncronously	= bUseSyncBreak;
	// Late registration.
	m_bLateRegistration			= (m_info.m_dtStartTime < m_info.m_dtRegEndTime);

	if( !m_tournamentThread.StartThread((LPTHREAD_START_ROUTINE)&TournamentThread_Entry, this, THREAD_PRIORITY_NORMAL, SIZE_512KB, false) )
		return false;
	return true;
	}

void
PokerTournamentManager::Stop(bool bSaveState){
	m_bDestroying = true;
	// Signal sync start event.
	if( m_pSyncGameStartEvent )
		m_pSyncGameStartEvent->SetEvent();
	// Stop manager thread.
	m_tournamentThread.StopThread();

	// Destroy table controllers.
	m_lockGameTables.Lock();		// Lock
	// Stop game threads.
	for(int i=0; i<m_listTables.GetCount(); i++){
		TournamentTableController* pController = m_listTables[i];
		if( pController )
			pController->GThread_Stop();
		}

	m_listTables		.RemoveAll(true);
	m_arrGameTableById	.DeleteAll();
	m_arrGameTableById	.FreeExtra();
	m_lockGameTables.Unlock();	// Unlock
	m_bDestroying = false;
	}

void
PokerTournamentManager::SendStopSignal(){
	// Send stop event.
	m_tournamentThread.SetStopEvent(true);

	m_lockGameTables.Lock(); // Lock
	int nLoop = 0, nCt = m_listTables.GetCount();
	if( m_bCancelling ){
		while( nLoop < nCt ){
			TournamentTableController* pController = m_listTables.GetAt(nLoop);
			if( pController ){
				pController->m_bCanceled = true;
				pController->SendStopSignal();
				}
			nLoop ++;
			}
		}
	else{
		while( nLoop < nCt ){
			TournamentTableController* pController = m_listTables.GetAt(nLoop);
			if( pController )
				pController->SendStopSignal();
			nLoop ++;
			}
		}
	m_lockGameTables.Unlock(); // Unlock
	}

bool
PokerTournamentManager::CancelTournament(bool bChangeStatus){
	m_bCancelling = true;
	// Signal sync start event.
	if( m_pSyncGameStartEvent )
		m_pSyncGameStartEvent->SetEvent();
	// Notify manager thread and active game table threads for stopping.
	SendStopSignal();
	// Stop manager thread.
	m_tournamentThread.StopThread();
	
	// Destroy table controllers.
	m_lockGameTables.Lock();		// Lock

	// Stop game threads.
	for(int i=0; i<m_listTables.GetCount(); i++){
		TournamentTableController* pController = m_listTables[i];
		if( pController )
			pController->GThread_Stop();
		}

	m_listTables		.RemoveAll(true);
	m_arrGameTableById	.DeleteAll();
	m_arrGameTableById	.FreeExtra();
	m_lockGameTables.Unlock();		// Unlock
	m_bCancelling = false;

	// Set canceled status.
	if( bChangeStatus )
		SetTournamentStatus(TournamentStatus::Tournament_Canceled);
	return true;
	}

bool
PokerTournamentManager::LateRegisterPlayer(TournamentPlayerInfo* pPlayer, int nUserId, PokerSession* pSession){
	if( !m_bTournamentIsInProgress )
		return false;
	m_lockGameTables.Lock();	// Lock
	pPlayer->m_nChips	= m_info.m_nBuyInChips;
	bool bRet			= m_tablesMan.SeatLatelyRegisteredPlayer(this, pPlayer, nUserId, pSession);
	m_lockGameTables.Unlock();	// Unock
	return bRet;
	}

TournamentTableController*
PokerTournamentManager::CreateTableController(int nTableId, bool bInitializeTableSeatsWithNull /*= false*/){
	TournamentTableController*	pTableController	= new TournamentTableController(m_pServer, m_pOwner, this);
	PokerTableInfo*				pTableInfo			= &pTableController->m_info;
	TournamentInfo*				pInfo				= &m_info;

	pTableInfo->m_nTableId					= nTableId;
	pTableInfo->m_sTableName				= _T("Table");
	pTableInfo->m_nTournamentId				= pInfo->m_nId;
	pTableInfo->m_bTournamentTable			= true;
	pTableInfo->ClearStatInfo();
	pTableInfo->m_cGameTypeId				= (BYTE)pInfo->m_nGameTypeId;
	pTableInfo->m_shSubTypeId				= (short)pInfo->m_nSubTypeId;
	pTableInfo->m_cMoneyTypeId				= (BYTE)MoneyType::Play;
	pTableInfo->m_cGameSpeedId				= (BYTE)pInfo->m_nGameSpeedId;
	pTableInfo->m_fRakePercent				= 0.0f; // Not supported
	pTableInfo->m_cCurrencyId				= (BYTE)pInfo->m_nCurrencyId;
	pTableInfo->m_gameStatus				= PokerGameStatus::NotStarted;
	pTableInfo->m_round						= PokerRoundType::No;

	if( m_currLevel.m_nId > 0 ){
		pTableInfo->m_nSmallBlindAmount		= (int)(m_currLevel.m_dSmallBlind*100);
		pTableInfo->m_nBigBlindAmount		= (int)(m_currLevel.m_dBigBlind*100);
		pTableInfo->m_nAnteAmount			= (int)(m_currLevel.m_dAnte*100);
		}
	else{
		const TournamentBlindStructure* pBlindStruct = ((TournamentInfo*)pInfo)->GetLevelInfo(0);
		ASSERT(pBlindStruct);
		pTableInfo->m_nSmallBlindAmount		= (int)(pBlindStruct->m_dSmallBlind*100);
		pTableInfo->m_nBigBlindAmount		= (int)(pBlindStruct->m_dBigBlind*100);
		pTableInfo->m_nAnteAmount			= (int)(pBlindStruct->m_dAnte*100);
		}

	if( bInitializeTableSeatsWithNull ){
		for(int i=0; i<pInfo->m_shMaxSeatCt; i++)
			pTableInfo->m_seats.Add(NULL);
		}
	
	pTableInfo->m_nMinBuyIn					= 0; // Not supported
	pTableInfo->m_nMaxBuyIn					= 0; // Not supported
		
	pTableInfo->m_cMaxPlayersCt				= (char)pInfo->m_shMaxSeatCt;
	pTableInfo->m_cCard1					= -1;
	pTableInfo->m_cCard2					= -1;
	pTableInfo->m_cCard3					= -1;
	pTableInfo->m_cCard4					= -1;
	pTableInfo->m_cCard5					= -1;

	pTableController->m_nCurrencyId			= pInfo->m_nCurrencyId;
	pTableController->m_nSBAmount			= pTableInfo->m_nSmallBlindAmount;
	pTableController->m_nBBAmount			= pTableInfo->m_nBigBlindAmount;
	pTableController->m_nAnteAmount			= pTableInfo->m_nAnteAmount;
	pTableController->m_nMinBuyIn			= 0; // Not supported
	pTableController->m_nMaxBuyIn			= 0; // Not supported
		
	pTableController->m_nHandStartDelay		= (int)pInfo->m_gameSpeed.m_shHandStartDelay;
	pTableController->m_nPostBlindDelay		= (int)pInfo->m_gameSpeed.m_shPostBlindDelay;
	pTableController->m_nStartRoundDelay	= (int)pInfo->m_gameSpeed.m_shStartRoundDelay;
	pTableController->m_nRegularTimeSec		= (int)pInfo->m_shActionTimeSec;
	pTableController->m_nExtraTimeSec		= 0;
	pTableController->m_nTimeBankSec		= (int)pInfo->m_shTimeBankInitTimeSec;
	pTableController->m_nTimeBankInc		= (int)pInfo->m_shTimeBankIncSec;
	pTableController->m_nTimeBankIncHands	= 0;
	pTableController->m_nTimeBankIncLevel	= (int)pInfo->m_shTimeBankIncLevel;
	pTableController->m_nShowMuckDelay		= (int)pInfo->m_gameSpeed.m_shShowMuckDelay;
	pTableController->m_nReserveSeatDelay	= (int)pInfo->m_gameSpeed.m_shReserveSeatDelay;
	pTableController->m_nLeaveSitoutedDelay	= (int)pInfo->m_gameSpeed.m_shLeaveSitOutedDelay;
	pTableInfo->m_shTimeBankSec				= pInfo->m_shTimeBankInitTimeSec;
	pTableInfo->m_btRegularActTimeSec		= pTableController->m_nRegularTimeSec;

	pTableController->m_gametype			= (GameType)pInfo->m_nGameTypeId;
	pTableController->m_subType				= (GameSubType)pInfo->m_nSubTypeId;
	pTableController->m_moneyType			= MoneyType::Play;
	return pTableController;
	}

TournamentTableController*
PokerTournamentManager::GetTableController(int nTableId){
	TournamentTableController* pTableController = NULL;
	m_lockGameTables.Lock(); // Lock
	int nIndex = m_arrGameTableById.IndexOf((void*)nTableId);
	if( nIndex != -1 )
		pTableController = (TournamentTableController*)m_arrGameTableById.GetData(nIndex);
	m_lockGameTables.Unlock(); // Unlock
	return pTableController;
	}

bool
PokerTournamentManager::IsRegisterUnregisterAllowed(bool bRegisterAction /*= true*/){
	return m_bRegistrationStatus;
	/* Waste of CPU 
	if( !bRegisterAction )
		return (m_info.m_nStatus != (int)TournamentStatus::Tournament_Registering);
	if( m_info.m_nStatus < ((int)TournamentStatus::Tournament_Registering) || m_info.m_nStatus > ((int)TournamentStatus::Tournament_InProgress)  )
		return false;
	if( m_nRegisteredCt >= m_info.m_nMaxPlayers )
		return false;
	// Check for late registration.
	COleDateTime dtTime = COleDateTime::GetCurrentTime();
	if( dtTime > m_info.m_dtRegEndTime )
		return false;
	return true;*/
	}

void
PokerTournamentManager::OnUserRegister(int nUserId, _String sUserName, PokerSession* pSession, bool bUnregister /*= false*/){
	if( !bUnregister ){// Registration.	
		// Start Sit & Go tournament.
		if( (m_info.m_nSubTypeId&((int)GameSubType::Tournament_SitNGo)) > 0 && (m_nRegisteredCt == m_info.m_nMaxPlayers) ){
			m_bRegistrationStatus = false;
			m_tournamentThread.StartThread((LPTHREAD_START_ROUTINE)&TournamentThread_Entry, this, THREAD_PRIORITY_NORMAL, SIZE_512KB, false);
			return;
			}
		}
	else{
		}
	}

bool
PokerTournamentManager::UserRebuyAction(int nUserId){
	// Change prize pool amount.
	return false;
	}

bool
PokerTournamentManager::UserAddonAction(int nUserId){
	// Change prize pool amount.
	return false;
	}

void
PokerTournamentManager::SetHandForHand(bool bHandForHand) { 
	bool bHandForHandOld = m_bHandForHand;
	m_bHandForHand = bHandForHand;
	// Make sync event signaled.
	if( bHandForHandOld && !bHandForHand && m_pSyncGameStartEvent ){
		if( ::WaitForSingleObject(*m_pSyncGameStartEvent, 0) != WAIT_OBJECT_0 )
			m_pSyncGameStartEvent->SetEvent();
		}
	}

void
PokerTournamentManager::SetFinishTournament(bool bFinish) { 
	bool bFinishOld	= bFinish;
	if( m_bFinishTournament )
		return;
	m_bFinishTournament	= bFinish;
	}

bool
PokerTournamentManager::IsRebuyAllowed(PokerSession* pSess, int& nRebuyCtLeft, int nRebuyCtAtOnce /*= 1*/){
	if( !m_bRebuyIsAllowed ) 
		return false; // Rebuy is not allowed.
	int nAddonCt = 0;
	if( !pSess->GetTournamentUserDetail_RebuyAddonCt(m_info.m_nId, nRebuyCtLeft, nAddonCt) || (nRebuyCtLeft - nRebuyCtAtOnce) < 0 )
		return false;
	return true;
	}

bool
PokerTournamentManager::IsAddonAllowed(PokerSession* pSess, int& nAddonCtLeft){
	if( !(m_bAddonBreak && m_bIsBreak) )
		return false; // Addon is not allowed.
	int nRebuyCt = 0;
	if( !pSess->GetTournamentUserDetail_RebuyAddonCt(m_info.m_nId, nRebuyCt, nAddonCtLeft) || nAddonCtLeft <= 0 )
		return false;
	return true;
	}

void
PokerTournamentManager::IsRebuyAddonAllowed(PokerSession* pUserSession, bool& bRebuyAllowed, bool& bAddonAllowed, int* pRebuyCtLeft /*= NULL*/, int* pAddonCtLeft /*= NULL*/){
	int nAddonCtLeft = 0, nRebuyCtLeft = 0;
	if( m_bRebuyIsAllowed ){
		bRebuyAllowed = true;
		}

	if( (m_bAddonBreak && m_bIsBreak) ){
		bAddonAllowed = true;
		}

	if( !pUserSession->GetTournamentUserDetail_RebuyAddonCt(m_info.m_nId, nRebuyCtLeft, nAddonCtLeft) ){
		bAddonAllowed = false;
		bRebuyAllowed = false;
		return;
		}

	if( pRebuyCtLeft )
		*pRebuyCtLeft = nRebuyCtLeft;

	if( pAddonCtLeft )
		*pAddonCtLeft = nAddonCtLeft;

	if( bRebuyAllowed )
		bRebuyAllowed = (nRebuyCtLeft > 0);

	if( bAddonAllowed )
		bAddonAllowed = (nAddonCtLeft > 0);
	}

bool
PokerTournamentManager::GetLevelStakes(int nLevel, UINT& nSB, UINT& nBB, UINT& nAnte){
	TournamentBlindStructure* pNextLevel = m_info.m_listBlindStructure.GetAt(nLevel);
	if( pNextLevel ){
		nSB		= (UINT)Serializable::ConvertMoney(pNextLevel->m_dSmallBlind);
		nBB		= (UINT)Serializable::ConvertMoney(pNextLevel->m_dBigBlind);
		nAnte	= (UINT)Serializable::ConvertMoney(pNextLevel->m_dAnte);
		return true;
		}
	return false; 
	};

///////////////////////////////////////////////////////////
///
///	Packet handler methods
///

void
PokerTournamentManager::OnOpenTable(PokerSession* pSess, ClientConnection* pConn, PokerOpenTable_TournamentPacket* p){
	// Looking for table by current user.
	if( p->m_bLookForMyTable ){
		if( !pConn->IsAuthorized() )
			return; // Authorization is required !!!
		int nUserId		= pSess->GetUserId();
		int nTableId	= 0;

		// Looking for table id.
		m_lockUsersPlaying.Lock();	// Lock
		int nIndex = m_arrUsersPlaying.IndexOf((void*)nUserId);
		if( nIndex != -1 )
			nTableId = (int)m_arrUsersPlaying.GetData(nIndex);
		m_lockUsersPlaying.Unlock(); // Unlock
		if( !nTableId )
			return; // Table couldn't found by that user id.
		p->m_nTableId = nTableId;
		}

	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	bool	bOpened			= false;
	int		nSeatIndex		= pSess->GetSeatIndex(p->m_nTableId, bOpened, p->m_nTournamentId);
	if( !bOpened )
		m_pServer->AddTableConnection(p->m_nTableId, p->m_nTournamentId, pConn, pController);

	if( m_bIsBreak ){
		p->m_btTableWaitingState	= 0;
		if( m_bWaitingToStartBreak )
			p->m_btTableWaitingState = (BYTE)PokerTableWaitingState::WaitingForTablesToStartBreak;
		else
			if( m_bIsBreak )
				p->m_btTableWaitingState = (BYTE)PokerTableWaitingState::WaitingBreakIsInProgress;

		p->m_shLevelSecLeft			= (short)(m_nBreakSecondsLeft);
		p->m_shBreakSecLeft			= (short)(0);
		}
	else{
		p->m_btTableWaitingState	= (pController->GThread_IsPaused() || !pController->GThread_IsStarted()) ? (BYTE)PokerTableWaitingState::WaitingForOtherTables : 0;
		p->m_shLevelSecLeft			= (short)(m_nLevelSecondsLeft);
		p->m_shBreakSecLeft			= (short)(m_nNextBreakSecondsLeft);

		if( m_bTournamentIsInProgress && m_nTournamentStartSecondsLeft > 1 ){
			p->m_btWaitSec			= m_nTournamentStartSecondsLeft; // Tournament will start in ??? seconds.
			p->m_btTableWaitingState= PokerTableWaitingState::WaitingForStartHandToStartTournament;
			}
		else
			p->m_btWaitSec			= 0;
		}

	int nRebuyCtLeft, nAddonCtLeft = 0;
	p->m_cLevelIndex			= (BYTE)m_nLevelIndex;
	p->m_nPlayersCt				= m_nPlayersCt;
	p->m_shTablesCt				= (WORD)m_nTableCt;
	p->m_bRebuyAllowed			= false;
	p->m_bAddonAllowed			= false;
	IsRebuyAddonAllowed(pSess, p->m_bRebuyAllowed, p->m_bAddonAllowed, &nRebuyCtLeft, &nAddonCtLeft);
	p->m_btRebuyCtLeft			= (BYTE)nRebuyCtLeft;
	p->m_btAddonCtLeft			= (BYTE)nAddonCtLeft;

	p->m_nRebuyChipsCt			= GetRebuyChipsCt()*100;
	p->m_nAddonChipsCt			= GetAddonChipsCt()*100;
	p->m_nRebuyAmount			= GetRebuyAmount();
	p->m_nRebuyVPPAmount		= GetRebuyVPPAmount();
	p->m_nAddonAmount			= GetAddonAmount();
	p->m_nAddonVPPAmount		= GetAddonVPPAmount();

	pController->OnOpenTable_Tournament(pSess, pConn, p);
	}

void
PokerTournamentManager::OnLeaveTable(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTable_TournamentPacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	
	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, p->m_nTournamentId);
	if( bOpened ){
		if( nSeatIndex > -1 )
			pController->OnLeaveTable_Tournament(pSess, pConn, p);

		// Send reply.
		m_pServer->SendPacket(pConn, p);
		// Remove table connection.
		m_pServer->RemoveTableConnection(p->m_nTableId, p->m_nTournamentId, pConn, pController);
		}
	}

void
PokerTournamentManager::OnAction(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	pController->OnAction(pSess, pConn, p);
	}

void
PokerTournamentManager::OnReturnBack(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	pController->OnReturnBack(pSess, pConn, p);
	}

void
PokerTournamentManager::OnSitOut(PokerSession* pSess, ClientConnection* pConn, PokerSitOutPacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	}

void
PokerTournamentManager::OnPostBlind(PokerSession* pSess, ClientConnection* pConn, PokerPostBlindPacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	}

void
PokerTournamentManager::OnShowMuckCards(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	pController->OnShowMuckCards(pSess, pConn, p);
	}

void
PokerTournamentManager::OnSetOption(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	pController->OnSetOption(pSess, pConn, p);
	}

void
PokerTournamentManager::OnChatMessage(PokerSession* pSess, ClientConnection* pConn, PokerChatMessagePacket* p){
	TournamentTableController* pController = GetTableController(p->m_nTableId);
	if( !pController ) return; // Table controller not have been found !!!
	pController->OnChatMessage(pSess, pConn, p);
	}

void
PokerTournamentManager::OnRebuy(PokerSession* pSess, ClientConnection* pConn, PokerRebuyPacket* p){
	int nRebuyCtLeft = 0, nRebuyMultipler = (p->m_btRebuyMultiplier == 2 ? 2 : 1);
	if( !IsRebuyAllowed(pSess, nRebuyCtLeft) )
		return; // Rebuy is not allowed !!!

	int nTableId = p->m_nTableId, nUserId = pSess->GetUserId();
	if( nUserId <= 0 || nTableId <= 0 )
		return; // User id is invalid !!!

	TournamentTableController* pController = GetTableController(nTableId);
	if( !pController )
		return; // Tournament table not found !!!

	INT_CHIPS nUserBalanceOld = 0;
	int nSeatIndex = pController->GetSeatIndexByUserId(nUserId, &nUserBalanceOld);
	if( nSeatIndex == -1 )
		return; // Seat is not occupied !!!

	if( nUserBalanceOld > GetStartingChips()*100 ){
		p->m_nErrorCode		= ES_ERROR_REBUY_FAIL_STILL_SUFFICIENT_CHIPS;
		p->m_nChipsAmountNew= 0;
		p->m_nChipsAdded	= 0;
		p->m_btRebuyCtLeft	= (BYTE)(nRebuyCtLeft);
		p->m_nBalanceNew	= pSess->GetBalance();
		p->m_fVPPBalanceNew	= (float)pSess->GetVPPoints();
		m_pServer->SendPacket(pConn, p); // Reply fail
		return; // User can't make rebuy because of chips amount.
		}

	// Add rebuy chips.
	int			nRebuyChipsCt = (GetRebuyChipsCt() * 100) * nRebuyMultipler;
	INT_CHIPS	nAddedChipsCt = pController->AddChips(nSeatIndex, (INT_CHIPS)nRebuyChipsCt, 
						(p->m_bByVPPAmount ? PokerTableController::AddChipsType::AddChips_RebuyVPPoints : PokerTableController::AddChipsType::AddChips_Rebuy));
	if( nAddedChipsCt > 0 ){
		m_tablesMan.PlayerAddedChips(nUserId, nUserBalanceOld, (nUserBalanceOld + nRebuyChipsCt));

		p->m_nErrorCode		= ES_ERROR_SUCCESS;
		p->m_nChipsAmountNew= (nUserBalanceOld + nRebuyChipsCt);
		p->m_nChipsAdded	= nRebuyChipsCt;
		p->m_btRebuyCtLeft	= (BYTE)(nRebuyCtLeft - 1);
		p->m_nBalanceNew	= pSess->GetBalance();
		p->m_fVPPBalanceNew	= (float)pSess->GetVPPoints();
		m_pServer->SendPacket(pConn, p); // Reply success

		// Send rebuy chips action.
		pController->SendSeatActionPacket(nSeatIndex, PokerActionTypes::AddChips, PokerSeatStatus::Sitted, nRebuyChipsCt);
		}
	else{
		if( nAddedChipsCt < 0 )
			p->m_nErrorCode	= ES_ERROR_CHIPS_WILL_ADD_AFTER_HANDFINISH;
		else
			p->m_nErrorCode	= ES_ERROR_REBUY_FAIL_INSUFFICIENT_AMOUNT;
		p->m_nChipsAmountNew= 0;
		p->m_nChipsAdded	= 0;
		p->m_btRebuyCtLeft	= (BYTE)(nRebuyCtLeft);
		p->m_nBalanceNew	= pSess->GetBalance();
		p->m_fVPPBalanceNew	= (float)pSess->GetVPPoints();
		m_pServer->SendPacket(pConn, p); // Reply fail
		return;
		}

	/*
	// Find out user's table id.
	m_lockObjects.Lock();	// Lock
	int nIndex = m_arrUsersPlaying.IndexOf((void*)nUserId);
	if( nIndex != -1 )
		nTableId = (int)m_arrUsersPlaying.GetData(nIndex);
	m_lockObjects.Unlock(); // Unlock

	if( nTableId <= 0 )
		return; // User's table id has not found !!!
	*/
	}

void
PokerTournamentManager::OnAddon(PokerSession* pSess, ClientConnection* pConn, PokerAddonPacket* p){
	int nAddonCtLeft = 0;
	if( !IsAddonAllowed(pSess, nAddonCtLeft) )
		return; // Rebuy is not allowed !!!

	int nTableId = p->m_nTableId, nUserId = pSess->GetUserId();
	if( nUserId <= 0 || nTableId <= 0 )
		return; // User id is invalid !!!

	TournamentTableController* pController = GetTableController(nTableId);
	if( !pController )
		return; // Tournament table not found !!!

	INT_CHIPS nUserBalanceOld = 0;
	int nSeatIndex = pController->GetSeatIndexByUserId(nUserId, &nUserBalanceOld);
	if( nSeatIndex == -1 )
		return; // Seat is not occupied !!!

	// Add addon chips.
	int			nAddonChipsCt = GetAddonChipsCt() * 100;
	INT_CHIPS	nAddedChipsCt = pController->AddChips(nSeatIndex, (INT_CHIPS)nAddonChipsCt, 
		(p->m_bByVPPAmount ? PokerTableController::AddChipsType::AddChips_AddonVPPoints : PokerTableController::AddChipsType::AddChips_Addon));
	if( nAddedChipsCt > 0 ){
		m_tablesMan.PlayerAddedChips(nUserId, nUserBalanceOld, (nUserBalanceOld + nAddonChipsCt));

		p->m_nErrorCode		= ES_ERROR_SUCCESS;
		p->m_nChipsAmountNew= nUserBalanceOld + nAddonChipsCt;
		p->m_nChipsAdded	= nAddonChipsCt;
		p->m_btAddonCtLeft	= (BYTE)(nAddonCtLeft - 1);
		p->m_nBalanceNew	= pSess->GetBalance();
		p->m_fVPPBalanceNew	= (float)pSess->GetVPPoints();
		m_pServer->SendPacket(pConn, p); // Reply success

		// Send addon chips action.
		pController->SendSeatActionPacket(nSeatIndex, PokerActionTypes::AddChips, PokerSeatStatus::Sitted, nAddonChipsCt);
		}
	else{
		if( nAddedChipsCt < 0 )
			p->m_nErrorCode		= ES_ERROR_CHIPS_WILL_ADD_AFTER_HANDFINISH;
		else// User can't make rebuy because of balance amount.
			p->m_nErrorCode		= ES_ERROR_ADDON_FAIL_INSUFFICIENT_AMOUNT;
		p->m_nChipsAmountNew= 0;
		p->m_nChipsAdded	= 0;
		p->m_btAddonCtLeft	= (BYTE)(nAddonCtLeft);
		p->m_nBalanceNew	= pSess->GetBalance();
		p->m_fVPPBalanceNew	= (float)pSess->GetVPPoints();
		m_pServer->SendPacket(pConn, p); // Reply fail
		return; 
		}
	}

///////////////////////////////////////////////////////////
///
///	Tournament table controllers callback methods.
///

void
PokerTournamentManager::OnHandStarted(TournamentTableController* pTable){
	if( m_bCancelling || m_bDestroying ) return;
	int nTableId = pTable->GetTableId();
	m_tablesMan.HandStarted(nTableId, pTable);

	m_lockGameTablesSync.Lock();	// Lock
	int nIndex = m_arrHandInProgressGameTableIds.IndexOf((void*)nTableId);
	if( nIndex == -1 )
		m_arrHandInProgressGameTableIds.Add((void*)nTableId);
	else
		ASSERT(FALSE);

	if( m_arrHandInProgressGameTableIds.GetCount() == m_nTableCt ){
		// Stop hand on all game tables.
		if( m_bHandForHand && !m_bIsBreak )
			m_pSyncGameStartEvent->ResetEvent();
		}
	/*
	m_nStartedTableCt ++;
	if( m_nStartedTableCt == m_nTableCt ){
		m_nStartedTableCt = 0;
		// Force each table to stop until all hand finishes.
		if( m_bHandForHand && !m_bIsBreak )
			m_pSyncGameStartEvent->ResetEvent();
		}*/
	m_lockGameTablesSync.Unlock(); // Unlock
	}

void
PokerTournamentManager::OnHandFinished(TournamentTableController* pTable, bool& bTerminateGame, int nHandAvgDurationSec){
	if( m_bCancelling || m_bDestroying ) 
		return;

	int nTableCtOld = m_nTableCt, nTableCtNew = 0, nTableId = pTable->GetTableId();
	if( !m_bFinishTournament ){
		m_tablesMan.HandFinished(nTableId, pTable, nTableCtNew);
		bTerminateGame	= !m_tablesMan.IsTableActive(nTableId);
		}

	// Update stakes statistic.
	INT_CHIPS nStakeLowest = 0, nStakeHighest = 0, nStakeAvg = 0;
	m_tablesMan.GetStakesStat(&nStakeHighest, &nStakeLowest, &nStakeAvg);
	m_nStakeHighest		= nStakeHighest;
	m_nStakeAverage		= nStakeAvg;
	m_nStakeLowest		= nStakeLowest;
	
	// Tournament finished !!!.
	if( (m_nPlayersCt == 1 && nTableCtNew == 1) || m_bFinishTournament ){
		m_nStakeHighest		= m_nStakeAverage = m_nStakeLowest = 0;
		m_nStartedTableCt	= 0;
		m_nFinishedTableCt	= 0;
		m_nTableCt			= 0;
		// Set complete tournament status.
		SetTournamentStatus(TournamentStatus::Tournament_Completed);
		bTerminateGame		= true;
		return;
		}

	m_lockGameTablesSync.Lock();		// Lock
	m_nTableCt			= nTableCtNew;
	int nIndex			= m_arrHandInProgressGameTableIds.IndexOf((void*)nTableId);
	if( nIndex != -1 )
		m_arrHandInProgressGameTableIds.Delete(nIndex);

	if( !m_arrHandInProgressGameTableIds.GetCount() ){
		// Start hand on all tables.
		if( (!m_bIsBreak && !m_bWaitingToStartBreak) && m_bHandForHand /*|| ::WaitForSingleObject(*m_pSyncGameStartEvent, 0) != WAIT_OBJECT_0*/ )//Syncstart event is not signaled.
			SyncStartPausedGameThreads(200, 50);
		}
	else{
		// Send wait for break packet.
		int nSecondsBeforeBreak = m_nNextBreakSecondsLeft;
		if( m_bWaitingToStartBreak /*|| (nSecondsBeforeBreak <= 5)*/ /*If thers is only 5 seconds left till break*/ ){
			PokerWaitForPacket packet;
			packet.m_nTournamentId		= m_info.m_nId;
			packet.m_nTableId			= nTableId;
			packet.m_cWaitFor			= PokerTableWaitingState::WaitingForTablesToStartBreak;
			packet.m_shWaitDurationSec	= (short)nSecondsBeforeBreak;
			m_pServer->SendPacketByTable(packet.m_nTableId, packet.m_nTournamentId, &packet, false);
			}	
		else // Send wait for hand start packet.
			if( m_bHandForHand ){
				PokerWaitForPacket packet;
				packet.m_nTournamentId		= m_info.m_nId;
				packet.m_nTableId			= nTableId;
				packet.m_cWaitFor			= PokerTableWaitingState::WaitingForOtherTables;
				packet.m_shWaitDurationSec	= 0;
				m_pServer->SendPacketByTable(packet.m_nTableId, packet.m_nTournamentId, &packet, false);
				}
		}
	m_lockGameTablesSync.Unlock(); // Unlock
	}

void
PokerTournamentManager::OnSeatEmpty(TournamentTableController* pTable, int nSeat, int nTableId, int nUserId){
	//TRACE("OnSeatEmpty TableId:%d Seat:%d UserId:%d\r\n", nTableId, nSeat, nUserId);
	m_nPlayersCt --; // Reduce players ct.

	// Remove from users list.
	m_lockUsersPlaying.Lock();	// Lock
	int nIndex = m_arrUsersPlaying.IndexOf((void*)nUserId);
	if( nIndex != -1 )
		m_arrUsersPlaying.Delete(nIndex);
	m_lockUsersPlaying.Unlock(); // Unlock
	}

void
PokerTournamentManager::OnSeatTaken(TournamentTableController* pTable, int nSeat, int nTableId, int nUserId){
	m_nPlayersCt ++; // Increase players ct.

	// Add in playing user's list.
	m_lockUsersPlaying.Lock();	// Lock
	m_arrUsersPlaying.Add((void*)nUserId, (void*)nTableId);
	m_lockUsersPlaying.Unlock(); // Unlock
	}

void
PokerTournamentManager::OnUserReseated(TournamentTableController* pTableDest, int nTableIdDest, int nSeatDest, TournamentTableController* pTableFrom, int nTableIdFrom, int nSeatFrom, int nUserId){
	//TRACE("OnUserReseated TableId:%d Seat:%d UserId:%d\r\n", nTableIdDest, nSeatDest, nUserId);
	// Update user's table.
	m_lockUsersPlaying.Lock();	// Lock
	int nIndex = m_arrUsersPlaying.IndexOf((void*)nUserId);
	if( nIndex != -1 )
		m_arrUsersPlaying.SetData(nIndex, (void*)nTableIdDest);
	m_lockUsersPlaying.Unlock(); // Unlock

	if( ((short)pTableFrom->GetEmptySeatCt()) == m_info.m_shMaxSeatCt && pTableFrom->m_bSyncState ){
		m_lockGameTablesSync.Lock();		// Lock
		int nIndex			= m_arrHandInProgressGameTableIds.IndexOf((void*)nTableIdFrom);
		if( nIndex != -1 )
			m_arrHandInProgressGameTableIds.Delete(nIndex);
		m_lockGameTablesSync.Unlock();		// Unlock
		/*
		_String sLog;
		sLog.Format(_T("OnUserReseated table in sync state id=%d"), nTableIdFrom);
		Logger::CreateLogEntry(sLog.GetBuffer());
		*/
		}
	}

void
PokerTournamentManager::OnGameTableCreated(TournamentTableController* pTable){
	int nTableId = pTable->GetTableId();
	m_pOwner->TournamentTableCreated(m_info.m_nId, nTableId, m_pParam, pTable);
	/*
	_String sLog;
	sLog.Format(_T("OnGameTableCreated id=%d"), nTableId);
	Logger::CreateLogEntry(sLog.GetBuffer());
	*/
	m_lockGameTables.Lock();	// Lock
	int nIndex = m_arrGameTableById.IndexOf((void*)nTableId);
	if( nIndex == -1 ){
		m_arrGameTableById	.Add((void*)nTableId, (void*)pTable);
		m_listTables		.Add(pTable);
		}
	m_lockGameTables.Unlock(); // Unlock
	ASSERT( nIndex == -1 );
	}

void
PokerTournamentManager::OnGameTableClosed(TournamentTableController* pTable){
	if( m_bDestroying ) return;
	int nTableId		= pTable->GetTableId();
	TRACE("OnGameTableClosed TableId:%d\r\n", nTableId);
	int nLeftSeatCt		= pTable->EmptyAllSeats();
	m_pOwner->GameTableClosed(m_info.m_nId, nTableId, m_pParam, pTable);
	/*
	_String sLog;
	sLog.Format(_T("OnGameTableClosed id=%d"), nTableId);
	Logger::CreateLogEntry(sLog.GetBuffer());
	*/
	if( !m_bCancelling ){
		m_lockGameTables.Lock();	// Lock
		m_listTables.Remove(pTable, true); // Destroy game table object.
		int nIndex = m_arrGameTableById.IndexOf((void*)nTableId);
		if( nIndex != -1 )
			m_arrGameTableById.Delete(nIndex);
		m_lockGameTables.Unlock(); // Unlock
		}
	}

///////////////////////////////////////////////////////////
///
///	Private methods
///

void
PokerTournamentManager::SetTournamentStatus(TournamentStatus statusNew){
	m_pOwner->TournamentStatusChanged(m_info.m_nId, m_pParam, statusNew);
	m_info.m_nStatus	= (int)statusNew;
	m_tournamentStatus	= statusNew;
	}

void
PokerTournamentManager::SetTournamentRegStatus(bool bRegIsOpen){
	m_bRegistrationStatus = bRegIsOpen;
	m_pOwner->TournamentRegistrationStatusChanged(m_info.m_nId, m_pParam, bRegIsOpen);
	}

int
PokerTournamentManager::GetSecondsUntilRegWillClose(){
	if( !m_bRegistrationStatus )
		return -1; // Registration is not even open !!!
	COleDateTime		dtCurrent	= COleDateTime::GetCurrentTime();
	COleDateTime		dtRegEnd	= m_info.m_dtRegEndTime;
	COleDateTimeSpan	dtSpan		= dtRegEnd - dtCurrent;
	int					nResult		= (int)dtSpan.GetTotalSeconds();
	// Close tournament status.
	if( nResult <= 0 ){
		SetTournamentRegStatus(false);
		nResult = -1;
		}
	return nResult;
	}

void
PokerTournamentManager::SetTournamentVisibility(bool bVisible){
	if( m_bVisible == bVisible ) return;
	m_pOwner->TournamentVisibilityChanged(m_info.m_nId, m_pParam, bVisible);
	m_bVisible = bVisible;
	}

int
PokerTournamentManager::SendBreakPacketToAllTables(bool bAddonBreak){
	m_lockGameTables.Lock(); // Lock
	int			nLoop			= 0, nTableCt = m_listTables.GetCount(), nTournamentId	= m_info.m_nId;
	PokerBreakPacket packet;
	packet.m_nTournamentId		= m_info.m_nId;
	packet.m_cLevelIndex		= (BYTE)m_nLevelIndex;
	packet.m_shBreakSecLeft		= (short)m_info.m_shBreakDurationMin*60;
	packet.m_bAddonBreak		= bAddonBreak;
	packet.m_nSmallBlind		= (UINT)Serializable::ConvertMoney(m_nextLevel.m_dSmallBlind);
	packet.m_nBigBlind			= (UINT)Serializable::ConvertMoney(m_nextLevel.m_dBigBlind);
	packet.m_nAnte				= (UINT)Serializable::ConvertMoney(m_nextLevel.m_dAnte);

	while( nLoop < nTableCt ){
		TournamentTableController* pTable = m_listTables.GetAt(nLoop);
		ASSERT(pTable);
		if( pTable ){
			packet.m_nTableId	= pTable->GetTableId();
			m_pServer->SendPacketByTable(packet.m_nTableId, nTournamentId, &packet, false);
			}
		nLoop ++;
		}
	m_lockGameTables.Unlock(); // Unlock
	return nLoop;
	}

int
PokerTournamentManager::PerformAutoAddonOnAllTables(){
	int	nRet	= 0;
	m_lockGameTables.Lock();	// Lock
	int	nLoop	= 0, nTableCt = m_listTables.GetCount();
	m_lockGameTables.Unlock();	// Unlock

	int nSleepPerTable	= 2, nLoopSleep	= 0;
	while( nLoop < nTableCt ){
		int nAddonsPerformed = 0;
		m_lockGameTables.Lock();	// Lock
		TournamentTableController* pTable = m_listTables.GetAt(nLoop);
		if( pTable ){
			nAddonsPerformed = pTable->PerformAutoAddonSeats();
			nRet			+= nAddonsPerformed;
			}
		m_lockGameTables.Unlock();	// Unlock

		if( nAddonsPerformed > 0 )
			nLoopSleep ++;

		if( nLoopSleep > 0 && !((nLoopSleep)%nSleepPerTable) ){
			if( SleepMSAndWait(500, 250) == MethodResult::Result_Stopped )
				return MethodResult::Result_Stopped;
			}
		nLoop ++;
		}
	return nRet;
	}

///////////////////////////////////////////////////////////
///
///	Tournament thread.
///

LRESULT __stdcall
PokerTournamentManager::TournamentThread_Entry(WinThread* pThread){
	PokerTournamentManager*	pMan						= (PokerTournamentManager*)pThread->GetThreadParameter();
	PokerServer*			pServer						= pMan->m_pServer;
	MethodResult			result;
	int						nOpenTableBeforeStartSec	= max(pMan->m_info.m_shOpenTableBeforeStartSec, 10);  // at least 10 sec to open table before tournament starts.
	int						nSeatPlayersOpElapseTimeMS	= 0;
	bool					bSitNGo						= ((pMan->m_info.m_nSubTypeId&((int)GameSubType::Tournament_SitNGo)) > 0);

	if( !bSitNGo ){
		result = pMan->PerformStartCountdown(nOpenTableBeforeStartSec);
		if( result == MethodResult::Result_Stopped ){
			return 0L; // Thread stopped by event.
			}
		}

	result = pMan->SeatPlayers(nSeatPlayersOpElapseTimeMS, nOpenTableBeforeStartSec);
	if( result == MethodResult::Result_Stopped ){
		return 0L; // Thread stopped by event.
		}

	if( result == MethodResult::Result_InsufficientTournamentRegistrants ){
		pMan->CancelTournament(true);
		return 0L; // Cancel tournament.
		}

	// Wait before start the tournament. {{
	nOpenTableBeforeStartSec	-= (nSeatPlayersOpElapseTimeMS / 1000);
	if( nOpenTableBeforeStartSec < 0 )
		nOpenTableBeforeStartSec = 0;
	pMan->m_nTournamentStartSecondsLeft = nOpenTableBeforeStartSec;

	int nMSCheckStopEvent	= 250;
	int nElapsedMS			= 0, nLoop = 0;
	int nLoopCt				= (nOpenTableBeforeStartSec*1000) / nMSCheckStopEvent;
	while(nLoop < nLoopCt){
		if( pThread->IsStopEventSignaled() )
			return MethodResult::Result_Stopped;
		Sleep(nMSCheckStopEvent);

		nElapsedMS	+= nMSCheckStopEvent;
		nLoop		++;
		if( nElapsedMS >= 1000 ){
			pMan->m_nTournamentStartSecondsLeft --;
			nElapsedMS = 0;
			}
		}
	// }}

	// Tournament competition lifecycle. {{
	int nLevel			= 19;//pMan->m_info.m_nCurrentLevelIndex;
	int nLevelCt		= pMan->m_info.m_listBlindStructure.GetCount();
	int nBreakPerLevel	= (int)pMan->m_info.m_shBreakPerLevel;
	int nAddonBreak		= (int)pMan->m_info.m_shAddonBreak;
	int nBreak			= ((nLevel) / nBreakPerLevel);

	if( pMan->m_info.m_shRebuyCt > 0 )
		pMan->m_bRebuyIsAllowed = (nLevel < (nAddonBreak*pMan->m_info.m_shBreakPerLevel));
	else
		pMan->m_bRebuyIsAllowed = false;

	_String sLog;
	//while( nLevel < nLevelCt ){
	while( TRUE ){
		// Set rebuy allow flag.
		if( nLevel < (nAddonBreak*pMan->m_info.m_shBreakPerLevel) )
			pMan->m_bRebuyIsAllowed = true;

		if( (nLevel > 0) && !(nLevel%nBreakPerLevel) ){
			// Addon break.
			if( ((nBreak + 1) == nAddonBreak) ){
				pMan->m_bRebuyIsAllowed = false;
				result = pMan->PerformBreak(true, true);
				}
			else
				result = pMan->PerformBreak(false, true);

			if( result == MethodResult::Result_Stopped )
				return 0L; // Thread stopped by event.
			nBreak ++;
			}
		/*
		sLog.Format(_T("Level : %d started"), nLevel);
		Logger::CreateLogEntry(sLog.GetBuffer());
		*/
		result = pMan->PerformLevel(nLevel);
		if( result == MethodResult::Result_Stopped )
			return 0L; // Thread stopped by event.
		else
		if( result == MethodResult::Result_TournamentCompleted )
			break; // Tournament completed.

		/*
		sLog.Format(_T("Level : %d finished"), nLevel);
		Logger::CreateLogEntry(sLog.GetBuffer());
		*/
		nLevel ++;
		}
	// }}

	// Finish tournament.
	pMan->FinishCompetition();
	return 0L;
	}

MethodResult
PokerTournamentManager::PerformStartCountdown(int nReturnBeforeStartSec /*= 0*/){
	if( m_info.m_nStatus == TournamentStatus::Tournament_Completed || 
		m_info.m_nStatus == TournamentStatus::Tournament_Canceled )
		return MethodResult::Result_TournamentCompleted;
	
	// Countdown until tournament appearance.
	int nSecondsUntilShow				= 0;
	int nSecondsUntilRegisterStart		= 0;
	int nSecondsUntilTournamentStart	= 0;

	if( m_info.m_nStatus == TournamentStatus::Tournament_Announced ){
		COleDateTime		dtCurrent	= COleDateTime::GetCurrentTime();
		COleDateTime		dtShow		= m_info.m_dtShowTime;
		COleDateTimeSpan	dtSpan		= dtShow - dtCurrent;
		nSecondsUntilShow				= (int)dtSpan.GetTotalSeconds();

		// Wait untill tournament is invisible.
		if( SleepAndWait(nSecondsUntilShow) != Result_Success ){
			return MethodResult::Result_Stopped;
			}
		// Make tournament visible.
		SetTournamentVisibility(true);
		}
	else{
		// Make tournament visible.
		SetTournamentVisibility(true);
		}

	if( m_info.m_nStatus < TournamentStatus::Tournament_Registering ){
		COleDateTime		dtCurrent	= COleDateTime::GetCurrentTime();
		COleDateTime		dtStart		= m_info.m_dtRegStartTime;
		COleDateTimeSpan	dtSpan		= dtStart - dtCurrent;
		nSecondsUntilRegisterStart		= (int)dtSpan.GetTotalSeconds();

		// Wait untill registration starts.
		if( SleepAndWait(nSecondsUntilRegisterStart) != Result_Success ){
			return MethodResult::Result_Stopped;
			}
		// Set tournament status.
		SetTournamentStatus(TournamentStatus::Tournament_Registering);
		}
	
	if( m_info.m_nStatus != TournamentStatus::Tournament_Starting ){
		// Open tournament registration.
		SetTournamentRegStatus(true);

		COleDateTime		dtCurrent	= COleDateTime::GetCurrentTime();
		COleDateTime		dtStart		= m_info.m_dtStartTime;
		COleDateTimeSpan	dtSpan		= dtStart - dtCurrent;
		nSecondsUntilTournamentStart	= (int)dtSpan.GetTotalSeconds();

		if( nReturnBeforeStartSec > 0 ){
			if( nReturnBeforeStartSec > nSecondsUntilTournamentStart )
				nSecondsUntilTournamentStart = 0; // Return without waiting.
			else
				nSecondsUntilTournamentStart -= nReturnBeforeStartSec;
			}

		// Wait untill tournament starts.
		if( SleepAndWait(nSecondsUntilTournamentStart) != Result_Success )
			return MethodResult::Result_Stopped;

		// Close registration.
		if( !m_bLateRegistration )
			SetTournamentRegStatus(false);

		// Set tournament status.
		SetTournamentStatus(TournamentStatus::Tournament_Starting);
		}

	return MethodResult::Result_Success;
	}

MethodResult
PokerTournamentManager::SeatPlayers(int& nElapsedTimeMS, int nStartTournamentTimeSec){
//	if( m_info.m_nStatus != TournamentStatus::Tournament_Starting )
//		return MethodResult::Result_Success;

	clock_t start	= StartCountdown();
	nElapsedTimeMS	= 0;
	if( m_nRegisteredCt < m_info.m_nMinPlayers ){
		// Set canceled status.
//		SetTournamentStatus(TournamentStatus::Tournament_Canceled);
		return MethodResult::Result_InsufficientTournamentRegistrants;
		}

	m_lockGameTables.Lock(); // Lock
	int nTableCt = m_pOwner->CreateTournamentGameTables(this, m_info.m_nId, &m_listTables, &m_arrGameTableById);
	if( !nTableCt ){
		m_lockGameTables.Unlock(); // Unlock
		return MethodResult::Result_CreateGameTablesFail;
		}

	m_nTableCt					= nTableCt;
	m_nPlayersCt				= m_nRegisteredCt;
	m_bTournamentIsInProgress	= true;
	m_bRebuyIsAllowed			= (m_info.m_shRebuyCt > 0);

	UINT nSB_Next = 0, nBB_Next = 0, nAnte_Next = 0;
	GetLevelStakes(1, nSB_Next, nBB_Next, nAnte_Next);

	// Initialize stake stat values.
	m_nStakeLowest				= m_info.m_nBuyInChips*100;
	m_nStakeHighest				= m_nStakeLowest;
	m_nStakeAverage				= m_nStakeLowest;

	// Initialize tables manager.
	m_tablesMan.Initialize(this, &m_listTables, (int)m_info.m_shMaxSeatCt);
	m_lockGameTables.Unlock(); // Unlock	

	// Prepare tables to start tournament. {{
	SetTournamentStatus(TournamentStatus::Tournament_Starting);

	int nSecSleepPerTable = 4;
	for(int i=0; i<nTableCt; i++){
		TournamentTableController* pTable = m_listTables.GetAt(i);
		if( pTable )
			pTable->SendOpenTournamentTableToSeats(-1, nSB_Next, nBB_Next, nAnte_Next, nStartTournamentTimeSec);

		if( !((i+1)%nSecSleepPerTable) ){
			if( SleepAndWait(1, 250) == MethodResult::Result_Stopped ){
				return MethodResult::Result_Stopped;
				}
			}
		}
	// }}

	// Calculate time sec till next break.	
	m_nNextBreakSecondsLeft = (m_info.m_shBreakPerLevel*m_info.m_shLevelDuration*60);

	// Set tournament status.
	SetTournamentStatus(TournamentStatus::Tournament_InProgress);
	// Tournament started.
	m_pOwner->TournamentStarted(m_info.m_nId, m_pParam);

	// Calculate elapsed time during performing method.
	nElapsedTimeMS			= EndCountdown(start);
	return MethodResult::Result_Success;
	}

MethodResult
PokerTournamentManager::PerformLevel(int nLevelIndex){
	TournamentBlindStructure*	pLevelBlinds		= m_info.m_listBlindStructure.GetAt(nLevelIndex);
	TournamentBlindStructure*	pNextLevel			= m_info.m_listBlindStructure.GetAt(nLevelIndex + 1);
	int							nBreakInEveryHourMin = (((int)m_info.m_cBreakTimeMinute) & 0xFF);
	bool						bLevelChanged		= true;

	if( !m_info.m_listBlindStructure.GetCount() )
		return MethodResult::Result_BlindStructureNotExists;

	if( !pLevelBlinds ){	
		pLevelBlinds	= m_info.m_listBlindStructure.GetAt(m_info.m_listBlindStructure.GetCount() - 1);
		pNextLevel		= NULL;
		bLevelChanged	= false;
		}

	// Copy level stakes. {{
	if( pLevelBlinds )
		pLevelBlinds->Copy(&m_currLevel);
	else
		m_currLevel.ZeroInit();

	if( pNextLevel )
		pNextLevel->Copy(&m_nextLevel);
	else
		m_nextLevel.ZeroInit();
	// }}

	m_lockGameTables.Lock();			// Lock
	int			nLoop					= 0;
	int			nTableCt				= m_listTables.GetCount();
	int			nGameThreadsStarted		= 0;
	const int	nSecSleepPerGameTable	= 10;
	m_lockGameTables.Unlock();			// Unlock

	// Set current level index.
	m_nLevelIndex						= nLevelIndex;
	// Tournament level changed.
	if( bLevelChanged )
		m_pOwner->TournamentLevelChanged(nLevelIndex, m_pParam);
	// Set level duration seconds.
	m_nLevelSecondsLeft					= m_info.m_shLevelDuration*60;
	// Calculate time sec till next break.	
	m_nNextBreakSecondsLeft				= (m_info.m_shBreakPerLevel - (nLevelIndex - (nLevelIndex / m_info.m_shBreakPerLevel)*m_info.m_shBreakPerLevel))*m_nLevelSecondsLeft;

	while( nLoop < nTableCt ){
		m_lockGameTables.Lock();	// Lock
		TournamentTableController* pTableController = m_listTables.GetAt(nLoop);
		m_lockGameTables.Unlock();	// Unlock

		if( !pTableController ){
			nLoop ++;
			continue;
			}

		// Set blind stakes.
		if( bLevelChanged )
			pTableController->SetBlindStakes(&m_currLevel, &m_nextLevel);

		if( !pTableController->GThread_IsStarted() ){
			pTableController->GThread_Start	(false); // Start game thread.
			nGameThreadsStarted ++;

			if( (nGameThreadsStarted)%nSecSleepPerGameTable == 0 ){
				// Sleep for second.
				if( SleepAndWait(1) == MethodResult::Result_Stopped )
					return MethodResult::Result_Stopped;
				}
			}
		nLoop ++;
		}

	// Start paused game threads syncronously. in milliseconds like this 500-525-550-575 ...
	SyncStartPausedGameThreads(500, 25);

	// Calculate seconds until registration close.
	int nSecUntelRegCloses = -1;
	if( m_bLateRegistration && m_bRegistrationStatus ){
		nSecUntelRegCloses = GetSecondsUntilRegWillClose();
		if( nSecUntelRegCloses > m_nLevelSecondsLeft )
			nSecUntelRegCloses = -1;
		}

	// Level countdown. {{
	if( nSecUntelRegCloses <= 0 ){
		while( m_nLevelSecondsLeft > 0 ){
			// Sleep for second.
			if( SleepAndWait(1) == MethodResult::Result_Stopped )
				return MethodResult::Result_Stopped;

			// Check for completion.
			if( m_nPlayersCt <= 1 )
				return MethodResult::Result_TournamentCompleted;

			m_nLevelSecondsLeft		--;
			m_nNextBreakSecondsLeft --;
			}
		}
	else{
		while( m_nLevelSecondsLeft > 0 ){
			// Sleep for second.
			if( SleepAndWait(1) == MethodResult::Result_Stopped )
				return MethodResult::Result_Stopped;

			// Check for completion.
			if( m_nPlayersCt <= 1 )
				return MethodResult::Result_TournamentCompleted;

			// Close tournament registration.
			nSecUntelRegCloses --;
			if( nSecUntelRegCloses <= 0 )
				SetTournamentRegStatus(false);					

			m_nLevelSecondsLeft		--;
			m_nNextBreakSecondsLeft --;
			}
		}
	// }}
		
	return MethodResult::Result_Success;
	}

MethodResult
PokerTournamentManager::PerformBreak(bool bAddonBreak, bool bKeepGameThreadsPaused){
	bool bHandForHand		= m_bHandForHand;
	m_bHandForHand			= false;
	m_pSyncGameStartEvent->ResetEvent();

	m_lockGameTablesSync.Lock();	// Lock
	int nTableCtPlaying		= m_arrHandInProgressGameTableIds.GetCount();
	m_lockGameTablesSync.Unlock();	// Unlock
	if( nTableCtPlaying > 0 ){
		m_bWaitingToStartBreak	= true;
		// Wait for tables to finish hand. 
		while(TRUE){
			// Sleep for second.
			if( SleepMSAndWait(1000) == MethodResult::Result_Stopped )
				return MethodResult::Result_Stopped;

			m_lockGameTablesSync.Lock();	// Lock
			nTableCtPlaying = m_arrHandInProgressGameTableIds.GetCount();
			m_lockGameTablesSync.Unlock();	// Unlock
			if( nTableCtPlaying == 0 ) break;
			}
		m_bWaitingToStartBreak	= false;
		}	

	// Initialize break parameters to start.
	m_bIsBreak				= true;
	m_bAddonBreak			= bAddonBreak;
	m_nBreakSecondsLeft		= m_info.m_shBreakDurationMin*60;
	m_nNextBreakSecondsLeft = 0;

	// Send break start packet to every tables.
	SendBreakPacketToAllTables(bAddonBreak);

	// Perform auto addon on all tables.
	if( bAddonBreak ){
		clock_t start = clock();
		PerformAutoAddonOnAllTables();
		clock_t end = clock();
		double dMilliSeconds = (1000*(end-start)/CLOCKS_PER_SEC);
		m_nBreakSecondsLeft -= (int)(dMilliSeconds / 1000);
		if( m_nBreakSecondsLeft < 0 )
			m_nBreakSecondsLeft = 0;
		}

	// Calculate seconds until registration will close.
	int nSecUntelRegCloses = -1;
	if( m_bLateRegistration && m_bRegistrationStatus ){
		nSecUntelRegCloses = GetSecondsUntilRegWillClose();
		if( nSecUntelRegCloses > m_nBreakSecondsLeft )
			nSecUntelRegCloses = -1;
		}

	if( nSecUntelRegCloses <= 0 ){
		while( m_nBreakSecondsLeft > 0 ){
			// Sleep for second.
			if( SleepMSAndWait(1000) == MethodResult::Result_Stopped )
				return MethodResult::Result_Stopped;
			m_nBreakSecondsLeft --;
			}
		}
	else{
		while( m_nBreakSecondsLeft > 0 ){
			// Sleep for second.
			if( SleepMSAndWait(1000) == MethodResult::Result_Stopped )
				return MethodResult::Result_Stopped;
			// Close tournament registration.
			nSecUntelRegCloses --;
			if( nSecUntelRegCloses == 0 )
				SetTournamentRegStatus(false);
			m_nBreakSecondsLeft --;
			}
		}

	m_bIsBreak			= false;
	m_bAddonBreak		= false;
	m_bHandForHand		= bHandForHand;

	if( !bKeepGameThreadsPaused )
		m_pSyncGameStartEvent->SetEvent();
	// }}
	return MethodResult::Result_Success;
	}

MethodResult
PokerTournamentManager::FinishCompetition(){
	m_bTournamentIsInProgress = false;
	// Tournament finished.
	m_pOwner->TournamentFinished(m_info.m_nId, m_pParam);
	return MethodResult::Result_Success;
	}

MethodResult
PokerTournamentManager::SleepAndWait(int nSecWait, int nMSecCheckStopEvent /*= 250*/){
	// Negative second value.
	if( nSecWait <= 0 ) 
		return MethodResult::Result_Success;
	int nLoopCt = (nSecWait*1000) / nMSecCheckStopEvent, nLoop = 0;
	while(nLoop < nLoopCt){
		if( m_tournamentThread.IsStopEventSignaled() )
			return MethodResult::Result_Stopped;
		// Sleep
		Sleep(nMSecCheckStopEvent);
		nLoop ++;
		}
	return MethodResult::Result_Success;
	}

MethodResult
PokerTournamentManager::SleepMSAndWait(int nMSecWait, int nMSecCheckStopEvent /*= 250*/){
	// Negative second value.
	if( nMSecWait <= 0 ) 
		return MethodResult::Result_Success;
	int nLoopCt = nMSecWait / nMSecCheckStopEvent, nLoop = 0;
	while(nLoop < nLoopCt){
		if( m_tournamentThread.IsStopEventSignaled() )
			return MethodResult::Result_Stopped;
		// Sleep
		Sleep(nMSecCheckStopEvent);
		nLoop ++;
		}
	return MethodResult::Result_Success;
	}

MethodResult
PokerTournamentManager::SyncStartPausedGameThreads(int nWaitMSecInitial, int nWaitMSecInc){
	if( ::WaitForSingleObject(*m_pSyncGameStartEvent, 0) == WAIT_OBJECT_0 )
		return MethodResult::Result_Success; // Sync start event already signaled!

	int	nLoop = 0, nTableCt = m_listTables.GetCount();
	while( nLoop < nTableCt ){
		TournamentTableController* pTableController = m_listTables.GetAt(nLoop);
		ASSERT(pTableController);
		pTableController->SetSyncEventWaitMSec(nWaitMSecInitial);
		nWaitMSecInitial	+= nWaitMSecInc;
		nLoop				++;
		}

	// Signal sync start event.
	m_pSyncGameStartEvent->SetEvent();
	return MethodResult::Result_Success;
	}

clock_t
PokerTournamentManager::StartCountdown(){
	return clock();
	}

int
PokerTournamentManager::EndCountdown(clock_t start){
	// Calculate elapsed time.
	clock_t end				= clock();
	double dMilliSeconds	= (1000*(end-start)/CLOCKS_PER_SEC);
	return (int)(dMilliSeconds);
	}