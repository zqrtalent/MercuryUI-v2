// LobbyTournamentInfo.cpp : implementation file
//

#include "LobbyTournamentInfo.h"

////////////////////////////////////////////
//	class LobbyTournamentInfo
//
LobbyTournamentInfo::LobbyTournamentInfo(){
	// Create variable indexes. {{
	m_listTables		.CreateVariableIndex(_T("TableId"), Serializable::Compare_Int32);
	m_listParticipants	.CreateVariableIndex(_T("UserId"),	Serializable::Compare_Int32);
	m_listParticipants	.CreateVariableIndex(_T("Order"),	Serializable::Compare_Int32_Desc);
//	m_listParticipants	.CreateVariableIndex(_T("Chips"), (CompareProc)Serializable::Compare_Int32_Desc);
	// }}
	}

BEGIN_DECLARE_VAR(LobbyTournamentInfo, Serializable)
	DECLARE_VAR(_T("LobbyTournamentInfo"),		_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Name"),						_T(""),	VariableType::VT_String, offsetof(LobbyTournamentInfo,m_sName), false, false, false)
	DECLARE_VAR(_T("Description"),				_T(""),	VariableType::VT_String, offsetof(LobbyTournamentInfo,m_sDescription), false, false, false)
	DECLARE_VAR(_T("TimeTournamentStart"),		_T(""),	VariableType::VT_DateTime, offsetof(LobbyTournamentInfo,m_dtTimeTournamentStart), false, false, false)
	DECLARE_VAR(_T("TimeTournamentFinished"),	_T(""),	VariableType::VT_DateTime, offsetof(LobbyTournamentInfo,m_dtTimeTournamentFinished), false, false, false)
	DECLARE_VAR(_T("TimeRegistrationStart"),	_T(""),	VariableType::VT_DateTime, offsetof(LobbyTournamentInfo,m_dtTimeRegistrationStart), false, false, false)
	DECLARE_VAR(_T("TimeRegistrationEnd"),		_T(""),	VariableType::VT_DateTime, offsetof(LobbyTournamentInfo,m_dtTimeRegistrationEnd), false, false, false)
	DECLARE_VAR(_T("GameType"),					_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentInfo,m_cGameType), false, false, false)
	DECLARE_VAR(_T("SubType"),					_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentInfo,m_shSubType), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),				_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentInfo,m_cCurrencyId), false, false, false)
	DECLARE_VAR(_T("Status"),					_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentInfo,m_status), false, false, false)
	DECLARE_VAR(_T("BuyinAmount"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nBuyinAmount), false, false, false)
	DECLARE_VAR(_T("FeeAmount"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nFeeAmount), false, false, false)
	DECLARE_VAR(_T("BuyInPoints"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nBuyInPoints), false, false, false)

	DECLARE_VAR(_T("StartingChips"),			_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentInfo,m_shStartingChips), false, false, false)

	DECLARE_VAR(_T("RebuyAmount"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nRebuyAmount), false, false, false)
	DECLARE_VAR(_T("RebuyChips"),				_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentInfo,m_shRebuyChips), false, false, false)
	DECLARE_VAR(_T("AddonAmount"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nAddonAmount), false, false, false)
	DECLARE_VAR(_T("AddonChips"),				_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentInfo,m_shAddonChips), false, false, false)

	DECLARE_VAR(_T("RebuyCt"),					_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentInfo,m_cRebuyCt), false, false, false)
	DECLARE_VAR(_T("AddonCt"),					_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentInfo,m_cAddonCt), false, false, false)
	
	DECLARE_VAR(_T("PrizeAmount"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nPrizeAmount), false, false, false)
	DECLARE_VAR(_T("PrizeTicketId"),			_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nPrizeTicketId), false, false, false)
	DECLARE_VAR(_T("MinPlayers"),				_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentInfo,m_shMinPlayers), false, false, false)
	DECLARE_VAR(_T("MaxPlayers"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nMaxPlayers), false, false, false)
	DECLARE_VAR(_T("RegisteredCt"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nRegisteredCt), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentInfo,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TableCt"),					_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentInfo,m_shTableCt), false, false, false)

	// Complex objects.
//	DECLARE_VAR(_T("Tables"),					_T(""),	VariableType::VT_None, offsetof(LobbyTournamentInfo,m_listTables), false, true, true)
//	DECLARE_VAR(_T("Participants"),				_T(""),	VariableType::VT_None, offsetof(LobbyTournamentInfo,m_listParticipants), false, true, true)
//	DECLARE_VAR(_T("BlindStructure"),			_T(""),	VariableType::VT_None, offsetof(LobbyTournamentInfo,m_listBlindStructure), false, true, true)
//	DECLARE_VAR(_T("PayoutInfo"),				_T(""),	VariableType::VT_None, offsetof(LobbyTournamentInfo,m_listPayoutInfo), false, true, true)
END_DECLARE_VAR()

LobbyTournamentInfo::~LobbyTournamentInfo(){
	}

int
LobbyTournamentInfo::GetPlayersPlace(int nUserId){
	int						nRank			= -1;
	TournamentPlayerInfo*	pParticipant	= m_listParticipants.FindOneVariable(_T("UserId"), &nUserId, &nRank);
	if( !pParticipant )
		return -1;
	m_listParticipants.FindOneVariable(_T("Order"), &pParticipant->m_nOrder, &nRank);
	return nRank;
	}

bool
LobbyTournamentInfo::SetParticipantChipsAmountAndPlace(int nUserId, int nChipsAmount, int nPlace /*= -1*/){
	TournamentPlayerInfo* pParticipant = m_listParticipants.FindOneVariable(_T("UserId"), &nUserId);
	if( !pParticipant ) return false;

	pParticipant->m_shPlace = (short)nPlace;
	pParticipant->m_nChips	= nChipsAmount;

	//TRACE(_T("SetParticipantChipsAmountAndPlace UserId=%d nChipsAmount=%d nPlace=%d\r\n"), nUserId, nChipsAmount, nPlace);

	int	nOrder = nChipsAmount;
	// Need to make an right order of tournament players by rank and chips amount.
	if( nPlace > 0 && !nChipsAmount )
		nOrder = (-1 * nPlace);

	if( pParticipant->m_nOrder == nOrder ) return true;
	// Update variable index.
	if( !m_listParticipants.ModifyIndexVariable(_T("Order"), pParticipant, &nOrder) ){
		ASSERT(FALSE);
		m_listParticipants.ModifyIndexVariable(_T("Order"), pParticipant, &nOrder);
		}
	return true;
	}

////////////////////////////////////////////
//	class LobbyTournamentDynamicInfo
//
LobbyTournamentDynamicInfo::LobbyTournamentDynamicInfo(){
	}

BEGIN_DECLARE_VAR(LobbyTournamentDynamicInfo, Serializable)
	DECLARE_VAR(_T("LobbyTournamentDynamicInfo"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TournamentId"),						_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentDynamicInfo,m_nTournamentId), false, false, false)
	DECLARE_VAR(_T("Status"),							_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentDynamicInfo,m_status), false, false, false)
	DECLARE_VAR(_T("TimeTournamentFinished"),			_T(""),	VariableType::VT_DateTime, offsetof(LobbyTournamentDynamicInfo,m_dtTimeTournamentFinished), false, false, false)
	DECLARE_VAR(_T("RegisteredCt"),						_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentDynamicInfo,m_nRegisteredCt), false, false, false)
	DECLARE_VAR(_T("PlayersCt"),						_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentDynamicInfo,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TableCt"),							_T(""),	VariableType::VT_Int16, offsetof(LobbyTournamentDynamicInfo,m_shTableCt), false, false, false)
END_DECLARE_VAR()

LobbyTournamentDynamicInfo::~LobbyTournamentDynamicInfo(){
	}

////////////////////////////////////////////
//	class TournamentPlayerInfo
//
TournamentPlayerInfo::TournamentPlayerInfo(){
	}

BEGIN_DECLARE_VAR(TournamentPlayerInfo, Serializable)
	DECLARE_VAR(_T("TournamentPlayerInfo"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Level"),				_T(""),	VariableType::VT_Int8, offsetof(TournamentPlayerInfo,m_cLevel), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T(""),	VariableType::VT_Int32, offsetof(TournamentPlayerInfo,m_nUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),				_T(""),	VariableType::VT_String, offsetof(TournamentPlayerInfo,m_sUserName), false, false, false)
	DECLARE_VAR(_T("Chips"),				_T(""),	VariableType::VT_Int32, offsetof(TournamentPlayerInfo,m_nChips), false, false, false)
	DECLARE_VAR(_T("Place"),				_T(""),	VariableType::VT_Int16, offsetof(TournamentPlayerInfo,m_shPlace), false, false, false)
	DECLARE_VAR(_T("Order"),				_T(""),	VariableType::VT_Int32, offsetof(TournamentPlayerInfo,m_nOrder), false, false, false)
END_DECLARE_VAR()

TournamentPlayerInfo::~TournamentPlayerInfo(){
	}

////////////////////////////////////////////
//	class TournamentTableInfo
//
TournamentTableInfo::TournamentTableInfo(){
	}

BEGIN_DECLARE_VAR(TournamentTableInfo, Serializable)
	DECLARE_VAR(_T("TournamentTableInfo"),		_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T(""),	VariableType::VT_Int32, offsetof(TournamentTableInfo,m_nTableId), false, false, false)
	DECLARE_VAR(_T("PlayerCt"),					_T(""),	VariableType::VT_Int8, offsetof(TournamentTableInfo,m_cPlayerCt), false, false, false)
	DECLARE_VAR(_T("StakeLarge"),				_T(""),	VariableType::VT_Int32, offsetof(TournamentTableInfo,m_nStakeLarge), false, false, false)
	DECLARE_VAR(_T("StakeSmall"),				_T(""),	VariableType::VT_Int32, offsetof(TournamentTableInfo,m_nStakeSmall), false, false, false)
	DECLARE_VAR(_T("Players"),					_T(""),	VariableType::VT_None, offsetof(TournamentTableInfo,m_players), false, true, true)
END_DECLARE_VAR()

TournamentTableInfo::~TournamentTableInfo(){
	}

////////////////////////////////////////////
//	class LobbyTournamentLevelInfo
//
LobbyTournamentLevelInfo::LobbyTournamentLevelInfo(){
	}

BEGIN_DECLARE_VAR(LobbyTournamentLevelInfo, Serializable)
	DECLARE_VAR(_T("LobbyTournamentLevelInfo"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("LevelIndex"),				_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentLevelInfo,m_cLevelIndex), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),				_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentLevelInfo,m_nSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),					_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentLevelInfo,m_nBigBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),						_T(""),	VariableType::VT_Int32, offsetof(LobbyTournamentLevelInfo,m_nAnte), false, false, false)
END_DECLARE_VAR()

LobbyTournamentLevelInfo::~LobbyTournamentLevelInfo(){
	}

////////////////////////////////////////////
//	class LobbyTournamentBlindStructureInfo
//
LobbyTournamentBlindStructureInfo::LobbyTournamentBlindStructureInfo(){
	}

BEGIN_DECLARE_VAR(LobbyTournamentBlindStructureInfo, Serializable)
	DECLARE_VAR(_T("LobbyTournamentBlindStructureInfo"),	_T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("LevelDuarationMin"),					_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentBlindStructureInfo,m_cLevelDuarationMin), false, false, false)
	DECLARE_VAR(_T("BreakDurationMin"),						_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentBlindStructureInfo,m_cBreakDurationMin), false, false, false)
	DECLARE_VAR(_T("BreakPerLevel"),						_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentBlindStructureInfo,m_cBreakPerLevel), false, false, false)
	DECLARE_VAR(_T("AddonBreakIndex"),						_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentBlindStructureInfo,m_cAddonBreakIndex), false, false, false)
	DECLARE_VAR(_T("RebuyCt"),								_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentBlindStructureInfo,m_cRebuyCt), false, false, false)
	DECLARE_VAR(_T("AddonCt"),								_T(""),	VariableType::VT_Int8, offsetof(LobbyTournamentBlindStructureInfo,m_cAddonCt), false, false, false)
	DECLARE_VAR(_T("Levels"),								_T(""),	VariableType::VT_None, offsetof(LobbyTournamentBlindStructureInfo,m_listLevels), false, true, true)
END_DECLARE_VAR()

LobbyTournamentBlindStructureInfo::~LobbyTournamentBlindStructureInfo(){
	}