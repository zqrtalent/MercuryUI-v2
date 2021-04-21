#include "stdafx.h"
#include "ODBCGridView.h"
#include "..\AutoSortedArray.h"
#include "..\RMC.h"
#include "ODBCStringsMatrix.h"
#include "ODBCSearchGridView.h"

// ODBCSearchGridView
IMPLEMENT_DYNCREATE(ODBCSearchGridView, ODBCGridView)

//
//	ODBCSearchGridView::ODBCSearchGridView
//
ODBCSearchGridView::ODBCSearchGridView() : ODBCGridView()
	{
	}

ODBCSearchGridView::~ODBCSearchGridView()
	{
	}

BEGIN_MESSAGE_MAP(ODBCSearchGridView, ODBCGridView)
END_MESSAGE_MAP()

//
//	[public] ODBCSearchGridView::InitializeSearch
//
int
ODBCSearchGridView::InitializeSearch(CODBCRecordset* pRstSelect, CString sUID, CStringArray* arrStringFields)
	{
	int*	pnIds	= new int[arrStringFields->GetCount()];
	int		nLoop	= 0;
	while( nLoop < arrStringFields->GetCount() )
		{
		pnIds[nLoop] = pRstSelect->GetFieldID(arrStringFields->GetAt(nLoop));
		nLoop		++;
		}
	
	BuildODBCColumnsArray();
	m_nUIDField = pRstSelect->GetFieldID(sUID);

	SetRedraw(FALSE);
	int nRet = m_matrix.LoadFromRst(pRstSelect, m_nUIDField, pnIds, arrStringFields->GetCount());
	SetRedraw(TRUE);
	
	CListCtrl&	listCtrl	= GetListCtrl();
	if( nRet > 0 ) // Fill list.
		{
		listCtrl.SetItemCountEx(nRet, LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);
		CListCtrl&	list = GetListCtrl();
		if( list.GetItemCount() > 0 )
			SelectItem(0);
		}
	else
		{
		listCtrl.SetItemCountEx(nRet, LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);
		}

	delete pnIds;
	return nRet;
	}

int	
ODBCSearchGridView::InitializeSearch2(CRecord* pRecord, AutoSortedArray* pArrIdToRecData, 
										CString sUID, CStringArray* arrStringFields)
	{
	if( !pRecord )
		pRecord			= m_pRstSelect->GetRecordPtr();
	if( !pArrIdToRecData )
		pArrIdToRecData = m_pArrRecDataById;

	int*	pnIds	= new int[arrStringFields->GetCount()];
	int		nLoop	= 0;
	while( nLoop < arrStringFields->GetCount() )
		{
		pnIds[nLoop] = pRecord->GetFieldID(arrStringFields->GetAt(nLoop));
		nLoop		++;
		}

	if( !m_pColByIndex )
		BuildODBCColumnsArray();
	if( m_nUIDField == -1 )
		m_nUIDField = pRecord->GetFieldID(sUID);

	SetRedraw(FALSE);
	int nRet = m_matrix.LoadFromRst2(pRecord, pArrIdToRecData, pRecord->GetFieldID(sUID), pnIds, arrStringFields->GetCount());
	SetRedraw(TRUE);
	
	CListCtrl&	listCtrl	= GetListCtrl();
	if( nRet > 0 ) // Fill list.
		{
		listCtrl.SetItemCountEx(nRet, LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);

		CListCtrl&	list = GetListCtrl();
		if( list.GetItemCount() > 0 )
			SelectItem(0);
		}
	else
		listCtrl.SetItemCountEx(nRet, LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);

	OnSetItemCount(nRet);

	delete pnIds;
	return nRet;
	}

//
//	[pubic] ODBCSearchGridView::OnSearch
//
int
ODBCSearchGridView::OnSearch(LPTSTR lpszSearch)
	{
	SetRedraw(FALSE);
	CListCtrl&	listCtrl	= GetListCtrl();
	int			nResult		= m_matrix.SearchStrings(lpszSearch);
	if( nResult > 0 ) // Fill list.
		{
		listCtrl.SetItemCountEx(nResult, LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);

		CListCtrl&	list = GetListCtrl();
		if( list.GetItemCount() > 0 )
			SelectItem(0);
		}
	else
		listCtrl.SetItemCountEx(nResult, LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);

	OnSetItemCount(nResult);

	SetRedraw	(TRUE);
	Invalidate	();
	return nResult;
	}

//
//	[virtual] ODBCSearchGridView::OnGetCellBorders
//
bool 
ODBCSearchGridView::OnGetCellBorders(	IN  int			Row,
										IN  int			Col,
										OUT COLORREF&	crBorder,
										OUT UINT&		nPenStyle,
										OUT UINT&		nBorderStyle,
										OUT int&		nWidth) const
	{
	nPenStyle		= PS_SOLID;
	nWidth			= 1;
	bool	bSel	= mDraw.Selected;

	ODBCStringsMatrix* pMatrix = (ODBCStringsMatrix*)&m_matrix;

	if( m_bUseFirstColAsSelState && 
		m_pColByIndex[0]->bRowIndex		== TRUE && 
		m_pColByIndex[0]->nCheckFieldId	> -1 )
		{
		void*	lpRecData	= (void*)pMatrix->GetResultParam2(Row);
		void*	lpDataOld	= m_pRecord->GetRecordData();
		m_pRecord->SetRecordData(lpRecData);
		BOOL	bCheck		= (BOOL)m_pRecord->GetLong(m_pColByIndex[0]->nCheckFieldId);
		if( bCheck != FALSE  )
			bSel	= true;
		m_pRecord->SetRecordData(lpDataOld);
		}

	if( !bSel )
		crBorder		= RGB(217, 217, 217);
	else
		{
//		if( GetFocus() == this )
			crBorder	= RGB(50, 109, 190);
//		else
//			crBorder	= RGB(172, 172, 172);

		if( !mDraw.Selected )
			crBorder	= RGB(30, 89, 170);
		}

	nBorderStyle	= BORDER_STYLE_RIGHT;
	return true;
	}

//
//	ODBCSearchGridView::OnGetCellColors
//
bool 
ODBCSearchGridView::OnGetCellColors(IN  int Row,
									IN  int Col,
									OUT COLORREF& TextColor,
									OUT COLORREF& BkColor,
									OUT	COLORREF& BottomLine,
									OUT UINT8& TextStyle) const
	{
	// NOTE: this routine is called VERY often by the painting routines. keep it
	//       as small/fast as possible!
	// NOTE: if you are comfortable with the default colors of the current cell,
	//       you should return FALSE, instead of returning TRUE and leaving the
	//       colors unchanged. this will increase drawing speed...

	if( mDraw.Type == CELL_SEPARATOR )
		return false;

	bool	bSel	= mDraw.Selected;
	BOOL	bCheck	= FALSE;
	
	ODBCStringsMatrix* pMatrix = (ODBCStringsMatrix*)&m_matrix;

	if( m_bUseFirstColAsSelState && 
		m_pColByIndex[0]->bRowIndex		== TRUE && 
		m_pColByIndex[0]->nCheckFieldId	> -1 )
		{
		void*	lpRecData	= (void*)pMatrix->GetResultParam2(Row);
		void*	lpDataOld	= m_pRecord->GetRecordData();

		m_pRecord->SetRecordData(lpRecData);
		bCheck		= (BOOL)m_pRecord->GetLong(m_pColByIndex[0]->nCheckFieldId);
		if( bCheck != FALSE  )
			bSel	= true;
		m_pRecord->SetRecordData(lpDataOld);
		}

	if( !bSel )
		{
		if( Row%2 == 0 )
			{
			BkColor		= RGB(241, 245, 250);
			TextColor	= RGB(0, 0, 0);
			BottomLine	= RGB(241, 245, 250);
			}
		else
			{
			BkColor		= RGB(255, 255, 255);
			TextColor	= RGB(0, 0, 0);
			BottomLine	= RGB(255, 255, 255);
			}
		return true;
		}
	else
		{
		if( mDraw.Selected )
			{
//			if( GetFocus() == this )
				{
				BkColor		= RGB(61, 128, 223);
				TextColor	= RGB(255, 255, 255);
				BottomLine	= RGB(125, 170, 234);
				}
//			else
//				{
//				BkColor		= RGB(192, 192, 192);
//				TextColor	= RGB(0, 0, 0);
//				BottomLine	= RGB(224, 224, 224);
//				}
			}
		else
			{
			BkColor		= RGB(41, 108, 203);
			TextColor	= RGB(192, 192, 192);
			BottomLine	= RGB(105, 150, 214);
//			TextStyle	= FONT_BOLD;
			}
		
		return true;
		}
	return (TextStyle != 0);
	}

//
//	[virtual] ODBCSearchGridView::CheckAll
//
void
ODBCSearchGridView::CheckAll(int nCol, BOOL bCheck, BOOL& bRedraw)
	{
	bRedraw = FALSE;
	if( nCol <= -1 )
		return;

	m_csection.Lock();
	if( m_pColByIndex[nCol]->bCheckBox &&  
		m_pColByIndex[nCol]->nCheckFieldId > -1 )
		{
		int		nChanged	= 0;
		void*	lpDataOld	= m_pRecord->GetRecordData();
		void*	lpRecData	= NULL;
		int		nCheckField	= m_pColByIndex[nCol]->nCheckFieldId;

		for( int nLoop=0; nLoop<m_matrix.GetResultCount(); nLoop ++ )
			{
			lpRecData		= (void*)m_matrix.GetResultParam2(nLoop);
			if( !lpRecData ) continue;
			m_pRecord->SetRecordData(lpRecData);

			BOOL bState		= m_pRecord->GetLong(nCheckField);
			if( bState != bCheck )
				nChanged	++;

			m_pRecord->SetLong(nCheckField, bCheck);
			
			}
		m_pRecord->SetRecordData(lpDataOld);
		if( nChanged > 0 )
			{
			m_pColByIndex[nCol]->nCheckHeader = bCheck;
			if( ::IsWindow(m_headerCtrl) && ::IsWindowVisible(m_headerCtrl) )
				m_headerCtrl.Invalidate();
			bRedraw = TRUE;
			}
		}
	m_csection.Unlock();
	}

//
//	[virtual] ODBCSearchGridView::OnCellLBtnDown
//
bool
ODBCSearchGridView::OnCellLBtnDown(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point)
	{
	if( m_pColByIndex[Col]->bCheckBox		&& 
		m_pColByIndex[Col]->nCheckFieldId > -1 ) // CheckBox column.
		{
		CPoint pt			= Point;
		CRect rcCheckBox	= CellRect;
		rcCheckBox.right	= rcCheckBox.left + CHECK_MARKWIDTH;

		if( rcCheckBox.PtInRect(pt) )
			{
			if( !m_pRecord )
				m_pRecord = m_pRstSelect->GetRecordPtr();

			ODBCStringsMatrix* pMatrix = (ODBCStringsMatrix*)&m_matrix;

			m_csection.Lock();
			int		nCheckFld	= m_pColByIndex[Col]->nCheckFieldId;
			void*	lpRecData	= (void*)pMatrix->GetResultParam2(Row);
			void*	lpDataOld	= m_pRecord->GetRecordData();
			m_pRecord->SetRecordData(lpRecData);
			m_csection.Unlock();

			if( lpRecData )
				{
				BOOL	bCheck	= m_pRecord->GetLong(nCheckFld);
				BOOL	bAllow	= TRUE;
				OnCheckItem(m_pRecord, Col, Row, bAllow, bCheck);
				
				if( bAllow )
					{
					bCheck		= !bCheck;
					m_pRecord->SetLong		(nCheckFld, bCheck);
					m_pRecord->SetRecordData(lpDataOld);					
					RedrawItem(Row);
					return true; // We don't need to select checked item !!! [ZqrTalent]
					}
				}
			m_pRecord->SetRecordData(lpDataOld);
			}
		}

	// Single selection only.
	if( m_bSingleSel )
		{
		if( !HIBYTE(GetKeyState(VK_CONTROL))	&&
			!HIBYTE(GetKeyState(VK_SHIFT)) )
			{
			return GridView::OnCellLBtnDown(Row, Col, CellRect, Point);
			}
		return true;
		}

	return GridView::OnCellLBtnDown(Row, Col, CellRect, Point);
	}

//
//	[virtual] ODBCSearchGridView::OnGetCellType
//
CellType 
ODBCSearchGridView::OnGetCellType(IN int Row, IN int Col)
	{
	if( m_pColByIndex[Col]->bCheckBox		&& 
		m_pColByIndex[Col]->nCheckFieldId > -1 )
		{
		if( !m_pRecord )
			m_pRecord = m_pRstSelect->GetRecordPtr();

		m_csection.Lock();
		void*	lpRecData	= (void*)m_matrix.GetResultParam2(Row);
		void*	lpDataOld	= m_pRecord->GetRecordData();
		m_pRecord->SetRecordData(lpRecData);
		BOOL bCheck = lpRecData ? m_pRecord->GetLong(m_pColByIndex[Col]->nCheckFieldId) : FALSE;
		m_pRecord->SetRecordData(lpDataOld);
		m_csection.Unlock();
		return bCheck ? CELL_CHECKBOXON : CELL_CHECKBOXOFF;
		}
	
	return CELL_REGULAR;
	}

//
//	[virtual] ODBCSearchGridView::GetCellText
//
bool
ODBCSearchGridView::OnGetCellText(IN int Row, IN int Col, 
							CString& strCellText, IN LONG lRowParam)
	{
	ODBCGridCol*	pCol		= m_pColByIndex[Col];
	if( pCol->bRowIndex )
		{
		strCellText.Format("%d", Row+1);
		return true;
		}
	else
		{
		if( m_matrix.GetResultString2(Row, Col - 1, strCellText) )
			return true;
		}
	return false;
	}