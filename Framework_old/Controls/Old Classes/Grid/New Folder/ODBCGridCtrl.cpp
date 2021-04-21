
#include "StdAfx.h"
#include "ODBCGridCtrl.h"
#include "UsefulFunctions.h"
#include <stddef.h>

#include "..\iODBCRst.h"
#include "..\AutoSortedArray.h"


//
//	ODBCGridCtrl::ODBCGridCtrl
//
ODBCGridCtrl::ODBCGridCtrl() : CGridCtrl()
	{
	m_pCols				= NULL;
	m_pColByIndex		= NULL;
	m_pRstSelect		= NULL;
	m_pRecord			= NULL;
	m_nNumberCols		= 0;
	m_pArrRecDataById	= new AutoSortedArray();
	m_pFont				= NULL;

	m_bDetach			= FALSE;
	m_pWndAttach		= NULL;

	}

BEGIN_MESSAGE_MAP(ODBCGridCtrl, CGridCtrl)
	ON_COMMAND			(-1,				OnDelete		)
	ON_COMMAND			(-2,				OnAddNew		)
	ON_UPDATE_COMMAND_UI(-1,				OnUpdateDelete	)
	ON_UPDATE_COMMAND_UI(-2,				OnUpdateAddNew	)
	ON_NOTIFY_REFLECT	(LVN_ITEMCHANGED,	OnItemChanged	)
END_MESSAGE_MAP()

//
//	ODBCGridCtrl::OnDestroy
//
void 
ODBCGridCtrl::OnDestroy()
	{
	// Delete recordset object. #
	if( m_pRstSelect )
		{
		if( m_pRstSelect->IsOpen() )
			m_pRstSelect->Close ();
		}
	// ##########################

	// Free col by index array.##
	if( m_pColByIndex )
		delete m_pColByIndex;
	// ##########################

	// Free cols array. #########
	FreePointersChain(m_pCols, (long)offsetof(ODBCGridCol, pNextCol));
	// ##########################

	// Free record data array. ###################
	FreeRecordDataArray();
	delete m_pArrRecDataById;
	// ###########################################

	// Delete font object.#######
	if( m_pFont )
		delete m_pFont;
	// ##########################

	if( m_bDetach )
		{
		m_pWndAttach->m_hWnd = NULL;
		m_pWndAttach->Attach(Detach());
		}

	CGridCtrl::OnDestroy();
	}

//
//	[public] ODBCGridCtrl::DetachOnDestroy
//
void			
ODBCGridCtrl::DetachOnDestroy(BOOL bDetach, CWnd* pWndAttach)
	{
	m_bDetach		= bDetach;
	m_pWndAttach	= pWndAttach;
	}

//
//	[public] ODBCGridCtrl::IsCmdHandled
//
BOOL
ODBCGridCtrl::IsCmdHandled(UINT nCode, UINT nCmdId)
	{
	const AFX_MSGMAP*	pMsgMap = GetMessageMap();
	int					iLoop	= 0;
	
	while( pMsgMap->lpEntries[iLoop].nID )
		{
		
		if( pMsgMap->lpEntries[iLoop].nCode == nCode && 
			pMsgMap->lpEntries[iLoop].nID	== nCmdId )
			{
			return TRUE;
			}
		
		iLoop ++;
		}
	
	return FALSE;
	}

//
//	[public] ODBCGridCtrl::DefineColumn
//
void
ODBCGridCtrl::DefineColumn(int nIndex, LPCTSTR lpszTitle, LPCTSTR lpszDBField, int nAlignment, int nWidth)
	{
	int nNewCol = InsertColumn(nIndex, lpszTitle, nAlignment, nWidth);
	ASSERT( nWidth > 0 );

	ODBCGridCol* pNewCol	= new ODBCGridCol;
	pNewCol->sFieldName		= lpszDBField;
	pNewCol->pNextCol		= NULL;
	pNewCol->nFieldID		= -1;
	pNewCol->pGetAsTextFunc	= NULL;
	pNewCol->bRowIndex		= pNewCol->sFieldName == "$ROW_INDEX$" ? TRUE : FALSE;

	// Determine alignment. ######################
	pNewCol->nAlign			= DT_VCENTER;
	if( nAlignment == LVCFMT_LEFT )
		pNewCol->nAlign		|= DT_LEFT;
	else
	if( nAlignment == LVCFMT_RIGHT )
		pNewCol->nAlign		|= DT_RIGHT;
	else
	  pNewCol->nAlign		|= DT_CENTER;
	// ##########################################
	
	m_pCols					= (ODBCGridCol*)InsertToPointersChain((void*)m_pCols, (void*)pNewCol, nIndex, (long)offsetof(ODBCGridCol, pNextCol), -1);
	m_nNumberCols			++;
	}

//
//	[public] ODBCGridCtrl::SetRowId
//
void
ODBCGridCtrl::SetRowId(const CString sUIDField)
	{
	m_sUIDField		= sUIDField;
	}

//
//	[public] ODBCGridCtrl::SetRst
//
iODBCRst*			
ODBCGridCtrl::SetRst(iODBCRst* pRst)
	{
	ASSERT( pRst );
	iODBCRst* pRstOld	= m_pRstSelect;
	m_pRstSelect		= pRst;
	m_pRecord			= pRst->GetRecordPtr();
	return pRstOld;
	}

//
//	[public] ODBCGridCtrl::InitCommands
//
void
ODBCGridCtrl::InitCommands(UINT nCmdDelete, UINT nCmdAddNew)
	{
	const AFX_MSGMAP*	pMsgMap = GetMessageMap();
	int					iLoop	= 0;
	// Replace identifiers in message map. ################
	while( pMsgMap->lpEntries[iLoop].nID )
		{
		UINT* pID = (UINT*)&pMsgMap->lpEntries[iLoop].nID;
		if( *pID == (WORD)-1 )
			{
			MEMORY_BASIC_INFORMATION mbi_thunk;
			VirtualQuery(pID, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
			VERIFY(VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect));

			*pID = nCmdDelete;

			// Change the protection back to what it was before I blasted.
			DWORD dwOldProtect;
			VERIFY(VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect));
			}
		else
		if( *pID == (WORD)-2 )
			{
			MEMORY_BASIC_INFORMATION mbi_thunk;
			VirtualQuery(pID, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
			VERIFY(VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect));

			*pID = nCmdAddNew;

			// Change the protection back to what it was before I blasted.
			DWORD dwOldProtect;
			VERIFY(VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect));
			}
		iLoop ++;
		}
	// ####################################################
	}

//
//	[virtual] ODBCGridCtrl::OnSetups
//
void	
ODBCGridCtrl::OnSetup()
	{
	/*
	SELECT 
	INC.UID, 
	INC.ORDER_N, 
	INC.CLIENT_UID, 
	(SELECT NAME FROM AXSYS_CONTACT WHERE UID = INC.CLIENT_UID ) AS CLIENT_NAME,
	INC.OUTPUT_DATE, 
	INC.PAYMENT, 
	INC.REAL_PAYMENT,  
	INC.NATIVE_PAYMENT,  
	INC.CLOSED 
	FROM AXSYS_OUTPUT  INC
	ORDER BY INC.UID;
	*/

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, NULL);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&ncm.lfCaptionFont);
	SetFont(m_pFont);

//	SubclassHeader();
//	GetFlatHeaderCtrl()->ShowSortArrow(FALSE);

	CHeaderCtrl* pHdr = GetHeaderCtrl();
	if( pHdr )
		pHdr->SetFont(m_pFont);
	SetCellSelectionMode(FALSE);
	}

//
//	[public] ODBCGridCtrl::Load
//
BOOL 
ODBCGridCtrl::Load()
	{
	ASSERT( m_pRstSelect && m_pRstSelect->IsOpen() );
	AfxGetApp()->BeginWaitCursor();
	SetRedraw(FALSE);

	// Open recordset and load record data. ##########
	try
		{
		// Free record data array. ###################
		FreeRecordDataArray();
		// ###########################################

		m_pRecord	= m_pRstSelect->GetRecordPtr();
		ASSERT( m_pRecord );
		m_nUIDField = m_pRecord->GetFieldID(m_sUIDField);
		ASSERT( m_nUIDField > -1 );

		m_pArrRecDataById->OptimizeForLargeData	(TRUE);
		BuildODBCColumnsArray					();
		m_pRecord->SetFreeDataOnDestroy			(FALSE);

		void*	lpRecData		= m_pRecord->GetRecordData();
		int		nRecDataSize	= m_pRecord->GetRecordSize();
		m_pRecord->SetRecordData(lpRecData);

		while( !m_pRstSelect->IsEOF() )
			{
			InsertItem(0, _T(""));
			long lUID			= m_pRecord->GetLong(m_nUIDField);
			
			// Allocate record data. #############################
			void* lpNewRecData	= new BYTE[nRecDataSize];
			memcpy(lpNewRecData, lpRecData, nRecDataSize);
			// ###################################################

			m_pArrRecDataById->Add	(lUID, (long)lpNewRecData);
			m_pRstSelect->MoveNext	();
			}

		delete lpRecData;
		m_pRstSelect->Close();
		}
	catch(CDBException* e)
		{
		SetRedraw(TRUE);
		m_strLastError = e->m_strError;
		e->Delete();
		AfxGetApp()->EndWaitCursor();
		return FALSE;
		}
	// ###############################################

	if( GetItemCount() > -1 )
		SelectCell(0, 0, true);

	SetRedraw(TRUE);
	AfxGetApp()->EndWaitCursor();
	return TRUE;
	}

//
//	[public] ODBCGridCtrl::ReLoad
//

BOOL
ODBCGridCtrl::ReLoad(CStringArray* pArrFields /*= NULL*/)
	{
	ASSERT( m_pRstSelect && m_pRstSelect->IsOpen() );
	AfxGetApp()->BeginWaitCursor();
	SetRedraw(FALSE);

	m_pRecord	= m_pRstSelect->GetRecordPtr();
	// Open recordset and reload record data. ##########
	try
		{
		ASSERT( m_pRecord );
		m_nUIDField				= m_pRecord->GetFieldID(m_sUIDField);
		ASSERT( m_nUIDField > -1 );

		AutoSortedArray*	pArrIds			= new AutoSortedArray();
		int					iLoop			= 0;
		void*				lpRecData		= m_pRecord->GetRecordData();
		int					nRecDataSize	= m_pRecord->GetRecordSize();
		int					nOldRecCount	= m_pArrRecDataById->GetCount();

		m_pRecord	->SetRecordData			(lpRecData);
		pArrIds		->OptimizeForLargeData	(TRUE);
		m_pRecord	->SetFreeDataOnDestroy	(FALSE);

		while( !m_pRstSelect->IsEOF() )
			{
			long			lUID			= m_pRecord->GetLong(m_nUIDField);
			void*			lpNewRecData	= NULL;
			// Retrieve record data. #########################
			if( iLoop >= nOldRecCount )
				{
				InsertItem(0, "");
				lpNewRecData = new BYTE[nRecDataSize];
				}
			else
				lpNewRecData = (void*)m_pArrRecDataById->GetData(iLoop);
			// ###############################################
			
			// Copy record data. #############################
			memcpy(lpNewRecData, lpRecData, nRecDataSize);
			// ###############################################
			pArrIds->Add			(lUID, (long)lpNewRecData);
			m_pRstSelect->MoveNext	(); iLoop ++;
			}

		// Delete not used record data buffers. ##############
		while( iLoop < nOldRecCount )
			{
			void* lpRecData = (void*)m_pArrRecDataById->GetData(iLoop);
			if( lpRecData )
				delete lpRecData;
			DeleteItem(0);
			iLoop ++;
			}
		delete m_pArrRecDataById;
		m_pArrRecDataById = pArrIds;
		// ###################################################

		delete lpRecData;
		m_pRstSelect->Close();
		}
	catch(CDBException* e)
		{
		SetRedraw(TRUE);
		m_strLastError = e->m_strError;
		e->Delete();
		AfxGetApp()->EndWaitCursor();
		return FALSE;
		}
	// ###############################################

	SetRedraw(TRUE);
	AfxGetApp()->EndWaitCursor();
	return TRUE;
	}

//
//	[virtual] ODBCGridCtrl::OnChooseGridFont
//
bool
ODBCGridCtrl::OnChooseGridFont(OUT LOGFONT* plf)
	{
	ZeroMemory(plf, sizeof(LOGFONT));
	plf->lfCharSet			= ANSI_CHARSET;
	plf->lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	plf->lfHeight			= 16;
	plf->lfOrientation		= 1500;
	plf->lfOutPrecision		= CLIP_DEFAULT_PRECIS;
	plf->lfPitchAndFamily	= VARIABLE_PITCH | FF_ROMAN;
	plf->lfQuality			= PROOF_QUALITY;
	plf->lfWeight			= FW_NORMAL;
	strcpy(plf->lfFaceName, "GeoMegasoft");
	return true;
	}

//
//	[virtual] ODBCGridCtrl::GetCellText
//
bool
ODBCGridCtrl::OnGetCellText(IN int Row, IN int Col, 
							CString& strCellText, IN LONG lRowParam)
	{
	ODBCGridCol*	pCol		= m_pColByIndex[Col];
	void*			lpRecData	= (void*)m_pArrRecDataById->GetData(Row);

	if( lpRecData )
		{
		m_pRecord->SetRecordData(lpRecData);
		if( pCol->pGetAsTextFunc )
			{
			GetFieldAsText pFunc = (GetFieldAsText)pCol->pGetAsTextFunc;
			(*pFunc)(pCol, strCellText, m_pRecord);
			}
		else
		if( pCol->bRowIndex )
			{
			strCellText.Format("%d", Row+1);
			}
		return true;
		}

	return false;
	}

//
//	[virtual] ODBCGridCtrl::OnGetCellTextAlign
//
bool
ODBCGridCtrl::OnGetCellTextAlign(IN int Row, IN int Col, OUT int& nAlign)
	{
	nAlign = DT_SINGLELINE|m_pColByIndex[Col]->nAlign;
	return true;
	}

//
//	ODBCGridCtrl::OnGetCellColors
//
bool 
ODBCGridCtrl::OnGetCellColors(	IN  int Row,
								IN  int Col,
								OUT COLORREF& TextColor,
								OUT COLORREF& BkColor,
								OUT UINT8& TextStyle) const
	{
	// NOTE: this routine is called VERY often by the painting routines. keep it
	//       as small/fast as possible!
	// NOTE: if you are comfortable with the default colors of the current cell,
	//       you should return FALSE, instead of returning TRUE and leaving the
	//       colors unchanged. this will increase drawing speed...

	if( mDraw.Type == CELL_SEPARATOR )
		return false;

	// change the background of even rows
	if( Row % 2 == 0 && !mDraw.Selected )
		{
		AdjustBrightness(BkColor, -15);
		AdjustBrightness(TextColor, 60);
		return true;
		}
	return (TextStyle != 0);
	}

//
//	ODBCGridCtrl::AdjustBrightness
//
void 
ODBCGridCtrl::AdjustBrightness(	IN OUT COLORREF&	Color,
								IN     INT16		Amount) const
	{
	INT32 comp = GetRValue(Color) + Amount;
	if (comp > 255) comp = 255;
	else if (comp < 0) comp = 0;
	Color = (Color & 0xffffff00) | comp;

	comp = GetGValue(Color) + Amount;
	if (comp > 255) comp = 255;
	else if (comp < 0) comp = 0;
	Color = (Color & 0xffff00ff) | (comp << 8);

	comp = GetBValue(Color) + Amount;
	if (comp > 255) comp = 255;
	else if (comp < 0) comp = 0;
	Color = (Color & 0xff00ffff) | (comp << 16);
	}

//
//	[protected] ODBCGridCtrl::BuildODBCColumnsArray
//
void
ODBCGridCtrl::BuildODBCColumnsArray()
	{
	ASSERT( m_nNumberCols );
	if( m_pColByIndex )
		delete m_pColByIndex;

	m_pColByIndex				= new ODBCGridCol*[m_nNumberCols];
	int				iLoop		= 0;
	ODBCGridCol*	pCol		= m_pCols;
	RecordField*	pFields		= m_pRecord->GetFieldInfosArray();

	while( iLoop < m_nNumberCols )
		{
		m_pColByIndex[iLoop]	= pCol;
		pCol->nFieldID			= m_pRecord->GetFieldID(pCol->sFieldName);
		pCol->pGetAsTextFunc	= NULL;
		pCol->nPrecision		= 0;
		pCol->nScale			= 0;

		if( pCol->nFieldID < 0 || pCol->bRowIndex )
			{
			iLoop ++; pCol = pCol->pNextCol;
			continue;
			}

		UINT nFieldDbvt			= pFields[pCol->nFieldID].dwfieldDbvtType;
		switch( nFieldDbvt )
			{
			case DBVT_BOOL:
				{
				pCol->pGetAsTextFunc = ((PROC)GetBool_FieldAsText);
				break;
				};
			case DBVT_STRING:
			case DBVT_UCHAR:
				{
				CODBCFieldInfo fieldInfo;
				m_pRstSelect->GetODBCFieldInfo(pCol->nFieldID, fieldInfo);
				if( fieldInfo.m_nSQLType == SQL_NUMERIC || 
					fieldInfo.m_nSQLType == SQL_DECIMAL  )
					{
					pCol->nPrecision	 = fieldInfo.m_nPrecision;
					pCol->nScale		 = fieldInfo.m_nScale;
					pCol->pGetAsTextFunc = ((PROC)GetDecimal_FieldAsText);
					}
				else
					pCol->pGetAsTextFunc = ((PROC)GetString_FieldAsText);
				break;
				};
			case DBVT_SHORT:
			case DBVT_LONG:
				{
				pCol->pGetAsTextFunc = ((PROC)GetInt_FieldAsText);
				break;
				};
			case DBVT_DATE:
				{
				/*	case SQL_DATE:
					case SQL_TIME:
					case SQL_TIMESTAMP: */
			//	CODBCFieldInfo fieldInfo;
			//	m_pRstSelect->GetODBCFieldInfo(pCol->nFieldID, fieldInfo);

				pCol->pGetAsTextFunc = ((PROC)GetDate_FieldAsText);
				break;
				};
			case DBVT_SINGLE:
			case DBVT_DOUBLE:
				{
				CODBCFieldInfo fieldInfo;
				m_pRstSelect->GetODBCFieldInfo(pCol->nFieldID, fieldInfo);
				pCol->pGetAsTextFunc = ((PROC)GetDouble_FieldAsText);
				break;
				};
			};
	
		iLoop ++; pCol = pCol->pNextCol;
		}
	}

//
//	[protected] ODBCGridCtrl::FreeRecordDataArray
//
void
ODBCGridCtrl::FreeRecordDataArray()
	{
	int nCount = m_pArrRecDataById->GetCount();
	while( nCount > 0 )
		{
		nCount --;
		void* lpRecData = (void*)m_pArrRecDataById->GetData(nCount);
		if( lpRecData )
			delete lpRecData;
		}
	m_pArrRecDataById->DeleteAll();
	m_pArrRecDataById->FreeExtra();
	}

//
//	[static protected] ODBCGridCtrl::GetBool_FieldAsText
//
void
ODBCGridCtrl::GetBool_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	if( pRec->IsFieldNull(pDBCol->nFieldID) )
		{
		sFieldText = _T("");
		}
	else
		{
		const CString sValues[] = {"yes", "no"};
		bool bValue				= pRec->GetBool(pDBCol->nFieldID);
		sFieldText				= sValues[bValue];
		}
	}

//
//	[static protected] ODBCGridCtrl::GetInt_FieldAsText
//
void		
ODBCGridCtrl::GetInt_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	if( pRec->IsFieldNull(pDBCol->nFieldID) )
		{
		sFieldText = _T("0");
		}
	else
		{
		int nValue = pRec->GetLong(pDBCol->nFieldID);
		sFieldText.Format("%d", nValue);
		}
	}

//
//	[static protected] ODBCGridCtrl::GetDouble_FieldAsText
//
void
ODBCGridCtrl::GetDouble_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	if( pRec->IsFieldNull(pDBCol->nFieldID) )
		{
		sFieldText = _T("0");
		}
	else
		{
		double dValue = pRec->GetDouble(pDBCol->nFieldID);
		if( pDBCol->nDecimalPlaces > 0 )
			sFieldText.Format("%d", (int)dValue);
		else
			{
			char szFormat[] = "%.2f";
			szFormat[2]		= (char)(pDBCol->nDecimalPlaces + (BYTE)'0');
			sFieldText.Format(szFormat, dValue);
			}
		}
	}

//
//	[static protected] ODBCGridCtrl::GetDateTime_FieldAsText
//
void
ODBCGridCtrl::GetDateTime_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	static COleDateTime dtTime;
	if( pRec->IsFieldNull(pDBCol->nFieldID) )
		sFieldText = _T("");
	else
		{
		pRec->GetDate(pDBCol->nFieldID, dtTime);
		sFieldText.Format("%02d/%02d/%04d %02d:%02d", dtTime.GetDay(), dtTime.GetMonth(), 
						  dtTime.GetYear(), dtTime.GetHour(), dtTime.GetMinute());
		}
	}

//
//	[static protected] ODBCGridCtrl::GetDate_FieldAsText
//
void		
ODBCGridCtrl::GetDate_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	static COleDateTime dtTime;
	if( pRec->IsFieldNull(pDBCol->nFieldID) )
		sFieldText = _T("");
	else
		{
		pRec->GetDate(pDBCol->nFieldID, dtTime);
		sFieldText.Format("%02d/%02d/%04d", dtTime.GetDay(), dtTime.GetMonth(), dtTime.GetYear());
		}
	}

//
//	[static protected] ODBCGridCtrl::GetString_FieldAsText
//
void
ODBCGridCtrl::GetString_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	pRec->GetString(pDBCol->nFieldID, sFieldText);
	}

//
//	[static protected] ODBCGridCtrl::GetDecimal_FieldAsText
//
void		
ODBCGridCtrl::GetDecimal_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	pRec->GetString(pDBCol->nFieldID, sFieldText);
	if( sFieldText[0] == '.' )
		sFieldText = "0" + sFieldText;
	}

//
//	[protected virtual] ODBCGridCtrl::OnDeleteRecord
//
BOOL	
ODBCGridCtrl::OnDeleteRecord(CRecord* pRecDelete)
	{
	return TRUE;
	}

//
//	[protected virtual] ODBCGridCtrl::OnAddNewRecord
//
BOOL
ODBCGridCtrl::OnAddNewRecord(CRecord* pNewRec)
	{
	return TRUE;
	}

//
//	[protected handler] ODBCGridCtrl::OnDelete
//
void 
ODBCGridCtrl::OnDelete()
	{
	}

//
//	[protected handler] ODBCGridCtrl::OnAddNew
//
void 
ODBCGridCtrl::OnAddNew()
	{
	}

//
//	[protected handler] ODBCGridCtrl::OnUpdateDelete
//
void 
ODBCGridCtrl::OnUpdateDelete(CCmdUI* pCmdUI)
	{
	POSITION	pos		= GetFirstSelectedItemPosition();
	int			nIndex  = pos ? GetNextSelectedItem(pos) : -1;

	pCmdUI->Enable( (nIndex > -1) ? TRUE : FALSE );
	}

//
//	[protected handler] ODBCGridCtrl::OnUpdateAddNew
//
void 
ODBCGridCtrl::OnUpdateAddNew(CCmdUI* pCmdUI)
	{
	pCmdUI->Enable(TRUE);
	}

//
//	[protected handler] ODBCGridCtrl::OnItemChanged
//
void
ODBCGridCtrl::OnItemChanged(LPNMHDR pNMHDR, LRESULT* pResult)
	{
	NM_LISTVIEW*	pNMListView	= (NM_LISTVIEW*)pNMHDR;
	*pResult					= 0;
	int				nCurrRow	= pNMListView->iItem;
	int				nRecCount	= m_pArrRecDataById->GetCount();
	ASSERT( nRecCount > nCurrRow );

	void*			lpRecData	= (void*)m_pArrRecDataById->GetData(nCurrRow);
	ASSERT( lpRecData );
	m_pRecord->SetRecordData(lpRecData);
	}

