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

	bool	StartBots(_String sServerIP, int nPort, _String sBotsInfoXML);
	bool	StartBots2(_String sServerIP, int nPort, List<PokerBotInfo>* pListBots);
	void	StopBots();

public:
	List<PokerBot>	m_listBots;
	PokerBots		m_botInfos;
};