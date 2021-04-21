#pragma once
#include "..\Serialize\Serializable.h"

class TaskParamObj : public Serializable
{
public:
	TaskParamObj();
	virtual ~TaskParamObj();

public:
	int					m_nTaskId;
	CString				m_sTaskName;
public:
	virtual Serializable* CreateSerializableObject(){return new TaskParamObj();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

