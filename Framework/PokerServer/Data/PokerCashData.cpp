#include "stdafx.h"
#include "../PokerCashTableController.h"
#include "../../Protocol/PokerAdministrationPacket.h"
#include "PokerCashData.h"
#include "../../Utility/GrowableMemory.h"
#include "../../Path/Path.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerCashData::PokerCashData() : PokerData() {
	// Create index
	m_listGameTable.CreateVariableIndex(_T("TableId"), (CompareProc)Serializable::Compare_Int32);
	}

BEGIN_DECLARE_VAR(PokerCashData, Serializable)
	DECLARE_VAR(_T("PokerCashData"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("GameTable"),		_T(""), VariableType::VT_None, offsetof(PokerCashData,m_listGameTable), false, true, true)
END_DECLARE_VAR()

PokerCashData::~PokerCashData(){
	FreeData(true); // Free data close database.
	}

bool
PokerCashData::LoadData(){
	if( !PokerData::LoadData() )
		return false;
	
	m_lockGameTables.Lock(); // Lock
	// Load game tables.
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listGameTable, _T("where [STATUS]=0"), _T(""));

	// Link game speed info.
	for(int i=0; i<m_listGameTable.GetCount(); i++){
		PokerGameTable* pTable = (PokerGameTable*)m_listGameTable.GetAt(i);
		if( !pTable ) continue;

		// Playmoney table.
		if( pTable->m_nMoneyType == MoneyType::Play ){
			pTable->m_nCurrencyId	= 0;
			pTable->m_dRakePercent	= 0.0; // No rake.
			}

		PokerGameSpeed* pGameSpeed = m_listGameSpeed.FindOneVariable(_T("Id"), &pTable->m_nGameSpeedId);
		if( pGameSpeed )
			pGameSpeed->Copy(&pTable->m_speed);
		else
			pTable->m_speed.ZeroInit();
		}

	m_lockGameTables.Unlock(); // Unlock
	return true;
	}

void
PokerCashData::FreeData(bool bCloseDb /*= false*/){
	PokerData::FreeData(bCloseDb);
	m_lockGameTables.Lock();	// Lock
	m_listGameTable.RemoveAll(true);		
	m_lockGameTables.Unlock(); // Unlock
	}

int
PokerCashData::CreateGameTableControllers(PokerServer* pServer, PokerGameBase* pGameBase,List<PokerTableController>* pListControllers, AutoSortedArray* pArrControllerById){
	ASSERT(pServer);
	m_lockGameTables.Lock(); // Lock
	if( !m_listGameTable.GetCount() || !pListControllers || !pArrControllerById ){
		m_lockGameTables.Unlock(); // Unlock
		return 0;
		}

	int nRet = 0;
	for( int i=0; i<m_listGameTable.GetCount(); i++ ){
		PokerGameTable* pTable = m_listGameTable[i];
		if( !pTable )continue;

		PokerCashTableController* pContr = new PokerCashTableController(pServer, pGameBase);
		PokerTableInfo* pInfo		= &pContr->m_info;
		pInfo->m_nTableId			= pTable->m_nTableId;
		pInfo->m_sTableName			= pTable->m_sName;
		pInfo->ClearStatInfo();
		pInfo->m_cGameTypeId		= (BYTE)pTable->m_nGameType;
		pInfo->m_shSubTypeId		= (short)pTable->m_nGameSubType;
		pInfo->m_cMoneyTypeId		= (BYTE)pTable->m_nMoneyType;
		pInfo->m_cGameSpeedId		= (BYTE)pTable->m_nGameSpeedId;

		pInfo->m_fRakePercent		= (float)pTable->m_dRakePercent;
		pInfo->m_nRakeAmountMax		= (int)(pTable->m_dRakeAmountMax*100);
		pInfo->m_cCurrencyId		= (BYTE)pTable->m_nCurrencyId;

		pInfo->m_gameStatus			= PokerGameStatus::NotStarted;
		pInfo->m_round				= PokerRoundType::No;
		pInfo->m_nSmallBlindAmount	= (int)(pTable->m_dSmallBlind*100);
		pInfo->m_nBigBlindAmount	= (int)(pTable->m_dBigBlind*100);
		pInfo->m_nAnteAmount		= (int)(pTable->m_dAnte*100);
		pInfo->m_nMinBuyIn			= (int)(pTable->m_dMinBuyin*100);
		pInfo->m_nMaxBuyIn			= (int)(pTable->m_dMaxBuyin*100);

		pInfo->m_cMaxPlayersCt		= ((int)pTable->m_shMaxPlayerCt)&0xFF;
		pInfo->m_cCard1				= -1;
		pInfo->m_cCard2				= -1;
		pInfo->m_cCard3				= -1;
		pInfo->m_cCard4				= -1;
		pInfo->m_cCard5				= -1;

		pContr->m_nCurrencyId		= pTable->m_nCurrencyId;
		pContr->m_nSBAmount			= (int)(pTable->m_dSmallBlind*100);
		pContr->m_nBBAmount			= (int)(pTable->m_dBigBlind*100);
		pContr->m_nAnteAmount		= (int)(pTable->m_dAnte*100);
		pContr->m_nMinBuyIn			= (int)(pTable->m_dMinBuyin*100);
		pContr->m_nMaxBuyIn			= (int)(pTable->m_dMaxBuyin*100);
		
		if( pTable->m_speed.m_nId > 0 ){
			pContr->m_nHandStartDelay		= (int)pTable->m_speed.m_shHandStartDelay;
			pContr->m_nPostBlindDelay		= (int)pTable->m_speed.m_shPostBlindDelay;
			pContr->m_nStartRoundDelay		= (int)pTable->m_speed.m_shStartRoundDelay;
			pContr->m_nRegularTimeSec		= (int)pTable->m_speed.m_shRegularTimeSec;
			pContr->m_nExtraTimeSec			= (int)pTable->m_speed.m_shExtraTimeSec;
			pContr->m_nTimeBankSec			= (int)pTable->m_speed.m_shTimeBankSec;
			pContr->m_nTimeBankInc			= (int)pTable->m_speed.m_shTimeBankIncSec;
			pContr->m_nTimeBankIncHands		= pTable->m_speed.m_nTimeBankIncPerHands;	
			pContr->m_nShowMuckDelay		= (int)pTable->m_speed.m_shShowMuckDelay;
			pContr->m_nReserveSeatDelay		= (int)pTable->m_speed.m_shReserveSeatDelay;
			pContr->m_nLeaveSitoutedDelay	= (int)pTable->m_speed.m_shLeaveSitOutedDelay;
			pContr->m_nLeaveZeroBalancedSeatDelay	= (int)pTable->m_speed.m_shLeaveZeroBalancedSeatDelay;
			pInfo->m_shTimeBankSec			= pTable->m_speed.m_shTimeBankSec;
			}
		else{ // Default values.
			pContr->m_nHandStartDelay		= 10;
			pContr->m_nPostBlindDelay		= 4;
			pContr->m_nStartRoundDelay		= 3;
			pContr->m_nRegularTimeSec		= 20;
			pContr->m_nExtraTimeSec			= 5;

			pInfo->m_shTimeBankSec			= 60;
			pContr->m_nTimeBankSec			= 60;
			pContr->m_nTimeBankInc			= 1;
			pContr->m_nTimeBankIncHands		= 1;	

			pContr->m_nShowMuckDelay		= 5;
			pContr->m_nReserveSeatDelay		= 5;
			pContr->m_nLeaveSitoutedDelay	= 30;
			pContr->m_nLeaveZeroBalancedSeatDelay	= 10;
			}
		
		pInfo->m_btRegularActTimeSec= (BYTE)pContr->m_nRegularTimeSec;
		pInfo->m_btExtraActTimeSec	= (BYTE)pContr->m_nExtraTimeSec;
		pContr->m_gametype			= (GameType)pTable->m_nGameType;
		pContr->m_subType			= (GameSubType)pTable->m_nGameSubType;
		pContr->m_moneyType			= (MoneyType)pTable->m_nMoneyType;

		for(int j=0; j<pInfo->m_cMaxPlayersCt; j++){
			pInfo->m_listSeatAvatars.Add(NULL);
			pInfo->m_seats			.Add(NULL);
			}

		pListControllers	->Add(pContr);
		pArrControllerById	->Add((void*)pTable->m_nTableId, (void*)pContr);
		nRet ++;
		}
	m_lockGameTables.Unlock(); // Unlock
	return nRet;
	}

int
PokerCashData::CreateGameTable(PokerServer* pServer, PokerGameBase* pGameBase, CreateGameTableObject* pTableObject, PokerTableController*& ppTableController){
	if( !pTableObject ) return 0;
	PokerGameTable* pTable		= new PokerGameTable();
	pTable->m_nTableId			= 0;		
	pTable->m_nAdminUserId		= 0;	
	pTable->m_sName				= pTableObject->m_sName;		
	pTable->m_nCurrencyId		= pTableObject->m_nCurrencyId;	
	pTable->m_nMoneyType		= (int)MoneyType::Real;	
	pTable->m_nGameType			= (int)pTableObject->m_cGameType;	
	pTable->m_nGameSubType		= (int)pTableObject->m_shGameSubType;	
	pTable->m_nGameSpeedId		= pTableObject->m_nGameSpeedId;	
	pTable->m_nPlayedHandsCt	= 0;
//	pTable->m_dMinBet			= (double)pTableObject->m_fMinBet;
//	pTable->m_dMaxBet			= (double)pTableObject->m_fMaxBet;
	pTable->m_dMinBuyin			= (double)pTableObject->m_fMinBuyin;
	pTable->m_dMaxBuyin			= (double)pTableObject->m_fMaxBuyin;
	pTable->m_dSmallBlind		= (double)pTableObject->m_fSmallBlind;
	pTable->m_dBigBlind			= (double)pTableObject->m_fBigBlind;
	pTable->m_dAnte				= (double)pTableObject->m_fAnte;
	pTable->m_dRakePercent		= (double)pTableObject->m_fRakePercent;
	pTable->m_dRakeAmountMax	= (double)pTableObject->m_fMaxRakeAmount;
	pTable->m_bOffline			= false;
	pTable->m_bPrivate			= pTableObject->m_sPassword.IsEmpty() ? false : true;
	pTable->m_sPwd				= pTableObject->m_sPassword;
	pTable->m_shMaxPlayerCt		= (short)pTableObject->m_cMaxSeatCt;
	pTable->m_shStatus			= 0;

	PokerGameSpeed* pGameSpeed = m_listGameSpeed.FindOneVariable(_T("Id"), &pTable->m_nGameSpeedId);
	if( pGameSpeed )
		pGameSpeed->Copy(&pTable->m_speed);
	else
		pTable->m_speed.ZeroInit();

	if( !m_pDataMan->SaveRecord(pTable, NULL) )
		return 0;

	PokerCashTableController* pContr	= new PokerCashTableController(pServer, pGameBase);
	PokerTableInfo* pInfo				= &pContr->m_info;
	pInfo->m_nTableId					= pTable->m_nTableId;
	pInfo->m_sTableName					= pTable->m_sName;
	pInfo->ClearStatInfo();
	pInfo->m_cGameTypeId				= (BYTE)pTable->m_nGameType;
	pInfo->m_shSubTypeId				= (short)pTable->m_nGameSubType;
	pInfo->m_cMoneyTypeId				= (BYTE)pTable->m_nMoneyType;
	pInfo->m_cGameSpeedId				= (BYTE)pTable->m_nGameSpeedId;
	pInfo->m_fRakePercent				= (float)pTable->m_dRakePercent;
	pInfo->m_nRakeAmountMax				= (UINT)Serializable::ConvertMoney(pTable->m_dRakeAmountMax);
	pInfo->m_cCurrencyId				= (BYTE)pTable->m_nCurrencyId;
	pInfo->m_gameStatus					= PokerGameStatus::NotStarted;
	pInfo->m_round						= PokerRoundType::No;
	pInfo->m_nSmallBlindAmount			= (UINT)Serializable::ConvertMoney(pTable->m_dSmallBlind);
	pInfo->m_nBigBlindAmount			= (UINT)Serializable::ConvertMoney(pTable->m_dBigBlind);
	pInfo->m_nAnteAmount				= (UINT)Serializable::ConvertMoney(pTable->m_dAnte);
	pInfo->m_nMinBuyIn					= (UINT)Serializable::ConvertMoney(pTable->m_dMinBuyin);
	pInfo->m_nMaxBuyIn					= (UINT)Serializable::ConvertMoney(pTable->m_dMaxBuyin);
	pInfo->m_cMaxPlayersCt				= ((int)pTable->m_shMaxPlayerCt)&0xFF;
	pInfo->m_cCard1						= -1;
	pInfo->m_cCard2						= -1;
	pInfo->m_cCard3						= -1;
	pInfo->m_cCard4						= -1;
	pInfo->m_cCard5						= -1;

	pContr->m_nCurrencyId				= pTable->m_nCurrencyId;
	pContr->m_nSBAmount					= pInfo->m_nSmallBlindAmount;
	pContr->m_nBBAmount					= pInfo->m_nBigBlindAmount;
	pContr->m_nAnteAmount				= pInfo->m_nAnteAmount;
	pContr->m_nMinBuyIn					= pInfo->m_nMinBuyIn;
	pContr->m_nMaxBuyIn					= pInfo->m_nMaxBuyIn;
		
	if( pTable->m_speed.m_nId > 0 ){
		pContr->m_nHandStartDelay		= (int)pTable->m_speed.m_shHandStartDelay;
		pContr->m_nPostBlindDelay		= (int)pTable->m_speed.m_shPostBlindDelay;
		pContr->m_nStartRoundDelay		= (int)pTable->m_speed.m_shStartRoundDelay;
		pContr->m_nRegularTimeSec		= (int)pTable->m_speed.m_shRegularTimeSec;
		pContr->m_nExtraTimeSec			= (int)pTable->m_speed.m_shExtraTimeSec;
		pContr->m_nTimeBankSec			= (int)pTable->m_speed.m_shTimeBankSec;
		pContr->m_nTimeBankInc			= (int)pTable->m_speed.m_shTimeBankIncSec;
		pContr->m_nTimeBankIncHands		= pTable->m_speed.m_nTimeBankIncPerHands;	
		pContr->m_nShowMuckDelay		= (int)pTable->m_speed.m_shShowMuckDelay;
		pContr->m_nReserveSeatDelay		= (int)pTable->m_speed.m_shReserveSeatDelay;
		pContr->m_nLeaveSitoutedDelay	= (int)pTable->m_speed.m_shLeaveSitOutedDelay;
		pContr->m_nLeaveZeroBalancedSeatDelay	= (int)pTable->m_speed.m_shLeaveZeroBalancedSeatDelay;
		pInfo->m_shTimeBankSec			= pTable->m_speed.m_shTimeBankSec;
		}
	else{ // Default values.
		pContr->m_nHandStartDelay		= 10;
		pContr->m_nPostBlindDelay		= 4;
		pContr->m_nStartRoundDelay		= 3;
		pContr->m_nRegularTimeSec		= 20;
		pContr->m_nExtraTimeSec			= 5;

		pInfo->m_shTimeBankSec			= 60;
		pContr->m_nTimeBankSec			= 60;
		pContr->m_nTimeBankInc			= 1;
		pContr->m_nTimeBankIncHands		= 1;	

		pContr->m_nShowMuckDelay		= 5;
		pContr->m_nReserveSeatDelay		= 5;
		pContr->m_nLeaveSitoutedDelay	= 30;
		pContr->m_nLeaveZeroBalancedSeatDelay	= 10;
		}
		
	pInfo->m_btRegularActTimeSec= (BYTE)pContr->m_nRegularTimeSec;
	pInfo->m_btExtraActTimeSec	= (BYTE)pContr->m_nExtraTimeSec;
	pContr->m_gametype			= (GameType)pTable->m_nGameType;
	pContr->m_subType			= (GameSubType)pTable->m_nGameSubType;
	pContr->m_moneyType			= (MoneyType)pTable->m_nMoneyType;

	for(int j=0; j<pInfo->m_cMaxPlayersCt; j++){
		pInfo->m_listSeatAvatars.Add(NULL);
		pInfo->m_seats			.Add(NULL);
		}

	m_lockGameTables.Lock();	// Lock
	m_listGameTable.Add(pTable);
	m_lockGameTables.Unlock();	// Unlock

	ppTableController = pContr;
	return pTable->m_nTableId;
	}

bool
PokerCashData::GetGameTable(int nTableId, PokerGameTable* pGameTable){
	if( !pGameTable ) return false;
	bool bRet = false;
	m_lockGameTables.Lock();	// Lock
	PokerGameTable* pGameTableExist = m_listGameTable.FindOneVariable(_T("TableId"), &nTableId);
	if( pGameTableExist ){
		pGameTableExist->Copy(pGameTable);
		bRet = true;
		}
	m_lockGameTables.Unlock();	// Unlock
	return bRet;
	}

bool
PokerCashData::UnseatUsers(){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	UnseatUsersProc proc;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		return true;
		}
	return false;
	}

bool
PokerCashData::SeatUser(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	SeatUserProc proc;
	proc.m_nTableId			= nTableId;
	proc.m_nUserId			= nUserId;
	proc.m_dAmount			= (double)(nAmount / 100.0);
	proc.m_nCurrencyId		= nCurrencyId;
	proc.m_dBalanceNew		= 0.0;
	nBalanceNew				= 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerCashData::UnseatUser(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	UnseatUserProc proc;
	proc.m_nTableId			= nTableId;
	proc.m_nUserId			= nUserId;
	proc.m_dAmount			= (double)(nAmount / 100.0);
	proc.m_nCurrencyId		= nCurrencyId;
	proc.m_dBalanceNew		= 0.0;
	nBalanceNew				= 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerCashData::SeatUserPlaymoney(int nUserId, int nTableId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	SeatUserPlaymoneyProc proc;
	proc.m_nTableId			= nTableId;
	proc.m_nUserId			= nUserId;
	proc.m_dAmount			= (double)(nAmount / 100.0);
	proc.m_dBalanceNew		= 0.0;
	nBalanceNew				= 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerCashData::UnseatUserPlaymoney(int nUserId, int nTableId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	UnseatUserPleymoneyProc proc;
	proc.m_nTableId			= nTableId;
	proc.m_nUserId			= nUserId;
	proc.m_dAmount			= (double)(nAmount / 100.0);
	proc.m_dBalanceNew		= 0.0;
	nBalanceNew				= 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerCashData::UserAddChips(int nUserId, int nTableId,  int nSeat, INT_CHIPS nAmount, int nCurrencyId,  INT_CHIPS& nBalanceNew){
	if( nUserId <= 0 || nAmount <= 0 )
		return false;
	UserAddChipsProc proc;
	proc.m_nTableId			= nTableId;
	proc.m_nUserId			= nUserId;
	proc.m_dAmount			= (double)(nAmount / 100.0);
	proc.m_nCurrencyId		= nCurrencyId;
	proc.m_dBalanceNew		= 0.0;
	nBalanceNew				= 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerCashData::UserAddChipsPlaymoney(int nUserId, int nTableId,  int nSeat, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew){
	if( nUserId <= 0 || nAmount <= 0 )
		return false;
	UserAddChipsPlaymoneyProc proc;
	proc.m_nTableId			= nTableId;
	proc.m_nUserId			= nUserId;
	proc.m_dAmount			= (double)(nAmount / 100.0);
	proc.m_dBalanceNew		= 0.0;
	nBalanceNew				= 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

__int64
PokerCashData::StartHand(int nTableId, int nPlayersCt){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return 0;

	PokerGameTable*	pTable = (PokerGameTable*)m_listGameTable.FindOneVariable(_T("TableId"), &nTableId);
	if( !pTable ) return 0;

	StartHandProc proc;
	proc.m_nTableId				= nTableId;
	proc.m_nPlayersCt			= nPlayersCt;
	proc.m_nSeatCt				= (int)pTable->m_shMaxPlayerCt;
	proc.m_dBigBlindAmount		= pTable->m_dBigBlind;
	proc.m_dSmallBlindAmount	= pTable->m_dSmallBlind;
	proc.m_dAnteAmount			= pTable->m_dAnte;
	proc.m_dRakePercent			= pTable->m_dRakePercent;
	if( m_pDataMan->ExecuteProc(&proc) ){
		return proc.m_nHandId;
		}
	return 0;
	}

bool
PokerCashData::GetSeatMinBuyIn(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS& nAmount, INT_CHIPS& nBalance){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	RetrieveMinSeatBuyInProc proc;
	proc.m_nTableId			= nTableId;
	proc.m_nUserId			= nUserId;
	proc.m_nCurrencyId		= nCurrencyId;
	proc.m_dAmount			= 0.0;
	proc.m_dBalance			= 0.0;

	nAmount		= 0;
	nBalance	= 0;

	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nAmount = ConvertMoney(proc.m_dAmount);
		nBalance = ConvertMoney(proc.m_dBalance);
		return true;
		}
	return false;
	}

bool
PokerCashData::FinishHand(__int64 nHandId, int nTableId, int nCurrencyId, INT_CHIPS nTotalPotAmount, UINT nRakeAmount, _String& sTableCards, _String& sPlayersInfo, void* lpHandData, int nSize){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	FinishHandProc proc;
	proc.m_bFinishStatus	= false;
	proc.m_nHandId			= nHandId;
	proc.m_nTableId			= nTableId;
	proc.m_nCurrencyId		= nCurrencyId;
	proc.m_sPlayersInfo		= sPlayersInfo;
	proc.m_sTableCards		= sTableCards;
	proc.m_dTotalPotAmount	= (double)(nTotalPotAmount / 100.0);
	proc.m_dRakeAmount		= (double)(nRakeAmount / 100.0);

	// Allocate binary memory and copy.
	proc.m_binHandData.SetBinarySize(nSize, lpHandData);

	if( m_pDataMan->ExecuteProc(&proc) )
		return proc.m_bFinishStatus;
	return false;
	}

bool
PokerCashData::FinishHandPlaymoney(__int64 nHandId, int nTableId, INT_CHIPS nTotalPotAmount, UINT nRakeAmount, _String& sTableCards, _String& sPlayersInfo){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	FinishHandPlaymoneyProc proc;
	proc.m_bFinishStatus	= false;
	proc.m_nHandId			= nHandId;
	proc.m_nTableId			= nTableId;
	proc.m_sPlayersInfo		= sPlayersInfo;
	proc.m_sTableCards		= sTableCards;
	proc.m_dTotalPotAmount	= (double)(nTotalPotAmount / 100.0);
	proc.m_dRakeAmount		= (double)(nRakeAmount / 100.0);

	if( m_pDataMan->ExecuteProc(&proc) )
		return proc.m_bFinishStatus;
	return false;
	}

bool
PokerCashData::UpdateGameTableInfo(PokerTableInfo* pInfo){
	if( !pInfo ) return false;
	/*
	int nTableId = pInfo->m_nTableId;
	PokerGameTable*	pTable = (PokerGameTable*)m_listGameTable.FindOneVariable(_T("TableId"), &nTableId);
	if( !pTable ) return 0;

	// Statistic info.
	pTable->m_nPlayedHandsCt = pInfo->m_nPlayedHandsCt;

	CStringArray arrDirtyFields;
	arrDirtyFields.Add(_T("PLAYED_HANDS_COUNT"));
	bool bRet = m_pDataMan->SaveRecord(pTable, &arrDirtyFields);
	return bRet;*/
	return true;
	}

bool
PokerCashData::GetTableStat(int nUserId, int nTableId, int& nPlayedHandsCt, int& nWonHandsCt, INT_CHIPS& nSumBetAmount, INT_CHIPS& nSumWonAmount, INT_CHIPS& nSumAmountIn, INT_CHIPS& nSumAmountOut){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	GetTableStatProc proc;
	proc.m_nUserId			= nUserId;
	proc.m_nTableId			= nTableId;
	if( m_pDataMan->ExecuteProc(&proc) ){
		nSumBetAmount	= ConvertMoney(proc.m_dSumBetAmount);
		nSumAmountOut	= ConvertMoney(proc.m_dSumAmountOut);
		nSumAmountIn	= ConvertMoney(proc.m_dSumAmountIn);
		nSumWonAmount	= ConvertMoney(proc.m_dSumWonAmount);
		nPlayedHandsCt	= proc.m_nHandsPlayed;
		nWonHandsCt		= proc.m_nHandsWon;
		return true;
		}
	return false;
	}

int
PokerCashData::GetGameTableInfoObjects(int nGameType, int nGameSubType, EnumerableObject<PokerGameTableInfoObject>* pListObjects){
	if( !pListObjects ) return 0;
	m_lockGameTables.Lock(); // Lock
	int nLoop = 0, nCt = m_listGameTable.GetCount(), nResultCt = 0;
	while(nLoop < nCt){
		PokerGameTable* pTable = m_listGameTable.GetAt(nLoop);
		if( pTable && pTable->m_nGameType == nGameType && (pTable->m_nGameSubType&nGameSubType) == pTable->m_nGameSubType ){
			PokerGameTableInfoObject* pCopyInto	= new PokerGameTableInfoObject;
			pCopyInto->m_nId					= pTable->m_nTableId;
			pCopyInto->m_sName					= pTable->m_sName;
			pCopyInto->m_nGameType				= pTable->m_nGameType;
			pCopyInto->m_nGameSubType			= pTable->m_nGameSubType;
			pCopyInto->m_nCurrencyId			= pTable->m_nCurrencyId;
			pCopyInto->m_fSmallBlind			= (float)pTable->m_dSmallBlind;
			pCopyInto->m_fBigBlind				= (float)pTable->m_dBigBlind;
			pCopyInto->m_fAnte					= (float)pTable->m_dAnte;
			pCopyInto->m_fRakePercent			= (float)pTable->m_dRakePercent;
			pCopyInto->m_fMaxRakeAmount			= (float)pTable->m_dRakeAmountMax;
			pCopyInto->m_fMinBuyin				= (float)pTable->m_dMinBuyin;
			pCopyInto->m_fMaxBuyin				= (float)pTable->m_dMaxBuyin;
			pCopyInto->m_fMinBet				= 0.0f;//(float)pTable->m_dMinBet;
			pCopyInto->m_fMaxBet				= 0.0f;//(float)pTable->m_dMaxBet;
			pCopyInto->m_cMaxSeatCt				= (char)pTable->m_shMaxPlayerCt;
			pCopyInto->m_sPassword				= pTable->m_sPwd;
			pCopyInto->m_cPlayersCt				= 0;
			pCopyInto->m_cWaitingPlayersCt		= 0;
			pCopyInto->m_fAvgPotAmount			= 0.0f;
			pCopyInto->m_shHandPerHour			= 0;
			pCopyInto->m_bActiveStatus			= true;
			pListObjects->Add(pCopyInto);
			nResultCt ++;
			}
		nLoop ++;
		}
	m_lockGameTables.Unlock(); // Unlock
	return nResultCt;
	}