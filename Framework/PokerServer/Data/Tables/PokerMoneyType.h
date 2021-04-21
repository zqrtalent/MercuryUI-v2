#pragma once
#include "../../../Serialize/Serializable.h"

class PokerMoneyType : public Serializable
{
public:
	PokerMoneyType();
	virtual ~PokerMoneyType();

protected:
	int			m_nId;				// Id.
	_String		m_sName;			// Money type name.

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerMoneyType();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerMoneyType>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
