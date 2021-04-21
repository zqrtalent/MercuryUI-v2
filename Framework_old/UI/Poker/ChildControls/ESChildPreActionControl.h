#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildPreActionControl window

#include "..\..\..\Controls\ESChildControl.h"

class ESChildPreActionControl : public ESChildControl
{
// Construction
public:
	ESChildPreActionControl();
	virtual ~ESChildPreActionControl();

	virtual bool	Create					(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetTextFont				(Font* pFont);
	void			SetLabel				(std::string sLabel, bool bRedraw);
	void			SetActive				(bool bActive, bool bRedraw = true)	{m_bActiveState = bActive; if( bRedraw ) Redraw();};
	bool			GetActive				() { return m_bActiveState; };

protected:
	virtual void	OnPaintClient			(_DC* pDC, Rect* pRectDC, Rect* pRectClient);
	virtual	void	OnLButtonClick			(Point pt, UINT nFlags);
	virtual void	OnLocalizeStrings		();

	// Designer virtual methods
	virtual std::string	GetControlDefaultName	(){return _T("_preaction");};
	virtual Size	GetControlDefaultSize	(){return Size(100, 50);};

	Image*			GetActiveImage			();
	Image*			GetDefaultImage			();

protected:
	Font				m_fontText;					// Text font.
	COLORREF			m_crTextDefault;			// Text color default.
	COLORREF			m_crTextActive;				// Text color active.
	std::string			m_sLabel;					// Label text.
	std::string			m_sLabelLocal;				// Localized label text.

	Image				m_imgActiveBg;				// Active state bg image.
	Image				m_imgDefaultBg;				// Default state bg image.
	std::string			m_sActiveBgImageFile;		// Active state bg image.
	std::string			m_sDefaultBgImageFile;		// Default state bg image.
	Rect				m_rcOmitborder;				// Image omitborder.

	int					m_nTextLeftOffset;			// Left offset of text.

	// Attributes that's are used in scale mode. {{
	int					m_nTextLeftOffsetScaled;		
	Image				m_imageActiveBg_Scaled;
	Image				m_imagDefaultBg_Scaled;
	Rect				m_rcOmitborder_Scaled;
	// }}

	bool				m_bActiveState;				// Active state.

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildPreActionControl();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	AttributeModify	(ESChildPreActionControl* pThis, VariableInfo* pInfo, void* lpValueNew);
};