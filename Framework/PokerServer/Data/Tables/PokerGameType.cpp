#include "stdafx.h"
#include "PokerGameType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerGameType::PokerGameType(){
	} 

BEGIN_DECLARE_VAR(PokerGameType, Serializable)
	DECLARE_VAR(_T("PokerGameType"), _T("TB_GAME_TYPE"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),			_T("Id"),				VariableType::VT_Int32, offsetof(PokerGameType,m_nId), false, false, false)
	DECLARE_VAR(_T("GameTypeName"),	_T("NAME"),				VariableType::VT_String, offsetof(PokerGameType,m_sGameTypeName), false, false, false)
END_DECLARE_VAR()

PokerGameType::~PokerGameType(){
	}



