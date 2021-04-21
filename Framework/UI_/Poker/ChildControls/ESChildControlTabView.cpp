// ESChildControlTabView.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlTabView.h"
#include "ESChildControlTabViewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabView

ESChildControlTabView::ESChildControlTabView(){
	m_nActiveItem	= -1;
	}

ESChildControlTabView::~ESChildControlTabView(){
	}

bool
ESChildControlTabView::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlTabView::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlTabView::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlTabView::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlTabView::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);

	// Resize page controls. {{
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	int nBorderWidth	= 1;

	CRect rcPage;
	rcPage.left		= m_rcRect.left + nBorderWidth;
	rcPage.top		= m_rcRect.top + pGUI->m_nTabViewItemCY + 2*nBorderWidth;
	rcPage.right	= m_rcRect.right - nBorderWidth;
	rcPage.bottom	= m_rcRect.bottom - nBorderWidth;

	int nItem		= 0;
	int nCt			= m_listTabItems.GetCount();

	while( nItem < nCt ){
		ESTabViewItem* pItem = m_listTabItems.GetAt(nItem);
		if( pItem && pItem->pPage ){
			pItem->pPage->SetPos	(rcPage.left, rcPage.top, false);
			pItem->pPage->SetSize	(rcPage.Width(), rcPage.Height(), false);
			}
		nItem ++;
		}
	// }}
	}

bool
ESChildControlTabView::AddTabPage(ESChildControlTabViewPage* pPage, int nId, CString sTitle, int nData){
	if( !pPage ) return false;

	ESTabViewItem* pItem	= new ESTabViewItem();
	pItem->pPage			= pPage;
	pItem->sTitle			= sTitle;
	pItem->nData			= nData;
	m_listTabItems.Add(pItem);

	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	int nBorderWidth	= 1;

	// Add page control. {{
	CRect rcPage;
	rcPage.left		= m_rcRect.left + nBorderWidth;
	rcPage.top		= m_rcRect.top + pGUI->m_nTabViewItemCY + 2*nBorderWidth;
	rcPage.right	= m_rcRect.right - nBorderWidth;
	rcPage.bottom	= m_rcRect.bottom - nBorderWidth;
	m_pOwner->AddChildControl	(pPage, nId, rcPage, this);
	pPage->SetVisible			(false, false);
	// }}
	
	if( m_nActiveItem == -1 ){
		pPage->SetVisible(true, false);
		m_nActiveItem = 0;
		}
	return true;
	}

bool
ESChildControlTabView::RemoveTabPage(int nIndex){
	return false;
	}

bool
ESChildControlTabView::ActivateTabPage(int nIndex,  bool bRedraw /*= false*/){
	if( nIndex < 0 || nIndex >= m_listTabItems.GetCount() )
		return false;

	ESTabViewItem* pItemNew = m_listTabItems.GetAt(nIndex);
	ESTabViewItem* pItemOld = m_listTabItems.GetAt(m_nActiveItem);
		
	if( pItemOld && pItemOld->pPage )
		pItemOld->pPage->SetVisible(false, false);

	if( pItemNew && pItemNew->pPage ){
		pItemNew->pPage->SetVisible(true, false);

		m_nActiveItem = nIndex;
		if( bRedraw ){
			Redraw(true);
			}
		return true;
		}

	return false;
	}

int
ESChildControlTabView::GetItemByPoint(CPoint pt){
	if( !m_listTabItems.GetCount() )
		return -1;

	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	int nItemCt			= m_listTabItems.GetCount();
	int nItem			= 0;
	int nBorderWidth	= 1;
	int nItemHeight		= pGUI->m_nTabViewItemCY;
	int nItemWidth		= (m_rcRect.Width() - (nItemCt + 1)*nBorderWidth) / nItemCt;

	CRect rcTabItem;
	rcTabItem.left		= m_rcRect.left + nBorderWidth;
	rcTabItem.top		= m_rcRect.top + nBorderWidth;
	rcTabItem.right		= m_rcRect.right - nBorderWidth;
	rcTabItem.bottom	= rcTabItem.top + nItemHeight;

	if( !rcTabItem.PtInRect(pt) )
		return -1;
	nItem = ((pt.x - rcTabItem.left) / nItemWidth);
	return nItem;
	}

bool
ESChildControlTabView::RedrawItems(){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	int nItemCt			= m_listTabItems.GetCount();
	int nItem			= 0;
	int nBorderWidth	= 1;
	int nItemHeight		= pGUI->m_nTabViewItemCY;

	CRect rcTabItem;
	rcTabItem.left		= m_rcRect.left + nBorderWidth;
	rcTabItem.top		= m_rcRect.top + nBorderWidth;
	rcTabItem.right		= m_rcRect.right - nBorderWidth;
	rcTabItem.bottom	= rcTabItem.top + nItemHeight;

	if( m_pOwner )
		m_pOwner->RedrawRect(rcTabItem, 1);
	return (m_pOwner != NULL);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlTabView::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	int nItemCt			= m_listTabItems.GetCount();
	int nItem			= 0;
	int nBorderWidth	= 1;
	int nItemWidth		= nItemCt ? (m_rcRect.Width() - (nItemCt + 1)*nBorderWidth) / nItemCt : 0;
	int nItemHeight		= pGUI->m_nTabViewItemCY;

	CImage*	pImage		= NULL;
	CImage*	pItemActive	= pGUI->m_pTabViewItemActive;
	CImage*	pItemDefault= pGUI->m_pTabViewItemDefault;
	if( !pItemActive || !pItemDefault ) return;

	CRect	rcTabViewDC, rcInvalid;
	rcTabViewDC.left	= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcTabViewDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcTabViewDC.right	= rcTabViewDC.left + m_rcRect.Width(); 
	rcTabViewDC.bottom	= rcTabViewDC.top + m_rcRect.Height(); 

	// Draw round border. {{
	CPen	penBorder(PS_SOLID, nBorderWidth, pGUI->m_crTabViewBorder);
	CPen*	pPenOld	= pDC->SelectObject(&penBorder);
	pDC->MoveTo(rcTabViewDC.left, rcTabViewDC.top);
	pDC->LineTo(rcTabViewDC.right, rcTabViewDC.top);
	pDC->LineTo(rcTabViewDC.right, rcTabViewDC.bottom);
	pDC->LineTo(rcTabViewDC.left, rcTabViewDC.bottom);
	pDC->LineTo(rcTabViewDC.left, rcTabViewDC.top);

	// Draw bottom line.
	pDC->MoveTo(rcTabViewDC.left, rcTabViewDC.top + nItemHeight + nBorderWidth);
	pDC->LineTo(rcTabViewDC.right, rcTabViewDC.top + nItemHeight + nBorderWidth);

	bool			bDrawLine	= true;
	int				nOffsetX	= rcTabViewDC.left + nBorderWidth;
	ESTabViewItem*	pItem		= NULL;
	CSize			szText;
	bool			bActive		= false;

	while( nItem < nItemCt ){
		pItem				= m_listTabItems.GetAt(nItem);
		bActive				= (nItem == m_nActiveItem);

		CRect rcTabItemDC;
		rcTabItemDC.left	= nOffsetX;
		rcTabItemDC.top		= rcTabViewDC.top + nBorderWidth;
		rcTabItemDC.right	= rcTabItemDC.left + nItemWidth;
		rcTabItemDC.bottom	= rcTabItemDC.top + nItemHeight;

		if( nItem == (nItemCt - 1) ){
			rcTabItemDC.right	= rcTabViewDC.right;
			bDrawLine			= false;
			}

		// Draw tabview item.
		pImage		= bActive ? pItemActive : pItemDefault;
		rcInvalid	= rcTabItemDC & *pRectDC;
		if( pImage && !rcInvalid.IsRectEmpty() )
			PokerGUI::RenderImage(pDC, rcInvalid, rcTabItemDC, pImage);

		// Draw item label. {{
		CFont* pFontOld = pDC->SelectObject(pGUI->m_pTabViewItemFont);
		ImgHlp::GetTextSize(&pItem->sTitle, pGUI->m_pTabViewItemFont, szText);

		CPoint ptLabel		(rcTabItemDC.left + (rcTabItemDC.Width() - szText.cx)/2, rcTabItemDC.top + (rcTabItemDC.Height() - szText.cy)/2);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(bActive ? pGUI->m_crTabViewItemLabelAct : pGUI->m_crTabViewItemLabel);
		PokerGUI::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcTabItemDC, pItem->sTitle);
		pDC->SelectObject	(pFontOld);
		// }}

		// Draw divider border line. 
		if( bDrawLine ){
			pDC->MoveTo(rcTabItemDC.right, rcTabItemDC.top);
			pDC->LineTo(rcTabItemDC.right, rcTabItemDC.bottom);
			}

		bDrawLine	= true;
		nOffsetX	+= (nItemWidth + nBorderWidth);
		nItem		++;
		}
	pDC->SelectObject(pPenOld);
	}

void	
ESChildControlTabView::OnMouseHover(CPoint pt, UINT nFlags){
	m_nItemUnderCursor = GetItemByPoint(pt);
	}

BOOL	
ESChildControlTabView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	if( m_nItemUnderCursor == -1 )
		return 0;
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return 1;
	}

void	
ESChildControlTabView::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlTabView::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlTabView::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlTabView::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabView::OnLButtonClick(CPoint pt, UINT nFlags){
	if( m_nItemUnderCursor != -1 ){
		ActivateTabPage(m_nItemUnderCursor, true);
		}
	}

void	
ESChildControlTabView::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabView::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabView::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabView::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabView::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}