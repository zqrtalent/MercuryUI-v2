#include "stdafx.h"
#include "TournamentPayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentPayout::TournamentPayout(){
	}

BEGIN_DECLARE_VAR(TournamentPayout, Serializable)
	DECLARE_VAR(_T("TournamentPayout"),		_T("TB_TOURNAMENT_PAYOUT"),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),					_T("ID"),						VariableType::VT_Int32, offsetof(TournamentPayout,m_nId), false, false, false)
	DECLARE_VAR(_T("ProfileId"),			_T("PAYOUT_PROFILE_ID"),		VariableType::VT_Int32, offsetof(TournamentPayout,m_nProfileId), false, false, false)
	DECLARE_VAR(_T("EntriesMin"),			_T("ENTRIES_MIN"),				VariableType::VT_Int32, offsetof(TournamentPayout,m_nEntriesMin), false, false, false)
	DECLARE_VAR(_T("EntriesMax"),			_T("ENTRIES_MAX"),				VariableType::VT_Int32, offsetof(TournamentPayout,m_nEntriesMax), false, false, false)
	// Linked table.
	DECLARE_VAR(_T("Detail"),				_T("ID=PAYOUT_ID"),				VariableType::VT_Int32, offsetof(TournamentPayout,m_listDetail), false, true, true)
END_DECLARE_VAR()

TournamentPayout::~TournamentPayout(){
	}