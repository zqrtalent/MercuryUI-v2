//////////////////////////////////////////////////////
//                                                  //
// started [23.06.2009]                             // 
//                                                  //
// Windows API Hooker class		                    //
//                                                  //
// Author:  ZqrTalent                               //
//                                                  //
//////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\String\StringDecoder.h"
#include "APIHook.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

APIHook::APIHook()
	{
	ASSERT(FALSE);
	}

APIHook::APIHook(BOOL bStatic)
	{
	}

APIHook::~APIHook()
	{
	Destroy();
	}

APIHook* 
APIHook::GetAPIHook()
	{
	static APIHook api(TRUE);
	return &api;
	}

BOOL	
APIHook::SetHook(LPCTSTR pszModule, LPCTSTR pszFunction, PROC funcHook, BOOL bIgnoreExistance /*= FALSE*/)
	{
	CString sDecode = pszModule;
	if( !sDecode.IsEmpty() )
		sDecode		+= ":";
	sDecode			+= pszFunction;
	DWORD dwCode	= DecodeString(sDecode.GetBuffer());
	ASSERT( dwCode );

	// Check Hooks existance. ##########################
	if( !bIgnoreExistance )
		{
		if( m_arrModules.FindValue((void*)dwCode) != -1 )
			return FALSE;	
		}
	else
		{
		int nFind = m_arrModules.FindValue((void*)dwCode);
		if( nFind != -1 )
			{
			HookInfo* pInfo = (HookInfo*)m_arrModules.GetData(nFind);
			UnHook(pInfo);
			}
		}
	// #################################################
	
	// Retrieve destination function. ##################
	PROC	procDest = NULL;
	if( !CString(pszModule).IsEmpty() ) 
		{
		HMODULE hModule		= GetModuleHandle(pszModule);
		if( !hModule )
			return FALSE;
		procDest	= GetProcAddress(hModule, pszFunction);
		if( !procDest )
			return FALSE;
		}
	else // pszFunction is address of function to hook.
		{
		procDest = (PROC)atoi(pszFunction);
		}
	// #################################################

	// Hook function. ##################################
	JumpOperation jmpReal, jmpHook;
	if( !SetHook(procDest, funcHook, &jmpReal, &jmpHook) )
		return FALSE;
	// #################################################
	
	// Save hook after succesfull hook. ################
	HookInfo* pInfo		= new HookInfo;
	pInfo[0].jmpReal	= jmpReal;
	pInfo[0].jmpHook	= jmpHook;
	pInfo[0].procDest	= procDest;
	pInfo[0].procHook	= funcHook;
	m_arrModules.Add((void*)dwCode, (void*)pInfo);
	// #################################################
	return TRUE;
	}

BOOL	
APIHook::UnHook(HookInfo* pHook)
	{
	ASSERT( pHook );
	memcpy((void*)pHook[0].procDest, (void*)&pHook[0].jmpReal, sizeof(JumpOperation));
	return TRUE;
	}

BOOL	
APIHook::UnHook(LPCTSTR pszModule, LPCTSTR pszFunction)
	{
	CString sDecode = pszModule;
	if( !sDecode.IsEmpty() )
		sDecode		+= ":";
	sDecode			+= pszFunction;
	DWORD dwCode	= DecodeString(sDecode.GetBuffer());
	ASSERT( dwCode );

	int nFind = m_arrModules.FindValue((void*)dwCode);
	if( nFind == -1 )
		return FALSE;

	HookInfo* pInfo = (HookInfo*)m_arrModules.GetData(nFind);
	if( UnHook(pInfo) )
		{
		delete pInfo;
		m_arrModules.Delete(nFind);
		return TRUE;
		}
	return FALSE;
	}

BOOL
APIHook::SetHook(PROC funcDest, PROC funcHook, JumpOperation* jmpReal, JumpOperation* jmpOp)
	{
	if( !funcDest || !funcHook || !jmpOp )
		return FALSE;

	// Change protection option for memory block.
	MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(funcDest, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
    VERIFY(VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect));

	JumpOperation* pJmpDest = (JumpOperation*)funcDest;
	JumpOperation	jmpHk;
	jmpHk.bJmpCode			= 0xE9;
	jmpHk.lJmpOffset		= long(funcHook) - long(5 + long(funcDest));

	if( jmpHk.bJmpCode		== pJmpDest->bJmpCode && 
		jmpHk.lJmpOffset	== pJmpDest->lJmpOffset )
		{
		return TRUE; // Already hooked.
		}

	// save real bytes. #####################
	jmpReal->bJmpCode		= pJmpDest->bJmpCode;
	jmpReal->lJmpOffset		= pJmpDest->lJmpOffset;
	//---------------------------------------
		
	// write hook function jmp instruction.//
	pJmpDest->bJmpCode		= jmpHk.bJmpCode;
	pJmpDest->lJmpOffset	= jmpHk.lJmpOffset;
	//---------------------------------------

	// save hook function jump instruction.//
	jmpOp->bJmpCode			= jmpHk.bJmpCode;
	jmpOp->lJmpOffset		= jmpHk.lJmpOffset;
	//---------------------------------------
    return TRUE;
	}

BOOL
APIHook::SetHook(PROC func, PROC funcHook, BOOL bIgnoreExistance)
	{
	CString sFunc;
	sFunc.Format("%d", (int)func);
	return SetHook("", sFunc.GetBuffer(), funcHook, bIgnoreExistance);
	}

BOOL
APIHook::UnHook(PROC func, PROC* procDest)
	{
	CString sFunc;
	sFunc.Format("%d", (int)func);

	CString sDecode;
	sDecode			+= sFunc;
	DWORD dwCode	= DecodeString(sDecode.GetBuffer());
	ASSERT( dwCode );

	int nFind = m_arrModules.FindValue((void*)dwCode);
	if( nFind == -1 )
		return FALSE;

	HookInfo* pInfo = (HookInfo*)m_arrModules.GetData(nFind);
	if( UnHook(pInfo) )
		{
		if( procDest )
			*procDest = pInfo->procDest;
		delete pInfo;
		m_arrModules.Delete(nFind);
		return TRUE;
		}
	return FALSE;
	}

BOOL
APIHook::UnHookTemporary(LPCTSTR pszModule, LPCTSTR pszFunction, PROC& procDest)
	{
	CString sDecode = pszModule;
	sDecode			+= ":";
	sDecode			+= pszFunction;
	DWORD dwCode	= DecodeString(sDecode.GetBuffer());
	ASSERT( dwCode );

	int nFind = m_arrModules.FindValue((void*)dwCode);
	if( nFind == -1 )
		return FALSE;

	HookInfo* pInfo = (HookInfo*)m_arrModules.GetData(nFind);
	ASSERT( pInfo );
	UnHook(pInfo);
	procDest = pInfo->procDest;
	return TRUE;
	}

BOOL
APIHook::HookTemporary(LPCTSTR pszModule, LPCTSTR pszFunction)
	{
	CString sDecode = pszModule;
	sDecode			+= ":";
	sDecode			+= pszFunction;
	DWORD dwCode	= DecodeString(sDecode.GetBuffer());
	ASSERT( dwCode );

	int nFind = m_arrModules.FindValue((void*)dwCode);
	if( nFind == -1 )
		return FALSE;

	HookInfo* pInfo = (HookInfo*)m_arrModules.GetData(nFind);
	return SetHook(pInfo->procDest, pInfo->procHook, &pInfo->jmpReal, &pInfo->jmpHook);
	}

void
APIHook::Destroy()
	{
	for( int nLoop=0; nLoop<m_arrModules.GetCount(); nLoop ++ )
		{
		HookInfo* pInfo = (HookInfo*)m_arrModules.GetData(nLoop);
		UnHook(pInfo);
		}
	m_arrModules.DeleteAll();
	}