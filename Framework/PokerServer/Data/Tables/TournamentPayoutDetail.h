#pragma once
#include "../../../Serialize/Serializable.h"

class TournamentPayoutDetail : public Serializable
{
public:
	TournamentPayoutDetail();
	virtual ~TournamentPayoutDetail();

public:
	int					m_nId;					// Id.
	int					m_nPayoutId;
	int					m_nFinishingStart;
	int					m_nFinishingEnd;
	double				m_dPercentWin;
	
protected:
	virtual Serializable*						CreateSerializableObject(){return new TournamentPayoutDetail();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<TournamentPayoutDetail>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
