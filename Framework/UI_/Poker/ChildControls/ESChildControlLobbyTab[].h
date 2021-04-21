#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildControlLobbyTab window

#include "..\..\..\Controls\ESChildControl.h"

struct ESLobbyTabItem{
	CString		sTitle;
	UINT		nData;
	int			nWidth;
	};

class ESChildControlLobbyTab : public ESChildControl
{
// Construction
public:
	ESChildControlLobbyTab();
	virtual ~ESChildControlLobbyTab();

	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetTabItemImages		(CImage* pImageTabItem, CImage* pImageTabItemActive, CRect rcTabItemOmitBorder);
	void			SetTabItemFonts			(CFont* pFontTabItem, CFont* pFontTabItemActive);
	void			SetTabItemTextColors	(COLORREF crTabItemText, COLORREF crActiveTabItemText);

	int				AddTabItem				(CString sTitle, UINT nData);
	bool			RemoveTabItem			(int nIndex);
	bool			RemoveAllTabItem		();
	bool			ActivateTabItem			(int nIndex, bool bRedraw = false);
	bool			SetTabItemTitle			(int nItem, CString sTitle, bool bRedraw);
	int				GetActiveItem			(){return m_nActiveItem;};
	UINT			GetActiveItemData		(){return GetItemData(GetActiveItem());}
	UINT			GetItemData				(int nItem);

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
	int				GetItemByPoint		(CPoint pt);
	bool			RedrawItems			();

protected:
	List<ESLobbyTabItem>	m_listTabItems;
	int						m_nActiveItem;
	int						m_nItemUnderCursor;

	CImage					*m_pImageTabItem;
	CImage					*m_pImageTabItemActive;
	CRect					m_rcTabItemOmitBorder;

	CFont					*m_pTabItemFont;
	CFont					*m_pTabItemActiveFont;

	COLORREF				m_crTabItemText;
	COLORREF				m_crActiveTabItemText;
};