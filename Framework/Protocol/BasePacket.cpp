#include "stdafx.h"
#include "BasePacket.h"
#include "_PokerDefines.h"
#include "../Utility/GrowableMemory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////
//
//	class BasePacket
//
/////////////////////////////////////////////////////
BasePacket::BasePacket(){
	m_nSessionId	= 0;
//	m_bIsRequest	= false;
	m_nPacketSize	= 0;
	m_nErrorCode	= 0;
	}

BEGIN_DECLARE_VAR(BasePacket, Serializable)
	DECLARE_VAR(_T("BasePacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Code"),			_T(""), VariableType::VT_Int16, offsetof(BasePacket,m_nCode), false, false, false)
	DECLARE_VAR(_T("PacketSize"),	_T(""), VariableType::VT_Int32, offsetof(BasePacket,m_nPacketSize), false, false, false)
	DECLARE_VAR(_T("SessionId"),	_T(""), VariableType::VT_Int64, offsetof(BasePacket,m_nSessionId), false, false, false)
//	DECLARE_VAR(_T("IsRequest"),	_T(""), VariableType::VT_Bool, offsetof(BasePacket,m_bIsRequest), false, false, false)
	DECLARE_VAR(_T("ErrorStr"),		_T(""), VariableType::VT_String, offsetof(BasePacket,m_sError), false, false, false)
	DECLARE_VAR(_T("ErrorCode"),	_T(""), VariableType::VT_Int16, offsetof(BasePacket,m_nErrorCode), false, false, false)
END_DECLARE_VAR()

BasePacket::~BasePacket(){
	}

void
BasePacket::ZeroPacketVariables(){
	short shCode = m_nCode;
	ZeroInit();
	m_nCode = shCode;
	}

//////////////////////////////////////////////////////
//
//	class HandShakePacket
//
/////////////////////////////////////////////////////
HandShakePacket::HandShakePacket(){
	m_nCode			= PacketCodes::_HandShake;
	m_nProtocolVer	= PROTOCOL_VER;
	}

BEGIN_DECLARE_VAR(HandShakePacket, BasePacket)
	DECLARE_VAR(_T("HandShakePacket"),		_T(""),	VariableType::VT_None, 0, false, false, false)
	DECLARE_VAR(_T("ProtocolVer"),			_T(""),	VariableType::VT_Int32, offsetof(HandShakePacket,m_nProtocolVer), false, false, false)
	DECLARE_VAR(_T("ClientVer"),			_T(""),	VariableType::VT_Int32, offsetof(HandShakePacket,m_nClientVer), false, false, false)
	DECLARE_VAR(_T("PacketCompressionZ"),	_T(""),	VariableType::VT_Bool, offsetof(HandShakePacket,m_bPacketCompressionZ), false, false, false)
	DECLARE_VAR(_T("PCName"),				_T(""),	VariableType::VT_String, offsetof(HandShakePacket,m_sPCName), false, false, false)
	DECLARE_VAR(_T("OS"),					_T(""),	VariableType::VT_String, offsetof(HandShakePacket,m_sOS), false, false, false)
	DECLARE_VAR(_T("Processor"),			_T(""),	VariableType::VT_String, offsetof(HandShakePacket,m_sProcessor), false, false, false)
END_DECLARE_VAR()

HandShakePacket::~HandShakePacket(){
	}

//////////////////////////////////////////////////////
//
//	class AcquireTableAccessPacket
//
/////////////////////////////////////////////////////
AcquireTableAccessPacket::AcquireTableAccessPacket(){
	m_nCode	= PacketCodes::_AcquireTableAccess;
	}

BEGIN_DECLARE_VAR(AcquireTableAccessPacket, BasePacket)
	DECLARE_VAR(_T("AcquireTableAccessPacket"),	_T(""),	VariableType::VT_None, 0, false, false, false)
	DECLARE_VAR(_T("SessionIdAcquire"),			_T(""),	VariableType::VT_Int64, offsetof(AcquireTableAccessPacket,m_nSessionIdAcquire), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T(""),	VariableType::VT_Int32, offsetof(AcquireTableAccessPacket,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""),	VariableType::VT_Int32, offsetof(AcquireTableAccessPacket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("UserId"),					_T(""),	VariableType::VT_Int32, offsetof(AcquireTableAccessPacket,m_nUserId), false, false, false)
	DECLARE_VAR(_T("ChatIsBlocked"),			_T(""),	VariableType::VT_Bool, offsetof(AcquireTableAccessPacket,m_bChatIsBlocked), false, false, false)
	DECLARE_VAR(_T("AvatarIsBlocked"),			_T(""),	VariableType::VT_Bool, offsetof(AcquireTableAccessPacket,m_bAvatarIsBlocked), false, false, false)
END_DECLARE_VAR()

AcquireTableAccessPacket::~AcquireTableAccessPacket(){
	}

//////////////////////////////////////////////////////
//
//	class AvatarInfo
//
/////////////////////////////////////////////////////
AvatarInfo::AvatarInfo(){
	}

BEGIN_DECLARE_VAR(AvatarInfo, Serializable)
	DECLARE_VAR(_T("AvatarInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("AvatarId"),			_T(""), VariableType::VT_Int32, offsetof(AvatarInfo,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("AvatarName"),		_T(""), VariableType::VT_String, offsetof(AvatarInfo,m_sAvatarName), false, false, false)
	DECLARE_VAR(_T("AvatarFileName"),	_T(""), VariableType::VT_String, offsetof(AvatarInfo,m_sAvatarFileName), false, false, false)
END_DECLARE_VAR()

AvatarInfo::~AvatarInfo(){
	}

//////////////////////////////////////////////////////
//
//	class OpenedTableInfo
//
/////////////////////////////////////////////////////
OpenedTableInfo::OpenedTableInfo(){
	}

BEGIN_DECLARE_VAR(OpenedTableInfo, Serializable)
	DECLARE_VAR(_T("OpenedTableInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),			_T(""), VariableType::VT_Int32, offsetof(OpenedTableInfo,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),		_T(""), VariableType::VT_Int32, offsetof(OpenedTableInfo,m_nTournamentId), false, false, false)
END_DECLARE_VAR()

OpenedTableInfo::~OpenedTableInfo(){
	}

//////////////////////////////////////////////////////
//
//	class UserTournament
//
/////////////////////////////////////////////////////
UserTournament::UserTournament(){
	}

BEGIN_DECLARE_VAR(UserTournament, Serializable)
	DECLARE_VAR(_T("UserTournament"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),		_T(""), VariableType::VT_Int32, offsetof(UserTournament,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Name"),				_T(""), VariableType::VT_String, offsetof(UserTournament,m_sName), false, false, false)
	DECLARE_VAR(_T("StartDTime"),		_T(""), VariableType::VT_DateTime, offsetof(UserTournament,m_dtStart), false, false, false)
	DECLARE_VAR(_T("GameType"),			_T(""), VariableType::VT_Int8, offsetof(UserTournament,m_cGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),		_T(""), VariableType::VT_Int16, offsetof(UserTournament,m_shGameSubType), false, false, false)
	DECLARE_VAR(_T("BuyinAmount"),		_T(""), VariableType::VT_Int32, offsetof(UserTournament,m_nBuyinAmount), false, false, false)
	DECLARE_VAR(_T("FeeAmount"),		_T(""), VariableType::VT_Int32, offsetof(UserTournament,m_nFeeAmount), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),		_T(""), VariableType::VT_Int8, offsetof(UserTournament,m_cCurrencyId), false, false, false)
	DECLARE_VAR(_T("Status"),			_T(""), VariableType::VT_Int8, offsetof(UserTournament,m_status), false, false, false)
END_DECLARE_VAR()

UserTournament::~UserTournament(){
	}

//////////////////////////////////////////////////////
//
//	class LoginPacket
//
/////////////////////////////////////////////////////
LoginPacket::LoginPacket(){
	m_nCode				= PacketCodes::_Login;
	m_dVPPoints			= 0.0;
	m_btRank			= 0;
	m_nUserId			= 0;
	m_nPlayBalance		= 0;
	m_nBalance			= 0;
	m_bChatIsBlocked	= false;
	m_bAvatarIsBlocked	= false;
	}

BEGIN_DECLARE_VAR(LoginPacket, BasePacket)
	DECLARE_VAR(_T("LoginPacket"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserName"),			_T(""), VariableType::VT_String, offsetof(LoginPacket,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Password"),			_T(""), VariableType::VT_String, offsetof(LoginPacket,m_sPassword), false, false, false)
	DECLARE_VAR(_T("WebSessionId"),		_T(""), VariableType::VT_String, offsetof(LoginPacket,m_sWebSessionId), false, false, false)
	DECLARE_VAR(_T("Balance"),			_T(""), VariableType::VT_Chips, offsetof(LoginPacket,m_nBalance), false, false, false)
	DECLARE_VAR(_T("PlayBalance"),		_T(""), VariableType::VT_Chips, offsetof(LoginPacket,m_nPlayBalance), false, false, false)
	DECLARE_VAR(_T("VPPoints"),			_T(""), VariableType::VT_Double, offsetof(LoginPacket,m_dVPPoints), false, false, false)
	DECLARE_VAR(_T("Rank"),				_T(""), VariableType::VT_Int8, offsetof(LoginPacket,m_btRank), false, false, false)
	DECLARE_VAR(_T("UserId"),			_T(""), VariableType::VT_Int32, offsetof(LoginPacket,m_nUserId), false, false, false)
	DECLARE_VAR(_T("AvatarId"),			_T(""), VariableType::VT_Int32, offsetof(LoginPacket,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("OpenedTables"),		_T(""), VariableType::VT_None, offsetof(LoginPacket,m_openedTables), false, true, true)
	DECLARE_VAR(_T("RegTournaments"),	_T(""), VariableType::VT_None, offsetof(LoginPacket,m_listRegTournaments), false, true, true)
	DECLARE_VAR(_T("Avatars"),			_T(""), VariableType::VT_None, offsetof(LoginPacket,m_listAvatars), false, true, true)
	DECLARE_VAR(_T("SessionRest"),		_T(""), VariableType::VT_Bool, offsetof(LoginPacket,m_bPrevSessionRestored), false, false, false)	
	DECLARE_VAR(_T("ChatIsBlocked"),	_T(""), VariableType::VT_Bool, offsetof(LoginPacket,m_bChatIsBlocked), false, false, false)	
	DECLARE_VAR(_T("AvatarIsBlocked"),	_T(""), VariableType::VT_Bool, offsetof(LoginPacket,m_bAvatarIsBlocked), false, false, false)	
END_DECLARE_VAR()

LoginPacket::~LoginPacket(){
	}

//////////////////////////////////////////////////////
//
//	class LogoutPacket
//
/////////////////////////////////////////////////////
LogoutPacket::LogoutPacket(){
	m_nCode = PacketCodes::_Logout;
	}

BEGIN_DECLARE_VAR(LogoutPacket, BasePacket)
	DECLARE_VAR(_T("LogoutPacket"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("SessionIdNew"),		_T(""), VariableType::VT_Int64, offsetof(LogoutPacket,m_nSessionIdNew), false, false, false)
END_DECLARE_VAR()

LogoutPacket::~LogoutPacket(){
	}

//////////////////////////////////////////////////////
//
//	class RegisterUserPacket
//
/////////////////////////////////////////////////////
RegisterUserPacket::RegisterUserPacket(){
	m_nCode = PacketCodes::_RegisterUser;
	}

BEGIN_DECLARE_VAR(RegisterUserPacket, BasePacket)
	DECLARE_VAR(_T("RegisterUserPacket"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserName"),				_T(""), VariableType::VT_String, offsetof(RegisterUserPacket,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Pwd"),					_T(""), VariableType::VT_String, offsetof(RegisterUserPacket,m_sPwd), false, false, false)
	DECLARE_VAR(_T("PwdRepeat"),			_T(""), VariableType::VT_String, offsetof(RegisterUserPacket,m_sPwdRepeat), false, false, false)
	DECLARE_VAR(_T("EmailAddress"),			_T(""), VariableType::VT_String, offsetof(RegisterUserPacket,m_sEmailAddress), false, false, false)
	DECLARE_VAR(_T("PhoneNumber"),			_T(""), VariableType::VT_String, offsetof(RegisterUserPacket,m_sPhoneNumber), false, false, false)
	DECLARE_VAR(_T("NickName"),				_T(""), VariableType::VT_String, offsetof(RegisterUserPacket,m_sNickName), false, false, false)
END_DECLARE_VAR()

RegisterUserPacket::~RegisterUserPacket(){
	}