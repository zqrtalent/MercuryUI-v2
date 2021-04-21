#include "stdafx.h"
#include "SocketServerOptions.h"

// SocketServerOptions

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SocketServerOptions.
//
/////////////////////////////////////////////////////////////////////

SocketServerOptions::SocketServerOptions(){
	} 

BEGIN_DECLARE_VAR(SocketServerOptions, Serializable)
	DECLARE_VAR(_T("SocketServerOptions"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ListenerInfo"),			_T(""), VariableType::VT_None, offsetof(SocketServerOptions,m_listenOpt), false, true, false)
END_DECLARE_VAR()

SocketServerOptions::~SocketServerOptions(){
	}

/////////////////////////////////////////////////////////////////////
//
//	SockListenOptions.
//
/////////////////////////////////////////////////////////////////////

SockListenOptions::SockListenOptions(){
	} 

BEGIN_DECLARE_VAR(SockListenOptions, Serializable)
	DECLARE_VAR(_T("SockListenOptions"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Name"),					_T(""), VariableType::VT_String, offsetof(SockListenOptions,m_sName), false, false, false)
	DECLARE_VAR(_T("IpListen"),				_T(""), VariableType::VT_String, offsetof(SockListenOptions,m_sIpListen), false, false, false)
	DECLARE_VAR(_T("Port"),					_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nPort), false, false, false)
	DECLARE_VAR(_T("ListenBackLog"),		_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nListenBackLog), false, false, false)
	DECLARE_VAR(_T("MaxReceiveDataSize"),	_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nMaxReceiveDataSize), false, false, false)
	DECLARE_VAR(_T("MaxConnection"),		_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nMaxConnection), false, false, false)
	DECLARE_VAR(_T("ReceiveBufferSize"),	_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nReceiveBufferSize), false, false, false)
	DECLARE_VAR(_T("SendBufferSize"),		_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nSendBufferSize), false, false, false)
	DECLARE_VAR(_T("UseSSL"),				_T(""), VariableType::VT_Bool, offsetof(SockListenOptions,m_bUseSSL), false, false, false)
	DECLARE_VAR(_T("SSLPemFile"),			_T(""), VariableType::VT_String, offsetof(SockListenOptions,m_sSSLPemFile), false, false, false)
	DECLARE_VAR(_T("KeepAlive"),			_T(""), VariableType::VT_Bool, offsetof(SockListenOptions,m_bKeepAlive), false, false, false)
	DECLARE_VAR(_T("KeepAliveTime"),		_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nKeepAliveTime), false, false, false)
	DECLARE_VAR(_T("KeepAliveInterval"),	_T(""), VariableType::VT_Int32, offsetof(SockListenOptions,m_nKeepAliveInterval), false, false, false)
	DECLARE_VAR(_T("IpsBanned"),			_T(""), VariableType::VT_None, offsetof(SockListenOptions,m_listIpsBanned), false, true, true)
END_DECLARE_VAR()

SockListenOptions::~SockListenOptions(){
	}

/////////////////////////////////////////////////////////////////////
//
//	IpBanned.
//
/////////////////////////////////////////////////////////////////////

IpBanned::IpBanned(){
	} 

BEGIN_DECLARE_VAR(IpBanned, Serializable)
	DECLARE_VAR(_T("IpBanned"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Ip"),			_T(""), VariableType::VT_String, offsetof(IpBanned,m_sIp), false, false, false)
END_DECLARE_VAR()

IpBanned::~IpBanned(){
	}
