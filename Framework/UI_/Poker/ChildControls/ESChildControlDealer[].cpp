// ESChildControlDealer.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlDealer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlDealer

ESChildControlDealer::ESChildControlDealer(){
	}

ESChildControlDealer::~ESChildControlDealer(){
	}

bool
ESChildControlDealer::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlDealer::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlDealer::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlDealer::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlDealer::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	CImage*		pImage	= pGUI->m_pDealerBtnImg;
	if( !pImage ) return;

	CRect rcDealer;
	rcDealer.left	= m_rcRect.left + (m_rcRect.Width() - pImage->GetWidth()) / 2;
	rcDealer.top	= m_rcRect.top + (m_rcRect.Height() - pImage->GetHeight()) / 2;
	rcDealer.right	= rcDealer.left + pImage->GetWidth();
	rcDealer.bottom = rcDealer.top + pImage->GetHeight();

	CRect	rcDealerBtnDC, rcInvalid;
	rcDealerBtnDC.left		= (rcDealer.left - pRectClient->left) + pRectDC->left; 
	rcDealerBtnDC.top		= (rcDealer.top - pRectClient->top) + pRectDC->top; 
	rcDealerBtnDC.right		= rcDealerBtnDC.left + rcDealer.Width(); 
	rcDealerBtnDC.bottom	= rcDealerBtnDC.top + rcDealer.Height(); 

	// Draw button background. {{
	rcInvalid	= rcDealerBtnDC & *pRectDC;
	if( pImage && !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcDealerBtnDC, pImage);
	// }}
	}

void	
ESChildControlDealer::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlDealer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlDealer::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlDealer::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlDealer::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlDealer::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlDealer::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlDealer::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlDealer::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlDealer::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlDealer::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlDealer::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}