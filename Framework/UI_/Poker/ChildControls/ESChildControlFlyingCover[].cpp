// ESChildControlFlyingCover.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlFlyingCover.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlFlyingCover

ESChildControlFlyingCover::ESChildControlFlyingCover(){
	}

ESChildControlFlyingCover::~ESChildControlFlyingCover(){
	}

bool
ESChildControlFlyingCover::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlFlyingCover::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlFlyingCover::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlFlyingCover::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlFlyingCover::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	CImage*		pImage	= pGUI->m_pCardBackImg;
	if( !pImage ) return;

	CRect	rcCoverDC, rcInvalid;
	rcCoverDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcCoverDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcCoverDC.right		= rcCoverDC.left + m_rcRect.Width(); 
	rcCoverDC.bottom	= rcCoverDC.top + m_rcRect.Height();

	CRect rcOwner;
	m_pOwner->GetClientRect(rcOwner);
	float fStretchCX = ((float)rcOwner.Width()) / pGUI->m_szDefWindowSize.cx;
	float fStretchCY = ((float)rcOwner.Height()) / pGUI->m_szDefWindowSize.cy;

	// Draw button background. {{
	rcInvalid	= rcCoverDC & *pRectDC;
	if( pImage && !rcInvalid.IsRectEmpty() ){
		PokerGUI::RenderImage(pDC, rcInvalid, rcCoverDC, pImage, true, false, true);
		}
	// }}
	}

void	
ESChildControlFlyingCover::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlFlyingCover::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlFlyingCover::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlFlyingCover::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlFlyingCover::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlFlyingCover::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlyingCover::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlyingCover::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlyingCover::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlyingCover::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlyingCover::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlFlyingCover::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}