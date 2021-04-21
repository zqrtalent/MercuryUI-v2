// ESGrid.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ESGrid.h"
#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===========================================================================
// class ESGridCol
//===========================================================================

ESGridCol::ESGridCol()
	{		
	m_nWidth		= 40;		
	m_nOrdinal		= 0;		
	m_bVisible		= true;		
	m_bLocked		= false;		
	m_shType		= 0;		
	m_shSortStatus	= 0;	
	m_param			= NULL;		
	m_pNextCol		= NULL;		
	m_pPrevCol		= NULL;		
	}

ESGridCol::~ESGridCol()
	{
	}

//===========================================================================
// class ESGrid
//===========================================================================

ESGrid::ESGrid() : MercuryBaseWnd()
	{
	m_pFirstCol		= NULL;
	m_nNumberRows	= 0;
	m_nNumberCols	= 0;
	m_pColHeights	= NULL;
	m_bVirtualMode	= true;
	m_nItemHeight	= 24;

	m_pLeftCol		= NULL;
	m_nTopRow		= 0;
	m_nOffsetX		= 0;
	m_nOffsetY		= 0;
	}

ESGrid::~ESGrid()
	{
	DestroyAll();
	}

void
ESGrid::DestroyAll()
	{
	while( m_pFirstCol ){
		ESGridCol* tmp = m_pFirstCol->m_pNextCol;
		delete m_pFirstCol;
		m_pFirstCol = tmp;
		}

	m_pLeftCol		= NULL;
	m_nTopRow		= 0;
	m_nOffsetX		= 0;
	m_nOffsetY		= 0;
	m_pFirstCol		= NULL;
	m_nNumberRows	= 0;
	m_nNumberCols	= 0;
	m_pColHeights	= NULL;
	m_bVirtualMode	= true;
	}

BEGIN_MESSAGE_MAP(ESGrid, MercuryBaseWnd)
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_CHAR			()
	ON_WM_KEYDOWN		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_SETCURSOR		()
	ON_WM_MOUSEWHEEL	()
	ON_WM_GETDLGCODE	()
	ON_WM_SHOWWINDOW	()
	ON_WM_MOUSEACTIVATE	()
	ON_MESSAGE			(WM_VSCROLL, OnVScroll)
	ON_WM_SIZE			()
END_MESSAGE_MAP()

void
ESGrid::SetNumberCols(int nNumCols)
	{
	if( m_pFirstCol && nNumCols == 0 ){ // Remove all cols.
		while( m_pFirstCol ){
			ESGridCol* tmp = m_pFirstCol->m_pNextCol;
			delete m_pFirstCol;
			m_pFirstCol = tmp;
			}
		m_nNumberCols	= 0;
		m_pFirstCol		= NULL;
		return;
		}

	ESGridCol*	prev	= m_pFirstCol;
	int			nLoop	= m_nNumberCols;

	if( prev ){ // Find last existed column.
		while(prev->m_pNextCol)
			prev = prev->m_pNextCol;
		}

	while( nLoop < nNumCols ){
		ESGridCol* pColNew	= new ESGridCol();
		if( prev )
			prev->m_pNextCol=  pColNew;
		else
			m_pFirstCol		= pColNew;

		pColNew->m_pPrevCol	= prev;
		prev				= pColNew;
		nLoop ++;
		}
	m_nNumberCols = nNumCols;
	}

void
ESGrid::SetNumberRows(int nRows)
	{
	m_nNumberRows = nRows;
	}

BOOL
ESGrid::SetText(int nRow, int nCol, TCHAR* pszText)
	{
	return FALSE;
	}

void
ESGrid::SetVirtualMode(bool bVirtualMode)
	{
	m_bVirtualMode = bVirtualMode;
	}

void 
ESGrid::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient)
	{
	}

void
ESGrid::GetClientRectMy(CRect& rcClient)
	{
	GetClientRect(rcClient);
	/*
	if( m_bVScroll ){
		rcClient.right -= m_pGUI->scrollBar.m_nVScrollWidth;
		}*/
	}

void
ESGrid::OnPrePaint(CRect rcRepaint)
	{
	/*
	// We mast mark items in repaint rect when clipbox.
	if( !m_bClipBox ) return;

	CRect rcItem;
	if( !GetItemRect(m_pFirstItem, rcItem) )
		return;

	CRect rcClient;
	GetClientRectMy(rcClient);

	// Set redraw state for items in repaint rect. ####
	MerTreeItem* pItem = GetFirstVisibleItem();
	while( rcItem.top < rcClient.bottom ){
		if( !(rcRepaint&rcItem).IsRectEmpty() ){
			SetItemRedraw(pItem, TRUE);
			}

		pItem			= GetNextVisibleItem(pItem);
		if( !pItem ) break;
		rcItem.top		= rcItem.bottom;
		rcItem.bottom	+= m_pGUI->treeGrid.m_nItemHeight;
		}
	// ################################################
	*/
	}

BOOL 
ESGrid::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
	{
	if( RegisterWndClass(_T("ESGrid_v1.0")) ){
		BOOL bRet =  CWnd::Create(_T("ESGrid_v1.0"), _T(""), dwStyle, rect, pParentWnd, nID, pContext);
		if( bRet ){
		//	CRect rcVScroll(0, 0, 0, 0);
		//	m_vScroll.Create(WS_CHILD|WS_VSCROLL, rcVScroll, this, 1000, NULL);
			}
		return bRet;
		}
	return FALSE;
	}

BOOL 
ESGrid::RegisterWndClass(LPCTSTR lpszClassName)
	{
	WNDCLASS wndClass;
	memset(&wndClass,0,sizeof(WNDCLASS));

	HINSTANCE hInst = AfxGetInstanceHandle();
	if( !GetClassInfo(hInst, lpszClassName, &wndClass) ){
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hbrBackground	= (HBRUSH)NULL;
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon			= (HICON)NULL;
		wndClass.hInstance		= hInst;
		wndClass.lpfnWndProc	= ::DefWindowProc;
		wndClass.lpszClassName	= lpszClassName;
		wndClass.lpszMenuName	= NULL;
		wndClass.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
		return AfxRegisterClass(&wndClass);
		}
	return TRUE;
	}

LRESULT	
ESGrid::OnVScroll(WPARAM wParam, LPARAM)
	{
	return 0;
	}

LRESULT
ESGrid::OnHScroll(WPARAM wParam, LPARAM)
	{
	return 0;
	}

void
ESGrid::OnShowWindow(BOOL bShow, UINT nStatus)
	{
	CWnd::OnShowWindow(bShow, nStatus);
	}

void
ESGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	CWnd::OnChar(nChar, nRepCnt, nFlags);
	}

void
ESGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	/*
	if( nChar == VK_DOWN ){ // Move Down
		MerTreeItem* pItem = m_pFirstItem;
		if( m_pSelItem )
			pItem = GetNextVisibleItem(m_pSelItem);
		if( pItem )
			SelectItem(pItem, TRUE);
		}
	else
	if( nChar == VK_UP ){	// Move Up
		MerTreeItem* pItem = m_pFirstItem;
		if( m_pSelItem )
			pItem = GetPrevVisibleItem(m_pSelItem);
		if( pItem )
			SelectItem(pItem, TRUE);
		}
	else
	if( nChar == VK_LEFT ){ // Collapse selected item.
		if( m_pSelItem ){
			if( !ExpandItem(m_pSelItem, TRUE, TRUE) ){
				// Movo to parent item. #############
				if( m_pSelItem->parent ){
					SelectItem(m_pSelItem->parent);
					}
				// ##################################
				}
			}
		}
	else
	if( nChar == VK_RIGHT ){ // Expand selected item.
		if( m_pSelItem ){
			ExpandItem(m_pSelItem, FALSE, TRUE);
			}
		}
*/
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}

void
ESGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
	CWnd::OnLButtonDblClk(nFlags, point);
	}

void
ESGrid::OnLButtonDown(UINT nFlags, CPoint point)
	{
	CWnd::OnLButtonDown(nFlags, point);
	}

void
ESGrid::OnLButtonUp(UINT nFlags, CPoint point)
	{
	CWnd::OnLButtonUp(nFlags, point);
	}

void
ESGrid::OnMouseMove(UINT nFlags, CPoint point)
	{
	CWnd::OnMouseMove(nFlags, point);
	}

BOOL
ESGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL
ESGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	}

UINT
ESGrid::OnGetDlgCode()
	{
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

BOOL	
ESGrid::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
	{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

void	
ESGrid::OnSize(UINT nType, int cx, int cy)
	{
	CWnd::OnSize(nType, cx, cy);
	}

/*
/////////////////////////////////////////////////////////////////////////////
// MercuryTreeGrid

MercuryTreeGrid::MercuryTreeGrid() : MercuryBaseWnd()
	{
	m_pContent			= new MercuryTreeGridContent();
	m_pSelItem			= NULL;
	m_pFirstItem		= NULL;
	m_nHeightView		= 0;
	m_nWidthView		= 0;
	m_nTopItemIndex		= 0;
	m_bVScroll			= FALSE;
	m_ptViewOffset.SetPoint(0, 0);
	}

MercuryTreeGrid::~MercuryTreeGrid()
	{
	DestroyClassMembers();
	}

BEGIN_MESSAGE_MAP(MercuryTreeGrid, MercuryBaseWnd)
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_CHAR			()
	ON_WM_KEYDOWN		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_SETCURSOR		()
	ON_WM_MOUSEWHEEL	()
	ON_WM_GETDLGCODE	()
	ON_WM_SHOWWINDOW	()
	ON_WM_MOUSEACTIVATE	()
	ON_MESSAGE			(WM_VSCROLL, OnVScroll)
	ON_WM_SIZE			()
END_MESSAGE_MAP()

BOOL 
MercuryTreeGrid::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
	{
	if( RegisterWndClass(_T("MercuryTreeGrid")) ){
		BOOL bRet =  CWnd::Create(_T("MercuryTreeGrid"), _T(""), dwStyle, rect, pParentWnd, nID, pContext);
		if( bRet ){
			CRect rcVScroll(0, 0, 0, 0);
			m_vScroll.Create(WS_CHILD|WS_VSCROLL, rcVScroll, this, 1000, NULL);
			}
		return bRet;
		}
	return FALSE;
	}

void
MercuryTreeGrid::DestroyClassMembers()
	{
	if( m_pContent ){
		delete m_pContent;
		m_pContent = NULL;
		}

	for( int i=0; i<m_arrImages.GetCount(); i++ ){
		CImage* pImage = m_arrImages.GetData(i);
		if( pImage )
			delete pImage;
		}
	m_arrImages.DeleteAll();

	if( IsWindow(m_vScroll) )
		m_vScroll.DestroyWindow();

	m_nTopItemIndex	= 0;
	m_nHeightView	= 0;
	m_nWidthView	= 0;
	m_pSelItem		= NULL;
	m_pFirstItem	= NULL;
	}

MerTreeItem*
MercuryTreeGrid::GetSelectedItem()
	{
	return m_pSelItem;
	}
int
MercuryTreeGrid::GetVisChildsCtSetRedraw(MerTreeItem* pItem)
	{
	if( !pItem || !pItem->child || !IsItemExpanded(pItem) )
		return 0;

	// Count visible child items. ############
	MerTreeItem* pChildItem = pItem->child;
	MerTreeItem* pNextItem  = pItem->next;
	MerTreeItem* pParItem	= pItem->parent;
	pItem->parent			= NULL;
	pItem->next				= NULL;

	int nCt = 0;
	while( pChildItem ){
		SetItemRedraw(pChildItem, TRUE);
		pChildItem = GetNextVisibleItem(pChildItem);
		nCt ++;
		}
	pItem->parent			= pParItem;
	pItem->next				= pNextItem;
	// #######################################
	return nCt;
	}

int
MercuryTreeGrid::GetVisibleChildsCt(MerTreeItem* pItem)
	{
	if( !pItem || !pItem->child || !IsItemExpanded(pItem) )
		return 0;

	// Count visible child items. ############
	MerTreeItem* pChildItem = pItem->child;
	MerTreeItem* pNextItem  = pItem->next;
	MerTreeItem* pParItem	= pItem->parent;
	pItem->parent			= NULL;
	pItem->next				= NULL;

	int nCt = 0;
	while( pChildItem ){
		pChildItem = GetNextVisibleItem(pChildItem);
		nCt ++;
		}
	pItem->parent			= pParItem;
	pItem->next				= pNextItem;
	// #######################################
	return nCt;
	}

BOOL
MercuryTreeGrid::IsItemVisible(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;

	// Check for collapsed parent item. ##########
	MerTreeItem* parent = pItem->parent;
	while( parent ){
		if( !IsItemExpanded(parent) ) return FALSE;
		parent = parent->parent;
		}
	// ###########################################

	// Check item visibility. ####################
	CRect			rcItem;
	if( GetItemRect(pItem, rcItem) ){
		CRect rcClient;
		GetClientRectMy(rcClient);

		int nItemHeight = m_pGUI->treeGrid.m_nItemHeight;
		if( (rcClient.bottom == rcItem.bottom || rcClient.top == rcItem.top) && rcItem.Height() < nItemHeight ){
			return FALSE; // Partially visible last item is not visible yet.
			}
		return 1;
		}
	return FALSE;
	}

BOOL
MercuryTreeGrid::EnsureItemVisibility(MerTreeItem* pItem)
	{
	if( IsItemVisible(pItem) ) return FALSE;

	CRect			rcClient;
	GetClientRectMy(rcClient);
	int				nItemHeight		= m_pGUI->treeGrid.m_nItemHeight;
	int				nMaxTopItem		= (m_nHeightView - rcClient.Height())/nItemHeight;
	int				nMaxTopItemY	= (m_nHeightView - rcClient.Height())%nItemHeight;
	int				nItemsVisible	= (rcClient.Height() / nItemHeight);
	int				nItemsCt		= (m_nHeightView / nItemHeight);
	int				nIndex			= 0;
	MerTreeItem*	pItemSeek		= GetRootItem();

	while( pItemSeek ){
		if( pItemSeek == pItem ){
			int nTopIndex		= (nIndex + 1)%nItemsVisible + (((nIndex + 1)/nItemsVisible) - 1)*nItemsVisible;
			if( m_nTopItemIndex > nTopIndex ){
				if( (m_nTopItemIndex - nIndex) > 1  ){
					nTopIndex = (nIndex/nItemsVisible)*nItemsVisible;
					}
				else
					nTopIndex = nIndex;
				}
			int nOffsetY		= 0;
			if( IsLastItem(pItemSeek) ){
				nOffsetY = nMaxTopItemY;
				}

			MoveViewPos(0, (nTopIndex*nItemHeight + nOffsetY) - (m_nTopItemIndex*nItemHeight + m_ptViewOffset.y), TRUE);
			return TRUE;
			}

		nIndex ++;
		pItemSeek = GetNextVisibleItem(pItemSeek);
		}

	return FALSE;
	}

BOOL
MercuryTreeGrid::SelectItem(MerTreeItem* pItem, BOOL bInvalidate)
	{
	if( !pItem ) return 0;
	if( m_pSelItem == pItem ) return 1;
	if( m_pSelItem ){
		SetItemSelected(m_pSelItem, FALSE);
		// Clear old selection.
		RedrawItem(m_pSelItem);
		}

	m_pSelItem = pItem;
	SetItemSelected(m_pSelItem, TRUE);
	if( bInvalidate ){
		if( !EnsureItemVisibility(m_pSelItem) ){
			// Redraw currently selected item.
			RedrawItem(m_pSelItem);
			}
		}
	return 1;
	}

BOOL
MercuryTreeGrid::ExpandItem(MerTreeItem* pItem, BOOL bCollapse, BOOL bInvalidate)
	{
	if( !pItem || !IsItemParent(pItem) || !IsItemExpandable(pItem) ) return FALSE;
	if( bCollapse ){
		// Already collapsed.
		if( !IsItemExpanded(pItem) ) return FALSE;
		int nHeightViewOld	= m_nHeightView;
		// Set collapsed state.
		SetItemExpanded(pItem, FALSE);
		// ############################
		
		// Arrange top item when collapse item. ##########
		CRect rcClient;
		GetClientRectMy(rcClient);

		int nItemHeight		= m_pGUI->treeGrid.m_nItemHeight;
		int nHeightVisItems = m_nHeightView - (m_nTopItemIndex*nItemHeight + m_ptViewOffset.y);
		if( nHeightVisItems < rcClient.Height() ){
			MoveViewPos(0, nHeightVisItems - rcClient.Height(), TRUE);
			// Update scroll bars. ########
			UpdateScrollBars();
			// ############################
			return TRUE;
			}
		// ###############################################

		// Invalidate items. ######
		if( bInvalidate ){
			CRect rcClient, rcItem;
			GetClientRectMy(rcClient);
			if( GetItemRect(pItem, rcItem) ){
				CRect rcInvalidate(rcItem.left, rcItem.top, rcItem.right, rcClient.bottom);
				while( rcItem.top <  rcInvalidate.bottom ){
					// Set redraw flag. ##################
					SetItemRedraw(pItem, TRUE);
					// ###################################
					pItem = GetNextVisibleItem(pItem);
					if( !pItem ) break;
					rcItem.top		= rcItem.bottom;
					rcItem.bottom	+= m_pGUI->treeGrid.m_nItemHeight;
					}
				RedrawRect(rcInvalidate);
				}
			else{
				RedrawItem(pItem);
				}
			// ###################
			// Update scroll bars. ########
			UpdateScrollBars();
			// ############################
			return TRUE;
			}
		}
	else{
		// Already expanded.
		if( IsItemExpanded(pItem) ) return FALSE;
	
		// Set expanded state.
		SetItemExpanded(pItem, TRUE);

		// Invalidate items. ######
		if( bInvalidate ){
			CRect rcClient, rcItem;
			GetClientRectMy(rcClient);
			if( GetItemRect(pItem, rcItem) ){
				CRect rcInvalidate(rcItem.left, rcItem.top, rcItem.right, rcClient.bottom);
				while( rcItem.top <  rcInvalidate.bottom ){
					// Set redraw flag. ##################
					SetItemRedraw(pItem, TRUE);
					// ###################################
					
					pItem = GetNextVisibleItem(pItem);
					if( !pItem ) break;
					rcItem.top		= rcItem.bottom;
					rcItem.bottom	+= m_pGUI->treeGrid.m_nItemHeight;
					}
				RedrawRect(rcInvalidate);
				}
			else
				RedrawItem(pItem);
			// ###################
			// Update scroll bars. ########
			UpdateScrollBars();
			// ############################
			return TRUE;
			}
		}
	return FALSE;
	}

BOOL
MercuryTreeGrid::CheckItem(MerTreeItem* pItem, BOOL bCheck, BOOL bInvalidate)
	{
	if( bInvalidate ){
		RedrawItem(pItem);
		}
	return FALSE;
	}

BOOL
MercuryTreeGrid::DeleteItem(MerTreeItem* pItem)
	{
	return FALSE;
	}

int
MercuryTreeGrid::AddItemImage(CString sImageFile)
	{
	CImage* pImage = new CImage;
	if( pImage->Load(sImageFile.GetBuffer()) != 0 ){
		delete pImage;
		return -1;
		}

	m_arrImages.Add(m_arrImages.GetCount(), pImage);
	return m_arrImages.GetCount() - 1;
	}

BOOL
MercuryTreeGrid::SetItemImage(MerTreeItem* pItem, int nImage, int nImageSel, BOOL bInvalidate)
	{
	if( !pItem ) return FALSE;
	pItem->iImage		= nImage;
	pItem->iSelImage	= nImageSel;

	if( bInvalidate ){
		RedrawItem(pItem);
		}

	return TRUE;
	}

MerTreeItem*
MercuryTreeGrid::AddItem(LPTSTR lpszText, MerTreeItem* parent)
	{
	MerTreeItem* pItemNew = new MerTreeItem;
	pItemNew->parent	= parent;
	pItemNew->next		= NULL;
	pItemNew->prev		= NULL;
	pItemNew->child		= NULL;
	pItemNew->sText		= lpszText;
	pItemNew->param		= 0;
	pItemNew->iImage	= -1;
	pItemNew->iSelImage = -1;
	pItemNew->dwState	= m_pGUI->treeGrid.m_dwDefState;

	if( !m_pContent->Add(pItemNew, parent) ){
		delete pItemNew;
		return NULL;
		}

	if( !m_pFirstItem ){
		m_pFirstItem = pItemNew;
		}

	// Check for collapsed item.
	BOOL			bVisible= TRUE;
	while( parent ){
		if( !IsItemExpanded(parent) ){
			bVisible = FALSE;
			break;
			}
		parent = parent->parent;
		}
	// #########################

	if( bVisible ){
		m_nHeightView += m_pGUI->treeGrid.m_nItemHeight;
		// Update scroll bars. ########
		UpdateScrollBars();
		// ############################
		}
	return pItemNew;
	}

LPCTSTR
MercuryTreeGrid::GetItemText(MerTreeItem* pItem)
	{
	return pItem->sText.GetBuffer();
	}

BOOL
MercuryTreeGrid::SetItemText(MerTreeItem* pItem, CString sText)
	{
	pItem->sText = sText;
	return 1;
	}

BOOL
MercuryTreeGrid::GetItemRect(MerTreeItem* pItem, CRect& rcItem)
	{
	if( !pItem ) return FALSE;
	MerTreeItem* pItemRect = GetFirstVisibleItem();
	rcItem.top		= 0;
	rcItem.bottom	= m_pGUI->treeGrid.m_nItemHeight;

	if( m_ptViewOffset.y > 0 ){
		rcItem.bottom	= (m_pGUI->treeGrid.m_nItemHeight - m_ptViewOffset.y);
		}
	else
	if( m_ptViewOffset.y < 0 ){
		rcItem.top		= -1*m_ptViewOffset.y;
		rcItem.bottom	= rcItem.top + m_pGUI->treeGrid.m_nItemHeight;
		}

	CRect rcClient;
	GetClientRectMy(rcClient);
	rcItem.left		= rcClient.left;
	rcItem.right	= rcClient.right;

	while( rcItem.top < rcClient.bottom ){
		if( pItem == pItemRect ){
			rcItem.bottom = min(rcClient.bottom, rcItem.bottom);
			return 1;
			}

		pItemRect		= GetNextVisibleItem(pItemRect);
		rcItem.top		= rcItem.bottom;
		rcItem.bottom	+= m_pGUI->treeGrid.m_nItemHeight;
		}
	return FALSE;
	}

MerTreeItem*
MercuryTreeGrid::GetFirstVisibleItem()
	{
	return m_pFirstItem;
	}

MerTreeItem*			
MercuryTreeGrid::GetNextVisibleItem(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	if( pItem->child && IsItemExpanded(pItem) ){
		return pItem->child;
		}
	else
	if( pItem->next ){
		return pItem->next;
		}
	else{
		MerTreeItem* parent = pItem->parent;
		while( parent ){
			if( parent->next )
				return parent->next;
			parent = parent->parent;
			}
		}
	return NULL;
	}

MerTreeItem*
MercuryTreeGrid::GetPrevVisibleItem(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	if( pItem->prev ){
		pItem = pItem->prev;
		while( pItem->child && IsItemExpanded(pItem) ){
			// Find last child. #####
			pItem = pItem->child;
			while( pItem->next ){
				pItem = pItem->next;
				}
			// ######################
			}
		return pItem;
		}
	else
		return pItem->parent;
	return NULL;
	}

CFont*
MercuryTreeGrid::GetItemFont(MerTreeItem* pItem)
	{
	if( m_bActive ){
		if( IsItemSelected(pItem) )
			return &m_pGUI->treeGrid.m_fontSelItemActive;
		else
		if( IsItemReadOnly(pItem) )
			return &m_pGUI->treeGrid.m_fontReadOnlyItemActive;
		else
			return &m_pGUI->treeGrid.m_fontNormalItemActive;
		}
	else{
		if( IsItemSelected(pItem) )
			return &m_pGUI->treeGrid.m_fontSelItemNonActive;
		else
		if( IsItemReadOnly(pItem) )
			return &m_pGUI->treeGrid.m_fontReadOnlyItemNonActive;
		else
			return &m_pGUI->treeGrid.m_fontNormalItemNonActive;
		}
	return NULL;
	}

COLORREF
MercuryTreeGrid::GetItemTextColor(MerTreeItem* pItem, COLORREF& crShadow)
	{
	if( m_bActive ){
		if( IsItemSelected(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crSelItemTextActShadow;
			return m_pGUI->treeGrid.m_crSelItemTextActive;
			}
		else
		if( IsItemReadOnly(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crReadOnlyItemTextActShadow;
			return m_pGUI->treeGrid.m_crReadOnlyItemTextActive;
			}
		else{
			crShadow = m_pGUI->treeGrid.m_crNormalItemTextActShadow;
			return m_pGUI->treeGrid.m_crNormalItemTextActive;
			}
		}
	else{
		if( IsItemSelected(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crSelItemTextNonActShadow;
			return m_pGUI->treeGrid.m_crSelItemTextNonActive;
			}
		else
		if( IsItemReadOnly(pItem) ){
			crShadow = m_pGUI->treeGrid.m_crReadOnlyItemTextNonActShadow;
			return m_pGUI->treeGrid.m_crReadOnlyItemTextNonActive;
			}
		else{
			crShadow = m_pGUI->treeGrid.m_crNormalItemTextNonActShadow;
			return m_pGUI->treeGrid.m_crNormalItemTextNonActive;
			}
		}
	return 0;
	}

MerTreeItem*
MercuryTreeGrid::ItemByPoint(CPoint pt, CRect* pRcItem)
	{
	CRect rcView;
	GetClientRectMy(rcView);
	if( !rcView.PtInRect(pt) ) return NULL;

	MerTreeItem*	pItem		= m_pFirstItem;
	int				nOffsetY	= m_ptViewOffset.y;
	CRect			rcItem;
	rcItem.left		= rcView.left;
	rcItem.right	= rcView.right;

	if( m_ptViewOffset.y < 0 ){
		pItem		= GetPrevVisibleItem(pItem);
		nOffsetY	= (m_pGUI->treeGrid.m_nItemHeight + m_ptViewOffset.y);
		}

	int nItemHeight = m_pGUI->treeGrid.m_nItemHeight;
	rcItem.top		= rcView.top - nOffsetY;
	rcItem.bottom	= rcItem.top + nItemHeight;

	pt.y -= (nItemHeight - nOffsetY);
	if( pt.y > 0 ){
		do{
			if( !pItem ) return NULL;
			pt.y			-= nItemHeight;
			pItem			= GetNextVisibleItem(pItem);
			rcItem.top		+= nItemHeight;
			rcItem.bottom	+= nItemHeight;
			if( pt.y <= 0 ) break;
			}
		while( TRUE );
		}

	if( pRcItem )
		*pRcItem = rcItem;
	return pItem;
	}

BOOL
MercuryTreeGrid::SelectItemByPoint(CPoint pt)
	{
	CRect		 rcItem;
	MerTreeItem* pItemSel = ItemByPoint(pt, &rcItem);
	if( !pItemSel ) return FALSE;

	if( pItemSel != m_pSelItem ){
		// Clear old selection. ############
		if( m_pSelItem ){
			SetItemSelected(m_pSelItem, FALSE);
			RedrawItem(m_pSelItem);
			}
		// #################################
		SetItemSelected(pItemSel, TRUE);
		RedrawItem(pItemSel);
		m_pSelItem = pItemSel;
		return TRUE;
		}
	else // Expand / Collapse operation.
	if( IsItemExpandable(pItemSel) && IsItemParent(pItemSel) ){
		ExpandItem(pItemSel, IsItemExpanded(pItemSel), TRUE);
		return TRUE;
		}
	return FALSE;
	}

void
MercuryTreeGrid::UpdateScrollBars()
	{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcVScroll(rcClient.right - m_pGUI->scrollBar.m_nVScrollWidth, rcClient.top, rcClient.right, rcClient.bottom);
	if( m_nHeightView > rcClient.Height() ){
		// Move scrollbar window in left side.
		::MoveWindow(m_vScroll, rcVScroll.left, rcVScroll.top, rcVScroll.Width(), rcVScroll.Height(), FALSE);

		if( !m_bVScroll ){
			m_vScroll.ShowWindow(SW_SHOW);

			// Redraw scroll bar. ###############
			m_vScroll.RedrawRect(CRect(0, 0, rcVScroll.Width(), rcVScroll.Height()), TRUE);
			// ##################################
			m_bVScroll = TRUE;
			}
		// Update scroll position. ##############
		GetClientRectMy(rcClient);
		int nPos = (m_nTopItemIndex*m_pGUI->treeGrid.m_nItemHeight)+m_ptViewOffset.y;
		m_vScroll.SetScrollPos	(nPos, FALSE);
		m_vScroll.SetScrollInfo	(0, m_nHeightView, rcClient.Height());
		// ######################################
		}
	else{
		if( m_bVScroll ){
			m_vScroll.ShowWindow(SW_HIDE);
			m_bVScroll = FALSE;
			}
		}
	}

BOOL
MercuryTreeGrid::MoveViewPos(int nMoveX, int nMoveY, BOOL bInvalidate, BOOL bUpdateScrolls, BOOL bRedrawAll )
	{
	if( !nMoveX && !nMoveY ) return FALSE;
	CRect rcClient;
	GetClientRectMy(rcClient);

	// When view height is changed. ####################
	if( nMoveY && m_nHeightView <= rcClient.Height() ){
		// Change first visible item. ######
		m_pFirstItem = GetRootItem();
		// #################################
		if( m_ptViewOffset.y != 0 || m_nTopItemIndex > 0 ){
			m_ptViewOffset.y	= 0;
			m_nTopItemIndex		= 0;
			Invalidate(FALSE);
			return TRUE;
			}

		Invalidate(FALSE);
		return FALSE;
		}
	// #################################################

	int nItemHeight		= m_pGUI->treeGrid.m_nItemHeight;
	int nMaxTopItem		= (m_nHeightView - rcClient.Height())/nItemHeight;
	int nMaxTopItemY	= (m_nHeightView - rcClient.Height())%nItemHeight;
	int nItemMove		= (m_ptViewOffset.y + nMoveY)/nItemHeight;
	int nItemOffsetY	= (m_ptViewOffset.y + nMoveY)%nItemHeight;

	// Check top item index and offset values. #########
	if( nMoveY > 0 ){
		if( ((m_nTopItemIndex + nItemMove) > nMaxTopItem) ||
			((m_nTopItemIndex + nItemMove) == nMaxTopItem) && nItemOffsetY > nMaxTopItemY){
			nItemMove		= (nMaxTopItem - m_nTopItemIndex);
			nItemOffsetY	= nMaxTopItemY;
			}
		}
	if( nMoveY < 0 ){
		if( ((m_nTopItemIndex + nItemMove) < 0) ||
			((m_nTopItemIndex + nItemMove) == 0 && nItemOffsetY < 0 ) ){
			nItemMove		= -1*m_nTopItemIndex;
			nItemOffsetY	= 0;
			}
		}
	// ################################################

	// Sync top item. #######################
	int nLoop = 0;
	while( nItemMove > 0 ){
		m_pFirstItem = GetNextVisibleItem(m_pFirstItem);
		nItemMove --; nLoop ++;
		}

	while( nItemMove < 0 ){
		m_pFirstItem = GetPrevVisibleItem(m_pFirstItem);
		nItemMove ++; nLoop --;
		}
	// ######################################

	BOOL bViewChanged	= TRUE;
	if( nItemOffsetY < 0 ){
		nItemOffsetY	+= nItemHeight;
		m_pFirstItem	= GetPrevVisibleItem(m_pFirstItem);
		nLoop			--;
		}

	int nTopIndexOld		= m_nTopItemIndex;
	CPoint ptViewOffsetOld	= m_ptViewOffset;
	m_ptViewOffset.y		= nItemOffsetY;
	m_nTopItemIndex			+= nLoop;

	nMoveY = (m_nTopItemIndex*nItemHeight - (nItemHeight - m_ptViewOffset.y)) - 
			 (nTopIndexOld*nItemHeight - (nItemHeight - ptViewOffsetOld.y));
	if( bInvalidate && nMoveY != 0 ){
		if( bRedrawAll ){
			RedrawAllItems();
			}
		else{ // Scroll DC.
			if( abs(nMoveY) < rcClient.Height() ){
				ScrollWindow(0, -1*nMoveY);
				}
			else
				RedrawAllItems();
			}
		}

	// Update scroll bars. ########
	if( bUpdateScrolls )
		UpdateScrollBars();
	// ############################

//	CString s;
//	s.Format(_T("%d %d"), m_nTopItemIndex, m_ptViewOffset.y);
//	GetParent()->SetWindowText(s);
	return TRUE;
	}

void
MercuryTreeGrid::RedrawAllItems()
	{
	CRect rcClient;
	GetClientRectMy(rcClient);
	RedrawRect(rcClient);

	CRect rcItem;
	MerTreeItem* pItem = m_pFirstItem;
	if( GetItemRect(pItem, rcItem) ){
		while( rcItem.top <  rcClient.bottom ){
			// Set redraw flag. ##################
			SetItemRedraw(pItem, TRUE);
			// ###################################
			pItem = GetNextVisibleItem(pItem);
			if( !pItem ) break;
			rcItem.top		= rcItem.bottom;
			rcItem.bottom	+= m_pGUI->treeGrid.m_nItemHeight;
			}
		}
	}

BOOL
MercuryTreeGrid::RedrawItem(MerTreeItem* pItem)
	{
	if( !pItem ) return FALSE;
	CRect rcItem;
	if( GetItemRect(pItem, rcItem) ){
		SetItemRedraw(pItem, TRUE);
		RedrawRect(rcItem);
		return TRUE;
		}
	return FALSE;
	}

BOOL 
MercuryTreeGrid::RegisterWndClass(LPCTSTR lpszClassName)
	{
	WNDCLASS wndClass;
	memset(&wndClass,0,sizeof(WNDCLASS));

	HINSTANCE hInst = AfxGetInstanceHandle();
	if( !GetClassInfo(hInst, lpszClassName, &wndClass) ){
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hbrBackground	= (HBRUSH)NULL;
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon			= (HICON)NULL;
		wndClass.hInstance		= hInst;
		wndClass.lpfnWndProc	= ::DefWindowProc;
		wndClass.lpszClassName	= lpszClassName;
		wndClass.lpszMenuName	= NULL;
		wndClass.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
		return AfxRegisterClass(&wndClass);
		}
	return TRUE;
	}

void
MercuryTreeGrid::OnPrePaint(CRect rcRepaint)
	{
	// We mast mark items in repaint rect when clipbox.
	if( !m_bClipBox ) return;

	CRect rcItem;
	if( !GetItemRect(m_pFirstItem, rcItem) )
		return;

	CRect rcClient;
	GetClientRectMy(rcClient);

	// Set redraw state for items in repaint rect. ####
	MerTreeItem* pItem = GetFirstVisibleItem();
	while( rcItem.top < rcClient.bottom ){
		if( !(rcRepaint&rcItem).IsRectEmpty() ){
			SetItemRedraw(pItem, TRUE);
			}

		pItem			= GetNextVisibleItem(pItem);
		if( !pItem ) break;
		rcItem.top		= rcItem.bottom;
		rcItem.bottom	+= m_pGUI->treeGrid.m_nItemHeight;
		}
	// ################################################
	}

void 
MercuryTreeGrid::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient)
	{
	CRect	rcClient;
	GetClientRectMy(rcClient);

	CRect	rcDestDC; // Full destination rect in DC coordinates.
	rcDestDC.left	= pRectDC->left - pRectClient->left;
	rcDestDC.top	= pRectDC->top - pRectClient->top;
	rcDestDC.right	= rcDestDC.left + rcClient.Width();
	rcDestDC.bottom	= rcDestDC.top + rcClient.Height();
	pDC->FillSolidRect(&rcDestDC, GetBkColor());

	int nItemHeight = m_pGUI->treeGrid.m_nItemHeight;
	MerTreeItem*	pItem = GetFirstVisibleItem();
	CString			sItemText;
	CRect			rcItem;
	COLORREF		crShadow, crText;
	rcItem.left		= rcDestDC.left + (m_pGUI->treeGrid.m_ptOffset.x - m_ptViewOffset.x);
	rcItem.right	= rcDestDC.right;
	rcItem.top		= rcDestDC.top + (m_pGUI->treeGrid.m_ptOffset.y - m_ptViewOffset.y);
	rcItem.bottom	= rcItem.top + nItemHeight;

	CImage*			pImageSel = NULL;
	BITMAP			bmSel;
	if( m_bActive ){
		pImageSel = &m_pGUI->treeGrid.m_imageSelActive;
		GetObject(m_pGUI->treeGrid.m_imageSelActive, sizeof(BITMAP), &bmSel);
		}
	else{
		pImageSel = &m_pGUI->treeGrid.m_imageSelNonActive;
		GetObject(m_pGUI->treeGrid.m_imageSelNonActive, sizeof(BITMAP), &bmSel);
		}

	int	nLoop	= 0;
	while( pItem ){
		if( rcItem.top > rcDestDC.bottom ) break;
		int nParCt		= GetParentsCount(pItem);
		int nOffsetX	= 14*nParCt;

		// Skip invisible items. ###############
		if( rcItem.bottom < 0 || (m_bClipBox && !GetItemRedraw(pItem)) ){
			pItem			= GetNextVisibleItem(pItem);
			rcItem.left		= rcDestDC.left + (m_pGUI->treeGrid.m_ptOffset.x - m_ptViewOffset.x);
			rcItem.top		= rcItem.bottom;
			rcItem.bottom	+= nItemHeight;
			continue;
			}
		// ####################################

		// Draw selected item. ################
		if( IsItemSelected(pItem) ){
			pImageSel->Draw(pDC->m_hDC, rcDestDC.left, rcItem.top, rcDestDC.Width(), 
				rcItem.Height(), 0, 0, bmSel.bmWidth, bmSel.bmHeight);
			}
		// ####################################

		// Draw expand / collapse sign. #######
		if( pItem->child && IsItemExpandable(pItem) ){
			CImage* pImgSign = &m_pGUI->treeGrid.m_imageExpandSign;
			if( IsItemExpanded(pItem) ){
				pImgSign = &m_pGUI->treeGrid.m_imageExpandSign;
				if( IsItemSelected(pItem) )
					pImgSign = &m_pGUI->treeGrid.m_imageExpandSignSel;
				}
			else{
				pImgSign = &m_pGUI->treeGrid.m_imageCollapseSign;
				if( IsItemSelected(pItem) )
					pImgSign = &m_pGUI->treeGrid.m_imageCollapseSignSel;
				}

			int nImageCX	= pImgSign->GetWidth(), nImageCY = pImgSign->GetHeight();
			CRect rcSign;
			rcSign.left		= rcItem.left + nOffsetX;
			rcSign.top		= rcItem.top + (rcItem.Height() - nImageCY)/2;
			rcSign.right	= rcSign.left + nImageCX;
			rcSign.bottom	= rcSign.top + nImageCY;

			CBitmap bmBkgnd;
			if( ImageHelper::GetBitmap32bppFromDC(bmBkgnd, pDC, rcSign.left, rcSign.top, nImageCX, nImageCY) ){
				BITMAP bmSrc;
				GetObject(*pImgSign, sizeof(BITMAP), &bmSrc);

				BITMAP bmDest;
				bmBkgnd.GetBitmap(&bmDest);
				ImageHelper::BitmapOnBitmapEx(&bmDest, 0, 0, &bmSrc, 0, 0, nImageCX, nImageCY, nImageCX, nImageCY, 1);

				CDC memDC;
				memDC.CreateCompatibleDC(pDC);
				memDC.SelectObject(&bmBkgnd);
				pDC->BitBlt(rcSign.left, rcSign.top, rcSign.Width(), rcSign.Height(), &memDC, 0, 0, SRCCOPY);
				bmBkgnd	.DeleteObject();
				memDC	.DeleteDC();

				rcItem.left += nImageCX + 2;
				}
			}
		// ####################################

		// Draw item image. ###################
		CImage* pImage = GetItemImage(pItem);
		if( pImage ){
			int nImageCX = pImage->GetWidth(), nImageCY = pImage->GetHeight();
			CRect rcImage;
			rcImage.left	= rcItem.left + nOffsetX;
			rcImage.top		= rcItem.top + (rcItem.Height() - nImageCY)/2;
			rcImage.right	= rcImage.left + nImageCX;
			rcImage.bottom	= rcImage.top + nImageCY;

			if( pImage->GetBPP() == 32 ){
				CBitmap bmBkgnd;
				if( ImageHelper::GetBitmap32bppFromDC(bmBkgnd, pDC, rcImage.left, rcImage.top, nImageCX, nImageCY) ){
					BITMAP bmSrc;
					GetObject(*pImage, sizeof(BITMAP), &bmSrc);

					BITMAP bmDest;
					bmBkgnd.GetBitmap(&bmDest);
					ImageHelper::BitmapOnBitmapEx(&bmDest, 0, 0, &bmSrc, 0, 0, nImageCX, nImageCY, nImageCX, nImageCY, 1);

					CDC memDC;
					memDC.CreateCompatibleDC(pDC);
					memDC.SelectObject(&bmBkgnd);
					pDC->BitBlt(rcImage.left, rcImage.top, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, SRCCOPY);
					bmBkgnd	.DeleteObject();
					memDC	.DeleteDC();
					}
				}
			else
				pImage->Draw(pDC->m_hDC, rcImage.left, rcImage.top, rcImage.Width(), rcImage.Height(), 0, 0, nImageCX, nImageCY);
			rcItem.left += nImageCX + 4;
			}
		// ####################################

		// Draw item text with shadow. ########
		sItemText		= GetItemText(pItem);
		CFont* pFont	= GetItemFont(pItem);
		if( pFont )
			pDC->SelectObject(pFont);

		CSize	szText		= pDC->GetTextExtent(sItemText, sItemText.GetLength());
		CPoint  ptText	(rcItem.left + nOffsetX, rcItem.top + (rcItem.Height()-szText.cy)/2);

		crText				= GetItemTextColor(pItem, crShadow);
		pDC->SetBkMode		(TRANSPARENT);
		if( HasItemShadow(pItem) && crShadow != crText ){
			pDC->SetTextColor	(crShadow);
			pDC->ExtTextOut		(ptText.x, ptText.y+1, ETO_CLIPPED, rcItem, sItemText, sItemText.GetLength(), NULL);
			}
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, rcItem, sItemText, sItemText.GetLength(), NULL);
		// ####################################

		// Clear redraw flag. #################
		SetItemRedraw(pItem, FALSE);
		// ####################################
		pItem			= GetNextVisibleItem(pItem);
		rcItem.left		= rcDestDC.left + (m_pGUI->treeGrid.m_ptOffset.x - m_ptViewOffset.x);
		rcItem.top		= rcItem.bottom;
		rcItem.bottom	+= nItemHeight;
		nLoop			++;
		}
	}

void
MercuryTreeGrid::OnShowWindow(BOOL bShow, UINT nStatus)
	{
	CWnd::OnShowWindow(bShow, nStatus);
	}

void
MercuryTreeGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	CWnd::OnChar(nChar, nRepCnt, nFlags);
	}

void
MercuryTreeGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	if( nChar == VK_DOWN ){ // Move Down
		MerTreeItem* pItem = m_pFirstItem;
		if( m_pSelItem )
			pItem = GetNextVisibleItem(m_pSelItem);
		if( pItem )
			SelectItem(pItem, TRUE);
		}
	else
	if( nChar == VK_UP ){	// Move Up
		MerTreeItem* pItem = m_pFirstItem;
		if( m_pSelItem )
			pItem = GetPrevVisibleItem(m_pSelItem);
		if( pItem )
			SelectItem(pItem, TRUE);
		}
	else
	if( nChar == VK_LEFT ){ // Collapse selected item.
		if( m_pSelItem ){
			if( !ExpandItem(m_pSelItem, TRUE, TRUE) ){
				// Movo to parent item. #############
				if( m_pSelItem->parent ){
					SelectItem(m_pSelItem->parent);
					}
				// ##################################
				}
			}
		}
	else
	if( nChar == VK_RIGHT ){ // Expand selected item.
		if( m_pSelItem ){
			ExpandItem(m_pSelItem, FALSE, TRUE);
			}
		}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}

void 
MercuryTreeGrid::OnLButtonDown(UINT nFlags, CPoint point) 
	{
	// Select item by point. #####
	SelectItemByPoint(point);
	// ###########################
	CWnd::OnLButtonDown(nFlags, point);
	}

void 
MercuryTreeGrid::OnLButtonUp(UINT nFlags, CPoint point)
	{
	CWnd::OnLButtonUp(nFlags, point);
	}

void 
MercuryTreeGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
	// Select item by point. #####
	SelectItemByPoint(point);
	// ###########################
	CWnd::OnLButtonDblClk(nFlags, point);
	}

void
MercuryTreeGrid::OnSize(UINT nType, int cx, int cy)
	{
	CWnd::OnSize(nType, cx, cy);

	BOOL bVScroll = m_bVScroll;

	// Update scrollbars. ############################
	if( IsWindow(m_vScroll) || IsWindow(m_hScroll) ){
		UpdateScrollBars();
		}
	// ################################################

	if( !m_bVScroll && bVScroll ){
		
		}
	}

void 
MercuryTreeGrid::OnMouseMove(UINT nFlags, CPoint point)
	{
	CWnd::OnMouseMove(nFlags, point);
	}

BOOL 
MercuryTreeGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL 
MercuryTreeGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
	MoveViewPos(0, -2*(zDelta/120)*m_pGUI->treeGrid.m_nItemHeight, TRUE, TRUE, FALSE);
	return CWnd::OnMouseWheel(nFlags,  zDelta, pt);
	}

UINT 
MercuryTreeGrid::OnGetDlgCode()
	{
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

LRESULT	
MercuryTreeGrid::OnVScroll(WPARAM wParam, LPARAM lParam)
	{
	int		nItemHeight = m_pGUI->treeGrid.m_nItemHeight;
	int		nPos		= (int)wParam;
	int		nPosOld		= (m_nTopItemIndex*nItemHeight + m_ptViewOffset.y);

	CRect	rcClient;	
	UINT	nScrollCode = (UINT)lParam;
	if( m_bVScroll ){
		switch( nScrollCode ){
			case SB_LINEUP:
				MoveViewPos(0, -1*m_pGUI->treeGrid.m_nItemHeight, TRUE, TRUE, FALSE);
				break;
			case SB_LINEDOWN:
				MoveViewPos(0, m_pGUI->treeGrid.m_nItemHeight, TRUE, TRUE, FALSE);
				break;
			case SB_PAGEUP:
				
				GetClientRectMy(rcClient);
				MoveViewPos(0, -1*rcClient.Height(), TRUE, TRUE, FALSE);
				break;
			case SB_PAGEDOWN:
				GetClientRectMy(rcClient);
				MoveViewPos(0, rcClient.Height(), TRUE, TRUE, FALSE);
				break;
			default:
				MoveViewPos(0, nPos - nPosOld, TRUE, FALSE, FALSE);
				break;
			};
		}
	else{
		switch( nScrollCode ){
			case SB_LINELEFT:
				break;
			case SB_LINERIGHT:
				break;
			case SB_PAGELEFT:
				break;
			case SB_PAGERIGHT:
				break;
			default:
				MoveViewPos(nPos - nPosOld, 0, TRUE, FALSE, FALSE);
				break;
			};
		}
	return 0;
	}

BOOL
MercuryTreeGrid::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
	{
	SetFocus();
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

LRESULT	
MercuryTreeGrid::OnHScroll(WPARAM wParam, LPARAM)
	{
	return 0;
	}
*/