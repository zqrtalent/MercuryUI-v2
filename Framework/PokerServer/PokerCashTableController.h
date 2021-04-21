#pragma once
#include "../Serialize/Serializable.h"
#include "../Array/List.h"
#include "../Protocol/PokerPacket.h"
#include "../Thread/WinThread.h"
#include "Object/PokerTableSeatInfo.h"
#include "Object/PokerTableInfo.h"
#include "Object/PokerHandHistory.h"
#include "PokerTableController.h"

class PokerCashTableController;
class PokerServer;
class PokerSession;
class PokerGameBase;

typedef bool (__thiscall PokerCashTableController::*OnTimerElapsed)();

class PokerCashTableController : public PokerTableController
{
public:
	PokerCashTableController(PokerServer* pServer = NULL, PokerGameBase* pGameBase = NULL);
	virtual ~PokerCashTableController();

	friend class PokerCashGames;
	friend class PokerServer;
	friend class PokerData;
	friend class PokerCashData;

	virtual int		GetTableId						(){return m_info.m_nTableId;};
	virtual int		GetTournamentId					(){return m_info.m_nTournamentId;}
	bool			GetSeatInfo						(int nSeat, PokerTableSeatInfo* pSeatInfo);
	bool			SetSeatInfo						(int nSeat, PokerTableSeatInfo* pSeatInfo);
	void			GetSeatBalanceStat				(INT_CHIPS& nBalanceSmallest, INT_CHIPS& nBalanceLargest);
	INT_CHIPS		GetSeatBalance					(int nSeat);
	void			GetTableInfo					(PokerTableInfo* pTableInfo){
						ASSERT(pTableInfo); 
						m_lockObjects.Lock();	// Lock 
						m_info.Copy(pTableInfo); 
						m_lockObjects.Unlock(); // Unlock 
					}

	bool			CompareTypes					(int nGameType, int nGameSubType){ return (m_info.m_cGameTypeId == (BYTE)nGameType && m_info.m_shSubTypeId); };
	bool			IsGameInProgress				(){ return (m_info.m_gameStatus == PokerGameStatus::Finished || m_info.m_gameStatus == PokerGameStatus::NotStarted); };

protected:
	static void	__stdcall	OnTimerEvent					(int nIdEvent, void* lpParam);
	void					OnTimerEventElapsed				(int nIdEvent);
	void					OnReservationTimerElapsed		(int nSeat);
	void					OnLeaveSeatTimerElapsed			(int nSeat);
	bool					StartSeatReservationTimer		(int nSeat);
	bool					StopSeatReservationTimer		(int nSeat);
	bool					StartLeaveSeatTimer				(int nSeat, bool bSeatIsZeroBalanced = false);
	bool					StopLeaveSeatTimer				(int nSeat);
	void					KillAllTimers					();
	bool					IsSeatReservedByConn			(int nSeat, ClientConnection* pConn);
	bool					CancelSeatReservation			(int nSeat);

protected:
	virtual void			OnOpenTable						(PokerSession* pSess, ClientConnection* pConn, PokerOpenTablePacket* p);
	virtual void			OnLeaveTable					(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTablePacket* p);

	virtual void			OnOpenTable_Tournament			(PokerSession* pSess, ClientConnection* pConn, PokerOpenTable_TournamentPacket* p){};
	virtual void			OnLeaveTable_Tournament			(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTable_TournamentPacket* p){};

	virtual void			OnSeatAt						(PokerSession* pSess, ClientConnection* pConn, PokerSeatAtTablePacket* p);
	virtual void			OnReserveSeat					(PokerSession* pSess, ClientConnection* pConn, PokerReserveSeatPacket* p);
	virtual void			OnCancelReservation				(PokerSession* pSess, ClientConnection* pConn, PokerCancelReservationPacket* p);
	virtual void			OnJoinWaitingList				(PokerSession* pSess, ClientConnection* pConn, PokerJoinWaitingListPacket* p);
	virtual void			OnAddChips						(PokerSession* pSess, ClientConnection* pConn, PokerAddChipsPacket* p);
	virtual void			OnAction						(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p, bool bPreAction = false);
	virtual void			OnTableInfo						(PokerSession* pSess, ClientConnection* pConn, PokerTableInfoPacket* p);
	virtual void			OnReturnBack					(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p);
	virtual void			OnSitOut						(PokerSession* pSess, ClientConnection* pConn, PokerSitOutPacket* p){};
	virtual void			OnShowMuckCards					(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p);
	virtual void			OnSetOption						(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p);
	virtual void			OnChatMessage					(PokerSession* pSess, ClientConnection* pConn, PokerChatMessagePacket* p);
	
	virtual	void			SendStopSignal					();
	virtual void			OnConnectionDropped				(PokerSession* pSess, ClientConnection* pConn, int nSeat);
	virtual void			OnGameSessionExpired			(PokerSession* pSess, ClientConnection* pConn, bool& bCancel);
	virtual bool			OnGameSessionRestored			(PokerSession* pSess, ClientConnection* pConn, int nUserId);

	// Table game thread methods. {{
	bool					GThread_IsStarted				(){ return (!m_gameThread.IsThreadStopped()); }
	void					GThread_Start					(bool bSuspended = false);
	void					GThread_Stop					();
	static LRESULT __stdcall GThread_Entry					(WinThread* pGameThread);
	bool					GThread_WaitForResponse			(ClientConnection* pConn, BasePacket* pSent, int nSecWait, 
															BasePacket* pPacketReceived, bool& bElapsed, int* pMSecWasted);
	bool					GThread_Sleep					(int nMSecSleep, int nMSecSingleSleep = 250);
	bool					GThread_StartHand				();
	bool					GThread_FinishHand				(bool& bTerminate, int& nPotCt);
	bool					GThread_FinishRound				(bool& bHandFinished);
	bool					GThread_AskToPostBlind			(int nSeat, bool bSmallBlind, ClientConnection* pConn, PokerActionPacket* pPacketRecv, bool& bSuccess, bool bCheckForAutoPostFlag = true, bool bAllowedToFine = false);
	bool					GThread_AskForAction			(int nSeatPrev, int& nSeatAsked, bool& bRoundEnded, bool& bOpenCards);
	bool					GThread_ActionResponseRecieved	(PokerActionPacket* p, /*Received action packet*/
															int nSecElapsed /*Seconds elapsed while waiting for action from client*/, 
															PokerSeatStatus statusSet = PokerSeatStatus::Unavailable);
	bool					GThread_ActTimerElapsed			(PokerActPacket* pSent /*Act packet sent to client*/, 
															ClientConnection* pConn, int& nSeatFrom, int nElapsedTimeSec);
	bool					GThread_Pause					(){return m_gameThread.PauseThread();};
	bool					GThread_IsPaused				(){return m_gameThread.IsPauseEventSignaled();};
	bool					GThread_Resume					(){ if( !GThread_IsStarted() ){GThread_Start(false);} return m_gameThread.ContinueThread();};
	virtual void			OnGThread_Starting				(){};
	virtual void			OnGThread_Finishing				(bool& bTurnInPauseMode){bTurnInPauseMode = true;};
	virtual void			OnGThread_Terminated			();
	// }}
	
	int						PerformAddChipsAction			();// Perform all addchips actions.
	// Returns new balance amount. returns zero when fail.
	INT_CHIPS				AddChips						(int nSeat, INT_CHIPS nAmount, AddChipsType chipsType = AddChipsType::AddChips_BalanceAmount);// Add chips.

	bool					DealCardsAndStartHand			(int nDealer, int nSBSeat, int nBBSeat, int nSeatRefusedToPaySB, CDWordArray& arrPayBBSeats, int nTournamentPlayersCt = 0, int nTournamentTablesCt = 0);
	void					ClearSeatCards					();
	bool					SendSeatStatusChange			(int nSeat, PokerSeatStatus status);
	bool					SendSetActiveSeat				(int nActiveSeat, int nRegularTimeSec, int nExtraTimeSec = 0, int nTimeBankSec = 0);
	bool					SendPostBlindRequest			(int nSeat, bool bSmallBlind, bool& bResult, bool bCheckForAutoFlag = true, bool bAllowedToFine = false);
	bool					SendShowMuckCardsRequest		(int nSeat, bool& bResult);
	bool					SendSeatActionPacket			(int nSeat, PokerActionTypes action, PokerSeatStatus status, INT_CHIPS nAmount, bool bSetSitOutNextHand = false, int nActionWaitTimeSec = 0/*Seconds passed until response received.*/);
	bool					SitOutBySeat					(int nSeat, bool bSendAddChips = false, bool bMissedSB = false, bool bMisedBB = false);
	bool					SitOut							(int nSeat, ClientConnection* pConn);
	bool					SetSeatOffline					(int nSeat, bool bOnline = false, PokerSession* pSess = NULL);
	virtual	bool			EmptySeat						(int nSeat, bool bSendCallbacks = true);
	int						EmptyOfflineSeats				();
	bool					ForceSeatToLeaveTable			(int nSeat);
	bool					GetEmptySeat					(int& nSeat, int nStartFrom = 0);
	int						GetSeatIndexByUserId			(int nUserId, INT_CHIPS* pUserBalance = NULL);

	// Wait list.
	void					SeatWaitListUsers				(AutoSortedArray& arrSeated);
	int						AddInWaitList					(ClientConnection* pConn);
	bool					RemoveFromWaitList				(ClientConnection* pConn);
	bool					IsInWaitList					(ClientConnection* pConn);
	int						GetWaitListUsersCt				();

	// Close table.
	bool					CloseTable						();

protected:
	PokerTableInfo				m_info;
	List<ClientConnection>		m_listWaitingList;				// Waiting list connections.
	AutoSortedArrayInt64		m_arrWaitingListConn;			// Array of waiting list connections.
	AutoSortedArrayInt64		m_arrAddChips;					// Add chips actions list. < byte add chips type constant | byte seat index><add chips amount>
	CCriticalSection			m_lockObjects;

	volatile	bool			m_bSyncState;					// Game thread is in sync state.
	WinThread					m_gameThread;

	int							m_nDealer;
	int							m_nSmallBlind;
	int							m_nBigBlind;
	__int64						m_nHandId;						// Current hand id.
	UINT						m_nSBAmount;					// Small blind amount.
	UINT						m_nBBAmount;					// Big blind amount.
	UINT						m_nAnteAmount;					// Ante amount.
	UINT						m_nMinBuyIn;					// Min buy in amount.
	UINT						m_nMaxBuyIn;					// Max buy in amount.
	int							m_nCurrencyId;					// Table currency id.
	
	GameType					m_gametype;						// Game type definition.
	GameSubType					m_subType;						// Game sub type enum.
	MoneyType					m_moneyType;					// Money type definition.

	int							m_arrSeatReserveTimerIds[10];	// Seat reservation timer ids.
	int							m_arrLeaveSeatTimerIds[10];		// Leave seat timer ids.

	int							m_nHandStartDelay;				// Hand start seconds delay.
	int							m_nPostBlindDelay;				// Post blind action delay.
	int							m_nStartRoundDelay;				// Start round delay.
	int							m_nShowMuckDelay;				// Show/Muck cards delay.
	int							m_nReserveSeatDelay;			// Reserve seat delay.
	int							m_nLeaveSitoutedDelay;			// Leave sitouted seat delay.
	int							m_nLeaveZeroBalancedSeatDelay;	// Leave zero balanced seat delay.

	int							m_nRegularTimeSec;				// Regular thinking time seconds.
	int							m_nExtraTimeSec;				// Extra thinking time seconds.
	int							m_nTimeBankSec;					// Time bank seconds.
	int							m_nTimeBankInc;					// Time bank increase seconds.
	int							m_nTimeBankIncHands;			// Time bank increase hand count.

	PokerHandHistory			m_handHistory;					// Hand history.
	
protected: // Serialization.	
	virtual Serializable* CreateObject(){return new PokerCashTableController();};
	INIT_RUNTIME_VARIABLE()
};
