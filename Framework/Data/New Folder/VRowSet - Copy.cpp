// ===========================================================================
// About: MFC new generation data management class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#include "StdAfx.h"
#include "VRowSet.h"

VirtualRowSet::VirtualRowSet(){
	m_pFields	= NULL;
	m_pRow		= NULL;
	m_pRowRoot	= NULL;
	m_bCalcRows = FALSE;
	m_nRowCt	= 0;
	}

VirtualRowSet::~VirtualRowSet(){
	// Empty rowset values.
	EmptyRowSet();
	}

int
VirtualRowSet::Create(){
	if( !m_pFields )	return 0;
	if( m_pRow )		return m_pFields->GetCount();
	int nRowSize	= sizeof(VRowHeader);
	int	nLoop		= 0;

	// Calculate row size in bytes.
	VRowSetFieldInfo* pField		= m_pFields->GetAt(0);
	VRowSetFieldInfo* pFieldHead	= pField;
	while( pField ){

		switch( pField->type ){
			case FTBool:
				{	// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetBoolAsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetBoolAsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetBoolAsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetBoolAsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetBoolAsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetBoolAsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetBoolAsDouble);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetBoolAsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetBoolAsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetBoolAsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetBoolAsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetBoolAsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetBoolAsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetBoolAsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetBoolAsDouble);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetBoolAsString);
					break;
				}
			case FTInt8:
				{	// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetInt8AsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetInt8AsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetInt8AsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetInt8AsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetInt8AsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetInt8AsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetInt8AsDouble);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetInt8AsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetInt8AsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetInt8AsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetInt8AsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetInt8AsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetInt8AsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetInt8AsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetInt8AsDouble);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetInt8AsString);
					break;
				}
			case FTInt16:
				{	// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetInt16AsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetInt16AsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetInt16AsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetInt16AsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetInt16AsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetInt16AsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetInt16AsDouble);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetInt16AsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetInt16AsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetInt16AsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetInt16AsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetInt16AsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetInt16AsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetInt16AsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetInt16AsDouble);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetInt16AsString);
					break;
				}
			case FTInt32:
				{	// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetInt32AsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetInt32AsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetInt32AsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetInt32AsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetInt32AsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetInt32AsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetInt32AsDouble);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetInt32AsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetInt32AsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetInt32AsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetInt32AsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetInt32AsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetInt32AsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetInt32AsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetInt32AsDouble);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetInt32AsString);
					break;
				}
			case FTInt64:
				{	// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetInt64AsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetInt64AsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetInt64AsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetInt64AsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetInt64AsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetInt64AsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetInt64AsDouble);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetInt64AsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetInt64AsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetInt64AsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetInt64AsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetInt64AsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetInt64AsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetInt64AsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetInt64AsDouble);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetInt64AsString);
					break;
				}
			case FTFloat:
				{	// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetFloatAsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetFloatAsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetFloatAsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetFloatAsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetFloatAsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetFloatAsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetFloatAsDouble);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetFloatAsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetFloatAsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetFloatAsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetFloatAsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetFloatAsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetFloatAsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetFloatAsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetFloatAsDouble);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetFloatAsString);
					break;
				}
			case FTDouble:
				{	// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetDoubleAsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetDoubleAsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetDoubleAsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetDoubleAsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetDoubleAsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetDoubleAsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetDoubleAsDouble);
					pField->methodGetAsDate		= ((VRowSetFieldInfoTag::GetFieldAsDate)GetDoubleAsDate);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetDoubleAsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetDoubleAsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetDoubleAsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetDoubleAsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetDoubleAsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetDoubleAsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetDoubleAsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetDoubleAsDouble);
					pField->methodSetAsDate		= ((VRowSetFieldInfoTag::SetFieldAsDate)SetDoubleAsDate);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetDoubleAsString);
					break;
				}
			case FTDate:
				{	// Get
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetDateAsDouble);
					pField->methodGetAsDate		= ((VRowSetFieldInfoTag::GetFieldAsDate)GetDateAsDate);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetDateAsString);
					// Set
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetDateAsDouble);
					pField->methodSetAsDate		= ((VRowSetFieldInfoTag::SetFieldAsDate)SetDateAsDate);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetDateAsString);
					break;
				}
			case FTDateTime:
				{	// Get
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetDateTimeAsDouble);
					pField->methodGetAsDate		= ((VRowSetFieldInfoTag::GetFieldAsDate)GetDateTimeAsDate);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetDateTimeAsString);
					// Set
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetDateTimeAsDouble);
					pField->methodSetAsDate		= ((VRowSetFieldInfoTag::SetFieldAsDate)SetDateTimeAsDate);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetDateTimeAsString);
					break;
				}
			case FTChar:
				{
#ifndef _UNICODE
					// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetStringAsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetStringAsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetStringAsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetStringAsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetStringAsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetStringAsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetStringAsDouble);
					pField->methodGetAsDate		= ((VRowSetFieldInfoTag::GetFieldAsDate)GetStringAsDate);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetStringAsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetStringAsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetStringAsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetStringAsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetStringAsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetStringAsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetStringAsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetStringAsDouble);
					pField->methodSetAsDate		= ((VRowSetFieldInfoTag::SetFieldAsDate)SetStringAsDate);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetStringAsString);
#endif
					break;
				}
			case FTWchar:
				{
#ifdef _UNICODE
					// Get
					pField->methodGetAsBool		= ((VRowSetFieldInfoTag::GetFieldAsBool)GetStringAsBool);
					pField->methodGetAsInt8		= ((VRowSetFieldInfoTag::GetFieldAsInt8)GetStringAsInt8);
					pField->methodGetAsInt16	= ((VRowSetFieldInfoTag::GetFieldAsInt16)GetStringAsInt16);
					pField->methodGetAsInt32	= ((VRowSetFieldInfoTag::GetFieldAsInt32)GetStringAsInt32);
					pField->methodGetAsInt64	= ((VRowSetFieldInfoTag::GetFieldAsInt64)GetStringAsInt64);
					pField->methodGetAsFloat	= ((VRowSetFieldInfoTag::GetFieldAsFloat)GetStringAsFloat);
					pField->methodGetAsDouble	= ((VRowSetFieldInfoTag::GetFieldAsDouble)GetStringAsDouble);
					pField->methodGetAsDate		= ((VRowSetFieldInfoTag::GetFieldAsDate)GetStringAsDate);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetStringAsString);
					// Set
					pField->methodSetAsBool		= ((VRowSetFieldInfoTag::SetFieldAsBool)SetStringAsBool);
					pField->methodSetAsInt8		= ((VRowSetFieldInfoTag::SetFieldAsInt8)SetStringAsInt8);
					pField->methodSetAsInt16	= ((VRowSetFieldInfoTag::SetFieldAsInt16)SetStringAsInt16);
					pField->methodSetAsInt32	= ((VRowSetFieldInfoTag::SetFieldAsInt32)SetStringAsInt32);
					pField->methodSetAsInt64	= ((VRowSetFieldInfoTag::SetFieldAsInt64)SetStringAsInt64);
					pField->methodSetAsFloat	= ((VRowSetFieldInfoTag::SetFieldAsFloat)SetStringAsFloat);
					pField->methodSetAsDouble	= ((VRowSetFieldInfoTag::SetFieldAsDouble)SetStringAsDouble);
					pField->methodSetAsDate		= ((VRowSetFieldInfoTag::SetFieldAsDate)SetStringAsDate);
					pField->methodSetAsString	= ((VRowSetFieldInfoTag::SetFieldAsString)SetStringAsString);
#endif
					break;
				}
			case FTBinary:
				{
					pField->methodGetAsBinary	= ((VRowSetFieldInfoTag::GetFieldAsBinary)GetBinaryAsBinary);
					pField->methodGetAsString	= ((VRowSetFieldInfoTag::GetFieldAsString)GetBinaryAsString);
					break;
				}
			};

		// Save offset of field value length.
		pField->dwValueLengthOffset = nRowSize;
		// Length of current field value in row.
		nRowSize += sizeof(int);
		// Save offset of field value.
		pField->dwValueOffset = nRowSize;
		// Length of field value.
		nRowSize += pField->nSize;
		pField = pField->next; nLoop ++;
		}

	// Allocate row memory and initialize it.
	void*		lpRow		= new BYTE[nRowSize];
	VRowHeader* pRowHeader	= (VRowHeader*)lpRow;
	pRowHeader->nFieldCt	= nLoop;
	pRowHeader->nMemAlloc	= nRowSize;
	pRowHeader->nMemUsage	= nRowSize;
	pRowHeader->pChildRow	= NULL;
	pRowHeader->pNextRow	= NULL;
	pRowHeader->pPrevRow	= NULL;
	pRowHeader->pOwner		= this;
	pRowHeader->pParentRow	= NULL;

	// Set null size to all fields.
	while( pFieldHead ){
		// Set null length.
		*(int*)(((__int64)lpRow) + ((__int64)pFieldHead->dwValueLengthOffset)) = -1;
		pFieldHead = pFieldHead->next;
		}

	m_pRowRoot = (VRowHeader*)lpRow;
	return nLoop;
	}

int
VirtualRowSet::AddField(TCHAR* pszName, FieldType fType)
	{
	if( m_pFields == NULL )
		m_pFields = new ChainTempl<VRowSetFieldInfo>();
	// Field with that name already exists.
	if( FieldExists(pszName) ) return -1;
	// Initialize field info structure.
	VRowSetFieldInfo* pNew		= new VRowSetFieldInfo();
	pNew->next					= NULL;
	pNew->prev					= NULL;
	pNew->type					= fType;
	pNew->dwValueOffset			= 0;
	pNew->dwValueLengthOffset	= 0;
	_tcscpy_s(pNew->szName, pszName);
	pNew->bIsBlob				= false;
	pNew->nSize					= _field_default_size_by_type[(int)fType];
	pNew->nOrdinal				= m_pFields->GetCount();
	if( fType == FTBinary )
		pNew->bIsBlob = true;

	pNew->methodGetAsBool	= NULL;
	pNew->methodGetAsInt8	= NULL;
	pNew->methodGetAsInt16	= NULL;
	pNew->methodGetAsInt32	= NULL;
	pNew->methodGetAsInt64	= NULL;
	pNew->methodGetAsFloat	= NULL;
	pNew->methodGetAsDouble	= NULL;
	pNew->methodGetAsDate	= NULL;
	pNew->methodGetAsBinary	= NULL;
	pNew->methodGetAsString	= NULL;

	pNew->methodSetAsBool	= NULL;
	pNew->methodSetAsInt8	= NULL;
	pNew->methodSetAsInt16	= NULL;
	pNew->methodSetAsInt32	= NULL;
	pNew->methodSetAsInt64	= NULL;
	pNew->methodSetAsFloat	= NULL;
	pNew->methodSetAsDouble	= NULL;
	pNew->methodSetAsDate	= NULL;
	pNew->methodSetAsBinary	= NULL;
	pNew->methodSetAsString	= NULL;

	// Add into chain.
	m_pFields->Add(pNew, false);
	return pNew->nOrdinal;
	}

int
VirtualRowSet::AddFieldEx(TCHAR* pszName, FieldType fType, int nLength)
	{
	if( m_pFields == NULL )
		m_pFields = new ChainTempl<VRowSetFieldInfo>();
	// Field with that name already exists.
	if( FieldExists(pszName) ) return -1;
	// Initialize field info structure.
	VRowSetFieldInfo* pNew		= new VRowSetFieldInfo();
	pNew->next					= NULL;
	pNew->prev					= NULL;
	pNew->type					= fType;
	pNew->dwValueOffset			= 0;
	pNew->dwValueLengthOffset	= 0;
	_tcscpy_s(pNew->szName, pszName);
	pNew->bIsBlob				= false;
	pNew->nSize					= nLength;
	pNew->nOrdinal				= m_pFields->GetCount();
	if( fType == FTBinary )
		pNew->bIsBlob			= true;

	pNew->methodGetAsBool	= NULL;
	pNew->methodGetAsInt8	= NULL;
	pNew->methodGetAsInt16	= NULL;
	pNew->methodGetAsInt32	= NULL;
	pNew->methodGetAsInt64	= NULL;
	pNew->methodGetAsFloat	= NULL;
	pNew->methodGetAsDouble	= NULL;
	pNew->methodGetAsDate	= NULL;
	pNew->methodGetAsBinary	= NULL;
	pNew->methodGetAsString	= NULL;

	pNew->methodSetAsBool	= NULL;
	pNew->methodSetAsInt8	= NULL;
	pNew->methodSetAsInt16	= NULL;
	pNew->methodSetAsInt32	= NULL;
	pNew->methodSetAsInt64	= NULL;
	pNew->methodSetAsFloat	= NULL;
	pNew->methodSetAsDouble	= NULL;
	pNew->methodSetAsDate	= NULL;
	pNew->methodSetAsBinary	= NULL;
	pNew->methodSetAsString	= NULL;

	// Add into chain.
	m_pFields->Add(pNew, false);
	return pNew->nOrdinal;
	}

bool
VirtualRowSet::RemoveField(TCHAR* pszName)
	{
	int nOrdinal = GetOrdinalIndex(pszName);
	if( nOrdinal == -1 ) return false;
	return RemoveField(nOrdinal);
	}

bool
VirtualRowSet::RemoveField(int nFieldIndex)
	{
	if( !m_pFields ) return false; 
	VRowSetFieldInfo*	pInfo = m_pFields->GetAt(nFieldIndex);
	if( !pInfo ) return false;

	// Reindex fields in chain.
	int nOrdinal	= nFieldIndex;
	pInfo			= pInfo->next;
	while( pInfo ){
		pInfo->nOrdinal = nOrdinal;
		pInfo			= pInfo->next;
		nOrdinal		++;
		}
	// ###########################

	// Remove from chain.
	return m_pFields->RemoveAt(nFieldIndex);
	}

int
VirtualRowSet::GetRowCount(){
	int			nRetCt	= m_nRowCt;
	if( m_bCalcRows ){
		nRetCt = 0;
		VRowHeader* pRow	= m_pRowRoot;
		while( pRow ){
			nRetCt	++;
			pRow	= pRow->pNextRow;
			}
		m_bCalcRows = FALSE;
		m_nRowCt	= nRetCt;
		}
	return m_nRowCt;
	}

int
VirtualRowSet::GetOrdinalIndex(TCHAR* pszName, VRowSetFieldInfo** ppFieldInfo /*= NULL*/){
	if( !m_pFields ) return -1; 
	VRowSetFieldInfo*	pInfo = m_pFields->FindWithCharArray(offsetof(VRowSetFieldInfo, szName), pszName);
	
	if( ppFieldInfo )
		*ppFieldInfo = pInfo;

	if( pInfo )
		return pInfo->nOrdinal;
	return -1;
	}

bool
VirtualRowSet::FieldExists(TCHAR* pszName){
	if( !m_pFields ) return false; 
	VRowSetFieldInfo*	pInfo = m_pFields->FindWithCharArray(offsetof(VRowSetFieldInfo, szName), pszName);
	return (pInfo != NULL);
	}

void
VirtualRowSet::EmptyRowSet()
	{
	if( m_pFields != NULL ){
		// Destroy full rowset.
		if( m_pRowRoot ){
			// Determine blob fields
			CPtrArray			arrBlobFields;
			VRowSetFieldInfo*	pField = m_pFields->GetAt(0);
			while( pField ){
				if( pField->bIsBlob )
					arrBlobFields.Add(pField);
				pField = pField->next;
				}
			// =====================

			VRowHeader* pRow = m_pRowRoot;
			// Move up to first row of set.
			while( pRow->pPrevRow )
				pRow = pRow->pPrevRow;
			// ########################
			
			VRowSetFieldInfo*	pInfo	= NULL;
			int					nBlobCt = (int)arrBlobFields.GetCount();
			int					nLoop	= 0;

			if( nBlobCt > 0 ) // Destroy rowsets with blob fields.
				{ 
				while( pRow ){
					VRowHeader* pNextRow = pRow->pNextRow;
					nLoop = 0;
					while( nLoop < nBlobCt ){ // Destroy blob values.
						// Retrieve field info.
						pInfo = (VRowSetFieldInfo*)arrBlobFields[nLoop];
						// Retrieve field length pointer.
						int* lpBlobValueLength = (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
						// Check blob value size.
						if( *lpBlobValueLength > -1 ){
							// Retrieve blob value.
							void* lpBlobValue = (void*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
							// Destroy it.
							delete lpBlobValue;
							}
						nLoop ++;
						}

					delete pRow;
					pRow = pNextRow;
					}
				}
			else{ // Destroy rowsets without blob fields.
				while( pRow ){
					VRowHeader* pNextRow = pRow->pNextRow;
					// Destroy full row.
					delete pRow;
					pRow = pNextRow;
					}
				}
			m_pRow		= NULL;
			m_pRowRoot	= NULL;
			}
		// ###################

		delete m_pFields;
		m_pFields = NULL;
		}

	m_bCalcRows = FALSE;
	m_nRowCt	= 0;
	}

void
VirtualRowSet::GetFieldPtrs(CPtrArray& arrFieldPtrs)
	{
	}

int*
VirtualRowSet::GetFieldLengthPtr(TCHAR* pszName)
	{
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return NULL;
	return GetFieldLengthPtr(nFieldIdx);
	}

int*
VirtualRowSet::GetFieldLengthPtr(int nFieldIndex)
	{
	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( !pInfo ) return NULL;
	int* lpValueLen = (int*)(((__int64)m_pRow) + ((__int64)pInfo->dwValueLengthOffset));
	return lpValueLen;
	}

void*	
VirtualRowSet::GetFieldValuePtr(TCHAR* pszName)
	{
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return NULL;
	return GetFieldValuePtr(nFieldIdx);
	}

void*
VirtualRowSet::GetFieldValuePtr(int nFieldIndex)
	{
	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( !pInfo ) return NULL;
	void* lpValue = (void*)(((__int64)m_pRow) + ((__int64)pInfo->dwValueOffset));
	return lpValue;
	}

int
VirtualRowSet::GetFieldCount(){
	if( !m_pFields ) return 0;
	return m_pFields->GetCount();
	}

VRowHeader*
VirtualRowSet::AllocateRow(){
	if( !m_pRowRoot ) return NULL;
	int nRowSize = m_pRowRoot->nMemAlloc;
	if( nRowSize <= 0 ) return NULL;

	// Allocate row memory and initialize it.
	void*		lpRow		= new BYTE[nRowSize];
	VRowHeader* pRowHeader	= (VRowHeader*)lpRow;
	pRowHeader->nFieldCt	= GetFieldCount();
	pRowHeader->nMemAlloc	= nRowSize;
	pRowHeader->nMemUsage	= nRowSize;
	pRowHeader->pChildRow	= NULL;
	pRowHeader->pNextRow	= NULL;
	pRowHeader->pPrevRow	= NULL;
	pRowHeader->pOwner		= this;
	pRowHeader->pParentRow	= NULL;

	// Set null size to all fields.
	VRowSetFieldInfo* pField = m_pFields->GetAt(0);
	while( pField ){
		// Set null length.
		*(int*)(((__int64)lpRow) + ((__int64)pField->dwValueLengthOffset)) = -1;
		pField = pField->next;
		}

	return pRowHeader;
	}

bool
VirtualRowSet::AddRow(){
	VRowHeader* pRowNew = AllocateRow();
	if( !pRowNew ) return false;


	VRowHeader* pRowLast = m_pRow;
	if( pRowLast  ){
		while( pRowLast->pNextRow )
			pRowLast = pRowLast->pNextRow;
		}
	else{
		pRowLast = m_pRowRoot;
		}

	pRowLast->pNextRow	= pRowNew;
	pRowNew->pPrevRow	= pRowLast;
	m_nRowCt			= GetRowCount();
	m_nRowCt			++;

	m_nCurrentRow		= m_nRowCt - 1;
	m_pRow				= pRowLast;
	return true;
	}

bool
VirtualRowSet::SetCurrentRow(int nRow){
	VRowHeader* pRow = GetRowByIndex(nRow);
	if( !pRow ) return false;

	m_nCurrentRow	= nRow;
	m_pRow			= pRow;
	return true;
	}

VRowHeader*	
VirtualRowSet::GetRowByIndex(int nRow){
	int nRowCt = GetRowCount();
	if( nRow >= nRowCt || nRow < 0 ) return NULL;

	VRowHeader* pRow	= m_pRow;
	int			nMove	= nRow - m_nCurrentRow;
	if( nMove > 0 ){ 
		while( nMove != 0 ){
			ASSERT(pRow);
#ifdef _DEBUG
			if( !pRow ) return NULL;
#endif
			pRow = pRow->pNextRow;
			nMove --;
			}
		}
	else
	if( nMove < 0 ){
		while( nMove != 0 ){
			ASSERT(pRow);
#ifdef _DEBUG
			if( !pRow ) return NULL;
#endif
			pRow = pRow->pPrevRow;
			nMove ++;
			}
		}

	return pRow;
	}

int
VirtualRowSet::GetRowMemSize(){
	if( !m_pRow ) return 0;
	return m_pRow->nMemAlloc;
	}

VRowSetFieldInfo*
VirtualRowSet::GetFieldInfo(TCHAR* pszName){
	VRowSetFieldInfo* pInfo = NULL;
	GetOrdinalIndex(pszName, &pInfo);
	return pInfo;
	}

VRowSetFieldInfo*
VirtualRowSet::GetFieldInfo(int nFieldIdx){
	if( !m_pFields ) return NULL; 
	VRowSetFieldInfo*	pInfo = m_pFields->GetAt(nFieldIdx);
	return pInfo;
	}

// =============================================================
//
//	Set field value methods.
//
// =============================================================

bool
VirtualRowSet::SetBool(TCHAR* pszName, bool bVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetBool(nFieldIdx, bVal);
	}

bool
VirtualRowSet::SetBool(int nFieldIndex, bool bVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsBool ){
		return (pInfo->methodSetAsBool)(pInfo, m_pRow, bVal);
		}
	return false;
	}

bool
VirtualRowSet::SetInt8(TCHAR* pszName, char cVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetInt8(nFieldIdx, cVal);
	}

bool
VirtualRowSet::SetInt8(int nFieldIndex, char cVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsInt8 ){
		return (pInfo->methodSetAsInt8)(pInfo, m_pRow, cVal);
		}
	return false;
	}

bool
VirtualRowSet::SetInt16(TCHAR* pszName, short shVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetInt16(nFieldIdx, shVal);
	}

bool
VirtualRowSet::SetInt16(int nFieldIndex, short shVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsInt16 ){
		return (pInfo->methodSetAsInt16)(pInfo, m_pRow, shVal);
		}
	return false;
	}

bool
VirtualRowSet::SetInt32(TCHAR* pszName, int nVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetInt32(nFieldIdx, nVal);
	}

bool
VirtualRowSet::SetInt32(int nFieldIndex, int nVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsInt32 ){
		return (pInfo->methodSetAsInt32)(pInfo, m_pRow, nVal);
		}
	return false;
	}

bool
VirtualRowSet::SetInt64(TCHAR* pszName, __int64 nVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetInt64(nFieldIdx, nVal);
	}

bool
VirtualRowSet::SetInt64(int nFieldIndex, __int64 nVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsInt64 ){
		return (pInfo->methodSetAsInt64)(pInfo, m_pRow, nVal);
		}
	return false;
	}

bool
VirtualRowSet::SetFloat(TCHAR* pszName, float fVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetFloat(nFieldIdx, fVal);
	}

bool
VirtualRowSet::SetFloat(int nFieldIndex, float fVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsFloat ){
		return (pInfo->methodSetAsFloat)(pInfo, m_pRow, fVal);
		}
	return false;
	}

bool
VirtualRowSet::SetDouble(TCHAR* pszName, double dVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetDouble(nFieldIdx, dVal);
	}

bool
VirtualRowSet::SetDouble(int nFieldIndex, double dVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsDouble ){
		return (pInfo->methodSetAsDouble)(pInfo, m_pRow, dVal);
		}
	return false;
	}

bool
VirtualRowSet::SetDate(TCHAR* pszName, COleDateTime dtVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetDate(nFieldIdx, dtVal);
	}

bool
VirtualRowSet::SetDate(int nFieldIndex, COleDateTime dtVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsDate ){
		return (pInfo->methodSetAsDate)(pInfo, m_pRow, dtVal);
		}
	return false;
	}

bool
VirtualRowSet::SetDate(TCHAR* pszName, int nYear, int nMonth, int nDay){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetDate(nFieldIdx, nYear, nMonth, nDay);
	}

bool
VirtualRowSet::SetDate(int nFieldIndex, int nYear, int nMonth, int nDay){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsDate ){
		COleDateTime dtVal;
		dtVal.SetDate(nYear, nMonth, nDay);
		return (pInfo->methodSetAsDate)(pInfo, m_pRow, dtVal);
		}
	return false;	
	}

bool
VirtualRowSet::SetDateTime(TCHAR* pszName, COleDateTime dtVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetDateTime(nFieldIdx, dtVal);
	}

bool
VirtualRowSet::SetDateTime(int nFieldIndex, COleDateTime dtVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsDate ){
		return (pInfo->methodSetAsDate)(pInfo, m_pRow, dtVal);
		}
	return false;
	}

bool
VirtualRowSet::SetDateTime(TCHAR* pszName, int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetDateTime(nFieldIdx, nYear, nMonth, nDay, nHour, nMinute, nSecond);
	}

bool
VirtualRowSet::SetDateTime(int nFieldIndex, int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsDate ){
		COleDateTime dtVal;
		dtVal.SetDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);
		return (pInfo->methodSetAsDate)(pInfo, m_pRow, dtVal);
		}
	return false;
	}

bool
VirtualRowSet::SetString(TCHAR* pszName, CString sVal){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetString(nFieldIdx, sVal);
	}

bool
VirtualRowSet::SetString(int nFieldIndex, CString sVal){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodSetAsString ){
		return (pInfo->methodSetAsString)(pInfo, m_pRow, sVal);
		}
	return false;
	}

bool
VirtualRowSet::SetBinary(TCHAR* pszName, void* lpData, int nSize, BOOL bCopyData /*= FALSE*/){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return SetBinary(nFieldIdx, lpData, nSize, bCopyData);
	}

bool
VirtualRowSet::SetBinary(int nFieldIndex, void* lpData, int nSize, BOOL bCopyData /*= FALSE*/){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	ASSERT( AfxIsValidAddress(lpData, nSize, TRUE) );
	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);

	if( pInfo && pInfo->methodSetAsBinary ){
		BinaryData bin = {lpData, nSize};
		return (pInfo->methodSetAsBinary)(pInfo, m_pRow, bin, bCopyData);
		}
	return false;
	}

// =============================================================
//
//	Get field value methods.
//
// =============================================================

bool
VirtualRowSet::GetBool(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetBool(nFieldIdx);
	}

bool
VirtualRowSet::GetBool(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsBool ){
		return (pInfo->methodGetAsBool)(pInfo, m_pRow);
		}
	return false;
	}

char
VirtualRowSet::GetInt8(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetInt8(nFieldIdx);
	}

char	
VirtualRowSet::GetInt8(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return '\0';

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsInt8 ){
		return (pInfo->methodGetAsInt8)(pInfo, m_pRow);
		}
	return '\0';
	}

short	
VirtualRowSet::GetInt16(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetInt16(nFieldIdx);
	}

short	
VirtualRowSet::GetInt16(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return 0;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsInt16 ){
		return (pInfo->methodGetAsInt16)(pInfo, m_pRow);
		}
	return 0;
	}

int		
VirtualRowSet::GetInt32(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetInt32(nFieldIdx);
	}

int		
VirtualRowSet::GetInt32(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return 0;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsInt32 ){
		return (pInfo->methodGetAsInt32)(pInfo, m_pRow);
		}
	return 0;
	}

__int64	
VirtualRowSet::GetInt64(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetInt64(nFieldIdx);
	}

__int64	
VirtualRowSet::GetInt64(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return 0;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsInt64 ){
		return (pInfo->methodGetAsInt64)(pInfo, m_pRow);
		}
	return 0;
	}

float	
VirtualRowSet::GetFloat(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetFloat(nFieldIdx);
	}

float	
VirtualRowSet::GetFloat(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return 0.0f;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsFloat ){
		return (pInfo->methodGetAsFloat)(pInfo, m_pRow);
		}
	return 0.0f;
	}

double	
VirtualRowSet::GetDouble(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetDouble(nFieldIdx);
	}

double	
VirtualRowSet::GetDouble(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return 0.0;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDouble ){
		return (pInfo->methodGetAsDouble)(pInfo, m_pRow);
		}
	return 0.0;
	}

bool	
VirtualRowSet::GetDate(TCHAR* pszName, COleDateTime& dtDate){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetDate(nFieldIdx, dtDate);
	}

bool	
VirtualRowSet::GetDate(int nFieldIndex, COleDateTime& dtDate){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return true;
		}
	return false;
	}

bool	
VirtualRowSet::GetDateTime(TCHAR* pszName, COleDateTime& dtDate){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetDateTime(nFieldIdx, dtDate);
	}

bool	
VirtualRowSet::GetDateTime(int nFieldIndex, COleDateTime& dtDate){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return true;
		}
	return false;
	}

int		
VirtualRowSet::GetYear(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetYear(nFieldIdx);
	}

int		
VirtualRowSet::GetYear(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return -1;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		COleDateTime dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return dtDate.GetYear();
		}
	return -1;
	}

int		
VirtualRowSet::GetMonth(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetMonth(nFieldIdx);
	}

int		
VirtualRowSet::GetMonth(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return -1;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		COleDateTime dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return dtDate.GetMonth();
		}
	return -1;
	}

int		
VirtualRowSet::GetDay(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetDay(nFieldIdx);
	}

int		
VirtualRowSet::GetDay(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return -1;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		COleDateTime dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return dtDate.GetDay();
		}
	return -1;
	}

int		
VirtualRowSet::GetHour(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetHour(nFieldIdx);
	}

int		
VirtualRowSet::GetHour(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return -1;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		COleDateTime dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return dtDate.GetHour();
		}
	return -1;
	}

int		
VirtualRowSet::GetMinute(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetMinute(nFieldIdx);
	}

int		
VirtualRowSet::GetMinute(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return -1;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		COleDateTime dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return dtDate.GetMinute();
		}
	return -1;
	}

int		
VirtualRowSet::GetSecond(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetSecond(nFieldIdx);
	}

int		
VirtualRowSet::GetSecond(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return -1;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsDate ){
		COleDateTime dtDate = (pInfo->methodGetAsDate)(pInfo, m_pRow);
		return dtDate.GetSecond();
		}
	return -1;
	}

CString	
VirtualRowSet::GetString(TCHAR* pszName){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetString(nFieldIdx);
	}

CString	
VirtualRowSet::GetString(int nFieldIndex){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return _T("");

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsString ){
		return (pInfo->methodGetAsString)(pInfo, m_pRow);
		}
	return _T("");
	}

bool	
VirtualRowSet::GetString(TCHAR* pszName, CString& strValue){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return false;
	return GetString(nFieldIdx, strValue);
	}

bool	
VirtualRowSet::GetString(int nFieldIndex, CString& strValue){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return false;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsString ){
		strValue = (pInfo->methodGetAsString)(pInfo, m_pRow);
		return true;
		}
	return false;
	}

// returns length of binary data.
int		
VirtualRowSet::GetBinary(TCHAR* pszName, BinaryData& binaryValue){
	int nFieldIdx = GetOrdinalIndex(pszName);
	if( nFieldIdx == -1 ) return 0;
	return GetBinary(nFieldIdx, binaryValue);
	}

// returns length of binary data.
int		
VirtualRowSet::GetBinary(int nFieldIndex, BinaryData& binaryValue){
	ASSERT( m_pFields );
	ASSERT( m_pRow );
	if( nFieldIndex < 0 || nFieldIndex >= m_pRow->nFieldCt )
		return 0;

	VRowSetFieldInfo* pInfo = GetFieldInfo(nFieldIndex);
	if( pInfo && pInfo->methodGetAsBinary ){
		binaryValue = (pInfo->methodGetAsBinary)(pInfo, m_pRow);
		return binaryValue.length;
		}
	return 0;
	}
