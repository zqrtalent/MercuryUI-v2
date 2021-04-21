#include "stdafx.h"
#include "PokerServer.h"
#include "PokerGameBase.h"
#include "PokerTableController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerTableController
///
///////////////////////////////////////////////////////////

PokerTableController::PokerTableController(PokerServer* pServer /*= NULL*/, PokerGameBase* pGameBase /*= NULL*/){
	if( !pServer ){ m_pServer = NULL; return; }
	m_bStopHand			= false;
	m_bCloseTable		= false;
	m_bTableIsClosed	= false;
	m_pServer			= pServer;
	m_pGameBase			= pGameBase;
	m_pParam			= NULL;
	} 

BEGIN_DECLARE_VAR(PokerTableController, Serializable)
	DECLARE_VAR(_T("PokerTableController"),	 _T(""), VariableType::VT_None, 0, false, true, false)
END_DECLARE_VAR()

PokerTableController::~PokerTableController(){
	m_pServer		= NULL;
	m_pGameBase		= NULL;
	}

void
PokerTableController::OnConnectionDropped(PokerSession* pSess, ClientConnection* pConn, int nSeat){
	}

void
PokerTableController::OnGameSessionExpired(PokerSession* pSess, ClientConnection* pConn, bool& bCancel){
	}

bool
PokerTableController::OnGameSessionRestored(PokerSession* pSess, ClientConnection* pConn, int nUserId){
	return false;
	}
