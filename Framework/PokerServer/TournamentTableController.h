#pragma once
#include "PokerCashTableController.h"
#include "Data/Tables/TournamentInfo.h"

class PokerTournamentManager;
class TournamentTableController : public PokerCashTableController
{
public:
	TournamentTableController(PokerServer* pServer = NULL, PokerGameBase* pGameBase = NULL, PokerTournamentManager* pMan = NULL);
	virtual ~TournamentTableController();

	friend class PokerServer;
	friend class PokerTournamentData;
	friend class PokerTournamentManager;
	friend class TournamentTablesManager;
	friend PokerCashTableController;

protected:
	// Packet handle virtual methods.
	virtual void			OnOpenTable							(PokerSession* pSess, ClientConnection* pConn, PokerOpenTablePacket* p);
	virtual void			OnLeaveTable						(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTablePacket* p);
	virtual void			OnOpenTable_Tournament				(PokerSession* pSess, ClientConnection* pConn, PokerOpenTable_TournamentPacket* p);
	virtual void			OnLeaveTable_Tournament				(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTable_TournamentPacket* p);

	virtual void			OnSeatAt							(PokerSession* pSess, ClientConnection* pConn, PokerSeatAtTablePacket* p);
	virtual void			OnReserveSeat						(PokerSession* pSess, ClientConnection* pConn, PokerReserveSeatPacket* p);
	virtual void			OnCancelReservation					(PokerSession* pSess, ClientConnection* pConn, PokerCancelReservationPacket* p);
	virtual void			OnJoinWaitingList					(PokerSession* pSess, ClientConnection* pConn, PokerJoinWaitingListPacket* p);
	virtual void			OnAction							(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p, bool bPreAction = false);
	virtual void			OnTableInfo							(PokerSession* pSess, ClientConnection* pConn, PokerTableInfoPacket* p);
	virtual void			OnReturnBack						(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p);
	virtual void			OnShowMuckCards						(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p);
	virtual void			OnSetOption							(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p);
	
	virtual void			SendStopSignal						();
	virtual void			OnConnectionDropped					(PokerSession* pSess, ClientConnection* pConn, int nSeat);
	virtual void			OnGameSessionExpired				(PokerSession* pSess, ClientConnection* pConn, bool& bCancel);
	virtual bool			OnGameSessionRestored				(PokerSession* pSess, ClientConnection* pConn, int nUserId);

	virtual void			HandStarting						();
	virtual void			HandStarted							();
	virtual void			HandFinished						(bool& bTerminateGame, int nHandAvgDurationSec);
	virtual	bool			EmptySeat							(int nSeat, bool bSendCallbacks = true);
	virtual void			OnGThread_Starting					();
	virtual void			OnGThread_Finishing					(bool& bTurnInPauseMode);
	virtual void			OnGThread_Terminated				();

	PokerSession*			GetSeatSession						(int nSeat, bool bRetain);
	void					SetBlindStakes						(TournamentBlindStructure* pCurrLevelStakes, TournamentBlindStructure* pNextLevelStakes);
	INT_CHIPS				GetAverageBalanceAmount				();
	int						GetSeatsAvailability				(bool* pSeats, INT_CHIPS* nArrBalances);
	//int						GetEmptySeatCt						();

	bool					SeatFromAnotherTable				(int nSeatDest, TournamentTableController* pTableSrc, int nSeatSrc);
	bool					SeatSessionAtTable					(PokerTableSeatInfo* pSeat, PokerSession* pSession,  int nSeatDest);
	int						EmptyAllSeats						();

	int						PerformAutoRebuySeats				();
	int						PerformAutoAddonSeats				();

	// Send open tournament table event to seats (nCertainSeat (-1) means to send to all seats.).
	int						SendOpenTournamentTableToSeats		(int nTargetSeat = -1, UINT nSB_Next = 0, UINT nBB_Next = 0, UINT nAnte_Next = 0, int nStartTournamentTimeSec = 0);
	// Change sync event wait milliseconds.
	void					SetSyncEventWaitMSec				(int nMillSec){m_nSyncStartWaitMS = nMillSec;}

public:
	int						GetEmptySeatCt						();
	int						GetTournamentPlayersCount			();
	int						GetTournamentTablesCount			();
	bool					IsRebuyAllowed						();
	bool					IsAddonAllowed						();
	PokerTournamentManager*	GetTournamentManager				() {return m_pTournamentMan;};
	bool					IsTournamentCanceled				() {return m_bCanceled;};

protected:
	PokerTournamentManager*	m_pTournamentMan;				// Tournament manager.
	// See: PokerTournamentManager::GetSyncGameStartEvent()
	// When synchronous start event signaled we need some time to sleep each game thread.
	int						m_nSyncStartWaitMS;				// Wait milliseconds after sync start event signals.	

	int						m_nTimeBankIncLevel;			// Time bank increase level count.

	volatile bool			m_bChangeStakes;				// Changes stakes on FinishHand
	volatile UINT			m_nSBAmountNew;					// Smallblind new amount.
	volatile UINT			m_nBBAmountNew;					// Bigblind new amount.
	volatile UINT			m_nAnteAmountNew;				// Ante new amount.
	volatile UINT			m_nSBAmountNext;				// Smallblind amount of next level.
	volatile UINT			m_nBBAmountNext;				// Bigblind amount of next level.
	volatile UINT			m_nAnteAmountNext;				// Ante amount of next level.

	volatile bool			m_bCanceled;					// Tournament canceled.

protected: // Serialization.	
	virtual Serializable* CreateObject(){return new TournamentTableController();};
	INIT_RUNTIME_VARIABLE()
};
