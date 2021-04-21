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

	std::string GetFilePath		(TCHAR* tpszName, TCHAR* tszDefault = NULL);
	std::string	GetString		(TCHAR* tpszName, TCHAR* tszDefault = NULL);
	int 	GetStringArr	(TCHAR* tpszName, CStringArray& arr, TCHAR delimiter = ';');
	long	GetLong			(TCHAR* tpszName, long lDefault = 0);
	double	GetDouble		(TCHAR* tpszName,  double dDefault = 0.0);
	BOOL	GetBool			(TCHAR* tpszName, BOOL bDefault = FALSE);
	BOOL	GetDate			(TCHAR* tpszName, DateTime& dtValue);
	BOOL	GetTime			(TCHAR* tpszName, DateTime& dtValue);
	BOOL	GetDateTime		(TCHAR* tpszName, DateTime& dtValue);
public:
	
	BOOL	IsValid(){return (m_pContent != NULL);};
	BOOL	Load(const std::string sXmlFile);
	BOOL	Save(const std::string sXmlFile);

protected:
	XMLReader*		m_pContent;
	std::string		m_sPathToValue;
};