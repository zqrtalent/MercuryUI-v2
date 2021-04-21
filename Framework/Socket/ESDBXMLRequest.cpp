#include "stdafx.h"
#include "..\Array\StringToStringArray.h"
#include "..\Array\StringToIntArray.h"
#include "..\XML\Markup.h"
#include "ESDBXMLRequestMan.h"
#include "ESDBXMLRequest.h"
#include "ESDBXMLResponse.h"
#include "ESSocket.h"

#include <afxdb.h>

/////////////////////////////////////////////////////////////////////
//
//	ESDBXMLRequest - Data XML Request class.
//
/////////////////////////////////////////////////////////////////////

ESDBXMLRequest::ESDBXMLRequest(){
	m_nReqId		= -1;
	m_reqType		= RequestType::db_unknown;
	m_reqStatus		= RequestStatus::Unknown;
	m_pParams		= NULL;
	m_pResponse		= NULL;
	m_pSock			= NULL;
	} 

ESDBXMLRequest::~ESDBXMLRequest(){
	if( !m_pParams != NULL ){
		delete m_pParams;
		m_pParams = NULL;
		}

	if( m_pResponse != NULL ){
		delete m_pResponse;
		m_pResponse = NULL;
		}
	}

CString
ESDBXMLRequest::GetRequestNameByType(RequestType type){
	switch(type){
		case RequestType::db_connect:
			return _T("db_connect");
		case RequestType::db_disconnect:
			return _T("db_disconnect");
		case RequestType::db_execute:
			return _T("db_execute");
		case RequestType::db_query:
			return _T("db_query");
		default:
			return _T("");
		}
	}

StringToIntArray*
ESDBXMLRequest::GetRequestTypes(){
	static StringToIntArray arrReqTypes;
	if( arrReqTypes.GetCount() == 0 ){
		arrReqTypes.Add(_T("db_connect"), (int)RequestType::db_connect);
		arrReqTypes.Add(_T("db_disconnect"), (int)RequestType::db_disconnect);
		arrReqTypes.Add(_T("db_execute"), (int)RequestType::db_execute);
		arrReqTypes.Add(_T("db_query"), (int)RequestType::db_query);
		}
	return &arrReqTypes;
	}

bool
ESDBXMLRequest::ParseXMLString(CString sXML){
	if( m_reqStatus != RequestStatus::Unknown )
		return false;

	CMarkup xmlDoc;
	if( !xmlDoc.SetDoc(sXML) )
		return false;

	if( !xmlDoc.FindElem() )
		return false;

	StringToIntArray*	pReqTypes	= ESDBXMLRequest::GetRequestTypes();
 	RequestType			req			= (RequestType)pReqTypes->GetValue(xmlDoc.GetTagName());
	if( req == RequestType::db_unknown )
		return false;

	if( !xmlDoc.IntoElem() )
		return false;

	// Fill params array. ######
	if( m_pParams )
		delete m_pParams;
	m_pParams = new StringToStringArray();

	while( xmlDoc.FindElem() ){
		m_pParams->Add(xmlDoc.GetTagName(), xmlDoc.GetData());
		}
	// #########################

	int nReqId = atoi(m_pParams->GetValue("req_id"));
	SetId	(nReqId);
	SetType	(req);
	return true;
	}

void
ESDBXMLRequest::AddParam(const CString sName, const CString sValue){
	if( !sName.GetLength() )
		return;

	if( m_pParams == NULL ){
		m_pParams = new StringToStringArray();
		}

	if( !m_pParams->SetValue(sName, sValue) ){
		m_pParams->Add(sName, sValue);
		}
	}
	
const CString
ESDBXMLRequest::GetParam(const CString sName){
	if( !sName.GetLength() || m_pParams == NULL )
		return _T("");
	return m_pParams->GetValue(sName);
	}

bool
ESDBXMLRequest::GetParam(int nIndex, CString& sName, CString& sValue){
	if( nIndex < 0 || nIndex >= m_pParams->GetCount() )
		return false;
	sName	= m_pParams->GetKey				(nIndex);
	sValue	= m_pParams->GetValueByIndex	(nIndex);
	return true;
	}

int
ESDBXMLRequest::ClearParams(){
	if( !m_pParams )
		return 0;
	int nCt = m_pParams->GetCount();
	delete m_pParams;
	m_pParams = NULL;
	return nCt;
	}

bool
ESDBXMLRequest::CommitRequest(ESDBXMLRequestMan* pReqMan){

	if( GetType() == RequestType::db_connect ){
		CString		sConnString = GetParam(_T("connect_sql"));
		CString		sName		= GetParam(_T("name"));
		CDatabase*	pDb			= new CDatabase();

		pReqMan->CreateResponse		(m_nReqId);
		pReqMan->AddResponseParam	(m_nReqId, _T("name"), sName);
		pReqMan->AddResponseParam	(m_nReqId, _T("connect_sql"), sConnString);
		
		try{
			pDb->OpenEx(sConnString);
			pReqMan->AddResponseParam	(m_nReqId, _T("err_code"), _T("0"));
			pReqMan->AddResponseParam	(m_nReqId, _T("err_text"), _T(""));
			pReqMan->AddNamedObject		(sName, (void*)pDb);
			}
		catch(CDBException* e){
			pReqMan->AddResponseParam	(m_nReqId, _T("err_code"), (int)e->m_nRetCode);
			pReqMan->AddResponseParam	(m_nReqId, _T("err_text"), e->m_strError);
			}
		return true;
		}
	else
	if( GetType() == RequestType::db_disconnect ){
		CString		sName	= GetParam(_T("name"));
		CDatabase*	pDb		= (CDatabase*)pReqMan->GetNamedObject(sName);

		if( pDb ){
			if( pDb->IsOpen() )
				pDb->Close();
			pReqMan->RemoveNamedObject(sName);
			}
		return true;
		}

	return false;
	}