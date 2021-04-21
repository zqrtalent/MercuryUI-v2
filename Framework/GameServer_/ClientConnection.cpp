#include "stdafx.h"
#include "ClientConnection.h"
#include "GameServer.h"
#include "../SocketLibrary/SocketServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class ClientConnection
///
///////////////////////////////////////////////////////////

ClientConnection::ClientConnection(ConnectedSocketInfo* pSockInfo, GameServer* pServer, SocketContext* pSockConn, SocketServer* pSockServer){
	m_nConnectionId		= 0;
	m_sock				= 0;
	m_pSession			= NULL;
	m_bAuthorized		= false;
	m_bTableConnection	= false;
	m_nTableId			= 0;
	m_nTournamentId		= 0;
	m_bConnected		= false;
	m_bSending			= false;
	m_nPortLocal		= 0;
	m_nPortRemote		= 0;
	m_dwIpRemote		= 0;
	m_pSockConn			= pSockConn;
	m_pSockServer		= pSockServer;

	if( pSockConn )
		pSockConn->SetParam(this);

	Initialize(pSockInfo, pServer);
	}

ClientConnection::~ClientConnection(){
	if( m_pSession )
		SafePointer::Release(m_pSession); // Release pointer.
	m_pSession = NULL;
	}

bool
ClientConnection::Initialize(ConnectedSocketInfo* pSockInfo, GameServer* pServer){
	if( !pSockInfo ) return false;
	SocketServer::GetIpAddress(pSockInfo->m_sock, m_sIpLocal, m_nPortLocal, m_sIpRemote, m_nPortRemote, &m_dwIpRemote);
	m_sock			= pSockInfo->m_sock;
	m_pServer		= pServer;
	m_bConnected	= true;
	return true;
	}

Session*
ClientConnection::GetSession(bool bRetain){
	if( bRetain && m_pSession )
		return (Session*)SafePointer::Retain(m_pSession);
	return m_pSession;
	}

__int64
ClientConnection::GetSessionId(){
	return (m_pSession ? m_pSession->GetSessionId() : 0);
	}

void
ClientConnection::SetSession(Session* pSession){
	if( pSession == m_pSession ) return;
	if( m_pSession )
		SafePointer::Release(m_pSession); // Release pointer.
	if( pSession )
		pSession = (Session*)SafePointer::Retain(pSession); // Retain pointer.
	m_pSession = pSession;
	}

bool
ClientConnection::Send(GrowableMemory* pMem){
	if( !IsConnected() || !m_pSockConn )
		return false;
	/*
	//Temp
	// Split send data into pieces and send seperately.
	int nBytesSizeSendPart = 30;
	int nPartCt = (pMem->GetUsedBufferSize() / nBytesSizeSendPart);
	if( (pMem->GetUsedBufferSize() % nBytesSizeSendPart) > 0 )
		nPartCt ++;

	for(int i=0; i<nPartCt; i++){
		int nOffset = i*nBytesSizeSendPart;
		int nSize	= ((i == nPartCt - 1) ? pMem->GetUsedBufferSize() % nBytesSizeSendPart : nBytesSizeSendPart);
		m_pSockConn->Send(&pMem->GetBufferPtr()[nOffset], nSize);

		}
	return true;
	// Temp
	*/
	return m_pSockConn->Send(pMem->GetBufferPtr(), pMem->GetUsedBufferSize());
	}

bool
ClientConnection::SendBytes(BYTE* lpBytes, UINT nSize){
	if( !IsConnected() || !m_pSockConn )
		return false;
	/*
	//Temp
	// Split send data into pieces and send seperately.
	int nBytesSizeSendPart = 30;
	int nPartCt = (nSize / nBytesSizeSendPart);
	if( (nSize % nBytesSizeSendPart) > 0 )
		nPartCt ++;

	for(int i=0; i<nPartCt; i++){
		int nOffset = i*nBytesSizeSendPart;
		int nSizePart	= ((i == nPartCt - 1) ? nSize % nBytesSizeSendPart : nBytesSizeSendPart);
		m_pSockConn->Send(&lpBytes[nOffset], nSizePart);

		//Sleep(10);
		}

	return true;
	// Temp
	*/
	return m_pSockConn->Send(lpBytes, nSize);
	}

ClientConnection*
ClientConnection::GetCopy(bool bKeepSession /*= false*/){
	ClientConnection* pCopy = new ClientConnection(NULL, NULL, NULL, NULL);
	pCopy->m_sock			= m_sock;
	pCopy->m_pServer		= m_pServer;
	if( !bKeepSession )
		pCopy->SetSession(m_pSession);
	else
		pCopy->m_pSession	= NULL;
	pCopy->m_bAuthorized	= m_bAuthorized;
	pCopy->m_bConnected		= m_bConnected;
	pCopy->m_bSending		= m_bSending;
	pCopy->m_nPortLocal		= m_nPortLocal;
	pCopy->m_nPortRemote	= m_nPortRemote;
	pCopy->m_sIpLocal		= m_sIpLocal;
	pCopy->m_sIpRemote		= m_sIpRemote;
	pCopy->m_dwIpRemote		= m_dwIpRemote;
	pCopy->m_pSockServer	= m_pSockServer;

	// Empty old one
	m_sock			= 0;
	if( !bKeepSession )
		SetSession(NULL);
	m_bAuthorized	= false;
	m_bConnected	= false;
	m_bSending		= false;
	return pCopy;
	}