#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildScrollView window

#include "ESChildControl.h"

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

class ESChildScrollBar : public ESChildControl
{
// Construction
public:
	ESChildScrollBar(bool bVScroll = true);
	virtual ~ESChildScrollBar();

	friend class ESChildScrollView;

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	VisibilityChanged		(bool bVisible);

	bool			SetScrollInfo			(int nMin, int nMax, int nPage, bool bInvalidate);
	bool			SetScrollPos			(int nPosNew, bool bInvalidate);
	int				GetScrollPos			(){return m_nPos;};
	int				GetScrollPage			(){return m_nPage;};
	int				GetLimitPos				(){return (m_nMax - m_nPage) + m_nMin;};

	void			SetScrollMin			(int nMin){m_nMin = nMin;};
	void			SetScrollMax			(int nMax){m_nMax = nMax;};
	void			SetScrollPage			(int nPage){m_nPage = nPage;};

protected:
	virtual void	OnPaintClient		(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	virtual	void	OnMouseHover		(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave		(UINT nFlags);
	virtual void	OnMouseEnter		(UINT nFlags);
	virtual BOOL	OnSetCursor			(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick		(_Point pt, UINT nFlags);
	virtual	void	OnRButtonClick		(_Point pt, UINT nFlags);
	virtual	void	OnMButtonClick		(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown		(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDblClick	(_Point pt, UINT nFlags);
	virtual	void	OnRButtonDblClick	(_Point pt, UINT nFlags);
	virtual	void	OnMButtonDblClick	(_Point pt, UINT nFlags);
	virtual void	OnTimer				(UINT nIDEvent);

	void			DrawVScrollBar				(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	void			DrawHScrollBar				(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	void			CalculateRect				();
	int				ThumbTopOffsetByScrollPos	(int nThumbWidthHeight);
	int				ScrollPosByThumbTopOffset	();
	int				HitTest						(_Point pt);
	bool			RedrawRectByHitTest			(int nHitTest, _Rect& rRect);
	bool			SendEventByHitTest			(int nHitTest, bool bStartAutoScrollTimer = false);

protected:
	bool				m_bHover;
	bool				m_bMouseDown;

	BOOL				m_bVscroll;

	int					m_nPos;
	int					m_nPage;
	int					m_nMin;
	int					m_nMax;

	int					m_nThumbHeightMin;

	_Point				m_ptMouseDown;
	UINT				m_nHitTest;			

	_Rect				m_rcArrowBtnTop;
	_Rect				m_rcArrowBtnBottom;
	_Rect				m_rcArrowBtnLeft;
	_Rect				m_rcArrowBtnRight;
	_Rect				m_rcThumb;
	_Rect				m_rcThumbDown;		// Thumb button rect when it is captured by mouse lbutton.

	// Scrollbar image resources.
	Image*				m_pImageVScrollThumb;
	Image*				m_pImageVScrollArrowBottomSel;
	Image*				m_pImageVScrollArrowBottomNormal;
	Image*				m_pImageVScrollMiddle;
	Image*				m_pImageVScrollArrowTopSel;
	Image*				m_pImageVScrollArrowTopNormal;
	_Rect				m_rcVThumbOmitBorder;

	Image*				m_pImageHScrollThumb;
	Image*				m_pImageHScrollArrowRightSel;
	Image*				m_pImageHScrollArrowRightNormal;
	Image*				m_pImageHScrollMiddle;
	Image*				m_pImageHScrollArrowLeftSel;
	Image*				m_pImageHScrollArrowLeftNormal;
	_Rect				m_rcHThumbOmitBorder;

	Image*				m_pImageDivider;

	int					m_nArrowBtnCX;
	int					m_nArrowBtnCY;
};