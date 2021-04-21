#pragma once 

class XMLReader;
class AppConfig
{
protected:
	AppConfig	();
	~AppConfig	();

public:
	static AppConfig* Instance(){
		static AppConfig appConf;
		return &appConf;
		}

	CString GetFilePath		(TCHAR* tpszName, TCHAR* tszDefault = NULL);
	CString	GetString		(TCHAR* tpszName, TCHAR* tszDefault = NULL);
	int 	GetStringArr	(TCHAR* tpszName, CStringArray& arr, TCHAR delimiter = ';');
	long	GetLong			(TCHAR* tpszName, long lDefault = 0);
	double	GetDouble		(TCHAR* tpszName,  double dDefault = 0.0);
	BOOL	GetBool			(TCHAR* tpszName, BOOL bDefault = FALSE);
	BOOL	GetDate			(TCHAR* tpszName, COleDateTime& dtValue);
	BOOL	GetTime			(TCHAR* tpszName, COleDateTime& dtValue);
	BOOL	GetDateTime		(TCHAR* tpszName, COleDateTime& dtValue);
public:
	
	BOOL	IsValid(){return (m_pContent != NULL);};
	BOOL	Load(const CString sXmlFile);
	BOOL	Save(const CString sXmlFile);

protected:
	XMLReader*	m_pContent;
	CString		m_sPathToValue;
};