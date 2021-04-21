////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005 by Franc Morales. All Rights Reserved.
//
//  This file (AP_FontInstaller.h) may be redistributed provided that:
//    - It is not modified.
//    - It is not sold for profit.
//    - This notice is left intact.
//
//  This file (AP_FontInstaller.h) is provided 'as is' with no expressed or implied warranty.
//  The author accepts no liability if it causes any damage to your computer.
//  Expect the worst, expect bugs, and if you find any, please let me know.
//
//  Contact: Franc@SequencePublishing.com
//
////////////////////////////////////////////////////////////////////////////////////////////
// AP_FontInstaller.h: interface for the CAP_FontInstaller class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class FontInstaller  
{
public:
	FontInstaller();
	virtual ~FontInstaller();

// Attributes
protected:
	typedef struct
	{
		CString		m_csFontName;
		CString		m_csInstalledFontFullPath;
		UINT		m_uResID;
		CString		m_csResType;
		BOOL		m_bInstalled;
		BOOL		m_bAlreadyInTheSystem;
	} FONT_DETAILS, *PFONT_DETAILS;

	//	CUIntArray		m_cuiFontDescriptions;
	CArray<PFONT_DETAILS, PFONT_DETAILS>		m_cuiFontDescriptions;

	CString			m_csWindowsFontDir;

// Implementation
public:
	virtual FontInstaller& AddFont			( const CString& csFontName, UINT uResID, const CString& csResType );
	
	BOOL				InstallAllFonts			();
	BOOL				UninstallAllFonts		();

	BOOL				InstallFont				( const CString& csFontName );
	BOOL				UninstallFont			( const CString& csFontName );

	BOOL				IsFontInstalled			( const CString& csFontName );
	BOOL				IsFontAlreadyInSystem	( const CString& csFontName );

protected:
	static int CALLBACK	_EnumFontFamExProc		(	ENUMLOGFONTEX * lpelfe,
													NEWTEXTMETRICEX * lpntme,
													int nFontType,
													LPARAM lParam );

	BOOL				WriteFontFile			(	const CString& csInstalledFontFullPath,
													UINT uResID,
													const CString& csResType );

	BOOL				FindFontDescription		( const CString& csFontName, PFONT_DETAILS& pfd );

	void				OperatingSystemSpecific	();
};
