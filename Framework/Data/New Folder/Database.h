// ===========================================================================
// About: MFC new generation data management class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#pragma once

class CDatabase;
class Database;
class DataTable;
class DataField;
class DataCommand;
class DataCommandParams;
class DatabaseList;
class XMLSQLScript;

#include "../Array/AutoSortedArrayTempl.h"

// =======================
// DatabaseMan
//========================
class DatabaseMan
{
protected:
	DatabaseMan(CString sDataXml);
	~DatabaseMan();

public:
	static DatabaseMan* Instance(){
		static DatabaseMan man(_String(_T("data.xml")));
		return &man;
		}

	// Retrieve database object by name.
	Database* GetDatabase(TCHAR* pszDbName, BOOL bOpenIfNot = FALSE);

protected:
	DatabaseList* m_pDbList;
};

// =======================
// Database
//========================
class Database
{
protected: // You can't create object without manager class.
	Database();

	friend DatabaseMan;
	friend class DatabaseInfo;

public:
	~Database();

public:
	// First you must open database.
	virtual BOOL			Open			();
	// Close database.
	virtual BOOL			Close			(BOOL bCloseDataTables = TRUE);
	// Retrieves initialized DataCommand object's pointer by name.
	virtual DataCommand*	GetDataCommand	(TCHAR* pszCommandName);
	virtual CDatabase*		GetDBObject		(){return NULL;};


public:
	BOOL		IsOpen					(){return m_bIsOpen;};
	CString		GetConnectionString		(){return m_sConnectionString;};
	CString		GetSqlScriptFilePath	(){return m_sSqlScriptFile;};
	BOOL		DeleteDataCommand		(TCHAR* pszCommandName);
	int			EmptyUsedDataCommands	();

protected:
	XMLSQLScript*	m_pSqlScript;

	AutoSortedArrayTempl<UINT,DataCommand*> m_arrUsedCommands;

	_String			m_sConnectionString;
	_String			m_sSqlScriptFile;
	long			m_lStatus;
	BOOL			m_bIsOpen;
};

// =======================
// FirebirdDatabase
//========================

//#include <afxdb.h>

class FirebirdDatabase : public Database
{
protected:
	FirebirdDatabase();
	~FirebirdDatabase();

	friend DatabaseMan;
	friend class DatabaseInfo;

public:
	// First you must open database.
	virtual BOOL			Open			();
	// Close database.
	virtual BOOL			Close			(BOOL bCloseDataTables = TRUE);
	virtual CDatabase*		GetDBObject		(){return m_pDb;};

protected:
	CDatabase* m_pDb;
};

class MSSQLDatabase : public Database
{
protected:
	MSSQLDatabase();
	~MSSQLDatabase();

	friend DatabaseMan;
	friend class DatabaseInfo;

public:
	// First you must open database.
	virtual BOOL			Open			();
	// Close database.
	virtual BOOL			Close			(BOOL bCloseDataTables = TRUE);

	virtual CDatabase*		GetDBObject		(){return m_pDb;};
protected:
	CDatabase* m_pDb;
};

// =======================
// DataTable
//========================
class DataTable
{
protected:
	DataTable();
	~DataTable();

	// Friend classes.
	friend DataField;

public:

protected:
	DataField*	m_pFields;
	int			m_nFieldsCt;

};

// =======================
// DataField
//========================
class DataField
{
protected:
	DataField();
	~DataField();

	// Field type definition.
	enum FieldType 
	{
		Int = 1,
		BigInt,
		Short,
		Bool,
		Double,
		Float,
		StringA,
		StringW,
		DateTime,
		Date,
		Time,
		Binary
	};

public:
	int		GetInt(int nDefault = 0);
	int		GetBigInt(__int64 nDefault = 0);
	int		GetShort(short shDefault = 0);
	BOOL	GetBool(BOOL bDefault = FALSE);
	double	GetDouble(double dDefault = 0.0);
	float	GetFloat(float fDefault = 0.0);
	BOOL	GetString(CString& sValue);
	_String	GetString();
	BOOL	GetDateTime(COleDateTime& dtValue);
	BOOL	GetDate(COleDateTime& dtValue);
	BOOL	GetTime(COleDateTime& dtValue);
	BOOL	GetBinary(CLongBinary& binary);

	// Diagnostic methods.
	BOOL	IsNull();
	void	SetNull();

protected:
	char			m_szFieldName[255];	// Field name.
	FieldType		m_fieldType;		// Field type.
	int				m_nIndex;			// Ordinal index of field.
	DataField*		m_next;				// Next field.
};

// =======================
// DataCommandParams
//========================

class DataCommandParams
{
protected:
	DataCommandParams();
	~DataCommandParams();

	enum DateTimeParam
	{
		DateTimeValue,
		DateValue,
		TimeValue
	};

	friend class DataCommand;

public:
	void	AddParam(char* pszParam, int nValue);
	void	AddParam(char* pszParam, __int64 nValue);
	void	AddParam(char* pszParam, double dValue);
	void	AddParam(char* pszParam, bool bValue);
	void	AddParam(char* pszParam, float fValue);
	void	AddParam(char* pszParam, char* pszValue);
	void	AddParam(char* pszParam, wchar_t* pwszValue);
	void	AddParam(char* pszParam, COleDateTime dtValue, DateTimeParam valueType = DateTimeValue);
	void	AddParam(char* pszParam, void* lpBuffer, int nSize);

	void	ClearParams	();
	int		GetCount	();

protected:
};

// =======================
// DataCommand
//========================
class DataCommand
{
protected:
	DataCommand();
	~DataCommand();

	friend Database;

public:
	enum CommandType{
		QueryCommand = 0,
		UpdateCommand,
		InsertCommand,
		DeleteCommand
	};

	CommandType			GetCommandType(){return m_cmdType;};
	DataCommandParams*	GetCommandParams(){return &m_params;};

	virtual int			ExecuteCommand();
	virtual DataTable*	ExecuteQuery();

protected:
	void	SetCommandText(_String cmdText);

protected:
	CommandType			m_cmdType;	// Command type.
	_String				m_sCmdText;	// Command text.
	DataCommandParams	m_params;	// Command params.
	Database*			m_pDatabase; // Database with whom associated to.
};