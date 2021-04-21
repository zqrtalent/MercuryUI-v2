// PokerHandHistoryWnd.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Controls\MercuryGUI.h"

#include "ChildControls\ESChildControlButton.h"
#include "..\..\Controls\ESChildControlImage.h"
#include "ChildControls\ESChildControlLobbyTab.h"
#include "ChildControls\ESChildControlLobbyBanner.h"
#include "ChildControls\ESChildControlLobbyUserInfo.h"
#include "..\..\Controls\ESChildControlImageButton.h"
#include "ChildControls\ESChildControlInvisible.h"
#include "..\..\Controls\ESChildControlLabel.h"
#include "ChildControls\ESChildHistoryView.h"

#include "..\..\Image\ImageHelper.h"
#include "..\..\Path\Path.h"
#include "PokerHandHistoryWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerHandHistoryWnd

PokerHandHistoryWnd::PokerHandHistoryWnd() : MercuryBaseView(){
	m_bUseForegroundColor	= 0;
	m_crForeground			= RGB(0, 0, 0);
	m_nAlpha				= 50;
	m_pGUI					= PokerGUI::GetInstance();
	}

IMPLEMENT_DYNCREATE(PokerHandHistoryWnd, MercuryBaseView)

PokerHandHistoryWnd::~PokerHandHistoryWnd(){
	}

BEGIN_MESSAGE_MAP(PokerHandHistoryWnd, MercuryBaseView)
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_LBUTTONUP		()
	ON_WM_SIZE			()
	ON_WM_MOUSEMOVE		()
	ON_WM_GETDLGCODE	()
END_MESSAGE_MAP()

BOOL 
PokerHandHistoryWnd::Create(LPCTSTR lpszClassName,
						LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect,
						CWnd* pParentWnd, UINT nID,
						CCreateContext* pContext /*= NULL*/){
	BOOL bRet =  CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if( bRet ){
		CRect rcClient;
		GetClientRect(rcClient);
		// Create child controls.
		CreateChildControls();
		}
	return bRet;
	}

void
PokerHandHistoryWnd::OnPrePaint(CRect& rcRepaint){
	}

void 
PokerHandHistoryWnd::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	RedrawChildControls	(m_pFirstChild, pDC, pRectDC, pRectClient);
	}

void
PokerHandHistoryWnd::OnSize(UINT nType, int cx, int cy){
	MercuryBaseView::OnSize(nType, cx, cy);
	// Resize child controls.
	CreateChildControls();
	}

void
PokerHandHistoryWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	MercuryBaseView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

UINT 
PokerHandHistoryWnd::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

BOOL
PokerHandHistoryWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

void 
PokerHandHistoryWnd::OnMouseMove(UINT nFlags, CPoint point){
	MercuryBaseView::OnMouseMove(nFlags, point);
	}

BOOL 
PokerHandHistoryWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return MercuryBaseView::OnSetCursor(pWnd, nHitTest, message);
	}

void 
PokerHandHistoryWnd::OnLButtonUp(UINT nFlags, CPoint point){
	MercuryBaseView::OnLButtonUp(nFlags, point);
	}

void 
PokerHandHistoryWnd::OnLButtonDblClk(UINT nFlags, CPoint point){
	MercuryBaseView::OnLButtonDblClk(nFlags, point);
	}

void
PokerHandHistoryWnd::OnLButtonDown(UINT nFlags, CPoint point){
	CRect rcRedraw (point.x - 40, point.y - 40, point.x + 100, point.y + 40);
	RedrawRect(rcRedraw, 1);
	MercuryBaseView::OnLButtonDown(nFlags, point);
	}

// ######################################################
//
//	Operations.
//
// ######################################################

void
PokerHandHistoryWnd::SetTitle(CString sTitle, bool bRedraw){
	m_sTitle = sTitle;
//	ESChildControlLabel* pTitle = (ESChildControlLabel*)GetChildControl(CHILD_HistoryTitle);
//	if( pTitle ){
//		pTitle->SetLabel		(m_sTitle, bRedraw);
//		}
	}

int
PokerHandHistoryWnd::AddHistoryInfo(PokerSimpleHistory* pHistoryInfo, bool bClearExisting /*= true*/, bool bRedraw /*= true*/){
//	ESChildHistoryView* pScrollView = (ESChildHistoryView*)GetChildControl(CHILD_HistoryView);
//	if( pScrollView ){
//		return pScrollView->AddHistoryInfo(pHistoryInfo, bClearExisting, bRedraw);
//		}
	return 0;
	}

bool
PokerHandHistoryWnd::CreateChildControls(){
	CRect rcClient;
	GetClientRect(&rcClient);
	/*
	CRect rcTitle;
	rcTitle.left	= rcClient.left;
	rcTitle.top		= rcClient.top;
	rcTitle.right	= rcClient.right;
	rcTitle.bottom	= rcTitle.top + m_pGUI->m_nHistoryTitleHeight;

	CRect rcView;
	rcView.left		= rcClient.left;
	rcView.top		= rcTitle.bottom;
	rcView.right	= rcClient.right;
	rcView.bottom	= rcClient.bottom;

	ESChildControlLabel* pTitle = (ESChildControlLabel*)GetChildControl(CHILD_HistoryTitle);
	if( !pTitle ){
		pTitle = new ESChildControlLabel();
		pTitle->SetTextColor	(m_pGUI->m_crHistoryTitleLabel);
		pTitle->SetFillColor	(m_pGUI->m_crHistoryTitleLabelFill);
		pTitle->SetFont			(m_pGUI->m_pHistoryTitleFont);
		pTitle->SetLabel		(m_sTitle, false);
		pTitle->SetCentred		(true, true);
		pTitle->SetSizeTextFont	(false);

		AddChildControl(pTitle, CHILD_HistoryTitle, rcTitle, NULL);
		pTitle->SetSizeAutoCalculation(false);
		}
	else{
		pTitle->SetPos	(rcTitle.left, rcTitle.top, false);
		pTitle->SetSize	(rcTitle.Width(), rcTitle.Height(), false);
		pTitle->OwnerWindowSizeChanged(rcClient.Width(), rcClient.Height());
		}

	ESChildHistoryView* pScrollView = (ESChildHistoryView*)GetChildControl(CHILD_HistoryView);
	if( !pScrollView ){
		pScrollView = new ESChildHistoryView();
		AddChildControl(pScrollView, CHILD_HistoryView, rcView, NULL);
		pScrollView->SetSizeAutoCalculation(false);
		}
	else{
		pScrollView->SetPos	(rcView.left, rcView.top, false);
		pScrollView->SetSize(rcView.Width(), rcView.Height(), false);
		pScrollView->OwnerWindowSizeChanged(rcClient.Width(), rcClient.Height());
		}*/
	return true;
	}

