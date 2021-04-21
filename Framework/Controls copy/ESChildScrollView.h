#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildScrollView window

#include "ESChildControl.h"
#include "ESEasyMotion.h"
class ESChildScrollBar;

class ESChildScrollView : public ESChildControl
{
// Construction
protected:
	ESChildScrollView();
	virtual ~ESChildScrollView();

public:
	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	IsScrollBarVisible		(ESChildControl* pScrollBar);

	int				GetVScrollPos			();
	int				GetHScrollPos			();
	void			SetBorder				(BOOL bBorder){bBorder ? m_nBorderWidth = 1 : m_nBorderWidth = 0; };

	ESChildScrollBar*	GetVScrollBar(){return m_pVScroll;};

protected:
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual	void	OnMouseWheel			(UINT nFlags, short zDelta, _Point pt);
	virtual bool	OnScrollBarEvent		(bool bVScroll, int nCode, ESChildControl* pSender);

	virtual void	OnDeserializationCompleted	();

	// Scroll motion.
	virtual void	OnMotionStart			(DWORD_PTR dwParam);
	virtual void	OnMotionProgress		(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY);
	virtual void	OnMotionEnd				();
	virtual void	OnMotionStopped			();

protected:
	void			AdjustScrollBarPos		(bool bRedraw);
	void			RedrawViewArea			();
	void			EnsureItemVisibility	(int nItem, bool bRedraw);

protected:
	ESChildScrollBar*	m_pVScroll;
	ESChildScrollBar*	m_pHScroll;

	ESEasyMotion		m_motion;
	int					m_nMotionStartPos;
	int					m_nMotionEndPos;

	COLORREF			m_crBorder;
	int					m_nBorderWidth;

	bool				m_bFillWithColor;
	COLORREF			m_crFill;
	int					m_nLineHeight;

	int					m_nVScrollBarTopOffset;

	_Size				m_szView;
	_Size				m_szPage;

	// Scrollbar resources. {{
	_Image				m_imageVScrollThumb;
	_Image				m_imageVScrollArrowBottomSel;
	_Image				m_imageVScrollArrowBottomNormal;
	_Image				m_imageVScrollMiddle;
	_Image				m_imageVScrollArrowTopSel;
	_Image				m_imageVScrollArrowTopNormal;
	_Rect				m_rcVThumbOmitBorder;

	_Image				m_imageHScrollThumb;
	_Image				m_imageHScrollArrowRightSel;
	_Image				m_imageHScrollArrowRightNormal;
	_Image				m_imageHScrollMiddle;
	_Image				m_imageHScrollArrowLeftSel;
	_Image				m_imageHScrollArrowLeftNormal;
	_Rect				m_rcHThumbOmitBorder;

	_Image				m_imageDivider;

	int					m_nArrowBtnCX;
	int					m_nArrowBtnCY;
	// }}

protected:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildScrollView();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};