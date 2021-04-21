#include "StringToIntArray.h"
#include "../String/StringDecoder.h"
#include <locale>

StringToIntArray::StringToIntArray(){
	m_nCurrent = -1;
	}

StringToIntArray::~StringToIntArray(){
	DeleteAll();
	}

int	
StringToIntArray::operator[](const std::string sKey){
	return GetValue(sKey);
	}

BOOL
StringToIntArray::Add(const std::string sKey, int nValue){
	std::string sKeyLower = sKey;
    stringEx::MakeLower(sKeyLower);
	UINT nCode = DecodeString(stringEx::GetBuffer(sKeyLower));
    
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
StringToIntArray::SetValue(const std::string sKey, int nValue){
    std::string sKeyLower = sKey;
    stringEx::MakeLower(sKeyLower);
	UINT nCode = DecodeString(stringEx::GetBuffer(sKeyLower));
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
StringToIntArray::SetKeyByIndex(int nIndex, const std::string sKey){
	if( nIndex < 0 || nIndex >= m_arrKeys.GetCount() )
		return FALSE;

    std::string sKeyLower = sKey;
    stringEx::MakeLower(sKeyLower);
	UINT nCodeNew = DecodeString(stringEx::GetBuffer(sKeyLower));

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
StringToIntArray::Delete(const std::string sKey){
	std::string sKeyLower = sKey;
    stringEx::MakeLower(sKeyLower);
	UINT nCode = DecodeString(stringEx::GetBuffer(sKeyLower));

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
StringToIntArray::KeyExists(const std::string sKey, int* pKeyIndex /*= NULL*/){
    std::string sKeyLower = sKey;
    stringEx::MakeLower(sKeyLower);
	UINT nCode = DecodeString(stringEx::GetBuffer(sKeyLower));

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
	
const std::string
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
StringToIntArray::GetValue(const std::string sKey){
    std::string sKeyLower = sKey;
    stringEx::MakeLower(sKeyLower);
	UINT nCode = DecodeString(stringEx::GetBuffer(sKeyLower));

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

const std::string
StringToIntArray::GetCurrentKey(){
	int nIndex = GetCurrentIndex();
	if( nIndex > -1 )
		return GetKey(nIndex);		
	return _T("");
	}