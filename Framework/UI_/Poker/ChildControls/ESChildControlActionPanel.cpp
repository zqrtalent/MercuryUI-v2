// ESChildControlActionPanel.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlActionPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlActionPanel

ESChildControlActionPanel::ESChildControlActionPanel(){
	m_fPosX		= 0.0f;
	m_fPosY		= 0.80f;
	m_fCX		= 1.0f;
	m_fCY		= 0.2f;
	}

ESChildControlActionPanel::~ESChildControlActionPanel(){
	}

bool
ESChildControlActionPanel::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlActionPanel::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return false;
	}

bool
ESChildControlActionPanel::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return false;
	}

bool
ESChildControlActionPanel::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlActionPanel::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	CImage*		pImage	= pGUI->m_pActionPanelBg;
	if( !pImage ) return;

	CRect	rcControl;
	rcControl.left			= (pRectClient->left - m_rcRect.left);
	rcControl.top			= (pRectClient->top - m_rcRect.top);
	rcControl.right			= rcControl.left + pRectClient->Width();
	rcControl.bottom		= rcControl.top + pRectClient->Height();

	CRect	rcActionPanel	= m_rcRect;
	CRect	rcImage			(0, 0, pImage->GetWidth(), pImage->GetHeight());
	
	CRect	rcSrcImage;
	rcSrcImage.left			= rcControl.left % rcImage.Width();
	rcSrcImage.top			= rcControl.top % rcImage.Height();
	rcSrcImage.right		= rcImage.Width();
	rcSrcImage.bottom		= rcImage.Height();

//	ImgHlp::DrawAlphaImage(pDC, *pImage, rcSrcImage.left, rcSrcImage.top, rcImage.Width(), rcImage.Height(), pRectDC);
	}

void	
ESChildControlActionPanel::OnMouseHover(CPoint pt, UINT nFlags){
	}

void	
ESChildControlActionPanel::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlActionPanel::OnMouseEnter(UINT nFlags){
	}

void	
ESChildControlActionPanel::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlActionPanel::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlActionPanel::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlActionPanel::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlActionPanel::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlActionPanel::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}