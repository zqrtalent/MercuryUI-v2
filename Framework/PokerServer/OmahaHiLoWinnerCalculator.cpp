#include "stdafx.h"
#include "../Array/AutoSortedArray.h"
#include "OmahaHiLoWinnerCalculator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OmahaHiLoWinnerCalculator::OmahaHiLoWinnerCalculator() : HoldemWinnerCalculator() {
	}

OmahaHiLoWinnerCalculator::~OmahaHiLoWinnerCalculator(){
	}


bool
OmahaHiLoWinnerCalculator::CalculateWinners(PokerTableInfo* pTableInfo, List<PokerHandWinnerInfo>* pWinners, float fRakePercent, 
										UINT& nRakeAmount, INT_CHIPS& nTotalPotAmount, bool& bOpenWinnersCards, bool& bTableCardsIsTheBestComb, CDWordArray* pArrSeatsBestHandCards){
											/*
	int	nInPlaySeatCt	= pTableInfo->CountSeatsByStatus(PokerSeatStatus::InPlay) + (pTableInfo->m_bTournamentTable ? pTableInfo->CountSeatsByStatus(PokerSeatStatus::SittedOut) : 0);
	int	nAllInSeatCt	= pTableInfo->CountSeatsByStatus(PokerSeatStatus::All_In);
	nRakeAmount			= 0;
	nTotalPotAmount		= pTableInfo->GetTotalPotAmount();
	bOpenWinnersCards	= false;

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

		nTotalPotAmount					= pTableInfo->GetTotalPotAmount();
		if( fRakePercent > 0.0f && pTableInfo->m_round > PokerRoundType::PreFlop ){
			// Exclude extra amount from total pot. {{
			int nCt			= pTableInfo->m_cMaxPlayersCt;
			int nLoop		= 0;
			int nWinnersPot = pWinnerSeat->GetTotalPot();
			int nWinnerSeat	= pWinnerSeat->GetSeat();
			int nExcludePot	= 0;
			int nSecondPot	= 0;

			while( nLoop < nCt ){
				if( nLoop != nWinnerSeat ){
					int nPot = pTableInfo->GetSeatTotalPot(nLoop);
					if( nPot > nSecondPot )
						nSecondPot = nPot;
					}
				nLoop ++;
				}

			nExcludePot = max((nWinnersPot - nSecondPot), 0);
			// }}

			nRakeAmount					= (int)((nTotalPotAmount - nExcludePot)*fRakePercent);
//			nRakeAmount					= max(1, nRakeAmount);
			}

		PokerHandWinnerInfo* pWinner	= new PokerHandWinnerInfo();
		pWinner->m_cSeat				= (char)pWinnerSeat->GetSeat();
		pWinner->m_nPotAmount			= pWinnerSeat->GetTotalPot();
		pWinner->m_nBalance				= pWinnerSeat->GetBalance();
		pWinner->m_nWonAmount			= (nTotalPotAmount - nRakeAmount);
		pWinner->m_cHandCardClass		= PokerHandCardsClass::CardsClass_None;
		pWinnerSeat->SetBalance			(pWinnerSeat->GetBalance() + pWinner->m_nWonAmount);
		pWinnerSeat->SetHandWonState	(true);
		pWinners->Add(pWinner);
		return true;
		}
	// }}

	int nTableCard1		= pTableInfo->GetTableCard1();
	int nTableCard2		= pTableInfo->GetTableCard2();
	int nTableCard3		= pTableInfo->GetTableCard3();
	int nTableCard4		= pTableInfo->GetTableCard4();
	int nTableCard5		= pTableInfo->GetTableCard5();
	int	nTableVal		= GetHandValue(GetCardValue(nTableCard1), GetCardValue(nTableCard2), GetCardValue(nTableCard3),
						GetCardValue(nTableCard4), GetCardValue(nTableCard5));

	int nLoop			= 0; 
	int nCt				= (char)pTableInfo->m_cMaxPlayersCt;

	List<PokerTableSeatInfo> *pParticipants = new List<PokerTableSeatInfo>();
	pParticipants->SetDestroyObjects(FALSE);

	while( nLoop < nCt ){
		PokerTableSeatInfo* pInfo = pTableInfo->GetSeatInfo(nLoop);
		if( pInfo && 
			(pInfo->GetStatus() == PokerSeatStatus::All_In || 
			pInfo->GetStatus() == PokerSeatStatus::InPlay) ){
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

		int nHandCard1				= GetCardValue(pInfo->GetCard1());
		int nHandCard2				= GetCardValue(pInfo->GetCard2());
		int nBestHandCardsVal		= 0x7fffffff;
		int nBestHandCardsValTemp	= 0;
		HandCombination*	pComb	= new HandCombination();
		pComb->pSeatInfo			= pInfo;

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

		pComb->nHandValue = nBestHandCardsVal;
		memcpy(pComb->arrWinnerCardsIdx, arrWinnerCardsIdx, sizeof(arrWinnerCardsIdx));
		// Save calculated hand value with combination info.
		arrHandValByHandCombination.Add(nBestHandCardsVal, (PTR_HOLDER_INTEGER)pComb);
		}

	if( arrHandValByHandCombination.GetCount() <= 0 ){
		delete pParticipants;
		return false;
		}

	// Determine winner seat(s). {{
	arrHandValByHandCombination.Add(nTableVal, 0);
	int		nMaxHandVal	= arrHandValByHandCombination.Get(0);
	_String sHandClassName;

	for( int i=0; i<arrHandValByHandCombination.GetCount(); i++ ){
		HandCombination* pComb = (HandCombination*)arrHandValByHandCombination.GetData(i);
		if( arrHandValByHandCombination.Get(i) == nMaxHandVal ){
			if( pComb ){
				PokerTableSeatInfo* pSeatInfo	= pComb->pSeatInfo;
				PokerHandWinnerInfo* pWinner	= new PokerHandWinnerInfo();
				pWinner->m_cSeat				= (char)pSeatInfo->GetSeat();
				pWinner->m_cHandCardClass		= (char)CardsHandClass(nMaxHandVal, sHandClassName);
				pWinner->m_nPotAmount			= pSeatInfo->GetTotalPot();
				pWinner->m_nBalance				= pSeatInfo->GetBalance();
				pWinner->m_bWinner				= true;

				pWinner->m_cWinnerCard1			= (char)pComb->arrWinnerCardsIdx[0];
				pWinner->m_cWinnerCard2			= (char)pComb->arrWinnerCardsIdx[1];
				pWinner->m_cWinnerCard3			= (char)pComb->arrWinnerCardsIdx[2];
				pWinner->m_cWinnerCard4			= (char)pComb->arrWinnerCardsIdx[3];
				pWinner->m_cWinnerCard5			= (char)pComb->arrWinnerCardsIdx[4];
				pWinners->Add(pWinner);
				delete pComb; // Delete combination info class object
				}
			else{ // Everyone wins.
				int nLoop	= 0;
				int nCt		= arrHandValByHandCombination.GetCount();
				while( nLoop < nCt ){
					HandCombination* pComb = (HandCombination*)arrHandValByHandCombination.GetData(i);
					if( pComb ){
						PokerTableSeatInfo* pSeatInfo	= pComb->pSeatInfo;
						PokerHandWinnerInfo* pWinner	= new PokerHandWinnerInfo();
						pWinner->m_cSeat				= (char)pSeatInfo->GetSeat();
						pWinner->m_cHandCardClass		= (char)CardsHandClass(nMaxHandVal, sHandClassName);
						pWinner->m_nPotAmount			= pSeatInfo->GetTotalPot();
						pWinner->m_nBalance				= pSeatInfo->GetBalance();
						pWinner->m_bWinner				= true;
						
						pWinner->m_cWinnerCard1			= 4;	// table card 1 index
						pWinner->m_cWinnerCard2			= 5;	// table card 2 index
						pWinner->m_cWinnerCard3			= 6;	// table card 3 index
						pWinner->m_cWinnerCard4			= 7;	// table card 4 index
						pWinner->m_cWinnerCard5			= 8;	// table card 5 index
						pWinners->Add(pWinner);
						delete pComb; // Delete combination info class object
						}
					nLoop ++;
					}
				break;
				}
			}
		else{
			if( pComb )
				delete pComb;
			}
		}
	UNUSED(arrHandValByHandCombination);
	// }}

	int nTotalPot		= pTableInfo->GetTotalPotAmount();
	int nWinnersPotFull = 0;
	int	nWinnerCt		= pWinners->GetCount();

	// Winner's maximum pot amount.
	int				nSeatPotMax = 0;
	int				nSeatPotMaxIdx = 0;
	for( int i=0; i<pWinners->GetCount(); i++ ){
		PokerHandWinnerInfo* pWinner = pWinners->GetAt(i);
		if( nSeatPotMax < pWinner->m_nPotAmount ){
			nSeatPotMax	= pWinner->m_nPotAmount;
			nSeatPotMaxIdx = (int)pWinner->m_cSeat;
			}
		nWinnersPotFull	+= pWinner->m_nPotAmount;
		}

	// Return back amount to participants.
	for( int i=0; i<pParticipants->GetCount(); i++ ){
		PokerTableSeatInfo*	pSeatInfo = pParticipants->GetAt(i);
		if( nSeatPotMax < pSeatInfo->GetTotalPot() ){
			int nAmountBack			= (pSeatInfo->GetTotalPot() - nSeatPotMax);
			nTotalPot				-= nAmountBack;
			pSeatInfo->SetBalance	(pSeatInfo->GetBalance() + nAmountBack);
//			pSeatInfo->SetPotAmount	(0);
//			pSeatInfo->SetTotalPot	(0);

			// Return back money from pot. NonWinner
			PokerHandWinnerInfo* pWinner	= new PokerHandWinnerInfo();
			pWinner->m_cSeat				= (char)pSeatInfo->GetSeat();
			pWinner->m_cHandCardClass		= PokerHandCardsClass::CardsClass_None;
			pWinner->m_nPotAmount			= pSeatInfo->GetTotalPot();
			pWinner->m_nBalance				= pSeatInfo->GetBalance() - nAmountBack;
			pWinner->m_nWonAmount			= nAmountBack;
			pWinner->m_bWinner				= false;	
			pWinners->Add(pWinner);
			}
		}
	// }}

	// Exclude extra amount from total pot. {{
	nCt				= pTableInfo->m_cMaxPlayersCt;
	nLoop			= 0;
	int nWinnersPot = nSeatPotMax, nWinnerSeat	= nSeatPotMaxIdx;
	int nExcludePot	= 0, nSecondPot	= 0;

	while( nLoop < nCt ){
		if( nLoop != nWinnerSeat ){
			int nPot = pTableInfo->GetSeatTotalPot(nLoop);
			if( nPot > nSecondPot )
				nSecondPot = nPot;
			}
		nLoop ++;
		}
	nExcludePot = max((nWinnersPot - nSecondPot), 0);
	// }}

	// Calculate rake amount. {{
	nTotalPotAmount	= nTotalPot;
	if( fRakePercent > 0.0f && pTableInfo->m_round > PokerRoundType::PreFlop ){
		nRakeAmount		= (int)((nTotalPot - nExcludePot)*fRakePercent);
//		nRakeAmount		= max(1, nRakeAmount);
		}
	// }}

	// Award winner(s).
	for( int i=0; i<nWinnerCt; i++ ){
		PokerHandWinnerInfo*	pWinner		= pWinners->GetAt(i);
		int						nSeat		= (int)pWinner->m_cSeat;
		// Won percent from total pot.
		float					fWonPercent	= (float)(pWinner->m_nPotAmount / ((float)nWinnersPotFull));
		// Won amount.
		double					dAmount		= ((nTotalPot - nRakeAmount)*fWonPercent)/100.0f;
		pWinner->m_nWonAmount				= PokerData::ConvertMoney(dAmount);
		// Set seat balance.
		pTableInfo->SetBalance(nSeat, pTableInfo->GetBalance(nSeat) + pWinner->m_nWonAmount);
		// Set seat hand won state.
		pTableInfo->SetHandWonState	(nSeat, true);
		}

	bOpenWinnersCards = true;
	delete pParticipants;*/
	return true;
	}

int
OmahaHiLoWinnerCalculator::GetBestHandCardValue(int nCard1, int nCard2, int nCard3, int nCard4, 
											int nTableCard1, int nTableCard2, int nTableCard3, int nTableCard4, int nTableCard5, 
											PokerHandCardsClass& cardsClass, CDWordArray& arrCardsIdx){
												/*
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
	cardsClass = CardsHandClass(nBestHandCardsVal, sClassName);
	if( cardsClass != PokerHandCardsClass::CardsClass_None ){
		arrCardsIdx.Add(arrWinnerCardsIdx[0]);
		arrCardsIdx.Add(arrWinnerCardsIdx[1]);
		arrCardsIdx.Add(arrWinnerCardsIdx[2]);
		arrCardsIdx.Add(arrWinnerCardsIdx[3]);
		arrCardsIdx.Add(arrWinnerCardsIdx[4]);
		}
	return nBestHandCardsVal;*/ 

	cardsClass = PokerHandCardsClass::CardsClass_None;
	return 0;
	}
