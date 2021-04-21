#include "stdafx.h"
#include "DataProcedures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//==============================================
//
//	procedure: CreateUserProc
//
//==============================================

CreateUserProc::CreateUserProc(){
	} 

BEGIN_DECLARE_VAR(CreateUserProc, Serializable)
	DECLARE_VAR(_T("CreateUserProc"),	_T("create_user_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("RetUserId"),		_T("RET"),				VariableType::VT_Int32, offsetof(CreateUserProc,m_nRetUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),			_T("IN"),				VariableType::VT_String, offsetof(CreateUserProc,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Password"),			_T("IN"),				VariableType::VT_String, offsetof(CreateUserProc,m_sPassword), false, false, false)
	DECLARE_VAR(_T("EMail"),			_T("IN"),				VariableType::VT_String, offsetof(CreateUserProc,m_sEMail), false, false, false)
	DECLARE_VAR(_T("FirstName"),		_T("IN"),				VariableType::VT_String, offsetof(CreateUserProc,m_sFirstName), false, false, false)
	DECLARE_VAR(_T("LastName"),			_T("IN"),				VariableType::VT_String, offsetof(CreateUserProc,m_sLastName), false, false, false)
	DECLARE_VAR(_T("Phone"),			_T("IN"),				VariableType::VT_String, offsetof(CreateUserProc,m_sPhone), false, false, false)
	DECLARE_VAR(_T("UserId"),			_T("IN"),				VariableType::VT_Int32, offsetof(CreateUserProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("PlayAmount"),		_T("IN"),				VariableType::VT_Double, offsetof(CreateUserProc,m_dPlayAmount), false, false, false)
END_DECLARE_VAR()

CreateUserProc::~CreateUserProc(){
	ZeroInit();
	}

//==============================================
//
//	procedure: AuthenticateUserProc
//
//==============================================

AuthenticateUserProc::AuthenticateUserProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(AuthenticateUserProc, Serializable)
	DECLARE_VAR(_T("AuthenticateUserProc"),	_T("authenticate_user_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),				_T("RET"),						VariableType::VT_Int32, offsetof(AuthenticateUserProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),				_T("IN"),						VariableType::VT_String, offsetof(AuthenticateUserProc,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Password"),				_T("IN"),						VariableType::VT_String, offsetof(AuthenticateUserProc,m_sPassword), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T("OUT"),						VariableType::VT_Double, offsetof(AuthenticateUserProc,m_dBalance), false, false, false)
	DECLARE_VAR(_T("PlayBalance"),			_T("OUT"),						VariableType::VT_Double, offsetof(AuthenticateUserProc,m_dPlayBalance), false, false, false)
	DECLARE_VAR(_T("VPPoints"),				_T("OUT"),						VariableType::VT_Double, offsetof(AuthenticateUserProc,m_dVPPoints), false, false, false)
	DECLARE_VAR(_T("IsBlocked"),			_T("OUT"),						VariableType::VT_Bool, offsetof(AuthenticateUserProc,m_bIsBlocked), false, false, false)
	DECLARE_VAR(_T("AvatarId"),				_T("OUT"),						VariableType::VT_Int32, offsetof(AuthenticateUserProc,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("PermissionId"),			_T("OUT"),						VariableType::VT_Int32, offsetof(AuthenticateUserProc,m_nPermissionId), false, false, false)
	DECLARE_VAR(_T("UserLevelId"),			_T("OUT"),						VariableType::VT_Int32, offsetof(AuthenticateUserProc,m_nUserLevelId), false, false, false)
	DECLARE_VAR(_T("UserExists"),			_T("OUT"),						VariableType::VT_Bool, offsetof(AuthenticateUserProc,m_bUserExists), false, false, false)
	DECLARE_VAR(_T("ChatBlockStatus"),		_T("OUT"),						VariableType::VT_Bool, offsetof(AuthenticateUserProc,m_bChatBlockStatus), false, false, false)
	DECLARE_VAR(_T("AvatarStatus"),			_T("OUT"),						VariableType::VT_Bool, offsetof(AuthenticateUserProc,m_bAvatarStatus), false, false, false)
END_DECLARE_VAR()

AuthenticateUserProc::~AuthenticateUserProc(){
	}

//==============================================
//
//	procedure: AuthenticateUserBySessionIdProc
//
//==============================================

AuthenticateUserBySessionIdProc::AuthenticateUserBySessionIdProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(AuthenticateUserBySessionIdProc, Serializable)
	DECLARE_VAR(_T("AuthenticateUserBySessionIdProc"),	_T("authenticate_user_by_sessionId_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),							_T("RET"),				VariableType::VT_Int32, offsetof(AuthenticateUserBySessionIdProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("SessionId"),						_T("IN"),				VariableType::VT_String, offsetof(AuthenticateUserBySessionIdProc,m_sSessionId), false, false, false)
	DECLARE_VAR(_T("UserName"),							_T("OUT"),				VariableType::VT_String, offsetof(AuthenticateUserBySessionIdProc,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Balance"),							_T("OUT"),				VariableType::VT_Double, offsetof(AuthenticateUserBySessionIdProc,m_dBalance), false, false, false)
	DECLARE_VAR(_T("PlayBalance"),						_T("OUT"),				VariableType::VT_Double, offsetof(AuthenticateUserBySessionIdProc,m_dPlayBalance), false, false, false)
	DECLARE_VAR(_T("VPPoints"),							_T("OUT"),				VariableType::VT_Double, offsetof(AuthenticateUserBySessionIdProc,m_dVPPoints), false, false, false)
	DECLARE_VAR(_T("IsBlocked"),						_T("OUT"),				VariableType::VT_Bool, offsetof(AuthenticateUserBySessionIdProc,m_bIsBlocked), false, false, false)
	DECLARE_VAR(_T("AvatarId"),							_T("OUT"),				VariableType::VT_Int32, offsetof(AuthenticateUserBySessionIdProc,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("PermissionId"),						_T("OUT"),				VariableType::VT_Int32, offsetof(AuthenticateUserBySessionIdProc,m_nPermissionId), false, false, false)
	DECLARE_VAR(_T("UserLevelId"),						_T("OUT"),				VariableType::VT_Int32, offsetof(AuthenticateUserBySessionIdProc,m_nUserLevelId), false, false, false)
	DECLARE_VAR(_T("SessionIsAlreadyActive"),			_T("OUT"),				VariableType::VT_Bool, offsetof(AuthenticateUserBySessionIdProc,m_bSessionIsAlreadyActive), false, false, false)
	DECLARE_VAR(_T("ChatBlockStatus"),					_T("OUT"),				VariableType::VT_Bool, offsetof(AuthenticateUserBySessionIdProc,m_bChatBlockStatus), false, false, false)
	DECLARE_VAR(_T("AvatarStatus"),						_T("OUT"),				VariableType::VT_Bool, offsetof(AuthenticateUserBySessionIdProc,m_bAvatarStatus), false, false, false)
END_DECLARE_VAR()

AuthenticateUserBySessionIdProc::~AuthenticateUserBySessionIdProc(){
	}

//==============================================
//
//	procedure: DeauthenticateUserSessionProc
//
//==============================================

DeauthenticateUserSessionProc::DeauthenticateUserSessionProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(DeauthenticateUserSessionProc, Serializable)
	DECLARE_VAR(_T("DeauthenticateUserSessionProc"),	_T("deauthenticate_user_session"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),							_T("RET"),							VariableType::VT_Int32, offsetof(DeauthenticateUserSessionProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),							_T("IN"),							VariableType::VT_Int32, offsetof(DeauthenticateUserSessionProc,m_nUserId), false, false, false)
END_DECLARE_VAR()

DeauthenticateUserSessionProc::~DeauthenticateUserSessionProc(){
	}

//==============================================
//
//	procedure: CreateUserSessionProc
//
//==============================================

CreateUserSessionProc::CreateUserSessionProc(){
	ZeroInit();
}

BEGIN_DECLARE_VAR(CreateUserSessionProc, Serializable)
	DECLARE_VAR(_T("CreateUserSessionProc"), _T("create_user_session"), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),			_T("RET"), VariableType::VT_Int32, offsetof(CreateUserSessionProc, m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),			_T("IN"), VariableType::VT_Int32, offsetof(CreateUserSessionProc, m_nUserId), false, false, false)
	DECLARE_VAR(_T("SessionId"),		_T("OUT"), VariableType::VT_String, offsetof(CreateUserSessionProc, m_sSessionId), false, false, false)
	DECLARE_VAR(_T("IsAlreadyActive"),	_T("OUT"), VariableType::VT_Bool, offsetof(CreateUserSessionProc, m_bSessionIsAlreadyActive), false, false, false)
END_DECLARE_VAR()

CreateUserSessionProc::~CreateUserSessionProc(){
	}

//==============================================
//
//	procedure: StartHandProc
//
//==============================================

StartHandProc::StartHandProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(StartHandProc, Serializable)
	DECLARE_VAR(_T("StartHandProc"),		_T("start_hand_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("HandId"),				_T("RET"),				VariableType::VT_Int64, offsetof(StartHandProc,m_nHandId), false, false, false)
	DECLARE_VAR(_T("TableId"),				_T("IN"),				VariableType::VT_Int32, offsetof(StartHandProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),			_T("IN"),				VariableType::VT_Int32, offsetof(StartHandProc,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("SeatCt"),				_T("IN"),				VariableType::VT_Int32, offsetof(StartHandProc,m_nSeatCt), false, false, false)
	DECLARE_VAR(_T("BigBlindAmount"),		_T("IN"),				VariableType::VT_Double, offsetof(StartHandProc,m_dBigBlindAmount), false, false, false)
	DECLARE_VAR(_T("SmallBlindAmount"),		_T("IN"),				VariableType::VT_Double, offsetof(StartHandProc,m_dSmallBlindAmount), false, false, false)
	DECLARE_VAR(_T("AnteAmount"),			_T("IN"),				VariableType::VT_Double, offsetof(StartHandProc,m_dAnteAmount), false, false, false)
	DECLARE_VAR(_T("RakePercent"),			_T("IN"),				VariableType::VT_Double, offsetof(StartHandProc,m_dRakePercent), false, false, false)
END_DECLARE_VAR()

StartHandProc::~StartHandProc(){
	}

//==============================================
//
//	procedure: FinishHandProc
//
//==============================================

FinishHandProc::FinishHandProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(FinishHandProc, Serializable)
	DECLARE_VAR(_T("FinishHandProc"),	_T("finish_hand_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("FinishStatus"),		_T("RET"),				VariableType::VT_Bool, offsetof(FinishHandProc,m_bFinishStatus), false, false, false)
	DECLARE_VAR(_T("HandId"),			_T("IN"),				VariableType::VT_Int64, offsetof(FinishHandProc,m_nHandId), false, false, false)
	DECLARE_VAR(_T("HandData"),			_T("IN"),				VariableType::VT_Binary, offsetof(FinishHandProc,m_binHandData), false, false, false)
	DECLARE_VAR(_T("TableId"),			_T("IN"),				VariableType::VT_Int32, offsetof(FinishHandProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),		_T("IN"),				VariableType::VT_Int32, offsetof(FinishHandProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("PlayersInfo"),		_T("IN"),				VariableType::VT_String, offsetof(FinishHandProc,m_sPlayersInfo), false, false, false)
	DECLARE_VAR(_T("TableCards"),		_T("IN"),				VariableType::VT_String, offsetof(FinishHandProc,m_sTableCards), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),	_T("IN"),				VariableType::VT_Double, offsetof(FinishHandProc,m_dTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),		_T("IN"),				VariableType::VT_Double, offsetof(FinishHandProc,m_dRakeAmount), false, false, false)
END_DECLARE_VAR()

FinishHandProc::~FinishHandProc(){
	}

//==============================================
//
//	procedure: FinishHandPlaymoneyProc
//
//==============================================

FinishHandPlaymoneyProc::FinishHandPlaymoneyProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(FinishHandPlaymoneyProc, Serializable)
	DECLARE_VAR(_T("FinishHandPlaymoneyProc"),	_T("finish_hand_playmoney_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("FinishStatus"),				_T("RET"),				VariableType::VT_Bool, offsetof(FinishHandPlaymoneyProc,m_bFinishStatus), false, false, false)
	DECLARE_VAR(_T("HandId"),					_T("IN"),				VariableType::VT_Int64, offsetof(FinishHandPlaymoneyProc,m_nHandId), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T("IN"),				VariableType::VT_Int32, offsetof(FinishHandPlaymoneyProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("PlayersInfo"),				_T("IN"),				VariableType::VT_String, offsetof(FinishHandPlaymoneyProc,m_sPlayersInfo), false, false, false)
	DECLARE_VAR(_T("TableCards"),				_T("IN"),				VariableType::VT_String, offsetof(FinishHandProc,m_sTableCards), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),			_T("IN"),				VariableType::VT_Double, offsetof(FinishHandPlaymoneyProc,m_dTotalPotAmount), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),				_T("IN"),				VariableType::VT_Double, offsetof(FinishHandProc,m_dRakeAmount), false, false, false)
END_DECLARE_VAR()

FinishHandPlaymoneyProc::~FinishHandPlaymoneyProc(){
	}

//==============================================
//
//	procedure: SeatUserProc
//
//==============================================

SeatUserProc::SeatUserProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(SeatUserProc, Serializable)
	DECLARE_VAR(_T("SeatUserProc"),			_T("seat_user_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),				VariableType::VT_Int32, offsetof(SeatUserProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(SeatUserProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),				_T("IN"),				VariableType::VT_Int32, offsetof(SeatUserProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Amount"),				_T("IN"),				VariableType::VT_Double, offsetof(SeatUserProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("IN"),				VariableType::VT_Int32, offsetof(SeatUserProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),			_T("OUT"),				VariableType::VT_Double, offsetof(SeatUserProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

SeatUserProc::~SeatUserProc(){
	}

//==============================================
//
//	procedure: SeatUserPlaymoneyProc
//
//==============================================

SeatUserPlaymoneyProc::SeatUserPlaymoneyProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(SeatUserPlaymoneyProc, Serializable)
	DECLARE_VAR(_T("SeatUserPlaymoneyProc"),_T("seat_user_playmoney_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),						VariableType::VT_Int32, offsetof(SeatUserPlaymoneyProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),						VariableType::VT_Int32, offsetof(SeatUserPlaymoneyProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),				_T("IN"),						VariableType::VT_Int32, offsetof(SeatUserPlaymoneyProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Amount"),				_T("IN"),						VariableType::VT_Double, offsetof(SeatUserPlaymoneyProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),			_T("OUT"),						VariableType::VT_Double, offsetof(SeatUserPlaymoneyProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

SeatUserPlaymoneyProc::~SeatUserPlaymoneyProc(){
	}

//==============================================
//
//	procedure: UnseatUserProc
//
//==============================================

UnseatUserProc::UnseatUserProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(UnseatUserProc, Serializable)
	DECLARE_VAR(_T("UnseatUserProc"),		_T("unseat_user_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),				VariableType::VT_Int32, offsetof(UnseatUserProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(UnseatUserProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),				_T("IN"),				VariableType::VT_Int32, offsetof(UnseatUserProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Amount"),				_T("IN"),				VariableType::VT_Double, offsetof(UnseatUserProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("IN"),				VariableType::VT_Int32, offsetof(UnseatUserProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),			_T("OUT"),				VariableType::VT_Double, offsetof(UnseatUserProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

UnseatUserProc::~UnseatUserProc(){
	}

//==============================================
//
//	procedure: UnseatUserPleymoneyProc
//
//==============================================

UnseatUserPleymoneyProc::UnseatUserPleymoneyProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(UnseatUserPleymoneyProc, Serializable)
	DECLARE_VAR(_T("UnseatUserPleymoneyProc"),	_T("unseat_user_playmoney_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),					_T("RET"),							VariableType::VT_Int32, offsetof(UnseatUserPleymoneyProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),					_T("IN"),							VariableType::VT_Int32, offsetof(UnseatUserPleymoneyProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T("IN"),							VariableType::VT_Int32, offsetof(UnseatUserPleymoneyProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Amount"),					_T("IN"),							VariableType::VT_Double, offsetof(UnseatUserPleymoneyProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),				_T("OUT"),							VariableType::VT_Double, offsetof(UnseatUserPleymoneyProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

UnseatUserPleymoneyProc::~UnseatUserPleymoneyProc(){
	}

//==============================================
//
//	procedure: UserAddChipsProc
//
//==============================================

UserAddChipsProc::UserAddChipsProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(UserAddChipsProc, Serializable)
	DECLARE_VAR(_T("UserAddChipsProc"),		_T("user_add_chips_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),				VariableType::VT_Int32, offsetof(UserAddChipsProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(UserAddChipsProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),				_T("IN"),				VariableType::VT_Int32, offsetof(UserAddChipsProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Amount"),				_T("IN"),				VariableType::VT_Double, offsetof(UserAddChipsProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("IN"),				VariableType::VT_Int32, offsetof(UserAddChipsProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),			_T("OUT"),				VariableType::VT_Double, offsetof(UserAddChipsProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

UserAddChipsProc::~UserAddChipsProc(){
	}

//==============================================
//
//	procedure: UserAddChipsPlaymoneyProc
//
//==============================================

UserAddChipsPlaymoneyProc::UserAddChipsPlaymoneyProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(UserAddChipsPlaymoneyProc, Serializable)
	DECLARE_VAR(_T("UserAddChipsPlaymoneyProc"),	_T("user_add_chips_playmoney_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),						_T("RET"),								VariableType::VT_Int32, offsetof(UserAddChipsPlaymoneyProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),						_T("IN"),								VariableType::VT_Int32, offsetof(UserAddChipsPlaymoneyProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),						_T("IN"),								VariableType::VT_Int32, offsetof(UserAddChipsPlaymoneyProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("Amount"),						_T("IN"),								VariableType::VT_Double, offsetof(UserAddChipsPlaymoneyProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),					_T("OUT"),								VariableType::VT_Double, offsetof(UserAddChipsPlaymoneyProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

UserAddChipsPlaymoneyProc::~UserAddChipsPlaymoneyProc(){
	}


//==============================================
//
//	procedure: TransferFromMainBalanceProc
//
//==============================================

TransferFromMainBalanceProc::TransferFromMainBalanceProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(TransferFromMainBalanceProc, Serializable)
	DECLARE_VAR(_T("TransferFromMainBalanceProc"),	_T("transfer_from_main_balance_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),						_T("RET"),	VariableType::VT_Int32, offsetof(TransferFromMainBalanceProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),						_T("IN"),	VariableType::VT_Int32, offsetof(TransferFromMainBalanceProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("Amount"),						_T("IN"),	VariableType::VT_Double, offsetof(TransferFromMainBalanceProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),					_T("IN"),	VariableType::VT_Int32, offsetof(TransferFromMainBalanceProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("MainBalanceEntryId"),			_T("IN"),	VariableType::VT_Int32, offsetof(TransferFromMainBalanceProc,m_nMainBalanceEntryId), false, false, false)
	DECLARE_VAR(_T("MainBalance"),					_T("IN"),	VariableType::VT_Double, offsetof(TransferFromMainBalanceProc,m_dMainBalance), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),					_T("OUT"),	VariableType::VT_Double, offsetof(TransferFromMainBalanceProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

TransferFromMainBalanceProc::~TransferFromMainBalanceProc(){
	}

//==============================================
//
//	procedure: TransferToMainBalanceProc
//
//==============================================

TransferToMainBalanceProc::TransferToMainBalanceProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(TransferToMainBalanceProc, Serializable)
	DECLARE_VAR(_T("TransferToMainBalanceProc"),	_T("transfer_to_main_balance_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),						_T("RET"),	VariableType::VT_Int32, offsetof(TransferToMainBalanceProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),						_T("IN"),	VariableType::VT_Int32, offsetof(TransferToMainBalanceProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("Amount"),						_T("IN"),	VariableType::VT_Double, offsetof(TransferToMainBalanceProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),					_T("IN"),	VariableType::VT_Int32, offsetof(TransferToMainBalanceProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("MainBalanceEntryId"),			_T("IN"),	VariableType::VT_Int32, offsetof(TransferToMainBalanceProc,m_nMainBalanceEntryId), false, false, false)
	DECLARE_VAR(_T("MainBalance"),					_T("IN"),	VariableType::VT_Double, offsetof(TransferToMainBalanceProc,m_dMainBalance), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),					_T("OUT"),	VariableType::VT_Double, offsetof(TransferToMainBalanceProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

TransferToMainBalanceProc::~TransferToMainBalanceProc(){
	}

//==============================================
//
//	procedure: GetUserBalanceProc
//
//==============================================

GetUserBalanceProc::GetUserBalanceProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(GetUserBalanceProc, Serializable)
	DECLARE_VAR(_T("GetUserBalanceProc"),	_T("get_user_balance_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(GetUserBalanceProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("IN"),				VariableType::VT_Int32, offsetof(GetUserBalanceProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T("OUT"),				VariableType::VT_Double, offsetof(GetUserBalanceProc,m_dBalance), false, false, false)
END_DECLARE_VAR()

GetUserBalanceProc::~GetUserBalanceProc(){
	}

//==============================================
//
//	procedure: SetUserPlayBalanceProc
//
//==============================================

SetUserPlayBalanceProc::SetUserPlayBalanceProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(SetUserPlayBalanceProc, Serializable)
	DECLARE_VAR(_T("SetUserPlayBalanceProc"),	_T("set_user_playbalance_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),					_T("RET"),				VariableType::VT_Int32, offsetof(SetUserPlayBalanceProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),					_T("IN"),				VariableType::VT_Int32, offsetof(SetUserPlayBalanceProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("Balance"),					_T("IN"),				VariableType::VT_Double, offsetof(SetUserPlayBalanceProc,m_dBalance), false, false, false)
END_DECLARE_VAR()

SetUserPlayBalanceProc::~SetUserPlayBalanceProc(){
	}

//==============================================
//
//	procedure: RetrieveMinSeatBuyInProc
//
//==============================================

RetrieveMinSeatBuyInProc::RetrieveMinSeatBuyInProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(RetrieveMinSeatBuyInProc, Serializable)
	DECLARE_VAR(_T("RetrieveMinSeatBuyInProc"),	_T("retrieve_seat_min_amount_and_balance"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),				VariableType::VT_Int32, offsetof(RetrieveMinSeatBuyInProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(RetrieveMinSeatBuyInProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),				_T("IN"),				VariableType::VT_Int32, offsetof(RetrieveMinSeatBuyInProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("IN"),				VariableType::VT_Int32, offsetof(RetrieveMinSeatBuyInProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("AmountRet"),			_T("OUT"),				VariableType::VT_Double, offsetof(RetrieveMinSeatBuyInProc,m_dAmount), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T("OUT"),				VariableType::VT_Double, offsetof(RetrieveMinSeatBuyInProc,m_dBalance), false, false, false)
END_DECLARE_VAR()

RetrieveMinSeatBuyInProc::~RetrieveMinSeatBuyInProc(){
	}

//==============================================
//
//	procedure: SetUserAvatarProc
//
//==============================================

SetUserAvatarProc::SetUserAvatarProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(SetUserAvatarProc, Serializable)
	DECLARE_VAR(_T("SetUserAvatarProc"),	_T("set_user_avatar"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),				VariableType::VT_Int32, offsetof(SetUserAvatarProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(SetUserAvatarProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("AvatarId"),				_T("IN"),				VariableType::VT_Int32, offsetof(SetUserAvatarProc,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("IsPrivateAvatar"),		_T("IN"),				VariableType::VT_Bool, offsetof(SetUserAvatarProc,m_bIsPrivateAvatar), false, false, false)
	DECLARE_VAR(_T("AvatarFileName"),		_T("IN"),				VariableType::VT_String, offsetof(SetUserAvatarProc,m_sAvatarFileName), false, false, false)
	DECLARE_VAR(_T("AvatarIdNew"),			_T("OUT"),				VariableType::VT_Int32, offsetof(SetUserAvatarProc,m_nAvatarIdNew), false, false, false)
END_DECLARE_VAR()

SetUserAvatarProc::~SetUserAvatarProc(){
	}

//==============================================
//
//	procedure: UnseatUsersProc
//
//==============================================

UnseatUsersProc::UnseatUsersProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(UnseatUsersProc, Serializable)
	DECLARE_VAR(_T("UnseatUsersProc"),	_T("unseat_users_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),			_T("RET"),					VariableType::VT_Int32, offsetof(UnseatUsersProc,m_nSuccess), false, false, false)
END_DECLARE_VAR()

UnseatUsersProc::~UnseatUsersProc(){
	}

//==============================================
//
//	procedure: GetTableStatProc
//
//==============================================

GetTableStatProc::GetTableStatProc(){
	ZeroInit();
	} 

BEGIN_DECLARE_VAR(GetTableStatProc, Serializable)
	DECLARE_VAR(_T("GetTableStatProc"),	_T("get_table_stat_info"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),			_T("IN"),	VariableType::VT_Int32, offsetof(GetTableStatProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TableId"),			_T("IN"),	VariableType::VT_Int32, offsetof(GetTableStatProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("HandsPlayed"),		_T("OUT"),	VariableType::VT_Int32, offsetof(GetTableStatProc,m_nHandsPlayed), false, false, false)
	DECLARE_VAR(_T("HandsWon"),			_T("OUT"),	VariableType::VT_Int32, offsetof(GetTableStatProc,m_nHandsWon), false, false, false)
	DECLARE_VAR(_T("SumBetAmount"),		_T("OUT"),	VariableType::VT_Double, offsetof(GetTableStatProc,m_dSumBetAmount), false, false, false)
	DECLARE_VAR(_T("SumWonAmount"),		_T("OUT"),	VariableType::VT_Double, offsetof(GetTableStatProc,m_dSumWonAmount), false, false, false)
	DECLARE_VAR(_T("SumAmountIn"),		_T("OUT"),	VariableType::VT_Double, offsetof(GetTableStatProc,m_dSumAmountIn), false, false, false)
	DECLARE_VAR(_T("SumAmountOut"),		_T("OUT"),	VariableType::VT_Double, offsetof(GetTableStatProc,m_dSumAmountOut), false, false, false)
END_DECLARE_VAR()

GetTableStatProc::~GetTableStatProc(){
	}

//==============================================
//
//	object: CancelTournamentProc_Return
//
//==============================================

CancelTournamentProc_Return::CancelTournamentProc_Return(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(CancelTournamentProc_Return, Serializable)
	DECLARE_VAR(_T("CancelTournamentProc_Return"),	_T(""),					VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),						_T("USER_ID"),			VariableType::VT_Int32, offsetof(CancelTournamentProc_Return,m_nUserId), false, false, false)
	DECLARE_VAR(_T("RefundAmount"),					_T("REFUND_AMOUNT"),	VariableType::VT_Double, offsetof(CancelTournamentProc_Return,m_dRefundAmount), false, false, false)
END_DECLARE_VAR()

CancelTournamentProc_Return::~CancelTournamentProc_Return(){
	}

//==============================================
//
//	procedure: CancelTournamentProc
//
//==============================================

CancelTournamentProc::CancelTournamentProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(CancelTournamentProc, Serializable)
	DECLARE_VAR(_T("CancelTournamentProc"),	_T("cancel_tournament"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Return"),				_T("RET"),					VariableType::VT_None, offsetof(CancelTournamentProc,m_refundInfo), false, true, true)
	DECLARE_VAR(_T("TournamentId"),			_T("IN"),					VariableType::VT_Int32, offsetof(CancelTournamentProc,m_nTournamentId), false, false, false)
END_DECLARE_VAR()

CancelTournamentProc::~CancelTournamentProc(){
	}

//==============================================
//
//	procedure: CancelTournamentRefundProc
//
//==============================================

CancelTournamentRefundProc::CancelTournamentRefundProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(CancelTournamentRefundProc, Serializable)
	DECLARE_VAR(_T("CancelTournamentRefundProc"),	_T("cancel_tournament_refund_amount"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Return"),						_T("RET"),					VariableType::VT_Int32, offsetof(CancelTournamentRefundProc,m_nRetCode), false, false, false)
	DECLARE_VAR(_T("TournamentId"),					_T("IN"),					VariableType::VT_Int32, offsetof(CancelTournamentRefundProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("UserId"),						_T("IN"),					VariableType::VT_Int32, offsetof(CancelTournamentRefundProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("RefundAmount"),					_T("IN"),					VariableType::VT_Double, offsetof(CancelTournamentRefundProc,m_dRefundAmount), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),					_T("OUT"),					VariableType::VT_Double, offsetof(CancelTournamentRefundProc,m_dBalanceNew), false, false, false)
END_DECLARE_VAR()

CancelTournamentRefundProc::~CancelTournamentRefundProc(){
	}

//==============================================
//
//	procedure: HideTournamentProc
//
//==============================================

HideTournamentProc::HideTournamentProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(HideTournamentProc, Serializable)
	DECLARE_VAR(_T("HideTournamentProc"),	_T("hide_tournament"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Return"),				_T("RET"),				VariableType::VT_Int32, offsetof(HideTournamentProc,m_nRetCode), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T("IN"),				VariableType::VT_Int32, offsetof(HideTournamentProc,m_nTournamentId), false, false, false)
END_DECLARE_VAR()

HideTournamentProc::~HideTournamentProc(){
	}

//==============================================
//
//	procedure: RegisterInTournamentProc
//
//==============================================

RegisterInTournamentProc::RegisterInTournamentProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(RegisterInTournamentProc, Serializable)
	DECLARE_VAR(_T("RegisterInTournamentProc"),	_T("register_in_tournament"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Return"),					_T("RET"),						VariableType::VT_None, offsetof(RegisterInTournamentProc,m_return), false, true, true)
	DECLARE_VAR(_T("UserId"),					_T("IN"),						VariableType::VT_Int32, offsetof(RegisterInTournamentProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("IP"),						_T("IN"),						VariableType::VT_String, offsetof(RegisterInTournamentProc,m_sIp), false, false, false)
	DECLARE_VAR(_T("IP_DWORD"),					_T("IN"),						VariableType::VT_Int32, offsetof(RegisterInTournamentProc,m_nIP), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T("IN"),						VariableType::VT_Int32, offsetof(RegisterInTournamentProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),				_T("IN"),						VariableType::VT_Int32, offsetof(RegisterInTournamentProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("UseVPPoints"),				_T("IN"),						VariableType::VT_Bool, offsetof(RegisterInTournamentProc,m_bUseVPPoints), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),				_T("OUT"),						VariableType::VT_Double, offsetof(RegisterInTournamentProc,m_dBalanceNew), false, false, false)
	DECLARE_VAR(_T("VPPointsNew"),				_T("OUT"),						VariableType::VT_Double, offsetof(RegisterInTournamentProc,m_dVPPointsNew), false, false, false)
	DECLARE_VAR(_T("ParticipantId"),			_T("OUT"),						VariableType::VT_Int32, offsetof(RegisterInTournamentProc,m_nParticipantId), false, false, false)
	DECLARE_VAR(_T("ErrorCode"),				_T("OUT"),						VariableType::VT_Int32, offsetof(RegisterInTournamentProc,m_nErrorCode), false, false, false)
END_DECLARE_VAR()

RegisterInTournamentProc::~RegisterInTournamentProc(){
	}

//==============================================
//
//	procedure: RegisterInTournamentByTicketProc
//
//==============================================

RegisterInTournamentByTicketProc::RegisterInTournamentByTicketProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(RegisterInTournamentByTicketProc, Serializable)
	DECLARE_VAR(_T("RegisterInTournamentByTicketProc"),	_T("register_in_tournament_by_ticket"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Return"),							_T("RET"),								VariableType::VT_None, offsetof(RegisterInTournamentByTicketProc,m_return), false, true, true)
	DECLARE_VAR(_T("UserId"),							_T("IN"),								VariableType::VT_Int32, offsetof(RegisterInTournamentByTicketProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("IP"),								_T("IN"),								VariableType::VT_String, offsetof(RegisterInTournamentByTicketProc,m_sIp), false, false, false)
	DECLARE_VAR(_T("IP_DWORD"),							_T("IN"),								VariableType::VT_Int32, offsetof(RegisterInTournamentByTicketProc,m_nIP), false, false, false)
	DECLARE_VAR(_T("TournamentId"),						_T("IN"),								VariableType::VT_Int32, offsetof(RegisterInTournamentByTicketProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),						_T("IN"),								VariableType::VT_Int32, offsetof(RegisterInTournamentByTicketProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),						_T("OUT"),								VariableType::VT_Double, offsetof(RegisterInTournamentByTicketProc,m_dBalanceNew), false, false, false)
	DECLARE_VAR(_T("ParticipantId"),					_T("OUT"),								VariableType::VT_Int32, offsetof(RegisterInTournamentByTicketProc,m_nParticipantId), false, false, false)
	DECLARE_VAR(_T("ErrorCode"),						_T("OUT"),								VariableType::VT_Int32, offsetof(RegisterInTournamentByTicketProc,m_nErrorCode), false, false, false)
END_DECLARE_VAR()

RegisterInTournamentByTicketProc::~RegisterInTournamentByTicketProc(){
	}

//==============================================
//
//	procedure: UnRegisterFromTournamentProc
//
//==============================================

UnRegisterFromTournamentProc::UnRegisterFromTournamentProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(UnRegisterFromTournamentProc, Serializable)
	DECLARE_VAR(_T("UnRegisterFromTournamentProc"),	_T("unregister_from_tournament"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),					_T("RET"),						VariableType::VT_Int32, offsetof(UnRegisterFromTournamentProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),					_T("IN"),						VariableType::VT_Int32, offsetof(UnRegisterFromTournamentProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T("IN"),						VariableType::VT_Int32, offsetof(UnRegisterFromTournamentProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),				_T("OUT"),						VariableType::VT_Double, offsetof(UnRegisterFromTournamentProc,m_dBalanceNew), false, false, false)
	DECLARE_VAR(_T("VPPointsNew"),				_T("OUT"),						VariableType::VT_Double, offsetof(UnRegisterFromTournamentProc,m_dVPPointsNew), false, false, false)
	DECLARE_VAR(_T("ParticipantId"),			_T("OUT"),						VariableType::VT_Int32, offsetof(UnRegisterFromTournamentProc,m_nParticipantId), false, false, false)
END_DECLARE_VAR()

UnRegisterFromTournamentProc::~UnRegisterFromTournamentProc(){
	}

//==============================================
//
//	procedure: UserRegTournamentsProc
//
//==============================================

UserRegTournament::UserRegTournament(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(UserRegTournament, Serializable)
	DECLARE_VAR(_T("UserRegTournament"),	_T(""),					VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ParticipantId"),		_T("ID"),				VariableType::VT_Int32, offsetof(UserRegTournament,m_nParticipantId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T("TOURNAMENT_ID"),	VariableType::VT_Int32, offsetof(UserRegTournament,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Name"),					_T("NAME"),				VariableType::VT_String, offsetof(UserRegTournament,m_sName), false, false, false)
	DECLARE_VAR(_T("StartDTime"),			_T("START_DTIME"),		VariableType::VT_DateTime, offsetof(UserRegTournament,m_dtStartDTime), false, false, false)
	DECLARE_VAR(_T("Status"),				_T("STATUS_ID"),		VariableType::VT_Int32, offsetof(UserRegTournament,m_nStatus), false, false, false)
	DECLARE_VAR(_T("BuyIn"),				_T("BUYIN_AMOUNT"),		VariableType::VT_Double, offsetof(UserRegTournament,m_dBuyIn), false, false, false)
	DECLARE_VAR(_T("BuyInFee"),				_T("BUYIN_FEE"),		VariableType::VT_Double, offsetof(UserRegTournament,m_dBuyInFee), false, false, false)
	DECLARE_VAR(_T("GameTypeId"),			_T("GAME_TYPE_ID"),		VariableType::VT_Int32, offsetof(UserRegTournament,m_nGameTypeId), false, false, false)
	DECLARE_VAR(_T("SubTypeId"),			_T("SUB_TYPE_ID"),		VariableType::VT_Int32, offsetof(UserRegTournament,m_nSubTypeId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("CURRENCY_ID"),		VariableType::VT_Int32, offsetof(UserRegTournament,m_nCurrencyId), false, false, false)
END_DECLARE_VAR()

UserRegTournament::~UserRegTournament(){
	}

UserRegTournamentsProc::UserRegTournamentsProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(UserRegTournamentsProc, Serializable)
	DECLARE_VAR(_T("UserRegTournamentsProc"),	_T("list_user_reg_tournaments"),VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("RegTournaments"),			_T("RET"),						VariableType::VT_None, offsetof(UserRegTournamentsProc,m_listRegTournaments), false, true, true)
	DECLARE_VAR(_T("UserId"),					_T("IN"),						VariableType::VT_Int32, offsetof(UserRegTournamentsProc,m_nUserId), false, false, false)
END_DECLARE_VAR()

UserRegTournamentsProc::~UserRegTournamentsProc(){
	}

//==============================================
//
//	procedure: ConvertVPPointsToMoneyProc
//
//==============================================

ConvertVPPointsToMoneyProc::ConvertVPPointsToMoneyProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(ConvertVPPointsToMoneyProc, Serializable)
	DECLARE_VAR(_T("ConvertVPPointsToMoneyProc"),	_T("convert_vppoints_into_money"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),						_T("RET"),							VariableType::VT_Int32, offsetof(ConvertVPPointsToMoneyProc,m_nSuccess), false, true, true)
	DECLARE_VAR(_T("UserId"),						_T("IN"),							VariableType::VT_Int32, offsetof(ConvertVPPointsToMoneyProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),					_T("IN"),							VariableType::VT_Int32, offsetof(ConvertVPPointsToMoneyProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("VPPointsConvert"),				_T("IN"),							VariableType::VT_Double, offsetof(ConvertVPPointsToMoneyProc,m_dVPPointsConvert), false, false, false)
	DECLARE_VAR(_T("ExactAmount"),					_T("IN"),							VariableType::VT_Double, offsetof(ConvertVPPointsToMoneyProc,m_dExactAmount), false, false, false)
	DECLARE_VAR(_T("BalanceNew"),					_T("OUT"),							VariableType::VT_Double, offsetof(ConvertVPPointsToMoneyProc,m_dBalanceNew), false, false, false)
	DECLARE_VAR(_T("VPPointsNew"),					_T("OUT"),							VariableType::VT_Double, offsetof(ConvertVPPointsToMoneyProc,m_dVPPointsNew), false, false, false)
END_DECLARE_VAR()

ConvertVPPointsToMoneyProc::~ConvertVPPointsToMoneyProc(){
	}

//==============================================
//
//	procedure: TournamentRebuyProc
//
//==============================================

TournamentRebuyProc::TournamentRebuyProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(TournamentRebuyProc, Serializable)
	DECLARE_VAR(_T("TournamentRebuyProc"),	_T("tournament_rebuy"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),				VariableType::VT_Int32, offsetof(TournamentRebuyProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(TournamentRebuyProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T("IN"),				VariableType::VT_Int32, offsetof(TournamentRebuyProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("ByPoints"),				_T("IN"),				VariableType::VT_Bool, offsetof(TournamentRebuyProc,m_bByPoints), false, false, false)
	DECLARE_VAR(_T("RebuyCtAtOnce"),		_T("IN"),				VariableType::VT_Int16, offsetof(TournamentRebuyProc,m_shRebuyCtAtOnce), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T("OUT"),				VariableType::VT_Double, offsetof(TournamentRebuyProc,m_dBalance), false, false, false)
	DECLARE_VAR(_T("Points"),				_T("OUT"),				VariableType::VT_Double, offsetof(TournamentRebuyProc,m_dPoints), false, false, false)
END_DECLARE_VAR()

TournamentRebuyProc::~TournamentRebuyProc(){
	}

//==============================================
//
//	procedure: TournamentAddonProc
//
//==============================================

TournamentAddonProc::TournamentAddonProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(TournamentAddonProc, Serializable)
	DECLARE_VAR(_T("TournamentAddonProc"),	_T("tournament_addon"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),				_T("RET"),				VariableType::VT_Int32, offsetof(TournamentAddonProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T("IN"),				VariableType::VT_Int32, offsetof(TournamentAddonProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),			_T("IN"),				VariableType::VT_Int32, offsetof(TournamentAddonProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("ByPoints"),				_T("IN"),				VariableType::VT_Bool, offsetof(TournamentAddonProc,m_bByPoints), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T("OUT"),				VariableType::VT_Double, offsetof(TournamentAddonProc,m_dBalance), false, false, false)
	DECLARE_VAR(_T("Points"),				_T("OUT"),				VariableType::VT_Double, offsetof(TournamentAddonProc,m_dPoints), false, false, false)
END_DECLARE_VAR()

TournamentAddonProc::~TournamentAddonProc(){
	}

//==============================================
//
//	procedure: PlayerFinishedTournamentProc
//
//==============================================

PlayerFinishedTournamentProc::PlayerFinishedTournamentProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(PlayerFinishedTournamentProc, Serializable)
	DECLARE_VAR(_T("PlayerFinishedTournamentProc"),	_T("player_finished_tournament"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),						_T("RET"),				VariableType::VT_Int32, offsetof(PlayerFinishedTournamentProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("ParticipantId"),				_T("IN"),				VariableType::VT_Int32, offsetof(PlayerFinishedTournamentProc,m_nParticipantId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),					_T("IN"),				VariableType::VT_Int32, offsetof(PlayerFinishedTournamentProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("UserId"),						_T("IN"),				VariableType::VT_Int32, offsetof(PlayerFinishedTournamentProc,m_nUserId), false, false, false)
	DECLARE_VAR(_T("Place"),						_T("IN"),				VariableType::VT_Int32, offsetof(PlayerFinishedTournamentProc,m_nPlace), false, false, false)
	DECLARE_VAR(_T("TournamentTicketIdWon"),		_T("IN"),				VariableType::VT_Int32, offsetof(PlayerFinishedTournamentProc,m_nTournamentTicketIdWon), false, false, false)
	DECLARE_VAR(_T("AmountWon"),					_T("IN"),				VariableType::VT_Double, offsetof(PlayerFinishedTournamentProc,m_dAmountWon), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),					_T("IN"),				VariableType::VT_Int32, offsetof(PlayerFinishedTournamentProc,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("Balance"),						_T("OUT"),				VariableType::VT_Double, offsetof(PlayerFinishedTournamentProc,m_dBalance), false, false, false)
END_DECLARE_VAR()

PlayerFinishedTournamentProc::~PlayerFinishedTournamentProc(){
	}

//==============================================
//
//	procedure: StartTournamentHandProc
//
//==============================================

StartTournamentHandProc::StartTournamentHandProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(StartTournamentHandProc, Serializable)
	DECLARE_VAR(_T("StartTournamentHandProc"),	_T("start_tournament_hand_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("HandId"),					_T("RET"),						VariableType::VT_Int64, offsetof(StartTournamentHandProc,m_nHandId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T("IN"),						VariableType::VT_Int32, offsetof(StartTournamentHandProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T("IN"),						VariableType::VT_Int32, offsetof(StartTournamentHandProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("LevelIndex"),				_T("IN"),						VariableType::VT_Int32, offsetof(StartTournamentHandProc,m_nLevelIndex), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),				_T("IN"),						VariableType::VT_Int32, offsetof(StartTournamentHandProc,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("BigBlindAmount"),			_T("IN"),						VariableType::VT_Double, offsetof(StartTournamentHandProc,m_dBigBlindAmount), false, false, false)
	DECLARE_VAR(_T("SmallBlindAmount"),			_T("IN"),						VariableType::VT_Double, offsetof(StartTournamentHandProc,m_dSmallBlindAmount), false, false, false)
	DECLARE_VAR(_T("AnteAmount"),				_T("IN"),						VariableType::VT_Double, offsetof(StartTournamentHandProc,m_dAnteAmount), false, false, false)
END_DECLARE_VAR()

StartTournamentHandProc::~StartTournamentHandProc(){
	}

//==============================================
//
//	procedure: FinishTournamentHandProc
//
//==============================================

FinishTournamentHandProc::FinishTournamentHandProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(FinishTournamentHandProc, Serializable)
	DECLARE_VAR(_T("FinishTournamentHandProc"),	_T("finish_tournament_hand_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("FinishStatus"),				_T("RET"),						VariableType::VT_Bool, offsetof(FinishTournamentHandProc,m_bFinishStatus), false, false, false)
	DECLARE_VAR(_T("HandId"),					_T("IN"),						VariableType::VT_Int64, offsetof(FinishTournamentHandProc,m_nHandId), false, false, false)
	DECLARE_VAR(_T("HandData"),					_T("IN"),						VariableType::VT_Binary, offsetof(FinishTournamentHandProc,m_binHandData), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T("IN"),						VariableType::VT_Int32, offsetof(FinishTournamentHandProc,m_nTableId), false, false, false)
	DECLARE_VAR(_T("PlayersInfo"),				_T("IN"),						VariableType::VT_String, offsetof(FinishTournamentHandProc,m_sPlayersInfo), false, false, false)
	DECLARE_VAR(_T("TableCards"),				_T("IN"),						VariableType::VT_String, offsetof(FinishTournamentHandProc,m_sTableCards), false, false, false)
	DECLARE_VAR(_T("TotalPotAmount"),			_T("IN"),						VariableType::VT_Double, offsetof(FinishTournamentHandProc,m_dTotalPotAmount), false, false, false)
END_DECLARE_VAR()

FinishTournamentHandProc::~FinishTournamentHandProc(){
	}

//==============================================
//
//	procedure: FinishTournamentProc
//
//==============================================

FinishTournamentProc::FinishTournamentProc(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(FinishTournamentProc, Serializable)
	DECLARE_VAR(_T("FinishTournamentProc"),		_T("finish_tournament_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Success"),					_T("RET"),						VariableType::VT_Int32, offsetof(FinishTournamentProc,m_nSuccess), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T("IN"),						VariableType::VT_Int32, offsetof(FinishTournamentProc,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),				_T("IN"),						VariableType::VT_Int32, offsetof(FinishTournamentProc,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("FinishedDtime"),			_T("IN"),						VariableType::VT_DateTime, offsetof(FinishTournamentProc,m_dtFinished), false, false, false)
END_DECLARE_VAR()

FinishTournamentProc::~FinishTournamentProc(){
	}

//==============================================
//
//	procedure: TestTableProc
//
//==============================================

TestTableProc::TestTableProc(){
	} 

TestTableProc::~TestTableProc(){
	}

BEGIN_DECLARE_VAR(TestTableProc, Serializable)
	DECLARE_VAR(_T("TestTableProc"),	_T("test_proc"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Var0"),				_T("RET"),			VariableType::VT_Int32, offsetof(TestTableProc,m_nId), false, false, false)
	DECLARE_VAR(_T("Var1"),				_T("IN"),			VariableType::VT_String, offsetof(TestTableProc,m_str), false, false, false)
	DECLARE_VAR(_T("Var2"),				_T("IN"),			VariableType::VT_DateTime, offsetof(TestTableProc,m_date), false, false, false)
	DECLARE_VAR(_T("Var3"),				_T("IN"),			VariableType::VT_DateTime, offsetof(TestTableProc,m_datetime), false, false, false)
	DECLARE_VAR(_T("Var4"),				_T("IN"),			VariableType::VT_Binary, offsetof(TestTableProc,m_binary), false, false, false)
	DECLARE_VAR(_T("Var5"),				_T("IN"),			VariableType::VT_Double, offsetof(TestTableProc,m_float), false, false, false)
	DECLARE_VAR(_T("Var6"),				_T("IN"),			VariableType::VT_Double, offsetof(TestTableProc,m_real), false, false, false)
	DECLARE_VAR(_T("Var7"),				_T("IN"),			VariableType::VT_Int64, offsetof(TestTableProc,m_big), false, false, false)
	DECLARE_VAR(_T("Var8"),				_T("IN"),			VariableType::VT_Double, offsetof(TestTableProc,m_dec), false, false, false)
	DECLARE_VAR(_T("Var9"),				_T("IN"),			VariableType::VT_Double, offsetof(TestTableProc,m_num), false, false, false)
	DECLARE_VAR(_T("Var10"),			_T("IN"),			VariableType::VT_Double, offsetof(TestTableProc,m_money), false, false, false)
END_DECLARE_VAR()
