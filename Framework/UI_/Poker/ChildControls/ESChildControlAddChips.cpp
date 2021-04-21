// ESChildControlAddChips.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlButton.h"
#include "ESChildControlMoneyEdit.h"
#include "..\..\..\Controls\ESChildControlLabel.h"
#include "ESChildControlAddChips.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlAddChips

ESChildControlAddChips::ESChildControlAddChips(){
	m_bMouseDown	= false;
	m_nPos			= 100;
	m_nMin			= 50;
	m_nMax			= 500;
	m_nStep			= 10;
	m_nBalance		= 0;
	m_pButton		= NULL;
	m_pMoneyEdit	= NULL;
	m_bCloseButton	= false;

	m_ptCursorPos.SetPoint(0, 0);
	m_bCursorCaptured	= false;
	}

ESChildControlAddChips::~ESChildControlAddChips(){
	}

bool
ESChildControlAddChips::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		PokerGUI*	pGUI	= PokerGUI::GetInstance(); 
		/*
		CString		sLabel;
		CPoint		ptButton	(20, 70);
		CPoint		ptEdit		(20, 6);
		CSize		szEdit		(pGUI->m_szActionButton.cx, pGUI->m_rcSliderMoneyEdit.Height());
		CSize		szButton	(pGUI->m_szActionButton.cx, 36);
		CPoint		ptLabel		(220, 6);
		CSize		szLabel		(120, 20);
		int			nDistanceLabelY = 6;

		// Add money edit control. {{
		CRect rcMoneyEdit;//	= m_pGUI->m_rcSliderMoneyEdit;
		rcMoneyEdit.left	=  pGUI->m_rcAddChips.left + ptEdit.x;
		rcMoneyEdit.top		=  pGUI->m_rcAddChips.top + ptEdit.y;
		rcMoneyEdit.right	= rcMoneyEdit.left + szEdit.cx;
		rcMoneyEdit.bottom	= rcMoneyEdit.top + szEdit.cy;
		m_pMoneyEdit = new ESChildControlMoneyEdit();
		pOwner->AddChildControlEx(m_pMoneyEdit, CHILD_AddChipsMoneyEdit, rcMoneyEdit, this, pGUI->m_szDefWindowSize); 
		m_pButton->SetVisible		(true, false);
		// }}
		
		// Add button control. {{
		CRect rcButton;
		rcButton.left	= pGUI->m_rcAddChips.left + ptButton.x;
		rcButton.top	= pGUI->m_rcAddChips.top + ptButton.y;
		rcButton.right	= rcButton.left + szButton.cx;
		rcButton.bottom = rcButton.top + szButton.cy;

		ASSERT( pOwner );
		m_pButton = new ESChildControlButton();
		pOwner->AddChildControlEx	(m_pButton, CHILD_AddChipsButton, rcButton, this, pGUI->m_szDefWindowSize);
		m_pButton->SetText			(PokerGUI::GetLangValue(_T("ADD_CHIPS_LABEL")), false);
		m_pButton->SetVisible		(true, false);
		// }}

		// Add labels. {{
		CRect rcLabel;
		rcLabel.left	= pGUI->m_rcAddChips.left + ptLabel.x;
		rcLabel.top		= pGUI->m_rcAddChips.top + ptLabel.y;
		rcLabel.right	= rcLabel.left + szLabel.cx;
		rcLabel.bottom	= rcLabel.top + szLabel.cy;

		// Min
		sLabel.Format(_T("%s: %d.%02d"),PokerGUI::GetLangValue(_T("MIN_LABEL")), m_nMin/100, m_nMin%100);
		ESChildControlLabel* pLabel = new ESChildControlLabel();
		pOwner->AddChildControlEx	(pLabel, CHILD_AddChipsMinLabel, rcLabel, this, pGUI->m_szDefWindowSize);
		pLabel->SetLabel			(sLabel, false);
		pLabel->SetFont				(pGUI->m_pTotalPotLabelFont);
		pLabel->SetTextColor		(RGB(255, 255, 255));
		pLabel->SetVisible			(true, false);
		pLabel->SetCentred			(false);

		// Max
		sLabel.Format(_T("%s: %d.%02d"),PokerGUI::GetLangValue(_T("MAX_LABEL")), m_nMax/100, m_nMax%100);
		rcLabel.top			+= szLabel.cy + nDistanceLabelY;
		rcLabel.bottom		= rcLabel.top + szLabel.cy;
		pLabel = new ESChildControlLabel();
		pOwner->AddChildControlEx	(pLabel, CHILD_AddChipsMaxLabel, rcLabel, this, pGUI->m_szDefWindowSize);
		pLabel->SetLabel			(sLabel, false);
		pLabel->SetFont				(pGUI->m_pTotalPotLabelFont);
		pLabel->SetTextColor		(RGB(255, 255, 255));
		pLabel->SetVisible			(true, false);
		pLabel->SetCentred			(false);

		// Balance
		sLabel.Format(_T("%s: %d.%02d"),PokerGUI::GetLangValue(_T("BALANCE_LABEL")), m_nBalance/100, m_nBalance%100);
		rcLabel.top			+= szLabel.cy + nDistanceLabelY;
		rcLabel.bottom		= rcLabel.top + szLabel.cy;
		pLabel = new ESChildControlLabel();
		pOwner->AddChildControlEx	(pLabel, CHILD_AddChipsBalanceLabel, rcLabel, this, pGUI->m_szDefWindowSize);
		pLabel->SetLabel			(sLabel, false);
		pLabel->SetFont				(pGUI->m_pTotalPotLabelFont);
		pLabel->SetTextColor		(RGB(255, 255, 255));
		pLabel->SetVisible			(true, false);
		pLabel->SetCentred			(false);
		// }}*/
		}
	return bRet;
	}

bool
ESChildControlAddChips::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlAddChips::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlAddChips::Destroy(){
	if( !m_imageScreen.IsNull() ){
		DeleteObject(m_imageScreen.Detach());
		}

	if( m_pButton ){
		m_pOwner->RemoveChildControl(m_pButton);
		m_pButton = NULL;
		}

	if( m_pMoneyEdit ){
		m_pOwner->RemoveChildControl(m_pMoneyEdit);
		m_pMoneyEdit = NULL;
		}

	ESChildControl::Destroy();
	m_bMouseDown = false;
	return true;
	}

void
ESChildControlAddChips::OwnerWindowSizeChanged(int cx, int cy){
	CRect rcRectOld = m_rcRect;
	ESChildControl::OwnerWindowSizeChanged(cx, cy);

	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	// Calculate positions. {{
	float		fStretchCX		= m_rcRect.Width() / ((float)pGUI->m_szSlider.cx);
	float		fStretchCY		= m_rcRect.Height() / ((float)pGUI->m_szSlider.cy);
	int			nEdgeWidth		= 4;
	int			nSliderWidth	= 130;
	int			nSliderX		= 18*fStretchCX;
	int			nCloseImageCX	= pGUI->m_pCloseButton ? pGUI->m_pCloseButton->GetWidth() : 0;
	int			nCloseImageCY	= pGUI->m_pCloseButton ? pGUI->m_pCloseButton->GetHeight() : 0;

	m_rcCloseButton.left	= m_rcRect.right - 2*nEdgeWidth - nCloseImageCX;
	m_rcCloseButton.top		= m_rcRect.top + 2*nEdgeWidth;
	m_rcCloseButton.right	= m_rcCloseButton.left + nCloseImageCX;
	m_rcCloseButton.bottom	= m_rcCloseButton.top + nCloseImageCY;

	m_rcSlider.left			= m_rcRect.left + nEdgeWidth;
	m_rcSlider.top			= m_rcRect.top + nSliderX;
	m_rcSlider.right		= m_rcSlider.left + nSliderWidth*fStretchCX;
	m_rcSlider.bottom		= m_rcSlider.top + pGUI->m_szSlider.cy;

	m_rcMinus.left			= m_rcSlider.left + nEdgeWidth;
	m_rcMinus.top			= m_rcSlider.top + (m_rcSlider.Height() - pGUI->m_szSliderMinus.cy)/2;
	m_rcMinus.right			= m_rcMinus.left + pGUI->m_szSliderMinus.cx;
	m_rcMinus.bottom		= m_rcMinus.top + pGUI->m_szSliderMinus.cy;

	m_rcPlus.right			= m_rcSlider.right - nEdgeWidth;
	m_rcPlus.left			= m_rcPlus.right - pGUI->m_szSliderPlus.cx;
	m_rcPlus.top			= m_rcSlider.top + (m_rcSlider.Height() - pGUI->m_szSliderPlus.cy)/2;
	m_rcPlus.bottom			= m_rcPlus.top + pGUI->m_szSliderPlus.cy;

	m_rcSliderBar.left		= m_rcMinus.right + nEdgeWidth;
	m_rcSliderBar.top		= m_rcSlider.top + (m_rcSlider.Height() - pGUI->m_szSliderBar.cy)/2;
	m_rcSliderBar.right		= m_rcPlus.left - nEdgeWidth;
	m_rcSliderBar.bottom	= m_rcSliderBar.top + pGUI->m_szSliderBar.cy;

	m_rcCursor.left			= m_rcSliderBar.left;
	m_rcCursor.top			= m_rcSlider.top + (m_rcSlider.Height() - pGUI->m_szSliderCursor.cy);
	m_rcCursor.right		= m_rcCursor.left + pGUI->m_szSliderCursor.cx;
	m_rcCursor.bottom		= m_rcCursor.top + pGUI->m_szSliderCursor.cy;
	CalculateCursorPos(false);

	if( rcRectOld != m_rcRect && !m_imageScreen.IsNull() ){
		DeleteObject(m_imageScreen.Detach());
		}
	}

bool
ESChildControlAddChips::Setup(int nMin, int nMax, int nStep, int nBalance, bool bInvalidate /*= true*/){
	if( nMin > nMax ) return false;
	m_nMin		= nMin;
	m_nMax		= nMax;
	m_nStep		= nStep;
	m_nPos		= m_nMin;
	m_nBalance	= nBalance;
	CalculateCursorPos(false);
	/*
	// Set label titles.
	ESChildControlLabel*	pLabel;
	CString					sLabel;
	// Min
	sLabel.Format(_T("%s: %d.%02d"),PokerGUI::GetLangValue(_T("MIN_LABEL")), m_nMin/100, m_nMin%100);
	pLabel = (ESChildControlLabel*)m_pOwner->GetChildControl(CHILD_AddChipsMinLabel);
	if( pLabel )
		pLabel->SetLabel(sLabel, false);

	// Max
	sLabel.Format(_T("%s: %d.%02d"),PokerGUI::GetLangValue(_T("MAX_LABEL")), m_nMax/100, m_nMax%100);
	pLabel = (ESChildControlLabel*)m_pOwner->GetChildControl(CHILD_AddChipsMaxLabel);
	if( pLabel )
		pLabel->SetLabel(sLabel, false);

	// Balance
	sLabel.Format(_T("%s: %d.%02d"),PokerGUI::GetLangValue(_T("BALANCE_LABEL")), m_nBalance/100, m_nBalance%100);
	pLabel = (ESChildControlLabel*)m_pOwner->GetChildControl(CHILD_AddChipsBalanceLabel);
	if( pLabel )
		pLabel->SetLabel(sLabel, false);
		
	if( bInvalidate ){
		Redraw();
		}*/
	return true;
	}

bool
ESChildControlAddChips::SetSliderPos(int nPos, bool bInvalidate /*= true*/, bool bReflectEvent /*= true*/){
	if( nPos > m_nMax || nPos < m_nMin )
		return false;
	m_nPos = nPos;
	CalculateCursorPos(false);
	if( bInvalidate ){
		Redraw();
		}

	if( bReflectEvent )
		m_pOwner->SendMessage(WM_SLIDER_POS_CHANGED, (WPARAM)m_nPos, (LPARAM)this);
	return true;
	}

bool
ESChildControlAddChips::CalculateCursorPos(bool bInvalidate){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	int			nWidth	= m_rcCursor.Width();
	int			nHeight = m_rcCursor.Height();

	if( m_nPos < m_nMin ){
		m_nPos				= m_nMin;
		m_rcCursor.left		= m_rcSliderBar.left;
		m_rcCursor.top		= m_rcSlider.top + (m_rcSlider.Height() - nHeight);
		m_rcCursor.right	= m_rcCursor.left + nWidth;
		m_rcCursor.bottom	= m_rcCursor.top + nHeight;
		return false;
		}

	if( m_nPos > m_nMax ){
		m_nPos				= m_nMax;
		m_rcCursor.right	= m_rcSliderBar.right;
		m_rcCursor.left		= m_rcCursor.right - nWidth;
		m_rcCursor.top		= m_rcSlider.top + (m_rcSlider.Height() - nHeight);
		m_rcCursor.bottom	= m_rcCursor.top + nHeight;
		return false;
		}

	float fDelta		= (float)((m_rcSliderBar.Width() - m_rcCursor.Width()) / ((m_nMax - m_nMin)*1.0f));
	m_ptCursorPos.x		= m_rcSliderBar.left + (LONG)(fDelta*(m_nPos - m_nMin));
	m_ptCursorPos.y		= m_rcCursor.top;

	CRect	rcCursorOld	= m_rcCursor;

	m_rcCursor.left		= m_ptCursorPos.x;
	m_rcCursor.top		= m_rcSlider.top + (m_rcSlider.Height() - nHeight);
	m_rcCursor.right	= m_rcCursor.left + nWidth;
	m_rcCursor.bottom	= m_rcCursor.top + nHeight;
	
	if( bInvalidate ){
		m_pOwner->RedrawRect(rcCursorOld, 1);
		m_pOwner->RedrawRect(m_rcCursor, 1);
		}
	return true;
	}

bool
ESChildControlAddChips::CalculateCursorPosByPoint(CPoint pt){
	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	float		fDelta		= (float)((m_rcSliderBar.Width()) / ((m_nMax - m_nMin)*1.0f));
	m_nPos					= m_nMin + (int)((pt.x - m_rcSliderBar.left)/fDelta);
	CalculateCursorPos(true);
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlAddChips::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CImage*		pImageBg		= pGUI->m_pAddChipskgnd;
	CImage*		pImageBar		= pGUI->m_pSliderBarBg;
	CImage*		pImageMinus		= pGUI->m_pSliderMinus;
	CImage*		pImagePlus		= pGUI->m_pSliderPlus;
	CImage*		pImageCursor	= pGUI->m_pSliderCursor;
	CImage*		pImageCloseBtn	= pGUI->m_pCloseButton;

	bool bCanSaveScreen = (pRectDC->Width() == m_rcRect.Width() && pRectDC->Height() == m_rcRect.Height());

	bool	bRestoreScreen		= false;
	bool	bSaveScreenImage	= m_bAllowToSaveScreen;
	CSize szScreenImage(0, 0);
	if( m_bAllowToSaveScreen && !m_imageScreen.IsNull() ){
		szScreenImage.SetSize(m_imageScreen.GetWidth(), m_imageScreen.GetHeight());
		if( szScreenImage.cx == m_rcRect.Width() || szScreenImage.cy == m_rcRect.Height() ){
			bRestoreScreen = true;
			}
		}

	// Restore background.
	CRect	rcBkgndDC, rcInvalid;
	rcBkgndDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcBkgndDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcBkgndDC.right		= rcBkgndDC.left + m_rcRect.Width(); 
	rcBkgndDC.bottom	= rcBkgndDC.top + m_rcRect.Height(); 

	if( bRestoreScreen ){
		rcInvalid = rcBkgndDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() ){
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			::SelectObject(memDC.m_hDC, (HBITMAP)m_imageScreen);

			CPoint ptSrc;
			ptSrc.x = (pRectClient->left - m_rcRect.left);
			ptSrc.y = (pRectClient->top - m_rcRect.top);
			pDC->BitBlt(rcInvalid.left, rcInvalid.top, rcInvalid.Width(), rcInvalid.Height(), &memDC, ptSrc.x, ptSrc.y, SRCCOPY);
			memDC.DeleteDC();
			}
		}
	else{
		// Draw background. {{
		rcInvalid = rcBkgndDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() )
			PokerGUI::RenderImage(pDC, rcInvalid, rcBkgndDC, pImageBg, true);
		// }}

		// Save screen image.
		if( bSaveScreenImage && bCanSaveScreen ){
			if( !m_imageScreen.IsNull() )
				::DeleteObject(m_imageScreen.Detach());

			CBitmap bmScreen;
			if( ImageHelper::GetBitmap24bppFromDC(bmScreen, pDC, rcBkgndDC.left, rcBkgndDC.top, rcBkgndDC.Width(), rcBkgndDC.Height()) ){
				m_imageScreen.Attach((HBITMAP)bmScreen.Detach());
				}
			}
		}
	

	// Draw close button. {{
	CRect rcCloseButtonDC;
	rcCloseButtonDC.left	= (m_rcCloseButton.left - pRectClient->left) + pRectDC->left; 
	rcCloseButtonDC.top		= (m_rcCloseButton.top - pRectClient->top) + pRectDC->top; 
	rcCloseButtonDC.right	= rcCloseButtonDC.left + m_rcCloseButton.Width(); 
	rcCloseButtonDC.bottom	= rcCloseButtonDC.top + m_rcCloseButton.Height(); 
	
	rcInvalid = rcCloseButtonDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcCloseButtonDC, pImageCloseBtn, true);
	// }}

	CRect rcOmitBorder(0, 0, 0, 0);

	// Draw minus sign. {{
	CRect	rcMinusDC;
	rcMinusDC.left		= (m_rcMinus.left - pRectClient->left) + pRectDC->left; 
	rcMinusDC.top		= (m_rcMinus.top - pRectClient->top) + pRectDC->top; 
	rcMinusDC.right		= rcMinusDC.left + m_rcMinus.Width(); 
	rcMinusDC.bottom	= rcMinusDC.top + m_rcMinus.Height(); 

	rcInvalid = rcMinusDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcMinusDC, pImageMinus, &rcOmitBorder);
	// }}

	// Draw slider bar. {{
	CRect	rcSliderBarDC;
	rcSliderBarDC.left		= (m_rcSliderBar.left - pRectClient->left) + pRectDC->left; 
	rcSliderBarDC.top		= (m_rcSliderBar.top - pRectClient->top) + pRectDC->top; 
	rcSliderBarDC.right		= rcSliderBarDC.left + m_rcSliderBar.Width(); 
	rcSliderBarDC.bottom	= rcSliderBarDC.top + m_rcSliderBar.Height(); 

	rcInvalid = rcSliderBarDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcSliderBarDC, pImageBar, true);
	// }}

	// Draw cursor. {{
	CRect	rcCursorDC;
	rcCursorDC.left			= (m_rcCursor.left - pRectClient->left) + pRectDC->left; 
	rcCursorDC.top			= (m_rcCursor.top - pRectClient->top) + pRectDC->top; 
	rcCursorDC.right		= rcCursorDC.left + m_rcCursor.Width(); 
	rcCursorDC.bottom		= rcCursorDC.top + m_rcCursor.Height(); 

	rcInvalid = rcCursorDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcCursorDC, pImageCursor, false);
	// }}

	// Draw plus sign. {{
	CRect	rcPlusDC;
	rcPlusDC.left		= (m_rcPlus.left - pRectClient->left) + pRectDC->left; 
	rcPlusDC.top		= (m_rcPlus.top - pRectClient->top) + pRectDC->top; 
	rcPlusDC.right		= rcPlusDC.left + m_rcPlus.Width(); 
	rcPlusDC.bottom		= rcPlusDC.top + m_rcPlus.Height(); 

	rcInvalid = rcPlusDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcPlusDC, pImagePlus, &rcOmitBorder);
	// }}
	}

void	
ESChildControlAddChips::OnMouseHover(CPoint pt, UINT nFlags){
	if( m_bCursorCaptured ){
		if( CalculateCursorPosByPoint(pt.x) ){
			m_pOwner->SendMessage(WM_SLIDER_POS_CHANGED, (WPARAM)m_nPos, (LPARAM)this);
			}
		}

	if( m_rcCloseButton.PtInRect(pt) ){
		m_bCloseButton = true;
		}
	else
		m_bCloseButton = false;

	if( m_rcMinus.PtInRect(pt) || m_rcPlus.PtInRect(pt)){
		m_bHandCursor = true;
		}
	else
		m_bHandCursor = false;
	}

BOOL	
ESChildControlAddChips::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	if( m_bHandCursor || m_bCloseButton ){
		SetCursor(LoadCursor(NULL, IDC_HAND));
		}
	else
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	return 1;
	}

void	
ESChildControlAddChips::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlAddChips::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlAddChips::OnLButtonDown(CPoint pt, UINT nFlags){
	if( m_rcSlider.PtInRect(pt) && !m_rcCursor.PtInRect(pt) ){
		/*
		if( CalculateCursorPosByPoint(pt) && m_pOwner ){
			m_pOwner->SendMessage(WM_SLIDER_POS_CHANGED, (WPARAM)m_nPos, (LPARAM)this);
			Redraw();
			return;
			}*/

		if( pt.x > m_rcCursor.right ){
			m_nPos += m_nStep;
			m_nPos	= min(m_nPos, m_nMax);
			CalculateCursorPos(true);
			m_pOwner->SendMessage(WM_SLIDER_POS_CHANGED, (WPARAM)m_nPos, (LPARAM)this);
			Redraw();
			}
		else
		if( pt.x < m_rcCursor.left ){
			m_nPos -= m_nStep;
			m_nPos	= max(m_nPos, m_nMin);
			CalculateCursorPos(true);
			m_pOwner->SendMessage(WM_SLIDER_POS_CHANGED, (WPARAM)m_nPos, (LPARAM)this);
			Redraw();
			}
		}
	else{
		if( m_rcPlus.PtInRect(pt) ){
			if( m_nPos < m_nMax ){
				m_nPos += m_nStep;
				m_nPos	= min(m_nPos, m_nMax);
				CalculateCursorPos(true);
				m_pOwner->SendMessage(WM_SLIDER_POS_CHANGED, (WPARAM)m_nPos, (LPARAM)this);
				Redraw();
				}
			return;
			}

		if( m_rcMinus.PtInRect(pt) ){
			if( m_nPos > m_nMin ){
				m_nPos -= m_nStep;
				m_nPos	= max(m_nPos, m_nMin);
				CalculateCursorPos(true);
				m_pOwner->SendMessage(WM_SLIDER_POS_CHANGED, (WPARAM)m_nPos, (LPARAM)this);
				Redraw();
				}
			return;
			}
		}

	if( m_rcCursor.PtInRect(pt) ){
		m_bCursorCaptured = true;
		}
	}

void
ESChildControlAddChips::OnLButtonUp(CPoint pt, UINT nFlags){
	if( m_bCursorCaptured ){
		m_bCursorCaptured = false;
		Redraw();
		}

	if( m_bCloseButton ){
		if( m_rcCloseButton.PtInRect(pt) ){
			m_pOwner->SendMessage(WM_ADD_CHIPS_CLOSE_BUTTON_CLICK, 0, 0);
			}
		m_bCloseButton = false;
		}
	}

void	
ESChildControlAddChips::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlAddChips::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlAddChips::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlAddChips::OnLButtonDblClick(CPoint pt, UINT nFlags){
	OnLButtonDown(pt, nFlags);
	}

void	
ESChildControlAddChips::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlAddChips::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}