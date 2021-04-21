#pragma once

typedef struct MerTreeItemTag
	{
	MerTreeItemTag*	parent;
	MerTreeItemTag*	next;
	MerTreeItemTag*	prev;
	MerTreeItemTag*	child;
	CString			sText;
	LONG			param;
	int				iImage;
	int				iSelImage;
	DWORD			dwState;
	} MerTreeItem, *PMerTreeItem;

class MercuryTreeGridContent
{
// Construction
public:
	MercuryTreeGridContent();
	virtual ~MercuryTreeGridContent();

public:
	BOOL			LoadFromXML	(const CString sXMLFile);
	BOOL			SaveInXML	(const CString sXMLFile);

	BOOL			Add			(MerTreeItem* pItem, MerTreeItem* parent = NULL);
	BOOL			Delete		(MerTreeItem* pItem);
	MerTreeItem*	GetCurrItem	();
	MerTreeItem*	GetRootItem	();

protected:
	MerTreeItem*	m_pRootItem;
	MerTreeItem*	m_pCurrent;


protected:
	void			Recur_Delete(MerTreeItem* pItem);

};

__inline
MerTreeItem*
MercuryTreeGridContent::GetCurrItem()
	{
	return m_pCurrent;
	}

__inline
MerTreeItem*
MercuryTreeGridContent::GetRootItem()
	{
	return m_pRootItem->child;
	}



