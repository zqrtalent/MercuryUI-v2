// ImageHelper.h header file.
//
//	ESCL (Elite Soft Class Library)
//	
//	Author: ZqrTalent
//

#pragma once

const double dAlpha[] = 
{
0.000000, 0.003922, 0.007843, 0.011765, 0.015686, 0.019608, 0.023529, 0.027451, 0.031373, 0.035294, 0.039216, 0.043137, 0.047059, 0.050980, 0.054902, 0.058824, 
0.062745, 0.066667, 0.070588, 0.074510, 0.078431, 0.082353, 0.086275, 0.090196, 0.094118, 0.098039, 0.101961, 0.105882, 0.109804, 0.113725, 0.117647, 0.121569, 
0.125490, 0.129412, 0.133333, 0.137255, 0.141176, 0.145098, 0.149020, 0.152941, 0.156863, 0.160784, 0.164706, 0.168627, 0.172549, 0.176471, 0.180392, 0.184314, 
0.188235, 0.192157, 0.196078, 0.200000, 0.203922, 0.207843, 0.211765, 0.215686, 0.219608, 0.223529, 0.227451, 0.231373, 0.235294, 0.239216, 0.243137, 0.247059, 
0.250980, 0.254902, 0.258824, 0.262745, 0.266667, 0.270588, 0.274510, 0.278431, 0.282353, 0.286275, 0.290196, 0.294118, 0.298039, 0.301961, 0.305882, 0.309804, 
0.313725, 0.317647, 0.321569, 0.325490, 0.329412, 0.333333, 0.337255, 0.341176, 0.345098, 0.349020, 0.352941, 0.356863, 0.360784, 0.364706, 0.368627, 0.372549, 
0.376471, 0.380392, 0.384314, 0.388235, 0.392157, 0.396078, 0.400000, 0.403922, 0.407843, 0.411765, 0.415686, 0.419608, 0.423529, 0.427451, 0.431373, 0.435294, 
0.439216, 0.443137, 0.447059, 0.450980, 0.454902, 0.458824, 0.462745, 0.466667, 0.470588, 0.474510, 0.478431, 0.482353, 0.486275, 0.490196, 0.494118, 0.498039, 
0.501961, 0.505882, 0.509804, 0.513725, 0.517647, 0.521569, 0.525490, 0.529412, 0.533333, 0.537255, 0.541176, 0.545098, 0.549020, 0.552941, 0.556863, 0.560784, 
0.564706, 0.568627, 0.572549, 0.576471, 0.580392, 0.584314, 0.588235, 0.592157, 0.596078, 0.600000, 0.603922, 0.607843, 0.611765, 0.615686, 0.619608, 0.623529, 
0.627451, 0.631373, 0.635294, 0.639216, 0.643137, 0.647059, 0.650980, 0.654902, 0.658824, 0.662745, 0.666667, 0.670588, 0.674510, 0.678431, 0.682353, 0.686275, 
0.690196, 0.694118, 0.698039, 0.701961, 0.705882, 0.709804, 0.713725, 0.717647, 0.721569, 0.725490, 0.729412, 0.733333, 0.737255, 0.741176, 0.745098, 0.749020, 
0.752941, 0.756863, 0.760784, 0.764706, 0.768627, 0.772549, 0.776471, 0.780392, 0.784314, 0.788235, 0.792157, 0.796078, 0.800000, 0.803922, 0.807843, 0.811765, 
0.815686, 0.819608, 0.823529, 0.827451, 0.831373, 0.835294, 0.839216, 0.843137, 0.847059, 0.850980, 0.854902, 0.858824, 0.862745, 0.866667, 0.870588, 0.874510, 
0.878431, 0.882353, 0.886275, 0.890196, 0.894118, 0.898039, 0.901961, 0.905882, 0.909804, 0.913725, 0.917647, 0.921569, 0.925490, 0.929412, 0.933333, 0.937255, 
0.941176, 0.945098, 0.949020, 0.952941, 0.956863, 0.960784, 0.964706, 0.968627, 0.972549, 0.976471, 0.980392, 0.984314, 0.988235, 0.992157, 0.996078, 1.000000
};

#ifndef PI
 #define PI 3.141592653589793f
#endif

#define _PI	3.1415926f											// Value of PI
#define _BITS_PER_PIXEL_32	32									// 32-bit color depth
#define _BITS_PER_PIXEL_24	24									// 24-bit color depth
#define _PIXEL	DWORD											// Pixel
#define _RGB(r,g,b)	(((r) << 16) | ((g) << 8) | (b))			// Convert to RGB
#define _GetRValue(c)	((BYTE)(((c) & 0x00FF0000) >> 16))		// Red color component
#define _GetGValue(c)	((BYTE)(((c) & 0x0000FF00) >> 8))		// Green color component
#define _GetBValue(c)	((BYTE)((c) & 0x000000FF))				// Blue color component
#define _GetAValue(c)	((BYTE)(((c) & 0xFF000000) >> 24))		// Alpha color component
#define _NOISE_WIDTH	192
#define _NOISE_HEIGHT	192
#define _NOISE_DEPTH	64

typedef long fixed;												// Our new fixed point type
#define itofx(x) ((x) << 8)										// Integer to fixed point
#define ftofx(x) (long)((x) * 256)								// Float to fixed point
#define dtofx(x) (long)((x) * 256)								// Double to fixed point
#define fxtoi(x) ((x) >> 8)										// Fixed point to integer
#define fxtof(x) ((float) (x) / 256)							// Fixed point to float
#define fxtod(x) ((double)(x) / 256)							// Fixed point to double
#define Mulfx(x,y) (((x) * (y)) >> 8)							// Multiply a fixed by a fixed
#define Divfx(x,y) (((x) << 8) / (y))							// Divide a fixed by a fixed

enum ScaleMode
	{
	ScaleMode_NearestNeghbore = 1,
	ScaleMode_Bilinier
	};

// ZqrTalent Specific.
#define ALPHA(alpha, src, dest) \
	((BYTE)(dest* (alpha/255.0) + src* (1.0-(alpha/255.0)))

#define ALPHA_RGB_TO_RGB(alpha, rSrc, gSrc, bSrc, rDest, gDest, bDest)				\
						((COLORREF)(ALPHA(alpha, rSrc, rDest)) |					\
						((WORD)	ALPHA(alpha, gSrc, gDest)) << 8)) |					\
						(((DWORD)ALPHA(alpha, bSrc, bDest)) << 16)))				\

#define ALPHA_EX(alpha, src, dest) \
	((BYTE)(dest*(dAlpha[alpha]) + src*(dAlpha[255-alpha]))

#define ALPHA_RGB_TO_RGB_EX(alpha, rSrc, gSrc, bSrc, rDest, gDest, bDest)			\
						((COLORREF)(ALPHA_EX(alpha, rSrc, rDest)) |					\
						((WORD)	ALPHA_EX(alpha, gSrc, gDest)) << 8)) |				\
						(((DWORD)ALPHA_EX(alpha, bSrc, bDest)) << 16)))				\


typedef class ImageHelper ImgHlp;

#include <atlimage.h>
#include "..\Array\AutoSortedArray.h"

class ImageHelper
{
public:
	// Constructor / Destructor
	ImageHelper();
	virtual ~ImageHelper();

	// Public attributes
public:

	// Public methods
public:
	static BOOL		LoadImage32bpp				(const CString sImageFile, CBitmap& bmImage);
	static CImage*	LoadImage					(CString sImageFile, int nWidth = -1, int nHeight = -1, bool bConvert32bpp = true);

	static BOOL		ConvertToBitmap32bpp		(CBitmap& bmImage);
	static BOOL		ConvertToBitmap24bpp		(CBitmap& bmImage, CBitmap* pCopyInto = NULL);

	static BOOL		SaveBitmap					(const CString sImageFile, HBITMAP hImage, int nBPP);
	static BOOL		GetBitmapFromDC				(int nBPP, CBitmap& bmImage, CDC* pSrc, UINT xSrc, UINT ySrc, UINT nWidth, UINT nHeight, CDC** pDCBitmap = NULL);

	static CBitmap*	GetTempBitmap(){
		static CBitmap _bmpTemp;
		if( !_bmpTemp.m_hObject ){
			BITMAPINFOHEADER	bmpInfo32;
			memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
			bmpInfo32.biBitCount		= 24;
			bmpInfo32.biCompression		= BI_RGB;
			bmpInfo32.biPlanes			= 1;
			bmpInfo32.biHeight			= 1;
			bmpInfo32.biSize			= sizeof(BITMAPINFOHEADER);
			bmpInfo32.biWidth			= 1;
		
			CDC	memDC;
			memDC.CreateCompatibleDC(NULL);
			UINT*	lpMap				= NULL;
			HBITMAP hDib				= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, (void**)&lpMap, NULL, 0);
			ASSERT( lpMap );
			memDC.DeleteDC();

			if( hDib )
				_bmpTemp.Attach(hDib);
			}	
		return &_bmpTemp;
		}

	/*
	static AutoSortedArray* GetArrayCreatedDibImages()
	{
		static AutoSortedArray _arrCreatedDibs;
		return &_arrCreatedDibs;
	}*/

	static bool		GetSelectedDibImageFromDC	(CDC* pDC, HBITMAP& hDibImage);

	static BOOL		CreateDIBBitmap				(int nBPP, CBitmap& bmImage, UINT* pColorTable, UINT nWidth, UINT nHeight, BITMAP* pBmp = NULL);
	static BOOL		CreateDIBBitmap				(int nBPP, CBitmap& bmImage, COLORREF crFill, UINT nWidth, UINT nHeight, BITMAP* pBmp = NULL, CDC* pDCCompatible = NULL);
	static bool		GetImageFromImage			(CImage* pSrcImage, CRect rcDest, CImage& image);

	static BOOL		AlphaColorOnBitmap			(HBITMAP hBmpDest, int xDest, int yDest, int nDrawWidth, int nDrawHeight, COLORREF crForeground, BYTE btAlpha);

	static CImage*	CombineImages				(CImage* pImageBg, CImage* pImageFg, bool bFgIsAlphaChannelImage, int nCombinedImageBPP = 24, CRect rcForeground = CRect(0, 0, 0, 0));
	static bool		CopyAlphaChannel			(HBITMAP hDest, HBITMAP hSrcCopyFrom, CRect rcSrc = CRect(0, 0, 0, 0), bool bCheckDestPixelAlpha = false, BYTE btMaxDestPixelAlphaChange = 255, int nSrcAlphaPixel = -1);

	//Desc:  Renders one image on another and calculates alpha channel of combined image.
	static HBITMAP  CombineTwo32bppImages		(HBITMAP hImage1, HBITMAP hImage2);

	static bool		RenderImage_OmitBorder		(CDC* pDC, const CRect rcClipDestDC, const CRect rcDestDC, CImage* pImageSrc, CRect rcOmitBorder, 
												BOOL bAlpha = TRUE, BOOL bSkipMiddleCenterPart = FALSE);

	static bool		RenderImage					(CDC* pDC, const CRect rcClipDestDC, const CRect rcDestDC, CImage* pImageSrc, bool bScale = true, bool bUseAlphaChannel = true, 
												ScaleMode mode = ScaleMode::ScaleMode_NearestNeghbore, HBITMAP* pImageRendered = NULL, int nAlphaConstant = 255, bool bDontAffectAlphaChannel = false);

	static BOOL		ScaleBitmap					(ScaleMode mode, HBITMAP hBmpSrc, UINT nDestCX, UINT nDestCY, CRect rcInvalidDest, HBITMAP& hBmpDest, bool bUseAlphaChannel, BYTE btConstantAlpha = 255, bool bDontAffectAlphaChannel = false);
	static BOOL		ScaleBitmap_NearestN		(HBITMAP hBmpSrc, UINT nDestCX, UINT nDestCY, CRect rcInvalidDest, HBITMAP& hBmpDest, bool bUseAlphaChannel, BYTE btConstantAlpha = 255, bool bDontAffectAlphaChannel = false);
	static BOOL		ScaleBitmap_Bilinier		(HBITMAP hBmpSrc, UINT nDestCX, UINT nDestCY, CRect rcInvalidDest, HBITMAP& hBmpDest, bool bUseAlphaChannel, BYTE btConstantAlpha = 255, bool bDontAffectAlphaChannel = false);

	static bool		MultiplyBlendImageBpp32		(HBITMAP hImage, COLORREF crBlend);

	static BOOL		BitmapOnBitmap				(HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
												int nDrawWidth, int nDrawHeight, BOOL bAlpha = FALSE, BYTE btAlpha = 255, BOOL bIgnoreFullAlphaPixel = FALSE, bool bDontAffectAlphaChannel = false);

	static BOOL		BitmapOnBitmap				(BITMAP* pbmpDest, UINT xDest, UINT yDest, BITMAP* pbmpSrc, UINT xSrc, UINT ySrc, UINT nWidthSrc, UINT nHeightSrc, 
												UINT nDrawWidth, UINT nDrawHeight, BOOL bAlpha = FALSE,  BYTE btAlpha = 255, BOOL bIgnoreFullAlphaPixel = FALSE, bool bDontAffectAlphaChannel = false);

	static BOOL		Rotate90Bitmap32bpp			(HBITMAP hBmpDest, CImage& imageRotated);

	static bool		DrawImage32bpp_Omitborder	(CDC* pDC, /*Destination DC.*/ CImage* pImageSrc, CRect rcDestDC, CRect rcClipDC, CRect rcOmitBorder, BOOL bAlpha = TRUE, BOOL bSkipMiddleCenterPart = FALSE);

	static BOOL		DrawBitmap32bppClip			(CBitmap& imgDest, CBitmap& imgSrc, CRect& rcDest, CRect& rcClip, BOOL bAlpha = FALSE, CRect* pRcOmitBorder = NULL);

	// Region from bitmap.
	static	HRGN	GetBitmap32bppRgnByAlphaChannel	(HBITMAP hbmImage, BYTE btMinAlpha = 255);


	// Soften filter.
	static bool		SetSoftenFilter				(HBITMAP hbm);

	// Text drawing.
	static wchar_t*	GetTemporaryUnicodeArray(){
		static wchar_t wszTemp[1024];
		wszTemp[0] = 0;
		return wszTemp;
		};

	static int		UTF8ToUnicode				(CString* pString, wchar_t* pwszUnicode, int nLen);
	static int		UnicodeToUTF8				(wchar_t* pwszUnicode, int nLen, CString& pString);
	static bool		GetTextSize					(CString* pStr, CFont* pFont, CSize& szText);
	static int		GetSymbolIndexByXOffset		(CString* pStr, CFont* pFont, int nXTextStart,  int nXSymbolAt);
	static int		GetSymbolWidthArray			(CString* pStr, CFont* pFont, CDWordArray& arrSymbolWidth);

	static bool		GetTextSizeEx				(CString* pStr, CDC* pDC, CSize& szText);
	static bool		GetTextSizeW				(HDC hDC, wchar_t* pwszText, int nLen, CSize& szText);
	static bool		GetTextExtentPoint32		(HDC hDC, CString* pText, int nStrLen, CSize& szText);
	static	void	DrawText					(CDC* pDC, CFont* pFont, int x, int y, int flag, CRect rcClipRect, TCHAR* pszText, float fStretchCX, float fStretchCY);
	static	bool	StretchedTextFont			(CFont* pFontIn, CFont* pFontOut, float fStretchCX, float fStretchCY, bool bUnderLine = false, bool bBold = false);
	static  bool	ExtTextOut					(CDC* pDC, int x, int y, UINT nOptions, const RECT* lpRect, CString sText);
	static  bool	ExtTextOutW					(CDC* pDC, int x, int y, UINT nOptions, const RECT* lpRect, wchar_t* pwsText, int nLen);
	static	bool	IsEmbedeedFontAvailable		(CFont* pFont);

protected:
	// Usefull to retreive selected font object from DC.
	static CFont*	GetFontTemp(){
		static CFont _fontTemp;
		if( !_fontTemp.m_hObject )
			_fontTemp.CreatePointFont(10, _T("Arial"));
		return &_fontTemp;
		}

protected:
	// Gaussian blur.
	static int		gen_convolve_matrix (float radius, float **cmatrix_p);
	static float*	gen_lookup_table	(float *cmatrix, int cmatrix_length);
	static void		blur_line			(float *ctable, float *cmatrix, int cmatrix_length, BYTE* cur_col, BYTE* dest_col, int y, long bytes);
	static bool		Image_SetCol		(BITMAP bmDesc, int x, BYTE* src_col);
	static bool		Image_GetCol		(BITMAP bmDesc, int x, BYTE* src_col);
	static bool		Image_SetRow		(BITMAP bmDesc, int y, BYTE* src_row);
	static bool		Image_GetRow		(BITMAP bmDesc, int y, BYTE* src_row);
	static bool		GetPixel			(BITMAP bmImage, int x, int y, RGBQUAD& c);
	static bool		SetPixel			(BITMAP bmImage, int x, int y, RGBQUAD& c);

	// Protected methods
public:

	static BYTE* GetColorByteByAlphaByteArray(){
		static BYTE g_btArray[256][256];
		static BOOL	g_bInited;
		if( !g_bInited ){ // Initialize array.
			for(int c=0; c<=255; c++){
				for(int a=0; a<=255; a++){
					g_btArray[c][a] = (BYTE)(dAlpha[a]*c);
					}
				}
			g_bInited = TRUE;
			}
		return &g_btArray[0][0];
		}
};