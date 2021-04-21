// ESListControl.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ESListControl.h"
#include "ESItemsList.h"
#include "..\Image\ImageHelper.h"
#include "..\Data\DataFieldCopier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESListControl

ESListControl::ESListControl() : MercuryBaseView(){
	m_pView			= NULL;
	m_bView			= FALSE;
	m_pHeader		= NULL;
	m_bHeader		= FALSE;
	m_pVScroll		= NULL;
	m_bVScroll		= FALSE;
	m_pHScroll		= NULL;
	m_bHScroll		= FALSE;
	m_nScrollsCt	= 0;
	m_rcBorders.SetRect(0, 0, 0, 0);
	}

ESListControl::~ESListControl(){
	DestroyClassMembers();
	}

BEGIN_MESSAGE_MAP(ESListControl, MercuryBaseView)
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
ESListControl::Create(LPCTSTR lpszClassName,
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

		m_pView->m_pRoot		= this;
		m_pHeader->m_pRoot		= this;

		m_pRowSet = new VirtualRowSet;
		m_pRowSet->SetOwner(this);

		m_bView	= m_bHeader = m_bVScroll = m_bHScroll = TRUE;

		UINT nID = 1000;
		m_pView		->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pHeader	->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pVScroll	->Create(WS_CHILD|WS_VSCROLL, rcEmpty, this, nID++, NULL);
		m_pHScroll	->Create(WS_CHILD|WS_HSCROLL, rcEmpty, this, nID++, NULL);

		// Set owner to redirect selection event messages.
		m_pView->SetOwner(GetParent());

/*
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

			m_bView	= m_bHeader = m_bVScroll = m_bHScroll = TRUE;

			UINT nID = 1000;
			m_pView		->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
			m_pHeader	->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
			m_pVScroll	->Create(WS_CHILD|WS_VSCROLL, rcEmpty, this, nID++, NULL);
			m_pHScroll	->Create(WS_CHILD|WS_HSCROLL, rcEmpty, this, nID++, NULL);

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
*/
			m_pHeader	->SetRowSet(m_pRowSet);
			m_pView		->SetRowSet(m_pRowSet);
/*
		int nImg = m_pTree->AddItemImage(_T("d:\\c++\\folder_24.png"));
		int nImg2 = m_pTree->AddItemImage(_T("d:\\c++\\documents.png"));

		MerTreeItem* pRoot = m_pTree->AddItem(_T("registracia"));
		MerTreeItem* pItem = NULL;
		pRoot->iImage = nImg;
		pRoot->iSelImage = nImg;
		

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
		}*/

		m_controlRects.Add(VScrolBar,		CRect(0, 0, 0, 0));
		m_controlRects.Add(HScrolBar,		CRect(0, 0, 0, 0));
		m_controlRects.Add(ItemsListCtrl,	CRect(0, 0, 0, 0));
		m_controlRects.Add(HeaderCtrl,		CRect(0, 0, 0, 0));
		m_controlRects.Add(SwitchBar,		CRect(0, 0, 0, 0));
		m_controlRects.Add(TreeControl,		CRect(0, 0, 0, 0));

		if( !CRect(rect).IsRectEmpty() ){
			RecalcChildWndRects	(CRect(rect).Width(), CRect(rect).Height());
			SizeAllChildWindows	();
			AdjustScrollBars	();
			}
		}
	return bRet;
	}

void
ESListControl::SetBorder(bool bLeft, bool bTop, bool bRight, bool bBottom){
	m_rcBorders.SetRect(bLeft, bTop, bRight, bBottom);
	}

int		
ESListControl::AddColumn(CString sTitle, CString sFieldName, int nAlign, int nWidth, int nWidthMin){
	if( !m_pHeader ) return -1;
	int nRet = m_pHeader->AddItem(sTitle.GetBuffer(), sFieldName.GetBuffer(), nAlign, nWidth, nWidthMin);
	return nRet;
	}

bool
ESListControl::SetColTitle(int nCol, CString sTitle){
	return false;
	}

bool
ESListControl::RemoveColumn(int nCol){
	if( !m_pHeader ) return false;
	m_pHeader->RemoveItem(nCol);
	return true;
	}

ESHeaderCtrl*
ESListControl::GetHeaderCtrl(){
	return m_pHeader;
	}

MercuryScrollBar*
ESListControl::GetVScrollCtrl(){
	return m_pVScroll;
	}

MercuryScrollBar*
ESListControl::GetHScrollCtrl(){
	return m_pHScroll;
	}

void
ESListControl::DestroyClassMembers(){
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
ESListControl::OnPrePaint(CRect& rcRepaint){
	if( m_nScrollsCt == 2 ){ // Scrolbars are visible
		CRect rcClient;
		GetClientRect(rcClient);

		CImage* pImg = &m_pGUI->scrollBar.m_imgScrollBarDivider;
		rcRepaint.SetRect(rcClient.right - pImg->GetWidth(), 
						rcClient.bottom - pImg->GetHeight(), 
						rcClient.right, rcClient.bottom);
		}
	//else
	//	rcRepaint.SetRect(0, 0, 0, 0);
	}

void 
ESListControl::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
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

	// Draw border. ###############################
	CRect	rcClient;
	GetClientRectMy(rcClient);

	CRect	rcDestDC; // Full destination rect in DC coordinates.
	rcDestDC.left	= pRectDC->left - pRectClient->left;
	rcDestDC.top	= pRectDC->top - pRectClient->top;
	rcDestDC.right	= rcDestDC.left + rcClient.Width();
	rcDestDC.bottom	= rcDestDC.top + rcClient.Height();

	CRect rcFill = rcDestDC;
	CPen penBorder(PS_SOLID, 1, m_pGUI->treeGrid.m_crBorder);
	CPen* penOld = pDC->SelectObject(&penBorder);

	pDC->MoveTo(rcDestDC.left, rcDestDC.top);
	if( m_rcBorders.top > 0 ){
		pDC->LineTo(rcDestDC.right, rcDestDC.top);
		rcFill.top ++;
		}

	if( m_rcBorders.right > 0 ){
		pDC->MoveTo(rcDestDC.right - 1, rcDestDC.top);
		pDC->LineTo(rcDestDC.right - 1, rcDestDC.bottom);
		rcFill.right --;
		}

	if( m_rcBorders.bottom > 0 ){
		pDC->MoveTo(rcDestDC.right - 1, rcDestDC.bottom - 1);
		pDC->LineTo(rcDestDC.left, rcDestDC.bottom - 1);
		rcFill.bottom --;
		}
	
	if( m_rcBorders.left > 0 ){
		pDC->MoveTo(rcDestDC.left, rcDestDC.bottom - 1);
		pDC->LineTo(rcDestDC.left, rcDestDC.top - 1);
		rcFill.left ++;
		}
	pDC->SelectObject(penOld);
	// #############################################
	}

void
ESListControl::OnShowWindow(BOOL bShow, UINT nStatus){
	CWnd::OnShowWindow(bShow, nStatus);
	}

void
ESListControl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags){
	CWnd::OnChar(nChar, nRepCnt, nFlags);
	}

void 
ESListControl::OnLButtonDown(UINT nFlags, CPoint point) {
	CWnd::OnLButtonDown(nFlags, point);
	}

void 
ESListControl::OnLButtonUp(UINT nFlags, CPoint point){
	CWnd::OnLButtonUp(nFlags, point);
	}

void 
ESListControl::OnLButtonDblClk(UINT nFlags, CPoint point){
	CWnd::OnLButtonDblClk(nFlags, point);
	}

void
ESListControl::OnSize(UINT nType, int cx, int cy){
	CWnd::OnSize(nType, cx, cy);

	RecalcChildWndRects	(cx, cy);
	SizeAllChildWindows	();
	AdjustScrollBars	();
	}

void 
ESListControl::OnMouseMove(UINT nFlags, CPoint point){
	CWnd::OnMouseMove(nFlags, point);
	}

BOOL 
ESListControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL 
ESListControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if( pState && m_pView )
		m_pView->SendMessage(pState->m_msgCur.message, pState->m_msgCur.wParam, pState->m_msgCur.lParam);
	return 1;
	}

void
ESListControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if( pState && m_pView )
		m_pView->SendMessage(pState->m_msgCur.message, pState->m_msgCur.wParam, pState->m_msgCur.lParam);
	else
		MercuryBaseView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

UINT 
ESListControl::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

LRESULT	
ESListControl::OnVScroll(WPARAM wParam, LPARAM lParam){
	if( m_pView && IsWindow(*m_pView) ){
		BOOL bRet = (BOOL)m_pView->SendMessage(WM_VSCROLL, wParam, lParam);
		return bRet;
		}
	return FALSE;
	}

LRESULT	
ESListControl::OnHScroll(WPARAM wParam, LPARAM lParam){
	if( m_pView && IsWindow(*m_pView) ){
		BOOL bRet = (BOOL)m_pView->SendMessage(WM_HSCROLL, wParam, lParam);
		return bRet;
		}
	return 0;
	}

BOOL
ESListControl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

/*
Virtual methods
*/
BOOL
ESListControl::CanSizeHeaderItem(int nItem, int nWidth){
	if( nWidth > m_pHeader->GetItemWidthMin(nItem) )
		return TRUE;
	return FALSE;
	}

void
ESListControl::OnSizeHeaderItem(int nItem, int nWidth, int nWidthOld){
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
ESListControl::SizeAllChildWindows(){
	CRect rc = m_controlRects.GetAt(ControlTypes::HeaderCtrl);
	if( !rc.IsRectEmpty() && m_pHeader )
		m_pHeader->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height(), TRUE);

	rc = m_controlRects.GetAt(ControlTypes::ItemsListCtrl);
	if( !rc.IsRectEmpty() && m_pView )
		m_pView->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
	}

void
ESListControl::RecalcChildWndRects(int cx, int cy){
	if( !m_pHeader || !m_pView ) return;
	CRect	rcHeader		(0, 0, cx, m_pGUI->header.m_nHeight);
	/*
	if( m_pSwitchBar && IsWindow(m_pSwitchBar->GetSafeHwnd()) ){
		rcHeader.top		+= rcSwitchBar.Height();
		rcHeader.bottom		+= rcSwitchBar.Height();
		}*/

	CRect	rcVScroll		(0, 0, 0, 0);
	CRect	rcHScroll		(0, 0, 0, 0);
	CRect	rcItemsList		(0, rcHeader.bottom, cx, cy);

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
		rcHScroll.SetRect(0, cy - m_pGUI->scrollBar.m_nHScrollHeight, cx, cy);

		rcVScroll.bottom	-= m_pGUI->scrollBar.m_nHScrollHeight;
		rcHScroll.right		-= m_pGUI->scrollBar.m_nVScrollWidth;
		rcItemsList.right	-= m_pGUI->scrollBar.m_nVScrollWidth;
		rcItemsList.bottom	-= m_pGUI->scrollBar.m_nHScrollHeight;
		}
	else{
		if( bHScroll ){
			rcHScroll.SetRect(0, cy - m_pGUI->scrollBar.m_nHScrollHeight, cx, cy);
			rcItemsList.bottom	-= m_pGUI->scrollBar.m_nHScrollHeight;
			}
		else
		if( bVScroll ){
			rcVScroll.SetRect(cx - m_pGUI->scrollBar.m_nVScrollWidth, rcHeader.bottom, cx, cy);
			rcItemsList.right	-= m_pGUI->scrollBar.m_nVScrollWidth;		
			}
		}

	if( m_rcBorders.top > 0 ){
		rcHeader.top	+= m_rcBorders.top;
		rcHeader.bottom += m_rcBorders.top;
		rcItemsList.top += m_rcBorders.top;
		rcVScroll.top	+= m_rcBorders.top;
		}

	if( m_rcBorders.left > 0 ){
		rcHeader.left		+= m_rcBorders.left;
		rcItemsList.left	+= m_rcBorders.left;
		rcHScroll.left		+= m_rcBorders.left;
		}

	if( m_rcBorders.bottom > 0 ){
		rcHScroll.bottom	-= m_rcBorders.bottom;
		rcHScroll.top		-= m_rcBorders.bottom;
		rcItemsList.bottom	-= m_rcBorders.bottom;
		rcVScroll.bottom	-= m_rcBorders.bottom;
		}

	if( m_rcBorders.right > 0 ){
		rcHeader.right		-= m_rcBorders.right;
		rcVScroll.right		-= m_rcBorders.right;
		rcVScroll.left		-= m_rcBorders.right;
		rcItemsList.right	-= m_rcBorders.right;
		}

	m_controlRects.SetAt(HeaderCtrl,	rcHeader);
	m_controlRects.SetAt(ItemsListCtrl, rcItemsList);
	m_controlRects.SetAt(HScrolBar,		rcHScroll);
	m_controlRects.SetAt(VScrolBar,		rcVScroll);
	}

bool
ESListControl::AdjustListViewPos(){
	if( !m_pView || !IsWindow(*m_pView) )
		return false;

	CRect rcList;
	m_pView->GetWindowRect(rcList);
	ScreenToClient(rcList);

	CRect rcList2 = m_controlRects.GetAt(ItemsListCtrl);
	if( rcList2 != rcList ){
		::SetWindowPos(*m_pView, NULL, rcList2.left, rcList2.top, rcList2.Width(), rcList2.Height(), SWP_NOZORDER);
		return true;
		}
	return false;
	}

void
ESListControl::AdjustScrollBars(){
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
ESListControl::OnGetMinMaxInfo(MINMAXINFO* lpMMI){
	lpMMI->ptMinTrackSize.y = 200;
	MercuryBaseView::OnGetMinMaxInfo(lpMMI);
	}

//=====================================================
//
//	List content methods.
//
//=====================================================

bool		
ESListControl::Fill(EnumerableObject<Serializable>* pListObjects){
	if( !pListObjects || !pListObjects->GetCount() ){
		if( m_pRowSet ){
			m_pRowSet	->EmptyRowSet();
			m_pHeader	->SetRowSet(m_pRowSet);
			m_pView		->SetRowSet(m_pRowSet);

			Invalidate(FALSE);
			}
		return false;
		}

	Serializable*	pTempObj	= pListObjects->CreateObject();
	ASSERT(pTempObj);

	VirtualRowSet*	pVRowSet = new VirtualRowSet();
	pVRowSet->AddField(_T("_STATE"), FieldType::FTInt32);
	VirtualRowSet::CreateFromSerializable(pTempObj, pVRowSet);
	if( !pVRowSet ){
		return false;
		}

	// Clear items.
	Clear(false);

	DataFieldCopier copyPerformer(pVRowSet, pTempObj);
	int				nLoop		= 0;
	int				nCount		= pListObjects->GetCount();

	pVRowSet->AddRow();
	while( nLoop < nCount ){
		if( copyPerformer.CopyFromSerializable(pVRowSet, pListObjects->GetAt(nLoop)) ){
			pVRowSet->AddRow();
			}
		nLoop ++;
		}
	// Delete last row. {{
	pVRowSet->DeleteRow(pVRowSet->GetRowCount() - 1);
	// }}

	if( m_pRowSet ){
		delete m_pRowSet;
		}
	m_pRowSet = pVRowSet;
	m_pHeader	->SetRowSet(m_pRowSet);
	m_pView		->SetRowSet(m_pRowSet);

	CRect rcClient;
	GetClientRect(rcClient);
	RecalcChildWndRects	(rcClient.Width(), rcClient.Height());
	AdjustScrollBars	();
	AdjustListViewPos	();

	if( IsWindow(*m_pView) ){
		m_pView->Invalidate();
		}

	/*
	// Fill header items. {{
	m_pHeader->RemoveAllItems();
	int	nField		= 0;
	int nFieldCt	= m_pRowSet->GetFieldCount();

	while( nField < nFieldCt ){
		VRowSetFieldInfo* pVField = m_pRowSet->GetFieldInfo(nField);
		ASSERT(pVField);
		m_pHeader->AddItem((TCHAR*)pVField->szName, (TCHAR*)pVField->szName, TEXT_HALIGN_LEFT, 120, 40);
		nField ++;
		}
	// }}
	*/
	return true;
	}

int
ESListControl::Clear(bool bRedraw){
	m_pHeader->SetLeftItem	(0);
	m_pHeader->SetLeftOffset(0, FALSE);
	m_pHeader->SetRowSet	(NULL);
	m_pView->ClearItems		();

	if( m_pVScroll && IsWindow(*m_pVScroll) && ::IsWindowVisible(*m_pVScroll) ){
		m_pVScroll->ShowWindow(SW_HIDE);
		}
	if( m_pHScroll && IsWindow(*m_pHScroll) && ::IsWindowVisible(*m_pHScroll) ){
		m_pHScroll->ShowWindow(SW_HIDE);
		}

	if( bRedraw ){
		Invalidate(TRUE);
		}

	return 0;
	}

int
ESListControl::Add(EnumerableObject<Serializable>* pListObjects){
	return 0;
	}

int
ESListControl::GetInt(int nRow, TCHAR* pszField, int nDefault /*= 0*/){
	if( !m_pRowSet || !m_pView )
		return nDefault;

	if( nRow == -1 )
		nRow = m_pView->GetSelectedItem();

	if( !m_pRowSet->SetCurrentRow(nRow) )
		return nDefault;
	return m_pRowSet->GetInt32((TCHAR*)pszField);
	}

CString
ESListControl::GetString(int nRow, TCHAR* pszField){
	if( !m_pRowSet || !m_pView )
		return _T("");

	if( nRow == -1 )
		nRow = m_pView->GetSelectedItem();

	if( !m_pRowSet->SetCurrentRow(nRow) )
		return _T("");
	return m_pRowSet->GetString((TCHAR*)pszField);
	}

COleDateTime
ESListControl::GetDateTime(int nRow, TCHAR* pszField){
	COleDateTime dtRet;
	if( !m_pRowSet || !m_pView )
		return dtRet;

	if( nRow == -1 )
		nRow = m_pView->GetSelectedItem();

	if( !m_pRowSet->SetCurrentRow(nRow) )
		return dtRet;

	m_pRowSet->GetDate((TCHAR*)pszField, dtRet);
	return dtRet;
	}