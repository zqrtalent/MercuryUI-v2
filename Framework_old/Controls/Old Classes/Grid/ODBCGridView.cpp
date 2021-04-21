/////////////////////////////////////////
//	class: ODBCGridView
//	programmer:	ZqrTalent
//	Date Start:	23/8/2007
/////////////////////////////////////////

#include "stdafx.h"
#include "ODBCGridView.h"
#include "..\..\UsefulFunctions.h"
#include <stddef.h>

/*
#include "..\iODBCRst.h"
#include "..\AutoSortedArray.h"
#include "..\RMC.h"

#include "..\AutoSortedArrayTempl.h"
#include "..\GlobalStorage\GlobalStorage.h"
#include "ODBCStringsMatrix.h"
*/

// CODBCHeaderCtrl

//
//	CODBCHeaderCtrl::CODBCHeaderCtrl
//
CODBCHeaderCtrl::CODBCHeaderCtrl()
	{
	m_pGridView = NULL;
	m_iSpacing	= 1;
	}

//
//	CODBCHeaderCtrl::CODBCHeaderCtrl
//
CODBCHeaderCtrl::~CODBCHeaderCtrl()
	{
	}

BEGIN_MESSAGE_MAP(CODBCHeaderCtrl, CXHeaderCtrl)
	ON_WM_RBUTTONDOWN	()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONDBLCLK	()
END_MESSAGE_MAP()

//
//	[afx_msg] CODBCHeaderCtrl::OnRButtonDown
//
void 
CODBCHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
	{
	CHeaderCtrl::OnRButtonDown(nFlags, point);
	}

//
//	[afx_msg] CODBCHeaderCtrl::OnLButtonDblClk
//
void 
CODBCHeaderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
	{/*
	HD_HITTESTINFO ht;
	ht.pt = point;
	::SendMessage(m_hWnd, HDM_HITTEST, 0L, (LPARAM)&ht);

	if( ht.flags&HHT_ONHEADER && ht.iItem > -1 && m_pGridView->m_pColByIndex ){
		CRect rcCheck;
		GetItemRect(ht.iItem, rcCheck);
		rcCheck.left ++;
		rcCheck.right = rcCheck.left + CHECK_MARKWIDTH;

		BOOL bCheck = FALSE;
		if( GetColCheckState(ht.iItem, bCheck) && rcCheck.PtInRect(point) )
			{
			// Check or Uncheck all. ####################
			bCheck	= m_pGridView->m_pColByIndex[ht.iItem]->nCheckHeader;
			bCheck	= !bCheck;
			m_pGridView->m_pColByIndex[ht.iItem]->nCheckHeader = bCheck;

			m_pGridView->m_nColCheck = ht.iItem;
			if( bCheck )
				m_pGridView->SendMessage(WM_COMMAND, MENU_ITEM_CHECK, 0L);
			else
				m_pGridView->SendMessage(WM_COMMAND, MENU_ITEM_UNCHECK, 0L);
			// #########################################
			CHeaderCtrl::OnLButtonDblClk(nFlags, point);
			return;
			}
		
		if( m_pGridView->m_pColByIndex[ht.iItem]->bAllowSort )
			{
			int		nSortFlagOld	= m_pGridView->m_nSortFlag;
			int		nSortColOld		= m_pGridView->m_nSortCol;
			BOOL	bAsc			= FALSE;

			int *pSortFlag = &m_pGridView->m_nSortFlag;
			if( m_pGridView->m_nSortCol == ht.iItem ){
				if( *pSortFlag == SORT_FLAG_NOT )
					*pSortFlag	= SORT_FLAG_ASC;
				else
				if( *pSortFlag == SORT_FLAG_ASC )
					*pSortFlag	= SORT_FLAG_DESC;
				else
					{
					*pSortFlag	= SORT_FLAG_ASC;
					bAsc		= TRUE;
					}
				}
			else{
				m_pGridView->m_nSortCol	= ht.iItem;
				*pSortFlag	= SORT_FLAG_ASC;
				bAsc		= TRUE;
				}

			// Sort column. #######################
			if( !m_pGridView->SortColumn(ht.iItem, bAsc) ){
				*pSortFlag				= nSortFlagOld;
				m_pGridView->m_nSortCol	= nSortColOld;
				}
			// ####################################
			Invalidate();
			}
		}

	if( ht.flags&HHT_ONDIVIDER && ht.iItem > -1 ){
		return;
		}*/
	CHeaderCtrl::OnLButtonDblClk(nFlags, point);
	}

//
//	[afx_msg] CODBCHeaderCtrl::OnLButtonDown
//
void 
CODBCHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
	{/*
	HD_HITTESTINFO ht;
	ht.pt = point;
	::SendMessage(m_hWnd, HDM_HITTEST, 0L, (LPARAM)&ht);

	if( ht.flags&HHT_ONHEADER && ht.iItem > -1 ){
		CRect rcItem;
		GetItemRect(ht.iItem, rcItem);
		rcItem.left ++;
		rcItem.right = rcItem.left + CHECK_MARKWIDTH;
		
		if( rcItem.PtInRect(point) ){
			BOOL bCheck = FALSE;
			if( GetColCheckState(ht.iItem, bCheck) )
				{
				BOOL bCheck = m_pGridView->m_pColByIndex[ht.iItem]->nCheckHeader;
				bCheck		= !bCheck;
				m_pGridView->m_pColByIndex[ht.iItem]->nCheckHeader = bCheck;

				m_pGridView->m_nColCheck = ht.iItem;
				if( bCheck )
					m_pGridView->SendMessage(WM_COMMAND, MENU_ITEM_CHECK, 0L);
				else
					m_pGridView->SendMessage(WM_COMMAND, MENU_ITEM_UNCHECK, 0L);
				}
			}
		}*/
	CHeaderCtrl::OnLButtonDown(nFlags, point);
	}

//
//	[virtual] CODBCHeaderCtrl::GetColCheckState
//
BOOL
CODBCHeaderCtrl::GetColCheckState(int nCol, BOOL& bState)
	{/*
	if( !m_hBmpCheckOn || !m_hBmpCheckOff )
		m_pGridView->GetCheckBitmaps(m_hBmpCheckOn, m_hBmpCheckOff);

	if( m_pGridView->m_pColByIndex && 
		m_pGridView->m_pColByIndex[nCol] ){
		bState = m_pGridView->m_pColByIndex[nCol]->nCheckHeader;
		return m_pGridView->m_pColByIndex[nCol]->bCheckBox;
		}
*/
	return FALSE;
	}

//
//	[virtual] CODBCHeaderCtrl::GetColSortStatus
//
BOOL
CODBCHeaderCtrl::GetColSortStatus(int nCol, BOOL& bState)
	{/*
	if( m_pGridView->m_pColByIndex && 
		m_pGridView->m_pColByIndex[nCol] && 
		m_pGridView->m_pColByIndex[nCol]->bAllowSort && 
		m_pGridView->m_nSortCol == nCol )
		{
		int* pSortFlag = &m_pGridView->m_nSortFlag;
		if( *pSortFlag == SORT_FLAG_NOT )
			return FALSE;
		else
		if( *pSortFlag == SORT_FLAG_ASC )
			bState = TRUE;
		else
			bState = FALSE;
		return TRUE;
		}
*/
	return FALSE;
	}

// ODBCGridView
IMPLEMENT_DYNCREATE(ODBCGridView, GridView)

//
//	ODBCGridView::ODBCGridView
//
ODBCGridView::ODBCGridView() : GridView()
	{/*
	m_pCols						= NULL;
	m_pColByIndex				= NULL;
	m_pRstSelect				= NULL;
	m_pRecord					= NULL;
	m_nNumberCols				= 0;
	m_pFont						= NULL;
	m_bIsRowSelected			= FALSE;
	m_bWaitRowChange			= TRUE;
	m_hWndOwner					= NULL;

	m_pArrRecDataById			= new AutoSortedArray();
	m_pArrRecDataShow			= NULL;
	
	
	m_bAutoRefresh				= TRUE;
	m_bIdSort					= TRUE;
	m_dwExStyle					= 0L;
	m_bUseFirstColAsSelState	= FALSE;
	m_nColCheck					= -1;
	m_menuCheck					= NULL;
	m_bSingleSel				= FALSE;

	m_nSortCol					= 0;
	m_nSortFlag					= SORT_FLAG_NOT;
	m_hMenuMain					= NULL;
	m_bGridInfoLoaded			= FALSE;

	// Create popup menu. ################
	m_menuCheck					= ::CreatePopupMenu();
	::AppendMenuW	(m_menuCheck, MF_STRING|MF_CHECKED,		MENU_ITEM_CHECK,	MENU_ITEM_CHECK_TEXT);
	::AppendMenuW	(m_menuCheck, MF_STRING|MF_UNCHECKED,		MENU_ITEM_UNCHECK,	MENU_ITEM_UNCHECK_TEXT);
	// ###################################

	ODBCStringsMatrix::InitSymbolIndexArray();
	*/
	}

ODBCGridView::~ODBCGridView()
	{
	/*
	if( m_menuCheck )
		DestroyMenu(m_menuCheck);

	if( IsMenu(m_hMenuMain) )
		::DestroyMenu(m_hMenuMain);

	if( m_pArrRecDataShow )
		delete m_pArrRecDataShow;
		*/
	}

BEGIN_MESSAGE_MAP(ODBCGridView, GridView)
	ON_NOTIFY_REFLECT	(LVN_ITEMCHANGED,	OnItemChanged	)
	ON_NOTIFY_REFLECT	(LVN_DELETEITEM,	OnItemDelete	)
	ON_WM_MOUSEACTIVATE	()
	ON_WM_SHOWWINDOW	()
	ON_MESSAGE			(WM_ADD_RECORDS,	OnAddRecords	)
	ON_COMMAND			(MENU_ITEM_CHECK,	OnCheckAll		)
	ON_COMMAND			(MENU_ITEM_UNCHECK,	OnUnCheckAll	)
	ON_WM_CREATE		()
	ON_WM_CONTEXTMENU	()
END_MESSAGE_MAP()

//
//	ODBCGridView::OnDraw
//
void	
ODBCGridView::OnDraw(CDC* pDC)
	{
	}

/*
// #########################################################################
//
//	Hook functionality of odbc32.dll : SQLExecDirectA function.		[START]
//
// #########################################################################

static	AutoSortedArray m_arrGridHwnds;
static PROC				m_funcToHookExecDir;

static BYTE*	m_jmpHookExecDir = new BYTE[5];
static BYTE*	m_jmpRealExecDir = new BYTE[5];

//BOOL SetFunctionHook(PROC funcDest, PROC funcHook, JumpOperation& jmpOp);
BOOL IsInsertQuery	(const CString sSQLQuery, CString& sTableName);
BOOL IsUpdateQuery	(const CString sSQLQuery, CString& sTableName);
BOOL IsDeleteQuery	(const CString sSQLQuery, CString& sTableName);

LONG  SQL_API	
SQLExecDirectA_My(SQLHSTMT StatementHandle, LPTSTR StatementText, SQLINTEGER TextLength)
	{
	BYTE* pJmp;
	// turn back hook operation.///////////////
	pJmp					= (BYTE*)m_funcToHookExecDir;
	pJmp[0]					= m_jmpRealExecDir[0];
	*(long*)(&pJmp[1])		= *((long*)&m_jmpRealExecDir[1]);
	// ========================================

	LONG lRet = 0;
	__asm // call odbc32.dll::SQLExecDirectA function.
		{
		push TextLength;
		push StatementText;
		push StatementHandle;
		mov  eax, m_funcToHookExecDir;
		call eax;
		mov  lRet, eax; // save returned value.
		};

	// write hook jmp./////////////////////////
	pJmp					= (BYTE*)m_funcToHookExecDir;
	pJmp[0]					= m_jmpHookExecDir[0];
	*(long*)(&pJmp[1])		= *((long*)&m_jmpHookExecDir[1]);
	//#########################################

	// If succesfully executed. ###############
	if( ((SQLRETURN)lRet) == SQL_SUCCESS ){
		// Retrieve array of grid HWND's. #####
		CWnd* pMainWnd = AfxGetMainWnd();
		if( !pMainWnd || !pMainWnd->m_hWnd )
			return lRet;
		
		AutoSortedArray* pArrHWNDs = (AutoSortedArray*)GetProp(pMainWnd->m_hWnd, _T("GridHWNDs"));
		if( pArrHWNDs == NULL || pArrHWNDs->GetCount() == 0 )
			return lRet;
		// ####################################

		CString sTable;
		if( IsInsertQuery(StatementText, sTable) ){
			DWORD	dwTableName = DecodeString(sTable.GetBuffer());
			int		nFind		= pArrHWNDs->FindFirst((long)dwTableName);
			while( nFind != -1 )
				{
				ASSERT( pArrHWNDs->GetData(nFind) );
				HWND			hWndGrid	= (HWND)pArrHWNDs->GetData(nFind);
				if( !IsWindow(hWndGrid) )
					return lRet;
				ODBCGridView*	pGridView	= (ODBCGridView*)CWnd::FromHandlePermanent(hWndGrid);

				if( pGridView->IsWindowVisible() )
					pGridView->RefreshForAddNew();

				nFind = pArrHWNDs->FindNext();
				}
			}
		else
		if( IsUpdateQuery(StatementText, sTable) ){
//			long	lCount		= 0L;
//			SQLRowCount(StatementHandle, (SQLINTEGER*)&lCount);
			
			DWORD	dwTableName = DecodeString(sTable.GetBuffer());
			int		nFind		= pArrHWNDs->FindFirst((long)dwTableName);
			while( nFind != -1 )
				{
				ASSERT( pArrHWNDs->GetData(nFind) );
				HWND			hWndGrid	= (HWND)pArrHWNDs->GetData(nFind);
				if( !IsWindow(hWndGrid) ) return lRet;
				ODBCGridView*	pGridView	= (ODBCGridView*)CWnd::FromHandlePermanent(hWndGrid);

				if( pGridView->IsWindowVisible() )
					{
					CString sUpdateSQL	= StatementText;
					long	lUid		= pGridView->UIDFromUpdateSQLQuery(sUpdateSQL);
					if( lUid > 0 )
						pGridView->RefreshForUpdate(lUid);
					}

				nFind = pArrHWNDs->FindNext();
				}
			}
		else
		if( IsDeleteQuery(StatementText, sTable) ){
			DWORD	dwTableName = DecodeString(sTable.GetBuffer());
			int		nFind		= pArrHWNDs->FindFirst((long)dwTableName);
			while( nFind != -1 ){
				ASSERT( pArrHWNDs->GetData(nFind) );
				HWND			hWndGrid	= (HWND)pArrHWNDs->GetData(nFind);
				if( !IsWindow(hWndGrid) ) return lRet;
				ODBCGridView*	pGridView	= (ODBCGridView*)CWnd::FromHandlePermanent(hWndGrid);

				if( pGridView->IsWindowVisible() ){
					CString sDeleteSQL	= StatementText;
					long	lUid		= pGridView->UIDFromDeleteSQLQuery(sDeleteSQL);
					if( lUid > 0 )
						pGridView->RefreshForDelete(lUid);
					}

				nFind = pArrHWNDs->FindNext();
				}
			}
		else
			return lRet;
		}
	// ########################################
	return lRet;
	}

//
//	IsUpdateQuery
//
BOOL 
IsUpdateQuery(const CString sSQLQuery, CString& sTableName)
	{
	CString			sTmp		= sSQLQuery;
	const CString	sUpdateSign = _T("UPDATE");
	
	// Remove some chars. #############
	sTmp.Remove(' ');
	sTmp.Remove('\t');
	sTmp.Remove('\r');
	sTmp.Remove('\n');
	sTmp.MakeUpper();
	// ################################

	CString			sSignature	= sTmp.Left(sUpdateSign.GetLength()).MakeUpper();
	if( sSignature ==  sUpdateSign ){
		// retrieve table name. #####
		int nStartIndex = sUpdateSign.GetLength();
		int nEndIndex	= sTmp.Find("SET", nStartIndex);
		ASSERT( nEndIndex > nStartIndex );

		sTableName = sTmp.Mid(nStartIndex, nEndIndex - nStartIndex);
		// ##########################
		return TRUE;
		}
	
//	update  AXSYS_BACK_CLIENT 
//	set 
//	ORDER_N		= '[ord]', 
//	CLIENT_UID	= [cstuid], 
//	INCOME_DATE	= '[indat]', 
//	REAL_PAYMENT	= [rpmnt],  
//	CLOSED		= [clsd] 
//	where UID	= [uid]
	
	return FALSE;
	}

//
//	IsInsertQuery
//
BOOL 
IsInsertQuery(const CString sSQLQuery, CString& sTableName)
	{
	CString			sTmp		= sSQLQuery;
	const CString	sInsertSign = _T("INSERTINTO");
	
	// Remove some chars. #############
	sTmp.Remove(' ');
	sTmp.Remove('\t');
	sTmp.Remove('\r');
	sTmp.Remove('\n');
	sTmp.MakeUpper();
	// ################################

	CString			sSignature	= sTmp.Left(sInsertSign.GetLength()).MakeUpper();
	if( sSignature ==  sInsertSign ){
		// retrieve table name. #####
		int nStartIndex = sInsertSign.GetLength();
		int nEndIndex	= sTmp.Find('(', nStartIndex);
		ASSERT( nEndIndex > nStartIndex );

		sTableName = sTmp.Mid(nStartIndex, nEndIndex - nStartIndex);
		// ##########################
		return TRUE;
		}

	
//	insert into AXSYS_BARCODE 
//  (NAME, DESCRIPTION, BARCODE) 
//  values('[nam]', '[desc]', '[barc]');
	
	return FALSE;
	}

//
//	IsDeleteQuery
//
BOOL 
IsDeleteQuery(const CString sSQLQuery, CString& sTableName)
	{
	const CString	sDeleteSign = _T("DELETEFROM");
	CString			sTmp		= sSQLQuery;
	sTmp.MakeUpper();
	int				nFindWhere	= sTmp.Find("WHERE");
	if( nFindWhere == -1 )
		return FALSE;
	else
		sTmp					= sTmp.Left(nFindWhere);
	
	// Remove some chars. #############
	sTmp.Remove(' ');
	sTmp.Remove('\t');
	sTmp.Remove('\r');
	sTmp.Remove('\n');
	// ################################

	CString			sSignature	= sTmp.Left(sDeleteSign.GetLength()).MakeUpper();
	if( sSignature ==  sDeleteSign ){
		// retrieve table name. #####
		int nStartIndex = sDeleteSign.GetLength();
		sTableName = sTmp.Mid(nStartIndex, sTmp.GetLength() - nStartIndex);
		// ##########################
		return TRUE;
		}

	//delete from AXSYS_BACK_CLIENT where UID = [uid];
	return FALSE;
	}

//
//	SetAPIFunctionHook
//
BOOL              
SetAPIFunctionHook(PROC funcDest, PROC funcHook, BYTE* jmpOp)
	{
	// Change protection option for memory block.
	MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(funcDest, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
    VERIFY(VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect));
		
	// write hook function jmp instruction.//
	((BYTE*)funcDest)[0]			= 0xE9;
	*((long*)&((BYTE*)funcDest)[1]) = long(funcHook) - long(5 + long(funcDest));
	//---------------------------------------

	// save hook function jump instruction.//
	jmpOp[0]                        = ((BYTE*)funcDest)[0];
	*((long*)&jmpOp[1])             = *((long*)&((BYTE*)funcDest)[1]);
	//---------------------------------------
    return TRUE;
	}

// #########################################################################
//
//	Hook functionality of odbc32.dll : SQLExecDirectA function.		[END]
//
// #########################################################################
*/

//
//	ODBCGridView::OnInitialUpdate
//
void
ODBCGridView::OnInitialUpdate()
	{
	GridView::OnInitialUpdate();

	if( m_sGridName.IsEmpty() )
		m_sGridName = GetRuntimeClass()->m_lpszClassName;

	// Register self as global named object. ############
//	GlobalStorage::GetInstance()->SetNamedObject(m_sGridName.GetBuffer(), this);
	// ##################################################

	// Retrieve system font. ############################
	CListCtrl&			list	= GetListCtrl();
	NONCLIENTMETRICS	ncm;
	ncm.cbSize					= sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, NULL);
	// ##################################################
	// Set list view's font. ############################
	m_pFont						= new CFont();
	m_pFont->CreateFontIndirect(&ncm.lfCaptionFont);
	list.SetFont(m_pFont);
	// ##################################################
	// Set extended style. ##############################
	if( !m_dwExStyle )
		m_dwExStyle = LVS_EX_ONECLICKACTIVATE|LVS_EX_UNDERLINEHOT|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES;
	list.SetExtendedStyle(m_dwExStyle);
	// ##################################################


	// Set header font. #################################
	if( IsWindow(m_headerCtrl) )
		{
		m_headerCtrl.SetFont	(m_pFont);
		m_headerCtrl.SetGradient(RGB(189, 196, 212), RGB(237, 237, 237));
		}
	else
		{
		CListCtrl&		list	= GetListCtrl();
		CHeaderCtrl*	pHeader = list.GetHeaderCtrl();
		if( pHeader && IsWindow(pHeader->GetSafeHwnd()) )
			pHeader->SetFont(m_pFont);
		}
	// #################################################

	// Reinitialize header. ############################
	ReInitializeHeader();
	// #################################################

	// Set row selection mode. #########################
	SetCellSelectionMode(FALSE);
	// #################################################
/*
	// Don't hook if already hooked. ###################
	m_funcToHookExecDir		= (PROC)GetProcAddress(LoadLibrary(_T("odbc32.dll")), _T("SQLExecDirectA"));

	CWnd*	pMainWnd	= AfxGetMainWnd();
	ASSERT( pMainWnd && pMainWnd->m_hWnd );
	
	BYTE	btCode		= (BYTE)	GetProp(pMainWnd->m_hWnd, _T("realJumpCode"));
	DWORD	dwOffset	= (DWORD)	GetProp(pMainWnd->m_hWnd, _T("realJumpOffset"));
	if( !btCode )
		{
		m_jmpRealExecDir[0] = ((BYTE*)m_funcToHookExecDir)[0];
		m_jmpRealExecDir[1] = ((BYTE*)m_funcToHookExecDir)[1];
		m_jmpRealExecDir[2] = ((BYTE*)m_funcToHookExecDir)[2];
		m_jmpRealExecDir[3] = ((BYTE*)m_funcToHookExecDir)[3];
		m_jmpRealExecDir[4] = ((BYTE*)m_funcToHookExecDir)[4];

		SetProp(pMainWnd->m_hWnd, _T("realJumpCode"),	(HANDLE)m_jmpRealExecDir[0]);
		SetProp(pMainWnd->m_hWnd, _T("realJumpOffset"), (HANDLE)*((long*)&m_jmpRealExecDir[1]));
		}
	else
		{
		m_jmpRealExecDir[0]					= btCode;
		*((long*)&m_jmpRealExecDir[1])		= (long)dwOffset;
		}
	SetAPIFunctionHook(m_funcToHookExecDir, (PROC)SQLExecDirectA_My, m_jmpHookExecDir);
	// #################################################
*/
	}

//
//	ODBCGridView::OnDestroy
//
void 
ODBCGridView::OnDestroy()
	{
	/*
	// Remove from global storage. ######################
	GlobalStorage::GetInstance()->RemoveNamedObject(m_sGridName.GetBuffer());
	// ##################################################

	EmptyList();
	if( m_pArrRecDataById )
		delete m_pArrRecDataById;

	// Delete font object.#######
	if( m_pFont )
		delete m_pFont;
	// ##########################

	// Delete image list. #######
	DeleteObject(m_bmImageList);
	// ##########################
*/
	GridView::OnDestroy();
	}

//
//	[public] ODBCGridView::IsCmdHandled
//
BOOL
ODBCGridView::IsCmdHandled(UINT nCode, UINT nCmdId)
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
//	[public] ODBCGridView::FindKeyMoveTo
//
BOOL
ODBCGridView::FindKeyMoveTo(LPCTSTR lpszKey, BOOL bMove /*= TRUE*/)
	{/*
	ASSERT( m_pColByIndex && m_pArrRecDataById );

//	m_csection.Lock();
//	int nFindIndex = m_pArrRecDataById->FindValue(atoi(lpszKey));
//	m_csection.Unlock();
//	if( nFindIndex != -1 )
//		{
//		SelectItem(nFindIndex);
//		return TRUE;
//		}

	AutoSortedArray* pArr	= m_pArrRecDataById;
	if( m_pArrRecDataShow )
		pArr = m_pArrRecDataShow;

	m_csection.Lock();
	int		nCount	= pArr->GetCount();
	int		nLoop	= 0;
	long	lUid	= atoi(lpszKey);

	while( nLoop < nCount )
		{
		if( pArr->Get(nLoop) == lUid )
			{
			m_csection.Unlock();
			if( bMove )
				{
				SelectItem(nLoop);
				return TRUE;
				}
			else
				return nLoop;
			}
		nLoop ++;
		}
	m_csection.Unlock();
	return bMove ? TRUE : -1;*/return FALSE;
	}

//
//	[public] ODBCGridView::MoveTo
//
void
ODBCGridView::MoveTo(int nItem)
	{
	SelectItem(nItem);
	}

//
//	[public] ODBCGridView::GetRowId
//
CString			
ODBCGridView::GetRowId(int nItem /*= -1*/)
	{
	long	lRowId = GetRowIdAsLong(nItem);
	CString sRowId;
	sRowId.Format("%d", lRowId);
	return sRowId;
	}

//
//	[public] ODBCGridView::GetRowIdAsLong
//
long
ODBCGridView::GetRowIdAsLong(int nItem /*= -1*/)
	{/*
	ASSERT( m_pArrRecDataById && m_nUIDField > -1 );

	AutoSortedArray* pArr = m_pArrRecDataById;
	if( m_pArrRecDataShow )
		pArr = m_pArrRecDataShow;

	if( nItem == -1 )
		{
		if( m_pRecord->HaveNullRecordData() )
			return 0;
		return m_pRecord->GetLong(m_nUIDField);
		}
	else
		{
		m_csection.Lock();
		ASSERT( nItem > -1 && nItem < pArr->GetCount() );
		void* lpRecData		= (void*)pArr->GetData(nItem);
		ASSERT( lpRecData );
		void* lpOldRecData	= m_pRecord->GetRecordData();

		m_pRecord->SetRecordData(lpRecData		);
		long lValue = m_pRecord->GetLong(m_nUIDField);
		m_pRecord->SetRecordData(lpOldRecData	);
		m_csection.Unlock();
		return lValue;
		}*/
	return 0;
	}

//
//	[public] ODBCGridView::SelectSQLWithUID
//
BOOL			
ODBCGridView::SelectSQLWithUID(CString& sSelectSQL, CString sUIDField, long lUid)
	{/*
	CString sFind1	= _T("FROM");
	CString sFind2	= _T("WHERE");
	CString sFind3	= _T("ORDER BY");

	int		nFind	= -1;
	int		nFind2	= -1;
	int		nFind3	= -1;

	CString sTemp	= sSelectSQL;
	sTemp.MakeUpper();

	// Find 'FROM' clause. ##############
	nFind		= sTemp.Find(sFind1);
	int nFind1	= -1;
	while( nFind != -1 )
		{
		nFind1	= nFind;

		int nStart = nFind1 + (int)strlen(sFind1);
		while(	sSelectSQL[nStart] == ' '	||
				sSelectSQL[nStart] == '\r'	||
				sSelectSQL[nStart] == '\n'	||
				sSelectSQL[nStart] == '\t'	)
			{
			nStart ++;
			}

		CString sTableName = sSelectSQL.Mid(nStart, m_strTableName.GetLength());
		if( !sTableName.CompareNoCase(m_strTableName) )
			{
			break;
			}

		nFind	= sSelectSQL.Find(sFind1 , nFind1 + (int)strlen(sFind1));
		}
	if( nFind1 == -1 )
		return FALSE;
	// ##################################

	// Find 'WHERE' clause. #########
	nFind2		= sTemp.Find(sFind2, nFind1 + (int)strlen(sFind1));
	// ##################################

	// Find 'ORDER' clause. #########
	nFind3		= sTemp.Find(sFind3, nFind1 + (int)strlen(sFind1));
	// ##################################

	// if WHERE exists.
	CString sClauseAdd;
	if( nFind2 != -1 )
		sClauseAdd.Format(" AND %s.%s=%d", m_strTableName, sUIDField, lUid);
	else
		sClauseAdd.Format(" WHERE %s=%d ", sUIDField, lUid);

	// if ORDER exists.
	if( nFind3 != -1 )
		sSelectSQL.Insert( nFind3-1, sClauseAdd);
	else
		sSelectSQL += sClauseAdd;
*/
	return TRUE;
	}

//
//	[public] ODBCGridView::UIDFromUpdateSQLQuery
//
long			
ODBCGridView::UIDFromUpdateSQLQuery(CString& sUpdateSQL)
	{/*
	sUpdateSQL.MakeUpper();
	int nFind = sUpdateSQL.Find("WHERE");
	if( nFind == -1 )
		return 0;

	nFind = sUpdateSQL.Find(m_sUIDField, nFind);
	if( nFind == -1 )
		return 0;

	nFind	+= m_sUIDField.GetLength();

	while(	sUpdateSQL[nFind] == ' '	||
			sUpdateSQL[nFind] == '\r'	||
			sUpdateSQL[nFind] == '\n'	||
			sUpdateSQL[nFind] == '\t'	)
		{
		nFind ++;
		}

	if( sUpdateSQL[nFind] != '=' )
		return 0;
	else
		nFind ++;

	while(	sUpdateSQL[nFind] == ' '	||
			sUpdateSQL[nFind] == '\r'	||
			sUpdateSQL[nFind] == '\n'	||
			sUpdateSQL[nFind] == '\t'	)
		{
		nFind ++;
		}

	char*	pszUpdateSQL	= sUpdateSQL.GetBuffer();
	long	lUid			= atoi(((LPTSTR)&pszUpdateSQL[nFind]));
	return lUid;*/ return 0;
	}

//
//	[public] ODBCGridView::UIDFromDeleteSQLQuery
//
long			
ODBCGridView::UIDFromDeleteSQLQuery(CString& sDeleteSQL)
	{/*
	sDeleteSQL.MakeUpper();
	int nFind = sDeleteSQL.Find("WHERE");
	if( nFind == -1 )
		return 0;

	nFind = sDeleteSQL.Find(m_sUIDField, nFind);
	if( nFind == -1 )
		return 0;

	nFind	+= m_sUIDField.GetLength();

	while(	sDeleteSQL[nFind] == ' '	||
			sDeleteSQL[nFind] == '\r'	||
			sDeleteSQL[nFind] == '\n'	||
			sDeleteSQL[nFind] == '\t'	)
		{
		nFind ++;
		}

	if( sDeleteSQL[nFind] != '=' )
		return 0;
	else
		nFind ++;

	while(	sDeleteSQL[nFind] == ' '	||
			sDeleteSQL[nFind] == '\r'	||
			sDeleteSQL[nFind] == '\n'	||
			sDeleteSQL[nFind] == '\t'	)
		{
		nFind ++;
		}

	char*	pszDeleteSQL	= sDeleteSQL.GetBuffer();
	long	lUid			= atoi(((LPTSTR)&pszDeleteSQL[nFind]));
	return lUid;*/ return 0;
	}

//
//	[public] ODBCGridView::RefreshForAddNew
//
void			
ODBCGridView::RefreshForAddNew()
	{/*
	ASSERT( m_pRstSelect && !m_pRstSelect->IsOpen() );
	if( !m_bAutoRefresh ) return;

	void*	lpRecData	= m_pRecord->GetRecordData();
	long	lUid		= 0;

	if( m_sGeneratorName.IsEmpty() )
		GetLastUIDofTable(lUid);
	else
		lUid = atoi(RMC::get_ncv2(m_pRstSelect->m_pDatabase, m_sGeneratorName));

	if( lUid == 0 )
		{
		m_pRecord->SetRecordData(lpRecData);
		return;
		}

	// Build select SQL for only one, addnew record. ##########
	CString sSQLSelect = m_pRstSelect->GetLastSqlSource();
	if( !SelectSQLWithUID(sSQLSelect, m_sUIDField, lUid) )
		{
		m_pRecord->SetRecordData(lpRecData);
		return;
		}
	// ########################################################

	// Select only new record. ################################
	CString sSQLSave = m_pRstSelect->GetLastSqlSource();
	m_pRstSelect->Open				(sSQLSelect);
	m_pRstSelect->SetLastSqlSource	(sSQLSave);

	if( m_pRstSelect->GetRecordCount() == 0 )
		{
		delete m_pRecord->GetRecordData	();
		m_pRecord	->SetRecordData		(lpRecData);
		m_pRstSelect->Close				();
		return;
		}
	m_pRstSelect->Close		();
	// ########################################################

	// Add new record in array. ###############################
	long lUidNew	= m_pRstSelect->GetLong(m_sUIDField);
	ASSERT( lUidNew == lUid );
	m_csection.Lock();
	void* lpRecDataNew = m_pRecord->GetRecordData();
	m_pArrRecDataById->Add(lUidNew, (long)lpRecDataNew);
	if( m_pArrRecDataShow )
		m_pArrRecDataShow->Add(lUidNew, (long)lpRecDataNew);
	// ########################################################

	// Add new item and select it. ############################
	GetListCtrl().InsertItem(GetRowCount(), "");
	SelectItem(m_pArrRecDataById->GetCount() - 1);
	RedrawItem(m_pArrRecDataById->GetCount() - 1);
	m_csection.Unlock();
	// ########################################################
	*/
	}

//
//	[public] ODBCGridView::RefreshForUpdate
//
void
ODBCGridView::RefreshForUpdate(long lUid)
	{/*
	ASSERT( m_pRstSelect && !m_pRstSelect->IsOpen() );
	if( !m_bAutoRefresh ) return;

	// Find if exists record with that UID. #######################
	int			nFind	= -1;
	m_csection.Lock();

	nFind	= m_pArrRecDataById->FindValue(lUid);
	if( nFind == -1 )
		{
		m_csection.Unlock();
		return;
		}
	// ############################################################
	// Build select SQL for only one, updated record. #############
	void*	lpRecData	= (void*)m_pArrRecDataById->GetData(nFind);
	CString sSQLSelect	= m_pRstSelect->GetLastSqlSource();

	if( !SelectSQLWithUID(sSQLSelect, m_sUIDField, lUid) )
		{
		m_csection.Unlock();
		return;
		}
	// ############################################################
	// Query for updated record data. #############################
	m_pRecord	->SetRecordData	(NULL);

	CString sSQLSave = m_pRstSelect->GetLastSqlSource();
	m_pRstSelect->Open				(sSQLSelect);
	m_pRstSelect->SetLastSqlSource	(sSQLSave);

	if( m_pRstSelect->GetRecordCount() == 0 )
		{
		delete m_pRecord->GetRecordData	();
		m_pRecord	->SetRecordData		(lpRecData);
		m_pRstSelect->Close				();

		// if not exists then delete from list.
		CString sKey;
		sKey.Format("%d", nFind);
		int nFind = FindKeyMoveTo(sKey, FALSE);
		GetListCtrl().DeleteItem(nFind);
		m_csection.Unlock();
		return;
		}

	void* lpRecDataNew = m_pRecord->GetRecordData();
	memcpy(lpRecData, lpRecDataNew, m_pRecord->GetRecordSize());
	delete lpRecDataNew;
	m_pRstSelect->Close();
	// ############################################################
	// Select edited item. ########################################
	CString sKey;
	sKey.Format("%d", lUid);
	FindKeyMoveTo	(sKey);
	RedrawItem		(nFind);
	// ############################################################
	m_csection.Unlock();
	*/
	}

//
//	[public] ODBCGridView::RefreshForDelete
//
void			
ODBCGridView::RefreshForDelete(long lUid)
	{/*
	ASSERT( m_pRstSelect && !m_pRstSelect->IsOpen() );
	if( !m_bAutoRefresh ) return;
	m_csection.Lock();
	
	// Find if exists record with that UID. #######################
	int	nFind	= m_pArrRecDataById->FindValue(lUid);
	if( nFind == -1 )
		{
		m_csection.Unlock();
		return;
		}
	// ############################################################

	// Delete item from list. #####################################
	GetListCtrl().DeleteItem(nFind);
	// ############################################################
	m_csection.Unlock();
	*/
	}

//
//	[public] ODBCGridView::DefineColumn
//
void
ODBCGridView::DefineColumn(int nIndex, LPCTSTR lpszTitle, LPCTSTR lpszDBField, 
						   int nAlignment, int nWidth, int iImage /*= -1*/, 
						   LPCTSTR lpszChkField /*= NULL*/, int nScaleShow /*= -1*/)
	{
	/*
	CListCtrl&	list		= GetListCtrl();

	LVCOLUMN	column;
	column.mask				= LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	if( iImage > -1 )
		column.mask			|= LVCF_IMAGE;
	column.pszText			= (LPTSTR)lpszTitle;
	column.cchTextMax		= 255;
	column.fmt				= nAlignment;
	column.cx				= nWidth;
	column.iImage			= iImage;
	int			nNewCol		= list.InsertColumn(nIndex, &column);
	ASSERT( nWidth > 0 );

	ODBCGridCol* pNewCol	= new ODBCGridCol;
	pNewCol->sFieldName		= lpszDBField;
	pNewCol->sTitle			= lpszTitle;
	pNewCol->nWidth			= nWidth;
	pNewCol->pNextCol		= NULL;
	pNewCol->nFieldID		= -1;
	pNewCol->sCheckFieldName= lpszChkField ? lpszChkField : "";
	pNewCol->nCheckFieldId	= -1;
	pNewCol->nCheckHeader	= 0;
	pNewCol->pGetAsTextFunc	= NULL;
	pNewCol->iImage			= iImage;
	pNewCol->bRowIndex		= pNewCol->sFieldName == "$ROW_INDEX$" ? TRUE : FALSE;
	pNewCol->bCheckBox		= (lpszChkField && strlen(lpszChkField) > 0) ? TRUE : FALSE;
	pNewCol->bAllowSort		= TRUE;
	pNewCol->nScaleShow		= nScaleShow;

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
	*/
	}

//
//	[public] ODBCGridView::SetImageList
//
void
ODBCGridView::SetImageList(UINT imageId, int cx, int cy, 
						   UINT flags, int images, COLORREF transparent, UINT imageMaskId /*= 0L*/)
	{
	/*
	// Delete old object. #################
	DeleteObject(m_bmImageList);
	// ####################################

	BYTE bRed	= (BYTE)transparent & 0x0000FF;
	BYTE bGreen = (BYTE)((transparent & 0x00FF00) >> 8);
	BYTE bBlue	= (BYTE)((transparent & 0xFF0000) >> 16);

	// Load and convert image. #############
	m_bmImageList.LoadBitmap(imageId);
	HBITMAP hImage			= m_bmImageList;
	m_bmImageList.m_hObject = NULL;
	CBitmapEx::ConvertToBitmap32bpp(hImage);
	// #####################################

	BITMAP bmImage;
	GetObject(hImage, sizeof(BITMAP), &bmImage);

	int nWidth		= bmImage.bmWidth;
	int nHeight		= bmImage.bmHeight;
	int nLoop		= 0;
	int nPixelCount = nHeight*nWidth;

	// Set alpha parameter for transparent pixel. ################
	while( nLoop < nPixelCount )
		{
		RGBQUAD* pRGBQuad	= &((RGBQUAD*)bmImage.bmBits)[nLoop];
		COLORREF crPixel	= RGB(pRGBQuad->rgbRed, pRGBQuad->rgbGreen, pRGBQuad->rgbBlue);
		if( crPixel == transparent )
			pRGBQuad->rgbReserved = 0x00;
		else
			pRGBQuad->rgbReserved = 0xFF;
		nLoop ++;
		}
	// ###########################################################

	m_bmImageList.m_hObject = hImage;
	*/
	}

//
//	[public] ODBCGridView::SetRowId
//
void
ODBCGridView::SetRowId(const CString sUIDField)
	{
	m_sUIDField		= sUIDField;
	}
/*
//
//	[public] ODBCGridView::SetRst
//
iODBCRst*			
ODBCGridView::SetRst(iODBCRst* pRst)
	{
	ASSERT( pRst );
	iODBCRst* pRstOld	= m_pRstSelect;
	m_pRstSelect		= pRst;
	m_pRecord			= pRst->GetRecordPtr();
	return pRstOld;
	}
*/
//
//	[virtual] ODBCGridView::OnSetup
//
void	
ODBCGridView::OnSetup()
	{
	}

//
//	[public] ODBCGridView::LoadOrReload
//
BOOL
ODBCGridView::LoadOrReload(BOOL bLoad /*= FALSE*/)
	{/*
	// Is not initialized. #####
	if( m_nNumberCols <= 0 )
		return FALSE;
	// #########################

	BOOL bRet = FALSE;
	if( !bLoad ){
		if( m_pColByIndex )
			bRet = ReLoad();
		else
			bRet = Load();
		}
	else
		bRet = Load();

	CListCtrl&	list = GetListCtrl();
	if( list.GetItemCount() > 0 ){
		if( mRow != 0 ){
			SelectItem(0);
			}
		// Manually call rowchange event.
		else{ 
			void*	lpRecData	= (void*)m_pArrRecDataById->GetData(0);
			ASSERT( lpRecData );
			m_pRecord->SetRecordData(lpRecData);

			// Call rowchanged virtual. ########
			if( m_hWndOwner == NULL )
				RowChanged();
			else
				::SendMessage(m_hWndOwner, WM_ROW_CHANGED, mRow, 0L);
			// #################################
			}
		}
	else
		m_pRecord->SetRecordData(NULL);

	if( m_pArrRecDataShow ){
		delete m_pArrRecDataShow;
		m_pArrRecDataShow = NULL;
		}

	if( m_nSortFlag != SORT_FLAG_NOT ){
		m_nSortCol	= -1;
		m_nSortFlag = SORT_FLAG_NOT;

		if( IsWindow(m_headerCtrl) )
			m_headerCtrl.Invalidate();
		}

	return bRet;*/
		return FALSE;
	}

//
//	[public] ODBCGridView::Load
//
BOOL 
ODBCGridView::Load()
	{/*
	ASSERT( m_pRstSelect && m_pRstSelect->IsOpen() && m_nNumberCols > 0 );
	AfxGetApp()->BeginWaitCursor();
	SetRedraw(FALSE);
	CListCtrl&	list = GetListCtrl();

	if( m_pArrRecDataShow ){
		delete m_pArrRecDataShow;
		m_pArrRecDataShow = NULL;
		}

	// Open recordset and load record data. ##########
	try
		{
		// Free record data array. ###################
		FreeRecordDataArray();
		// ###########################################

		m_csection.Lock();

		m_pRecord	= m_pRstSelect->GetRecordPtr();
		ASSERT( m_pRecord );
		m_nUIDField = m_pRecord->GetFieldID(m_sUIDField);
		ASSERT( m_nUIDField > -1 );

		m_pArrRecDataById->OptimizeForLargeData	(TRUE);
		BuildODBCColumnsArray					();
		m_pRecord	->SetFreeDataOnDestroy		(FALSE);
		m_pRstSelect->SetRecordDataArray		(m_pArrRecDataById);
		m_pRstSelect->SetUniqueField			(m_sUIDField);

		void*	lpRecData		= m_pRecord->GetRecordData();
		int		nRecDataSize	= m_pRecord->GetRecordSize();
		m_pRecord->SetRecordData(lpRecData);

		// Retrieve base table name. ##############################
		m_pRstSelect->GetBaseTableName(m_pRstSelect->GetFieldID(m_sUIDField)+1, m_strTableName);
		// ########################################################

		// Add grid control's HWND to sorted array of grid HWNDs. #
		CWnd* pMainWnd = AfxGetMainWnd();
		ASSERT( pMainWnd && pMainWnd->m_hWnd );
		
		AutoSortedArray* pArrHWNDs = (AutoSortedArray*)GetProp(pMainWnd->m_hWnd, _T("GridHWNDs"));
		if( pArrHWNDs == NULL ){
			pArrHWNDs = new AutoSortedArray();
			SetProp(pMainWnd->m_hWnd, _T("GridHWNDs"), (HANDLE)pArrHWNDs);
			}

		long	lCode	= (long)DecodeString(m_strTableName.GetBuffer());
		int		nFind	= pArrHWNDs->FindFirst(lCode);
		BOOL	bAdded	= FALSE;
		while( nFind != -1 )
			{
			if( pArrHWNDs->GetData(nFind) == (long)m_hWnd ){
				bAdded = TRUE;
				break;
				}
			nFind = pArrHWNDs->FindNext();
			}

		if( !bAdded )
			pArrHWNDs->Add(lCode, (long)m_hWnd);
		// ########################################################

		while( !m_pRstSelect->IsEOF() ){
			long lUID			= m_pRecord->GetLong(m_nUIDField);
			
			// Allocate record data. #############################
			void* lpNewRecData	= new BYTE[nRecDataSize];
			memcpy(lpNewRecData, lpRecData, nRecDataSize);
			// ###################################################

			m_pArrRecDataById->Add	(lUID, (long)lpNewRecData);
			m_pRstSelect->MoveNext	();
			}

		// Allocate items in list control. #########################
		int nItemCount = m_pArrRecDataById->GetCount();
		if( nItemCount >= 0 ){
			// Force my virtual list view control to allocate.
			list.SetItemCount(nItemCount);
			}
		// ########################################################
		

		delete lpRecData;
		m_pRstSelect->Close();
		}
	catch(CDBException* e)
		{
		m_csection.Unlock();
		SetRedraw(TRUE);
		m_strLastError = e->m_strError;
		e->Delete();
		AfxGetApp()->EndWaitCursor();
		return FALSE;
		}
	// ###############################################
	m_csection.Unlock();
	SetRedraw(TRUE);
	AfxGetApp()->EndWaitCursor();*/
	return TRUE;
	}

//
//	[public] ODBCGridView::ReLoad
//

BOOL
ODBCGridView::ReLoad(CStringArray* pArrFields /*= NULL*/)
	{/*
	ASSERT( m_pRstSelect && m_pRstSelect->IsOpen() && m_nNumberCols > 0 );
	AfxGetApp()->BeginWaitCursor();
	SetRedraw(FALSE);
	CListCtrl&	list = GetListCtrl();

	m_csection.Lock();

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
		pArrIds		->SetCompareFunction	(m_pArrRecDataById->GetCompareFunc());
		pArrIds		->OptimizeForLargeData	(TRUE);
		m_pRecord	->SetFreeDataOnDestroy	(FALSE);
		m_pRstSelect->SetRecordDataArray	(pArrIds);
		m_pRstSelect->SetUniqueField		(m_sUIDField);

		while( !m_pRstSelect->IsEOF() )
			{
			long			lUID			= m_pRecord->GetLong(m_nUIDField);
			void*			lpNewRecData	= NULL;

			// Retrieve record data. #########################
			if( iLoop >= nOldRecCount )
				lpNewRecData = new BYTE[nRecDataSize];
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
			iLoop ++;
			}
		delete m_pArrRecDataById;
		m_pArrRecDataById = pArrIds;
		// ###################################################

		// Allocate items in list control. #########################
		int nItemCount = m_pArrRecDataById->GetCount();
		if( nItemCount > 0 )
			{
			// Force my virtual list view control to allocate.
			list.SetItemCount(nItemCount);
			}
		else
			{
			list.DeleteAllItems	();
			m_pRecord->SetRecordData(NULL);
			}
		// ########################################################

		delete lpRecData;
		m_pRstSelect->Close();
		}
	catch(CDBException* e)
		{
		SetRedraw(TRUE);
		m_strLastError = e->m_strError;
		e->Delete();
		AfxGetApp()->EndWaitCursor();
		m_csection.Unlock();
		return FALSE;
		}
	// ###############################################

	SetRedraw(TRUE);
	AfxGetApp()->EndWaitCursor();

	// Select first item. ############################
	if( m_pArrRecDataById->GetCount() > 0 )
		{
		if( mRow == 0 )
			{
			void*			lpRecData	= (void*)m_pArrRecDataById->GetData(0);
			ASSERT( lpRecData );
			m_pRecord->SetRecordData(lpRecData);

			// Call rowchanged virtual. ########
			if( m_hWndOwner == NULL )
				RowChanged();
			else
				::SendMessage(m_hWndOwner, WM_ROW_CHANGED, mRow, 0L);
			// #################################
			}
		else
			SelectItem(0);
		}
	else{
		// Call rowchanged virtual. ########
		if( m_hWndOwner == NULL )
			RowChanged();
		else
			::SendMessage(m_hWndOwner, WM_ROW_CHANGED, mRow, 0L);
		// #################################
		}
	// ###############################################
	m_csection.Unlock();*/
	return TRUE;
	}

//
//	[virtual] ODBCGridView::OnChooseGridFont
//
bool
ODBCGridView::OnChooseGridFont(OUT LOGFONT* plf)
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
	strcpy_s(plf->lfFaceName, _T("GeoMegasoft"));
	return true;
	}

//
//	[virtual] ODBCGridView::OnDrawCellIcon
//
bool
ODBCGridView::OnDrawCellIcon(IN int Row, IN int Col, IN HDC dc, IN int x, IN int y, IN COLORREF crBkgnd)
	{/*
	ODBCGridCol*	pCol	= m_pColByIndex[Col];
	DetermineCellIcon(Row, Col, pCol->iImage);
	if( pCol->iImage > -1 ){
		CDC*	pDC		= CDC::FromHandle(DC);
		pDC->FillSolidRect(CRect(x, y, x + 17, y + 17), crBkgnd);

		CDC		memDC;
		memDC.CreateCompatibleDC(NULL);
		HBITMAP hBkgnd	= CBitmapEx::GetBitmap32bppFromDC(pDC, x, y, 16, 16);
		memDC.SelectObject		(hBkgnd);

		CPoint ptImageSrc(pCol->iImage*16, 0);
		if( CBitmapEx::BitmapOnBitmap(hBkgnd, 0, 0, m_bmImageList, ptImageSrc.x, ptImageSrc.y, ptImageSrc.x + 16, 16, 16, 16, TRUE) ){
			BitBlt(DC, x, y, 16, 16, memDC, 0, 0, SRCCOPY);
			}

		memDC.DeleteDC	();
		DeleteObject	(hBkgnd);
		return true;
		}*/
	return false;
	}

//
//	[virtual] ODBCGridView::OnGetCellBorders
//
bool 
ODBCGridView::OnGetCellBorders(	IN  int			Row,
								IN  int			Col,
								OUT COLORREF&	crBorder,
								OUT UINT&		nPenStyle,
								OUT UINT&		nBorderStyle,
								OUT int&		nWidth) const
	{/*
	nPenStyle		= PS_SOLID;
	nWidth			= 1;
	bool	bSel	= mDraw.Selected;

	if( m_bUseFirstColAsSelState && 
		m_pColByIndex[0]->bRowIndex		== TRUE && 
		m_pColByIndex[0]->nCheckFieldId	> -1 )
		{
//		m_csection.Lock();
		AutoSortedArray* pArr = m_pArrRecDataById;
		if( m_pArrRecDataShow )
			pArr = m_pArrRecDataShow;
		void*			lpRecData	= (void*)pArr->GetData(Row);
//		m_csection.Unlock();

		void* lpRecDataOld = m_pRecord->GetRecordData();
		m_pRecord->SetRecordData(lpRecData);
		BOOL			bCheck		= (BOOL)m_pRecord->GetLong(m_pColByIndex[0]->nCheckFieldId);
		if( bCheck != FALSE  )
			bSel	= true;
		m_pRecord->SetRecordData(lpRecDataOld);
		}

	if( !bSel )
		crBorder		= RGB(217, 217, 217);
	else
		{
		if( GetFocus() == this )
			crBorder	= RGB(50, 109, 190);
		else
			crBorder	= RGB(172, 172, 172);

		if( !mDraw.Selected )
			crBorder	= RGB(30, 89, 170);
		}

	nBorderStyle	= BORDER_STYLE_RIGHT;*/
	return true;
	}

//
//	[virtual] ODBCGridView::GetCellText
//
bool
ODBCGridView::OnGetCellText(IN int Row, IN int Col, CString& strCellText, IN LONG lRowParam)
	{/*
	ODBCGridCol*	pCol		= m_pColByIndex[Col];
	m_csection.Lock();

	AutoSortedArray* pArr		= m_pArrRecDataById;
	if( m_pArrRecDataShow )
		pArr = m_pArrRecDataShow;

	void*			lpRecData	= (void*)pArr->GetData(Row);
	m_csection.Unlock();

	if( lpRecData ){
		void*		lpRec		= (void*)m_pRecord->GetRecordData();
		m_pRecord->SetRecordData(lpRecData);
		if( pCol->pGetAsTextFunc ){
			GetFieldAsText pFunc = (GetFieldAsText)pCol->pGetAsTextFunc;
			(*pFunc)(pCol, strCellText, m_pRecord);
			}
		else
		if( pCol->bRowIndex )
			strCellText.Format("%d", Row+1);
		m_pRecord->SetRecordData(lpRec);
		return true;
		}*/
	return false;
	}

//
//	[virtual] ODBCGridView::OnGetCellTextAlign
//
bool
ODBCGridView::OnGetCellTextAlign(IN int Row, IN int Col, OUT int& nAlign)
	{/*
	nAlign = DT_SINGLELINE|m_pColByIndex[Col]->nAlign;*/
	return true;
	}

//
//	ODBCGridView::OnGetCellColors
//
bool 
ODBCGridView::OnGetCellColors(	IN  int Row,
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
/*
	if( mDraw.Type == CELL_SEPARATOR )
		return false;

	bool	bSel	= mDraw.Selected;
	BOOL	bCheck	= FALSE;

	if( m_bUseFirstColAsSelState && 
		m_pColByIndex[0]->bRowIndex		== TRUE && 
		m_pColByIndex[0]->nCheckFieldId	> -1 )
		{
		AutoSortedArray* pArr		= m_pArrRecDataById;
		if( m_pArrRecDataShow )
			pArr = m_pArrRecDataShow;
		
		void*	lpRecDataOld	= m_pRecord->GetRecordData();
		void*	lpRecData		= (void*)pArr->GetData(Row);
		m_pRecord->SetRecordData(lpRecData);
		bCheck		= (BOOL)m_pRecord->GetLong(m_pColByIndex[0]->nCheckFieldId);
		if( bCheck != FALSE  )
			bSel	= true;
		m_pRecord->SetRecordData(lpRecDataOld);
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
			if( GetFocus() == this ){
				BkColor		= RGB(61, 128, 223);
				TextColor	= RGB(255, 255, 255);
				BottomLine	= RGB(125, 170, 234);
				}
			else{
				BkColor		= RGB(192, 192, 192);
				TextColor	= RGB(0, 0, 0);
				BottomLine	= RGB(224, 224, 224);
				}
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
	return (TextStyle != 0);*/
		return false;
	}

//
//	[protected] ODBCGridView::FreeRecordDataArray
//
void
ODBCGridView::FreeRecordDataArray()
	{
	m_csection.Lock();
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
	m_csection.Unlock();
	}

//
//	[static protected] ODBCGridView::GetBool_FieldAsText
//
/*
void
ODBCGridView::GetBool_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
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
*/
//
//	[static protected] ODBCGridView::GetInt_FieldAsText
//
/*
void		
ODBCGridView::GetInt_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
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
*/
//
//	[static protected] ODBCGridView::GetDouble_FieldAsText
//
/*
void
ODBCGridView::GetDouble_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
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
*/
//
//	[static protected] ODBCGridView::GetDateTime_FieldAsText
//
/*
void
ODBCGridView::GetDateTime_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
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
*/
//
//	[static protected] ODBCGridView::GetDate_FieldAsText
//
/*
void		
ODBCGridView::GetDate_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
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
*/
//
//	[static protected] ODBCGridView::GetString_FieldAsText
//
/*
void
ODBCGridView::GetString_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	pRec->GetString(pDBCol->nFieldID, sFieldText);
	}
*/
//
//	[static protected] ODBCGridView::GetDecimal_FieldAsText
//
/*
void		
ODBCGridView::GetDecimal_FieldAsText(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec)
	{
	pRec->GetString(pDBCol->nFieldID, sFieldText);
	if( sFieldText[0] == '.' )
		sFieldText.Insert(0, '0');

	if( pDBCol->nScaleShow > -1 && 
		pDBCol->nScaleShow < pDBCol->nScale )
		{
		int	nDelCt		= pDBCol->nScale - pDBCol->nScaleShow;
		int nStartDel	= sFieldText.GetLength() - nDelCt;
		if( !pDBCol->nScaleShow )
			{
			nStartDel --; nDelCt ++;
			}
		if( nStartDel > -1 )
			sFieldText.Delete(nStartDel, nDelCt);
		}

	if( sFieldText.IsEmpty() || sFieldText == "0" )
		{
		CString	sFmt;
		sFmt.Format("0.%%0%dd", pDBCol->nScaleShow);
		sFieldText.Format(sFmt,0);
		}
	}
*/
//
//	[protected virtual] ODBCGridView::GetLastUIDofTable
//
void	
ODBCGridView::GetLastUIDofTable(long& lUID)
	{
	lUID = 0;
	}

//
//	[protected virtual] ODBCGridView::RowChanged
//
void
ODBCGridView::RowChanged()
	{
	}

//
//	[protected virtual] ODBCGridView::SortList
//
bool	
ODBCGridView::SortList(int nCol, bool bAscending)
	{
	return true;
	}

//
//	[protected virtual] ODBCGridView::PreCreateWindow
//
BOOL 
ODBCGridView::PreCreateWindow(CREATESTRUCT& cs)
	{
	cs.style |= LVS_REPORT|LVS_SHOWSELALWAYS|LVS_OWNERDATA;
	return GridView::PreCreateWindow(cs);
	}

//
//	[afx_msg] ODBCGridView::OnCreate
//
int
ODBCGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
	BOOL bRet = GridView::OnCreate(lpCreateStruct);
	if( !IsWindow(m_headerCtrl) )
		{
		CListCtrl&		list		= GetListCtrl();
		CHeaderCtrl*	pHeaderCtrl = list.GetHeaderCtrl();
		if( pHeaderCtrl )
			{
			// Subclass header control. ##############
			if( m_headerCtrl.SubclassWindow(pHeaderCtrl->GetSafeHwnd()) )
				m_headerCtrl.m_pGridView = this;
			// #######################################
			}
		}
	return bRet;
	}

//
//	[protected virtual] ODBCGridView::PreSubclassWindow
//
void
ODBCGridView::PreSubclassWindow(void)
	{
	GridView::PreSubclassWindow();

	CListCtrl&		list		= GetListCtrl();
	CHeaderCtrl*	pHeaderCtrl = list.GetHeaderCtrl();
	if( pHeaderCtrl && !IsWindow(m_headerCtrl) )
		{
		// Subclass header control. ##############
		if( m_headerCtrl.SubclassWindow(pHeaderCtrl->GetSafeHwnd()) )
			m_headerCtrl.m_pGridView = this;
		// #######################################
		}
	}

//
//	[protected handler] ODBCGridView::OnTimer
//
void	
ODBCGridView::OnTimer(UINT_PTR nIDEvent)
	{/*
	if( nIDEvent == TIMER_ROW_CHANGE )
		{
		KillTimer(TIMER_ROW_CHANGE);
		m_csection.Lock();

		AutoSortedArray* pArr		= m_pArrRecDataById;
		if( m_pArrRecDataShow )
			pArr = m_pArrRecDataShow;

		int				nCurrRow	= mRow;
		int				nRecCount	= pArr->GetCount();
		if( nRecCount > 0 && nRecCount > nCurrRow )
			{
			ASSERT( nRecCount > nCurrRow );
			void*			lpRecData	= (void*)pArr->GetData(nCurrRow);
			ASSERT( lpRecData );
			m_pRecord->SetRecordData(lpRecData);
			}

		// Call rowchanged virtual. #######
		if( m_hWndOwner == NULL )
			RowChanged();
		else
			::SendMessage(m_hWndOwner, WM_ROW_CHANGED, nCurrRow, 0L);
		// #################################
		m_csection.Unlock();
		}*/
	GridView::OnTimer(nIDEvent);
	}

//
//	[protected handler] ODBCGridView::OnItemChanged
//
void
ODBCGridView::OnItemChanged(LPNMHDR pNMHDR, LRESULT* pResult)
	{
	NM_LISTVIEW*		pNMListView		= (NM_LISTVIEW*)pNMHDR;
	*pResult							= 0;
	_AFX_THREAD_STATE*	pThreadState	= AfxGetThreadState();

	// Check non item click. #####################
	CPoint	pt	= pThreadState->m_msgCur.pt;
	ScreenToClient(&pt);
	CRect	rcClient;
	GetClientRect(rcClient);

	int		nRow, nCol;
	if( rcClient.PtInRect(pt) && 
		!CellHitTest(pt, nRow, nCol, NULL) )
		{
		SelectItem(mRow);
		pNMListView->iItem = mRow;
		}
	// ##########################################

	// Retrieve current mouse message. ##########
	if( pThreadState->m_msgCur.message == WM_RBUTTONDOWN )
		{
		mRow = pNMListView->iItem;
		}
	// ##########################################

	if( m_bWaitRowChange )
		{
		KillTimer	(TIMER_ROW_CHANGE);
		SetTimer	(TIMER_ROW_CHANGE, 100, NULL);
		}
	else
		OnTimer(TIMER_ROW_CHANGE);
	}

//
//	[protected handler] ODBCGridView::OnItemDelete
//
void	
ODBCGridView::OnItemDelete(LPNMHDR pNMHDR, LRESULT* pResult)
	{/*
	NM_LISTVIEW*	pNMListView		= (NM_LISTVIEW*)pNMHDR;
	*pResult						= 0;
	if( !m_pArrRecDataById->GetCount() ) return;
	int				nDelIndex		= pNMListView->iItem;

	// Determine selected item. ########################
	int				nRowCount		= GetRowCount() - 1;
	int				nSelItem		= -1;
	if( nDelIndex == nRowCount )
		{
		if( nRowCount != 0 )
			nSelItem = nRowCount - 1;
		}
	else
		nSelItem = nDelIndex;
	// #################################################

	// Select item. ####################################
	BOOL bCallRowChange = FALSE;
	if( nSelItem != -1 )
		{
		int nInc	= 0;
		if( nSelItem == nDelIndex )
			{
			nSelItem ++;
			nInc	= 1;
			}

		SelectItem(nSelItem);
		RedrawItem(nSelItem);
		mRow	-= nInc;
		}
	else
		{
		bCallRowChange = TRUE;
		}
	// #################################################

	m_csection.Lock();
	// Delete record data. #############################
	void*			lpRecDataDel	= (void*)m_pArrRecDataById->GetData(nDelIndex);
	if( lpRecDataDel )
		delete lpRecDataDel;
	m_pArrRecDataById->Delete(nDelIndex);
	if( !m_pArrRecDataById->GetCount() )
		m_pRecord->SetRecordData(NULL);
	// #################################################
	m_csection.Unlock();

	// Call RowChanged function. #######################
	if( bCallRowChange )
		OnTimer(TIMER_ROW_CHANGE);
	// #################################################
	*/
	}

//
//	[protected handler] ODBCGridView::OnMouseActivate
//
int		
ODBCGridView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
	{
	if( message == WM_LBUTTONDOWN || 
		message == WM_RBUTTONDOWN ||
		message == WM_MBUTTONDOWN )
		{
		RECT	rect;
		int		nCol, nRow;

		CPoint	point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		if( CellHitTest(point, nRow, nCol, &rect) )
			{
			m_bIsRowSelected = TRUE;
			if( mRow == nRow )
				{
				if( m_bWaitRowChange )
					{
					KillTimer	(TIMER_ROW_CHANGE);
					SetTimer	(TIMER_ROW_CHANGE, 100, NULL);
					}
				else
					OnTimer(TIMER_ROW_CHANGE);
				}
			}
		else
			m_bIsRowSelected = FALSE;
		}

	if( pDesktopWnd->IsKindOf(RUNTIME_CLASS(CDialog)) )
		return MA_ACTIVATE;

	return GridView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

//
//	[public] ODBCGridView::GetCurRow
//
int
ODBCGridView::GetCurRow()
	{
	m_csection.Lock();
	if( m_pArrRecDataById && m_pArrRecDataById->GetCount() > 0)
		{
		m_csection.Unlock();
		return mRow;
		}
	m_csection.Unlock();
	return -1;
	}

//
//	[public] ODBCGridView::GetSelectedCount
//
int
ODBCGridView::GetSelectedCount()
	{
	CListCtrl&	list		= GetListCtrl();
	int			nSelCount	= list.GetSelectedCount();
	return nSelCount;
	}

//
//	[public] ODBCGridView::GetRowCount
//
int
ODBCGridView::GetRowCount()
	{
	m_csection.Lock();
	if( m_pArrRecDataById )
		{
		int nCount = m_pArrRecDataById->GetCount();
		m_csection.Unlock();
		return nCount;
		}
	else
		{
		m_csection.Unlock();
		CListCtrl& list = GetListCtrl();
		return list.GetItemCount();
		}
	m_csection.Unlock();
	}

//
//	[public] ODBCGridView::SelectItem
//
void
ODBCGridView::SelectItem(int nItem, BOOL bDeselect /*= TRUE*/)
	{/*
	if( bDeselect )
		{
		mRow = nItem;
		DeselectAll();
		if( GetListCtrl().SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED) )
			GetListCtrl().EnsureVisible(nItem, FALSE);
		}
	else
		{
		mRow = nItem;
		if( GetListCtrl().SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED) )
			GetListCtrl().EnsureVisible(nItem, FALSE);
		}

	m_csection.Lock();

	AutoSortedArray* pArr = m_pArrRecDataById;
	if( m_pArrRecDataShow )
		pArr = m_pArrRecDataShow;

	if( pArr->GetCount() > nItem )
		{
		void* lpRecData = (void*)pArr->GetData(mRow);
		ASSERT( lpRecData );
		m_pRecord->SetRecordData(lpRecData);
		}
	m_csection.Unlock();*/
	}

//
//	[public] ODBCGridView::Attach
//
BOOL			
ODBCGridView::Attach(HWND hWndAttachTo)
	{
	if( !SubclassWindow(hWndAttachTo) )
		return FALSE;

	SetWindowLong	(m_hWnd, GWL_STYLE, WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_OWNERDATA);
	OnInitialUpdate	();
	return TRUE;
	}

//
//	[protected handler] ODBCGridView::OnShowWindow
//
void	
ODBCGridView::OnShowWindow(BOOL bShow, UINT nStatus)
	{
	GridView::OnShowWindow(bShow, nStatus);
	}

//
//	[private] ODBCGridView::ReInitializeHeader
//
void
ODBCGridView::ReInitializeHeader()
	{
	// No sort column. ###########
	m_nSortCol	= -1;
	m_nSortFlag = SORT_FLAG_NOT;
	// ###########################

	CHeaderCtrl* pHdrCtrl = NULL;
	if( IsWindow(m_headerCtrl) )
		{
		pHdrCtrl = &m_headerCtrl;
		}
	else
		{
		CListCtrl&	list	= GetListCtrl();
		pHdrCtrl			= list.GetHeaderCtrl();
		}

	if( !pHdrCtrl || m_nNumberCols <= 0 )
		return;

	if( !m_pCols )
		return;

	char			szBuffer[256];
	int				nItemCount		= pHdrCtrl->GetItemCount();
	int				nLoop			= 0;
	ODBCGridCol*	pCol			= m_pCols;

	HDITEM headerItem;
	headerItem.pszText				= szBuffer;
	headerItem.cchTextMax			= 255;
	headerItem.mask					= HDI_TEXT;

	while( nLoop < nItemCount )
		{
		if( !pCol )
			break;

		strcpy_s(szBuffer, pCol->sTitle);
		pHdrCtrl->SetItem(nLoop, &headerItem); 
		nLoop ++; pCol = pCol->pNextCol;
		}
	}

CString 
ODBCGridView::GetSelectedKeyList( LPCTSTR delimiter )
	{
	CString		keylist;
	BOOL		first		= TRUE;
	int			row			= -1;
	CListCtrl&	listCtrl	= GetListCtrl();

	POSITION _position = listCtrl.GetFirstSelectedItemPosition();
	if( _position != NULL )
		row = listCtrl.GetNextSelectedItem(_position);

	while( row != -1 )
		{
		if( first )
			{
			keylist = _T(delimiter) + GetRowId(row) + _T(delimiter);
			first	= FALSE;
			}
		else
			{
			keylist += _T(",");
			keylist += _T(delimiter) + GetRowId(row) + _T(delimiter);
			}

		if( _position != NULL )
			row  = listCtrl.GetNextSelectedItem(_position);
		else
			row = -1;
		}
	return keylist;
	}

//
//	[public] ODBCGridView::GetCheckedKeyList
//

CString
ODBCGridView::GetCheckedKeyList(LPCTSTR delimiter)
	{
	CString			keylist;
	BOOL			first		= TRUE;
	AutoSortedArray arrIds;

	if( !GetCheckedArray(0, arrIds, TRUE) )
		return "";

	int				nLoop = 0;
	CString			sRowId;
	while( nLoop < arrIds.GetCount() )
		{
		sRowId.Format("%d", arrIds[nLoop]);
		if( first )
			{
			keylist = _T(delimiter) + sRowId + _T(delimiter);
			first	= FALSE;
			}
		else
			{
			keylist += _T(",");
			keylist += _T(delimiter) + sRowId + _T(delimiter);
			}

		nLoop ++;
		}

	return keylist;
	}

//
//	[virtual] ODBCGridView::OnGetCellType
//
CellType 
ODBCGridView::OnGetCellType(IN int Row, IN int Col)
	{/*
	if( m_pColByIndex[Col]->bCheckBox		&& 
		m_pColByIndex[Col]->nCheckFieldId > -1 )
		{
		if( !m_pRecord )
			m_pRecord = m_pRstSelect->GetRecordPtr();

		AutoSortedArray* pArr = m_pArrRecDataById;
		if( m_pArrRecDataShow )
			pArr = m_pArrRecDataShow;
		
		m_csection.Lock();
		void*	lpRecData	= (void*)pArr->GetData(Row);
		void*	lpDataOld	= m_pRecord->GetRecordData();
		m_pRecord->SetRecordData(lpRecData);
		BOOL bCheck = lpRecData ? m_pRecord->GetLong(m_pColByIndex[Col]->nCheckFieldId) : FALSE;
		m_pRecord->SetRecordData(lpDataOld);
		m_csection.Unlock();
		return bCheck ? CELL_CHECKBOXON : CELL_CHECKBOXOFF;
		}
	*/
	return CELL_REGULAR;
	}

//
//	[virtual] ODBCGridView::OnCheckItem
//
/*
void
ODBCGridView::OnCheckItem(CRecord* pCurrRecord, int nCol, int nRow, BOOL& bAllow, BOOL bCurrentState)
	{
	bAllow = TRUE;
	}*/

//
//	[virtual] ODBCGridView::OnCellRBtnDown
//
void	
ODBCGridView::OnCellRBtnDown(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point)
	{
	if( m_pColByIndex[Col]->bCheckBox		&& 
		m_pColByIndex[Col]->nCheckFieldId > -1 ) // CheckBox column.
		{/*
		CPoint pt			= Point;
		CRect rcCheckBox	= CellRect;
		rcCheckBox.right	= rcCheckBox.left + CHECK_MARKWIDTH;

		if( rcCheckBox.PtInRect(pt) )
			{
			// Show menu. ########################
			ClientToScreen	(&pt);
			if( ::TrackPopupMenu(m_menuCheck, TPM_LEFTALIGN, pt.x, pt.y, 0L, m_hWnd, NULL) )
				{
				m_nColCheck	= Col;
				return;
				}
			// ###################################
			}*/
		}
	else
		{
		}
	
//	GridView::OnCellRBtnUp(Row, Col, CellRect, Point);
	}

//
//	[virtual] ODBCGridView::OnCellLBtnDown
//
bool
ODBCGridView::OnCellLBtnDown(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point)
	{/*
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

			AutoSortedArray* pArr = m_pArrRecDataById;
			if( m_pArrRecDataShow )
				pArr = m_pArrRecDataShow;

			m_csection.Lock();
			int		nCheckFld	= m_pColByIndex[Col]->nCheckFieldId;
			void*	lpRecData	= (void*)pArr->GetData(Row);
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
		}*/

	return GridView::OnCellLBtnDown(Row, Col, CellRect, Point);
	}

LRESULT	
ODBCGridView::OnAddRecords(WPARAM wParam, LPARAM lParam)
	{
	if( wParam == 0 || lParam == 0 )
		return FALSE;

	CDWordArray*	pIds		= (CDWordArray*)wParam;
	CDWordArray*	pRecDatas	= (CDWordArray*)lParam;
	int				nCount		= (int)min(pIds->GetCount(), pRecDatas->GetCount());
	if( nCount <= 0 )
		return FALSE;

	m_csection.Lock	();
	SetRedraw		(FALSE);

	int				nLoop		= 0;
	while( nLoop < nCount )
		{
		m_pArrRecDataById->Add(pIds->GetAt(nLoop), pRecDatas->GetAt(nLoop));
		nLoop ++;
		}

	// Set virtual item count. ############################
	CListCtrl&	listCtrl	= GetListCtrl();
	listCtrl.SetItemCount(m_pArrRecDataById->GetCount());
	// ####################################################

	m_csection.Unlock	();
	SetRedraw			(TRUE);
	return TRUE;
	}

//
//	[afx_msg] ODBCGridView::OnCheckAll
//
void
ODBCGridView::OnCheckAll()
	{
	BOOL bRedraw = FALSE;
	CheckAll(m_nColCheck, TRUE, bRedraw);
	if( bRedraw )
		Invalidate(FALSE);
	}

//
//	[afx_msg] ODBCGridView::OnUnCheckAll
//
void
ODBCGridView::OnUnCheckAll()
	{
	BOOL bRedraw = FALSE;
	CheckAll(m_nColCheck, FALSE, bRedraw);
	if( bRedraw )
		Invalidate(FALSE);
	}

//
//	[protected] ODBCGridView::CheckAll
//
void
ODBCGridView::CheckAll(int nCol, BOOL bCheck, BOOL& bRedraw)
	{
	bRedraw = FALSE;
	if( nCol <= -1 )
		return;

	m_csection.Lock();/*
	if( m_pColByIndex[nCol]->bCheckBox &&  
		m_pColByIndex[nCol]->nCheckFieldId > -1 )
		{
		int		nChanged	= 0;
		void*	lpDataOld	= m_pRecord->GetRecordData();
		void*	lpRecData	= NULL;
		int		nCheckField	= m_pColByIndex[nCol]->nCheckFieldId;

		AutoSortedArray* pArr = m_pArrRecDataById;
		if( m_pArrRecDataShow )
			pArr = m_pArrRecDataShow;

		for( int nLoop=0; nLoop<pArr->GetCount(); nLoop ++ )
			{
			lpRecData		= (void*)pArr->GetData(nLoop);
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
		}*/
	m_csection.Unlock();
	}

//
//	[public] ODBCGridView::GetCheckBitmaps
//

void
ODBCGridView::GetCheckBitmaps(HBITMAP& pbmpCheck, HBITMAP& pbmpUnCheck)
	{
	if( pbmpCheck )
		DeleteObject(pbmpCheck);

	if( pbmpUnCheck )
		DeleteObject(pbmpUnCheck);

	// Check on bitmap. ###################
	BITMAP bmp;
	GetObject(mDraw.CheckOnBmp, sizeof(BITMAP), &bmp);

	CDC		memDC;
	memDC.CreateCompatibleDC(NULL);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&memDC, bmp.bmWidth, bmp.bmHeight);
	
	memDC.SelectObject	(&bitmap);
	memDC.BitBlt		(0, 0, bmp.bmWidth, bmp.bmHeight, CDC::FromHandle(mDraw.CheckOnDC), 0, 0, SRCCOPY);		
	pbmpCheck			= bitmap;
	bitmap.m_hObject	= NULL;
	memDC.DeleteDC();
	// #####################################

	// Check off bitmap. ###################
	memDC.CreateCompatibleDC(NULL);
	GetObject(mDraw.CheckOffBmp, sizeof(BITMAP), &bmp);
	bitmap.CreateCompatibleBitmap(&memDC, bmp.bmWidth, bmp.bmHeight);
	
	memDC.SelectObject	(&bitmap);
	memDC.BitBlt		(0, 0, bmp.bmWidth, bmp.bmHeight, CDC::FromHandle(mDraw.CheckOffDC), 0, 0, SRCCOPY);		
	pbmpUnCheck			= bitmap;
	bitmap.m_hObject	= NULL;
	// #####################################
	}

//
//	[virtual] ODBCGridView::OnSysColorChange
//
void 
ODBCGridView::OnSysColorChange()
	{
	GridView::OnSysColorChange();
	GetCheckBitmaps(m_headerCtrl.m_hBmpCheckOn, m_headerCtrl.m_hBmpCheckOff);	
	}

//////////////////////////////////////////////////////////
//
//	Compare functions.
//
//////////////////////////////////////////////////////////

//
//	[static] ODBCGridView::CompareDummy
//
int		
ODBCGridView::CompareDummy(void* lpValue1, void* lpValue2)
	{
	return -1; // Add to last
	}

//
//	[static] ODBCGridView::CompareTextField
//

int
ODBCGridView::CompareTextField(void* lpValue1, void* lpValue2)
	{
	return -1;
//	return CompareStrings_Geo((LPCTSTR)lpValue1, (LPCTSTR)lpValue2);
	}

//
//	[static] ODBCGridView::CompareDecimalField
//

int
ODBCGridView::CompareDecimalField(void* lpValue1, void* lpValue2)
	{
	ASSERT( lpValue1 && lpValue2 );
	double d1 = atof((LPCSTR)lpValue1);
	double d2 = atof((LPCSTR)lpValue2);
	return d1 > d2 ? 1 : (d1 == d2 ? 0 : -1);
	}

//
//	[static] ODBCGridView::CompareDateField
//
int
ODBCGridView::CompareDateField(void* lpValue1, void* lpValue2)
	{
	return 0;
	}

//
//	[static] ODBCGridView::CompareLongField
//
int
ODBCGridView::CompareLongField(void* lpValue1, void* lpValue2)
	{
	ASSERT( lpValue1 && lpValue2 );
	return (*(long*)lpValue1) > (*(long*)lpValue2) ? 1 : ((*(long*)lpValue1) == (*(long*)lpValue2) ? 0 : -1);
	}

//
//	[static] ODBCGridView::CompareDoubleField
//
int
ODBCGridView::CompareDoubleField(void* lpValue1, void* lpValue2)
	{
	ASSERT( lpValue1 && lpValue2 );
	return (*(double*)lpValue1) > (*(double*)lpValue2) ? 1 : ((*(double*)lpValue1) == (*(double*)lpValue2) ? 0 : -1);
	}

//
//	[static] ODBCGridView::CompareTextField_Desc
//

int
ODBCGridView::CompareTextField_Desc(void* lpValue1, void* lpValue2)
	{
	return -1;
//	return CompareStrings_Geo((LPCTSTR)lpValue2, (LPCTSTR)lpValue1);
	}

//
//	[static] ODBCGridView::CompareDecimalField_Desc
//

int
ODBCGridView::CompareDecimalField_Desc(void* lpValue1, void* lpValue2)
	{
	ASSERT( lpValue1 && lpValue2 );
	double d1 = atof((LPCSTR)lpValue1);
	double d2 = atof((LPCSTR)lpValue2);
	return d1 > d2 ? -1 : (d1 == d2 ? 0 : 1);
	}

//
//	[static] ODBCGridView::CompareDateField_Desc
//
int
ODBCGridView::CompareDateField_Desc(void* lpValue1, void* lpValue2)
	{
	return 0;
	}

//
//	[static] ODBCGridView::CompareLongField_Desc
//
int
ODBCGridView::CompareLongField_Desc(void* lpValue1, void* lpValue2)
	{
	ASSERT( lpValue1 && lpValue2 );
	return (*(long*)lpValue1) > (*(long*)lpValue2) ? -1 : ((*(long*)lpValue1) == (*(long*)lpValue2) ? 0 : 1);
	}

//
//	[static] ODBCGridView::CompareDoubleField_Desc
//
int
ODBCGridView::CompareDoubleField_Desc(void* lpValue1, void* lpValue2)
	{
	ASSERT( lpValue1 && lpValue2 );
	return (*(double*)lpValue1) > (*(double*)lpValue2) ? -1 : ((*(double*)lpValue1) == (*(double*)lpValue2) ? 0 : 1);
	}

//
//	[virtual] ODBCGridView::SortColumn
//

BOOL
ODBCGridView::SortColumn(int nCol, BOOL bAsc)
	{/*
	if( !m_pRecord || m_pArrRecDataById->GetCount() == 0 )
		return FALSE;

	AutoSortedArray arrNewOrder;
	arrNewOrder.OptimizeForLargeData(TRUE);
	AutoSortedArray* pArrShow = new AutoSortedArray();
	pArrShow->OptimizeForLargeData(TRUE);
	pArrShow->SetCompareFunction(CompareDummy);

	long pProc = (long)m_pColByIndex[nCol]->pGetAsTextFunc;
	if( pProc ==((long)GetBool_FieldAsText) )
		arrNewOrder.SetCompareFunction(bAsc ? CompareLongField : CompareLongField_Desc);
	else
	if( pProc ==((long)GetString_FieldAsText) )
		arrNewOrder.SetCompareFunction(bAsc ? CompareTextField : CompareTextField_Desc);
	else
	if( pProc ==((long)GetInt_FieldAsText) )
		arrNewOrder.SetCompareFunction(bAsc ? CompareLongField : CompareLongField_Desc);
	else
	if( pProc ==((long)GetDouble_FieldAsText) )
		arrNewOrder.SetCompareFunction(bAsc ? CompareDoubleField : CompareDoubleField_Desc);
	else
	if( pProc ==((long)GetDate_FieldAsText) )
		arrNewOrder.SetCompareFunction(bAsc ? CompareDateField : CompareDateField_Desc);
	else
	if( pProc ==((long)GetDecimal_FieldAsText) )
		arrNewOrder.SetCompareFunction(bAsc ? CompareDecimalField : CompareDecimalField_Desc);
	else
	if( !m_pColByIndex[nCol]->bRowIndex )
		return FALSE;

	AutoSortedArray* pArr = m_pArrRecDataById;
	if( m_pArrRecDataShow )
		pArr = m_pArrRecDataShow;

	int		nLoop		= 0;
	int		nCount		= pArr->GetCount();
	int		nSortField	= m_pColByIndex[nCol]->nFieldID;
	BOOL	bRowIndex	= m_pColByIndex[nCol]->bRowIndex;
	int		nCheckField	= m_pColByIndex[nCol]->nCheckFieldId;
	int		nIndex		= GetCurRow();
	long	lIdSel		= 0L;

	if( nIndex > -1 )
		{
		m_pRecord->SetRecordData((void*)pArr->GetData(nIndex));
		lIdSel			= m_pRecord->GetLong(m_nUIDField);
		}
	
	if( bRowIndex )
		{
		if( nCheckField > -1 )
			{
			arrNewOrder.SetCompareFunction(bAsc ? CompareLongField : CompareLongField_Desc);
			nSortField = nCheckField;
			}
		else
			return FALSE;
		}

	if( nSortField == -1 || !m_pColByIndex[nCol]->bAllowSort )
		return FALSE;

	AfxGetApp()->BeginWaitCursor();
	// Allocate array size previously. ####
	arrNewOrder	.SetSize(nCount);
	pArrShow->SetSize(nCount);
	// ####################################
	m_csection.Lock();

	// Sort loop. #########################
	while( nLoop < nCount ){
		void* lpRecData		=  (void*)pArr->GetData(nLoop);
		ASSERT( lpRecData );
		m_pRecord->SetRecordData(lpRecData);
		void* lpValuePtr	= m_pRecord->GetBindedValuePtr(nSortField);
		ASSERT( lpValuePtr );

		arrNewOrder	.Add	((long)lpValuePtr, (long)lpRecData);
		nLoop ++;
		}
	// ####################################
	
	// Fill main array. ###################
	nLoop = 0;
	while( nLoop < nCount )
		{

//		wchar_t *wszValue1 = NULL;
//		CString s = (LPCTSTR)arrNewOrder[nLoop];
//		PluginGlobal::GeoAnsiToUnicode(s, wszValue1);
//		delete [] wszValue1;

		void*	lpRecData	= (void*)arrNewOrder.GetData(nLoop);
		ASSERT( lpRecData );
		m_pRecord->SetRecordData(lpRecData);
		long	lUid		= m_pRecord->GetLong(m_nUIDField);
		pArrShow->Add(lUid, (long)lpRecData);
		nLoop				++;
		}
	// ####################################	

	// Change show array. #####################
	if( m_pArrRecDataShow )
		delete m_pArrRecDataShow;
	m_pArrRecDataShow = pArrShow;
	// ########################################

	SetRedraw(FALSE);
	// Find and select recently selected item #
	if( lIdSel > 0 )
		{
		CString sId;
		sId.Format("%d", lIdSel);
		FindKeyMoveTo(sId);
		}
	// ########################################
	
	m_csection.Unlock();
	AfxGetApp()->EndWaitCursor();
	SetRedraw	(TRUE);
	Invalidate	(FALSE);*/
	return TRUE;
	}

//
//	[public] ODBCGridView::GetCheckedArray
//

BOOL
ODBCGridView::GetCheckedArray(int nCol, AutoSortedArray& arrIdsChecked, BOOL bChecked /*= TRUE*/)
	{/*
	CListCtrl&		list	= GetListCtrl();
	CHeaderCtrl*	pHeader	= list.GetHeaderCtrl();

	if( !pHeader || nCol < 0 || nCol >= pHeader->GetItemCount() || 
		!m_pColByIndex[nCol] || !m_pColByIndex[nCol]->bCheckBox )
		return FALSE;

	int	nCheckField	= m_pColByIndex[nCol]->nCheckFieldId;
	if( nCheckField < 0 )
		return FALSE;

	AutoSortedArray* pArr = m_pArrRecDataById;
	if( m_pArrRecDataShow )
		pArr = m_pArrRecDataShow;

	int nLoop	= 0;
	int nCount	= pArr->GetCount();

	m_csection.Lock();

	while( nLoop < nCount )
		{
		void* lpRecData = (void*)pArr->GetData(nLoop);
		ASSERT( lpRecData );
		m_pRecord->SetRecordData(lpRecData);

		if( m_pRecord->GetLong(nCheckField) == bChecked )
			arrIdsChecked.Add(m_pRecord->GetLong(m_nUIDField));
		nLoop ++;
		}

	m_csection.Unlock();*/
	return TRUE;
	}

//
//	[public] ODBCGridView::SetMenuId
//

BOOL
ODBCGridView::SetMenuId(UINT nIDMenu, UINT nPopupIndex /*= 1*/)
	{/*
	CMenu menu;
	if( !XTPResourceManager()->LoadMenu(&menu, nIDMenu) )
		return FALSE;

	CMenu* pPopup = menu.GetSubMenu(nPopupIndex);
	if( !pPopup )
		return FALSE;

	if( IsMenu(m_menuPopup.m_hMenu) )
		m_menuPopup.Detach();
	if( IsMenu(m_hMenuMain) )
		DestroyMenu(m_hMenuMain);
	
	m_menuPopup.Attach(pPopup->Detach());
	m_hMenuMain = menu.Detach();*/
	return TRUE;
	}

//
//	[afx_msg] ODBCGridView::OnContextMenu
//

void
ODBCGridView::OnContextMenu(CWnd* pWnd, CPoint pos)
	{/*
	if( pWnd == this && ::IsMenu(m_menuPopup.m_hMenu) )
		{
		((CXTPMDIFrameWnd*)AfxGetMainWnd())->GetCommandBars()->
		TrackPopupMenuEx(&m_menuPopup, TPM_RIGHTBUTTON, pos.x, pos.y);
		}
	GridView::OnContextMenu(pWnd, pos);*/
	}

//
//	[public] ODBCGridView::EmptyList
//

BOOL
ODBCGridView::EmptyList()
	{/*
	GetListCtrl().SetItemCount(0);

	// Save grid info. ##########
	RegSaveGridInfo();
	m_bGridInfoLoaded = FALSE;
	// ##########################

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
	m_pCols = NULL;
	// ##########################

	// Free record data array. ###################
	FreeRecordDataArray();
	m_csection.Lock();
	m_pArrRecDataById->DeleteAll();
	m_pArrRecDataById->FreeExtra();
	m_csection.Unlock();
	// ###########################################

	// Remove grid control's HWND to array of grid HWNDs.
	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT( pMainWnd && pMainWnd->m_hWnd );
	
	AutoSortedArray* pArrHWNDs = (AutoSortedArray*)GetProp(pMainWnd->m_hWnd, _T("GridHWNDs"));
	if( pArrHWNDs != NULL )
		{
		DWORD	dwTable = DecodeString(m_strTableName.GetBuffer());
		int		nIndex	= pArrHWNDs->FindFirst((long)dwTable);

		while( nIndex != -1 )
			{
			if( pArrHWNDs->GetData(nIndex) == (DWORD)m_hWnd )
				{
				pArrHWNDs->Delete(nIndex);
				// Remove property value if array is empty. ############
				if( !pArrHWNDs->GetCount() )
					{
					delete pArrHWNDs;
					RemoveProp(pMainWnd->m_hWnd, _T("GridHWNDs"));

					// Remove hook. ####################################
					memmove((void*)m_funcToHookExecDir, &m_jmpRealExecDir[0], 5);
					// #################################################

//					RemoveProp(pMainWnd->m_hWnd, _T("realJumpCode"));
//					RemoveProp(pMainWnd->m_hWnd, _T("realJumpOffset"));
					}
				// #####################################################
				break;
				}
			nIndex = pArrHWNDs->FindNext();
			}
		}
	// ########################################################

	m_pRstSelect		= NULL;
	m_pRecord			= NULL;
	m_pArrRecDataShow	= NULL;
	m_pCols				= NULL;
	m_pColByIndex		= NULL;
	m_nNumberCols		= 0L;

	// Delete columns. #########################################
	int nColumnCount = GetListCtrl().GetHeaderCtrl()->GetItemCount();
	// Delete all of the columns.
	for (int i=0;i < nColumnCount;i++)
		GetListCtrl().DeleteColumn(0);
	// #########################################################
*/
	return TRUE;
	}

//
//	[public] ODBCGridView::GetItemText
//

CString
ODBCGridView::GetItemText(int nRow, int nCol)
	{
	CString sText;
	OnGetCellText(nRow, nCol, sText, 0);
	return sText;
	}

//
//	[public] ODBCGridView::ReloadCols
//

void
ODBCGridView::ReloadCols()
	{
/*	ASSERT( m_pRecord );

	CMarkup xmlGrid;
	if( !RegLoadGridInfo(xmlGrid) )
		return FALSE;

	CPtrArray	arrColPtrs;
	if( !RegLoadGridInfo(arrColPtrs) )
		return FALSE;
	
	CListCtrl&	list		= GetListCtrl();
	list.SetRedraw(FALSE);
	
	// Free col by index array.##
	if( m_pColByIndex )
		delete m_pColByIndex;
	// ##########################
	// Free cols array. #########
	FreePointersChain(m_pCols, (long)offsetof(ODBCGridCol, pNextCol));
	// ##########################
	// Delete columns. ##########
	int nColumnCount = GetListCtrl().GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nColumnCount;i++)
		GetListCtrl().DeleteColumn(0);
	// ##########################

	LVCOLUMN		column;
	m_nNumberCols				= (long)arrColPtrs.GetCount();
	int				nLoop		= 0;
	RecordField*	pFields		= m_pRecord->GetFieldInfosArray();
	int				nInvalid	= 0;

	while( nLoop < m_nNumberCols )
		{
		ODBCGridCol* pCol		= (ODBCGridCol*)arrColPtrs[nLoop];
		ASSERT( pCol );
		
		column.mask				= LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
		if( iImage > -1 )
			column.mask			|= LVCF_IMAGE;
		column.pszText			= pCol->sTitle;
		column.cchTextMax		= 255;
		column.fmt				= pCol->nAlign;
		column.cx				= pCol->nWidth;
		column.iImage			= pCol->iImage;
		int			nNewCol		= list.InsertColumn(nLoop, &column);

		pCol->bRowIndex			= pCol->sFieldName == "$ROW_INDEX$" ? TRUE : FALSE;
		pCol->bCheckBox			= (lpszChkField && strlen(lpszChkField) > 0) ? TRUE : FALSE;
		if( pCol->bRowIndex )
			pNewCol->bAllowSort	= FALSE;
		else
			pNewCol->bAllowSort	= TRUE;

		int	nFieldID			= m_pRecord->GetFieldID(pCol->sFieldName);
		if( nFieldID <= -1 )
			{
			delete		pCol;
			nInvalid	++;
			nLoop		++;
			continue;
			}

		pCol->nFieldID			= nFieldID;
		pCol->nCheckFieldId		= m_pRecord->GetFieldID(pCol->sCheckFieldName);
		pCol->pGetAsTextFunc	= NULL;
		pCol->nPrecision		= 0;
		pCol->nScale			= 0;
		pCol->nCheckHeader		= 0;

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
				//	case SQL_DATE:
				//	case SQL_TIME:
				//	case SQL_TIMESTAMP: 
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


		m_pCols					= (ODBCGridCol*)InsertToPointersChain((void*)m_pCols, (void*)pCol, nLoop - nInvalid, (long)offsetof(ODBCGridCol, pNextCol), -1);
		nLoop ++;
		}

	m_nNumberCols -= nInvalid;


	list.SetRedraw(TRUE);*/
//	return TRUE;

/*
	LVCOLUMN	column;
	column.mask				= LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	if( iImage > -1 )
		column.mask			|= LVCF_IMAGE;
	column.pszText			= (LPTSTR)lpszTitle;
	column.cchTextMax		= 255;
	column.fmt				= nAlignment;
	column.cx				= nWidth;
	column.iImage			= iImage;

	int			nNewCol		= list.InsertColumn(nIndex, &column);
	ASSERT( nWidth > 0 );

	ODBCGridCol* pNewCol	= new ODBCGridCol;
	pNewCol->sFieldName		= lpszDBField;
	pNewCol->sTitle			= lpszTitle;
	pNewCol->nWidth			= nWidth;
	pNewCol->pNextCol		= NULL;
	pNewCol->nFieldID		= -1;
	pNewCol->sCheckFieldName= lpszChkField ? lpszChkField : "";
	pNewCol->nCheckFieldId	= -1;
	pNewCol->nCheckHeader	= 0;
	pNewCol->pGetAsTextFunc	= NULL;
	pNewCol->iImage			= iImage;
	pNewCol->bRowIndex		= pNewCol->sFieldName == "$ROW_INDEX$" ? TRUE : FALSE;
	pNewCol->bCheckBox		= (lpszChkField && strlen(lpszChkField) > 0) ? TRUE : FALSE;
	pNewCol->bAllowSort		= TRUE;
	pNewCol->nScaleShow		= nScaleShow;

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
	*/
	}

//
//	[public] ODBCGridView::RegSaveGridInfo
//
BOOL
ODBCGridView::RegSaveGridInfo()
	{/*
	if( m_sGridName.IsEmpty() || !m_pColByIndex )
		return FALSE;

	CWinApp* pApp = AfxGetApp();
	if( !pApp )
		return FALSE;

	// Generate info XML. ####################
	int		nLoop = 0;
	CString sGridInfoXML;

	CListCtrl&		list		= GetListCtrl();
	CHeaderCtrl*	pHeader		= list.GetHeaderCtrl();

	sGridInfoXML += "<gridinfo>";
	while( nLoop < m_nNumberCols )
		{
		ODBCGridCol* pCol = m_pColByIndex[nLoop];
		if( pCol )
			{
			CString sFmt;
			sGridInfoXML += "<col ";
			sFmt.Format(" sFieldName='%s' ", pCol->sFieldName);sGridInfoXML += sFmt;
			sFmt.Format(" sTitle='%s' ", pCol->sTitle);sGridInfoXML += sFmt;
			sFmt.Format(" nFieldID='%d' ", pCol->nFieldID);sGridInfoXML += sFmt;

			int nAlignment			= LVCFMT_LEFT;
			if( nAlignment&DT_RIGHT )
				pCol->nAlign		= LVCFMT_RIGHT;
			else
			if( nAlignment&DT_CENTER )
				pCol->nAlign		= LVCFMT_CENTER;

			sFmt.Format(" nAlign='%d' ", nAlignment);sGridInfoXML += sFmt;

			if( pHeader )
				{
				CRect rcHdrItem;
				pHeader->GetItemRect(nLoop, rcHdrItem);
				sFmt.Format(" nWidth='%d' ", rcHdrItem.Width());sGridInfoXML += sFmt;
				}
			else
				sFmt.Format(" nWidth='%d' ", pCol->nWidth);sGridInfoXML += sFmt;

			sFmt.Format(" iImage='%d' ", pCol->iImage);sGridInfoXML += sFmt;
			sFmt.Format(" bRowIndex='%d' ", pCol->bRowIndex);sGridInfoXML += sFmt;
			sFmt.Format(" bCheckBox='%d' ", pCol->bCheckBox);sGridInfoXML += sFmt;
			sFmt.Format(" sCheckFieldName='%s' ", pCol->sCheckFieldName);sGridInfoXML += sFmt;
			sFmt.Format(" nCheckFieldId='%d' ", pCol->nCheckFieldId);sGridInfoXML += sFmt;
			sFmt.Format(" nCheckHeader='%d' ", pCol->nCheckHeader);sGridInfoXML += sFmt;
			sFmt.Format(" bAllowSort='%d' ", pCol->bAllowSort);sGridInfoXML += sFmt;
			sFmt.Format(" nPrecision='%d' ", pCol->nPrecision);sGridInfoXML += sFmt;
			sFmt.Format(" nScale='%d' ", pCol->nScale);sGridInfoXML += sFmt;
			sFmt.Format(" nScaleShow='%d' ", pCol->nScaleShow);sGridInfoXML += sFmt;
			sGridInfoXML += "/>";
			}
		nLoop ++;
		}
	sGridInfoXML += "</gridinfo>";
	// #####################################

	if( !pApp->WriteProfileStringA("ODBCGridView", m_sGridName.GetBuffer(), sGridInfoXML) )
		return FALSE;*/
	return TRUE;
	}

//
//	[public] ODBCGridView::RegLoadGridInfo
//

ODBCGridCol* 
ODBCGridView::RegLoadGridInfo(long& lColCount)
	{/*
	if( m_sGridName.IsEmpty() || !m_pRecord )
		return NULL;

	CWinApp* pApp = AfxGetApp();
	ASSERT( pApp );

	CString sGridInfoXML = pApp->GetProfileString("ODBCGridView", m_sGridName.GetBuffer(), "");
	if( sGridInfoXML.IsEmpty() )
		return NULL;

	CMarkup xml;
	xml.SetDoc(sGridInfoXML);
	if( !xml.FindElem("gridinfo") )
		return NULL;
	xml.IntoElem();

	ODBCGridCol*	pColFirst	= NULL;
	int				nLoop		= 0;
	int				nInvalid	= 0;

	while(xml.FindElem("col"))
		{
		ODBCGridCol* pCol		= new ODBCGridCol;
		pCol->sFieldName		= xml.GetAttrib("sFieldName");
		pCol->sTitle			= xml.GetAttrib("sTitle");
		pCol->nFieldID			= atoi(xml.GetAttrib("nFieldID"));
		pCol->nAlign			= atoi(xml.GetAttrib("nAlign"));
		pCol->nWidth			= atoi(xml.GetAttrib("nWidth"));
		pCol->iImage			= atoi(xml.GetAttrib("iImage"));
		pCol->bRowIndex			= atoi(xml.GetAttrib("bRowIndex"));
		pCol->bCheckBox			= atoi(xml.GetAttrib("bCheckBox"));
		pCol->sCheckFieldName	= xml.GetAttrib("sCheckFieldName");
		pCol->nCheckFieldId		= atoi(xml.GetAttrib("nCheckFieldId"));
		pCol->nCheckHeader		= atoi(xml.GetAttrib("nCheckHeader"));
		pCol->bAllowSort		= atoi(xml.GetAttrib("bAllowSort"));
		pCol->nPrecision		= atoi(xml.GetAttrib("nPrecision"));
		pCol->nScale			= atoi(xml.GetAttrib("nScale"));
		pCol->nScaleShow		= atoi(xml.GetAttrib("nScaleShow"));
		pCol->pGetAsTextFunc	= NULL;
		pCol->pNextCol			= NULL;

		if( !GetODBCGridCol(pCol) )
			{
			delete pCol;
			nInvalid	++;
			nLoop		++; 
			continue;
			}

		pColFirst				= (ODBCGridCol*)InsertToPointersChain((void*)pColFirst, (void*)pCol, nLoop - nInvalid, (long)offsetof(ODBCGridCol, pNextCol), -1);		
		nLoop		++; 
		}

	lColCount = (long)(nLoop - nInvalid);
	return pColFirst;*/
	return NULL;
	}

//
//	[protected] ODBCGridView::BuildODBCColumnsArray
//
void
ODBCGridView::BuildODBCColumnsArray()
	{/*
	if( m_nNumberCols <= 0 )
		return;

	if( m_pColByIndex )
		delete m_pColByIndex;

	// Load field infos. ######################
	int	iLoop		= 0;
	int nFieldCount = m_pRstSelect->GetODBCFieldCount();
	while( iLoop < nFieldCount )
		{
		m_pRstSelect->GetODBCFieldInfo(iLoop, m_fieldInfos[iLoop]);
		iLoop ++;
		}
	// ########################################

	long			lRegCols	= 0;
	ODBCGridCol*	pRegCols	= NULL;//RegLoadGridInfo(lRegCols);
	BOOL			bLoadReg	= (pRegCols != NULL);

	if( !m_pRecord ) // When recordset is not opened.
		{
		m_pColByIndex				= new ODBCGridCol*[m_nNumberCols];
		ODBCGridCol*	pCol		= m_pCols;

		while( iLoop < m_nNumberCols )
			{
			m_pColByIndex[iLoop]	= pCol;
			pCol->nFieldID			= 0;
			pCol->nCheckFieldId		= 0;
			pCol->pGetAsTextFunc	= NULL;
			pCol->nPrecision		= 0;
			pCol->nScale			= 0;
			pCol->nCheckHeader		= 0;
			pCol->bAllowSort		= TRUE;
			pCol->nWidth			= 0;

			if( pCol->nFieldID < 0 || pCol->bRowIndex )
				{
				iLoop ++; pCol = pCol->pNextCol;
				continue;
				}
			iLoop ++; pCol = pCol->pNextCol;
			}

		if( pRegCols )
			FreePointersChain((void*)pRegCols, (long)offsetof(ODBCGridCol, pNextCol));
		return;
		}

	RecordField*	pFields		= m_pRecord->GetFieldInfosArray();
	iLoop						= 0;

	if( bLoadReg && !m_bGridInfoLoaded ) // Reinitialize grid from registry loaded info.
		{
		m_nNumberCols			= lRegCols;
		m_pColByIndex			= new ODBCGridCol*[m_nNumberCols];
		ODBCGridCol*	pCol	= pRegCols;
		CListCtrl&		list	= GetListCtrl();
		list.SetRedraw(FALSE);

		// Delete columns. #############
		int nColumnCount = list.GetHeaderCtrl()->GetItemCount();
		// Delete all of the columns.
		for (int i=0;i < nColumnCount;i++)
			list.DeleteColumn(0);
		// #############################

		int			nInvalid = 0;
		while( iLoop < m_nNumberCols )
			{
			if( GetODBCGridCol(pCol) )
				{
				// Add new column. ###############
				LVCOLUMN	column;
				column.mask				= LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
				if( pCol->iImage > -1 )
					column.mask			|= LVCF_IMAGE;
				column.pszText			= (LPTSTR)pCol->sTitle.GetBuffer();
				column.cchTextMax		= 255;
				column.fmt				= pCol->nAlign;
				column.cx				= pCol->nWidth;
				column.iImage			= 0;//pCol->iImage;
				list.InsertColumn(iLoop, &column);
				// ###############################
				m_pColByIndex[iLoop] = pCol;
				}
			else
				nInvalid ++;

			iLoop ++; pCol = pCol->pNextCol;
			}

		// Free pointers chain. ########
		if( m_pCols )
			FreePointersChain((void*)m_pCols, (long)offsetof(ODBCGridCol, pNextCol));
		m_pCols = pRegCols;
		m_bGridInfoLoaded = TRUE;
		list.SetRedraw(TRUE);
		}
	else
		{
		m_pColByIndex			= new ODBCGridCol*[m_nNumberCols];
		ODBCGridCol*	pCol	= m_pCols;

		while( iLoop < m_nNumberCols )
			{
			if( GetODBCGridCol(pCol) )
				m_pColByIndex[iLoop] = pCol;
			iLoop ++; pCol = pCol->pNextCol;
			}

		if( pRegCols )
			FreePointersChain((void*)pRegCols, (long)offsetof(ODBCGridCol, pNextCol));
		}*/
	}

//
//	[protected] ODBCGridView::GetODBCGridCol
//
BOOL
ODBCGridView::GetODBCGridCol(ODBCGridCol* pCol)
	{/*
	if( !m_pRecord )
		return FALSE;

	RecordField*	pFields	= m_pRecord->GetFieldInfosArray();
	pCol->nFieldID			= m_pRecord->GetFieldID(pCol->sFieldName);
	pCol->nCheckFieldId		= m_pRecord->GetFieldID(pCol->sCheckFieldName);
	pCol->pGetAsTextFunc	= NULL;
	pCol->nPrecision		= 0;
	pCol->nScale			= 0;
	pCol->nCheckHeader		= 0;

	if( pCol->nFieldID < 0 || pCol->bRowIndex )
		{
		return TRUE;
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
			CODBCFieldInfo* fieldInfo = &m_fieldInfos[pCol->nFieldID];
			if( fieldInfo->m_nSQLType == SQL_NUMERIC || 
				fieldInfo->m_nSQLType == SQL_DECIMAL  )
				{
				pCol->nPrecision	 = fieldInfo->m_nPrecision;
				pCol->nScale		 = fieldInfo->m_nScale;
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
		//	case SQL_DATE:
		//	case SQL_TIME:
		//	case SQL_TIMESTAMP: 
		//	CODBCFieldInfo fieldInfo;
		//	m_pRstSelect->GetODBCFieldInfo(pCol->nFieldID, fieldInfo);

			pCol->pGetAsTextFunc = ((PROC)GetDate_FieldAsText);
			break;
			};
		case DBVT_SINGLE:
		case DBVT_DOUBLE:
			{
			CODBCFieldInfo* fieldInfo	= &m_fieldInfos[pCol->nFieldID];
			pCol->pGetAsTextFunc		= ((PROC)GetDouble_FieldAsText);
			break;
			};
		};*/

	return TRUE;
	}

// ===============================================================================
// GridView by given name.
// ===============================================================================
ODBCGridView*
ODBCGridView::GridViewByName(TCHAR* lpszGridName)
	{
	return NULL;
//	return (ODBCGridView*)GlobalStorage::GetInstance()->GetNamedObject(lpszGridName);
	}

// ==============================================================================
/* Retrieve field given grid name, field and row index(-1 means selected row)  */
// ==============================================================================

CString
ODBCGridView::GetString(TCHAR* lpszGridName, TCHAR* lpszField, int nRow /*= -1*/)
	{/*
	ASSERT( nRow >= -1 );
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid ){
		// Lock
		pGrid->m_csection.Lock();
		AutoSortedArray* pArr = pGrid->m_pArrRecDataById;
		if( pGrid->m_pArrRecDataShow )
			pArr = pGrid->m_pArrRecDataShow;
		
		nRow = ((nRow == -1) ? pGrid->GetCurRow() : nRow);
		if( nRow < 0 ) nRow = 0;

		if( pArr->GetCount() <= nRow ) {
			pGrid->m_csection.Unlock();
			return "";
			}

		void* lpRecData		= (void*)pArr->GetData(nRow);
		void* lpRecDataOld	= pGrid->m_pRecord->GetRecordData();

		pGrid->m_pRecord->SetRecordData(lpRecData);
		CString sValue		= pGrid->m_pRecord->GetString(lpszField);
		pGrid->m_pRecord->SetRecordData(lpRecDataOld);
		// Unlock.
		pGrid->m_csection.Unlock();
		return sValue;
		}*/
	return "";
	}

double
ODBCGridView::GetDouble(TCHAR* lpszGridName, TCHAR* lpszField, int nRow /*= -1*/, double dDefault /*= 0.0*/)
	{/*
	ASSERT( nRow >= -1 );
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid ){
		// Lock
		pGrid->m_csection.Lock();
		AutoSortedArray* pArr = pGrid->m_pArrRecDataById;
		if( pGrid->m_pArrRecDataShow )
			pArr = pGrid->m_pArrRecDataShow;
		
		nRow = ((nRow == -1) ? pGrid->GetCurRow() : nRow);
		if( nRow < 0 ) nRow = 0;

		if( pArr->GetCount() <= nRow ) {
			pGrid->m_csection.Unlock();
			return dDefault;
			}

		void* lpRecData		= (void*)pArr->GetData(nRow);
		void* lpRecDataOld	= pGrid->m_pRecord->GetRecordData();

		pGrid->m_pRecord->SetRecordData(lpRecData);
		double dValue		= pGrid->m_pRecord->GetDouble(lpszField);
		pGrid->m_pRecord->SetRecordData(lpRecDataOld);
		// Unlock.
		pGrid->m_csection.Unlock();
		return dValue;
		}*/
	return dDefault;
	}

long
ODBCGridView::GetLong(TCHAR* lpszGridName, TCHAR* lpszField, int nRow /*= -1*/, long lDefault /*= 0*/)
	{/*
	ASSERT( nRow >= -1 );
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid ){
		// Lock
		pGrid->m_csection.Lock();
		AutoSortedArray* pArr = pGrid->m_pArrRecDataById;
		if( pGrid->m_pArrRecDataShow )
			pArr = pGrid->m_pArrRecDataShow;
		
		nRow = ((nRow == -1) ? pGrid->GetCurRow() : nRow);
		if( nRow < 0 ) nRow = 0;

		if( pArr->GetCount() <= nRow ) {
			pGrid->m_csection.Unlock();
			return lDefault;
			}

		void* lpRecData		= (void*)pArr->GetData(nRow);
		void* lpRecDataOld	= pGrid->m_pRecord->GetRecordData();

		pGrid->m_pRecord->SetRecordData(lpRecData);
		long lValue		= pGrid->m_pRecord->GetLong(lpszField);
		pGrid->m_pRecord->SetRecordData(lpRecDataOld);
		// Unlock.
		pGrid->m_csection.Unlock();
		return lValue;
		}*/
	return lDefault;
	}

BOOL
ODBCGridView::GetBool(TCHAR* lpszGridName, TCHAR* lpszField, int nRow /*= -1*/, BOOL bDefault /*= FALSE*/)
	{/*
	ASSERT( nRow >= -1 );
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid ){
		// Lock
		pGrid->m_csection.Lock();
		AutoSortedArray* pArr = pGrid->m_pArrRecDataById;
		if( pGrid->m_pArrRecDataShow )
			pArr = pGrid->m_pArrRecDataShow;
		
		nRow = ((nRow == -1) ? pGrid->GetCurRow() : nRow);
		if( nRow < 0 ) nRow = 0;

		if( pArr->GetCount() <= nRow ) {
			pGrid->m_csection.Unlock();
			return bDefault;
			}

		void* lpRecData		= (void*)pArr->GetData(nRow);
		void* lpRecDataOld	= pGrid->m_pRecord->GetRecordData();

		pGrid->m_pRecord->SetRecordData(lpRecData);
		BOOL bValue			= (pGrid->m_pRecord->GetBool(lpszField) == true);
		pGrid->m_pRecord->SetRecordData(lpRecDataOld);
		// Unlock.
		pGrid->m_csection.Unlock();
		return bValue;
		}*/
	return bDefault;
	}

BOOL
ODBCGridView::GetDate(TCHAR* lpszGridName, TCHAR* lpszField, COleDateTime& dtValue, int nRow /*= -1*/)
	{/*
	ASSERT( nRow >= -1 );
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid ){
		// Lock
		pGrid->m_csection.Lock();
		AutoSortedArray* pArr = pGrid->m_pArrRecDataById;
		if( pGrid->m_pArrRecDataShow )
			pArr = pGrid->m_pArrRecDataShow;
		
		nRow = ((nRow == -1) ? pGrid->GetCurRow() : nRow);
		if( nRow < 0 ) nRow = 0;

		if( pArr->GetCount() <= nRow ) {
			pGrid->m_csection.Unlock();
			return FALSE;
			}

		void* lpRecData		= (void*)pArr->GetData(nRow);
		void* lpRecDataOld	= pGrid->m_pRecord->GetRecordData();

		pGrid->m_pRecord->SetRecordData(lpRecData);
		pGrid->m_pRecord->GetDate(lpszField, dtValue);
		pGrid->m_pRecord->SetRecordData(lpRecDataOld);
		// Unlock.
		pGrid->m_csection.Unlock();
		return TRUE;
		}*/
	return FALSE;
	}

int
ODBCGridView::GetRowCount(TCHAR* lpszGridName)
	{/*
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid ){
		return pGrid->GetRowCount();
		}*/
	return 0;
	}

CString
ODBCGridView::GetField(TCHAR* lpszGridName, int nField)
	{/*
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid && pGrid->m_pRecord ){
		return pGrid->m_pRecord->GetFieldName(nField);
		}*/
	return _T("");
	}

int
ODBCGridView::GetFieldCt(TCHAR* lpszGridName)
	{/*
	ODBCGridView* pGrid = GridViewByName(lpszGridName);
	if( pGrid && pGrid->m_pRecord ){
		return pGrid->m_pRecord->GetFieldCount();
		}*/
	return 0;
	}
