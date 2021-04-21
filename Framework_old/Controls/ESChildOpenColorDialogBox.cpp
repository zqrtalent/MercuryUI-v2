// ESChildOpenColorDialogBox.cpp : implementation file
//

#include "ESChildOpenColorDialogBox.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildOpenColorDialogBox

ESChildOpenColorDialogBox::ESChildOpenColorDialogBox() : ESChildTextBox(){
	m_sClassName	= _T("ESChildOpenColorDialogBox");
	m_crChosen		= 0;
	}

ESChildOpenColorDialogBox::~ESChildOpenColorDialogBox(){
	}

bool
ESChildOpenColorDialogBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildTextBox::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		m_bReadOnly	= true;
		}
	return bRet;
	}

void
ESChildOpenColorDialogBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildTextBox::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildOpenColorDialogBox::SetPos(int x, int y, bool bRedraw){
	ESChildTextBox::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildOpenColorDialogBox::SetSize(int cx, int cy, bool bRedraw){
	bool bRet = ESChildTextBox::SetSize(cx, cy, bRedraw);
	return bRet;
	}

bool
ESChildOpenColorDialogBox::Destroy(){
	if( ESChildTextBox::Destroy() ){
		}
	return true;
	}

bool
ESChildOpenColorDialogBox::SetFocus(){
	return true;
	}

void
ESChildOpenColorDialogBox::KillFocus(){
	}

void
ESChildOpenColorDialogBox::SetColor(COLORREF cr, bool bRedraw){
	m_crChosen	= cr;

	_string sText;
	BYTE red	= (BYTE)((m_crChosen)&0xFF);
	BYTE green	= (BYTE)(((m_crChosen)&0xFF00) >> 8);
	BYTE blue	= (BYTE)(((m_crChosen)&0xFF0000) >> 16);
	stringEx::Format(sText, _T("#%02X%02X%02X"), red, green, blue);

	m_crFill	= m_crChosen;
	m_crText	= RGB(255-red, 255-green, 255-blue);

	SetText(sText, false, bRedraw);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildOpenColorDialogBox::OnLButtonDown(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonDown(pt, nFlags);
	/*
	CColorDialog crDialog; 
	if( crDialog.DoModal() == IDOK ){
		m_crChosen = crDialog.m_cc.rgbResult;
		SetColor(m_crChosen, true);
		}*/
	}

void
ESChildOpenColorDialogBox::OnLButtonUp(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonUp(pt, nFlags);
	}

void	
ESChildOpenColorDialogBox::OnLButtonClick(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonClick(pt, nFlags);
	}

void
ESChildOpenColorDialogBox::OnMouseWheel(UINT nFlags, short zDelta, _Point pt){
	ESChildTextBox::OnMouseWheel(nFlags, zDelta, pt);
	}
