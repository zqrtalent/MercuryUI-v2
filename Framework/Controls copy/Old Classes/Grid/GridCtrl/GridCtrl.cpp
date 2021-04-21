//-------------------------------------------------------------------
//
//  $Id: CGridCtrl.cpp,v 1.12 2004/06/16 23:50:36 miezoo Exp $
//  Author: Marius Negrutiu (18.05.2004 21:13:01)
//  mailto://negrutiu@as.ro
//
//-------------------------------------------------------------------

#include "stdafx.h"
#include "GridCtrl.h"
#include "GridCtrlCellWndProcs.h"

#pragma warning(disable: 4100)

IMPLEMENT_DYNAMIC(CGridCtrl, CListCtrl)

#define MiddleInt(i1, i2)	(((i1) + (i2)) / 2)
#define RHEIGHT(pRect)		((pRect)->bottom - (pRect)->top)
#define RWIDTH(pRect)		((pRect)->right - (pRect)->left)
#define DC					mDraw.cd->nmcd.hdc

// custom drawing settings
#define COMBO_BTNWIDTH		14
#define CUSTOM_BTNWIDTH		14
#define CHECK_BTNWIDTH		14
#define CHECK_MARKWIDTH		11

//
//	CGridCtrl::CGridCtrl
//
CGridCtrl::CGridCtrl()
:	mEditLen(DEFAULT_EDIT_LIMIT),
	mBuffer(NULL),
	mBufferLen(0),
	mRow(-1),
	mCol(0),
	mDestroying(false),
	mEditCol(-1),
	mEditRow(-1),
	mIncSearch(true),
	mIncSearchTip(NULL),
	mIncSearchTimer(DEFAULT_TIMER_INCSEARCH)
	{
	// alloc string buffer
	SetStringBufferLen(DEFAULT_BUFFER_LENGTH);
	m_bCellMode		= TRUE;
	m_pFont			= NULL;
	}

//
//	CGridCtrl::~CGridCtrl
//
CGridCtrl::~CGridCtrl()
	{
	if( mBuffer )
		delete [] mBuffer;

	if( m_pFont )
		delete m_pFont;
	}

//
//	[virtual] CGridCtrl::PreSubclassWindow
//
void 
CGridCtrl::PreSubclassWindow(void)
	{
	// modify control style
	LONG style = GetWindowLong(m_hWnd, GWL_STYLE);

	style &= ~(	LVS_ICON | LVS_SMALLICON | LVS_LIST |	// set to "Report" view
				LVS_EDITLABELS);						// disable the default "edit labels"

	style |=	LVS_REPORT |							// set to "Report" view
				WS_CLIPCHILDREN;						// avoid flickering when resizing columns

	SetWindowLong(m_hWnd, GWL_STYLE, style);

	// modify control extended style
	int ExStyle =	LVS_EX_SUBITEMIMAGES |				// allow subitem icons
//					LVS_EX_GRIDLINES |
					LVS_EX_FULLROWSELECT;

	::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, ExStyle, ExStyle);

	// cache colors
	CacheVisuals();

	//
	// create the incremental-search tooltip
	// NOTE: it will be automatically destroyed
	//
	/*
	mIncSearchTip = CreateWindowEx(	WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX,
									CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
									m_hWnd, NULL, (HINSTANCE)(UINT64)GetWindowLong(m_hWnd, GWL_HINSTANCE), NULL);
	ASSERT(mIncSearchTip);
	*/

	// INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
	TOOLINFO ti;
	ti.cbSize	= sizeof(TOOLINFO);
	ti.uFlags	= TTF_TRACK;
	ti.hwnd		= m_hWnd;
	ti.hinst	= (HINSTANCE)(UINT64)GetWindowLong(m_hWnd, GWL_HINSTANCE);
	ti.uId		= ID_TOOLTIP;
	ti.lpszText = NULL;

	// register our listctrl with the tooltip
	::SendMessage(mIncSearchTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO)&ti);

	// Retrieve font object and set it to list. #######
	LOGFONT lf;
	if( OnChooseGridFont(&lf) )
		{
		if( m_pFont )
			delete m_pFont;

		m_pFont = new CFont();
		if( m_pFont->CreateFontIndirect(&lf) )
			SetFont(m_pFont);
		else
			{
			delete m_pFont;
			m_pFont = NULL;
			}
		}
	// ################################################
	}

//
//	CGridCtrl::SetStringBufferLen
//
void 
CGridCtrl::SetStringBufferLen(UINT16 Len)
	{
	if (Len != mBufferLen)
		{
		if( mBuffer )
			{
			delete [] mBuffer;
			mBufferLen = 0;
			}

		mBuffer = new TCHAR[Len];
		ASSERT(mBuffer);
		if( mBuffer )
			mBufferLen = Len;
		}
	}

//
//	CGridCtrl::OnLButtonDown
//
void 
CGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
	{
	int Row, Col;
	RECT rect;

	// destroy any in-place editor
	if (mEditRow != -1)
		{
		SetFocus();
		// do not perform the default action. in case of multiedit the selection
		// must not change, in order to apply the new data
		return;
		}

	// test which cell was clicked
	bool EditorOn = false;
	if( CellHitTest(point, Row, Col, &rect) )
		EditorOn = OnCellLBtnDown(Row, Col, &rect, point);

	// perform default action (only if no inplace editors are on - multiedit stuff)
	if (!EditorOn)
		CListCtrl::OnLButtonDown(nFlags, point);
	}

//
//	CGridCtrl::OnLClick
//
BOOL 
CGridCtrl::OnLClick(NMHDR* hdr, LRESULT* res)
	{
	// default message handle
	Default();

	int Row, Col;
	RECT rect;

	NMITEMACTIVATE *ia = (NMITEMACTIVATE*)hdr;
	CPoint point(ia->ptAction);

	// test which cell was clicked
	if( CellHitTest(point, Row, Col, &rect))
		OnCellLBtnUp(Row, Col, &rect, point);

	return FALSE;
	}

//
//	CGridCtrl::OnLButtonDblClk
//
void 
CGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
	// perform default action
	CListCtrl::OnLButtonDblClk(nFlags, point);

	// test which cell was clicked
	int Row, Col;
	RECT rect;
	if (CellHitTest(point, Row, Col, &rect))
		OnCellDblClick(Row, Col, &rect);
	}

//
//	CGridCtrl::OnRButtonDown
//
void 
CGridCtrl::OnRButtonDown(UINT nFlags, CPoint point)
	{
	int Row, Col;
	RECT rect;

	// destroy any in-place editor
	if (mEditRow != -1)
		{
		SetFocus();
		// do not perform the default action. in case of multiedit the selection
		// must not change, in order to apply the new data
		return;
		}

	// test which cell was clicked
	if( CellHitTest(point, Row, Col, &rect) )
		OnCellRBtnDown(Row, Col, &rect, point);

	// perform default action
	CListCtrl::OnRButtonDown(nFlags, point);
	}

//
//	CGridCtrl::OnRClick
//
BOOL 
CGridCtrl::OnRClick(NMHDR* hdr, LRESULT* res)
	{
	// default message handle
	Default();

	int Row, Col;
	RECT rect;

	NMITEMACTIVATE *ia = (NMITEMACTIVATE*)hdr;
	CPoint point(ia->ptAction);

	// test which cell was clicked
	if( CellHitTest(point, Row, Col, &rect) )
		OnCellRBtnUp(Row, Col, &rect, point);

	return FALSE;
	}

//
//	CGridCtrl::OnKeyDown
//
void 
CGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	switch (nChar)
		{
		case VK_UP:
			SelectCell(mRow - 1, mCol);
			break;

		case VK_DOWN:
			SelectCell(mRow + 1, mCol);
			break;

		case VK_LEFT:
			if( SelectCell(mRow, mCol - 1, true) )
				DeselectAll();
			return;

		case VK_RIGHT:
			if( SelectCell(mRow, mCol + 1, true) )
				DeselectAll();
			return;

		case VK_HOME:
			if( HIBYTE(GetKeyState(VK_CONTROL)) )
				{
				CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

				// update focused row
				int OldRow = mRow;
				mRow = GetFocused();

				// redraw old row (for clearing the focus)
				if (OldRow != mRow)
					RedrawItem(OldRow);

				// redraw new row (for painting the focus);
				RedrawItems(mRow, false);
				return;
				}
			else
				{
				// jump to the first cell (in line)
				if( SelectCell(mRow, 0, true) )
					DeselectAll();
				}
			return;

		case VK_END:
			if( HIBYTE(GetKeyState(VK_CONTROL)) )
				{
				CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

				// update focused row
				int OldRow	= mRow;
				mRow		= GetFocused();

				// redraw old row (for clearing the focus)
				if( OldRow != mRow )
					RedrawItem(OldRow);

				// redraw new row (for painting the focus);
				RedrawItem(mRow, false);

				return;
				}
			else
				{
				// jump to the last cell (in line)
				if (SelectCell(mRow, GetColumnCount() - 1, true))
					DeselectAll();
				}
			return;

		case VK_PRIOR:
		case VK_NEXT:
			{
			CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

			// update focused row
			int OldRow	= mRow;
			mRow		= GetFocused();

			// redraw old row (for clearing the focus)
			if( OldRow != mRow )
				RedrawItem(OldRow);

			// redraw new row (for drawing the focus)
			RedrawItem(mRow, false);
			return;
			}

		case VK_SPACE:
			{
			if (mIncSearch && !mIncSearchString.IsEmpty())
				{
				OnIncrementalSearch((TCHAR)nChar);
				break;
				}

			EditCell(mRow, mCol);
			return;
			}

		case VK_F2:
			{
			EditCell(mRow, mCol);
			return;
			}
		}

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}

//
//	CGridCtrl::OnChar
//
void 
CGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	if( mIncSearch && nChar != VK_SPACE && _istprint(nChar))
		{
		OnIncrementalSearch((TCHAR)nChar);
		}
	}

//
//	CGridCtrl::EditCell
//
bool 
CGridCtrl::EditCell(	IN int Row,
						IN int Col,
						IN const RECT* CellRect OPTIONAL)
	{
	// ensure cell visibility
	VERIFY(EnsureVisible(Row, FALSE));
	if( EnsureColVisible(Col) != 0 )
		CellRect = NULL;

	// cell rectangle
	RECT rect;
	if( CellRect )
		::CopyRect(&rect, CellRect);
	else
		GetCellRect(Row, Col, rect);

	// make sure the cell rect does not exceed the listview
	ValidateCellRect(&rect);

	// handle each cell type
	CellType type = OnGetCellType(Row, Col);
	switch (type)
		{
		case CELL_REGULAR:
			break;

		case CELL_CHECKBOXON:
		case CELL_CHECKBOXOFF:
			{
			if (GetSelectedCount() == 1)
				{
				// reverse check state
				OnCheckBoxClick(Row, Col);
				return true;
				}
			else
				{
				// reverse check state of all selected cells
				int Index;
				CellType ct;

				POSITION pos = GetFirstSelectedItemPosition();
				while (pos)
					{
					Index = GetNextSelectedItem(pos);

					ct = OnGetCellType(Index, Col);
					if (ct == type)
						OnCheckBoxClick(Index, Col);
					}
				return true;
				}
			}

		case CELL_EDITBOX:
			{
			LVITEM item;
			item.mask		= LVIF_IMAGE;
			item.iItem		= Row;
			item.iSubItem	= Col;
			GetItem(&item);

			// jump over the icon
			if (item.iImage >= 0 && GetImageList(LVSIL_SMALL))
				rect.left += 16;
			return CreateInPlaceEdit(Row, Col, &rect);
			}

		case CELL_COMBOBOX:
			return CreateInPlaceCombo(Row, Col, &rect, false);

		case CELL_EDITCOMBOBOX:
			return CreateInPlaceCombo(Row, Col, &rect, true);

		case CELL_CUSTOMEDITOR:
			OnCustomCellClick(Row, Col);
			return true;
		}
	return false;
	}

//
//	[virtual] CGridCtrl::OnCellLBtnDown
//
bool 
CGridCtrl::OnCellLBtnDown(	IN int Row,
							IN int Col,
							IN const RECT *CellRect,
							IN const CPoint& Point)
{
	// reset the selection if the column was changed
	if( mCol != Col )
		{
		int temp	= mCol;
		mCol		= Col;
		DeselectAll();
		mCol		= temp;
		}

	// select the clicked cell
	if( !SelectCell(Row, Col, mRow == Row) &&
		!HIBYTE(GetKeyState(VK_CONTROL)) &&
		!HIBYTE(GetKeyState(VK_SHIFT)) )
		{
		// the cell was already selected. edit it!
		return EditCell(Row, Col, CellRect);
		}

	// no inplace editor was created
	return false;
	}

//
//	[virtual] CGridCtrl::OnCellLBtnUp
//
void 
CGridCtrl::OnCellLBtnUp(	IN int Row,
							IN int Col,
							IN const RECT *CellRect,
							IN const CPoint& Point)
	{
		// no action
	}

//
//	[virtual] CGridCtrl::OnCellDblClick
//
void 
CGridCtrl::OnCellDblClick(IN int Row, IN int Col, IN const RECT* CellRect)
	{
	// enter edit mode
	EditCell(Row, Col);
	}

//
//	[virtual] CGridCtrl::OnCellRBtnDown
//
void 
CGridCtrl::OnCellRBtnDown(	IN int Row,
							IN int Col,
							IN const RECT *CellRect,
							IN const CPoint& Point)
	{
	// select the clicked cell
	if (!HIBYTE(GetKeyState(VK_CONTROL)) && !HIBYTE(GetKeyState(VK_SHIFT)))
		{
		if( Col != mCol )
			{
			DeselectAll();
			SelectCell(Row, Col, true);
			}
		else
			SelectCell(Row, Col);
		}
	}

//
//	[virtual] CGridCtrl::OnCellRBtnUp
//
void 
CGridCtrl::OnCellRBtnUp(	IN int Row,
							IN int Col,
							IN const RECT *CellRect,
							IN const CPoint& Point)
	{
	// no action
	}

//
//	[virtual] CGridCtrl::OnCustomCellClick
//
void 
CGridCtrl::OnCustomCellClick(int Row, int Col)
	{
	// no action
	}

//
//	[virtual] CGridCtrl::OnCheckBoxClick
//
void 
CGridCtrl::OnCheckBoxClick(int Row, int Col)
	{
	// paint the new check state
	RedrawItem(Row);
	}

//
//	[virtual] CGridCtrl::OnGetComboBoxItems
//
void 
CGridCtrl::OnGetComboBoxItems(	IN  int Row,
								IN  int Col,
								OUT bool& HasImages,
								OUT CStringList& Items,
								OUT CArray<int, int>& Images)
	{
	// no action
	}

//
//	CGridCtrl::OnEndInPlaceEdit
//
bool 
CGridCtrl::OnEndInPlaceEdit(	IN int Row,
								IN int Col,
								IN bool Canceled,
								IN const TCHAR* Text,
								IN int Image)
	{
	if( !Canceled )
		{
		if( GetSelectedCount() == 1 )
			{
			// apply text
			SetItemText(Row, Col, Text);

			// apply image
			if (Image != -1 && GetImageList(LVSIL_SMALL))
				SetItem(Row, Col, LVIF_IMAGE, NULL, Image, 0, 0, 0);
			}
		else
			{
			CellType type = OnGetCellType(Row, Col), ct;
			ASSERT(type != CELL_REGULAR);

			int Index;
			POSITION pos = GetFirstSelectedItemPosition();
			while (pos)
				{
				Index = GetNextSelectedItem(pos);

				ct = OnGetCellType(Index, Col);
				if (type == ct)
					{
					// apply text
					SetItemText(Index, Col, Text);

					// apply image
					if (Image != -1 && GetImageList(LVSIL_SMALL))
						SetItem(Index, Col, LVIF_IMAGE, NULL, Image, 0, 0, 0);
					}
				}
			}
		}

	// allow the in-place editor be destroyed
	return true;
	}

//
//	[virtual] CGridCtrl::OnCommand
//
BOOL 
CGridCtrl::OnCommand(IN WPARAM wp, IN LPARAM lp)
	{
	// destroy in-place editor
	if (LOWORD(wp) == IDC_INPLACEEDIT)
	{
		unsigned short msg = HIWORD(wp);
		if( msg	== EN_KILLFOCUS	||
			msg	== EN_FINISH	||
			msg == CBN_KILLFOCUS )
			{
			static bool AlreadyValidating = false;

			// avoid reentrance
			if( AlreadyValidating ) 
				return TRUE;
			AlreadyValidating = true;

			// get inplace control data
			int		Image		= -1;
			long	UsrData		= GetWindowLong((HWND)lp, GWL_USERDATA);
			bool	Canceled	= (LOWORD(UsrData) == INPLACE_CANCEL);
			bool	ComboBox	= (HIWORD(UsrData) == INPLACE_COMBO);

			if( ComboBox )
				{
				// get selected index
				int Index = CB_ERR;

				HWND edit = (HWND)::SendMessage((HWND)lp, CBEM_GETEDITCONTROL, 0, 0);
				if (edit)
					{
					int len = (int)::SendMessage(edit, WM_GETTEXTLENGTH, 0, 0) + 1;
					ASSERT(len <= mBufferLen);
					::SendMessage(edit, WM_GETTEXT, len, (LPARAM)mBuffer);

					Index = (int)::SendMessage((HWND)lp, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)mBuffer);
					}
				else
					{
					Index = (int)::SendMessage((HWND)lp, CB_GETCURSEL, 0, 0);
					}

				// get item text and image index
				COMBOBOXEXITEM item;
				item.mask = CBEIF_TEXT | CBEIF_IMAGE;
				item.iItem = Index;
				item.pszText = mBuffer;
				item.cchTextMax = mBufferLen;
				::SendMessage((HWND)lp, CBEM_GETITEM, 0, (LPARAM)&item);

				Image = (Index != CB_ERR ? item.iImage : -1);
				}
			else
				{
				::GetWindowText((HWND)lp, mBuffer, mBufferLen);
				}

			// trigger the event
			if (OnEndInPlaceEdit(mEditRow, mEditCol, Canceled, mBuffer, Image))
				{
				// kill	the inplace editor
				mEditRow = mEditCol = -1;
				if (ComboBox)
					{
					// we cannot destroy the ComboBoxEx here, because its message
					// processing is pending.
					// instead, we schedule its destruction by posting a
					// 'suicide' message to its ComboBox child (which is
					// subclassed by us).
					// see CGridCtrl::ComboBoxProc for details
					HWND combo = (HWND)::SendMessage((HWND)lp, CBEM_GETCOMBOCONTROL, 0, 0);
					ASSERT(combo);
					VERIFY(::PostMessage(combo, CB_SUICIDE, 0, 0));
					}
				else
					{
					// kill'em all
					::DestroyWindow((HWND)lp);
					}
				SetFocus();
				}
			else
				{
				// do not kill the inplace editor
				::SetFocus((HWND)lp);
				}

			AlreadyValidating = false;
			return TRUE;
			}
		}

	return CListCtrl::OnCommand(wp, lp);
	}

//
//	[virtual] CGridCtrl::OnGetCellColors
//
bool 
CGridCtrl::OnGetCellColors(	IN  int Row,
							IN  int Col,
							OUT COLORREF& TextColor,
							OUT COLORREF& BkColor,
							OUT UINT8& TextStyle) const
{
	// use default colors
	return false;
}

bool 
CGridCtrl::OnGetCellBorders(	IN  int Row,
								IN  int Col,
								OUT COLORREF& crBorder,
								OUT UINT& nPenStyle,
								OUT UINT& nBorderStyle,
								OUT int& nWidth) const
	{
	// use no border.
	return false;
	}

//
//	[virtual] CGridCtrl::OnGetCellType
//
CellType 
CGridCtrl::OnGetCellType(IN int Row, IN int Col)
	{
	// regular cell, by default
	return CELL_REGULAR;
	}

//
//	[virtual] CGridCtrl::GetCellText
//
bool
CGridCtrl::OnGetCellText(IN int Row, IN int Col, CString& strCellText, IN LONG lRowParam)
	{
	return false;
	}

//
//	[virtual] CGridCtrl::OnGetCellTextAlign
//
bool
CGridCtrl::OnGetCellTextAlign(IN int Row,
							  IN int Col,
							  OUT int& nAlign)
	{
	return false;
	}

//
//	[virtual] CGridCtrl::OnChooseGridFont
//
bool
CGridCtrl::OnChooseGridFont(OUT LOGFONT* plf)
	{
	return false;
	}

//
//	[virtual] CGridCtrl::OnSetup
//
void
CGridCtrl::OnSetup()
	{
	}

//
//	CGridCtrl::OnSetFocus
//
void 
CGridCtrl::OnSetFocus(CWnd* pOldWnd)
	{
	CListCtrl::OnSetFocus(pOldWnd);
	RedrawItem(mRow);
	}

//
//	CGridCtrl::OnKillFocus
//
void 
CGridCtrl::OnKillFocus(CWnd* pNewWnd)
	{
	CListCtrl::OnKillFocus(pNewWnd);
	RedrawItem(mRow);
	}

//
//	[virtual] CGridCtrl::CreateInPlaceEdit
//
bool CGridCtrl::CreateInPlaceEdit(	IN int Row,
									IN int Col,
									IN const RECT *CellRect)
{
	// create an editbox
	HWND edit =	CreateWindow(	TEXT("EDIT"),
								NULL,				// no window title
								WS_CHILD | WS_VISIBLE |	ES_LEFT	| WS_BORDER	| ES_AUTOHSCROLL,
								CellRect->left,
								CellRect->top,
								RWIDTH(CellRect),
								RHEIGHT(CellRect),
								m_hWnd,				// parent window
								(HMENU)IDC_INPLACEEDIT,	// control ID
								(HINSTANCE)(UINT64)GetWindowLong(m_hWnd, GWL_HINSTANCE),
								NULL);				// pointer not needed

	if (!edit) return false;

	// subclass	editbox
	mOldChildProc = (WNDPROC)(UINT64)SetWindowLong(edit, GWL_WNDPROC, (LONG)(UINT64)EditBoxProc);

	// init	edit window
	::SetWindowLong(edit, GWL_USERDATA, MAKELONG(INPLACE_OK, INPLACE_EDIT));
	::SendMessage(edit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), FALSE);
	::SetFocus(edit);
	::SendMessage(edit, EM_LIMITTEXT, mEditLen, 0);

	// set txt
	GetItemText(Row, Col, mBuffer, mBufferLen);
	::SendMessage(edit, WM_SETTEXT, 0, (LPARAM)mBuffer);
	::SendMessage(edit, EM_SETSEL, 0, -1);

	// turn on the child editor
	mEditRow = Row, mEditCol = Col;

	return true;
}

//
//	[virtual] CGridCtrl::CreateInPlaceCombo
//
bool CGridCtrl::CreateInPlaceCombo(	IN int Row,
									IN int Col,
									IN const RECT *CellRect,
									IN bool Editable)
{
	// get combo box items
	bool HasImages = false;
	CStringList Items;
	CArray<int, int> Images;
	OnGetComboBoxItems(Row, Col, HasImages, Items, Images);
	ASSERT(!HasImages || (HasImages && (Items.GetCount() == Images.GetSize())));

	// create an editbox
	HWND ComboEx = CreateWindowEx(	0,
									WC_COMBOBOXEX,
									NULL,				// no window title
									WS_CHILD | WS_VISIBLE |	ES_LEFT	| WS_BORDER	| CBS_AUTOHSCROLL | (Editable ? CBS_DROPDOWN : CBS_DROPDOWNLIST),
									CellRect->left,
									CellRect->top - 1,
									0,
									204,				// dropped-down list height
									m_hWnd,				// parent window
									(HMENU)IDC_INPLACEEDIT,	// control ID
									(HINSTANCE)(UINT64)GetWindowLong(m_hWnd, GWL_HINSTANCE),
									NULL);				// pointer not needed

	if (!ComboEx) return false;

	// set the image list
	if (HasImages && GetImageList(LVSIL_SMALL))
		::SendMessage(ComboEx, CBEM_SETIMAGELIST, 0, (LPARAM)GetImageList(LVSIL_SMALL)->m_hImageList);

	// resize the combo box to its final dimensions
	::SendMessage(ComboEx, CB_SETITEMHEIGHT, (WPARAM)-1, RHEIGHT(CellRect) - 4);	// adjust this value if the combo is too small
	::SetWindowPos(	ComboEx,
					NULL,
					CellRect->left, CellRect->top - 1,
					RWIDTH(CellRect), 0,
					SWP_NOACTIVATE);

	// subclass	combo box
	HWND ComboBox = (HWND)::SendMessage(ComboEx, CBEM_GETCOMBOCONTROL, 0, 0);
	mOldChildProc = (WNDPROC)(UINT64)SetWindowLong(ComboBox, GWL_WNDPROC, (LONG)(UINT64)ComboBoxProc);

	// subclass its edit box, too (for intercepting ENTER and ESCAPE)
	HWND edit = (HWND)::SendMessage(ComboEx, CBEM_GETEDITCONTROL, 0, 0);
	if (Editable)
	{
		ASSERT(edit);
		mOldComboEditProc = (WNDPROC)(UINT64)SetWindowLong(edit, GWL_WNDPROC, (LONG)(UINT64)ComboEditProc);
		::SendMessage(edit, EM_LIMITTEXT, mEditLen, 0);
	}

	// init	the combo box
	::SetWindowLong(ComboEx, GWL_USERDATA, MAKELONG(INPLACE_OK, INPLACE_COMBO));
	//::SendMessage(ComboBox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), FALSE);
	::SetFocus(ComboEx);
	::SendMessage(ComboEx, CB_SETDROPPEDWIDTH, 70, 0);
	
	// populate the combo box
	COMBOBOXEXITEM cbei;
	cbei.mask = CBEIF_TEXT | (HasImages ? CBEIF_IMAGE | CBEIF_SELECTEDIMAGE : 0);

	int i = 0;
	for (POSITION pos = Items.GetHeadPosition(); pos != NULL; i++)
	{
		if (HasImages)
		{
			cbei.iImage = Images[i];
			cbei.iSelectedImage = Images[i];
		}

		cbei.iItem = i;
		cbei.pszText = Items.GetNext(pos).GetBuffer(0);
		::SendMessage(ComboEx, CBEM_INSERTITEM, 0, (LPARAM)&cbei);
	}

	// set initial text/selection
	GetItemText(Row, Col, mBuffer, mBufferLen);
	int index = (int)::SendMessage(ComboEx, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)mBuffer);
	if (index != CB_ERR)
		::SendMessage(ComboEx, CB_SETCURSEL, index, 0);
	else if (Editable)
	{
		::SendMessage(edit, WM_SETTEXT, 0, (LPARAM)mBuffer);
		::SendMessage(edit, EM_SETSEL, 0, -1);
	}

	// drop it down!
	::SendMessage(ComboEx, CB_SHOWDROPDOWN, TRUE, 0);

	// turn on the child editor
	mEditRow = Row, mEditCol = Col;

	return true;
}

//
//	CGridCtrl::EnsureColVisible
//
int	CGridCtrl::EnsureColVisible(IN int Col)
{
	ASSERT(IsWindow(m_hWnd));
	RECT rect;
	
	// get X of	the	first column
	if (!GetItemRect(0, &rect, LVIR_BOUNDS)) return 0;
	int	x =	rect.left;
	
	// get client rect
	GetClientRect(&rect);

	// get X of	our	column
	for	(int c = 0;	c <	Col; c++)
		x += GetColumnWidth(c);
		
	// get our column's	width
	int	w =	GetColumnWidth(Col);

	// is the column already visible?
	if (x >= rect.left && x	+ w	<= rect.right)
		return 0;

	// ensure visibility
	int	scroll_to_start	= x	- rect.left;
	int	scroll_to_end =	x +	w -	rect.right;
	if (w >	RHEIGHT(&rect) ||						// if the column is	larger than	the	view, always scroll	to its start position
		abs(scroll_to_start) < abs(scroll_to_end))	// optimal scroll (to column start or end position)
	{
		Scroll(CSize(scroll_to_start, 0));
		return scroll_to_start;
	}
	else
	{
		Scroll(CSize(scroll_to_end, 0));
		return scroll_to_end;
	}
}

//
//	CGridCtrl::CellHitTest
//
BOOL CGridCtrl::CellHitTest(IN	const CPoint& Pt,
							OUT	int &Row,
							OUT	int	&Col,
							OUT	RECT *Rect OPTIONAL)
{
	ASSERT(IsWindow(m_hWnd));

	// detect which	item was hit
	int	top	= GetTopIndex();
	int	btm	= min(top + GetCountPerPage(), GetItemCount());
								
	RECT _rect = {0, 0, 0, 0};
	for	(Row = top; Row < btm; Row++)
	{
		GetItemRect(Row, &_rect, LVIR_BOUNDS);
		if ((Pt.y >= _rect.top) && (Pt.y < _rect.bottom))
			break;
	}

	// exit	if no item was hit
	if (Row >= btm) return(FALSE);

	// exit	if no column was hit
	if (Pt.x > _rect.right) return(FALSE);

	// detect which	column was hit
	Col = 0;
	LV_COLUMN lvc;
	lvc.mask = LVCF_WIDTH;
	while (GetColumn(Col, &lvc))
	{
		if (Pt.x < _rect.left + lvc.cx)
		{
			_rect.right	= _rect.left + lvc.cx;
			break;
		}

		_rect.left += lvc.cx, Col++;
	}

	// return cell rectangle
	if (Rect)
		CopyRect(Rect, &_rect);
	
	return(TRUE);
}

//
//	CGridCtrl::ValidateCellRect
//
void CGridCtrl::ValidateCellRect(IN OUT RECT* rect)
{
	ASSERT(rect);
	ASSERT(IsWindow(m_hWnd));

	RECT client, r;
	GetClientRect(&client);
	CopyRect(&r, rect);

	IntersectRect(rect, &client, &r);
}

//
//	CGridCtrl::GetCellRect
//
bool CGridCtrl::GetCellRect(IN  int Row,
							IN  int Col,
							OUT RECT& Rect)
{
	// valid arguments?
	ASSERT(IsWindow(m_hWnd));
	ASSERT(Row >= 0 && Row < GetItemCount() && Col >= 0 && Col < GetColumnCount());

	// treat "LVS_EX_GRIDLINES"
	UINT8 spacing = HasGridLines() ? (UINT8)1 : (UINT8)0;

	// get the whole item rect
	if (!GetItemRect(Row, &Rect, LVIR_BOUNDS))
		return false;

	// set left/right
	Rect.left = Rect.right = 0;
	for (int c = 0; c <= Col; c++)
	{
		Rect.left = Rect.right;
		Rect.right += GetColumnWidth(c);
	}

	Rect.left += spacing;
	Rect.bottom -= spacing;

	::OffsetRect(&Rect, -GetScrollPos(SB_HORZ), 0);

	return true;
}

//
//	CGridCtrl::GetFocused
//
int CGridCtrl::GetFocused() const
{
/*	POSITION pos = GetFirstSelectedItemPosition();
	if (!pos) return -1;

	int Index;
	while (pos)
	{
		Index = GetNextSelectedItem(pos);
		if (GetItemState(Index, LVIS_FOCUSED) != 0)
			return Index;
	}*/
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
		if (GetItemState(i, LVIS_FOCUSED) != 0)
			return i;
	return -1;
}

//
//	CGridCtrl::SelectCell
//
bool CGridCtrl::SelectCell(IN int Row, IN int Col, IN bool DoRedraw)
{
	if (Row == mRow &&
		Col == mCol &&
		GetItemState(Row, LVIS_SELECTED) != 0) return false;
	
	if (Row < 0 || Row >= GetItemCount() ||
		Col < 0 || Col >= GetColumnCount()
		)
		return false;

	// backup current row
	int OldCurRow = mRow;

	// update current cell
	mRow = Row;
	mCol = Col;

	// ensure cell visibility
	VERIFY(EnsureVisible(Row, FALSE));
	EnsureColVisible(Col);

	if (DoRedraw)
	{
		// redraw old cell (for clearing its focus)
		RedrawItem(OldCurRow);

		// redraw current cell (for paining its focus)
		if (Row != OldCurRow)
			RedrawItem(Row, false);
	}

	return true;
}

//
//	CGridCtrl::DeselectAll
//
void 
CGridCtrl::DeselectAll()
	{
	int Index;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
		{
		Index = GetNextSelectedItem(pos);
		if (Index != mRow)
			SetItemState(Index, 0, LVIS_SELECTED);
		}
	SetSelectionMark(mRow);
	SetItemState	(mRow, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}

//
//	CGridCtrl::OnCustomDraw
//
BOOL 
CGridCtrl::OnCustomDraw(IN NMHDR* Header, OUT LRESULT* Result)
	{
	static LVITEM		Item;
	static RECT			rcItem, rcClient;
	static int			nCellOffset, nScrollPos;
	static UINT8		nSpacing;
	static int			nRow, nCol;

	static CImageList*	il	= NULL;
	mDraw.cd				= reinterpret_cast<NMLVCUSTOMDRAW*>(Header);

	switch( mDraw.cd->nmcd.dwDrawStage )
		{
		case CDDS_PREPAINT:
			// request ITEM notification
			*Result			= CDRF_NOTIFYITEMDRAW;
			// cache the small image list
			il				= GetImageList(LVSIL_SMALL);
			// treat "LVS_EX_GRIDLINES"
			nSpacing		= (HasGridLines() ? 1 : 0);
			// prepare LVITEM
			Item.pszText	= mBuffer;
			Item.cchTextMax = mBufferLen;
			Item.mask		= LVIF_IMAGE | LVIF_TEXT;

			// painting preparations
			GetClientRect(&rcClient);
			mDraw.CellText	= mBuffer;
			mDraw.GridLines = HasGridLines();

			::SetBkMode(DC, TRANSPARENT);
			break;

		case CDDS_ITEMPREPAINT:
			// request SUBITEM notification
			*Result			= CDRF_NOTIFYSUBITEMDRAW;
			// cache item rect
			GetItemRect(ROW, &rcItem, LVIR_BOUNDS);
			// init
			nCellOffset		= 0;
			nScrollPos		= GetScrollPos(SB_HORZ);
			Item.iItem		= ROW;
			break;

		case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
			{
			// it might happen that the first subitem drawn has a wrong index (>0) (double click on column header to automatically resize)
			// in this case the index will be corrected (windows bug??)
			if( nCellOffset == 0 && COL != 0)
				COL = 0;

			mDraw.Icon			= -1;
			nRow				= ROW;
			nCol				= COL;

			static LVCOLUMN lvc;
			lvc.mask			= LVCF_FMT;
			GetColumn(nCol, &lvc);
			mDraw.Justify		= (UINT8)(lvc.fmt & LVCFMT_JUSTIFYMASK);

			// compute cell rect (not using GetCellRect() for extra performance)
			mDraw.Rect.top		= rcItem.top;
			mDraw.Rect.bottom	= rcItem.bottom;

			mDraw.Rect.left		= nCellOffset - nScrollPos;
			nCellOffset			+= GetColumnWidth(nCol);

			if( nCol > 0 )
				mDraw.Rect.left += nSpacing;

			mDraw.Rect.bottom	-= nSpacing;
			mDraw.Rect.right	= nCellOffset - nScrollPos;

			// do not draw the cell if it's not visible
			if( mDraw.Rect.right < rcClient.left ||
				mDraw.Rect.left > rcClient.right )
				{
				*Result = CDRF_SKIPDEFAULT;
				break;
				}

			// get cell info
			Item.iSubItem	= nCol;
			GetItem(&Item);
			mDraw.Type		= OnGetCellType(nRow, nCol);

			// draw the icon
			if( mDraw.Type != CELL_SEPARATOR && il && Item.iImage >= 0 )
				{
				mDraw.Icon = Item.iImage;

				// erase icon background
				mDraw.Rect.right = mDraw.Rect.left + 17;
				::FillRect(DC, &mDraw.Rect, mDraw.BrushNormalFill);

				// draw the icon
				ImageList_Draw(	il->m_hImageList,
								Item.iImage,
								DC,
								mDraw.Rect.left,
								mDraw.Rect.top,
								ILD_TRANSPARENT);

				// update remaining cell rect (jump over the icon)
				mDraw.Rect.left += 17;
				}
			mDraw.Rect.right = nCellOffset - nScrollPos;

			// draw the label
			if( RWIDTH(&mDraw.Rect) > 1 )
				{
				
				mDraw.Focused		= FALSE;//(nRow == mRow && /*nCol == mCol &&*/ GetFocus() == this);
				if( m_bCellMode )
					mDraw.Selected	= (nCol == mCol && GetItemState(nRow, LVIS_SELECTED) != 0);
				else
					mDraw.Selected	= (/*nCol == mCol &&*/ GetItemState(nRow, LVIS_SELECTED) != 0);
				
				switch (mDraw.Type)
					{
					case CELL_REGULAR:
						DrawRegularCell();
						break;

					case CELL_EDITBOX:
						DrawEditBoxCell();
						break;

					case CELL_COMBOBOX:
					case CELL_EDITCOMBOBOX:
						DrawComboBoxCell();
						break;

					case CELL_CHECKBOXON:
					case CELL_CHECKBOXOFF:
						DrawCheckBoxCell();
						break;

					case CELL_CUSTOMEDITOR:
						DrawCustomCell();
						break;

					case CELL_SEPARATOR:
						DrawSeparatorCell();
						break;
					}

				*Result = CDRF_SKIPDEFAULT;
				break;
				}
			}

		default:
			*Result = CDRF_DODEFAULT;
		}
	return TRUE;
	}

//
//	CGridCtrl::GenerateFont
//
void 
CGridCtrl::GenerateFont(IN UINT8 Style)
	{
	// check whether the font is already cached
	if( mDraw.CustomFontStyle == Style && mDraw.CustomFont != NULL )
		return;

	// destroy previous cache font
	if( mDraw.CustomFont )
		::DeleteObject(mDraw.CustomFont);

	// read current font's metrics
	static TEXTMETRIC metrix;
	GetTextMetrics(DC, &metrix);

	static LOGFONT lf;
	if( m_pFont  )
		m_pFont->GetLogFont(&lf);
	else
		::GetTextFace(DC, LF_FACESIZE, lf.lfFaceName);

	//_tcscpy(lf.lfFaceName, TEXT("Tahoma"));
	lf.lfCharSet		= metrix.tmCharSet;
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lf.lfEscapement		= 0;
	lf.lfHeight			= metrix.tmHeight;
	lf.lfItalic			= (Style & FONT_ITALIC) != 0 ? TRUE : FALSE;
	lf.lfOrientation	= 0;
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lf.lfPitchAndFamily = metrix.tmPitchAndFamily;
	lf.lfQuality		= DEFAULT_QUALITY;//PROOF_QUALITY;
	lf.lfStrikeOut		= (Style & FONT_STRIKEOUT) != 0 ? TRUE : FALSE;
	lf.lfUnderline		= (Style & FONT_UNDERLINE) != 0 ? TRUE : FALSE;
	lf.lfWeight			= (Style & FONT_BOLD) != 0 ? FW_BOLD : FW_NORMAL;
	lf.lfWidth			= 0;

	mDraw.CustomFont = CreateFontIndirect(&lf);
	ASSERT(mDraw.CustomFont);
	mDraw.CustomFontStyle = Style;
}

//
//	[virtual] CGridCtrl::DrawRegularCell
//
void __fastcall 
CGridCtrl::DrawRegularCell()
	{
	// query for local colors
	static COLORREF clText, clBk, clDefBk;
	static UINT8	TxtStyle;
	static HFONT	oldf;
	static HBRUSH	brush;
	static bool		KillBrush;

	TxtStyle = 0, oldf = NULL, KillBrush = false;

	// prepare default colors
	if( mDraw.Selected )
		{
		brush	= mDraw.BrushSelectedFill;
		clDefBk = clBk = mDraw.ColorSelectedFill;
		clText	= mDraw.ColorSelText;
		}
	else
		{
		brush	= mDraw.BrushNormalFill;
		clDefBk = clBk = mDraw.ColorNormalFill;
		clText	= mDraw.ColorText;
		}

	COLORREF	crBorder;
	UINT		nPenStyle;
	UINT		nBorderStyle;
	int			nWidth;

	CRect rcOffset(0, 0, 0, 0);
	if( OnGetCellBorders(ROW, COL, crBorder, nPenStyle, nBorderStyle, nWidth) )
		{
		CPen	penBorder(nPenStyle, nWidth, crBorder);
		CDC*	pDC		= CDC::FromHandle(DC);
		CPen*	pOldPen = pDC->SelectObject(&penBorder);

		// Draw cell borders. ##############################
		if( nBorderStyle&BORDER_STYLE_LEFT )
			{
			rcOffset.left = nWidth;
			pDC->MoveTo(mDraw.Rect.left, mDraw.Rect.top);
			pDC->LineTo(mDraw.Rect.left, mDraw.Rect.bottom);
			}

		if( nBorderStyle&BORDER_STYLE_TOP )
			{
			rcOffset.top = nWidth;
			pDC->MoveTo(mDraw.Rect.left, mDraw.Rect.top);
			pDC->LineTo(mDraw.Rect.right, mDraw.Rect.top);
			}

		if( nBorderStyle&BORDER_STYLE_RIGHT )
			{
			rcOffset.right = nWidth;
			pDC->MoveTo(mDraw.Rect.right-1, mDraw.Rect.top);
			pDC->LineTo(mDraw.Rect.right-1, mDraw.Rect.bottom);
			}

		if( nBorderStyle&BORDER_STYLE_BOTTOM )
			{
			rcOffset.bottom = nWidth;
			pDC->MoveTo(mDraw.Rect.right-1, mDraw.Rect.bottom-1);
			pDC->LineTo(mDraw.Rect.left,  mDraw.Rect.bottom-1);
			}
		// ##################################################
		pDC->SelectObject(pOldPen);
		}

	// query for color override
	if( OnGetCellColors(ROW, COL, clText, clBk, TxtStyle) )
		{
		// brush
		if( clBk != clDefBk )
			{
			brush = ::CreateSolidBrush(clBk);
			KillBrush = true;
			}

		// font
		if( TxtStyle != 0 )
			{
			GenerateFont(TxtStyle);
			oldf = (HFONT)::SelectObject(DC, mDraw.CustomFont);
			}
		}

	// background
	CRect rcFill	 = mDraw.Rect;
	rcFill.left		+= rcOffset.left;
	rcFill.top		+= rcOffset.top;
	rcFill.right	-= rcOffset.right;
	rcFill.bottom	-= rcOffset.bottom;

	::FillRect(DC, rcFill, brush);
	if( KillBrush ) 
		::DeleteObject(brush);

	// text
	::SetTextColor(DC, clText);

	// focus
	if( mDraw.Focused )
		{
		::SetBkColor(DC, clBk);
		DrawFocusRect(DC, &mDraw.Rect);
		}

	//GetCellText
	CString sCellText = mDraw.CellText;
	OnGetCellText(ROW, COL, sCellText, (LONG)mDraw.cd->nmcd.lItemlParam/*row param value*/);

	// text
	::InflateRect	(&mDraw.Rect, -2, 0);

	// Retrieve cell text align. #########
	int nCellAlign = 0;
	if( !OnGetCellTextAlign(ROW, COL, nCellAlign) )
		nCellAlign = DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | (mDraw.Justify & LVCFMT_RIGHT ? DT_RIGHT : (mDraw.Justify & LVCFMT_CENTER ? DT_CENTER : DT_LEFT));
	// ###################################

	::DrawText(DC, sCellText, sCellText.GetLength(), &mDraw.Rect, nCellAlign);
	if( oldf ) 
		::SelectObject(DC, oldf);
	}

//
//	[virtual] CGridCtrl::DrawEditBoxCell
//
void __fastcall 
CGridCtrl::DrawEditBoxCell()
	{
	if( !mDraw.GridLines )
		{
		mDraw.Rect.bottom --;
		mDraw.Rect.right  --;

		static RECT r;
		::SetRect(&r, mDraw.Rect.left, mDraw.Rect.bottom, mDraw.Rect.right, mDraw.Rect.bottom);
		::FrameRect(DC, &r, mDraw.BrushNormalFill);
		::SetRect(&r, mDraw.Rect.right, mDraw.Rect.top - 1, mDraw.Rect.right, mDraw.Rect.bottom + 2);
		::FrameRect(DC, &r, mDraw.BrushNormalFill);
		}

	// frame
	::FrameRect		(DC, &mDraw.Rect, mDraw.BrushFrame);
	::InflateRect	(&mDraw.Rect, -1, -1);

	// query for local colors
	static COLORREF clText, clBk, clDefBk;
	static UINT8	TxtStyle;
	static HFONT	oldf;
	static HBRUSH	brush;
	static bool		KillBrush;
	TxtStyle = 0, oldf = NULL, KillBrush = false;

	// prepare default colors
	if( mDraw.Selected )
		{
		brush	= mDraw.BrushSelectedFill;
		clDefBk = clBk = mDraw.ColorSelectedFill;
		clText	= mDraw.ColorSelText;
		}
	else
		{
		brush	= mDraw.BrushNormalFill;
		clDefBk = clBk = mDraw.ColorNormalFill;
		clText	= mDraw.ColorText;
		}

	// query for color override
	if( OnGetCellColors(ROW, COL, clText, clBk, TxtStyle) )
		{
		// brush
		if( clBk != clDefBk )
			{
			brush = ::CreateSolidBrush(clBk);
			KillBrush = true;
			}

		// font
		if( TxtStyle != 0 )
			{
			GenerateFont(TxtStyle);
			oldf = (HFONT)::SelectObject(DC, mDraw.CustomFont);
			}
		}

	// background
	::FillRect(DC, &mDraw.Rect, brush);
	if( KillBrush ) 
		::DeleteObject(brush);

	// text
	::SetTextColor(DC, clText);

	// focus
	if( mDraw.Focused )
		{
		::SetBkColor(DC, clBk);
		DrawFocusRect(DC, &mDraw.Rect);
		}

	// text
	::InflateRect	(&mDraw.Rect, -2, 0);
	::DrawText		(DC, mDraw.CellText, (int)_tcslen(mDraw.CellText), &mDraw.Rect, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|(mDraw.Justify & LVCFMT_RIGHT ? DT_RIGHT : (mDraw.Justify & LVCFMT_CENTER ? DT_CENTER : DT_LEFT)));
	if( oldf ) 
		::SelectObject(DC, oldf);
	}

//
//	[virtual] CGridCtrl::DrawComboBoxCell
//
void __fastcall 
CGridCtrl::DrawComboBoxCell()
	{
	if( !mDraw.GridLines )
		{
		mDraw.Rect.bottom--;
		mDraw.Rect.right --;

		static RECT r;
		::SetRect	(&r, mDraw.Rect.left, mDraw.Rect.bottom, mDraw.Rect.right, mDraw.Rect.bottom);
		::FrameRect	(DC, &r, mDraw.BrushNormalFill);
		::SetRect	(&r, mDraw.Rect.right, mDraw.Rect.top - 1, mDraw.Rect.right, mDraw.Rect.bottom + 2);
		::FrameRect	(DC, &r, mDraw.BrushNormalFill);
		}

	mDraw.Rect.right -= COMBO_BTNWIDTH;

	// combo button
	int MinOffset = mDraw.Rect.right < mDraw.Rect.left ? mDraw.Rect.left - mDraw.Rect.right : 0;
	::BitBlt(	DC,
				mDraw.Rect.right + MinOffset, mDraw.Rect.top,
				COMBO_BTNWIDTH, RHEIGHT(&mDraw.Rect),
				mDraw.ComboDC,
				MinOffset, 0,
				SRCCOPY);

	// is there room for text?
	if( MinOffset > 0 ) 
		return;

	// frame
	::FrameRect		(DC, &mDraw.Rect, mDraw.BrushFrame);
	::InflateRect	(&mDraw.Rect, -1, -1);

	// query for local colors
	static COLORREF clText, clBk, clDefBk;
	static UINT8	TxtStyle;
	static HFONT	oldf;
	static HBRUSH	brush;
	static bool		KillBrush;
	TxtStyle = 0, oldf = NULL, KillBrush = false;

	// prepare default colors
	if( mDraw.Selected )
		{
		brush	= mDraw.BrushSelectedFill;
		clDefBk = clBk = mDraw.ColorSelectedFill;
		clText	= mDraw.ColorSelText;
		}
	else
		{
		brush	= mDraw.BrushNormalFill;
		clDefBk = clBk = mDraw.ColorNormalFill;
		clText	= mDraw.ColorText;
		}

	// query for color override
	if( OnGetCellColors(ROW, COL, clText, clBk, TxtStyle) )
		{
		// brush
		if( clBk != clDefBk )
			{
			brush = ::CreateSolidBrush(clBk);
			KillBrush = true;
			}

		// font
		if( TxtStyle )
			{
			GenerateFont(TxtStyle);
			oldf = (HFONT)::SelectObject(DC, mDraw.CustomFont);
			}
		}

	// background
	::FillRect(DC, &mDraw.Rect, brush);
	if( KillBrush ) 
		::DeleteObject(brush);

	// text
	::SetTextColor(DC, clText);

	// focus
	if( mDraw.Focused )
		{
		::SetBkColor(DC, clBk);
		DrawFocusRect(DC, &mDraw.Rect);
		}

	// text
	::InflateRect	(&mDraw.Rect, -2, 0);
	::DrawText		(DC, mDraw.CellText, (int)_tcslen(mDraw.CellText), &mDraw.Rect, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | (mDraw.Justify & LVCFMT_RIGHT ? DT_RIGHT : (mDraw.Justify & LVCFMT_CENTER ? DT_CENTER : DT_LEFT)));
	if( oldf ) 
		::SelectObject(DC, oldf);
	}

//
//	[virtual] CGridCtrl::DrawCheckBoxCell
//
void __fastcall 
CGridCtrl::DrawCheckBoxCell()
	{
	// check box mark
	::BitBlt(	DC,
				mDraw.Rect.left, mDraw.Rect.top,
				CHECK_BTNWIDTH, RHEIGHT(&mDraw.Rect),
				mDraw.Type == CELL_CHECKBOXON ? mDraw.CheckOnDC : mDraw.CheckOffDC,
				0, 0,
				SRCCOPY);

	mDraw.Rect.left += CHECK_BTNWIDTH;

	// query for local colors
	static COLORREF clText, clBk, clDefBk;
	static UINT8	TxtStyle;
	static HFONT	oldf;
	static HBRUSH	brush;
	static bool		KillBrush;
	TxtStyle = 0, oldf = NULL, KillBrush = false;

	// prepare default colors
	if( mDraw.Selected )
		{
		brush	= mDraw.BrushSelectedFill;
		clDefBk = clBk = mDraw.ColorSelectedFill;
		clText	= mDraw.ColorSelText;
		}
	else
		{
		brush	= mDraw.BrushNormalFill;
		clDefBk = clBk = mDraw.ColorNormalFill;
		clText	= mDraw.ColorText;
		}

	// query for color override
	if( OnGetCellColors(ROW, COL, clText, clBk, TxtStyle) )
		{
		// brush
		if( clBk != clDefBk )
			{
			brush		= ::CreateSolidBrush(clBk);
			KillBrush	= true;
			}

		// font
		if( TxtStyle )
			{
			GenerateFont(TxtStyle);
			oldf = (HFONT)::SelectObject(DC, mDraw.CustomFont);
			}
		}

	// background
	::FillRect(DC, &mDraw.Rect, brush);
	if( KillBrush ) 
		::DeleteObject(brush);

	// text
	::SetTextColor(DC, clText);

	// focus
	if( mDraw.Focused )
		{
		::SetBkColor(DC, clBk);
		DrawFocusRect(DC, &mDraw.Rect);
		}

	// text
	::InflateRect	(&mDraw.Rect, -2, 0);
	::DrawText		(DC, mDraw.CellText, (int)_tcslen(mDraw.CellText), &mDraw.Rect, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|(mDraw.Justify & LVCFMT_RIGHT ? DT_RIGHT : (mDraw.Justify & LVCFMT_CENTER ? DT_CENTER : DT_LEFT)));
	if( oldf ) 
		::SelectObject(DC, oldf);
	}

//
//	[virtual] CGridCtrl::DrawCustomCell
//
void __fastcall 
CGridCtrl::DrawCustomCell()
	{
	if( !mDraw.GridLines )
		{
		mDraw.Rect.bottom	--;
		mDraw.Rect.right	--;

		static RECT r;
		::SetRect	(&r, mDraw.Rect.left, mDraw.Rect.bottom, mDraw.Rect.right, mDraw.Rect.bottom);
		::FrameRect	(DC, &r, mDraw.BrushNormalFill);
		::SetRect	(&r, mDraw.Rect.right, mDraw.Rect.top - 1, mDraw.Rect.right, mDraw.Rect.bottom + 2);
		::FrameRect	(DC, &r, mDraw.BrushNormalFill);
	}

	mDraw.Rect.right -= CUSTOM_BTNWIDTH;

	// ellipsis button
	int MinOffset = mDraw.Rect.right < mDraw.Rect.left ? mDraw.Rect.left - mDraw.Rect.right : 0;
	::BitBlt(	DC,
				mDraw.Rect.right + MinOffset, mDraw.Rect.top,
				CUSTOM_BTNWIDTH, RHEIGHT(&mDraw.Rect),
				mDraw.CustomDC,
				MinOffset, 0,
				SRCCOPY);

	// is there room for text?
	if( MinOffset > 0 ) 
		return;

	// frame
	::FrameRect		(DC, &mDraw.Rect, mDraw.BrushFrame);
	::InflateRect	(&mDraw.Rect, -1, -1);

	// query for local colors
	static COLORREF clText, clBk, clDefBk;
	static UINT8	TxtStyle;
	static HFONT	oldf;
	static HBRUSH	brush;
	static bool		KillBrush;

	TxtStyle = 0, oldf = NULL, KillBrush = false;

	// prepare default colors
	if( mDraw.Selected )
		{
		brush	= mDraw.BrushSelectedFill;
		clDefBk = clBk = mDraw.ColorSelectedFill;
		clText	= mDraw.ColorSelText;
		}
	else
		{
		brush	= mDraw.BrushNormalFill;
		clDefBk = clBk = mDraw.ColorNormalFill;
		clText	= mDraw.ColorText;
		}

	// query for color override
	if( OnGetCellColors(ROW, COL, clText, clBk, TxtStyle) )
		{
		// brush
		if( clBk != clDefBk )
			{
			brush = ::CreateSolidBrush(clBk);
			KillBrush = true;
			}

		// font
		if( TxtStyle )
			{
			GenerateFont(TxtStyle);
			oldf = (HFONT)::SelectObject(DC, mDraw.CustomFont);
			}
		}

	// background
	::FillRect(DC, &mDraw.Rect, brush);
	if( KillBrush ) 
		::DeleteObject(brush);

	// text
	::SetTextColor(DC, clText);

	// focus
	if( mDraw.Focused )
		{
		::SetBkColor(DC, clBk);
		DrawFocusRect(DC, &mDraw.Rect);
		}

	// text
	::InflateRect	(&mDraw.Rect, -2, 0);
	::DrawText		(DC, mDraw.CellText, (int)_tcslen(mDraw.CellText), &mDraw.Rect, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | (mDraw.Justify & LVCFMT_RIGHT ? DT_RIGHT : (mDraw.Justify & LVCFMT_CENTER ? DT_CENTER : DT_LEFT)));
	if( oldf ) 
		::SelectObject(DC, oldf);
	}

//
//	[virtual]
//
void 
CGridCtrl::DrawSeparatorCell()
	{
	COLORREF	BkColor, TextColor = mDraw.ColorText;
	UINT8		TextStyle	= 0;
	BkColor					= mDraw.ColorNormalFill;
	HBRUSH		brush		= mDraw.BrushNormalFill;
	bool		KillBrush	= false;

	if( OnGetCellColors(ROW, COL, TextColor, BkColor, TextStyle) &&
		BkColor != mDraw.ColorNormalFill )
		{
		brush = ::CreateSolidBrush(BkColor);
		KillBrush = true;
		}

	::FillRect(DC, &mDraw.Rect, brush);
	if( KillBrush ) 
		::DeleteObject(brush);

	HPEN	pen		= ::CreatePen(PS_SOLID, 0, TextColor);
	HPEN	oldp	= (HPEN)::SelectObject(DC, pen);
	int		y		= MiddleInt(mDraw.Rect.top, mDraw.Rect.bottom);

	::MoveToEx		(DC, mDraw.Rect.left, y, NULL);
	::LineTo		(DC, mDraw.Rect.right, y);
	::DeleteObject	(::SelectObject(DC, oldp));
	}

//
//	CGridCtrl::OnEraseBkgnd
//
BOOL 
CGridCtrl::OnEraseBkgnd(CDC *dc)
	{
	static	RECT rect, clrect;
	int		Items = GetItemCount();

	if( Items > 0 )
		{
		GetClientRect(&clrect);

		GetItemRect(GetTopIndex(), &rect, LVIR_BOUNDS);
		if( rect.top > 0 )
			{
			rect.bottom = rect.top;
			rect.top	= 0;
			rect.left	= clrect.left;
			rect.right	= clrect.right;
			::FillRect(dc->m_hDC, &rect, mDraw.BrushNormalFill);
			}

		// is the last item visible?
		if( GetTopIndex() + GetCountPerPage() >= Items )
			{
			// draw the (whitespace) area NOT covered by items
			GetItemRect(GetItemCount() - 1, &rect, LVIR_BOUNDS);
			int Bottom	= rect.bottom;

			GetClientRect(&rect);
			rect.top	= Bottom;
			::FillRect(dc->m_hDC, &rect, GetSysColorBrush(COLOR_BTNFACE));
			}
		}
	else
		return CListCtrl::OnEraseBkgnd(dc);
	return TRUE;
	}

//
//	CGridCtrl::OnSysColorChange
//
void 
CGridCtrl::OnSysColorChange()
	{
	CListCtrl::OnSysColorChange();
	if( mDraw.UseDefaultColors )
		CacheVisuals();
	}

//
//	CGridCtrl::CacheVisuals
//
void 
CGridCtrl::CacheVisuals()
	{
	RECT rect, TempRect;
	CClientDC dc(this);

	// kill previous cached values
	mDraw.KillObjects();

	// bitmap height
	int Height = (GetImageList(LVSIL_SMALL) ? 16 : 13);
	int Width;

	//
	// generate combo box button
	//
	Width					= COMBO_BTNWIDTH;
	mDraw.ComboDC			= ::CreateCompatibleDC(dc);
	mDraw.ComboBmp			= ::CreateCompatibleBitmap(dc, Width, Height);
	mDraw.DefaultStockBmp	= (HBITMAP)::SelectObject(mDraw.ComboDC, mDraw.ComboBmp);

	// background
	::SetRect	(&rect, 0, 0, Width, Height);
	::FillRect	(mDraw.ComboDC, &rect, mDraw.BrushButtonFill);

	// frame
	rect.left--;
	::FrameRect(mDraw.ComboDC, &rect, mDraw.BrushFrame);
	rect.left++;

	// arrow
	TempRect.left	= MiddleInt(rect.left, rect.right) - 3;
	TempRect.top	= MiddleInt(rect.top, rect.bottom) - 1;
	TempRect.right	= TempRect.left + 5;
	TempRect.bottom = TempRect.top + 1;

	for(char i = 0; i < 3; i++)
		{
		::FrameRect(mDraw.ComboDC, &TempRect, mDraw.BrushFrame);
		TempRect.left ++, TempRect.top ++, TempRect.bottom ++, TempRect.right --;
		}

	//
	// generate elipsis button
	//
	Width			= CUSTOM_BTNWIDTH;
	mDraw.CustomDC	= ::CreateCompatibleDC(dc);
	mDraw.CustomBmp = ::CreateCompatibleBitmap(dc, Width, Height);
	::SelectObject(mDraw.CustomDC, mDraw.CustomBmp);

	// background
	::SetRect	(&rect, 0, 0, Width, Height);
	::FillRect	(mDraw.CustomDC, &rect, mDraw.BrushButtonFill);

	// frame
	rect.left --;
	::FrameRect(mDraw.CustomDC, &rect, mDraw.BrushFrame);
	rect.left ++;

	// ellipsis
	TempRect.left	= MiddleInt(rect.left, rect.right) - 5;
	TempRect.top	= rect.bottom - 5;
	TempRect.right	= TempRect.left + 2;
	TempRect.bottom = TempRect.top + 2;
	for(int i = 0; i < 3; i++)
		{
		::FillRect	(mDraw.CustomDC, &TempRect, mDraw.BrushFrame);
		::OffsetRect(&TempRect, 3, 0);
		}

	//
	//	generate check box marks
	//
	Width				= CHECK_BTNWIDTH;
	::SetRect(&rect, 0, 0, Width, Height);
	rect.bottom ++;

	mDraw.CheckOffDC	= ::CreateCompatibleDC(dc);
	mDraw.CheckOffBmp	= ::CreateCompatibleBitmap(dc, Width, Height + 1);
	::SelectObject(mDraw.CheckOffDC, mDraw.CheckOffBmp);

	mDraw.CheckOnDC		= ::CreateCompatibleDC(dc);
	mDraw.CheckOnBmp	= ::CreateCompatibleBitmap(dc, Width, Height + 1);
	::SelectObject(mDraw.CheckOnDC, mDraw.CheckOnBmp);

	// background
	::FillRect(mDraw.CheckOffDC, &rect, mDraw.BrushNormalFill);

	// draw button check frame
	TempRect.left	= rect.left + (RWIDTH(&rect) - CHECK_MARKWIDTH) / 2;
	TempRect.top	= rect.top + (RHEIGHT(&rect) - CHECK_MARKWIDTH) / 2;
	TempRect.right	= TempRect.left + CHECK_MARKWIDTH;
	TempRect.bottom = TempRect.top + CHECK_MARKWIDTH;
	::FrameRect(mDraw.CheckOffDC, &TempRect, mDraw.BrushFrame);

	rect.bottom --;

	// draw check mark
	::BitBlt(	mDraw.CheckOnDC,
				0, 0, Width, Height + 1,
				mDraw.CheckOffDC,
				0, 0,
				SRCCOPY);

	HPEN ShadowPen	= ::CreatePen(PS_SOLID, 0, mDraw.ColorFrame);
	HPEN oldp		= (HPEN)::SelectObject(mDraw.CheckOnDC, ShadowPen);

	POINT pt;
	pt.x = MiddleInt(TempRect.left, TempRect.right) - 3;
	pt.y = MiddleInt(TempRect.top, TempRect.bottom) - 1;

	for( int i = 0; i < 3; i++ )
		{
		::MoveToEx	(mDraw.CheckOnDC, pt.x, pt.y, NULL);
		::LineTo	(mDraw.CheckOnDC, pt.x + 2, pt.y + 2);
		::LineTo	(mDraw.CheckOnDC, pt.x + 2 + 5, pt.y + 2 - 5);
		pt.y ++;
		}

	::SelectObject(mDraw.CheckOnDC, oldp);
	::DeleteObject(ShadowPen);
	// set flag
	mDraw.Initialized = true;
	}

//
//	CGridCtrl::OnSetImageList
//
LRESULT	CGridCtrl::OnSetImageList(WPARAM wp, LPARAM lp)
{
	LRESULT ret = Default();

	if (!mDestroying && wp == LVSIL_SMALL)
	{
		// some cached items depend on the image list, so they
		// must be update...
		CacheVisuals();
	}

	return ret;
}

//
//	CGridCtrl::OnDestroy
//
void 
CGridCtrl::OnDestroy()
	{
	// set "Destroying" flag in order to avoid recaching
	mDestroying = true;
	CListCtrl::OnDestroy();
	}

//
//	[virtual] CGridCtrl::OnIncrementalSearch
//
void 
CGridCtrl::OnIncrementalSearch(TCHAR ch)
	{
	ASSERT(mIncSearch);
	CString sTip;
	int n;

	// string overflow?
	if( mIncSearchString.GetLength() >= mBufferLen )
		{
		MessageBeep((UINT)-1);
		goto _exit;
		}

	// update incremental string
	mIncSearchString += ch;

	// get window position
	RECT rect;
	GetWindowRect(&rect);

	// update the tooltip
	sTip = mIncSearchString + TEXT("_");
	TOOLINFO ti;
	ti.cbSize = sizeof(ti);
	ti.lpszText = sTip.GetBuffer(0);
	ti.hwnd = m_hWnd;
	ti.uId = ID_TOOLTIP;
	::SendMessage(mIncSearchTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
	::SendMessage(mIncSearchTip, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(rect.left + 5, rect.top + 5));
	::SendMessage(mIncSearchTip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

	// find the item that matches the key
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.pszText = mBuffer;
	item.cchTextMax = mBufferLen;
	item.iSubItem = mCol;

	n = GetItemCount();
	for (item.iItem = mRow; item.iItem < n; item.iItem++)
		{
		GetItem(&item);
		if (_tcsnicmp(mBuffer, mIncSearchString.GetBuffer(0), mIncSearchString.GetLength()) == 0)
			{
			if (SelectCell(item.iItem, mCol))
				DeselectAll();
			goto _exit;
			}
		}

	// continue searching from the 1st item
	for (item.iItem = 0; item.iItem < mRow; item.iItem++)
		{
		GetItem(&item);
		if (_tcsnicmp(mBuffer, mIncSearchString.GetBuffer(0), mIncSearchString.GetLength()) == 0)
			{
			if (!SelectCell(item.iItem, mCol))
				DeselectAll();
			goto _exit;
			}
		}

_exit:

	SetTimer(ID_TIMER, mIncSearchTimer, NULL);
	}

//
//	CGridCtrl::OnTimer
//
void 
CGridCtrl::OnTimer(UINT_PTR nIDEvent)
	{
	if( nIDEvent == ID_TIMER )
		{
		mIncSearchString.Empty();

		// hide the incremental-search tooltip
		TOOLINFO ti;
		ti.cbSize = sizeof(ti);
		ti.hwnd = m_hWnd;
		ti.uId = ID_TOOLTIP;
		::SendMessage(mIncSearchTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&ti);

		// stop incremental-search timer
		KillTimer(ID_TIMER);
		return;
		}

	CListCtrl::OnTimer(nIDEvent);
	}

//
//	CGridCtrl::RedrawItem
//
void CGridCtrl::RedrawItem(	IN int Row,
							IN bool CheckVisibility)
{
	if (CheckVisibility)
	{
		int Top = GetTopIndex();
		if (Row < Top || Row > (Top + GetCountPerPage()))
			return;
	}

	RedrawItems(Row, Row);
}

//
//	MESSAGE_MAP
//
BEGIN_MESSAGE_MAP(CGridCtrl, CListCtrl)
	ON_WM_ERASEBKGND		()
	ON_WM_LBUTTONDOWN		()
	ON_NOTIFY_REFLECT_EX	(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT_EX	(NM_CLICK,		OnLClick)
	ON_NOTIFY_REFLECT_EX	(NM_RCLICK,		OnRClick)

	ON_WM_LBUTTONDBLCLK		()
	ON_WM_RBUTTONDOWN		()
	ON_WM_KEYDOWN			()
	ON_WM_CHAR				()
	ON_WM_SETFOCUS			()
	ON_WM_KILLFOCUS			()
	ON_WM_SYSCOLORCHANGE	()
	ON_MESSAGE				(LVM_SETIMAGELIST, OnSetImageList)
	ON_WM_DESTROY			()
	ON_WM_TIMER				()
END_MESSAGE_MAP()
