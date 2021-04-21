#pragma once
#include "../../../Serialize/Serializable.h"

class PokerGameSubType : public Serializable
{
public:
	PokerGameSubType();
	virtual ~PokerGameSubType();

protected:
	int			m_nId;					// Id.
	_String		m_sName;				// Sub type name.
	
protected:
	virtual Serializable* CreateSerializableObject(){return new PokerGameSubType();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerGameSubType>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
