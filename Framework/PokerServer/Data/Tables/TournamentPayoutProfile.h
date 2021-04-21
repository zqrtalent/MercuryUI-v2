#pragma once
#include "../../../Serialize/Serializable.h"
#include "TournamentPayout.h"

class TournamentPayoutProfile : public Serializable
{
public:
	TournamentPayoutProfile();
	virtual ~TournamentPayoutProfile();

public:
	int											m_nId;					// Id.
	_String										m_sName;				// Profile name.
	EnumerableObject<TournamentPayout>			m_listPayoutEntries;	// List payout entries.

public:
	virtual Serializable*						CreateSerializableObject(){return new TournamentPayoutProfile();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentPayoutProfile>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};