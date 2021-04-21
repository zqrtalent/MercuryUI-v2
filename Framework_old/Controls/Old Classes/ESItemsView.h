#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESItemsView window

#include "MercuryBaseWnd.h"
#include "MercuryScrollBar.h"
#include "ESHeaderCtrl.h"
#include "MercuryTreeGridContent.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

class ESItemsView : public MercuryBaseView
{
// Construction
public:
	ESItemsView();
	virtual ~ESItemsView();

	friend class ESListControl;

public:
	MercuryTreeGridContent* GetContentClass		();
	void					DestroyClassMembers	();

	MerTreeItem*			AddItem				(LPTSTR lpszText, MerTreeItem* parent = NULL);
	LPCTSTR					GetItemText			(MerTreeItem* pItem);
	BOOL					SetItemText			(MerTreeItem* pItem, CString sText);
//	MerTreeItem*			GetSelectedItem		();
	int						AddItemImage		(CString sImageFile);
	BOOL					SetItemImage		(MerTreeItem* pItem, int nImage, int nImageSel, BOOL bInvalidate = TRUE);
	
	MerTreeItem*			GetRootItem			();
	MerTreeItem*			GetFirstVisibleItem	();
	MerTreeItem*			GetNextVisibleItem	(MerTreeItem* pItem);
	MerTreeItem*			GetPrevVisibleItem	(MerTreeItem* pItem);
	int						GetParentsCount		(MerTreeItem* pItem);
	int						GetVisibleChildsCt	(MerTreeItem* pItem);
	int						GetVisChildsCtSetRedraw	(MerTreeItem* pItem);
	CImage*					GetItemImage		(MerTreeItem* pItem);
	CFont*					GetItemFont			(MerTreeItem* pItem);
	COLORREF				GetItemTextColor	(MerTreeItem* pItem, COLORREF& crShadow);
	BOOL					IsItemSelected		(MerTreeItem* pItem);
	BOOL					GetItemRedraw		(MerTreeItem* pItem);
	BOOL					IsItemReadOnly		(MerTreeItem* pItem);
	BOOL					IsItemExpandable	(MerTreeItem* pItem);
	BOOL					IsItemExpanded		(MerTreeItem* pItem);
	BOOL					IsItemParent		(MerTreeItem* pItem);
	BOOL					IsItemChecked		(MerTreeItem* pItem);
	BOOL					HasItemShadow		(MerTreeItem* pItem);
	BOOL					IsLastItem			(MerTreeItem* pItem);
	BOOL					SetItemRedraw		(MerTreeItem* pItem, BOOL bOn);	
	BOOL					SetItemVisible		(MerTreeItem* pItem, BOOL bOn);	
	BOOL					SetItemSelected		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemReadOnly		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemExpandable	(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemExpanded		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemChecked		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemShadow		(MerTreeItem* pItem, BOOL bOn);
	COLORREF				GetBkColor			();

	virtual BOOL			RedrawItem			(MerTreeItem* pItem){return FALSE;};
	virtual void			RedrawAllItems		(){};
	virtual BOOL			GetItemRect			(int nItem, CRect& rcItem){return FALSE;};
	virtual BOOL			SelectItemByPoint	(CPoint pt){return FALSE;};
	virtual MerTreeItem*	ItemByPoint			(CPoint pt, CRect* pRcItem = NULL){return NULL;};
	virtual BOOL			ExpandItem			(MerTreeItem* pItem, BOOL bCollapse = FALSE, BOOL bInvalidate = TRUE){return FALSE;};
	virtual BOOL			CheckItem			(MerTreeItem* pItem, BOOL bCheck = TRUE, BOOL bInvalidate = TRUE){return FALSE;};
	virtual BOOL			SelectItem			(MerTreeItem* pItem, BOOL bInvalidate = TRUE){return FALSE;};
	virtual BOOL			DeleteItem			(MerTreeItem* pItem){return FALSE;};
	virtual BOOL			EnsureItemVisibility(MerTreeItem* pItem){return FALSE;};
	virtual BOOL			IsItemVisible		(MerTreeItem* pItem){return FALSE;};
	virtual void			ClearItems			(){};

	virtual int				GetSelectedItem		(){return -1;};
	virtual	void			OnItemsCountChanged	(){};
	virtual int				GetViewHeight		(){CRect rc;GetClientRect(rc);return rc.Height();};

protected:
	ESListControl*									m_pRoot;
	MercuryTreeGridContent*							m_pContent;
	MerTreeItem*									m_pFirstItem;	// First visible item.
	int												m_nTopItemIndex;
	MerTreeItem*									m_pSelItem;
	AutoSortedArrayTempl<MerTreeItemTag*, long>		m_arrItems;
	AutoSortedArrayTempl<long, CImage*>				m_arrImages;
	CPoint											m_ptViewOffset; // View area offset.
	int												m_nHeightView;	// Height of full view in pixels.
	int												m_nWidthView;	// Width of full view in pixels.

public:
	virtual BOOL	Create			(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void	OnPaintClient	(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual void	OnPrePaint		(CRect& rcRepaint);
	virtual void	GetClientRectMy	(CRect& rcClient);
	
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

__inline MercuryTreeGridContent* 
ESItemsView::GetContentClass(){
	if( !m_pContent )
		m_pContent = new MercuryTreeGridContent();
	return m_pContent;
	}

__inline CImage*
ESItemsView::GetItemImage(MerTreeItem* pItem){
	if( !pItem || pItem->iImage < 0 ) return NULL;
	return m_arrImages.GetData((UINT)pItem->iImage);
	}

__inline BOOL
ESItemsView::IsItemSelected(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x80000000);
	}

__inline BOOL
ESItemsView::IsItemReadOnly(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x40000000);
	}

__inline BOOL
ESItemsView::IsItemExpandable(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	return !(pItem->dwState&0x20000000);
	}

__inline BOOL
ESItemsView::IsItemExpanded(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	return !(pItem->dwState&0x10000000);
	}

__inline BOOL
ESItemsView::IsItemChecked(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	if( pItem->dwState&0x08000000 ){		// Is check box allowed.
		return pItem->dwState&0x04000000;	// Check state.
		}
	return 0;
	}

__inline BOOL					
ESItemsView::HasItemShadow(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x02000000);
	}

__inline BOOL
ESItemsView::IsItemParent(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->child != NULL);
	}

__inline BOOL
ESItemsView::SetItemSelected(MerTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x80000000;
	else
		pItem->dwState &= ~0x80000000;
	return 1;
	}

__inline BOOL					
ESItemsView::SetItemReadOnly(MerTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x40000000;
	else
		pItem->dwState &= ~0x40000000;
	return 1;
	}

__inline BOOL				
ESItemsView::SetItemExpandable(MerTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( !bOn )
		pItem->dwState |= 0x20000000;
	else
		pItem->dwState &= ~0x20000000;
	return 1;
	}

__inline BOOL
ESItemsView::SetItemExpanded(MerTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( !bOn )	{ // Collapse
		if( IsItemExpanded(pItem) )
			m_nHeightView -= m_pGUI->treeGrid.m_nItemHeight*GetVisibleChildsCt(pItem);
		pItem->dwState |= 0x10000000;
		}
	else{		// Expand
		BOOL bExpState = IsItemExpanded(pItem);
		pItem->dwState &= ~0x10000000;
		if( !bExpState )
			m_nHeightView += m_pGUI->treeGrid.m_nItemHeight*GetVisibleChildsCt(pItem);
		}
	return 1;
	}

__inline BOOL
ESItemsView::SetItemChecked(MerTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;

	if( bOn )
		pItem->dwState |= 0x0C000000; // Check allow flag and check flag.
	else
		pItem->dwState &= ~0x04000000; // Check flag.
	return 1;
	}

__inline BOOL
ESItemsView::SetItemShadow(MerTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x02000000; // Set item shadow.
	else
		pItem->dwState &= ~0x02000000; // Unset shadow flag.
	return 1;
	}

__inline BOOL
ESItemsView::SetItemRedraw(MerTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x01000000; // Set item redraw flag.
	else
		pItem->dwState &= ~0x01000000; // Unset item redraw flag.
	return 1;
	}

__inline BOOL
ESItemsView::GetItemRedraw(MerTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x01000000);
	}

__inline int
ESItemsView::GetParentsCount(MerTreeItem* pItem){
	if( !pItem ) return 0;
	int nCount = 0;
	while( pItem->parent ){
		nCount ++;
		pItem = pItem->parent;
		}
	return nCount;
	}

__inline COLORREF
ESItemsView::GetBkColor(){
	if( m_bActive )
		return m_pGUI->treeGrid.m_crBkgndActive;
	return m_pGUI->treeGrid.m_crBkgndNonActive;
	}

__inline BOOL
ESItemsView::IsLastItem(MerTreeItem* pItem){
	return (GetNextVisibleItem(pItem) == NULL);
	}

__inline MerTreeItem*
ESItemsView::GetRootItem(){
	if( !m_pContent ) return NULL;
	return m_pContent->GetRootItem();
	}

__inline void
ESItemsView::GetClientRectMy(CRect& rcClient){
	GetClientRect(rcClient);
	}
/*
__inline MerTreeItem*
ESItemsView::GetSelectedItem(){
	return m_pSelItem;
	}*/

__inline LPCTSTR
ESItemsView::GetItemText(MerTreeItem* pItem){
	if( !pItem ) return _T("");
	return pItem->sText.GetBuffer();
	}

__inline BOOL
ESItemsView::SetItemText(MerTreeItem* pItem, CString sText){
	if( !pItem )
		return FALSE;
	pItem->sText = sText;
	return TRUE;
	}

__inline MerTreeItem*
ESItemsView::GetFirstVisibleItem(){
	return m_pFirstItem;
	}