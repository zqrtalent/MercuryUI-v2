// ImageHelper.cpp source file.
//
//	ESCL (Elite Soft Class Library)
//	
//	Author: ZqrTalent
//

#include "stdafx.h"
#include "ImageHelper.h"
#include "..\Path\Path.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OPTIMIZE_RENDER

#ifdef USE_PNG_LIBRARY
#pragma comment(lib, "PNGLibrary.lib")
extern "C" __declspec(dllimport) HBITMAP LoadPNG(BYTE* lpPNGFileData, UINT nDataSize);
#endif

// Constructor / Destructor
ImageHelper::ImageHelper(){
	}

ImageHelper::~ImageHelper(){
	}

BOOL
ImageHelper::LoadImage32bpp(const CString sImageFile, CBitmap& bmImage){
	CImage img;
	if( img.Load(sImageFile) != 0 )
		return FALSE;
	if( bmImage.m_hObject )
		bmImage.DeleteObject();

	if( img.GetBPP() != 32 ){
		bmImage.Attach(img.Detach());
		ConvertToBitmap32bpp(bmImage);
		}
	else
		bmImage.Attach(img.Detach());
	return TRUE;
	}

CImage*
ImageHelper::LoadImage(CString sImageFile, int nWidth /*= -1*/, int nHeight /*= -1*/, bool bConvert32bpp /*= true*/){
	CImage* pImage = NULL;
#ifdef LOAD_FILES_FROM_DLL
	CString sTemp = sImageFile;
	sTemp.Replace('\\', '@');
	sTemp.MakeUpper();

	CString sResName;
	sResName.Format(_T("~%s~"), sTemp);
	sResName.Replace('~', '\x22');
	HMODULE hResModule = GetModuleHandle(RESOURCE_MODULE);
	pImage = LoadPngFromResource(hResModule, sResName.GetBuffer());
#endif

	if( !pImage ){
		pImage = new CImage();
		Path::BuildFullPath(sImageFile);

		LPTSTR lpszImageFile	= sImageFile.GetBuffer();
		int		nLen			= sImageFile.GetLength();
		LPCTSTR lpszExt			= &lpszImageFile[nLen - 4];

#ifdef USE_PNG_LIBRARY
		if( nLen > 4 && !strcmpi(lpszExt, _T(".png")) ){
			BYTE	*lpData		= NULL;
			int		nDataSize	= 0;
			if( !Path::ReadFileContent(sImageFile, lpData, nDataSize) ){
				delete pImage;
				return NULL;
				}

			HBITMAP hPngImage = LoadPNG(lpData, nDataSize);
			if( !hPngImage ){
				delete [] lpData;
				delete pImage;
				return NULL;
				}
			pImage->Attach(hPngImage);
			delete [] lpData;
			}
		else{
			if( pImage->Load(sImageFile.GetBuffer()) != 0 ){
				delete pImage;
				return NULL;
				}
			}
#else
		if( pImage->Load(sImageFile.GetBuffer()) != 0 ){
			delete pImage;
			return NULL;
			}
#endif

		}

	if( nWidth > 0 && nHeight > 0 ){
		CBitmap bmStretch;
		if( ImgHlp::CreateDIBBitmap(pImage->GetBPP(), bmStretch, (COLORREF)0L, nWidth, nHeight, NULL) ){
			CDC memDC;
			memDC.CreateCompatibleDC(NULL);
			memDC.SelectObject(&bmStretch);
			if( pImage->StretchBlt(memDC, CRect(0, 0, nWidth, nHeight), SRCCOPY) ){
				DeleteObject( pImage->Detach() );
				pImage->Attach((HBITMAP)bmStretch.Detach());
				}
			else
				DeleteObject(bmStretch.Detach());
			memDC.DeleteDC();
			}
		}

	// Convert into 32bpp. ################
	if( pImage->GetBPP() != 32 && bConvert32bpp ){
		CBitmap bmTemp;
		bmTemp.Attach(pImage->Detach());
		ImageHelper::ConvertToBitmap32bpp(bmTemp);
		pImage->Attach((HBITMAP)bmTemp.Detach());

		// Make image opaque
		BITMAP bmImage;
		::GetObject(*pImage, sizeof(BITMAP), &bmImage);
		RGBQUAD* p = (RGBQUAD*)bmImage.bmBits;

		int nLoop = 0, nCt = bmImage.bmWidth*bmImage.bmHeight;
		while( nLoop < nCt ){
			p->rgbReserved = 255;
			p ++;
			nLoop ++;
			}
		}
	// #######################################
	return pImage;
	}

BOOL
ImageHelper::ConvertToBitmap32bpp(CBitmap& bmImage){
	BOOL	bRet		= FALSE;
	HBITMAP hBmImage	= (HBITMAP)bmImage.Detach();
	HDC		memDC, srcDC;

	memDC = ::CreateCompatibleDC(NULL);
	srcDC = ::CreateCompatibleDC(memDC);
	SelectObject(srcDC, hBmImage);
	
	BITMAP bmp;
	::GetObject (hBmImage, sizeof(BITMAP), &bmp);
	if( bmp.bmBitsPixel == 32 && bmp.bmBits ){
		DeleteDC(memDC);
		DeleteDC(srcDC);
		return TRUE;
		}

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32,0,sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = 32;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = bmp.bmHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = bmp.bmWidth;
	
	void*   lpMap = NULL;
	HBITMAP hDib  = CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	if( !hDib ){
		DeleteDC(memDC);
		DeleteDC(srcDC);
		bmImage.Attach(hBmImage);
		return FALSE;
		}

/*
#ifdef DEBUG
	AutoSortedArray* pArr = GetArrayCreatedDibImages();
	pArr->Add((void*)hDib, (void*)5);
#endif
*/

	SelectObject(memDC, hDib);
	BitBlt		(memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

	::DeleteObject	(hBmImage);
	DeleteDC		(memDC);
	DeleteDC		(srcDC);

	bmImage.Attach(hDib);
	return 1;
	}

BOOL
ImageHelper::ConvertToBitmap24bpp(CBitmap& bmImage, CBitmap* pCopyInto /*= NULL*/){
	BITMAP bmp;
	::GetObject (bmImage, sizeof(BITMAP), &bmp);
	if( bmp.bmBitsPixel == 24 && bmp.bmBits )
		return FALSE;

	HBITMAP hBmImage	= (HBITMAP)bmImage.Detach();
	HDC		memDC;
	memDC = ::CreateCompatibleDC(NULL);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32,0,sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = 24;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = bmp.bmHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = bmp.bmWidth;
	
	void*   lpMap = NULL;
	HBITMAP hDib  = CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	if( !hDib ){
		DeleteDC(memDC);
		bmImage.Attach(hBmImage);
		return FALSE;
		}
/*
#ifdef DEBUG
	AutoSortedArray* pArr = GetArrayCreatedDibImages();
	pArr->Add((void*)hDib, (void*)4);
#endif
*/
	BITMAP bmDest;
	GetObject(hDib, sizeof(BITMAP), &bmDest);

	BYTE* pDest = (BYTE*)lpMap;
	BYTE* pSrc = (BYTE*)bmp.bmBits;
	for(int y=0; y<bmp.bmHeight; y++){
		pSrc	= &((BYTE*)bmp.bmBits)[y*bmp.bmWidthBytes];
		pDest	= &((BYTE*)lpMap)[y*bmDest.bmWidthBytes];

		for(int x=0; x<bmp.bmWidth; x++){
			pDest[0] = pSrc[0]; // Blue
			pDest[1] = pSrc[1]; // Green
			pDest[2] = pSrc[2]; // Blue
			pSrc	+= 4;
			pDest	+= 3;
			}
		}

	if( !pCopyInto ){
		::DeleteObject	(hBmImage);
		bmImage.Attach	(hDib);
		}
	else{
		bmImage.Attach(hBmImage);
		if( pCopyInto->m_hObject )
			pCopyInto->DeleteObject();
		pCopyInto->Attach(hDib);
		}
	DeleteDC		(memDC);
	return 1;
	}

BOOL
ImageHelper::SaveBitmap(const CString sImageFile, HBITMAP hImage, int nBPP){
	BOOL	bRet		= FALSE;
	HDC		memDC = NULL, srcDC = NULL;
	HBITMAP	hBmImage	= hImage;
	
	BITMAP bmp;
	::GetObject(hBmImage, sizeof(BITMAP),&bmp);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = nBPP;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = bmp.bmHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = bmp.bmWidth;
	
	void*   lpMap	= NULL;
	HBITMAP hDib	= NULL;

	if( !bmp.bmBits || bmp.bmBitsPixel != 32 ){
		memDC	= ::CreateCompatibleDC(NULL);
		srcDC	= ::CreateCompatibleDC(memDC);
		SelectObject(srcDC, hBmImage);

		hDib	= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
		SelectObject(memDC, hDib);
		BitBlt(memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);
		}
	else
		lpMap = bmp.bmBits;
	

	HANDLE hFile = CreateFile(sImageFile, GENERIC_READ|GENERIC_WRITE,
							  FILE_SHARE_READ|FILE_SHARE_WRITE,
							  NULL,CREATE_NEW, 0L, NULL);
	if( hFile != INVALID_HANDLE_VALUE && lpMap ){
		BITMAPFILEHEADER bfh;
		ZeroMemory(&bfh, sizeof(BITMAPFILEHEADER));
		bfh.bfType		= 0x4d42; // 'BM'
		bfh.bfSize		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmp.bmHeight*bmp.bmWidth*(nBPP/8);
		bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		DWORD dwWritten = 0L;
		//write bitmap file header structure.
		WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		//write info header structure.
		WriteFile(hFile,&bmpInfo32, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
		//An array of RGBQUAD. The elements of the array that make up the color table. 
		WriteFile(hFile, lpMap, bfh.bfSize - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), &dwWritten, NULL);
		bRet = TRUE;
		}

	CloseHandle		(hFile);
	if( memDC )
		DeleteDC(memDC);
	if( srcDC )
		DeleteDC(srcDC);
	if( hDib )
		::DeleteObject(hDib);
	return bRet;
	}

BOOL
ImageHelper::GetBitmapFromDC(int nBPP, CBitmap& bmImage, CDC* pSrc, UINT xSrc, UINT ySrc, UINT nWidth, UINT nHeight, CDC** pDCBitmap /*= NULL*/){
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(NULL);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = nBPP;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = nHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = nWidth;
	
	void*   lpMap = NULL;
	HBITMAP hDib  = CreateDIBSection(memDC->m_hDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	if( !hDib ){
		memDC->DeleteDC();
		delete memDC;
		return 0;
		}

	memDC->SelectObject	(hDib);
	memDC->BitBlt		(0, 0, nWidth, nHeight, pSrc, xSrc, ySrc, SRCCOPY);
	if( pDCBitmap )
		*pDCBitmap = memDC;
	else{
		memDC->DeleteDC();
		delete memDC;
		}
/*
#ifdef DEBUG
	AutoSortedArray* pArr = GetArrayCreatedDibImages();
	pArr->Add((void*)hDib, (void*)3);
#endif
*/
	bmImage.Attach(hDib);
	return 1;
	}

BOOL		
ImageHelper::CreateDIBBitmap(int nBPP, CBitmap& bmImage, UINT* pColorTable, UINT nWidth, UINT nHeight, BITMAP* pBmp /*= NULL*/){
	BITMAPINFOHEADER	bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount		= nBPP;
	bmpInfo32.biCompression		= BI_RGB;
	bmpInfo32.biPlanes			= 1;
	bmpInfo32.biHeight			= nHeight;
	bmpInfo32.biSize			= sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth			= nWidth;

	HDC					memDC	= ::CreateCompatibleDC(NULL);
	UINT*				lpMap;
	HBITMAP				hDib	= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, 
													(void**)&lpMap, NULL, 0);
	ASSERT( lpMap );

	UINT index = 0;
	while( index < nWidth*nHeight ){
		lpMap[index] = pColorTable[index];
		index ++;
		}

	if( pBmp )
		::GetObject(hDib, sizeof(BITMAP), pBmp);
/*
#ifdef DEBUG
	AutoSortedArray* pArr = GetArrayCreatedDibImages();
	pArr->Add((void*)hDib, (void*)2);
#endif
*/
	DeleteDC(memDC);
	bmImage.Attach(hDib);
	return 1;
	}

BOOL
ImageHelper::CreateDIBBitmap(int nBPP, CBitmap& bmImage, COLORREF crFill, UINT nWidth, UINT nHeight, BITMAP* pBmp /*= NULL*/, CDC* pDCCompatible /*= NULL*/){
	BITMAPINFOHEADER	bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount		= nBPP;
	bmpInfo32.biCompression		= BI_RGB;
	bmpInfo32.biPlanes			= 1;
	bmpInfo32.biHeight			= nHeight;
	bmpInfo32.biSize			= sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth			= nWidth;

	UINT*	lpMap	= NULL;
	HBITMAP	hDib	= NULL;
	
	if( !pDCCompatible ){
		CDC	memDC;
		memDC.CreateCompatibleDC(NULL);
		hDib	= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, (void**)&lpMap, NULL, 0);
		ASSERT( lpMap );
		memDC.DeleteDC();
		}
	else{
		hDib	= CreateDIBSection(pDCCompatible->m_hDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, (void**)&lpMap, NULL, 0);
		ASSERT( lpMap );
		}

	if( crFill != 0 ){
		int			nLoop		= 0, nIndex = 0, nCount		= nWidth*nHeight;
		nBPP					/= 8;

		BYTE btRed				= crFill&0xFF;
		BYTE btGreen			= (crFill&0xFF00)>>8;
		BYTE btBlue				= (crFill&0xFF0000)>>16;

		BITMAP bmImage;
		GetObject(hDib, sizeof(BITMAP), &bmImage);

		for(int y=0; y<nHeight; y++){
			BYTE* lpBits = &((BYTE*)lpMap)[nIndex];
			for(int x=0; x<nWidth; x++){
				lpBits[0]	= btBlue; // Blue
				lpBits[1]	= btGreen; // Green
				lpBits[2]	= btRed; // Red
				lpBits		= &lpBits[nBPP];
				}
			nIndex += bmImage.bmWidthBytes;
			}
		}

	if( pBmp )
		::GetObject(hDib, sizeof(BITMAP), pBmp);
/*
#ifdef DEBUG
	AutoSortedArray* pArr = GetArrayCreatedDibImages();
	pArr->Add((void*)hDib, (void*)1);
#endif
*/
	bmImage.Attach(hDib);
	return 1;
	}

bool
ImageHelper::GetImageFromImage(CImage* pSrcImage, CRect rcDest, CImage& image){
	if( !pSrcImage || rcDest.IsRectEmpty() )
		return false;

	if( !image.IsNull() ) return false;

	BITMAP bmImage;
	::GetObject(*pSrcImage, sizeof(BITMAP), &bmImage);

	CBitmap bmpImage;
	if( !CreateDIBBitmap((int)bmImage.bmBitsPixel, bmpImage, RGB(0, 0, 0), rcDest.Width(), rcDest.Height()) )
		return false;

	BITMAP bmImageDest;
	::GetObject(bmpImage, sizeof(BITMAP), &bmImageDest);
	BitmapOnBitmap(&bmImageDest, 0, 0, &bmImage, rcDest.left, rcDest.top, bmImage.bmWidth, bmImage.bmHeight, rcDest.Width(), rcDest.Height()); 
	
	image.Attach((HBITMAP)bmpImage.Detach());
	return true;
	}

BOOL
ImageHelper::BitmapOnBitmap(HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, int nDrawWidth, int nDrawHeight, BOOL bAlpha, BYTE btAlpha /*= 255*/, BOOL bIgnoreFullAlphaPixel, bool bDontAffectAlphaChannel){
	// check parameters validation. ======================================
	BITMAP bmpSrc, bmpDest;
	::GetObject(hBmpSrc, sizeof(BITMAP), &bmpSrc);
	::GetObject(hBmpDest, sizeof(BITMAP), &bmpDest);
	if( bmpSrc.bmBits == NULL || bmpDest.bmBits == NULL )
		return FALSE;
	// ===================================================================
	return BitmapOnBitmap(&bmpDest, xDest, yDest, &bmpSrc, xSrc, ySrc, nWidthSrc, nHeightSrc, nDrawWidth, nDrawHeight, bAlpha, btAlpha, bIgnoreFullAlphaPixel, bDontAffectAlphaChannel);
	}

BOOL
ImageHelper::BitmapOnBitmap(BITMAP* pbmpDest, UINT xDest, UINT yDest, BITMAP* pbmpSrc, UINT xSrc, UINT ySrc, UINT nWidthSrc, UINT nHeightSrc, UINT nDrawWidth, UINT nDrawHeight, BOOL bAlpha /*= FALSE*/, BYTE btAlpha /*= 255*/, BOOL bIgnoreFullAlphaPixel, bool bDontAffectAlphaChannel){
	ASSERT( xDest < pbmpDest->bmWidth  && nDrawWidth  <= pbmpDest->bmWidth);
	ASSERT( yDest < pbmpDest->bmHeight && nDrawHeight <= pbmpDest->bmHeight);
	ASSERT( xSrc  < pbmpSrc->bmWidth   && nWidthSrc   <= pbmpSrc->bmWidth );
	ASSERT( ySrc  < pbmpSrc->bmHeight  && nHeightSrc  <= pbmpSrc->bmHeight);

	if( !nWidthSrc ) nWidthSrc  = pbmpSrc->bmWidth;
	if( !nHeightSrc ) nHeightSrc = pbmpSrc->bmHeight;

	if( pbmpSrc->bmBitsPixel < 24 || pbmpDest->bmBitsPixel < 24 )
		return FALSE;

	int   nAlpha	= (int)btAlpha;
	CRect rcDest	(xDest, yDest, xDest + nDrawWidth, yDest + nDrawHeight);
	CRect rcSrc		(xSrc, ySrc, min(xSrc+nWidthSrc, pbmpSrc->bmWidth), min(ySrc+nHeightSrc, pbmpSrc->bmHeight));

	if( rcSrc.Width() > rcDest.Width() )
		rcSrc.right = rcSrc.left + rcDest.Width();
	if( rcSrc.Height() > rcDest.Height() )
		rcSrc.bottom = rcSrc.top + rcDest.Height();

	CRect rcDrawDest(rcDest.left, rcDest.top, rcDest.left + rcSrc.Width(), rcDest.top + rcSrc.Height());
	BYTE* pbtColorByteByAlphaByte = GetColorByteByAlphaByteArray();

	if( pbmpSrc->bmBitsPixel != 0x20 )
		bAlpha = FALSE;

	int nBPPSrc	= pbmpSrc->bmBitsPixel / 8, nBPPDest = pbmpDest->bmBitsPixel / 8;
	if( bAlpha ){
		if( btAlpha != 255 ){ // Use constant alpha.
			while( rcDrawDest.Width() > 0 ){
				while( rcDrawDest.Height() > 0 ){
					// Perform draw operation here. ##########
					for( UINT yLoop = 0; yLoop < rcDrawDest.Height(); yLoop ++ ){
						BYTE*	pPixelSrc	= (BYTE*)(((UINT)pbmpSrc->bmBits) + pbmpSrc->bmWidthBytes*(pbmpSrc->bmHeight - yLoop - rcSrc.top - 1) + nBPPSrc*rcSrc.left);
						BYTE*	pPixelDest	= (BYTE*)(((UINT)pbmpDest->bmBits) + pbmpDest->bmWidthBytes*(pbmpDest->bmHeight - yLoop - rcDrawDest.top - 1) + nBPPDest*rcDrawDest.left);

						if( nBPPSrc == 4 ){
							for( UINT xLoop = 0; xLoop < rcDrawDest.Width(); xLoop ++ ){
								if( pPixelSrc[3] < 10 ){/*
									// Blue color byte.
									pPixelDest[0] = pbtColorByteByAlphaByte[pPixelSrc[0]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[0]*256 + (255 - nAlpha)];
									// Green color byte.
									pPixelDest[1] = pbtColorByteByAlphaByte[pPixelSrc[1]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[1]*256 + (255 - nAlpha)];
									// Red color byte.
									pPixelDest[2] = pbtColorByteByAlphaByte[pPixelSrc[2]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[2]*256 + (255 - nAlpha)];*/
									}
								else{
									// Blue color byte.
									pPixelDest[0] = pbtColorByteByAlphaByte[pPixelSrc[0]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[0]*256 + (255 - nAlpha)];
									// Green color byte.
									pPixelDest[1] = pbtColorByteByAlphaByte[pPixelSrc[1]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[1]*256 + (255 - nAlpha)];
									// Red color byte.
									pPixelDest[2] = pbtColorByteByAlphaByte[pPixelSrc[2]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[2]*256 + (255 - nAlpha)];
									}

								pPixelDest	= &pPixelDest[nBPPDest];
								pPixelSrc	= &pPixelSrc[nBPPSrc];
								}
							}
						else{
							for( UINT xLoop = 0; xLoop < rcDrawDest.Width(); xLoop ++ ){
								// Blue color byte.
								pPixelDest[0] = pbtColorByteByAlphaByte[pPixelSrc[0]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[0]*256 + (255 - nAlpha)];
								// Green color byte.
								pPixelDest[1] = pbtColorByteByAlphaByte[pPixelSrc[1]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[1]*256 + (255 - nAlpha)];
								// Red color byte.
								pPixelDest[2] = pbtColorByteByAlphaByte[pPixelSrc[2]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[2]*256 + (255 - nAlpha)];

								pPixelDest	= &pPixelDest[nBPPDest];
								pPixelSrc	= &pPixelSrc[nBPPSrc];
								}
							}
						}
					// #######################################
					int nIncreaseWidth	 =  min(rcSrc.Width(), (rcDest.right - rcDrawDest.right));
					if( nIncreaseWidth <= 0 ) break;
					rcDrawDest.left		 = rcDrawDest.right;
					rcDrawDest.right	+= nIncreaseWidth;
					}

				int nIncreaseHeight	 =  min(rcSrc.Height(), (rcDest.bottom - rcDrawDest.bottom));
				if( nIncreaseHeight <= 0 ) break;
				rcDrawDest.top		 = rcDrawDest.bottom;
				rcDrawDest.bottom	+= nIncreaseHeight;
				rcDrawDest.left		 = rcDest.left;
				rcDrawDest.right	 = rcDest.left + rcSrc.Width();	
				}
			}
		else{
			bool bUseDestAlpha = (nBPPDest == 4);
			if( bDontAffectAlphaChannel )
				bUseDestAlpha = false;
			while( rcDrawDest.Width() > 0 ){
				while( rcDrawDest.Height() > 0 ){
					// Perform draw operation here. ##########
					for( UINT yLoop = 0; yLoop < rcDrawDest.Height(); yLoop ++ ){
						BYTE*	pPixelSrc	= (BYTE*)(((UINT)pbmpSrc->bmBits) + pbmpSrc->bmWidthBytes*(pbmpSrc->bmHeight - yLoop - rcSrc.top - 1) + nBPPSrc*rcSrc.left);
						BYTE*	pPixelDest	= (BYTE*)(((UINT)pbmpDest->bmBits) + pbmpDest->bmWidthBytes*(pbmpDest->bmHeight - yLoop - rcDrawDest.top - 1) + nBPPDest*rcDrawDest.left);

						for( UINT xLoop = 0; xLoop < rcDrawDest.Width(); xLoop ++ ){
							nAlpha		= pPixelSrc[3];
							if( bIgnoreFullAlphaPixel && !nAlpha ){
								pPixelDest	= &pPixelDest[nBPPDest];
								pPixelSrc	= &pPixelSrc[nBPPSrc];
								continue;
								}

							if( nAlpha < 255 ){
								// Blue color byte.
								pPixelDest[0] = pbtColorByteByAlphaByte[pPixelSrc[0]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[0]*256 + (255 - nAlpha)];
								// Green color byte.
								pPixelDest[1] = pbtColorByteByAlphaByte[pPixelSrc[1]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[1]*256 + (255 - nAlpha)];
								// Red color byte.
								pPixelDest[2] = pbtColorByteByAlphaByte[pPixelSrc[2]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[2]*256 + (255 - nAlpha)];
								}
							else{
								pPixelDest[0] = pPixelSrc[0]; // rgbBlue
								pPixelDest[1] = pPixelSrc[1]; // rgbGreen
								pPixelDest[2] = pPixelSrc[2]; // rgbRed
								}

							if( bUseDestAlpha )
								pPixelDest[3] = nAlpha;
							pPixelDest	= &pPixelDest[nBPPDest];
							pPixelSrc	= &pPixelSrc[nBPPSrc];
							}
						}
					// #######################################
					int nIncreaseWidth	 =  min(rcSrc.Width(), (rcDest.right - rcDrawDest.right));
					if( nIncreaseWidth <= 0 ) break;
					rcDrawDest.left		 = rcDrawDest.right;
					rcDrawDest.right	+= nIncreaseWidth;
					}

				int nIncreaseHeight	 =  min(rcSrc.Height(), (rcDest.bottom - rcDrawDest.bottom));
				if( nIncreaseHeight <= 0 ) break;
				rcDrawDest.top		 = rcDrawDest.bottom;
				rcDrawDest.bottom	+= nIncreaseHeight;
				rcDrawDest.left		 = rcDest.left;
				rcDrawDest.right	 = rcDest.left + rcSrc.Width();	
				}
			}
		}
	else{
		int nCopyBytesCt =  (bDontAffectAlphaChannel ? min(3, min(nBPPDest, nBPPSrc)) : min(nBPPDest, nBPPSrc));
		while( rcDrawDest.Width() > 0 ){
			while( rcDrawDest.Height() > 0 ){
				// Perform draw operation here. ##########
				for( UINT yLoop = 0; yLoop < rcDrawDest.Height(); yLoop ++ ){
					BYTE*	pPixelSrc	= (BYTE*)(((UINT)pbmpSrc->bmBits) + pbmpSrc->bmWidthBytes*(pbmpSrc->bmHeight - yLoop - rcSrc.top - 1) + nBPPSrc*rcSrc.left);
					BYTE*	pPixelDest	= (BYTE*)(((UINT)pbmpDest->bmBits) + pbmpDest->bmWidthBytes*(pbmpDest->bmHeight - yLoop - rcDrawDest.top - 1) + nBPPDest*rcDrawDest.left);

					for( UINT xLoop = 0; xLoop < rcDrawDest.Width(); xLoop ++ ){
						memcpy(pPixelDest, pPixelSrc, nCopyBytesCt);
						/*
						pPixelDest[0] = pPixelSrc[0]; // rgbBlue
						pPixelDest[1] = pPixelSrc[1]; // rgbGreen
						pPixelDest[2] = pPixelSrc[2]; // rgbRed
//						pPixelDest[3] = pPixelSrc[3]; // rgbReserved*/
						pPixelDest		= &pPixelDest[nBPPDest];
						pPixelSrc		= &pPixelSrc[nBPPSrc];
						}
					}
				// #######################################
				
				int nIncreaseWidth	 =  min(rcSrc.Width(), (rcDest.right - rcDrawDest.right));
				if( nIncreaseWidth <= 0 ) break;
				rcDrawDest.left		 = rcDrawDest.right;
				rcDrawDest.right	+= nIncreaseWidth;
				}

			int nIncreaseHeight	 =  min(rcSrc.Height(), (rcDest.bottom - rcDrawDest.bottom));
			if( nIncreaseHeight <= 0 ) break;
			rcDrawDest.top		 = rcDrawDest.bottom;
			rcDrawDest.bottom	+= nIncreaseHeight;
			rcDrawDest.left		 = rcDest.left;
			rcDrawDest.right	 = rcDest.left + rcSrc.Width();	
			}
		}
	return TRUE;
	}

BOOL		
ImageHelper::AlphaColorOnBitmap(HBITMAP hBmpDest, int xDest, int yDest, int nDrawWidth, int nDrawHeight, COLORREF crForeground, BYTE btAlpha){
	if( xDest < 0 || yDest < 0 ) return FALSE;

	RGBQUAD	 rgbQuadSrc = {
	((BYTE)(crForeground >> 16)),	// Blue
	((BYTE)(crForeground >> 8)),	// Green
	((BYTE)(crForeground)),			// Red
	btAlpha							// Alpha
	};

	BITMAP bmpDest;
	GetObject(hBmpDest, sizeof(BITMAP), &bmpDest);
	if( bmpDest.bmBits == NULL || 
		xDest >= bmpDest.bmWidth || yDest >= bmpDest.bmHeight )
		return FALSE;

	if( xDest + nDrawWidth > bmpDest.bmWidth )
		nDrawWidth = bmpDest.bmWidth - xDest;
	if( yDest + nDrawHeight > bmpDest.bmHeight )
		nDrawHeight = bmpDest.bmHeight - yDest;

	int nIndexDest, nIndexSrc = 0;
	int nGotoStartDest;
	int nWidth = nDrawWidth, nHeight = nDrawHeight;
	int xDrawed = 0, yDrawed = 0;
	int xSrc = 0, ySrc = 0;

	BYTE*	pPixelSrc	= (BYTE*)&rgbQuadSrc;
	BYTE*	pArrDest	= (BYTE*)bmpDest.bmBits;
	int		nAlpha		= ((int)btAlpha)&0xFF;
	int		nAlphaDest	=  255 - nAlpha;
	BYTE*	pbtColorByteByAlphaByte = GetColorByteByAlphaByteArray();
	int		nBPP		= bmpDest.bmBitsPixel / 8;

	while( yDrawed < nDrawHeight ){
		while( xDrawed < nDrawWidth ){
			// =================================================
			nIndexDest     = (xDrawed + xDest + (bmpDest.bmHeight - (1 + yDrawed + yDest))*bmpDest.bmWidthBytes);
			nGotoStartDest = bmpDest.bmWidthBytes;
			// =================================================

			// Basic operation on bitmaps. =====================
			for( int y = 0; y < nHeight; y++ ){
				BYTE* pPixelDest	= (BYTE*)&pArrDest[nIndexDest];
				for( int x = 0; x < nWidth; x++ ){
					// Set Alpha Color. ========================
					// Blue color byte.
					pPixelDest[0]		= pbtColorByteByAlphaByte[pPixelSrc[0]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[0]*256 + nAlphaDest];
					// Green color byte.
					pPixelDest[1]		= pbtColorByteByAlphaByte[pPixelSrc[1]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[1]*256 + nAlphaDest];
					// Red color byte.
					pPixelDest[2]		= pbtColorByteByAlphaByte[pPixelSrc[2]*256 + nAlpha] + pbtColorByteByAlphaByte[pPixelDest[2]*256 + nAlphaDest];
					// ==========================================
					pPixelDest			= &pPixelDest[nBPP];
					}
				nIndexDest -= nGotoStartDest;
				}
			// =================================================
			xDrawed += nWidth;
			}

		yDrawed += nHeight;
		xDrawed  = 0;
		}
	return TRUE;
	}

CImage*
ImageHelper::CombineImages(CImage* pImageBg, CImage* pImageFg, bool bFgIsAlphaChannelImage, int nCombinedImageBPP /*= 24*/, CRect rcForeground /*= CRect(0, 0, 0, 0)*/){
	if( !pImageBg || !pImageFg ) 
		return NULL;
	CBitmap bmpCombined;
	if( !CreateDIBBitmap(bFgIsAlphaChannelImage ? 32 : nCombinedImageBPP, bmpCombined, RGB(0, 0, 0), pImageBg->GetWidth(), pImageBg->GetHeight()) )
		return NULL;

	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject		(&bmpCombined);

	// Draw background image.
	CRect rcDraw(0, 0, pImageBg->GetWidth(), pImageBg->GetHeight());
	pImageBg->Draw(memDC, 0, 0, rcDraw.Width(), rcDraw.Height(), 0, 0, rcDraw.Width(), rcDraw.Height());

	if( rcForeground.IsRectEmpty() )
		rcForeground.SetRect(0, 0, pImageBg->GetWidth(), pImageBg->GetHeight());

	// Draw foreground image.
	if( bFgIsAlphaChannelImage ){
		CSize szBg(pImageBg->GetWidth(), pImageBg->GetHeight());
		CSize szFg(pImageFg->GetWidth(), pImageFg->GetHeight());

		// Copy alpha channel.
		if( szBg == CSize(rcForeground.Width(), rcForeground.Height()) ){
			BITMAP bmImageBg;
			GetObject(bmpCombined, sizeof(BITMAP), &bmImageBg);
			RGBQUAD* pColors = (RGBQUAD*)bmImageBg.bmBits;

			BITMAP bmImageFg;
			GetObject(*pImageFg, sizeof(BITMAP), &bmImageFg);
			RGBQUAD* pAlpha = (RGBQUAD*)bmImageFg.bmBits;

			if( bmImageFg.bmBitsPixel == 0x08 ){
				BYTE* pBtAlpha = 0;
				for(int y=0; y<szBg.cy; y++){
					pBtAlpha = (BYTE*)((LONG)bmImageFg.bmBits + bmImageFg.bmWidthBytes*(rcForeground.top + y) + rcForeground.left*sizeof(BYTE));
					for(int x=0; x<szBg.cx; x++){
						pColors[y*szBg.cx + x].rgbReserved = *pBtAlpha;
						pBtAlpha ++;
						}
					}
				}
			else{
				if( bmImageFg.bmBitsPixel == 32 ){
					for(int y=0; y<szBg.cy; y++){
						pAlpha = (RGBQUAD*)((LONG)bmImageFg.bmBits + bmImageFg.bmWidthBytes*(rcForeground.top + y) + rcForeground.left*sizeof(RGBQUAD));
						for(int x=0; x<szBg.cx; x++){
							pColors[y*szBg.cx + x].rgbReserved = pAlpha->rgbReserved;
							pAlpha ++;
							}
						}
					}
				else{
					int nBPPFg = bmImageFg.bmBitsPixel / 8;
					BYTE* pRGBFg = NULL;
					for(int y=0; y<szBg.cy; y++){
						pRGBFg = (BYTE*)((LONG)bmImageFg.bmBits + bmImageFg.bmWidthBytes*(rcForeground.top + y) + rcForeground.left*nBPPFg);
						for(int x=0; x<szBg.cx; x++){
							pColors[y*szBg.cx + x].rgbReserved = pRGBFg[0];
							pRGBFg = &pRGBFg[nBPPFg];
							}
						}	
					}
				}
			}
		}
	else{
		BitmapOnBitmap(bmpCombined, 0, 0, *pImageFg, rcForeground.left, rcForeground.top, 
			rcForeground.Width(), rcForeground.Height(), rcDraw.Width(), rcDraw.Height(), (pImageFg->GetBPP() == 32), 255, 1);
		}

	// Clean up.
	memDC.DeleteDC();

	CImage* pImageComb = new CImage();
	pImageComb->Attach((HBITMAP)bmpCombined.Detach());
	return pImageComb;
	}

bool
ImageHelper::CopyAlphaChannel(HBITMAP hDest, HBITMAP hSrcCopyFrom, CRect rcSrc /*= CRect(0, 0, 0, 0)*/, bool bCheckDestPixelAlpha /*= false*/, BYTE btMaxDestPixelAlphaChange /*= 255*/, int nSrcAlphaPixel /*= -1*/){
	BITMAP bmDest, bmSrc;
	GetObject(hDest,		sizeof(BITMAP), &bmDest);
	GetObject(hSrcCopyFrom, sizeof(BITMAP), &bmSrc);
	if( !bmDest.bmBits || !bmSrc.bmBits )
		return false;

	if( rcSrc.IsRectEmpty() ){
		rcSrc.left		= 0;
		rcSrc.top		= 0;
		rcSrc.right		= min(bmDest.bmWidth, bmSrc.bmWidth);
		rcSrc.bottom	= min(bmDest.bmHeight, bmSrc.bmHeight);
		}
	else{
		CRect rcDestImg(0, 0, bmDest.bmWidth, bmDest.bmHeight);
		CRect rcSrcImg(0, 0, bmSrc.bmWidth, bmSrc.bmHeight);

		rcSrc &= rcSrcImg;
		if( rcSrc.Width() > bmDest.bmWidth ||
			rcSrc.Height() > bmDest.bmHeight )
			return false;
		}

	BYTE*	pBitsDest			= (BYTE*)bmDest.bmBits;
	BYTE*	pBitsSrc			= (BYTE*)bmSrc.bmBits;
	int		nIndexSrc			= 0, nIndexDest = 0;
	int		nBPPSrc				= bmSrc.bmBitsPixel/8, nBPPDest = bmDest.bmBitsPixel/8;
	int		nSrcAlphaCompIndex	= (nBPPSrc == 4 ? 3 : 0);
	int		nDestAlphaCompIndex	= (nBPPDest == 4 ? 3 : 0);
	int		nWidth				= rcSrc.Width();
	int		nHeight				= rcSrc.Height();

	nSrcAlphaCompIndex			= (nBPPSrc == 3 ? 1 : nSrcAlphaCompIndex);
	nDestAlphaCompIndex			= (nBPPDest == 3 ? 1 : nDestAlphaCompIndex);

	if( nSrcAlphaPixel > -1 && nSrcAlphaPixel < 4 )
		nSrcAlphaCompIndex		= nSrcAlphaPixel;

	if( !bCheckDestPixelAlpha ){
		for(int y=0; y<nHeight; y++){
			pBitsSrc	= (BYTE*)((LONG)bmSrc.bmBits + bmSrc.bmWidthBytes*(rcSrc.top + y) + rcSrc.left*nBPPSrc);
			pBitsDest	= (BYTE*)((LONG)bmDest.bmBits + bmDest.bmWidthBytes*y);
			for(int x=0; x<nWidth; x++){
				pBitsDest[nDestAlphaCompIndex]	= pBitsSrc[nSrcAlphaCompIndex];
				pBitsDest						+= nBPPDest;
				pBitsSrc						+= nBPPSrc;
				}
			}	
		}
	else{
		BYTE*	pbtColorByteByAlphaByte = GetColorByteByAlphaByteArray();
		BYTE	btAlpha					= 0;
		for(int y=0; y<nHeight; y++){
			pBitsSrc	= (BYTE*)((LONG)bmSrc.bmBits + bmSrc.bmWidthBytes*(rcSrc.top + y) + rcSrc.left*nBPPSrc);
			pBitsDest	= (BYTE*)((LONG)bmDest.bmBits + bmDest.bmWidthBytes*y);
			for(int x=0; x<nWidth; x++){
				BYTE btAlpha					= pBitsSrc[nSrcAlphaCompIndex];
				pBitsDest[nDestAlphaCompIndex]	= pbtColorByteByAlphaByte[btAlpha*256 + btAlpha] + pbtColorByteByAlphaByte[pBitsDest[nDestAlphaCompIndex]*256 + (255 - btAlpha)];
				pBitsDest	+= nBPPDest;
				pBitsSrc	+= nBPPSrc;
				}
			}

		// Old one.
			/*
			for(int y=0; y<nHeight; y++){
				pBitsSrc	= (BYTE*)((LONG)bmSrc.bmBits + bmSrc.bmWidthBytes*(rcSrc.top + y) + rcSrc.left*nBPPSrc);
				pBitsDest	= (BYTE*)((LONG)bmDest.bmBits + bmDest.bmWidthBytes*y);
				for(int x=0; x<nWidth; x++){				
					if( pBitsDest[nDestAlphaCompIndex] <= btMaxDestPixelAlphaChange )
						pBitsDest[nDestAlphaCompIndex]	= pBitsSrc[nSrcAlphaCompIndex];

					pBitsDest						+= nBPPDest;
					pBitsSrc						+= nBPPSrc;
					}
				}	
			}*/
		}


	return true;
	}

//Desc:  Renders one image on another and calculates alpha channel of combined image.
HBITMAP
ImageHelper::CombineTwo32bppImages(HBITMAP hImage1, HBITMAP hImage2){
	BITMAP bmImage1, bmImage2;
	if( GetObject(hImage1, sizeof(BITMAP), &bmImage1) != sizeof(BITMAP) )
		return 0L;
	if( GetObject(hImage2, sizeof(BITMAP), &bmImage2) != sizeof(BITMAP) )
		return 0L;

	if( !bmImage1.bmBits || !bmImage2.bmBits ||
		bmImage1.bmBitsPixel != 32 || bmImage2.bmBitsPixel != 32 ||
		bmImage1.bmWidth != bmImage2.bmWidth ||
		bmImage1.bmHeight != bmImage2.bmHeight ){
		ASSERT(FALSE);
		return 0L;
		}

	CBitmap imageCombined;
	if( !ImageHelper::CreateDIBBitmap(32, imageCombined, RGB(0, 0, 0), bmImage1.bmWidth, bmImage1.bmHeight) )
		return 0L;

	BITMAP bmCombined;
	if( GetObject(imageCombined, sizeof(BITMAP), &bmCombined) != sizeof(BITMAP) ){
		imageCombined.DeleteObject();
		return 0L;
		}

	RGBQUAD*	pPixel1 = (RGBQUAD*)bmImage1.bmBits, *pPixel2 = (RGBQUAD*)bmImage2.bmBits, *pPixelDest = (RGBQUAD*)bmCombined.bmBits;
	int			nLoop = 0, nCt = bmImage1.bmHeight*bmImage1.bmWidth;
	float		fAlphaA = 0.0f, fAlphaB		= 0.0f, fAlphaDest = 0.0f;

	while(nLoop < nCt){
		fAlphaA		= (pPixel2->rgbReserved / 256.0f);
		fAlphaB		= (pPixel1->rgbReserved / 256.0f);
		fAlphaDest	= fAlphaA + fAlphaB*(1.0f - fAlphaA);
		
		// Second approach
		pPixelDest->rgbReserved = (BYTE)(256*fAlphaDest);
		pPixelDest->rgbBlue		= (BYTE)((pPixel2->rgbBlue*fAlphaA + (1.0f - fAlphaA)*pPixel1->rgbBlue*fAlphaB) / fAlphaDest);
		pPixelDest->rgbGreen	= (BYTE)((pPixel2->rgbGreen*fAlphaA + (1.0f - fAlphaA)*pPixel1->rgbGreen*fAlphaB) / fAlphaDest);
		pPixelDest->rgbRed		= (BYTE)((pPixel2->rgbRed*fAlphaA + (1.0f - fAlphaA)*pPixel1->rgbRed*fAlphaB) / fAlphaDest);

		pPixel1 ++;
		pPixel2 ++;
		pPixelDest ++;
		nLoop	++;
		}

	return (HBITMAP)imageCombined.Detach();
	}

bool
ImageHelper::RenderImage_OmitBorder(CDC* pDC, const CRect rcClipDestDC, const CRect rcDestDC, CImage* pImageSrc, CRect rcOmitBorder, BOOL bAlpha /*= TRUE*/, BOOL bSkipMiddleCenterPart /*= FALSE*/){
	if( !pDC || !pImageSrc || pImageSrc->IsNull() )
		return false;

	int		nImageWidth		= pImageSrc->GetWidth();
	int		nImageHeight	= pImageSrc->GetHeight();

	int		nSrcImageX		= (rcClipDestDC.left - rcDestDC.left);
	int		nSrcImageY		= (rcClipDestDC.top - rcDestDC.top);
	int		nImageDrawCX	= rcClipDestDC.Width();
	int		nImageDrawCY	= rcClipDestDC.Height();

	DrawImage32bpp_Omitborder(pDC, pImageSrc, rcDestDC, rcClipDestDC, &rcOmitBorder, bAlpha, bSkipMiddleCenterPart); 
	return true;
	}

bool
ImageHelper::RenderImage(CDC* pDC, const CRect rcClipDestDC, const CRect rcDestDC, CImage* pImageSrc, bool bScale /*= true*/, bool bUseAlphaChannel /*= true*/, ScaleMode mode /*= ScaleMode::ScaleMode_NearestNeghbore*/, 
						HBITMAP* pImageRendered /*= NULL*/, int nAlphaConstant /*= 255*/, bool bDontAffectAlphaChannel /*= false*/){
	if( !pDC || !pImageSrc )
		return false;

	bool bRet = false;
	// Scale image.
	if( bScale ){
		HBITMAP hDestBmp = 0;
		CBitmap bmBkgnd;
		CSize	szClipDesDC(rcClipDestDC.Width(), rcClipDestDC.Height());
		if( bUseAlphaChannel ){
			if( GetBitmapFromDC(32, bmBkgnd, pDC, rcClipDestDC.left, rcClipDestDC.top, szClipDesDC.cx, szClipDesDC.cy) )
				hDestBmp = (HBITMAP)bmBkgnd.Detach();
			}

		CRect rcDestInvalid;
		if( rcDestDC.left < 0 )
			rcDestInvalid.left = -1*rcDestDC.left;
		else
			rcDestInvalid.left = (rcClipDestDC.left - rcDestDC.left);

		if( rcDestDC.top < 0 )
			rcDestInvalid.top = -1*rcDestDC.top;
		else
			rcDestInvalid.top = (rcClipDestDC.top - rcDestDC.top);

		rcDestInvalid.right		= rcDestInvalid.left + szClipDesDC.cx;
		rcDestInvalid.bottom	= rcDestInvalid.top + szClipDesDC.cy;

		if( ImageHelper::ScaleBitmap(mode, *pImageSrc, rcDestDC.Width(), rcDestDC.Height(), rcDestInvalid, hDestBmp, bUseAlphaChannel, nAlphaConstant, bDontAffectAlphaChannel) ){
			// Render on DC bitmap instead of DC. {{
			// Reason: We should keep alpha channel.
			bool bRenderOnDC = true;
			CBitmap bmDestBmp;
			bmDestBmp.Attach(hDestBmp);
			CBitmap* pbmPrevSelected = (CBitmap*)pDC->SelectObject(&bmDestBmp);
			if( pbmPrevSelected ){
				pDC->SelectObject(pbmPrevSelected);
				bRet = (1 == BitmapOnBitmap((HBITMAP)pbmPrevSelected->m_hObject, rcClipDestDC.left, rcClipDestDC.top, hDestBmp, 0, 0, 
					szClipDesDC.cx, szClipDesDC.cy, szClipDesDC.cx, szClipDesDC.cy, 0, 255, 0, bDontAffectAlphaChannel));
				bRenderOnDC = false;
				}
			else
				pDC->SelectObject(pbmPrevSelected);
			bmDestBmp.Detach();
			// }}

			if( bRenderOnDC ){
				CDC memDC;
				memDC.CreateCompatibleDC(pDC);
				::SelectObject(memDC, hDestBmp);
				::BitBlt(*pDC, rcClipDestDC.left, rcClipDestDC.top, szClipDesDC.cx, szClipDesDC.cy, memDC, 0, 0, SRCCOPY);
				memDC.DeleteDC();
				bRet = true;
				}
			}

		if( bRet && pImageRendered )
			*pImageRendered = hDestBmp;
		else
		if( hDestBmp )
			::DeleteObject(hDestBmp);
		}
	else{ // Repeat image
		CSize szImage		(pImageSrc->GetWidth(), pImageSrc->GetHeight());
		CRect rcOmitBorder	((szImage.cx-2)/2, (szImage.cy-2)/2, (szImage.cx-2)/2, (szImage.cy-2)/2);
		return ImageHelper::RenderImage_OmitBorder(pDC, rcClipDestDC, rcDestDC, pImageSrc, rcOmitBorder, (pImageSrc->GetBPP() == 32));
		}
	return bRet;
	}

BOOL
ImageHelper::ScaleBitmap(ScaleMode mode, HBITMAP hBmpSrc, UINT nDestCX, UINT nDestCY, CRect rcInvalidDest, HBITMAP& hBmpDest, bool bUseAlphaChannel, BYTE btConstantAlpha /*= 255*/, bool bDontAffectAlphaChannel){
	BOOL bRet = 0;
	switch( mode ){
		case ScaleMode::ScaleMode_NearestNeghbore:{
			bRet = ScaleBitmap_NearestN(hBmpSrc, nDestCX, nDestCY, rcInvalidDest, hBmpDest, bUseAlphaChannel, btConstantAlpha, bDontAffectAlphaChannel);
			break;
			}
		case ScaleMode::ScaleMode_Bilinier:{
			bRet = ScaleBitmap_Bilinier(hBmpSrc, nDestCX, nDestCY, rcInvalidDest, hBmpDest, bUseAlphaChannel, btConstantAlpha, bDontAffectAlphaChannel);
			break;
			}
		}
	return bRet;
	}

BOOL
ImageHelper::ScaleBitmap_NearestN(HBITMAP hBmpSrc, UINT nDestCX, UINT nDestCY, CRect rcInvalidDest, HBITMAP& hBmpDest, bool bUseAlphaChannel, BYTE btConstantAlpha, bool bDontAffectAlphaChannel){
	BITMAP bmSrc;
	bmSrc.bmBits = NULL;
	GetObject(hBmpSrc, sizeof(BITMAP), &bmSrc);
	if( !bmSrc.bmBits ) return 0;

	CRect rcDest(0, 0, nDestCX, nDestCY);
	rcInvalidDest &= rcDest;
	if( rcInvalidDest.IsRectEmpty() )
		return 0;

	CBitmap bmDest;
	BITMAP bmpDest;

	if( hBmpDest != NULL && GetObject(hBmpDest, sizeof(BITMAP), &bmpDest) == sizeof(BITMAP) && 
		bmpDest.bmWidth >= rcInvalidDest.Width() && bmpDest.bmHeight >= rcInvalidDest.Height() ){
		bmDest.Attach(hBmpDest);
		}
	else{
		if( !CreateDIBBitmap(bmSrc.bmBitsPixel, bmDest, (COLORREF)0, rcInvalidDest.Width(), rcInvalidDest.Height(), &bmpDest) )
			return 0;
		}

	// Calculate scaling params
	float dx					= 1.0f / (nDestCX / (float)bmSrc.bmWidth);
	float dy					= 1.0f / (nDestCY / (float)bmSrc.bmHeight);
	long _width					= bmpDest.bmWidth;
	long _height				= bmpDest.bmHeight;
	long _pitch					= bmpDest.bmWidthBytes;

	long m_iPitch				= bmSrc.bmWidthBytes;
	long m_iBpp					= (bmSrc.bmBitsPixel / 8);
	int biWidth					= bmSrc.bmWidth;
	int biHeight				= bmSrc.bmHeight;

	fixed f_dx					= ftofx(dx);
	fixed f_dy					= ftofx(dy);
	fixed f_1					= itofx(1);

	if( nDestCX == biWidth && nDestCY == biHeight ){
		if( BitmapOnBitmap(&bmpDest, 0, 0, &bmSrc, rcInvalidDest.left, rcInvalidDest.top, biWidth, biHeight, rcInvalidDest.Width(), rcInvalidDest.Height(), bUseAlphaChannel, btConstantAlpha, 0, bDontAffectAlphaChannel) ){
			hBmpDest = (HBITMAP)bmDest.Detach();
			return 1;
			}
		return 0;
		}

	if( rcInvalidDest.Width() < nDestCX ||
		rcInvalidDest.Height() < nDestCY ){
		int nHeight				= rcInvalidDest.Height();
		rcInvalidDest.top		= (nDestCY - rcInvalidDest.top - nHeight);
		rcInvalidDest.bottom	= rcInvalidDest.top + nHeight;
		}

	// Create temporary bitmap
	DWORD	dwSize				= _pitch * _height;
	LPBYTE	lpData				= (LPBYTE)bmpDest.bmBits;

	// Scale bitmap
	DWORD	dwDstHorizontalOffset;
	DWORD	dwDstVerticalOffset = 0;
	DWORD	dwDstTotalOffset;
	LPDWORD lpSrcData			= (LPDWORD)bmSrc.bmBits;
	DWORD	dwSrcTotalOffset	= 0;
	LPDWORD lpDstData			= (LPDWORD)lpData;

	if( bmpDest.bmBitsPixel != 32 )
		bUseAlphaChannel = false;

	if( bUseAlphaChannel ){
		BYTE*	pbtColorByteByAlphaByte = GetColorByteByAlphaByteArray();
		if( btConstantAlpha == 255 ){
			for (long i=rcInvalidDest.bottom-1; i>=rcInvalidDest.top; i--){
				dwDstHorizontalOffset	= 0;
				dwDstVerticalOffset		= (i - rcInvalidDest.top)*_pitch;
				for (long j=rcInvalidDest.left; j<rcInvalidDest.right; j++){
					// Update destination total offset
					dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

					// Update bitmap
					fixed f_i						= itofx(i);
					fixed f_j						= itofx(j);
					fixed f_a						= Mulfx(f_i, f_dy);
					fixed f_b						= Mulfx(f_j, f_dx);
					long m							= fxtoi(f_a);
					long n							= fxtoi(f_b);
					dwSrcTotalOffset				= m*m_iPitch + n*m_iBpp;

					RGBQUAD* pPixelSrc				= (RGBQUAD*)&lpSrcData[dwSrcTotalOffset>>2];
					BYTE red						= pPixelSrc->rgbRed;
					BYTE green						= pPixelSrc->rgbGreen;
					BYTE blue						= pPixelSrc->rgbBlue;
					BYTE alpha						= pPixelSrc->rgbReserved;
			
					//lpDstData[dwDstTotalOffset>>2]	= lpSrcData[dwSrcTotalOffset>>2];

					// Set Alpha Color. ========================
					RGBQUAD* pPixelDest = (RGBQUAD*)&lpDstData[dwDstTotalOffset>>2]; 
					// Blue color byte.
					((BYTE*)pPixelDest)[0] = pbtColorByteByAlphaByte[blue*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbBlue*256 + (255 - alpha)];
					// Green color byte.
					((BYTE*)pPixelDest)[1] = pbtColorByteByAlphaByte[green*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbGreen*256 + (255 - alpha)];
					// Red color byte.
					((BYTE*)pPixelDest)[2] = pbtColorByteByAlphaByte[red*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbRed*256 + (255 - alpha)];
					// Alpha
					((BYTE*)pPixelDest)[3] = alpha;
					// ==========================================

					// Update destination horizontal offset
					dwDstHorizontalOffset += m_iBpp;
					}
				}
			}
		else{
			for (long i=rcInvalidDest.bottom-1; i>=rcInvalidDest.top; i--){
				dwDstHorizontalOffset	= 0;
				dwDstVerticalOffset		= (i - rcInvalidDest.top)*_pitch;
				for (long j=rcInvalidDest.left; j<rcInvalidDest.right; j++){
					// Update destination total offset
					dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

					// Update bitmap
					fixed f_i						= itofx(i);
					fixed f_j						= itofx(j);
					fixed f_a						= Mulfx(f_i, f_dy);
					fixed f_b						= Mulfx(f_j, f_dx);
					long m							= fxtoi(f_a);
					long n							= fxtoi(f_b);
					dwSrcTotalOffset				= m*m_iPitch + n*m_iBpp;

					RGBQUAD* pPixelSrc				= (RGBQUAD*)&lpSrcData[dwSrcTotalOffset>>2];
					BYTE red						= pPixelSrc->rgbRed;
					BYTE green						= pPixelSrc->rgbGreen;
					BYTE blue						= pPixelSrc->rgbBlue;
					BYTE alpha						= pPixelSrc->rgbReserved;
			
					//lpDstData[dwDstTotalOffset>>2]	= lpSrcData[dwSrcTotalOffset>>2];

					// Set Alpha Color. ========================
					RGBQUAD* pPixelDest = (RGBQUAD*)&lpDstData[dwDstTotalOffset>>2]; 
					// Blue color byte.
					((BYTE*)pPixelDest)[0] = pbtColorByteByAlphaByte[blue*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbBlue*256 + (255 - alpha)];
					// Green color byte.
					((BYTE*)pPixelDest)[1] = pbtColorByteByAlphaByte[green*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbGreen*256 + (255 - alpha)];
					// Red color byte.
					((BYTE*)pPixelDest)[2] = pbtColorByteByAlphaByte[red*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbRed*256 + (255 - alpha)];
					// Alpha
					if( alpha > 0 ){
						((BYTE*)pPixelDest)[3] = btConstantAlpha;
						}
					// ==========================================

					// Update destination horizontal offset
					dwDstHorizontalOffset += m_iBpp;
					}
				}
			}
		}
	else{
		for (long i=rcInvalidDest.bottom-1; i>=rcInvalidDest.top; i--){
			dwDstHorizontalOffset	= 0;
			dwDstVerticalOffset		= (i - rcInvalidDest.top)*_pitch;
			for (long j=rcInvalidDest.left; j<rcInvalidDest.right; j++){
				// Update destination total offset
				dwDstTotalOffset				= dwDstVerticalOffset + dwDstHorizontalOffset;

				// Update bitmap
				fixed f_i						= itofx(i);
				fixed f_j						= itofx(j);
				fixed f_a						= Mulfx(f_i, f_dy);
				fixed f_b						= Mulfx(f_j, f_dx);
				long m							= fxtoi(f_a);
				long n							= fxtoi(f_b);
				dwSrcTotalOffset				= m*m_iPitch + n*m_iBpp;

				//lpDstData[dwDstTotalOffset>>2]	= lpSrcData[dwSrcTotalOffset>>2];
				((BYTE*)lpDstData)[dwDstTotalOffset]	= ((BYTE*)lpSrcData)[dwSrcTotalOffset];		// Blue
				((BYTE*)lpDstData)[dwDstTotalOffset+1]	= ((BYTE*)lpSrcData)[dwSrcTotalOffset+1];	// Green
				((BYTE*)lpDstData)[dwDstTotalOffset+2]	= ((BYTE*)lpSrcData)[dwSrcTotalOffset+2];	// Red

				// Update destination horizontal offset
				dwDstHorizontalOffset += m_iBpp;
				}
			}
		}

	hBmpDest = (HBITMAP)bmDest.Detach();
	return 1;
	}

BOOL
ImageHelper::ScaleBitmap_Bilinier(HBITMAP hBmpSrc, UINT nDestCX, UINT nDestCY, CRect rcInvalidDest, HBITMAP& hBmpDest, bool bUseAlphaChannel, BYTE btConstantAlpha, bool bDontAffectAlphaChannel){
	BITMAP bmSrc;
	bmSrc.bmBits = NULL;
	GetObject(hBmpSrc, sizeof(BITMAP), &bmSrc);
	if( !bmSrc.bmBits ) return 0;

	CRect rcDest(0, 0, nDestCX, nDestCY);
	rcInvalidDest &= rcDest;
	if( rcInvalidDest.IsRectEmpty() )
		return 0;

	CBitmap bmDest;
	BITMAP bmpDest;
	memset(&bmpDest, 0, sizeof(BITMAP));

	if( hBmpDest != NULL && GetObject(hBmpDest, sizeof(BITMAP), &bmpDest) == sizeof(BITMAP) && 
		bmpDest.bmWidth >= rcInvalidDest.Width() && bmpDest.bmHeight >= rcInvalidDest.Height() ){
		bmDest.Attach(hBmpDest);
		}
	else{
		if( !CreateDIBBitmap(bmSrc.bmBitsPixel, bmDest, (COLORREF)0, rcInvalidDest.Width(), rcInvalidDest.Height(), &bmpDest) )
			return 0;
		}

	// Calculate scaling params
	float dx					= 1.0f / (nDestCX / (float)bmSrc.bmWidth);
	float dy					= 1.0f / (nDestCY / (float)bmSrc.bmHeight);
	long _width					= bmpDest.bmWidth;
	long _height				= bmpDest.bmHeight;
	long _pitch					= bmpDest.bmWidthBytes;

	long m_iPitch				= bmSrc.bmWidthBytes;
	long m_iBpp					= (bmSrc.bmBitsPixel / 8);
	int biWidth					= bmSrc.bmWidth;
	int biHeight				= bmSrc.bmHeight;

	fixed f_dx					= ftofx(dx);
	fixed f_dy					= ftofx(dy);
	fixed f_1					= itofx(1);
	
	if( nDestCX == biWidth && nDestCY == biHeight ){
		if( BitmapOnBitmap(&bmpDest, 0, 0, &bmSrc, rcInvalidDest.left, rcInvalidDest.top, biWidth, biHeight, rcInvalidDest.Width(), rcInvalidDest.Height(), bUseAlphaChannel, btConstantAlpha, 0, bDontAffectAlphaChannel) ){
			hBmpDest = (HBITMAP)bmDest.Detach();
			return 1;
			}
		return 0;
		}

	if( rcInvalidDest.Width() < nDestCX ||
		rcInvalidDest.Height() < nDestCY ){
		int nHeight				= rcInvalidDest.Height();
		rcInvalidDest.top		= (nDestCY - rcInvalidDest.top - nHeight);
		rcInvalidDest.bottom	= rcInvalidDest.top + nHeight;
		}

	// Create temporary bitmap
	DWORD	dwSize				= _pitch * _height;
	LPBYTE	lpData				= (LPBYTE)bmpDest.bmBits;
	ASSERT(lpData);

	// Scale bitmap
	DWORD	dwDstHorizontalOffset;
	DWORD	dwDstVerticalOffset = 0;
	DWORD	dwDstTotalOffset;
	LPDWORD lpSrcData			= (LPDWORD)bmSrc.bmBits;
	DWORD	dwSrcTotalOffset	= 0;
	LPDWORD lpDstData			= (LPDWORD)lpData;

	if( /*bmpDest.bmBitsPixel != 32 &&*/ bmSrc.bmBitsPixel != 32 )
		bUseAlphaChannel = false;

	if( !bDontAffectAlphaChannel && bmpDest.bmBitsPixel != 32 )
		bDontAffectAlphaChannel = true;

	if( bUseAlphaChannel ){
		BYTE*	pbtColorByteByAlphaByte = GetColorByteByAlphaByteArray();
		if( btConstantAlpha == 255 ){
			for (long i=rcInvalidDest.bottom-1; i>=rcInvalidDest.top; i--){
				dwDstHorizontalOffset	= 0;
				dwDstVerticalOffset		= (i - rcInvalidDest.top)*_pitch;
				for (long j=rcInvalidDest.left; j<rcInvalidDest.right; j++){
					// Update destination total offset
					dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

					// Update bitmap
					fixed f_i = itofx(i);
					fixed f_j = itofx(j);
					fixed f_a = Mulfx(f_i, f_dy);
					fixed f_b = Mulfx(f_j, f_dx);
					long m = fxtoi(f_a);
					long n = fxtoi(f_b);
					fixed f_f = f_a - itofx(m);
					fixed f_g = f_b - itofx(n);
					dwSrcTotalOffset = m*m_iPitch + n*m_iBpp;
					DWORD dwSrcTopLeft = dwSrcTotalOffset;
					DWORD dwSrcTopRight = dwSrcTotalOffset + m_iBpp;
					if (n >= biWidth-1)
						dwSrcTopRight = dwSrcTotalOffset;
					DWORD dwSrcBottomLeft = dwSrcTotalOffset + m_iPitch;
					if (m >= biHeight-1 )
						dwSrcBottomLeft = dwSrcTotalOffset;
					DWORD dwSrcBottomRight = dwSrcTotalOffset + m_iPitch + m_iBpp;
					if ((n >= biWidth-1) || (m >= biHeight-1))
						dwSrcBottomRight = dwSrcTotalOffset;

					fixed f_w1		= Mulfx(f_1-f_f, f_1-f_g);
					fixed f_w2		= Mulfx(f_1-f_f, f_g);
					fixed f_w3		= Mulfx(f_f, f_1-f_g);
					fixed f_w4		= Mulfx(f_f, f_g);
					/*
					_PIXEL pixel1	= lpSrcData[dwSrcTopLeft>>2];
					_PIXEL pixel2	= lpSrcData[dwSrcTopRight>>2];
					_PIXEL pixel3	= lpSrcData[dwSrcBottomLeft>>2];
					_PIXEL pixel4	= lpSrcData[dwSrcBottomRight>>2];
					*/
					_PIXEL pixel1	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcTopLeft]);
					_PIXEL pixel2	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcTopRight]);
					_PIXEL pixel3	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcBottomLeft]);
					_PIXEL pixel4	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcBottomRight]);

					fixed f_r1		= itofx(_GetRValue(pixel1));
					fixed f_r2		= itofx(_GetRValue(pixel2));
					fixed f_r3		= itofx(_GetRValue(pixel3));
					fixed f_r4		= itofx(_GetRValue(pixel4));
					fixed f_g1		= itofx(_GetGValue(pixel1));
					fixed f_g2		= itofx(_GetGValue(pixel2));
					fixed f_g3		= itofx(_GetGValue(pixel3));
					fixed f_g4		= itofx(_GetGValue(pixel4));
					fixed f_b1		= itofx(_GetBValue(pixel1));
					fixed f_b2		= itofx(_GetBValue(pixel2));
					fixed f_b3		= itofx(_GetBValue(pixel3));
					fixed f_b4		= itofx(_GetBValue(pixel4));

					fixed f_a1		= itofx(_GetAValue(pixel1));
					fixed f_a2		= itofx(_GetAValue(pixel2));
					fixed f_a3		= itofx(_GetAValue(pixel3));
					fixed f_a4		= itofx(_GetAValue(pixel4));

					BYTE red		= (BYTE)fxtoi(Mulfx(f_w1, f_r1) + Mulfx(f_w2, f_r2) + Mulfx(f_w3, f_r3) + Mulfx(f_w4, f_r4));
					BYTE green		= (BYTE)fxtoi(Mulfx(f_w1, f_g1) + Mulfx(f_w2, f_g2) + Mulfx(f_w3, f_g3) + Mulfx(f_w4, f_g4));
					BYTE blue		= (BYTE)fxtoi(Mulfx(f_w1, f_b1) + Mulfx(f_w2, f_b2) + Mulfx(f_w3, f_b3) + Mulfx(f_w4, f_b4));
					BYTE alpha		= (BYTE)fxtoi(Mulfx(f_w1, f_a1) + Mulfx(f_w2, f_a2) + Mulfx(f_w3, f_a3) + Mulfx(f_w4, f_a4));
					/*
					// Set Alpha Color. ========================
					RGBQUAD* pPixelDest = (RGBQUAD*)&lpDstData[dwDstTotalOffset>>2]; 
					// Blue color byte.
					((BYTE*)pPixelDest)[0] = pbtColorByteByAlphaByte[blue*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbBlue*256 + (255 - alpha)];
					// Green color byte.
					((BYTE*)pPixelDest)[1] = pbtColorByteByAlphaByte[green*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbGreen*256 + (255 - alpha)];
					// Red color byte.
					((BYTE*)pPixelDest)[2] = pbtColorByteByAlphaByte[red*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbRed*256 + (255 - alpha)];
					// Alpha
					if( !bDontAffectAlphaChannel )
						((BYTE*)pPixelDest)[3] = alpha;
					// ==========================================
					*/
					BYTE* pPixelDest = (BYTE*)&((BYTE*)lpDstData)[dwDstTotalOffset]; 
					((BYTE*)lpDstData)[dwDstTotalOffset]	= pbtColorByteByAlphaByte[blue*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest[0]*256 + (255 - alpha)]; // Blue
					((BYTE*)lpDstData)[dwDstTotalOffset+1]	= pbtColorByteByAlphaByte[green*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest[1]*256 + (255 - alpha)];// Green
					((BYTE*)lpDstData)[dwDstTotalOffset+2]	= pbtColorByteByAlphaByte[red*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest[2]*256 + (255 - alpha)];// Red
					// Alpha
					if( !bDontAffectAlphaChannel )
						((BYTE*)pPixelDest)[3] = alpha;

					// Update destination horizontal offset
					dwDstHorizontalOffset += m_iBpp;
					}
				}
			}
		else{
			for (long i=rcInvalidDest.bottom-1; i>=rcInvalidDest.top; i--){
				dwDstHorizontalOffset	= 0;
				dwDstVerticalOffset		= (i - rcInvalidDest.top)*_pitch;
				for (long j=rcInvalidDest.left; j<rcInvalidDest.right; j++){
					// Update destination total offset
					dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

					// Update bitmap
					fixed f_i = itofx(i);
					fixed f_j = itofx(j);
					fixed f_a = Mulfx(f_i, f_dy);
					fixed f_b = Mulfx(f_j, f_dx);
					long m = fxtoi(f_a);
					long n = fxtoi(f_b);
					fixed f_f = f_a - itofx(m);
					fixed f_g = f_b - itofx(n);
					dwSrcTotalOffset = m*m_iPitch + n*m_iBpp;
					DWORD dwSrcTopLeft = dwSrcTotalOffset;
					DWORD dwSrcTopRight = dwSrcTotalOffset + m_iBpp;
					if (n >= biWidth-1)
						dwSrcTopRight = dwSrcTotalOffset;
					DWORD dwSrcBottomLeft = dwSrcTotalOffset + m_iPitch;
					if (m >= biHeight-1 )
						dwSrcBottomLeft = dwSrcTotalOffset;
					DWORD dwSrcBottomRight = dwSrcTotalOffset + m_iPitch + m_iBpp;
					if ((n >= biWidth-1) || (m >= biHeight-1))
						dwSrcBottomRight = dwSrcTotalOffset;

					fixed f_w1		= Mulfx(f_1-f_f, f_1-f_g);
					fixed f_w2		= Mulfx(f_1-f_f, f_g);
					fixed f_w3		= Mulfx(f_f, f_1-f_g);
					fixed f_w4		= Mulfx(f_f, f_g);
					/*
					_PIXEL pixel1	= lpSrcData[dwSrcTopLeft>>2];
					_PIXEL pixel2	= lpSrcData[dwSrcTopRight>>2];
					_PIXEL pixel3	= lpSrcData[dwSrcBottomLeft>>2];
					_PIXEL pixel4	= lpSrcData[dwSrcBottomRight>>2];
*/
					_PIXEL pixel1	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcTopLeft]);
					_PIXEL pixel2	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcTopRight]);
					_PIXEL pixel3	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcBottomLeft]);
					_PIXEL pixel4	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcBottomRight]);

					fixed f_r1		= itofx(_GetRValue(pixel1));
					fixed f_r2		= itofx(_GetRValue(pixel2));
					fixed f_r3		= itofx(_GetRValue(pixel3));
					fixed f_r4		= itofx(_GetRValue(pixel4));
					fixed f_g1		= itofx(_GetGValue(pixel1));
					fixed f_g2		= itofx(_GetGValue(pixel2));
					fixed f_g3		= itofx(_GetGValue(pixel3));
					fixed f_g4		= itofx(_GetGValue(pixel4));
					fixed f_b1		= itofx(_GetBValue(pixel1));
					fixed f_b2		= itofx(_GetBValue(pixel2));
					fixed f_b3		= itofx(_GetBValue(pixel3));
					fixed f_b4		= itofx(_GetBValue(pixel4));

					fixed f_a1		= itofx(_GetAValue(pixel1));
					fixed f_a2		= itofx(_GetAValue(pixel2));
					fixed f_a3		= itofx(_GetAValue(pixel3));
					fixed f_a4		= itofx(_GetAValue(pixel4));

					BYTE red		= (BYTE)fxtoi(Mulfx(f_w1, f_r1) + Mulfx(f_w2, f_r2) + Mulfx(f_w3, f_r3) + Mulfx(f_w4, f_r4));
					BYTE green		= (BYTE)fxtoi(Mulfx(f_w1, f_g1) + Mulfx(f_w2, f_g2) + Mulfx(f_w3, f_g3) + Mulfx(f_w4, f_g4));
					BYTE blue		= (BYTE)fxtoi(Mulfx(f_w1, f_b1) + Mulfx(f_w2, f_b2) + Mulfx(f_w3, f_b3) + Mulfx(f_w4, f_b4));
					BYTE alpha		= (BYTE)fxtoi(Mulfx(f_w1, f_a1) + Mulfx(f_w2, f_a2) + Mulfx(f_w3, f_a3) + Mulfx(f_w4, f_a4));
					/*
					// Set Alpha Color. ========================
					RGBQUAD* pPixelDest = (RGBQUAD*)&lpDstData[dwDstTotalOffset>>2]; 
					// Blue color byte.
					((BYTE*)pPixelDest)[0] = pbtColorByteByAlphaByte[blue*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbBlue*256 + (255 - alpha)];
					// Green color byte.
					((BYTE*)pPixelDest)[1] = pbtColorByteByAlphaByte[green*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbGreen*256 + (255 - alpha)];
					// Red color byte.
					((BYTE*)pPixelDest)[2] = pbtColorByteByAlphaByte[red*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest->rgbRed*256 + (255 - alpha)];
					*/

					BYTE* pPixelDest = (BYTE*)&((BYTE*)lpDstData)[dwDstTotalOffset]; 
					((BYTE*)lpDstData)[dwDstTotalOffset]	= pbtColorByteByAlphaByte[blue*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest[0]*256 + (255 - alpha)]; // Blue
					((BYTE*)lpDstData)[dwDstTotalOffset+1]	= pbtColorByteByAlphaByte[green*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest[1]*256 + (255 - alpha)];// Green
					((BYTE*)lpDstData)[dwDstTotalOffset+2]	= pbtColorByteByAlphaByte[red*256 + alpha] + pbtColorByteByAlphaByte[pPixelDest[2]*256 + (255 - alpha)];	// Red

					// Alpha
				//	if( alpha > 0 ){
				//		((BYTE*)pPixelDest)[3] = btConstantAlpha;
				//		}
					// ==========================================

					// Update destination horizontal offset
					dwDstHorizontalOffset += m_iBpp;
					}
				}
			}
		}
	else{
		for (long i=rcInvalidDest.bottom-1; i>=rcInvalidDest.top; i--){
			dwDstHorizontalOffset	= 0;
			dwDstVerticalOffset		= (i - rcInvalidDest.top)*_pitch;
			for (long j=rcInvalidDest.left; j<rcInvalidDest.right; j++){
				// Update destination total offset
				dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

				// Update bitmap
				fixed f_i = itofx(i);
				fixed f_j = itofx(j);
				fixed f_a = Mulfx(f_i, f_dy);
				fixed f_b = Mulfx(f_j, f_dx);
				long m = fxtoi(f_a);
				long n = fxtoi(f_b);
				fixed f_f = f_a - itofx(m);
				fixed f_g = f_b - itofx(n);
				dwSrcTotalOffset = m*m_iPitch + n*m_iBpp;
				DWORD dwSrcTopLeft = dwSrcTotalOffset;
				DWORD dwSrcTopRight = dwSrcTotalOffset + m_iBpp;
				if (n >= biWidth-1)
					dwSrcTopRight = dwSrcTotalOffset;
				DWORD dwSrcBottomLeft = dwSrcTotalOffset + m_iPitch;
				if (m >= biHeight-1 )
					dwSrcBottomLeft = dwSrcTotalOffset;
				DWORD dwSrcBottomRight = dwSrcTotalOffset + m_iPitch + m_iBpp;
				if ((n >= biWidth-1) || (m >= biHeight-1))
					dwSrcBottomRight = dwSrcTotalOffset;

				fixed f_w1		= Mulfx(f_1-f_f, f_1-f_g);
				fixed f_w2		= Mulfx(f_1-f_f, f_g);
				fixed f_w3		= Mulfx(f_f, f_1-f_g);
				fixed f_w4		= Mulfx(f_f, f_g);

				_PIXEL pixel1	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcTopLeft]);
				_PIXEL pixel2	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcTopRight]);
				_PIXEL pixel3	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcBottomLeft]);
				_PIXEL pixel4	= *((DWORD*)&((BYTE*)lpSrcData)[dwSrcBottomRight]);
	
				/*
				_PIXEL pixel1	= lpSrcData[dwSrcTopLeft>>2];
				_PIXEL pixel2	= lpSrcData[dwSrcTopRight>>2];
				_PIXEL pixel3	= lpSrcData[dwSrcBottomLeft>>2];
				_PIXEL pixel4	= lpSrcData[dwSrcBottomRight>>2];*/

				fixed f_r1		= itofx(_GetRValue(pixel1));
				fixed f_r2		= itofx(_GetRValue(pixel2));
				fixed f_r3		= itofx(_GetRValue(pixel3));
				fixed f_r4		= itofx(_GetRValue(pixel4));
				fixed f_g1		= itofx(_GetGValue(pixel1));
				fixed f_g2		= itofx(_GetGValue(pixel2));
				fixed f_g3		= itofx(_GetGValue(pixel3));
				fixed f_g4		= itofx(_GetGValue(pixel4));
				fixed f_b1		= itofx(_GetBValue(pixel1));
				fixed f_b2		= itofx(_GetBValue(pixel2));
				fixed f_b3		= itofx(_GetBValue(pixel3));
				fixed f_b4		= itofx(_GetBValue(pixel4));

				BYTE red		= (BYTE)fxtoi(Mulfx(f_w1, f_r1) + Mulfx(f_w2, f_r2) + Mulfx(f_w3, f_r3) + Mulfx(f_w4, f_r4));
				BYTE green		= (BYTE)fxtoi(Mulfx(f_w1, f_g1) + Mulfx(f_w2, f_g2) + Mulfx(f_w3, f_g3) + Mulfx(f_w4, f_g4));
				BYTE blue		= (BYTE)fxtoi(Mulfx(f_w1, f_b1) + Mulfx(f_w2, f_b2) + Mulfx(f_w3, f_b3) + Mulfx(f_w4, f_b4));

				((BYTE*)lpDstData)[dwDstTotalOffset]	= blue; // Blue
				((BYTE*)lpDstData)[dwDstTotalOffset+1]	= green;// Green
				((BYTE*)lpDstData)[dwDstTotalOffset+2]	= red;	// Red
				//lpDstData[dwDstTotalOffset>>2] = _RGB(red, green, blue);

				// Update destination horizontal offset
				dwDstHorizontalOffset += m_iBpp;
				}
			}
		}

	hBmpDest = (HBITMAP)bmDest.Detach();
	return 1;
	}

bool
ImageHelper::GetSelectedDibImageFromDC(CDC* pDC, HBITMAP& hDibImage){
	if( !pDC ) 
		return false;
	CBitmap* pTemp	= GetTempBitmap();
	hDibImage		= (HBITMAP)::SelectObject(pDC->m_hDC, (HBITMAP)pTemp->m_hObject);

	if( hDibImage != NULL ){
		BITMAP bmInfo;
		if( GetObject(hDibImage, sizeof(BITMAP), &bmInfo) == sizeof(BITMAP) && bmInfo.bmBits != NULL ){
			::SelectObject(pDC->m_hDC, hDibImage);
			return true;
			}
		else{
			::SelectObject(pDC->m_hDC, hDibImage);
			hDibImage = NULL;
			}
		}
	return false;
	}

bool
ImageHelper::DrawImage32bpp_Omitborder(CDC* pDC, CImage* pImageSrc, CRect rcDestDC, CRect rcClipDC, CRect rcOmitBorder, BOOL bAlpha /*= TRUE*/, BOOL bSkipMiddleCenterPart /*= FALSE*/){
	// Parameters validation. ===
	if( !pDC || !pImageSrc )
		return false;

	if( rcClipDC.Width() <= 0 ||
		rcClipDC.Height() <= 0 )
		return false;

	if( rcDestDC.Width() <= 0 ||
		rcDestDC.Height() <= 0 )
		return false;
	
	rcClipDC &= rcDestDC;
	if( rcClipDC.IsRectEmpty() )
		return false;
	// ==========================

	int nSrcCX	= pImageSrc->GetWidth();
	int nSrcCY	= pImageSrc->GetHeight();

	bool bBkImage = false;
	CBitmap bmpSrc;
	bmpSrc.Attach((HBITMAP)pImageSrc->Detach());

	// Retrieve destination bitmap image. (To draw image on)
	CBitmap bmBkgnd;
	HBITMAP hDestDCImage = NULL;

	if( !GetSelectedDibImageFromDC(pDC, hDestDCImage) ){
		pImageSrc->Attach((HBITMAP)bmpSrc.Detach());
		return FALSE;
		}
	// ####################################################

	CRect rcTopLeftPart			(rcDestDC.left, rcDestDC.top, rcDestDC.left + rcOmitBorder.left, rcDestDC.top + rcOmitBorder.top);
	CRect rcTopCenterPart		(rcTopLeftPart.right, rcDestDC.top, rcDestDC.right - rcOmitBorder.right, rcDestDC.top + rcOmitBorder.top);
	CRect rcTopRightPart		(rcTopCenterPart.right, rcDestDC.top, rcDestDC.right, rcDestDC.top + rcOmitBorder.top);

	CRect rcBottomLeftPart		(rcDestDC.left, rcDestDC.bottom - rcOmitBorder.bottom, rcDestDC.left + rcOmitBorder.left, rcDestDC.bottom);
	CRect rcBottomCenterPart	(rcTopLeftPart.right, rcDestDC.bottom - rcOmitBorder.bottom, rcDestDC.right - rcOmitBorder.right, rcDestDC.bottom);
	CRect rcBottomRightPart		(rcTopCenterPart.right, rcDestDC.bottom - rcOmitBorder.bottom, rcDestDC.right, rcDestDC.bottom);

	CRect rcMidLeftPart			(rcDestDC.left, rcTopLeftPart.bottom, rcDestDC.left + rcOmitBorder.left, rcBottomLeftPart.top);
	CRect rcMidCenterPart		(rcTopLeftPart.right, rcTopLeftPart.bottom, rcDestDC.right - rcOmitBorder.right, rcBottomLeftPart.top);
	CRect rcMidRightPart		(rcTopCenterPart.right, rcTopLeftPart.bottom, rcDestDC.right, rcBottomLeftPart.top);

	CRect rcTopLeftPartImg		(0, 0, rcOmitBorder.left, rcOmitBorder.top);
	CRect rcTopCenterPartImg	(rcTopLeftPartImg.right, 0, nSrcCX - rcOmitBorder.right, rcOmitBorder.top);
	CRect rcTopRightPartImg		(rcTopCenterPartImg.right, 0, nSrcCX, rcOmitBorder.top);

	CRect rcBottomLeftPartImg	(0, nSrcCY - rcOmitBorder.bottom, rcOmitBorder.left, nSrcCY);
	CRect rcBottomCenterPartImg	(rcBottomLeftPartImg.right, nSrcCY - rcOmitBorder.bottom, nSrcCX - rcOmitBorder.right, nSrcCY);
	CRect rcBottomRightPartImg	(rcBottomCenterPartImg.right, nSrcCY - rcOmitBorder.bottom, nSrcCX, nSrcCY);

	CRect rcMidLeftPartImg		(0, rcTopLeftPartImg.bottom, rcTopLeftPartImg.right, rcBottomLeftPartImg.top);
	CRect rcMidCenterPartImg	(rcBottomCenterPartImg.left, rcTopLeftPartImg.bottom, rcBottomCenterPartImg.right, rcBottomLeftPartImg.top);
	CRect rcMidRightPartImg		(rcBottomRightPartImg.left, rcTopLeftPartImg.bottom, rcBottomRightPartImg.right, rcBottomLeftPartImg.top);

	CRect* pArrRects[]		= {	&rcTopLeftPart, &rcTopCenterPart, &rcTopRightPart, 
								&rcBottomLeftPart, &rcBottomCenterPart, &rcBottomRightPart,
								&rcMidLeftPart, &rcMidCenterPart, &rcMidRightPart};

	CRect* pArrRectImgs[]	= {	&rcTopLeftPartImg, &rcTopCenterPartImg, &rcTopRightPartImg, 
								&rcBottomLeftPartImg, &rcBottomCenterPartImg, &rcBottomRightPartImg,
								&rcMidLeftPartImg, &rcMidCenterPartImg, &rcMidRightPartImg};

	int nLoop	= 0;
	int nCt		= sizeof(pArrRects) / sizeof(CRect*);

	if( bSkipMiddleCenterPart ){
		pArrRects[7]	= pArrRects[8];
		pArrRectImgs[7] = pArrRectImgs[8];
		nCt --;
		}

	while( nLoop < nCt ){
		CRect* pRect	= pArrRects[nLoop];
		CRect* pRectImg = pArrRectImgs[nLoop];
		if( !pRect->IsRectEmpty() && !pRectImg->IsRectEmpty() ){
			CRect rcInvalid = *pRect&rcClipDC;
			if( !rcInvalid.IsRectEmpty() ){
				int xSrc		= pRectImg->left + (rcInvalid.left - pRect->left)%pRectImg->Width();
				int ySrc		= pRectImg->top + (rcInvalid.top - pRect->top)%pRectImg->Height();
				int	xDest		= max(rcInvalid.left, 0);
				int	yDest		= max(rcInvalid.top, 0);

				while( yDest < rcInvalid.bottom ){
					while( xDest < rcInvalid.right ){
						BitmapOnBitmap(hDestDCImage, xDest, yDest, bmpSrc, xSrc, ySrc, 
									min(pRectImg->Width(), rcInvalid.Width()), 
									min(pRectImg->Height(), rcInvalid.Height()), 
									min((rcInvalid.right - xDest), (pRectImg->right - xSrc)), 
									min((rcInvalid.bottom - yDest), (pRectImg->bottom - ySrc)), bAlpha);

						xDest	+= min((rcInvalid.right - xDest), (pRectImg->right - xSrc));
						xSrc	= pRectImg->left;
						}
					xDest	= max(rcInvalid.left, 0);
					yDest	+= min((rcInvalid.bottom - yDest), (pRectImg->bottom - ySrc));
					xSrc	= pRectImg->left + (rcInvalid.left - pRect->left)%pRectImg->Width();
					ySrc	= pRectImg->top;
					}
				}
			}

		nLoop ++;
		}

	BOOL bRet = TRUE;
	pImageSrc->Attach((HBITMAP)bmpSrc.Detach());
	bmBkgnd.DeleteObject();
	return (bRet == TRUE);
	}

BOOL
ImageHelper::Rotate90Bitmap32bpp(HBITMAP hBmpDest, CImage& imageRotated){
	BITMAP bmp;
	GetObject(hBmpDest, sizeof(BITMAP), &bmp);
	if( bmp.bmBits == NULL || bmp.bmBitsPixel != 32 )
		return FALSE;

	CBitmap bmRotated;
	BITMAP bmpRotated;
	if( !CreateDIBBitmap(bmp.bmBitsPixel, bmRotated, RGB(0, 0, 0), bmp.bmHeight, bmp.bmWidth, &bmpRotated) )
		return FALSE;

	RGBQUAD* pixelRotated	= (RGBQUAD*)bmpRotated.bmBits;
	RGBQUAD* pixelSrc		= (RGBQUAD*)bmp.bmBits;

	for( int y=0; y<bmpRotated.bmHeight; y++ ){
		for( int x=0; x<bmpRotated.bmWidth; x++ ){
			pixelSrc = (RGBQUAD*)(((long)bmp.bmBits) + (x)*bmp.bmWidthBytes + y*sizeof(RGBQUAD));

			pixelRotated->rgbBlue		= pixelSrc->rgbBlue;
			pixelRotated->rgbGreen		= pixelSrc->rgbGreen;
			pixelRotated->rgbRed		= pixelSrc->rgbRed;
			pixelRotated->rgbReserved	= pixelSrc->rgbReserved;

			pixelRotated ++;
			}
		}
	imageRotated.Attach((HBITMAP)bmRotated.Detach());
	return TRUE;
	}

HRGN
ImageHelper::GetBitmap32bppRgnByAlphaChannel(HBITMAP hbmImage, BYTE btMinAlpha /*= 255*/){
	BITMAP bmp;
	GetObject(hbmImage, sizeof(BITMAP), &bmp);
	if( GetObject(hbmImage, sizeof(BITMAP), &bmp) != sizeof(BITMAP) || !bmp.bmBits )
		return NULL;

	HRGN		hRgnBmp = NULL; //Handle To The Region
	HRGN		hOr     = NULL; 
	
#define MAX_RECT 200			
	HGLOBAL hGlobal		= GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATA)+MAX_RECT*sizeof(RECT));
	RGNDATA * pData		= (RGNDATA*)GlobalLock(hGlobal);
	pData->rdh.iType	= RDH_RECTANGLES;
	pData->rdh.dwSize	= sizeof(RGNDATAHEADER);
	SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
	pData->rdh.nRgnSize = MAX_RECT*sizeof(RECT);
	pData->rdh.nCount	= 0;

	BYTE * p = (BYTE*)bmp.bmBits + (bmp.bmHeight-1)*bmp.bmWidthBytes;
	for(int y = 0; y < bmp.bmHeight; y ++){
		for(int x = 0; x < bmp.bmWidth; x ++){
			RGBQUAD *	pPixel	= &((RGBQUAD*)(p))[x];
			int			x1		= x;
			while( x < bmp.bmWidth ){
				if( pPixel->rgbReserved >= btMinAlpha )
					break;
				x	++;
				pPixel ++;
				}
			if( x > x1 ){
				int nRcCount = pData->rdh.nRgnSize / sizeof(RECT);
				int nCount   = pData->rdh.nCount;
				if( nCount >= nRcCount ){
					pData->rdh.nRgnSize += MAX_RECT*sizeof(RECT);
					GlobalUnlock(hGlobal);
					hGlobal				= GlobalReAlloc(hGlobal,sizeof(RGNDATA)+pData->rdh.nRgnSize,GMEM_MOVEABLE);
					pData				= (RGNDATA*)GlobalLock(hGlobal);
					}

				RECT * pRect = (RECT*)&pData->Buffer;
				SetRect(&pRect[pData->rdh.nCount],x1,y,x,y+1);
				pData->rdh.nCount ++;
						
				if( x1 < pData->rdh.rcBound.left )
					pData->rdh.rcBound.left = x1;
				if( y < pData->rdh.rcBound.top )
					pData->rdh.rcBound.top = y;
				if( x > pData->rdh.rcBound.right )
					pData->rdh.rcBound.right = x;
				if( y+1 > pData->rdh.rcBound.bottom )
					pData->rdh.rcBound.bottom = y+1;
				}
			}
		p -= bmp.bmWidthBytes;
		}

	hRgnBmp = ExtCreateRegion(NULL, sizeof(RGNDATA)+pData->rdh.nRgnSize, pData);
	GlobalFree(hGlobal);

	// Invert region. {{
	HRGN hRgnDest	= CreateRectRgn(0, 0, bmp.bmWidth, bmp.bmHeight);
	if( CombineRgn(hRgnDest, hRgnDest, hRgnBmp, RGN_DIFF) == ERROR ){
		DeleteObject(hRgnDest);
		hRgnDest = 0;
		}
	DeleteObject(hRgnBmp);
	// }}
	return hRgnDest;
	}

bool
ImageHelper::MultiplyBlendImageBpp32(HBITMAP hImage, COLORREF crBlend){
	BITMAP bmImage;
	if( GetObject(hImage, sizeof(BITMAP), &bmImage) != sizeof(BITMAP) )
		return false;

	if( !bmImage.bmBits )
		return false;

	RGBQUAD	 rgbQuadBlend = 
		{
		((BYTE)(crBlend >> 16)),	// Blue
		((BYTE)(crBlend >> 8)),		// Green
		((BYTE)(crBlend)),			// Red
		0							// Alpha
		};

	RGBQUAD* pixelSrc	= (RGBQUAD*)bmImage.bmBits;
	for( int y=0; y<bmImage.bmHeight; y++ ){
		pixelSrc = (RGBQUAD*)(((long)bmImage.bmBits) + y*bmImage.bmWidthBytes);
		for( int x=0; x<bmImage.bmWidth; x++ ){
			if( pixelSrc->rgbReserved != 0 ){
				pixelSrc->rgbBlue = pixelSrc->rgbBlue*rgbQuadBlend.rgbBlue / 255;
				pixelSrc->rgbGreen = pixelSrc->rgbGreen*rgbQuadBlend.rgbGreen / 255;
				pixelSrc->rgbRed = pixelSrc->rgbRed*rgbQuadBlend.rgbRed / 255;
				
				/*
				pixelSrc->rgbBlue = pixelSrc->rgbBlue + rgbQuadBlend.rgbBlue -  pixelSrc->rgbBlue*rgbQuadBlend.rgbBlue / 255;
				pixelSrc->rgbGreen = pixelSrc->rgbGreen + rgbQuadBlend.rgbGreen - pixelSrc->rgbGreen*rgbQuadBlend.rgbGreen / 255;
				pixelSrc->rgbRed = pixelSrc->rgbRed + rgbQuadBlend.rgbRed - pixelSrc->rgbRed*rgbQuadBlend.rgbRed / 255;
*/
				}

			pixelSrc ++;
			}
		}

	return true;
	}

/*
////////////////////////////////////////////////////////////////////////////////
/** 
 * generates a 1-D convolution matrix to be used for each pass of 
 * a two-pass gaussian blur.  Returns the length of the matrix.
 * \author [nipper]
 */
int 
ImageHelper::gen_convolve_matrix (float radius, float **cmatrix_p){
	int matrix_length;
	int matrix_midpoint;
	float* cmatrix;
	int i,j;
	float std_dev;
	float sum;
	
	/* we want to generate a matrix that goes out a certain radius
	* from the center, so we have to go out ceil(rad-0.5) pixels,
	* inlcuding the center pixel.  Of course, that's only in one direction,
	* so we have to go the same amount in the other direction, but not count
	* the center pixel again.  So we double the previous result and subtract
	* one.
	* The radius parameter that is passed to this function is used as
	* the standard deviation, and the radius of effect is the
	* standard deviation * 2.  It's a little confusing.
	* <DP> modified scaling, so that matrix_lenght = 1+2*radius parameter
	*/
	radius = (float)fabs(0.5*radius) + 0.25f;
	
	std_dev = radius;
	radius = std_dev * 2;
	
	/* go out 'radius' in each direction */
	matrix_length = int (2 * ceil(radius-0.5) + 1);
	if (matrix_length <= 0) matrix_length = 1;
	matrix_midpoint = matrix_length/2 + 1;
	*cmatrix_p = new float[matrix_length];
	cmatrix = *cmatrix_p;
	
	/*  Now we fill the matrix by doing a numeric integration approximation
	* from -2*std_dev to 2*std_dev, sampling 50 points per pixel.
	* We do the bottom half, mirror it to the top half, then compute the
	* center point.  Otherwise asymmetric quantization errors will occur.
	*  The formula to integrate is e^-(x^2/2s^2).
	*/
	
	/* first we do the top (right) half of matrix */
	for (i = matrix_length/2 + 1; i < matrix_length; i++){
		float base_x = i - (float)floor((float)(matrix_length/2)) - 0.5f;
		sum = 0;
		for (j = 1; j <= 50; j++){
			if ( base_x+0.02*j <= radius ) 
				sum += (float)exp (-(base_x+0.02*j)*(base_x+0.02*j) / 
				(2*std_dev*std_dev));
		}
		cmatrix[i] = sum/50;
    }
	
	/* mirror the thing to the bottom half */
	for (i=0; i<=matrix_length/2; i++) {
		cmatrix[i] = cmatrix[matrix_length-1-i];
	}
	
	/* find center val -- calculate an odd number of quanta to make it symmetric,
	* even if the center point is weighted slightly higher than others. */
	sum = 0;
	for (j=0; j<=50; j++){
		sum += (float)exp (-(0.5+0.02*j)*(0.5+0.02*j) /
			(2*std_dev*std_dev));
    }
	cmatrix[matrix_length/2] = sum/51;
	
	/* normalize the distribution by scaling the total sum to one */
	sum=0;
	for (i=0; i<matrix_length; i++) sum += cmatrix[i];
	for (i=0; i<matrix_length; i++) cmatrix[i] = cmatrix[i] / sum;
	return matrix_length;
	}
////////////////////////////////////////////////////////////////////////////////
/**
 * generates a lookup table for every possible product of 0-255 and
 * each value in the convolution matrix.  The returned array is
 * indexed first by matrix position, then by input multiplicand (?)
 * value.
 * \author [nipper]
 */
float* ImageHelper::gen_lookup_table (float *cmatrix, int cmatrix_length){
	float* lookup_table = new float[cmatrix_length * 256];
	float* lookup_table_p = lookup_table;
	float* cmatrix_p      = cmatrix;
	
	for (int i=0; i<cmatrix_length; i++){
		for (int j=0; j<256; j++){
			*(lookup_table_p++) = *cmatrix_p * (float)j;
		}
		cmatrix_p++;
    }
	return lookup_table;
	}
////////////////////////////////////////////////////////////////////////////////
/**
 * this function is written as if it is blurring a column at a time,
 * even though it can operate on rows, too.  There is no difference
 * in the processing of the lines, at least to the blur_line function.
 * \author [nipper]
 */
void 
ImageHelper::blur_line (float *ctable, float *cmatrix, int cmatrix_length, BYTE* cur_col, BYTE* dest_col, int y, long bytes){
	float scale;
	float sum;
	int i=0, j=0;
	int row;
	int cmatrix_middle = cmatrix_length/2;
	
	float *cmatrix_p;
	BYTE  *cur_col_p;
	BYTE  *cur_col_p1;
	BYTE  *dest_col_p;
	float *ctable_p;
	
	/* this first block is the same as the non-optimized version --
	* it is only used for very small pictures, so speed isn't a
	* big concern.
	*/
	if (cmatrix_length > y){
		for (row = 0; row < y ; row++){
			scale=0;
			/* find the scale factor */
			for (j = 0; j < y ; j++){
				/* if the index is in bounds, add it to the scale counter */
				if ((j + cmatrix_middle - row >= 0) &&
					(j + cmatrix_middle - row < cmatrix_length))
					scale += cmatrix[j + cmatrix_middle - row];
				}
			for (i = 0; i<bytes; i++){
				sum = 0;
				for (j = 0; j < y; j++){
					if ((j >= row - cmatrix_middle) &&
						(j <= row + cmatrix_middle))
						sum += cur_col[j*bytes + i] * cmatrix[j];
					}
				dest_col[row*bytes + i] = (BYTE)(0.5f + sum / scale);
				}
			}
		}
	else{
		/* for the edge condition, we only use available info and scale to one */
		for (row = 0; row < cmatrix_middle; row++){
			/* find scale factor */
			scale=0;
			for (j = cmatrix_middle - row; j<cmatrix_length; j++)
				scale += cmatrix[j];
			for (i = 0; i<bytes; i++){
				sum = 0;
				for (j = cmatrix_middle - row; j<cmatrix_length; j++){
					sum += cur_col[(row + j-cmatrix_middle)*bytes + i] * cmatrix[j];
					}
				dest_col[row*bytes + i] = (BYTE)(0.5f + sum / scale);
				}
		}
		/* go through each pixel in each col */
		dest_col_p = dest_col + row*bytes;
		for (; row < y-cmatrix_middle; row++){
			cur_col_p = (row - cmatrix_middle) * bytes + cur_col;
			for (i = 0; i<bytes; i++){
				sum = 0;
				cmatrix_p = cmatrix;
				cur_col_p1 = cur_col_p;
				ctable_p = ctable;
				for (j = cmatrix_length; j>0; j--){
					sum += *(ctable_p + *cur_col_p1);
					cur_col_p1 += bytes;
					ctable_p += 256;
					}
				cur_col_p++;
				*(dest_col_p++) = (BYTE)(0.5f + sum);
				}
			}
		
		/* for the edge condition , we only use available info, and scale to one */
		for (; row < y; row++){
			/* find scale factor */
			scale=0;
			for (j = 0; j< y-row + cmatrix_middle; j++)
				scale += cmatrix[j];
			for (i = 0; i<bytes; i++){
				sum = 0;
				for (j = 0; j<y-row + cmatrix_middle; j++){
					sum += cur_col[(row + j-cmatrix_middle)*bytes + i] * cmatrix[j];
					}
				dest_col[row*bytes + i] = (BYTE) (0.5f + sum / scale);
				}
			}
		}
	}

bool
ImageHelper::Image_SetCol(BITMAP bmDest, int x, BYTE* src_col){
	if( bmDest.bmWidth <= x || x < 0 || !bmDest.bmBits )
		return false;
	if( bmDest.bmBitsPixel != 32 ) 
		return false;

	BYTE*		lpPixels	= (BYTE*)bmDest.bmBits;
	RGBQUAD*	pPixelDest	= NULL;
	int			nIndexDest	= 0;
	int			nLoop		= 0;
	int			bypp		= (bmDest.bmBitsPixel >> 3);
	for(int y=0; y<bmDest.bmHeight; y++){
		nIndexDest = x + ((bmDest.bmHeight - y - 1)*bmDest.bmWidth);
		pPixelDest = &((RGBQUAD*)lpPixels)[nIndexDest];

		pPixelDest->rgbBlue		= src_col[nLoop];
		pPixelDest->rgbGreen	= src_col[++nLoop];
		pPixelDest->rgbRed		= src_col[++nLoop];
		pPixelDest->rgbReserved = src_col[++nLoop];
		}

	return true;
	}

bool
ImageHelper::Image_GetCol(BITMAP bmDest, int x, BYTE* src_col){
	if( bmDest.bmWidth <= x || x < 0 || !bmDest.bmBits )
		return false;
	if( bmDest.bmBitsPixel != 32 ) 
		return false;

	BYTE*		lpPixels	= (BYTE*)bmDest.bmBits;
	RGBQUAD*	pPixelDest	= NULL;
	int			nIndexDest	= 0;
	int			nLoop		= 0;
	int			bypp		= (bmDest.bmBitsPixel >> 3);
	for(int y=0; y<bmDest.bmHeight; y++){
		nIndexDest = x + ((bmDest.bmHeight - y - 1)*bmDest.bmWidth);
		pPixelDest = (RGBQUAD*)&lpPixels[nIndexDest];

		src_col[nLoop] = pPixelDest->rgbBlue;
		src_col[++nLoop] = pPixelDest->rgbGreen;
		src_col[++nLoop] = pPixelDest->rgbRed;
		src_col[++nLoop] = pPixelDest->rgbReserved;
		}
	return true;
	}

bool
ImageHelper::Image_SetRow(BITMAP bmDest, int y, BYTE* src_row){
	if( bmDest.bmHeight <= y || y < 0 || !bmDest.bmBits )
		return false;
	if( bmDest.bmBitsPixel != 32 ) 
		return false;

	BYTE*		lpPixels	= (BYTE*)bmDest.bmBits;
	RGBQUAD*	pPixelDest	= NULL;
	int			nIndexDest	= 0;
	int			nLoop		= 0;
	int			bypp		= (bmDest.bmBitsPixel >> 3);
	for(int x=0; x<bmDest.bmWidth; x++){
		nIndexDest = x + ((bmDest.bmHeight - y - 1)*bmDest.bmWidth);
		pPixelDest = &((RGBQUAD*)lpPixels)[nIndexDest];

		pPixelDest->rgbBlue		= src_row[nLoop];
		pPixelDest->rgbGreen	= src_row[++nLoop];
		pPixelDest->rgbRed		= src_row[++nLoop];
		pPixelDest->rgbReserved = src_row[++nLoop];
		}
	return true;
	}

bool
ImageHelper::Image_GetRow(BITMAP bmDest, int y, BYTE* src_row){
	if( bmDest.bmHeight <= y || y < 0 || !bmDest.bmBits )
		return false;
	if( bmDest.bmBitsPixel != 32 ) 
		return false;

	BYTE*		lpPixels	= (BYTE*)bmDest.bmBits;
	RGBQUAD*	pPixelDest	= NULL;
	int			nIndexDest	= 0;
	int			nLoop		= 0;
	int			bypp		= (bmDest.bmBitsPixel >> 3);
	for(int x=0; x<bmDest.bmWidth; x++){
		nIndexDest = x + ((bmDest.bmHeight - y - 1)*bmDest.bmWidth);
		pPixelDest = &((RGBQUAD*)lpPixels)[nIndexDest];

		src_row[nLoop]		= pPixelDest->rgbBlue;
		src_row[++nLoop]	= pPixelDest->rgbGreen;
		src_row[++nLoop]	= pPixelDest->rgbRed;
		src_row[++nLoop]	= pPixelDest->rgbReserved;
		}
	return true;
	}

bool
ImageHelper::GetPixel(BITMAP bmImage, int x, int y, RGBQUAD& c){
	if( !bmImage.bmBits || x >= bmImage.bmWidth || y >= bmImage.bmHeight || x < 0 || y < 0)
		return false;
	int			nIndexDest = x + ((bmImage.bmHeight - y - 1)*bmImage.bmWidth);
	RGBQUAD*	pPixelDest = &((RGBQUAD*)bmImage.bmBits)[nIndexDest];
	/*
	c.rgbBlue	= pPixelDest->rgbBlue;
	c.rgbGreen	= pPixelDest->rgbGreen;
	c.rgbRed	= pPixelDest->rgbRed;
	c.rgbReserved = pPixelDest->rgbReserved;*/
	memcpy(&c, pPixelDest, sizeof(RGBQUAD));
	return true;
	}

bool
ImageHelper::SetPixel(BITMAP bmImage, int x, int y, RGBQUAD& c){
	if( !bmImage.bmBits || x >= bmImage.bmWidth || y >= bmImage.bmHeight || x < 0 || y < 0)
		return false;
	int			nIndexDest = x + ((bmImage.bmHeight - y - 1)*bmImage.bmWidth);
	RGBQUAD*	pPixelDest = &((RGBQUAD*)bmImage.bmBits)[nIndexDest];
	memcpy(pPixelDest, &c, sizeof(RGBQUAD));
	return true;
	}

bool 
ImageHelper::SetSoftenFilter(HBITMAP hbm){
	BITMAP bmSoften;
	if( ::GetObject(hbm, sizeof(BITMAP), &bmSoften) != sizeof(BITMAP) || !bmSoften.bmBits )
		return false;

	CBitmap bmpTest;
	if( !ImageHelper::CreateDIBBitmap(bmSoften.bmBitsPixel, bmpTest, (UINT*)bmSoften.bmBits, bmSoften.bmWidth, bmSoften.bmHeight) )
		return false;

	BITMAP bmTemp;
	if( ::GetObject((HBITMAP)bmpTest.m_hObject, sizeof(BITMAP), &bmTemp) != sizeof(BITMAP) || !bmTemp.bmBits )
		return false;

	// Basic parameters. {{
	long Ksize		= 3;
	long Kfactor	= 16;
	long Koffset	= 0;
	long kernel[]	={1,1,1,1,8,1,1,1,1};
	// }}

	long k2 = Ksize/2;
	long kmax= Ksize-k2;
	long r,g,b,i;
	long ksumcur,ksumtot;
	RGBQUAD c = {0, 0, 0, 0};

	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = bmSoften.bmWidth; ymax=bmSoften.bmHeight;

	ksumtot = 0;
	for(long j=-k2;j<kmax;j++){
		for(long k=-k2;k<kmax;k++){
			ksumtot += kernel[(j+k2)+Ksize*(k+k2)];
			}
		}

	int nWidth = bmSoften.bmWidth;
	int nHeight = bmSoften.bmHeight;

	for(long y=ymin; y<ymax; y++){
		for(long x=xmin; x<xmax; x++){
			r=b=g=ksumcur=0;
			for(long j=-k2;j<kmax;j++){
				for(long k=-k2;k<kmax;k++){
					// Is not inside.
					if ( !(0<=(y+k) && (y+k)<nHeight && 0<=(x+k) && (x+k)<nWidth) ) continue;
					if( GetPixel(bmSoften, x+j,y+k, c) ){
						i = kernel[(j+k2)+Ksize*(k+k2)];
						r += c.rgbRed * i;
						g += c.rgbGreen * i;
						b += c.rgbBlue * i;
						ksumcur += i;
						}
					}
				}

			if (Kfactor==0 || ksumcur==0){
				c.rgbRed   = (BYTE)min(255, max(0,(int)(r + Koffset)));
				c.rgbGreen = (BYTE)min(255, max(0,(int)(g + Koffset)));
				c.rgbBlue  = (BYTE)min(255, max(0,(int)(b + Koffset)));
			} else if (ksumtot == ksumcur) {
				c.rgbRed   = (BYTE)min(255, max(0,(int)(r/Kfactor + Koffset)));
				c.rgbGreen = (BYTE)min(255, max(0,(int)(g/Kfactor + Koffset)));
				c.rgbBlue  = (BYTE)min(255, max(0,(int)(b/Kfactor + Koffset)));
			} else {
				c.rgbRed   = (BYTE)min(255, max(0,(int)((r*ksumtot)/(ksumcur*Kfactor) + Koffset)));
				c.rgbGreen = (BYTE)min(255, max(0,(int)((g*ksumtot)/(ksumcur*Kfactor) + Koffset)));
				c.rgbBlue  = (BYTE)min(255, max(0,(int)((b*ksumtot)/(ksumcur*Kfactor) + Koffset)));
				}
			SetPixel(bmTemp, x, y, c);
			}
		}

	int nPixelDataSize = bmTemp.bmWidthBytes*bmTemp.bmHeight;
	memcpy(bmSoften.bmBits, bmTemp.bmBits, nPixelDataSize);
	bmpTest.DeleteObject();
	return true;
	}

void
ImageHelper::DrawText(CDC* pDC, CFont* pFont, int x, int y, int flag, CRect rcClipRect, TCHAR* pszText, float fStretchCX, float fStretchCY){
	if( !pDC || !pFont ) return;

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	lf.lfWidth	= (lf.lfWidth * fStretchCX);
	lf.lfHeight = (lf.lfHeight * fStretchCY);

	CFont fontNew;
	if( fontNew.CreateFontIndirect(&lf) ){
		CFont* pFontOld = pDC->SelectObject(&fontNew);
		ImageHelper::ExtTextOut(pDC, x, y, flag, rcClipRect, pszText);
		pDC->SelectObject(pFontOld);
		fontNew.DeleteObject();
		}
	else{
		CFont* pFontOld = pDC->SelectObject(pFont);
		ImageHelper::ExtTextOut(pDC, x, y, flag, rcClipRect, pszText);
		pDC->SelectObject(pFontOld);
		}
	}

bool
ImageHelper::StretchedTextFont(CFont* pFontIn, CFont* pFontOut, float fStretchCX, float fStretchCY, bool bUnderLine /*= false*/, bool bBold /*= false*/){
	if( !pFontIn || !pFontOut ) return false;

	LOGFONT lf;
	if( !pFontIn->m_hObject || !pFontIn->GetLogFont(&lf) )
		return false;
/*
	if( fStretchCX < 0.8f )
		fStretchCX = 0.08f;
*/
	float fMin = 0.5f, fMax = 1.8f;
	if( fStretchCY < fMin )
		fStretchCY = fMin;

	if( fStretchCY > fMax )
		fStretchCY = fMax;

	if( fStretchCX > fMax )
		fStretchCX = fMax;

	lf.lfWidth = (lf.lfWidth * fStretchCX);
	lf.lfHeight = (lf.lfHeight * fStretchCY);

	if( bUnderLine )
		lf.lfUnderline = 1;
//	else
//		lf.lfUnderline = 0;

	if( bBold )
		lf.lfWeight = FW_BOLD;

	CFont fontNew;
	if( pFontOut->CreateFontIndirect(&lf) )
		return true;
	return false;
	}

#include "EmbedeedFontMan.h"

bool
ImageHelper::ExtTextOut(CDC* pDC, int x, int y, UINT nOptions, const RECT* lpRect, CString sText){
	if( !pDC || sText.GetLength() <= 0 ) 
		return false;

	// Invalid rect area.
	if( lpRect && (lpRect->right <= 0 || lpRect->bottom <= 0) )
		return false;

	wchar_t	*	pwszTemp	= GetTemporaryUnicodeArray();
	int			nLen		= ImageHelper::UTF8ToUnicode(&sText, pwszTemp, 1024);
	bool		bRet		= false;

	LOGFONT lf;
	CFont* pRealFont = (CFont*)pDC->SelectObject(ImageHelper::GetFontTemp());
	pRealFont->GetLogFont(&lf);
	pDC->SelectObject(pRealFont);

	EmbedeedFontMan* pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	if( nFontId == -1 )
		bRet = (TRUE == ::ExtTextOutW(pDC->m_hDC, x, y, nOptions, lpRect, pwszTemp, nLen, NULL));
	else
		bRet = pEmbedeedFontMan->DrawTextByEmbedeedFont(nFontId, pDC, pwszTemp, nLen, x, y, lpRect);
	return	bRet;
	}

bool
ImageHelper::ExtTextOutW(CDC* pDC, int x, int y, UINT nOptions, const RECT* lpRect, wchar_t* pwsText, int nLen){
	if( !pDC || nLen <= 0 ) 
		return false;

	// Invalid rect area.
	if( lpRect && (lpRect->right <= 0 || lpRect->bottom <= 0) )
		return false;
	
	bool	bRet = false;
	LOGFONT lf;
	CFont* pRealFont = (CFont*)pDC->SelectObject(ImageHelper::GetFontTemp());
	pRealFont->GetLogFont(&lf);
	pDC->SelectObject(pRealFont);

	EmbedeedFontMan* pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	if( nFontId == -1 )
		bRet = (TRUE == ::ExtTextOutW(pDC->m_hDC, x, y, nOptions, lpRect, pwsText, nLen, NULL));
	else
		bRet = pEmbedeedFontMan->DrawTextByEmbedeedFont(nFontId, pDC, pwsText, nLen, x, y, lpRect);
	return	bRet;
	}

int
ImageHelper::UTF8ToUnicode(CString* pString, wchar_t* pwszUnicode, int nLen){
	int		nLoop	= 0;
	int		nCt		= pString->GetLength();
	char*	pszText = pString->GetBuffer();
	int		nLoop1	= 0;

	// E18390 - ა
	// E183B0 - ჰ

	// D090		- А
	// D0AF		- Я

	// D0B0		- а
	// D0AF		- Я

	// 10D0 - a
	// 10F0 - h

	int nRet = (int)::MultiByteToWideChar(CP_UTF8, 0, pszText, -1, pwszUnicode, nLen);
	return (nRet - 1);
	/*
	while( nLoop < nCt ){
		if( (nCt - nLoop) >= 3 ){
			int nValue = ((((int)pszText[nLoop]) << 16) & 0xFF0000) | ((((int)pszText[nLoop + 1]) << 8) & 0xFF00) | (((int)pszText[nLoop + 2]) & 0xFF);
			if( nValue >= 0xE18390 && nValue <= 0xE183F0 ){
				pwszUnicode[nLoop1] = (wchar_t)(0x10D0 + (nValue - 0xE18390));
				nLoop1	++;
				nLoop	+= 3;
				continue;
				}
			}

		pwszUnicode[nLoop1] = (wchar_t)(pszText[nLoop]);
		nLoop1	++;
		nLoop	++;
		}
	return nLoop1;*/
	}

int
ImageHelper::UnicodeToUTF8(wchar_t* pwszUnicode, int nLen, CString& pString){
	BOOL	bUsedDefChar	= FALSE;
	static  char szUtf8[512];
	szUtf8[0] = '\0';
	int		nRet = WideCharToMultiByte(CP_UTF8, 0, pwszUnicode, -1, szUtf8, 512, NULL, NULL);
	if( nRet > 0 ){
		nLen	= strnlen_s(szUtf8, 511);
		pString = szUtf8;
		}
	return nRet;
	}

bool
ImageHelper::GetTextSize(CString* pStr, CFont* pFont, CSize& szText){
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(pFont);

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	wchar_t	*	pwszTemp	= GetTemporaryUnicodeArray();
	int			nLen		= UTF8ToUnicode(pStr, pwszTemp, 1024);

	EmbedeedFontMan* pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	if( nFontId == -1 )
		VERIFY(::GetTextExtentPoint32W(memDC.m_hAttribDC, pwszTemp, nLen, &szText));
	else
		pEmbedeedFontMan->GetTextSize(&memDC, pwszTemp, nLen, nFontId, szText);
	memDC.DeleteDC();
	return true;
	}

int
ImageHelper::GetSymbolIndexByXOffset(CString* pStr, CFont* pFont, int nXTextStart,  int nXSymbolAt){
	if( !pStr || !pStr->GetLength() )
		return -1;

	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(pFont);

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	wchar_t	*			pwszTemp = GetTemporaryUnicodeArray();
	int					nLen = UTF8ToUnicode(pStr, pwszTemp, 1024);

	EmbedeedFontMan*	pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int					nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	CSize				szText(0, 0);
	wchar_t				wszSymbol[2];
	int					nRet = -1;

	for(int i=0; i<nLen; i++){
		wszSymbol[0] = pwszTemp[i];
		wszSymbol[1] = 0;

		if( nFontId == -1 )
			VERIFY(::GetTextExtentPoint32W(memDC.m_hAttribDC, wszSymbol, 1, &szText));
		else{
			if( pEmbedeedFontMan->GetTextSize(&memDC, wszSymbol, 1, nFontId, szText) )
				szText.cx += EmbedeedFontMan::GetDistanceBetweenSymbols();
			}
		nXTextStart += szText.cx;

		if( nXSymbolAt <= nXTextStart ){
			if( nXSymbolAt <= (nXTextStart - szText.cx/2) )
				nRet = i;
			else
				nRet = i + 1;
			break;
			}
		}

	if( nXSymbolAt > nXTextStart )
		nRet = (nLen - 1);

	memDC.DeleteDC();
	return nRet;
	}

int
ImageHelper::GetSymbolWidthArray(CString* pStr, CFont* pFont, CDWordArray& arrSymbolWidth){
	if( !pStr || !pStr->GetLength() )
		return 0;

	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(pFont);

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	wchar_t	*			pwszTemp	= GetTemporaryUnicodeArray();
	int					nLen		= UTF8ToUnicode(pStr, pwszTemp, 1024);

	EmbedeedFontMan*	pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int					nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	CSize				szText(0, 0);
	wchar_t				wszSymbol[2];
	int					nRet = 0;

	for(int i=0; i<nLen; i++){
		wszSymbol[0] = pwszTemp[i];
		wszSymbol[1] = 0;
		if( nFontId == -1 )
			VERIFY(::GetTextExtentPoint32W(memDC.m_hAttribDC, wszSymbol, 1, &szText));
		else{
			if( pEmbedeedFontMan->GetTextSize(&memDC, wszSymbol, 1, nFontId, szText) )
				szText.cx += EmbedeedFontMan::GetDistanceBetweenSymbols();
			else
				continue;
			}
		arrSymbolWidth.Add(szText.cx);
		nRet ++;
		}
	memDC.DeleteDC();
	return nRet;
	}

bool
ImageHelper::GetTextSizeEx(CString* pStr, CDC* pDC, CSize& szText){
	LOGFONT lf;
	CFont* pRealFont = (CFont*)pDC->SelectObject(ImageHelper::GetFontTemp());
	pRealFont->GetLogFont(&lf);
	pDC->SelectObject(pRealFont);

	wchar_t	*	pwszTemp	= GetTemporaryUnicodeArray();
	int			nLen		= UTF8ToUnicode(pStr, pwszTemp, 1024);

	EmbedeedFontMan* pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	if( nFontId == -1 )
		VERIFY(::GetTextExtentPoint32W(pDC->m_hAttribDC, pwszTemp, nLen, &szText));
	else
		pEmbedeedFontMan->GetTextSize(pDC, pwszTemp, nLen, nFontId, szText);
	return true;
	}

bool
ImageHelper::GetTextSizeW(HDC hDC, wchar_t* pwszText, int nLen, CSize& szText){
	if( !hDC ) return false;

	CDC* pDC = CDC::FromHandle(hDC);
	LOGFONT lf;
	CFont* pRealFont = CFont::FromHandle((HFONT)::SelectObject( hDC, ImageHelper::GetFontTemp()->m_hObject));
	pRealFont->GetLogFont(&lf);
	::SelectObject(hDC, pRealFont->m_hObject);

	EmbedeedFontMan* pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	if( nFontId == -1 )
		VERIFY(::GetTextExtentPoint32W(pDC->m_hAttribDC, pwszText, nLen, &szText));
	else
		pEmbedeedFontMan->GetTextSize(pDC, pwszText, nLen, nFontId, szText);
	return true;
	}

bool
ImageHelper::GetTextExtentPoint32(HDC hDC, CString* pText, int nStrLen, CSize& szText){
	static wchar_t wszTemp[1024];

	TCHAR tch = ((TCHAR*)pText->GetBuffer())[nStrLen];
	((TCHAR*)pText->GetBuffer())[nStrLen] = '\0';
	CString sTemp = *pText;
	int nLen = UTF8ToUnicode(&sTemp, wszTemp, 1024);
	((TCHAR*)pText->GetBuffer())[nStrLen] = tch;

	VERIFY(::GetTextExtentPoint32W(hDC, wszTemp, nLen, &szText));
	return true;
	}

bool
ImageHelper::IsEmbedeedFontAvailable(CFont* pFont){
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	EmbedeedFontMan* pEmbedeedFontMan = EmbedeedFontMan::GetInstance();
	int nFontId = pEmbedeedFontMan->GetEmbedeedFontId(lf.lfFaceName);
	if( nFontId == -1 )
		return false;
	return true;
	}