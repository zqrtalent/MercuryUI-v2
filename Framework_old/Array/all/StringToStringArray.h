#pragma once

#include "List.h"
#include "AutoSortedArrayTempl.h"

typedef struct StringToStringItem_tag{
	CString					sKey;
	CString					sKeyOrig;
	CString					sValue;
	UINT					nKeyCode; // Decoded key string.
	StringToStringItem_tag*	next;
	StringToStringItem_tag*	prev;
} StringToStringItem, *PStringToStringItem;


class StringToStringArray
{
public:
	StringToStringArray();
	~StringToStringArray();

public:
	CString	operator[]		(const CString sKey);
	BOOL	Add				(const CString sKey, const CString sValue);
	BOOL	SetValue		(const CString sKey, const CString sValue);
	BOOL	SetValueByIndex	(int nIndex, const CString sValue);
	BOOL	SetKeyByIndex	(int nIndex, const CString sKey);
	BOOL	Delete			(const CString sKey);
	BOOL	DeleteByIndex	(int nIndex);
	void	DeleteAll		();
	int		GetCount		(){return m_arrKeys.GetCount();}		

	bool	KeyExists		(const CString sKey, int* pKeyIndex = NULL);
	CString GetKey			(int nIndex);
	CString GetValueByIndex	(int nIndex);
	CString GetValue		(const CString sKey);

protected:
	List<StringToStringItem>						m_listItems;
	AutoSortedArrayTempl<UINT, StringToStringItem*> m_arrKeys;
};
/*
// ===================================================================
// Template class StringToObjectArray
// ===================================================================

template <class OBJECT> class StringToObjectArray
{
public:
	StringToStringArray();
	~StringToStringArray();

typedef struct StringToObjectItem_tag{
	CString					sKey;
	OBJECT					value;
	UINT					nKeyCode; // Decoded key string.
	StringToStringItem_tag*	next;
	StringToStringItem_tag*	prev;
} StringToObjectItem, *PStringToObjectItem;

public:
	OBJECT	operator[]		(const CString sKey);
	BOOL	Add				(const CString sKey, const OBJECT value);
	BOOL	SetValue		(const CString sKey, const OBJECT value);
	BOOL	SetValueByIndex	(int nIndex, const OBJECT value);
	BOOL	SetKeyByIndex	(int nIndex, const CString sKey);
	BOOL	Delete			(const CString sKey);
	BOOL	DeleteByIndex	(int nIndex);
	void	DeleteAll		();
		
	CString GetKey			(int nIndex);
	OBJECT	GetValueByIndex	(int nIndex);
	OBJECT	GetValue		(const CString sKey);

protected:
	ChainTempl<StringToObjectItem>					m_listItems;
	AutoSortedArrayTempl<UINT, StringToObjectItem*> m_arrKeys;
};

template <class OBJECT>
StringToStringArray<OBJECT>::StringToStringArray(){
	}

template <class OBJECT>
StringToStringArray::~StringToStringArray(){
	DeleteAll();
	}

template <class OBJECT>
CString	
StringToStringArray::operator[](const CString sKey){
	return GetValue(sKey);
	}

template <class OBJECT>
BOOL
StringToStringArray::Add(const CString sKey, const OBJECT value){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	if( m_arrKeys.FindValue(nCode) != -1 ){
		return FALSE; // Key exists.
		}

	StringToObjectItem* pItemNew = new StringToObjectItem();
	pItemNew->nKeyCode	= nCode;
	pItemNew->sKey		= sKeyLower;
	pItemNew->value		= value;
	pItemNew->next		= NULL;
	pItemNew->prev		= NULL;

	m_arrKeys.Add(nCode, pItemNew);
	return TRUE;
	}

template <class OBJECT>
BOOL
StringToStringArray::SetValue(const CString sKey, const OBJECT value){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();

	UINT nCode = DecodeString(sKeyLower.GetBuffer());
	int nIndex = m_arrKeys.FindValue(nCode);
	if( nIndex == -1 ){
		return FALSE; // Key not exists.
		}

	StringToObjectItem* pItem = m_arrKeys.GetValueByIndex(nIndex);
	ASSERT(pItem);
	pItem->value = value;
	return TRUE;
	}

template <class OBJECT>
BOOL
StringToStringArray::SetValueByIndex(int nIndex, const OBJECT value){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToObjectItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	pItem->value = value;
	return TRUE;
	}

template <class OBJECT>
BOOL
StringToStringArray::SetKeyByIndex(int nIndex, const CString sKey){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCodeNew = DecodeString(sKeyLower.GetBuffer());

	StringToObjectItem* pItem = m_listItems.GetAt(nIndex);
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

template <class OBJECT>
BOOL
StringToStringArray::Delete(const CString sKey){
	CString sKeyLower = sKey;
	sKeyLower.MakeLower();
	UINT nCode = DecodeString(sKeyLower.GetBuffer());

	int nIndexBy = m_arrKeys.FindValue(nCode);
	if( nIndexBy != -1 ){
		// Remove from list.
		m_listItems.RemoveAt(m_arrKeys.GetData(nIndexBy));
		// Remove from sorted array.
		m_arrKeys.Delete(nIndexBy);
		return TRUE;
		}

	return FALSE;
	}

template <class OBJECT>
BOOL
StringToStringArray::DeleteByIndex(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);

	int nIndexBy = m_arrKeys.FindValue(pItem->nKeyCode);
	if( nIndexBy != -1 ){ 
		// Remove from list.
		m_listItems.RemoveAt(m_arrKeys.GetData(nIndexBy));
		// Remove from sorted array.
		m_arrKeys.Delete(nIndexBy);
		return TRUE;
		}

	return FALSE;
	}

template <class OBJECT>
void
StringToStringArray::DeleteAll(){
	// Remove from list.
	m_listItems.DeleteAll();
	// Remove from sorted array.
	m_arrKeys.DeleteAll();
	}
	
template <class OBJECT>
CString 
StringToStringArray::GetKey(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return _T("");

	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->sKey;
	}

template <class OBJECT>
CString
StringToStringArray::GetValueByIndex(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return _T("");

	StringToStringItem* pItem = m_listItems.GetAt(nIndex);
	ASSERT(pItem);
	return pItem->sValue;
	}

template <class OBJECT>
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
	}*/