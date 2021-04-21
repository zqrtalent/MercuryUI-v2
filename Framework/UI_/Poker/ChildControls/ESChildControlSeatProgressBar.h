#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeatProgressBar window

#include "..\..\..\Controls\ESChildControl.h"

class ESChildControlSeatProgressBar : public ESChildControl
{
// Construction
public:
	ESChildControlSeatProgressBar();
	virtual ~ESChildControlSeatProgressBar();

	virtual bool	Create			(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos			(int x, int y, bool bRedraw = true);
	virtual bool	SetSize			(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy			();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);


	bool			SetProgressPos	(int nPos, int nPosMax, bool bRedraw = true);

protected:
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
	bool		m_bActive;
	int			m_nPos;
	int			m_nPosMax;
};