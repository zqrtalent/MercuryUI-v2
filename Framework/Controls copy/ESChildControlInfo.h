#pragma once
#include "../Serialize/Serializable.h"

class ESChildControl;
class ESChildControlInfo : public Serializable
{
public:
	ESChildControlInfo();
	virtual ~ESChildControlInfo();

public:
	int				m_nId;				// Child control's id.
	_string		m_sName;			// Child control's name.
	_string		m_sClassName;		// Child control's class name.
	_Rect			m_rRect;			// Child Control's rect area.
	int				m_nZIndex;			// Child control's z-Index.

	ESChildControl*	m_pControl;			// Child control.

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlInfo();};

protected:
	virtual BOOL			Serialize		(GrowableMemory* pMem);
	virtual BOOL			Deserialize		(GrowableMemory* pMem);


protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
