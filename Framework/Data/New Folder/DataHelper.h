// ===========================================================================
// About: MFC new generation data management class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#pragma once
#include "..\Serialize\Serializable.h"

class Database;
class DatabaseInfo;

class DatabaseList : public Serializable
{
public:
	DatabaseList	();
	virtual ~DatabaseList	();

	DatabaseInfo* GetDatabaseInfo(TCHAR* pszDbName);
protected:
	virtual Serializable* CreateSerializableObject(){return new DatabaseList(); }
	INIT_RUNTIME_VARIABLE()
	
	EnumerableObject<DatabaseInfo> m_list;
};

class DatabaseInfo : public Serializable
{
public:
	DatabaseInfo	();
	virtual ~DatabaseInfo	();

	friend DatabaseList;

public:
	Database* GetDbObject();

protected:
	virtual Serializable* CreateSerializableObject(){return new DatabaseInfo(); }
	
	INIT_RUNTIME_VARIABLE()

	_String			m_sName;
	_String			m_sDbType;
	_String			m_sSqlScriptFile;
	_String			m_sConnectionString;
	Database*		m_pDbObject;			// Database object pointer;
};
