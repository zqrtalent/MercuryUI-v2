// ESChildControlSeatProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlSeatProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeatProgressBar

ESChildControlSeatProgressBar::ESChildControlSeatProgressBar(){
	m_bActive			= false;
	m_nPos				= 0;
	m_nPosMax			= 0;
	}

ESChildControlSeatProgressBar::~ESChildControlSeatProgressBar(){
	}

bool
ESChildControlSeatProgressBar::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlSeatProgressBar::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlSeatProgressBar::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlSeatProgressBar::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlSeatProgressBar::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	if( !m_rcRect.IsRectEmpty() && IsVisible() ){
		PokerGUI* pGUI = PokerGUI::GetInstance();
		if( m_rcRect.Height() > pGUI->m_nMaxProgressBarCY ){
			m_rcRect.bottom = m_rcRect.top + pGUI->m_nMaxProgressBarCY;
			}
		else
		if( m_rcRect.Height() < pGUI->m_nMinProgressBarCY ){
			m_rcRect.bottom = m_rcRect.top + pGUI->m_nMinProgressBarCY;
			}
		}
	}

bool
ESChildControlSeatProgressBar::SetProgressPos(int nPos, int nPosMax, bool bRedraw /*= true*/){
	m_nPos		= nPos;
	m_nPosMax	= nPosMax;

	if( bRedraw ){
		Redraw();
		}
	return true;
	}


void
ESChildControlSeatProgressBar::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CImage*		pPBarImage		= pGUI->m_pPBarBgImg;
	CImage*		pPBarFillImage	= pGUI->m_pPBarFillImg;
	if( !pPBarImage || !pPBarFillImage )
		return;

	int nHeight = m_rcRect.Height();
	if( m_rcRect.Height() > pGUI->m_nMaxProgressBarCY )
		nHeight =  pGUI->m_nMaxProgressBarCY;
	else
	if( m_rcRect.Height() < pGUI->m_nMinProgressBarCY )
		nHeight =  pGUI->m_nMinProgressBarCY;

	float fProgressFilled = m_nPosMax > 0 ? (float)(m_nPos / (float)m_nPosMax) : 0.0f;
	CRect	rcPBarFillDC, rcInvalid;
	rcPBarFillDC.left	= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcPBarFillDC.top	= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcPBarFillDC.right	= rcPBarFillDC.left + ((int)m_rcRect.Width()*fProgressFilled); 
	rcPBarFillDC.bottom	= rcPBarFillDC.top + nHeight; 

	CRect rcOmitBorder(8, 3, 8, 3);

	// Draw progress fill. {{
	rcInvalid	= rcPBarFillDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() ){
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcPBarFillDC, pPBarFillImage, &rcOmitBorder);
		}
	// }}

	CRect	rcPBarDC;
	rcPBarDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcPBarDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcPBarDC.right		= rcPBarDC.left + m_rcRect.Width(); 
	rcPBarDC.bottom		= rcPBarDC.top + nHeight; 

	// Draw progress background. {{
	rcInvalid	= rcPBarDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcPBarDC, pPBarImage, &rcOmitBorder);
	// }}
	}

void	
ESChildControlSeatProgressBar::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlSeatProgressBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return FALSE;
	}

void	
ESChildControlSeatProgressBar::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlSeatProgressBar::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlSeatProgressBar::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlSeatProgressBar::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlSeatProgressBar::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlSeatProgressBar::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlSeatProgressBar::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlSeatProgressBar::OnLButtonDblClick(CPoint pt, UINT nFlags){
	OnLButtonDown(pt, nFlags);
	}

void	
ESChildControlSeatProgressBar::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlSeatProgressBar::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}