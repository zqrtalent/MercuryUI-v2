// ESFrameBase.cpp : implementation file
//

#include "ESFrameBase.h"

// ESFrameBase dialog
ESFrameBase::ESFrameBase() {
	m_hWnd					= NULL;
//	m_hIcon					= NULL;
	m_fHeightAndWidthRatio	= 0.0f;
	m_bResizingFrame		= false;
	m_szMin			.SetSize(0, 0);
	m_szMax			.SetSize(0, 0);
	m_szDefault		.SetSize(0, 0);
	m_ptSetupPos	.SetPoint(-1, -1); // Centered by default.
	m_nHitTestCode			= 0;
	m_nBorderWidth			= 0;
	m_nCaptionHeight		= 0;
	m_sWndClass				= _T("ESFrameBase");
	m_pOwner				= NULL;
    
    m_pNSWnd                = NULL;
	}

ESFrameBase::~ESFrameBase(){
	}

int
ESFrameBase::Create(){
	ASSERT(m_hWnd == NULL);
	if( m_hWnd ){
		// Window is already created!!!!
		return 0;
		}

    if( !OnInitDialog() ){
		// Initialization of window is failed !!!
		return 0;
		}
/*
	MSG msg;
	// Main message loop:
	while ( GetMessage(&msg, NULL, 0, 0) ){
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		}
	return msg.wParam;*/
    return 0;
	}

void
ESFrameBase::ShowWindow(int nCmd){
    /*
	if( IsWindow() )
		::ShowWindow(m_hWnd, nCmd);*/
	}

void
ESFrameBase::SetActiveWindow(){
    /*
	if( IsWindow() )
		::SetActiveWindow(m_hWnd);*/
	}

void
ESFrameBase::CloseWindow(){
    /*
	if( IsWindow() )
		::PostMessage(m_hWnd, WM_CLOSE, 0L, 0L);*/
	}

void
ESFrameBase::SetOwner(ESFrameBase* pOwner){
	m_pOwner = pOwner;
	}

ESFrameBase*
ESFrameBase::GetOwner(){
	return m_pOwner;
	}

BOOL
ESFrameBase::GetClientRect(_Rect& rcClient){
    if( m_hWnd != NULL ){
        NSRect rcFrame = [m_hWnd frame];
        rcClient.SetRect(0, 0, (int)rcFrame.size.width, (int)rcFrame.size.height);
        return TRUE;
    }
    
    /*
	if( !IsWindow() )
		return FALSE;
	return ::GetClientRect(m_hWnd, rcClient);*/
    return FALSE;
	}

BOOL
ESFrameBase::GetWindowRect(_Rect& rcWindow){/*
	if( !IsWindow() )
		return FALSE;
	return ::GetWindowRect(m_hWnd, rcWindow);*/
    return FALSE;
	}

BOOL
ESFrameBase::ScreenToClient(_Point& pt){/*
	if( !IsWindow() )
		return FALSE;
	return ::ScreenToClient(m_hWnd, pt);*/
    return FALSE;
	}

BOOL
ESFrameBase::ScreenToClient(_Rect& rect){/*
	if( !IsWindow() )
		return FALSE;

	_Point pt1(rect.left, rect.top);
	_Point pt2(rect.right, rect.bottom);
	::ScreenToClient(m_hWnd, pt1);
	::ScreenToClient(m_hWnd, pt2);
	rect.SetRect(pt1.x, pt1.y, pt2.x, pt2.y);*/
	return TRUE;
	}

BOOL
ESFrameBase::ClientToScreen(_Point& pt){/*
	if( !IsWindow() )
		return FALSE;
	return ::ClientToScreen(m_hWnd, pt);*/
    return FALSE;
	}

BOOL
ESFrameBase::ClientToScreen(_Rect& rect){/*
	if( !IsWindow() )
		return FALSE;

	_Point pt1(rect.left, rect.top);
	_Point pt2(rect.right, rect.bottom);
	::ClientToScreen(m_hWnd, pt1);
	::ClientToScreen(m_hWnd, pt2);
	rect.SetRect(pt1.x, pt1.y, pt2.x, pt2.y);*/
	return TRUE;
	}

BOOL
ESFrameBase::SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT nFlags){
    /*
	if( !IsWindow() )
		return FALSE;
	return ::SetWindowPos(m_hWnd, hWndInsertAfter, X,  Y, cx, cy, nFlags);*/
    return FALSE;
	}

BOOL
ESFrameBase::ModifyStyleEx(DWORD dwStyleRemove, DWORD dwStyleAdd){
    /*
	if( !IsWindow() )
		return FALSE;
	DWORD dwStyle = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
	dwStyle &= ~dwStyleRemove;
	dwStyle |= dwStyleAdd;
	::SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, dwStyle);*/
	return TRUE;
	}

BOOL
ESFrameBase::ModifyStyle(DWORD dwStyleRemove, DWORD dwStyleAdd){/*
	if( !IsWindow() )
		return FALSE;
	DWORD dwStyle = ::GetWindowLongPtr(m_hWnd, GWL_STYLE);
	dwStyle &= ~dwStyleRemove;
	dwStyle |= dwStyleAdd;
	::SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);*/
	return TRUE;
	}

DWORD
ESFrameBase::GetStyle(){/*
	if( !IsWindow() )
		return 0L;
	return (DWORD)::GetWindowLongPtr(m_hWnd, GWL_STYLE);*/
    return 0;
	}

DWORD
ESFrameBase::GetStyleEx(){/*
	if( !IsWindow() )
		return 0L;
	return (DWORD)::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);*/
    return 0;
	}

UINT
ESFrameBase::SendMessage(UINT nMessage, WPARAM wParam, LPARAM lParam){
    return 0;/*
	if( !m_hWnd )
		return 0;
	return ::SendMessage(m_hWnd, nMessage, wParam, lParam);*/
	}

UINT
ESFrameBase::PostMessage(UINT nMessage, WPARAM wParam, LPARAM lParam){
    /*
	if( !m_hWnd )
		return 0;
	return ::PostMessage(m_hWnd, nMessage, wParam, lParam);*/
    return 0;
	}

BOOL
ESFrameBase::SetTimer(UINT_PTR nIDEvent, UINT nElapse){
    /*
	if( !m_hWnd )
		return FALSE;
	return ::SetTimer(m_hWnd, nIDEvent, nElapse, NULL);*/
    return FALSE;
	}

BOOL
ESFrameBase::KillTimer(UINT_PTR nIDEvent){
    return FALSE;/*
	if( !m_hWnd )
		return FALSE;
	return ::KillTimer(m_hWnd, nIDEvent);*/
	}

void
ESFrameBase::CloseDialog(int nRetCode){
    /*
	if( ::IsWindow(m_hWnd) )
		PostMessage(WM_CLOSE, nRetCode, 0);*/
	}

bool
ESFrameBase::LoadWindowPlacement(_Point ptPos, _Size szWindow){
    /*
	if( ptPos.x <= 0 || ptPos.y <= 0 )
		return false;

	_Rect rcWindow	(0, 0, 0, 0);
	_Rect rcDesktop	(0, 0, 0, 0);
	::GetWindowRect(::GetDesktopWindow(), rcDesktop);

	_Size	szDefault	= m_szDefault;
	_Point	ptDefault	= m_ptSetupPos;

	if( szWindow.cx > 0 && szWindow.cy > 0 ){
		m_szDefault.cx = szWindow.cx;
		m_szDefault.cy = szWindow.cy;
		}

	if( (ptPos.x < 0 || ptPos.y < 0) ){
		m_ptSetupPos.x = rcDesktop.left + (rcDesktop.Width() - m_szDefault.cx)/2;
		m_ptSetupPos.y = rcDesktop.top + (rcDesktop.Height() - m_szDefault.cy)/2;
		}
	else{
		m_ptSetupPos.x = ptPos.x;
		m_ptSetupPos.y = ptPos.y;
		}

	rcWindow.SetRect(m_ptSetupPos.x, m_ptSetupPos.y, m_ptSetupPos.x + m_szDefault.cx, m_ptSetupPos.y + m_szDefault.cy);
	if( (rcWindow&rcDesktop) != rcWindow ){
		m_szDefault		= szDefault;
		m_ptSetupPos	= ptDefault;
		}*/
	return true;
	}

_Size
ESFrameBase::GetSizeMin(){
	return _Size(m_szMin.cx + 2*m_nBorderWidth, m_szMin.cy + m_nCaptionHeight + m_nBorderWidth);
	}

_Size
ESFrameBase::GetSizeMax(){
	return _Size(m_szMax.cx + 2*m_nBorderWidth, m_szMax.cy + m_nCaptionHeight + m_nBorderWidth);
	}

_Size
ESFrameBase::GetSizeDefault(){
	return _Size(m_szDefault.cx + 2*m_nBorderWidth, m_szDefault.cy + m_nCaptionHeight + m_nBorderWidth);
	}

_Size
ESFrameBase::GetCurrentSizeOmitBorder(_Point* pWindowPos){
	_Size szRet(0, 0);
	_Rect rcWindow(0, 0, 0, 0);
	/*
	if( pWindowPos )
		pWindowPos->SetPoint(0, 0);
	
	if( !::GetWindowRect(m_hWnd, rcWindow) )
		return szRet;

	if( pWindowPos )
		pWindowPos->SetPoint(rcWindow.left, rcWindow.top);
*/
	szRet.SetSize(rcWindow.Width() - 2*m_nBorderWidth, rcWindow.Height() - m_nCaptionHeight - m_nBorderWidth);
	return szRet;
	}

void
ESFrameBase::CalcCaptionBorderSize(){
    /*
	DWORD dwStyle			= ::GetWindowLongPtr(m_hWnd, GWL_STYLE);
	int	cyFrame				= ::GetSystemMetrics(SM_CYFRAME);
	int	cyDlgFrame			= ::GetSystemMetrics(SM_CYDLGFRAME);
	int	cyBorder			= ::GetSystemMetrics(SM_CYBORDER);
	int	cyCaption			= ::GetSystemMetrics(SM_CYCAPTION);

	int	cxFrame				= ::GetSystemMetrics(SM_CXFRAME);
	int	cxDlgFrame			= ::GetSystemMetrics(SM_CXDLGFRAME);
	int	cxBorder			= ::GetSystemMetrics(SM_CXBORDER);

	m_nBorderWidth			= 0;
	m_nCaptionHeight		= 0;

	if( dwStyle&WS_BORDER )
		m_nBorderWidth		+= cyBorder;
	if( dwStyle&WS_DLGFRAME )
		m_nBorderWidth		+= cxDlgFrame;

//	m_nBorderWidth		+= cxFrame;
	m_nBorderWidth		= cxFrame;
	if( dwStyle&WS_CAPTION )
		m_nCaptionHeight += (cyFrame + cyCaption);
	else
		m_nCaptionHeight = m_nBorderWidth;*/
	}


// ESFrameBase message handlers
BOOL 
ESFrameBase::OnInitDialog(){
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
//	if( m_hIcon ){
//		SetIcon(m_hIcon, TRUE);			// Set big icon
//		SetIcon(m_hIcon, FALSE);		// Set small icon
//		}
/*
	HINSTANCE	hInstance	= GetModuleHandle(NULL);
	//HWND		hWnd		= CreateWindow(m_sWndClass.c_str(), _T(""), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	HWND		hWnd		= CreateWindow(m_sWndClass.c_str(), _T(""), WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if( !hWnd ){
		return FALSE;
		}

	m_hWnd	= hWnd;
	AttachWND(m_hWnd, this);
	
	CalcCaptionBorderSize();

	// Set default window size.
	if( m_szDefault.cx >0 && m_szDefault.cy > 0 )
		SetWindowPos(0L, 0, 0, m_szDefault.cx, m_szDefault.cy, SWP_NOMOVE);

	// Setup.
	Setup();
	// Set title.
	SetTitle();

	::ShowWindow	(hWnd, SW_NORMAL);
	//UpdateWindow	(hWnd);
*/
    
    NSRect contentRect = CGRectMake(0, 0, 200, 200);
    NSUInteger styleMask = NSWindowStyleMaskTitled|NSWindowStyleMaskClosable;
    
    // Set default window size.
    if( m_szDefault.cx >0 && m_szDefault.cy > 0 ){
        contentRect.size.width = m_szDefault.cx;
        contentRect.size.height = m_szDefault.cy;
    }
    
    // Creation of NSWindow.
    NSWindowFrame* myModal = [[NSWindowFrame alloc] initWithContentRect:contentRect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
    [myModal setReleasedWhenClosed:NO];
    
    NSWindowFrameView* modalView = [[NSWindowFrameView alloc] init];
    [myModal setContentView:modalView];
    [myModal makeKeyAndOrderFront:myModal];
    
    m_pNSWnd = myModal;
    m_hWnd	= modalView;
    AttachWND(m_hWnd, this);
    
    //CalcCaptionBorderSize();
    
    // Setup.
    Setup();
    // Set title.
    SetTitle();
    
    return TRUE;  // return TRUE  unless you set the focus to a control
	}

bool
ESFrameBase::OnRegisterWindowClass(HINSTANCE hInst, _string& wndClassName){/*
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= ESFrameBase::_WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	//wcex.hIcon		= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ESPOKERCLIENTWIN32));
	wcex.hIcon			= m_hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_ESPOKERCLIENTWIN32);
	wcex.lpszClassName	= m_sWndClass.c_str();
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hbrBackground	= 0L;
	RegisterClassEx(&wcex);

	wndClassName		= m_sWndClass;*/
	return true;
	}

DCDef
ESFrameBase::GetDC(){
    return (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
	}

void
ESFrameBase::ReleaseDC(DCDef dc){
    /*
	if( m_hWnd )
		::ReleaseDC(m_hWnd, dc);*/
	}

ESFrameBase*
ESFrameBase::FromHandle(HWND hWnd){
	AutoSortedArray* pArrWnds = GetArrayWNDAttachments();
	ASSERT(pArrWnds);
	ESFrameBase* pResult = NULL;
	int nIndex = -1;
	pArrWnds->GetData((__bridge void*)hWnd, (void*&)pResult, nIndex);
	return pResult;
	}

void
ESFrameBase::Invalidate(BOOL eraseBkgnd){
    if( m_hWnd )
        [m_hWnd setNeedsDisplay:YES];
	}

void
ESFrameBase::InvalidateRect(LPCRECTDef lpRect, BOOL eraseBkgnd){
	if( m_hWnd )
        [m_hWnd setNeedsDisplayInRect:CGRectMake(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top)];
	}

bool
ESFrameBase::AttachWND(HWND hWnd, ESFrameBase* pFrame){
	AutoSortedArray* pArrWnds = GetArrayWNDAttachments();
	ASSERT(pArrWnds);
	bool bRet = false;
	if( pArrWnds->IndexOf((__bridge void*)hWnd) == -1 ){
		pArrWnds->Add((__bridge void*)hWnd, (void*)pFrame);
		bRet = true;
		}
	else{
		// Couldn't attach window hande that is already attached !!!
		ASSERT(FALSE);
		}
	return bRet;
	}

ESFrameBase*
ESFrameBase::DetachWND(HWND hWnd){
	AutoSortedArray* pArrWnds = GetArrayWNDAttachments();
	ASSERT(pArrWnds);
	ESFrameBase* pResult = NULL;
	int nIndex = -1;
	pArrWnds->GetData((__bridge void*)hWnd, (void*&)pResult, nIndex);
	if( nIndex > -1 )
		pArrWnds->Delete(nIndex);
	return pResult;
	}
/*
LRESULT CALLBACK
ESFrameBase::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	int				wmId, wmEvent;
	PAINTSTRUCT		ps;
	HDC				hdc;
	ESFrameBase*	pFrame = ESFrameBase::FromHandle(hWnd);

	switch (message){
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
	
			break;
		case WM_SYSCOMMAND:
			if( pFrame ){
				pFrame->OnSysCommand((UINT)wParam, lParam);
				return DefWindowProc(hWnd, message, wParam, lParam);
				return 0;//An application should return zero if it processes this message.
				}
			
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			if( pFrame ){
				_DC dc;
				dc.Attach(hdc);
				pFrame->OnPaint(&dc);
				dc.Detach();
				}
			EndPaint(hWnd, &ps);
			break;
		case WM_ERASEBKGND:
			if( pFrame ){
				_DC dc;
				dc.Attach((DCDef)wParam);
				LRESULT ret = (LRESULT)pFrame->OnEraseBkgnd(&dc);
				dc.Detach();
				return ret;
				//An application should return nonzero if it erases the background; otherwise, it should return zero.
				}
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_SIZE:
			if( pFrame ){
				pFrame->OnSize((UINT)wParam, (int)(lParam&0xFFFF), (int)((lParam >> 16)&0xFFFF));
				return DefWindowProc(hWnd, message, wParam, lParam);
				//return 0;//If an application processes this message, it should return zero
				}
			break;
		case WM_WINDOWPOSCHANGING:
			if( pFrame ){
				pFrame->OnWindowPosChanging((WINDOWPOS*)lParam);
				return DefWindowProc(hWnd, message, wParam, lParam);
				return 0;//If an application processes this message, it should return zero
				}
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_GETMINMAXINFO:
			if( pFrame ){
				pFrame->OnGetMinMaxInfo((MINMAXINFO*)lParam);
				return 0;//If an application processes this message, it should return zero
				}
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_SETCURSOR:
			if( pFrame ){
				//If an application processes this message, it should return TRUE to halt further processing or FALSE to continue.
				if( pFrame->OnSetCursor((HWND)wParam, (UINT)(lParam&0xFFFF), (UINT)((lParam>>16)&0xFFFF)) ){
					return TRUE;
					}
				}
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;

		case WM_SETFOCUS:
			if( pFrame ){
				pFrame->OnSetFocus((HWND)wParam);
				return 0;//An application should return zero if it processes this message.
				}
			break;
		case WM_KILLFOCUS:
			if( pFrame ){
				pFrame->OnKillFocus((HWND)wParam);
				return 0;//An application should return zero if it processes this message.
				}
			break;
		case WM_MOUSEMOVE:
			if( pFrame ){
				pFrame->OnMouseMove((UINT)wParam ,_Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_MOUSEWHEEL:
			if( pFrame ){
				pFrame->OnMouseWheel((UINT)(wParam&0xFFFF), (short)((wParam>>16)&0xFFFF), _Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//An application should return zero if it processes this message.
				}
			break;
		case WM_LBUTTONDOWN:
			if( pFrame ){
				pFrame->OnLButtonDown((UINT)wParam ,_Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_LBUTTONDBLCLK:
			if( pFrame ){
				pFrame->OnLButtonDblClk((UINT)wParam ,_Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_LBUTTONUP:
			if( pFrame ){
				pFrame->OnLButtonUp((UINT)wParam ,_Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_RBUTTONUP:
			if( pFrame ){
				pFrame->OnRButtonUp((UINT)wParam ,_Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_KEYDOWN:
			if( pFrame ){
				pFrame->OnKeyDown(wParam, (UINT)(lParam&0xFFFF), (UINT)((lParam>>16)&0xFFFF));
				return 0;//An application should return zero if it processes this message.
				}
			break;
		case WM_CHAR:
			if( pFrame ){
				pFrame->OnChar(wParam, (UINT)(lParam&0xFFFF), (UINT)((lParam>>16)&0xFFFF));
				return 0;//An application should return zero if it processes this message.
				}
			break;
		case WM_NCHITTEST:
			if( pFrame ){
				return pFrame->OnNcHitTest(_Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				}
			break;
			
		case WM_CLOSE:
			if( pFrame ){
				pFrame->OnClose();
				return DefWindowProc(hWnd, message, wParam, lParam);
				//return 0;//If an application processes this message, it should return zero
				}
		case WM_DESTROY:
			if( pFrame )
				pFrame->OnDestroy();
			PostQuitMessage(0);
			break;
		case WM_TIMER:
			if( pFrame ){
				if( lParam ){
					// Call timer procedure.
					//VOID  CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
					}
				else{
					pFrame->OnTimer((UINT_PTR)wParam);
					return 0;//An application should return zero if it processes this message.
					}
				}
			break;
		case WM_REDRAW_RECT:
			if( pFrame ){
				pFrame->OnRedrawRect((CPtrArray*)lParam, (BOOL)wParam);
				return 0;//An application should return zero if it processes this message.
				}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	return 0;
	}
*/
void
ESFrameBase::OnPaint(_DC *pDC){
	}

BOOL
ESFrameBase::OnEraseBkgnd(_DC *pDC){
	return 1;
	}

void
ESFrameBase::OnSysCommand(UINT nID, LPARAM lParam){
    /*
	if( (nID&0xFFF0) == SC_MAXIMIZE ){
		// Restore window to its original size.
		if( !m_rcRestoredRect.IsRectEmpty() ){
			// Remove miximized style.
			//DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
			//dwStyle &= ~WS_MAXIMIZE;
			ModifyStyle(WS_MAXIMIZE, 0L);
		
			_Rect rcRestoredRect = m_rcRestoredRect;
			m_rcRestoredRect.SetRect(0, 0, 0, 0);
			SetWindowPos(NULL, rcRestoredRect.left, rcRestoredRect.top, rcRestoredRect.Width(), rcRestoredRect.Height(), 0L);
			return;
			}
		else{
			WINDOWPLACEMENT wplc;
			wplc.length = sizeof(WINDOWPLACEMENT);
			if( ::GetWindowPlacement(m_hWnd, &wplc) ){
				// Save restored window size.
				m_rcRestoredRect = wplc.rcNormalPosition;

				// Remove miximized style.
				//DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
				//dwStyle &= ~WS_MAXIMIZE;
				//SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
				ModifyStyle(WS_MAXIMIZE, 0L);
				return;
				}
			}
		}*/
	}

void
ESFrameBase::OnDestroy(){
	//DetachWND(m_hWnd);
	m_hWnd = NULL;
	}

void
ESFrameBase::OnSize(UINT nType, int cx, int cy){
    /*
	if( nType != SIZE_MINIMIZED ){
		}
	else
		m_nHitTestCode = 0;

	// Set focus to the next window.
	if( nType == SIZE_MINIMIZED ){
		HWND hWndNext = ::GetWindow(m_hWnd, GW_HWNDNEXT);
		::SetFocus(hWndNext);
		}*/
	}

void
ESFrameBase::OnClose(){
	}
/*
void
ESFrameBase::OnGetMinMaxInfo(MINMAXINFO* lpMMI){
	lpMMI->ptMaxPosition.x	= (lpMMI->ptMaxSize.x -  m_szMax.cx)/2;
	lpMMI->ptMaxPosition.y	= (lpMMI->ptMaxSize.y -  m_szMax.cy)/2;

	lpMMI->ptMinTrackSize.x = m_szMin.cx + m_nBorderWidth*2;
	lpMMI->ptMinTrackSize.y = m_szMin.cy + m_nBorderWidth + m_nCaptionHeight;
	lpMMI->ptMaxTrackSize.x = m_szMax.cx + m_nBorderWidth*2;
	lpMMI->ptMaxTrackSize.y = m_szMax.cy + m_nBorderWidth + m_nCaptionHeight;

	lpMMI->ptMaxSize.x		= lpMMI->ptMaxTrackSize.x;
	lpMMI->ptMaxSize.y		= lpMMI->ptMaxTrackSize.y;
	}*/

LRESULT	
ESFrameBase::OnNcHitTest(_Point pt){
    LRESULT lRet	= 0;//DefWindowProc(m_hWnd, WM_NCHITTEST, 0L,  ((pt.x&0xFFFF) | (pt.y << 16)));
	m_nHitTestCode	= lRet;
	return lRet;
	}

/*
void
ESFrameBase::OnWindowPosChanging(LPWINDOWPOS lpWndPos){
	if( lpWndPos->cx > 0 && lpWndPos->cy > 0 && lpWndPos->hwnd == m_hWnd && m_fHeightAndWidthRatio > 0.0f ){
		_Rect rcClient;
		::GetClientRect(m_hWnd, rcClient);

		_Rect rcWindow;
		::GetWindowRect(m_hWnd, rcWindow);

		if( lpWndPos->x < 0 || lpWndPos->y < 0 )
			m_nHitTestCode = 0;

		_Size szClient(rcClient.Width(), rcClient.Height());
		_Size szWindow(rcWindow.Width(), rcWindow.Height());

		switch( m_nHitTestCode ){
//			case HTSIZE:
			case HTTOPLEFT:
			case HTTOPRIGHT:
			case HTBOTTOMLEFT:
			case HTBOTTOMRIGHT:
				{
				lpWndPos->cy = ((lpWndPos->cx - (szWindow.cx - szClient.cx))*m_fHeightAndWidthRatio + (szWindow.cy - szClient.cy));
				break;
				}
			case HTLEFT:
			case HTRIGHT:
				{
				lpWndPos->cy = ((lpWndPos->cx - (szWindow.cx - szClient.cx))*m_fHeightAndWidthRatio + (szWindow.cy - szClient.cy));
				break;
				}
			case HTTOP:
			case HTBOTTOM:
				{
				lpWndPos->cx = ((lpWndPos->cy - (szWindow.cy - szClient.cy))/m_fHeightAndWidthRatio + (szWindow.cx - szClient.cx));
				break;
				}
			};
		}
	}*/

BOOL
ESFrameBase::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	return FALSE;
	}
