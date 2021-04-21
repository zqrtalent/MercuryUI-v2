// ESChildControlImage.cpp : implementation file
//

#include "ESChildControlImage.h"
#include "../Path/Path.h"
//#include <io.h>

/////////////////////////////////////////////////////////////////////////////
// ESChildControlImage

ESChildControlImage::ESChildControlImage(){
	m_sClassName		= _T("ESChildControlImage");
	m_bUseHandCursor	= false;
	m_bMultiImageMode	= false;
	m_imageFillMode	.Add(_T("CentredImage"),		_FillMode::FillMode_CentredImage);
	m_imageFillMode	.Add(_T("StretchImage"),		_FillMode::FillMode_StretchImage);
	m_imageFillMode	.Add(_T("RepeatedImage"),		_FillMode::FillMode_RepeatedImage);
	m_imageFillMode	.Add(_T("OmitBorderedImage"),	_FillMode::FillMode_OmitBorderedImage);
	m_imageFillMode	.SetCurrentIndex(0);
	m_bDestroyMultiImages	= true;
	}

BEGIN_DECLARE_VAR(ESChildControlImage, ESChildControl)
DECLARE_VAR(_T("ESChildControlImage"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Image"),				_T(""), VariableType::VT_Image, offsetof(ESChildControlImage,m_image), false, false, false)
	DECLARE_VAR(_T("ImageFile"),			_T(""), VariableType::VT_String, offsetof(ESChildControlImage,m_sImageFile), false, false, false)
	DECLARE_VAR(_T("MultiImageMode"),		_T(""), VariableType::VT_Bool, offsetof(ESChildControlImage,m_bMultiImageMode), false, false, false)
	DECLARE_VAR(_T("MultiImgFolder"),		_T(""), VariableType::VT_String, offsetof(ESChildControlImage,m_sMultipleImagesFolder), false, false, false)
	DECLARE_VAR(_T("MultiImgNameFormat"),	_T(""), VariableType::VT_String, offsetof(ESChildControlImage,m_sMultipleImageNameFormat), false, false, false)
	DECLARE_VAR(_T("Fill Mode"),			_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlImage,m_imageFillMode), false, false, false)
	DECLARE_VAR(_T("OmitBorder"),			_T(""), VariableType::VT_Rect, offsetof(ESChildControlImage,m_rcOmitBorder), false, false, false)
	DECLARE_VAR(_T("HandCursor"),			_T(""), VariableType::VT_Bool, offsetof(ESChildControlImage,m_bUseHandCursor), false, false, false)
END_DECLARE_VAR()

ESChildControlImage::~ESChildControlImage(){
	if( !m_image.IsNull() )
		m_image.Destroy();
	// Remove multi images.
	m_listMultiImages.RemoveAll(m_bDestroyMultiImages);
	}

bool
ESChildControlImage::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		if( m_pOwner->IsDesignerMode() ){
			// Register variable modify callback methods. {{
			AddVariableModifyCallBack(&m_bMultiImageMode,			(VariableValueModifyCallback)&ESChildControlImage::AttributeModify);
			AddVariableModifyCallBack(&m_sMultipleImagesFolder,		(VariableValueModifyCallback)&ESChildControlImage::AttributeModify);
			AddVariableModifyCallBack(&m_sMultipleImageNameFormat,	(VariableValueModifyCallback)&ESChildControlImage::AttributeModify);
			// }}
			}
		ScanMultiImages();
		}
	return bRet;
	}

bool
ESChildControlImage::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlImage::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

void
ESChildControlImage::SetLogicalPos(float fPosX, float fPosY){
	ESChildControl::SetLogicalPos(fPosX, fPosY);
	}

void
ESChildControlImage::SetLogicalSize(float fCX, float fCY){
	ESChildControl::SetLogicalSize(fCX, fCY);
	}

void
ESChildControlImage::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildControlImage::SetImage(_Image* pImage, bool bRedraw){
	if( !pImage || pImage->IsNull() )
		return false;

	if( !m_image.IsNull() )
		m_image.Destroy();

	pImage->CopyImage(&m_image);
	
	if( bRedraw )
		Redraw();
	return true;
	}

void
ESChildControlImage::ScanMultiImages(){
	if( !m_bMultiImageMode || !m_sMultipleImagesFolder.length() || !m_sMultipleImageNameFormat.length() )
		return;

	m_listMultiImages.RemoveAll(m_bDestroyMultiImages);
	_string sImageFile, sImageFileName;
	int nLoop = 0;

	_string sFolder = m_sMultipleImagesFolder;
	if( sFolder[sFolder.length()-1] == '\\' )
		sFolder = sFolder.substr(0, sFolder.length()-1);
	
	// Do not cache images in designer mode.
	if( m_pOwner->IsDesignerMode() ){
		Path::BuildFullPath(sFolder);

		while(TRUE){
			stringEx::Format(sImageFileName, m_sMultipleImageNameFormat.c_str(), nLoop);
			stringEx::Format(sImageFile, _T("%s\\%s"), sFolder.c_str(), sImageFileName.c_str());
			if( Path::FileExists(sImageFile) ){
				_Image* pImageNew = new _Image();
				if( pImageNew->LoadImage32bpp(sImageFile) ){
					m_listMultiImages.Add(pImageNew);
					}
				else
					delete pImageNew;
				}
			else
				break;
			nLoop ++;
			}
		}
	else{
		m_bDestroyMultiImages = false;
		MercuryGUI* pGUI = MercuryGUI::GetInstance();
		while(TRUE){
			stringEx::Format(sImageFileName, m_sMultipleImageNameFormat.c_str(), nLoop);
			stringEx::Format(sImageFile, _T("%s\\%s"), sFolder.c_str(), sImageFileName.c_str());
			
			_Image* pImageNew = pGUI->LoadImage(sImageFile, -1, -1, true, false);
			if( pImageNew )
				m_listMultiImages.Add(pImageNew);
			else
				break; // Stop loading images.
			nLoop ++;
			}
		}
	}

bool
ESChildControlImage::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlImage::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Image*		pImage	= &m_image;
	MercuryGUI* pGUI	= MercuryGUI::GetInstance();
	if( pImage->IsNull() )
		pImage = pGUI->LoadImage(m_sImageFile);
	
	// Choose from multi image. {{
	if( m_bMultiImageMode ){
		int nLoop = 0, nCt = m_listMultiImages.GetCount();
		_Size szImageNeed(m_rRect.Width(), m_rRect.Height());
		pImage = (nCt > 0 ? m_listMultiImages.GetAt(0) : NULL);
		while( nLoop < nCt ){
			_Image* p = m_listMultiImages.GetAt(nLoop);
			if( p && p->GetWidth() <= szImageNeed.cx && p->GetHeight() <= szImageNeed.cy )
				pImage = p;
			else
				break;
			nLoop ++;
			}

		if( nLoop >= nCt )
			pImage = (nCt > 0 ? m_listMultiImages.GetAt(nCt - 1) : NULL);
		}
	// }}

	_Rect rcClient	= GetClientRect();
	if( pImage && !pImage->IsNull() ){
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, m_imageFillMode.GetCurrentValue(), RGB(0, 0, 0), pImage, m_rcOmitBorder, false, ScaleMode::ScaleMode_Bilinier);
		}/*
	else{
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode_Color, RGB(0, 122, 0), pImage, m_rcOmitBorder, false, ScaleMode::ScaleMode_Bilinier);
		}*/
	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildControlImage::OnMouseHover(_Point pt, UINT nFlags){
	}

BOOL	
ESChildControlImage::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	if( m_bUseHandCursor ){
//		SetCursor(LoadCursor(NULL, IDC_HAND));
		return 1;
		}
	return 0;
	}

void	
ESChildControlImage::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlImage::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlImage::OnLButtonDown(_Point pt, UINT nFlags){
	}

void
ESChildControlImage::OnLButtonUp(_Point pt, UINT nFlags){
	}

void	
ESChildControlImage::OnLButtonClick(_Point pt, UINT nFlags){
	// Send click event.
	SendEvent(this, ChildControlEventCode::EventCode_ImageClick);
	}

void	
ESChildControlImage::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlImage::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlImage::OnLButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlImage::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlImage::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

bool
ESChildControlImage::AttributeModify(ESChildControlImage* pThis, VariableInfo* pInfo, void* lpValueNew){
	// Destroy images.
	pThis->m_listMultiImages.RemoveAll(true);
	pThis->ScanMultiImages();
	return true;
	}