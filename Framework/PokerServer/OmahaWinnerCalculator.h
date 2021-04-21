#pragma once
#include "HoldemWinnerCalculator.h"

class OmahaWinnerCalculator : public HoldemWinnerCalculator
{
protected:
	OmahaWinnerCalculator();
	virtual ~OmahaWinnerCalculator();

public:
	static OmahaWinnerCalculator* GetInstance(){
		static OmahaWinnerCalculator inst;
		return &inst;
		}

	virtual int			GetBestHandCardValue	(int nCard1, int nCard2, int nCard3, int nCard4,
												int nTableCard1, int nTableCard2, int nTableCard3, int nTableCard4, int nTableCard5, 
												PokerHandCardsClass& cardsClass, CDWordArray& arrCardsIdx);

	virtual bool		CalculateWinners		(PokerTableInfo* pTableInfo, List<PokerHandWinnerInfo>* pWinners, 
												float fRakePercent, UINT& nRakeAmount, INT_CHIPS& nTotalPotAmount, bool& bOpenWinnersCards, bool& bTableCardsIsTheBestComb, int* pArrSeatsBestHandCards);

protected:
};