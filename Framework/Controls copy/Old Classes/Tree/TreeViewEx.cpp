// TreeViewEx.cpp : implementation file
//
#include "stdafx.h"
#include "TreeViewEx.h"


// TreeViewEx
IMPLEMENT_DYNCREATE(TreeViewEx, CTreeView)

TreeViewEx::TreeViewEx() :
m_pRecord(NULL)
	{
	m_pFont					= NULL;
	m_nStyle				= 0L;
	m_sInvalidChars			= _T("%'*\"+?><:\\!");
	m_hCursorDrag			= NULL;
	m_hCursorDropTo			= NULL;
	m_hItemDrop				= NULL;
	m_hItemDrag				= NULL;
	m_bOptimizeSearch		= FALSE;
	m_nUIDField				= -1;
	m_nItemTextField		= -1;
	m_nParentField			= -1;
	m_nUIDFieldSlave		= -1;
	m_nItemTextFieldSlave	= -1;
	m_nParentFieldSlave		= -1;
	m_nImageMaster			= 0;
	m_nImageSlave			= 0;
	m_pArrRecUIDToHTreeItem	= new AutoSortedArrayInt64;
	m_bSelMasterItem		= TRUE;
	m_bDragIsAllowed		= TRUE;	// Drag and drop is allowed.
	m_pRecordSlave			= NULL;
	m_pArrSlaveIdToRecData	= NULL;
	m_nCheckField			= -1;

	m_arrIdToHtreeItem		.OptimizeForLargeData(TRUE);
	m_pArrRecUIDToHTreeItem->OptimizeForLargeData(TRUE);
	}

TreeViewEx::~TreeViewEx()
	{
	if( m_hCursorDrag )
		::DeleteObject(m_hCursorDrag);

	if( m_hCursorDropTo )
		::DeleteObject(m_hCursorDropTo);

	if( m_pFont )
		delete m_pFont;

	if( m_pRecordSlave )
		FreeRecDataArrayInt64(m_pArrRecUIDToHTreeItem);

	delete m_pArrRecUIDToHTreeItem;
	}

BEGIN_MESSAGE_MAP(TreeViewEx, CTreeView)
	ON_WM_DESTROY		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONUP		()
	ON_WM_NCHITTEST		()
	ON_WM_MOUSEACTIVATE	()
	ON_NOTIFY_REFLECT	(TVN_SELCHANGED,	OnTvnSelchanged)
	ON_NOTIFY_REFLECT	(TVN_BEGINDRAG,		OnTvnBegindrag)
	ON_NOTIFY_REFLECT	(TVN_ENDLABELEDIT,	OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT	(TVN_BEGINLABELEDIT, OnTvnBeginlabeledit)
END_MESSAGE_MAP  ()


// TreeViewEx diagnostics
#ifdef _DEBUG
void 
TreeViewEx::AssertValid() const
	{
	CTreeView::AssertValid();
	}

void
TreeViewEx::Dump(CDumpContext& dc) const
	{
	CTreeView::Dump(dc);
	}
#endif //_DEBUG


HTREEITEM
TreeViewEx::InsertTreeItem(long lNewUid, HTREEITEM hItemParent, 
							 CString sItemText, void* lpNewRecData)
	{
	/*
	CTreeCtrl&	ctrlTree	= GetTreeCtrl();
	long		lParentId	= 0;

	if( !hItemParent )
		{
		hItemParent = ctrlTree.GetSelectedItem();
		ASSERT( hItemParent );
		}

	if( hItemParent )
		{
		TreeItem*	pParentItem	= (TreeItem*)((HTreeItem*)hItemParent)->param;
		if( pParentItem )
			lParentId = pParentItem->nId;
		}

	// Insert new item to tree array. ######################
	void*		lpRecData	= m_pRecord->GetRecordData();
	m_pRecord->SetRecordData(lpNewRecData);
	m_pRecord->SetLong		(m_sParentField,	lParentId);
	m_pRecord->SetLong		(m_sUIDField,		lNewUid);

	HTreeItem*	pHTreeItem	= (HTreeItem*)hItemParent;
	TreeItem*	pItemParent	= pHTreeItem ? (TreeItem*)pHTreeItem->param : NULL;
	int			nNewIndex	= 0;
	TreeItem*	pItemNew	= m_arrTreeItems.AddTreeItem((UINT)lNewUid, (UINT)lpNewRecData, pItemParent, &nNewIndex);
	// #####################################################

	// Insert new item into tree. ##########################
	HTREEITEM	hItemAfter	= TVI_LAST;
	HTreeItem*	pHItemNext	= hItemParent ? ((HTreeItem*)hItemParent)->child : NULL;

	// Find insert after item. #############################
	while( pHItemNext )
		{
		if( !nNewIndex ) break;
		pHItemNext	= pHItemNext->next;
		nNewIndex	--;
		}

	if( !nNewIndex && pHItemNext )
		hItemAfter	= (HTREEITEM)pHItemNext;

	HTREEITEM	hItemNew			= ctrlTree.InsertItem(sItemText, hItemParent ? hItemParent : TVI_ROOT, hItemAfter);
	ASSERT( hItemNew );
	((HTreeItem*)hItemNew)->param	= (LONG)pItemNew;
	m_arrIdToHtreeItem.Add(lNewUid, (long)hItemNew);
	// #####################################################
	return hItemNew;*/return NULL;
	}

HTreeItem*
TreeViewEx::GetPrevItem(HTreeItem* pItem)
	{/*
	HTreeItem*	hParentItem	= pItem->parent;
	HTreeItem*	hPrevItem	= hParentItem->child;

	if( hPrevItem == pItem )
		return NULL;

	while( hPrevItem->next != pItem )
		hPrevItem = hPrevItem->next;
	return hPrevItem;*/
	}

BOOL
TreeViewEx::DeleteItem(HTREEITEM hItem)
	{/*
	CTreeCtrl&	ctrlTree	= GetTreeCtrl();
	TreeItem*	pItem		= (TreeItem*)((HTreeItem*)hItem)->param;
	ASSERT( pItem );

	// Notify item deletion. ####################
	OnPreDeleteItem(hItem);
	// ##########################################

	// Walk through tree items. #######################################
	CDWordArray	arrRecData;
	WalkThruTree(arrRecData, ((HTreeItem*)hItem)->child, TRUE);
	arrRecData.Add(pItem->nData);
	// ################################################################
	
	// Remove from tree items array. ##################################
	m_arrTreeItems.RemoveItem(pItem);
	// ################################################################

	// Free deleted item's record data. ###############################
	INT_PTR nCount	= arrRecData.GetCount();
	int iLoop	= 0;
	while( iLoop < nCount )
		{
		void*	lpRecData		= (void*)arrRecData[iLoop];
		ASSERT( lpRecData );

		void*	lpRecDataOld	= m_pRecord->GetRecordData();
		m_pRecord->SetRecordData(lpRecData);
		long	lUid			= m_pRecord->GetLong(m_nUIDField);
		int		nFind			= m_arrIdToHtreeItem.FindValue(lUid);
		if( nFind != -1 )
			m_arrIdToHtreeItem.Delete(nFind);
		m_pRecord->SetRecordData(lpRecDataOld);

		delete lpRecData;
		iLoop ++;
		}
	// ################################################################

	// Delete tree item. ##############################################
	ctrlTree.DeleteItem(hItem);
	// ################################################################
	return TRUE;*/ return FALSE;
	}

//
//	TreeViewEx::LoadMasterSlaveFromRst
//
BOOL		
TreeViewEx::LoadMasterSlaveFromRst(iODBCRst* pRstMaster, iODBCRst* pRstSlave, const CString sItemTextField, 
									const CString sUidField, const CString sParentField /*= _T("")*/, int nImageMaster /*= 0*/, CString sImageSlave /*= _T("")*/)
	{	/*
	CTreeCtrl&		treeCtrl		= GetTreeCtrl();
	m_sUIDField						= sUidField;	
	m_sItemTextField				= sItemTextField;
	m_sParentField					= sParentField;
	
	OnLoadStarted();

	// Free tree items array. #########################################
	m_arrTreeItems.RemoveAll();
	// ################################################################
	// Free id to HTREEITEM array. ####################################
	m_arrIdToHtreeItem.DeleteAll();
	// ################################################################

	m_pArrRecUIDToHTreeItem					= new AutoSortedArrayInt64();
	m_pRecord								= pRstMaster->GetRecordPtr();				// CRecord pointer.
	m_pRecordSlave							= pRstSlave ? pRstSlave->GetRecordPtr() : m_pRecordSlave;				// CRecord pointer.

	int				nIndex					= 0;										// Current index.
	int				nRecDataSz				= m_pRecord->GetRecordSize();				// Record data size in bytes.
	void*			lpRecDataBuffer			= m_pRecord->GetRecordData();

	int				nRecDataSzSlave			= m_pRecordSlave->GetRecordSize();			// Record data size in bytes.
	void*			lpRecDataBufferSlave	= m_pRecordSlave->GetRecordData();

	m_nUIDField						= m_pRecord->GetFieldID(sUidField);
	ASSERT( m_nUIDField				>= 0 );
	m_nItemTextField				= m_pRecord->GetFieldID(sItemTextField);
	ASSERT( m_nItemTextField		>= 0 );
	m_nParentField					= m_pRecord->GetFieldID(sParentField);
	ASSERT( m_nParentField			>= 0 );
	m_nImageMaster					= nImageMaster;
	m_nImageSlave					= m_pRecordSlave->GetFieldID(sImageSlave);

	m_nUIDFieldSlave				= m_pRecordSlave->GetFieldID(sUidField);
	ASSERT( m_nUIDFieldSlave		>= 0 );
	m_nItemTextFieldSlave			= m_pRecordSlave->GetFieldID(sItemTextField);
	ASSERT( m_nItemTextFieldSlave	>= 0 );
	m_nParentFieldSlave				= m_pRecordSlave->GetFieldID(sParentField);
	ASSERT( m_nParentFieldSlave		>= 0 );

	//  Delete all Items. #############################################
	treeCtrl.DeleteAllItems	();
	treeCtrl.SetRedraw		(FALSE);
	// ################################################################

	// Free record data array. ########################################
	FreeRecDataArrayInt64(m_pArrRecUIDToHTreeItem);
	// ################################################################

	m_pRecord		->SetRecordData			(lpRecDataBuffer);
	m_pRecordSlave	->SetRecordData			(lpRecDataBufferSlave);
	m_pRecord		->SetFreeDataOnDestroy	(FALSE);
	m_pRecordSlave	->SetFreeDataOnDestroy	(FALSE);

	// Slave check field. #############################################
	if( m_sCheckField )
		m_nCheckField = m_pRecordSlave->GetFieldID(m_sCheckField);
	else
		m_nCheckField = -1;
	// ################################################################

	__int64 nValue = 0, nData = 0;
	// Read records from master table. ################################
	while( !pRstMaster->IsEOF() )
		{
		// Read fields from current record. ###########################
		long lUid = m_pRecord->GetLong(m_nUIDField), lParentId = m_pRecord->GetLong(m_nParentField);
		if( m_pRecord->IsFieldNull(m_nParentField) || lUid == lParentId )
			lParentId = 0;
		// ############################################################
		
		// Allocate and Copy record data. #############################
		void* lpRecData = new BYTE[nRecDataSz];
		memcpy(lpRecData, lpRecDataBuffer, nRecDataSz);
		// ############################################################

		// Add special values to sorted array of __int64. #############
		nValue	= (((__int64)lParentId) << 32) | ((__int64)lUid);
		nData	= ((__int64)lpRecData);
		m_pArrRecUIDToHTreeItem->Add(nValue, nData);
		// ############################################################

		// Goto new record. ###########################################
		pRstMaster->MoveNext();
		// ############################################################
		}
	// ################################################################

	if( pRstSlave ) // Load directly from slave table.
		{
		// Read records from slave table. #################################
		while( !pRstSlave->IsEOF() )
			{
			// Read fields from current record. ###########################
			long lUid = m_pRecordSlave->GetLong(m_nUIDFieldSlave), lParentId = m_pRecordSlave->GetLong(m_nParentFieldSlave);
			if( m_pRecordSlave->IsFieldNull(m_nParentFieldSlave) /*|| lUid == lParentId*/ /*) // Updated 4.10.2008 ZqrTalent
				lParentId = 0;
			// ############################################################
			
			// Allocate and Copy record data. #############################
			void* lpRecData = new BYTE[nRecDataSzSlave];
			memcpy(lpRecData, lpRecDataBufferSlave, nRecDataSzSlave);
			// ############################################################

			// Add special values to sorted array of __int64. #############
			nValue	= (((__int64)lParentId) << 32) | ((__int64)(lUid|0x80000000));
			nData	= ((__int64)lpRecData);
			m_pArrRecUIDToHTreeItem->Add(nValue, nData);
			// ############################################################

			// Goto new record. ###########################################
			pRstSlave->MoveNext();
			// ############################################################
			}
		// ################################################################
		}
	else	// Load slave data from array.
		{
		// Read records from loaded slave table. ##########################
		int		nCount		= m_pArrSlaveIdToRecData->GetCount(), nLoop = 0;
		void*	lpRecData	= NULL;
		while( nLoop < nCount )
			{
			lpRecData		= (void*)m_pArrSlaveIdToRecData->GetData(nLoop);
			m_pRecordSlave->SetRecordData(lpRecData);
			// Read fields from current record. ###########################
			long lUid = m_pRecordSlave->GetLong(m_nUIDFieldSlave), lParentId = m_pRecordSlave->GetLong(m_nParentFieldSlave);
			if( m_pRecordSlave->IsFieldNull(m_nParentFieldSlave) /*|| lUid == lParentId*//* ) // Updated 4.10.2008 ZqrTalent
				lParentId = 0;
			// ############################################################

			// Add special values to sorted array of __int64. #############
			nValue	= (((__int64)lParentId) << 32) | ((__int64)(lUid|0x80000000));
			nData	= ((__int64)lpRecData);
			m_pArrRecUIDToHTreeItem->Add(nValue, nData);
			// ############################################################
			nLoop ++;
			}
		// ################################################################
		}

	BuildMasterSlaveTree(0, &treeCtrl);
	SetRedraw			(TRUE);

	// Delete binded record data buffers. #############################
	delete lpRecDataBuffer;
	delete lpRecDataBufferSlave;
	// ################################################################

	// Close recordsets. ##############################################
	if( pRstSlave )
		pRstSlave	->Close();
	pRstMaster	->Close();
	// ################################################################
	OnLoadFinished();*/
	return TRUE;
	}

//
//	TreeViewEx::BuildMasterSlaveTree
//
void				
TreeViewEx::BuildMasterSlaveTree(long lParentId, CTreeCtrl* pTreeCtrl, HTreeItem* hParent /*= NULL*/, long lStartIndex /*= 0*/)
	{/*
	bool		bExists		= false;
	long		lIndex		= lStartIndex;
	long		lEndIdx		= m_pArrRecUIDToHTreeItem->FindValuePlace(((__int64)lParentId + 1) << 32, bExists);
	ASSERT( !bExists );

	long		lCount		= m_pArrRecUIDToHTreeItem->GetCount();
	__int64		nData		= 0, nValue = 0;
	void*		lpRecData	= NULL;
	CString		sItemText;
	HTreeItem*	pHTreeItem	= NULL;
	BOOL		bIsSlave	= FALSE;
	int			iImage		= 0;

	if( lIndex >= lCount )
		return;

	while( lIndex < lEndIdx )
		{
		nData			= m_pArrRecUIDToHTreeItem->GetData	(lIndex);
		nValue			= m_pArrRecUIDToHTreeItem->Get		(lIndex);
		long lId		= (long)(nValue)&0x7FFFFFFF;
		lpRecData		= (void*)((long)nData);
		ASSERT( lpRecData );

		// Retrieve item text and insert it. ############################
		bIsSlave		= (BOOL)(((long)nValue)&0x80000000);
		if( bIsSlave )
			{
			m_pRecordSlave->SetRecordData	(lpRecData);
			m_pRecordSlave->GetString		(m_nItemTextFieldSlave, sItemText);
			iImage				= m_pRecordSlave->GetLong(m_nImageSlave);
			}
		else
			{
			m_pRecord->SetRecordData		(lpRecData);
			m_pRecord->GetString			(m_nItemTextField, sItemText);
			iImage				= m_nImageMaster;
			}

		pHTreeItem				= (HTreeItem*)pTreeCtrl->InsertItem(sItemText, (HTREEITEM)hParent);
		pHTreeItem->iImage		= iImage;
		pHTreeItem->iSelImage	= iImage;
		pHTreeItem->param		= (long)lpRecData;
		nData					= ((__int64)(pHTreeItem) << 32) | ((long)nData);
		m_pArrRecUIDToHTreeItem->SetData(lIndex, nData);
		// ##############################################################
		// if not slave item, Check for child. ##########################
		if( !bIsSlave )
			{
			// Set check master item. ###################################
			SetItemCheck(pHTreeItem, TRUE);
			// ##########################################################
			long lFirstChildIdx	= m_pArrRecUIDToHTreeItem->FindValuePlace(((__int64)lId) << 32, bExists);
			if( lFirstChildIdx < lCount && (long)(m_pArrRecUIDToHTreeItem->Get(lFirstChildIdx) >> 32) == lId )
				BuildMasterSlaveTree(lId, pTreeCtrl, pHTreeItem, lFirstChildIdx);
			}
		else
			{
			// Set checked item. ############################################
			if( m_nCheckField == -1 )
				SetItemCheck(pHTreeItem, TRUE);
			else
				SetItemCheck(pHTreeItem, m_pRecordSlave->GetLong(m_nCheckField));
			// ##############################################################
			}
		// ##############################################################
		lIndex ++;
		}*/
	}

BOOL
TreeViewEx::LoadFromRst(iODBCRst* pRst, const CString sItemTextField, 
						  const CString sUidField, const CString sParentField /*= _T("")*/)
	{/*
	CDWordArray		arrRecData;														// Array of record data pointers.
	AutoSortedArray	arrParentIdToId;												// Sorted array of Parant id to UID.
	AutoSortedArray	arrIdToRecDataPtr;												// Sorted array of id to record data pointers.
	
	OnLoadStarted();	

	CTreeCtrl&		treeCtrl		= GetTreeCtrl();
	m_sUIDField						= sUidField;	
	m_sItemTextField				= sItemTextField;
	m_sParentField					= sParentField;

	// Retrieve array of record data. #################################
	WalkThruTree(arrRecData, (HTreeItem*)treeCtrl.GetRootItem(), TRUE);
	// ################################################################
	// Free tree items array. #########################################
	m_arrTreeItems.RemoveAll();
	// ################################################################
	// Free id to HTREEITEM array. ####################################
	m_arrIdToHtreeItem.DeleteAll();
	// ################################################################

	m_pRecord						= pRst->GetRecordPtr();								// CRecord pointer.
	int				nItemTextField	= m_pRecord->GetFieldID(sItemTextField);			// Tree Item's text field index.
	int				nUidField		= m_pRecord->GetFieldID(sUidField);					// UID field index.
	int				nParentField	= m_pRecord->GetFieldID(sParentField);				// Parent field index.
	int				nIndex			= 0;												// Current index.
	int				nRecDataCt		= arrRecData.GetCount();							// Record data array size.
	int				nRecDataSz		= m_pRecord->GetRecordSize();						// Record data size in bytes.
	void*			lpRecDataBuffer	= m_pRecord->GetRecordData();

	m_nUIDField						= m_pRecord->GetFieldID(sUidField);
	ASSERT( m_nUIDField >= 0 );
	m_nItemTextField				= m_pRecord->GetFieldID(sItemTextField);
	ASSERT( m_nItemTextField >= 0 );
	m_nParentField					= m_pRecord->GetFieldID(sParentField);
	ASSERT( m_nParentField >= 0 );

	//  Delete all Items. #############################################
	treeCtrl.DeleteAllItems	();
	treeCtrl.SetRedraw		(FALSE);
	// ################################################################

	m_pRecord->SetRecordData		(lpRecDataBuffer);
	m_pRecord->SetFreeDataOnDestroy	(FALSE);

	// Loop to read all data. #########################################
	while( !pRst->IsEOF() )
		{
		// Read fields from current record. ###########################
		long lUid = m_pRecord->GetLong(nUidField), lParentId = m_pRecord->GetLong(nParentField);
		if( m_pRecord->IsFieldNull(nParentField) || lUid == lParentId )
			lParentId = 0;
		// ############################################################

		// Retrive(Allocate or get from array) record data buffer. ####
		void* lpRecData = NULL;
		if( nIndex < nRecDataCt )
			lpRecData	= (void*)arrRecData[nIndex];
		else
			lpRecData	= new BYTE[nRecDataSz];
		// ############################################################
		
		// Id to Record data pointer. #################################
		arrIdToRecDataPtr.	Add((long)lUid, (long)lpRecData);
		// Parent id to id.############################################
		arrParentIdToId.	Add(lParentId, lUid);

		// Copy record data. ##########################################
		memcpy(lpRecData, lpRecDataBuffer, nRecDataSz);
		// ############################################################

		// Goto new record. ###########################################
		pRst->MoveNext(); nIndex ++;
		// ############################################################
		}

	if( nIndex > 0 )
		{
		// Build tree array from sorted array of parent id to id. #########
		BuildTreeArray(&m_arrTreeItems, &arrParentIdToId, &arrIdToRecDataPtr, 0, NULL);
		// ################################################################
		
		TreeItem* pTreeItem = m_arrTreeItems.GetRootItem()->pChild;
		// Build Tree from tree array. ####################################
		BuildTree(pTreeItem, &treeCtrl, m_pRecord, nItemTextField);
		// ################################################################
		}

	// Free unused record data. #######################################
	long lOldRecData	= arrRecData.GetCount();
	long lNewRecData	= arrIdToRecDataPtr.GetCount();
	while( lOldRecData > lNewRecData )
		{
		lOldRecData --;
		void* lpRecData = (void*)arrRecData[lOldRecData];
		ASSERT( lpRecData );
		delete lpRecData;
		}
	// ################################################################

	if( arrParentIdToId.GetCount() )
		{
		HTreeItem*	hRootItem = (HTreeItem*)treeCtrl.GetRootItem();
		ASSERT( hRootItem );
		HTreeItem*	pRoot		= hRootItem->parent;

		TreeItem*	pItemData = (TreeItem*)((HTreeItem*)hRootItem)->param;
		ASSERT( pItemData );

		if( lpRecDataBuffer )
			delete lpRecDataBuffer;
		pRst->Close();
		
		m_pRecord->SetRecordData		((void*)pItemData->nData);
		treeCtrl.SetRedraw				(TRUE);
		treeCtrl.SelectSetFirstVisible	((HTREEITEM)hRootItem);	
		}
	else
		{
		delete lpRecDataBuffer;
		m_pRecord->SetRecordData(NULL);
		}

	// Clean up temporary arrays. #####################################
	arrRecData			.RemoveAll();	
	arrParentIdToId		.FreeExtra();
	arrIdToRecDataPtr	.FreeExtra();
	// ################################################################
	OnLoadFinished();*/
	return TRUE;
	}

void
TreeViewEx::WalkThruTree(CDWordArray& arrRecData, HTreeItemTag* pItem, BOOL bDetachDataFromItem)
	{/*
	if( pItem == NULL )
		return;

	do
		{
		TreeItem* pItemData = (TreeItem*)pItem->param;
		if( pItemData && pItemData->nData )
			{
			void* pRecData = (void*)pItemData->nData;
			ASSERT( pRecData );
			arrRecData.Add((DWORD)pItemData->nData);
			}

		if( bDetachDataFromItem )
			pItem->param	= 0L;

		// Call function recursively. ##############
		WalkThruTree(arrRecData, pItem->child, bDetachDataFromItem);
		// #########################################
		pItem				= pItem->next;
		}
	while( pItem );*/
	}

/*****************************************************
Builds array like tree structure.
*****************************************************/
void	
TreeViewEx::BuildTreeArray(CTreeArray* pArrTreeItems, 
							AutoSortedArray* pArrParentIdToUid, 
							AutoSortedArray* pArrIdToRecordData,
							int nStartIndex, TreeItem* pParent /*= NULL*/)
	{/*
	long lId		= pArrParentIdToUid->GetData(nStartIndex);
	long lParentId	= pArrParentIdToUid->Get(nStartIndex);
	int	 nFindIndex	= -1;

	do
		{
		// Find record data associated to the tree item. ##################
		nFindIndex		= pArrIdToRecordData->FindValue(lId);
		ASSERT( nFindIndex != -1 );
		void* lpRecData = (void*)pArrIdToRecordData->GetData(nFindIndex);
		ASSERT( lpRecData );
		// ################################################################

		// Add new tree item. #############################################
		TreeItem* pItemNew = pArrTreeItems->AddTreeItem(lId, (UINT)lpRecData, pParent);
		// ################################################################

		nFindIndex	= pArrParentIdToUid->FindValue(lId);
		if( nFindIndex != -1 )
			{
			// Find first child item id. ##################
			if( nFindIndex > 0 )
				{
				do
					{
					nFindIndex --;
					}
				while( pArrParentIdToUid->Get(nFindIndex) == lId );
				nFindIndex ++;
				}
			// ############################################

			// Call function recursively. #################
			BuildTreeArray(pArrTreeItems, pArrParentIdToUid, pArrIdToRecordData, nFindIndex, pItemNew);
			// ############################################
			}

		// Goto next tree item. ###########################
		nStartIndex ++;
		if( nStartIndex < pArrParentIdToUid->GetCount() )
			{
			lId			= pArrParentIdToUid->GetData(nStartIndex);
			lParentId	= pArrParentIdToUid->Get	(nStartIndex);
			}
		else
			{
			lId			= -1;
			lParentId	= -1;
			}
		// ################################################
		}
	while( pArrParentIdToUid->Get(nStartIndex-1) == lParentId );*/
	}

void 
TreeViewEx::BuildTree(TreeItem* pCurrItem, CTreeCtrl* pTreeCtrl, CRecord* pRec,
						int nItemTextField, HTREEITEM hParent /*= NULL*/)
	{/*
	CString sItemText; 
	if( hParent == NULL ) hParent = TVI_ROOT;
	while( pCurrItem )
		{
		void* lpRecData = (void*)pCurrItem->nData;
		ASSERT( lpRecData );

		// Change record data for retrieving current record fields. ####
		void* lpOldRecData = pRec->GetRecordData();
		pRec->SetRecordData(lpRecData);
		// #############################################################
		// Get tree item text. #########################################
		pRec->GetString(nItemTextField, sItemText);
		// #############################################################
		// Add new tree item .##########################################
		HTreeItem* pHTreeItem	= (HTreeItem*)pTreeCtrl->InsertItem(sItemText, hParent);
		ASSERT( pHTreeItem );
		// #############################################################
		// Attach tree item to current record data. ####################
		pCurrItem->nData		= (UINT)lpRecData;
		// Attach item to tree item structure. #########################
		pHTreeItem->param		= (LONG)pCurrItem;
		// #############################################################
		// If optimize search by id. ###################################
		if( m_bOptimizeSearch )
			m_arrIdToHtreeItem.Add(pRec->GetLong(m_nUIDField), (long)pHTreeItem);
		// #############################################################
		// Call function recursively. ##################################
		BuildTree(pCurrItem->pChild, pTreeCtrl, pRec, nItemTextField, (HTREEITEM)pHTreeItem);
		// #############################################################
		pRec->SetRecordData(lpOldRecData);
		pCurrItem				= pCurrItem->pNext;
		}*/
	}

BOOL			
TreeViewEx::FindItemById(HTreeItem* pHItem, CRecord* pRec, int nIdField, int nId, HTREEITEM& hItemFound)
	{/*
	while( pHItem )
		{
		TreeItem* pItem = (TreeItem*)pHItem->param;
		if( pItem && pItem->nData )
			{
			void*	lpRecData	= pRec->GetRecordData();
			pRec->SetRecordData((void*)pItem->nData);
			long	lId			= pRec->GetLong(nIdField);
			pRec->SetRecordData(lpRecData);

			if( lId == nId )
				{
				hItemFound = (HTREEITEM)pHItem;
				return TRUE;
				}
			}

		if( FindItemById(pHItem->child, pRec, nIdField, nId, hItemFound) )
			return TRUE;
		pHItem = pHItem->next;
		}*/
	return FALSE;
	}

HTREEITEM
TreeViewEx::FindItemById2(int nId, int nParentId, BOOL bSelect /*= FALSE*/)
	{
	if( m_bOptimizeSearch )
		{/*
		__int64		nIdFind	= (((__int64)nParentId) << 32)|(0x0000000080000000|((__int64)nId));
		int			nIndex	= m_pArrRecUIDToHTreeItem->FindValue(nIdFind);
		HTREEITEM	hRet	= (nIndex > -1) ? ((HTREEITEM)(m_pArrRecUIDToHTreeItem->GetData(nIndex) >> 32)) : NULL;
		if( hRet && bSelect )
			{
			CTreeCtrl& tree = GetTreeCtrl();
			tree.SelectItem(hRet);
			}
		return hRet;*/
		}
	else
		{/*
		CTreeCtrl&	treeCtrl	= GetTreeCtrl();
		HTreeItem*	pHItem		= (HTreeItem*)treeCtrl.GetRootItem();
		HTREEITEM	hItem		= NULL;

		TreeViewEx::FindItemById(pHItem, m_pRecord, m_nUIDField, nId, hItem);
		if( hItem && bSelect )
			{
			CTreeCtrl& tree = GetTreeCtrl();
			tree.SelectItem(hItem);
			}
		return hItem;*/
		}
	return NULL;
	}

HTREEITEM	
TreeViewEx::FindItemById(int nId, BOOL bSelect /*= FALSE*/)
	{
	if( m_bOptimizeSearch )
		{
		int			nIndex	= m_arrIdToHtreeItem.FindValue(nId);
		HTREEITEM	hRet	= (nIndex > -1) ? ((HTREEITEM)m_arrIdToHtreeItem.GetData(nIndex)) : NULL;
		if( hRet && bSelect )
			{
			CTreeCtrl& tree = GetTreeCtrl();
			tree.SelectItem(hRet);
			}
		return hRet;
		}
	else
		{
		CTreeCtrl&	treeCtrl	= GetTreeCtrl();
		HTreeItem*	pHItem		= (HTreeItem*)treeCtrl.GetRootItem();
		HTREEITEM	hItem		= NULL;

		TreeViewEx::FindItemById(pHItem, m_pRecord, m_nUIDField, nId, hItem);
		if( hItem && bSelect )
			{
			CTreeCtrl& tree = GetTreeCtrl();
			tree.SelectItem(hItem);
			}
		return hItem;
		}
	return NULL;
	}

void		
TreeViewEx::FreeTreeRecData()
	{
	// Free record data attached to each tree item. ####################
	CDWordArray		arrRecDatas;
	CTreeCtrl&		treeCtrl	= GetTreeCtrl();
	WalkThruTree(arrRecDatas, ((HTreeItem*)treeCtrl.GetRootItem()), TRUE);

	int iLoop	= 0;
	int nCount	= arrRecDatas.GetCount();
	while( iLoop < nCount )
		{
		void* lpRecData = (void*)arrRecDatas[iLoop];
		if( lpRecData )
			delete lpRecData;
		iLoop ++;
		}
	// #################################################################
	}

void				
TreeViewEx::FreeRecDataArrayInt64(AutoSortedArrayInt64* pArrRecData)
	{
	long	lCount	= pArrRecData->GetCount();
	long	lLoop	= 0;
	__int64 nData	= 0;

	while( lLoop < lCount )
		{
		nData			= pArrRecData->GetData(lLoop);
		void* lpRecData = (void*)((long)nData);
		if( lpRecData )
			delete lpRecData;
		lLoop ++;
		}

	pArrRecData->DeleteAll();
	}

//*****************************************************************
//	HBITMAP GetBitmap32bppFromDC(CDC* pSrc,int xSrc,int ySrc,
//								 int nWidth,int nHeight)
//*****************************************************************
HBITMAP 
GetBitmap32bppFromDC(CDC* pSrc, int xSrc, int ySrc, 
					int nWidth, int nHeight)
	{
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = 32;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = nHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = nWidth;
	
	void*   lpMap = NULL;
	HBITMAP hDib  = CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	
	memDC.SelectObject(hDib);
	memDC.BitBlt(0, 0, nWidth, nHeight, pSrc, xSrc, ySrc, SRCCOPY);
	memDC.DeleteDC();
	return hDib;
	}

void 
TreeViewEx::CreateDragImage(CTreeCtrl* pTree, CString sItemText, HTREEITEM hItemDrag, CImageList* &pImgList)
	{
	CImageList* pImgListTree	= pTree->GetImageList(TVSIL_NORMAL);
//	CString		sItemText		= pTree->GetItemText(hItemDrag);
	int			nImageIndex		= 0;

	TVITEM		item;
	item.mask					= TVIF_STATE;
	pTree->GetItem(&item);
	if( item.mask&TVIS_EXPANDED ) 
		nImageIndex				= 1;

	IMAGEINFO imgInfo;
	pImgListTree->GetImageInfo(nImageIndex, &imgInfo);

	int			nIndent			= pTree->GetIndent();
	CDC*		pTreeDC			= pTree->GetDC();
	CFont*		pFont			= pTree->GetFont(); pTreeDC->SelectObject(pFont);
	CSize		szText			= pTreeDC->GetTextExtent(sItemText, sItemText.GetLength());
	CRect		rcImage			= imgInfo.rcImage;
	CRect		rcText		(rcImage.Width() + 4, 0, rcImage.Width() + szText.cx + 4, szText.cy);
	CRect		rcDragImage	(0, 0, rcText.Width() + rcImage.Width() + 4, max(rcText.Height(), rcImage.Height()));
	HBITMAP		hTempImage		= GetBitmap32bppFromDC(pTreeDC, 0, 0, rcDragImage.Width(), rcDragImage.Height());
	COLORREF	crBkgnd			= RGB(255, 255, 255);
	
	// Draw item text on device context. ##########################################
	CDC			memDC;
	memDC.CreateCompatibleDC(pTreeDC);
	memDC.SelectObject		(pFont);
	memDC.SelectObject		(hTempImage);
	memDC.FillSolidRect		(rcDragImage, crBkgnd);
	memDC.DrawText			(sItemText, sItemText.GetLength(), rcText, DT_VCENTER);
	// ###########################################################################

	// Draw image from image list to device context. #############################
	pImgListTree->DrawIndirect(	&memDC, nImageIndex, CPoint(0, 0), 
								CSize(rcImage.Width(), rcImage.Height()), 
								CPoint(rcImage.left, rcImage.top));
	// ###########################################################################

	HBITMAP		hImage			= GetBitmap32bppFromDC(&memDC, 0, 0, rcDragImage.Width(), rcDragImage.Height());
	if( hImage )
		{
		pImgList	= new CImageList();
		CBitmap bmpImage;
		bmpImage.Attach	(hImage);
		pImgList->Create(rcDragImage.Width(), rcDragImage.Height(), ILC_COLOR24|ILC_MASK, 1, crBkgnd);
		pImgList->Add	(&bmpImage, crBkgnd);
		}

	::DeleteObject			(hImage);
	::DeleteObject			(hTempImage);
	memDC.DeleteDC			();
	pTree->ReleaseDC		(pTreeDC);
	}

CString	
TreeViewEx::GetCurrRecordFieldAsString(const CString sField)
	{
	if( m_pRecord && !m_pRecord->HaveNullRecordData() )
		return m_pRecord->GetString(m_pRecord->GetFieldID(sField));
	else
		return _T("");
	}

long	
TreeViewEx::GetCurrRecordFieldAsLong(const CString sField)
	{
	if( m_pRecord && !m_pRecord->HaveNullRecordData() )
		return m_pRecord->GetLong(m_pRecord->GetFieldID(sField));
	else
		return -1;
	}

CString		
TreeViewEx::GetFieldAsString(HTREEITEM hItem, const CString sField)
	{
	ASSERT( m_pRecord );
	ASSERT( hItem && ((HTreeItem*)hItem)->param );

	TreeItem*	pItem		 = (TreeItem*)((HTreeItem*)hItem)->param;
	ASSERT( pItem->nData );
	void*		lpOldRecData = m_pRecord->GetRecordData();
	CString		sRet		 = _T("");

	m_pRecord->SetRecordData((void*)pItem->nData);
	m_pRecord->GetString	(m_pRecord->GetFieldID(sField), sRet);
	m_pRecord->SetRecordData(lpOldRecData);
	return sRet;
	}

long
TreeViewEx::GetFieldAsLong(HTREEITEM hItem, const CString sField)
	{
	ASSERT( m_pRecord );
	ASSERT( hItem && ((HTreeItem*)hItem)->param );

	TreeItem*	pItem		 = (TreeItem*)((HTreeItem*)hItem)->param;
	ASSERT( pItem->nData );
	void*		lpOldRecData = m_pRecord->GetRecordData();
	long		lRet		 = -1;

	m_pRecord->SetRecordData((void*)pItem->nData);
	m_pRecord->GetLong		(m_pRecord->GetFieldID(sField), lRet);
	m_pRecord->SetRecordData(lpOldRecData);
	return lRet;
	}

void		
TreeViewEx::RefreshItem(HTREEITEM hItemRefresh /*= NULL*/)
	{
	/*
	if( hItemRefresh == NULL )
		{
		if( m_pRecord && !m_pRecord->HaveNullRecordData() )
			{
			CTreeCtrl&	ctrlTree	= GetTreeCtrl();
			HTREEITEM	hSelItem	= ctrlTree.GetSelectedItem();
			ASSERT( hSelItem );
			ctrlTree.SetItemText(hSelItem, m_pRecord->GetString(m_pRecord->GetFieldID(m_sItemTextField)));
			}
		}
	else
		{
		if( m_pRecord )
			{
			TreeItem* pItem = (TreeItem*)((HTreeItem*)hItemRefresh)->param;
			if( pItem && pItem->nData )
				{
				void* lpRecData = m_pRecord->GetRecordData();
				m_pRecord->SetRecordData((void*)pItem->nData);
				CTreeCtrl&	ctrlTree	= GetTreeCtrl();
				ctrlTree.SetItemText(hItemRefresh, m_pRecord->GetString(m_pRecord->GetFieldID(m_sItemTextField)));
				m_pRecord->SetRecordData(lpRecData);
				}
			}
		}
	*/
	}

BOOL
TreeViewEx::CanMoveItem(HTREEITEM hItemMove, HTREEITEM hItemMoveTo)
	{/*
	if( !hItemMove || !hItemMoveTo ) 
		return FALSE;
	HTreeItem* pItemMove	= (HTreeItem*)hItemMove;
	HTreeItem* pItemMoveTo	= (HTreeItem*)hItemMoveTo;

	if( IsItemReadOnly(GetFieldAsLong((HTREEITEM)pItemMove, m_sUIDField)) )
		return FALSE;

	if( pItemMoveTo == pItemMove->parent )
		return FALSE;
	else
		{
		HTreeItem* pItem	= (HTreeItem*)hItemMoveTo;
		while( pItem )
			{
			if( pItem == pItemMove )
				return FALSE;
			pItem = pItem->parent;
			}
		}*/
	return TRUE;
	}

BOOL
TreeViewEx::MoveItem(HTREEITEM hItemMove, HTREEITEM hItemMoveTo, int nInsertAfter /*= -1*/)
	{
	if( !hItemMoveTo || !hItemMove )
		return FALSE;
	
	HTreeItem*	pItemMove		= (HTreeItem*)hItemMove;
	HTreeItem*	pItemMoveTo		= (HTreeItem*)hItemMoveTo;
	HTreeItem*	pItemMoveParent	= pItemMove->parent;
	HTreeItem*	pItemChild		= pItemMove->child;
	TreeItem*	pItem			= (TreeItem*)pItemMove->param;
	ASSERT( pItem );

	
	
	CTreeCtrl&	treeCtrl		= GetTreeCtrl();
//	CString		sItemText		= treeCtrl.GetItemText(hItemMove);
	
	void*		lpRecData		= (void*)pItem->nData;
	void*		lpRecDataOld	= m_pRecord->GetRecordData();
	m_pRecord->SetRecordData(lpRecData);
	CString		sItemText		= m_pRecord->GetString(m_nItemTextField);
	m_pRecord->SetRecordData(lpRecDataOld);
	
	// Notify item deletion. ####################
	OnPreDeleteItem(hItemMove);
	// ##########################################
	if( treeCtrl.DeleteItem(hItemMove) )
		{
		// Determine insert after item. ###########
		HTREEITEM	hInstertAfter	= (HTREEITEM)pItemMoveTo->child;
		int			iLoop			= 0;
		if( nInsertAfter == -1 )
			hInstertAfter			= TVI_FIRST;
		else
			{
			while( iLoop < nInsertAfter )
				{
				if( !hInstertAfter )
					break;
				hInstertAfter = (HTREEITEM)((HTreeItem*)hInstertAfter)->next;
				iLoop ++;
				}
			}
		// ########################################

		// Insert first item and then build other ones. ##########
		int nItemTextField		= m_pRecord->GetFieldID(m_sItemTextField);
		HTreeItem* pItemInsert	= (HTreeItem*)treeCtrl.InsertItem(sItemText, hItemMoveTo, hInstertAfter ? hInstertAfter : TVI_LAST);
		pItemInsert->param		= (LONG)pItem;
		treeCtrl.SetRedraw(FALSE);
		BuildTree(pItem->pChild, &treeCtrl, m_pRecord, nItemTextField, (HTREEITEM)pItemInsert);
		treeCtrl.SetRedraw(TRUE);

		// Expand move to item. #################################
		treeCtrl.Expand(hItemMoveTo, TVE_EXPAND);
		// #######################################################
		}

	return FALSE;
	}

// TreeViewEx message handlers
void 
TreeViewEx::OnInitialUpdate()
	{
	CTreeView::OnInitialUpdate();
	CTreeCtrl& ctrlTree		= GetTreeCtrl();

	// Set System font to tree control. #####################
	NONCLIENTMETRICS ncm;
	ncm.cbSize				= sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, NULL);

	m_pFont					= new CFont();
	OnPreCreateFont(&ncm.lfCaptionFont);
	m_pFont->CreateFontIndirect(&ncm.lfCaptionFont);
	ctrlTree.SetFont(m_pFont);
	// ######################################################

	// Modify tree control's style. #########################
	ctrlTree.ModifyStyle(0L, m_nStyle);
	// ######################################################

	ctrlTree.SetIndent(1.1*ctrlTree.GetIndent());
	}

void 
TreeViewEx::OnDestroy()
	{
	if( !m_pRecordSlave )
		FreeTreeRecData();

	if( m_pRecord )
		m_pRecord->GetRecordData();
	
	CTreeView::OnDestroy();
	}

void 
TreeViewEx::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
	{
	LPNMTREEVIEW pNMTreeView	= reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	ASSERT(pNMTreeView->itemNew.hItem != NULL);

	if( m_pRecordSlave )
		{
		HTreeItem*	pTreeItem	= (HTreeItem*)pNMTreeView->itemNew.hItem;
		void*		lpRecData	=  (void*)pTreeItem->param;
		CRecord*	pRec		=  CRecord::FromRecordDataPtr(lpRecData);
		ASSERT( pRec );
		pRec->SetRecordData(lpRecData);

		if( m_pRecord == pRec )			// Master record
			{
			m_bSelMasterItem = TRUE;
			}
		else
		if( m_pRecordSlave == pRec )	// Slave record.
			{
			m_bSelMasterItem = FALSE;
			}
		else
			ASSERT( FALSE );
		}
	else
		{
		// Retrieve tree item structure from htreeitem. ###################
		TreeItem*	pTreeItem		= (TreeItem*)((HTreeItem*)pNMTreeView->itemNew.hItem)->param;
		if( !pTreeItem ) return;
		// ################################################################
		
		// Change record data. ############################################
		void*		lpRecData		= (void*)pTreeItem->nData;
		ASSERT( lpRecData );
		m_pRecord->SetRecordData(lpRecData);
		// ################################################################
		}

	// call virtual function.
	OnItemChanged(pNMTreeView);
	*pResult = 0;
	}

WNDPROC wndProcOld = NULL;

LRESULT CALLBACK 
EditLabelProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	CEdit*	pLabelEdit	= (CEdit*)CWnd::FromHandle(hwnd);
	ASSERT( pLabelEdit );

	if( uMsg == WM_CHAR ) 
		{
		LRESULT			lRet		= 0L;
		char			cReplace	= '?';
		TreeViewEx*	pTreeView	= (TreeViewEx*)pLabelEdit->GetParent();
		ASSERT( pTreeView );

		if( wParam != VK_DELETE && wParam != VK_BACK )
			{
			CString sInvalidChars = pTreeView->GetInvalidChars();
			if( sInvalidChars.Find((char)wParam) != -1 )
				{
				MessageBeep(-1);
				return 0;
				}
			lRet = (wndProcOld)(hwnd, uMsg, cReplace, lParam);
			}
		else
			{
			lRet = (wndProcOld)(hwnd, uMsg, wParam, lParam);
			}

		CString	sNewLabelText;
		pLabelEdit->GetWindowText(sNewLabelText);
		if( sNewLabelText.Replace(cReplace, (char)wParam) )
			{
			int nSel = pLabelEdit->GetSel();
			pLabelEdit->SetWindowText(sNewLabelText);
			pLabelEdit->SetSel		 (nSel);
			}
		return lRet;
		}
	else
	if( uMsg == WM_SETFOCUS )
		{
		CString sNewText;
		pLabelEdit->GetWindowText(sNewText);

		HTreeItem*	pItem		= (HTreeItem*)GetProp(pLabelEdit->m_hWnd, _T("HTREEITEM"));
		ASSERT( pItem );
		CTreeCtrl*	pTreeCtrl	= (CTreeCtrl*)pLabelEdit->GetParent();
		CString		sItemText	= pTreeCtrl->GetItemText((HTREEITEM)pItem);
		pLabelEdit->SetWindowText(sItemText);

		return (wndProcOld)(hwnd, WM_SETFOCUS, wParam, lParam);
		}
	else
	if( uMsg == WM_KILLFOCUS )
		{
		RemoveProp(pLabelEdit->m_hWnd, _T("HTREEITEM"));
		return (wndProcOld)(hwnd, WM_KILLFOCUS, wParam, lParam);
		}
	else
		return (wndProcOld)(hwnd, uMsg, wParam, lParam);
	}

void 
TreeViewEx::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
	{
	LPNMTVDISPINFO	pTVDispInfo	= reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	*pResult					= 0;
	CTreeCtrl&		treeCtrl	= GetTreeCtrl();
	CEdit*			pLabelEdit	= treeCtrl.GetEditControl();
	ASSERT( pLabelEdit );

	// Set HTREEITEM as label edit's property. ###############
	HTreeItem* pItem = (HTreeItem*)pTVDispInfo->item.hItem;
	SetProp(pLabelEdit->m_hWnd, _T("HTREEITEM"), (HANDLE)pItem);
	// #######################################################

	// Change window proc address. ###########################
	wndProcOld					= (WNDPROC)::SetWindowLong(pLabelEdit->GetSafeHwnd(), GWL_WNDPROC, ((LONG)EditLabelProc));
	// #######################################################
	}

void 
TreeViewEx::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
	{
	LPNMTVDISPINFO	pTVDispInfo		= reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	CTreeCtrl&		treeCtrl		= GetTreeCtrl();
	int				nField			= m_pRecord->GetFieldID(m_sItemTextField);
	UINT			nFieldSize		= m_pRecord->GetFieldSize(nField);
	CString			sOldValue		= m_pRecord->GetString(nField);
	BOOL			bSuccess		= FALSE;
	CString			sNewLabelText	= _T("");

	// Don't edit if read only item. ###################
	long			lUid		= m_pRecord->GetLong(m_pRecord->GetFieldID(m_sUIDField));
	if( IsItemReadOnly((UINT)lUid) )
		return;
	// #################################################

	CEdit*			pEditControl	= treeCtrl.GetEditControl();
	if( pEditControl )
		pEditControl->GetWindowText(sNewLabelText);
	
	// Check field size. #############################
	if( sNewLabelText.GetLength() > nFieldSize )
		sNewLabelText = sNewLabelText.Left(nFieldSize);
	// ###############################################

	if( sNewLabelText == sOldValue )
		return;
	
	m_pRecord->SetString(nField, sNewLabelText);
	OnSaveTreeItemText(bSuccess, sNewLabelText);

	if( !bSuccess )
		m_pRecord->SetString(nField, sOldValue);
	else
		RefreshItem();
	*pResult = 0;
	}

void		
TreeViewEx::AllowDragAndDrop(BOOL bAllow /*= FALSE*/)
	{
	m_bDragIsAllowed = bAllow;
	}

void 
TreeViewEx::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
	{
	if( !m_bDragIsAllowed )
		return;

	LPNMTREEVIEW	lpnmtv		= (LPNMTREEVIEW)pNMHDR;
	*pResult					= 0;	// allow drag
	CTreeCtrl&		treeCtrl	= GetTreeCtrl();
	CImageList*		piml		= NULL;
	POINT			ptOffset;
	RECT			rcItem;
	HTREEITEM		hItemDrag	= lpnmtv->itemNew.hItem;

	// Don't drag if read only item. ###################
	TreeItem*		pItemDrag	= (TreeItem*)((HTreeItem*)hItemDrag)->param;
	void*			lpRecDataOld= m_pRecord->GetRecordData();
	m_pRecord->SetRecordData((void*)pItemDrag->nData);
	long			lUid		= m_pRecord->GetLong(m_pRecord->GetFieldID(m_sUIDField));
	CString			sItemText	= m_pRecord->GetString(m_pRecord->GetFieldID(m_sItemTextField));
	m_pRecord->SetRecordData(lpRecDataOld);
	if( IsItemReadOnly((UINT)lUid) )
		return;
	// #################################################
	
	CreateDragImage(&treeCtrl, sItemText, hItemDrag, piml);
	if( piml == NULL )
		return;

	// get the bounding rectangle of the item being dragged (rel to top-left of control)
	if( treeCtrl.GetItemRect(hItemDrag, &rcItem, TRUE) )
		{
		CPoint ptDragBegin;
		int nX, nY;
		// get offset into image that the mouse is at
		// item rect doesn't include the image
		ptDragBegin = lpnmtv->ptDrag;
		ImageList_GetIconSize(piml->GetSafeHandle(), &nX, &nY);
		ptOffset.x	= (ptDragBegin.x - rcItem.left) + (nX - (rcItem.right - rcItem.left));
		ptOffset.y	= nY -1;//(ptDragBegin.y - rcItem.top) + (nY - (rcItem.bottom - rcItem.top));
		// convert the item rect to screen co-ords, for use later.
		MapWindowPoints(NULL, &rcItem);
		}
	else
		{
		GetWindowRect(&rcItem);
		ptOffset.x = ptOffset.y = 8;
		}

	ptOffset.y = 16;
	BOOL bDragBegun = piml->BeginDrag(0, ptOffset);
	if( !bDragBegun )
		{
		delete piml;
		return;
		}

	CPoint ptDragEnter = lpnmtv->ptDrag;
	ClientToScreen(&ptDragEnter);
	if( !piml->DragEnter(NULL, ptDragEnter) )
		{
		delete piml;
		return;
		}
	delete piml;

	// set the focus here, so we get a WM_CANCELMODE if needed
	SetFocus();

	// redraw item being dragged, otherwise it remains (looking) selected 
	InvalidateRect	(&rcItem, TRUE);
	UpdateWindow	();

	// Hide the mouse cursor, and direct mouse input to this window 
	SetCapture		(); 
	m_hItemDrag = hItemDrag;
	}

void 
TreeViewEx::OnMouseMove(UINT nFlags, CPoint point)
	{
	if( m_hItemDrag != NULL && nFlags&MK_LBUTTON )
		{
		CTreeCtrl&	treeCtrl	= GetTreeCtrl();
		CPoint		pt			= point;
		ClientToScreen(&pt);
		
		/* drag the item to the current position */
		CImageList::DragMove		(pt);
		CImageList::DragShowNolock	(FALSE);

		if( ::WindowFromPoint(pt) != treeCtrl )
			{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
			m_hItemDrop	= NULL;
			}
		else
			{
			TVHITTESTINFO tvhti;
			tvhti.pt			= pt;
			treeCtrl.ScreenToClient(&tvhti.pt);
			HTREEITEM hItemSel	= treeCtrl.HitTest(&tvhti);
			treeCtrl.SelectDropTarget(tvhti.hItem);
			
			// Determine wether can we move item. ##########
			if( !CanMoveItem(m_hItemDrag, tvhti.hItem) )
				tvhti.hItem		= NULL;
			// #############################################

			m_hItemDrop			= tvhti.hItem;
			HCURSOR	hCursor		= tvhti.hItem ? m_hCursorDropTo : m_hCursorDrag;
			if( hCursor )
				SetCursor(hCursor);
			else
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			}
		CImageList::DragShowNolock(TRUE);
		}
	else
	if( m_hItemDrag != NULL )
		{
		// Cancel if LBUTTON is not pressed, during drag.
		CImageList::DragLeave	(NULL);
		CImageList::EndDrag		();
		ReleaseCapture			();
		ShowCursor				(TRUE);
		
		m_hItemDrag				= NULL;
		m_hItemDrop				= NULL;
		CTreeCtrl&	treeCtrl	= GetTreeCtrl();
		treeCtrl.SelectDropTarget(NULL);
		// ##############################################
		}

	CTreeView::OnMouseMove(nFlags, point);
	}

void 
TreeViewEx::OnLButtonUp(UINT nFlags, CPoint point)
	{
	if( m_hItemDrag != NULL )
		{
		CImageList::DragLeave	(NULL);
		CImageList::EndDrag		();
		
		ReleaseCapture	();
		ShowCursor		(TRUE);

		// Move tree items. ######################
		if( m_hItemDrop )
			{
			BOOL bCancelMove	= FALSE;
			int	 nInsertAfter	= -1;

			OnMoveItem(bCancelMove, m_pRecord, GetRecordData(m_hItemDrag), GetRecordData(m_hItemDrop));
			if( !bCancelMove )
				{
				m_arrTreeItems.MoveItem(((TreeItem*)((HTreeItem*)m_hItemDrag)->param), 
										((TreeItem*)((HTreeItem*)m_hItemDrop)->param), &nInsertAfter);
				MoveItem(m_hItemDrag, m_hItemDrop, nInsertAfter - 1);
				}
			}
		// #######################################
		
		m_hItemDrag				= NULL;
		m_hItemDrop				= NULL;
		CTreeCtrl&	treeCtrl	= GetTreeCtrl();
		treeCtrl.SelectDropTarget(NULL);
		}
	else
		{
		
		}

	CTreeView::OnLButtonUp(nFlags, point);
	}

//
//	[protected handler] TreeViewEx::OnMouseActivate
//
int		
TreeViewEx::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
	{
	if( message == WM_LBUTTONDOWN )
		{
		DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE); 
		if( dwStyle&TVS_CHECKBOXES )
			{
			CPoint		point;
			GetCursorPos	(&point);
			ScreenToClient	(&point);

			CTreeCtrl&	treeCtrl	= GetTreeCtrl();
			UINT		uFlags		= 0;
			HTREEITEM	hItem		= treeCtrl.HitTest(point, &uFlags);
			if( hItem && uFlags&TVHT_ONITEMSTATEICON )
				{
				BOOL bCheck = GetItemCheck((HTreeItem*)hItem);
				SetItemCheckRecursively((HTreeItem*)hItem, !bCheck);
	
				if( treeCtrl.GetItemState(hItem, TVIS_EXPANDED)&TVIS_EXPANDED && ((HTreeItem*)hItem)->child )
					{
					Invalidate();
					}
				else
					{
					CRect rcRedraw;
					treeCtrl.GetItemRect(hItem, rcRedraw, FALSE);
					InvalidateRect(rcRedraw);
					}
				return MA_ACTIVATEANDEAT;
				}
			}
		}

	if( pDesktopWnd->IsKindOf(RUNTIME_CLASS(CDialog)) )
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	return CTreeView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
 
//
//	[public] TreeViewEx::Attach
//
BOOL			
TreeViewEx::Attach(HWND hWndAttachTo)
	{
	if( !SubclassWindow(hWndAttachTo) )
		return FALSE;

	OnInitialUpdate	();
	return TRUE;
	}

//
//	[public] TreeViewEx::SetItemCheckRecursively
//
void
TreeViewEx::SetItemCheckRecursively(HTreeItem* pItem, BOOL bCheck /*= TRUE*/)
	{
	if( !pItem  )
		return;
	SetItemCheck(pItem, bCheck);
	pItem = pItem->child;
	while( pItem )
		{
		SetItemCheckRecursively(pItem, bCheck);
		pItem = pItem->next;
		}
	}