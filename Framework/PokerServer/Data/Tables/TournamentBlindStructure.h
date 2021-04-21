#pragma once
#include "../../../Serialize/Serializable.h"

class TournamentBlindStructure : public Serializable
{
public:
	TournamentBlindStructure();
	virtual ~TournamentBlindStructure();

public:
	int					m_nId;					// Id.
	double				m_dSmallBlind;			// Small blind amount.
	double				m_dBigBlind;			// Big blind amount.
	double				m_dAnte;				// Ante amount.

public:
	virtual Serializable*						CreateSerializableObject(){return new TournamentBlindStructure();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentBlindStructure>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
