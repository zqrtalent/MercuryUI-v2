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
#include "..\Serialize\Serializable.h"
#include <atlimage.h>

class ESListControl : public MercuryBaseView
{
// Construction
public:
	ESListControl();
	virtual ~ESListControl();

	friend ESItemsList;
	friend ESHeaderCtrl;

public:
	void			SetBorder			(bool bLeft, bool bTop, bool bRight, bool bBottom);
	int				AddColumn			(CString	sTitle, CString sFieldName, int nAlign, int nWidth, int nWidthMin);
	bool			SetColTitle			(int nCol, CString sTitle);
	bool			RemoveColumn		(int nCol);

	int				GetInt				(int nRow, TCHAR* pszField, int nDefault = 0);
	CString			GetString			(int nRow, TCHAR* pszField);
	COleDateTime	GetDateTime			(int nRow, TCHAR* pszField);

	bool			Fill				(EnumerableObject<Serializable>* pListObjects);
	int				Clear				(bool bRedraw);
	int				Add					(EnumerableObject<Serializable>* pListObjects);

public:
	virtual void				DestroyClassMembers	();
	virtual BOOL				Create				(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	virtual	ESHeaderCtrl*		GetHeaderCtrl		();
	virtual MercuryScrollBar*	GetVScrollCtrl		();
	virtual MercuryScrollBar*	GetHScrollCtrl		();

protected:
	virtual BOOL				CanSizeHeaderItem	(int nItem, int nWidth);
	virtual void				OnSizeHeaderItem	(int nItem, int nWidth, int nWidthOld);
	virtual void				OnPaintClient		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual BOOL				UseDoubleBufferedDC	(){return FALSE;};
	virtual void				OnPrePaint			(CRect& rcRepaint);

	void						RecalcChildWndRects	(int cx, int cy);
	void						SizeAllChildWindows	();
	void						AdjustScrollBars	();
	bool						AdjustListViewPos	();

protected:
	ESItemsView*								m_pView;
	BOOL										m_bView;
	ESHeaderCtrl*								m_pHeader;
	BOOL										m_bHeader;
	MercuryScrollBar*							m_pVScroll;
	BOOL										m_bVScroll;
	MercuryScrollBar*							m_pHScroll;
	BOOL										m_bHScroll;
	AutoSortedArrayTempl<ControlTypes, RECT>	m_controlRects;
	int											m_nScrollsCt; // Visible scroll count.

	CRect										m_rcBorders;

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