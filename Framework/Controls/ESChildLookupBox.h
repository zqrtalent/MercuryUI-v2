#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildLookupBox window

#include "ESChildTextBox.h"

class ESChildLookupBox : public ESChildTextBox
{
// Construction
public:
	ESChildLookupBox();
	virtual ~ESChildLookupBox();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	SetFocus				();
	virtual void	KillFocus				();

protected:
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual BOOL	OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp				(_Point pt, UINT nFlags);

	virtual _Rect	GetTextAreaRect			();

protected:
	void			CalcButtonRect			();

protected:
	_Rect					m_rcBoxButton;
	BOOL					m_bButtonPushed;

	// Control events.
protected:
};