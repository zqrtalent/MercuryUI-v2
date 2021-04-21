#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlTextEdit window

#include "..\..\..\Controls\ESChildControl.h"
#include "..\..\..\Controls\amsEdit.h"

class ESChildControlTextEdit : public ESChildControl
{
// Construction
public:
	ESChildControlTextEdit();
	virtual ~ESChildControlTextEdit();

	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	VisibilityChanged		(bool bVisible);
	virtual bool	SetFocus				();
	virtual void	SetFont					(CFont* pFont);
	void			SetPassword				(bool bPassword){m_bPassword = bPassword;};
	CEdit*			GetEditControl			(){return &m_editControl;};
	CString			GetText					();
	void			SetText					(CString sText);

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

	static	LRESULT	__stdcall OnEditWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
	static WNDPROC*				GetOldProcAddress()
			{
			static WNDPROC procOld;
			return &procOld;
			}

protected:
	CAMSAlphanumericEdit	m_editControl;
	CFont*					m_pEditFont;
	bool					m_bPassword;
};