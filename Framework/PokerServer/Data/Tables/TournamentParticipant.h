#pragma once
#include "../../../Serialize/Serializable.h"

class TournamentParticipant : public Serializable
{
public:
	TournamentParticipant();
	virtual ~TournamentParticipant();

public:
	int					m_nId;
	int					m_nTournamentId;
	int					m_nUserId;
	_String				m_sUserName;
	_String				m_sIP;
	int					m_nIp;
	double				m_dAmountChips;
	short				m_shAddonCountLeft;
	short				m_shRebuyCountLeft;
	bool				m_bIsActive;
	int					m_nPlace;
	double				m_dPrizeWon;
	int					m_nTournamentTicketIdWon;
	short				m_shRegistrationType;	// Registration type: 0-by amount, 1-by points, 2-by ticket.
	
protected:
	virtual Serializable*						CreateSerializableObject(){return new TournamentParticipant();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentParticipant>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
