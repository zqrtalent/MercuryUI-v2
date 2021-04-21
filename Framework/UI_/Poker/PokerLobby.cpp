// PokerLobby.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Controls\MercuryGUI.h"
#include "..\..\Controls\ESItemsList.h"
#include "..\..\Image\ImageHelper.h"
#include "PokerLobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerLobby

PokerLobby::PokerLobby() : MercuryBaseView(){
	m_pView			= NULL;
	m_bView			= FALSE;
	m_pHeader		= NULL;
	m_bHeader		= FALSE;
	m_pVScroll		= NULL;
	m_bVScroll		= FALSE;
	m_pHScroll		= NULL;
	m_bHScroll		= FALSE;
	m_pSwitchBar	= NULL;
	m_bSwitchBar	= FALSE;
	m_pTree			= NULL;
	m_bTree			= FALSE;
	m_nScrollsCt	= 0;
	}

IMPLEMENT_DYNCREATE(PokerLobby, MercuryBaseView)

PokerLobby::~PokerLobby()
	{
	DestroyClassMembers();
	}

BEGIN_MESSAGE_MAP(PokerLobby, MercuryBaseView)
	/*
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_CHAR			()
	ON_WM_KEYDOWN		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_SETCURSOR		()
	ON_WM_MOUSEWHEEL	()
	ON_WM_GETDLGCODE	()
	ON_WM_SHOWWINDOW	()
	ON_WM_MOUSEACTIVATE	()*/
	ON_MESSAGE			(WM_VSCROLL, OnVScroll)
	ON_MESSAGE			(WM_HSCROLL, OnHScroll)
	ON_WM_GETMINMAXINFO	()
	ON_WM_SIZE			()
	ON_WM_MOUSEWHEEL	()
	ON_WM_KEYDOWN		()
END_MESSAGE_MAP()

BOOL 
PokerLobby::Create(LPCTSTR lpszClassName,
						LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect,
						CWnd* pParentWnd, UINT nID,
						CCreateContext* pContext /*= NULL*/){
	BOOL bRet =  CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if( bRet ){
		CRect rcEmpty(0, 0, 0, 0);
		m_pView			= new ESItemsList();
		m_pHeader		= new ESHeaderCtrl();
		m_pVScroll		= new MercuryScrollBar();
		m_pHScroll		= new MercuryScrollBar();
		m_pSwitchBar	= new ESSwitchBar();
		m_pTree			= new MercuryTreeGrid();

		/*
		m_pView->m_pRoot		= this;
		m_pHeader->m_pRoot		= this;
		m_pSwitchBar->m_pRoot	= this;
		*/


	m_pRowSet				= new VirtualRowSet;
	m_pRowSet->SetOwner(this);

	m_pRowSet->AddField(_T("ID"), FieldType::FTInt32);
	m_pRowSet->AddFieldEx(_T("NAME"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME1"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME2"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME3"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME4"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME5"), FieldType::FTChar, 64);
	m_pRowSet->AddField(_T("_STATE"), FieldType::FTInt32);

	if( m_pRowSet->Create() > 0 ){

		CString sTitle;
		int nLoop = 0, nCt = 1000;
		while( nLoop < nCt ){

			m_pRowSet->AddRow();
			m_pRowSet->SetInt32(_T("ID"), nLoop);
			sTitle.Format(_T("sdfsdfsdfsd - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME1"), sTitle);
			sTitle.Format(_T("ghfghfghfg - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME2"), sTitle);
			sTitle.Format(_T("cvbcvbcvbcv - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME3"), sTitle);
			sTitle.Format(_T("tyutyuuioui - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME"), sTitle);
			sTitle.Format(_T("kl;khjhjkhj - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME4"), sTitle);
			sTitle.Format(_T("bnmbnmbnmbnm - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME5"), sTitle);

			nLoop ++;
			}

		m_pRowSet->SetCurrentRow(0);
		}
	// ----------------

		m_bView	= m_bHeader = m_bVScroll = m_bHScroll = m_bSwitchBar = m_bTree = TRUE;

		UINT nID = 1000;
		m_pView		->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pHeader	->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pVScroll	->Create(WS_CHILD|WS_VSCROLL, rcEmpty, this, nID++, NULL);
		m_pHScroll	->Create(WS_CHILD|WS_HSCROLL, rcEmpty, this, nID++, NULL);
		m_pSwitchBar->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pTree		->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pTree->SetBorders	(FALSE, TRUE, TRUE, FALSE);

		// TEMP_CODE
		m_pHeader->AddItem(_T("item1"), _T("ID"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item2"), _T("NAME1"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item3"), _T("NAME2"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item4"), _T("NAME3"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item5"), _T("NAME4"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item6"), _T("NAME5"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item7"), _T("NAME6"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item8"), _T("NAME7"), TEXT_HALIGN_LEFT, 120, 40);
		// TEMP_CODE

		m_pHeader	->SetRowSet(m_pRowSet);
		m_pView		->SetRowSet(m_pRowSet);

		int nImg = m_pTree->AddItemImage(_T("d:\\c++\\folder_24.png"));
		int nImg2 = m_pTree->AddItemImage(_T("d:\\c++\\documents.png"));

		MerTreeItem* pRoot = m_pTree->AddItem(_T("registracia"));
		MerTreeItem* pItem = NULL;
		pRoot->iImage = nImg;
		pRoot->iSelImage = nImg;
		/*
		for(int i=0; i<40; i++)
		{
		pItem = m_pTree->AddItem(_T("Item1"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;
		}*/

		pItem = m_pTree->AddItem(_T("pacienti"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;

		pItem = m_pTree->AddItem(_T("servisi"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;

		pItem = m_pTree->AddItem(_T("personali"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;

		pRoot = m_pTree->AddItem(_T("Root2"));
		pRoot->iImage = nImg;
		pRoot->iSelImage = nImg;
		for(int i=0; i<2; i++)
		{
		pItem = m_pTree->AddItem(_T("Item1"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;
		}

		m_controlRects.Add(VScrolBar, CRect(0, 0, 0, 0));
		m_controlRects.Add(HScrolBar, CRect(0, 0, 0, 0));
		m_controlRects.Add(ItemsListCtrl, CRect(0, 0, 0, 0));
		m_controlRects.Add(HeaderCtrl, CRect(0, 0, 0, 0));
		m_controlRects.Add(SwitchBar, CRect(0, 0, 0, 0));
		m_controlRects.Add(TreeControl, CRect(0, 0, 0, 0));

		if( !CRect(rect).IsRectEmpty() ){
			RecalcChildWndRects	(CRect(rect).Width(), CRect(rect).Height());
			SizeAllChildWindows	();
			AdjustScrollBars	();
			}
		}
	return bRet;
	}

void
PokerLobby::DestroyClassMembers()
	{
	/*
	if( m_pContent ){
		delete m_pContent;
		m_pContent = NULL;
		}

	for( int i=0; i<m_arrImages.GetCount(); i++ ){
		CImage* pImage = m_arrImages.GetData(i);
		if( pImage )
			delete pImage;
		}
	m_arrImages.DeleteAll();

	if( IsWindow(m_vScroll) )
		m_vScroll.DestroyWindow();

	if( IsWindow(m_header) ){
		m_header.DestroyWindow();
		}

	m_nTopItemIndex	= 0;
	m_nHeightView	= 0;
	m_nWidthView	= 0;
	m_pSelItem		= NULL;
	m_pFirstItem	= NULL;
	*/
	}

void
PokerLobby::OnPrePaint(CRect& rcRepaint){
	if( m_nScrollsCt == 2 ){ // Scrolbars are visible
		CRect rcClient;
		GetClientRect(rcClient);

		CImage* pImg = &m_pGUI->scrollBar.m_imgScrollBarDivider;
		rcRepaint.SetRect(rcClient.right - pImg->GetWidth(), 
						rcClient.bottom - pImg->GetHeight(), 
						rcClient.right, rcClient.bottom);
		}
	else
		rcRepaint.SetRect(0, 0, 0, 0);
	}

void 
PokerLobby::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	if( m_nScrollsCt == 2 ){ // Scrolbars are visible
		CRect rcClient;
		GetClientRect(rcClient);
		if( rcClient.IsRectEmpty() ) 
			return;

		CRect rcDestDC	= *pRectDC;
		CImage* pImg	= &m_pGUI->scrollBar.m_imgScrollBarDivider;
		CRect	rcSrc	(0, 0, pImg->GetWidth(), pImg->GetHeight());
		CRect	rcDest	(rcClient.right - rcSrc.Width(), 
						rcClient.bottom - rcSrc.Height(), 
						rcClient.right, rcClient.bottom);

		CRect rcDestItemDC;
		rcDestItemDC.left	= pRectDC->left + (rcDest.left - pRectClient->left);
		rcDestItemDC.top	= pRectDC->top + (rcDest.top - pRectClient->top);
		rcDestItemDC.right	= rcDestItemDC.left + rcDest.Width();
		rcDestItemDC.bottom = rcDestItemDC.top + rcDest.Height();

		rcDestItemDC 		&= rcDestDC;
		if( !rcDestItemDC.IsRectEmpty() ){
			ImgHlp::DrawImage(pDC, &m_pGUI->scrollBar.m_imgScrollBarDivider, rcSrc, rcDestItemDC);
			}
		}
	}

void
PokerLobby::OnShowWindow(BOOL bShow, UINT nStatus){
	CWnd::OnShowWindow(bShow, nStatus);
	}

void
PokerLobby::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags){
	CWnd::OnChar(nChar, nRepCnt, nFlags);
	}

void 
PokerLobby::OnLButtonDown(UINT nFlags, CPoint point) {
	CWnd::OnLButtonDown(nFlags, point);
	}

void 
PokerLobby::OnLButtonUp(UINT nFlags, CPoint point){
	CWnd::OnLButtonUp(nFlags, point);
	}

void 
PokerLobby::OnLButtonDblClk(UINT nFlags, CPoint point){
	CWnd::OnLButtonDblClk(nFlags, point);
	}

void
PokerLobby::OnSize(UINT nType, int cx, int cy){
	CWnd::OnSize(nType, cx, cy);

	RecalcChildWndRects	(cx, cy);
	SizeAllChildWindows	();
	AdjustScrollBars	();
	}

void 
PokerLobby::OnMouseMove(UINT nFlags, CPoint point){
	CWnd::OnMouseMove(nFlags, point);
	}

BOOL 
PokerLobby::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL 
PokerLobby::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if( pState && m_pView )
		m_pView->SendMessage(pState->m_msgCur.message, pState->m_msgCur.wParam, pState->m_msgCur.lParam);
	return 1;
	}

void
PokerLobby::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if( pState && m_pView )
		m_pView->SendMessage(pState->m_msgCur.message, pState->m_msgCur.wParam, pState->m_msgCur.lParam);
	else
		MercuryBaseView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

UINT 
PokerLobby::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

LRESULT	
PokerLobby::OnVScroll(WPARAM wParam, LPARAM lParam){
	if( m_pView && IsWindow(*m_pView) ){
		BOOL bRet = (BOOL)m_pView->SendMessage(WM_VSCROLL, wParam, lParam);
		return bRet;
		}
	return FALSE;
	}

LRESULT	
PokerLobby::OnHScroll(WPARAM wParam, LPARAM lParam){
	if( m_pView && IsWindow(*m_pView) ){
		BOOL bRet = (BOOL)m_pView->SendMessage(WM_HSCROLL, wParam, lParam);
		return bRet;
		}
	return 0;
	}

BOOL
PokerLobby::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

/*
Virtual methods
*/
BOOL
PokerLobby::CanSizeHeaderItem(int nItem, int nWidth){
	if( nWidth > m_pHeader->GetItemWidthMin(nItem) )
		return TRUE;
	return FALSE;
	}

void
PokerLobby::OnSizeHeaderItem(int nItem, int nWidth, int nWidthOld){
	ASSERT(m_pHeader && IsWindow(*m_pHeader));
	if( (nWidth - nWidthOld) == 0 )
		return;

	if( m_pHeader->GetLeftItem() < 0 )
		m_pHeader->SetLeftItem(0);

	BOOL bVScroll = (m_pVScroll != NULL) ? ::IsWindowVisible(*m_pVScroll) : FALSE;
	BOOL bHScroll = (m_pHScroll != NULL) ? ::IsWindowVisible(*m_pHScroll) : FALSE;

	CRect rcClient;
	GetClientRect		(rcClient);
	RecalcChildWndRects	(rcClient.Width(), rcClient.Height());
	AdjustScrollBars	();

	CRect rcHScroll(0, 0, 0, 0);
	if( m_pHScroll && IsWindow(*m_pHScroll) ){
		m_pHScroll->GetWindowRect(rcHScroll);
		}

	CRect rcVScroll(0, 0, 0, 0);
	if( m_pVScroll && IsWindow(*m_pVScroll) ){
		m_pVScroll->GetWindowRect(rcVScroll);
		}

	BOOL bVScrollNow = (m_pVScroll != NULL) ? ::IsWindowVisible(*m_pVScroll) : FALSE;
	BOOL bHScrollNow = (m_pHScroll != NULL) ? ::IsWindowVisible(*m_pHScroll) : FALSE;

	CRect rcHdrItem, rcInvalidate, rcView;
	m_pView->GetWindowRect(rcView);
	ScreenToClient(rcView);
	m_pHeader->GetItemRect(nItem, rcHdrItem);
	// Enable items width calculate state.
	m_pHeader->SetItemsWidthCalcState(TRUE);
	
	CRect rcInvalidHeader	(rcHdrItem.left - 1, rcHdrItem.top, rcView.right, rcHdrItem.bottom);
	CRect rcInvalidView		(rcHdrItem.left, 0, rcView.right, rcView.Height());

	// When horizontal scrollbar desappears.
	if( bHScrollNow != TRUE && bHScroll == TRUE ){
		CRect rcView = m_controlRects.GetAt(ItemsListCtrl);
		m_pView->MoveWindow		(rcView.left, rcView.top, rcView.Width(), rcView.Height(), FALSE);

		m_pView->ScreenToClient	(rcHScroll);
		m_pView->RedrawRect		(rcHScroll, TRUE);
		rcInvalidView.bottom = rcHScroll.top;
		}

	// When horizontal scrollbar appears.
	if( bHScrollNow == TRUE && bHScroll == FALSE ){
		CRect rcView = m_controlRects.GetAt(ItemsListCtrl);
		m_pView->MoveWindow	(rcView.left, rcView.top, rcView.Width(), rcView.Height(), FALSE);
		}
	// #####################################

	m_pHeader	->RedrawRect(rcInvalidHeader, TRUE);
	m_pView		->RedrawRect(rcInvalidView, TRUE);
	}

void
PokerLobby::SizeAllChildWindows(){
	CRect rc = m_controlRects.GetAt(ControlTypes::HeaderCtrl);
	if( !rc.IsRectEmpty() && m_pHeader )
		m_pHeader->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height(), TRUE);

	rc = m_controlRects.GetAt(ControlTypes::SwitchBar);
	if( !rc.IsRectEmpty() && m_pSwitchBar )
		m_pSwitchBar->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height(), TRUE);

	rc = m_controlRects.GetAt(ControlTypes::ItemsListCtrl);
	if( !rc.IsRectEmpty() && m_pView )
		m_pView->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height(), TRUE);

	rc = m_controlRects.GetAt(ControlTypes::TreeControl);
	if( !rc.IsRectEmpty() && m_pTree )
		m_pTree->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
	}

void
PokerLobby::RecalcChildWndRects(int cx, int cy){
	if( !m_pHeader || !m_pView ) return;
	CRect	rcSwitchBar		(0, 0, cx, m_pGUI->switchBar.m_nHeight);
	CRect	rcTree			(0, rcSwitchBar.bottom, 250, cy);
	CRect	rcHeader		(rcTree.right, 0, cx, m_pGUI->header.m_nHeight);
	
	if( m_pSwitchBar && IsWindow(m_pSwitchBar->GetSafeHwnd()) ){
		rcHeader.top		+= rcSwitchBar.Height();
		rcHeader.bottom		+= rcSwitchBar.Height();
		}

	CRect	rcVScroll		(0, 0, 0, 0);
	CRect	rcHScroll		(0, 0, 0, 0);
	CRect	rcItemsList		(rcTree.right, rcHeader.bottom, cx, cy);

	// Determine scrolls visibility. ########
	int		nViewCX		= m_pHeader->GetItemsWidth();
	int		nViewCY		= m_pView->GetViewHeight();
	BOOL	bVScroll	= FALSE, bHScroll = FALSE;
	if( nViewCX > rcHeader.Width() )
		bHScroll = TRUE;		

	if( nViewCY > rcItemsList.Height() )
		bVScroll = TRUE;
	// ######################################

	if( bVScroll && bHScroll ){
		rcVScroll.SetRect(cx - m_pGUI->scrollBar.m_nVScrollWidth, rcHeader.bottom, cx, cy);
		rcHScroll.SetRect(rcTree.right, cy - m_pGUI->scrollBar.m_nHScrollHeight, cx, cy);

		rcVScroll.bottom	-= m_pGUI->scrollBar.m_nHScrollHeight;
		rcHScroll.right		-= m_pGUI->scrollBar.m_nVScrollWidth;
		rcItemsList.right	-= m_pGUI->scrollBar.m_nVScrollWidth;
		rcItemsList.bottom	-= m_pGUI->scrollBar.m_nHScrollHeight;
		}
	else{
		if( bHScroll ){
			rcHScroll.SetRect(rcTree.right, cy - m_pGUI->scrollBar.m_nHScrollHeight, cx, cy);
			rcItemsList.bottom	-= m_pGUI->scrollBar.m_nHScrollHeight;
			}
		else
		if( bVScroll ){
			rcVScroll.SetRect(cx - m_pGUI->scrollBar.m_nVScrollWidth, rcHeader.bottom, cx, cy);
			rcItemsList.right	-= m_pGUI->scrollBar.m_nVScrollWidth;		
			}
		}

	m_controlRects.SetAt(HeaderCtrl,	rcHeader);
	m_controlRects.SetAt(SwitchBar,		rcSwitchBar);
	m_controlRects.SetAt(ItemsListCtrl, rcItemsList);
	m_controlRects.SetAt(HScrolBar,		rcHScroll);
	m_controlRects.SetAt(VScrolBar,		rcVScroll);
	m_controlRects.SetAt(TreeControl,	rcTree);
	}

void
PokerLobby::AdjustScrollBars(){
	CRect rcClient;
	GetClientRect(rcClient);
	if( !m_pHeader ) return;

	CRect rcHScroll = m_controlRects.GetAt(HScrolBar);
	CRect rcVScroll = m_controlRects.GetAt(VScrolBar);
	m_nScrollsCt	= 0;

	// Show/Hide horizontal scroll.
	if( !rcHScroll.IsRectEmpty() ){
		::SetWindowPos(*m_pHScroll, NULL, rcHScroll.left, rcHScroll.top, rcHScroll.Width(), rcHScroll.Height(), SWP_NOZORDER|SWP_NOREDRAW);
		m_pView->SendMessage(WM_SETUP_HSCROLL_INFO, 0, (LPARAM)m_pHScroll);
		if( ::IsWindowVisible(*m_pHScroll) )
			m_pHScroll->Invalidate();
		else
			m_pHScroll->ShowWindow(SW_SHOW);
		m_nScrollsCt ++;
		}
	else{
		if( ::IsWindowVisible(*m_pHScroll) ){
			m_pView		->SendMessage	(WM_SETUP_HSCROLL_INFO, 0, (LPARAM)m_pHScroll);
			m_pHScroll	->ShowWindow	(SW_HIDE);
			}
		}
	// Show/Hide vertical scroll.
	if( !rcVScroll.IsRectEmpty() ){
		::SetWindowPos(*m_pVScroll, NULL, rcVScroll.left, rcVScroll.top, rcVScroll.Width(), rcVScroll.Height(), SWP_NOZORDER|SWP_NOREDRAW);
		m_pView->SendMessage(WM_SETUP_VSCROLL_INFO, 0, (LPARAM)m_pVScroll);

		if( ::IsWindowVisible(*m_pVScroll) )
			m_pVScroll->Invalidate();
		else
			m_pVScroll->ShowWindow(SW_SHOW);
		m_nScrollsCt ++;
		}
	else{
		if( ::IsWindowVisible(*m_pVScroll) ){
			m_pView		->SendMessage	(WM_SETUP_VSCROLL_INFO, 0, (LPARAM)m_pVScroll);
			m_pVScroll	->ShowWindow	(SW_HIDE);
			}
		}
	}

void
PokerLobby::OnGetMinMaxInfo(MINMAXINFO* lpMMI){
	lpMMI->ptMinTrackSize.y = 200;
	MercuryBaseView::OnGetMinMaxInfo(lpMMI);
	}