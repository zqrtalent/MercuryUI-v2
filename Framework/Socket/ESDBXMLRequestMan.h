#pragma once

/////////////////////////////////////////
//	class: ESDBXMLRequestMan
//	programmer:	ZqrTalent
//	Date:	30/6/2009
/////////////////////////////////////////

#include "..\Array\List.h"
#include "..\Array\List.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include "..\Array\StringToPtrArray.h"

enum RequestType;
class ESSocket;
class ESDBXMLRequest;
class ESDBXMLResponse;

class ESDBXMLRequestMan
{
protected:
	ESDBXMLRequestMan();
	~ESDBXMLRequestMan();

	friend class ESDBXMLServer;

public:
	/*
	static ESDBXMLRequestMan* Instance(){
		static ESDBXMLRequestMan reqMan;
		return &reqMan;
		}*/

public:
	int				CreateRequest		(RequestType req);
	bool			AddRequestParam		(int nReqId, const CString sName, const CString sValue);
	bool			AddRequestParam		(int nReqId, const CString sName, int nValue);
	bool			GetRequestString	(int nReqId, CString& sRequest, bool bClearAfter = true);
	bool			ClearRequest		(int nReqId);
	bool			SendRequest			(int nReqId, ESSocket* pSock);
	ESDBXMLRequest* GetRequestById		(int nReqId);
	bool			ParseRequest		(ESDBXMLRequest*& req, BYTE* pBytes, int nLength);

	bool			CreateResponse		(int nReqId);
	bool			AddResponseParam	(int nReqId, const CString sName, const CString sValue);
	bool			AddResponseParam	(int nReqId, const CString sName, int nValue);
	bool			GetResponseString	(int nReqId, CString& sResponse);
	bool			SendResponse		(int nReqId, ESSocket* pSock);
	ESDBXMLResponse* GetResponseById	(int nReqId);
	bool			ParseResponse		(ESDBXMLResponse*& response, BYTE* pBytes, int nLength);
	bool			CommitRequest		(ESDBXMLRequest* pReq, BOOL bAsync = TRUE);

	bool			AddNamedObject		(CString sName, void* lpObj);
	void*			GetNamedObject		(CString sName);
	void			RemoveNamedObject	(CString sName);

protected:
	// Request provider thread.
	bool			StartThread			();	// Start thread.
	bool			StopThread			();	// Stop thread.
	bool			ResumeThread		();	// Resume thread to execute requests.

protected:
	List<ESDBXMLRequest>						m_listRequests;
	AutoSortedArrayTempl<int, ESDBXMLRequest*>	m_listReqIdByRequest;
	int											m_nReqIdCounter;

	HANDLE										m_hReqManager;			// Request manager thread handle;
	DWORD										m_dwReqManThreadId;		// Request manager thread id.
	HANDLE										m_hReqMan_StopEvent;	// Request manager thread control event handle.	
	HANDLE										m_hReqMan_Continue;		// Request manager thread control event handle.	
	CCriticalSection							m_secReqMan; 

	StringToPtrArray							m_arrObjects;			// List of objects by name.

};