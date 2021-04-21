// LobbyTournamentSummaryInfo.cpp : implementation file
//

#include "stdafx.h"
#include "LobbyTournamentSummaryInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////
//	class LobbyTournamentSummaryInfo
//
LobbyTournamentSummaryInfo::LobbyTournamentSummaryInfo(){
	}

BEGIN_DECLARE_VAR(LobbyTournamentSummaryInfo, Serializable)
	DECLARE_VAR(_T("LobbyTournamentSummaryInfo"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("RegisteredCt"),					_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentSummaryInfo,m_nRegisteredCt), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),					_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentSummaryInfo,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TablesCt"),						_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentSummaryInfo,m_shTablesCt), false, false, false)
	DECLARE_VAR(_T("PlacesPaid"),					_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentSummaryInfo,m_shPlacesPaid), false, false, false)
	DECLARE_VAR(_T("TournamentStatus"),				_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentSummaryInfo,m_cTournamentStatus), false, false, false)
	DECLARE_VAR(_T("RegisteredStatus"),				_T(""),	VariableType::VT_Bool, offsetof(LobbyTournamentSummaryInfo,m_bRegisteredStatus), false, false, false)
	DECLARE_VAR(_T("RegistrationAllowed"),			_T(""),	VariableType::VT_Bool, offsetof(LobbyTournamentSummaryInfo,m_bRegistrationAllowed), false, false, false)

	DECLARE_VAR(_T("IsBreak"),						_T(""),	VariableType::VT_Bool, offsetof(LobbyTournamentSummaryInfo,m_bIsBreak), false, false, false)
	DECLARE_VAR(_T("HandForHand"),					_T(""),	VariableType::VT_Bool, offsetof(LobbyTournamentSummaryInfo,m_bHandForHand), false, false, false)
	DECLARE_VAR(_T("LevelIndex"),					_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentSummaryInfo,m_cLevelIndex), false, false, false)
	DECLARE_VAR(_T("SecLeftTillNextLevel"),			_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentSummaryInfo,m_shSecLeftTillNextLevel), false, false, false)
	DECLARE_VAR(_T("SecLeftTillNextBreak"),			_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentSummaryInfo,m_shSecLeftTillNextBreak), false, false, false)

	DECLARE_VAR(_T("LargestStake"),					_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentSummaryInfo,m_nLargestStake), false, false, false)
	DECLARE_VAR(_T("AverageStake"),					_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentSummaryInfo,m_nAverageStake), false, false, false)
	DECLARE_VAR(_T("SmallestStake"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentSummaryInfo,m_nSmallestStake), false, false, false)

	// Complex objects.
	DECLARE_VAR(_T("BlindStructure"),				_T(""),	VariableType::VT_None, offsetof(LobbyTournamentSummaryInfo,m_listBlindStructure), false, true, true)
	DECLARE_VAR(_T("Tables"),						_T(""),	VariableType::VT_None, offsetof(LobbyTournamentSummaryInfo,m_listTables), false, true, true)
	DECLARE_VAR(_T("TablePlayers"),					_T(""),	VariableType::VT_None, offsetof(LobbyTournamentSummaryInfo,m_listTablePlayers), false, true, true)
	DECLARE_VAR(_T("Participants"),					_T(""),	VariableType::VT_None, offsetof(LobbyTournamentSummaryInfo,m_listParticipants), false, true, true)
	DECLARE_VAR(_T("PayoutInfo"),					_T(""),	VariableType::VT_None, offsetof(LobbyTournamentSummaryInfo,m_listPayoutInfo), false, true, true)
	DECLARE_VAR(_T("TournamentInfo"),				_T(""),	VariableType::VT_None, offsetof(LobbyTournamentSummaryInfo,m_listInfo), false, true, true)
END_DECLARE_VAR()

LobbyTournamentSummaryInfo::~LobbyTournamentSummaryInfo(){
	}