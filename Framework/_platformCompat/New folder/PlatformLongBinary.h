#if !defined(PLATFORMLONGBINARY_H_INCLUDED)
#define      PLATFORMLONGBINARY_H_INCLUDED

#include "PlatformCompat.h"

class LongBinary
{
public:
	LongBinary();
	virtual ~LongBinary();

/*	operator HGLOBAL(){return m_hMemory;}; */

	HGLOBAL CreateGlobalMemory();

	void*	LockMemory	();
	void	UnlockMemory();

	BOOL	SetBinarySize(DWORD sizeNew, void* lpDataCopy = NULL);
	DWORD	GetBinarySize(){ return m_dwSize; };

protected:
	HGLOBAL		m_hMemory;
	DWORD		m_dwSize;
	void*		m_pBuffer;
};

#endif // !defined(PLATFORMLONGBINARY_H_INCLUDED)

