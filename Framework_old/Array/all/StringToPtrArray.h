#pragma once

#include "List.h"
#include "AutoSortedArrayTempl.h"

typedef struct StringToPtrItem_tag{
	CString					sKey;
	void*					lpValue;
	UINT					nKeyCode; // Decoded key string.
	StringToPtrItem_tag*	next;
	StringToPtrItem_tag*	prev;
} StringToPtrItem, *PStringToPtrItem;


class StringToPtrArray
{
public:
	StringToPtrArray();
	~StringToPtrArray();

public:
	void*	operator[]		(const CString sKey);
	BOOL	Add				(const CString sKey, void* lpValue);
	BOOL	SetValue		(const CString sKey, void* lpValue);
	BOOL	SetValueByIndex	(int nIndex, void* lpValue);
	BOOL	SetKeyByIndex	(int nIndex, const CString sKey);
	BOOL	Delete			(const CString sKey, bool bDestroyPtrObject = false);
	BOOL	DeleteByIndex	(int nIndex, bool bDestroyPtrObject = false);
	void	DeleteAll		(bool bDestroyPtrObject = false);
	int		GetCount		(){return m_arrKeys.GetCount();}		

	bool	KeyExists		(const CString sKey, int* pKeyIndex = NULL);
	CString GetKey			(int nIndex);
	void*	GetValueByIndex	(int nIndex);
	void*	GetValue		(const CString sKey);

	bool	SetCurrentIndex	(int nIndex);
	int		GetCurrentIndex	();
	void*	GetCurrentValue	();
	CString	GetCurrentKey	();

protected:
	List<StringToPtrItem>							m_listItems;
	AutoSortedArrayTempl<UINT, StringToPtrItem*>	m_arrKeys;
	int												m_nCurrent;
};