#include "stdafx.h"
#include "SocketServerContext.h"
#include "SocketServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SocketServerContext.
//
/////////////////////////////////////////////////////////////////////

SocketServerContext::SocketServerContext(SocketServer* pSockServer, UINT nSendBuffSize /*= 8192*/, UINT nReceiveBuffSize /*= 8192*/) 
	: SocketContext(nSendBuffSize, nReceiveBuffSize){
	m_pSockServer = pSockServer;
	} 

SocketServerContext::~SocketServerContext(){
	}

void
SocketServerContext::OnInitialize(){
	/*
	DecreaseIOWorkersReferenceCounter();
	if( !m_pSockIOCP->BindSocket(m_sock, (ULONG_PTR)this) ){
		OnDisconnect(0);
		return;
		}
	// Start waiting data.
	ReceiveBytes();
	*/
	}

void
SocketServerContext::OnDisconnect(DWORD dwLastError){
	m_pSockServer->OnDisconnected(this, dwLastError);
	}

void
SocketServerContext::OnDataReceived(DWORD dwSize){
	DecreaseIOWorkersReferenceCounter();
	if( !dwSize ){
		OnDisconnect(0);
		return;
		}

	DWORD dwBytesReceived = m_dwBytesReceived;
	::EnterCriticalSection(&m_lock); // Lock
	if( ReadReceivedBytes(dwSize) ){
		m_pSockServer->OnDataReceived(this, &m_receivedData, (dwBytesReceived == 0));
		// Zero used memory fore received data bytes.
		m_receivedData.ZeroUsedMemory();
		}
	::LeaveCriticalSection(&m_lock); // Unlock

	// Start waiting data.
	ReceiveBytes();
	}

void
SocketServerContext::OnDataSent(DWORD dwSize){
	DecreaseIOWorkersReferenceCounter();
	m_dwBytesSent += dwSize;
	if( m_bSendIsInProgress ){
		m_bSendIsInProgress = false;
		SendBytes();
		}
	}