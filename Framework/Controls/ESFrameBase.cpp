// ESFrameBase.cpp : implementation file
//

//#include "stdafx.h"
#include "ESFrameBase.h"
#include "ESChildControl.h"
#include "MercuryBaseWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ESFrameBase dialog
ESFrameBase::ESFrameBase()
	m_hIcon					= NULL;
	m_pMainWnd				= NULL;
	m_fHeightAndWidthRatio	= 0.0f;
	m_bResizingFrame		= false;
	m_szMin			.SetSize(0, 0);
	m_szMax			.SetSize(0, 0);
	m_szDefault		.SetSize(0, 0);
	m_ptSetupPos	.SetPoint(0xFFFFFF, 0xFFFFFF); // Centered by default.
	m_nHitTestCode			= 0;
	m_nBorderWidth			= 0;
	m_nCaptionHeight		= 0;
	m_bMouseUnderClientArea	= false;
	m_bRunloopLessDialog = false;
	}

void 
ESFrameBase::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	}

IMPLEMENT_DYNCREATE(ESFrameBase, CDialog)
BEGIN_MESSAGE_MAP(ESFrameBase, CDialog)
	ON_WM_SYSCOMMAND		()
	ON_WM_INITMENUPOPUP		()
	ON_WM_PAINT				()
	ON_WM_QUERYDRAGICON		()
	ON_WM_DESTROY			()
	ON_WM_SIZE				()
	ON_WM_GETMINMAXINFO		()
	ON_WM_NCCALCSIZE		()
	ON_WM_NCHITTEST			()
	ON_WM_LBUTTONDOWN		()
	ON_WM_WINDOWPOSCHANGING	()
	ON_WM_CLOSE				()
	ON_WM_ERASEBKGND		()
	ON_WM_SETCURSOR			()
	ON_WM_GETDLGCODE		()
	ON_WM_MOUSELEAVE		()
	ON_WM_NCMOUSELEAVE		()
	ON_WM_NCMOUSEHOVER		()
	ON_WM_MOUSEMOVE			()
	ON_WM_LBUTTONDOWN		()
END_MESSAGE_MAP()


// ESFrameBase message handlers
BOOL 
ESFrameBase::OnInitDialog(){
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	if( m_hIcon ){
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
		}

	CalcCaptionBorderSize();

	// Set default window size.
	if( m_szDefault.cx >0 && m_szDefault.cy > 0 )
		::SetWindowPos(m_hWnd, 0L, 0, 0, m_szDefault.cx, m_szDefault.cy, SWP_NOMOVE);

	// Setup.
	Setup();
	// Set title.
	SetTitle();

	return TRUE;  // return TRUE  unless you set the focus to a control
	}

BOOL
ESFrameBase::PreCreateWindow(CREATESTRUCT& cs){
	return 1;
	}

BOOL
ESFrameBase::PreTranslateMessage(MSG* pMsg){
	if( m_pMainWnd ){
		m_pMainWnd->PreTranslateMessage(pMsg);

		if( pMsg->message == WM_KEYDOWN ){
			if( pMsg->wParam == VK_TAB ){
				if( !m_pMainWnd->OnTabControl() )
					return TRUE;
				}
			else
			if( pMsg->wParam == VK_RETURN ){
				if( !m_pMainWnd->OnOK() ){
					OnOK();
					return 1;
					}
			//	if( !m_pMainWnd->OnOK() )
			//		return TRUE;
				}
			else
			if( pMsg->wParam == VK_ESCAPE ){
				OnCancel();
				return 1;
			//	if( !m_pMainWnd->OnCancel() )
			//		return TRUE;
				}
			}
		}

	BOOL bRet = CDialog::PreTranslateMessage(pMsg);
	return bRet;
	}

BOOL
ESFrameBase::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){
	BOOL bRet = FALSE;
	if( message == WM_PACKET_RECEIVED )
		return CDialog::OnWndMsg(message, wParam, lParam, pResult);

	if( message == WM_CLOSE ){
		CallMsgHandler(message, wParam, lParam);
		bRet = CDialog::OnWndMsg(message, wParam, lParam, pResult);
		}
	else{
		HWND hWnd = m_hWnd;
		bRet = CDialog::OnWndMsg(message, wParam, lParam, pResult);
		if( ::IsWindow(hWnd) )
			CallMsgHandler(message, wParam, lParam);
		}

	if( m_hWnd == NULL )
		bRet = 1; // Do not call DefWindfowProc
	return bRet;
	}

BOOL
ESFrameBase::OnCommand(WPARAM wParam, LPARAM lParam){
	UINT	nID			= LOWORD(wParam);
	HWND	hWndCtrl	= (HWND)lParam;
	int		nCode		= HIWORD(wParam);
	HWND	hWnd		= m_hWnd;
	BOOL	bRet		= CDialog::OnCommand(wParam, lParam);

	if( nCode == CN_COMMAND ){
		// Call command message handler.
		if( ::IsWindow(hWnd) )
			CallCmdMsgHandler(nID, nCode);
		return bRet;
		}
	return bRet;
	}

BOOL
ESFrameBase::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo){
	BOOL bRet = CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	if( nCode == CN_COMMAND && !bRet ){
		bRet = IsCmdHandler(nID, nCode) ? TRUE : FALSE;
		}
	return bRet;
	}

bool
ESFrameBase::LoadWindowPlacement(_Point ptPos, _Size szWindow){
//	if( ptPos.x <= 0 || ptPos.y <= 0 )
//		return false;

	_Size	szDefault	= m_szDefault;
	_Point	ptDefault	= m_ptSetupPos;

	if( szWindow.cx > 0 && szWindow.cy > 0 ){
		m_szDefault.cx = szWindow.cx;
		m_szDefault.cy = szWindow.cy;
		}

	_Rect	rcWindow(ptPos.x, ptPos.y, ptPos.x + m_szDefault.cx, ptPos.y + m_szDefault.cy), rcWorkArea;

	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	HMONITOR hMon = MonitorFromRect(rcWindow, MONITOR_DEFAULTTONEAREST);
	if( hMon ){
		GetMonitorInfo(hMon, &mi);
		rcWorkArea = mi.rcWork;
		}
	else
		::GetWindowRect(::GetDesktopWindow(), rcWorkArea);

	if( rcWorkArea.Width() >= rcWindow.Width() && rcWorkArea.Height() >= rcWindow.Height() ){
		if( (rcWindow&rcWorkArea) != rcWindow ){
			if( rcWindow.left < rcWorkArea.left ){
				rcWindow.left	= rcWorkArea.left;
				rcWindow.right	= rcWindow.left + m_szDefault.cx;
				}

			if( rcWindow.top < rcWorkArea.top ){
				rcWindow.top	= rcWorkArea.top;
				rcWindow.bottom	= rcWindow.top + m_szDefault.cy;
				}

			if( rcWindow.right > rcWorkArea.right ){
				rcWindow.right	= rcWorkArea.right;
				rcWindow.left	= rcWindow.right - m_szDefault.cx;
				}

			if( rcWindow.bottom > rcWorkArea.bottom ){
				rcWindow.bottom	= rcWorkArea.bottom;
				rcWindow.top	= rcWindow.bottom - m_szDefault.cy;
				}
			}
		m_ptSetupPos.x = rcWindow.left;
		m_ptSetupPos.y = rcWindow.top;
		}
	else{
		m_szDefault		= szDefault;
		m_ptSetupPos	= ptDefault;
		rcWindow.SetRect(m_ptSetupPos.x, m_ptSetupPos.y, m_ptSetupPos.x + m_szDefault.cx, m_ptSetupPos.y + m_szDefault.cy); 
		}

	/* Old stuff
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

int
ESFrameBase::CloseAllModals(){
	if (!IsWindow(m_hWnd))
		return 0;
	CPtrVector arrWnds;
	if( GetProp(m_hWnd, _T("HasModalLoop")) == (HANDLE)0x1 )
		arrWnds.push_back((void*)m_hWnd);

	HWND hWindow = (HWND)::GetProp(m_hWnd, _T("ChildWnd"));
	while (hWindow){
		if (GetProp(hWindow, _T("HasModalLoop")) == (HANDLE)0x1)
			arrWnds.push_back((void*)hWindow);
		hWindow = (HWND)::GetProp(hWindow, _T("ChildWnd"));
		}

	int nRet = 0;
	for (int i = arrWnds.size() - 1; i >= 0; i--){
		hWindow = (HWND)arrWnds.at(i);
		::PostMessage(hWindow, WM_DESTROY, 0L, 0L);
		nRet++;
		}

	return nRet;
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

int
ESFrameBase::CreateModal(CWnd* pParent, DWORD dwStyle, DWORD dwStyleEx, UINT nMenuId){
	if( !dwStyle )
		dwStyle = (DS_SETFONT|DS_FIXEDSYS|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME);
	if( !dwStyleEx )
		dwStyleEx = WS_EX_APPWINDOW;

	ResourceCompiler*	pCompiler		= ResourceCompiler::GetCompiler();
	UINT				nSize			= 0;
	BYTE*				lpDlgTemplate	= new BYTE[1024];

	_String	sResourceScript	= 
_T("IDD_POKER_TABLE_DIALOG DIALOGEX 0, 0, 800, 314 \
STYLE DS_SETFONT | DS_FIXEDSYS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME \
EXSTYLE WS_EX_APPWINDOW \
CAPTION ~Window 1~ %s\
FONT 8, ~MS Shell Dlg~, 0, 0, 0x1 \
BEGIN \
END");
	sResourceScript.Replace('~', '"');

	_String sScriptFormated;
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
	if( /*pParent && !(pParent->m_nFlags&WF_MODALLOOP)*/ TRUE ){
		HWND hWndParent = pParent->GetSafeHwnd();
		if( hWndParent )
			::EnableWindow(hWndParent, 0);

		BOOL bRet = CreateDlgIndirect(m_lpDialogTemplate, pParent, AfxGetInstanceHandle()); 
		if( !bRet ){
			::EnableWindow(hWndParent, 1);
			ASSERT(FALSE);
			return 0;
			}

		// Set modal loop attribute.
		::SetProp(m_hWnd, _T("HasModalLoop"), (HANDLE)1);
		if ( pParent )
			::SetProp(*pParent, _T("ChildWnd"), (HANDLE)m_hWnd);

		if( dwStyle&DS_CENTER )
			CenterWindow();

		MSG					msg;
		HWND				hWnd		= m_hWnd;
		CWnd*				pWnd		= NULL;
		ESFrameBase*		pFrameWnd	= NULL;
		MercuryBaseView*	pMainView	= NULL;

		while( ::GetMessageA(&msg, NULL, 0, 0)  ){
			/*
			if( msg.hwnd == hWnd ){
				if( msg.message == WM_DESTROY || msg.message == WM_CLOSE || msg.message == WM_NULL ){
					if( hWndParent ){
						::EnableWindow(hWndParent, 1);
						if (hWndParent != NULL && ::GetActiveWindow() == hWnd)
							::SetActiveWindow(hWndParent);
						}
					//nRet = (int)msg.wParam;
					nRet = m_nModalResult;
					DestroyWindow();
					break;
					}
				}*/

			if( msg.message == WM_DESTROY /*|| msg.message == WM_CLOSE*/ || msg.message == WM_NULL ){
				if( msg.hwnd == hWnd ){
					if( hWndParent ){
						::EnableWindow(hWndParent, 1);
						if (hWndParent != NULL && ::GetActiveWindow() == hWnd)
							::SetActiveWindow(hWndParent);
						// Post PostQuit message to parent window.
						//if (msg.message == WM_QUIT)
						//	::PostMessage(hWndParent, msg.message, 0L, 0L);
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

			
			/*
				if( PreTranslateMessage(&msg) ){pWnd = CWnd::FromHandle(msg.hwnd);
				if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
					pFrameWnd	= (ESFrameBase*)pWnd;
					pMainView	= pFrameWnd->m_pMainWnd;
					if( !ContinueModal() ){
						m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);
						nRet = m_nModalResult;
						break;
						}
					}*/

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
	}

BOOL
ESFrameBase::Create(CWnd* pParent, DWORD dwStyle, DWORD dwStyleEx, UINT nMenuId){
	if( !dwStyle )
		dwStyle = (DS_SETFONT|DS_FIXEDSYS|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME);

	if( !dwStyleEx )
		dwStyleEx = WS_EX_APPWINDOW;

	ResourceCompiler*	pCompiler		= ResourceCompiler::GetCompiler();
	UINT				nSize			= 0;
	BYTE*				lpDlgTemplate	= new BYTE[1024];
	
	_String	sResourceScript	= 
_T("IDD_POKER_TABLE_DIALOG DIALOGEX 0, 0, 551, 314 \
STYLE DS_SETFONT | DS_CENTER | DS_FIXEDSYS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME \
EXSTYLE WS_EX_APPWINDOW \
CAPTION ~Poker Table~ %s\
FONT 8, ~MS Shell Dlg~, 0, 0, 0x1 \
BEGIN \
END");
	sResourceScript.Replace('~', '"'); 

	_String sScriptFormated;
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
		if (pParent){
			_Rect rcDialog;
			::GetWindowRect(m_hWnd, &rcDialog);
			_Rect rcParent;
			::GetWindowRect(*pParent, &rcParent);

			if (rcDialog.Width() <= rcParent.Width()){
				int nWidth = rcDialog.Width();
				rcDialog.left = rcParent.left + (rcParent.Width() - nWidth) / 2;
				rcDialog.right = rcDialog.left + nWidth;
				}

			if (rcDialog.Height() <= rcParent.Height()){
				int nHeight = rcDialog.Height();
				rcDialog.top = rcParent.top + (rcParent.Height() - nHeight) / 2;
				rcDialog.bottom = rcDialog.top + nHeight;
				}

			::SetWindowPos(m_hWnd, NULL, rcDialog.left, rcDialog.top, rcDialog.Width(), rcDialog.Height(), SWP_NOSIZE);
			}
		
		UpdateWindow();
//		ShowWindow(SW_SHOW);
		}
	return bRet;
	}

BOOL
ESFrameBase::CreateModalWitoutRunloop(CWnd* pParent, std::function<void(ESFrameBase*, UINT)> fnOnEndDialog, DWORD dwStyle, DWORD dwStyleEx, UINT nMenuId){
	if (!Create(pParent, dwStyle, dwStyleEx, nMenuId))
		return FALSE;
	m_fnOnEndDialog = fnOnEndDialog;
	m_bRunloopLessDialog = true;

	// Disable parent window.
	if (pParent)
		::EnableWindow(*pParent, 0);
	return TRUE;
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
ESFrameBase::PostPacketReceivedMsg(Serializable* packet, void* pParam){
	if( !IsCurrentWindowThread() ){
		Serializable* p1 = packet->CreateSerializableObject();
		packet->Copy(p1);
		::PostMessage(m_hWnd, WM_PACKET_RECEIVED, (WPARAM)p1, (LPARAM)pParam);
		return true;
		}
	return false;
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
	
	if (m_bRunloopLessDialog){
		m_fnOnEndDialog(this, (UINT)nRetCode);
		PostMessage(WM_CLOSE, nRetCode, 0);
		}
	else{
		EndDialog(nRetCode);

		if (::IsWindow(m_hWnd))
			PostMessage(WM_CLOSE, nRetCode, 0);
		}
	}

void
ESFrameBase::OnClose(){
	CDialog::OnClose();

	if (m_bRunloopLessDialog){
		_AFX_THREAD_STATE*	pState = AfxGetThreadState();
		UINT				wParam = (UINT)(pState->m_lastSentMsg.wParam);
		if (wParam == 0){ // Framework sending indicator. ESC key
			OnCancel();
		}

		::DestroyWindow(m_hWnd);
		delete this;
		}
	else{
		// End modal loop.
		if (m_nFlags & WF_CONTINUEMODAL)
			EndModalLoop(IDOK);
		else
			EndDialog(IDOK);
		}
	}

void
ESFrameBase::OnDestroy(){
	CDialog::OnDestroy();

	// Enable and activate parent window.
	if (m_bRunloopLessDialog){
		HWND hParentWnd = ::GetParent(m_hWnd);
		if (hParentWnd){
			::EnableWindow(hParentWnd, 1);
			::SetActiveWindow(hParentWnd);
			}
		}

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

	/*
	if( nType == SIZE_MAXIMIZED ){
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
				// Remove miximized style.
				DWORD dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
				dwStyle &= ~WS_MAXIMIZE;
				SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
				}
			}
		}*/

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
ESFrameBase::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp){
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
	}

LRESULT
ESFrameBase::OnNcHitTest(CPoint pt){
	LRESULT lRet	= CDialog::OnNcHitTest(pt);

	// Bring back arrow cursor.
	if( lRet != HTCLIENT ){
		if( m_bMouseUnderClientArea ){
			//SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			m_bMouseUnderClientArea = false;
			}
		}
	else{
		m_bMouseUnderClientArea = true;
		}

	m_nHitTestCode	= lRet;
	return lRet;
	}

void
ESFrameBase::OnWindowPosChanging(LPWINDOWPOS lpWndPos){
	if( lpWndPos->cx > 0 && lpWndPos->cy > 0 && lpWndPos->hwnd == m_hWnd && m_fHeightAndWidthRatio > 0.0f ){
		_Rect rcClient, rcWindow;
		GetClientRect(rcClient);
		GetWindowRect(rcWindow);

//		if( lpWndPos->x < 0 || lpWndPos->y < 0 )
//			m_nHitTestCode = 0;

		_Size szClient(rcClient.Width(), rcClient.Height()), szWindow(rcWindow.Width(), rcWindow.Height());
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
ESFrameBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	BOOL bRet = CDialog::OnSetCursor(pWnd, nHitTest, message);
	/*
	// Call handler.
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	UINT	msg		= pState->m_lastSentMsg.message;
	WPARAM	wParam	= pState->m_lastSentMsg.wParam;
	LPARAM	lParam	= pState->m_lastSentMsg.lParam;
	bRet			= (BOOL)CallMsgHandler(msg, wParam, lParam);
	*/
	return bRet;
	}

UINT
ESFrameBase::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

void
ESFrameBase::OnMouseLeave(){
	CDialog::OnMouseLeave();

	TRACE("MouseLeave %d \r\n", (int)time(0));
	// Restore arrow cursor.
	SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	}

void
ESFrameBase::OnNcMouseLeave(){
	CDialog::OnNcMouseLeave();

	TRACE("NcMouseLeave %d \r\n", (int)time(0));
	// Restore arrow cursor.
	//SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	//m_bMouseUnderNonClient = false;
	}

void
ESFrameBase::OnLButtonDown(UINT nFlags, CPoint pt){
	CDialog::OnLButtonDown(nFlags, pt);
	}

void
ESFrameBase::OnNcMouseHover(UINT nFlags, CPoint pt){
	CDialog::OnNcMouseHover(nFlags, pt);
	/*
	TRACE("NcMouseHover %d \r\n", (int)time(0));
	if( !m_bMouseUnderNonClient ){
		// Restore arrow cursor.
		//SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		m_bMouseUnderNonClient = true;
		}*/
	}

void
ESFrameBase::OnMouseMove(UINT nFlags, CPoint pt){
	CDialog::OnMouseMove(nFlags, pt);

	//TRACE("MouseMove %d \r\n", (int)time(0));
	
	TRACKMOUSEEVENT tme;
	tme.cbSize		= sizeof(tme);
	tme.dwFlags		= TME_HOVER|TME_NONCLIENT;
	tme.dwHoverTime = HOVER_DEFAULT;
	tme.hwndTrack	= m_hWnd;
	TrackMouseEvent(&tme);
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
ESFrameBase::GetCurrentSizeOmitBorder(_Point* pWindowPos /*= NULL*/){
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
		m_pMainWnd->AttachToWindow(this);
		/*
		_Rect rcClient;
		GetClientRect(rcClient);
		BOOL bRet = ((CWnd*)m_pMainWnd)->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, _Rect(0, 0, rcClient.Width(), rcClient.Height()), this, _MercuryViewId, NULL);
		if( bRet )
			m_pMainWnd->SetOwner(this);
			*/
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
	/*
	CWnd* pWnd = GetFocus();
	if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CEdit)) ){
		CEdit* pWndEdit = (CEdit*)pWnd;
		int nId = (int)GetWindowLong(*pWnd, GWL_ID);
		switch( nId ){
			case CHILD_SliderMoneyEdit:{
				if( GetKeyState(VK_CONTROL)&0x8000 ){
					// Call or check
					ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(1);
					if( pInfo )
						OnActionButtonClicked(pInfo);
					}
				else{
					// Bet or rise
					ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(2);
					if( pInfo )
						OnActionButtonClicked(pInfo);
					}
				break;
				}
		   // Add chips edit.
			case CHILD_AddChipsMoneyEdit:{
				int nAmount = m_pTableWnd->GetAddChipsAmount();
				OnAddChipsButtonClicked(nAmount);
				break;
				}

			// Send chat message.
			case CHILD_Table_Chat_Edit:{
				_String sChatMessage;
				pWndEdit->GetWindowText(sChatMessage);
				pWndEdit->SetWindowText(_T(""));
				m_client->SendChatMessage(m_nTableId, m_nTournamentId, sChatMessage);
				break;
				}
			};
		}
	else{
		if( GetKeyState(VK_CONTROL)&0x8000 ){
			// Call or check
			ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(1);
			if( pInfo && (pInfo->type == ButtonCall || pInfo->type == ButtonCheck) )
				OnActionButtonClicked(pInfo);
			}
		else{
			// Bet or rise
			ActionButtonInfo* pInfo = m_pTableWnd->GetActionButtonInfoByIdx(2);
			if( pInfo && (pInfo->type == ButtonRaise || pInfo->type == ButtonBet) )
				OnActionButtonClicked(pInfo);
			}
		}*/
	}

void
ESFrameBase::OnCancel(){
	if( m_pMainWnd && m_pMainWnd->IsDesignerMode() )
		ESFrameBase::CloseDialog(IDCANCEL);
	}