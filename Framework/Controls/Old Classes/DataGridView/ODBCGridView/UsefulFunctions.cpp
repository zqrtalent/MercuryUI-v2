#include "stdafx.h"
#include "UsefulFunctions.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Usefull functions. [START]
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
//	Free pointers chain. |-> |-> |-> |
//

int __stdcall
FreePointersChain(void* pFirstElem, const long nNextPointerOffset)
	{
	int nFreeCt = 0;
	while( pFirstElem ){
		void* pNextPointer	= (void*)((INT32*)((INT32)(pFirstElem) + nNextPointerOffset))[0];
		delete pFirstElem;
		pFirstElem			= pNextPointer;
		nFreeCt				++;
		}

	return nFreeCt;
	}

//
//	Inserts pointer to pointer chain.
//	CAUTION: pInsert must be the same type as pFirstElem.
//	returns first pointer of chain if success otherwise NULL.
//

void* __stdcall
InsertToPointersChain(void* pFirstElem, void* pInsert, int nIndex, 
					  const long nNextPointerOffset, const long nPrevPointerOffset)
	{
	ASSERT( pInsert && nIndex > -1 );

	int		nLoop			= 0;
	void*	pNextPointer	= pFirstElem;
	void*	pPrevPointer	= NULL;

	// Move to index. ###################
	while( pNextPointer )
		{
		if( nLoop == nIndex )
			break;
		pPrevPointer	= pNextPointer;
		pNextPointer	= (void*)((INT32*)((INT32)(pNextPointer) + nNextPointerOffset))[0];
		nLoop			++;
		}
	// ##################################

	// Insert pointer. ##################
	if( pNextPointer )
		{
		if( pPrevPointer )
			{
			*((INT32*)((INT32)(pPrevPointer)		+ nNextPointerOffset)) = (INT32)pInsert;
			*((INT32*)((INT32)(pInsert)				+ nNextPointerOffset)) = (INT32)pNextPointer;
			if( nPrevPointerOffset != -1 )
				{
				*((INT32*)((INT32)(pInsert)			+ nPrevPointerOffset)) = (INT32)pPrevPointer;
				*((INT32*)((INT32)(pNextPointer)	+ nPrevPointerOffset)) = (INT32)pInsert;
				}
			return pFirstElem;
			}
		else								// add pointer on the top.
			{
			*((INT32*)((INT32)(pInsert)				+ nNextPointerOffset)) = (INT32)pFirstElem;
			if( nPrevPointerOffset != -1 )
				{
				*((INT32*)((INT32)(pInsert)			+ nPrevPointerOffset)) = NULL;
				*((INT32*)((INT32)(pFirstElem)		+ nPrevPointerOffset)) = (INT32)pInsert;
				}
			return pInsert;
			}
		}
	else
	if( nLoop == nIndex && pPrevPointer ) // add pointer on the bottom.
		{
		*((INT32*)((INT32)(pPrevPointer)			+ nNextPointerOffset)) = (INT32)pInsert;
		*((INT32*)((INT32)(pInsert)				+ nNextPointerOffset)) = NULL;
		if( nPrevPointerOffset != -1 )
			*((INT32*)((INT32)(pInsert)				+ nPrevPointerOffset)) = (INT32)pPrevPointer;			
		return pFirstElem;
		}
	// #################################


	return (!pFirstElem && nIndex == 0) ? pInsert : NULL;
	}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Usefull functions. [END]
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++