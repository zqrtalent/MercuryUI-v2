#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildOpenFileBox window

#include "ESChildTextBox.h"

class ESChildOpenFileBox : public ESChildTextBox
{
// Construction
public:
	ESChildOpenFileBox(_string sFilter);
	virtual ~ESChildOpenFileBox();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	SetFocus				();
	virtual void	KillFocus				();

	_string			GetFilePath				(){return m_sOpenedFilePath;};

protected:
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);

protected:
	_string		m_sFilter;
	_string		m_sOpenedFilePath;

	// Control events.
protected:
};