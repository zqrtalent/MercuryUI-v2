// ESChildControlImageButton.cpp : implementation file
//

#include "ESChildControlImageButton.h"
#include "../Utility/StringHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlImageButton

ESChildControlImageButton::ESChildControlImageButton(){
	m_sClassName		= _T("ESChildControlImageButton");
	m_dwState			= 0;
	m_sText				= _T("");
	
	MercuryGUI* pGUI	= MercuryGUI::GetInstance();
	m_rcOmitBorder		= pGUI->imagebutton.m_rcOmitBorder;
	m_pImageNormalDef	= &pGUI->imagebutton.m_imageNomal;
	m_pImageHoverDef	= &pGUI->imagebutton.m_imageHover;
	m_pImagePushedDef	= &pGUI->imagebutton.m_imagePushed;
	m_pImageDisabledDef	= &pGUI->imagebutton.m_imageDisabled;

	m_imageFillMode	.Add(_T("CentredImage"),		FillMode::FillMode_CentredImage);
	m_imageFillMode	.Add(_T("StretchImage"),		FillMode::FillMode_StretchImage);
	m_imageFillMode	.Add(_T("RepeatedImage"),		FillMode::FillMode_RepeatedImage);
	m_imageFillMode	.Add(_T("OmitBorderedImage"),	FillMode::FillMode_OmitBorderedImage);
	m_imageFillMode	.SetCurrentIndex(3);

	m_bUseHandCursor				= true;
	m_bTooltip						= false;
	m_bComplexRegion				= false;
	m_bAllowClickEventWhenDisabled	= false;
	m_szRegionImage.SetSize(0, 0);

	LOGFONT lf;
	pGUI->imagebutton.m_textFont.GetLogFont(&lf);
	m_textFont.CreateFontIndirect(&lf);
	m_crText			= pGUI->imagebutton.m_crText;
	}

BEGIN_DECLARE_VAR(ESChildControlImageButton, ESChildControl)
DECLARE_VAR(_T("ESChildControlImageButton"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ImageNormal"),				_T(""), VariableType::VT_Image, offsetof(ESChildControlImageButton,m_imageNormal), false, false, false)
	DECLARE_VAR(_T("ImageHover"),				_T(""), VariableType::VT_Image, offsetof(ESChildControlImageButton,m_imageHover), false, false, false)
	DECLARE_VAR(_T("ImagePushed"),				_T(""), VariableType::VT_Image, offsetof(ESChildControlImageButton,m_imagePushed), false, false, false)
	DECLARE_VAR(_T("ImageDisabled"),			_T(""), VariableType::VT_Image, offsetof(ESChildControlImageButton,m_imageDisabled), false, false, false)

	DECLARE_VAR(_T("ImageFileNormal"),			_T(""), VariableType::VT_String, offsetof(ESChildControlImageButton,m_sImageNormal), false, false, false)
	DECLARE_VAR(_T("ImageFileHover"),			_T(""), VariableType::VT_String, offsetof(ESChildControlImageButton,m_sImageHover), false, false, false)
	DECLARE_VAR(_T("ImageFilePushed"),			_T(""), VariableType::VT_String, offsetof(ESChildControlImageButton,m_sImagePushed), false, false, false)
	DECLARE_VAR(_T("ImageFileDisabled"),		_T(""), VariableType::VT_String, offsetof(ESChildControlImageButton,m_sImageDisabled), false, false, false)
	DECLARE_VAR(_T("ComplexRgnImage"),			_T(""), VariableType::VT_Bool, offsetof(ESChildControlImageButton,m_bComplexRegion), false, false, false)

	DECLARE_VAR(_T("OmitBorder"),				_T(""), VariableType::VT_Rect, offsetof(ESChildControlImageButton,m_rcOmitBorder), false, false, false)
	DECLARE_VAR(_T("Image Fill Mode"),			_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlImageButton,m_imageFillMode), false, false, false)
	DECLARE_VAR(_T("Text"),						_T(""), VariableType::VT_String, offsetof(ESChildControlImageButton,m_sText), false, false, false)
	DECLARE_VAR(_T("Font"),						_T(""), VariableType::VT_Font, offsetof(ESChildControlImageButton,m_textFont), false, false, false)
	DECLARE_VAR(_T("Text Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlImageButton,m_crText), false, false, false)
	DECLARE_VAR(_T("Tooltip"),					_T(""), VariableType::VT_Bool, offsetof(ESChildControlImageButton,m_bTooltip), false, false, false)
	DECLARE_VAR(_T("Tooltip Text"),				_T(""), VariableType::VT_String, offsetof(ESChildControlImageButton,m_sTooltipText), false, false, false)
	DECLARE_VAR(_T("Use Hand Cursor"),			_T(""), VariableType::VT_Bool, offsetof(ESChildControlImageButton,m_bUseHandCursor), false, false, false)
END_DECLARE_VAR()

ESChildControlImageButton::~ESChildControlImageButton(){
	//if( m_rgnDefaultImage )
	//	::DeleteObject(m_rgnDefaultImage);
	}

bool
ESChildControlImageButton::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		OnLocalizeStrings();

		// Create region from image. {{
		if( m_bComplexRegion && !m_pOwner->IsDesignerMode() ){
			m_rgnDefaultImage.DeleteObject();

			Image* pDefImage = GetImageByState(ODS_DEFAULT);
			if( pDefImage && !pDefImage->IsNull() ){
				RGNDef hRgn = pDefImage->GetBitmap32bppRgnByAlphaChannel();
				if( hRgn )
					m_rgnDefaultImage.Attach(hRgn);
				m_szRegionImage.SetSize(pDefImage->GetWidth(), pDefImage->GetHeight());
				}
			}
		// }}
		}
	return bRet;
	}

bool
ESChildControlImageButton::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlImageButton::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlImageButton::Destroy(){
	if( ESChildControl::Destroy() ){
		if( !m_imageNormal.IsNull() )
			m_imageNormal.Destroy();
		if( !m_imageHover.IsNull() )
			m_imageHover.Destroy();
		if( !m_imagePushed.IsNull() )
			m_imagePushed.Destroy();
		if( !m_imageDisabled.IsNull() )
			m_imageDisabled.Destroy();
		m_textFont.DeleteObject();
		}
	return true;
	}

Image*
ESChildControlImageButton::GetImageByState(DWORD dwState){
	MercuryGUI* pGUI	= MercuryGUI::GetInstance();
	if( dwState&ODS_SELECTED ){
		if( m_imagePushed.IsNull() ){
			if( m_sImagePushed.length() ){
				Image* pImage = pGUI->LoadImage(m_sImagePushed);
				if( pImage )
					return pImage;
				}
			return m_pImagePushedDef;
			}
		return &m_imagePushed;
		}
	if( dwState&ODS_DISABLED ){
		if( m_imageDisabled.IsNull() ){
			if( m_sImageDisabled.length() ){
				Image* pImage = pGUI->LoadImage(m_sImageDisabled);
				if( pImage )
					return pImage;
				}
			return m_pImageDisabledDef;
			}
		return &m_imageDisabled;
		}
	if( dwState&ODS_HOTLIGHT ){
		if( m_imageHover.IsNull() ){
			if( m_sImageHover.length() ){
				Image* pImage = pGUI->LoadImage(m_sImageHover);
				if( pImage )
					return pImage;
				}
			return m_pImageHoverDef;
			}
		return &m_imageHover;
		}
	if( m_imageNormal.IsNull() ){
		if( m_sImageNormal.length() ){
			Image* pImage = pGUI->LoadImage(m_sImageNormal);
			if( pImage )
				return pImage;
			}
		return m_pImageNormalDef;
		}
	return &m_imageNormal;
	}

bool
ESChildControlImageButton::GetTooltipText(_string& sTooltipText){
	if( m_bTooltip && m_sTooltipText.length() ){
		sTooltipText = m_sTooltipText;
		return true;
		}
	return false;
	}

void
ESChildControlImageButton::DisableButton(bool bRedraw){
	if( IsDisabled() )
		return;
	m_dwState = ODS_DISABLED;
	if( bRedraw )
		Redraw();
	}

void
ESChildControlImageButton::EnableButton(bool bRedraw){
	if( !IsDisabled() )
		return;
	m_dwState = ODS_DEFAULT;
	if( bRedraw )
		Redraw();
	}

void
ESChildControlImageButton::SetText(_string sText, bool bRedraw){ 
	m_sText = sText; 
	OnLocalizeStrings();
	if( bRedraw )
		Redraw(true);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlImageButton::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	Image*		pImage	= GetImageByState(m_dwState);
	float		fZoomCX = 1.0f;
	float		fZoomCY = 1.0f;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	if( pImage && !pImage->IsNull() ){
		_Rect rcClient	= GetClientRect();
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, m_imageFillMode.GetCurrentValue(), RGB(0, 0, 0), pImage, m_rcOmitBorder, false, ScaleMode_Bilinier);
		}

	// Draw text. {{
	if( m_sTextLocal.length() ){
		CStringArray	arrLines;
		_string			sLine, sLabel = m_sTextLocal;
		StringHelper::Split(&sLabel, _T("\r\n"), arrLines);

		Font textFont;
		m_textFont.StretchedTextFont(&textFont, fZoomCX, fZoomCY);
		Font* pFont = &textFont;

		_Size szSymbol;
		sLine					= _T("X");
		pFont->GetTextSize(&sLine, szSymbol);
		int nHeightTextLines	= arrLines.size()*szSymbol.cy;// + (arrLines.GetCount() - 1);

		// Draw text. {{
		_Size	szText;
		int		nOffsetY		= m_rRect.top + (m_rRect.Height() - nHeightTextLines)/2;

		Font*	pFontOld = pDC->SelectObject(pFont);
		for( int nLoop=0; nLoop<arrLines.size(); nLoop++ ){
			sLine = arrLines.at(nLoop);
			pFont->GetTextSize(&sLine, szText);

			_Rect	rcLabel;
			rcLabel.left	= m_rRect.left + (m_rRect.Width() - szText.cx)/2;
			rcLabel.top		= nOffsetY;
			rcLabel.right	= rcLabel.left + szText.cx;
			rcLabel.bottom	= rcLabel.top + szText.cy;

			// Translate from window pixel to DC pixel.{{
			_Point ptLabel;
			ptLabel.x = (rcLabel.left - pRectClient->left) + pRectDC->left; 
			ptLabel.y = (rcLabel.top - pRectClient->top) + pRectDC->top; 
			//}}

			pDC->SetBkMode		(TRANSPARENT);
			pDC->SetTextColor	(m_crText);
			pDC->ExtTextOut     (ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, sLine.c_str());
			nOffsetY	+=	szText.cy;
			}
		pDC->SelectObject(pFontOld);
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		/*
		CFont textFont;
		ImageHelper::StretchedTextFont(&m_textFont, &textFont, fZoomCX, fZoomCY);
		CFont* pFont = &textFont;

		_Rect rcControlDC;
		rcControlDC.left	= (m_rRect.left - pRectClient->left) + pRectDC->left; 
		rcControlDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top; 
		rcControlDC.right	= rcControlDC.left + m_rRect.Width(); 
		rcControlDC.bottom	= rcControlDC.top + m_rRect.Height(); 

		CFont*	pFontOld = pDC->SelectObject(pFont);
		_Size	szText;
		ImgHlp::GetTextSize(&m_sTextLocal, pFont, szText);

		_Point ptLabel		(rcControlDC.left + (rcControlDC.Width() - szText.cx)/2, rcControlDC.top + (rcControlDC.Height() - szText.cy)/2);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(m_crText);
		ImageHelper::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, m_sTextLocal);
		//ImageHelper::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcControlDC, m_sTextLocal);
		pDC->SelectObject	(pFontOld);
		*/
		}
	// }}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildControlImageButton::OnMouseHover(_Point pt, UINT nFlags){
	}

BOOL	
ESChildControlImageButton::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	if( !IsDisabled() && m_bUseHandCursor ){
		//SetCursor(LoadCursor(NULL, IDC_HAND));
		//return 1;
		}
	return FALSE;
	}

void	
ESChildControlImageButton::OnMouseLeave(UINT nFlags){
	if( !IsDisabled() ){
		m_dwState = ODS_DEFAULT;
		Redraw();
		}
	}

void	
ESChildControlImageButton::OnMouseEnter(UINT nFlags){
	if( !IsDisabled() ){
		m_dwState = ODS_HOTLIGHT;
		Redraw();
		}
	}

void
ESChildControlImageButton::OnLButtonDown(_Point pt, UINT nFlags){
	if( !IsDisabled() ){
		if( m_bComplexRegion ){
			if( MercuryBaseView::PtInImageRegion(pt, this, m_rgnDefaultImage, m_szRegionImage, true) ){
				m_dwState = ODS_SELECTED;
				Redraw();
				return;
				}
			}
		else{
			m_dwState = ODS_SELECTED;
			Redraw();
			}
		}
	}

void
ESChildControlImageButton::OnLButtonUp(_Point pt, UINT nFlags){
	if( !IsDisabled() ){
		if( m_bComplexRegion ){
			if( MercuryBaseView::PtInImageRegion(pt, this, m_rgnDefaultImage, m_szRegionImage, true) )
				m_dwState = ODS_HOTLIGHT;
			else
				m_dwState = ODS_DEFAULT;
			}
		else{
			if( m_rRect.PtInRect(pt) )
				m_dwState = ODS_HOTLIGHT;
			else
				m_dwState = ODS_DEFAULT;
			}
		Redraw();
		}
	}

void	
ESChildControlImageButton::OnLButtonClick(_Point pt, UINT nFlags){
	if( IsDisabled() && !m_bAllowClickEventWhenDisabled ) return;
	if( m_bComplexRegion ){
		if( MercuryBaseView::PtInImageRegion(pt, this, m_rgnDefaultImage, m_szRegionImage, true) )
			SendEvent(this, EventCode_ImageButtonClick);
		}
	else
		SendEvent(this, EventCode_ImageButtonClick);
	}

void
ESChildControlImageButton::OnLocalizeStrings(){
	m_sTextLocal = m_sText;
	if( !m_pOwner->IsDesignerMode() )
		ESLocalizationMan::ParseText(m_sTextLocal);
	}