#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ODBCRecordset.h"
#include "VRowSet.h"
#include "..\Serialize\Serializable.h"

// disable type cast warnings.[ZQR]
#pragma warning(disable:4311)
#pragma warning(disable:4312)
//---------------------------------

// disable deprecation warning.
#pragma warning(disable:4996)
//---------------------------------

#ifndef __AFXDB_H__
#define DBVT_NULL       0
#define DBVT_BOOL       1
#define DBVT_UCHAR      2
#define DBVT_SHORT      3
#define DBVT_LONG       4
#define DBVT_SINGLE     5
#define DBVT_DOUBLE     6
#define DBVT_DATE       7
#define DBVT_STRING     8
#define DBVT_BINARY     9
#define DBVT_ASTRING    10
#define DBVT_WSTRING    11
#endif

#ifndef DBVT_BIGINT
#define DBVT_BIGINT		12
#endif

typedef void (*My_BindCol_Func)  (HSTMT hStmt, int nField, void* lpValue, 
								  SQLLEN* pLength, CODBCFieldInfo* pInfo);
typedef void (*My_BindParam_Func)(HSTMT hStmt, int nField, void* lpValue, 
								  SQLLEN* pLength, CODBCFieldInfo* pInfo);

enum DatabaseType
{
	MSSQL = 1, // Default
	MSACCESS,
	MYSQL,
	FIREBIRDSQL,
	POSTGRESQL,
	SQLite,
};

	
/////////////////////////////////////////////////////////////////////////////
//	iODBCRst
/////////////////////////////////////////////////////////////////////////////

class iODBCRst : public CODBCRecordset
{
public:
	iODBCRst( CDatabase* pDatabase = NULL );
	virtual	~iODBCRst();

	virtual	BOOL			Open				(LPCTSTR lpszSQL, UINT    nOpenType = AFX_DB_USE_DEFAULT_TYPE, DWORD   dwOptions = 0);
	virtual void			Close				();

	// Loads records as serializable object. 
	// Returns count of loaded records.
	static int				LoadRecords			(CDatabase* pDb, EnumerableObject<Serializable> *pListRecords, DatabaseType databaseType,
												const _String sWhereSQL = _T(""), const _String sOrderBySQL = _T(""),  const _String sTop = _T(""), bool bUseDbFieldNames = true);

	static int				LoadRecordsBySql	(CDatabase* pDb, EnumerableObject<Serializable> *pListRecords, DatabaseType databaseType,
												const _String sSQL, bool bUseDbFieldNames = true);

	static bool				ExecuteSQL			(CDatabase* pDb, const _String sSQL);

	// Update/AddNew record from serializable object.
	static bool				SaveRecord			(CDatabase* pDBase, Serializable* pSave, DatabaseType databaseType, CStringVector* pArrDirtyFieldNames = NULL);
	// Deletes current record.
	static bool				DeleteRecord		(CDatabase* pDBase, Serializable* pDelete, DatabaseType databaseType);
	// Refresh (reload) record from database.
	static bool				RefreshRecord		(CDatabase* pDBase, Serializable* pRefresh, DatabaseType databaseType);
	// Execute procedure
	static bool				ExecuteProcedure	(CDatabase* pDBase, Serializable* pProcedureDesc, DatabaseType databaseType);

	static bool				FreeStatementHandle	(SQLHSTMT hstmt);

	static _String			GetLastError		(SQLHSTMT hstmt);

	static	CCriticalSection*	GetSectionLock	()
	{
		static CCriticalSection secLock;
		return &secLock;
	}

	void					GetFieldIDs			(CStringArray* pArrFields, CDWordArray* pArrIDs);
	long					GetFieldNames		(CStringArray* pArrFields);
	BOOL					IsValidField        (int nField, DWORD dbvtType);
	BYTE*					GetFieldFlagsArray  ();
	int						GetFieldsCount      ();

	static int				LoadTableNames		(CDatabase* pDBase, CStringArray& arrTables);
	BOOL					GetBaseTableName	(int nCol, _String& sTableName);

protected:
	static CPtrArray* GetBindFieldMethodsArray(){
		static CPtrArray _arrBindFieldMethods;
		if( !_arrBindFieldMethods.GetCount() ){
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Null)  );     // DBVT_NULL
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Bool)  );     // DBVT_BOOL
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Byte)  );     // DBVT_UCHAR
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Short) );     // DBVT_SHORT
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Int)  );		// DBVT_LONG
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Single));     // DBVT_SINGLE
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Double));     // DBVT_DOUBLE
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Date)  );     // DBVT_DATE
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_String));     // DBVT_STRING
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Binary));     // DBVT_BINARY
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Null));		// DBVT_ASTRING
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_Null));		// DBVT_WSTRING
			_arrBindFieldMethods  .Add((void*)((PROC)iODBCRst::My_BindCol_BigInt));     // DBVT_BIGINT
			}
		return &_arrBindFieldMethods;
		}

	static CPtrArray* GetBindParamMethodsArray(){
		static CPtrArray _arrBindParamMethods;
		if( !_arrBindParamMethods.GetCount() ){
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Null)  );     // DBVT_NULL
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Bool)  );     // DBVT_BOOL
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Byte)  );     // DBVT_UCHAR
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Short)  );    // DBVT_SHORT
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Int)  );	  // DBVT_LONG
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Single));     // DBVT_SINGLE
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Double));     // DBVT_DOUBLE
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Date)  );     // DBVT_DATE
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_String));     // DBVT_STRING
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Binary));     // DBVT_BINARY
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Null));		  // DBVT_ASTRING
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_Null));       // DBVT_WSTRING
			_arrBindParamMethods  .Add((void*)((PROC)iODBCRst::My_BindParam_BigInt));     // DBVT_BIGINT
			}
		return &_arrBindParamMethods;
		}

	// VariableType from DBVT type
	static VariableType VarTypeFromDBVT(DWORD dwDbvt){
		VariableType type = VariableType::VT_None;
		static AutoSortedArray arrVarTypeToDBVT;
		if( !arrVarTypeToDBVT.GetCount() ){
			arrVarTypeToDBVT.AddAsLong(DBVT_BOOL,		VariableType::VT_Bool);
			arrVarTypeToDBVT.AddAsLong(DBVT_UCHAR,	VariableType::VT_Int8);
			arrVarTypeToDBVT.AddAsLong(DBVT_SHORT,	VariableType::VT_Int16);
			arrVarTypeToDBVT.AddAsLong(DBVT_LONG,		VariableType::VT_Int32);
			arrVarTypeToDBVT.AddAsLong(DBVT_SINGLE,	VariableType::VT_Float);
			arrVarTypeToDBVT.AddAsLong(DBVT_DOUBLE,	VariableType::VT_Double);
			arrVarTypeToDBVT.AddAsLong(DBVT_DATE,		VariableType::VT_DateTime);
			arrVarTypeToDBVT.AddAsLong(DBVT_STRING,	VariableType::VT_String);
			arrVarTypeToDBVT.AddAsLong(DBVT_BINARY,	VariableType::VT_Binary);
			arrVarTypeToDBVT.AddAsLong(DBVT_BIGINT,	VariableType::VT_Int64);
			}
		int nIndex = arrVarTypeToDBVT.IndexOf((void*)dwDbvt);
		if( nIndex != -1 )
			type = (VariableType)arrVarTypeToDBVT.GetDataAsLong(nIndex);
		return type;
		}

	// FieldType from DBVT type
	static FieldType FieldTypeFromDBVT(DWORD dwDbvt){
		FieldType type = FieldType::FTNone;
		static AutoSortedArray arrFieldTypeToDBVT;
		if( !arrFieldTypeToDBVT.GetCount() ){
			arrFieldTypeToDBVT.AddAsLong(DBVT_BOOL,		FieldType::FTBool);
			arrFieldTypeToDBVT.AddAsLong(DBVT_UCHAR,	FieldType::FTInt8);
			arrFieldTypeToDBVT.AddAsLong(DBVT_SHORT,	FieldType::FTInt16);
			arrFieldTypeToDBVT.AddAsLong(DBVT_LONG,		FieldType::FTInt32);
			arrFieldTypeToDBVT.AddAsLong(DBVT_SINGLE,	FieldType::FTDouble);
			arrFieldTypeToDBVT.AddAsLong(DBVT_DOUBLE,	FieldType::FTDouble);
			arrFieldTypeToDBVT.AddAsLong(DBVT_DATE,		FieldType::FTDateTime);
			arrFieldTypeToDBVT.AddAsLong(DBVT_STRING,	FieldType::FTChar);
			arrFieldTypeToDBVT.AddAsLong(DBVT_BINARY,	FieldType::FTBinary);
			arrFieldTypeToDBVT.AddAsLong(DBVT_BIGINT,	FieldType::FTInt64);
			}
		int nIndex = arrFieldTypeToDBVT.IndexOf((void*)dwDbvt);
		if( nIndex != -1 )
			type = (FieldType)arrFieldTypeToDBVT.GetDataAsLong(nIndex);
		return type;
		}

	bool SetupVirtualRowSet();

protected:
	VirtualRowSet*	m_pVRowSet;
	 
protected:
	static void My_BindCol_Null    (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Bool    (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Byte    (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Short   (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Int		(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_BigInt	(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Single  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Double  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Date    (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_String  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindCol_Binary  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);

	static void My_BindParam_Null  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Bool  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Byte  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Short (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Int   (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_BigInt(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Single(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Double(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Date  (HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_String(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	static void My_BindParam_Binary(HSTMT hStmt, int nField, void* lpValue, SQLLEN* pLength, CODBCFieldInfo* pInfo);
	
	virtual	void	DoFieldExchange		(CFieldExchange* pFX);
};

__inline BOOL    
iODBCRst::IsValidField(int nField, DWORD dbvtType)
	{
	if( nField > -1 && nField < (int)m_nFields )
		return (BOOL)(m_fields[nField].m_dwType == dbvtType);
	return FALSE;
	}

__inline BYTE*    
iODBCRst::GetFieldFlagsArray(){
	return m_pbFieldFlags;
	}

__inline int      
iODBCRst::GetFieldsCount(){
	return m_nFields;
	}