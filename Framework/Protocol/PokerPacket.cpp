#include "stdafx.h"
#include "PokerPacket.h"
#include "../PokerServer/Object/PokerHandHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////
//
//	class PokerGetLobbyInfoPacket
//
/////////////////////////////////////////////////////

PokerGetLobbyInfoPacket::PokerGetLobbyInfoPacket(){
	m_nCode = PacketCodes::_PokerGetLobbyInfo;
	}

BEGIN_DECLARE_VAR(PokerGetLobbyInfoPacket, BasePacket)
	DECLARE_VAR(_T("PokerGetLobbyInfoPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("MoneyType"),				_T(""), VariableType::VT_Int8, offsetof(PokerGetLobbyInfoPacket,m_cMoneyType), false, false, false)
	DECLARE_VAR(_T("GameType"),					_T(""), VariableType::VT_Int8, offsetof(PokerGetLobbyInfoPacket,m_cGameType), false, false, false)
	DECLARE_VAR(_T("SubType"),					_T(""), VariableType::VT_Int16, offsetof(PokerGetLobbyInfoPacket,m_shSubType), false, false, false)

	DECLARE_VAR(_T("RequestFlag"),				_T(""), VariableType::VT_Int8, offsetof(PokerGetLobbyInfoPacket,m_cRequestFlag), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerGetLobbyInfoPacket,m_nTableId), false, false, false)

	DECLARE_VAR(_T("PlayersCt"),				_T(""),	VariableType::VT_Int32, offsetof(PokerGetLobbyInfoPacket,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TablesCt"),					_T(""),	VariableType::VT_Int16, offsetof(PokerGetLobbyInfoPacket,m_shTablesCt), false, false, false)
	DECLARE_VAR(_T("TournamentsCt"),			_T(""),	VariableType::VT_Int16, offsetof(PokerGetLobbyInfoPacket,m_shTournamentsCt), false, false, false)

	DECLARE_VAR(_T("TablesCtByRequest"),		_T(""),	VariableType::VT_Int16, offsetof(PokerGetLobbyInfoPacket,m_shTablesCtByRequest), false, false, false)
	DECLARE_VAR(_T("TournamentsCtByRequest"),	_T(""),	VariableType::VT_Int16, offsetof(PokerGetLobbyInfoPacket,m_shTournamentsCtByRequest), false, false, false)

	DECLARE_VAR(_T("InPlayAmount"),				_T(""),	VariableType::VT_Chips, offsetof(PokerGetLobbyInfoPacket,m_nInPlayAmount), false, false, false)
	DECLARE_VAR(_T("Balance"),					_T(""),	VariableType::VT_Chips, offsetof(PokerGetLobbyInfoPacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("PlayBalance"),				_T(""),	VariableType::VT_Chips, offsetof(PokerGetLobbyInfoPacket,m_nPlayBalance), false, false, false)
	DECLARE_VAR(_T("BBJackpotAmount"),			_T(""),	VariableType::VT_Int32, offsetof(PokerGetLobbyInfoPacket,m_nBadBeatJackpotAmount), false, false, false)

	DECLARE_VAR(_T("CashTablesStaticInfo"),		_T(""),	VariableType::VT_None, offsetof(PokerGetLobbyInfoPacket,m_listCashTablesStaticInfo), false, true, true)
	DECLARE_VAR(_T("CashTablesDynamicInfo"),	_T(""),	VariableType::VT_None, offsetof(PokerGetLobbyInfoPacket,m_listCashTablesDynamicInfo), false, true, true)
	DECLARE_VAR(_T("CashTablesPlayersInfo"),	_T(""),	VariableType::VT_None, offsetof(PokerGetLobbyInfoPacket,m_listCashTablePlayersInfo), false, true, true)
	DECLARE_VAR(_T("TournamentInfo"),			_T(""),	VariableType::VT_None, offsetof(PokerGetLobbyInfoPacket,m_listTournamentInfo), false, true, true)
	DECLARE_VAR(_T("TournamentDynamicInfo"),	_T(""),	VariableType::VT_None, offsetof(PokerGetLobbyInfoPacket,m_listTournamentDynamicInfo), false, true, true)
END_DECLARE_VAR()

PokerGetLobbyInfoPacket::~PokerGetLobbyInfoPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerOpenTablePacket
//
/////////////////////////////////////////////////////
PokerOpenTablePacket::PokerOpenTablePacket(){
	m_nCode = PacketCodes::_PokerOpenTable;
	}

BEGIN_DECLARE_VAR(PokerOpenTablePacket, BasePacket)
	DECLARE_VAR(_T("PokerOpenTablePacket"), _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""), VariableType::VT_Int32, offsetof(PokerOpenTablePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("HandId"),				_T(""), VariableType::VT_Int64, offsetof(PokerOpenTablePacket,m_nHandId), false, false, false)
	DECLARE_VAR(_T("SeatOrJoinWaitList"),	_T(""), VariableType::VT_Bool, offsetof(PokerOpenTablePacket,m_bSeatOrJoinWaitList), false, false, false)
	DECLARE_VAR(_T("Info"),					_T(""), VariableType::VT_None, offsetof(PokerOpenTablePacket,m_info), false, true, true)	
	DECLARE_VAR(_T("ActiveSeatTimeFull"),	_T(""), VariableType::VT_Int8, offsetof(PokerOpenTablePacket,m_cActiveSeatTimeFull), false, false, false)
	DECLARE_VAR(_T("ActiveSeatTimePassed"),	_T(""), VariableType::VT_Int8, offsetof(PokerOpenTablePacket,m_cActiveSeatTimePassed), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int8, offsetof(PokerOpenTablePacket,m_cSeat), false, false, false)
	// Table statistic.
	DECLARE_VAR(_T("HandsDealed"),			_T(""), VariableType::VT_Int32, offsetof(PokerOpenTablePacket,m_nHandsDealed), false, false, false)
	DECLARE_VAR(_T("HandsPlayed"),			_T(""), VariableType::VT_Int32, offsetof(PokerOpenTablePacket,m_nHandsPlayed), false, false, false)
	DECLARE_VAR(_T("HandsWon"),				_T(""), VariableType::VT_Int32, offsetof(PokerOpenTablePacket,m_nHandsWon), false, false, false)
	DECLARE_VAR(_T("HandsWonPreflop"),		_T(""), VariableType::VT_Int32, offsetof(PokerOpenTablePacket,m_nHandsWonPreflop), false, false, false)
	DECLARE_VAR(_T("HandsWonPostflop"),		_T(""), VariableType::VT_Int32, offsetof(PokerOpenTablePacket,m_nHandsWonPostflop), false, false, false)
	DECLARE_VAR(_T("HandsWonShowdown"),		_T(""), VariableType::VT_Int32, offsetof(PokerOpenTablePacket,m_nHandsWonShowdown), false, false, false)
END_DECLARE_VAR()

PokerOpenTablePacket::~PokerOpenTablePacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerLeaveTablePacket
//
/////////////////////////////////////////////////////
PokerLeaveTablePacket::PokerLeaveTablePacket(){
	m_nCode = PacketCodes::_PokerLeaveTable;
	}


BEGIN_DECLARE_VAR(PokerLeaveTablePacket, BasePacket)
	DECLARE_VAR(_T("PokerLeaveTablePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerLeaveTablePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Seat"),						_T(""), VariableType::VT_Int8, offsetof(PokerLeaveTablePacket,m_cSeat), false, false, false)
END_DECLARE_VAR()

PokerLeaveTablePacket::~PokerLeaveTablePacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerTableClosedPacket
//
/////////////////////////////////////////////////////
PokerTableClosedPacket::PokerTableClosedPacket(){
	m_nCode = PacketCodes::_PokerTableClosed;
	}

BEGIN_DECLARE_VAR(PokerTableClosedPacket, BasePacket)
	DECLARE_VAR(_T("PokerTableClosedPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""), VariableType::VT_Int32, offsetof(PokerTableClosedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerTableClosedPacket,m_nTableId), false, false, false)
END_DECLARE_VAR()

PokerTableClosedPacket::~PokerTableClosedPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerReserveSeatPacket
//
/////////////////////////////////////////////////////
PokerReserveSeatPacket::PokerReserveSeatPacket(){
	m_nCode = PacketCodes::_PokerReserveSeat;
	}

BEGIN_DECLARE_VAR(PokerReserveSeatPacket, BasePacket)
	DECLARE_VAR(_T("PokerReserveSeatPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerReserveSeatPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Seat"),						_T(""), VariableType::VT_Int8, offsetof(PokerReserveSeatPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("UserName"),					_T(""), VariableType::VT_String, offsetof(PokerReserveSeatPacket,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Balance"),					_T(""), VariableType::VT_Chips, offsetof(PokerReserveSeatPacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("AmountMin"),				_T(""), VariableType::VT_Chips, offsetof(PokerReserveSeatPacket,m_nAmountMin), false, false, false)
	DECLARE_VAR(_T("AmountMax"),				_T(""), VariableType::VT_Chips, offsetof(PokerReserveSeatPacket,m_nAmountMax), false, false, false)
	DECLARE_VAR(_T("ReservationTimeSec"),		_T(""), VariableType::VT_Int16, offsetof(PokerReserveSeatPacket,m_shReservationTimeSec), false, false, false)
	DECLARE_VAR(_T("ReservationExpired"),		_T(""), VariableType::VT_Bool, offsetof(PokerReserveSeatPacket,m_bReservationExpired), false, false, false)
END_DECLARE_VAR()

PokerReserveSeatPacket::~PokerReserveSeatPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerCancelReservePacket
//
/////////////////////////////////////////////////////
PokerCancelReservationPacket::PokerCancelReservationPacket(){
	m_nCode = PacketCodes::_PokerCancelReservation;
	}

BEGIN_DECLARE_VAR(PokerCancelReservationPacket, BasePacket)
	DECLARE_VAR(_T("PokerCancelReservationPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerCancelReservationPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Seat"),							_T(""), VariableType::VT_Int8, offsetof(PokerCancelReservationPacket,m_cSeat), false, false, false)
END_DECLARE_VAR()

PokerCancelReservationPacket::~PokerCancelReservationPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSeatAtTablePacket
//
/////////////////////////////////////////////////////
PokerSeatAtTablePacket::PokerSeatAtTablePacket(){
	m_nCode = PacketCodes::_PokerSeatAt;
	}

BEGIN_DECLARE_VAR(PokerSeatAtTablePacket, BasePacket)
	DECLARE_VAR(_T("PokerSeatAtTablePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerSeatAtTablePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""), VariableType::VT_Int32, offsetof(PokerSeatAtTablePacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("SeatIndex"),				_T(""), VariableType::VT_Int8, offsetof(PokerSeatAtTablePacket,m_cSeatIndex), false, false, false)
	DECLARE_VAR(_T("ChipAmount"),				_T(""), VariableType::VT_Chips, offsetof(PokerSeatAtTablePacket,m_nChipAmount), false, false, false)
	DECLARE_VAR(_T("SeatSessionId"),			_T(""), VariableType::VT_Int64, offsetof(PokerSeatAtTablePacket,m_nSeatSessionId), false, false, false)
	DECLARE_VAR(_T("SeatInfo"),					_T(""), VariableType::VT_None, offsetof(PokerSeatAtTablePacket,m_seatInfo), false, true, false)
	DECLARE_VAR(_T("AvatarFileName"),			_T(""), VariableType::VT_String, offsetof(PokerSeatAtTablePacket,m_sAvatarFileName), false, false, false)
	//DECLARE_VAR(_T("AvatarData"),				_T(""), VariableType::VT_None, offsetof(PokerSeatAtTablePacket,m_avatarData), false, true, false)
END_DECLARE_VAR()

PokerSeatAtTablePacket::~PokerSeatAtTablePacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerAddChipsPacket
//
/////////////////////////////////////////////////////
PokerAddChipsPacket::PokerAddChipsPacket(){
	m_nCode = PacketCodes::_PokerAddChips;
	}


BEGIN_DECLARE_VAR(PokerAddChipsPacket, BasePacket)
	DECLARE_VAR(_T("PokerAddChipsPacket"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerAddChipsPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Amount"),					_T(""), VariableType::VT_Chips, offsetof(PokerAddChipsPacket,m_nAmount), false, false, false)
	DECLARE_VAR(_T("PrepareToAdd"),				_T(""), VariableType::VT_Bool, offsetof(PokerAddChipsPacket,m_bPrepareToAdd), false, false, false)
	DECLARE_VAR(_T("Seat"),						_T(""), VariableType::VT_Int8, offsetof(PokerAddChipsPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("Balance"),					_T(""), VariableType::VT_Chips, offsetof(PokerAddChipsPacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("Min"),						_T(""), VariableType::VT_Chips, offsetof(PokerAddChipsPacket,m_nMin), false, false, false)
	DECLARE_VAR(_T("Max"),						_T(""), VariableType::VT_Chips, offsetof(PokerAddChipsPacket,m_nMax), false, false, false)
	DECLARE_VAR(_T("SeatSessionId"),			_T(""), VariableType::VT_Int64, offsetof(PokerAddChipsPacket,m_nSeatSessionId), false, false, false)
END_DECLARE_VAR()

PokerAddChipsPacket::~PokerAddChipsPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerJoinWaitingListPacket
//
/////////////////////////////////////////////////////
PokerJoinWaitingListPacket::PokerJoinWaitingListPacket(){
	m_nCode = PacketCodes::_PokerJoinWaitingList;
	}

BEGIN_DECLARE_VAR(PokerJoinWaitingListPacket, BasePacket)
	DECLARE_VAR(_T("PokerJoinWaitingListPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerJoinWaitingListPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("UserId"),						_T(""), VariableType::VT_Int32, offsetof(PokerJoinWaitingListPacket,m_nUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),						_T(""), VariableType::VT_String, offsetof(PokerJoinWaitingListPacket,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Index"),						_T(""), VariableType::VT_Int16, offsetof(PokerJoinWaitingListPacket,m_shIndex), false, false, false)
END_DECLARE_VAR()

PokerJoinWaitingListPacket::~PokerJoinWaitingListPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerHandStartedPacket
//
/////////////////////////////////////////////////////
PokerHandStartedPacket::PokerHandStartedPacket(){
	m_nCode = PacketCodes::_PokerHandStarted;
	}

BEGIN_DECLARE_VAR(PokerHandStartedPacket, BasePacket)
	DECLARE_VAR(_T("PokerHandStartedPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerHandStartedPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""), VariableType::VT_Int32, offsetof(PokerHandStartedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("HandId"),					_T(""), VariableType::VT_Int64, offsetof(PokerHandStartedPacket,m_nHandId), false, false, false)
	DECLARE_VAR(_T("BigBlind"),					_T(""), VariableType::VT_Int8, offsetof(PokerHandStartedPacket,m_cBigBlind), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),				_T(""), VariableType::VT_Int8, offsetof(PokerHandStartedPacket,m_cSmallBlind), false, false, false)
	DECLARE_VAR(_T("Dealer"),					_T(""), VariableType::VT_Int8, offsetof(PokerHandStartedPacket,m_cDealer), false, false, false)
	DECLARE_VAR(_T("SeatReceiver"),				_T(""), VariableType::VT_Int8, offsetof(PokerHandStartedPacket,m_cSeatReceiver), false, false, false)
	DECLARE_VAR(_T("handCards"),				_T(""), VariableType::VT_None, offsetof(PokerHandStartedPacket,m_handCards), false, true, true)
	DECLARE_VAR(_T("SeatAmounts"),				_T(""), VariableType::VT_None, offsetof(PokerHandStartedPacket,m_seatAmounts), false, true, true)
	DECLARE_VAR(_T("SeatStatus"),				_T(""), VariableType::VT_None, offsetof(PokerHandStartedPacket,m_listSeatStatus), false, true, true)
	DECLARE_VAR(_T("TotalPotAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerHandStartedPacket,m_nTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("MainPotAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerHandStartedPacket,m_nMainPotAmount), false, false, false)
	DECLARE_VAR(_T("ActionTime"),				_T(""), VariableType::VT_DateTime, offsetof(PokerHandStartedPacket,m_dtAction), false, false, false)
	// Tournament table params only.
	DECLARE_VAR(_T("RankIndex"),				_T(""), VariableType::VT_Int32, offsetof(PokerHandStartedPacket,m_nRankIndex), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),				_T(""), VariableType::VT_Int32, offsetof(PokerHandStartedPacket,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TablesCt"),					_T(""), VariableType::VT_Int16, offsetof(PokerHandStartedPacket,m_shTablesCt), false, false, false)
END_DECLARE_VAR()

PokerHandStartedPacket::~PokerHandStartedPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSeatAmountInfo
//
/////////////////////////////////////////////////////
PokerSeatAmountInfo::PokerSeatAmountInfo(){
	}

BEGIN_DECLARE_VAR(PokerSeatAmountInfo, Serializable)
	DECLARE_VAR(_T("PokerSeatAmountInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int8, offsetof(PokerSeatAmountInfo,m_cSeat), false, false, false)
	DECLARE_VAR(_T("PotAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerSeatAmountInfo,m_nPotAmount), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),		_T(""), VariableType::VT_Chips, offsetof(PokerSeatAmountInfo,m_nTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T(""), VariableType::VT_Chips, offsetof(PokerSeatAmountInfo,m_nBalance), false, false, false)
END_DECLARE_VAR()

PokerSeatAmountInfo::~PokerSeatAmountInfo(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSeatStateInfo
//
/////////////////////////////////////////////////////
PokerSeatStateInfo::PokerSeatStateInfo(){
	}

BEGIN_DECLARE_VAR(PokerSeatStateInfo, Serializable)
	DECLARE_VAR(_T("PokerSeatStateInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int8, offsetof(PokerSeatStateInfo,m_cSeat), false, false, false)
	DECLARE_VAR(_T("Status"),				_T(""), VariableType::VT_Int8, offsetof(PokerSeatStateInfo,m_cStatus), false, false, false)
END_DECLARE_VAR()

PokerSeatStateInfo::~PokerSeatStateInfo(){
	}

//////////////////////////////////////////////////////
//
//	class PokerHandFinishedPacket
//
/////////////////////////////////////////////////////
PokerHandFinishedPacket::PokerHandFinishedPacket(){
	m_nCode = PacketCodes::_PokerHandFinished;
	}

BEGIN_DECLARE_VAR(PokerHandFinishedPacket, BasePacket)
	DECLARE_VAR(_T("PokerHandFinishedPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerHandFinishedPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""), VariableType::VT_Int32, offsetof(PokerHandFinishedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("HandId"),					_T(""), VariableType::VT_Int64, offsetof(PokerHandFinishedPacket,m_nHandId), false, false, false)
	DECLARE_VAR(_T("HandStartSec"),				_T(""), VariableType::VT_Int8, offsetof(PokerHandFinishedPacket,m_cHandStartSec), false, false, false)
	DECLARE_VAR(_T("Showdown"),					_T(""), VariableType::VT_Int8, offsetof(PokerHandFinishedPacket,m_bShowdown), false, false, false)
	DECLARE_VAR(_T("TableCardsBestComb"),		_T(""), VariableType::VT_Bool, offsetof(PokerHandFinishedPacket,m_bTableCardsBestComb), false, false, false)
	DECLARE_VAR(_T("Winners"),					_T(""), VariableType::VT_None, offsetof(PokerHandFinishedPacket,m_winners), false, true, true)
	DECLARE_VAR(_T("HandCards"),				_T(""), VariableType::VT_None, offsetof(PokerHandFinishedPacket,m_handCards), false, true, true)
	DECLARE_VAR(_T("ActionTime"),				_T(""), VariableType::VT_DateTime, offsetof(PokerHandFinishedPacket,m_dtAction), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),				_T(""), VariableType::VT_Int32, offsetof(PokerHandFinishedPacket,m_wRakeAmount), false, false, false)
	DECLARE_VAR(_T("PotAmountsDetail"),			_T(""), VariableType::VT_None, offsetof(PokerHandFinishedPacket,m_listPotAmounts), false, true, true)
	DECLARE_VAR(_T("HandHistory"),				_T(""), VariableType::VT_None, offsetof(PokerHandFinishedPacket,m_handHistory), false, true, false)
END_DECLARE_VAR()

PokerHandFinishedPacket::~PokerHandFinishedPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerRoundStartedPacket
//
/////////////////////////////////////////////////////

PokerRoundStartedPacket::PokerRoundStartedPacket(){
	m_nCode				= PacketCodes::_PokerRoundStarted;
	m_nMainPotAmount	= 0;
	}

BEGIN_DECLARE_VAR(PokerRoundStartedPacket, BasePacket)
	DECLARE_VAR(_T("PokerRoundStartedPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerRoundStartedPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""), VariableType::VT_Int32, offsetof(PokerRoundStartedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("HandId"),					_T(""), VariableType::VT_Int64, offsetof(PokerRoundStartedPacket,m_nHandId), false, false, false)
	DECLARE_VAR(_T("ReceiverSession"),			_T(""), VariableType::VT_Int64, offsetof(PokerRoundStartedPacket,m_nReceiverSession), false, false, false)
	DECLARE_VAR(_T("ReceiverSeat"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cReceiverSeat), false, false, false)
	DECLARE_VAR(_T("RoundType"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cRoundType), false, false, false)
	DECLARE_VAR(_T("MainPotAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerRoundStartedPacket,m_nMainPotAmount), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerRoundStartedPacket,m_nTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("RoundStartSec"),			_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cRoundStartSec), false, false, false)
	DECLARE_VAR(_T("PreActions"),				_T(""), VariableType::VT_None, offsetof(PokerRoundStartedPacket,m_listPreActions), false, true, true)
	DECLARE_VAR(_T("TableCard1"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_btTableCard1), false, false, false)
	DECLARE_VAR(_T("TableCard2"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_btTableCard2), false, false, false)
	DECLARE_VAR(_T("TableCard3"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_btTableCard3), false, false, false)
	DECLARE_VAR(_T("TableCard4"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_btTableCard4), false, false, false)
	DECLARE_VAR(_T("TableCard5"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_btTableCard5), false, false, false)
	DECLARE_VAR(_T("BestCardsClass"),			_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cBestCardsClass), false, false, false)
	DECLARE_VAR(_T("BestCard1"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cBestCard1), false, false, false)
	DECLARE_VAR(_T("BestCard2"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cBestCard2), false, false, false)
	DECLARE_VAR(_T("BestCard3"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cBestCard3), false, false, false)
	DECLARE_VAR(_T("BestCard4"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cBestCard4), false, false, false)
	DECLARE_VAR(_T("BestCard5"),				_T(""), VariableType::VT_Int8, offsetof(PokerRoundStartedPacket,m_cBestCard5), false, false, false)
	DECLARE_VAR(_T("HandCards"),				_T(""), VariableType::VT_None, offsetof(PokerRoundStartedPacket,m_listHandCards), false, true, true)
	DECLARE_VAR(_T("ActionTime"),				_T(""), VariableType::VT_DateTime, offsetof(PokerRoundStartedPacket,m_dtAction), false, false, false)
	DECLARE_VAR(_T("PotAmounts"),				_T(""), VariableType::VT_None, offsetof(PokerRoundStartedPacket,m_listPotAmounts), false, true, true)
	DECLARE_VAR(_T("RakeAmount"),				_T(""), VariableType::VT_Int32, offsetof(PokerRoundStartedPacket,m_wRakeAmount), false, false, false)
END_DECLARE_VAR()

PokerRoundStartedPacket::~PokerRoundStartedPacket(){
	}


//////////////////////////////////////////////////////
//
//	class PokerAction
//
/////////////////////////////////////////////////////
PokerAction::PokerAction(){
	}

BEGIN_DECLARE_VAR(PokerAction, Serializable)
	DECLARE_VAR(_T("PokerAction"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ActionType"),	_T(""), VariableType::VT_Int8, offsetof(PokerAction,m_cActionType), false, false, false)
	DECLARE_VAR(_T("AmountMin"),	_T(""), VariableType::VT_Chips, offsetof(PokerAction,m_nAmountMin), false, false, false)
	DECLARE_VAR(_T("AmountMax"),	_T(""), VariableType::VT_Chips, offsetof(PokerAction,m_nAmountMax), false, false, false)
END_DECLARE_VAR()

PokerAction::~PokerAction(){
	}

//////////////////////////////////////////////////////
//
//	class PokerActPacket
//
/////////////////////////////////////////////////////
PokerActPacket::PokerActPacket(){
	m_nCode = PacketCodes::_PokerAct;
	}

BEGIN_DECLARE_VAR(PokerActPacket, BasePacket)
	DECLARE_VAR(_T("PokerActPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),			_T(""), VariableType::VT_Int32, offsetof(PokerActPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),		_T(""), VariableType::VT_Int32, offsetof(PokerActPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),				_T(""), VariableType::VT_Int8, offsetof(PokerActPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("ActionTime"),		_T(""), VariableType::VT_Int8, offsetof(PokerActPacket,m_cActionTime), false, false, false)
	DECLARE_VAR(_T("HandId"),			_T(""), VariableType::VT_Int64, offsetof(PokerActPacket,m_nHandId), false, false, false)
	DECLARE_VAR(_T("Actions"),			_T(""), VariableType::VT_None, offsetof(PokerActPacket,m_actions), false, true, true)
END_DECLARE_VAR()

PokerActPacket::~PokerActPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerPreAction
//
/////////////////////////////////////////////////////
PokerPreAction::PokerPreAction(){
	}

BEGIN_DECLARE_VAR(PokerPreAction, Serializable)
	DECLARE_VAR(_T("PokerPreAction"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ActionType"),		_T(""), VariableType::VT_Int8, offsetof(PokerPreAction,m_cActionType), false, false, false)
	DECLARE_VAR(_T("Value"),			_T(""), VariableType::VT_Bool, offsetof(PokerPreAction,m_bValue), false, false, false)
	DECLARE_VAR(_T("Amount"),			_T(""), VariableType::VT_Chips, offsetof(PokerPreAction,m_nValue), false, false, false)
END_DECLARE_VAR()

PokerPreAction::~PokerPreAction(){
	}

//////////////////////////////////////////////////////
//
//	class PokerActionPacket
//
/////////////////////////////////////////////////////
PokerActionPacket::PokerActionPacket(){
	m_nCode = PacketCodes::_PokerAction;
	}

BEGIN_DECLARE_VAR(PokerActionPacket, BasePacket)
	DECLARE_VAR(_T("PokerActionPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""), VariableType::VT_Int32, offsetof(PokerActionPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T(""), VariableType::VT_Int32, offsetof(PokerActionPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int8, offsetof(PokerActionPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("SeatReceiver"),			_T(""), VariableType::VT_Int8, offsetof(PokerActionPacket,m_cSeatReceiver), false, false, false)
	DECLARE_VAR(_T("SeatStatus"),			_T(""), VariableType::VT_Int8, offsetof(PokerActionPacket,m_cSeatStatus), false, false, false)
	DECLARE_VAR(_T("ActionType"),			_T(""), VariableType::VT_Int8, offsetof(PokerActionPacket,m_cActionType), false, false, false)
	DECLARE_VAR(_T("ActionTime"),			_T(""), VariableType::VT_DateTime, offsetof(PokerActionPacket,m_dtAction), false, false, false)
	DECLARE_VAR(_T("Amount"),				_T(""), VariableType::VT_Chips, offsetof(PokerActionPacket,m_nAmount), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T(""), VariableType::VT_Chips, offsetof(PokerActionPacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("PotAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerActionPacket,m_nPotAmount), false, false, false)
	DECLARE_VAR(_T("SeatTotalPot"),			_T(""), VariableType::VT_Chips, offsetof(PokerActionPacket,m_nSeatTotalPot), false, false, false)
	DECLARE_VAR(_T("TotalPot"),				_T(""), VariableType::VT_Chips, offsetof(PokerActionPacket,m_nTotalPot), false, false, false)
	DECLARE_VAR(_T("SetPreActions"),		_T(""), VariableType::VT_Bool, offsetof(PokerActionPacket,m_bSetPreActions), false, false, false)
	DECLARE_VAR(_T("RoundFinished"),		_T(""), VariableType::VT_Bool, offsetof(PokerActionPacket,m_bRoundFinished), false, false, false)
	DECLARE_VAR(_T("PreActions"),			_T(""), VariableType::VT_None, offsetof(PokerActionPacket,m_listPreActions), false, true, true)
	DECLARE_VAR(_T("CardsShown"),			_T(""), VariableType::VT_None, offsetof(PokerActionPacket,m_listCardsShown), false, true, true)
	DECLARE_VAR(_T("SeatChangedAmounts"),	_T(""), VariableType::VT_None, offsetof(PokerActionPacket,m_listSeatChangedAmounts), false, true, true)
END_DECLARE_VAR()

PokerActionPacket::~PokerActionPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerTableInfoPacket
//
/////////////////////////////////////////////////////
PokerTableInfoPacket::PokerTableInfoPacket(){
	m_nCode = PacketCodes::_PokerTableInfo;
	}

PokerTableInfoPacket::~PokerTableInfoPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerPostBlindPacket
//
/////////////////////////////////////////////////////
PokerPostBlindPacket::PokerPostBlindPacket(){
	m_nCode = PacketCodes::_PokerPostBlind;
	}

BEGIN_DECLARE_VAR(PokerPostBlindPacket, BasePacket)
	DECLARE_VAR(_T("PokerPostBlindPacket"), _T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""),		VariableType::VT_Int32, offsetof(PokerPostBlindPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""),		VariableType::VT_Int8, offsetof(PokerPostBlindPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("PostBlind"),			_T(""),		VariableType::VT_Int8, offsetof(PokerPostBlindPacket,m_cPostBlind), false, false, false)
	DECLARE_VAR(_T("ActionSec"),			_T(""),		VariableType::VT_Int16, offsetof(PokerPostBlindPacket,m_shActionSec), false, false, false)
//	DECLARE_VAR(_T("Amount"),				_T(""),		VariableType::VT_Int32, offsetof(PokerPostBlindPacket,m_nAmount), false, false, false)
END_DECLARE_VAR()

PokerPostBlindPacket::~PokerPostBlindPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerShowMuckCardsPacket
//
/////////////////////////////////////////////////////
PokerShowMuckCardsPacket::PokerShowMuckCardsPacket(){
	m_nCode = PacketCodes::_PokerShowMuckCards;
	}

BEGIN_DECLARE_VAR(PokerShowMuckCardsPacket, BasePacket)
	DECLARE_VAR(_T("PokerShowMuckCardsPacket"), _T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""),		VariableType::VT_Int32, offsetof(PokerShowMuckCardsPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""),		VariableType::VT_Int32, offsetof(PokerShowMuckCardsPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),						_T(""),		VariableType::VT_Int8, offsetof(PokerShowMuckCardsPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("ShowCards"),				_T(""),		VariableType::VT_Bool, offsetof(PokerShowMuckCardsPacket,m_bShowCards), false, false, false)
	DECLARE_VAR(_T("ActionSec"),				_T(""),		VariableType::VT_Int16, offsetof(PokerShowMuckCardsPacket,m_shActionSec), false, false, false)
END_DECLARE_VAR()

PokerShowMuckCardsPacket::~PokerShowMuckCardsPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSetOptionPacket
//
/////////////////////////////////////////////////////
PokerSetOptionPacket::PokerSetOptionPacket(){
	m_nCode = PacketCodes::_PokerSetOption;
	}

BEGIN_DECLARE_VAR(PokerSetOptionPacket, BasePacket)
	DECLARE_VAR(_T("PokerSetOptionPacket"), _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""),	VariableType::VT_Int32, offsetof(PokerSetOptionPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T(""),	VariableType::VT_Int32, offsetof(PokerSetOptionPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""),	VariableType::VT_Int8, offsetof(PokerSetOptionPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("Option"),				_T(""),	VariableType::VT_Int8, offsetof(PokerSetOptionPacket,m_cOption), false, false, false)
	DECLARE_VAR(_T("Value"),				_T(""),	VariableType::VT_Bool, offsetof(PokerSetOptionPacket,m_bValue), false, false, false)
	DECLARE_VAR(_T("AmountValue"),			_T(""),	VariableType::VT_Chips, offsetof(PokerSetOptionPacket,m_nValue), false, false, false)
END_DECLARE_VAR()

PokerSetOptionPacket::~PokerSetOptionPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSitOutPacket
//
/////////////////////////////////////////////////////
PokerSitOutPacket::PokerSitOutPacket(){
	m_nCode = PacketCodes::_PokerSitOut;
	}

BEGIN_DECLARE_VAR(PokerSitOutPacket, BasePacket)
	DECLARE_VAR(_T("PokerSitOutPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""), VariableType::VT_Int32, offsetof(PokerSitOutPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T(""), VariableType::VT_Int32, offsetof(PokerSitOutPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int32, offsetof(PokerSitOutPacket,m_nSeat), false, false, false)
	DECLARE_VAR(_T("SitOut"),				_T(""), VariableType::VT_Bool, offsetof(PokerSitOutPacket,m_bSitOut), false, false, false)
END_DECLARE_VAR()

PokerSitOutPacket::~PokerSitOutPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerReturnBackPacket
//
/////////////////////////////////////////////////////
PokerReturnBackPacket::PokerReturnBackPacket(){
	m_nCode = PacketCodes::_PokerReturnBack;
	}

BEGIN_DECLARE_VAR(PokerReturnBackPacket, BasePacket)
	DECLARE_VAR(_T("PokerReturnBackPacket"),_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""), VariableType::VT_Int32, offsetof(PokerReturnBackPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T(""), VariableType::VT_Int32, offsetof(PokerReturnBackPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int8, offsetof(PokerReturnBackPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("ReturnBack"),			_T(""), VariableType::VT_Bool, offsetof(PokerReturnBackPacket,m_bReturnBack), false, false, false)
END_DECLARE_VAR()

PokerReturnBackPacket::~PokerReturnBackPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSetActivePacket
//
/////////////////////////////////////////////////////
PokerSetActivePacket::PokerSetActivePacket(){
	m_nCode = PacketCodes::_PokerSetActive;
	}

BEGIN_DECLARE_VAR(PokerSetActivePacket, BasePacket)
	DECLARE_VAR(_T("PokerSetActivePacket"), _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""), VariableType::VT_Int32, offsetof(PokerSetActivePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T(""), VariableType::VT_Int32, offsetof(PokerSetActivePacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int8, offsetof(PokerSetActivePacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("RegularTimeSec"),		_T(""), VariableType::VT_Int8, offsetof(PokerSetActivePacket,m_cRegularTimeSec), false, false, false)
	DECLARE_VAR(_T("ExtraTimeSec"),			_T(""), VariableType::VT_Int8, offsetof(PokerSetActivePacket,m_cExtraTimeSec), false, false, false)
	DECLARE_VAR(_T("TimeBankSec"),			_T(""), VariableType::VT_Int8, offsetof(PokerSetActivePacket,m_cTimeBankSec), false, false, false)
END_DECLARE_VAR()

PokerSetActivePacket::~PokerSetActivePacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSeatStatusChangedPacket
//
/////////////////////////////////////////////////////
PokerSeatStatusChangedPacket::PokerSeatStatusChangedPacket(){
	m_nCode = PacketCodes::_PokerSeatStatusChanged;
	}

BEGIN_DECLARE_VAR(PokerSeatStatusChangedPacket, BasePacket)
	DECLARE_VAR(_T("PokerSeatStatusChangedPacket"), _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerSeatStatusChangedPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerSeatStatusChangedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),							_T(""), VariableType::VT_Int8, offsetof(PokerSeatStatusChangedPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("UserName"),						_T(""), VariableType::VT_String, offsetof(PokerSeatStatusChangedPacket,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Status"),						_T(""), VariableType::VT_Int8, offsetof(PokerSeatStatusChangedPacket,m_status), false, false, false)
	DECLARE_VAR(_T("SeatAvatar"),					_T(""), VariableType::VT_None, offsetof(PokerSeatStatusChangedPacket,m_seatAvatar), false, true, true)
END_DECLARE_VAR()

PokerSeatStatusChangedPacket::~PokerSeatStatusChangedPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSeatOnlineChangedPacket
//
/////////////////////////////////////////////////////
PokerSeatOnlineChangedPacket::PokerSeatOnlineChangedPacket(){
	m_nCode = PacketCodes::_PokerSeatOnlineChanged;
	}

BEGIN_DECLARE_VAR(PokerSeatOnlineChangedPacket, BasePacket)
	DECLARE_VAR(_T("PokerSeatOnlineChangedPacket"), _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerSeatOnlineChangedPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerSeatOnlineChangedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Seat"),							_T(""), VariableType::VT_Int8, offsetof(PokerSeatOnlineChangedPacket,m_cSeat), false, false, false)
	DECLARE_VAR(_T("Online"),						_T(""), VariableType::VT_Bool, offsetof(PokerSeatOnlineChangedPacket,m_bOnline), false, false, false)
END_DECLARE_VAR()

PokerSeatOnlineChangedPacket::~PokerSeatOnlineChangedPacket(){
	}


//////////////////////////////////////////////////////
//
//	class PokerGetHandHistoryPacket
//
/////////////////////////////////////////////////////
PokerGetHandHistoryPacket::PokerGetHandHistoryPacket(){
	m_nCode = PacketCodes::_PokerGetHandHistoryPacket;
	}

BEGIN_DECLARE_VAR(PokerGetHandHistoryPacket, BasePacket)
	DECLARE_VAR(_T("PokerGetHandHistoryPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("HandId"),						_T(""), VariableType::VT_Int64, offsetof(PokerGetHandHistoryPacket,m_nHandId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerGetHandHistoryPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("handData"),						_T(""), VariableType::VT_None, offsetof(PokerGetHandHistoryPacket,m_handData), false, true, true)
END_DECLARE_VAR()

PokerGetHandHistoryPacket::~PokerGetHandHistoryPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerQuitClientPacket
//
/////////////////////////////////////////////////////
PokerQuitClientPacket::PokerQuitClientPacket(){
	m_nCode = PacketCodes::_PokerQuitClient;
	}

BEGIN_DECLARE_VAR(PokerQuitClientPacket, BasePacket)
	DECLARE_VAR(_T("PokerQuitClientPacket"), _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ServerTime"),			_T(""), VariableType::VT_DateTime, offsetof(PokerQuitClientPacket,m_dtServerTime), false, false, false)
END_DECLARE_VAR()

PokerQuitClientPacket::~PokerQuitClientPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSetAvatarPacket
//
/////////////////////////////////////////////////////
PokerSetAvatarPacket::PokerSetAvatarPacket(){
	m_nCode = PacketCodes::_PokerSetAvatar;
	}

BEGIN_DECLARE_VAR(PokerSetAvatarPacket, BasePacket)
	DECLARE_VAR(_T("PokerSetAvatarPacket"), _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("AvatarId"),				_T(""), VariableType::VT_Int32, offsetof(PokerSetAvatarPacket,m_nAvatarId), false, false, false)
END_DECLARE_VAR()

PokerSetAvatarPacket::~PokerSetAvatarPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerUploadAvatarPacket
//
/////////////////////////////////////////////////////
PokerUploadAvatarPacket::PokerUploadAvatarPacket(){
	m_nCode = PacketCodes::_PokerUploadAvatar;
	}

BEGIN_DECLARE_VAR(PokerUploadAvatarPacket, BasePacket)
	DECLARE_VAR(_T("PokerUploadAvatarPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("AvatarId"),					_T(""), VariableType::VT_Int32, offsetof(PokerUploadAvatarPacket,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("Size"),						_T(""), VariableType::VT_Int16, offsetof(PokerUploadAvatarPacket,m_shSize), false, false, false)
	DECLARE_VAR(_T("Uploaded"),					_T(""), VariableType::VT_Int32, offsetof(PokerUploadAvatarPacket,m_nUploaded), false, false, false)
	DECLARE_VAR(_T("FullBinarySize"),			_T(""), VariableType::VT_Int32, offsetof(PokerUploadAvatarPacket,m_nFullBinarySize), false, false, false)
	DECLARE_VAR(_T("Completed"),				_T(""), VariableType::VT_Bool, offsetof(PokerUploadAvatarPacket,m_bCompleted), false, false, false)
	DECLARE_VAR(_T("imageData"),				_T(""), VariableType::VT_Binary, offsetof(PokerUploadAvatarPacket,m_imageData), false, false, false)
END_DECLARE_VAR()

PokerUploadAvatarPacket::~PokerUploadAvatarPacket(){
	}


//////////////////////////////////////////////////////
//
//	class PokerDownloadAvatarPacket
//
/////////////////////////////////////////////////////
PokerDownloadAvatarPacket::PokerDownloadAvatarPacket(){
	m_nCode = PacketCodes::_PokerDownloadAvatar;
	}

BEGIN_DECLARE_VAR(PokerDownloadAvatarPacket, BasePacket)
	DECLARE_VAR(_T("PokerDownloadAvatarPacket"),_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),					_T(""), VariableType::VT_Int32, offsetof(PokerDownloadAvatarPacket,m_nUserId), false, false, false)
	DECLARE_VAR(_T("AvatarId"),					_T(""), VariableType::VT_Int32, offsetof(PokerDownloadAvatarPacket,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("Size"),						_T(""), VariableType::VT_Int16, offsetof(PokerDownloadAvatarPacket,m_shSize), false, false, false)
	DECLARE_VAR(_T("Downloaded"),				_T(""), VariableType::VT_Int32, offsetof(PokerDownloadAvatarPacket,m_nDownloaded), false, false, false)
	DECLARE_VAR(_T("FullBinarySize"),			_T(""), VariableType::VT_Int32, offsetof(PokerDownloadAvatarPacket,m_nFullBinarySize), false, false, false)
	DECLARE_VAR(_T("Completed"),				_T(""), VariableType::VT_Bool, offsetof(PokerDownloadAvatarPacket,m_bCompleted), false, false, false)
	DECLARE_VAR(_T("imageData"),				_T(""), VariableType::VT_Binary, offsetof(PokerDownloadAvatarPacket,m_imageData), false, false, false)
END_DECLARE_VAR()

PokerDownloadAvatarPacket::~PokerDownloadAvatarPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerChatMessagePacket
//
/////////////////////////////////////////////////////
PokerChatMessagePacket::PokerChatMessagePacket(){
	m_nCode = PacketCodes::_PokerChatMessage;
	}

BEGIN_DECLARE_VAR(PokerChatMessagePacket, BasePacket)
	DECLARE_VAR(_T("PokerChatMessagePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerChatMessagePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""), VariableType::VT_Int32, offsetof(PokerChatMessagePacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("DestUserId"),				_T(""), VariableType::VT_Int32, offsetof(PokerChatMessagePacket,m_nDestUserId), false, false, false)
	DECLARE_VAR(_T("message"),					_T(""), VariableType::VT_None, offsetof(PokerChatMessagePacket,m_message), false, true, false)
END_DECLARE_VAR()

PokerChatMessagePacket::~PokerChatMessagePacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerOpenRandomTablePacket
//
/////////////////////////////////////////////////////
PokerOpenRandomTablePacket::PokerOpenRandomTablePacket(){
	m_nCode = PacketCodes::_PokerOpenRandomTable;
	}

BEGIN_DECLARE_VAR(PokerOpenRandomTablePacket, BasePacket)
	DECLARE_VAR(_T("PokerOpenRandomTablePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("GameType"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenRandomTablePacket,m_nGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),					_T(""), VariableType::VT_Int32, offsetof(PokerOpenRandomTablePacket,m_nGameSubType), false, false, false)
	DECLARE_VAR(_T("ReserveSeat"),					_T(""), VariableType::VT_Bool, offsetof(PokerOpenRandomTablePacket,m_bReserveSeat), false, false, false)
END_DECLARE_VAR()

PokerOpenRandomTablePacket::~PokerOpenRandomTablePacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerMoneyTransferPacket
//
/////////////////////////////////////////////////////
PokerMoneyTransferPacket::PokerMoneyTransferPacket(){
	m_nCode = PacketCodes::_PokerMoneyTransfer;
	}

BEGIN_DECLARE_VAR(PokerMoneyTransferPacket, BasePacket)
	DECLARE_VAR(_T("PokerMoneyTransferPacket"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TransAmount"),					_T(""), VariableType::VT_Int32, offsetof(PokerMoneyTransferPacket,m_nTransAmount), false, false, false)
	DECLARE_VAR(_T("AccountFrom"),					_T(""), VariableType::VT_Int32, offsetof(PokerMoneyTransferPacket,m_nAccountFrom), false, false, false)
	DECLARE_VAR(_T("AccountTo"),					_T(""), VariableType::VT_Int32, offsetof(PokerMoneyTransferPacket,m_nAccountTo), false, false, false)
	DECLARE_VAR(_T("BalanceFrom"),					_T(""), VariableType::VT_Int32, offsetof(PokerMoneyTransferPacket,m_nBalanceFrom), false, false, false)
	DECLARE_VAR(_T("BalanceTo"),					_T(""), VariableType::VT_Int32, offsetof(PokerMoneyTransferPacket,m_nBalanceTo), false, false, false)
	DECLARE_VAR(_T("RefreshBalances"),				_T(""), VariableType::VT_Bool, offsetof(PokerMoneyTransferPacket,m_bRefreshBalances), false, false, false)
END_DECLARE_VAR()

PokerMoneyTransferPacket::~PokerMoneyTransferPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerResetPlaymoneyPacket
//
/////////////////////////////////////////////////////
PokerResetPlaymoneyPacket::PokerResetPlaymoneyPacket(){
	m_nCode = PacketCodes::_PokerResetPlaymoney;
	}

BEGIN_DECLARE_VAR(PokerResetPlaymoneyPacket, BasePacket)
	DECLARE_VAR(_T("PokerResetPlaymoneyPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("PlayMoneyAmount"),				_T(""), VariableType::VT_Chips, offsetof(PokerResetPlaymoneyPacket,m_nPlayMoneyAmount), false, false, false)
END_DECLARE_VAR()

PokerResetPlaymoneyPacket::~PokerResetPlaymoneyPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerRefreshBalancePacket
//
/////////////////////////////////////////////////////
PokerRefreshBalancePacket::PokerRefreshBalancePacket(){
	m_nCode = PacketCodes::_PokerRefreshBalance;
	}

BEGIN_DECLARE_VAR(PokerRefreshBalancePacket, BasePacket)
	DECLARE_VAR(_T("PokerRefreshBalancePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Balance"),						_T(""), VariableType::VT_Chips, offsetof(PokerRefreshBalancePacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("CashInPlay"),					_T(""), VariableType::VT_Chips, offsetof(PokerRefreshBalancePacket,m_nCashInPlay), false, false, false)
	DECLARE_VAR(_T("PlayMoneyAmount"),				_T(""), VariableType::VT_Chips, offsetof(PokerRefreshBalancePacket,m_nPlayMoneyAmount), false, false, false)
	DECLARE_VAR(_T("PlayMoneyInPlay"),				_T(""), VariableType::VT_Chips, offsetof(PokerRefreshBalancePacket,m_nPlayMoneyInPlay), false, false, false)
	DECLARE_VAR(_T("VPPoints"),						_T(""), VariableType::VT_Double, offsetof(PokerRefreshBalancePacket,m_dVPPoints), false, false, false)
	DECLARE_VAR(_T("TicketsCt"),					_T(""), VariableType::VT_Int8, offsetof(PokerRefreshBalancePacket,m_cTicketsCt), false, false, false)
END_DECLARE_VAR()

PokerRefreshBalancePacket::~PokerRefreshBalancePacket(){
	}

//////////////////////////////////////////////////////
//
//	class FindPlayerTableInfo
//
/////////////////////////////////////////////////////
FindPlayerTableInfo::FindPlayerTableInfo(){
	}

BEGIN_DECLARE_VAR(FindPlayerTableInfo, Serializable)
	DECLARE_VAR(_T("FindPlayerTableInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T(""), VariableType::VT_Int32, offsetof(FindPlayerTableInfo,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T(""), VariableType::VT_Int32, offsetof(FindPlayerTableInfo,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableName"),			_T(""), VariableType::VT_String, offsetof(FindPlayerTableInfo,m_sTableName), false, false, false)
	DECLARE_VAR(_T("GameType"),				_T(""), VariableType::VT_Int8, offsetof(FindPlayerTableInfo,m_cGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),			_T(""), VariableType::VT_Int16, offsetof(FindPlayerTableInfo,m_shGameSubType), false, false, false)
	DECLARE_VAR(_T("MaxSeatCt"),			_T(""), VariableType::VT_Int8, offsetof(FindPlayerTableInfo,m_cMaxSeatCt), false, false, false)
	DECLARE_VAR(_T("SBAmount"),				_T(""), VariableType::VT_Int32, offsetof(FindPlayerTableInfo,m_nSBAmount), false, false, false)
	DECLARE_VAR(_T("BBAmount"),				_T(""), VariableType::VT_Int32, offsetof(FindPlayerTableInfo,m_nBBAmount), false, false, false)
	DECLARE_VAR(_T("AnteAmount"),			_T(""), VariableType::VT_Int32, offsetof(FindPlayerTableInfo,m_nAnteAmount), false, false, false)
END_DECLARE_VAR()

FindPlayerTableInfo::~FindPlayerTableInfo(){
	}

//////////////////////////////////////////////////////
//
//	class PokerFindPlayerPacket
//
/////////////////////////////////////////////////////
PokerFindPlayerPacket::PokerFindPlayerPacket(){
	m_nCode = PacketCodes::_PokerFindPlayer;
	}

BEGIN_DECLARE_VAR(PokerFindPlayerPacket, BasePacket)
	DECLARE_VAR(_T("PokerFindPlayerPacket"),_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserName"),				_T(""), VariableType::VT_String, offsetof(PokerFindPlayerPacket,m_sUserName), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T(""), VariableType::VT_Int32, offsetof(PokerFindPlayerPacket,m_nUserId), false, false, false)
	DECLARE_VAR(_T("Tables"),				_T(""), VariableType::VT_None, offsetof(PokerFindPlayerPacket,m_listTables), false, true, true)
END_DECLARE_VAR()

PokerFindPlayerPacket::~PokerFindPlayerPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerFindTournamentPacket
//
/////////////////////////////////////////////////////
PokerFindTournamentPacket::PokerFindTournamentPacket(){
	m_nCode = PacketCodes::_PokerFindTournament;
	}

BEGIN_DECLARE_VAR(PokerFindTournamentPacket, BasePacket)
	DECLARE_VAR(_T("PokerFindTournamentPacket"),_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentID"),				_T(""), VariableType::VT_String, offsetof(PokerFindTournamentPacket,m_sTournamentID), false, false, false)
	DECLARE_VAR(_T("Tournaments"),				_T(""), VariableType::VT_None, offsetof(PokerFindTournamentPacket,m_listTournaments), false, true, true)
END_DECLARE_VAR()

PokerFindTournamentPacket::~PokerFindTournamentPacket(){
	}

//=====================================================================================================================
//
//	Poker tournament packets
//
//=====================================================================================================================

//////////////////////////////////////////////////////
//
//	class PokerTournamentCustomInfoPacket
//
/////////////////////////////////////////////////////
PokerTournamentCustomInfoPacket::PokerTournamentCustomInfoPacket(){
	m_nCode = PacketCodes::_PokerTournamentCustomInfo;
	}

BEGIN_DECLARE_VAR(PokerTournamentCustomInfoPacket, BasePacket)
	DECLARE_VAR(_T("PokerTournamentCustomInfoPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),						_T(""), VariableType::VT_Int32, offsetof(PokerTournamentCustomInfoPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Flags"),							_T(""), VariableType::VT_Int32, offsetof(PokerTournamentCustomInfoPacket,m_nFlags), false, false, false)
	DECLARE_VAR(_T("GameTableIndex"),					_T(""), VariableType::VT_Int16, offsetof(PokerTournamentCustomInfoPacket,m_shGameTableIndex), false, false, false)
	DECLARE_VAR(_T("GameTableCt"),						_T(""), VariableType::VT_Int16, offsetof(PokerTournamentCustomInfoPacket,m_shGameTableCt), false, false, false)
	DECLARE_VAR(_T("ParticipantIndex"),					_T(""), VariableType::VT_Int16, offsetof(PokerTournamentCustomInfoPacket,m_shParticipantIndex), false, false, false)
	DECLARE_VAR(_T("ParticipantCt"),					_T(""), VariableType::VT_Int16, offsetof(PokerTournamentCustomInfoPacket,m_shParticipantCt), false, false, false)
	DECLARE_VAR(_T("GameTableIndexPlayers"),			_T(""), VariableType::VT_Int16, offsetof(PokerTournamentCustomInfoPacket,m_shGameTableIndexPlayers), false, false, false)
	DECLARE_VAR(_T("Result"),							_T(""), VariableType::VT_None, offsetof(PokerTournamentCustomInfoPacket,m_listResult), false, true, true)
END_DECLARE_VAR()

PokerTournamentCustomInfoPacket::~PokerTournamentCustomInfoPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerRegisterInTournamentPacket
//
/////////////////////////////////////////////////////
PokerRegisterInTournamentPacket::PokerRegisterInTournamentPacket(){
	m_nCode = PacketCodes::_PokerRegisterInTournament;
	}

BEGIN_DECLARE_VAR(PokerRegisterInTournamentPacket, BasePacket)
	DECLARE_VAR(_T("PokerRegisterInTournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),						_T(""), VariableType::VT_Int32, offsetof(PokerRegisterInTournamentPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("RegisterBy"),						_T(""), VariableType::VT_Int8, offsetof(PokerRegisterInTournamentPacket,m_cRegisterBy), false, false, false)
	DECLARE_VAR(_T("Balance"),							_T(""), VariableType::VT_Chips, offsetof(PokerRegisterInTournamentPacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("Points"),							_T(""), VariableType::VT_Double, offsetof(PokerRegisterInTournamentPacket,m_dPoints), false, false, false)
	DECLARE_VAR(_T("PlayerInfo"),						_T(""), VariableType::VT_None, offsetof(PokerRegisterInTournamentPacket,m_listPlayerInfo), false, true, true)
	DECLARE_VAR(_T("RegTournamentInfo"),				_T(""), VariableType::VT_None, offsetof(PokerRegisterInTournamentPacket,m_listRegTournament), false, true, true)
END_DECLARE_VAR()

PokerRegisterInTournamentPacket::~PokerRegisterInTournamentPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerUnregisterFromTournamentPacket
//
/////////////////////////////////////////////////////
PokerUnregisterFromTournamentPacket::PokerUnregisterFromTournamentPacket(){
	m_nCode = PacketCodes::_PokerUnregisterFromTournament;
	}

BEGIN_DECLARE_VAR(PokerUnregisterFromTournamentPacket, BasePacket)
	DECLARE_VAR(_T("PokerUnregisterFromTournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),							_T(""), VariableType::VT_Int32, offsetof(PokerUnregisterFromTournamentPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Balance"),								_T(""), VariableType::VT_Chips, offsetof(PokerUnregisterFromTournamentPacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("Points"),								_T(""), VariableType::VT_Double, offsetof(PokerUnregisterFromTournamentPacket,m_dPoints), false, false, false)
END_DECLARE_VAR()

PokerUnregisterFromTournamentPacket::~PokerUnregisterFromTournamentPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerOpenTable_TournamentPacket
//
/////////////////////////////////////////////////////
PokerOpenTable_TournamentPacket::PokerOpenTable_TournamentPacket(){
	m_nCode = PacketCodes::_PokerOpenTable_Tournament;
	}

BEGIN_DECLARE_VAR(PokerOpenTable_TournamentPacket, PokerOpenTablePacket)
	DECLARE_VAR(_T("PokerOpenTable_TournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("LookForMyTable"),					_T(""), VariableType::VT_Bool, offsetof(PokerOpenTable_TournamentPacket,m_bLookForMyTable), false, false, false)
	DECLARE_VAR(_T("TableWaitingState"),				_T(""), VariableType::VT_Int8, offsetof(PokerOpenTable_TournamentPacket,m_btTableWaitingState), false, false, false)	
	DECLARE_VAR(_T("WaitSec"),							_T(""), VariableType::VT_Int8, offsetof(PokerOpenTable_TournamentPacket,m_btWaitSec), false, false, false)	

	DECLARE_VAR(_T("RebuyAllowed"),						_T(""), VariableType::VT_Bool, offsetof(PokerOpenTable_TournamentPacket,m_bRebuyAllowed), false, false, false)
	DECLARE_VAR(_T("RebuyCtLeft"),						_T(""), VariableType::VT_Int8, offsetof(PokerOpenTable_TournamentPacket,m_btRebuyCtLeft), false, false, false)
	DECLARE_VAR(_T("AddonAllowed"),						_T(""), VariableType::VT_Bool, offsetof(PokerOpenTable_TournamentPacket,m_bAddonAllowed), false, false, false)
	DECLARE_VAR(_T("AddonCtLeft"),						_T(""), VariableType::VT_Int8, offsetof(PokerOpenTable_TournamentPacket,m_btAddonCtLeft), false, false, false)
	DECLARE_VAR(_T("LevelIndex"),						_T(""), VariableType::VT_Int8, offsetof(PokerOpenTable_TournamentPacket,m_cLevelIndex), false, false, false)
	DECLARE_VAR(_T("LevelSecLeft"),						_T(""), VariableType::VT_Int16, offsetof(PokerOpenTable_TournamentPacket,m_shLevelSecLeft), false, false, false)
	DECLARE_VAR(_T("BreakSecLeft"),						_T(""), VariableType::VT_Int16, offsetof(PokerOpenTable_TournamentPacket,m_shBreakSecLeft), false, false, false)
	DECLARE_VAR(_T("SmallBlind_Next"),					_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nSmallBlind_Next), false, false, false)
	DECLARE_VAR(_T("BigBlind_Next"),					_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nBigBlind_Next), false, false, false)
	DECLARE_VAR(_T("Ante_Next"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nAnte_Next), false, false, false)

	DECLARE_VAR(_T("RebuyChipsCt"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nRebuyChipsCt), false, false, false)
	DECLARE_VAR(_T("AddonChipsCt"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nAddonChipsCt), false, false, false)
	DECLARE_VAR(_T("RebuyAmount"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nRebuyAmount), false, false, false)
	DECLARE_VAR(_T("RebuyVPPAmount"),					_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nRebuyVPPAmount), false, false, false)
	DECLARE_VAR(_T("AddonAmount"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nAddonAmount), false, false, false)
	DECLARE_VAR(_T("AddonVPPAmount"),					_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nAddonVPPAmount), false, false, false)

	DECLARE_VAR(_T("RankIndex"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nRankIndex), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),						_T(""), VariableType::VT_Int32, offsetof(PokerOpenTable_TournamentPacket,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TablesCt"),							_T(""), VariableType::VT_Int16, offsetof(PokerOpenTable_TournamentPacket,m_shTablesCt), false, false, false)
END_DECLARE_VAR()

PokerOpenTable_TournamentPacket::~PokerOpenTable_TournamentPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerTournamentTableInfoPacket
//
/////////////////////////////////////////////////////
PokerTournamentTableInfoPacket::PokerTournamentTableInfoPacket(){
	m_nCode = PacketCodes::_PokerTournamentTableInfo;
	}

PokerTournamentTableInfoPacket::~PokerTournamentTableInfoPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerRedirectPacket
//
/////////////////////////////////////////////////////
PokerRedirectPacket::PokerRedirectPacket(){
	m_nCode = PacketCodes::_PokerRedirect;
	}

BEGIN_DECLARE_VAR(PokerRedirectPacket, BasePacket)
	DECLARE_VAR(_T("PokerRedirectPacket"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""), VariableType::VT_Int32, offsetof(PokerRedirectPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""), VariableType::VT_Int32, offsetof(PokerRedirectPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId_Redirect"),			_T(""), VariableType::VT_Int32, offsetof(PokerRedirectPacket,m_nTableId_Redirect), false, false, false)
	DECLARE_VAR(_T("TournamentId_Redirect"),	_T(""), VariableType::VT_Int32, offsetof(PokerRedirectPacket,m_nTournamentId_Redirect), false, false, false)
END_DECLARE_VAR()

PokerRedirectPacket::~PokerRedirectPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerLeaveTable_TournamentPacket
//
/////////////////////////////////////////////////////
PokerLeaveTable_TournamentPacket::PokerLeaveTable_TournamentPacket(){
	m_nCode = PacketCodes::_PokerLeaveTable_Tournament;
	}

BEGIN_DECLARE_VAR(PokerLeaveTable_TournamentPacket, PokerLeaveTablePacket)
	DECLARE_VAR(_T("PokerLeaveTable_TournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),						_T(""), VariableType::VT_Int32, offsetof(PokerLeaveTable_TournamentPacket,m_nTournamentId), false, false, false)
END_DECLARE_VAR()

PokerLeaveTable_TournamentPacket::~PokerLeaveTable_TournamentPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerLevelChangedPacket
//
/////////////////////////////////////////////////////
PokerLevelChangedPacket::PokerLevelChangedPacket(){
	m_nCode = PacketCodes::_PokerLevelChanged;
	}

BEGIN_DECLARE_VAR(PokerLevelChangedPacket, BasePacket)
	DECLARE_VAR(_T("PokerLevelChangedPacket"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("LevelIndex"),					_T(""), VariableType::VT_Int8, offsetof(PokerLevelChangedPacket,m_cLevelIndex), false, false, false)
	DECLARE_VAR(_T("RebuyAllowed"),					_T(""), VariableType::VT_Bool, offsetof(PokerLevelChangedPacket,m_bRebuyAllowed), false, false, false)
	DECLARE_VAR(_T("LevelSecLeft"),					_T(""), VariableType::VT_Int16, offsetof(PokerLevelChangedPacket,m_shLevelSecLeft), false, false, false)
	DECLARE_VAR(_T("NextBreakSecLeft"),				_T(""), VariableType::VT_Int16, offsetof(PokerLevelChangedPacket,m_shNextBreakSecLeft), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),					_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),						_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nBigBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),							_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nAnte), false, false, false)
	DECLARE_VAR(_T("SmallBlind_Next"),				_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nSmallBlind_Next), false, false, false)
	DECLARE_VAR(_T("BigBlind_Next"),				_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nBigBlind_Next), false, false, false)
	DECLARE_VAR(_T("Ante_Next"),					_T(""), VariableType::VT_Int32, offsetof(PokerLevelChangedPacket,m_nAnte_Next), false, false, false)
END_DECLARE_VAR()

PokerLevelChangedPacket::~PokerLevelChangedPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerBreakPacket
//
/////////////////////////////////////////////////////
PokerBreakPacket::PokerBreakPacket(){
	m_nCode = PacketCodes::_PokerBreak;
	}

BEGIN_DECLARE_VAR(PokerBreakPacket, BasePacket)
	DECLARE_VAR(_T("PokerBreakPacket"),				_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerBreakPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerBreakPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("LevelIndex"),					_T(""), VariableType::VT_Int8, offsetof(PokerBreakPacket,m_cLevelIndex), false, false, false)
	DECLARE_VAR(_T("BreakSecLeft"),					_T(""), VariableType::VT_Int16, offsetof(PokerBreakPacket,m_shBreakSecLeft), false, false, false)
	DECLARE_VAR(_T("AddonBreak"),					_T(""), VariableType::VT_Bool, offsetof(PokerBreakPacket,m_bAddonBreak), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),					_T(""), VariableType::VT_Int32, offsetof(PokerBreakPacket,m_nSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),						_T(""), VariableType::VT_Int32, offsetof(PokerBreakPacket,m_nBigBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),							_T(""), VariableType::VT_Int32, offsetof(PokerBreakPacket,m_nAnte), false, false, false)
END_DECLARE_VAR()

PokerBreakPacket::~PokerBreakPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerWaitForPacket
//
/////////////////////////////////////////////////////
PokerWaitForPacket::PokerWaitForPacket(){
	m_nCode = PacketCodes::_PokerWaitFor;
	}

BEGIN_DECLARE_VAR(PokerWaitForPacket, BasePacket)
	DECLARE_VAR(_T("PokerWaitForPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),			_T(""), VariableType::VT_Int32, offsetof(PokerWaitForPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),				_T(""), VariableType::VT_Int32, offsetof(PokerWaitForPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("WaitFor"),				_T(""), VariableType::VT_Int8, offsetof(PokerWaitForPacket,m_cWaitFor), false, false, false)
	DECLARE_VAR(_T("WaitDurationSec"),		_T(""), VariableType::VT_Int16, offsetof(PokerWaitForPacket,m_shWaitDurationSec), false, false, false)
END_DECLARE_VAR()

PokerWaitForPacket::~PokerWaitForPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerTournamentFinishedPacket
//
/////////////////////////////////////////////////////
PokerTournamentFinishedPacket::PokerTournamentFinishedPacket(){
	m_nCode = PacketCodes::_PokerTournamentFinished;
	}

BEGIN_DECLARE_VAR(PokerTournamentFinishedPacket, BasePacket)
	DECLARE_VAR(_T("PokerTournamentFinishedPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),						_T(""), VariableType::VT_Int32, offsetof(PokerTournamentFinishedPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),							_T(""), VariableType::VT_Int32, offsetof(PokerTournamentFinishedPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Canceled"),							_T(""), VariableType::VT_Bool, offsetof(PokerTournamentFinishedPacket,m_bCanceled), false, false, false)
	DECLARE_VAR(_T("Rank"),								_T(""), VariableType::VT_Int32, offsetof(PokerTournamentFinishedPacket,m_nRank), false, false, false)
	DECLARE_VAR(_T("AmountWon"),						_T(""), VariableType::VT_Chips, offsetof(PokerTournamentFinishedPacket,m_nAmountWon), false, false, false)
END_DECLARE_VAR()

PokerTournamentFinishedPacket::~PokerTournamentFinishedPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerRebuyPacket
//
/////////////////////////////////////////////////////
PokerRebuyPacket::PokerRebuyPacket(){
	m_nCode = PacketCodes::_PokerRebuy;
	}

BEGIN_DECLARE_VAR(PokerRebuyPacket, BasePacket)
	DECLARE_VAR(_T("PokerRebuyPacket"),				_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerRebuyPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerRebuyPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("ByVPPAmount"),					_T(""), VariableType::VT_Bool, offsetof(PokerRebuyPacket,m_bByVPPAmount), false, false, false)
	DECLARE_VAR(_T("RebuyMultiplier"),				_T(""), VariableType::VT_Int8, offsetof(PokerRebuyPacket,m_btRebuyMultiplier), false, false, false)
	DECLARE_VAR(_T("ChipsAmountNew"),				_T(""), VariableType::VT_Chips, offsetof(PokerRebuyPacket,m_nChipsAmountNew), false, false, false)
	DECLARE_VAR(_T("ChipsAdded"),					_T(""), VariableType::VT_Chips, offsetof(PokerRebuyPacket,m_nChipsAdded), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),					_T(""), VariableType::VT_Chips, offsetof(PokerRebuyPacket,m_nBalanceNew), false, false, false)
	DECLARE_VAR(_T("VPPBalanceNew"),				_T(""), VariableType::VT_Float, offsetof(PokerRebuyPacket,m_fVPPBalanceNew), false, false, false)
	DECLARE_VAR(_T("RebuyCtLeft"),					_T(""), VariableType::VT_Int8, offsetof(PokerRebuyPacket,m_btRebuyCtLeft), false, false, false)
END_DECLARE_VAR()

PokerRebuyPacket::~PokerRebuyPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerAddonPacket
//
/////////////////////////////////////////////////////
PokerAddonPacket::PokerAddonPacket(){
	m_nCode = PacketCodes::_PokerAddon;
	}

BEGIN_DECLARE_VAR(PokerAddonPacket, BasePacket)
	DECLARE_VAR(_T("PokerAddonPacket"),				_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerAddonPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerAddonPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("ByVPPAmount"),					_T(""), VariableType::VT_Bool, offsetof(PokerRebuyPacket,m_bByVPPAmount), false, false, false)
	DECLARE_VAR(_T("ChipsAmountNew"),				_T(""), VariableType::VT_Chips, offsetof(PokerAddonPacket,m_nChipsAmountNew), false, false, false)
	DECLARE_VAR(_T("ChipsAdded"),					_T(""), VariableType::VT_Chips, offsetof(PokerAddonPacket,m_nChipsAdded), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),					_T(""), VariableType::VT_Chips, offsetof(PokerAddonPacket,m_nBalanceNew), false, false, false)
	DECLARE_VAR(_T("VPPBalanceNew"),				_T(""), VariableType::VT_Float, offsetof(PokerAddonPacket,m_fVPPBalanceNew), false, false, false)
	DECLARE_VAR(_T("AddonCtLeft"),					_T(""), VariableType::VT_Int8, offsetof(PokerAddonPacket,m_btAddonCtLeft), false, false, false)
END_DECLARE_VAR()

PokerAddonPacket::~PokerAddonPacket(){
	}