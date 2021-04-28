// ESChildScrollView.cpp : implementation file
//

#include "ESChildScrollView.h"
#include "ESChildScrollBar.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildScrollView

ESChildScrollView::ESChildScrollView(){
	m_sClassName			= _T("ESChildScrollView");
	m_crBorder				= RGB(0, 0, 0);
	m_nBorderWidth			= 1;
	m_nVScrollBarTopOffset	= 0;

	m_bFillWithColor		= true;
	m_crFill				= RGB(255, 255, 255);

	m_nLineHeight			= 10;
	m_szView.SetSize(0, 0);
	m_szPage.SetSize(0, 0);

	m_pVScroll				= NULL;
	m_pHScroll				= NULL;

	MercuryGUI* pGUI		= MercuryGUI::GetInstance();
	m_nArrowBtnCX			= pGUI->scrollBar.m_nVScrollWidth;
	m_nArrowBtnCY			= pGUI->scrollBar.m_nHScrollHeight;
	m_rcVThumbOmitBorder.SetRect(0, 0, 0, 0);
	m_rcHThumbOmitBorder.SetRect(0, 0, 0, 0);
	}

BEGIN_DECLARE_VAR(ESChildScrollView, ESChildControl)
	DECLARE_VAR(_T("ESChildScrollView"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("LineCY"),						_T(""), VariableType::VT_Int32, offsetof(ESChildScrollView,m_nLineHeight), false, false, false)
	DECLARE_VAR(_T("VScrollBarTopOffset"),			_T(""), VariableType::VT_Int32, offsetof(ESChildScrollView,m_nVScrollBarTopOffset), false, false, false)

	DECLARE_VAR(_T("VScrollThumb"),					_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageVScrollThumb), false, false, false)
	DECLARE_VAR(_T("VScrollArrowBottomSel"),		_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageVScrollArrowBottomSel), false, false, false)
	DECLARE_VAR(_T("VScrollArrowBottomNormal"),		_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageVScrollArrowBottomNormal), false, false, false)
	DECLARE_VAR(_T("VScrollMiddle"),				_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageVScrollMiddle), false, false, false)
	DECLARE_VAR(_T("VScrollArrowTopSel"),			_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageVScrollArrowTopSel), false, false, false)
	DECLARE_VAR(_T("VScrollArrowTopNormal"),		_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageVScrollArrowTopNormal), false, false, false)
	DECLARE_VAR(_T("VThumbOmitBorder"),				_T(""), VariableType::VT_Rect, offsetof(ESChildScrollView,m_rcVThumbOmitBorder), false, false, false)

	DECLARE_VAR(_T("HScrollThumb"),					_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageHScrollThumb), false, false, false)
	DECLARE_VAR(_T("HScrollArrowRightSel"),			_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageHScrollArrowRightSel), false, false, false)
	DECLARE_VAR(_T("HScrollArrowRightNormal"),		_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageHScrollArrowRightNormal), false, false, false)
	DECLARE_VAR(_T("HScrollMiddle"),				_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageHScrollMiddle), false, false, false)
	DECLARE_VAR(_T("HScrollArrowLeftSel"),			_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageHScrollArrowLeftSel), false, false, false)
	DECLARE_VAR(_T("HScrollArrowLeftNormal"),		_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageHScrollArrowLeftNormal), false, false, false)
	DECLARE_VAR(_T("HThumbOmitBorder"),				_T(""), VariableType::VT_Rect, offsetof(ESChildScrollView,m_rcHThumbOmitBorder), false, false, false)

	DECLARE_VAR(_T("ScrollBarDivider"),				_T(""), VariableType::VT_Image, offsetof(ESChildScrollView,m_imageDivider), false, false, false)
	DECLARE_VAR(_T("ArrowBtnCX"),					_T(""), VariableType::VT_Int32, offsetof(ESChildScrollView,m_nArrowBtnCX), false, false, false)
	DECLARE_VAR(_T("ArrowBtnCY"),					_T(""), VariableType::VT_Int32, offsetof(ESChildScrollView,m_nArrowBtnCY), false, false, false)
END_DECLARE_VAR()

ESChildScrollView::~ESChildScrollView(){
	}

bool
ESChildScrollView::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		m_pVScroll	= new ESChildScrollBar(true);
		m_pHScroll	= new ESChildScrollBar(false);

		if( !m_imageVScrollThumb.IsNull() )
			m_pVScroll->m_pImageVScrollThumb = &m_imageVScrollThumb;
		if( !m_imageVScrollArrowBottomSel.IsNull() )
			m_pVScroll->m_pImageVScrollArrowBottomSel = &m_imageVScrollArrowBottomSel;
		if( !m_imageVScrollArrowBottomNormal.IsNull() )
			m_pVScroll->m_pImageVScrollArrowBottomNormal = &m_imageVScrollArrowBottomNormal;
		if( !m_imageVScrollMiddle.IsNull() )
			m_pVScroll->m_pImageVScrollMiddle = &m_imageVScrollMiddle;
		if( !m_imageVScrollArrowTopSel.IsNull() )
			m_pVScroll->m_pImageVScrollArrowTopSel = &m_imageVScrollArrowTopSel;
		if( !m_imageVScrollArrowTopNormal.IsNull() )
			m_pVScroll->m_pImageVScrollArrowTopNormal = &m_imageVScrollArrowTopNormal;
		if(	!m_rcVThumbOmitBorder.IsRectNull() )
			m_pVScroll->m_rcVThumbOmitBorder = m_rcVThumbOmitBorder;

		if( pOwner->AddChildControl(m_pVScroll, MercuryBaseView::GetChildControlIdUnused(m_pOwner), _Rect(0, 0, m_nArrowBtnCY, 0), this) ){
			_Rect rcFixedPos;
			rcFixedPos.left		= -1;
			rcFixedPos.top		= m_nBorderWidth + m_nVScrollBarTopOffset;
			rcFixedPos.right	= m_nBorderWidth;
			rcFixedPos.bottom	= m_nBorderWidth;
			m_pVScroll->SetSerializeFlag(false);
			m_pVScroll->SetFixedPosition(rcFixedPos);
			m_pVScroll->SetVisible		(false, false);
            m_pVScroll->AllowMouseEvents(true);
			}
        
		if( !m_imageHScrollThumb.IsNull() )
			m_pHScroll->m_pImageHScrollThumb = &m_imageHScrollThumb;
		if( !m_imageHScrollArrowRightSel.IsNull() )
			m_pHScroll->m_pImageHScrollArrowRightSel = &m_imageHScrollArrowRightSel;
		if( !m_imageHScrollArrowRightNormal.IsNull() )
			m_pHScroll->m_pImageHScrollArrowRightNormal = &m_imageHScrollArrowRightNormal;
		if( !m_imageHScrollMiddle.IsNull() )
			m_pHScroll->m_pImageHScrollMiddle = &m_imageHScrollMiddle;
		if( !m_imageHScrollArrowLeftSel.IsNull() )
			m_pHScroll->m_pImageHScrollArrowLeftSel = &m_imageHScrollArrowLeftSel;
		if( !m_imageHScrollArrowLeftNormal.IsNull() )
			m_pHScroll->m_pImageHScrollArrowLeftNormal = &m_imageHScrollArrowLeftNormal;
		if( !m_rcHThumbOmitBorder.IsRectNull() )
			m_pHScroll->m_rcHThumbOmitBorder = &m_rcHThumbOmitBorder;
			
		if( pOwner->AddChildControl(m_pHScroll, MercuryBaseView::GetChildControlIdUnused(m_pOwner), _Rect(0, 0, 0, m_nArrowBtnCX), this) ){
			_Rect rcFixedPos;
			rcFixedPos.left		= m_nBorderWidth;
			rcFixedPos.top		= -1;
			rcFixedPos.right	= m_nBorderWidth;
			rcFixedPos.bottom	= m_nBorderWidth;
			m_pHScroll->SetSerializeFlag(false);
			m_pHScroll->SetFixedPosition(rcFixedPos);
			m_pHScroll->SetVisible		(false, false);
            m_pHScroll->AllowMouseEvents(true);
			}
		AdjustScrollBarPos(false);
		}
	return bRet;
	}

bool
ESChildScrollView::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	AdjustScrollBarPos(bRedraw);
	return true;
	}

bool
ESChildScrollView::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	AdjustScrollBarPos(bRedraw);
	return true;
	}

bool
ESChildScrollView::Destroy(){
	ESChildControl::Destroy();

	m_pVScroll		= NULL;
	m_pHScroll		= NULL;
	return true;
	}

void
ESChildScrollView::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	AdjustScrollBarPos(false);
	}

bool
ESChildScrollView::IsScrollBarVisible(ESChildControl* pScrollBar){
	if( pScrollBar == m_pVScroll && m_pVScroll ){
		return m_pVScroll->SetScrollInfo(0, m_szView.cy, m_szPage.cy, false);
		}

	if( pScrollBar == m_pHScroll && m_pHScroll ){
		return m_pHScroll->SetScrollInfo(0, m_szView.cx, m_szPage.cx, false);
		}
	return false;
	}

int
ESChildScrollView::GetVScrollPos(){
	return m_pVScroll ? m_pVScroll->GetScrollPos() : 0;
	}

int
ESChildScrollView::GetHScrollPos(){	
	return m_pHScroll ? m_pHScroll->GetScrollPos() : 0;
	}

void
ESChildScrollView::OnMouseWheel(UINT nFlags, short zDelta, _Point pt){
	int nOffset = (zDelta / 120)*m_nLineHeight*2;
	if( m_pVScroll->IsVisible() ){
		/*
		float fStart				= 0.0f;
		float fEnd					= (float)abs(nOffset);
		m_nMotionStartPos			= m_pVScroll->GetScrollPos();
		m_nMotionEndPos				= (m_pVScroll->GetScrollPos() - nOffset);
		m_motion.AddMotionObject	(this, 400, 20, ESEasyMotionType::MotionQuadraticOut, ESEasyMotionType::MotionQuadraticIn, fStart, 0.0f, fEnd, 0.0f);
		m_motion.StartMotion		(NULL, 0L);*/
		
		m_pVScroll->SetScrollPos(m_pVScroll->GetScrollPos() - nOffset, true);
		}
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildScrollView::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcControlDC, rcInvalid;
	rcControlDC.left	= (m_rRect.left - pRectClient->left) + pRectDC->left; 
	rcControlDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top; 
	rcControlDC.right	= rcControlDC.left + m_rRect.Width(); 
	rcControlDC.bottom	= rcControlDC.top + m_rRect.Height(); 

	_Rect rcFillDC;
	if( m_nBorderWidth > 0 ){
		// Draw border.
		pDC->DrawPath(rcControlDC, m_nBorderWidth, m_crBorder);

		rcFillDC = rcControlDC;
		rcFillDC.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
		pDC->FillSolidRect(rcFillDC, m_crFill);
		}
	else
		rcFillDC = rcControlDC;
	

	if( m_pVScroll && m_pHScroll && m_pVScroll->IsVisible() && m_pHScroll->IsVisible() ){
		_Image* pImgCorner		= &MercuryGUI::GetInstance()->scrollBar.m_imgScrollBarDivider;

		_Rect rcCornerDC;
		rcCornerDC.left		= rcControlDC.right - pImgCorner->GetWidth() - m_nBorderWidth;
		rcCornerDC.top		= rcControlDC.bottom - pImgCorner->GetHeight() - m_nBorderWidth;
		rcCornerDC.right	= rcCornerDC.left + pImgCorner->GetWidth();
		rcCornerDC.bottom	= rcCornerDC.top + pImgCorner->GetHeight();

		_Rect rcInvalid		= *pRectDC&rcCornerDC;
		if( !rcInvalid.IsRectEmpty() )
			pImgCorner->RenderImage(pDC, rcInvalid, rcCornerDC, false, false);
		}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

bool
ESChildScrollView::OnScrollBarEvent(bool bVScroll, int nCode, ESChildControl* pSender){
	ESChildScrollBar* pScrollBar = (ESChildScrollBar*)pSender;
	bool bRet = true;
	switch( nCode ) {
		case SB_LINEDOWN:{
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + m_nLineHeight, true);
			break;
			}
		case SB_LINEUP:{
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - m_nLineHeight, true);
			break;
			}
		case SB_PAGEDOWN:{
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + pScrollBar->GetScrollPage(), true);
			break;
			}
		case SB_PAGEUP:{
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - pScrollBar->GetScrollPage(), true);
			break;
			}
		case SB_THUMBTRACK:{
			break;
			}
		};

	if( bRet )
		RedrawViewArea();
	return bRet;
	}

void
ESChildScrollView::OnMotionStart(DWORD_PTR dwParam){
	}

void
ESChildScrollView::OnMotionProgress(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY){
	int nPosNew = m_nMotionStartPos + (int)(fPosX);
	if( m_nMotionStartPos > m_nMotionEndPos ){
		nPosNew = m_nMotionStartPos - (int)(fPosX);
		}
	/*
	_string s;
	s.Format("%d", nPosNew);
	m_pOwner->GetParent()->SetWindowText(s);*/
	m_pVScroll->SetScrollPos(nPosNew, true);
	}

void
ESChildScrollView::OnMotionEnd(){
	}

void
ESChildScrollView::OnMotionStopped(){
	}

void
ESChildScrollView::AdjustScrollBarPos(bool bRedraw){
	_Rect rcView = GetClientRect();
	if( m_nBorderWidth > 0 ){
		rcView.left		+= m_nBorderWidth;
		rcView.top		+= m_nBorderWidth;
		rcView.right	-= m_nBorderWidth;
		rcView.bottom	-= m_nBorderWidth;
		}

	_Rect	rcVScroll, rcHScroll;
	bool	bVScroll		= true;
	bool	bHScroll		= true;

	if( !m_pVScroll->SetScrollInfo(0, m_szView.cy, m_szPage.cy, false) || !IsVisible() ){
		bVScroll = false;
		m_pVScroll->SetVisible(false, false);
		}

	if( !m_pHScroll->SetScrollInfo(0, m_szView.cx, m_szPage.cx, false) || !IsVisible() ){
		bHScroll = false;
		m_pHScroll->SetVisible(false, false);
		}

	if( bVScroll && bHScroll ){
		rcVScroll.left		= rcView.right - m_nArrowBtnCX;
		rcVScroll.top		= rcView.top;
		rcVScroll.right		= rcVScroll.left + m_nArrowBtnCX;
		rcVScroll.bottom	= rcView.bottom - m_nArrowBtnCY;

		rcHScroll.left		= rcView.left;
		rcHScroll.top		= rcView.bottom - m_nArrowBtnCY;
		rcHScroll.right		= rcView.right - m_nArrowBtnCX;
		rcHScroll.bottom	= rcHScroll.top + m_nArrowBtnCY;
		}
	else{
		rcVScroll.left		= rcView.right - m_nArrowBtnCX;
		rcVScroll.top		= rcView.top;
		rcVScroll.right		= rcVScroll.left + m_nArrowBtnCX;
		rcVScroll.bottom	= rcView.bottom;

		rcHScroll.left		= rcView.left;
		rcHScroll.top		= rcView.bottom - m_nArrowBtnCY;
		rcHScroll.right		= rcView.right;
		rcHScroll.bottom	= rcHScroll.top + m_nArrowBtnCY;
		}

	if( bVScroll ){
	//	m_pVScroll->SetPos	(rcVScroll.left, rcVScroll.top, false);
	//	m_pVScroll->SetSize	(rcVScroll.Width(), rcVScroll.Height(), false);
		if( !m_pVScroll->IsVisible() )
			m_pVScroll->SetVisible(true, false);
		}

	if( bHScroll ){
	//	m_pHScroll->SetPos	(rcHScroll.left, rcHScroll.top, false);
	//	m_pHScroll->SetSize	(rcHScroll.Width(), rcHScroll.Height(), false);
		if( !m_pHScroll->IsVisible() )
			m_pHScroll->SetVisible(true, false);
		}

	if( m_pVScroll->IsVisible() && bRedraw )
		m_pVScroll->Redraw(false);

	if( m_pHScroll->IsVisible() && bRedraw )
		m_pHScroll->Redraw(false);
	}

void
ESChildScrollView::RedrawViewArea(){
	_Rect rcView = GetClientRect();
	if( m_nBorderWidth > 0 ){
		rcView.left		+= m_nBorderWidth;
		rcView.top		+= m_nBorderWidth;
		rcView.right	-= m_nBorderWidth;
		rcView.bottom	-= m_nBorderWidth;
		}
	_Rect	rcRedraw;
	rcRedraw.left		= rcView.left;
	rcRedraw.top		= rcView.top;
	rcRedraw.right		= m_pVScroll->IsVisible() ? rcView.right - m_nArrowBtnCX : rcView.right;
	rcRedraw.bottom		= rcRedraw.top + (m_pHScroll->IsVisible() ? (rcView.Height() - m_nArrowBtnCY) : rcView.Height());
	RedrawRect(rcRedraw, false);
	}

void
ESChildScrollView::EnsureItemVisibility(int nItem, bool bRedraw){
	if( !m_pVScroll || !m_pVScroll->IsVisible() )
		return;

	int nTopPos		= m_pVScroll->GetScrollPos();
	int nBottomPos	= nTopPos + m_pVScroll->GetScrollPage();
	int nPos		= nItem*m_nLineHeight;

	if( nPos >= nTopPos && nPos <= nBottomPos )
		return; // Is visible.

	m_pVScroll->SetScrollPos(nPos, bRedraw);
	}

void
ESChildScrollView::OnDeserializationCompleted(){
	ESChildControl::OnDeserializationCompleted();

	MercuryGUI* pGUI	= MercuryGUI::GetInstance();
	m_nArrowBtnCX		= pGUI->scrollBar.m_nVScrollWidth;
	m_nArrowBtnCY		= pGUI->scrollBar.m_nHScrollHeight;
	}
