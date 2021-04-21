#include "stdafx.h"
#include "ESSocketMan.h"
#include "ESSocket.h"

/////////////////////////////////////////////////////////////////////
//
//	ESSocketMan - manager class.
//
/////////////////////////////////////////////////////////////////////

ESSocketMan::ESSocketMan(){
	}

ESSocketMan::~ESSocketMan(){
	DestroyAllSockets();
	}

ESSocket*
ESSocketMan::Listen(short shPort, CString sIpAddress){
	ESSocket* pSockNew = new ESSocket();
	if( !pSockNew->Bind(shPort, sIpAddress) ){
		delete pSockNew;
		return NULL;
		}
	m_listListenSock.Add(pSockNew);
	return pSockNew;
	}

ESSocket*
ESSocketMan::Connect(short shPort, CString sIpAddress){
	ESSocket* pSockNew = new ESSocket();
	if( !pSockNew->Connect(shPort, sIpAddress) ){
		delete pSockNew;
		return NULL;
		}
	m_listReadWriteSock.Add(pSockNew);
	return pSockNew;
	}

void
ESSocketMan::OnReceiveData(ESSocket* pSockSrc, BYTE* lpData, UINT nSize){
	// Call notify method.
	pSockSrc->Call_ReceiveDataNotifier(pSockSrc, lpData, nSize);
	}

BOOL
ESSocketMan::OnAccept(ESSocket* pSockSrc, SOCKET sockAccept){
	ESSocket* pSockNew = new ESSocket();

	// Call notify method.
	BOOL bAccept = TRUE;
	pSockSrc->Call_AcceptNotifier(pSockNew, bAccept);
	if( !bAccept ){
		delete pSockNew;
		return FALSE;
		}
	
	if( !pSockNew->Accept(sockAccept) ){
		delete pSockNew;
		return FALSE;
		}

	m_listReadWriteSock.Add(pSockNew);
	return TRUE;
	}

BOOL
ESSocketMan::DestroySocket(ESSocket* pSock){
	if( !m_listListenSock.Remove(pSock) ){
		return m_listReadWriteSock.Remove(pSock);
		}
	return TRUE;
	}

void
ESSocketMan::DestroyAllSockets(){
	m_listListenSock	.RemoveAll();
	m_listReadWriteSock	.RemoveAll();
	}