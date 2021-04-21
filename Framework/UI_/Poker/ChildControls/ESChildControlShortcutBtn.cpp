// ESChildControlShortcutBtn.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlShortcutBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlShortcutBtn

ESChildControlShortcutBtn::ESChildControlShortcutBtn(){
	m_sText			= _T("");
	m_bMouseDown	= false;
	}

ESChildControlShortcutBtn::~ESChildControlShortcutBtn(){
	}

bool
ESChildControlShortcutBtn::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlShortcutBtn::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlShortcutBtn::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlShortcutBtn::Destroy(){
	ESChildControl::Destroy();
	m_bMouseDown = false;
	return true;
	}

void
ESChildControlShortcutBtn::SetText(CString sText, bool bRedraw){
	m_sText = sText;
	if( bRedraw ){
		Redraw();
		}
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlShortcutBtn::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	CImage*		pImage	= m_bMouseDown ? pGUI->m_pShortcutButtonClicked : pGUI->m_pShortcutButtonDefault;
	if( !pImage ) return;

	CRect	rcShortcutDC, rcInvalid;
	rcShortcutDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcShortcutDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcShortcutDC.right		= rcShortcutDC.left + m_rcRect.Width(); 
	rcShortcutDC.bottom		= rcShortcutDC.top + m_rcRect.Height(); 

	CRect rcOwner;
	m_pOwner->GetClientRect(rcOwner);
	float fStretchCX		= ((float)rcOwner.Width()) / pGUI->m_szDefWindowSize.cx;
	float fStretchCY		= ((float)rcOwner.Height()) / pGUI->m_szDefWindowSize.cy;

	// Draw background. {{
	rcInvalid	= rcShortcutDC & *pRectDC;
	if( pImage && !rcInvalid.IsRectEmpty() ){
		CRect rcOmitBorder(10, 10, 10, 10);
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcShortcutDC, pImage, rcOmitBorder);
		//PokerGUI::RenderImage(pDC, rcInvalid, rcShortcutDC, pImage);
		}
	// }}

	// Draw button label. {{
	CFont	font;
	PokerGUI::StretchedTextFont(pGUI->m_pShortcutButtFont, &font, fStretchCX, fStretchCY);
	CFont* pFontOld = pDC->SelectObject(&font);
	CSize szText;
	ImgHlp::GetTextSize(&m_sText, &font, szText);

	CPoint ptLabel		(rcShortcutDC.left + (m_rcRect.Width() - szText.cx)/2, rcShortcutDC.top + (m_rcRect.Height() - szText.cy)/2);
	pDC->SetBkMode		(TRANSPARENT);
	pDC->SetTextColor	(m_bMouseDown ? pGUI->m_crShortcutButtTextClicked : pGUI->m_crShortcutButtText);
	PokerGUI::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcShortcutDC, m_sText);
	pDC->SelectObject	(pFontOld);
	font.DeleteObject	();
	// }}
	}

void	
ESChildControlShortcutBtn::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlShortcutBtn::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return 1;
	}

void	
ESChildControlShortcutBtn::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlShortcutBtn::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlShortcutBtn::OnLButtonDown(CPoint pt, UINT nFlags){
	m_bMouseDown = true;
	Redraw();
	}

void
ESChildControlShortcutBtn::OnLButtonUp(CPoint pt, UINT nFlags){
	m_bMouseDown = false;
	Redraw();
	}

void	
ESChildControlShortcutBtn::OnLButtonClick(CPoint pt, UINT nFlags){
	if( m_pOwner ){
		m_pOwner->SendMessage(WM_SHORTCUT_BUTTON_CLICK, (WPARAM)GetId(), NULL);
		}
	}

void	
ESChildControlShortcutBtn::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlShortcutBtn::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlShortcutBtn::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlShortcutBtn::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlShortcutBtn::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}