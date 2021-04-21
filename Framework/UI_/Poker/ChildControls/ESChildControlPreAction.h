#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlPreAction window

#include "..\..\..\Controls\ESChildControl.h"

class ESChildControlPreAction : public ESChildControl
{
// Construction
public:
	ESChildControlPreAction();
	virtual ~ESChildControlPreAction();


	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetText					(CString sText, bool bRedraw = true);
	void			SetCheck				(bool bCheck, bool bRedraw = true);
	bool			GetCheck				(){ return m_bChecked; };

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
	CString			m_sText;
	bool			m_bMouseDown;
	bool			m_bChecked;
	CSize			m_szCheckImage;
};