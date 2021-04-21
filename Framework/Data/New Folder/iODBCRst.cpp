#include "stdafx.h"
#include "iODBCRst.h"
#include "DataFieldCopier.h"
#include "../Log/Logger.h"
#include <afxmt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* this is how to get error string ftom statement handle.
	SQLCHAR       SqlState[6], SQLStmt[100], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i=1, MsgLen;

	SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, i, SqlState, &NativeError,
				  Msg, sizeof(Msg), &MsgLen);
*/

////////////////////////////////////////////////////////////////
//
//	iODBCRst class
//
////////////////////////////////////////////////////////////////

iODBCRst::iODBCRst( CDatabase* pDatabase /*= NULL*/ ) : 
	CODBCRecordset( pDatabase ){
	m_pVRowSet		= NULL;
	}

////////////////////////////////////////////////////////////////
iODBCRst::~iODBCRst(){
	if( m_pVRowSet ){
		delete m_pVRowSet;
		m_pVRowSet = NULL;
		}
	}

////////////////////////////////////////////////////////////////
BOOL	
iODBCRst::Open(LPCTSTR lpszSQL, UINT nOpenType, DWORD dwOptions){
	BOOL	bRet = CODBCRecordset::Open(lpszSQL, nOpenType, dwOptions);
	if( bRet ){
		}
	return bRet;
	}

bool
iODBCRst::ExecuteProcedure(CDatabase* pDBase, Serializable* pProcedureDesc, DatabaseType databaseType){
	ASSERT( pDBase && pDBase->IsOpen() );
	ASSERT( pProcedureDesc );

	_String	sExecSQL;
	List<SQLExecParamInfo> params;
	if( !pProcedureDesc->GetProcedureSQL(sExecSQL, &params) ){
		return false;
		}

	VirtualRowSet* pRowSetBind = VirtualRowSet::CreateFromSerializable(pProcedureDesc, NULL, NULL, false, 255);
	if( !pRowSetBind || params.GetCount() != pRowSetBind->GetFieldCount() ){
		return false;
		}
	pRowSetBind->AddRow(); // Single row.

	DataFieldCopier copy(pRowSetBind, pProcedureDesc, false);
	// Copy to virtual rowset.
	if( !copy.CopyFromSerializable(pRowSetBind, pProcedureDesc) ){
		return false;
		}

	static int nThreadIdEnter;
	static int nCallCt;

	CCriticalSection* lock = GetSectionLock();
	lock->Lock();

	SQLRETURN		rc; 
	SQLHSTMT		hstmt;

	// Alloc statement.
//	rc = SQLAllocStmt(pDBase->m_hdbc, &hstmt);
	rc = SQLAllocHandle(SQL_HANDLE_STMT, pDBase->m_hdbc, &hstmt);

	nThreadIdEnter	= GetCurrentThreadId();
	nCallCt ++;

	int nLoop	= 0;
	int nCt		= params.GetCount();
	while( nLoop < nCt ){
		SQLExecParamInfo* pParam = params[nLoop];
		void*		lpVal		= pRowSetBind->GetFieldValuePtr(nLoop);
		SQLLEN*		lpValLen	= (SQLLEN*)pRowSetBind->GetFieldLengthPtr(nLoop);
		SQLSMALLINT tp			= pParam->wSQLType;

		VRowSetFieldInfo* pFieldInfo = pRowSetBind->GetFieldInfo(nLoop);
		if( pFieldInfo->bIsBlob ){ // Attach binary.
			rc = SQLBindParameter(hstmt, nLoop + 1, (SQLSMALLINT)pParam->dwParameterType, 
							pParam->wSQLCType, pParam->wSQLType, 0, 0, (SQLPOINTER)*((void**)lpVal), 0, lpValLen);
			}
		else{
			if( pFieldInfo->type == FieldType::FTChar ){
				if( *lpValLen > 0 ) // NULL symbol must be absent in length parameter.
					*lpValLen = (*lpValLen - 1);
				}
			rc = SQLBindParameter(hstmt, nLoop + 1, (SQLSMALLINT)pParam->dwParameterType, 
							pParam->wSQLCType, pParam->wSQLType, 0, 0, (SQLPOINTER)lpVal, pFieldInfo->nSize, lpValLen);
			}
		
		if( rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO ){
			lock->Unlock(); // Unlock
			// Log error
			Logger::CreateLogEntry(GetLastError(hstmt).GetBuffer());
			delete pRowSetBind;
			// Free statement handle.
			FreeStatementHandle(hstmt);
			return false;
			}
		nLoop ++;
		}

	 // Execute the command. 
	rc = SQLExecDirect(hstmt, (UCHAR*)sExecSQL.GetBuffer(), SQL_NTS);
	if( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) ) {
		lock->Unlock(); // Unlock
		// Log error
		Logger::CreateLogEntry(GetLastError(hstmt).GetBuffer());
		// Delete rowset.
		delete pRowSetBind;
		// Free statement handle.
		FreeStatementHandle(hstmt);
		return false;
		}

	// When result set is returning by procedure.
	VariableInfo* pVarInfoRet = pProcedureDesc->GetDbFieldInfo(_T("RET"));
	if( pVarInfoRet && pVarInfoRet->m_bComplexType && pVarInfoRet->m_bEnumerable ){
		// CALL statement has executed successfully. Process the result set. 
		// Get number of result columns for the result set.
		SQLSMALLINT wNum = 0;
		rc = SQLNumResultCols(hstmt, &wNum);
		if ((rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) && wNum > 0 ) {
			SQLCHAR			cFieldName[128];
			SQLSMALLINT		sBufferLen = 128;
			SQLSMALLINT		sNameLen;
			SQLSMALLINT		sDataType;
			SQLSMALLINT		sDecimalDigits;
			SQLSMALLINT		sNullable;
			SQLULEN			sColumnSize;

			VirtualRowSet*	pResultSet = new VirtualRowSet();
			CODBCFieldInfo	fieldInfos[64];
			CDWordArray		arrDbvtTypes;

			void*			lpValue		= NULL;
			SQLLEN*			lpLength	= NULL;
			My_BindCol_Func bindCol_func;
			AutoSortedArray	arrBinaryFields;
			int				nFieldCt	= 0;

			// Describe result set columns. {{
			SQLSMALLINT i = 1;
			while(i <= wNum){
				rc = SQLDescribeCol(hstmt, i, cFieldName, sBufferLen, &sNameLen, &sDataType, &sColumnSize, &sDecimalDigits, &sNullable);
				if( rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO ){
					int dwDbvt = CODBCRecordset::GetVariantType((short)sDataType);
					ASSERT(dwDbvt != DBVT_NULL);		// Unknown type.
					FieldType fType = FieldTypeFromDBVT(dwDbvt);
					ASSERT(fType != FieldType::FTNone); // Unknown field type.

					arrDbvtTypes.Add(dwDbvt);
					fieldInfos[nFieldCt].m_nPrecision	= sColumnSize;
					fieldInfos[nFieldCt].m_nNullability = sNullable;
					fieldInfos[nFieldCt].m_nScale		= sDecimalDigits;
					fieldInfos[nFieldCt].m_nSQLType		= sDataType;
					fieldInfos[nFieldCt].m_strName		= cFieldName;
					nFieldCt ++;

					if( fType == FieldType::FTChar )
						pResultSet->AddFieldEx((TCHAR*)cFieldName, fType, (int)sColumnSize + 1);
					else
						pResultSet->AddField((TCHAR*)cFieldName, fType);
					}
				i ++;
				}
			// }}

			// Bind columns to values. {{
			if( !pResultSet->Create() )
				ASSERT(FALSE);
			pResultSet->AddRow();

			nLoop = 0;
			while( nLoop < nFieldCt ){
				bindCol_func		= (My_BindCol_Func)(GetBindFieldMethodsArray()->GetAt(arrDbvtTypes[nLoop]));
				ASSERT( bindCol_func != NULL );
				lpValue				= pResultSet->GetFieldValuePtr(nLoop);
				lpLength			= (SQLLEN*)pResultSet->GetFieldLengthPtr(nLoop);
				if( arrDbvtTypes[nLoop] == DBVT_BINARY )
					arrBinaryFields.AddAsLong(nLoop);
				// Call my BindCol method. {{
				(*bindCol_func)(hstmt, nLoop+1, lpValue, lpLength, &fieldInfos[nLoop]);
				// }}
				nLoop ++;
				}
			// }}
	
			EnumerableObject<Serializable>* pListResultSet	=  (EnumerableObject<Serializable>*)pProcedureDesc->GetVariablePtr(pVarInfoRet->m_sAttributeName);
			ASSERT(pListResultSet);	
			Serializable*					pTempObj		= pListResultSet->CreateSerializableObject();
			DataFieldCopier					copyPerformer(pResultSet, pTempObj);

			CRecordset rstDummy;
			rstDummy.m_hstmt = hstmt;
			CFieldExchange fEx(0, &rstDummy);

			while ((rc = SQLFetch(hstmt)) != SQL_NO_DATA){
				// Load binary fields. {{
				for( int i=0; i<arrBinaryFields.GetCount(); i++){
					int					nField	= (int)arrBinaryFields[i];
					VRowSetFieldInfo*	pInfo	= pResultSet->GetFieldInfo(nField);
					ASSERT(pInfo);
			
					if( pInfo->bIsBlob ){ // Binary fields.
						lpValue		= pResultSet->GetFieldValuePtr(nField);
						lpLength	= (SQLLEN*)pResultSet->GetFieldLengthPtr(nField);

						// Get binary size.=======================
						RETCODE nRetCode;
						int		nDummy;
						SQLLEN	lSize = SQL_NULL_DATA;

						// Give empty buffer to find size of entire LongBinary
						nRetCode = ::SQLGetData(hstmt, (UWORD)nField+1, SQL_C_DEFAULT, &nDummy, 0, &lSize);
						// ========================================

						SQLLEN nBinarylen	= lSize;
						if( nBinarylen != SQL_NULL_DATA ){
							CLongBinary binary;
							fEx.GetLongBinaryData(nField+1, binary, &nBinarylen);

							void* lpSrc = GlobalLock(binary.m_hData);	// Lock memory
							pResultSet->SetBinary(nField, lpSrc, (int)nBinarylen, TRUE); // Set binary value.
							GlobalUnlock(binary.m_hData);				// Unlock memory
							}
						else
							pResultSet->SetBinary(nField, NULL, 0); // Clear binary value.
						}
					}
				// }}

				// Copy from VRowSet into serializable object. {{
				Serializable*	pObject	= pListResultSet->CreateSerializableObject();
				if( copyPerformer.CopyFromVRow(pResultSet, pObject) > 0 ){
					pListResultSet->Add(pObject);
					}
				else
					delete pObject;
				// }}
				}

			rstDummy.m_hstmt = NULL;
			delete pTempObj;
			delete pResultSet;
			}
		}
	else{
		// Clear any result sets generated.
		while ( ( rc = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
		}

	// Copy from virtual rowset.
	copy.CopyFromVRow(pRowSetBind, pProcedureDesc);
	// Delete rowset.
	delete pRowSetBind;
	// Free statement handle.
	FreeStatementHandle(hstmt);
	lock->Unlock(); // Unlock
	return true;
	}

bool
iODBCRst::SaveRecord(CDatabase* pDBase, Serializable* pSave, DatabaseType databaseType, CStringVector* pArrDirtyFieldNames /*= NULL*/){
	ASSERT( pDBase && pDBase->IsOpen() );
	ASSERT( pSave );

	VariableInfo*	pInfo	= pSave->GetDbFieldInfo(_T("ID"));
	if( !pInfo )
		return false; // ID field not exists !!!

	_String sId;
	if( !pSave->GetFieldAsString(pInfo->m_sAttributeName, sId) ){
		return false; // ID field not exists !!!
		}

	List<SQLExecParamInfo> params;
	_String sSQLCommand;
	__int64 nId		= _ttoi64(sId.GetBuffer());
	if( nId <= 0 ){// Add New
		if( !pSave->GetAddNewSQL(sSQLCommand, &params, (databaseType == DatabaseType::MSSQL), pArrDirtyFieldNames) ){
			return false; // Couldn't build SQL command.
			}
		}
	else{// Update 
		if( !pSave->GetUpdateSQL(sSQLCommand, &params, pArrDirtyFieldNames) ){
			return false; // Couldn't build SQL command.
			}
		}

	VirtualRowSet* pRowSetBind = VirtualRowSet::CreateFromSerializable(pSave, pArrDirtyFieldNames, NULL, true, 255);
	if( !pRowSetBind || pRowSetBind->GetFieldCount() <= 0 ){
		return false;
		}
	pRowSetBind->AddRow(); // Single row.

	DataFieldCopier copy(pRowSetBind, pSave, true);
	// Copy to virtual rowset.
	if( !copy.CopyFromSerializable(pRowSetBind, pSave) ){
		return false;
		}

	CCriticalSection* lock = GetSectionLock();
	lock->Lock();

	SQLRETURN		rc; 
	SQLHSTMT		hstmt;
	// Alloc statement.
	rc			= SQLAllocStmt(pDBase->m_hdbc, &hstmt);

	int nLoop	= 0;
	int nCt		= params.GetCount();
	while( nLoop < nCt ){
		SQLExecParamInfo* pParam	= params[nLoop];
		void*		lpVal			= pRowSetBind->GetFieldValuePtr(pParam->nField);
		SQLLEN*		lpValLen		= (SQLLEN*)pRowSetBind->GetFieldLengthPtr(pParam->nField);
		SQLSMALLINT tp				= pParam->wSQLType;

		VRowSetFieldInfo* pFieldInfo = pRowSetBind->GetFieldInfo(pParam->nField);
		if( pFieldInfo->bIsBlob ){ // Attach binary.
			rc = SQLBindParameter(hstmt, nLoop + 1, (SQLSMALLINT)pParam->dwParameterType, 
							pParam->wSQLCType, pParam->wSQLType, 0, 0, (SQLPOINTER)*((void**)lpVal), 0, lpValLen);
			}
		else{
			rc = SQLBindParameter(hstmt, nLoop + 1, (SQLSMALLINT)pParam->dwParameterType, 
							pParam->wSQLCType, pParam->wSQLType, 0, 0, (SQLPOINTER)lpVal, pFieldInfo->nSize, lpValLen);
			}

		if( rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO ){
			// Log error
			Logger::CreateLogEntry(GetLastError(hstmt).GetBuffer());
			delete pRowSetBind;
			// Free statement handle.
			FreeStatementHandle(hstmt);
			lock->Unlock();
			return false;
			}
		nLoop ++;
		}

	 // Execute the command. 
 	rc = SQLExecDirect(hstmt, (UCHAR*)sSQLCommand.GetBuffer(), SQL_NTS);
	if( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) ) {
		// Log error
		Logger::CreateLogEntry(GetLastError(hstmt).GetBuffer());
		// Delete rowset.
		delete pRowSetBind;
		// Free statement handle.
		FreeStatementHandle(hstmt);
		lock->Unlock();
		return false;
		}

	SQLLEN rowsAffected = 0;
	SQLRowCount(hstmt, &rowsAffected);

	// Copy from virtual rowset.
	copy.CopyFromVRow(pRowSetBind, pSave);
	// Delete rowset.
	delete pRowSetBind;
	// Free statement handle.
	FreeStatementHandle(hstmt);
	lock->Unlock();
	return (rowsAffected != 0);
	}

_String			
iODBCRst::GetLastError(SQLHSTMT hstmt){
	SQLCHAR		stateChar[32];
	SQLINTEGER	nativeError;
	SQLCHAR		messageText[255];
	SQLSMALLINT textLength = 0;
	SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, 1, stateChar, &nativeError, messageText, 255, &textLength);
	if( textLength <= 0 ){
		return _T("");
		}

	_String sError;
	sError.Format(_T("STATE=%s NATIVE=%d MESSAGE=%s"), stateChar, nativeError, messageText);
	return sError;
	}

bool
iODBCRst::FreeStatementHandle(SQLHSTMT hstmt){
	::SQLFreeStmt	(hstmt, SQL_CLOSE);
	::SQLFreeStmt	(hstmt, SQL_UNBIND);
	::SQLFreeStmt	(hstmt, SQL_RESET_PARAMS);
	::SQLFreeStmt	(hstmt, SQL_DROP);
	return true;
	}

bool
iODBCRst::DeleteRecord(CDatabase* pDBase, Serializable* pDelete, DatabaseType databaseType){
	ASSERT( pDBase && pDBase->IsOpen() );
	ASSERT( pDelete );

	_String sDeleteSQL;
	if( !pDelete->GetDeleteSQL(sDeleteSQL) )
		return false;

	CCriticalSection* lock = GetSectionLock();
	lock->Lock();

	SQLRETURN		rc; 
	SQLHSTMT		hstmt;
	// Alloc statement.
	rc = SQLAllocStmt(pDBase->m_hdbc, &hstmt);

	// Execute the command. 
	rc = SQLExecDirect(hstmt, (UCHAR*)sDeleteSQL.GetBuffer(), SQL_NTS);
	if( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) ) {
		// Log error
		Logger::CreateLogEntry(GetLastError(hstmt).GetBuffer());
		// Free statement handle.
		FreeStatementHandle(hstmt);
		lock->Unlock();
		return false;
		}

	SQLLEN rowsAffected = 0;
	SQLRowCount(hstmt, &rowsAffected);
	
	// Free statement handle.
	FreeStatementHandle(hstmt);
	lock->Unlock();
	return (rowsAffected != 0);
	}

bool
iODBCRst::RefreshRecord(CDatabase* pDBase, Serializable* pRefresh, DatabaseType databaseType){
	return false;
	}

int
iODBCRst::LoadRecords(CDatabase* pDb, EnumerableObject<Serializable> *pListRecords, DatabaseType databaseType,
					const _String sWhereSQL /*= _T("")*/, const _String sOrderBySQL /*= _T("")*/, const _String sTop /*= _T("")*/, bool bUseDbFieldNames /*= true*/)
	{
	_String			sSQL		= _T("");
	Serializable*	pTempObj	= pListRecords->CreateSerializableObject();
	if( !pTempObj || !pTempObj->GetSelectSQL(sSQL, sTop) ){
		if( pTempObj )
			delete pTempObj;
		return -1;
		}

	// Check for dependency tables. {{
	CPtrArray		arrDependentFields;
	VariableInfo*	pVarInfo	= pTempObj->GetThisInfo();
	int				nLoop		= 1;
	while( !pVarInfo[nLoop].m_sAttributeName.IsEmpty() ){
		VariableInfo* ppp = &pVarInfo[nLoop];
		if( !pVarInfo[nLoop].m_sDbFieldName.IsEmpty() && pVarInfo[nLoop].m_bComplexType /*&& pVarInfo[nLoop].m_bEnumerable*/ ){
			arrDependentFields.Add((void*)&pVarInfo[nLoop]);
			}
		nLoop ++;
		}
	// }}

	if( !sWhereSQL.IsEmpty() )
		sSQL += _T(" ") + sWhereSQL;

	if( !sOrderBySQL.IsEmpty() )
		sSQL += _T(" ") + sOrderBySQL;

	SQLRETURN		rc; 
	SQLHSTMT		hstmt;
	VirtualRowSet*	pRowSet = new VirtualRowSet();
	CODBCFieldInfo	fieldInfos[64];
	CDWordArray		arrDbvtTypes;

	nLoop						= 0;
	void*			lpValue		= NULL;
	SQLLEN*			lpLength	= NULL;
	My_BindCol_Func bindCol_func;
	AutoSortedArray	arrBinaryFields;

	SQLCHAR			cFieldName[128];
	SQLSMALLINT		sBufferLen = 128;
	SQLSMALLINT		sNameLen;
	SQLSMALLINT		sDataType;
	SQLSMALLINT		sDecimalDigits;
	SQLSMALLINT		sNullable;
	SQLULEN			sColumnSize;

	CCriticalSection* lock = GetSectionLock();
	lock->Lock();

	// Alloc statement.
	rc = SQLAllocStmt(pDb->m_hdbc, &hstmt);
	// Create a result set containing the ID and picture of each part.
	rc = SQLExecDirect(hstmt, (SQLCHAR*)sSQL.GetBuffer(), SQL_NTS);
	if( rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO ){
		_String sError;
		sError.Format(_T("SQL=%s ErrorDesc=%s"), sSQL.GetBuffer(), GetLastError(hstmt).GetBuffer());

		if( pTempObj )
			delete pTempObj;
		lock->Unlock();
		// Log error
		Logger::CreateLogEntry(sError.GetBuffer());
		return -1;
		}

	// Describe fields. {{
	int			nFieldCt = 0;
	DWORD		dwDbvt;
	SQLSMALLINT sLoop = 1; 
	rc = SQLDescribeCol(hstmt, sLoop, cFieldName, sBufferLen, &sNameLen, &sDataType, &sColumnSize, &sDecimalDigits, &sNullable);
	while(	rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO ){
		dwDbvt = CODBCRecordset::GetVariantType((short)sDataType);
		if( dwDbvt == DBVT_NULL )
			ASSERT(FALSE); // Unknown type.
		FieldType fType = FieldTypeFromDBVT(dwDbvt);
		if( fType == FieldType::FTNone )
			ASSERT(FALSE); // Unknown field type.

		arrDbvtTypes.Add(dwDbvt);

		fieldInfos[nFieldCt].m_nPrecision	= sColumnSize;
		fieldInfos[nFieldCt].m_nNullability = sNullable;
		fieldInfos[nFieldCt].m_nScale		= sDecimalDigits;
		fieldInfos[nFieldCt].m_nSQLType		= sDataType;
		fieldInfos[nFieldCt].m_strName		= cFieldName;

		if( fType == FieldType::FTChar ){
			pRowSet->AddFieldEx((TCHAR*)cFieldName, fType, (int)sColumnSize + 1);
			}
		else
			pRowSet->AddField((TCHAR*)cFieldName, fType);
		
		sLoop ++; nFieldCt ++;
		rc = SQLDescribeCol(hstmt, sLoop, cFieldName, sBufferLen, &sNameLen, &sDataType, &sColumnSize, &sDecimalDigits, &sNullable);
		if( !sDataType ) break;
		}
	// }}
	
	// Create virtual row. {{
	if( !pRowSet->Create() )
		ASSERT(FALSE);
	pRowSet->AddRow();
	// }}

	// Bind columns. {{
	while( nLoop < nFieldCt ){
		bindCol_func		= (My_BindCol_Func)(GetBindFieldMethodsArray()->GetAt(arrDbvtTypes[nLoop]));
		ASSERT( bindCol_func != NULL );
		lpValue				= pRowSet->GetFieldValuePtr(nLoop);
		lpLength			= (SQLLEN*)pRowSet->GetFieldLengthPtr(nLoop);

		if( arrDbvtTypes[nLoop] == DBVT_BINARY )
			arrBinaryFields.AddAsLong(nLoop);

		// Call my BindCol method. {{
		(*bindCol_func)(hstmt, nLoop+1, lpValue, lpLength, &fieldInfos[nLoop]);
		// }}
		nLoop ++;
		}
	// }}

	// Fetch records loop. {{
	DataFieldCopier copyPerformer(pRowSet, pTempObj, bUseDbFieldNames);

	CRecordset rstDummy;
	rstDummy.m_hstmt = hstmt;
	CFieldExchange fEx(0, &rstDummy);

	while ((rc = SQLFetch(hstmt)) != SQL_NO_DATA){
		// Load binary fields. {{
		for( int i=0; i<arrBinaryFields.GetCount(); i++){
			int					nField	= (int)arrBinaryFields[i];
			VRowSetFieldInfo*	pInfo	= pRowSet->GetFieldInfo(nField);
			ASSERT(pInfo);
			
			if( pInfo->bIsBlob ){ // Binary fields.
				lpValue		= pRowSet->GetFieldValuePtr(nField);
				lpLength	= (SQLLEN*)pRowSet->GetFieldLengthPtr(nField);

				// Get binary size.=======================
				RETCODE nRetCode;
				int		nDummy;
				SQLLEN	lSize = SQL_NULL_DATA;

				// Give empty buffer to find size of entire LongBinary
				nRetCode = ::SQLGetData(hstmt, (UWORD)nField+1, SQL_C_DEFAULT, &nDummy, 0, &lSize);
				// ========================================

				SQLLEN nBinarylen	= lSize;
				if( nBinarylen != SQL_NULL_DATA ){
					CLongBinary binary;
					fEx.GetLongBinaryData(nField+1, binary, &nBinarylen);

					void* lpSrc = GlobalLock(binary.m_hData);	// Lock memory
					pRowSet->SetBinary(nField, lpSrc, (int)nBinarylen, TRUE); // Set binary value.
					GlobalUnlock(binary.m_hData);				// Unlock memory
					}
				else
					pRowSet->SetBinary(nField, NULL, 0); // Clear binary value.
				}
			}
		// }}

		// Copy from VRowSet into serializable object. {{
		Serializable*	pObject	= pListRecords->CreateSerializableObject();
		if( copyPerformer.CopyFromVRow(pRowSet, pObject) > 0 ){
			pListRecords->Add(pObject);

			// Load dependent table.
			for(int j=0; j<arrDependentFields.GetCount(); j++){
				VariableInfo*	pVarInfo	= (VariableInfo*)arrDependentFields[j];
				int				nFind		= pVarInfo->m_sDbFieldName.Find("=");
				if( nFind == -1 ) continue;

				_String			sField1		= pVarInfo->m_sDbFieldName.Left(nFind).Trim();
				_String			sField2		= pVarInfo->m_sDbFieldName.Right(pVarInfo->m_sDbFieldName.GetLength() - nFind - 1).Trim();
				_String			sValue		= pRowSet->GetString(sField1.GetBuffer());
				_String			sWhereSQL;
				sWhereSQL.Format(_T("where %s = %s"), sField2.GetBuffer(), sValue.GetBuffer());

				// Load list of table objects.
				if( pVarInfo->m_bEnumerable ){
					EnumerableObject<Serializable>* pList = (EnumerableObject<Serializable>*)((PTR_HOLDER_INTEGER)pObject + pVarInfo->m_nOffset);
					LoadRecords(pDb, pList, databaseType, sWhereSQL, _T(""));
					}
				else{ // Load only one instance of table object.
					Serializable* pVarObject = (Serializable*)((PTR_HOLDER_INTEGER)pObject + pVarInfo->m_nOffset);
					EnumerableObject<Serializable>* pList = pVarObject->CreateEnumerableObject();
					if( pList ){
						LoadRecords(pDb, pList, databaseType, sWhereSQL, _T(""));
						if( pList->GetCount() > 0 ){
							pList->GetAt(0)->Copy(pVarObject);
							}
						delete pList;
						}
					}
				}
			}
		else
			delete pObject;
		// }}
		}
	// }}		

	rstDummy.m_hstmt = NULL;
	// Free statement handle.
	FreeStatementHandle(hstmt);
	lock->Unlock();
	if( pRowSet ){delete pRowSet;}
	return pListRecords->GetCount();
	}

int
iODBCRst::LoadRecordsBySql(CDatabase* pDb, EnumerableObject<Serializable> *pListRecords, DatabaseType databaseType,
					const _String sSQL, bool bUseDbFieldNames /*= true*/)
	{
	SQLRETURN		rc; 
	SQLHSTMT		hstmt;
	VirtualRowSet*	pRowSet = new VirtualRowSet();
	CODBCFieldInfo	fieldInfos[64];
	CDWordArray		arrDbvtTypes;

	int				nLoop		= 0;
	void*			lpValue		= NULL;
	SQLLEN*			lpLength	= NULL;
	My_BindCol_Func bindCol_func;
	AutoSortedArray	arrBinaryFields;

	SQLCHAR			cFieldName[128];
	SQLSMALLINT		sBufferLen = 128;
	SQLSMALLINT		sNameLen;
	SQLSMALLINT		sDataType;
	SQLSMALLINT		sDecimalDigits;
	SQLSMALLINT		sNullable;
	SQLULEN			sColumnSize;

	CCriticalSection* lock = GetSectionLock();
	lock->Lock();

	_String sSQL_ = sSQL;

	// Alloc statement.
	rc = SQLAllocStmt(pDb->m_hdbc, &hstmt);
	// Create a result set containing the ID and picture of each part.
	rc = SQLExecDirect(hstmt, (SQLCHAR*)sSQL_.GetBuffer(), SQL_NTS);
	if( rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO ){
		_String sError;
		sError.Format(_T("SQL=%s ErrorDesc=%s"), (const TCHAR*)sSQL, GetLastError(hstmt).GetBuffer());
		lock->Unlock();
		// Log error
		Logger::CreateLogEntry(sError.GetBuffer());
		delete pRowSet;
		return -1;
		}

	// Describe fields. {{
	int			nFieldCt = 0;
	DWORD		dwDbvt;
	SQLSMALLINT sLoop = 1; 
	rc = SQLDescribeCol(hstmt, sLoop, cFieldName, sBufferLen, &sNameLen, &sDataType, &sColumnSize, &sDecimalDigits, &sNullable);
	while(	rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO ){
		dwDbvt = CODBCRecordset::GetVariantType((short)sDataType);
		if( dwDbvt == DBVT_NULL ){
			ASSERT(FALSE); // Unknown type.
			}

		FieldType fType = FieldTypeFromDBVT(dwDbvt);
		if( fType == FieldType::FTNone ){
			ASSERT(FALSE); // Unknown field type.
			}

		arrDbvtTypes.Add(dwDbvt);

		fieldInfos[nFieldCt].m_nPrecision	= sColumnSize;
		fieldInfos[nFieldCt].m_nNullability = sNullable;
		fieldInfos[nFieldCt].m_nScale		= sDecimalDigits;
		fieldInfos[nFieldCt].m_nSQLType		= sDataType;
		fieldInfos[nFieldCt].m_strName		= cFieldName;

		if( fType == FieldType::FTChar ){
			pRowSet->AddFieldEx((TCHAR*)cFieldName, fType, (int)sColumnSize + 1);
			}
		else
			pRowSet->AddField((TCHAR*)cFieldName, fType);
		
		sLoop ++; nFieldCt ++;
		rc = SQLDescribeCol(hstmt, sLoop, cFieldName, sBufferLen, &sNameLen, &sDataType, &sColumnSize, &sDecimalDigits, &sNullable);
		if( !sDataType ) break;
		}
	// }}
	
	// Create virtual row. {{
	if( !pRowSet->Create() ){
		ASSERT(FALSE);
		}
	pRowSet->AddRow();
	// }}

	// Bind columns. {{
	while( nLoop < nFieldCt ){
		bindCol_func		= (My_BindCol_Func)(GetBindFieldMethodsArray()->GetAt(arrDbvtTypes[nLoop]));
		ASSERT( bindCol_func != NULL );
		lpValue				= pRowSet->GetFieldValuePtr(nLoop);
		lpLength			= (SQLLEN*)pRowSet->GetFieldLengthPtr(nLoop);
		if( arrDbvtTypes[nLoop] == DBVT_BINARY )
			arrBinaryFields.AddAsLong(nLoop);

		// Call my BindCol method. {{
		(*bindCol_func)(hstmt, nLoop+1, lpValue, lpLength, &fieldInfos[nLoop]);
		// }}
		nLoop ++;
		}
	// }}

	// Fetch records loop. {{
	Serializable*	pTempObj	= pListRecords->CreateSerializableObject();
	DataFieldCopier copyPerformer(pRowSet, pTempObj, bUseDbFieldNames);

	CRecordset rstDummy;
	rstDummy.m_hstmt = hstmt;
	CFieldExchange fEx(0, &rstDummy);

	while ((rc = SQLFetch(hstmt)) != SQL_NO_DATA){
		// Load binary fields. {{
		for( int i=0; i<arrBinaryFields.GetCount(); i++){
			int					nField	= (int)arrBinaryFields[i];
			VRowSetFieldInfo*	pInfo	= pRowSet->GetFieldInfo(nField);
			ASSERT(pInfo);
			
			if( pInfo->bIsBlob ){ // Binary fields.
				lpValue		= pRowSet->GetFieldValuePtr(nField);
				lpLength	= (SQLLEN*)pRowSet->GetFieldLengthPtr(nField);

				// Get binary size.=======================
				RETCODE nRetCode;
				int		nDummy;
				SQLLEN	lSize = SQL_NULL_DATA;

				// Give empty buffer to find size of entire LongBinary
				nRetCode = ::SQLGetData(hstmt, (UWORD)nField+1, SQL_C_DEFAULT, &nDummy, 0, &lSize);
				// ========================================

				SQLLEN nBinarylen	= lSize;
				if( nBinarylen != SQL_NULL_DATA ){
					CLongBinary binary;
					fEx.GetLongBinaryData(nField+1, binary, &nBinarylen);

					void* lpSrc = GlobalLock(binary.m_hData);	// Lock memory
					pRowSet->SetBinary(nField, lpSrc, (int)nBinarylen, TRUE); // Set binary value.
					GlobalUnlock(binary.m_hData);				// Unlock memory
					}
				else
					pRowSet->SetBinary(nField, NULL, 0); // Clear binary value.
				}
			}
		// }}

		// Copy from VRowSet into serializable object. {{
		Serializable*	pObject	= pListRecords->CreateSerializableObject();
		if( copyPerformer.CopyFromVRow(pRowSet, pObject) > 0 ){
			pListRecords->Add(pObject);
			}
		else
			delete pObject;
		// }}
		}
	// }}		

	delete pTempObj;
	rstDummy.m_hstmt = NULL;
	// Free statement handle.
	FreeStatementHandle(hstmt);
	lock->Unlock();
	delete pRowSet;
	return pListRecords->GetCount();
	}

bool
iODBCRst::ExecuteSQL(CDatabase* pDb, const _String sSQL){
	SQLRETURN		rc; 
	SQLHSTMT		hstmt;

	CCriticalSection* lock	= GetSectionLock();
	lock->Lock();

	_String sSQL_			= sSQL;
	// Alloc statement.
	rc = SQLAllocStmt(pDb->m_hdbc, &hstmt);
	// Create a result set containing the ID and picture of each part.
	rc = SQLExecDirect(hstmt, (SQLCHAR*)sSQL_.GetBuffer(), SQL_NTS);
	if( rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO ){
		_String sError;
		sError.Format(_T("SQL=%s ErrorDesc=%s"), (const TCHAR*)sSQL, GetLastError(hstmt).GetBuffer());
		lock->Unlock();
		// Log error
		Logger::CreateLogEntry(sError.GetBuffer());
		return false;
		}

	// Free statement handle.
	FreeStatementHandle(hstmt);
	lock->Unlock();
	return true;
	}

////////////////////////////////////////////////////////////////
void  
iODBCRst::Close(){
	CODBCRecordset::Close();
	}

////////////////////////////////////////////////////////////////
//	iODBCRst::GetBaseTableName
////////////////////////////////////////////////////////////////
BOOL
iODBCRst::GetBaseTableName(int nCol, _String& sTableName){
	SQLSMALLINT nBaseNameLen = 0;
	char		szBaseTableName[255];
	szBaseTableName[0]		 = '\x00';
	::SQLColAttribute(m_hstmt, nCol, SQL_DESC_BASE_TABLE_NAME, szBaseTableName, 255, &nBaseNameLen, NULL);
	sTableName				 = szBaseTableName;
	return (nBaseNameLen > 0);
	}

////////////////////////////////////////////////////////////////

bool 
iODBCRst::SetupVirtualRowSet(){
	if( !m_pVRowSet ){
		m_pVRowSet				= new VirtualRowSet();
		UINT		nField		= 0;
		DWORD		dwDbvtType	= 0L;
		_String		sField		= _T("");

		while( nField < m_nFields ){
			dwDbvtType	= m_fields[nField].m_dwType;
			sField		= m_fields[nField].GetName();
			switch( dwDbvtType ){
				case DBVT_BOOL:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTBool); 
					break;
					};
				case DBVT_UCHAR:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTInt8);
					break;
					};
				case DBVT_STRING:{
					m_pVRowSet->AddFieldEx	(sField.GetBuffer(), FieldType::FTChar, 
											(int)(m_rgODBCFieldInfos[nField].m_nPrecision) + 1);
					break;
					};
				case DBVT_SHORT:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTInt16);
					break;
					}
				case DBVT_LONG:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTInt32);
					break;
					};
				case DBVT_BIGINT:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTInt64);
					break;
					};
				case DBVT_DATE:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTDateTime);
					break;
					};
				case DBVT_SINGLE:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTFloat);
					break;
					}
				case DBVT_DOUBLE:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTDouble);
					break;
					};
				case DBVT_BINARY:{
					m_pVRowSet->AddField(sField.GetBuffer(), FieldType::FTBinary);
					break;
					}
				};
			nField ++;
			}
		m_pVRowSet->Create();
		m_pVRowSet->AddRow();
		}
	return true;
	}

////////////////////////////////////////////////////////////////
void 
iODBCRst::DoFieldExchange( CFieldExchange* pFX ){
	pFX->SetFieldType(CFieldExchange::outputColumn);

	switch( pFX->m_nOperation ){
		case CFieldExchange::Name:{
			_String strName;
			int		nCt		= m_pVRowSet->GetFieldCount();
			for( UINT i = 0; i < nCt; i++ ){
				//if( !IsFieldStatusDirty(i) ) continue;
				VRowSetFieldInfo* pFieldInfo	= m_pVRowSet->GetFieldInfo(i);
				strName.Format("[%s]", pFieldInfo->szName);
				*pFX->m_pstr        += strName;
				*pFX->m_pstr        += pFX->m_lpszSeparator;
				pFX->m_nParamFields ++;
				}
			break;
			}
		case CFieldExchange::NameValue:{
			_String           strName, strValue;
			My_BindParam_Func bindParam_func	= NULL;
			int               nField			= 1;
			int				  nCt				= m_pVRowSet->GetFieldCount();
			void*             lpValue			= NULL;
			SQLLEN*           lpLength			= NULL;

			for( UINT i = 1; i < nCt; i++ ){
		//		if( !IsFieldStatusDirty(i)     ) continue;
				if( pFX->m_nParamFields > 0    )
					*pFX->m_pstr += pFX->m_lpszSeparator;

				CDBField*			pDBField	= &m_fields[i];
				VRowSetFieldInfo*	pFieldInfo	= m_pVRowSet->GetFieldInfo(i);
				bindParam_func					= (My_BindParam_Func)GetBindParamMethodsArray()->GetAt(pDBField->m_dwType);
				ASSERT( bindParam_func != NULL );

				lpValue		= m_pVRowSet->GetFieldValuePtr(i);
				lpLength	= (SQLLEN*)m_pVRowSet->GetFieldLengthPtr(i);
				
				// call my BindParam method. {{
				(*bindParam_func)(m_hstmtUpdate, nField, lpValue, lpLength, &m_rgODBCFieldInfos[i]);
				// }}
				
				strName.Format(_T("[%s]=?"), pFieldInfo->szName);
				*pFX->m_pstr        += strName;
				pFX->m_nParamFields ++;
				nField              ++;
				}

			VRowSetFieldInfo* pIdField = m_pVRowSet->GetFieldInfo(_T("ID"));
			if( pIdField )
				strValue.Format(" WHERE [%s]=%d,", pIdField->szName, m_pVRowSet->GetInt32(pIdField->nOrdinal));
			else
				ASSERT(FALSE);
			*pFX->m_pstr += strValue;
			m_nEditMode   = 0xFF;
			break;
			}
		case CFieldExchange::Value:{
			My_BindParam_Func bindParam_func = NULL;
			int					nField		= 1;
			int					nCt			= m_pVRowSet->GetFieldCount();
			void*				lpValue		= NULL;
			SQLLEN*				lpLength	= NULL;

			for( UINT i=0; i<nCt; i++ ){
				//if( !IsFieldStatusDirty(i)     ) continue;
				CDBField*			pDBField	= &m_fields[i];
				VRowSetFieldInfo*	pFieldInfo	= m_pVRowSet->GetFieldInfo(i);
				bindParam_func					= (My_BindParam_Func)GetBindParamMethodsArray()->GetAt(pDBField->m_dwType);
				ASSERT( bindParam_func != NULL );

				lpValue		= m_pVRowSet->GetFieldValuePtr(i);
				lpLength	= (SQLLEN*)m_pVRowSet->GetFieldLengthPtr(i);
				
				// Call my BindParam method. {{
				(*bindParam_func)(m_hstmtUpdate, nField, lpValue, lpLength, &m_rgODBCFieldInfos[i]);
				// }}

				*pFX->m_pstr        += _T("?");
				*pFX->m_pstr        += pFX->m_lpszSeparator;
				pFX->m_nParamFields ++;
				nField              ++;
				}
			}
		case CFieldExchange::BindFieldToColumn:{
			My_BindCol_Func bindCol_func	= NULL;
			HSTMT           hStmt			= pFX->m_prs->m_hstmt;
			void*			lpValue			= NULL;
			SQLLEN*			lpLength		= NULL;

			SetupVirtualRowSet();
			ASSERT(m_pVRowSet);

			int nLoop	= 0, nCount	= m_pVRowSet->GetFieldCount();
			while( nLoop < nCount ){
				CDBField* pDBField	= &m_fields[nLoop];
				bindCol_func		= (My_BindCol_Func)(GetBindFieldMethodsArray()->GetAt(pDBField->m_dwType));
				ASSERT( bindCol_func != NULL );
				lpValue				= m_pVRowSet->GetFieldValuePtr(nLoop);
				lpLength			= (SQLLEN*)m_pVRowSet->GetFieldLengthPtr(nLoop);

				// Call my BindCol method. {{
				(*bindCol_func)(hStmt, nLoop+1, lpValue, lpLength, &m_rgODBCFieldInfos[nLoop]);
				// }}
				nLoop ++;
				}
			pFX->m_nFields = nLoop;
			break;
			}

	    case CFieldExchange::Fixup:{
			int nLoop	= 0, nCount	= m_pVRowSet->GetFieldCount();
			void*	lpValue		= NULL;
			SQLLEN*	lpLength	= NULL;

			while( nLoop < nCount ){
				VRowSetFieldInfo* pInfo	= m_pVRowSet->GetFieldInfo(nLoop);
				ASSERT(pInfo);
				
				if( pInfo->bIsBlob ){ // Binary fields.
					lpValue		= m_pVRowSet->GetFieldValuePtr(nLoop);
					lpLength	= (SQLLEN*)m_pVRowSet->GetFieldLengthPtr(nLoop);
					
					SQLLEN nBinarylen	= pFX->GetLongBinarySize(nLoop+1);
					if( nBinarylen != SQL_NULL_DATA ){
						CLongBinary binary;
						pFX->GetLongBinaryData(nLoop+1, binary, &nBinarylen);

						void* lpSrc = GlobalLock(binary.m_hData);	// Lock memory
						m_pVRowSet->SetBinary(nLoop, lpSrc, (int)nBinarylen, TRUE); // Set binary value.
						GlobalUnlock(binary.m_hData);				// Unlock memory
						}
					else
						m_pVRowSet->SetBinary(nLoop, NULL, 0); // Clear binary value.
					}
				nLoop ++;
				}
			break;
			}

		case CFieldExchange::SetFieldNull:{
			void*   lpValue  = NULL;
			SQLLEN* lpLength = NULL;

			for( UINT i = 0; i < m_nFields; i++ ){
				lpValue			= m_pVRowSet->GetFieldValuePtr(i);
				lpLength		= (SQLLEN*)m_pVRowSet->GetFieldLengthPtr(i);
				*lpLength		= SQL_NULL_DATA;
				
				pFX->m_nFields     ++;
				pFX->m_nFieldFound ++;
				}
			break;
			}
		};
	}
////////////////////////////////////////////////////////////////
void 
iODBCRst::My_BindCol_Null(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	}

void 
iODBCRst::My_BindCol_Bool(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_BIT, lpValue, sizeof(bool), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_Byte(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_TINYINT, 
		                             lpValue, sizeof(BYTE), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_Short(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_C_SHORT, lpValue, sizeof(short), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_Int(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_INTEGER, lpValue, sizeof(int), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_BigInt(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_C_SBIGINT, lpValue, sizeof(__int64), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_Single(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_C_DOUBLE, lpValue, sizeof(double), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_Double(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_C_DOUBLE, lpValue, sizeof(double), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_Date(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_C_TIMESTAMP, 
		                             lpValue, sizeof(TIMESTAMP_STRUCT), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_String(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_CHAR, lpValue, (pInfo->m_nPrecision + 1), pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindCol_Binary(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;/*
	SQLRETURN retCode = ::SQLBindCol(hStmt, (UWORD)nField, (SWORD)SQL_BINARY, lpValue, pInfo->m_nPrecision, pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind col line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}*/
	}

void 
iODBCRst::My_BindParam_Null(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo)
	{
	}

void 
iODBCRst::My_BindParam_Bool(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	*pLength          = 1;
	SQLRETURN retCode = SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, SQL_BIT,
		                                 pInfo->m_nSQLType, pInfo->m_nPrecision,
		                                 pInfo->m_nScale, lpValue, 0, pLength);

	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_Byte(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo)
	{
	if( lpValue == NULL ) return;
	*pLength          = 3;
	SQLRETURN retCode = SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, SQL_TINYINT,
			  						     pInfo->m_nSQLType, pInfo->m_nPrecision,
		                                 pInfo->m_nScale, lpValue, 0, pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_Short(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	*pLength          = 10;
	SQLRETURN retCode = ::SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, (SWORD)SQL_C_LONG, 
		                                   pInfo->m_nSQLType, pInfo->m_nPrecision, 
										   pInfo->m_nScale, lpValue, 0, pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_Int(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo)
	{
	if( lpValue == NULL ) return;
	*pLength          = 10;
	SQLRETURN retCode = ::SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, (SWORD)SQL_C_LONG, 
		                                   pInfo->m_nSQLType, pInfo->m_nPrecision, 
										   pInfo->m_nScale, lpValue, 0, pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_BigInt(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo)
	{
	if( lpValue == NULL ) return;
	*pLength          = 10;
	SQLRETURN retCode = ::SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, (SWORD)SQL_C_SBIGINT, 
		                                   pInfo->m_nSQLType, pInfo->m_nPrecision, 
										   pInfo->m_nScale, lpValue, 0, pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_Single(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo)
	{
	if( lpValue == NULL ) return;
	*pLength          = 15;
	SQLRETURN retCode = SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, SQL_C_DOUBLE,
										 pInfo->m_nSQLType, pInfo->m_nPrecision, 
										 pInfo->m_nScale, lpValue, 0, pLength);

	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_Double(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo)
	{
	if( lpValue == NULL ) return;
	*pLength          = 15;
	SQLRETURN retCode = SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, SQL_C_DOUBLE, 
			   						     pInfo->m_nSQLType, pInfo->m_nPrecision, 
										 pInfo->m_nScale, lpValue, 0, pLength);
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_Date(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;

	// check for empty date.//
	TIMESTAMP_STRUCT* pDate = (TIMESTAMP_STRUCT*)lpValue;
	if( !pDate->year  || !pDate->month || !pDate->day ) {
		pDate->year     = AFX_RFX_TIMESTAMP_PSEUDO_NULL;
		pDate->month    = AFX_RFX_TIMESTAMP_PSEUDO_NULL;
		pDate->day      = AFX_RFX_TIMESTAMP_PSEUDO_NULL;
		pDate->hour     = 0;
		pDate->minute   = 0;
		pDate->second   = 0;
		pDate->fraction = AFX_RFX_TIMESTAMP_PSEUDO_NULL;
		*pLength        = SQL_NULL_DATA;
		}
	else{
		pDate->fraction = 0;
		*pLength        = 19;
		}
	//--------------------------


	SQLRETURN retCode = SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, SQL_C_TIMESTAMP,
									     pInfo->m_nSQLType, pInfo->m_nPrecision, 
										 pInfo->m_nScale, lpValue, 0, pLength);

	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_String(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLRETURN retCode = SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, SQL_C_CHAR, 
			    					     pInfo->m_nSQLType, pInfo->m_nPrecision, 
										 pInfo->m_nScale, lpValue, 0, pLength);
	
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void 
iODBCRst::My_BindParam_Binary(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo){
	if( lpValue == NULL ) return;
	SQLPOINTER pVal = *((void**)lpValue);
	SQLRETURN retCode = SQLBindParameter(hStmt, nField, SQL_PARAM_INPUT, SQL_C_BINARY, 
			    					     pInfo->m_nSQLType, pInfo->m_nPrecision, 
										 pInfo->m_nScale, pVal, 0, pLength);
	
	if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO ){
		TRACE2("can't bind param line = %d, function = %s",__LINE__,__FUNCTION__);
		ASSERT( FALSE );
		}
	}

void
iODBCRst::GetFieldIDs(CStringArray* pArrFields, CDWordArray* pArrIDs){
	ASSERT( pArrFields && pArrIDs );
	int		iLoop = 0;
	pArrIDs->RemoveAll();

	while( iLoop < pArrFields->GetCount() ){
		int nID = GetFieldID(pArrFields->GetAt(iLoop));
		if( nID != -1 )
			pArrIDs->Add(nID);
		iLoop	++;
		}
	}

long
iODBCRst::GetFieldNames(CStringArray* pArrFields){
	for( int nField = 0; nField < m_mapNameIdx.GetCount(); nField ++){
		_String		cName;
		void*		idx;
		POSITION	pos = m_mapNameIdx.GetStartPosition();
		while( pos != NULL ) {
			m_mapNameIdx.GetNextAssoc( pos, cName, idx );
			if( (int)(INT_PTR)idx == nField )
				pArrFields->Add(cName);
			}
		}
	return (int)pArrFields->GetCount();
	}

int 
iODBCRst::LoadTableNames(CDatabase* pDBase, CStringArray& arrTables)
	{
	/*
	HSTMT	hStmt           = NULL;
	char	szTableName[128]= {'\x00'};
	int		ret				= 0L;	

	// allocate new handle of statement.
	ret = SQLAllocHandle(SQL_HANDLE_STMT, pDBase->m_hdbc, &hStmt);
	if( ret != SQL_SUCCESS  ) 
		return 0;
	
	char pszTable[] = _T("'TABLE'\0");
	// prepare HSTMT to retrive table names.
	ret = SQLTables(hStmt, NULL, SQL_NTS, NULL, SQL_NTS, NULL,
		            SQL_NTS, (BYTE*)pszTable, SQL_NTS);

	if( ret == SQL_SUCCESS )
		{
		long size= 0;
		int loop = 0;

		while(1)
			{	
			ret = SQLFetch(hStmt);
			if( ret == SQL_NO_DATA_FOUND )
				break;
			SQLGetData(hStmt, 3, SQL_C_CHAR, szTableName, 128, &size);
			
			arrTables.Add(szTableName);
			loop ++;
			}
		ret = loop;
		}

	FreeStatementHandle(hStmt);
	return ret;
	*/

	CODBCRecordset* pRst = new CODBCRecordset(pDBase);

	try
		{
		pRst->Open(_T("select rdb$relation_name from rdb$relations where (rdb$system_flag is null or rdb$system_flag = 0);"));
		while( !pRst->IsEOF() ){
			_String sTBL = pRst->GetString(0);
			sTBL.TrimRight();
			arrTables.Add(sTBL);
			pRst->MoveNext();
			}

		pRst->Close();
		}
	catch(...)
		{
		}

	delete pRst;
	return (int)arrTables.GetCount();

	/*
	 select rdb$relation_name from rdb$relations where (rdb$system_flag is null or rdb$system_flag = 0);
	*/

	}




