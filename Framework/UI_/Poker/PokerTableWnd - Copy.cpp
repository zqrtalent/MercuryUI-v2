// PokerTableWnd.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Controls\MercuryGUI.h"
#include "..\..\Log\Logger.h"
#include "ChildControls\ESChildControlButton.h"
#include "ChildControls\ESChildControlShortcutBtn.h"
#include "ChildControls\ESChildControlSlider.h"
#include "ChildControls\ESChildControlPreAction.h"
#include "ChildControls\ESChildControlActionPanel.h"

#include "ChildControls\ESChildControlTableCards.h"
#include "ChildControls\ESChildControlTable.h"
#include "ChildControls\ESChildSeatStatus.h"
#include "ChildControls\ESChildSeat.h"
#include "ChildControls\ESChildControlChips.h"
#include "ChildControls\ESChildControlSeatCards.h"
#include "ChildControls\ESChildControlSeatHiddenCards.h"

#include "ChildControls\ESChildControlSeat.h"
#include "ChildControls\ESChildControlSeatCards.h"
#include "ChildControls\ESChildControlChips.h"
#include "ChildControls\ESChildControlDealer.h"
#include "ChildControls\ESChildControlSeatProgressBar.h"
#include "ChildControls\ESChildControlTabView.h"
#include "ChildControls\ESChildControlTabViewChatPage.h"
#include "ChildControls\ESChildControlTabViewInfoPage.h"
#include "ChildControls\ESChildControlTabViewStatPage.h"
#include "ChildControls\ESChildControlMoneyEdit.h"
#include "..\..\Controls\ESChildControlLabel.h"
#include "ChildControls\ESChildControlAddChips.h"
#include "ChildControls\ESChildControlFlatButton.h"
#include "ChildControls\ESChildControlFlyingCover.h"

#include "..\..\Image\ImageHelper.h"
#include "..\..\Path\Path.h"
#include "..\..\Protocol\PokerPacket.h"
#include "..\..\PokerServer\Object\PokerHandCards.h"
#include "PokerTableWnd.h"

#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerTableWnd

PokerTableWnd::PokerTableWnd() : MercuryBaseView(){
	m_szDefWindowSize		= CSize(792, 546);
	m_bAllowZooming			= true;
	m_fRatioCX				= 1.0f;
	m_fRatioCY				= 1.0f;

	m_bUseForegroundColor	= 0;
	m_crForeground			= RGB(0, 0, 0);
	m_nAlpha				= 50;
	m_pGUI					= PokerGUI::GetInstance();
	m_currentRound			= PokerRoundType::No;
	m_fZoomFactor			= 1.0f;
	m_seatPanelInfos		= NULL;
	m_nRegularThinkTimeSec	= 20;
	m_pHandler				= NULL;
	m_pTimerActiveSeat		= NULL;
	m_bConnectedMode		= false;

	m_bDesignerMode			= true;

	m_nCard1				= -1;
	m_nCard2				= -1;
	m_nCard3				= -1;
	m_nCard4				= -1;
	m_nCard5				= -1;

	m_nActiveSeat			= -1;
	m_nDealerSeat			= 0;
	m_nTotalPotAmount		= 0;
	m_nMainPotAmount		= 0;
	m_nMaxPlayerCt			= 10;

	// Register child classes. {{
	ESChildControl::RegisterChildClass(_T("ESChildControlTable"),			new ESChildControlTable);
	ESChildControl::RegisterChildClass(_T("ESChildSeatStatus"),				new ESChildSeatStatus);
	ESChildControl::RegisterChildClass(_T("ESChildSeat"),					new ESChildSeat);
	ESChildControl::RegisterChildClass(_T("ESChildControlChips"),			new ESChildControlChips);
	ESChildControl::RegisterChildClass(_T("ESChildControlTableCards"),		new ESChildControlTableCards);
	ESChildControl::RegisterChildClass(_T("ESChildControlSeatCards"),		new ESChildControlSeatCards);
	ESChildControl::RegisterChildClass(_T("ESChildControlSeatHiddenCards"),	new ESChildControlSeatHiddenCards);
	// }}

	// Acceptable max players count. ####
	m_acceptebleMaxPlayersCount.Add(2);
	m_acceptebleMaxPlayersCount.Add(4);
	m_acceptebleMaxPlayersCount.Add(6);
	m_acceptebleMaxPlayersCount.Add(9);
	m_acceptebleMaxPlayersCount.Add(10);
	// #################################
	}

IMPLEMENT_DYNCREATE(PokerTableWnd, MercuryBaseView)

PokerTableWnd::~PokerTableWnd(){
	}

void
PokerTableWnd::StopAnimationThread(){
	m_animationThread.Lock();
	m_listAnimations.RemoveAll(true);
	m_animationThread.Unlock();

	m_animationThread.StopThread(1000);
	}


BEGIN_MESSAGE_MAP(PokerTableWnd, MercuryBaseView)
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_LBUTTONUP		()
	ON_WM_MOUSEWHEEL	()
	ON_WM_MOUSEMOVE		()
	ON_WM_KEYDOWN		()
	ON_WM_TIMER			()
	ON_WM_DESTROY		()

	ON_MESSAGE			(WM_SEAT_CLICK, OnSeatClick)
	ON_MESSAGE			(WM_ACTION_BUTTON_CLICK, OnActionButtonClick)
	ON_MESSAGE			(WM_FLAT_BUTTON_CLICK, OnFlatButtonClick)
	ON_MESSAGE			(WM_SHORTCUT_BUTTON_CLICK, OnShortcutButtonClick)
	ON_MESSAGE			(WM_SLIDER_POS_CHANGED, OnSliderPosChanged)
	ON_MESSAGE			(WM_SLIDER_MONEY_CHANGED, OnSliderMoneyChanged)
	ON_MESSAGE			(WM_PREACTION_CHECKED, OnPreActionChecked)
	ON_MESSAGE			(WM_ADD_CHIPS_CLOSE_BUTTON_CLICK, OnAddChipsClose)

	ON_MESSAGE			(WM_COLLECT_CHIPS_FINISHED, OnCollectChipsFinished)
	ON_MESSAGE			(WM_MOVE_CHIPS_FROM_POT_FINISHED, OnMoveChipsFinished)
	ON_MESSAGE			(WM_SEAT_CARDS_DEAL_FINISHED, OnSeatCardsDealFinished)
	ON_MESSAGE			(WM_TABLE_CARDS_DEAL_FINISHED, OnTableCardsDealFinished)

	// Designer commands.
	ON_COMMAND			(MI_TABLE,					OnCommandTable)
	ON_COMMAND			(MI_SEAT_LABEL_AS_CHILD,	OnCommandSeatStatusAsChild)
	ON_COMMAND			(MI_SEAT_AS_CHILD,			OnCommandSeatAsChild)
	ON_COMMAND			(MI_CHIPS_AS_CHILD,			OnCommandChipsAsChild)
	ON_COMMAND			(MI_HIDDEN_CARDS_AS_CHILD,	OnCommandHiddenCardsAsChild)
	ON_COMMAND			(MI_SEAT_CARDS_AS_CHILD,	OnCommandSeatCardsAsChild)
	ON_COMMAND			(MI_TABLE_CARDS_AS_CHILD,	OnCommandTableCardsAsChild)	   
END_MESSAGE_MAP()

BOOL 
PokerTableWnd::Create(LPCTSTR lpszClassName,
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
		// Resize child controls.
		ResizeChildControls(rcClient.Width(), rcClient.Height());
		}
	return bRet;
	}

void
PokerTableWnd::OnDestroy(){
	MercuryBaseView::OnDestroy();
	}

void
PokerTableWnd::OnPrePaint(CRect& rcRepaint){
	}

void
PokerTableWnd::DesignerContextMenu(CMenu* pPopupMenu, CMenu* pNewControlsMenu, CMenu* pNewChildControlsMenu){
	if( pNewControlsMenu && ::IsMenu(*pNewControlsMenu) ){
		pNewControlsMenu->AppendMenu(MF_SEPARATOR, 0, _T(""));
		pNewControlsMenu->AppendMenu(MF_STRING, MI_TABLE,		(LPCTSTR)_T("Poker Table Control"));
		}

	if( pNewChildControlsMenu && ::IsMenu(*pNewChildControlsMenu) ){
		pNewChildControlsMenu->AppendMenu(MF_SEPARATOR, 0, _T(""));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_SEAT_LABEL_AS_CHILD,	(LPCTSTR)_T("Seat Status Control"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_SEAT_AS_CHILD,			(LPCTSTR)_T("Seat Control"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_CHIPS_AS_CHILD,			(LPCTSTR)_T("Chips Control"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_HIDDEN_CARDS_AS_CHILD,	(LPCTSTR)_T("Hidden Cards"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_TABLE_CARDS_AS_CHILD,	(LPCTSTR)_T("Table Cards"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_SEAT_CARDS_AS_CHILD,	(LPCTSTR)_T("Seat Cards"));
		}
	}


/*
void 
PokerTableWnd::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	CRect rcClipDC = *pRectDC;
	CRect rcDestDC(-1*pRectClient->left, -1*pRectClient->top, pRectDC->Width(), pRectDC->Height());

	DrawFloor			(pDC, &rcDestDC, &rcClipDC);
	RedrawChildControls	(m_pFirstChild, pDC, pRectDC, pRectClient);
	}*/

void
PokerTableWnd::OnShowWindow(BOOL bShow, UINT nStatus){
	CWnd::OnShowWindow(bShow, nStatus);
	}

void
PokerTableWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags){
	CWnd::OnChar(nChar, nRepCnt, nFlags);
	}

BOOL 
PokerTableWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	if( !m_bConnectedMode ){
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);
		}

	if( IsAddChipsControlVisible() ){
		ESChildControlAddChips* pAddChips	= (ESChildControlAddChips*)GetChildControl(CHILD_AddChipsControl);
		int						nStep		= pAddChips->GetSliderStep();
		int						nPosNew		= pAddChips->GetSliderPos() + (zDelta/120)*nStep;
		pAddChips->SetSliderPos(nPosNew);
		return 1;
		}
	else
	if( IsActionSliderVisible() ){
		ESChildControlSlider*		pSlider				= (ESChildControlSlider*)GetChildControl(CHILD_Slider);
		ESChildControlMoneyEdit*	pSliderMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_SliderMoneyEdit);
		if( !pSlider || !pSliderMoneyEdit ) return 1;

		int		nStep		= pSlider->GetSliderStep();
		int		nPosNew		= pSlider->GetSliderPos() + (zDelta/120)*nStep;
		pSlider->SetSliderPos	(nPosNew);

		CEdit* pEdit = pSliderMoneyEdit->GetEditControl();
		if( pEdit && GetFocus() != pEdit ){
			pEdit->SetFocus	();
			}
		pEdit->SetSel	(0, -1);
		return 1;
		}

	/*
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if( pState && m_pView )
		m_pView->SendMessage(pState->m_msgCur.message, pState->m_msgCur.wParam, pState->m_msgCur.lParam);
	*/
	return MercuryBaseView::OnMouseWheel(nFlags, zDelta, pt);
	}

void
PokerTableWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	/*
	if( pState && m_pView )
		m_pView->SendMessage(pState->m_msgCur.message, pState->m_msgCur.wParam, pState->m_msgCur.lParam);
	else*/
	if( !m_bConnectedMode ){
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
		}
	MercuryBaseView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

UINT 
PokerTableWnd::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

BOOL
PokerTableWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

void 
PokerTableWnd::OnMouseMove(UINT nFlags, CPoint point){
	if( !m_bConnectedMode ){
		CWnd::OnMouseMove(nFlags, point);
		return;
		}
	MercuryBaseView::OnMouseMove(nFlags, point);
	}

BOOL 
PokerTableWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	if( !m_bConnectedMode ){
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
		}
	return MercuryBaseView::OnSetCursor(pWnd, nHitTest, message);
	}

void 
PokerTableWnd::OnLButtonUp(UINT nFlags, CPoint point){
	if( !m_bConnectedMode ){
		CWnd::OnLButtonUp(nFlags, point);
		return;
		}
	MercuryBaseView::OnLButtonUp(nFlags, point);
	}

void 
PokerTableWnd::OnLButtonDblClk(UINT nFlags, CPoint point){
	if( !m_bConnectedMode ){
		CWnd::OnLButtonDblClk(nFlags, point);
		return;
		}
	MercuryBaseView::OnLButtonDblClk(nFlags, point);
	}

void
PokerTableWnd::OnLButtonDown(UINT nFlags, CPoint point){

#ifdef _DEBUG
//	CRect rcRedraw (point.x - 40, point.y - 40, point.x + 40, point.y + 40);
//	RedrawRect(rcRedraw, 1);

	// Collect chips.
	//CollectChips(0, 23409);

/*	
	// Move chips from pot.
	CDWordArray arrSeatChipAmounts;
	arrSeatChipAmounts.Add(789789);
	arrSeatChipAmounts.Add(4356);
	arrSeatChipAmounts.Add(0);
	arrSeatChipAmounts.Add(435435);
	arrSeatChipAmounts.Add(0);
	arrSeatChipAmounts.Add(0);
	arrSeatChipAmounts.Add(0);
	arrSeatChipAmounts.Add(0);
	arrSeatChipAmounts.Add(0);
	arrSeatChipAmounts.Add(0);
	MoveChipsFromPot(0, arrSeatChipAmounts);
*/

/*
	// Main chips motion code. {{
	if( m_motion.IsStarted() ){
		m_motion.StopMotion();
		}

	CRect					rcClient;
	GetClientRect(rcClient);
	ESChildControlChips*	pMainChips	= GetMainChipsControl();
	CRect					rcControl	= pMainChips->GetClientRect();
	float 					fPosX		= pMainChips->GetLogicalPosX();
	float 					fPosY		= pMainChips->GetLogicalPosY();
	float 					fPosEndX	= ((point.x - rcControl.Width()/2) / ((float)rcClient.Width()));
	float 					fPosEndY	= ((point.y - rcControl.Height()/2) / ((float)rcClient.Height()));
	
//	m_motion.AddMotionObject(pMainChips, 2000, 20, ESEasyMotionType::MotionQuadraticOut, ESEasyMotionType::MotionQuadraticOut, fPosX, fPosY, fPosEndX, fPosEndY);
	m_motion.AddMotionObject(pMainChips, 400, 20, ESEasyMotionType::MotionQuadraticOut, ESEasyMotionType::MotionQuadraticOut, fPosX, fPosY, fPosEndX, fPosEndY);
	m_motion.StartMotion	();
	// }}
	*/

	/*
	// Collect chips. {{
	if( m_motion.IsStarted() )
		m_motion.StopMotion();

	CRect					rcClient;
	GetClientRect(rcClient);
	ESChildControlChips*	pMainChips	= GetMainChipsControl();
	CRect					rcMainChips	= pMainChips->GetClientRect();
//	float 					fPosEndX	= ((rcMainChips.left + rcMainChips.Width()/2) / ((float)rcClient.Width()));
//	float 					fPosEndY	= ((rcMainChips.top + rcMainChips.Height()/2) / ((float)rcClient.Height()));

	for(int i=0; i<m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(i);
		if( !pSeat || pSeat->GetPotAmount() <= 0 ) 
			continue;

		ESChildControlChips*	pChip		= GetChipsControl(i);
		CRect					rcControl	= pChip->GetClientRect();
		float 					fPosX		= pChip->GetLogicalPosX();
		float 					fPosY		= pChip->GetLogicalPosY();
		float 					fPosEndX	= ((rcMainChips.left + (rcMainChips.Width() - rcControl.Width())/2) / ((float)rcClient.Width()));
		float 					fPosEndY	= ((rcMainChips.top + (rcMainChips.Height() - rcControl.Height())/2) / ((float)rcClient.Height()));

		m_motion.AddMotionObject(pChip, 400, 20, ESEasyMotionType::MotionExponentialIn, ESEasyMotionType::MotionExponentialIn, fPosX, fPosY, fPosEndX, fPosEndY);
		}

	
	if( m_motion.StartMotion(m_hWnd, WM_COLLECT_CHIPS_FINISHED, 5555/*Main pot chiups amount*//*) ){
		SetMainPotAmount(0, true);
		}
	*/

	/*
	// 3 table card animation. {{
	// Table cards motion code. {{
	if( m_motion.IsStarted() ){
		m_motion.StopMotion();
		}

	CRect					rcClient;
	GetClientRect(rcClient);

	float						fCX				= m_pGUI->m_szCardDimension.cx / ((float)m_pGUI->m_szDefWindowSize.cx);
	float						fCY				= m_pGUI->m_szCardDimension.cy / ((float)m_pGUI->m_szDefWindowSize.cy);

	ESChildControlTableCards*	pTableCards		= GetTableCardsControl();
	CRect						rcControl		= pTableCards->GetClientRect();
	float 						fSizeStartCX	= 1.1*fCX;
	float 						fSizeStartCY	= fCY;
	float 						fSizeEndCX		= ((m_pGUI->m_ptTableCard3.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
//	float 						fSizeEndCX		= ((m_pGUI->m_ptTableCard5.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
	float 						fSizeEndCY		= fCY;
	
	pTableCards->SetCard1(10);
	pTableCards->SetCard2(22);
	pTableCards->SetCard3(34);
//	pTableCards->SetCard4(35);
//	pTableCards->SetCard5(36);

	m_motion.AddMotionObject(pTableCards, 250, 20, ESEasyMotionType::MotionQuadraticInOut, ESEasyMotionType::MotionQuadraticOut, 
							fSizeStartCX, fSizeStartCY, fSizeEndCX, fSizeEndCY);
	m_motion.StartMotion	(0,0);
	// }}
	*/

	/*
	// 4 th table card animation. {{
	// Table cards motion code. {{
	if( m_motion.IsStarted() ){
		m_motion.StopMotion();
		}

	CRect					rcClient;
	GetClientRect(rcClient);

	float						fCX				= m_pGUI->m_szCardDimension.cx / ((float)m_pGUI->m_szDefWindowSize.cx);
	float						fCY				= m_pGUI->m_szCardDimension.cy / ((float)m_pGUI->m_szDefWindowSize.cy);

	ESChildControlTableCards*	pTableCards		= GetTableCardsControl();
	CRect						rcControl		= pTableCards->GetClientRect();
	float 						fSizeStartCX	= ((m_pGUI->m_ptTableCard3.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
	float 						fSizeStartCY	= fCY;
	float 						fSizeEndCX		= ((m_pGUI->m_ptTableCard4.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
	float 						fSizeEndCY		= fCY;
	
	pTableCards->SetCard1(10);
	pTableCards->SetCard2(22);
	pTableCards->SetCard3(34);
	pTableCards->SetCard4(35);
	pTableCards->SetCard5(-1);

	m_motion.AddMotionObject(pTableCards, 400, 20, ESEasyMotionType::MotionQuadraticOut, ESEasyMotionType::MotionQuadraticOut, 
							fSizeStartCX, fSizeStartCY, fSizeEndCX, fSizeEndCY);
	m_motion.StartMotion	();
	// }}
	*/
/*
	// 5 th table card animation. {{
	// Table cards motion code. {{
	if( m_motion.IsStarted() ){
		m_motion.StopMotion();
		}

	CRect					rcClient;
	GetClientRect(rcClient);

	float						fCX				= m_pGUI->m_szCardDimension.cx / ((float)m_pGUI->m_szDefWindowSize.cx);
	float						fCY				= m_pGUI->m_szCardDimension.cy / ((float)m_pGUI->m_szDefWindowSize.cy);

	ESChildControlTableCards*	pTableCards		= GetTableCardsControl();
	CRect						rcControl		= pTableCards->GetClientRect();
	float 						fSizeStartCX	= ((m_pGUI->m_ptTableCard4.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
	float 						fSizeStartCY	= fCY;
	float 						fSizeEndCX		= ((m_pGUI->m_ptTableCard5.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
	float 						fSizeEndCY		= fCY;
	
	pTableCards->SetCard1(10);
	pTableCards->SetCard2(22);
	pTableCards->SetCard3(34);
	pTableCards->SetCard4(35);
	pTableCards->SetCard5(36);

	m_motion.AddMotionObject(pTableCards, 400, 20, ESEasyMotionType::MotionQuadraticOut, ESEasyMotionType::MotionQuadraticOut, 
							fSizeStartCX, fSizeStartCY, fSizeEndCX, fSizeEndCY);
	m_motion.StartMotion	();
	// }}
	*/

/*
	// Move chips from pot animation.
	CDWordArray arrTableCards;
	arrTableCards.Add(22);
	arrTableCards.Add(GetTableCard2());
	arrTableCards.Add(GetTableCard3());
	arrTableCards.Add(GetTableCard4());
	arrTableCards.Add(GetTableCard5());
//	SetRound(PokerRoundType::No, &arrTableCards);
	
	CDWordArray arrSeatChips;
	arrSeatChips.Add(0);
	arrSeatChips.Add(0);
	arrSeatChips.Add(330);
	arrSeatChips.Add(0);
	arrSeatChips.Add(0);


	arrSeatChips.Add(222);
	arrSeatChips.Add(0);
	arrSeatChips.Add(0);
	arrSeatChips.Add(456);
	arrSeatChips.Add(0);
	MoveChipsFromPot(5, arrSeatChips);
*/
	// Collect chips.
	//CollectChips(5, 20);
/*
	// Round start animation.
	
	PokerRoundStartedPacket* p = new PokerRoundStartedPacket();
	p->m_btTableCard1	= 12;
	p->m_btTableCard2	= 14;
	p->m_btTableCard3	= 45;
	p->m_btTableCard4	= -1;
	p->m_btTableCard5	= -1;
	p->m_cRoundType		= (char)PokerRoundType::Flop;
	DealTableCardsAnimation(p);
	delete p;
*/

//	SetPotAmount(0, 1199, true);

	/*
	char szBuffer[2048];
	CFile file(_T("c:\\starthand.txt"), CFile::modeRead);
	UINT n = file.Read(szBuffer, 2048);
	szBuffer[n] = '\0';
	CString s = szBuffer;
	p->DeserializeAsXML(&s);*/
/*
	 // Start hand animation.
	PokerHandStartedPacket* p = new PokerHandStartedPacket();

	
	p->m_cDealer = 5;
	p->m_cBigBlind = 5;
	p->m_cSmallBlind = 0;

	PokerHandCards* pHand = NULL;
	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = 34;
	pHand->m_btCard4 = 5;
	pHand->m_btSeat	= 0;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = 35;
	pHand->m_btCard4 = 23;
	pHand->m_btSeat	= 5;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = 12;
	pHand->m_btCard4 = 15;
	pHand->m_btSeat	= 3;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = 45;
	pHand->m_btCard4 = 51;
	pHand->m_btSeat	= 2;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = 45;
	pHand->m_btCard4 = 51;
	pHand->m_btSeat	= 6;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = -1;
	pHand->m_btCard4 = -1;
	pHand->m_btSeat	= 1;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = -1;
	pHand->m_btCard4 = -1;
	pHand->m_btSeat	= 4;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = 45;
	pHand->m_btCard4 = 51;
	pHand->m_btSeat	= 7;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 13;
	pHand->m_btCard2 = 5;
	pHand->m_btCard3 = 46;
	pHand->m_btCard4 = 52;
	pHand->m_btSeat	= 9;
	p->m_handCards.Add(pHand);

	pHand = new PokerHandCards();
	pHand->m_btCard1 = 12;
	pHand->m_btCard2 = 4;
	pHand->m_btCard3 = 45;
	pHand->m_btCard4 = 51;
	pHand->m_btSeat	= 8;
	p->m_handCards.Add(pHand);

	DealSeatCardsAnimation(p);
//	DealSeatCardsAnimation(p);
//	DealSeatCardsAnimation(p);
	delete p;
	*/
/*
	// Move chips from pot animation.
	CDWordArray arrTableCards;
	arrTableCards.Add(22);
	arrTableCards.Add(GetTableCard2());
	arrTableCards.Add(GetTableCard3());
	arrTableCards.Add(GetTableCard4());
	arrTableCards.Add(GetTableCard5());
//	SetRound(PokerRoundType::No, &arrTableCards);
	
	CDWordArray arrSeatChips;
	arrSeatChips.Add(0);
	arrSeatChips.Add(0);
	arrSeatChips.Add(330);
	arrSeatChips.Add(0);
	arrSeatChips.Add(0);


	arrSeatChips.Add(222);
	arrSeatChips.Add(0);
	arrSeatChips.Add(0);
	arrSeatChips.Add(456);
	arrSeatChips.Add(0);
	MoveChipsFromPot(5, arrSeatChips);
*/

//	SetActiveSeat(0);

//	m_bUseForegroundColor = TRUE;

	//int nSeat = 0;
	//SetPotAmount(nSeat, GetPotAmount(nSeat) + 90);
	//Anim_CollectChips();

	/*
	CRect rcSeat;
	int nSeat = 0;
	if( GetSeatPanelByPoint(point, nSeat, rcSeat) ){
		SetupSeatTimer	(nSeat, 20, 0);
		SetActiveSeat	(nSeat, true, true);
		}*/

#endif
	if( !m_bConnectedMode ){
		CWnd::OnLButtonDown(nFlags, point);
		return;
		}
	MercuryBaseView::OnLButtonDown(nFlags, point);
	}

void
PokerTableWnd::OnTimer(UINT_PTR nIDEvent){
	if( nIDEvent == POKER_SEAT_ACTION_TIMER ){
		if( m_pTimerActiveSeat ){
			if( !m_pTimerActiveSeat->IncreaseTimer() ){
				KillTimer(POKER_SEAT_ACTION_TIMER);
				m_pTimerActiveSeat = NULL;
				// Time wasted.
				}
			else{
				int								nSeat		= m_pTimerActiveSeat->GetSeat();
				ESChildControlSeatProgressBar*	pProgress	= GetSeatProgressControl(nSeat);
				if( pProgress ){
					int nPosMax = m_pTimerActiveSeat->GetAllSeconds();
					int nPos	= nPosMax - m_pTimerActiveSeat->GetPassedSeconds();
					pProgress->SetProgressPos(nPos, nPosMax, true);
					}
				}
			}
		else{
			KillTimer(POKER_SEAT_ACTION_TIMER);
			}
		}
	MercuryBaseView::OnTimer(nIDEvent);
	}

// ######################################################
//
//	Operations.
//
// ######################################################

void
PokerTableWnd::SetConnectedMode(bool bConnected /*= true*/, bool bRedraw /*= true*/){
	m_bConnectedMode		= bConnected;
	m_bUseForegroundColor	= !bConnected;
	if( bRedraw )
		Invalidate(FALSE);
	}

bool
PokerTableWnd::SetTableStatistic(PokerOpenTablePacket* p){
	if( !p ) return false;
	ESChildControlTabViewStatPage* pTableStatPage = (ESChildControlTabViewStatPage*)GetChildControl(CHILD_TableStatPage);
	if( pTableStatPage ){
		pTableStatPage->SetStatInfo(p);
		return true;
		}
	return false;
	}


bool
PokerTableWnd::SetTableInfo(PokerTableInfo* pInfo){
	if( !pInfo ) return false;
	ESChildControlTabViewInfoPage* pTableInfoPage = (ESChildControlTabViewInfoPage*)GetChildControl(CHILD_TableInfoPage);
	if( pTableInfoPage ){
		pTableInfoPage->SetTableInfo(pInfo);
		return true;
		}
	return false;
	}

bool
PokerTableWnd::Setup(int nMaxPlayerCt,
					EnumerableObject<PokerTableSeatInfo>* seats, 
					EnumerableObject<PokerAvatar>* seatAvatars,
					PokerRoundType round, 
					CDWordArray *pListTableCards,
					CString	sAmountLabelSufix /*= _T("GEL")*/)
	{
	return false;
	// Remove child controls. {{
	ESChildControl* pControl = NULL;
	for(int i=0; i<m_nMaxPlayerCt;  i++){
		// Seat control.
		pControl = GetChildControl(CHILD_Seat + i);
		if( pControl )
			RemoveChildControl(pControl, true);

		// Seat dealer control.
		pControl = GetChildControl(CHILD_SeatDealer + i);
		if( pControl )
			RemoveChildControl(pControl, true);

		// Seat chip control.
		pControl = GetChildControl(CHILD_SeatChips + i);
		if( pControl )
			RemoveChildControl(pControl, true);

		// Seat progress control.
		pControl = GetChildControl(CHILD_SeatProgressBar + i);
		if( pControl )
			RemoveChildControl(pControl, true);
		}
	m_seats.RemoveAll(true);
	// ###########################

	// Set max player count. ######
	EnumerableObject<SeatPanelInfo>* ppSeatPanelInfos[] = 
			{&m_pGUI->m_seatInfo2, &m_pGUI->m_seatInfo4, 
			&m_pGUI->m_seatInfo6, &m_pGUI->m_seatInfo9, &m_pGUI->m_seatInfo10};

	int nIndex = m_acceptebleMaxPlayersCount.IndexOf(nMaxPlayerCt);
	if( nIndex == -1 ){
		nMaxPlayerCt = 2;
		nIndex = 0;
		}
	m_nMaxPlayerCt		= nMaxPlayerCt;
	m_seatPanelInfos	= ppSeatPanelInfos[nIndex];
	// ############################

	CString sFormat;
//	sFormat = _T("%d.%02d ") + sAmountLabelSufix;
	sFormat = _T("%d.%02d");
	// Set seat info. #############
	m_seats.RemoveAll();
	ESChildControlTable* pTable = (ESChildControlTable*)GetChildControl(CHILD_PokerTable);
	if( seats && seats->GetCount() > 0 ){
		for(int i=0; i<seats->GetCount(); i++){
			PokerTableSeatInfo* pSeatInfo = seats->GetAt(i);
			// Copy PokerTableSeatInfo class into new instance.
			if( pSeatInfo ){
				PokerTableSeatInfo* pSeatCopy = new PokerTableSeatInfo();
				pSeatInfo->Copy(pSeatCopy);
				pSeatInfo = pSeatCopy;
				}

			if( pSeatInfo ){
				PokerAvatar* pAvatarInfo = seatAvatars ? seatAvatars->GetAt(i) : NULL;
				if( pAvatarInfo ){
					if( pAvatarInfo->m_binImage.m_hData != NULL && pAvatarInfo->m_binImage.m_dwDataLength > 0 ){
						pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
						}
					else{
						CString sAvatarFile = m_pGUI->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
						CImage* pAvatarImage = PokerGUI::LoadImage(sAvatarFile);
						if( pAvatarImage ){
							if( Serializable::BinaryFromImage(pAvatarImage, &pAvatarInfo->m_binImage) ){
								pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
								}
							delete pAvatarImage;
							}
/*
						Path::BuildFullPath(sAvatarFile);
						CImage avatar_image;
						if( avatar_image.Load(sAvatarFile) == S_OK && Serializable::BinaryFromImage(&avatar_image, &pAvatarInfo->m_binImage) ){
							pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
							}
						avatar_image.Destroy();*/
						}
					}
				}

			// Create seat control. {{
			SeatPanelInfo*	pSeatPanelInfo	= m_seatPanelInfos->GetAt(i);
			if( pSeatPanelInfo ){
				CRect rcSeat;
				rcSeat.left		= pSeatPanelInfo->m_ptPos.x;
				rcSeat.top		= pSeatPanelInfo->m_ptPos.y;
				rcSeat.right	= rcSeat.left + m_pGUI->m_szSeatPanel.cx;
				rcSeat.bottom	= rcSeat.top + m_pGUI->m_szSeatPanel.cy;

				ESChildControlSeat* pSeat = new ESChildControlSeat();
				pSeat->SetSeatIndex	(i);
				pSeat->SetSeatInfo	(pSeatInfo, pSeatPanelInfo, false);
				pSeat->SetSeatActive(false, false);

				if( AddChildControlEx(pSeat, CHILD_Seat + i, rcSeat, pTable, m_pGUI->m_szDefWindowSize) ){
					// Add seat chips.{{
					CRect rcSeatChips;
					rcSeatChips.left	= pSeatPanelInfo->m_ptChipPos.x;
					rcSeatChips.top		= pSeatPanelInfo->m_ptChipPos.y;
					rcSeatChips.right	= rcSeatChips.left + m_pGUI->m_szChipContainer.cx;
					rcSeatChips.bottom	= rcSeatChips.top + m_pGUI->m_szChipContainer.cy;

					ESChildControlChips* pSeatChips = new ESChildControlChips();
					if( !AddChildControlEx(pSeatChips, CHILD_SeatChips + i, rcSeatChips, pTable, m_pGUI->m_szDefWindowSize) )
						delete pSeatChips;
					else{
						// Set label text position.
						pSeatChips->SetLabelTextPosition(pSeatPanelInfo->m_chipLabelTextPos);
						// Set chip grow direction.
						pSeatChips->SetChipsGrowDir		((pSeatPanelInfo->m_chipGrowDir == ChipGrowDirection::LeftToRight));
						pSeatChips->SetAmount			(pSeatInfo ? pSeatInfo->GetPotAmount() : 0);
						pSeatChips->SetFormatString		(sFormat);
						}
					// }}

					// Add dealer button.{{
					CRect rcDealerBtn;
					rcDealerBtn.left	= pSeatPanelInfo->m_ptDealerButtonPos.x;
					rcDealerBtn.top		= pSeatPanelInfo->m_ptDealerButtonPos.y;
					rcDealerBtn.right	= rcDealerBtn.left + m_pGUI->m_szDealer.cx;
					rcDealerBtn.bottom	= rcDealerBtn.top + m_pGUI->m_szDealer.cy;

					ESChildControlDealer* pSeatDealer = new ESChildControlDealer();
					if( !AddChildControlEx(pSeatDealer, CHILD_SeatDealer + i, rcDealerBtn, pTable, m_pGUI->m_szDefWindowSize) )
						delete pSeatDealer;
					else
						pSeatDealer->SetVisible(false, false);
					// }}

					// Add seat progress.{{
					CRect rcSeatProgress;
					rcSeatProgress.left		= pSeatPanelInfo->m_ptPos.x + m_pGUI->m_ptProgressBarPos.x;
					rcSeatProgress.top		= pSeatPanelInfo->m_ptPos.y + m_pGUI->m_ptProgressBarPos.y;
					rcSeatProgress.right	= rcSeatProgress.left + m_pGUI->m_szProgressBar.cx;
					rcSeatProgress.bottom	= rcSeatProgress.top + m_pGUI->m_szProgressBar.cy;

					ESChildControlSeatProgressBar* pSeatProgress = new ESChildControlSeatProgressBar();
					if( !AddChildControlEx(pSeatProgress, CHILD_SeatProgressBar + i, rcSeatProgress, pTable, m_pGUI->m_szDefWindowSize) )
						delete pSeatProgress;
					else
						pSeatProgress->SetVisible(false, false);
					// }}
					}
				else
					delete pSeat;
				}
			// }}

			m_seats.Add(pSeatInfo);

			// Set seat hand cards class.
			if( pSeatInfo ){
				//SetHandCards(i, (PokerHandCardsClass)pSeatInfo->GetHandCardsClass());
				}
			}
		}
	// ############################

	

	AddFlatButton(0, CHILD_Button_LeaveTable, PokerGUI::GetLangValue(_T("LABEL_LEAVE_TABLE")), true);
//	AddFlatButton(0, CHILD_Button_LeaveTable, _T("Leave Table"), true);
	AddFlatButton(2, CHILD_Button_ViewLobby, PokerGUI::GetLangValue(_T("LABEL_VIEW_LOBBY")), true);
//	AddFlatButton(2, CHILD_Button_ViewLobby, _T("View Lobby"), true);
	AddFlatButton(3, CHILD_Button_History, PokerGUI::GetLangValue(_T("LABEL_VIEW_HISTORY")), true);
//	AddFlatButton(3, CHILD_Button_History, _T("View History"), true);
	
	// Set main chips pot label format.
	ESChildControlChips* pMainPotChips = (ESChildControlChips*)GetChildControl(CHILD_SeatMainPotChips);
	if( pMainPotChips ){
//		sFormat.Format(_T("%s"), PokerGUI::GetLangValue(_T("LABEL_MAIN_POT")));
//		sFormat += _T(": %d.%02d ") + sAmountLabelSufix; 
		//sFormat = _T("Main Pot: %d.%02d ") + sAmountLabelSufix; 
		sFormat = _T("%d.%02d ") + sAmountLabelSufix; 
		pMainPotChips->SetFormatString(sFormat);
		}

	// Set current round. #########
	m_currentRound	= round;
	m_nCard1		= -1;
	m_nCard2		= -1;
	m_nCard3		= -1;
	m_nCard4		= -1;
	m_nCard5		= -1;

	if( pListTableCards ){
		int* ppTableCards[] = {&m_nCard1, &m_nCard2, &m_nCard3, &m_nCard4, &m_nCard5};
		for(int i=0; i<max(pListTableCards->GetCount(), 5); i++){
			*ppTableCards[i] = pListTableCards->GetAt(i);
			}
		}

	ESChildControlTableCards* pTableCards = (ESChildControlTableCards*)GetChildControl(CHILD_PokerTableCards);
	if( pTableCards ){
		pTableCards->SetCard1(m_nCard1);
		pTableCards->SetCard2(m_nCard2);
		pTableCards->SetCard3(m_nCard3);
		pTableCards->SetCard4(m_nCard4);
		pTableCards->SetCard5(m_nCard5);
		}
	// ############################
	m_sAmountLabelSuffix = sAmountLabelSufix;
	return true;
	}

bool
PokerTableWnd::CreateChildControls(){
	CRect rcClient;
	GetClientRect(rcClient);
	return true;

	// Action panel.
	ESChildControlActionPanel* pActionPanel = new ESChildControlActionPanel();
	CRect rcActionPanel = m_pGUI->m_rcActionPanel;
	AddChildControl(pActionPanel, CHILD_PokerActionPanel, rcActionPanel, NULL);
	/*
	// TabView control. {{
	CRect rcTabView;
	rcTabView.left		= m_pGUI->m_ptTabView.x;
	rcTabView.top		= m_pGUI->m_ptTabView.y;
	rcTabView.right		= rcTabView.left + m_pGUI->m_szTabView.cx;
	rcTabView.bottom	= rcTabView.top + m_pGUI->m_szTabView.cy;

	ESChildControlTabView* pTabView = new ESChildControlTabView();
	if( !AddChildControlEx(pTabView, CHILD_TableTabView, rcTabView, pActionPanel, m_pGUI->m_szDefWindowSize) ){
		delete pTabView;
		pTabView = NULL;
		}
	else{
		ESChildControlTabViewChatPage* pChatPage = new ESChildControlTabViewChatPage();
		pTabView->AddTabPage(pChatPage, CHILD_TableChatPage, PokerGUI::GetLangValue(_T("LABEL_CHAT_TAB")), 0);

		// Create formats. {{
		pChatPage->CreateTextFormat(_T("Time"),			_T("Segoe UI"), false, false, false, m_pGUI->m_nChatTimeFontHeight);
		pChatPage->CreateTextFormat(_T("UserName"),		_T("Segoe UI"), false, false, true, m_pGUI->m_nChatUserNameFontHeight);
		pChatPage->CreateTextFormat(_T("Dealer"),		_T("Segoe UI"), false, false, false, m_pGUI->m_nChatDealerFontHeight);
		pChatPage->CreateTextFormat(_T("System"),		_T("Segoe UI"), false, false, false, m_pGUI->m_nChatSystemFontHeight);
		pChatPage->CreateTextFormat(_T("Message"),		_T("Segoe UI"), false, false, false, m_pGUI->m_nChatMessageFontHeight);
		// }}*/

		/*
		CString sMessage;
		sMessage.Format(_T("River dealt: ^%s ^%s ^%s ^%s ^%s"), PokerGUI::CardNameByIndex(11), PokerGUI::CardNameByIndex(24), 
			PokerGUI::CardNameByIndex(45), PokerGUI::CardNameByIndex(6), PokerGUI::CardNameByIndex(23));
		pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);


		int nLine1 = pChatPage->AddTextToLine(-1, _T("Time"), _T("dfgdfgdfdgdf^2r^^2D^2H^2Srtyrty^2Cdfgdfgdgdf"), RGB(0, 122, 0), false);
		pChatPage->AddTextToLine(-1, _T("Time"), _T("dfgdfgdfdgdf"), RGB(0, 122, 0), false);

		//int nLine1 = 0;
		
		int nLine1 = pChatPage->AddTextToLine(-1, _T("Time"), _T("[11:12:00 11:23] "), RGB(0, 122, 0));
		pChatPage->AddTextToLine(nLine1, _T("DealerSign"), _T("D: "), RGB(0, 0, 0));
		pChatPage->AddTextToLine(nLine1, _T("Dealer"), _T("zqrtalent calls "), RGB(0, 122, 0));
		pChatPage->AddTextToLine(nLine1, _T("Dealer"), _T("0.22 GEL"), RGB(122, 122, 0));

		
	//	pChatPage->AddTextToLine(-1, _T("Dealer"), _T(" 1111 "), RGB(0, 122, 0));
	//	pChatPage->AddTextToLine(0, _T("System"), _T(" 2222 "), RGB(0, 122, 122));
	//	pChatPage->AddTextToLine(0, _T("User"),	_T(" 3333 "), RGB(0, 122, 0));

		pChatPage->AddTextToLine(-1, _T("Dealer"), _T(" 1111 "), RGB(0, 122, 0));
		pChatPage->AddTextToLine(-1, _T("System"), _T(" 2222 "), RGB(0, 122, 122));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T(" 3333 "), RGB(0, 122, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 122, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));
		pChatPage->AddTextToLine(-1, _T("UserName"),	_T("https:////maximum.ge//Casino//ChooseGame.aspx"), RGB(122, 0, 0));

		/*
		nLine1 = pChatPage->AddImageToLine(-1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);
		pChatPage->AddImageToLine(nLine1, _T("theme\\rank_star.png"), 13, 12);

		pChatPage->GotoLine(pChatPage->GetLineCount() - 1, false);*/
		/**/
		/*
		ESChildControlTabViewInfoPage* pInfoPage = new ESChildControlTabViewInfoPage();
		pTabView->AddTabPage(pInfoPage, CHILD_TableInfoPage, PokerGUI::GetLangValue(_T("LABEL_TABLE_INFO")), 1);

		ESChildControlTabViewStatPage* pStatPage = new ESChildControlTabViewStatPage();
		pTabView->AddTabPage(pStatPage, CHILD_TableStatPage, PokerGUI::GetLangValue(_T("LABEL_TABLE_STAT")), 2);
		}
	// }}
*/
	// Table control.
	CRect rcTable;
	rcTable.left	= m_pGUI->m_ptTablePos.x;
	rcTable.top		= m_pGUI->m_ptTablePos.y;
	rcTable.right	= rcTable.left + m_pGUI->m_szTable.cx;
	rcTable.bottom	= rcTable.top + m_pGUI->m_szTable.cy;

	ESChildControlTable* pTable = new ESChildControlTable();
	if( !AddChildControlEx(pTable, CHILD_PokerTable, rcTable, NULL, m_pGUI->m_szDefWindowSize) ){
		delete pTable;
		pTable = NULL;
		}
	// }}

	// Add table cards.{{
	CRect rcTableCards;
	rcTableCards.left	= m_pGUI->m_ptTableCard1.x;
	rcTableCards.top	= m_pGUI->m_ptTableCard1.y;
	rcTableCards.right	= rcTableCards.left + (m_pGUI->m_ptTableCard5.x - m_pGUI->m_ptTableCard1.x + m_pGUI->m_szCardDimension.cx);
	rcTableCards.bottom	= rcTableCards.top + m_pGUI->m_szCardDimension.cy + 5;

	ESChildControlTableCards* pTableCards = new ESChildControlTableCards();
	if( !AddChildControlEx(pTableCards, CHILD_PokerTableCards, rcTableCards, pTable, m_pGUI->m_szDefWindowSize) ){
		delete pTableCards;
		pTableCards = NULL;
		}
	// }}

	// Table main pot chips
	ESChildControlChips* pChips = new ESChildControlChips();
	if( AddChildControl(pChips, CHILD_SeatMainPotChips, CRect(0, 0, 1, 1), pTable) ){
		float fPosX = (m_pGUI->m_ptMainPotChipsPos.x / ((float)m_pGUI->m_szDefWindowSize.cx));
		float fPosY = (m_pGUI->m_ptMainPotChipsPos.y / ((float)m_pGUI->m_szDefWindowSize.cy));
		float fCX	= (2*m_pGUI->m_szChipContainer.cx / ((float)m_pGUI->m_szDefWindowSize.cx));
		float fCY	= (m_pGUI->m_szChipContainer.cy / ((float)m_pGUI->m_szDefWindowSize.cy));

		pChips->SetLogicalPos	(fPosX, fPosY);
		pChips->SetLogicalSize	(fCX, fCY);
		}

	// Total pot label.
	CRect rcTotalPot;
	rcTotalPot.left		= m_pGUI->m_ptTotalPotLabelPos.x;
	rcTotalPot.top		= m_pGUI->m_ptTotalPotLabelPos.y;
	rcTotalPot.right	= rcTotalPot.left + m_pGUI->m_szTotalPotLabel.cx;
	rcTotalPot.bottom	= rcTotalPot.top + m_pGUI->m_szTotalPotLabel.cy;

	ESChildControlLabel* pLabel = new ESChildControlLabel();
	if( AddChildControlEx(pLabel, CHILD_TotalPotLabel, rcTotalPot, pTable, m_pGUI->m_szDefWindowSize) ){
		pLabel->SetTextColor(m_pGUI->m_crTotalPotLabel);
		pLabel->SetFont		(m_pGUI->m_pTotalPotLabelFont);
		}
	else{
		delete pLabel;
		pLabel = NULL;
		}
	// }}

	// Table message label.
	CRect rcTableMessage;
	rcTableMessage.left		= m_pGUI->m_ptTableCard1.x;
	rcTableMessage.top		= m_pGUI->m_ptTableCard1.y + m_pGUI->m_szCardDimension.cy;
	rcTableMessage.right	= m_pGUI->m_ptTableCard5.x + m_pGUI->m_szCardDimension.cx;
	rcTableMessage.bottom	= rcTableMessage.top + 100;

	ESChildControlLabel* pMessageLabel = new ESChildControlLabel();
	if( AddChildControlEx(pMessageLabel, CHILD_Table_Message, rcTableMessage, pTable, m_pGUI->m_szDefWindowSize) ){
		pMessageLabel->SetLabel		(_T("Hello everyone !!!\r\nThis is my new poker room\r\nEnjoy :)\r\nთინათი�\r\nfgdfgdfgd\r\n"), false);
//		pMessageLabel->SetZIndex	(2);
		pMessageLabel->SetTextColor	(m_pGUI->m_crTMessageText);
		pMessageLabel->SetFont		(m_pGUI->m_pTMessageFont);
		pMessageLabel->SetBkImage	(m_pGUI->m_pTableMessageBk, m_pGUI->m_rcTableMessageOmit);
		pMessageLabel->SetCentred	(true);
		pMessageLabel->SetVisible	(false, false);
		}
	else{
		delete pMessageLabel;
		pMessageLabel = NULL;
		}
	// }}

/*
	// Slider
	ShowSlider(100, 10, 1000, 10, false);

	// Action buttons.
	AddActionButton(ActionButton::ButtonFold, _T("Fold"), 0, 0, false);
	AddActionButton(ActionButton::ButtonCall, _T("Call"), 100, 100, false);
	AddActionButton(ActionButton::ButtonBet, _T("Raise"), 100, 10000, false);

	
	// Pre-Actions
	AddPreActionButton(PreActionButton::PreAction_AutoMuck, 0, _T("Auto Muck"), true, false);
	AddPreActionButton(PreActionButton::PreAction_AutoPostBB, 0, _T("Auto Post Blind"), true, false);
	AddPreActionButton(PreActionButton::PreAction_SitOutNextBB, 0, _T("Sit Out Next BB"), true, false);
	AddPreActionButton(PreActionButton::PreAction_SitOutNextHand, 0, _T("Sit Out Next Hand"), true, false);

	AddPreActionButton(PreActionButton::PreAction_CheckFold, 0, _T("Check/Fold"), true, false);
	AddPreActionButton(PreActionButton::PreAction_Call, 0, _T("Call 0.1"), true, false);
	AddPreActionButton(PreActionButton::PreAction_CallAny, 0, _T("Call Any"), true, false);

	// Add chips controls.
//	ShowAddChipsControl(100, 10, 1000, 10, 900, false);

		// Slider
	ShowSlider(100, 10, 1000, 10, false);

	// Action buttons.
	AddActionButton(ActionButton::ButtonFold, _T("Fold"), 0, 0, false);
	AddActionButton(ActionButton::ButtonCall, _T("Call"), 100, 100, false);
	AddActionButton(ActionButton::ButtonBet, _T("Raise"), 100, 10000, false);

	// Shortcut buttons.
	AddShortcutButton(_T("BB"), 100, false);
	AddShortcutButton(_T("2BB"), 200, false);
	AddShortcutButton(_T("3BB"), 300, false);
	AddShortcutButton(_T("5BB"), 500, false);
	AddShortcutButton(_T("ALLIN"), -1, false);

	
	
	// Pre-Actions
	AddPreActionButton(PreActionButton::PreAction_CheckFold, _T("Check/Fold"), true, false);
	AddPreActionButton(PreActionButton::PreAction_Check, _T("Check"), true, false);
	AddPreActionButton(PreActionButton::PreAction_Fold, _T("Fold"), true, false);
	AddPreActionButton(PreActionButton::PreAction_Call, _T("Call"), true, false);
	AddPreActionButton(PreActionButton::PreAction_CallAny, _T("Call Any"), true, false);
	AddPreActionButton(PreActionButton::PreAction_CallAnyCheck, _T("Call Any/Check"), true, false);

	// Action buttons.
	AddActionButton(ActionButton::ButtonFold, _T("Fold"), 0, 0, false);
	AddActionButton(ActionButton::ButtonCall, _T("Call"), 100, 100, false);
	AddActionButton(ActionButton::ButtonBet, _T("Raise"), 100, 10000, false);

	// Shortcut buttons.
	AddShortcutButton(_T("BB"), 100, false);
	AddShortcutButton(_T("2BB"), 200, false);
	AddShortcutButton(_T("3BB"), 300, false);
	AddShortcutButton(_T("5BB"), 500, false);
	AddShortcutButton(_T("ALLIN"), -1, false);

	// Slider
	ShowSlider(100, 10, 1000, 10, false);

	// Pre-Actions
	AddPreActionButton(PreActionButton::PreAction_AutoMuck, 0, _T("Auto Muck"), true, false);
	AddPreActionButton(PreActionButton::PreAction_AutoPostBB, 0, _T("Auto Post Blind"), true, false);
	AddPreActionButton(PreActionButton::PreAction_SitOutNextBB, 0, _T("Sit Out Next BB"), true, false);
	AddPreActionButton(PreActionButton::PreAction_SitOutNextHand, 0, _T("Sit Out Next Hand"), true, false);

	AddPreActionButton(PreActionButton::PreAction_CheckFold, 0, _T("Check/Fold"), true, false);
	AddPreActionButton(PreActionButton::PreAction_Call, 0, _T("Call 0.1"), true, false);
	AddPreActionButton(PreActionButton::PreAction_CallAny, 0, _T("Call Any"), true, false);

	AddPreActionButton(PreActionButton::PreAction_WaitForBB_PostBB, 0, PokerGUI::GetLangValue(_T("PRE_ACTION_POST_BB_R_WAIT_FOR_BB")), true, false, true);

	EnumerableObject<PokerTableSeatInfo> seats;
	PokerTableSeatInfo* pSeatInfo = NULL;

	CLongBinary imageData;
	CString sAvatarFile = m_pGUI->m_sAvatarsCacheFolder + _T("\\") + _T("2818a0c4b1bf2fb3ef9fec0944f878c1.png");
	CImage* pAvatarImage = PokerGUI::LoadImage(sAvatarFile);
	if( pAvatarImage ){
		Serializable::BinaryFromImage(pAvatarImage, &imageData);
		delete pAvatarImage;
		}

//	SetTableMessage(_T(" Four Of A Kind "), true);

//	int nAmounts[] = {199, 199, 199, 199, 199, 199, 199, 199, 199, 199};
	int nAmounts[] = {0, 0, 1199, 199, 199, 0, 0, 0, 0, 199};

	for(int i=0; i<10; i++){
		pSeatInfo = new PokerTableSeatInfo();
		pSeatInfo->SetUserName	(_T("ZqrTalent"));
		pSeatInfo->SetActionName(_T("Call 0.4"));
		pSeatInfo->SetBalance	(5090);
//		pSeatInfo->SetPotAmount	(199);
//		pSeatInfo->SetPotAmount	(nAmounts[i]);
		pSeatInfo->SetStatus	(PokerSeatStatus::Sitted);
//		pSeatInfo->SetCard1		(11);
//		pSeatInfo->SetCard2		(10);
//		pSeatInfo->SetCard3		(9);
//		pSeatInfo->SetCard4		(8);
		pSeatInfo->SetRankLevel	(4);
		pSeatInfo->SetSeat		(i);
		pSeatInfo->SetAvatarData(&imageData);
		seats.Add(pSeatInfo);
		}

	EnumerableObject<PokerAvatar> avatars;
	PokerAvatar* pAvatar = new PokerAvatar();
	pAvatar->m_nId = 1;
	pAvatar->m_nPrivateUserId = 0;
	pAvatar->m_sAvatarFileName = _T("2818a0c4b1bf2fb3ef9fec0944f878c1.png");
	pAvatar->m_recStatus = 0;
	avatars.Add(pAvatar);

	pAvatar = new PokerAvatar();
	pAvatar->m_nId = 1;
	pAvatar->m_nPrivateUserId = 0;
	pAvatar->m_sAvatarFileName = _T("815f82d7cc59230f978fb5c7e7fe5bf8.png");
	pAvatar->m_recStatus = 0;
	avatars.Add(pAvatar);

	pAvatar = new PokerAvatar();
	pAvatar->m_nId = 1;
	pAvatar->m_nPrivateUserId = 0;
	pAvatar->m_sAvatarFileName = _T("2818a0c4b1bf2fb3ef9fec0944f878c1.png");
	pAvatar->m_recStatus = 0;
	avatars.Add(pAvatar);


	Setup			(10, &seats, &avatars, PokerRoundType::No, NULL);
//	SetTableCards	(26, 27, 29, 34, 30);
	SetMainPotAmount(12340, true);*/
	return true;
	}

int
PokerTableWnd::GetMaxPotAmount(){
	int nMaxPotAmount = 0;
	for( int i=0; i<m_seats.GetCount(); i++ ){
		PokerTableSeatInfo* pSeatInfo = m_seats.GetAt(i);
		if( pSeatInfo )
			nMaxPotAmount = max(nMaxPotAmount, pSeatInfo->GetPotAmount());
		}
	return nMaxPotAmount;
	}

bool
PokerTableWnd::SetSeatAvatarInfo(int nSeat, PokerAvatar* pAvatarInfo, bool bRedraw /*= true*/){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;

	if( pAvatarInfo ){
		if( pAvatarInfo->m_binImage.m_hData != NULL && pAvatarInfo->m_binImage.m_dwDataLength > 0 ){
			pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
			}
		else{
			CString sAvatarFile = m_pGUI->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
			CImage* pAvatarImage = PokerGUI::LoadImage(sAvatarFile);
			if( pAvatarImage ){
				if( Serializable::BinaryFromImage(pAvatarImage, &pAvatarInfo->m_binImage) ){
					pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
					}
				delete pAvatarImage;
				}
/*
			CString sAvatarFile = m_pGUI->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
			Path::BuildFullPath(sAvatarFile);
			CImage avatar_image;
			if( avatar_image.Load(sAvatarFile) == S_OK && Serializable::BinaryFromImage(&avatar_image, &pAvatarInfo->m_binImage) ){
				pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
				}
			avatar_image.Destroy();*/
			}
		}
	
	if( bRedraw )
		RedrawSeat(nSeat);
	return true;
	}

bool
PokerTableWnd::SetSeatInfo(int nSeatIndex, PokerTableSeatInfo* pInfo, bool bRedraw /*= true*/){
	if( nSeatIndex < 0 || nSeatIndex >= m_nMaxPlayerCt )
		return false;

	// Keep avatar image of existing seat.
	PokerTableSeatInfo* pSeatExisting = m_seats.GetAt(nSeatIndex);
	if( pSeatExisting && pInfo && pSeatExisting->GetUserId() == pInfo->GetUserId() ){
		pInfo->SetAvatarImage(pSeatExisting->GetAvatarImage());
		pSeatExisting->SetAvatarImage(NULL); // Clear avatar image of old one.
		}

	if( !m_seats.SetAt(nSeatIndex, pInfo, true) )
		return false;

	ESChildControlSeat* pSeatCtrl = GetSeatControl(nSeatIndex);
	if( pSeatCtrl )
		pSeatCtrl->SetSeatInfo(pInfo, GetSeatPanelInfo(nSeatIndex), false);

	SetSeatChips(nSeatIndex, pInfo ? pInfo->GetPotAmount() : 0);

	if( bRedraw ){
		RedrawSeat		(nSeatIndex);
		RedrawSeatChips	(nSeatIndex);
		}
	return true;
	}

void
PokerTableWnd::SetRound(PokerRoundType round, CDWordArray *pListTableCards, bool bRedrawCards /*= false*/){
	m_currentRound = round;
	if( !pListTableCards ){
		SetTableCards(-1, -1, -1, -1, -1, bRedrawCards);
		return;
		}

	int *pCards[]	= {&m_nCard1, &m_nCard2, &m_nCard3, &m_nCard4, &m_nCard5};
	int nLoop		= 0;
	int nCt			= min(pListTableCards->GetCount(), 5);

	while( nLoop < nCt ){
		*pCards[nLoop] = (int)pListTableCards->GetAt(nLoop);
		nLoop ++;
		}

	SetTableCards(m_nCard1, m_nCard2, m_nCard3, m_nCard4, m_nCard5, bRedrawCards);
	}

bool
PokerTableWnd::SetupSeatTimer(int nSeat, int nFullSeconds, int nSecondsPassed, bool bInvalidate /*= false*/, bool bStart /*= false*/){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return false;

	pSeat->SetupTimer(nFullSeconds, nSecondsPassed);
	// Start or stop action timer. {{
	StartActionTimer(nSeat, bStart, bInvalidate);
	// }}
	return true;
	}

void
PokerTableWnd::SetDealerSeat(int nSeatIndex, bool bInvalidate){
	int nLoop = 0;
	while( nLoop < m_seats.GetCount() ){
		ESChildControlDealer*	pDealer = GetSeatDealerControl(nLoop);
		if( pDealer ){
			if( nSeatIndex == nLoop )
				pDealer->SetVisible(true, bInvalidate);
			else{
				if( pDealer->IsVisible() )
					pDealer->SetVisible(false, bInvalidate);
				}
			}
		nLoop ++;
		}
	}

void		
PokerTableWnd::SetTableCards(int nCard1, int nCard2, int nCard3, int nCard4 /*= -1*/, int nCard5 /*= -1*/, bool bInvalidate /*= true*/){
	m_nCard1 = nCard1;
	m_nCard2 = nCard2;
	m_nCard3 = nCard3;
	m_nCard4 = nCard4;
	m_nCard5 = nCard5;

	ESChildControlTableCards* pTableCards = (ESChildControlTableCards*)GetChildControl(CHILD_PokerTableCards);
	if( pTableCards ){
		pTableCards->SetCard1(m_nCard1);
		pTableCards->SetCard2(m_nCard2);
		pTableCards->SetCard3(m_nCard3);
		pTableCards->SetCard4(m_nCard4);
		pTableCards->SetCard5(m_nCard5);

		pTableCards->SetMarkCards(false, false, false, false, false, false);

		if( bInvalidate )
			pTableCards->Redraw();
		}
	}

// Set seat cards.
bool
PokerTableWnd::SetSeatCards(int nSeat, int nCard1, int nCard2, int nCard3, int nCard4, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat ){
		RedrawSeatCards(nSeat);
		return false;
		}

	pSeat->SetCard1(nCard1);
	pSeat->SetCard2(nCard2);
	pSeat->SetCard3(nCard3);
	pSeat->SetCard4(nCard4);
	
	if( bInvalidate )
		RedrawSeatCards(nSeat);
	return true;
	}

void		
PokerTableWnd::SetTotalPotAmount(int nTotalPotAmount, bool bRedraw){
	int nAmountOld		= m_nTotalPotAmount;
	m_nTotalPotAmount	= nTotalPotAmount;

	ESChildControlLabel* pLabel = (ESChildControlLabel*)GetChildControl(CHILD_TotalPotLabel);
	if( pLabel ){
		if( m_nTotalPotAmount > 0 ){
			CString sLabel;
			sLabel.Format(_T("%s: %d.%02d %s"), PokerGUI::GetLangValue(_T("LABEL_TOTAL_POT")), m_nTotalPotAmount/100, m_nTotalPotAmount%100, m_sAmountLabelSuffix);
			//sLabel.Format(_T("Total Pot: %d.%02d %s"), m_nTotalPotAmount/100, m_nTotalPotAmount%100, m_sAmountLabelSuffix);
			pLabel->SetLabel(sLabel, bRedraw);
			}
		else{
			pLabel->SetLabel(_T(""), bRedraw);
			}
		}
	}

void		
PokerTableWnd::SetMainPotAmount(int nMainPotAmount, bool bRedraw){
	int nAmountOld		= m_nMainPotAmount;
	m_nMainPotAmount	= nMainPotAmount;
	SetSeatChips(CHILD_SeatMainPotChips - CHILD_SeatChips, m_nMainPotAmount);
	if( bRedraw )
		RedrawSeatChips	(CHILD_SeatMainPotChips - CHILD_SeatChips);
	}

// ######################################################
//
//	Drawing methods.
//
// ######################################################

bool
PokerTableWnd::DrawFloor(CDC* pDC, CRect* pRectDC, CRect* pClipRect){
	ASSERT( pRectDC && pClipRect );

	if( m_pGUI->m_pImageTable ){
		CRect rcClient;
		GetClientRect(rcClient);

		CRect rcTableDC;
		rcTableDC.left		= pRectDC->left - pClipRect->left;
		rcTableDC.top		= pRectDC->top - pClipRect->top;
		rcTableDC.right		= rcTableDC.left + rcClient.Width();
		rcTableDC.bottom	= rcTableDC.top + rcClient.Height();

		CRect rcInvalid		= rcTableDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() )
			PokerGUI::RenderImage(pDC, rcInvalid, rcTableDC, m_pGUI->m_pImageTable);
		}

	// m_pGUI->m_pImageTable

	/*

	CImage* pImage = m_pGUI->m_pFloorBgImg;
	if( pImage ){
		bool bRet = ImgHlp::DrawImage32bpp_Repeated(pDC, pImage, *pRectDC, *pClipRect);
		return bRet;
		}*/
	return false;
	}

int
PokerTableWnd::GetPotAmount(int nSeat){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return 0;
	return pSeat->GetPotAmount();
	}

bool
PokerTableWnd::SetPotAmount(int nSeat, int nAmount, bool bRedraw /*= true*/){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return false;

	pSeat->SetPotAmount	(nAmount);
	SetSeatChips		(nSeat, nAmount);
	if( bRedraw ){
		RedrawSeatChips(nSeat);
		}
	return true;
	}

int
PokerTableWnd::GetSeatTotalPot(int nSeat){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return 0;
	return pSeat->GetTotalPot();
	}

bool
PokerTableWnd::SetSeatTotalPot(int nSeat, int nAmount, bool bRedraw /*= true*/){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return false;

	int nAmountOld = pSeat->GetTotalPot();
	pSeat->SetTotalPot(nAmount);
	if( bRedraw ){
		RedrawSeat(nSeat);
		}
	return true;
	}

int
PokerTableWnd::GetBalance(int nSeat){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return 0;
	return pSeat->GetBalance();
	}

bool
PokerTableWnd::SetBalance(int nSeat, int nBalance, bool bRedraw /*= true*/){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return false;
	pSeat->SetBalance(nBalance);
	if( bRedraw )
		RedrawSeat(nSeat);
	return true;
	}

bool
PokerTableWnd::StartActionTimer(int nSeat, bool bStart /*= true*/, bool bInvalidate /*= false*/){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat )
		return false;

	if( bStart ){ // Start
		// Stop recently active seat timer. {{
		if( m_pTimerActiveSeat ){
			int nSeatRecentlyActive = m_pTimerActiveSeat->GetSeat();
			// Hide seat progress bar.
			SetChildControlVisible(nSeatRecentlyActive + CHILD_SeatProgressBar, false, bInvalidate);
			m_pTimerActiveSeat->ActivateTimer(false);
			}
		// }}

		// Show seat progress bar.
		SetChildControlVisible(nSeat + CHILD_SeatProgressBar, true, bInvalidate);
		pSeat->ActivateTimer(true);
		m_pTimerActiveSeat = pSeat; // Save timer active seat.

		// Set timer progress postion. {{
		ESChildControlSeatProgressBar*	pProgress	= GetSeatProgressControl(nSeat);
		if( pProgress ){
			int nPosMax = pSeat->GetAllSeconds();
			int nPos	= nPosMax - pSeat->GetPassedSeconds();
			pProgress->SetProgressPos(nPos, nPosMax, true);
			}
		// }}

		// Set action timer.
		SetTimer(POKER_SEAT_ACTION_TIMER, 1000, NULL);
		}
	else{ // Stop
		pSeat->ActivateTimer(false);
		m_pTimerActiveSeat = NULL;
		// Hide seat progress bar.
		SetChildControlVisible(nSeat + CHILD_SeatProgressBar, false, bInvalidate);

		// Kill action timer.
		KillTimer(POKER_SEAT_ACTION_TIMER);
		}

	if( bInvalidate )
		RedrawChildControl(ChildControls::CHILD_SeatProgressBar + nSeat);
	return true;
	}

void
PokerTableWnd::ClearPotAmounts(bool bRedraw /*= true*/){
	int nSeat		= 0;
	int nCt			= m_seats.GetCount();
	int nSumChips	= 0;

	while( nSeat < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
		if( !pSeat ){
			nSeat ++;
			continue;
			}
		int nAmountOld = pSeat->GetPotAmount();
		pSeat->SetTotalPot	(0);
		pSeat->SetPotAmount	(0);
		SetSeatChips		(nSeat, 0);
		if( bRedraw ){
			RedrawSeatChips(nSeat);
			}
		nSeat ++;
		}
	}

bool
PokerTableWnd::CollectChips(int nAnimationSec, int nMainChipsAmount){
	/*
	if( m_animationThread.IsThreadStopped() ){
		m_animationThread.StartThread((LPTHREAD_START_ROUTINE)AnimationProc, this);
		}

	m_animationThread.Lock();
	CollectChipsAnimationInfo* pInfo	= new CollectChipsAnimationInfo();
	pInfo->m_nAnimationSec				= nAnimationSec;
	pInfo->m_nMainChipsAmount			= nMainChipsAmount;
	m_listAnimations.Add(pInfo);
	m_animationThread.Unlock();
	*/

	ESEasyMotion*			pMotion		= new ESEasyMotion();
	CRect					rcClient;
	GetClientRect(rcClient);
	ESChildControlChips*	pMainChips	= GetMainChipsControl();
	CRect					rcMainChips	= pMainChips->GetClientRect();

	for(int i=0; i<m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(i);
		if( !pSeat || pSeat->GetPotAmount() <= 0 ) 
			continue;

		ESChildControlChips*	pChip		= GetChipsControl(i);
		CRect					rcControl	= pChip->GetClientRect();
		float 					fPosX		= pChip->GetLogicalPosX();
		float 					fPosY		= pChip->GetLogicalPosY();
		float 					fPosEndX	= ((rcMainChips.left + (rcMainChips.Width() - rcControl.Width())/2) / ((float)rcClient.Width()));
		float 					fPosEndY	= ((rcMainChips.top + (rcMainChips.Height() - rcControl.Height())/2) / ((float)rcClient.Height()));

		pMotion->AddMotionObject(pChip, 400, 20, ESEasyMotionType::MotionExponentialIn, ESEasyMotionType::MotionExponentialIn, fPosX, fPosY, fPosEndX, fPosEndY);
		}
	
	pMotion->SetAutoDelete(true);
	if( pMotion->StartMotion(m_hWnd, WM_COLLECT_CHIPS_FINISHED, nMainChipsAmount/*Main pot chiups amount*/) ){
		SetMainPotAmount(0, true);
		}
	else{
		delete pMotion;
		return false;
		}
	return true;
	}

bool
PokerTableWnd::MoveChipsFromPot(int nAnimationSec, CDWordArray& arrSeatChipAmounts){
	/*
	if( m_nMainPotAmount <= 0 && m_nTotalPotAmount <= 0 ) return false;

	if( m_animationThread.IsThreadStopped() ){
		m_animationThread.StartThread((LPTHREAD_START_ROUTINE)AnimationProc, this);
		}

	ChipsMovementAnimationInfo* pInfo	= new ChipsMovementAnimationInfo();
	pInfo->m_nAnimationSec				= nAnimationSec;
	memset(pInfo->m_arrMoveChipsFromPotAmounts, 0, sizeof(pInfo->m_arrMoveChipsFromPotAmounts));

	int nLoop	= 0;
	int nCt		= min(arrChipAmounts.GetCount(), m_nMaxPlayerCt);
	int nSeatCt	= 0;

	while( nLoop < nCt ){
		if( arrChipAmounts.GetAt(nLoop) <= 0 ){
			nLoop ++;
			continue;
			}
		pInfo->m_arrMoveChipsFromPotAmounts[nLoop] = arrChipAmounts.GetAt(nLoop);
		nLoop ++; nSeatCt ++;
		}

	m_animationThread.Lock();
	m_listAnimations.Add(pInfo);
	m_animationThread.Unlock();

	// Zero main pot amount.
	SetMainPotAmount(0, true);
	return true;
	*/
	
	CRect					rcClient;
	GetClientRect(rcClient);
	ESChildControlChips*	pMainChips	= GetMainChipsControl();
	CRect					rcMainChips	= pMainChips->GetClientRect();
	ESEasyMotion*			pMotion		= new ESEasyMotion();

	for(int i=0; i<m_seats.GetCount(); i++){
		int					nAmount	= arrSeatChipAmounts.GetAt(i);
		if( nAmount <= 0 ) {
			SetPotAmount(i, 0, false);
			continue;
			}

		SetPotAmount(i, nAmount, false);

		ESChildControlChips*	pChip		= GetChipsControl(i);
		if( !pChip ) continue;
		CRect					rcControl	= pChip->GetClientRect();
		float 					fPosEndX	= pChip->GetLogicalPosX();
		float 					fPosEndY	= pChip->GetLogicalPosY();
		float 					fPosX		= ((rcMainChips.left + (rcMainChips.Width() - rcControl.Width())/2) / ((float)rcClient.Width()));
		float 					fPosY		= ((rcMainChips.top + (rcMainChips.Height() - rcControl.Height())/2) / ((float)rcClient.Height()));

		pMotion->AddMotionObject(pChip, 400, 20, ESEasyMotionType::MotionQuadraticOut, ESEasyMotionType::MotionQuadraticOut, fPosX, fPosY, fPosEndX, fPosEndY);
		}
	
	pMotion->SetAutoDelete(true);
	if( pMotion->StartMotion(m_hWnd, WM_MOVE_CHIPS_FROM_POT_FINISHED) ){
		SetMainPotAmount(0, true);
		}
	else{
		delete pMotion;
		return false;
		}
	return true;
	}

bool
PokerTableWnd::DealSeatCardsAnimation(PokerHandStartedPacket* p){
	if( !p || !p->m_handCards.GetCount() ) return false;

	if( m_animationThread.IsThreadStopped() ){
		m_animationThread.StartThread((LPTHREAD_START_ROUTINE)AnimationProc, this);
		}

	m_animationThread.Lock();
	DealSeatCardsAnimationInfo* pInfo	= new DealSeatCardsAnimationInfo();
	pInfo->m_nAnimationSec				= 3;
	p->Copy(&pInfo->m_handStartedPacket);
	m_listAnimations.Add(pInfo);
	m_animationThread.Unlock();
	return true;
	}

bool
PokerTableWnd::DealTableCardsAnimation(PokerRoundStartedPacket* p){
	if( !p ) return false;

	/*
	if( m_animationThread.IsThreadStopped() ){
		m_animationThread.StartThread((LPTHREAD_START_ROUTINE)AnimationProc, this);
		}
	
	DealTableCardsAnimationInfo* pInfo	= new DealTableCardsAnimationInfo();
	pInfo->m_nAnimationSec				= 3;
	p->Copy(&pInfo->m_roundStartedPacket);
	m_animationThread.Lock();
	m_listAnimations.Add(pInfo);
	m_animationThread.Unlock();
	return true;*
	*/

	CRect					rcClient;
	GetClientRect(rcClient);

	float						fCX				= m_pGUI->m_szCardDimension.cx / ((float)m_pGUI->m_szDefWindowSize.cx);
	float						fCY				= m_pGUI->m_szCardDimension.cy / ((float)m_pGUI->m_szDefWindowSize.cy);

	ESChildControlTableCards*	pTableCards		= GetTableCardsControl();
	CRect						rcControl		= pTableCards->GetClientRect();
	float 						fSizeStartCX	= 0.0f, fSizeEndCX = 0.0f;
	float 						fSizeStartCY	= fCY;
	float 						fSizeEndCY		= fCY;

	switch( p->m_cRoundType ){
		case PokerRoundType::Flop:{
			fSizeStartCX	= 1.1*fCX;
			fSizeEndCX		= ((m_pGUI->m_ptTableCard3.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
			break;
			}
		case PokerRoundType::Turn:{
			fSizeStartCX	= ((m_pGUI->m_ptTableCard3.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
			fSizeEndCX		= ((m_pGUI->m_ptTableCard4.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
			break;
			}
		case PokerRoundType::River:{
			fSizeStartCX	= ((m_pGUI->m_ptTableCard4.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
			fSizeEndCX		= ((m_pGUI->m_ptTableCard5.x + m_pGUI->m_szCardDimension.cx) - m_pGUI->m_ptTableCard1.x) / ((float)m_pGUI->m_szDefWindowSize.cx);
			break;
			}
		}

	ESEasyMotion*	pMotion	= new ESEasyMotion();
	pMotion->SetAutoDelete(true);

	pTableCards->SetCard1(((char)p->m_btTableCard1));
	pTableCards->SetCard2(((char)p->m_btTableCard2));
	pTableCards->SetCard3(((char)p->m_btTableCard3));
	pTableCards->SetCard4(((char)p->m_btTableCard4));
	pTableCards->SetCard5(((char)p->m_btTableCard5));

	pMotion->AddMotionObject(pTableCards, 400, 20, ESEasyMotionType::MotionQuadraticInOut, ESEasyMotionType::MotionQuadraticOut, fSizeStartCX, fSizeStartCY, fSizeEndCX, fSizeEndCY);
	if( !pMotion->StartMotion(m_hWnd, WM_TABLE_CARDS_DEAL_FINISHED) ){
		pTableCards->Redraw();
		delete pMotion;
		return false;
		}
	return true;
	}

void
PokerTableWnd::ClearSeatCards(bool bRedraw){
	int nSeat		= 0;
	int nCt			= m_seats.GetCount();
	int nSumChips	= 0;

	while( nSeat < nCt ){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
		if( !pSeat ){
			nSeat ++;
			continue;
			}
		SetSeatCards(nSeat, -1, -1, -1, -1, bRedraw);
		nSeat ++;
		}
	}

void
PokerTableWnd::ClearTableCards(bool bRedraw){
	SetTableCards(-1, -1, -1, -1, -1, bRedraw);
	}

bool
PokerTableWnd::IsSeatTimerActive(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) return false;
	return pSeat->IsTimerActive();
	}

bool
PokerTableWnd::SetActiveSeat(int nSeatIndex, bool bActivateTimer /*= false*/, bool bInvalidate){
	int nActiveSeatOld	= m_nActiveSeat;
	m_nActiveSeat		= nSeatIndex;

	ESChildControlSeat* pSeatCtrl = GetSeatControl(nSeatIndex);
	if( pSeatCtrl ){
		pSeatCtrl->SetSeatActive(true, bInvalidate);
		}

	if( nSeatIndex != nActiveSeatOld ){
		ESChildControlSeat* pSeatCtrlOld = GetSeatControl(nActiveSeatOld);
		if( pSeatCtrlOld )
			pSeatCtrlOld->SetSeatActive(false, bInvalidate);

		// Stop action timer on old seat. {{
		if( IsSeatTimerActive(nActiveSeatOld) )
			StartActionTimer(nActiveSeatOld, false, bInvalidate);
		// }}
		}
	
	if( bActivateTimer )
		StartActionTimer(nSeatIndex, bInvalidate);
	return true;
	}

SeatPanelInfo*
PokerTableWnd::GetSeatPanelInfo(int nSeat){
	return m_seatPanelInfos->GetAt(nSeat);
	}

bool
PokerTableWnd::AddActionButton(ActionButton type, CString sLabel, int nAmountMin, int nAmountMax, bool bInvalidate /*= true*/){
	ActionButtonInfo* pNew	= new ActionButtonInfo();
	pNew->nAmountMin		= nAmountMin;
	pNew->nAmount			= min(nAmountMin, nAmountMax);
	pNew->nAmountMax		= nAmountMax;
	pNew->sLabel			= sLabel;
	pNew->type				= type;
	m_listActionButtons.Add(pNew);
	int	nActionButtonCt		= m_listActionButtons.GetCount();

	ESChildControl*			pActionPanel	= GetChildControl(CHILD_PokerActionPanel);
	ESChildControlButton*	pActionButton	= new ESChildControlButton();
	
	CRect rcButton;
	rcButton.left		= (m_pGUI->m_ptActionButton.x) + (m_pGUI->m_szActionButton.cx + 5)*(nActionButtonCt-1);
	rcButton.top		= m_pGUI->m_ptActionButton.y;
	rcButton.right		= rcButton.left + m_pGUI->m_szActionButton.cx;
	rcButton.bottom		= rcButton.top + m_pGUI->m_szActionButton.cy;

	int nId				= CHILD_ActionButton + (nActionButtonCt - 1);
	pNew->nButtonId		= nId;
	if( AddChildControlEx(pActionButton, nId, rcButton, pActionPanel, m_pGUI->m_szDefWindowSize) ){
		pActionButton->SetText(sLabel, bInvalidate);
		return true;
		}
	else
		delete pActionButton;
	return false;
	}

bool
PokerTableWnd::IsActionButtonVisible(ActionButton type){
	int nCt		= m_listActionButtons.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
		if( pInfo && pInfo->type == type ){
			ESChildControl*	pActionButton = GetChildControl(CHILD_ActionButton + nLoop);
			if( pActionButton ){
				return pActionButton->IsVisible();
				}
			return false;
			}
		nLoop ++;
		}
	return false;
	}

ActionButtonInfo*	
PokerTableWnd::GetActionButtonInfo(ActionButton type){
	int nCt		= m_listActionButtons.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
		if( pInfo && pInfo->type == type ){
			return pInfo;
			}
		nLoop ++;
		}
	return NULL;
	}

bool
PokerTableWnd::ClearActionButtons(bool bInvalidate /*= true*/){
	int nCt		= m_listActionButtons.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		ESChildControl*	pActionButton = GetChildControl(CHILD_ActionButton + nLoop);
		if( pActionButton ){
			if( bInvalidate ){
				pActionButton->SetVisible(false, bInvalidate);
				}
			RemoveChildControl(pActionButton);
			}
		nLoop ++;
		}
	m_listActionButtons.RemoveAll();
	return true;
	}

bool
PokerTableWnd::RedrawActionButtons(){
	int nCt = m_listActionButtons.GetCount();
	if( !nCt )
		return false;
	
	int nLoop = 0;
	while( nLoop < nCt ){
		ESChildControl*	pActionButton = GetChildControl(CHILD_ActionButton + nLoop);
		if( pActionButton )
			pActionButton->Redraw();
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::SetActionButtonInfo(int nIndex, CString sLabel, int nAmount, bool bInvalidate){
	ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nIndex);
	if( !pInfo )
		return false;

	pInfo->sLabel	= sLabel;
	pInfo->nAmount	= nAmount;

	ESChildControlButton*	pActionButton = (ESChildControlButton*)GetChildControl(CHILD_ActionButton + nIndex);
	if( pActionButton )
		pActionButton->SetText(sLabel, false);

	if( bInvalidate )
		RedrawActionButton(nIndex);
	return true;
	}

bool
PokerTableWnd::RedrawActionButton(int nIndex){
	ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nIndex);
	if( !pInfo )
		return false;
	return RedrawChildControl(CHILD_ActionButton + nIndex);
	}

bool
PokerTableWnd::ClearActionLabels(bool bRedraw){
	int nLoop	= 0;
	int nCt		= m_nMaxPlayerCt;

	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nLoop);
		if( pSeatInfo ){
			pSeatInfo->SetActionName(_T(""));
			pSeatInfo->SetLastAction(PokerActionTypes::NoAction);
			if( bRedraw ){
				RedrawSeatActionLabel(nLoop);
				}
			}
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::SetSeatActionLabel(int nSeat, CString sActionName, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo )
		return false;
	pSeatInfo->SetActionName(sActionName);
	if( bInvalidate )
		RedrawSeatActionLabel(nSeat);
	return true;
	}

bool
PokerTableWnd::RedrawSeatActionLabel(int nSeat){
	PokerTableSeatInfo* pInfo = GetSeatInfo(nSeat);
	if( !pInfo )
		return false;

	ESChildControlSeat* pSeat = GetSeatControl(nSeat);
	if( pSeat )
		pSeat->RedrawActionLabel();
	return (pSeat != NULL);
	}

bool
PokerTableWnd::SetSeatStatus(int nSeat, PokerSeatStatus status, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) 
		return false;
	pSeat->SetStatus(status);
	if( bInvalidate ){
		RedrawSeat(nSeat);
		}
	return true;
	}

PokerSeatStatus	
PokerTableWnd::GetSeatStatus(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) 
		return PokerSeatStatus::Empty;
	return pSeat->GetStatus();
	}

void
PokerTableWnd::SetSeatOffline(int nSeat, bool bOnline /*= false*/, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) 
		return;

	pSeat->SetOffline(!bOnline);
	if( bInvalidate )
		RedrawSeat(nSeat);
	}

bool
PokerTableWnd::GetSeatOffline(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) 
		return false;
	return pSeat->IsOffline();
	}

CString
PokerTableWnd::GetSeatUserName(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) 
		return _T("");
	return pSeat->GetUserName();
	}

LRESULT
PokerTableWnd::OnSeatClick(WPARAM wParam, LPARAM lParam){
	int nSeat = (int)wParam - CHILD_Seat;
	if( nSeat < 0 || nSeat >= m_seats.GetCount() )
		return 0L;
	if( m_pHandler )
		m_pHandler->OnSeatPanelClick(nSeat, GetSeatInfo(nSeat));
	return 0L;
	}

LRESULT
PokerTableWnd::OnFlatButtonClick(WPARAM wParam, LPARAM lParam){
	int nId = (int)wParam;
	if( !m_pHandler ) return 0L;
	switch( nId ){
		case CHILD_Button_LeaveTable:{
			m_pHandler->OnLeaveTableClicked();
			break;
			}
		case CHILD_Button_ViewLobby:{
			m_pHandler->OnViewLobbyClicked();
			break;
			}
		case CHILD_Button_BuyChips:{
			m_pHandler->OnBuyChipsClicked();
			break;
			}
		case CHILD_Button_History:{
			m_pHandler->OnHistoryClicked();
			break;
			}
		};
	return 0L;
	}

LRESULT	
PokerTableWnd::OnActionButtonClick(WPARAM wParam, LPARAM lParam){
	int nId = (int)wParam;
	if( nId == CHILD_AddChipsButton ){
		ESChildControlAddChips* pAddChips	= (ESChildControlAddChips*)GetChildControl(CHILD_AddChipsControl);
		if( pAddChips ){
		int nAmount = pAddChips->GetSliderPos();
		if( m_pHandler ){
			m_pHandler->OnAddChipsButtonClicked(nAmount);
			}
		}
		return 0L;
		}

	nId = (int)wParam - CHILD_ActionButton;
	if( nId >= m_listActionButtons.GetCount() )
		return 0L;

	ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nId);
	if( !pInfo )
		return 0L;

	if( m_pHandler )
		m_pHandler->OnActionButtonClicked(pInfo);
	return 0L;
	}

LRESULT	
PokerTableWnd::OnShortcutButtonClick(WPARAM wParam, LPARAM lParam){
	int nId = (int)wParam - CHILD_ShortcutButton;
	if( nId >= m_listShortcutButtons.GetCount() )
		return 0L;

	ShortcutButtonInfo* pInfo = m_listShortcutButtons.GetAt(nId);
	if( !pInfo )
		return 0L;

	int nAmount = pInfo->nAmount;
	if( nAmount == -1 ){ // All-in shortcut button
		ESChildControlSlider* pSlider = (ESChildControlSlider*)GetChildControl(CHILD_Slider);
		if( !pSlider ) return 0L;
		nAmount = pSlider->GetSliderPosMax();
		}
		
	if( SetSliderAmount(nAmount, true) ){
		// Set raise/bet button info. {{
		CString sLabel;
		if( m_currentRound == PokerRoundType::PreFlop )
			sLabel.Format(_T("Raise %d.%02d"), nAmount/100, nAmount%100);
		else
			sLabel.Format(_T("Bet %d.%02d"), nAmount/100, nAmount%100);
		SetActionButtonInfo(2, sLabel, nAmount, true);
		// }}
		}
	return 0L;
	}

LRESULT
PokerTableWnd::OnSliderMoneyChanged(WPARAM wParam, LPARAM lParam){
	int nId = (int)lParam;
	int nAmount = (int)wParam;
	if( nId == CHILD_SliderMoneyEdit ){
		ESChildControlSlider* pSlider = (ESChildControlSlider*)GetChildControl(CHILD_Slider);
		if( !pSlider )
			return 0L;

		if( nAmount == pSlider->GetSliderPos() )
			return 1;

		int nMin = pSlider->GetSliderPosMin();
		int nMax = pSlider->GetSliderPosMax();
		/*
		if( nAmount == 0 && nMin < 100 ){
			return 1; // Valid while typing.
			}

		if( (nAmount > nMax || nAmount < nMin) )
			return 0L;*/

		if( nAmount > nMax ){
			nAmount = nMax;
			}

		if( nAmount < nMin ){
			nAmount = nMin;
			}

		// Set slider pos.
		pSlider->SetSliderPos(nAmount, true, false);
		// Set raise/bet button info. {{
		CString sLabel;
		if( m_currentRound == PokerRoundType::PreFlop ){
			sLabel.Format(_T("%s %d.%02d"),PokerGUI::GetLangValue(_T("BUTTON_LABEL_RAISE")), nAmount/100, nAmount%100);
			//sLabel.Format(_T("Raise %d.%02d"), nAmount/100, nAmount%100);
			}
		else{
			sLabel.Format(_T("%s %d.%02d"), PokerGUI::GetLangValue(_T("BUTTON_LABEL_BET")), nAmount/100, nAmount%100);
			//sLabel.Format(_T("Bet %d.%02d"), nAmount/100, nAmount%100);
			}
		SetActionButtonInfo(2, sLabel, nAmount, true);
		// }}
		}
	else
	if( nId == CHILD_AddChipsMoneyEdit ){
		ESChildControlAddChips*		pAddChips	= (ESChildControlAddChips*)GetChildControl(CHILD_AddChipsControl);
		if( !pAddChips ) return 0L;

		if( nAmount == pAddChips->GetSliderPos() )
			return 1;

		int nMin = pAddChips->GetSliderPosMin();
		int nMax = pAddChips->GetSliderPosMax();
		if( nAmount == 0 && nMin < 100 ){
			return 1; // Valid while typing.
			}

		if( (nAmount > nMax || nAmount < nMin) )
			return 0L;
		pAddChips->SetSliderPos(nAmount, true, false);
		}
	return 1; // Success
	}

LRESULT
PokerTableWnd::OnSliderPosChanged(WPARAM wParam, LPARAM lParam){
	ESChildControlSlider* pSlider = (ESChildControlSlider*)lParam;
	if( !pSlider ) return 0L;
	int nAmount = (int)wParam;

	if( pSlider->GetId() == CHILD_Slider ){
		ESChildControlMoneyEdit* pSliderMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_SliderMoneyEdit);
		if( pSliderMoneyEdit ){
			pSliderMoneyEdit->SetAmount(nAmount);
			}

		// Set raise button info. {{
		CString sLabel;
		if( m_currentRound == PokerRoundType::PreFlop ){
			sLabel.Format(_T("%s %d.%02d"),PokerGUI::GetLangValue(_T("BUTTON_LABEL_RAISE")), nAmount/100, nAmount%100);
			//sLabel.Format(_T("Raise %d.%02d"), nAmount/100, nAmount%100);
			}
		else{
			sLabel.Format(_T("%s %d.%02d"), PokerGUI::GetLangValue(_T("BUTTON_LABEL_BET")), nAmount/100, nAmount%100);
			//sLabel.Format(_T("Bet %d.%02d"), nAmount/100, nAmount%100);
			}
		SetActionButtonInfo(2, sLabel, nAmount, true);
		// }}
		}
	else
	if( pSlider->GetId() == CHILD_AddChipsControl ){
		ESChildControlAddChips*		pAddChips	= (ESChildControlAddChips*)lParam;
		ESChildControlMoneyEdit*	pMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_AddChipsMoneyEdit);
		if( pMoneyEdit ){
			pMoneyEdit->SetAmount(nAmount);
			}
		}
	return 0L;
	}

LRESULT
PokerTableWnd::OnPreActionChecked(WPARAM wParam, LPARAM lParam){
	int							nId			= wParam;
	ESChildControlPreAction*	pPreAction	= (ESChildControlPreAction*)lParam;
	if( !pPreAction )			return 0L;
	bool						bCheckState = pPreAction->GetCheck();
	PreActionButtonInfo*		pInfo		= GetPreActionInfo(nId);
	if( pInfo && m_pHandler ){
		m_pHandler->OnPreActionButtonChecked(pInfo, bCheckState);
		}
	return 0L;
	}

LRESULT
PokerTableWnd::OnAddChipsClose(WPARAM wParam, LPARAM lParam){
	if( m_pHandler ){
		m_pHandler->OnAddChipsControlClose();
		}
	return 0L;
	}

void
PokerTableWnd::AddFlatButton(int nIndex, int nId, CString sTitle, bool bRedraw){
	ESChildControlFlatButton* pFlatButton = (ESChildControlFlatButton*)GetChildControl(nId);
	if( pFlatButton ) return;

	pFlatButton = new ESChildControlFlatButton();
	pFlatButton->SetText(sTitle);

	CRect rcFlatButton;
	if( nIndex < 2 ){
		rcFlatButton.left	= 0;
		rcFlatButton.top	= 4 + nIndex*(m_pGUI->m_szPreAction.cy + 4);
		rcFlatButton.right	= (rcFlatButton.left + m_pGUI->m_szPreAction.cx);
		rcFlatButton.bottom = (rcFlatButton.top + m_pGUI->m_szPreAction.cy);
		}
	else{
		nIndex -= 2;
		rcFlatButton.left	= (m_pGUI->m_szDefWindowSize.cx - m_pGUI->m_szPreAction.cx);
		rcFlatButton.top	= 4 + nIndex*(m_pGUI->m_szPreAction.cy + 4);
		rcFlatButton.right	= (rcFlatButton.left + m_pGUI->m_szPreAction.cx);
		rcFlatButton.bottom = (rcFlatButton.top + m_pGUI->m_szPreAction.cy);
		}

	AddChildControlEx(pFlatButton, nId, rcFlatButton, NULL, m_pGUI->m_szDefWindowSize);
//	pFlatButton->SetZIndex(1); // Must be on the top.
	if( bRedraw ){
		pFlatButton->Redraw();
		}
	}

bool
PokerTableWnd::AddShortcutButton(CString sLabel, int nAmount, bool bInvalidate /*= true*/){
	ShortcutButtonInfo* pInfo = new ShortcutButtonInfo();
	pInfo->sLabel		= sLabel;
	pInfo->nAmount		= nAmount;
	pInfo->nButtonId	= m_listShortcutButtons.GetCount();
	m_listShortcutButtons.Add(pInfo);

	ESChildControl*				pActionPanel	= GetChildControl(CHILD_PokerActionPanel);
	ESChildControlShortcutBtn*	pShortcutButton	= new ESChildControlShortcutBtn();
	
	CRect rcButton;
	rcButton.left			= (m_pGUI->m_ptShortcutButton.x) + (m_pGUI->m_szShortcutButton.cx + 2)*pInfo->nButtonId;
	rcButton.top			= m_pGUI->m_ptShortcutButton.y;
	rcButton.right			= rcButton.left + m_pGUI->m_szShortcutButton.cx;
	rcButton.bottom			= rcButton.top + m_pGUI->m_szShortcutButton.cy;

	pInfo->nButtonId		+= CHILD_ShortcutButton;
	if( AddChildControlEx(pShortcutButton, pInfo->nButtonId, rcButton, pActionPanel, m_pGUI->m_szDefWindowSize) ){
		pShortcutButton->SetText		(sLabel, bInvalidate);
		return true;
		}
	return false;
	} 

bool
PokerTableWnd::ClearShortcutButtons(bool bInvalidate /*= true*/){
	int nCt		= m_listShortcutButtons.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		ESChildControl*	pShortcutButton = GetChildControl(CHILD_ShortcutButton + nLoop);
		if( pShortcutButton ){
			if( bInvalidate )
				pShortcutButton->SetVisible(false, bInvalidate);
			RemoveChildControl(pShortcutButton);
			}
		nLoop ++;
		}
	m_listShortcutButtons.RemoveAll();
	return true;
	}

bool
PokerTableWnd::RedrawShortcutButtons(){
	int nCt		= m_listShortcutButtons.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		ESChildControl*	pShortcutButton = GetChildControl(CHILD_ShortcutButton + nLoop);
		if( pShortcutButton ){
			pShortcutButton->Redraw();
			}
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::SetSliderAmount(int nAmount, bool bInvalidate /*= true*/){
	ESChildControlSlider*	pSlider			= (ESChildControlSlider*)GetChildControl(CHILD_Slider);
	if( !pSlider )
		return false;

	ESChildControlMoneyEdit* pSliderMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_SliderMoneyEdit);
	if( pSliderMoneyEdit ){
		pSliderMoneyEdit->SetAmount	(nAmount);
		}
	return pSlider->SetSliderPos(nAmount, bInvalidate);
	}

bool
PokerTableWnd::ShowAddChipsControl(int nAmount, int nAmountMin, int nAmountMax, int nAmountStep, int nBalance, bool bInvalidate /*= true*/){
	ESChildControl*			pActionPanel	= GetChildControl(CHILD_PokerActionPanel);
	ESChildControlAddChips* pAddChips		= (ESChildControlAddChips*)GetChildControl(CHILD_AddChipsControl);

	if( !pAddChips ){
		pAddChips = new ESChildControlAddChips();
		AddChildControlEx(pAddChips, CHILD_AddChipsControl, m_pGUI->m_rcAddChips, pActionPanel, m_pGUI->m_szDefWindowSize); 
		}

	// Initialize money edit.
	ESChildControlMoneyEdit* pMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_AddChipsMoneyEdit);
	if( pMoneyEdit ){
		pMoneyEdit->SetVisible	(true, false);
		pMoneyEdit->Setup		(nAmount, nAmountMin, min(nBalance,nAmountMax));
		}

	// Hide some child controls.
	HideActionButtons	(false, true);
	HidePreActionButtons(false, true);
	HideShortcutButtons	(false, true);

	// Hide slider.
	HideSlider(true);

	// Initialize add chips control.
	pAddChips->Setup		(nAmountMin, min(nBalance,nAmountMax), nAmountStep, nBalance, false);
	pAddChips->SetSliderPos	(nAmount, false);
	pAddChips->SetVisible	(true, false);

	CEdit* pEdit	= pMoneyEdit->GetEditControl();
	if( pEdit ){
		pEdit->SetFocus	();
		pEdit->SetSel	(0, -1);
		}

	if( bInvalidate )
		pAddChips->Redraw();
	return true;
	}

int
PokerTableWnd::GetAddChipsAmount(){
	ESChildControlAddChips* pAddChips = (ESChildControlAddChips*)GetChildControl(CHILD_AddChipsControl);
	if( !pAddChips )
		return 0;
	return pAddChips->GetSliderPos();
	}

bool
PokerTableWnd::IsAddChipsControlVisible(){
	ESChildControl*		pAddChips	= GetChildControl(CHILD_AddChipsControl);
	if( !pAddChips ) return false;
	return pAddChips->IsVisible();
	}

bool
PokerTableWnd::HideAddChipsControl(bool bInvalidate /*= true*/){
	ESChildControl*		pAddChips	= GetChildControl(CHILD_AddChipsControl);
	if( !pAddChips ) return false;
	pAddChips->SetVisible(false, bInvalidate);

	ESChildControlMoneyEdit* pMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_AddChipsMoneyEdit);
	if( pMoneyEdit ){
		pMoneyEdit->SetVisible(false, true);
		}
	SetFocus();

	// Show slider control.
	if( m_listActionButtons.GetCount() > 0 ){
		int nLoop = 0;
		int nCt = m_listActionButtons.GetCount();

		while( nLoop < nCt ){
			ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
			if( pInfo ){
				if( pInfo->type == ActionButton::ButtonRaise ||  
					pInfo->type == ActionButton::ButtonBet ){
					if( pInfo->nAmountMin < pInfo->nAmountMax ){
						ESChildControlSlider*	pSlider	= (ESChildControlSlider*)GetChildControl(CHILD_Slider);
						if( pSlider ){
							ShowSlider(pInfo->nAmount, pInfo->nAmountMin, pInfo->nAmountMax, pSlider->GetSliderStep(), true);
							}
						break;
						}
					}
				}
			nLoop ++;
			}
		}

	// Show some child controls.
	HideActionButtons	(true, true);
	HidePreActionButtons(true, true);
	HideShortcutButtons	(true, true);
	return true;
	}

bool
PokerTableWnd::ShowSlider(int nAmount, int nAmountMin, int nAmountMax, int nAmountStep, bool bInvalidate /*= true*/){
	ESChildControl*			pActionPanel	= GetChildControl(CHILD_PokerActionPanel);
	ESChildControlSlider*	pSlider			= (ESChildControlSlider*)GetChildControl(CHILD_Slider);
	if( !pSlider ){
		pSlider					= new ESChildControlSlider();
		CRect rcSlider;
		rcSlider.left			= m_pGUI->m_ptSlider.x;
		rcSlider.top			= m_pGUI->m_ptSlider.y;
		rcSlider.right			= rcSlider.left + m_pGUI->m_szSlider.cx;
		rcSlider.bottom			= rcSlider.top + m_pGUI->m_szSlider.cy;
		AddChildControlEx(pSlider, CHILD_Slider, rcSlider, pActionPanel, m_pGUI->m_szDefWindowSize); 

		// Add money edit.
		CRect					rcSliderMoneyEdit	= m_pGUI->m_rcSliderMoneyEdit;
		ESChildControlMoneyEdit* pSliderMoneyEdit	= new ESChildControlMoneyEdit();
		AddChildControlEx(pSliderMoneyEdit, CHILD_SliderMoneyEdit, rcSliderMoneyEdit, pActionPanel, m_pGUI->m_szDefWindowSize); 
		}

	ESChildControlMoneyEdit* pSliderMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_SliderMoneyEdit);
	if( pSliderMoneyEdit ){
		pSliderMoneyEdit->SetVisible(true, false);
		pSliderMoneyEdit->Setup		(nAmount, nAmountMin, nAmountMax);
		CEdit* pEdit	= pSliderMoneyEdit->GetEditControl();
		if( pEdit ){
			pEdit->SetFocus	();
			pEdit->SetSel	(0, -1);
			}
		}

	pSlider->SetupSlider	(nAmountMin, nAmountMax, nAmountStep, false);
	pSlider->SetSliderPos	(nAmount, false);
	pSlider->SetVisible		(true, false);
	if( bInvalidate )
		pSlider->Redraw();
	return true;
	}

bool
PokerTableWnd::HideSlider(bool bInvalidate /*= true*/){
	ESChildControl*		pSlider	= GetChildControl(CHILD_Slider);
	if( !pSlider ) return false;
	pSlider->SetVisible(false, bInvalidate);

	ESChildControlMoneyEdit* pSliderMoneyEdit	= (ESChildControlMoneyEdit*)GetChildControl(CHILD_SliderMoneyEdit);
	if( pSliderMoneyEdit ){
		pSliderMoneyEdit->SetVisible(false, true);
		}

	SetFocus();
	return true;
	}

bool
PokerTableWnd::IsActionSliderVisible(){
	ESChildControl*		pSlider	= GetChildControl(CHILD_Slider);
	if( !pSlider ) return false;
	return pSlider->IsVisible();
	}

bool
PokerTableWnd::AddPreActionButton(PreActionButton preAction, int nValue, CString sLabel, bool bCheckState, bool bInvalidate, bool bCalcSize /*= false*/){
	PokerGUI* pGUI = PokerGUI::GetInstance();
	CPoint pos;
	if( !pGUI->GetPreActionPosition(preAction, pos) )
		return false;

	CRect rcClient;
	GetClientRect(rcClient);
	float fStretchCX = ((float)rcClient.Width()) / pGUI->m_szDefWindowSize.cx;
	float fStretchCY = ((float)rcClient.Height()) / pGUI->m_szDefWindowSize.cy;

	CPoint ptOrig = pos;
	ptOrig.x *= fStretchCX;
	ptOrig.y *= fStretchCY;

	ESChildControl*	pChildExisted	= GetChildControl(CHILD_PreAction + (int)preAction);
	if( pChildExisted ){
		int nId		= pChildExisted->GetId();
		int nLoop	= 0;
		int nCt		= m_listPreActions.GetCount();

		while( nLoop < nCt ){
			PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
			if( pInfo && pInfo->nButtonId == nId ){
				pInfo->nValue = nValue;
				pInfo->sLabel = sLabel;
				break;
				}
			nLoop ++;
			}

		pChildExisted->SetVisible(true, false);
		((ESChildControlPreAction*)pChildExisted)->SetCheck(bCheckState, false);
		((ESChildControlPreAction*)pChildExisted)->SetText(sLabel, bInvalidate);
		return true;
		}

	ESChildControl*				pActionPanel	= GetChildControl(CHILD_PokerActionPanel);
	ESChildControlPreAction*	pPreAction		= new ESChildControlPreAction();
	ESChildControl*				pParent			= GetChildControl(CHILD_PokerTable);

	PreActionButtonInfo* pInfo = new PreActionButtonInfo();
	pInfo->sLabel		= sLabel;
	pInfo->type			= preAction;
	pInfo->nButtonId	= CHILD_PreAction + (int)preAction;
	pInfo->ptPos		= ptOrig;
	pInfo->nValue		= nValue;	
	m_listPreActions.Add(pInfo);

	CRect rcPreAction;
	rcPreAction.left		= pos.x;
	rcPreAction.top			= pos.y;
	rcPreAction.right		= rcPreAction.left + m_pGUI->m_szPreAction.cx;
	rcPreAction.bottom		= rcPreAction.top + m_pGUI->m_szPreAction.cy;

	if( bCalcSize ){
		CSize szText;
		ImageHelper::GetTextSize(&sLabel, m_pGUI->m_pPreActionFont, szText);
		rcPreAction.right		= rcPreAction.left + szText.cx;
		rcPreAction.bottom		= rcPreAction.top + szText.cy;
		}

	if( pActionPanel->GetClientRect().PtInRect(ptOrig) )
		pParent = pActionPanel;

	if( AddChildControlEx(pPreAction, pInfo->nButtonId, rcPreAction, pParent, m_pGUI->m_szDefWindowSize) ){
//		pPreAction->SetZIndex		(1);
		pPreAction->SetCheck		(bCheckState, false);
		pPreAction->SetText			(sLabel, false);
		pPreAction->SetVisible		(true, bInvalidate);
		return true;
		}
	return false;
	}

bool
PokerTableWnd::RemovePreActionButton(PreActionButton preAction, bool bInvalidate){
	ESChildControl*	 pPreAction	= GetChildControl(CHILD_PreAction + (int)preAction);
	if( !pPreAction ){
		return false;
		}

	int nId		= pPreAction->GetId();
	int nLoop	= 0;
	int nCt		= m_listPreActions.GetCount();

	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo && pInfo->nButtonId == nId ){
			m_listPreActions.RemoveAt(nLoop);
			break;
			}
		nLoop ++;
		}

	pPreAction->SetVisible(false, bInvalidate);
	RemoveChildControl(pPreAction);
	return true;
	}

bool
PokerTableWnd::SetPreActionButtonLabel(PreActionButton preAction, CString sLabel, bool bInvalidate /*= true*/){
	ESChildControlPreAction*	 pPreAction	= (ESChildControlPreAction*)GetChildControl(CHILD_PreAction + (int)preAction);
	if( !pPreAction )
		return false;
	int						nId		= pPreAction->GetId();
	PreActionButtonInfo*	pInfo	= GetPreActionInfo(nId);
	if( !pInfo )
		return false;

	pInfo->sLabel = sLabel;
	pPreAction->SetText(sLabel, bInvalidate);
	return true;
	}

bool
PokerTableWnd::SetPreActionCheckState(PreActionButton preAction, bool bCheckState, bool bInvalidate /*= true*/){
	ESChildControlPreAction*	 pPreAction	= (ESChildControlPreAction*)GetChildControl(CHILD_PreAction + (int)preAction);
	if( !pPreAction ){
		return false;
		}
	pPreAction->SetCheck(bCheckState, bInvalidate);
	return true;
	}

bool
PokerTableWnd::GetPreActionCheckState(PreActionButton preAction){
	ESChildControlPreAction*	 pPreAction	= (ESChildControlPreAction*)GetChildControl(CHILD_PreAction + (int)preAction);
	if( !pPreAction )
		return false;
	return pPreAction->GetCheck();
	}

bool
PokerTableWnd::ClearPreActions(bool bInvalidate /*= true*/){
	int nLoop	= 0;
	int nCt		= m_listPreActions.GetCount();

	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo ){
			ESChildControlPreAction*	 pPreAction	= (ESChildControlPreAction*)GetChildControl(pInfo->nButtonId);
			if( pPreAction ){
				pPreAction->SetVisible(false, bInvalidate);
				RemoveChildControl(pPreAction);
				}
			}
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::RedrawPreActions(){
	int nLoop	= 0;
	int nCt		= m_listPreActions.GetCount();

	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo ){
			ESChildControlPreAction* pPreAction	= (ESChildControlPreAction*)GetChildControl(pInfo->nButtonId);
			if( pPreAction ){
				pPreAction->Redraw();
				}
			}
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::RedrawPreActionButton(PreActionButton preAction){
	ESChildControlPreAction* pPreAction	= (ESChildControlPreAction*)GetChildControl(CHILD_PreAction + (int)preAction);
	if( pPreAction ){
		pPreAction->Redraw();
		return true;
		}
	return false;
	}

PreActionButtonInfo*
PokerTableWnd::GetPreActionInfo(int nId){
	int nLoop	= 0;
	int nCt		= m_listPreActions.GetCount();

	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo && pInfo->nButtonId == nId ){
			return pInfo;
			}
		nLoop ++;
		}
	return NULL;
	}

ESChildControlSeat*
PokerTableWnd::GetSeatControl(int nSeat){
	int					nId			= ChildControls::CHILD_Seat + nSeat;
	ESChildControlSeat* pControl	= (ESChildControlSeat*)GetChildControl(nId);
	return pControl;
	}

ESChildControlSeatCards*
PokerTableWnd::GetSeatCardsControl(int nSeat){
	int							nId			= ChildControls::CHILD_SeatCards + nSeat;
	ESChildControlSeatCards*	pControl	= (ESChildControlSeatCards*)GetChildControl(nId);
	return pControl;
	}

CRect
PokerTableWnd::GetSeatCardsRect(int nSeat){
	ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(nSeat);
	if( !pSeatCards ) return CRect(0, 0, 0, 0);
	return pSeatCards->GetClientRect();
	}

CRect
PokerTableWnd::GetSeatRect(int nSeat){
	ESChildControlSeat* pSeat = GetSeatControl(nSeat);
	if( !pSeat ) return CRect(0, 0, 0, 0);
	return pSeat->GetClientRect();
	}

ESChildControlSeatProgressBar*
PokerTableWnd::GetSeatProgressControl(int nSeat){
	int								nId			= ChildControls::CHILD_SeatProgressBar + nSeat;
	ESChildControlSeatProgressBar*	pControl	= (ESChildControlSeatProgressBar*)GetChildControl(nId);
	return pControl;
	}

ESChildControlChips*
PokerTableWnd::GetChipsControl(int nSeat){
	int					nId			= ChildControls::CHILD_SeatChips + nSeat;
	ESChildControlChips* pControl	= (ESChildControlChips*)GetChildControl(nId);
	return pControl;
	}

ESChildControlChips*
PokerTableWnd::GetMainChipsControl(){
	int					nId			= ChildControls::CHILD_SeatMainPotChips;
	ESChildControlChips* pControl	= (ESChildControlChips*)GetChildControl(nId);
	return pControl;
	}

ESChildControlDealer*
PokerTableWnd::GetSeatDealerControl	(int nSeat){
	int					nId			= ChildControls::CHILD_SeatDealer + nSeat;
	ESChildControlDealer* pControl	= (ESChildControlDealer*)GetChildControl(nId);
	return pControl;
	}

ESChildControlTableCards*
PokerTableWnd::GetTableCardsControl(){
	int							nId			= ChildControls::CHILD_PokerTableCards;
	ESChildControlTableCards*	pControl	= (ESChildControlTableCards*)GetChildControl(nId);
	return pControl;
	}

bool
PokerTableWnd::RedrawSeat(int nSeat, bool bWithCards /*= false*/){
	ESChildControlSeat*			pSeatCtrl	= GetSeatControl(nSeat);
	if( !pSeatCtrl )
		return false;

	CRect						rcSeat		= pSeatCtrl->GetClientRect();
	ESChildControlSeatCards*	pSeatCards	=  GetSeatCardsControl(nSeat);
	if( pSeatCards && bWithCards && pSeatCards->IsVisible() )
		rcSeat |= pSeatCards->GetClientRect();
	RedrawRect(rcSeat);
	return true;
	}

void
PokerTableWnd::RedrawSeats(bool bWithCards /*= false*/){
	for( int i=0; i<m_nMaxPlayerCt; i++){
		RedrawSeat(i, bWithCards);
		}
	}

bool
PokerTableWnd::RedrawSeatCards(int nSeat){
	ESChildControlSeatCards* pSeatCards	= (ESChildControlSeatCards*)GetChildControl(CHILD_SeatCards + nSeat);
	if( !pSeatCards )
		return false;
	pSeatCards->Redraw();
	return true;
	}

bool
PokerTableWnd::RedrawSeatChips(int nSeat){
	ESChildControlChips* pChipsCtrl = GetChipsControl(nSeat);
	if( !pChipsCtrl )
		return false;
	pChipsCtrl->Redraw();
	return true;
	}

bool
PokerTableWnd::RedrawChildControl(int nId){
	ESChildControl* pControl = GetChildControl(nId);
	if( !pControl )
		return false;
	pControl->Redraw();
	return true;
	}

bool
PokerTableWnd::SetChildControlVisible(int nId, bool bVisible, bool bRedraw){
	ESChildControl* pControl = GetChildControl(nId);
	if( !pControl )
		return false;
	pControl->SetVisible(bVisible, bRedraw);
	return true;
	}

bool
PokerTableWnd::SetSeatChips(int nSeat, int nAmount){
	ESChildControlChips* pChips = GetChipsControl(nSeat);
	if( !pChips )
		return false;
	pChips->SetAmount(nAmount, false);
	return true;
	}

void
PokerTableWnd::HideActionButtons(bool bShow /*= false*/, bool bRedraw /*= true*/){
	int nCt		= m_listActionButtons.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		ESChildControl*	pActionButton = GetChildControl(CHILD_ActionButton + nLoop);
		if( pActionButton ){
			pActionButton->SetVisible(bShow, bRedraw);
			}
		nLoop ++;
		}
	}

void
PokerTableWnd::HidePreActionButtons(bool bShow /*= false*/, bool bRedraw /*= true*/){
	int nCt		= m_listPreActions.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions.GetAt(nLoop);
		if( pInfo ){
			ESChildControl*	 pPreAction	= GetChildControl(CHILD_PreAction + (int)pInfo->type);
			if( pPreAction ){
				pPreAction->SetVisible(bShow, bRedraw);
				}
			}
		nLoop ++;
		}
	}

void
PokerTableWnd::HideShortcutButtons(bool bShow /*= false*/, bool bRedraw /*= true*/){
	int nCt		= m_listShortcutButtons.GetCount();
	int nLoop	= 0;

	while( nLoop < nCt ){
		ShortcutButtonInfo* pInfo = m_listShortcutButtons.GetAt(nLoop);
		if( pInfo ){
			ESChildControl*	pShortcutButton = GetChildControl(pInfo->nButtonId);
			if( pShortcutButton ){
				pShortcutButton->SetVisible(bShow, bRedraw);
				}
			}
		nLoop ++;
		}
	}

void
PokerTableWnd::ClearChatMessages(bool bRedraw){
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( pChatPage ){
		pChatPage->ClearContent(bRedraw);
		}
	}

void
PokerTableWnd::AddChatMessage(PokerChatMessage* pMessage){
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !pMessage )
		return;

	CString sFormat;
	COleDateTime dtAction = pMessage->m_dtTime;//COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	CString	sUserName	= pMessage->m_sUserName;

	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	if( pMessage->m_bSystem ){
		pChatPage->AddTextToLine(nLine, _T("UserName"), _T("SYSTEM: "), m_pGUI->m_crChatUserName, false);
		pChatPage->AddTextToLine(nLine, _T("System"), pMessage->m_sMessage, m_pGUI->m_crChatSystem, false);
		}
	else{
		if( ((PokerActionTypes)pMessage->m_actionType) != PokerActionTypes::NoAction ){
			pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);
			PokerActionTypes	acType		= (PokerActionTypes)pMessage->m_actionType;
			switch( acType){
				case PokerActionTypes::AllIn:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_ALLIN_FORMAT")), sUserName);
					//sFormat.Format(_T("%s acts AllIn"), sUserName);
					break;
					}
				case PokerActionTypes::Bet:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_BET_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s acts Bet to %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				case PokerActionTypes::Call:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_CALL_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s Calls %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				case PokerActionTypes::Check:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_CHECK_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Checks"), sUserName);
					break;
					}
				case PokerActionTypes::Fold:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_FOLD_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Folds"), sUserName);
					break;
					}
				case PokerActionTypes::Muck:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_MUCK_CARDS_FORMAT")), sUserName);
					//sFormat.Format(_T("%s muck cards"), sUserName);
					break;
					}
				case PokerActionTypes::PostBB:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_POST_BB_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Posts Big Blind"), sUserName);
					break;
					}
				case PokerActionTypes::PostSB:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_POST_SB_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Posts Small Blind"), sUserName);
					break;
					}
				case PokerActionTypes::Raise:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_RAISE_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s acts Raise to %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				case PokerActionTypes::ShowCards:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SHOW_CARDS_FORMAT")), sUserName);
					//sFormat.Format(_T("%s show cards"), sUserName);
					break;
					}
				case PokerActionTypes::SitOut:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SITOUT_FORMAT")), sUserName);
					//sFormat.Format(_T("%s sitted out"), sUserName);
					break;
					}
				case PokerActionTypes::ReturnBack:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_RETURN_BACK_FORMAT")), sUserName);
					//sFormat.Format(_T("%s returned back"), sUserName);
					break;
					}
				case PokerActionTypes::AddChips:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_ADD_CHIPS_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s added chips %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				};
			pChatPage->AddTextToLine(nLine, _T("Message"), sFormat, m_pGUI->m_crChatMessage, false);
			}
		else{
			sFormat = sUserName + _T(": ");
			pChatPage->AddTextToLine(nLine, _T("UserName"), sFormat, m_pGUI->m_crChatUserName, false);
			pChatPage->AddTextToLine(nLine, _T("Message"), pMessage->m_sMessage, m_pGUI->m_crChatMessage, false);
			}
		}

	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	}

void
PokerTableWnd::AddChatMessage_HandStart(PokerHandStartedPacket* p){
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;

	CString sFormat;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);

	CString sMessage;
	sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_HAND_STARTED_FORMAT")), p->m_nHandId);
	//sMessage.Format(_T("/********* Hand Started %d *******/"), p->m_nHandId);
	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	sMessage.Format(_T("Dealer: %d"), (int)p->m_cDealer);
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	sMessage.Format(_T("Small Blind: %d"), (int)p->m_cSmallBlind);
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	sMessage.Format(_T("Big Blind: %d"), (int)p->m_cBigBlind);
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	}

void
PokerTableWnd::AddChatMessage_HandFinish(PokerHandFinishedPacket* p){
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;
	
	CString sFormat, sMessage;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);

	sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_HAND_FINISHED_FORMAT")), p->m_nHandId);
	//sMessage.Format(_T("/************ Hand Finished %d ***********/"), p->m_nHandId);
	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);

	int		nLoop = 0;
	int		nCt		= p->m_winners.GetCount();
	while( nLoop < nCt ){
		PokerHandWinnerInfo* pWinner = p->m_winners.GetAt(nLoop);
		if( !pWinner || !pWinner->m_bWinner ){
			nLoop ++;
			continue;
			}

		sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_WON_FORMAT")), GetSeatUserName((int)pWinner->m_cSeat), pWinner->m_nWonAmount/100, pWinner->m_nWonAmount%100, m_sAmountLabelSuffix);
		//sMessage.Format(_T("%s won %d.%02d %s"), GetSeatUserName((int)pWinner->m_cSeat), pWinner->m_nWonAmount/100, pWinner->m_nWonAmount%100, m_sAmountLabelSuffix);
		pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
		nLoop ++;
		}
	sMessage = _T("/***********************/");
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	}

void
PokerTableWnd::AddChatMessage_RoundStart(PokerRoundStartedPacket* p){
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;
	
	CString sFormat, sMessage;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);


	PokerRoundType round = (PokerRoundType)p->m_cRoundType;
	switch( round ){
		case PokerRoundType::Flop:
			{	
				sMessage.Format(_T("%s: ^%s ^%s ^%s"), PokerGUI::GetLangValue(_T("CHAT_FLOP_DEALT")), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3));
/*
				sMessage.Format(_T("Flop dealt: ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3));*/
				break;
			}
		case PokerRoundType::Turn:
			{	
				sMessage.Format(_T("%s: ^%s ^%s ^%s ^%s"), PokerGUI::GetLangValue(_T("CHAT_TURN_DEALT")), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard4));
/*
				sMessage.Format(_T("Turn dealt: ^%s ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard4));*/
				break;
			}
		case PokerRoundType::River:
			{	
				sMessage.Format(_T("%s: ^%s ^%s ^%s ^%s ^%s"), PokerGUI::GetLangValue(_T("CHAT_RIVER_DEALT")), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard4), PokerGUI::CardNameByIndex((int)p->m_btTableCard5));
				/*
				sMessage.Format(_T("River dealt: ^%s ^%s ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard4), PokerGUI::CardNameByIndex((int)p->m_btTableCard5));*/
				break;
			}
		}

	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	}

void
PokerTableWnd::AddChatMessage_ShowCards(PokerActionPacket* p){
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;

	CString sFormat;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	CString	sUserName	= GetSeatUserName((int)p->m_cSeat);

	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);
	sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SHOW_CARDS_FORMAT")), sUserName);

	CDWordArray arrCards;
	for(int i=0; i<p->m_listCardsShown.GetCount(); i++){
		PokerHandCards* pCards = p->m_listCardsShown.GetAt(i);
		if( pCards ){
			arrCards.Add((DWORD)pCards->m_btCard1);
			arrCards.Add((DWORD)pCards->m_btCard2);

			if( pCards->m_btCard3 != (char)-1 )
				arrCards.Add((DWORD)pCards->m_btCard3);

			if( pCards->m_btCard4 != (char)-1 )
				arrCards.Add((DWORD)pCards->m_btCard4);
			break;
			}
		}

	if( arrCards.GetCount() == 2 ){
		CString sMessage;
		sMessage.Format(_T(": ^%s ^%s"), PokerGUI::CardNameByIndex((int)arrCards[0]), PokerGUI::CardNameByIndex((int)arrCards[1]));
		sFormat += sMessage;
		}
	else
	if( arrCards.GetCount() == 4 ){
		CString sMessage;
		sMessage.Format(_T(": ^%s ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)arrCards[0]), PokerGUI::CardNameByIndex((int)arrCards[1]),
						PokerGUI::CardNameByIndex((int)arrCards[2]), PokerGUI::CardNameByIndex((int)arrCards[3]));
		sFormat += sMessage;
		}

	pChatPage->AddTextToLine(nLine, _T("Message"), sFormat, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	}

void
PokerTableWnd::AddChatMessage_SetSeatStatus(PokerSetSeatStatusPacket* p){
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;
	
	CString sFormat, sMessage;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);

	PokerSeatStatus status = (PokerSeatStatus)p->m_status;
	switch( status ){
		case PokerSeatStatus::Sitted:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SIT_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s seated at table"), p->m_sUserName);
			break;
			}
		case PokerSeatStatus::SittedOut:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SITOUT_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s seated out"), p->m_sUserName);
			break;
			}
		case PokerSeatStatus::Reserved:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_JOIN_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s joined the table"), p->m_sUserName);
			break;
			}
		case PokerSeatStatus::Empty:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_LEAVE_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s left the table"), p->m_sUserName);
			break;
			}
		}

	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	}

int
PokerTableWnd::AddChatMessages(EnumerableObject<PokerChatMessage>* pListMessage){
	if( !pListMessage || !pListMessage->GetCount() )
		return 0;

	ClearChatMessages(false);

	int nLoop	= 0;
	int nCt		= pListMessage->GetCount();

	while( nLoop < nCt ){
		PokerChatMessage* pMsg = pListMessage->GetAt(nLoop);
		AddChatMessage(pMsg);
		nLoop ++;
		}
	return nLoop;
	}

void
PokerTableWnd::SetTableMessage(CString sMessage, bool bInvalidate){
	ESChildControlLabel* pMessageLabel = (ESChildControlLabel*)GetChildControl(CHILD_Table_Message);
	if( !pMessageLabel ) return;

	if( !pMessageLabel->IsVisible() )
		pMessageLabel->SetVisible(true, false);
	pMessageLabel->SetLabel(sMessage, bInvalidate);
	}

void
PokerTableWnd::ClearTableMessage(bool bInvalidate){
	ESChildControlLabel* pMessageLabel = (ESChildControlLabel*)GetChildControl(CHILD_Table_Message);
	if( !pMessageLabel ) return;
	if( pMessageLabel->IsVisible() ){
		pMessageLabel->SetVisible(false, bInvalidate);
		}
	}

void
PokerTableWnd::SetSeatMessage(int nSeat, CString sMessage, bool bInvalidate){
	if( nSeat < 0 || nSeat >= m_nMaxPlayerCt )
		return;

	int						nId				= CHILD_Seat_Message + nSeat;
	ESChildControlSeat*		pSeatCtrl		= GetSeatControl(nSeat);
	if( !pSeatCtrl ) return;
	ESChildControlLabel*	pMessageLabel	= (ESChildControlLabel*)GetChildControl(nId);
	CRect					rcSeat			= pSeatCtrl->GetClientRect();

	if( !pMessageLabel ){
		pMessageLabel = new ESChildControlLabel();
		// Seat message label.
		CRect rcSeatMessage;
		rcSeatMessage.left		= rcSeat.left;
		rcSeatMessage.top		= rcSeat.top - 14;
		rcSeatMessage.right		= rcSeat.right;
		rcSeatMessage.bottom	= rcSeatMessage.top + 28;

		CRect rcClient;
		GetClientRect(rcClient);

		if( AddChildControlEx(pMessageLabel, nId, rcSeatMessage, NULL, CSize(rcClient.Width(), rcClient.Height())) ){
//			pMessageLabel->SetZIndex	(2);
			pMessageLabel->SetTextColor	(m_pGUI->m_crTMessageText);
			pMessageLabel->SetFont		(m_pGUI->m_pTMessageFont);
			pMessageLabel->SetBkImage	(m_pGUI->m_pTableMessageBk, m_pGUI->m_rcTableMessageOmit);
			pMessageLabel->SetCentred	(false);
			pMessageLabel->SetVisible	(true, false);
			}
		else{
			delete pMessageLabel;
			pMessageLabel = NULL;
			return;
			}
		}
	if( !pMessageLabel->IsVisible() ){
		pMessageLabel->SetVisible(true, false);
		}
	pMessageLabel->SetLabel(sMessage, bInvalidate);
	}

void
PokerTableWnd::ClearSeatMessage(int nSeat, bool bInvalidate){
	int						nId				= CHILD_Seat_Message + nSeat;
	ESChildControlLabel*	pMessageLabel	= (ESChildControlLabel*)GetChildControl(nId);
	if( pMessageLabel ){
		if( bInvalidate && pMessageLabel->IsVisible() ){
			pMessageLabel->SetVisible(false, true);
			}
		RemoveChildControl(pMessageLabel, false);
		}
	}

void
PokerTableWnd::ClearSeatMessages(bool bInvalidate){
	int nLoop	= 0;
	int nCt		= m_nMaxPlayerCt;

	while( nLoop < nCt ){
		ClearSeatMessage(nLoop, bInvalidate);
		nLoop ++;
		}
	}

bool
PokerTableWnd::SetBestHandCards(int nSeat, PokerHandCards* pCards, bool bSetAsSeatMessage /*= true*/, 
							CString* psLabelText /*= NULL*/, CDWordArray* pArrWinnerCards /*= NULL*/){
	PokerTableSeatInfo* pSeat = m_seats.GetAt(nSeat);
	if( !pSeat || pSeat->GetCard1() < 0 || pSeat->GetCard2() < 0 || !pCards )
		return false;

	if( m_nCard1 < 0 || m_nCard2 < 0 || m_nCard3 < 0 )
		return false; // We don't have table cards.

	int nCards[] = {(int)pCards->m_btCard1, (int)pCards->m_btCard2, (int)pCards->m_btCard3, 
					(int)pCards->m_btCard4, GetTableCard1(), GetTableCard2(), GetTableCard3(),GetTableCard4(), GetTableCard5()};
		
	int nArrCards[5];
	memset(nArrCards, 0, sizeof(nArrCards)); // Fill with zero.

	if( pCards->m_cBestCard1 > -1 )
		nArrCards[0] = nCards[pCards->m_cBestCard1];
	if( pCards->m_cBestCard2 > -1 )
		nArrCards[1] = nCards[pCards->m_cBestCard2];
	if( pCards->m_cBestCard3 > -1 )
		nArrCards[2] = nCards[pCards->m_cBestCard3];
	if( pCards->m_cBestCard4 > -1 )
		nArrCards[3] = nCards[pCards->m_cBestCard4];
	if( pCards->m_cBestCard5 > -1 )
		nArrCards[4] = nCards[pCards->m_cBestCard5];

	/* For test :)
	int nArrCards[] = {2, 3, 26, 27, 29, 34, 30};
	handCards = PokerHandCardsClass::CardsClass_Flush;
	CDWordArray arr;
	pArrWinnerCards = &arr;
*/
	AutoSortedArray arrCards;

	CString sCardKind[]			= {PokerGUI::GetLangValue(_T("CARD_KIND_CROSS")), PokerGUI::GetLangValue(_T("CARD_KIND_DIAMOND")), PokerGUI::GetLangValue(_T("CARD_KIND_SPADE")), PokerGUI::GetLangValue(_T("CARD_KIND_HEART"))};
	CString sCardKind_Plural[]	= {PokerGUI::GetLangValue(_T("CARD_KIND_CROSS_PL")), PokerGUI::GetLangValue(_T("CARD_KIND_DIAMOND_PL")), PokerGUI::GetLangValue(_T("CARD_KIND_SPADE_PL")), PokerGUI::GetLangValue(_T("CARD_KIND_HEART_PL"))};
	CString sCardNames[]		= {PokerGUI::GetLangValue(_T("CARD_TWO")), PokerGUI::GetLangValue(_T("CARD_THREE")), PokerGUI::GetLangValue(_T("CARD_FOUR")), PokerGUI::GetLangValue(_T("CARD_FIVE")), PokerGUI::GetLangValue(_T("CARD_SIX")), PokerGUI::GetLangValue(_T("CARD_SEVEN")), 
								PokerGUI::GetLangValue(_T("CARD_EIGHT")), PokerGUI::GetLangValue(_T("CARD_NINE")), PokerGUI::GetLangValue(_T("CARD_TEN")), PokerGUI::GetLangValue(_T("CARD_JACK")), PokerGUI::GetLangValue(_T("CARD_QUEEN")), PokerGUI::GetLangValue(_T("CARD_KING")), PokerGUI::GetLangValue(_T("CARD_ACE"))};
	CString sCardNames_Plural[]	= {PokerGUI::GetLangValue(_T("CARD_TWO_PL")), PokerGUI::GetLangValue(_T("CARD_THREE_PL")), PokerGUI::GetLangValue(_T("CARD_FOUR_PL")), PokerGUI::GetLangValue(_T("CARD_FIVE_PL")), PokerGUI::GetLangValue(_T("CARD_SIX_PL")), PokerGUI::GetLangValue(_T("CARD_SEVEN_PL")), 
								PokerGUI::GetLangValue(_T("CARD_EIGHT_PL")), PokerGUI::GetLangValue(_T("CARD_NINE_PL")), PokerGUI::GetLangValue(_T("CARD_TEN_PL")), PokerGUI::GetLangValue(_T("CARD_JACK_PL")), PokerGUI::GetLangValue(_T("CARD_QUEEN_PL")), PokerGUI::GetLangValue(_T("CARD_KING_PL")), PokerGUI::GetLangValue(_T("CARD_ACE_PL"))};

	CString sHandCards			= _T("");
	int		nCardsCt			= sizeof(nArrCards)/sizeof(int);
	int		nCard1, nCard2;
	switch( pCards->m_cBestCardsClass ){
		case CardsClass_StraightFlush:{
			if( pArrWinnerCards ){
				pArrWinnerCards->Add(nArrCards[0]);
				pArrWinnerCards->Add(nArrCards[1]);
				pArrWinnerCards->Add(nArrCards[2]);
				pArrWinnerCards->Add(nArrCards[3]);
				pArrWinnerCards->Add(nArrCards[4]);
				}
			sHandCards = PokerGUI::GetLangValue(_T("STRAIGHT_FLUSH"));
			break;
			}
		case CardsClass_FourOfAKind:{
			sHandCards = PokerGUI::GetLangValue(_T("FOUR_OF_A_KIND"));

			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add(nArrCards[i]%13, nArrCards[i]);
				}

			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				// Found out starting point.
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 4 ){
					if( pArrWinnerCards ){
						pArrWinnerCards->Add(arrCards.GetData(i));
						pArrWinnerCards->Add(arrCards.GetData(i+1));
						pArrWinnerCards->Add(arrCards.GetData(i+2));
						pArrWinnerCards->Add(arrCards.GetData(i+3));
						}
					sHandCards.Format(_T("%s, %s"),PokerGUI::GetLangValue(_T("THREE_OF_A_KIND")), sCardNames_Plural[arrCards[i]]);
					break;
					}
				}
			break;
			}
		case CardsClass_FullHouse:{
			sHandCards = PokerGUI::GetLangValue(_T("FULL_HOUSE"));
			
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add(nArrCards[i]%13, nArrCards[i]);
				}

			// Find out pair cards.
			nCard1 = nCard2 = -1;
			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 3 ){
					nCard1 = arrCards.GetData(i);
					}
				else
				if( nCt == 2 ){
					if( (nCard2 == -1)|| (nCard2 != -1 && arrCards[i] > nCard2%13) )
						nCard2 = arrCards.GetData(i);
					}
				// Skip the same card values.
				if( nCt > 1 ){
					i += (nCt - 1);
					}
				}

			if( nCard1 != -1 && nCard2 != -1 ){
				if( pArrWinnerCards ){
					int nIndex = arrCards.FindFirst(nCard1%13);
					pArrWinnerCards->Add(arrCards.GetData(nIndex));
					pArrWinnerCards->Add(arrCards.GetData(nIndex+1));
					pArrWinnerCards->Add(arrCards.GetData(nIndex+2));

					nIndex = arrCards.FindFirst(nCard2%13);
					pArrWinnerCards->Add(arrCards.GetData(nIndex));
					pArrWinnerCards->Add(arrCards.GetData(nIndex+1));
					}

				sHandCards.Format(_T("%s, %s, %s"), PokerGUI::GetLangValue(_T("FULL_HOUSE")), sCardNames_Plural[nCard1%13], sCardNames_Plural[nCard2%13]);
				}
			break;
			}
		case CardsClass_Flush:{
			if( pArrWinnerCards ){
				pArrWinnerCards->Add(nArrCards[0]);
				pArrWinnerCards->Add(nArrCards[1]);
				pArrWinnerCards->Add(nArrCards[2]);
				pArrWinnerCards->Add(nArrCards[3]);
				pArrWinnerCards->Add(nArrCards[4]);
				}
			sHandCards.Format(_T("%s, %s"),PokerGUI::GetLangValue(_T("FLUSH")), sCardKind_Plural[nArrCards[0]/13]);
			break;
			}
		case CardsClass_Straight:{
			if( pArrWinnerCards ){
				pArrWinnerCards->Add(nArrCards[0]);
				pArrWinnerCards->Add(nArrCards[1]);
				pArrWinnerCards->Add(nArrCards[2]);
				pArrWinnerCards->Add(nArrCards[3]);
				pArrWinnerCards->Add(nArrCards[4]);
				}
			sHandCards = PokerGUI::GetLangValue(_T("STRAIGHT"));
			break;
			}
		case CardsClass_ThreeOfAKind:{
			sHandCards	= PokerGUI::GetLangValue(_T("THREE_OF_A_KIND"));
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add(nArrCards[i]%13, nArrCards[i]);
				}

			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				// Found out starting point.
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 3 ){
					if( pArrWinnerCards ){
						pArrWinnerCards->Add(arrCards.GetData(i));
						pArrWinnerCards->Add(arrCards.GetData(i+1));
						pArrWinnerCards->Add(arrCards.GetData(i+2));
						}
					sHandCards.Format(_T("%s, %s"),PokerGUI::GetLangValue(_T("THREE_OF_A_KIND")), sCardNames_Plural[arrCards[i]]);
					break;
					}
				}
			break;
			}
		case CardsClass_TwoPairs:{
			sHandCards = PokerGUI::GetLangValue(_T("TWO_PAIRS"));
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add(nArrCards[i]%13, nArrCards[i]);
				}

			// Find out pair cards.
			nCard1 = nCard2 = -1;
			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 2 ){
					if( nCard1 == -1 ){
						nCard1 = arrCards.GetData(i);
						}
					else
					if( (nCard2 == -1)|| (nCard2 != -1 && arrCards[i] > nCard2%13) )
						nCard2 = arrCards.GetData(i);
					}
				// Skip the same card values.
				if( nCt > 1 ){
					i += (nCt - 1);
					}
				}

			if( nCard1 != -1 && nCard2 != -1 ){
				if( pArrWinnerCards ){
					int nIndex = arrCards.FindFirst(nCard1%13);
					pArrWinnerCards->Add(arrCards.GetData(nIndex));
					pArrWinnerCards->Add(arrCards.GetData(nIndex+1));

					nIndex = arrCards.FindFirst(nCard2%13);
					pArrWinnerCards->Add(arrCards.GetData(nIndex));
					pArrWinnerCards->Add(arrCards.GetData(nIndex+1));
					}

				sHandCards.Format(_T("%s, %s, %s"), PokerGUI::GetLangValue(_T("TWO_PAIRS")), sCardNames_Plural[nCard1%13], sCardNames_Plural[nCard2%13]);
				}
			break;
			}
		case CardsClass_Pair:{
			sHandCards = PokerGUI::GetLangValue(_T("PAIR"));
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add(nArrCards[i]%13, nArrCards[i]);
				}

			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				// Found out starting point.
				if( arrCards[i] == arrCards[i + 1] ){
					if( (arrCards.GetCount() - i) < 3 ){
						return false; // Not Pair.
						}
					if( pArrWinnerCards ){
						pArrWinnerCards->Add(arrCards.GetData(i));
						pArrWinnerCards->Add(arrCards.GetData(i+1));
						}
					sHandCards.Format(_T("%s, %s"), PokerGUI::GetLangValue(_T("PAIR")), sCardNames_Plural[arrCards[i]]);
					break;
					}
				}
			break;
			}
		case CardsClass_HighCard:{
			sHandCards = PokerGUI::GetLangValue(_T("HIGH_CARD"));
			//sHandCards = _T("High Card");
			break;
			}
		};

	if( psLabelText )
		*psLabelText = sHandCards;
	if( bSetAsSeatMessage )
		SetSeatMessage(nSeat, sHandCards, true);
	return true;
	}

bool
PokerTableWnd::SetWinnerTableCards(PokerHandCardsClass handCards, CDWordArray* pArrWinnerCards){
	CString sCardKind[]			= {PokerGUI::GetLangValue(_T("CARD_KIND_CROSS")), PokerGUI::GetLangValue(_T("CARD_KIND_DIAMOND")), PokerGUI::GetLangValue(_T("CARD_KIND_SPADE")), PokerGUI::GetLangValue(_T("CARD_KIND_HEART"))};
	CString sCardKind_Plural[]	= {PokerGUI::GetLangValue(_T("CARD_KIND_CROSS_PL")), PokerGUI::GetLangValue(_T("CARD_KIND_DIAMOND_PL")), PokerGUI::GetLangValue(_T("CARD_KIND_SPADE_PL")), PokerGUI::GetLangValue(_T("CARD_KIND_HEART_PL"))};
	CString sCardNames[]		= {PokerGUI::GetLangValue(_T("CARD_TWO")), PokerGUI::GetLangValue(_T("CARD_THREE")), PokerGUI::GetLangValue(_T("CARD_FOUR")), PokerGUI::GetLangValue(_T("CARD_FIVE")), PokerGUI::GetLangValue(_T("CARD_SIX")), PokerGUI::GetLangValue(_T("CARD_SEVEN")), 
								PokerGUI::GetLangValue(_T("CARD_EIGHT")), PokerGUI::GetLangValue(_T("CARD_NINE")), PokerGUI::GetLangValue(_T("CARD_TEN")), PokerGUI::GetLangValue(_T("CARD_JACK")), PokerGUI::GetLangValue(_T("CARD_QUEEN")), PokerGUI::GetLangValue(_T("CARD_KING")), PokerGUI::GetLangValue(_T("CARD_ACE"))};
	CString sCardNames_Plural[]	= {PokerGUI::GetLangValue(_T("CARD_TWO_PL")), PokerGUI::GetLangValue(_T("CARD_THREE_PL")), PokerGUI::GetLangValue(_T("CARD_FOUR_PL")), PokerGUI::GetLangValue(_T("CARD_FIVE_PL")), PokerGUI::GetLangValue(_T("CARD_SIX_PL")), PokerGUI::GetLangValue(_T("CARD_SEVEN_PL")), 
								PokerGUI::GetLangValue(_T("CARD_EIGHT_PL")), PokerGUI::GetLangValue(_T("CARD_NINE_PL")), PokerGUI::GetLangValue(_T("CARD_TEN_PL")), PokerGUI::GetLangValue(_T("CARD_JACK_PL")), PokerGUI::GetLangValue(_T("CARD_QUEEN_PL")), PokerGUI::GetLangValue(_T("CARD_KING_PL")), PokerGUI::GetLangValue(_T("CARD_ACE_PL"))};

	int nCard1 = pArrWinnerCards->GetCount() > 0 ? pArrWinnerCards->GetAt(0) : -1;
	int nCard2 = pArrWinnerCards->GetCount() > 1 ? pArrWinnerCards->GetAt(1) : -1;
	int nCard3 = pArrWinnerCards->GetCount() > 2 ? pArrWinnerCards->GetAt(2) : -1;
	int nCard4 = pArrWinnerCards->GetCount() > 3 ? pArrWinnerCards->GetAt(3) : -1;
	int nCard5 = pArrWinnerCards->GetCount() > 4 ? pArrWinnerCards->GetAt(4) : -1;

	AutoSortedArray arrTableCards;
	arrTableCards.Add(m_nCard1);
	arrTableCards.Add(m_nCard2);
	arrTableCards.Add(m_nCard3);
	arrTableCards.Add(m_nCard4);
	arrTableCards.Add(m_nCard5);

	CString sHandCards	= _T("");
	switch( handCards ){
		case CardsClass_StraightFlush:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 &&
				nCard5 != -1){
				sHandCards = PokerGUI::GetLangValue(_T("STRAIGHT_FLUSH"));
				}
			break;
			}
		case CardsClass_FourOfAKind:{
			if( nCard1 != -1 )
				sHandCards.Format(_T("%s, %s"), PokerGUI::GetLangValue(_T("FOUR_OF_A_KIND")), sCardNames_Plural[nCard1%13]);
			break;
			}
		case CardsClass_FullHouse:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 ){
				sHandCards.Format(_T("%s, %s, %s"), PokerGUI::GetLangValue(_T("FULL_HOUSE")), sCardNames_Plural[nCard1%13], sCardNames_Plural[nCard2%13]);
				}
			break;
			}
		case CardsClass_Flush:{
			if( nCard1 != -1 )
				sHandCards.Format(_T("%s, %s"),PokerGUI::GetLangValue(_T("FLUSH")), sCardKind_Plural[nCard1/13]);
			break;
			}
		case CardsClass_Straight:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 &&
				nCard5 != -1){
				sHandCards = PokerGUI::GetLangValue(_T("STRAIGHT"));
				}
			break;
			}
		case CardsClass_ThreeOfAKind:{
			if( nCard1 != -1 )
				sHandCards.Format(_T("%s, %s"),PokerGUI::GetLangValue(_T("THREE_OF_A_KIND")), sCardNames_Plural[nCard1%13]);
			break;
			}
		case CardsClass_TwoPairs:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 ){
				sHandCards.Format(_T("%s, %s, %s"), PokerGUI::GetLangValue(_T("TWO_PAIRS")), sCardNames_Plural[nCard1%13], sCardNames_Plural[nCard3%13]);
				}
			break;
			}
		case CardsClass_Pair:{
			if( nCard1 != -1 &&
				nCard2 != -1 ){
				sHandCards.Format(_T("%s, %s"), PokerGUI::GetLangValue(_T("PAIR")), sCardNames_Plural[nCard1%13]);
				}			
			break;
			}
		case CardsClass_HighCard:{
			sHandCards = PokerGUI::GetLangValue(_T("HIGH_CARD"));
			break;
			}
		default:
			return false;
		};

	ESChildControlTableCards* pTableCards = GetTableCardsControl();
	if( pTableCards ){
		AutoSortedArray arrCards;
		for( int i=0; i<pArrWinnerCards->GetCount(); i++ ){
			arrCards.Add(pArrWinnerCards->GetAt(i));
			}

		pTableCards->SetMarkCards(
			(arrCards.IndexOf(m_nCard1) == -1),
			(arrCards.IndexOf(m_nCard2) == -1),
			(arrCards.IndexOf(m_nCard3) == -1),
			(arrCards.IndexOf(m_nCard4) == -1),
			(arrCards.IndexOf(m_nCard5) == -1));

		SetTableMessage(sHandCards, true);
		return true;
		}
	return false;
	}

void
PokerTableWnd::SetBestHandLabel(CString sText, bool bInvalidate /*= true*/){
	ESChildControlLabel* pLabel = (ESChildControlLabel*)GetChildControl(CHILD_Table_Best_Hand_Label);
	if( !pLabel ){
		pLabel = new ESChildControlLabel();
		if( AddChildControlEx(pLabel, CHILD_Table_Best_Hand_Label, m_pGUI->m_rcPlayerBestHandLabel, NULL, m_pGUI->m_szDefWindowSize) ){
//			pLabel->SetZIndex	(1);
			pLabel->SetTextColor(RGB(255, 255, 255));
			pLabel->SetFont		(m_pGUI->m_pTMessageFont);
			pLabel->SetCentred	(true);
			}
		else{
			ASSERT(FALSE);
			return;
			}
		}

	if( !pLabel->IsVisible() )
		pLabel->SetVisible(true, false);
	pLabel->SetLabel(sText, bInvalidate);
	}

void
PokerTableWnd::ClearBestHandLabel(bool bInvalidate /*= true*/){
	ESChildControlLabel* pLabel = (ESChildControlLabel*)GetChildControl(CHILD_Table_Best_Hand_Label);
	if( pLabel ){
		pLabel->SetLabel	(_T(""), false);
		pLabel->SetVisible	(false, bInvalidate);
		}
	}

LRESULT	
PokerTableWnd::OnCollectChipsFinished(WPARAM wParam, LPARAM lParam){
	int nMainPotAMount = (int)wParam;

	// Return chips to it's first position. {{
	CRect rcTableWnd;
	GetClientRect(rcTableWnd);

	for(int i=0; i<m_nMaxPlayerCt; i++){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(i);
		if( !pSeat || pSeat->GetPotAmount() <= 0 ) continue;
		SeatPanelInfo*			pSeatPanel	= GetSeatPanelInfo(i);
		ESChildControlChips*	pChips		= GetChipsControl(i);	
		if( pChips ){
			float fPosX	= (pSeatPanel->m_ptChipPos.x / ((float)m_pGUI->m_szDefWindowSize.cx));
			float fPosY	= (pSeatPanel->m_ptChipPos.y / ((float)m_pGUI->m_szDefWindowSize.cy));
			pChips->SetLogicalPos			(fPosX, fPosY);
			pChips->OwnerWindowSizeChanged	(rcTableWnd.Width(), rcTableWnd.Height());
			SetPotAmount					(i, 0, false);
			}
		}

	SetMainPotAmount(nMainPotAMount, true);
	// }}

	if( m_pHandler )
		m_pHandler->OnCollectChipsFinished();
	return 0L;
	}

LRESULT
PokerTableWnd::OnMoveChipsFinished(WPARAM wParam, LPARAM lParam){
	// Redraw chips. {{
	CRect rcRedraw(0, 0, 0, 0);
	for(int i=0; i<m_nMaxPlayerCt; i++){
		PokerTableSeatInfo* pSeat = m_seats.GetAt(i);
		if( !pSeat || pSeat->GetPotAmount() <= 0 ) continue;
		ESChildControlChips*	pChip		= GetChipsControl(i);	
		if( pChip ){
			pChip->SetDrawLabelText(true);
			rcRedraw |= pChip->GetClientRect();
			}
		}

	RedrawRect(rcRedraw, 1);
	// }}

	if( m_pHandler )
		m_pHandler->OnMoveChipsFromPotFinished();
	return 0L;
	}

LRESULT
PokerTableWnd::OnSeatCardsDealFinished(WPARAM wParam, LPARAM lParam){
	if( m_pHandler )
		m_pHandler->OnSeatCardsDealAnimationFinished();
	return 0L;
	}

LRESULT
PokerTableWnd::OnTableCardsDealFinished(WPARAM wParam, LPARAM lParam){
	if( m_pHandler )
		m_pHandler->OnTableCardsDealAnimationFinished();
	return 0L;
	}

//=======================================================
//
//	Designer context menu commands.
//

void
PokerTableWnd::OnCommandTable(){
	ESChildControlTable*	pControlNew = new ESChildControlTable();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandSeatStatusAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildSeatStatus*	pControlNew = new ESChildSeatStatus();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandSeatAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildSeat*	pControlNew = new ESChildSeat();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandChipsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlChips*	pControlNew = new ESChildControlChips();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandHiddenCardsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlSeatHiddenCards*	pControlNew = new ESChildControlSeatHiddenCards();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandSeatCardsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlSeatCards*	pControlNew = new ESChildControlSeatCards();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandTableCardsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlTableCards*	pControlNew = new ESChildControlTableCards();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}