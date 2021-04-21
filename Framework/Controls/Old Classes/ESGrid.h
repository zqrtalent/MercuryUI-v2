#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESGrid window

#include "MercuryBaseWnd.h"
#include "MercuryScrollBar.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

class ESGridColTheme
{
public:
	ESGridColTheme();
	virtual ~ESGridColTheme();

protected:
	virtual BOOL DrawCell();
};

class ESGridCol
{
public:
	ESGridCol();
	virtual ~ESGridCol();



protected:
	CString		m_sText;		// Column text.
	int			m_nWidth;		// Width of column in pixels.
	int			m_nOrdinal;		// Ordinal index of column.
	bool		m_bVisible;		// Visibility of column.
	bool		m_bLocked;		// Locked status of column.
	short		m_shType;		// Column type.
	short		m_shSortStatus;	// 0 - none, 1 - ascending, -1 descanding.
	void*		m_param;		// Param assiciated to column.
public:
	ESGridCol*	m_pNextCol;		// Next column pointer.
	ESGridCol*	m_pPrevCol;		// Previous column pointer.
};

class ESGrid : public MercuryBaseWnd
{
public:
	ESGrid();
	virtual ~ESGrid();

	void	SetNumberCols	(int nNumCols);
	void	SetNumberRows	(int nRows);
	BOOL	SetText			(int nRow, int nCol, TCHAR* pszText);
	void	SetVirtualMode	(bool bVirtualMode);


	virtual BOOL Create		(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	

protected:
	virtual BOOL	RegisterWndClass(LPCTSTR lpszClassName);
	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void	OnPaintClient	(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual void	OnPrePaint		(CRect rcRepaint);
	virtual void	GetClientRectMy	(CRect& rcClient);

	void	DestroyAll();

protected:
	ESGridCol*		m_pFirstCol;
	int				m_nNumberRows;
	int				m_nNumberCols;
	int*			m_pColHeights;
	int				m_nItemHeight;
	bool			m_bVirtualMode;


	ESGridCol*		m_pLeftCol;
	int				m_nTopRow;
	int				m_nOffsetX;
	int				m_nOffsetY;
	

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


/*
class ESGrid : public MercuryBaseWnd
{
// Construction
public:
	ESGrid();
	virtual ~ESGrid();

public:
	virtual BOOL			Create				(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL			RegisterWndClass	(LPCTSTR lpszClassName);

	MercuryTreeGridContent* GetContentClass		();
	void					DestroyClassMembers	();

	MerTreeItem*			AddItem				(LPTSTR lpszText, MerTreeItem* parent = NULL);
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

protected:
	MercuryScrollBar								m_vScroll;
	MercuryScrollBar								m_hScroll;
	BOOL											m_bVScroll;
	BOOL											m_bHScroll;

	MercuryTreeGridContent*							m_pContent;
	MerTreeItem*									m_pFirstItem;	// First visible item.
	int												m_nTopItemIndex;
	MerTreeItem*									m_pSelItem;
	AutoSortedArrayTempl<MerTreeItemTag*, long>		m_arrItems;
	AutoSortedArrayTempl<long, CImage*>				m_arrImages;
	CPoint											m_ptViewOffset; // View area offset.

	int												m_nHeightView;	// Height of full view in pixels.
	int												m_nWidthView;	// Width of full view in pixels.
protected:
	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void	OnPaintClient	(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual void	OnPrePaint		(CRect rcRepaint);
	virtual void	GetClientRectMy	(CRect& rcClient);
	
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
*/