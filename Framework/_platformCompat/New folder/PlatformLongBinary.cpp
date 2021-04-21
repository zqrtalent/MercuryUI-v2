#include "stdafx.h"
#include "_platformCompat/PlatformLongBinary.h"
/*
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LongBinary::LongBinary(){
	m_hMemory = NULL;
	m_dwSize = 0L;
}

LongBinary::~LongBinary(){
	SetBinarySize(0);
}

void*
LongBinary::LockMemory(){
	if (m_dwSize == 0 || !m_hMemory)
		return NULL;
	void* lpMem = NULL;
	lpMem = ::GlobalLock(m_hMemory);
	return lpMem;
}

void
LongBinary::UnlockMemory(){
	if (m_dwSize == 0 || !m_hMemory)
		return;
	::GlobalUnlock(m_hMemory);
}

BOOL
LongBinary::SetBinarySize(DWORD sizeNew, void* lpDataCopy){
	DWORD sizeOld = (DWORD)::GlobalSize(m_hMemory);
	if (sizeOld == 0){
		m_hMemory = ::GlobalAlloc(GMEM_MOVEABLE, sizeNew);
		m_dwSize = sizeNew;
	}
	else{
		if (sizeNew > sizeOld){
			m_hMemory = ::GlobalReAlloc(m_hMemory, sizeNew, GMEM_MOVEABLE);
			m_dwSize = sizeNew;
		}
		else{
			if (sizeNew == 0){
				if (m_hMemory)
					GlobalFree(m_hMemory);
				m_hMemory = 0L;
			}
		}
	}

	if (lpDataCopy != NULL && m_hMemory){
		void* lpDest = ::GlobalLock(m_hMemory);
		memcpy(lpDest, lpDataCopy, sizeNew);
		::GlobalUnlock(m_hMemory);
	}
	return TRUE;
}*/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LongBinary::LongBinary(){
	m_hMemory = NULL;
	m_dwSize = 0L;
	m_pBuffer = NULL;
	}

LongBinary::~LongBinary(){
	SetBinarySize(0);
	}

void*
LongBinary::LockMemory(){
	if( m_dwSize == 0 || !m_hMemory )
		return NULL;
	return m_pBuffer;
	}

void
LongBinary::UnlockMemory(){
	if( m_dwSize == 0  || !m_hMemory )
		return;
	}

HGLOBAL
LongBinary::CreateGlobalMemory(){
	if (!m_dwSize)
		return NULL;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_dwSize);
	void* lpDest = ::GlobalLock(hGlobal);
	memcpy(lpDest, m_pBuffer, m_dwSize);
	::GlobalUnlock(hGlobal);

	return hGlobal;
	}

BOOL
LongBinary::SetBinarySize(DWORD sizeNew, void* lpDataCopy){
	if (m_hMemory == NULL){ // Allocate
		m_pBuffer = NULL;
		if (sizeNew == 0)
			return TRUE;
		m_hMemory = HeapCreate(0L, sizeNew, 0L);
		ASSERT(m_hMemory);
		m_pBuffer = HeapAlloc(m_hMemory, 0L, sizeNew);
		m_dwSize = sizeNew;
	}
	else{ // Reallocate
		if (sizeNew > m_dwSize){
			// Allocate or realocate memory . #####
			m_pBuffer = HeapReAlloc(m_hMemory, 0L, m_pBuffer, sizeNew);
			m_dwSize = sizeNew;
		}
		else
			if (sizeNew == 0){
				HeapFree(m_hMemory, 0L, m_pBuffer);
				HeapDestroy(m_hMemory);
				m_pBuffer = NULL;
				m_hMemory = NULL;
				m_dwSize = 0;
			}
		}

	if( lpDataCopy != NULL && m_hMemory ){
		memcpy(m_pBuffer, lpDataCopy, sizeNew);
		}
	return TRUE;
	}