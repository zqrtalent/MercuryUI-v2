#pragma once
#include "../Serialize/Serializable.h"
#include "../Array/List.h"
#include "../Protocol/PokerPacket.h"
#include "../Thread/WinThread.h"
#include "Object/PokerTableSeatInfo.h"
#include "Object/PokerTableInfo.h"
#include "Object/PokerHandHistory.h"

#include <afxmt.h>

class PokerTableController;
class PokerServer;
class PokerSession;
class PokerGameBase;

class PokerTableController : public Serializable
{
public:
	PokerTableController(PokerServer* pServer = NULL, PokerGameBase* pGameBase = NULL);
	virtual ~PokerTableController();

	friend class PokerCashGames;
	friend class PokerServer;
	friend class PokerData;
	friend class PokerCashData;

	virtual int		GetTableId						(){return 0;};
	virtual int		GetTournamentId					(){return 0;};

	enum AddChipsType
	{
		AddChips_BalanceAmount = 0,
		AddChips_Rebuy,
		AddChips_Addon,
		AddChips_RebuyVPPoints,
		AddChips_AddonVPPoints
	};

protected:
	virtual	void	SendStopSignal					(){};
	virtual void	OnConnectionDropped				(PokerSession* pSess, ClientConnection* pConn, int nSeat);
	virtual void	OnGameSessionExpired			(PokerSession* pSess, ClientConnection* pConn, bool& bCancel);
	virtual bool	OnGameSessionRestored			(PokerSession* pSess, ClientConnection* pConn, int nUserId);

	virtual void	HandStarting					(){};
	virtual void	HandStarted						(){};
	virtual void	HandFinished					(bool& bTerminateGame, int nHandAvgDurationSec){bTerminateGame = m_bCloseTable;};
	virtual	bool	EmptySeat						(int nSeat){return false;};
	virtual void	OnGThread_Terminated			(){ASSERT(FALSE);};

	void			SetParam						(void* pParam){ m_pParam = pParam; };
	void*			GetParam						(){ return m_pParam; };

protected:
	PokerServer*				m_pServer;
	PokerGameBase*				m_pGameBase;		// Game (Cash, Tournament, ...) base class.
	volatile bool				m_bStopHand;		// Stop hand flag.
	volatile bool				m_bCloseTable;		// Close table flag.
	volatile bool				m_bTableIsClosed;	// Table closed status.
	void*						m_pParam;
	
protected: // Serialization.	
	virtual Serializable* CreateObject(){return new PokerTableController();};
	INIT_RUNTIME_VARIABLE()

};
