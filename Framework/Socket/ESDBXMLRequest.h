#pragma once

/////////////////////////////////////////
//	class: ESDBXMLRequestMan
//	programmer:	ZqrTalent
//	Date:	30/6/2009
/////////////////////////////////////////

enum RequestStatus{
	Unknown = 0,
	Sent,
	Received,
	WaitedForResponse,
	Completed
	};

enum RequestType{
	db_unknown = 0,
	db_connect,
	db_disconnect,
	db_query,
	db_execute
	};


class StringToStringArray;
class StringToIntArray;
class ESDBXMLResponse;
class ESDBXMLRequestMan;
class ESSocket;


class ESDBXMLRequest
{
public:
	ESDBXMLRequest();
	~ESDBXMLRequest();

	static StringToIntArray*	GetRequestTypes();
	static CString				GetRequestNameByType(RequestType type);

public:
	bool				ParseXMLString	(CString sXML);
	void				SetStatus		(RequestStatus reqStatus){m_reqStatus = reqStatus;};
	RequestStatus		GetStatus		(){return m_reqStatus;};
	void				SetType			(RequestType reqType){m_reqType = reqType;};
	RequestType			GetType			(){return m_reqType;};
	void				SetId			(int nId){m_nReqId = nId;};
	int					GetId			(){return m_nReqId;};

	void				AddParam		(const CString sName, const CString sValue);
	const CString		GetParam		(const CString sName);
	bool				GetParam		(int nIndex, CString& sName, CString& sValue);
	int					ClearParams		(); // Returns count of existing params.

	ESDBXMLResponse*	GetResponse		(){return m_pResponse;};
	void				SetResponse		(ESDBXMLResponse* pResponse){m_pResponse = pResponse;};

	void				SetSocket		(ESSocket* pSock){m_pSock = pSock;};
	ESSocket*			GetSocket		(){return m_pSock;};

	bool				CommitRequest	(ESDBXMLRequestMan* pReqMan);

protected:
	int						m_nReqId;
	RequestType				m_reqType;
	RequestStatus			m_reqStatus;
	StringToStringArray*	m_pParams;
	ESDBXMLResponse*		m_pResponse;
	ESSocket*				m_pSock;		// Socket received from or socket which we must send reply.
};