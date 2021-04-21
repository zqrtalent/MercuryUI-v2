// LobbyTournamentPayoutInfo.cpp : implementation file
//

#include "LobbyTournamentPayoutInfo.h"

////////////////////////////////////////////
//	class LobbyTournamentPayoutInfo
//
LobbyTournamentPayoutInfo::LobbyTournamentPayoutInfo(){
	}

BEGIN_DECLARE_VAR(LobbyTournamentPayoutInfo, Serializable)
	DECLARE_VAR(_T("LobbyTournamentPayoutInfo"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("PrizePool"),					_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentPayoutInfo,m_nPrizePool), false, false, false)
	DECLARE_VAR(_T("EntriesCt"),					_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentPayoutInfo,m_nEntriesCt), false, false, false)
	DECLARE_VAR(_T("RebuysCt"),						_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentPayoutInfo,m_nRebuysCt), false, false, false)
	DECLARE_VAR(_T("AddonsCt"),						_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentPayoutInfo,m_nAddonsCt), false, false, false)
	DECLARE_VAR(_T("PlacesPaid"),					_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentPayoutInfo,m_shPlacesPaid), false, false, false)
	DECLARE_VAR(_T("Detail"),						_T(""),	VariableType::VT_None, offsetof(LobbyTournamentPayoutInfo,m_listDetail), false, true, true)
END_DECLARE_VAR()

LobbyTournamentPayoutInfo::~LobbyTournamentPayoutInfo(){
	}

////////////////////////////////////////////
//	class LobbyTournamentPayoutDetail
//
LobbyTournamentPayoutDetail::LobbyTournamentPayoutDetail(){
	}

BEGIN_DECLARE_VAR(LobbyTournamentPayoutDetail, Serializable)
	DECLARE_VAR(_T("LobbyTournamentPayoutDetail"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("PlaceFirst"),					_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentPayoutDetail,m_shPlaceFirst), false, false, false)
	DECLARE_VAR(_T("PlaceLast"),					_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentPayoutDetail,m_shPlaceLast), false, false, false)
	DECLARE_VAR(_T("Amount"),						_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentPayoutDetail,m_nAmount), false, false, false)
END_DECLARE_VAR()

LobbyTournamentPayoutDetail::~LobbyTournamentPayoutDetail(){
	}