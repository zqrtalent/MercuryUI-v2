// ===========================================================================
// About: MFC new generation data serialization class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#include "stdafx.h"
#include "DataManager.h"

// Driver={SQL Native Client};Server=192.168.1.222;Database=ESPokerDB; Trusted_Connection=yes;Uid=sa;Pwd=adl;
// Driver={SQL Server};Server=localhost;Database=ESPokerDB; Trusted_Connection=yes;

DataManager::DataManager(_String sConnString /*= _T("")*/){
	m_sConnectionString = sConnString;
	m_pDb				= NULL;
	m_bIsOpen			= false;
	m_databaseType		= DatabaseType::MSSQL;
	}

DataManager::~DataManager(){
	Close(); // Close connection.
	}

bool
DataManager::Open(){
	if( m_sConnectionString.IsEmpty() || IsOpen() )
		return false;
	m_lock.Lock();		// Lock
	bool bRet	= true;
	m_pDb		= new CDatabase();
	try{
		m_pDb->OpenEx(m_sConnectionString, CDatabase::useCursorLib);
		m_bIsOpen = true;
		}
	catch(CDBException* e){
		m_sLastError = e->m_strError;
		delete	m_pDb;
		m_pDb	= NULL;
		bRet	= false;
		}
	m_lock.Unlock();	// Unlock
	return bRet;
	}

bool
DataManager::IsOpen(){
	m_lock.Lock();		// Lock
	bool bRet = m_bIsOpen;
	m_lock.Unlock();	// Unlock
	return bRet;
	}

void
DataManager::Close(){
	if( IsOpen() ){
		m_lock.Lock();		// Lock
		if( m_pDb->IsOpen() ){
			m_pDb->Close();
			}
		m_bIsOpen = false;
		delete m_pDb;
		m_lock.Unlock();	// Unlock
		}
	}

void
DataManager::SetConnString(_String	sConnString){
	m_sConnectionString = sConnString;
	}

_String	
DataManager::GetConnString(){
	return m_sConnectionString;
	}

bool
DataManager::LoadTable(EnumerableObject<Serializable>* pRecordObjects, const _String sWhereClause /*= _T("")*/, const _String sOrderBy /*= _T("")*/, const _String sTop /*= _T("")*/, bool bUseDbFieldNames /*= true*/){
	if( !IsOpen() )
		return false;
	int nRecCt = iODBCRst::LoadRecords(m_pDb, pRecordObjects, m_databaseType, sWhereClause, sOrderBy, sTop, bUseDbFieldNames);
	return (nRecCt > 0);
	}

bool
DataManager::LoadTableBySql(EnumerableObject<Serializable>* pRecordObjects, const _String sSQL,  bool bUseDbFieldNames /*= true*/){
	if( !IsOpen() )
		return false;
	int nRecCt = iODBCRst::LoadRecordsBySql(m_pDb, pRecordObjects, m_databaseType, sSQL, bUseDbFieldNames);
	return (nRecCt > 0);
	}

bool	
DataManager::RefreshRecord(Serializable* pRecord){
	if( !IsOpen() )
		return false;
	return iODBCRst::RefreshRecord(m_pDb, pRecord, m_databaseType);
	}

bool
DataManager::SaveRecord(Serializable* pRecord, CStringVector* pArrDirtyFieldNames /*= NULL*/){
	if( !IsOpen() )
		return false;
	bool bRet = iODBCRst::SaveRecord(m_pDb, pRecord, m_databaseType, pArrDirtyFieldNames);
	return bRet;
	}

bool
DataManager::DeleteRecord(Serializable* pRecord){
	if( !IsOpen() )
		return false;
	bool bRet = iODBCRst::DeleteRecord(m_pDb, pRecord, m_databaseType);
	return bRet;
	}

bool
DataManager::ExecuteProc(Serializable* pProcDesc){
	if( !IsOpen() )
		return false;
	return iODBCRst::ExecuteProcedure(m_pDb, pProcDesc, m_databaseType);
	}