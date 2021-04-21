#include "AppConfig.h"
#include "..\XML\XMLReader.h"
#include "..\Path\Path.h"

AppConfig::AppConfig()
	{
	m_pContent = NULL;
	}

AppConfig::~AppConfig()
	{
	if( m_pContent != NULL )
		delete m_pContent;
	}

CString
AppConfig::GetFilePath(TCHAR* tpszName, TCHAR* tszDefault /*= NULL*/)
	{
	if( !IsValid()  ){
		if( tszDefault )
			return CString(tszDefault);
		else
			return "";
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetFilePath(m_sPathToValue.GetBuffer());
	}

CString	
AppConfig::GetString(TCHAR* tpszName, TCHAR* tszDefault /*= NULL*/)
	{
	if( !IsValid()  ){
		if( tszDefault )
			return CString(tszDefault);
		else
			return "";
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetString(m_sPathToValue.GetBuffer());
	}

int
AppConfig::GetStringArr(TCHAR* tpszName, CStringArray& arr, TCHAR delimiter /*= ';'*/)
	{
	if( !IsValid()  ){
		return 0;
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetStringArr(m_sPathToValue.GetBuffer(), arr, delimiter);
	}

long
AppConfig::GetLong(TCHAR* tpszName, long lDefault /*= 0*/)
	{
	if( !IsValid()  ){
		return lDefault;
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetLong(m_sPathToValue.GetBuffer());
	}

double	
AppConfig::GetDouble(TCHAR* tpszName,  double dDefault /*= 0.0*/)
	{
	if( !IsValid()  ){
		return dDefault;
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetDouble(m_sPathToValue.c_str());
	}

BOOL	
AppConfig::GetBool(TCHAR* tpszName, BOOL bDefault /*= FALSE*/)
	{
	if( !IsValid()  ){
		return bDefault;
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetBool(m_sPathToValue.c_str());
	}

BOOL
AppConfig::GetDate(TCHAR* tpszName, COleDateTime& dtValue)
	{
	if( !IsValid()  ){
		return FALSE;
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetDate(m_sPathToValue.c_str(), dtValue);
	}

BOOL
AppConfig::GetTime(TCHAR* tpszName, COleDateTime& dtValue)
	{
	if( !IsValid()  ){
		return FALSE;
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetTime(m_sPathToValue.c_str(), dtValue);
	}

BOOL
AppConfig::GetDateTime(TCHAR* tpszName, COleDateTime& dtValue){
	if( !IsValid()  ){
		return FALSE;
		}

	m_sPathToValue = _T("configuration/");
	m_sPathToValue += tpszName;
	return m_pContent->GetDateTime(m_sPathToValue.c_str(), dtValue);
	}

BOOL
AppConfig::Load(const CString sXmlFile){
	CString xmlFile = sXmlFile;
	m_pContent = new XMLReader();
	Path::BuildFullPath(xmlFile);
	if( !m_pContent->Load(xmlFile) ){
		delete m_pContent;
		m_pContent = NULL;
		return FALSE;
		}
	return TRUE;
	}

BOOL
AppConfig::Save(const CString sXmlFile){
	return FALSE;
	}