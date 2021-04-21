#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESSimpleRenderView window

#include "ESChildScrollView.h"

enum Alignment{
		HAlign_Left = 0,
		HAlign_Right,
		HAlign_Center,
		VAlign_Top,
		VAlign_Middle,
		VAlign_Bottom
		};

class ESSimpleRenderView_TextStyle : public Serializable
{
// Construction
public:
	ESSimpleRenderView_TextStyle();
	virtual ~ESSimpleRenderView_TextStyle();

public:
	_string		m_sStyleName;
	_string		m_sFontName;
	int				m_nFontHeight;
	bool			m_bItalic;
	bool			m_bUnderLine;
	bool			m_bBold;
	Font			m_font;
public:
	virtual Serializable*						CreateSerializableObject	(){return new ESSimpleRenderView_TextStyle();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject		(){return (EnumerableObject<Serializable>*)new EnumerableObject<ESSimpleRenderView_TextStyle>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESSimpleRenderView_ImageStyle : public Serializable
{
// Construction
public:
	ESSimpleRenderView_ImageStyle();
	virtual ~ESSimpleRenderView_ImageStyle();

public:
	_string		m_sStyleName;
	_string		m_sImageFile;
	_Size			m_szImage;
	Image*			m_pImage;

public:
	virtual Serializable*						CreateSerializableObject	(){return new ESSimpleRenderView_ImageStyle();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject		(){return (EnumerableObject<Serializable>*)new EnumerableObject<ESSimpleRenderView_ImageStyle>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class RenderItem;
class RenderLine{
	public:
		RenderLine(){
			m_pNextLine		= NULL;
			m_pPrevLine		= NULL;
			m_pFirstItem	= NULL;
			m_szLine.SetSize(0, 0);
			m_bLineBreak	= false;
			}

		~RenderLine(){
			}

	virtual void	Draw(_DC* pDC, _Rect rcLine, _Rect rcDraw, ESChildControl* pControl);

	public:
		RenderLine*		m_pNextLine;
		RenderLine*		m_pPrevLine;
		RenderItem*		m_pFirstItem;
		_Size			m_szLine;
		bool			m_bLineBreak;
	};

class RenderItem{
	public:
		RenderItem(){
			m_pLine			= NULL;
			m_pNext			= NULL;
			m_pPrev			= NULL;
			m_szItem		.SetSize(0, 0);
			m_ptOffset		.SetPoint(0, 0);
			m_nVAlign		= Alignment::VAlign_Middle;
			m_nHAlign		= Alignment::HAlign_Left;
			m_fPosX			= 0.0f;
			m_fCX			= 0.0f;
			m_bBreakLine	= false;
			}

		~RenderItem(){
			}

		virtual void	Draw(_DC* pDC, _Rect rcItem, _Rect rcDraw, ESChildControl* pControl){};

	public:
		RenderLine*		m_pLine;
		RenderItem*		m_pNext;
		RenderItem*		m_pPrev;
		float			m_fPosX;		// Item start position x by float.
		float			m_fCX;			// Item width by float.
		int				m_nVAlign;
		int				m_nHAlign;
		_Point			m_ptOffset;		// Offset of item by line coords.
		_Size			m_szItem;
		bool			m_bBreakLine;	// Break line indicator.
	};

class RenderItemText : public RenderItem{
	public:
		RenderItemText() : RenderItem(){
			m_pStyle	= NULL;
			m_crText	= 0L;
			}

		~RenderItemText(){
			}

	virtual void	Draw(_DC* pDC, _Rect rcItem, _Rect rcDraw, ESChildControl* pControl);

	public:
		ESSimpleRenderView_TextStyle*	m_pStyle;
		_string							m_sText;
		_string							m_sTextLocal;
		_Size							m_szText;
		COLORREF						m_crText;
	};

class RenderItemImage : public RenderItem{
	public:
		RenderItemImage() : RenderItem(){
			m_pStyle = NULL;
			}
		~RenderItemImage(){
			}

	virtual void	Draw(_DC* pDC, _Rect rcItem, _Rect rcDraw, ESChildControl* pControl);

	public:
		ESSimpleRenderView_ImageStyle*	m_pStyle;
	};

class ESChildScrollBar;
class ESSimpleRenderView : public ESChildScrollView
{
// Construction
public:
	ESSimpleRenderView();
	virtual ~ESSimpleRenderView();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	
	void			SetBgColor				(COLORREF crBg){m_crFill = crBg;};
	bool			AddTextStyle			(_string sName, const _string sFontName, bool bItalic, bool bUnderLine, bool bBold, int nFontHeight);
	bool			AddImageStyle			(_string sName, _string sImageFile);

	void			AddText					(_string sStyleName, 
											_string sText, 
											bool bLineBreak,
											COLORREF crText, 
											_Size szText, 
											float fCX = 0.0f,  
											float fPosX = 0.0f,
											int nVAlign = VAlign_Middle, int nHAlign = HAlign_Left, bool bRedraw = false);

	bool			UpdateText				(void* pText, _string sText, COLORREF crText, bool bRedraw);

	bool			AddImage				(_string sStyleName, 
											_Size szImage, 
											float fCX = 0.0f, 
											float fPosX = 0.0f, 
											int nVAlign = VAlign_Middle, int nHAlign = HAlign_Left, bool bRedraw = false);
	void			ClearContent			(bool bRedraw);

protected:
	Serializable*	GetTextStyleByName		(const TCHAR* lpszName);
	Serializable*	GetImageStyleByName		(const TCHAR* lpszName);
	bool			CalcLineSize			(RenderLine* pLine);
	RenderLine*		GetLineByOffset			(int& nOffsetY);
	bool			IsLineVisible			(RenderLine* pLine, _Rect& rcLine);

protected:
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	virtual	void	OnMouseHover			(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave			(UINT nFlags);
	virtual void	OnMouseEnter			(UINT nFlags);
	virtual BOOL	OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnRButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnMButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp				(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void	OnRButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void	OnMButtonDblClick		(_Point pt, UINT nFlags);

	virtual void	OnDeserializationCompleted();

	virtual void	OnLocalizeStrings		();

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_renderview");};
	virtual _Size	GetControlDefaultSize	(){return _Size(250, 250);};

protected:
	EnumerableObject<ESSimpleRenderView_TextStyle>	m_listTextStyles;
	EnumerableObject<ESSimpleRenderView_ImageStyle>	m_listImageStyles;

	bool											m_bLineColors;
	COLORREF										m_crLine1;
	COLORREF										m_crLine2;
	COLORREF										m_crLineBorder;

	int												m_nLineHeightMin;
	int												m_nTextMarginLeftRight;
	int												m_nTextMarginTopBottom;
	
	// Formated text and image attributes.
	RenderLine*										m_pRootLine;
	RenderLine*										m_pLastLine;
	RenderLine*										m_pTopLine;
	int												m_nTopOffsetY;
	int												m_nTopFullOffsetY;
	int												m_nContentHeight;

protected:
	virtual Serializable*	CreateSerializableObject	(){return new ESSimpleRenderView();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Control events.
protected:
	// Variable modify callbacks.
protected:
};