// ESChildScrollBar.cpp : implementation file
//

#include "ESChildScrollBar.h"
#include "MercuryGUI.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildScrollBar

ESChildScrollBar::ESChildScrollBar(bool bVScroll /*= true*/){
	m_sClassName		= _T("ESChildScrollBar");
	m_bMouseDown		= false;
	m_bHover			= false;
	m_bLogicalPos		= false;

	m_bVscroll			= bVScroll;
	m_nPos				= 0;
	m_nPage				= 0;
	m_nMin				= 0;
	m_nMax				= 0;
	m_nThumbHeightMin	= 24;
	m_rcArrowBtnTop		.SetRect(0, 0, 0, 0);
	m_rcArrowBtnBottom	.SetRect(0, 0, 0, 0);
	m_rcArrowBtnLeft	.SetRect(0, 0, 0, 0);
	m_rcArrowBtnRight	.SetRect(0, 0, 0, 0);
	m_rcThumb			.SetRect(0, 0, 0, 0);
	m_ptMouseDown		.SetPoint(0, 0);
	m_nHitTest			= 0;

	MercuryGUI* pGUI					= MercuryGUI::GetInstance();
	m_pImageVScrollThumb				= &pGUI->scrollBar.m_imgVScrollThumb;
	m_pImageVScrollArrowBottomSel		= &pGUI->scrollBar.m_imgVScrollBottomSel;
	m_pImageVScrollArrowBottomNormal	= &pGUI->scrollBar.m_imgVScrollBottomNormal;
	m_pImageVScrollMiddle				= &pGUI->scrollBar.m_imgVScrollMiddle;
	m_pImageVScrollArrowTopSel			= &pGUI->scrollBar.m_imgVScrollTopSel;
	m_pImageVScrollArrowTopNormal		= &pGUI->scrollBar.m_imgVScrollTopNormal;

	m_pImageHScrollThumb				= &pGUI->scrollBar.m_imgHScrollThumb;
	m_pImageHScrollArrowRightSel		= &pGUI->scrollBar.m_imgHScrollRightSel;
	m_pImageHScrollArrowRightNormal		= &pGUI->scrollBar.m_imgHScrollRightNormal;
	m_pImageHScrollMiddle				= &pGUI->scrollBar.m_imgHScrollMiddle;
	m_pImageHScrollArrowLeftSel			= &pGUI->scrollBar.m_imgHScrollLeftSel;
	m_pImageHScrollArrowLeftNormal		= &pGUI->scrollBar.m_imgHScrollLeftNormal;

	m_pImageDivider						= &pGUI->scrollBar.m_imgScrollBarDivider;

	m_nArrowBtnCX						= pGUI->scrollBar.m_nArrowBtnCXCY;
	m_nArrowBtnCY						= pGUI->scrollBar.m_nArrowBtnCXCY;

	m_rcVThumbOmitBorder				= pGUI->scrollBar.m_rcOmitVScrollThumb;
	m_rcHThumbOmitBorder				= pGUI->scrollBar.m_rcOmitHScrollThumb;
	}

ESChildScrollBar::~ESChildScrollBar(){
	}

bool
ESChildScrollBar::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rRect, pOwner, pParent);
	CalculateRect();
	return true;
	}

bool
ESChildScrollBar::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	CalculateRect();
	return true;
	}

bool
ESChildScrollBar::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	CalculateRect();
	return true;
	}

bool
ESChildScrollBar::Destroy(){
	ESChildControl::Destroy();
	m_bMouseDown = false;
	return true;
	}

void
ESChildScrollBar::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	CalculateRect();
	}

bool
ESChildScrollBar::VisibilityChanged(bool bVisible){
	if( bVisible && !IsVisible() ){
		if( m_pParent )
			return m_pParent->IsScrollBarVisible(this);
		}
	return true;
	}

void
ESChildScrollBar::CalculateRect(){
	if( m_bVscroll ){
		m_rcArrowBtnTop		.SetRect(m_rRect.left, m_rRect.top, m_rRect.right, m_rRect.top + m_pImageVScrollArrowTopNormal->GetHeight());
		m_rcArrowBtnBottom	.SetRect(m_rRect.left, m_rRect.bottom - m_pImageVScrollArrowBottomNormal->GetHeight(), m_rRect.right, m_rRect.bottom);

		// Calculate thumb button rect. {{
		int	nThumbHeightMax = m_rRect.Height() - 2*m_nArrowBtnCY;
		int	nThumbHeight	= (((double)m_nPage)/(m_nMax - m_nMin))*((double)nThumbHeightMax);
		nThumbHeight		= (nThumbHeight < m_nThumbHeightMin) ? m_nThumbHeightMin : nThumbHeight;
		int	nThumbHeightOld	= m_rcThumb.Height();
		int nThumbImageCX	= m_pImageVScrollThumb->GetWidth();

		_Rect	rcThumbOld	= m_rcThumb;
		int nThumbTopPos	= ThumbTopOffsetByScrollPos(nThumbHeight);
		m_rcThumb.left		= m_rRect.left + (m_rRect.Width() - nThumbImageCX)/2;
		m_rcThumb.top		= nThumbTopPos;
		m_rcThumb.right		= m_rcThumb.left + nThumbImageCX;
		m_rcThumb.bottom	= std::min(m_rcThumb.top + nThumbHeight, m_rcArrowBtnBottom.bottom - m_nArrowBtnCY);

		if( m_rcThumb.top + nThumbHeight > (m_rcArrowBtnBottom.bottom - m_nArrowBtnCY) )
			m_rcThumb.bottom = (m_rcArrowBtnBottom.bottom - m_nArrowBtnCY);
		if( m_rcThumb.top <= 0 )
			m_rcThumb.top = nThumbTopPos;
		// }}

		if( m_rRect.Height() <= (m_rcArrowBtnTop.Height() + m_rcArrowBtnBottom.Height()) ){
			m_rcArrowBtnTop		.SetRect(m_rRect.left, m_rRect.top, m_rRect.right, m_rRect.top + m_rRect.Height()/2);
			m_rcArrowBtnBottom	.SetRect(m_rRect.left, m_rRect.bottom - m_rRect.Height()/2, m_rRect.right, m_rRect.bottom);
			m_rcThumb.SetRect(0, 0, 0, 0);
			}
		}
	else{
		m_rcArrowBtnLeft	.SetRect(m_rRect.left, m_rRect.top, m_rRect.left + m_pImageHScrollArrowRightNormal->GetWidth(), m_rRect.bottom);
		m_rcArrowBtnRight	.SetRect(m_rRect.right - m_pImageHScrollArrowLeftNormal->GetWidth(), m_rRect.top, m_rRect.right, m_rRect.bottom);

		// Calculate thumb button rect. {{
		int	nThumbWidthMax	= m_rRect.Width() - 2*m_nArrowBtnCX;
		int	nThumbWidth		= (((double)m_nPage)/(m_nMax - m_nMin))*((double)nThumbWidthMax);
		nThumbWidth			= (nThumbWidth < m_nThumbHeightMin) ? m_nThumbHeightMin : nThumbWidth;
		int	nThumbWidthOld	= m_rcThumb.Width();
		int nThumbImageCY	= m_pImageHScrollThumb->GetHeight();

		_Rect	rcThumbOld	= m_rcThumb;
		int nThumbTopPos	= ThumbTopOffsetByScrollPos(nThumbWidth);

		m_rcThumb.left		= nThumbTopPos;
		m_rcThumb.top		= m_rcArrowBtnLeft.top + (m_rRect.Height() - nThumbImageCY)/2;
		m_rcThumb.right		= m_rcThumb.left + nThumbWidth;
		m_rcThumb.bottom	= m_rcThumb.top + nThumbImageCY;

		if( m_rcThumb.left + nThumbWidth > (m_rcArrowBtnRight.right - m_nArrowBtnCX) )
			m_rcThumb.right = m_rcArrowBtnRight.right - m_nArrowBtnCX;
		// }}

		if( m_rRect.Width() <= (m_rcArrowBtnLeft.Width() + m_rcArrowBtnRight.Width()) ){
			m_rcArrowBtnLeft	.SetRect(m_rRect.left, m_rRect.top, m_rRect.left + m_rRect.Width()/2, m_rRect.bottom);
			m_rcArrowBtnRight	.SetRect(m_rRect.right - m_rRect.Width()/2, m_rRect.top, m_rRect.right, m_rRect.bottom);
			m_rcThumb.SetRect(0, 0, 0, 0);
			}
		}
	}

int
ESChildScrollBar::ScrollPosByThumbTopOffset(){
	MercuryGUI* pGUI		= MercuryGUI::GetInstance();
	if( m_bVscroll ){
		int nPos			= m_nMin;
		int nThumbPos		= (m_rcThumb.top - m_rRect.top - pGUI->scrollBar.m_nArrowBtnCXCY);
		int nPosMax			= ((m_nMax - m_nMin) - m_nPage);
		int	nThumbPosMax	= (m_rRect.Height() - 2*pGUI->scrollBar.m_nArrowBtnCXCY) - m_rcThumb.Height();
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
		return nPos;
		}
	else{
		int nPos			= m_nMin;
		int nThumbPos		= m_rcThumb.left -  m_rRect.left - pGUI->scrollBar.m_nArrowBtnCXCY;
		int nPosMax			= ((m_nMax - m_nMin) - m_nPage);
		int	nThumbPosMax	= (m_rRect.Width() - 2*pGUI->scrollBar.m_nArrowBtnCXCY) - m_rcThumb.Width();
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
		return nPos;
		}
	}

int
ESChildScrollBar::ThumbTopOffsetByScrollPos(int nThumbWidthHeight){
	MercuryGUI* pGUI = MercuryGUI::GetInstance();
	if( m_bVscroll ){
		int nPos			= m_nPos;
		int nThumbPos		= m_rRect.top + m_nArrowBtnCY;
		int	nThumbPosMax	= (m_rRect.Height() - 2*m_nArrowBtnCY) - std::max(nThumbWidthHeight, m_nThumbHeightMin);
		int nPosMax			= (int)((m_nMax - m_nPage) + m_nMin);
		if( nThumbPosMax <= 0 ) 
			return nThumbPos;

		double dDiff		= ((double)nThumbPosMax) / nPosMax;
		nThumbPos			= std::min((int)(dDiff*nPos), nThumbPosMax) + nThumbPos;
		return nThumbPos;
		}
	else{
		int nPos			= m_nPos;
		int nThumbPos		= m_rRect.left + m_nArrowBtnCX;
		int	nThumbPosMax	= (m_rRect.Width() - 2*m_nArrowBtnCX) - std::max(nThumbWidthHeight, m_nThumbHeightMin);
		int nPosMax			= (int)((m_nMax - m_nPage) + m_nMin);
		if( nThumbPosMax <= 0 ) 
			return nThumbPos;

		double dDiff		= ((double)nThumbPosMax) / nPosMax;
		nThumbPos			= std::min((int)(dDiff*nPos), nThumbPosMax) + nThumbPos;
		return nThumbPos;
		}
	}

int
ESChildScrollBar::HitTest(_Point pt){
	if( m_rcThumb.PtInRect(pt) )
		return HITTEST_THUMB;

	if( m_bVscroll ){
		if( m_rcArrowBtnTop.PtInRect(pt) )
			return HITTEST_TOP_ARROW;
		if( m_rcArrowBtnBottom.PtInRect(pt) )
			return HITTEST_BOTTOM_ARROW;

		if( pt.x > m_rRect.left && pt.x < m_rRect.right ){
			if( pt.y > m_rcThumb.bottom && pt.y < m_rcArrowBtnBottom.top )
				return HITTEST_PAGE_DOWN;
			if( pt.y < m_rcThumb.top && pt.y > m_rcArrowBtnTop.bottom )
				return HITTEST_PAGE_UP;
			}
		}
	else{
		if( m_rcArrowBtnLeft.PtInRect(pt) )
			return HITTEST_LEFT_ARROW;
		if( m_rcArrowBtnRight.PtInRect(pt) )
			return HITTEST_RIGHT_ARROW;

		if( pt.y > m_rRect.top && pt.y < m_rRect.bottom ){
			if( pt.x > m_rcThumb.right && pt.x < m_rcArrowBtnRight.left )
				return HITTEST_PAGE_RIGHT;
			if( pt.x < m_rcThumb.left && pt.x > m_rcArrowBtnLeft.right )
				return HITTEST_PAGE_LEFT;
			}
		}
	return -1;
	}

bool
ESChildScrollBar::RedrawRectByHitTest(int nHitTest, _Rect& rRect){
	_Rect* pArrRects[] = {&m_rcArrowBtnLeft, &m_rcArrowBtnRight, &m_rcArrowBtnTop, &m_rcArrowBtnBottom, NULL, NULL, NULL, NULL, &m_rcThumb};
	if( nHitTest < 1 || nHitTest > (sizeof(pArrRects) / sizeof(_Rect*)) )
		return false;
	if( pArrRects[nHitTest - 1] == NULL )
		return false;
	rRect = *pArrRects[nHitTest - 1];
	return true;
	}

bool
ESChildScrollBar::SendEventByHitTest(int nHitTest, bool bStartAutoScrollTimer){
	if( !m_pParent ) return false;

	int nArrTimerIds[]	= {TIMER_SCROLL_LEFT, TIMER_SCROLL_RIGHT, TIMER_SCROLL_UP, TIMER_SCROLL_DOWN,  TIMER_SCROLL_PAGEDOWN, TIMER_SCROLL_PAGEUP, TIMER_SCROLL_PAGELEFT, TIMER_SCROLL_PAGERIGHT, -1};
	int nArrCodes[]		= {SB_LINELEFT, SB_LINERIGHT, SB_LINEUP, SB_LINEDOWN,  SB_PAGEDOWN, SB_PAGEUP, SB_PAGELEFT, SB_PAGERIGHT, -1};
	if( nHitTest < 1 || nHitTest > (sizeof(nArrCodes) / sizeof(int)) )
		return false;

	if( nArrCodes[nHitTest - 1] == -1 )
		return false;

	m_pParent->OnScrollBarEvent(m_bVscroll, nArrCodes[nHitTest - 1], this);

	// Auto scroll timer.
	if( bStartAutoScrollTimer )
		SetTimer(nArrTimerIds[nHitTest - 1], AUTO_SCROLL_START_TIME);
	return true;
	}

bool 
ESChildScrollBar::SetScrollPos(int nPosNew, bool bInvalidate){
	if( nPosNew > m_nMax ) return false;
	if( nPosNew < m_nMin )
		nPosNew = m_nMin;

	_Rect rcThumbOld = m_rcThumb;
	int	nPosOld = m_nPos;
	m_nPos = nPosNew;
	if( !SetScrollInfo(m_nMin, m_nMax, m_nPage, false) ){
		m_nPos = nPosOld;
		return false;
		}

	if( bInvalidate && rcThumbOld != m_rcThumb ){
		_Rect rcRedraw = rcThumbOld|m_rcThumb;
		m_pOwner->RedrawRect(rcRedraw, 1);
		}

	if( m_nPos != nPosOld )
		m_pParent->OnScrollBarEvent(m_bVscroll, SB_THUMBTRACK, this);
	return true;
	}

bool
ESChildScrollBar::SetScrollInfo(int nMin, int nMax, int nPage, bool bInvalidate){
	// Validate scroll infoes. ########################
	int	nPosOld	= m_nPos;
	if( nMin >= nMax || nPage >= (nMax - nMin) || nPage <= 0 ) {
		if( m_nPos != m_nMin ){
			m_nPos = m_nMin;
			m_pParent->OnScrollBarEvent(m_bVscroll, SB_THUMBTRACK, this);
			}
		return false;
		}

	int nLimitPos = (nMax - nPage) + nMin;
	if( m_nPos > nLimitPos )
		m_nPos = nLimitPos;

	m_nMin	= nMin;
	m_nMax	= nMax;
	m_nPage	= nPage;
	// ################################################

	_Rect rcThumbOld = m_rcThumb;
	CalculateRect();

	if( bInvalidate && rcThumbOld != m_rcThumb ){
		_Rect rcRedraw = rcThumbOld|m_rcThumb;
		m_pOwner->RedrawRect(rcRedraw, 0);
		}

	if( m_nPos != nPosOld )
		m_pParent->OnScrollBarEvent(m_bVscroll, SB_THUMBTRACK, this);
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildScrollBar::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	if( m_bVscroll )
		DrawVScrollBar(pDC, pRectDC, pRectClient);
	else
		DrawHScrollBar(pDC, pRectDC, pRectClient);
	}

void
ESChildScrollBar::DrawVScrollBar(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcInvalid;
	Image*		pImage		= NULL;

	// Draw top arrow. {{
	_Rect	rcTopArrowDC;
	rcTopArrowDC.left		= (m_rcArrowBtnTop.left - pRectClient->left) + pRectDC->left; 
	rcTopArrowDC.top		= (m_rcArrowBtnTop.top - pRectClient->top) + pRectDC->top; 
	rcTopArrowDC.right		= rcTopArrowDC.left + m_rcArrowBtnTop.Width(); 
	rcTopArrowDC.bottom		= rcTopArrowDC.top + m_rcArrowBtnTop.Height(); 

	pImage					= m_pImageVScrollArrowTopNormal;
	if( m_nHitTest == HITTEST_TOP_ARROW )
		pImage				= m_pImageVScrollArrowTopSel;

	rcInvalid = rcTopArrowDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage(pDC, rcInvalid, rcTopArrowDC, true, false, ScaleMode::ScaleMode_Bilinier);
	// }}

	// Draw bottom arrow. {{
	_Rect	rcBottomArrowDC;
	rcBottomArrowDC.left	= (m_rcArrowBtnBottom.left - pRectClient->left) + pRectDC->left; 
	rcBottomArrowDC.top		= (m_rcArrowBtnBottom.top - pRectClient->top) + pRectDC->top; 
	rcBottomArrowDC.right	= rcBottomArrowDC.left + m_rcArrowBtnBottom.Width(); 
	rcBottomArrowDC.bottom	= rcBottomArrowDC.top + m_rcArrowBtnBottom.Height(); 

	pImage					= m_pImageVScrollArrowBottomNormal;
	if( m_nHitTest == HITTEST_BOTTOM_ARROW )
		pImage				= m_pImageVScrollArrowBottomSel;

	rcInvalid = rcBottomArrowDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage(pDC, rcInvalid, rcBottomArrowDC, true, false, ScaleMode::ScaleMode_Bilinier);
	// }}

	// Draw middle part. {{
	_Rect	rcBkgndDC;
	rcBkgndDC.left			= (m_rcArrowBtnTop.left - pRectClient->left) + pRectDC->left; 
	rcBkgndDC.top			= rcTopArrowDC.bottom; 
	rcBkgndDC.right			= rcBkgndDC.left + m_rcArrowBtnTop.Width(); 
	rcBkgndDC.bottom		= rcBottomArrowDC.top; 
	pImage					= m_pImageVScrollMiddle;

	rcInvalid = rcBkgndDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage_OmitBorder(pDC, rcInvalid, rcBkgndDC, _Rect(0, 0, 0, 0), FALSE);
	// }}

	// Draw thumb button. {{
	_Rect	rcThumbBtnDC;
	rcThumbBtnDC.left		= (m_rcThumb.left - pRectClient->left) + pRectDC->left; 
	rcThumbBtnDC.top		= (m_rcThumb.top - pRectClient->top) + pRectDC->top; 
	rcThumbBtnDC.right		= rcThumbBtnDC.left + m_rcThumb.Width(); 
	rcThumbBtnDC.bottom		= rcThumbBtnDC.top + m_rcThumb.Height(); 

	pImage					= m_pImageVScrollThumb;
	rcInvalid = rcThumbBtnDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage_OmitBorder(pDC, rcInvalid, rcThumbBtnDC, &m_rcVThumbOmitBorder);
	// }}
	}

void
ESChildScrollBar::DrawHScrollBar(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcInvalid;
	Image*		pImage		= NULL;

	// Draw left arrow. {{
	_Rect	rcLeftArrowDC;
	rcLeftArrowDC.left		= (m_rcArrowBtnLeft.left - pRectClient->left) + pRectDC->left; 
	rcLeftArrowDC.top		= (m_rcArrowBtnLeft.top - pRectClient->top) + pRectDC->top; 
	rcLeftArrowDC.right		= rcLeftArrowDC.left + m_rcArrowBtnLeft.Width(); 
	rcLeftArrowDC.bottom	= rcLeftArrowDC.top + m_rcArrowBtnLeft.Height(); 

	pImage					= m_pImageHScrollArrowLeftNormal;
	if( m_nHitTest == HITTEST_LEFT_ARROW )
		pImage				= m_pImageHScrollArrowLeftSel;

	rcInvalid = rcLeftArrowDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage(pDC, rcInvalid, rcLeftArrowDC, true, false, ScaleMode::ScaleMode_Bilinier);
	// }}

	// Draw right arrow. {{
	_Rect	rcRightArrowDC;
	rcRightArrowDC.left		= (m_rcArrowBtnRight.left - pRectClient->left) + pRectDC->left; 
	rcRightArrowDC.top		= (m_rcArrowBtnRight.top - pRectClient->top) + pRectDC->top; 
	rcRightArrowDC.right	= rcRightArrowDC.left + m_rcArrowBtnRight.Width(); 
	rcRightArrowDC.bottom	= rcRightArrowDC.top + m_rcArrowBtnRight.Height(); 

	pImage					= m_pImageHScrollArrowRightNormal;
	if( m_nHitTest == HITTEST_RIGHT_ARROW )
		pImage				= m_pImageHScrollArrowRightSel;

	rcInvalid = rcRightArrowDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage(pDC, rcInvalid, rcRightArrowDC, true, false, ScaleMode::ScaleMode_Bilinier);
	// }}

	// Draw middle part. {{
	_Rect	rcBkgndDC;
	rcBkgndDC.left			= rcLeftArrowDC.right; 
	rcBkgndDC.top			= rcLeftArrowDC.top; 
	rcBkgndDC.right			= rcRightArrowDC.left; 
	rcBkgndDC.bottom		= rcRightArrowDC.bottom; 
	pImage					= m_pImageHScrollMiddle;

	rcInvalid = rcBkgndDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage_OmitBorder(pDC, rcInvalid, rcBkgndDC, _Rect(0, 0, 0, 0), FALSE);
	// }}
	
	// Draw thumb button. {{
	_Rect	rcThumbBtnDC;
	rcThumbBtnDC.left		= (m_rcThumb.left - pRectClient->left) + pRectDC->left; 
	rcThumbBtnDC.top		= (m_rcThumb.top - pRectClient->top) + pRectDC->top; 
	rcThumbBtnDC.right		= rcThumbBtnDC.left + m_rcThumb.Width(); 
	rcThumbBtnDC.bottom		= rcThumbBtnDC.top + m_rcThumb.Height(); 
	pImage					= m_pImageHScrollThumb;

	rcInvalid = rcThumbBtnDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage->RenderImage_OmitBorder(pDC, rcInvalid, rcThumbBtnDC, &m_rcHThumbOmitBorder);
	// }}
	}

void	
ESChildScrollBar::OnMouseHover(_Point pt, UINT nFlags){
	if( m_bMouseDown && nFlags&MK_LBUTTON ){
		if( m_nHitTest == HITTEST_THUMB ){
			if( m_bVscroll ){
				int nMoveY			= (pt.y - m_ptMouseDown.y);
				if( nMoveY == 0 ) return;

				_Rect rcThumb		= m_rcThumb;
				m_rcThumb.top		+= nMoveY;
				m_rcThumb.bottom	+= nMoveY;

				_Point ptMouseDown = m_ptMouseDown;
				m_ptMouseDown		= pt;

				if( m_rcThumb.top		< (m_rRect.top + m_nArrowBtnCY) || 
					m_rcThumb.bottom	> m_rcArrowBtnBottom.bottom - m_nArrowBtnCY ){
					m_rcThumb.top		-= nMoveY;
					m_rcThumb.bottom	-= nMoveY;
					return;
					}

				int	nPosNew	= ScrollPosByThumbTopOffset();
				if( nPosNew == m_nPos ){
					m_rcThumb			= rcThumb;
					m_ptMouseDown		= ptMouseDown;
					}
				else{
					m_nPos				= nPosNew;
					m_pParent->OnScrollBarEvent(m_bVscroll, SB_THUMBTRACK, this);
					_Rect rcInvalidate	= rcThumb|m_rcThumb;
					m_pOwner->RedrawRect(rcInvalidate, 1);
					}
				}
			else{
				int nMoveX			= (pt.x - m_ptMouseDown.x);
				if( nMoveX == 0 ) return;

				_Rect rcThumb		= m_rcThumb;
				m_rcThumb.left		+= nMoveX;
				m_rcThumb.right		+= nMoveX;
				m_ptMouseDown		= pt;

				if( m_rcThumb.left		< (m_rRect.left + m_nArrowBtnCX) || 
					m_rcThumb.right		> m_rcArrowBtnRight.right - m_nArrowBtnCX ){
					m_rcThumb.left		-= nMoveX;
					m_rcThumb.right		-= nMoveX;
					return;
					}

				int	nPosNew	= ScrollPosByThumbTopOffset();
				m_pParent->OnScrollBarEvent(m_bVscroll, SB_THUMBTRACK, this);
				m_nPos				= nPosNew;
				_Rect rcInvalidate	= rcThumb|m_rcThumb;
				m_pOwner->RedrawRect(rcInvalidate, 1);
				}
			}
		}
	}

BOOL	
ESChildScrollBar::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	return FALSE;
//	SetCursor(LoadCursor(NULL, IDC_HAND));
//	return 1;
	}

void	
ESChildScrollBar::OnMouseLeave(UINT nFlags){
	m_bHover = false;
//	if( m_bUnderLine ){
//		Redraw();
//		}
	}

void	
ESChildScrollBar::OnMouseEnter(UINT nFlags){
	m_bHover = true;
//	if( m_bUnderLine ){
//		Redraw();
//		}
	}

void
ESChildScrollBar::OnLButtonDown(_Point pt, UINT nFlags){
	m_bMouseDown	= true;
	m_nHitTest		= HitTest(pt);
	m_ptMouseDown	= pt;
	m_rcThumbDown	= m_rcThumb;

	_Rect rcRedraw;
	if( RedrawRectByHitTest(m_nHitTest, rcRedraw) ){
		m_pOwner->RedrawRect(rcRedraw, 1);
		}

	SendEventByHitTest(m_nHitTest, true);
	}

void
ESChildScrollBar::OnLButtonUp(_Point pt, UINT nFlags){
	m_bMouseDown = false;

	// Kill auto scroll timer by hittest. {{
	int nArrTimerIds[]	= {TIMER_SCROLL_LEFT, TIMER_SCROLL_RIGHT, TIMER_SCROLL_UP, TIMER_SCROLL_DOWN,  TIMER_SCROLL_PAGEDOWN, TIMER_SCROLL_PAGEUP, TIMER_SCROLL_PAGELEFT, TIMER_SCROLL_PAGERIGHT, -1};
	if( m_nHitTest >= 1 || m_nHitTest <= (sizeof(nArrTimerIds) / sizeof(int)) ){
		if( nArrTimerIds[m_nHitTest - 1] != -1 ){
			KillTimer(nArrTimerIds[m_nHitTest - 1]);
			}
		}
	// }}

	_Rect rcRedraw;
	if( RedrawRectByHitTest(m_nHitTest, rcRedraw) ){
		m_nHitTest = 0;
		m_pOwner->RedrawRect(rcRedraw, 1);
		}
	else
		m_nHitTest = 0;
	}

void	
ESChildScrollBar::OnLButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildScrollBar::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildScrollBar::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildScrollBar::OnLButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildScrollBar::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildScrollBar::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void
ESChildScrollBar::OnTimer(UINT nIDEvent){
	// Auto scroll events. #######################
	KillTimer(nIDEvent);

	if( m_bVscroll ){
		if( nIDEvent == TIMER_SCROLL_DOWN ){

			SendEventByHitTest(m_nHitTest);
			/*
			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_LINEDOWN);
			if( bRet ){
				SetScrollPos(m_nPos + bRet, TRUE);
				}*/
			}
		else
		if( nIDEvent == TIMER_SCROLL_UP ){
			SendEventByHitTest(m_nHitTest);
			/*
			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_LINEUP);
			if( bRet ){
				SetScrollPos(m_nPos - bRet, TRUE);
				}*/
			}
		else
		if( nIDEvent == TIMER_SCROLL_PAGEDOWN ){
			SendEventByHitTest(m_nHitTest);
			/*
			if( m_rcThumb.bottom > m_ptMouseDown.y ){
				m_nHitTest = 0;
				CWnd::OnTimer(nIDEvent);
				return;
				}
			
			BOOL bRet = GetParent()->SendMessage(WM_VSCROLL, 0, SB_PAGEDOWN);
			if( bRet ){
				SetScrollPos((int)bRet, TRUE);
				}*/
			}
		else
		if( nIDEvent == TIMER_SCROLL_PAGEUP ){
			if( m_rcThumb.top < m_ptMouseDown.y ){
				m_nHitTest = 0;
				return;
				}

			SendEventByHitTest(m_nHitTest);
			}
		}/*
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
		}*/
	// ##########################################
	SetTimer(nIDEvent, AUTO_SCROLL_TIME);
	}