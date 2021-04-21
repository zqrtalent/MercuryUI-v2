#include "PokerAvatar.h"

PokerAvatar::PokerAvatar(){
	} 

BEGIN_DECLARE_VAR(PokerAvatar, Serializable)
	DECLARE_VAR(_T("PokerAvatar"),		_T("TB_AVATAR"),			VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),				_T("Id"),					VariableType::VT_Int32, offsetof(PokerAvatar,m_nId), false, false, false)
	DECLARE_VAR(_T("PrivateUserId"),	_T("PRIVATE_USER_ID"),		VariableType::VT_Int32, offsetof(PokerAvatar,m_nPrivateUserId), false, false, false)
	DECLARE_VAR(_T("Status"),			_T("STATUS"),				VariableType::VT_Int16, offsetof(PokerAvatar,m_recStatus), false, false, false)
	DECLARE_VAR(_T("AvatarFile"),		_T("AVATAR_FILE_NAME"),		VariableType::VT_String, offsetof(PokerAvatar,m_sAvatarFileName), false, false, false)
//	DECLARE_VAR(_T("BinImage"),			_T(""),						VariableType::VT_Binary, offsetof(PokerAvatar,m_binImage), false, false, false)
END_DECLARE_VAR()

PokerAvatar::~PokerAvatar(){
	}



