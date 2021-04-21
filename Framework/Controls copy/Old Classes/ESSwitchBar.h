#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESSwitchBar window

#include "MercuryBaseWnd.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include "..\Array\ChainTempl.h"
#include <atlimage.h>

class ESItemContainer;

typedef struct SwitchBarItemTag
{
SwitchBarItemTag*	next;
SwitchBarItemTag*	prev;
CString				sItemText;
bool				bDisabled;
int					nIndex;
long				lData;
} SwitchBarItem, *PSwitchBarItem;

class ESSwitchBar : public MercuryBaseWnd
{
// Construction
public:
	ESSwitchBar();
	virtual ~ESSwitchBar();

	friend class ESItemContainer;

public:
	virtual BOOL	Create				(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL	UseDoubleBufferedDC	(){return FALSE;};
	virtual BOOL	OnParentResized		(int cx, int cy);

	// Operations
public:
	int			AddItem					(TCHAR* pszText, long lItemData = 0);
	int			InsertItem				(int nIndex, TCHAR* pszText);
	int			RemoveItem				(int nIndex);
	void		RemoveAllItems			();
	int			GetItemCount			();
	void		SetItemCount			(int nCount);
	bool		EnableItem				(int nItem, bool bEnable, bool bRedraw = true);
	void		SetSwitchable			(bool bSwitchable){m_bSwitchable = bSwitchable;};

protected:

	// Draw
protected:
	BOOL			RedrawItem				(int nItem);
	void			RedrawAllItems			();	
	BOOL			GetItemRect				(int nItem, CRect& rcItem);
	int				GetItemIndexByPoint		(CPoint pt, CRect& rcItem);
	SwitchBarItem*	GetItem					(int nItem);

protected:
	ESItemContainer*			m_pRoot;
	ChainTempl<SwitchBarItem>	m_items;
	
	bool						m_bSwitchable;		// Is item switchable to keep activated items.
	CRgn						m_rgnOutOfItems;
	CRect						m_rcItemsFull;
	int							m_nItemWidth;		// Item width in pixels.
	int							m_nItemFocus;		// Focused item not active yet.
	BOOL						m_bMouseDown;		// Mouse is pressed.
	int							m_nActiveItem;		// Active item index.
	BOOL						m_bOverTheItems;	// Cursor is over the items.


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