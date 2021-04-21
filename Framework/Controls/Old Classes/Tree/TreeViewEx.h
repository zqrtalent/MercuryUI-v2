#pragma once
#include "..\..\Array\AutoSortedArray.h"
#include "..\..\Array\AutoSortedArrayInt64.h"
#include "..\..\Array\TreeArray.h"

// HTREEITEM structure.
typedef struct HTreeItemTag
	{
	HTreeItemTag*	parent;
	HTreeItemTag*	next;
	HTreeItemTag*	child;
	LPWSTR			lpszText;
	LONG			param;
	WORD			wWord1;
	WORD			iImage;
	WORD			iSelImage;
	} HTreeItem, *PHTreeItem;


// TreeViewEx view

class CRecord;
class TreeViewEx : public CTreeView
{
	DECLARE_DYNCREATE(TreeViewEx)

protected:
	TreeViewEx();           // protected constructor used by dynamic creation
	virtual ~TreeViewEx();

public:
#ifdef _DEBUG
	virtual void AssertValid()					const;
	virtual void Dump		(CDumpContext& dc)	const;
#endif

public:		// public methods. #########################################################################
	CRecord*	GetRecordPointer			();
	void		RefreshItem					(HTREEITEM hItemRefresh = NULL); /* If null then selected item. */
	CString&	GetInvalidChars				();
	void		SetReadOnlyItem				(long nUidItem);
	BOOL		IsItemReadOnly				(long nUidItem);
	CString		GetCurrRecordFieldAsString	(const CString sField);
	long		GetCurrRecordFieldAsLong	(const CString sField);
	CString		GetFieldAsString			(HTREEITEM hItem, const CString sField);
	long		GetFieldAsLong				(HTREEITEM hItem, const CString sField);
	void		SetDragAndDropCursors		(HCURSOR hCurDrag, HCURSOR hCurDropTo);
	/* Optimize search by ID of record */
	BOOL		OptimizeIdSearch			(BOOL bOptimize); // Set that mode before load tree.
	/* Set item check state *//*22.1.2009*/
	void		SetItemCheck				(HTreeItem* pItem, BOOL bCheck = TRUE);
	BOOL		GetItemCheck				(HTreeItem* pItem);
	void		SetItemCheckRecursively		(HTreeItem* pItem, BOOL bCheck = TRUE);
	/*25.1.2009*/
	void		SetSlaveInfo				(iODBCRst* pRstSlave, AutoSortedArray* pArrSlaveRecords);
	/*26.1.2009*/
	void		SetSlaveCheckField			(CString sCheckField);
	BOOL		IsSlaveItem					(HTreeItem* pItem);

public:	// public methods.				################################################################
	BOOL		LoadFromRst					(iODBCRst* pRst, const CString sItemTextField, 
											const CString sUidField, const CString sParentField = _T(""));
	BOOL		LoadMasterSlaveFromRst		(iODBCRst* pRstMaster, iODBCRst* pRstSlave, const CString sItemTextField, 
											const CString sUidField, const CString sParentField = _T(""), int nImageMaster = 0, CString sImageSlave = _T(""));
protected:	// protected methods.		################################################################
	// Free record data attached to each tree item.
	void				FreeTreeRecData		();
	void				FreeRecDataArrayInt64(AutoSortedArrayInt64* pArrRecData);
	void*				GetRecordData		(HTREEITEM hItem);
	static HTreeItem*	GetPrevItem			(HTreeItem* pItem);
	static void			BuildTreeArray		(CTreeArray* pArrTreeItems, AutoSortedArray* pArrParentIdToUid, AutoSortedArray* pArrIdToRecordData, int nStartIndex, TreeItem* pParent = NULL);
	void				BuildTree			(TreeItem* pCurrItem, CTreeCtrl* pTreeCtrl, CRecord* pRec, int nItemTextField, HTREEITEM hParent = NULL);
	void				BuildMasterSlaveTree(long lParentId, CTreeCtrl* pTreeCtrl, HTreeItem* hParent = NULL, long lStartIndex = 0);
	static void			WalkThruTree		(CDWordArray& arrRecData, HTreeItem* pItem, BOOL bDetachDataFromItem);
	static void			CreateDragImage		(CTreeCtrl* pTree, CString sItemText, HTREEITEM hItemDrag, CImageList* &pImgList);
	static BOOL			FindItemById		(HTreeItem* pHItem, CRecord* pRec, int nIdField, int nId, HTREEITEM& hItemFound);
	/*26.1.2009*/
	virtual	void		OnItemCheck			(HTreeItem* pHItem, BOOL bCheck){};
	/*2.2.2009*/
	virtual	void		OnPreCreateFont		(LOGFONT* lf){};
	/*23.2.2009*/
	virtual void		OnLoadFinished		(){};
	virtual void		OnLoadStarted		(){};
public:
	HTREEITEM	InsertTreeItem	(long lNewUid, HTREEITEM hItemParent, CString sItemText, void* lpNewRecData);
	BOOL		DeleteItem		(HTREEITEM hItem);
	BOOL		CanMoveItem		(HTREEITEM hItemMove, HTREEITEM hItemMoveTo);
	BOOL		MoveItem		(HTREEITEM hItemMove, HTREEITEM hItemMoveTo, int nInsertAfter = -1);
	HTREEITEM	FindItemById	(int nId, BOOL bSelect = FALSE);
	HTREEITEM	FindItemById2	(int nId, int nParentId, BOOL bSelect = FALSE);
	void		AllowDragAndDrop(BOOL bAllow = FALSE);
	BOOL		FindKeyMoveTo	(LPCTSTR key);
	BOOL		Attach			(HWND hWndAttachTo);


protected:	// protected virtual methods.###############################################################
	virtual void	OnInitialUpdate		();
	virtual void	OnItemChanged		(LPNMTREEVIEW pNMTreeView){};
	virtual void	OnSaveTreeItemText	(BOOL& bSuccess, CString& sItemText){};
	virtual void	OnMoveItem			(BOOL& bCancel, CRecord* pRecord, void* lpRecDataMove, void* lpRecDataMoveTo){};
	virtual void	OnPreDeleteItem		(HTREEITEM hItemDelete){};

protected:	// protected attributes.	################################################################
	CString					m_sUIDField;			// UID field name.
	CString					m_sItemTextField;		// Tree item text field name.
	CString					m_sParentField;			// Parent field name.

	int						m_nUIDField;			// UID field index.
	int						m_nItemTextField;		// Tree item text field index.
	int						m_nParentField;			// Parent field index.
	int						m_nImageMaster;			// Image index of master item.

	int						m_nUIDFieldSlave;		// UID field index of slave table.
	int						m_nItemTextFieldSlave;	// Tree item text field index of slave table.
	int						m_nParentFieldSlave;	// Parent field index of slave table.
	int						m_nImageSlave;			// Image index of slave item.

	CTreeArray				m_arrTreeItems;			// Array like tree hierarchy.
	AutoSortedArray			m_arrReadOnlyUids;		// Array of read-only item Uid's;
	AutoSortedArray			m_arrIdToHtreeItem;		// record id to HTREEITEM. (Works in optimize search mode.)
	AutoSortedArray*		m_pArrSlaveIdToRecData;	// Slave recordset id to record data.

	AutoSortedArrayInt64*	m_pArrRecUIDToHTreeItem;// Unique ID of record(int64 32bit[parent id] | 1bit[if is slave record] | 31bit[record id]  ) to 32bit[HTREEITEM] | 32bit[record data pointer].
	BOOL					m_bDragIsAllowed;		// Drag operation allowation.

	HTREEITEM				m_hItemDrag;			// Currently draged tree item.
	HTREEITEM				m_hItemDrop;			// Drop to item.
	CRecord*				m_pRecord;				// record pointer of recordset.
	CRecord*				m_pRecordSlave;			// record pointer of recordset.
	BOOL					m_bOptimizeSearch;		// Optimize search mode.	

	HCURSOR					m_hCursorDrag;			// drag cursor.
	HCURSOR					m_hCursorDropTo;		// drop to cursor.

	CFont*					m_pFont;				// Treeview's font.
	UINT					m_nStyle;				// Treeview's style.
	CString					m_sInvalidChars;		// Invalid characters.
	
	CString					m_sCheckField;			// Slave check field name.
	int						m_nCheckField;			// Slave check field index.

public:// public attributes.	################################################################
	BOOL					m_bSelMasterItem;		// Indicates selected item owner(master or slave);

protected:
	afx_msg void OnDestroy			();
	afx_msg void OnTvnSelchanged	(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindrag		(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit	(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove		(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp		(UINT nFlags, CPoint point);
	afx_msg int	 OnMouseActivate	(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()	
};

__inline CRecord*	
TreeViewEx::GetRecordPointer()
	{
	return m_pRecord;
	}

__inline CString&	
TreeViewEx::GetInvalidChars()
	{
	return m_sInvalidChars;
	}

__inline void
TreeViewEx::SetReadOnlyItem(long nUidItem)
	{
	m_arrReadOnlyUids.Add(nUidItem);
	}

__inline BOOL
TreeViewEx::IsItemReadOnly(long nUidItem)
	{
	return (m_arrReadOnlyUids.FindValue(nUidItem) == -1) ? FALSE : TRUE;
	}

__inline void
TreeViewEx::SetDragAndDropCursors(HCURSOR hCurDrag, HCURSOR hCurDropTo)
	{
	m_hCursorDrag	= hCurDrag; 
	m_hCursorDropTo	= hCurDropTo;
	}

__inline void*
TreeViewEx::GetRecordData(HTREEITEM hItem)
	{
	if( !hItem ) return NULL;
	TreeItem* pItem = (TreeItem*)((HTreeItem*)hItem)->param;
	if( !pItem ) return NULL;
	return ((void*)pItem->nData);
	}

__inline BOOL
TreeViewEx::OptimizeIdSearch(BOOL bOptimize)
	{
	BOOL bRet			= m_bOptimizeSearch;
	m_bOptimizeSearch	= bOptimize;
	return bRet;
	}

__inline BOOL
TreeViewEx::FindKeyMoveTo(LPCTSTR key)
	{
	int nId = atoi(key);
	return (FindItemById(nId, TRUE) != NULL);
	}

__inline void
TreeViewEx::SetItemCheck(HTreeItem* pItem, BOOL bCheck /*= TRUE*/)
	{
	ASSERT( pItem );
	if( bCheck ){
		pItem->wWord1 &= ~0x1000;
		pItem->wWord1 |= 0x2000;
		}
	else{
		pItem->wWord1 &= ~0x2000;
		pItem->wWord1 |= 0x1000;
		}
	OnItemCheck(pItem, bCheck);
	}

__inline BOOL
TreeViewEx::GetItemCheck(HTreeItem* pItem)
	{
	ASSERT( pItem );
	return (pItem->wWord1&0x2000);
	}

__inline void
TreeViewEx::SetSlaveInfo(iODBCRst* pRstSlave, AutoSortedArray* pArrSlaveRecords)
	{
	m_pRecordSlave			= pRstSlave->GetRecordPtr();
	m_pArrSlaveIdToRecData	= pArrSlaveRecords;
	}

__inline void
TreeViewEx::SetSlaveCheckField(CString sCheckField)
	{
	m_sCheckField	= sCheckField;
	m_nCheckField	= -1;
	}

__inline BOOL
TreeViewEx::IsSlaveItem(HTreeItem* pItem)
	{
	void*		lpRecData	=  (void*)pItem->param;
	CRecord*	pRec		=  CRecord::FromRecordDataPtr(lpRecData);
	ASSERT( pRec );
	pRec->SetRecordData(lpRecData);

	if( m_pRecord == pRec )			// Master record
		return FALSE;
	else
	if( m_pRecordSlave == pRec )	// Slave record.
		return TRUE;
	else
		ASSERT( FALSE );
	return FALSE;
	}

