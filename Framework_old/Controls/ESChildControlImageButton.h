#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlImageButton window

#include "ESChildControl.h"

class ESChildControlImageButton : public ESChildControl
{
// Construction
public:
	ESChildControlImageButton();
	virtual ~ESChildControlImageButton();

	virtual bool	Create							(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos							(int x, int y, bool bRedraw = true);
	virtual bool	SetSize							(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy							();

	bool			IsDisabled						(){return ((m_dwState&ODS_DISABLED) ? true : false);};
	void			DisableButton					(bool bRedraw);
	void			EnableButton					(bool bRedraw);

	void			SetText							(_string sText, bool bRedraw = true);
	void			SetTextColor					(COLORREF crText)	{ m_crText = crText; };
	void			SetOmitBorder					(_Rect rcOmitBorder){m_rcOmitBorder = rcOmitBorder;};
	void			AllowClickEventWhenDisabled		(bool bAllow){m_bAllowClickEventWhenDisabled = bAllow;};

protected:
	virtual void	OnPaintClient					(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual	void	OnMouseHover					(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave					(UINT nFlags);
	virtual void	OnMouseEnter					(UINT nFlags);
	virtual BOOL	OnSetCursor						(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick					(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown					(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp						(_Point pt, UINT nFlags);
	virtual bool	RemainCapturedOutOfClientArea	(){return false;};
	virtual bool	GetTooltipText					(_string& sTooltipText);


	// Retrieve region, bound size and centered flag.
	virtual RGNDef	GetControlRgn					(_Size& szRgnBound, bool& bCentered){
		if( m_bComplexRegion && m_rgnDefaultImage ){
			bCentered	= true;
			szRgnBound	= m_szRegionImage;
			return m_rgnDefaultImage;
			}
		return NULL;
		};

	virtual void	OnLocalizeStrings				();

	// Designer virtual methods
	virtual _string	GetControlDefaultName			(){return _T("_imagebutton");};
	virtual _Size	GetControlDefaultSize			(){return _Size(120, 34);};

	Image*			GetImageByState					(DWORD dwState);

protected: // Serialization.
	virtual Serializable*	CreateSerializableObject(){return new ESChildControlImageButton();};	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:

protected:
	Image				m_imageNormal;
	Image				m_imageHover;
	Image				m_imagePushed;
	Image				m_imageDisabled;

	_string             m_sImageNormal;
	_string             m_sImageHover;
	_string             m_sImagePushed;
	_string             m_sImageDisabled;

	Image				*m_pImageNormalDef;
	Image				*m_pImageHoverDef;
	Image				*m_pImagePushedDef;
	Image				*m_pImageDisabledDef;
	DWORD				m_dwState;

	bool				m_bComplexRegion;					// Complex region status of image button. 
	Rgn                 m_rgnDefaultImage;					// Default image region. Only when complex region status is true.
	_Size				m_szRegionImage;

	StringToIntArray	m_imageFillMode;

	_Rect				m_rcOmitBorder;
	bool				m_bUseHandCursor;					// User hand cursor on hover.

	_string             m_sText;
	_string             m_sTextLocal;						// Localized text string.
	Font				m_textFont;
	COLORREF			m_crText;

	bool				m_bTooltip;
	_string             m_sTooltipText;
	bool				m_bAllowClickEventWhenDisabled;
};