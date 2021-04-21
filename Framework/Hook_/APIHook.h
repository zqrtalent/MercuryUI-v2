//////////////////////////////////////////////////////
//                                                  //
// started [23.06.2009]                             // 
//                                                  //
// Windows API Hooker class		                    //
//                                                  //
// Author:  ZqrTalent                               //
//                                                  //
//////////////////////////////////////////////////////

#pragma once
#include "..\Array\AutoSortedArray.h"

#include <pshpack1.h>
struct JumpOperation
	{
	BYTE bJmpCode;
	long lJmpOffset;
	};
#include <poppack.h>

struct HookInfo
	{
	PROC			procHook;	// Hook proc.
	PROC			procDest;	// Destionation proc.
	JumpOperation	jmpReal;	// Unhook data.
	JumpOperation	jmpHook;	// jmp data.
	};

class APIHook  
{
public:
	APIHook ();
	APIHook (BOOL bStatic);
	~APIHook();
	static APIHook* GetAPIHook();

protected:
	// DecodeString(ModuleName + ":" + FunctionName)
	AutoSortedArray m_arrModules; // Decoded string array.

public:
	BOOL		SetHook			(LPCTSTR pszModule, LPCTSTR pszFunction, PROC funcHook, BOOL bIgnoreExistance = FALSE);
	BOOL		UnHook			(LPCTSTR pszModule, LPCTSTR pszFunction);

	BOOL		SetHook			(PROC func, PROC funcHook, BOOL bIgnoreExistance = FALSE);
	BOOL		UnHook			(PROC func, PROC* procDest);

	void		Destroy			();

	BOOL		SetHook			(PROC funcDest, PROC funcHook, JumpOperation* jmpReal, JumpOperation* jmpOp);
	BOOL		UnHook			(HookInfo*	pHook);

	BOOL		UnHookTemporary	(LPCTSTR pszModule, LPCTSTR pszFunction, PROC& procDest);
	BOOL		HookTemporary	(LPCTSTR pszModule, LPCTSTR pszFunction);

};
