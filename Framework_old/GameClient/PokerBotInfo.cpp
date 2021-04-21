#include "PokerBotInfo.h"
#include "..\Utility\GrowableMemory.h"

///////////////////////////////////////////////////////////
///
///	class PokerBotInfo
///
///////////////////////////////////////////////////////////

PokerBotInfo::PokerBotInfo(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(PokerBotInfo, Serializable)
	DECLARE_VAR(_T("PokerBotInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserName"),			_T(""), VariableType::VT_String, offsetof(PokerBotInfo,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Password"),			_T(""), VariableType::VT_String, offsetof(PokerBotInfo,m_sPassword), false, false, false)
	DECLARE_VAR(_T("GameTypeId"),		_T(""), VariableType::VT_Int32, offsetof(PokerBotInfo,m_nGameTypeId), false, false, false)
	DECLARE_VAR(_T("SubGameTypeId"),	_T(""),	VariableType::VT_Int32, offsetof(PokerBotInfo,m_nSubGameTypeId), false, false, false)
	DECLARE_VAR(_T("PlayTablesCt"),		_T(""),	VariableType::VT_Int32, offsetof(PokerBotInfo,m_nPlayTablesCt), false, false, false)
END_DECLARE_VAR()

PokerBotInfo::~PokerBotInfo(){
	}


///////////////////////////////////////////////////////////
///
///	class PokerBots
///
///////////////////////////////////////////////////////////

PokerBots::PokerBots(){
	}

BEGIN_DECLARE_VAR(PokerBots, Serializable)
	DECLARE_VAR(_T("PokerBots"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Bots"),			_T(""), VariableType::VT_None, offsetof(PokerBots,m_listBots), false, true, true)
END_DECLARE_VAR()

PokerBots::~PokerBots(){
	}


