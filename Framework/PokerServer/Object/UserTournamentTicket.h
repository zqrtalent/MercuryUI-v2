#pragma once

/////////////////////////////////////////////////////////////////////////////
// UserTournamentTicket

#include "..\..\Serialize\Serializable.h"

class UserTournamentTicket : public Serializable
{
public:
	UserTournamentTicket();
	~UserTournamentTicket();

public:
	int				m_nTournamentId;
	_String			m_sTournamentName;
	char			m_cTicketCount;

protected:
	virtual Serializable* CreateSerializableObject(){return new UserTournamentTicket();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};