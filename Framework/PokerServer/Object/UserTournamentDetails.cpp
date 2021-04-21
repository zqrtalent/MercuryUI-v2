// UserTournamentDetails.cpp : implementation file
//

#include "stdafx.h"
#include "UserTournamentDetails.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UserTournamentDetails::UserTournamentDetails(){
	}

BEGIN_DECLARE_VAR(UserTournamentDetails, Serializable)
	DECLARE_VAR(_T("UserTournamentDetails"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""),		VariableType::VT_Int32, offsetof(UserTournamentDetails,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Stake"),					_T(""),		VariableType::VT_Chips, offsetof(UserTournamentDetails,m_nStake), false, false, false)
	DECLARE_VAR(_T("Place"),					_T(""),		VariableType::VT_Int32, offsetof(UserTournamentDetails,m_nPlace), false, false, false)
	DECLARE_VAR(_T("RebuyLeft"),				_T(""),		VariableType::VT_Int8, offsetof(UserTournamentDetails,m_cRebuyLeft), false, false, false)
	DECLARE_VAR(_T("AddonLeft"),				_T(""),		VariableType::VT_Int8, offsetof(UserTournamentDetails,m_cAddonLeft), false, false, false)
END_DECLARE_VAR()

UserTournamentDetails::~UserTournamentDetails(){
	}