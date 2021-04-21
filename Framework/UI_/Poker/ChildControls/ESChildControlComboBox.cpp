// ESChildControlComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlComboBox

ESChildControlComboBox::ESChildControlComboBox(){
	m_pComboFont		= NULL;
	m_bDropListStyle	= false;
	}

ESChildControlComboBox::~ESChildControlComboBox(){
	}

bool
ESChildControlComboBox::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		PokerGUI*	pGUI	= PokerGUI::GetInstance();
		DWORD		dwStyle	= WS_VISIBLE|WS_CHILD|CBS_DROPDOWN|CBS_HASSTRINGS|CBS_NOINTEGRALHEIGHT|WS_BORDER;
		if( m_bDropListStyle ){
			dwStyle |= CBS_DROPDOWNLIST;
			}
		m_comboControl.Create(dwStyle, rcRect, pOwner, (UINT)nId);
		if( m_pComboFont )
			m_comboControl.SetFont(m_pComboFont);
		SetProp(m_comboControl, _T("OWNER"), (HANDLE)this);
		}
	return bRet;
	}

void
ESChildControlComboBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	if( IsWindow(m_comboControl) ){
		}
	}

bool
ESChildControlComboBox::VisibilityChanged(bool bVisible){	
	if( IsWindow(m_comboControl) ){
		m_comboControl.ShowWindow(bVisible ? SW_SHOW : SW_HIDE);
		}
	return true;
	}

bool
ESChildControlComboBox::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlComboBox::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	if( IsWindow(m_comboControl) ){
		m_comboControl.SetWindowPos(NULL, m_rcRect.left, m_rcRect.top, m_rcRect.Width(), m_rcRect.Height(), SWP_NOZORDER);
		}
	return true;
	}

bool
ESChildControlComboBox::Destroy(){
	ESChildControl::Destroy();
	if( IsWindow(m_comboControl) ){
		DestroyWindow(m_comboControl);
		}
	return true;
	}

LRESULT	__stdcall 
ESChildControlComboBox::OnEditWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam){
	LRESULT lRet = 0L;

	switch( nMessage ){
		case WM_ENTERIDLE:
			{
				break;
			}
		case WM_PASTE:
			{
				return 0;
			}
		case WM_CHAR:
			{
				break;
			}
		};

	lRet = (*GetOldProcAddress())(hWnd, nMessage, wParam, lParam);
	return lRet;
	}

void
ESChildControlComboBox::SetFont(CFont* pFont){
	m_pComboFont = pFont;
	if( IsWindow(m_comboControl) && pFont ){
		m_comboControl.SetFont(pFont);
		}
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlComboBox::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	m_comboControl.Invalidate(FALSE);
	}

void	
ESChildControlComboBox::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlComboBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlComboBox::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlComboBox::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlComboBox::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlComboBox::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlComboBox::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlComboBox::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlComboBox::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlComboBox::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlComboBox::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlComboBox::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}