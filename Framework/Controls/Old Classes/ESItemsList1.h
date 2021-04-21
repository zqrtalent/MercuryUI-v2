#pragma once
/////////////////////////////////////////////////////////////////////////////
// ESItemsList window

#include "ESListControl.h"
#include "ESItemsView.h"

class ESItems;
class ESHeaderCtrl;
class VirtualRowSet;

class ESItemsList : public ESItemsView
{
// Construction
public:
	ESItemsList();
	virtual ~ESItemsList();

	friend class ESListControl;
	friend class ESHeaderCtrl;
	friend class ESItems;

public:
	BOOL					CheckItem			(int nItem, BOOL bCheck = TRUE, BOOL bInvalidate = TRUE);
	BOOL					SelectItem			(int nItem, BOOL bSelect, BOOL bInvalidate = TRUE);
	BOOL					GotoItem			(int nItem);
	BOOL					DeleteItem			(int nItem);
	int						GetItemByPoint		(CPoint pt);
	int						GetItemHeight		(){return m_pGUI->itemsList.m_nItemHeight;};
	virtual int				GetSelectedItem		();

	virtual	void			OnItemsCountChanged	();
	virtual int				GetViewHeight		();
	virtual BOOL			UseDoubleBufferedDC(){return TRUE;};

public:
	ESHeaderCtrl*			GetHeaderCtrl		(){return m_pRoot ? m_pRoot->GetHeaderCtrl() : NULL;};
	int						GetItemCount		();
	virtual void			ClearItems			();

protected:
	ESItems*			m_items;				// Items management class.
	int					m_nTopItem;				// Top item index.
	int					m_nTopItemOffset;		// Top item offset.
	int					m_nCurrItem;			// Current item index.
	int					m_nSubItemLeft;			// Left sub item index.
	int					m_nSubItemLeftOffset;	// Offset of left subitem.

protected:
	BOOL	GetVisibleItems	(int& nTop, int& nBottom);
	int		GetMaxTopItem	();
	void	SetTopItem		(int nTop, BOOL bRedraw);
	int		GetItemsPerPage	();
	
public:
	virtual BOOL	Create			(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
									DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
									UINT nID, CCreateContext* pContext = NULL);
protected:

	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void	OnPaintClient	(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual void	OnPrePaint		(CRect& rcRepaint);
	virtual void	GetClientRectMy	(CRect& rcClient);
	virtual void	SetRowSet		(VirtualRowSet* pRowSet);
	
	// Generated message map functions
protected:
	afx_msg LRESULT		OnVScroll			(WPARAM wParam, LPARAM);
	afx_msg LRESULT		OnHScroll			(WPARAM wParam, LPARAM);
	afx_msg void		OnSize				(UINT nType, int cx, int cy);
	afx_msg LRESULT		OnSetupVScrollInfo	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnSetupHScrollInfo	(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL		OnMouseWheel		(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void		OnLButtonDown		(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonDblClk		(UINT nFlags, CPoint point);
	afx_msg void		OnKeyDown			(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT		OnGetDlgCode		();
	afx_msg BOOL		OnMouseActivate		(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};

__inline void
ESItemsList::GetClientRectMy(CRect& rcClient){
	GetClientRect(rcClient);
	}