#include "stdafx.h"
#include "XHeaderCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl

IMPLEMENT_DYNCREATE(CXHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CXHeaderCtrl, CHeaderCtrl)
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_SYSCOLORCHANGE()
	ON_MESSAGE			(HDM_INSERTITEMA, OnInsertItem)
	ON_MESSAGE			(HDM_INSERTITEMW, OnInsertItem)
	ON_MESSAGE			(HDM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE			(HDM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE			(HDM_LAYOUT, OnLayout)
	ON_WM_LBUTTONDBLCLK	()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXHeaderCtrl::CXHeaderCtrl()
	{
	m_cr3DHighLight   = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow      = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace        = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText       = ::GetSysColor(COLOR_BTNTEXT);

	m_pListCtrl       = NULL;
	m_nFormat         = DT_DEFAULT;
	m_rgbText         = m_crBtnText;
	m_bDividerLines   = TRUE;
	m_bDoubleBuffer   = TRUE;
	m_iSpacing        = 6;
	m_sizeArrow.cx    = 8;
	m_sizeArrow.cy    = 8;
	m_sizeImage.cx    = 0;
	m_sizeImage.cy    = 0;
	m_bStaticBorder   = TRUE;
	m_nDontDropCursor = 0;
	m_bResizing       = FALSE;
	m_nClickFlags     = 0;

	m_crGradient1		= 0L;
	m_crGradient2		= 0L;
	m_bGradient			= FALSE;

	m_hBmpCheckOn		= NULL;
	m_hBmpCheckOff		= NULL;
	}

CXHeaderCtrl::~CXHeaderCtrl()
	{
	if( m_hBmpCheckOn )
		DeleteObject(m_hBmpCheckOn);
	if( m_hBmpCheckOff )
		DeleteObject(m_hBmpCheckOff);
	}

void
CXHeaderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
	CHeaderCtrl::OnLButtonDblClk(nFlags, point);
	}

BOOL 
CXHeaderCtrl::ModifyProperty(WPARAM wParam, LPARAM lParam)
	{
	switch( wParam )
		{
		case FH_PROPERTY_SPACING:
			m_iSpacing = (int)lParam;
			break;

		case FH_PROPERTY_ARROW:
			m_sizeArrow.cx = LOWORD(lParam);
			m_sizeArrow.cy = HIWORD(lParam);
			break;

		case FH_PROPERTY_STATICBORDER:
			m_bStaticBorder = (BOOL)lParam;
			break;

		case FH_PROPERTY_DONTDROPCURSOR:
			m_nDontDropCursor = (UINT)lParam;
			break;

		default:
			return FALSE;
		}

	Invalidate();
	return TRUE;
	}

void 
CXHeaderCtrl::DrawCtrl(CDC* pDC)
	{
	CRect rectClip;
	if( pDC->GetClipBox(&rectClip) == ERROR )
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

	if( !m_bGradient )
		pDC->FillSolidRect(rectClip, m_cr3DFace);
	else
		{
		// Fill gradient. #################################################
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);

		CRect rcGradient = rectClip;
		CBitmap  bitmap, *pOldBitmap;
		bitmap.CreateCompatibleBitmap(pDC, rcGradient.Width(), rcGradient.Height());
		pOldBitmap = memDC.SelectObject(&bitmap);

		TRIVERTEX		tr[2];
		GRADIENT_RECT	gradient_rc;

		tr[0].Blue					= (WORD)((m_crGradient1 & 0x00FF0000)>>8);
		tr[0].Green					= (WORD)((m_crGradient1 & 0x0000FF00)); 
		tr[0].Red					= (WORD)((m_crGradient1 & 0x000000FF)<<8);
		tr[0].Alpha					= (WORD)((m_crGradient1 & 0xFF000000)>>16);
		tr[1].Blue					= (WORD)((m_crGradient2 & 0x00FF0000)>>8);
		tr[1].Green					= (WORD)((m_crGradient2 & 0x0000FF00)); 
		tr[1].Red					= (WORD)((m_crGradient2 & 0x000000FF)<<8);
		tr[1].Alpha					= (WORD)((m_crGradient2 & 0xFF000000)>>16);

		tr[0].x						= 0;
		tr[0].y						= 0;

		tr[1].x						= rcGradient.Width();
		tr[1].y						= rcGradient.Height() - 1;

		gradient_rc.UpperLeft		= 0;
		gradient_rc.LowerRight		= 1;

		if( GradientFill( memDC.m_hDC, tr, 2, &gradient_rc, 1, GRADIENT_FILL_RECT_V) )
			{
			pDC->BitBlt	(rcGradient.left, rcGradient.top, rcGradient.Width(), rcGradient.Height() - 1,
						&memDC, 0, 0, SRCCOPY );

			// Line after gradient. ##########################
			CPen pen(PS_SOLID, 1, m_crGradient1);
			CPen* penOld	= pDC->SelectObject(&pen);
			pDC->MoveTo(rcGradient.left, rcGradient.bottom - 1);
			pDC->LineTo(rcGradient.right, rcGradient.bottom - 1);
			pDC->SelectObject(penOld);
			// ################################################
			}

	//	pDC->SelectObject(pOldBitmap);
		memDC.DeleteDC();
		// ################################################################
		}

	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen	penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen	penShadow(PS_SOLID, 1, m_cr3DShadow);

	CPen*	pPen	= pDC->GetCurrentPen();
	CFont*	pFont	= pDC->SelectObject(GetFont());

	pDC->SetBkMode		(TRANSPARENT);
	pDC->SetTextColor	(m_crBtnText);

	int iWidth = 0;
	for( int i = 0; i < iItems; i++ )
		{
		int iItem = OrderToIndex(i);

		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hditem;
		hditem.mask			= HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP|HDI_ORDER;
		hditem.pszText		= szText;
		hditem.cchTextMax	= sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));
		VERIFY(GetItemRect(iItem, rectItem));
		if( rectItem.right >= rectClip.left || rectItem.left <= rectClip.right )
			{
			if( hditem.fmt & HDF_OWNERDRAW )
				{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType		= ODT_BUTTON;
				disItem.CtlID		= GetDlgCtrlID();
				disItem.itemID		= iItem;
				disItem.itemAction	= ODA_DRAWENTIRE;
				disItem.itemState	= 0;
				disItem.hwndItem	= m_hWnd;
				disItem.hDC			= pDC->m_hDC;
				disItem.rcItem		= rectItem;
				disItem.itemData	= 0;
				DrawItem(&disItem);
				}
			else
				{
				rectItem.DeflateRect(m_iSpacing, 0);
				DrawItem(pDC, rectItem, &hditem);
				rectItem.InflateRect(m_iSpacing, 0);

				//if (m_nClickFlags & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags & HHT_ONHEADER)
				//	pDC->InvertRect(rectItem);
				}

			if( i < iItems-1 )
				{
				// draw divider lines
				if( m_bDividerLines )
					{
					pDC->SelectObject	(&penShadow);
					pDC->MoveTo			(rectItem.right-1, rectItem.top+2);
					pDC->LineTo			(rectItem.right-1, rectItem.bottom-2);

					pDC->SelectObject	(&penHighLight);
					pDC->MoveTo			(rectItem.right, rectItem.top+2);
					pDC->LineTo			(rectItem.right, rectItem.bottom-2);
					}
				}
			}
		iWidth += hditem.cxy;
		}

	if( iWidth > 0 )
		{
		rectClient.right = rectClient.left + iWidth;
		pDC->Draw3dRect(rectClient, m_cr3DHighLight, m_cr3DShadow);
		}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

	penHighLight.DeleteObject();
	penShadow	.DeleteObject();
	}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void 
CXHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
	{
	ASSERT(FALSE);  // must override for self draw header controls
	}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void 
CXHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
	{
	ASSERT(lphdi->mask & HDI_FORMAT);

	// If have checkbox column. #########################
	BOOL bCheck = FALSE;
	if( GetColCheckState(lphdi->iOrder, bCheck) && m_hBmpCheckOn && m_hBmpCheckOff )
		{
		HBITMAP	hBmp = bCheck ? m_hBmpCheckOn : m_hBmpCheckOff;
		BITMAP	bmp;
		GetObject(hBmp, sizeof(BITMAP), &bmp);

		CDC srcDC;
		srcDC.CreateCompatibleDC(NULL);
		srcDC.SelectObject		(hBmp);
		// #######################

		int		nTop = rect.top + (rect.Height() - bmp.bmHeight)/2;
		CRect	rcImage	(rect.left, nTop, 
						rect.left + bmp.bmWidth, nTop + bmp.bmHeight);

		BOOL	bRet = pDC->BitBlt(rcImage.left, rcImage.top, bmp.bmWidth, bmp.bmHeight, &srcDC, 0, 0, SRCCOPY);
		if( bRet )
			rect.left += bmp.bmWidth + 1;
		}
	// ##################################################

	// If have sort status. #############################
	BOOL bAsc = TRUE;
	if( GetColSortStatus(lphdi->iOrder, bAsc) )
		{
		int		nTop = rect.top + (rect.bottom - rect.top - SORT_SIGN_HEIGHT)/2;
		CRect	rcSortSign	(rect.right - SORT_SIGN_WIDTH - SORT_SIGN_RIGHT_MARGIN, 
							nTop, rect.right - SORT_SIGN_RIGHT_MARGIN, nTop + SORT_SIGN_HEIGHT);

		
		if( DrawSortSign(pDC, rcSortSign, bAsc, GetSysColor(COLOR_GRAYTEXT)) )
			{
			rect.right = rcSortSign.left;
			}
		}
	// ##################################################
	int			iWidth	= 0;
	CBitmap*	pBitmap = NULL;
	BITMAP		BitmapInfo;
	if( lphdi->fmt&HDF_BITMAP )
		{
		ASSERT(lphdi->mask & HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if (pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
		}

	rect.left += ((iWidth = DrawImage(pDC, rect, lphdi, FALSE)) != 0) ? 
		iWidth + m_iSpacing : 0;

	rect.right -= ((iWidth = DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) != 0) ? 
		iWidth + m_iSpacing : 0;
	DrawText(pDC, rect, lphdi);
	}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int 
CXHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
	{
	int iWidth = 0;
	if( lphdi->iImage != XHEADERCTRL_NO_IMAGE )
		{
		CImageList* pImageList = GetImageList();

		if( pImageList && (rect.Width() > 0) )
			{
			POINT point;

			point.y = rect.CenterPoint().y - ((m_sizeImage.cy+1) >> 1);

			if (bRight)
				point.x = rect.right - m_sizeImage.cx;
			else
				point.x = rect.left;

			SIZE size;
			size.cx = rect.Width() < m_sizeImage.cx ? rect.Width() : m_sizeImage.cx;
			size.cy = m_sizeImage.cy;

			// save image list background color
			COLORREF rgb = pImageList->GetBkColor();

			// set image list background color to same as header control
			pImageList->SetBkColor	(pDC->GetBkColor());
			pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));
			pImageList->SetBkColor	(rgb);

			iWidth = m_sizeImage.cx;
			}
		else 
		if( rect.Width() > 0 )
			{
			// no image list, just draw checkbox

			CRect chkboxrect = rect;

			// center the checkbox

			int h = 13;	// height and width are the same
			chkboxrect.right	= chkboxrect.left + h;
			chkboxrect.top		= rect.top + (rect.Height() - h) / 2;
			chkboxrect.bottom	= chkboxrect.top + h;

			// fill rect inside checkbox with white
			COLORREF rgbBackground = pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->FillSolidRect(&chkboxrect, ::GetSysColor(COLOR_WINDOW));

			// draw border
			CBrush brush(RGB(51, 102, 153));
			pDC->FrameRect(&chkboxrect, &brush);

			if (lphdi->iImage == XHEADERCTRL_CHECKED_IMAGE)
				{
				CPen blackpen(PS_SOLID, 1, RGB(51,153,51));
				CPen * pOldPen = pDC->SelectObject(&blackpen);

				// draw the checkmark
				int x = chkboxrect.left + 9;
				ASSERT(x < chkboxrect.right);
				int y = chkboxrect.top + 3;
				int i = 0;
				for (i = 0; i < 4; i++)
				{
					pDC->MoveTo(x, y);
					pDC->LineTo(x, y+3);
					x--;
					y++;
				}
				for (i = 0; i < 3; i++)
				{
					pDC->MoveTo(x, y);
					pDC->LineTo(x, y+3);
					x--;
					y--;
				}

				if (pOldPen)
					pDC->SelectObject(pOldPen);
			}

			pDC->SetBkColor(rgbBackground);

			iWidth = chkboxrect.Width();
			}
		}
	return iWidth;
	}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap
int CXHeaderCtrl::DrawBitmap(	CDC* pDC, 
								CRect rect, 
								LPHDITEM lphdi, 
								CBitmap* pBitmap, 
								BITMAP* pBitmapInfo, 
								BOOL bRight)
	{
	UNUSED_ALWAYS(lphdi);

	int iWidth = 0;

	if( pBitmap )
		{
		iWidth = pBitmapInfo->bmWidth;
		if (iWidth <= rect.Width() && rect.Width() > 0)
			{
			POINT point;

			point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight >> 1);

			if (bRight)
				point.x = rect.right - iWidth;
			else
				point.x = rect.left;

			CDC dc;
			if (dc.CreateCompatibleDC(pDC) == TRUE) 
				{
				CBitmap * pOldBitmap = dc.SelectObject(pBitmap);
				iWidth = pDC->BitBlt(
					point.x, point.y, 
					pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, 
					&dc, 
					0, 0, 
					SRCCOPY
				) ? iWidth:0;
				dc.SelectObject(pOldBitmap);
				}
			else 
				iWidth = 0;
			}
		else
			iWidth = 0;
		}
	return iWidth;
	}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int 
CXHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
	{
	CSize size = pDC->GetTextExtent(lphdi->pszText);

#if 0  // -----------------------------------------------------------
	pDC->SetTextColor(RGB(0,0,255));

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
	{
		size = pDC->GetTextExtent(lphdi->pszText);

		// always center column headers
		pDC->DrawText(lphdi->pszText, -1, rect, 
			DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
	}
#endif // -----------------------------------------------------------

	//+++

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
		{
		pDC->SetTextColor(m_rgbText);

		UINT nFormat = m_nFormat;

		if (nFormat == DT_DEFAULT)
			{
			// default to whatever alignment the column is set to

			if (lphdi->fmt & LVCFMT_CENTER)
				nFormat = DT_CENTER;
			else if (lphdi->fmt & LVCFMT_RIGHT)
				{
				nFormat = DT_RIGHT;
				rect.right -= 2;

				}
			else
				{
				nFormat = DT_LEFT;
				rect.left += 2;
				}
			}

		pDC->DrawText(lphdi->pszText, -1, rect, 
			nFormat | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
		}

	size.cx = rect.Width() > size.cx ? size.cx : rect.Width();
	return size.cx > 0 ? size.cx : 0;
	}

///////////////////////////////////////////////////////////////////////////////
// OnInsertItem
LRESULT 
CXHeaderCtrl::OnInsertItem(WPARAM, LPARAM)
	{
	return Default();
	}

///////////////////////////////////////////////////////////////////////////////
// OnDeleteItem
LRESULT 
CXHeaderCtrl::OnDeleteItem(WPARAM, LPARAM)
	{
	return Default();
	}

///////////////////////////////////////////////////////////////////////////////
// OnSetImageList
LRESULT 
CXHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
	{
	CImageList* pImageList;
	pImageList = CImageList::FromHandle((HIMAGELIST)lParam);

	IMAGEINFO info;
	if( pImageList->GetImageInfo(0, &info) )
		{
		m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
		m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
		}
	return Default();
	}

///////////////////////////////////////////////////////////////////////////////
// OnLayout
LRESULT 
CXHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
	{
	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;
	if( m_bStaticBorder )
		lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER)*2;
	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
	}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void 
CXHeaderCtrl::OnSysColorChange() 
	{
	TRACE0(_T("in CXHeaderCtrl::OnSysColorChange\n"));

	CHeaderCtrl::OnSysColorChange();
	
	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
	}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL 
CXHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
	{
	UNUSED_ALWAYS(pDC);
	return TRUE;
	}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void 
CXHeaderCtrl::OnPaint() 
	{
    CPaintDC dc(this);
    if( m_bDoubleBuffer )
		{
        CMemDC MemDC(&dc);
		if( MemDC.m_hDC )
			DrawCtrl(&MemDC);
		else
			DrawCtrl(&dc);
		}
    else
        DrawCtrl(&dc);
	}

BOOL
CXHeaderCtrl::DrawSortSign(CDC* pDC, CRect rect, BOOL bAsc, COLORREF cr)
	{
	if( rect.left < 0 || rect.right < 0 || rect.left > rect.right )
		return FALSE;

	CPen pen(PS_SOLID, 1, cr);
	pDC->SelectObject(&pen);
	
	if( bAsc )
		{
		CPoint pt(rect.left + rect.Width() / 2, rect.top);
		for( int nLoop=0; nLoop<rect.Height(); nLoop ++ )
			{
			pDC->MoveTo(pt.x - nLoop, pt.y + nLoop);
			pDC->LineTo(pt.x + nLoop, pt.y + nLoop);
			}
		}
	else
		{
		CPoint pt(rect.left + rect.Width() / 2, rect.bottom);
		for( int nLoop=0; nLoop<rect.Height(); nLoop ++ )
			{
			pDC->MoveTo(pt.x - nLoop, pt.y - nLoop);
			pDC->LineTo(pt.x + nLoop, pt.y - nLoop);
			}
		}
	return TRUE;
	}
