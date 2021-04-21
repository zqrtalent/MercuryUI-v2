
#pragma once
int		__stdcall FreePointersChain		(void* pFirstElem, const long nNextPointerOffset);
void*	__stdcall InsertToPointersChain	(void* pFirstElem, void* pInsert, int nIndex, const long nNextPointerOffset, const long nPrevPointerOffset);
