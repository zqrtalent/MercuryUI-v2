// ESChildControlLobbyBanner.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildControlLobbyBanner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlLobbyBanner

ESChildControlLobbyBanner::ESChildControlLobbyBanner(){
	m_pFlash					= NULL;
	m_nCurrentImage				= -1;
	m_nImageOffset				= 0;

	m_szBannerButton.SetSize(0, 0);
	m_pBannerButtonImageDef		= NULL;
	m_pBannerButtonImageActive	= NULL;

	m_nChangeImageMS			= 5000;
	m_nDistanceBetweenImages	= 20;	
	m_bVerticalSlide			= true;
	m_szImage	.SetSize(0, 0);
	}

ESChildControlLobbyBanner::~ESChildControlLobbyBanner(){
	}

bool
ESChildControlLobbyBanner::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);

	if( bRet ){
		// Set banner image change timer.
		SetTimer(GetId(), m_nChangeImageMS);
		}

	/*
	m_pFlash = new CFlashWindow();
	m_pFlash->Create(NULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE, rcRect, m_pOwner, nId);
	
	CString sFlashParams;
	//sFlashParams.Format(_T("face=lobby&mfc=1&address=%s&port=%d"), "127.0.0.1", m_server.GetListenPort());
	try
		{
		m_pFlash->put_BackgroundColor(RGB(102,102,102));
		m_pFlash->put_Scale(_T("noscale"));
		m_pFlash->put_FlashVars((LPCTSTR)sFlashParams.GetBuffer());
		m_pFlash->put_Movie((LPCTSTR)_T(m_sFlashMovie));
		//m_pFlash->put_Movie((LPCTSTR)_T("\\\\92.241.65.218\\Poker\\MainGame.swf"));
		//m_pFlash->LoadMovie(1, (LPCTSTR)"D:\\Poker[]\\MainGame.swf");
		//m_pFlash->put_Loop( (VARIANT_BOOL) TRUE);
		//m_pFlash->put_Menu( (VARIANT_BOOL) TRUE);
		m_pFlash->Play();
		}
	catch( ...){ }
	*/
	
	return bRet;
	}

bool
ESChildControlLobbyBanner::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlLobbyBanner::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	if( m_pFlash ){
		m_pFlash->MoveWindow(m_rcRect.left, m_rcRect.top, m_rcRect.Width(), m_rcRect.Height(), TRUE);
		}
	return true;
	}

void
ESChildControlLobbyBanner::SetLogicalPos(float fPosX, float fPosY){
	ESChildControl::SetLogicalPos(fPosX, fPosY);
	}

void
ESChildControlLobbyBanner::SetLogicalSize(float fCX, float fCY){
	ESChildControl::SetLogicalSize(fCX, fCY);
	}

void
ESChildControlLobbyBanner::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

void
ESChildControlLobbyBanner::AddBannerImage(CImage* pImage){
	if( !pImage ) return;
	m_lock.Lock();	// Lock
	if( m_nCurrentImage == -1 )
		m_nCurrentImage = 0;

	m_szImage.SetSize(pImage->GetWidth(), pImage->GetHeight());

	m_listBannerImages.Add(pImage);
	m_lock.Unlock();	// Unlock
	}

void
ESChildControlLobbyBanner::ClearBannerImages(){
	m_lock.Lock();	// Lock
	m_listBannerImages.RemoveAll(true);
	m_lock.Unlock();	// Unlock
	}

void
ESChildControlLobbyBanner::SetBannerButtonImages(CImage* pImageDef, CImage* pImageActive){
	ASSERT(pImageDef && pImageActive);

	m_szBannerButton.SetSize(pImageDef->GetWidth(), pImageDef->GetHeight());
	m_pBannerButtonImageDef		= pImageDef;
	m_pBannerButtonImageActive	= pImageActive;
	}

bool
ESChildControlLobbyBanner::Destroy(){
	ESChildControl::Destroy();

	// Kill banner image change timer.
	KillTimer(GetId());

	if( m_pFlash ){
		delete m_pFlash;
		m_pFlash = NULL;
		}
	return true;
	}

void
ESChildControlLobbyBanner::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	int			nImageCt	= m_listBannerImages.GetCount();
	CImage*		pImage1		= m_listBannerImages.GetAt(m_nCurrentImage);
	CImage*		pImage2		= pImage1;

	if( nImageCt > 1 )
		pImage2				= (m_nCurrentImage == nImageCt - 1) ? m_listBannerImages.GetAt(0) : m_listBannerImages.GetAt(m_nCurrentImage + 1);

	CRect rcImageArea;
	rcImageArea.left	= m_rcRect.left + (m_rcRect.Width() - m_szImage.cx) / 2;
	rcImageArea.top		= m_rcRect.top;
	rcImageArea.right	= rcImageArea.left + m_szImage.cx;
	rcImageArea.bottom	= rcImageArea.top + m_szImage.cy;

	CRect rcImage1, rcImage2;
	if( pImage1 ){
		if( m_bVerticalSlide ){
			rcImage1.left	= rcImageArea.left;
			rcImage1.top	= rcImageArea.top - m_nImageOffset;
			rcImage1.right	= rcImage1.left + m_szImage.cx;
			rcImage1.bottom = rcImage1.top + m_szImage.cy;
			}
		else{
			rcImage1.left	= rcImageArea.left - m_nImageOffset;
			rcImage1.top	= m_rcRect.top;
			rcImage1.right	= rcImage1.left + m_szImage.cx;
			rcImage1.bottom = rcImage1.top + m_szImage.cy;
			}
		}

	if( pImage2 ){
		if( m_bVerticalSlide ){
			rcImage2.left	= rcImage1.left;
			rcImage2.top	= rcImage1.bottom + m_nDistanceBetweenImages;
			rcImage2.right	= rcImage2.left + m_szImage.cx;
			rcImage2.bottom = rcImage2.top + m_szImage.cy;
			}
		else{
			rcImage2.left	= rcImage1.right + m_nDistanceBetweenImages;
			rcImage2.top	= rcImage1.top;
			rcImage2.right	= rcImage2.left + m_szImage.cx;
			rcImage2.bottom = rcImage2.top + m_szImage.cy;
			}
		}

	rcImage1 &= rcImageArea;
	rcImage2 &= rcImageArea;
	
	CRect	rcImageDC, rcInvalid;
	// Draw images. {{
	rcImageDC.left		= (rcImage1.left - pRectClient->left) + pRectDC->left; 
	rcImageDC.top		= (rcImage1.top - pRectClient->top) + pRectDC->top; 
	rcImageDC.right		= rcImageDC.left + rcImage1.Width(); 
	rcImageDC.bottom	= rcImageDC.top + rcImage1.Height(); 

	rcInvalid	= rcImageDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcImageDC, pImage1, false);

	rcImageDC.left		= (rcImage2.left - pRectClient->left) + pRectDC->left; 
	rcImageDC.top		= (rcImage2.top - pRectClient->top) + pRectDC->top; 
	rcImageDC.right		= rcImageDC.left + rcImage2.Width(); 
	rcImageDC.bottom	= rcImageDC.top + rcImage2.Height(); 
	rcInvalid	= rcImageDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcImageDC, pImage2, false);
	// }}


	// Draw buttons. {{
	if( nImageCt > 1 ){
		int nWidthButtons	= m_szBannerButton.cx*nImageCt + 4*(nImageCt - 1);

		CRect rcButtons;
		rcButtons.left		= rcImageArea.left + (rcImageArea.Width() - nWidthButtons)/2;
		rcButtons.top		= rcImageArea.bottom + 4;
		rcButtons.right		= rcButtons.left + nWidthButtons;
		rcButtons.bottom	= rcButtons.top + m_szBannerButton.cy;

		for(int i=0; i<nImageCt; i++){
			rcImageDC.left		= (rcButtons.left - pRectClient->left) + pRectDC->left; 
			rcImageDC.top		= (rcButtons.top - pRectClient->top) + pRectDC->top; 
			rcImageDC.right		= rcImageDC.left + m_szBannerButton.cx; 
			rcImageDC.bottom	= rcImageDC.top + m_szBannerButton.cy; 

			rcInvalid	= rcImageDC & *pRectDC;
			if( !rcInvalid.IsRectEmpty() ){
				PokerGUI::RenderImage(pDC, rcInvalid, rcImageDC, (i == m_nCurrentImage) ? m_pBannerButtonImageActive : m_pBannerButtonImageDef, false);
				}

			rcButtons.left += (4 + m_szBannerButton.cx);
			}
		}
	// }}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildControlLobbyBanner::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlLobbyBanner::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return 1;
	}

void	
ESChildControlLobbyBanner::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlLobbyBanner::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlLobbyBanner::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlLobbyBanner::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyBanner::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyBanner::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyBanner::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyBanner::OnLButtonDblClick(CPoint pt, UINT nFlags){
	OnLButtonDown(pt, nFlags);
	}

void	
ESChildControlLobbyBanner::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyBanner::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}

void
ESChildControlLobbyBanner::OnTimer(UINT nIdEvent){
	KillTimer(nIdEvent);

	if( !m_bVerticalSlide ){
		float fStartX	= 0.0f;
		float fEndX		= (float)(m_szImage.cx + m_nDistanceBetweenImages);
		m_motion.AddMotionObject(this, 500, 20, ESEasyMotionType::MotionQuadraticInOut, ESEasyMotionType::MotionQuadraticInOut, fStartX, 0.0f, fEndX, 0.0f);
		}
	else{
		float fStartY	= 0.0f;
		float fEndY		= (float)(m_szImage.cy + m_nDistanceBetweenImages);
		m_motion.AddMotionObject(this, 500, 20, ESEasyMotionType::MotionQuadraticInOut, ESEasyMotionType::MotionQuadraticInOut, 0.0f, fStartY, 0.0f, fEndY);
		}
	m_motion.StartMotion	(0L, 0L);
	}

void
ESChildControlLobbyBanner::OnMotionStart(){
	m_nImageOffset = 0;
	}

void
ESChildControlLobbyBanner::OnMotionProgress	(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY){
	if( nFrame == nFrameCt ){
		m_nImageOffset = 0;

		m_nCurrentImage ++;
		if( m_nCurrentImage == m_listBannerImages.GetCount() )
			m_nCurrentImage = 0;
		}
	else{
		if( m_bVerticalSlide )
			m_nImageOffset = (int)fPosY;
		else
			m_nImageOffset = (int)fPosX;
		}
	Redraw();
	}

void
ESChildControlLobbyBanner::OnMotionEnd(){
	// Set banner image change timer.
	SetTimer(GetId(), m_nChangeImageMS);
	}

void
ESChildControlLobbyBanner::OnMotionStopped(){
	}