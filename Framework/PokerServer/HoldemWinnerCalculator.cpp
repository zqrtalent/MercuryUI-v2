#include "stdafx.h"
#include "../Array/AutoSortedArray.h"
#include "HoldemWinnerCalculator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HoldemWinnerCalculator::HoldemWinnerCalculator() : HandWinnerCalculator() {
	FillProductsSortedArray();
	}

HoldemWinnerCalculator::~HoldemWinnerCalculator(){
	}

PokerHandCardsClass
HoldemWinnerCalculator::CardsHandClass(int nHandValue, _String* pStrClassName){
	PokerHandCardsClass handClass = PokerHandCardsClass::CardsClass_None;
	if (nHandValue >= 1 && nHandValue <= 10){
		handClass = PokerHandCardsClass::CardsClass_StraightFlush; // STRAIGHT FLUSH
		if( pStrClassName ) *pStrClassName = _T("STRAIGHT FLUSH");
		return handClass;
		}
	if (nHandValue >= 11 && nHandValue <= 166){
		handClass = PokerHandCardsClass::CardsClass_FourOfAKind; // FOUR OF A KIND
		if( pStrClassName ) *pStrClassName = _T("FOUR OF A KIND");
		return handClass;
		}
	if (nHandValue >= 167 && nHandValue <= 322){
		handClass = PokerHandCardsClass::CardsClass_FullHouse; // FULL HOUSE
		if( pStrClassName ) *pStrClassName = _T("FULL HOUSE");
		return handClass;
		}
	if (nHandValue >= 323 && nHandValue <= 1599){
		handClass = PokerHandCardsClass::CardsClass_Flush; // FLUSH
		if( pStrClassName ) *pStrClassName = _T("FLUSH");
		return handClass;
		}
	if (nHandValue >= 1600 && nHandValue <= 1609){
		handClass = PokerHandCardsClass::CardsClass_Straight; // STRAIGHT
		if( pStrClassName ) *pStrClassName = _T("STRAIGHT");
		return handClass;
		}
	if (nHandValue >= 1610 && nHandValue <= 2467){
		handClass = PokerHandCardsClass::CardsClass_ThreeOfAKind; // THREE OF A KIND
		if( pStrClassName ) *pStrClassName = _T("THREE OF A KIND");
		return handClass;
		}
	if (nHandValue >= 2468 && nHandValue <= 3325){
		handClass = PokerHandCardsClass::CardsClass_TwoPairs; // 2 PAIRS
		if( pStrClassName ) *pStrClassName = _T("2 PAIRS");
		return handClass;
		}
	if (nHandValue >= 3326 && nHandValue <= 6185){
		handClass = PokerHandCardsClass::CardsClass_Pair; // PAIR
		if( pStrClassName ) *pStrClassName = _T("PAIR");
		return handClass;
		}
	if (nHandValue >= 6182 && nHandValue <= 7462){
		handClass = PokerHandCardsClass::CardsClass_HighCard; // HIGH CARD
		if( pStrClassName ) *pStrClassName = _T("HIGH CARD");
		return handClass;
		}
    return handClass;
	}

bool
HoldemWinnerCalculator::CalculateWinners(PokerTableInfo* pTableInfo, List<PokerHandWinnerInfo>* pWinners, float fRakePercent, 
										UINT& nRakeAmount, INT_CHIPS& nTotalPotAmount, bool& bOpenWinnersCards, bool& bTableCardsIsTheBestComb, int* pArrSeatsBestHandCards){


	int	nInPlaySeatCt	= pTableInfo->CountSeatsByStatus(PokerSeatStatus::InPlay) + (pTableInfo->m_bTournamentTable ? pTableInfo->CountSeatsByStatus(PokerSeatStatus::SittedOut) : 0);
	int	nAllInSeatCt	= pTableInfo->CountSeatsByStatus(PokerSeatStatus::All_In);
	nRakeAmount			= 0;
	nTotalPotAmount		= pTableInfo->GetTotalPotAmount();
	bOpenWinnersCards	= false;
	bTableCardsIsTheBestComb	= false;

	// WITHOUT CALCULATION. {{
	// Calculation is not needed becouse only one seat is active.
	if( (nInPlaySeatCt + nAllInSeatCt) == 1 ){
		PokerTableSeatInfo* pWinnerSeat = pTableInfo->GetNextSeatInfoByStatus(-1, PokerSeatStatus::All_In);
		if( pWinnerSeat == NULL )
			pWinnerSeat = pTableInfo->GetNextSeatInfoByStatus(-1, PokerSeatStatus::InPlay);
		if( pWinnerSeat == NULL ){ // Can not calculate !!
			if( pTableInfo->m_bTournamentTable )
				pWinnerSeat = pTableInfo->GetNextSeatInfoByStatus(-1, PokerSeatStatus::SittedOut);
			if( !pWinnerSeat )
				return false;
			}

		// Calculate return back amount. {{
		int			nCt					= pTableInfo->m_cMaxPlayersCt, nLoop = 0;
		INT_CHIPS	nWinnersPot			= pWinnerSeat->GetTotalPot();
		int			nWinnerSeat			= pWinnerSeat->GetSeat();
		INT_CHIPS	nReturnBackAmount	= 0, nSecondPot	= 0;

		while( nLoop < nCt ){
			if( nLoop != nWinnerSeat ){
				INT_CHIPS nPot = pTableInfo->GetSeatTotalPot(nLoop);
				if( nPot > nSecondPot )
					nSecondPot = nPot;
				}
			nLoop ++;
			}
		nReturnBackAmount = max((nWinnersPot - nSecondPot), 0);
		// }}

		nTotalPotAmount					-= nReturnBackAmount;
		nRakeAmount						= pTableInfo->CalculateRakeAmount(nTotalPotAmount);
		
		PokerHandWinnerInfo* pWinner	= new PokerHandWinnerInfo();
		pWinner->m_cSeat				= (char)pWinnerSeat->GetSeat();
		pWinner->m_nBalance				= pWinnerSeat->GetBalance();
		pWinner->m_nWonAmount			= (nTotalPotAmount - nRakeAmount);
		pWinner->m_nReturnBackAmount	= nReturnBackAmount;
		pWinner->m_cHandCardClass		= PokerHandCardsClass::CardsClass_None;
		pWinnerSeat->SetBalance			(pWinnerSeat->GetBalance() + pWinner->m_nWonAmount);
		pWinnerSeat->SetHandWonState	(true); // Set hand won state.
		pWinners->Add(pWinner);

		PokerWonAmountDetail* pDetail	= new PokerWonAmountDetail();
		pDetail->m_cPotIndex			= 0; // Main pot
		pDetail->m_nWonAmount			= pWinner->m_nWonAmount;
		pWinner->m_listWonAmountDetail.Add(pDetail);

		// Return back amount.
		pWinnerSeat->SetBalance(pWinnerSeat->GetBalance() + nReturnBackAmount);
		return true;
		}
	// }}

	AutoSortedArray arrSeatSidePots;
	List<SidePotInfoStruct> listSidePots;
	listSidePots.SetDestroyObjects(true);
	pTableInfo->CalculateSidePots(listSidePots);

	int nTableCard1		= pTableInfo->GetTableCard1();
	int nTableCard2		= pTableInfo->GetTableCard2();
	int nTableCard3		= pTableInfo->GetTableCard3();
	int nTableCard4		= pTableInfo->GetTableCard4();
	int nTableCard5		= pTableInfo->GetTableCard5();
	int	nTableVal		= GetHandValue(GetCardValue(nTableCard1), GetCardValue(nTableCard2), GetCardValue(nTableCard3), GetCardValue(nTableCard4), GetCardValue(nTableCard5));

	int nLoop			= 0, nCt = (char)pTableInfo->m_cMaxPlayersCt;

	List<PokerTableSeatInfo> *pParticipants = new List<PokerTableSeatInfo>();
	pParticipants->SetDestroyObjects(FALSE);

	while( nLoop < nCt ){
		PokerTableSeatInfo* pInfo = pTableInfo->GetSeatInfo(nLoop);
		if( pInfo && pTableInfo->IsInPlayByStatus(nLoop, pInfo->GetStatus()) /*pInfo->GetStatus() == PokerSeatStatus::All_In || pInfo->GetStatus() == PokerSeatStatus::InPlay)*/ ){
			pParticipants->Add(pInfo);
			}
		nLoop ++;
		}

	int				arrCards[]			= {	GetCardValue(nTableCard1), GetCardValue(nTableCard2), 
											GetCardValue(nTableCard3), GetCardValue(nTableCard4), 
											GetCardValue(nTableCard5)};

	int				arrCardsReplaced[]	= {	GetCardValue(nTableCard1), GetCardValue(nTableCard2), 
											GetCardValue(nTableCard3), GetCardValue(nTableCard4), 
											GetCardValue(nTableCard5)};

	int				arrWinnerCardsIdx[]	= {	4, 5, 6, 7, 8 }; // Copy best combinations card indexes.((0, 1, 2, 3) - identifies hand cards (4, 5, 6, 7, 8) = identifies table cards)

	AutoSortedArray	arrHandValByHandCombination;
	struct HandCombination{
		PokerTableSeatInfo* pSeatInfo;
		int					nHandValue;
		int					arrWinnerCardsIdx[5];
		};

	INT_CHIPS nTotalPot = pTableInfo->GetTotalPotAmount(), nParticipantsPotSum = 0;
	for(int i=0; i<pParticipants->GetCount(); i++){
		PokerTableSeatInfo* pInfo = pParticipants->GetAt(i);
		ASSERT(pInfo);
		int nHandCard1				= GetCardValue(pInfo->GetCard1());
		int nHandCard2				= GetCardValue(pInfo->GetCard2());
		int nBestHandCardsVal		= 0x7fffffff;
		int nBestHandCardsValTemp	= 0;
		HandCombination*	pComb	= new HandCombination;
		pComb->pSeatInfo			= pInfo;
		nParticipantsPotSum			+= pInfo->GetTotalPot();

		for( int j=0; j<5; j++ ){
			// Copy cards array.
			memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));

			arrCardsReplaced[j] = nHandCard1;
			nBestHandCardsValTemp = GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
			if( nBestHandCardsVal > nBestHandCardsValTemp ){
				nBestHandCardsVal = nBestHandCardsValTemp;

				arrWinnerCardsIdx[0] = 4; // table card 1 index
				arrWinnerCardsIdx[1] = 5; // table card 2 index
				arrWinnerCardsIdx[2] = 6; // table card 3 index
				arrWinnerCardsIdx[3] = 7; // table card 4 index
				arrWinnerCardsIdx[4] = 8; // table card 5 index
				arrWinnerCardsIdx[j] = 0; // hand card 1
				}
			//nBestHandCardsVal = min(nBestHandCardsVal, nBestHandCardsValTemp);
			}

		for( int j=0; j<5; j++ ){
			// Copy cards array.
			memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));

			arrCardsReplaced[j] = nHandCard2;
			nBestHandCardsValTemp = GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
			if( nBestHandCardsVal > nBestHandCardsValTemp ){
				nBestHandCardsVal = nBestHandCardsValTemp;

				arrWinnerCardsIdx[0] = 4; // table card 1 index
				arrWinnerCardsIdx[1] = 5; // table card 2 index
				arrWinnerCardsIdx[2] = 6; // table card 3 index
				arrWinnerCardsIdx[3] = 7; // table card 4 index
				arrWinnerCardsIdx[4] = 8; // table card 5 index
				arrWinnerCardsIdx[j] = 1; // hand card 2
				}
			}

		for( int j=0; j<5; j++ ){
			for( int k=j+1; k<5; k++ ){
				// Copy cards array.
				memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));

				arrCardsReplaced[j] = nHandCard1;
				arrCardsReplaced[k] = nHandCard2;

				nBestHandCardsValTemp = GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
				if( nBestHandCardsVal > nBestHandCardsValTemp ){
					nBestHandCardsVal = nBestHandCardsValTemp;

					arrWinnerCardsIdx[0] = 4; // table card 1 index
					arrWinnerCardsIdx[1] = 5; // table card 2 index
					arrWinnerCardsIdx[2] = 6; // table card 3 index
					arrWinnerCardsIdx[3] = 7; // table card 4 index
					arrWinnerCardsIdx[4] = 8; // table card 5 index
					arrWinnerCardsIdx[j] = 0; // hand card 1
					arrWinnerCardsIdx[k] = 1; // hand card 2
					}
				}
			}

		// Set best hand card class.
		if( pArrSeatsBestHandCards )
			pArrSeatsBestHandCards[pComb->pSeatInfo->GetSeat()] =  (int)CardsHandClass(nBestHandCardsVal, NULL);
		
		// Temp
		//if( pComb->pSeatInfo->GetUserId() == 39803 )
		//	nBestHandCardsVal = 10;
		// Temp

		pComb->nHandValue = nBestHandCardsVal;
		memcpy(pComb->arrWinnerCardsIdx, arrWinnerCardsIdx, sizeof(arrWinnerCardsIdx));
		// Save calculated hand value with combination info.
		arrHandValByHandCombination.Add((void*)nBestHandCardsVal, pComb);
		}

	if( arrHandValByHandCombination.GetCount() <= 0 ){
		delete pParticipants;
		return false;
		}

	// Determine winner seat(s). {{
	arrHandValByHandCombination.Add((void*)nTableVal, 0);
	int		nMaxHandVal			= (int)arrHandValByHandCombination.Get(0);
	_String sHandClassName;
	INT_CHIPS	nAwardAmount		= nTotalPotAmount, nAllAwardAmount	= 0;
	bool	bTableCombIsTheBest = (nMaxHandVal == nTableVal && ((int)arrHandValByHandCombination.Get(1)) >= nTableVal);
	bTableCardsIsTheBestComb	= bTableCombIsTheBest;

	// Remove table cards combination. {{
	nLoop = 0, nCt = arrHandValByHandCombination.GetCount();
	while( nLoop < nCt ){
		if( arrHandValByHandCombination.GetData(nLoop) == 0 ){
			arrHandValByHandCombination.Delete(nLoop);
			break;
			}
		nLoop ++;
		}
	// }}

	int nPotIndex = 0;
	while( nAwardAmount > 0 ){
		if( !arrHandValByHandCombination.GetCount() ){
			//ASSERT(FALSE);
			// By some divisions of pot amounts there are some remnant amount and below we are sharing that amount to winners.
			int			nWinnersCt			= pWinners->GetCount();
			INT_CHIPS	nShareForOneWinner	= nWinnersCt > 0 ? (nAwardAmount / nWinnersCt) : 0;
			int			nRemain				= nWinnersCt > 0 ? (nAwardAmount % nWinnersCt) : 0;
			if( nShareForOneWinner < 1 ){
				nShareForOneWinner	= 1;
				nWinnersCt			= (int)nAwardAmount;
				nRemain				= 0;
				}

			for(int i=0; i<nWinnersCt; i++){
				PokerHandWinnerInfo* pWinner	= pWinners->GetAt(i);
				if( !pWinner ) continue;
				if( i == (pWinners->GetCount() - 1) ){
					pWinner->m_nWonAmount	+= (nShareForOneWinner + nRemain);
					nAllAwardAmount			+= (nShareForOneWinner + nRemain);
					}
				else{
					pWinner->m_nWonAmount	+= nShareForOneWinner;
					nAllAwardAmount			+= nShareForOneWinner;
					}
				}
			break;
			}

		int nWinnersCt	= 1;
		if( !bTableCombIsTheBest ){
			nMaxHandVal	= (int)arrHandValByHandCombination.Get(0);
			for(UINT i=1; i<arrHandValByHandCombination.GetCount(); i++){
				if( ((int)arrHandValByHandCombination.Get(i)) != nMaxHandVal )
					break;
				nWinnersCt ++;
				}
			}
		else{
			nWinnersCt	= arrHandValByHandCombination.GetCount();
			nMaxHandVal	= nTableVal;
			}

		int nSidePotsUsed = 0;
		for(int i=0; i<nWinnersCt; i++){
			HandCombination*	pComb		= (HandCombination*)arrHandValByHandCombination.GetData(i);
			ASSERT(pComb);
			PokerTableSeatInfo* pSeatInfo	= pComb->pSeatInfo;
			PokerHandWinnerInfo* pWinner	= NULL;

			arrSeatSidePots.DeleteAll();
			if( pTableInfo->GetSeatSidePots(listSidePots, pSeatInfo->GetSeat(), arrSeatSidePots) > nPotIndex ){
				for(UINT j=nPotIndex; j<arrSeatSidePots.GetCount(); j++){
					if( !pWinner ){
						pWinner						= new PokerHandWinnerInfo();
						pWinner->m_cSeat			= (char)pSeatInfo->GetSeat();
						pWinner->m_cHandCardClass	= (char)CardsHandClass(nMaxHandVal, &sHandClassName);
						pWinner->m_nBalance			= pSeatInfo->GetBalance();
						pWinner->m_nWonAmount		= 0;
						pWinner->m_nReturnBackAmount= 0;
						pWinner->m_bWinner			= true;
						}

					INT_CHIPS nSeatPotAmount		= (int)arrSeatSidePots.GetData(j);
					INT_CHIPS nWonAmountFromThatPot	= (nSeatPotAmount * listSidePots[j]->nSeatsCtBelongsTo) / nWinnersCt;
					pWinner->m_nWonAmount			+= nWonAmountFromThatPot;

					PokerWonAmountDetail* pDetail	= new PokerWonAmountDetail();
					pDetail->m_cPotIndex			= (char)arrSeatSidePots.Get(j);
					pDetail->m_nWonAmount			= nWonAmountFromThatPot;
					pDetail->m_wRakeAmount			= (UINT)pTableInfo->CalculateRakeAmount(nWonAmountFromThatPot);
					pWinner->m_listWonAmountDetail.Add(pDetail);
						
					nAwardAmount					-= nWonAmountFromThatPot;
					nAllAwardAmount					+= nWonAmountFromThatPot;
					}
				nSidePotsUsed = max(nSidePotsUsed, arrSeatSidePots.GetCount() - nPotIndex);
				}

			if( pWinner ){
				if( bTableCombIsTheBest ){
					pWinner->m_cWinnerCard1		= 4;	// table card 1 index
					pWinner->m_cWinnerCard2		= 5;	// table card 2 index
					pWinner->m_cWinnerCard3		= 6;	// table card 3 index
					pWinner->m_cWinnerCard4		= 7;	// table card 4 index
					pWinner->m_cWinnerCard5		= 8;	// table card 5 index
					}
				else{
					pWinner->m_cWinnerCard1		= (char)pComb->arrWinnerCardsIdx[0];
					pWinner->m_cWinnerCard2		= (char)pComb->arrWinnerCardsIdx[1];
					pWinner->m_cWinnerCard3		= (char)pComb->arrWinnerCardsIdx[2];
					pWinner->m_cWinnerCard4		= (char)pComb->arrWinnerCardsIdx[3];
					pWinner->m_cWinnerCard5		= (char)pComb->arrWinnerCardsIdx[4];
					}
				pWinners->Add(pWinner);
				}
			}

		// Delete winners combinations. {{
		nLoop = 0;
		while(nLoop < nWinnersCt ){
			HandCombination*	pComb = (HandCombination*)arrHandValByHandCombination.GetData(0);
			ASSERT(pComb);
			delete pComb; // Delete combination info class object
			arrHandValByHandCombination.Delete(0);
			nLoop ++;
			}
		// }}			
		nPotIndex	+= nSidePotsUsed;
		}

	// Delete losers combinations. {{
	while( arrHandValByHandCombination.GetCount() ){
		HandCombination*	pComb = (HandCombination*)arrHandValByHandCombination.GetData(0);
		ASSERT(pComb);
		delete pComb; // Delete combination info class object
		arrHandValByHandCombination.Delete(0);
		}
	// }}
	UNUSED(arrHandValByHandCombination);
	// }}

	AutoSortedArray arrWinnerSeats; // Winner seats ordered from dealer clockwise.
	UINT nRakeAmountSum = 0, nMaxSeatCt = (UINT)pTableInfo->m_cMaxPlayersCt;
	nRakeAmount = pTableInfo->CalculateRakeAmount(nAllAwardAmount);
	for(int i=0; i<pWinners->GetCount(); i++){
		PokerHandWinnerInfo* pWinner = pWinners->GetAt(i);
		if( pWinner->m_bWinner ){
			int						nSeat	= (int)pWinner->m_cSeat;
			if( nAllAwardAmount > 0 ){
				// Won percent from total pot.
				float					fWonPercent	= (float)(pWinner->m_nWonAmount / ((float)nAllAwardAmount));
				// Won amount.
				float fRakeAmount					= (float)(nRakeAmount * fWonPercent) / 100.0f;
				nRakeAmountSum						+= (UINT)Serializable::ConvertMoney(fRakeAmount);
				pWinner->m_nWonAmount				-= Serializable::ConvertMoney(fRakeAmount);

				// Arrange info clokwise from dealler seat.
				if( nSeat <= pTableInfo->m_cDealerSeat )
					arrWinnerSeats.Add((void*)(nMaxSeatCt - (((int)pTableInfo->m_cDealerSeat) - nSeat)), (void*)pWinner);
				else
					arrWinnerSeats.Add((void*)((nSeat - 1) - ((int)pTableInfo->m_cDealerSeat)), (void*)pWinner);
				}

			// Set seat balance.
			pTableInfo->SetBalance		(nSeat, pTableInfo->GetBalance(nSeat) + pWinner->m_nWonAmount + pWinner->m_nReturnBackAmount);
			// Set seat won state.
			pTableInfo->SetHandWonState	(nSeat, true);
			}
		}

	// Award one seat after dealer(clockwise) with fraction amount remnant from total pot.
	int nRemnantFractionOfPot = (nRakeAmountSum - nRakeAmount);
	if( nRemnantFractionOfPot != 0 ){
		int nAmountAddToEach = (nRemnantFractionOfPot / (int)arrWinnerSeats.GetCount());
		int nFraction		 = abs(nRemnantFractionOfPot % (int)arrWinnerSeats.GetCount());
		int nSign			 = (nRemnantFractionOfPot > 0 ? 1 : -1);

		for(UINT i=0; i<arrWinnerSeats.GetCount(); i++){
			PokerHandWinnerInfo* pWinner = (PokerHandWinnerInfo*)arrWinnerSeats.GetData(i);
			if( pWinner ){
				int	nSeat				= (int)pWinner->m_cSeat;
				if( nFraction > 0 ){
					pWinner->m_nWonAmount	+= (nAmountAddToEach + 1*nSign);
					pWinner->m_nBalance		+= (nAmountAddToEach + 1*nSign);
					nFraction --;
					}
				else{
					pWinner->m_nWonAmount	+= nAmountAddToEach;
					pWinner->m_nBalance		+= nAmountAddToEach;
					}
				// Set seat balance.
				pTableInfo->SetBalance(nSeat, pWinner->m_nBalance);
				}
			}
		/* Old code.
		PokerHandWinnerInfo* pWinner = (PokerHandWinnerInfo*)arrWinnerSeats.GetData(0);
		ASSERT(pWinner);
		if( pWinner ){
			int	nSeat				= (int)pWinner->m_cSeat;
			pWinner->m_nWonAmount	+= nRemnantFractionOfPot;
			// Set seat balance.
			pTableInfo->SetBalance(nSeat, pTableInfo->GetBalance(nSeat) + nRemnantFractionOfPot);
			}*/
		}

	bOpenWinnersCards = true;
	delete pParticipants;
	return true;
	}

int
HoldemWinnerCalculator::GetBestHandCardValue(int nCard1, int nCard2, int nCard3, int nCard4, 
											int nTableCard1, int nTableCard2, int nTableCard3, int nTableCard4, int nTableCard5, 
											PokerHandCardsClass& cardsClass, CDWordArray& arrCardsIdx){

	UNUSED(nCard3);
	UNUSED(nCard4);

	if( nTableCard1 <= -1 || nTableCard2 <= -1 || nTableCard3 <= -1 ){
		cardsClass = PokerHandCardsClass::CardsClass_None;
		return PokerHandCardsClass::CardsClass_None;
		}

	int				arrCards[]			= {	GetCardValue(nTableCard1), GetCardValue(nTableCard2), 
											GetCardValue(nTableCard3), GetCardValue(nTableCard4), 
											GetCardValue(nTableCard5)};
	int				arrCardsReplaced[]	= {	arrCards[0], arrCards[1], arrCards[2], arrCards[3], arrCards[4]}; 
	int				arrWinnerCardsIdx[]	= {	4, 5, 6, 7, 8 }; // Copy best combinations card indexes.((0, 1, 2, 3) - identifies hand cards (4, 5, 6, 7, 8) = identifies table cards)


	int nHandCardVal1			= GetCardValue(nCard1);
	int nHandCardVal2			= GetCardValue(nCard2);
	int nBestHandCardsVal		= 0x7fffffff;
	int nBestHandCardsValTemp	= 0;

	// 3 table cards
	if( nTableCard4 <= -1 ){
		arrCardsReplaced[3]		= nHandCardVal1;
		arrCardsReplaced[4]		= nHandCardVal2;
		nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
		if( nBestHandCardsVal > nBestHandCardsValTemp ){
			nBestHandCardsVal = nBestHandCardsValTemp;

			arrWinnerCardsIdx[0] = 4; // table card 1 index
			arrWinnerCardsIdx[1] = 5; // table card 2 index
			arrWinnerCardsIdx[2] = 6; // table card 3 index
			arrWinnerCardsIdx[3] = 0; // hand card 1
			arrWinnerCardsIdx[4] = 1; // hand card 2
			}
		}
	else
	// 4 table cards.
	if( nTableCard5 <= -1 ){
		arrCardsReplaced[4]		= nHandCardVal1;
		nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
		if( nBestHandCardsVal > nBestHandCardsValTemp ){
			nBestHandCardsVal = nBestHandCardsValTemp;

			arrWinnerCardsIdx[0] = 4; // table card 1 index
			arrWinnerCardsIdx[1] = 5; // table card 2 index
			arrWinnerCardsIdx[2] = 6; // table card 3 index
			arrWinnerCardsIdx[3] = 7; // table card 4 index
			arrWinnerCardsIdx[4] = 0; // hand card 1
			}

		for( int j=0; j<4; j++ ){
			// Copy cards array.
			memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));

			arrCardsReplaced[j] = nHandCardVal2;
			arrCardsReplaced[4] = nHandCardVal1;

			nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
			if( nBestHandCardsVal > nBestHandCardsValTemp ){
				nBestHandCardsVal = nBestHandCardsValTemp;

				arrWinnerCardsIdx[0] = 4; // table card 1 index
				arrWinnerCardsIdx[1] = 5; // table card 2 index
				arrWinnerCardsIdx[2] = 6; // table card 3 index
				arrWinnerCardsIdx[3] = 7; // table card 4 index
				arrWinnerCardsIdx[4] = 8; // table card 5 index

				arrWinnerCardsIdx[j] = 1; // hand card 2 index
				arrWinnerCardsIdx[4] = 0; // hand card 1 index
				}
			}

		memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));
		arrCardsReplaced[4] = nHandCardVal2;

		nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
		if( nBestHandCardsVal > nBestHandCardsValTemp ){
			nBestHandCardsVal = nBestHandCardsValTemp;

			arrWinnerCardsIdx[0] = 4; // table card 1 index
			arrWinnerCardsIdx[1] = 5; // table card 2 index
			arrWinnerCardsIdx[2] = 6; // table card 3 index
			arrWinnerCardsIdx[3] = 7; // table card 4 index
			arrWinnerCardsIdx[4] = 1; // hand card 2 index
			}

		for( int j=0; j<4; j++ ){
			// Copy cards array.
			memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));

			arrCardsReplaced[j] = nHandCardVal1;
			arrCardsReplaced[4] = nHandCardVal2;

			nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
			if( nBestHandCardsVal > nBestHandCardsValTemp ){
				nBestHandCardsVal = nBestHandCardsValTemp;

				arrWinnerCardsIdx[0] = 4; // table card 1 index
				arrWinnerCardsIdx[1] = 5; // table card 2 index
				arrWinnerCardsIdx[2] = 6; // table card 3 index
				arrWinnerCardsIdx[3] = 7; // table card 4 index
				arrWinnerCardsIdx[4] = 1; // hand card 2 index
				arrWinnerCardsIdx[j] = 0; // hand card 1 index
				}
			}
		}
	// 5 table cards.
	else{
		for( int j=0; j<5; j++ ){
			// Copy cards array.
			memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));
			arrCardsReplaced[j] = nHandCardVal1;

			nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
			if( nBestHandCardsVal > nBestHandCardsValTemp ){
				nBestHandCardsVal = nBestHandCardsValTemp;

				arrWinnerCardsIdx[0] = 4; // table card 1 index
				arrWinnerCardsIdx[1] = 5; // table card 2 index
				arrWinnerCardsIdx[2] = 6; // table card 3 index
				arrWinnerCardsIdx[3] = 7; // table card 4 index
				arrWinnerCardsIdx[4] = 8; // table card 5 index

				arrWinnerCardsIdx[j] = 0; // hand card 1 index
				}
			}

		for( int j=0; j<5; j++ ){
			// Copy cards array.
			memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));
			arrCardsReplaced[j] = nHandCardVal2;

			nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
			if( nBestHandCardsVal > nBestHandCardsValTemp ){
				nBestHandCardsVal = nBestHandCardsValTemp;

				arrWinnerCardsIdx[0] = 4; // table card 1 index
				arrWinnerCardsIdx[1] = 5; // table card 2 index
				arrWinnerCardsIdx[2] = 6; // table card 3 index
				arrWinnerCardsIdx[3] = 7; // table card 4 index
				arrWinnerCardsIdx[4] = 8; // table card 5 index
				arrWinnerCardsIdx[j] = 1; // hand card 2 index
				}
			}

		for( int j=0; j<5; j++ ){
			for( int k=j+1; k<5; k++ ){
				// Copy cards array.
				memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));

				arrCardsReplaced[j] = nHandCardVal1;
				arrCardsReplaced[k] = nHandCardVal2;

				nBestHandCardsValTemp	= GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
				if( nBestHandCardsVal > nBestHandCardsValTemp ){
					nBestHandCardsVal = nBestHandCardsValTemp;

					arrWinnerCardsIdx[0] = 4; // table card 1 index
					arrWinnerCardsIdx[1] = 5; // table card 2 index
					arrWinnerCardsIdx[2] = 6; // table card 3 index
					arrWinnerCardsIdx[3] = 7; // table card 4 index
					arrWinnerCardsIdx[4] = 8; // table card 5 index

					arrWinnerCardsIdx[j] = 0; // hand card 1 index
					arrWinnerCardsIdx[k] = 1; // hand card 2 index
					}
				}
			}
		}

	_String sClassName;
	cardsClass = CardsHandClass(nBestHandCardsVal, &sClassName);
	if( cardsClass != PokerHandCardsClass::CardsClass_None ){
		arrCardsIdx.Add(arrWinnerCardsIdx[0]);
		arrCardsIdx.Add(arrWinnerCardsIdx[1]);
		arrCardsIdx.Add(arrWinnerCardsIdx[2]);
		arrCardsIdx.Add(arrWinnerCardsIdx[3]);
		arrCardsIdx.Add(arrWinnerCardsIdx[4]);
		}
	return nBestHandCardsVal;
	}


void
HoldemWinnerCalculator::FillProductsSortedArray(){
	if( m_arrHoldemProducts.GetCount() == 0 ){
		m_arrHoldemProducts.OptimizeForLargeData(TRUE);

		int nLoop = 0, nCt = sizeof(_holdem_products) / sizeof(int);
		while( nLoop < nCt ){
			m_arrHoldemProducts.AddAsLong(_holdem_products[nLoop]);
			nLoop ++;
			}
		}
	}

int
HoldemWinnerCalculator::GetHandValue(int nCard1, int nCard2, int nCard3, int nCard4, int nCard5){
	int q	= (nCard1 | nCard2 | nCard3 | nCard4 | nCard5) >> 16;
	int hv	= -1;

	if ((nCard1 & nCard2 & nCard3 & nCard4 & nCard5 & 0xF000) == 0){
		hv = _holdem_unique5[q];
		if (hv == 0){
			q	= (nCard1 & 0xFF) * (nCard2 & 0xFF) * (nCard3 & 0xFF) * (nCard4 & 0xFF) * (nCard5 & 0xFF);
			bool bExists = false;
			q	= m_arrHoldemProducts.FindValuePlace((void*)q, bExists);
			hv	= (_holdem_values[q]);
			}
		}
	else
		{
		hv = _holdem_flushes[q];
		}
	return hv;
	}

int
HoldemWinnerCalculator::GetCardValue(int nCardIndex){
	if( nCardIndex < 0 || nCardIndex > 52 )
		return -1;
	return _holdem_card_values[nCardIndex];
	}
