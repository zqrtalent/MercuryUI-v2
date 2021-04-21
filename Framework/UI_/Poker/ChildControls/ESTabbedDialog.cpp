//// ESTabbedDialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "..\..\..\Controls\ESChildControlImageButton.h"
#include "ESTabbedDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESTabbedDialog

ESTabbedDialog::ESTabbedDialog(){
	}

ESTabbedDialog::~ESTabbedDialog(){
	}

bool
ESTabbedDialog::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESDialog::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		}
	return bRet;
	}

bool
ESTabbedDialog::SetPos(int x, int y, bool bRedraw){
	ESDialog::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESTabbedDialog::SetSize(int cx, int cy, bool bRedraw){
	ESDialog::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESTabbedDialog::Destroy(){
	ESDialog::Destroy();
	return true;
	}

void
ESTabbedDialog::OwnerWindowSizeChanged(int cx, int cy){
	ESDialog::OwnerWindowSizeChanged(cx, cy);
	}

BOOL
ESTabbedDialog::OnOK(){
	m_nRetCode = IDOK;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, true);
	return FALSE;
	}

BOOL
ESTabbedDialog::OnCancel(){
	m_nRetCode = IDCANCEL;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, true);
	return FALSE;
	}

BOOL
ESTabbedDialog::OnTabControl(){
	ESChildControl* pChild = m_pFocusedChild;
	if( !pChild )
		pChild = m_pChild;

	while( pChild ){
		pChild = pChild->GetNext();
		}

	return FALSE;
	}

void
ESTabbedDialog::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	ESDialog::OnPaintClient(pDC, pRectDC, pRectClient);
	/*
	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CImage*		pImageBk		= pGUI->m_pDialogBk;
	CRect		rcOmitBorder	= pGUI->m_rcDialogBkOmitBorder;
	ASSERT(pImageBk);
	if( !pImageBk ) return;

	CRect rcOwner;
	m_pOwner->GetClientRect(rcOwner);
	float fStretchCX = ((float)rcOwner.Width()) / pGUI->m_szDefWindowSize.cx;
	float fStretchCY = ((float)rcOwner.Height()) / pGUI->m_szDefWindowSize.cy;

	CFont* pFontOld		= NULL;
	CFont	font;
	CFont*	pCaptionFont= pGUI->m_pDialogFont;
	if( m_bSizeTextFont ){
		PokerGUI::StretchedTextFont(pCaptionFont, &font, fStretchCX, fStretchCY);
		pFontOld		= pDC->SelectObject(&font);
		pCaptionFont	= &font;
		}
	else
		pFontOld		= pDC->SelectObject(pCaptionFont);

	// Draw Dialog.
	CRect	rcBkDC, rcInvalid;
	rcBkDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcBkDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcBkDC.right	= rcBkDC.left + m_rcRect.Width(); 
	rcBkDC.bottom	= rcBkDC.top + m_rcRect.Height(); 

	CRect rcFillDC;
	rcFillDC.left	= rcBkDC.left + rcOmitBorder.left;
	rcFillDC.right	= rcBkDC.right - rcOmitBorder.right;
	rcFillDC.top	= rcBkDC.top + rcOmitBorder.top;
	rcFillDC.bottom	= rcBkDC.bottom - rcOmitBorder.bottom;

	// Draw background. {{
	rcInvalid	= rcBkDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcBkDC, pImageBk, rcOmitBorder, TRUE, TRUE);
	// }}
	// Fill color. {{
	rcInvalid	= rcFillDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pDC->FillSolidRect(rcInvalid, m_crFill);
	// }}

	// Draw title. {{
	CSize	szText;
	int		nOffsetY	= m_rcRect.top + 4;
	CRect	rcTitle;

	if( !m_sTitle.IsEmpty() ){
		ImageHelper::GetTextSize(&m_sTitle, pCaptionFont, szText);
		rcTitle.left	= m_rcRect.left + (m_rcRect.Width() - szText.cx)/2;
		rcTitle.top		= nOffsetY;
		rcTitle.right	= rcTitle.left + szText.cx;
		rcTitle.bottom	= rcTitle.top + szText.cy;

		// Translate from window pixel to DC pixel.{{
		CPoint ptTitle;
		ptTitle.x = (rcTitle.left - pRectClient->left) + pRectDC->left; 
		ptTitle.y = (rcTitle.top - pRectClient->top - 3) + pRectDC->top; 
		//}}

		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(m_crText);
		PokerGUI::ExtTextOut(pDC, ptTitle.x, ptTitle.y, ETO_CLIPPED, pRectDC, m_sTitle);
		// }}
		}
	// #########################################
	pDC->SelectObject(pFontOld);
	*/
	}

void	
ESTabbedDialog::OnMouseLeave(UINT nFlags){
	}

void	
ESTabbedDialog::OnMouseEnter(UINT nFlags){
	}

void	
ESTabbedDialog::OnMouseHover(CPoint pt, UINT nFlags){
	}

void
ESTabbedDialog::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESTabbedDialog::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESTabbedDialog::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESTabbedDialog::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESTabbedDialog::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESTabbedDialog::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESTabbedDialog::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void
ESTabbedDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	}

void	
ESTabbedDialog::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}