#include "stdafx.h"
#include "StringToPtrArray.h"
#include "..\String\StringDecoder.h"

StringToPtrArray::StringToPtrArray(){
	m_nCurrent = -1;
	}

StringToPtrArray::~StringToPtrArray(){
	DeleteAll();
	}

void*	
StringToPtrArray::operator[](const CString sKey){
	return GetValue(sKey);
	}

BOOL
StringToPtrArray::Add(const CString sKey, void* lpValue){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	if( m_arrKeys.FindValue(nCode) != -1 ){
		return FALSE; // Key exists.
		}

	StringToPtrItem* pItemNew = new StringToPtrItem();
	pItemNew->nKeyCode	= nCode;
	pItemNew->sKey		= sKey;
	pItemNew->lpValue	= lpValue;
	pItemNew->next		= NULL;
	pItemNew->prev		= NULL;

	m_listItems.Add(pItemNew);
	m_arrKeys.Add(nCode, pItemNew);
	return TRUE;
	}

BOOL
StringToPtrArray::SetValue(const CString sKey, void* lpValue){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	int nIndex = m_arrKeys.FindValue(nCode);
	if( nIndex == -1 ){
		return FALSE; // Key not exists.
		}

	StringToPtrItem* pItem = m_arrKeys.GetValueByIndex(nIndex);
	ASSERT(pItem);
	pItem->lpValue = lpValue;
	return TRUE;
	}

BOOL
StringToPtrArray::SetValueByIndex(int nIndex, void* lpValue){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToPtrItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	pItem->lpValue = lpValue;
	return TRUE;
	}

BOOL
StringToPtrArray::SetKeyByIndex(int nIndex, const CString sKey){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCodeNew = DecodeString(sKeyLower.GetBuffer());

	StringToPtrItem* pItem = m_listItems.GetAt(nIndex);
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
StringToPtrArray::Delete(const CString sKey, bool bDestroyPtrObject /*= false*/){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		// Destroy pointer object. {{
		PStringToPtrItem pPtr = m_arrKeys.GetData(nIndexBy);
		if( bDestroyPtrObject && pPtr ){
			void* lpPtr = pPtr->lpValue;
			if( lpPtr ){
				delete lpPtr;
				}
			}
		// }}

		// Remove from list.
		m_listItems.Remove(pPtr);
		// Remove from sorted array.
		m_arrKeys.Delete(nIndexBy);
		return TRUE;
		}
	return FALSE;
	}

BOOL
StringToPtrArray::DeleteByIndex(int nIndex, bool bDestroyPtrObject /*= false*/){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToPtrItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);

	int nIndexBy = m_arrKeys.FindValue(pItem->nKeyCode);
	if( nIndexBy != -1 ){ 
		// Destroy pointer object. {{
		PStringToPtrItem pPtr = m_arrKeys.GetData(nIndexBy);
		if( bDestroyPtrObject && pPtr ){
			void* lpPtr = pPtr->lpValue;
			if( lpPtr ){
				delete lpPtr;
				}
			}
		// }}

		// Remove from list.
		m_listItems.RemoveAt(nIndex);
		// Remove from sorted array.
		m_arrKeys.Delete(nIndexBy);
		return TRUE;
		}

	return FALSE;
	}

void
StringToPtrArray::DeleteAll(bool bDestroyPtrObject /*= false*/){
	if( bDestroyPtrObject ){
		int nLoop	= 0;
		int nCt		= m_listItems.GetCount();
		while( nLoop < nCt ){
			PStringToPtrItem pPtr = m_listItems[nLoop];
			if( pPtr ){
				CObject* lpPtr = (CObject*)pPtr->lpValue;
				if( lpPtr ){
					delete lpPtr;
					}
				}

			nLoop ++;
			}
		}
	// Remove from list.
	m_listItems.RemoveAll(true);
	// Remove from sorted array.
	m_arrKeys.DeleteAll();
	m_nCurrent = -1;
	}
	
CString 
StringToPtrArray::GetKey(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return _T("");

	StringToPtrItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->sKey;
	}

void*
StringToPtrArray::GetValueByIndex(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return NULL;

	StringToPtrItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->lpValue;
	}

void*
StringToPtrArray::GetValue(const CString sKey){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		StringToPtrItem* pItem = m_arrKeys.GetData(nIndexBy);
		ASSERT(pItem);
		return pItem->lpValue;
		}

	return NULL;
	}

bool
StringToPtrArray::KeyExists(const CString sKey, int* pKeyIndex /*= NULL*/){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		if( pKeyIndex != NULL )
			*pKeyIndex = m_listItems.Find((StringToPtrItem*)m_arrKeys.GetData(nIndexBy));
//		if( pKeyIndex != NULL )
//			*pKeyIndex = nIndexBy;
		return true;
		}
	return false;
	}

bool
StringToPtrArray::SetCurrentIndex(int nIndex){
	if( nIndex >= 0 && nIndex < GetCount() ){
		m_nCurrent = nIndex;
		return true;
		}
	return false;
	}

int
StringToPtrArray::GetCurrentIndex(){
	if( !GetCount() ) return -1;
	if( m_nCurrent < 0 || m_nCurrent >= GetCount() )
		m_nCurrent = 0;
	return m_nCurrent;
	}

void*
StringToPtrArray::GetCurrentValue(){
	int nIndex = GetCurrentIndex();
	if( nIndex > -1 )
		return GetValueByIndex(nIndex);
	return NULL;
	}

CString
StringToPtrArray::GetCurrentKey(){
	int nIndex = GetCurrentIndex();
	if( nIndex > -1 )
		return GetKey(nIndex);		
	return _T("");
	}