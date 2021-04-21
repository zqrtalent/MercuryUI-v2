#include "stdafx.h"
#include "../GameServer/ClientConnection.h"
#include "../Protocol/PokerPacket.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"
#include "../MD5/MD5Checksum.h"
#include "MaximumWebService.h"

#include "Log/Logger.h"
#include "PokerServer.h"
#include "PokerCashGames.h"

#include "Data/PokerCashData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerCashGames
///
///////////////////////////////////////////////////////////

PokerCashGames::PokerCashGames() : PokerGameBase(){
	m_pData = NULL;
	} 

PokerCashGames::~PokerCashGames(){
	Stop();
	}

void
PokerCashGames::Initialize(PokerServer* pServer, PacketHandler* pPacketHandler){
	if( m_bInited ) return;
	ASSERT(pServer && pPacketHandler);
	m_pServer			= pServer;
	m_pPacketHandler	= pPacketHandler;
	m_bInited			= true;
	}

bool
PokerCashGames::Start(PokerServerConfig* pConfig, LobbyInfo* pLobbyInfo){
	ASSERT(pConfig && pLobbyInfo);
	if( !pConfig || !pLobbyInfo) return false;
	if( !m_pData )
		m_pData = new PokerCashData();
	else
		m_pData->FreeData(true);

	// Set connection string.
	if( !pConfig->m_sSQLConnect.IsEmpty() )
		m_pData->SetConnString(pConfig->m_sSQLConnect);
	// Set avatars folder.
	m_pData->SetAvatarsFolder(pConfig->m_sAvatarsFolder);
	// Load data from database.
	bool bRet = m_pData->LoadData();
	// Unseat users who has remain seated.
	m_pData->UnseatUsers();

	// Create game controllers.
	m_lockGames.Lock();		// Lock
	m_pData->CreateGameTableControllers(m_pServer, this, &m_listActiveGames, &m_arrActiveGameById);
	m_lockGames.Unlock();	// Unlock
	
	// Setup lobby info.
	SetupLobbyInfo(pLobbyInfo);
	m_pLobbyInfo = pLobbyInfo;
	return bRet;
	}

bool
PokerCashGames::Stop(){
	// Stop all game controllers.
	m_lockGames.Lock();		// Lock
	for(int i=0; i<m_listActiveGames.GetCount(); i++){
		PokerTableController* pController = m_listActiveGames.GetAt(i);
		if( pController )
			pController->SendStopSignal();
		}
	m_listActiveGames	.RemoveAll(true);
	m_arrActiveGameById	.DeleteAll();
	m_lockGames.Unlock();	// Unlock

	// Destroy database object.
	if( m_pData ){
		m_pData->FreeData(true);
		delete m_pData;
		m_pData = NULL;
		}
	m_pLobbyInfo = NULL;
	return true;
	}

bool
PokerCashGames::SetupLobbyInfo(LobbyInfo* pLobbyInfo){
	if( !m_pData ) return false;
	//pLobbyInfo->EmptyInfo();
	int nLoop = 0, nCt = m_pData->m_listGameTable.GetCount();
	while( nLoop < nCt ){
		PokerGameTable* pTable = m_pData->m_listGameTable.GetAt(nLoop);
		if( !pTable ){
			nLoop ++;
			continue;
			}
		pLobbyInfo->SetTable(pTable);
		nLoop ++;
		}
	return true;
	}

PokerCashTableController*
PokerCashGames::GetGameController(int nTableId){
	m_lockGames.Lock(); // Lock
	int nIndex = m_arrActiveGameById.IndexOf((void*)nTableId);
	if( nIndex == -1 ){
		m_lockGames.Unlock(); // Unlock
		return NULL;
		}
	PokerCashTableController* pGame = (PokerCashTableController*)m_arrActiveGameById.GetData(nIndex);
	m_lockGames.Unlock(); // Unlock
	return pGame;
	}

///////////////////////////////////////////////////////////
///
///	Packet handler methods.
///
///////////////////////////////////////////////////////////

void	
PokerCashGames::OnOpenTable(PokerOpenTablePacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() ) return;

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return; // Table not found !!!

	bool bOpened	= false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( !bOpened ){
		m_pServer->AddTableConnection(p->m_nTableId, 0, pConn, pController);
		}
	pController->OnOpenTable(pSess, pConn, p);
	}

void
PokerCashGames::OnLeaveTable(PokerLeaveTablePacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() ) return;

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;

	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( bOpened ){
		pController->OnLeaveTable(pSess, pConn, p);
		// Remove table connection.
		m_pServer->RemoveTableConnection(p->m_nTableId, 0, pConn, pController);
		}
	}

void
PokerCashGames::OnReserveSeat(PokerReserveSeatPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() )		return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;

	// Table wouldn't be seated or reserved.
	if( pSess->IsTableObserver(p->m_nTableId, 0) )
		pController->OnReserveSeat(pSess, pConn, p);
	}

void
PokerCashGames::OnCancelReservation(PokerCancelReservationPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;
	// Cancel seat reservation.
	pController->OnCancelReservation(pSess, pConn, p);
	}

void
PokerCashGames::OnSeatAt(PokerSeatAtTablePacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() )		return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;
	// Seat at table.
	pController->OnSeatAt(pSess, pConn, p);
	}

void
PokerCashGames::OnJoinWaitingList(PokerJoinWaitingListPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;

	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( bOpened){ // Connection must not be seated and must be opened table.
		p->m_sUserName	= pSess->GetUserName();
		p->m_nUserId	= pSess->GetUserId();
		pController->OnJoinWaitingList(pSess, pConn, p);
		}
	}

void
PokerCashGames::OnAddChips(PokerAddChipsPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() )		return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;

	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( nSeatIndex == -1 ) return; // Connection is not sitted.
	p->m_cSeat = (char)nSeatIndex;
	pController->OnAddChips(pSess, pConn, p);
	}

void
PokerCashGames::OnAction(PokerActionPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;

	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( nSeatIndex == -1 ) return; // Connection is not sitted.

	// Ensure seat index to be valid.
	p->m_cSeat	= (char)nSeatIndex;
	pController->OnAction(pSess, pConn, p);
	}

void
PokerCashGames::OnTableInfo(PokerTableInfoPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;
	pController->OnTableInfo(pSess, pConn, p);
	}

void
PokerCashGames::OnSitOut(PokerSitOutPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;
	}

void
PokerCashGames::OnPostBlind(PokerPostBlindPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;
	}

void
PokerCashGames::OnShowMuckCards(PokerShowMuckCardsPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() )		return; // Need authorization.
	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;

	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( nSeatIndex == -1 ) return; // User is not sitted.
	//Ensure seat index to be valid.
	p->m_cSeat	= (char)nSeatIndex;
	pController->OnShowMuckCards(pSess, pConn, p);
	}

void
PokerCashGames::OnSetOption(PokerSetOptionPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( nSeatIndex == -1 ) return; // User is not sitted.
	//Ensure seat index to be valid.
	p->m_cSeat	= (char)nSeatIndex;
	
	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;
	pController->OnSetOption(pSess, pConn, p);
	}

void
PokerCashGames::OnReturnBack(PokerReturnBackPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() )		return; // Need authorization.

	// Get seat index by connection.
	bool bOpened = false;
	int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( nSeatIndex == -1 ) return; // User is not sitted.
	//Ensure seat index to be valid.
	p->m_cSeat	= (char)nSeatIndex;

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;
	pController->OnReturnBack(pSess, pConn, p);
	}

void
PokerCashGames::OnChatMessage(PokerChatMessagePacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() ) return;
	if( !pConn->IsAuthorized() || pSess->IsTableObserver(p->m_nTableId, 0) ) return; // Need authorization and must be seated already.

	PokerCashTableController* pController = GetGameController(p->m_nTableId);
	if( !pController ) return;

	bool bOpened = false;
	p->m_message.m_cSeat = (char)pSess->GetSeatIndex(p->m_nTableId, bOpened, 0);
	if( bOpened ){
		p->m_message.m_sUserName = pSess->GetUserName();
		pController->OnChatMessage(pSess, pConn,  p);
		}
	}

void
PokerCashGames::OnOpenRandomTable(PokerOpenRandomTablePacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() ) return;
	if( !pConn->IsAuthorized() ) return; // Need authorization

	// Find table to open automatically.
	AutoSortedArray notInTableIds;
retry:
	int nTableId = m_pLobbyInfo->GetTableId(p->m_nGameType, p->m_nGameSubType, true, notInTableIds);
	bool bOpened = false;

	while( nTableId > 0 && pSess->GetSeatIndex(nTableId, bOpened, 0) != -1 ){
		notInTableIds.Add((void*)nTableId);
		nTableId = m_pLobbyInfo->GetTableId(p->m_nGameType, p->m_nGameSubType, true, notInTableIds);
		}
	// }}

	// Open table automatically.
	if( nTableId > 0 ){
		// Table is full retry another one.
		PokerCashTableController* pController = GetGameController(nTableId);
		if( pController ){
			int nSeat;
			pController->GetEmptySeat(nSeat);
			if( nSeat == -1 ) {
				notInTableIds.Add((void*)nTableId);
				goto retry;
				}
			}

		PokerOpenTablePacket p1;
		p1.m_nSessionId				= p->m_nSessionId;
		p1.m_cSeat					= (char)-1;
		p1.m_bSeatOrJoinWaitList	= p->m_bReserveSeat;
		p1.m_nTableId				= nTableId;
		p1.m_nErrorCode				= 0;
		OnOpenTable(&p1, pConn);
		}
	}

void
PokerCashGames::OnGetGameTables(PokerGetGameTablesPacket* p, ClientConnection* pConn){
	p->m_listResult.RemoveAll(true);
	m_pData->GetGameTableInfoObjects((int)p->m_cGameType, (int)p->m_shSubType, &p->m_listResult);
	int nPlayersCt, nWaitingCt, nHandPerHour;
	float fAvgPotAmount = 0.0f;
	for(int i=0; i<p->m_listResult.GetCount(); i++){
		PokerGameTableInfoObject* pInfo = p->m_listResult.GetAt(i);
		if( pInfo ){
			if( m_pLobbyInfo->GetTableAttributes(pInfo->m_nId, nPlayersCt, nWaitingCt, fAvgPotAmount, nHandPerHour) ){
				pInfo->m_cPlayersCt			= (char)nPlayersCt;
				pInfo->m_cWaitingPlayersCt	= (char)nWaitingCt;
				pInfo->m_fAvgPotAmount		= fAvgPotAmount;
				pInfo->m_shHandPerHour		= (short)nHandPerHour;
				}
			}
		}
	p->m_nErrorCode	= PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
	m_pServer->SendPacket(pConn, p);
	}

void
PokerCashGames::OnCreateGameTable(PokerCreateGameTablePacket* p, ClientConnection* pConn){
	if( p->m_tableInfo.GetCount() < 1 || p->m_tableInfo.GetAt(0) == NULL ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;
		m_pServer->SendPacket(pConn, p);
		return;
		}

	CreateGameTableObject*	pTableObject	= p->m_tableInfo.GetAt(0);
	if( pTableObject->m_sName.IsEmpty() || 
		pTableObject->m_fSmallBlind <= 0.0f ||
		pTableObject->m_fBigBlind <= 0.0f ||
		!IS_VALID_GAME_TYPE(pTableObject->m_cGameType)||
		!IS_VALID_GAME_SUB_TYPE(pTableObject->m_shGameSubType)||
		pTableObject->m_fRakePercent <= 0.0f ||
		pTableObject->m_cMaxSeatCt >= 11 
		){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;
		m_pServer->SendPacket(pConn, p);
		return;
		}
	
	PokerTableController*	pController	= NULL;
	int						nTableId	= m_pData->CreateGameTable(m_pServer, this, pTableObject, pController);
	if( nTableId <= 0 ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;
		m_pServer->SendPacket(pConn, p);
		return;
		}

	// Make game table visible in lobby. {{
	PokerGameTable table;
	if( m_pData->GetGameTable(nTableId, &table) )
		m_pLobbyInfo->SetTable(&table);
	// }}

	m_lockGames.Lock();		// Lock
	m_listActiveGames	.Add(pController);
	m_arrActiveGameById	.Add((void*)nTableId, (void*)pController);
	m_lockGames.Unlock();	// Unlock

	p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;
	p->m_nTableId	= nTableId;
	m_pServer->SendPacket(pConn, p);
	}

void
PokerCashGames::OnDeactivateGameTable(PokerDeactivateGameTablePacket* p, ClientConnection* pConn){
	}

// ======================================================================
//
//	Other usefull methods.
//
// ======================================================================

bool
PokerCashGames::UserAddChips(PokerSession* pSess, PokerTableController* pController, void* pParam, int nTableId,  int nSeat, INT_CHIPS nAmount, int nCurrencyId){
	if( !pSess || !m_pData ) return false;
	int			nUserId		= pSess->GetUserId();
	INT_CHIPS	nBalanceNew = 0;
	bool		bRet		= false;

	// Playmoney
	if( nCurrencyId == 0 ){
		bRet = m_pData->UserAddChipsPlaymoney(nUserId, nTableId, nSeat, nAmount, nBalanceNew);
		if( bRet )
			pSess->SetUserPlayBalance(nBalanceNew);
		}
	else{
		bRet = m_pData->UserAddChips(nUserId, nTableId, nSeat, nAmount, nCurrencyId, nBalanceNew);
		if( bRet )
			pSess->SetUserBalance(nBalanceNew);
		}
	return bRet;
	}

bool
PokerCashGames::GetSeatMinBuyIn(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS& nAmount, INT_CHIPS& nBalance){
	if( !m_pData ) return false;
	bool bRet = m_pData->GetSeatMinBuyIn(nUserId, nTableId, nCurrencyId, nAmount, nBalance);
	return bRet;
	}

bool
PokerCashGames::SeatUser(PokerSession* pSess, int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount){
	if( !m_pData || !pSess ) return false;
	INT_CHIPS	nBalanceNew = 0;
	bool		bRet		= false;

	// Playmoney
	if( nCurrencyId == 0 ){
		bRet = m_pData->SeatUserPlaymoney(nUserId, nTableId, nAmount, nBalanceNew);
		if( bRet && pSess )
			pSess->SetUserPlayBalance(nBalanceNew);
		}
	else{
		bRet = m_pData->SeatUser(nUserId, nTableId, nCurrencyId, nAmount, nBalanceNew);
		if( bRet && pSess )
			pSess->SetUserBalance(nBalanceNew);
		}
	return bRet;
	}

bool
PokerCashGames::UnseatUser(PokerSession* pSess, int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount){
	if( !m_pData ) return false;
	INT_CHIPS	nBalanceNew		= 0;
	bool		bRet			= false;
	
	// Playmoney
	if( nCurrencyId == 0 ){
		bRet = m_pData->UnseatUserPlaymoney(nUserId, nTableId, nAmount, nBalanceNew);
		if( bRet && pSess )
			pSess->SetUserPlayBalance(nBalanceNew);
		}
	else{
		bRet = m_pData->UnseatUser(nUserId, nTableId, nCurrencyId, nAmount, nBalanceNew);
		if( bRet && pSess )
			pSess->SetUserBalance(nBalanceNew);
		}
	return bRet;
	}

// ======================================================================
//
//	Events from game controller.
//
// ======================================================================

bool
PokerCashGames::PlayerSeated(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo,  int nSeat, INT_CHIPS nSeatAmount, PokerTableSeatInfo* pSeat){
	ASSERT(pInfo);
	if( !pSeat ) return false;
	m_pLobbyInfo->SeatTablePlayer(pInfo->m_nTableId, nSeat, pSeat);

	ClientConnection*	pConn	= pSeat->GetConnection();
	PokerSession*		pSess	= pConn ? (PokerSession*)pConn->GetSession(false) : (PokerSession*)pSeat->GetSession(false);
	int					nUserId	= pSeat->GetUserId();

	m_lockGames	.Lock();	// Lock
	if( pController )
		((PokerCashTableController*)pController)->RemoveFromWaitList(pSess->GetConnection(false, false));
	m_lockGames.Unlock();	// Unlock
	if( !SeatUser(pSess, nUserId, pInfo->m_nTableId, (int)pInfo->m_cCurrencyId, nSeatAmount) )
		return false;
	pSess->SeatAt(pInfo->m_nTableId, nSeat, 0);
	return true;
	}

void
PokerCashGames::PlayerLeftSeat(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, PokerTableSeatInfo* pSeat){
	ASSERT(pSeat && pInfo);
	if( !pSeat ) return;

	m_pLobbyInfo->UnseatTablePlayer(pInfo->m_nTableId, pSeat->GetUserId());
	ClientConnection*	pConn	= pSeat->GetConnection();
	PokerSession*		pSess	= pConn ? (PokerSession*)pConn->GetSession(false) : (PokerSession*)pSeat->GetSession(false);
	if( pSess )
		pSess->Unseat(pInfo->m_nTableId, 0);
	UnseatUser(pSess, pSeat->GetUserId(), pInfo->m_nTableId, (int)pInfo->m_cCurrencyId, pSeat->GetBalance());

	m_lockGames.Lock();		// Lock
	//PokerCashTableController* pController = GetGameController(pInfo->m_nTableId);
	if( pController ){
		AutoSortedArray arrSeated;
		((PokerCashTableController*)pController)->SeatWaitListUsers(arrSeated);
		}
	m_lockGames.Unlock();	// Unlock
	}

void
PokerCashGames::PlayerReservedSeat(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nSeat){
	PokerTableSeatInfo seat;
	if( pSess ){
		UserInfo user;
		if( pSess->CopyUserInfo(user) ){
			seat.SetUserName(user.m_sUserName);
			seat.SetUserId	(user.m_nUserId);
			seat.SetBalance	(0);
			}
		// Seat session player.
		pSess->SeatAt(nTableId, nSeat, 0);
		// Seat lobby player.
		m_pLobbyInfo->SeatTablePlayer(nTableId, nSeat, &seat);
		}

	m_lockGames	.Lock();		// Lock
	if( pController )
		((PokerCashTableController*)pController)->RemoveFromWaitList(pSess->GetConnection(false, false));
	m_lockGames.Unlock();		// Unlock
	}

void
PokerCashGames::PlayerCanceledReservation(PokerTableController* pController, void* pParam, PokerSession* pSess, int nUserId, int nTableId, int nSeat){
	if( pSess )
		pSess->Unseat(nTableId, 0);
	bool bSuccess = m_pLobbyInfo->UnseatTablePlayer(nTableId, nUserId);

	m_lockGames	.Lock();		// Lock
	if( pController ){
		AutoSortedArray arrSeated;
		((PokerCashTableController*)pController)->SeatWaitListUsers(arrSeated);
		}
	m_lockGames.Unlock();		// Unlock

	if( !bSuccess ){
		_String sLog;
		sLog.Format(_T("UnseatPlayerFailed TableId=%d Seat=%d"), nTableId, nSeat);
		Logger::CreateLogEntry(sLog.GetBuffer());
		}
	}

void
PokerCashGames::PlayerAddedInWaitList(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId){
	PokerTableSeatInfo seat;
	if( pSess ){
		UserInfo user;
		if( pSess->CopyUserInfo(user) ){
			seat.SetUserName(user.m_sUserName);
			seat.SetUserId	(user.m_nUserId);
			seat.SetBalance	(0);
			}
		// Seat session player.
		pSess->SeatAt(nTableId, -1, 0);
		// Seat lobby player.
		m_pLobbyInfo->SeatTablePlayer(nTableId, -1, &seat);
		}
	}

void
PokerCashGames::PlayerRemovedFromWaitList(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId){
	ASSERT(pSess);
	if( !pSess ) return;
	pSess->Unseat(nTableId, 0);
	m_pLobbyInfo->UnseatTablePlayer(nTableId, pSess->GetUserId());
	}

void
PokerCashGames::PlayerSeatBalanceChanged(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nCurrencyId, int nSeat, INT_CHIPS nSeatBalanceNew, INT_CHIPS nSeatBalanceOld){
	m_pLobbyInfo->SetTableSeatAmount(nTableId, nSeat, nSeatBalanceNew);
	if( pSess ){
		if( nCurrencyId > 0 ) // Real balance
			pSess->SetUserInPlay(pSess->GetInPlayBalance() + (nSeatBalanceNew - nSeatBalanceOld));
		else // Play balance
			pSess->SetUserInPlayPlayBalance(pSess->GetInPlayPlayBalance() + (nSeatBalanceNew - nSeatBalanceOld));
		}
	}

bool
PokerCashGames::HandStarted(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64& nHandId, int nTableId, int nPlayersCt, UINT nSB, UINT nBB, UINT nAnte){
	if( !m_pData ) return false;
	nHandId = m_pData->StartHand(nTableId, nPlayersCt);
	return true;
	}

bool
PokerCashGames::HandFinished(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64 nHandId, INT_CHIPS nTotalPotAmount, UINT nRakeAmount, PokerHandHistory* pHandHistory, _String& sPlayersInfo, _String& sTableCards){
	if( !m_pData || !pHandHistory ) return false;

	// Playmoney game table.
	if( !pInfo->m_cCurrencyId )
		m_pData->FinishHandPlaymoney(nHandId, pInfo->m_nTableId, nTotalPotAmount, nRakeAmount, sTableCards, sPlayersInfo);
	else{
		// Save hand history as binary.
		GrowableMemory mem(0x400, 0x400, true);
		if( pHandHistory->Serialize(&mem) )
			m_pData->FinishHand(nHandId, pInfo->m_nTableId, pInfo->m_cCurrencyId, nTotalPotAmount, nRakeAmount, sTableCards, sPlayersInfo, mem.GetBufferPtr(), mem.GetUsedBufferSize());
		}

	m_pLobbyInfo->UpdateTableInfo(pInfo);

	// Update statistics.
//	m_pData->UpdateGameTableInfo(pInfo);
	return true;
	}