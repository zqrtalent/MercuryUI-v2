#include "stdafx.h"
#include "SafePointer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SafePointer::SafePointer(){
	m_nRetainCt		= 0;
	m_procDeleting	= NULL;
	}

SafePointer::~SafePointer(){
	}

SafePointer*
SafePointer::Retain(SafePointer* pPtr){
	LockPointer();		// Lock
	pPtr->RetainPointer();
	UnlockPointer();	// Lock
	return pPtr;
	}

bool
SafePointer::Release(SafePointer* pPtr){
	LockPointer();			// Lock
	bool bRet = pPtr->ReleasePointer();
	if( bRet ){
		delete pPtr;
		}
	UnlockPointer();	// Unlock
	return bRet;
	}

void
SafePointer::RetainPointer(){
	m_nRetainCt ++;
	}

bool
SafePointer::ReleasePointer(){
	if( !m_nRetainCt ) return false;
	m_nRetainCt --;
	if( !m_nRetainCt ){
		bool bDelete = true;
		if(	m_procDeleting )
			bDelete = (m_procDeleting)(this);	
		return bDelete;
		}
	return false;
	}