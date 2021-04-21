#include "stdafx.h"
#include "..\Utility\GrowableMemory.h"
#include "..\Thread\WinThread.h"
#include "PokerBotController.h"

///////////////////////////////////////////////////////////
///
///	class PokerBotController
///
///////////////////////////////////////////////////////////

PokerBotController::PokerBotController(){
	}


PokerBotController::~PokerBotController(){
	StopBots();
	}

UINT __stdcall
PokerBotController::StartBotProc(WinThread* pThread){
	PokerBot*		pBot		= (PokerBot*)pThread->GetThreadParameter();
	PokerBotInfo*	pBotInfo	= (PokerBotInfo*)pThread->GetParam(_T("INFO"));
	if( pBot && pBot->StartBot(pBotInfo) ){

		/*
		SOCKET sock = pBot->GetSock();

		int nLoop = 0;
		while( TRUE ){

			// In every 5 seconds.
			if( nLoop%10 == 0 ){
				DWORD			dwValue = 0;
				int				nOptLen	= sizeof(DWORD);
				if( getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&dwValue, &nOptLen) == SOCKET_ERROR ){
					break;
					}
				pBot->TransferMoney(AccountType::MainBalance, AccountType::PokerBalance, 2, false);
				//pBot->GetLobbyInfo(MoneyType::Real, GameType::TexasHoldem, GameSubType::Holdem_NoLimit);
				}

			Sleep(100);

			DWORD			dwValue = 0;
			int				nOptLen	= sizeof(DWORD);
			if( getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&dwValue, &nOptLen) == SOCKET_ERROR ){
				break;
				}
			nLoop ++;
			}*/
		}
	delete pThread;
	return 0;
	}

bool
PokerBotController::StartBots(_String sServerIP, int nPort, _String sBotsInfoXML){
	if( m_botInfos.DeserializeAsXML(sBotsInfoXML) ){
		int nLoop	= 0;
		int nCt		= min(m_botInfos.m_listBots.GetCount(), 100);

		while( nLoop < nCt ){
			PokerBotInfo* pBotInfo = m_botInfos.m_listBots[nLoop];
			if( pBotInfo ){
				PokerBot* pBot = new PokerBot(NULL, sServerIP, nPort);
				WinThread* pThread = new WinThread();
				pThread->AddParam(_T("INFO"), pBotInfo);
				pThread->StartThread((LPTHREAD_START_ROUTINE)&PokerBotController::StartBotProc, pBot);

				m_listBots.Add(pBot);

				/*
				if( pBot->StartBot(pBotInfo) ){
					m_listBots.Add(pBot);
					}
				else
					delete pBot;
					*/
				}
			nLoop ++;
			}
		return true;
		}

	return false;

	/*
	PokerBotInfo* pBotInfo = new PokerBotInfo();
	pBotInfo->m_sUserName		= _T("zack1");
	pBotInfo->m_sPassword		= _T("zack");
	pBotInfo->m_nGameTypeId		= TexasHoldem;
	pBotInfo->m_nSubGameTypeId	= Holdem_NoLimit;
	pBotInfo->m_nPlayTablesCt	= 3;
	bots.m_listBots.Add(pBotInfo);

	 pBotInfo = new PokerBotInfo();
	pBotInfo->m_sUserName		= _T("zack2");
	pBotInfo->m_sPassword		= _T("zack");
	pBotInfo->m_nGameTypeId		= TexasHoldem;
	pBotInfo->m_nSubGameTypeId	= Holdem_NoLimit;
	pBotInfo->m_nPlayTablesCt	= 3;
	bots.m_listBots.Add(pBotInfo);

	bots.SerializeAsXML(_T("c:\\bots.xml"), TRUE);*/
	}

bool
PokerBotController::StartBots2(_String sServerIP, int nPort, List<PokerBotInfo>* pListBots){
	int nLoop = 0, nCt = pListBots->GetCount();
	while( nLoop < nCt ){
		PokerBotInfo* pBotInfo = pListBots->GetAt(nLoop);
		if( pBotInfo ){
			PokerBot* pBot = new PokerBot(NULL, sServerIP, nPort);
			WinThread* pThread = new WinThread();
			pThread->AddParam(_T("INFO"), pBotInfo);
			pThread->StartThread((LPTHREAD_START_ROUTINE)&PokerBotController::StartBotProc, pBot);
			m_listBots.Add(pBot);

			Sleep(500);
			}
		nLoop ++;
		}
	return true;
	}

void
PokerBotController::StopBots(){
	m_listBots.RemoveAll(true);
	}
