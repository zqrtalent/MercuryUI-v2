#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlLabel window

#include "ESChildControl.h"

class ESChildControlLabel : public ESChildControl
{
// Construction
public:
	ESChildControlLabel();
	virtual ~ESChildControlLabel();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetTextColor			(COLORREF crText) { m_crText = crText; };
	void			SetLabel				(_string sText, bool bRedraw);
	void			SetCentred				(bool bCentred, bool bVCentred = false){/*m_bCentred = bCentred; m_bVCentred = bVCentred;*/};
	void			SetBkImage				(Image* pImageBk, _Rect rcOmitBorder){/*m_pImageBk = pImageBk; m_rcBgImageOmitBorder = rcOmitBorder;*/};
	void			SetSizeTextFont			(bool bSizeTextFont){m_bSizeTextFont = bSizeTextFont;};
	void			SetFillColor			(COLORREF crFill){m_crFill = crFill;};
	_string         GetLabel				(){return m_sLabelLocal;};

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

	virtual void	OnLocalizeStrings		();

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_label");};
	virtual _Size	GetControlDefaultSize	(){return _Size(100, 50);};

protected:
	Font				m_fontText;					// Text font.
	COLORREF			m_crText;					// Text color.
	COLORREF			m_crFill;					// Background fill color.
	_string             m_sLabel;					// Label text.
	_string             m_sLabelLocal;				// Label text localized.
	bool				m_bUseHandMouse;			// Use hand mouse over control.

	StringToIntArray	m_fillMode;					// Fill mode.
	Image				m_bgImage;					// Background image.
	_string             m_sBgImageFile;				// Background image file.

	_Rect				m_rcBgImageOmitBorder;		// Background image omitborder rect.
	bool				m_bFillTextBackgroundOnly;	// Fill only text background.
	StringToIntArray	m_textVAlignement;			// Vertical alignement.
	StringToIntArray	m_textHAlignement;			// Horizontal alignement.

	bool				m_bSizeTextFont;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlLabel();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	FontModify					(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	TextColorModify				(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	FillColorModify				(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	LabelTextModify				(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	FillModeModify				(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	BgImageModify				(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	OmitBorderModify			(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	FillTextBgOnlyModify		(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	VAlignModify				(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	HAlignModify				(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew);
};