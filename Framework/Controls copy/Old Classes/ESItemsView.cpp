// ESItemsView.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ESItemsView.h"
#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESItemsView

ESItemsView::ESItemsView() : MercuryBaseView()
	{
	m_pContent			= new MercuryTreeGridContent();
	m_pSelItem			= NULL;
	m_pFirstItem		= NULL;
	m_nHeightView		= 0;
	m_nWidthView		= 0;
	m_nTopItemIndex		= 0;
	m_ptViewOffset.SetPoint(0, 0);
	}

ESItemsView::~ESItemsView()
	{
	DestroyClassMembers();
	}

BEGIN_MESSAGE_MAP(ESItemsView, MercuryBaseView)
END_MESSAGE_MAP()

BOOL 
ESItemsView::Create(LPCTSTR lpszClassName,
					LPCTSTR lpszWindowName, DWORD dwStyle,
					const RECT& rect,
					CWnd* pParentWnd, UINT nID,
					CCreateContext* pContext /*= NULL*/)
	{
	ASSERT(FALSE);
	return FALSE;
	}

void
ESItemsView::DestroyClassMembers()
	{
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

	m_nTopItemIndex	= 0;
	m_nHeightView	= 0;
	m_nWidthView	= 0;
	m_pSelItem		= NULL;
	m_pFirstItem	= NULL;
	}

int
ESItemsView::GetVisChildsCtSetRedraw(MerTreeItem* pItem)
	{
	if( !pItem || !pItem->child || !IsItemExpanded(pItem) )
		return 0;
	// Count visible child items. ############
	MerTreeItem* pChildItem = pItem->child;
	MerTreeItem* pNextItem  = pItem->next;
	MerTreeItem* pParItem	= pItem->parent;
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
ESItemsView::GetVisibleChildsCt(MerTreeItem* pItem)
	{
	if( !pItem || !pItem->child || !IsItemExpanded(pItem) )
		return 0;

	// Count visible child items. ############
	MerTreeItem* pChildItem = pItem->child;
	MerTreeItem* pNextItem  = pItem->next;
	MerTreeItem* pParItem	= pItem->parent;
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

int
ESItemsView::AddItemImage(CString sImageFile){
	CImage* pImage = new CImage;
	if( pImage->Load(sImageFile.GetBuffer()) != 0 ){
		delete pImage;
		return -1;
		}
	m_arrImages.Add(m_arrImages.GetCount(), pImage);
	return m_arrImages.GetCount() - 1;
	}

BOOL
ESItemsView::SetItemImage(MerTreeItem* pItem, int nImage, int nImageSel, BOOL bInvalidate /*= TRUE*/){
	if( !pItem ) return FALSE;
	pItem->iImage		= nImage;
	pItem->iSelImage	= nImageSel;

	if( bInvalidate ){
		RedrawItem(pItem);
		}
	return TRUE;
	}

MerTreeItem*
ESItemsView::AddItem(LPTSTR lpszText, MerTreeItem* parent /*= NULL*/)
	{
	MerTreeItem* pItemNew = new MerTreeItem;
	pItemNew->parent	= parent;
	pItemNew->next		= NULL;
	pItemNew->prev		= NULL;
	pItemNew->child		= NULL;
	pItemNew->sText		= lpszText;
	pItemNew->param		= 0;
	pItemNew->iImage	= -1;
	pItemNew->iSelImage = -1;
	pItemNew->dwState	= m_pGUI->treeGrid.m_dwDefState;

	if( !m_pContent->Add(pItemNew, parent) ){
		delete pItemNew;
		return NULL;
		}

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
/*
	if( bVisible ){
		m_nHeightView += m_pGUI->treeGrid.m_nItemHeight;
		// Update scroll bars. ########
		UpdateScrollBars();
		// ############################
		}
*/
	return pItemNew;
	}

MerTreeItem*			
ESItemsView::GetNextVisibleItem(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	if( pItem->child && IsItemExpanded(pItem) ){
		return pItem->child;
		}
	else
	if( pItem->next ){
		return pItem->next;
		}
	else{
		MerTreeItem* parent = pItem->parent;
		while( parent ){
			if( parent->next )
				return parent->next;
			parent = parent->parent;
			}
		}
	return NULL;
	}

MerTreeItem*
ESItemsView::GetPrevVisibleItem(MerTreeItem* pItem)
	{
	if( !pItem ) return NULL;
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
	return pItem->parent;
	}

CFont*
ESItemsView::GetItemFont(MerTreeItem* pItem)
	{
	if( m_bActive ){
		if( IsItemSelected(pItem) )
			return &m_pGUI->treeGrid.m_fontSelItemActive;
		else
		if( IsItemReadOnly(pItem) )
			return &m_pGUI->treeGrid.m_fontReadOnlyItemActive;
		else
			return &m_pGUI->treeGrid.m_fontNormalItemActive;
		}
	else{
		if( IsItemSelected(pItem) )
			return &m_pGUI->treeGrid.m_fontSelItemNonActive;
		else
		if( IsItemReadOnly(pItem) )
			return &m_pGUI->treeGrid.m_fontReadOnlyItemNonActive;
		else
			return &m_pGUI->treeGrid.m_fontNormalItemNonActive;
		}
	return NULL;
	}

COLORREF
ESItemsView::GetItemTextColor(MerTreeItem* pItem, COLORREF& crShadow)
	{
	if( m_bActive ){
		if( IsItemSelected(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crSelItemTextActShadow;
			return m_pGUI->treeGrid.m_crSelItemTextActive;
			}
		else
		if( IsItemReadOnly(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crReadOnlyItemTextActShadow;
			return m_pGUI->treeGrid.m_crReadOnlyItemTextActive;
			}
		else{
			crShadow = m_pGUI->treeGrid.m_crNormalItemTextActShadow;
			return m_pGUI->treeGrid.m_crNormalItemTextActive;
			}
		}
	else{
		if( IsItemSelected(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crSelItemTextNonActShadow;
			return m_pGUI->treeGrid.m_crSelItemTextNonActive;
			}
		else
		if( IsItemReadOnly(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crReadOnlyItemTextNonActShadow;
			return m_pGUI->treeGrid.m_crReadOnlyItemTextNonActive;
			}
		else{
			crShadow = m_pGUI->treeGrid.m_crNormalItemTextNonActShadow;
			return m_pGUI->treeGrid.m_crNormalItemTextNonActive;
			}
		}
	return 0;
	}

void
ESItemsView::OnPrePaint(CRect& rcRepaint)
	{
	}

void 
ESItemsView::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient)
	{
	}