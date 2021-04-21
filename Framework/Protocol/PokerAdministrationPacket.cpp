#include "stdafx.h"
#include "PokerAdministrationPacket.h"
#include "../PokerServer/Object/PokerHandHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////
//
//	class PokerBlockUserPacket
//
/////////////////////////////////////////////////////

PokerBlockUserPacket::PokerBlockUserPacket(){
	m_nCode = PacketCodes::_PokerBlockUnblockUser;
	}

BEGIN_DECLARE_VAR(PokerBlockUserPacket, Serializable)
	DECLARE_VAR(_T("PokerBlockUserPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),				_T(""), VariableType::VT_Int32, offsetof(PokerBlockUserPacket,m_nUserId), false, false, false)
	DECLARE_VAR(_T("Block"),				_T(""), VariableType::VT_Bool, offsetof(PokerBlockUserPacket,m_bBlock), false, false, false)
END_DECLARE_VAR()

PokerBlockUserPacket::~PokerBlockUserPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerGameTableInfoObject
//
/////////////////////////////////////////////////////

PokerGameTableInfoObject::PokerGameTableInfoObject(){
	}

BEGIN_DECLARE_VAR(PokerGameTableInfoObject, Serializable)
	DECLARE_VAR(_T("PokerGameTableInfoObject"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),						_T(""), VariableType::VT_Int32, offsetof(PokerGameTableInfoObject,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),						_T(""), VariableType::VT_String, offsetof(PokerGameTableInfoObject,m_sName), false, false, false)
	DECLARE_VAR(_T("GameType"),					_T(""), VariableType::VT_Int32, offsetof(PokerGameTableInfoObject,m_nGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),				_T(""), VariableType::VT_Int32, offsetof(PokerGameTableInfoObject,m_nGameSubType), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),				_T(""), VariableType::VT_Int32, offsetof(PokerGameTableInfoObject,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),				_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),					_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fBigBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),						_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fAnte), false, false, false)
	DECLARE_VAR(_T("RakePercent"),				_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fRakePercent), false, false, false)
	DECLARE_VAR(_T("MaxRakeAmount"),			_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fMaxRakeAmount), false, false, false)
	DECLARE_VAR(_T("MinBuyin"),					_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fMinBuyin), false, false, false)
	DECLARE_VAR(_T("MaxBuyin"),					_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fMaxBuyin), false, false, false)
	DECLARE_VAR(_T("MaxSeatCt"),				_T(""), VariableType::VT_Int8, offsetof(PokerGameTableInfoObject,m_cMaxSeatCt), false, false, false)
	DECLARE_VAR(_T("Password"),					_T(""), VariableType::VT_String, offsetof(PokerGameTableInfoObject,m_sPassword), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),				_T(""), VariableType::VT_Int8, offsetof(PokerGameTableInfoObject,m_cPlayersCt), false, false, false)
	DECLARE_VAR(_T("WaitingPlayersCt"),			_T(""), VariableType::VT_Int8, offsetof(PokerGameTableInfoObject,m_cWaitingPlayersCt), false, false, false)
	DECLARE_VAR(_T("AvgPotAmount"),				_T(""), VariableType::VT_Float, offsetof(PokerGameTableInfoObject,m_fAvgPotAmount), false, false, false)
	DECLARE_VAR(_T("HandPerHour"),				_T(""), VariableType::VT_Int16, offsetof(PokerGameTableInfoObject,m_shHandPerHour), false, false, false)
	DECLARE_VAR(_T("ActiveStatus"),				_T(""), VariableType::VT_Bool, offsetof(PokerGameTableInfoObject,m_bActiveStatus), false, false, false)
END_DECLARE_VAR()

PokerGameTableInfoObject::~PokerGameTableInfoObject(){
	}

//////////////////////////////////////////////////////
//
//	class PokerGetGameTablesPacket
//
/////////////////////////////////////////////////////

PokerGetGameTablesPacket::PokerGetGameTablesPacket(){
	m_nCode = PacketCodes::_PokerGetGameTablesByType;
	}

BEGIN_DECLARE_VAR(PokerGetGameTablesPacket, BasePacket)
	DECLARE_VAR(_T("PokerGetGameTablesPacket"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("GameType"),						_T(""), VariableType::VT_Int8, offsetof(PokerGetGameTablesPacket,m_cGameType), false, false, false)
	DECLARE_VAR(_T("SubType"),						_T(""), VariableType::VT_Int16, offsetof(PokerGetGameTablesPacket,m_shSubType), false, false, false)
	DECLARE_VAR(_T("Result"),						_T(""),	VariableType::VT_None, offsetof(PokerGetGameTablesPacket,m_listResult), false, true, true)
END_DECLARE_VAR()

PokerGetGameTablesPacket::~PokerGetGameTablesPacket(){
	}

//////////////////////////////////////////////////////
//
//	class TournamentInfoObject
//
/////////////////////////////////////////////////////

TournamentInfoObject::TournamentInfoObject(){
	}

BEGIN_DECLARE_VAR(TournamentInfoObject, Serializable)
	DECLARE_VAR(_T("TournamentInfoObject"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),						_T(""), VariableType::VT_Int32, offsetof(TournamentInfoObject,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),						_T(""), VariableType::VT_String, offsetof(TournamentInfoObject,m_sName), false, false, false)
	DECLARE_VAR(_T("Description"),				_T(""), VariableType::VT_String, offsetof(TournamentInfoObject,m_sDescription), false, false, false)
	DECLARE_VAR(_T("TournamentType"),			_T(""), VariableType::VT_Int32, offsetof(TournamentInfoObject,m_nTournamentType), false, false, false)
	DECLARE_VAR(_T("GameType"),					_T(""), VariableType::VT_Int8, offsetof(TournamentInfoObject,m_cGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),				_T(""), VariableType::VT_Int16, offsetof(TournamentInfoObject,m_nGameSubType), false, false, false)
	DECLARE_VAR(_T("Status"),					_T(""), VariableType::VT_Int8, offsetof(TournamentInfoObject,m_cStatus), false, false, false)
	DECLARE_VAR(_T("RegisteredCt"),				_T(""), VariableType::VT_Int32, offsetof(TournamentInfoObject,m_nRegisteredCt), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),				_T(""), VariableType::VT_Int32, offsetof(TournamentInfoObject,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TablesCt"),					_T(""), VariableType::VT_Int32, offsetof(TournamentInfoObject,m_nTablesCt), false, false, false)
	DECLARE_VAR(_T("StartTime"),				_T(""), VariableType::VT_DateTime, offsetof(TournamentInfoObject,m_dtStartTime), false, false, false)
	DECLARE_VAR(_T("RegStartTime"),				_T(""), VariableType::VT_DateTime, offsetof(TournamentInfoObject,m_dtRegStartTime), false, false, false)
	DECLARE_VAR(_T("RegEndTime"),				_T(""), VariableType::VT_DateTime, offsetof(TournamentInfoObject,m_dtRegEndTime), false, false, false)
	DECLARE_VAR(_T("FinishedTime"),				_T(""), VariableType::VT_DateTime, offsetof(TournamentInfoObject,m_dtFinishedTime), false, false, false)
END_DECLARE_VAR()

TournamentInfoObject::~TournamentInfoObject(){
	}

//////////////////////////////////////////////////////
//
//	class PokerGetTournamentsPacket
//
/////////////////////////////////////////////////////

PokerGetTournamentsPacket::PokerGetTournamentsPacket(){
	m_nCode = PacketCodes::_PokerGetTournamentsByType;
	}

BEGIN_DECLARE_VAR(PokerGetTournamentsPacket, BasePacket)
	DECLARE_VAR(_T("PokerGetTournamentsPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentType"),				_T(""), VariableType::VT_Int32, offsetof(PokerGetTournamentsPacket,m_nTournamentType), false, false, false)
	DECLARE_VAR(_T("Result"),						_T(""),	VariableType::VT_None, offsetof(PokerGetTournamentsPacket,m_listResult), false, true, true)
END_DECLARE_VAR()

PokerGetTournamentsPacket::~PokerGetTournamentsPacket(){
	}

//////////////////////////////////////////////////////
//
//	class ClientConnectionInfoObject
//
/////////////////////////////////////////////////////

ClientConnectionInfoObject::ClientConnectionInfoObject(){
	}

BEGIN_DECLARE_VAR(ClientConnectionInfoObject, Serializable)
	DECLARE_VAR(_T("ClientConnectionInfoObject"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ClientIP"),						_T(""), VariableType::VT_Int32, offsetof(ClientConnectionInfoObject,m_nClientIP), false, false, false)
	DECLARE_VAR(_T("ClientPort"),					_T(""), VariableType::VT_Int32, offsetof(ClientConnectionInfoObject,m_nClientPort), false, false, false)
	DECLARE_VAR(_T("ServerPort"),					_T(""), VariableType::VT_Int32, offsetof(ClientConnectionInfoObject,m_nServerPort), false, false, false)
	DECLARE_VAR(_T("SessionId"),					_T(""), VariableType::VT_Int64, offsetof(ClientConnectionInfoObject,m_nSessionId), false, false, false)
	DECLARE_VAR(_T("UserId"),						_T(""), VariableType::VT_Int32, offsetof(ClientConnectionInfoObject,m_nUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),						_T(""), VariableType::VT_String, offsetof(ClientConnectionInfoObject,m_sUserName), false, false, false)
	DECLARE_VAR(_T("TimeConnected"),				_T(""), VariableType::VT_DateTime, offsetof(ClientConnectionInfoObject,m_dtTimeConnected), false, false, false)
	DECLARE_VAR(_T("ReceiveBytesCt"),				_T(""), VariableType::VT_Int32, offsetof(ClientConnectionInfoObject,m_nReceivedBytesCt), false, false, false)
	DECLARE_VAR(_T("SentBytesCt"),					_T(""), VariableType::VT_Int32, offsetof(ClientConnectionInfoObject,m_nSentBytesCt), false, false, false)
END_DECLARE_VAR()

ClientConnectionInfoObject::~ClientConnectionInfoObject(){
	}


//////////////////////////////////////////////////////
//
//	class PokerGetClientConnectionsPacket
//
/////////////////////////////////////////////////////

PokerGetClientConnectionsPacket::PokerGetClientConnectionsPacket(){
	m_nCode = PacketCodes::_PokerGetClientConnections;
	}

BEGIN_DECLARE_VAR(PokerGetClientConnectionsPacket, BasePacket)
	DECLARE_VAR(_T("PokerGetClientConnectionsPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("MaxConnectionCt"),					_T(""), VariableType::VT_Int32, offsetof(PokerGetClientConnectionsPacket,m_nMaxConnectionCt), false, false, false)
	DECLARE_VAR(_T("Result"),							_T(""),	VariableType::VT_None, offsetof(PokerGetClientConnectionsPacket,m_listResult), false, true, true)
END_DECLARE_VAR()

PokerGetClientConnectionsPacket::~PokerGetClientConnectionsPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSendAdminMessageByTablePacket
//
/////////////////////////////////////////////////////

PokerSendAdminMessageByTablePacket::PokerSendAdminMessageByTablePacket(){
	m_nCode = PacketCodes::_PokerSendAdminMessageByTable;
	}

BEGIN_DECLARE_VAR(PokerSendAdminMessageByTablePacket, BasePacket)
	DECLARE_VAR(_T("PokerSendAdminMessageByTablePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),								_T(""), VariableType::VT_Int32, offsetof(PokerSendAdminMessageByTablePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),							_T(""), VariableType::VT_Int32, offsetof(PokerSendAdminMessageByTablePacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Message"),								_T(""),	VariableType::VT_String, offsetof(PokerSendAdminMessageByTablePacket,m_sMessage), false, false, false)
END_DECLARE_VAR()

PokerSendAdminMessageByTablePacket::~PokerSendAdminMessageByTablePacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerSendAdminMessageByUserPacket
//
/////////////////////////////////////////////////////

PokerSendAdminMessageByUserPacket::PokerSendAdminMessageByUserPacket(){
	m_nCode = PacketCodes::_PokerSendAdminMessageByUser;
	}

BEGIN_DECLARE_VAR(PokerSendAdminMessageByUserPacket, BasePacket)
	DECLARE_VAR(_T("PokerSendAdminMessageByUserPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),								_T(""), VariableType::VT_Int32, offsetof(PokerSendAdminMessageByUserPacket,m_nUserId), false, false, false)
	DECLARE_VAR(_T("SessionId"),							_T(""), VariableType::VT_Int64, offsetof(PokerSendAdminMessageByUserPacket,m_nSessionId), false, false, false)
	DECLARE_VAR(_T("Message"),								_T(""),	VariableType::VT_String, offsetof(PokerSendAdminMessageByUserPacket,m_sMessage), false, false, false)
END_DECLARE_VAR()

PokerSendAdminMessageByUserPacket::~PokerSendAdminMessageByUserPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerCreateGameTablePacket
//
/////////////////////////////////////////////////////

PokerCreateGameTablePacket::PokerCreateGameTablePacket(){
	m_nCode = PacketCodes::_PokerCreateGameTable;
	}

BEGIN_DECLARE_VAR(PokerCreateGameTablePacket, BasePacket)
	DECLARE_VAR(_T("PokerCreateGameTablePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),						_T(""), VariableType::VT_Int32, offsetof(PokerCreateGameTablePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TableInfo"),					_T(""),	VariableType::VT_None, offsetof(PokerCreateGameTablePacket,m_tableInfo), false, true, true)
END_DECLARE_VAR()

PokerCreateGameTablePacket::~PokerCreateGameTablePacket(){
	}

//////////////////////////////////////////////////////
//
//	class CreateGameTableObject
//
/////////////////////////////////////////////////////

CreateGameTableObject::CreateGameTableObject(){
	}

BEGIN_DECLARE_VAR(CreateGameTableObject, Serializable)
	DECLARE_VAR(_T("CreateGameTableObject"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("GameType"),						_T(""), VariableType::VT_Int8, offsetof(CreateGameTableObject,m_cGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),					_T(""), VariableType::VT_Int16, offsetof(CreateGameTableObject,m_shGameSubType), false, false, false)
	DECLARE_VAR(_T("GameSpeedId"),					_T(""), VariableType::VT_Int32, offsetof(CreateGameTableObject,m_nGameSpeedId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),					_T(""), VariableType::VT_Int32, offsetof(CreateGameTableObject,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("Name"),							_T(""), VariableType::VT_String, offsetof(CreateGameTableObject,m_sName), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),					_T(""), VariableType::VT_Float, offsetof(CreateGameTableObject,m_fSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),						_T(""), VariableType::VT_Float, offsetof(CreateGameTableObject,m_fBigBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),							_T(""), VariableType::VT_Float, offsetof(CreateGameTableObject,m_fAnte), false, false, false)
	DECLARE_VAR(_T("RakePercent"),					_T(""), VariableType::VT_Float, offsetof(CreateGameTableObject,m_fRakePercent), false, false, false)
	DECLARE_VAR(_T("MaxRakeAmount"),				_T(""), VariableType::VT_Float, offsetof(CreateGameTableObject,m_fMaxRakeAmount), false, false, false)
	DECLARE_VAR(_T("MinBuyin"),						_T(""), VariableType::VT_Float, offsetof(CreateGameTableObject,m_fMinBuyin), false, false, false)
	DECLARE_VAR(_T("MaxBuyin"),						_T(""), VariableType::VT_Float, offsetof(CreateGameTableObject,m_fMaxBuyin), false, false, false)
	DECLARE_VAR(_T("MaxSeatCt"),					_T(""), VariableType::VT_Int8, offsetof(CreateGameTableObject,m_cMaxSeatCt), false, false, false)
	DECLARE_VAR(_T("Password"),						_T(""), VariableType::VT_String, offsetof(CreateGameTableObject,m_sPassword), false, false, false)
END_DECLARE_VAR()

CreateGameTableObject::~CreateGameTableObject(){
	}

//////////////////////////////////////////////////////
//
//	class PokerForceClientSessionToQuitPacket
//
/////////////////////////////////////////////////////

PokerForceClientSessionToQuitPacket::PokerForceClientSessionToQuitPacket(){
	m_nCode = PacketCodes::_PokerForceClientSessionToQuit;
	}

BEGIN_DECLARE_VAR(PokerForceClientSessionToQuitPacket, BasePacket)
	DECLARE_VAR(_T("PokerForceClientSessionToQuitPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("SessionId"),							_T(""), VariableType::VT_Int64, offsetof(PokerForceClientSessionToQuitPacket,m_nSessionId), false, false, false)
	DECLARE_VAR(_T("QuitAll"),								_T(""), VariableType::VT_Bool, offsetof(PokerForceClientSessionToQuitPacket,m_bQuitAll), false, false, false)
END_DECLARE_VAR()

PokerForceClientSessionToQuitPacket::~PokerForceClientSessionToQuitPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerDisableServerConnectPacket
//
/////////////////////////////////////////////////////

PokerDisableServerConnectPacket::PokerDisableServerConnectPacket(){
	m_nCode = PacketCodes::_PokerForceClientSessionToQuit;
	}

BEGIN_DECLARE_VAR(PokerDisableServerConnectPacket, BasePacket)
	DECLARE_VAR(_T("PokerDisableServerConnectPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Disable"),							_T(""), VariableType::VT_Bool, offsetof(PokerDisableServerConnectPacket,m_bDisable), false, false, false)
	DECLARE_VAR(_T("MessageDescription"),				_T(""), VariableType::VT_String, offsetof(PokerDisableServerConnectPacket,m_sMessageDescription), false, false, false)
END_DECLARE_VAR()

PokerDisableServerConnectPacket::~PokerDisableServerConnectPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerDeactivateCashTablePacket
//
/////////////////////////////////////////////////////

PokerDeactivateGameTablePacket::PokerDeactivateGameTablePacket(){
	m_nCode = PacketCodes::_PokerDeactivateGameTable;
	}

BEGIN_DECLARE_VAR(PokerDeactivateGameTablePacket, BasePacket)
	DECLARE_VAR(_T("PokerDeactivateGameTablePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),							_T(""), VariableType::VT_Int32, offsetof(PokerDeactivateGameTablePacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Deactivate"),						_T(""), VariableType::VT_Bool, offsetof(PokerDeactivateGameTablePacket,m_bDeactivate), false, false, false)
	DECLARE_VAR(_T("AllTables"),						_T(""), VariableType::VT_Bool, offsetof(PokerDeactivateGameTablePacket,m_bAllTables), false, false, false)
END_DECLARE_VAR()

PokerDeactivateGameTablePacket::~PokerDeactivateGameTablePacket(){
	}


//////////////////////////////////////////////////////
//
//	class PokerCreateTournamentPacket
//
/////////////////////////////////////////////////////
PokerCreateTournamentPacket::PokerCreateTournamentPacket(){
	ZeroInit();
	m_nCode = PacketCodes::_PokerCreateTournament;
	}

BEGIN_DECLARE_VAR(PokerCreateTournamentPacket, BasePacket)
	DECLARE_VAR(_T("PokerCreateTournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("ResponseLog"),					_T(""), VariableType::VT_String, offsetof(PokerCreateTournamentPacket,m_sResponseLog), false, false, false)
	DECLARE_VAR(_T("GameType"),						_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nGameSubType), false, false, false)
	DECLARE_VAR(_T("GameSpeedId"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nGameSpeedId), false, false, false)
	DECLARE_VAR(_T("Name"),							_T(""), VariableType::VT_String, offsetof(PokerCreateTournamentPacket,m_sName), false, false, false)
	DECLARE_VAR(_T("Description"),					_T(""), VariableType::VT_String, offsetof(PokerCreateTournamentPacket,m_sDescription), false, false, false)

	DECLARE_VAR(_T("TimeStart"),					_T(""), VariableType::VT_DateTime, offsetof(PokerCreateTournamentPacket,m_dtTimeStart), false, false, false)
	DECLARE_VAR(_T("TimeShow"),						_T(""), VariableType::VT_DateTime, offsetof(PokerCreateTournamentPacket,m_dtTimeShow), false, false, false)
	DECLARE_VAR(_T("TimeRegStart"),					_T(""), VariableType::VT_DateTime, offsetof(PokerCreateTournamentPacket,m_dtTimeRegStart), false, false, false)
	DECLARE_VAR(_T("TimeRegEnd"),					_T(""), VariableType::VT_DateTime, offsetof(PokerCreateTournamentPacket,m_dtTimeRegEnd), false, false, false)

	DECLARE_VAR(_T("MinPlayers"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nMinPlayers), false, false, false)
	DECLARE_VAR(_T("MaxPlayers"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nMaxPlayers), false, false, false)

	DECLARE_VAR(_T("ActionTimeSec"),				_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shActionTimeSec), false, false, false)
	DECLARE_VAR(_T("TBInitialSec"),					_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shTBInitialSec), false, false, false)
	DECLARE_VAR(_T("TBIncLevel"),					_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shTBIncLevel), false, false, false)
	DECLARE_VAR(_T("TBIncSec"),						_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shTBIncSec), false, false, false)
	DECLARE_VAR(_T("MaxSeatCt"),					_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shMaxSeatCt), false, false, false)
	DECLARE_VAR(_T("LevelDurationMin"),				_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shLevelDurationMin), false, false, false)
	DECLARE_VAR(_T("OpenTableBeforeStartSec"),		_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shOpenTableBeforeStartSec), false, false, false)

	DECLARE_VAR(_T("BuyinAmount"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nBuyinAmount), false, false, false)
	DECLARE_VAR(_T("BuyinVPP"),						_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nBuyinVPP), false, false, false)
	DECLARE_VAR(_T("BuyinFeeAmount"),				_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nBuyinFeeAmount), false, false, false)
	DECLARE_VAR(_T("BuyinChips"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nBuyinChips), false, false, false)

	DECLARE_VAR(_T("BreakPerLevel"),				_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shBreakPerLevel), false, false, false)
	DECLARE_VAR(_T("BreakDurationMin"),				_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shBreakDurationMin), false, false, false)

	DECLARE_VAR(_T("RebuyAmount"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nRebuyAmount), false, false, false)
	DECLARE_VAR(_T("RebuyVPP"),						_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nRebuyVPP), false, false, false)
	DECLARE_VAR(_T("RebuyFeeAmount"),				_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nRebuyFeeAmount), false, false, false)
	DECLARE_VAR(_T("RebuyChips"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nRebuyChips), false, false, false)

	DECLARE_VAR(_T("AddonAmount"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nAddonAmount), false, false, false)
	DECLARE_VAR(_T("AddonVPP"),						_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nAddonVPP), false, false, false)
	DECLARE_VAR(_T("AddonFeeAmount"),				_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nAddonFeeAmount), false, false, false)
	DECLARE_VAR(_T("AddonChips"),					_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nAddonChips), false, false, false)

	DECLARE_VAR(_T("AddonBreak"),					_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shAddonBreak), false, false, false)
	DECLARE_VAR(_T("PrizeAmount"),					_T(""), VariableType::VT_Double, offsetof(PokerCreateTournamentPacket,m_dPrizeAmount), false, false, false)
	DECLARE_VAR(_T("PrizeTicketId"),				_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nPrizeTicketId), false, false, false)
	DECLARE_VAR(_T("TicketPriceAmount"),			_T(""), VariableType::VT_Double, offsetof(PokerCreateTournamentPacket,m_dTicketPriceAmount), false, false, false)
	DECLARE_VAR(_T("GuaranteedTicketCt"),			_T(""), VariableType::VT_Int16, offsetof(PokerCreateTournamentPacket,m_shGuaranteedTicketCt), false, false, false)

	DECLARE_VAR(_T("BlindStructProfileId"),			_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nBlindStructProfileId), false, false, false)
	DECLARE_VAR(_T("PayoutProfileId"),				_T(""), VariableType::VT_Int32, offsetof(PokerCreateTournamentPacket,m_nPayoutProfileId), false, false, false)

	DECLARE_VAR(_T("RebuyCt"),						_T(""), VariableType::VT_Int8, offsetof(PokerCreateTournamentPacket,m_btRebuyCt), false, false, false)
	DECLARE_VAR(_T("AddonCt"),						_T(""), VariableType::VT_Int8, offsetof(PokerCreateTournamentPacket,m_btAddonCt), false, false, false)
	DECLARE_VAR(_T("AllowToUseVPPoints"),			_T(""), VariableType::VT_Bool, offsetof(PokerCreateTournamentPacket,m_bAllowToUseVPPoints), false, false, false)
	DECLARE_VAR(_T("RestrictSameIPs"),				_T(""), VariableType::VT_Bool, offsetof(PokerCreateTournamentPacket,m_bRestrictSameIPs), false, false, false)
END_DECLARE_VAR()

PokerCreateTournamentPacket::~PokerCreateTournamentPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerCancelTournamentPacket
//
/////////////////////////////////////////////////////
PokerCancelTournamentPacket::PokerCancelTournamentPacket(){
	m_nCode = PacketCodes::_PokerCancelTournament;
	}

BEGIN_DECLARE_VAR(PokerCancelTournamentPacket, BasePacket)
	DECLARE_VAR(_T("PokerCancelTournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),					_T(""), VariableType::VT_Int32, offsetof(PokerCancelTournamentPacket,m_nTournamentId), false, false, false)
END_DECLARE_VAR()

PokerCancelTournamentPacket::~PokerCancelTournamentPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerHideShowTournamentPacket
//
/////////////////////////////////////////////////////
PokerHideShowTournamentPacket::PokerHideShowTournamentPacket(){
	m_nCode = PacketCodes::_PokerHideShowTournament;
	}

BEGIN_DECLARE_VAR(PokerHideShowTournamentPacket, BasePacket)
	DECLARE_VAR(_T("PokerHideShowTournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),						_T(""), VariableType::VT_Int32, offsetof(PokerHideShowTournamentPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Hide"),								_T(""), VariableType::VT_Bool, offsetof(PokerHideShowTournamentPacket,m_bHide), false, false, false)
END_DECLARE_VAR()

PokerHideShowTournamentPacket::~PokerHideShowTournamentPacket(){
	}

//////////////////////////////////////////////////////
//
//	class PokerRegisterUserInTournamentPacket
//
/////////////////////////////////////////////////////
PokerRegisterUserInTournamentPacket::PokerRegisterUserInTournamentPacket(){
	m_nCode = PacketCodes::_PokerRegisterUserInTournament;
	}

BEGIN_DECLARE_VAR(PokerRegisterUserInTournamentPacket, BasePacket)
	DECLARE_VAR(_T("PokerRegisterUserInTournamentPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),							_T(""), VariableType::VT_Int32, offsetof(PokerRegisterUserInTournamentPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Register"),								_T(""), VariableType::VT_Bool, offsetof(PokerRegisterUserInTournamentPacket,m_bRegister), false, false, false)
	DECLARE_VAR(_T("UserId"),								_T(""), VariableType::VT_Int32, offsetof(PokerRegisterUserInTournamentPacket,m_nUserId), false, false, false)
END_DECLARE_VAR()

PokerRegisterUserInTournamentPacket::~PokerRegisterUserInTournamentPacket(){
	}
