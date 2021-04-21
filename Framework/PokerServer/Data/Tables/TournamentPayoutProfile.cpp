#include "stdafx.h"
#include "TournamentPayoutProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentPayoutProfile::TournamentPayoutProfile(){
	}

BEGIN_DECLARE_VAR(TournamentPayoutProfile, Serializable)
	DECLARE_VAR(_T("TournamentPayoutProfile"),		_T("TB_TOURNAMENT_PAYOUT_PROFILE"),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),							_T("ID"),								VariableType::VT_Int32, offsetof(TournamentPayoutProfile,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),							_T("NAME"),								VariableType::VT_String, offsetof(TournamentPayoutProfile,m_sName), false, false, false)
END_DECLARE_VAR()

TournamentPayoutProfile::~TournamentPayoutProfile(){
	}