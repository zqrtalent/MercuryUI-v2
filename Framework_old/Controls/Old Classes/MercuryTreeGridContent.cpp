// MercuryTreeGridContent.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryTreeGridContent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MercuryTreeGridContent

MercuryTreeGridContent::MercuryTreeGridContent()
	{
	m_pRootItem			= new MerTreeItem;
	m_pRootItem->parent = NULL;
	m_pRootItem->child	= NULL;
	m_pRootItem->next	= NULL;
	m_pRootItem->prev	= NULL;
	m_pCurrent			= NULL;
	}

MercuryTreeGridContent::~MercuryTreeGridContent()
	{
	Delete(m_pRootItem);
	m_pCurrent	= NULL;
	m_pRootItem	= NULL;
	}

BOOL
MercuryTreeGridContent::Add(MerTreeItem* pItem, MerTreeItem* parent /*= NULL*/)
	{
	if( !pItem ) return FALSE;
	MerTreeItem* prev = NULL;
	if( !parent ){
		parent = m_pRootItem;
		}

	if( parent->child ){
		prev = parent->child;
		while( prev->next )
			prev = prev->next;
		prev->next = pItem;
		}
	else
		parent->child = pItem;

	pItem->parent	= (parent == m_pRootItem) ? NULL : parent;
	pItem->prev		= prev;
	pItem->next		= NULL;

	if( !m_pCurrent )
		m_pCurrent = pItem;
	return TRUE;
	}

BOOL
MercuryTreeGridContent::Delete(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	MerTreeItem* parent = pItem->parent;
	Recur_Delete(pItem);
	if( parent )
		parent->child = NULL;
	return TRUE;
	}

void
MercuryTreeGridContent::Recur_Delete(MerTreeItem* pItem)
	{
	while( pItem ){
		if( pItem->child ){
			Recur_Delete(pItem->child);
			}
		MerTreeItem* pNext = pItem->next;
		delete pItem;
		pItem = pNext;
		}
	}