#pragma once


/////////////////////////////////////////////////////////////////////////////
// MercuryTreeGrid window

#include "MercuryBaseWnd.h"
#include "MercuryScrollBar.h"
#include "ESSwitchBar.h"
#include "ESHeaderCtrl.h"
#include "ESItemsView.h"
#include "MercuryTreeGrid.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

class ESItemContainer : public MercuryBaseView
{
// Construction
public:
	ESItemContainer();
	virtual ~ESItemContainer();

	DECLARE_DYNCREATE(ESItemContainer)

	friend ESItemsList;
	friend ESHeaderCtrl;

public:
	virtual void	DestroyClassMembers	();
protected:

	ESItemsView*					m_pView;
	BOOL							m_bView;
	ESHeaderCtrl*					m_pHeader;
	BOOL							m_bHeader;
	MercuryScrollBar*				m_pVScroll;
	BOOL							m_bVScroll;
	MercuryScrollBar*				m_pHScroll;
	BOOL							m_bHScroll;
	ESSwitchBar*					m_pSwitchBar;
	BOOL							m_bSwitchBar;
	MercuryTreeGrid*				m_pTree;
	BOOL							m_bTree;

	AutoSortedArrayTempl<ControlTypes, RECT> m_controlRects;

	int								m_nScrollsCt; // Visible scroll count.

	virtual BOOL	Create				(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	virtual BOOL	CanSizeHeaderItem	(int nItem, int nWidth);
	virtual void	OnSizeHeaderItem	(int nItem, int nWidth, int nWidthOld);
	virtual void	OnPaintClient		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual BOOL	UseDoubleBufferedDC	(){return FALSE;};
	virtual void	OnPrePaint			(CRect& rcRepaint);

	void			RecalcChildWndRects	(int cx, int cy);
	void			SizeAllChildWindows	();
	void			AdjustScrollBars	();
	
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
	afx_msg void	OnGetMinMaxInfo	(MINMAXINFO* lpMMI);
	DECLARE_MESSAGE_MAP()
};