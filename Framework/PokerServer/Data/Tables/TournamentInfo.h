#pragma once
#include "../../../Serialize/Serializable.h"
#include "TournamentParticipant.h"
#include "TournamentBlindStructure.h"
#include "TournamentGameTable.h"
#include "PokerGameSpeed.h"
#include "TournamentPayout.h"

class TournamentInfo : public Serializable
{
public:
	TournamentInfo();
	virtual ~TournamentInfo();

public:
	int											m_nId;							// Id.
	int											m_nGameTypeId;					// Game type id.
	int											m_nSubTypeId;					// Game sub type id.
	int											m_nCurrencyId;					// Currency id.
	int											m_nGameSpeedId;					// Game speed id.
	int											m_nStatus;						// Tournament status.

	int											m_nBlindStructProfileId;		// Blind structure profile identifier.
	int											m_nPayoutProfileId;				// Payout profile id.

	_String										m_sName;
	_String										m_sDescription;

	DateTime									m_dtStartTime;
	DateTime									m_dtFinished;
	DateTime									m_dtShowTime;

	bool										m_bHiddenStatus;
	DateTime									m_dtHideTime;

	DateTime									m_dtRegStartTime;
	DateTime									m_dtRegEndTime;

	int											m_nMinPlayers;
	int											m_nMaxPlayers;

	short										m_shActionTimeSec;
	short										m_shTimeBankInitTimeSec;
	short										m_shTimeBankIncLevel;
	short										m_shTimeBankIncSec;
	short										m_shMaxSeatCt;
	short										m_shLevelDuration;
	short										m_shOpenTableBeforeStartSec;
	int											m_nCurrentLevelIndex;			// Blind level index.

	double										m_dBuyIn;						// Without fee amount.
	double										m_dBuyInVPP;
	double										m_dBuyInFee;
	UINT										m_nBuyInChips;

	short										m_shBreakPerLevel;
	char										m_cBreakTimeMinute;				// Fixed minute of a hour to start tournament break.
	short										m_shBreakDurationMin;

	double										m_dRebuy;
	double										m_dRebuyVPP;
	double										m_dRebuyFee;
	UINT										m_nRebuyChips;

	double										m_dAddon;
	double										m_dAddonVPP;
	double										m_dAddonFee;
	UINT										m_nAddonChips;
	short										m_shAddonBreak;

	short										m_shRebuyCt;					// Rebuy count allowed by each user during tournament.
	short										m_shAddonCt;					// Addon count allowed by each user douring addon break.

	double										m_dPrizeAmount;
	int											m_nSatelliteToId;				// Main tournament id. (Satellite tournament)
	double										m_dTicketPrice;					// Prize tournament ticket price (Satellite tournament).
	short										m_shGuaranteedTicketCt;			// Guaranteed prize ticket count (Satellite tournament).
	short										m_shRecState;

	bool										m_bPermitSameIPRegistration;	// Permit registration of same IP addresses 
	bool										m_bAllowToUseVPPoints;			// Permission flag to use vppoints. (as buyin, rebuy, addon)

	// Post tournament parameters.
	int											m_nEntriesCt;
	int											m_nRebuysCt;
	int											m_nAddonsCt;
	double										m_dPrizePool;

	EnumerableObject<TournamentBlindStructure>	m_listBlindStructure;
	EnumerableObject<TournamentParticipant>		m_listParticipants;
	EnumerableObject<TournamentGameTable>		m_listGameTables;
	PokerGameSpeed								m_gameSpeed;
	EnumerableObject<TournamentPayout>			m_listPayoutEntries;

private:
	int											m_nPlayersCtFinished;			// Tournament finished players count.
	int											m_nPayoutRegistrants;			
	TournamentPayout*							m_pCurrentPayout;

public:
	const TournamentBlindStructure*				GetLevelInfo			(int nLevelIndex);
	double										CalcPrizePool			();
	bool										PlayerFinished			(int nUserId, int& nPlace, double& dAmountWon, TournamentParticipant*& ppParticipant, bool& bHandForHand);
	TournamentPayout*							GetPayout				(int nRegistrantCt);

protected:
	virtual Serializable*						CreateSerializableObject(){return new TournamentInfo();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentInfo>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
