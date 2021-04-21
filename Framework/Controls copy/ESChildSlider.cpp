// ESChildSlider.cpp : implementation file
//

#include "ESChildSlider.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildSlider

ESChildSlider::ESChildSlider(){
	m_sClassName				= _T("ESChildSlider");
	m_rcOmitBarFilled.SetRect(0, 0, 0, 0);
	m_rcOmitBarNotFilled.SetRect(0, 0, 0, 0);
	m_nDistBetweenBarAndButton	= 5;
	m_bVerticalMode				= false;
	m_fZoom						= 1.0f;

	m_bMouseDown				= false;
	m_nPos						= 0;
	m_nMin						= 0;
	m_nMax						= 0;
	m_nStep						= 0;
	m_ptCursorPos.SetPoint(0, 0);
	m_bCursorCaptured			= false;
	m_bAllowDrag				= true;
	m_bJumpToClickedPos			= false;
	/*
	SetupSlider		(0, 100, 10, false);
	SetSliderPos	(76, false);
	*/
	}

BEGIN_DECLARE_VAR(ESChildSlider, ESChildControl)
	DECLARE_VAR(_T("ESChildSlider"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("SliderBarFilled"),			_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imageSliderBarFilled), false, false, false)
	DECLARE_VAR(_T("OmitBarFilled"),			_T(""), VariableType::VT_Rect, offsetof(ESChildSlider,m_rcOmitBarFilled), false, false, false)
	DECLARE_VAR(_T("SliderBarNotFilled"),		_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imageSliderBarNotFilled), false, false, false)
	DECLARE_VAR(_T("OmitBarNotFilled"),			_T(""), VariableType::VT_Rect, offsetof(ESChildSlider,m_rcOmitBarNotFilled), false, false, false)
	DECLARE_VAR(_T("CursorNormal"),				_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imageCursor), false, false, false)
	DECLARE_VAR(_T("CursorPushed"),				_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imageCursorPushed), false, false, false)
	DECLARE_VAR(_T("PlusButtonNormal"),			_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imagePlusButtonNormal), false, false, false)
	DECLARE_VAR(_T("PlusButtonPushed"),			_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imagePlusButtonPushed), false, false, false)
	DECLARE_VAR(_T("MinusButtonNormal"),		_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imageMinusButtonNormal), false, false, false)
	DECLARE_VAR(_T("MinusButtonPushed"),		_T(""), VariableType::VT_Image, offsetof(ESChildSlider,m_imageMinusButtonPushed), false, false, false)
	DECLARE_VAR(_T("DistBetweenBarAndButton"),	_T(""), VariableType::VT_Int32, offsetof(ESChildSlider,m_nDistBetweenBarAndButton), false, false, false)
	DECLARE_VAR(_T("VerticalMode"),				_T(""), VariableType::VT_Bool, offsetof(ESChildSlider,m_bVerticalMode), false, false, false)
	DECLARE_VAR(_T("Zoom"),						_T(""), VariableType::VT_Float, offsetof(ESChildSlider,m_fZoom), false, false, false)
	DECLARE_VAR(_T("JumpToClickedPos"),			_T(""), VariableType::VT_Bool, offsetof(ESChildSlider,m_bJumpToClickedPos), false, false, false)
END_DECLARE_VAR()

ESChildSlider::~ESChildSlider(){
	}

bool
ESChildSlider::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	if( ESChildControl::Create(nId, rRect, pOwner, pParent) ){
		if( pOwner->IsDesignerMode() ){
			// Register variable modify callback methods. {{
			AddVariableModifyCallBack(&m_bVerticalMode,	(VariableValueModifyCallback)&ESChildSlider::VerticalModeModify);
			// }}

			SetupSlider(0, 100, 1, false);
			}
		AdjustAttributesByVerticalMode(m_bVerticalMode);
		}
	return true;
	}

bool
ESChildSlider::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildSlider::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildSlider::Destroy(){
	ESChildControl::Destroy();
	m_bMouseDown = false;
	return true;
	}

void
ESChildSlider::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildSlider::SetupSlider(int nMin, int nMax, int nStep, bool bInvalidate /*= true*/){
//	if( nMin >= nMax ) return false;
	m_nMin	= nMin;
	m_nMax	= nMax;
	m_nStep	= nStep;
	m_nPos	= m_nMin;

	CalRects		();
	CalcCursorRect	(m_rcBar, m_rcCursor);

	if( bInvalidate )
		Redraw();
	return true;
	}

bool
ESChildSlider::SetSliderPos(int nPos, bool bInvalidate /*= true*/, bool bReflectEvent /*= true*/){
	if( nPos > m_nMax || nPos < m_nMin )
		return false;

	m_nPos = nPos;
	if( m_bCreated ){
		CalRects		();
		CalcCursorRect	(m_rcBar, m_rcCursor);
		}

	if( bInvalidate )
		Redraw();

	if( bReflectEvent && m_pOwner )
		SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
	return true;
	}

void
ESChildSlider::CalRects(){
	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	if( m_pOwner )
		m_pOwner->GetZoom(fZoomCX, fZoomCY);

	_Rect		rcControl = GetClientRect();
	_Rect		rcMinus, rcPlus, rcBar, rcCursor;

	_Size		szMinusImg(0, 0), szPlusImg(0, 0), szBarImg(0, 0), szCursorImg(0, 0);
	if( !m_imageSliderBarFilled.IsNull() )
		szBarImg.SetSize(m_imageSliderBarFilled.GetWidth(), m_imageSliderBarFilled.GetHeight());
	if( !m_imagePlusButtonNormal.IsNull() )
		szPlusImg.SetSize(m_imagePlusButtonNormal.GetWidth(), m_imagePlusButtonNormal.GetHeight());
	if( !m_imageMinusButtonNormal.IsNull() )
		szMinusImg.SetSize(m_imageMinusButtonNormal.GetWidth(), m_imageMinusButtonNormal.GetHeight());
	if( !m_imageCursor.IsNull() )
		szCursorImg.SetSize(m_imageCursor.GetWidth(), m_imageCursor.GetHeight());

	m_rcOmitBarFilled_Scale		= _Rect(m_rcOmitBarFilled.left*fZoomCX, m_rcOmitBarFilled.top*fZoomCY, m_rcOmitBarFilled.right*fZoomCX, m_rcOmitBarFilled.bottom*fZoomCY);
	m_rcOmitBarNotFilled_Scale	= _Rect(m_rcOmitBarNotFilled.left*fZoomCX, m_rcOmitBarNotFilled.top*fZoomCY, m_rcOmitBarNotFilled.right*fZoomCX, m_rcOmitBarNotFilled.bottom*fZoomCY);

	// Calculate rects. {{
	if( m_bVerticalMode ){
		rcPlus.left		= rcControl.left;
		rcPlus.top		= rcControl.top;
		rcPlus.right	= rcPlus.left + (szPlusImg.cx*fZoomCX);
		rcPlus.bottom	= rcPlus.top + (szPlusImg.cy*fZoomCY);

		rcMinus.left	= rcControl.left;
		rcMinus.top		= rcControl.bottom - (szMinusImg.cy*fZoomCY);
		rcMinus.right	= rcMinus.left + (szMinusImg.cx*fZoomCX);
		rcMinus.bottom	= rcMinus.top + (szMinusImg.cy*fZoomCY);

		rcBar.top		= rcPlus.bottom + (m_nDistBetweenBarAndButton*fZoomCY);
		rcBar.left		= rcControl.left + ((max(szMinusImg.cx, szCursorImg.cx) - szBarImg.cx) / 2)*fZoomCX;
		rcBar.bottom	= rcMinus.top - (m_nDistBetweenBarAndButton*fZoomCY);
		rcBar.right		= rcBar.left + (szBarImg.cx*fZoomCX);
		}
	else{
		rcMinus.left	= rcControl.left;
		rcMinus.top		= rcControl.top;
		rcMinus.right	= rcMinus.left + (szMinusImg.cx*fZoomCX);
		rcMinus.bottom	= rcMinus.top + (szMinusImg.cy*fZoomCY);

		rcPlus.right	= rcControl.right;
		rcPlus.top		= rcControl.top;
		rcPlus.left		= rcPlus.right - (szPlusImg.cx*fZoomCX);
		rcPlus.bottom	= rcMinus.top + (szPlusImg.cy*fZoomCY);

		rcBar.left		= rcMinus.right + (m_nDistBetweenBarAndButton*fZoomCX);
		rcBar.top		= rcControl.top + ((max(szMinusImg.cy,szCursorImg.cy) - szBarImg.cy) / 2)*fZoomCY;
		rcBar.right		= rcPlus.left - (m_nDistBetweenBarAndButton*fZoomCX);
		rcBar.bottom	= rcBar.top + (szBarImg.cy*fZoomCY);
		}
	
	CalcCursorRect(rcBar, rcCursor);

	m_rcBar			= rcBar;
	m_rcMinus		= rcMinus;
	m_rcPlus		= rcPlus;
	m_rcCursor		= rcCursor;
	// }}
	}

bool
ESChildSlider::CalcCursorRect(_Rect rcBar, _Rect& rcCursor){
	_Size szCursorImg(0, 0);
	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	if( m_pOwner )
		m_pOwner->GetZoom(fZoomCX, fZoomCY);
	if( !m_imageCursor.IsNull() )
		szCursorImg.SetSize(m_imageCursor.GetWidth()*fZoomCX, m_imageCursor.GetHeight()*fZoomCY);

	if( m_nPos < m_nMin ){
		m_nPos				= m_nMin;
		if( m_bVerticalMode ){
			rcCursor.top		= rcBar.bottom - szCursorImg.cy;
			rcCursor.left		= rcBar.left + (rcBar.Width() - szCursorImg.cx)/2;
			rcCursor.right		= rcCursor.left + szCursorImg.cx;
			rcCursor.bottom		= rcCursor.top + szCursorImg.cy;
			}
		else{
			rcCursor.left		= rcBar.left;
			rcCursor.top		= rcBar.top + (rcBar.Height() - szCursorImg.cy)/2;
			rcCursor.right		= rcCursor.left + szCursorImg.cx;
			rcCursor.bottom		= rcCursor.top + szCursorImg.cy;
			}
		return true;
		}

	if( m_nPos > m_nMax ){
		m_nPos				= m_nMax;
		if( m_bVerticalMode ){
			rcCursor.top		= rcBar.top;
			rcCursor.left		= rcBar.left + (rcBar.Width() - szCursorImg.cx)/2;
			rcCursor.right		= rcCursor.left + szCursorImg.cx;
			rcCursor.bottom		= rcCursor.top + szCursorImg.cy;
			}
		else{
			rcCursor.left		= rcBar.right - szCursorImg.cx;
			rcCursor.top		= rcBar.top + (rcBar.Height() - szCursorImg.cy)/2;
			rcCursor.right		= rcCursor.left + szCursorImg.cx;
			rcCursor.bottom		= rcCursor.top + szCursorImg.cy;
			}
		return true;
		}

	if( m_bVerticalMode ){
		float fDelta		= (float)((rcBar.Height() - szCursorImg.cy) / ((m_nMax - m_nMin)*1.0f));
		m_ptCursorPos.y		= (rcBar.bottom - szCursorImg.cy) - (int)(fDelta*(m_nPos - m_nMin));
		m_ptCursorPos.x		= rcBar.left;

		rcCursor.top		= m_ptCursorPos.y;
		rcCursor.left		= rcBar.left + (rcBar.Width() - szCursorImg.cx)/2;
		rcCursor.right		= rcCursor.left + szCursorImg.cx;
		rcCursor.bottom		= rcCursor.top + szCursorImg.cy;
		}
	else{
		float fDelta		= (float)((rcBar.Width() - szCursorImg.cx) / ((m_nMax - m_nMin)*1.0f));
		m_ptCursorPos.x		= rcBar.left + (int)(fDelta*(m_nPos - m_nMin));
		m_ptCursorPos.y		= rcBar.top;

		rcCursor.left		= m_ptCursorPos.x;
		rcCursor.top		= rcBar.top + (rcBar.Height() - szCursorImg.cy)/2;
		rcCursor.right		= rcCursor.left + szCursorImg.cx;
		rcCursor.bottom		= rcCursor.top + szCursorImg.cy;
		}
	return true;
	}

void
ESChildSlider::AdjustAttributesByVerticalMode(bool bVerticalMode){
	bool bRotate = false;
	if( !m_imageSliderBarFilled.IsNull() ){
		if( (m_imageSliderBarFilled.GetWidth() > m_imageSliderBarFilled.GetHeight() && bVerticalMode) ||
			(m_imageSliderBarFilled.GetWidth() < m_imageSliderBarFilled.GetHeight() && !bVerticalMode))
			{
			bRotate = true;
			}
		}

	if( bRotate ){
		if( !m_imageSliderBarFilled.IsNull() )
			m_imageSliderBarFilled.Rotate90Bitmap32bpp();
		if( !m_imageSliderBarNotFilled.IsNull() )
			m_imageSliderBarNotFilled.Rotate90Bitmap32bpp();
		if( !m_imageCursor.IsNull() )
			m_imageCursor.Rotate90Bitmap32bpp();
		if( !m_imageCursorPushed.IsNull() )
			m_imageCursorPushed.Rotate90Bitmap32bpp();
		if( !m_imagePlusButtonNormal.IsNull() )
			m_imagePlusButtonNormal.Rotate90Bitmap32bpp();
		if( !m_imagePlusButtonPushed.IsNull() )
			m_imagePlusButtonPushed.Rotate90Bitmap32bpp();
		if( !m_imageMinusButtonNormal.IsNull() )
			m_imageMinusButtonNormal.Rotate90Bitmap32bpp();
		if( !m_imageMinusButtonPushed.IsNull() )
			m_imageMinusButtonPushed.Rotate90Bitmap32bpp();

		_Rect rcTemp;
		rcTemp.SetRect(m_rcOmitBarFilled.top, m_rcOmitBarFilled.left, m_rcOmitBarFilled.bottom, m_rcOmitBarFilled.right);
		m_rcOmitBarFilled = rcTemp;

		rcTemp.SetRect(m_rcOmitBarNotFilled.top, m_rcOmitBarNotFilled.left, m_rcOmitBarNotFilled.bottom, m_rcOmitBarNotFilled.right);
		m_rcOmitBarNotFilled = rcTemp;
		}
	}

bool
ESChildSlider::CalcCursorPosByPoint(_Point pt){
	if( m_bVerticalMode ){
		float		fDelta		= (float)((m_rcBar.Height()) / ((m_nMax - m_nMin)*1.0f));
		m_nPos					= m_nMin + (int)((pt.y - m_rcBar.top)/fDelta);
		_Rect rcCursor;
		if( CalcCursorRect(m_rcBar, rcCursor) ){
			_Rect rcRedraw = m_rcCursor|rcCursor;
			RedrawRect(rcRedraw, true);
			m_rcCursor = rcCursor;
			return true;
			}
		}
	else{
		float		fDelta		= (float)((m_rcBar.Width()) / ((m_nMax - m_nMin)*1.0f));
		m_nPos					= m_nMin + (int)((pt.x - m_rcBar.left)/fDelta);
		_Rect rcCursor;
		if( CalcCursorRect(m_rcBar, rcCursor) ){
			_Rect rcRedraw = m_rcCursor|rcCursor;
			RedrawRect(rcRedraw, true);
			m_rcCursor = rcCursor;
			return true;
			}
		}
	return false;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildSlider::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcControlDC, rcInvalid;
	rcControlDC.left	= (m_rRect.left - pRectClient->left) + pRectDC->left; 
	rcControlDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top; 
	rcControlDC.right	= rcControlDC.left + m_rRect.Width(); 
	rcControlDC.bottom	= rcControlDC.top + m_rRect.Height();

	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	if( m_pOwner )
		m_pOwner->GetZoom(fZoomCX, fZoomCY);

	_Rect		rcControl = GetClientRect();
	CalRects();

	// Draw minus sign. {{
	if( m_bMouseCaptured && m_rcMinus.PtInRect(m_ptMouseDown) && !m_imageMinusButtonPushed.IsNull() )
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcMinus, _FillMode::FillMode_StretchImage, RGB(0, 0, 0), &m_imageMinusButtonPushed, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
	else
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcMinus, _FillMode::FillMode_StretchImage, RGB(0, 0, 0), &m_imageMinusButtonNormal, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
	// }}
	// Draw plus sign. {{
	if( m_bMouseCaptured && m_rcPlus.PtInRect(m_ptMouseDown) && !m_imagePlusButtonPushed.IsNull() )
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcPlus, _FillMode::FillMode_StretchImage, RGB(0, 0, 0), &m_imagePlusButtonPushed, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
	else
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcPlus, _FillMode::FillMode_StretchImage, RGB(0, 0, 0), &m_imagePlusButtonNormal, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
	// }}

	// Draw slider. {{
	if( m_imageSliderBarNotFilled.IsNull() ){
		_Size szBarFilled	(m_imageSliderBarFilled.GetWidth()*fZoomCX, m_imageSliderBarFilled.GetHeight()*fZoomCY);
		if( fZoomCX == 1.0f && fZoomCY == 1.0f )
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcBar, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarFilled, m_rcOmitBarFilled);
		else{
			if( !m_imageSliderBarFilled_Scale.IsNull() && m_imageSliderBarFilled_Scale.GetWidth() == szBarFilled.cx && m_imageSliderBarFilled_Scale.GetHeight() == szBarFilled.cy ){
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcBar, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarFilled_Scale, m_rcOmitBarFilled_Scale);
				}
			else{
				ImageDef hDestScaled = NULL;
				if( m_imageSliderBarFilled.ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szBarFilled.cx, szBarFilled.cy, _Rect(0, 0, szBarFilled.cx, szBarFilled.cy), hDestScaled, true) ){
					m_imageSliderBarFilled_Scale.Destroy();
					m_imageSliderBarFilled_Scale.Attach(hDestScaled);
					m_rcOmitBarFilled_Scale = _Rect(m_rcOmitBarFilled.left*fZoomCX, m_rcOmitBarFilled.top*fZoomCY, m_rcOmitBarFilled.right*fZoomCX, m_rcOmitBarFilled.bottom*fZoomCY);

					m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcBar, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarFilled_Scale, m_rcOmitBarFilled_Scale);
					}
				}
			}
		}
	else{
		_Rect rcBarFilled, rcBarEmpty, rcBarFilledDC;
		if( m_bVerticalMode ){
			rcBarEmpty.left		= m_rcBar.left;
			rcBarEmpty.top		= m_rcBar.top;
			rcBarEmpty.right	= m_rcBar.right;
			rcBarEmpty.bottom	= m_rcCursor.bottom;

			rcBarFilled.left	= m_rcBar.left;
			rcBarFilled.top		= rcBarEmpty.bottom;
			rcBarFilled.right	= m_rcBar.right;
			rcBarFilled.bottom	= m_rcBar.bottom;
			}
		else{
			rcBarFilled.left	= m_rcBar.left;
			rcBarFilled.top		= m_rcBar.top;
			rcBarFilled.right	= m_rcCursor.left;
			rcBarFilled.bottom	= m_rcBar.bottom;

			rcBarEmpty.left		= rcBarFilled.right;
			rcBarEmpty.top		= m_rcBar.top;
			rcBarEmpty.right	= m_rcBar.right;
			rcBarEmpty.bottom	= m_rcBar.bottom;
			}
		
		// Draw filled bar cliped. {{
		_Size szBarFilled	(m_imageSliderBarFilled.GetWidth()*fZoomCX, m_imageSliderBarFilled.GetHeight()*fZoomCY);
		if( fZoomCX == 1.0f && fZoomCY == 1.0f )
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcBar, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarFilled, m_rcOmitBarFilled);
		else{
			if( !m_imageSliderBarFilled_Scale.IsNull() && m_imageSliderBarFilled_Scale.GetWidth() == szBarFilled.cx && m_imageSliderBarFilled_Scale.GetHeight() == szBarFilled.cy ){
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcBar, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarFilled_Scale, m_rcOmitBarFilled_Scale);
				}
			else{
				ImageDef hDestScaled = NULL;
				if( m_imageSliderBarFilled.ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szBarFilled.cx, szBarFilled.cy, _Rect(0, 0, szBarFilled.cx, szBarFilled.cy), hDestScaled, true) ){
					m_imageSliderBarFilled_Scale.Destroy();
					m_imageSliderBarFilled_Scale.Attach	(hDestScaled);
					m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcBar, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarFilled_Scale, m_rcOmitBarFilled_Scale);
					}
				}
			}
		// }}

		_Size szBarNotFilled(m_imageSliderBarNotFilled.GetWidth()*fZoomCX, m_imageSliderBarNotFilled.GetHeight()*fZoomCY);
		if( fZoomCX == 1.0f && fZoomCY == 1.0f )
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcBarEmpty, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarNotFilled, m_rcOmitBarNotFilled);
		else{
			if( !m_imageSliderBarNotFilled_Scale.IsNull() && m_imageSliderBarNotFilled_Scale.GetWidth() == szBarNotFilled.cx && m_imageSliderBarNotFilled_Scale.GetHeight() == szBarNotFilled.cy ){
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcBarEmpty, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarNotFilled_Scale, m_rcOmitBarNotFilled_Scale);
				}
			else{
				ImageDef hDestScaled = NULL;
				if( m_imageSliderBarNotFilled.ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szBarNotFilled.cx, szBarNotFilled.cy, _Rect(0, 0, szBarNotFilled.cx, szBarNotFilled.cy), hDestScaled, true) ){
					m_imageSliderBarNotFilled_Scale.Destroy	();
					m_imageSliderBarNotFilled_Scale.Attach	(hDestScaled);
					m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcBarEmpty, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), &m_imageSliderBarNotFilled_Scale, m_rcOmitBarNotFilled_Scale);
					}
				}
			}
		}
	// }}

	// Draw cursor.
	m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, m_rcCursor, _FillMode::FillMode_StretchImage, RGB(0, 0, 0), 
		(m_bCursorCaptured && !m_imageCursorPushed.IsNull()) ?  &m_imageCursorPushed : &m_imageCursor, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildSlider::OnMouseHover(_Point pt, UINT nFlags){
	if( m_bCursorCaptured ){
		if( CalcCursorPosByPoint(pt) ){
			SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
			}
		}

	if( m_rcMinus.PtInRect(pt) || m_rcPlus.PtInRect(pt) )
		m_bHandCursor = true;
	else
		m_bHandCursor = false;
	}

BOOL	
ESChildSlider::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){/*
	if( m_bHandCursor )
		SetCursor(LoadCursor(NULL, IDC_HAND));
	else
		SetCursor(LoadCursor(NULL, IDC_ARROW));*/
	return 1;
	}

void	
ESChildSlider::OnMouseLeave(UINT nFlags){
	KillTimer(MinusSlideTimerId);
	KillTimer(PlusSlideTimerId);
	}

void	
ESChildSlider::OnMouseEnter(UINT nFlags){
	}

void
ESChildSlider::OnLButtonDown(_Point pt, UINT nFlags){
	ESChildControl::OnLButtonDown(pt, nFlags);

	if( m_rcBar.PtInRect(pt) ){
		if( m_rcCursor.PtInRect(pt) && m_bAllowDrag ){
			m_hitTest			= SliderHitTest::CursorSign;
			m_bCursorCaptured	= true;
			RedrawRect(m_rcCursor);
			return;
			}

		if( m_bJumpToClickedPos && CalcCursorPosByPoint(pt) ){
			SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
			Redraw();
			return;
			}

		if( m_bVerticalMode ){
			if( pt.y > m_rcCursor.bottom ){
				m_nPos -= m_nStep;
				m_nPos	= max(m_nPos, m_nMin);
				CalcCursorRect(m_rcBar, m_rcCursor);
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			else
			if( pt.y < m_rcCursor.top ){
				m_nPos += m_nStep;
				m_nPos	= min(m_nPos, m_nMax);
				CalcCursorRect(m_rcBar, m_rcCursor);
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			}
		else{
			if( pt.x > m_rcCursor.right ){
				m_nPos += m_nStep;
				m_nPos	= min(m_nPos, m_nMax);
				CalcCursorRect(m_rcBar, m_rcCursor);
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			else
			if( pt.x < m_rcCursor.left ){
				m_nPos -= m_nStep;
				m_nPos	= max(m_nPos, m_nMin);
				CalcCursorRect(m_rcBar, m_rcCursor);
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			}
		}
	else{
		if( m_rcPlus.PtInRect(pt) ){
			if( m_nPos < m_nMax ){
				m_hitTest = SliderHitTest::PlusSign;
				m_nPos += m_nStep;
				m_nPos	= min(m_nPos, m_nMax);
				CalcCursorRect(m_rcBar, m_rcCursor);
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();

				SetTimer(PlusSlideTimerId, 300);
				}
			return;
			}

		if( m_rcMinus.PtInRect(pt) ){
			if( m_nPos > m_nMin ){
				m_hitTest = SliderHitTest::MinusSign;
				m_nPos -= m_nStep;
				m_nPos	= max(m_nPos, m_nMin);
				CalcCursorRect(m_rcBar, m_rcCursor);
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();

				SetTimer(MinusSlideTimerId, 300);
				}
			return;
			}
		}
	}

void
ESChildSlider::OnLButtonUp(_Point pt, UINT nFlags){
	ESChildControl::OnLButtonUp(pt, nFlags);

	KillTimer(MinusSlideTimerId);
	KillTimer(PlusSlideTimerId);

	if( m_hitTest == SliderHitTest::MinusSign ){
		RedrawRect(m_rcMinus);
		}
	else
	if( m_hitTest == SliderHitTest::PlusSign ){
		RedrawRect(m_rcPlus);
		}

	if( m_bCursorCaptured || m_rcCursor.PtInRect(pt) ){
		m_bCursorCaptured = false;
		RedrawRect(m_rcCursor);
		}
	m_hitTest = SliderHitTest::NoSign;
	}

void	
ESChildSlider::OnLButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildSlider::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildSlider::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildSlider::OnLButtonDblClick(_Point pt, UINT nFlags){
	if( m_rcPlus.PtInRect(pt) ){
		if( m_nPos < m_nMax ){
			m_hitTest = SliderHitTest::PlusSign;
			m_nPos += m_nStep;
			m_nPos	= min(m_nPos, m_nMax);
			CalcCursorRect(m_rcBar, m_rcCursor);
			SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
			Redraw();
			}
		return;
		}
	else
	if( m_rcMinus.PtInRect(pt) ){
		if( m_nPos > m_nMin ){
			m_hitTest = SliderHitTest::MinusSign;
			m_nPos -= m_nStep;
			m_nPos	= max(m_nPos, m_nMin);
			CalcCursorRect(m_rcBar, m_rcCursor);
			SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
			Redraw();
			}
		return;
		}
	else{
		if( !m_rcCursor.PtInRect(pt) ){
			if( m_bVerticalMode ){
				if( pt.y > m_rcCursor.bottom ){ // Minus
					m_hitTest = SliderHitTest::MinusSign;
					m_nPos -= m_nStep;
					m_nPos	= max(m_nPos, m_nMin);
					CalcCursorRect(m_rcBar, m_rcCursor);
					SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
					Redraw();
					}
				else{ // Plus
					m_hitTest = SliderHitTest::PlusSign;
					m_nPos += m_nStep;
					m_nPos	= min(m_nPos, m_nMax);
					CalcCursorRect(m_rcBar, m_rcCursor);
					SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
					Redraw();
					}
				}
			else{
				if( pt.x > m_rcCursor.right ){ // Plus
					m_hitTest = SliderHitTest::PlusSign;
					m_nPos += m_nStep;
					m_nPos	= min(m_nPos, m_nMax);
					CalcCursorRect(m_rcBar, m_rcCursor);
					SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
					Redraw();
					}
				else{ // Minus
					m_hitTest = SliderHitTest::MinusSign;
					m_nPos -= m_nStep;
					m_nPos	= max(m_nPos, m_nMin);
					CalcCursorRect(m_rcBar, m_rcCursor);
					SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
					Redraw();
					}
				}
			}
		}
	}

void	
ESChildSlider::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildSlider::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildSlider::OnTimer(UINT nIDEvent){
	// Auto scroll events. #######################
	KillTimer(nIDEvent);

	if( nIDEvent == PlusSlideTimerId ){
		if( m_bVerticalMode ){
			m_nPos += m_nStep;
			m_nPos	= min(m_nPos, m_nMax);
			if( CalcCursorRect(m_rcBar, m_rcCursor) ){
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			}
		else{
			m_nPos += m_nStep;
			m_nPos	= min(m_nPos, m_nMax);
			if( CalcCursorRect(m_rcBar, m_rcCursor) ){
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			}
		}


	if( nIDEvent == MinusSlideTimerId ){
		if( m_bVerticalMode ){
			m_nPos -= m_nStep;
			m_nPos	= max(m_nPos, m_nMin);
			if( CalcCursorRect(m_rcBar, m_rcCursor) ){
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			}
		else{
			m_nPos -= m_nStep;
			m_nPos	= max(m_nPos, m_nMin);
			if( CalcCursorRect(m_rcBar, m_rcCursor) ){
				SendEvent(this, ChildControlEventCode::EventCode_SliderPosChanged, (void*)m_nPos);
				Redraw();
				}
			}
		}

	SetTimer(nIDEvent, 100);
	}

bool
ESChildSlider::VerticalModeModify(ESChildSlider* pThis, VariableInfo* pInfo, void* lpValueNew){
	bool bVerticalMode = *(bool*)lpValueNew;
	pThis->AdjustAttributesByVerticalMode(bVerticalMode);
	pThis->Redraw();
	return true;
	}
