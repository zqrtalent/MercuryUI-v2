#include "stdafx.h"
#include "GridTheme.h"

#include "Aqua.h"
using namespace Aqua;

//===========================================================================
// CGridCtrlTheme class
//===========================================================================

static JumpOperation  m_jmpHook1;
static JumpOperation  m_jmpReal1;
static PROC           m_funcToHook1;

LRESULT __stdcall	HookDrawThemeBackground	(UINT hTheme, HDC hDC, int iPartId, int iStateId, const RECT* pRect, const RECT* pRect1);
BOOL				SetFunctionHook		(PROC funcDest, PROC funcHook, JumpOperation& jmpOp);

LRESULT __stdcall 
HookDrawThemeBackground(UINT hTheme, HDC hDC, int iPartId, int iStateId, const RECT* pRect, const RECT* pRect1)
	{
	register JumpOperation* pJmp;
	// Check if window is ownerdrawed control. ############## 
	HWND hWnd = WindowFromDC(hDC);
	if( hWnd )
		{
		BOOL bSkip = (BOOL)GetProp(hWnd, "OwnerDrawWindow");
		if( bSkip )
			return TRUE;
		}
	// ######################################################
	
	// turn back hook operation.///////////////
	pJmp				= (JumpOperation*)m_funcToHook1;
	pJmp->bJmpCode      = m_jmpReal1.bJmpCode;
	pJmp->lJmpOffset    = m_jmpReal1.lJmpOffset;
	//-----------------------------------------

	LRESULT lRet = 0L;

	__asm // call Uxtheme.dll::DrawThemeBackground function.
		{
		push pRect1;
		push pRect;
		push iStateId;
		push iPartId;
		push hDC;
		push hTheme;
		mov  eax, m_funcToHook1;
		call eax;
		mov  lRet, eax; // save returned value.
		};
	
	// write hook jmp./////////////////////////
	pJmp				= (JumpOperation*)m_funcToHook1;
	pJmp->bJmpCode      = m_jmpHook1.bJmpCode;
	pJmp->lJmpOffset    = m_jmpHook1.lJmpOffset;
	//-----------------------------------------
	return lRet;
	}


BOOL              
SetFunctionHook(PROC funcDest, PROC funcHook, JumpOperation& jmpOp)
	{
	// Change protection option for memory block.
	MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(funcDest, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
    VERIFY(VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect));
		
	// write hook function jmp instruction.//
	((JumpOperation*)funcDest)->bJmpCode   = 0xE9;
	((JumpOperation*)funcDest)->lJmpOffset = long(funcHook) - long(sizeof(JumpOperation) + long(funcDest));
	//---------------------------------------

	// save hook function jump instruction.//
	jmpOp.bJmpCode                         = ((JumpOperation*)funcDest)->bJmpCode;
	jmpOp.lJmpOffset                       = ((JumpOperation*)funcDest)->lJmpOffset;
	//---------------------------------------
    return TRUE;
	}

CGridCtrlTheme::CGridCtrlTheme()
	{
	m_funcToHook1 = (PROC)GetProcAddress(LoadLibrary(_T("uxtheme.dll")), _T("DrawThemeBackground"));
	// save function header code before not changed.
	memcpy(&m_jmpReal1, (void*)m_funcToHook1, sizeof(JumpOperation));
	SetFunctionHook(m_funcToHook1, (PROC)HookDrawThemeBackground, m_jmpHook1);
	}

CGridCtrlTheme::~CGridCtrlTheme()
	{
	}

void 
CGridCtrlTheme::LoadTheme()
	{
	FreeThemeResources();

	// load horizontal scroll resources. =======================
	m_hArrowLeftH[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowLeftNormal,   g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[0]);
	m_hArrowLeftH[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowLeftHot,      g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[1]);
	m_hArrowLeftH[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowLeftPressed,  g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[2]);
	m_hArrowLeftH[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowLeftDisabled, g_nCX_HArrowLeft, g_nCY_HArrowLeft, &m_bmpArrowLeftH[3]);

	m_hThumbLeftH[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbLeftNormal,   g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[0]);
	m_hThumbLeftH[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbLeftHot,      g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[1]);
	m_hThumbLeftH[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbLeftPressed,  g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[2]);
	m_hThumbLeftH[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbLeftDisabled, g_nCX_HThumbLeft, g_nCY_HThumbLeft, &m_bmpThumbLeftH[3]);

	m_hThumbCenterH[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbCenterNormal,   g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[0]);
	m_hThumbCenterH[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbCenterHot,      g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[1]);
	m_hThumbCenterH[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbCenterPressed,  g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[2]);
	m_hThumbCenterH[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbCenterDisabled, g_nCX_HThumbCenter, g_nCY_HThumbCenter, &m_bmpThumbCenterH[3]);

	m_hThumbRightH[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbRightNormal,   g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[0]);
	m_hThumbRightH[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbRightHot,      g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[1]);
	m_hThumbRightH[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbRightPressed,  g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[2]);
	m_hThumbRightH[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HThumbRightDisabled, g_nCX_HThumbRight, g_nCY_HThumbRight, &m_bmpThumbRightH[3]);

	m_hArrowRightH[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowRightNormal,   g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[0]);
	m_hArrowRightH[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowRightHot,      g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[1]);
	m_hArrowRightH[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowRightPressed,  g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[2]);
	m_hArrowRightH[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HArrowRightDisabled, g_nCX_HArrowRight, g_nCY_HArrowRight, &m_bmpArrowRightH[3]);
	
	m_hBackgroundH[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HBkgndNormal,   g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[0]);
	m_hBackgroundH[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HBkgndHot,      g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[1]);
	m_hBackgroundH[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HBkgndPressed,  g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[2]);
	m_hBackgroundH[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_HBkgndDisabled, g_nCX_HBkgnd, g_nCY_HBkgnd, &m_bmpBackgroundH[3]);
	//=================================================================

	// load vertical scroll resources. ================================
	m_hArrowTopV[0]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowTopNormal,   g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[0]);
	m_hArrowTopV[1]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowTopHot,      g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[1]);
	m_hArrowTopV[2]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowTopPressed,  g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[2]);
	m_hArrowTopV[3]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowTopDisabled, g_nCX_VArrowTop, g_nCY_VArrowTop, &m_bmpArrowTopV[3]);

	m_hThumbTopV[0]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbTopNormal,   g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[0]);
	m_hThumbTopV[1]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbTopHot,      g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[1]);
	m_hThumbTopV[2]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbTopPressed,  g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[2]);
	m_hThumbTopV[3]		= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbTopDisabled, g_nCX_VThumbTop, g_nCY_VThumbTop, &m_bmpThumbTopV[3]);

	m_hThumbCenterV[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbCenterNormal,   g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[0]);
	m_hThumbCenterV[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbCenterHot,      g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[1]);
	m_hThumbCenterV[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbCenterPressed,  g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[2]);
	m_hThumbCenterV[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbCenterDisabled, g_nCX_VThumbCenter, g_nCY_VThumbCenter, &m_bmpThumbCenterV[3]);

	m_hThumbBottomV[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbBottomNormal,   g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[0]);
	m_hThumbBottomV[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbBottomHot,      g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[1]);
	m_hThumbBottomV[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbBottomPressed,  g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[2]);
	m_hThumbBottomV[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VThumbBottomDisabled, g_nCX_VThumbBottom, g_nCY_VThumbBottom, &m_bmpThumbBottomV[3]);

	m_hArrowBottomV[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowBottomNormal,   g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[0]);
	m_hArrowBottomV[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowBottomHot,      g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[1]);
	m_hArrowBottomV[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowBottomPressed,  g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[2]);
	m_hArrowBottomV[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VArrowBottomDisabled, g_nCX_VArrowBottom, g_nCY_VArrowBottom, &m_bmpArrowBottomV[3]);
	
	m_hBackgroundV[0]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VBkgndNormal,   g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[0]);
	m_hBackgroundV[1]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VBkgndHot,      g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[1]);
	m_hBackgroundV[2]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VBkgndPressed,  g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[2]);
	m_hBackgroundV[3]	= CBitmapEx::CreateBitmap32bpp((UINT*)&ct_VBkgndDisabled, g_nCX_VBkgnd, g_nCY_VBkgnd, &m_bmpBackgroundV[3]);
	//=================================================================

	// create arrow regions. ==================
	if( !CBitmapEx::RegionFromAlphaData(m_hArrowLeftH[0], m_rgnArrowLeft) )
		m_rgnArrowLeft.CreateRectRgn(0, 0, m_bmpArrowLeftH[0].bmWidth, m_bmpArrowLeftH[0].bmHeight);

	if( !CBitmapEx::RegionFromAlphaData(m_hArrowRightH[0], m_rgnArrowRight) )
		m_rgnArrowRight.CreateRectRgn(0, 0, m_bmpArrowRightH[0].bmWidth, m_bmpArrowRightH[0].bmHeight);

	if( !CBitmapEx::RegionFromAlphaData(m_hArrowTopV[0], m_rgnArrowTop) )
		m_rgnArrowTop.CreateRectRgn(0, 0, m_bmpArrowTopV[0].bmWidth, m_bmpArrowTopV[0].bmHeight);

	if( !CBitmapEx::RegionFromAlphaData(m_hArrowBottomV[0], m_rgnArrowBottom) )
		m_rgnArrowBottom.CreateRectRgn(0, 0, m_bmpArrowBottomV[0].bmWidth, m_bmpArrowBottomV[0].bmHeight);
	// ========================================

	m_nCY_HScroll	= g_nHScrollCY;
	m_nCX_HArrowMin = g_nHArrowCXMin;
	m_nCX_HArrowMax = g_nHArrowCXMax;
	m_nCX_VScroll   = g_nVScrollCX;
	m_nCY_VArrowMin = g_nVArrowCYMin;
	m_nCY_VArrowMax = g_nVArrowCYMax;
	m_crSizeGripper	= RGB(196, 196, 196);
	m_crGridBorder	= RGB(102, 102, 102);
	m_nCY_ThumbMin  = m_bmpThumbBottomV[0].bmHeight + 4 + m_bmpThumbTopV[0].bmHeight;
	m_nCX_ThumbMin  = m_bmpThumbLeftH[0].bmWidth    + 4 + m_bmpThumbRightH[0].bmWidth;
	}

void
CGridCtrlTheme::FreeThemeResources()
	{
	// ======= Scroll Resources. ==================================
	HBITMAP* hBmpList[]   = {(HBITMAP*)&m_hArrowLeftH,		(HBITMAP*)&m_hThumbLeftH,  
							 (HBITMAP*)&m_hThumbCenterH,	(HBITMAP*)&m_hThumbRightH, 
						     (HBITMAP*)&m_hArrowRightH,		(HBITMAP*)&m_hBackgroundH, 
						     (HBITMAP*)&m_hArrowTopV,		(HBITMAP*)&m_hThumbTopV,
							 (HBITMAP*)&m_hThumbCenterV,	(HBITMAP*)&m_hThumbBottomV,
							 (HBITMAP*)&m_hArrowBottomV,	(HBITMAP*)&m_hBackgroundV, NULL};

	BITMAP* pBmpObjList[] = {(BITMAP*)&m_bmpArrowLeftH,		(BITMAP*)&m_bmpThumbLeftH,
							 (BITMAP*)&m_bmpThumbCenterH,	(BITMAP*)&m_bmpThumbRightH,
							 (BITMAP*)&m_bmpArrowRightH,	(BITMAP*)&m_bmpBackgroundH, 
							 (BITMAP*)&m_bmpArrowTopV,		(BITMAP*)&m_bmpThumbTopV,    
							 (BITMAP*)&m_bmpThumbCenterV,	(BITMAP*)&m_bmpThumbBottomV, 
							 (BITMAP*)&m_bmpArrowBottomV,	(BITMAP*)&m_bmpBackgroundV, NULL};
	int		nLoop		= 0;
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
	}
