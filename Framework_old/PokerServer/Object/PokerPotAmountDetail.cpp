// PokerPotAmountDetail.cpp : implementation file
//

#include "PokerPotAmountDetail.h"

//////////////////////////////////////////////////////
//
//	class PokerPotAmountDetail
//

PokerPotAmountDetail::PokerPotAmountDetail(){
	}

BEGIN_DECLARE_VAR(PokerPotAmountDetail, Serializable)
	DECLARE_VAR(_T("PokerPotAmountDetail"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Amount"),					_T(""), VariableType::VT_Int32, offsetof(PokerPotAmountDetail,m_nAmount), false, false, false)
	DECLARE_VAR(_T("RakeAmount"),				_T(""), VariableType::VT_Int16, offsetof(PokerPotAmountDetail,m_wRakeAmount), false, false, false)
	DECLARE_VAR(_T("PotIndex"),					_T(""), VariableType::VT_Int8, offsetof(PokerPotAmountDetail,m_cPotIndex), false, false, false)
END_DECLARE_VAR()

PokerPotAmountDetail::~PokerPotAmountDetail(){
	}