#include "stdafx.h"
#include "PokerUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerUser::PokerUser(){
	} 

BEGIN_DECLARE_VAR(PokerUser, Serializable)
	DECLARE_VAR(_T("PokerUser"),			_T("TB_USER"),					VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),					_T("Id"),						VariableType::VT_Int32, offsetof(PokerUser,m_nId), false, false, false)
	DECLARE_VAR(_T("UserName"),				_T("USER_NAME"),				VariableType::VT_String, offsetof(PokerUser,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Password"),				_T("PASSWORD"),					VariableType::VT_String, offsetof(PokerUser,m_sPassword), false, false, false)
	DECLARE_VAR(_T("PermissionId"),			_T("PERMISSION_ID"),			VariableType::VT_Int32, offsetof(PokerUser,m_nPermissionId), false, false, false)
	DECLARE_VAR(_T("AvatarId"),				_T("AVATAR_ID"),				VariableType::VT_Int32, offsetof(PokerUser,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("Email"),				_T("E_MAIL"),					VariableType::VT_String, offsetof(PokerUser,m_sEmail), false, false, false)
	DECLARE_VAR(_T("FirstName"),			_T("FIRST_NAME"),				VariableType::VT_String, offsetof(PokerUser,m_sFirstName), false, false, false)
	DECLARE_VAR(_T("LastName"),				_T("LAST_NAME"),				VariableType::VT_String, offsetof(PokerUser,m_sLastName), false, false, false)
	DECLARE_VAR(_T("Phone"),				_T("PHONE"),					VariableType::VT_String, offsetof(PokerUser,m_sPhone), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("CURRENCY_ID"),				VariableType::VT_Int32, offsetof(PokerUser,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("PlayAmount"),			_T("PLAY_AMOUNT"),				VariableType::VT_Double, offsetof(PokerUser,m_dPlayAmount), false, false, false)
	DECLARE_VAR(_T("VPPoints"),				_T("VPPOINTS"),					VariableType::VT_Double, offsetof(PokerUser,m_dVPPoints), false, false, false)
	DECLARE_VAR(_T("RankIndex"),			_T("RANK_INDEX"),				VariableType::VT_Int16, offsetof(PokerUser,m_shRankIndex), false, false, false)
	DECLARE_VAR(_T("Registered"),			_T("REGISTER_DTIME"),			VariableType::VT_DateTime, offsetof(PokerUser,m_dtRegistered), false, false, false)
	DECLARE_VAR(_T("IsBlocked"),			_T("IS_BLOCKED"),				VariableType::VT_Bool, offsetof(PokerUser,m_bIsBlocked), false, false, false)
	DECLARE_VAR(_T("AutoUnblockDtime"),		_T("AUTO_UNBLOCK_DTIME"),		VariableType::VT_DateTime, offsetof(PokerUser,m_dtAutoUnblockDtime), false, false, false)
	DECLARE_VAR(_T("IsChatBlocked"),		_T("IS_CHAT_BLOCKED"),			VariableType::VT_Bool, offsetof(PokerUser,m_bIsChatBlocked), false, false, false)
	DECLARE_VAR(_T("IsAvatarBlocked"),		_T("IS_AVATAR_BLOCKED"),		VariableType::VT_Bool, offsetof(PokerUser,m_bIsAvatarBlocked), false, false, false)
END_DECLARE_VAR()

PokerUser::~PokerUser(){
	}
