// MercuryTreeGrid.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "MercuryScrollBar.h"
#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MercuryScrollBar

MercuryScrollBar::MercuryScrollBar() : MercuryBaseWnd(){
	m_bVscroll			= TRUE;
	m_nPos				= 0;
	m_nPage				= 0;
	m_nMin				= 0;
	m_nMax				= 0;
	m_nThumbHeightMin	= 32;
	m_rcArrowBtnTop		.SetRect(0, 0, 0, 0);
	m_rcArrowBtnBottom	.SetRect(0, 0, 0, 0);
	m_rcArrowBtnLeft	.SetRect(0, 0, 0, 0);
	m_rcArrowBtnRight	.SetRect(0, 0, 0, 0);
	m_rcThumb			.SetRect(0, 0, 0, 0);
	m_ptMouseDown		.SetPoint(0, 0);
	m_bMouseDown		= FALSE;
	m_nHitTest			= 0;
	}

MercuryScrollBar::~MercuryScrollBar()
	{
	}

BEGIN_MESSAGE_MAP(MercuryScrollBar, MercuryBaseWnd)
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_SETCURSOR		()
	ON_WM_MOUSEWHEEL	()
	ON_WM_SIZE			()
	ON_WM_TIMER			()
END_MESSAGE_MAP()

BOOL 
MercuryScrollBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/){
	if( RegisterWndClass(_T("MercuryScrollBar")) ){
		if( dwStyle&WS_VSCROLL ){
			dwStyle &= ~WS_VSCROLL;
			m_bVscroll = TRUE;
			}
		if( dwStyle&WS_HSCROLL ){
			dwStyle &= ~WS_HSCROLL;
			m_bVscroll = FALSE;
			}
		BOOL bRet =  CWnd::Create(_T("MercuryScrollBar"), _T(""), dwStyle, rect, pParentWnd, nID, pContext);
		return bRet;
		}
	return FALSE;
	}

BOOL 
MercuryScrollBar::RegisterWndClass(LPCTSTR lpszClassName){
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
MercuryScrollBar::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient)
	{
	CRect	rcClient;
	GetClientRect(rcClient);
	if( rcClient.IsRectEmpty() || pRectClient->IsRectEmpty() ) 
		return;

	CRect	rcDestDC; // Full destination rect in DC coordinates.
	rcDestDC.left	= pRectDC->left - pRectClient->left;
	rcDestDC.top	= pRectDC->top - pRectClient->top;
	rcDestDC.right	= rcDestDC.left + rcClient.Width();
	rcDestDC.bottom	= rcDestDC.top + rcClient.Height();

	CBitmap bmBkgnd, bmpImg;
	ImageHelper::CreateBitmap32bpp(bmBkgnd, (COLORREF)0, rcClient.Width(), rcClient.Height());

	if( m_bVscroll ){
		// Draw top arrow. #########################################
		CRect rcDraw;
		rcDraw.left			= rcDestDC.left + m_rcArrowBtnTop.left;
		rcDraw.right		= rcDraw.left + m_rcArrowBtnTop.Width();
		rcDraw.top			= rcDestDC.top + m_rcArrowBtnTop.top;
		rcDraw.bottom		= rcDraw.top + m_rcArrowBtnTop.Height();
		CImage* pImage		= &m_pGUI->scrollBar.m_imgVScrollTopNormal;
		if( m_nHitTest == HITTEST_TOP_ARROW )
			pImage			= &m_pGUI->scrollBar.m_imgVScrollTopSel;
		
		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, NULL, FALSE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// #########################################################

		// Draw bottom arrow. ######################################
		rcDraw.left			= rcDestDC.left + m_rcArrowBtnBottom.left;
		rcDraw.right		= rcDraw.left + m_rcArrowBtnBottom.Width();
		rcDraw.top			= rcDestDC.top + m_rcArrowBtnBottom.top;
		rcDraw.bottom		= rcDraw.top + m_rcArrowBtnBottom.Height();
		if( m_nHitTest == HITTEST_BOTTOM_ARROW )
			pImage			= &m_pGUI->scrollBar.m_imgVScrollBottomSel;
		else
			pImage			= &m_pGUI->scrollBar.m_imgVScrollBottomNormal;

		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, NULL, FALSE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// #########################################################

		// Draw middle part. #######################################
		CRect rcMiddle	(m_rcArrowBtnTop.left, m_rcArrowBtnTop.bottom, 
						m_rcArrowBtnTop.right, m_rcArrowBtnBottom.top);
		rcDraw.left			= rcDestDC.left + m_rcArrowBtnTop.left;
		rcDraw.right		= rcDraw.left + m_rcArrowBtnTop.Width();
		rcDraw.top			= rcDestDC.top + m_rcArrowBtnTop.bottom;
		rcDraw.bottom		= rcDraw.top + rcMiddle.Height();
		pImage				= &m_pGUI->scrollBar.m_imgVScrollMiddle;

		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, NULL, FALSE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// #########################################################

		// Draw thumb. ##########################################
		pImage				= &m_pGUI->scrollBar.m_imgVScrollThumb;
		rcDraw.left			= rcDestDC.left + (rcDestDC.Width() - m_pGUI->scrollBar.m_imgVScrollThumb.GetWidth())/2;
		rcDraw.right		= rcDraw.left + m_pGUI->scrollBar.m_imgVScrollThumb.GetWidth();
		rcDraw.top			= rcDestDC.top + m_rcThumb.top;
		rcDraw.bottom		= rcDraw.top + m_rcThumb.Height();

		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, &m_pGUI->scrollBar.m_rcOmitVScrollThumb, TRUE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// ######################################################
		}
	else{
		// Draw left arrow. #########################################
		CRect rcDraw;
		rcDraw.left			= rcDestDC.left + m_rcArrowBtnLeft.left;
		rcDraw.right		= rcDraw.left + m_rcArrowBtnLeft.Width();
		rcDraw.top			= rcDestDC.top + m_rcArrowBtnLeft.top;
		rcDraw.bottom		= rcDraw.top + m_rcArrowBtnLeft.Height();
		CImage* pImage		= &m_pGUI->scrollBar.m_imgHScrollLeftNormal;
		if( m_nHitTest == HITTEST_LEFT_ARROW )
			pImage			= &m_pGUI->scrollBar.m_imgHScrollLeftSel;
		
		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, NULL, FALSE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// #########################################################

		// Draw middle part. #######################################
		CRect rcMiddle	(m_rcArrowBtnLeft.right, m_rcArrowBtnLeft.top, 
						m_rcArrowBtnRight.left, m_rcArrowBtnLeft.bottom);

		rcDraw.left			= rcDestDC.left + m_rcArrowBtnLeft.right;
		rcDraw.right		= rcDraw.left + rcMiddle.Width();
		rcDraw.top			= rcDestDC.top + m_rcArrowBtnLeft.top;
		rcDraw.bottom		= rcDraw.top + m_rcArrowBtnLeft.Height();
		pImage				= &m_pGUI->scrollBar.m_imgHScrollMiddle;

		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, NULL, FALSE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// #########################################################

		// Draw right arrow. ######################################
		rcDraw.left			= rcDestDC.left + m_rcArrowBtnRight.left;
		rcDraw.right		= rcDraw.left + m_rcArrowBtnRight.Width();
		rcDraw.top			= rcDestDC.top + m_rcArrowBtnRight.top;
		rcDraw.bottom		= rcDraw.top + m_rcArrowBtnRight.Height();
		if( m_nHitTest == HITTEST_RIGHT_ARROW )
			pImage			= &m_pGUI->scrollBar.m_imgHScrollRightSel;
		else
			pImage			= &m_pGUI->scrollBar.m_imgHScrollRightNormal;

		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, NULL, FALSE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// #########################################################

		// Draw thumb. ##########################################
		pImage				= &m_pGUI->scrollBar.m_imgHScrollThumb;
		rcDraw.left			= rcDestDC.left + m_rcThumb.left;
		rcDraw.right		= rcDraw.left + m_rcThumb.Width();
		rcDraw.top			= rcDestDC.top + (rcDestDC.Height() - m_pGUI->scrollBar.m_imgHScrollThumb.GetHeight())/2;
		rcDraw.bottom		= rcDraw.top + m_rcThumb.Height();

		bmpImg.Attach(pImage->Detach());
		ImgHlp::DrawBitmap32bppEx(&bmBkgnd, &bmpImg, &rcDraw, pRectDC, &m_pGUI->scrollBar.m_rcOmitHScrollThumb, TRUE);
		pImage->Attach((HBITMAP)bmpImg.Detach());
		// ######################################################
		}

	// From memory image to DC. ################################
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmBkgnd);

	if( !UseDoubleBufferedDC() )
		pDC->BitBlt(pRectClient->left, pRectClient->top, pRectClient->Width(), pRectClient->Height(), &memDC, 
					pRectClient->left, pRectClient->top, SRCCOPY);
	else
		pDC->BitBlt(0, 0, pRectClient->Width(), pRectClient->Height(), &memDC, 0, 0, SRCCOPY);
	
	memDC.DeleteDC();
	bmBkgnd.DeleteObject();
	// #########################################################
	}

void 
MercuryScrollBar::OnLButtonDown(UINT nFlags, CPoint point) {
	CRect	rcArrowBtnTop		= m_rcArrowBtnTop;
	CRect	rcArrowBtnBottom	= m_rcArrowBtnBottom;
	CRect	rcArrowBtnLeft		= m_rcArrowBtnLeft;
	CRect	rcArrowBtnRight		= m_rcArrowBtnRight;	

	// Save mouse down event. ########
	if( m_rcThumb.PtInRect(point) ){
		m_bMouseDown	= TRUE;
		m_nHitTest		= HITTEST_THUMB;
		m_ptMouseDown	= point;
		SetCapture();
		}
	else
	if( m_bVscroll ){
		rcArrowBtnTop.bottom = rcArrowBtnTop.top + m_pGUI->scrollBar.m_nArrowBtnCXCY;
		rcArrowBtnBottom.top = rcArrowBtnBottom.bottom - m_pGUI->scrollBar.m_nArrowBtnCXCY;
		if( rcArrowBtnTop.PtInRect(point) ){
			m_bMouseDown	= TRUE;
			m_nHitTest		= HITTEST_TOP_ARROW;
			m_ptMouseDown	= point;
			SetCapture();
			RedrawArrowButton(HITTEST_TOP_ARROW);

			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_LINEUP);
			if( bRet ){
				SetScrollPos(m_nPos - bRet, TRUE);
				}

			// Start auto scroll timer.
			SetTimer(TIMER_SCROLL_UP, AUTO_SCROLL_START_TIME, NULL);
			}
		else
		if( rcArrowBtnBottom.PtInRect(point) ){
			m_bMouseDown	= TRUE;
			m_nHitTest		= HITTEST_BOTTOM_ARROW;
			m_ptMouseDown	= point;
			SetCapture();
			RedrawArrowButton(HITTEST_BOTTOM_ARROW);

			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_LINEDOWN);
			if( bRet ){
				SetScrollPos(m_nPos + bRet, TRUE);
				}
			// Start auto scroll timer.
			SetTimer(TIMER_SCROLL_DOWN, AUTO_SCROLL_START_TIME, NULL);
			}
		else{
			CRect	rcClient;
			GetClientRect(rcClient);

			CRect	rcPageUp	(rcClient.left, rcClient.top, rcClient.right, m_rcThumb.top);
			CRect	rcPageDown	(rcClient.left, m_rcThumb.bottom, rcClient.right, rcClient.bottom);

			if( rcPageUp.PtInRect(point) ){
				BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_PAGEUP);
				if( (int)bRet != m_nPos){
					SetScrollPos((int)bRet, TRUE);
					}

				m_bMouseDown	= TRUE;
				m_nHitTest		= HITTEST_PAGE_UP;
				m_ptMouseDown	= point;

				// Start auto scroll timer.
				SetTimer(TIMER_SCROLL_PAGEUP, AUTO_SCROLL_START_TIME, NULL);
				}
			else
			if( rcPageDown.PtInRect(point) ){
				BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_PAGEDOWN);
				if( (int)bRet != m_nPos){
					SetScrollPos((int)bRet, TRUE);
					}

				m_bMouseDown	= TRUE;
				m_nHitTest		= HITTEST_PAGE_DOWN;
				m_ptMouseDown	= point;
				// Start auto scroll timer.
				SetTimer(TIMER_SCROLL_PAGEDOWN, AUTO_SCROLL_START_TIME, NULL);
				}
			}
		}
	else{
		if( rcArrowBtnLeft.PtInRect(point) ){
			m_bMouseDown	= TRUE;
			m_nHitTest		= HITTEST_LEFT_ARROW;
			m_ptMouseDown	= point;
			SetCapture();
			RedrawArrowButton(HITTEST_LEFT_ARROW);

			BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_LINELEFT);
			if( bRet ){
				SetScrollPos(m_nPos - bRet, TRUE);
				}
			// Start auto scroll timer.
			SetTimer(TIMER_SCROLL_LEFT, AUTO_SCROLL_START_TIME, NULL);
			}
		else
		if( rcArrowBtnRight.PtInRect(point) ){
			m_bMouseDown	= TRUE;
			m_nHitTest		= HITTEST_RIGHT_ARROW;
			m_ptMouseDown	= point;
			SetCapture();
			RedrawArrowButton(HITTEST_RIGHT_ARROW);

			BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_LINERIGHT);
			if( bRet ){
				SetScrollPos(m_nPos + bRet, TRUE);
				}
			// Start auto scroll timer.
			SetTimer(TIMER_SCROLL_RIGHT, AUTO_SCROLL_START_TIME, NULL);
			}
		else{
			CRect	rcClient;
			GetClientRect(rcClient);

			CRect	rcPageLeft	(rcClient.left, rcClient.top, m_rcThumb.left, rcClient.bottom);
			CRect	rcPageRight	(m_rcThumb.right, rcClient.top, rcClient.right, rcClient.bottom);

			if( rcPageLeft.PtInRect(point) ){
				BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_PAGELEFT);
				if( bRet ){
					SetScrollPos(m_nPos - (int)bRet, TRUE);
					}

				m_bMouseDown	= TRUE;
				m_nHitTest		= HITTEST_PAGE_LEFT;
				m_ptMouseDown	= point;

				// Start auto scroll timer.
				SetTimer(TIMER_SCROLL_PAGELEFT, AUTO_SCROLL_START_TIME, NULL);
				}
			else
			if( rcPageRight.PtInRect(point) ){
				BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_PAGERIGHT);
				if( bRet ){
					SetScrollPos(m_nPos + (int)bRet, TRUE);
					}

				m_bMouseDown	= TRUE;
				m_nHitTest		= HITTEST_PAGE_RIGHT;
				m_ptMouseDown	= point;
				// Start auto scroll timer.
				SetTimer(TIMER_SCROLL_PAGERIGHT, AUTO_SCROLL_START_TIME, NULL);
				}
			}
		}
	// ###############################
	CWnd::OnLButtonDown(nFlags, point);
	}

void 
MercuryScrollBar::OnLButtonUp(UINT nFlags, CPoint point){
	// Release mouse capture. ########
	if( m_bMouseDown ){
		m_bMouseDown	= FALSE;
		UINT nHitTest	= m_nHitTest;
		m_nHitTest		= 0;

		if( m_bVscroll ){
			if( nHitTest == HITTEST_BOTTOM_ARROW ){
				RedrawArrowButton(nHitTest);
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_DOWN);
				}
			else
			if( nHitTest == HITTEST_TOP_ARROW ){
				RedrawArrowButton(nHitTest);
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_UP);
				}
			else
			if( nHitTest == HITTEST_PAGE_DOWN ){
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_PAGEDOWN);
				}
			else
			if( nHitTest == HITTEST_PAGE_UP ){
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_PAGEUP);
				}
			}
		else{
			if( nHitTest == HITTEST_LEFT_ARROW ){
				RedrawArrowButton(nHitTest);
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_LEFT);
				}
			else
			if( nHitTest == HITTEST_RIGHT_ARROW ){
				RedrawArrowButton(nHitTest);
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_RIGHT);
				}
			else
			if( nHitTest == HITTEST_PAGE_LEFT ){
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_PAGELEFT);
				}
			else
			if( nHitTest == HITTEST_PAGE_RIGHT ){
				// Kill scroll timer.
				KillTimer(TIMER_SCROLL_PAGERIGHT);
				}
			}
		ReleaseCapture();
		}
	// ###############################
	CWnd::OnLButtonUp(nFlags, point);
	}

void 
MercuryScrollBar::OnLButtonDblClk(UINT nFlags, CPoint point){
	OnLButtonDown(nFlags, point);
	CWnd::OnLButtonDblClk(nFlags, point);
	}

void 
MercuryScrollBar::OnMouseMove(UINT nFlags, CPoint point)
	{
	CWnd::OnMouseMove(nFlags, point);

	if( m_bMouseDown && nFlags&MK_LBUTTON ){
		if( m_nHitTest == HITTEST_THUMB ){
			if( m_bVscroll ){
				int nMoveY			= (point.y - m_ptMouseDown.y);
				if( nMoveY == 0 ) return;

				CRect rcThumb		= m_rcThumb;
				m_rcThumb.top		+= nMoveY;
				m_rcThumb.bottom	+= nMoveY;

				CPoint ptMouseDown = m_ptMouseDown;
				m_ptMouseDown		= point;

				if( m_rcThumb.top		< m_pGUI->scrollBar.m_nArrowBtnCXCY || 
					m_rcThumb.bottom	> m_rcArrowBtnBottom.bottom - m_pGUI->scrollBar.m_nArrowBtnCXCY ){
					m_rcThumb.top		-= nMoveY;
					m_rcThumb.bottom	-= nMoveY;
					return;
					}

				int	nPosNew	= PosByThumbPos	();
				if( nPosNew == m_nPos ){
					m_rcThumb			= rcThumb;
					m_ptMouseDown		= ptMouseDown;
					}
				else{
					BOOL	bRet = GetParent()->SendMessage(WM_VSCROLL, nPosNew, SB_THUMBTRACK);
					if( bRet ){
						m_nPos = nPosNew;
						CRect rcInvalidate = rcThumb|m_rcThumb;
						RedrawRect(rcInvalidate, TRUE);
						}
					else{
						m_rcThumb		= rcThumb;
						m_ptMouseDown	= ptMouseDown;
						}
					}
				return;
				}
			else{
				int nMoveX			= (point.x - m_ptMouseDown.x);
				if( nMoveX == 0 ) return;

				CRect rcThumb		= m_rcThumb;
				m_rcThumb.left		+= nMoveX;
				m_rcThumb.right		+= nMoveX;
				m_ptMouseDown		= point;

				if( m_rcThumb.left		< m_pGUI->scrollBar.m_nArrowBtnCXCY || 
					m_rcThumb.right		> m_rcArrowBtnRight.right - m_pGUI->scrollBar.m_nArrowBtnCXCY ){
					m_rcThumb.left		-= nMoveX;
					m_rcThumb.right		-= nMoveX;
					return;
					}

				int		nPosNew		= PosByThumbPos	();
				BOOL	bRet		= GetParent()->SendMessage(WM_HSCROLL, nPosNew, SB_THUMBTRACK);
				if( bRet ){
					m_nPos = nPosNew;
					CRect rcInvalidate = rcThumb|m_rcThumb;
					RedrawRect(rcInvalidate, TRUE);
					}
				else
					m_rcThumb = rcThumb;
				return;
				}
			}
		else
		if( m_bVscroll ){
			if( m_nHitTest == HITTEST_TOP_ARROW ){
				if( !m_rcArrowBtnTop.PtInRect(point) ){
					m_bMouseDown	= FALSE;
					m_nHitTest		= 0;
					ReleaseCapture	();
					RedrawArrowButton(HITTEST_TOP_ARROW);
					// Stop auto scroll timer.
					KillTimer(TIMER_SCROLL_UP);
					}
				}
			else
			if( m_nHitTest == HITTEST_BOTTOM_ARROW ){
				if( !m_rcArrowBtnBottom.PtInRect(point) ){
					m_bMouseDown	= FALSE;
					m_nHitTest		= 0;
					ReleaseCapture		();
					RedrawArrowButton	(HITTEST_BOTTOM_ARROW);
					// Stop auto scroll timer.
					KillTimer(TIMER_SCROLL_DOWN);
					}
				}
			}
		else{
			if( m_nHitTest == HITTEST_LEFT_ARROW ){
				if( !m_rcArrowBtnLeft.PtInRect(point) ){
					m_bMouseDown	= FALSE;
					m_nHitTest		= 0;
					ReleaseCapture	();
					RedrawArrowButton(HITTEST_LEFT_ARROW);
					// Stop auto scroll timer.
					KillTimer(TIMER_SCROLL_LEFT);
					}
				}
			else
			if( m_nHitTest == HITTEST_RIGHT_ARROW ){
				if( !m_rcArrowBtnRight.PtInRect(point) ){
					m_bMouseDown	= FALSE;
					m_nHitTest		= 0;
					ReleaseCapture		();
					RedrawArrowButton	(HITTEST_RIGHT_ARROW);
					// Stop auto scroll timer.
					KillTimer(TIMER_SCROLL_RIGHT);
					}
				}
			}
		}
	}

void
MercuryScrollBar::OnTimer(UINT_PTR nIDEvent){
	// Auto scroll events. #######################
	KillTimer(nIDEvent);

	if( m_bVscroll ){
		if( nIDEvent == TIMER_SCROLL_DOWN ){
			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_LINEDOWN);
			if( bRet ){
				SetScrollPos(m_nPos + bRet, TRUE);
				}
			}
		else
		if( nIDEvent == TIMER_SCROLL_UP ){
			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_LINEUP);
			if( bRet ){
				SetScrollPos(m_nPos - bRet, TRUE);
				}
			}
		else
		if( nIDEvent == TIMER_SCROLL_PAGEDOWN ){
			if( m_rcThumb.bottom > m_ptMouseDown.y ){
				m_nHitTest = 0;
				CWnd::OnTimer(nIDEvent);
				return;
				}
			
			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_PAGEDOWN);
			if( bRet ){
				SetScrollPos((int)bRet, TRUE);
				}
			}
		else
		if( nIDEvent == TIMER_SCROLL_PAGEUP ){
			if( m_rcThumb.top < m_ptMouseDown.y ){
				m_nHitTest = 0;
				CWnd::OnTimer(nIDEvent);
				return;
				}

			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_PAGEUP);
			if( bRet ){
				SetScrollPos((int)bRet, TRUE);
				}
			}
		}
	else{
		if( nIDEvent == TIMER_SCROLL_LEFT ){
			BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_LINELEFT);
			if( bRet ){
				SetScrollPos(m_nPos - bRet, TRUE);
				}
			}
		else
		if( nIDEvent == TIMER_SCROLL_RIGHT ){
			BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_LINERIGHT);
			if( bRet ){
				SetScrollPos(m_nPos + bRet, TRUE);
				}
			}
		else
		if( nIDEvent == TIMER_SCROLL_PAGELEFT ){
			if( m_rcThumb.right > m_ptMouseDown.x ){
				m_nHitTest = 0;
				CWnd::OnTimer(nIDEvent);
				return;
				}
			
			BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_PAGELEFT);
			if( bRet ){
				SetScrollPos(m_nPos - (int)bRet, TRUE);
				}
			}
		else
		if( nIDEvent == TIMER_SCROLL_PAGERIGHT ){
			if( m_rcThumb.left < m_ptMouseDown.x ){
				m_nHitTest = 0;
				CWnd::OnTimer(nIDEvent);
				return;
				}
			
			BOOL bRet = GetParent()->SendMessage(WM_HSCROLL, 0, SB_PAGERIGHT);
			if( bRet ){
				SetScrollPos(m_nPos + (int)bRet, TRUE);
				}
			}
		}
	// ##########################################

	SetTimer(nIDEvent, AUTO_SCROLL_TIME, NULL);
	CWnd::OnTimer(nIDEvent);
	}

BOOL 
MercuryScrollBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL 
MercuryScrollBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	return CWnd::OnMouseWheel(nFlags,  zDelta, pt);
	}

void 
MercuryScrollBar::OnSize(UINT nType, int cx, int cy){
	MercuryBaseWnd::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);

	if( m_bVscroll ){
		m_rcArrowBtnTop		.SetRect(rcClient.left, rcClient.top, rcClient.right, 
									rcClient.top + m_pGUI->scrollBar.m_imgVScrollTopNormal.GetHeight());
		m_rcArrowBtnBottom	.SetRect(rcClient.left, rcClient.bottom - m_pGUI->scrollBar.m_imgVScrollBottomNormal.GetHeight(), 
									rcClient.right, rcClient.bottom);
		}
	else{
		m_rcArrowBtnLeft	.SetRect(rcClient.left, rcClient.top, rcClient.left + m_pGUI->scrollBar.m_nArrowBtnCXCY,
									rcClient.bottom);
		m_rcArrowBtnRight	.SetRect(rcClient.right - m_pGUI->scrollBar.m_nArrowBtnCXCY, rcClient.top, rcClient.right,
									rcClient.bottom);
		}
	}

BOOL 
MercuryScrollBar::SetScrollPos(int nPosNew, BOOL bInvalidate /*= FALSE*/){
	if( nPosNew > m_nMax ) return FALSE;
	if( nPosNew < m_nMin )
		nPosNew = m_nMin;

	m_nPos = nPosNew;
	SetScrollInfo(m_nMin, m_nMax, m_nPage);
	return 1;
	}

BOOL
MercuryScrollBar::SetScrollInfo(int nMin, int nMax, int nPage)
	{
	CRect rcClient;
	GetClientRect(rcClient);

	if( m_bVscroll ){
		m_rcArrowBtnTop		.SetRect(rcClient.left, rcClient.top, rcClient.right, 
									rcClient.top + m_pGUI->scrollBar.m_imgVScrollTopNormal.GetHeight());
		m_rcArrowBtnBottom	.SetRect(rcClient.left, rcClient.bottom - m_pGUI->scrollBar.m_imgVScrollBottomNormal.GetHeight(), 
									rcClient.right, rcClient.bottom);
		}
	else{
		m_rcArrowBtnLeft	.SetRect(rcClient.left, rcClient.top, rcClient.left + m_pGUI->scrollBar.m_imgHScrollLeftNormal.GetWidth(),
									rcClient.bottom);
		m_rcArrowBtnRight	.SetRect(rcClient.right - m_pGUI->scrollBar.m_imgHScrollRightNormal.GetWidth(), rcClient.top, rcClient.right,
									rcClient.bottom);
		}

	// Validate scroll infoes. ########################
	int	nPosOld	= m_nPos;
	if( nMin >= nMax || nPage >= (nMax - nMin)) {
		if( m_nPos != m_nMin ){
			m_nPos = m_nMin;
			GetParent()->SendMessage(m_bVscroll ? WM_VSCROLL : WM_HSCROLL, m_nPos, SB_THUMBTRACK);
			}
		return FALSE;
		}
	/*
	if( m_nMin != nMin || m_nMax != nMax ){
		int nLimitPos = (nMax - nPage) + nMin;
		if( m_nPos > m_nMax || m_nPos < m_nMin )
			m_nPos = 0;
		}
	else{
		int nLimitPos = (nMax - nPage) + nMin;
		if( m_nPos > nLimitPos ){
			m_nPos = nLimitPos;
			}
		}*/

	int nLimitPos = (nMax - nPage) + nMin;
	if( m_nPos > nLimitPos ){
		m_nPos = nLimitPos;
		}

	m_nMin	= nMin;
	m_nMax	= nMax;
	m_nPage	= nPage;
	// ################################################

	// Calculate scroll bar thumb rect. ###############
	CRect rcThumbOld;
	if( m_bVscroll ){
		int			nThumbHeightMax = rcClient.Height() - 2*m_pGUI->scrollBar.m_nArrowBtnCXCY;
		int			nThumbHeight	= (((double)m_nPage)/(m_nMax - m_nMin))*((double)nThumbHeightMax);
		if( nThumbHeight < m_nThumbHeightMin )
			nThumbHeight = m_nThumbHeightMin;
		int			nThumbHeightOld	= m_rcThumb.Height();

		rcThumbOld	= m_rcThumb;
		int nThumbTopPos	= ThumbPosByPos();
		m_rcThumb.left		= 0;
		m_rcThumb.top		= nThumbTopPos;
		m_rcThumb.right		= rcClient.Width();
		m_rcThumb.bottom	= min(m_rcThumb.top + nThumbHeight, m_rcArrowBtnBottom.bottom - m_pGUI->scrollBar.m_nArrowBtnCXCY);

		if( m_rcThumb.top + nThumbHeight > m_rcArrowBtnBottom.top ){
			m_rcThumb.top -= (nThumbHeight - nThumbHeightOld);
			}
		if( m_rcThumb.top <= 0 )
			m_rcThumb.top = nThumbTopPos;
		}
	else{
		int			nThumbWidthMax	= rcClient.Width() - 2*m_pGUI->scrollBar.m_nArrowBtnCXCY;
		int			nThumbWidth		= (((double)m_nPage)/(m_nMax - m_nMin))*((double)nThumbWidthMax);
		if( nThumbWidth < m_nThumbHeightMin )
			nThumbWidth = m_nThumbHeightMin;
		int			nThumbWidthOld	= m_rcThumb.Width();

		rcThumbOld	= m_rcThumb;
		int nThumbLeftPos	= ThumbPosByPos();
		m_rcThumb.left		= nThumbLeftPos;
		m_rcThumb.top		= 0;
		m_rcThumb.right		= min(m_rcThumb.left + nThumbWidth, m_rcArrowBtnRight.right - m_pGUI->scrollBar.m_nArrowBtnCXCY);
		m_rcThumb.bottom	= rcClient.Height();

		if( m_rcThumb.left + nThumbWidth > m_rcArrowBtnRight.left ){
			m_rcThumb.left -= (nThumbWidth - nThumbWidthOld);
			}

		if( m_rcThumb.left <= 0 )
			m_rcThumb.left = nThumbLeftPos;
		}

	if( rcThumbOld != m_rcThumb )
		RedrawRect(rcThumbOld|m_rcThumb, TRUE);

	if( m_nPos != nPosOld ){
		GetParent()->SendMessage(m_bVscroll ? WM_VSCROLL : WM_HSCROLL, m_nPos, SB_THUMBTRACK);
		}
	// #################################################
	return 1;
	}

int
MercuryScrollBar::PosByThumbPos(){
	if( m_bVscroll ){
		int nPos			= m_nMin;
		int nThumbPos		= m_rcThumb.top - m_pGUI->scrollBar.m_nArrowBtnCXCY;
		int nPosMax			= ((m_nMax - m_nMin) - m_nPage);
		int	nThumbPosMax	= (m_rcArrowBtnBottom.bottom - 2*m_pGUI->scrollBar.m_nArrowBtnCXCY) - m_rcThumb.Height();
		if( nPosMax <= 0 ) return m_nMin;

		double	dPosPerUnit	= (double)nThumbPosMax / nPosMax;
		nPos				= ((double)(nThumbPos / dPosPerUnit))*100;
		
		if( nPos < m_nPos*100 ){
			if( nPos%100 > 1 )
				nPos = nPos/100 + 1;
			else
				nPos /= 100;
			}
		else
			nPos /= 100;

// Old
//		nPos				= dDiff*nThumbPos + m_nMin;
		return nPos;
		}
	else{
		int nPos			= m_nMin;
		int nThumbPos		= m_rcThumb.left - m_pGUI->scrollBar.m_nArrowBtnCXCY;
		int nPosMax			= ((m_nMax - m_nMin) - m_nPage);
		int	nThumbPosMax	= (m_rcArrowBtnRight.right - 2*m_pGUI->scrollBar.m_nArrowBtnCXCY) - m_rcThumb.Width();
		if( nPosMax <= 0 ) return m_nMin;

		double dDiff		= nPosMax / ((double)nThumbPosMax);
		nPos				= dDiff*nThumbPos + m_nMin;
		return nPos;
		}
	}

int
MercuryScrollBar::ThumbPosByPos()
	{
	if( m_bVscroll ){
		int nPos			= m_nPos;
		int nThumbPos		= m_pGUI->scrollBar.m_nArrowBtnCXCY;
		int	nThumbPosMax	= (m_rcArrowBtnBottom.bottom - 2*m_pGUI->scrollBar.m_nArrowBtnCXCY) - max(m_rcThumb.Height(), m_nThumbHeightMin);
		int nPosMax			= (int)GetLimit();
		if( nThumbPosMax <= 0 ) 
			return m_pGUI->scrollBar.m_nArrowBtnCXCY;

		double dDiff		= ((double)nThumbPosMax) / nPosMax;
		nThumbPos			= min(dDiff*nPos, nThumbPosMax) + m_pGUI->scrollBar.m_nArrowBtnCXCY;
		return nThumbPos;
		}
	else{
		int nPos			= m_nPos;
		int nThumbPos		= m_pGUI->scrollBar.m_nArrowBtnCXCY;
		int	nThumbPosMax	= (m_rcArrowBtnRight.right - 2*m_pGUI->scrollBar.m_nArrowBtnCXCY) - max(m_rcThumb.Width(), m_nThumbHeightMin);
		int nPosMax			= (int)GetLimit();
		if( nThumbPosMax <= 0 ) 
			return m_pGUI->scrollBar.m_nArrowBtnCXCY;

		double dDiff		= ((double)nThumbPosMax) / nPosMax;
		nThumbPos			= min(dDiff*nPos, nThumbPosMax) + m_pGUI->scrollBar.m_nArrowBtnCXCY;
		return nThumbPos;
		}
	}

void
MercuryScrollBar::RedrawArrowButton(UINT nBtnHitTest)
	{
	if( m_bVscroll ){
		if( nBtnHitTest == HITTEST_TOP_ARROW ){
			CRect rcInvalidate	(m_rcArrowBtnTop.left, m_rcArrowBtnTop.top, 
				m_rcArrowBtnTop.right, m_rcArrowBtnTop.top + m_pGUI->scrollBar.m_imgVScrollTopSel.GetHeight());
			RedrawRect(rcInvalidate, TRUE);
			}
		else
		if( nBtnHitTest == HITTEST_BOTTOM_ARROW ){
			CRect rcInvalidate	(m_rcArrowBtnBottom.left, m_rcArrowBtnBottom.bottom - m_pGUI->scrollBar.m_imgVScrollBottomSel.GetHeight(), 
							m_rcArrowBtnBottom.right, m_rcArrowBtnBottom.bottom);
			RedrawRect(rcInvalidate, TRUE);
			}
		}
	else{
		if( nBtnHitTest == HITTEST_LEFT_ARROW ){
			CRect rcInvalidate	(m_rcArrowBtnLeft.left, m_rcArrowBtnLeft.top, 
								m_rcArrowBtnLeft.left + m_pGUI->scrollBar.m_imgHScrollLeftSel.GetWidth(), m_rcArrowBtnLeft.bottom);
			RedrawRect(rcInvalidate, TRUE);
			}
		else
		if( nBtnHitTest == HITTEST_RIGHT_ARROW ){
			CRect rcInvalidate	(m_rcArrowBtnRight.right - m_pGUI->scrollBar.m_imgHScrollLeftSel.GetWidth(), m_rcArrowBtnRight.top, 
								m_rcArrowBtnRight.right, m_rcArrowBtnRight.bottom);
			RedrawRect(rcInvalidate, TRUE);
			}
		}
	}