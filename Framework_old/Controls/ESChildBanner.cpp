// ESChildBanner.cpp : implementation file
//

#include "ESChildBanner.h"
#include "../Path/Path.h"

/////////////////////////////////////////////////////////////////////////////
// ESBannerImageInfo

ESBannerImageInfo::ESBannerImageInfo(){
	}

BEGIN_DECLARE_VAR(ESBannerImageInfo, Serializable)
DECLARE_VAR(_T("ESBannerImageInfo"),	_T(""),				VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Image File Path"),	_T("IMAGE"),		VariableType::VT_String, offsetof(ESBannerImageInfo,m_sImage), false, false, false)
	DECLARE_VAR(_T("Action Name"),		_T("ACTION_NAME"),	VariableType::VT_String, offsetof(ESBannerImageInfo,m_sActionName), false, false, false)
	DECLARE_VAR(_T("URL"),				_T("URL"),			VariableType::VT_String, offsetof(ESBannerImageInfo,m_sURL), false, false, false)
END_DECLARE_VAR()

ESBannerImageInfo::~ESBannerImageInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// ESChildBanner

ESChildBanner::ESChildBanner(){
	m_sClassName				= _T("ESChildBanner");
	m_nCurrentImage				= -1;
	m_nImageOffset				= 0;

	m_nImageChangeMS			= 5000;
	m_nDistanceBetweenImages	= 20;	
	m_bVerticalSlide			= true;

	m_szBannerImage				.SetSize(200, 200);

	m_szSwitchButton			.SetSize(0, 0);
	m_rcSwitchButtonsBound		.SetRect(0, 0, 0, 0);
	m_nDistanceBetweenButtons	= 5;
	}

BEGIN_DECLARE_VAR(ESChildBanner, ESChildControl)
	DECLARE_VAR(_T("ESChildBanner"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Banner Images"),			_T(""), VariableType::VT_None, offsetof(ESChildBanner,m_imageInfos), false, true, true)
	DECLARE_VAR(_T("Change MS"),				_T(""), VariableType::VT_Int32, offsetof(ESChildBanner,m_nImageChangeMS), false, false, false)
	DECLARE_VAR(_T("Distance Between Images"),	_T(""), VariableType::VT_Int32, offsetof(ESChildBanner,m_nDistanceBetweenImages), false, false, false)
	DECLARE_VAR(_T("Vertical Slide"),			_T(""), VariableType::VT_Bool, offsetof(ESChildBanner,m_bVerticalSlide), false, false, false)
	DECLARE_VAR(_T("Banner _Size"),				_T(""), VariableType::VT_Size, offsetof(ESChildBanner,m_szBannerImage), false, false, false)
	DECLARE_VAR(_T("Switch _Size"),				_T(""), VariableType::VT_Size, offsetof(ESChildBanner,m_szSwitchButton), false, false, false)
	DECLARE_VAR(_T("Switch Image Normal"),		_T(""), VariableType::VT_Image, offsetof(ESChildBanner,m_imageSwitchButtonNormal), false, false, false)
	DECLARE_VAR(_T("Switch Image Click"),		_T(""), VariableType::VT_Image, offsetof(ESChildBanner,m_imageSwitchButtonClick), false, false, false)
END_DECLARE_VAR()

ESChildBanner::~ESChildBanner(){
	}

bool
ESChildBanner::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		if( m_pOwner->IsDesignerMode() ){
			// Register variable modify callback methods. {{
			AddVariableModifyCallBack(&m_imageInfos,					(VariableValueModifyCallback)&ESChildBanner::BannerImagesModify);
			AddVariableModifyCallBack(&m_imageSwitchButtonNormal,		(VariableValueModifyCallback)&ESChildBanner::BannerSwitchModify);
			AddVariableModifyCallBack(&m_imageSwitchButtonClick,		(VariableValueModifyCallback)&ESChildBanner::BannerSwitchModify);
			AddVariableModifyCallBack(&m_szSwitchButton,				(VariableValueModifyCallback)&ESChildBanner::BannerSwitchModify);
			// }}
			}

		UpdateBannerImages	();
		UpdateSwitchButtons	();
		}
	return bRet;
	}

bool
ESChildBanner::Destroy(){
	ESChildControl::Destroy();

	// Kill banner image change timer.
	KillTimer(GetId());

	if( !m_imageSwitchButtonNormal.IsNull() )
		m_imageSwitchButtonNormal.Destroy();

	if( !m_imageSwitchButtonClick.IsNull() )
		m_imageSwitchButtonClick.Destroy();

	ClearBannerImages();
	return true;
	}

bool
ESChildBanner::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildBanner::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	UpdateSwitchButtons();
	return true;
	}

void
ESChildBanner::SetLogicalPos(float fPosX, float fPosY){
	ESChildControl::SetLogicalPos(fPosX, fPosY);
	}

void
ESChildBanner::SetLogicalSize(float fCX, float fCY){
	ESChildControl::SetLogicalSize(fCX, fCY);
	}

void
ESChildBanner::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildBanner::AddBannerImage(_string sImageFile, _string sActionName /*= _T("")*/, _string sURL /*= _T("")*/){
    
	if( !Path::FileExists(sImageFile) )
		return false; // Image file not exists !!!

	Image* pImage = Image::LoadImage(sImageFile, -1, -1);
	if( !pImage ) 
		return false; // Image load error !!!

	m_lock.Lock();	// Lock
	if( m_nCurrentImage == -1 )
		m_nCurrentImage = 0;

	ESBannerImageInfo* pInfo	= new ESBannerImageInfo();
	pInfo->m_sImage				= sImageFile;
	pInfo->m_sActionName		= sActionName;
	pInfo->m_sURL				= sURL;
	m_imageInfos.Add(pInfo);

	m_listBannerImages.Add(pImage);
	m_lock.Unlock();	// Unlock
	return true;
	}

int
ESChildBanner::ClearBannerImages(){
	m_lock.Lock();	// Lock
	int nResult = m_listBannerImages.RemoveAll(true);
	m_imageInfos.RemoveAll(true);
	m_lock.Unlock();	// Unlock
	return nResult;
	}

bool
ESChildBanner::UpdateBannerImages(){
	m_lock.Lock();	// Lock
	KillTimer(GetId());
	m_motion.StopMotion();
	m_nCurrentImage = -1;
	m_nImageOffset	= 0;
	m_listBannerImages.RemoveAll(true);

	for(int i=0; i<m_imageInfos.GetCount(); i++){
		ESBannerImageInfo* pInfo	= m_imageInfos.GetAt(i);
		if( !pInfo ) continue;
		Image* pImage				= Image::LoadImage(pInfo->m_sImage, -1, -1);
		if( pImage && !pImage->IsNull() ){
			m_listBannerImages.Add(pImage);
			}
		}

	if( m_listBannerImages.GetCount() > 0 ){
		m_nCurrentImage = 0;
		// Set banner image change timer.
		SetTimer(GetId(), m_nImageChangeMS);
		}
	m_lock.Unlock();	// Unlock
	return true;
	}

bool
ESChildBanner::UpdateSwitchButtons(){
	m_lock.Lock();	// Lock
	int nCt = m_listBannerImages.GetCount();
	m_lock.Unlock();	// Unlock

	if( nCt == 0 ){
		m_rcSwitchButtonsBound.SetRect(0, 0, 0, 0);
		}
	else{
		if( m_imageSwitchButtonNormal.IsNull() || m_imageSwitchButtonClick.IsNull() ){
			m_rcSwitchButtonsBound.SetRect(0, 0, 0, 0);
			}
		else{
			_Size szBound;
			szBound.cx = m_imageSwitchButtonNormal.GetWidth() + (m_imageSwitchButtonNormal.GetWidth() + m_nDistanceBetweenButtons)*(nCt - 1);
			szBound.cy = m_imageSwitchButtonNormal.GetHeight();

			_Rect rcClient = GetClientRect();
			m_rcSwitchButtonsBound.left		= rcClient.left + (rcClient.Width() - szBound.cx)/2;
			m_rcSwitchButtonsBound.top		= rcClient.bottom - szBound.cy - 2;
			m_rcSwitchButtonsBound.right	= m_rcSwitchButtonsBound.left + szBound.cx;
			m_rcSwitchButtonsBound.bottom	= m_rcSwitchButtonsBound.top - szBound.cy;
			}
		}
	return true;
	}

int
ESChildBanner::GetSwitchButtonByPoint(_Point pt){
	if( m_imageSwitchButtonNormal.IsNull() )
		return -1;
	if( m_rcSwitchButtonsBound.PtInRect(pt) ){
		_Rect	rcButton(m_rcSwitchButtonsBound.left, m_rcSwitchButtonsBound.top, m_rcSwitchButtonsBound.left + m_imageSwitchButtonNormal.GetWidth(), m_rcSwitchButtonsBound.bottom);
		int		nLoop = 0;
		while( pt.x <= rcButton.left ){
			if( pt.x >= rcButton.left && pt.x <= rcButton.right ){
				return nLoop;
				}
			rcButton.left	= rcButton.right + m_nDistanceBetweenButtons;
			rcButton.right	= rcButton.left + m_imageSwitchButtonNormal.GetWidth();
			nLoop ++;
			}
		}
	return -1;
	}

void
ESChildBanner::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	int			nImageCt	= m_listBannerImages.GetCount();
	Image*		pImage1		= m_listBannerImages.GetAt(m_nCurrentImage);
	Image*		pImage2		= pImage1;

	if( nImageCt > 1 )
		pImage2	= (m_nCurrentImage == nImageCt - 1) ? m_listBannerImages.GetAt(0) : m_listBannerImages.GetAt(m_nCurrentImage + 1);

	_Rect rImageArea;
	rImageArea.left		= m_rRect.left + (m_rRect.Width() - m_szBannerImage.cx) / 2;
	rImageArea.top		= m_rRect.top;
	rImageArea.right	= rImageArea.left + m_szBannerImage.cx;
	rImageArea.bottom	= rImageArea.top + m_szBannerImage.cy;

	_Rect rImage1, rImage2;
	if( pImage1 ){
		if( m_bVerticalSlide ){
			rImage1.left	= rImageArea.left;
			rImage1.top		= rImageArea.top - m_nImageOffset;
			rImage1.right	= rImage1.left + m_szBannerImage.cx;
			rImage1.bottom	= rImage1.top + m_szBannerImage.cy;
			}
		else{
			rImage1.left	= rImageArea.left - m_nImageOffset;
			rImage1.top		= m_rRect.top;
			rImage1.right	= rImage1.left + m_szBannerImage.cx;
			rImage1.bottom	= rImage1.top + m_szBannerImage.cy;
			}
		}

	if( pImage2 ){
		if( m_bVerticalSlide ){
			rImage2.left	= rImage1.left;
			rImage2.top		= rImage1.bottom + m_nDistanceBetweenImages;
			rImage2.right	= rImage2.left + m_szBannerImage.cx;
			rImage2.bottom	= rImage2.top + m_szBannerImage.cy;
			}
		else{
			rImage2.left	= rImage1.right + m_nDistanceBetweenImages;
			rImage2.top		= rImage1.top;
			rImage2.right	= rImage2.left + m_szBannerImage.cx;
			rImage2.bottom	= rImage2.top + m_szBannerImage.cy;
			}
		}

	rImage1 &= rImageArea;
	rImage2 &= rImageArea;
	
	_Rect	rImageDC, rcInvalid;
	// Draw images. {{
	rImageDC.left		= (rImage1.left - pRectClient->left) + pRectDC->left; 
	rImageDC.top		= (rImage1.top - pRectClient->top) + pRectDC->top; 
	rImageDC.right		= rImageDC.left + rImage1.Width(); 
	rImageDC.bottom	= rImageDC.top + rImage1.Height(); 

	rcInvalid	= rImageDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage1->RenderImage(pDC, rcInvalid, rImageDC, false);

	rImageDC.left		= (rImage2.left - pRectClient->left) + pRectDC->left; 
	rImageDC.top		= (rImage2.top - pRectClient->top) + pRectDC->top; 
	rImageDC.right		= rImageDC.left + rImage2.Width(); 
	rImageDC.bottom	= rImageDC.top + rImage2.Height(); 
	rcInvalid	= rImageDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImage2->RenderImage(pDC, rcInvalid, rImageDC, false);
	// }}

	// Draw switch buttons. {{
	if( nImageCt > 1 && !m_imageSwitchButtonNormal.IsNull() && !m_imageSwitchButtonClick.IsNull() ){
		_Rect rcButtons;
		rcButtons.left		= m_rcSwitchButtonsBound.left;
		rcButtons.top		= m_rcSwitchButtonsBound.top;
		rcButtons.right		= rcButtons.left + m_szSwitchButton.cx;
		rcButtons.bottom	= rcButtons.top + m_szSwitchButton.cy;

		for(int i=0; i<nImageCt; i++){
			rImageDC.left		= (rcButtons.left - pRectClient->left) + pRectDC->left; 
			rImageDC.top		= (rcButtons.top - pRectClient->top) + pRectDC->top; 
			rImageDC.right		= rImageDC.left + m_szSwitchButton.cx; 
			rImageDC.bottom	= rImageDC.top + m_szSwitchButton.cy; 

			rcInvalid	= rImageDC & *pRectDC;
			if( !rcInvalid.IsRectEmpty() )
				((i != m_nCurrentImage) ? &m_imageSwitchButtonNormal : &m_imageSwitchButtonClick)->RenderImage(pDC, rcInvalid, rImageDC, false);
			rcButtons.left = (rcButtons.right + m_nDistanceBetweenButtons);
			rcButtons.right	= rcButtons.left + m_szSwitchButton.cx;
			}
		}
	// }}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildBanner::OnMouseHover(_Point pt, UINT nFlags){
	}

BOOL	
ESChildBanner::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
//	SetCursor(LoadCursor(NULL, IDC_HAND));
//	return 1;
    return FALSE;
	}

void	
ESChildBanner::OnMouseLeave(UINT nFlags){
	}

void	
ESChildBanner::OnMouseEnter(UINT nFlags){
	}

void
ESChildBanner::OnLButtonDown(_Point pt, UINT nFlags){
	}

void
ESChildBanner::OnLButtonUp(_Point pt, UINT nFlags){
	}

void	
ESChildBanner::OnLButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildBanner::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildBanner::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildBanner::OnLButtonDblClick(_Point pt, UINT nFlags){
	OnLButtonDown(pt, nFlags);
	}

void	
ESChildBanner::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildBanner::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void
ESChildBanner::OnTimer(UINT nIdEvent){
	KillTimer(nIdEvent);

	if( !m_bVerticalSlide ){
		float fStartX	= 0.0f;
		float fEndX		= (float)(m_szBannerImage.cx + m_nDistanceBetweenImages);
		m_motion.AddMotionObject(this, 500, 20, ESEasyMotionType::MotionQuadraticInOut, ESEasyMotionType::MotionQuadraticInOut, fStartX, 0.0f, fEndX, 0.0f);
		}
	else{
		float fStartY	= 0.0f;
		float fEndY		= (float)(m_szBannerImage.cy + m_nDistanceBetweenImages);
		m_motion.AddMotionObject(this, 500, 20, ESEasyMotionType::MotionQuadraticInOut, ESEasyMotionType::MotionQuadraticInOut, 0.0f, fStartY, 0.0f, fEndY);
		}
	m_motion.StartMotion	(0L, 0L);
	}

void
ESChildBanner::OnMotionStart(DWORD_PTR dwParam){
	m_nImageOffset = 0;
	}

void
ESChildBanner::OnMotionProgress	(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY){
	if( nFrame == nFrameCt ){
		m_nImageOffset = 0;

		m_nCurrentImage ++;
		if( m_nCurrentImage == m_listBannerImages.GetCount() ){
			m_nCurrentImage = 0;
			}
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
ESChildBanner::OnMotionEnd(){
	// Set banner image change timer.
	SetTimer(GetId(), m_nImageChangeMS);
	}

void
ESChildBanner::OnMotionStopped(){
	}

bool
ESChildBanner::BannerImagesModify(ESChildBanner* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->UpdateBannerImages	();
	pThis->UpdateSwitchButtons	();
	pThis->Redraw				(false);
	return true;
	}

bool
ESChildBanner::BannerSwitchModify(ESChildBanner* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->UpdateSwitchButtons	();
	pThis->Redraw				(false);
	return true;
	}