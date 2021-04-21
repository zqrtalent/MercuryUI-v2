#include "stdafx.h"
#include "TaskRunner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SocketIOCP. Socket IO completion port.
//
/////////////////////////////////////////////////////////////////////

TaskRunner::TaskRunner(){
	m_phThread		= NULL;
	m_pdwThreadIds	= NULL;
	m_hStopEvent	= NULL;
	m_nThreadCt		= 0;
	m_hIOCP			= NULL;
	m_pKey			= NULL;

	m_listTask.CreateVariableIndex(_T("Id"), Serializable::Compare_Int32);
	m_listTask.CreateVariableIndex(_T("Name"), Serializable::Compare_String);
	}

TaskRunner::~TaskRunner(){
	Stop();
	}

bool
TaskRunner::Start(int nWorkerThreadCt /*= 1*/){
	if( m_nThreadCt > 0 )
		return false; // Already started.
	// Invalid parameter.
	if( nWorkerThreadCt < 1 || nWorkerThreadCt > 10 )
		return false; 
	// Setup IO Completion port.
	if( m_hIOCP != NULL )
		return false;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if( sock == INVALID_SOCKET )
		return false;

	m_hIOCP = CreateIoCompletionPort((HANDLE)sock, 0, NULL, 0);
	if( m_hIOCP == NULL ){
		closesocket(sock);
		return false;
		}

	closesocket(sock);
	SOCKET sockNew = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if( sockNew == INVALID_SOCKET )
		return false;

	m_pKey			= new DefaultKey;
	m_pKey->_sock	= sockNew;
	if( !AssociateSocketHandle(sockNew, (ULONG_PTR)m_pKey) )
		return false;
	
	m_phThread		= new HANDLE[nWorkerThreadCt];
	m_pdwThreadIds	= new DWORD[nWorkerThreadCt];
	m_hStopEvent	= CreateEvent(NULL, TRUE, FALSE, NULL);
	memset(m_phThread, 0, sizeof(HANDLE)*nWorkerThreadCt);
	memset(m_pdwThreadIds, 0, sizeof(DWORD)*nWorkerThreadCt);

	for(UINT i=0; i<nWorkerThreadCt; i++){
		UINT	dwThreadId	= 0;
		/*HANDLE	hThread		= (HANDLE)_beginthreadex(NULL,				// Security
									0,								// default stack size
									&SocketIOMan::_MainThreadProc,	// thread proc
									(void*) this,
									0,								// init flag
									&dwThreadId);
									*/
		HANDLE	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TaskRunner::_MainThreadProc, this, 0L, (DWORD*)&dwThreadId);
		if( hThread != INVALID_HANDLE_VALUE ){
			m_phThread[m_nThreadCt]		= hThread;
			m_pdwThreadIds[m_nThreadCt] = dwThreadId;
			m_nThreadCt ++;
			}
		else
			continue;
		}

	if( !m_nThreadCt ){
		delete [] m_phThread;
		delete [] m_pdwThreadIds;
		m_phThread		= NULL;
		m_pdwThreadIds	= NULL;
		CloseHandle(m_hStopEvent);
		m_hStopEvent	= NULL;
		return false;
		}
	return true;
	}

void
TaskRunner::Stop(){
	if( m_nThreadCt == 0 ){
		m_listTask.RemoveAll(true);
		return; // Not started.
		}
	// Signal stop event.
	SetEvent(m_hStopEvent);
	// Post quit event.
	PostTerminationSignal();
	// Wait until thread finishes.
	::WaitForMultipleObjects(m_nThreadCt, m_phThread, TRUE, INFINITE);
	// Close thread handles.
	for(int i=0; i<m_nThreadCt; i++)
		CloseHandle(m_phThread[i]);
	// Close completion port.
	if( m_hIOCP != NULL ){
		closesocket(m_pKey->_sock);
		CloseHandle(m_hIOCP);
		m_hIOCP = 0L;
		delete m_pKey;
		m_pKey = NULL;
		}
	// Delete arrays.
	delete [] m_phThread;
	m_phThread = NULL;
	delete [] m_pdwThreadIds;
	m_pdwThreadIds = NULL;

	CloseHandle(m_hStopEvent);
	m_hStopEvent	= NULL;
	m_nThreadCt		= 0;

	m_listTask.RemoveAll(true);
	}

int
TaskRunner::RegisterTask(const CString sName, void* pClassObj, TaskHandlerMethod method){
	if( IsStarted() ) return false;
	m_lock.Lock();	// Lock
	if( m_listTask.FindOneVariable(_T("Name"), (void*)&sName) != NULL ){
		m_lock.Unlock(); // Unlock
		return false; // Task with that name already exists !!!
		}
	TaskInfo* pNewTask		= new TaskInfo();
	pNewTask->m_sName		= sName;
	pNewTask->m_nId			= m_listTask.GetCount() + 1;
	pNewTask->m_method		= method;
	pNewTask->m_pClassObj	= pClassObj;
	m_listTask.Add(pNewTask);
	m_lock.Unlock(); // Unlock
	return pNewTask->m_nId;
	}

bool
TaskRunner::PostTask(const CString sTaskName, TaskParamObj* pParam){
	if( !m_hIOCP ) return false;
	pParam->m_nTaskId	= 0;
	pParam->m_sTaskName = sTaskName;
	if( (PostQueuedCompletionStatus(m_hIOCP, 0, (ULONG_PTR)pParam, NULL) == 1) )
		return true;
	return false;
	}

bool
TaskRunner::PostTask(int nTaskId, TaskParamObj* pParam){
	if( !m_hIOCP ) return false;
	pParam->m_nTaskId	= nTaskId;
	pParam->m_sTaskName.Empty();
	if( (PostQueuedCompletionStatus(m_hIOCP, 0, (ULONG_PTR)pParam, NULL) == 1) )
		return true;
	return false;
	}

bool
TaskRunner::AssociateSocketHandle(SOCKET sock, ULONG_PTR keyObject){
	if( !m_hIOCP ) return false;
	if( (CreateIoCompletionPort((HANDLE)sock, m_hIOCP, keyObject, 0) == m_hIOCP) )
		return true;
	return false;
	}

bool
TaskRunner::GetQueuedTask(TaskParamObj** pTaskParams){
	if( !m_hIOCP ) return false;
	OVERLAPPED* ov;
	DWORD		dwBytesTransferred = 0;
	return (::GetQueuedCompletionStatus(m_hIOCP, &dwBytesTransferred, (ULONG_PTR*)pTaskParams, &ov, INFINITE) == 1);
	}

void
TaskRunner::PostTerminationSignal(){
	if( m_hIOCP == NULL ) return;
	PostQueuedCompletionStatus(m_hIOCP, 0, NULL, NULL);
	}

//=============================================================================================
//
//	TaskRunner IO Completition thread routine.
//

unsigned 
TaskRunner::_MainThreadProc(void* pIOMan){
	((TaskRunner*)pIOMan)->DoMainThreadJob();
	return 0;
	}

void
TaskRunner::DoMainThreadJob(){
	TaskParamObj*				pTaskParam		= NULL;
	int							nLastError		= 0;
	EnumerableObject<TaskInfo>	listTask;
	listTask.CreateVariableIndex(_T("Id"), Serializable::Compare_Int32);
	listTask.CreateVariableIndex(_T("Name"), Serializable::Compare_String);
	
	m_lock.Lock();		// Lock
	m_listTask.Copy_Into(&listTask);
	m_lock.Unlock();	// Unlock

	while( WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0 ){
		BOOL bResult = GetQueuedTask(&pTaskParam);
		if( !bResult )
			continue;

		// PostQueuedCompletionStatus was called.
		if( pTaskParam == NULL ){
			// Post quit signal to the next thread.
			PostTerminationSignal();
			break;
			}

		// Search for task.
		TaskInfo* pTask = NULL;
		if( pTaskParam->m_nTaskId )
			pTask = listTask.FindOneVariable(_T("Id"), &pTaskParam->m_nTaskId);
		else
			pTask = listTask.FindOneVariable(_T("Name"), &pTaskParam->m_sTaskName);

		// Execute task.
		if( pTask ){
			ThisCallHelper* pThisCall = (ThisCallHelper*)(pTask->m_pClassObj);
			pThisCall->Call_TaskHandlerMethod(pTask->m_method, pTaskParam);
			}

		delete pTaskParam;
		}

	listTask.RemoveAll(true);
	}