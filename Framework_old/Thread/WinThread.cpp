#include "WinThread.h"

WinThread::WinThread(){
	m_hThread		= NULL;
	m_threadProc	= NULL;
	m_dwThreadId	= 0;
	m_pParameter	= NULL;
	_lockCt			= 0;
	}

WinThread::~WinThread(){
	StopThread(5000);
	}

bool
WinThread::StartThread(LPTHREAD_START_ROUTINE lpThreadProc, void* lpParameter,
						int nPriority /*= THREAD_PRIORITY_NORMAL*/, 
						int nStackSize /*= DEF_THREAD_STOCK_SIZE*/, bool bSuspended /*= false*/)
	{
	m_threadProc		= lpThreadProc;
	m_pParameter		= lpParameter;
	
	m_eventStop.ResetEvent();
	m_eventStop.ResetEvent();
	m_eventStop.ResetEvent();
	m_eventStop.ResetEvent();
        
#ifdef _WINDOWS
	m_hThread           = ::CreateThread(0L, (SIZE_T)nStackSize, (LPTHREAD_START_ROUTINE)WinThread::_ThreadProc,
                                         this, bSuspended ? CREATE_SUSPENDED : 0L, &m_dwThreadId);
	if( !m_hThread ){
		//TRACE(_T("WinThread couldn't create new thread !!!"));
		m_pParameter	= NULL;
		return false;
		}

	if( nPriority != THREAD_PRIORITY_NORMAL ){
		if( !::SetThreadPriority(m_hThread, nPriority) ){
			//TRACE(_T("WinThread couldn't set thread priority !!!"));
			}
		}
#else
        
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setstacksize(&attr, (size_t)nStackSize);
        
        int nErrorCode = (bSuspended ? pthread_create_suspended_np(&m_hThread, &attr, m_threadProc, this) : pthread_create(&m_hThread, &attr, m_threadProc, this));
        if( nErrorCode != 0 )
        {
            m_hThread = NULL;
            pthread_attr_destroy(&attr);
            m_pParameter	= NULL;
            return false;
        }
        
        pthread_attr_destroy(&attr);
        
#endif
	return true;
	}

bool
WinThread::ResumeThread(){
#ifdef _WINDOWS
	// Resume suspended thread.
	return (::ResumeThread(m_hThread) != -1);
#else
    return false;
#endif
	}

bool
WinThread::StopThread(DWORD dwWait /*= INFINITE*/){
	if( !m_hThread )
		return false;

	// Operation not allowed from same thread.
	if( IsCurrentThread() )
		return false;

	if( !IsThreadStopped() ){
		SetStopEvent	(true);
		SetPauseEvent	(false);
		SetContinueEvent(true);
        
#ifdef _WINDOWS
		// Wait while thread exits.
		if( ::WaitForSingleObject(m_hThread, 20000) == WAIT_TIMEOUT ){
			TerminateThread(m_hThread, 0);
			}
		/*
		// Wait while thread exits.
		if( ::WaitForSingleObject(m_hThread, dwWait) == WAIT_TIMEOUT ){
			TerminateThread(m_hThread, 0);
			}*/
#else
        //pthread_join(m_hThread, );
#endif
		}

	m_threadProc	= NULL;
#ifdef _WINDOWS
	CloseHandle(m_hThread);
#endif
	m_hThread		= NULL;
	m_dwThreadId	= 0L;
	m_pParameter	= NULL;
	RemoveAllParams();
	return true;
	}

bool
WinThread::PauseThread(){
	if( IsThreadStopped() )
		return false;

	if( !IsThreadPaused() ){
		SetContinueEvent(false);
		SetPauseEvent	(true);
		}
	return true;
	}

bool
WinThread::ContinueThread(){
	if( IsThreadStopped() )
		return false;

	if( IsThreadPaused() ){
		SetPauseEvent	(false);
		SetContinueEvent(true);
		}
	return true;
	}

bool
WinThread::IsThreadPaused(){
	Lock();
	bool bRet = (m_eventPause.WaitForEvent(0) == WAIT_OBJECT_0);
	Unlock();
	return bRet;
	}

bool
WinThread::IsThreadStopped(){
	Lock();
#ifdef _WINDOWS
	bool bRet = (::WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0);
#else
    bool bRet = false;
#endif
	Unlock();
	return bRet;
	}

bool
WinThread::IsStopEventSignaled(){
	Lock();
	bool bRet = (m_eventStop.WaitForEvent(0) == WAIT_OBJECT_0); 
	Unlock();
	return bRet;
	}

bool
WinThread::IsPauseEventSignaled(){
	Lock();
	bool bRet = (m_eventPause.WaitForEvent(0) == WAIT_OBJECT_0); 
	Unlock();
	return bRet;
	}

bool
WinThread::IsContinueEventSignaled(){
	Lock();
	bool bRet = (m_eventContinue.WaitForEvent(0) == WAIT_OBJECT_0); 
	Unlock();
	return bRet;
	}

bool
WinThread::IsSyncEventSignaled(){
	Lock();
	bool bRet = (m_eventSync.WaitForEvent(0) == WAIT_OBJECT_0); 
	Unlock();
	return bRet;
	}

bool
WinThread::WaitForContinueEvent(DWORD dwWaitMS /*= INFINITE*/){
	bool bRet = (m_eventContinue.WaitForEvent(dwWaitMS) == WAIT_OBJECT_0); 
	return bRet;
	}

bool
WinThread::WaitForStopEvent(DWORD dwWaitMS /*= INFINITE*/){
	bool bRet = (m_eventStop.WaitForEvent(dwWaitMS) == WAIT_OBJECT_0); 
	return bRet;
	}

bool
WinThread::WaitForPauseEvent(DWORD dwWaitMS /*= INFINITE*/){
	bool bRet = (m_eventPause.WaitForEvent(dwWaitMS) == WAIT_OBJECT_0); 
	return bRet;
	}

bool
WinThread::SetStopEvent(bool bState){
	Lock();
	if( bState )
		m_eventStop.SetEvent();
	else
		m_eventStop.ResetEvent();
	Unlock();
	return true;
	}

bool
WinThread::SetPauseEvent(bool bState){
	Lock();
	if( bState )
		m_eventPause.SetEvent();
	else
		m_eventPause.ResetEvent();
	Unlock();
	return true;
	}

bool
WinThread::SetContinueEvent(bool bState){
	Lock();
	if( bState )
		m_eventContinue.SetEvent();
	else
		m_eventContinue.ResetEvent();
	Unlock();
	return true;
	}

bool
WinThread::SetSyncEvent(bool bState){
	Lock();
	if( bState )
		m_eventSync.SetEvent();
	else
		m_eventSync.ResetEvent();
	Unlock();
	return true;
	}

bool
WinThread::IsCurrentThread(){
#ifdef _WINDOWS
	return (GetCurrentThreadId() == m_dwThreadId);
#else
    return (pthread_equal(m_hThread, pthread_self()) != 0);
#endif
	}

bool
WinThread::AddParam(const _string sName, void* lpValue){
	Lock();
	bool bRet = (m_namedParams.Add(sName, lpValue) == TRUE);
	Unlock();
	return bRet;
	}

bool
WinThread::SetParam(const _string sName, void* lpValue){
	Lock();
	if( m_namedParams.KeyExists(sName) )
		m_namedParams.SetValue(sName, lpValue);
	else
		m_namedParams.Add(sName, lpValue);
	Unlock();
	return true;
	}

void*
WinThread::GetParam(const _string sName){
	Lock();
	void* lpValue = m_namedParams.GetValue(sName);
	Unlock();
	return lpValue;
	}

bool
WinThread::RemoveParam(const _string sName){
	Lock();
	bool bRet = (m_namedParams.Delete(sName) == TRUE);
	Unlock();
	return bRet;
	}

int	
WinThread::RemoveAllParams(){
	Lock();
	int nRet = m_namedParams.GetCount();
	m_namedParams.DeleteAll();
	Unlock();
	return nRet;
	}

#ifdef _WINDOWS
UINT _stdcall
WinThread::_ThreadProc(WinThread* pThread){
	UINT nRet = (*pThread->m_threadProc)(pThread);
	// Thread finished.
	if( pThread )
		pThread->OnThreadFinished();
	return nRet;
	}
#else
void*
WinThread::_ThreadProc(WinThread* pThread){
    void* lpRet = (*pThread->m_threadProc)(pThread);
	// Thread finished.
	if( pThread )
		pThread->OnThreadFinished();
	return lpRet;
    }
#endif

void
WinThread::OnThreadFinished(){
	if( IsCurrentThread() ){ // Safe destroy thread params.
		RemoveAllParams();

		m_threadProc	= NULL;
#ifdef _WINDOWS
		CloseHandle(m_hThread);
#else
        
#endif
		m_hThread		= NULL;
		m_dwThreadId	= 0L;
		m_pParameter	= NULL;
		}
	}
