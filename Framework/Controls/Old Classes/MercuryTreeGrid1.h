#pragma once


/////////////////////////////////////////////////////////////////////////////
// MercuryTreeGrid window

#include "MercuryBaseWnd.h"
#include "MercuryScrollBar.h"
#include "MercuryTreeGridContent.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

/*
class ESItemInfo : public XmlSerializable
{
public:
	ESItemInfo(){};
	~ESItemInfo(){};

protected:
	virtual XmlSerializable* CreateObject(){return new ESItemInfo(); }

	BEGIN_DECLARE_VAR(ESItemInfo)
	DECLARE_VAR("item", "", 0, false, true, false)
	DECLARE_VAR("id", "int", offsetof(ESItemInfo,m_lId), false, false, false)
	DECLARE_VAR("text", "CString", offsetof(ESItemInfo,m_sItemText), false, false, false)
	DECLARE_VAR("desc", "CString", offsetof(ESItemInfo,m_sItemDescription), false, false, false)
	DECLARE_VAR("image", "CString", offsetof(ESItemInfo,m_sImage), false, false, false)
	END_DECLARE_VAR()

public:
	long		m_lId;
	CString		m_sItemText;
	CString		m_sItemDescription;
	CString		m_sImage;
};

class ESItems : public XmlSerializable
{
public:
	ESItems(){};
	~ESItems(){};

protected:
	virtual XmlSerializable* CreateObject(){return new ESItems(); }
	BEGIN_DECLARE_VAR(ESItems)
	DECLARE_VAR("items", "", 0, false, true, false)
	DECLARE_VAR("item", "", offsetof(ESItems,m_items), false, true, true)
	END_DECLARE_VAR()

public:
	EnumerableObject<ESItemInfo> m_items;
};
*/

class MercuryTreeGrid : public MercuryBaseWnd
{
// Construction
public:
	MercuryTreeGrid();
	virtual ~MercuryTreeGrid();

public:
	virtual BOOL			Create				(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL			RegisterWndClass	(LPCTSTR lpszClassName);

	MercuryTreeGridContent* GetContentClass		();
	void					DestroyClassMembers	();

	MerTreeItem*			AddItem				(LPTSTR lpszText, MerTreeItem* parent = NULL, BOOL bInvalidate = FALSE);
	LPCTSTR					GetItemText			(MerTreeItem* pItem);
	BOOL					SetItemText			(MerTreeItem* pItem, CString sText);

	MerTreeItem*			GetSelectedItem		();
	BOOL					SelectItem			(MerTreeItem* pItem, BOOL bInvalidate = TRUE);
	BOOL					DeleteItem			(MerTreeItem* pItem);
	int						AddItemImage		(CString sImageFile);
	BOOL					SetItemImage		(MerTreeItem* pItem, int nImage, int nImageSel, BOOL bInvalidate = TRUE);

	BOOL					ExpandItem			(MerTreeItem* pItem, BOOL bCollapse = FALSE, BOOL bInvalidate = TRUE);
	BOOL					CheckItem			(MerTreeItem* pItem, BOOL bCheck = TRUE, BOOL bInvalidate = TRUE);
	BOOL					RedrawItem			(MerTreeItem* pItem);
	void					UpdateScrollBars	();

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
	BOOL					IsItemVisible		(MerTreeItem* pItem);
	BOOL					IsLastItem			(MerTreeItem* pItem);
	BOOL					EnsureItemVisibility(MerTreeItem* pItem);

	BOOL					SetItemRedraw		(MerTreeItem* pItem, BOOL bOn);	
	BOOL					SetItemVisible		(MerTreeItem* pItem, BOOL bOn);	
	BOOL					SetItemSelected		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemReadOnly		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemExpandable	(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemExpanded		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemChecked		(MerTreeItem* pItem, BOOL bOn);
	BOOL					SetItemShadow		(MerTreeItem* pItem, BOOL bOn);

	void					RedrawAllItems		();
	BOOL					GetItemRect			(MerTreeItem* pItem, CRect& rcItem);
	BOOL					SelectItemByPoint	(CPoint pt);
	MerTreeItem*			ItemByPoint			(CPoint pt, CRect* pRcItem = NULL);
	BOOL					MoveViewPos			(int nMoveX, int nMoveY, BOOL bInvalidate, BOOL bUpdateScrolls = TRUE, BOOL bRedrawAll = TRUE);
	COLORREF				GetBkColor			();
	void					SetBorders			(BOOL bLeft, BOOL bTop, BOOL bRight, BOOL bBottom){
		m_rcBorders.left	= bLeft;
		m_rcBorders.top		= bTop;
		m_rcBorders.right	= bRight;
		m_rcBorders.bottom	= bBottom;
		};

protected:
	MercuryScrollBar							m_vScroll;
	MercuryScrollBar							m_hScroll;
	BOOL										m_bVScroll;
	BOOL										m_bHScroll;

	MercuryTreeGridContent*						m_pContent;
	MerTreeItem*								m_pFirstItem;	// First visible item.
	int											m_nTopItemIndex;
	MerTreeItem*								m_pSelItem;
	AutoSortedArrayTempl<MerTreeItemTag*, long>		m_arrItems;
	AutoSortedArrayTempl<long, CImage*>				m_arrImages;
	CPoint										m_ptViewOffset; // View area offset.

	int											m_nHeightView;	// Height of full view in pixels.
	int											m_nWidthView;	// Width of full view in pixels.
	CRect										m_rcBorders;	// Borders visibility.
protected:
	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void	OnPaintClient	(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual void	OnPrePaint		(CRect rcRepaint);
	virtual void	GetClientRectMy	(CRect& rcClient);
	virtual BOOL	GetBorders		(CRect& rcBorder){rcBorder = m_rcBorders; return TRUE;};
	virtual BOOL	UseDoubleBufferedDC(){return TRUE;};
	
	// Generated message map functions
protected:
	afx_msg LRESULT	OnVScroll		(WPARAM wParam, LPARAM);
	afx_msg LRESULT	OnHScroll		(WPARAM wParam, LPARAM);
	afx_msg void	OnShowWindow	(BOOL bShow, UINT nStatus);
	afx_msg void	OnChar			(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnKeyDown		(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnLButtonDblClk	(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown	(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp		(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove		(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor		(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL	OnMouseWheel	(UINT nFlags, short zDelta, CPoint pt);
	afx_msg UINT	OnGetDlgCode	();
	afx_msg BOOL	OnMouseActivate	(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg	void	OnSize			(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

__inline MercuryTreeGridContent* 
MercuryTreeGrid::GetContentClass()
	{
	if( !m_pContent )
		m_pContent = new MercuryTreeGridContent();
	return m_pContent;
	}

__inline CImage*
MercuryTreeGrid::GetItemImage(MerTreeItem* pItem)
	{
	if( !pItem || pItem->iImage < 0 ) return NULL;
	return m_arrImages.GetData((UINT)pItem->iImage);
	}

__inline BOOL
MercuryTreeGrid::IsItemSelected(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x80000000);
	}

__inline BOOL
MercuryTreeGrid::IsItemReadOnly(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x40000000);
	}

__inline BOOL
MercuryTreeGrid::IsItemExpandable(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	return !(pItem->dwState&0x20000000);
	}

__inline BOOL
MercuryTreeGrid::IsItemExpanded(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	return !(pItem->dwState&0x10000000);
	}

__inline BOOL
MercuryTreeGrid::IsItemChecked(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	if( pItem->dwState&0x08000000 ){		// Is check box allowed.
		return pItem->dwState&0x04000000;	// Check state.
		}
	return 0;
	}

__inline BOOL					
MercuryTreeGrid::HasItemShadow(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x02000000);
	}

__inline BOOL
MercuryTreeGrid::IsItemParent(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	return (pItem->child != NULL);
	}

__inline BOOL
MercuryTreeGrid::SetItemSelected(MerTreeItem* pItem, BOOL bOn)
	{
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x80000000;
	else
		pItem->dwState &= ~0x80000000;
	return 1;
	}

__inline BOOL					
MercuryTreeGrid::SetItemReadOnly(MerTreeItem* pItem, BOOL bOn)
	{
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x40000000;
	else
		pItem->dwState &= ~0x40000000;
	return 1;
	}

__inline BOOL				
MercuryTreeGrid::SetItemExpandable(MerTreeItem* pItem, BOOL bOn)
	{
	if( !pItem ) return FALSE;
	if( !bOn )
		pItem->dwState |= 0x20000000;
	else
		pItem->dwState &= ~0x20000000;
	return 1;
	}

__inline BOOL
MercuryTreeGrid::SetItemExpanded(MerTreeItem* pItem, BOOL bOn)
	{
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
MercuryTreeGrid::SetItemChecked(MerTreeItem* pItem, BOOL bOn)
	{
	if( !pItem ) return FALSE;

	if( bOn )
		pItem->dwState |= 0x0C000000; // Check allow flag and check flag.
	else
		pItem->dwState &= ~0x04000000; // Check flag.
	return 1;
	}

__inline BOOL
MercuryTreeGrid::SetItemShadow(MerTreeItem* pItem, BOOL bOn)
	{
	if( !pItem ) return FALSE;

	if( bOn )
		pItem->dwState |= 0x02000000; // Set item shadow.
	else
		pItem->dwState &= ~0x02000000; // Unset shadow flag.
	return 1;
	}

__inline BOOL
MercuryTreeGrid::SetItemRedraw(MerTreeItem* pItem, BOOL bOn)
	{
	if( !pItem ) return FALSE;

	if( bOn )
		pItem->dwState |= 0x01000000; // Set item redraw flag.
	else
		pItem->dwState &= ~0x01000000; // Unset item redraw flag.
	return 1;
	}

__inline BOOL
MercuryTreeGrid::GetItemRedraw(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x01000000);
	}

__inline int
MercuryTreeGrid::GetParentsCount(MerTreeItem* pItem)
	{
	if( !pItem ) return 0;
	int nCount = 0;
	while( pItem->parent ){
		nCount ++;
		pItem = pItem->parent;
		}
	return nCount;
	}

__inline COLORREF
MercuryTreeGrid::GetBkColor()
	{
	if( m_bActive )
		return m_pGUI->treeGrid.m_crBkgndActive;
	return m_pGUI->treeGrid.m_crBkgndNonActive;
	}

__inline BOOL
MercuryTreeGrid::IsLastItem(MerTreeItem* pItem)
	{
	return (GetNextVisibleItem(pItem) == NULL);
	}

__inline MerTreeItem*
MercuryTreeGrid::GetRootItem()
	{
	if( !m_pContent ) return NULL;
	return m_pContent->GetRootItem();
	}

__inline void
MercuryTreeGrid::GetClientRectMy(CRect& rcClient)
	{
	GetClientRect(rcClient);
	if( m_bVScroll ){
		rcClient.right -= m_pGUI->scrollBar.m_nVScrollWidth;
		}
	}
