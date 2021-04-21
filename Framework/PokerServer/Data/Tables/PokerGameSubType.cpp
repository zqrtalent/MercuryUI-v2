#include "stdafx.h"
#include "PokerGameSubType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerGameSubType::PokerGameSubType(){
	} 

BEGIN_DECLARE_VAR(PokerGameSubType, Serializable)
	DECLARE_VAR(_T("PokerLimitType"),	_T("TB_GAME_SUB_TYPE"), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),				_T("Id"), VariableType::VT_Int32, offsetof(PokerGameSubType,m_nId), false, false, false)
	DECLARE_VAR(_T("NAME"),				_T("NAME"), VariableType::VT_String, offsetof(PokerGameSubType,m_sName), false, false, false)
END_DECLARE_VAR()

PokerGameSubType::~PokerGameSubType(){
	}
