#pragma once
#include "ODBCGridView.h"
#include "ODBCStringsMatrix.h"

class CODBCRecordset;
class ODBCSearchGridView: public ODBCGridView
{
	// < Constructor/Destructor >
public:
	ODBCSearchGridView();
	virtual ~ODBCSearchGridView();

	DECLARE_DYNCREATE(ODBCSearchGridView)

	// < Public methods >
public:
	int		InitializeSearch	(CODBCRecordset* pRstSelect, CString sUID, CStringArray* arrStringFields);
	int		InitializeSearch2	(CRecord* pRecord, AutoSortedArray* pArrIdToRecData,CString sUID, CStringArray* arrStringFields);
	int		OnSearch			(LPTSTR lpszSearch);

	// < Protected methods >
protected:
	virtual CellType	OnGetCellType	(IN int Row, IN int Col);
	virtual bool		OnGetCellText	(IN int Row, IN int Col, OUT CString& strCellText, IN LONG lRowParam);
	virtual void		OnSetItemCount	(int nItems){};
	virtual bool		OnGetCellColors	(IN int Row, IN int Col, OUT COLORREF& TextColor, OUT COLORREF& BkColor, OUT	COLORREF& BottomLine,OUT UINT8& TextStyle) const;
	virtual bool		OnGetCellBorders(IN  int Row, IN  int Col, OUT COLORREF& crBorder, OUT UINT& nPenStyle, OUT UINT& nBorderStyle, OUT int& nWidth) const;
	virtual bool		OnCellLBtnDown	(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point);
	virtual void		CheckAll		(int nCol, BOOL bCheck, BOOL& bRedraw);

	// < Private methods >
protected:
	ODBCStringsMatrix	m_matrix;
	
	// < Protected attributes >
protected:
	
	
	// < Protected message handlers >
protected:

	DECLARE_MESSAGE_MAP()
};
