// ===========================================================================
// About: MFC new generation data management class library.
// Author:	Zaqro Butskhrikidze
// Version: 1.0
// Started: 12/21/2010
// ===========================================================================

#pragma once
#include "VRowSet.h"

class VirtualRowSetIndex
{
protected:
	VirtualRowSetIndex();
	~VirtualRowSetIndex();

public:
	static VirtualRowSetIndex* CreateIndex(VirtualRowSet* pRowSet, 
											VRowSetFieldInfo* pField, 
											BOOL bOrderByAsc);

protected:
	VRowSetFieldInfo*	m_pFieldInfo;	// Field info class.
	BOOL				m_bOrderAsc;	// Ascending or descanding order.


protected:
	virtual int		OrderBy				(BOOL bOrderByAsc){return 0;};
	virtual void	OnAddVirtualRow		(VirtualRowSet* pRowSet, VRowHeader* pRow){};
	virtual void	OnRemoveVirtualRow	(VirtualRowSet* pRowSet, VRowHeader* pRow){};
	virtual void	OnEditVirtualRow	(VirtualRowSet* pRowSet, VRowHeader* pRow){};
	virtual int		GetOrdinalIndex		(VirtualRowSet* pRowSet){return 0;};

};