// ESChildPreActionControl.cpp : implementation file
//

#include "stdafx.h"
#include "ESChildPreActionControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildPreActionControl

ESChildPreActionControl::ESChildPreActionControl(){
	m_sClassName				= _T("ESChildPreActionControl");
	m_sLabel					= _T("Pre Action");
	m_bAllowMouseEvents			= false;
	m_bActiveState				= false;
	m_crTextDefault				= RGB(255, 255, 255);
	m_crTextActive				= RGB(255, 255, 255);
	m_rcOmitborder.SetRect(0, 0, 0, 0);
	m_nTextLeftOffset			= 3;
	}

BEGIN_DECLARE_VAR(ESChildPreActionControl, ESChildControl)
DECLARE_VAR(_T("ESChildPreActionControl"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Text Font"),					_T(""), VariableType::VT_Font, offsetof(ESChildPreActionControl,m_fontText), false, false, false)
	DECLARE_VAR(_T("Active Text"),					_T(""), VariableType::VT_Color32, offsetof(ESChildPreActionControl,m_crTextActive), false, false, false)
	DECLARE_VAR(_T("Default Text"),					_T(""), VariableType::VT_Color32, offsetof(ESChildPreActionControl,m_crTextDefault), false, false, false)
	DECLARE_VAR(_T("Label"),						_T(""), VariableType::VT_String, offsetof(ESChildPreActionControl,m_sLabel), false, false, false)
	DECLARE_VAR(_T("ActiveBgImage"),				_T(""), VariableType::VT_Image, offsetof(ESChildPreActionControl,m_imgActiveBg), false, false, false)
	DECLARE_VAR(_T("DefaultBgImage"),				_T(""), VariableType::VT_Image, offsetof(ESChildPreActionControl,m_imgDefaultBg), false, false, false)
	DECLARE_VAR(_T("Active Bg Image File"),			_T(""), VariableType::VT_String, offsetof(ESChildPreActionControl,m_sActiveBgImageFile), false, false, false)
	DECLARE_VAR(_T("Default Bg Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildPreActionControl,m_sDefaultBgImageFile), false, false, false)
	DECLARE_VAR(_T("Bg image Omitborder"),			_T(""), VariableType::VT_Rect, offsetof(ESChildPreActionControl,m_rcOmitborder), false, false, false)
	DECLARE_VAR(_T("Active state"),					_T(""), VariableType::VT_Bool, offsetof(ESChildPreActionControl,m_bActiveState), false, false, false)
	DECLARE_VAR(_T("Left Offset"),					_T(""), VariableType::VT_Int32, offsetof(ESChildPreActionControl,m_nTextLeftOffset), false, false, false)
END_DECLARE_VAR()

ESChildPreActionControl::~ESChildPreActionControl(){
	m_fontText		.DeleteObject();
	m_imgActiveBg	.Destroy();
	m_imgDefaultBg	.Destroy();
	}

bool
ESChildPreActionControl::Create(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet && m_pOwner->IsDesignerMode() ){
		// Register variable modify callback methods. {{
		AddVariableModifyCallBack(&m_fontText,					(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_crTextDefault,				(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_crTextDefault,				(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_sLabel,					(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_imgActiveBg,				(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_imgDefaultBg,				(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_sActiveBgImageFile,		(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_sDefaultBgImageFile,		(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		AddVariableModifyCallBack(&m_bActiveState,				(VariableValueModifyCallback)&ESChildPreActionControl::AttributeModify);
		// }}
		}
	if( bRet )
		OnLocalizeStrings();
	return bRet;
	}

bool
ESChildPreActionControl::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildPreActionControl::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);

	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);
	m_nTextLeftOffsetScaled = fZoomCX*m_nTextLeftOffset;
	}

void
ESChildPreActionControl::SetTextFont(Font* pFont){
	if( !pFont ) return;
	pFont->CopyFont(&m_fontText);
	}

void
ESChildPreActionControl::SetLabel(_String sLabel, bool bRedraw)	{
	m_sLabel = sLabel; 
	OnLocalizeStrings();
	if( bRedraw ) 
		Redraw();
	}

Image*
ESChildPreActionControl::GetActiveImage(){
	Image* pImage = &m_imgActiveBg;
	if( pImage->IsNull() ){
		MercuryGUI* pGUI	= MercuryGUI::GetInstance();
		pImage				= pGUI->LoadImage(m_sActiveBgImageFile);
		}
	return pImage;
	}

Image*
ESChildPreActionControl::GetDefaultImage(){
	Image* pImage = &m_imgDefaultBg;
	if( pImage->IsNull() ){
		MercuryGUI* pGUI	= MercuryGUI::GetInstance();
		pImage				= pGUI->LoadImage(m_sDefaultBgImageFile);
		}
	return pImage;
	}

void
ESChildPreActionControl::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
Image* pImage	= m_bActiveState ? GetActiveImage() : GetDefaultImage();
	COLORREF crText = m_bActiveState ? m_crTextActive : m_crTextDefault;
	float	fZoomCX = 1.0f, fZoomCY = 1.0f;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	if( fZoomCX == 1.0f )
		m_nTextLeftOffsetScaled = m_nTextLeftOffset;

	_Size szImage;
	_Rect rcClient		= GetClientRect();
	if( pImage && !pImage->IsNull() ){
		szImage.SetSize(pImage->GetWidth()*fZoomCX, pImage->GetHeight()*fZoomCY);
		if( m_bActiveState ){
			if( fZoomCX == 1.0f && fZoomCY == 1.0f )
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), pImage, m_rcOmitborder);
			else{
				if( !m_imageActiveBg_Scaled.IsNull() && m_imageActiveBg_Scaled.GetWidth() == szImage.cx && m_imageActiveBg_Scaled.GetHeight() == szImage.cy )
					m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageActiveBg_Scaled, m_rcOmitborder_Scaled);
				else{
					ImageDef hDestScaled = NULL;
					if( pImage->ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szImage.cx, szImage.cy, _Rect(0, 0, szImage.cx, szImage.cy), hDestScaled, true) ){
						m_imageActiveBg_Scaled.Destroy();
						m_imageActiveBg_Scaled.Attach(hDestScaled);
						m_rcOmitborder_Scaled = _Rect(m_rcOmitborder.left*fZoomCX, m_rcOmitborder.top*fZoomCY, m_rcOmitborder.right*fZoomCX, m_rcOmitborder.bottom*fZoomCY);
						m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageActiveBg_Scaled, m_rcOmitborder_Scaled);
						}
					}
				}
			}
		else{
			if( fZoomCX == 1.0f && fZoomCY == 1.0f )
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), pImage, m_rcOmitborder);
			else{
				if( !m_imagDefaultBg_Scaled.IsNull() && m_imagDefaultBg_Scaled.GetWidth() == szImage.cx && m_imagDefaultBg_Scaled.GetHeight() == szImage.cy )
					m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imagDefaultBg_Scaled, m_rcOmitborder_Scaled);
				else{
					ImageDef hDestScaled = NULL;
					if( pImage->ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szImage.cx, szImage.cy, _Rect(0, 0, szImage.cx, szImage.cy), hDestScaled, true) ){
						m_imagDefaultBg_Scaled.Destroy();
						m_imagDefaultBg_Scaled.Attach(hDestScaled);
						m_rcOmitborder_Scaled = _Rect(m_rcOmitborder.left*fZoomCX, m_rcOmitborder.top*fZoomCY, m_rcOmitborder.right*fZoomCX, m_rcOmitborder.bottom*fZoomCY);
						m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imagDefaultBg_Scaled, m_rcOmitborder_Scaled);
						}
					}
				}
			}
		}

	// Draw text. {{
	_Size	szText;
	if( m_sLabelLocal.GetLength() ){
		Font	fontText;
		m_fontText.StretchedTextFont(&fontText, fZoomCX, fZoomCY);
		fontText.GetTextSize(&m_sLabelLocal, szText);

		FONTDef	pFontOld = pDC->SelectObject(fontText);
		// Translate from window pixel to DC pixel.{{
		_Point ptLabel;
		ptLabel.x = ((rcClient.left + m_nTextLeftOffsetScaled /*(rcClient.Width() - szText.cx)/2*/) - pRectClient->left) + pRectDC->left; 
		ptLabel.y = ((rcClient.top + (rcClient.Height() - szText.cy)/2) - pRectClient->top) + pRectDC->top; 
		//}}

		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, m_sLabelLocal);
		pDC->SelectObject	(pFontOld);
		}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildPreActionControl::OnLButtonClick(_Point pt, UINT nFlags){
	m_bActiveState = !m_bActiveState;
	SendEvent(this, ChildControlEventCode::EventCode_ControlClick, (void*)m_bActiveState);
	Redraw(true);
	}

void
ESChildPreActionControl::OnLocalizeStrings(){
	m_sLabelLocal = m_sLabel;
	if( !m_pOwner->IsDesignerMode() )
		ESLocalizationMan::ParseText(m_sLabelLocal);
	}

/////////////////////////////////////////////////////////////////////////////
// Variable modify callbacks.

bool
ESChildPreActionControl::AttributeModify(ESChildPreActionControl* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}
