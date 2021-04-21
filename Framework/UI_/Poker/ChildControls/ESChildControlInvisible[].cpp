// ESChildControlInvisible.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlInvisible.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlInvisible

ESChildControlInvisible::ESChildControlInvisible(){
	}

ESChildControlInvisible::~ESChildControlInvisible(){
	}

bool
ESChildControlInvisible::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return bRet;
	}

bool
ESChildControlInvisible::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlInvisible::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

void
ESChildControlInvisible::SetLogicalPos(float fPosX, float fPosY){
	ESChildControl::SetLogicalPos(fPosX, fPosY);
	}

void
ESChildControlInvisible::SetLogicalSize(float fCX, float fCY){
	ESChildControl::SetLogicalSize(fCX, fCY);
	}

void
ESChildControlInvisible::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildControlInvisible::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlInvisible::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	}

void	
ESChildControlInvisible::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlInvisible::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlInvisible::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlInvisible::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlInvisible::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlInvisible::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlInvisible::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlInvisible::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlInvisible::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlInvisible::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlInvisible::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlInvisible::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}