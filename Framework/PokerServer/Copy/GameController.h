#pragma once
#include "..\Serialize\Serializable.h"
#include "..\Array\List.h"
#include "..\Protocol\PokerPacket.h"
#include "..\Thread\WinThread.h"
#include "Object\PokerTableSeatInfo.h"
#include "Object\PokerTableInfo.h"
#include "Object\PokerHandHistory.h"

#include <afxmt.h>

class PokerGameController;
class PokerServer;
class PokerSession;

typedef bool (__thiscall PokerGameController::*OnTimerElapsed)();

class PokerGameController : public Serializable
{
public:
	PokerGameController(PokerServer* pServer = NULL);
	virtual ~PokerGameController();

	friend class PokerServer;
	friend class PokerData;

protected:
	static void	__stdcall	OnTimerEvent					(int nIdEvent, void* lpParam);
	void					OnTimerEventElapsed				(int nIdEvent);
	void					OnReservationTimerElapsed		(int nSeat);
	void					OnLeaveSeatTimerElapsed			(int nSeat);
	bool					StartSeatReservationTimer		(int nSeat);
	bool					StopSeatReservationTimer		(int nSeat);
	bool					StartLeaveSeatTimer				(int nSeat);
	bool					StopLeaveSeatTimer				(int nSeat);
	void					KillAllTimers					();
	bool					IsSeatReservedByConn			(int nSeat, ClientConnection* pConn);
	bool					CancelSeatReservation			(int nSeat);

protected:
	void					OnOpenTable						(PokerSession* pSess, ClientConnection* pConn, PokerOpenTablePacket* p);
	void					OnLeaveTable					(PokerSession* pSess, ClientConnection* pConn, PokerLeaveTablePacket* p);
	void					OnSeatAt						(PokerSession* pSess, ClientConnection* pConn, PokerSeatAtTablePacket* p);
	void					OnReserveSeat					(PokerSession* pSess, ClientConnection* pConn, PokerReserveSeatPacket* p);
	void					OnCancelReservation				(PokerSession* pSess, ClientConnection* pConn, PokerCancelReservationPacket* p);
	void					OnJoinWaitingList				(PokerSession* pSess, ClientConnection* pConn, PokerJoinWaitingListPacket* p);
	void					OnAddChips						(PokerSession* pSess, ClientConnection* pConn, PokerAddChipsPacket* p);
	void					OnAction						(PokerSession* pSess, ClientConnection* pConn, PokerActionPacket* p, bool bPreAction = false);
	void					OnTableInfo						(PokerSession* pSess, ClientConnection* pConn, PokerTableInfoPacket* p);
	void					OnReturnBack					(PokerSession* pSess, ClientConnection* pConn, PokerReturnBackPacket* p);
	void					OnSitOut						(PokerSession* pSess, ClientConnection* pConn, PokerSitOutPacket* p);
	void					OnPostBlind						(PokerSession* pSess, ClientConnection* pConn, PokerPostBlindPacket* p);
	void					OnShowMuckCards					(PokerSession* pSess, ClientConnection* pConn, PokerShowMuckCardsPacket* p);
	void					OnSetOption						(PokerSession* pSess, ClientConnection* pConn, PokerSetOptionPacket* p);
	void					OnChatMessage					(PokerSession* pSess, ClientConnection* pConn, PokerChatMessagePacket* p);
	
	void					OnConnectionDropped				(PokerSession* pSess, ClientConnection* pConn, int nSeat);
	void					OnGameSessionExpired			(PokerSession* pSess, ClientConnection* pConn, bool& bCancel);
	bool					OnGameSessionRestored			(PokerSession* pSess, ClientConnection* pConn, int nUserId);


	// Game thread methods. {{
	bool					GThread_IsStarted				(){ return (!m_gameThread.IsThreadStopped()); }
	void					GThread_Start					(bool bSuspended = false);
	void					GThread_Stop					();
	static	LRESULT			GThread_Entry					(WinThread* pGameThread);
	bool					GThread_WaitForReply			(ClientConnection* pConn, BasePacket* pSent, int nSecWait, 
															BasePacket* pPacketReceived, bool& bElapsed, int* pMSecWasted);
	bool					GThread_Sleep					(int nSecSleep);
	bool					GThread_StartHand				();
	bool					GThread_FinishHand				();
	bool					GThread_FinishRound				(bool& bHandFinished);
	bool					GThread_AskForAction			(int nSeatPrev, int& nSeatAsked, bool& bRoundEnded, bool& bOpenCards);
	bool					GThread_ActionReplyRecieved		(PokerActionPacket* p, /*Received action packet*/
															int nSecElapsed /*Seconds elapsed while waiting for action from client*/);
	bool					GThread_ActTimerElapsed			(PokerActPacket* pSent /*Act packet sent to client*/, 
															ClientConnection* pConn, int& nSeatFrom);
	bool					GThread_IsPaused				(){return m_gameThread.IsPauseEventSignaled();};
	bool					GThread_Resume					(){ if( !GThread_IsStarted() ){GThread_Start(false);} return m_gameThread.ContinueThread();};
	// }}
	
	int						PerformAddChipsAction			();// Perform all addchips actions.
	// Returns new balance amount. returns zero when fail.
	int						AddChips						(int nSeat, int nAmount);// Add chips.

	bool					DealCardsAndStartHand			(int nDealer, int SBSeat, int BBSeat, CDWordArray& arrPayBBSeats);
	bool					SendSeatStatusChange			(int nSeat, PokerSeatStatus status);
	bool					SendSetActiveSeat				(int nActiveSeat, int nRegularTimeSec, int nExtraTimeSec = 0, int nTimeBankSec = 0);
	bool					SendPostBlindRequest			(int nSeat, bool bSmallBlind, bool& bResult);
	bool					SendShowMuckCardsRequest		(int nSeat, bool& bResult);
	bool					SendSeatActionPacket			(int nSeat, PokerActionTypes action, PokerSeatStatus status, int nAmount, bool bSetSitOutNextHand = false);
	bool					SitOutBySeat					(int nSeat);
	bool					SitOut							(int nSeat, ClientConnection* pConn);
	bool					SetSeatOffline					(int nSeat, bool bOnline = false, PokerSession* pSess = NULL);
	bool					EmptySeat						(int nSeat);
	int						EmptySessionlessSeats			();
	bool					ForceSeatToLeaveTable			(int nSeat);
	bool					GetEmptySeat					(int& nSeat);

	// Wait list.
	void					SeatWaitListUsers				(AutoSortedArray& arrSeated);
	int						AddInWaitList					(ClientConnection* pConn);
	bool					RemoveFromWaitList				(ClientConnection* pConn);
	bool					IsInWaitList					(ClientConnection* pConn);

protected:
	PokerServer*				m_pServer;
	PokerTableInfo				m_info;
	List<ClientConnection>		m_listWaitingList;				// Waiting list connections.
	AutoSortedArrayInt64		m_arrWaitingListConn;			// Array of waiting list connections.
	AutoSortedArray				m_arrAddChips;					// Add chips actions list. <int seat index><add chips amount>
	CCriticalSection			m_lockObjects;

	WinThread					m_gameThread;

	int							m_nDealer;
	int							m_nSmallBlind;
	int							m_nBigBlind;
	__int64						m_nHandId;						// Current hand id.
	int							m_nSBAmount;					// Small blind amount.
	int							m_nBBAmount;					// Big blind amount.
	int							m_nAnteAmount;					// Ante amount.
	int							m_nMinBuyIn;					// Min buy in amount.
	int							m_nMaxBuyIn;					// Max buy in amount.
	int							m_nCurrencyId;					// Table currency id.
	
	GameType					m_gametype;						// Game type definition.
	GameSubType					m_subType;						// Game sub type enum.
	MoneyType					m_moneyType;					// Money type definition.

	bool						m_bIsTournamentGame;			// Indicates tournament game type.

	int							m_arrSeatReserveTimerIds[10];	// Seat reservation timer ids.
	int							m_arrLeaveSeatTimerIds[10];		// Leave seat timer ids.

	int							m_nHandStartDelay;				// Hand start seconds delay.
	int							m_nPostBlindDelay;				// Post blind action delay.
	int							m_nStartRoundDelay;				// Start round delay.
	int							m_nShowMuckDelay;				// Show/Muck cards delay.
	int							m_nReserveSeatDelay;			// Reserve seat delay.
	int							m_nLeaveSitoutedDelay;			// Leave sitouted seat delay.

	int							m_nRegularTimeSec;				// Regular thinking time seconds.
	int							m_nExtraTimeSec;				// Extra thinking time seconds.
	int							m_nTimeBankSec;					// Time bank seconds.
	int							m_nTimeBankInc;					// Time bank increase seconds.
	int							m_nTimeBankIncHands;			// Time bank increase hand count.

	PokerHandHistory			m_handHistory;					// Hand history class.
	
protected: // Serialization.	
	virtual Serializable* CreateObject(){return new PokerGameController();};
	INIT_RUNTIME_VARIABLE()

};
