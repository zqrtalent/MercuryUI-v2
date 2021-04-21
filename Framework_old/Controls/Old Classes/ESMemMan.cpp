// ESMemMan.cpp : implementation file
//

#include "stdafx.h"
#include "ESMemMan.h"
#include "ESHeaderCtrl.h"
#include "ESItemsList.h"
#include "..\UsefulFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESItems

ESItems::ESItems(){
	m_pRowSet = NULL;
	}

ESItems::~ESItems(){
	m_pRowSet = NULL;
	}

bool
ESItems::SetRowSet(VirtualRowSet* pRowSet){
	m_pRowSet = pRowSet;
	return true;
	}

DWORD
ESItems::GetItemState(int nItem){
	if( m_pRowSet->SetCurrentRow(nItem) ){
		return (DWORD)m_pRowSet->GetInt32(_T("_STATE"));
		}
	return 0;
	}

bool
ESItems::SetItemState(int nItem, DWORD dwState){
	ASSERT(m_pRowSet);
	if( !m_pRowSet->SetCurrentRow(nItem) )
		return false;

	DWORD dwStateOld = (DWORD)m_pRowSet->GetInt32(_T("_STATE"));
	if( dwState&ITEM_SELECTED && (dwStateOld&ITEM_SELECTED) == 0 ){
		m_arrSelItems.Add(nItem);
		}
	else
	if( (dwState&ITEM_SELECTED) == 0 && dwStateOld&ITEM_SELECTED ){
		int nIndex = m_arrSelItems.FindValue(nItem);
		if( nIndex > -1 ){
			m_arrSelItems.Delete(nIndex);
			}
		}

	m_pRowSet->SetInt32(_T("_STATE"), dwState);
	return true;
	}

bool
ESItems::GetSubItemText(CString& sText, int nFieldId /*= 0*/, int nItem /*= -1*/){
	ASSERT(m_pRowSet);
	if( nItem == -1 )
		nItem = m_pRowSet->GetCurrentRow();
	return m_pRowSet->GetString(nFieldId, sText);
	}

bool
ESItems::SetSubItemText(int nItem, int nSubItem, TCHAR* pszText){
	ASSERT(m_pRowSet);
	if( nItem == -1 )
		nItem = m_pRowSet->GetCurrentRow();
	return m_pRowSet->SetString(nSubItem, pszText);
	}

int
ESItems::GetItemByPoint(ESItemsList* pList, CPoint pt){
	if( !pList ) return -1;
	CRect rcClient;
	pList->GetClientRect(rcClient);

	int nItemsCount = GetItemsCount();
	if( pt.x > rcClient.right || pt.x < rcClient.left )
		return -1;
	if( pt.y > rcClient.bottom || pt.y < rcClient.top )
		return -1;

	int nTopItem		= pList->m_nTopItem;
	int nTopItemOffset	= pList->m_nTopItemOffset;
	pt.y				+= nTopItemOffset; 
	int	nItem			= nTopItem + (pt.y / pList->GetItemHeight());
	if( nItem >= nItemsCount ) return -1;
	return nItem;
	}

bool
ESItems::GetItemRect(ESItemsList* pList, int nItem, CRect& rcItem){
	if( !pList ) return false;
	CRect rcClient;
	pList->GetClientRect(rcClient);

	GetSubItemRect(pList, nItem, pList->m_nSubItemLeft, rcItem);
	if( !rcItem.IsRectEmpty() ){
		rcItem.left = rcClient.left;
		rcItem.right = rcClient.right;
		return true;
		}
	return false;
	}

bool
ESItems::GetSubItemRect(ESItemsList* pList, int nItem, int nSubItem, CRect& rcSubItem){
	ASSERT_VALID(pList);
	ASSERT(m_pRowSet);

	if( m_pRowSet->GetFieldCount() <= nSubItem ){
		return false;
		}

	MercuryGUI*		pGUI = MercuryGUI::GetInstance();
	ESHeaderCtrl*	pHdr = pList->GetHeaderCtrl();
	
	CRect rcClient;
	pList->GetClientRect(rcClient);

	int nItemPosY = (nItem - pList->m_nTopItem)*pGUI->itemsList.m_nItemHeight;
	// When item is invisible. #################
	if( nItemPosY >= rcClient.Height() )
		return false;

	if( nSubItem < pList->m_nSubItemLeft )
		return false;
	// #########################################

	CRect rcSubItemTemp = rcSubItem;
	rcSubItem.left		= -1*pList->m_nSubItemLeftOffset;
	rcSubItem.right		= 0;
	rcSubItem.top		= nItemPosY;
	rcSubItem.bottom	= rcSubItem.top + pGUI->itemsList.m_nItemHeight;

	int	nItemWidth		= 0;
	int nIndex			= pList->m_nSubItemLeft;
	while( nSubItem > nIndex ){
		if( rcSubItem.left >= rcClient.right ){
			rcSubItem = rcSubItemTemp;
			return false;
			}
		rcSubItem.left	+= pHdr->GetItemWidth(nIndex++);
		}

	rcSubItem.right		= rcSubItem.left + pHdr->GetItemWidth(nSubItem);
	return true;
	}

int
ESItems::ClearSelectedItems(){
	int nCt = m_arrSelItems.GetCount();
	if( !m_pRowSet ){
		m_arrSelItems.DeleteAll();
		return nCt;
		}

	int nLoop = 0;
	while(nLoop < nCt){
		if( m_pRowSet->SetCurrentRow(m_arrSelItems[nLoop]) ){
			DWORD dwState	= (DWORD)m_pRowSet->GetInt32(_T("_STATE"));
			dwState			&= ~ITEM_SELECTED;
			m_pRowSet->SetInt32(_T("_STATE"), (int)dwState);
			}
		nLoop			++;
		}

	m_arrSelItems.DeleteAll();
	return nCt;
	}

int
ESItems::GetSelectedItems(CDWordArray& arrSelItems){
	int nLoop=0;
	while( nLoop < m_arrSelItems.GetCount() ){
		arrSelItems.Add(m_arrSelItems[nLoop]);
		nLoop ++;
		}
	return nLoop;
	}


/////////////////////////////////////////////////////////////////////////////
// ESHeaderItems

ESHeaderItems::ESHeaderItems(){
	m_leftPosX		= 0;
	m_nCurrent		= -1;
	m_nItemsCount	= 0;

	// Allocate new root item.
	m_pRootItem		= NewItem();
	m_pCurrent		= m_pRootItem;
	}

ESHeaderItems::~ESHeaderItems(){
	// Delete all items.
	FreeItemsChain(m_pRootItem);
	m_pRootItem		= NULL;
	m_pCurrent		= NULL;
	m_nCurrent		= 0;
	m_nItemsCount	= 0;
	}

BOOL
ESHeaderItems::SetItemCount(int nCount){
	if( nCount >= MAX_ITEMS_COUNT )
		return FALSE;

	if( GotoItem(nCount-1, TRUE) != nCount - 1 ){
		return FALSE;
		}

	int nCt = FreeItemsChain(m_pCurrent->next);
	m_pCurrent->next = NULL;
	return TRUE;
	}

//////////////////////////
// Tested
/////////////////////
BOOL
ESHeaderItems::AddItem(TCHAR* pszText){
	if( m_nItemsCount >= MAX_ITEMS_COUNT )
		return FALSE;

	int nGoto = m_nItemsCount - 1;
	if( GotoItem(nGoto, FALSE) != nGoto )
		return FALSE;

	ESHeaderItem* pItemNew = NewItem(m_pCurrent, NULL);
	if( pItemNew == NULL )
		return FALSE;

	m_nCurrent		++;
	m_pCurrent		= pItemNew;
	pItemNew->sText = pszText;
	m_nItemsCount	++;
	return TRUE;
	}

int
ESHeaderItems::InsertItem(int nInsertAt, TCHAR* pszText){
	return -1;
	}

int
ESHeaderItems::DeleteItem(int nItem, int nCount /*= 1*/){
	if( nItem < 0 || nItem >= m_nItemsCount )
		return 0;

	if( GotoItem(nItem) != nItem || !m_pCurrent )
		return 0;

	ESHeaderItem* pDelete	= m_pCurrent;
	ESHeaderItem* pNext		= m_pCurrent->next;
	ESHeaderItem* pPrev		= m_pCurrent->prev;
	if( pNext )
		pNext->prev = pPrev;
	if( pPrev )
		pPrev->next = pNext;
	
	pDelete->next	= NULL;
	pDelete->prev	= NULL;
	int nCt			= FreeItemsChain(pDelete);
	return nCt;
	}

void
ESHeaderItems::DeleteAll(){
	// Delete all items.
	FreeItemsChain(m_pRootItem);
	m_pRootItem		= NULL;
	m_nItemsCount	= 0;
	}

int
ESHeaderItems::GetItemWidth	(int nItem){
	if( GotoItem(nItem) == nItem ){
		return m_pCurrent->nWidth;
		}
	return -1;
	}

BOOL
ESHeaderItems::SetItemText(int nItem, TCHAR* pszText){
	ASSERT(pszText);
	if( GotoItem(nItem) == nItem ){
		m_pCurrent->sText = pszText;
		return TRUE;
		}
	return FALSE;
	}

BOOL
ESHeaderItems::GetItemText(int nItem, CString& sItemText){
	if( GotoItem(nItem) == nItem ){
		sItemText = m_pCurrent->sText;
		return TRUE;
		}
	return FALSE;
	}

int
ESHeaderItems::GotoItem(int nItem, BOOL bCreate /*= TRUE*/){
	if( nItem < 0 ) return m_nCurrent;
	ESHeaderItem*	pItem		= m_pCurrent;
	int				nMove		= nItem - m_nCurrent;
	int				nLeftPosX	= m_leftPosX;
	ASSERT(pItem);
	if( !pItem ) return m_nCurrent;

	if( nMove > 0 ){	// Shift next
		ESHeaderItem* pPrev	= m_pRootItem;
		while( pItem ){
			if( !nMove ) break;
			pPrev		= pItem;
			nLeftPosX	= pItem->nWidth;
			pItem		= pItem->next;
			nMove --;
			}

		// Create item objects if creat flag is set.
		if( (nMove && bCreate && nItem < MAX_ITEMS_COUNT)){
			while(nMove > 0){
				pItem = NewItem(pPrev);
				pPrev = pItem;
				m_nItemsCount ++;// Increase counter.
				nMove --;
				}
			}
		}
	else{	// Shift prev.
		while( pItem ){
			if( !nMove )
				break;
			pItem = pItem->prev;
			nMove ++;
			}
		}

	// When succesfully find item with that index
	// set current position as actual.
	if( !pItem ) return m_nCurrent;
	m_pCurrent	= pItem;
	m_nCurrent	= nItem;
	m_leftPosX	= nLeftPosX;
	return nItem;
	}

ESHeaderItem*
ESHeaderItems::GetItem(int nItem){
	if( GotoItem(nItem, FALSE) == nItem ){
		return m_pCurrent;
		}
	return NULL;
	}

BOOL
ESHeaderItems::GetItemRect(ESHeaderCtrl* pCtrl, int nItem, CRect& rcItem){
	ASSERT_VALID(pCtrl);
	
	if( GotoItem(nItem, FALSE) == nItem ){
		MercuryGUI*		pGUI = MercuryGUI::GetInstance();

		CRect rcClient;
		pCtrl->GetClientRectMy(rcClient);

		int nLeftItem	= pCtrl->GetLeftItem();
		int	nLeftOffset	= -1*pCtrl->m_nLeftItemOffsetX;

		if( nItem >= nLeftItem ){
			for(int nLoop=nLeftItem; nLoop<nItem; nLoop++){
				if( nLeftOffset >= rcClient.right ){
					rcItem.SetRect(0, 0, 0, 0);
					return FALSE;
					}
				nLeftOffset += pCtrl->GetItemWidth(nLoop);
				}

			rcItem.left		= nLeftOffset;
			rcItem.top		= 0;
			rcItem.right	= rcItem.left + pCtrl->GetItemWidth(nItem);
			rcItem.bottom	= rcItem.top + pGUI->header.m_nHeight;
			if( rcItem.left < 0 )
				rcItem.left = 0;
			if( rcItem.right > rcClient.right )
				rcItem.right = rcClient.right;
			return TRUE;
			}

		rcItem.SetRect(0, 0, 0, 0);
		}
	return FALSE;
	}

int
ESHeaderItems::ItemByPoint(ESHeaderCtrl* pCtrl, CPoint pt, CRect* pRcItem /*= NULL*/){
	ASSERT_VALID(pCtrl);

	CRect rcClient;
	pCtrl->GetClientRectMy(rcClient);
	if( rcClient.PtInRect(pt) ){
		int		nItemWidth	= 0;
		int		nItem		= pCtrl->m_nLeftItem;
		int		nItemCt		= pCtrl->GetItemCount();
		int		nDecrease	= 0;
		int		nOffset		= pCtrl->m_nLeftItemOffsetX;
		CRect	rcItem		(0, 0, 0, rcClient.Height());

		while( pt.x > 0 ){
			if( nItem >= nItemCt ) return -1;

			nItemWidth		= pCtrl->GetItemWidth(nItem) - nOffset;
			rcItem.left		+= nItemWidth;

			pt.x			-= nItemWidth;
			nOffset			= 0;
			nDecrease		= 1;
			nItem			++;
			}

		rcItem.right = rcItem.left;
		rcItem.left	 = rcItem.left - nItemWidth;

		if( pRcItem ){
			*pRcItem = rcItem;
			}
		return nItem - nDecrease;
		}
	return -1;
	}

int
ESHeaderItems::FreeItemsChain(ESHeaderItem* pItem){
	int nCt = 0;
	while( pItem ){// Free items chain.
		ESHeaderItem* next = pItem->next;
		delete pItem;
		pItem = next;
		nCt ++;
		}
	return nCt;
	}

void
ESHeaderItems::ReorderItems(int nItem1, int nItem2){
	if( nItem1 == nItem2 ) return;

	ESHeaderItem* pItem1 = GetItem(nItem1);
	if( !pItem1 ) return;
	ESHeaderItem* pItem2 = GetItem(nItem2);
	if( !pItem2 ) return;

	ESHeaderItemTag* pItem1Prev = pItem1->prev, *pItem1Next = pItem1->next;
	ESHeaderItemTag* pItem2Prev = pItem2->prev, *pItem2Next = pItem2->next;

	if( abs(nItem1 - nItem2) > 1 ){ // They are not neighbores
		pItem1->next = pItem2Next;
		if( pItem2Next )
			pItem2Next->prev = pItem1;
		pItem1->prev = pItem2Prev;
		if( pItem2Prev )
			pItem2Prev->next = pItem1;

		pItem2->next = pItem1Next;
		if( pItem1Next )
			pItem1Next->prev = pItem2;
		pItem2->prev = pItem1Prev;
		if( pItem1Prev )
			pItem1Prev->next = pItem2;
		}
	else{
		if( (nItem1 - nItem2) == -1 ){
			pItem2->prev = pItem1Prev;
			if( pItem1Prev )
				pItem1Prev->next = pItem2;

			pItem2->next = pItem1;
			pItem1->prev = pItem2;
			pItem1->next = pItem2Next;
			if( pItem2Next )
				pItem2Next->prev = pItem1;
			}
		else{
			pItem1->prev = pItem2Prev;
			if( pItem2Prev )
				pItem2Prev->next = pItem1;

			pItem1->next = pItem2;
			pItem2->prev = pItem1;
			pItem2->next = pItem1Next;
			if( pItem1Next )
				pItem1Next->prev = pItem2;
			}
		}

	m_nCurrent = 0;
	m_pCurrent = m_pRootItem->next;
	}