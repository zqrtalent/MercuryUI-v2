#pragma once
#include "../../../Serialize/Serializable.h"

class TournamentGameTable : public Serializable
{
public:
	TournamentGameTable();
	virtual ~TournamentGameTable();

public:
	int					m_nId;
	int					m_nTableId;
	int					m_nTournamentId;
	LongBinary			m_binTableInfo;		// PokerTableInfo structure in serialized state.
	char				m_cPlayerCt;

protected:
	virtual Serializable*						CreateSerializableObject(){return new TournamentGameTable();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentGameTable>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
