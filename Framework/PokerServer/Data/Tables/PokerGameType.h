#pragma once
#include "../../../Serialize/Serializable.h"

class PokerGameType : public Serializable
{
public:
	PokerGameType();
	virtual ~PokerGameType();

protected:
	int			m_nId;					// Id.
	_String		m_sGameTypeName;		// Name

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerGameType();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerGameType>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
