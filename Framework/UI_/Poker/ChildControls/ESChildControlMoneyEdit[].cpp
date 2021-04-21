// ESChildControlMoneyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlMoneyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlMoneyEdit

ESChildControlMoneyEdit::ESChildControlMoneyEdit(){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	m_nAmount			= 1099;
	m_nAmountMin		= 0;
	m_nAmountMax		= 2000;
	m_pFont				= pGUI->m_pSliderFont;
	}

ESChildControlMoneyEdit::~ESChildControlMoneyEdit(){
	}

bool
ESChildControlMoneyEdit::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		PokerGUI*	pGUI	= PokerGUI::GetInstance();
		m_editControl.Create(WS_VISIBLE|WS_CHILD|ES_LEFT|WS_BORDER, rcRect, pOwner, (UINT)nId);
		if( m_pFont )
			m_editControl.SetFont(m_pFont);
		SetProp(m_editControl, _T("OWNER"), (HANDLE)this);

		m_editControl.SetMaxWholeDigits		(9);
		m_editControl.SetMaxDecimalPlaces	(2);
		m_editControl.AllowNegative			(false);
		m_editControl.SetSeparators			('.', ',');

		double dMin = (double)(m_nAmountMin/100.0);
		double dMax = (double)(m_nAmountMax/100.0);
		if( dMin > dMax )
			dMin = dMax;
		m_editControl.SetRange(dMin, dMax);
		
		WNDPROC* pOldProc	= GetOldProcAddress();
		*pOldProc			= (WNDPROC)::GetWindowLong(m_editControl, GWL_WNDPROC);
		::SetWindowLong(m_editControl, GWL_WNDPROC, (LONG)OnEditWndProc);
		}
	return bRet;
	}

void
ESChildControlMoneyEdit::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);

	if( IsWindow(m_editControl) ){
		PokerGUI*	pGUI	= PokerGUI::GetInstance();
		CRect rcOwner;
		m_pOwner->GetClientRect(rcOwner);
		m_fontEdit.DeleteObject	();
		float fStretchCX = ((float)rcOwner.Width()) / pGUI->m_szDefWindowSize.cx;
		float fStretchCY = ((float)rcOwner.Height()) / pGUI->m_szDefWindowSize.cy;
		PokerGUI::StretchedTextFont(m_pFont, &m_fontEdit, fStretchCX, fStretchCY);	
		m_editControl.SetFont	(&m_fontEdit);

		m_editControl.SetWindowPos(NULL, m_rcRect.left, m_rcRect.top, m_rcRect.Width(), m_rcRect.Height(), SWP_NOZORDER);
		}
	}

bool
ESChildControlMoneyEdit::SetFocus(){
	if( IsWindow(m_editControl) ){
		m_editControl.SetFocus();
		return true;
		}
	return false; // Refuse focus accepting.
	}

void
ESChildControlMoneyEdit::SetFont(CFont* pFont){
	m_pFont = pFont;
	}

bool
ESChildControlMoneyEdit::VisibilityChanged(bool bVisible){	
	if( IsWindow(m_editControl) ){
		m_editControl.ShowWindow(bVisible ? SW_SHOW : SW_HIDE);
		}
	return true;
	}

bool
ESChildControlMoneyEdit::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlMoneyEdit::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	if( IsWindow(m_editControl) ){
		m_editControl.SetWindowPos(NULL, m_rcRect.left, m_rcRect.top, m_rcRect.Width(), m_rcRect.Height(), SWP_NOZORDER);
		}
	return true;
	}

bool
ESChildControlMoneyEdit::Destroy(){
	ESChildControl::Destroy();
	if( IsWindow(m_editControl) ){
		DestroyWindow(m_editControl);
		}
	m_fontEdit.DeleteObject();
	return true;
	}

void
ESChildControlMoneyEdit::Setup(int nAmount, int nMin, int nMax){
	m_nAmount		= nAmount;
	m_nAmountMin	= nMin;
	m_nAmountMax	= nMax;

	if( IsWindow(m_editControl) ){
		double dMin = (double)(m_nAmountMin/100.0);
		double dMax = (double)(m_nAmountMax/100.0);
		if( dMin > dMax )
			dMin = dMax;
		m_editControl.SetRange(dMin, dMax);

		CString sFormat;
		sFormat.Format(_T("%d.%02d"), nAmount/100, nAmount%100);
		m_editControl.SetWindowText(sFormat);
		m_editControl.SetSel(0, -1);
		}
	}

void
ESChildControlMoneyEdit::SetAmount(int nAmount, bool bSetEditText /*= true*/){
	m_nAmount		= nAmount;
	if( bSetEditText && IsWindow(m_editControl) ){
		CString sFormat;
		sFormat.Format(_T("%d.%02d"), nAmount/100, nAmount%100);
		m_editControl.SetWindowText	(sFormat);
		}
	}

int
ESChildControlMoneyEdit::GetAmount(){
	int nRetValue = 0;
	if( IsWindow(m_editControl) ){
		double dValue = m_editControl.GetDouble();
		nRetValue = Serializable::ConvertMoney(dValue);
		}
	return nRetValue;
	}

LRESULT	__stdcall 
ESChildControlMoneyEdit::OnEditWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam){
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
		case WM_SETFOCUS:
			{
			CEdit*	pEditWnd	= (CEdit*)CWnd::FromHandle(hWnd);
			ASSERT(pEditWnd);
			// Call old window proc.
			lRet = (*GetOldProcAddress())(hWnd, nMessage, wParam, lParam);
			pEditWnd->SetSel(0, -1);
			}
		case WM_CHAR:{
			char						cSymbol		= (char)wParam;
			ESChildControlMoneyEdit*	pEdit		= (ESChildControlMoneyEdit*)::GetProp(hWnd, _T("OWNER"));
			CEdit*						pEditWnd	= (CEdit*)CWnd::FromHandle(hWnd);
			ASSERT(pEditWnd);

			CString						sTextOld;
			pEditWnd->GetWindowText(sTextOld);

			if( !((cSymbol >= '0' && cSymbol <= '9') || cSymbol == '.' || cSymbol == VK_BACK ) )
				return 0; // Only allowed symbols.

			// Call old window proc.
			lRet = (*GetOldProcAddress())(hWnd, nMessage, wParam, lParam);

			CString sText, sAfterDot, sBeforeDot;
			pEditWnd->GetWindowText(sText);

			int nFindDot	= sText.Find('.');
			if( nFindDot != -1 ){
				sAfterDot	= sText.Right(sText.GetLength() - nFindDot - 1);
				sAfterDot	= sAfterDot.Left(2);
				sBeforeDot	= sText.Left(nFindDot);
				}
			else
				sBeforeDot = sText;

			// Convert edit text into amount value. {{
			int nAmount = _ttoi(sBeforeDot.GetBuffer())*100;
			if( sAfterDot.GetLength() == 1 )
				nAmount += _ttoi(sAfterDot.GetBuffer())*10;
			else
				nAmount += _ttoi(sAfterDot.GetBuffer());
			// }}

			if( pEdit->m_pOwner ){
				if( !pEdit->SendEvent(pEdit, EventCode_EditTextChange) )
					pEdit->m_pOwner->SendMessage(WM_SLIDER_MONEY_CHANGED, (WPARAM)nAmount, pEdit->GetId());
/*
				int nStartChar = 0, nEndChar = 0;
				pEditWnd->GetSel(nStartChar, nEndChar);
				if( pEdit->m_pOwner->SendMessage(WM_SLIDER_MONEY_CHANGED, (WPARAM)nAmount, pEdit->GetId()) ){
					if( nStartChar == sText.GetLength() &&  sAfterDot.GetLength() == 2 )
						pEditWnd->SetSel(0, 0); // Goto start
					else
						pEditWnd->SetSel(nStartChar, nStartChar);
					}
				else{
					EDITBALLOONTIP tip;
					tip.cbStruct	= sizeof(tip);
					tip.pszText		= L"Type valid Amount";
					tip.pszTitle	= L"Error";
					Edit_ShowBalloonTip(pEditWnd->m_hWnd, &tip);

					// Select all.
					pEditWnd->SetWindowText	(sTextOld);
					pEditWnd->SetSel		(0, -1);
					}*/
				}
			return lRet;
			break;
			}
		};

	lRet = (*GetOldProcAddress())(hWnd, nMessage, wParam, lParam);
	return lRet;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlMoneyEdit::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	m_editControl.Invalidate(FALSE);
	}

void	
ESChildControlMoneyEdit::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlMoneyEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlMoneyEdit::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlMoneyEdit::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlMoneyEdit::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlMoneyEdit::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlMoneyEdit::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlMoneyEdit::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlMoneyEdit::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlMoneyEdit::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlMoneyEdit::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlMoneyEdit::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}