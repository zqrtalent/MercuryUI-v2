#include "stdafx.h"
#include "MaximumWebService.h"
#include "..\XML\Markup.h"
#include "..\WebService\HTTPClient.h"
#include "..\Log\Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class MaximumWebService
///
///////////////////////////////////////////////////////////

MaximumWebService::MaximumWebService(){
}

MaximumWebService::~MaximumWebService(){
}

int 
MaximumWebService::Authenticate(_String sUserName, _String sPassword){
	// Call WebService method.
	_String sResp;
	HTTPClient client;

	_String	sMethodURL;
	sMethodURL.Format(_T("%s/Authenthicate"), m_sServiceURL.GetBuffer());

	_String sParam;
	sParam.Format(_T("userName=%s&password=%s"), sUserName.GetBuffer(), sPassword.GetBuffer());
	if( !client.Post(sMethodURL, m_nPort, sParam, sResp) )
		return -1;
	// }}

	CMarkup xmlResp;
	if( !xmlResp.SetDoc(sResp) )
		return -1;

	if( xmlResp.FindElem(_T("int")) ){
		int nUserId = _ttoi((xmlResp.GetData()).GetBuffer());
		return nUserId;
		}

	return -1;
	}


bool
MaximumWebService::GetUserDetails(_String sUserName, MaximumUserDetails& user){
	// Call WebService method.
	_String sResp;
	HTTPClient client;

	_String	sMethodURL;
	sMethodURL.Format(_T("%s/GetUserInfo"), m_sServiceURL.GetBuffer());

	_String sParam;
	sParam.Format(_T("userName=%s"), sUserName.GetBuffer());
	if( !client.Post(sMethodURL, m_nPort, sParam, sResp) ){
		_String sLog;
		sLog.Format(_T("PostMethodFail URL='%s' Param='%s'"), sMethodURL.GetBuffer(), sParam.GetBuffer());
		Logger::CreateLogEntry(sLog.GetBuffer());
		return false;
		}
	// }}

	CMarkup xmlResp;
	if( !xmlResp.SetDoc(sResp) )
		return false;

	if( xmlResp.FindElem(_T("UserInfo")) ){
		xmlResp.IntoElem();
		if( xmlResp.FindElem(_T("userId")) )
			user.nUserId = _ttoi(xmlResp.GetData().GetBuffer());
		if( xmlResp.FindElem(_T("userName")) )
			user.userName = xmlResp.GetData();
		if( xmlResp.FindElem(_T("firstName")) )
			user.firstName = xmlResp.GetData();
		if( xmlResp.FindElem(_T("lastName")) )
			user.lastName = xmlResp.GetData();
		if( xmlResp.FindElem(_T("address")) )
			user.address = xmlResp.GetData();
		if( xmlResp.FindElem(_T("email")) )
			user.email = xmlResp.GetData();
		if( xmlResp.FindElem(_T("phoneNumber")) )
			user.phoneNumber = xmlResp.GetData();
		if( xmlResp.FindElem(_T("mobileNumber")) )
			user.mobileNumber = xmlResp.GetData();
		if( xmlResp.FindElem(_T("docNumber")) )
			user.docNumber = xmlResp.GetData();
		if( xmlResp.FindElem(_T("docPersonalNumber")) )
			user.docPersonalNumber = xmlResp.GetData();
		return true;
		}
	return false;
	}

/* Response XML
<MoneyTransferProgress><success>0</success><balanceNew>465.8900</balanceNew><entryId>447448051</entryId></MoneyTransferProgress>
*/

bool
MaximumWebService::DepositMoney(int nUserId, int nAmount, int& nBalance, int& nEntryId){
	// Call WebService method.
	_String sResp;
	HTTPClient client;

	_String	sMethodURL;
	sMethodURL.Format(_T("%s/DepositMoney"), m_sServiceURL.GetBuffer());

	_String sParam;
	sParam.Format(_T("userid=%d&amount=%d.%02d"), nUserId, nAmount/100, nAmount%100);
	if( !client.Post(sMethodURL, m_nPort, sParam, sResp) ){
		_String sLog;
		sLog.Format(_T("PostMethodFail URL='%s' Param='%s'"), sMethodURL.GetBuffer(), sParam.GetBuffer());
		Logger::CreateLogEntry(sLog.GetBuffer());
		return false;
		}
	// }}

	CMarkup xmlResp;
	if( !xmlResp.SetDoc(sResp) )
		return false;

	int nSuccess = -1;
	if( xmlResp.FindElem(_T("MoneyTransferProgress")) ){
		xmlResp.IntoElem();
		if( xmlResp.FindElem(_T("success")) )
			nSuccess = _ttoi(xmlResp.GetData().GetBuffer());
		if( xmlResp.FindElem(_T("balanceNew")) )
			nBalance = (int)(_ttof(xmlResp.GetData())*100.0);
		if( xmlResp.FindElem(_T("entryId")) )
			nEntryId = _ttoi(xmlResp.GetData());

		if( nSuccess == 0 ){
			// Completed successfully.
			return true;
			}
		}
	return false;
	}

bool
MaximumWebService::WithdrawMoney(int nUserId, int nAmount, int& nBalance, int& nEntryId){
	// Call WebService method.
	_String sResp;
	HTTPClient client;

	_String	sMethodURL;
	sMethodURL.Format(_T("%s/WithdrawMoney"), m_sServiceURL.GetBuffer());

	_String sParam;
	sParam.Format(_T("userid=%d&amount=%d.%02d"), nUserId, nAmount/100, nAmount%100);
	if( !client.Post(sMethodURL, m_nPort, sParam, sResp) ){
		CString sLog;
		sLog.Format(_T("PostMethodFail URL='%s' Param='%s'"), sMethodURL.GetBuffer(), sParam.GetBuffer());
		Logger::CreateLogEntry(sLog.GetBuffer());
		return false;
		}
	// }}

	CMarkup xmlResp;
	if( !xmlResp.SetDoc(sResp) )
		return false;

	int nSuccess = -1;
	if( xmlResp.FindElem(_T("MoneyTransferProgress")) ){
		xmlResp.IntoElem();
		if( xmlResp.FindElem(_T("success")) )
			nSuccess = _ttoi(xmlResp.GetData().GetBuffer());
		if( xmlResp.FindElem(_T("balanceNew")) )
			nBalance = (int)(_ttof(xmlResp.GetData())*100.0);
		if( xmlResp.FindElem(_T("entryId")) )
			nEntryId = _ttoi(xmlResp.GetData());

		if( nSuccess == 0 ){
			// Completed successfully.
			return true;
			}
		}
	return false;
	}

bool
MaximumWebService::DeleteTransferEntry(int nEntryId){
	// Call WebService method.
	_String sResp;
	HTTPClient client;

	_String	sMethodURL;
	sMethodURL.Format(_T("%s/DeleteEntry"), m_sServiceURL.GetBuffer());

	_String sParam;
	sParam.Format(_T("entryId=%d"), nEntryId);
	if( !client.Post(sMethodURL, m_nPort, sParam, sResp) ){
		CString sLog;
		sLog.Format(_T("PostMethodFail URL='%s' Param='%s'"), sMethodURL.GetBuffer(), sParam.GetBuffer());
		Logger::CreateLogEntry(sLog.GetBuffer());
		return false;
		}
	// }}

	CMarkup xmlResp;
	if( !xmlResp.SetDoc(sResp) )
		return false;

	if( xmlResp.FindElem(_T("int")) ){
		int nSuccess = _ttoi((xmlResp.GetData()).GetBuffer());
		return (nSuccess == 0);
		}

	return false;
	}

double	
MaximumWebService::GetUserBalance(int nUserId){
	// Call WebService method.
	_String sResp;
	HTTPClient client;

	_String	sMethodURL;
	sMethodURL.Format(_T("%s/GetBalance"), m_sServiceURL.GetBuffer());

	_String sParam;
	sParam.Format(_T("userid=%d"), nUserId);
	if( !client.Post(sMethodURL, m_nPort, sParam, sResp) )
		return 0.0;
	// }}

	CMarkup xmlResp;
	if( !xmlResp.SetDoc(sResp) )
		return 0.0;

	if( xmlResp.FindElem(_T("decimal")) ){
		double dBalance = _ttof((xmlResp.GetData()).GetBuffer());
		return dBalance;
		}
	return 0.0;
	}
