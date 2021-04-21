// ThemeButton.cpp : implementation file
//

#include "stdafx.h"
#include "ThemeButton.h"


// ThemeButton
IMPLEMENT_DYNAMIC(ThemeButton, CButton)

ThemeButton::ThemeButton() :
m_tracking(false),
m_erase_bkg(false),
m_inited(false)
{}

ThemeButton::~ThemeButton(){
	}

BEGIN_MESSAGE_MAP(ThemeButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL
ThemeButton::InitControl(DrawStyle style){
	if( m_inited )  return FALSE;
	m_pTheme = GetGDIStreamByStyle(style);
	return (m_pTheme != NULL);
	}

/*
BOOL ThemeButton::InitControl(UINT idBmpList,BOOL bVertical,DWORD width,
							DWORD height,DWORD crTransparent,
							CString sFontName,DWORD drawOption,
							DWORD leftArcValue,DWORD rightArcValue,
							DWORD topArcValue,DWORD bottomArcValue)
{
	if( m_inited )  return FALSE;
	
	HBITMAP hImage = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(idBmpList),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	if( !hImage )   return FALSE;

	
	m_pGDSData = new GdiDrawStreamStruct;
	ZeroMemory(m_pGDSData,sizeof(GdiDrawStreamStruct));
	
	m_pGDSData->signature      = 0x44727753;//"Swrd"
	m_pGDSData->hImage         = (HBITMAP)hImage;
	m_pGDSData->unknown1       = 0x1;
	m_pGDSData->unknown2       = 0x9;
	m_pGDSData->drawOption     = drawOption;
	m_pGDSData->leftArcValue   = leftArcValue;
	m_pGDSData->rightArcValue  = rightArcValue;
	m_pGDSData->topArcValue    = topArcValue;
	m_pGDSData->bottomArcValue = bottomArcValue;
	m_pGDSData->crTransparent  = crTransparent;

	if( bVertical ){
		::CopyRect(&m_rcNormal,   CRect(0,0,                   width, height));
		::CopyRect(&m_rcHot,      CRect(0,m_rcNormal.bottom,   width, m_rcNormal.bottom+height));
		::CopyRect(&m_rcPressed,  CRect(0,m_rcHot.bottom,      width, m_rcHot.bottom+height));
		::CopyRect(&m_rcDissabled,CRect(0,m_rcPressed.bottom,  width, m_rcPressed.bottom+height));
		::CopyRect(&m_rcDefault,  CRect(0,m_rcDissabled.bottom,width, m_rcDissabled.bottom+height));
	}
	else{
		::CopyRect(&m_rcNormal,   CRect(0,                  0, width,                    height));
		::CopyRect(&m_rcHot,      CRect(m_rcNormal.right,   0, m_rcNormal.right+width,   height));
		::CopyRect(&m_rcPressed,  CRect(m_rcHot.right,      0, m_rcHot.right+width,      height));
		::CopyRect(&m_rcDissabled,CRect(m_rcPressed.right,  0, m_rcPressed.right+width,  height));
		::CopyRect(&m_rcDefault,  CRect(m_rcDissabled.right,0, m_rcDissabled.right+width,height));
	}

	// Create font for Button Text. {{
	LOGFONT lf;
	memset(&lf,0,sizeof(lf));
	lf.lfCharSet        = ANSI_CHARSET;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfHeight         = 14;
	lf.lfOrientation    = 1500;
	lf.lfOutPrecision   = CLIP_DEFAULT_PRECIS;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	lf.lfQuality        = PROOF_QUALITY;
	lf.lfWeight         = FW_NORMAL;
	_tcscpy(lf.lfFaceName,sFontName);
	m_fontText.CreateFontIndirect(&lf);
	// }}
	
	m_inited = TRUE;
	return m_inited;
}


BOOL ThemeButton::InitControl(UINT idBmpList,BOOL bVertical,DWORD width,
							DWORD height,DWORD crTransparent,
							CString sFontName,DWORD drawStyle)
{
	BOOL bRet = FALSE;

	switch(drawStyle)
	{
	case MAC_STYLE:
		{
			bRet = InitControl(idBmpList,bVertical,width,height,crTransparent,sFontName,
				               DTBG_CLIPRECT|DTBG_OMITBORDER,0xd,0xc,0xb,0xb);
		}
		break;
	case VISTA_STYLE:
		{
			bRet = InitControl(idBmpList,bVertical,width,height,crTransparent,sFontName,
				               DTBG_CLIPRECT|DTBG_OMITBORDER,0x8,0x8,0x8,0x8);
		}
		break;
	case XP_STYLE:
		{
			bRet = InitControl(idBmpList,bVertical,width,height,crTransparent,sFontName,
				               DTBG_CLIPRECT|DTBG_OMITBORDER,0x8,0x8,0x9,0x9);
		}
		break;
	case LUNA_LONGHORN:
		{
			bRet = InitControl(idBmpList,bVertical,width,height,crTransparent,sFontName,
				               DTBG_CLIPRECT|DTBG_OMITBORDER,0x8,0x8,0x9,0x9);
		}
		break;
	};

	return bRet;
}
*/
// ThemeButton message handlers

void 
ThemeButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{/*
	HDC     hDC    = lpDrawItemStruct->hDC;
	CDC*    pDC    = CDC::FromHandle(hDC);
	CRect   rc     = lpDrawItemStruct->rcItem;
	CString sText    (_T(""));
	
	::CopyRect(&m_pGDSData->rcClip,rc);
	::CopyRect(&m_pGDSData->rcDest,rc);
	m_pGDSData->hDC = hDC;

	if( !(lpDrawItemStruct->itemState & (ODS_DISABLED|ODS_SELECTED)) )
	{
		if( !m_tracking )
		{
			if( lpDrawItemStruct->itemState & ODS_FOCUS )
				::CopyRect(&m_pGDSData->rcSrc,m_rcDefault);
			else
				::CopyRect(&m_pGDSData->rcSrc,m_rcNormal);
		}
		else
			::CopyRect(&m_pGDSData->rcSrc,m_rcHot);
	}
	else
	{
		if( lpDrawItemStruct->itemState & ODS_SELECTED )
			::CopyRect(&m_pGDSData->rcSrc,m_rcPressed);
		if( lpDrawItemStruct->itemState & ODS_DISABLED )
			::CopyRect(&m_pGDSData->rcSrc,m_rcDissabled);
	}

	// call Gdi32::GdiDrawStream function. {{
	(*GdiDrawStreamFunc)(hDC,sizeof(GdiDrawStreamStruct),m_pGDSData);
	// }}

	// Draw Text of Button. {{
	rc.bottom     -= 2;
	GetWindowText    (sText);
	pDC->SetBkMode   (TRANSPARENT);
	pDC->SelectObject(m_fontText.m_hObject);
	pDC->DrawText    (sText,sText.GetLength(),rc,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	// }}
	*/
}

void 
ThemeButton::PreSubclassWindow(){
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
	m_parentWnd = GetParent();
	ASSERT      (m_parentWnd);
	}

void 
ThemeButton::OnLButtonDown(UINT nFlags, CPoint point){
	m_erase_bkg = true;
	Invalidate();
	CButton::OnLButtonUp(nFlags,point);
	}

void 
ThemeButton::OnLButtonUp(UINT nFlags, CPoint point){
	m_erase_bkg = true;
	Invalidate();
	CButton::OnLButtonUp(nFlags,point);
	}

void 
ThemeButton::OnMouseMove(UINT nFlags, CPoint point){
	bool  bInvalidate = false;
	DWORD dwState     = GetState();

	if( ::GetCapture() == m_hWnd ) {
	    POINT p2 = point;
        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        bool pressed      = (dwState & BST_PUSHED) == BST_PUSHED;
        bool need_pressed = (mouse_wnd == m_hWnd);

        if(pressed != need_pressed) {
            SetState(need_pressed ? TRUE : FALSE);
            bInvalidate = true;
			}
		}

	if( !m_tracking ){
        TRACKMOUSEEVENT t = {sizeof(TRACKMOUSEEVENT),TME_LEAVE,m_hWnd,0};
        if( ::_TrackMouseEvent(&t) ){
            m_tracking  = true;
			bInvalidate = true;
			}
		}

	if( bInvalidate ){
		m_erase_bkg = true;
		Invalidate();
		}
	CButton::OnMouseMove(nFlags, point);
	}

void 
ThemeButton::OnLButtonDblClk(UINT nFlags, CPoint point){
	SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
	}

void 
ThemeButton::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags){
	CButton::OnKeyDown(nChar,nRepCnt,nFlags);
	if( GetState() & BST_PUSHED ){
		m_erase_bkg = true;
		Invalidate();
		}
	}

void 
ThemeButton::OnKillFocus(CWnd* pnewWnd){
	m_erase_bkg = true;
	Invalidate();
	CButton::OnKillFocus(pnewWnd);
	}

LRESULT 
ThemeButton::OnMouseLeave(WPARAM, LPARAM){
	ASSERT(m_tracking);
    m_tracking = false;
	m_erase_bkg = true;
	Invalidate      ();
	return           0;
	}

BOOL 
ThemeButton::OnEraseBkgnd(CDC* pDC){
	if( m_erase_bkg ){
		m_erase_bkg = !m_erase_bkg;
		// transformation of child window rect in parent window rect coordinates. {{
		CRect rc;
		GetClientRect (rc);
		ClientToScreen(rc);
		m_parentWnd->ScreenToClient(rc);
		// }}
		// Invalidate parent window rect.
		m_parentWnd->InvalidateRect(rc);
		}
	return TRUE;
	}
