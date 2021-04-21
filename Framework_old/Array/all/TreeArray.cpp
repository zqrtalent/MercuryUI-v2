// TreeArray.cpp : implementation file
//
#include "stdafx.h"
#include "TreeArray.h"

// CTreeArray
CTreeArray::CTreeArray()
	{
	m_pRootItem		= (TreeItem*)::malloc(sizeof(TreeItem));
	memset(m_pRootItem, 0, sizeof(TreeItem));
	m_pCurrItem		= NULL;
	m_bSortById		= TRUE;
	}

CTreeArray::~CTreeArray()
	{
	FreeTreeData(m_pRootItem);

	m_pRootItem = NULL;
	m_pCurrItem = NULL;
	}

TreeItem* 
CTreeArray::AddTreeItem(UINT nItemId, UINT nItemData, TreeItem* pBelongsTo /*= NULL*/, 
						int* pIndexAdd /*= NULL*/, TreeItem* pItemAdd /*= NULL*/)
	{
	if( pBelongsTo == NULL )
		pBelongsTo = m_pRootItem;

	int			nIndex		= 0;
	TreeItem*	pPrevItem	= NULL;
	TreeItem*	pNextItem	= pBelongsTo->pChild;
	
	TreeItem*	pItemNew	= NULL;
	if( pItemAdd )
		pItemNew			= pItemAdd;
	else
		{
		pItemNew			= (TreeItem*)::malloc(sizeof(TreeItem));
		pItemNew->nData		= nItemData;
		pItemNew->nId		= nItemId;
		pItemNew->pChild	= NULL;
		}
	pItemNew->pNext			= NULL;
	pItemNew->pParent		= pBelongsTo;

	if( m_bSortById )
		nIndex = FindIndexById(pBelongsTo, pItemNew, pPrevItem, pNextItem);

	// Insert new tree item to pointers chaine.######
	if( pPrevItem )
		pPrevItem->pNext	= pItemNew;
	else
		pBelongsTo->pChild	= pItemNew;

	if( pNextItem )
		pItemNew->pNext		= pNextItem;
	// ##############################################
	
	if( pIndexAdd )
		*pIndexAdd			= nIndex;
	return pItemNew;
	}

TreeItem*	
CTreeArray::FindItemById(TreeItem* pFind, int nId)
	{
	if( !m_pRootItem || !m_pRootItem->pChild )
		return NULL;

	TreeItem* pItem = !pFind ? m_pRootItem->pChild : pFind->pNext;
	while( pItem )
		{
		if( pItem->nId == nId )
			return pItem;
		pItem = pItem->pNext;
		}
	return NULL;
	}

int
CTreeArray::GetChildsCount(TreeItem* pParent)
	{
	if( !pParent )
		pParent = m_pRootItem;

	TreeItem*	pItem	= pParent ? pParent->pChild : NULL;
	int			nCount	= 0;
	while( pItem )
		{
		pItem = pItem->pNext; nCount ++;
		}

	return nCount;
	}


BOOL
CTreeArray::RemoveItem(TreeItem* pItem)
	{
	ASSERT( pItem );
	TreeItem* pParent	= pItem->pParent;
	if( !pParent || pParent->pChild == NULL )	
		return FALSE;

	TreeItem* pPrevItem	= pParent->pChild;
	TreeItem* pNextItem	= pItem->pNext;

	if( pPrevItem != pItem  )
		{
		// Find prev tree item. #########
		while( pPrevItem->pNext != pItem )
			pPrevItem = pPrevItem->pNext;
		// ###############################
		pPrevItem->pNext = pNextItem;
		}
	else
		pParent->pChild = pNextItem;

	pItem->pNext = NULL;
	FreeTreeData(pItem);
	return TRUE;
	}

BOOL
CTreeArray::CanMoveItem(TreeItem* pItemMove, TreeItem* pItemMoveTo)
	{
	if( !pItemMove || !pItemMoveTo ) 
		return FALSE;

	if( pItemMoveTo == pItemMove->pParent )
		return FALSE;
	else
		{
		TreeItem* pItem	= pItemMoveTo;
		while( pItem )
			{
			if( pItem == pItemMove )
				return FALSE;
			pItem = pItem->pParent;
			}
		}
	return TRUE;
	}

BOOL
CTreeArray::MoveItem(TreeItem* pItemMove, TreeItem* pItemMoveTo, int* pIndexMove /*= NULL*/)
	{
	ASSERT( pItemMove && pItemMoveTo );
	TreeItem* pParent	= pItemMove->pParent;
	if( !pParent || pParent->pChild == NULL )	
		return FALSE;

	if( !CanMoveItem(pItemMove, pItemMoveTo) )
		return FALSE;

	TreeItem* pPrevItem	= pParent->pChild;
	TreeItem* pNextItem	= pItemMove->pNext;

	// Remove from pointers chain. #######
	if( pPrevItem != pItemMove  )
		{
		// Find prev tree item. ##########
		while( pPrevItem->pNext != pItemMove )
			pPrevItem = pPrevItem->pNext;
		// ###############################
		pPrevItem->pNext = pNextItem;
		}
	else
		pParent->pChild = pNextItem;
	pItemMove->pNext = NULL;
	// ##################################

	// Add item to chain. ###############
	if( AddTreeItem(0, 0, pItemMoveTo, pIndexMove,  pItemMove) == pItemMove )
		return TRUE;
	// ##################################
	return FALSE;
	}

BOOL
CTreeArray::SetItemIdSortFlag(BOOL bSort /*= TRUE*/)
	{
	BOOL bOld	= m_bSortById;
	m_bSortById = bSort;
	return bOld;
	}

int	
CTreeArray::FindIndexById(TreeItem* pBelongsTo, TreeItem* pItem, 
						  TreeItem*& pPrevItem, TreeItem*& pNextItem)
	{
	ASSERT( pBelongsTo && pItem );
	if( pBelongsTo->pChild == NULL )
		{
		pPrevItem = NULL;
		return 0;
		}
	
	// Build sorted array by id. #######################
	AutoSortedArray arrItemsById;
	TreeItem*		pItemTemp = pBelongsTo->pChild;
	do{
		arrItemsById.Add((void*)pItemTemp->nId, (void*)pItemTemp);
		pItemTemp = pItemTemp->pNext; 
		}
	while( pItemTemp );
	// #################################################

	// Insert new item to sorted array. ################
	arrItemsById.Add((void*)pItem->nId, (void*)pItem);
	// #################################################


	bool	bExists	= false;
	int		nIndex	= arrItemsById.FindValue((void*)pItem->nId);
	int		nCount	= arrItemsById.GetCount();

	if( nIndex > 0 )
		pPrevItem	= (TreeItem*)arrItemsById.GetData(nIndex - 1);
	else
		pPrevItem	= NULL;

	if( nIndex < nCount - 1 )
		pNextItem	= (TreeItem*)arrItemsById.GetData(nIndex + 1);
	else
		pNextItem	= NULL;
	return nIndex;
	}

void
CTreeArray::FreeTreeData(TreeItem* pTree)
	{
	if( pTree == NULL )
		return;
	do
		{
		// Call function recursivly. ##
		FreeTreeData(pTree->pChild);
		// ############################

		// Free item data. ############
		TreeItem* pItem = pTree->pNext;
		OnFreeTreeData(pTree);
		free(pTree);
		pTree = pItem;
		// ############################
		}
	while( pTree );
	}

// CXmlTreeArray

CXmlTreeArray::CXmlTreeArray()
	{
	m_postLoadXmlProc	= NULL;
	}

CXmlTreeArray::~CXmlTreeArray()
	{
	FreeTreeData(m_pRootItem);
	m_pRootItem = NULL;
	m_pCurrItem = NULL;
	}

BOOL
CXmlTreeArray::Init(const CString sXmlFile)
	{
	if( !m_xml.Load(sXmlFile) )
		{
		m_strError = m_xml.GetError();
		return FALSE;
		}

	// Call post load xml function. ####
	if( m_postLoadXmlProc )
		(m_postLoadXmlProc)(&m_xml);
	// #################################
	return TRUE;
	}

BOOL
CXmlTreeArray::LoadXml(CStringArray* parrParents, CDWordArray* pArrIds, CStringArray** parrElements, int nIndex)
	{
	int			nLoop		= nIndex;
	int			nCount		= (int)parrParents->GetCount();
	CString		sParentElem;
	if( !m_pCurrItem )
		m_pCurrItem			= GetRootItem();

	if( nLoop < nCount )
		{
		sParentElem			= parrParents->GetAt(nLoop);
		if( sParentElem.IsEmpty() )
			return FALSE;
		while( m_xml.FindElem(sParentElem) )
			{
			if( !m_xml.IntoElem() )
				return FALSE;

			if( WalkThroughXml(parrElements[nLoop], pArrIds->GetAt(nLoop)) )
				{
				// Call itself recursively. #########
				TreeItem* pTmp	= m_pCurrItem;
				m_pCurrItem		= m_pCurrItem->pChild;
				LoadXml(parrParents, pArrIds, parrElements, nIndex + 1);
				m_pCurrItem		= pTmp;
				// ##################################
				}
			else
				{
				m_xml.OutOfElem	();
				break;
				}
			m_xml.OutOfElem	();
			}

		return TRUE;
		}
	
	return FALSE;
	}

BOOL
CXmlTreeArray::WalkThroughXml(CStringArray* parrElements, int nIdIndex /*= -1*/)
	{
	// Load datas. #############################
	int					nCount		= (int)parrElements->GetCount();
	int					nLoop		= 0;
	CString				sElemName;
	CMapStringToString*	pArrDatas	= new CMapStringToString;

	while( nLoop < nCount )
		{
		sElemName = parrElements->GetAt(nLoop);
		if( !sElemName.IsEmpty() && m_xml.FindElem(sElemName) )
			pArrDatas->SetAt(sElemName, m_xml.GetData());
		nLoop ++;
		}
	// #####################################

	// Add new tree item. ##################
	CString sId;
	UINT	nId = 0;
	if( (nIdIndex >= 0 && nIdIndex < parrElements->GetCount()) && 
		pArrDatas->Lookup(parrElements->GetAt(nIdIndex), sId) )
		{
		nId = atoi(sId);
		}

	if( !AddTreeItem(nId, (UINT)pArrDatas, m_pCurrItem) )
		{
		delete pArrDatas;
		return FALSE;
		}
	// #####################################
	return TRUE;
	}

BOOL
CXmlTreeArray::GetElement(TreeItem* pItem, const CString sElemName, CString& sElem)
	{
	if( !pItem )
		return FALSE;

	CMapStringToString* pMap = (CMapStringToString*)pItem->nData;
	if( !pMap )
		return FALSE;

	return pMap->Lookup(sElemName, sElem);
	}

void 
CXmlTreeArray::OnFreeTreeData(TreeItem* pTree)
	{
	if( pTree->nData )
		{
		CMapStringToString* pStrArr = (CMapStringToString*)pTree->nData;
		delete pStrArr;
		}
	}