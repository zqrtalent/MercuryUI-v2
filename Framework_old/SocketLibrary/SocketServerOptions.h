#pragma once
#include "..\Serialize\Serializable.h"

class IpBanned : public Serializable
{
public:
	IpBanned();
	virtual ~IpBanned();

public:
	std::string		m_sIp;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new IpBanned();};
	INIT_RUNTIME_VARIABLE()
};

class SockListenOptions : public Serializable
{
public:
	SockListenOptions			();
	virtual ~SockListenOptions	();

public:
	std::string					m_sName;
	std::string					m_sIpListen;
	int							m_nPort;
	int							m_nListenBackLog;
	int							m_nMaxReceiveDataSize;
	int							m_nMaxConnection;
	int							m_nReceiveBufferSize;
	int							m_nSendBufferSize;
	bool						m_bKeepAlive;
	int							m_nKeepAliveTime;
	int							m_nKeepAliveInterval;
	EnumerableObject<IpBanned>	m_listIpsBanned;
	bool						m_bUseSSL;					// SSL usage.
	std::string					m_sSSLPemFile;				// SSL Pem file.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new SockListenOptions();};
	INIT_RUNTIME_VARIABLE()
};

class SocketServerOptions : public Serializable
{
public:
	SocketServerOptions			();
	virtual ~SocketServerOptions();

public:
	SockListenOptions m_listenOpt;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new SocketServerOptions();};
	INIT_RUNTIME_VARIABLE()
};

