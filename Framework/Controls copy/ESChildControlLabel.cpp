// ESChildControlLabel.cpp : implementation file
//

#include "ESChildControlLabel.h"
#include "../Utility/StringHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlLabel

ESChildControlLabel::ESChildControlLabel(){
	m_sClassName				= _T("ESChildControlLabel");
	m_sLabel					= _T("");
	m_bAllowMouseEvents			= false;
	m_bUseHandMouse				= false;

	MercuryGUI* pGUI			= MercuryGUI::GetInstance();
	m_crFill					= pGUI->label.m_crFill;
	m_crText					= pGUI->label.m_crText;
	pGUI->label.m_textFont.CopyFont(&m_fontText);

	m_rcBgImageOmitBorder.SetRect(0, 0, 0, 0);
	m_bSizeTextFont				= true;
	m_bFillTextBackgroundOnly	= true;

	m_fillMode			.Add(_T("None"),				_FillMode::FillMode_None);
	m_fillMode			.Add(_T("Color"),				_FillMode::FillMode_Color);
	m_fillMode			.Add(_T("CentredImage"),		_FillMode::FillMode_CentredImage);
	m_fillMode			.Add(_T("StretchImage"),		_FillMode::FillMode_StretchImage);
	m_fillMode			.Add(_T("RepeatedImage"),		_FillMode::FillMode_RepeatedImage);
	m_fillMode			.Add(_T("OmitBorderedImage"),	_FillMode::FillMode_OmitBorderedImage);
	m_fillMode			.SetCurrentIndex(0);

	m_textVAlignement	.Add(_T("Top"),					VAlignment::VAlignement_Top);
	m_textVAlignement	.Add(_T("Center"),				VAlignment::VAlignement_Center);
	m_textVAlignement	.Add(_T("Bottom"),				VAlignment::VAlignement_Bottom);
	m_textVAlignement	.SetCurrentIndex(1);
	m_textHAlignement	.Add(_T("Left"),				HAlignment::HAlignement_Left);
	m_textHAlignement	.Add(_T("Middle"),				HAlignment::HAlignement_Middle);
	m_textHAlignement	.Add(_T("Right"),				HAlignment::HAlignement_Right);
	m_textHAlignement	.SetCurrentIndex(1);
    
    AutoSortedArray* pArr = GetArrayOfVariableInfosByName();
    pArr->GetCount();
	}

BEGIN_DECLARE_VAR(ESChildControlLabel, ESChildControl)
DECLARE_VAR(_T("ESChildControlLabel"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Text Font"),				_T(""), VariableType::VT_Font, offsetof(ESChildControlLabel,m_fontText), false, false, false)
	DECLARE_VAR(_T("Text Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlLabel,m_crText), false, false, false)
	DECLARE_VAR(_T("Label"),					_T(""), VariableType::VT_String, offsetof(ESChildControlLabel,m_sLabel), false, false, false)
	DECLARE_VAR(_T("FillMode"),					_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlLabel,m_fillMode), false, false, false)
	DECLARE_VAR(_T("Fill Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlLabel,m_crFill), false, false, false)
	DECLARE_VAR(_T("BgImage"),					_T(""), VariableType::VT_Image, offsetof(ESChildControlLabel,m_bgImage), false, false, false)
	DECLARE_VAR(_T("BgImageFile"),				_T(""), VariableType::VT_String, offsetof(ESChildControlLabel,m_sBgImageFile), false, false, false)
	DECLARE_VAR(_T("ImageOmitBorder"),			_T(""), VariableType::VT_Rect, offsetof(ESChildControlLabel,m_rcBgImageOmitBorder), false, false, false)
	DECLARE_VAR(_T("Fill Text Bg Only"),		_T(""), VariableType::VT_Bool, offsetof(ESChildControlLabel,m_bFillTextBackgroundOnly), false, false, false)
	DECLARE_VAR(_T("VAlignement"),				_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlLabel,m_textVAlignement), false, false, false)
	DECLARE_VAR(_T("HAlignement"),				_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlLabel,m_textHAlignement), false, false, false)
	DECLARE_VAR(_T("UseHandMouse"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlLabel,m_bUseHandMouse), false, false, false)
END_DECLARE_VAR()

ESChildControlLabel::~ESChildControlLabel(){
	m_fontText	.DeleteObject();
	m_bgImage	.Destroy();
	}

bool
ESChildControlLabel::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet && m_pOwner->IsDesignerMode() ){
		// Register variable modify callback methods. {{
		AddVariableModifyCallBack(&m_fontText,					(VariableValueModifyCallback)&ESChildControlLabel::FontModify);
		AddVariableModifyCallBack(&m_crText,					(VariableValueModifyCallback)&ESChildControlLabel::TextColorModify);
		AddVariableModifyCallBack(&m_crFill,					(VariableValueModifyCallback)&ESChildControlLabel::FillColorModify);
		AddVariableModifyCallBack(&m_sLabel,					(VariableValueModifyCallback)&ESChildControlLabel::LabelTextModify);
		AddVariableModifyCallBack(&m_fillMode,					(VariableValueModifyCallback)&ESChildControlLabel::FillModeModify);
		AddVariableModifyCallBack(&m_bgImage,					(VariableValueModifyCallback)&ESChildControlLabel::BgImageModify);
		AddVariableModifyCallBack(&m_sBgImageFile,				(VariableValueModifyCallback)&ESChildControlLabel::BgImageModify);
		AddVariableModifyCallBack(&m_bFillTextBackgroundOnly,	(VariableValueModifyCallback)&ESChildControlLabel::FillTextBgOnlyModify);
		AddVariableModifyCallBack(&m_rcBgImageOmitBorder,		(VariableValueModifyCallback)&ESChildControlLabel::OmitBorderModify);
		AddVariableModifyCallBack(&m_textVAlignement,			(VariableValueModifyCallback)&ESChildControlLabel::VAlignModify);
		AddVariableModifyCallBack(&m_textHAlignement,			(VariableValueModifyCallback)&ESChildControlLabel::HAlignModify);
		// }}
		}

	if( bRet )
		OnLocalizeStrings();
	return bRet;
	}

bool
ESChildControlLabel::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlLabel::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlLabel::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlLabel::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

void
ESChildControlLabel::SetLabel(_string sText, bool bRedraw){
	m_sLabel		= sText;
	m_sLabelLocal	= sText;
	if( !m_pOwner->IsDesignerMode() )
		ESLocalizationMan::ParseText(m_sLabelLocal);

	if( bRedraw ) 
		Redraw();
	}

void
ESChildControlLabel::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
    if( !m_pOwner->IsDesignerMode() && !m_sLabelLocal.length() )
        return;
    float fZoomCX = 1.0f, fZoomCY = 1.0f;
    m_pOwner->GetZoom(fZoomCX, fZoomCY);
    
    // Calculate lines.
    CStringArray	arrLines;
    _string			sLine, sLabel = m_sLabelLocal;
    StringHelper::Split(&sLabel, _T("\r\n"), arrLines);
    
    _Font*	pFontOld	= NULL;
    _Font	font;
    _Font*	pFont		= &m_fontText;
    if( m_bSizeTextFont && !m_fontText.IsNull() ){
        m_fontText.StretchedTextFont(&font, fZoomCX, fZoomCY);
        pFontOld		= pDC->SelectObject(&font);
        pFont			= &font;
    }
    else
        pFontOld		= pDC->SelectObject(&m_fontText);
    
    // Fill background. {{
    _Rect	rcBkDC, rcInvalid;
    rcBkDC.left		= (m_rRect.left - pRectClient->left) + pRectDC->left;
    rcBkDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top;
    rcBkDC.right	= rcBkDC.left + m_rRect.Width();
    rcBkDC.bottom	= rcBkDC.top + m_rRect.Height();
    
    _Size szSymbol;
    sLine					= _T("X");
    pFont->GetTextSize(&sLine, szSymbol);
    //	ImageHelper::GetTextSizeEmbedeed(&sLine, pFont, 0, szSymbol);
    int nHeightTextLines	= ((int)arrLines.size())*szSymbol.cy + (arrLines.size() - 1);
    
    if( m_bFillTextBackgroundOnly ){
        rcBkDC.bottom	= std::min(rcBkDC.bottom, rcBkDC.top + nHeightTextLines);
        if( m_textVAlignement.GetCurrentValue() == VAlignment::VAlignement_Center ){
            int nAdd = (GetClientHeight() -  rcBkDC.Height())/2;
            rcBkDC.top		+= nAdd;
            rcBkDC.bottom	+= nAdd;
        }
    }
    
    _Image* pBgImage = &m_bgImage;
    if( pBgImage->IsNull() ){
        MercuryGUI* pGUI	= MercuryGUI::GetInstance();
        pBgImage			= pGUI->LoadImage(m_sBgImageFile);
    }
    m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, GetClientRect(), m_fillMode.GetCurrentValue(), m_crFill, pBgImage, m_rcBgImageOmitBorder, false, ScaleMode_Bilinier);
    // }}
    
    // Draw text. {{
    _Size	szText;
    int		nOffsetY	= m_rRect.top + 2;
    if( m_textVAlignement.GetCurrentValue() == VAlignment::VAlignement_Center ){
        nOffsetY = m_rRect.top + (m_rRect.Height() - nHeightTextLines)/2;
    }
    
    for( int nLoop=0; nLoop<arrLines.size(); nLoop++ ){
        sLine = arrLines.at(nLoop);
        pFont->GetTextSize(&sLine, szText);
        
        _Rect	rcLabel;
        if( m_textHAlignement.GetCurrentValue() == HAlignment::HAlignement_Middle ){
            if( szText.cx <= m_rRect.Width() )
                rcLabel.left = m_rRect.left + (m_rRect.Width() - szText.cx)/2;
            else
                rcLabel.left = m_rRect.left + 2;
        }
        else
            rcLabel.left	= m_rRect.left + 2;
        
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
        pDC->ExtTextOut    (ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, sLine.c_str());
        nOffsetY	+=	szText.cy;
    }
    
    pDC->SelectObject	(pFontOld);
    font.DeleteObject	();
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    
    ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildControlLabel::OnMouseHover(_Point pt, UINT nFlags){
	}

BOOL	
ESChildControlLabel::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	if( m_bUseHandMouse ){
//		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
		}
	return FALSE;
	}

void	
ESChildControlLabel::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlLabel::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlLabel::OnLButtonDown(_Point pt, UINT nFlags){
	}

void
ESChildControlLabel::OnLButtonUp(_Point pt, UINT nFlags){
	}

void	
ESChildControlLabel::OnLButtonClick(_Point pt, UINT nFlags){
	SendEvent(this, ChildControlEventCode::EventCode_ControlClick);
	}

void	
ESChildControlLabel::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlLabel::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlLabel::OnLButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlLabel::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlLabel::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void
ESChildControlLabel::OnLocalizeStrings(){
	m_sLabelLocal = m_sLabel;
	if( !m_pOwner->IsDesignerMode() )
		ESLocalizationMan::ParseText(m_sLabelLocal);
	}

/////////////////////////////////////////////////////////////////////////////
// Variable modify callbacks.

bool
ESChildControlLabel::FontModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::TextColorModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::FillColorModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::LabelTextModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::FillModeModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::BgImageModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::OmitBorderModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::FillTextBgOnlyModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::VAlignModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}

bool
ESChildControlLabel::HAlignModify(ESChildControlLabel* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}
