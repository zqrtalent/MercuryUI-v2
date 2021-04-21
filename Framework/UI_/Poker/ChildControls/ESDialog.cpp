//// ESDialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "..\..\..\Controls\ESChildControlImageButton.h"
#include "..\..\..\Controls\ESChildControlLabel.h"
#include "ESChildControlComboBox.h"
#include "ESChildControlTextEdit.h"
#include "ESChildControlMoneyEdit.h"
#include "ESDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESDialog

ESDialog::ESDialog(){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	m_bLogicalPos		= false;
	m_pFont				= NULL;
	m_crText			= pGUI->m_crDialogText;
	m_crFill			= pGUI->m_crDialogFill;
	m_rcOmitBorder		= pGUI->m_rcDialogBkOmitBorder;
	m_sTitle			= _T("Dialog");
	m_bCentred			= true;
	m_bSizeTextFont		= false;
	m_pCloseButton		= NULL;
	m_rcClientArea		= CRect(0, 0, 0, 0);
	m_pFocusedChild		= NULL;
	m_nRetCode			= IDCANCEL;
	m_bMoving			= false;
	m_bTitleBar			= true;
	}

ESDialog::~ESDialog(){
	}

bool
ESDialog::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		if( m_bTitleBar ){
			PokerGUI* pGUI	= PokerGUI::GetInstance();
			m_pCloseButton	= new ESChildControlImageButton();
			int nId			= MercuryBaseView::GetChildControlIdUnused(m_pOwner);

			m_pCloseButton->SetDefaultImage	(pGUI->m_pDialogCloseNormal);
			m_pCloseButton->SetHoverImage	(pGUI->m_pDialogCloseHover);
			m_pCloseButton->SetSelectedImage(pGUI->m_pDialogClosePushed);
			m_pCloseButton->SetDisabledImage(pGUI->m_pDialogCloseNormal);
			m_pCloseButton->SetSizeAutoCalculation(false);

			CRect		rcOmitBorder	= m_rcOmitBorder;
			// Calculate client area rect.
			m_rcClientArea.left			= m_rcRect.left + rcOmitBorder.left;
			m_rcClientArea.right		= m_rcRect.right - rcOmitBorder.right;
			m_rcClientArea.top			= m_rcRect.top + rcOmitBorder.top;
			m_rcClientArea.bottom		= m_rcRect.bottom - rcOmitBorder.bottom;

			CRect rcCloseButton = GetCloseButtonRect();
			if( !m_pOwner->AddChildControl(m_pCloseButton, nId, rcCloseButton, this) ){
				delete m_pCloseButton;
				m_pCloseButton = NULL;
				}
			else{
				m_pCloseButton->AddEventListener(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&ESDialog::OnCloseDialog);
				}
			}
		}
	return bRet;
	}

ESChildControl*
ESDialog::InitChildControl(DialogItem* pItem){
	ESChildControl* pChild	= NULL;
	PokerGUI*		pGUI	= PokerGUI::GetInstance();
	switch( pItem->wClassId ){
		case 0x80: // Button
			{	
			if( pItem->dwStyle&BS_CHECKBOX ){
				}
			else
			if( pItem->dwStyle&BS_RADIOBUTTON ){
				}
			else{
				ESChildControlImageButton* pCtrl = new ESChildControlImageButton();
				pCtrl->SetDefaultImage			(pGUI->m_pPushButtonNormal);
				pCtrl->SetHoverImage			(pGUI->m_pPushButtonHover);
				pCtrl->SetSelectedImage			(pGUI->m_pPushButtonPushed);
				pCtrl->SetDisabledImage			(pGUI->m_pPushButtonDisabled);
				pCtrl->SetSizeAutoCalculation	(false);
				pCtrl->SetOmitBorder			(pGUI->m_rcPushButtonOmitBorder);
				pCtrl->SetFont					(m_pFont);
				pCtrl->SetTextColor				(pGUI->m_crPushButtonText);
				pCtrl->SetText					(pItem->sText);
				pChild = pCtrl;
				}
			break;
			}
		case 0x82: // Label
			{
			ESChildControlLabel* pCtrl = new ESChildControlLabel();
			pCtrl->SetSizeAutoCalculation(false);
			pCtrl->SetSizeTextFont	(false);
			pCtrl->SetTextColor		(RGB(0, 0, 0));
			pCtrl->SetFont			(m_pFont);
			pCtrl->SetCentred		(false);
			pCtrl->SetLabel			(pItem->sText, false);
			pChild = pCtrl;
			break;
			}
		case 0x81: // Edit
			{
			if( !(pItem->dwStyle&ES_NUMBER) ){
				ESChildControlTextEdit* pCtrl = new ESChildControlTextEdit();
				pCtrl->SetSizeAutoCalculation(false);
				pCtrl->SetFont(m_pFont);
				if( pItem->dwStyle&ES_PASSWORD ){
					pCtrl->SetPassword(true);
					}
				pChild = pCtrl;
				}
			else{
				ESChildControlMoneyEdit* pCtrl = new ESChildControlMoneyEdit();
				pCtrl->SetSizeAutoCalculation(false);
				pCtrl->SetFont(m_pFont);
				pChild = pCtrl;
				}
			break;
			}
		case 0x85: // Combobox
			{
			ESChildControlComboBox* pCtrl = new ESChildControlComboBox();
			pCtrl->SetSizeAutoCalculation(false);
			pCtrl->SetFont(m_pFont);
			if( pItem->dwStyle&CBS_DROPDOWNLIST ){
				pCtrl->SetDropListStyle(false);
				}
			pChild = pCtrl;
			break;
			}
		case 0x83: // listbox
			{
			break;
			}
		}
	return pChild;
	}

CRect
ESDialog::GetDialogOmitBorder(){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	return pGUI->m_rcDialogBkOmitBorder;
	}

void
ESDialog::SetTitleBar(bool bTitleBar){
	m_bTitleBar			= bTitleBar;
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	m_rcOmitBorder		= bTitleBar ? pGUI->m_rcDialogBkOmitBorder : CRect(0, 0, 0, 0);
	}

bool
ESDialog::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESDialog::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESDialog::Destroy(){
	ESChildControl::Destroy();
	if( m_bDestroyFont && m_pFont ){
		delete m_pFont;
		m_pFont = NULL;
		}
	return true;
	}

void
ESDialog::OnLoading(){
	}

void
ESDialog::OwnerWindowSizeChanged(int cx, int cy){
	CRect rcRectOld = m_rcRect;
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	if( m_bCentred ){
		CRect rcRectNew;
		rcRectNew.left		= (cx - m_rcRect.Width()) / 2;
		rcRectNew.top		= (cy - m_rcRect.Height()) / 2;
		rcRectNew.right		= rcRectNew.left + m_rcRect.Width();
		rcRectNew.bottom	= rcRectNew.top + m_rcRect.Height();
		m_rcRect			= rcRectNew;
/*
		CRect rcCloseButton	= GetCloseButtonRect();
		m_pCloseButton->SetPos	(rcCloseButton.left, rcCloseButton.top, false);
		m_pCloseButton->SetSize	(rcCloseButton.Width(), rcCloseButton.Height(), false);*/

		// Move child controls.
		MoveChildControls(m_rcRect, rcRectOld);
		}

	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CRect		rcOmitBorder	= m_rcOmitBorder;
	// Calculate client area rect.
	m_rcClientArea.left			= m_rcRect.left + rcOmitBorder.left;
	m_rcClientArea.right		= m_rcRect.right - rcOmitBorder.right;
	m_rcClientArea.top			= m_rcRect.top + rcOmitBorder.top;
	m_rcClientArea.bottom		= m_rcRect.bottom - rcOmitBorder.bottom;
	}

bool
ESDialog::AddChild(ESChildControl* pChild, CRect rcChild, bool bRedraw){
	if( !pChild ) return false;
	CSize szControl(rcChild.Width(), rcChild.Height());
	rcChild.left	+= m_rcClientArea.left;
	rcChild.right	= rcChild.left + szControl.cx;
	rcChild.top		+= m_rcClientArea.top;
	rcChild.bottom	= rcChild.top + szControl.cy;
	return m_pOwner->AddChildControl(pChild, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcChild, this);
	}

bool
ESDialog::MoveControl(int x, int y, bool bRedraw){
	int nWidth	= m_rcRect.Width();
	int nHeight = m_rcRect.Height();

	CRect rcRectOld = m_rcRect;
	m_rcRect.left	= x;
	m_rcRect.top	= y;
	m_rcRect.right	= m_rcRect.left + nWidth;
	m_rcRect.bottom = m_rcRect.top + nHeight;

	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CRect		rcOmitBorder	= m_rcOmitBorder;
	// Calculate client area rect.
	m_rcClientArea.left			= m_rcRect.left + rcOmitBorder.left;
	m_rcClientArea.right		= m_rcRect.right - rcOmitBorder.right;
	m_rcClientArea.top			= m_rcRect.top + rcOmitBorder.top;
	m_rcClientArea.bottom		= m_rcRect.bottom - rcOmitBorder.bottom;

	// Move child controls.
	MoveChildControls(m_rcRect, rcRectOld);

	if( bRedraw ){
		Redraw(true);
		}
	return true;
	}

BOOL
ESDialog::OnOK(){
	m_nRetCode = IDOK;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, true);
	return FALSE;
	}

BOOL
ESDialog::OnCancel(){
	m_nRetCode = IDCANCEL;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, true);
	return FALSE;
	}

BOOL
ESDialog::OnTabControl(){
	bool bRetry = false;
	ESChildControl* pChild = m_pFocusedChild;
	if( !pChild )
		pChild	= m_pChild;
	else{
		bRetry	= true;
		pChild	= pChild->GetNext();
		}
retry:
	while( pChild ){
		if( pChild->SetFocus() )
			break;
		pChild = pChild->GetNext();
		}

	if( pChild )
		m_pFocusedChild = pChild;
	else{
		bRetry = false;
		pChild = m_pChild;
		goto retry;
		}
	return FALSE;
	}

void
ESDialog::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CImage*		pImageBk		= pGUI->m_pDialogBk;
	CRect		rcOmitBorder	= m_rcOmitBorder;
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
	if( m_bTitleBar ){
		rcInvalid	= rcBkDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() )
			PokerGUI::RenderImage_OmitBorder(pDC, rcInvalid, rcBkDC, pImageBk, rcOmitBorder, TRUE, TRUE);
		}
	// }}
	// Fill color. {{
	rcInvalid	= rcFillDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pDC->FillSolidRect(rcInvalid, m_crFill);
	// }}

	// Draw title. {{
	if( m_bTitleBar ){
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
		}
	// #########################################
	pDC->SelectObject(pFontOld);
	}

BOOL	
ESDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	if( m_bMoving ){
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		return TRUE;
		}
	return FALSE;
	}

void	
ESDialog::OnMouseLeave(UINT nFlags){
	}

void	
ESDialog::OnMouseEnter(UINT nFlags){
	}

void	
ESDialog::OnMouseHover(CPoint pt, UINT nFlags){
	if( m_bMoving ){
		if( (nFlags&MK_LBUTTON) == 0 ){
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			m_bMoving = false;
			}
		else{
			CPoint ptMove(m_rcRect.left + (pt.x - m_ptCaptured.x), m_rcRect.top + (pt.y - m_ptCaptured.y));
			CRect rcRedraw;
			rcRedraw.left	= min(ptMove.x, m_rcRect.left);
			rcRedraw.top	= min(ptMove.y, m_rcRect.top);
			rcRedraw.right	= m_rcRect.right + abs(pt.x - m_ptCaptured.x);
			rcRedraw.bottom = m_rcRect.bottom + abs(pt.y - m_ptCaptured.y);
			MoveControl(ptMove.x, ptMove.y, false);

			m_pOwner->RedrawRect(rcRedraw, 1);
			m_ptCaptured = pt;
			}
		}
	}

void
ESDialog::OnLButtonDown(CPoint pt, UINT nFlags){
	CRect rcCaption = GetCaptionRect();
	if( rcCaption.PtInRect(pt) ){
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		m_ptCaptured = pt;
		m_bMoving	= true;
		}
	}

void
ESDialog::OnLButtonUp(CPoint pt, UINT nFlags){
	if( m_bMoving ){
		m_bMoving = false;
		}
	}

void	
ESDialog::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESDialog::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESDialog::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESDialog::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESDialog::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void
ESDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	}

void	
ESDialog::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}

CRect
ESDialog::GetCloseButtonRect(){
	if( !m_pCloseButton || !m_bTitleBar )
		return CRect(0, 0, 0, 0);

	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CRect		rcOmitBorder	= m_rcOmitBorder;
	CSize		szButton		= pGUI->m_szDialogCloseButton;

	CRect rcClose;
	rcClose.left	= m_rcRect.right - rcOmitBorder.right - szButton.cx - 4;
	rcClose.top		= m_rcRect.top + (rcOmitBorder.top - szButton.cy)/2;
	rcClose.right	= rcClose.left + szButton.cx;
	rcClose.bottom	= rcClose.top + szButton.cy;
	return rcClose;
	}

CRect
ESDialog::GetCaptionRect(){
	if( !m_bTitleBar )
		return CRect(0, 0, 0, 0);

	PokerGUI*	pGUI			= PokerGUI::GetInstance();
	CRect		rcOmitBorder	= pGUI->m_rcDialogBkOmitBorder;

	CRect		rcCaption;
	rcCaption.left		= m_rcRect.left;
	rcCaption.top		= m_rcRect.top;
	rcCaption.right		= m_rcRect.right;
	rcCaption.bottom	= rcCaption.top + rcOmitBorder.top;
	return rcCaption;
	}

int
ESDialog::MoveChildControls(CRect rcParent, CRect rcParentOld){
	// Change child controls rect, relate to parent.
	int				nRet	= 0;
	ESChildControl* pChild	= m_pChild;
	while( pChild ){
		CRect rcChildOld = pChild->GetClientRect();
		CRect rcChild = rcChildOld;
		
		rcChild.left	= rcParent.left + (rcChild.left - rcParentOld.left);
		rcChild.top		= rcParent.top + (rcChild.top - rcParentOld.top);
		rcChild.right	= rcChild.left + rcChildOld.Width();
		rcChild.bottom	= rcChild.top + rcChildOld.Height();

		pChild->SetPos	(rcChild.left, rcChild.top, false);
		pChild->SetSize	(rcChild.Width(), rcChild.Height(), false);

		nRet	++;
		pChild	= pChild->GetNext();
		}

	return nRet;
	}

int
ESDialog::GetCloseButtonId(){
	if( !m_pCloseButton ) return 0;
	return m_pCloseButton->GetId();
	}

void
ESDialog::OnCloseDialog(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	OnCancel();
	}
