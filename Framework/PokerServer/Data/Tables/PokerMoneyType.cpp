#include "stdafx.h"
#include "PokerMoneyType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerMoneyType::PokerMoneyType(){
	} 

PokerMoneyType::~PokerMoneyType(){
	}

BEGIN_DECLARE_VAR(PokerMoneyType, Serializable)
	DECLARE_VAR(_T("PokerMoneyType"),	_T("TB_MONEY_TYPE"),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),				_T("Id"),					VariableType::VT_Int32, offsetof(PokerMoneyType,m_nId), false, false, false)
	DECLARE_VAR(_T("NAME"),				_T("NAME"),					VariableType::VT_String, offsetof(PokerMoneyType,m_sName), false, false, false)
END_DECLARE_VAR()

