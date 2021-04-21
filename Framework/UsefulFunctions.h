#pragma once
#include <stddef.h>

int		__stdcall FreePointersChain			(void* pFirstElem, const long nNextPointerOffset);
int		__stdcall CountMembersInChain		(void* pFirstElem, const long nNextPointerOffset);
void*	__stdcall LastMember				(void* pFirstElem, const long nNextPointerOffset);
void	__stdcall SetPointersCount			(void* pFirstElem, const long nNextPointerOffset, int nCount);
void*	__stdcall InsertToPointersChain		(void* pFirstElem, void* pInsert, int nIndex, 
											const long nNextPointerOffset, 
											const long nPrevPointerOffset);
