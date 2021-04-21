// ESChildControlCheckBox.cpp : implementation file
//

#include "ESChildControlCheckBox.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildControlCheckBox

ESChildControlCheckBox::ESChildControlCheckBox(){
	m_sClassName		= _T("ESChildControlCheckBox");
	m_sLabel			= _T("");
	m_bAllowMouseEvents	= false;
	m_bCheckState		= false;
	m_crText			= RGB(255, 255, 255);
	}

BEGIN_DECLARE_VAR(ESChildControlCheckBox, ESChildControl)
	DECLARE_VAR(_T("ESChildControlCheckBox"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Text Font"),					_T(""), VariableType::VT_Font, offsetof(ESChildControlCheckBox,m_fontText), false, false, false)
	DECLARE_VAR(_T("Text Color"),					_T(""), VariableType::VT_Color32, offsetof(ESChildControlCheckBox,m_crText), false, false, false)
	DECLARE_VAR(_T("Label"),						_T(""), VariableType::VT_String, offsetof(ESChildControlCheckBox,m_sLabel), false, false, false)
	DECLARE_VAR(_T("CheckedImage"),					_T(""), VariableType::VT_Image, offsetof(ESChildControlCheckBox,m_checkedImage), false, false, false)
	DECLARE_VAR(_T("UncheckedImage"),				_T(""), VariableType::VT_Image, offsetof(ESChildControlCheckBox,m_uncheckedImage), false, false, false)
	DECLARE_VAR(_T("Checked Image File"),			_T(""), VariableType::VT_String, offsetof(ESChildControlCheckBox,m_sCheckedImageFile), false, false, false)
	DECLARE_VAR(_T("Unchecked Image File"),			_T(""), VariableType::VT_String, offsetof(ESChildControlCheckBox,m_sUncheckedImageFile), false, false, false)
	DECLARE_VAR(_T("Check state"),					_T(""), VariableType::VT_Bool, offsetof(ESChildControlCheckBox,m_bCheckState), false, false, false)
END_DECLARE_VAR()

ESChildControlCheckBox::~ESChildControlCheckBox(){
	m_fontText.DeleteObject();
	m_checkedImage.Destroy();
	m_uncheckedImage.Destroy();
	}

bool
ESChildControlCheckBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet && m_pOwner->IsDesignerMode() ){
		// Register variable modify callback methods. {{
		AddVariableModifyCallBack(&m_fontText,					(VariableValueModifyCallback)&ESChildControlCheckBox::AttributeModify);
		AddVariableModifyCallBack(&m_crText,					(VariableValueModifyCallback)&ESChildControlCheckBox::AttributeModify);
		AddVariableModifyCallBack(&m_sLabel,					(VariableValueModifyCallback)&ESChildControlCheckBox::AttributeModify);
		AddVariableModifyCallBack(&m_checkedImage,				(VariableValueModifyCallback)&ESChildControlCheckBox::AttributeModify);
		AddVariableModifyCallBack(&m_uncheckedImage,			(VariableValueModifyCallback)&ESChildControlCheckBox::AttributeModify);
		AddVariableModifyCallBack(&m_sCheckedImageFile,			(VariableValueModifyCallback)&ESChildControlCheckBox::AttributeModify);
		AddVariableModifyCallBack(&m_sUncheckedImageFile,		(VariableValueModifyCallback)&ESChildControlCheckBox::AttributeModify);
		// }}
		}
	if( bRet )
		OnLocalizeStrings();
	return bRet;
	}

bool
ESChildControlCheckBox::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlCheckBox::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlCheckBox::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlCheckBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

void
ESChildControlCheckBox::SetTextFont(Font* pFont){
	if( !pFont ) return;
	pFont->CopyFont(&m_fontText);
	}

void
ESChildControlCheckBox::SetLabel(_string sText, bool bRedraw){
	m_sLabel = sText; 
	OnLocalizeStrings();
	if( bRedraw ) 
		Redraw();
	}

Image*
ESChildControlCheckBox::GetCheckedImage(){
	Image* pCheckedImage = &m_checkedImage;
	if( pCheckedImage->IsNull() ){
		MercuryGUI* pGUI	= MercuryGUI::GetInstance();
		pCheckedImage		= pGUI->LoadImage(m_sCheckedImageFile);
		}
	return pCheckedImage;
	}

Image*
ESChildControlCheckBox::GetUncheckedImage(){
	Image* pUncheckedImage = &m_uncheckedImage;
	if( pUncheckedImage->IsNull() ){
		MercuryGUI* pGUI	= MercuryGUI::GetInstance();
		pUncheckedImage		= pGUI->LoadImage(m_sUncheckedImageFile);
		}
	return pUncheckedImage;
	}

bool
ESChildControlCheckBox::SetCheckImages(Image* pImageCheck, Image* pImageNormal, bool bRedraw){
	if( pImageCheck && !pImageCheck->IsNull() &&
		pImageNormal && !pImageNormal->IsNull() ){
		if( !m_uncheckedImage.IsNull() )
			m_uncheckedImage.Destroy();
		m_uncheckedImage.Attach	(pImageNormal->Detach());

		if( !m_checkedImage.IsNull() )
			m_checkedImage.Destroy();
		m_checkedImage.Attach	(pImageCheck->Detach());
		return true;
		}
	return false;
	}

void
ESChildControlCheckBox::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	Image* pImage	= NULL;
	if( m_bCheckState )
		pImage = GetCheckedImage();
	else
		pImage = GetUncheckedImage();

	float		fZoomCX = 1.0f;
	float		fZoomCY = 1.0f;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	_Size szImage(0, 0);
	_Rect rcClient		= GetClientRect();
	if( pImage && !pImage->IsNull() ){
		szImage.SetSize(pImage->GetWidth()*fZoomCX, pImage->GetHeight()*fZoomCY);
		
		_Rect rcCheckBox;
		rcCheckBox.left		= rcClient.left + 2;
		rcCheckBox.top		= rcClient.top + (rcClient.Height() - szImage.cy)/2;
		rcCheckBox.right	= rcCheckBox.left + szImage.cx;
		rcCheckBox.bottom	= rcCheckBox.top + szImage.cy;

		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcCheckBox, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pImage, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
		rcClient.left		= rcCheckBox.right + 4;
		}

	// Draw text. {{
	_Size	szText;
	if( m_sLabelLocal.length() ){
		Font textFont;
		m_fontText.StretchedTextFont(&textFont, fZoomCX, fZoomCY);
		textFont.GetTextSize(&m_sLabelLocal, szText);

		Font*	pFontOld = pDC->SelectObject(&textFont);
		// Translate from window pixel to DC pixel.{{
		_Point ptLabel;
		ptLabel.x = (rcClient.left - pRectClient->left) + pRectDC->left; 
		ptLabel.y = ((rcClient.top + (rcClient.Height() - szText.cy)/2) - pRectClient->top) + pRectDC->top; 
		//}}

		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(m_crText);
		pDC->ExtTextOut     (ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, m_sLabelLocal.c_str());
		pDC->SelectObject	(pFontOld);
		}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildControlCheckBox::OnMouseHover(_Point pt, UINT nFlags){
	}

BOOL	
ESChildControlCheckBox::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	return FALSE;
	}

void	
ESChildControlCheckBox::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlCheckBox::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlCheckBox::OnLButtonDown(_Point pt, UINT nFlags){
	}

void
ESChildControlCheckBox::OnLButtonUp(_Point pt, UINT nFlags){
	}

void	
ESChildControlCheckBox::OnLButtonClick(_Point pt, UINT nFlags){
	m_bCheckState = !m_bCheckState;
	SendEvent(this, ChildControlEventCode::EventCode_CheckBoxClick, (void*)m_bCheckState);
	Redraw(true);
	}

void	
ESChildControlCheckBox::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlCheckBox::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlCheckBox::OnLButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlCheckBox::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlCheckBox::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void
ESChildControlCheckBox::OnLocalizeStrings(){
	m_sLabelLocal = m_sLabel;
	if( !m_pOwner->IsDesignerMode() )
		ESLocalizationMan::ParseText(m_sLabelLocal);
	}

/////////////////////////////////////////////////////////////////////////////
// Variable modify callbacks.

bool
ESChildControlCheckBox::AttributeModify(ESChildControlCheckBox* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw();
	return true;
	}
