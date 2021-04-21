///////////////////////////////////////////////////////////////////////////////
//
// Draw.h : header file
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
typedef DWORD HLSCOLOR;
#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))

///////////////////////////////////////////////////////////////////////////////
#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls)>>16))

///////////////////////////////////////////////////////////////////////////////
HLSCOLOR RGB2HLS (COLORREF rgb);
COLORREF HLS2RGB (HLSCOLOR hls);

///////////////////////////////////////////////////////////////////////////////
// Performs some modifications on the specified color : luminance and saturation
COLORREF HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S);

///////////////////////////////////////////////////////////////////////////////
HBITMAP WINAPI GetScreenBitmap (LPCRECT pRect);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBufferDC : public CDC
{
    HDC     m_hDestDC;
    CBitmap m_bitmap;     // Bitmap in Memory DC
    _Rect   m_rect;
    HGDIOBJ m_hOldBitmap; // Previous Bitmap

public:
    CBufferDC (HDC hDestDC, const _Rect& rcPaint = _Rect(0,0,0,0));
   ~CBufferDC ();
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CPenDC
{
protected:
    CPen m_pen;
    HDC m_hDC;
    HPEN m_hOldPen;

public:
    CPenDC (HDC hDC, COLORREF crColor = CLR_NONE);
   ~CPenDC ();

    void Color (COLORREF crColor);
    COLORREF Color () const;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBrushDC
{
protected:
    CBrush m_brush;
    HDC m_hDC;
    HBRUSH m_hOldBrush;

public:
    CBrushDC (HDC hDC, COLORREF crColor = CLR_NONE);
   ~CBrushDC ();

    void Color (COLORREF crColor);
    COLORREF Color () const;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class FontDC
{
protected:
    HFONT		m_hFont;
    HDC			m_hDC;
    HFONT		m_hDefFont;
    COLORREF	m_crTextOld;
	int			m_nFontHeight;

public:
    FontDC (HDC hDC, LPCTSTR sFaceName, int nFontHeight, COLORREF crText = CLR_DEFAULT);
    FontDC (HDC hDC, BYTE nStockFont, COLORREF crText = CLR_DEFAULT);
    FontDC (HDC hDC, HFONT hFont, COLORREF crText = CLR_DEFAULT);
   ~FontDC ();

    const FontDC& operator = (LPCTSTR sFaceName);
    const FontDC& operator = (BYTE nStockFont);
    const FontDC& operator = (HFONT hFont);
    const FontDC& operator = (COLORREF crText);
    operator LPCTSTR ();
    operator COLORREF ();
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBoldDC
{
protected:
    Font m_fontBold;
    HDC m_hDC;
    HFONT m_hDefFont;

public:
    CBoldDC (HDC hDC, bool bBold);
   ~CBoldDC ();
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// CDrawButton types
//

#ifndef DB_EMPTY
#define DB_EMPTY       0x0000 // Empty button
#endif

#define DB_UP          0x0001 // Up arrow
#define DB_DOWN        0x0002 // Down arrow
#define DB_LEFT        0x0003 // Left arrow
#define DB_RIGHT       0x0004 // Right arrow
#define DB_UPDOWN      0x0005 // Up and down arrows
#define DB_LEFTRIGHT   0x0006 // Left and right arrow
#define DB_3POINTS     0x0007 // Three points (assistant, more, ...)
#define DB_CROSS       0x0008 // Cross like small close button
// CDrawButton styles
#define DB_ENABLED     0x0000 // Enabled button(s)   [Default]
#define DB_DISABLED    0x0100 // Disabled button(s)
#define DB_BORDER      0x0200 // Buttons have border on left and top
#define DB_WINDOWDC    0x0400 // Positions are in screen coordinates
#define DB_FLAT        0x0800 // Office 2000 look & feel
#define DB_PRESSED     0x1000 // First button is pressed
#define DB_PRESSED2    0x2000 // Second button is pressed (if exists)
#define DB_OVER        0x4000 // Mouse is over button
#define DB_TRANSPARENT 0x8000 // Don't draw background (flat button)

#define DB_DEFAULT DB_EMPTY

///////////////////////////////////////////////////////////////////////////////
class CDrawButton
{
protected:
    DWORD m_wStyle;
    _Rect m_Rect;

public:
    CDrawButton (DWORD wStyle = DB_EMPTY);
    CDrawButton (DWORD wStyle, LPCRECT pRect);

    virtual void Draw (CDC* pDC, DWORD wStyle = DB_DEFAULT) const;

    DWORD Click (CWnd* pWnd, CPoint pt, UINT nIDRepeat = 0) const;

    void SetRect (LPCRECT pRect);
    bool PtInRect (POINT pt) const;

    void CheckForMouseOver (CWnd* pWnd, _Point pt);
};

///////////////////////////////////////////////////////////////////////////////
inline CDrawButton::CDrawButton (DWORD wStyle) :
    m_wStyle (wStyle), m_Rect (0, 0, 0, 0)
{
}

///////////////////////////////////////////////////////////////////////////////
inline CDrawButton::CDrawButton (DWORD wStyle, LPCRECT pRect) :
    m_wStyle (wStyle), m_Rect (pRect)
{
}

///////////////////////////////////////////////////////////////////////////////
inline void CDrawButton::SetRect (LPCRECT pRect)
{
    m_Rect = pRect;
}

///////////////////////////////////////////////////////////////////////////////
inline bool CDrawButton::PtInRect (POINT pt) const
{
    return m_Rect.PtInRect (pt) != 0;
}
