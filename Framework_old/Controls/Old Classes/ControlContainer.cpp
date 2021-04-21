// ControlContainer.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ControlContainer.h"
#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ControlContainer

ControlContainer::ControlContainer() : MercuryBaseWnd()
	{
	}

ControlContainer::~ControlContainer()
	{
	DestroyClassMembers();
	}

BEGIN_MESSAGE_MAP(ControlContainer, MercuryBaseWnd)
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

BOOL 
ControlContainer::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/)
	{
	if( RegisterWndClass(_T("ControlContainer")) ){
		BOOL bRet =  CWnd::Create(_T("ControlContainer"), _T(""), dwStyle, rect, pParentWnd, nID, pContext);
		if( bRet ){
			}
		return bRet;
		}
	return FALSE;
	}

void
ControlContainer::DestroyClassMembers()
	{
	}

BOOL 
ControlContainer::RegisterWndClass(LPCTSTR lpszClassName)
	{
	WNDCLASS wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASS));

	HINSTANCE hInst = AfxGetInstanceHandle();
	if( !GetClassInfo(hInst, lpszClassName, &wndClass) ){
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hbrBackground	= (HBRUSH)NULL;
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon			= (HICON)NULL;
		wndClass.hInstance		= hInst;
		wndClass.lpfnWndProc	= ::DefWindowProc;
		wndClass.lpszClassName	= lpszClassName;
		wndClass.lpszMenuName	= NULL;
		wndClass.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
		return AfxRegisterClass(&wndClass);
		}
	return TRUE;
	}

void
ControlContainer::OnPrePaint(CRect rcRepaint)
	{
	}

void 
ControlContainer::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient)
	{
	CRect	rcClient;
	GetClientRectMy(rcClient);
	}

UINT 
ControlContainer::OnGetDlgCode()
	{
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}