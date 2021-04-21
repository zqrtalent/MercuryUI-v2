#pragma once

// SSScript
#include "afxwin.h"
#include "afxcmn.h"
#include "Ini.h"

#define WM_SSS_NOTIFY WM_USER + 50

#define PUMPMESSAGE()\
		{\
			MSG msg;\
			while( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )\
			{\
				TranslateMessage(&msg);\
				DispatchMessage(&msg);\
			}\
		}

class CMarkup;
class SSScript
	{
public:
	SSScript();
	~SSScript();

public:
	// static functions. ===============================
	 static BOOL	RegisterOcx				(CString pszDllName, BOOL bRegister = TRUE);
	 static BOOL	Execute					(CString& sExecutable, LPSTR sParam, HWND hWnd = NULL, BOOL bShowWindow = FALSE);
	 static BOOL	CopyFolders				(CWnd* pWndPutString, CString& strCopyFrom, CString& strCopyTo, BOOL bShowError, BOOL bOverWrite, BOOL bCopyOnlyFiles);
	 static BOOL	LoadKeyToRegistry		(CMarkup* pXml, HKEY hKey, CMapStringToPtr* pMapValueTypes);	
	 static BOOL	InstallFBirdODBCDriver	(CString strInstallPath);
	 // ================================================

	//  ZqrTalent smart setup script support. ==========
	enum SetupAction
		{
		extract = 0, 
		create_shortcut, 
		regocx, 
		run_during_install, 
		copy_file,
		copy_folder,
		delete_file,
		delete_folder,
		create_folder,
		replace_ini_keywords,
		sleep,
		install_ttf_font,
		move_file,
		reg_delete_key,
		write_in_file,
		reg_load_xml,
		replace_image_resources
		};

	 // Smart Script Setup action params.
	 struct SSSActionParams{
		LPTSTR  lpszSectionName;
		CIni*	pSetupIni;
		CWnd*   pPutActionString;
		};
	 // ################################

	 // Smart Setup Script file.
	 CIni					m_setup_ini;
	 // Array of pointers to delete.
	 CDWordArray			m_arrDelete;
	 // Action function addresses.
	 CDWordArray			m_arrActionFunc;
	 // Setup action name + action id.
	 CMapStringToPtr		m_setup_action_list;
	 // Section name of action(int setup ini file) + action function address.
	 CDWordArray			m_setup_actions;
	 // Map of keywords.
	 CMapStringToString		m_setup_keywords;
	 // Program executable path.
	 CString				m_strProgramExe;
	 // Owner window.
	 HWND					m_hOwner;

public:
	 // progress control.
	 CProgressCtrl		   *m_progress;
	 // action status control.
	 CStatic			   *m_action_status;
	 // install application name.
	 CString				m_strAppName;
	 // application create company.
	 CString				m_strCompany;
	 // install application version.
	 CString				m_strAppVersion;
	 // install path.
	 CString				m_strInstallPath;

	 // Smart Setup Script Functions. ====================
	 void InitSSScript				(CString strSetupIni, HWND hOwner = NULL);
	 BOOL ReadSSScript				();
	 BOOL RunSSScript				();
	 void UninitializeSSScript		();
	 void ReplaceKeywords			(CString& str);
	 CIni*	GetSetupIni(){ return  &m_setup_ini;}
	 // ==================================================

	 // Add keywords. [Use that function before initialize]
	 void AddKeyWord				(const CString strKeyword, CString& strKeywordValue);
	 void RegisterSetupPackage		();

protected:	
	 // Smart Setup Script Action Functions. =============
	 static LRESULT Extract				(WPARAM wParam, LPARAM lParam);
	 static LRESULT CreateShortcut		(WPARAM wParam, LPARAM lParam);
	 static LRESULT RegOcx				(WPARAM wParam, LPARAM lParam);
	 static LRESULT FbServer			(WPARAM wParam, LPARAM lParam);
	 static LRESULT RunDuringInstall	(WPARAM wParam, LPARAM lParam);
	 static LRESULT CopyFile			(WPARAM wParam, LPARAM lParam);
	 static LRESULT Reg_FBird_Database	(WPARAM wParam, LPARAM lParam);
	 static LRESULT CopyFolder			(WPARAM wParam, LPARAM lParam);
	 static LRESULT DeleteFile			(WPARAM wParam, LPARAM lParam);
	 static LRESULT DeleteFolder		(WPARAM wParam, LPARAM lParam);
	 static LRESULT CreateFolder		(WPARAM wParam, LPARAM lParam);
	 static LRESULT ReplaceIniKeywords	(WPARAM wParam, LPARAM lParam);
	 static LRESULT Sleep				(WPARAM wParam, LPARAM lParam);
	 static LRESULT ChangeFbServerPwd	(WPARAM wParam, LPARAM lParam);
	 static LRESULT InstallTTFFont		(WPARAM wParam, LPARAM lParam);
	 static LRESULT MoveFile			(WPARAM wParam, LPARAM lParam);
	 static LRESULT RegDeleteKey		(WPARAM wParam, LPARAM lParam);
	 static LRESULT WriteInFile			(WPARAM wParam, LPARAM lParam);
	 static LRESULT RegLoadXml			(WPARAM wParam, LPARAM lParam);
	 static LRESULT ReplaceImageRes		(WPARAM wParam, LPARAM lParam);
	 // ==================================================
	};