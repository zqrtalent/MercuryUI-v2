// PokerHandHistory.cpp : implementation file
//

#include "stdafx.h"
#include "PokerHandHistory.h"
#include "../../Utility/GrowableMemory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ================================================================
//
//	class: PokerHandAction
//
// ================================================================

PokerHandAction::PokerHandAction(){
	}

BEGIN_DECLARE_VAR(PokerHandAction, Serializable)
	DECLARE_VAR(_T("PokerHandAction"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Seat"),				_T(""),	VariableType::VT_Int8, offsetof(PokerHandAction,m_cSeat), false, false, false)
	DECLARE_VAR(_T("Action"),			_T(""),	VariableType::VT_Int8, offsetof(PokerHandAction,m_cAction), false, false, false)
	DECLARE_VAR(_T("SeatStatus"),		_T(""),	VariableType::VT_Int8, offsetof(PokerHandAction,m_cSeatStatus), false, false, false)
	DECLARE_VAR(_T("TimeSec"),			_T(""),	VariableType::VT_Int8, offsetof(PokerHandAction,m_cTimeSec), false, false, false)
	DECLARE_VAR(_T("ActAmount"),		_T(""),	VariableType::VT_Chips, offsetof(PokerHandAction,m_nActAmount), false, false, false)
	DECLARE_VAR(_T("PotAmount"),		_T(""),	VariableType::VT_Chips, offsetof(PokerHandAction,m_nPotAmount), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),	_T(""),	VariableType::VT_Chips, offsetof(PokerHandAction,m_nTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("BalanceAmount"),	_T(""),	VariableType::VT_Chips, offsetof(PokerHandAction,m_nBalanceAmount), false, false, false)
	DECLARE_VAR(_T("CardsShown"),		_T(""),	VariableType::VT_None, offsetof(PokerHandAction,m_listCardsShown), false, true, true)
END_DECLARE_VAR()

PokerHandAction::~PokerHandAction(){
	}

// ================================================================
//
//	class: PokerHandBestCards
//
// ================================================================

PokerHandBestCards::PokerHandBestCards(){
	}

BEGIN_DECLARE_VAR(PokerHandBestCards, Serializable)
	DECLARE_VAR(_T("PokerHandBestCards"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Seat"),					_T(""),		VariableType::VT_Int8, offsetof(PokerHandBestCards,m_btSeat), false, false, false)
	DECLARE_VAR(_T("BestCardsClass"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandBestCards,m_cBestCardsClass), false, false, false)
	DECLARE_VAR(_T("BestCard1"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandBestCards,m_cBestCard1), false, false, false)
	DECLARE_VAR(_T("BestCard2"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandBestCards,m_cBestCard2), false, false, false)
	DECLARE_VAR(_T("BestCard3"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandBestCards,m_cBestCard3), false, false, false)
	DECLARE_VAR(_T("BestCard4"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandBestCards,m_cBestCard4), false, false, false)
	DECLARE_VAR(_T("BestCard5"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandBestCards,m_cBestCard5), false, false, false)
END_DECLARE_VAR()

PokerHandBestCards::~PokerHandBestCards(){
	}

// ================================================================
//
//	class: PokerHandRound
//
// ================================================================

PokerHandRound::PokerHandRound(){
	}

BEGIN_DECLARE_VAR(PokerHandRound, Serializable)
	DECLARE_VAR(_T("PokerHandRound"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("RoundType"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandRound,m_cRoundType), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),	_T(""),		VariableType::VT_Chips, offsetof(PokerHandRound,m_nTotalPotAmount_Start), false, false, false)
	DECLARE_VAR(_T("RoundPotAmount"),	_T(""),		VariableType::VT_Int32, offsetof(PokerHandRound,m_nRakeAmount_Start), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),	_T(""),		VariableType::VT_Chips, offsetof(PokerHandRound,m_nTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("RoundPotAmount"),	_T(""),		VariableType::VT_Chips, offsetof(PokerHandRound,m_nRoundPotAmount), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),		_T(""),		VariableType::VT_Int32, offsetof(PokerHandRound,m_nRakeAmount), false, false, false)
	DECLARE_VAR(_T("roundActions"),		_T(""),		VariableType::VT_None, offsetof(PokerHandRound,m_roundActions), false, true, true)
	DECLARE_VAR(_T("listHandBestCards"),_T(""),		VariableType::VT_None, offsetof(PokerHandRound,m_listHandBestCards), false, true, true)
	DECLARE_VAR(_T("listPots"),			_T(""),		VariableType::VT_None, offsetof(PokerHandRound,m_listPots), false, true, true)
END_DECLARE_VAR()

PokerHandRound::~PokerHandRound(){
	}

// ================================================================
//
//	class: PokerHandHistory
//
// ================================================================

PokerHandHistory::PokerHandHistory(){
	ClearHistory();
	}

BEGIN_DECLARE_VAR(PokerHandHistory, Serializable)
	DECLARE_VAR(_T("PokerHandHistory"), _T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("HandId"),			_T(""),		VariableType::VT_Int64, offsetof(PokerHandHistory,m_nHandId), false, false, false)
	DECLARE_VAR(_T("TableId"),			_T(""),		VariableType::VT_Int32, offsetof(PokerHandHistory,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),		_T(""),		VariableType::VT_Int32, offsetof(PokerHandHistory,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableName"),		_T(""),		VariableType::VT_String, offsetof(PokerHandHistory,m_sTableName), false, false, false)
	DECLARE_VAR(_T("GameType"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cGameType), false, false, false)
	DECLARE_VAR(_T("SubType"),			_T(""),		VariableType::VT_Int16, offsetof(PokerHandHistory,m_shSubType), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cCurrencyId), false, false, false)
	DECLARE_VAR(_T("HandStartSec"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cHandStartSec), false, false, false)
	DECLARE_VAR(_T("RoundStartSec"),	_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cRoundStartSec), false, false, false)
	DECLARE_VAR(_T("HandFinishStartSec"),_T(""),	VariableType::VT_Int8, offsetof(PokerHandHistory,m_cHandFinishStartSec), false, false, false)
	DECLARE_VAR(_T("MaxSeatCt"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cMaxSeatCt), false, false, false)
	DECLARE_VAR(_T("Dealer"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cDealer), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cBigBlind), false, false, false)
	DECLARE_VAR(_T("RakePercent"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cRakePercent), false, false, false)
	DECLARE_VAR(_T("SmallBlindAmount"),	_T(""),		VariableType::VT_Int32, offsetof(PokerHandHistory,m_nSmallBlindAmount), false, false, false)
	DECLARE_VAR(_T("BigBlindAmount"),	_T(""),		VariableType::VT_Int32, offsetof(PokerHandHistory,m_nBigBlindAmount), false, false, false)
	DECLARE_VAR(_T("AnteAmount"),		_T(""),		VariableType::VT_Int32, offsetof(PokerHandHistory,m_nAnteAmount), false, false, false)
	DECLARE_VAR(_T("TableCard1"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cTableCard1), false, false, false)
	DECLARE_VAR(_T("TableCard2"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cTableCard2), false, false, false)
	DECLARE_VAR(_T("TableCard3"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cTableCard3), false, false, false)
	DECLARE_VAR(_T("TableCard4"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cTableCard4), false, false, false)
	DECLARE_VAR(_T("TableCard5"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandHistory,m_cTableCard5), false, false, false)
	DECLARE_VAR(_T("listSeats"),		_T(""),		VariableType::VT_None, offsetof(PokerHandHistory,m_listSeats), false, true, true)
	DECLARE_VAR(_T("listRounds"),		_T(""),		VariableType::VT_None, offsetof(PokerHandHistory,m_listRounds), false, true, true)
	DECLARE_VAR(_T("listWinners"),		_T(""),		VariableType::VT_None, offsetof(PokerHandHistory,m_listWinners), false, true, true)
	DECLARE_VAR(_T("listPots"),			_T(""),		VariableType::VT_None, offsetof(PokerHandHistory,m_listPots), false, true, true)
	DECLARE_VAR(_T("listHandCards"),	_T(""),		VariableType::VT_None, offsetof(PokerHandHistory,m_listHandCards), false, true, true)
END_DECLARE_VAR()

PokerHandHistory::~PokerHandHistory(){
	m_pCurrentRound = NULL;
	}

bool
PokerHandHistory::Load(const _String sFile){
	bool bRet = false;
	HANDLE hFile = CreateFile(sFile, GENERIC_READ, 0L, NULL, OPEN_EXISTING, 0L, NULL);
	if( hFile != INVALID_HANDLE_VALUE ){
		DWORD dwRead = GetFileSize(hFile, NULL);
		GrowableMemory mem(dwRead);
		mem.SetBufferSize		(dwRead);
		mem.SetUsedBufferSize	(dwRead);
		ReadFile(hFile, mem.GetBufferPtr(), dwRead, &dwRead, NULL);
		// Clear history info.
		ClearHistory();
		// Deserialize.
		bRet = (Deserialize(&mem) == TRUE);
		CloseHandle(hFile);
		}
	return bRet;
	}

bool
PokerHandHistory::Load(LongBinary* pBin){
	if( !pBin || !pBin->GetBinarySize() )
		return false;
	void* lpBuffer = pBin->LockMemory();
	if( !lpBuffer ) return false;
	GrowableMemory mem(0, 0, false);
	mem.SetReadonlyBuffer((BYTE*)lpBuffer, (int)pBin->GetBinarySize());
	// Clear history info.
	ClearHistory();
	// Deserialize.
	bool bRet = (Deserialize(&mem) == TRUE);
	pBin->UnlockMemory();
	return bRet;
	}

void
PokerHandHistory::StartHand(PokerTableInfo* pInfo, __int64 nHandId, int nRakePercent, int nHandStartSec, int nRoundStartSec){
	ClearHistory(); // Clear previous history info.

	m_nHandId				= nHandId;
	m_nTableId				= pInfo->m_nTableId;
	m_nTournamentId			= pInfo->m_nTournamentId;
	m_sTableName			= pInfo->m_sTableName;
	m_cGameType				= (BYTE)pInfo->m_cGameTypeId;
	m_shSubType				= (short)pInfo->m_shSubTypeId;
	m_cMaxSeatCt			= (char)pInfo->m_cMaxPlayersCt;
	m_cDealer				= pInfo->m_cDealerSeat;
	m_cSmallBlind			= pInfo->m_cSBSeat;
	m_cBigBlind				= pInfo->m_cBBSeat;
	m_cRakePercent			= (char)nRakePercent;
	m_cCurrencyId			= pInfo->m_cCurrencyId;

	m_cHandStartSec			= (char)nHandStartSec;
	m_cHandFinishStartSec	= 0;
	m_cRoundStartSec		= (char)nRoundStartSec;

	m_nSmallBlindAmount		= pInfo->m_nSmallBlindAmount;
	m_nBigBlindAmount		= pInfo->m_nBigBlindAmount;
	m_nAnteAmount			= pInfo->m_nAnteAmount;

	m_cTableCard1			= -1; // Hidden card
	m_cTableCard2			= -1;
	m_cTableCard3			= -1;
	m_cTableCard4			= -1;
	m_cTableCard5			= -1;

	// Copy seat info list. {{
	int nLoop	= 0, nCt	= pInfo->m_seats.GetCount();
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeatInfo = pInfo->m_seats.GetAt(nLoop);
		if( !pSeatInfo ){
			m_listSeats.Add(NULL);
			nLoop ++;
			continue;
			}

		// Copy seat info.
		PokerTableSeatInfo* pSeatInfoCopy = new PokerTableSeatInfo();
		pSeatInfo->Copy(pSeatInfoCopy);
		pSeatInfoCopy->SetSession(NULL, 0, 0);

#ifdef _SERVER
		
#else
		pSeatInfoCopy->SetCard1(pSeatInfo->GetCard1());
		pSeatInfoCopy->SetCard2(pSeatInfo->GetCard2());
		pSeatInfoCopy->SetCard3(pSeatInfo->GetCard3());
		pSeatInfoCopy->SetCard4(pSeatInfo->GetCard4());
#endif
		pSeatInfoCopy->SetBalance	(pSeatInfoCopy->GetBalance() + pSeatInfoCopy->GetPotAmount());
		pSeatInfoCopy->SetPotAmount	(0);
		pSeatInfoCopy->SetTotalPot	(0);
		m_listSeats.Add(pSeatInfoCopy);
		nLoop ++;
		}
	// }}
	}

void
PokerHandHistory::FinishHand(List<PokerHandWinnerInfo>* pWinners, PokerTableInfo* pTableInfo, int nHandFinishSec){
	if( pTableInfo ){
		m_cTableCard1		= pTableInfo->m_cCard1;
		m_cTableCard2		= pTableInfo->m_cCard2;
		m_cTableCard3		= pTableInfo->m_cCard3;
		m_cTableCard4		= pTableInfo->m_cCard4;
		m_cTableCard5		= pTableInfo->m_cCard5;
		}
	m_cHandFinishStartSec	= (char)nHandFinishSec;

	if( m_pCurrentRound && !m_pCurrentRound->m_nTotalPotAmount ){
		if( m_pCurrentRound->m_cRoundType == PokerRoundType::Showdown ){
			m_pCurrentRound->m_nTotalPotAmount	= pTableInfo->GetTotalPotAmount();
			m_pCurrentRound->m_nRoundPotAmount	= 0;
			m_pCurrentRound->m_nRakeAmount		= pTableInfo->GetRakeAmount();
			}
		else{
			pTableInfo->GetRoundAmounts((PokerRoundType)m_pCurrentRound->m_cRoundType, 
										m_pCurrentRound->m_nTotalPotAmount, 
										m_pCurrentRound->m_nRoundPotAmount, 
										m_pCurrentRound->m_nRakeAmount);
			}
		}

	int nLoop = 0, nCt = pWinners->GetCount();
	while( nLoop < nCt ){
		PokerHandWinnerInfo* p = pWinners->GetAt(nLoop);
		if( !p /*|| !p->m_bWinner*/ ){
			nLoop ++;
			continue;
			}
		PokerHandWinnerInfo* pWinner	= new PokerHandWinnerInfo();
		p->Copy(pWinner);
		m_listWinners.Add(pWinner);
		nLoop ++;
		}
	}

void
PokerHandHistory::FinishHand2(EnumerableObject<PokerHandWinnerInfo>* pWinners, PokerTableInfo* pTableInfo, int nHandFinishSec, 
							EnumerableObject<PokerHandCards>* pListHandCards,
							EnumerableObject<PokerPotAmountDetail>* pListPots
							){
	if( pTableInfo ){
		m_cTableCard1		= pTableInfo->m_cCard1;
		m_cTableCard2		= pTableInfo->m_cCard2;
		m_cTableCard3		= pTableInfo->m_cCard3;
		m_cTableCard4		= pTableInfo->m_cCard4;
		m_cTableCard5		= pTableInfo->m_cCard5;
		}
	m_cHandFinishStartSec	= (char)nHandFinishSec;

	
	if( m_pCurrentRound && !m_pCurrentRound->m_nTotalPotAmount ){
		if( m_pCurrentRound->m_cRoundType == PokerRoundType::Showdown ){
			m_pCurrentRound->m_nTotalPotAmount	= pTableInfo->GetTotalPotAmount();
			m_pCurrentRound->m_nRoundPotAmount	= 0;
			m_pCurrentRound->m_nRakeAmount		= pTableInfo->GetRakeAmount();
			}
		else{
			pTableInfo->GetRoundAmounts((PokerRoundType)m_pCurrentRound->m_cRoundType, 
										m_pCurrentRound->m_nTotalPotAmount, 
										m_pCurrentRound->m_nRoundPotAmount, 
										m_pCurrentRound->m_nRakeAmount);
			}
		}

	// Update best hand cards class.
	if( pListHandCards ){
		for(int i=0; i<pListHandCards->GetCount(); i++){
			PokerHandCards* pCards = pListHandCards->GetAt(i);
			if( pCards ){
				PokerTableSeatInfo* pSeat = m_listSeats.GetAt((int)pCards->m_btSeat);
				if( pSeat ){
					if( pSeat->GetCard1() == -2 && pCards->m_btCard1 > -1 )
						pSeat->SetCard1((pCards->m_btCard1 + 3)*-1);
					if( pSeat->GetCard2() == -2 && pCards->m_btCard2 > -1 )
						pSeat->SetCard2((pCards->m_btCard2 + 3)*-1);
					if( pSeat->GetCard3() == -2 && pCards->m_btCard3 > -1 )
						pSeat->SetCard3((pCards->m_btCard3 + 3)*-1);
					if( pSeat->GetCard4() == -2 && pCards->m_btCard4 > -1 )
						pSeat->SetCard4((pCards->m_btCard4 + 3)*-1);
					pSeat->SetHandCardsClass(pCards->m_cBestCardsClass);
					}
				}
			}

		m_listHandCards.RemoveAll(true);
		pListHandCards->Copy_Into(&m_listHandCards);
		}

	// Update pots.
	if( pListPots ){
		m_listPots.RemoveAll(true);
		pListPots->Copy_Into(&m_listPots);
		}

	int nLoop = 0, nCt = pWinners->GetCount();
	while( nLoop < nCt ){
		PokerHandWinnerInfo* p = pWinners->GetAt(nLoop);
		if( !p /*|| !p->m_bWinner*/ ){
			nLoop ++;
			continue;
			}
		PokerHandWinnerInfo* pWinner	= new PokerHandWinnerInfo();
		p->Copy(pWinner);
		m_listWinners.Add(pWinner);
		nLoop ++;
		}
	}

void
PokerHandHistory::StartRound(PokerRoundType roundType, EnumerableObject<PokerHandCards>* pListHandCards, 
							EnumerableObject<PokerPotAmountDetail>* pListPots, INT_CHIPS nTotalPotAmount, UINT nRakeAmount){
	PokerHandRound* pRound			= new PokerHandRound();
	pRound->m_cRoundType			= roundType;
	pRound->m_nTotalPotAmount_Start	= nTotalPotAmount;	// Total pot amount.
	pRound->m_nRakeAmount_Start		= nRakeAmount;		// Rake amount.
	pRound->m_nRakeAmount			= 0;
	pRound->m_nRoundPotAmount		= 0;
	pRound->m_nTotalPotAmount		= 0;
	m_listRounds.Add(pRound);
	m_pCurrentRound				= pRound;

	// Copy pots detailed list.
	if( pListPots )
		pListPots->Copy_Into(&pRound->m_listPots);

	// Update best hand cards class.
	if( pListHandCards ){
		for(int i=0; i<pListHandCards->GetCount(); i++){
			PokerHandCards* pCards = pListHandCards->GetAt(i);
			if( pCards ){
				PokerTableSeatInfo* pSeat = m_listSeats.GetAt((int)pCards->m_btSeat);
				if( pSeat ){
					pSeat->SetHandCardsClass(pCards->m_cBestCardsClass);
					}

				// Save best hand cards.
				if( pCards->m_cBestCardsClass != PokerHandCardsClass::CardsClass_None && pCards->m_cBestCard1 != -1 ){
					PokerHandBestCards* pBestHandCards	= new PokerHandBestCards();
					pBestHandCards->m_btSeat			= pCards->m_btSeat;
					pBestHandCards->m_cBestCard1		= pCards->m_cBestCard1;
					pBestHandCards->m_cBestCard2		= pCards->m_cBestCard2;
					pBestHandCards->m_cBestCard3		= pCards->m_cBestCard3;
					pBestHandCards->m_cBestCard4		= pCards->m_cBestCard4;
					pBestHandCards->m_cBestCard5		= pCards->m_cBestCard5;
					pBestHandCards->m_cBestCardsClass	= pCards->m_cBestCardsClass;
					pRound->m_listHandBestCards.Add(pBestHandCards);
					}
				}
			}
		}
	}

void
PokerHandHistory::FinishRound(INT_CHIPS nTotalPotAmount, INT_CHIPS nPotAmount, UINT nRakeAmount){
	if( m_pCurrentRound == NULL ) return;
	m_pCurrentRound->m_nRakeAmount		= nRakeAmount;
	m_pCurrentRound->m_nRoundPotAmount	= nPotAmount;
	m_pCurrentRound->m_nTotalPotAmount	= nTotalPotAmount;
	m_pCurrentRound						= NULL;
	}

void
PokerHandHistory::FinishRound2(){
	if( m_pCurrentRound == NULL ) return;
	m_pCurrentRound	= NULL;
	}

bool
PokerHandHistory::UpdateCurrentRound(INT_CHIPS nTotalPotAmount, INT_CHIPS nPotAmount, UINT nRakeAmount){
	if( m_pCurrentRound == NULL ) return false;
	if( nTotalPotAmount > -1 )
		m_pCurrentRound->m_nRakeAmount		= nRakeAmount;
	if( nPotAmount > -1 )
		m_pCurrentRound->m_nRoundPotAmount	= nPotAmount;
	if( nTotalPotAmount > -1 )
		m_pCurrentRound->m_nTotalPotAmount	= nTotalPotAmount;
	return false;
	}

void
PokerHandHistory::Action(int nSeat, PokerActionTypes actionType, PokerSeatStatus status, INT_CHIPS nAmount, INT_CHIPS nBalance, INT_CHIPS nPotAmount, INT_CHIPS nTotalPotAmount, int nTimeSec, EnumerableObject<PokerHandCards>* pCardsShown /*= NULL*/){
	if( m_pCurrentRound == NULL ) return;
	if( actionType == PokerActionTypes::ShowCards ){
		PokerTableSeatInfo* pSeat = m_listSeats.GetAt(nSeat);
		if( pSeat && 
			((pSeat->GetCard1() >= 0 || pSeat->GetCard1() < -2) ||
			(pSeat->GetCard2() >= 0 || pSeat->GetCard2() < -2) ||
			(pSeat->GetCard3() >= 0 || pSeat->GetCard3() < -2) ||
			(pSeat->GetCard4() >= 0 || pSeat->GetCard4() < -2)) ){
			return; // Cards are visible so that action is useless.
			}
		}

	PokerHandAction* pNewAction		= new PokerHandAction;
	pNewAction->m_cSeat				= (char)nSeat;
	pNewAction->m_cAction			= (char)actionType;
	pNewAction->m_cSeatStatus		= (char)status;
	pNewAction->m_nActAmount		= nAmount;
	pNewAction->m_nPotAmount		= nPotAmount;
	pNewAction->m_nTotalPotAmount	= nTotalPotAmount;
	pNewAction->m_nBalanceAmount	= nBalance;
	pNewAction->m_cTimeSec			= (char)nTimeSec;

	if( pCardsShown ){
		for(int i=0; i<pCardsShown->GetCount(); i++){
			PokerHandCards* pCards = pCardsShown->GetAt(i);
			if( pCards ){
				PokerHandCards* pCardsCopy = new PokerHandCards();
				pCards->Copy(pCardsCopy);
				pNewAction->m_listCardsShown.Add(pCardsCopy);

				PokerTableSeatInfo* pSeat = m_listSeats.GetAt((int)pCards->m_btSeat);
				if( pSeat ){
					if( pSeat->GetCard1() == -2 && pCardsCopy->m_btCard1 > -1 )
						pSeat->SetCard1((pCardsCopy->m_btCard1 + 3)*-1);
					if( pSeat->GetCard2() == -2 && pCardsCopy->m_btCard2 > -1 )
						pSeat->SetCard2((pCardsCopy->m_btCard2 + 3)*-1);
					if( pSeat->GetCard3() == -2 && pCardsCopy->m_btCard3 > -1 )
						pSeat->SetCard3((pCardsCopy->m_btCard3 + 3)*-1);
					if( pSeat->GetCard4() == -2 && pCardsCopy->m_btCard4 > -1 )
						pSeat->SetCard4((pCardsCopy->m_btCard4 + 3)*-1);
					}
				}
			}
		}
	m_pCurrentRound->m_roundActions.Add(pNewAction);
	}

bool
PokerHandHistory::ChangeLastActionAmounts(int nSeat, INT_CHIPS nAmountNew, INT_CHIPS nBalanceNew, INT_CHIPS nPotAmountNew, INT_CHIPS nTotalPotAmountNew){
	if( !m_pCurrentRound )
		return false;

	for(int i=m_pCurrentRound->m_roundActions.GetCount() - 1; i>=0; i--){
		PokerHandAction* pAction = m_pCurrentRound->m_roundActions[i];
		if( pAction && pAction->m_cSeat == (char)nSeat && pAction->m_nActAmount > 0 ){
			pAction->m_nActAmount		= nAmountNew;
			pAction->m_nPotAmount		= nPotAmountNew;
			pAction->m_nTotalPotAmount	= nTotalPotAmountNew;
			pAction->m_nBalanceAmount	= nBalanceNew;
			return true;
			}
		}
	return false;
	}

void
PokerHandHistory::ActionWithHandCard(int nSeat, PokerActionTypes actionType, INT_CHIPS nAmount, INT_CHIPS nBalance, INT_CHIPS nPotAmount, INT_CHIPS nTotalPotAmount, int nTimeSec, PokerHandCards* pHandCards){
	if( m_pCurrentRound == NULL ) return;

	if( actionType == PokerActionTypes::ShowCards ){
		PokerTableSeatInfo* pSeat = m_listSeats.GetAt(nSeat);
		if( pSeat && pSeat->GetCardsShowStatus()/*
			((pSeat->GetCard1() >= 0 || pSeat->GetCard1() < -2) ||
			(pSeat->GetCard2() >= 0 || pSeat->GetCard2() < -2) ||
			(pSeat->GetCard3() >= 0 || pSeat->GetCard3() < -2) ||
			(pSeat->GetCard4() >= 0 || pSeat->GetCard4() < -2))*/ ){
			return; // Cards are visible so that action is useless.
			}
		}

	PokerHandAction* pNewAction		= new PokerHandAction;
	pNewAction->m_cSeat				= (char)nSeat;
	pNewAction->m_cAction			= (char)actionType;
	pNewAction->m_nActAmount		= nAmount;
	pNewAction->m_nPotAmount		= nPotAmount;
	pNewAction->m_nTotalPotAmount	= nTotalPotAmount;
	pNewAction->m_nBalanceAmount	= nBalance;
	pNewAction->m_cTimeSec			= (char)nTimeSec;

	if( pHandCards ){
		PokerHandCards* pCardsCopy = new PokerHandCards();
		pHandCards->Copy(pCardsCopy);
		pNewAction->m_listCardsShown.Add(pCardsCopy);
		
		PokerTableSeatInfo* pSeat = m_listSeats.GetAt(nSeat);
		if( pSeat ){
			if( pSeat->GetCard1() == -2 && pCardsCopy->m_btCard1 > -1 )
				pSeat->SetCard1((pCardsCopy->m_btCard1 + 3)*-1);
			if( pSeat->GetCard2() == -2 && pCardsCopy->m_btCard2 > -1 )
				pSeat->SetCard2((pCardsCopy->m_btCard2 + 3)*-1);
			if( pSeat->GetCard3() == -2 && pCardsCopy->m_btCard3 > -1 )
				pSeat->SetCard3((pCardsCopy->m_btCard3 + 3)*-1);
			if( pSeat->GetCard4() == -2 && pCardsCopy->m_btCard4 > -1 )
				pSeat->SetCard4((pCardsCopy->m_btCard4 + 3)*-1);
			}
		}
	m_pCurrentRound->m_roundActions.Add(pNewAction);
	}

void
PokerHandHistory::ClearHistory(){
	ZeroInit();
	m_cDealer			= -1;
	m_cSmallBlind		= -1;
	m_cBigBlind			= -1;

	m_cTableCard1		= -1;
	m_cTableCard2		= -1;
	m_cTableCard3		= -1;
	m_cTableCard4		= -1;
	m_cTableCard5		= -1;

	m_pCurrentRound		= NULL;

	m_listSeats			.RemoveAll();
	m_listRounds		.RemoveAll();
	m_listWinners		.RemoveAll();
	}

int
PokerHandHistory::GetWinnersSeperatedBySemiCollon(_String& sWinners){
	int		nWinnersCt = 0;
	_String sTemp;

	for(int i=0; i<m_listWinners.GetCount(); i++){
		PokerHandWinnerInfo* pWinner = m_listWinners[i];
		if( pWinner && pWinner->m_bWinner ){
			PokerTableSeatInfo* pSeatInfo = m_listSeats.GetAt(pWinner->m_cSeat);
			if( pSeatInfo ){
				sTemp.Format(_T("%s;"), pSeatInfo->GetUserName().GetBuffer());
				sWinners += sTemp;
				}
			nWinnersCt ++;
			}
		}

	if( nWinnersCt > 0 )
		sWinners.Delete(sWinners.GetLength() - 1);
	return nWinnersCt;
	}

INT_CHIPS
PokerHandHistory::GetPotAmount(){
	INT_CHIPS	nPotAmount = 0;
	for(int i=0; i<m_listWinners.GetCount(); i++){
		PokerHandWinnerInfo* pWinner = 	m_listWinners.GetAt(i);
		if( pWinner && pWinner->m_bWinner ){
			nPotAmount += pWinner->m_nWonAmount;
			}
		}
	return nPotAmount;
	}

bool
PokerHandHistory::GetMyHoleCards(int nUserId, _String& sMyHoleCards){
	for(int i=0; i<m_listSeats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_listSeats.GetAt(i);
		if( pSeat && pSeat->GetUserId() == nUserId ){
			sMyHoleCards.Format( _T("%s%s%s%s"),
				PokerHandCards::GetCardNameByIndex(pSeat->GetCard1()).GetBuffer(),
				PokerHandCards::GetCardNameByIndex(pSeat->GetCard2()).GetBuffer(),
				PokerHandCards::GetCardNameByIndex(pSeat->GetCard3()).GetBuffer(),
				PokerHandCards::GetCardNameByIndex(pSeat->GetCard4()).GetBuffer());
			return true;
			}
		}
	return false;
	}

bool
PokerHandHistory::SetHandCards(int nUserId, int nCard1, int nCard2, int nCard3, int nCard4){
	if( nUserId <= 0 ) return false;
	for(int i=0; i<m_listSeats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_listSeats.GetAt(i);
		if( pSeat && pSeat->GetUserId() == nUserId ){
			pSeat->SetCard1(nCard1);
			pSeat->SetCard2(nCard2);
			pSeat->SetCard3(nCard3);
			pSeat->SetCard4(nCard4);
			return true;
			}
		}
	return false;
	}
