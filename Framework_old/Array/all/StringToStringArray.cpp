#include "stdafx.h"
#include "StringToStringArray.h"
#include "..\String\StringDecoder.h"

StringToStringArray::StringToStringArray(){
	}

StringToStringArray::~StringToStringArray(){
	DeleteAll();
	}

CString	
StringToStringArray::operator[](const CString sKey){
	return GetValue(sKey);
	}

BOOL
StringToStringArray::Add(const CString sKey, const CString sValue){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	if( m_arrKeys.FindValue(nCode) != -1 ){
		return FALSE; // Key exists.
		}

	StringToStringItem* pItemNew = new StringToStringItem();
	pItemNew->nKeyCode	= nCode;
	pItemNew->sKeyOrig	= sKey;
	pItemNew->sKey		= sKeyLower;
	pItemNew->sValue	= sValue;
	pItemNew->next		= NULL;
	pItemNew->prev		= NULL;

	m_listItems	.Add(pItemNew);
	m_arrKeys	.Add(nCode, pItemNew);
	return TRUE;
	}

BOOL
StringToStringArray::SetValue(const CString sKey, const CString sValue){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	int nIndex = m_arrKeys.FindValue(nCode);
	if( nIndex == -1 ){
		return FALSE; // Key not exists.
		}

	StringToStringItem* pItem = m_arrKeys.GetValueByIndex(nIndex);
	ASSERT(pItem);
	pItem->sValue = sValue;
	return TRUE;
	}

BOOL
StringToStringArray::SetValueByIndex(int nIndex, const CString sValue){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	pItem->sValue = sValue;
	return TRUE;
	}

BOOL
StringToStringArray::SetKeyByIndex(int nIndex, const CString sKey){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCodeNew = DecodeString(sKeyLower.GetBuffer());

	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	
	int nIndexBy = m_arrKeys.FindValue(pItem->nKeyCode);
	ASSERT(nIndexBy != -1);
	if( nIndexBy != -1 ){
		m_arrKeys.Set(nIndexBy, nCodeNew, pItem);
		}

	pItem->nKeyCode = nCodeNew;
	pItem->sKeyOrig = sKey;
	pItem->sKey = sKeyLower;
	return TRUE;
	}

BOOL
StringToStringArray::Delete(const CString sKey){
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
StringToStringArray::DeleteByIndex(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
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
StringToStringArray::DeleteAll(){
	// Remove from list.
	m_listItems.RemoveAll();
	// Remove from sorted array.
	m_arrKeys.DeleteAll();
	}

bool
StringToStringArray::KeyExists(const CString sKey, int* pKeyIndex /*= NULL*/){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		if( pKeyIndex != NULL )
			*pKeyIndex = m_listItems.Find((StringToStringItem*)m_arrKeys.GetData(nIndexBy));
//		if( pKeyIndex != NULL )
//			*pKeyIndex = nIndexBy;
		return true;
		}
	return false;
	}
	
CString 
StringToStringArray::GetKey(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return _T("");
	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->sKeyOrig;
	}

CString
StringToStringArray::GetValueByIndex(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return _T("");

	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->sValue;
	}

CString
StringToStringArray::GetValue(const CString sKey){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		StringToStringItem* pItem = m_arrKeys.GetData(nIndexBy);
		ASSERT(pItem);
		return pItem->sValue;
		}

	return _T("");
	}