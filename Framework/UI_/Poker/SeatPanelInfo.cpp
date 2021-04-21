// SeatPanelInfo.cpp : implementation file
//

#include "stdafx.h"
#include "SeatPanelInfo.h"

SeatPanelInfo::SeatPanelInfo(){
	m_ptPos				.SetPoint(0, 0);
	m_ptChipPos			.SetPoint(0, 0);
	m_ptDealerButtonPos	.SetPoint(0, 0);
	m_chipGrowDir		= ChipGrowDirection::LeftToRight;
	}

BEGIN_DECLARE_VAR(SeatPanelInfo, Serializable)
	DECLARE_VAR(_T("SeatPanelInfo"),	_T(""),		VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("posX"),				_T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptPos.x), false, false, false)
	DECLARE_VAR(_T("posY"),				_T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptPos.y), false, false, false)
	DECLARE_VAR(_T("chipPosX"),			_T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptChipPos.x), false, false, false)
	DECLARE_VAR(_T("chipPosY"),			_T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptChipPos.y), false, false, false)
	DECLARE_VAR(_T("DealerButtonPosX"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptDealerButtonPos.x), false, false, false)
	DECLARE_VAR(_T("DealerButtonPosY"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptDealerButtonPos.y), false, false, false)

	DECLARE_VAR(_T("chipLabelTextPos"),	_T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_chipLabelTextPos), false, false, false)
	DECLARE_VAR(_T("chipGrowDir"),		_T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_chipGrowDir), false, false, false)

	DECLARE_VAR(_T("HiddenCard1PosX"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard1.x), false, false, false)
	DECLARE_VAR(_T("HiddenCard1PosY"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard1.y), false, false, false)
	DECLARE_VAR(_T("HiddenCard2PosX"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard2.x), false, false, false)
	DECLARE_VAR(_T("HiddenCard2PosY"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard2.y), false, false, false)
	DECLARE_VAR(_T("HiddenCard3PosX"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard3.x), false, false, false)
	DECLARE_VAR(_T("HiddenCard3PosY"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard3.y), false, false, false)
	DECLARE_VAR(_T("HiddenCard4PosX"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard4.x), false, false, false)
	DECLARE_VAR(_T("HiddenCard4PosY"), _T(""),		VariableType::Int32, offsetof(SeatPanelInfo,m_ptHiddenCard4.y), false, false, false)



END_DECLARE_VAR()

SeatPanelInfo::~SeatPanelInfo(){
	}