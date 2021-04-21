// ESChildOpenFileBox.cpp : implementation file
//
#include "ESChildOpenFileBox.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildOpenFileBox

ESChildOpenFileBox::ESChildOpenFileBox(_string sFilter) : ESChildTextBox(){
	m_sClassName			= _T("ESChildOpenFileBox");
	m_sFilter				= sFilter;
	m_bCloseBox				= 1;
	}

ESChildOpenFileBox::~ESChildOpenFileBox(){
	}

bool
ESChildOpenFileBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildTextBox::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		m_bReadOnly	= true;
		}
	return bRet;
	}

void
ESChildOpenFileBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildTextBox::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildOpenFileBox::SetPos(int x, int y, bool bRedraw){
	ESChildTextBox::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildOpenFileBox::SetSize(int cx, int cy, bool bRedraw){
	bool bRet = ESChildTextBox::SetSize(cx, cy, bRedraw);
	return bRet;
	}

bool
ESChildOpenFileBox::Destroy(){
	if( ESChildTextBox::Destroy() ){
		}
	return true;
	}

bool
ESChildOpenFileBox::SetFocus(){
	return true;
	}

void
ESChildOpenFileBox::KillFocus(){
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildOpenFileBox::OnLButtonClick(_Point pt, UINT nFlags){
	if( m_bCloseBox && m_rcCloseBox.PtInRect(pt) ){
		}
	else{
		//	CFileDialog openFile(TRUE, 0, 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||", m_pOwner); 
		/*
		CFileDialog openFile(TRUE, 0, 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, m_sFilter, CWnd::FromHandle(m_pOwner->GetHWND())); 
		if( openFile.DoModal() == IDOK ){
			m_sOpenedFilePath = openFile.GetPathName();
			SetText(openFile.GetPathName(), false, true);
			}*/
		}

	ESChildTextBox::OnLButtonClick(pt, nFlags);
	}
