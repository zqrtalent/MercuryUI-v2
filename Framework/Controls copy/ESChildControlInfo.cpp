#include "ESChildControlInfo.h"
#include "ESChildControl.h"
#include "../Utility/GrowableMemory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////
//
//	class ESChildControlInfo
//
/////////////////////////////////////////////////////
ESChildControlInfo::ESChildControlInfo(){
	m_nId		= 0;
	m_rRect.SetRect(0, 0, 0, 0);
	m_nZIndex	= 0;
	m_pControl	= NULL;
	}

BEGIN_DECLARE_VAR(ESChildControlInfo, Serializable)
	DECLARE_VAR(_T("ESChildControlInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlInfo,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),					_T(""), VariableType::VT_String, offsetof(ESChildControlInfo,m_sName), false, false, false)
	DECLARE_VAR(_T("ClassName"),			_T(""), VariableType::VT_String, offsetof(ESChildControlInfo,m_sClassName), false, false, false)
	DECLARE_VAR(_T("Rect"),					_T(""), VariableType::VT_Rect, offsetof(ESChildControlInfo,m_rRect), false, false, false)
	DECLARE_VAR(_T("ZIndex"),				_T(""), VariableType::VT_Int32, offsetof(ESChildControlInfo,m_nZIndex), false, false, false)
END_DECLARE_VAR()

ESChildControlInfo::~ESChildControlInfo(){
	}

BOOL
ESChildControlInfo::Serialize(GrowableMemory* pMem){
	BOOL bRet = Serializable::Serialize(pMem);
	if( bRet ){
		if( m_pControl ){
			bool bExists = true;
			pMem->AddBytes((BYTE*)&bExists, sizeof(bool));
			m_pControl->Serialize(pMem);
			}
		else{
			bool bExists = false;
			pMem->AddBytes((BYTE*)&bExists, sizeof(bool));
			}
		}
	return bRet; 
	}

BOOL
ESChildControlInfo::Deserialize(GrowableMemory* pMem){
	BOOL bRet = Serializable::Deserialize(pMem);
	if( bRet ){
		bool bExists = false;
		if( pMem->ReadInt8((char*)&bExists) && bExists){
			// WTF??? Zack
			//m_pControl = (ESChildControl*)CRuntimeClass::CreateObject(m_sClassName);
			//if( m_pControl )
			//	m_pControl->Deserialize(pMem);
			}
		}
	return bRet; 
	}
