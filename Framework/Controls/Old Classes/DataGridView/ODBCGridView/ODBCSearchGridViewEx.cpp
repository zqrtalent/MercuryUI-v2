#include "stdafx.h"
#include "..\AutoSortedArray.h"
#include "..\RMC.h"
#include "ODBCStringsMatrix.h"
#include "ODBCSearchGridViewEx.h"

// ODBCSearchGridView
IMPLEMENT_DYNCREATE(ODBCSearchGridViewEx, ODBCSearchGridView)

//
//	ODBCSearchGridView::ODBCSearchGridView
//
ODBCSearchGridViewEx::ODBCSearchGridViewEx() : ODBCSearchGridView()
	{
	}

ODBCSearchGridViewEx::~ODBCSearchGridViewEx()
	{
	}

BEGIN_MESSAGE_MAP(ODBCSearchGridViewEx, ODBCSearchGridView)
END_MESSAGE_MAP()

//
//	[virtual] ODBCSearchGridViewEx::GetCellText
//
bool
ODBCSearchGridViewEx::OnGetCellText(IN int Row, IN int Col, 
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
		void* lpRecData = NULL;
		
		// If search option is not used.
		if( m_matrix.GetStringsCount() == m_matrix.GetResultCount() )
			{
			lpRecData = (Row >= 0 && Row < m_arrRecDatas.GetCount()) ? m_arrRecDatas[Row] : NULL;
			if( !lpRecData )
				return false;
			}
		else// Get record data from result record datas.
			lpRecData = (void*)m_matrix.GetResultParam2(Row);

		if( lpRecData && m_pRecord)
			{
			void*		lpRec		= (void*)m_pRecord->GetRecordData();
			m_pRecord->SetRecordData(lpRecData);
			if( pCol->pGetAsTextFunc )
				{
				GetFieldAsText pFunc = (GetFieldAsText)pCol->pGetAsTextFunc;
				(*pFunc)(pCol, strCellText, m_pRecord);
				}
			m_pRecord->SetRecordData(lpRec);
			return true;
			}
		else
			{
			if( m_matrix.GetResultString2(Row, Col - 1, strCellText) )
				return true;
			}
		}
	return false;
	}