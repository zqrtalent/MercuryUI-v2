#pragma once
#include "..\Serialize\Serializable.h"
#include "..\GameServer\GameServerConfig.h"

#include "..\Protocol\BasePacket.h"

class MultiUserIp : public Serializable
{
public:
	MultiUserIp();
	virtual ~MultiUserIp();

	_String		m_sIp;

protected:
	virtual Serializable* CreateSerializableObject(){return new MultiUserIp();};
	INIT_RUNTIME_VARIABLE()
};

class DontRestrictIpUsers : public Serializable
{
public:
	DontRestrictIpUsers();
	virtual ~DontRestrictIpUsers();

	_String		m_sUserName;

protected:
	virtual Serializable* CreateSerializableObject(){return new DontRestrictIpUsers();};
	INIT_RUNTIME_VARIABLE()
};

class PokerServerConfig : public GameServerConfig
{
public:
	PokerServerConfig();
	virtual ~PokerServerConfig();

public:
	_String									m_sSQLConnect;				// Database connection string
	_String									m_sWebServiceURL;			// Web service URL.
	int										m_nWebServicePort;			// Web service port.
	int										m_nPlayMoneyChips;			// Playmoney starting chips amount.

	EnumerableObject<MultiUserIp>			m_listMultiUserIps;			// Multi user Ips.
	EnumerableObject<DontRestrictIpUsers>	m_listUsers;				// Don't restrict IP  users.

protected:
	AutoSortedArray							m_arrMultiUserIps;		// IP's that is allowed to play on the same table.
	AutoSortedArray							m_arrDontRestrictUsers;	// Dont restrict IP users.

public:
	bool			IsAllowedMultiUser		(DWORD dwIp);
	bool			IsDontRestrictIPUser	(CString& sUserName);

public:
	virtual BOOL	DeserializeAsXML		(_String sXmlFile);
	virtual BOOL	DeserializeAsXML		(_String* psXmlSource);

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerServerConfig();};
	INIT_RUNTIME_VARIABLE()
};

