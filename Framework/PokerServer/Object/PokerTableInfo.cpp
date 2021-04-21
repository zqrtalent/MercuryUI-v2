// PokerTableInfo.cpp : implementation file
//

#include "stdafx.h"
#include "../../Array/AutoSortedArray.h"
#include "../../UI/Poker/PokerGUI.h"
#include "../../Protocol/_PokerDefines.h"
#include "PokerTableSeatInfo.h"
#include "PokerTableInfo.h"
#include "../../Protocol/PokerPacket.h"
#include <time.h>

#ifdef _SERVER
#include "../HandWinnerCalculator.h"
#include "../OmahaWinnerCalculator.h"
#include "../OmahaHiLoWinnerCalculator.h"
#include "../HoldemWinnerCalculator.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerTableInfo::PokerTableInfo(){
	m_nTableId				= 0;	
	m_nTournamentId			= 0;
	m_bTournamentTable		= false;
	
#ifdef _SERVER
	ClearStatInfo();
	m_nBetRaiseCtPerRound	= 0;
	m_nSeatRefusedToPaySB	= -1;
#endif

	m_cMaxPlayersCt			= 0;
	m_nTotalPotAmount		= 0;	
	m_nMainPotAmount		= 0;
	m_nRakeAmount			= 0;
	m_cSBSeat				= -1;
	m_cBBSeat				= -1;
	m_cActiveSeat			= -1;	
	m_cDealerSeat			= -1;	
	m_cCard1				= -1;	
	m_cCard2				= -1;	
	m_cCard3				= -1;	
	m_cCard4				= -1;	
	m_cCard5				= -1;	
	m_gameStatus			= PokerGameStatus::NotStarted;	
	m_round					= PokerRoundType::No;

	m_btRegularActTimeSec	= 0;
	m_btExtraActTimeSec		= 0;

	m_nMaxChatMessageCt		= 5;

	m_nSmallBlindAmount		= 0;
	m_nBigBlindAmount		= 0;
	m_nAnteAmount			= 0;

	m_nMinBuyIn				= 0;
	m_nMaxBuyIn				= 0;

	m_shTimeBankSec			= 0;

	m_cGameTypeId			= 0;
	m_shSubTypeId			= 0;
	m_cMoneyTypeId			= 0;
	m_cCurrencyId			= 0;
	m_cGameSpeedId			= 0;
	m_fRakePercent			= 0.0f;
	m_nRakeAmountMax		= 0;
	m_nFineAmount			= 0;

	m_bCardsShown			= false;

	m_nTableCard1			= -1;
	m_nTableCard2			= -1;
	m_nTableCard3			= -1;
	m_nTableCard4			= -1;
	m_nTableCard5			= -1;
	}

BEGIN_DECLARE_VAR(PokerTableInfo, Serializable)
	DECLARE_VAR(_T("PokerTableInfo"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),			_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TableName"),		_T(""),		VariableType::VT_String, offsetof(PokerTableInfo,m_sTableName), false, false, false)
	DECLARE_VAR(_T("TournamentId"),		_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TournamentTable"),	_T(""),		VariableType::VT_Bool, offsetof(PokerTableInfo,m_bTournamentTable), false, false, false)
	DECLARE_VAR(_T("MaxPlayersCt"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cMaxPlayersCt), false, false, false)
	DECLARE_VAR(_T("Seats"),			_T(""),		VariableType::VT_None, offsetof(PokerTableInfo,m_seats), false, true, true)
	DECLARE_VAR(_T("SeatAvatars"),		_T(""),		VariableType::VT_None, offsetof(PokerTableInfo,m_listSeatAvatars), false, true, true)
	DECLARE_VAR(_T("TotalPotAmount"),	_T(""),		VariableType::VT_Chips, offsetof(PokerTableInfo,m_nTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("MainPotAmount"),	_T(""),		VariableType::VT_Chips, offsetof(PokerTableInfo,m_nMainPotAmount), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),		_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nRakeAmount), false, false, false)
	DECLARE_VAR(_T("TimeBankSec"),		_T(""),		VariableType::VT_Int16, offsetof(PokerTableInfo,m_shTimeBankSec), false, false, false)
	DECLARE_VAR(_T("ActiveSeat"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cActiveSeat), false, false, false)
	DECLARE_VAR(_T("DealerSeat"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cDealerSeat), false, false, false)
	DECLARE_VAR(_T("SBSeat"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cSBSeat), false, false, false)
	DECLARE_VAR(_T("BBSeat"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cBBSeat), false, false, false)
	DECLARE_VAR(_T("RegularActTimeSec"),_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_btRegularActTimeSec), false, false, false)
	DECLARE_VAR(_T("ExtraActTimeSec"),	_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_btExtraActTimeSec), false, false, false)
	DECLARE_VAR(_T("Card1"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cCard1), false, false, false)
	DECLARE_VAR(_T("Card2"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cCard2), false, false, false)
	DECLARE_VAR(_T("Card3"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cCard3), false, false, false)
	DECLARE_VAR(_T("Card4"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cCard4), false, false, false)
	DECLARE_VAR(_T("Card5"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cCard5), false, false, false)
	DECLARE_VAR(_T("gameStatus"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_gameStatus), false, false, false)
	DECLARE_VAR(_T("round"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_round), false, false, false)
	DECLARE_VAR(_T("SmallBlindAmount"), _T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nSmallBlindAmount), false, false, false)
	DECLARE_VAR(_T("BigBlindAmount"),	_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nBigBlindAmount), false, false, false)
	DECLARE_VAR(_T("AnteAmount"),		_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nAnteAmount), false, false, false)
	DECLARE_VAR(_T("MinBuyIn"),			_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nMinBuyIn), false, false, false)
	DECLARE_VAR(_T("MaxBuyIn"),			_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nMaxBuyIn), false, false, false)
	DECLARE_VAR(_T("GameTypeId"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cGameTypeId), false, false, false)
	DECLARE_VAR(_T("SubTypeId"),		_T(""),		VariableType::VT_Int16, offsetof(PokerTableInfo,m_shSubTypeId), false, false, false)
	DECLARE_VAR(_T("MoneyTypeId"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cMoneyTypeId), false, false, false)
	DECLARE_VAR(_T("GameSpeedId"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cGameSpeedId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cCurrencyId), false, false, false)
	DECLARE_VAR(_T("listPots"),			_T(""),		VariableType::VT_None, offsetof(PokerTableInfo,m_listPots), false, true, true)
	DECLARE_VAR(_T("ChatItems"),		_T(""),		VariableType::VT_None, offsetof(PokerTableInfo,m_listChatItems), false, true, true)
END_DECLARE_VAR()

PokerTableInfo::~PokerTableInfo(){
	// Don't destroy PokerAvatar objects.
	m_listSeatAvatars.RemoveAll(false);
	}

bool
PokerTableInfo::Copy(Serializable* pInto){
	bool bRet = Serializable::Copy(pInto);
	if( bRet ){
		PokerTableInfo* pTableInfoInto = (PokerTableInfo*)pInto;
		for(int i=0; i<m_cMaxPlayersCt; i++){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(i);
			PokerTableSeatInfo* pSeatInto = pTableInfoInto->GetSeatInfo(i);
			if( pSeat && pSeatInto ){
#ifdef _SERVER
				// Copy session from one to another.
				pSeatInto->SetSession(pSeat->GetSession(false), pSeat->m_nTableId, pSeat->m_nTournamentId);
#endif
				// Copy offline flag.
				pSeatInto->SetOffline(pSeat->IsOffline());
				}
			}
		}
	return bRet;
	}


#ifdef _SERVER // Server side functionality.
PokerTableSeatInfo*
PokerTableInfo::GetSeatByConnection(ClientConnection* pConn){
	if( !pConn ) return NULL;
	int nLoop = 0, nRet	= 0;
	while( nLoop < m_seats.GetCount() ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && pSeat->GetConnection() == pConn ){
			return pSeat;
			}
		nLoop ++;
		}
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetSeatBySession(Session* pSess){
	if( !pSess ) return NULL;
	int nLoop = 0, nRet = 0;
	while( nLoop < m_seats.GetCount() ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && pSeat->GetSession(false) == pSess ){
			return pSeat;
			}
		nLoop ++;
		}
	return NULL;
	}

int
PokerTableInfo::EmptyOfflineSeats(bool bEmpty, CDWordArray* pArrSeatIndexes){
	int nLoop = 0, nRet = 0;
	while( nLoop < m_seats.GetCount() ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && (pSeat->IsOffline() || pSeat->GetSession(false) == NULL) ){
			if( bEmpty )
				m_seats.SetAt(nLoop, NULL); // Empty seat.
			if( pArrSeatIndexes )
				pArrSeatIndexes->Add(nLoop);
			nRet ++;
			}
		nLoop ++;
		}
	return nRet;
	}

void
PokerTableInfo::ClearStatInfo(){
	m_nPlayedHandsCt		= 0;
	m_dAvgPotAmount			= 0.0;
	m_dAvgPlayers			= 0.0;
	m_dAvgHandsPerHour		= 0.0;
	m_dPlayerFlop			= 0.0;
	m_nAvgHandDurationSec	= 0;
	}

int
PokerTableInfo::GetSidePots(AutoSortedArray& arrPotAmountBySeatIndex, INT_CHIPS& nMaxSeatPot){
	int nLoop = 0, nCt = m_cMaxPlayersCt, nSidePotCt = 0;
	arrPotAmountBySeatIndex.DeleteAll();
	nMaxSeatPot	= 0;

	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat ){
			nLoop ++;
			continue;
			}

		PokerSeatStatus status		= pSeat->GetStatus();
		INT_CHIPS		nTotalPot	= pSeat->GetTotalPot();
		if( status == PokerSeatStatus::All_In ){
			if( arrPotAmountBySeatIndex.IndexOf((void*)nTotalPot) == -1 )
				nSidePotCt ++;
			arrPotAmountBySeatIndex.Add((void*)nTotalPot, (void*)nLoop);
			}

		if( IsInPlayByStatus(nLoop, status) )
			nMaxSeatPot = max(nMaxSeatPot, nTotalPot);
		nLoop ++;
		}
	return nSidePotCt;
	}

int
PokerTableInfo::NormalizeSeatPots(EnumerableObject<PokerSeatAmountInfo>* pSeatsAmountChanged){
	int nLoop = 0, nCt = m_cMaxPlayersCt, nSidePotCt = 0;
	INT_CHIPS nMaxSeatPot = 0, nFoldedSeatsPot = 0, nFoldedSeatPotMax = 0;
	AutoSortedArrayInt64 arrPotAmountBySeatIndex;
	arrPotAmountBySeatIndex.SetCompareFunction(AutoSortedArrayInt64::CompareDesc);

	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat ){
			nLoop ++;
			continue;
			}

		PokerSeatStatus status		= pSeat->GetStatus();
		INT_CHIPS		nPotAmount	= pSeat->GetPotAmount();
		if( nPotAmount > 0 ){
			if( IsInPlayByStatus(nLoop, status) ){
				arrPotAmountBySeatIndex.Add((__int64)nPotAmount, (__int64)nLoop);
				nMaxSeatPot		= max(nMaxSeatPot, nPotAmount);
				}
			else{
				nFoldedSeatPotMax	= max(nFoldedSeatPotMax, nPotAmount);
				nFoldedSeatsPot		+= nPotAmount;
				}
			}
		nLoop ++;
		}

	if( !arrPotAmountBySeatIndex.GetCount() )
		return 0;

	if( arrPotAmountBySeatIndex.GetCount() > 1 ){
		INT_CHIPS nAmountDifference = arrPotAmountBySeatIndex.Get(0) - arrPotAmountBySeatIndex.Get(1);
		if( nAmountDifference > 0 ){
			int nSeat					= (int)arrPotAmountBySeatIndex.GetData(0);
			INT_CHIPS nTotalPotNew		= m_nTotalPotAmount - nAmountDifference;
			INT_CHIPS nSeatTotalPotNew	= GetSeatTotalPot(nSeat) - nAmountDifference;
			INT_CHIPS nPotAmountNew		= GetPotAmount(nSeat) - nAmountDifference;
			INT_CHIPS nBalanceNew		= GetBalance(nSeat) + nAmountDifference;

			// Change total pot amount.
			SetTotalPotAmount	(nTotalPotNew);
			// Change seat pot amount.
			SetPotAmount		(nSeat, nPotAmountNew);
			// Change seat balance amount.
			SetBalance			(nSeat, nBalanceNew);
			// Set seat total pot amount.
			SetSeatTotalPot		(nSeat, nSeatTotalPotNew);
			
			PokerSeatAmountInfo* pSeatAmount = new PokerSeatAmountInfo();
			pSeatAmount->m_cSeat			= (char)nSeat;
			pSeatAmount->m_nBalance			= nBalanceNew;
			pSeatAmount->m_nPotAmount		= nPotAmountNew;
			pSeatAmount->m_nTotalPotAmount	= nSeatTotalPotNew;
			pSeatsAmountChanged->Add(pSeatAmount);
			return 1;
			}
		}
	else{
		int nSeat					= (int)arrPotAmountBySeatIndex.GetData(0);
		INT_CHIPS nAmountDifference	= ((INT_CHIPS)arrPotAmountBySeatIndex.Get(0)) - nFoldedSeatPotMax;
		INT_CHIPS nTotalPotNew		= m_nTotalPotAmount - nAmountDifference;
		INT_CHIPS nSeatTotalPotNew	= GetSeatTotalPot(nSeat) - nAmountDifference;
		INT_CHIPS nPotAmountNew		= GetPotAmount(nSeat) - nAmountDifference;
		INT_CHIPS nBalanceNew		= GetBalance(nSeat) + nAmountDifference;

		// Change total pot amount.
		SetTotalPotAmount	(nTotalPotNew);
		// Change seat pot amount.
		SetPotAmount		(nSeat, nPotAmountNew);
		// Change seat balance amount.
		SetBalance			(nSeat, nBalanceNew);
		// Set seat total pot amount.
		SetSeatTotalPot		(nSeat, nSeatTotalPotNew);
			
		PokerSeatAmountInfo* pSeatAmount = new PokerSeatAmountInfo();
		pSeatAmount->m_cSeat			= (char)nSeat;
		pSeatAmount->m_nBalance			= nBalanceNew;
		pSeatAmount->m_nPotAmount		= nPotAmountNew;
		pSeatAmount->m_nTotalPotAmount	= nSeatTotalPotNew;
		pSeatsAmountChanged->Add(pSeatAmount);
		return 1;
		}
	return 0;
	}

int
PokerTableInfo::GetDetailedPots(EnumerableObject<PokerPotAmountDetail>* pListPost){
	List<SidePotInfoStruct> listSidePots;
	CalculateSidePots(listSidePots);
	if( !listSidePots.GetCount() )
		return 0;

	int nPotIndex = 0;
	pListPost->RemoveAll(true);
	for(int i=0; i<listSidePots.GetCount(); i++){
		SidePotInfoStruct* pInfo = listSidePots[i];
		if( !pInfo ) continue;

		PokerPotAmountDetail* pDetail	= new PokerPotAmountDetail();
		pDetail->m_cPotIndex			= (char)nPotIndex;
		pDetail->m_nAmount				= pInfo->nPotAmount;
		pDetail->m_wRakeAmount			= (UINT)CalculateRakeAmount(pDetail->m_nAmount);
		pDetail->m_cPlayersCt			= (char)pInfo->nSeatsCtBelongsTo;
		pListPost->Add(pDetail);
		nPotIndex ++;
		}
	return pListPost->GetCount();
	}

HandWinnerCalculator*
PokerTableInfo::GetHandWinnerCalculator(){
	switch( m_cGameTypeId ){
		case GameType::Omaha:{
				if( (m_shSubTypeId&GameSubType::Omaha_Omaha) == GameSubType::Omaha_Omaha )
					return OmahaWinnerCalculator::GetInstance();
				
				if( (m_shSubTypeId&GameSubType::Omaha_OmahaHiLo) == GameSubType::Omaha_OmahaHiLo )
					return OmahaHiLoWinnerCalculator::GetInstance();
				
				break;
				}
		case GameType::TexasHoldem:{
				return HoldemWinnerCalculator::GetInstance();
				}
		};
	return NULL;
	}

int
PokerTableInfo::CalculateSidePots(List<SidePotInfoStruct>& listSidePots){
	if( m_nMainPotAmount <= 0 ){
		listSidePots.RemoveAll(true);
		return 0;
		}

	AutoSortedArrayInt64 arrSeatTotalPots, arrFoldedSeatAmounts;
	INT_CHIPS		nAmountFoldedSeats  = 0;
	arrFoldedSeatAmounts.SetCompareFunction(AutoSortedArrayInt64::CompareEmpty);

	for(int i=0; i<m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_seats[i];
		if( !pSeat ) continue;

		if( pSeat->GetTotalPot() > 0 ){ 
			PokerSeatStatus status = pSeat->GetStatus();
			if( IsInPlayByStatus(i, status) )
				arrSeatTotalPots.Add((__int64)pSeat->GetTotalPot(), (__int64)pSeat->GetSeat());
			if( status == PokerSeatStatus::Folded ){
				arrFoldedSeatAmounts.Add((__int64)pSeat->GetTotalPot(), (__int64)pSeat->GetSeat());
				nAmountFoldedSeats	+= pSeat->GetTotalPot();
				}
			}
		}

	listSidePots.RemoveAll(true);
	if( arrSeatTotalPots.GetCount() > 0 ){
		INT_CHIPS	nPrevSeatPotAmount	= 0;
		for(UINT i=0; i<arrSeatTotalPots.GetCount(); i++){
			INT_CHIPS nSeatTotalPotAmount	= ((INT_CHIPS)arrSeatTotalPots.Get(i)) - nPrevSeatPotAmount;
			// Add new side pot.
			if( nSeatTotalPotAmount > 0 ){
				int nSeatCtBelongsTo		= arrSeatTotalPots.GetCount() - i;
				INT_CHIPS nSidePotAmount	= nSeatTotalPotAmount * nSeatCtBelongsTo;

				// Include folded amounts into pot. {{
				if( nAmountFoldedSeats > 0 ){
					for(UINT loop=0; loop<arrFoldedSeatAmounts.GetCount(); loop++){
						INT_CHIPS nAmount = (INT_CHIPS)arrFoldedSeatAmounts[loop];
						if( !nAmount )	continue;
						if( nAmount > nSeatTotalPotAmount ){
							nSidePotAmount		+= nSeatTotalPotAmount;
							nAmountFoldedSeats	-= nSeatTotalPotAmount;
							nAmount				-= nSeatTotalPotAmount;
							}
						else{
							nSidePotAmount		+= nAmount;
							nAmountFoldedSeats	-= nAmount;
							nAmount				= 0;
							}
						arrFoldedSeatAmounts.Set(loop, (__int64)nAmount, arrFoldedSeatAmounts.GetData(loop));
						}
					}
				// }}

				// Add fine amount to the first pot.
				if( listSidePots.GetCount() == 0 )
					nSidePotAmount += GetFineAmount();

				SidePotInfoStruct* pPotInfo		= new SidePotInfoStruct;
				pPotInfo->nPotAmount			= nSidePotAmount;
				pPotInfo->nSeatsCtBelongsTo		= nSeatCtBelongsTo;
				listSidePots.Add(pPotInfo);

				// Copy pot belonging seat indexes.
				int nSeatCt = 0;
				for(UINT j=i; j<arrSeatTotalPots.GetCount(); j++){
					pPotInfo->nArrSeatsBelongsTo[nSeatCt] = (int)arrSeatTotalPots.GetData(j); // Seat index.
					nSeatCt ++;
					}
				}

			nPrevSeatPotAmount = (UINT)arrSeatTotalPots.Get(i);
			}
		}
	return listSidePots.GetCount();
	}

int
PokerTableInfo::GetSeatSidePots(List<SidePotInfoStruct>& listSidePots, int nSeat, AutoSortedArray& arrSidePotAmountByPotIndex){
	for(int i=0; i<listSidePots.GetCount(); i++){
		SidePotInfoStruct* pPotInfo = listSidePots[i];
		if( !pPotInfo ) continue;

		for(int j=0; j<pPotInfo->nSeatsCtBelongsTo; j++){
			if( nSeat == pPotInfo->nArrSeatsBelongsTo[j] ){
				arrSidePotAmountByPotIndex.Add((void*)i, (void*)(pPotInfo->nPotAmount / pPotInfo->nSeatsCtBelongsTo));
				break;
				}
			}
		}
	return arrSidePotAmountByPotIndex.GetCount();
	}

int
PokerTableInfo::GetSeatsHandCards(EnumerableObject<PokerHandCards>* pListHandCards, CDWordArray* pArrAskShowMuckCards, AutoSortedArray* pArrWinnerSeats){
	if( !pListHandCards ) return 0;

	int						nLoop				= 0;
	int						nCt					= m_seats.GetCount();
	int						nRet				= 0;
	int						nFirstOpenCards		= -1;
	PokerHandCardsClass		handCards;
	bool					bArrAskShowMuck[]	= {false, false, false, false, false, false, false, false, false, false};
	HandWinnerCalculator*	pCalc				= GetHandWinnerCalculator();
	if( !pCalc ){
		return false;
		}

	CDWordArray	arrCardsIdx;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && IsInPlayByStatus(nLoop, pSeat->GetStatus())  /*(pSeat->GetStatus() == PokerSeatStatus::All_In || pSeat->GetStatus() == PokerSeatStatus::InPlay)*/ ){
			PokerHandCards* pHandCards		= new PokerHandCards();
			pHandCards->m_btSeat			= (BYTE)nLoop;
			pHandCards->m_btCard1			= (BYTE)pSeat->GetCard1();
			pHandCards->m_btCard2			= (BYTE)pSeat->GetCard2();
			pHandCards->m_btCard3			= (BYTE)pSeat->GetCard3();
			pHandCards->m_btCard4			= (BYTE)pSeat->GetCard4();

			arrCardsIdx.RemoveAll();
			pCalc->GetBestHandCardValue(pSeat->GetCard1(), pSeat->GetCard2(), pSeat->GetCard3(), pSeat->GetCard4(),
				m_cCard1, m_cCard2, m_cCard3, m_cCard4, m_cCard5, (PokerHandCardsClass&)handCards, arrCardsIdx);

			// Copy best combinations card indexes.((0, 1, 2, 3) - identifies hand cards (4, 5, 6, 7, 8) = identifies table cards)
			if( handCards != PokerHandCardsClass::CardsClass_None ){
				pHandCards->m_cBestCard1	= (char)arrCardsIdx[0];
				pHandCards->m_cBestCard2	= (char)arrCardsIdx[1];
				pHandCards->m_cBestCard3	= (char)arrCardsIdx[2];
				pHandCards->m_cBestCard4	= (char)arrCardsIdx[3];
				pHandCards->m_cBestCard5	= (char)arrCardsIdx[4];
				}

			if( pSeat->m_bCardsShown ){
				pSeat->SetHandCardsClass((int)handCards);
				}

			if( pSeat->GetLastAction() == PokerActionTypes::Bet )
				nFirstOpenCards = nLoop;

			if( !pSeat->GetAutoMuck() )
				bArrAskShowMuck[nLoop] = true;
			else
				bArrAskShowMuck[nLoop] = (pArrWinnerSeats && pArrWinnerSeats->IndexOf((void*)nLoop) != -1);

			pHandCards->m_cBestCardsClass = (char)handCards;
			pListHandCards->Add(pHandCards);
			nRet ++;
			}
		nLoop ++;
		}

	// Create array of seat indexes, which indicates order of seat to ask. {{
	if( pArrAskShowMuckCards ){
		nLoop	= 0;
		nCt		= 10;

		if( nFirstOpenCards == -1 )
			nFirstOpenCards = (int)m_cSBSeat;

		int nAskSeat = nFirstOpenCards;
		while( nLoop < nCt ){
			nAskSeat = (nFirstOpenCards + nLoop)%10;
			if( bArrAskShowMuck[nAskSeat] ){
				pArrAskShowMuckCards->Add(nAskSeat);
				}
			nLoop ++;
			}
		}
	// }}
	return nRet;
	}

bool
PokerTableInfo::GetSeatHandCards(int nSeat, PokerHandCards* pHandCards){
	HandWinnerCalculator*	pCalc	= GetHandWinnerCalculator();
	PokerTableSeatInfo*		pSeat	= m_seats.GetAt(nSeat);

	if( pSeat && IsInPlayByStatus(nSeat, pSeat->GetStatus()) /*(pSeat->GetStatus() == PokerSeatStatus::All_In || pSeat->GetStatus() == PokerSeatStatus::InPlay)*/ ){
		pHandCards->m_btSeat		= (BYTE)nSeat;
		pHandCards->m_btCard1		= (BYTE)pSeat->GetCard1();
		pHandCards->m_btCard2		= (BYTE)pSeat->GetCard2();
		pHandCards->m_btCard3		= (BYTE)pSeat->GetCard3();
		pHandCards->m_btCard4		= (BYTE)pSeat->GetCard4();

		if( pCalc ){
			PokerHandCardsClass handCards;
			CDWordArray	arrCardsIdx;
			pCalc->GetBestHandCardValue(pSeat->GetCard1(), pSeat->GetCard2(), pSeat->GetCard3(), pSeat->GetCard4(),
				m_cCard1, m_cCard2, m_cCard3, m_cCard4, m_cCard5, (PokerHandCardsClass&)handCards, arrCardsIdx);

			// Copy best combinations card indexes.((0, 1, 2, 3) - identifies hand cards (4, 5, 6, 7, 8) = identifies table cards)
			if( handCards != PokerHandCardsClass::CardsClass_None ){
				pHandCards->m_cBestCard1	= (char)arrCardsIdx[0];
				pHandCards->m_cBestCard2	= (char)arrCardsIdx[1];
				pHandCards->m_cBestCard3	= (char)arrCardsIdx[2];
				pHandCards->m_cBestCard4	= (char)arrCardsIdx[3];
				pHandCards->m_cBestCard5	= (char)arrCardsIdx[4];
				}

			pHandCards->m_cBestCardsClass = (char)handCards;
			return true;
			}
		}
	return false;
	}

void
PokerTableInfo::OnSeatAction(int nSeat, PokerActionTypes actionType, INT_CHIPS nAmount){
	if( actionType == PokerActionTypes::Bet || actionType == PokerActionTypes::Raise )
		m_nBetRaiseCtPerRound ++;

	if( m_gameStatus != PokerGameStatus::NotStarted && !m_arrHandPlayersInfo[nSeat].sUserName.IsEmpty() ){
		PokerTableSeatInfo* pSeat					= m_seats.GetAt(nSeat);
		m_arrHandPlayersInfo[nSeat].sUserName		= pSeat->GetUserName();
		m_arrHandPlayersInfo[nSeat].nUserId			= pSeat->GetUserId();
		m_arrHandPlayersInfo[nSeat].btCard1			= (BYTE)pSeat->GetCard1();
		m_arrHandPlayersInfo[nSeat].btCard2			= (BYTE)pSeat->GetCard2();
		m_arrHandPlayersInfo[nSeat].btCard3			= (BYTE)pSeat->GetCard3();
		m_arrHandPlayersInfo[nSeat].btCard4			= (BYTE)pSeat->GetCard4();
		m_arrHandPlayersInfo[nSeat].nBalanceAmount	= pSeat->GetBalance();
		m_arrHandPlayersInfo[nSeat].nPotAmount		= pSeat->GetPotAmount();
		m_arrHandPlayersInfo[nSeat].nTotalPotAmount = pSeat->GetTotalPot();
		m_arrHandPlayersInfo[nSeat].status			= pSeat->GetStatus();
		}

	}

int
PokerTableInfo::GetSBAmount(){
	// Fixed-Limit
	GameSubType limit = (GameSubType)m_shSubTypeId;
	if( (int)(limit&GameSubType::Holdem_FixedLimit) == (int)GameSubType::Holdem_FixedLimit )
		return (m_nSmallBlindAmount == 1 ? m_nSmallBlindAmount : m_nSmallBlindAmount/2);
	return m_nSmallBlindAmount;
	}

int
PokerTableInfo::GetBBAmount(){
	// Fixed-Limit
	GameSubType limit = (GameSubType)m_shSubTypeId;
	if( (int)(limit&GameSubType::Holdem_FixedLimit) == (int)GameSubType::Holdem_FixedLimit )
		return (m_nBigBlindAmount == 1 ? m_nBigBlindAmount : m_nBigBlindAmount/2);
	return m_nBigBlindAmount;
	}

#endif

UINT
PokerTableInfo::CalculateRakeAmount(INT_CHIPS nTotalPotAmount){
	UINT nRakeAmount = 0;
	if( m_nTournamentId > 0 )
		return 0; // Tournament.
	if( m_fRakePercent > 0.0f && m_round > PokerRoundType::PreFlop ){
		nRakeAmount		= (UINT)(nTotalPotAmount*m_fRakePercent);
		nRakeAmount		= min(max(0, nRakeAmount), m_nRakeAmountMax);
		}
	return nRakeAmount;
	}

bool
PokerTableInfo::SetAvatarInfo(int nSeat, PokerAvatar* pAvatar){
	if( nSeat < 0 || nSeat >= m_cMaxPlayersCt || !pAvatar )
		return false;
	if( m_listSeatAvatars.GetCount() < m_cMaxPlayersCt ){
		for(int i=m_listSeatAvatars.GetCount(); i<m_cMaxPlayersCt; i++){
			m_listSeatAvatars.Add(NULL);
			}
		}

	PokerAvatar* pAvatarInfo		= new PokerAvatar();
	pAvatarInfo->m_nId				= pAvatar->m_nId;
	pAvatarInfo->m_nPrivateUserId	= pAvatar->m_nPrivateUserId;
	pAvatarInfo->m_recStatus		= 0;
	pAvatarInfo->m_sAvatarFileName	= pAvatar->m_sAvatarFileName;
	m_listSeatAvatars.SetAt(nSeat, pAvatarInfo, true);
	return true;
	}

PokerAvatar*
PokerTableInfo::GetAvatarInfo(int nSeat){
	if( nSeat < 0 || nSeat >= m_cMaxPlayersCt )
		return NULL;
	return m_listSeatAvatars.GetAt(nSeat);
	}

PokerSeatStatus
PokerTableInfo::GetSeatStatus(int nSeatIndex){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeatIndex);
	if( pSeat )
		return pSeat->GetStatus();
	return PokerSeatStatus::Empty;
	}

PokerTableSeatInfo*
PokerTableInfo::GetSeatByUserId(int nUserId){
	int nLoop = 0, nRet = 0;
	while( nLoop < m_seats.GetCount() ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && pSeat->GetUserId() == nUserId ){
			return pSeat;
			}
		nLoop ++;
		}
	return NULL;
	}

int
PokerTableInfo::GetSittedCt(){
	int nLoop	= 0, nCt = m_seats.GetCount(), nSitted	= 0;
	if( m_bTournamentTable ){ // Tournament table.
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( pSeat && (pSeat->GetStatus() == PokerSeatStatus::Sitted || pSeat->GetStatus() == PokerSeatStatus::SittedOut) )
				nSitted ++;
			nLoop ++;
			}
		}
	else{	// Cash table.
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( pSeat && pSeat->GetStatus() == PokerSeatStatus::Sitted )
				nSitted ++;
			nLoop ++;
			}
		}
	return nSitted;
	}

PokerTableSeatInfo*
PokerTableInfo::GetNextSeatInfo(int nSeatIndex, int* pSeatIndex /*= NULL*/){
	int nCount = m_seats.GetCount();
	int nSeat = nSeatIndex + 1;
	if( nSeat >= nCount )
		nSeat = 0;

	for(int i=nSeat; i<nCount; i++ ){
		if( m_seats.GetAt(i) != NULL ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return m_seats.GetAt(i);
			}
		}

	if( nSeat > 1 ){
		nSeat --;
		for(int i=0; i<nSeat; i++ ){
			if( m_seats.GetAt(i) != NULL ){
				if( pSeatIndex )
					*pSeatIndex = i;
				return m_seats.GetAt(i);
				}
			}
		}
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetPrevSeatInfo(int nSeatIndex, int* pSeatIndex /*= NULL*/){
	int nCount = m_seats.GetCount();
	int nSeat = nSeatIndex - 1;
	if( nSeat < 0 )
		nSeat = nCount - 1;

	for(int i=nSeat; i>-1; i-- ){
		if( m_seats.GetAt(i) != NULL ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return m_seats.GetAt(i);
			}
		}

	if( nSeat > 1 ){
		nSeat --;
		for(int i=nCount-1; i>nSeat; i-- ){
			if( m_seats.GetAt(i) != NULL ){
				if( pSeatIndex )
					*pSeatIndex = i;
				return m_seats.GetAt(i);
				}
			}
		}

	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetNextSeatInfoByStatus(int nSeatIndex, PokerSeatStatus status, int* pSeatIndex /*= NULL*/){
	int nCount = m_seats.GetCount();
	int nSeat = nSeatIndex + 1;
	if( nSeat >= nCount )
		nSeat = 0;

	PokerTableSeatInfo* pSeat = NULL;
	for(int i=nSeat; i<nCount; i++ ){
		pSeat = m_seats.GetAt(i);
		if( pSeat != NULL && pSeat->GetStatus() == status ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return pSeat;
			}
		}

	if( nSeat > 1 ){
		nSeat --;
		for(int i=0; i<nSeat; i++ ){
			pSeat = m_seats.GetAt(i);
			if( pSeat != NULL && pSeat->GetStatus() == status ){
				if( pSeatIndex )
					*pSeatIndex = i;
				return pSeat;
				}
			}
		}
	return NULL;
	}


PokerTableSeatInfo*
PokerTableInfo::GetNextSitoutButInplaySeat(int nSeatIndex, int* pSeatIndex /*= NULL*/){
#ifdef _SERVER
	int nCount = m_seats.GetCount();
	int nSeat = nSeatIndex + 1;
	if( nSeat >= nCount )
		nSeat = 0;

	PokerTableSeatInfo* pSeat = NULL;
	for(int i=nSeat; i<nCount; i++ ){
		pSeat = m_seats.GetAt(i);
		if( pSeat != NULL && pSeat->GetStatus() == PokerSeatStatus::SittedOut && pSeat->GetSitoutButInPlayFlag() ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return pSeat;
			}
		}

	if( nSeat > 1 ){
		nSeat --;
		for(int i=0; i<nSeat; i++ ){
			pSeat = m_seats.GetAt(i);
			if( pSeat != NULL && pSeat->GetStatus() == PokerSeatStatus::SittedOut && pSeat->GetSitoutButInPlayFlag() ){
				if( pSeatIndex )
					*pSeatIndex = i;
				return pSeat;
				}
			}
		}
#endif
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetNextSeatedSeatInfo(int nSeatIndex, int* pSeatIndex /*= NULL*/){
	int nCount = m_seats.GetCount();
	int nSeat = nSeatIndex + 1;
	if( nSeat >= nCount )
		nSeat = 0;

	for(int i=nSeat; i<nCount; i++ ){
		if( m_seats.GetAt(i) != NULL ){
			PokerSeatStatus status = m_seats.GetAt(i)->GetStatus();
			if( status == PokerSeatStatus::Sitted || status == PokerSeatStatus::SittedOut ){
				if( pSeatIndex )
					*pSeatIndex = i;
				return m_seats.GetAt(i);
				}
			}
		}

	if( nSeat > 1 ){
		nSeat --;
		for(int i=0; i<nSeat; i++ ){
			if( m_seats.GetAt(i) != NULL ){
				PokerSeatStatus status = m_seats.GetAt(i)->GetStatus();
				if( status == PokerSeatStatus::Sitted || status == PokerSeatStatus::SittedOut ){
					if( pSeatIndex )
						*pSeatIndex = i;
					return m_seats.GetAt(i);
					}
				}
			}
		}
	return NULL;
	}

PokerTableSeatInfo*		
PokerTableInfo::GetPrevSeatInfoByStatus(int nSeatIndex, PokerSeatStatus status, int* pSeatIndex /*= NULL*/){
	int nCount = m_seats.GetCount();
	int nSeat = nSeatIndex - 1;
	if( nSeat < 0 )
		nSeat = nCount - 1;

	for(int i=nSeat; i>-1; i-- ){
		if( m_seats.GetAt(i) != NULL && m_seats.GetAt(i)->GetStatus() == status ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return m_seats.GetAt(i);
			}
		}

	if( nSeat > 1 ){
		nSeat --;
		for(int i=nCount-1; i>nSeat; i-- ){
			if( m_seats.GetAt(i) != NULL && m_seats.GetAt(i)->GetStatus() == status ){
				if( pSeatIndex )
					*pSeatIndex = i;
				return m_seats.GetAt(i);
				}
			}
		}
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetNextActiveSeat(int nSeatPrev, int& nSeatIndex, bool& bIsRoundEnded, bool& bOpenSeatCards){
#ifdef _SERVER
	if( m_gameStatus != PokerGameStatus::Started )
		return NULL;

	int nAllIn = 0, nFolded = 0, nInPlay = 0, nSitout = 0, nSitoutByInPlay = 0;
	int	nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat ){
			nLoop ++;
			continue;
			}

		if( pSeat->GetStatus() == PokerSeatStatus::All_In ){
			nAllIn ++;
			nLoop ++;
			continue;
			}
		if( pSeat->GetStatus() == PokerSeatStatus::Folded ){
			nFolded ++;
			nLoop ++;
			continue;
			}
		if( pSeat->GetStatus() == PokerSeatStatus::InPlay ){
			nInPlay ++;
			nLoop ++;
			continue;
			}
		if( pSeat->GetStatus() == PokerSeatStatus::SittedOut ){
			if( pSeat->GetSitoutButInPlayFlag() )
				nSitoutByInPlay ++;
			nSitout ++;
			nLoop ++;
			continue;
			}

		nLoop ++;
		}

	/* Old code.
	int nAllIn		= CountSeatsByStatus(PokerSeatStatus::All_In);
	int nFolded		= CountSeatsByStatus(PokerSeatStatus::Folded);
	int nInPlay		= CountSeatsByStatus(PokerSeatStatus::InPlay);
	*/
	bIsRoundEnded	= false;
	bOpenSeatCards	= false;

	if( nSeatPrev == -1 ){
		if( m_round == PokerRoundType::PreFlop )
			nSeatPrev = ((int)m_cBBSeat);
		else
			nSeatPrev = ((int)m_cDealerSeat);
		}

	PokerTableSeatInfo* pSeat = NULL;
	if( !m_bTournamentTable && !nSitoutByInPlay ){
		pSeat = GetNextSeatInfoByStatus(nSeatPrev, PokerSeatStatus::InPlay, &nSeatIndex);
		}
	// In tournament table we should also consider sitout seats as a inplay seats.
	else{
		//int nSitout = CountSeatsByStatus(PokerSeatStatus::SittedOut);
		nSitout		= (m_bTournamentTable ? nSitout : nSitoutByInPlay);
		nInPlay		+= nSitout;
		if( (nInPlay + nAllIn) > 1 && nSitout > 0 ){
			PokerTableSeatInfo* pSeat_SitOuted	= (m_bTournamentTable ? 
				GetNextSeatInfoByStatus(nSeatPrev, PokerSeatStatus::SittedOut, &nSeatIndex) :
				GetNextSitoutButInplaySeat(nSeatPrev, &nSeatIndex));
			pSeat = GetNextSeatInfoByStatus(nSeatPrev, PokerSeatStatus::InPlay, &nSeatIndex);

			if( pSeat_SitOuted ){
				if( !pSeat || (pSeat && IsBetweenSeats(pSeat_SitOuted->GetSeat(), nSeatPrev, pSeat->GetSeat())) ){
					pSeat		= pSeat_SitOuted;
					nSeatIndex	= pSeat_SitOuted->GetSeat();
					}
				}
			}
		else
			pSeat = GetNextSeatInfoByStatus(nSeatPrev, PokerSeatStatus::InPlay, &nSeatIndex);
		}

	if( pSeat ){
		INT_CHIPS			nMaxPot		= 0;
		INT_CHIPS			nMinPot		= 0;
		INT_CHIPS			nPot		= pSeat->GetPotAmount();
		PokerTableSeatInfo* pSeatMaxPot = GetMaxPotAmount(nMaxPot, nMinPot);

		if( (nInPlay + nAllIn) == 1 ){		// Hand must be finished.
			bIsRoundEnded	= true;
			nSeatIndex		= -1;
			return NULL;
			}

		if( (nPot < nMaxPot) || (nPot == nMaxPot && pSeat->GetLastAction() == PokerActionTypes::NoAction /*It would be first action in this round.*/ && nInPlay > 1) ){
			bIsRoundEnded	= false;
			nSeatIndex		= pSeat->GetSeat();
			return pSeat;
			}

		if( ((nInPlay == 1) && (nInPlay + nAllIn) > 1) || (nAllIn + nInPlay == nAllIn)){ 
			bOpenSeatCards = true;
			}
		bIsRoundEnded	= true;
		return pSeat;
		}

	if( ((nInPlay == 1) && (nInPlay + nAllIn) > 1) || ((nAllIn + nInPlay) == nAllIn && nAllIn > 1)){ 
		bOpenSeatCards = true;
		}
	bIsRoundEnded	= (nAllIn > 0 || (m_bTournamentTable && nInPlay > 0));
	nSeatIndex		= -1;
#endif
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetMaxPotAmount(INT_CHIPS& nPotAmount, INT_CHIPS& nMinPot){
	int	nCt	= m_seats.GetCount(), nLoop	= 0;
	PokerTableSeatInfo* pSeatMaxPot = NULL;

	nMinPot = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || !IsInPlayByStatus(nLoop, pSeat->GetStatus()) ){
			nLoop ++;
			continue;
			}

		if( nMinPot >= pSeat->GetPotAmount() )
			nMinPot = pSeat->GetPotAmount();

		if( nPotAmount < pSeat->GetPotAmount() ){
			pSeatMaxPot = pSeat;
			nPotAmount	= pSeat->GetPotAmount();
			}
		nLoop ++;
		}
	if( !pSeatMaxPot )
		nMinPot = 0;
	return pSeatMaxPot;
	}

int
PokerTableInfo::CountSeatsByStatus(PokerSeatStatus status){
	int	nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && pSeat->GetStatus() == status )
			nResult ++;
		nLoop ++;
		}
	return nResult;
	}

int
PokerTableInfo::CountSitoutButInPlaySeats(){
#ifdef _SERVER
	int	nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && pSeat->GetStatus() == PokerSeatStatus::SittedOut && pSeat->GetSitoutButInPlayFlag() )
			nResult ++;
		nLoop ++;
		}
	return nResult;
#endif
	return 0;
	}

bool
PokerTableInfo::EmptySeat(int nSeat, bool bDestroySeatObject /*= true*/){
	if( nSeat < 0 || nSeat >= m_cMaxPlayersCt )
		return false;
	m_seats				.SetAt(nSeat, NULL, bDestroySeatObject);
	m_listSeatAvatars	.SetAt(nSeat, NULL, false);
	return true;
	}

bool
PokerTableInfo::SetSeatInfo(int nSeatIndex, PokerTableSeatInfo* pSeatInfo){
	if( nSeatIndex < 0 || nSeatIndex >= m_cMaxPlayersCt )
		return false;
	return m_seats.SetAt(nSeatIndex, pSeatInfo, true);
	}

void
PokerTableInfo::ClearTableCards(){
	m_cCard1 = m_cCard2 = m_cCard3 = m_cCard4 = m_cCard5 = -1;
	}

void
PokerTableInfo::ClearSeatCards(){
	int nLoop = 0, nCt = m_seats.GetCount();
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat )
			pSeat->SetCards(-1, -1, -1, -1);
		nLoop ++;
		}
	}

int
PokerTableInfo::GetFirstEmptySeat(){
	int nLoop = 0, nCt = m_seats.GetCount();
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty )
			return nLoop;
		nLoop ++;
		}
	return -1;
	}

PokerTableSeatInfo*
PokerTableInfo::GetBBSeat(int nSBSeat, int nDealerSeat, int* pSeatIndex /*= NULL*/){
#ifdef _SERVER
	if( m_gameStatus != PokerGameStatus::Started ){
		PokerTableSeatInfo* pSeat			= NULL;
		PokerSeatStatus		status			= PokerSeatStatus::Empty;
		int					nBBSeatIndex	= -1;
		int					nLoop			= (nSBSeat != -1 ? nSBSeat : nDealerSeat);
		int					nStart			= (nSBSeat != -1 ? nSBSeat : nDealerSeat);
		int					nEnd			= nDealerSeat;
		int					nSittedCt		= GetSittedCt();

		if( nSittedCt < 2 ){
			if( pSeatIndex )
				*pSeatIndex = -1;
			return NULL;
			}

		CPtrArray			arrSeats;
		if( nStart != nEnd )
			GetSeatBetweenSeatsClockwise(&arrSeats, nStart, nEnd);
		else{
			for(int i=nLoop+1; i<m_cMaxPlayersCt; i++){
				if( m_seats.GetAt(i) )
					arrSeats.Add(m_seats.GetAt(i));
				}

			for(int i=0; i<=nLoop; i++){
				if( m_seats.GetAt(i) )
					arrSeats.Add(m_seats.GetAt(i));
				}
			}

		for(int i=0; i<arrSeats.GetCount(); i++){
			pSeat = (PokerTableSeatInfo*)arrSeats.GetAt(i);
			if( !pSeat ){
				nBBSeatIndex = -1;
				break;
				}

			if( nBBSeatIndex == nSBSeat )
				continue;

			status = pSeat->GetStatus();
			if( m_bTournamentTable ){ // Tournament table
				if( status == PokerSeatStatus::SittedOut ||
					status == PokerSeatStatus::Sitted ||
					status == PokerSeatStatus::All_In ||
					status == PokerSeatStatus::WaitingForBB ){
					nBBSeatIndex = pSeat->GetSeat();
					break;
					}
				}
			else{ // Cash table
				if( status == PokerSeatStatus::WaitingForBB ||
					status == PokerSeatStatus::PayBBAndPlay ||
					status == PokerSeatStatus::CheckForPositionToStart ||
					status == PokerSeatStatus::All_In ||
					status == PokerSeatStatus::Sitted ){
					nBBSeatIndex = pSeat->GetSeat();
					break;
					}
				else{
					if( status == PokerSeatStatus::SittedOut && pSeat->GetHandsDealed() > 0 )
						pSeat->SetMissedBB(true);
					}
				}
			}

		/*
		while( TRUE ){
			pSeat = GetNextSeatInfo(nLoop, &nBBSeatIndex);
			if( !pSeat ){
				nBBSeatIndex = -1;
				break;
				}

			status = pSeat->GetStatus();
			if( m_bTournamentTable ){ // Tournament table
				if( status == PokerSeatStatus::SittedOut ||
					status == PokerSeatStatus::Sitted ||
					status == PokerSeatStatus::All_In ||
					status == PokerSeatStatus::WaitingForBB ){
					nBBSeatIndex = pSeat->GetSeat();
					break;
					}
				}
			else{ // Cash table
				if( status == PokerSeatStatus::WaitingForBB ||
					status == PokerSeatStatus::PayBBAndPlay ||
					status == PokerSeatStatus::CheckForPositionToStart ||
					status == PokerSeatStatus::All_In ||
					status == PokerSeatStatus::Sitted ){
					nBBSeatIndex = pSeat->GetSeat();
					break;
					}
				else{
					if( status == PokerSeatStatus::SittedOut && pSeat->GetHandsDealed() > 0 )
						pSeat->SetMissedBB(true);
					}
				}
			nLoop = pSeat->GetSeat();
			}*/
	
		// Dealer button couldn't be bigblind!!!!
		//if( nBBSeatIndex == nDealerSeat )
		//	nDealerSeat = nSBSeat;

		if( pSeatIndex )
			*pSeatIndex = nBBSeatIndex;
		return pSeat;
		}
#endif
	return NULL;
	}

int
PokerTableInfo::GetBBAskSeats(int nSBSeat, int nDealerSeat, CDWordArray* pArrSeatIndexes /*= NULL*/){
	ASSERT(pArrSeatIndexes);
	if( !pArrSeatIndexes )
		return 0;
	if( m_gameStatus != PokerGameStatus::Started ){
		PokerTableSeatInfo* pSeat			= NULL;
		PokerSeatStatus		status			= PokerSeatStatus::Empty;
		int					nBBSeatIndex	= -1;
		int					nLoop			= (nSBSeat != -1 ? nSBSeat : nDealerSeat);
		int					nStart			= (nSBSeat != -1 ? nSBSeat : nDealerSeat);
		int					nEnd			= nDealerSeat;

		CPtrArray			arrSeats;
		if( nStart != nEnd )
			GetSeatBetweenSeatsClockwise(&arrSeats, nStart, nEnd);
		else{
			for(int i=nLoop+1; i<m_cMaxPlayersCt; i++){
				if( m_seats.GetAt(i) )
					arrSeats.Add(m_seats.GetAt(i));
				}

			for(int i=0; i<nLoop; i++){
				if( m_seats.GetAt(i) )
					arrSeats.Add(m_seats.GetAt(i));
				}
			}

		for(int i=0; i<arrSeats.GetCount(); i++){
			pSeat = (PokerTableSeatInfo*)arrSeats.GetAt(i);
			if( !pSeat ){
				continue;
				}

			status = pSeat->GetStatus();
			if( m_bTournamentTable ){ // Tournament table
				if( status == PokerSeatStatus::SittedOut ||
					status == PokerSeatStatus::Sitted ||
					status == PokerSeatStatus::All_In ||
					status == PokerSeatStatus::CheckForPositionToStart ||
					status == PokerSeatStatus::WaitingForBB ){
					pArrSeatIndexes->Add(pSeat->GetSeat());
					break;
					}
				}
			else{ // Cash table
				if( status == PokerSeatStatus::WaitingForBB ||
					status == PokerSeatStatus::PayBBAndPlay ||
					status == PokerSeatStatus::CheckForPositionToStart ||
					status == PokerSeatStatus::All_In ||
					status == PokerSeatStatus::Sitted ){
					pArrSeatIndexes->Add(pSeat->GetSeat());
					break;
					}
				}
			}
		return (int)pArrSeatIndexes->GetCount();
		}
	return 0;
	}


PokerTableSeatInfo*
PokerTableInfo::GetSBSeat(int nDealerSeat, int* pSeatIndex /*= NULL*/){
#ifdef _SERVER
	if( m_gameStatus != PokerGameStatus::Started ){
		PokerTableSeatInfo* pSeatInfo = NULL;
		int nSeatCt			= GetSittedCt();
		int nSBSeatIndex	= ((int)m_cBBSeat);
		if( nSBSeatIndex == -1 ){ // First hand ....
			if( nSeatCt > 2 ){
				if( m_bTournamentTable ){ // Tournament table
					pSeatInfo = GetNextSeatedSeatInfo(nDealerSeat, &nSBSeatIndex); // Small blind.
					}
				else{	// Cash table
					/* old one
					pSeatInfo = GetNextSeatInfoByStatus(-1, PokerSeatStatus::Sitted, &nSBSeatIndex);			// Dealer
					pSeatInfo = GetNextSeatInfoByStatus(nSBSeatIndex, PokerSeatStatus::Sitted, &nSBSeatIndex);	// Small blind.
					*/
					pSeatInfo = GetNextSeatInfoByStatus(nDealerSeat, PokerSeatStatus::Sitted, &nSBSeatIndex);	// Small blind.
					}				
				}
			else
			if( nSeatCt == 2 ){
				nSBSeatIndex	= nDealerSeat;
				pSeatInfo		= GetSeatInfo(nSBSeatIndex);
				if( m_bTournamentTable ){ // Tournament table
					if( pSeatInfo == NULL || (pSeatInfo->GetStatus() != PokerSeatStatus::Sitted && pSeatInfo->GetStatus() != PokerSeatStatus::SittedOut))
						nSBSeatIndex = -1;
					}
				else{// Cash table.
					if( pSeatInfo == NULL || pSeatInfo->GetStatus() != PokerSeatStatus::Sitted )
						nSBSeatIndex = -1;
					}
				}
			}
		else{
			if( nSeatCt == 2 )
				nSBSeatIndex = nDealerSeat;
			pSeatInfo = GetSeatInfo(nSBSeatIndex);
			if( m_bTournamentTable ){ // Tournament table
				if( pSeatInfo == NULL || (pSeatInfo->GetStatus() != PokerSeatStatus::Sitted && pSeatInfo->GetStatus() != PokerSeatStatus::SittedOut))
					nSBSeatIndex = -1;
				}
			else{// Cash table.
				if( pSeatInfo == NULL || pSeatInfo->GetStatus() != PokerSeatStatus::Sitted )
					nSBSeatIndex = -1;

				// Mark missing sb seat.
				PokerTableSeatInfo* pPrevBBSeat = GetSeatInfo((int)m_cBBSeat);
				if( pPrevBBSeat && pPrevBBSeat->GetStatus() == PokerSeatStatus::SittedOut ){
					pPrevBBSeat->SetMissedSB(true);
					}
				}
			}

		if( pSeatIndex )
			*pSeatIndex = nSBSeatIndex;
		return pSeatInfo;
		}
#endif
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetDealerSeat(int* pSeatIndex /*= NULL*/){
#ifdef _SERVER
	if( m_gameStatus == PokerGameStatus::Starting ){
		PokerTableSeatInfo* pSeatInfo	= NULL;
		int					nSeatCt		= GetSittedCt();

		//Old
		//int nDealer = (m_cDealerSeat == m_cSBSeat && m_cDealerSeat != -1 && nSeatCt == 2) ? m_cBBSeat : m_cSBSeat;
		int nDealer = -1;//(m_cDealerSeat == m_cSBSeat && m_cDealerSeat != -1 && nSeatCt == 2) ? m_cBBSeat : m_cSBSeat;
		if( m_cDealerSeat != -1 ){
			if( m_cDealerSeat == m_cSBSeat && m_cDealerSeat != -1 && nSeatCt == 2 ){
				nDealer = m_cBBSeat;

				// Remain dealer button the same becouse there are some more players who pays bb.
				int nWaitForBBSeat = -1;
				GetNextSeatInfoByStatus(m_cBBSeat, PokerSeatStatus::WaitingForBB, &nWaitForBBSeat);
				if( nWaitForBBSeat != -1 && IsBetweenSeats(nWaitForBBSeat, m_cBBSeat, m_cDealerSeat) ){
					nDealer = m_cDealerSeat;
					SetSeatStatus(nWaitForBBSeat, PokerSeatStatus::Sitted);
					}
				else{
					int nPayBBAndSeat = -1;
					GetNextSeatInfoByStatus(m_cBBSeat, PokerSeatStatus::PayBBAndPlay, &nPayBBAndSeat);
					if( nPayBBAndSeat != -1 && IsBetweenSeats(nPayBBAndSeat, m_cBBSeat, m_cDealerSeat) ){
						nDealer = m_cDealerSeat;
						SetSeatStatus(nPayBBAndSeat, PokerSeatStatus::Sitted);
						}
					}
				}
			else{
				// In previous hand we didn't have smallblind
				if( m_cSBSeat == -1 ){
					nDealer = (m_nSeatRefusedToPaySB == -1 ? m_cBBSeat : m_nSeatRefusedToPaySB);
					}
				else{
					if( GetSeatStatus(m_cSBSeat) != PokerSeatStatus::Sitted ){
						if( GetSeatStatus(m_cBBSeat) != PokerSeatStatus::Sitted )
							GetPrevSeatInfoByStatus(m_cBBSeat, PokerSeatStatus::Sitted, &nDealer);
						else
							nDealer = m_cBBSeat;
						}
					else
						nDealer = m_cSBSeat;
					}
				}
			}
		
		if( nDealer == -1 ){
			/* Random dealer seat. */
			AutoSortedArray arrSeats;
			int nLoop = 0, nCt = (int)m_cMaxPlayersCt;

			m_nSeatRefusedToPaySB	= -1;

			if( m_bTournamentTable ){ // Tournament table
				while( nLoop < nCt ){
					PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
					if( pSeat && (pSeat->GetStatus() == PokerSeatStatus::Sitted || pSeat->GetStatus() == PokerSeatStatus::SittedOut ) )
						arrSeats.Add((void*)nLoop, pSeat);
					nLoop ++;
					}

				if( arrSeats.GetCount() > 1 ){
					srand(time(NULL));
					int nIndex	= rand()%arrSeats.GetCount();
					nDealer		= (int)arrSeats.Get(nIndex);
					pSeatInfo	= (PokerTableSeatInfo*)arrSeats.GetData(nIndex);
					}
				else
					pSeatInfo = GetNextSeatInfoByStatus(-1, PokerSeatStatus::Sitted, &nDealer);// Dealer
				}
			else{ // Cash table
				while( nLoop < nCt ){
					PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
					if( !pSeat ){
						nLoop ++;
						continue;
						}
					if( pSeat->GetStatus() == PokerSeatStatus::Sitted ){
						// Unmark missed sb and bb flags.
						pSeat->SetMissedSB(false);
						pSeat->SetMissedBB(false);
						arrSeats.Add((void*)nLoop, pSeat);
						}
					nLoop ++;
					}

				if( arrSeats.GetCount() > 1 ){
					srand(time(NULL));
					int nIndex	= rand()%arrSeats.GetCount();
					nDealer		= (int)arrSeats.Get(nIndex);
					pSeatInfo	= (PokerTableSeatInfo*)arrSeats.GetData(nIndex);
					}
				else
					pSeatInfo = GetNextSeatInfoByStatus(-1, PokerSeatStatus::Sitted, &nDealer);// Dealer
				}

			}
		if( pSeatIndex )
			*pSeatIndex = nDealer;
		pSeatInfo = GetSeatInfo(nDealer);
		return pSeatInfo;
		}
#endif
	return NULL;
	}

int
PokerTableInfo::GetPayBBAndPlaySeats(int nBBSeat, int nDealerSeat, CDWordArray& arrPayBBSeats){
	arrPayBBSeats.RemoveAll();
	if( nBBSeat == -1 || nDealerSeat == -1 )
		return 0;
	
	// Seats between BB and Dealer who is ready to pay BB and play.
	int nSeat = nBBSeat;
	while( TRUE ){
		PokerTableSeatInfo* pSeat = GetNextSeatInfo(nSeat);
		if( !pSeat || !IsBetweenSeats(pSeat->GetSeat(), nBBSeat, nDealerSeat) )
			break;
		if( pSeat->GetStatus() == PokerSeatStatus::PayBBAndPlay )
			arrPayBBSeats.Add((DWORD)pSeat->GetSeat());
		nSeat = pSeat->GetSeat();
		}
	return (int)arrPayBBSeats.GetCount();
	}

bool
PokerTableInfo::IsBetweenSeats(int nSeat, int nSeatStart, int nSeatEnd){
	int nSeatCt = m_seats.GetCount();
	if( nSeat >= nSeatCt )
		return false;

	if( nSeatStart >= nSeatCt )
		return false;

	if( nSeatEnd >= nSeatCt || nSeatStart == nSeatEnd)
		return false;

	if( nSeatStart > nSeatEnd )
		nSeatEnd += (nSeatCt - nSeatStart);
	else
		nSeatEnd -= nSeatStart;

	if( nSeatStart > nSeat )
		nSeat += (nSeatCt - nSeatStart);
	else
		nSeat -= nSeatStart;
	nSeatStart = 0;
	return (nSeat > nSeatStart && nSeat < nSeatEnd);
	}

int
PokerTableInfo::GetSeatBetweenSeatsByStatusClockwise(CPtrArray* pArrResult, int nSeatStart, int nSeatEnd, PokerSeatStatus status){
	if( !pArrResult )
		return 0;
	int	nResultCt	 = 0;
	int	nSeatBetween = nSeatStart;
	PokerTableSeatInfo* pSeatBetween = GetNextSeatInfoByStatus(nSeatBetween, status, &nSeatBetween);
	while( IsBetweenSeats(nSeatBetween, nSeatStart, nSeatEnd) ){
		for(int i=0; i<pArrResult->GetCount(); i++){
			if( pArrResult->GetAt(i) == pSeatBetween )
				return nResultCt;
			}
		pArrResult->Add(pSeatBetween);
		nResultCt ++;
		pSeatBetween = GetNextSeatInfoByStatus(nSeatBetween, status, &nSeatBetween);
		if( !pSeatBetween ) 
			break;
		nSeatBetween = pSeatBetween->GetSeat();
		}
	return nResultCt;
	}

int
PokerTableInfo::GetSeatBetweenSeatsClockwise(CPtrArray* pArrResult, int nSeatStart, int nSeatEnd){
	if( !pArrResult )
		return 0;
	int	nResultCt	 = 0;
	int	nSeatBetween = nSeatStart;
	PokerTableSeatInfo* pSeatBetween = GetNextSeatInfo(nSeatBetween, &nSeatBetween);
	while( IsBetweenSeats(nSeatBetween, nSeatStart, nSeatEnd) ){
		for(int i=0; i<pArrResult->GetCount(); i++){
			if( pArrResult->GetAt(i) == pSeatBetween )
				return nResultCt;
			}
		pArrResult->Add(pSeatBetween);
		nResultCt ++;

		pSeatBetween = GetNextSeatInfo(nSeatBetween, &nSeatBetween);
		if( !pSeatBetween ) 
			break;
		nSeatBetween = pSeatBetween->GetSeat();
		}
	return nResultCt;
	}

bool
PokerTableInfo::SetLastAction(int nSeat, PokerActionTypes action){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetLastAction(action);
	return true;
	}

PokerActionTypes
PokerTableInfo::GetLastActionType(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( pSeatInfo )
		return pSeatInfo->GetLastAction();
	return PokerActionTypes::NoAction;
	}

bool
PokerTableInfo::SetSitOutNextHand(int nSeat, bool bValue){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetSitOutNextHand(bValue);
	return true;
	}

bool
PokerTableInfo::SetSitOutNextBB(int nSeat, bool bValue){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetSitOutNextBB(bValue);
	return true;
	}

bool
PokerTableInfo::GetSitOutNextHand(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	return pSeatInfo->GetSitOutNextHand();
	}

bool
PokerTableInfo::GetSitOutNextBB(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	return pSeatInfo->GetSitOutNextBB();
	}	


bool
PokerTableInfo::SetSitOutButInPlay(int nSeat, bool bValue){
#ifdef _SERVER
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	if( bValue ){
		if( pSeatInfo->GetStatus() == PokerSeatStatus::SittedOut )
			pSeatInfo->SetSitoutButInPlayFlag(true);
		}
	else
		pSeatInfo->SetSitoutButInPlayFlag(false);
	return true;
#endif
	return false;
	}

bool
PokerTableInfo::GetSitOutButInPlay(int nSeat){
#ifdef _SERVER
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo || pSeatInfo->GetStatus() != PokerSeatStatus::SittedOut ) 
		return false;
	return pSeatInfo->GetSitoutButInPlayFlag();
#endif
	return false;
	}

int
PokerTableInfo::SetSeatStatus(PokerSeatStatus destStatus, PokerSeatStatus status){
	int nSeat = 0, nCt = m_seats.GetCount(), nChanged = 0;
	while( nSeat < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
		if( !pSeat ){
			nSeat ++;
			continue;
			}

#ifdef _SERVER
	if( m_arrHandPlayersInfo[nSeat].nUserId > 0 )
		m_arrHandPlayersInfo[nSeat].status = status;
#endif

		if( pSeat->GetStatus() == destStatus ){
			pSeat->SetStatus(status);
			nChanged ++;
			}
		nSeat ++;
		}
	return nChanged;
	}

bool
PokerTableInfo::SetSeatStatus(int nSeat, PokerSeatStatus status){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetStatus(status);
	return true;
	}

int
PokerTableInfo::GetEmptySeatCt(){
	int nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty ){
			nResult ++;
			nLoop ++;
			continue;
			}
		nLoop ++;
		}
	return nResult;
	}

bool
PokerTableInfo::FoldSeat(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetLastAction(PokerActionTypes::Fold);
	pSeatInfo->SetStatus	(PokerSeatStatus::Folded);
	return true;
	}

bool
PokerTableInfo::SitOut(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetLastAction(PokerActionTypes::SitOut);
	pSeatInfo->SetStatus	(PokerSeatStatus::SittedOut);
	return true;
	}

bool
PokerTableInfo::ReturnBack(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetLastAction(PokerActionTypes::ReturnBack);
	pSeatInfo->SetStatus	(PokerSeatStatus::Sitted);
	return true;
	}

INT_CHIPS
PokerTableInfo::GetBalance(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return 0;
	return pSeatInfo->GetBalance();
	}

bool
PokerTableInfo::SetBalance(int nSeat, INT_CHIPS nAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetBalance(nAmount);

#ifdef _SERVER
	if( m_arrHandPlayersInfo[nSeat].nUserId > 0 )
		m_arrHandPlayersInfo[nSeat].nBalanceAmount = nAmount;
#endif
	return true;
	}

void
PokerTableInfo::SetHandWonState(int nSeat, bool bWon){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo) return;
	pSeatInfo->SetHandWonState(bWon);
	}

int
PokerTableInfo::GetSeatAmounts(EnumerableObject<PokerSeatAmountInfo>* seatAmounts, bool bAddEmptyInfos /*= false*/){
	int nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty ){
			if( bAddEmptyInfos )
				seatAmounts->Add(NULL);
			nLoop ++;
			continue;
			}

		PokerSeatAmountInfo* pAmount = new PokerSeatAmountInfo();
		pAmount->m_cSeat			= (char)nLoop;
		pAmount->m_nBalance			= pSeat->GetBalance();
		pAmount->m_nPotAmount		= pSeat->GetPotAmount();
		pAmount->m_nTotalPotAmount	= pSeat->GetTotalPot();
		seatAmounts->Add(pAmount);
		nLoop ++;
		}
	return nLoop;
	}

int
PokerTableInfo::GetSeatStatuses(EnumerableObject<PokerSeatStateInfo>* seatStatuses){
	int nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty ){
			nLoop ++;
			continue;
			}

		PokerSeatStateInfo* pStatus = new PokerSeatStateInfo();
		pStatus->m_cSeat			= (char)nLoop;
		pStatus->m_cStatus			= (char)pSeat->GetStatus();
		seatStatuses->Add(pStatus);
		nLoop ++;
		}
	return nLoop;
	}

_String
PokerTableInfo::GetSeatUserName(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return _T("");
	return pSeatInfo->GetUserName();
	}

ClientConnection*
PokerTableInfo::GetConnection(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return NULL;
	return pSeatInfo->GetConnection();
	}

INT_CHIPS
PokerTableInfo::GetAddedAmount(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return 0;
	return pSeatInfo->GetAddedAmount();
	}

bool
PokerTableInfo::Add_AddedAmount(int nSeat, INT_CHIPS nAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetAddedAmount(pSeatInfo->GetAddedAmount() + nAmount);
	return true;
	}

bool
PokerTableInfo::SetAddedAmount(int nSeat, INT_CHIPS nAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetAddedAmount(nAmount);
	return true;
	}

void
PokerTableInfo::ClearPotAmount(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return;
	pSeatInfo->SetTotalPot	(0);
	pSeatInfo->SetPotAmount	(0);

#ifdef _SERVER
	if( m_arrHandPlayersInfo[nSeat].nUserId > 0 ){
		m_arrHandPlayersInfo[nSeat].nPotAmount = 0;
		m_arrHandPlayersInfo[nSeat].nTotalPotAmount = 0;
		}
#endif
	}

int
PokerTableInfo::ClearPotAmounts(){
	int nLoop = 0, nCt = m_cMaxPlayersCt, nRet	= 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nLoop);
		if( !pSeatInfo ){
			nLoop ++;
			continue;
			}
#ifdef _SERVER
	if( m_arrHandPlayersInfo[nLoop].nUserId > 0 ){
		m_arrHandPlayersInfo[nLoop].nPotAmount = 0;
		m_arrHandPlayersInfo[nLoop].nTotalPotAmount = 0;
		}
#endif
		pSeatInfo->SetTotalPot	(0);
		pSeatInfo->SetPotAmount	(0);
		nLoop ++; nRet ++;
		}
	return nRet;
	}

bool
PokerTableInfo::AddPotAmount(int nSeat, INT_CHIPS nAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;

	pSeatInfo->SetPotAmount	(pSeatInfo->GetPotAmount() + nAmount);
	pSeatInfo->AddTotalPot	(nAmount);
	AddTotalPotAmount		(nAmount);
#ifdef _SERVER
	if( m_arrHandPlayersInfo[nSeat].nUserId > 0 )
		m_arrHandPlayersInfo[nSeat].nPotAmount = pSeatInfo->GetPotAmount();
#endif
	return true;
	}

INT_CHIPS
PokerTableInfo::GetPotAmount(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return 0;
	return pSeatInfo->GetPotAmount();
	}

bool
PokerTableInfo::SetPotAmount(int nSeat, INT_CHIPS nPotAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetPotAmount(nPotAmount);
#ifdef _SERVER
	if( m_arrHandPlayersInfo[nSeat].nUserId > 0 )
		m_arrHandPlayersInfo[nSeat].nPotAmount = nPotAmount;
#endif
	return true;
	}

INT_CHIPS
PokerTableInfo::GetSeatTotalPot(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return 0;
	return pSeatInfo->GetTotalPot();
	}

bool
PokerTableInfo::SetSeatTotalPot(int nSeat, INT_CHIPS nTotalPotAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetTotalPot(nTotalPotAmount);

#ifdef _SERVER
	if( m_arrHandPlayersInfo[nSeat].nUserId > 0 )
		m_arrHandPlayersInfo[nSeat].nTotalPotAmount = nTotalPotAmount;
#endif
	return true;
	}

bool
PokerTableInfo::GetBetAmounts(int nSeat, INT_CHIPS& nMinBet, INT_CHIPS& nMaxBet){
#ifdef _SERVER
	INT_CHIPS			nCallAmount = 0;
	PokerTableSeatInfo* pInfoDest	= m_seats.GetAt(nSeat);
	PokerTableSeatInfo* pInfo		= NULL;
	int					nLoop		= 0;
	int					nCt			= m_cMaxPlayersCt;
	INT_CHIPS			nMaxPot		= 0; // Maximum entered pot amount.
	nMinBet							= 0;
	nMaxBet							= 0;

	if( !pInfoDest )
		return false;

	if( !IsInPlayByStatus(nSeat, pInfoDest->GetStatus()) )
		return false;

	while(nLoop < nCt){
		pInfo = m_seats.GetAt(nLoop);
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Folded || pInfo == pInfoDest ){
			nLoop ++;
			continue;
			}
		nMaxPot = max(nMaxPot, pInfo->GetPotAmount());
		nLoop ++;
		}

	GameSubType limit = (GameSubType)m_shSubTypeId;
	if( m_cGameTypeId == (BYTE)GameType::TexasHoldem ){
		// No-Limit
		if( (int)(limit&GameSubType::Holdem_NoLimit) == (int)GameSubType::Holdem_NoLimit ){
			//nMinBet = 2*(nMaxPot - pInfoDest->GetPotAmount());
			//Old code
			//nMinBet = 2*nMaxPot - pInfoDest->GetPotAmount();
			nMinBet = 2*max(nMaxPot, m_nBigBlindAmount) - pInfoDest->GetPotAmount();
			if( nMinBet <= 0 ){
				nMinBet = m_nBigBlindAmount;
				}

			nMaxBet = pInfoDest->GetBalance();
			if( nMinBet > nMaxBet )
				return false;
			return true;
			}

		// Fixed limit
		if( (int)(limit&GameSubType::Holdem_FixedLimit) == (int)GameSubType::Holdem_FixedLimit ){
			/* Old code
			int nSmallBet	= m_nBigBlindAmount;
			int nBigBet		= 2*nSmallBet;

			if( m_round == PokerRoundType::PreFlop ||
				m_round == PokerRoundType::Flop ){
				nMinBet = nMaxBet = ((nMaxPot + nSmallBet) - pInfoDest->GetPotAmount());
				}
			else
			if( m_round == PokerRoundType::Turn ||
				m_round == PokerRoundType::River ){
				nMinBet = nMaxBet = ((nMaxPot + nBigBet) - pInfoDest->GetPotAmount());
				}
			else
				return false;
*/
			int nMaxBetReiseCt = ((m_round == PokerRoundType::PreFlop) ? 3 : 4);
			if( m_nBetRaiseCtPerRound >= nMaxBetReiseCt /*Maximum 3 or 4 bets or reises per round*/ )
				return false;

			int nSmallBet	= m_nBigBlindAmount/2;
			//int nBigBet	= 2*nSmallBet;

			if( m_round == PokerRoundType::PreFlop ||
				m_round == PokerRoundType::Flop ){
				nMinBet = nMaxBet = ((nMaxPot + nSmallBet) - pInfoDest->GetPotAmount());
				}
			else
			if( m_round == PokerRoundType::Turn ||
				m_round == PokerRoundType::River ){
				nSmallBet	*= 2;
				nMinBet = nMaxBet = ((nMaxPot + nSmallBet) - pInfoDest->GetPotAmount());
				}
			else
				return false;

			if( nMinBet > pInfoDest->GetBalance() )
				return false;
			return true;
			}

		// Pot Limit.
		if( (int)(limit&GameSubType::Holdem_PotLimit) == (int)GameSubType::Holdem_PotLimit ){
			INT_CHIPS nCallAmount = (nMaxPot - pInfoDest->GetPotAmount());
			/*
			if( !nCallAmount ){
				if( nMaxPot == 0 ){
					nMinBet	= m_nBigBlindAmount;
					nMaxBet	= max(m_nTotalPotAmount, nMinBet);
					}
				else{
					nMinBet	= m_nBigBlindAmount;
					nMaxBet	= nMinBet;
					}
				}
			else{
				nMinBet	= 2*nCallAmount + pInfoDest->GetPotAmount();
				nMaxBet	= 2*nCallAmount + m_nTotalPotAmount;
				}*/

			nMinBet	= max(m_nBigBlindAmount, 2*nCallAmount + pInfoDest->GetPotAmount());
			nMaxBet	= max(nMinBet, 2*nCallAmount + m_nTotalPotAmount);
			return true;
			}
		}
	else
		if( m_cGameTypeId == (BYTE)GameType::Omaha ){
			// Omaha or Omaha Hi/Lo
			if( (limit&GameSubType::Omaha_Omaha)		== GameSubType::Omaha_Omaha || 
				(limit&GameSubType::Omaha_OmahaHiLo)	== GameSubType::Omaha_OmahaHiLo
				){
				//nMinBet = 2*(nMaxPot - pInfoDest->GetPotAmount());
				nMinBet = 2*nMaxPot - pInfoDest->GetPotAmount();
				if( nMinBet <= 0 ){
					nMinBet = m_nBigBlindAmount;
					}

				nMaxBet = pInfoDest->GetBalance();
				if( nMinBet > nMaxBet )
					return false;
				return true;
				}
			}
#endif
	return false;
	}

INT_CHIPS
PokerTableInfo::GetCallAmount(int nSeat){
	INT_CHIPS			nCallAmount = 0;
	PokerTableSeatInfo* pInfoDest	= m_seats.GetAt(nSeat);
	PokerTableSeatInfo* pInfo		= NULL;
	int					nLoop		= 0;
	int					nCt			= m_cMaxPlayersCt;
	INT_CHIPS			nMaxPot		= 0; // Maximum entered pot amount.

	/* Old one
	if( !pInfoDest || pInfoDest->GetStatus() != PokerSeatStatus::InPlay )
		return 0;
	*/
	if( !pInfoDest )
		return false;

	if( !IsInPlayByStatus(nSeat, pInfoDest->GetStatus()) )
		return false;

	while(nLoop < nCt){
		pInfo = m_seats.GetAt(nLoop);
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Folded || pInfo == pInfoDest ){
			nLoop ++;
			continue;
			}
		nMaxPot = max(nMaxPot, pInfo->GetPotAmount());
		nLoop ++;
		}

	if( pInfoDest->GetPotAmount() > nMaxPot )
		return 0;
	return (nMaxPot - pInfoDest->GetPotAmount());
	}

int	
PokerTableInfo::GetSeatCtByStatus(PokerSeatStatus status, CPtrArray* pArrSeatInfos /*= NULL*/){
	int nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	if( pArrSeatInfos ){
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( !pSeat || pSeat->GetStatus() != status ){
				nLoop ++;
				continue;
				}
			pArrSeatInfos->Add(pSeat);
			nResult ++;
			nLoop	++;
			}
		}
	else{
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( !pSeat || pSeat->GetStatus() != status ){
				nLoop ++;
				continue;
				}
			nResult ++;
			nLoop	++;
			}
		}
	
	return nResult;
	}

int
PokerTableInfo::GetSeats(CPtrArray& arrSeats){
	for(int i=0; i<m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(i);
		if( pSeat )
			arrSeats.Add((void*)pSeat);
		}
	return (int)arrSeats.GetCount();
	}

int
PokerTableInfo::GetSitOutNextHandSeats(CDWordArray& arrSeatIndexes, bool bExcludeEmptyBalanceSeats /*= false*/){
	arrSeatIndexes.RemoveAll();
	int nLoop = 0, nCt = m_cMaxPlayersCt;
	if( !bExcludeEmptyBalanceSeats ){
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( !pSeat || pSeat->GetStatus() < PokerSeatStatus::Sitted ){
				nLoop ++;
				continue;
				}

			// Empty balance or 'SitOutNextHand' flag
//			if( pSeat->GetBalance() < m_nBigBlindAmount || 
			if( pSeat->GetBalance() <= 0 || 
				pSeat->GetSitOutNextHand() || 
				(pSeat->IsOffline() && !m_bTournamentTable) /* Offline but not tournament seat*/ ){
				arrSeatIndexes.Add(pSeat->GetSeat());
				}
			nLoop ++;
			}
		}
	else{
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( !pSeat || pSeat->GetStatus() < PokerSeatStatus::Sitted ){
				nLoop ++;
				continue;
				}

			// 'SitOutNextHand' flag
			if( pSeat->GetSitOutNextHand() || 
				(pSeat->IsOffline() && !m_bTournamentTable) /* Offline but not tournament seat*/ ){
				arrSeatIndexes.Add(pSeat->GetSeat());
				}
			nLoop ++;
			}
		}
	return ((int)arrSeatIndexes.GetCount());
	}

int
PokerTableInfo::GetEmptyBalanceSeats(CDWordArray& arrSeatIndexes, bool bExcludeSitoutSeats /*= false*/){
	arrSeatIndexes.RemoveAll();
	int nLoop = 0, nCt = m_cMaxPlayersCt;
	if( bExcludeSitoutSeats ){
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( !pSeat || pSeat->GetStatus() < PokerSeatStatus::Sitted || pSeat->GetStatus() == PokerSeatStatus::SittedOut ){
				nLoop ++;
				continue;
				}
			// Empty balance
	//		if( pSeat->GetBalance() < m_nBigBlindAmount )
			if( pSeat->GetBalance() <= 0 )
				arrSeatIndexes.Add(pSeat->GetSeat());
			nLoop ++;
			}
		}
	else{
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( !pSeat || pSeat->GetStatus() < PokerSeatStatus::Sitted ){
				nLoop ++;
				continue;
				}
			// Empty balance
	//		if( pSeat->GetBalance() < m_nBigBlindAmount )
			if( pSeat->GetBalance() <= 0 )
				arrSeatIndexes.Add(pSeat->GetSeat());
			nLoop ++;
			}
		}
	return ((int)arrSeatIndexes.GetCount());
	}

void
PokerTableInfo::StartHand(int nDealerSeat, int nSBSeat, int nBBSeat, int nSeatRefusedToPaySB /*= -1*/){
#ifdef _SERVER
	CPtrArray		arrInPlaySeats;
	GetSeatCtByStatus(PokerSeatStatus::InPlay, &arrInPlaySeats);
	GetSeatCtByStatus(PokerSeatStatus::All_In, &arrInPlaySeats);
	if( m_bTournamentTable )
		GetSeatCtByStatus(PokerSeatStatus::SittedOut, &arrInPlaySeats);

	int				nPlayersCt	= arrInPlaySeats.GetCount();
	int				nLoop		= 0;
	int				nCard		= 0;
	AutoSortedArray arrUnique;

	// Empty hand players info.
	for(int i=0; i<sizeof(m_arrHandPlayersInfo)/sizeof(HandPlayersInfoStruct); i++){
		m_arrHandPlayersInfo[i].sUserName		= _T("");
		m_arrHandPlayersInfo[i].nUserId			= 0;
		m_arrHandPlayersInfo[i].btCard1			= -1;
		m_arrHandPlayersInfo[i].btCard2			= -1;
		m_arrHandPlayersInfo[i].btCard3			= -1;
		m_arrHandPlayersInfo[i].btCard4			= -1;
		m_arrHandPlayersInfo[i].nBalanceAmount	= 0;
		m_arrHandPlayersInfo[i].nPotAmount		= 0;
		m_arrHandPlayersInfo[i].nTotalPotAmount = 0;
		m_arrHandPlayersInfo[i].status			= PokerSeatStatus::Empty;
		} 

	// Start round.
	StartRound(PokerRoundType::PreFlop);

	m_dTimeStartHand		= COleDateTime::GetCurrentTime();
	m_cDealerSeat			= nDealerSeat;
	m_cSBSeat				= nSBSeat;
	m_cBBSeat				= nBBSeat;
	m_nSeatRefusedToPaySB	= nSeatRefusedToPaySB;
	m_gameStatus			= PokerGameStatus::Started;
	m_nPlayersCtStarted		= 0;
	
	srand(time(NULL));
	// Set table card 1. {{
	rand();
	nCard = rand()%52;
	while( arrUnique.IndexOf((void*)nCard) != -1 )
		nCard = rand()%52;
	arrUnique.Add((void*)nCard);
	m_nTableCard1 = nCard;
	// }}

	// Set table card 2. {{
	rand();
	nCard = rand()%52;
	while( arrUnique.IndexOf((void*)nCard) != -1 )
		nCard = rand()%52;
	arrUnique.Add((void*)nCard);
	m_nTableCard2 = nCard;
	// }}

	// Set table card 3. {{
	rand();
	nCard = rand()%52;
	while( arrUnique.IndexOf((void*)nCard) != -1 )
		nCard = rand()%52;
	arrUnique.Add((void*)nCard);
	m_nTableCard3 = nCard;
	// }}

	// Set table card 4. {{
	rand();
	nCard = rand()%52;
	while( arrUnique.IndexOf((void*)nCard) != -1 )
		nCard = rand()%52;
	arrUnique.Add((void*)nCard);
	m_nTableCard4 = nCard;
	// }}

	// Set table card 5. {{
	rand();
	nCard = rand()%52;
	while( arrUnique.IndexOf((void*)nCard) != -1 )
		nCard = rand()%52;
	arrUnique.Add((void*)nCard);
	m_nTableCard5 = nCard;
	// }}
	
	/*
	/// Temp BEGIN {{
	//m_nTableCard1 = 45;
	//m_nTableCard2 = 18;
	//m_nTableCard3 = 51;
	//m_nTableCard4 = 28;
	//m_nTableCard5 = 7;
	m_nTableCard1 = CardIndexByName(_T("9D"));
	m_nTableCard2 = CardIndexByName(_T("8D"));
	m_nTableCard3 = CardIndexByName(_T("AH"));
	m_nTableCard4 = CardIndexByName(_T("9S"));
	m_nTableCard5 = CardIndexByName(_T("AD"));

	//9D8DAH9SAD          

	CDWordArray arrHandCards;
	arrHandCards.Add(CardIndexByName(_T("8C")));
	arrHandCards.Add(CardIndexByName(_T("7D")));
	arrHandCards.Add(CardIndexByName(_T("6D")));
	arrHandCards.Add(CardIndexByName(_T("7C")));
	arrHandCards.Add(CardIndexByName(_T("8S")));
	arrHandCards.Add(CardIndexByName(_T("QD")));
	arrHandCards.Add(CardIndexByName(_T("QH")));
	arrHandCards.Add(CardIndexByName(_T("JD")));
	arrHandCards.Add(CardIndexByName(_T("JH")));
	arrHandCards.Add(CardIndexByName(_T("TD")));
	arrHandCards.Add(CardIndexByName(_T("TH")));
	arrHandCards.Add(CardIndexByName(_T("9D")));
	arrHandCards.Add(CardIndexByName(_T("9H")));
	
	int nCardCt = arrHandCards.GetCount();
	int nCardIn = 0;

	while(nLoop < nPlayersCt){
		PokerTableSeatInfo* pSeat = (PokerTableSeatInfo*)arrInPlaySeats[nLoop];
		ASSERT(pSeat);
		if( !pSeat ){
			nLoop ++;
			continue;
			}

		pSeat->SetCard1(arrHandCards[nCardIn%nCardCt]); nCardIn ++;
		pSeat->SetCard2(arrHandCards[nCardIn%nCardCt]); nCardIn ++;

		// Set hand player info.
		int nSeat = pSeat->GetSeat();
		m_arrHandPlayersInfo[nSeat].sUserName		= pSeat->GetUserName();
		m_arrHandPlayersInfo[nSeat].nUserId			= pSeat->GetUserId();
		m_arrHandPlayersInfo[nSeat].btCard1			= (BYTE)pSeat->GetCard1();
		m_arrHandPlayersInfo[nSeat].btCard2			= (BYTE)pSeat->GetCard2();
		m_arrHandPlayersInfo[nSeat].btCard3			= (BYTE)pSeat->GetCard3();
		m_arrHandPlayersInfo[nSeat].btCard4			= (BYTE)pSeat->GetCard4();
		m_arrHandPlayersInfo[nSeat].nBalanceAmount	= pSeat->GetBalance();
		m_arrHandPlayersInfo[nSeat].nPotAmount		= pSeat->GetPotAmount();
		m_arrHandPlayersInfo[nSeat].nTotalPotAmount = pSeat->GetTotalPot();
		m_arrHandPlayersInfo[nSeat].status			= pSeat->GetStatus();

		nLoop ++;
		}
	
	return;
	// }}*/
	// Temp END
	
	if( m_cGameTypeId == (BYTE)GameType::Omaha ){
		while(nLoop < nPlayersCt){
			PokerTableSeatInfo* pSeat = (PokerTableSeatInfo*)arrInPlaySeats[nLoop];
			ASSERT(pSeat);
			if( !pSeat ){
				nLoop ++;
				continue;
				}

			// Set card 1. {{
			nCard = rand()%52;
			while( arrUnique.IndexOf((void*)nCard) != -1 )
				nCard = rand()%52;
			arrUnique.Add((void*)nCard);
			pSeat->SetCard1(nCard);
			// }}

			// Set card 2. {{
			nCard = rand()%52;
			while( arrUnique.IndexOf((void*)nCard) != -1 )
				nCard = rand()%52;
			arrUnique.Add((void*)nCard);
			pSeat->SetCard2(nCard);
			// }}

			// Set card 3. {{
			nCard = rand()%52;
			while( arrUnique.IndexOf((void*)nCard) != -1 )
				nCard = rand()%52;
			arrUnique.Add((void*)nCard);
			pSeat->SetCard3(nCard);
			// }}

			// Set card 4. {{
			nCard = rand()%52;
			while( arrUnique.IndexOf((void*)nCard) != -1 )
				nCard = rand()%52;
			arrUnique.Add((void*)nCard);
			pSeat->SetCard4(nCard);
			// }}
			m_nPlayersCtStarted ++;
			nLoop ++;

			// Set hand player info.
			int nSeat = pSeat->GetSeat();
			m_arrHandPlayersInfo[nSeat].sUserName		= pSeat->GetUserName();
			m_arrHandPlayersInfo[nSeat].nUserId			= pSeat->GetUserId();
			m_arrHandPlayersInfo[nSeat].btCard1			= (BYTE)pSeat->GetCard1();
			m_arrHandPlayersInfo[nSeat].btCard2			= (BYTE)pSeat->GetCard2();
			m_arrHandPlayersInfo[nSeat].btCard3			= (BYTE)pSeat->GetCard3();
			m_arrHandPlayersInfo[nSeat].btCard4			= (BYTE)pSeat->GetCard4();
			m_arrHandPlayersInfo[nSeat].nBalanceAmount	= pSeat->GetBalance();
			m_arrHandPlayersInfo[nSeat].nPotAmount		= pSeat->GetPotAmount();
			m_arrHandPlayersInfo[nSeat].nTotalPotAmount = pSeat->GetTotalPot();
			m_arrHandPlayersInfo[nSeat].status			= pSeat->GetStatus();
			}
		}
	else{
		while(nLoop < nPlayersCt){
			PokerTableSeatInfo* pSeat = (PokerTableSeatInfo*)arrInPlaySeats[nLoop];
			ASSERT(pSeat);
			if( !pSeat ){
				nLoop ++;
				continue;
				}

			//Temp
			/*
			if( !nLoop && pSeat->GetUserId() == 39803 ){
				arrUnique.Add((void*)CardIndexByName(_T("QH")));
				pSeat->SetCard1(CardIndexByName(_T("QH")));
				arrUnique.Add((void*)CardIndexByName(_T("QC")));
				pSeat->SetCard2(CardIndexByName(_T("QC")));
				nLoop ++;
				continue;
				}*/

			// Set card 1. {{
			nCard = rand()%52;
			while( arrUnique.IndexOf((void*)nCard) != -1 )
				nCard = rand()%52;
			arrUnique.Add((void*)nCard);
			pSeat->SetCard1(nCard);
			// }}

			// Set card 2. {{
			nCard = rand()%52;
			while( arrUnique.IndexOf((void*)nCard) != -1 )
				nCard = rand()%52;
			arrUnique.Add((void*)nCard);
			pSeat->SetCard2(nCard);
			// }}
			m_nPlayersCtStarted ++;
			nLoop ++;

			// Set hand player info.
			int nSeat = pSeat->GetSeat();
			m_arrHandPlayersInfo[nSeat].sUserName		= pSeat->GetUserName();
			m_arrHandPlayersInfo[nSeat].nUserId			= pSeat->GetUserId();
			m_arrHandPlayersInfo[nSeat].btCard1			= (BYTE)pSeat->GetCard1();
			m_arrHandPlayersInfo[nSeat].btCard2			= (BYTE)pSeat->GetCard2();
			m_arrHandPlayersInfo[nSeat].btCard3			= (BYTE)pSeat->GetCard3();
			m_arrHandPlayersInfo[nSeat].btCard4			= (BYTE)pSeat->GetCard4();
			m_arrHandPlayersInfo[nSeat].nBalanceAmount	= pSeat->GetBalance();
			m_arrHandPlayersInfo[nSeat].nPotAmount		= pSeat->GetPotAmount();
			m_arrHandPlayersInfo[nSeat].nTotalPotAmount = pSeat->GetTotalPot();
			m_arrHandPlayersInfo[nSeat].status			= pSeat->GetStatus();
			}
		}
	m_nFlopPlayersCt = 0;
#endif
	}

void
PokerTableInfo::StartRound(PokerRoundType round){
	bool bPreFlop = false;
	switch(round){
		case PokerRoundType::PreFlop:{
			m_round				= round;
// Before start round player's should pay SB or BB or ANTE.
//			m_nTotalPotAmount	= 0;	
			if( m_nAnteAmount <= 0 && m_nFineAmount <= 0 )
				m_nMainPotAmount= 0;
			m_nRakeAmount		= 0;
			m_cCard1			= -1;	
			m_cCard2			= -1;	
			m_cCard3			= -1;	
			m_cCard4			= -1;	
			m_cCard5			= -1;	
			m_nTableCard1		= -1;
			m_nTableCard2		= -1;
			m_nTableCard3		= -1;
			m_nTableCard4		= -1;
			m_nTableCard5		= -1;
			bPreFlop			= true;

			m_listRoundAmounts.RemoveAll();
			PokerRoundAmountStruct* pRoundStruct	= new PokerRoundAmountStruct;
			pRoundStruct->round						= round;
			pRoundStruct->nTotalPotAmount			= m_nMainPotAmount;
			pRoundStruct->nRoundPotAmount			= m_nMainPotAmount;
			pRoundStruct->nRakeAmount				= 0;
			m_listRoundAmounts.Add(pRoundStruct);
			break;
			}
		case PokerRoundType::Flop:{
			PokerRoundAmountStruct* pRoundStruct	= new PokerRoundAmountStruct;
			pRoundStruct->round						= round;
			pRoundStruct->nTotalPotAmount			= m_nTotalPotAmount;
			pRoundStruct->nRoundPotAmount			= (m_nTotalPotAmount - m_nMainPotAmount);
			pRoundStruct->nRakeAmount				= CalculateRakeAmount(m_nTotalPotAmount);
			m_listRoundAmounts.Add(pRoundStruct);

			m_round									= round;
			m_cCard1								= m_nTableCard1;
			m_cCard2								= m_nTableCard2;
			m_cCard3								= m_nTableCard3;
			m_nMainPotAmount						= m_nTotalPotAmount;
			m_nRakeAmount							= pRoundStruct->nRakeAmount;
			m_nFlopPlayersCt						= GetSeatCtByStatus(PokerSeatStatus::All_In) + GetSeatCtByStatus(PokerSeatStatus::InPlay);
			break;
			}
		case PokerRoundType::Turn:{
			PokerRoundAmountStruct* pRoundStruct	= new PokerRoundAmountStruct;
			pRoundStruct->round						= round;
			pRoundStruct->nTotalPotAmount			= m_nTotalPotAmount;
			pRoundStruct->nRoundPotAmount			= (m_nTotalPotAmount - m_nMainPotAmount);
			pRoundStruct->nRakeAmount				= CalculateRakeAmount(m_nTotalPotAmount);
			m_listRoundAmounts.Add(pRoundStruct);

			m_round									= round;
			m_cCard1								= m_nTableCard1;
			m_cCard2								= m_nTableCard2;
			m_cCard3								= m_nTableCard3;
			m_cCard4								= m_nTableCard4;
			m_nMainPotAmount						= m_nTotalPotAmount;
			m_nRakeAmount							= pRoundStruct->nRakeAmount;
			break;
			}
		case PokerRoundType::River:{
			PokerRoundAmountStruct* pRoundStruct	= new PokerRoundAmountStruct;
			pRoundStruct->round						= round;
			pRoundStruct->nTotalPotAmount			= m_nTotalPotAmount;
			pRoundStruct->nRoundPotAmount			= (m_nTotalPotAmount - m_nMainPotAmount);
			pRoundStruct->nRakeAmount				= CalculateRakeAmount(m_nTotalPotAmount);
			m_listRoundAmounts.Add(pRoundStruct);

			m_round									= round;
			m_cCard1								= m_nTableCard1;
			m_cCard2								= m_nTableCard2;
			m_cCard3								= m_nTableCard3;
			m_cCard4								= m_nTableCard4;
			m_cCard5								= m_nTableCard5;
			m_nMainPotAmount						= m_nTotalPotAmount;
			m_nRakeAmount							= pRoundStruct->nRakeAmount;
			break;
			}
		};

#ifdef _SERVER
	m_nBetRaiseCtPerRound	= 0;
	m_listPots.RemoveAll(true);
	if( !bPreFlop )
		GetDetailedPots(&m_listPots);
#endif

	if( !bPreFlop ){
		int nLoop = 0, nCt = m_seats.GetCount();
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( pSeat ){
				pSeat->SetPotAmount	(0);
				pSeat->SetLastAction(PokerActionTypes::NoAction);
				}
			nLoop	++;
			}
		}
	}

bool
PokerTableInfo::CanEmptySeatWhenLeave(int nSeat){
#ifdef _SERVER
	PokerTableSeatInfo*	pSeat	= GetSeatInfo(nSeat);
	if( !pSeat ) return false;
	PokerSeatStatus		status	= GetSeatStatus(nSeat);
	if(m_gameStatus == PokerGameStatus::NotStarted || 
		(status == PokerSeatStatus::SittedOut && !pSeat->GetSitoutButInPlayFlag()) || 
		status == PokerSeatStatus::CheckForPositionToStart || 
		status == PokerSeatStatus::Empty || 
		(m_gameStatus != PokerGameStatus::NotStarted && !IsInPlayByStatus(nSeat, status) /*status != PokerSeatStatus::InPlay && status != PokerSeatStatus::All_In*/) ){
		return true;
		}
#endif
	return false;
	}

void
PokerTableInfo::FinishHand(bool bShowdown){
	// Calculate statustic info. {{
	if( m_nTotalPotAmount > 0 ){
		COleDateTime		dTimeFinish		= COleDateTime::GetCurrentTime();
		COleDateTimeSpan	diff			= dTimeFinish - m_dTimeStartHand;
		int					nHandDurSec		= diff.GetTotalSeconds();

		if( m_nPlayedHandsCt > 0 ){
			// Avg pot amount.
			double dPotAmount		= m_dAvgPotAmount * m_nPlayedHandsCt;
			dPotAmount				+= (double)(m_nTotalPotAmount / 100.0);
			m_dAvgPotAmount			= dPotAmount / (m_nPlayedHandsCt + 1);

			// Avg players count.
			double dPlayersCt		= m_dAvgPlayers*m_nPlayedHandsCt;
			dPlayersCt += (double)(m_nPlayersCtStarted);
			m_dAvgPlayers			= dPlayersCt / (m_nPlayedHandsCt + 1);

			// Avg hand duration(sec)
			m_nSumPlayedHandsDurSec += nHandDurSec;
			m_nAvgHandDurationSec	= m_nSumPlayedHandsDurSec / (m_nPlayedHandsCt + 1); 
/*
			_String s1, s2;
			s1.Format(_T("%02d-%02d-%04d %02d:%02d:%02d"), dTimeFinish.GetDay(), dTimeFinish.GetMonth(), dTimeFinish.GetYear(), 
				dTimeFinish.GetHour(), dTimeFinish.GetMinute(), dTimeFinish.GetSecond());
			s2.Format(_T("%02d-%02d-%04d %02d:%02d:%02d"), m_dtStatStarted.GetDay(), m_dtStatStarted.GetMonth(), m_dtStatStarted.GetYear(), 
				m_dtStatStarted.GetHour(), m_dtStatStarted.GetMinute(), m_dtStatStarted.GetSecond());
*/
			// Avg hands per hour.
			diff					= dTimeFinish - m_dtStatStarted;
			int nStatGetherDurSec	= diff.GetTotalSeconds();
			m_dAvgHandsPerHour		= (nStatGetherDurSec < 3600) ? (double)(m_nPlayedHandsCt + 1) : (m_nPlayedHandsCt + 1) / ((double)(nStatGetherDurSec / 3600.0));

			// Avg player flop percent.
			m_nSumPlayersCtStarted	+= m_nPlayersCtStarted;
			m_nSumFlopPlayersCt		+= m_nFlopPlayersCt;
			m_dPlayerFlop			= m_nSumPlayersCtStarted > 0 ? ((double)m_nSumFlopPlayersCt) / m_nSumPlayersCtStarted : 0.0;
			}
		else{
			m_dtStatStarted			= COleDateTime::GetCurrentTime();
			m_dAvgPotAmount			= (double)(m_nTotalPotAmount / 100.0f);
			m_dAvgPlayers			= (double)(m_nPlayedHandsCt);
			m_dAvgHandsPerHour		= 0.0;
			m_nSumPlayedHandsDurSec	= nHandDurSec;
			m_nAvgHandDurationSec	= nHandDurSec;
			m_nSumPlayersCtStarted	= m_nPlayersCtStarted;
			m_nSumFlopPlayersCt		= m_nFlopPlayersCt;
			m_dPlayerFlop			= m_nSumPlayersCtStarted > 0 ? ((double)m_nSumFlopPlayersCt) / m_nSumPlayersCtStarted : 0.0;
			}
		m_nPlayedHandsCt	++;
		}
	// }}

	// Update seat statistic info. {{ 
	for(int i=0; i<m_cMaxPlayersCt; i++){
		PokerTableSeatInfo* pInfo = GetSeatInfo(i);
		if( pInfo ){
			PokerSeatStatus status = pInfo->GetStatus();
			if( status == PokerSeatStatus::InPlay || status == PokerSeatStatus::All_In || status == PokerSeatStatus::Folded ){
				pInfo->m_nHandsDealed		++;
				if( pInfo->GetPotAmount() > 0 )
					pInfo->m_nHandsPlayed	++;

				if( pInfo->m_bWonState ){
					pInfo->m_nHandsWon	++;
					if( m_round < PokerRoundType::Flop )
						pInfo->m_nHandsWonPreflop	++;
					else
						pInfo->m_nHandsWonPostflop	++;
					if( bShowdown )
						pInfo->m_nHandsWonShowdown	++;
					}
				pInfo->SetHandWonState(false);
				}
			else{
#ifdef _SERVER
				if( status == PokerSeatStatus::SittedOut ){
					if( pInfo->GetSitoutButInPlayFlag() )
						pInfo->SetSitoutButInPlayFlag(false);
					}
#endif
				}
			}
		}
	// }}

	// Hide seat cards if they are shown.
	ShowSeatCards		(false);
	ClearPotAmounts		();
	SetMainPotAmount	(0);
	SetTotalPotAmount	(0);
	m_nRakeAmount	= 0;
	m_nFineAmount	= 0;
	ClearTableCards		();
	
	m_gameStatus	= PokerGameStatus::NotStarted;
	m_round			= PokerRoundType::No;
	m_cActiveSeat	= -1;

	// Remove list of pots.
	m_listPots.RemoveAll(true);

	// Change seat status as sitted. {{
	SetSeatStatus(PokerSeatStatus::InPlay, PokerSeatStatus::Sitted);
	SetSeatStatus(PokerSeatStatus::Folded, PokerSeatStatus::Sitted);
	SetSeatStatus(PokerSeatStatus::All_In, PokerSeatStatus::Sitted);
	// }}
	}

int
PokerTableInfo::ShowSeatCards(bool bShow /*= true*/){
	if( m_bCardsShown == bShow ) return 0;
	int nLoop = 0, nCt = m_seats.GetCount(), nRet = 0;
	m_bCardsShown = bShow;
	if( bShow ){
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( pSeat && (pSeat->GetStatus() == PokerSeatStatus::All_In || pSeat->GetStatus() == PokerSeatStatus::InPlay) ){
				pSeat->m_bCardsShown = true;
				nRet ++;
				}
			nLoop ++;
			}
		}
	else{
		while( nLoop < nCt ){
			PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
			if( pSeat ){
				pSeat->m_bCardsShown = false;
				nRet ++;
				}
			nLoop ++;
			}
		}
	return nRet;
	}

int
PokerTableInfo::PayAnte(){
	if( m_nAnteAmount <= 0 ) return 0;
	int nLoop = 0, nPlayersCt	= m_seats.GetCount(), nResultCt	= 0;
	// Pay ante amount.
	while(nLoop < nPlayersCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || 
			(!m_bTournamentTable && pSeat->GetStatus() != PokerSeatStatus::Sitted) || 
			(m_bTournamentTable && pSeat->GetStatus() != PokerSeatStatus::Sitted && pSeat->GetStatus() != PokerSeatStatus::SittedOut) ){
			nLoop ++;
			continue;
			}

		INT_CHIPS nAnteAmount	= m_nAnteAmount;
		INT_CHIPS nBalance		= pSeat->GetBalance();
		INT_CHIPS nBalanceNew	= pSeat->GetBalance() - nAnteAmount;

		// Insufficient balance amount.
		if( nBalance < nAnteAmount ){
			nBalanceNew = 0;
			nAnteAmount	= nBalance;
			pSeat->SetStatus(PokerSeatStatus::All_In);
			}

		pSeat->SetBalance		(nBalanceNew);
		pSeat->AddTotalPot		(nAnteAmount);
		AddMainPotAmount		(nAnteAmount);
		AddTotalPotAmount		(nAnteAmount);
		nResultCt	++;
		nLoop		++;
		}
	return nResultCt;
	}

bool
PokerTableInfo::GetRoundAmounts(PokerRoundType round, INT_CHIPS& nTotalPot, INT_CHIPS& nRoundPot, UINT& nRakeAmount){
	int nLoop = 0, nCt = m_listRoundAmounts.GetCount();
	while( nLoop < nCt ){
		PokerRoundAmountStruct* pStruct = m_listRoundAmounts.GetAt(nLoop);
		if( pStruct && pStruct->round == round ){
			nTotalPot	= pStruct->nTotalPotAmount;
			nRoundPot	= pStruct->nRoundPotAmount;
			nRakeAmount = pStruct->nRakeAmount;
			return true;
			}
		nLoop ++;
		}
	nTotalPot = nRoundPot = nRakeAmount = 0;
	return false;
	}

void
PokerTableInfo::FalsePreActions(){
	int nLoop = 0;
	while( nLoop < m_seats.GetCount()  ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat )
			pSeat->FalsePreActions();
		nLoop ++;
		}
	}

bool
PokerTableInfo::GetSeatPreActions(int nSeat, EnumerableObject<PokerPreAction>* pListPreActions){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat || pSeat->GetStatus() != PokerSeatStatus::InPlay )
		return false;

	INT_CHIPS		nCallAmount = GetCallAmount(nSeat);
	PokerPreAction* pPreAction	= NULL;
	if( nCallAmount == 0 ){
		pPreAction					= new PokerPreAction();
		pPreAction->m_cActionType	= PokerTableOption::Option_Check;
		pPreAction->m_bValue		= pSeat->m_bPreAction_Check;
		pPreAction->m_nValue		= 0;
		pListPreActions->Add(pPreAction);

		pPreAction					= new PokerPreAction();
		pPreAction->m_cActionType	= PokerTableOption::Option_CallAnyCheck;
		pPreAction->m_bValue		= pSeat->m_bPreAction_CallAnyCheck;
		pPreAction->m_nValue		= 0;
		pListPreActions->Add(pPreAction);

		pPreAction					= new PokerPreAction();
		pPreAction->m_cActionType	= PokerTableOption::Option_CheckFold;
		pPreAction->m_bValue		= pSeat->m_bPreAction_CheckFold;
		pPreAction->m_nValue		= 0;
		pListPreActions->Add(pPreAction);
		}
	else{
		pPreAction					= new PokerPreAction();
		pPreAction->m_cActionType	= PokerTableOption::Option_Fold;
		pPreAction->m_bValue		= pSeat->m_bPreAction_Fold;
		pPreAction->m_nValue		= 0;
		pListPreActions->Add(pPreAction);

		if( nCallAmount <= pSeat->GetBalance() ){
			pSeat->m_bPreAction_Call		= (pSeat->m_nPreAction_CallAmount == nCallAmount);
			pPreAction						= new PokerPreAction();
			pPreAction->m_cActionType		= PokerTableOption::Option_Call;
			pPreAction->m_bValue			= pSeat->m_bPreAction_Call;
			pPreAction->m_nValue			= nCallAmount;
			pListPreActions->Add(pPreAction);
			}
		else
			pSeat->m_bPreAction_Call = false;

		pPreAction					= new PokerPreAction();
		pPreAction->m_cActionType	= PokerTableOption::Option_CallAny;
		pPreAction->m_bValue		= pSeat->m_bPreAction_CallAny;
		pPreAction->m_nValue		= 0;
		pListPreActions->Add(pPreAction);
		}
	return true;
	}

int
PokerTableInfo::GetTimeBankSeconds(int nSeat){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat ) return 0;
	return pSeat->GetTimeBankSeconds();
	}

void
PokerTableInfo::SetTimeBankSeconds(int nSeat, int nTimeBankSec){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat ) return;
	pSeat->SetTimeBankSeconds(nTimeBankSec);
	}

float
PokerTableInfo::GetCollectedTimeBank(int nSeat){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat ) return 0.0f;
	return pSeat->GetCollectedTimeBank();
	}

void
PokerTableInfo::SetCollectedTimeBank(int nSeat, float fTimeBankSec){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat ) return;
	pSeat->SetCollectedTimeBank(fTimeBankSec);
	}

void
PokerTableInfo::AddChatMessage(DateTime dtTime, int nSeat, PokerActionTypes acType, INT_CHIPS nAmount, bool bSystem /*= false*/, _String sMessage /*= _T("")*/){
	PokerChatMessage* pMessage	= new PokerChatMessage();
	pMessage->m_cSeat			= (char)nSeat;
	pMessage->m_sUserName		= GetSeatUserName(nSeat);
	pMessage->m_bSystem			= bSystem;
	pMessage->m_actionType		= (char)acType;
	pMessage->m_nAmount			= nAmount;
	pMessage->m_sMessage		= sMessage;
	pMessage->m_dtTime			= dtTime;
	m_listChatItems.Add(pMessage);

	if( m_listChatItems.GetCount() > m_nMaxChatMessageCt )
		m_listChatItems.RemoveAt(0, true);
	}

bool
PokerTableInfo::GetSeatWithIp(DWORD dwIp, int& nSeat){
	int nLoop = 0, nCt = m_cMaxPlayersCt;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && pSeat->GetIpAddress() == dwIp ){
			nSeat = nLoop;
			return true;
			}
		nLoop ++;
		}
	return false;
	}

int
PokerTableInfo::CardIndexByName(_String sName){
	if( sName.IsEmpty() || sName.GetLength() != 2 )
		return -1;

	sName.MakeUpper();

	const _String arrCardNum[]	= {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	const _String arrCardType[] = {_T("S"), _T("C"), _T("D"), _T("H")};

	int nCardTypeIndex	= -1;
	int nCardNum		= -1;

	_String sType = sName.Right(1);
	_String sNum = sName.Left(1);

	for(int i=0; i<4; i++){
		if( sType == arrCardType[i] ){
			nCardTypeIndex = i;
			break;
			}
		}
	if( nCardTypeIndex == -1 ) return -1;
	for(int i=0; i<13; i++){
		if( sNum == arrCardNum[i] ){
			nCardNum = i;
			break;
			}
		}
	if( nCardNum == -1 ) return -1;
	return ((nCardTypeIndex*13) + nCardNum);
	}
