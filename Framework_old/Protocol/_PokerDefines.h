#pragma once

// Error codes. {{
enum PokerErrorCodes
{
	ES_ERROR_SUCCESS = 0,
	ES_ERROR_UNKNOWN,
	ES_ERROR_INVALID_PARAMETER,
	ES_ERROR_USER_NOTFOUND_OR_WRONG_PASSWORD = 25000,
	ES_ERROR_USER_ALREADY_LOGGED_IN,
	ES_ERROR_SESSION_IS_ALREADY_ACTIVE,
	ES_ERROR_USER_ALREADY_LOGGED_OUT,
	ES_ERROR_USER_BLOCKED,
	ES_ERROR_INSUFFICIENT_AMOUNT,
	ES_ERROR_IP_ALREADY_EXISTS,
	ES_ERROR_AUTHENTICATION_REQUIRED,
	ES_ERROR_TOURNAMENT_REGISTRATION,
	ES_ERROR_PROTOCOL_VERSION_MISMATCH,
};
// }}

// Timer IDs. {{
#define	TIMER_SEAT_RESERVATION			1000	 
#define	TIMER_LEAVE_SEAT				1100	 
// }}

#define PROTOCOL_VER					26

enum MoneyType
{
	Real = 1,
	Play
};

enum GameType
{
	TexasHoldem = 1,
	Omaha = 2,
	Tournament = 4,
	Stud = 8,

	Playmoney = 128 // Play money.
};

enum AccountType
{
	MainBalance = 1,
	PokerBalance = 2
};

enum TournamentStatus
{
	Tournament_Announced = 1,
	Tournament_Seating,
	Tournament_Registering,
	Tournament_Starting,
	Tournament_InProgress,
	Tournament_Completed,
	Tournament_Canceled
};

enum GameSubType
{
	// Holde'm 
	Holdem_NoLimit = 1,
	Holdem_FixedLimit = 2,
	Holdem_PotLimit	= 4,

	// Omaha
	Omaha_Omaha = 8,
	Omaha_OmahaHiLo = 16,

	// Tournaments
	Tournament_Freeroll = 32,
	Tournament_Guaranteed = 64,
	Tournament_Regular = 128,
	Tournament_SitNGo = 256
};

#define IS_VALID_GAME_TYPE(gameType)\
	(((int)gameType)&(GameType::TexasHoldem|GameType::Omaha))

#define IS_VALID_GAME_SUB_TYPE(gameSubType)\
	(((int)gameSubType)&(GameSubType::Holdem_NoLimit|GameSubType::Holdem_PotLimit|GameSubType::Holdem_FixedLimit|GameSubType::Omaha_Omaha|GameSubType::Omaha_OmahaHiLo))

#define IS_VALID_TOURNAMENT_TYPE(tournamentType)\
	(((int)tournamentType)&(GameSubType::Tournament_Freeroll|GameSubType::Tournament_Guaranteed|GameSubType::Tournament_Regular|GameSubType::Tournament_SitNGo))

enum CurrencyType
{
	Currency_GEL = 1,
	Currency_USD,
	Currency_EUR,
	Currency_PT
};

enum PokerHandCardsClass
{
	CardsClass_None = 0,
	CardsClass_StraightFlush = 1,
	CardsClass_FourOfAKind,
	CardsClass_FullHouse,
	CardsClass_Flush,
	CardsClass_Straight,
	CardsClass_ThreeOfAKind,
	CardsClass_TwoPairs,
	CardsClass_Pair,
	CardsClass_HighCard
};

enum PokerActionTypes
{
	NoAction = 0,
	Call,
	Check,
	Bet,
	Fold,
	Raise,
	Muck,
	ShowCards,
	PostSB,
	PostBB,
	SitOut,
	ReturnBack,
	AllIn,
	ReserveSeat,
	AddChips
};

// Don't change order of round types. [Zack]
enum PokerRoundType
{
	No = -1,
	PreFlop,
	Flop,
	Turn,
	River,
	Showdown
};

enum PokerGameStatus
{
	NotStarted = 0,
	Started,
	Starting,
	Finished
};

enum PokerTableOption
{
	Option_AutoPostBB = 1,
	Option_AutoMuck,
	Option_SitOutNextBB,
	Option_SitOutNextHand,

	Option_Check,
	Option_CheckFold,
	Option_CallAnyCheck,
	Option_Fold,
	Option_Call,
	Option_CallAny,

	// Option choose when seat at table.
	Option_PayBB,
	Option_WaitingForBB,

	// Cash table option
	Option_AutoBuyChips,
	// Tournament table options.
	Option_AutoRebuy,
	Option_AutoAddon

};

// ==============================
// Zack: Don't change order and value, it is important !!!!
// See method PokerTableInfo::GetSitOutSeats
enum PokerSeatStatus
{
	Empty = 0,
	Reserved,
	WaitingForBB,
	PayBBAndPlay,
	Sitted,
	SittedOut,
	InPlay,
	Folded,
	All_In,
	ShownCards
};

enum PacketCodes
{
	_Unknown = 0,
	_HandShake,
	_Login,
	_Logout,
	_RegisterUser,

	// Server administration packets. {{
	_PacketAdministrationStart = 50,	// Start of administration packets.	

	_PokerBlockUnblockUser,
	_PokerQuitClient,
	_PokerGetGameTablesByType,
	_PokerGetTournamentsByType,
	_PokerGetClientConnections,
	_PokerSendAdminMessageByTable,
	_PokerSendAdminMessageByUser,
	_PokerCreateGameTable,
	_PokerForceClientSessionToQuit,
	_PokerDisableServerConnect,
	_PokerDeactivateGameTable,
	
	_PokerCreateTournament,				// Create tournament.
	_PokerCancelTournament,				// Cancel tournament.
	_PokerHideShowTournament,			// Hide/Show tournament.
	_PokerRegisterUserInTournament,		// Register/Unregister user for tournament.

	// BBJackpot.
	_PokerEnableBBJackpot,				// Enable and initialize BBJackpot.
	_PokerDisableBBJackpot,				// Disable BBJackpot.

	_PacketAdministrationEnd = 100,		// End of administration packets
	// }}
	
	_PokerTableInfo,					// Retrieves table info
	_PokerOpenTable,
	_PokerLeaveTable,
	_PokerTableClosed,
	_PokerReserveSeat,
	_PokerCancelReservation,
	_PokerSeatAt,
	_PokerJoinWaitingList,
	_PokerAddChips,
	_PokerPostBlind,
	_PokerShowMuckCards,
	_PokerSetOption,					// Set game option. (Auto post blind, Auto Muck cards and etc)
	_PokerSitOut,
	_PokerSeatStatusChanged,
	_PokerReturnBack,
	_PokerSeatOnlineChanged,			// When user connection status is changed.
	_PokerSetActive,
	_PokerAction,
	_PokerAct,							// Sent only from server to specified client.
	_PokerHandStarted,
	_PokerHandFinished,
	_PokerRoundStarted,
	_PokerChatMessage,
	_PokerSetAvatar,
	_PokerUploadAvatar,
	_PokerDownloadAvatar,
	_PokerOpenRandomTable,				// Open random table
	_PokerGetHandHistoryPacket,			// Retrieve hand history info.
	_PokerGetLobbyInfo,					// Retrieve game tables from server.
	_PokerMoneyTransfer,				// Transfer money between two systems.

	// Tournament packets.
	_PokerTournamentCustomInfo,
	_PokerRegisterInTournament,
	_PokerUnregisterFromTournament,
	_PokerOpenTable_Tournament,
	_PokerLeaveTable_Tournament,
	_PokerRedirect,						// Sent only from server to specified client.
	_PokerLevelChanged,					// Sent only from server to specified client.
	_PokerBreak,						// Sent only from server to specified client.
	_PokerWaitFor,						// Sent only from server to specified client.
	_PokerRebuy,
	_PokerAddon,
	_PokerTournamentFinished,			// Sent only from server to specified client.

	_PokerResetPlaymoney,				// Reset playmoney amount packet.
	_PokerRefreshBalance,				// Refresh balance packet.

	_PokerFindPlayer,					// Find player by username.
	_PokerFindTournament,				// Find tournament by id.
	_PokerTournamentTableInfo,			// Tournament table info packet
};
