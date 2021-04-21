#pragma once

/////////////////////////////////////////////////////////////////////////////
// LobbyTournamentInfo class

#include "../../Serialize/Serializable.h"
#include "../../Protocol/_PokerDefines.h"
#include "../../Array/List.h"
#include "../../Array/AutoSortedArrayInt64.h"
#include "LobbyTournamentPayoutInfo.h"

class TournamentTableInfo;
class TournamentPlayerInfo;
class LobbyTournamentLevelInfo;
class LobbyTournamentBlindStructureInfo;
class TournamentInfo;

class LobbyTournamentInfo : public Serializable
{
public:
	LobbyTournamentInfo();
	~LobbyTournamentInfo();

	friend class LobbyInfo;

public:
	int													m_nTournamentId;			// Tournament id.
	_String												m_sName;					// Tournament name.
	_String												m_sDescription;				// Tournament description.

	DateTime											m_dtTimeTournamentStart;	// Tournament start time.
	DateTime											m_dtTimeTournamentFinished;	// Time finished.
	DateTime											m_dtTimeRegistrationStart;	// Registration start time.
	DateTime											m_dtTimeRegistrationEnd;	// Registration end time.

	char												m_cGameType;				// One of the GameType.
	short												m_shSubType;				// One of the GameSubType.
	char												m_cCurrencyId;				// Tournament currency.

	char												m_status;					// Tournament status (TournamentStatus).
	bool												m_bRegistrationIsOpen;		// Registration open flag.
	bool												m_bAllowedToUseVPPoints;	// Permission flag to use points.

	int													m_nBuyinAmount;				// Buyin amount (Full amount (including fee amount) needed to make registration).
	int													m_nFeeAmount;				// Buyin fee.
	
	short												m_shStartingChips;			// Starting chips.

	int													m_nRebuyAmount;				// Rebuy amount.
	short												m_shRebuyChips;				// Rebuy chips.
	int													m_nAddonAmount;				// Addon amount.
	short												m_shAddonChips;				// Addon chips.

	char												m_cRebuyCt;					// Rebuy count (Available to do).
	char												m_cAddonCt;					// Addon count (Available to do).

	int													m_nPrizeAmount;				// Prize amount guaranteed.
	int													m_nSatelliteToId;			// Main tournament id. (satellite tournaments)
	_String												m_sSatelliteToName;			// Satellite to tournament name.
	DateTime											m_dtSatelliteToStart;		// Satellite to tournament start time.

	short												m_shMinPlayers;				// Min players.
	int													m_nMaxPlayers;				// Max players.

	int													m_nRegisteredCt;			// Registered players count.
	int													m_nPlayersCt;				// Playing players count.
	short												m_shTableCt;				// Game table count.		

protected:
	EnumerableObject<TournamentTableInfo>				m_listTables;				// List of tournament game tables.
	EnumerableObject<LobbyTournamentBlindStructureInfo>	m_listBlindStructure;		// List of blind structure infos.
	EnumerableObject<LobbyTournamentPayoutInfo>			m_listPayoutInfo;			// Tournament payout info.

private:
	EnumerableObject<TournamentPlayerInfo>				m_listParticipants;			// List of tournament participant players.
	//CCriticalSection									m_lock;

protected:
	int		GetPlayersPlace						(int nUserId);
	bool	SetParticipantChipsAmountAndPlace	(int nUserId, INT_CHIPS nChipsAmount, int nPlace = -1);

public:
	virtual Serializable* CreateSerializableObject(){return new LobbyTournamentInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class LobbyTournamentDynamicInfo : public Serializable
{
public:
	LobbyTournamentDynamicInfo();
	~LobbyTournamentDynamicInfo();

public:
	int							m_nTournamentId;			// Tournament id.
	char						m_status;					// Tournament status.
	DateTime					m_dtTimeTournamentFinished;	// Time finished.
	bool						m_bRegistrationIsOpen;		// Registration flag.
	int							m_nRegisteredCt;			// Registered players count.
	int							m_nPlayersCt;				// Playing players count.
	short						m_shTableCt;				// Game tables count.
	UINT						m_nPrizePool;				// Tournament prize pool.
	int							m_nAllRebuysCt;				// Rebuys count have been made during tournamnet.
	int							m_nAllAddonsCt;				// Addons count have been made during tournamnet.

public:
	virtual Serializable* CreateSerializableObject(){return new LobbyTournamentDynamicInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TournamentPlayerInfo : public Serializable
{
public:
	TournamentPlayerInfo();
	~TournamentPlayerInfo();

public:
	char						m_cLevel;
	int							m_nUserId;
	_String						m_sUserName;
	INT_CHIPS					m_nChips;
	short						m_shPlace;
	INT_CHIPS					m_nOrder;	// Order participants.

public:
	virtual Serializable* CreateSerializableObject(){return new TournamentPlayerInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TournamentTableInfo : public Serializable
{
public:
	TournamentTableInfo();
	~TournamentTableInfo();

public:
	int											m_nTableId;
	char										m_cPlayerCt;
	INT_CHIPS									m_nStakeLarge;
	INT_CHIPS									m_nStakeSmall;
	EnumerableObject<TournamentPlayerInfo>		m_players;

public:
	virtual Serializable* CreateSerializableObject(){return new TournamentTableInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class LobbyTournamentLevelInfo : public Serializable
{
public:
	LobbyTournamentLevelInfo();
	~LobbyTournamentLevelInfo();

public:
	char						m_cLevelIndex;
	UINT						m_nSmallBlind;
	UINT						m_nBigBlind;
	UINT						m_nAnte;

public:
	virtual Serializable* CreateSerializableObject(){return new LobbyTournamentLevelInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class LobbyTournamentBlindStructureInfo : public Serializable
{
public:
	LobbyTournamentBlindStructureInfo();
	~LobbyTournamentBlindStructureInfo();

public:
	char										m_cLevelDuarationMin;		// Level duration in min.
	char										m_cBreakDurationMin;		// Break duration in min.
	char										m_cBreakPerLevel;			// Break per level.
	char										m_cAddonBreakIndex;			// Addon break index.

	char										m_cRebuyCt;					// Rebuy count.
	char										m_cAddonCt;					// Addon count.

	EnumerableObject<LobbyTournamentLevelInfo>	m_listLevels;				// Level stakes.

public:
	virtual Serializable* CreateSerializableObject(){return new LobbyTournamentBlindStructureInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

