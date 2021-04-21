#include "stdafx.h"
#include "../Array/AutoSortedArray.h"
#include "OmahaWinnerCalculator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OmahaWinnerCalculator::OmahaWinnerCalculator() : HoldemWinnerCalculator() {
	}

OmahaWinnerCalculator::~OmahaWinnerCalculator(){
	}


bool
OmahaWinnerCalculator::CalculateWinners(PokerTableInfo* pTableInfo, List<PokerHandWinnerInfo>* pWinners, float fRakePercent, 
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
		UINT nLoop				= 0, nCt = pTableInfo->m_cMaxPlayersCt;;
		INT_CHIPS nWinnersPot	= pWinnerSeat->GetTotalPot();
		int nWinnerSeat			= pWinnerSeat->GetSeat();
		INT_CHIPS nReturnBackAmount	= 0, nSecondPot	 = 0;

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
	int	nTableVal		= GetHandValue(GetCardValue(nTableCard1), GetCardValue(nTableCard2), GetCardValue(nTableCard3),
						GetCardValue(nTableCard4), GetCardValue(nTableCard5));

	UINT nLoop			= 0, nCt = (char)pTableInfo->m_cMaxPlayersCt;
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

	for(int i=0; i<pParticipants->GetCount(); i++){
		PokerTableSeatInfo* pInfo = pParticipants->GetAt(i);
		ASSERT(pInfo);

		int nHandCard1					= GetCardValue(pInfo->GetCard1());
		int nHandCard2					= GetCardValue(pInfo->GetCard2());
		int nHandCard3					= GetCardValue(pInfo->GetCard3());
		int nHandCard4					= GetCardValue(pInfo->GetCard4());
		int nBestHandCardsVal			= 0x7fffffff;
		int nBestHandCardsValTemp		= 0;
		HandCombination*	pHandComb	= new HandCombination;
		pHandComb->pSeatInfo			= pInfo;

		struct HandCardsCombination{
			int nHandCardVal1;
			int nHandCardVal2;
			int nHandCardIndex1;
			int nHandCardIndex2;
			};

		HandCardsCombination	comb[] = 
			{
			{nHandCard1, nHandCard2, 0, 1}, 
			{nHandCard1, nHandCard3, 0, 2}, 
			{nHandCard1, nHandCard4, 0, 3}, 
			{nHandCard2, nHandCard3, 1, 2}, 
			{nHandCard2, nHandCard4, 1, 3}, 
			{nHandCard3, nHandCard4, 2, 3}
			};

		for( int x=0; x<6; x++){
			HandCardsCombination* pComb = &comb[x];
			for( int j=0; j<5; j++ ){
				for( int k=j+1; k<5; k++ ){
					// Copy cards array.
					memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));
					arrCardsReplaced[j] = pComb->nHandCardVal1;
					arrCardsReplaced[k] = pComb->nHandCardVal2;

					nBestHandCardsValTemp = GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
					if( nBestHandCardsVal > nBestHandCardsValTemp ){
						nBestHandCardsVal		= nBestHandCardsValTemp;
						arrWinnerCardsIdx[0]	= 4;						// table card 1 index
						arrWinnerCardsIdx[1]	= 5;						// table card 2 index
						arrWinnerCardsIdx[2]	= 6;						// table card 3 index
						arrWinnerCardsIdx[3]	= 7;						// table card 4 index
						arrWinnerCardsIdx[4]	= 8;						// table card 5 index
						arrWinnerCardsIdx[j]	= pComb->nHandCardIndex1;	// hand card index
						arrWinnerCardsIdx[k]	= pComb->nHandCardIndex2;	// hand card index
						}
					}
				}
			}

		// Set best hand card class.
		if( pArrSeatsBestHandCards )
			pArrSeatsBestHandCards[pHandComb->pSeatInfo->GetSeat()] =  (int)CardsHandClass(nBestHandCardsVal, NULL);

		pHandComb->nHandValue = nBestHandCardsVal;
		memcpy(pHandComb->arrWinnerCardsIdx, arrWinnerCardsIdx, sizeof(arrWinnerCardsIdx));
		// Save calculated hand value with combination info.
		arrHandValByHandCombination.Add((void*)nBestHandCardsVal, (void*)pHandComb);
		}

	if( arrHandValByHandCombination.GetCount() <= 0 ){
		delete pParticipants;
		return false;
		}

	// Determine winner seat(s). {{
	arrHandValByHandCombination.Add((void*)nTableVal, 0);
	int			nMaxHandVal			= (int)arrHandValByHandCombination.Get(0);
	_String		sHandClassName;
	INT_CHIPS	nAwardAmount		= nTotalPotAmount, nAllAwardAmount	= 0;
	bool		bTableCombIsTheBest = (nMaxHandVal == nTableVal && ((int)arrHandValByHandCombination.Get(1)) >= nTableVal);
	bTableCardsIsTheBestComb		= bTableCombIsTheBest;

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
			INT_CHIPS	nRemain				= nWinnersCt > 0 ? (nAwardAmount % nWinnersCt) : 0;
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

					INT_CHIPS nSeatPotAmount		= (INT_CHIPS)arrSeatSidePots.GetData(j);
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
	UINT nRakeAmountSum = 0, nMaxSeatCt = (int)pTableInfo->m_cMaxPlayersCt;
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
				pWinner->m_nWonAmount				-= (UINT)Serializable::ConvertMoney(fRakeAmount);

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

		for(int i=0; i<arrWinnerSeats.GetCount(); i++){
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
OmahaWinnerCalculator::GetBestHandCardValue(int nCard1, int nCard2, int nCard3, int nCard4, 
											int nTableCard1, int nTableCard2, int nTableCard3, int nTableCard4, int nTableCard5, 
											PokerHandCardsClass& cardsClass, CDWordArray& arrCardsIdx){
	if( nTableCard1 <= -1 || nTableCard2 <= -1 || nTableCard3 <= -1 ){
		cardsClass = PokerHandCardsClass::CardsClass_None;
		return PokerHandCardsClass::CardsClass_None;
		}

	int				arrCards[]			= {	GetCardValue(nTableCard1), GetCardValue(nTableCard2), 
											GetCardValue(nTableCard3), GetCardValue(nTableCard4), 
											GetCardValue(nTableCard5)};
	int				arrCardsReplaced[]	= {	arrCards[0], arrCards[1], arrCards[2], arrCards[3], arrCards[4]}; 
	int				arrWinnerCardsIdx[]	= {	4, 5, 6, 7, 8 }; // Copy best combinations card indexes.((0, 1, 2, 3) - identifies hand cards (4, 5, 6, 7, 8) = identifies table cards)

	struct HandCardsCombination{
		int nHandCardVal1;
		int nHandCardVal2;
		int nHandCardIndex1;
		int nHandCardIndex2;
		};

	int nHandCardVal1			= GetCardValue(nCard1);
	int nHandCardVal2			= GetCardValue(nCard2);
	int nHandCardVal3			= GetCardValue(nCard3);
	int nHandCardVal4			= GetCardValue(nCard4);
	int nBestHandCardsVal		= 0x7fffffff;
	int nBestHandCardsValTemp	= 0;

	HandCardsCombination	comb[] = 
		{
		{nHandCardVal1, nHandCardVal2, 0, 1}, 
		{nHandCardVal1, nHandCardVal3, 0, 2}, 
		{nHandCardVal1, nHandCardVal4, 0, 3}, 
		{nHandCardVal2, nHandCardVal3, 1, 2}, 
		{nHandCardVal2, nHandCardVal4, 1, 3}, 
		{nHandCardVal3, nHandCardVal4, 2, 3}
		};

	// 3 table cards
	if( nTableCard4 <= -1 ){
		for( int x=0; x<6; x++){
			HandCardsCombination* pComb = &comb[x];
			// Copy cards array.
			memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));
			arrCardsReplaced[3]			= pComb->nHandCardVal1;
			arrCardsReplaced[4]			= pComb->nHandCardVal2;

			nBestHandCardsValTemp = GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
			if( nBestHandCardsVal > nBestHandCardsValTemp ){
				nBestHandCardsVal		= nBestHandCardsValTemp;
				arrWinnerCardsIdx[0]	= 4;						// table card 1 index
				arrWinnerCardsIdx[1]	= 5;						// table card 2 index
				arrWinnerCardsIdx[2]	= 6;						// table card 3 index
				arrWinnerCardsIdx[3]	= pComb->nHandCardIndex1;	// hand card index
				arrWinnerCardsIdx[4]	= pComb->nHandCardIndex2;	// hand card index
				}
			}

		}
	else
	// 4 table cards.
	if( nTableCard5 <= -1 ){
		for( int x=0; x<6; x++){
			HandCardsCombination* pComb = &comb[x];
			for( int j=0; j<4; j++ ){
				// Copy cards array.
				memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));
				arrCardsReplaced[j]			= pComb->nHandCardVal1;
				arrCardsReplaced[4]			= pComb->nHandCardVal2;

				nBestHandCardsValTemp = GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
				if( nBestHandCardsVal > nBestHandCardsValTemp ){
					nBestHandCardsVal		= nBestHandCardsValTemp;
					arrWinnerCardsIdx[0]	= 4;						// table card 1 index
					arrWinnerCardsIdx[1]	= 5;						// table card 2 index
					arrWinnerCardsIdx[2]	= 6;						// table card 3 index
					arrWinnerCardsIdx[3]	= 7;						// table card 4 index
					arrWinnerCardsIdx[4]	= pComb->nHandCardIndex2;	// hand card index
					arrWinnerCardsIdx[j]	= pComb->nHandCardIndex1;	// hand card index
					}
				}
			}
		}
	// 5 table cards.
	else{
		for( int x=0; x<6; x++){
			HandCardsCombination* pComb = &comb[x];
			for( int j=0; j<5; j++ ){
				for( int k=j+1; k<5; k++ ){
					// Copy cards array.
					memcpy(arrCardsReplaced, arrCards, sizeof(arrCards));
					arrCardsReplaced[j] = pComb->nHandCardVal1;
					arrCardsReplaced[k] = pComb->nHandCardVal2;

					nBestHandCardsValTemp = GetHandValue(arrCardsReplaced[0], arrCardsReplaced[1], arrCardsReplaced[2], arrCardsReplaced[3], arrCardsReplaced[4]);
					if( nBestHandCardsVal > nBestHandCardsValTemp ){
						nBestHandCardsVal		= nBestHandCardsValTemp;
						arrWinnerCardsIdx[0]	= 4;						// table card 1 index
						arrWinnerCardsIdx[1]	= 5;						// table card 2 index
						arrWinnerCardsIdx[2]	= 6;						// table card 3 index
						arrWinnerCardsIdx[3]	= 7;						// table card 4 index
						arrWinnerCardsIdx[4]	= 8;						// table card 5 index
						arrWinnerCardsIdx[j]	= pComb->nHandCardIndex1;	// hand card index
						arrWinnerCardsIdx[k]	= pComb->nHandCardIndex2;	// hand card index
						}
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
