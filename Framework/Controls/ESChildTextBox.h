#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildTextBox window

#include "ESChildControl.h"

#define CURSOR_BLINK_TIMER	14001

class ESChildTextBox : public ESChildControl
{
// Construction
public:
	ESChildTextBox();
	virtual ~ESChildTextBox();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	VisibilityChanged		(bool bVisible){return true;};
	virtual bool	SetFocus				();
	virtual void	KillFocus				();
	virtual BOOL	OnOK					();
	virtual BOOL	OnCancel				();

	void			SetBgImage				(_Image* pImage, _Rect rcOmitBorder);
	void			SetAlternateTextFont	(_Font* pFont)				{pFont->CopyFont(&m_alternateTextFont);};
	void			SetAlternateTextColor	(COLORREF crText)			{m_crAlternateText = crText;};
	void			SetTextFont				(_Font* pFont, bool bRedraw = false);
	void			SetTextColor			(COLORREF crText, COLORREF crTextSelected){m_crText = crText; m_crSelectedText	= crTextSelected;};

	void			SetAlternateText		(_string sAlternateText){m_sAlternateText = sAlternateText;};
	void			SetPassword				(bool bPassword)			{m_bPassword = bPassword;};
	void			SetMultiLine			(bool bMultiLine)			{m_bMultiLine = bMultiLine;};
	void			SetLimitSymbolCt		(int nLimitSymbolCt)		{m_nLimitSymbolCt = nLimitSymbolCt;};
	bool			IsDisabled				()							{return m_bDisabled;};
	void			DisableTextBox			(bool bDisable)				{m_bDisabled = bDisable;};

	int				GetTextAsInt			();
	double			GetTextAsDouble			();
	DateTime		GetTextAsDateTime		();
	_string		GetText					();
	int				GetCount				();
	int				GetLineCount			()							{return (int)m_arrTextLines.size();};
	bool			IsEditable				()							{return (!m_bReadOnly && !m_bDisabled);};
	bool			IsMultiLine				()							{return (m_bMultiLine && !m_bPassword);};


	bool			SetText					(_string sText, bool bSelectAll = false, bool bRedraw = true);
	bool			SetInt					(int nValue, bool bRedraw = true);
	bool			SetDouble				(double dValue, bool bRedraw = true);
	bool			SetFloat				(float fValue, bool bRedraw = true);
	bool			SetDateTime				(DateTime dtValue, bool bRedraw = true);
	bool			SetSel					(int nSelStart, int nSelEnd, bool bChangeCursorPos, bool bRedraw);

protected:
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	virtual	void	OnMouseHover			(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave			(UINT nFlags);
	virtual BOOL	OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp				(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDblClick		(_Point pt, UINT nFlags);
	virtual void	OnTimer					(UINT nIdEvent);
	virtual void	OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual	void	OnChar					(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool	RemainCapturedOutOfClientArea(){return true;};
	virtual _Rect	GetTextAreaRect			();

	// Designer virtual methods
	virtual _string	GetControlDefaultName(){return _T("_textbox");};
	virtual _Size	GetControlDefaultSize	(){return _Size(150, 30);};
	
	int				GetCursorIndexByPoint	(_Point pt, int& nLineIndex);
	bool			ClearSelection			(bool bRedraw);
	
	void			CalcTextDimensions		(_string* pStrText);
	void			CalcTextMaxOffsets		();
	int				GetTextWidth			(int nLine, int nStartIndex, int nEndIndex);
	bool			GetTextRect				(int nLine, int nStartIndex, int nEndIndex, _Rect& rcText);
	int				GetTextSymbolCount		(_string& sText, int& nLineCt);

	int				CalcMaxLeftOffsetOfLine	(int nLine, int& nLeftSymbolOffsetX);

	int				GetCursorPosByCharIndex	(int nCharIndex);
	int				GetCharIndexByCursorPos	(int nCursorPos);

	int				GetLineSymbolCt			(int nLine);
	bool			GetLineString			(int nLine, _string& sLine);

	bool			EnsureCursorVisibility	(bool& bRedrawAll);

	bool			GetCursorRect			(_Rect& rcCursor);
	bool			SetCursorPos			(int nSymbolIndex, bool bRedraw);

	// Clipboard. {{
	bool			CopySelectedString		();
	bool			CutSelectedString		(bool bCopyInClipboard, bool bRedraw);
	bool			PasteString				(bool bRedraw);
	// }}

private:
	bool			GetSelectedString		(_string& sText);
	int				GetCompleteWordByIndex	(int nCursorPos, _string& sWord);
	bool			InsertStringAtIndex		(int nCursorPos, _string sInsert, bool bRedraw);
	bool			DeleteCharAtIndex		(int nCursorPos, bool bRedraw);

	bool			GetTextSize				(_string* pStr, _Font* pFont, _Size& szText);
	bool			ExtTextOutMy			(_DC* pDC, int x, int y, UINT nOptions, RECT* lpRect, _string sText);
	int				GetSymbolIndexByXOffset	(_string* pStr, _Font* pFont, int nXTextStart,  int nXSymbolAt);
	int				GetSymbolWidthArray		(_string* pStr, _Font* pFont, CDWordArray& arrSymbolWidth);

protected:
	_Image					m_imageBg;				// Background image.
	_Rect					m_rcOmitBorder;			// Omit border of bg image.
	COLORREF				m_crBorder;				// Border color.
	COLORREF				m_crFill;				// Fill color.
	
	_Font					m_textFont;				// Text font.
	_Font					m_textFontOriginal;		// Text font original.
	COLORREF				m_crText;				// Text color.
	COLORREF				m_crSelectedText;		// Selected text color.
	COLORREF				m_crSelectedTextBg;		// Selected text background color.
	
	_Font					m_alternateTextFont;	// Alternate text font.
	_string                 m_sAlternateText;		// Alternate text.
	COLORREF				m_crAlternateText;		// Alternate Text color.

	float					m_fZoomCX;
	float					m_fZoomCY;

	bool					m_bReadOnly;			// Readonly text box.
	bool					m_bPassword;			// Password text box.
	bool					m_bMultiLine;			// Multiline textbox specification.
	int						m_nLimitSymbolCt;		// Limited symbol count.
	bool					m_bDisabled;			// Disabled state of textbox.		

	// In multiline mode it is like: [line index][symbol index]
	int						m_nCursorPos;			// Cursor index position.
	int						m_nCursorWidth;			// Cursor width.
	bool					m_bCursorIsVisible;		// Cusror visibility.

	_string					m_sText;				// Original text string.
	_string					m_sRegex;				// Regex text.

	CStringArray			m_arrTextLines;			// Text line strings.
	CDWordArray				m_arrLineWidth;			// Array of text line width.
	int						m_nTextLineHeight;		// Text line height.
	int						m_nSymbolCt;			// Symbol count.
	bool					m_bCalcSymbolCt;		// Symbols count calculation flag.

	int						m_nTopLine;				// Top line index.
	int						m_nTopLineOffsetY;		// Line top offset.
	int						m_nLeftIndex;			// Left symbol index.
	int						m_nLeftOffsetX;			// Line left offset.

	int						m_nMaxTopLine;
	int						m_nMaxTopLineOffsetY;
	int						m_nMaxLeftIndex;
	int						m_nMaxLeftOffsetX;
	
	// Text selection.
	// For multiline textbox symbol index parameter have such format. [line index][symbol index]
	int						m_nSelStart;			// Selection starting char index.
	int						m_nSelEnd;				// Selection end char index.
	
	// Close box
	_Point					m_ptHoverPos;
	BOOL					m_bCloseBox;
	_Rect					m_rcCloseBox;
	_Image*					m_pImageCloseBoxNormal;
	_Image*					m_pImageCloseBoxHover;
	_Image*					m_pImageCloseBoxPushed;

public: // Serialization.	
	virtual Serializable*	CreateSerializableObject(){return new ESChildTextBox();};
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	TextModify		(ESChildTextBox* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	TextFontModify	(ESChildTextBox* pThis, VariableInfo* pInfo, void* lpValueNew);
};
