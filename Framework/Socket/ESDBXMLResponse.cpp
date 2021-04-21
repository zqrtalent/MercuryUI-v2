#include "stdafx.h"
#include "..\Array\StringToStringArray.h"
#include "..\Array\StringToIntArray.h"
#include "..\XML\Markup.h"

#include "ESDBXMLRequestMan.h"
#include "ESDBXMLRequest.h"
#include "ESDBXMLResponse.h"


/////////////////////////////////////////////////////////////////////
//
//	ESDBXMLResponse - Data XML Response class.
//
/////////////////////////////////////////////////////////////////////

ESDBXMLResponse::ESDBXMLResponse(){
	m_nReqId		= 0;
	m_pRequest		= NULL;
	m_pParams		= NULL;
	} 

ESDBXMLResponse::~ESDBXMLResponse(){
	if( !m_pParams != NULL ){
		delete m_pParams;
		m_pParams = NULL;
		}
	}

void
ESDBXMLResponse::AddParam(const CString sName, const CString sValue){
	if( !sName.GetLength() )
		return;

	if( m_pParams == NULL )
		m_pParams = new StringToStringArray();

	if( !m_pParams->SetValue(sName, sValue) ){
		m_pParams->Add(sName, sValue);
		}
	}

bool
ESDBXMLResponse::GetParam(int nIndex, CString& sName, CString& sValue){
	if( nIndex < 0 || nIndex >= m_pParams->GetCount() )
		return false;
	sName	= m_pParams->GetKey				(nIndex);
	sValue	= m_pParams->GetValueByIndex	(nIndex);
	return true;
	}

bool
ESDBXMLResponse::ParseXMLString(CString sXML, ESDBXMLRequestMan* pReqMan){
	CMarkup xmlDoc;
	if( !xmlDoc.SetDoc(sXML) )
		return false;

	if( !xmlDoc.FindElem() )
		return false;

	StringToIntArray*	pReqTypes	= ESDBXMLRequest::GetRequestTypes();
 	RequestType			reqType		= (RequestType)pReqTypes->GetValue(xmlDoc.GetTagName());
	if( reqType == RequestType::db_unknown )
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

	int	nReqId	= atoi(m_pParams->GetValue("req_id"));
	ESDBXMLRequest* req	= pReqMan->GetRequestById(nReqId);
	if( req == NULL || req->GetResponse() != NULL )
		return false;

	// Attach to request object.
	m_pRequest = req;
	req->SetType	(reqType);
	req->SetResponse(this);
	return true;
	}

const CString
ESDBXMLResponse::GetParam(const CString sName){
	if( !sName.GetLength() || m_pParams == NULL )
		return _T("");
	return m_pParams->GetValue(sName);
	}

int
ESDBXMLResponse::ClearParams(){
	if( !m_pParams )
		return 0;
	int nCt = m_pParams->GetCount();
	delete m_pParams;
	m_pParams = NULL;
	return nCt;
	}
