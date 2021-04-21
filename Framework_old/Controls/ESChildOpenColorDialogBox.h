#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildOpenColorDialogBox window

#include "ESChildTextBox.h"

class ESChildOpenColorDialogBox : public ESChildTextBox
{
// Construction
public:
	ESChildOpenColorDialogBox();
	virtual ~ESChildOpenColorDialogBox();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	SetFocus				();
	virtual void	KillFocus				();

	void			SetColor				(COLORREF cr, bool bRedraw);
	COLORREF		GetColor				(){return m_crChosen;};

protected:
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp				(_Point pt, UINT nFlags);
	virtual	void	OnMouseWheel			(UINT nFlags, short zDelta, _Point pt);

protected:
	COLORREF	m_crChosen;

	// Control events.
protected:
};