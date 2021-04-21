#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerTableInfo window

#include "../../Serialize/Serializable.h"
#include "../../Array/List.h"
#include "PokerHandCards.h"
#include "PokerChatMessage.h"
#include "PokerPotAmountDetail.h"
#include "../Data/Tables/PokerAvatar.h"

struct PokerRoundAmountStruct{
	PokerRoundType	round;
	int				nTotalPotAmount;
	int				nRoundPotAmount;
	int				nRakeAmount;
};

struct SidePotInfoStruct{
	int		nPotAmount;					// Side pot amount.
	int		nArrSeatsBelongsTo[10];		// Array of pot belonging seats.
	int		nSeatsCtBelongsTo;			// Count of pot belonging seats.
};

class HandWinnerCalculator;
class PokerSeatAmountInfo;
class PokerPreAction;
class PokerTableInfo : public Serializable
{
public:
	PokerTableInfo();
	~PokerTableInfo();

public:
	int										m_nTableId;					// Table id.
	std::string									m_sTableName;				// Table name.
	int										m_nTournamentId;			// Tournament id.
	bool									m_bTournamentTable;			// Tournament table.

	// Statistic info. {{
	DateTime								m_dtStatStarted;			// Statistic info gathering started.
	int										m_nPlayedHandsCt;			// Played hand count.

	int										m_nPlayersCtStarted;		// Players count started.
	int										m_nFlopPlayersCt;			// Flop players count.

	int										m_nSumPlayersCtStarted;		// Sum of started players counts.
	int										m_nSumFlopPlayersCt;		// Sum of Flop players counts.

	double									m_dAvgPotAmount;			// Average pot amount.
	double									m_dAvgPlayers;				// Average player count.
	double									m_dAvgHandsPerHour;			// Played hands count per hour.
	double									m_dPlayerFlop;				// Player/Flop percentage.
	// }}

	char									m_cMaxPlayersCt;
	EnumerableObject<PokerTableSeatInfo>	m_seats;
	EnumerableObject<PokerAvatar>			m_listSeatAvatars;			// Avatars by seat index.
	int										m_nTotalPotAmount;			// Total pot amount.
	int										m_nMainPotAmount;			// Main pot amount.
	int										m_nRakeAmount;				// Rake amount.

	// Basic seat positions.
	char									m_cActiveSeat;				// Active seat index.
	char									m_cDealerSeat;				// Dealer seat index.
	char									m_cSBSeat;					// SB seat index.
	char									m_cBBSeat;					// BB seat index.

	BYTE									m_btRegularActTimeSec;		// Regular action time seconds (Without time bank).

	// Table cards.
	char									m_cCard1;					// Table card1.
	char									m_cCard2;					// Table card2.
	char									m_cCard3;					// Table card3.
	char									m_cCard4;					// Table card4.
	char									m_cCard5;					// Table card5.
	PokerGameStatus							m_gameStatus;				// Poker game status.
	PokerRoundType							m_round;					// Poker round.

	int										m_nSmallBlindAmount;		// Small blind amount.
	int										m_nBigBlindAmount;			// Big blind amount.
	int										m_nAnteAmount;				// Ante amount.
	int										m_nMinBuyIn;				// Min buy in amount
	int										m_nMaxBuyIn;				// Max buy in amount
	short									m_shTimeBankSec;			// Time bank seconds.
	BYTE									m_cGameTypeId;				// Poker game type id. (Holdem, Omaha etc.)
	short									m_shSubTypeId;				// Game sub type id.
	BYTE									m_cMoneyTypeId;				// Money type id.
	float									m_fRakePercent;				// Rake percent.
	int										m_nRakeAmountMax;			// Maximum rake amount.
	BYTE									m_cCurrencyId;				// Currency id.
	BYTE									m_cGameSpeedId;				// Game speed id.
	List<PokerRoundAmountStruct>			m_listRoundAmounts;			// List of round amount info.
	EnumerableObject<PokerChatMessage>		m_listChatItems;			// List of chat items.
	EnumerableObject<PokerPotAmountDetail>	m_listPots;					// List of pots.

	int										m_nMaxChatMessageCt;		// Max chat message count.
	DateTime								m_dTimeStartHand;			// Start hand datetime.
	bool									m_bCardsShown;				// Seat cards show status.

private:
	// Private members. {{
	List<PokerHandCards>					m_handCards;				// Hand cards of every in-play seats.
	int										m_nTableCard1;				// Table card1.
	int										m_nTableCard2;				// Table card2.
	int										m_nTableCard3;				// Table card3.
	int										m_nTableCard4;				// Table card4.
	int										m_nTableCard5;				// Table card5.
	// }}


#ifdef _SERVER
public:
	PokerTableSeatInfo*		GetSeatByConnection		(ClientConnection* pConn);
	PokerTableSeatInfo*		GetSeatBySession		(Session* pSess);
	int						EmptyOfflineSeats		(bool bEmpty, CDWordArray* pArrSeatIndexes = NULL);

	// Clear statistic info.
	void					ClearStatInfo			();
	// Returns side pot count.
	int						GetSidePots				(AutoSortedArray& arrPotAmountBySeatIndex, int& nSeatMaxPot);
	// Normalize seat pots just before round finishes.
	int						NormalizeSeatPots		(EnumerableObject<PokerSeatAmountInfo>* pSeatsAmountChanged);
	// First pot is main pot others are side pots by index. {{
	// Calculate side pots and put them into list.
	int						CalculateSidePots		(List<SidePotInfoStruct>& listSidePots);
	// Retrieve seat belonging side pots.
	int						GetSeatSidePots			(List<SidePotInfoStruct>& listSidePots, int nSeat, AutoSortedArray& arrSidePotAmountByPotIndex);
	// }}

	int						GetDetailedPots			(EnumerableObject<PokerPotAmountDetail>* pListPost);
	HandWinnerCalculator*	GetHandWinnerCalculator	();
	int						CalculateRakeAmount		(int nTotalPotAmount);

	int						GetSeatsHandCards		(EnumerableObject<PokerHandCards>* pListHandCards, CDWordArray* pArrAskShowMuckCards, AutoSortedArray* pArrWinnerSeats = NULL);
	bool					GetSeatHandCards		(int nSeat, PokerHandCards* pHandCards);
#endif

public:
	int						GetTableCard1			(){return m_nTableCard1;};
	int						GetTableCard2			(){return m_nTableCard2;};
	int						GetTableCard3			(){return m_nTableCard3;};
	int						GetTableCard4			(){return m_nTableCard4;};
	int						GetTableCard5			(){return m_nTableCard5;};

	void					AddChatMessage			(DateTime dtTime, int nSeat, PokerActionTypes acType, int nAmount, bool bSystem = false, std::string sMessage = _T(""));
	void					ClearChatMessages		(){m_listChatItems.RemoveAll(true);};

public:
	void					StartHand				(int nDealerSeat, int nSBSeat, int nBBSeat);
	void					FinishHand				(bool bShowdown);
	void					StartRound				(PokerRoundType round);

	int						ShowSeatCards			(bool bShow = true);

	int						PayAnte					();	

	bool					SetAvatarInfo			(int nSeat, PokerAvatar* pAvatar);
	PokerAvatar*			GetAvatarInfo			(int nSeat);

	int						GetSitOutSeats			(CDWordArray& arrSeatIndexes, bool bExcludeEmptyBalanceSeats = false);
	int						GetEmptyBalanceSeats	(CDWordArray& arrSeatIndexes);

	int						GetSittedCt				();
	int						GetSeatCtByStatus		(PokerSeatStatus status, CPtrArray* pArrSeatInfos = NULL);

	int						GetBalance				(int nSeat);
	bool					SetBalance				(int nSeat, int nAmount);
	void					SetHandWonState			(int nSeat, bool bWon);

	int						GetSeatAmounts			(EnumerableObject<PokerSeatAmountInfo>* seatAmounts);

	std::string				GetSeatUserName			(int nSeat);
	ClientConnection*		GetConnection			(int nSeat);

	int						GetAddedAmount			(int nSeat);
	bool					Add_AddedAmount			(int nSeat, int nAmount);
	bool					SetAddedAmount			(int nSeat, int nAmount);

	int						GetMainPotAmount		()					{return m_nMainPotAmount;};
	void					SetMainPotAmount		(int nMainPot)		{m_nMainPotAmount = nMainPot;};
	void					AddMainPotAmount		(int nAmountAdd)	{m_nMainPotAmount += nAmountAdd;};

	int						GetRakeAmount			()					{return m_nRakeAmount;};

	int						GetTotalPotAmount		()					{return m_nTotalPotAmount;};
	void					SetTotalPotAmount		(int nTotalPot)		{m_nTotalPotAmount = nTotalPot;};
	void					AddTotalPotAmount		(int nAmountAdd)	{m_nTotalPotAmount += nAmountAdd;};

	bool					AddPotAmount			(int nSeat, int nAmount);
	int						GetPotAmount			(int nSeat);
	bool					SetPotAmount			(int nSeat, int nPotAmount);
	int						GetSeatTotalPot			(int nSeat);
	bool					SetSeatTotalPot			(int nSeat, int nTotalPotAmount);
	void					ClearPotAmount			(int nSeat);
	int						ClearPotAmounts			();

	bool					GetRoundAmounts			(PokerRoundType round, int& nTotalPot, int& nRoundPot, int& nRakeAmount);
	bool					GetBetAmounts			(int nSeat, int& nMinBet, int& nMaxBet);
	int						GetCallAmount			(int nSeat);

	int						GetSeats				(CPtrArray& arrSeats);

	PokerTableSeatInfo*		GetSeatByUserId			(int nUserId);
	int						GetFirstEmptySeat		();

	PokerTableSeatInfo*		GetDealerSeat			(int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetSBSeat				(int nDealerSeat, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetBBSeat				(int nSBSeat, int* pSeatIndex = NULL);
	int						GetPayBBAndPlaySeats	(int nBBSeat, int nDealerSeat, CDWordArray& arrPayBBSeats);				

	int						GetEmptySeatCt			();
	PokerSeatStatus			GetSeatStatus			(int nSeatIndex);
	bool					EmptySeat				(int nSeat, bool bDestroySeatObject = true);
	bool					GetSeatWithIp			(DWORD dwIp, int& nSeat);
	PokerTableSeatInfo*		GetSeatInfo				(int nSeatIndex) {return m_seats.GetAt(nSeatIndex);};
	bool					SetSeatInfo				(int nSeatIndex, PokerTableSeatInfo* pSeatInfo);
	PokerTableSeatInfo*		GetNextSeatInfo			(int nSeatIndex, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetNextSeatInfoByStatus	(int nSeatIndex, PokerSeatStatus status, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetNextSeatedSeatInfo	(int nSeatIndex, int* pSeatIndex = NULL);
	
	PokerTableSeatInfo*		GetPrevSeatInfo			(int nSeatIndex, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetPrevSeatInfoByStatus	(int nSeatIndex, PokerSeatStatus status, int* pSeatIndex = NULL);
	int						CountSeatsByStatus		(PokerSeatStatus status);
	PokerTableSeatInfo*		GetMaxPotAmount			(int& nPotAmount, int& nMinPot); // Seat with maximal pot amount.
	PokerTableSeatInfo*		GetNextActiveSeat		(int nSeatPrev, int& nSeatIndex, bool& bIsRoundEnding, bool& bOpenSeatCards);

	bool					IsBetweenSeats			(int nSeat, int nSeatStart, int nSeatEnd);

	void					ClearTableCards			();
	void					ClearSeatCards			();

	void					FalsePreActions			();
	bool					GetSeatPreActions		(int nSeat, EnumerableObject<PokerPreAction>* pListPreActions);

	bool					SitOut					(int nSeat);
	bool					FoldSeat				(int nSeat);
	int						SetSeatStatus			(PokerSeatStatus destStatus, PokerSeatStatus status);
	bool					SetSeatStatus			(int nSeat, PokerSeatStatus status);
	bool					SetLastAction			(int nSeat, PokerActionTypes action);

	bool					SetSitOutNextHand		(int nSeat, bool bValue);
	bool					SetSitOutNextBB			(int nSeat, bool bValue);
	bool					GetSitOutNextHand		(int nSeat);
	bool					GetSitOutNextBB			(int nSeat);					

	PokerActionTypes		GetLastActionType		(int nSeat);
	bool					ReturnBack				(int nSeat);

	int						GetTimeBankSeconds		(int nSeat);
	void					SetTimeBankSeconds		(int nSeat, int nTimeBankSec);
	float					GetCollectedTimeBank	(int nSeat);
	void					SetCollectedTimeBank	(int nSeat, float fTimeBankSec);

	static int				CardIndexByName			(std::string sName);

public:
	virtual bool			Copy					(Serializable* pInto);
	virtual Serializable*	CreateSerializableObject(){return new PokerTableInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};