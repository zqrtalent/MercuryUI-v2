#include "stdafx.h"
#include "SocketIOQueue.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SocketIOQueue.
//
/////////////////////////////////////////////////////////////////////

SocketIOQueue::SocketIOQueue(){
	m_hIOCP = NULL;
	m_pKey	= NULL;
	}

SocketIOQueue::~SocketIOQueue(){
	Free();
	}

bool
SocketIOQueue::Create(){
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
	if( !BindSocket(sockNew, (ULONG_PTR)m_pKey) )
		return false;
	return true;
	}

void
SocketIOQueue::Free(){
	if( m_hIOCP == NULL )
		return;

	closesocket(m_pKey->_sock);
	CloseHandle(m_hIOCP);
	m_hIOCP = 0L;
	delete m_pKey;
	m_pKey = NULL;
	}

bool
SocketIOQueue::BindSocket(SOCKET sock, ULONG_PTR key){
	if( !m_hIOCP ) return false;
	return (CreateIoCompletionPort((HANDLE)sock, m_hIOCP, key, 0) == m_hIOCP);
	}

bool
SocketIOQueue::GetQueuedEvent(LPDWORD lpBytesLen, ULONG_PTR* lpKeyObject, LPOVERLAPPED* lpOverlapped){
	if( !m_hIOCP ) return false;
	return (::GetQueuedCompletionStatus(m_hIOCP, lpBytesLen, lpKeyObject, lpOverlapped, INFINITE) == 1);
	}

bool
SocketIOQueue::PostInitializeEvent(ULONG_PTR keyObject){
	if( m_hIOCP == NULL )
		return false;
	OVERLAPPEDEX* pOver = new OVERLAPPEDEX(SockIOType::IOInitialize);
	return (PostQueuedCompletionStatus(m_hIOCP, 0, keyObject, &pOver->m_ol) == 1);
	}

bool
SocketIOQueue::PostDisconnectEvent(ULONG_PTR keyObject){
	if( m_hIOCP == NULL )
		return false;
	OVERLAPPEDEX* pOver = new OVERLAPPEDEX(SockIOType::IODisconnect);
	return (PostQueuedCompletionStatus(m_hIOCP, 0, keyObject, &pOver->m_ol) == 1);
	}

bool
SocketIOQueue::PostEvent(OVERLAPPEDEX* lpOverlapped, ULONG_PTR keyObject){
	if( m_hIOCP == NULL )
		return false;
	return (PostQueuedCompletionStatus(m_hIOCP, 0, keyObject, &lpOverlapped->m_ol) == 1);
	}

void
SocketIOQueue::PostTerminationSignal(){
	if( m_hIOCP == NULL )
		return;
	PostQueuedCompletionStatus(m_hIOCP, 0, NULL, NULL);
	}