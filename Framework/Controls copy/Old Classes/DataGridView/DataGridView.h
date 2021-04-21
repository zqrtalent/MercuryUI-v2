/////////////////////////////////////////
//	class: DataGridView
//	programmer:	ZqrTalent
//	Date Start:	23/8/2007
/////////////////////////////////////////

#pragma once
#include "GridView.h"
#include "StringDecoder.h"

//
//	class DataGridView
//

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
	void	SetImageList(UINT imageId, int cx, int cy, 
						   UINT flags, int images, COLORREF transparent, UINT imageMaskId = 0L);
	BOOL	Load();
	BOOL	ReLoad(CStringArray* pArrField = NULL);
	int		GetCurRow();
	int		GetSelectedCount();
	int		GetRowCount();
	void	SelectItem(int nItem, BOOL bDeselect = TRUE);

	BOOL	Attach(HWND hWndAttachTo);

public:
	
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
	virtual bool	SortList			(int nCol, bool bAscending);

	virtual void	RowChanged			();
	virtual void	DetermineCellIcon	(int row, int col, int& nIcon){};

	virtual void	OnDraw				(CDC* pDC);
	virtual void	OnInitialUpdate		();
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	virtual void	PreSubclassWindow	(void);
	virtual void	OnSysColorChange	();

	// < protected methods >
protected:
	void			BuildODBCColumnsArray	();
	void			FreeRecordDataArray		();
	void			ReInitializeHeader		();
	void			GetCheckBitmaps			(HBITMAP& pbmpCheck, HBITMAP& pbmpUnCheck);

	// < Protected attributes >
protected:
		
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