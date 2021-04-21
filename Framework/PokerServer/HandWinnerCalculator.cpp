#include "stdafx.h"
#include "HandWinnerCalculator.h"
#include "Object/PokerTableSeatInfo.h"
#include "Object/PokerTableInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HandWinnerCalculator::HandWinnerCalculator(){
	}

HandWinnerCalculator::~HandWinnerCalculator(){
	}

int
HandWinnerCalculator::GetBestHandCardValue	(int nCard1, int nCard2, int nCard3, int nCard4, int nTableCard1, int nTableCard2, int nTableCard3, int nTableCard4, int nTableCard5, 
											PokerHandCardsClass& cardsClass, CDWordArray& arrCardsIdx)
	{
	return -1;
	}

bool
HandWinnerCalculator::CalculateWinners(PokerTableInfo* pTableInfo, List<PokerHandWinnerInfo>* pWinners, 
										float fRakePercent, UINT& nRakeAmount, INT_CHIPS& nTotalPotAmount, bool& bOpenWinnersCards, 
										bool& bTableCardsIsTheBestComb, int* pArrSeatsBestHandCards)
	{
		return false;
	}