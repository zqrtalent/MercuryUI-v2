#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESHeaderCtrl window

#include "MercuryBaseWnd.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

class ESHeaderItems;
class ESListControl;
class VirtualRowSet;

class ESHeaderCtrl : public MercuryBaseWnd
{
// Construction
public:
	ESHeaderCtrl();
	virtual ~ESHeaderCtrl();

	friend class ESItemsList;
	friend class ESHeaderItems;
	friend class ESListControl;

public:
	virtual BOOL	Create				(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL	OnParentResized		(int cx, int cy);
	virtual BOOL	UseDoubleBufferedDC	(){return m_bDoubleBufferedDC;};

	// Operations
public:

	int			AddItem					(TCHAR* pszText, TCHAR* pszField, DWORD dwTextAlignH = TEXT_HALIGN_LEFT, 
										int nWidth = 80, int nWidthMin = 40,
										int nImage = -1, int nSelImage = -1);
	int			InsertItem				(int nInsertAt, TCHAR* pszText, TCHAR* pszField, DWORD dwTextAlignH = TEXT_HALIGN_LEFT, 
										int nWidth = 80, int nWidthMin = 40,
										int nImage = -1, int nSelImage = -1);
	int			RemoveItem				(int nIndex);

	void		RemoveAllItems			();
	int			GetItemCount			();
	void		SetItemCount			(int nCount);
	int			GetFieldId				(int nItem);

	void		SetItemsWidthCalcState	(BOOL bCalculate){m_bCalcItemsWidth = bCalculate;};
	int			GetItemsWidth			();
	int			GetItemWidth			(int nItem);
	int			GetItemWidthMin			(int nItem);
	int			GetLeftItem				(){return m_nLeftItem;};
	void		SetLeftItem				(int nLeftItem){m_nLeftItem = nLeftItem;};
	int			GetLeftItemOffset		(){return m_nLeftItemOffsetX;};
	void		GetItemRect				(int nItem, CRect& rcItem);
	int			GetItemHAlign			(int nItem);

	int			GetLeftOffset			();
	int			SetLeftOffset			(int nOffset, BOOL bRedraw);
	int			GetDragItemIndex		();
	void		SetDragItemIndex		(int nDragItem);

	// Attributes
public:

protected:
	BOOL		ItemDragging			();
	BOOL		StartDragItem			(int nItem);
	BOOL		EndDragItem				();
	void		OnDragItem				(CPoint pt);

	// Draw
protected:
	BOOL		RedrawItem				(int nItem);
	void		RedrawAllItems			();	
	void		RedrawColumn			(int nCol);

public:
	void		SetRowSet				(VirtualRowSet* pRowSet);

protected:
	ESListControl*		m_pRoot;
	ESHeaderItems*		m_pItems;
	VirtualRowSet*		m_pRowSet;

	BOOL				m_bDoubleBufferedDC;
	int					m_nLeftItem;		// Left item index;
	int					m_nLeftItemOffsetX;	// OX offset of left item in pixels.
	BOOL				m_bCalcOffsetX;		// Means that we must calculate offset.
	int					m_nOffsetX;			// OX offset of items in pixels.

	int					m_nItemHover;
	BOOL				m_bSizing;
	BOOL				m_bOverSizingLabel;
	BOOL				m_bMoving;

	CPoint				m_ptMouseDown;
	BOOL				m_bMouseDown;
	UINT				m_nHitTest;

	int					m_nItemsWidth;		// Calculated items width.
	BOOL				m_bCalcItemsWidth;	// Flag to calculate all items width.

	// Item dragging
	int					m_nItemDrag;		// Draging item index.
	CPoint				m_ptDragImage;		// Mouse capture point of drag image.
	CSize				m_szDragImage;		// Drag image size.
	CImageList			m_dragImageList;	// Dragging image list.

protected:
	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	
	// Generated message map functions
public:
	afx_msg void	OnLButtonDblClk		(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown		(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp			(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove			(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor			(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL	OnMouseWheel		(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnTimer				(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};