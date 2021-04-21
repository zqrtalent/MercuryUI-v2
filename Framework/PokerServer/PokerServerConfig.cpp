#include "stdafx.h"
#include "../Utility/GrowableMemory.h"
#include "../String/StringDecoder.h"
#include "../Log/Logger.h"
#include "PokerServerConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class MultiUserIp
///
///////////////////////////////////////////////////////////

MultiUserIp::MultiUserIp(){
	}

BEGIN_DECLARE_VAR(MultiUserIp, Serializable)
	DECLARE_VAR(_T("MultiUserIp"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Ip"),			_T(""), VariableType::VT_String, offsetof(MultiUserIp,m_sIp), false, false, false)
END_DECLARE_VAR()

MultiUserIp::~MultiUserIp(){
	}

///////////////////////////////////////////////////////////
///
///	class DontRestrictIpUsers
///
///////////////////////////////////////////////////////////

DontRestrictIpUsers::DontRestrictIpUsers(){
	}

BEGIN_DECLARE_VAR(DontRestrictIpUsers, Serializable)
	DECLARE_VAR(_T("DontRestrictIpUsers"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserName"),				_T(""), VariableType::VT_String, offsetof(DontRestrictIpUsers,m_sUserName), false, false, false)
END_DECLARE_VAR()

DontRestrictIpUsers::~DontRestrictIpUsers(){
	}

///////////////////////////////////////////////////////////
///
///	class PokerServerConfig
///
///////////////////////////////////////////////////////////

PokerServerConfig::PokerServerConfig(){
	}

BEGIN_DECLARE_VAR(PokerServerConfig, GameServerConfig)
	DECLARE_VAR(_T("PokerServerConfig"),	_T(""), VariableType::VT_None, 0, false, false, false)
	DECLARE_VAR(_T("SQLConnect"),			_T(""), VariableType::VT_String, offsetof(PokerServerConfig,m_sSQLConnect), false, false, false)
	DECLARE_VAR(_T("WebServiceURL"),		_T(""), VariableType::VT_String, offsetof(PokerServerConfig,m_sWebServiceURL), false, false, false)
	DECLARE_VAR(_T("WebServicePort"),		_T(""), VariableType::VT_Int32, offsetof(PokerServerConfig,m_nWebServicePort), false, false, false)
	DECLARE_VAR(_T("PlayMoneyChips"),		_T(""), VariableType::VT_Int32, offsetof(PokerServerConfig,m_nPlayMoneyChips), false, false, false)
	DECLARE_VAR(_T("MultiUserIps"),			_T(""), VariableType::VT_None, offsetof(PokerServerConfig,m_listMultiUserIps), false, true, true)
	DECLARE_VAR(_T("DontRestrictIpUsers"),	_T(""), VariableType::VT_None, offsetof(PokerServerConfig,m_listUsers), false, true, true)
END_DECLARE_VAR()

PokerServerConfig::~PokerServerConfig(){
	}

BOOL
PokerServerConfig::DeserializeAsXML(_String sXmlFile){
	m_listMultiUserIps		.RemoveAll(true);
	m_listUsers				.RemoveAll(true);

	BOOL bRet = GameServerConfig::DeserializeAsXML(sXmlFile);
	if( bRet ){
		/*
		FileModifiedMethod method = (FileModifiedMethod)&PokerServerConfig::OnFileModified;
		// Start modify monitor.
		m_modifyMonitor.StartMonitor(sXmlFile, method, this);
		*/

		m_lock.Lock(); // Lock
		// Multi user IPs
		m_arrMultiUserIps.DeleteAll();
		for(int i=0; i<m_listMultiUserIps.GetCount(); i++){
			MultiUserIp* pIp = m_listMultiUserIps.GetAt(i);
			if( pIp && pIp->m_sIp.GetLength() > 0 ){
				// [100][1][168][192] = "192.168.1.100"
				ULONG ulIp = inet_addr(pIp->m_sIp.GetBuffer());

				DWORD dw[4];
				dw[3] = (ulIp >> 24) & 0xFF;
				dw[2] = (ulIp >> 16) & 0xFF;
				dw[1] = (ulIp >> 8) & 0xFF;
				dw[0] = ulIp & 0xFF;

				DWORD dwIp = (dw[0] << 24) | (dw[1] << 16) | (dw[2] << 8) | dw[3];
				m_arrMultiUserIps.Add((void*)dwIp);
				}
			}

		m_arrDontRestrictUsers.DeleteAll();
		// Dont restict usernames.
		for(int i=0; i<m_listUsers.GetCount(); i++){
			DontRestrictIpUsers* pUser = m_listUsers.GetAt(i);
			if( pUser && pUser->m_sUserName.GetLength() > 0 ){
				pUser->m_sUserName.MakeUpper();
				m_arrDontRestrictUsers.Add((void*)DecodeString(pUser->m_sUserName.GetBuffer()));
				}
			}
		m_lock.Unlock(); // Unlock
		}
	return bRet;
	}

BOOL
PokerServerConfig::DeserializeAsXML(_String* psXmlSource){
	m_listMultiUserIps		.RemoveAll(true);
	m_listUsers				.RemoveAll(true);

	BOOL bRet = GameServerConfig::DeserializeAsXML(psXmlSource);
	if( bRet ){
		m_lock.Lock(); // Lock
		m_arrMultiUserIps		.DeleteAll();

		// Multi user IPs
		for(int i=0; i<m_listMultiUserIps.GetCount(); i++){
			MultiUserIp* pIp = m_listMultiUserIps.GetAt(i);
			if( pIp && pIp->m_sIp.GetLength() > 0 ){
				// [100][1][168][192] = "192.168.1.100"
				ULONG ulIp = inet_addr(pIp->m_sIp.GetBuffer());

				DWORD dw[4];
				dw[3] = (ulIp >> 24) & 0xFF;
				dw[2] = (ulIp >> 16) & 0xFF;
				dw[1] = (ulIp >> 8) & 0xFF;
				dw[0] = ulIp & 0xFF;

				DWORD dwIp = (dw[0] << 24) | (dw[1] << 16) | (dw[2] << 8) | dw[3];
				m_arrMultiUserIps.Add((void*)dwIp);
				}
			}

		m_arrDontRestrictUsers.DeleteAll();
		// Dont restict usernames.
		for(int i=0; i<m_listUsers.GetCount(); i++){
			DontRestrictIpUsers* pUser = m_listUsers.GetAt(i);
			if( pUser && pUser->m_sUserName.GetLength() > 0 ){
				pUser->m_sUserName.MakeUpper();
				m_arrDontRestrictUsers.Add((void*)DecodeString(pUser->m_sUserName.GetBuffer()));
				}
			}
		m_lock.Unlock(); // Unlock
		}
	return bRet;
	}

bool
PokerServerConfig::IsAllowedMultiUser(DWORD dwIp){
	m_lock.Lock(); // Lock
	bool bRet = (m_arrMultiUserIps.IndexOf((void*)dwIp) != -1);
	m_lock.Unlock(); // Unlock
	return bRet;
	}

bool
PokerServerConfig::IsDontRestrictIPUser(CString& sUserName){
	m_lock.Lock();	// Lock
	sUserName.MakeUpper();
	long	lCode	= (long)DecodeString(sUserName.GetBuffer());
	int		nIndex	= (long)m_arrDontRestrictUsers.IndexOf((void*)lCode);
	bool	bRet	= (nIndex != -1);
	m_lock.Unlock(); // Unlock
	return bRet;
	}



