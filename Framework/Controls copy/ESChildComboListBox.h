#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildComboListBox window

#include "ESChildScrollView.h"
#include "../Array/StringToPtrArray.h"

class ESChildComboListBox : public ESChildScrollView
{
// Construction
public:
	ESChildComboListBox();
	virtual ~ESChildComboListBox();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	VisibilityChanged		(bool bVisible);
	virtual bool	SetFocus				();
	virtual void	KillFocus				();	

	bool			AddItem					(_string sText, void* lpParam = NULL, bool bRedraw = false);
	void			ClearContent			(bool bRedraw);
	int				GetSelectedItem			()	{return m_nSelIndex;};
	_string		GetItemText				(int nItem);
	void*			GetItemParam			(int nItem);
	bool			SetSelectedItem			(int nItem, bool bRedraw, bool bAdjustScroll = true);
	
	void			SetTextFont				(_Font* pTextFont, bool bRedraw);
	int				GetItemHeight			(){return m_nItemHeight;};

protected:
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual	void	OnMouseHover			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual void	OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool	OnScrollBarEvent		(bool bVScroll, int nCode, ESChildControl* pSender);

protected:
	int				GetItemByPoint			(_Point pt);
	bool			GetItemRect				(int nItem, _Rect& rcItem);

protected:
	int						m_nTopIndex;
	int						m_nTopOffset;
	int						m_nSelIndex;
	StringToPtrArray		m_content;

	int						m_nItemHeight;
	_Font					m_textFont;
	COLORREF				m_crText;
	COLORREF				m_crTextSel;
	COLORREF				m_crSelection;
};