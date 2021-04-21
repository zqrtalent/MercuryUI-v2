#include "stdafx.h"
#include "TournamentParticipant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentParticipant::TournamentParticipant(){
	}

TournamentParticipant::~TournamentParticipant(){
	}

BEGIN_DECLARE_VAR(TournamentParticipant, Serializable)
	DECLARE_VAR(_T("TournamentParticipant"),	_T("TB_TOURNAMENT_PARTICIPANTS"),	VariableType::VT_None,		0, false, true, false)
	DECLARE_VAR(_T("Id"),						_T("ID"),							VariableType::VT_Int32,		offsetof(TournamentParticipant,m_nId), false, false, false)
	DECLARE_VAR(_T("TournamentId"),				_T("TOURNAMENT_ID"),				VariableType::VT_Int32,		offsetof(TournamentParticipant,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("UserId"),					_T("USER_ID"),						VariableType::VT_Int32,		offsetof(TournamentParticipant,m_nUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),					_T("USER_NAME"),					VariableType::VT_String,	offsetof(TournamentParticipant,m_sUserName), false, false, false)
	DECLARE_VAR(_T("IP"),						_T("IP"),							VariableType::VT_String,	offsetof(TournamentParticipant,m_sIP), false, false, false)
	DECLARE_VAR(_T("Ip_Number"),				_T("IP_DWORD"),						VariableType::VT_Int32,		offsetof(TournamentParticipant,m_nIp), false, false, false)
	DECLARE_VAR(_T("AmountChips"),				_T("AMOUNT_CHIPS"),					VariableType::VT_Double,	offsetof(TournamentParticipant,m_dAmountChips), false, false, false)
	DECLARE_VAR(_T("AddonCountLeft"),			_T("ADDON_COUNT_LEFT"),				VariableType::VT_Int16,		offsetof(TournamentParticipant,m_shAddonCountLeft), false, false, false)
	DECLARE_VAR(_T("RebuyCountLeft"),			_T("REBUY_COUNT_LEFT"),				VariableType::VT_Int16,		offsetof(TournamentParticipant,m_shRebuyCountLeft), false, false, false)
	DECLARE_VAR(_T("IsActive"),					_T("IS_ACTIVE"),					VariableType::VT_Bool,		offsetof(TournamentParticipant,m_bIsActive), false, false, false)
	DECLARE_VAR(_T("Place"),					_T("PLACE"),						VariableType::VT_Int32,		offsetof(TournamentParticipant,m_nPlace), false, false, false)
	DECLARE_VAR(_T("PrizeWon"),					_T("PRIZE_WON"),					VariableType::VT_Double,	offsetof(TournamentParticipant,m_dPrizeWon), false, false, false)
	DECLARE_VAR(_T("TournamentTicketIdWon"),	_T("TOURNAMENT_TICKET_ID_WON"),		VariableType::VT_Int32,		offsetof(TournamentParticipant,m_nTournamentTicketIdWon), false, false, false)
	DECLARE_VAR(_T("RegistrationType"),			_T("REGISTRATION_TYPE"),			VariableType::VT_Int16,		offsetof(TournamentParticipant,m_shRegistrationType), false, false, false)
END_DECLARE_VAR()