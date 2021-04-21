// ESChildLookupBox.cpp : implementation file
//

#include "ESChildLookupBox.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildLookupBox

ESChildLookupBox::ESChildLookupBox() : ESChildTextBox(){
	m_sClassName	= _T("ESChildLookupBox");
	m_bButtonPushed	= FALSE;
	}

ESChildLookupBox::~ESChildLookupBox(){
	}

bool
ESChildLookupBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildTextBox::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		CalcButtonRect();
		m_bReadOnly	= true;
		}
	return bRet;
	}

void
ESChildLookupBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildTextBox::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildLookupBox::SetPos(int x, int y, bool bRedraw){
	ESChildTextBox::SetPos(x, y, bRedraw);
	CalcButtonRect();
	return true;
	}

bool
ESChildLookupBox::SetSize(int cx, int cy, bool bRedraw){
	bool bRet = ESChildTextBox::SetSize(cx, cy, bRedraw);
	CalcButtonRect();
	return bRet;
	}

bool
ESChildLookupBox::Destroy(){
	if( ESChildTextBox::Destroy() ){
		}
	return true;
	}

bool
ESChildLookupBox::SetFocus(){
	return true;
	}

void
ESChildLookupBox::KillFocus(){
	}

void
ESChildLookupBox::CalcButtonRect(){
	MercuryGUI* pGUI		= MercuryGUI::GetInstance();
	int			nCX			= pGUI->lookupBox.m_imgLookupButton.GetWidth();
	int			nCY			= pGUI->lookupBox.m_imgLookupButton.GetHeight();

	m_rcBoxButton.left		= m_rRect.right - nCX - 2;
	m_rcBoxButton.top		= m_rRect.top + (m_rRect.Height() - nCY)/2;
	m_rcBoxButton.right		= m_rcBoxButton.left + nCX;
	m_rcBoxButton.bottom	= m_rcBoxButton.top + nCY;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildLookupBox::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	MercuryGUI* pMerGUI		= MercuryGUI::GetInstance();
	
	_Rect		rcBoxDC, rcInvalid;
	rcBoxDC.left		= (m_rcBoxButton.left - pRectClient->left) + pRectDC->left; 
	rcBoxDC.top			= (m_rcBoxButton.top - pRectClient->top) + pRectDC->top; 
	rcBoxDC.right		= rcBoxDC.left + m_rcBoxButton.Width(); 
	rcBoxDC.bottom		= rcBoxDC.top + m_rcBoxButton.Height();

	Image* pImageBox	= m_bButtonPushed ? &pMerGUI->lookupBox.m_imgLookupButton : &pMerGUI->lookupBox.m_imgLookupButtonPushed;

	// First draw entire textbox.
	ESChildTextBox::OnPaintClient(pDC, pRectDC, pRectClient);

	// Draw box button. {{
	rcInvalid		= rcBoxDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImageBox->RenderImage(pDC, rcInvalid, rcBoxDC, false);
	// }}
	}

BOOL	
ESChildLookupBox::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){/*
	_Point pt;
	::GetCursorPos(&pt);
	ScreenToClient(*m_pOwner, &pt);
	if( m_rcBoxButton.PtInRect(pt) ){
		::SetCursor(LoadCursor(NULL, IDC_HAND));
		return 1;
		}*/

	return ESChildTextBox::OnSetCursor(hWnd, nHitTest, message);
	}

void
ESChildLookupBox::OnLButtonDown(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonDown(pt, nFlags);

	if( m_rcBoxButton.PtInRect(pt) ){
		if( !m_bButtonPushed ){
			m_bButtonPushed = TRUE;
			RedrawRect(m_rcBoxButton, false);
			}
		}
	}

void
ESChildLookupBox::OnLButtonUp(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonUp(pt, nFlags);
	if( m_bButtonPushed ){
		m_bButtonPushed = FALSE;
		RedrawRect(m_rcBoxButton, false);
		}
	}

void	
ESChildLookupBox::OnLButtonClick(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonClick(pt, nFlags);

	// Send event.
	if( m_rcBoxButton.PtInRect(pt) ){
		SendEvent(this, EventCode_Lookup);
		}
	}

_Rect
ESChildLookupBox::GetTextAreaRect(){
	_Rect rcTextArea = ESChildTextBox::GetTextAreaRect();
	rcTextArea.right -= m_rcBoxButton.Width();
	return rcTextArea;
	}