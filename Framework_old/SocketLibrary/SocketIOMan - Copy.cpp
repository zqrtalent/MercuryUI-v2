#include "stdafx.h"
#include "SocketIOMan.h"
#include "SocketIOQueue.h"
#include "SocketContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SocketIOMan.
//
/////////////////////////////////////////////////////////////////////

SocketIOMan::SocketIOMan(){
	m_phThread		= NULL;
	m_pdwThreadIds	= NULL;
	m_hStopEvent	= NULL;
	m_nThreadCt		= 0;
	}

SocketIOMan::~SocketIOMan(){
	Stop();
	}

bool
SocketIOMan::Start(UINT nWorkerThreadCt /*= 1*/){
	/*
	SYSTEM_INFO info = {0};
	GetNativeSystemInfo(&info);
	iocp->threads_count = info.dwNumberOfProcessors * 2;
	iocp->h_threads = (HANDLE*)malloc(sizeof(HANDLE) * iocp->threads_count);
	for(size_t n = 0; n < iocp->threads_count; ++n)
		iocp->h_threads[n] = (HANDLE)_beginthreadex(0, 0, iocp_proc, iocp, 0, 0);
		*/
	if( m_nThreadCt > 0 )
		return false; // Already started.
	if( nWorkerThreadCt < 1 || nWorkerThreadCt > 100 )
		return false; // Invalid parameter.
	/*
	SYSTEM_INFO info = {0};
	GetNativeSystemInfo(&info);
	nWorkerThreadCt = info.dwNumberOfProcessors * 2;
	*/
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
		HANDLE	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&SocketIOMan::_MainThreadProc, this, 0L, (DWORD*)&dwThreadId);
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
SocketIOMan::Stop(){
	if( m_nThreadCt == 0 )
		return; // Not started.
	// Signal stop event.
	SetEvent(m_hStopEvent);
	// Post quit event.
	SocketIOQueue::GetInstance()->PostTerminationSignal();
	// Wait until thread finishes.
	::WaitForMultipleObjects(m_nThreadCt, m_phThread, TRUE, INFINITE);
	// Close thread handles.
	for(int i=0; i<m_nThreadCt; i++)
		CloseHandle(m_phThread[i]);

	// Delete arrays.
	delete [] m_phThread;
	m_phThread = NULL;
	delete [] m_pdwThreadIds;
	m_pdwThreadIds = NULL;

	CloseHandle(m_hStopEvent);
	m_hStopEvent = NULL;
	m_nThreadCt = 0;
	}

unsigned 
SocketIOMan::_MainThreadProc(void* pIOMan){
	((SocketIOMan*)pIOMan)->DoMainThreadJob();
	return 0;
	}

void
SocketIOMan::DoMainThreadJob(){
	SocketContext*		pSocket			= NULL;
	DWORD				dwBytesLen		= 0;
	LPOVERLAPPED		lpOverlapped	= NULL;	
	OVERLAPPEDEX*		pOverlappedEx	= NULL;	

	bool				bDropConnection	= false;
	SocketIOQueue*		pIOQueue		= SocketIOQueue::GetInstance();
	int					nLastError		= 0;
	
	while( WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0 ){
		BOOL bResult = pIOQueue->GetQueuedEvent(&dwBytesLen, (ULONG_PTR*)&pSocket, &lpOverlapped);
		if( !bResult ){
			if( pSocket ){
				nLastError = WSAGetLastError();
				//Last error values:
				//ERROR_SEM_TIMEOUT - Keep-alive timeouted.
				//ERROR_NETNAME_DELETED - When application is crushed or forcibly closed.
				pSocket->OnDisconnect(nLastError);
				}
			continue;
			}

		// PostQueuedCompletionStatus was called.
		if( pSocket == NULL ){
			/*
			if( !bResult ){
				DWORD dwIOError = GetLastError();

				LPVOID lpMsgBuf;

				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					dwIOError,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &lpMsgBuf,
					0, NULL );


				CString s; //ERROR_INVALID_PARAMETER
				s.Format("%s result=%d pSocket=%d error=%d", lpMsgBuf, (int)bResult, (int)pSocket, GetLastError());
				AfxMessageBox(s);
				}*/
			break;
			}
		
		pOverlappedEx = CONTAINING_RECORD(lpOverlapped, OVERLAPPEDEX, m_ol);
		switch(pOverlappedEx->m_ioType){
			case IORead:{
				if( !dwBytesLen ){
					nLastError = WSAGetLastError();
					pSocket->OnDisconnect(nLastError);
					break;
					}
				pSocket->OnDataReceived(dwBytesLen);
				break;
				}
			case IOWrite:{
				pSocket->OnDataSent(dwBytesLen);
				break;
				}
			case IOInitialize:{
				pSocket->OnInitialize();
				delete pOverlappedEx;
				break;
				}
			case IODisconnect:{
				pSocket->OnDisconnect(0);
				delete pOverlappedEx;
				break;
				}
			};
		}
	}