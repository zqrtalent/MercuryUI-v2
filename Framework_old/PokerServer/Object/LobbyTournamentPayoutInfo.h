#pragma once

/////////////////////////////////////////////////////////////////////////////
// LobbyTournamentPayoutInfo class

#include "..\..\Serialize\Serializable.h"
#include "..\..\Protocol\_PokerDefines.h"
#include "..\..\Array\List.h"

class LobbyTournamentPayoutDetail;

class LobbyTournamentPayoutInfo : public Serializable
{
public:
	LobbyTournamentPayoutInfo();
	~LobbyTournamentPayoutInfo();

public:
	int												m_nPrizePool;				// Prize pool.
	int												m_nEntriesCt;				// Entries count. (Registrants)
	int												m_nRebuysCt;				// Rebuy count maked.
	int												m_nAddonsCt;				// Addons count maked.
	short											m_shPlacesPaid;				// Paid places.
	EnumerableObject<LobbyTournamentPayoutDetail>	m_listDetail;				// Detail info.

public:
	virtual Serializable* CreateSerializableObject(){return new LobbyTournamentPayoutInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class LobbyTournamentPayoutDetail : public Serializable
{
public:
	LobbyTournamentPayoutDetail();
	~LobbyTournamentPayoutDetail();

public:
	short											m_shPlaceFirst;				// Place first.
	short											m_shPlaceLast;				// Prize last.
	int												m_nAmount;					// Pay amount.

public:
	virtual Serializable* CreateSerializableObject(){return new LobbyTournamentPayoutDetail();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

