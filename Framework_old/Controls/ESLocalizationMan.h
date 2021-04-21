#pragma once
#include "../Serialize/Serializable.h"
#include "../Array/StringToStringArray.h"
#include "../_platformCompat/PlatformCriticalSection.h"

class ESLocalizationMan
{
public:
	static ESLocalizationMan* GetInstance(){
		static ESLocalizationMan man;
		return &man;
		}

protected:
	ESLocalizationMan();
	virtual ~ESLocalizationMan();

public:
	bool			Load				(_string sFile, bool bNotifyAllWindows = true);
	static bool		ParseText			(_string& sText);
	static _string	GetLoadedFilePath	(){return ESLocalizationMan::GetInstance()->m_sFile;};
	static _string	GetLoadedFileName	(){return ESLocalizationMan::GetInstance()->m_sLoadedFileName;};

public:
	_string                 m_sFile;
	_string                 m_sLoadedFileName;
	StringToStringArray     m_arrKeyValues;
	CriticalSection         m_lock;
};
