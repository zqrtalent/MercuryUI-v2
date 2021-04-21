#pragma once
#include "..\Serialize\Serializable.h"

class PokerBotInfo : public Serializable
{
public:
	PokerBotInfo();
	virtual ~PokerBotInfo();

public:
	std::string		m_sUserName;
	std::string		m_sPassword;
	int			m_nGameTypeId;
	int			m_nSubGameTypeId;
	int			m_nPlayTablesCt;
protected:
	virtual Serializable* CreateSerializableObject(){return new PokerBotInfo();};
	INIT_RUNTIME_VARIABLE()
};

class PokerBots : public Serializable
{
public:
	PokerBots();
	virtual ~PokerBots();

public:
	EnumerableObject<PokerBotInfo> m_listBots;

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerBots();};
	INIT_RUNTIME_VARIABLE()
};
