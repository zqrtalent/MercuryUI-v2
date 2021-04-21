#pragma once

/////////////////////////////////////////////////////////////////////////////
// LobbyTournamentInfo class

#include "..\..\Serialize\Serializable.h"
#include "..\..\Protocol\_PokerDefines.h"
#include "..\..\Array\List.h"
#include "..\..\Array\AutoSortedArrayInt64.h"
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
	std::string											m_sName;					// Tournament name.
	std::string											m_sDescription;				// Tournament description.

	DateTime											m_dtTimeTournamentStart;	// Tournament start time.
	DateTime											m_dtTimeTournamentFinished;	// Time finished.
	DateTime											m_dtTimeRegistrationStart;	// Registration start time.
	DateTime											m_dtTimeRegistrationEnd;	// Registration end time.

	char												m_cGameType;				// One of the GameType.
	short												m_shSubType;				// One of the GameSubType.
	char												m_cCurrencyId;				// Tournament currency.

	// TournamentStatus
	char												m_status;					// Tournament status.
	int													m_nBuyinAmount;				// Buyin amount.
	int													m_nFeeAmount;				// Buyin fee.
	int													m_nBuyInPoints;				// Buyin points.
	
	short												m_shStartingChips;			// Starting chips.

	int													m_nRebuyAmount;				// Rebuy amount.
	short												m_shRebuyChips;				// Rebuy chips.
	int													m_nAddonAmount;				// Addon amount.
	short												m_shAddonChips;				// Addon chips.

	char												m_cRebuyCt;					// Rebuy count.
	char												m_cAddonCt;					// Addon count.

	int													m_nPrizeAmount;				// Prize amount guaranteed.
	int													m_nPrizeTicketId;			// Prize ticket id. (Tournament Ticket Id)

	short												m_shMinPlayers;				// Min players.
	int													m_nMaxPlayers;				// Max players.

	int													m_nRegisteredCt;			// Registered players count.
	int													m_nPlayersCt;				// Playing players count.
	short												m_shTableCt;				// Game table count.		

protected:
	EnumerableObject<TournamentTableInfo>				m_listTables;				// List of tournament game tables.
	EnumerableObject<TournamentPlayerInfo>				m_listParticipants;			// List of tournament participant players.
	EnumerableObject<LobbyTournamentBlindStructureInfo>	m_listBlindStructure;		// List of blind structure infos.
	EnumerableObject<LobbyTournamentPayoutInfo>			m_listPayoutInfo;			// Tournament payout info.

public:
	int		GetPlayersPlace						(int nUserId);
	bool	SetParticipantChipsAmountAndPlace	(int nUserId, int nChipsAmount, int nPlace = -1);

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
	int											m_nTournamentId;			// Tournament id.
	char										m_status;					// Tournament status.
	DateTime									m_dtTimeTournamentFinished;	// Time finished.
	int											m_nRegisteredCt;			// Registered players count.
	int											m_nPlayersCt;				// Playing players count.
	short										m_shTableCt;				// Game table count.					

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
	std::string					m_sUserName;
	int							m_nChips;
	short						m_shPlace;
	int							m_nOrder;	// Need to order participants.

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
	int											m_nStakeLarge;
	int											m_nStakeSmall;

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
	int							m_nSmallBlind;
	int							m_nBigBlind;
	int							m_nAnte;

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

