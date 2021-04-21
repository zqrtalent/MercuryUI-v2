// PokerTableInfo.cpp : implementation file
//

#include "../../Array/AutoSortedArray.h"
#include "../../UI/Poker/PokerGUI.h"
#include "../../Protocol/_PokerDefines.h"
#include "PokerTableSeatInfo.h"
#include "PokerTableInfo.h"
#include "../../Protocol/PokerPacket.h"

#ifdef _SERVER
#include "../HandWinnerCalculator.h"
#include "../OmahaWinnerCalculator.h"
#include "../OmahaHiLoWinnerCalculator.h"
#include "../HoldemWinnerCalculator.h"
#endif

PokerTableInfo::PokerTableInfo(){
	m_nTableId				= 0;	
	m_nTournamentId			= 0;
	m_bTournamentTable		= false;
	
#ifdef _SERVER
	ClearStatInfo();
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
	DECLARE_VAR(_T("TotalPotAmount"),	_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("MainPotAmount"),	_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nMainPotAmount), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),		_T(""),		VariableType::VT_Int32, offsetof(PokerTableInfo,m_nRakeAmount), false, false, false)
	DECLARE_VAR(_T("TimeBankSec"),		_T(""),		VariableType::VT_Int16, offsetof(PokerTableInfo,m_shTimeBankSec), false, false, false)
	DECLARE_VAR(_T("ActiveSeat"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cActiveSeat), false, false, false)
	DECLARE_VAR(_T("DealerSeat"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cDealerSeat), false, false, false)
	DECLARE_VAR(_T("SBSeat"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cSBSeat), false, false, false)
	DECLARE_VAR(_T("BBSeat"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_cBBSeat), false, false, false)
	DECLARE_VAR(_T("RegularActTimeSec"),_T(""),		VariableType::VT_Int8, offsetof(PokerTableInfo,m_btRegularActTimeSec), false, false, false)
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
				// Copy session from one to another.
				pSeatInto->SetSession(pSeat->GetSession(false));
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
	int nLoop	= 0;
	int nRet	= 0;
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
	int nLoop	= 0, nRet	= 0;
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
	int nLoop	= 0, nRet	= 0;
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
	m_nPlayedHandsCt	= 0;
	m_dAvgPotAmount		= 0.0;
	m_dAvgPlayers		= 0.0;
	m_dAvgHandsPerHour	= 0.0;
	m_dPlayerFlop		= 0.0;
	}

int
PokerTableInfo::GetSidePots(AutoSortedArray& arrPotAmountBySeatIndex, int& nMaxSeatPot){
	int nLoop = 0, nCt = m_cMaxPlayersCt, nSidePotCt = 0;
	arrPotAmountBySeatIndex.DeleteAll();
	nMaxSeatPot	= 0;

	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat ){
			nLoop ++;
			continue;
			}

		PokerSeatStatus status = pSeat->GetStatus();
		int	nTotalPot = pSeat->GetTotalPot();
		if( status == PokerSeatStatus::All_In ){
			if( arrPotAmountBySeatIndex.IndexOf((void*)nTotalPot) == -1 )
				nSidePotCt ++;
			arrPotAmountBySeatIndex.Add((void*)nTotalPot, (void*)nLoop);
			}

		if( status == PokerSeatStatus::All_In || status == PokerSeatStatus::InPlay )
			nMaxSeatPot = max(nMaxSeatPot, nTotalPot);
		nLoop ++;
		}
	return nSidePotCt;
	}

int
PokerTableInfo::NormalizeSeatPots(EnumerableObject<PokerSeatAmountInfo>* pSeatsAmountChanged){
	int nLoop = 0, nCt = m_cMaxPlayersCt, nSidePotCt = 0, nMaxSeatPot = 0;
	AutoSortedArray arrPotAmountBySeatIndex;
	arrPotAmountBySeatIndex.SetCompareFunction(AutoSortedArray::CompareDesc);

	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat ){
			nLoop ++;
			continue;
			}

		PokerSeatStatus status		= pSeat->GetStatus();
		int				nPotAmount	= pSeat->GetPotAmount();
		if( (status == PokerSeatStatus::All_In || status == PokerSeatStatus::InPlay) && nPotAmount > 0 ){
			arrPotAmountBySeatIndex.Add((void*)nPotAmount, (void*)nLoop);
			nMaxSeatPot = max(nMaxSeatPot, nPotAmount);
			}
		nLoop ++;
		}

	if( !arrPotAmountBySeatIndex.GetCount() )
		return 0;

	if( arrPotAmountBySeatIndex.GetCount() > 1 ){
		int nAmountDifference = ((int)arrPotAmountBySeatIndex.Get(0)) - ((int)arrPotAmountBySeatIndex.Get(1));
		if( nAmountDifference > 0 ){
			int nSeat			= (int)arrPotAmountBySeatIndex.GetData(0);
			int nTotalPotNew	= m_nTotalPotAmount - nAmountDifference;
			int nSeatTotalPotNew= GetSeatTotalPot(nSeat) - nAmountDifference;
			int nPotAmountNew	= GetPotAmount(nSeat) - nAmountDifference;
			int nBalanceNew		= GetBalance(nSeat) + nAmountDifference;

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
		int nSeat				= (int)arrPotAmountBySeatIndex.GetData(0);
		int nAmountDifference	= ((int)arrPotAmountBySeatIndex.Get(0));
		int nTotalPotNew		= m_nTotalPotAmount - nAmountDifference;
		int nSeatTotalPotNew	= GetSeatTotalPot(nSeat) - nAmountDifference;
		int nPotAmountNew		= GetPotAmount(nSeat) - nAmountDifference;
		int nBalanceNew			= GetBalance(nSeat) + nAmountDifference;

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
		pDetail->m_wRakeAmount			= (WORD)CalculateRakeAmount(pDetail->m_nAmount);
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
PokerTableInfo::CalculateRakeAmount(int nTotalPotAmount){
	int nRakeAmount = 0;
	if( m_nTournamentId > 0 )
		return 0; // Tournament.
	if( m_fRakePercent > 0.0f && m_round > PokerRoundType::PreFlop ){
		nRakeAmount		= (int)(nTotalPotAmount*m_fRakePercent);
		nRakeAmount		= min(max(0, nRakeAmount), m_nRakeAmountMax);
		}
	return nRakeAmount;
	}

int
PokerTableInfo::CalculateSidePots(List<SidePotInfoStruct>& listSidePots){
	if( m_nMainPotAmount <= 0 ){
		listSidePots.RemoveAll(true);
		return 0;
		}

	AutoSortedArray arrSeatTotalPots, arrFoldedSeatAmounts;
	int				nAmountFoldedSeats  = 0;
	arrFoldedSeatAmounts.SetCompareFunction(AutoSortedArray::CompareEmpty);

	for(int i=0; i<m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_seats[i];
		if( !pSeat ) continue;

		if( pSeat->GetTotalPot() > 0 ){ 
			PokerSeatStatus status = pSeat->GetStatus();
			if( (status == PokerSeatStatus::All_In || status == PokerSeatStatus::InPlay) )
				arrSeatTotalPots.Add((void*)pSeat->GetTotalPot(), (void*)pSeat->GetSeat());
			if( status == PokerSeatStatus::Folded ){
				arrFoldedSeatAmounts.Add((void*)pSeat->GetTotalPot(), (void*)pSeat->GetSeat());
				nAmountFoldedSeats	+= pSeat->GetTotalPot();
				}
			}
		}

	listSidePots.RemoveAll(true);
	if( arrSeatTotalPots.GetCount() > 0 ){
		int				nPrevSeatPotAmount	= 0;
		for(int i=0; i<arrSeatTotalPots.GetCount(); i++){
			int nSeatTotalPotAmount				= ((int)arrSeatTotalPots.Get(i)) - nPrevSeatPotAmount;
			// Add new side pot.
			if( nSeatTotalPotAmount > 0 ){
				int nSeatCtBelongsTo			= arrSeatTotalPots.GetCount() - i;
				int nSidePotAmount				= nSeatTotalPotAmount * nSeatCtBelongsTo;

				// Include folded amounts into pot. {{
				if( nAmountFoldedSeats > 0 ){
					for(int loop=0; loop<arrFoldedSeatAmounts.GetCount(); loop++){
						int nAmount = (int)arrFoldedSeatAmounts[loop];
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
						arrFoldedSeatAmounts.Set(loop, (void*)nAmount, arrFoldedSeatAmounts.GetData(loop));
						}
					}
				// }}

				SidePotInfoStruct* pPotInfo		= new SidePotInfoStruct;
				pPotInfo->nPotAmount			= nSidePotAmount;
				pPotInfo->nSeatsCtBelongsTo		= nSeatCtBelongsTo;
				listSidePots.Add(pPotInfo);

				// Copy pot belonging seat indexes.
				int nSeatCt = 0;
				for(int j=i; j<arrSeatTotalPots.GetCount(); j++){
					pPotInfo->nArrSeatsBelongsTo[nSeatCt] = (int)arrSeatTotalPots.GetData(j); // Seat index.
					nSeatCt ++;
					}
				}

			nPrevSeatPotAmount = (int)arrSeatTotalPots.Get(i);
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
		if( pSeat && (pSeat->GetStatus() == PokerSeatStatus::All_In || pSeat->GetStatus() == PokerSeatStatus::InPlay) ){
			PokerHandCards* pHandCards		= new PokerHandCards();
			pHandCards->m_btSeat			= (BYTE)nLoop;
			pHandCards->m_btCard1			= (BYTE)pSeat->GetCard1();
			pHandCards->m_btCard2			= (BYTE)pSeat->GetCard2();
			pHandCards->m_btCard3			= (BYTE)pSeat->GetCard3();
			pHandCards->m_btCard4			= (BYTE)pSeat->GetCard4();

			arrCardsIdx.empty();
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
				pArrAskShowMuckCards->push_back(nAskSeat);
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

	if( pSeat && (pSeat->GetStatus() == PokerSeatStatus::All_In || pSeat->GetStatus() == PokerSeatStatus::InPlay) ){
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
#endif

bool
PokerTableInfo::SetAvatarInfo(int nSeat, PokerAvatar* pAvatar){
	if( nSeat < 0 || nSeat >= m_cMaxPlayersCt || !pAvatar )
		return false;
	if( m_listSeatAvatars.GetCount() < m_cMaxPlayersCt ){
		for(int i=m_listSeatAvatars.GetCount(); i<m_cMaxPlayersCt; i++){
			m_listSeatAvatars.Add(NULL);
			}
		}

	PokerAvatar* pAvatarInfo = new PokerAvatar();
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
	int nLoop	= 0, nRet	= 0;
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

	for(int i=0; i<nSeat; i++ ){
		if( m_seats.GetAt(i) != NULL ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return m_seats.GetAt(i);
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

	for(int i=nCount-1; i>nSeat; i-- ){
		if( m_seats.GetAt(i) != NULL ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return m_seats.GetAt(i);
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

	for(int i=nSeat; i<nCount; i++ ){
		if( m_seats.GetAt(i) != NULL && m_seats.GetAt(i)->GetStatus() == status ){
			if( pSeatIndex )
				*pSeatIndex = i;
			return m_seats.GetAt(i);
			}
		}

	if( nSeat > 1 ){
		nSeat --;
		for(int i=0; i<nSeat; i++ ){
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
	if( m_gameStatus != PokerGameStatus::Started )
		return NULL;

	int nAllIn		= CountSeatsByStatus(PokerSeatStatus::All_In);
	int nFolded		= CountSeatsByStatus(PokerSeatStatus::Folded);
	int nInPlay		= CountSeatsByStatus(PokerSeatStatus::InPlay);
	bIsRoundEnded	= false;
	bOpenSeatCards	= false;

	if( nSeatPrev == -1 ){
		if( m_round == PokerRoundType::PreFlop )
			nSeatPrev = ((int)m_cBBSeat);
		else
			nSeatPrev = ((int)m_cDealerSeat);
		}

	PokerTableSeatInfo* pSeat = GetNextSeatInfoByStatus(nSeatPrev, PokerSeatStatus::InPlay, &nSeatIndex);
	if( pSeat ){
		int					nMaxPot		= 0;
		int					nMinPot		= 0;
		int					nPot		= pSeat->GetPotAmount();
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
	bIsRoundEnded	= (nAllIn > 0);
	nSeatIndex		= -1;
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetMaxPotAmount(int& nPotAmount, int& nMinPot){
	int					nCt			= m_seats.GetCount();
	int					nLoop		= 0;
	PokerTableSeatInfo* pSeatMaxPot = NULL;

	nMinPot = 0x7fffffff;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || (pSeat->GetStatus() != PokerSeatStatus::InPlay && pSeat->GetStatus() != PokerSeatStatus::All_In) ){
			nLoop ++;
			continue;
			}

		if( nMinPot > pSeat->GetPotAmount() )
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
	int	nLoop	= 0;
	int nCt		= m_seats.GetCount();
	int nResult	= 0;
	
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( pSeat && pSeat->GetStatus() == status )
			nResult ++;
		nLoop ++;
		}
	return nResult;
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
PokerTableInfo::GetBBSeat(int nSBSeat, int* pSeatIndex /*= NULL*/){
	if( m_gameStatus != PokerGameStatus::Started ){
		PokerTableSeatInfo* pSeatInfo		= NULL;
		int					nBBSeatIndex	= -1;
		int					nSeatCt			= GetSittedCt() + GetSeatCtByStatus(PokerSeatStatus::All_In);

		if( nSeatCt > 1 ){
			int nLoop = nSBSeat;
			while( TRUE ){
				pSeatInfo = GetNextSeatInfo(nLoop, &nBBSeatIndex);
				if( !pSeatInfo ){
					nBBSeatIndex = -1;
					break;
					}

				if( m_bTournamentTable ){ // Tournament table
					if( pSeatInfo->GetStatus() == PokerSeatStatus::SittedOut ||
						pSeatInfo->GetStatus() == PokerSeatStatus::Sitted ||
						pSeatInfo->GetStatus() == PokerSeatStatus::All_In ||
						pSeatInfo->GetStatus() == PokerSeatStatus::WaitingForBB ){
						nBBSeatIndex = pSeatInfo->GetSeat();
						break;
						}
					}
				else{ // Cash table
					if( pSeatInfo->GetStatus() == PokerSeatStatus::WaitingForBB ||
						pSeatInfo->GetStatus() == PokerSeatStatus::All_In ||
						pSeatInfo->GetStatus() == PokerSeatStatus::Sitted ){
						nBBSeatIndex = pSeatInfo->GetSeat();
						break;
						}
					}
				nLoop = pSeatInfo->GetSeat();
				}
			}
		else
			nBBSeatIndex = -1;
	
		if( pSeatIndex )
			*pSeatIndex = nBBSeatIndex;
		return pSeatInfo;
		}
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetSBSeat(int nDealerSeat, int* pSeatIndex /*= NULL*/){
	if( m_gameStatus != PokerGameStatus::Started ){
		PokerTableSeatInfo* pSeatInfo = NULL;
		int nSeatCt			= GetSittedCt();
		int nSBSeatIndex	= ((int)m_cBBSeat);
		if( nSBSeatIndex == -1 ){
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
				if( m_bTournamentTable )
					pSeatInfo = GetNextSeatedSeatInfo(nDealerSeat, &nSBSeatIndex); // Small blind.
				else{
					//Old
					//pSeatInfo		= GetNextSeatInfoByStatus(-1, PokerSeatStatus::Sitted, &nSBSeatIndex);			// Dealer as Small Blind
					pSeatInfo		= GetSeatInfo(nDealerSeat);
					nSBSeatIndex	= nDealerSeat;
					if( pSeatInfo == NULL || pSeatInfo->GetStatus() != PokerSeatStatus::Sitted ){
						nSBSeatIndex = -1;
						}
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
			else{	// Cash table.
				if( pSeatInfo == NULL || pSeatInfo->GetStatus() != PokerSeatStatus::Sitted )
					nSBSeatIndex = -1;
				}
			}
		if( pSeatIndex )
			*pSeatIndex = nSBSeatIndex;
		return pSeatInfo;
		}
	return NULL;
	}

PokerTableSeatInfo*
PokerTableInfo::GetDealerSeat(int* pSeatIndex /*= NULL*/){
	if( m_gameStatus == PokerGameStatus::Starting ){
		PokerTableSeatInfo* pSeatInfo = NULL;
		int nSeatCt = GetSittedCt();

		int nDealer = (m_cDealerSeat == m_cSBSeat && m_cDealerSeat != -1 && nSeatCt == 2) ? m_cBBSeat : m_cSBSeat;
		if( nDealer == -1 ){
			/* Random dealer seat. */
			AutoSortedArray arrSeats;
			int nLoop	= 0;
			int nCt		= (int)m_cMaxPlayersCt;

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
					if( pSeat && pSeat->GetStatus() == PokerSeatStatus::Sitted )
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

			}
		if( pSeatIndex )
			*pSeatIndex = nDealer;
		pSeatInfo = GetSeatInfo(nDealer);
		return pSeatInfo;
		}
	return NULL;
	}

int
PokerTableInfo::GetPayBBAndPlaySeats(int nBBSeat, int nDealerSeat, CDWordArray& arrPayBBSeats){
	arrPayBBSeats.empty();
	if( nBBSeat == -1 || nDealerSeat == -1 )
		return 0;
	
	// Seats between BB and Dealer who is ready to pay BB and play.
	int nSeat = nBBSeat;
	while( TRUE ){
		PokerTableSeatInfo* pSeat = GetNextSeatInfo(nSeat);
		if( !pSeat || !IsBetweenSeats(pSeat->GetSeat(), nBBSeat, nDealerSeat) )
			break;
		if( pSeat->GetStatus() == PokerSeatStatus::PayBBAndPlay )
			arrPayBBSeats.push_back((DWORD)pSeat->GetSeat());
		nSeat = pSeat->GetSeat();
		}
	return (int)arrPayBBSeats.size();
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

int
PokerTableInfo::SetSeatStatus(PokerSeatStatus destStatus, PokerSeatStatus status){
	int nSeat		= 0;
	int nCt			= m_seats.GetCount();
	int nChanged	= 0;

	while( nSeat < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
		if( !pSeat ){
			nSeat ++;
			continue;
			}

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

int
PokerTableInfo::GetBalance(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return 0;
	return pSeatInfo->GetBalance();
	}

bool
PokerTableInfo::SetBalance(int nSeat, int nAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetBalance(nAmount);
	return true;
	}

void
PokerTableInfo::SetHandWonState(int nSeat, bool bWon){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo) return;
	pSeatInfo->SetHandWonState(bWon);
	}

int
PokerTableInfo::GetSeatAmounts(EnumerableObject<PokerSeatAmountInfo>* seatAmounts){
	int nLoop = 0, nCt = m_seats.GetCount(), nResult = 0;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty ){
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

std::string
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

int
PokerTableInfo::GetAddedAmount(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return 0;
	return pSeatInfo->GetAddedAmount();
	}

bool
PokerTableInfo::Add_AddedAmount(int nSeat, int nAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetAddedAmount(pSeatInfo->GetAddedAmount() + nAmount);
	return true;
	}

bool
PokerTableInfo::SetAddedAmount(int nSeat, int nAmount){
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
		pSeatInfo->SetTotalPot	(0);
		pSeatInfo->SetPotAmount	(0);
		nLoop ++; nRet ++;
		}
	return nRet;
	}

bool
PokerTableInfo::AddPotAmount(int nSeat, int nAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;

	pSeatInfo->SetPotAmount	(pSeatInfo->GetPotAmount() + nAmount);
	pSeatInfo->AddTotalPot	(nAmount);
	AddTotalPotAmount		(nAmount);
	return true;
	}

int
PokerTableInfo::GetPotAmount(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return 0;
	return pSeatInfo->GetPotAmount();
	}

bool
PokerTableInfo::SetPotAmount(int nSeat, int nPotAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetPotAmount(nPotAmount);
	return true;
	}

int
PokerTableInfo::GetSeatTotalPot(int nSeat){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return 0;
	return pSeatInfo->GetTotalPot();
	}

bool
PokerTableInfo::SetSeatTotalPot(int nSeat, int nTotalPotAmount){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetTotalPot(nTotalPotAmount);
	return true;
	}

bool
PokerTableInfo::GetBetAmounts(int nSeat, int& nMinBet, int& nMaxBet){
	int					nCallAmount = 0;
	PokerTableSeatInfo* pInfoDest	= m_seats.GetAt(nSeat);
	PokerTableSeatInfo* pInfo		= NULL;
	int					nLoop		= 0;
	int					nCt			= m_cMaxPlayersCt;
	int					nMaxPot		= 0; // Maximum entered pot amount.
	nMinBet							= 0;
	nMaxBet							= 0;

	if( !pInfoDest || pInfoDest->GetStatus() != PokerSeatStatus::InPlay )
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
			nMinBet = 2*nMaxPot - pInfoDest->GetPotAmount();
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

			if( nMinBet > pInfoDest->GetBalance() )
				return false;
			return true;
			}

		// Pot Limit.
		if( (int)(limit&GameSubType::Holdem_PotLimit) == (int)GameSubType::Holdem_PotLimit ){
			int nCallAmount = (nMaxPot - pInfoDest->GetPotAmount());
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
				}
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

	return false;
	}

int
PokerTableInfo::GetCallAmount(int nSeat){
	int					nCallAmount = 0;
	PokerTableSeatInfo* pInfoDest	= m_seats.GetAt(nSeat);
	PokerTableSeatInfo* pInfo		= NULL;
	int					nLoop		= 0;
	int					nCt			= m_cMaxPlayersCt;
	int					nMaxPot		= 0; // Maximum entered pot amount.

	if( !pInfoDest || pInfoDest->GetStatus() != PokerSeatStatus::InPlay )
		return 0;

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
			pArrSeatInfos->push_back(pSeat);
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
			arrSeats.push_back((void*)pSeat);
		}
	return (int)arrSeats.size();
	}

int
PokerTableInfo::GetSitOutSeats(CDWordArray& arrSeatIndexes, bool bExcludeEmptyBalanceSeats /*= false*/){
	arrSeatIndexes.empty();
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
				arrSeatIndexes.push_back(pSeat->GetSeat());
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
				arrSeatIndexes.push_back(pSeat->GetSeat());
				}
			nLoop ++;
			}
		}
	return ((int)arrSeatIndexes.size());
	}

int
PokerTableInfo::GetEmptyBalanceSeats(CDWordArray& arrSeatIndexes){
	arrSeatIndexes.empty();
	int nLoop = 0, nCt = m_cMaxPlayersCt;
	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nLoop);
		if( !pSeat || pSeat->GetStatus() < PokerSeatStatus::Sitted ){
			nLoop ++;
			continue;
			}
		// Empty balance
//		if( pSeat->GetBalance() < m_nBigBlindAmount ){
		if( pSeat->GetBalance() <= 0 ){
			arrSeatIndexes.push_back(pSeat->GetSeat());
			}
		nLoop ++;
		}
	return ((int)arrSeatIndexes.size());
	}

void
PokerTableInfo::StartHand(int nDealerSeat, int nSBSeat, int nBBSeat){
	CPtrArray		arrInPlaySeats;
	GetSeatCtByStatus(PokerSeatStatus::InPlay, &arrInPlaySeats);
	GetSeatCtByStatus(PokerSeatStatus::All_In, &arrInPlaySeats);
	int				nPlayersCt	= arrInPlaySeats.size();
	int				nLoop		= 0;
	int				nCard		= 0;
	AutoSortedArray arrUnique;

	// Start round.
	StartRound(PokerRoundType::PreFlop);

	m_dTimeStartHand.SetCurrentTime();
	m_cDealerSeat	= nDealerSeat;
	m_cSBSeat		= nSBSeat;
	m_cBBSeat		= nBBSeat;
	m_gameStatus	= PokerGameStatus::Started;
	m_nPlayersCtStarted = 0;
	
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
	m_nTableCard1 = 45;
	m_nTableCard2 = 18;
	m_nTableCard3 = 51;
	m_nTableCard4 = 28;
	m_nTableCard5 = 7;
	m_nTableCard1 = CardIndexByName(_T("7S"));
	m_nTableCard2 = CardIndexByName(_T("QS"));
	m_nTableCard3 = CardIndexByName(_T("3H"));
	m_nTableCard4 = CardIndexByName(_T("KC"));
	m_nTableCard5 = CardIndexByName(_T("8S"));

	CDWordArray arrHandCards;
	//arrHandCards.Add(CardIndexByName(_T("AD")));
	//arrHandCards.Add(CardIndexByName(_T("AH")));
	//arrHandCards.Add(CardIndexByName(_T("KD")));
	//arrHandCards.Add(CardIndexByName(_T("KH")));
	//arrHandCards.Add(CardIndexByName(_T("QD")));
	//arrHandCards.Add(CardIndexByName(_T("QH")));
	//arrHandCards.Add(CardIndexByName(_T("JD")));
	//arrHandCards.Add(CardIndexByName(_T("JH")));
	//arrHandCards.Add(CardIndexByName(_T("TD")));
	//arrHandCards.Add(CardIndexByName(_T("TH")));
	//arrHandCards.Add(CardIndexByName(_T("9D")));
	//arrHandCards.Add(CardIndexByName(_T("9H")));

	arrHandCards.Add(45);
	arrHandCards.Add(8);
	arrHandCards.Add(45);
	arrHandCards.Add(21);
	

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
			}
		}
	m_nFlopPlayersCt = 0;
	}

void
PokerTableInfo::StartRound(PokerRoundType round){
	bool bPreFlop = false;
	switch(round){
		case PokerRoundType::PreFlop:{
			m_round				= round;
// Before start round player's should pay SB or BB or ANTE.
//			m_nTotalPotAmount	= 0;	
			m_nMainPotAmount	= 0;
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
			pRoundStruct->nTotalPotAmount			= 0;
			pRoundStruct->nRoundPotAmount			= 0;
			pRoundStruct->nRakeAmount				= 0;
			m_listRoundAmounts.Add(pRoundStruct);
			break;
			}
		case PokerRoundType::Flop:{
			PokerRoundAmountStruct* pRoundStruct	= new PokerRoundAmountStruct;
			pRoundStruct->round						= round;
			pRoundStruct->nTotalPotAmount			= m_nTotalPotAmount;
			pRoundStruct->nRoundPotAmount			= (m_nTotalPotAmount - m_nMainPotAmount);
			pRoundStruct->nRakeAmount				= max((int)(m_fRakePercent*m_nTotalPotAmount), 1);
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
			pRoundStruct->nRakeAmount				= max((int)(m_fRakePercent*m_nTotalPotAmount), 1);
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
			pRoundStruct->nRakeAmount				= max((int)(m_fRakePercent*m_nTotalPotAmount), 1);
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
	m_listPots.RemoveAll(true);
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

void
PokerTableInfo::FinishHand(bool bShowdown){
	// Calculate statustic info. {{
	if( m_nTotalPotAmount > 0 ){
		DateTime			dTimeFinish(true);
		int					nHandDurSec		= m_dTimeStartHand.GetDifferenceInSeconds(dTimeFinish);

		if( m_nPlayedHandsCt > 0 ){
			// Avg pot amount.
			double dPotAmount		= m_dAvgPotAmount * m_nPlayedHandsCt;
			dPotAmount				+= (double)(m_nTotalPotAmount / 100.0);
			m_dAvgPotAmount			= dPotAmount / (m_nPlayedHandsCt + 1);

			// Avg players count.
			double dPlayersCt		= m_dAvgPlayers*m_nPlayedHandsCt;
			dPlayersCt += (double)(m_nPlayersCtStarted);
			m_dAvgPlayers			= dPlayersCt / (m_nPlayedHandsCt + 1);
/*
			std::string s1, s2;
			s1.Format(_T("%02d-%02d-%04d %02d:%02d:%02d"), dTimeFinish.GetDay(), dTimeFinish.GetMonth(), dTimeFinish.GetYear(), 
				dTimeFinish.GetHour(), dTimeFinish.GetMinute(), dTimeFinish.GetSecond());
			s2.Format(_T("%02d-%02d-%04d %02d:%02d:%02d"), m_dtStatStarted.GetDay(), m_dtStatStarted.GetMonth(), m_dtStatStarted.GetYear(), 
				m_dtStatStarted.GetHour(), m_dtStatStarted.GetMinute(), m_dtStatStarted.GetSecond());
*/
			// Avg hands per hour.
			int nStatGetherDurSec	= m_dtStatStarted.GetDifferenceInSeconds(dTimeFinish);
			m_dAvgHandsPerHour		= (nStatGetherDurSec < 3600) ? (double)(m_nPlayedHandsCt + 1) : (m_nPlayedHandsCt + 1) / ((double)(nStatGetherDurSec / 3600.0));

			// Avg player flop percent.
			m_nSumPlayersCtStarted	+= m_nPlayersCtStarted;
			m_nSumFlopPlayersCt		+= m_nFlopPlayersCt;
			m_dPlayerFlop			= m_nSumPlayersCtStarted > 0 ? ((double)m_nSumFlopPlayersCt) / m_nSumPlayersCtStarted : 0.0;
			}
		else{
			m_dtStatStarted.SetCurrentTime();
			m_dAvgPotAmount			= (double)(m_nTotalPotAmount / 100.0f);
			m_dAvgPlayers			= (double)(m_nPlayedHandsCt);
			m_dAvgHandsPerHour		= 0.0;
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
			}
		}
	// }}

	// Hide seat cards if they are shown.
	ShowSeatCards		(false);
	ClearPotAmounts		();
	SetMainPotAmount	(0);
	SetTotalPotAmount	(0);
	m_nRakeAmount	= 0;
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
	if( m_bCardsShown == bShow ) 
		return 0;
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
		if( !pSeat || pSeat->GetStatus() != PokerSeatStatus::Sitted ){
			nLoop ++;
			continue;
			}

		int nAnteAmount = m_nAnteAmount;
		int nBalance	= pSeat->GetBalance();
		int nBalanceNew	= pSeat->GetBalance() - nAnteAmount;

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
PokerTableInfo::GetRoundAmounts(PokerRoundType round, int& nTotalPot, int& nRoundPot, int& nRakeAmount){
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

	int				nCallAmount = GetCallAmount(nSeat);
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
PokerTableInfo::AddChatMessage(DateTime dtTime, int nSeat, PokerActionTypes acType, int nAmount, bool bSystem /*= false*/, std::string sMessage /*= _T("")*/){
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
PokerTableInfo::CardIndexByName(std::string sName){
	if( !sName.length() || sName.length() != 2 )
		return -1;

	stringEx::MakeUpper(sName);

	const std::string arrCardNum[]	= {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	const std::string arrCardType[] = {_T("S"), _T("C"), _T("D"), _T("H")};

	int nCardTypeIndex	= -1;
	int nCardNum		= -1;

	std::string sType = stringEx::Right(sName, 1);
	std::string sNum = sName.substr(0, 1);

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
