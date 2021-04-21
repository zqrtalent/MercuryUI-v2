#pragma once

#include "List.h"
#include "AutoSortedArrayTempl.h"

typedef struct StringToIntItem_tag{
	CString					sKey;
	int						nValue;
	UINT					nKeyCode; // Decoded key string.
	StringToIntItem_tag*	next;
	StringToIntItem_tag*	prev;
} StringToIntItem, *PStringToIntItem;


class StringToIntArray
{
public:
	StringToIntArray();
	~StringToIntArray();

public:
	int		operator[]				(const CString sKey);
	BOOL	Add						(const CString sKey, int nValue);
	BOOL	SetValue				(const CString sKey, int nValue);
	BOOL	SetValueByIndex			(int nIndex, int nValue);
	BOOL	SetKeyByIndex			(int nIndex, const CString sKey);
	BOOL	Delete					(const CString sKey);
	BOOL	DeleteByIndex			(int nIndex);
	void	DeleteAll				();
	int		GetCount				(){return m_arrKeys.GetCount();}		

	bool	KeyExists				(const CString sKey, int* pKeyIndex = NULL);
	CString GetKey					(int nIndex);
	int		GetValueByIndex			(int nIndex);
	int		GetValue				(const CString sKey);

	bool	SetCurrentIndex			(int nIndex);
	bool	SetCurrentValue			(int nValue);
	int		GetCurrentIndex			();
	int		GetCurrentValue			();
	CString	GetCurrentKey			();

protected:
	List<StringToIntItem>							m_listItems;
	AutoSortedArrayTempl<UINT, StringToIntItem*>	m_arrKeys;
	int												m_nCurrent;
};