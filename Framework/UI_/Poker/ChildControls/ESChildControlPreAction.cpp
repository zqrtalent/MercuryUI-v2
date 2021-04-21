// ESChildControlPreAction.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlPreAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlPreAction

ESChildControlPreAction::ESChildControlPreAction(){
	m_sText			= _T("");
	m_bMouseDown	= false;
	m_bChecked		= false;
	}

ESChildControlPreAction::~ESChildControlPreAction(){
	}

bool
ESChildControlPreAction::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlPreAction::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlPreAction::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlPreAction::Destroy(){
	ESChildControl::Destroy();
	m_bMouseDown = false;
	return true;
	}

void
ESChildControlPreAction::SetText(CString sText, bool bRedraw){
	m_sText = sText;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlPreAction::SetCheck(bool bCheck, bool bRedraw /*= true*/){
	m_bChecked = bCheck;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlPreAction::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);

	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	if( m_bLogicalPos ){
		float		fStretchCX	= (float)(cx / ((float)pGUI->m_szDefWindowSize.cx));
		float		fStretchCY	= (float)(cy / ((float)pGUI->m_szDefWindowSize.cy));
		CImage*		pImageCheck	= pGUI->m_pPreActionChecked;

		if( pImageCheck ){
			m_szCheckImage.cx	= pImageCheck->GetWidth();// * fStretchCX;
			m_szCheckImage.cy	= pImageCheck->GetHeight();// * fStretchCY;
			}
		}
	else{
		CImage*		pImageCheck	= pGUI->m_pPreActionChecked;
		if( pImageCheck ){
			m_szCheckImage.cx	= pImageCheck->GetWidth();// * fStretchCX;
			m_szCheckImage.cy	= pImageCheck->GetHeight();// * fStretchCY;
			}
		}
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlPreAction::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	CImage*		pCheckImage	= m_bChecked ? pGUI->m_pPreActionChecked : pGUI->m_pPreActionUnchecked;
	CImage*		pBgImage	= pGUI->m_pPreActionBg;

	CRect rcOwner;
	m_pOwner->GetClientRect(rcOwner);
	float fStretchCX = m_bLogicalPos ? ((float)rcOwner.Width()) / pGUI->m_szDefWindowSize.cx : 1.0f;
	float fStretchCY = m_bLogicalPos ? ((float)rcOwner.Height()) / pGUI->m_szDefWindowSize.cy : 1.0f;

	if( !m_bLogicalPos && pCheckImage ){
		m_szCheckImage.cx	= pCheckImage->GetWidth();// * fStretchCX;
		m_szCheckImage.cy	= pCheckImage->GetHeight();// * fStretchCY;
		}

	CRect	rcPreActionDC, rcInvalid, rcCheckDC;
	rcPreActionDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcPreActionDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcPreActionDC.right		= rcPreActionDC.left + m_rcRect.Width(); 
	rcPreActionDC.bottom	= rcPreActionDC.top + m_rcRect.Height(); 

	rcCheckDC.left			= rcPreActionDC.left + 2;
	rcCheckDC.top			= rcPreActionDC.top + (m_rcRect.Height() - m_szCheckImage.cy)/2;
	rcCheckDC.right			= rcCheckDC.left + m_szCheckImage.cx;
	rcCheckDC.bottom		= rcCheckDC.top + m_szCheckImage.cy;

	// Draw background. {{
	rcInvalid	= rcPreActionDC & *pRectDC;
	if( pBgImage && !rcInvalid.IsRectEmpty() ){
		CRect rcOmitBorder(2, 2, 2, 2);
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcPreActionDC, pBgImage, rcOmitBorder);
		}
	// }}

	// Draw check image. {{
	rcInvalid	= rcCheckDC & *pRectDC;
	if( pBgImage && !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcCheckDC, pCheckImage);
	// }}

	// Draw label. {{
	CFont	font;
	PokerGUI::StretchedTextFont(pGUI->m_pPreActionFont, &font, fStretchCX, fStretchCY);
	CFont* pFontOld = pDC->SelectObject(&font);
	CSize szText;
	ImgHlp::GetTextSize(&m_sText, &font, szText);

	CPoint ptLabel		(rcCheckDC.right + 2, rcPreActionDC.top + (m_rcRect.Height() - szText.cy)/2);
	pDC->SetBkMode		(TRANSPARENT);
	pDC->SetTextColor	(pGUI->m_crPreActionLabel);
	PokerGUI::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcPreActionDC, m_sText);
//	pDCPokerGUI::ExtTextOut		(ptLabel.x, ptLabel.y, ETO_CLIPPED, rcPreActionDC, m_sText.GetBuffer(), NULL);
	pDC->SelectObject	(pFontOld);
	font.DeleteObject	();
	// }}
	}

void	
ESChildControlPreAction::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlPreAction::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return 1;
	}

void	
ESChildControlPreAction::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlPreAction::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlPreAction::OnLButtonDown(CPoint pt, UINT nFlags){
	m_bMouseDown	= true;
	}

void
ESChildControlPreAction::OnLButtonUp(CPoint pt, UINT nFlags){
	m_bMouseDown = false;
	}

void	
ESChildControlPreAction::OnLButtonClick(CPoint pt, UINT nFlags){
	m_bChecked		= !m_bChecked;
	if( m_pOwner ){
		m_pOwner->SendMessage(WM_PREACTION_CHECKED, (WPARAM)GetId(), (LPARAM)this);
		}
	Redraw();
	}

void	
ESChildControlPreAction::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlPreAction::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlPreAction::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlPreAction::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlPreAction::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}