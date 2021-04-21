////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005 by Franc Morales. All Rights Reserved.
//
//  This file (AP_FontInstaller.cpp) may be redistributed provided that:
//    - It is not modified.
//    - It is not sold for profit.
//    - This notice is left intact.
//
//  This file (AP_FontInstaller.cpp) is provided 'as is' with no expressed or implied warranty.
//  The author accepts no liability if it causes any damage to your computer.
//  Expect the worst, expect bugs, and if you find any, please let me know.
//
//  Contact: Franc@SequencePublishing.com
//
////////////////////////////////////////////////////////////////////////////////////////////
// AP_FontInstaller.cpp: implementation of the FontInstaller class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FontInstaller.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FontInstaller::FontInstaller()
{
	m_cuiFontDescriptions.RemoveAll();

	OperatingSystemSpecific();
}

FontInstaller::~FontInstaller()
{
	for( int i = 0; i < m_cuiFontDescriptions.GetSize(); i++ )
		delete m_cuiFontDescriptions[ i ];

	m_cuiFontDescriptions.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// PROTECTED METHODS
/////////////////////////////////////////////////////////////////////////////
void FontInstaller::OperatingSystemSpecific()
{
	GetWindowsDirectory( m_csWindowsFontDir.GetBufferSetLength(MAX_PATH), MAX_PATH );
	m_csWindowsFontDir.ReleaseBuffer();

	if( m_csWindowsFontDir.Right( 1 ) != '\\' )
		m_csWindowsFontDir += '\\';

	OSVERSIONINFO osVersion;

	osVersion.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

	GetVersionEx( &osVersion );

	if( osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT && osVersion.dwMajorVersion <= 4 )
		m_csWindowsFontDir += _T("system\\");
	else
		m_csWindowsFontDir += _T("fonts\\");
}

BOOL FontInstaller::FindFontDescription( const CString& csFontName, PFONT_DETAILS& pfd )
{
	pfd = NULL;

	for( int i = 0; i < m_cuiFontDescriptions.GetSize(); i++ )
		if( m_cuiFontDescriptions[ i ]->m_csFontName == csFontName )
		{
			pfd = m_cuiFontDescriptions[ i ];
			break;
		}

	return ( pfd != NULL );
}

BOOL FontInstaller::WriteFontFile( const CString& csInstalledFontFullPath, UINT uResID, const CString& csResType )
{
	BOOL		bSuccess	= FALSE;
	HINSTANCE	hInst		= AfxGetResourceHandle();
	HRSRC		hResource	= FindResource( hInst, MAKEINTRESOURCE( uResID ), csResType );

	if( hResource )
	{
		HGLOBAL hGlobal = LoadResource( hInst, hResource );

		if( hGlobal )
		{
			TCHAR*	szTemp	= (TCHAR*)LockResource( hGlobal );
			UINT	uSize	= (UINT)SizeofResource( hInst, hResource );

			DeleteObject( (HGDIOBJ)hGlobal );

			CFile cf;

			if( cf.Open( csInstalledFontFullPath, CFile::modeWrite | CFile::modeCreate ) )
			{
				cf.Write( szTemp, uSize );
				cf.Close();

				bSuccess = TRUE;
			}
		}
	}

	return bSuccess;
}

int CALLBACK FontInstaller::_EnumFontFamExProc(	ENUMLOGFONTEX * lpelfe,
													NEWTEXTMETRICEX * /*lpntme*/,
													int /*nFontType*/,
													LPARAM lParam )
{ 
	PFONT_DETAILS pfd = (PFONT_DETAILS)lParam;

	if( pfd->m_csFontName == lpelfe->elfLogFont.lfFaceName )
	{
		pfd->m_bInstalled = TRUE;
		
		return FALSE;	// Finished
	}
	else
		return TRUE;	// Continue
}

/////////////////////////////////////////////////////////////////////////////
// PUBLIC
/////////////////////////////////////////////////////////////////////////////
BOOL FontInstaller::IsFontInstalled( const CString& csFontName )
{
	PFONT_DETAILS pfd = NULL;
	
	if( FindFontDescription( csFontName, pfd ) )
		return ( pfd->m_bInstalled );
	else
		return FALSE;
}

BOOL FontInstaller::IsFontAlreadyInSystem( const CString& csFontName )
{
	HDC				hDC			= GetDC( NULL );
	LOGFONT			lf			= { 0, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, NULL };
	FONT_DETAILS	fd;

	fd.m_csFontName = csFontName;
	fd.m_bInstalled = FALSE;

	EnumFontFamiliesEx(	hDC, &lf, (FONTENUMPROC)_EnumFontFamExProc, (LPARAM)(LPVOID)&fd, 0 );

	return fd.m_bInstalled;
}

FontInstaller& FontInstaller::AddFont( const CString& csFontName, UINT uResID, const CString& csResType )
{
	PFONT_DETAILS pfd = NULL;
	
	if( !FindFontDescription( csFontName, pfd ) )
	{
		pfd = new FONT_DETAILS;

		pfd->m_csFontName				= csFontName;
		pfd->m_csInstalledFontFullPath	= m_csWindowsFontDir + csFontName + _T(".ttf");
		pfd->m_uResID					= uResID;
		pfd->m_csResType				= csResType;
		pfd->m_bAlreadyInTheSystem		= FALSE;//IsFontAlreadyInSystem( csFontName );
		pfd->m_bInstalled				= FALSE;

		m_cuiFontDescriptions.Add( pfd );
	}

	return *this;
}

BOOL FontInstaller::InstallFont( const CString& csFontName )
{
	BOOL			bSuccess	= FALSE;
	PFONT_DETAILS	pfd			= NULL;

	if( FindFontDescription( csFontName, pfd ) &&
		pfd->m_bInstalled == FALSE &&
		pfd->m_bAlreadyInTheSystem == FALSE &&
		WriteFontFile( pfd->m_csInstalledFontFullPath, pfd->m_uResID, pfd->m_csResType ) )
	{
		bSuccess			= ( AddFontResource( pfd->m_csInstalledFontFullPath ) != 0 );
		pfd->m_bInstalled	= bSuccess;
	}

	return bSuccess;
}

BOOL FontInstaller::UninstallFont( const CString& csFontName )
{
	BOOL			bSuccess	= FALSE;
	PFONT_DETAILS	pfd			= NULL;

	if( FindFontDescription( csFontName, pfd ) &&
		pfd->m_bInstalled == TRUE &&
		pfd->m_bAlreadyInTheSystem == FALSE &&
		RemoveFontResource( pfd->m_csInstalledFontFullPath ) )
	{
		_unlink( pfd->m_csInstalledFontFullPath );

		pfd->m_bInstalled	= FALSE;
		bSuccess			= TRUE;
	}

	return bSuccess;
}

BOOL FontInstaller::InstallAllFonts()
{
	BOOL bSuccess = TRUE;

	for( int i = 0; i < m_cuiFontDescriptions.GetSize(); i++ )
		bSuccess &= InstallFont( m_cuiFontDescriptions[ i ]->m_csFontName );

	return bSuccess;
}

BOOL FontInstaller::UninstallAllFonts()
{
	BOOL bSuccess = TRUE;

	for( int i = 0; i < m_cuiFontDescriptions.GetSize(); i++ )
		bSuccess &= UninstallFont( m_cuiFontDescriptions[ i ]->m_csFontName );

	return bSuccess;
}

