// ESChildAvatar.cpp : implementation file
//

#include "ESChildAvatar.h"
#include "../Path/Path.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildAvatar

ESChildAvatar::ESChildAvatar(){
	m_sClassName				= _T("ESChildAvatar");
	m_pImageAvatarBorder		= NULL;
	m_pImageAvatarBg			= NULL;
	m_pImageAvatar				= NULL;
	}

BEGIN_DECLARE_VAR(ESChildAvatar, ESChildControl)
DECLARE_VAR(_T("ESChildAvatar"),				_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Border Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildAvatar,m_sBorderImageFile), false, false, false)
	DECLARE_VAR(_T("Avatar Bg Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildAvatar,m_sAvatarBgImageFile), false, false, false)
	DECLARE_VAR(_T("Avatar Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildAvatar,m_sAvatarImage), false, false, false)
	DECLARE_VAR(_T("Border Image"),				_T(""), VariableType::VT_Image, offsetof(ESChildAvatar,m_imageAvatarBorder), false, false, false)
	DECLARE_VAR(_T("Avatar Bg Image"),			_T(""), VariableType::VT_Image, offsetof(ESChildAvatar,m_imageAvatarBg), false, false, false)
	DECLARE_VAR(_T("Avatar Image"),				_T(""), VariableType::VT_Image, offsetof(ESChildAvatar,m_imageAvatar), false, false, false)
END_DECLARE_VAR()

ESChildAvatar::~ESChildAvatar(){
	}

bool
ESChildAvatar::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		MercuryGUI* pGUI = MercuryGUI::GetInstance();
		_string sImage;
		if( m_imageAvatarBorder.IsNull() ){
			sImage = m_sBorderImageFile;
			Path::BuildFullPath(sImage);
			m_pImageAvatarBorder = pGUI->LoadImage(sImage);
			}
		else
			m_pImageAvatarBorder = &m_imageAvatarBorder;

		if( m_imageAvatarBg.IsNull() ){
			sImage = m_sAvatarBgImageFile;
			Path::BuildFullPath(sImage);
			m_pImageAvatarBg = pGUI->LoadImage(sImage, -1, -1, true, false);
			}
		else
			m_pImageAvatarBg = &m_imageAvatarBg;

		if( m_imageAvatar.IsNull() ){
			sImage = m_sAvatarImage;
			Path::BuildFullPath(sImage);
			m_pImageAvatar = pGUI->LoadImage(sImage);
			}
		else
			m_pImageAvatar = &m_imageAvatar;

		SetAvatarImage(m_pImageAvatar);
		}
	return bRet;
	}

void
ESChildAvatar::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildAvatar::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildAvatar::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

void
ESChildAvatar::SetLogicalPos(float fPosX, float fPosY){
	ESChildControl::SetLogicalPos(fPosX, fPosY);
	}

void
ESChildAvatar::SetLogicalSize(float fCX, float fCY){
	ESChildControl::SetLogicalSize(fCX, fCY);
	}

bool
ESChildAvatar::Destroy(){
	ESChildControl::Destroy();
	// Free resources.
	m_imageAvatarBorder	.Destroy();
	m_imageAvatarBg		.Destroy();
	m_imageAvatar		.Destroy();
	return true;
	}

void
ESChildAvatar::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect rcClient = GetClientRect();
	_Rect rcControlDC((rcClient.left - pRectClient->left) + pRectDC->left, (rcClient.top - pRectClient->top) + pRectDC->top, rcClient.Size());

	// Draw avatar image.
	if( m_pImageAvatar && !m_pImageAvatar->IsNull() )
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, _FillMode::FillMode_StretchImage, RGB(0, 0, 0), m_pImageAvatar, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);

	// Draw border.
	if( m_pImageAvatarBorder && !m_pImageAvatarBorder->IsNull() )
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, _FillMode::FillMode_StretchImage, RGB(0, 0, 0), m_pImageAvatarBorder, _Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

bool
ESChildAvatar::SetAvatarImage(_Image* pImage, bool bRedraw){
	if( !pImage || pImage->IsNull() )
		return false;

	if( !m_pImageAvatarBg || m_pImageAvatarBg->IsNull() )
		return false;

	_Size	szAvatar(m_pImageAvatarBg->GetWidth(), m_pImageAvatarBg->GetHeight());
	_Image bmAvatar;
	if( !bmAvatar.CreateDIBBitmap(32, RGB(0, 0, 0), szAvatar.cx, szAvatar.cy) )
		return false;

	ImageDef hDest = bmAvatar;
	if( !pImage->ScaleBitmap_Bilinier(szAvatar.cx, szAvatar.cy, _Rect(0, 0, szAvatar.cx, szAvatar.cy), hDest, (pImage->GetBPP() == 32), 255) ){
		bmAvatar.Destroy();
		return false;
		}

	_Image imgBg;
	imgBg.Attach(bmAvatar.Detach());
	_Image* pImgAvatar = _Image::CombineImages(&imgBg, m_pImageAvatarBg, true);
	if( !pImgAvatar ){
		imgBg.Destroy();
		return false;
		}

	m_imageAvatar.Destroy();
	m_imageAvatar.Attach(pImgAvatar->Detach());
	m_pImageAvatar = &m_imageAvatar;
	imgBg.Destroy();
	delete pImgAvatar;

	if( bRedraw )
		Redraw();
	return true;

	}

void
ESChildAvatar::ClearAvatarImage(bool bRedraw){
	if( m_imageAvatar.IsNull() )
		return;
	m_imageAvatar.Destroy();
	if( bRedraw )
		Redraw();
	}
