#include "stdafx.h"
#include "StringDecoder.h"

UINT 
DecodeString(LPTSTR lpText)
	{
	DWORD dwCode = (DWORD)-1;
	size_t dwSize = strlen(lpText);
	
	_asm
	{
		push  ebx;
		push  ecx;
		push  esi;
		push  edx;
		push  edi;
		mov   eax,dword ptr dwCode;
		lea   edi, dword ptr gDecoderData;
		mov   edx,dword ptr dwSize;
		mov   ecx,lpText;
m1:
		xor   ebx,ebx;
		inc   ecx;
		movzx esi,ax;
		shr   eax,8;
		mov   bl,byte ptr [ecx-1];
		xor   ebx,esi;
		and   ebx,0FFh;
		mov   esi,dword ptr [ebx*4+edi];
		xor   esi,eax;
		mov   eax,esi;
		mov   esi,edx;
		dec   edx;
		test  esi,esi;
		jne   m1;
		
		mov   dwCode,eax;
		pop   edx;
		pop   esi;
		pop   ecx;
		pop   ebx;
		pop   edi;
	};

	return dwCode;
	}