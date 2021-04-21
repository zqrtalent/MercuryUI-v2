#pragma once
#define SIZE_64KB	65536
#define SIZE_128KB	131072
#define SIZE_256KB	262144
#define SIZE_512KB	524288
#define SIZE_1M		1048576

#define DEF_THREAD_STOCK_SIZE SIZE_128KB // 128KB

#include "../_platformCompat/PlatformCompat.h"
#include "../_platformCompat/PlatformCriticalSection.h"
#include "../_platformCompat/PlatformEvent.h"
#include "../Array/StringToPtrArray.h"

#ifndef _WINDOWS
#include <pthread.h>

typedef void* (*LPTHREAD_START_ROUTINE)(void*);


#define THREAD_PRIORITY_NORMAL 1 // temporary

#endif

class WinThread
{
public:
	WinThread();
	~WinThread();
    

public:
	bool	StartThread				(LPTHREAD_START_ROUTINE lpThreadProc, void* lpParameter,
									int nPriority = THREAD_PRIORITY_NORMAL, 
									int nStackSize = DEF_THREAD_STOCK_SIZE, bool bSuspended = false);
	bool	ResumeThread			();	// When thread created with suspended flag.
	bool	StopThread				(DWORD dwWait = INFINITE);	// Stop thread.
	bool	PauseThread				();	// Use to pause thread.
	bool	ContinueThread			();	// Use to continue paused thread.
	bool	IsThreadPaused			();
	bool	IsThreadStopped			();
	DWORD	GetThreadId				(){return m_dwThreadId;};
	void*	GetThreadParameter		(){return m_pParameter;};
	bool	IsCurrentThread			();
	Event*	GetStopEventPtr			(){return &m_eventStop;};

	// Check event methods.
	bool	IsStopEventSignaled		();
	bool	IsPauseEventSignaled	();
	bool	IsContinueEventSignaled	();
	bool	IsSyncEventSignaled		();

	// Event wait methods.
	bool	WaitForStopEvent		(DWORD dwWaitMS = INFINITE);
	bool	WaitForPauseEvent		(DWORD dwWaitMS = INFINITE);
	bool	WaitForContinueEvent	(DWORD dwWaitMS = INFINITE);

	// Event state methods.
	bool	SetStopEvent			(bool bState);
	bool	SetPauseEvent			(bool bState);
	bool	SetContinueEvent		(bool bState);
	bool	SetSyncEvent			(bool bState);

	// Lock / Unlock
	BOOL	Lock					(){ m_lock.Lock(); _lockCt ++; return TRUE;};
	void	Unlock					(){m_lock.Unlock(); _lockCt --;};

	// Named params.
	bool	AddParam				(const _string sName, void* lpValue);
	bool	SetParam				(const _string sName, void* lpValue);
	void*	GetParam				(const _string sName);
	bool	RemoveParam				(const _string sName);
	int		RemoveAllParams			();

protected:
#ifdef _WINDOWS
	static UINT	_stdcall	_ThreadProc(WinThread* pThread);
#else
    static void*            _ThreadProc(WinThread* pThread);
#endif
    
	void	OnThreadFinished		();

protected:
#ifdef _WINDOWS
	HANDLE					m_hThread;
#else
    pthread_t               m_hThread;
#endif
    
    LPTHREAD_START_ROUTINE	m_threadProc;
    DWORD					m_dwThreadId;
	Event					m_eventStop;
	Event					m_eventPause;
	Event					m_eventContinue;
	Event					m_eventSync;

	CriticalSection			m_lock;

	void*					m_pParameter;
	StringToPtrArray		m_namedParams;

	volatile int			_lockCt;
};