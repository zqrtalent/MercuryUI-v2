#include "stdafx.h"
#include "TaskParamObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	Task param class.
//
/////////////////////////////////////////////////////////////////////

TaskParamObj::TaskParamObj(){
	}

BEGIN_DECLARE_VAR(TaskParamObj, Serializable)
	DECLARE_VAR(_T("TaskParamObj"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TaskId"),		_T(""), VariableType::VT_Int32, offsetof(TaskParamObj, m_nTaskId), false, false, false)
	DECLARE_VAR(_T("TaskName"),		_T(""), VariableType::VT_String, offsetof(TaskParamObj, m_sTaskName), false, false, false)
END_DECLARE_VAR()

TaskParamObj::~TaskParamObj(){
	}