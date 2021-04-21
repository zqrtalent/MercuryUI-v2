// ESMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "..\..\..\Controls\ESChildControlImageButton.h"
#include "ESMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESMessageBox

ESMessageBox::ESMessageBox(){
	m_bAllowMouseEvents	= true;
	m_bCentred			= true;
	m_bLogicalPos		= false;
	m_sMessageText		= _T("");
	m_nIDMessage		= MB_YESNOCANCEL;
	m_hIcon				= ::LoadIcon(NULL, MAKEINTRESOURCE(IDI_EXCLAMATION));
	}

ESMessageBox::~ESMessageBox(){
	}

bool
ESMessageBox::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESDialog::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){/*
		m_szIcon.cx				= GetSystemMetrics(SM_CXICON);
		m_szIcon.cy				= GetSystemMetrics(SM_CYICON);

		int							nId		= MercuryBaseView::GetChildControlIdUnused(m_pOwner);
		PokerGUI*					pGUI	= PokerGUI::GetInstance();
		ESChildControlImageButton*	pOK		= new ESChildControlImageButton();
		
		pOK->SetDefaultImage		(pGUI->m_pPushButtonNormal);
		pOK->SetHoverImage			(pGUI->m_pPushButtonHover);
		pOK->SetSelectedImage		(pGUI->m_pPushButtonPushed);
		pOK->SetDisabledImage		(pGUI->m_pPushButtonDisabled);
		pOK->SetSizeAutoCalculation	(false);
		pOK->SetOmitBorder			(pGUI->m_rcPushButtonOmitBorder);
		pOK->SetFont				(pGUI->m_pPushButtonFont);
		pOK->SetTextColor			(pGUI->m_crPushButtonText);
		pOK->SetText				(_T("OK"));
		
		CSize	szButton(90, 26);
		CRect	rcButton;
		rcButton.left	= m_rcClientArea.left;
		rcButton.top	= m_rcClientArea.bottom - szButton.cy - 15;
		rcButton.right	= rcButton.left + szButton.cx;
		rcButton.bottom = rcButton.top + szButton.cy;

		if( !m_pOwner->AddChildControl(pOK, nId, rcButton, this) )
			delete pOK;*/
		}
	return bRet;
	}

bool
ESMessageBox::SetPos(int x, int y, bool bRedraw){
	ESDialog::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESMessageBox::SetSize(int cx, int cy, bool bRedraw){
	ESDialog::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESMessageBox::Destroy(){
	ESDialog::Destroy();
	return true;
	}

void
ESMessageBox::OwnerWindowSizeChanged(int cx, int cy){
	ESDialog::OwnerWindowSizeChanged(cx, cy);
/*
	CRect rcRectNew;
	rcRectNew.left		= (cx - m_rcRect.Width()) / 2;
	rcRectNew.top		= (cy - m_rcRect.Height()) / 2;
	rcRectNew.right		= rcRectNew.left + m_rcRect.Width();
	rcRectNew.bottom	= rcRectNew.top + m_rcRect.Height();
	m_rcRect			= rcRectNew;*/
	}

void
ESMessageBox::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	ESDialog::OnPaintClient(pDC, pRectDC, pRectClient);
/*
	PokerGUI*	pGUI= PokerGUI::GetInstance();
	CRect		rcIcon;
	rcIcon.left		= m_rcClientArea.left + 6;
	rcIcon.top		= m_rcClientArea.top + 10;
	rcIcon.right	= rcIcon.left + m_szIcon.cx;
	rcIcon.bottom	= rcIcon.top + m_szIcon.cy;

	CRect	rcIconDC, rcInvalid;
	rcIconDC.left	= (rcIcon.left - pRectClient->left) + pRectDC->left; 
	rcIconDC.top	= (rcIcon.top - pRectClient->top) + pRectDC->top; 
	rcIconDC.right	= rcIconDC.left + rcIcon.Width(); 
	rcIconDC.bottom	= rcIconDC.top + rcIcon.Height(); 

	// Draw icon. {{
	if( m_hIcon != NULL )
		pDC->DrawIcon(rcIconDC.left, rcIconDC.top, m_hIcon);
	// }}
	
	// Draw message text. {{
	if( m_sMessageText.IsEmpty() ) 
		return;

	// Calculate lines. {{
	CStringArray	arrLines;
	CString			sLine;
	CString			sLabel = m_sMessageText;
	int nFind = sLabel.Find(_T("\r\n"), 0);
	if( nFind == -1 && sLabel.GetLength() > 0 )
		nFind = sLabel.GetLength() - 1;

	do{
		sLine = sLabel.Left(nFind + 1);
		sLabel.Delete(0, nFind + 2);

		arrLines.Add(sLine);

		nFind		=	sLabel.Find(_T("\r\n"), 0);
		if( nFind == -1 && sLabel.GetLength() > 0 )
			nFind = sLabel.GetLength() - 1;
		}
	while( nFind != -1 );
	// }}

	CRect	rcMessageText;
	rcMessageText.left		= rcIcon.right + 10;
	rcMessageText.top		= rcIcon.top;
	rcMessageText.right		= m_rcClientArea.right;
	rcMessageText.bottom	= m_rcClientArea.bottom;

	CFont*	pFont			= m_pFont;
	CFont*	pFontOld		= pDC->SelectObject(pFont);
	CSize	szText;
	int		nOffsetY		= rcMessageText.top + 4;

	for( int nLoop=0; nLoop<arrLines.GetCount(); nLoop++ ){
		sLine = arrLines.GetAt(nLoop);
		ImageHelper::GetTextSize(&sLine, pFont, szText);

		CRect	rcLabel;
		rcLabel.left	= rcMessageText.left + 2;
		//rcLabel.left	= rcMessageText.left + (rcMessageText.Width() - szText.cx)/2;
		rcLabel.top		= nOffsetY;
		rcLabel.right	= rcLabel.left + szText.cx;
		rcLabel.bottom	= rcLabel.top + szText.cy;

		// Translate from window pixel to DC pixel.{{
		CPoint ptLabel;
		ptLabel.x = (rcLabel.left - pRectClient->left) + pRectDC->left; 
		ptLabel.y = (rcLabel.top - pRectClient->top - 3) + pRectDC->top; 
		//}}

		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(m_crText);
		PokerGUI::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, sLine);
		nOffsetY	+=	szText.cy + 2;
		}

	pDC->SelectObject	(pFontOld);
	// }}
	*/
	}

void	
ESMessageBox::OnMouseHover(CPoint pt, UINT nFlags){
	ESDialog::OnMouseHover(pt, nFlags);
	}

BOOL	
ESMessageBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return ESDialog::OnSetCursor(pWnd, nHitTest, message);
	}

void	
ESMessageBox::OnMouseLeave(UINT nFlags){
	ESDialog::OnMouseLeave(nFlags);
	}

void	
ESMessageBox::OnMouseEnter(UINT nFlags){
	ESDialog::OnMouseEnter(nFlags);
	}

void
ESMessageBox::OnLButtonDown(CPoint pt, UINT nFlags){
	ESDialog::OnLButtonDown(pt, nFlags);
	}

void
ESMessageBox::OnLButtonUp(CPoint pt, UINT nFlags){
	ESDialog::OnLButtonUp(pt, nFlags);
	}

void	
ESMessageBox::OnLButtonClick(CPoint pt, UINT nFlags){
	ESDialog::OnLButtonClick(pt, nFlags);
	}

void	
ESMessageBox::OnRButtonClick(CPoint pt, UINT nFlags){
	ESDialog::OnRButtonClick(pt, nFlags);
	}

void	
ESMessageBox::OnMButtonClick(CPoint pt, UINT nFlags){
	ESDialog::OnMButtonClick(pt, nFlags);
	}

void	
ESMessageBox::OnLButtonDblClick(CPoint pt, UINT nFlags){
	ESDialog::OnLButtonDblClick(pt, nFlags);
	}

void	
ESMessageBox::OnRButtonDblClick(CPoint pt, UINT nFlags){
	ESDialog::OnRButtonDblClick(pt, nFlags);
	}

void	
ESMessageBox::OnMButtonDblClick(CPoint pt, UINT nFlags){
	ESDialog::OnMButtonDblClick(pt, nFlags);
	}