#include "stdafx.h"
#include "../../Protocol/BasePacket.h"
#include "PokerTournamentData.h"
#include "../PokerServer.h"
#include "../PokerTournaments.h"
#include "../TournamentTableController.h"
#include "../../Protocol/PokerAdministrationPacket.h"
#include "../Object/LobbyInfo.h"
#include "../../Utility/GrowableMemory.h"
#include "../../Path/Path.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerTournamentData::PokerTournamentData() : PokerData() {
	// Create variable indexes.
	m_listPayoutProfiles		.CreateVariableIndex(_T("Id"), Serializable::Compare_Int32);
	m_listBlindStructProfiles	.CreateVariableIndex(_T("Id"), Serializable::Compare_Int32);
	}

BEGIN_DECLARE_VAR(PokerTournamentData, Serializable)
	DECLARE_VAR(_T("PokerTournamentData"),	_T(""), VariableType::VT_None, 0, false, true, false)
END_DECLARE_VAR()

PokerTournamentData::~PokerTournamentData(){
	FreeData(true); // Free data close database.
	}

bool
PokerTournamentData::LoadData(){
	if( !PokerData::LoadData() )
		return false;

	m_lockTournamentObjects.Lock(); // Lock
	// Create variable index.
	m_listTournaments.CreateVariableIndex(_T("Id"), (CompareProc)Serializable::Compare_Int32);
	// Load tables except for completed tournaments.
	// STATUS_ID value 7 means cancel status.
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listTournaments, _T("where [STATUS_ID] <> 7 and HIDDEN_STATUS = 0"), _T(""));	// Tournaments

	/*
	DateTime dt(true);
	COleDateTime dtOle = COleDateTime::GetCurrentTime();

	dt.SetDateTime(dtOle.GetYear(), dtOle.GetMonth(), dtOle.GetDay(), 
					dtOle.GetHour(), dtOle.GetMinute(), dtOle.GetSecond());

	for(int i=0; i<m_listTournaments.GetCount(); i++){
		TournamentInfo* pInfo = m_listTournaments[i];
		if( pInfo && pInfo->m_dtStartTime > dt ){
			}
		}*/

	_String sWhereSQL;
	// Load payout profiles.
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listPayoutProfiles, _T(""), _T(""));
	// Load payout entries.
	for(int i=0; i<m_listPayoutProfiles.GetCount(); i++){
		TournamentPayoutProfile* p = m_listPayoutProfiles.GetAt(i);
		if( p ){
			sWhereSQL.Format(_T("where [PAYOUT_PROFILE_ID]=%d"), p->m_nId);
			m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&p->m_listPayoutEntries, sWhereSQL, _T(""));
			}
		}

	// Load blind structure profiles.
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listBlindStructProfiles, _T(""), _T(""));
	// Load blind structure entries.
	for(int i=0; i<m_listPayoutProfiles.GetCount(); i++){
		TournamentBlindStructureProfile* p = m_listBlindStructProfiles.GetAt(i);
		if( p ){
			sWhereSQL.Format(_T("where [BLIND_STRUCT_PROFILE_ID]=%d"), p->m_nId);
			m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&p->m_listBlindStructureEntries, sWhereSQL, _T(""));
			}
		}

	// Set blind structrure and payout info of tournament. {{
	for(int i=0; i<m_listTournaments.GetCount(); i++){
		TournamentInfo* pInfo	= m_listTournaments.GetAt(i);
		if( !pInfo ) continue;

		TournamentBlindStructureProfile*	pBlindStructProf	= m_listBlindStructProfiles.FindOneVariable(_T("Id"), &pInfo->m_nBlindStructProfileId);
		TournamentPayoutProfile*			pPayoutProf			= m_listPayoutProfiles.FindOneVariable(_T("Id"), &pInfo->m_nPayoutProfileId);

		if( pBlindStructProf ){
			pInfo->m_listBlindStructure.RemoveAll(true);
			pBlindStructProf->m_listBlindStructureEntries.Copy_Into(&pInfo->m_listBlindStructure);
			}

		if( pPayoutProf ){
			pInfo->m_listPayoutEntries.RemoveAll(true);
			pPayoutProf->m_listPayoutEntries.Copy_Into(&pInfo->m_listPayoutEntries);
			}
		}
	// }}
	m_lockTournamentObjects.Unlock();
	return true;
	}

void
PokerTournamentData::FreeData(bool bCloseDb /*= false*/){
	PokerData::FreeData(bCloseDb);
	m_lockTournamentObjects.Lock(); // Lock
	m_listTournaments.RemoveAll(true);
	m_lockTournamentObjects.Unlock(); // Unlock
	}

int
PokerTournamentData::CreatTournament(PokerCreateTournamentPacket* p){
	ASSERT(p);
	if( !p ) return 0;
	TournamentInfo* pTournamentNew				= new TournamentInfo();
	pTournamentNew->ZeroInit();

	pTournamentNew->m_nGameTypeId				= p->m_nGameType;
	pTournamentNew->m_nSubTypeId				= p->m_nGameSubType;
	pTournamentNew->m_nCurrencyId				= CurrencyType::Currency_GEL;
	pTournamentNew->m_nGameSpeedId				= p->m_nGameSpeedId;
	pTournamentNew->m_nStatus					= ((pTournamentNew->m_nSubTypeId & ((int)GameSubType::Tournament_SitNGo)) ? TournamentStatus::Tournament_Registering : TournamentStatus::Tournament_Announced);
	pTournamentNew->m_sName						= p->m_sName;
	pTournamentNew->m_sDescription				= p->m_sDescription;

	pTournamentNew->m_dtStartTime				= p->m_dtTimeStart;
	pTournamentNew->m_dtShowTime				= p->m_dtTimeShow;
	pTournamentNew->m_dtRegStartTime			= p->m_dtTimeRegStart;
	pTournamentNew->m_dtRegEndTime				= p->m_dtTimeRegEnd;

	pTournamentNew->m_nMinPlayers				= p->m_nMinPlayers;
	pTournamentNew->m_nMaxPlayers				= p->m_nMaxPlayers;

	pTournamentNew->m_shActionTimeSec			= p->m_shActionTimeSec;
	pTournamentNew->m_shTimeBankInitTimeSec		= p->m_shTBInitialSec;
	pTournamentNew->m_shTimeBankIncLevel		= p->m_shTBIncLevel;
	pTournamentNew->m_shTimeBankIncSec			= p->m_shTBIncSec;
	pTournamentNew->m_shMaxSeatCt				= p->m_shMaxSeatCt;
	pTournamentNew->m_shLevelDuration			= p->m_shLevelDurationMin;
	pTournamentNew->m_shOpenTableBeforeStartSec	= p->m_shOpenTableBeforeStartSec;

	pTournamentNew->m_dBuyIn					= (double)(p->m_nBuyinAmount / 100.0);
	pTournamentNew->m_dBuyInVPP					= (double)(p->m_nBuyinVPP / 100.0);
	pTournamentNew->m_dBuyInFee					= (double)(p->m_nBuyinFeeAmount / 100.0);
	pTournamentNew->m_nBuyInChips				= p->m_nBuyinChips;

	pTournamentNew->m_shBreakPerLevel			= p->m_shBreakPerLevel;
	pTournamentNew->m_shBreakDurationMin		= p->m_shBreakDurationMin;

	pTournamentNew->m_dRebuy					= (double)(p->m_nRebuyAmount / 100.0);
	pTournamentNew->m_dRebuyVPP					= (double)(p->m_nRebuyVPP / 100.0);
	pTournamentNew->m_dRebuyFee					= (double)(p->m_nRebuyFeeAmount / 100.0);
	pTournamentNew->m_nRebuyChips				= p->m_nRebuyChips;

	pTournamentNew->m_dAddon					= (double)(p->m_nAddonAmount / 100.0);
	pTournamentNew->m_dAddonVPP					= (double)(p->m_nAddonVPP / 100.0);
	pTournamentNew->m_dAddonFee					= (double)(p->m_nAddonFeeAmount / 100.0);
	pTournamentNew->m_nAddonChips				= p->m_nAddonChips;

	pTournamentNew->m_bPermitSameIPRegistration	= !p->m_bRestrictSameIPs;
	pTournamentNew->m_bAllowToUseVPPoints		= p->m_bAllowToUseVPPoints;

	if( !p->m_bAllowToUseVPPoints ){
		pTournamentNew->m_dBuyInVPP				= 0.0;
		pTournamentNew->m_dRebuyVPP				= 0.0;
		pTournamentNew->m_dAddonVPP				= 0.0;
		}

	// Satellite tournament.
	if( (p->m_nGameSubType&GameSubType::Tournament_Satellite) > 0 ){
		pTournamentNew->m_nSatelliteToId		= p->m_nPrizeTicketId;
		pTournamentNew->m_dTicketPrice			= p->m_dTicketPriceAmount;
		pTournamentNew->m_shGuaranteedTicketCt	= p->m_shGuaranteedTicketCt;
		}
	else{
		pTournamentNew->m_nSatelliteToId		= 0;
		pTournamentNew->m_dTicketPrice			= 0.0;
		pTournamentNew->m_shGuaranteedTicketCt	= 0;
		}

	pTournamentNew->m_shAddonBreak				= p->m_shAddonBreak;
	pTournamentNew->m_dPrizeAmount				= (double)(p->m_dPrizeAmount);
	pTournamentNew->m_shRebuyCt					= p->m_btRebuyCt;	
	pTournamentNew->m_shAddonCt					= p->m_btAddonCt;	
	pTournamentNew->m_shRecState				= 0;

	pTournamentNew->m_nBlindStructProfileId		= p->m_nBlindStructProfileId;	// By default.
	pTournamentNew->m_nPayoutProfileId			= p->m_nPayoutProfileId;		// By default.

	if( m_pDataMan->SaveRecord(pTournamentNew) ){
		m_lockAttributeTables.Lock();		// Lock
		PokerGameSpeed* pSpeed = m_listGameSpeed.FindOneVariable(_T("Id"), &pTournamentNew->m_nGameSpeedId);
		if( pSpeed != NULL )
			pSpeed->Copy(&pTournamentNew->m_gameSpeed);
		m_lockAttributeTables.Unlock();		// Unlock

		m_lockTournamentObjects.Lock();		// Lock
		TournamentBlindStructureProfile* pBlind = GetBlindStructProfileById(pTournamentNew->m_nBlindStructProfileId);
		if( pBlind )
			pBlind->m_listBlindStructureEntries.Copy_Into(&pTournamentNew->m_listBlindStructure);

		TournamentPayoutProfile* pPayout = GetPayoutProfileById(pTournamentNew->m_nPayoutProfileId);
		if( pPayout )
			pPayout->m_listPayoutEntries.Copy_Into(&pTournamentNew->m_listPayoutEntries);

		m_listTournaments.Add(pTournamentNew);
		p->m_nTournamentId					= pTournamentNew->m_nId;
		m_lockTournamentObjects.Unlock();	// Unlock
		return pTournamentNew->m_nId;
		}
	
	delete pTournamentNew;
	return 0;
	}

PokerTournamentManager*
PokerTournamentData::CreateTournamentManager(int nTournamentId, PokerServer* pServer, PokerTournaments* pGameBase){
	m_lockTournamentObjects.Lock();		// Lock
	TournamentInfo* pInfo = (TournamentInfo*)TournamentInfoById(nTournamentId);
	if( !pInfo ){
		m_lockTournamentObjects.Unlock();	// Unlock
		return NULL;
		}

	PokerTournamentManager* pMan	= new PokerTournamentManager(pServer, pGameBase);
	pInfo->Copy(&pMan->m_info);
//	pMan->m_dPrizePool				= pInfo->CalcPrizePool();
	pMan->m_nRegisteredCt			= pInfo->m_listParticipants.GetCount();
	pMan->m_nPlayersCt				= 0;
	pMan->m_bVisible				= false;
	pMan->m_nLevelIndex				= -1;
	pMan->m_bIsBreak				= false;
	pMan->m_nBreakSecondsLeft		= 0;
	pMan->m_nLevelSecondsLeft		= 0;
	pMan->m_bHandForHand			= false;
	m_lockTournamentObjects.Unlock();	// Unlock
	return pMan;
	}

int	
PokerTournamentData::CreateTournamentManagers(PokerServer* pServer, PokerTournaments* pGameBase, EnumerableObject<PokerTournamentManager>* pListMan){
	ASSERT(pServer);
	m_lockTournamentObjects.Lock();			// Lock
	if( !m_listTournaments.GetCount() || !pListMan ){
		m_lockTournamentObjects.Unlock();	// Unlock
		return 0;
		}

	int nRet = 0;
	for( int i=0; i<m_listTournaments.GetCount(); i++ ){
		TournamentInfo* pInfo			= m_listTournaments[i];
		if( !pInfo ) continue;
		PokerTournamentManager* pMan	= new PokerTournamentManager(pServer, pGameBase);
		pInfo->Copy(&pMan->m_info);
//		pMan->m_dPrizePool				= pInfo->CalcPrizePool();
		pMan->m_nRegisteredCt			= pInfo->m_listParticipants.GetCount();
		pMan->m_nPlayersCt				= 0;
		pMan->m_bVisible				= false;
		pMan->m_nLevelIndex				= -1;
		pMan->m_bIsBreak				= false;
		pMan->m_nBreakSecondsLeft		= 0;
		pMan->m_nLevelSecondsLeft		= 0;
		pMan->m_bHandForHand			= false;
		pListMan->Add(pMan);
		nRet ++;
		}
	m_lockTournamentObjects.Unlock();	// Unlock
	return nRet;
	}

int
PokerTournamentData::CreateTournamentGameTables(int nTournamentId, PokerServer* pServer, PokerGameBase* pGameBase, PokerTournamentManager* pManager, List<TournamentTableController>*	pListTables, AutoSortedArray* pArrGameTableById){
	ASSERT(pListTables && pArrGameTableById);
	const TournamentInfo* pInfo = TournamentInfoById(nTournamentId);
	if( !pInfo ) return 0; // Tournament not found !!!
	const TournamentBlindStructure* pBlindStruct = ((TournamentInfo*)pInfo)->GetLevelInfo(0);
	ASSERT(pBlindStruct);
	if( !pBlindStruct ) return 0; // Tournament blind structure unavailable!!!

	EnumerableObject<TournamentParticipant>* pListRegistrants = (EnumerableObject<TournamentParticipant>*)&pInfo->m_listParticipants;
	if( pListRegistrants->GetCount() < pInfo->m_nMinPlayers )
		return 0; // Minimum registrants required !!!

	// Shuffle registrants to seat on different tables. {{
	int				nRegistrantCt	= (int)pListRegistrants->GetCount();
	AutoSortedArray arrRandomRegistrants; // <random number><registrant index>
	arrRandomRegistrants.SetSize(nRegistrantCt);

	srand(time(NULL));
	for(int i=0; i<nRegistrantCt; i++){
		//rand(); // Skip
		arrRandomRegistrants.Add((void*)rand(), (void*)i);	
		}
	// }}

	int		nMaxSeatCt			= (int)pInfo->m_shMaxSeatCt;
	int		nTableCt			= 0;
	int		nAvgPlayerPerTable	= 0;
	int*	pArrPlayersPerTable	= TournamentTablesManager::SeatPlayersOnTables(nRegistrantCt, nMaxSeatCt, nTableCt, nAvgPlayerPerTable);
	int		nRegistrantIndex	= 0, nRebuyCt = (int)pInfo->m_shRebuyCt, nAddonCt = (int)pInfo->m_shAddonCt;

	for(int i=0; i<nTableCt; i++ ){
		TournamentTableController*		pTableController	= pManager->CreateTableController(i+1);
		PokerTableInfo*					pTableInfo			= &pTableController->m_info;

		for(int j=0; j<pArrPlayersPerTable[i]; j++){
			TournamentParticipant* pRegistrant = pListRegistrants->GetAt((int)arrRandomRegistrants.GetData(nRegistrantIndex));
			ASSERT( pRegistrant );
			if( !pRegistrant ){
				pTableInfo->m_seats				.Add(NULL);
				pTableInfo->m_listSeatAvatars	.Add(NULL);
				nRegistrantIndex ++;
				}

			PokerTableSeatInfo* pSeat = new PokerTableSeatInfo();
			pSeat->SetUserId			(pRegistrant->m_nUserId);
			pSeat->SetUserName			(pRegistrant->m_sUserName);
			pSeat->SetSeat				(j);
			pSeat->SetOffline			(false);
			//pSeat->SetOffline			(true);
			pSeat->SetBalance			( Serializable::ConvertMoney(pRegistrant->m_dAmountChips));
			pSeat->SetStatus			(PokerSeatStatus::Sitted);
			pSeat->SetTimeBankSeconds	((int)pInfo->m_shTimeBankInitTimeSec);

			// Seat user session.
			PokerSession* pSession = pServer->SeatUserSession(pRegistrant->m_nUserId, pTableInfo->m_nTableId, j, pTableInfo->m_nTournamentId, 0, nRebuyCt, nAddonCt);
			pSeat->SetSession(pSession, pTableInfo->m_nTableId, pTableInfo->m_nTournamentId); // Set seat session.
			pTableInfo->m_seats.Add(pSeat);
			if( pSession ){
				UserInfo* pUser = pSession->GetUserInfo();
				//pTableInfo->m_listSeatAvatars	.Add(pUser->GetAvatarInfo());
				pTableInfo->m_listSeatAvatars	.Add(NULL);
				// Set avatar id.
				pSeat->SetAvatarId( (pUser->m_bAvatarIsBlocked ? 0 : pUser->m_nAvatarId) );
				// Set rank level.
				pSeat->SetRankLevel((int)pUser->m_shRankLevel);
				}
			else
				pTableInfo->m_listSeatAvatars.Add(NULL);

			// Call Seat Taken notify.
			pManager->OnSeatTaken(pTableController, j, pTableInfo->m_nTableId, pRegistrant->m_nUserId);

			if( pSession ) SafePointer::Release(pSession); // Release session pointer.
			nRegistrantIndex ++;
			}

		for(int j=pArrPlayersPerTable[i]; j<pTableInfo->m_cMaxPlayersCt; j++){
			pTableInfo->m_seats				.Add(NULL);
			pTableInfo->m_listSeatAvatars	.Add(NULL);
			}
		pListTables			->Add(pTableController);
		pArrGameTableById	->Add((void*)pTableInfo->m_nTableId, (void*)pTableController);
		}
	
	delete [] pArrPlayersPerTable;
	return pListTables->GetCount();
	}

bool
PokerTournamentData::CancelTournament(int nTournamentId, int nUserId, EnumerableObject<CancelTournamentProc_Return>& refundPlayers){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	CancelTournamentProc proc;
	proc.m_nTournamentId	= nTournamentId;
//	proc.m_nUserId			= nUserId;
	if( m_pDataMan->ExecuteProc(&proc) ){
		m_lockTournamentObjects.Lock();		// Lock
		TournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("Id"), &nTournamentId);
		if( pTournamentInfo ){
			pTournamentInfo->m_nStatus = TournamentStatus::Tournament_Canceled;
			//m_listTournaments.Remove(pTournamentInfo, true);
			}
		m_lockTournamentObjects.Unlock();	// Unlock

		for(int i=0; i<proc.m_refundInfo.GetCount(); i++)
			refundPlayers.Add(proc.m_refundInfo.GetAt(i));
		proc.m_refundInfo.RemoveAll(false);
		return true;
		}
	return false;
	}

bool
PokerTournamentData::CancelTournamentRefund(int nTournamentId, int nUserId, double dRefundAmount){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	CancelTournamentRefundProc proc;
	proc.m_nTournamentId	= nTournamentId;
	proc.m_nUserId			= nUserId;
	proc.m_dRefundAmount	= dRefundAmount;
	if( m_pDataMan->ExecuteProc(&proc) && !proc.m_nRetCode )
		return true;
	return false;
	}

bool
PokerTournamentData::HideTournament(int nTournamentId){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	HideTournamentProc proc;
	proc.m_nTournamentId	= nTournamentId;

	if( m_pDataMan->ExecuteProc(&proc) && !proc.m_nRetCode ){
		m_lockTournamentObjects.Lock();		// Lock
		TournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("Id"), &nTournamentId);
		if( pTournamentInfo )
			m_listTournaments.Remove(pTournamentInfo, true);
		m_lockTournamentObjects.Unlock();	// Unlock
		return true;
		}
	return false;
	}

int
PokerTournamentData::GetTournamentInfoObjects(int nTournamentType, EnumerableObject<TournamentInfoObject>* pListObjects){
	m_lockTournamentObjects.Lock();		// Lock
	int nLoop = 0, nCt = m_listTournaments.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		TournamentInfo* pInfo = m_listTournaments.GetAt(nLoop);
		if( pInfo && (pInfo->m_nSubTypeId&nTournamentType) > 0 ){
			TournamentInfoObject* pNew	= new TournamentInfoObject();
			pNew->m_nId					= pInfo->m_nId;
			pNew->m_sName				= pInfo->m_sName;
			pNew->m_sDescription		= pInfo->m_sDescription;
			pNew->m_nTournamentType		= IS_VALID_TOURNAMENT_TYPE(pInfo->m_nSubTypeId);
			pNew->m_cGameType			= (int)pInfo->m_nGameTypeId;
			pNew->m_nGameSubType		= IS_VALID_GAME_SUB_TYPE(pInfo->m_nSubTypeId);;
			pNew->m_cStatus				= (char)pInfo->m_nStatus;
			pNew->m_nRegisteredCt		= pInfo->m_nEntriesCt;
			pNew->m_nPlayersCt			= 0;
			pNew->m_nTablesCt			= pInfo->m_listGameTables.GetCount();
			pNew->m_dtStartTime			= pInfo->m_dtStartTime;
			pNew->m_dtRegStartTime		= pInfo->m_dtRegStartTime;
			pNew->m_dtRegEndTime		= pInfo->m_dtRegEndTime;
			pNew->m_dtFinishedTime		= pInfo->m_dtFinished;
			pListObjects->Add(pNew);
			nResult ++;
			}
		nLoop ++;
		}
	m_lockTournamentObjects.Unlock();	// Unlock
	return nResult;
	}

bool
PokerTournamentData::RebuildTournamentPayouts(int nTournamentId, LobbyInfo* pLobby){
	if( !pLobby ) return false;
	m_lockTournamentObjects.Lock();		// Lock
	TournamentInfo* pInfo = (TournamentInfo*)TournamentInfoById(nTournamentId);
	if( !pInfo ){
		m_lockTournamentObjects.Unlock();	// Unlock
		return false;
		}

	bool bRet = pLobby->RebuildTournamentPayouts(pInfo);
	m_lockTournamentObjects.Unlock();	// Unlock
	return bRet;
	}

bool
PokerTournamentData::Rebuy(int nUserId, int nTournamentId, int nRebuyCtAtOnce, double& dBalance, double& dVPPoints, bool bUseVPPoints /*= false*/){
	if( !m_pDataMan || !m_pDataMan->IsOpen() ) 
		return false;
	
	TournamentRebuyProc proc;
	proc.m_nUserId			= nUserId;
	proc.m_nTournamentId	= nTournamentId;
	proc.m_bByPoints		= bUseVPPoints;
	proc.m_shRebuyCtAtOnce	= (short)(nRebuyCtAtOnce <= 0 ? 1 : nRebuyCtAtOnce);
	proc.m_dBalance			= 0.0;
	proc.m_dPoints			= 0.0;

	if( m_pDataMan->ExecuteProc(&proc) && !proc.m_nSuccess ){
		m_lockTournamentObjects.Lock(); // Lock
		const TournamentInfo* pInfo = TournamentInfoById(nTournamentId);
		if( pInfo )
			((TournamentInfo*)pInfo)->m_nRebuysCt ++;
		m_lockTournamentObjects.Unlock(); // Unlock

		if( !bUseVPPoints )
			dBalance = proc.m_dBalance;
		else
			dVPPoints = proc.m_dPoints;
		return true;
		}
	return false;
	}

bool
PokerTournamentData::Addon(int nUserId, int nTournamentId, double& dBalance, double& dVPPoints, bool bUseVPPoints /*= false*/){
	if( !m_pDataMan || !m_pDataMan->IsOpen() ) 
		return false;
	TournamentAddonProc proc;
	proc.m_nUserId			= nUserId;
	proc.m_nTournamentId	= nTournamentId;
	proc.m_bByPoints		= bUseVPPoints;
	proc.m_dBalance			= 0.0;
	proc.m_dPoints			= 0.0;

	if( m_pDataMan->ExecuteProc(&proc) && !proc.m_nSuccess ){
		m_lockTournamentObjects.Lock(); // Lock
		const TournamentInfo* pInfo = TournamentInfoById(nTournamentId);
		if( pInfo )
			((TournamentInfo*)pInfo)->m_nAddonsCt ++;
		m_lockTournamentObjects.Unlock(); // Unlock

		if( !bUseVPPoints )
			dBalance = proc.m_dBalance;
		else
			dVPPoints = proc.m_dPoints;
		return true;
		}
	return false;
	}

const TournamentInfo*	
PokerTournamentData::TournamentInfoById(int nTournamentId){
	return (const TournamentInfo*)m_listTournaments.FindOneVariable(_T("Id"), &nTournamentId);
	}

bool
PokerTournamentData::IsBreakAfterLevel(int nTournamentId, int nLevelIndex){
	m_lockTournamentObjects.Lock(); // Lock
	const TournamentInfo* pInfo = TournamentInfoById(nTournamentId);
	if( !pInfo ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false;
		}

	bool bIsBreak;
	if( pInfo->m_shBreakPerLevel <= 0 )
		bIsBreak = false;
	else
		bIsBreak = (((nLevelIndex + 1) % pInfo->m_shBreakPerLevel) == 0);
	m_lockTournamentObjects.Unlock(); // Unlock
	return bIsBreak;
	}

const TournamentBlindStructure*	
PokerTournamentData::GetTournamentLevelInfo	(int nTournamentId, int nLevelIndex){
	m_lockTournamentObjects.Lock(); // Lock
	TournamentInfo* pInfo = (TournamentInfo*)TournamentInfoById(nTournamentId);
	if( !pInfo ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return NULL;
		}
	m_lockTournamentObjects.Unlock(); // Unlock
	return pInfo->GetLevelInfo(nLevelIndex);
	}

TournamentPayoutProfile*
PokerTournamentData::GetPayoutProfileById(int nProfileId){
	TournamentPayoutProfile* pPayoutProf = m_listPayoutProfiles.FindOneVariable(_T("Id"), &nProfileId);
	if( !pPayoutProf || !pPayoutProf->m_listPayoutEntries.GetCount() ){
		_String sWhereSQL;
		EnumerableObject<TournamentPayoutProfile> listProfiles;
		// Load payout profiles.
		sWhereSQL.Format(_T("where [ID]=%d"), nProfileId);
		m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&listProfiles, sWhereSQL, _T(""));

		// Load payout entries.
		if( listProfiles.GetCount() > 0 ){
			TournamentPayoutProfile* p = listProfiles.GetAt(0);
			if( !p ) return NULL;

			sWhereSQL.Format(_T("where [PAYOUT_PROFILE_ID]=%d"), p->m_nId);
			m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&p->m_listPayoutEntries, sWhereSQL, _T(""));

			if( pPayoutProf )
				m_listPayoutProfiles.Remove(pPayoutProf, true);

			listProfiles.RemoveAt(0, false);
			m_listPayoutProfiles.Add(p);
			return p;
			}
		}
	return pPayoutProf;
	}

TournamentBlindStructureProfile*
PokerTournamentData::GetBlindStructProfileById(int nProfileId){
	TournamentBlindStructureProfile* pBlindProf = m_listBlindStructProfiles.FindOneVariable(_T("Id"), &nProfileId);
	if( !pBlindProf || !pBlindProf->m_listBlindStructureEntries.GetCount() ){
		_String sWhereSQL;
		EnumerableObject<TournamentBlindStructureProfile> listProfiles;
		// Load blind structrure profiles.
		sWhereSQL.Format(_T("where [ID]=%d"), nProfileId);
		m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&listProfiles, sWhereSQL, _T(""));

		// Load blind entries.
		if( listProfiles.GetCount() > 0 ){
			TournamentBlindStructureProfile* p = listProfiles.GetAt(0);
			if( !p ) return NULL;

			sWhereSQL.Format(_T("where [BLIND_STRUCT_PROFILE_ID]=%d"), p->m_nId);
			m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&p->m_listBlindStructureEntries, sWhereSQL, _T(""));

			if( pBlindProf )
				m_listBlindStructProfiles.Remove(pBlindProf, true);

			listProfiles.RemoveAt(0, false);
			m_listBlindStructProfiles.Add(p);
			return p;
			}
		}
	return pBlindProf;
	}

__int64
PokerTournamentData::StartHand(int nTournamentId, int nTableId, int nLevelIndex, int nPlayersCt, UINT nSB, UINT nBB, UINT nAnte){
	if( !m_pDataMan || !m_pDataMan->IsOpen() ) return 0;
	StartTournamentHandProc proc;
	proc.m_nTournamentId		= nTournamentId;
	proc.m_nTableId				= nTableId;
	proc.m_nLevelIndex			= nLevelIndex;
	proc.m_nPlayersCt			= nPlayersCt;
	proc.m_dBigBlindAmount		= (double)(nSB / 100.0);
	proc.m_dSmallBlindAmount	= (double)(nBB / 100.0);
	proc.m_dAnteAmount			= (double)(nAnte / 100.0);
	m_lockTournamentObjects.Unlock(); // Unlock

	if( m_pDataMan->ExecuteProc(&proc) )
		return proc.m_nHandId;
	return 0;
	}

bool
PokerTournamentData::FinishHand(__int64 nHandId, int nTableId, INT_CHIPS nTotalPotAmount, _String& sTableCards, _String& sPlayersInfo, void* lpHandData, int nSize){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	FinishTournamentHandProc proc;
	proc.m_bFinishStatus	= false;
	proc.m_nHandId			= nHandId;
	proc.m_nTableId			= nTableId;
	proc.m_sPlayersInfo		= sPlayersInfo;
	proc.m_sTableCards		= sTableCards;
	proc.m_dTotalPotAmount	= (double)(nTotalPotAmount / 100.0);

	// Allocate binary memory and copy.
	proc.m_binHandData.SetBinarySize(nSize, lpHandData);

	if( m_pDataMan->ExecuteProc(&proc) ){
		return proc.m_bFinishStatus;
		}
	return false;
	}

bool
PokerTournamentData::FinishTournament(int nTournamentId){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	FinishTournamentProc proc;
	proc.ZeroInit();
	proc.m_nTournamentId	= nTournamentId;

	m_lockTournamentObjects.Lock(); // Lock
	TournamentInfo*	pTournamentInfo = (TournamentInfo*)m_listTournaments.FindOneVariable(_T("Id"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false;
		}
	proc.m_dtFinished.SetCurrentTime();
	proc.m_nPlayersCt		= pTournamentInfo->m_listParticipants.GetCount();
	m_lockTournamentObjects.Unlock(); // Unlock
	if( m_pDataMan->ExecuteProc(&proc) && !proc.m_nSuccess )
		return true;
	return false;
	}

bool
PokerTournamentData::PlayerFinishedTournament(int nTournamentId, int nUserId, int& nPlace, double& dAmountWon, bool& bHandForHand, bool& bFinishTournament, int& nTournamentTicketId){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	m_lockTournamentObjects.Lock(); // Lock
	TournamentInfo*			pInfo			= (TournamentInfo*)TournamentInfoById(nTournamentId);
	TournamentParticipant*	pParticipant	= NULL;

	if( pInfo ){
		if( pInfo->PlayerFinished(nUserId, nPlace, dAmountWon, pParticipant, bHandForHand) ){
			m_lockTournamentObjects.Unlock(); // Unlock

			if( pParticipant ){
				nTournamentTicketId = pParticipant->m_nTournamentTicketIdWon;
				bFinishTournament	= (nTournamentTicketId != 0);
				}

			PlayerFinishedTournamentProc proc;
			proc.m_nParticipantId	= pParticipant->m_nId;
			proc.m_nTournamentId	= nTournamentId;
			proc.m_nUserId			= nUserId;
			proc.m_nPlace			= nPlace;
			proc.m_dAmountWon		= dAmountWon;
			proc.m_nCurrencyId		= pInfo->m_nCurrencyId;

			return (m_pDataMan->ExecuteProc(&proc) && !proc.m_nSuccess);
			}
		}
	m_lockTournamentObjects.Unlock(); // Unlock
	return false;
	}

bool
PokerTournamentData::SetTournamentStatus(int nTournamentId, TournamentStatus statusNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	m_lockTournamentObjects.Lock(); // Lock
	TournamentInfo* pInfo = (TournamentInfo*)TournamentInfoById(nTournamentId);
	if( !pInfo ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false;
		}
	TournamentStatus	statusOld = (TournamentStatus)pInfo->m_nStatus;
	if( statusOld != statusNew ){
		pInfo->m_nStatus = (int)statusNew;
		m_lockTournamentObjects.Unlock(); // Unlock

		CStringVector arrDirtyFields;
		arrDirtyFields.push_back(_T("STATUS_ID"));
		if( !m_pDataMan->SaveRecord(pInfo, &arrDirtyFields) )
			pInfo->m_nStatus = statusOld;
		}
	else
		m_lockTournamentObjects.Unlock(); // Unlock
	return true;
	}

bool
PokerTournamentData::SetTournamentLevel(int nTournamentId, int nLevelIndex){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	m_lockTournamentObjects.Lock(); // Lock
	TournamentInfo* pInfo = (TournamentInfo*)TournamentInfoById(nTournamentId);
	if( !pInfo ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false;
		}

	if( pInfo->m_nCurrentLevelIndex != nLevelIndex ){
		int nLevelIndexOld = pInfo->m_nCurrentLevelIndex;
		pInfo->m_nCurrentLevelIndex = nLevelIndex;
		m_lockTournamentObjects.Unlock(); // Unlock

		CStringVector arrDirtyFields;
		arrDirtyFields.push_back(_T("CURRENT_LEVEL_INDEX"));
		if( !m_pDataMan->SaveRecord(pInfo, &arrDirtyFields) )
			pInfo->m_nCurrentLevelIndex = nLevelIndexOld;
		}
	else
		m_lockTournamentObjects.Unlock(); // Unlock
	return true;
	}

bool	
PokerTournamentData::RegisterUserInTournament(int nUserId, int nTournamentId, _String sIp, int nIp, INT_CHIPS& nBalanceNew, double& dVPPointsNew, bool bByPoints /*= false*/){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	m_lockTournamentObjects.Lock(); // Lock
	const TournamentInfo* pInfo = TournamentInfoById(nTournamentId);
	if( !pInfo || 
		(bByPoints && !pInfo->m_bAllowToUseVPPoints) || 
		((pInfo->m_nEntriesCt + 1) > pInfo->m_nMaxPlayers) ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false;
		}

	if( ((TournamentInfo*)pInfo)->m_listParticipants.FindOneVariable(_T("UserId"), &nUserId) ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false; // That user already registered.
		}

	// Check for IP address.
	int nIpNumber = nIp;
	if( pInfo->m_bPermitSameIPRegistration && ((TournamentInfo*)pInfo)->m_listParticipants.FindOneVariable(_T("Ip_Number"), &nIpNumber) ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false; // That Ip address already registered.
		}
	m_lockTournamentObjects.Unlock(); // Unlock

	RegisterInTournamentProc proc;
	proc.m_nUserId			= nUserId;
	proc.m_sIp				= sIp;
	proc.m_nIP				= nIp;
	proc.m_nTournamentId	= nTournamentId;
	proc.m_bUseVPPoints		= bByPoints;
	proc.m_nCurrencyId		= pInfo->m_nCurrencyId;
	proc.m_dBalanceNew		= 0.0;
	proc.m_dVPPointsNew		= 0.0;
	
	nBalanceNew = 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nErrorCode == 0 && proc.m_return.GetCount() > 0 ){
		// Add new participant in memory list.
		TournamentParticipant* pNewRegistrant = proc.m_return.GetAt(0);
		m_lockTournamentObjects.Lock(); // Lock
		((TournamentInfo*)pInfo)->m_listParticipants.Add(pNewRegistrant);
		((TournamentInfo*)pInfo)->m_nEntriesCt ++;
		m_lockTournamentObjects.Unlock(); // Unlock

		if( proc.m_return.GetCount() > 1 ){
			proc.m_return.RemoveAt(0, false);
			proc.m_return.RemoveAll(true);
			}
		else
			proc.m_return.RemoveAll(false);
		
		if( bByPoints )
			dVPPointsNew = proc.m_dVPPointsNew;
		else
			nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool	
PokerTournamentData::RegisterUserInTournamentByTicket(int nUserId, int nTournamentId, _String sIp, int nIp, INT_CHIPS& nBalanceNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	m_lockTournamentObjects.Lock(); // Lock
	const TournamentInfo* pInfo = TournamentInfoById(nTournamentId);
	if( !pInfo || (pInfo->m_nEntriesCt + 1) > pInfo->m_nMaxPlayers ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false;
		}

	if( ((TournamentInfo*)pInfo)->m_listParticipants.FindOneVariable(_T("UserId"), &nUserId) ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false; // That user already registered.
		}
	m_lockTournamentObjects.Unlock(); // Unlock

	RegisterInTournamentByTicketProc proc;
	proc.m_nUserId			= nUserId;
	proc.m_sIp				= sIp;
	proc.m_nIP				= nIp;
	proc.m_nTournamentId	= nTournamentId;
	proc.m_nCurrencyId		= pInfo->m_nCurrencyId;
	proc.m_dBalanceNew		= 0.0;
	
	nBalanceNew = 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nErrorCode == 0 && proc.m_return.GetCount() > 0 ){
		m_lockTournamentObjects.Lock(); // Lock
		// Add new participant in memory list.
		TournamentParticipant* pNewRegistrant = proc.m_return.GetAt(0);
		((TournamentInfo*)pInfo)->m_listParticipants.Add(pNewRegistrant);
		((TournamentInfo*)pInfo)->m_nEntriesCt ++;
		m_lockTournamentObjects.Unlock(); // Unlock

		if( proc.m_return.GetCount() > 1 ){
			proc.m_return.RemoveAt(0, false);
			proc.m_return.RemoveAll(true);
			}
		else
			proc.m_return.RemoveAll(false);
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerTournamentData::UnregisterUserFromTournament(int nUserId, int nTournamentId, INT_CHIPS& nBalanceNew, double& dVPPointsNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	m_lockTournamentObjects.Lock(); // Lock
	const TournamentInfo* pInfo = TournamentInfoById(nTournamentId);
	if( !pInfo ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false; // Tournament not have been found !!!
		}

	if( !((TournamentInfo*)pInfo)->m_listParticipants.FindOneVariable(_T("UserId"), &nUserId) ){
		m_lockTournamentObjects.Unlock(); // Unlock
		return false; // That user not registered.
		}
	m_lockTournamentObjects.Unlock(); // Unlock

	UnRegisterFromTournamentProc proc;
	proc.m_nUserId			= nUserId;
	proc.m_nTournamentId	= nTournamentId;
	proc.m_dBalanceNew		= 0.0;
	proc.m_dVPPointsNew		= 0.0;
	
	nBalanceNew = 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		m_lockTournamentObjects.Lock(); // Lock
		// Remove participant from memory list. {{
		TournamentParticipant* pRegistrant = ((TournamentInfo*)pInfo)->m_listParticipants.FindOneVariable(_T("Id"), &proc.m_nParticipantId);
		if( pRegistrant )
			((TournamentInfo*)pInfo)->m_listParticipants.Remove(pRegistrant, true);
		((TournamentInfo*)pInfo)->m_nEntriesCt --;
		// }}
		m_lockTournamentObjects.Unlock(); // Unlock
		nBalanceNew		= ConvertMoney(proc.m_dBalanceNew);
		dVPPointsNew	= proc.m_dVPPointsNew;
		return true;
		}
	return false;
	}