#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerHandWinnerInfo 
#include "../../Serialize/Serializable.h"

class PokerWonAmountDetail : public Serializable
{
public:
	PokerWonAmountDetail();
	virtual ~PokerWonAmountDetail();

	INT_CHIPS		m_nWonAmount;		// [OUT] Won amount (Without rake).
	UINT			m_wRakeAmount;		// [OUT] Rake amount.
	// 0 indicates main pot all others are side pots by index.
	char			m_cPotIndex;		// [OUT] Pot index.

public: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerWonAmountDetail(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerHandWinnerInfo : public Serializable
{
public:
	PokerHandWinnerInfo();
	virtual ~PokerHandWinnerInfo();

	char									m_cSeat;				// [OUT] Seat index.
	char									m_cHandCardClass;		// [OUT] PokerHandCardsClass value
	INT_CHIPS								m_nWonAmount;			// [OUT] Won amount (Without rake). 
	INT_CHIPS								m_nReturnBackAmount;	// [OUT] Return back amount.
	INT_CHIPS								m_nBalance;				// [OUT] Balance amount (Won or return back amount are not included.).
	bool									m_bWinner;				// [OUT] Is winner otherwise is loser who gets back some amount of money from pot.

	EnumerableObject<PokerWonAmountDetail>	m_listWonAmountDetail;	// [OUT] Detail info of won amount.

	char									m_cWinnerCard1;			// [OUT] Winner card index 1.
	char									m_cWinnerCard2;			// [OUT] Winner card index 2.
	char									m_cWinnerCard3;			// [OUT] Winner card index 3.
	char									m_cWinnerCard4;			// [OUT] Winner card index 4.
	char									m_cWinnerCard5;			// [OUT] Winner card index 5.

public: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerHandWinnerInfo(); };
	INIT_RUNTIME_VARIABLE()
};