#pragma once
#include "PokerBot.h"
#include "PokerBotInfo.h"
#include "..\Thread\WinThread.h"

class PokerBotController : public Serializable
{
public:
	PokerBotController();
	virtual ~PokerBotController();

	static	UINT __stdcall	StartBotProc(WinThread* pThread);

	bool	StartBots(std::string sServerIP, int nPort, std::string sBotsInfoXML);
	void	StopBots();

public:
	List<PokerBot>	m_listBots;
	PokerBots		m_botInfos;
};