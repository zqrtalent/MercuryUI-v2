#pragma once

/////////////////////////////////////////////////////////////////////////////
// UserTournamentDetails

#include "..\..\Serialize\Serializable.h"

class UserTournamentDetails : public Serializable
{
public:
	UserTournamentDetails();
	~UserTournamentDetails();

public:
	int				m_nTournamentId;		// Tournament id.
	INT_CHIPS		m_nStake;				// User current stake.
	int				m_nPlace;				// User current place.
	char			m_cRebuyLeft;			// Rebuy count left.
	char			m_cAddonLeft;			// Addon count left.

protected:
	virtual Serializable* CreateSerializableObject(){return new UserTournamentDetails();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};