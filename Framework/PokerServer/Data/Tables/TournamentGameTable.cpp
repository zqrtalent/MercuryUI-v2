#include "stdafx.h"
#include "TournamentGameTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentGameTable::TournamentGameTable(){
	}

TournamentGameTable::~TournamentGameTable(){
	}

BEGIN_DECLARE_VAR(TournamentGameTable, Serializable)
	DECLARE_VAR(_T("TournamentGameTable"),		_T("TB_TOURNAMENT_GAME_TABLES"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),						_T("ID"),							VariableType::VT_Int32, offsetof(TournamentGameTable,m_nId), false, false, false)
	DECLARE_VAR(_T("TableId"),					_T("TABLE_ID"),						VariableType::VT_Int32, offsetof(TournamentGameTable,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T("TOURNAMENT_ID"),				VariableType::VT_Int32, offsetof(TournamentGameTable,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TableInfo"),				_T("TABLE_INFO"),					VariableType::VT_Binary, offsetof(TournamentGameTable,m_binTableInfo), false, false, false)
	DECLARE_VAR(_T("PlayerCt"),					_T("PLAYER_CT"),					VariableType::VT_Int8, offsetof(TournamentGameTable,m_cPlayerCt), false, false, false)
END_DECLARE_VAR()