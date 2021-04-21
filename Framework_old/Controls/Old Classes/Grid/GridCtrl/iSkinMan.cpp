/***********************************************
************************************************/

#include "stdafx.h"
#include "iSkinMan.h"

#include "Aqua.h"
using namespace Aqua;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// iSkinManager

iSkinManager::iSkinManager()
	{
	LoadSkinDefault();
	}

iSkinManager::~iSkinManager()
	{
	FreeSkinRes();
	}

iSkinManager* 
iSkinManager::SkinManager()
	{
	static iSkinManager pMan;
	return &pMan;
	}

//*****************************************************************
//	HBITMAP GetBitmap32bppFromDC(CDC* pSrc,int xSrc,int ySrc,
//								 int nWidth,int nHeight)
//*****************************************************************
HBITMAP 
iSkinManager::GetBitmap32bppFromDC(CDC* pSrc, int xSrc, int ySrc, 
								   int nWidth, int nHeight)
	{
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = 32;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = nHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = nWidth;
	
	void*   lpMap = NULL;
	HBITMAP hDib  = CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	
	memDC.SelectObject(hDib);
	memDC.BitBlt(0, 0, nWidth, nHeight, pSrc, xSrc, ySrc, SRCCOPY);
	memDC.DeleteDC();
	return hDib;
	}

//*****************************************************************
//	BOOL ConvertInBitmap32bpp(HBITMAP& hBmp)
//*****************************************************************
BOOL 
iSkinManager::ConvertInBitmap32bpp(HBITMAP& hBmp)
	{
	BOOL bRet = FALSE;
	HDC  memDC, srcDC;
	memDC = ::CreateCompatibleDC(NULL);
	srcDC = ::CreateCompatibleDC(memDC);
	SelectObject(srcDC, hBmp);
	
	BITMAP bmp;
	ZeroMemory(&bmp, sizeof(BITMAP));
	GetObject (hBmp, sizeof(BITMAP), &bmp);
	if( bmp.bmBitsPixel == 32 )
		{
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
	HBITMAP hDib  = CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL,0);
	SelectObject(memDC, hDib);
	
	GetObject(hDib, sizeof(BITMAP), &bmp);
	BitBlt(memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

	DeleteObject(hBmp);
	DeleteDC(memDC);
	DeleteDC(srcDC);

	hBmp = hDib;
	return hDib ? TRUE : FALSE;
	}

//*****************************************************************
//	void SaveMaskDataInBasicImg(HBITMAP hbmpBasic, HBITMAP hbmpMask)
//*****************************************************************
void 
iSkinManager::SaveMaskDataInBasicImg(HBITMAP hbmpBasic, HBITMAP hbmpMask)
	{
	BITMAP bmpBasic, bmpMask;
	GetObject(hbmpBasic, sizeof(BITMAP), &bmpBasic);
	GetObject(hbmpMask,  sizeof(BITMAP), &bmpMask);

	if( bmpBasic.bmBitsPixel != 32 )
		{
		ConvertInBitmap32bpp(hbmpBasic);
		GetObject(hbmpBasic, sizeof(BITMAP), &bmpBasic);
		}

	if( bmpMask.bmBitsPixel != 32 )
		{
		ConvertInBitmap32bpp(hbmpMask);
		GetObject(hbmpMask, sizeof(BITMAP), &bmpMask);
		}

	if( bmpBasic.bmWidth     != bmpMask.bmWidth  ||
		bmpBasic.bmHeight    != bmpMask.bmHeight ||
	    bmpBasic.bmBits      == NULL             || 
		bmpMask.bmBits       == NULL )
		{
		return;
		}
	
	RGBQUAD* pBitsBasic = (RGBQUAD*)bmpBasic.bmBits;
	RGBQUAD* pBitsMask  = (RGBQUAD*)bmpMask.bmBits;
	int      nLoop      = 0;

	for( int y=0; y<bmpMask.bmHeight; y++ )
		for( int x=0; x<bmpMask.bmWidth; x++ )
			{
			ASSERT( pBitsMask[nLoop].rgbBlue == pBitsMask[nLoop].rgbRed  &&
				    pBitsMask[nLoop].rgbBlue == pBitsMask[nLoop].rgbGreen );

			pBitsBasic[nLoop].rgbReserved = pBitsMask[nLoop].rgbBlue;
			nLoop ++;
			}
	}

//*****************************************************************
//	BOOL SaveBitmap32bppFile(LPCTSTR lpszFile, HBITMAP hBmp)
//*****************************************************************
BOOL 
iSkinManager::SaveBitmap32bppFile(LPCTSTR lpszFile, HBITMAP hBmp)
	{
	BOOL bRet = FALSE;
	HDC memDC,srcDC;
	memDC = ::CreateCompatibleDC(NULL);
	srcDC = ::CreateCompatibleDC(memDC);
	SelectObject(srcDC,hBmp);
	
	BITMAP bmp;
	ZeroMemory(&bmp,sizeof(BITMAP));
	GetObject (hBmp,sizeof(BITMAP),&bmp);

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
	SelectObject(memDC,hDib);
	
	GetObject(hDib,sizeof(BITMAP),&bmp);
	ASSERT(BitBlt(memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY));

	HANDLE hFile = CreateFile(lpszFile,GENERIC_READ|GENERIC_WRITE,
							  FILE_SHARE_READ|FILE_SHARE_WRITE,
							  NULL,CREATE_NEW,0L,NULL            );
	if( hFile && hDib )
		{
		BITMAPFILEHEADER bfh;
		ZeroMemory(&bfh,sizeof(BITMAPFILEHEADER));
		bfh.bfType = 0x4d42; // 'BM'
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmp.bmHeight*bmp.bmWidth*4;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		DWORD dwWritten = 0L;

		//write bitmap file header structure.
		WriteFile(hFile,&bfh,sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
		//write info header structure.
		WriteFile(hFile,&bmpInfo32,sizeof(BITMAPINFOHEADER),&dwWritten,NULL);
		//An array of RGBQUAD. The elements of the array that make up the color table. 
		WriteFile(hFile,lpMap,bfh.bfSize - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)),&dwWritten,NULL);

		bRet = TRUE;
		}

	CloseHandle (hFile);
	DeleteDC    (memDC);
	DeleteDC    (srcDC);
	DeleteObject(hDib);
	return bRet;
	}

//*****************************************************************
//	BOOL Bitmap32bppFromBitmap(HBITMAP hBmpSrc, int xSrc, 
//							   int ySrc, int nWidth, int nHeight)
//*****************************************************************
HBITMAP 
iSkinManager::Bitmap32bppFromBitmap(HBITMAP hBmpSrc, int xSrc, 
									int ySrc, int nWidth, int nHeight)
	{
	HDC memDC, srcDC;
	memDC = ::CreateCompatibleDC(NULL);
	srcDC = ::CreateCompatibleDC(memDC);
	SelectObject(srcDC, hBmpSrc);
	
	BITMAP bmp;
	GetObject(hBmpSrc, sizeof(BITMAP),&bmp);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32,0,sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = 32;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = nHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = nWidth;
	
	void*   lpMap = NULL;
	HBITMAP hDib  = CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	SelectObject(memDC, hDib);

	if( nWidth > bmp.bmWidth || nHeight > bmp.bmHeight )
		{
		DeleteObject(hDib);
		DeleteDC    (memDC);
		DeleteDC    (srcDC);
		return NULL;
		}
	
	BitBlt(memDC, 0, 0, nWidth, nHeight, srcDC, xSrc, ySrc, SRCCOPY);

	DeleteDC    (memDC);
	DeleteDC    (srcDC);
	return hDib;
	}

//*****************************************************************
//	HBITMAP CreateBitmap32bpp(UINT* pColorTable, int x, int y, int nWidth, int nHeight)
//*****************************************************************
HBITMAP 
iSkinManager::CreateBitmap32bpp(UINT* pColorTable, int nWidth, int nHeight, BITMAP* pBmp)
	{
	/*if( !::AfxIsValidAddress(pColorTable, nWidth*nHeight*sizeof(RGBQUAD)) )
		{
		if( pBmp )
			ZeroMemory(pBmp, sizeof(BITMAP));
		return NULL;
	} */

	HDC memDC = ::CreateCompatibleDC(NULL);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32,0,sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = 32;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = nHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = nWidth;
	
	UINT* lpMap;
	HBITMAP hDib  = CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, 
									(void**)&lpMap, NULL, 0);
	ASSERT( lpMap );

	int index = 0;
	while( index < nWidth*nHeight )
		{
		lpMap[index] = pColorTable[index];
		index ++;
		}

	if( pBmp )
		GetObject(hDib, sizeof(BITMAP), pBmp);

	DeleteDC(memDC);
	return hDib;
	}

//*****************************************************************
//	BOOL BitmapOnBitmap(HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc, 
//						int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
//						int nDrawWidth, int nDrawHeight, BOOL bAlpha)
//*****************************************************************
BOOL
iSkinManager::BitmapOnBitmap(HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc, 
							 int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, int nDrawWidth, 
							 int nDrawHeight, BOOL bAlpha)
	{
	// check parameters validation. ======================================
	BITMAP bmpSrc, bmpDest;

	if( sizeof(BITMAP) != GetObject(hBmpSrc, sizeof(BITMAP), &bmpSrc)   || 
		sizeof(BITMAP) != GetObject(hBmpDest, sizeof(BITMAP), &bmpDest) ||
		bmpSrc.bmBits == NULL || bmpDest.bmBits == NULL )
		return FALSE;

	ASSERT( xDest < bmpDest.bmWidth && yDest < bmpDest.bmHeight );
	ASSERT( xSrc  < bmpSrc.bmWidth  && ySrc  < bmpSrc.bmHeight  );
	// ===================================================================

	return BitmapOnBitmap(&bmpDest, xDest, yDest, &bmpSrc, 
						  xSrc, ySrc, nWidthSrc, nHeightSrc, nDrawWidth, nDrawHeight, bAlpha);

	}

//*****************************************************************
//	BOOL BitmapOnBitmap(BITMAP* pbmpDest, int xDest, int yDest, BITMAP* pbmpSrc, 
//						int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
//						int nDrawWidth, int nDrawHeight, BOOL bAlpha)
//*****************************************************************
BOOL
iSkinManager::BitmapOnBitmap(BITMAP* pbmpDest, int xDest, int yDest, BITMAP* pbmpSrc, 
							 int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
							 int nDrawWidth, int nDrawHeight, BOOL bAlpha)
	{
	nDrawWidth  = min(pbmpDest->bmWidth - xDest, nDrawWidth);
	nDrawHeight = min(pbmpDest->bmHeight - yDest, nDrawHeight);

	if( nDrawWidth  <= 0 || 
		nDrawHeight <= 0  ) return FALSE;

	int nIndexDest, nIndexSrc;
	int nGotoStartDest, nGotoStartSrc;
	int nWidth, nHeight;
	int xSrcOld = xSrc, ySrcOld = ySrc;
	int xDrawed = 0, yDrawed = 0;

	ASSERT( xDest >= 0 && xDest < pbmpDest->bmWidth );
	ASSERT( yDest >= 0 && yDest < pbmpDest->bmHeight);
	ASSERT( xSrc  >= 0 && xSrc  < pbmpSrc->bmWidth  );
	ASSERT( ySrc  >= 0 && ySrc  < pbmpSrc->bmHeight );

	ASSERT( nWidthSrc  <= pbmpSrc->bmWidth  && nWidthSrc > 0  );
	ASSERT( nHeightSrc <= pbmpSrc->bmHeight && nHeightSrc > 0 );

	if( bAlpha )
		{
		RGBQUAD* pArrSrc  = (RGBQUAD*)pbmpSrc->bmBits;
		RGBQUAD* pArrDest = (RGBQUAD*)pbmpDest->bmBits;
		BYTE     red_src,  green_src,  blue_src;
		BYTE     red_dest, green_dest, blue_dest;
		int      nAlpha;

		while( yDrawed < nDrawHeight )
			{
			while( xDrawed < nDrawWidth )
				{
				// =================================================
				nWidth         = min(nWidthSrc - xSrc, nDrawWidth - xDrawed);
				nHeight        = min(nHeightSrc - ySrc, nDrawHeight - yDrawed);
				nIndexSrc      = xSrc + ((pbmpSrc->bmHeight - ySrc - 1)*pbmpSrc->bmWidth);
				nIndexDest     = xDrawed + xDest + ((pbmpDest->bmHeight - (1 + yDrawed + yDest))*pbmpDest->bmWidth);
				nGotoStartDest = pbmpDest->bmWidth + nWidth;
				nGotoStartSrc  = pbmpSrc->bmWidth  + nWidth;
				// =================================================

				// Basic operation on bitmaps. =====================
				for( int y = 0; y < nHeight; y++ )
					{
					for( int x = 0; x < nWidth; x++ )
						{
						// Set Alpha Color. ========================
						nAlpha     = pArrSrc [nIndexSrc].rgbReserved;
						red_src    = pArrDest[nIndexDest].rgbRed;
						green_src  = pArrDest[nIndexDest].rgbGreen;
						blue_src   = pArrDest[nIndexDest].rgbBlue;
						red_dest   = pArrSrc [nIndexSrc].rgbRed;
						green_dest = pArrSrc [nIndexSrc].rgbGreen;
						blue_dest  = pArrSrc [nIndexSrc].rgbBlue;

						pArrDest[nIndexDest].rgbBlue  = static_cast<BYTE>(blue_dest* (nAlpha/255.0) + blue_src* (1.0-(nAlpha/255.0)));
						pArrDest[nIndexDest].rgbGreen = static_cast<BYTE>(green_dest*(nAlpha/255.0) + green_src*(1.0-(nAlpha/255.0)));
						pArrDest[nIndexDest].rgbRed   = static_cast<BYTE>(red_dest*  (nAlpha/255.0) + red_src*  (1.0-(nAlpha/255.0)));
						// ==========================================
						nIndexDest ++;
						nIndexSrc  ++;
						}

					nIndexDest -= nGotoStartDest;
					nIndexSrc  -= nGotoStartSrc;
					}
				// =================================================
				xDrawed += nWidth;
				xSrc     = 0;
				}

			yDrawed += nHeight;
			xDrawed  = 0;
			xSrc     = xSrcOld;
			ySrc     = 0;
			}
		}
	else
		{
		UINT* pArrSrc  = (UINT*)pbmpSrc->bmBits;
		UINT* pArrDest = (UINT*)pbmpDest->bmBits;

		while( yDrawed < nDrawHeight )
			{
			while( xDrawed < nDrawWidth )
				{
				// =================================================
				nWidth         = min(nWidthSrc, nDrawWidth - xDrawed);
				nHeight        = min(nHeightSrc, nDrawHeight - yDrawed);
				nIndexSrc      = xSrc + ((pbmpSrc->bmHeight - ySrc - 1)*pbmpSrc->bmWidth);
				nIndexDest     = xDrawed + xDest + ((pbmpDest->bmHeight - (1 + yDrawed + yDest))*pbmpDest->bmWidth);
				nGotoStartDest = pbmpDest->bmWidth + nWidth;
				nGotoStartSrc  = pbmpSrc->bmWidth  + nWidth;
				// =================================================

				// Basic operation on bitmps. =====================
				for( int y = 0; y < nHeight; y++ )
					{
					for( int x = 0; x < nWidth; x++ )
						{
						pArrDest[nIndexDest] = pArrSrc[nIndexSrc];

						nIndexDest ++;
						nIndexSrc  ++;
						}

					nIndexDest -= nGotoStartDest;
					nIndexSrc  -= nGotoStartSrc;
					}
				// =================================================
				xDrawed += nWidth;
				xSrc     = 0;
				}

			yDrawed += nHeight;
			xDrawed  = 0;
			xSrc     = xSrcOld;
			ySrc     = 0;
			}
		}

	return TRUE;
	}

//*****************************************************************
//	BOOL RegionFromAlphaData(HBITMAP hBmpAlpha, CRgn& rgnMask)
//*****************************************************************
BOOL 
iSkinManager::RegionFromAlphaData(HBITMAP hBmpAlpha, CRgn& rgnMask)
	{
	HDC  hMemDC  = CreateCompatibleDC(NULL);
	HDC	 hSrc    = CreateCompatibleDC(hMemDC);
	::SelectObject(hSrc,hBmpAlpha);

	BITMAP bmp;
	GetObject(hBmpAlpha, sizeof(BITMAP),&bmp);
	if( !bmp.bmWidth  || !bmp.bmHeight )
		return FALSE;

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount	= 32;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes		= 1;
	bmpInfo32.biHeight		= bmp.bmHeight;
	bmpInfo32.biSize		= sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth		= bmp.bmWidth;

	LPVOID	lpMap = NULL; //Pointer to the Dib Section
	HBITMAP hDib  = CreateDIBSection(hMemDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	if( hDib )
		{
		GetObject(hDib,sizeof(BITMAP),&bmp);
		SelectObject(hMemDC,hDib);
		BitBlt(hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hSrc, 0, 0, SRCCOPY);

		HGLOBAL hGlobal     = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATA) + 200*sizeof(RECT));
		RGNDATA *pData      = (RGNDATA*)GlobalLock(hGlobal);
		pData->rdh.iType    = RDH_RECTANGLES;
		pData->rdh.dwSize   = sizeof(RGNDATAHEADER);
		SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
		pData->rdh.nRgnSize = 200*sizeof(RECT);
		pData->rdh.nCount   = 0;

		BYTE *p = (BYTE*)bmp.bmBits + bmp.bmWidthBytes*(bmp.bmHeight-1);
		for(int y = 0; y < bmp.bmHeight; y ++)
			{
			for(int x = 0; x < bmp.bmWidth; x ++)
				{
				LONG* pPixel = (LONG*)p + x;
				int   x1     = x;
				while( x < bmp.bmWidth )
					{
					if( ((RGBQUAD*)pPixel)->rgbReserved == 0 )
						break;
					
					x ++; pPixel ++;
					}
				if( x > x1 )
					{
					int nRcCount = pData->rdh.nRgnSize / sizeof(RECT);
					int nCount   = pData->rdh.nCount;
					if( nCount >= nRcCount )
						{
						pData->rdh.nRgnSize += 200*sizeof(RECT);
						GlobalUnlock(hGlobal);
						hGlobal = GlobalReAlloc(hGlobal, sizeof(RGNDATA)+pData->rdh.nRgnSize, GMEM_MOVEABLE);
						pData	= (RGNDATA*)GlobalLock(hGlobal);
						}

					RECT* pRect = (RECT*)&pData->Buffer;
					SetRect(&pRect[pData->rdh.nCount], x1, y, x, y+1);
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
		rgnMask.CreateFromData(NULL, sizeof(RGNDATA)+pData->rdh.nRgnSize, pData);
		// Clean Up ...
		GlobalFree(hGlobal);
		}

	DeleteDC(hMemDC);
	DeleteDC(hSrc);
	return TRUE;	
	}

//*****************************************************************
//	void LoadSkin(CString strSkinPath)
//*****************************************************************
BOOL 
iSkinManager::LoadSkin(CString strSkinPath)
	{
	FreeSkinRes();

	CString sValue, sBmpFile, sBmpMaskFile;
	m_ini.setINIFileName(strSkinPath + _T("setup.ini"));

	// ***************************************************************
	// *** Top Header Section ****************************************
	// ***************************************************************
	if( m_ini.sectionExists(_T("TOPHEADER")) )
		{
		int nXSrc, nYSrc;
		BITMAP bmp;

		sValue             = m_ini.getKeyValue(_T("height"), _T("TOPHEADER"));
		m_nCY_TopHeader    = atoi(sValue.GetBuffer(0));
		sValue             = m_ini.getKeyValue(_T("ImgSrc"), _T("TOPHEADER"));
		sBmpFile           = strSkinPath + sValue;

		HBITMAP hBasicImg  = (HBITMAP)LoadImage(NULL, sBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject((void*)hBasicImg, sizeof(BITMAP), (void*)&bmp);

		LPCTSTR lpListKey[] = 
		{_T("X_ImageNoFocusNormal"),   _T("Y_ImageNoFocusNormal"),	   _T("X_ImageFocusNormal"), 
		_T("Y_ImageFocusNormal"),      _T("X_ImagePressedNormal"),     _T("Y_ImagePressedNormal"),        
		_T("X_ImageNoFocusSelectAsc"), _T("Y_ImageNoFocusSelectAsc"),  _T("X_ImageFocusSelectAsc"),
		_T("Y_ImageFocusSelectAsc"),   _T("X_ImagePressedSelectAsc"),  _T("Y_ImagePressedSelectAsc"),
		_T("X_ImageNoFocusSelectDesc"),_T("Y_ImageNoFocusSelectDesc"), _T("X_ImageFocusSelectDesc"),
		_T("Y_ImageFocusSelectDesc"),  _T("X_ImagePressedSelectDesc"), _T("Y_ImagePressedSelectDesc"), NULL};

		HBITMAP* hBmpList[]   = {(HBITMAP*)&m_hTH_NoFocusNormal,    (HBITMAP*)&m_hTH_FocusNormal,  
								 (HBITMAP*)&m_hTH_PressedNormal,    (HBITMAP*)&m_hTH_NoFocusSelectAsc, 
								 (HBITMAP*)&m_hTH_FocusSelectAsc,   (HBITMAP*)&m_hTH_PressedSelectAsc,
								 (HBITMAP*)&m_hTH_NoFocusSelectDesc,(HBITMAP*)&m_hTH_FocusSelectDesc,
								 (HBITMAP*)&m_hTH_PressedSelectDesc, NULL};

		BITMAP* pBmpObjList[] = {(BITMAP*)&m_bmpTH_NoFocusNormal,     (BITMAP*)&m_bmpTH_FocusNormal,
								 (BITMAP*)&m_bmpTH_PressedNormal,     (BITMAP*)&m_bmpTH_NoFocusSelectAsc,
								 (BITMAP*)&m_bmpTH_FocusSelectAsc,    (BITMAP*)&m_bmpTH_PressedSelectAsc, 
								 (BITMAP*)&m_bmpTH_NoFocusSelectDesc, (BITMAP*)&m_bmpTH_FocusSelectDesc,    
								 (BITMAP*)&m_bmpTH_PressedSelectDesc, NULL};

		int nLoop = 0;
		while( hBmpList[nLoop] )
			{
			HBITMAP* hBmp  = (HBITMAP*)hBmpList[nLoop];
			BITMAP*  pObj  = (BITMAP*)pBmpObjList[nLoop];

			sValue = m_ini.getKeyValue(lpListKey[nLoop*2],   _T("TOPHEADER"));
			nXSrc  = atoi(sValue.GetBuffer(0));
			sValue = m_ini.getKeyValue(lpListKey[nLoop*2+1], _T("TOPHEADER"));
			nYSrc  = atoi(sValue.GetBuffer(0));

			*hBmp  = Bitmap32bppFromBitmap(hBasicImg, nXSrc, nYSrc, bmp.bmWidth, m_nCY_TopHeader);
			GetObject((void*)*hBmp, sizeof(BITMAP), (void*)pObj);

			nLoop ++;
			}

		::DeleteObject(hBasicImg);
		}

	// ***************************************************************
	// *** Scroll Section ********************************************
	// ***************************************************************
	if( m_ini.sectionExists(_T("SCROLL")) )
		{
		// load image resources. =====================================================
		LPCTSTR lpListKey[] = 
					{_T("HArrowLeftNormal_img"),   _T("HArrowLeftHot_img"),		   _T("HArrowLeftPressed_img"), 
					_T("HArrowLeftDisabled_img"),  _T("HThumbLeftNormal_img"),     _T("HThumbLeftHot_img"),        
					_T("HThumbLeftPressed_img"),   _T("HThumbLeftDisabled_img"),   _T("HThumbCenterNormal_img"),
					_T("HThumbCenterHot_img"),     _T("HThumbCenterPressed_img"),  _T("HThumbCenterDisabled_img"),
					_T("HThumbRightNormal_img"),   _T("HThumbRightHot_img"),       _T("HThumbRightPressed_img"),
					_T("HThumbRightDisabled_img"), _T("HArrowRightNormal_img"),    _T("HArrowRightHot_img"),
					_T("HArrowRightPressed_img"),  _T("HArrowRightDisabled_img"),  _T("HBackgroundNormal_img"),
					_T("HBackgroundHot_img"),      _T("HBackgroundPressed_img"),   _T("HBackgroundDisabled_img"),
					_T("VArrowTopNormal_img"),     _T("VArrowTopHot_img"),		   _T("VArrowTopPressed_img"), 
					_T("VArrowTopDisabled_img"),   _T("VThumbTopNormal_img"),      _T("VThumbTopHot_img"),         
					_T("VThumbTopPressed_img"),    _T("VThumbTopDisabled_img"),    _T("VThumbCenterNormal_img"),   
					_T("VThumbCenterHot_img"),     _T("VThumbCenterPressed_img"),  _T("VThumbCenterDisabled_img"), 
					_T("VThumbBottomNormal_img"),  _T("VThumbBottomHot_img"),      _T("VThumbBottomPressed_img"),  
					_T("VThumbBottomDisabled_img"),_T("VArrowBottomNormal_img"),   _T("VArrowBottomHot_img"),
					_T("VArrowBottomPressed_img"), _T("VArrowBottomDisabled_img"), _T("VBackgroundNormal_img"),   
					_T("VBackgroundHot_img"),      _T("VBackgroundPressed_img"),   _T("VBackgroundDisabled_img"), NULL
					};

		HBITMAP* hBmpList[]   = {(HBITMAP*)&m_hArrowLeftH,  (HBITMAP*)&m_hThumbLeftH,  
								 (HBITMAP*)&m_hThumbCenterH,(HBITMAP*)&m_hThumbRightH, 
							     (HBITMAP*)&m_hArrowRightH, (HBITMAP*)&m_hBackgroundH, 
							     (HBITMAP*)&m_hArrowTopV,   (HBITMAP*)&m_hThumbTopV,
								 (HBITMAP*)&m_hThumbCenterV,(HBITMAP*)&m_hThumbBottomV,
								 (HBITMAP*)&m_hArrowBottomV,(HBITMAP*)&m_hBackgroundV, NULL};

		BITMAP* pBmpObjList[] = {(BITMAP*)&m_bmpArrowLeftH,   (BITMAP*)&m_bmpThumbLeftH,
								 (BITMAP*)&m_bmpThumbCenterH, (BITMAP*)&m_bmpThumbRightH,
								 (BITMAP*)&m_bmpArrowRightH,  (BITMAP*)&m_bmpBackgroundH, 
								 (BITMAP*)&m_bmpArrowTopV,    (BITMAP*)&m_bmpThumbTopV,    
								 (BITMAP*)&m_bmpThumbCenterV, (BITMAP*)&m_bmpThumbBottomV, 
								 (BITMAP*)&m_bmpArrowBottomV, (BITMAP*)&m_bmpBackgroundV, NULL};

		CRgn* pMaskBmpRgns[]  = {&m_rgnArrowLeft, NULL, NULL, NULL, &m_rgnArrowRight, NULL,
								 &m_rgnArrowTop, NULL, NULL, NULL, &m_rgnArrowBottom, NULL};

		int nLoop = 0;
		while( hBmpList[nLoop] )
			{
			HBITMAP* hBmp     = (HBITMAP*)hBmpList[nLoop];
			BITMAP*  pObj     = (BITMAP*)pBmpObjList[nLoop];
			HBITMAP  hBmpMask = NULL;

			sValue   = m_ini.getKeyValue(lpListKey[nLoop*4], _T("SCROLL"));
			sBmpFile = strSkinPath + sValue;
			hBmp[0]  = (HBITMAP)LoadImage(NULL, sBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			GetObject((void*)hBmp[0], sizeof(BITMAP), (void*)&pObj[0]);
			if( hBmp[0] )
				{
				// if bitmap's bpp is not 32 then convert it. =
				if( pObj[0].bmBitsPixel != 32 )
					{
					ConvertInBitmap32bpp(hBmp[0]);
					GetObject((void*)hBmp[0], sizeof(BITMAP), (void*)&pObj[0]);
					}
				// load mask image & store it in basic image. =
				if( pObj[0].bmBits )
					{
					sBmpMaskFile = sBmpFile.Left(sBmpFile.GetLength()-4) + _T("_Mask") + sBmpFile.Right(4);
					hBmpMask     = (HBITMAP)LoadImage(NULL, sBmpMaskFile, IMAGE_BITMAP, 
													  0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
					// save mask data in basic image, as reserved param,
					// in RGBQUAD structure. ============================
					SaveMaskDataInBasicImg(hBmp[0], hBmpMask);

					// create region from mask bitmap. ==================
					if( pMaskBmpRgns[nLoop] )
						{
						// if can't create mask bitmap region then create rect region. =====
						if( !RegionFromAlphaData(hBmp[0], *pMaskBmpRgns[nLoop]) )
							pMaskBmpRgns[nLoop]->CreateRectRgn(0, 0, pObj[0].bmWidth, pObj[0].bmHeight);
						// =================================================================
						}
					// ==================================================
					DeleteObject(hBmpMask);
					}
				}
			else
				ASSERT(FALSE); // normal state bitmap must have.
			// ============================================
			sValue   = m_ini.getKeyValue(lpListKey[nLoop*4+1], _T("SCROLL"));
			sBmpFile = strSkinPath + sValue;
			hBmp[1]  = (HBITMAP)LoadImage(NULL, sBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			GetObject((void*)hBmp[1], sizeof(BITMAP), (void*)&pObj[1]);
			if( hBmp[1] )
				{
				// if bitmap's bpp is not 32 then convert it. =
				if( pObj[1].bmBitsPixel != 32 )
					{
					ConvertInBitmap32bpp(hBmp[1]);
					GetObject((void*)hBmp[1], sizeof(BITMAP), (void*)&pObj[1]);
					}
				// load mask image & store it in basic image. =
				if( pObj[1].bmBits )
					{
					sBmpMaskFile = sBmpFile.Left(sBmpFile.GetLength()-4) + _T("_Mask") + sBmpFile.Right(4);
					hBmpMask     = (HBITMAP)LoadImage(NULL, sBmpMaskFile, IMAGE_BITMAP, 
													  0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
					// save mask data in basic image, as reserved param,
					// in RGBQUAD structure. ============================
					SaveMaskDataInBasicImg(hBmp[1], hBmpMask);
					DeleteObject(hBmpMask);
					}
				}
			// ============================================
			sValue   = m_ini.getKeyValue(lpListKey[nLoop*4+2], _T("SCROLL"));
			sBmpFile = strSkinPath + sValue;
			hBmp[2]  = (HBITMAP)LoadImage(NULL, sBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			GetObject((void*)hBmp[2], sizeof(BITMAP), (void*)&pObj[2]);
			if( hBmp[2] )
				{
				// if bitmap's bpp is not 32 then convert it. =
				if( pObj[2].bmBitsPixel != 32 )
					{
					ConvertInBitmap32bpp(hBmp[2]);
					GetObject((void*)hBmp[2], sizeof(BITMAP), (void*)&pObj[2]);
					}
				// load mask image & store it in basic image. =
				if( pObj[2].bmBits )
					{
					sBmpMaskFile = sBmpFile.Left(sBmpFile.GetLength()-4) + _T("_Mask") + sBmpFile.Right(4);
					hBmpMask     = (HBITMAP)LoadImage(NULL, sBmpMaskFile, IMAGE_BITMAP, 
													  0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
					// save mask data in basic image, as reserved param,
					// in RGBQUAD structure. ============================
					SaveMaskDataInBasicImg(hBmp[2], hBmpMask);
					DeleteObject(hBmpMask);
					}
				}
			// ============================================
			sValue   = m_ini.getKeyValue(lpListKey[nLoop*4+3], _T("SCROLL"));
			sBmpFile = strSkinPath + sValue;
			hBmp[3]  = (HBITMAP)LoadImage(NULL, sBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			GetObject((void*)hBmp[3], sizeof(BITMAP), (void*)&pObj[3]);
			if( hBmp[3] )
				{
				// if bitmap's bpp is not 32 then convert it. =
				if( pObj[3].bmBitsPixel != 32 )
					{
					ConvertInBitmap32bpp(hBmp[3]);
					GetObject((void*)hBmp[3], sizeof(BITMAP), (void*)&pObj[3]);
					}
				// load mask image & store it in basic image. =
				if( pObj[3].bmBits )
					{
					sBmpMaskFile = sBmpFile.Left(sBmpFile.GetLength()-4) + _T("_Mask") + sBmpFile.Right(4);
					hBmpMask     = (HBITMAP)LoadImage(NULL, sBmpMaskFile, IMAGE_BITMAP, 
													  0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
					// save mask data in basic image, as reserved param,
					// in RGBQUAD structure. ============================
					SaveMaskDataInBasicImg(hBmp[3], hBmpMask);
					DeleteObject(hBmpMask);
					}
				}
			// ============================================
			nLoop ++;
			}
		// ====================================================================================
		
		// load other Scroll params. ============================
		sValue			 = m_ini.getKeyValue(_T("ScrollCY"),   _T("SCROLL"));
		m_nCY_HScroll	 = atoi(sValue);
		sValue			 = m_ini.getKeyValue(_T("ArrowCXMin"), _T("SCROLL"));
		m_nCX_HArrowMin  = atoi(sValue);
		sValue			 = m_ini.getKeyValue(_T("ArrowCXMax"), _T("SCROLL"));
		m_nCX_HArrowMax  = atoi(sValue);
		sValue			 = m_ini.getKeyValue(_T("ScrollCX"),   _T("SCROLL"));
		m_nCX_VScroll    = atoi(sValue);
		sValue           = m_ini.getKeyValue(_T("ArrowCYMin"), _T("SCROLL"));
		m_nCY_VArrowMin  = atoi(sValue);
		sValue           = m_ini.getKeyValue(_T("ArrowCYMax"), _T("SCROLL"));
		m_nCY_VArrowMax  = atoi(sValue);

		m_nCY_ThumbMin   = m_bmpThumbBottomV[0].bmHeight + 4 + m_bmpThumbTopV[0].bmHeight;
		m_nCX_ThumbMin   = m_bmpThumbLeftH[0].bmWidth    + 4 + m_bmpThumbRightH[0].bmWidth;
		// =======================================================
		}

	return TRUE;
	}

//*****************************************************************
//	void LoadSkinDefault()
//*****************************************************************
void 
iSkinManager::LoadSkinDefault()
	{
	FreeSkinRes();

	// load horizontal scroll resources. =======================
	m_hArrowLeftH[0] = CreateBitmap32bpp((UINT*)&ct_HArrowLeftNormal,   g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[0]);
	m_hArrowLeftH[1] = CreateBitmap32bpp((UINT*)&ct_HArrowLeftHot,      g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[1]);
	m_hArrowLeftH[2] = CreateBitmap32bpp((UINT*)&ct_HArrowLeftPressed,  g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[2]);
	m_hArrowLeftH[3] = CreateBitmap32bpp((UINT*)&ct_HArrowLeftDisabled, g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[3]);

	m_hThumbLeftH[0] = CreateBitmap32bpp((UINT*)&ct_HThumbLeftNormal,   g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[0]);
	m_hThumbLeftH[1] = CreateBitmap32bpp((UINT*)&ct_HThumbLeftHot,      g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[1]);
	m_hThumbLeftH[2] = CreateBitmap32bpp((UINT*)&ct_HThumbLeftPressed,  g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[2]);
	m_hThumbLeftH[3] = CreateBitmap32bpp((UINT*)&ct_HThumbLeftDisabled, g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[3]);

	m_hThumbCenterH[0] = CreateBitmap32bpp((UINT*)&ct_HThumbCenterNormal,   g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[0]);
	m_hThumbCenterH[1] = CreateBitmap32bpp((UINT*)&ct_HThumbCenterHot,      g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[1]);
	m_hThumbCenterH[2] = CreateBitmap32bpp((UINT*)&ct_HThumbCenterPressed,  g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[2]);
	m_hThumbCenterH[3] = CreateBitmap32bpp((UINT*)&ct_HThumbCenterDisabled, g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[3]);

	m_hThumbRightH[0] = CreateBitmap32bpp((UINT*)&ct_HThumbRightNormal,   g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[0]);
	m_hThumbRightH[1] = CreateBitmap32bpp((UINT*)&ct_HThumbRightHot,      g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[1]);
	m_hThumbRightH[2] = CreateBitmap32bpp((UINT*)&ct_HThumbRightPressed,  g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[2]);
	m_hThumbRightH[3] = CreateBitmap32bpp((UINT*)&ct_HThumbRightDisabled, g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[3]);

	m_hArrowRightH[0] = CreateBitmap32bpp((UINT*)&ct_HArrowRightNormal,   g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[0]);
	m_hArrowRightH[1] = CreateBitmap32bpp((UINT*)&ct_HArrowRightHot,      g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[1]);
	m_hArrowRightH[2] = CreateBitmap32bpp((UINT*)&ct_HArrowRightPressed,  g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[2]);
	m_hArrowRightH[3] = CreateBitmap32bpp((UINT*)&ct_HArrowRightDisabled, g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[3]);
	
	m_hBackgroundH[0] = CreateBitmap32bpp((UINT*)&ct_HBkgndNormal,   g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[0]);
	m_hBackgroundH[1] = CreateBitmap32bpp((UINT*)&ct_HBkgndHot,      g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[1]);
	m_hBackgroundH[2] = CreateBitmap32bpp((UINT*)&ct_HBkgndPressed,  g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[2]);
	m_hBackgroundH[3] = CreateBitmap32bpp((UINT*)&ct_HBkgndDisabled, g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[3]);
	//=================================================================

	// load vertical scroll resources. ================================
	m_hArrowTopV[0] = CreateBitmap32bpp((UINT*)&ct_VArrowTopNormal,   g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[0]);
	m_hArrowTopV[1] = CreateBitmap32bpp((UINT*)&ct_VArrowTopHot,      g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[1]);
	m_hArrowTopV[2] = CreateBitmap32bpp((UINT*)&ct_VArrowTopPressed,  g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[2]);
	m_hArrowTopV[3] = CreateBitmap32bpp((UINT*)&ct_VArrowTopDisabled, g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[3]);

	m_hThumbTopV[0] = CreateBitmap32bpp((UINT*)&ct_VThumbTopNormal,   g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[0]);
	m_hThumbTopV[1] = CreateBitmap32bpp((UINT*)&ct_VThumbTopHot,      g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[1]);
	m_hThumbTopV[2] = CreateBitmap32bpp((UINT*)&ct_VThumbTopPressed,  g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[2]);
	m_hThumbTopV[3] = CreateBitmap32bpp((UINT*)&ct_VThumbTopDisabled, g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[3]);

	m_hThumbCenterV[0] = CreateBitmap32bpp((UINT*)&ct_VThumbCenterNormal,   g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[0]);
	m_hThumbCenterV[1] = CreateBitmap32bpp((UINT*)&ct_VThumbCenterHot,      g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[1]);
	m_hThumbCenterV[2] = CreateBitmap32bpp((UINT*)&ct_VThumbCenterPressed,  g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[2]);
	m_hThumbCenterV[3] = CreateBitmap32bpp((UINT*)&ct_VThumbCenterDisabled, g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[3]);

	m_hThumbBottomV[0] = CreateBitmap32bpp((UINT*)&ct_VThumbBottomNormal,   g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[0]);
	m_hThumbBottomV[1] = CreateBitmap32bpp((UINT*)&ct_VThumbBottomHot,      g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[1]);
	m_hThumbBottomV[2] = CreateBitmap32bpp((UINT*)&ct_VThumbBottomPressed,  g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[2]);
	m_hThumbBottomV[3] = CreateBitmap32bpp((UINT*)&ct_VThumbBottomDisabled, g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[3]);

	m_hArrowBottomV[0] = CreateBitmap32bpp((UINT*)&ct_VArrowBottomNormal,   g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[0]);
	m_hArrowBottomV[1] = CreateBitmap32bpp((UINT*)&ct_VArrowBottomHot,      g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[1]);
	m_hArrowBottomV[2] = CreateBitmap32bpp((UINT*)&ct_VArrowBottomPressed,  g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[2]);
	m_hArrowBottomV[3] = CreateBitmap32bpp((UINT*)&ct_VArrowBottomDisabled, g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[3]);
	
	m_hBackgroundV[0] = CreateBitmap32bpp((UINT*)&ct_VBkgndNormal,   g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[0]);
	m_hBackgroundV[1] = CreateBitmap32bpp((UINT*)&ct_VBkgndHot,      g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[1]);
	m_hBackgroundV[2] = CreateBitmap32bpp((UINT*)&ct_VBkgndPressed,  g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[2]);
	m_hBackgroundV[3] = CreateBitmap32bpp((UINT*)&ct_VBkgndDisabled, g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[3]);
	//=================================================================

	// create arrow regions. ==================
	if( !RegionFromAlphaData(m_hArrowLeftH[0], m_rgnArrowLeft) )
		m_rgnArrowLeft.CreateRectRgn(0, 0, m_bmpArrowLeftH[0].bmWidth, m_bmpArrowLeftH[0].bmHeight);

	if( !RegionFromAlphaData(m_hArrowRightH[0], m_rgnArrowRight) )
		m_rgnArrowRight.CreateRectRgn(0, 0, m_bmpArrowRightH[0].bmWidth, m_bmpArrowRightH[0].bmHeight);

	if( !RegionFromAlphaData(m_hArrowTopV[0], m_rgnArrowTop) )
		m_rgnArrowTop.CreateRectRgn(0, 0, m_bmpArrowTopV[0].bmWidth, m_bmpArrowTopV[0].bmHeight);

	if( !RegionFromAlphaData(m_hArrowBottomV[0], m_rgnArrowBottom) )
		m_rgnArrowBottom.CreateRectRgn(0, 0, m_bmpArrowBottomV[0].bmWidth, m_bmpArrowBottomV[0].bmHeight);
	// ========================================

	m_nCY_HScroll	= g_nHScrollCY;
	m_nCX_HArrowMin = g_nHArrowCXMin;
	m_nCX_HArrowMax = g_nHArrowCXMax;
	m_nCX_VScroll   = g_nVScrollCX;
	m_nCY_VArrowMin = g_nVArrowCYMin;
	m_nCY_VArrowMax = g_nVArrowCYMax;

	m_nCY_ThumbMin  = m_bmpThumbBottomV[0].bmHeight + 4 + m_bmpThumbTopV[0].bmHeight;
	m_nCX_ThumbMin  = m_bmpThumbLeftH[0].bmWidth    + 4 + m_bmpThumbRightH[0].bmWidth;

	// Load Top Header Resources. ================================
	HBITMAP hbmpTopHdr = CreateBitmap32bpp((UINT*)&ct_TopHeader, g_szTopHeaderImg.cx, g_szTopHeaderImg.cy, NULL);
	
	CRect rcClipImage = g_rcTH_NoFocusNormal;
	m_hTH_NoFocusNormal = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                        rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_NoFocusNormal, sizeof(BITMAP), &m_bmpTH_NoFocusNormal);

	rcClipImage = g_rcTH_FocusNormal;
	m_hTH_FocusNormal = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                      rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_FocusNormal, sizeof(BITMAP), &m_bmpTH_FocusNormal);

	rcClipImage = g_rcTH_PressedNormal;
	m_hTH_PressedNormal = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                        rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_PressedNormal, sizeof(BITMAP), &m_bmpTH_PressedNormal);

	rcClipImage = g_rcTH_NoFocusSelectAsc;
	m_hTH_NoFocusSelectAsc = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                           rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_NoFocusSelectAsc, sizeof(BITMAP), &m_bmpTH_NoFocusSelectAsc);

	rcClipImage = g_rcTH_FocusSelectAsc;
	m_hTH_FocusSelectAsc = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                         rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_FocusSelectAsc, sizeof(BITMAP), &m_bmpTH_FocusSelectAsc);

	rcClipImage = g_rcTH_PressedSelectAsc;
	m_hTH_PressedSelectAsc = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                           rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_PressedSelectAsc, sizeof(BITMAP), &m_bmpTH_PressedSelectAsc);
	
	rcClipImage = g_rcTH_NoFocusSelectDesc;
	m_hTH_NoFocusSelectDesc = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                            rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_NoFocusSelectDesc, sizeof(BITMAP), &m_bmpTH_NoFocusSelectDesc);

	rcClipImage = g_rcTH_FocusSelectDesc;
	m_hTH_FocusSelectDesc = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                          rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_FocusSelectDesc, sizeof(BITMAP), &m_bmpTH_FocusSelectDesc);

	rcClipImage = g_rcTH_PressedSelectDesc;
	m_hTH_PressedSelectDesc = Bitmap32bppFromBitmap(hbmpTopHdr, rcClipImage.left, 
		                                            rcClipImage.top, rcClipImage.Width(), rcClipImage.Height());
	GetObject(m_hTH_PressedSelectDesc, sizeof(BITMAP), &m_bmpTH_PressedSelectDesc);
	m_nCY_TopHeader = g_nTopHeaderCY;
	::DeleteObject(hbmpTopHdr);
	// ===============================================================================
	}

//*****************************************************************
//	void FreeSkinRes()
//*****************************************************************
void 
iSkinManager::FreeSkinRes()
	{
	// ======= Scroll Resources. ==================================
	HBITMAP* hBmpList[]   = {(HBITMAP*)&m_hArrowLeftH,  (HBITMAP*)&m_hThumbLeftH,  
							 (HBITMAP*)&m_hThumbCenterH,(HBITMAP*)&m_hThumbRightH, 
						     (HBITMAP*)&m_hArrowRightH, (HBITMAP*)&m_hBackgroundH, 
						     (HBITMAP*)&m_hArrowTopV,   (HBITMAP*)&m_hThumbTopV,
							 (HBITMAP*)&m_hThumbCenterV,(HBITMAP*)&m_hThumbBottomV,
							 (HBITMAP*)&m_hArrowBottomV,(HBITMAP*)&m_hBackgroundV, NULL};

	BITMAP* pBmpObjList[] = {(BITMAP*)&m_bmpArrowLeftH,   (BITMAP*)&m_bmpThumbLeftH,
							 (BITMAP*)&m_bmpThumbCenterH, (BITMAP*)&m_bmpThumbRightH,
							 (BITMAP*)&m_bmpArrowRightH,  (BITMAP*)&m_bmpBackgroundH, 
							 (BITMAP*)&m_bmpArrowTopV,    (BITMAP*)&m_bmpThumbTopV,    
							 (BITMAP*)&m_bmpThumbCenterV, (BITMAP*)&m_bmpThumbBottomV, 
							 (BITMAP*)&m_bmpArrowBottomV, (BITMAP*)&m_bmpBackgroundV, NULL};
	int nLoop = 0;
	while( hBmpList[nLoop] )
		{
		HBITMAP* hBmp = (HBITMAP*)hBmpList[nLoop];
		BITMAP*  pObj = (BITMAP*)pBmpObjList[nLoop];

		for( int nLoop1=0; nLoop1<4; nLoop1++ )
			{
			if( hBmp[nLoop1] )
				{
				::DeleteObject(hBmp[nLoop1]);
				hBmp[nLoop1] = NULL;
				memset((void*)&pObj[nLoop1], 0, sizeof(BITMAP));
				}
			}
		nLoop ++;
		}

	// delete region objects. ===================================
	m_rgnArrowLeft	.DeleteObject();
	m_rgnArrowRight .DeleteObject();
	m_rgnArrowTop	.DeleteObject();
	m_rgnArrowBottom.DeleteObject();
	// ==========================================================
	// **********************************************************

	// Delete Other Images. =====================================
	HBITMAP hBmpList1[]   = { m_hTH_NoFocusNormal, m_hTH_FocusNormal, m_hTH_PressedNormal, 
							  m_hTH_NoFocusSelectAsc, m_hTH_FocusSelectAsc, m_hTH_PressedSelectAsc,
							  m_hTH_NoFocusSelectDesc,m_hTH_FocusSelectDesc, m_hTH_PressedSelectDesc,
							  m_hPushBtnNormal, m_hPushBtnHot, m_hPushBtnPressed, m_hPushBtnDisabled, m_hPushBtnDefault, 
							  m_hCheckBtnNormal0, m_hCheckBtnHot0, m_hCheckBtnPressed0, m_hCheckBtnDisabled0,
							  m_hCheckBtnNormal1, m_hCheckBtnHot1, m_hCheckBtnPressed1, m_hCheckBtnDisabled1,
							  m_hCheckBtnNormal2, m_hCheckBtnHot2, m_hCheckBtnPressed2, m_hCheckBtnDisabled2,
							  m_hRadioBtnNormal0, m_hRadioBtnHot0, m_hRadioBtnPressed0, m_hRadioBtnDisabled0,
							  m_hRadioBtnNormal1, m_hRadioBtnHot1, m_hRadioBtnPressed1, m_hRadioBtnDisabled1};


	BITMAP* pBmpObjList1[] = {(BITMAP*)&m_bmpTH_NoFocusNormal,    (BITMAP*)&m_bmpTH_FocusNormal,
							  (BITMAP*)&m_bmpTH_PressedNormal,    (BITMAP*)&m_bmpTH_NoFocusSelectAsc,
							  (BITMAP*)&m_bmpTH_FocusSelectAsc,   (BITMAP*)&m_bmpTH_PressedSelectAsc, 
							  (BITMAP*)&m_bmpTH_NoFocusSelectDesc,(BITMAP*)&m_bmpTH_FocusSelectDesc,    
							  (BITMAP*)&m_bmpTH_PressedSelectDesc,(BITMAP*)&m_bmpPushBtnNormal,
							  (BITMAP*)&m_bmpPushBtnHot,		  (BITMAP*)&m_bmpPushBtnPressed, 
							  (BITMAP*)&m_bmpPushBtnDisabled,     (BITMAP*)&m_bmpPushBtnDefault,

							  (BITMAP*)&m_bmpCheckBtnNormal0,	  (BITMAP*)&m_bmpCheckBtnHot0, 
							  (BITMAP*)&m_bmpCheckBtnPressed0,    (BITMAP*)&m_bmpCheckBtnDisabled0,
							  (BITMAP*)&m_bmpCheckBtnNormal1,	  (BITMAP*)&m_bmpCheckBtnHot1, 
							  (BITMAP*)&m_bmpCheckBtnPressed1,    (BITMAP*)&m_bmpCheckBtnDisabled1,
							  (BITMAP*)&m_bmpCheckBtnNormal2,	  (BITMAP*)&m_bmpCheckBtnHot2, 
							  (BITMAP*)&m_bmpCheckBtnPressed2,    (BITMAP*)&m_bmpCheckBtnDisabled2,
							  (BITMAP*)&m_bmpRadioBtnNormal0,	  (BITMAP*)&m_bmpRadioBtnHot0, 
							  (BITMAP*)&m_bmpRadioBtnPressed0,    (BITMAP*)&m_bmpRadioBtnDisabled0,
							  (BITMAP*)&m_bmpRadioBtnNormal1,	  (BITMAP*)&m_bmpRadioBtnHot1, 
							  (BITMAP*)&m_bmpRadioBtnPressed1,    (BITMAP*)&m_bmpRadioBtnDisabled1,
							  NULL};

	nLoop = 0;
	while( pBmpObjList1[nLoop] )
		{
		HBITMAP  hBmp  = hBmpList1[nLoop];
		BITMAP*  pObj  = (BITMAP*)pBmpObjList1[nLoop];

		ZeroMemory    (pObj, sizeof(BITMAP));
		::DeleteObject(hBmp);
		nLoop ++;
		}
	// **********************************************************
	}

//*****************************************************************
//	BOOL DrawHScrollOnBitmap(CDC* pDC, HBITMAP hbmpBK, UINT nLeftArrowState, 
//  						 UINT nRightArrowState, DrawScroll& draw)
//*****************************************************************
BOOL 
iSkinManager::DrawHScrollOnBitmap(CDC* pDC, HBITMAP hbmpBK, UINT nLeftArrowState, 
								  UINT nRightArrowState, DrawScroll& draw)
	{
	BITMAP*  pBmpArrowLeftH   = &m_bmpArrowLeftH  [nLeftArrowState];
	BITMAP*  pBmpArrowRightH  = &m_bmpArrowRightH [nRightArrowState];
	BITMAP*  pBmpBackgroundH  = &m_bmpBackgroundH [0];
	
	BITMAP  bmpBK;
	GetObject(hbmpBK, sizeof(BITMAP), &bmpBK);
	if( !bmpBK.bmBits ) return FALSE;

	CRect rect;
	CRect rcArrowLeft (0, 0, pBmpArrowLeftH->bmWidth,  pBmpArrowLeftH->bmHeight);
	CRect rcArrowRight(0, 0, pBmpArrowRightH->bmWidth, pBmpArrowRightH->bmHeight);
	rcArrowRight.OffsetRect(draw.rcScroll.Width() - pBmpArrowLeftH->bmWidth, 0);
	CRect rcBackground(rcArrowLeft.right, 0, rcArrowRight.left, pBmpArrowLeftH->bmHeight);

	// draw Arrow Left. =================================
	rect = rcArrowLeft&draw.rcInvalidate;

	if( rect.Width() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpArrowLeftH, 
					   0, 0, pBmpArrowLeftH->bmWidth, pBmpArrowLeftH->bmHeight, 
					   rect.Width(), rect.Height());
		}
	// ================================================

	// draw Background. ===============================
	rect = rcBackground&draw.rcInvalidate;
	if( rect.Width() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpBackgroundH, 
					   0, 0, pBmpBackgroundH->bmWidth, pBmpBackgroundH->bmHeight,
					   rect.Width(), rect.Height()); 
		}
	// ================================================

	// draw Arrow Right. ==============================
	rect = rcArrowRight&draw.rcInvalidate;

	if( rect.Width() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpArrowRightH,
					   0, 0, pBmpArrowRightH->bmWidth, pBmpArrowRightH->bmHeight,
					   rect.Width(), rect.Height());
		}
	// ================================================
	
	DrawThumbHorzOnDib(pDC, bmpBK, draw);
	return TRUE;
	}

//*****************************************************************
//	void DrawThumbHorzOnDib(CDC* pDC, BITMAP& bmpBK, DrawScroll& draw)
//*****************************************************************
void 
iSkinManager::DrawThumbHorzOnDib(CDC* pDC, BITMAP& bmpBK, DrawScroll& draw)
	{
	BITMAP*  pBmpThumbLeftH   = &m_bmpThumbLeftH  [0];
	BITMAP*  pBmpThumbCenterH = &m_bmpThumbCenterH[0];
	BITMAP*  pBmpThumbRightH  = &m_bmpThumbRightH [0];

	if( (draw.nMax - draw.nMin) <= 0 || (int)draw.nPage > (draw.nMax - draw.nMin) )
		{
		draw.rcThumbBtn.left   = 0;
		draw.rcThumbBtn.top    = 0;
		draw.rcThumbBtn.right  = 0;
		draw.rcThumbBtn.bottom = 0;
		draw.nTrackPosMax	   = 0;
		return;
		}

	// calculate thumb button's width. ================
	int	nThumbMax	= bmpBK.bmWidth - m_nCX_HArrowMin - m_nCY_VArrowMin;
	int	nThumb		= (int)(nThumbMax*draw.nPage) / (draw.nMax-draw.nMin);

	if( nThumb < (int)m_nCX_ThumbMin )
		nThumb = m_nCX_ThumbMin;
	else
	if( nThumb >=  nThumbMax )
		nThumb  = 0;

	draw.nTrackPosMax = nThumbMax - nThumb;

	if( (int)draw.nTrackPos < 0 )
		draw.nTrackPos = 0;
	else
	if( draw.nTrackPos > draw.nTrackPosMax )
		draw.nTrackPos = draw.nTrackPosMax;

	draw.rcThumbBtn.left   = m_nCX_HArrowMin + draw.nTrackPos;
	draw.rcThumbBtn.top    = 0;
	draw.rcThumbBtn.right  = draw.rcThumbBtn.left + nThumb;
	draw.rcThumbBtn.bottom = bmpBK.bmHeight;
	// ================================================

	// calculate rect objects. ========================
	CRect rect;
	CRect rcThumbCenter(0, 0, nThumb - pBmpThumbLeftH->bmWidth - pBmpThumbRightH->bmWidth, 
		                pBmpThumbCenterH->bmHeight);
	CRect rcThumbLeft  (0, 0, pBmpThumbLeftH->bmWidth,   pBmpThumbLeftH->bmHeight);
	CRect rcThumbRight (0, 0, pBmpThumbRightH->bmWidth,	 pBmpThumbRightH->bmHeight);

	int nOffset = m_nCX_HArrowMin + draw.nTrackPos;
	rcThumbLeft  .OffsetRect(nOffset, 0);
	rcThumbCenter.OffsetRect(nOffset + pBmpThumbLeftH->bmWidth, 0);
	rcThumbRight .OffsetRect(nOffset + nThumb - pBmpThumbRightH->bmWidth, 0);
	// ================================================
	
	// draw Thumb Left. ===============================
	rect = rcThumbLeft&draw.rcInvalidate;
	if( rect.Width() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpThumbLeftH, 0, 0,
					   pBmpThumbLeftH->bmWidth, pBmpThumbLeftH->bmHeight,
					   pBmpThumbLeftH->bmWidth, pBmpThumbLeftH->bmHeight, TRUE);
		}
	// ================================================

	// draw Thumb Center. =============================
	rect = rcThumbCenter&draw.rcInvalidate;
	if( rect.Width() > 0 )
		{
		int xSrc = 0;
		if( draw.nTrackPos )
			xSrc = draw.nTrackPos%pBmpThumbCenterH->bmWidth;
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpThumbCenterH, xSrc, 0, 
					   pBmpThumbCenterH->bmWidth, pBmpThumbCenterH->bmHeight,
					   rect.Width(), pBmpThumbCenterH->bmHeight, TRUE);
		}
	
	// ================================================

	// draw Thumb Right. ==============================
	rect = rcThumbRight&draw.rcInvalidate;
	if( rect.Width() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpThumbRightH, 0, 0, 
					   pBmpThumbRightH->bmWidth, pBmpThumbRightH->bmHeight,
					   pBmpThumbRightH->bmWidth, pBmpThumbRightH->bmHeight, TRUE);
		}
	// ================================================
	}

//*****************************************************************
//	BOOL DrawVScrollOnBitmap(CDC* pDC, HBITMAP hbmpBK, UINT nTopArrowState, 
//  						 UINT nBottomArrowState, DrawScroll& draw)
//*****************************************************************
BOOL 
iSkinManager::DrawVScrollOnBitmap(CDC* pDC, HBITMAP hbmpBK, UINT nTopArrowState, 
								  UINT nBottomArrowState, DrawScroll& draw)
	{
	BITMAP*  pBmpArrowTopV    = &m_bmpArrowTopV   [nTopArrowState];
	BITMAP*  pBmpArrowBottomV = &m_bmpArrowBottomV[nBottomArrowState];
	BITMAP*  pBmpBackgroundV  = &m_bmpBackgroundV [0];

	BITMAP  bmpBK;
	GetObject(hbmpBK, sizeof(BITMAP), &bmpBK);
	if( !bmpBK.bmBits ) return FALSE;

	CRect rect;
	CRect rcArrowTop   (0, 0, pBmpArrowTopV->bmWidth,    pBmpArrowTopV->bmHeight);
	CRect rcArrowBottom(0, 0, pBmpArrowBottomV->bmWidth, pBmpArrowBottomV->bmHeight);
	rcArrowBottom.OffsetRect(0, draw.rcScroll.Height() - pBmpArrowTopV->bmHeight);
	CRect rcBackground (0, rcArrowTop.bottom, rcArrowBottom.right, rcArrowBottom.top);

	// draw Arrow Top. ===================================
	rect = rcArrowTop&draw.rcInvalidate;
	if( rect.Height() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpArrowTopV, 
			           0, 0, pBmpArrowTopV->bmWidth, pBmpArrowTopV->bmHeight,
					   rect.Width(), rect.Height());
		}
	// ================================================

	// draw BackGround. ===============================
	rect = rcBackground&draw.rcInvalidate;
	if( rect.Width() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpBackgroundV, 
			           0, 0, pBmpBackgroundV->bmWidth, pBmpBackgroundV->bmHeight,
					   rect.Width(), rect.Height());
		}
	// ================================================

	// draw Arrow Bottom. =============================
	rect = rcArrowBottom&draw.rcInvalidate;
	if( rect.Height() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpArrowBottomV, 
			           0, 0, pBmpArrowBottomV->bmWidth, pBmpArrowBottomV->bmHeight,
					   rect.Width(), rect.Height());
		}
	// ================================================
	
	DrawThumbVertOnDib(pDC, bmpBK, draw);
	return TRUE;
	}

//*****************************************************************
//	void DrawThumbVertOnDib(CDC* pDC, BITMAP& bmpBK, DrawScroll& draw)
//*****************************************************************
void 
iSkinManager::DrawThumbVertOnDib(CDC* pDC, BITMAP& bmpBK, DrawScroll& draw)
	{
	BITMAP*  pBmpThumbTopV    = &m_bmpThumbTopV   [0];
	BITMAP*  pBmpThumbCenterV = &m_bmpThumbCenterV[0];
	BITMAP*  pBmpThumbBottomV = &m_bmpThumbBottomV[0];

	if( (draw.nMax - draw.nMin) <= 0 || (int)draw.nPage > (draw.nMax - draw.nMin) )
		{
		draw.rcThumbBtn.left   = 0;
		draw.rcThumbBtn.top    = 0;
		draw.rcThumbBtn.right  = 0;
		draw.rcThumbBtn.bottom = 0;
		draw.nTrackPosMax	   = 0;
		return;
		}

	// calculate thumb button's height. ================
	int	nThumbMax	= bmpBK.bmHeight - 2*m_nCY_VArrowMin;
	int	nThumb		= (int)(nThumbMax*draw.nPage) / (draw.nMax-draw.nMin);

	if( nThumb < (int)m_nCY_ThumbMin )
		nThumb = m_nCY_ThumbMin;
	else
	if( nThumb >=  nThumbMax )
		nThumb  = 0;

	draw.nTrackPosMax  = nThumbMax - nThumb;

	if( (int)draw.nTrackPos < 0 )
		draw.nTrackPos = 0;
	else
	if( draw.nTrackPos > draw.nTrackPosMax )
		draw.nTrackPos = draw.nTrackPosMax;

	draw.rcThumbBtn.left   = 0;
	draw.rcThumbBtn.top    = m_nCY_VArrowMin + draw.nTrackPos;
	draw.rcThumbBtn.right  = bmpBK.bmWidth;
	draw.rcThumbBtn.bottom = draw.rcThumbBtn.top + nThumb;
	// ================================================

	// calculate rect objects. ========================
	CRect rect;
	CRect rcThumbCenter(0, 0, pBmpThumbCenterV->bmWidth, nThumb - pBmpThumbTopV->bmHeight - pBmpThumbBottomV->bmHeight);
	CRect rcThumbTop   (0, 0, pBmpThumbTopV->bmWidth,    pBmpThumbTopV->bmHeight);
	CRect rcThumbBottom(0, 0, pBmpThumbBottomV->bmWidth, pBmpThumbBottomV->bmHeight);

	int nOffset = m_nCY_VArrowMin + draw.nTrackPos;
	rcThumbTop   .OffsetRect(0, nOffset);
	rcThumbCenter.OffsetRect(0, nOffset + pBmpThumbTopV->bmHeight);
	rcThumbBottom.OffsetRect(0, nOffset + nThumb - pBmpThumbBottomV->bmHeight);
	// ================================================
	
	// draw Thumb Top. ================================
	rect = rcThumbTop&draw.rcInvalidate;
	if( rect.Height() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpThumbTopV, 
			           0, 0, pBmpThumbTopV->bmWidth, pBmpThumbTopV->bmHeight,
					   rect.Width(), rect.Height(), TRUE);
		}
	// ================================================

	// draw Thumb Center. =============================
	rect = rcThumbCenter&draw.rcInvalidate;
	if( rect.Height() > 0 )
		{
		int ySrc = 0;
		if( draw.nTrackPos )
			ySrc = draw.nTrackPos%pBmpThumbCenterV->bmHeight;
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpThumbCenterV, 0, ySrc, 
					   pBmpThumbCenterV->bmWidth, pBmpThumbCenterV->bmHeight,
					   pBmpThumbCenterV->bmWidth, rect.Height(), TRUE);
		}
	
	// ================================================

	// draw Thumb Bottom. ==============================
	rect  = rcThumbBottom&draw.rcInvalidate;
	if( rect.Height() > 0 )
		{
		BitmapOnBitmap(&bmpBK, rect.left, rect.top, pBmpThumbBottomV,
			           0, 0, pBmpThumbBottomV->bmWidth, pBmpThumbBottomV->bmHeight,
					   rect.Width(), rect.Height(), TRUE);
		}
	// ================================================
	}

//*****************************************************************
//	void DrawTopHeaderItem(CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState, UINT nItemWidth)
//*****************************************************************
BOOL 
iSkinManager::DrawTopHeaderItem(CDC* pDC, HBITMAP hbmpBK, int x, int y, 
								UINT nItemState, UINT nItemWidth,  LPCTSTR lpszText, 
								int nMargin, int nAlign, BOOL bSortedSign)
	{
	BITMAP   bmpBK;
	GetObject(hbmpBK, sizeof(BITMAP), &bmpBK);
	if( !bmpBK.bmBits ) return FALSE;

	CRect rcDraw(x, y, x + nItemWidth, bmpBK.bmHeight);

	// retrive src bitmap image. ================================
	// ZqrTalent Specific.
	BITMAP*  pBmpSrc   = &((BITMAP*) (&m_bmpTH_NoFocusNormal))[nItemState];
	HBITMAP* phBmpSrc  = &((HBITMAP*)(&m_hTH_NoFocusNormal))  [nItemState];
	// ===========================================================

	// draw Top Header's Item. ===================================
	if( bSortedSign )
		{
		int nLeftPartWidth = nItemWidth - pBmpSrc->bmWidth;
		if( nLeftPartWidth > 0 )
			{
			BitmapOnBitmap(&bmpBK, x, y, pBmpSrc, 0, 0, 1, bmpBK.bmHeight, 
							nLeftPartWidth, bmpBK.bmHeight);
			BitmapOnBitmap(&bmpBK, x + nLeftPartWidth, y, pBmpSrc, 0, 0, 
						   pBmpSrc->bmWidth, bmpBK.bmHeight, pBmpSrc->bmWidth, bmpBK.bmHeight); 
			}
		else
			{
			int nXSrc = 0;
			if( nItemWidth < (UINT)pBmpSrc->bmWidth )
				nXSrc = pBmpSrc->bmWidth - nItemWidth;

			BitmapOnBitmap(&bmpBK, x, y, pBmpSrc, nXSrc, 0, pBmpSrc->bmWidth, 
						   bmpBK.bmHeight, nItemWidth, bmpBK.bmHeight);
			}
		}
	else
		{
		BitmapOnBitmap(&bmpBK, x, y, pBmpSrc, 0, 0, 1, 
						bmpBK.bmHeight, nItemWidth-1, bmpBK.bmHeight);
		// draw divition border. ==================================
		BitmapOnBitmap(&bmpBK, x + nItemWidth - 1, y, pBmpSrc, pBmpSrc->bmWidth-1, 0, 1, 
						bmpBK.bmHeight, 1, bmpBK.bmHeight);
		// ========================================================
		}
	// ===========================================================

	rcDraw.left  += nMargin;
	rcDraw.right --; // 1px must be reserved for vertical line on the right of item.
	if( rcDraw.Width() > 0 )
		pDC->DrawText(lpszText, rcDraw, nAlign);

	return TRUE;
	}

//*****************************************************************
//	void DrawPushButton(CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState,
//						UINT nItemWidth, LPCTSTR lpszText, int nMargin, int nAlign);		
//*****************************************************************
BOOL 
iSkinManager::DrawPushButton(CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState,
							 LPCTSTR lpszText, int nMargin, int nAlign)
	{
/*
	BITMAP   bmpBK;
	GetObject(hbmpBK, sizeof(BITMAP), &bmpBK);
	if( !bmpBK.bmBits ) return FALSE;

	CRect rcDraw(x, y, x + bmpBK.bmWidth, bmpBK.bmHeight);

	// retrive src bitmap image. ================================
	// ZqrTalent Specific.
	BITMAP*  pBmpSrc   = &((BITMAP*) (&m_bmpPushBtnNormal))[nItemState];
	HBITMAP* phBmpSrc  = &((HBITMAP*)(&m_hPushBtnNormal))  [nItemState];
	// ===========================================================

	// Draw Image. ===============================================
	m_gds.m_hImage         = (HBITMAP)*phBmpSrc;
	m_gds.m_drawOption     = m_nPushBtnDrawOp;
	m_gds.m_leftArcValue   = m_nPushBtnArcLeft;
	m_gds.m_rightArcValue  = m_nPushBtnArcRight;
	m_gds.m_topArcValue    = m_nPushBtnArcTop;
	m_gds.m_bottomArcValue = m_nPushBtnArcBottom;
	m_gds.m_crTransparent  = m_nPushBtnTransClr;
	m_gds.m_hDC			   = pDC->m_hDC;

	m_gds.m_rcClip.left    = rcDraw.left;
	m_gds.m_rcClip.top     = rcDraw.top;
	m_gds.m_rcClip.right   = rcDraw.right;
	m_gds.m_rcClip.bottom  = rcDraw.bottom;

	m_gds.m_rcDest.left    = rcDraw.left;
	m_gds.m_rcDest.top     = rcDraw.top;
	m_gds.m_rcDest.right   = rcDraw.right;
	m_gds.m_rcDest.bottom  = rcDraw.bottom;

	m_gds.m_rcSrc.left	   = 0;
	m_gds.m_rcSrc.top      = 0;
	m_gds.m_rcSrc.right    = pBmpSrc->bmWidth;
	m_gds.m_rcSrc.bottom   = pBmpSrc->bmHeight;

	m_gds.Call_GdiDrawStream(pDC->m_hDC);
	// =============================================================

	// draw text. ==================================================
	rcDraw.left  += nMargin;
	rcDraw.bottom -= 2;
	if( rcDraw.Width() > 0 )
		{
		pDC->SetBkMode   (TRANSPARENT);
		pDC->SelectObject(&m_btnFont);
		pDC->DrawText    (lpszText, rcDraw, nAlign);
		}*/
	// ==============================================================
	return TRUE;
	}

//*****************************************************************
//	void DrawCheckButton(CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState,
//						 UINT nItemWidth, LPCTSTR lpszText, int nMargin, int nAlign);		
//*****************************************************************
BOOL 
iSkinManager::DrawCheckButton(CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState,
							  LPCTSTR lpszText, int nMargin, int nAlign)
	{
/*
	BITMAP   bmpBK;
	GetObject(hbmpBK, sizeof(BITMAP), &bmpBK);
	if( !bmpBK.bmBits ) return FALSE;

	CRect rcDraw(x, y, x + bmpBK.bmWidth, bmpBK.bmHeight);

	// retrive src bitmap image. ================================
	// ZqrTalent Specific.
	BITMAP*  pBmpSrc   = &((BITMAP*) (&m_bmpCheckBtnNormal0))[nItemState];
	HBITMAP* phBmpSrc  = &((HBITMAP*)(&m_hCheckBtnNormal0))  [nItemState];
	// ===========================================================

	if( rcDraw.Width() > pBmpSrc->bmWidth )
		{
		rcDraw.right = pBmpSrc->bmWidth;
		}

	if( rcDraw.Height() > pBmpSrc->bmHeight )
		{
		rcDraw.top    = (rcDraw.Height() - pBmpSrc->bmHeight)/2;
		rcDraw.bottom = rcDraw.top + pBmpSrc->bmHeight;
		}

	// Draw Image. ===============================================
	m_gds.m_hImage         = (HBITMAP)*phBmpSrc;
	m_gds.m_drawOption     = m_nCheckBtnDrawOp;
	m_gds.m_leftArcValue   = m_nCheckBtnArcLeft;
	m_gds.m_rightArcValue  = m_nCheckBtnArcRight;
	m_gds.m_topArcValue    = m_nCheckBtnArcTop;
	m_gds.m_bottomArcValue = m_nCheckBtnArcBottom;

	m_gds.m_crTransparent  = m_nCheckBtnTransClr;
	m_gds.m_hDC			   = pDC->m_hDC;

	m_gds.m_rcClip.left    = rcDraw.left;
	m_gds.m_rcClip.top     = rcDraw.top;
	m_gds.m_rcClip.right   = rcDraw.right;
	m_gds.m_rcClip.bottom  = rcDraw.bottom;

	m_gds.m_rcDest.left    = rcDraw.left;
	m_gds.m_rcDest.top     = rcDraw.top;
	m_gds.m_rcDest.right   = rcDraw.right;
	m_gds.m_rcDest.bottom  = rcDraw.bottom;

	m_gds.m_rcSrc.left	   = 0;
	m_gds.m_rcSrc.top      = 0;
	m_gds.m_rcSrc.right    = pBmpSrc->bmWidth;
	m_gds.m_rcSrc.bottom   = pBmpSrc->bmHeight;

	m_gds.Call_GdiDrawStream(pDC->m_hDC);
	// =============================================================

	// draw text. ==================================================
	rcDraw.left   += nMargin;
	rcDraw.bottom -= 2;
	if( rcDraw.Width() > 0 )
		{
		pDC->SetBkMode   (TRANSPARENT);
		pDC->SelectObject(&m_btnFont);
//		pDC->DrawText    (lpszText, rcDraw, nAlign);
		}
	// ==============================================================*/
	return TRUE;
	}