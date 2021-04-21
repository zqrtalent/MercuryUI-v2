#pragma once

/////////////////////////////////////////////////////////////////////////////
// class PokerHandHistory

#include "..\..\UI\Poker\PokerGUI.h"
#include "PokerHandWinnerInfo.h"
#include "PokerTableSeatInfo.h"
#include "PokerTableInfo.h"

class PokerHandAction : public Serializable
{
public:
	PokerHandAction();
	~PokerHandAction();

public:
	char								m_cSeat;			// Seat index.
	char								m_cAction;			// Action type.
	char								m_cSeatStatus;		// Seat status.
	char								m_cTimeSec;			// Action time seconds.
	INT_CHIPS							m_nActAmount;		// Action amount value.
	INT_CHIPS							m_nPotAmount;		// Pot amount value.
	INT_CHIPS							m_nTotalPotAmount;	// Total Pot amount value.
	INT_CHIPS							m_nBalanceAmount;	// Balance amount.
	EnumerableObject<PokerHandCards>	m_listCardsShown;	// Cards shown.

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerHandAction();};
	INIT_RUNTIME_VARIABLE()
};

class PokerHandBestCards : public Serializable
{
public:
	PokerHandBestCards();
	~PokerHandBestCards();

public:
	BYTE								m_btSeat;			// Seat index.
	char								m_cBestCardsClass;	// Best hand cards class. PokerHandCardsClass
	char								m_cBestCard1;		// Best 1 card index. 
	char								m_cBestCard2;		// Best 2 card index.
	char								m_cBestCard3;		// Best 3 card index.
	char								m_cBestCard4;		// Best 4 card index.
	char								m_cBestCard5;		// Best 5 card index.

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerHandBestCards();};
	INIT_RUNTIME_VARIABLE()
};

class PokerHandRound : public Serializable
{
public:
	PokerHandRound();
	~PokerHandRound();

public:
	char									m_cRoundType;			// Poker game round type.
	INT_CHIPS								m_nTotalPotAmount_Start;// Total pot amount.
	UINT									m_nRakeAmount_Start;	// Rake amount.
	INT_CHIPS								m_nTotalPotAmount;		// Total pot amount.
	INT_CHIPS								m_nRoundPotAmount;		// Round pot amount.
	UINT									m_nRakeAmount;			// Rake amount.
	EnumerableObject<PokerHandAction>		m_roundActions;			// Round actions list.
	EnumerableObject<PokerHandBestCards>	m_listHandBestCards;	// Hand best cards list.
	EnumerableObject<PokerPotAmountDetail>	m_listPots;				// List of pots.

protected: // Serialization.	
	virtual Serializable* CreateSerializableObject(){return new PokerHandRound();};
	INIT_RUNTIME_VARIABLE()
};

class PokerHandHistory : public Serializable
{
public:
	PokerHandHistory	();
	~PokerHandHistory	();

	virtual bool	Load					(const _String sFile);
	virtual bool	Load					(LongBinary* pBin);

	void			StartHand				(PokerTableInfo* pInfo, __int64 nHandId, int nRakePercent, int nHandStartSec, int nRoundStartSec);
	void			FinishHand				(List<PokerHandWinnerInfo>* pWinners, PokerTableInfo* pTableInfo, int nHandFinishSec);
	void			FinishHand2				(EnumerableObject<PokerHandWinnerInfo>* pWinners, PokerTableInfo* pTableInfo, int nHandFinishSec, 
											EnumerableObject<PokerHandCards>* pListHandCards = NULL,
											EnumerableObject<PokerPotAmountDetail>* pListPots = NULL);

	void			StartRound				(PokerRoundType roundType, EnumerableObject<PokerHandCards>* pListHandCards, EnumerableObject<PokerPotAmountDetail>* pListPots, INT_CHIPS nTotalPotAmount, UINT nRakeAmount);
	void			FinishRound				(INT_CHIPS nTotalPotAmount, INT_CHIPS nPotAmount, UINT nRakeAmount);
	void			FinishRound2			();
	bool			UpdateCurrentRound		(INT_CHIPS nTotalPotAmount, INT_CHIPS nPotAmount, UINT nRakeAmount);

	void			Action					(int nSeat, PokerActionTypes actionType, PokerSeatStatus status, INT_CHIPS nAmount, INT_CHIPS nBalance, INT_CHIPS nPotAmount, INT_CHIPS nTotalPotAmount, int nTimeSec, 
											EnumerableObject<PokerHandCards>* pCardsShown = NULL);

	bool			ChangeLastActionAmounts	(int nSeat, INT_CHIPS nAmountNew, INT_CHIPS nBalanceNew, INT_CHIPS nPotAmountNew, INT_CHIPS nTotalPotAmountNew);

	void			ActionWithHandCard		(int nSeat, PokerActionTypes actionType, INT_CHIPS nAmount, INT_CHIPS nBalance, INT_CHIPS nPotAmount, INT_CHIPS nTotalPotAmount, int nTimeSec, PokerHandCards* pHandCards);
	void			ClearHistory			();

	int				GetWinnersSeperatedBySemiCollon(_String& sWinners);
	INT_CHIPS		GetPotAmount			();
	bool			GetMyHoleCards			(int nUserId, _String& sMyHoleCards);

	bool			SetHandCards			(int nUserId, int nCard1, int nCard2, int nCard3, int nCard4);   
public:
	__int64									m_nHandId;				// Hand id.
	int										m_nTableId;				// Table id.
	int										m_nTournamentId;		// Tournament id.
	_String									m_sTableName;			// Table name.
	BYTE									m_cGameType;			// Game type.
	short									m_shSubType;			// Game sub type.
	char									m_cCurrencyId;			// Currency Id.

	char									m_cHandStartSec;		// Hand start time sec.
	char									m_cRoundStartSec;		// Round start sec.
	char									m_cHandFinishStartSec;	// Hand finish sec.

	char									m_cMaxSeatCt;			// Max seat count.
	char									m_cDealer;				// Dealer seat index.
	char									m_cSmallBlind;			// Small Blind seat index.
	char									m_cBigBlind;			// Big Blind seat index.
	char									m_cRakePercent;			// Rake percent.

	UINT									m_nSmallBlindAmount;	// Small Blind amount.
	UINT									m_nBigBlindAmount;		// Big Blind amount.
	UINT									m_nAnteAmount;			// Ante amount.

	char									m_cTableCard1;			// Table card1
	char									m_cTableCard2;			// Table card2
	char									m_cTableCard3;			// Table card3
	char									m_cTableCard4;			// Table card4
	char									m_cTableCard5;			// Table card5

	EnumerableObject<PokerTableSeatInfo>	m_listSeats;			// Table seat info list when hand started.
	EnumerableObject<PokerHandRound>		m_listRounds;			// List of rounds.
	EnumerableObject<PokerHandWinnerInfo>	m_listWinners;			// Hand winners list.
	EnumerableObject<PokerPotAmountDetail>	m_listPots;				// List of pots (Pots visible when hand is finished).
	EnumerableObject<PokerHandCards>		m_listHandCards;		// List of hand cards. (Hand cards when hand is finished)

protected:
	PokerHandRound*							m_pCurrentRound;		// Current round.

protected: // Serialization.	
	virtual Serializable* CreateSerializableObject(){return new PokerHandHistory();};
	INIT_RUNTIME_VARIABLE()
};