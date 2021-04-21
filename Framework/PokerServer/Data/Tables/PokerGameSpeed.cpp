#include "stdafx.h"
#include "PokerGameSpeed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerGameSpeed::PokerGameSpeed(){
	} 

PokerGameSpeed::~PokerGameSpeed(){
	}

BEGIN_DECLARE_VAR(PokerGameSpeed, Serializable)
	DECLARE_VAR(_T("PokerGameSpeed"),				_T("TB_GAME_SPEED"),					VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),							_T("ID"),								VariableType::VT_Int32, offsetof(PokerGameSpeed,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),							_T("NAME"),								VariableType::VT_String, offsetof(PokerGameSpeed,m_sName), false, false, false)
	DECLARE_VAR(_T("RegularTimeSec"),				_T("REGULAR_TIME_SEC"),					VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shRegularTimeSec), false, false, false)
	DECLARE_VAR(_T("ExtraTimeSec"),					_T("EXTRA_TIME_SEC"),					VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shExtraTimeSec), false, false, false)
	DECLARE_VAR(_T("HandStartDelay"),				_T("HAND_START_DELAY"),					VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shHandStartDelay), false, false, false)
	DECLARE_VAR(_T("PostBlindDelay"),				_T("POST_BLIND_DELAY"),					VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shPostBlindDelay), false, false, false)
	DECLARE_VAR(_T("StartRoundDelay"),				_T("START_ROUND_DELAY"),				VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shStartRoundDelay), false, false, false)
	DECLARE_VAR(_T("ShowMuckDelay"),				_T("SHOW_MUCK_DELAY"),					VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shShowMuckDelay), false, false, false)
	DECLARE_VAR(_T("ReserveSeatDelay"),				_T("RESERVE_SEAT_DELAY"),				VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shReserveSeatDelay), false, false, false)
	DECLARE_VAR(_T("LeaveSitOutedDelay"),			_T("LEAVE_SITOUTED_DELAY"),				VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shLeaveSitOutedDelay), false, false, false)
	DECLARE_VAR(_T("LeaveZeroBalancedSeatDelay"),	_T("LEAVE_ZERO_BALANCE_SEAT_DELAY"),	VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shLeaveZeroBalancedSeatDelay), false, false, false)
	DECLARE_VAR(_T("TimeBankSec"),					_T("TBANK_SEC"),						VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shTimeBankSec), false, false, false)
	DECLARE_VAR(_T("TimeBankIncSec"),				_T("TBANK_INCREASE_SEC"),				VariableType::VT_Int16, offsetof(PokerGameSpeed,m_shTimeBankIncSec), false, false, false)
	DECLARE_VAR(_T("TimeBankIncPerHands"),			_T("TBANK_INC_PER_HANDS"),				VariableType::VT_Int32, offsetof(PokerGameSpeed,m_nTimeBankIncPerHands), false, false, false)
END_DECLARE_VAR()
