#pragma once
#include "../../../Serialize/Serializable.h"
#include "TournamentPayoutDetail.h"

class TournamentPayout : public Serializable
{
public:
	TournamentPayout();
	virtual ~TournamentPayout();

public:
	int											m_nId;					// Id.
	int											m_nProfileId;			// Payout profile id.
	int											m_nEntriesMin;			// Min entries.
	int											m_nEntriesMax;			// Max entries.

	EnumerableObject<TournamentPayoutDetail>	m_listDetail;			// List payout detail.

public:
	virtual Serializable*						CreateSerializableObject(){return new TournamentPayout();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentPayout>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};