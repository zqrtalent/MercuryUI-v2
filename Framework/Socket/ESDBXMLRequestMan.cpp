#include "stdafx.h"
#include "ESSocket.h"
#include "ESDBXMLRequest.h"
#include "ESDBXMLResponse.h"
#include "ESDBXMLRequestMan.h"

/////////////////////////////////////////////////////////////////////
//
//	Request manager thread.
//
/////////////////////////////////////////////////////////////////////

struct RequestManagerThreadStruct{
	HANDLE										hEventStop;				// Stop event.
	HANDLE										hEventContinue;			// Continue event.
	CCriticalSection*							pCritSection;			
	List<ESDBXMLRequest>*						pListRequests;			// List of requests.
	AutoSortedArrayTempl<int, ESDBXMLRequest*>*	pListReqIdByRequest;	// Dictionary of requests by id.
	ESDBXMLRequestMan*							pReqMan;
	};

UINT 
RequestManagerProc(RequestManagerThreadStruct* pData){
	CCriticalSection* pSect = pData->pCritSection;
	ASSERT( pSect );

	ESDBXMLRequest*								pRequest				= NULL;
	List<ESDBXMLRequest>*						pListRequests			= pData->pListRequests;
	AutoSortedArrayTempl<int, ESDBXMLRequest*>*	pListReqIdByRequest		= pData->pListReqIdByRequest;
	ESDBXMLRequestMan*							pReqMan					= pData->pReqMan;

	int nLoop = 0;

	while( TRUE ){
		// Wait for 2 seconds till continue event has not signaled.
		WaitForSingleObject(pData->hEventContinue, INFINITE);

		// If stop event is signaled.
		if( WaitForSingleObject(pData->hEventStop, 0) == WAIT_OBJECT_0 ){
			delete pData;
			return 0L;
			}

		//pSect->Lock();

		nLoop		= 0;
		pRequest	= pListRequests->GetAt(nLoop);

		while( pRequest ){
			if( pRequest->GetStatus() == RequestStatus::Received ){
				// Commit request synchroniously.
				pReqMan->CommitRequest(pRequest, FALSE);
				}
			pRequest = pListRequests->GetAt(++ nLoop);
			}

		ResetEvent(pData->hEventContinue);
		//pSect->Unlock();
		}

	delete pData;
	return 0L;
	}

/////////////////////////////////////////////////////////////////////
//
//	ESDBXMLRequestMan - request manager class.
//
/////////////////////////////////////////////////////////////////////

ESDBXMLRequestMan::ESDBXMLRequestMan(){
	m_nReqIdCounter		= 1000;
	m_hReqManager		= NULL;		
	m_dwReqManThreadId	= 0L;
	m_hReqMan_StopEvent	= NULL;
	m_hReqMan_Continue	= NULL;
	} 

ESDBXMLRequestMan::~ESDBXMLRequestMan(){
	StopThread();
	}

int
ESDBXMLRequestMan::CreateRequest(RequestType req){
	ESDBXMLRequest* reqNew = new ESDBXMLRequest();
	reqNew->SetType		(req);
	reqNew->SetId		(++m_nReqIdCounter);
	// Add request into list.
	m_listRequests.Add	(reqNew, true);
	// Add into key array.
	m_listReqIdByRequest.Add(m_nReqIdCounter, reqNew);	
	return m_nReqIdCounter;
	}

bool
ESDBXMLRequestMan::AddRequestParam(int nReqId, const CString sName, const CString sValue){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) )
		return false;
	req->AddParam(sName, sValue);
	return true;
	}

bool
ESDBXMLRequestMan::AddRequestParam(int nReqId, const CString sName, int nValue){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) )
		return false;

	CString sValue;
	sValue.Format(_T("%d"), nValue);
	req->AddParam(sName, sValue);
	return true;
	}

bool
ESDBXMLRequestMan::GetRequestString(int nReqId, CString& sRequest, bool bClearAfter/* = true*/){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) )
		return false;

	CString sName, sValue, sFormat;
	int nLoop = 0;
	sFormat.Format(_T("<%s>"), ESDBXMLRequest::GetRequestNameByType(req->GetType()));
	sRequest = sFormat;

	while( req->GetParam(nLoop, sName, sValue) ){
		sFormat.Format(_T("<%s>%s</%s>"), sName, sValue, sName);
		sRequest += sFormat;
		nLoop ++;
		}

	sFormat.Format(_T("</%s>"), ESDBXMLRequest::GetRequestNameByType(req->GetType()));
	sRequest += sFormat;
	return true;
	}

bool
ESDBXMLRequestMan::ClearRequest(int nReqId){
	int nIndex = m_listReqIdByRequest.FindValue(nReqId);
	if( nIndex == -1 )
		return false;

	ESDBXMLRequest* req = m_listReqIdByRequest.GetData(nIndex);
	// Remove from array.
	m_listReqIdByRequest.Delete(nIndex);
	// Remove from list.
	m_listRequests.Remove(req);
	return true;
	}

bool
ESDBXMLRequestMan::SendRequest(int nReqId, ESSocket* pSock){
	if( !pSock ) return false;

	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) )
		return false;

	CString sReq;
	if( !GetRequestString(nReqId, sReq, false) )
		return false;
	
	int		nSentBytes	= pSock->Send((BYTE*)sReq.GetBuffer(), sReq.GetLength() + 1);
	bool	bRet		= (nSentBytes == sReq.GetLength() + 1); 
	
	if( bRet )
		req->SetStatus(RequestStatus::Sent);
	return bRet;
	}

ESDBXMLRequest* 
ESDBXMLRequestMan::GetRequestById(int nReqId){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) )
		return NULL;
	return req;
	}

bool
ESDBXMLRequestMan::ParseRequest(ESDBXMLRequest*& req, BYTE* pBytes, int nLength){
	if( !pBytes || nLength == 0 || pBytes[nLength-1] != 0 )
		return false;

	CString sXML((char*)pBytes);
	ESDBXMLRequest* reqParsed = new ESDBXMLRequest();
	if( !reqParsed->ParseXMLString(sXML) ){
		delete reqParsed;
		return false;
		}
	
	if( GetRequestById(reqParsed->GetId()) != NULL ){
		delete reqParsed;
		return false;
		}

	m_listReqIdByRequest.Add(reqParsed->GetId(), reqParsed);
	m_listRequests		.Add(reqParsed);
	req = reqParsed;
	return true;
	}

bool
ESDBXMLRequestMan::CreateResponse(int nReqId){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) )
		return false;

	ESDBXMLResponse* pResponse = new ESDBXMLResponse();
	req->SetResponse(pResponse);
	pResponse->SetRequest(req);
	return true;
	}

bool
ESDBXMLRequestMan::AddResponseParam	(int nReqId, const CString sName, const CString sValue){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) || req->GetResponse() == NULL )
		return false;

	ESDBXMLResponse* response = req->GetResponse();
	response->AddParam(sName, sValue);
	return true;

	}
bool
ESDBXMLRequestMan::AddResponseParam	(int nReqId, const CString sName, int nValue){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) || req->GetResponse() == NULL )
		return false;

	CString sValue;
	sValue.Format(_T("%d"), nValue);
	ESDBXMLResponse* response = req->GetResponse();
	response->AddParam(sName, sValue);
	return true;
	}

bool
ESDBXMLRequestMan::GetResponseString(int nReqId, CString& sResponse){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) || req->GetResponse() == NULL )
		return false;

	ESDBXMLResponse* response = req->GetResponse();
	CString sName, sValue, sFormat;
	int nLoop = 0;
	sFormat.Format(_T("<%s>"), ESDBXMLRequest::GetRequestNameByType(req->GetType()));
	sResponse = sFormat;

	while( response->GetParam(nLoop, sName, sValue) ){
		sFormat.Format(_T("<%s>%s</%s>"), sName, sValue, sName);
		sResponse += sFormat;
		nLoop ++;
		}

	sFormat.Format(_T("</%s>"), ESDBXMLRequest::GetRequestNameByType(req->GetType()));
	sResponse += sFormat;
	return true;
	}

bool
ESDBXMLRequestMan::SendResponse(int nReqId, ESSocket* pSock){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) || req->GetResponse() == NULL )
		return false;

	CString sResponse;
	if( !GetResponseString(nReqId, sResponse) )
		return false;
	
	int		nSentBytes	= pSock->Send((BYTE*)sResponse.GetBuffer(), sResponse.GetLength() + 1);
	bool	bRet		= (nSentBytes == sResponse.GetLength() + 1); 
	
	if( bRet )
		req->SetStatus(RequestStatus::Completed);
	return bRet;
	}

ESDBXMLResponse*
ESDBXMLRequestMan::GetResponseById(int nReqId){
	ESDBXMLRequest* req = NULL;
	if( !m_listReqIdByRequest.GetData(nReqId, req) )
		return NULL;
	return req->GetResponse();
	}

bool
ESDBXMLRequestMan::ParseResponse(ESDBXMLResponse*& response, BYTE* pBytes, int nLength){
	if( !pBytes || nLength == 0 || pBytes[nLength-1] != 0 )
		return false;

	CString sXML((char*)pBytes);
	ESDBXMLResponse* resParsed = new ESDBXMLResponse();
	if( !resParsed->ParseXMLString(sXML, this) ){
		delete resParsed;
		return false;
		}

	ESDBXMLRequest* req = resParsed->GetRequest();
	if( GetRequestById(req->GetId()) != NULL ){
		delete resParsed;
		return false;
		}

	response = resParsed;
	return true;
	}

bool
ESDBXMLRequestMan::CommitRequest(ESDBXMLRequest* pReq, BOOL bAsync /*= TRUE*/){
	if( pReq == NULL || pReq->GetType() == RequestType::db_unknown )
		return false;

	if( bAsync ){ 
		if( m_hReqManager == NULL ){
			StartThread();
			}
		ResumeThread();
		return true;
		}

	if( !pReq->CommitRequest(this) )
		return false;

	// Send reply. ######################
	if( pReq->GetResponse() && pReq->GetSocket() ){
		SendResponse(pReq->GetId(), pReq->GetSocket());
		}
	// ##################################
	return true;
	}

bool
ESDBXMLRequestMan::StartThread(){
	if( m_hReqManager != NULL )
		return false;

	m_hReqMan_StopEvent = ::CreateEvent	(NULL, FALSE, FALSE, _T("StopReqMan_Thread")); 
	m_hReqMan_Continue	= ::CreateEvent	(NULL, FALSE, FALSE, _T("ContinueReqMan_Thread")); 

	RequestManagerThreadStruct* pThreadData = new RequestManagerThreadStruct;
	pThreadData->hEventContinue	= m_hReqMan_Continue;
	pThreadData->hEventStop		= m_hReqMan_StopEvent;
	pThreadData->pCritSection	= &m_secReqMan;
	pThreadData->pListReqIdByRequest = &m_listReqIdByRequest;
	pThreadData->pListRequests	= &m_listRequests;
	pThreadData->pReqMan		= this;
	m_hReqManager				= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)RequestManagerProc, pThreadData, 0L, &m_dwReqManThreadId);
	return true;
	}

bool
ESDBXMLRequestMan::StopThread(){
	if( m_hReqManager == NULL )
		return false;

	::SetEvent			(m_hReqMan_Continue);
	::SetEvent			(m_hReqMan_StopEvent);
	WaitForSingleObject	(m_hReqManager, INFINITE);
	CloseHandle			(m_hReqManager);
	CloseHandle			(m_hReqMan_StopEvent);
	CloseHandle			(m_hReqMan_Continue);

	m_dwReqManThreadId	= 0L;
	m_hReqManager		= NULL;
	m_hReqMan_StopEvent	= NULL;
	m_hReqMan_Continue	= NULL;
	return true;
	}

bool
ESDBXMLRequestMan::ResumeThread(){
	if( m_hReqManager == NULL )
		return false;
	::SetEvent(m_hReqMan_Continue);
	return true;
	}

bool
ESDBXMLRequestMan::AddNamedObject(CString sName, void* lpObj){
	if( m_arrObjects.GetValue(sName) != NULL ){
		m_arrObjects.SetValue(sName, lpObj);
		return true;
		}
	m_arrObjects.Add(sName, lpObj);
	return true;
	}

void*
ESDBXMLRequestMan::GetNamedObject(CString sName){
	return m_arrObjects.GetValue(sName);
	}

void
ESDBXMLRequestMan::RemoveNamedObject(CString sName){
	m_arrObjects.Delete(sName);
	}
