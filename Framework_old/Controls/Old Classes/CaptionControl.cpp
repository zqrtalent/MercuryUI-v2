// MercuryTreeGrid.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "CaptionControl.h"
#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CaptionControl

CaptionControl::CaptionControl() : MercuryBaseWnd()
	{
	}

CaptionControl::~CaptionControl()
	{
	DestroyClassMembers();
	}

BEGIN_MESSAGE_MAP(CaptionControl, MercuryBaseWnd)
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

BOOL 
CaptionControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/)
	{
	if( RegisterWndClass(_T("CaptionControl")) ){
		BOOL bRet =  CWnd::Create(_T("CaptionControl"), _T(""), dwStyle, rect, pParentWnd, nID, pContext);
		if( bRet ){
			}
		return bRet;
		}
	return FALSE;
	}

void
CaptionControl::DestroyClassMembers()
	{
	}

BOOL 
CaptionControl::RegisterWndClass(LPCTSTR lpszClassName)
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

BOOL
CaptionControl::DrawiTunesCaption(CDC* pDC, CRect rcGradient)
	{
	rcGradient.DeflateRect(0, 1);

	COLORREF crBk1			= RGB(133, 133, 133);
	COLORREF crBk2			= RGB(77, 77, 77);
	COLORREF crBk3			= RGB(58, 58, 58);
	COLORREF crBk4			= RGB(66, 66, 68);
	COLORREF crTopLine		= RGB(151, 151, 151);
	COLORREF crBottomLine	= RGB(74, 74, 74);

	CRect rcLine;
	rcLine.SetRect(rcGradient.left, 0, rcGradient.right, 1);
	pDC->FillSolidRect(rcLine,		crTopLine);
	rcLine.SetRect(rcGradient.left, rcGradient.bottom, rcGradient.right, rcGradient.bottom + 1);
	pDC->FillSolidRect(rcLine,	crBottomLine);

	CRect rcGradientDraw;
	rcGradientDraw.SetRect(rcGradient.left, rcGradient.top, rcGradient.right, rcGradient.bottom / 2);
	DrawVGradient(pDC, rcGradientDraw, crBk1, crBk2);
	rcGradientDraw.SetRect(rcGradient.left, rcGradient.bottom / 2, rcGradient.right, rcGradient.bottom);
	DrawVGradient(pDC, rcGradientDraw, crBk3, crBk4);
	return TRUE;
	}

BOOL
CaptionControl::DrawVGradient(CDC* pDC, CRect rcGradient, COLORREF cr1, COLORREF cr2)
	{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap  bitmap, *pOldBitmap;
	bitmap.CreateCompatibleBitmap(pDC, rcGradient.Width(), rcGradient.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	TRIVERTEX		tr[2];
	GRADIENT_RECT	gradient_rc;

	tr[0].Blue					= (WORD)((cr1 & 0x00FF0000)>>8);
	tr[0].Green					= (WORD)((cr1 & 0x0000FF00)); 
	tr[0].Red					= (WORD)((cr1 & 0x000000FF)<<8);
	tr[0].Alpha					= (WORD)((cr1 & 0xFF000000)>>16);
	tr[1].Blue					= (WORD)((cr2 & 0x00FF0000)>>8);
	tr[1].Green					= (WORD)((cr2 & 0x0000FF00)); 
	tr[1].Red					= (WORD)((cr2 & 0x000000FF)<<8);
	tr[1].Alpha					= (WORD)((cr2 & 0xFF000000)>>16);

	tr[0].x						= 0;
	tr[0].y						= 0;

	tr[1].x						= rcGradient.Width();
	tr[1].y						= rcGradient.Height();

	gradient_rc.UpperLeft		= 0;
	gradient_rc.LowerRight		= 1;

	if( GradientFill( memDC.m_hDC, tr, 2, &gradient_rc, 1, GRADIENT_FILL_RECT_V) ){
		pDC->BitBlt	(rcGradient.left, rcGradient.top, rcGradient.Width(), rcGradient.Height(),
					&memDC, 0, 0, SRCCOPY );
		}

	pDC->SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return TRUE;
	}

void
CaptionControl::OnPrePaint(CRect rcRepaint)
	{
	// We mast mark items in repaint rect when clipbox.
	if( !m_bClipBox ) return;
/*
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

void 
CaptionControl::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient)
	{
	CRect	rcClient;
	GetClientRectMy(rcClient);

	DrawiTunesCaption(pDC, rcClient);
/*
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
		}*/
	}

UINT 
CaptionControl::OnGetDlgCode()
	{
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}