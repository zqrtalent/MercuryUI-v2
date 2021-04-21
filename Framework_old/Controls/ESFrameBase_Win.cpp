// ESFrameBase.cpp : implementation file
//

#include "ESFrameBase.h"

// ESFrameBase dialog
ESFrameBase::ESFrameBase() {
	m_hWnd					= NULL;
	//m_hIcon					= NULL;
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
	}

ESFrameBase::~ESFrameBase(){
	}

int
ESFrameBase::Create(){
    /*
	ASSERT(m_hWnd == NULL);
	if( m_hWnd ){
		// Window is already created!!!!
		return 0;
		}

	std::string wndClassName;
	if( !OnRegisterWindowClass(NULL, wndClassName) ){
		// Window class registration failed !!!
		return 0;
		}

	if( !OnInitDialog() ){
		// Initialization of window is failed !!!
		return 0;
		}

	MSG msg;
	// Main message loop:
	while ( GetMessage(&msg, NULL, 0, 0) ){
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		}
     return msg.wParam;*/return 0;
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
ESFrameBase::CloseWindow(){/*
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
ESFrameBase::GetClientRect(Rect& rcClient){
    /*if( !IsWindow() )
		return FALSE;
	return ::GetClientRect(m_hWnd, rcClient);*/
    rcClient.right = 909;
    return FALSE;
	}

BOOL
ESFrameBase::GetWindowRect(Rect& rcWindow){/*
	if( !IsWindow() )
		return FALSE;
	return ::GetWindowRect(m_hWnd, rcWindow);*/return FALSE;
	}

BOOL
ESFrameBase::ScreenToClient(Point& pt){/*
	if( !IsWindow() )
		return FALSE;
	return ::ScreenToClient(m_hWnd, pt);*/return FALSE;
	}

BOOL
ESFrameBase::ScreenToClient(Rect& rect){
    /*
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
ESFrameBase::ClientToScreen(Point& pt){/*
	if( !IsWindow() )
		return FALSE;
	return ::ClientToScreen(m_hWnd, pt);*/return FALSE;
	}

BOOL
ESFrameBase::ClientToScreen(Rect& rect){
    /*
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
ESFrameBase::ModifyStyle(DWORD dwStyleRemove, DWORD dwStyleAdd){
    /*
	if( !IsWindow() )
		return FALSE;
	DWORD dwStyle = ::GetWindowLongPtr(m_hWnd, GWL_STYLE);
	dwStyle &= ~dwStyleRemove;
	dwStyle |= dwStyleAdd;
	::SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);*/
	return TRUE;
	}

DWORD
ESFrameBase::GetStyle(){
    /*
	if( !IsWindow() )
		return 0L;
	return (DWORD)::GetWindowLongPtr(m_hWnd, GWL_STYLE);*/
    return 0L;
	}

DWORD
ESFrameBase::GetStyleEx(){
    /*
	if( !IsWindow() )
		return 0L;
	return (DWORD)::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);*/
    return 0L;
	}

UINT
ESFrameBase::SendMessage(UINT nMessage, WPARAM wParam, LPARAM lParam){
    /*
	if( !m_hWnd )
		return 0;
	return ::SendMessage(m_hWnd, nMessage, wParam, lParam);
    */
    return 0;
	}

UINT
ESFrameBase::PostMessage(UINT nMessage, WPARAM wParam, LPARAM lParam){
    /*
	if( !m_hWnd )
		return 0;
	return ::PostMessage(m_hWnd, nMessage, wParam, lParam);
	*/
    }

BOOL
ESFrameBase::SetTimer(UINT_PTR nIDEvent, UINT nElapse){
    /*
	if( !m_hWnd )
		return FALSE;
	return ::SetTimer(m_hWnd, nIDEvent, nElapse, NULL);
     */
    return FALSE;
	}

BOOL
ESFrameBase::KillTimer(UINT_PTR nIDEvent){
    /*
	if( !m_hWnd )
		return FALSE;
	return ::KillTimer(m_hWnd, nIDEvent);*/
    return FALSE;
	}

void
ESFrameBase::CloseDialog(int nRetCode){
	//if( ::IsWindow(m_hWnd) )
	//	PostMessage(WM_CLOSE, nRetCode, 0);
	}

bool
ESFrameBase::LoadWindowPlacement(_Point ptPos, _Size szWindow){
    /*
	if( ptPos.x <= 0 || ptPos.y <= 0 )
		return false;

	_Rect rcWindow	(0, 0, 0, 0);
	_Rect rcDesktop	(0, 0, 0, 0);
	::GetWindowRect(::GetDesktopWindow(), rcDesktop);

	Size	szDefault	= m_szDefault;
	Point	ptDefault	= m_ptSetupPos;

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

Size
ESFrameBase::GetSizeMin(){
	return Size(m_szMin.cx + 2*m_nBorderWidth, m_szMin.cy + m_nCaptionHeight + m_nBorderWidth);
	}

Size
ESFrameBase::GetSizeMax(){
	return Size(m_szMax.cx + 2*m_nBorderWidth, m_szMax.cy + m_nCaptionHeight + m_nBorderWidth);
	}

Size
ESFrameBase::GetSizeDefault(){
	return Size(m_szDefault.cx + 2*m_nBorderWidth, m_szDefault.cy + m_nCaptionHeight + m_nBorderWidth);
	}

Size
ESFrameBase::GetCurrentSizeOmitBorder(Point* pWindowPos){
	_Size szRet(0, 0);
	_Rect rcWindow(0, 0, 0, 0);
	
	if( pWindowPos )
		pWindowPos->SetPoint(0, 0);
	
	if( !::GetWindowRect(m_hWnd, rcWindow) )
		return szRet;

	if( pWindowPos )
		pWindowPos->SetPoint(rcWindow.left, rcWindow.top);

	szRet.SetSize(rcWindow.Width() - 2*m_nBorderWidth, rcWindow.Height() - m_nCaptionHeight - m_nBorderWidth);
	return szRet;
	}

void
ESFrameBase::CalcCaptionBorderSize(){
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
		m_nCaptionHeight = m_nBorderWidth;
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

	return TRUE;  // return TRUE  unless you set the focus to a control
	}

bool
ESFrameBase::OnRegisterWindowClass(HINSTANCE hInst, std::string& wndClassName){
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

	wndClassName		= m_sWndClass;
	return true;
	}

/*
IDD_POKER_TABLE_DIALOG DIALOGEX 0, 0, 551, 314
STYLE DS_SETFONT | DS_FIXEDSYS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME
EXSTYLE WS_EX_APPWINDOW
CAPTION "Poker Table"
FONT 8, "MS Shell Dlg", 0, 0, 0x1
BEGIN
END
*/
/*
int
ESFrameBase::CreateModal(CWnd* pParent, DWORD dwStyle, DWORD dwStyleEx, UINT nMenuId){
	if( !dwStyle )
		dwStyle = (DS_SETFONT|DS_FIXEDSYS|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME);
	if( !dwStyleEx )
		dwStyleEx = WS_EX_APPWINDOW;

	ResourceCompiler*	pCompiler		= ResourceCompiler::GetCompiler();
	UINT				nSize			= 0;
	BYTE*				lpDlgTemplate	= new BYTE[1024];

	std::string	sResourceScript	= 
_T("IDD_POKER_TABLE_DIALOG DIALOGEX 0, 0, 800, 314 \
STYLE DS_SETFONT | DS_FIXEDSYS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME \
EXSTYLE WS_EX_APPWINDOW \
CAPTION ~Window 1~ %s\
FONT 8, ~MS Shell Dlg~, 0, 0, 0x1 \
BEGIN \
END");
	sResourceScript.Replace('~', '"');

	std::string sScriptFormated;
	if( nMenuId > 0 ){
		sScriptFormated.Format(sResourceScript, _T("MENU IDR_MENU1 "));
		pCompiler->AddDefine(_T("IDR_MENU1"), nMenuId);
		}
	else
		sScriptFormated.Format(sResourceScript, _T(""));

	pCompiler->AddDefine(_T("IDD_POKER_TABLE_DIALOG"), 1000);
	if( !pCompiler->CompileDialogScript(sScriptFormated, (void*&)lpDlgTemplate, nSize, dwStyle, dwStyleEx) ){
		delete [] lpDlgTemplate;
		return -1;
		}

	m_lpDialogTemplate	= (LPCDLGTEMPLATE)lpDlgTemplate;

	int nRet = 0;
	// If parent window is not modal window.
	if(  TRUE ){
		HWND hWndParent = pParent->GetSafeHwnd();
		if( hWndParent )
			::EnableWindow(hWndParent, 0);

		BOOL bRet = CreateDlgIndirect(m_lpDialogTemplate, pParent, AfxGetInstanceHandle()); 
		if( !bRet ){
			ASSERT(FALSE);
			return 0;
			}

		if( dwStyle&DS_CENTER )
			CenterWindow();

		MSG					msg;
		HWND				hWnd		= m_hWnd;
		CWnd*				pWnd		= NULL;
		ESFrameBase*		pFrameWnd	= NULL;
		MercuryBaseView*	pMainView	= NULL;

		while( ::GetMessageA(&msg, NULL, 0, 0)  ){
			
			//if( msg.hwnd == hWnd ){
			//	if( msg.message == WM_DESTROY || msg.message == WM_CLOSE || msg.message == WM_NULL ){
			//		if( hWndParent ){
			//			::EnableWindow(hWndParent, 1);
			//			if (hWndParent != NULL && ::GetActiveWindow() == hWnd)
			//				::SetActiveWindow(hWndParent);
			//			}
			//		//nRet = (int)msg.wParam;
			//		nRet = m_nModalResult;
			//		DestroyWindow();
			//		break;
			//		}
			//	}

			if( msg.message == WM_DESTROY || msg.message == WM_NULL ){
				if( msg.hwnd == hWnd ){
					if( hWndParent ){
						::EnableWindow(hWndParent, 1);
						if (hWndParent != NULL && ::GetActiveWindow() == hWnd)
							::SetActiveWindow(hWndParent);
						}

					//nRet = (int)msg.wParam;
					nRet = m_nModalResult;
					if( msg.message == WM_CLOSE )
						nRet = msg.wParam;
					DestroyWindow();
					break; // Main loop exit.
					}
				else{
					pWnd = CWnd::FromHandle(msg.hwnd);
					if( pWnd )
						pWnd->DestroyWindow();
					}
				}

			if( msg.message == WM_KEYDOWN ){
				pWnd = CWnd::FromHandle(msg.hwnd);
				if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
					pFrameWnd	= (ESFrameBase*)pWnd;
					pMainView	= pFrameWnd->m_pMainWnd;
					if( pMainView ){
						if( msg.wParam == VK_TAB ){
							pMainView->OnTabControl();
							}
						else
						if( msg.wParam == VK_RETURN ){
							if( !pMainView->OnOK() )
								pFrameWnd->OnOK();
							}
						else
						if( msg.wParam == VK_ESCAPE ){
							if( !pMainView->OnCancel() )
								pFrameWnd->OnCancel();
							}
						}
					}
				}

			
			//
			//	if( PreTranslateMessage(&msg) ){pWnd = CWnd::FromHandle(msg.hwnd);
			//	if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
			//		pFrameWnd	= (ESFrameBase*)pWnd;
			//		pMainView	= pFrameWnd->m_pMainWnd;
			//		if( !ContinueModal() ){
			//			m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);
			//			nRet = m_nModalResult;
			//			break;
			//			}
			//		}

			// Tooltip message events.
			if( msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST ){
				pWnd = CWnd::FromHandle(msg.hwnd);
				if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
					pFrameWnd	= (ESFrameBase*)pWnd;
					pMainView	= pFrameWnd->m_pMainWnd;
					if( pMainView )
						pMainView->PreTranslateMessage(&msg);
					}
				}

			::TranslateMessage	(&msg);
			::DispatchMessage	(&msg);
			}
		}
	else{
		nRet = (int)DoModal();
		}
	
	m_lpDialogTemplate = NULL;
	delete [] lpDlgTemplate;
	return nRet;
	}*/

		/*
BOOL
ESFrameBase::Create(CWnd* pParent, DWORD dwStyle, DWORD dwStyleEx, UINT nMenuId){
	if( !dwStyle )
		dwStyle = (DS_SETFONT|DS_FIXEDSYS|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME);

	if( !dwStyleEx )
		dwStyleEx = WS_EX_APPWINDOW;

	ResourceCompiler*	pCompiler		= ResourceCompiler::GetCompiler();
	UINT				nSize			= 0;
	BYTE*				lpDlgTemplate	= new BYTE[1024];

	std::string	sResourceScript	= 
_T("IDD_POKER_TABLE_DIALOG DIALOGEX 0, 0, 551, 314 \
STYLE DS_SETFONT | DS_FIXEDSYS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME \
EXSTYLE WS_EX_APPWINDOW \
CAPTION ~Poker Table~ %s\
FONT 8, ~MS Shell Dlg~, 0, 0, 0x1 \
BEGIN \
END");
	sResourceScript.Replace('~', '"'); 

	std::string sScriptFormated;
	if( nMenuId > 0 ){
		sScriptFormated.Format(sResourceScript, _T("MENU IDR_MENU1 "));
		pCompiler->AddDefine(_T("IDR_MENU1"), nMenuId);
		}
	else{
		sScriptFormated.Format(sResourceScript, _T(""));
		}

	pCompiler->AddDefine(_T("IDD_POKER_TABLE_DIALOG"), 1000);
	if( !pCompiler->CompileDialogScript(sScriptFormated, (void*&)lpDlgTemplate, nSize, dwStyle, dwStyleEx) ){
		delete [] lpDlgTemplate;
		return -1;
		}

	m_lpDialogTemplate	= (LPCDLGTEMPLATE)lpDlgTemplate;
	BOOL bRet = CreateDlgIndirect(m_lpDialogTemplate, pParent, AfxGetInstanceHandle());
	if( !bRet ){
		m_lpDialogTemplate = NULL;
		delete [] lpDlgTemplate;
		}
	else{
		UpdateWindow();
//		ShowWindow(SW_SHOW);
		}
	return bRet;
	}

bool
ESFrameBase::CallCmdMsgHandler(UINT nID, int nCode){
	DWORD	dwKey	= ((((DWORD)nCode)<<16)|(((DWORD)nID)&0xFFFF));
	int		nIndex	= m_arrCmdMsgHandlerByCode.FindFirst(dwKey);
	bool	bRet	= false;
	while( nIndex != -1 ){
		MsgHandlerInfo* pHandlerInfo = (MsgHandlerInfo*)m_arrCmdMsgHandlerByCode.GetData(nIndex);
		if( pHandlerInfo ){
			//(this->*mmf.pfn_l_p)(point);
			((pHandlerInfo->pReceiver)->*pHandlerInfo->cmdMethod)();
			bRet = true;
			}
		break;
		nIndex = m_arrCmdMsgHandlerByCode.FindNext();
		}
	return bRet;
	}

LRESULT
ESFrameBase::CallMsgHandler	(UINT message, WPARAM wParam, LPARAM lParam){
	int		nIndex	= m_arrMsgHandlerByCode.FindFirst(message);
	LRESULT	lRet	= 0L;

	while( nIndex != -1 ){
		MsgHandlerInfo* pHandlerInfo = (MsgHandlerInfo*)m_arrMsgHandlerByCode.GetData(nIndex);
		if( pHandlerInfo ){
			//(this->*mmf.pfn_l_p)(point);
			lRet = ((pHandlerInfo->pReceiver)->*pHandlerInfo->msgMethod)(wParam, lParam);
			}
		break;
		//nIndex = m_arrMsgHandlerByCode.FindNext();
		}
	return lRet;
	}

bool
ESFrameBase::IsCmdHandler(UINT nID, int nCode){
	DWORD	dwKey	= ((((DWORD)nCode)<<16)|(((DWORD)nID)&0xFFFF));
	int		nIndex	= m_arrCmdMsgHandlerByCode.FindFirst(dwKey);
	return (nIndex != -1);
	}

bool
ESFrameBase::AddCommandMsgListener(CObject* pReceiver, WORD wNotifyCode, WORD wId, PCMDMethod method){
	DWORD dwKey						= ((((DWORD)wNotifyCode)<<16)|(((DWORD)wId)&0xFFFF));
	MsgHandlerInfo* pHandlerInfo	= new MsgHandlerInfo();
	pHandlerInfo->cmdMethod			= method;
	pHandlerInfo->pReceiver			= pReceiver;
	m_arrCmdMsgHandlerByCode.Add(dwKey, pHandlerInfo);
	return true;
	}

bool
ESFrameBase::AddWndMsgListener(CObject* pReceiver, DWORD dwMessage, PMSGMethod method){
	MsgHandlerInfo* pHandlerInfo	= new MsgHandlerInfo();
	pHandlerInfo->msgMethod			= method;
	pHandlerInfo->pReceiver			= pReceiver;
	m_arrMsgHandlerByCode.Add(dwMessage, pHandlerInfo);
	return true;
	}

bool
ESFrameBase::AddEventListenerByName(CObject* pReceiver, TCHAR* pszName, int eventCode, ControlEventListenerTemp method){
	if( !m_pMainWnd )
		return false;
	return m_pMainWnd->AddEventListenerByName(pReceiver, pszName, (ChildControlEventCode)eventCode, (ControlEventListener)method);
	}

bool
ESFrameBase::AddEventListener(CObject* pReceiver, int nControlId, int eventCode, ControlEventListenerTemp method){
	if( !m_pMainWnd )
		return false;
	return m_pMainWnd->AddEventListener(pReceiver, nControlId, (ChildControlEventCode)eventCode, (ControlEventListener)method);
	}

void
ESFrameBase::CloseDialog(int nRetCode){
	if( !::IsWindow(m_hWnd) )
		return;
	EndDialog(nRetCode);
	if( ::IsWindow(m_hWnd) )
		PostMessage(WM_CLOSE, nRetCode, 0);
	}

void
ESFrameBase::OnClose(){
	CDialog::OnClose();

	// End modal loop.
	if( m_nFlags & WF_CONTINUEMODAL )
		EndModalLoop(IDOK);
	else
		EndDialog(IDOK);
	}

void
ESFrameBase::OnDestroy(){
	CDialog::OnDestroy();

	if( m_pMainWnd ){
		delete m_pMainWnd;
		m_pMainWnd = NULL;
		} 

	// Clean up.
	for(int i=0; i<m_arrCmdMsgHandlerByCode.GetCount(); i++){
		MsgHandlerInfo* pInfo = (MsgHandlerInfo*)m_arrCmdMsgHandlerByCode.GetData(i);
		if( pInfo )
			delete pInfo;
		}
	for(int i=0; i<m_arrMsgHandlerByCode.GetCount(); i++){
		MsgHandlerInfo* pInfo = (MsgHandlerInfo*)m_arrMsgHandlerByCode.GetData(i);
		if( pInfo )
			delete pInfo;
		}

	m_arrCmdMsgHandlerByCode.DeleteAll();
	m_arrMsgHandlerByCode	.DeleteAll();
	}

void 
ESFrameBase::OnSysCommand(UINT nID, LPARAM lParam){
	if( (nID&0xFFF0) == SC_MAXIMIZE ){
		// Restore window to its original size.
		if( !m_rcRestoredRect.IsRectEmpty() ){
			// Remove miximized style.
			DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
			dwStyle &= ~WS_MAXIMIZE;
			SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
		
			_Rect rcRestoredRect = m_rcRestoredRect;
			m_rcRestoredRect.SetRectEmpty();
			::SetWindowPos(m_hWnd, NULL, rcRestoredRect.left, rcRestoredRect.top, rcRestoredRect.Width(), rcRestoredRect.Height(), 0L);
			return;
			}
		else{
			WINDOWPLACEMENT wplc;
			wplc.length = sizeof(WINDOWPLACEMENT);
			if( ::GetWindowPlacement(m_hWnd, &wplc) ){
				// Save restored window size.
				m_rcRestoredRect		= wplc.rcNormalPosition;

				CDialog::OnSysCommand(nID, lParam);

				// Remove miximized style.
				DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
				dwStyle &= ~WS_MAXIMIZE;
				SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
				return;
				}
			}
		}

	CDialog::OnSysCommand(nID, lParam);
	}

void
ESFrameBase::OnSize(UINT nType, int cx, int cy){
	CDialog::OnSize(nType, cx, cy);

	
	//if( nType == SIZE_MAXIMIZED ){
	//	// Restore window to its original size.
	//	if( !m_rcRestoredRect.IsRectEmpty() ){
	//		// Remove miximized style.
	//		DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
	//		dwStyle &= ~WS_MAXIMIZE;
	//		SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
	//	
	//		_Rect rcRestoredRect = m_rcRestoredRect;
	//		m_rcRestoredRect.SetRectEmpty();
	//		::SetWindowPos(m_hWnd, NULL, rcRestoredRect.left, rcRestoredRect.top, rcRestoredRect.Width(), rcRestoredRect.Height(), 0L);
	//		return;
	//		}
	//	else{
	//		WINDOWPLACEMENT wplc;
	//		wplc.length = sizeof(WINDOWPLACEMENT);
	//		if( ::GetWindowPlacement(m_hWnd, &wplc) ){
	//			// Save restored window size.
	//			m_rcRestoredRect		= wplc.rcNormalPosition;
	//			// Remove miximized style.
	//			DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
	//			dwStyle &= ~WS_MAXIMIZE;
	//			SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
	//			}
	//		}
	//	}

	if( nType != SIZE_MINIMIZED && m_pMainWnd ){
		}
	else
		m_nHitTestCode = 0;

	// Set focus to the next window.
	if( nType == SIZE_MINIMIZED ){
		HWND hWndNext = ::GetWindow(m_hWnd, GW_HWNDNEXT);
		::SetFocus(hWndNext);
		}
	}

void
ESFrameBase::OnGetMinMaxInfo(MINMAXINFO* lpMMI){
	CDialog::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMaxPosition.x	= (lpMMI->ptMaxSize.x -  m_szMax.cx)/2;
	lpMMI->ptMaxPosition.y	= (lpMMI->ptMaxSize.y -  m_szMax.cy)/2;

	lpMMI->ptMinTrackSize.x = m_szMin.cx + m_nBorderWidth*2;
	lpMMI->ptMinTrackSize.y = m_szMin.cy + m_nBorderWidth + m_nCaptionHeight;
	lpMMI->ptMaxTrackSize.x = m_szMax.cx + m_nBorderWidth*2;
	lpMMI->ptMaxTrackSize.y = m_szMax.cy + m_nBorderWidth + m_nCaptionHeight;

	lpMMI->ptMaxSize.x		= lpMMI->ptMaxTrackSize.x;
	lpMMI->ptMaxSize.y		= lpMMI->ptMaxTrackSize.y;
	}

void
ESFrameBase::OnNcCalSize(BOOL bCalcValidRects, NCCALSize_PARAMS* lpncsp){
	CDialog::OnNcCalSize(bCalcValidRects, lpncsp);
	}

LRESULT
ESFrameBase::OnNcHitTest(_Point pt){
	LRESULT lRet	= CDialog::OnNcHitTest(pt);
	m_nHitTestCode	= lRet;
	return lRet;
	}

void
ESFrameBase::OnWindowPosChanging(LPWINDOWPOS lpWndPos){
	if( lpWndPos->cx > 0 && lpWndPos->cy > 0 && lpWndPos->hwnd == m_hWnd && m_fHeightAndWidthRatio > 0.0f ){
		_Rect rcClient;
		GetClientRect(rcClient);

		_Rect rcWindow;
		GetWindowRect(rcWindow);

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
	CDialog::OnWindowPosChanging(lpWndPos);
	}

BOOL
ESFrameBase::OnEraseBkgnd(CDC* pDC){
	return 1;
	}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void 
ESFrameBase::OnPaint(){
	if (IsIconic() && m_hIcon){
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		_Rect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		}
	else{
		CDialog::OnPaint();
		}
	}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR 
ESFrameBase::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
	}

BOOL
ESFrameBase::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	BOOL bRet = CDialog::OnSetCursor(pWnd, nHitTest, message);
	
	//// Call handler.
	//_AFX_THREAD_STATE* pState = AfxGetThreadState();
	//UINT	msg		= pState->m_lastSentMsg.message;
	//WPARAM	wParam	= pState->m_lastSentMsg.wParam;
	//LPARAM	lParam	= pState->m_lastSentMsg.lParam;
	//bRet			= (BOOL)CallMsgHandler(msg, wParam, lParam);
	
	return bRet;
	}

UINT
ESFrameBase::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

void
ESFrameBase::SetTitle(){
	// Update title.
	if( IsWindow(m_hWnd) && m_pMainWnd )
		SetWindowText(m_pMainWnd->GetViewInfo()->m_sTitle);
	}

void
ESFrameBase::UseForgroundColor(BOOL bUse){
	if( !m_pMainWnd )
		return;
	m_pMainWnd->UseForegroundColor	(bUse);
	m_pMainWnd->Invalidate			();
	}

Size
ESFrameBase::GetSizeMin(){
	return Size(m_szMin.cx + 2*m_nBorderWidth, m_szMin.cy + m_nCaptionHeight + m_nBorderWidth);
	}

Size
ESFrameBase::GetSizeMax(){
	return Size(m_szMax.cx + 2*m_nBorderWidth, m_szMax.cy + m_nCaptionHeight + m_nBorderWidth);
	}

Size
ESFrameBase::GetSizeDefault(){
	return Size(m_szDefault.cx + 2*m_nBorderWidth, m_szDefault.cy + m_nCaptionHeight + m_nBorderWidth);
	}

Size
ESFrameBase::GetCurrentSizeOmitBorder(Point* pWindowPos){
	_Size szRet(0, 0);
	_Rect rcWindow(0, 0, 0, 0);
	
	if( pWindowPos )
		pWindowPos->SetPoint(0, 0);
	
	if( !::GetWindowRect(GetSafeHwnd(), rcWindow) )
		return szRet;

	if( pWindowPos )
		pWindowPos->SetPoint(rcWindow.left, rcWindow.top);

	szRet.SetSize(rcWindow.Width() - 2*m_nBorderWidth, rcWindow.Height() - m_nCaptionHeight - m_nBorderWidth);
	return szRet;
	}

void
ESFrameBase::Setup(){
	if( m_pMainWnd ){
		m_pMainWnd->AttachToFrame(this);
		
		//_Rect rcClient;
		//GetClientRect(rcClient);
		//BOOL bRet = ((CWnd*)m_pMainWnd)->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, Rect(0, 0, rcClient.Width(), rcClient.Height()), this, _MercuryViewId, NULL);
		//if( bRet )
		//	m_pMainWnd->SetOwner(this);
		//	
		}
	}

void
ESFrameBase::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu){
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	if( bSysMenu ) return;
	UpdatePopupMenuItems(pPopupMenu);
	}

void
ESFrameBase::UpdatePopupMenuItems(CMenu* pPopupMenu){
	if( !pPopupMenu ) return;
	int nCt = pPopupMenu->GetMenuItemCount();

	CCmdUI state;
    state.m_pMenu	= pPopupMenu;

	MENUITEMINFO miinfo;
	miinfo.cbSize	= sizeof(MENUITEMINFO);
	miinfo.fMask	= MIIM_STATE|MIIM_ID|MIIM_SUBMENU;

	for(int i=0; i<nCt; i++){
		if( pPopupMenu->GetMenuItemInfoA(i, &miinfo, TRUE) ){
			if( miinfo.hSubMenu ){
				CMenu* pSubMenu = CMenu::FromHandle(miinfo.hSubMenu);
				UpdatePopupMenuItems(pSubMenu);
				}
			else{
				if( miinfo.wID != 0 ){
					state.m_nID			= miinfo.wID;
					state.m_nIndex		= i;
					state.m_nIndexMax	= nCt;
					state.DoUpdate(this, TRUE);
					}
				}
			}
		}
	}

void
ESFrameBase::CalcCaptionBorderSize(){
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
		m_nCaptionHeight = m_nBorderWidth;
	}

void
ESFrameBase::OnOK(){
	PostMessage(WM_CLOSE, IDOK, 0);
	
	//CWnd* pWnd = GetFocus();
	//if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CEdit)) ){
	//	CEdit* pWndEdit = (CEdit*)pWnd;
	//	int nId = (int)GetWindowLong(*pWnd, GWL_ID);
	//	switch( nId ){
	//		case CHILD_SliderMoneyEdit:{
	//			if( GetKeyState(VK_CONTROL)&0x8000 ){
	//				// Call or check
	//				ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(1);
	//				if( pInfo )
	//					OnActionButtonClicked(pInfo);
	//				}
	//			else{
	//				// Bet or rise
	//				ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(2);
	//				if( pInfo )
	//					OnActionButtonClicked(pInfo);
	//				}
	//			break;
	//			}
	//	   // Add chips edit.
	//		case CHILD_AddChipsMoneyEdit:{
	//			int nAmount = m_pTableWnd->GetAddChipsAmount();
	//			OnAddChipsButtonClicked(nAmount);
	//			break;
	//			}

	//		// Send chat message.
	//		case CHILD_Table_Chat_Edit:{
	//			std::string sChatMessage;
	//			pWndEdit->GetWindowText(sChatMessage);
	//			pWndEdit->SetWindowText(_T(""));
	//			m_client->SendChatMessage(m_nTableId, m_nTournamentId, sChatMessage);
	//			break;
	//			}
	//		};
	//	}
	//else{
	//	if( GetKeyState(VK_CONTROL)&0x8000 ){
	//		// Call or check
	//		ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(1);
	//		if( pInfo && (pInfo->type == ButtonCall || pInfo->type == ButtonCheck) )
	//			OnActionButtonClicked(pInfo);
	//		}
	//	else{
	//		// Bet or rise
	//		ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(2);
	//		if( pInfo && (pInfo->type == ButtonRaise || pInfo->type == ButtonBet) )
	//			OnActionButtonClicked(pInfo);
	//		}
	//	}
	}

void
ESFrameBase::OnCancel(){
	if( m_pMainWnd && m_pMainWnd->IsDesignerMode() )
		ESFrameBase::CloseDialog(IDCANCEL);
	
	//if( m_pTableWnd ){
	//	if( m_pTableWnd->IsAddChipsControlVisible() )
	//		OnAddChipsControlClose();
	//	}
	}
*/

DCDef
ESFrameBase::GetDC(){
	if( !m_hWnd )
		return NULL;
	return ::GetDC(m_hWnd);
	}

void
ESFrameBase::ReleaseDC(DCDef dc){
	if( m_hWnd )
		::ReleaseDC(m_hWnd, dc);
	}

ESFrameBase*
ESFrameBase::FromHandle(HWND hWnd){
	AutoSortedArray* pArrWnds = GetArrayWNDAttachments();
	ASSERT(pArrWnds);
	ESFrameBase* pResult = NULL;
	int nIndex = -1;
	pArrWnds->GetData((void*)hWnd, (void*&)pResult, nIndex);
	return pResult;
	}

void
ESFrameBase::Invalidate(BOOL eraseBkgnd){
	if( m_hWnd )
		::InvalidateRect(m_hWnd, NULL, eraseBkgnd);
	}

void
ESFrameBase::InvalidateRect(LPCRECTDef lpRect, BOOL eraseBkgnd){
	if( m_hWnd )
		::InvalidateRect(m_hWnd, lpRect, eraseBkgnd);
	}

bool
ESFrameBase::AttachWND(HWND hWnd, ESFrameBase* pFrame){
	AutoSortedArray* pArrWnds = GetArrayWNDAttachments();
	ASSERT(pArrWnds);
	bool bRet = false;
	if( pArrWnds->IndexOf((void*)hWnd) == -1 ){
		pArrWnds->Add((void*)hWnd, (void*)pFrame);
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
	pArrWnds->GetData((void*)hWnd, (void*&)pResult, nIndex);
	if( nIndex > -1 )
		pArrWnds->Delete(nIndex);
	return pResult;
	}

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
			/*
			// Parse the menu selections:
			switch (wmId){
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}*/
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
				pFrame->OnMouseMove((UINT)wParam ,Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_MOUSEWHEEL:
			if( pFrame ){
				pFrame->OnMouseWheel((UINT)(wParam&0xFFFF), (short)((wParam>>16)&0xFFFF), Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//An application should return zero if it processes this message.
				}
			break;
		case WM_LBUTTONDOWN:
			if( pFrame ){
				pFrame->OnLButtonDown((UINT)wParam ,Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_LBUTTONDBLCLK:
			if( pFrame ){
				pFrame->OnLButtonDblClk((UINT)wParam ,Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_LBUTTONUP:
			if( pFrame ){
				pFrame->OnLButtonUp((UINT)wParam ,Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
				return 0;//If an application processes this message, it should return zero.
				}
			break;
		case WM_RBUTTONUP:
			if( pFrame ){
				pFrame->OnRButtonUp((UINT)wParam ,Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
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
				return pFrame->OnNcHitTest(Point((int)(lParam&0xFFFF), (int)((lParam>>16)&0xFFFF)));
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

void
ESFrameBase::OnPaint(_DC *pDC){
	}

BOOL
ESFrameBase::OnEraseBkgnd(_DC *pDC){
	return 1;
	}

void
ESFrameBase::OnSysCommand(UINT nID, LPARAM lParam){
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
		}
	}

void
ESFrameBase::OnDestroy(){
	DetachWND(m_hWnd);
	m_hWnd = NULL;
	}

void
ESFrameBase::OnSize(UINT nType, int cx, int cy){
	if( nType != SIZE_MINIMIZED ){
		}
	else
		m_nHitTestCode = 0;

	// Set focus to the next window.
	if( nType == SIZE_MINIMIZED ){
		HWND hWndNext = ::GetWindow(m_hWnd, GW_HWNDNEXT);
		::SetFocus(hWndNext);
		}
	}

void
ESFrameBase::OnClose(){
	}

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
	}

LRESULT	
ESFrameBase::OnNcHitTest(_Point pt){
	LRESULT lRet	= DefWindowProc(m_hWnd, WM_NCHITTEST, 0L,  ((pt.x&0xFFFF) | (pt.y << 16)));
	m_nHitTestCode	= lRet;
	return lRet;
	}

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
	}

BOOL
ESFrameBase::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	return FALSE;
	}