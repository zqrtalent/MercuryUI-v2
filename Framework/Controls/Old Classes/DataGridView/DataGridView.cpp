/////////////////////////////////////////
//	class: DataGridView
//	programmer:	ZqrTalent
//	Date Start:	23/8/2007
/////////////////////////////////////////

#include "stdafx.h"
#include "DataGridView.h"
//#include "BitmapEx.h"
//#include "UsefulFunctions.h"
#include <stddef.h>

// DataGridView
IMPLEMENT_DYNCREATE(DataGridView, GridView)

//
//	DataGridView::DataGridView
//
DataGridView::DataGridView() : GridView(){
	}

DataGridView::~DataGridView(){
	}

BEGIN_MESSAGE_MAP(DataGridView, GridView)
	ON_NOTIFY_REFLECT	(LVN_ITEMCHANGED,	OnItemChanged	)
	ON_NOTIFY_REFLECT	(LVN_DELETEITEM,	OnItemDelete	)
	ON_WM_MOUSEACTIVATE	()
	ON_WM_CREATE		()
END_MESSAGE_MAP()

//
//	DataGridView::OnDraw
//
void	
DataGridView::OnDraw(CDC* pDC){}

//
//	DataGridView::OnInitialUpdate
//
void
DataGridView::OnInitialUpdate()
	{
	GridView::OnInitialUpdate();

	/*
	if( m_sGridName.IsEmpty() )
		m_sGridName = GetRuntimeClass()->m_lpszClassName;

	// Register self as global named object. ############
	GlobalStorage::GetInstance()->SetNamedObject(m_sGridName.GetBuffer(), this);
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
//	DataGridView::OnDestroy
//
void 
DataGridView::OnDestroy()
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
//	[public] DataGridView::SetImageList
//
void
DataGridView::SetImageList(UINT imageId, int cx, int cy, 
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
	while( nLoop < nPixelCount ){
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
//	[virtual] DataGridView::OnSetup
//
void	
DataGridView::OnSetup()
	{
	}

//
//	[public] DataGridView::Load
//
BOOL 
DataGridView::Load()
	{
	/*
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
	AfxGetApp()->EndWaitCursor();
	return TRUE;
	*/
	return FALSE;
	}

//
//	[public] DataGridView::ReLoad
//

BOOL
DataGridView::ReLoad(CStringArray* pArrFields /*= NULL*/)
	{
	/*
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
	m_csection.Unlock();
	return TRUE;*/
	return FALSE;
	}

//
//	[virtual] DataGridView::OnChooseGridFont
//
bool
DataGridView::OnChooseGridFont(OUT LOGFONT* plf){
	ZeroMemory(plf, sizeof(LOGFONT));
	plf->lfCharSet			= ANSI_CHARSET;
	plf->lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	plf->lfHeight			= 16;
	plf->lfOrientation		= 1500;
	plf->lfOutPrecision		= CLIP_DEFAULT_PRECIS;
	plf->lfPitchAndFamily	= VARIABLE_PITCH | FF_ROMAN;
	plf->lfQuality			= PROOF_QUALITY;
	plf->lfWeight			= FW_NORMAL;
	
	_tcscpy(plf->lfFaceName, _T("GeoMegasoft"));
	return true;
	}

//
//	[virtual] DataGridView::OnDrawCellIcon
//
bool
DataGridView::OnDrawCellIcon(IN int Row, IN int Col,
							IN HDC dc, IN int x, IN int y, IN COLORREF crBkgnd)
	{
	/*
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
//	[virtual] DataGridView::OnGetCellBorders
//
bool 
DataGridView::OnGetCellBorders(	IN  int			Row,
								IN  int			Col,
								OUT COLORREF&	crBorder,
								OUT UINT&		nPenStyle,
								OUT UINT&		nBorderStyle,
								OUT int&		nWidth) const
	{
	nPenStyle		= PS_SOLID;
	nWidth			= 1;
	bool	bSel	= mDraw.Selected;
/*
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
	*/

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

	nBorderStyle	= BORDER_STYLE_RIGHT;
	return true;
	}

//
//	[virtual] DataGridView::GetCellText
//
bool
DataGridView::OnGetCellText(IN int Row, IN int Col, CString& strCellText, IN LONG lRowParam)
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
//	[virtual] DataGridView::OnGetCellTextAlign
//
bool
DataGridView::OnGetCellTextAlign(IN int Row, IN int Col, OUT int& nAlign)
	{
	nAlign = DT_SINGLELINE;//|m_pColByIndex[Col]->nAlign;
	return true;
	}

//
//	DataGridView::OnGetCellColors
//
bool 
DataGridView::OnGetCellColors(	IN  int Row,
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
/*
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
*/
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
	return (TextStyle != 0);
	}



//
//	[protected virtual] DataGridView::RowChanged
//
void
DataGridView::RowChanged()
	{
	}

//
//	[protected virtual] DataGridView::SortList
//
bool	
DataGridView::SortList(int nCol, bool bAscending)
	{
	return true;
	}

//
//	[protected virtual] DataGridView::PreCreateWindow
//
BOOL 
DataGridView::PreCreateWindow(CREATESTRUCT& cs)
	{
	cs.style |= LVS_REPORT|LVS_SHOWSELALWAYS|LVS_OWNERDATA;
	return GridView::PreCreateWindow(cs);
	}

//
//	[afx_msg] DataGridView::OnCreate
//
int
DataGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
	BOOL bRet = GridView::OnCreate(lpCreateStruct);
	/*
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
		}*/
	return bRet;
	}

//
//	[protected virtual] DataGridView::PreSubclassWindow
//
void
DataGridView::PreSubclassWindow(void)
	{
	GridView::PreSubclassWindow();
/*
	CListCtrl&		list		= GetListCtrl();
	CHeaderCtrl*	pHeaderCtrl = list.GetHeaderCtrl();
	if( pHeaderCtrl && !IsWindow(m_headerCtrl) )
		{
		// Subclass header control. ##############
		if( m_headerCtrl.SubclassWindow(pHeaderCtrl->GetSafeHwnd()) )
			m_headerCtrl.m_pGridView = this;
		// #######################################
		}*/
	}

//
//	[protected handler] DataGridView::OnTimer
//
void	
DataGridView::OnTimer(UINT_PTR nIDEvent)
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
//	[protected handler] DataGridView::OnItemChanged
//
void
DataGridView::OnItemChanged(LPNMHDR pNMHDR, LRESULT* pResult)
	{
	/*
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
		*/
	}

//
//	[protected handler] DataGridView::OnItemDelete
//
void	
DataGridView::OnItemDelete(LPNMHDR pNMHDR, LRESULT* pResult)
	{
	/*
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
//	[protected handler] DataGridView::OnMouseActivate
//
int		
DataGridView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
	{/*
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
*/
	return GridView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

//
//	[public] DataGridView::GetCurRow
//
int
DataGridView::GetCurRow()
	{
	/*
	m_csection.Lock();
	if( m_pArrRecDataById && m_pArrRecDataById->GetCount() > 0)
		{
		m_csection.Unlock();
		return mRow;
		}
	m_csection.Unlock();*/
	return -1;
	}

//
//	[public] DataGridView::GetSelectedCount
//
int
DataGridView::GetSelectedCount()
	{
	/*
	CListCtrl&	list		= GetListCtrl();
	int			nSelCount	= list.GetSelectedCount();
	return nSelCount;*/
	return 0;
	}

//
//	[public] DataGridView::GetRowCount
//
int
DataGridView::GetRowCount()
	{/*
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
	*/
	return 0;
	}

//
//	[public] DataGridView::SelectItem
//
void
DataGridView::SelectItem(int nItem, BOOL bDeselect /*= TRUE*/)
	{
	/*
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
	m_csection.Unlock();
	*/
	}

//
//	[public] DataGridView::Attach
//
BOOL			
DataGridView::Attach(HWND hWndAttachTo)
	{
	if( !SubclassWindow(hWndAttachTo) )
		return FALSE;

	SetWindowLong	(m_hWnd, GWL_STYLE, WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_OWNERDATA);
	OnInitialUpdate	();
	return TRUE;
	}

//
//	[virtual] DataGridView::OnGetCellType
//
CellType 
DataGridView::OnGetCellType(IN int Row, IN int Col)
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
//	[virtual] DataGridView::OnCellRBtnDown
//
void	
DataGridView::OnCellRBtnDown(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point)
	{/*
	if( m_pColByIndex[Col]->bCheckBox		&& 
		m_pColByIndex[Col]->nCheckFieldId > -1 ) // CheckBox column.
		{
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
			}
		}
	else
		{
		}*/
	
//	GridView::OnCellRBtnUp(Row, Col, CellRect, Point);
	}

//
//	[virtual] DataGridView::OnCellLBtnDown
//
bool
DataGridView::OnCellLBtnDown(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point)
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
		}
*/
	return GridView::OnCellLBtnDown(Row, Col, CellRect, Point);
	}

//
//	[virtual] DataGridView::OnSysColorChange
//
void 
DataGridView::OnSysColorChange()
	{
	GridView::OnSysColorChange();
//	GetCheckBitmaps(m_headerCtrl.m_hBmpCheckOn, m_headerCtrl.m_hBmpCheckOff);	
	}