// MipmapImage.cpp source file.
//
//	ESCL (Elite Soft Class Library)
//	
//	Author: ZqrTalent
//

#include "stdafx.h"
#include "MipmapImage.h"
#include "ImageHelper.h"
#include <math.h>

// Constructor / Destructor
MipmapImage::MipmapImage(){
	}

MipmapImage::~MipmapImage(){
	Destroy();
	}

bool
MipmapImage::LoadImage(CString sImageFile){
	Destroy();
	if( m_imageOriginal.Load(sImageFile) != S_OK )
		return false;

	// Generate mipmap images. {{
	double dWidth	= (double)m_imageOriginal.GetWidth();
	double dHeight	= (double)m_imageOriginal.GetHeight();
	double d1		= dWidth;//pow(dWidth, 0.5);
	double d2		= dHeight;// pow(dHeight, 0.5);

	while( d1 > 2.0 && d2 > 2.0 ){
		CBitmap bmStretch;
		if( ImgHlp::CreateDIBBitmap(32, bmStretch, (COLORREF)0L, (int)d1, (int)d2, NULL) ){
			CDC memDC;
			memDC.CreateCompatibleDC(NULL);
			memDC.SelectObject(&bmStretch);
			if( m_imageOriginal.StretchBlt(memDC, CRect(0, 0, (int)dWidth, (int)dHeight), SRCCOPY) ){
				CImage* pImage = new CImage();
				pImage->Attach((HBITMAP)bmStretch.Detach());

				int nSize = (((int)dWidth) << 16) | (((int)dHeight) & 0xFFFF);
				m_arrImageBySize.Add((void*)nSize, (void*)pImage);
				}
			else
				DeleteObject(bmStretch.Detach());
			memDC.DeleteDC();
			}

		d1		= d1 / 2.0;//pow(d1, 0.5);
		d2		= d2 / 2.0;// pow(d2, 0.5);
		}
	// }}
	return true;
	}

bool
MipmapImage::SetImage(CImage* pImage){
	if( !pImage || pImage->IsNull()) return false;

	Destroy();
	m_imageOriginal.Attach(pImage->Detach());

	// Generate mipmap images. {{
	double dWidth	= (double)m_imageOriginal.GetWidth();
	double dHeight	= (double)m_imageOriginal.GetHeight();
	double d1		= dWidth;//pow(dWidth, 0.5);
	double d2		= dHeight;// pow(dHeight, 0.5);

	while( d1 > 2.0 && d2 > 2.0 ){
		CBitmap bmStretch;
		if( ImgHlp::CreateDIBBitmap(32, bmStretch, (COLORREF)0L, (int)d1, (int)d2, NULL) ){
			CDC memDC;
			memDC.CreateCompatibleDC(NULL);
			memDC.SelectObject		(&bmStretch);
			memDC.SetStretchBltMode	(HALFTONE);
			if( m_imageOriginal.StretchBlt(memDC, 0, 0, (int)d1, (int)d2, 0, 0, m_imageOriginal.GetWidth(), m_imageOriginal.GetHeight(), SRCCOPY) ){
				CImage* pImage = new CImage();
				pImage->Attach((HBITMAP)bmStretch.Detach());

				int		nSize	= (((int)d1) << 16) | (((int)d2) & 0xFFFF);
				m_arrImageBySize.Add((void*)nSize, (void*)pImage);
				}
			else
				DeleteObject(bmStretch.Detach());
			memDC.DeleteDC();
			}

		d1		= d1 / 2.0;//pow(d1, 0.5);
		d2		= d2 / 2.0;// pow(d2, 0.5);
		}
	// }}
	return true;
	}

void
MipmapImage::Destroy(){
	m_imageOriginal.Destroy();

	int nLoop	= 0;
	int nCt		= m_arrImageBySize.GetCount();
	while( nLoop < nCt ){
		CImage* pImage = (CImage*)m_arrImageBySize.GetData(nLoop);
		if( pImage ){
			delete pImage;
			}
		nLoop ++;
		}
	m_arrImageBySize.DeleteAll();
	}

BOOL	
MipmapImage::Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight){
	CImage *pImage = GetNearestImage(nDestWidth, nDestHeight);
	if( !pImage ) return FALSE;

	CDC memDC;
	memDC.CreateCompatibleDC(CDC::FromHandle(hDestDC));
	::SetStretchBltMode(hDestDC, HALFTONE); 
	::SelectObject(memDC, (HGDIOBJ)*pImage);

	BOOL bRet = ::StretchBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, memDC, xSrc, ySrc, nSrcWidth, nSrcHeight, SRCCOPY);
	memDC.DeleteDC();

	//BOOL bRet = pImage->Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, nSrcWidth, nSrcHeight);
	return bRet;
	}

CImage*
MipmapImage::GetNearestImage(int cx, int cy){
	return &m_imageOriginal;
	if( !m_arrImageBySize.GetCount() ) {
		if( m_imageOriginal.IsNull() )
			return NULL;
		return &m_imageOriginal;
		}

	long	lSize	= (((int)cx) << 16) | (((int)cy) & 0xFFFF);
	bool	bExists = false;
	int		nIndex	= m_arrImageBySize.FindValuePlace((void*)lSize, bExists);
	if( bExists )
		return (CImage*)m_arrImageBySize.GetData(nIndex);

	int		nPrev	= nIndex ? (nIndex - 1) : 0;
	int		nNext	= (nIndex >= m_arrImageBySize.GetCount()) ? m_arrImageBySize.GetCount() - 1 : nIndex;


	CImage* pImageNext	= nNext > -1 ? (CImage*)m_arrImageBySize.GetData(nNext) : NULL;
	CImage* pImagePrev	= nPrev > -1 ? (CImage*)m_arrImageBySize.GetData(nPrev) : NULL;

	if( pImageNext && pImagePrev ){
		CSize sz1	(abs(pImageNext->GetWidth() - cx), abs(pImageNext->GetHeight() - cy));
		CSize sz2	(abs(pImagePrev->GetWidth() - cy), abs(pImagePrev->GetHeight() - cy));
		if( sz1.cx  > sz2.cx && sz1.cy  > sz2.cy ){
			return pImagePrev;
			}
		else
			return pImageNext;
		}

	if( pImagePrev )
		return pImagePrev;
	if( pImageNext )
		return pImageNext;
	return NULL;
	}