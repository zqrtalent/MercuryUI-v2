#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildOpenFontDialogBox window

#include "ESChildTextBox.h"

class ESChildOpenFontDialogBox : public ESChildTextBox
{
// Construction
public:
	ESChildOpenFontDialogBox();
	virtual ~ESChildOpenFontDialogBox();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	SetFocus				();
	virtual void	KillFocus				();

	void			SetFont					(Font* pFont, bool bRedraw);
	Font*			GetFont					(){return &m_textFont;};

protected:
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp				(_Point pt, UINT nFlags);
	virtual	void	OnMouseWheel			(UINT nFlags, short zDelta, _Point pt);

protected:
	LOGFONT		m_lfChosen;

	// Control events.
protected:
};