#include "stdafx.h"
#include "TournamentPayoutDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentPayoutDetail::TournamentPayoutDetail(){
	}

BEGIN_DECLARE_VAR(TournamentPayoutDetail, Serializable)
	DECLARE_VAR(_T("TournamentPayoutDetail"),		_T("TB_TOURNAMENT_PAYOUT_DETAIL"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),							_T("ID"),							VariableType::VT_Int32, offsetof(TournamentPayoutDetail,m_nId), false, false, false)
	DECLARE_VAR(_T("PayoutId"),						_T("PAYOUT_ID"),					VariableType::VT_Int32, offsetof(TournamentPayoutDetail,m_nPayoutId), false, false, false)
	DECLARE_VAR(_T("FinishingStart"),				_T("FINISHING_POSITION_START"),		VariableType::VT_Int32, offsetof(TournamentPayoutDetail,m_nFinishingStart), false, false, false)
	DECLARE_VAR(_T("FinishingEnd"),					_T("FINISHING_POSITION_END"),		VariableType::VT_Int32, offsetof(TournamentPayoutDetail,m_nFinishingEnd), false, false, false)
	DECLARE_VAR(_T("PercentWin"),					_T("PERCENT_WIN"),					VariableType::VT_Double, offsetof(TournamentPayoutDetail,m_dPercentWin), false, false, false)
END_DECLARE_VAR()

TournamentPayoutDetail::~TournamentPayoutDetail(){
	}
