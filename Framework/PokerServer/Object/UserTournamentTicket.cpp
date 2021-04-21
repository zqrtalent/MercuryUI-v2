// UserTournamentTicket.cpp : implementation file
//

#include "stdafx.h"
#include "UserTournamentTicket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UserTournamentTicket::UserTournamentTicket(){
	}

BEGIN_DECLARE_VAR(UserTournamentTicket, Serializable)
	DECLARE_VAR(_T("UserTournamentTicket"),		_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""),		VariableType::VT_Int32, offsetof(UserTournamentTicket,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("TournamentName"),			_T(""),		VariableType::VT_String, offsetof(UserTournamentTicket,m_sTournamentName), false, false, false)
	DECLARE_VAR(_T("TicketCount"),				_T(""),		VariableType::VT_Int8, offsetof(UserTournamentTicket,m_cTicketCount), false, false, false)
END_DECLARE_VAR()

UserTournamentTicket::~UserTournamentTicket(){
	}