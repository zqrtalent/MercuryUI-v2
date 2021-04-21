#include "stdafx.h"
#include "TaskInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	Task info class.
//
/////////////////////////////////////////////////////////////////////

TaskInfo::TaskInfo(){
	}

BEGIN_DECLARE_VAR(TaskInfo, Serializable)
	DECLARE_VAR(_T("TaskInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),			_T(""), VariableType::VT_Int32, offsetof(TaskInfo,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),			_T(""), VariableType::VT_String, offsetof(TaskInfo,m_sName), false, false, false)

#ifdef _WIN64
	DECLARE_VAR(_T("ClassObj"),		_T(""), VariableType::VT_Int64, offsetof(TaskInfo,m_pClassObj), false, false, false)
	DECLARE_VAR(_T("Method"),		_T(""), VariableType::VT_Int64, offsetof(TaskInfo,m_method), false, false, false)
#else
	DECLARE_VAR(_T("ClassObj"),		_T(""), VariableType::VT_Int32, offsetof(TaskInfo,m_pClassObj), false, false, false)
	DECLARE_VAR(_T("Method"),		_T(""), VariableType::VT_Int32, offsetof(TaskInfo,m_method), false, false, false)
#endif

END_DECLARE_VAR()

TaskInfo::~TaskInfo(){
	}