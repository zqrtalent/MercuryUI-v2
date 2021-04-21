// ESHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ESListControl.h"
#include "ESHeaderCtrl.h"
#include "ESItemsList.h"
#include "ESMemMan.h"

#include "..\Data\VRowSet.h"
#include "..\Image\ImageHelper.h"
#include "..\UsefulFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESHeaderCtrl

ESHeaderCtrl::ESHeaderCtrl() : MercuryBaseWnd(){
	m_ptMouseDown		.SetPoint(0, 0);
	m_bMouseDown		= FALSE;
	m_nHitTest			= 0;
	m_nItemHover		= -1;
	m_nLeftItemOffsetX	= 0;
	m_nLeftItem			= 0;
	m_pItems			= new ESHeaderItems();
	m_pRowSet			= NULL;

	m_bSizing			= FALSE;
	m_bOverSizingLabel	= FALSE;
	m_bMoving			= FALSE;

	m_nItemsWidth		= 0;
	m_bCalcItemsWidth	= TRUE;

	m_bCalcOffsetX		= TRUE;
	m_nOffsetX			= 0;

	m_bDoubleBufferedDC	= TRUE;
	m_nItemDrag			= -1;
	}

ESHeaderCtrl::~ESHeaderCtrl(){
	m_nLeftItem			= 0;
	m_nItemDrag			= -1;
	delete m_pItems;
	}

BEGIN_MESSAGE_MAP(ESHeaderCtrl, MercuryBaseWnd)
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_SETCURSOR		()
	ON_WM_MOUSEWHEEL	()
	ON_WM_SIZE			()
//	ON_WM_TIMER			()
END_MESSAGE_MAP()

BOOL 
ESHeaderCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/){
	if( RegisterWndClass(_T("ESHeaderCtrl")) ){
		BOOL bRet =  CWnd::Create(_T("ESHeaderCtrl"), _T(""), dwStyle, rect, pParentWnd, nID, pContext);
		return bRet;
		}
	return FALSE;
	}

BOOL
ESHeaderCtrl::OnParentResized(int cx, int cy){
	MoveWindow(0, 0, cx, m_pGUI->header.m_nHeight, TRUE);
	return TRUE;
	}

void
ESHeaderCtrl::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	CRect	rcClient;
	GetClientRect(rcClient);
	if( rcClient.IsRectEmpty() || pRectClient->IsRectEmpty() ) 
		return;

	CRect	rcDestDC; // Full destination rect in DC coordinates.
	if( UseDoubleBufferedDC() == TRUE ){
		/*
		rcDestDC.left	= pRectDC->left - pRectClient->left;
		rcDestDC.top	= pRectDC->top - pRectClient->top;
		rcDestDC.right	= rcDestDC.left + pRectClient->Width();
		rcDestDC.bottom	= rcDestDC.top + pRectClient->Height();
		*/

		rcDestDC = *pRectDC;

		// Old code
		//rcDestDC.right	= rcDestDC.left + rcClient.Width();
		//rcDestDC.bottom	= rcDestDC.top + rcClient.Height();
		}
	else
		rcDestDC = *pRectDC;

	int				nItem		= m_nLeftItem;
	ESHeaderItem*	pItem		= m_pItems->GetItem(nItem);
	int				nDragItem	= GetDragItemIndex();
	int				nOffsetX	= 0;

	CRect rcItem;
	rcItem.left		= rcClient.left;
	rcItem.top		= rcClient.top;
	rcItem.right	= rcItem.left + pItem ? pItem->nWidth : 0;
	rcItem.bottom	= rcClient.bottom;

	CImage *pImg1 = NULL, *pImg2 = NULL;
	CBitmap bmDiv, bmBkgnd;
	CString sItemText;
	CSize	szText(0, 0);

	CRect	rcLeftBkgnd(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
	while( pItem && rcItem.left < rcClient.right ){
		// Translate item rect in destination DC coordinates.
		/*
		CRect rcDestItemDC;
		rcDestItemDC.left	= rcItem.left - m_nLeftItemOffsetX;
		rcDestItemDC.top	= rcItem.top;
		rcDestItemDC.right	= rcDestItemDC.left + rcItem.Width();
		rcDestItemDC.bottom = rcDestItemDC.top + rcItem.Height();
		*/

		CRect rcDestItemDC;
		rcDestItemDC.left	= pRectDC->left + ((rcItem.left - m_nLeftItemOffsetX) - pRectClient->left);
		rcDestItemDC.top	= pRectDC->top + (rcItem.top - pRectClient->top);
		rcDestItemDC.right	= rcDestItemDC.left + rcItem.Width();
		rcDestItemDC.bottom = rcDestItemDC.top + rcItem.Height();
		// ##################################################

		nOffsetX			= rcDestItemDC.left;			
		rcDestItemDC		= rcDestDC&rcDestItemDC;
		if( rcDestItemDC.IsRectEmpty() ){
			// Move to next item. ##########################
			rcItem.left		+= pItem->nWidth;
			pItem = pItem->next;
			if( !pItem ) break;
			rcItem.right	+= pItem->nWidth;
			nItem			++;
			continue;
			// #############################################
			}

		rcLeftBkgnd.left	= rcDestItemDC.right;
		// Determine item images by state. ##################
		if( pItem->dwState == HDR_ITEM_SELECTED ){
			pImg1 = &m_pGUI->header.m_imageItem_sel;
			pImg2 = &m_pGUI->header.m_imageItemDiv_sel;
			}
		else
		if( pItem->dwState == HDR_ITEM_NORMAL ){
			pImg1 = &m_pGUI->header.m_imageItem_normal;
			pImg2 = &m_pGUI->header.m_imageItemDiv_normal;
			}
		else{
			// Move to next item. ############
			nItem		++;
			rcItem.left	+= pItem->nWidth;
			pItem		= pItem->next;
			if( pItem )			
				rcItem.right	= rcItem.left + pItem->nWidth;
			else break;
			continue;
			// ###############################
			}
		// ###################################################

		// Draw dragging item. ###############################
		if( nItem == nDragItem ){
			CBrush brush(m_pGUI->header.m_crDragItemBkgnd);
			pDC->FillRect(rcDestItemDC, &brush);
			brush.DeleteObject();

			// Move to next item. ############
			nItem		++;
			rcItem.left	+= pItem->nWidth;
			pItem		= pItem->next;
			if( pItem )		
				rcItem.right	= rcItem.left + pItem->nWidth;
			else break;
			continue;
			// ###############################
			}
		// ###################################################

		// Draw item background. #############################
		CRect rcSrc(0, 0, pImg1->GetWidth(), rcDestItemDC.Height());
		ImgHlp::DrawImage(pDC, pImg1, rcSrc, rcDestItemDC); 
		// ###################################################
		
		// Draw item divider. ################################
		int nDividerWidth = 1;
		if( rcDestItemDC.right  >= (nOffsetX + pItem->nWidth) - nDividerWidth ){
			nDividerWidth = rcDestItemDC.right - ((nOffsetX + pItem->nWidth) - nDividerWidth);
			if( nDividerWidth > 0 ){
				CRect rcDivider(rcDestItemDC.right - nDividerWidth, rcDestItemDC.top, rcDestItemDC.right, rcDestItemDC.bottom);
				rcSrc.SetRect(0, rcDivider.top, nDividerWidth, rcDivider.bottom);
				ImgHlp::DrawImage(pDC, pImg2, rcSrc, rcDivider);
				}
			}
		// ###################################################

		// Draw item text. ###################################
		if( pItem->sText.GetLength() > 0 ){
			sItemText		= pItem->sText;
			CFont* pOldFont = pDC->SelectObject(&m_pGUI->header.m_fontItemText);
			szText			= pDC->GetTextExtent(sItemText.GetBuffer(), sItemText.GetLength());

			CPoint posText(nOffsetX + 2, rcDestItemDC.top);
			if( pItem->dwTextAlignH == TEXT_HALIGN_MIDDLE ){
				posText.x = nOffsetX + (rcItem.Width() - szText.cx)/2;
				}
			else
			if( pItem->dwTextAlignH == TEXT_HALIGN_RIGHT ){
				posText.x = (nOffsetX + rcItem.Width() - szText.cx - 2);
				}
			else
				posText.x += 2;
			posText.y = rcDestItemDC.top + (rcItem.Height() - szText.cy)/2;

			pDC->SetBkMode		(TRANSPARENT);
			pDC->ExtTextOut		(posText.x, posText.y, ETO_CLIPPED, &rcDestItemDC, sItemText, NULL);
			pDC->SelectObject	(pOldFont);
			}
		// ###################################################

		// Move to next item. ################################
		nItem		++;
		rcItem.left	+= pItem->nWidth;
		pItem		= pItem->next;
		if( pItem ){			
			rcItem.right	= rcItem.left + pItem->nWidth;
			}
		// ###################################################
		}
	
	if( UseDoubleBufferedDC() == FALSE ){
		rcLeftBkgnd &= rcDestDC;
		}

	if( !rcLeftBkgnd.IsRectEmpty() ){
		pImg1 = &m_pGUI->header.m_imageItem_normal;
		CRect rcSrc(0, rcLeftBkgnd.top, pImg1->GetWidth(), rcLeftBkgnd.bottom);
		ImgHlp::DrawImage(pDC, pImg1, rcSrc, rcLeftBkgnd); 
		}
	}

void 
ESHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point){
	// Save down mouse point. ####
	m_bMouseDown = TRUE;
	if( m_bOverSizingLabel ){
		SetCapture();
		m_ptMouseDown	= point;
		m_bSizing		= TRUE;
		}
	// ###########################
	CWnd::OnLButtonDown(nFlags, point);
	}

void 
ESHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point){
	// Release capture and cancel sizing.
	if( m_bMouseDown ){
		ReleaseCapture();
		m_bMouseDown = FALSE;
		if( m_bOverSizingLabel ){
			m_bSizing = FALSE;
			}

		// Stop drag item. ########
		if( ItemDragging() ){
			EndDragItem();
			}
		// ########################
		}
	// #################################
	CWnd::OnLButtonUp(nFlags, point);
	}

void 
ESHeaderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point){
	OnLButtonDown(nFlags, point);
	CWnd::OnLButtonDblClk(nFlags, point);
	}

void 
ESHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point){
	TRACE1("mousemove %d\r\n", point.x);
	
	CRect	rcItem(0, 0, 0, 0);
	// Process item sizing. ########## 
	if( m_bSizing ){
		if( (point.x - m_ptMouseDown.x) != 0 && m_nItemHover > -1 ){
			ESHeaderItem*	pItemSize	= m_pItems->GetItem(m_nItemHover);
			int				nSizeNew	= pItemSize->nWidth + (point.x - m_ptMouseDown.x);
			int				nSizeOld	= pItemSize->nWidth;
			m_ptMouseDown.x				= point.x;

			if( m_pRoot->CanSizeHeaderItem(m_nItemHover, nSizeNew) && 
				m_pItems->GetItemRect(this, m_nItemHover, rcItem) ){
				pItemSize->nWidth = nSizeNew;
				// Send notify to root window.
				m_pRoot->OnSizeHeaderItem(m_nItemHover, nSizeNew, nSizeOld);
				}
			}
		MercuryBaseWnd::OnMouseMove(nFlags, point);
		return;
		}
	// ###############################

	// Do item dragging. ###############
	if( m_bMouseDown && nFlags&MK_LBUTTON && !m_bSizing ){
		if( !ItemDragging() ){
			int	nItem = m_pItems->ItemByPoint(this, point, &rcItem);
			if( nItem > -1 )
				StartDragItem(nItem);
			}
		else{
			OnDragItem(point);
			}

		MercuryBaseWnd::OnMouseMove(nFlags, point);
		return;
		}
	// ###############################
	
	int	nItem = m_pItems->ItemByPoint(this, point, &rcItem);
	// Check for size cursor visibility.
	m_bOverSizingLabel = FALSE;
	if( !rcItem.IsRectEmpty() ){
		if( point.x >= (rcItem.right - 3) ){
			m_bOverSizingLabel = TRUE;
			}
		}
	// ################################

	m_nItemHover = nItem;
	MercuryBaseWnd::OnMouseMove(nFlags, point);
	}

void
ESHeaderCtrl::OnTimer(UINT_PTR nIDEvent){
	CWnd::OnTimer(nIDEvent);
	}

BOOL 
ESHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	if( m_bOverSizingLabel ){
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		return TRUE;
		}
	else
	if( ItemDragging() ){
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
		return TRUE;
		}
	else
	if( m_nItemHover != -1 ){
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return TRUE;
		}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL 
ESHeaderCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	return CWnd::OnMouseWheel(nFlags,  zDelta, pt);
	}

void 
ESHeaderCtrl::OnSize(UINT nType, int cx, int cy){
	MercuryBaseWnd::OnSize(nType, cx, cy);
	}

// #######################################
//	Operations
// #######################################

int
ESHeaderCtrl::AddItem(TCHAR* pszText, TCHAR* pszField, DWORD dwTextAlignH, int nWidth, int nWidthMin, int nImage, int nSelImage){
	return InsertItem(m_pItems->GetItemCount(), pszText, pszField, dwTextAlignH, nWidth, 
					 nWidthMin, nImage, nSelImage);
	}
int
ESHeaderCtrl::InsertItem(int nInsertAt, TCHAR* pszText, TCHAR* pszField, DWORD dwTextAlignH, int nWidth, int nWidthMin, int nImage, int nSelImage){
	m_pItems->GotoItem(nInsertAt - 1, FALSE);
	if( !m_pItems->AddItem(pszText) ) return -1;

	ESHeaderItem* pItemNew = m_pItems->GetCurrentItem();
	ASSERT( pItemNew );

	pItemNew->dwTextAlignH	= dwTextAlignH;
	pItemNew->nWidth		= nWidth;
	pItemNew->nMinWidth		= nWidthMin;
	pItemNew->nSelImage		= nSelImage;
	pItemNew->nImage		= nImage;
	pItemNew->sField		= pszField;

	if( m_pRowSet ){
		pItemNew->nFieldId = m_pRowSet->GetOrdinalIndex(pszField);
		}
	else
		pItemNew->nFieldId = -1;

	m_bCalcItemsWidth = TRUE;
	return nInsertAt;
	}

int
ESHeaderCtrl::RemoveItem(int nIndex){
	return -1;
	}

void
ESHeaderCtrl::RemoveAllItems(){
	m_bCalcItemsWidth	= TRUE;
	m_nLeftItem			= 0;
	m_pItems->SetItemCount(0);
	}

// #######################################
//	Attributes
// #######################################

void
ESHeaderCtrl::SetItemCount(int nCount){
	if( m_pItems->SetItemCount(nCount) ){
		m_bCalcItemsWidth = TRUE;
		RedrawAllItems();
		}
	}

int
ESHeaderCtrl::GetItemWidthMin(int nItem){
	if( !m_pItems ) return 0;
	ESHeaderItem* pHdrItem = m_pItems->GetItem(nItem);
	if( !pHdrItem ) return 0;
	return pHdrItem->nMinWidth;
	}

int
ESHeaderCtrl::GetItemWidth(int nItem){
	if( !m_pItems ) return 0;
	ESHeaderItem* pHdrItem = m_pItems->GetItem(nItem);
	if( !pHdrItem ) return 0;
	return pHdrItem->nWidth;
	}

int
ESHeaderCtrl::GetItemHAlign(int nItem){
	if( !m_pItems ) return 0;
	ESHeaderItem* pHdrItem = m_pItems->GetItem(nItem);
	if( !pHdrItem ) return 0;
	return pHdrItem->dwTextAlignH;
	}

int
ESHeaderCtrl::GetItemCount(){
	return m_pItems ? m_pItems->GetItemCount() : 0;
	}

// #######################################
//	Attributes
// #######################################

BOOL
ESHeaderCtrl::RedrawItem(int nItem){
	CRect rcItem(0, 0, 0, 0);
	if( m_pItems->GetItemRect(this, nItem, rcItem) ){
		RedrawRect(rcItem, TRUE);
		return TRUE;
		}
	return FALSE;
	}

void
ESHeaderCtrl::RedrawAllItems(){
	CRect rcClient;
	GetClientRect(rcClient);
	RedrawRect(rcClient, TRUE);
	}

int
ESHeaderCtrl::GetItemsWidth(){
	if( !m_pItems ) 
		return 0;
	if( m_bCalcItemsWidth ){
		int				nWidth	= 0;
		int				nLoop	= 0;
		ESHeaderItem*	pItem	= m_pItems->GetItem(nLoop);
		while(pItem){
			nWidth += pItem->nWidth;
			nLoop ++;
			pItem = m_pItems->GetItem(nLoop);
			}

		m_bCalcItemsWidth	= FALSE;
		m_nItemsWidth		= nWidth;
		return nWidth;
		}
	return m_nItemsWidth;
	}

void
ESHeaderCtrl::GetItemRect(int nItem, CRect& rcItem){
	if( !m_pItems ){
		rcItem.SetRect(0, 0, 0, 0);
		return;
		}
	m_pItems->GetItemRect(this, nItem, rcItem);
	}

int
ESHeaderCtrl::GetLeftOffset(){
	if( m_bCalcOffsetX && m_pItems ){
		m_nOffsetX	= m_nLeftItemOffsetX;
		int nItem	= m_nLeftItem - 1;

		while( nItem > 0 ){
			m_nOffsetX += m_pItems->GetItemWidth(nItem--);
			}
		m_bCalcOffsetX = FALSE;
		}
	return m_nOffsetX;
	}

int
ESHeaderCtrl::SetLeftOffset(int nOffset, BOOL bRedraw){
	int				nOffsetOld	= GetLeftOffset();
	int				nItem		= m_nLeftItem;
	ESHeaderItem*	pItem		= m_pItems->GetItem(nItem);
	int				nMoveX		= (nOffset - nOffsetOld);

	if( !nMoveX )
		return 0;

	if( nMoveX > 0 ){
		if( (nMoveX + m_nLeftItemOffsetX) >= pItem->nWidth ){
			nMoveX		-= (pItem->nWidth - m_nLeftItemOffsetX);
			pItem		= m_pItems->GetItem(++nItem);
			while( pItem && nMoveX >= pItem->nWidth ){
				nMoveX -= pItem->nWidth;
				pItem	= m_pItems->GetItem(++nItem);
				}
			m_nLeftItem			= nItem;
			m_nLeftItemOffsetX  = nMoveX;
			}
		else // Item index remains the same.
			m_nLeftItemOffsetX += nMoveX;

		m_nOffsetX	= nOffset;
		return (nOffset - nOffsetOld);
		}
	else{
		if( (m_nLeftItemOffsetX + nMoveX) < 0 ){
			nMoveX		= abs(nMoveX);
			nMoveX		-= m_nLeftItemOffsetX;
			pItem		= m_pItems->GetItem(--nItem);

			while( pItem && nMoveX >= pItem->nWidth ){
				nMoveX -= pItem->nWidth;
				pItem	= m_pItems->GetItem(--nItem);
				}
		
			if( !nMoveX ) nItem ++;
			m_nLeftItem			= nItem;
			m_nLeftItemOffsetX  = pItem ? (pItem->nWidth - nMoveX) : 0;
			}
		else // Item index remains the same.
			m_nLeftItemOffsetX += nMoveX;

		m_nOffsetX	= nOffset;
		return (nOffset - nOffsetOld);
		}
	return 0;
	}

void
ESHeaderCtrl::RedrawColumn(int nCol){
	if( !m_pItems ) return;

	CRect rcItem, rcClient;
	if( !m_pItems->GetItemRect(this, nCol, rcItem) )
		return;

	ESItemsList* pListView = ((ESItemsList*)m_pRoot->m_pView);
	// Invalidate column. ###########
	CRect rcInvalidHeader;
	GetClientRect(rcClient);
	rcInvalidHeader.SetRect(rcItem.left, rcClient.top, rcItem.right, rcClient.bottom);
	RedrawRect(rcInvalidHeader, TRUE);

	CRect rcInvalidListView;
	pListView->GetClientRect(rcClient);
	rcInvalidHeader.SetRect(rcItem.left, rcClient.top, rcItem.right, rcClient.bottom);
	pListView->RedrawRect(rcInvalidHeader, TRUE);
	// ###############################
	}

void
ESHeaderCtrl::SetRowSet(VirtualRowSet* pRowSet){
	if( !pRowSet ){
		m_pRowSet = NULL;
		return;
		}
	// Set field ids. ##############
	ESHeaderItem* pItem = m_pItems->GetItem(0);
	while( pItem ){
		pItem->nFieldId = pRowSet->GetOrdinalIndex(pItem->sField.GetBuffer());
		pItem = pItem->next;
		}
	// #############################
	m_pRowSet = pRowSet;
	}

int
ESHeaderCtrl::GetFieldId(int nItem){
	if( !m_pItems ) return -1;
	ESHeaderItem* pItem = m_pItems->GetItem(nItem);
	if( !pItem ) return -1;
	return pItem->nFieldId;
	}

///////////////////////////////////////////////////////////////////
//
//	Item dragging
//
///////////////////////////////////////////////////////////////////

int
ESHeaderCtrl::GetDragItemIndex(){
	if( !m_pItems ) return -1;
	return m_nItemDrag;
	}

void
ESHeaderCtrl::SetDragItemIndex(int nDragItem){
	if( !m_pItems ) return;
	m_nItemDrag = nDragItem;
	}

BOOL
ESHeaderCtrl::ItemDragging(){
	return (GetDragItemIndex() != -1);
	}

BOOL
ESHeaderCtrl::StartDragItem(int nItem){
	if( ItemDragging() ) 
		return FALSE;

	CRect rcItem;
	if( !m_pItems->GetItemRect(this, nItem, rcItem) )
		return FALSE;

	int nImgCX = m_pItems->GetItemWidth(nItem), nImgCY = 200;
	m_szDragImage.SetSize(nImgCX, nImgCY);

	CBitmap bmImage;
	if( !ImgHlp::CreateBitmap32bpp(bmImage, RGB(0,0,0), nImgCX, nImgCY) )
		return FALSE;

	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(&bmImage);

	CRect rcDC(0, 0, nImgCX, nImgCY), rcClient = rcItem;
	int nTemp			= m_nLeftItemOffsetX;
	m_nLeftItemOffsetX	= 0;
	// Draw header item on dc.
	OnPaintClient(&memDC, &rcDC, &rcClient);
	// #######################
	// Draw list items on dc.
	ESItemsList* pListView = ((ESItemsList*)m_pRoot->m_pView);
	rcClient.SetRect(rcItem.left, 0, rcItem.right, (nImgCY - rcItem.Height()));
	rcDC.SetRect(0, rcItem.Height(), nImgCX, nImgCY);
	pListView->OnPaintClient(&memDC, &rcDC, &rcClient);	
	// #######################
	m_nLeftItemOffsetX	= nTemp;

	HIMAGELIST hImgList = m_dragImageList.Detach();
	if( hImgList != NULL ){
		m_dragImageList.DragLeave	(m_pRoot);
		m_dragImageList.EndDrag		();
		// Destroy image list.
		ImageList_Destroy			(hImgList);
		}

	// Prepare image list for dragging. ###################
	m_dragImageList.Create		(nImgCX, nImgCY, ILC_COLOR32|ILC_MASK, 1, RGB(255, 255, 255));
	m_dragImageList.Add			(&bmImage, RGB(255, 255, 255));
	IMAGEINFO imgInfo;
	m_dragImageList.GetImageInfo(0, &imgInfo);
	ImgHlp::BitmapOnBitmap		(imgInfo.hbmImage, 0, 0, bmImage, 0, 0, 
								nImgCX, nImgCY, nImgCX, nImgCY, FALSE);
	// ####################################################

	CPoint pt;
	::GetCursorPos(&pt);
	
	CPoint ptDrag = pt;
	::ScreenToClient(m_hWnd, &ptDrag);

	ptDrag.x -= rcItem.left; 
	ptDrag.y -= rcItem.top; 
	// Save drag point in drag image coordinates.
	m_ptDragImage = ptDrag;
	::ScreenToClient(*m_pRoot, &pt);

	if( m_dragImageList.BeginDrag(0, ptDrag) ){
		SetDragItemIndex(nItem);
		// Redraw column.
		RedrawColumn(nItem);
		m_dragImageList.DragEnter(m_pRoot, pt);
		}

	memDC	.DeleteDC();
	bmImage	.DeleteObject();
	SetCapture();
	return TRUE;
	}

BOOL
ESHeaderCtrl::EndDragItem(){
	if( !ItemDragging() ) return FALSE;

	m_dragImageList.DragLeave	(m_pRoot);
	m_dragImageList.EndDrag		();

	int nItem = GetDragItemIndex();
	CRect rcItem;
	m_pItems->GetItemRect(this, nItem, rcItem);
	ESItemsList* pListView = ((ESItemsList*)m_pRoot->m_pView);

	// Destroy image list.
	ImageList_Destroy		(m_dragImageList.Detach());
	// Set drag item index to -1.
	SetDragItemIndex		(-1);
	// Redraw column.
	RedrawColumn			(nItem);

	ReleaseCapture();
	return TRUE;
	} 

void
ESHeaderCtrl::OnDragItem(CPoint pt){
	if( !ItemDragging() ) return;

	CRect rcItemDrag(0, 0, 0, 0);
	m_pItems->GetItemRect(this, m_nItemDrag, rcItemDrag);

	if( pt.x > (rcItemDrag.left + m_ptDragImage.x) ){
		CPoint	ptNextItem;
		ptNextItem.y = rcItemDrag.top;
		ptNextItem.x = pt.x + (m_szDragImage.cx - m_ptDragImage.x) - GetItemWidth(m_nItemDrag + 1)/2;

		int	nItemNew = m_pItems->ItemByPoint(this, ptNextItem);
		if( nItemNew > m_nItemDrag && nItemNew > -1 ){
			CImageList::DragShowNolock	(FALSE);
			int nItemDrag = m_nItemDrag;
			// Set drag item index.
			SetDragItemIndex		(nItemNew);
			// Reorder items.
			m_pItems->ReorderItems	(nItemDrag, nItemNew);

			// Redraw recently dragged column.
			RedrawColumn			(nItemDrag);
			// Redraw dragged column.
			RedrawColumn			(nItemNew);
			}
		}
	else
	if( pt.x < (rcItemDrag.left + m_ptDragImage.x) ){
		CPoint	ptPrevItem;
		ptPrevItem.y = rcItemDrag.top;
		ptPrevItem.x = pt.x - m_ptDragImage.x + GetItemWidth(m_nItemDrag - 1)/2;

		int	nItemNew = m_pItems->ItemByPoint(this, ptPrevItem);
		if( nItemNew < m_nItemDrag && nItemNew > -1 ){
			CImageList::DragShowNolock	(FALSE);
			int nItemDrag = m_nItemDrag;
			// Set drag item index.
			SetDragItemIndex		(nItemNew);
			// Reorder items.
			m_pItems->ReorderItems	(nItemDrag, nItemNew);
			// Redraw column dragged column.
			RedrawColumn			(nItemDrag);
			// Redraw column reordered.
			RedrawColumn			(nItemNew);
			}
		}

	/* drag the item to the current position */
	GetCursorPos(&pt);
	::ScreenToClient(*m_pRoot, &pt);

	CImageList::DragMove		(pt);
	CImageList::DragShowNolock	(TRUE);
	}