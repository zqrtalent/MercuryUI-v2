#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlTree window

#include "..\..\..\Controls\ESChildControl.h"

typedef struct ESTreeItemTag
	{
	ESTreeItemTag*	parent;
	ESTreeItemTag*	next;
	ESTreeItemTag*	prev;
	ESTreeItemTag*	child;
	CString			sText;
	LONG			param;
	int				iImage;
	int				iSelImage;
	DWORD			dwState;
	} ESTreeItem;

class ESChildScrollBar;
class ESChildControlTree : public ESChildControl
{
// Construction
public:
	ESChildControlTree();
	virtual ~ESChildControlTree();

public:
	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy					();
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual void	SetLogicalPos			(float fPosX, float fPosY);
	virtual	void	SetLogicalSize			(float fCX, float fCY);
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	ESTreeItem*				AddItem				(LPTSTR lpszText, ESTreeItem* parent = NULL, BOOL bInvalidate = FALSE);
	LPCTSTR					GetItemText			(ESTreeItem* pItem);
	BOOL					SetItemText			(ESTreeItem* pItem, CString sText);

	ESTreeItem*				GetSelectedItem		();
	BOOL					SelectItem			(ESTreeItem* pItem, BOOL bInvalidate = TRUE);
	BOOL					DeleteItem			(ESTreeItem* pItem);
	int						AddItemImage		(CString sImageFile);
	BOOL					SetItemImage		(ESTreeItem* pItem, int nImage, int nImageSel, BOOL bInvalidate = TRUE);

	BOOL					ExpandItem			(ESTreeItem* pItem, BOOL bCollapse = FALSE, BOOL bInvalidate = TRUE);
	BOOL					CheckItem			(ESTreeItem* pItem, BOOL bCheck = TRUE, BOOL bInvalidate = TRUE);
	BOOL					RedrawItem			(ESTreeItem* pItem);
	void					UpdateScrollBars	();

	ESTreeItem*				GetRootItem			();
	ESTreeItem*				GetFirstVisibleItem	();
	ESTreeItem*				GetNextVisibleItem	(ESTreeItem* pItem);
	ESTreeItem*				GetPrevVisibleItem	(ESTreeItem* pItem);
	int						GetParentsCount		(ESTreeItem* pItem);
	int						GetVisibleChildsCt	(ESTreeItem* pItem);
	int						GetVisChildsCtSetRedraw	(ESTreeItem* pItem);

	CImage*					GetItemImage		(ESTreeItem* pItem);
	CFont*					GetItemFont			(ESTreeItem* pItem);
	COLORREF				GetItemTextColor	(ESTreeItem* pItem, COLORREF& crShadow);

	BOOL					IsItemSelected		(ESTreeItem* pItem);
	BOOL					GetItemRedraw		(ESTreeItem* pItem);
	BOOL					IsItemReadOnly		(ESTreeItem* pItem);
	BOOL					IsItemExpandable	(ESTreeItem* pItem);
	BOOL					IsItemExpanded		(ESTreeItem* pItem);
	BOOL					IsItemParent		(ESTreeItem* pItem);
	BOOL					IsItemChecked		(ESTreeItem* pItem);
	BOOL					HasItemShadow		(ESTreeItem* pItem);
	BOOL					IsItemVisible		(ESTreeItem* pItem);
	BOOL					IsLastItem			(ESTreeItem* pItem);
	BOOL					EnsureItemVisibility(ESTreeItem* pItem);

	BOOL					SetItemRedraw		(ESTreeItem* pItem, BOOL bOn);	
	BOOL					SetItemVisible		(ESTreeItem* pItem, BOOL bOn);	
	BOOL					SetItemSelected		(ESTreeItem* pItem, BOOL bOn);
	BOOL					SetItemReadOnly		(ESTreeItem* pItem, BOOL bOn);
	BOOL					SetItemExpandable	(ESTreeItem* pItem, BOOL bOn);
	BOOL					SetItemExpanded		(ESTreeItem* pItem, BOOL bOn);
	BOOL					SetItemChecked		(ESTreeItem* pItem, BOOL bOn);
	BOOL					SetItemShadow		(ESTreeItem* pItem, BOOL bOn);

	void					RedrawAllItems		();
	BOOL					GetItemRect			(ESTreeItem* pItem, CRect& rcItem);
	BOOL					SelectItemByPoint	(CPoint pt);
	ESTreeItem*				ItemByPoint			(CPoint pt, CRect* pRcItem = NULL);
	BOOL					MoveViewPos			(int nMoveX, int nMoveY, BOOL bInvalidate, BOOL bUpdateScrolls = TRUE, BOOL bRedrawAll = TRUE);
	COLORREF				GetBkColor			();

	void					SetBorders			(BOOL bLeft, BOOL bTop, BOOL bRight, BOOL bBottom){
		m_rcBorders.left	= bLeft;
		m_rcBorders.top		= bTop;
		m_rcBorders.right	= bRight;
		m_rcBorders.bottom	= bBottom;
		};

protected:

	virtual void	OnPaintClient		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);

	virtual	void	OnMouseHover		(CPoint pt, UINT nFlags);
	virtual void	OnMouseLeave		(UINT nFlags);
	virtual void	OnMouseEnter		(UINT nFlags);
	virtual BOOL	OnSetCursor			(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick		(CPoint pt, UINT nFlags);
	virtual	void	OnRButtonClick		(CPoint pt, UINT nFlags);
	virtual	void	OnMButtonClick		(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonDown		(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonUp			(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonDblClick	(CPoint pt, UINT nFlags);
	virtual	void	OnRButtonDblClick	(CPoint pt, UINT nFlags);
	virtual	void	OnMButtonDblClick	(CPoint pt, UINT nFlags);
	virtual	void	OnMouseWheel		(UINT nFlags, short zDelta, CPoint pt);
	virtual bool	OnScrollBarEvent	(bool bVScroll, int nCode, ESChildControl* pSender);
	virtual bool	SetFocus			();
	virtual void	KillFocus			();
	virtual void	OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags);

	/*
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
	*/

protected:
	ESChildScrollBar*								m_pVScroll;
	ESChildScrollBar*								m_pHScroll;

	ESTreeItem*										m_pFirstItem;	// First visible item.
	int												m_nTopItemIndex;
	ESTreeItem*										m_pSelItem;
	AutoSortedArrayTempl<ESTreeItem*, long>			m_arrItems;
	List<CImage>									m_listImages;
	CPoint											m_ptViewOffset; // View area offset.

	int												m_nHeightView;	// Height of full view in pixels.
	int												m_nWidthView;	// Width of full view in pixels.
	CRect											m_rcBorders;	// Borders visibility.

};

__inline ESTreeItem*
ESChildControlTree::GetSelectedItem(){
	return m_pSelItem;
	}

__inline CImage*
ESChildControlTree::GetItemImage(ESTreeItem* pItem){
	if( !pItem || pItem->iImage < 0 ) return NULL;
	return m_listImages[pItem->iImage];
	}

__inline BOOL
ESChildControlTree::IsItemSelected(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x80000000);
	}

__inline BOOL
ESChildControlTree::IsItemReadOnly(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x40000000);
	}

__inline BOOL
ESChildControlTree::IsItemExpandable(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	return !(pItem->dwState&0x20000000);
	}

__inline BOOL
ESChildControlTree::IsItemExpanded(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	return !(pItem->dwState&0x10000000);
	}

__inline BOOL
ESChildControlTree::IsItemChecked(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	if( pItem->dwState&0x08000000 ){		// Is check box allowed.
		return pItem->dwState&0x04000000;	// Check state.
		}
	return 0;
	}

__inline BOOL					
ESChildControlTree::HasItemShadow(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x02000000);
	}

__inline BOOL
ESChildControlTree::IsItemParent(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->child != NULL);
	}

__inline BOOL
ESChildControlTree::SetItemSelected(ESTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x80000000;
	else
		pItem->dwState &= ~0x80000000;
	return 1;
	}

__inline BOOL					
ESChildControlTree::SetItemReadOnly(ESTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x40000000;
	else
		pItem->dwState &= ~0x40000000;
	return 1;
	}

__inline BOOL				
ESChildControlTree::SetItemExpandable(ESTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( !bOn )
		pItem->dwState |= 0x20000000;
	else
		pItem->dwState &= ~0x20000000;
	return 1;
	}

__inline BOOL
ESChildControlTree::SetItemExpanded(ESTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( !bOn )	{ // Collapse
		if( IsItemExpanded(pItem) )
			m_nHeightView -= MercuryGUI::GetInstance()->treeGrid.m_nItemHeight*GetVisibleChildsCt(pItem);
		pItem->dwState |= 0x10000000;
		}
	else{		// Expand
		BOOL bExpState = IsItemExpanded(pItem);
		pItem->dwState &= ~0x10000000;
		if( !bExpState )
			m_nHeightView += MercuryGUI::GetInstance()->treeGrid.m_nItemHeight*GetVisibleChildsCt(pItem);
		}
	return 1;
	}

__inline BOOL
ESChildControlTree::SetItemChecked(ESTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x0C000000; // Check allow flag and check flag.
	else
		pItem->dwState &= ~0x04000000; // Check flag.
	return 1;
	}

__inline BOOL
ESChildControlTree::SetItemShadow(ESTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x02000000; // Set item shadow.
	else
		pItem->dwState &= ~0x02000000; // Unset shadow flag.
	return 1;
	}

__inline BOOL
ESChildControlTree::SetItemRedraw(ESTreeItem* pItem, BOOL bOn){
	if( !pItem ) return FALSE;
	if( bOn )
		pItem->dwState |= 0x01000000; // Set item redraw flag.
	else
		pItem->dwState &= ~0x01000000; // Unset item redraw flag.
	return 1;
	}

__inline BOOL
ESChildControlTree::GetItemRedraw(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	return (pItem->dwState&0x01000000);
	}

__inline int
ESChildControlTree::GetParentsCount(ESTreeItem* pItem){
	if( !pItem ) return 0;
	int nCount = 0;
	while( pItem->parent ){
		nCount ++;
		pItem = pItem->parent;
		}
	return nCount;
	}

__inline COLORREF
ESChildControlTree::GetBkColor(){
	if( m_pOwner->HasFocus(this) )
		return MercuryGUI::GetInstance()->treeGrid.m_crBkgndActive;
	return MercuryGUI::GetInstance()->treeGrid.m_crBkgndNonActive;
	}

__inline BOOL
ESChildControlTree::IsLastItem(ESTreeItem* pItem){
	return (GetNextVisibleItem(pItem) == NULL);
	}

__inline ESTreeItem*
ESChildControlTree::GetRootItem(){
	return NULL;
	}

