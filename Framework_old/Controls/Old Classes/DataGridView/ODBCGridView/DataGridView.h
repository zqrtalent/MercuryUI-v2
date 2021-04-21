/////////////////////////////////////////
//	class: DataGridView
//	programmer:	ZqrTalent
//	Date Start:	23/8/2007
/////////////////////////////////////////

#pragma once
#include "GridView.h"
#include "StringDecoder.h"
#include "XHeaderCtrl.h"
#include "..\Markup.h"

class AutoSortedArray;
class iODBCRst;
class CRecord;
class RMC;

// Row changed message to owner.
#define WM_ROW_CHANGED		WM_USER + 1005
#define	WM_ADD_RECORDS		WM_USER + 1006
#define MENU_ITEM_CHECK		20000
#define MENU_ITEM_UNCHECK	20001

#define SORT_FLAG_NOT		0
#define SORT_FLAG_ASC		1
#define SORT_FLAG_DESC		2

#define MENU_ITEM_CHECK_TEXT	L"ყველას ჩართვა"
#define MENU_ITEM_UNCHECK_TEXT	L"ყველას გათიშვა"

#define TIMER_ROW_CHANGE	23

/*
//
//	class CODBCHeaderCtrl
//

class DataGridView;
class CODBCHeaderCtrl: public CXHeaderCtrl
{
public:
	CODBCHeaderCtrl	();
	~CODBCHeaderCtrl();

	friend DataGridView;

public:
	DataGridView* m_pGridView;

protected:
	virtual	BOOL	GetColCheckState(int nCol, BOOL& bState);
	virtual BOOL	GetColSortStatus(int nCol, BOOL& bState);

protected:

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
*/
//
//	class DataGridView
//

typedef struct ODBCGridCol_tag
	{
	CString				sFieldName;			// Specifies field name.
	CString				sTitle;				// Column title.
	int					nFieldID;			// Specifies field index.
	int					nAlign;				// Specifies column align.
	int					nWidth;				// col width.
	int					iImage;
	BOOL				bRowIndex;			// Indicates if this column is row index column.
	BOOL				bCheckBox;			// Indicates checkbox column.
	CString				sCheckFieldName;	// Check field name.
	int					nCheckFieldId;		// Check field id.
	int					nCheckHeader;		// Check header state.
	BOOL				bAllowSort;			// Allow sort that column.
	PROC				pGetAsTextFunc;		// Function to retrieve field as text.
	int					nPrecision;
	int					nScale;
	int					nScaleShow;
	ODBCGridCol_tag*	pNextCol;			// Next DBColumn pointer.
	} ODBCGridCol, *PODBCGridCol;

typedef struct DefinedCol_tag
	{
	CString				sColTitle;			// Column title.
	CString				sDBField;			// Field name.
	int					nAlign;				// Specifies column align.
	int					nWidth;				// col width.
	int					iImage;				// Image index.
	CString				sCheckFieldName;			// Checkbox
	int					nScaleShow;
	DefinedCol_tag*		pNextCol;			// Next DefinedCol pointer.
	} DefinedCol, *PDefinedCol;

typedef void (*GetFieldAsText)		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);

class DataGridView: public GridView
{
	// < Constructor/Destructor >
public:
	DataGridView();
	virtual ~DataGridView();

	friend class CODBCHeaderCtrl;

	DECLARE_DYNCREATE(DataGridView)

	// < Public methods >
public:
	// Grid as window operations. ##################################
	BOOL			Attach				(HWND hWndAttachTo);
	void			SetExtendedStyle	(DWORD dwExStyle);
	void			SetImageList		(UINT imageId, int cx, int cy, UINT flags, int images, 
										COLORREF transparent, UINT imageMaskId = 0L);
	void			SetGridOwner		(HWND hWndOwner);
	void			DefineColumn		(int nIndex, LPCTSTR lpszTitle, LPCTSTR lpszDBField, 
										int nAlignment, int nWidth, int iImage = -1, 
										LPCTSTR lpszChkField = NULL, int nScaleShow = -1);

	void			ReloadCols			(); // Reloads grid columns.

	void			SetWaitWhenRowChange(BOOL bWait = TRUE);
	int				FindKeyMoveTo		(LPCTSTR lpszKey, BOOL bMove = TRUE);
	void			MoveTo				(int nItem);
	void			SelectItem			(int nItem, BOOL bDeselect = TRUE);
	CString			GetItemText			(int nRow, int nCol);

	void			UseFirstColAsSelState(BOOL bUse);
	void			SetSingleSelection	(BOOL bSet);
	// ############################################################

	// Grid operations. ###########################################
	int				GetNumberCols		();
	int				GetCurRow			();
	int				GetSelectedCount	();
	int				GetRowCount			();
	CString			GetSelectedKeyList	(LPCTSTR delimiter);
	CString			GetCheckedKeyList	(LPCTSTR delimiter);
	// ############################################################
	
	// Grid record data manipulation. #############################
	void			SetRowId			(const CString sUIDField);
	iODBCRst*		SetRst				(iODBCRst* pRst);
	iODBCRst*		GetRst				();
	CString			GetRowId			(int nItem = -1);
	long			GetRowIdAsLong		(int nItem = -1);

	BOOL			Load				();
	BOOL			ReLoad				(CStringArray* pArrFields = NULL);
	BOOL			LoadOrReload		(BOOL bLoad = FALSE);
	BOOL			EmptyList			();
	// ############################################################

	// Grid auto refresh operations. ##############################
	void			SetAutoRefresh		(BOOL bAutoRefresh = TRUE);
	void			RefreshForAddNew		();
	void			RefreshForUpdate		(long lUid);
	void			RefreshForDelete		(long lUid);
	BOOL			SelectSQLWithUID		(CString& sSelectSQL, CString sUIDField, long lUid);
	long			UIDFromUpdateSQLQuery	(CString& sUpdateSQL);
	long			UIDFromDeleteSQLQuery	(CString& sDeleteSQL);
	void			SetGenerator			(CString sGenName);
	// ############################################################
	
	// Grid item check operations. #################################
	BOOL			GetCheckedArray			(int nCol, AutoSortedArray& arrIdsChecked, BOOL bChecked = TRUE);
	virtual void	CheckAll				(int nCol, BOOL bCheck, BOOL& bRedraw);
	// ############################################################

	// Sort operations. ###########################################
	BOOL			SortColumn				(int nCol, BOOL bAsc);
	BOOL			ColAllowSort			(int nCol, BOOL bAllow = TRUE);
	// ############################################################

	BOOL			SetMenuId				(UINT nIDMenu, UINT nPopupIndex = 1);
	CString			GetLastError			();
	BOOL			IsCmdHandled			(UINT nCode, UINT nCmdId);

public:
	// GridView by given name.
	static DataGridView*	GridViewByName(TCHAR* lpszGridName);
	/* Retrieve field given grid name, field and row index(-1 means selected row)  */
	static CString			GetString	(TCHAR* lpszGridName, TCHAR* lpszField, int nRow = -1); 
	static long				GetLong		(TCHAR* lpszGridName, TCHAR* lpszField, int nRow = -1, long lDefault = 0); 
	static double			GetDouble	(TCHAR* lpszGridName, TCHAR* lpszField, int nRow = -1, double dDefault = 0.0); 
	static BOOL				GetBool		(TCHAR* lpszGridName, TCHAR* lpszField, int nRow = -1, BOOL bDefault = FALSE); 
	static BOOL				GetDate		(TCHAR* lpszGridName, TCHAR* lpszField, COleDateTime& dtValue, int nRow = -1);

	static int				GetRowCount	(TCHAR* lpszGridName);
	static CString			GetField	(TCHAR* lpszGridName, int nField);
	static int				GetFieldCt	(TCHAR* lpszGridName);
	
protected:
	// Registry operations. ########################################
	BOOL			RegSaveGridInfo			();
	ODBCGridCol*	RegLoadGridInfo			(long& lColCount);
	// ############################################################

	// < Protected virtual methods >
protected:
	virtual bool	OnGetCellColors		(IN int Row, IN int Col, OUT COLORREF& TextColor, OUT COLORREF& BkColor, OUT	COLORREF& BottomLine,OUT UINT8& TextStyle) const;
	virtual bool	OnChooseGridFont	(OUT LOGFONT* plf);
	virtual void	OnSetup				();
	virtual bool	OnGetCellText		(IN int Row, IN int Col, OUT CString& strCellText, IN LONG lRowParam);
	virtual bool	OnGetCellTextAlign	(IN int Row, IN int Col, OUT int& nAlign);
	virtual bool	OnDrawCellIcon		(IN int Row, IN int Col, IN HDC dc, IN int x, IN int y, IN COLORREF crBkgnd);
	virtual bool	OnGetCellBorders	(IN  int Row, IN  int Col, OUT COLORREF& crBorder, OUT UINT& nPenStyle, OUT UINT& nBorderStyle, OUT int& nWidth) const;
	virtual void	OnDestroy			();
	virtual CellType OnGetCellType		(IN int Row, IN int Col);
	virtual bool	OnCellLBtnDown		(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point);
	virtual void	OnCellRBtnDown		(IN int Row, IN int Col, IN const RECT *CellRect, IN const CPoint& Point);
	virtual	void	OnCheckItem			(CRecord* pCurrRecord, int nCol, int nRow, BOOL& bAllow, BOOL bCurrentState);
	virtual bool	SortList			(int nCol, bool bAscending);

	virtual void	RowChanged			();
	virtual void	GetLastUIDofTable	(long& lUID);
	virtual void	DetermineCellIcon	(int row, int col, int& nIcon){};

	virtual void	OnDraw				(CDC* pDC);
	virtual void	OnInitialUpdate		();
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	virtual void	PreSubclassWindow	(void);
	virtual void	OnSysColorChange	();

	// < protected methods >
protected:
	void			AdjustBrightness		(IN OUT COLORREF& Color, IN INT16 Amount) const;
	void			BuildODBCColumnsArray	();
	BOOL			GetODBCGridCol			(ODBCGridCol* pCol);
	void			FreeRecordDataArray		();
	void			ReInitializeHeader		();
	void			GetCheckBitmaps			(HBITMAP& pbmpCheck, HBITMAP& pbmpUnCheck);

	// Functions to retrieve field value as text. ########################
	static void		GetBool_FieldAsText		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetInt_FieldAsText		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDouble_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDateTime_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDate_FieldAsText		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetString_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDecimal_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	// ###################################################################

	// Compare static functions. #########################################
	static int		CompareDummy		(void* lpValue1, void* lpValue2);
	static int		CompareTextField	(void* lpValue1, void* lpValue2);
	static int		CompareDateField	(void* lpValue1, void* lpValue2);
	static int		CompareLongField	(void* lpValue1, void* lpValue2);
	static int		CompareDoubleField	(void* lpValue1, void* lpValue2);
	static int		CompareDecimalField	(void* lpValue1, void* lpValue2);

	static int		CompareTextField_Desc	(void* lpValue1, void* lpValue2);
	static int		CompareDateField_Desc	(void* lpValue1, void* lpValue2);
	static int		CompareLongField_Desc	(void* lpValue1, void* lpValue2);
	static int		CompareDoubleField_Desc	(void* lpValue1, void* lpValue2);
	static int		CompareDecimalField_Desc(void* lpValue1, void* lpValue2);
	// #####################################################################

	// < Protected attributes >
protected:
	CString				m_sSelectSQL;		// Select SQL query.
	CString				m_sUIDField;		// Unique field name.
	int					m_nUIDField;		// Unique field id.
	iODBCRst*			m_pRstSelect;		// Select recordset.
	CString				m_strTableName;		// Origin table name.
	CRecord*			m_pRecord;			// Record object pointer.
	AutoSortedArray*	m_pArrRecDataById;	// Array record UID by Record data pointer.
	AutoSortedArray*	m_pArrRecDataShow;	// Array of record datas by show order.
	ODBCGridCol*		m_pCols;			// Chain of ODBCGridCol structures.
	ODBCGridCol**		m_pColByIndex;		// Array of ODBCGridCol structure pointers used currently.
	long				m_nNumberCols;		// Number of columns.
	CFont*				m_pFont;			// Font object for list control.
	CBitmap				m_bmImageList;		// Image for list control column images.
	BOOL				m_bIsRowSelected;	// Specifies selected row index on LButtondown.
	BOOL				m_bWaitRowChange;	// Wait when RowChange.
	HWND				m_hWndOwner;		// Owner window handle.
	BOOL				m_bAutoRefresh;		// Auto refresh flag.
	CString				m_sGeneratorName;	// generator name;
	BOOL				m_bIdSort;			// Id sort option for fast perfomance.
	DWORD				m_dwExStyle;		// Extended style.
	BOOL				m_bUseFirstColAsSelState;	// Use first col as selected state. (only when 1 col has checkboxes)
	int					m_nColCheck;		// Column index who needs to be checked or unchecked.
	HMENU				m_menuCheck;		// Check menu handler.
	BOOL				m_bSingleSel;		// Single selection state.
	CCriticalSection	m_csection;			// Critical section object.
	CODBCHeaderCtrl		m_headerCtrl;		// Subclassed header control.
	int					m_nSortCol;			// Sorted column index.
	int					m_nSortFlag;		// 0 - not, 1 - SORT_ASC, 2 - SORT_DESC
	CMenu				m_menuPopup;		// Menu showing on right click.
	HMENU				m_hMenuMain;		// Main manu handle.
	
	CString				m_sGridName;		// Grid name.
	BOOL				m_bGridInfoLoaded;	// Determinies grid's loaded state. 



	CODBCFieldInfo		m_fieldInfos[255];	// Field infos from select query.
	CString				m_strLastError;
	
	// < Protected message handlers >
protected:

	afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnShowWindow	(BOOL bShow, UINT nStatus);
	afx_msg void	OnTimer			(UINT_PTR nIDEvent);
	afx_msg void	OnItemChanged	(LPNMHDR pNMHDR, LRESULT* pResult);
	afx_msg void	OnItemDelete	(LPNMHDR pNMHDR, LRESULT* pResult);
	afx_msg int		OnMouseActivate	(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT	OnAddRecords	(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnCheckAll		();
	afx_msg void	OnUnCheckAll	();
	afx_msg void	OnContextMenu	(CWnd* pWnd, CPoint pos);
	DECLARE_MESSAGE_MAP()
};

__inline CString
DataGridView::GetLastError()
	{
	return m_strLastError;
	}

__inline iODBCRst*		
DataGridView::GetRst()
	{
	return m_pRstSelect;
	}

__inline void			
DataGridView::SetWaitWhenRowChange(BOOL bWait /*= TRUE*/)
	{
	m_bWaitRowChange = bWait;
	}

__inline void			
DataGridView::SetGridOwner(HWND hWndOwner)
	{
	m_hWndOwner = hWndOwner;
	}

__inline int				
DataGridView::GetNumberCols()
	{
	return m_nNumberCols;
	}

__inline void	
DataGridView::SetAutoRefresh(BOOL bAutoRefresh /*= TRUE*/)
	{
	m_bAutoRefresh = bAutoRefresh;
	}

__inline void
DataGridView::SetGenerator(CString sGenName)
	{
	m_sGeneratorName = sGenName;
	}

__inline void
DataGridView::SetExtendedStyle(DWORD dwExStyle)
	{
	m_dwExStyle = dwExStyle;
	}

__inline void
DataGridView::UseFirstColAsSelState	(BOOL bUse)
	{
	m_bUseFirstColAsSelState = bUse;
	}

__inline void
DataGridView::SetSingleSelection(BOOL bSet)
	{
	m_bSingleSel = bSet;
	}

__inline BOOL
DataGridView::ColAllowSort(int nCol, BOOL bAllow /*= TRUE*/)
	{
	CListCtrl&		list	= GetListCtrl();
	CHeaderCtrl*	pHeader	= list.GetHeaderCtrl();

	if( !pHeader || nCol < 0 || nCol >= pHeader->GetItemCount() )
		return FALSE;

	if( m_pColByIndex )
		m_pColByIndex[nCol]->bAllowSort = bAllow;
	else
		{
		ODBCGridCol*	pCol	= m_pCols;
		int				nLoop	= 0;
		while( pCol )
			{
			if( nLoop == nCol )
				{
				pCol->bAllowSort = bAllow;
				return TRUE;
				}
			pCol = pCol->pNextCol; nLoop ++;
			}
		return FALSE;
		}
	return TRUE;
	}

