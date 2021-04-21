#pragma once
#ifndef GridView_H
#define GridView_H

#include "GridDefines.h"

//
//	class GridView
//

class GridView: public CXTListView
{
	DECLARE_DYNAMIC(GridView)

public:
	GridView();
	virtual ~GridView();

/*
	PURPOSE:
		Scrolls the grid control so that the specified column gets visible

	ARGUMENTS:
		Col			Column index

	RETURN:
		The amount of pixels necessary to scroll the
		grid in order to make this column visible. If the
		return value is 0, the column was already visible.
*/
	int		EnsureColVisible(IN int Col);
	
/*
	PURPOSE:
		Determines the cell that contains the supplied point

	ARGUMENTS:
		Pt			The point in client coordinates
		Row			The hit cell row
		Col			The hit cell column
		Rect		The cell rectangle

	RETURN:
		TRUE if a cell is hit. FALSE == whitespace.
*/
	BOOL	CellHitTest(IN const CPoint& Pt, OUT int &Row, OUT int	&Col, OUT RECT *Rect = NULL OPTIONAL);

/*
	PURPOSE:
		Retrieve a cell bounding rectangle.

	ARGUMENTS:
		Self explanatory

	RETURN:
		TRUE for success
*/
	bool	GetCellRect(IN  int Row, IN int Col, OUT RECT& Rect);

/*
	PURPOSE:
		Ensure the supplied rectangle does not exceed the
		grid control's client area. (The rectangle is intersected
		with the grid client rect)

	ARGUMENTS:
		Self explanatory
*/
	void	ValidateCellRect(IN OUT RECT* rect);

/*
	PURPOSE:
		Get the index of the focused item (this information is read
		from the parent list control)

	RETURN:
		The index or -1 if no item has the focus
*/
	int		GetFocused() const;

/*
	PURPOSE:
		Get the focused cell (this information is read from the cached
		coordinates)
*/
	inline void GetFocusedCell(OUT int& Row, OUT int& Col) { Row = mRow, Col = mCol; };

/*
	PURPOSE:
		Select the specified cell and ensure it is visible.

	ARGUMENTS:
		Row			...
		Col			...
		DoRedraw	If true, the cell that was previously selected
					is redrawn for clearing the focus rectangle. Also,
					the new selected cell is redrawn for painting
					the focus rectangle.
	RETURN:
		TRUE if success
*/
	bool	SelectCell(IN int Row, IN int Col, IN bool DoRedraw = false);

/*
	PURPOSE:
		Deselect all (selected) items, select and focus the current cell
*/
	void	DeselectAll();

/*
	PURPOSE:
		Enter edit mode

	ARGUMENTS:
		Row
		Col
		CellRect		If NULL, GetCellRect(...) is called automatically

	RETURN:
		TRUE if success and the inplace editor was created
*/
	bool	EditCell(	IN int Row,
						IN int Col,
						IN const RECT* CellRect = NULL OPTIONAL);

/*
	PURPOSE:
		GetColumnCount
*/
	inline	int GetColumnCount() const { return GetListCtrl().GetHeaderCtrl()->GetItemCount(); };

/*
	PURPOSE:
		HasGridLines
*/
	inline	bool HasGridLines() const { return ((GetListCtrl().GetExtendedStyle() & LVS_EX_GRIDLINES) != 0); };

/*
	PURPOSE:
		Redraws the specified row. If CheckVisibility == true,
		the row is redrawn only if its visible in the current view.
*/
	void RedrawItem(IN int Row, IN bool CheckVisibility = true);

/*
	PURPOSE:
		Get/set the in-place editors text limit. This limit prevents
		large string typing. The editors that uses this information
		are EDITBOX-es and EDITCOMBOBOX-es.
*/
	inline	UINT16	GetInplaceEditLimit() const { return mEditLen; };
	void	SetInplaceEditLimit(UINT16 Limit) { mEditLen = Limit; };

/*
	PURPOSE:
		GridView has an internal buffer used for any string operation.
		This buffer MUST be large enough to carry any string displayed by
		the grid control.
		The following routines handle its size.
*/
	inline	UINT16	GetStringBufferLen() const { return mBufferLen; };
	void	SetStringBufferLen(UINT16 Len);

/*
	PURPOSE:
		Enable/disable grid's incremental search support.
*/
	inline	bool GetIncrementalSearch() const { return mIncSearch; };
	inline	void SetInctementalSearch(bool Enable = true) { mIncSearch = Enable; };

/*
	PURPOSE:
		Controls the time after an incremental search is reset.
*/
	inline	UINT16 GetIncrementalSearchTimer() const { return mIncSearchTimer; };
	inline	void   SetIncrementalSearchTimer(IN UINT16 msecs) { mIncSearchTimer = msecs; };

/*
	PURPOSE:
		Set/reset drawing colors
*/
	void SetColors			(COLORREF ColorFrame, COLORREF ColorNormalFill, COLORREF ColorSelFill, COLORREF ColorBtnFill, COLORREF ColorNormalText, COLORREF ColorSelText);
	void SetColorsToDefault	();

/*
	PURPOSE:
		Set Cell/Row selection mode.
*/

	void SetCellSelectionMode(BOOL bMode = TRUE);

protected:

/*
	PURPOSE:
		Cache colors and generate bitmaps.
*/
	virtual void CacheVisuals();

/*
	PURPOSE:
		Window message handlers
*/
	afx_msg virtual void	OnLButtonDown		(UINT nFlags, CPoint point);
	afx_msg virtual BOOL	OnLClick			(NMHDR* hdr, LRESULT* res);
	afx_msg virtual void	OnLButtonDblClk		(UINT nFlags, CPoint point);
	afx_msg virtual void	OnRButtonDown		(UINT nFlags, CPoint point);
	afx_msg virtual BOOL	OnRClick			(NMHDR* hdr, LRESULT* res);
	afx_msg virtual void	OnKeyDown			(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg virtual void	OnChar				(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg virtual void	OnSetFocus			(CWnd* pOldWnd);
	afx_msg virtual void	OnKillFocus			(CWnd* pNewWnd);
	afx_msg virtual BOOL	OnEraseBkgnd		(CDC *dc);
	afx_msg virtual void	OnSysColorChange	();
	afx_msg virtual LRESULT	OnSetImageList		(WPARAM wp, LPARAM lp);

	afx_msg virtual void	OnDestroy			();
	afx_msg virtual void	OnTimer				(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	virtual void			PreSubclassWindow	(void);
	virtual BOOL			OnCommand			(IN WPARAM wp, IN LPARAM lp);

/*
	PURPOSE:
		Callbacks for mouse events

	NOTE:
		Make sure you supercall the default implementation!

	RETURN:
		"OnCellClick" must return TRUE if any inplace editor was created.
*/
	virtual bool	OnCellLBtnDown(		IN int Row,
										IN int Col,
										IN const RECT *CellRect,
										IN const CPoint& Point);

	virtual void	OnCellLBtnUp(		IN int Row,
										IN int Col,
										IN const RECT *CellRect,
										IN const CPoint& Point);

	virtual void	OnCellRBtnDown(		IN int Row,
										IN int Col,
										IN const RECT *CellRect,
										IN const CPoint& Point);

	virtual void	OnCellRBtnUp(		IN int Row,
										IN int Col,
										IN const RECT *CellRect,
										IN const CPoint& Point);

	virtual void	OnCellDblClick(		IN int Row,
										IN int Col,
										IN const RECT* CellRect);

	virtual bool	OnChooseGridFont	(OUT LOGFONT* plf);
	virtual bool	OnGetCellText		(	IN int Row,
											IN int Col,
											OUT CString& strCellText, 
											IN LONG lRowParam);
	virtual bool	OnGetCellTextAlign	(	IN int Row,
											IN int Col,
											OUT int& nAlign);
	virtual bool	OnDrawCellIcon		(	IN int Row,
											IN int Col,
											IN HDC dc, 
											IN int x, 
											IN int y,
											IN COLORREF crBkgnd);

	virtual void	OnSetup				();

/*
	PURPOSE:
		Called when a check-box cell was clicked

	NOTE:
		Make sure you supercall the default implementation!
*/
	virtual void	OnCheckBoxClick(int Row, int Col);

/*
	PURPOSE:
		Called when a custom-edit cell was clicked
*/
	virtual void	OnCustomCellClick(int Row, int Col);

/*
	PURPOSE:
		Called when an inplace combo box is about to be displayed. The routine should
		provide the items for the combo box to be populated. It also may specify an icon
		for each item. In this case it must set 'HasImages' to TRUE, and fill 'Images'
*/
	virtual void	OnGetComboBoxItems(	IN  int Row,
										IN  int Col,
										OUT bool& HasImages,
										OUT CStringList& Items,
										OUT CArray<int, int>& Images);

/*
	PURPOSE:
		Called when an inplace editor is about to be destroyed. If the return value
		is FALSE, the editor will not be destroyed and the edit operation will continue.
		This is the right place to perform input validation.
*/
	virtual bool	OnEndInPlaceEdit(	IN int Row,
										IN int Col,
										IN bool Canceled,
										IN const TCHAR* Text,
										IN int Image);

/*
	PURPOSE:
		Called when an ASCII key is pressed.

	NOTE:
		Make sure you supercall the default implementation!
*/
	virtual void	OnIncrementalSearch(IN TCHAR ch);

/*
	PURPOSE:
		Called for each cell for determining its type
*/
	virtual CellType	OnGetCellType(IN int Row, IN int Col);

/*
	PURPOSE:
		Called for each cell in order to overwrite the global colors.
		Additional information can be found in mDraw.
*/
	virtual bool	OnGetCellColors(	IN  int Row,
										IN  int Col,
										OUT COLORREF& TextColor,
										OUT COLORREF& BkColor,
										OUT	COLORREF& BottomLine,
										OUT UINT8& TextStyle) const;

	virtual bool	OnGetCellBorders(	IN  int Row,
										IN  int Col,
										OUT COLORREF&	crBorder,
										OUT UINT&		nPenStyle,
										OUT UINT&		nBorderStyle,
										OUT int&		nWidth) const;

	virtual void	OnPreCreateFont(LOGFONT* lf){};



/*
	PURPOSE:
		Create in-place controls.
*/
	virtual bool	CreateInPlaceEdit(IN int Row, IN int Col, IN const RECT *CellRect);
	virtual bool	CreateInPlaceCombo(IN int Row, IN int Col, IN const RECT *CellRect, IN bool Editable);

/*
	PURPOSE:
		The DrawXxxCell routines handle cell painting.
*/
	virtual void __fastcall DrawRegularCell();
	virtual void __fastcall DrawEditBoxCell();
	virtual void __fastcall DrawComboBoxCell();
	virtual void __fastcall DrawCheckBoxCell();
	virtual void __fastcall DrawCustomCell();
	virtual void __fastcall DrawSeparatorCell();

protected:
	bool		mDestroying;

	int			mEditRow, mEditCol;	// the cell that is currently beeing edited
	int			mRow, mCol;			// the cell that has the focus

	UINT16		mEditLen;			// edit box text limit
	TCHAR*		mBuffer;			// global buffer for handling strings
	UINT16		mBufferLen;

	BOOL		m_bCellMode;		// Cell selection mode.
	CFont*		m_pFont;			// default grid font.

	bool		mIncSearch;
	CString		mIncSearchString;
	HWND		mIncSearchTip;
	UINT16		mIncSearchTimer;

protected:
	struct TransientCustomDraw
	{
		NMLVCUSTOMDRAW*		cd;
		bool				Focused;
		bool				Selected;
		UINT8				Justify;
		RECT				Rect;
		const TCHAR*		CellText;
		CellType			Type;
		int					Icon;

		bool				GridLines;

		HBITMAP				ComboBmp;
		HDC					ComboDC;
		HBITMAP				CustomBmp;
		HDC					CustomDC;
		HBITMAP				CheckOnBmp;
		HDC					CheckOnDC;
		HBITMAP				CheckOffBmp;
		HDC					CheckOffDC;

		bool				UseDefaultColors;
		HBRUSH				BrushFrame;
		COLORREF			ColorFrame;
		HBRUSH				BrushNormalFill;
		COLORREF			ColorNormalFill;
		HBRUSH				BrushSelectedFill;
		COLORREF			ColorSelectedFill;
		HBRUSH				BrushButtonFill;
		COLORREF			ColorText;
		COLORREF			ColorSelText;

		HFONT				CustomFont;
		UINT8				CustomFontStyle;

		HBITMAP				DefaultStockBmp;
		bool				Initialized;

		void KillObjects()
			{
			if (!Initialized) return;

			::DeleteObject(::SelectObject(ComboDC, DefaultStockBmp));
			::DeleteDC(ComboDC);

			::DeleteObject(::SelectObject(CustomDC, DefaultStockBmp));
			::DeleteDC(CustomDC);

			::DeleteObject(::SelectObject(CheckOnDC, DefaultStockBmp));
			::DeleteDC(CheckOnDC);

			::DeleteObject(::SelectObject(CheckOffDC, DefaultStockBmp));
			::DeleteDC(CheckOffDC);

			Initialized = false;
			}

		void KillBrushes()
			{
			::DeleteObject(BrushFrame);
			::DeleteObject(BrushNormalFill);
			::DeleteObject(BrushSelectedFill);
			::DeleteObject(BrushButtonFill);
			}

		void SetColors(	COLORREF ClFrame,
						COLORREF ClNormalFill,
						COLORREF ClSelFill,
						COLORREF ClBtnFill,
						COLORREF ClText,
						COLORREF ClSelText)
		{
			// destroy previous brushes
			if (!UseDefaultColors)
				KillBrushes();

			// new brushes
			ColorFrame			= ClFrame;
			BrushFrame			= ::CreateSolidBrush(ClFrame);
			ColorNormalFill		= ClNormalFill;
			BrushNormalFill		= ::CreateSolidBrush(ClNormalFill);
			ColorSelectedFill	= ClSelFill;
			BrushSelectedFill	= ::CreateSolidBrush(ClSelFill);
			BrushButtonFill		= ::CreateSolidBrush(ClBtnFill);
			ColorText			= ClText;
			ColorSelText		= ClSelText;

			// mark custom colors flag
			UseDefaultColors = false;
		}

		void ResetColors()
		{
			// destroy previous brushes
			if (!UseDefaultColors)
				KillBrushes();

			// set to defaults
			ColorFrame = ::GetSysColor(COLOR_BTNSHADOW);
			BrushFrame = ::GetSysColorBrush(COLOR_BTNSHADOW);
			ColorNormalFill = ::GetSysColor(COLOR_WINDOW);
			BrushNormalFill = ::GetSysColorBrush(COLOR_WINDOW);
			ColorSelectedFill = ::GetSysColor(COLOR_HIGHLIGHT);
			BrushSelectedFill = ::GetSysColorBrush(COLOR_HIGHLIGHT);
			BrushButtonFill = ::GetSysColorBrush(COLOR_BTNFACE);
			ColorText = ::GetSysColor(COLOR_WINDOWTEXT);
			ColorSelText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		}

		TransientCustomDraw(): Initialized(false), UseDefaultColors(true), CustomFont(NULL), CustomFontStyle(0) { ResetColors(); };
		~TransientCustomDraw()
		{
			if (!UseDefaultColors)
				KillBrushes();
			KillObjects();
			if (CustomFont)
				::DeleteObject(CustomFont);
		}
	};

	// transient drawing info (valid beetween OnCustomDraw and DrawXxxCell(..) only!)
	mutable TransientCustomDraw mDraw;

private:
/*
	PURPOSE:
		Handles NM_CUSTOMDRAW
*/
	BOOL		OnCustomDraw(IN NMHDR* Header, OUT LRESULT* Result);

/*
	PURPOSE:
		Generates a new font based on the current one, and caches it
		within mDraw

	ARGUMENTS:
		Style		Is a combination of 'enum FontStyle' values
*/
	void		GenerateFont(IN UINT8 Style);
};

__inline void 
GridView::SetColors(COLORREF ColorFrame, COLORREF ColorNormalFill, COLORREF ColorSelFill, COLORREF ColorBtnFill, COLORREF ColorNormalText, COLORREF ColorSelText) 
	{
	mDraw.SetColors(ColorFrame, ColorNormalFill, ColorSelFill, ColorBtnFill, ColorNormalText, ColorSelText); 
	CacheVisuals(); 
	};

__inline void 
GridView::SetColorsToDefault()
	{
	mDraw.ResetColors(); 
	CacheVisuals(); 
	};

__inline void 
GridView::SetCellSelectionMode(BOOL bMode /*= TRUE*/)
	{
	m_bCellMode = bMode;
	}

#endif //GridView_H