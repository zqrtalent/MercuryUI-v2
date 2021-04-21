// ESChildControlTextEdit.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlTextEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTextEdit

ESChildControlTextEdit::ESChildControlTextEdit(){
	m_pEditFont = NULL;
	m_bPassword	= false;
	}

ESChildControlTextEdit::~ESChildControlTextEdit(){
	}

bool
ESChildControlTextEdit::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		PokerGUI*	pGUI	= PokerGUI::GetInstance();
		DWORD		dwStyle	= WS_VISIBLE|WS_CHILD|ES_LEFT|WS_BORDER;
		if( m_bPassword ){
			dwStyle |= ES_PASSWORD;
			}
		m_editControl.Create(dwStyle, rcRect, pOwner, (UINT)nId);
		if( m_pEditFont )
			m_editControl.SetFont(m_pEditFont);
		m_editControl.SetInvalidCharacters(_T(""));
		SetProp(m_editControl, _T("OWNER"), (HANDLE)this);
		}
	return bRet;
	}

void
ESChildControlTextEdit::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	if( IsWindow(m_editControl) ){
		}
	}

bool
ESChildControlTextEdit::SetFocus(){
	if( IsWindow(m_editControl) ){
		m_editControl.SetFocus();
		return true; // Accept focus.
		}
	return false; // Refuse focus accepting.
	}

bool
ESChildControlTextEdit::VisibilityChanged(bool bVisible){	
	if( IsWindow(m_editControl) ){
		m_editControl.ShowWindow(bVisible ? SW_SHOW : SW_HIDE);
		}
	return true;
	}

bool
ESChildControlTextEdit::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlTextEdit::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	if( IsWindow(m_editControl) ){
		m_editControl.SetWindowPos(NULL, m_rcRect.left, m_rcRect.top, m_rcRect.Width(), m_rcRect.Height(), SWP_NOZORDER);
		}
	return true;
	}

bool
ESChildControlTextEdit::Destroy(){
	ESChildControl::Destroy();
	if( IsWindow(m_editControl) ){
		DestroyWindow(m_editControl);
		}
	return true;
	}

LRESULT	__stdcall 
ESChildControlTextEdit::OnEditWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam){
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
ESChildControlTextEdit::SetFont(CFont* pFont){
	m_pEditFont = pFont;
	if( IsWindow(m_editControl) && pFont ){
		m_editControl.SetFont(pFont);
		}
	}

void
ESChildControlTextEdit::SetText(CString sText){
	if( IsWindow(m_editControl) ){
		m_editControl.SetWindowText(sText);
		}
	}

CString	
ESChildControlTextEdit::GetText(){
	CString sText;
	if( IsWindow(m_editControl) ){
		m_editControl.GetWindowText(sText);
		}
	return sText;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlTextEdit::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	m_editControl.Invalidate(FALSE);
	}

void	
ESChildControlTextEdit::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlTextEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlTextEdit::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlTextEdit::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlTextEdit::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlTextEdit::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTextEdit::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTextEdit::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTextEdit::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTextEdit::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTextEdit::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTextEdit::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}