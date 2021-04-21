#include "stdafx.h"
#include "TournamentInfo.h"
#include "../../Protocol/_PokerDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TournamentInfo::TournamentInfo(){
	// Create variable indexes.
	m_listParticipants.CreateVariableIndex(_T("Id"), (CompareProc)Serializable::Compare_Int32);
	m_listParticipants.CreateVariableIndex(_T("UserId"), (CompareProc)Serializable::Compare_Int32);

	m_nPlayersCtFinished	= 0;
	m_nPayoutRegistrants	= 0;
	m_pCurrentPayout		= NULL;
	}

TournamentInfo::~TournamentInfo(){
	}

BEGIN_DECLARE_VAR(TournamentInfo, Serializable)
	DECLARE_VAR(_T("TournamentInfo"),		_T("TB_TOURNAMENT"),						VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),					_T("ID"),									VariableType::VT_Int32, offsetof(TournamentInfo,m_nId), false, false, false)
	DECLARE_VAR(_T("GameTypeId"),			_T("GAME_TYPE_ID"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nGameTypeId), false, false, false)
	DECLARE_VAR(_T("SubTypeId"),			_T("SUB_TYPE_ID"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nSubTypeId), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),			_T("CURRENCY_ID"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("GameSpeedId"),			_T("GAME_SPEED_ID"),						VariableType::VT_Int32, offsetof(TournamentInfo,m_nGameSpeedId), false, false, false)
	DECLARE_VAR(_T("Status"),				_T("STATUS_ID"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nStatus), false, false, false)
	DECLARE_VAR(_T("BlindStructProfileId"),	_T("BLIND_STRUCTURE_PROFILE_ID"),			VariableType::VT_Int32, offsetof(TournamentInfo,m_nBlindStructProfileId), false, false, false)
	DECLARE_VAR(_T("PayoutProfileId"),		_T("PAYOUT_PROFILE_ID"),					VariableType::VT_Int32, offsetof(TournamentInfo,m_nPayoutProfileId), false, false, false)

	DECLARE_VAR(_T("Name"),					_T("NAME"),									VariableType::VT_String, offsetof(TournamentInfo,m_sName), false, false, false)
	DECLARE_VAR(_T("Description"),			_T("DESCRIPTION"),							VariableType::VT_String, offsetof(TournamentInfo,m_sDescription), false, false, false)
	DECLARE_VAR(_T("StartTime"),			_T("START_DTIME"),							VariableType::VT_DateTime, offsetof(TournamentInfo,m_dtStartTime), false, false, false)
	DECLARE_VAR(_T("TimeFinished"),			_T("FINISHED_DTIME"),						VariableType::VT_DateTime, offsetof(TournamentInfo,m_dtFinished), false, false, false)
	DECLARE_VAR(_T("ShowTime"),				_T("SHOW_DTIME"),							VariableType::VT_DateTime, offsetof(TournamentInfo,m_dtShowTime), false, false, false)
	DECLARE_VAR(_T("HiddenStatus"),			_T("HIDDEN_STATUS"),						VariableType::VT_Bool, offsetof(TournamentInfo,m_bHiddenStatus), false, false, false)
	DECLARE_VAR(_T("HideTime"),				_T("HIDE_DTIME"),							VariableType::VT_DateTime, offsetof(TournamentInfo,m_dtHideTime), false, false, false)

	DECLARE_VAR(_T("RegStartTime"),			_T("REG_START_DTIME"),						VariableType::VT_DateTime, offsetof(TournamentInfo,m_dtRegStartTime), false, false, false)
	DECLARE_VAR(_T("RegEndTime"),			_T("REG_END_DTIME"),						VariableType::VT_DateTime, offsetof(TournamentInfo,m_dtRegEndTime), false, false, false)

	DECLARE_VAR(_T("MinPlayers"),			_T("MIN_PLAYERS"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nMinPlayers), false, false, false)
	DECLARE_VAR(_T("MaxPlayers"),			_T("MAX_PLAYERS"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nMaxPlayers), false, false, false)
	DECLARE_VAR(_T("ActionTimeSec"),		_T("ACTION_TIME_SEC"),						VariableType::VT_Int16, offsetof(TournamentInfo,m_shActionTimeSec), false, false, false)
	DECLARE_VAR(_T("TimeBankInitTimeSec"),	_T("TIMEBANK_INITIAL_SEC"),					VariableType::VT_Int16, offsetof(TournamentInfo,m_shTimeBankInitTimeSec), false, false, false)
	DECLARE_VAR(_T("TimeBankIncLevel"),		_T("TIMABANK_INC_LEVEL"),					VariableType::VT_Int16, offsetof(TournamentInfo,m_shTimeBankIncLevel), false, false, false)
	DECLARE_VAR(_T("TimeBankIncSec"),		_T("TIMEBANK_INC_SEC"),						VariableType::VT_Int16, offsetof(TournamentInfo,m_shTimeBankIncSec), false, false, false)
	DECLARE_VAR(_T("MaxSeatCt"),			_T("MAX_SEAT_CT"),							VariableType::VT_Int16, offsetof(TournamentInfo,m_shMaxSeatCt), false, false, false)
	DECLARE_VAR(_T("LevelDuration"),		_T("LEVEL_DURATION_MIN"),					VariableType::VT_Int16, offsetof(TournamentInfo,m_shLevelDuration), false, false, false)
	DECLARE_VAR(_T("OpenTableBeforeStartSec"),_T("OPEN_TABLE_BEFORE_START_SEC"),		VariableType::VT_Int16, offsetof(TournamentInfo,m_shOpenTableBeforeStartSec), false, false, false)

	DECLARE_VAR(_T("CurrentLevelIndex"),	_T("CURRENT_LEVEL_INDEX"),					VariableType::VT_Int32, offsetof(TournamentInfo,m_nCurrentLevelIndex), false, false, false)

	DECLARE_VAR(_T("BuyIn"),				_T("BUYIN_AMOUNT"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dBuyIn), false, false, false)
	DECLARE_VAR(_T("BuyInVPP"),				_T("BUYIN_VPPOINTS"),						VariableType::VT_Double, offsetof(TournamentInfo,m_dBuyInVPP), false, false, false)
	DECLARE_VAR(_T("BuyInFee"),				_T("BUYIN_FEE"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dBuyInFee), false, false, false)
	DECLARE_VAR(_T("BuyInChips"),			_T("BUYIN_CHIPS"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nBuyInChips), false, false, false)

	DECLARE_VAR(_T("BreakPerLevel"),		_T("BREAK_PER_LEVEL"),						VariableType::VT_Int16, offsetof(TournamentInfo,m_shBreakPerLevel), false, false, false)
	DECLARE_VAR(_T("BreakTimeMinute"),		_T("BREAK_TIME_MINUTE"),					VariableType::VT_Int8, offsetof(TournamentInfo,m_cBreakTimeMinute), false, false, false)

	DECLARE_VAR(_T("hBreakDurationMin"),	_T("BREAK_DURATION_MIN"),					VariableType::VT_Int16, offsetof(TournamentInfo,m_shBreakDurationMin), false, false, false)
	DECLARE_VAR(_T("Rebuy"),				_T("REBUY_AMOUNT"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dRebuy), false, false, false)
	DECLARE_VAR(_T("RebuyVPP"),				_T("REBUY_VPPOINTS"),						VariableType::VT_Double, offsetof(TournamentInfo,m_dRebuyVPP), false, false, false)
	DECLARE_VAR(_T("RebuyFee"),				_T("REBUY_FEE"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dRebuyFee), false, false, false)
	DECLARE_VAR(_T("RebuyChips"),			_T("REBUY_CHIPS"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nRebuyChips), false, false, false)

	DECLARE_VAR(_T("Addon"),				_T("ADDON_AMOUNT"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dAddon), false, false, false)
	DECLARE_VAR(_T("AddonVPP"),				_T("ADDON_VPPOINTS"),						VariableType::VT_Double, offsetof(TournamentInfo,m_dAddonVPP), false, false, false)
	DECLARE_VAR(_T("AddonFee"),				_T("ADDON_FEE"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dAddonFee), false, false, false)
	DECLARE_VAR(_T("AddonChips"),			_T("ADDON_CHIPS"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nAddonChips), false, false, false)
	DECLARE_VAR(_T("AddonBreak"),			_T("ADDON_BREAK"),							VariableType::VT_Int16, offsetof(TournamentInfo,m_shAddonBreak), false, false, false)

	DECLARE_VAR(_T("RebuyCt"),				_T("REBUY_CT"),								VariableType::VT_Int16, offsetof(TournamentInfo,m_shRebuyCt), false, false, false)
	DECLARE_VAR(_T("AddonCt"),				_T("ADDON_CT"),								VariableType::VT_Int16, offsetof(TournamentInfo,m_shAddonCt), false, false, false)

	DECLARE_VAR(_T("PrizeAmount"),			_T("PRIZE_AMOUNT"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dPrizeAmount), false, false, false)
	DECLARE_VAR(_T("SatelliteToId"),		_T("PRIZE_TICKET_TOURNAMENT_ID"),			VariableType::VT_Int32, offsetof(TournamentInfo,m_nSatelliteToId), false, false, false)
	DECLARE_VAR(_T("TicketPrice"),			_T("PRIZE_TICKET_PRICE"),					VariableType::VT_Double, offsetof(TournamentInfo,m_dTicketPrice), false, false, false)
	DECLARE_VAR(_T("GuaranteedTicketCt"),	_T("GUARANTEED_TICKET_CT"),					VariableType::VT_Int16, offsetof(TournamentInfo,m_shGuaranteedTicketCt), false, false, false)

	DECLARE_VAR(_T("PermitSameIPReg"),		_T("PERMIT_SAME_IP_REGISTRATION"),			VariableType::VT_Bool, offsetof(TournamentInfo,m_bPermitSameIPRegistration), false, false, false)
	DECLARE_VAR(_T("AllowToUseVPPoints"),	_T("ALLOW_TO_USE_VPPOINTS"),				VariableType::VT_Bool, offsetof(TournamentInfo,m_bAllowToUseVPPoints), false, false, false)

	DECLARE_VAR(_T("RecState"),				_T("REC_STATE"),							VariableType::VT_Int16, offsetof(TournamentInfo,m_shRecState), false, false, false)
	
	// Post tournament attributes.
	DECLARE_VAR(_T("EntriesCt"),			_T("COUNT_ENTRIES"),						VariableType::VT_Int32, offsetof(TournamentInfo,m_nEntriesCt), false, false, false)
	DECLARE_VAR(_T("RebuysCt"),				_T("COUNT_REBUYS"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nRebuysCt), false, false, false)
	DECLARE_VAR(_T("AddonsCt"),				_T("COUNT_ADDONS"),							VariableType::VT_Int32, offsetof(TournamentInfo,m_nAddonsCt), false, false, false)
	DECLARE_VAR(_T("PrizePool"),			_T("PRIZE_POOL"),							VariableType::VT_Double, offsetof(TournamentInfo,m_dPrizePool), false, false, false)

	// Sub select attached to the table.
	DECLARE_VAR(_T("BlindStructure"),		_T(""),										VariableType::VT_None, offsetof(TournamentInfo,m_listBlindStructure), false, true, true)
	DECLARE_VAR(_T("ParticipantsList"),		_T("ID=TOURNAMENT_ID"),						VariableType::VT_None, offsetof(TournamentInfo,m_listParticipants), false, true, true)
	DECLARE_VAR(_T("GameTables"),			_T("ID=TOURNAMENT_ID"),						VariableType::VT_None, offsetof(TournamentInfo,m_listGameTables), false, true, true)
	DECLARE_VAR(_T("GameSpeed"),			_T("GAME_SPEED_ID=ID"),						VariableType::VT_None, offsetof(TournamentInfo,m_gameSpeed), false, true, false)
	DECLARE_VAR(_T("PayoutEntries"),		_T(""),										VariableType::VT_None, offsetof(TournamentInfo,m_listPayoutEntries), false, true, true)
END_DECLARE_VAR()


const TournamentBlindStructure* 
TournamentInfo::GetLevelInfo(int nLevelIndex){
	if( nLevelIndex >= m_listBlindStructure.GetCount() )
		return NULL;
	return m_listBlindStructure.GetAt(nLevelIndex);
	}

double
TournamentInfo::CalcPrizePool(){
	return max(m_dPrizeAmount, (m_nEntriesCt*m_dBuyIn + m_nRebuysCt*m_dRebuy + m_nAddonsCt*m_dAddon));
	}

bool
TournamentInfo::PlayerFinished(int nUserId, int& nPlace, double& dAmountWon, TournamentParticipant*& ppParticipant, bool& bHandForHand){
	if( m_nPlayersCtFinished >= m_listParticipants.GetCount() )
		return false; // All participants allready finished tournament.
	if( !m_listPayoutEntries.GetCount() )
		return false; // Payout info not attached to tournament.

	bool					bSatellite		= ((m_nSubTypeId & GameSubType::Tournament_Satellite) > 0);
	int						nTournamentTicketIdWon	= 0;
	int						nParticipantsCt = m_listParticipants.GetCount();
	TournamentParticipant*	pParticipant	= m_listParticipants.FindOneVariable(_T("UserId"), &nUserId);
	if( !pParticipant )
		return false; // Participant not found.

	bHandForHand						= false;
	dAmountWon							= 0.0;
	nPlace								= (nParticipantsCt - m_nPlayersCtFinished);
	m_nPlayersCtFinished ++;

	if( !bSatellite ){
		TournamentPayout*		pPayout	= GetPayout(nParticipantsCt);	
		if( !pPayout ){
			ppParticipant				= pParticipant;
			pParticipant->m_nTournamentTicketIdWon = 0;
			pParticipant->m_dPrizeWon	= 0.0;
			pParticipant->m_nPlace		= nPlace;
			pParticipant->m_bIsActive	= false; // Finished tournament.
			return true;
			}

		int	nCt							= pPayout->m_listDetail.GetCount();
		int nFinishingPlaceMax			= 0; // Finishing place to earn smallest amount.
		EnumerableObject<TournamentPayoutDetail>* pListDetail = &pPayout->m_listDetail;

		for(int i=0; i<pListDetail->GetCount(); i++){
			TournamentPayoutDetail* pDetail = pListDetail->GetAt(i);
			if( !pDetail ) continue;
			nFinishingPlaceMax	= max(nFinishingPlaceMax, pDetail->m_nFinishingEnd);
			if( pDetail->m_nFinishingStart <= nPlace && nPlace <= pDetail->m_nFinishingEnd ){
				dAmountWon		= CalcPrizePool() * (pDetail->m_dPercentWin / 100.0);
				//Eliminate bubbles.
				//bHandForHand	= (pDetail->m_nFinishingStart == nPlace);
				break;
				}
			}

		// Before finishing player enter into payout places.
		if( nFinishingPlaceMax == (nPlace - 1) && nFinishingPlaceMax > m_shMaxSeatCt )
			bHandForHand = true;
		// Before last final table.
		if( (nPlace - 1) > m_shMaxSeatCt && (nPlace - 1) < 2*m_shMaxSeatCt )
			bHandForHand = true;
		}
	else{
		double	dPrizePool			= CalcPrizePool();
		int		nPrizeTicketsCt		= max(m_shGuaranteedTicketCt, (int)(dPrizePool / m_dTicketPrice));
		double	dAmountRemnant		= (nPrizeTicketsCt > m_shGuaranteedTicketCt ? (dPrizePool - (nPrizeTicketsCt*m_dTicketPrice)) : 0.0);
		
		if( (nPlace - 2) <= nPrizeTicketsCt ){
			if( (nPlace - 1) <= nPrizeTicketsCt ){
				if( (nPlace - 1) == nPrizeTicketsCt )
					dAmountWon	= dAmountRemnant;
				else
					nTournamentTicketIdWon = m_nSatelliteToId;
				}
			else
				bHandForHand	= true;
			}
		}

	ppParticipant				= pParticipant;
	pParticipant->m_nTournamentTicketIdWon = nTournamentTicketIdWon;
	pParticipant->m_dPrizeWon	= dAmountWon;
	pParticipant->m_nPlace		= nPlace;
	pParticipant->m_bIsActive	= false; // Finished tournament.
	return true;

	/*
	if( m_nPlayersCtFinished >= m_listParticipants.GetCount() )
		return false; // All participants allready finished tournament.
	if( !m_listPayoutEntries.GetCount() )
		return false; // Payout info not attached to tournament.

	int						nParticipantsCt = m_listParticipants.GetCount();
	TournamentPayout*		pPayout			= GetPayout(nParticipantsCt);	
	TournamentParticipant*	pParticipant	= m_listParticipants.FindOneVariable(_T("UserId"), &nUserId);
	if( !pParticipant )
		return false; // Participant not found.

	bHandForHand							= false;
	dAmountWon								= 0.0;
	nPlace									=  (nParticipantsCt - m_nPlayersCtFinished);
	m_nPlayersCtFinished ++;

	if( !pPayout ){
		pParticipant->m_nTournamentTicketIdWon = 0;
		ppParticipant				= pParticipant;
		pParticipant->m_dPrizeWon	= 0.0;
		pParticipant->m_nPlace		= nPlace;
		pParticipant->m_bIsActive	= false; // Finished tournament.
		return true;
		}

	int	nCt							= pPayout->m_listDetail.GetCount();
	int nFinishingPlaceMax			= 0; // Finishing place to earn smallest amount.
	EnumerableObject<TournamentPayoutDetail>* pListDetail = &pPayout->m_listDetail;

	for(int i=0; i<pListDetail->GetCount(); i++){
		TournamentPayoutDetail* pDetail = pListDetail->GetAt(i);
		if( !pDetail ) continue;
		nFinishingPlaceMax	= max(nFinishingPlaceMax, pDetail->m_nFinishingEnd);
		if( pDetail->m_nFinishingStart <= nPlace && nPlace <= pDetail->m_nFinishingEnd ){
			dAmountWon		= CalcPrizePool() * (pDetail->m_dPercentWin / 100.0);
			//Eliminate bubbles.
			//bHandForHand	= (pDetail->m_nFinishingStart == nPlace);
			break;
			}
		}

	// Before finishing player enter into payout places.
	if( nFinishingPlaceMax == (nPlace - 1) && nFinishingPlaceMax > m_shMaxSeatCt ){
		bHandForHand = true;
		}

	// Before last final table.
	if( (nPlace - 1) > m_shMaxSeatCt && (nPlace - 1) < 2*m_shMaxSeatCt ){
		bHandForHand = true;
		}

	ppParticipant				= pParticipant;
	pParticipant->m_nTournamentTicketIdWon = 0;
	pParticipant->m_dPrizeWon	= dAmountWon;
	pParticipant->m_nPlace		= nPlace;
	pParticipant->m_bIsActive	= false; // Finished tournament.
	return true;
	*/
	}

TournamentPayout*
TournamentInfo::GetPayout(int nRegistrantCt){
	if( m_pCurrentPayout ){
		if( nRegistrantCt >= m_pCurrentPayout->m_nEntriesMin && nRegistrantCt <= m_pCurrentPayout->m_nEntriesMax )
			return m_pCurrentPayout;
		}

	TournamentPayout*		pPayout			= NULL;
	int						nLoop			= 0;
	while( nLoop < m_listPayoutEntries.GetCount() ){
		pPayout	= m_listPayoutEntries.GetAt(nLoop);
		if( !pPayout ){
			nLoop ++;
			continue;
			}
		if( nRegistrantCt >= pPayout->m_nEntriesMin && nRegistrantCt <= pPayout->m_nEntriesMax ){
			m_nPayoutRegistrants	= nRegistrantCt;
			m_pCurrentPayout		= pPayout;
			return pPayout;
			}
		nLoop ++;
		}
	return NULL;
	}