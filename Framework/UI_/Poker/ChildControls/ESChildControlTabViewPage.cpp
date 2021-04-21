// ESChildControlTabViewPage.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlTabViewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabViewPage

ESChildControlTabViewPage::ESChildControlTabViewPage(){
	m_crFill		= RGB(0, 0, 122);
	m_bLogicalPos	= false;
	}

ESChildControlTabViewPage::~ESChildControlTabViewPage(){
	}

bool
ESChildControlTabViewPage::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlTabViewPage::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlTabViewPage::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlTabViewPage::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlTabViewPage::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	pDC->FillSolidRect(*pRectDC, m_crFill);
	}

void	
ESChildControlTabViewPage::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlTabViewPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlTabViewPage::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlTabViewPage::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlTabViewPage::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlTabViewPage::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewPage::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewPage::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewPage::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewPage::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewPage::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewPage::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}