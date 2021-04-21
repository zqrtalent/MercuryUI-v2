#pragma once
#include "ODBCSearchGridView.h"

class CODBCRecordset;
class ODBCSearchGridViewEx: public ODBCSearchGridView
{
	// < Constructor/Destructor >
public:
	ODBCSearchGridViewEx();
	virtual ~ODBCSearchGridViewEx();

	DECLARE_DYNCREATE(ODBCSearchGridViewEx)

	// < Public methods >
public:

	// < Protected methods >
protected:
	virtual bool	OnGetCellText		(IN int Row, IN int Col, OUT CString& strCellText, IN LONG lRowParam);
	virtual void	OnSetItemCount		(int nItems){};

	// < Private methods >
protected:
	
	// < Public attributes >
public:
	CPtrArray	m_arrRecDatas; // Array of record datas used at first, when search is not active yet.
	
	
	// < Protected message handlers >
protected:

	DECLARE_MESSAGE_MAP()
};
