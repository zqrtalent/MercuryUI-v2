#include "stdafx.h"
#include "ESDBXMLServer.h"
#include "ESSocketMan.h"
#include "ESSocket.h"
#include "ESDBXMLRequestMan.h"
#include "ESDBXMLRequest.h"
#include "ESDBXMLResponse.h"

/////////////////////////////////////////////////////////////////////
//
//	ESDBXMLServer - Data XML Server class.
//
/////////////////////////////////////////////////////////////////////

ESDBXMLServer::ESDBXMLServer(){
	m_shPort			= ESDBXMLServer_PORT;
	m_pSockListen		= NULL;
	m_pSockConnect		= NULL;
	m_instanceType		= Unknown;
	m_pRequestMan		= new ESDBXMLRequestMan();
	m_dwMainThreadId	= GetCurrentThreadId();
	} 

ESDBXMLServer::~ESDBXMLServer(){
	Destroy(); // Destroy everything.
	}

bool
ESDBXMLServer::Start(){
	// Only server instance could do that.
	if( m_instanceType != Server ) return false;
	ESSocketMan*	pSockMan	= ESSocketMan::GetInstance();
	ESSocket*		pSockListen = pSockMan->Listen(m_shPort, _T(""));
	if( !pSockListen ) return false;

	pSockListen->SetAcceptNotifier((AcceptProc)&ESDBXMLServer::OnAcceptConnection, this);
	m_pSockListen = pSockListen;
	return true;
	}

bool
ESDBXMLServer::Stop(){
	// Only server instance could do that.
	if( m_instanceType != Server ) return false;
	if( !m_pSockListen )
		return true;

	m_pSockListen->SetAcceptNotifier			(NULL, NULL);
	m_pSockListen->SetReceiveDataNotifier		(NULL, NULL);
	m_pSockListen->SetDisconnectedNotifier		(NULL, NULL);
	// Destroy listening socket.
	ESSocketMan::GetInstance()->DestroySocket	(m_pSockListen);
	m_pSockListen = NULL;
	return true;
	}

bool
ESDBXMLServer::Restart(){
	// Only server instance could do that.
	if( m_instanceType != Server ) return false;
	return false;
	}

bool
ESDBXMLServer::IsConnected(){
	return (m_pSockConnect ? (m_pSockConnect->IsConnected() == TRUE) : false);
	}

bool
ESDBXMLServer::Connect(){
	if( m_pSockConnect && m_pSockConnect->IsConnected() ){
		return true; // Already connected.
		}

	m_pSockConnect = ESSocketMan::GetInstance()->Connect(m_shPort, _T("127.0.0.1"));
	if( !m_pSockConnect ){
		return false;
		}
	
	// Set data receive notifier method.
	m_pSockConnect->SetReceiveDataNotifier((ReceiveDataProc)&ESDBXMLServer::OnReceiveReply, this);
	return true;
	}

bool
ESDBXMLServer::Disconnect(){
	if( m_pSockConnect ){
		m_pSockConnect->Disconect();
		ESSocketMan::GetInstance()->DestroySocket(m_pSockConnect);
		m_pSockConnect = NULL;
		return true;
		}
	return false;
	}

bool
ESDBXMLServer::OpenDatabase(const CString sDbName, const CString sConnString, CString& sErrorString){
	// Only client instance could do that.
	if( m_instanceType != Client ) return false;
	if( !IsConnected() ) return false;

	ESDBXMLRequestMan* pReqMan = m_pRequestMan;
	// Create request.
	int nReqId = pReqMan->CreateRequest(RequestType::db_connect);
	// Add params.
	pReqMan->AddRequestParam(nReqId, _T("req_id"), nReqId); 
	pReqMan->AddRequestParam(nReqId, _T("name"), sDbName); 
	pReqMan->AddRequestParam(nReqId, _T("connect_sql"), sConnString); 
	// Send request.
	bool bRet = pReqMan->SendRequest(nReqId, m_pSockConnect);
	if( !bRet ){
		pReqMan->ClearRequest(nReqId);
		return false;
		}
	return bRet;
	}

bool
ESDBXMLServer::CloseDatabase(const CString sDbName, CString& sErrorString){
	// Only client instance could do that.
	if( m_instanceType != Client ) return false;
	if( !IsConnected() ) return false;

	ESDBXMLRequestMan* pReqMan = m_pRequestMan;
	// Create request.
	int nReqId = pReqMan->CreateRequest(RequestType::db_disconnect);
	// Add params.
	pReqMan->AddRequestParam(nReqId, _T("req_id"), nReqId); 
	pReqMan->AddRequestParam(nReqId, _T("name"), sDbName); 
	// Send request.
	bool bRet = pReqMan->SendRequest(nReqId, m_pSockConnect);
	
	if( !bRet ){
		pReqMan->ClearRequest(nReqId);
		return false;
		}

	// Wait for response.
	return bRet;
	}

bool
ESDBXMLServer::QuerySQL(const CString sDbName, const CString sQuerySQL, CString& sErrorString){
	// Only client instance could do that.
	if( m_instanceType != Client ) return false;
	return false;
	}

bool
ESDBXMLServer::ExecuteSQL(const CString sDbName, const CString sExecuteSQL, CString& sErrorString){
	// Only client instance could do that.
	if( m_instanceType != Client ) return false;
	return false;
	}

//////////////////////////////////////////////////
//
//	Event methods.
//
//////////////////////////////////////////////////

void
ESDBXMLServer::OnAcceptConnection(ESSocket* pAccepted, BOOL& bAccept){
	pAccepted->SetReceiveDataNotifier((ReceiveDataProc)&ESDBXMLServer::OnReceiveRequest, this);
	bAccept = TRUE;
	}

void
ESDBXMLServer::OnReceiveRequest(ESSocket* pSockFrom, BYTE* lpData, UINT nSize){ // As Server
	TRACE1(_T("Received request %d bytes"), nSize);

	ESDBXMLRequest* req = NULL;
	if( m_pRequestMan->ParseRequest(req, lpData, (int)nSize) ){
		// Commit request. ##################
		req->SetSocket(pSockFrom);
		req->SetStatus(RequestStatus::Received);		
		m_pRequestMan->CommitRequest(req, TRUE);
		// ##################################
		}
	}

void
ESDBXMLServer::OnReceiveReply(ESSocket* pSockFrom, BYTE* lpData, UINT nSize){ // As Client
	TRACE1(_T("Received reply %d bytes"), nSize);

	ESDBXMLResponse* response = NULL;
	if( m_pRequestMan->ParseResponse(response, lpData, (int)nSize) ){
		ESDBXMLRequest* pReq = response->GetRequest();
		pReq->SetSocket(pSockFrom);
		}
	}

void
ESDBXMLServer::OnDisconnected(ESSocket* pSock){
	}

void
ESDBXMLServer::Destroy(){
	if( m_instanceType == Server )
		Stop();
	if( m_instanceType == Client )
		Disconnect();

	if( m_pRequestMan ){
		delete m_pRequestMan;
		m_pRequestMan = NULL;
		}
	}