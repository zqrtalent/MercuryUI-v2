#pragma once
#include "BasePacket.h"
#include "_PokerDefines.h"
#include "../PokerServer/Object/PokerTableSeatInfo.h"
#include "../PokerServer/Object/PokerTableInfo.h"
#include "../PokerServer/Object/PokerHandCards.h"
#include "../PokerServer/Object/PokerPotAmountDetail.h"
#include "../PokerServer/Object/UserInfo.h"
#include "../PokerServer/Object/LobbyCashTableInfo.h"
#include "../PokerServer/Object/LobbyTournamentInfo.h"
#include "../PokerServer/Object/LobbyTournamentSummaryInfo.h"
#include "../PokerServer/Object/PokerHandWinnerInfo.h"
#include "../PokerServer/Object/PokerHandHistory.h"

class PokerGetLobbyInfoPacket : public BasePacket
{
public:
	PokerGetLobbyInfoPacket();
	virtual ~PokerGetLobbyInfoPacket();

	enum {
		LobbyInfoReq_CashTablesStaticInfo	= 1,
		LobbyInfoReq_CashTablesDynamicInfo	= 2,
		LobbyInfoReq_CashTablePlayers		= 4,
		LobbyInfoReq_TournamentInfos		= 8,
		LobbyInfoReq_TournamentDynamicInfos	= 16
		};

public:
	BYTE											m_cMoneyType;					// [IN] Money type.
	BYTE											m_cGameType;					// [IN] Game type.
	short											m_shSubType;					// [IN]	Game sub type

	BYTE											m_cRequestFlag;					// [IN] Lobby info request flag. (One of these flags above)
	int												m_nTableId;						// [IN] Table id. (Existance of that parameter, depends on request flag).

	int												m_nPlayersCt;					// [OUT] Players count.
	short											m_shTablesCt;					// [OUT] Tables count.
	short											m_shTournamentsCt;				// [OUT] Tournaments count.

	short											m_shTablesCtByRequest;			// [OUT] Tables count by request filter.
	short											m_shTournamentsCtByRequest;		// [OUT] Tournaments count by request filter.

	INT_CHIPS										m_nInPlayAmount;				// [OUT] in-play amount (Real money tables only).
	INT_CHIPS										m_nBalance;						// [OUT] Balance.
	INT_CHIPS										m_nPlayBalance;					// [OUT] Play-money balance.
	UINT											m_nBadBeatJackpotAmount;		// [OUT] BadBeat jackpot amount.

	EnumerableObject<LobbyCashTableStaticInfo>		m_listCashTablesStaticInfo;		// [OUT] Cash table static info.
	EnumerableObject<LobbyCashTableDynamicInfo>		m_listCashTablesDynamicInfo;	// [OUT] Cash table dynamic info.
	EnumerableObject<LobbyCashTablePlayerInfo>		m_listCashTablePlayersInfo;		// [OUT] Cash table players list.
	EnumerableObject<LobbyTournamentInfo>			m_listTournamentInfo;			// [OUT] Tournament info.
	EnumerableObject<LobbyTournamentDynamicInfo>	m_listTournamentDynamicInfo;	// [OUT] Tournament dynamic info.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerGetLobbyInfoPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerOpenTablePacket : public BasePacket
{
public:
	PokerOpenTablePacket();
	virtual ~PokerOpenTablePacket();
public:
	int									m_nTableId;					// [IN] Table id.
	bool								m_bSeatOrJoinWaitList;		// [IN] Seat or join waiting list when open table.
	__int64								m_nHandId;					// [OUT] Current hand id.
	EnumerableObject<PokerTableInfo>	m_info;						// [OUT] Table info.
	BYTE								m_cActiveSeatTimeFull;		// [OUT] Active seat think time seconds full.
	BYTE								m_cActiveSeatTimePassed;	// [OUT] Active seat think time seconds passed.
	char								m_cSeat;					// [IN/OUT] [IN]Open table and seat at the seat by index m_bSeatOrJoinWaitList must be true.  [OUT]Seat index of current session.	

	// Table statistic info.
	int									m_nHandsDealed;				// [OUT] Hands count dealed.
	int									m_nHandsPlayed;				// [OUT] Hands count played.
	int									m_nHandsWon;				// [OUT] Hands count won.
	int									m_nHandsWonPreflop;			// [OUT] Hands count won preflop.
	int									m_nHandsWonPostflop;		// [OUT] Hands count won postflop.
	int									m_nHandsWonShowdown;		// [OUT] Hands count won show down.

	virtual Serializable* CreateSerializableObject(){return new PokerOpenTablePacket(); };
protected: // Serialization.
	INIT_RUNTIME_VARIABLE()
};

class PokerLeaveTablePacket : public BasePacket
{
public:
	PokerLeaveTablePacket();
	virtual ~PokerLeaveTablePacket();
public:
	int		m_nTableId;	// [IN]	Table id.
	char	m_cSeat;	// [OUT] Seat index.
	
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerLeaveTablePacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerTableClosedPacket : public BasePacket
{
public:
	PokerTableClosedPacket();
	virtual ~PokerTableClosedPacket();
public:
	int									m_nTournamentId;			// [OUT] Tournament id.
	int									m_nTableId;					// [OUT] Table id.
	
	virtual Serializable* CreateSerializableObject(){return new PokerTableClosedPacket(); };
protected: // Serialization.
	INIT_RUNTIME_VARIABLE()
};

class PokerReserveSeatPacket : public BasePacket
{
public:
	PokerReserveSeatPacket();
	virtual ~PokerReserveSeatPacket();

public:
	int									m_nTableId;				// [IN] Table Id.
	char								m_cSeat;				// [IN] Seat index.
	_String								m_sUserName;			// [OUT] User name.
	INT_CHIPS							m_nBalance;				// [OUT] User's balance.
	INT_CHIPS							m_nAmountMin;			// [OUT] Min amount to add.
	INT_CHIPS							m_nAmountMax;			// [OUT] Max amount to add.
	short								m_shReservationTimeSec;	// [OUT] Reservation time seconds.
	bool								m_bReservationExpired;	// [OUT] Reserve expiration status.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerReserveSeatPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerCancelReservationPacket : public BasePacket
{
public:
	PokerCancelReservationPacket();
	virtual ~PokerCancelReservationPacket();

public:
	int									m_nTableId;				// [IN] Table Id.
	char								m_cSeat;				// [IN] Seat index.
	
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerCancelReservationPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSeatAtTablePacket : public BasePacket
{
public:
	PokerSeatAtTablePacket();
	virtual ~PokerSeatAtTablePacket();

public:
	int						m_nTableId;			// [IN] Table id.
	int						m_nTournamentId;	// [OUT] Tournament id.
	char					m_cSeatIndex;		// [IN]	Seat index.
	INT_CHIPS				m_nChipAmount;		// [IN]	Add chip amount.
	__int64					m_nSeatSessionId;	// [OUT] Session id of sitten user's session.
	PokerTableSeatInfo		m_seatInfo;			// [OUT] Seat info.
	_String					m_sAvatarFileName;	// [OUT] Avatar file name.
	//PokerAvatar			m_avatarData;		// [OUT] Avatar data.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSeatAtTablePacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerAddChipsPacket : public BasePacket
{
public:
	PokerAddChipsPacket();
	virtual ~PokerAddChipsPacket();

public:
	int						m_nTableId;			// [IN] Table id.
	INT_CHIPS				m_nAmount;			// [IN] Add chips amount.
	bool					m_bPrepareToAdd;	// [IN] Prepare to add chips.
	char					m_cSeat;			// [OUT] Seat index.
	INT_CHIPS				m_nBalance;			// [OUT] Balance amount.
	INT_CHIPS				m_nMin;				// [OUT] Min amount
	INT_CHIPS				m_nMax;				// [OUT] Max amount
	__int64					m_nSeatSessionId;	// [OUT] Session id of sitted user's session.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerAddChipsPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerJoinWaitingListPacket : public BasePacket
{
public:
	PokerJoinWaitingListPacket();
	virtual ~PokerJoinWaitingListPacket();

public:
	int						m_nTableId;		// [IN] table id.
	_String					m_sUserName;	// [IN]	User name. {Isn't nacessary}
	int						m_nUserId;		// [IN] User id. {Isn't nacessary}
	short					m_shIndex;		// [OUT] Waiting list index number.
	
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerJoinWaitingListPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerPreAction : public Serializable
{
public:
	PokerPreAction();
	virtual ~PokerPreAction();

public:
	char					m_cActionType;		// Pre-Action type. (PokerTableOption)
	bool					m_bValue;			// Pre-action status.
	INT_CHIPS				m_nValue;			// Amount.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerPreAction(); };
	INIT_RUNTIME_VARIABLE()
};

// This packet are sent to every one who is eligable to see the game table.
class PokerActionPacket : public BasePacket
{
public:
	PokerActionPacket();
	virtual ~PokerActionPacket();

public:
	int										m_nTableId;					// [IN] Table Id.
	int										m_nTournamentId;			// [IN] Tournament Id.
	char									m_cActionType;				// [IN] Poker action types (PokerActionTypes).
	INT_CHIPS								m_nAmount;					// [IN]	Action amount.
	char									m_cSeat;					// [IN] Seat index.
	DateTime								m_dtAction;					// [OUT] Action time. (Server time)
	char									m_cSeatReceiver;			// [OUT] Packet reciever seat index. wher seated but not in play -1
	char									m_cSeatStatus;				// [OUT] Seat status (PokerSeatStatus).
	INT_CHIPS								m_nBalance;					// [OUT] Seat balance amount.
	INT_CHIPS								m_nPotAmount;				// [OUT] Pot amount of seat.
	INT_CHIPS								m_nSeatTotalPot;			// [OUT] Seat total pot amount.
	INT_CHIPS								m_nTotalPot;				// [OUT] Total pot amount of table.
	bool									m_bSetPreActions;			// [OUT] Set Pre-Actions.
	bool									m_bRoundFinished;			// [OUT] Round finish flag.
	EnumerableObject<PokerPreAction>		m_listPreActions;			// [OUT] Pre-Actions.
	EnumerableObject<PokerHandCards>		m_listCardsShown;			// [OUT] Hand cards shown by seat.
	EnumerableObject<PokerSeatAmountInfo>	m_listSeatChangedAmounts;	// [OUT] Seat amounts changed.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerActionPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerAction : public Serializable
{
public:
	PokerAction();
	virtual ~PokerAction();

public:
	char							m_cActionType;		// Action type. (PokerActionTypes)
	INT_CHIPS						m_nAmountMin;		// Amount min.
	INT_CHIPS						m_nAmountMax;		// Amount max.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerAction();};
	INIT_RUNTIME_VARIABLE()
};

class PokerActPacket : public BasePacket
{
public:
	PokerActPacket();
	virtual ~PokerActPacket();

public:
	int								m_nTableId;		// [IN] Table Id.
	int								m_nTournamentId;// [IN] Tournament Id.
	__int64							m_nHandId;		// [IN] Hand Id.
	char							m_cSeat;		// [OUT] Seat index.
	char							m_cActionTime;	// [OUT] Action time seconds.
	EnumerableObject<PokerAction>	m_actions;		// [OUT] List of actions available for client.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerActPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerTableInfoPacket : public PokerOpenTablePacket
{
public:
	PokerTableInfoPacket();
	virtual ~PokerTableInfoPacket();
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerTableInfoPacket();};
};

class PokerSeatAmountInfo : public Serializable
{
public:
	PokerSeatAmountInfo();
	virtual ~PokerSeatAmountInfo();

	char								m_cSeat;			// [OUT] Seat index.
	INT_CHIPS							m_nPotAmount;		// [OUT] Pot amount.
	INT_CHIPS							m_nTotalPotAmount;	// [OUT] Total pot amount.
	INT_CHIPS							m_nBalance;			// [OUT] Balance amount.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSeatAmountInfo(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerSeatStateInfo : public Serializable
{
public:
	PokerSeatStateInfo();
	virtual ~PokerSeatStateInfo();

	char								m_cSeat;			// [OUT] Seat index.
	char								m_cStatus;			// [OUT] Seat status.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSeatStateInfo(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerHandStartedPacket : public BasePacket
{
public:
	PokerHandStartedPacket();
	virtual ~PokerHandStartedPacket();

public:
	int										m_nTableId;			// [OUT] Table Id.
	int										m_nTournamentId;	// [OUT] Tournament Id.
	__int64									m_nHandId;			// [OUT] Recently started hand id.
	char									m_cBigBlind;		// [OUT] BigBlind seat index.
	char									m_cSmallBlind;		// [OUT] SmallBlind seat index.
	char									m_cDealer;			// [OUT] Dealer seat index.
	char									m_cSeatReceiver;	// [OUT] Packet reciever seat index. wher seated but not in play -1
	EnumerableObject<PokerHandCards>		m_handCards;		// [OUT] Hand cards.
	EnumerableObject<PokerSeatAmountInfo>	m_seatAmounts;		// [OUT] List of seat amounts (Only sent when ante is paid).
	EnumerableObject<PokerSeatStateInfo>	m_listSeatStatus;	// [OUT] List of seat statuses.
	INT_CHIPS								m_nTotalPotAmount;	// [OUT] Total pot amount.
	INT_CHIPS								m_nMainPotAmount;	// [OUT] Main pot amount.
	DateTime								m_dtAction;			// [OUT] Action time. (Server time)

	// Tournament table params.
	int										m_nRankIndex;		// [OUT] Rank index.
	int										m_nPlayersCt;		// [OUT] Tournament players count.
	WORD									m_shTablesCt;		// [OUT] Tournament tables count.

protected: // Serialization.
	virtual Serializable*	CreateSerializableObject(){return new PokerHandStartedPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerHandFinishedPacket : public BasePacket
{
public:
	PokerHandFinishedPacket();
	virtual ~PokerHandFinishedPacket();

public:
	int										m_nTableId;				// [OUT] Table Id.
	int										m_nTournamentId;		// [OUT] Tournament Id.
	__int64									m_nHandId;				// [OUT] Recently started hand id.
	char									m_cHandStartSec;		// [OUT] Hand start seconds.
	bool									m_bShowdown;			// [OUT] Showdown indicator.
	bool									m_bTableCardsBestComb;	// [OUT] Table cards is the best combination.
	EnumerableObject<PokerHandWinnerInfo>	m_winners;				// [OUT] Winners.
	EnumerableObject<PokerHandCards>		m_handCards;			// [OUT] Hand cards.
	DateTime								m_dtAction;				// [OUT] Action time. (Server time) 
	UINT									m_wRakeAmount;			// [OUT] Rake amount.
	EnumerableObject<PokerPotAmountDetail>	m_listPotAmounts;		// [OUT] Detailed pot amounts.

	PokerHandHistory						m_handHistory;			// [OUT] Hand history info.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerHandFinishedPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerRoundStartedPacket : public BasePacket
{
public:
	PokerRoundStartedPacket();
	virtual ~PokerRoundStartedPacket();

public:
	int										m_nTableId;				// [OUT] Table Id.
	int										m_nTournamentId;		// [OUT] Tournament Id.
	__int64									m_nHandId;				// [OUT] Recently started hand id.
	__int64									m_nReceiverSession;		// [OUT] Packet receiver's session id.
	char									m_cReceiverSeat;		// [OUT] Packet receiver seat index.	
	char									m_cRoundType;			// [OUT] Round type. (PokerRoundType)
	INT_CHIPS								m_nMainPotAmount;		// [OUT] Main pot amount (Rake amount is included).
	INT_CHIPS								m_nTotalPotAmount;		// [OUT] Total pot amount.
	char									m_cRoundStartSec;		// [OUT] Round start elapse seconds.
	EnumerableObject<PokerPreAction>		m_listPreActions;		// [OUT] pre-actions.
	BYTE									m_btTableCard1;			// [OUT] Table card1.
	BYTE									m_btTableCard2;			// [OUT] Table card2.
	BYTE									m_btTableCard3;			// [OUT] Table card3.
	BYTE									m_btTableCard4;			// [OUT] Table card4.
	BYTE									m_btTableCard5;			// [OUT] Table card5.
	char									m_cBestCardsClass;		// [OUT] Best hand cards class. PokerHandCardsClass

	// Best combinations card indexes.((0, 1, 2, 3) - identifies hand cards (4, 5, 6, 7, 8) = identifies table cards)
	char									m_cBestCard1;			// [OUT] Best 1 card index.
	char									m_cBestCard2;			// [OUT] Best 2 card index.
	char									m_cBestCard3;			// [OUT] Best 3 card index.
	char									m_cBestCard4;			// [OUT] Best 4 card index.
	char									m_cBestCard5;			// [OUT] Best 5 card index.

	// When round starts we need to update hand cards class. (especially when seat is in all-in state)
	EnumerableObject<PokerHandCards>		m_listHandCards;		// [OUT] Hand cards shown.
	DateTime								m_dtAction;				// [OUT] Action time. (Server time)

	EnumerableObject<PokerPotAmountDetail>	m_listPotAmounts;		// [OUT] Detailed pot amounts.
	UINT									m_wRakeAmount;			// [OUT] Rake amount.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerRoundStartedPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerPostBlindPacket : public BasePacket
{
public:
	PokerPostBlindPacket();
	virtual ~PokerPostBlindPacket();

public:
	int									m_nTableId;			// [IN] Table Id.
	char								m_cSeat;			// [IN] Seat index.
	char								m_cPostBlind;		// [IN] PostSB = 0, PostBB = 1, PostBB+PostSB = 2.
	short								m_shActionSec;		// [IN] Action time in seconds.
//	int									m_nAmount;			// [IN] Blind amount.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerPostBlindPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerShowMuckCardsPacket : public BasePacket
{
public:
	PokerShowMuckCardsPacket();
	virtual ~PokerShowMuckCardsPacket();

public:
	int									m_nTableId;			// [IN] Table Id.
	int									m_nTournamentId;	// [IN] Tournament Id.
	char								m_cSeat;			// [OUT] Seat index.
	bool								m_bShowCards;		// [IN] ShowCards = true, MuckCards = false.
	short								m_shActionSec;		// [OUT] Action time in seconds.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerShowMuckCardsPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSetOptionPacket : public BasePacket
{
public:
	PokerSetOptionPacket();
	virtual ~PokerSetOptionPacket();

public:
	int									m_nTableId;			// [IN] Table Id.
	int									m_nTournamentId;	// [IN] Tournament Id.
	char								m_cSeat;			// [IN] Seat index. {It is not mandatory}.
	char								m_cOption;			// [IN] PokerTableOption value.
	bool								m_bValue;			// [IN] Value true or false.
	INT_CHIPS							m_nValue;			// [IN] Value.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSetOptionPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSitOutPacket : public BasePacket
{
public:
	PokerSitOutPacket();
	virtual ~PokerSitOutPacket();

public:
	int									m_nTableId;			// [IN] Table Id.
	int									m_nTournamentId;	// [IN] Tournament Id.
	int									m_nSeat;			// [IN] Seat index.
	bool								m_bSitOut;			// [IN] Setout flag.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSitOutPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerReturnBackPacket : public BasePacket
{
public:
	PokerReturnBackPacket();
	virtual ~PokerReturnBackPacket();

public:
	int									m_nTableId;			// [IN] Table Id.
	int									m_nTournamentId;	// [IN] Tournament Id.
	char								m_cSeat;			// [IN] Seat index. {It is not mandatory}.
	bool								m_bReturnBack;		// [IN] Return back flag.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerReturnBackPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSetActivePacket : public BasePacket
{
public:
	PokerSetActivePacket();
	virtual ~PokerSetActivePacket();

public:
	int									m_nTableId;			// [IN] Table Id.
	int									m_nTournamentId;	// [IN] Tournament Id.
	char								m_cSeat;			// [IN] Active seat index.
	BYTE								m_cRegularTimeSec;	// [IN] Regular action time in seconds.
	BYTE								m_cExtraTimeSec;	// [IN] Extra action time in seconds.
	BYTE								m_cTimeBankSec;		// [IN] Time bank in seconds.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSetActivePacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSeatStatusChangedPacket : public BasePacket
{
public:
	PokerSeatStatusChangedPacket();
	virtual ~PokerSeatStatusChangedPacket();

public:
	int									m_nTableId;			// [OUT] Table Id.
	int									m_nTournamentId;	// [OUT] Tournament Id.
	char								m_cSeat;			// [OUT] Active seat index.
	_String								m_sUserName;		// [OUT] User name 
	char								m_status;			// [OUT] Seat status. (PokerSeatStatus)
	EnumerableObject<PokerAvatar>		m_seatAvatar;		// [OUT] Seat avatar.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSeatStatusChangedPacket();};
	INIT_RUNTIME_VARIABLE()
};

// From server to client only.
class PokerSeatOnlineChangedPacket : public BasePacket
{
public:
	PokerSeatOnlineChangedPacket();
	virtual ~PokerSeatOnlineChangedPacket();

public:
	int									m_nTableId;			// [IN] Table Id.
	int									m_nTournamentId;	// [IN] Tournament Id.
	char								m_cSeat;			// [IN] Active seat index.
	bool								m_bOnline;			// [IN] true = online, false = offline

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSeatOnlineChangedPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerGetHandHistoryPacket : public BasePacket
{
public:
	PokerGetHandHistoryPacket();
	virtual ~PokerGetHandHistoryPacket();

public:
	__int64								m_nHandId;			// [IN] Table Id.
	int									m_nTournamentId;	// [IN] Tournament Id.
	EnumerableObject<PokerHandHistory>	m_handData;			// [OUT] Hand info object.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerGetHandHistoryPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerQuitClientPacket : public BasePacket
{
public:
	PokerQuitClientPacket();
	virtual ~PokerQuitClientPacket();

public:
	DateTime		m_dtServerTime;	// [OUT] Server time.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerQuitClientPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSetAvatarPacket : public BasePacket
{
public:
	PokerSetAvatarPacket();
	virtual ~PokerSetAvatarPacket();

public:
	int									m_nAvatarId;		// [IN] Avatar Id.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSetAvatarPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerUploadAvatarPacket : public BasePacket
{
public:
	PokerUploadAvatarPacket();
	virtual ~PokerUploadAvatarPacket();

public:
	int									m_nAvatarId;			// [OUT] Avatar Id. (Avatar id is available when m_bCompleted will be received to client as true)
	short								m_shSize;				// [IN] Currently uploading binary data size.
	int									m_nUploaded;			// [OUT] Uploaded binary size.
	int									m_nFullBinarySize;		// [IN] Binary size of full image.
	bool								m_bCompleted;			// [OUT] Indicator of completion uploading.
	LongBinary							m_imageData;			// [OUT] Avatar image data.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerUploadAvatarPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerDownloadAvatarPacket : public BasePacket
{
public:
	PokerDownloadAvatarPacket();
	virtual ~PokerDownloadAvatarPacket();

public:
	int									m_nUserId;				// [IN] User Id.
	int									m_nAvatarId;			// [IN/OUT] Avatar Id. to receive current avatar id of requested user.
	short								m_shSize;				// [OUT] Currently downloading binary data size.
	int									m_nDownloaded;			// [OUT] Downloaded binary size.
	int									m_nFullBinarySize;		// [OUT] Binary size of full image.
	bool								m_bCompleted;			// [OUT] Indicator of completion downloading.
	LongBinary							m_imageData;			// [OUT] Avatar image data.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerDownloadAvatarPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerChatMessagePacket : public BasePacket
{
public:
	PokerChatMessagePacket();
	virtual ~PokerChatMessagePacket();

public:
	int									m_nTableId;			// [IN] Destination table id. -1 to send to every active table. (Permission needed to send to everyone)
	int									m_nTournamentId;	// [IN] Tournament Id.
	int									m_nDestUserId;		// [IN] Destination user id. -1 to send everyone on the table. (Permission needed to send to everyone)
	PokerChatMessage					m_message;			// [IN] Chat message info.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerChatMessagePacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerOpenRandomTablePacket : public BasePacket
{
public:
	PokerOpenRandomTablePacket();
	virtual ~PokerOpenRandomTablePacket();

public:
	int									m_nGameType;		// [IN] Game type id.
	int									m_nGameSubType;		// [IN] Game sub type id.
	bool								m_bReserveSeat;		// [IN] Reserve any seat.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerOpenRandomTablePacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerMoneyTransferPacket : public BasePacket
{
public:
	PokerMoneyTransferPacket();
	virtual ~PokerMoneyTransferPacket();

public:
	int									m_nTransAmount;		// [IN] Transfer money amount.
	int									m_nAccountFrom;		// [IN] Account from. (Main Balance = 1, Poker Balance 2)
	int									m_nAccountTo;		// [IN] Account to. (Main Balance = 1, Poker Balance 2)
	INT_CHIPS							m_nBalanceFrom;		// [OUT] Balance of account from.
	INT_CHIPS							m_nBalanceTo;		// [OUT] Balance of account to.
	bool								m_bRefreshBalances;	// [IN] When true no transfer is acted only retrives balance amounts.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerMoneyTransferPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerResetPlaymoneyPacket : public BasePacket
{
public:
	PokerResetPlaymoneyPacket();
	virtual ~PokerResetPlaymoneyPacket();

public:
	INT_CHIPS							m_nPlayMoneyAmount;	// [OUT] Playmoney amount after performing reset.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerResetPlaymoneyPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class PokerRefreshBalancePacket : public BasePacket
{
public:
	PokerRefreshBalancePacket();
	virtual ~PokerRefreshBalancePacket();

public:
	INT_CHIPS					m_nBalance;				// [OUT] Balance amount.
	INT_CHIPS					m_nCashInPlay;			// [OUT] Cash inplay amount.
	INT_CHIPS					m_nPlayMoneyAmount;		// [OUT] Playmoney amount.
	INT_CHIPS					m_nPlayMoneyInPlay;		// [OUT] Playmoney inplay amount.
	double						m_dVPPoints;			// [OUT] VPPoints
	char						m_cTicketsCt;			// [OUT] Tournament tickets count.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerRefreshBalancePacket();};
	INIT_RUNTIME_VARIABLE()
};

class FindPlayerTableInfo : public Serializable
{
public:
	FindPlayerTableInfo();
	virtual ~FindPlayerTableInfo();

public:
	int			m_nTableId;			// Table id
	int			m_nTournamentId;	// Tournament id;
	_String		m_sTableName;		// Table name.
	char		m_cGameType;		// Game type.
	short		m_shGameSubType;	// Game sub type.
	char		m_cMaxSeatCt;		// Max seat count.
	UINT		m_nSBAmount;		// Small blind amount.
	UINT		m_nBBAmount;		// Small blind amount.
	UINT		m_nAnteAmount;		// Ante amount.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new FindPlayerTableInfo();};
	INIT_RUNTIME_VARIABLE()
};

class PokerFindPlayerPacket : public BasePacket
{
public:
	PokerFindPlayerPacket();
	virtual ~PokerFindPlayerPacket();
public:
	_String									m_sUserName;	// [IN] User name.
	int										m_nUserId;		// [IN] User Id.
	EnumerableObject<FindPlayerTableInfo>	m_listTables;	// [OUT] List of tables where user have been found.
	
	virtual Serializable* CreateSerializableObject(){return new PokerFindPlayerPacket();};
protected: // Serialization.
	INIT_RUNTIME_VARIABLE()
};

class PokerFindTournamentPacket : public BasePacket
{
public:
	PokerFindTournamentPacket();
	virtual ~PokerFindTournamentPacket();
public:
	_String									m_sTournamentID;	// [IN] Tournament ID. ex: T0000078
	EnumerableObject<LobbyTournamentInfo>	m_listTournaments;	// [OUT] List of tournaments.
	
	virtual Serializable* CreateSerializableObject(){return new PokerFindTournamentPacket();};
protected: // Serialization.
	INIT_RUNTIME_VARIABLE()
};

class PokerTournamentCustomInfoPacket : public BasePacket
{
public:
	PokerTournamentCustomInfoPacket();
	virtual ~PokerTournamentCustomInfoPacket();

	enum {
		TournamentInfoCustom_BasicInfo				= 1,
		TournamentInfoCustom_PrizeInfo				= 2,
		TournamentInfoCustom_GameTables				= 4,
		TournamentInfoCustom_GameTablePlayers		= 8,
		TournamentInfoCustom_Participants			= 16,
		TournamentInfoCustom_BlindStructure			= 32,
		TournamentInfoCustom_TournamentInfo			= 64
		};

public:
	int												m_nTournamentId;				// [IN] Tournament id.
	int												m_nFlags;						// [IN] Custom flags.

	short											m_shGameTableIndex;				// [IN] Game table start index.
	short											m_shGameTableCt;				// [IN] Game table index.

	short											m_shParticipantIndex;			// [IN] Game table start index.
	short											m_shParticipantCt;				// [IN] Game table index.

	short											m_shGameTableIndexPlayers;		// [IN] Game table index to retrieve players.
	
	EnumerableObject<LobbyTournamentSummaryInfo>	m_listResult;			// [OUT] Result info.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerTournamentCustomInfoPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerRegisterInTournamentPacket : public BasePacket
{
public:
	PokerRegisterInTournamentPacket();
	virtual ~PokerRegisterInTournamentPacket();

	enum
	{
		RegisterBy_BalanceAmount = 1,
		RegisterBy_Points = 2,
		RegisterBy_Ticket = 3,
	};

public:
	int										m_nTournamentId;	// [IN] Tournament id.
	char									m_cRegisterBy;		// [IN] Register by flag.
	INT_CHIPS								m_nBalance;			// [OUT] Balance amount.
	double									m_dPoints;			// [OUT] Points amount.
	EnumerableObject<TournamentPlayerInfo>	m_listPlayerInfo;	// [OUT] Registered player info.
	EnumerableObject<UserTournament>		m_listRegTournament;// [OUT] Registered tournament info.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerRegisterInTournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerUnregisterFromTournamentPacket : public BasePacket
{
public:
	PokerUnregisterFromTournamentPacket();
	virtual ~PokerUnregisterFromTournamentPacket();

public:
	int										m_nTournamentId;	// [IN] Tournament id.
	INT_CHIPS								m_nBalance;			// [OUT] Balance amount.
	double									m_dPoints;			// [OUT] Points amount.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerUnregisterFromTournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerOpenTable_TournamentPacket : public PokerOpenTablePacket
{
public:
	PokerOpenTable_TournamentPacket();
	virtual ~PokerOpenTable_TournamentPacket();

public:
	int									m_nTournamentId;			// [IN] Tournament id.
	bool								m_bLookForMyTable;			// [IN] Looking for current users table in tournament and open it up.

	/*
	1 - Waiting for break (Tournament break is in progress).
	2 - Waiting for other tables to start hand (Hand for Hand).
	3 - Waiting for players to make rebuy / addon.
	4 - Waiting for start hand to start tournament.
	*/

	BYTE								m_btTableWaitingState;		// [OUT] Table waiting state. 
	BYTE								m_btWaitSec;				// [OUT] Table wait seconds value.
	bool								m_bRebuyAllowed;			// [OUT] Rebuy permission flag.
	BYTE								m_btRebuyCtLeft;			// [OUT] Rebuy count left.
	bool								m_bAddonAllowed;			// [OUT] Addon permission flag.
	BYTE								m_btAddonCtLeft;			// [OUT] Addon count left.
	BYTE								m_cLevelIndex;				// [OUT] Level index.
	short								m_shLevelSecLeft;			// [OUT] Seconds left till next level.
	short								m_shBreakSecLeft;			// [OUT] Seconds left till break.
	UINT								m_nSmallBlind_Next;			// [OUT] Smallblind of next level.
	UINT								m_nBigBlind_Next;			// [OUT] Bigblind of next level.
	UINT								m_nAnte_Next;				// [OUT] Ante of next level.
	
	UINT								m_nRebuyChipsCt;			// [OUT] Rebuy chips count.
	UINT								m_nAddonChipsCt;			// [OUT] Addon chips count.
	UINT								m_nRebuyAmount;				// [OUT] Rebuy amount.
	UINT								m_nRebuyVPPAmount;			// [OUT] Rebuy by VPP amount.
	UINT								m_nAddonAmount;				// [OUT] Addon amount.
	UINT								m_nAddonVPPAmount;			// [OUT] Addon by VPP amount.

	int									m_nRankIndex;				// [OUT] Your rank in tournament.
	int									m_nPlayersCt;				// [OUT] Tournament players count.
	WORD								m_shTablesCt;				// [OUT] Tournament tables count.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerOpenTable_TournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerTournamentTableInfoPacket : public PokerOpenTable_TournamentPacket
{
public:
	PokerTournamentTableInfoPacket();
	virtual ~PokerTournamentTableInfoPacket();
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerTournamentTableInfoPacket();};
};

class PokerRedirectPacket : public BasePacket
{
public:
	PokerRedirectPacket();
	virtual ~PokerRedirectPacket();

public:
	int									m_nTableId;					// [OUT] Table id.
	int									m_nTournamentId;			// [OUT] Tournament id.
	int									m_nTableId_Redirect;		// [OUT] Table Id redirect
	int									m_nTournamentId_Redirect;	// [OUT] Tournament Id redirect

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerRedirectPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerLeaveTable_TournamentPacket : public PokerLeaveTablePacket
{
public:
	PokerLeaveTable_TournamentPacket();
	virtual ~PokerLeaveTable_TournamentPacket();

public:
	int						m_nTournamentId;	// [IN] Tournament id.
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerLeaveTable_TournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerLevelChangedPacket : public BasePacket
{
public:
	PokerLevelChangedPacket();
	virtual ~PokerLevelChangedPacket();

public:
	int						m_nTournamentId;	// [OUT] Tournament id.
	int						m_nTableId;			// [OUT] Table id.
	char					m_cLevelIndex;		// [OUT] Level index.
	bool					m_bRebuyAllowed;	// [OUT] Rebuy alow flag.

	short					m_shLevelSecLeft;	// [OUT] Level seconds.
	short					m_shNextBreakSecLeft;// [OUT] Next break seconds.

	UINT					m_nSmallBlind;		// [OUT] Smallblind amount.
	UINT					m_nBigBlind;		// [OUT] Bigblind amount.
	UINT					m_nAnte;			// [OUT] Ante amount.

	UINT					m_nSmallBlind_Next;	// [OUT] Smallblind amount of next level.
	UINT					m_nBigBlind_Next;	// [OUT] Bigblind amount of next level.
	UINT					m_nAnte_Next;		// [OUT] Ante amount of next level.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerLevelChangedPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerBreakPacket : public BasePacket
{
public:
	PokerBreakPacket();
	virtual ~PokerBreakPacket();

public:
	int						m_nTournamentId;	// [OUT] Tournament id.
	int						m_nTableId;			// [OUT] Table id.
	BYTE					m_cLevelIndex;		// [OUT] Level index.
	short					m_shBreakSecLeft;	// [OUT] Break seconds
	bool					m_bAddonBreak;		// [OUT] Indicates addon break.
	UINT					m_nSmallBlind;		// [OUT] Smallblind amount.
	UINT					m_nBigBlind;		// [OUT] Bigblind amount.
	UINT					m_nAnte;			// [OUT] Ante amount.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerBreakPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerWaitForPacket : public BasePacket
{
public:
	PokerWaitForPacket();
	virtual ~PokerWaitForPacket();	

public:
	int						m_nTournamentId;		// [OUT] Tournament id.
	int						m_nTableId;				// [OUT] Table id.
	char					m_cWaitFor;				// [OUT] One of the PokerTableWaitingState constants.
	short					m_shWaitDurationSec;	// [OUT] Waiting duration in seconds if value is greater than zero.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerWaitForPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerTournamentFinishedPacket : public BasePacket
{
public:
	PokerTournamentFinishedPacket();
	virtual ~PokerTournamentFinishedPacket();

public:
	int						m_nTournamentId;		// [OUT] Tournament id.
	int						m_nTableId;				// [OUT] Table id.
	bool					m_bCanceled;			// [OUT] Tournament cancelation flag.
	int						m_nRank;				// [OUT] Participants rank in tournament.
	INT_CHIPS				m_nAmountWon;			// [OUT] Amount won or refunded (When cancel flag is set).

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerTournamentFinishedPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerRebuyPacket : public BasePacket
{
public:
	PokerRebuyPacket();
	virtual ~PokerRebuyPacket();

public:
	int						m_nTournamentId;	// [IN] Tournament id.
	int						m_nTableId;			// [IN] Table id.
	bool					m_bByVPPAmount;		// [IN] Make rebuy by VPPoints.
	BYTE					m_btRebuyMultiplier;// [IN] Rebuy multiplier.
	INT_CHIPS				m_nChipsAmountNew;	// [OUT] Chips amount.
	INT_CHIPS				m_nChipsAdded;		// [OUT] Chips amount added.
	INT_CHIPS				m_nBalanceNew;		// [OUT] Balance amount remaining.
	float					m_fVPPBalanceNew;	// [OUT] VPPoints amount remaining.
	BYTE					m_btRebuyCtLeft;	// [OUT] Rebuy count remaining.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerRebuyPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerAddonPacket : public BasePacket
{
public:
	PokerAddonPacket();
	virtual ~PokerAddonPacket();

public:
	int						m_nTournamentId;	// [IN] Tournament id.
	int						m_nTableId;			// [IN] Table id.
	bool					m_bByVPPAmount;		// [IN] Make addon by VPPoints.
	INT_CHIPS				m_nChipsAmountNew;	// [OUT] Chips amount.
	INT_CHIPS				m_nChipsAdded;		// [OUT] Chips amount added.
	INT_CHIPS				m_nBalanceNew;		// [OUT] Balance amount remaining.
	float					m_fVPPBalanceNew;	// [OUT] VPPoints amount remaining.
	BYTE					m_btAddonCtLeft;	// [OUT] Addon count remaining.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerAddonPacket();};
	INIT_RUNTIME_VARIABLE()
};

