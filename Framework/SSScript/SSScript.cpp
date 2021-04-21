// SSScript.cpp : implementation file
//

#include "stdafx.h"
#include "SSScript.h"

#include <winsock.h>
#include <Winsvc.h>
//#include <shfolder.h>
#include <comdef.h>

#include "..\XML\Markup.h"

#include "..\Utility\FolderUtils.h"
#include "..\XMessageBox\XMessageBox.h"
/*
#include "MS-Cabinet\Cabinet.h"
#include "MS-Cabinet\Registry.h"
*/
#include "..\Utility\ImageResourceHacker.h"

#ifndef _SHLOBJ_H_
	#include "shlobj.h"
#endif


// SSScript class
SSScript::SSScript(){
	m_progress		= NULL;
	m_action_status	= NULL;
	}

SSScript::~SSScript(){
	UninitializeSSScript();
	}

BOOL
SSScript::Execute(CString& sExecutable, LPSTR sParam, HWND hWnd, BOOL bShowWindow){
	SHELLEXECUTEINFO shellInfo;
    ::ZeroMemory(&shellInfo, sizeof(shellInfo));
	shellInfo.hwnd			= hWnd;
    shellInfo.cbSize		= sizeof(shellInfo);
    shellInfo.fMask			= SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
    shellInfo.lpFile		= sExecutable;
   	shellInfo.lpParameters	= sParam;
	shellInfo.nShow			= bShowWindow ? SW_SHOW : SW_HIDE;
    
	BOOL bRet = ::ShellExecuteEx(&shellInfo);
    WaitForInputIdle	(shellInfo.hProcess, INFINITE );
	WaitForSingleObject	(shellInfo.hProcess, INFINITE);
	::CloseHandle(shellInfo.hProcess);
	return bRet;
	}

void 
SSScript::RegisterSetupPackage(){
	CString strInstallPath = "%InstallPath%";
	ReplaceKeywords(strInstallPath);

	CString strAppName = _T("%AppName%");
	ReplaceKeywords(strAppName);

	CString strAppVer = _T("%appver%");
	ReplaceKeywords(strAppVer);

	CString strUpdateVer = _T("%update_version%");
	ReplaceKeywords(strUpdateVer);

	/*
	CRegKey reg(HKEY_LOCAL_MACHINE);
	CRegistry reg(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + m_strCompany + "\\" + strAppName, KEY_ALL_ACCESS, TRUE);
	reg.SetValue("DisplayName",		(const BYTE*)strAppName.GetBuffer(),	strAppName.GetLength(),		REG_SZ);
	reg.SetValue("DisplayVersion",	(const BYTE*)strAppVer.GetBuffer(),		strAppVer.GetLength(),		REG_SZ);
	reg.SetValue("update_version",	(const BYTE*)strUpdateVer.GetBuffer(),	strUpdateVer.GetLength(),		REG_SZ);
	reg.SetValue("Publisher",		(const BYTE*)m_strCompany.GetBuffer(),	m_strCompany.GetLength(),	REG_SZ);
	reg.SetValue("InstallLocation", (const BYTE*)strInstallPath.GetBuffer(),strInstallPath.GetLength(), REG_SZ);*/
	}

BOOL 
SSScript::InstallFBirdODBCDriver(CString strInstallPath)
	{/*
	CString sRegValue	= "Installed";

	CRegistry reg_odbc(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBCINST.INI\\ODBC Drivers");
	reg_odbc.SetValue("Firebird/InterBase(r) driver", (const BYTE*)sRegValue.GetBuffer(), sRegValue.GetLength(), REG_SZ);
	
	reg_odbc.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBCINST.INI\\Firebird/InterBase(r) driver", KEY_ALL_ACCESS, TRUE);
	UINT nUseCount = 0x00000001;
	reg_odbc.SetValue("UsageCount",			(const BYTE*)&nUseCount, sizeof(UINT), REG_DWORD);
	
	CString strDriver	= strInstallPath + "\\OdbcJdbc.dll";
	reg_odbc.SetValue("Driver",				(const BYTE*)strDriver.GetBuffer(), strDriver.GetLength(), REG_SZ);
	strDriver			= strInstallPath + "\\OdbcJdbcSetup.dll";
	reg_odbc.SetValue("Setup",				(const BYTE*)strDriver.GetBuffer(), strDriver.GetLength(), REG_SZ);
	sRegValue			= "*.fdb,*.gdb";
	reg_odbc.SetValue("FileExtns",			(const BYTE*)sRegValue.GetBuffer(), sRegValue.GetLength(), REG_SZ );
	sRegValue			= "1";
	reg_odbc.SetValue("APILevel",			(const BYTE*)sRegValue.GetBuffer(), sRegValue.GetLength(), REG_SZ );
	sRegValue			= "YYY";
	reg_odbc.SetValue("ConnectFunctions",	(const BYTE*)sRegValue.GetBuffer(), sRegValue.GetLength(), REG_SZ );
	sRegValue			= "0";
	reg_odbc.SetValue("FileUsage",			(const BYTE*)sRegValue.GetBuffer(), sRegValue.GetLength(), REG_SZ );
	sRegValue			= "03.00";
	reg_odbc.SetValue("DriverODBCVer",		(const BYTE*)sRegValue.GetBuffer(), sRegValue.GetLength(), REG_SZ );
	sRegValue			= "*1";
	reg_odbc.SetValue("SQLLevel",			(const BYTE*)sRegValue.GetBuffer(), sRegValue.GetLength(), REG_SZ );*/
	return TRUE;
	}

/*  ----------------------------------------------------------------------------
**
**  Name           :SSScript::RegisterOcx
**
**  Description    :This function actually does the registering or unregistering.
**
**  Parameters     :CString pszDllName
**				   :BOOL bRegister
**
**  Returns        :BOOL 
**
**
**  --------------------------------------------------------------------------*/
BOOL 
SSScript::RegisterOcx(CString pszDllName, BOOL bRegister /*= TRUE*/){
	BOOL bReturn = FALSE;

	// Load the library.
    HINSTANCE hLib = LoadLibraryEx(pszDllName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	TCHAR szError[12];
    if( hLib < (HINSTANCE)HINSTANCE_ERROR ) {
        wsprintf(szError, _T("0x%08lx"), GetLastError());
        AfxMessageBox(szError);
        goto CleanupOle;
		}

    // Find the entry point.
	FARPROC lpDllEntryPoint;
	if( bRegister == TRUE )
		(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");
	else
		(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllUnregisterServer");

    if( lpDllEntryPoint == NULL ) {
		wsprintf(szError, _T("0x%08lx"), GetLastError());
		AfxMessageBox(szError);
		goto CleanupLibrary;
		}
	
    // Call the entry point.
    if( lpDllEntryPoint ){
		HRESULT hRes = (*lpDllEntryPoint)();
		if( FAILED(hRes) ){
			_com_error e(hRes);
			AfxMessageBox(e.ErrorMessage(),  MB_ICONEXCLAMATION);
			goto CleanupLibrary;
			}
		}

	//Horray
	bReturn = TRUE;

CleanupLibrary:
    FreeLibrary(hLib);
    
CleanupOle:
    OleUninitialize();
    return bReturn;
	}

BOOL
SSScript::ReadSSScript(){
	if( m_setup_ini.GetPathName().IsEmpty() ) return FALSE;
	CString sActionsQueue = m_setup_ini.GetString("install_action", "actions", "");
	LPTSTR lpszActions	  = sActionsQueue.GetBuffer();
	LPTSTR lpszActionsEnd = (LPTSTR)((long)lpszActions + sActionsQueue.GetLength()); 
	sActionsQueue.Replace(',','\x00');

	// add action into actions list. ==========================
	while( ((UINT)lpszActionsEnd) > ((UINT)lpszActions) ){
		int		nLen		= strlen(lpszActions);
		void*	lpKey		= 0;
		CString strAction	= lpszActions;
		int		nFind		= strAction.ReverseFind('@');
		
		// make lower action string.
		strAction.MakeLower();

		// find @ symbol in action name and romove that part.
		if( nFind != -1 )
			strAction = strAction.Left(nFind);

		if( strAction == _T("actions_label") ){
			CString sLabelActions		= m_setup_ini.GetString(lpszActions, "actions", "");
			LPTSTR	lpszLabelActions	= sLabelActions.GetBuffer();
			LPTSTR	lpszLabelActionsEnd = (LPTSTR)((long)lpszLabelActions + sLabelActions.GetLength()); 
			sLabelActions.Replace(',','\x00');

			// goto next action.
			lpszActions = (LPTSTR)((long)lpszActions + nLen + 1);
			while( ((UINT)lpszLabelActionsEnd) > ((UINT)lpszLabelActions) ){
				nLen		= strlen(lpszLabelActions);
				strAction	= lpszLabelActions;
				nFind		= strAction.ReverseFind('@');
				
				// make lower action string.
				strAction.MakeLower();

				// find @ symbol in action name and romove that part.
				if( nFind != -1 )
					strAction = strAction.Left(nFind);

				// retrive current action identifier. ==============
				if( m_setup_action_list.Lookup(strAction, lpKey) )
					{
					// save setup action function address and action name.
					char* pszAction = new char[strlen(lpszLabelActions)+1];
					strcpy(pszAction, lpszLabelActions);
					m_setup_actions.Add((DWORD)pszAction);
					m_setup_actions.Add(m_arrActionFunc[((int)(lpKey))]);
					m_arrDelete    .Add((DWORD)(pszAction));
					}
				// ========================================================

				// goto next lebel action.
				lpszLabelActions = (LPTSTR)((long)lpszLabelActions + nLen + 1);
				}
			continue;
			}
			
		// retrive current action identifier. ==============
		if( m_setup_action_list.Lookup(strAction, lpKey) ){
			// save setup action function address and action name.
			char* pszAction = new char[strlen(lpszActions)+1];
			strcpy(pszAction, lpszActions);
			m_setup_actions.Add((DWORD)pszAction);
			m_setup_actions.Add(m_arrActionFunc[((int)(lpKey))]);
			m_arrDelete    .Add((DWORD)(pszAction));
			}
		// ========================================================
		// goto next action.
		lpszActions = (LPTSTR)((long)lpszActions + nLen + 1);
		}
	return TRUE;
	}

void 
SSScript::InitSSScript(CString strSetupIni, HWND hOwner /*= NULL*/){
	// ZqrTalent Smart Setup Script.
	// initialize setup ini file. =============
	if( strSetupIni.Find(':') == -1 ){
		// retrive current instance path. ==========
		CString strPath = AfxGetApp()->m_pszHelpFilePath;
		int     nFind   = strPath.ReverseFind('\\');
		strPath         = strPath.Left(nFind + 1);
		// =========================================
		strSetupIni		= strPath + strSetupIni;
		}
	if( !strSetupIni.IsEmpty() )
		m_setup_ini.SetPathName(strSetupIni);
	// ========================================

	// Save owner window handle. ##############
	m_hOwner = hOwner;
	// ########################################

	// Initialization of action functions array and action list. =
	m_arrActionFunc.RemoveAll();
	m_arrActionFunc.SetSize(255);

	PROC func_action = (PROC)SSScript::Extract;
	m_arrActionFunc.SetAt(extract, ((DWORD)func_action));

	func_action = (PROC)SSScript::CreateShortcut;
	m_arrActionFunc.SetAt(create_shortcut, ((DWORD)func_action));

	func_action = (PROC)SSScript::RegOcx;
	m_arrActionFunc.SetAt(regocx, ((DWORD)func_action));

	func_action = (PROC)SSScript::RunDuringInstall;
	m_arrActionFunc.SetAt(run_during_install, ((DWORD)func_action));

	func_action = (PROC)SSScript::CopyFile;
	m_arrActionFunc.SetAt(copy_file, ((DWORD)func_action));

	func_action = (PROC)SSScript::CopyFolder;
	m_arrActionFunc.SetAt(copy_folder, ((DWORD)func_action));

	func_action = (PROC)SSScript::CreateFolder;
	m_arrActionFunc.SetAt(create_folder, ((DWORD)func_action));

	func_action = (PROC)SSScript::DeleteFile;
	m_arrActionFunc.SetAt(delete_file, ((DWORD)func_action));
	
	func_action = (PROC)SSScript::DeleteFolder;
	m_arrActionFunc.SetAt(delete_folder, ((DWORD)func_action));

	func_action = (PROC)SSScript::ReplaceIniKeywords;
	m_arrActionFunc.SetAt(replace_ini_keywords, ((DWORD)func_action));

	func_action = (PROC)SSScript::Sleep;
	m_arrActionFunc.SetAt(sleep, ((DWORD)func_action));

	func_action = (PROC)SSScript::InstallTTFFont;
	m_arrActionFunc.SetAt(install_ttf_font, ((DWORD)func_action));

	func_action = (PROC)SSScript::MoveFile;
	m_arrActionFunc.SetAt(move_file, ((DWORD)func_action));

	func_action = (PROC)SSScript::RegDeleteKey;
	m_arrActionFunc.SetAt(reg_delete_key, ((DWORD)func_action));

	func_action = (PROC)SSScript::WriteInFile;
	m_arrActionFunc.SetAt(write_in_file, ((DWORD)func_action));

	func_action = (PROC)SSScript::RegLoadXml;
	m_arrActionFunc.SetAt(reg_load_xml, ((DWORD)func_action));

	func_action = (PROC)SSScript::ReplaceImageRes;
	m_arrActionFunc.SetAt(replace_image_resources, ((DWORD)func_action));

	m_setup_action_list.SetAt("extract",					(void*)extract);
	m_setup_action_list.SetAt("create_shortcut",			(void*)create_shortcut);
	m_setup_action_list.SetAt("regocx",						(void*)regocx);
	m_setup_action_list.SetAt("run_during_install",			(void*)run_during_install);
	m_setup_action_list.SetAt("copy_file",					(void*)copy_file);
	m_setup_action_list.SetAt("copy_folder",				(void*)copy_folder);
	m_setup_action_list.SetAt("delete_file",				(void*)delete_file);
	m_setup_action_list.SetAt("delete_folder",				(void*)delete_folder);
	m_setup_action_list.SetAt("create_folder",				(void*)create_folder);
	m_setup_action_list.SetAt("replace_ini_keywords",		(void*)replace_ini_keywords);
	m_setup_action_list.SetAt("sleep",						(void*)sleep);
	m_setup_action_list.SetAt("install_ttf_font",			(void*)install_ttf_font);
	m_setup_action_list.SetAt("move_file",					(void*)move_file);
	m_setup_action_list.SetAt("reg_delete_key",				(void*)reg_delete_key);
	m_setup_action_list.SetAt("write_in_file",				(void*)write_in_file);
	m_setup_action_list.SetAt("reg_load_xml",				(void*)reg_load_xml);
	m_setup_action_list.SetAt("replace_image_resources",	(void*)replace_image_resources);
	// ===========================================================

	// initialize keyword names. =================================
	if( m_setup_ini.IsSectionExist(_T("keywords")) ){
		CString strKeyWord;
		int nLoop = 1;
		while( TRUE ){
			CString strKey;
			strKey.Format(_T("keyword%d"), nLoop);
			
			CString strValue	= m_setup_ini.GetString(_T("keywords"), strKey);
			int		nFindStart	= strValue.Find('%', 0);
			int		nFindEnd	= strValue.ReverseFind('%');
			if( (nFindEnd - nFindStart) > 0 ){
				int nFind		= strValue.Find(',', nFindEnd);
				if( nFind != -1 && nFind < strValue.GetLength() - 1 ){
					strKeyWord	= strValue.Right(strValue.GetLength() - nFind - 1);
					strKey		= strValue.Mid	(nFindStart, nFindEnd - nFindStart + 1);
					strKey.MakeLower();
					m_setup_keywords.SetAt(strKey, strKeyWord);
					}
				}
			else
				break;
			nLoop ++;
			}
		}
	
	/* Unique keywords.
	%SystemRoot%
	%WinDir%
	%Programs%
	%Desktop%
	%ProgramFiles%
	%CurrentDirectory%
	%Temp%
	*/
	char szPath[260];
	SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, 0, szPath);
	m_setup_keywords.SetAt("%systemroot%", szPath);
	m_setup_keywords.SetAt("%windir%", szPath);

	SHGetFolderPath(NULL, CSIDL_PROGRAMS, NULL, 0, szPath);
	m_setup_keywords.SetAt("%programs%", szPath);

	SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, szPath);
	m_setup_keywords.SetAt("%programfiles%", szPath);

	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
	m_setup_keywords.SetAt("%appdata%", szPath);

	SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, szPath);
	m_setup_keywords.SetAt("%desktop%", szPath);

	// retrieve temp directory. ================
	::GetTempPath(255, szPath);
	szPath[strlen(szPath) - 1] = '\x00';
	// =========================================
	m_setup_keywords.SetAt("%temp%", szPath);

	// retrive current instance path. ==========
	CString strPath = strSetupIni;
	int     nFind   = strPath.ReverseFind('\\');
	strPath         = strPath.Left(nFind);
	// =========================================
	m_setup_keywords.SetAt("%currentdirectory%", strPath);
	// ===========================================================

	// create temprary ini file. =================================
	HANDLE	hTempIni		= INVALID_HANDLE_VALUE;
	UINT	nTryCountMax	= 100;
	UINT	nLoop			= 1;
	CString strTempIni;

	::srand(::GetTickCount());
	while( hTempIni == INVALID_HANDLE_VALUE || nTryCountMax < nLoop ){
		CString strIniName;
		strIniName.Format("\\SSSTempIni%d.ini", ::rand());
		strTempIni  = szPath + strIniName;
		hTempIni	= CreateFile(strTempIni, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 
								 NULL, CREATE_NEW, 0L, NULL);
		nLoop ++;
		}
	if( hTempIni == INVALID_HANDLE_VALUE )
		AfxMessageBox("Couldn't create temporary ini file !!!");
	ASSERT( hTempIni != INVALID_HANDLE_VALUE );
	
	try{
		CFile	ini_file		(strSetupIni, CFile::modeRead);
		CFile	ini_temp_file	(hTempIni);
		UINT	nFileSize	= ini_file.GetLength();
		BYTE*	lpBuffer	= new BYTE[nFileSize + 1];
		ini_file.Read(lpBuffer, nFileSize);
		lpBuffer[nFileSize] = '\0';

		CString strContent	= (LPSTR)lpBuffer;
		ReplaceKeywords(strContent);
		
		ini_temp_file.Write	(strContent.GetBuffer(), strContent.GetLength());
		ini_temp_file.Close();
		delete lpBuffer;
		}
	catch( CFileException* e ){
		e->Delete();
		}
	// ===========================================================
	m_setup_ini.SetPathName		(strTempIni);
	// Delete keywords section. ==================================
	m_setup_ini.DeleteSection	(_T("keywords"));
	// retrive program executable file path. =====================
	m_strProgramExe = m_setup_ini.GetString("about", "progexe", "");
	// ===========================================================
	}

void 
SSScript::AddKeyWord(const CString strKeyword, CString& strKeywordValue){
	CString sKey = strKeyword;
	sKey.MakeLower();
	m_setup_keywords.SetAt(sKey, strKeywordValue);
	}

void 
SSScript::UninitializeSSScript(){
	UINT nCount = m_arrDelete.GetCount();
	for( int iLoop=0; iLoop<nCount; iLoop ++ ){
		if( m_arrDelete[iLoop] != 0L )
			delete (BYTE*)(m_arrDelete[iLoop]);
		}

	m_setup_keywords	.RemoveAll();
	m_arrDelete			.RemoveAll();
	m_arrActionFunc		.RemoveAll();
	m_setup_actions		.RemoveAll();
	m_setup_action_list	.RemoveAll();

	// Delete temporary ini file. =================
	CString strTempIni = m_setup_ini.GetPathName();
	::DeleteFile(strTempIni);
	// ============================================
	}

void
SSScript::ReplaceKeywords(CString& str){
	CString strKeyword;
	int		nFindStart	= str.Find('%'), nFindEnd = -1;
	if( nFindStart != -1 )
		nFindEnd	= str.Find('%', nFindStart + 1);

	int nFindStartOld = -1;
	while( nFindStart != -1 && nFindEnd != -1 ){
		// find keyword name. =========
		strKeyword	= str.Mid(nFindStart, (nFindEnd-nFindStart)+1);
		strKeyword.MakeLower();

		// find keyword string. =======
		CString strValue;
		if( m_setup_keywords.Lookup(strKeyword, strValue) ){
			// replace keyword with keyword string. ========
			CString strRight = str.Right(str.GetLength() - nFindEnd - 1);
			CString strLeft	 = str.Left (nFindStart);
			str.Format("%s%s%s", strLeft, strValue, strRight);
			// =============================================
			}
		// ============================

		nFindStart	= str.Find('%', nFindStart + 1);
		if( nFindStart != -1 )
			nFindEnd	= str.Find('%', nFindStart + 1);
		else
			break;
		}
	}

BOOL 
SSScript::RunSSScript(){
	typedef LRESULT (__cdecl *SetupActionFunc)(WPARAM, LPARAM);

	// prepare progress control. ==============
	if( m_progress ){
		m_progress->ShowWindow(SW_SHOW);
		m_progress->SetRange32(0, m_setup_actions.GetCount()/2);
		m_progress->SetPos(0);
		}
	// ========================================

	SSSActionParams params;
	params.pSetupIni = &m_setup_ini;
	params.pPutActionString = m_action_status;

	int nCount = m_setup_actions.GetCount();
	for( int iLoop = 0; iLoop < nCount; iLoop += 2 ){
		// retrive section name of Ini file and function address.
		void* strKey  = (void*)m_setup_actions[iLoop];
		void* lpValue = (void*)m_setup_actions[iLoop+1];

		// step progress.==========================
		if( m_progress )
			m_progress->SetPos(m_progress->GetPos()+1);
		PUMPMESSAGE();
		::Sleep(10);
		// ========================================
/*
		// Send notify to owner window. ===========
		UINT nRet = (UINT)SendMessage(m_hOwner, WM_SSS_NOTIFY, (WPARAM)iLoop/2, (LPARAM)nCount/2);
		if( nRet == FALSE )
			return FALSE;
		// ========================================
*/
		// call action function. ==================
		if( lpValue != NULL ){
			params.lpszSectionName = (LPTSTR)strKey;
			((SetupActionFunc)(lpValue))((WPARAM)this, (LPARAM)&params);
			}
		// ========================================
		}
	return TRUE;
	}

LRESULT 
SSScript::Extract(WPARAM wParam, LPARAM lParam){
	/*
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString strArchive			= pSetupIni->GetString(params->lpszSectionName, "cab",		   "");
	if( strArchive.IsEmpty() ) return FALSE;
	CString strExtractPath		= pSetupIni->GetString(params->lpszSectionName, "extract_path", "");
	BOOL			 bShowError	= pSetupIni->GetBool(params->lpszSectionName, "showerror", FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================
	
	CString strString;
	strString.Format("MS-Cabinet file(%s) extacting ...", strArchive.Right(strArchive.GetLength() - strArchive.ReverseFind('\\')-1));
	if( pPutString )
		if( pPutString ) pPutString->SetWindowText(strString);

	CCabinetExtractor cab;
	cab.SetDefaultExtractPath(strExtractPath);

	CABINETINFO cabinfo;
	if( !cab.GetCabinetInfo(strArchive.GetBuffer(), &cabinfo) ) // retrives cabinet information{
		if( bShowError )
			AfxMessageBox("Error: Files Not Found!");
		return FALSE;
		}

	cab.ExtractFiles(strArchive.GetBuffer());  //extracts files from cabinet */
	return TRUE;
	}

LRESULT 
SSScript::CreateShortcut(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString strLnkFile	= pSetupIni->GetString(params->lpszSectionName, "lnk", "");
	CString strExeFile	= pSetupIni->GetString(params->lpszSectionName, "exe", "");
	CString strDescFile	= pSetupIni->GetString(params->lpszSectionName, "desc", "");
	BOOL	bShowError	= pSetupIni->GetBool(params->lpszSectionName, "showerror", FALSE);

	// ask before do this operation. =======================================
	BOOL	bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore )
		{
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	// create shortcut. ===========================
	HRESULT hres=0;
    IShellLink *psl;

	// Create an IShellLink object and get a pointer to the IShellLink
	// interface (returned from CoCreateInstance).
    hres = CoCreateInstance(CLSID_ShellLink, NULL,  CLSCTX_ALL ,IID_IShellLinkA, (void **)&psl);
    if( SUCCEEDED(hres) ){
       IPersistFile *ppf;
  
	   // Query IShellLink for the IPersistFile interface for
	   // saving the shortcut in persistent storage.
       hres = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
	   if( SUCCEEDED(hres) ){	   
	        OLECHAR wsz[MAX_PATH];   // buffer for Unicode string
			
	   		// Set the path to the shortcut target.
          	hres = psl->SetPath(strExeFile);

			if( !SUCCEEDED(hres) && bShowError )
				AfxMessageBox("SetPath failed!");

            // Set the description of the shortcut.
          	hres = psl->SetDescription(strDescFile);
			//psl->SetShowCmd(SW_SHOWMAXIMIZED);

			if( !SUCCEEDED(hres) && bShowError )
				AfxMessageBox("SetDescription failed!");

	     	// Ensure that the string consists of ANSI characters.
	     	MultiByteToWideChar(CP_ACP, 0, strLnkFile, -1, wsz, MAX_PATH);
          	// Save the shortcut via the IPersistFile::Save member function.
          	hres = ppf->Save(wsz, TRUE);

			if( FAILED(hres) ){
				_com_error e(hres);
				if( bShowError )
					AfxMessageBox(e.ErrorMessage(),  MB_ICONEXCLAMATION);
				}

            // Release the pointer to IPersistFile.
          	ppf->Release();
			}
		// Release the pointer to IShellLink.
		psl->Release();
		}
	else{
		_com_error e(hres);
		if( bShowError )
			AfxMessageBox(e.ErrorMessage(),  MB_ICONEXCLAMATION);
		}
	return TRUE;
	}

LRESULT 
SSScript::RegOcx(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strOcxDll	= pSetupIni->GetString(params->lpszSectionName, "ocx", "");
	BOOL			 bRegister	= pSetupIni->GetBool(params->lpszSectionName, "reg", FALSE);
	BOOL			 bShowError	= pSetupIni->GetBool(params->lpszSectionName, "showerror", FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	CString strString;
	CString strFormat = bRegister ? "ActiveX file(%s) registering ..." : "ActiveX file(%s) unregistering ...";

	strString.Format(strFormat, strOcxDll.Right(strOcxDll.GetLength() - strOcxDll.ReverseFind('\\') - 1));
	if( pPutString ) pPutString->SetWindowText(strString);

	if( RegisterOcx(strOcxDll, bRegister) != TRUE ){
		strFormat = "<Error> " + strFormat;
		strString.Format(strFormat, strOcxDll.Right(strOcxDll.GetLength() - strOcxDll.ReverseFind('\\') - 1));
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	return TRUE;
	}

LRESULT 
SSScript::RunDuringInstall(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strFileToRun	= pSetupIni->GetString(params->lpszSectionName, "run", "");
	CString			 strParams		= pSetupIni->GetString(params->lpszSectionName, "params", "");
	BOOL			 bShowError		= pSetupIni->GetBool(params->lpszSectionName, "showerror", FALSE);
	BOOL			 bShowWindow	= pSetupIni->GetBool(params->lpszSectionName, "show_window", FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	CString strString;
	strString.Format("Running (%s) ...", strFileToRun); 
	if( pPutString ) pPutString->SetWindowText(strString);
	
	PUMPMESSAGE();
	::Sleep(10);

	Execute(strFileToRun, strParams.GetBuffer(), pPutString->m_hWnd, bShowWindow);
	return TRUE;
	}

LRESULT 
SSScript::CopyFile(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params			= (SSSActionParams*)lParam;
	SSScript*		 pThis			= (SSScript*)wParam;
	CIni*			 pSetupIni		= params->pSetupIni;
	CWnd*		     pPutString		= params->pPutActionString;

	CString			 strCopyFrom	= pSetupIni->GetString(params->lpszSectionName, "copy_from",			"");
	CString			 strCopyTo		= pSetupIni->GetString(params->lpszSectionName, "copy_to",				"");
	BOOL			 bShowError		= pSetupIni->GetBool(params->lpszSectionName,	"showerror",			FALSE);
	BOOL			 bOverWrite		= pSetupIni->GetBool(params->lpszSectionName,	"overwrite_existing",	FALSE);

	// ask before do this operation. #######################################
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// #####################################################################

	CString strString;
	strString.Format("Copy file to ""(%s)"" ...", strCopyTo);
	if( pPutString ) pPutString->SetWindowText(strString);

	// if not overwrite and file exists then quit. ==========================
	if( !bOverWrite ){
		HANDLE hFile = CreateFile(strCopyTo, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0L, NULL);
		if( hFile != INVALID_HANDLE_VALUE )
			{
			CloseHandle(hFile);
			return FALSE;
			}
		}
	// =====================================================================

	if( !::CopyFile(strCopyFrom, strCopyTo, FALSE) ){
		strString.Format("Copy file to ""(%s)"" ...", strCopyTo);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	return TRUE;
	}

LRESULT 
SSScript::WriteInFile(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strFile	= pSetupIni->GetString	(params->lpszSectionName, "file",		"");
	UINT			 nAddress	= pSetupIni->GetInt		(params->lpszSectionName, "address",	0, 16);
	BOOL			 bShowError	= pSetupIni->GetBool	(params->lpszSectionName, "showerror",	FALSE);
	CString			 strData	= pSetupIni->GetString	(params->lpszSectionName, "data",		"");

	HANDLE			 hFile		= CreateFile(strFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
											NULL, OPEN_EXISTING, 0L, NULL);
	if( hFile == INVALID_HANDLE_VALUE ){
		CString strString;
		strString.Format("<Error> CreateFile ""(%s)"" ...", strFile);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	if( SetFilePointer(hFile, nAddress, NULL, FILE_CURRENT) != nAddress ){
		CString strString;
		strString.Format("<Error> SetFilePointer ""(%s)(%d)"" ...", strFile, nAddress);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		CloseHandle(hFile);
		return FALSE;
		}

	DWORD dwWritten = 0L;
	if( WriteFile(hFile, strData.GetBuffer(), strData.GetLength(), &dwWritten, NULL) == FALSE || 
		dwWritten != strData.GetLength() ){
		CString strString;
		strString.Format("<Error> WriteFile ""(%s)"" ...", strFile);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		CloseHandle(hFile);
		return FALSE;
		}

	CloseHandle(hFile);
	return TRUE;
	}

LRESULT 
SSScript::RegDeleteKey(WPARAM wParam, LPARAM lParam){/*
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strRegKey	= pSetupIni->GetString(params->lpszSectionName, "subkey",		"");
	CString			 strRootKey = pSetupIni->GetString(params->lpszSectionName, "key",			"");
	BOOL			 bShowError	= pSetupIni->GetBool(params->lpszSectionName,	"showerror",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName,	"ask_before",	FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	HKEY hRootKey = HKEY_CLASSES_ROOT;
	if( strRootKey == _T("HKEY_CURRENT_CONFIG") ){
		hRootKey = HKEY_CURRENT_CONFIG;
		}
	else
	if( strRootKey == _T("HKEY_CURRENT_USER") ){
		hRootKey = HKEY_CURRENT_USER;
		}
	else
	if( strRootKey == _T("HKEY_LOCAL_MACHINE") ){
		hRootKey = HKEY_LOCAL_MACHINE;
		}
	else
	if( strRootKey == _T("HKEY_USERS") ){
		hRootKey = HKEY_USERS;
		}

	CRegistry reg(hRootKey, strRegKey);
	if( reg.Delete() != ERROR_SUCCESS ){
		CString strString;
		strString.Format("<Error> Delete registry key ""(%s)"" ...", strRegKey);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}*/
	return TRUE;
	}

LRESULT 
SSScript::MoveFile(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params				= (SSSActionParams*)lParam;
	SSScript*		 pThis				= (SSScript*)wParam;
	CIni*			 pSetupIni			= params->pSetupIni;
	CWnd*		     pPutString			= params->pPutActionString;

	CString			 strExistingFile	= pSetupIni->GetString	(params->lpszSectionName, "existing_file",	"");
	CString			 strNewFile			= pSetupIni->GetString	(params->lpszSectionName, "newfile",		"");
	BOOL			 bShowError			= pSetupIni->GetBool	(params->lpszSectionName, "showerror",		FALSE);

	// ask before do this operation. #######################################
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// #####################################################################

	CString strString;
	strString.Format("Moving file ""(%s)"" ...", strExistingFile);
	if( pPutString ) pPutString->SetWindowText(strString);

	if( ::MoveFile(strExistingFile, strNewFile) == FALSE ){
		strString.Format("<Error> Move file ""(%s)"" ...", strExistingFile);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	return TRUE;
	}

LRESULT 
SSScript::CopyFolder(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params			= (SSSActionParams*)lParam;
	SSScript*		 pThis			= (SSScript*)wParam;
	CIni*			 pSetupIni		= params->pSetupIni;
	CWnd*		     pPutString		= params->pPutActionString;

	CString			 strCopyFrom	= pSetupIni->GetString	(params->lpszSectionName, "copy_from",			"");
	CString			 strCopyTo		= pSetupIni->GetString	(params->lpszSectionName, "copy_to",			"");
	BOOL			 bShowError		= pSetupIni->GetBool	(params->lpszSectionName, "showerror",			FALSE);
	BOOL			 bOverWrite		= pSetupIni->GetBool	(params->lpszSectionName, "overwrite_existing", FALSE);
	BOOL			 bCopyOnlyFiles	= pSetupIni->GetBool	(params->lpszSectionName, "copy_only_files",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	// copy folder. ========================================================
	CopyFolders(pPutString, strCopyFrom, strCopyTo, 
				bShowError, bOverWrite, bCopyOnlyFiles);
	// =====================================================================
	return TRUE;
	}

BOOL 
SSScript::CopyFolders(CWnd* pWndPutString, CString& strCopyFrom, CString& strCopyTo, 
					  BOOL bShowError, BOOL bOverWrite, BOOL bCopyOnlyFiles){
	CFileFind fnd;
	BOOL bFind = fnd.FindFile(strCopyFrom + "\\*.*");

	CString strString;
	strString.Format("Copy folder ""(%s)"" ...", strCopyFrom);
	if( pWndPutString )
		pWndPutString->SetWindowText(strString);
	if( CFolderUtils::CreateFolder(strCopyTo) != TRUE ){
		strString.Format("<Error> Copy folder ""(%s)"" ...", strCopyFrom);
		if( pWndPutString )
			pWndPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	while( bFind ){
		bFind = fnd.FindNextFile();

		if( fnd.IsDots() ) continue;
		if( fnd.IsDirectory() ){
			if( bCopyOnlyFiles == FALSE )
				CopyFolders(pWndPutString, fnd.GetFilePath(), strCopyTo + "\\" + fnd.GetFileName(), 
							bShowError, bOverWrite, bCopyOnlyFiles);
			else
				CopyFolders(pWndPutString, fnd.GetFilePath(), strCopyTo, 
							bShowError, bOverWrite, bCopyOnlyFiles);
			}
		else{
			CString strString;
			strString.Format("Copy file ""(%s)"" ...", fnd.GetFileName());
			if( pWndPutString )
				pWndPutString->SetWindowText(strString);

			CString strFileCopyTo = strCopyTo + "\\" + fnd.GetFileName();

			BOOL bSkipCopy = FALSE;
			if( bOverWrite == FALSE ){
				HANDLE hFile = CreateFile(strFileCopyTo, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0L, NULL);
				if( hFile != INVALID_HANDLE_VALUE ){
					CloseHandle(hFile);
					bSkipCopy = TRUE;
					}
				}

			if( bSkipCopy == FALSE )
				if( ::CopyFile(fnd.GetFilePath(), strFileCopyTo, FALSE) != TRUE ){
					strString.Format("<Error> Copy file ""(%s)"" ...", fnd.GetFileName());
					if( pWndPutString )
						pWndPutString->SetWindowText(strString);
					if( bShowError )
						AfxMessageBox(strString);
					}
			}
		}
	return TRUE;
	}

LRESULT 
SSScript::CreateFolder(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strFolder	= pSetupIni->GetString(params->lpszSectionName, "folder",		"");
	BOOL			 bShowError	= pSetupIni->GetBool(params->lpszSectionName,	"showerror",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName,	"ask_before",	FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	CString strString;
	strString.Format("Creating folder ""(%s)"" ...", strFolder);
	if( pPutString ) pPutString->SetWindowText(strString);
	
	if( CFolderUtils::CreateFolder(strFolder) != TRUE ){
		HANDLE hFolder = CreateFile(strFolder, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0L, NULL);
		if( hFolder != INVALID_HANDLE_VALUE ){
			strString.Format("<Error> Creating file ""(%s)"" ...", strFolder);
			if( pPutString ) pPutString->SetWindowText(strString);
			if( bShowError )
				AfxMessageBox(strString);
			return FALSE;
			}
		CloseHandle(hFolder);
		}

	return TRUE;
	}

LRESULT 
SSScript::DeleteFile(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strFile	= pSetupIni->GetString	(params->lpszSectionName, "file",		"");
	BOOL			 bShowError	= pSetupIni->GetBool	(params->lpszSectionName, "showerror",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	CString strString;
	strString.Format("Deleting file ""(%s)"" ...", strFile);
	if( pPutString ) pPutString->SetWindowText(strString);
	
	if( ::DeleteFile(strFile) != TRUE ){
		strString.Format("<Error> Deleting file ""(%s)"" ...", strFile);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	return TRUE;
	}

LRESULT 
SSScript::ReplaceIniKeywords(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strIniFile	= pSetupIni->GetString(params->lpszSectionName, "ini", "");
	pThis->ReplaceKeywords(strIniFile);
	CString			 strIniContent;
	BOOL			 bShowError	= pSetupIni->GetBool(params->lpszSectionName, "showerror", FALSE);

	// ask before do this operation. ########################################
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// ######################################################################

	// open ini file. #######################################################
	HANDLE hIniFile = CreateFile(strIniFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
								 OPEN_EXISTING, 0L, NULL);

	if( hIniFile != INVALID_HANDLE_VALUE ){
		// replace ini file keywords. #######################################
		CFile file_ini(hIniFile);
		UINT  nFileSize = (UINT)file_ini.GetLength();
		file_ini.Read(strIniContent.GetBufferSetLength(nFileSize), nFileSize);
		pThis->ReplaceKeywords(strIniContent);
		file_ini.Close();

		file_ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite);
		file_ini.Write(strIniContent.GetBuffer(), strIniContent.GetLength());
		file_ini.Close();
		// ##################################################################
		return TRUE;
		}
	return FALSE;
	}

LRESULT 
SSScript::DeleteFolder(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strFolder	= pSetupIni->GetString	(params->lpszSectionName, "folder",		"");
	BOOL			 bShowError	= pSetupIni->GetBool	(params->lpszSectionName, "showerror",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	CString strString;
	strString.Format("Deleting folder ""(%s)"" ...", strFolder);
	if( pPutString ) pPutString->SetWindowText(strString);
	
	if( CFolderUtils::RemoveFolder(strFolder) != TRUE ){
		strString.Format("<Error> Deleting folder ""(%s)"" ...", strFolder);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	return TRUE;
	}

LRESULT 
SSScript::InstallTTFFont(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strFontFace= pSetupIni->GetString	(params->lpszSectionName, "ttffontface",	"");
	CString			 strFontFile= pSetupIni->GetString	(params->lpszSectionName, "ttffontfile",	"");
	BOOL			 bShowError	= pSetupIni->GetBool	(params->lpszSectionName, "showerror",		FALSE);
	BOOL			 bOnlyReg	= pSetupIni->GetBool	(params->lpszSectionName, "only_register",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	CString strString;
	strString.Format("Installing font ""(%s)"" ...", strFontFace);
	if( pPutString ) pPutString->SetWindowText(strString);

	// registering font file. ====================================
	CString strValueName;
	strValueName.Format("%s (True Type)", strFontFace);
	/*
	CRegistry reg(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
	reg.SetValue(strValueName, (const BYTE*)strFontFile.GetBuffer(), strFontFile.GetLength(), REG_SZ);
	*/
	// ===========================================================

	// install font to system memory. ============================
	if( bOnlyReg == FALSE ){
		if( ::AddFontResource(strFontFile) == 0 ){
			strString.Format("<Error> Couldn't install ttf font! ""(%s)"" ...", strFontFace);
			if( pPutString ) pPutString->SetWindowText(strString);
			if( bShowError )
				AfxMessageBox(strString);
			return FALSE;
			}
		}
	// ===========================================================
	return TRUE;
	}

LRESULT
SSScript::RegLoadXml(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			 strXmlFile	= pSetupIni->GetString(params->lpszSectionName, "xmlfile",		"");
	BOOL			 bShowError	= pSetupIni->GetBool(params->lpszSectionName,	"showerror",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName,	"ask_before",	FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	CMapStringToPtr mapRootNames;
	mapRootNames.SetAt(_T("HKEY_CURRENT_CONFIG"),	(void*)HKEY_CURRENT_CONFIG	);
	mapRootNames.SetAt(_T("HKEY_CLASSES_ROOT"),		(void*)HKEY_CLASSES_ROOT	);
	mapRootNames.SetAt(_T("HKEY_CURRENT_USER"),		(void*)HKEY_CURRENT_USER	);
	mapRootNames.SetAt(_T("HKEY_LOCAL_MACHINE"),	(void*)HKEY_LOCAL_MACHINE	);
	mapRootNames.SetAt(_T("HKEY_USERS"),			(void*)HKEY_USERS			);

	CMapStringToPtr mapValueTypes;
	mapValueTypes.SetAt(_T("REG_BINARY"),	(void*)REG_BINARY);
	mapValueTypes.SetAt(_T("REG_DWORD"),	(void*)REG_DWORD );
	mapValueTypes.SetAt(_T("REG_SZ"),		(void*)REG_SZ);

	CMarkup reg_xml;
	if( !reg_xml.Load(strXmlFile) ){
		AfxMessageBox(reg_xml.GetError());
		return FALSE;
		}
	
	if( reg_xml.FindElem(_T("registry_keys")) && reg_xml.IntoElem() ){
		while( reg_xml.FindElem("registry_key") ){
			HKEY	hKey		= NULL;
			HKEY	hKeyRoot	= NULL;

			// Fint root key name. ##############################
			CString sKeyPath	= reg_xml.GetAttrib(_T("key_path"));
			int		nFirstSlesh = sKeyPath.Find('\\');
			CString	sRootKey	= _T("");
			CString sSubKey		= _T("");

			if( nFirstSlesh == -1 ){
				sRootKey = sKeyPath;
				sRootKey.MakeUpper();
				}
			else{
				sRootKey = sKeyPath.Left(nFirstSlesh + 1);
				sRootKey.MakeUpper();
				sSubKey  = sKeyPath.Right(sKeyPath.GetLength() - sRootKey.GetLength() - 1);
				}
			// ################################################

			// Find root key value. ###########################
			void* lpValue = 0L;
			if( mapRootNames.Lookup(sRootKey, lpValue) )
				hKeyRoot = (HKEY)lpValue;
			// ################################################

			// Load xml key to registry. ######################
			if( !sSubKey.IsEmpty() ){
				if( ::RegOpenKey(hKeyRoot, sSubKey, &hKey) == ERROR_SUCCESS ){
					reg_xml.IntoElem	();
					LoadKeyToRegistry	(&reg_xml, hKey, &mapValueTypes);
					reg_xml.OutOfElem	();
					RegCloseKey			(hKey);
					}
				}
			else
				if( hKeyRoot != NULL ){
					reg_xml.IntoElem	();
					LoadKeyToRegistry	(&reg_xml, hKeyRoot, &mapValueTypes);
					reg_xml.OutOfElem	();
					}
			// ################################################
			}
		}
	return TRUE;
	}

BOOL	
SSScript::LoadKeyToRegistry(CMarkup* pXml, HKEY hKey, CMapStringToPtr* pMapValueTypes){
	while( pXml->FindElem(_T("key")) ){
		CString sKeyName	= pXml->GetAttrib(_T("name"));
		CString sValueName, sValueType, sValue;
		int		nValueIndex	= 1;
		HKEY	hNewKey		= NULL;

		if( ::RegCreateKey(hKey, sKeyName, &hNewKey) == ERROR_SUCCESS ){
			while( TRUE ){
				sValueName	.Format("value%d_name", nValueIndex);
				sValueType	.Format("value%d_type", nValueIndex);
				sValue		.Format("value%d",		nValueIndex);

				sValueType		= pXml->GetAttrib(sValueType);
				sValueName		= pXml->GetAttrib(sValueName);
				sValue			= pXml->GetAttrib(sValue);
				nValueIndex		++;

				if( sValueType.IsEmpty() )
					break;

				void* lpValue = NULL;
				if( pMapValueTypes->Lookup(sValueType.MakeUpper(), lpValue) ){
					UINT nValueType = (UINT)lpValue;
					switch( nValueType )
						{
						case REG_BINARY:{
							break;
							}
						case REG_DWORD:{
							RegSetValueEx(hNewKey, sValueName, 0L, REG_SZ, (BYTE*)sValue.GetBuffer(), sValue.GetLength());
							break;
							}
						case REG_SZ:{
							sValue.Replace("'", """");
							RegSetValueEx(hNewKey, sValueName, 0L, REG_SZ, (BYTE*)sValue.GetBuffer(), sValue.GetLength());
							break;
							}
						};
					}
				}
			// If have child, call function recursively. ############
			if( pXml->IntoElem() ){
				LoadKeyToRegistry(pXml, hNewKey, pMapValueTypes);
				pXml->OutOfElem();
				}
			// ######################################################
			RegCloseKey(hNewKey);
			}
		}

	return TRUE;
	}

LRESULT 
SSScript::ReplaceImageRes(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	CString			strImageIn	= pSetupIni->GetString(params->lpszSectionName, "image_in",				"");
	CString			strImageOut	= pSetupIni->GetString(params->lpszSectionName, "image_out",			"");
	BOOL			bShowError	= pSetupIni->GetBool(params->lpszSectionName,	"showerror",			FALSE);
	BOOL			bOverWrite	= pSetupIni->GetBool(params->lpszSectionName,	"overwrite_existing",	FALSE);

	// ask before do this operation. =======================================
	BOOL	bAskBefore	= pSetupIni->GetBool(params->lpszSectionName, "ask_before", FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	ImageResourceHacker res_hacker;
	if( !res_hacker.LoadImageFile(strImageIn) ){
		CString strString;
		strString.Format("<Error> Couldn't load image file! ""(%s)"" ...", strImageIn);
		if( pPutString ) pPutString->SetWindowText(strString);
		if( bShowError )
			AfxMessageBox(strString);
		return FALSE;
		}

	int nLoop = 0;
	while( TRUE ){
		CString sKeyType;
		sKeyType.Format("replace_res_type%d", nLoop + 1);
		CString sKeyName;
		sKeyName.Format("replace_res_name%d", nLoop + 1);
		CString sKeyReplaceWith;
		sKeyReplaceWith.Format("replace_res_with%d", nLoop + 1);

		if( !pSetupIni->IsKeyExist(params->lpszSectionName, sKeyType)		|| 
			!pSetupIni->IsKeyExist(params->lpszSectionName, sKeyName)		|| 
			!pSetupIni->IsKeyExist(params->lpszSectionName, sKeyReplaceWith)){
			break;
			}

		CString sResName		= pSetupIni->GetString(params->lpszSectionName, sKeyName);
		CString sResType		= pSetupIni->GetString(params->lpszSectionName, sKeyType);
		CString sResReplaceWith = pSetupIni->GetString(params->lpszSectionName, sKeyReplaceWith);

		if( !res_hacker.ReplaceResource(sResType, sResName, sResReplaceWith) ){
			CString strString;
			strString.Format("<Error> Couldn't replace resource %s!"" ...", sResName);
			if( pPutString ) pPutString->SetWindowText(strString);
			if( bShowError )
				AfxMessageBox(strString);
			return FALSE;
			}
		nLoop ++;
		}

	if( nLoop > 0 ){
		if( !res_hacker.FlushImage(strImageOut, bOverWrite) ){
			CString strString;
			strString.Format("<Error> Couldn't flush image %s!"" ...", strImageOut);
			if( pPutString ) pPutString->SetWindowText(strString);
			if( bShowError )
				AfxMessageBox(strString);
			return FALSE;
			}
		}

	return TRUE;
	}

LRESULT 
SSScript::Sleep(WPARAM wParam, LPARAM lParam){
	ASSERT( wParam != 0L && lParam != 0L );
	SSSActionParams* params		= (SSSActionParams*)lParam;
	SSScript*		 pThis		= (SSScript*)wParam;
	CIni*			 pSetupIni	= params->pSetupIni;
	CWnd*		     pPutString	= params->pPutActionString;

	UINT			 nTime		= pSetupIni->GetInt(params->lpszSectionName,	"time",			0);
	BOOL			 bShowError	= pSetupIni->GetBool(params->lpszSectionName,	"showerror",	FALSE);

	// ask before do this operation. =======================================
	BOOL			 bAskBefore	= pSetupIni->GetBool(params->lpszSectionName,	"ask_before",	FALSE);
	if( bAskBefore ){
		CString	strAsk = pSetupIni->GetString(params->lpszSectionName, "ask_title", "");
		if( pPutString && XMessageBox(pPutString->m_hWnd, strAsk, _T("GeoMegasoft"), MB_YESNO|MB_ICONQUESTION, NULL) == IDNO )
			return FALSE;
		}
	// =====================================================================

	if( nTime > 0 ){
		DWORD dwCurrThreadId	= GetCurrentThreadId();
		DWORD dwProcessId		= 0L;
		DWORD dwWndThreadId		= ::GetWindowThreadProcessId(pPutString->m_hWnd, &dwProcessId);

		if( dwCurrThreadId == dwWndThreadId ) // check for current thread.
			{
			UINT nTimerId  = 1;
			HWND hTimerWnd = pPutString->m_hWnd;
			pPutString->SetTimer(1, 1000, NULL);

			MSG msg;
			while( GetMessage( &msg, hTimerWnd, NULL, NULL ) ){
				TranslateMessage(&msg);
				if( msg.message == WM_TIMER  && 
					msg.hwnd	== hTimerWnd && 
					msg.wParam	== nTimerId	  )
					{
					CString strString;
					strString.Format("ïÅ÷ùìÅ üùøñôâùÅ %d öôüø ...", nTime);
					if( pPutString ) pPutString->SetWindowText(strString);

					nTime --;
					if( !nTime ) {
						pPutString->KillTimer(1);
						break;
						}
					}
				DispatchMessage(&msg);
				}
			}
		else{
			for( int nLoop=0; nLoop<nTime; nLoop++  ){
				CString strString;
				strString.Format("ïÅ÷ùìÅ üùøñôâùÅ %d öôüø ...", nTime - nLoop);
				if( pPutString ) pPutString->SetWindowText(strString);

				::Sleep(1000);
				}
			}
		}
	return TRUE;
	}
