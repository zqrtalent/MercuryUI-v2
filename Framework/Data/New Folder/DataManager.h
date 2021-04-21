// ===========================================================================
// About: MFC new generation data serialization class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#pragma once

#include <afxdb.h>
#include <afxmt.h>
#include "..\Serialize\Serializable.h"
#include "iODBCRst.h"

class DataManager
{
public:
	DataManager(_String	sConnString = _T(""));
	virtual ~DataManager();

public:
	bool					Open				();
	bool					IsOpen				();
	void					Close				();

	void					SetConnString		(_String	sConnString);
	_String					GetConnString		();
	_String					GetLastError		(){return m_sLastError;};

public:
	bool					LoadTable			(EnumerableObject<Serializable>* pRecordObjects,const _String sWhereClause = _T(""), const _String sOrderBy = _T(""), const _String sTop = _T(""), bool bUseDbFieldNames = true);
	bool					LoadTableBySql		(EnumerableObject<Serializable>* pRecordObjects,const _String sSQL,  bool bUseDbFieldNames = true);
	bool					RefreshRecord		(Serializable* pRecord);
	bool					SaveRecord			(Serializable* pRecord, CStringVector* pArrDirtyFieldNames = NULL);
	bool					DeleteRecord		(Serializable* pRecord);
	bool					ExecuteProc			(Serializable* pProcDesc);

	void					SetDatabaseType		(DatabaseType databaseType){m_databaseType = databaseType;};
	DatabaseType			GetDatabaseType		(){return m_databaseType;};

protected: 
	CDatabase*			m_pDb;
	_String				m_sConnectionString;
	bool				m_bIsOpen;
	_String				m_sLastError;
	DatabaseType		m_databaseType;
	CCriticalSection	m_lock;
};