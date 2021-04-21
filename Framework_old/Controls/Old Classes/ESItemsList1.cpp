// ESItemsList.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ESListControl.h"
#include "ESItemsList.h"
#include "ESMemMan.h"
#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESItemsList

ESItemsList::ESItemsList() : ESItemsView(){
	m_items					= new ESItems();
	m_nTopItem				= 0;
	m_nTopItemOffset		= 0;
	m_nCurrItem				= 0;
	m_nSubItemLeft			= 0;
	m_nSubItemLeftOffset	= 0;
	}

ESItemsList::~ESItemsList(){
	// Destroy list memory.
	if( m_items ){
		delete m_items;
		m_items = NULL;
		}
	// ####################
	}

BEGIN_MESSAGE_MAP(ESItemsList, ESItemsView)
	ON_WM_SIZE			()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_KEYDOWN		()
	ON_MESSAGE			(WM_VSCROLL, OnVScroll)
	ON_MESSAGE			(WM_HSCROLL, OnHScroll)
	ON_MESSAGE			(WM_SETUP_VSCROLL_INFO, OnSetupVScrollInfo)
	ON_MESSAGE			(WM_SETUP_HSCROLL_INFO, OnSetupHScrollInfo)
	ON_WM_GETDLGCODE	()
	ON_WM_MOUSEWHEEL	()
	ON_WM_MOUSEACTIVATE	()
END_MESSAGE_MAP()

BOOL 
ESItemsList::Create(LPCTSTR lpszClassName,
					LPCTSTR lpszWindowName, DWORD dwStyle,
					const RECT& rect,
					CWnd* pParentWnd, UINT nID,
					CCreateContext* pContext /*= NULL*/)
	{
	BOOL bRet =  CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	return bRet;
	}

void
ESItemsList::OnPrePaint(CRect& rcRepaint){
	}

void 
ESItemsList::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	CRect	rcClient;
	GetClientRect(rcClient);
	if( rcClient.IsRectEmpty() || pRectClient->IsRectEmpty() ) 
		return;

	CRect	rcDestDC; // Full destination rect in DC coordinates.
	if( UseDoubleBufferedDC() == TRUE ){
		/*
		rcDestDC.left	= pRectDC->left - pRectClient->left;
		rcDestDC.top	= pRectDC->top - pRectClient->top;
		rcDestDC.right	= rcDestDC.left + rcClient.Width();
		rcDestDC.bottom	= rcDestDC.top + rcClient.Height();
		*/
		rcDestDC = *pRectDC;
		}
	else
		rcDestDC = *pRectDC;

	ESHeaderCtrl*	pHeader			= m_pRoot->GetHeaderCtrl();
	int				nItem			= m_nTopItem;
	int				nItemTopOffset	= m_nTopItemOffset;
	int				nOffsetX		= 0;
	int				nOffsetY		= 0;

	int				nItemLast		= min(m_items->GetItemsCount() - 1, nItem + (rcClient.Height() / m_pGUI->itemsList.m_nItemHeight));
	int				nSubItem		= pHeader->m_nLeftItem;
	int				nSubItemOffset	= pHeader->m_nLeftItemOffsetX;
	int				nLeftSubItem	= nSubItem;
	COLORREF		crItemText		= RGB(255, 255, 255);

	m_nSubItemLeft					= pHeader->m_nLeftItem;
	m_nSubItemLeftOffset			= pHeader->m_nLeftItemOffsetX;

	COLORREF		crBrush1		= RGB(242, 245, 249);
	COLORREF		crBrush2		= RGB(255, 255, 255);
	COLORREF		crBrushSel		= RGB(75, 149, 229);
	COLORREF*		pcrBrushCurr	= NULL;

	CBrush			brush1(crBrush1);
	CBrush			brush2(crBrush2);
	CBrush			brushSel(crBrushSel);
	CBrush			*pBrushCurr = NULL;

	CRect			rcItems(0, 0, 0, 0);
	CRect			rcItem(0, 0, 0, 0);
	CString			sItemText;
	CRect			rcDestItemDC(0, 0, 0, 0);
	CSize			szText(0, 0);
	int				nDragItem		= pHeader->GetDragItemIndex();

	while( nItem <= nItemLast ){	
		pBrushCurr		= (((nItem - m_nTopItem)%2 == 0) ? &brush1 : &brush2);
		pcrBrushCurr	= (((nItem - m_nTopItem)%2 == 0) ? &crBrush1 : &crBrush2);

		if( (m_items->GetItemState(nItem)&ITEM_SELECTED) == ITEM_SELECTED ){
			pBrushCurr	= &brushSel;
			pcrBrushCurr = &crBrushSel;
			crItemText	= RGB(255, 255, 255);
			}
		else
			crItemText	= RGB(0, 0, 0);

		while( m_items->GetSubItemRect(this, nItem, nSubItem, rcItem) ){
			// Translate item rect in destination DC coordinates.
			/*
			rcDestItemDC.left	= rcItem.left;
			rcDestItemDC.top	= rcItem.top - nItemTopOffset;
			rcDestItemDC.right	= rcDestItemDC.left + rcItem.Width();
			rcDestItemDC.bottom = rcDestItemDC.top + rcItem.Height();
			rcDestItemDC		= rcDestDC&rcDestItemDC;
			*/
			rcDestItemDC.left	= pRectDC->left + ((rcItem.left /*- m_nSubItemLeftOffset*/) - pRectClient->left);
			rcDestItemDC.top	= pRectDC->top + ((rcItem.top - nItemTopOffset) - pRectClient->top);
			rcDestItemDC.right	= rcDestItemDC.left + rcItem.Width();
			rcDestItemDC.bottom = rcDestItemDC.top + rcItem.Height();

			nOffsetX			= rcDestItemDC.left;
			nOffsetY			= rcDestItemDC.top;
			rcDestItemDC		= rcDestDC&rcDestItemDC;
			// ##################################################

			if( rcDestItemDC.IsRectEmpty() ){
				nSubItem ++;
				continue;
				}

			// Draw dragging item. #############################
			if( nSubItem == nDragItem ){
				CBrush brush(m_pGUI->itemsList.m_crDragItemBkgnd);
				pDC->FillRect(rcDestItemDC, &brush);
				brush.DeleteObject();

				// Goto nex sub item.
				nSubItem	++;
				rcItems		|= rcItem;
				continue;
				}
			// #################################################

			// Retrieve subitem text.###########################
			if( !m_items->GetSubItemText(sItemText, pHeader->GetFieldId(nSubItem), nItem) )
				sItemText = "";
			// Draw sub item text. #############################
			CFont* pFontOld = pDC->SelectObject(&m_pGUI->itemsList.m_fontItemTextNormal);
			szText = pDC->GetTextExtent(sItemText.GetBuffer(), sItemText.GetLength());
/*
			CPoint	posText;
			posText.x = rcItem.left + 2;
			posText.y = rcItem.top + (rcItem.Height() - szText.cy)/2;
*/
			CPoint	posText(nOffsetX + 2, nOffsetY + (rcItem.Height() - szText.cy)/2);
			int		nAlign	= pHeader->GetItemHAlign(nSubItem);
			if( nAlign == TEXT_HALIGN_MIDDLE ){
				posText.x = nOffsetX + (rcItem.Width() - szText.cx)/2;
				}
			else
			if( nAlign == TEXT_HALIGN_RIGHT ){
				posText.x = (nOffsetX + rcItem.Width() - szText.cx - 2);
				}

			pDC->SetBkColor		(*pcrBrushCurr);
			pDC->SetBkMode		(OPAQUE);
			pDC->SetTextColor	(crItemText);
			pDC->ExtTextOut		(posText.x, posText.y, ETO_CLIPPED, &rcDestItemDC, sItemText, NULL);
			pDC->SelectObject	(pFontOld);
			// #################################################

			// Fill background of sub item. ####################
			CRgn rgnFill, rgnTextArea;
			rgnTextArea	.CreateRectRgn(posText.x, posText.y, posText.x + szText.cx, posText.y + szText.cy);
			rgnFill		.CreateRectRgn(rcDestItemDC.left, rcDestItemDC.top, rcDestItemDC.right, rcDestItemDC.bottom);
			rgnFill		.CombineRgn(&rgnFill, &rgnTextArea, RGN_DIFF);
			pDC->FillRgn(&rgnFill, pBrushCurr);

			rgnTextArea	.DeleteObject();
			rgnFill		.DeleteObject();
			// #################################################

			nSubItem ++;
			rcItems |= rcItem;
			}

		// Fill none drawn item area with item background color.
		if( rcDestItemDC.IsRectEmpty() ){
			rcDestItemDC.left	= rcItem.right;
			rcDestItemDC.top	= rcItem.top - nItemTopOffset;
			rcDestItemDC.right	= rcDestDC.right;
			rcDestItemDC.bottom = rcDestItemDC.top + rcItem.Height();
			if( !rcDestItemDC.IsRectEmpty() )
				pDC->FillRect(rcDestItemDC, pBrushCurr);
			}	
		else{
			if( rcDestDC.right > rcDestItemDC.right ){
				rcDestItemDC.left	= rcDestItemDC.right;
				rcDestItemDC.right	= rcDestDC.right;
				pDC->FillRect(rcDestItemDC, pBrushCurr);
				}
			}
		// ###################################################
		nSubItem = nLeftSubItem;
		nItem ++;
		}

	// Fill bottom items. ##############################
	if( rcItem.top > rcDestDC.top )
		rcDestDC.top = (rcItem.top - nItemTopOffset) + rcItem.Height();

	while( rcDestDC.top < rcDestDC.bottom ){
		nItem			++;
		pBrushCurr		= (((nItem - m_nTopItem)%2 == 1) ? &brush1 : &brush2);
		CRect rcFill(rcDestDC.left, rcDestDC.top, rcDestDC.right, min(rcDestDC.top + m_pGUI->itemsList.m_nItemHeight, rcDestDC.bottom));
		pDC->FillRect(rcFill, pBrushCurr);
		rcDestDC.top	+= m_pGUI->itemsList.m_nItemHeight;
		}
	// #################################################
	}

void
ESItemsList::OnSize(UINT nType, int cx, int cy){
	ESItemsView::OnSize(nType, cx, cy);
	}

void
ESItemsList::SetRowSet(VirtualRowSet* pRowSet){
	ASSERT( pRowSet );
	m_pRowSet = pRowSet;
	m_items->SetRowSet(m_pRowSet);

	if( !m_pRowSet->GetRowCount() ){
		m_nTopItemOffset		= 0;
		m_nCurrItem				= 0;
		m_nSubItemLeft			= 0;
		m_nSubItemLeftOffset	= 0;
		}
	}

void
ESItemsList::ClearItems(){
	m_nTopItem				= 0;
	m_nTopItemOffset		= 0;
	m_nCurrItem				= 0;
	m_nSubItemLeft			= 0;
	m_nSubItemLeftOffset	= 0;

	m_pRowSet = NULL;
	m_items->ClearSelectedItems	();
	m_items->SetRowSet			(NULL);
	}

void
ESItemsList::OnItemsCountChanged(){
	m_pRoot->AdjustScrollBars();
	}
int
ESItemsList::GetViewHeight(){
	if( !m_pRowSet ) return 0;
	return m_pRowSet->GetRowCount()*m_pGUI->itemsList.m_nItemHeight;
	}

int
ESItemsList::GetItemCount(){
	if( !m_pRowSet ) return 0;
	return m_pRowSet->GetRowCount();
	}

LRESULT	
ESItemsList::OnVScroll(WPARAM wParam, LPARAM lParam){
	int		nPos	= (int)wParam;
	int		nSBType	= (int)lParam;
	CRect	rcClient;

	switch( nSBType ){
		case SB_THUMBTRACK:{
			GetClientRect(rcClient);
			int nPage = rcClient.Height() / m_pGUI->itemsList.m_nItemHeight;
			if( nPos < 0 ) nPos = 0;
			else
			if( nPos > GetItemCount() - nPage ){
				nPos = (GetItemCount() - nPage);
				if( nPos < 0 ) nPos = 0;
				}
		
			if( m_nTopItem == nPos )
				return FALSE;
			m_nTopItem = nPos;
			RedrawRect(rcClient, TRUE);
			break;
			}
		case SB_LINEDOWN:{
			GetClientRect(rcClient);
			int nPage = rcClient.Height() / m_pGUI->itemsList.m_nItemHeight;
			if( m_nTopItem >= GetItemCount() - nPage ){
				return FALSE;
				}
			m_nTopItem ++;
			RedrawRect(rcClient, TRUE);
			break;
			}
		case SB_LINEUP:{
			m_nTopItem --;
			if( m_nTopItem < 0 ){
				m_nTopItem = 0;
				return FALSE;
				}
			GetClientRect(rcClient);
			RedrawRect(rcClient, TRUE);
			break;
			}
		case SB_PAGEUP:{
			GetClientRect(rcClient);
			int nPage = rcClient.Height() / m_pGUI->itemsList.m_nItemHeight;
			m_nTopItem -= nPage;
			if( m_nTopItem < 0 )
				m_nTopItem = 0;
			RedrawRect(rcClient, TRUE);
			return m_nTopItem; 
			break;			 
			}
	    case SB_PAGEDOWN:{
			GetClientRect(rcClient);
			int nPage = rcClient.Height() / m_pGUI->itemsList.m_nItemHeight;
			m_nTopItem += nPage;
			RedrawRect(rcClient, TRUE);
			return m_nTopItem;
			break;			 		 
			}
		}
	return TRUE;
	}

LRESULT	
ESItemsList::OnHScroll(WPARAM wParam, LPARAM lParam){
	int				nPos		= (int)wParam;
	int				nSBType		= (int)lParam;
	CRect			rcClient;
	ESHeaderCtrl*	pHeader		= m_pRoot->GetHeaderCtrl();
	if( !pHeader ) return 0;

	switch( nSBType ){
		case SB_THUMBTRACK:{
			GetClientRect(rcClient);
			int nPage = rcClient.Width();
			if( nPos < 0 )
				nPos = 0;
			else
			if( nPos > pHeader->GetItemsWidth() - nPage ){
				nPos = (pHeader->GetItemsWidth() - nPage);
				if( nPos < 0 ) nPos = 0;
				}
	
			int nMoveX = 0;
			if( !pHeader->SetLeftOffset(nPos, TRUE) )
				return 0;

			m_nSubItemLeft			= pHeader->m_nLeftItem;
			m_nSubItemLeftOffset	= pHeader->m_nLeftItemOffsetX;

			CRect rcHeader;
			pHeader->GetClientRect(rcHeader);

			CRect rcClient;
			GetClientRect(rcClient);

			RedrawRect(rcClient, TRUE);
			pHeader->RedrawRect(rcHeader, TRUE);
			break;
			}
		case SB_LINELEFT:{
			GetClientRect(rcClient);
			int	nPosOld = pHeader->GetLeftOffset();
			int nPos	= nPosOld - 5;
			int nPage	= rcClient.Width();
			if( nPos < 0 )
				nPos = 0;
			else
			if( nPos > pHeader->GetItemsWidth() - nPage ){
				nPos = (pHeader->GetItemsWidth() - nPage);
				}
	
			int nMoveX = 0;
			if( !pHeader->SetLeftOffset(nPos, TRUE) )
				return 0;

			m_nSubItemLeft			= pHeader->m_nLeftItem;
			m_nSubItemLeftOffset	= pHeader->m_nLeftItemOffsetX;

			CRect rcHeader;
			pHeader->GetClientRect(rcHeader);

			CRect rcClient;
			GetClientRect(rcClient);

			RedrawRect(rcClient, TRUE);
			pHeader->RedrawRect(rcHeader, TRUE);
			return abs(nPosOld - nPos);
			}
		case SB_LINERIGHT:{
			GetClientRect(rcClient);
			int	nPosOld = pHeader->GetLeftOffset();
			int nPos	= nPosOld + 5;
			int nPage	= rcClient.Width();
			if( nPos < 0 )
				nPos = 0;
			else
			if( nPos > pHeader->GetItemsWidth() - nPage ){
				nPos = (pHeader->GetItemsWidth() - nPage);
				}
			int nMoveX = 0;
			if( !pHeader->SetLeftOffset(nPos, TRUE) )
				return 0;

			m_nSubItemLeft			= pHeader->m_nLeftItem;
			m_nSubItemLeftOffset	= pHeader->m_nLeftItemOffsetX;

			CRect rcHeader;
			pHeader->GetClientRect(rcHeader);

			CRect rcClient;
			GetClientRect(rcClient);

			RedrawRect(rcClient, TRUE);
			pHeader->RedrawRect(rcHeader, TRUE);
			return abs(nPosOld - nPos);
			}
		case SB_PAGELEFT:{
			GetClientRect(rcClient);
			int	nPosOld = pHeader->GetLeftOffset();
			int nPage	= rcClient.Width();
			int nPos	= nPosOld - nPage;
			if( nPos < 0 )
				nPos = 0;
			else
			if( nPos > pHeader->GetItemsWidth() - nPage ){
				nPos = (pHeader->GetItemsWidth() - nPage);
				}
			int nMoveX = 0;
			if( !pHeader->SetLeftOffset(nPos, TRUE) )
				return 0;

			m_nSubItemLeft			= pHeader->m_nLeftItem;
			m_nSubItemLeftOffset	= pHeader->m_nLeftItemOffsetX;

			CRect rcHeader;
			pHeader->GetClientRect(rcHeader);

			CRect rcClient;
			GetClientRect(rcClient);

			RedrawRect(rcClient, TRUE);
			pHeader->RedrawRect(rcHeader, TRUE);
			return abs(nPosOld - nPos);
			}
	    case SB_PAGERIGHT:{
			GetClientRect(rcClient);
			int	nPosOld = pHeader->GetLeftOffset();
			int nPage	= rcClient.Width();
			int nPos	= nPosOld + nPage;
			int	nPosMax	= pHeader->GetItemsWidth() - nPage;
			if( nPos < 0 )
				nPos = 0;
			else
			if( nPos > nPosMax ){
				nPos = nPosMax;
				}
			int nMoveX = 0;
			if( !pHeader->SetLeftOffset(nPos, TRUE) )
				return 0;

			m_nSubItemLeft			= pHeader->m_nLeftItem;
			m_nSubItemLeftOffset	= pHeader->m_nLeftItemOffsetX;

			CRect rcHeader;
			pHeader->GetClientRect(rcHeader);

			CRect rcClient;
			GetClientRect(rcClient);

			RedrawRect(rcClient, TRUE);
			pHeader->RedrawRect(rcHeader, TRUE);
			return abs(nPosOld + nPos);		 		 
			}
		}
	return TRUE;
	}

LRESULT
ESItemsList::OnSetupVScrollInfo(WPARAM wParam, LPARAM lParam){
	MercuryScrollBar* pVscrollBar = (MercuryScrollBar*)lParam;
	if( !pVscrollBar ) return 0;

	CRect rcClient;
	GetClientRect(rcClient);

	int nPage = rcClient.Height() / m_pGUI->itemsList.m_nItemHeight;
	pVscrollBar->SetScrollInfo(0, GetItemCount(), nPage);
	return 1;
	}

LRESULT
ESItemsList::OnSetupHScrollInfo(WPARAM wParam, LPARAM lParam){
	MercuryScrollBar*	pHscrollBar = (MercuryScrollBar*)lParam;
	ESHeaderCtrl*		pHeader		= m_pRoot->GetHeaderCtrl();
	
	if( !pHscrollBar || !pHeader ) 
		return 0;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcHeader;
	pHeader->GetClientRect(rcHeader);

	int nPage = rcClient.Width();
	pHscrollBar->SetScrollInfo(0, pHeader->GetItemsWidth(), nPage);
	return 1;
	}

BOOL 
ESItemsList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	MercuryScrollBar* pVScroll = m_pRoot->GetVScrollCtrl();
	if( !nFlags && m_pRoot && pVScroll && ::IsWindowVisible(*pVScroll) ){
		int nPosNew = m_nTopItem - 4*(zDelta/WHEEL_DELTA);
		BOOL bRet = OnVScroll(nPosNew, (LPARAM)SB_THUMBTRACK);
		if( bRet ){
			pVScroll->SetScrollPos(m_nTopItem, TRUE);
			}
		}
	return 1;
	}

void
ESItemsList::OnLButtonDblClk(UINT nFlags, CPoint point){
	int nItem = GetSelectedItem();
	if( nItem != -1 ){
		// Notify selection change event to owner. {{
		CWnd* pOwner = GetOwner();
		if( pOwner ){
			pOwner->SendMessage(WM_DBLCLK_LIST_ITEM, (WPARAM)nItem, (LPARAM)GetParent());
			}
		// }}
		}
	ESItemsView::OnLButtonDblClk(nFlags, point);
	}

void
ESItemsList::OnLButtonDown(UINT nFlags, CPoint point){
	int nItem = GetItemByPoint(point);
	if( nItem != -1 ){
		SelectItem(nItem, TRUE, TRUE);
		}
	ESItemsView::OnLButtonDown(nFlags, point);
	}

UINT 
ESItemsList::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

BOOL
ESItemsList::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){
	SetFocus();
	BOOL bRet =  CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	
	/*
	if( m_bActive == FALSE ){
		m_bActive = TRUE;
		RedrawAllItems();
		}
	*/
	return bRet;
	}

void
ESItemsList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	switch(nChar){
		case VK_DOWN:
			GotoItem(GetSelectedItem()+1);
			break;
		case VK_UP:
			GotoItem(GetSelectedItem()-1);
			break;
		case VK_PRIOR: // Page up
			GotoItem(GetSelectedItem()-GetItemsPerPage());
			break;
		case VK_NEXT: // Page down
			GotoItem(GetSelectedItem()+GetItemsPerPage());
			break;
		};

	ESItemsView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

int
ESItemsList::GetSelectedItem(){
	if( !m_items ) return -1;

	CDWordArray arrSelItems;
	m_items->GetSelectedItems(arrSelItems);
	if( arrSelItems.GetCount() == 0 )
		return -1;
	return (int)arrSelItems[0];
	}

BOOL
ESItemsList::CheckItem(int nItem, BOOL bCheck /*= TRUE*/, BOOL bInvalidate /*= TRUE*/){
	return FALSE;
	}

int
ESItemsList::GetItemsPerPage(){
	CRect rcClient;
	GetClientRect(rcClient);
	int nItemsPerPage	= min(m_items->GetItemsCount(), rcClient.Height() / m_pGUI->itemsList.m_nItemHeight);
	return nItemsPerPage;
	}

int
ESItemsList::GetMaxTopItem(){
	int nItemsCt		= m_items->GetItemsCount();
	int nItemsPerPage	= GetItemsPerPage();

	if( nItemsPerPage >= nItemsCt )
		return 0;
	return (nItemsCt - nItemsPerPage);
	}

BOOL
ESItemsList::GetVisibleItems(int& nTop, int& nBottom){
	if( !m_items ) return FALSE;

	CRect rcClient;
	GetClientRect(rcClient);

	int nItemsPerPage = min(m_items->GetItemsCount(), rcClient.Height() / m_pGUI->itemsList.m_nItemHeight);
	if( nItemsPerPage <= 0 ){
		nTop	= 0;
		nBottom = 0;
		return TRUE;
		}

	nTop	= m_nTopItem;
	nBottom = nTop + (nItemsPerPage - 1);
	return TRUE;
	}

void
ESItemsList::SetTopItem(int nTop, BOOL bRedraw){
	int nItemTopMax	= GetMaxTopItem();
	if( nTop < 0 || nTop > nItemTopMax )
		return;
	m_nTopItem = nTop;
	if( bRedraw ){
		CRect rcClient;
		GetClientRect(rcClient);
		RedrawRect(rcClient, TRUE);
		}
	}

BOOL
ESItemsList::GotoItem(int nItem){
	if( !m_items ) return FALSE;

	// Validate item index. ##########
	if( nItem > GetItemCount() )
		nItem = GetItemCount() - 1;
	else
	if( nItem < 0 )
		nItem = 0;
	// ###############################

	if( nItem < 0 || nItem >= m_items->GetItemsCount() ) 
		return FALSE;

	// Is item visible.
	bool bVisible = false;
	int nItemTop = 0, nItemBottom = 0;
	if( !GetVisibleItems(nItemTop, nItemBottom) )
		return FALSE;
	if( nItem >= nItemTop && nItem <= nItemBottom )
		bVisible = true;
	// ################

	if( bVisible ){
		SelectItem(nItem, TRUE, TRUE);
		}
	else{
		// Just only set selected state.
		SelectItem(nItem, TRUE, FALSE);
		// Set Top item. ###################
		int nVisItemsCt = (nItemBottom - nItemTop);
		int nNewTopItem = (nItem - nVisItemsCt);
		if( m_nTopItem > nItem )
			nNewTopItem = nItem;
	//	SetTopItem(nNewTopItem, FALSE);
		// #################################

		// Scroll and redraw. ##############
		BOOL bRet = OnVScroll(nNewTopItem, (LPARAM)SB_THUMBTRACK);
		if( bRet ){
			m_pRoot->GetVScrollCtrl()->SetScrollPos(nNewTopItem, TRUE);
			}
		// #################################
		}
	return TRUE;
	}

BOOL
ESItemsList::SelectItem(int nItem, BOOL bSelect, BOOL bInvalidate /*= TRUE*/){
	if( !m_items ) return FALSE;
	if( nItem < 0 || nItem >= m_items->GetItemsCount() ) return FALSE;

	CDWordArray arrOldSel;
	m_items->GetSelectedItems(arrOldSel);

	CRect rcItemSel(0, 0, 0, 0), rcItemClear(0, 0, 0, 0);
	DWORD dwState = m_items->GetItemState(nItem);
	if( bSelect ){
		if( dwState&ITEM_SELECTED ) return TRUE; // Already selected.
		m_items->SetItemState(nItem, dwState|ITEM_SELECTED);

		m_items->GetItemRect(this, nItem, rcItemSel);
		if( arrOldSel.GetCount() <= 1 ){
			if( arrOldSel.GetCount() > 0 ){
				m_items->GetItemRect(this, arrOldSel[0], rcItemClear);
				// Clear selected state. ############################
				dwState &= ~ITEM_SELECTED;
				m_items->SetItemState(arrOldSel[0], dwState);
				// ##################################################
				}
			if( bInvalidate )
				RedrawRect(rcItemSel|rcItemClear, TRUE);

			// Notify selection change event to owner. {{
			CWnd* pOwner = GetOwner();
			if( pOwner ){
				pOwner->SendMessage(WM_SELCHANGED_LIST_ITEM, (WPARAM)nItem, (LPARAM)GetParent());
				}
			// }}
			}
		else{
			if( (dwState&ITEM_SELECTED) == 0 ) return TRUE; // Not selected.
			m_items->ClearSelectedItems();

			if( bInvalidate ){
				CRect rcClient;
				GetClientRect(rcClient);
				RedrawRect(rcClient, TRUE);
				}
			}
		return TRUE;
		}
	else{
		dwState &= ~ITEM_SELECTED;
		m_items->SetItemState(nItem, dwState);
		}
	return TRUE;
	}

BOOL
ESItemsList::DeleteItem(int nItem){
	return FALSE;
	}

int
ESItemsList::GetItemByPoint(CPoint pt){
	if( !m_items ) return -1;
	return m_items->GetItemByPoint(this, pt);
	}
