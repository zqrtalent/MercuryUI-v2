#pragma once
#include "..\_platformCompat\PlatformCompat.h"
#include "..\_platformCompat\PlatformCriticalSection.h"
#include "..\openssl\include\openssl\ssl.h"
#include "..\openssl\include\openssl\err.h"
#include "..\openssl\include\openssl\x509v3.h"

enum SockIOType{
	IOConnect = 1,
	IORecv,
	IOSend,
	IODisconnect
	};

enum SocketStatus{
	SockStatus_None = 0,
	SockStatus_Accepting = 1,
	SockStatus_Connecting = 2,
	SockStatus_Handshaking = 4,
	SockStatus_Connected = 8,
	SockStatus_Receiving = 16,
	SockStatus_Sending = 32,
	SockStatus_Closing = 64,
	SockStatus_Closed = 128,
	SockStatus_Operating	= SockStatus_Accepting | SockStatus_Connecting | SockStatus_Handshaking | SockStatus_Receiving | SockStatus_Sending
	};

class SocketServer;
class SocketContext;

struct ConnectedSocketInfo{
	SocketContext*		m_pSockConn;
	SocketServer*		m_pServer;
	SOCKET				m_sock;
	time_t				m_dtTimeConnected;
	std::string			m_sLocalIp;
	int					m_nLocalPort;
	std::string			m_sRemoteIp;
	DWORD				m_dwRemoteIp;
	int					m_nRemotePort;
	DWORD				m_dwBytesSent;
	DWORD				m_dwBytesReceived;
	};

class SocketContextBase{
	public:
		SocketContextBase(){};
		virtual ~SocketContextBase(){};

	public:
		virtual	bool	OnAccept			(){return false;};
		virtual	void	OnConnect			(){};
		virtual void	OnDisconnect		(DWORD dwLastError){};
		virtual void	OnDataReceived		(DWORD dwSize){};
		virtual void	OnDataSent			(DWORD dwSize){};
	};

class SocketIOEventHandler{
	public:
		SocketIOEventHandler(){};
		virtual ~SocketIOEventHandler(){};

	public:
		virtual void	OnAcceptConnection	(SOCKET sock, const std::string sRemoteIp, int nRemotePort, bool& bAccept){};
		virtual void	OnConnected			(SOCKET sock, ConnectedSocketInfo* pSockInfo){};
		virtual void	OnDataReceived		(SocketContext* pConn, BYTE* pData, DWORD dwSize, bool bIsFirstData){};
		virtual void	OnDisconnected		(SocketContext* pConn, DWORD dwLastError){};
	};

class OVERLAPPEDEX{
	public:
		OVERLAPPED	m_ol;
		SockIOType	m_ioType;
		DWORD		m_dwLastError;
		bool		m_bConnecting;

		OVERLAPPEDEX(SockIOType ioType){
			memset(&m_ol, 0, sizeof(OVERLAPPEDEX));
			m_ioType		= ioType;
			m_dwLastError	= 0;
			m_bConnecting	= true;
			}
	};

#ifndef ssl_lock
typedef CRITICAL_SECTION	ssl_lock;
#endif

#ifndef CRYPTO_dynlock_value
struct CRYPTO_dynlock_value {
	ssl_lock lock;
};
#endif

static bool ssl_is_fatal_error(int ssl_error){
	switch(ssl_error){
		case SSL_ERROR_NONE:
		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_WRITE:
		case SSL_ERROR_WANT_CONNECT:
		case SSL_ERROR_WANT_ACCEPT:
			return false;
		}
	return true;
	}

#ifdef _SERVER
#include "..\Log\Logger.h"
#endif

static int ssl_get_error(SSL *ssl, int result, bool& bIsFatalError){
	int error = SSL_get_error(ssl, result);
	if( error != SSL_ERROR_NONE ){
		char message[512] = {0};
		int error_log = error;
		bIsFatalError = false;
		while( error_log != SSL_ERROR_NONE ){
			ERR_error_string_n(error_log, message, _countof(message));
			if( ssl_is_fatal_error(error_log) ){
				if( !bIsFatalError )
					bIsFatalError = true;
				// print error message to console or logs
#ifdef _SERVER
				Logger::CreateLogEntry(message);
#else
				//AfxMessageBox(message);
#endif
				}
			error_log = ERR_get_error();
			}
		}
	return error;
	}