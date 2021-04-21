// PokerHandWinnerInfo.cpp : implementation file
//

#include "stdafx.h"
#include "PokerHandWinnerInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////
//
//	class PokerHandWinnerInfo
//

PokerHandWinnerInfo::PokerHandWinnerInfo(){
	m_cWinnerCard1 = m_cWinnerCard2 = m_cWinnerCard3 = m_cWinnerCard4 = m_cWinnerCard5 = -1;
	}

BEGIN_DECLARE_VAR(PokerHandWinnerInfo, Serializable)
	DECLARE_VAR(_T("PokerHandWinnerInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Seat"),					_T(""), VariableType::VT_Int8, offsetof(PokerHandWinnerInfo,m_cSeat), false, false, false)
	DECLARE_VAR(_T("HandCardClass"),		_T(""), VariableType::VT_Int8, offsetof(PokerHandWinnerInfo,m_cHandCardClass), false, false, false)
	DECLARE_VAR(_T("WonAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerHandWinnerInfo,m_nWonAmount), false, false, false)
	DECLARE_VAR(_T("ReturnBackAmount"),		_T(""), VariableType::VT_Chips, offsetof(PokerHandWinnerInfo,m_nReturnBackAmount), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T(""), VariableType::VT_Chips, offsetof(PokerHandWinnerInfo,m_nBalance), false, false, false)
	DECLARE_VAR(_T("IsWinner"),				_T(""), VariableType::VT_Bool, offsetof(PokerHandWinnerInfo,m_bWinner), false, false, false)
	DECLARE_VAR(_T("WonAmountDetail"),		_T(""), VariableType::VT_None, offsetof(PokerHandWinnerInfo,m_listWonAmountDetail), false, true, true)
	DECLARE_VAR(_T("WinnerCard1"),			_T(""), VariableType::VT_Int8, offsetof(PokerHandWinnerInfo,m_cWinnerCard1), false, false, false)
	DECLARE_VAR(_T("WinnerCard2"),			_T(""), VariableType::VT_Int8, offsetof(PokerHandWinnerInfo,m_cWinnerCard2), false, false, false)
	DECLARE_VAR(_T("WinnerCard3"),			_T(""), VariableType::VT_Int8, offsetof(PokerHandWinnerInfo,m_cWinnerCard3), false, false, false)
	DECLARE_VAR(_T("WinnerCard4"),			_T(""), VariableType::VT_Int8, offsetof(PokerHandWinnerInfo,m_cWinnerCard4), false, false, false)
	DECLARE_VAR(_T("WinnerCard5"),			_T(""), VariableType::VT_Int8, offsetof(PokerHandWinnerInfo,m_cWinnerCard5), false, false, false)
END_DECLARE_VAR()

PokerHandWinnerInfo::~PokerHandWinnerInfo(){
	}

//////////////////////////////////////////////////////
//
//	class PokerWonAmountDetail
//

PokerWonAmountDetail::PokerWonAmountDetail(){
	m_nWonAmount	= 0;
	m_wRakeAmount	= 0;
	m_cPotIndex		= 0;
	}

BEGIN_DECLARE_VAR(PokerWonAmountDetail, Serializable)
	DECLARE_VAR(_T("PokerWonAmountDetail"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("WonAmount"),			_T(""), VariableType::VT_Chips, offsetof(PokerWonAmountDetail,m_nWonAmount), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),			_T(""), VariableType::VT_Int32, offsetof(PokerWonAmountDetail,m_wRakeAmount), false, false, false)
	DECLARE_VAR(_T("PotIndex"),				_T(""), VariableType::VT_Int8, offsetof(PokerWonAmountDetail,m_cPotIndex), false, false, false)
END_DECLARE_VAR()

PokerWonAmountDetail::~PokerWonAmountDetail(){
	}