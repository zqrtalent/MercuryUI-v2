#include "stdafx.h"
#include "StringToIntArray.h"
#include "..\String\StringDecoder.h"

StringToIntArray::StringToIntArray(){
	m_nCurrent = -1;
	}

StringToIntArray::~StringToIntArray(){
	DeleteAll();
	}

int	
StringToIntArray::operator[](const CString sKey){
	return GetValue(sKey);
	}

BOOL
StringToIntArray::Add(const CString sKey, int nValue){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	if( m_arrKeys.FindValue(nCode) != -1 )
		return FALSE; // Key exists.

	StringToIntItem* pItemNew	= new StringToIntItem();
	pItemNew->nKeyCode			= nCode;
	pItemNew->sKey				= sKey;
	pItemNew->nValue			= nValue;
	pItemNew->next				= NULL;
	pItemNew->prev				= NULL;
	m_arrKeys.Add(nCode, pItemNew);
	m_listItems.Add(pItemNew);
	return TRUE;
	}

BOOL
StringToIntArray::SetValue(const CString sKey, int nValue){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	int nIndex = m_arrKeys.FindValue(nCode);
	if( nIndex == -1 ){
		return FALSE; // Key not exists.
		}

	StringToIntItem* pItem = m_arrKeys.GetValueByIndex(nIndex);
	ASSERT(pItem);
	pItem->nValue = nValue;
	return TRUE;
	}

BOOL
StringToIntArray::SetValueByIndex(int nIndex, int nValue){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToIntItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	pItem->nValue = nValue;
	return TRUE;
	}

BOOL
StringToIntArray::SetKeyByIndex(int nIndex, const CString sKey){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCodeNew = DecodeString(sKeyLower.GetBuffer());

	StringToIntItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	
	int nIndexBy = m_arrKeys.FindValue(pItem->nKeyCode);
	ASSERT(nIndexBy != -1);
	if( nIndexBy != -1 ){
		m_arrKeys.Set(nIndexBy, nCodeNew, pItem);
		}

	pItem->nKeyCode = nCodeNew;
	pItem->sKey = sKeyLower;
	return TRUE;
	}

BOOL
StringToIntArray::Delete(const CString sKey){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		// Remove from list.
		m_listItems.Remove(m_arrKeys.GetData(nIndexBy));
		// Remove from sorted array.
		m_arrKeys.Delete(nIndexBy);
		return TRUE;
		}
	return FALSE;
	}

BOOL
StringToIntArray::DeleteByIndex(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToIntItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);

	int nIndexBy = m_arrKeys.FindValue(pItem->nKeyCode);
	if( nIndexBy != -1 ){ 
		// Remove from list.
		m_listItems.Remove(m_arrKeys.GetData(nIndexBy));
		// Remove from sorted array.
		m_arrKeys.Delete(nIndexBy);
		return TRUE;
		}
	return FALSE;
	}

void
StringToIntArray::DeleteAll(){
	// Remove from list.
	m_listItems.RemoveAll(true);
	// Remove from sorted array.
	m_arrKeys.DeleteAll();
	m_nCurrent = -1;
	}


bool
StringToIntArray::KeyExists(const CString sKey, int* pKeyIndex /*= NULL*/){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		if( pKeyIndex != NULL )
			*pKeyIndex = m_listItems.Find((StringToIntItem*)m_arrKeys.GetData(nIndexBy));
//		if( pKeyIndex != NULL )
//			*pKeyIndex = nIndexBy;
		return true;
		}
	return false;
	}
	
CString 
StringToIntArray::GetKey(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return _T("");
	StringToIntItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->sKey;
	}

int
StringToIntArray::GetValueByIndex(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return 0;
	StringToIntItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->nValue;
	}

int
StringToIntArray::GetValue(const CString sKey){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT	nCode	= DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		StringToIntItem* pItem = m_arrKeys.GetData(nIndexBy);
		ASSERT(pItem);
		return pItem->nValue;
		}
	return 0;
	}

bool
StringToIntArray::SetCurrentIndex(int nIndex){
	if( nIndex >= 0 && nIndex < GetCount() ){
		m_nCurrent = nIndex;
		return true;
		}
	return false;
	}

bool
StringToIntArray::SetCurrentValue(int nValue){
	StringToIntItem* pItem = NULL;
	for(int i=0; i<m_listItems.GetCount(); i++){
		pItem = m_listItems.GetAt(i);
		if( !pItem ) continue;

		if( pItem->nValue == nValue ){
			m_nCurrent = i;
			return true;
			}
		}
	return false;
	}

int
StringToIntArray::GetCurrentIndex(){
	if( !GetCount() ) return -1;
	if( m_nCurrent < 0 || m_nCurrent >= GetCount() )
		m_nCurrent = 0;
	return m_nCurrent;
	}

int
StringToIntArray::GetCurrentValue(){
	int nIndex = GetCurrentIndex();
	if( nIndex > -1 )
		return GetValueByIndex(nIndex);
	return 0;
	}

CString
StringToIntArray::GetCurrentKey(){
	int nIndex = GetCurrentIndex();
	if( nIndex > -1 )
		return GetKey(nIndex);		
	return _T("");
	}