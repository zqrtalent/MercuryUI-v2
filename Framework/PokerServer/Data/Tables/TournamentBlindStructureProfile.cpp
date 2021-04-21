#include "stdafx.h"
#include "TournamentBlindStructureProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentBlindStructureProfile::TournamentBlindStructureProfile(){
	}

BEGIN_DECLARE_VAR(TournamentBlindStructureProfile, Serializable)
	DECLARE_VAR(_T("TournamentBlindStructureProfile"),		_T("TB_TOURNAMENT_BLIND_STRUCTURE_PROFILE"), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),									_T("ID"),				VariableType::VT_Int32, offsetof(TournamentBlindStructureProfile,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),									_T("NAME"),				VariableType::VT_String, offsetof(TournamentBlindStructureProfile,m_sName), false, false, false)
END_DECLARE_VAR()

TournamentBlindStructureProfile::~TournamentBlindStructureProfile(){
	}
