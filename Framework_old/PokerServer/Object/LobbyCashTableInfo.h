#pragma once
#include "..\..\Serialize\Serializable.h"
#include "..\..\Protocol\_PokerDefines.h"
#include "..\..\Array\List.h"

class LobbyCashTablePlayerInfo : public Serializable
{
public:
	LobbyCashTablePlayerInfo();
	~LobbyCashTablePlayerInfo();

	friend class LobbyTableInfo;

public:
	int				m_nUserId;			// User Id.
	std::string			m_sUserName;		// User name.
	int				m_nSeatAmount;		// Seat amount.
	char			m_cVIPLevel;		// VIP level
	char			m_cSeatIndex;		// Seat index or -1 if in waiting list.

protected:
	virtual Serializable* CreateSerializableObject(){return new LobbyCashTablePlayerInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class LobbyCashTableStaticInfo : public Serializable
{
public:
	LobbyCashTableStaticInfo();
	~LobbyCashTableStaticInfo();

public:
	int							m_nTableId;
	std::string						m_sTableName;
	BYTE						m_cGameType;
	short						m_shGameSubType;
	BYTE						m_btCurrencyId;
	int							m_nSBAmount;
	int							m_nBBAmount;
	int							m_nAnteAmount;
	char						m_cMaxPlayerCt;

	int							m_nMinBuyIn;
	int							m_nMaxBuyIn;

#ifndef _SERVER
	// Dynamic info just for caching.
	char						m_cPlayerCt;
	char						m_cPlayerFlopPercent;
	char						m_cWaitedPlayersCt;
	int							m_nAvgPotAmount;
	short						m_shHandPerHour;
#endif

protected:
	virtual Serializable* CreateSerializableObject(){return new LobbyCashTableStaticInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class LobbyCashTableDynamicInfo : public Serializable
{
public:
	LobbyCashTableDynamicInfo();
	~LobbyCashTableDynamicInfo();

	void						AddPlayerInfo			(LobbyCashTablePlayerInfo* pInfo, bool bCopyObject = false);
	int							GetPlayerCt				(){return m_listPlayers.GetCount();};
	bool						RemovePlayerInfoBySeat	(int nSeat);
	bool						RemovePlayerInfoById	(int nUserId);
	LobbyCashTablePlayerInfo*	GetPlayerInfo			(int nUserId);
	LobbyCashTablePlayerInfo*	GetPlayerInfoBySeat		(int nSeat);

public:
	int								m_nTableId;
	char							m_cPlayerCt;
	char							m_cPlayerFlopPercent;
	char							m_cWaitedPlayersCt;
	int								m_nAvgPotAmount;
	short							m_shHandPerHour;

	List<LobbyCashTablePlayerInfo>	m_listPlayers;

protected:
	virtual Serializable* CreateSerializableObject(){return new LobbyCashTableDynamicInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};