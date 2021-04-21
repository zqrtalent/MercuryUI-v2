// ===========================================================================
// About: MFC new generation data management class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#include "StdAfx.h"
#include "VRowSet.h"
/*
#include "ATLComTime.h"
#include "sqltypes.h"
*/
// ---------------------------------------------------------------------------
// Data convertion methods.
// ---------------------------------------------------------------------------

// Bool
bool
VirtualRowSet::GetBoolAsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr == -1 ) // NULL
		return false;
	return *lpValuePtr;
	}


_String
VirtualRowSet::GetBoolAsString	(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("F");
	return *lpValuePtr == true ? _T("T") : _T("F");
	}

char
VirtualRowSet::GetBoolAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return *lpValuePtr == true ? '\1' : '\0';
	}

short
VirtualRowSet::GetBoolAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return *lpValuePtr == true ? 1 : 0;
	}

int
VirtualRowSet::GetBoolAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return *lpValuePtr == true ? 1 : 0;
	}

__int64
VirtualRowSet::GetBoolAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return *lpValuePtr == true ? 1 : 0;
	}

float
VirtualRowSet::GetBoolAsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return *lpValuePtr == true ? 1.0f : 0.0f;
	}

double
VirtualRowSet::GetBoolAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return *lpValuePtr == true ? 1.0 : 0.0;
	}

bool
VirtualRowSet::SetBoolAsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= bValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

bool
VirtualRowSet::SetBoolAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (sValue == _T("F") || sValue == _T("False")) ? false : true; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

bool
VirtualRowSet::SetBoolAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= cValue == '\0' ? false : true; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

bool
VirtualRowSet::SetBoolAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= shValue == 0 ? false : true; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

bool
VirtualRowSet::SetBoolAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= nValue == 0 ? false : true; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

bool
VirtualRowSet::SetBoolAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= nValue == 0 ? false : true; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

bool
VirtualRowSet::SetBoolAsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= fValue == 0.0f ? false : true; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

bool
VirtualRowSet::SetBoolAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBool);

	bool*				lpValuePtr			= (bool*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= dValue == 0.0 ? false : true; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTBool];
	return true;
	}

// Int8
char
VirtualRowSet::GetInt8AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return *lpValuePtr;
	}

short
VirtualRowSet::GetInt8AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return ((short)*lpValuePtr)&0x00FF;
	}

int
VirtualRowSet::GetInt8AsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return ((int)*lpValuePtr)&0x000000FF;
	}

__int64
VirtualRowSet::GetInt8AsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return ((__int64)*lpValuePtr)&0x00000000000000FF;
	}

bool
VirtualRowSet::GetInt8AsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;
	return *lpValuePtr == 0 ? false : true;
	}

_String
VirtualRowSet::GetInt8AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");
	_String s;
	s.Format("%d", *lpValuePtr);
	return s;
	}

float
VirtualRowSet::GetInt8AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return (float)*lpValuePtr;
	}

double
VirtualRowSet::GetInt8AsDouble	(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return (double)*lpValuePtr;
	}

bool
VirtualRowSet::SetInt8AsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= bValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

bool
VirtualRowSet::SetInt8AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (char)_tstoi(sValue.GetBuffer()); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

bool
VirtualRowSet::SetInt8AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	*lpValuePtr					= cValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

bool
VirtualRowSet::SetInt8AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (char)(shValue & 0x00FF); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

bool
VirtualRowSet::SetInt8AsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (char)(nValue & 0x000000FF); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

bool
VirtualRowSet::SetInt8AsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (char)(nValue & 0x00000000000000FF); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

bool
VirtualRowSet::SetInt8AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (char)(fValue); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

bool
VirtualRowSet::SetInt8AsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt8);

	char*				lpValuePtr			= (char*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (char)(dValue); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt8];
	return true;
	}

// Int16
char
VirtualRowSet::GetInt16AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return (char)*lpValuePtr;
	}

short
VirtualRowSet::GetInt16AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return *lpValuePtr;
	}

int
VirtualRowSet::GetInt16AsInt32	(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return ((int)*lpValuePtr)&0x0000FFFF;
	}

__int64		
VirtualRowSet::GetInt16AsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return ((__int64)*lpValuePtr)&0x000000000000FFFF;
	}

bool			
VirtualRowSet::GetInt16AsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;
	return *lpValuePtr == 0 ? false : true;
	}

_String		
VirtualRowSet::GetInt16AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");
	_String s;
	s.Format("%d", *lpValuePtr);
	return s;
	}

float		
VirtualRowSet::GetInt16AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return (float)*lpValuePtr;
	}

double		
VirtualRowSet::GetInt16AsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return (double)*lpValuePtr;
	}

bool
VirtualRowSet::SetInt16AsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (short)(0xFF &((short)bValue)); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

bool
VirtualRowSet::SetInt16AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (char)_tstoi(sValue.GetBuffer()); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

bool
VirtualRowSet::SetInt16AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (short)(0xFF &((short)cValue)); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

bool
VirtualRowSet::SetInt16AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= shValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

bool
VirtualRowSet::SetInt16AsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (short)nValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

bool
VirtualRowSet::SetInt16AsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (short)nValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

bool
VirtualRowSet::SetInt16AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (short)fValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

bool
VirtualRowSet::SetInt16AsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt16);

	short*				lpValuePtr			= (short*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (short)dValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt16];
	return true;
	}

// Int32
char			
VirtualRowSet::GetInt32AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return (char)(*lpValuePtr);
	}

short
VirtualRowSet::GetInt32AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (short)(*lpValuePtr);
	}

int			
VirtualRowSet::GetInt32AsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return *lpValuePtr;
	}

__int64		
VirtualRowSet::GetInt32AsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return ((__int64)*lpValuePtr) & 0x00000000FFFFFFFF;
	}

bool			
VirtualRowSet::GetInt32AsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;
	return *lpValuePtr == 0 ? false : true;
	}

_String		
VirtualRowSet::GetInt32AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("0");
	_String s;
	s.Format(_T("%d"), *lpValuePtr);
	return s;
	}

float		
VirtualRowSet::GetInt32AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return (float)*lpValuePtr;
	}

double		
VirtualRowSet::GetInt32AsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return (double)*lpValuePtr;
	}

bool
VirtualRowSet::SetInt32AsBool	(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)(0xFF&((int)bValue)); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

bool
VirtualRowSet::SetInt32AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)_tstoi(sValue.GetBuffer()); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

bool
VirtualRowSet::SetInt32AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)(0xFF&((int)cValue)); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

bool
VirtualRowSet::SetInt32AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)(0xFFFF&((int)shValue)); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

bool
VirtualRowSet::SetInt32AsInt32	(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*				lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)(nValue); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

bool
VirtualRowSet::SetInt32AsInt64	(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)(nValue); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

bool
VirtualRowSet::SetInt32AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)(fValue); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

bool
VirtualRowSet::SetInt32AsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt32);

	int*	lpValuePtr			= (int*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (int)(dValue); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt32];
	return true;
	}

// Int64
char			
VirtualRowSet::GetInt64AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return (char)(*lpValuePtr);
	}

short		
VirtualRowSet::GetInt64AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (short)(*lpValuePtr);
	}

int			
VirtualRowSet::GetInt64AsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return ((__int64)*lpValuePtr)&0x00000000FFFFFFFF;
	}

__int64		
VirtualRowSet::GetInt64AsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return *lpValuePtr;
	}

bool			
VirtualRowSet::GetInt64AsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;
	return *lpValuePtr == 0 ? false : true;
	}

_String		
VirtualRowSet::GetInt64AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*			lpValuePtr		= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");
	_String s;
	s.Format("%d", *lpValuePtr);
	return s;
	}

float		
VirtualRowSet::GetInt64AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return (float)*lpValuePtr;
	}

double		
VirtualRowSet::GetInt64AsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return (float)*lpValuePtr;
	}

bool			
VirtualRowSet::SetInt64AsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)(0xFF&((__int64)bValue)); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

bool			
VirtualRowSet::SetInt64AsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)_tstoi64(sValue.GetBuffer()); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

bool			
VirtualRowSet::SetInt64AsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)(0xFF&((__int64)cValue)); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

bool			
VirtualRowSet::SetInt64AsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)(0xFF&((__int64)shValue)); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

bool			
VirtualRowSet::SetInt64AsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)(0xFF&((__int64)nValue)); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

bool			
VirtualRowSet::SetInt64AsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)(nValue); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

bool			
VirtualRowSet::SetInt64AsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)(fValue); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

bool			
VirtualRowSet::SetInt64AsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTInt64);

	__int64*				lpValuePtr			= (__int64*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr						= (__int64)(dValue); 
	*lpValueLengthPtr				= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTInt64];
	return true;
	}

// Float
char			
VirtualRowSet::GetFloatAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*	lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return (char)(*lpValuePtr);
	}

short		
VirtualRowSet::GetFloatAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (short)(*lpValuePtr);
	}

int			
VirtualRowSet::GetFloatAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (int)(*lpValuePtr);
	}

__int64		
VirtualRowSet::GetFloatAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (__int64)(*lpValuePtr);
	}

bool			
VirtualRowSet::GetFloatAsBool	(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;
	return *lpValuePtr == 0.0f ? false : true;
	}

_String		
VirtualRowSet::GetFloatAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");
	_String s;
	s.Format("%f", *lpValuePtr);
	return s;
	}

float		
VirtualRowSet::GetFloatAsFloat	(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return *lpValuePtr;
	}

double		
VirtualRowSet::GetFloatAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return (double)(*lpValuePtr);
	}

bool
VirtualRowSet::SetFloatAsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= bValue ? 1.0f : 0.0f; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

bool
VirtualRowSet::SetFloatAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (float)_tstof(sValue.GetBuffer()); 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

bool			
VirtualRowSet::SetFloatAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (float)cValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

bool			
VirtualRowSet::SetFloatAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (float)shValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

bool			
VirtualRowSet::SetFloatAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*	lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (float)nValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

bool			
VirtualRowSet::SetFloatAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (float)nValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

bool			
VirtualRowSet::SetFloatAsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*	lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (float)fValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

bool			
VirtualRowSet::SetFloatAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTFloat);

	float*				lpValuePtr			= (float*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (float)dValue; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTFloat];
	return true;
	}

// Double
char			
VirtualRowSet::GetDoubleAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return (char)(*lpValuePtr);
	}

short		
VirtualRowSet::GetDoubleAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (short)(*lpValuePtr);
	}

int			
VirtualRowSet::GetDoubleAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (int)(*lpValuePtr);
	}

__int64		
VirtualRowSet::GetDoubleAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (__int64)(*lpValuePtr);
	}

bool			
VirtualRowSet::GetDoubleAsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;
	return *lpValuePtr == 0.0 ? false : true;
	}

_String		
VirtualRowSet::GetDoubleAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");
	_String s;
	s.Format("%f", *lpValuePtr);
	return s;
	}

float		
VirtualRowSet::GetDoubleAsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return (float)*lpValuePtr;
	}

double		
VirtualRowSet::GetDoubleAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return *lpValuePtr;
	}

DateTime	
VirtualRowSet::GetDoubleAsDate(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	DateTime dtTime;
	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ){ // NULL
		dtTime.SetStatusNull();
		return dtTime;
		}
	dtTime = (time_t)*lpValuePtr;
	return dtTime;
	}

bool
VirtualRowSet::SetDoubleAsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= bValue ? 1.0 : 0.0; 
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool			
VirtualRowSet::SetDoubleAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (double)_tstof(sValue.GetBuffer());  
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool			
VirtualRowSet::SetDoubleAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (double)(cValue);  
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool			
VirtualRowSet::SetDoubleAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (double)(shValue);  
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool			
VirtualRowSet::SetDoubleAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*	lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (double)(nValue);  
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool		
VirtualRowSet::SetDoubleAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (double)(nValue);  
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool		
VirtualRowSet::SetDoubleAsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (double)(fValue);  
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool		
VirtualRowSet::SetDoubleAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	*lpValuePtr					= (double)(dValue);  
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
	return true;
	}

bool		
VirtualRowSet::SetDoubleAsDate(VRowSetFieldInfo* pInfo, VRowHeader* pRow, DateTime& dtValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDouble);

	double*				lpValuePtr			= (double*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( dtValue.GetStatus() == DateTime::Status::valid ){
		*lpValuePtr				= (double)((time_t)dtValue);  
		*lpValueLengthPtr		= (FIELD_LENGTH_TYPE)_field_default_size_by_type[FTDouble];
		}
	else
	if( dtValue.GetStatus() == DateTime::Status::null ){
		*lpValueLengthPtr = -1; // Set null
		}
	else
		return false;
	return true;
	}

// String
char			
VirtualRowSet::GetStringAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return (char)_tstoi(lpValuePtr);
#else
	ASSERT(pInfo->type == FTChar);
	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return '\0';
	return (char)_tstoi(lpValuePtr);
#endif
	}

short		
VirtualRowSet::GetStringAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (short)_tstoi(lpValuePtr);
#else
	ASSERT(pInfo->type == FTChar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (short)_tstoi(lpValuePtr);
#endif
	}

int			
VirtualRowSet::GetStringAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (int)_tstoi(lpValuePtr);
#else
	ASSERT(pInfo->type == FTChar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return (int)_tstoi(lpValuePtr);
#endif
	}

__int64		
VirtualRowSet::GetStringAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return _tstoi64(lpValuePtr);
#else
	ASSERT(pInfo->type == FTChar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0;
	return _tstoi64(lpValuePtr);
#endif
	}

bool			
VirtualRowSet::GetStringAsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;

	if( _String(lpValuePtr) == _T("False") || _String(lpValuePtr) == _T("F") )
		return false;
	else
		return true;
#else
	ASSERT(pInfo->type == FTChar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return false;

	if( _String(lpValuePtr) == _T("False") || _String(lpValuePtr) == _T("F") )
		return false;
	else
		return true;
#endif
	}

_String		
VirtualRowSet::GetStringAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
#else
	ASSERT(pInfo->type == FTChar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");
	return _String(lpValuePtr);
	}

float		
VirtualRowSet::GetStringAsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return (float)_tstof(lpValuePtr);
#else
	ASSERT(pInfo->type == FTChar);
	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0f;
	return (float)_tstof(lpValuePtr);
#endif
	}

double		
VirtualRowSet::GetStringAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return _tstof(lpValuePtr);
#else
	ASSERT(pInfo->type == FTChar);
	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;
	return _tstof(lpValuePtr);
#endif
	}

DateTime	
VirtualRowSet::GetStringAsDate(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

#ifdef _UNICODE
	ASSERT(pInfo->type == FTWchar);
	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ){ // NULL
		dt.SetStatusNull();
		return dt;
		}

	dt.ParseDate(lpValuePtr);
	return dt;
#else
	ASSERT(pInfo->type == FTChar);
	DateTime				dt;
	TCHAR*					lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*		lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ){ // NULL
		dt.SetStatusNull(); // null date time
		return dt;
		}

	dt.ParseDate(lpValuePtr);
	return dt;
#endif
	}

bool			
VirtualRowSet::SetStringAsBool(VRowSetFieldInfo* pInfo, VRowHeader* pRow, bool bValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*	lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	_String sValue				= bValue ? _T("True") : _T("False");
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr			= (sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type];
	return true;
	}

bool			
VirtualRowSet::SetStringAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( sValue.GetLength() < ( pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

bool			
VirtualRowSet::SetStringAsInt8(VRowSetFieldInfo* pInfo, VRowHeader* pRow, char cValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	_String sValue;
	sValue.Format(_T("%d"), cValue);
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

bool			
VirtualRowSet::SetStringAsInt16(VRowSetFieldInfo* pInfo, VRowHeader* pRow, short shValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	_String sValue;
	sValue.Format(_T("%d"), shValue);
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

bool			
VirtualRowSet::SetStringAsInt32(VRowSetFieldInfo* pInfo, VRowHeader* pRow, int nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	_String sValue;
	sValue.Format(_T("%d"), nValue);
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

bool			
VirtualRowSet::SetStringAsInt64(VRowSetFieldInfo* pInfo, VRowHeader* pRow, __int64 nValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	_String sValue;
	sValue.Format(_T("%d"), nValue);
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

bool			
VirtualRowSet::SetStringAsFloat(VRowSetFieldInfo* pInfo, VRowHeader* pRow, float fValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	_String sValue;
	sValue.Format(_T("%.f"), fValue);
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

bool			
VirtualRowSet::SetStringAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	_String sValue;
	sValue.Format(_T("%.d"), dValue);
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr			= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

bool			
VirtualRowSet::SetStringAsDate(VRowSetFieldInfo* pInfo, VRowHeader* pRow, DateTime& dtValue){
	ASSERT(pInfo);
	ASSERT(pRow);
#ifndef _UNICODE
	ASSERT(pInfo->type == FTChar);
#else
	ASSERT(pInfo->type == FTWchar);
#endif

	if( dtValue.GetStatus() == DateTime::Status::invalid )
		return false;

	TCHAR*				lpValuePtr			= (TCHAR*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( dtValue.GetStatus() == DateTime::Status::null ){
		*lpValueLengthPtr = -1; // Set null.
		return true;
		}

	_String sValue;
	sValue.Format(_T("%02d/%02d/%04d %02d:%02d"), dtValue.GetMonth(), dtValue.GetDay(), 
				  dtValue.GetYear(), dtValue.GetHour(), dtValue.GetMinute());
	if( sValue.GetLength() < (pInfo->nSize / _field_default_size_by_type[pInfo->type]) ){
		// Set field value.
		_tcscpy_s(lpValuePtr, pInfo->nSize*_field_default_size_by_type[pInfo->type], sValue.GetBuffer());
		}
	else
		return false;
	*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)((sValue.GetLength() + 1)*_field_default_size_by_type[pInfo->type]);
	return true;
	}

// Date
_String		
VirtualRowSet::GetDateAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);

	ASSERT(pInfo->type == FTDate);
	TIMESTAMP_STRUCT*				lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*				lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");

	_String s;
	s.Format(_T("%02d/%02d/%04d"), lpValuePtr->month, lpValuePtr->day, lpValuePtr->year);
	return s;
	}

double		
VirtualRowSet::GetDateAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDate);
	TIMESTAMP_STRUCT*				lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*				lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;

	DateTime dtTime;
	dtTime.SetDate(lpValuePtr->year, lpValuePtr->month, lpValuePtr->day);
	if( dtTime.GetStatus() == DateTime::Status::null ) // null
		return 0.0;
	return (double)((time_t)dtTime);
	}

DateTime	
VirtualRowSet::GetDateAsDate(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDate);
	DateTime					dtTime;
	TIMESTAMP_STRUCT*			lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*			lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ){ // NULL
		dtTime.SetStatusNull(); // null date time
		return dtTime;
		}

	dtTime.SetDate(lpValuePtr->year, lpValuePtr->month, lpValuePtr->day);
	return dtTime;
	}

bool			
VirtualRowSet::SetDateAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDate);
	DateTime			dtTime;
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	
	if( dtTime.ParseDate(sValue) ){
		lpValuePtr->day		= dtTime.GetDay();
		lpValuePtr->month	= dtTime.GetMonth();
		lpValuePtr->year	= dtTime.GetYear();
		//lpValuePtr->hour	= dtTime.GetHour();
		//lpValuePtr->minute= dtTime.GetMinute();
		//lpValuePtr->second= dtTime.GetSecond();
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)_field_default_size_by_type[pInfo->type];
		return true;
		}
	return false;
	}

bool			
VirtualRowSet::SetDateAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDate);
	DateTime			dtTime;
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	dtTime									= (time_t)dValue;

	if( dtTime.GetStatus() == DateTime::Status::valid ){
		lpValuePtr->day		= dtTime.GetDay();
		lpValuePtr->month	= dtTime.GetMonth();
		lpValuePtr->year	= dtTime.GetYear();
		//lpValuePtr->hour	= dtTime.GetHour();
		//lpValuePtr->minute	= dtTime.GetMinute();
		//lpValuePtr->second	= dtTime.GetSecond();
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)_field_default_size_by_type[pInfo->type];
		return true;
		}
	return false;
	}

bool			
VirtualRowSet::SetDateAsDate(VRowSetFieldInfo* pInfo, VRowHeader* pRow, DateTime& dtValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDate);
	DateTime			dtTime;
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( dtValue.GetStatus() == DateTime::Status::valid ){
		lpValuePtr->day		= dtTime.GetDay();
		lpValuePtr->month	= dtTime.GetMonth();
		lpValuePtr->year	= dtTime.GetYear();
		//lpValuePtr->hour	= dtTime.GetHour();
		//lpValuePtr->minute	= dtTime.GetMinute();
		//lpValuePtr->second	= dtTime.GetSecond();
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)_field_default_size_by_type[pInfo->type];
		return true;
		}
	else
	if( dtValue.GetStatus() == DateTime::Status::null ){
		*lpValueLengthPtr	= -1; // Set null.
		return true;
		}
	return false;
	}

// DateTime
_String		
VirtualRowSet::GetDateTimeAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDateTime);
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return _T("");

	_String s;
	s.Format(_T("%02d/%02d/%04d %02d:%02d:%02d"), 
		lpValuePtr->month, lpValuePtr->day, lpValuePtr->year, lpValuePtr->hour, lpValuePtr->minute, lpValuePtr->second);
	return s;
	}

double		
VirtualRowSet::GetDateTimeAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDateTime);
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return 0.0;

	DateTime dtTime;
	dtTime.SetDateTime	(lpValuePtr->year, lpValuePtr->month, lpValuePtr->day, 
						lpValuePtr->hour, lpValuePtr->minute, lpValuePtr->second);
	return (double)((time_t)dtTime);
	}

DateTime	
VirtualRowSet::GetDateTimeAsDate(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDateTime);
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	if( *lpValueLengthPtr == -1 ) // NULL
		return (time_t)0;

#ifndef AFX_RFX_INT_PSEUDO_NULL
	#define AFX_RFX_INT_PSEUDO_NULL 0xFFFF
#endif

	if( lpValuePtr->year == AFX_RFX_INT_PSEUDO_NULL )
		lpValuePtr->year = 0;
	if( lpValuePtr->month == AFX_RFX_INT_PSEUDO_NULL )
		lpValuePtr->month = 0;
	if( lpValuePtr->day == AFX_RFX_INT_PSEUDO_NULL )
		lpValuePtr->day = 0;
	if( lpValuePtr->hour == AFX_RFX_INT_PSEUDO_NULL )
		lpValuePtr->hour = 0;
	if( lpValuePtr->minute == AFX_RFX_INT_PSEUDO_NULL )
		lpValuePtr->minute = 0;
	if( lpValuePtr->second == AFX_RFX_INT_PSEUDO_NULL )
		lpValuePtr->second = 0;

	DateTime dtTime;
	dtTime.SetDateTime	(lpValuePtr->year, lpValuePtr->month, lpValuePtr->day, 
						lpValuePtr->hour, lpValuePtr->minute, lpValuePtr->second);
	return dtTime;
	}

bool
VirtualRowSet::SetDateTimeAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDateTime);
	DateTime			dtTime;
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	
	if( dtTime.ParseDateTime(sValue) ){
		lpValuePtr->day		= dtTime.GetDay();
		lpValuePtr->month	= dtTime.GetMonth();
		lpValuePtr->year	= dtTime.GetYear();
		lpValuePtr->hour	= dtTime.GetHour();
		lpValuePtr->minute	= dtTime.GetMinute();
		lpValuePtr->second	= dtTime.GetSecond();
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)_field_default_size_by_type[pInfo->type];
		return true;
		}
	return false;
	}

bool
VirtualRowSet::SetDateTimeAsDouble(VRowSetFieldInfo* pInfo, VRowHeader* pRow, double dValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDateTime);
	DateTime		dtTime;
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	dtTime									= (time_t)dValue;

	if( dtTime.GetStatus() == DateTime::Status::valid ){
		lpValuePtr->day		= dtTime.GetDay();
		lpValuePtr->month	= dtTime.GetMonth();
		lpValuePtr->year	= dtTime.GetYear();
		lpValuePtr->hour	= dtTime.GetHour();
		lpValuePtr->minute	= dtTime.GetMinute();
		lpValuePtr->second	= dtTime.GetSecond();
		lpValuePtr->fraction= 0;
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)_field_default_size_by_type[pInfo->type];
		return true;
		}
	return false;
	}

bool
VirtualRowSet::SetDateTimeAsDate	(VRowSetFieldInfo* pInfo, VRowHeader* pRow, DateTime& dtValue){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTDateTime);
	TIMESTAMP_STRUCT*	lpValuePtr			= (TIMESTAMP_STRUCT*)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( dtValue.GetStatus() == DateTime::Status::valid ){
		lpValuePtr->day		= dtValue.GetDay();
		lpValuePtr->month	= dtValue.GetMonth();
		lpValuePtr->year	= dtValue.GetYear();
		lpValuePtr->hour	= dtValue.GetHour();
		lpValuePtr->minute	= dtValue.GetMinute();
		lpValuePtr->second	= dtValue.GetSecond();
		lpValuePtr->fraction= 0;
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)_field_default_size_by_type[pInfo->type];
		return true;
		}
	else
	if( dtValue.GetStatus() == DateTime::Status::null ){
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)-1; // Set null.
		return true;
		}
	return false;
	}

// Binary
_String		
VirtualRowSet::GetBinaryAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBinary);
	return _T("Binary");
	}

BinaryData	
VirtualRowSet::GetBinaryAsBinary(VRowSetFieldInfo* pInfo, VRowHeader* pRow){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBinary);

	void**		lpValuePtr			= (void**)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));
	BinaryData	binary				= {NULL, 0};
	if( *lpValueLengthPtr == -1 ) // NULL
		return binary;

	binary.lpBinary = *lpValuePtr;
	binary.length	= (int)*lpValueLengthPtr;
	return binary;
	}

bool
VirtualRowSet::SetBinaryAsString(VRowSetFieldInfo* pInfo, VRowHeader* pRow, _String sValue, BOOL){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBinary);

	void**				lpValuePtr			= (void**)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr > 0 ){ // Free allocated memory.
		delete lpValuePtr;
		*lpValuePtr = NULL;
		}

	if( sValue.GetLength() == 0 ){
		*lpValueLengthPtr	= -1; // Set null.
		return true;
		}

	int nLenBytes		= (sValue.GetLength()+1)*sizeof(TCHAR);
	*lpValuePtr			= new BYTE[nLenBytes];
	*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)nLenBytes;
	// Copy string as binary.
	memcpy(*lpValuePtr, sValue.GetBuffer(), nLenBytes);
	return true;
	}

bool
VirtualRowSet::SetBinaryAsBinary(VRowSetFieldInfo* pInfo, VRowHeader* pRow, BinaryData& binValue, BOOL bCopyBinary /*= FALSE*/){
	ASSERT(pInfo);
	ASSERT(pRow);
	ASSERT(pInfo->type == FTBinary);

	void**				lpValuePtr			= (void**)(((__int64)pRow) + ((__int64)pInfo->dwValueOffset));
	FIELD_LENGTH_TYPE*	lpValueLengthPtr	= (FIELD_LENGTH_TYPE*)(((__int64)pRow) + ((__int64)pInfo->dwValueLengthOffset));

	if( *lpValueLengthPtr > 0 ){ // Free allocated memory.
		if( *lpValuePtr )
			delete *lpValuePtr;
		*lpValuePtr			= NULL;
		*lpValueLengthPtr	= 0;
		}

	if( binValue.length <= 0 ){
		*lpValueLengthPtr	= -1; // Set null.
		return true;
		}

	int nLenBytes = binValue.length;
	if( bCopyBinary ){
		*lpValuePtr			= new BYTE[nLenBytes];
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)nLenBytes;
		// Copy bytes.
		memcpy(*lpValuePtr, binValue.lpBinary, nLenBytes);
		}
	else{
		*lpValuePtr			= binValue.lpBinary;
		*lpValueLengthPtr	= (FIELD_LENGTH_TYPE)nLenBytes;
		}
	return true;
	}