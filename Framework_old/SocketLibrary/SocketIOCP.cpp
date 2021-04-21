#include "SocketIOCP.h"
#include "SocketContext.h"
#include "SocketConnectionListener.h"

/////////////////////////////////////////////////////////////////////
//
//	SocketIOCP. Socket IO completion port.
//
/////////////////////////////////////////////////////////////////////

SocketIOCP::SocketIOCP(){
	m_phThread		= NULL;
	m_pdwThreadIds	= NULL;
	m_hStopEvent	= NULL;
	m_nThreadCt		= 0;
	m_hIOCP			= NULL;
	m_pKey			= NULL;
	}

SocketIOCP::~SocketIOCP(){
	Stop();
	}

bool
SocketIOCP::Start(int nWorkerThreadCt /*= 1*/){
	if( m_nThreadCt > 0 )
		return false; // Already started.

	if( nWorkerThreadCt == -1 ){
		SYSTEM_INFO info = {0};
		GetNativeSystemInfo(&info);
		//nWorkerThreadCt = info.dwNumberOfProcessors * 2;
		nWorkerThreadCt = info.dwNumberOfProcessors;
		}
	if( nWorkerThreadCt < 1 || nWorkerThreadCt > 100 )
		return false; // Invalid parameter.

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
		HANDLE	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&SocketIOCP::_MainThreadProc, this, 0L, (DWORD*)&dwThreadId);
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
SocketIOCP::Stop(){
	if( m_nThreadCt == 0 )
		return; // Not started.
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
	m_hStopEvent = NULL;
	m_nThreadCt = 0;
	}

bool
SocketIOCP::AssociateSocketHandle(SOCKET sock, ULONG_PTR keyObject){
	if( !m_hIOCP ) return false;
	if( (CreateIoCompletionPort((HANDLE)sock, m_hIOCP, keyObject, 0) == m_hIOCP) ){
		return true;
		}
	return false;
	}

bool
SocketIOCP::GetQueuedEvent(LPDWORD lpBytesLen, SocketContextBase** pSockCtxBase, LPOVERLAPPED* lpOverlapped){
	if( !m_hIOCP ) return false;
	return (::GetQueuedCompletionStatus(m_hIOCP, lpBytesLen, (ULONG_PTR*)pSockCtxBase, lpOverlapped, INFINITE) == 1);
	}

bool
SocketIOCP::PostDisconnectEvent(ULONG_PTR keyObject, DWORD dwLastError){
	if( m_hIOCP == NULL ) return false;
	OVERLAPPEDEX* pOver		= new OVERLAPPEDEX(SockIOType::IODisconnect);
	pOver->m_dwLastError	= dwLastError;
	if( (PostQueuedCompletionStatus(m_hIOCP, 0, keyObject, &pOver->m_ol) == 1) )
		return true;
	return false;
	}

bool
SocketIOCP::PostEvent(OVERLAPPEDEX* lpOverlapped, ULONG_PTR keyObject){
	if( m_hIOCP == NULL ) return false;
	return (PostQueuedCompletionStatus(m_hIOCP, 0, keyObject, &lpOverlapped->m_ol) == 1);
	}

void
SocketIOCP::PostTerminationSignal(){
	if( m_hIOCP == NULL ) return;
	PostQueuedCompletionStatus(m_hIOCP, 0, NULL, NULL);
	}

//=============================================================================================
//
//	Socket IO Completition thread routine.
//

unsigned 
SocketIOCP::_MainThreadProc(void* pIOMan){
	((SocketIOCP*)pIOMan)->DoMainThreadJob();
	return 0;
	}

void
SocketIOCP::DoMainThreadJob(){
	SocketContextBase*	pSocket			= NULL;
	DWORD				dwBytesLen		= 0;
	LPOVERLAPPED		lpOverlapped	= NULL;	
	OVERLAPPEDEX*		pOverlappedEx	= NULL;	
	SocketIOCP*			pIOCP			= SocketIOCP::GetInstance();
	int					nLastError		= 0;
	
	while( WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0 ){
		BOOL bResult = pIOCP->GetQueuedEvent(&dwBytesLen, &pSocket, &lpOverlapped);
		if( !bResult ){
			if( pSocket ){
				nLastError = WSAGetLastError();
				if( pOverlappedEx )
					pOverlappedEx->m_dwLastError = nLastError;
				//Last error values:
				//ERROR_SEM_TIMEOUT(121)			- Keep-alive timeouted.
				//ERROR_NETNAME_DELETED(64)			- When application is crushed or forcibly closed (The specified network name is no longer available.).
				//ERROR_CONNECTION_ABORTED(1236)	- The network connection was aborted by the local system.
				//ERROR_OPERATION_ABORTED(995)		- Operation aborted.
				pSocket->OnDisconnect(nLastError);
				}
			continue;
			}

		// PostQueuedCompletionStatus was called.
		if( pSocket == NULL ){
			// Post quit signal to the next thread.
			pIOCP->PostTerminationSignal();
			break;
			}
		
		pOverlappedEx = CONTAINING_RECORD(lpOverlapped, OVERLAPPEDEX, m_ol);
		switch(pOverlappedEx->m_ioType){
			case IORecv:{
				// FIN packet received.
				if( !dwBytesLen ){
					pOverlappedEx->m_dwLastError = WSAGetLastError();
					pSocket->OnDisconnect(nLastError);
					}
				else
					pSocket->OnDataReceived(dwBytesLen);
				break;
				}
			case IOSend:{
				pSocket->OnDataSent(dwBytesLen);
				break;
				}
			case IOConnect:{
				if( pOverlappedEx->m_bConnecting )
					pSocket->OnConnect();
				else
					pSocket->OnAccept();
				break;
				}
			case IODisconnect:{
				//TRACE(_T("OnPreDisconnect called inst=%08X error=%d OverlappedIOType=%d Thread=%d\r\n"), (DWORD)(pSocket), pOverlappedEx->m_ioType,  GetCurrentThreadId());
				pSocket->OnDisconnect(pOverlappedEx->m_dwLastError);
				delete pOverlappedEx;
				break;
				}
			};
		}
	}