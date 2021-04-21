// ===========================================================================
// About: MFC new generation data management class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#include "StdAfx.h"
#include "DataHelper.h"
#include "Database.h"
#include "../Path/Path.h"

// ============================================
// DatabaseList 
//=============================================

DatabaseList::DatabaseList(){
	}

BEGIN_DECLARE_VAR(DatabaseList, Serializable)
	DECLARE_VAR("databases", _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR("data", _T(""), VariableType::VT_None, offsetof(DatabaseList,m_list), false, true, true)
END_DECLARE_VAR()

DatabaseList::~DatabaseList(){
	}

DatabaseInfo*
DatabaseList::GetDatabaseInfo(TCHAR* pszDbName){
	ASSERT(pszDbName);
	int nCount	= m_list.GetCount(), nLoop = 0;
	while( nLoop < nCount ){
		ASSERT(m_list.GetAt(nLoop));
		if( !m_list.GetAt(nLoop)->m_sName.CompareNoCase(pszDbName) )
			return m_list.GetAt(nLoop);
		nLoop ++;
		}
	return NULL;
	}

// ============================================
// DatabaseInfo 
//=============================================

DatabaseInfo::DatabaseInfo(){
	m_pDbObject = NULL;
	}

BEGIN_DECLARE_VAR(DatabaseInfo, Serializable)
	DECLARE_VAR("data", _T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR("Name", _T(""), VariableType::VT_String, offsetof(DatabaseInfo,m_sName), false, false, false)
	DECLARE_VAR("DbType", _T(""), VariableType::VT_String, offsetof(DatabaseInfo,m_sDbType), false, false, false)
	DECLARE_VAR("sqlscript", _T(""), VariableType::VT_String, offsetof(DatabaseInfo,m_sSqlScriptFile), false, false, false)
	DECLARE_VAR("ConnectStr", _T(""), VariableType::VT_String, offsetof(DatabaseInfo,m_sConnectionString), false, false, false)
END_DECLARE_VAR()

DatabaseInfo::~DatabaseInfo(){
	if( m_pDbObject ){
		m_pDbObject->Close();
		delete m_pDbObject;
		m_pDbObject = NULL;
		}
	}

Database*
DatabaseInfo::GetDbObject(){
	if( m_pDbObject )
		return m_pDbObject;

	if( !m_sDbType.CompareNoCase(_T("FirebirdSQL")) ){
		m_pDbObject = new FirebirdDatabase();
		}
	else
	if( !m_sDbType.CompareNoCase(_T("MSSQL")) ){
		m_pDbObject = new MSSQLDatabase();
		}
	else
		m_pDbObject = new Database();
	
	m_pDbObject->m_sConnectionString = m_sConnectionString;
	m_pDbObject->m_sSqlScriptFile = m_sSqlScriptFile;
	return m_pDbObject;
	}

// ============================================
// DataCommandParams 
//=============================================

DataCommandParams::DataCommandParams(){
	}

DataCommandParams::~DataCommandParams(){
	}

// ============================================
// DataCommand 
//=============================================

DataCommand::DataCommand(){
	m_cmdType = QueryCommand;
	}

DataCommand::~DataCommand(){
	}

void
DataCommand::SetCommandText(_String cmdText){
	m_sCmdText = cmdText;
	}

int
DataCommand::ExecuteCommand(){
	return 0;
	}

DataTable*
DataCommand::ExecuteQuery(){
	return NULL;
	}
