#pragma once
#include "..\Serialize\Serializable.h"
#include "..\Utility\ThisCallHelper.h"

class TaskInfo : public Serializable
{
public:
	TaskInfo();
	virtual ~TaskInfo();

	friend class TaskRunner;

public:
	int					m_nId;
	CString				m_sName;

protected:
	void*				m_pClassObj;
	TaskHandlerMethod	m_method;

public:
	virtual Serializable* CreateSerializableObject(){return new TaskInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

