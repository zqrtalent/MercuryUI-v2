#pragma once
#include "TournamentBlindStructure.h"

class TournamentBlindStructureProfile : public Serializable
{
public:
	TournamentBlindStructureProfile();
	virtual ~TournamentBlindStructureProfile();

public:
	int											m_nId;							// Id.
	_String										m_sName;						// Profile name.

	EnumerableObject<TournamentBlindStructure>	m_listBlindStructureEntries;

protected:
	virtual Serializable*						CreateSerializableObject(){return new TournamentBlindStructureProfile();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentBlindStructureProfile>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
