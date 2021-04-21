#pragma once
#ifndef _UXTHEME_H_

	#define DTBG_CLIPRECT        0x00000001   // rcClip has been specified
	#define DTBG_DRAWSOLID       0x00000002   // draw transparent/alpha images as solid
	#define DTBG_OMITBORDER      0x00000004   // don't draw border of part
	#define DTBG_OMITCONTENT     0x00000008   // don't draw content area of part
	#define DTBG_COMPUTINGREGION 0x00000010   // TRUE if calling to compute region
	#define DTBG_MIRRORDC        0x00000020   // assume the hdc is mirrorred and
											  // flip images as appropriate (currently 
											  // only supported for bgtype=imagefile)
#endif

typedef struct GdiDrawStreamStruct_tag
{
	DWORD   signature;     // = 0x44727753;//"Swrd"
	DWORD   reserved;      // Zero value.
	HDC     hDC;           // handle to the device object of windiw to draw.
	RECT    rcDest;        // desination rect of window to draw.
	DWORD   unknown1;      // must be 1.
	HBITMAP hImage;        // handle to the bitmap image.
	DWORD   unknown2;      // must be 9.
	RECT    rcClip;        // desination rect of window to draw.
	RECT    rcSrc;         // source rect of bitmap to draw.
	DWORD   drawOption;    // option flag for drawing image.
	DWORD   leftArcValue;  // arc value of left side.
	DWORD   rightArcValue; // arc value of right side.
	DWORD   topArcValue;   // arc value of top side.
	DWORD   bottomArcValue;// arc value of bottom side.
	DWORD   crTransparent; // transparent color.

} GdiDrawStreamStruct, *PGdiDrawStreamStruct;

typedef  BOOL (__stdcall *GdiDrawStream)(HDC hDC, DWORD dwStructSize,PGdiDrawStreamStruct pStream);
static GdiDrawStream  GdiDrawStreamFunc = (GdiDrawStream)GetProcAddress(GetModuleHandle(_T("GDI32.DLL")),"GdiDrawStream\0");