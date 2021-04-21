#include "stdafx.h"
#include "BitmapEx.h"

CBitmapEx::CBitmapEx()
	{
	m_hObject	= NULL;
	m_nBmWidth	= 0;
	m_nBmHeight	= 0;
	}

CBitmapEx::~CBitmapEx()
	{
	}

BOOL
CBitmapEx::SetBitmap32bpp(HBITMAP hBitmap)
	{
	BITMAP bmp;
	if( !::GetObject(hBitmap, sizeof(BITMAP), &bmp) || 
		bmp.bmBitsPixel != 32 )
		return FALSE;

	m_nBmWidth	= bmp.bmWidth;
	m_nBmHeight = bmp.bmHeight;

	Attach(hBitmap);
	return FALSE;
	}

HBITMAP
CBitmapEx::LoadTGAImageFile(const CString sTGAImage)
	{
	FILE*			filePtr;
	unsigned char	ucharBad;		// garbage data
	short int		sintBad;		// garbage data
	long			imageSize;		// size of TGA image
	int				colorMode;		// 4 for RGBA, 3 for RGB
	unsigned char*	imageData;		// the TGA data

	TGAHEADER		tgaHeaderStruct;
	TGAHEADER*		tgaHeader	= &tgaHeaderStruct;

	// open the TGA file
	filePtr = fopen(sTGAImage, "rb");
	if( !filePtr )
		return NULL;
	// read first two bytes of garbage
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
	// read in the image type
	fread(&tgaHeader->imageTypeCode, sizeof(unsigned char), 1, filePtr);
	// for our purposes, the image type should be either a 2 or a 3
	if( (tgaHeader->imageTypeCode != 2) && (tgaHeader->imageTypeCode != 3) )
		{
		fclose(filePtr);
		return NULL;
		}

	// read 13 bytes of garbage data
	fread(&sintBad, sizeof(short int), 1, filePtr);
	fread(&sintBad, sizeof(short int), 1, filePtr);
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
	fread(&sintBad, sizeof(short int), 1, filePtr);
	fread(&sintBad, sizeof(short int), 1, filePtr);
	// read image dimensions
	fread(&tgaHeader->imageWidth, sizeof(short int), 1, filePtr);
	fread(&tgaHeader->imageHeight, sizeof(short int), 1, filePtr);
	// read bit depth
	fread(&tgaHeader->bitCount, sizeof(unsigned char), 1, filePtr);
	// read garbage
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
	// colormode -> 3 = BGR, 4 = BGRA
	colorMode = tgaHeader->bitCount / 8;
	imageSize = tgaHeader->imageWidth * tgaHeader->imageHeight * colorMode;
	// allocate memory for image data
	imageData = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);
	// read image data
	fread(imageData, sizeof(unsigned char), imageSize, filePtr);

	// Create bitmap and 
	BITMAPINFOHEADER	bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount		= tgaHeader->bitCount;
	bmpInfo32.biCompression		= BI_RGB;
	bmpInfo32.biPlanes			= 1;
	bmpInfo32.biHeight			= tgaHeader->imageHeight;
	bmpInfo32.biSize			= sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth			= tgaHeader->imageWidth;

	HDC					memDC	= ::CreateCompatibleDC(NULL);
	UINT*				lpMap;
	HBITMAP				hBitmap	= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, (void**)&lpMap, NULL, 0);
	ASSERT( lpMap );

	if( tgaHeader->bitCount == 32 )
		{
		// copy pixels.
		memcpy(lpMap, imageData, imageSize);
		}
	else
		ASSERT(FALSE);
	// ##################

	DeleteDC(memDC);
	// free image data.
	free(imageData);
	// close the file
	fclose(filePtr);
	return hBitmap;
	}


HBITMAP
CBitmapEx::LoadTGAImage(HGLOBAL hRes)
	{
	char*			lpResData	= (char*)LockResource(hRes);
	unsigned char	ucharBad;		// garbage data
	short int		sintBad;		// garbage data
	long			imageSize;		// size of TGA image
	int				colorMode;		// 4 for RGBA, 3 for RGB
	unsigned char*	imageData;		// the TGA data

	TGAHEADER		tgaHeaderStruct;
	TGAHEADER*		tgaHeader	= &tgaHeaderStruct;

	// read first two bytes of garbage
	memcpy(&ucharBad, lpResData, sizeof(unsigned char));
	lpResData += sizeof(unsigned char);
	memcpy(&ucharBad, lpResData, sizeof(unsigned char));
	lpResData += sizeof(unsigned char);

	// read in the image type
	memcpy(&tgaHeader->imageTypeCode, lpResData, sizeof(unsigned char));
	lpResData += sizeof(unsigned char);

	// for our purposes, the image type should be either a 2 or a 3
	if( (tgaHeader->imageTypeCode != 2) && (tgaHeader->imageTypeCode != 3) )
		{
		UnlockResource(hRes);
		return NULL;
		}

	// read 13 bytes of garbage data
	memcpy(&sintBad, lpResData, sizeof(short int));
	lpResData += sizeof(short int);

	memcpy(&sintBad, lpResData, sizeof(short int));
	lpResData += sizeof(short int);

	memcpy(&ucharBad, lpResData, sizeof(unsigned char));
	lpResData += sizeof(unsigned char);

	memcpy(&sintBad, lpResData, sizeof(short int));
	lpResData += sizeof(short int);

	memcpy(&sintBad, lpResData, sizeof(short int));
	lpResData += sizeof(short int);



	// read image dimensions
	memcpy(&tgaHeader->imageWidth, lpResData, sizeof(short int));
	lpResData += sizeof(short int);
	memcpy(&tgaHeader->imageHeight, lpResData, sizeof(short int));
	lpResData += sizeof(short int);
	// read bit depth
	memcpy(&tgaHeader->bitCount, lpResData, sizeof(unsigned char));
	lpResData += sizeof(unsigned char);

	// read garbage
	memcpy(&ucharBad, lpResData, sizeof(unsigned char));
	lpResData += sizeof(unsigned char);
	// colormode -> 3 = BGR, 4 = BGRA
	colorMode = tgaHeader->bitCount / 8;
	imageSize = tgaHeader->imageWidth * tgaHeader->imageHeight * colorMode;

	// allocate memory for image data
	imageData = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);

	// read image data
	memcpy(imageData, lpResData, imageSize);
	lpResData += imageSize;

	// Create bitmap and 
	BITMAPINFOHEADER	bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount		= tgaHeader->bitCount;
	bmpInfo32.biCompression		= BI_RGB;
	bmpInfo32.biPlanes			= 1;
	bmpInfo32.biHeight			= tgaHeader->imageHeight;
	bmpInfo32.biSize			= sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth			= tgaHeader->imageWidth;

	HDC					memDC	= ::CreateCompatibleDC(NULL);
	UINT*				lpMap;
	HBITMAP				hBitmap	= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, (void**)&lpMap, NULL, 0);
	ASSERT( lpMap );

	if( tgaHeader->bitCount == 32 )
		{
		// copy pixels.
		memcpy(lpMap, imageData, imageSize);
		}
	else
		ASSERT(FALSE);
	// ##################

	free(imageData);
	DeleteDC(memDC);
	UnlockResource(hRes);
	return hBitmap;
	}

BOOL 
CBitmapEx::LoadBitmapFromFile(const CString sBmpFile)
	{
	ASSERT( !m_hObject );
	m_hObject = ::LoadImageA(NULL, sBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if( m_hObject == NULL )
		{
		m_sLastError = _T("Couldn't load bitmap from file !!!");
		return FALSE;
		}

	BITMAP bmpImage;
	::GetObject(m_hObject, sizeof(BITMAP), &bmpImage);

	m_nBmWidth	= bmpImage.bmWidth;
	m_nBmHeight	= bmpImage.bmHeight;
	return TRUE;
	}

BOOL
CBitmapEx::DrawBitmap(CDC* pDC, CRect rcDraw, CRect rcTile)
	{
	if( !m_hObject )
		{
		m_sLastError = _T("First load image and then draw it !!!");
		return FALSE;
		}

	CRect rcImage(0, 0, m_nBmWidth, m_nBmHeight);
	if( (rcImage|rcTile) != rcImage )
		{
		m_sLastError = _T("Incorrect rcTile parameter !!!");
		return FALSE;
		}

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(m_hObject);

	int nOldMode = pDC->SetStretchBltMode(4);

	if( !rcTile.Width() || !rcTile.Height() )
		{
		pDC->StretchBlt(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), &memDC, 
						rcImage.left, rcImage.top, rcImage.Width(), rcImage.Height(), SRCCOPY);
		}
	else
		{
		CRect rcLeftImg		(rcImage.left, rcImage.top, rcTile.left, rcImage.bottom);
		CRect rcRightImg	(rcTile.right, rcImage.top,	rcImage.right, rcImage.bottom);
		CRect rcTopImg		(rcImage.left, rcImage.top,	rcImage.right, rcTile.top);
		CRect rcBottomImg	(rcImage.left, rcTile.bottom, rcImage.right, rcImage.bottom);

		rcLeftImg.top		+= rcTopImg.Height();
		rcLeftImg.bottom	-= rcBottomImg.Height();
		rcRightImg.top		+= rcTopImg.Height();
		rcRightImg.bottom	-= rcBottomImg.Height();
		rcTopImg.left		+= rcLeftImg.Width();
		rcTopImg.right		-= rcRightImg.Width();
		rcBottomImg.left	+= rcLeftImg.Width();
		rcBottomImg.right	-= rcRightImg.Width();

		CRect rcLeft		(rcDraw.left, rcDraw.top + rcTopImg.Height(), rcDraw.left + rcLeftImg.Width(), rcDraw.bottom - rcBottomImg.Height());
		CRect rcRight		(rcDraw.right - rcRightImg.Width(), rcDraw.top + rcTopImg.Height(),	rcDraw.right, rcDraw.bottom - rcBottomImg.Height());
		CRect rcTop			(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.top + rcTopImg.Height());
		CRect rcBottom		(rcDraw.left, rcDraw.bottom - rcBottomImg.Height(), rcDraw.right, rcDraw.bottom);
		rcDraw.SetRect		(rcLeft.right, rcTop.bottom, rcRight.left, rcBottom.top);
		
		pDC->StretchBlt(rcLeft.left, rcLeft.top, rcLeft.Width(), rcLeft.Height(), &memDC, 
						rcLeftImg.left, rcLeftImg.top, rcLeftImg.Width(), rcLeftImg.Height(), SRCCOPY);

		pDC->StretchBlt(rcRight.left, rcRight.top, rcRight.Width(), rcRight.Height(), &memDC, 
						rcRightImg.left, rcRightImg.top, rcRightImg.Width(), rcRightImg.Height(), SRCCOPY);

		pDC->StretchBlt(rcTop.left, rcTop.top, rcTop.Width(), rcTop.Height(), &memDC, 
						rcTopImg.left, rcTopImg.top, rcTopImg.Width(), rcTopImg.Height(), SRCCOPY);

		pDC->StretchBlt(rcBottom.left, rcBottom.top, rcBottom.Width(), rcBottom.Height(), &memDC, 
						rcBottomImg.left, rcBottomImg.top, rcBottomImg.Width(), rcBottomImg.Height(), SRCCOPY);
	
		pDC->StretchBlt(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), &memDC, 
						rcTile.left, rcTile.top, rcTile.Width(), rcTile.Height(), SRCCOPY);
		}
	
	pDC->SetStretchBltMode(nOldMode);
	memDC.DeleteDC();
	return TRUE;
	}

// **********************************************************************
//							Static Functions.						   //
// **********************************************************************

//***********************************************************************
//	HBITMAP CreateBitmap32bpp(UINT* pColorTable, int x, int y, int nWidth, int nHeight)
//***********************************************************************
HBITMAP 
CBitmapEx::CreateBitmap32bpp(UINT* pColorTable, int nWidth, int nHeight, BITMAP* pBmp)
	{
	BITMAPINFOHEADER	bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount		= 32;
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

	int index = 0;
	while( index < nWidth*nHeight )
		{
		lpMap[index] = pColorTable[index];
		index ++;
		}

	if( pBmp )
		::GetObject(hDib, sizeof(BITMAP), pBmp);

	DeleteDC(memDC);
	return hDib;
	}

//*****************************************************************
//	BOOL RegionFromAlphaData(HBITMAP hBmpAlpha, CRgn& rgnMask)
//*****************************************************************
BOOL 
CBitmapEx::RegionFromAlphaData(HBITMAP hBmpAlpha, CRgn& rgnMask)
	{
	HDC  hMemDC  = CreateCompatibleDC(NULL);
	HDC	 hSrc    = CreateCompatibleDC(hMemDC);
	::SelectObject(hSrc,hBmpAlpha);

	BITMAP bmp;
	::GetObject(hBmpAlpha, sizeof(BITMAP),&bmp);
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
		::GetObject(hDib,sizeof(BITMAP),&bmp);
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
//	BOOL SaveBitmap32bppFile(LPCTSTR lpszFile, HBITMAP hBmp)
//*****************************************************************
BOOL 
CBitmapEx::SaveBitmap32bppFile(LPCTSTR lpszFile, HBITMAP hBmp)
	{
	BOOL	bRet = FALSE;
	HDC		memDC, srcDC;
	memDC = ::CreateCompatibleDC(NULL);
	srcDC = ::CreateCompatibleDC(memDC);
	SelectObject(srcDC,hBmp);
	
	BITMAP bmp;
	ZeroMemory(&bmp,sizeof(BITMAP));
	::GetObject (hBmp,sizeof(BITMAP),&bmp);

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
	
	::GetObject(hDib,sizeof(BITMAP),&bmp);
	ASSERT(BitBlt(memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY));

	HANDLE hFile = CreateFile(lpszFile,GENERIC_READ|GENERIC_WRITE,
							  FILE_SHARE_READ|FILE_SHARE_WRITE,
							  NULL,CREATE_NEW,0L,NULL            );
	if( hFile && hDib )
		{
		BITMAPFILEHEADER bfh;
		ZeroMemory(&bfh, sizeof(BITMAPFILEHEADER));
		bfh.bfType		= 0x4d42; // 'BM'
		bfh.bfSize		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmp.bmHeight*bmp.bmWidth*4;
		bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		DWORD dwWritten = 0L;
		//write bitmap file header structure.
		WriteFile(hFile,&bfh,sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
		//write info header structure.
		WriteFile(hFile,&bmpInfo32,sizeof(BITMAPINFOHEADER),&dwWritten,NULL);
		//An array of RGBQUAD. The elements of the array that make up the color table. 
		WriteFile(hFile,lpMap,bfh.bfSize - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)),&dwWritten,NULL);
		bRet = TRUE;
		}

	CloseHandle		(hFile);
	DeleteDC		(memDC);
	DeleteDC		(srcDC);
	::DeleteObject	(hDib);
	return bRet;
	}

//*****************************************************************
//	BOOL ConvertToBitmap32bpp(HBITMAP& hBmp)
//*****************************************************************
BOOL 
CBitmapEx::ConvertToBitmap32bpp(HBITMAP& hBmp)
	{
	BOOL bRet = FALSE;
	HDC  memDC, srcDC;
	memDC = ::CreateCompatibleDC(NULL);
	srcDC = ::CreateCompatibleDC(memDC);
	SelectObject(srcDC, hBmp);
	
	BITMAP bmp;
	ZeroMemory(&bmp, sizeof(BITMAP));
	::GetObject (hBmp, sizeof(BITMAP), &bmp);
	if( bmp.bmBitsPixel == 32 && bmp.bmBits )
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
	
	::GetObject(hDib, sizeof(BITMAP), &bmp);
	BitBlt(memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

	::DeleteObject	(hBmp);
	DeleteDC		(memDC);
	DeleteDC		(srcDC);

	hBmp = hDib;
	return hDib ? TRUE : FALSE;
	}

//*****************************************************************
//	HBITMAP GetBitmap32bppFromDC(CDC* pSrc,int xSrc,int ySrc,
//								 int nWidth,int nHeight)
//*****************************************************************
HBITMAP 
CBitmapEx::GetBitmap32bppFromDC(CDC* pSrc, int xSrc, int ySrc, 
								int nWidth, int nHeight, CDC** pDCBitmap /*= NULL*/)
	{
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(NULL);

	BITMAPINFOHEADER bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount    = 32;
	bmpInfo32.biCompression = BI_RGB;
	bmpInfo32.biPlanes      = 1;
	bmpInfo32.biHeight      = nHeight;
	bmpInfo32.biSize        = sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth       = nWidth;
	
	void*   lpMap = NULL;
	HBITMAP hDib  = CreateDIBSection(memDC->m_hDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
	
	memDC->SelectObject	(hDib);
	memDC->BitBlt		(0, 0, nWidth, nHeight, pSrc, xSrc, ySrc, SRCCOPY);
	if( pDCBitmap )
		{
		*pDCBitmap = memDC;
		}
	else
		{
		memDC->DeleteDC		();
		delete memDC;
		}
	return hDib;
	}

//*****************************************************************
//	BOOL Bitmap32bppFromBitmap(HBITMAP hBmpSrc, int xSrc, 
//							   int ySrc, int nWidth, int nHeight)
//*****************************************************************
HBITMAP 
CBitmapEx::Bitmap32bppFromBitmap(HBITMAP hBmpSrc, int xSrc, 
								int ySrc, int nWidth, int nHeight)
	{
	HDC memDC, srcDC;
	memDC = ::CreateCompatibleDC(NULL);
	srcDC = ::CreateCompatibleDC(memDC);
	SelectObject(srcDC, hBmpSrc);
	
	BITMAP bmp;
	::GetObject(hBmpSrc, sizeof(BITMAP),&bmp);

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
		::DeleteObject	(hDib);
		DeleteDC		(memDC);
		DeleteDC		(srcDC);
		return NULL;
		}
	
	BitBlt(memDC, 0, 0, nWidth, nHeight, srcDC, xSrc, ySrc, SRCCOPY);

	DeleteDC    (memDC);
	DeleteDC    (srcDC);
	return hDib;
	}

//*****************************************************************
//	BOOL BitmapOnBitmap(HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc, 
//						int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
//						int nDrawWidth, int nDrawHeight, BOOL bAlpha)
//*****************************************************************
BOOL
CBitmapEx::BitmapOnBitmap(HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc, 
						  int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, int nDrawWidth, 
						  int nDrawHeight, BOOL bAlpha)
	{
	// check parameters validation. ======================================
	BITMAP bmpSrc, bmpDest;

	if( sizeof(BITMAP) != ::GetObject(hBmpSrc, sizeof(BITMAP), &bmpSrc)   || 
		sizeof(BITMAP) != ::GetObject(hBmpDest, sizeof(BITMAP), &bmpDest) ||
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
CBitmapEx::BitmapOnBitmap(BITMAP* pbmpDest, int xDest, int yDest, BITMAP* pbmpSrc, 
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
						((COLORREF*)pArrDest)[nIndexDest]= ALPHA_RGB_TO_RGB((BYTE)nAlpha, pArrDest[nIndexDest].rgbBlue, pArrDest[nIndexDest].rgbGreen, pArrDest[nIndexDest].rgbRed, 
																			pArrSrc[nIndexSrc].rgbBlue, pArrSrc[nIndexSrc].rgbGreen, pArrSrc[nIndexSrc].rgbRed);
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
//	void CopyAlphaIndexesToImage(HBITMAP hbmpImage, HBITMAP hbmpAlpha)
//*****************************************************************
void 
CBitmapEx::CopyAlphaIndexesToImage(HBITMAP hbmpImage, HBITMAP hbmpAlpha)
	{
	BITMAP bmpImage, bmpAlpha;
	::GetObject(hbmpImage, sizeof(BITMAP), &bmpImage);
	::GetObject(hbmpAlpha, sizeof(BITMAP), &bmpAlpha);

	if( bmpImage.bmBitsPixel != 32 )
		{
		ConvertToBitmap32bpp(hbmpImage);
		::GetObject(hbmpImage, sizeof(BITMAP), &bmpImage);
		}

	if( bmpAlpha.bmBitsPixel != 32 )
		{
		ConvertToBitmap32bpp(hbmpAlpha);
		::GetObject(hbmpAlpha, sizeof(BITMAP), &bmpAlpha);
		}

	if( bmpImage.bmWidth     != bmpAlpha.bmWidth  ||
		bmpImage.bmHeight    != bmpAlpha.bmHeight ||
	    bmpImage.bmBits      == NULL             || 
		bmpImage.bmBits      == NULL )
		{
		return;
		}
	
	RGBQUAD* pBitsImage		= (RGBQUAD*)bmpImage.bmBits;
	RGBQUAD* pBitsAlpha		= (RGBQUAD*)bmpAlpha.bmBits;
	int      nLoop			= 0;

	for( int y=0; y<bmpImage.bmHeight; y++ )
		for( int x=0; x<bmpAlpha.bmWidth; x++ )
			{
			ASSERT( pBitsAlpha[nLoop].rgbBlue == pBitsAlpha[nLoop].rgbRed  &&
				    pBitsAlpha[nLoop].rgbBlue == pBitsAlpha[nLoop].rgbGreen );

			pBitsImage[nLoop].rgbReserved = pBitsAlpha[nLoop].rgbBlue;
			nLoop ++;
			}
	}

//*****************************************************************
//	void TransparentColor(HBITMAP hImage, COLORREF crTrans)
//*****************************************************************
void 
CBitmapEx::TransparentColor(HBITMAP hImage, COLORREF crTrans)
	{
	BITMAP bmImage;
	::GetObject(hImage, sizeof(BITMAP), &bmImage);

	int nWidth		= bmImage.bmWidth;
	int nHeight		= bmImage.bmHeight;
	int nLoop		= 0;
	int nPixelCount = nHeight*nWidth;

	// Set alpha parameter for transparent pixel. ################
	while( nLoop < nPixelCount )
		{
		RGBQUAD* pRGBQuad	= &((RGBQUAD*)bmImage.bmBits)[nLoop];
		COLORREF crPixel	= RGB(pRGBQuad->rgbRed, pRGBQuad->rgbGreen, pRGBQuad->rgbBlue);
		if( crPixel == crTrans )
			pRGBQuad->rgbReserved = 0x00;
		else
			pRGBQuad->rgbReserved = 0xFF;
		nLoop ++;
		}
	// ###########################################################
	}