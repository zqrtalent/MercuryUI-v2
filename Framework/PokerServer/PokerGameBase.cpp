#include "stdafx.h"
#include "../Protocol/PokerPacket.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"

#include "Log/Logger.h"
#include "PokerServer.h"
#include "PokerGameBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerGameBase
///
///////////////////////////////////////////////////////////

PokerGameBase::PokerGameBase(){
	m_pServer			= NULL;
	m_pPacketHandler	= NULL;
	m_pLobbyInfo		= NULL;
	m_bInited			= false;
	m_arrActiveGameById.OptimizeForLargeData(1);
	} 

PokerGameBase::~PokerGameBase(){
	}

PokerTableController*
PokerGameBase::GetTableController(int nTableId){
	m_lockGames.Lock(); // Lock
	int nIndex = m_arrActiveGameById.IndexOf((void*)nTableId);
	if( nIndex == -1 ){
		m_lockGames.Unlock(); // Unlock
		return NULL;
		}
	PokerTableController* pController = (PokerTableController*)m_arrActiveGameById.GetData(nIndex);
	m_lockGames.Unlock(); // Unlock
	return pController;
	}