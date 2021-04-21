#include "stdafx.h"
#include "TournamentBlindStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentBlindStructure::TournamentBlindStructure(){
	}

TournamentBlindStructure::~TournamentBlindStructure(){
	}

BEGIN_DECLARE_VAR(TournamentBlindStructure, Serializable)
	DECLARE_VAR(_T("TournamentBlindStructure"),		_T("TB_TOURNAMENT_BLIND_STRUCTURE"), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),							_T("ID"),				VariableType::VT_Int32, offsetof(TournamentBlindStructure,m_nId), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),					_T("SMALL_BLIND"),		VariableType::VT_Double, offsetof(TournamentBlindStructure,m_dSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),						_T("BIG_BLIND"),		VariableType::VT_Double, offsetof(TournamentBlindStructure,m_dBigBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),							_T("ANTE"),				VariableType::VT_Double, offsetof(TournamentBlindStructure,m_dAnte), false, false, false)
END_DECLARE_VAR()

