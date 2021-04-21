//
//  PlatformCompat.h
//  TestApp
//
//  Created by Zaqro Butskhrikidze on 7/30/14.
//  Copyright (c) 2014 Zaqro Butskhrikidze. All rights reserved.
//

#ifndef TestApp_PlatformCompat_h
#define TestApp_PlatformCompat_h
#include <WinSock2.h>
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include <Mstcpip.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

//#define MFCSTRING

#ifdef ASSERT
#undef ASSERT
#endif
#include <assert.h>

#ifndef BOOL
	#define BOOL int
#endif
	
#ifndef HANDLE
	typedef void* HANDLE;
#endif

#ifndef UINT
	#define UINT unsigned int
#endif

#ifndef INT_CHIPS
//#define INT_CHIPS unsigned int
#define INT_CHIPS __int64
#endif

#ifndef DWORD
	#define DWORD unsigned long
#endif

#ifndef ASSERT
	#define ASSERT(x) assert(x)
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef BYTE
	#define BYTE unsigned char
#endif

#ifndef LPCTSTR
	#define LPCTSTR const TCHAR*
#endif

#define ASSERT(x) assert(x)

#if defined(__x86_64__) || defined(_M_X64)
/* x86 64-bit ----------------------------------------------- */
#define PTR_HOLDER_INTEGER int64_t

#elif defined(__i386) || defined(_M_IX86)
/* x86 32-bit ----------------------------------------------- */
#define PTR_HOLDER_INTEGER long
#endif

//#ifndef _T
//#define _T(x) x
//#endif

#ifndef TCHAR
	#ifdef _UNICODE
		#define TCHAR wchar_t
	#else
		#define TCHAR char
	#endif
#endif

#define SQLSMALLINT short

// Rect
typedef tagRECT		RECTDef;
#define LPRECTDef RECTDef*
#define LPCRECTDef const RECTDef*

// Point
typedef tagPOINT	POINTDef;
#define LPPOINTDef POINTDef*
#define LP_PointDef const POINTDef*

/*
typedef struct tagPOINTF{
    FLOAT x;
    FLOAT y;
    } 	POINTF;
*/
// Size
typedef tagSIZE		SIZEDef;
#define LPSIZEDef	SIZEDef*
#define LPCSIZEDef const SIZEDef*

// Region
#define RGNDef HRGN

// Device Context Handle
typedef HDC DCDef;
// Font Object Handle
typedef HFONT FONTDef;
// Image Object Handle
typedef HBITMAP ImageDef;

/*
typedef struct tagLOGFONTA
{
    LONG      lfHeight;
    LONG      lfWidth;
    LONG      lfEscapement;
    LONG      lfOrientation;
    LONG      lfWeight;
    BYTE      lfItalic;
    BYTE      lfUnderline;
    BYTE      lfStrikeOut;
    BYTE      lfCharSet;
    BYTE      lfOutPrecision;
    BYTE      lfClipPrecision;
    BYTE      lfQuality;
    BYTE      lfPitchAndFamily;
    CHAR      lfFaceName[LF_FACESIZE];
} LOGFONTA, *PLOGFONTA, NEAR *NPLOGFONTA, FAR *LPLOGFONTA;

typedef struct RECT_tag
{
    int left;
    int top;
    int right;
    int bottom;
}RECT;

typedef struct POINT_tag
{
    int x;
    int y;
}POINT;

typedef struct SIZE_tag
{
    int cx;
    int cy;
}SIZE;
*/

#include <vector>
#include "PlatformString.h"

typedef std::vector<DWORD> CDWordVector;
typedef std::vector<_String> CStringVector;
typedef std::vector<void*> CPtrVector;

#include "PlatformSize_Win.h"
#include "PlatformRect_Win.h"
#include "PlatformPoint_Win.h"
#include "PlatformDateTime.h"
#include "PlatformLongBinary.h"
#include "PlatformFont_Win.h"

// Mutex/Synchronization
#include "PlatformCriticalSection_Win.h"

#endif
