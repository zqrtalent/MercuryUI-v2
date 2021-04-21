#include <afxwin.h>

// ZqrTalent Specific.
#define ALPHA(alpha, src, dest) \
	((BYTE)(dest* (alpha/255.0) + src* (1.0-(alpha/255.0)))

#define ALPHA_RGB_TO_RGB(alpha, rSrc, gSrc, bSrc, rDest, gDest, bDest)				\
						((COLORREF)(ALPHA(alpha, rSrc, rDest)) |					\
						((WORD)	ALPHA(alpha, gSrc, gDest)) << 8)) |					\
						(((DWORD)ALPHA(alpha, bSrc, bDest)) << 16)))				\


typedef struct
{
	unsigned char imageTypeCode;
	short int imageWidth;
	short int imageHeight;
	unsigned char bitCount;
} TGAHEADER;


class CBitmapEx : public CBitmap
{
public:
	CBitmapEx();
	virtual ~CBitmapEx();

	//	< Protected methods >
protected:

	CString		m_sLastError;
	int			m_nBmWidth;
	int			m_nBmHeight;

	//	< Public methods >
public:

	BOOL			SetBitmap32bpp		(HBITMAP hBitmap);
	BOOL			LoadBitmapFromFile	(const CString sBmpFile);
	BOOL			DrawBitmap			(CDC* pDC, CRect rcDraw, CRect rcTile);
	int				GetWidth			();
	int				GetHeight			();
	const CString	GetLastError		();

	// Load TGA imge. ####################
	static HBITMAP	LoadTGAImageFile	(const CString sTGAImage);
	static HBITMAP	LoadTGAImage		(HGLOBAL hRes);
	// ###################################

	static int		AlphaIndex			(BYTE redSrc, BYTE greenSrc, BYTE blueSrc, 
										 BYTE redDest, BYTE greenDest, BYTE blueDest);

	//	< Public static methods >
public:
	// **********************************************************************
	//							Static Functions.						   //
	// **********************************************************************
	/* Creates 32bpp bitmap image from color table. */
	static HBITMAP CreateBitmap32bpp	 (UINT* pColorTable, int nWidth, int nHeight, BITMAP* pBmp = NULL);         
	/* Retrieves alpha image region from alpha data.*/
	static BOOL    RegionFromAlphaData   (HBITMAP hBmpAlpha, CRgn& rgnMask);
	/* Saves image as bitmap file.*/
	static BOOL    SaveBitmap32bppFile   (LPCTSTR lpszFile, HBITMAP hBmp);
	/* Converts bitmap to 32bpp format.*/
	static BOOL    ConvertToBitmap32bpp  (HBITMAP& hBmp);
	/* Transparent color.*/
	static void	   TransparentColor		 (HBITMAP hImage, COLORREF crTrans);
	/* Retrieves bitmap 32bpp from device context.*/
	static HBITMAP GetBitmap32bppFromDC  (CDC* pSrc, int xSrc, int ySrc, int nWidth, int nHeight, CDC** pDCBitmap = NULL);
	/*Retrieves bitmap 32bpp from bitmap.*/
	static HBITMAP Bitmap32bppFromBitmap (HBITMAP hBmpSrc, int xSrc, int ySrc, int nWidth, int nHeight);
	/*Draws bitmap 32bpp to bitmap.*/
	static BOOL	   BitmapOnBitmap		 (HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc,
										  int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
										  int nDrawWidth, int nDrawHeight, BOOL bAlpha = FALSE);
	/*Draws bitmap 32bpp to bitmap.*/
	static BOOL	   BitmapOnBitmap		 (BITMAP* pbmpDest, int xDest, int yDest, BITMAP* pbmpSrc, 
										  int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
										  int nDrawWidth, int nDrawHeight, BOOL bAlpha = FALSE);

	/*Copies alpha indexes to image from alpha image (image that only contains alpha indexes).*/
	static void    CopyAlphaIndexesToImage(HBITMAP hbmpImage, HBITMAP hbmpAlpha);
	// **********************************************************************
};


__inline const CString
CBitmapEx::GetLastError()
	{
	return m_sLastError;
	}

__inline int
CBitmapEx::GetWidth()
	{
	return m_nBmWidth;
	}

__inline int
CBitmapEx::GetHeight()
	{
	return m_nBmHeight;
	}