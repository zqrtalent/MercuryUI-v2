// ===========================================================================
// About: MFC new generation data serialization class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#include "stdafx.h"
#include "Serializable.h"
#include "../XML/Markup.h"
#include "../Path/Path.h"
#include "../Utility/GrowableMemory.h"
#include "../Data/DataFieldCopier.h"
//#include <afxdb.h>	// CLongBinary

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================
//
//	class VariableInfoObject
//
//=============================================

VariableInfoObject::VariableInfoObject(){
	}

BEGIN_DECLARE_VAR(VariableInfoObject, Serializable)
DECLARE_VAR(_T("VariableInfoObject"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Name"),				_T(""), VariableType::VT_String, offsetof(VariableInfoObject,m_sName), false, false, false)
	DECLARE_VAR(_T("DbName"),			_T(""), VariableType::VT_String, offsetof(VariableInfoObject,m_sDbName), false, false, false)
	DECLARE_VAR(_T("Type"),				_T(""), VariableType::VT_Int8, offsetof(VariableInfoObject,m_btType), false, false, false)
END_DECLARE_VAR()

VariableInfoObject::~VariableInfoObject(){
	}

//=============================================
//
//	class Serializable
//
//=============================================

int
Serializable::Compare_Int8(void* lpValue1, void* lpValue2){
	if( *((char*)lpValue1) == *((char*)lpValue2) )
		return 0;
	else
	if( *((char*)lpValue1) > *((char*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_Int8_Desc(void* lpValue1, void* lpValue2){
	if( *((char*)lpValue1) == *((char*)lpValue2) )
		return 0;
	else
	if( *((char*)lpValue1) > *((char*)lpValue2) )
		return -1;
	return 1;
	}

int
Serializable::Compare_Int16(void* lpValue1, void* lpValue2){
	if( *((short*)lpValue1) == *((short*)lpValue2) )
		return 0;
	else
	if( *((short*)lpValue1) > *((short*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_Int16_Desc(void* lpValue1, void* lpValue2){
	if( *((short*)lpValue1) == *((short*)lpValue2) )
		return 0;
	else
	if( *((short*)lpValue1) > *((short*)lpValue2) )
		return -1;
	return 1;
	}

int
Serializable::Compare_Int32(void* lpValue1, void* lpValue2){
	if( *((int*)lpValue1) == *((int*)lpValue2) )
		return 0;
	else
	if( *((int*)lpValue1) > *((int*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_Int32_Desc(void* lpValue1, void* lpValue2){
	if( *((int*)lpValue1) == *((int*)lpValue2) )
		return 0;
	else
	if( *((int*)lpValue1) > *((int*)lpValue2) )
		return -1;
	return 1;
	}

int
Serializable::Compare_Int64(void* lpValue1, void* lpValue2){
	if( *((__int64*)lpValue1) == *((__int64*)lpValue2) )
		return 0;
	else
	if( *((__int64*)lpValue1) > *((__int64*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_Int64_Desc(void* lpValue1, void* lpValue2){
	if( *((__int64*)lpValue1) == *((__int64*)lpValue2) )
		return 0;
	else
	if( *((__int64*)lpValue1) > *((__int64*)lpValue2) )
		return -1;
	return 1;
	}

int
Serializable::Compare_Float(void* lpValue1, void* lpValue2){
	if( *((float*)lpValue1) == *((float*)lpValue2) )
		return 0;
	else
	if( *((float*)lpValue1) > *((float*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_Float_Desc(void* lpValue1, void* lpValue2){
	if( *((float*)lpValue1) == *((float*)lpValue2) )
		return 0;
	else
	if( *((float*)lpValue1) > *((float*)lpValue2) )
		return -1;
	return 1;
	}

int
Serializable::Compare_Double(void* lpValue1, void* lpValue2){
	if( *((double*)lpValue1) == *((double*)lpValue2) )
		return 0;
	else
	if( *((double*)lpValue1) > *((double*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_Double_Desc(void* lpValue1, void* lpValue2){
	if( *((double*)lpValue1) == *((double*)lpValue2) )
		return 0;
	else
	if( *((double*)lpValue1) > *((double*)lpValue2) )
		return -1;
	return 1;
	}

int
Serializable::Compare_String(void* lpValue1, void* lpValue2){
	if( *((_String*)lpValue1) == *((_String*)lpValue2) )
		return 0;
	else
	if( *((_String*)lpValue1) > *((_String*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_String_Desc(void* lpValue1, void* lpValue2){
	if( *((_String*)lpValue1) == *((_String*)lpValue2) )
		return 0;
	else
	if( *((_String*)lpValue1) > *((_String*)lpValue2) )
		return -1;
	return 1;
	}

int
Serializable::Compare_DateTime(void* lpValue1, void* lpValue2){
	if( *((DateTime*)lpValue1) == *((DateTime*)lpValue2) )
		return 0;
	else
	if( *((DateTime*)lpValue1) > *((DateTime*)lpValue2) )
		return 1;
	return -1;
	}

int
Serializable::Compare_DateTime_Desc(void* lpValue1, void* lpValue2){
	if( *((DateTime*)lpValue1) == *((DateTime*)lpValue2) )
		return 0;
	else
	if( *((DateTime*)lpValue1) > *((DateTime*)lpValue2) )
		return -1;
	return 1;
	}

_String
Serializable::GetClassName(){
	const AFX_VARINFOMAP* pInfoMap = GetVariableInfoMap();
	if( pInfoMap == NULL )
		return _T("");
	return pInfoMap->lpVarInfos[0].m_sAttributeName;
	}

VariableInfo*
Serializable::GetThisInfo(){
	const AFX_VARINFOMAP* pInfoMap = GetVariableInfoMap();
	if( pInfoMap == NULL )
		return NULL;
	return (VariableInfo*)pInfoMap->lpVarInfos;
	}

VariableInfo*
Serializable::GetDbFieldInfo(const _String sDbField){
	VariableInfo* pInfo = GetThisInfo();
	if( !pInfo )
		return NULL;

	int nLoop = 1;
	while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
		if( !pInfo[nLoop].m_sDbFieldName.CompareNoCase(sDbField) )
			return &pInfo[nLoop];
		nLoop ++;
		}
	return NULL;
	}

VariableInfo*
Serializable::GetVariableInfo(const _String sAttribName){
	const AFX_VARINFOMAP* pInfoMap = GetVariableInfoMap();
	if( pInfoMap == NULL )
		return NULL;
	
	AutoSortedArray*	pArrVariableInfosByName = GetArrayOfVariableInfosByName();
	VariableInfo*		pInfoRet = NULL;
	if( pArrVariableInfosByName ){
		// Initialize Array of variable infos only once.
		if( pArrVariableInfosByName->GetCount() == 0 ){
			while( pInfoMap->lpVarInfos ){
				VariableInfo* pInfo = (VariableInfo*)pInfoMap->lpVarInfos;
				int nLoop = 1;
				while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
					if( !pInfo[nLoop].m_sAttributeName.CompareNoCase(sAttribName) )
						pInfoRet = &pInfo[nLoop];
					pArrVariableInfosByName->Add((void*)DecodeString((LPCTSTR)pInfo[nLoop].m_sAttributeName), (void*)&pInfo[nLoop]);
					nLoop ++;
					}
				pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
				}
			}
		else{
			UINT	dwAttribNameAsCode	= DecodeString((LPCTSTR)sAttribName);
			int		nIndex				= pArrVariableInfosByName->IndexOf((void*)dwAttribNameAsCode);
			if( nIndex != -1 )
				pInfoRet = (VariableInfo*)pArrVariableInfosByName->GetData(nIndex);
			}
		return pInfoRet;
		}

	while( pInfoMap->lpVarInfos ){
		VariableInfo* pInfo = (VariableInfo*)pInfoMap->lpVarInfos;
		int nLoop = 1;
		while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
			if( !pInfo[nLoop].m_sAttributeName.CompareNoCase(sAttribName) )
				return &pInfo[nLoop];
			nLoop ++;
			}
		pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		}
	return NULL;
	}

void*
Serializable::GetVariablePtr(const _String sAttribName){
	VariableInfo* pInfoVar = GetVariableInfo(sAttribName);
	if( !pInfoVar )
		return NULL;
	return (void*)&((BYTE*)this)[pInfoVar->m_nOffset];
	}

int
Serializable::GetIdFieldValue(VariableInfo** ppInfo /*= NULL*/){
	VariableInfo* pInfo = GetDbFieldInfo(_T("ID"));
	if( !pInfo ){
		if( ppInfo )
			*ppInfo = NULL;
		return -1;
		}
	if( ppInfo )
		*ppInfo = pInfo;
	int nId = _ttoi(GetVariableAsString(pInfo));
	return nId;
	}

bool
Serializable::GetSelectSQL(_String& sSQL, const _String sTop /*= _T("")*/){
	VariableInfo*	pInfo	= GetThisInfo();
	if( !pInfo )
		return false;

	int		nLoop		= 1;
	_String	sSQLTemp	= !sTop.IsEmpty() ? (_T("SELECT ") + sTop + _T(" ")) : _T("SELECT ");
	int		nField		= 0;
	_String	sTableName	= pInfo[0].m_sDbFieldName;
	if( sTableName.IsEmpty() )
		return false;

	while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
		if( !pInfo[nLoop].m_sDbFieldName.IsEmpty() && !pInfo[nLoop].m_bComplexType ){
			sSQLTemp	+= pInfo[nLoop].m_sDbFieldName + _T(", ");
			nField		++;
			}
		nLoop ++;
		}

	if( nField > 0 ){
		// Delete last two symbols ', '
		sSQLTemp.Delete(sSQLTemp.GetLength() - 2, 2); 
		sSQL = sSQLTemp + _T(" FROM ") + sTableName;
		return true;
		}
	return false;
	}

bool
Serializable::GetDeleteSQL(_String& sDeleteSQL){
	VariableInfo*	pInfo	= GetThisInfo();
	if( !pInfo )
		return false;

	VariableInfo*	pInfoId		= NULL;
	int				nId			= GetIdFieldValue(&pInfoId);
	_String			sTableName	= pInfo[0].m_sDbFieldName;
	if( sTableName.IsEmpty() || nId <= 0 || !pInfoId )
		return false;

	sDeleteSQL.Format(_T("DELETE FROM [%s] WHERE ID=%d"), sTableName.GetBuffer(), nId);
	return true;
	}

bool
Serializable::GetUpdateSQL(_String& sUpdateSQL, List<SQLExecParamInfo>* pParameters, CStringVector* pArrDirtyFieldNames /*= NULL*/){
	VariableInfo*	pInfo	= GetThisInfo();
	if( !pInfo )
		return false;

	VariableInfo*	pInfoId		= NULL;
	int				nLoop		= 1;
	_String			sSQLTemp	= _T("UPDATE ");
	int				nField		= 0;
	int				nId			= GetIdFieldValue(&pInfoId);
	_String			sTableName	= pInfo[0].m_sDbFieldName;
	_String			sFormat;
	if( sTableName.IsEmpty() || nId <= 0 || !pInfoId )
		return false;

	sSQLTemp += sTableName;
	sSQLTemp += " SET ";

	// Custom variables.
	if( pArrDirtyFieldNames && pArrDirtyFieldNames->size() > 0  ){
		nLoop	= 0;
		int nCt = pArrDirtyFieldNames->size();
		while( nLoop < nCt ){
			VariableInfo*	pInfo = GetDbFieldInfo(pArrDirtyFieldNames->at(nLoop));
			if( pInfoId == pInfo || !pInfo ){
				nLoop ++;
				continue;
				}

			if( !pInfo->m_sDbFieldName.IsEmpty() && !pInfo->m_bComplexType ){
				// Add parameter info. {{
				SQLExecParamInfo*	pParamInfo	= new SQLExecParamInfo();
				pParamInfo->sName				= pInfo->m_sDbFieldName;
				pParamInfo->nField				= nLoop;
				pParamInfo->cValSize			= _field_default_size_by_type[pInfo->m_type];
				pParamInfo->lpValue				= (void*)&((LPBYTE)this)[pInfo->m_nOffset];
				pParamInfo->wSQLType			= (SQLSMALLINT)SQLTypeFromVarType(pInfo->m_type);
				pParamInfo->wSQLCType			= (SQLSMALLINT)SQLCTypeFromVarType(pInfo->m_type);
				pParamInfo->dwParameterType		= SQL_PARAM_INPUT;
				ASSERT(pParamInfo->wSQLType);
				pParameters->Add(pParamInfo);
				// }}

				sFormat.Format(_T("[%s]=?,"), ((TCHAR*)pInfo->m_sDbFieldName.GetBuffer()));
				sSQLTemp		+= sFormat;
				nField			++;
				}
			nLoop ++;
			}
		}
	else{ // All variables.
		while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
			if( pInfoId == &pInfo[nLoop] ){
				nLoop ++;
				continue;
				}

			if( !pInfo[nLoop].m_sDbFieldName.IsEmpty() && !pInfo[nLoop].m_bComplexType ){
				// Add parameter info. {{
				SQLExecParamInfo*	pParamInfo	= new SQLExecParamInfo();
				pParamInfo->sName				= pInfo[nLoop].m_sDbFieldName;
				pParamInfo->nField				= (nLoop - 1);
				pParamInfo->cValSize			= _field_default_size_by_type[pInfo[nLoop].m_type];
				pParamInfo->lpValue				= (void*)&((LPBYTE)this)[pInfo[nLoop].m_nOffset];
				pParamInfo->wSQLType			= (SQLSMALLINT)SQLTypeFromVarType(pInfo[nLoop].m_type);
				pParamInfo->wSQLCType			= (SQLSMALLINT)SQLCTypeFromVarType(pInfo[nLoop].m_type);
				pParamInfo->dwParameterType		= SQL_PARAM_INPUT;
				ASSERT(pParamInfo->wSQLType);
				pParameters->Add(pParamInfo);
				// }}

				sFormat.Format(_T("[%s]=?,"), (const TCHAR*)pInfo[nLoop].m_sDbFieldName.GetBuffer());
				sSQLTemp		+= sFormat;
				nField			++;
				}
			nLoop ++;
			}
		}

	if( nField > 0 ){
		// Delete last symbol ', '
		sSQLTemp.Delete(sSQLTemp.GetLength() - 1, 1); 
		// Add where part of SQL.
		sFormat.Format(_T(" WHERE [ID]=%d"), nId);
		sSQLTemp	+= sFormat;
		sUpdateSQL	= sSQLTemp;
		return true;
		}
	return false;
	}

bool
Serializable::GetAddNewSQL(_String& sAddNewSQL, List<SQLExecParamInfo>* pParameters, bool bReturnIdentity, CStringVector* pArrDirtyFieldNames /*= NULL*/){
	VariableInfo*	pInfo	= GetThisInfo();
	if( !pInfo )
		return false;

	VariableInfo*	pInfoId		= NULL;
	int				nLoop		= 1;
	_String			sSQLTemp	= _T("INSERT INTO ");
	int				nField		= 0;
	int				nId			= GetIdFieldValue(&pInfoId);
	_String			sTableName	= pInfo[0].m_sDbFieldName;
	_String			sFormat;
	if( sTableName.IsEmpty() || !pInfoId )
		return false;

	sSQLTemp += sTableName;
	sSQLTemp += " (";

	_String sFieldList;
	_String sValueList;
	int		nIdField = -1;

	if( pArrDirtyFieldNames && pArrDirtyFieldNames->size() > 0  ){
		nLoop	= 0;
		int nCt = pArrDirtyFieldNames->size();
		while( nLoop < nCt ){
			VariableInfo*	pInfo = GetDbFieldInfo(pArrDirtyFieldNames->at(nLoop));
			if( !pInfo ){
				nLoop	++;
				continue;
				}
			if( pInfoId == pInfo ){
				nIdField = (nLoop - 1);
				nLoop	++;
				continue;
				}

			if( !pInfo->m_sDbFieldName.IsEmpty() && !pInfo->m_bComplexType ){
				// Add parameter info. {{
				SQLExecParamInfo*	pParamInfo	= new SQLExecParamInfo();
				pParamInfo->sName				= pInfo->m_sDbFieldName;
				pParamInfo->nField				= nLoop;
				pParamInfo->cValSize			= _field_default_size_by_type[pInfo->m_type];
				pParamInfo->lpValue				= (void*)&((LPBYTE)this)[pInfo->m_nOffset];
				pParamInfo->wSQLType			= (SQLSMALLINT)SQLTypeFromVarType(pInfo->m_type);
				pParamInfo->wSQLCType			= (SQLSMALLINT)SQLCTypeFromVarType(pInfo->m_type);
				pParamInfo->dwParameterType		= SQL_PARAM_INPUT;
				ASSERT(pParamInfo->wSQLType);
				pParameters->Add(pParamInfo);
				// }}

				sFieldList		+= "[" + pInfo->m_sDbFieldName.GetBuffer() + "],";
				sValueList		+= _T("?,");
				nField			++;
				}
			nLoop ++;
			}
		}
	else{ // All variables.
		while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
			if( pInfoId == &pInfo[nLoop] ){
				nIdField = (nLoop - 1);
				nLoop	++;
				continue;
				}
			if( !pInfo[nLoop].m_sDbFieldName.IsEmpty() && !pInfo[nLoop].m_bComplexType ){
				// Add parameter info. {{
				SQLExecParamInfo*	pParamInfo	= new SQLExecParamInfo();
				pParamInfo->sName				= pInfo[nLoop].m_sDbFieldName;
				pParamInfo->nField				= (nLoop - 1);
				pParamInfo->cValSize			= _field_default_size_by_type[pInfo[nLoop].m_type];
				pParamInfo->lpValue				= (void*)&((LPBYTE)this)[pInfo[nLoop].m_nOffset];
				pParamInfo->wSQLType			= (SQLSMALLINT)SQLTypeFromVarType(pInfo[nLoop].m_type);
				pParamInfo->wSQLCType			= (SQLSMALLINT)SQLCTypeFromVarType(pInfo[nLoop].m_type);
				pParamInfo->dwParameterType		= SQL_PARAM_INPUT;
				ASSERT(pParamInfo->wSQLType);
				pParameters->Add(pParamInfo);
				// }}

				sFieldList		+= "[" + pInfo[nLoop].m_sDbFieldName + "],";
				sValueList		+= _T("?,");
				nField			++;
				}
			nLoop ++;
			}
		}

	if( nField > 0 ){
		// Delete last symbol ',' {{
		sFieldList.Delete(sFieldList.GetLength() - 1, 1); 
		sValueList.Delete(sValueList.GetLength() - 1, 1); 
		// }}
		sSQLTemp	+= sFieldList + ") VALUES (" + sValueList + ")";
		sAddNewSQL	= sSQLTemp;

		// Add identity field as output parameter. {{
		if( bReturnIdentity ){
			SQLExecParamInfo*	pParamInfo	= new SQLExecParamInfo();
			pParamInfo->sName				= pInfo[nIdField].m_sDbFieldName;
			pParamInfo->nField				= nIdField;
			pParamInfo->cValSize			= _field_default_size_by_type[pInfoId->m_type];
			pParamInfo->lpValue				= (void*)&((LPBYTE)this)[pInfoId->m_nOffset];
			pParamInfo->wSQLType			= (SQLSMALLINT)SQLTypeFromVarType(pInfoId->m_type);
			pParamInfo->wSQLCType			= (SQLSMALLINT)SQLCTypeFromVarType(pInfoId->m_type);
			pParamInfo->dwParameterType		= SQL_PARAM_OUTPUT;
			ASSERT(pParamInfo->wSQLType);
			pParameters->Add(pParamInfo);

			// Transform Insert SQL.
			sAddNewSQL.Format(_T("BEGIN TRANSACTION %s; SELECT ? = @@IDENTITY; COMMIT TRANSACTION"), sSQLTemp.GetBuffer());
//			sAddNewSQL.Format(_T(" %s; SELECT ? = @@IDENTITY;"), sSQLTemp);
			}
		// {{
		return true;
		}
	return false;
	}

bool
Serializable::GetProcedureSQL(_String& sProcSQL, List<SQLExecParamInfo>* pParameters){
	VariableInfo*	pInfo	= GetThisInfo();
	if( !pInfo )
		return false;

	// {? = CALL procname (?,?)}

	int		nLoop		= 1;
	_String	sSQLTemp	= _T("CALL ");
	int		nField		= 0;
	_String	sProcName	= pInfo[0].m_sDbFieldName;
	if( sProcName.IsEmpty() )
		return false;

	sSQLTemp += sProcName;
	sSQLTemp += " (";

	while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
		//_field_default_size_by_type
		if( pInfo[nLoop].m_bComplexType || pInfo[nLoop].m_bEnumerable ){
			nLoop ++;
			continue;
			}

		_String sDbField = pInfo[nLoop].m_sDbFieldName;
		if( !sDbField.CompareNoCase(_T("IN")) ||
			!sDbField.CompareNoCase(_T("OUT"))
			){	// Input or output parameter.
			SQLExecParamInfo*	pParamInfo	= new SQLExecParamInfo();
			pParamInfo->nField				= (nLoop - 1);
			pParamInfo->sName				= pInfo[nLoop].m_sDbFieldName;
			pParamInfo->cValSize			= _field_default_size_by_type[pInfo[nLoop].m_type];
			pParamInfo->lpValue				= (void*)&((LPBYTE)this)[pInfo[nLoop].m_nOffset];
			pParamInfo->wSQLType			= (SQLSMALLINT)SQLTypeFromVarType(pInfo[nLoop].m_type);
			pParamInfo->wSQLCType			= (SQLSMALLINT)SQLCTypeFromVarType(pInfo[nLoop].m_type);
			if( !sDbField.CompareNoCase(_T("IN")) )
				pParamInfo->dwParameterType	= SQL_PARAM_INPUT;
			else
				pParamInfo->dwParameterType	= SQL_PARAM_OUTPUT;

			ASSERT(pParamInfo->wSQLType);
			pParameters->Add(pParamInfo);

			sSQLTemp		+= "?,";
			nField			++;
			}
		else{	// Return parameter.
			if( !sDbField.CompareNoCase(_T("RET")) ){
				sSQLTemp.Insert(0, _T("? = "));

				SQLExecParamInfo*	pParamInfo	= new SQLExecParamInfo();
				pParamInfo->sName				= pInfo[nLoop].m_sDbFieldName;
				pParamInfo->nField				= nField;
				pParamInfo->cValSize			= _field_default_size_by_type[pInfo[nLoop].m_type];
				pParamInfo->lpValue				= (void*)&((LPBYTE)this)[pInfo[nLoop].m_nOffset];
				pParamInfo->wSQLType			= (SQLSMALLINT)SQLTypeFromVarType(pInfo[nLoop].m_type);
				pParamInfo->wSQLCType			= (SQLSMALLINT)SQLCTypeFromVarType(pInfo[nLoop].m_type);
				pParamInfo->dwParameterType		= SQL_PARAM_OUTPUT;
				ASSERT(pParamInfo->wSQLType);
				pParameters->Add(pParamInfo);
				nField ++;
				}	
			}
		nLoop ++;
		}
	
	if( nField > 0 ){
		// Delete last symbol ','
		if( sSQLTemp.GetAt(sSQLTemp.GetLength()-1) == ',' ){
			sSQLTemp.Delete(sSQLTemp.GetLength()-1, 1); 
			sProcSQL.Format(_T("{%s)}"), sSQLTemp.GetBuffer());
			}
		else{ // No parameters supplied.
			if( sSQLTemp.GetAt(sSQLTemp.GetLength()-1) == '(' )
				sSQLTemp.Delete(sSQLTemp.GetLength()-1, 1); 
			sProcSQL.Format(_T("{%s}"), sSQLTemp.GetBuffer());
			}
		return true;
		}
	else{
		sProcSQL.Format(_T("{%s}"), sSQLTemp.GetBuffer());
		return true;
		}
	return false;
	}

VirtualRowSet*
Serializable::SerializableObjectToVRowSet(EnumerableObject<Serializable>* pObjects){
	Serializable*	pTempObj	= pObjects->CreateSerializableObject();
	ASSERT(pTempObj);

	VirtualRowSet*	pVRowSet = new VirtualRowSet();
	VirtualRowSet::CreateFromSerializable(pTempObj, NULL, pVRowSet);
	if( !pVRowSet ){
		delete pTempObj;
		return NULL;
		}

	DataFieldCopier copyPerformer(pVRowSet, pTempObj);
	int				nLoop		= 0;
	int				nCount		= pObjects->GetCount();
	delete			pTempObj;

	pVRowSet->AddRow();
	while( nLoop < nCount ){
		if( copyPerformer.CopyFromSerializable(pVRowSet, pObjects->GetAt(nLoop)) ){
			pVRowSet->AddRow();
			}
		nLoop ++;
		}
	// Delete last row. {{
	pVRowSet->DeleteRow(pVRowSet->GetRowCount() - 1);
	// }}
	return pVRowSet;
	}
/*
bool
Serializable::FreeBinary(CLongBinary* pBinary){
	if( !pBinary )
		return false;

	int nSizeOld = ::GlobalSize(pBinary->m_hData);
	if( nSizeOld > 0 ){
		::GlobalFree(pBinary->m_hData);
		}
	pBinary->m_hData		= 0L;
	pBinary->m_dwDataLength	= 0;
	return true;
	}

bool
Serializable::GlobalReAlloc(CLongBinary* pBinary, int nSizeNew, void* lpCopy){
	if( !pBinary || nSizeNew <= 0 )
		return false;

	int nSizeOld = (int)::GlobalSize(pBinary->m_hData);
	if( nSizeOld <= 0 ){
		pBinary->m_hData		= ::GlobalAlloc(GMEM_MOVEABLE, nSizeNew);
		pBinary->m_dwDataLength	= nSizeNew;
		}
	else{
		if( nSizeNew > nSizeOld ){
			HGLOBAL hGlobalOld		= pBinary->m_hData;
			pBinary->m_hData		= ::GlobalReAlloc(pBinary->m_hData, nSizeNew, GMEM_MOVEABLE);
			pBinary->m_dwDataLength	= nSizeNew;
			}
		}

	if( lpCopy != NULL ){
		void* lpDest = ::GlobalLock(pBinary->m_hData);
		memcpy(lpDest, lpCopy, nSizeNew);
		::GlobalUnlock(pBinary->m_hData);
		}
	return true;
	}

bool
Serializable::LoadJpgImage(Image* pImage, _String sJpgFile){
	CLongBinary binImageData;
	bool		bRet = false;
	if( LoadImageData(sJpgFile, binImageData) ){
		int nDataSize = 0;
		bRet = ImageFromBinary(&binImageData, pImage, nDataSize);
		}
	return bRet;
	}

bool
Serializable::SaveJpgImage(Image* pImage, _String sJpgFileSaveAs, bool bReplace){
	CLongBinary binImageData;
	if( !BinaryFromImage(pImage, &binImageData, Serializable::ImageAsJPG) )
		return false;

	if( bReplace )
		::DeleteFile(sJpgFileSaveAs);

	HANDLE hJpgFile = CreateFile(sJpgFileSaveAs.GetBuffer(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, 0L, NULL);
	if( hJpgFile == INVALID_HANDLE_VALUE )
		return false;

	void*	lpData		= GlobalLock(binImageData.m_hData);
	DWORD	dwWritten	= 0L;
	bool	bRet		= (WriteFile(hJpgFile, lpData, binImageData.m_dwDataLength, &dwWritten, NULL) == TRUE);
	GlobalUnlock(binImageData.m_hData);
	CloseHandle(hJpgFile);
	return false;
	}

bool
Serializable::LoadImageData	(_String sImageFile, CLongBinary& binImage){	
	HANDLE hFile = CreateFile(sImageFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0L, NULL);
	if( hFile == INVALID_HANDLE_VALUE )
		return false;

	int nFileSize = GetFileSize(hFile, NULL);
	if( nFileSize == 0 || !GlobalReAlloc(&binImage, nFileSize) ){
		CloseHandle(hFile);
		return false;
		}

	void*	lpRead		= ::GlobalLock(binImage.m_hData);
	int		nBytesRead	= 0L;
	bool	bRet		= (ReadFile(hFile, lpRead, nFileSize, (DWORD*)&nBytesRead, NULL) == TRUE);
	::GlobalUnlock(binImage.m_hData);
	CloseHandle(hFile);
	return bRet;
	}

#ifdef USE_PNG_LIBRARY
#pragma comment(lib, "PNGLibrary.lib")
extern "C" __declspec(dllimport) HBITMAP LoadPNG(BYTE* lpPNGFileData, UINT nDataSize);
extern "C" __declspec(dllimport) bool EncodeToPNGData(HBITMAP hImageEncode, HGLOBAL &hPngData, UINT& nPngDataSize);
#endif

bool
Serializable::ImageFromBinary(CLongBinary* binImage, Image* pImage, int& nImageDataSize){
	nImageDataSize = 0;
	if( !binImage )
		return false;

	// 0xFF, 0xD8 - JPG image header.
	LPSTREAM lpStream = NULL;
	HRESULT hr = ::CreateStreamOnHGlobal(binImage->m_hData, FALSE, &lpStream);
	if( hr != S_OK )
		return false;
		
	bool bRet = false;

#ifdef USE_PNG_LIBRARY
	// PNG file header structure.
	//89  50  4e  47  0d  0a  1a  0a
	BYTE* lpData = (BYTE*)::GlobalLock(binImage->m_hData);
	if( binImage->m_dwDataLength > 8 &&
		lpData[0] == 0x89 &&
		lpData[1] == 0x50 &&
		lpData[2] == 0x4e &&
		lpData[3] == 0x47 &&
		lpData[4] == 0x0d &&
		lpData[5] == 0x0a &&
		lpData[6] == 0x1a &&
		lpData[7] == 0x0a){
		HBITMAP hPngImage = LoadPNG(lpData, binImage->m_dwDataLength);
		if( hPngImage ){
			pImage->Attach(hPngImage);
			bRet			= true;
			nImageDataSize	= binImage->m_dwDataLength;
			}
		}
	else{
		bRet = (pImage->Load(lpStream) == S_OK);
		if( bRet ){
			ULARGE_INTEGER uCurr = {0, 0};
			LARGE_INTEGER nMove = {0, 0};
			if( lpStream->Seek(nMove, STREAM_SEEK_CUR, &uCurr) == S_OK )
				nImageDataSize = (int)uCurr.LowPart;
			}
		}
	::GlobalUnlock(binImage->m_hData);
#else
	bRet = (pImage->Load(lpStream) == S_OK);
	if( bRet ){
		ULARGE_INTEGER uCurr = {0, 0};
		LARGE_INTEGER nMove = {0, 0};
		if( lpStream->Seek(nMove, STREAM_SEEK_CUR, &uCurr) == S_OK )
			nImageDataSize = (int)uCurr.LowPart;
		}
#endif
	lpStream->Release();
	return bRet;
	}

bool
Serializable::CopyImage(Image* pSrc, Image* pDest){
	if( !pDest || !pSrc )
		return false;

	if( !pDest->IsNull() )
		pDest->Destroy();

	CLongBinary binData;
	if( BinaryFromImage(pSrc, &binData) ){
		int nImageDataSize = 0;
		ImageFromBinary(&binData, pDest, nImageDataSize);
		}
	return true;
	}

bool
Serializable::CopyFontObject(Font* pSrc, Font* pDest){
	if( !pDest || !pSrc )
		return false;

	if( pDest->m_hObject )
		pDest->DeleteObject();
	if( !pSrc->m_hObject )
		return false;

	LOGFONT lf;
	pSrc->GetLogFont(&lf);
	pDest->CreateFontIndirect(&lf);
	return (pDest->m_hObject != NULL);
	}
	*/
__int64	
Serializable::ConvertMoney(double dMoney){
	__int64 nAmount = (__int64)(dMoney * 10000.0f);
	int nFrac	= (int)(nAmount % 100);
	if( nFrac >= 49 )
		nAmount = (nAmount / 100) + 1;
	else
		nAmount = (nAmount / 100) ;
	return nAmount;
	}

__int64	
Serializable::SerializeDateTimeObject(DateTime*pDateTime){
	ASSERT(pDateTime);
	__int64 nDateTimeSerialized = 0;
	if( pDateTime->GetStatus() == DateTime::Status::valid ){
		BYTE* pSerialize = (BYTE*)&nDateTimeSerialized;
		// [1][2][3][4][5][6][7][8] bytes
		// [1-2] = year
		// [3] = month
		// [4] = day
		// [5] = hour
		// [6] = minute
		// [7] = second
		// [8] = reserved

		pSerialize[6]			= (char)pDateTime->GetSecond();
		pSerialize[5]			= (char)pDateTime->GetMinute();
		pSerialize[4]			= (char)pDateTime->GetHour();
		pSerialize[3]			= (char)pDateTime->GetDay();
		pSerialize[2]			= (char)pDateTime->GetMonth();
		*((short*)pSerialize)	= (short)pDateTime->GetYear();
		}
	return nDateTimeSerialized;
	}

void	
Serializable::DeserializeDateTimeObject(__int64 nDateTimeSerialized, DateTime* pDateTime){
	ASSERT(pDateTime);
	BYTE* pSerialized = (BYTE*)&nDateTimeSerialized;
	// [1][2][3][4][5][6][7][8] bytes
	// [1-2] = year
	// [3] = month
	// [4] = day
	// [5] = hour
	// [6] = minute
	// [7] = second
	// [8] = reserved
	if( nDateTimeSerialized )
		pDateTime->SetDateTime((int)*((short*)pSerialized), (int)pSerialized[2], (int)pSerialized[3], (int)pSerialized[4], (int)pSerialized[5], (int)pSerialized[6]);
	else
		pDateTime->SetStatusNull();
	}
/*
bool
Serializable::BinaryFromImage(Image* pImage, CLongBinary* pBinData, ImageAsBinaryType binaryAs ){
	if( !pImage || pImage->IsNull() || !pBinData ) 
		return false;
	
#ifdef USE_PNG_LIBRARY
	if( binaryAs == Serializable::ImageAsPNG ){
		HGLOBAL	hPngData		= NULL;
		UINT	nPngDataSize	= 0L;
		if( EncodeToPNGData(*pImage, hPngData, nPngDataSize) ){
			if( pBinData->m_hData ){
				::GlobalFree(pBinData->m_hData);
				pBinData->m_hData			= NULL;
				pBinData->m_dwDataLength	= 0L;
				}

			pBinData->m_hData			= hPngData;
			pBinData->m_dwDataLength	= nPngDataSize;
//			void* lpData = ::GlobalLock(pBinData->m_hData);
//			Path::SaveAsFile(lpData, nPngDataSize, _T("d:\\2323232323232323.png"));
//			::GlobalUnlock(pBinData->m_hData);
			return true;
			}
		return false;
		}
#endif

	HGLOBAL hImageData = ::GlobalAlloc(GMEM_MOVEABLE, 100);
	LPSTREAM lpStream = NULL;
	if( CreateStreamOnHGlobal(hImageData, FALSE, &lpStream) != S_OK ){
		::GlobalFree(hImageData);
		return false;
		}

	
	GUID arrGUID[] = {
		{0xb96b3caf,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}, // PNG
		{0xb96b3cae,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}, // JPEG
		{0xb96b3cab,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}, // BMP
	};

	int nIndex = 0;
	switch (binaryAs){
		case Serializable::ImageAsPNG:
			nIndex = 0;
			break;
		case Serializable::ImageAsJPG:
			nIndex = 1;
			break;
		case Serializable::ImageAsBMP:
			nIndex = 2;
			break;
		default:
			nIndex = 0;
			break;
		}

	if( binaryAs == ImageAsBinaryType::ImageAsPNG ){
		if( pImage->GetBPP() == 0x20 )
			pImage->SetHasAlphaChannel(true);
		}

	if( pImage->Save(lpStream, arrGUID[nIndex]) != S_OK ){
		lpStream->Release();
		::GlobalFree(hImageData);
		return false;
		}

	int nImageDataSize = 0;
	ULARGE_INTEGER uCurr = {0, 0};
	LARGE_INTEGER nMove = {0, 0};
	if( lpStream->Seek(nMove, STREAM_SEEK_CUR, &uCurr) == S_OK ){
		nImageDataSize = (int)uCurr.LowPart;
		}

	if( pBinData->m_hData )
		::GlobalFree(pBinData->m_hData);
	pBinData->m_hData			= hImageData;
	pBinData->m_dwDataLength	= nImageDataSize;

	lpStream->Release();
//	::GlobalFree(hImageData);
	return true;
	}
	*/
int
Serializable::GetFieldIndex	(const _String sField){
	const AFX_VARINFOMAP* pInfoMap = GetVariableInfoMap();
	if( pInfoMap == NULL )
		return NULL;

	int nLoop = 0, nLoop1 = 0;
	while( pInfoMap->lpVarInfos ){
		VariableInfo* pInfo = (VariableInfo*)pInfoMap->lpVarInfos;
		while( !pInfo[nLoop1].m_sAttributeName.IsEmpty() ){
			if( !sField.CompareNoCase(pInfo[nLoop1].m_sAttributeName) ){
				return (int)((nLoop<<16)|(nLoop1&0xFFFF));
				}
			nLoop1 ++;
			}
		nLoop1	= 0;
		nLoop	++;
		pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		}
	return -1;
	}

bool
Serializable::GetFieldAsString(int nIndex, _String& sValue){
	int nBaseClassIndex = (nIndex >> 16);
	int nVarIndex		= (nIndex & 0xFFFF);
	VariableInfo*	ppinfo[10];
	if( GetVariableInfos(ppinfo) <= nBaseClassIndex )
		return false;

	VariableInfo*	pInfo	= &ppinfo[nBaseClassIndex][nVarIndex];
	if( pInfo->m_bComplexType && pInfo->m_bEnumerable ){
		EnumerableObject<Serializable>* pEnum =  (EnumerableObject<Serializable>*)&((LPBYTE)this)[pInfo->m_nOffset];
		sValue.Format(_T("{%s}"), pEnum->GetClassName().GetBuffer());
		return true;
		}
	sValue = GetVariableAsString(pInfo);
	return true;
	}

bool
Serializable::GetFieldAsString(const _String sFieldName, _String& sValue){
	int nIndex = GetFieldIndex(sFieldName);
	return GetFieldAsString(nIndex, sValue);
	}

bool
Serializable::AddVariableModifyCallBack(void* lpVariablePtr, VariableValueModifyCallback callback){
	const AFX_VARINFOMAP* pInfoMap = GetVariableInfoMap();
	if( pInfoMap == NULL )
		return false;

	int nLoop = 0;
	while( pInfoMap->lpVarInfos ){
		VariableInfo* pInfo = (VariableInfo*)pInfoMap->lpVarInfos;
		while( !pInfo[nLoop].m_sAttributeName.IsEmpty() ){
			if( lpVariablePtr == (void*)&((LPBYTE)this)[pInfo[nLoop].m_nOffset] ){
				if( !m_pVariableModifyCallbacks )
					m_pVariableModifyCallbacks = new AutoSortedArray();
				m_pVariableModifyCallbacks->Add(lpVariablePtr, (void*)callback);
				return true;
				}
			nLoop ++;
			}
		nLoop		= 0;
		pInfoMap	= (pInfoMap->pfnGetBaseVarInfoMap)();
		}
	return false;
	}

bool
Serializable::CallModifyCallback(void* lpVariablePtr, void* lpValuePtrNew){
	if( !lpVariablePtr )				return false;
	if( !m_pVariableModifyCallbacks )	return true; // No methods registered.

	int nIndex = m_pVariableModifyCallbacks->FindFirst(lpVariablePtr);
	if( nIndex == -1 ) return true;

	bool bRet = false;
	VariableValueModifyCallback callback;
	while( nIndex != -1 ){
		callback = (VariableValueModifyCallback)m_pVariableModifyCallbacks->GetData(nIndex);
		if( callback ){
			bRet = (callback)(this, NULL, lpValuePtrNew);
			if( !bRet )
				return false;
			}
		nIndex = m_pVariableModifyCallbacks->FindNext();
		}
	return bRet;
	}

BOOL
Serializable::SerializeAsXML(_String& xmlSource){
	SerializeAsXML_Rec(xmlSource);
	return (xmlSource.GetLength() != 0);
	}

BOOL				
Serializable::SerializeAsXML(_String sXmlFile, BOOL bReplaceExisting){
	Path::BuildFullPath(sXmlFile);

	_String xmlSource;
	if( SerializeAsXML(xmlSource) == FALSE || !xmlSource.GetLength() )
		return FALSE;

	CFile file(sXmlFile, CFile::modeCreate|CFile::modeWrite);
	file.Write(xmlSource, xmlSource.GetLength());
	return TRUE;
	}

BOOL				
Serializable::Serialize(GrowableMemory* pMem, AutoSortedArray* pArrVarOffsets /*= NULL*/){
	Serialize_Rec(pMem, pArrVarOffsets);
	return TRUE;
	}

BOOL				
Serializable::Serialize(_String sOutFile, BOOL bReplaceExisting){
	GrowableMemory* pMem = new GrowableMemory();
	if( !Serialize(pMem) ){
		delete pMem;
		return FALSE;
		}
	
	HANDLE hFile = 0L;
	if( bReplaceExisting ){
		hFile = CreateFile(sOutFile.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0L, NULL);
		if( hFile != INVALID_HANDLE_VALUE ){
			CloseHandle(hFile);
			DeleteFile(sOutFile.GetBuffer());
			}
		}

	hFile = CreateFile(sOutFile.GetBuffer(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, 0L, NULL);
	if( hFile == INVALID_HANDLE_VALUE ){
		delete pMem;
		return FALSE;
		}

	DWORD dwWritten;
	if( !WriteFile(hFile, pMem->GetBufferPtr(), pMem->GetUsedBufferSize(), &dwWritten, NULL) ){
		CloseHandle(hFile);
		delete pMem;
		return FALSE;
		}
		
	CloseHandle(hFile);
	delete pMem;
	return TRUE;
	}

BOOL
Serializable::SerializeWithVariableName(GrowableMemory* pMem){
	SerializeWithVariableName_Rec(pMem);
	return TRUE;
	}

BOOL			
Serializable::Deserialize(_String sFile){
	HANDLE hFile = CreateFile(sFile.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0L, NULL);
	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;

	DWORD dwSize = ::GetFileSize(hFile, NULL);
	if( !dwSize ){
		CloseHandle(hFile);
		return FALSE;
		} 

	BYTE* lpFileData = new BYTE[dwSize];
	GrowableMemory* pMem = new GrowableMemory();
	pMem->SetReadonlyBuffer(lpFileData, dwSize);

	DWORD dwRead = 0;
	if( !ReadFile(hFile, lpFileData, dwSize, &dwRead, NULL) ){
		delete pMem;
		CloseHandle(hFile);
		return FALSE;
		}
	
	BOOL bRet = Deserialize(pMem);
	delete pMem;
	CloseHandle(hFile);
	return FALSE;
	}

BOOL
Serializable::Deserialize(LongBinary* pBinary){
	if( !pBinary ) return FALSE;

	int	nBinarySize	= pBinary->GetBinarySize();
	if( nBinarySize <= 0 )
		return FALSE;

	// Deserialize from binary. {{
	void*			lpBinaryData = pBinary->LockMemory();
	GrowableMemory	mem(0, 0x400, false);
	mem.SetReadonlyBuffer((BYTE*)lpBinaryData, nBinarySize);
	BOOL			bRet = Deserialize(&mem);
	pBinary->UnlockMemory();
	// }}
	return bRet;
	}

BOOL				
Serializable::Deserialize(GrowableMemory* pMem){
	return Deserialize_Rec(pMem);
	}

BOOL			
Serializable::DeserializeAsXML(_String sXmlFile){
	CMarkup xmlReader;
	Path::BuildFullPath(sXmlFile);
	if( !xmlReader.Load(sXmlFile.GetBuffer()) )
		return FALSE;
	return DeserializeAsXML_Rec(&xmlReader);
	}

BOOL			
Serializable::DeserializeAsXML(_String* psXmlSource){
	if( !psXmlSource || !psXmlSource->GetLength() ) 
		return FALSE;

	CMarkup xmlReader;
	if( !xmlReader.SetDoc(psXmlSource->GetBuffer()) )
		return FALSE;

	xmlReader.FindElem(_T("xml"));
	xmlReader.IntoElem();

	return DeserializeAsXML_Rec(&xmlReader);
	}

BOOL
Serializable::DeserializeWithVariableName(GrowableMemory* pMem){
	return DeserializeWithVariableName_Rec(pMem);
	}

BOOL
Serializable::DeserializeWithVariableName(LongBinary* pBinary){
	if( !pBinary ) return FALSE;

	int	nBinarySize	= pBinary->GetBinarySize();
	if( nBinarySize <= 0 )
		return FALSE;

	// Deserialize from binary. {{
	void*			lpBinaryData = pBinary->LockMemory();
	GrowableMemory	mem(0, 0x400, false);
	mem.SetReadonlyBuffer((BYTE*)lpBinaryData, nBinarySize);
	BOOL			bRet = DeserializeWithVariableName(&mem);
	pBinary->UnlockMemory();
	// }}
	return bRet;
	}

void
Serializable::Serialize_Rec(GrowableMemory* pMem, AutoSortedArray* pArrVarOffsets /*= NULL*/){
	// Determine variable infos of this and base class(es). {{
	int						nMainLoop	= 0;
	const AFX_VARINFOMAP*	pInfoMap	= GetVariableInfoMap();
	VariableInfo*			ppinfo[10];
	if( !pInfoMap ) 
		return;
	while( pInfoMap->pfnGetBaseVarInfoMap ){
		ppinfo[nMainLoop] = (VariableInfo*)pInfoMap->lpVarInfos;
		pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		nMainLoop ++;
		}
	nMainLoop --;
	// }}

	while( nMainLoop > -1 ){
		VariableInfo*	info	= ppinfo[nMainLoop];
		int				nLoop	= 1;

		while( info[nLoop].m_nOffset > 0 ){
			if( !info[nLoop].m_bComplexType ){ // Simple type.
				if( info[nLoop].m_bEnumerable ){
					ASSERT(FALSE);
					}
				else{ // Simple type.
					// Update variable offsets.
					if( pArrVarOffsets ){
						int nIndex = pArrVarOffsets->FindValue((void*)(info[nLoop].m_nOffset));
						if( nIndex != -1 )
							pArrVarOffsets->SetData(nIndex, ((void*)(pMem->GetUsedBufferSize() + 1/*count variable type byte*/)));
						}

					MethodWriteVariableInBuffer	method	= Serializable::AddVariableMethod((int)info[nLoop].m_type);
					(method)(this, &info[nLoop], pMem);
					}
				}
			else{ // Complex type.
				int	nOffset = info[nLoop].m_nOffset;
				// Enumerable complex type.
				if( info[nLoop].m_bEnumerable ){ 
					EnumerableObject<Serializable>* varPtr = (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					if( varPtr ){
						int nCount = varPtr->GetCount();
						int nLoop1 = 0;

						// Add info header. {{
#pragma pack(1)
						struct VarHdr{
							char cType;
							int nLength;
						};

#ifdef _DEBUG
						VariableInfo*	pCurrVarInfo	= &info[nLoop];
#endif
						VarHdr hdr	= {(char)VariableType::VT_Array, nCount};
						if( nCount > 0 )
							pMem->AddBytes((BYTE*)&hdr, sizeof(char)+sizeof(int));
						else{
							hdr.cType = SET_DEFAULT_VALUE_TYPE(hdr.cType);
							pMem->AddBytes((BYTE*)&hdr, sizeof(char));
							}
						// }}

						while( nLoop1 < nCount ){
							Serializable* pObj = varPtr->GetAt(nLoop1);
							if( pObj ){
								pObj->Serialize(pMem);
								}
							else{ // Add null element.
								char nullTypeDef	= (char)VariableType::VT_None;
								pMem->AddBytes((BYTE*)&nullTypeDef, sizeof(char));
								}
							nLoop1 ++;
							}
						}
					}
				else{
					void*	varPtr	= (void*)&((LPBYTE)this)[nOffset];
					Serializable* pObject = (Serializable*)varPtr;
					if( pObject ){
						pObject->Serialize(pMem);
						}
					}
				}
			nLoop ++;
			}

		nMainLoop --;
		}
	}

void
Serializable::SerializeAsXML_Rec(_String& xmlSource, const AFX_VARINFOMAP* pInfoMap){
	if( pInfoMap == NULL ){
		pInfoMap = GetVariableInfoMap();
		if( !pInfoMap ) return;
		}

	if( pInfoMap->pfnGetBaseVarInfoMap != NULL ){
		VariableInfo*	info	= (VariableInfo*)pInfoMap->lpVarInfos;
		int				nLoop	= 1;

		_String sTemp;
		sTemp.Format(_T("<%s>\r\n"), (const TCHAR*)info[0].m_sAttributeName.GetBuffer());
		xmlSource += sTemp.GetBuffer();

		// Get base class variable info map.
		const AFX_VARINFOMAP* pBaseInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		// Deserialize base class.
		if( pBaseInfoMap )
			SerializeAsXML_Rec(xmlSource, pBaseInfoMap);

		while( info[nLoop].m_nOffset > 0 ){
			if( !info[nLoop].m_bComplexType ){ // Simple type.
				if( info[nLoop].m_bEnumerable ){
					ASSERT(FALSE);
					}
				else{ // Simple type.
					// Node start.
					sTemp.Format(_T("<%s type=\x22%s\x22>"), (const TCHAR*)info[nLoop].m_sAttributeName, (const TCHAR*)VariableTypeNames[info[nLoop].m_type]);
					xmlSource += sTemp + GetVariableAsString(&info[nLoop]);
					// Node close.
					sTemp.Format(_T("</%s>\r\n"), (const TCHAR*)info[nLoop].m_sAttributeName);
					xmlSource += sTemp;
					}
				}
			else{ // Complex type.

				int	nOffset = info[nLoop].m_nOffset;
				// Enumerable complex type.
				if( info[nLoop].m_bEnumerable ){ 
					EnumerableObject<Serializable>* varPtr = (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					if( varPtr ){
						int nCount = varPtr->GetCount();
						int nLoop1 = 0;

						while( nLoop1 < nCount ){
							Serializable* pObj = varPtr->GetAt(nLoop1);
							if( pObj ){
								pObj->SerializeAsXML_Rec(xmlSource);
								}
							nLoop1 ++;
							}
						}
					}
				else{
					void*	varPtr	= (void*)&((LPBYTE)this)[nOffset];
					Serializable* pObject = (Serializable*)varPtr;
					if( pObject ){
						pObject->SerializeAsXML_Rec(xmlSource);
						}
					}
				}
			nLoop ++;
			}

		sTemp.Format(_T("</%s>\r\n"), (const TCHAR*)info[0].m_sAttributeName.GetBuffer());
		xmlSource += sTemp.GetBuffer();
		}
	}

BOOL
Serializable::Deserialize_Rec(GrowableMemory* pMem){
	// Determine variable infos of this and base class(es). {{
	int						nMainLoop	= 0;
	const AFX_VARINFOMAP*	pInfoMap	= GetVariableInfoMap();
	VariableInfo*			ppinfo[10];
	if( !pInfoMap )
		return FALSE;
	while( pInfoMap->pfnGetBaseVarInfoMap ){
		ppinfo[nMainLoop] = (VariableInfo*)pInfoMap->lpVarInfos;
		pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		nMainLoop ++;
		}
	nMainLoop --;
	// }}

	while( nMainLoop > -1 ){
		VariableInfo*		info		= ppinfo[nMainLoop];
		int					nLoop		= 1;

		while( info[nLoop].m_nOffset > 0 ){
			if( !info[nLoop].m_bComplexType ){ // Simple type.
				ASSERT(!info[nLoop].m_bEnumerable); // Simple cannot be enumerable.
				MethodReadVariableFromBuffer	method	= Serializable::SetVariableMethod((int)info[nLoop].m_type);
				if( !(method)(this, &info[nLoop], pMem) ) 
					return FALSE;
				}
			else{// Complex type.
				int	nOffset = info[nLoop].m_nOffset;
				// Enumerable complex type.
				if( info[nLoop].m_bEnumerable ){
					EnumerableObject<Serializable>*	varPtr	= (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					if( varPtr ){
#pragma pack(1)
						// Read header info. {{
						struct VarHdr{
							char cType;
							int nLength;
						};

						VarHdr hdr	= {(char)VariableType::VT_Array, 0};
						pMem->ReadInt8(&hdr.cType);
						if( REMOVE_DEFAULT_VALUE_TYPE(hdr.cType) != VariableType::VT_Array )
							return FALSE;
						if( IS_DEFAULT_VALUE_TYPE(hdr.cType) )
							hdr.nLength = 0;
						else
							pMem->ReadInt32(&hdr.nLength);
						// }}

#ifdef _DEBUG
						VariableInfo*	pCurrVarInfo	= &info[nLoop];
#endif
						int nLoop = 0, nCt = hdr.nLength;
						char cNullTypeDef = 0;
						while(nLoop < nCt){
							// Check for null object. {{ 
							if( *((char*)pMem->GetCurrentOffsetPtr()) == (char)VariableType::VT_None ){
								pMem->ReadInt8(&cNullTypeDef);
								varPtr->Add(NULL);
								nLoop ++;
								continue;
								}
							// }}

							Serializable* pObject = varPtr->CreateSerializableObject();
							if( pObject ){	
								// Deserialize object.
								if( pObject->Deserialize_Rec(pMem) ){
									// Add object.
									varPtr->Add(pObject);
									}
								else{
									delete pObject;
									return FALSE;
									}
								}
							nLoop ++;
							}
						}
					}
				else{
					void*	varPtr	= (void*)&((LPBYTE)this)[nOffset];
					Serializable* pObject = (Serializable*)varPtr;
					// Use existing object instead of create new one.
					if( pObject ){
						// Deserialize object.
						if( !pObject->Deserialize(pMem) ) 
							return FALSE;
						}
					}
				}
			nLoop ++;
			}
		nMainLoop --;
		}
	return TRUE;
	}

void
Serializable::SerializeWithVariableName_Rec(GrowableMemory* pMem){
	// Determine variable infos of this and base class(es). {{
	int						nMainLoop	= 0;
	const AFX_VARINFOMAP*	pInfoMap	= GetVariableInfoMap();
	VariableInfo*			ppinfo[10];
	if( !pInfoMap ) 
		return;
	while( pInfoMap->pfnGetBaseVarInfoMap ){
		ppinfo[nMainLoop] = (VariableInfo*)pInfoMap->lpVarInfos;
		pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		nMainLoop ++;
		}
	nMainLoop --;
	// }}

	_String					sAttribName;
	DWORD					dwVariableLength	= 0;
	DWORD					dwOffsetVariableCt	= (DWORD)pMem->GetUsedBufferSize();
	int						nVariableCt			= 0;
	pMem->AddBytes((BYTE*)&nVariableCt, sizeof(int)); // Add variable count parameter (After calculating it will be updated).

	while( nMainLoop > -1 ){
		VariableInfo*	info	= ppinfo[nMainLoop];
		int				nLoop	= 1;

		while( info[nLoop].m_nOffset > 0 ){
			sAttribName = info[nLoop].m_sAttributeName;
			ASSERT(!sAttribName.IsEmpty());
			pMem->AddString	(sAttribName);								// Add variable name.
			pMem->AddBytes	((BYTE*)&info[nLoop].m_type, 1);			// Add variable type.
			DWORD dwVariableSizeOffset = (DWORD)pMem->GetUsedBufferSize();
			pMem->AddBytes	((BYTE*)&dwVariableLength, sizeof(DWORD));	// Add variable size type.

			nVariableCt ++;	// Increase variable count

			if( !info[nLoop].m_bComplexType ){ // Simple type.
				if( info[nLoop].m_bEnumerable ){
					ASSERT(FALSE);
					}
				else{ // Simple type.
					MethodWriteVariableInBuffer	method	= Serializable::AddVariableMethod((int)info[nLoop].m_type);
					(method)(this, &info[nLoop], pMem);
					}
				}
			else{ // Complex type.
				int	nOffset = info[nLoop].m_nOffset;
				// Enumerable complex type.
				if( info[nLoop].m_bEnumerable ){
					EnumerableObject<Serializable>* varPtr = (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					if( varPtr ){
						int nCount = varPtr->GetCount();
						int nLoop1 = 0;

						// Add info header. {{
#pragma pack(1)
						struct VarHdr{
							char cType;
							int nLength;
						};

#ifdef _DEBUG
						VariableInfo*	pCurrVarInfo	= &info[nLoop];
#endif
						VarHdr hdr	= {(char)VariableType::VT_Array, nCount};
						if( nCount > 0 )
							pMem->AddBytes((BYTE*)&hdr, sizeof(char)+sizeof(int));
						else{
							hdr.cType = SET_DEFAULT_VALUE_TYPE(hdr.cType);
							pMem->AddBytes((BYTE*)&hdr, sizeof(char));
							}
						// }}

						while( nLoop1 < nCount ){
							Serializable* pObj = varPtr->GetAt(nLoop1);
							if( pObj ){
								pObj->SerializeWithVariableName(pMem);
								}
							else{ // Add null element.
								char nullTypeDef	= (char)VariableType::VT_None;
								pMem->AddBytes((BYTE*)&nullTypeDef, sizeof(char));
								}
							nLoop1 ++;
							}
						}
					}
				else{
					void*	varPtr	= (void*)&((LPBYTE)this)[nOffset];
					Serializable* pObject = (Serializable*)varPtr;
					if( pObject )
						pObject->SerializeWithVariableName(pMem);
					}
				}

			// Update variable size parameter.
			BYTE* pVariableSizePtr = pMem->GetOffsetPtr(dwVariableSizeOffset);
			if( pVariableSizePtr )
				*((DWORD*)pVariableSizePtr) = (pMem->GetUsedBufferSize() - dwVariableSizeOffset - sizeof(DWORD));
			nLoop ++;
			}
		nMainLoop --;
		}

	// Update variable count parameter.
	if( nVariableCt > 0 ){
		BYTE* pVariableCtPtr = pMem->GetOffsetPtr(dwOffsetVariableCt);
		if( pVariableCtPtr )
			*((DWORD*)pVariableCtPtr) = nVariableCt;
		}
	}

BOOL
Serializable::DeserializeWithVariableName_Rec(GrowableMemory* pMem){
	_String sAttribName;
	BYTE	btVariableType	= 0;
	DWORD	dwVariableSize	= 0;
	int		nVariableCt		= 0;
	pMem->ReadInt32(&nVariableCt); // Read variable count.

	while( nVariableCt > 0 ){
		if( !pMem->ReadString(&sAttribName) || !pMem->ReadInt8((char*)&btVariableType) || !pMem->ReadInt32((int*)&dwVariableSize) )
			return FALSE;

		DWORD dwOffset = pMem->GetCurrentOffset();
		VariableInfo* pVarInfo = GetVariableInfo(sAttribName);
		if( pVarInfo && ((BYTE)pVarInfo->m_type) == btVariableType ){
			if( !pVarInfo->m_bComplexType ){ // Simple type.
				ASSERT(!pVarInfo->m_bEnumerable); // Simple cannot be enumerable.
				MethodReadVariableFromBuffer	method	= Serializable::SetVariableMethod((int)pVarInfo->m_type);
				if( !(method)(this, pVarInfo, pMem) ) 
					return FALSE;
				}
			else{// Complex type.
				int	nOffset = pVarInfo->m_nOffset;
				// Enumerable complex type.
				if( pVarInfo->m_bEnumerable ){
					EnumerableObject<Serializable>*	varPtr	= (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					if( varPtr ){
#pragma pack(1)
						// Read header info. {{
						struct VarHdr{
							char cType;
							int nLength;
						};

						VarHdr hdr	= {(char)VariableType::VT_Array, 0};
						pMem->ReadInt8(&hdr.cType);
						if( REMOVE_DEFAULT_VALUE_TYPE(hdr.cType) != VariableType::VT_Array )
							return FALSE;
						if( IS_DEFAULT_VALUE_TYPE(hdr.cType) )
							hdr.nLength = 0;
						else
							pMem->ReadInt32(&hdr.nLength);
						// }}

						int nLoop = 0;
						int nCt = hdr.nLength;
						char cNullTypeDef = 0;
						while(nLoop < nCt){
							// Check for null object. {{ 
							if( *((char*)pMem->GetCurrentOffsetPtr()) == (char)VariableType::VT_None ){
								pMem->ReadInt8(&cNullTypeDef);
								varPtr->Add(NULL);
								nLoop ++;
								continue;
								}
							// }}

							Serializable* pObject = varPtr->CreateSerializableObject();
							if( pObject ){	
								// Deserialize object.
//								if( pObject->Deserialize_Rec(pMem) ){
								if( pObject->DeserializeWithVariableName_Rec(pMem) ){
									// Add object.
									varPtr->Add(pObject);
									}
								else{
									delete pObject;
									return FALSE;
									}
								}
							nLoop ++;
							}
						}
					}
				else{
					void*	varPtr	= (void*)&((LPBYTE)this)[nOffset];
					Serializable* pObject = (Serializable*)varPtr;
					// Use existing object instead of create new one.
					if( pObject ){
						// Deserialize object
						if( !pObject->DeserializeWithVariableName_Rec(pMem) ) 
							return FALSE;
						}
					}
				}
			}

		// Skip variable data bytes. {{
		DWORD dwOffsetLast = pMem->GetCurrentOffset();
		int nBytesSkip = (dwVariableSize - (dwOffsetLast - dwOffset));
		if( nBytesSkip > 0 )
			pMem->ReadSkipBytes(nBytesSkip);
		// }}
		nVariableCt --;
		}
	return TRUE;
	}

BOOL
Serializable::DeserializeAsXML_Rec(CMarkup* xml, const AFX_VARINFOMAP* pInfoMap){
	if( pInfoMap == NULL ){
		pInfoMap = GetVariableInfoMap();
		if( !pInfoMap ) return 1;
		}
	if( pInfoMap->pfnGetBaseVarInfoMap != NULL ){
		VariableInfo*		info		= (VariableInfo*)pInfoMap->lpVarInfos;
		int					nLoop		= 1;
		_String				sElemName	= info[0].m_sAttributeName;
		_String				sData;

		if( !xml->FindElem(sElemName.GetBuffer()) ||
			!xml->IntoElem() )
			return FALSE;

		// Get base class variable info map.
		const AFX_VARINFOMAP* pBaseInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		if( pBaseInfoMap ){
			DeserializeAsXML_Rec(xml, pBaseInfoMap);
			}

		while( info[nLoop].m_nOffset > 0 ){
			if( !info[nLoop].m_bComplexType ){ // Simple type.
				if( xml->FindElem(info[nLoop].m_sAttributeName) ){
					ASSERT(!info[nLoop].m_bEnumerable); // Simple cannot be enumerable.
					sData = xml->GetData();
					SetVariableFromString(&info[nLoop], sData);
					}
				}
			else{// Complex type.
				int	nOffset = info[nLoop].m_nOffset;
				// Enumerable complex type.
				if( info[nLoop].m_bEnumerable ){
					EnumerableObject<Serializable>*	varPtr	= (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					if( varPtr ){
						while(TRUE){ // Endless loop till deserialize fails.
							Serializable* pObject = varPtr->CreateSerializableObject();
							if( pObject ){	
								// Deserialize object.
								if( pObject->DeserializeAsXML_Rec(xml) ){
									// Add object.
									varPtr->Add(pObject);
									}
								else{
									delete pObject;
									break;
									}
								}
							}
						}
					}
				else{
					void*	varPtr	= (void*)&((LPBYTE)this)[nOffset];
					Serializable* pObject = (Serializable*)varPtr;
					// Use existing object instead of create new one.
					if( pObject ){
						// Deserialize object.
						pObject->DeserializeAsXML_Rec(xml);
						}
					}
				}
			nLoop ++;
			}
		xml->OutOfElem();
		}
	return TRUE;
	}

BOOL
Serializable::SetVariableFromString(VariableInfo* pVarInfo, _String& sValue){
	MethodSetVariableFromString	method	= Serializable::SetVariableFromString((int)pVarInfo->m_type);
	if( method )
		return (method)(this, pVarInfo, sValue);
	return FALSE;
	}

bool
Serializable::SetVariable(VariableInfo* pVarInfo, void* lpVariablePtr){
	if( !pVarInfo )
		return false;

	MethodWriteVariableInVar	method	= Serializable::WriteVariableIntoVarMethod(pVarInfo->m_type);
	if( !method )
		return false;

	VariableInfo	infoSrc			= {_T(""), _T(""), pVarInfo->m_type, 0, pVarInfo->m_bNullable, pVarInfo->m_bComplexType, pVarInfo->m_bEnumerable};
	void*			lpValuePtrOld	= (void*)&((LPBYTE)this)[pVarInfo->m_nOffset];
	return (method)((Serializable*)lpVariablePtr, &infoSrc, (Serializable*)lpValuePtrOld);
	}


bool
Serializable::ZeroInit(){
	return ZeroInit_Rec();
	}

bool
Serializable::ZeroInit_Rec(){
	const AFX_VARINFOMAP*	pInfoMap	= GetVariableInfoMap();
	if( !pInfoMap ) return false;

	while( pInfoMap->lpVarInfos ){
		VariableInfo*		info		= (VariableInfo*)pInfoMap->lpVarInfos;
		int					nLoop		= 1;

		while( info[nLoop].m_nOffset > 0 ){
			if( !info[nLoop].m_bComplexType ){ // Simple type.
				ASSERT(!info[nLoop].m_bEnumerable); // Simple cannot be enumerable.

				MethodZeroInitVariable_Type	method	= Serializable::GetZeroInitMethod((int)info[nLoop].m_type);
				if( !(method)(this, &info[nLoop]) )
					return false;
				}
			else{// Complex type.
				int	nOffset = info[nLoop].m_nOffset;
				// Enumerable complex type.
				if( info[nLoop].m_bEnumerable ){
					EnumerableObject<Serializable>*	varPtr	= (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					if( varPtr ){
						varPtr->RemoveAll(true);
						/*
						int nLoop	= 0;
						int nCt		= varPtr->GetCount();
						while(nLoop < nCt){
							Serializable* pObject = varPtr->GetAt(nLoop);
							if( pObject ){	
								// Zero init object.
								if( !pObject->ZeroInit_Rec() )
									return false;
								}
							nLoop ++;
							}*/
						}
					}
				else{
					void*			varPtr	= (void*)&((LPBYTE)this)[nOffset];
					Serializable*	pObject = (Serializable*)varPtr;
					// Use existing object instead of create new one.
					if( pObject ){
						// Deserialize object.
						if( !pObject->ZeroInit_Rec() )
							return false;
						}
					}
				}
			nLoop ++;
			}
		pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap());
		}
	return true;
	}

bool
Serializable::Copy(Serializable* pInto){
	if( !pInto )
		return false;
	if( this != pInto )
		Copy_Rec(pInto);
	return true;
	}

void
Serializable::Copy_Rec(Serializable* pInto){
	const AFX_VARINFOMAP*	pInfoMap	= GetVariableInfoMap();
	if( !pInfoMap ) return;

	while( pInfoMap->lpVarInfos ){
		VariableInfo*	info	= (VariableInfo*)pInfoMap->lpVarInfos;
		int				nLoop	= 1;

		while( info[nLoop].m_nOffset > 0 ){
			if( !info[nLoop].m_bComplexType ){ // Simple type.
				if( info[nLoop].m_bEnumerable ){
					ASSERT(FALSE);
					}
				else{ // Simple type.
					MethodWriteVariableInVar	method	= Serializable::WriteVariableIntoVarMethod((int)info[nLoop].m_type);
					(method)(this, &info[nLoop], pInto);
					}
				}
			else{ // Complex type.
				int	nOffset = info[nLoop].m_nOffset;
				// Enumerable complex type.
				if( info[nLoop].m_bEnumerable ){ 
					EnumerableObject<Serializable>* varPtr		= (EnumerableObject<Serializable>*)&((LPBYTE)this)[nOffset];
					EnumerableObject<Serializable>* varPtrIn	= (EnumerableObject<Serializable>*)&((LPBYTE)pInto)[nOffset];
					if( varPtr && varPtrIn ){
						int nCount = varPtr->GetCount();
						int nLoop1 = 0;
						varPtrIn->SetCount(nCount, true);

						// Empty list.
						//varPtrIn->RemoveAll();
#ifdef _DEBUG
						VariableInfo*	pCurrVarInfo	= &info[nLoop];
#endif
						while( nLoop1 < nCount ){
							Serializable* pObj = varPtr->GetAt(nLoop1);
							if( pObj ){
								// Reuse created object.
								Serializable* pVarIn = varPtrIn->GetAt(nLoop1);
								if( !pVarIn ){
									pVarIn = varPtrIn->CreateSerializableObject();
									varPtrIn->SetAt(nLoop1, pVarIn, true);
									}
								pObj->Copy_Rec(pVarIn);
								//varPtrIn->Add(pVarIn);
								}	
							else{ 
								// Set null element.
								varPtrIn->SetAt(nLoop1, NULL, true);
								// Add null element.
								//varPtrIn->Add(NULL);
								}
							nLoop1 ++;
							}
						}
					}
				else{
					void*			varPtr		= (void*)&((LPBYTE)this)[nOffset];
					void*			varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
					Serializable*	pObject		= (Serializable*)varPtr;
					Serializable*	pObjectIn	= (Serializable*)varPtrIn;
					if( pObject ){
						pObject->Copy_Rec(pObjectIn);
						}
					}
				}
			nLoop ++;
			}
		pInfoMap = (pInfoMap->pfnGetBaseVarInfoMap)();
		}
	}

_String	
Serializable::GetVariableAsString(VariableInfo* pInfo){
	MethodGetVariableAsString	method	= Serializable::GetVariableMethod((int)pInfo->m_type);
	_String sValue;
	if( (method)(this, pInfo, sValue) ){
		return sValue;
		}
	return _T("");
	}
/*
BOOL
Serializable::SetVariableFromString(VariableInfo* pInfo, _String& sValue){
	VariableType		type	= pInfo->m_type;
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)this)[nOffset];

	switch(type){
		case  VariableType::VT_Bool:
			{
			*((bool*)varPtr) = (sValue == _T("True") || sValue == _T("1"));
			break;
			}
		case  VariableType::VT_Date:
			{
			COleDateTime dt;
			if( dt.ParseDateTime(sValue, VAR_DATEVALUEONLY) )
				*((COleDateTime*)varPtr) = dt;
			else{
				*((COleDateTime*)varPtr) = COleDateTime::GetCurrentTime();
				}
			return TRUE;
			}
		case  VariableType::VT_DateTime:
			{
			COleDateTime dt;
			if( dt.ParseDateTime(sValue) )
				*((COleDateTime*)varPtr) = dt;
			else{
				*((COleDateTime*)varPtr) = COleDateTime::GetCurrentTime();
				}
			return TRUE;
			}
		case  VariableType::VT_Time:
			{
			COleDateTime dt;
			if( dt.ParseDateTime(sValue, VAR_TIMEVALUEONLY) )
				*((COleDateTime*)varPtr) = dt;
			else{
				*((COleDateTime*)varPtr) = COleDateTime::GetCurrentTime();
				}
			return TRUE;
			}
		case  VariableType::VT_Double:
			{
			*((double*)varPtr) = _tstof(sValue.GetBuffer());
			return TRUE;
			}
		case  VariableType::VT_Float:
			{
			*((float*)varPtr) = _tstof(sValue.GetBuffer());
			return TRUE;
			}
		case  VariableType::VT_String:
			{
			*((_String*)varPtr) = sValue;
			return TRUE;
			}
		case  VariableType::VT_Int8:
			{
			*((char*)varPtr) = (char)_tstoi(sValue.GetBuffer());
			break;
			}
		case  VariableType::VT_Int16:
			{
			*((short*)varPtr) = (short)_tstoi(sValue.GetBuffer());
			return TRUE;
			}
		case  VariableType::VT_Int32:
			{
			*((int*)varPtr) = _tstoi(sValue.GetBuffer());
			return TRUE;
			}
		case  VariableType::VT_Int64:
			{
			*((__int64*)varPtr) = _tstoi64(sValue.GetBuffer());
			return TRUE;
			}
		}
	return FALSE;
	}*/

BOOL 
Serializable::GetVariableAsString_Bool(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Bool ){
		sValue = (*((bool*)varPtr) == true) ? _T("Yes") : _T("No");
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Int8(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int8 ){
		sValue.Format(_T("%d"), *((BYTE*)varPtr));
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Int16(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int16 ){
		sValue.Format(_T("%d"), *((short*)varPtr));
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Int32(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int32 ){
		sValue.Format(_T("%d"), *((int*)varPtr));
		return TRUE;
		}
	return FALSE;
	}
BOOL 
Serializable::GetVariableAsString_Int64(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int64 ){
		sValue.Format(_T("%d"), *((__int64*)varPtr));
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_String(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_String ){
		sValue = *((_String*)varPtr);
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_DateTime(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_DateTime ){
		COleDateTime* pdtVal = ((COleDateTime*)varPtr);
		sValue.Format(_T("%02d/%02d/%04d %02d:%02d:%02d"), 
				pdtVal->GetMonth(), pdtVal->GetDay(), pdtVal->GetYear(), 
				pdtVal->GetHour(), pdtVal->GetMinute(), pdtVal->GetSecond());
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Date(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Date ){
		COleDateTime* pdtVal = ((COleDateTime*)varPtr);
		sValue.Format(_T("%02d/%02d/%04d"), pdtVal->GetMonth(), pdtVal->GetDay(), pdtVal->GetYear());
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Time(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Bool ){
		COleDateTime* pdtVal = ((COleDateTime*)varPtr);
		sValue.Format(_T("%02d:%02d:%02d"), 
				pdtVal->GetHour(), pdtVal->GetMinute(), pdtVal->GetSecond());
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Float(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Float ){
		sValue.Format(_T("%f"), *((float*)varPtr));
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Double(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Double ){
		sValue.Format(_T("%f"), *((float*)varPtr));
		return TRUE;
		}
	return FALSE;
	}

BOOL 
Serializable::GetVariableAsString_Binary(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	LongBinary*			varPtr	= (LongBinary*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Binary ){
		if( varPtr->GetBinarySize() )
			sValue.Format(_T("Binary:%d bytes"), varPtr->GetBinarySize());
		else
			sValue = _T("Binary:0 bytes");
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Rect(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Rect*				varPtr	= (_Rect*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Rect ){
		sValue.Format(_T("%d,%d,%d,%d"), varPtr->left, varPtr->top, varPtr->right, varPtr->bottom);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Point(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Point*				varPtr	= (_Point*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Point ){
		sValue.Format(_T("%d,%d"), varPtr->x, varPtr->y);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Size(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Size*				varPtr	= (_Size*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Size ){
		sValue.Format(_T("%d,%d"), varPtr->cx, varPtr->cy);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Image(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	Image*				varPtr	= (Image*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Image ){
		if( varPtr->IsNull() )
			sValue = _T("Image cx=0 cy=0");
		else
			sValue.Format(_T("Image cx=%d cy=%d"), varPtr->GetWidth(), varPtr->GetHeight());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Font(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
//	Font*				varPtr	= (Font*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Font ){
		sValue = _T("Font object");
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Color32(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	COLORREF*			varPtr	= (COLORREF*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Size ){
		BYTE red	= (BYTE)(*varPtr)&0xFF;
		BYTE green	= (BYTE)(((*varPtr)&0xFF00) >> 8);
		BYTE blue	= (BYTE)(((*varPtr)&0xFF0000) >> 16);
		sValue.Format(_T("#%02X%02X%02X"), red, green, blue);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Enum(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	StringToIntArray*	varPtr	= (StringToIntArray*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Enumerable ){
		sValue.Format(_T("%d"), varPtr->GetCurrentValue());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_VoidPtr(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	void**	varPtr	= (void**)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_VoidPtr ){
		sValue.Format(_T("%08X"), (__int64)*varPtr);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::GetVariableAsString_Chips(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	INT_CHIPS*			varPtr	= (INT_CHIPS*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Chips ){
		sValue.Format(_T("%d"), *varPtr);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Bool(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	bool*				varPtr	= (bool*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Bool ){
		if( sValue == _T("Yes") )
			*varPtr = true;
		else
		if( sValue == _T("No") )
			*varPtr = false;
		else
			*varPtr = (bool)(_ttoi(sValue.GetBuffer()) != 0);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Int8(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	char*				varPtr	= (char*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int8 ){
		*varPtr = (char)_ttoi(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Int16(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	short*				varPtr	= (short*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int16 ){
		*varPtr = (short)_ttoi(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Int32(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	int*				varPtr	= (int*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Int32 ){
		*varPtr = _ttoi(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Int64(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	__int64*			varPtr	= (__int64*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Int64 ){
		*varPtr = _ttoi64(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_String(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	_String*	varPtr	= (_String*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_String ){
		*varPtr = sValue;
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_DateTime(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int				nOffset = pInfo->m_nOffset;
	COleDateTime*	varPtr	= (COleDateTime*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_DateTime ){
		varPtr->ParseDateTime(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Date(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int				nOffset = pInfo->m_nOffset;
	COleDateTime*	varPtr	= (COleDateTime*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Date ){
		varPtr->ParseDateTime(sValue.GetBuffer(), VAR_DATEVALUEONLY);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Time(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int				nOffset = pInfo->m_nOffset;
	COleDateTime*	varPtr	= (COleDateTime*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Time ){
		varPtr->ParseDateTime(sValue.GetBuffer(), VAR_TIMEVALUEONLY);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Float(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int				nOffset = pInfo->m_nOffset;
	float*			varPtr	= (float*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Float ){
		*varPtr = (float)_ttof(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Double(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int				nOffset = pInfo->m_nOffset;
	double*			varPtr	= (double*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Double ){
		*varPtr = _ttof(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Binary(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int				nOffset = pInfo->m_nOffset;
	LongBinary*	varPtr	= (LongBinary*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Binary ){
		//return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Rect(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	_Rect*	varPtr	= (_Rect*)&((LPBYTE)pThis)[nOffset];
	_Rect	rcFormated(0, 0, 0, 0);
	if( pInfo->m_type == VariableType::VT_Rect ){
		// left
		rcFormated.left = _ttoi(&((TCHAR*)sValue.GetBuffer())[0]);

		int nIndex = sValue.Find(',', 0); // top
		if( nIndex == -1 ) return FALSE;
		rcFormated.top = _ttoi(&((TCHAR*)sValue.GetBuffer())[nIndex + 1]);

		nIndex = sValue.Find(',', nIndex + 1); // right
		if( nIndex == -1 ) return FALSE;
		rcFormated.right = _ttoi(&((TCHAR*)sValue.GetBuffer())[nIndex + 1]);

		nIndex = sValue.Find(',', nIndex + 1); // bottom
		if( nIndex == -1 ) return FALSE;
		rcFormated.bottom = _ttoi(&((TCHAR*)sValue.GetBuffer())[nIndex + 1]);
		*varPtr = rcFormated;
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Point(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	_Point*	varPtr	= (_Point*)&((LPBYTE)pThis)[nOffset];
	_Point	ptFormated(0, 0);
	if( pInfo->m_type == VariableType::VT_Point ){
		int nIndex = sValue.Find(',');			
		ptFormated.x = _ttoi(&((TCHAR*)sValue.GetBuffer())[0]);// x
		if( nIndex == -1 ) return FALSE;
		ptFormated.y = _ttoi(&((TCHAR*)sValue.GetBuffer())[nIndex + 1]);// y
		*varPtr = ptFormated;
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Size(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	_Point*	varPtr	= (_Point*)&((LPBYTE)pThis)[nOffset];
	_Size	szFormated(0, 0);
	if( pInfo->m_type == VariableType::VT_Size ){
		szFormated.cx = _ttoi(&((TCHAR*)sValue.GetBuffer())[0]);
		int nIndex = sValue.Find(',');			// cx
		if( nIndex == -1 ) return FALSE;
		szFormated.cy = _ttoi(&((TCHAR*)sValue.GetBuffer())[nIndex + 1]); // cy
		varPtr->SetPoint(szFormated.cx, szFormated.cy);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Image(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	Image*	varPtr	= (Image*)&((LPBYTE)pThis)[nOffset];
	
	if( pInfo->m_type == VariableType::VT_Image ){
		varPtr->Destroy();
		if( varPtr->LoadImage32bpp(sValue) == S_OK ){
			}
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Font(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	/*
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	StringToIntArray*	varPtr	= (StringToIntArray*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Enumerable ){
		sValue.Format(_T("%d"), varPtr->GetCurrentValue());
		return TRUE;
		}*/
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Color32(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	COLORREF*	varPtr	= (COLORREF*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Color32 ){
		*varPtr = (COLORREF)_ttoi(sValue.GetBuffer());
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Enum(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	StringToIntArray*	varPtr	= (StringToIntArray*)&((LPBYTE)pThis)[nOffset];
	int					nValue	= _ttoi(sValue.GetBuffer());

	if( pInfo->m_type == VariableType::VT_Enumerable ){
		varPtr->SetCurrentValue(nValue);
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_VoidPtr(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void**				varPtr	= (void**)&((LPBYTE)pThis)[nOffset];
	__int64				nValue	= (__int64)_ttoi64(sValue.GetBuffer());

	if( pInfo->m_type == VariableType::VT_VoidPtr ){
		*varPtr = (void*)nValue;
		return TRUE;
		}
	return FALSE;
	}

BOOL
Serializable::SetVariableFromString_Chips(Serializable* pThis, VariableInfo* pInfo, _String& sValue){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	INT_CHIPS*	varPtr	= (INT_CHIPS*)&((LPBYTE)pThis)[nOffset];
	__int64		nValue	= _ttoi64(sValue.GetBuffer());

	if( pInfo->m_type == VariableType::VT_Chips ){
		*varPtr = (INT_CHIPS)nValue;
		return TRUE;
		}
	return FALSE;
	}

bool
Serializable::WriteVariableInBuffer_Bool(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	bool*				varPtr	= (bool*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Bool ){
		cType = (char)pInfo->m_type;
		if( *varPtr == 0 ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));	// Type.
		pMem->AddBytes((BYTE*)varPtr, sizeof(char));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Int8(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	char*				varPtr	= (char*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Int8 ){
		cType = (char)pInfo->m_type;
		if( *varPtr == 0 ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));	// Type.
		pMem->AddBytes((BYTE*)varPtr, sizeof(char));	// Variable
		return true;
		}
	return false;
	}
bool
Serializable::WriteVariableInBuffer_Int16(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	short*				varPtr	= (short*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Int16 ){
		cType = (char)pInfo->m_type;
		if( *varPtr == 0 ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));	// Type.
		pMem->AddBytes((BYTE*)varPtr, sizeof(short));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Int32(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	int*				varPtr	= (int*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Int32 ){
		cType = (char)pInfo->m_type;
		if( *varPtr == 0 ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}

		pMem->AddBytes((BYTE*)&cType, sizeof(char));	// Type.
		pMem->AddBytes((BYTE*)varPtr, sizeof(int));		// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Int64(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	__int64*			varPtr	= (__int64*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Int64 ){
		cType = (char)pInfo->m_type;
		if( *varPtr == 0 ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}

		pMem->AddBytes((BYTE*)&cType, sizeof(char));	// Type.
		pMem->AddBytes((BYTE*)varPtr, sizeof(__int64));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_String(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_String*			varPtr	= (_String*)&((LPBYTE)pThis)[nOffset];
	char				cType;
	short				shLen;

	if( pInfo->m_type == VariableType::VT_String ){
		cType = (char)pInfo->m_type;
		if( varPtr->IsEmpty() ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}

		size_t len		= varPtr->GetLength();
		cType			= (char)pInfo->m_type;
		shLen			= (short)((len)*sizeof(TCHAR));
		pMem->AddBytes((BYTE*)&cType, sizeof(char));			// Type.
		pMem->AddBytes((BYTE*)&shLen, sizeof(short));			// length.
		pMem->AddBytes((BYTE*)varPtr->GetBuffer(), (int)shLen);	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_DateTime(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	DateTime*			varPtr	= (DateTime*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_DateTime ){
		cType = (char)pInfo->m_type;
		DateTime::Status status = varPtr->GetStatus();
		if( status != DateTime::Status::valid || status == DateTime::Status::null ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));		    // Type.
		__int64 nVal = SerializeDateTimeObject(varPtr);
		pMem->AddBytes((BYTE*)&nVal, sizeof(__int64));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Date(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	DateTime*			varPtr	= (DateTime*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Date ){
		cType = (char)pInfo->m_type;
		DateTime::Status status = varPtr->GetStatus();
		if( status != DateTime::Status::valid || status == DateTime::Status::null ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));		    // Type.
		__int64 nVal = SerializeDateTimeObject(varPtr);
		pMem->AddBytes((BYTE*)&nVal, sizeof(__int64));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Time(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	DateTime*			varPtr	= (DateTime*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Time ){
		cType = (char)pInfo->m_type;
		DateTime::Status status = varPtr->GetStatus();
		if( status != DateTime::Status::valid || status == DateTime::Status::null ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));	// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));		// Type.
		__int64 nVal = SerializeDateTimeObject(varPtr);
		pMem->AddBytes((BYTE*)&nVal, sizeof(__int64));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Float(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	float*				varPtr	= (float*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Float ){
		cType = (char)pInfo->m_type;
		if( *varPtr == 0.0f ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));				// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));	// Type.
		pMem->AddBytes((BYTE*)varPtr, sizeof(float));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Double(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	double*				varPtr	= (double*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Double ){
		cType = (char)pInfo->m_type;
		if( *varPtr == 0.0 ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));				// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType, sizeof(char));	// Type.
		pMem->AddBytes((BYTE*)varPtr, sizeof(double));	// Variable
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Binary(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	LongBinary*			varPtr		= (LongBinary*)&((LPBYTE)pThis)[nOffset];
	char				cType;
	int					nLen;

	if( pInfo->m_type == VariableType::VT_Binary ){
		cType	= pInfo->m_type;
		nLen	= varPtr->GetBinarySize();
		if( nLen <= 0 ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));				// Type.
			return true;
			}

		pMem->AddBytes((BYTE*)&cType,	sizeof(char));					// Type.
		pMem->AddBytes((BYTE*)&nLen,	sizeof(int));					// Length.
		if( nLen > 0 ){
			void* lpBinData = varPtr->LockMemory();			// Lock global memory
			pMem->AddBytes((BYTE*)lpBinData, (int)varPtr->GetBinarySize());	// Variable
			varPtr->UnlockMemory();							// Unlock global memory.
			}
		return true;
		}
	return false;
	}


bool
Serializable::WriteVariableInBuffercRect(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Rect*				varPtr	= (_Rect*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Rect ){
		cType = (char)pInfo->m_type;
		if( varPtr->IsRectNull() ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));				// Type.
			return true;
			}
		pMem->AddBytes((BYTE*)&cType,			sizeof(char));			// Type.
		pMem->AddBytes((BYTE*)&varPtr->left,	sizeof(tagRECT));		// tagRECT structure
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Point(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Point*				varPtr	= (_Point*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Point ){
		cType = (char)pInfo->m_type;
		if( *varPtr == _Point(0, 0) ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));				// Type.
			return true;
			}

		pMem->AddBytes((BYTE*)&cType,		sizeof(char));			// Type.
		pMem->AddBytes((BYTE*)&varPtr->x,	sizeof(tagPOINT));		// tagPOINT structure
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Size(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Size*				varPtr	= (_Size*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Size ){
		cType = (char)pInfo->m_type;
		if( *varPtr == _Size(0, 0) ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));				// Type.
			return true;
			}

		pMem->AddBytes((BYTE*)&cType,		sizeof(char));			// Type.
		pMem->AddBytes((BYTE*)&varPtr->cx,	sizeof(tagSIZE));		// tagSIZE structure
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Image(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	Image*				varPtr	= (Image*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Image ){
		cType = (char)pInfo->m_type;
		if( varPtr->IsNull() ){
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));				// Type.
			return true;
			}

		LongBinary bin;
		if( varPtr->EncodeImageData(&bin, Image::ImageAsPNG) ){
			DWORD nLen = bin.GetBinarySize();
			if( nLen > 0 ){
				pMem->AddBytes((BYTE*)&cType,	sizeof(char));			// Type.
				pMem->AddBytes((BYTE*)&nLen,	sizeof(int));			// Length.
				void* lpBinData = bin.LockMemory();
				pMem->AddBytes((BYTE*)lpBinData, nLen);					// Binary data.
				bin.UnlockMemory();
				}
			else{// Default value type.
				cType = SET_DEFAULT_VALUE_TYPE(cType);
				pMem->AddBytes((BYTE*)&cType,	sizeof(char));			// Type.
				}
			}
		else{// Default value type.
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));			// Type.
			}
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Font(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	Font*				varPtr	= (Font*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Font ){
		cType = (char)pInfo->m_type;
		LOGFONT lf;
		if( !varPtr->IsNull() && varPtr->GetLogFont(&lf) ){
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));			// Type.
			pMem->AddBytes((BYTE*)&lf,		sizeof(LOGFONT));		// LOGFONT structure
			}
		else{ // Default value type.
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));			// Type.
			}
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Color32(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	COLORREF*			varPtr	= (COLORREF*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Color32 ){
		cType = (char)pInfo->m_type;
		if( *varPtr != 0 ){
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));			// Type.
			pMem->AddBytes((BYTE*)varPtr,	sizeof(COLORREF));		// Color32
			}
		else{ // Default value type.
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			}
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Enum(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	StringToIntArray*	varPtr	= (StringToIntArray*)&((LPBYTE)pThis)[nOffset];
	char				cType;

	if( pInfo->m_type == VariableType::VT_Enumerable ){
		cType		= (char)pInfo->m_type;
		int nIndex	= varPtr->GetCurrentIndex();
		if( nIndex != 0 ){
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			pMem->AddBytes((BYTE*)&nIndex,	sizeof(short));		// Current index of enumarable.
			}
		else{ // Default value type.
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			}
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_VoidPtr(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	void**		varPtr	= (void**)&((LPBYTE)pThis)[nOffset];
	char		cType;

	if( pInfo->m_type == VariableType::VT_VoidPtr ){
		cType		= (char)pInfo->m_type;
		if( *varPtr != NULL ){
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			pMem->AddBytes((BYTE*)varPtr,	sizeof(void*));		// Value
			}
		else{ // Default value type.
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			}
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInBuffer_Chips(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	INT_CHIPS*	varPtr	= (INT_CHIPS*)&((LPBYTE)pThis)[nOffset];
	char		cType;

	if( pInfo->m_type == VariableType::VT_Chips ){
		cType		= (char)pInfo->m_type;
		if( *varPtr != NULL ){
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			pMem->AddBytes((BYTE*)varPtr,	5/*bytes*/);	// Value.
			}
		else{ // Default value type.
			cType = SET_DEFAULT_VALUE_TYPE(cType);
			pMem->AddBytes((BYTE*)&cType,	sizeof(char));		// Type.
			}
		return true;
		}
	return false;
	}

bool 
Serializable::ReadVariableFromBuffer_Bool(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Bool ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Bool )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((bool*)varPtr) = false;
			return true;
			}
		return pMem->ReadInt8((char*)varPtr);
		}
	return false;
	}
bool 
Serializable::ReadVariableFromBuffer_Int8(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int8 ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Int8 )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((char*)varPtr) = 0;
			return true;
			}
		return pMem->ReadInt8((char*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Int16(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int16 ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Int16 )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((short*)varPtr) = 0;
			return true;
			}
		return pMem->ReadInt16((short*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Int32(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int32 ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Int32 )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((int*)varPtr) = 0;
			return true;
			}
		return pMem->ReadInt32((int*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Int64(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int64 ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Int64 )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((__int64*)varPtr) = 0;
			return true;
			}
		return pMem->ReadInt64((__int64*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_String(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_String ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_String )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			((_String*)varPtr)->Empty();
			return true;
			}
		return pMem->ReadString((_String*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_DateTime(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_DateTime ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_DateTime )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((DateTime*)varPtr) = time(NULL);
			return true;
			}

		DateTime* pOleDTime = (DateTime*)varPtr;
		__int64 nVal;
		if( !pMem->ReadInt64(&nVal) )
			return false;
		DeserializeDateTimeObject(nVal, pOleDTime);
		return true;
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Date(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Date ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Date )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((DateTime*)varPtr) = time(NULL);
			return true;
			}

		DateTime* pDTime = (DateTime*)varPtr;
		__int64 nVal;
		if( !pMem->ReadInt64(&nVal) )
			return false;
		DeserializeDateTimeObject(nVal, pDTime);
		return true;
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Time(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Time ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Time )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((DateTime*)varPtr) = time(NULL);
			return true;
			}

		DateTime* pOleDTime = (DateTime*)varPtr;
		__int64 nVal;
		if( !pMem->ReadInt64(&nVal) )
			return false;
		DeserializeDateTimeObject(nVal, pOleDTime);
		return true;
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Float(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Float ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Float )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((float*)varPtr) = 0.0f;
			return true;
			}
		return pMem->ReadFloat((float*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Double(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Double ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Double )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*((double*)varPtr) = 0.0;
			return true;
			}
		return pMem->ReadDouble((double*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Binary(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	LongBinary*			varPtr	= (LongBinary*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Binary ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Binary )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			varPtr->SetBinarySize(0);
			return true;
			}
		return pMem->ReadBinary(varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffercRect(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Rect*				varPtr	= (_Rect*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Rect ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Rect )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			varPtr->SetRect(0, 0, 0, 0);
			return true;
			}
		return pMem->ReadtagRect(varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Point(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Point*				varPtr	= (_Point*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Point ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Point )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			varPtr->SetPoint(0, 0);
			return true;
			}
		return pMem->ReadtagPoint(varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Size(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Size*				varPtr	= (_Size*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Size ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Size )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			varPtr->SetSize(0, 0);
			return true;
			}
		return pMem->ReadtagSize(varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Image(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	Image*				varPtr	= (Image*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Image ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Image )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			varPtr->Destroy();
			return true;
			}

		LongBinary bin;
		if( pMem->ReadBinary(&bin) ){
			varPtr->Destroy();
			if( varPtr->LoadFromBinary(&bin) )
				return true;
			}
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Font(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	Font*				varPtr	= (Font*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Font ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Font )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			varPtr->DeleteObject();
			return true;
			}

		LOGFONT lf;
		if( pMem->ReadLogFont(&lf) ){
			varPtr->DeleteObject();
			varPtr->CreateFontIndirect(&lf);
			return true;
			}
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Color32(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	COLORREF*			varPtr	= (COLORREF*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Color32 ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Color32 )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*varPtr = 0;
			return true;
			}
		return pMem->ReadInt32((int*)varPtr);
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Enum(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	StringToIntArray*	varPtr	= (StringToIntArray*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Enumerable ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Enumerable )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			varPtr->SetCurrentIndex(0);
			return true;
			}
		int nIndex = 0;
		if( pMem->ReadInt16((short*)&nIndex) ){
			varPtr->SetCurrentIndex(nIndex);
			return true;
			}
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_VoidPtr(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	void**	varPtr	= (void**)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_VoidPtr ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_VoidPtr )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*varPtr = NULL;
			return true;
			}
		void* lpVal = NULL;
		if( pMem->ReadBytes(&lpVal, sizeof(void*)) ){
			*varPtr = lpVal;
			return true;
			}
		}
	return false;
	}

bool
Serializable::ReadVariableFromBuffer_Chips(Serializable* pThis, VariableInfo* pInfo, GrowableMemory* pMem){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	INT_CHIPS*	varPtr	= (INT_CHIPS*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Chips ){
		char cType = 0;
		if( !pMem->ReadInt8(&cType) || REMOVE_DEFAULT_VALUE_TYPE(cType) != (char)VariableType::VT_Chips )
			return false;
		if( IS_DEFAULT_VALUE_TYPE(cType) ){
			*varPtr = 0;
			return true;
			}
		if( pMem->ReadChips(varPtr) )
			return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Bool(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Bool ){
		*((bool*)varPtrIn) = *((bool*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Int8(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Int8 ){
		*((char*)varPtrIn) = *((char*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Int16(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Int16 ){
		*((short*)varPtrIn) = *((short*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Int32(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Int32 ){
		*((int*)varPtrIn) = *((int*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Int64(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Int64 ){
		*((__int64*)varPtrIn) = *((__int64*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_String(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_String ){
		*((_String*)varPtrIn) = *((_String*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_DateTime(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	DateTime*			varPtr		= (DateTime*)&((LPBYTE)pThis)[nOffset];
	DateTime*			varPtrIn	= (DateTime*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_DateTime ){
		*varPtrIn = *varPtr;
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Date(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Date ){
		*((double*)varPtrIn) = *((double*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Time(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Time ){
		*((double*)varPtrIn) = *((double*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Float(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Float ){
		*((float*)varPtrIn) = *((float*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Double(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	void*				varPtr		= (void*)&((LPBYTE)pThis)[nOffset];
	void*				varPtrIn	= (void*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Double ){
		*((double*)varPtrIn) = *((double*)varPtr); // Copy variable.
		return true;
		}
	return false;
	}

bool 	
Serializable::WriteVariableInVar_Binary(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	LongBinary*			varPtr		= (LongBinary*)&((LPBYTE)pThis)[nOffset];
	LongBinary*			varPtrIn	= (LongBinary*)&((LPBYTE)pInto)[nOffset];

	if( pInfo->m_type == VariableType::VT_Binary ){
		int nSize =varPtr->GetBinarySize();
		if( nSize > 0 ){
			void*	lpDest	= varPtr->LockMemory();
			bool	bRet	= (varPtrIn->SetBinarySize(nSize, lpDest) == TRUE);
			varPtr->UnlockMemory();
			return bRet;
			}
		else
			varPtrIn->SetBinarySize(0);
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVarcRect(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	_Rect*				varPtr		= (_Rect*)&((LPBYTE)pThis)[nOffset];
	_Rect*				varPtrIn	= (_Rect*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Rect ){
		varPtrIn->SetRect(varPtr->left, varPtr->top, varPtr->right, varPtr->bottom);
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Point(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	_Point*				varPtr		= (_Point*)&((LPBYTE)pThis)[nOffset];
	_Point*				varPtrIn	= (_Point*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Point ){
		varPtrIn->SetPoint(varPtr->x, varPtr->y);
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Size(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	_Size*				varPtr		= (_Size*)&((LPBYTE)pThis)[nOffset];
	_Size*				varPtrIn	= (_Size*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Size ){
		varPtrIn->SetSize(varPtr->cx, varPtr->cy);
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Image(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	Image*				varPtr		= (Image*)&((LPBYTE)pThis)[nOffset];
	Image*				varPtrIn	= (Image*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Image ){
		varPtrIn->Destroy();
		return (varPtr->CopyImage(varPtrIn) == TRUE);
		/*
		LongBinary bin;
		if( BinaryFromImage(varPtr, &bin) ){
			int nImageDataSize = 0;
			return ImageFromBinary(&bin, varPtrIn, nImageDataSize);
			}*/
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Font(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	Font*				varPtr		= (Font*)&((LPBYTE)pThis)[nOffset];
	Font*				varPtrIn	= (Font*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Font ){
		varPtrIn->DeleteObject();

		LOGFONT lf;
		if( !varPtr->IsNull() && varPtr->GetLogFont(&lf) ){
			varPtrIn->CreateFontIndirect(&lf);
			}
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Color32(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	COLORREF*			varPtr		= (COLORREF*)&((LPBYTE)pThis)[nOffset];
	COLORREF*			varPtrIn	= (COLORREF*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Color32 ){
		*varPtrIn = *varPtr;
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Enum(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int					nOffset		= pInfo->m_nOffset;
	StringToIntArray*	varPtr		= (StringToIntArray*)&((LPBYTE)pThis)[nOffset];
	StringToIntArray*	varPtrIn	= (StringToIntArray*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Enumerable ){
		varPtrIn->SetCurrentIndex(varPtr->GetCurrentIndex());
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_VoidPtr(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int		nOffset		= pInfo->m_nOffset;
	void**	varPtr		= (void**)&((LPBYTE)pThis)[nOffset];
	void**	varPtrIn	= (void**)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_VoidPtr ){
		*varPtrIn = *varPtr;
		return true;
		}
	return false;
	}

bool
Serializable::WriteVariableInVar_Chips(Serializable* pThis, VariableInfo* pInfo, Serializable* pInto){
	ASSERT(pInfo);
	int			nOffset		= pInfo->m_nOffset;
	INT_CHIPS*	varPtr		= (INT_CHIPS*)&((LPBYTE)pThis)[nOffset];
	INT_CHIPS*	varPtrIn	= (INT_CHIPS*)&((LPBYTE)pInto)[nOffset];
	if( pInfo->m_type == VariableType::VT_Chips ){
		*varPtrIn = *varPtr;
		return true;
		}
	return false;
	}

bool 
Serializable::ZeroInitVariable_Bool(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Bool ){
		*((bool*)varPtr) = false;
		return true;
		}
	return false;
	}
bool
Serializable::ZeroInitVariable_Int8(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int8 ){
		*((char*)varPtr) = 0;
		return true;
		}
	return false;
	}
bool 	
Serializable::ZeroInitVariable_Int16(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int16 ){
		*((short*)varPtr) = 0;
		return true;
		}
	return false;
	}
bool 	
Serializable::ZeroInitVariable_Int32(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int32 ){
		*((int*)varPtr) = 0;
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_Int64(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Int64 ){
		*((__int64*)varPtr) = 0;
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_String(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_String ){
		*((_String*)varPtr) = _T("");
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_DateTime(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_DateTime ){
		((COleDateTime*)varPtr)->SetStatus(ATL::COleDateTime::DateTimeStatus::null);
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_Date(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Date ){
		((COleDateTime*)varPtr)->SetStatus(ATL::COleDateTime::DateTimeStatus::null);
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_Time(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Time ){
		((COleDateTime*)varPtr)->SetStatus(ATL::COleDateTime::DateTimeStatus::null);
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_Float(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Float ){
		*((float*)varPtr) = 0.0f;
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_Double(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Double ){
		*((double*)varPtr) = 0.0;
		return true;
		}
	return false;
	}

bool 	
Serializable::ZeroInitVariable_Binary(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	void*				varPtr	= (void*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Binary ){
		LongBinary* pBinary = (LongBinary*)varPtr;
		pBinary->SetBinarySize(0);
		return true;
		}
	return false;
	}

bool 
Serializable::ZeroInitVariable_Rect(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Rect*				varPtr	= (_Rect*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Rect ){
		varPtr->SetRect(0, 0, 0, 0);
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_Point(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Point*				varPtr	= (_Point*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Point ){
		varPtr->SetPoint(0, 0);
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_Size(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	_Size*				varPtr	= (_Size*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Size ){
		varPtr->SetSize(0, 0);
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_Image(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	Image*	varPtr	= (Image*)&((LPBYTE)pThis)[nOffset];

	if( pInfo->m_type == VariableType::VT_Image ){
		varPtr->Destroy();
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_Font(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int		nOffset = pInfo->m_nOffset;
	Font*	varPtr	= (Font*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Font ){
		varPtr->DeleteObject();
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_Color32(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	COLORREF*	varPtr	= (COLORREF*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Color32 ){
		*varPtr = 0L;
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_Enum(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int					nOffset = pInfo->m_nOffset;
	StringToIntArray*	varPtr	= (StringToIntArray*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Enumerable ){
		varPtr->SetCurrentIndex(-1);
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_VoidPtr(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	void**		varPtr	= (void**)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_VoidPtr ){
		*varPtr = NULL;
		return true;
		}
	return false;
	}

bool
Serializable::ZeroInitVariable_Chips(Serializable* pThis, VariableInfo* pInfo){
	ASSERT(pInfo);
	int			nOffset = pInfo->m_nOffset;
	INT_CHIPS*	varPtr	= (INT_CHIPS*)&((LPBYTE)pThis)[nOffset];
	if( pInfo->m_type == VariableType::VT_Chips ){
		*varPtr = 0;
		return true;
		}
	return false;
	}