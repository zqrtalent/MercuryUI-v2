#include "stdafx.h"
#include "PokerHand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerHand::PokerHand(){
	} 

BEGIN_DECLARE_VAR(PokerHand, Serializable)
	DECLARE_VAR(_T("PokerHand"),		_T("TB_HAND"),			VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),				_T("Id"),				VariableType::VT_Int64, offsetof(PokerHand,m_nId), false, false, false)
	DECLARE_VAR(_T("GameTableId"),		_T("GAME_TABLE_ID"),	VariableType::VT_Int32, offsetof(PokerHand,m_nGameTableId), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),		_T("PLAYERS_CT"),		VariableType::VT_Int16, offsetof(PokerHand,m_shPlayersCt), false, false, false)
	DECLARE_VAR(_T("SeatsCt"),			_T("SEATS_CT"),			VariableType::VT_Int16, offsetof(PokerHand,m_shSeatsCt), false, false, false)
	DECLARE_VAR(_T("Start"),			_T("START_DTIME"),		VariableType::VT_DateTime, offsetof(PokerHand,m_dtStart), false, false, false)
	DECLARE_VAR(_T("Finish"),			_T("FINISH_DTIME"),		VariableType::VT_DateTime, offsetof(PokerHand,m_dtFinish), false, false, false)
	DECLARE_VAR(_T("BigBlind"),			_T("BIG_BLIND"),		VariableType::VT_Double, offsetof(PokerHand,m_dBigBlind), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),		_T("SMALL_BLIND"),		VariableType::VT_Double, offsetof(PokerHand,m_dSmallBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),				_T("ANTE"),				VariableType::VT_Double, offsetof(PokerHand,m_dAnte), false, false, false)
	DECLARE_VAR(_T("TotalPot"),			_T("TOTAL_POT"),		VariableType::VT_Double, offsetof(PokerHand,m_dTotalPot), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),		_T("RAKE_AMOUNT"),		VariableType::VT_Double, offsetof(PokerHand,m_dRakeAmount), false, false, false)
	DECLARE_VAR(_T("RakePercent"),		_T("RAKE_PERCENT"),		VariableType::VT_Double, offsetof(PokerHand,m_dRakePercent), false, false, false)
	DECLARE_VAR(_T("binHandInfo"),		_T("HANDINFO_BIN"),		VariableType::VT_Binary, offsetof(PokerHand,m_binHandInfo), false, false, false)
END_DECLARE_VAR()

PokerHand::~PokerHand(){
	}



