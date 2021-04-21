// ESChildControlFlatButton.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlFlatButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlFlatButton

ESChildControlFlatButton::ESChildControlFlatButton(){
	m_sText			= _T("");
	m_bUnderLine	= true;
	m_bMouseDown	= false;
	m_bHover		= false;
	}

ESChildControlFlatButton::~ESChildControlFlatButton(){
	}

bool
ESChildControlFlatButton::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlFlatButton::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlFlatButton::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlFlatButton::Destroy(){
	ESChildControl::Destroy();
	m_bMouseDown = false;
	return true;
	}

void
ESChildControlFlatButton::SetText(CString sText, bool bRedraw){
	m_sText = sText;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlFlatButton::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlFlatButton::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	CImage*		pBgImage	= pGUI->m_pPreActionBg;

	CRect rcOwner;
	m_pOwner->GetClientRect(rcOwner);
	float fStretchCX = ((float)rcOwner.Width()) / pGUI->m_szDefWindowSize.cx;
	float fStretchCY = ((float)rcOwner.Height()) / pGUI->m_szDefWindowSize.cy;

	CRect	rcFlatButtonDC, rcInvalid;
	rcFlatButtonDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcFlatButtonDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcFlatButtonDC.right	= rcFlatButtonDC.left + m_rcRect.Width(); 
	rcFlatButtonDC.bottom	= rcFlatButtonDC.top + m_rcRect.Height(); 

	// Draw background. {{
	rcInvalid	= rcFlatButtonDC & *pRectDC;
	if( pBgImage && !rcInvalid.IsRectEmpty() ){
		CRect rcOmitBorder(2, 2, 2, 2);
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcFlatButtonDC, pBgImage, rcOmitBorder);
		}
	// }}

	// Draw label. {{
	CFont	font;
	PokerGUI::StretchedTextFont(pGUI->m_pPreActionFont, &font, fStretchCX, fStretchCY, (m_bHover ? m_bUnderLine : false));
	CFont* pFontOld = pDC->SelectObject(&font);
	CSize szText;
	ImgHlp::GetTextSize(&m_sText, &font, szText);

	CPoint ptLabel		(rcFlatButtonDC.left + (rcFlatButtonDC.Width() - szText.cx)/2, rcFlatButtonDC.top + (m_rcRect.Height() - szText.cy)/2);
	pDC->SetBkMode		(TRANSPARENT);
	pDC->SetTextColor	(pGUI->m_crPreActionLabel);
	PokerGUI::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcFlatButtonDC, m_sText);
//	pDCPokerGUI::ExtTextOut		(ptLabel.x, ptLabel.y, ETO_CLIPPED, rcFlatButtonDC, m_sText.GetBuffer(), NULL);
	pDC->SelectObject	(pFontOld);
	font.DeleteObject	();
	// }}
	}

void	
ESChildControlFlatButton::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlFlatButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return 1;
	}

void	
ESChildControlFlatButton::OnMouseLeave(UINT nFlags){
	m_bHover = false;
	if( m_bUnderLine ){
		Redraw();
		}
	}

void	
ESChildControlFlatButton::OnMouseEnter(UINT nFlags){
	m_bHover = true;
	if( m_bUnderLine ){
		Redraw();
		}
	}

void
ESChildControlFlatButton::OnLButtonDown(CPoint pt, UINT nFlags){
	m_bMouseDown	= true;
	}

void
ESChildControlFlatButton::OnLButtonUp(CPoint pt, UINT nFlags){
	m_bMouseDown = false;
	}

void	
ESChildControlFlatButton::OnLButtonClick(CPoint pt, UINT nFlags){
	if( m_pOwner ){
		m_pOwner->SendMessage(WM_FLAT_BUTTON_CLICK, (WPARAM)GetId(), (LPARAM)this);
		}
	Redraw();
	}

void	
ESChildControlFlatButton::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlatButton::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlatButton::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlatButton::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlatButton::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}