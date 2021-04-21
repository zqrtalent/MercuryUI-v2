#pragma once

/////////////////////////////////////////////////////////////////////////////
// LobbyTournamentSummaryInfo class

#include "LobbyTournamentInfo.h"

class LobbyTournamentSummaryInfo : public Serializable
{
public:
	LobbyTournamentSummaryInfo();
	~LobbyTournamentSummaryInfo();

public:
	int													m_nRegisteredCt;			// Registered players count.
	int													m_nPlayersCt;				// Current players count.
	short												m_shTablesCt;				// Current tables count.
	short												m_shPlacesPaid;				// Places paid.

	char												m_cTournamentStatus;		// Tournament status.
	bool												m_bRegisteredStatus;		// Registered status of user who is waiting for response.
	bool												m_bRegistrationAllowed;		// Registration alowed status. 

	bool												m_bIsBreak;					// Is break.
	bool												m_bHandForHand;				// Hand for hand flag.
	char												m_cLevelIndex;				// Current level index.
	short												m_shSecLeftTillNextLevel;	// Seconds left till next level.
	short												m_shSecLeftTillNextBreak;	// Seconds left till next break.

	int													m_nLargestStake;			// Largest stake.
	int													m_nAverageStake;			// Average stake.
	int													m_nSmallestStake;			// Smallest stake.

	EnumerableObject<LobbyTournamentBlindStructureInfo>	m_listBlindStructure;		// List of blind structure infos.
	EnumerableObject<TournamentTableInfo>				m_listTables;				// List of tournament game tables.
	EnumerableObject<TournamentPlayerInfo>				m_listTablePlayers;			// List of tournament table players.
	EnumerableObject<TournamentPlayerInfo>				m_listParticipants;			// List of tournament participant players.
	EnumerableObject<LobbyTournamentPayoutInfo>			m_listPayoutInfo;			// Tournament payout info.

protected:
	virtual Serializable* CreateSerializableObject(){return new LobbyTournamentSummaryInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};