#pragma once
#include "MercuryGUI.h"
#include "..\Array\AutoSortedArray.h"
#include "..\Data\VRowSet.h"

#define MAX_SUBITEMS_COUNT		200
#define MAX_ITEMS_COUNT			64000

// Items states
#define ITEM_READONLY			1
#define ITEM_SELECTED			2

typedef struct ESHeaderItemTag{
	ESHeaderItemTag*	next;
	ESHeaderItemTag*	prev;
	CString				sField;
	int					nFieldId;
	CString				sText;
	LONG				param;
	int					nImage;
	int					nSelImage;
	DWORD				dwTextAlignH;
	int					nWidth;
	int					nMinWidth;
	DWORD				dwState;
	} ESHeaderItem, *PESHeaderItem;

class ESItems
{
// Construction
public:
	ESItems();
	virtual ~ESItems();

	friend class ESItemsList;

public:

	bool		SetRowSet			(VirtualRowSet* pRowSet);
	DWORD		GetItemState		(int nItem);
	bool		SetItemState		(int nItem, DWORD dwState);
	bool		SetSubItemText		(int nItem, int nSubItem, TCHAR* pszText);

	int			GetItemByPoint		(ESItemsList* pList, CPoint pt);
	bool		GetSubItemRect		(ESItemsList* pList, int nItem, int nSubItem, CRect& rcSubItem);
	bool		GetItemRect			(ESItemsList* pList, int nItem, CRect& rcItem);

	int			ClearSelectedItems	();
	int			GetSelectedItems	(CDWordArray& arrSelItems);

	/*
	return: Retrieves success indicator.
	sText - Sub item text.
	nItem - item index. if -1 means current item.
	nSubItem - Sub item index.
	*/
	bool		GetSubItemText		(CString& sText, int nFieldId = 0, int nItem = -1);

	/*
	Retrieves items count.
	*/
	int			GetItemsCount		(){return (m_pRowSet ? m_pRowSet->GetRowCount() : 0);};

protected:
	VirtualRowSet*		m_pRowSet;
	AutoSortedArray		m_arrSelItems;
};

class ESHeaderItems
{
public:
	ESHeaderItems();
	~ESHeaderItems();

	friend class ESHeaderCtrl;

public:
	BOOL			SetItemCount	(int nCount);
	int				GetItemCount	();
	BOOL			AddItem			(TCHAR* pszText);
	int				InsertItem		(int nInsertAt, TCHAR* pszText);
	int				DeleteItem		(int nItem, int nCount = 1);
	void			DeleteAll		();
	int				GetItemWidth	(int nItem);
	BOOL			SetItemText		(int nItem, TCHAR* pszText);
	BOOL			GetItemText		(int nItem, CString& sItemText);
	int				ItemByPoint		(ESHeaderCtrl* pCtrl, CPoint pt, CRect* pRcItem = NULL);

	void			ReorderItems	(int nItem1, int nItem2);

protected:
	ESHeaderItem*	NewItem			(ESHeaderItem* prev = NULL, ESHeaderItem* next = NULL);
	int				FreeItemsChain	(ESHeaderItem* pItem);
	/* Returns position index. bCreate means auto creation when not exists.*/
	int				GotoItem		(int nItem, BOOL bCreate = TRUE);
	ESHeaderItem*	GetItem			(int nItem);
	BOOL			GetItemRect		(ESHeaderCtrl* pCtrl, int nItem, CRect& rcItem);
	ESHeaderItem*	GetCurrentItem	(){return m_pCurrent;};

protected:
	ESHeaderItem*	m_pRootItem;
	ESHeaderItem*	m_pCurrent;
	int				m_nCurrent;
	int				m_nItemsCount;
	int				m_leftPosX;		// Left position of current item.
};

__inline int
ESHeaderItems::GetItemCount(){
	return m_nItemsCount;
	}

__inline ESHeaderItem*	
ESHeaderItems::NewItem(ESHeaderItem* prev /*= NULL*/, ESHeaderItem* next /*= NULL*/){
	ESHeaderItem* pNew	= new ESHeaderItem;
	pNew->next			= next;
	pNew->prev			= prev;
	pNew->param			= 0L;
	pNew->nImage		= -1;
	pNew->nSelImage		= -1;
	pNew->dwTextAlignH	= TEXT_HALIGN_LEFT;
	pNew->nWidth		= 120;
	pNew->nMinWidth		= 40;
	pNew->dwState		= HDR_ITEM_NORMAL;
	pNew->nFieldId		= -1;

	if( prev ){
		ESHeaderItem* pTemp	= prev->next;
		prev->next			= pNew;
		pNew->next			= pTemp;
		if( pTemp )
			pTemp->prev		= pNew;
		return pNew;
		}

	if( next ){
		ESHeaderItem* pTemp	= next->prev;
		next->prev			= pNew;
		pNew->prev			= pTemp;
		if( pTemp )
			pTemp->next		= pNew;
		}
	return pNew;
	}