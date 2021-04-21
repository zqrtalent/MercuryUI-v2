#include "stdafx.h"
#include "PokerServer.h"
#include "PokerTournaments.h"
#include "../Utility/GrowableMemory.h"
#include "Data/PokerTournamentData.h"
#include "Object/LobbyTournamentSummaryInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerTournaments
///
///////////////////////////////////////////////////////////

PokerTournaments::PokerTournaments() : PokerGameBase(){
	m_pData = NULL;
	// Create variable index.
	m_listTournaments.CreateVariableIndex(_T("TournamentId"), Serializable::Compare_Int32);
	} 

PokerTournaments::~PokerTournaments(){
	Stop();
	}

void
PokerTournaments::Initialize(PokerServer* pServer, PacketHandler* pPacketHandler){
	if( m_bInited ) return;
	ASSERT(pServer && pPacketHandler);
	m_pServer			= pServer;
	m_pPacketHandler	= pPacketHandler;
	m_bInited			= true;
	}

bool
PokerTournaments::Start(PokerServerConfig* pConfig, LobbyInfo* pLobbyInfo){
	ASSERT(pConfig && pLobbyInfo);
	if( !pConfig || !pLobbyInfo) return false;

	// Setup lobby info.
	m_pLobbyInfo = pLobbyInfo;

	if( !m_pData )
		m_pData = new PokerTournamentData();
	else
		m_pData->FreeData(true);

	// Set connection string.
	if( !pConfig->m_sSQLConnect.IsEmpty() )
		m_pData->SetConnString(pConfig->m_sSQLConnect);
	// Load data from database.
	bool bRet = m_pData->LoadData();
	
	// Temp
	/*
	int nArrIds[] = {39803, 53594, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 65278,65279,65280,65281,65282,65283,65284,65285,65286};
	int nBalanceNew = 0;
	for(int i=0; i<sizeof(nArrIds)/sizeof(int); i++){
		m_pData->RegisterUserInTournament(nArrIds[i], 2, "127.0.0.1", -1062731396, nBalanceNew);
		}
		*/
	// Temp

	// Create tournaments.
	m_pData->CreateTournamentManagers(m_pServer, this, &m_listTournaments);
	// Start tournament managers. {{
	int nLoop = 0;
	while( nLoop < m_listTournaments.GetCount() ){
		PokerTournamentManager* pMan = m_listTournaments.GetAt(nLoop);
		if( pMan ){
			if( pMan->Start() ){
				}
			}
		nLoop ++;
		}
	// }}
	
	// Temp
	/*
	EnumerableObject<TournamentPlayerInfo> listPlayers;
	EnumerableObject<TournamentPlayerInfo> listPlayersResult;
	listPlayers	.CreateVariableIndex(_T("UserId"),	Serializable::Compare_Int32);
	listPlayers	.CreateVariableIndex(_T("Chips"),	Serializable::Compare_Int32);

	TournamentPlayerInfo* pNew = new TournamentPlayerInfo();
	pNew->m_nChips = 100;
	pNew->m_nUserId = 1;
	listPlayers.Add(pNew);

	TournamentPlayerInfo* pNew1 = new TournamentPlayerInfo();
	pNew1->m_nChips = 101;
	pNew1->m_nUserId = 2;
	listPlayers.Add(pNew1);


	int nChipsOld = pNew1->m_nChips;
	int nChipsNew = 99;
	listPlayers.ModifyIndexVariable(_T("Chips"), pNew1, &nChipsOld, &nChipsNew);


	listPlayers.GetObjectsByVariableIndex(_T("Chips"), 0, 1, listPlayersResult, true);
	for(int i=0; i<listPlayersResult.GetCount(); i++){
		TournamentPlayerInfo* pTemp = listPlayersResult.GetAt(i);
		if( pTemp ){
			pTemp->m_sUserName.Empty();
			}
		}

	listPlayers.RemoveAt(1, true);
	listPlayers.RemoveAt(0, true);
	*/
	// Temp
	return bRet;
	}

bool
PokerTournaments::Stop(){
	m_lockTournamentsList.Lock(); // Lock
	// Send stop event to all tournament managers.
	int nLoop = 0, nCt = m_listTournaments.GetCount();
	while( nLoop < nCt ){
		PokerTournamentManager* pMan = m_listTournaments.GetAt(nLoop); 
		if( pMan )
			pMan->SendStopSignal();
		nLoop ++;
		}

	// Stop tournaments.
	m_listTournaments.RemoveAll(true);
	m_lockTournamentsList.Unlock(); // Unlock

	// Destroy database object.
	if( m_pData ){
		m_pData->FreeData(true);
		delete m_pData;
		m_pData = NULL;
		}

	m_pLobbyInfo = NULL;
	return true;
	}

PokerTournamentManager*
PokerTournaments::GetTournamentManager(int nTournamentId){
	m_lockTournamentsList.Lock(); // Lock
	PokerTournamentManager* pMan = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	m_lockTournamentsList.Unlock(); // Unlock
	return pMan;
	}

TournamentTableController*
PokerTournaments::GetTableController(int nTournamentId, int nTableId){
	PokerTournamentManager* pMan = GetTournamentManager(nTournamentId);
	if( !pMan ) return NULL;
	return pMan->GetTableController(nTableId);
	}

int
PokerTournaments::CreateTournamentGameTables(PokerTournamentManager* pMan, int nTournamentId, List<TournamentTableController>* pListTables, AutoSortedArray* pArrGameTableById){
	ASSERT(pMan);
	int				nRet					= m_pData->CreateTournamentGameTables(nTournamentId, m_pServer, this, pMan, pListTables, pArrGameTableById); 
	int				nLoop					= 0;
	int				nRegistrantPlayersCt	= pMan->GetPlayersCount();
	PokerTableInfo	tableInfo;

	// Update lobby info.
	while( nLoop < nRet ){
		TournamentTableController* pTableController = pListTables->GetAt(nLoop);
		if( pTableController ){ 
			pTableController->GetTableInfo(&tableInfo);
			m_pLobbyInfo->SetTournamentTable(nTournamentId, &tableInfo);
			tableInfo.ZeroInit();
			}
		nLoop ++;
		}

	// Update tournament players count.
	m_pLobbyInfo->SetTournamentPlayersCt(nTournamentId, nRegistrantPlayersCt, nRegistrantPlayersCt);
	return nRet;
	}

int
PokerTournaments::RegisterUserInTournament(PokerSession* pSess, int nUserId, int nTournamentId, bool bByPoints, bool bUnregister /*= false*/, bool bByTicket /*= false*/){
	PokerTournamentManager* pMan = GetTournamentManager(nTournamentId);
	if( !pMan ) 
		return PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;			// Tournament manager not found!
	if( !pMan->IsRegisterUnregisterAllowed() )
		return PokerErrorCodes::ES_ERROR_TOURNAMENT_REGISTRATION;	// Tournament register/unregister not allowed. 
	
	INT_CHIPS	nBalanceNew = 0;
	double		dPointsNew	= 0.0;
	_String sUserName;
	if( !m_pData->GetUserNameById(nUserId, sUserName) )
		return PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;			// User couldn't found with that id.

	PokerSession*	pSessRegUser = (pSess ? pSess : PokerSession::GetSessionByUserId(nUserId, true));
	if( !pSessRegUser )
		return PokerErrorCodes::ES_ERROR_UNKNOWN;

	ClientConnection*	pConn	= pSessRegUser->GetConnection(true, true);
	int					nRet	= PokerErrorCodes::ES_ERROR_UNKNOWN;

	if( !bUnregister ){

		bool bSuccess = (bByTicket ? m_pData->RegisterUserInTournamentByTicket(nUserId, nTournamentId, (pConn ? pConn->GetRemoteIpAddress() : _T("127.0.0.1")), 
																(pConn ? (int)pConn->GetRemoteIp() : -1062731396), nBalanceNew) :
								 m_pData->RegisterUserInTournament(nUserId, nTournamentId, (pConn ? pConn->GetRemoteIpAddress() : _T("127.0.0.1")), 
																(pConn ? (int)pConn->GetRemoteIp() : -1062731396), nBalanceNew, dPointsNew, bByPoints));
		if( bByTicket )
			bByPoints = false;

		if( bSuccess ){
			// Update lobby info, add tournament player. {{
			TournamentPlayerInfo player;
			player.m_nUserId			= nUserId;
			player.m_sUserName			= sUserName;
			player.m_nChips				= 0;
			player.m_cLevel				= 0;
			player.m_shPlace			= -1;
			m_pLobbyInfo->SetTournamentParticipant(nTournamentId, &player);
			// }}

			const TournamentInfo* pInfo = pMan->GetTournamentInfo();
			/*
			UserTournament userTnmnt;
			userTnmnt.m_nTournamentId	= nTournamentId;
			userTnmnt.m_sName			= pInfo->m_sName;
			userTnmnt.m_dtStart			= pInfo->m_dtStartTime;
			userTnmnt.m_cGameType		= (BYTE)pInfo->m_nGameTypeId;
			userTnmnt.m_shGameSubType	= pInfo->m_nSubTypeId;
			userTnmnt.m_nBuyinAmount	= pInfo->m_dBuyIn;
			userTnmnt.m_nFeeAmount		= pInfo->m_dBuyInFee;
			userTnmnt.m_cCurrencyId		= pInfo->m_nCurrencyId;
			userTnmnt.m_status			= pInfo->m_nStatus;
			*/
			// Rebuild tournament payouts.
			pMan->m_nRegisteredCt	++;
			pMan->OnUserRegister(nUserId, sUserName, pSessRegUser, false);
			m_pData->RebuildTournamentPayouts(nTournamentId, m_pLobbyInfo);
			nRet					= PokerErrorCodes::ES_ERROR_SUCCESS;

			if( pSessRegUser ){
				if( bByPoints )
					pSessRegUser->SetUserVPPoints(dPointsNew);
				else
					pSessRegUser->SetUserBalance(nBalanceNew);
					
				// Register session.
				pSessRegUser->RegisterInTournament(nTournamentId, (int)pInfo->m_shRebuyCt, (int)pInfo->m_shAddonCt);			
				// Tournament is already in running mode. (Late registration)
				if( pMan->isTournamentInProgress() ){
					pMan->LateRegisterPlayer(&player, nUserId, pSessRegUser);
					}
				}
			}
		}
	else{
		if( m_pData->UnregisterUserFromTournament(nUserId, nTournamentId, nBalanceNew, dPointsNew) ){
			// Update lobby info, remove tournament participant.
			m_pLobbyInfo->RemoveTournamentParticipant(nTournamentId, nUserId);
			// Rebuild tournament payouts.
			pMan->m_nRegisteredCt --;
			m_pData->RebuildTournamentPayouts(nTournamentId, m_pLobbyInfo);
			pMan->OnUserRegister(nUserId, sUserName, pSessRegUser, true);
			nRet = PokerErrorCodes::ES_ERROR_SUCCESS;			// Success
			if( pSessRegUser ){
				pSessRegUser->SetUserBalance	(nBalanceNew);
				pSessRegUser->SetUserVPPoints	(dPointsNew);
				pSessRegUser->UnregisterFromTournament(nTournamentId);	// Unregister session.
				}
			}
		}

	// Release pointer.
	if( pSessRegUser && !pSess ){
		if( pConn )
			SafePointer::Release(pConn);
		PokerSession::Release(pSessRegUser);
		pSessRegUser = NULL;
		}
	return nRet; // Success
	}

///////////////////////////////////////////////////////////
///
///	Packet handle events.
///

void
PokerTournaments::OnAction(PokerActionPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( pMan ){
		// Get seat index by connection.
		bool bOpened = false;
		int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, p->m_nTournamentId);
		if( nSeatIndex == -1 ) return; // Connection is not sitted.
		// Ensure seat index to be valid.
		p->m_cSeat	= (char)nSeatIndex;

		pMan->OnAction(pSess, pConn, p);
		}
	}

void
PokerTournaments::OnReturnBack(PokerReturnBackPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() ) return; // Need authorization.

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( pMan ){
		// Get seat index by connection.
		bool bOpened = false;
		int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, p->m_nTournamentId);
		if( nSeatIndex == -1 ) return; // User is not sitted.
		//Ensure seat index to be valid.
		p->m_cSeat	= (char)nSeatIndex;
		pMan->OnReturnBack(pSess, pConn, p);
		}
	}

void
PokerTournaments::OnSitOut(PokerSitOutPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	}

void
PokerTournaments::OnPostBlind(PokerPostBlindPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	}

void
PokerTournaments::OnShowMuckCards(PokerShowMuckCardsPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() )		return; // Need authorization.

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( pMan ){
		// Get seat index by connection.
		bool bOpened = false;
		int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, p->m_nTournamentId);
		if( nSeatIndex == -1 ) return; // User is not sitted.
		//Ensure seat index to be valid.
		p->m_cSeat	= (char)nSeatIndex;
		pMan->OnShowMuckCards(pSess, pConn, p);
		}
	}

void
PokerTournaments::OnSetOption(PokerSetOptionPacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )	return;
	if( !pConn->IsAuthorized() )		return; // Need authorization.

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( pMan ){
		// Get seat index by connection.
		bool bOpened	= false;
		int nSeatIndex	= pSess->GetSeatIndex(p->m_nTableId, bOpened, p->m_nTournamentId);
		if( nSeatIndex == -1 ) return; // User is not sitted.
		//Ensure seat index to be valid.
		p->m_cSeat		= (char)nSeatIndex;

		pMan->OnSetOption(pSess, pConn, p);
		}
	}

void
PokerTournaments::OnChatMessage(PokerChatMessagePacket* p, ClientConnection* pConn){
	ASSERT(m_pServer);
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() ) return;
	if( !pConn->IsAuthorized() || pSess->IsTableObserver(p->m_nTableId, p->m_nTournamentId) ) return; // Need authorization and must be seated already.

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( pMan ){
		bool bOpened			= false;
		p->m_message.m_cSeat	= (char)pSess->GetSeatIndex(p->m_nTableId, bOpened, p->m_nTournamentId);
		
		if( bOpened ){
			p->m_message.m_sUserName = pSess->GetUserName();
			pMan->OnChatMessage(pSess, pConn,  p);
			}
		}
	}

void
PokerTournaments::OnOpenRandomTable(PokerOpenRandomTablePacket* p, ClientConnection* pConn){
	}

void
PokerTournaments::OnCreateTournament(PokerCreateTournamentPacket* p, ClientConnection* pConn){
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() )  return;
//	if( !pConn->IsAuthorized() ) return; // Need authorization.
//	if( !pSess->IsPermissionSet((int)UserPermissions::Permission_ManageTournaments) )
//		return; // No permission to create tournament.

	// Check create tournament parameters. {{
	if( p->m_sName.IsEmpty() || 
		!IS_VALID_GAME_TYPE(p->m_nGameType)||
		!IS_VALID_GAME_SUB_TYPE(p->m_nGameSubType)||
		!IS_VALID_TOURNAMENT_TYPE(p->m_nGameSubType)||
		p->m_shMaxSeatCt >= 11 ||
		p->m_nMinPlayers >= p->m_nMaxPlayers && p->m_nMinPlayers < 2
		){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;
		m_pServer->SendPacket(pConn, p);
		return;
		}

	// Time check is not required for Sit'N Go tournament.
	if( (p->m_nGameSubType&GameSubType::Tournament_SitNGo) == 0 ){
		COleDateTime dtCurrent = COleDateTime::GetCurrentTime();
		if( !(p->m_dtTimeStart > dtCurrent && p->m_dtTimeStart >= p->m_dtTimeShow && p->m_dtTimeStart > p->m_dtTimeRegStart && p->m_dtTimeRegStart < p->m_dtTimeRegEnd) ){
			p->m_nErrorCode = PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;
			m_pServer->SendPacket(pConn, p);
			return;
			}
		}

	/*
	if( p->m_sName.IsEmpty() || 
		!IS_VALID_GAME_TYPE(p->m_nGameType)||
		!IS_VALID_GAME_SUB_TYPE(p->m_nGameSubType)||
		!IS_VALID_TOURNAMENT_TYPE(p->m_nGameSubType)||
		p->m_shMaxSeatCt >= 11 ||
		!(p->m_dtTimeStart > dtCurrent && p->m_dtTimeStart >= p->m_dtTimeShow && p->m_dtTimeStart > p->m_dtTimeRegStart && p->m_dtTimeRegStart < p->m_dtTimeRegEnd) ||
		p->m_nMinPlayers >= p->m_nMaxPlayers && p->m_nMinPlayers < 2
		){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;
		m_pServer->SendPacket(pConn, p);
		return;
		}*/

	int nTournamentId = m_pData->CreatTournament(p);
	if( nTournamentId ){
		PokerTournamentManager* pMan = m_pData->CreateTournamentManager(nTournamentId, m_pServer, this);
		if( !pMan ){
			// Log entry.
			//_String sLogEntry;
			//sLogEntry.Format(_T("CreateNewTournament: Disconnected Ip=%s Port=%d UserName=%s"), pSockInfo->m_sRemoteIp, pSockInfo->m_nRemotePort, sUserName);
			//Logger::CreateLogEntry(sLogEntry);
			return;
			}

		m_lockTournamentsList.Lock();	// Lock
		m_listTournaments.Add(pMan);
		m_lockTournamentsList.Unlock(); // Unlock

		// Start tournament manager.
		if( pMan->Start() ){
			
			// Update lobby info, add new tournament info. {{
			m_pLobbyInfo->SetTournamentInfo(&pMan->m_info);
			// }}

			//Temp
			//Register bots in tournament.
			INT_CHIPS nBalanceNew = 0;
			double dVPPoints = 0.0;
			for(int i=200; i<390; i++){
				int nUserId = 100000 + i;
				if( m_pData->RegisterUserInTournament(100000 + i, pMan->GetTournamentId(), "127.0.0.1", -1062731396, nBalanceNew, dVPPoints) ){
					TournamentPlayerInfo player;
					player.m_cLevel		= 0;
					player.m_nUserId	= nUserId;
					player.m_sUserName.Format(_T("bot%d"), i +1);
					player.m_nChips		= pMan->GetStartingChips();
					player.m_shPlace	= i;
					player.m_nOrder		= i;	// Need to order participants.

					m_pLobbyInfo->SetTournamentParticipant(pMan->GetTournamentId(), &player);
					pMan->m_nRegisteredCt ++;
					}
				}

			// Rebuild tournament payouts.
			m_pData->RebuildTournamentPayouts(p->m_nTournamentId, m_pLobbyInfo);
			
			//Temp
			}
		}

	p->m_nTournamentId	= nTournamentId;
	p->m_nErrorCode		= PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
	m_pServer->SendPacket(pConn, p);						// Send reply
	// }}
	}

void
PokerTournaments::OnCancelTournament(PokerCancelTournamentPacket* p, ClientConnection* pConn){
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() /*|| !pConn->IsAuthorized()*/ )  
		return;
	/*
	if( !pSess->IsPermissionSet((int)UserPermissions::Permission_ManageTournaments) ) 
		return; // No permission to cancel tournament.
		*/

	int												nTournamentId	= p->m_nTournamentId;
	PokerTournamentManager*							pMan			= GetTournamentManager(p->m_nTournamentId);
	EnumerableObject<CancelTournamentProc_Return>	refundPlayers;

	if( pMan && pMan->CancelTournament(false) && m_pData->CancelTournament(p->m_nTournamentId, pSess->GetUserId(), refundPlayers) ){
		// Refund tournament amounts.
		for(int i=0; i<refundPlayers.GetCount(); i++){
			CancelTournamentProc_Return* pRefund = refundPlayers.GetAt(i);
			if( pRefund && pRefund->m_dRefundAmount > 0.0 ){
				if( m_pData->CancelTournamentRefund(nTournamentId, pRefund->m_nUserId, pRefund->m_dRefundAmount) ){
					PokerSession* pSessUser = PokerSession::GetSessionByUserId(pRefund->m_nUserId, true);
					if( pSessUser ){
						pSessUser->AddUserBalance(Serializable::ConvertMoney(pRefund->m_dRefundAmount));
						SafePointer::Release(pSessUser);
						}
					}
				}
			}

		// Update lobby info, remove tournament info.
		// It is not neccessary to remove from lobby info.
		//m_pLobbyInfo->RemoveTournamentInfo(p->m_nTournamentId);

		// Update tournament status.
		m_pLobbyInfo->SetTournamentStatus(p->m_nTournamentId, TournamentStatus::Tournament_Canceled);

		// Destroy tournament manager object.
		m_lockTournamentsList.Lock();	// Lock
		m_listTournaments.Remove(pMan, true);
		m_lockTournamentsList.Unlock();	// Unlock

		}
	}

void
PokerTournaments::OnTournamentCustomInfo(PokerTournamentCustomInfoPacket* p, ClientConnection* pConn){
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() /*|| !pConn->IsAuthorized()*/ )  return;

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( !pMan ) return;

	LobbyTournamentSummaryInfo result;
	LobbyTournamentSummaryInfo* pResult = &result;
	result.ZeroInit();
	p->m_listResult.RemoveAll	(true);
	p->m_listResult.Add			(&result);

	// Tournamet basic info.
	if( p->m_nFlags&(PokerTournamentCustomInfoPacket::TournamentInfoCustom_BasicInfo) ){
		pResult->m_nRegisteredCt			= pMan->m_nRegisteredCt;		// Registered players count.
		pResult->m_nPlayersCt				= pMan->m_nPlayersCt;			// Players count.
		pResult->m_shTablesCt				= (short)pMan->m_nTableCt;		// Tables count.
		pResult->m_shPlacesPaid				= 0;
		pResult->m_cTournamentStatus		= (char)pMan->m_tournamentStatus;
		pResult->m_bHandForHand				= pMan->m_bHandForHand;
		pResult->m_bIsBreak					= pMan->m_bIsBreak;
		pResult->m_shSecLeftTillNextLevel	= (short)(pResult->m_bIsBreak ? pMan->m_nBreakSecondsLeft : pMan->m_nLevelSecondsLeft);
		pResult->m_shSecLeftTillNextBreak	= pMan->m_nNextBreakSecondsLeft;

		if( pMan->IsRegisterUnregisterAllowed(true) ){
			if( pConn->IsAuthorized() )
				pResult->m_bRegisteredStatus	= pSess->IsRegisteredInTournament(p->m_nTournamentId);
			else
				pResult->m_bRegisteredStatus	= false;
			pResult->m_bRegistrationAllowed		= true;
			}
		else{
			pResult->m_bRegisteredStatus		= false;
			pResult->m_bRegistrationAllowed		= false;
			}

		//pResult->m_bRegistrationAllowed		= pMan->IsRegisterUnregisterAllowed(true);
		pResult->m_cLevelIndex					= ((char)pMan->m_nLevelIndex);			// Current level index.
		pResult->m_nLargestStake				= (UINT)(pMan->m_nStakeHighest/100);	// Highest stake.
		pResult->m_nAverageStake				= (UINT)(pMan->m_nStakeAverage/100);	// Average stake.
		pResult->m_nSmallestStake				= (UINT)(pMan->m_nStakeLowest/100);		// Lowest stake.
		}

	// Fill custom info packet.
	m_pLobbyInfo->FillCustomInfoPacket(p);

	p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
	m_pServer->SendPacket(pConn, p);						// Send reply
	p->m_listResult.RemoveAll(false);
	}

void
PokerTournaments::OnRegister(PokerRegisterInTournamentPacket* p, ClientConnection* pConn){
	PokerSession* pSess = (PokerSession*)pConn->GetSession(false);
	if( !pConn->IsAuthorized() ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_AUTHENTICATION_REQUIRED;
		m_pServer->SendPacket(pConn, p);
		return;
		}

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( !pMan ) 
		return; // Tournament manager not found!
	if( pSess->IsRegisteredInTournament(p->m_nTournamentId) )
		return; // User already registered.

	_String sUserName	= pSess->GetUserName();
	int		nBalanceNew = 0, nUserId = pSess->GetUserId();
	double	dVPPoints	= 0.0;
	bool	bByPoints	= (p->m_cRegisterBy == (char)PokerRegisterInTournamentPacket::RegisterBy_Points);

	p->m_nBalance		= 0;
	p->m_dPoints		= 0.0;

	if( !pMan->IsRegisterUnregisterAllowed(true) ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_TOURNAMENT_REGISTRATION;
		p->m_listPlayerInfo.RemoveAll(true);
		m_pServer->SendPacket(pConn, p);	// Send reply
		return;								// Tournament manager refuses user registration in tournament.
		}

	int nRetCode		= RegisterUserInTournament(pSess, nUserId, p->m_nTournamentId, bByPoints, false);
	if( nRetCode == ES_ERROR_SUCCESS ){
		// Update lobby info, add tournament player. {{
		TournamentPlayerInfo player;
		player.m_nUserId			= nUserId;
		player.m_sUserName			= sUserName;
		player.m_nChips				= 0;
		player.m_cLevel				= 0;
		player.m_shPlace			= -1;
		// }}

		const TournamentInfo* pInfo = pMan->GetTournamentInfo();
		UserTournament userTnmnt;
		userTnmnt.m_nTournamentId	= p->m_nTournamentId;
		userTnmnt.m_sName			= pInfo->m_sName;
		userTnmnt.m_dtStart			= pInfo->m_dtStartTime;
		userTnmnt.m_cGameType		= (BYTE)pInfo->m_nGameTypeId;
		userTnmnt.m_shGameSubType	= pInfo->m_nSubTypeId;
		userTnmnt.m_nBuyinAmount	= (UINT)Serializable::ConvertMoney(pInfo->m_dBuyIn);
		userTnmnt.m_nFeeAmount		= (UINT)Serializable::ConvertMoney(pInfo->m_dBuyInFee);
		userTnmnt.m_cCurrencyId		= pInfo->m_nCurrencyId;
		userTnmnt.m_status			= pInfo->m_nStatus;

		p->m_nBalance				= pSess->GetBalance();
		p->m_dPoints				= pSess->GetVPPoints();
		p->m_nErrorCode				= PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
		p->m_listPlayerInfo		.Add(&player);
		p->m_listRegTournament	.Add(&userTnmnt);
		m_pServer->SendPacket(pConn, p);						// Send reply
		p->m_listPlayerInfo		.RemoveAll(false);
		p->m_listRegTournament	.RemoveAll(false);
		}
	/*
	if( m_pData->RegisterUserInTournament(nUserId, p->m_nTournamentId, pConn->GetRemoteIpAddress(), pConn->GetRemoteIp(), nBalanceNew, dVPPoints, bByPoints) ){
		TournamentPlayerInfo player;
		player.m_cLevel		= 0;
		player.m_nUserId	= nUserId;
		player.m_sUserName	= sUserName;
		player.m_nChips		= 0;//pMan->GetStartingChips();
		player.m_shPlace	= 0;
		player.m_nOrder		= -1;

		m_pLobbyInfo->SetTournamentParticipant(pMan->GetTournamentId(), &player);
		pMan->m_nRegisteredCt ++;
		}
		*/

	/*
	if( p->m_cRegisterBy == (char)PokerRegisterInTournamentPacket::RegisterBy_BalanceAmount ){
		if( !pMan->UserRegistrationAction(nUserId, sUserName, pSess) ){
			p->m_nErrorCode = PokerErrorCodes::ES_ERROR_TOURNAMENT_REGISTRATION;
			p->m_listPlayerInfo.RemoveAll(true);
			m_pServer->SendPacket(pConn, p);	// Send reply
			return;								// Tournament manager refuses user registration in tournament.
			}

		if( m_pData->RegisterUserInTournament(nUserId, p->m_nTournamentId, pConn->GetRemoteIpAddress(), (int)pConn->GetRemoteIp(), nBalanceNew) ){
			// Update lobby info, add tournament player. {{
			TournamentPlayerInfo player;
			player.m_nUserId	= nUserId;
			player.m_sUserName	= sUserName;
			player.m_nChips		= 0;
			player.m_cLevel		= 0;
			player.m_shPlace	= -1;
			m_pLobbyInfo->SetTournamentParticipant(p->m_nTournamentId, &player);
			// }}

			const TournamentInfo* pInfo = pMan->GetTournamentInfo();
			UserTournament userTnmnt;
			userTnmnt.m_nTournamentId	= p->m_nTournamentId;
			userTnmnt.m_sName			= pInfo->m_sName;
			userTnmnt.m_dtStart			= pInfo->m_dtStartTime;
			userTnmnt.m_cGameType		= (BYTE)pInfo->m_nGameTypeId;
			userTnmnt.m_shGameSubType	= pInfo->m_nSubTypeId;
			userTnmnt.m_nBuyinAmount	= pInfo->m_dBuyIn;
			userTnmnt.m_nFeeAmount		= pInfo->m_dBuyInFee;
			userTnmnt.m_cCurrencyId		= pInfo->m_nCurrencyId;
			userTnmnt.m_status			= pInfo->m_nStatus;

			// Rebuild tournament payouts.
			m_pData->RebuildTournamentPayouts(p->m_nTournamentId, m_pLobbyInfo);
			pMan->m_nRegisteredCt ++;

			p->m_nBalance				= nBalanceNew;
			p->m_nErrorCode				= PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
			p->m_listPlayerInfo		.Add(&player);
			p->m_listRegTournament	.Add(&userTnmnt);
			m_pServer->SendPacket(pConn, p);						// Send reply
			p->m_listPlayerInfo		.RemoveAll(false);
			p->m_listRegTournament	.RemoveAll(false);

			pSess->RegisterInTournament(p->m_nTournamentId, (int)pInfo->m_shRebuyCt, (int)pInfo->m_shAddonCt);		// Register session.

			// Tournament is already in running mode. (Late registration)
			if( pMan->isTournamentInProgress() ){
				pMan->LateRegisterPlayer(&player, nUserId, pSess);
				}
			}
		}
	else
	if( p->m_cRegisterBy == (char)PokerRegisterInTournamentPacket::RegisterBy_Points ){
		}
	else
	if( p->m_cRegisterBy == (char)PokerRegisterInTournamentPacket::RegisterBy_Ticket ){
		}
	*/
	}

void
PokerTournaments::OnUnregister(PokerUnregisterFromTournamentPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pConn->IsAuthorized() ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_AUTHENTICATION_REQUIRED;
		m_pServer->SendPacket(pConn, p);
		return;
		}

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( !pMan ) 
		return; // Tournament manager not found!
	if( !pMan->IsRegisterUnregisterAllowed(false) )
		return; // Tournament unregister not allowed.
	if( !pSess->IsRegisteredInTournament(p->m_nTournamentId) )
		return; // User not registered.
	 
	int nBalanceNew = 0, nUserId = pSess->GetUserId();
	int nRetCode	= RegisterUserInTournament(pSess, nUserId, p->m_nTournamentId, false, true);
	if( nRetCode == ES_ERROR_SUCCESS ){
		p->m_nBalance	= pSess->GetBalance();
		p->m_dPoints	= pSess->GetVPPoints();
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
		m_pServer->SendPacket(pConn, p);						// Send reply
		}

	/*
	if( m_pData->UnregisterUserFromTournament(nUserId, p->m_nTournamentId, nBalanceNew) ){
		// Update lobby info, remove tournament participant.
		m_pLobbyInfo->RemoveTournamentParticipant(p->m_nTournamentId, nUserId);

		// Rebuild tournament payouts.
		m_pData->RebuildTournamentPayouts(p->m_nTournamentId, m_pLobbyInfo);
		
		pMan->m_nRegisteredCt --;

		p->m_nBalance	= nBalanceNew;
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
		m_pServer->SendPacket(pConn, p);					// Send reply
		pSess->UnregisterFromTournament(p->m_nTournamentId);	// Unregister session.
		}*/
	}

void
PokerTournaments::OnOpenTable_Tournament(PokerOpenTable_TournamentPacket* p, ClientConnection* pConn){
	PokerSession*			pSess	= (PokerSession*)pConn->GetSession(false);
	PokerTournamentManager*	pMan	= GetTournamentManager(p->m_nTournamentId);
	if( pMan )
		pMan->OnOpenTable(pSess, pConn, p);
	}

void
PokerTournaments::OnLeaveTable_Tournament(PokerLeaveTable_TournamentPacket* p, ClientConnection* pConn){
	PokerSession*			pSess	= (PokerSession*)pConn->GetSession(false);
	PokerTournamentManager*	pMan	= GetTournamentManager(p->m_nTournamentId);
	if( pMan )
		pMan->OnLeaveTable(pSess, pConn, p);
	}

void
PokerTournaments::OnRebuy(PokerRebuyPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pConn->IsAuthorized() ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_AUTHENTICATION_REQUIRED;
		m_pServer->SendPacket(pConn, p);
		return;
		}

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( !pMan ) return; // Tournament not exists !!!
	pMan->OnRebuy(pSess, pConn, p);
	}

void
PokerTournaments::OnAddon(PokerAddonPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pConn->IsAuthorized() ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_AUTHENTICATION_REQUIRED;
		m_pServer->SendPacket(pConn, p);
		return;
		}

	PokerTournamentManager* pMan = GetTournamentManager(p->m_nTournamentId);
	if( !pMan ) return; // Tournament not exists !!!
	pMan->OnAddon(pSess, pConn, p);
	}

void
PokerTournaments::OnGetTournaments(PokerGetTournamentsPacket* p, ClientConnection* pConn){
	p->m_listResult.RemoveAll(true);
	m_pData->GetTournamentInfoObjects(p->m_nTournamentType, &p->m_listResult);
	p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;
	m_pServer->SendPacket(pConn, p);
	}

void
PokerTournaments::OnRegisterUserInTournament(PokerRegisterUserInTournamentPacket* p, ClientConnection* pConn){
	PokerSession*			pSess		= (PokerSession*)pConn->GetSession(false);
	PokerTournamentManager* pMan		= GetTournamentManager(p->m_nTournamentId);
	if( !pMan ) return;							// Tournament manager not found!

	if( !pMan->IsRegisterUnregisterAllowed() ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_TOURNAMENT_REGISTRATION;
		m_pServer->SendPacket(pConn, p);		// Send reply
		return;									// Tournament register/unregister not allowed. 
		}
	
	int		nBalanceNew = 0, nUserId	= p->m_nUserId;
	double	dPointsNew	= 0.0;
	_String sUserName;
	if( !m_pData->GetUserNameById(nUserId, sUserName) ){
		p->m_nErrorCode = PokerErrorCodes::ES_ERROR_INVALID_PARAMETER;
		m_pServer->SendPacket(pConn, p);		// Send reply
		return;									// User couldn't found with that id.
		}

	int nRetCode	= RegisterUserInTournament(NULL, p->m_nUserId, p->m_nTournamentId, false, !p->m_bRegister);
	p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;			// Success
	m_pServer->SendPacket(pConn, p);								// Send reply
	/*
	PokerSession*			pSessRegUser = PokerSession::GetSessionByUserId(p->m_nUserId, true);
	// Register in tournament
	if( p->m_bRegister ){
		if( m_pData->RegisterUserInTournament(nUserId, p->m_nTournamentId, pConn->GetRemoteIpAddress(), (int)pConn->GetRemoteIp(), nBalanceNew, ) ){
			// Update lobby info, add tournament player. {{
			TournamentPlayerInfo player;
			player.m_nUserId	= nUserId;
			player.m_sUserName	= sUserName;
			player.m_nChips		= 0;
			player.m_cLevel		= 0;
			player.m_shPlace	= -1;
			m_pLobbyInfo->SetTournamentParticipant(p->m_nTournamentId, &player);
			// }}

			const TournamentInfo* pInfo = pMan->GetTournamentInfo();
			UserTournament userTnmnt;
			userTnmnt.m_nTournamentId	= p->m_nTournamentId;
			userTnmnt.m_sName			= pInfo->m_sName;
			userTnmnt.m_dtStart			= pInfo->m_dtStartTime;
			userTnmnt.m_cGameType		= (BYTE)pInfo->m_nGameTypeId;
			userTnmnt.m_shGameSubType	= pInfo->m_nSubTypeId;
			userTnmnt.m_nBuyinAmount	= pInfo->m_dBuyIn;
			userTnmnt.m_nFeeAmount		= pInfo->m_dBuyInFee;
			userTnmnt.m_cCurrencyId		= pInfo->m_nCurrencyId;
			userTnmnt.m_status			= pInfo->m_nStatus;

			// Rebuild tournament payouts.
			m_pData->RebuildTournamentPayouts(p->m_nTournamentId, m_pLobbyInfo);
			pMan->m_nRegisteredCt ++;

			p->m_nErrorCode				= PokerErrorCodes::ES_ERROR_SUCCESS;	// Success
			m_pServer->SendPacket(pConn, p);									// Send reply
			if( pSessRegUser )
				pSessRegUser->RegisterInTournament(p->m_nTournamentId, (int)pInfo->m_shRebuyCt, (int)pInfo->m_shAddonCt);			// Register session.
			}
		}
	// Unregister from tournament
	else{
		if( m_pData->UnregisterUserFromTournament(nUserId, p->m_nTournamentId, nBalanceNew) ){
			// Update lobby info, remove tournament participant.
			m_pLobbyInfo->RemoveTournamentParticipant(p->m_nTournamentId, nUserId);
			// Rebuild tournament payouts.
			m_pData->RebuildTournamentPayouts(p->m_nTournamentId, m_pLobbyInfo);
			pMan->m_nRegisteredCt --;
			p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;			// Success
			m_pServer->SendPacket(pConn, p);								// Send reply
			if( pSessRegUser )
				pSessRegUser->UnregisterFromTournament(p->m_nTournamentId);	// Unregister session.
			}
		}

	// Release pointer.
	if( pSessRegUser ){
		PokerSession::Release(pSessRegUser);
		pSessRegUser = NULL;
		}*/
	}

void
PokerTournaments::OnHideShowTournament(PokerHideShowTournamentPacket* p, ClientConnection* pConn){
	if( p->m_bHide ){
		int							nTournamentId	= p->m_nTournamentId;
		PokerTournamentManager*		pMan			= GetTournamentManager(p->m_nTournamentId);
	
		if( pMan ){
			TournamentStatus status = pMan->GetTournamentStatus();
			if( status == TournamentStatus::Tournament_Canceled ||
				status == TournamentStatus::Tournament_Completed ){

				m_pData->HideTournament(nTournamentId);

				m_pLobbyInfo->RemoveTournamentInfo(nTournamentId);
				// Destroy tournament manager object.
				m_lockTournamentsList.Lock();	// Lock
				m_listTournaments.Remove(pMan, true);
				m_lockTournamentsList.Unlock();	// Unlock
				}
			}
		else{
			m_pLobbyInfo->RemoveTournamentInfo	(nTournamentId);
			m_pData->HideTournament				(nTournamentId);
			}
		}
	else{
		// Reload tournament info from database.
		}
	}

// ======================================================================
//
//	Virtual methods.
//
// ======================================================================

bool
PokerTournaments::PlayerSeated(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, INT_CHIPS nSeatAmount, PokerTableSeatInfo* pSeat){
	return false;
	}

void
PokerTournaments::PlayerLeftSeat(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, PokerTableSeatInfo* pSeat){
	if( !pSeat ){
		ASSERT(FALSE);
		return;
		}

	TRACE(_T("%s has left the seat\r\n"), pSeat->GetUserName());
	PokerTournamentManager* pMan		= ((TournamentTableController*)pController)->GetTournamentManager();
	bool					bCanceled	= ((TournamentTableController*)pController)->IsTournamentCanceled();
	int						nUserId		= pSeat->GetUserId();
	int						nTournamentId = pInfo->m_nTournamentId;
	int						nTournamentTicketId = 0;
	bool					bFinish		= false;
	int						nPlace		= 0;
	double					dAmountWon	= 0.0;
	INT_CHIPS				nAmountWon	= 0;
	
	if( !bCanceled ){
		bool	bHandForHand = false;
		// Determine user place and win amount. {{
		if( m_pData ){
			if( m_pData->PlayerFinishedTournament(nTournamentId, nUserId, nPlace, dAmountWon, bHandForHand, bFinish, nTournamentTicketId) ){
				nAmountWon = Serializable::ConvertMoney(dAmountWon);
				m_pLobbyInfo->PlayerFinishedTournament(nTournamentId, nUserId, nPlace, nAmountWon);

				// Register in main tournament.
				if( nTournamentTicketId > 0 ){
					RegisterUserInTournament(NULL, nUserId, nTournamentTicketId, false, false, true); 
					}
				}
			}

		//_String sLog;
		//sLog.Format(_T("%s left seat place %d amount %d"), pSeat->GetUserName().GetBuffer(), nPlace, nAmountWon);
		//Logger::CreateLogEntry(sLog.GetBuffer());

		// Temp
//		if( nPlace > 1 ){
//			ASSERT( pSeat->GetBalance() == 0 );
//			}
		// Temp

		// Update tournament hand for hand status.
		pMan->SetHandForHand(bHandForHand);
		if( bFinish )
			pMan->SetFinishTournament(bFinish);
		}

	// Update user session. {{
	PokerSession* pSess = (PokerSession*)pSeat->GetSession(true);
	if( pSess ){
		if( dAmountWon > 0.0 ){
			pSess->SetUserBalance(pSess->GetBalance() + nAmountWon);
			}

		// Send tournament finished packet. {{
		ClientConnection* pConn = pSess->GetConnection(true, true);
		if( pConn ){
			PokerTournamentFinishedPacket packet;
			packet.m_nTournamentId	= pInfo->m_nTournamentId;
			packet.m_nTableId		= pInfo->m_nTableId;
			packet.m_nRank			= nPlace;
			packet.m_nAmountWon		= nAmountWon;
			packet.m_bCanceled		= bCanceled;
			m_pServer->SendPacket(pConn, &packet);

			// Remove table connection.
			m_pServer->RemoveTableConnection(packet.m_nTableId, packet.m_nTournamentId, pConn, pController);

			// Release pointer.
			SafePointer::Release(pConn);
			}
		// }}

		pSess->TableCloseAndUnseat		(pInfo->m_nTableId, pInfo->m_nTournamentId);
		pSess->UnregisterFromTournament	(pInfo->m_nTournamentId);
		SafePointer::Release(pSess);
		}
	// }}
	}

void
PokerTournaments::PlayerSeatBalanceChanged(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nCurrencyId, int nSeat, INT_CHIPS nSeatBalanceNew, INT_CHIPS nSeatBalanceOld){
	}

bool
PokerTournaments::HandStarted(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64& nHandId, int nTableId, int nPlayersCt, UINT nSB, UINT nBB, UINT nAnte){
	if( !m_pData ) return false;
	// Update lobby info.
	m_pLobbyInfo->SetTournamentTable(pInfo->m_nTournamentId, pInfo);

	// Update user place indexes.
	for(int i=0; i<pInfo->m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = pInfo->m_seats[i];
		if( pSeat )
			pSeat->SetUserPlaceIndex(m_pLobbyInfo->GetPlayersPlaceInTournament(pInfo->m_nTournamentId, pSeat->GetUserId()));
		}

	nHandId = m_pData->StartHand(pInfo->m_nTournamentId, nTableId, -1, nPlayersCt, nSB, nBB, nAnte);
	return (nHandId > 0);
	}

bool
PokerTournaments::HandFinished(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64 nHandId,  INT_CHIPS nTotalPotAmount, UINT nRakeAmount, PokerHandHistory* pHandHistory, _String& sPlayersInfo, _String& sTableCards){
	if( !m_pData || !pHandHistory ) return false;
	// Update lobby info.
	m_pLobbyInfo->SetTournamentTable(pInfo->m_nTournamentId, pInfo);

	bool bRet = false;
	// Save hand history as binary.
	GrowableMemory mem(0x400, 0x400, true);
	if( pHandHistory->Serialize(&mem) ){
		bRet = m_pData->FinishHand(nHandId, pInfo->m_nTableId, nTotalPotAmount, sTableCards, sPlayersInfo, mem.GetBufferPtr(), mem.GetUsedBufferSize());
		}
	return bRet;
	}

bool
PokerTournaments::UserAddRebuyChips(PokerSession* pSess, PokerTableController* pController, void* pParam, int nTableId, PokerTableSeatInfo* pSeat, INT_CHIPS nAmount, bool bUseVPPoints){
	if( !pSess || !pSeat || !m_pData ) 
		return false;

	PokerTournamentManager* pMan			= ((TournamentTableController*)pController)->GetTournamentManager();
	int						nUserId			= pSess->GetUserId();
	int						nTournamentId	= ((TournamentTableController*)pController)->GetTournamentId();
	double					dBalanceNew		= 0.0;
	double					dVPPointsNew	= 0.0;
	int						nRebuyCtLeft	= 0;
	int						nRebuyCtAtOnce	= (int)(nAmount / (pMan->GetRebuyChipsCt()*100));

	// Should be less or equal than starting chips.
	if( pSeat->GetBalance() > pMan->GetStartingChips()*100 )
		return false;

	// Validate rebuy action for current user.
	if( !pMan->IsRebuyAllowed(pSess, nRebuyCtLeft, nRebuyCtAtOnce) )
		return false; // Rebuy is not allowed or rebuy ct limit expired for current user.

	if( !m_pData->Rebuy(nUserId, nTournamentId, nRebuyCtAtOnce, dBalanceNew, dVPPointsNew, false) )
		return false; // Database failure or balance problem.

	// Update balance.
	if( !bUseVPPoints )
		pSess->SetUserBalance(Serializable::ConvertMoney(dBalanceNew));
	else
		pSess->SetUserVPPoints(dVPPointsNew);

	// Update rebuy count.
	nRebuyCtLeft -= nRebuyCtAtOnce;
	pSess->SetTournamentUserDetail(nTournamentId, -1, nRebuyCtLeft, -1, -1);

	// Update lobby info.
	TournamentInfo*		pInfo			= (TournamentInfo*)m_pData->TournamentInfoById(nTournamentId);
	m_pLobbyInfo->OnTournamentRebuy(pInfo, nUserId);
	return true;
	}

bool
PokerTournaments::UserAddAddonChips(PokerSession* pSess, PokerTableController* pController, void* pParam, int nTableId, PokerTableSeatInfo* pSeat, INT_CHIPS nAmount, bool bUseVPPoints){
	if( !pSess || !pSeat || !m_pData ) 
		return false;
	PokerTournamentManager* pMan			= ((TournamentTableController*)pController)->GetTournamentManager();
	int						nUserId			= pSess->GetUserId();
	int						nTournamentId	= ((TournamentTableController*)pController)->GetTournamentId();
	double					dBalanceNew		= 0.0;
	double					dVPPointsNew	= 0.0;
	int						nAddonCtLeft	= 0;

	// Validate addon action for current user.
	if( !pMan->IsAddonAllowed(pSess, nAddonCtLeft) )
		return false; // Rebuy is not allowed or rebuy ct limit expired for current user.

	if( !m_pData->Addon(nUserId, nTournamentId, dBalanceNew, dVPPointsNew, false) )
		return false; // Database failure or balance problem.

	// Update balance.
	if( !bUseVPPoints )
		pSess->SetUserBalance(Serializable::ConvertMoney(dBalanceNew));
	else
		pSess->SetUserVPPoints(dVPPointsNew);
	// Update addon count.
	nAddonCtLeft --;
	pSess->SetTournamentUserDetail(nTournamentId, -1, -1, nAddonCtLeft, -1);
	// Update lobby info.
	TournamentInfo*		pInfo	= (TournamentInfo*)m_pData->TournamentInfoById(nTournamentId);
	m_pLobbyInfo->OnTournamentAddon(pInfo, nUserId);
	return true;
	}

void
PokerTournaments::TournamentStarted(int nTournamentId, void* pParam){
	}

void
PokerTournaments::TournamentFinished(int nTournamentId, void* pParam){
	ASSERT(m_pData);
	m_pData->FinishTournament(nTournamentId);
	}

void
PokerTournaments::TournamentVisibilityChanged(int nTournamentId, void* pParam, bool bVisible){
	if( bVisible ){
		TournamentInfo* pInfo = (TournamentInfo*)m_pData->TournamentInfoById(nTournamentId);
		m_pLobbyInfo->SetTournamentInfo(pInfo);
		}
	else
		m_pLobbyInfo->RemoveTournamentInfo(nTournamentId);
	}

void
PokerTournaments::TournamentStatusChanged(int nTournamentId, void* pParam, TournamentStatus statusNew){
	ASSERT(m_pData);
	// Update database.
	m_pData->SetTournamentStatus(nTournamentId, statusNew);
	// Update lobby info.
	m_pLobbyInfo->SetTournamentStatus(nTournamentId, statusNew);
	}

void
PokerTournaments::TournamentRegistrationStatusChanged(int nTournamentId, void* pParam, bool bRegIsOpen){
	// Update lobby info.
	m_pLobbyInfo->SetTournamentRegistrationStatus(nTournamentId, bRegIsOpen);
	}

void
PokerTournaments::TournamentLevelChanged(int nLevelIndex, void* pParam){
	}

void
PokerTournaments::TournamentTableCreated(int nTournamentId, int nTableId, void* pParam, TournamentTableController* pController){
	ASSERT(pController && pController->GetTableId() == nTableId);
	// Update lobby info.
	PokerTableInfo	tableInfo;
	pController->GetTableInfo(&tableInfo);
	m_pLobbyInfo->SetTournamentTable(nTournamentId, &tableInfo);
	}

void
PokerTournaments::GameTableClosed(int nTournamentId, int nTableId, void* pParam, TournamentTableController* pController){
	// Remove table connections.
	m_pServer->RemoveConnectionsByTable(nTableId, nTournamentId, pController);
	// Update lobby info.
	m_pLobbyInfo->RemoveTournamentTable(nTournamentId, nTableId);
	}