#pragma once

/////////////////////////////////////////
//	class: ESDBXMLResponse
//	programmer:	ZqrTalent
//	Date:	30/6/2009
/////////////////////////////////////////

class StringToStringArray;
class ESDBXMLRequest;
class ESDBXMLRequestMan;

class ESDBXMLResponse
{
public:
	ESDBXMLResponse();
	~ESDBXMLResponse();

public:
	bool				ParseXMLString	(CString sXML, ESDBXMLRequestMan* pReqMan);	
	void				AddParam		(const CString sName, const CString sValue);
	bool				GetParam		(int nIndex, CString& sName, CString& sValue);
	const CString		GetParam		(const CString sName);
	int					ClearParams		(); // Returns count of existing params.

	ESDBXMLRequest*		GetRequest		(){return m_pRequest;};
	void				SetRequest		(ESDBXMLRequest* pReq){m_pRequest = pReq;};

protected:
	int						m_nReqId;
	ESDBXMLRequest*			m_pRequest;
	StringToStringArray*	m_pParams;
};