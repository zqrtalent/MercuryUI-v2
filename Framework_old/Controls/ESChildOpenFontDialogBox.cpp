// ESChildOpenFontDialogBox.cpp : implementation file
//

#include "ESChildOpenFontDialogBox.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildOpenFontDialogBox

ESChildOpenFontDialogBox::ESChildOpenFontDialogBox() : ESChildTextBox(){
	m_sClassName = _T("ESChildOpenFontDialogBox");
	memset(&m_lfChosen, 0, sizeof(LOGFONT));
	}

ESChildOpenFontDialogBox::~ESChildOpenFontDialogBox(){
	}

bool
ESChildOpenFontDialogBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildTextBox::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		m_bReadOnly	= true;
		}
	return bRet;
	}

void
ESChildOpenFontDialogBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildTextBox::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildOpenFontDialogBox::SetPos(int x, int y, bool bRedraw){
	ESChildTextBox::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildOpenFontDialogBox::SetSize(int cx, int cy, bool bRedraw){
	bool bRet = ESChildTextBox::SetSize(cx, cy, bRedraw);
	return bRet;
	}

bool
ESChildOpenFontDialogBox::Destroy(){
	if( ESChildTextBox::Destroy() ){
		}
	return true;
	}

bool
ESChildOpenFontDialogBox::SetFocus(){
	return true;
	}

void
ESChildOpenFontDialogBox::KillFocus(){
	}

void
ESChildOpenFontDialogBox::SetFont(Font* pFont, bool bRedraw){
	SetTextFont(pFont, false);

	if( !m_textFont.IsNull() ){
		LOGFONT lf;
		m_textFont.GetLogFont(&lf);
		SetText(lf.lfFaceName, false, false);
		}

	if( bRedraw )
		Redraw(false);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildOpenFontDialogBox::OnLButtonDown(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonDown(pt, nFlags);

	LOGFONT* pLogFontInitial = &m_lfChosen;
	if( m_lfChosen.lfFaceName[0] == '\0' ){
		if( !m_textFont.IsNull() )
			m_textFont.GetLogFont(&m_lfChosen);
		else
			pLogFontInitial = NULL;
		}
	/*
	CFontDialog fontDialog(pLogFontInitial); 
	if( fontDialog.DoModal() == IDOK ){
		if( fontDialog.m_cf.lpLogFont && fontDialog.m_cf.lpLogFont->lfFaceName[0] != '\0' ){
			memcpy(&m_lfChosen, fontDialog.m_cf.lpLogFont, sizeof(LOGFONT));

			CFont* pFontNew = new CFont();
			pFontNew->CreateFontIndirect(&m_lfChosen);

			SetTextFont	(pFontNew, true);
			SetText		(_string(m_lfChosen.lfFaceName), false, true);
			delete pFontNew;
			}
		}*/
	}

void
ESChildOpenFontDialogBox::OnLButtonUp(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonUp(pt, nFlags);
	}

void	
ESChildOpenFontDialogBox::OnLButtonClick(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonClick(pt, nFlags);
	}

void
ESChildOpenFontDialogBox::OnMouseWheel(UINT nFlags, short zDelta, _Point pt){
	ESChildTextBox::OnMouseWheel(nFlags, zDelta, pt);
	}
