#pragma once


/////////////////////////////////////////////////////////////////////////////
// MercuryScrollBar window

#include "MercuryBaseWnd.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

#define HITTEST_LEFT_ARROW		1
#define HITTEST_RIGHT_ARROW		2
#define HITTEST_TOP_ARROW		3
#define HITTEST_BOTTOM_ARROW	4
#define HITTEST_PAGE_DOWN		5
#define HITTEST_PAGE_UP			6
#define HITTEST_PAGE_LEFT		7
#define HITTEST_PAGE_RIGHT		8
#define HITTEST_THUMB			9

#define AUTO_SCROLL_TIME		50
#define AUTO_SCROLL_START_TIME	300

#define TIMER_SCROLL_DOWN		12000
#define TIMER_SCROLL_UP			12001
#define TIMER_SCROLL_PAGEDOWN	12002
#define TIMER_SCROLL_PAGEUP		12003
#define TIMER_SCROLL_LEFT		12004
#define TIMER_SCROLL_RIGHT		12005
#define TIMER_SCROLL_PAGELEFT	12006
#define TIMER_SCROLL_PAGERIGHT	12007

class MercuryScrollBar : public MercuryBaseWnd
{
// Construction
public:
	MercuryScrollBar();
	virtual ~MercuryScrollBar();

public:
	virtual BOOL			Create				(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL			RegisterWndClass	(LPCTSTR lpszClassName);
	virtual BOOL			UseDoubleBufferedDC	(){return TRUE;};

	BOOL	SetScrollPos	(int nPosNew, BOOL bInvalidate = FALSE);
	BOOL	SetScrollInfo	(int nMin, int nMax, int nPage);
	UINT	GetScrollPos	() {return m_nPos;};
	UINT	GetPageSize		() {return m_nPage;};
	UINT	GetMin			() {return m_nMin;};
	UINT	GetMax			() {return m_nMax;};
	UINT	GetLimit		() {return (m_nMax - m_nPage) + m_nMin;};

protected:
	bool				m_bVscroll;

	int					m_nPos;
	int					m_nPage;
	int					m_nMin;
	int					m_nMax;

	int					m_nThumbHeightMin;

	CPoint				m_ptMouseDown;
	BOOL				m_bMouseDown;
	UINT				m_nHitTest;			

	CRect				m_rcArrowBtnTop;
	CRect				m_rcArrowBtnBottom;
	CRect				m_rcArrowBtnLeft;
	CRect				m_rcArrowBtnRight;
	CRect				m_rcThumb;
	CRect				m_rcThumbDown;		// Thumb button rect when it is captured by mouse lbutton.

protected:
	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void	OnPaintClient		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);

		// Retrieves scroll position by thumb position.
	int				PosByThumbPos		();

		// Retrieves thumb position by scroll position.
	int				ThumbPosByPos		();

	void			RedrawArrowButton	(UINT nBtnHitTest);
	
	// Generated message map functions
protected:
	afx_msg void	OnLButtonDblClk	(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown	(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp		(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove		(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor		(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL	OnMouseWheel	(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void	OnSize			(UINT nType, int cx, int cy);
	afx_msg void	OnTimer			(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};