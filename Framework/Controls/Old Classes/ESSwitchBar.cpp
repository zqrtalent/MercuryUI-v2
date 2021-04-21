// ESSwitchBar.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ESItemContainer.h"
#include "ESSwitchBar.h"

#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESSwitchBar

ESSwitchBar::ESSwitchBar() : MercuryBaseWnd(){
/*
	SwitchBarItem* pItem	= new SwitchBarItem;
	pItem->next				= NULL;
	pItem->prev				= NULL;
	pItem->sItemText		= _T("ABCDEFGHIJKLM");

	SwitchBarItem* pItem1	= new SwitchBarItem;
	pItem1->next				= NULL;
	pItem1->prev				= NULL;
	pItem1->sItemText		= _T("ABCDEFGHIJKLM");

	SwitchBarItem* pItem2	= new SwitchBarItem;
	pItem2->next				= NULL;
	pItem2->prev				= NULL;
	pItem2->sItemText		= _T("ABCDEFGHIJKLM");

	SwitchBarItem* pItem3	= new SwitchBarItem;
	pItem3->next				= NULL;
	pItem3->prev				= NULL;
	pItem3->sItemText		= _T("ABCDEFGHIJKLM");

	m_items.Add(pItem);
	m_items.Add(pItem1);
	m_items.Add(pItem2);
	m_items.Add(pItem3);*/

	m_bSwitchable			= true;
	m_nActiveItem			= -1;

	m_rcItemsFull.SetRect(0, 0, 0, 0);
	m_nItemFocus			= -1;
	m_bOverTheItems			= FALSE;
	m_bMouseDown			= FALSE;
	}

ESSwitchBar::~ESSwitchBar(){
	m_rgnOutOfItems.DeleteObject();
	}

BEGIN_MESSAGE_MAP(ESSwitchBar, MercuryBaseWnd)
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
ESSwitchBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/){
	if( RegisterWndClass(_T("ESSwitchBar")) ){
		BOOL bRet =  CWnd::Create(_T("ESSwitchBar"), _T(""), dwStyle, rect, pParentWnd, nID, pContext);
		return bRet;
		}
	return FALSE;
	}

BOOL
ESSwitchBar::OnParentResized(int cx, int cy){
//	MoveWindow(0, 0, cx, m_pGUI->header.m_nHeight, TRUE);
	return TRUE;
	}

void
ESSwitchBar::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	CRect	rcClient;
	GetClientRect(rcClient);
	if( rcClient.IsRectEmpty() || pRectClient->IsRectEmpty() ) 
		return;

	int		nWidth	= 0.98f * rcClient.Width();
	int		nHeight	= m_pGUI->switchBar.m_imgItemActive.GetHeight();

	CRect rcItems;
	rcItems.left	= (rcClient.Width() - nWidth) / 2;
	rcItems.top		= (rcClient.Height() - nHeight) / 2;
	rcItems.right	= rcItems.left + nWidth;
	rcItems.bottom	= rcItems.top + nHeight;
	m_rcItemsFull	= rcItems;

	CRect rcOmitBorder	= m_pGUI->switchBar.m_rcOmitBorder;
	if( !m_rgnOutOfItems.m_hObject ){ // Create region to fill outside of items.
		CRgn rgn;
		rgn.CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

		POINT pts[] = {
		{rcItems.left, rcItems.top + rcOmitBorder.top}, 
		{rcItems.left + rcOmitBorder.left, rcItems.top}, 
		{rcItems.right - rcOmitBorder.right, rcItems.top}, 
		{rcItems.right, rcItems.top + rcOmitBorder.top},
		{rcItems.right, rcItems.bottom - rcOmitBorder.bottom},
		{rcItems.right - rcOmitBorder.right, rcItems.bottom},
		{rcItems.left + rcOmitBorder.left, rcItems.bottom},
		{rcItems.left, rcItems.bottom - rcOmitBorder.bottom}
		};

		CRgn rgnItems;
		rgnItems.CreatePolygonRgn(pts, sizeof(pts)/sizeof(POINT), WINDING);

		m_rgnOutOfItems.CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		m_rgnOutOfItems.CombineRgn(&rgn, &rgnItems, RGN_DIFF);
		}

	// Draw background.
	CBrush brush(m_pGUI->switchBar.m_crBkgnd);
	pDC->FillRgn(&m_rgnOutOfItems, &brush);
	
	CImage* pImg1	= &m_pGUI->switchBar.m_imgItemNormal;
	CImage* pImg2	= &m_pGUI->switchBar.m_imgItemActive;
	CImage* pImg	= pImg1;
	
	CRect rcSrcLeftRound	(0, 0, rcOmitBorder.left, nHeight);
	CRect rcSrcRightRound	((pImg1->GetWidth() - rcOmitBorder.right), 0, pImg1->GetWidth(), nHeight);
	CRect rcSrcMiddle		(rcSrcLeftRound.right, rcSrcLeftRound.top, rcSrcRightRound.left, rcSrcRightRound.bottom);

	CRect rcLeftRound		(rcItems.left, rcItems.top, rcItems.left + rcOmitBorder.left, rcItems.bottom);
	CRect rcRightRound		(rcItems.right - rcOmitBorder.left, rcItems.top, rcItems.right, rcItems.bottom);
	CRect rcMiddle			(rcLeftRound.right, rcLeftRound.top, rcRightRound.left, rcRightRound.bottom);

	int		nItemCt		= m_items.GetCount();
	int		nItemWidth	= nWidth / nItemCt;
	int		nLoop		= 0;
	m_nItemWidth		= nItemWidth;

	CPen	penNormal(PS_SOLID, 1, m_pGUI->switchBar.m_crDividerNormal);
	CPen	penActive(PS_SOLID, 1, m_pGUI->switchBar.m_crDividerActive);

	CFont*	pFontNormal = &m_pGUI->switchBar.m_fontItemTextNormal;
	CFont*	pFontActive = &m_pGUI->switchBar.m_fontItemTextActive;

	CRect	rcItem		(rcItems.left, rcItems.top, rcItems.left + nItemWidth - 1, rcItems.bottom);
	CSize	szText		(0, 0);
	BOOL	bDrawDivider = TRUE;

	while( nLoop < nItemCt ){
		SwitchBarItem* pItem = m_items[nLoop];
		if( m_bSwitchable ){
			pImg = ((nLoop == m_nActiveItem || nLoop == m_nItemFocus) ? pImg2 : pImg1);
			}
		else
			pImg = ((nLoop == m_nItemFocus) ? pImg2 : pImg1);

		if( !nLoop ){ // First item.
			ImgHlp::DrawAlphaImage(pDC, *pImg, rcSrcLeftRound.left, rcSrcLeftRound.top, rcSrcLeftRound.Width(), rcSrcLeftRound.Height(), rcLeftRound);
			rcItem.left		+= rcLeftRound.Width();
			rcItem.right	+= rcLeftRound.Width();
			}

		if( nLoop == (nItemCt - 1) ){ // Last Item
			rcItem.right = rcRightRound.left;
			ImgHlp::DrawAlphaImage(pDC, *pImg, rcSrcRightRound.left, rcSrcRightRound.top, rcSrcRightRound.Width(), rcSrcRightRound.Height(), rcRightRound);
			bDrawDivider = FALSE;
			}

		// Draw item background.
		ImgHlp::DrawImage(pDC, pImg, rcSrcMiddle, rcItem);
		// Draw items's divider line.
		if( bDrawDivider ){
			CPen* penOld = pDC->SelectObject(&penActive);
			pDC->MoveTo(rcItem.right, rcItem.top);
			pDC->LineTo(rcItem.right, rcItem.bottom);
			pDC->SelectObject(*penOld);
			}

		// Calculate text's size.
		pDC->SelectObject((nLoop == m_nActiveItem ? pFontActive : pFontNormal));
		szText = pDC->GetTextExtent(pItem->sItemText);
		// Calculate text draw position.
		CPoint ptText;
		ptText.x = rcItem.left + (rcItem.Width() - szText.cx)/2;
		ptText.y = rcItem.top + (rcItem.Height() - szText.cy)/2;
		pDC->SetBkMode(TRANSPARENT);
		pDC->ExtTextOut(ptText.x, ptText.y, ETO_CLIPPED, rcItem, pItem->sItemText, pItem->sItemText.GetLength(), NULL);

		rcItem.left		+= nItemWidth;
		rcItem.right	+= nItemWidth;
		nLoop			++;
		}
	}

void 
ESSwitchBar::OnLButtonDown(UINT nFlags, CPoint point){
	if( m_bOverTheItems ){
		CRect rcItem;
		int nItem = GetItemIndexByPoint(point, rcItem);
		if( nItem > -1 && m_nActiveItem != nItem){
			m_nItemFocus = nItem;
			RedrawRect(rcItem, TRUE);
			SetCapture();
			m_bMouseDown = TRUE;
			}
		}
	CWnd::OnLButtonDown(nFlags, point);
	}

void 
ESSwitchBar::OnLButtonUp(UINT nFlags, CPoint point){
	if( m_bMouseDown && m_nItemFocus > -1 ){
		CWnd* pParent = GetParent();
		if( pParent ){
			pParent->SendMessage(WM_SWITCHITEM_CLICKED, (WPARAM)this, (LPARAM)GetItem(m_nItemFocus));
			}

		if( m_bSwitchable ){
			m_nActiveItem	= m_nItemFocus;
			m_nItemFocus	= -1;
			RedrawItem		(m_nActiveItem);
			}
		else{
			int nItemRedraw = m_nItemFocus;
			m_nActiveItem	= -1;
			m_nItemFocus	= -1;
			RedrawItem		(nItemRedraw);
			}
		
		ReleaseCapture	();
		m_bMouseDown = FALSE;
		}

	CWnd::OnLButtonUp(nFlags, point);
	}

void 
ESSwitchBar::OnLButtonDblClk(UINT nFlags, CPoint point){
	OnLButtonDown(nFlags, point);
	CWnd::OnLButtonDblClk(nFlags, point);
	}

void 
ESSwitchBar::OnMouseMove(UINT nFlags, CPoint point){
	if( m_rcItemsFull.PtInRect(point) && 
		m_rgnOutOfItems.m_hObject && 
		!m_rgnOutOfItems.PtInRegion(point) ){
		m_bOverTheItems = TRUE;
		}
	else{
		if( m_bMouseDown ){
			m_nItemFocus = -1;
			RedrawItem		(m_nActiveItem);
			ReleaseCapture	();
			m_bMouseDown = FALSE;
			}
		m_bOverTheItems = FALSE;
		}
	MercuryBaseWnd::OnMouseMove(nFlags, point);
	}

void
ESSwitchBar::OnTimer(UINT_PTR nIDEvent){
	CWnd::OnTimer(nIDEvent);
	}

BOOL 
ESSwitchBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	if( m_bOverTheItems ){
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
		}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL 
ESSwitchBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	return CWnd::OnMouseWheel(nFlags,  zDelta, pt);
	}

void 
ESSwitchBar::OnSize(UINT nType, int cx, int cy){
	if( m_rgnOutOfItems.m_hObject ){
		m_rgnOutOfItems.DeleteObject();
		m_rgnOutOfItems.m_hObject = NULL;
		}
	MercuryBaseWnd::OnSize(nType, cx, cy);
	}

int
ESSwitchBar::GetItemIndexByPoint(CPoint pt, CRect& rcItem)
	{
	if( m_rcItemsFull.PtInRect(pt) ){
		int nItemResult = (pt.x - m_rcItemsFull.left) / m_nItemWidth;
		GetItemRect(nItemResult, rcItem);
		return nItemResult;
		}
	return -1;
	}

BOOL
ESSwitchBar::GetItemRect(int nItem, CRect& rcItem){
	if( nItem < 0 || nItem >= m_items.GetCount() )
		return FALSE;

	rcItem.SetRect(0, m_rcItemsFull.top, 0, m_rcItemsFull.bottom);
	rcItem.left		= m_rcItemsFull.left + (m_nItemWidth*nItem);
	rcItem.right	= rcItem.left + m_nItemWidth;
	return TRUE;
	}

bool
ESSwitchBar::EnableItem(int nItem, bool bEnable, bool bRedraw /*= true*/){
	SwitchBarItem* pItem	= GetItem(nItem);
	if( !pItem )
		return false;

	if( pItem->bDisabled == !bEnable )
		return true;
		
	pItem->bDisabled = !bEnable;
	if( bRedraw ){
		RedrawItem(nItem);
		}
	return true;
	}

int
ESSwitchBar::AddItem(TCHAR* pszText, long lItemData){
	SwitchBarItem* pItem	= new SwitchBarItem;
	pItem->next				= NULL;
	pItem->prev				= NULL;
	pItem->sItemText		= pszText;
	pItem->bDisabled		= false;
	pItem->nIndex			= m_items.GetCount();
	pItem->lData			= lItemData;

	m_items.Add(pItem);
	return m_items.GetCount();
	}

BOOL
ESSwitchBar::RedrawItem(int nItem){
	CRect rcItem(0, 0, 0, 0);
	if( !GetItemRect(nItem, rcItem) ) return FALSE;
	RedrawRect(rcItem, TRUE);
	return TRUE;
	}

SwitchBarItem*
ESSwitchBar::GetItem(int nItem){
	return m_items.GetAt(nItem);
	}