// ===========================================================================
// About: MFC new generation data management class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#include "stdafx.h"
#include "Database.h"
#include "../Path/Path.h"
#include "../XML/XMLSQLScript.h"
#include "../String/StringDecoder.h"
#include "DataHelper.h"


// ============================================
// DatabaseMan 
//=============================================
DatabaseMan::DatabaseMan(CString sDataXml){
	m_pDbList = new DatabaseList();
	m_pDbList->Deserialize(sDataXml);
	}

DatabaseMan::~DatabaseMan(){
	delete m_pDbList;
	m_pDbList = NULL;
	}

Database*
DatabaseMan::GetDatabase(TCHAR* pszDbName, BOOL bOpenIfNot /*= FALSE*/){
	ASSERT(m_pDbList);
	DatabaseInfo* pInfo = m_pDbList->GetDatabaseInfo(pszDbName);
	if( pInfo ){
		Database* pDb = pInfo->GetDbObject();
		if( pDb && bOpenIfNot )
			pDb->Open();
		return pDb;
		}
	return NULL;
	}

// ============================================
// Database 
//=============================================
Database::Database(){
	m_pSqlScript	= NULL;
	m_bIsOpen		= FALSE;
	}

Database::~Database(){
	Close();
	}

BOOL
Database::Open(){
	if( m_bIsOpen )
		return FALSE;

	m_pSqlScript = new XMLSQLScript();
	m_pSqlScript->Open(m_sSqlScriptFile);
	return TRUE;
	}

DataCommand*
Database::GetDataCommand(TCHAR* pszCommandName){
	int nIndex = m_arrUsedCommands.FindValue(DecodeString(pszCommandName));
	if( nIndex == -1 ) { // Create new one
		_String sql;
		if( m_pSqlScript->GetSqlSourceByName(pszCommandName,sql) ){
			DataCommand* pCmdNew = new DataCommand();
			m_arrUsedCommands.Add(DecodeString(pszCommandName),pCmdNew);
			pCmdNew->SetCommandText(sql);
			return pCmdNew;
			}
		return NULL;
 		}
	return m_arrUsedCommands.GetData(nIndex);
	}

BOOL
Database::DeleteDataCommand(TCHAR* pszCommandName){
	int nIndex = m_arrUsedCommands.FindValue(DecodeString(pszCommandName));
	if( nIndex != -1 ) { 
		DataCommand* pCmd = m_arrUsedCommands.GetData(nIndex);
		if( pCmd ){
			delete pCmd;
			}
		m_arrUsedCommands.Delete(nIndex);
		return TRUE;
 		}
	return FALSE;
	}

int
Database::EmptyUsedDataCommands(){
	int nLoop = 0, nCt = m_arrUsedCommands.GetCount();
	while( nLoop < nCt ){
		DataCommand* pCmd = m_arrUsedCommands.GetData(nLoop);
		if( pCmd ){
			delete pCmd;
			}
		nLoop ++;
		}
	m_arrUsedCommands.DeleteAll();
	m_arrUsedCommands.FreeExtra();
	return nCt;
	}

BOOL
Database::Close(BOOL bCloseDataTables /*= TRUE*/){
	if( m_pSqlScript ){
		delete m_pSqlScript;
		m_pSqlScript = NULL;
		}

	// Empty data commands.
	EmptyUsedDataCommands();

	if( bCloseDataTables ){
		}

	m_bIsOpen = FALSE;
	return FALSE;
	}

// =======================
// FirebirdDatabase
//========================

FirebirdDatabase::FirebirdDatabase() : Database(){
	m_pDb = NULL;
	}

FirebirdDatabase::~FirebirdDatabase(){
	}

BOOL
FirebirdDatabase::Open(){
	BOOL bRet = Database::Open();
	if( bRet ){
		CDatabase* pDb = new CDatabase();
		try
			{
			pDb->OpenEx(m_sConnectionString, CDatabase::noOdbcDialog);
			m_pDb = pDb;
			}
		catch(CDBException* e)
			{
#ifdef _DEBUG
			AfxMessageBox(e->m_strError);
#endif
			delete pDb;
			bRet = FALSE;
			}
		}
	return bRet;
	}

BOOL
FirebirdDatabase::Close(BOOL bCloseDataTables /*= TRUE*/){
	BOOL bRet = Database::Close(bCloseDataTables);
	if( m_pDb ){
		if( m_pDb->IsOpen() ){
			bRet = TRUE;
			m_pDb->Close();
			}
		delete m_pDb;
		m_pDb = NULL;
		}
	return bRet;
	}

// =======================
// MSSQLDatabase
//========================

MSSQLDatabase::MSSQLDatabase() : Database(){
	m_pDb = NULL;
	}

MSSQLDatabase::~MSSQLDatabase(){
	}

BOOL
MSSQLDatabase::Open(){
	BOOL bRet = Database::Open();
	if( bRet ){
		CDatabase* pDb = new CDatabase();
		try{
			pDb->OpenEx(m_sConnectionString, CDatabase::noOdbcDialog);
			m_pDb = pDb;
			}
		catch(CDBException* e)
			{
#ifdef _DEBUG
			AfxMessageBox(e->m_strError);
#endif
			delete pDb;
			bRet = FALSE;
			}
		}
	return bRet;
	}

BOOL
MSSQLDatabase::Close(BOOL bCloseDataTables /*= TRUE*/){
	BOOL bRet = Database::Close(bCloseDataTables);
	if( m_pDb ){
		if( m_pDb->IsOpen() ){
			bRet = TRUE;
			m_pDb->Close();
			}
		delete m_pDb;
		m_pDb = NULL;
		}
	return bRet;
	}