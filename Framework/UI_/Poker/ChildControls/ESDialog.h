#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESDialog window

#include "..\..\..\Controls\ESChildControl.h"
#include "..\..\..\Controls\ESDialogBase.h"


class ESChildControlImageButton;

class ESDialog : public ESDialogBase
{
// Construction
public:
	ESDialog();
	virtual ~ESDialog();

	virtual bool			Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool			SetPos					(int x, int y, bool bRedraw = true);
	virtual bool			SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool			Destroy					();
	virtual void			OwnerWindowSizeChanged	(int cx, int cy);

	virtual void			OnLoading				();

	virtual ESChildControl*	InitChildControl		(DialogItem* pItem);
	virtual CRect			GetDialogOmitBorder		();
	virtual bool			AddChild				(ESChildControl* pChild, CRect rcChild, bool bRedraw);

	virtual bool			MoveControl				(int x, int y, bool bRedraw);

	virtual BOOL			OnOK					();
	virtual BOOL			OnCancel				();
	virtual BOOL			OnTabControl			();

	virtual void			SetTitleBar				(bool bTitleBar);
	virtual void			SetFont					(CFont* pFont, bool bDestroy = false){ m_pFont = pFont; m_bDestroyFont = bDestroy;};
	virtual void			SetTextColor			(COLORREF crText) { m_crText = crText; };
	virtual void			SetTitle				(CString sText, bool bRedraw){m_sTitle = sText; if( bRedraw ) Redraw();};
	virtual void			SetCentred				(bool bCentred){m_bCentred = bCentred;};
	virtual void			SetSizeTextFont			(bool bSizeTextFont){m_bSizeTextFont = bSizeTextFont;};
	virtual void			SetFillColor			(COLORREF crFill){m_crFill = crFill;};
	virtual int				GetCloseButtonId		();

	void					SetFocus				(ESChildControl* pChild){m_pFocusedChild = pChild; if(pChild){pChild->SetFocus();} };

protected:
	CRect					GetCaptionRect			();
	CRect					GetCloseButtonRect		();
	int						MoveChildControls		(CRect rcParent, CRect rcParentOld);

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
	virtual	void	OnKeyDown			(UINT nChar, UINT nRepCnt, UINT nFlags);

	// Child control events.
protected:
	void			OnCloseDialog		(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam = 0);

protected:
	CRect						m_rcOmitBorder;		// Dialog border.
	CRect						m_rcClientArea;		// Client area rect.
	CFont*						m_pFont;			// Dialog font.
	bool						m_bDestroyFont;		// Destroy font.
	COLORREF					m_crText;			// Text color.
	COLORREF					m_crFill;			// Fill color.

	bool						m_bTitleBar;		// Title bar existance.

	CString						m_sTitle;			// Dialog title.
	bool						m_bCentred;			// Cantred state.
	bool						m_bMoving;			// Moving state.
	CPoint						m_ptCaptured;		// Mouse point where it was captured.

	ESChildControl*				m_pFocusedChild;	// Focused child control.
	int							m_nRetCode;			// Retirn code.

	bool						m_bSizeTextFont;

	ESChildControlImageButton*	m_pCloseButton;		// Close image button.
};