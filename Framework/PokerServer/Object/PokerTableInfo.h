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
	INT_CHIPS	nTotalPotAmount;
	INT_CHIPS	nRoundPotAmount;
	UINT		nRakeAmount;
	};

struct SidePotInfoStruct{
	INT_CHIPS	nPotAmount;					// Side pot amount.
	int			nArrSeatsBelongsTo[10];		// Array of pot belonging seats.
	int			nSeatsCtBelongsTo;			// Count of pot belonging seats.
	};

struct HandPlayersInfoStruct{
	_String			sUserName;
	int				nUserId;
	BYTE			btCard1;
	BYTE			btCard2;
	BYTE			btCard3;
	BYTE			btCard4;
	INT_CHIPS		nBalanceAmount;
	INT_CHIPS		nPotAmount;
	INT_CHIPS		nTotalPotAmount;
	PokerSeatStatus	status;
	};

class HandWinnerCalculator;
class PokerSeatAmountInfo;
class PokerSeatStateInfo;
class PokerPreAction;

class PokerTableInfo : public Serializable
{
public:
	PokerTableInfo();
	~PokerTableInfo();

public:
	int										m_nTableId;					// Table id.
	_String									m_sTableName;				// Table name.
	int										m_nTournamentId;			// Tournament id.
	bool									m_bTournamentTable;			// Tournament table.

	// Statistic info. {{
	COleDateTime							m_dtStatStarted;			// Statistic info gathering started.
	int										m_nPlayedHandsCt;			// Played hand count.

	int										m_nPlayersCtStarted;		// Players count started.
	int										m_nFlopPlayersCt;			// Flop players count.

	int										m_nSumPlayersCtStarted;		// Sum of started players counts.
	int										m_nSumFlopPlayersCt;		// Sum of Flop players counts.
	int										m_nSumPlayedHandsDurSec;	// Sum of played hand durations in seconds.

	double									m_dAvgPotAmount;			// Average pot amount.
	double									m_dAvgPlayers;				// Average player count.
	double									m_dAvgHandsPerHour;			// Played hands count per hour.
	double									m_dPlayerFlop;				// Player/Flop percentage.
	int										m_nAvgHandDurationSec;		// Average hand duration in seconds.
	// }}

	char									m_cMaxPlayersCt;
	EnumerableObject<PokerTableSeatInfo>	m_seats;
	EnumerableObject<PokerAvatar>			m_listSeatAvatars;			// Avatars by seat index.
	INT_CHIPS								m_nTotalPotAmount;			// Total pot amount.
	INT_CHIPS								m_nMainPotAmount;			// Main pot amount.
	UINT									m_nRakeAmount;				// Rake amount.
	UINT									m_nFineAmount;				// Fine amount.

	// Basic seat positions.
	char									m_cActiveSeat;				// Active seat index.
	char									m_cDealerSeat;				// Dealer seat index.
	char									m_cSBSeat;					// SB seat index.
	char									m_cBBSeat;					// BB seat index.

	BYTE									m_btRegularActTimeSec;		// Regular action time seconds.
	BYTE									m_btExtraActTimeSec;		// Extra action time seconds.

	// Table cards.
	char									m_cCard1;					// Table card1.
	char									m_cCard2;					// Table card2.
	char									m_cCard3;					// Table card3.
	char									m_cCard4;					// Table card4.
	char									m_cCard5;					// Table card5.
	PokerGameStatus							m_gameStatus;				// Poker game status.
	PokerRoundType							m_round;					// Poker round.

	UINT									m_nSmallBlindAmount;		// Small blind amount.
	UINT									m_nBigBlindAmount;			// Big blind amount.
	UINT									m_nAnteAmount;				// Ante amount.
	UINT									m_nMinBuyIn;				// Min buy in amount
	UINT									m_nMaxBuyIn;				// Max buy in amount
	short									m_shTimeBankSec;			// Time bank seconds.
	BYTE									m_cGameTypeId;				// Poker game type id. (Holdem, Omaha etc.)
	short									m_shSubTypeId;				// Game sub type id.
	BYTE									m_cMoneyTypeId;				// Money type id.
	float									m_fRakePercent;				// Rake percent.
	UINT									m_nRakeAmountMax;			// Maximum rake amount.
	BYTE									m_cCurrencyId;				// Currency id.
	BYTE									m_cGameSpeedId;				// Game speed id.
	List<PokerRoundAmountStruct>			m_listRoundAmounts;			// List of round amount info.
	EnumerableObject<PokerChatMessage>		m_listChatItems;			// List of chat items.
	EnumerableObject<PokerPotAmountDetail>	m_listPots;					// List of pots.

	int										m_nMaxChatMessageCt;		// Max chat message count.
	COleDateTime							m_dTimeStartHand;			// Start hand datetime.
	bool									m_bCardsShown;				// Seat cards show status.

private:
	// Private members. {{
	List<PokerHandCards>					m_handCards;				// Hand cards of every in-play seats.
	int										m_nTableCard1;				// Table card1.
	int										m_nTableCard2;				// Table card2.
	int										m_nTableCard3;				// Table card3.
	int										m_nTableCard4;				// Table card4.
	int										m_nTableCard5;				// Table card5.

	int										m_nSeatRefusedToPaySB;
	// }}

#ifdef _SERVER
public:
	HandPlayersInfoStruct*	GetHandPlayerInfo(int nSeat){ 
		if( m_arrHandPlayersInfo[nSeat].sUserName.IsEmpty() )
			return NULL;
		return &m_arrHandPlayersInfo[nSeat]; 
		};
private:
	HandPlayersInfoStruct					m_arrHandPlayersInfo[10];	// Hand players info to keep until hand ends.					
	int										m_nBetRaiseCtPerRound;		// Bet/Raise action count per round.

public:
	PokerTableSeatInfo*		GetSeatByConnection		(ClientConnection* pConn);
	PokerTableSeatInfo*		GetSeatBySession		(Session* pSess);
	int						EmptyOfflineSeats		(bool bEmpty, CDWordArray* pArrSeatIndexes = NULL);

	// Clear statistic info.
	void					ClearStatInfo			();
	// Returns side pot count.
	int						GetSidePots				(AutoSortedArray& arrPotAmountBySeatIndex, INT_CHIPS& nSeatMaxPot);
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

	int						GetSeatsHandCards		(EnumerableObject<PokerHandCards>* pListHandCards, CDWordArray* pArrAskShowMuckCards, AutoSortedArray* pArrWinnerSeats = NULL);
	bool					GetSeatHandCards		(int nSeat, PokerHandCards* pHandCards);

	// Seat action callback
	virtual void			OnSeatAction			(int nSeat, PokerActionTypes actionType, INT_CHIPS nAmount);
	int						GetSBAmount				();
	int						GetBBAmount				();

	void					SetGameStatus			(PokerGameStatus status){ m_gameStatus = status; };
	PokerGameStatus			GetGameStatus			(){ return m_gameStatus; };
#endif
	UINT					CalculateRakeAmount		(INT_CHIPS nTotalPotAmount);

public:
	int						GetTableCard1			(){return m_nTableCard1;};
	int						GetTableCard2			(){return m_nTableCard2;};
	int						GetTableCard3			(){return m_nTableCard3;};
	int						GetTableCard4			(){return m_nTableCard4;};
	int						GetTableCard5			(){return m_nTableCard5;};

	void					AddChatMessage			(DateTime dtTime, int nSeat, PokerActionTypes acType, INT_CHIPS nAmount, bool bSystem = false, _String sMessage = _T(""));
	void					ClearChatMessages		(){m_listChatItems.RemoveAll(true);};

public:
	void					StartHand				(int nDealerSeat, int nSBSeat, int nBBSeat, int nSeatRefusedToPaySB = -1);
	void					FinishHand				(bool bShowdown);
	void					StartRound				(PokerRoundType round);

	bool					CanEmptySeatWhenLeave	(int nSeat);

	int						ShowSeatCards			(bool bShow = true);

	int						PayAnte					();	

	bool					SetAvatarInfo			(int nSeat, PokerAvatar* pAvatar);
	PokerAvatar*			GetAvatarInfo			(int nSeat);

	int						GetSitOutNextHandSeats	(CDWordArray& arrSeatIndexes, bool bExcludeEmptyBalanceSeats = false);
	int						GetEmptyBalanceSeats	(CDWordArray& arrSeatIndexes, bool bExcludeSitoutSeats = false);

	int						GetSittedCt				();
	int						GetSeatCtByStatus		(PokerSeatStatus status, CPtrArray* pArrSeatInfos = NULL);

	INT_CHIPS				GetBalance				(int nSeat);
	bool					SetBalance				(int nSeat, INT_CHIPS nAmount);
	void					SetHandWonState			(int nSeat, bool bWon);

	int						GetSeatAmounts			(EnumerableObject<PokerSeatAmountInfo>* seatAmounts, bool bAddEmptyInfos = false);
	int						GetSeatStatuses			(EnumerableObject<PokerSeatStateInfo>* seatStatuses);

	_String					GetSeatUserName			(int nSeat);
	ClientConnection*		GetConnection			(int nSeat);

	INT_CHIPS				GetAddedAmount			(int nSeat);
	bool					Add_AddedAmount			(int nSeat, INT_CHIPS nAmount);
	bool					SetAddedAmount			(int nSeat, INT_CHIPS nAmount);

	INT_CHIPS				GetMainPotAmount		()						{return m_nMainPotAmount;};
	void					SetMainPotAmount		(INT_CHIPS nMainPot)	{m_nMainPotAmount = nMainPot;};
	void					AddMainPotAmount		(INT_CHIPS nAmountAdd)	{m_nMainPotAmount += nAmountAdd;};

	UINT					GetRakeAmount			()						{return m_nRakeAmount;};
	void					SetRakeAmount			(UINT nRakeAmount)		{m_nRakeAmount = nRakeAmount;};

	INT_CHIPS				GetTotalPotAmount		()						{return m_nTotalPotAmount;};
	void					SetTotalPotAmount		(INT_CHIPS nTotalPot)	{m_nTotalPotAmount = nTotalPot;};
	void					AddTotalPotAmount		(INT_CHIPS nAmountAdd)	{m_nTotalPotAmount += nAmountAdd;};

	void					AddFineAmount			(UINT nAmount)			{m_nFineAmount += nAmount; AddTotalPotAmount(nAmount); AddMainPotAmount(nAmount);};
	UINT					GetFineAmount			()						{return m_nFineAmount;};

	bool					AddPotAmount			(int nSeat, INT_CHIPS nAmount);
	INT_CHIPS				GetPotAmount			(int nSeat);
	bool					SetPotAmount			(int nSeat, INT_CHIPS nPotAmount);
	INT_CHIPS				GetSeatTotalPot			(int nSeat);
	bool					SetSeatTotalPot			(int nSeat, INT_CHIPS nTotalPotAmount);
	void					ClearPotAmount			(int nSeat);
	int						ClearPotAmounts			();

	bool					GetRoundAmounts			(PokerRoundType round, INT_CHIPS& nTotalPot, INT_CHIPS& nRoundPot, UINT& nRakeAmount);
	bool					GetBetAmounts			(int nSeat, INT_CHIPS& nMinBet, INT_CHIPS& nMaxBet);
	INT_CHIPS				GetCallAmount			(int nSeat);

	int						GetSeats				(CPtrArray& arrSeats);

	PokerTableSeatInfo*		GetSeatByUserId			(int nUserId);
	int						GetFirstEmptySeat		();

	PokerTableSeatInfo*		GetDealerSeat			(int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetSBSeat				(int nDealerSeat, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetBBSeat				(int nSBSeat, int nDealerSeat, int* pSeatIndex = NULL);
	int						GetBBAskSeats			(int nSBSeat, int nDealerSeat, CDWordArray* pArrSeatIndexes = NULL);
	int						GetPayBBAndPlaySeats	(int nBBSeat, int nDealerSeat, CDWordArray& arrPayBBSeats);				

	int						GetEmptySeatCt			();
	PokerSeatStatus			GetSeatStatus			(int nSeatIndex);
	bool					EmptySeat				(int nSeat, bool bDestroySeatObject = true);
	bool					GetSeatWithIp			(DWORD dwIp, int& nSeat);
	PokerTableSeatInfo*		GetSeatInfo				(int nSeatIndex) {return m_seats.GetAt(nSeatIndex);};
	bool					SetSeatInfo				(int nSeatIndex, PokerTableSeatInfo* pSeatInfo);
	PokerTableSeatInfo*		GetNextSeatInfo			(int nSeatIndex, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetNextSeatInfoByStatus	(int nSeatIndex, PokerSeatStatus status, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetNextSitoutButInplaySeat(int nSeatIndex, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetNextSeatedSeatInfo	(int nSeatIndex, int* pSeatIndex = NULL);
	
	PokerTableSeatInfo*		GetPrevSeatInfo			(int nSeatIndex, int* pSeatIndex = NULL);
	PokerTableSeatInfo*		GetPrevSeatInfoByStatus	(int nSeatIndex, PokerSeatStatus status, int* pSeatIndex = NULL);
	int						CountSeatsByStatus		(PokerSeatStatus status);
	int						CountSitoutButInPlaySeats();
	PokerTableSeatInfo*		GetMaxPotAmount			(INT_CHIPS& nPotAmount, INT_CHIPS& nMinPot); // Seat with maximal pot amount.
	PokerTableSeatInfo*		GetNextActiveSeat		(int nSeatPrev, int& nSeatIndex, bool& bIsRoundEnding, bool& bOpenSeatCards);

	bool					IsBetweenSeats			(int nSeat, int nSeatStart, int nSeatEnd);
	int						GetSeatBetweenSeatsByStatusClockwise(CPtrArray* pArrResult, int nSeatStart, int nSeatEnd, PokerSeatStatus status);
	int						GetSeatBetweenSeatsClockwise(CPtrArray* pArrResult, int nSeatStart, int nSeatEnd);

	bool					IsInPlayByStatus		(int nSeat, PokerSeatStatus status){ 
		if( m_bTournamentTable )
			return (status == PokerSeatStatus::InPlay || status == PokerSeatStatus::All_In || status == PokerSeatStatus::SittedOut);
		return (status == PokerSeatStatus::InPlay || status == PokerSeatStatus::All_In || (status == PokerSeatStatus::SittedOut && GetSitOutButInPlay(nSeat)) ); 
		};

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
	bool					SetSitOutButInPlay		(int nSeat, bool bValue);
	bool					GetSitOutButInPlay		(int nSeat);
	PokerActionTypes		GetLastActionType		(int nSeat);
	bool					ReturnBack				(int nSeat);

	int						GetTimeBankSeconds		(int nSeat);
	void					SetTimeBankSeconds		(int nSeat, int nTimeBankSec);
	float					GetCollectedTimeBank	(int nSeat);
	void					SetCollectedTimeBank	(int nSeat, float fTimeBankSec);

	static int				CardIndexByName			(_String sName);

public:
	virtual bool			Copy					(Serializable* pInto);
	virtual Serializable*	CreateSerializableObject(){return new PokerTableInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};