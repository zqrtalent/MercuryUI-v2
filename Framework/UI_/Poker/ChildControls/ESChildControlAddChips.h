#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlAddChips window

#include "..\..\..\Controls\ESChildControl.h"

class ESChildControlButton;
class ESChildControlMoneyEdit;
class ESChildControlAddChips : public ESChildControl
{
// Construction
public:
	ESChildControlAddChips();
	virtual ~ESChildControlAddChips();

	virtual bool	Create			(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos			(int x, int y, bool bRedraw = true);
	virtual bool	SetSize			(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy			();

	bool			Setup			(int nMin, int nMax, int nStep, int nBalance, bool bInvalidate = true);
	bool			SetSliderPos	(int nPos, bool bInvalidate = true, bool bReflectEvent = true);
	int				GetSliderPos	() { return m_nPos; };
	int				GetSliderPosMin	() { return m_nMin; };
	int				GetSliderPosMax	() { return m_nMax; };
	int				GetSliderStep	() { return m_nStep; };

protected:
	virtual void	OwnerWindowSizeChanged(int cx, int cy);
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

protected:
	bool			CalculateCursorPos			(bool bInvalidate);
	bool			CalculateCursorPosByPoint	(CPoint pt);

protected:
	bool						m_bMouseDown;
	int							m_nPos;
	int							m_nMin;
	int							m_nMax;
	int							m_nBalance;
	int							m_nStep;
	CPoint						m_ptCursorPos;

	bool						m_bHandCursor;
	bool						m_bCursorCaptured;
	bool						m_bCloseButton;

	CRect						m_rcSlider;
	CRect						m_rcSliderBar;
	CRect						m_rcMinus;
	CRect						m_rcPlus;
	CRect						m_rcCursor;

	CRect						m_rcCloseButton;

	ESChildControlButton*		m_pButton;
	ESChildControlMoneyEdit*	m_pMoneyEdit;

	CImage						m_imageScreen;			// Full image of chips control.
};