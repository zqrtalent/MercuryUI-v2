#pragma once
#include "AutoSortedArray.h"
#include "AutoSortedArrayInt64.h"
#include "..\XML\Markup.h"


// CTreeArray
typedef struct TreeItem_tag
	{
	TreeItem_tag*	pParent;
	TreeItem_tag*	pChild;
	TreeItem_tag*	pNext;
	UINT			nId;
	UINT			nData;
	UINT			nData2;
	} TreeItem, *PTreeItem;

class CTreeArray
	{
public:		// Constructor / Destructor.
	CTreeArray	();
	~CTreeArray	();

public:		// Public methods.
	TreeItem*	AddTreeItem			(UINT nItemId, UINT nItemData, TreeItem* pBelongsTo = NULL, 
									int* pIndexAdd = NULL, TreeItem* pItemAdd = NULL);
	BOOL		RemoveItem			(TreeItem* pItem);
	BOOL		MoveItem			(TreeItem* pItemMove, TreeItem* pItemMoveTo, int* pIndexMove = NULL);
	BOOL		CanMoveItem			(TreeItem* pItemMove, TreeItem* pItemMoveTo);
	BOOL		SetItemIdSortFlag	(BOOL bSort = TRUE);
	TreeItem*	GetRootItem			();
	void		RemoveAll			();
	
	TreeItem*	FindItemById		(TreeItem* pFind, int nId);
	int			GetChildsCount		(TreeItem* pParent);

protected:	// Protected methods.
	void		FreeTreeData		(TreeItem* pTree);
	// Finds tree item index in sorted item's list.
	int			FindIndexById		(TreeItem* pBelongsTo, TreeItem* pItem, TreeItem*& pPrevItem, TreeItem*& pNextItem);
	
	virtual		void OnFreeTreeData	(TreeItem* pTree){};

protected:	// Protected attributes.
	TreeItem*	m_pRootItem;
	TreeItem*	m_pCurrItem;
	BOOL		m_bSortById;
	};

__inline void
CTreeArray::RemoveAll(){
	FreeTreeData(m_pRootItem->pChild);
	m_pRootItem->pChild = NULL;
	m_pCurrItem			= m_pRootItem;
	}

__inline TreeItem*	
CTreeArray::GetRootItem()
	{
	return m_pRootItem;
	}

// CXmlTreeArray

typedef void (__cdecl* PostLoadXml_proc)(CMarkup* pXml);

class CXmlTreeArray : public CTreeArray
	{

public:		// Constructor / Destructor.
	CXmlTreeArray	();
	~CXmlTreeArray	();

	BOOL		Init				(const CString sXmlFile);
	BOOL		LoadXml				(CStringArray* parrParents, CDWordArray* pArrIds, CStringArray** parrElements, int nIndex = 0);
	BOOL		GetElement			(TreeItem* pItem, const CString sElemName, CString& sElem);
	CString*	GetLastError		();

protected:
	virtual		void OnFreeTreeData	(TreeItem* pTree);
	BOOL		WalkThroughXml		(CStringArray* parrElements, int nIdIndex = -1);


public:
	PostLoadXml_proc	m_postLoadXmlProc;

protected:
	CMarkup				m_xml;
	CString				m_strError;
	};


__inline CString*
CXmlTreeArray::GetLastError()
	{
	return &m_strError;
	}

