// ESChildControlTree.cpp : implementation file
//

#include "stdafx.h"
#include "ESChildControlTree.h"
#include "..\..\..\Controls\ESChildScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTree

ESChildControlTree::ESChildControlTree() : ESChildControl(){
	/*
	m_pContent			= new ESChildControlTreeContent();
	m_pSelItem			= NULL;
	m_pFirstItem		= NULL;
	m_nHeightView		= 0;
	m_nWidthView		= 0;
	m_nTopItemIndex		= 0;
	m_bVScroll			= FALSE;
	m_ptViewOffset.SetPoint(0, 0);*/
	}

ESChildControlTree::~ESChildControlTree(){
	m_listImages.RemoveAll(true);

	m_nTopItemIndex	= 0;
	m_nHeightView	= 0;
	m_nWidthView	= 0;
	m_pSelItem		= NULL;
	m_pFirstItem	= NULL;
	}

bool
ESChildControlTree::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	return true;
	}

bool
ESChildControlTree::Destroy(){
	return true;
	}

bool
ESChildControlTree::SetPos(int x, int y, bool bRedraw /*= true*/){
	return true;
	}

bool
ESChildControlTree::SetSize(int cx, int cy, bool bRedraw /*= true*/){
	return true;
	}

void
ESChildControlTree::SetLogicalPos(float fPosX, float fPosY){
	}

void
ESChildControlTree::SetLogicalSize(float fCX, float fCY){
	}

void
ESChildControlTree::OwnerWindowSizeChanged(int cx, int cy){
	}

int
ESChildControlTree::GetVisChildsCtSetRedraw(ESTreeItem* pItem){
	if( !pItem || !pItem->child || !IsItemExpanded(pItem) )
		return 0;

	// Count visible child items. ############
	ESTreeItem* pChildItem	= pItem->child;
	ESTreeItem* pNextItem	= pItem->next;
	ESTreeItem* pParItem	= pItem->parent;
	pItem->parent			= NULL;
	pItem->next				= NULL;

	int nCt = 0;
	while( pChildItem ){
		SetItemRedraw(pChildItem, TRUE);
		pChildItem = GetNextVisibleItem(pChildItem);
		nCt ++;
		}
	pItem->parent			= pParItem;
	pItem->next				= pNextItem;
	// #######################################
	return nCt;
	}

int
ESChildControlTree::GetVisibleChildsCt(ESTreeItem* pItem){
	if( !pItem || !pItem->child || !IsItemExpanded(pItem) )
		return 0;

	// Count visible child items. ############
	ESTreeItem* pChildItem = pItem->child;
	ESTreeItem* pNextItem  = pItem->next;
	ESTreeItem* pParItem	= pItem->parent;
	pItem->parent			= NULL;
	pItem->next				= NULL;

	int nCt = 0;
	while( pChildItem ){
		pChildItem = GetNextVisibleItem(pChildItem);
		nCt ++;
		}
	pItem->parent			= pParItem;
	pItem->next				= pNextItem;
	// #######################################
	return nCt;
	}

BOOL
ESChildControlTree::IsItemVisible(ESTreeItem* pItem){
	if( !pItem ) return FALSE;

	// Check for collapsed parent item. ##########
	ESTreeItem* parent = pItem->parent;
	while( parent ){
		if( !IsItemExpanded(parent) ) return FALSE;
		parent = parent->parent;
		}
	// ###########################################

	// Check item visibility. ####################
	CRect			rcItem;
	if( GetItemRect(pItem, rcItem) ){
		int nItemHeight = MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
		if( (m_rcRect.bottom == rcItem.bottom || m_rcRect.top == rcItem.top) && rcItem.Height() < nItemHeight ){
			return FALSE; // Partially visible last item is not visible yet.
			}
		return 1;
		}
	return FALSE;
	}

BOOL
ESChildControlTree::EnsureItemVisibility(ESTreeItem* pItem){
	if( IsItemVisible(pItem) ) return FALSE;

	int				nItemHeight		= MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
	int				nMaxTopItem		= (m_nHeightView - m_rcRect.Height())/nItemHeight;
	int				nMaxTopItemY	= (m_nHeightView - m_rcRect.Height())%nItemHeight;
	int				nItemsVisible	= (m_rcRect.Height() / nItemHeight);
	int				nItemsCt		= (m_nHeightView / nItemHeight);
	int				nIndex			= 0;
	ESTreeItem*	pItemSeek		= GetRootItem();

	while( pItemSeek ){
		if( pItemSeek == pItem ){
			int nTopIndex		= (nIndex + 1)%nItemsVisible + (((nIndex + 1)/nItemsVisible) - 1)*nItemsVisible;
			if( m_nTopItemIndex > nTopIndex ){
				if( (m_nTopItemIndex - nIndex) > 1  ){
					nTopIndex = (nIndex/nItemsVisible)*nItemsVisible;
					}
				else
					nTopIndex = nIndex;
				}
			int nOffsetY		= 0;
			if( IsLastItem(pItemSeek) ){
				nOffsetY = nMaxTopItemY;
				}

			MoveViewPos(0, (nTopIndex*nItemHeight + nOffsetY) - (m_nTopItemIndex*nItemHeight + m_ptViewOffset.y), TRUE);
			return TRUE;
			}

		nIndex ++;
		pItemSeek = GetNextVisibleItem(pItemSeek);
		}

	return FALSE;
	}

BOOL
ESChildControlTree::SelectItem(ESTreeItem* pItem, BOOL bInvalidate /*= TRUE*/){
	if( !pItem ) return 0;
	if( m_pSelItem == pItem ) return 1;
	if( m_pSelItem ){
		SetItemSelected(m_pSelItem, FALSE);
		// Clear old selection.
		RedrawItem(m_pSelItem);
		}

	m_pSelItem = pItem;
	SetItemSelected(m_pSelItem, TRUE);
	if( bInvalidate ){
		if( !EnsureItemVisibility(m_pSelItem) ){
			// Redraw currently selected item.
			RedrawItem(m_pSelItem);
			}
		}
	return 1;
	}

BOOL
ESChildControlTree::ExpandItem(ESTreeItem* pItem, BOOL bCollapse /*= FALSE*/, BOOL bInvalidate /*= TRUE*/)
	{
	if( !pItem || !IsItemParent(pItem) || !IsItemExpandable(pItem) ) return FALSE;
	if( bCollapse ){
		// Already collapsed.
		if( !IsItemExpanded(pItem) ) return FALSE;
		int nHeightViewOld	= m_nHeightView;
		// Set collapsed state.
		SetItemExpanded(pItem, FALSE);
		// ############################
		
		// Arrange top item when collapse item. ##########
		CRect rcClient = m_rcRect;

		int nItemHeight		= MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
		int nHeightVisItems = m_nHeightView - (m_nTopItemIndex*nItemHeight + m_ptViewOffset.y);
		if( nHeightVisItems < rcClient.Height() ){
			MoveViewPos(0, nHeightVisItems - rcClient.Height(), TRUE);
			// Update scroll bars. ########
			UpdateScrollBars();
			// ############################
			return TRUE;
			}
		// ###############################################

		// Invalidate items. ######
		if( bInvalidate ){
			CRect rcItem;
			if( GetItemRect(pItem, rcItem) ){
				CRect rcInvalidate(rcItem.left, rcItem.top, rcItem.right, rcClient.bottom);
				while( rcItem.top <  rcInvalidate.bottom ){
					// Set redraw flag. ##################
					SetItemRedraw(pItem, TRUE);
					// ###################################
					pItem = GetNextVisibleItem(pItem);
					if( !pItem ) break;
					rcItem.top		= rcItem.bottom;
					rcItem.bottom	+= MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
					}
				RedrawRect(rcInvalidate);
				}
			else{
				RedrawItem(pItem);
				}
			// ###################
			// Update scroll bars. ########
			UpdateScrollBars();
			// ############################
			return TRUE;
			}
		}
	else{
		// Already expanded.
		if( IsItemExpanded(pItem) ) return FALSE;
	
		// Set expanded state.
		SetItemExpanded(pItem, TRUE);

		// Invalidate items. ######
		if( bInvalidate ){
			CRect rcItem;
			if( GetItemRect(pItem, rcItem) ){
				CRect rcInvalidate(rcItem.left, rcItem.top, rcItem.right, m_rcRect.bottom);
				while( rcItem.top <  rcInvalidate.bottom ){
					// Set redraw flag. ##################
					SetItemRedraw(pItem, TRUE);
					// ###################################
					
					pItem = GetNextVisibleItem(pItem);
					if( !pItem ) break;
					rcItem.top		= rcItem.bottom;
					rcItem.bottom	+= MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
					}
				RedrawRect(rcInvalidate);
				}
			else
				RedrawItem(pItem);
			// ###################
			// Update scroll bars. ########
			UpdateScrollBars();
			// ############################
			return TRUE;
			}
		}
	return FALSE;
	}

BOOL
ESChildControlTree::CheckItem(ESTreeItem* pItem, BOOL bCheck /*= TRUE*/, BOOL bInvalidate /*= TRUE*/){
	if( bInvalidate )
		RedrawItem(pItem);
	return FALSE;
	}

BOOL
ESChildControlTree::DeleteItem(ESTreeItem* pItem){
	return FALSE;
	}

int
ESChildControlTree::AddItemImage(CString sImageFile){
	CImage* pImage = new CImage;
	if( pImage->Load(sImageFile.GetBuffer()) != 0 ){
		delete pImage;
		return -1;
		}
	m_listImages.Add(pImage);
	return (m_listImages.GetCount() - 1);
	}

BOOL
ESChildControlTree::SetItemImage(ESTreeItem* pItem, int nImage, int nImageSel, BOOL bInvalidate /*= TRUE*/){
	if( !pItem ) return FALSE;
	pItem->iImage		= nImage;
	pItem->iSelImage	= nImageSel;

	if( bInvalidate )
		RedrawItem(pItem);
	return TRUE;
	}

ESTreeItem*
ESChildControlTree::AddItem(LPTSTR lpszText, ESTreeItem* parent /*= NULL*/, BOOL bInvalidate /*= FALSE*/){
	ESTreeItem* pItemNew = new ESTreeItem;
	pItemNew->parent	= parent;
	pItemNew->next		= NULL;
	pItemNew->prev		= NULL;
	pItemNew->child		= NULL;
	pItemNew->sText		= lpszText;
	pItemNew->param		= 0;
	pItemNew->iImage	= -1;
	pItemNew->iSelImage = -1;
	pItemNew->dwState	= MercuryGUI::GetInstance()->treeGrid.m_dwDefState;
	/*
	if( !m_pContent->Add(pItemNew, parent) ){
		delete pItemNew;
		return NULL;
		}*/

	if( !m_pFirstItem ){
		m_pFirstItem = pItemNew;
		}

	// Check for collapsed item.
	BOOL			bVisible= TRUE;
	while( parent ){
		if( !IsItemExpanded(parent) ){
			bVisible = FALSE;
			break;
			}
		parent = parent->parent;
		}
	// #########################

	if( bVisible ){
		m_nHeightView += MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
		// Update scroll bars. ########
		if( bInvalidate )
			UpdateScrollBars();
		// ############################
		}
	return pItemNew;
	}

LPCTSTR
ESChildControlTree::GetItemText(ESTreeItem* pItem){
	return pItem->sText.GetBuffer();
	}

BOOL
ESChildControlTree::SetItemText(ESTreeItem* pItem, CString sText){
	pItem->sText = sText;
	return 1;
	}

BOOL
ESChildControlTree::GetItemRect(ESTreeItem* pItem, CRect& rcItem){
	if( !pItem ) return FALSE;
	ESTreeItem* pItemRect = GetFirstVisibleItem();
	rcItem.top		= m_rcBorders.top;
	rcItem.bottom	= rcItem.top + MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;

	if( m_ptViewOffset.y > 0 ){
		rcItem.bottom	= (MercuryGUI::GetInstance()->treeGrid.m_nItemHeight - m_ptViewOffset.y);
		}
	else
	if( m_ptViewOffset.y < 0 ){
		rcItem.top		= -1*m_ptViewOffset.y;
		rcItem.bottom	= rcItem.top + MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
		}

	CRect rcClient	= m_rcRect;
	rcClient.left	+= m_rcBorders.left;
	rcClient.top	+= m_rcBorders.top;
	rcClient.right	-= m_rcBorders.right;
	rcClient.bottom -= m_rcBorders.bottom;

	rcItem.left		= rcClient.left;
	rcItem.right	= rcClient.right;

	while( rcItem.top < rcClient.bottom ){
		if( pItem == pItemRect ){
			rcItem.bottom = min(rcClient.bottom, rcItem.bottom);
			return 1;
			}

		pItemRect		= GetNextVisibleItem(pItemRect);
		rcItem.top		= rcItem.bottom;
		rcItem.bottom	+= MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
		}
	return FALSE;
	}

ESTreeItem*
ESChildControlTree::GetFirstVisibleItem(){
	return m_pFirstItem;
	}

ESTreeItem*			
ESChildControlTree::GetNextVisibleItem(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	if( pItem->child && IsItemExpanded(pItem) ){
		return pItem->child;
		}
	else
	if( pItem->next ){
		return pItem->next;
		}
	else{
		ESTreeItem* parent = pItem->parent;
		while( parent ){
			if( parent->next )
				return parent->next;
			parent = parent->parent;
			}
		}
	return NULL;
	}

ESTreeItem*
ESChildControlTree::GetPrevVisibleItem(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	if( pItem->prev ){
		pItem = pItem->prev;
		while( pItem->child && IsItemExpanded(pItem) ){
			// Find last child. #####
			pItem = pItem->child;
			while( pItem->next ){
				pItem = pItem->next;
				}
			// ######################
			}
		return pItem;
		}
	else
		return pItem->parent;
	return NULL;
	}

CFont*
ESChildControlTree::GetItemFont(ESTreeItem* pItem){
	if( m_pOwner->HasFocus(this) ){
		if( IsItemSelected(pItem) )
			return &MercuryGUI::GetInstance()->treeGrid.m_fontSelItemActive;
		else
		if( IsItemReadOnly(pItem) )
			return &MercuryGUI::GetInstance()->treeGrid.m_fontReadOnlyItemActive;
		else
			return &MercuryGUI::GetInstance()->treeGrid.m_fontNormalItemActive;
		}
	else{
		if( IsItemSelected(pItem) )
			return &MercuryGUI::GetInstance()->treeGrid.m_fontSelItemNonActive;
		else
		if( IsItemReadOnly(pItem) )
			return &MercuryGUI::GetInstance()->treeGrid.m_fontReadOnlyItemNonActive;
		else
			return &MercuryGUI::GetInstance()->treeGrid.m_fontNormalItemNonActive;
		}
	return NULL;
	}

COLORREF
ESChildControlTree::GetItemTextColor(ESTreeItem* pItem, COLORREF& crShadow)
	{
	if( m_pOwner->HasFocus(this) ){
		if( IsItemSelected(pItem) ){
			crShadow = MercuryGUI::GetInstance()->treeGrid.m_crSelItemTextActShadow;
			return MercuryGUI::GetInstance()->treeGrid.m_crSelItemTextActive;
			}
		else
		if( IsItemReadOnly(pItem) ){
			crShadow = MercuryGUI::GetInstance()->treeGrid.m_crReadOnlyItemTextActShadow;
			return MercuryGUI::GetInstance()->treeGrid.m_crReadOnlyItemTextActive;
			}
		else{
			crShadow = MercuryGUI::GetInstance()->treeGrid.m_crNormalItemTextActShadow;
			return MercuryGUI::GetInstance()->treeGrid.m_crNormalItemTextActive;
			}
		}
	else{
		if( IsItemSelected(pItem) ){
			crShadow = MercuryGUI::GetInstance()->treeGrid.m_crSelItemTextNonActShadow;
			return MercuryGUI::GetInstance()->treeGrid.m_crSelItemTextNonActive;
			}
		else
		if( IsItemReadOnly(pItem) ){
			crShadow = MercuryGUI::GetInstance()->treeGrid.m_crReadOnlyItemTextNonActShadow;
			return MercuryGUI::GetInstance()->treeGrid.m_crReadOnlyItemTextNonActive;
			}
		else{
			crShadow = MercuryGUI::GetInstance()->treeGrid.m_crNormalItemTextNonActShadow;
			return MercuryGUI::GetInstance()->treeGrid.m_crNormalItemTextNonActive;
			}
		}
	return 0;
	}

ESTreeItem*
ESChildControlTree::ItemByPoint(CPoint pt, CRect* pRcItem /*= NULL*/){
	CRect rcView	= m_rcRect;
	rcView.left		+= m_rcBorders.left;
	rcView.top		+= m_rcBorders.top;
	rcView.right	-= m_rcBorders.right;
	rcView.bottom	-= m_rcBorders.bottom;
	if( !rcView.PtInRect(pt) ) return NULL;

	ESTreeItem*		pItem		= m_pFirstItem;
	int				nOffsetY	= m_ptViewOffset.y;
	CRect			rcItem;
	rcItem.left		= rcView.left;
	rcItem.right	= rcView.right;

	if( m_ptViewOffset.y < 0 ){
		pItem		= GetPrevVisibleItem(pItem);
		nOffsetY	= (MercuryGUI::GetInstance()->treeGrid.m_nItemHeight + m_ptViewOffset.y);
		}

	int nItemHeight = MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
	rcItem.top		= rcView.top - nOffsetY;
	rcItem.bottom	= rcItem.top + nItemHeight;

	pt.y -= (nItemHeight - nOffsetY);
	if( pt.y > 0 ){
		do{
			if( !pItem ) return NULL;
			pt.y			-= nItemHeight;
			pItem			= GetNextVisibleItem(pItem);
			rcItem.top		+= nItemHeight;
			rcItem.bottom	+= nItemHeight;
			if( pt.y <= 0 ) break;
			}
		while( TRUE );
		}

	if( pRcItem )
		*pRcItem = rcItem;
	return pItem;
	}

BOOL
ESChildControlTree::SelectItemByPoint(CPoint pt){
	CRect		 rcItem;
	ESTreeItem* pItemSel = ItemByPoint(pt, &rcItem);
	if( !pItemSel ) return FALSE;

	if( pItemSel != m_pSelItem ){
		// Clear old selection. ############
		if( m_pSelItem ){
			SetItemSelected(m_pSelItem, FALSE);
			RedrawItem(m_pSelItem);
			}
		// #################################
		SetItemSelected(pItemSel, TRUE);
		RedrawItem(pItemSel);
		m_pSelItem = pItemSel;
		return TRUE;
		}
	else // Expand / Collapse operation.
	if( IsItemExpandable(pItemSel) && IsItemParent(pItemSel) ){
		ExpandItem(pItemSel, IsItemExpanded(pItemSel), TRUE);
		return TRUE;
		}
	return FALSE;
	}

void
ESChildControlTree::UpdateScrollBars(){
	/*
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcVScroll(rcClient.right - MercuryGUI::GetInstance()->scrollBar.m_nVScrollWidth, rcClient.top, 
					rcClient.right, rcClient.bottom);

	if( m_nHeightView > rcClient.Height() ){
		// Move scrollbar window in left side.
		::MoveWindow(m_vScroll, rcVScroll.left, rcVScroll.top, rcVScroll.Width(), rcVScroll.Height(), FALSE);
		if( !m_bVScroll ){
			m_vScroll.ShowWindow(SW_SHOW);
			m_bVScroll = TRUE;
			}

		// Update scroll position. ##############
		GetClientRectMy(rcClient);
		int nPos = (m_nTopItemIndex*MercuryGUI::GetInstance()->treeGrid.m_nItemHeight) + m_ptViewOffset.y;
		m_vScroll.LockRedraw	(TRUE);
		m_vScroll.SetScrollPos	(nPos, FALSE);
		m_vScroll.SetScrollInfo	(0, m_nHeightView, rcClient.Height());
		m_vScroll.LockRedraw	(FALSE);
		m_vScroll.RedrawRect	(CRect(0, 0, rcVScroll.Width(), rcVScroll.Height()), TRUE);
		// ######################################
		}
	else{
		if( m_bVScroll ){
			m_vScroll.ShowWindow(SW_HIDE);
			m_bVScroll = FALSE;
			}
		}*/
	}

BOOL
ESChildControlTree::MoveViewPos(int nMoveX, int nMoveY, BOOL bInvalidate, BOOL bUpdateScrolls /*= TRUE*/, BOOL bRedrawAll /*= TRUE*/){
	/*
	if( !nMoveX && !nMoveY ) return FALSE;
	CRect rcClient = m_rcRect;

	// When view height is changed. ####################
	if( nMoveY && m_nHeightView <= rcClient.Height() ){
		// Change first visible item. ######
		m_pFirstItem = GetRootItem();
		// #################################
		if( m_ptViewOffset.y != 0 || m_nTopItemIndex > 0 ){
			m_ptViewOffset.y	= 0;
			m_nTopItemIndex		= 0;
			Invalidate(FALSE);
			return TRUE;
			}

		Invalidate(FALSE);
		return FALSE;
		}
	// #################################################

	int nItemHeight		= m_pGUI->treeGrid.m_nItemHeight;
	int nMaxTopItem		= (m_nHeightView - rcClient.Height())/nItemHeight;
	int nMaxTopItemY	= (m_nHeightView - rcClient.Height())%nItemHeight;
	int nItemMove		= (m_ptViewOffset.y + nMoveY)/nItemHeight;
	int nItemOffsetY	= (m_ptViewOffset.y + nMoveY)%nItemHeight;

	// Check top item index and offset values. #########
	if( nMoveY > 0 ){
		if( ((m_nTopItemIndex + nItemMove) > nMaxTopItem) ||
			((m_nTopItemIndex + nItemMove) == nMaxTopItem) && nItemOffsetY > nMaxTopItemY){
			nItemMove		= (nMaxTopItem - m_nTopItemIndex);
			nItemOffsetY	= nMaxTopItemY;
			}
		}
	if( nMoveY < 0 ){
		if( ((m_nTopItemIndex + nItemMove) < 0) ||
			((m_nTopItemIndex + nItemMove) == 0 && nItemOffsetY < 0 ) ){
			nItemMove		= -1*m_nTopItemIndex;
			nItemOffsetY	= 0;
			}
		}
	// ################################################

	// Sync top item. #######################
	int nLoop = 0;
	while( nItemMove > 0 ){
		m_pFirstItem = GetNextVisibleItem(m_pFirstItem);
		nItemMove --; nLoop ++;
		}

	while( nItemMove < 0 ){
		m_pFirstItem = GetPrevVisibleItem(m_pFirstItem);
		nItemMove ++; nLoop --;
		}
	// ######################################

	BOOL bViewChanged	= TRUE;
	if( nItemOffsetY < 0 ){
		nItemOffsetY	+= nItemHeight;
		m_pFirstItem	= GetPrevVisibleItem(m_pFirstItem);
		nLoop			--;
		}

	int nTopIndexOld		= m_nTopItemIndex;
	CPoint ptViewOffsetOld	= m_ptViewOffset;
	m_ptViewOffset.y		= nItemOffsetY;
	m_nTopItemIndex			+= nLoop;

	nMoveY = (m_nTopItemIndex*nItemHeight - (nItemHeight - m_ptViewOffset.y)) - 
			 (nTopIndexOld*nItemHeight - (nItemHeight - ptViewOffsetOld.y));
	if( bInvalidate && nMoveY != 0 ){
		if( bRedrawAll ){
			RedrawAllItems();
			}
		else{ // Scroll DC.
			if( abs(nMoveY) < rcClient.Height() ){
				ScrollWindow(0, -1*nMoveY);
				}
			else
				RedrawAllItems();
			}
		}

	// Update scroll bars. ########
	if( bUpdateScrolls )
		UpdateScrollBars();
	// ############################
	*/
	return TRUE;
	}

void
ESChildControlTree::RedrawAllItems(){
	/*
	CRect rcClient;
	GetClientRectMy(rcClient);
	RedrawRect(rcClient);

	CRect rcItem;
	ESTreeItem* pItem = m_pFirstItem;
	if( GetItemRect(pItem, rcItem) ){
		while( rcItem.top <  rcClient.bottom ){
			// Set redraw flag. ##################
			SetItemRedraw(pItem, TRUE);
			// ###################################
			pItem = GetNextVisibleItem(pItem);
			if( !pItem ) break;
			rcItem.top		= rcItem.bottom;
			rcItem.bottom	+= m_pGUI->treeGrid.m_nItemHeight;
			}
		}*/
	}

BOOL
ESChildControlTree::RedrawItem(ESTreeItem* pItem){
	if( !pItem ) return FALSE;
	CRect rcItem;
	if( GetItemRect(pItem, rcItem) ){
		SetItemRedraw(pItem, TRUE);
		RedrawRect(rcItem);
		return TRUE;
		}
	return FALSE;
	}

void 
ESChildControlTree::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	/*
	CRect	rcClient;
	GetClientRectMy(rcClient);

	CRect	rcDestDC; // Full destination rect in DC coordinates.
	rcDestDC.left	= pRectDC->left - pRectClient->left;
	rcDestDC.top	= pRectDC->top - pRectClient->top;
	rcDestDC.right	= rcDestDC.left + rcClient.Width();
	rcDestDC.bottom	= rcDestDC.top + rcClient.Height();

	// Draw border. ###############################
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
		pDC->LineTo(rcDestDC.left, rcDestDC.top);
		rcFill.left ++;
		}
	pDC->SelectObject(penOld);
	// #############################################

	CBrush brushFill(GetBkColor());
	pDC->FillRect(&rcFill, &brushFill);	
	rcDestDC = rcFill;

	int nItemHeight = m_pGUI->treeGrid.m_nItemHeight;
	ESTreeItem*	pItem = GetFirstVisibleItem();
	CString			sItemText;
	CRect			rcItem;
	COLORREF		crShadow, crText;
	rcItem.left		= rcDestDC.left + (m_pGUI->treeGrid.m_ptOffset.x - m_ptViewOffset.x);
	rcItem.right	= rcDestDC.right;
	rcItem.top		= rcDestDC.top + (m_pGUI->treeGrid.m_ptOffset.y - m_ptViewOffset.y);
	rcItem.bottom	= rcItem.top + nItemHeight;

	CImage*			pImageSel = NULL;
	BITMAP			bmSel;
	if( m_bActive ){
		pImageSel = &m_pGUI->treeGrid.m_imageSelActive;
		GetObject(m_pGUI->treeGrid.m_imageSelActive, sizeof(BITMAP), &bmSel);
		}
	else{
		pImageSel = &m_pGUI->treeGrid.m_imageSelNonActive;
		GetObject(m_pGUI->treeGrid.m_imageSelNonActive, sizeof(BITMAP), &bmSel);
		}

	int	nLoop	= 0;
	while( pItem ){
		if( rcItem.top > rcDestDC.bottom ) break;
		int nParCt		= GetParentsCount(pItem);
		int nOffsetX	= 18*nParCt;

		// Skip invisible items. ###############
		if( rcItem.bottom < 0 || (m_bClipBox && !GetItemRedraw(pItem)) ){
			pItem			= GetNextVisibleItem(pItem);
			rcItem.left		= rcDestDC.left + (m_pGUI->treeGrid.m_ptOffset.x - m_ptViewOffset.x);
			rcItem.top		= rcItem.bottom;
			rcItem.bottom	+= nItemHeight;
			continue;
			}
		// ####################################

		// Draw selected item. ################
		if( IsItemSelected(pItem) ){
			pImageSel->Draw(pDC->m_hDC, rcDestDC.left, rcItem.top, rcDestDC.Width(), 
				rcItem.Height(), 0, 0, bmSel.bmWidth, bmSel.bmHeight);
			}
		// ####################################

		// Draw expand / collapse sign. #######
		if( pItem->child && IsItemExpandable(pItem) ){
			CImage* pImgSign = &m_pGUI->treeGrid.m_imageExpandSign;
			if( IsItemExpanded(pItem) ){
				pImgSign = &m_pGUI->treeGrid.m_imageExpandSign;
				if( IsItemSelected(pItem) )
					pImgSign = &m_pGUI->treeGrid.m_imageExpandSignSel;
				}
			else{
				pImgSign = &m_pGUI->treeGrid.m_imageCollapseSign;
				if( IsItemSelected(pItem) )
					pImgSign = &m_pGUI->treeGrid.m_imageCollapseSignSel;
				}

			int nImageCX	= pImgSign->GetWidth(), nImageCY = pImgSign->GetHeight();
			CRect rcSign;
			rcSign.left		= rcItem.left + nOffsetX;
			rcSign.top		= rcItem.top + (rcItem.Height() - nImageCY)/2;
			rcSign.right	= rcSign.left + nImageCX;
			rcSign.bottom	= rcSign.top + nImageCY;

			CBitmap bmBkgnd;
			if( ImageHelper::GetBitmap32bppFromDC(bmBkgnd, pDC, rcSign.left, rcSign.top, nImageCX, nImageCY) ){
				BITMAP bmSrc;
				GetObject(*pImgSign, sizeof(BITMAP), &bmSrc);

				BITMAP bmDest;
				bmBkgnd.GetBitmap(&bmDest);
				ImageHelper::BitmapOnBitmapEx(&bmDest, 0, 0, &bmSrc, 0, 0, nImageCX, nImageCY, nImageCX, nImageCY, 1);

				CDC memDC;
				memDC.CreateCompatibleDC(pDC);
				memDC.SelectObject(&bmBkgnd);
				pDC->BitBlt(rcSign.left, rcSign.top, rcSign.Width(), rcSign.Height(), &memDC, 0, 0, SRCCOPY);
				bmBkgnd	.DeleteObject();
				memDC	.DeleteDC();

				rcItem.left += nImageCX + 2;
				}
			}
		// ####################################

		// Draw item image. ###################
		CImage* pImage = GetItemImage(pItem);
		if( pImage ){
			int nImageCX = pImage->GetWidth(), nImageCY = pImage->GetHeight();
			CRect rcImage;
			rcImage.left	= rcItem.left + nOffsetX;
			rcImage.top		= rcItem.top + (rcItem.Height() - nImageCY)/2;
			rcImage.right	= rcImage.left + nImageCX;
			rcImage.bottom	= rcImage.top + nImageCY;

			if( pImage->GetBPP() == 32 ){
				CBitmap bmBkgnd;
				if( ImageHelper::GetBitmap32bppFromDC(bmBkgnd, pDC, rcImage.left, rcImage.top, nImageCX, nImageCY) ){
					BITMAP bmSrc;
					GetObject(*pImage, sizeof(BITMAP), &bmSrc);

					BITMAP bmDest;
					bmBkgnd.GetBitmap(&bmDest);
					ImageHelper::BitmapOnBitmapEx(&bmDest, 0, 0, &bmSrc, 0, 0, nImageCX, nImageCY, nImageCX, nImageCY, 1);

					CDC memDC;
					memDC.CreateCompatibleDC(pDC);
					memDC.SelectObject(&bmBkgnd);
					pDC->BitBlt(rcImage.left, rcImage.top, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, SRCCOPY);
					bmBkgnd	.DeleteObject();
					memDC	.DeleteDC();
					}
				}
			else
				pImage->Draw(pDC->m_hDC, rcImage.left, rcImage.top, rcImage.Width(), rcImage.Height(), 0, 0, nImageCX, nImageCY);
			rcItem.left += nImageCX + 4;
			}
		// ####################################

		// Draw item text with shadow. ########
		sItemText		= GetItemText(pItem);
		CFont* pFont	= GetItemFont(pItem);
		if( pFont )
			pDC->SelectObject(pFont);

		CSize	szText		= pDC->GetTextExtent(sItemText, sItemText.GetLength());
		CPoint  ptText	(rcItem.left + nOffsetX, rcItem.top + (rcItem.Height()-szText.cy)/2);

		crText				= GetItemTextColor(pItem, crShadow);
		pDC->SetBkMode		(TRANSPARENT);
		if( HasItemShadow(pItem) && crShadow != crText ){
			pDC->SetTextColor	(crShadow);
			pDC->ExtTextOut		(ptText.x, ptText.y+1, ETO_CLIPPED, rcItem, sItemText, sItemText.GetLength(), NULL);
			}
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, rcItem, sItemText, sItemText.GetLength(), NULL);
		// ####################################

		// Clear redraw flag. #################
		SetItemRedraw(pItem, FALSE);
		// ####################################
		pItem			= GetNextVisibleItem(pItem);
		rcItem.left		= rcDestDC.left + (m_pGUI->treeGrid.m_ptOffset.x - m_ptViewOffset.x);
		rcItem.top		= rcItem.bottom;
		rcItem.bottom	+= nItemHeight;
		nLoop			++;
		}
		*/
	}

void
ESChildControlTree::OnMouseHover(CPoint pt, UINT nFlags){
	}

void
ESChildControlTree::OnMouseLeave(UINT nFlags){
	}

void
ESChildControlTree::OnMouseEnter(UINT nFlags){
	}

BOOL
ESChildControlTree::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return TRUE;
	}

void
ESChildControlTree::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void
ESChildControlTree::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void
ESChildControlTree::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void
ESChildControlTree::OnLButtonDown(CPoint pt, UINT nFlags){
	// Select item by point. #####
	SelectItemByPoint(pt);
	// ###########################
	}

void
ESChildControlTree::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void
ESChildControlTree::OnLButtonDblClick(CPoint pt, UINT nFlags){
	// Select item by point. #####
	SelectItemByPoint(pt);
	// ###########################
	}

void
ESChildControlTree::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void
ESChildControlTree::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}

void
ESChildControlTree::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	MoveViewPos(0, -2*(zDelta/120)*MercuryGUI::GetInstance()->treeGrid.m_nItemHeight, TRUE, TRUE, FALSE);
	}

bool
ESChildControlTree::OnScrollBarEvent(bool bVScroll, int nCode, ESChildControl* pSender){
	int		nItemHeight = MercuryGUI::GetInstance()->treeGrid.m_nItemHeight;
	int		nPos		= 0;//pSender->GetP;
	int		nPosOld		= (m_nTopItemIndex*nItemHeight + m_ptViewOffset.y);

	CRect	rcClient;	
	UINT	nScrollCode = (UINT)nCode;
	if( bVScroll ){
		switch( nScrollCode ){
			case SB_LINEUP:
				MoveViewPos(0, -1*MercuryGUI::GetInstance()->treeGrid.m_nItemHeight, TRUE, TRUE, FALSE);
				break;
			case SB_LINEDOWN:
				MoveViewPos(0, MercuryGUI::GetInstance()->treeGrid.m_nItemHeight, TRUE, TRUE, FALSE);
				break;
			case SB_PAGEUP:
				{/*
				GetClientRectMy(rcClient);
				MoveViewPos(0, -1*rcClient.Height(), TRUE, TRUE, FALSE);
				return (m_nTopItemIndex*MercuryGUI::GetInstance()->treeGrid.m_nItemHeight) + m_ptViewOffset.y;*/
				}
			case SB_PAGEDOWN:{/*
				GetClientRectMy(rcClient);
				MoveViewPos(0, rcClient.Height(), TRUE, TRUE, FALSE);
				return (m_nTopItemIndex*MercuryGUI::GetInstance()->treeGrid.m_nItemHeight) + m_ptViewOffset.y;*/
				}
			case SB_THUMBTRACK:{
				MoveViewPos(0, nPos - nPosOld, TRUE, FALSE, FALSE);
				break;
				}
			default:
				MoveViewPos(0, nPos - nPosOld, TRUE, FALSE, FALSE);
				break;
			};
		}
	else{
		switch( nScrollCode ){
			case SB_LINELEFT:
				break;
			case SB_LINERIGHT:
				break;
			case SB_PAGELEFT:
				break;
			case SB_PAGERIGHT:
				break;
			default:
				MoveViewPos(nPos - nPosOld, 0, TRUE, FALSE, FALSE);
				break;
			};
		}
	return false;
	}

bool
ESChildControlTree::SetFocus(){
	return true;
	}

void
ESChildControlTree::KillFocus(){
	}

void
ESChildControlTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	if( nChar == VK_DOWN ){ // Move Down
		ESTreeItem* pItem = m_pFirstItem;
		if( m_pSelItem )
			pItem = GetNextVisibleItem(m_pSelItem);
		if( pItem )
			SelectItem(pItem, TRUE);
		}
	else
	if( nChar == VK_UP ){	// Move Up
		ESTreeItem* pItem = m_pFirstItem;
		if( m_pSelItem )
			pItem = GetPrevVisibleItem(m_pSelItem);
		if( pItem )
			SelectItem(pItem, TRUE);
		}
	else
	if( nChar == VK_LEFT ){ // Collapse selected item.
		if( m_pSelItem ){
			if( !ExpandItem(m_pSelItem, TRUE, TRUE) ){
				// Movo to parent item. #############
				if( m_pSelItem->parent ){
					SelectItem(m_pSelItem->parent);
					}
				// ##################################
				}
			}
		}
	else
	if( nChar == VK_RIGHT ){ // Expand selected item.
		if( m_pSelItem ){
			ExpandItem(m_pSelItem, FALSE, TRUE);
			}
		}
	}
