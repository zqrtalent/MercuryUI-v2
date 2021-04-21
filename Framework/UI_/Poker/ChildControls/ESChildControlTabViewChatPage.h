#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabViewChatPage window

#include "ESChildControlTabViewPage.h"

#define ESCHAT_TAG_TEXT		1
#define ESCHAT_TAG_IMAGE	2

typedef struct ESChatTag_{
	ESChatTag_*	pFirst;
	ESChatTag_*	pNext;
	DWORD		dwChatType;
	} ESChatTag;

struct ESChatItem{
	ESChatTag* pTag;
	};

struct ESChatTextTag : ESChatTag{
	DWORD		dwFormatId;
	CString		sText;
	COLORREF	crText;
	};

struct ESChatImageTag : ESChatTag{
	int		nImageId;
	CSize	szImage;
	CString	sImageFile;
	};

class ESChildScrollBar;
class ESChildControlTextEdit;
class ESChildControlTabViewChatPage : public ESChildControlTabViewPage
{
// Construction
public:
	ESChildControlTabViewChatPage();
	virtual ~ESChildControlTabViewChatPage();

	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	IsScrollBarVisible		(ESChildControl* pScrollBar);

	bool			CreateTextFormat		(CString sName, const CString sFontName, bool bItalic, bool bUnderLine, bool bBold, int nFontHeight);
	int				AddTextToLine			(int nLine, CString sFormatName, CString sText, COLORREF crText, bool bRedraw);
	int				AddImageToLine			(int nLine, CString sImageFile, int nWidth, int nHeight,  bool bRedraw);
	bool			ClearContent			(bool bRedraw);
	int				GetLineCount			(){return m_listChatItems.GetCount();};
	bool			GotoLine				(int nLine, bool bRedraw);

protected:
	CFont*			GetFormatFont			(int nFormatId);
	CImage*			GetImageById			(int nId);
	void			DestroyObjects			();
	void			RecalcVScroll			(bool bInvalidate);
	int				SplitText				(CString& sText, CStringArray& arrStrings);

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
	virtual	void	OnMouseWheel		(UINT nFlags, short zDelta, CPoint pt);
	virtual bool	OnScrollBarEvent	(bool bVScroll, int nCode, ESChildControl* pSender);

protected:
	List<CImage>			m_listImages;
	List<CFont>				m_listFonts;
	List<ESChatItem>		m_listChatItems;
	AutoSortedArray			m_arrFormatNames;
	AutoSortedArray			m_arrImageNames;
	int						m_nItemHeight;
	int						m_nTopLine;
	ESChildScrollBar*		m_pVScroll;
	ESChildControlTextEdit*	m_pEdit;
	bool					m_bChatEdit;
	bool					m_bCenterAlign;
};