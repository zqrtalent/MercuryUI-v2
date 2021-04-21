#pragma once
#include <afxmt.h>

typedef bool (*SafePointerDeletingProc)(void* pPointer);

class SafePointer
{
public:
	SafePointer();
	virtual ~SafePointer();

	static	SafePointer*	Retain	(SafePointer* pPtr);
	static	bool			Release	(SafePointer* pPtr);

	void	SetDeletingDelegate(SafePointerDeletingProc proc){m_procDeleting = proc;}

protected:
	void	RetainPointer	();	// Retain this pointer.
	bool	ReleasePointer	(); // Release object pointer and return destroy status.

protected:
	static CCriticalSection* GetLocker(){
		static CCriticalSection lock;
		return &lock;
		}

	static void LockPointer(){
		GetLocker()->Lock();
		}
	
	static void UnlockPointer(){
		GetLocker()->Unlock();
		}

private:
	volatile int			m_nRetainCt;
	SafePointerDeletingProc	m_procDeleting;
};