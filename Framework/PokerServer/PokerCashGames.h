#pragma once
#include "PokerGameBase.h"

class PokerServer;
class PokerCashData;
class PokerCashGames : public PokerGameBase
{
public:
	friend class PokerServer;
	friend class PokerGameController;

	PokerCashGames();
	virtual ~PokerCashGames();

	virtual void			Initialize							(PokerServer* pServer, PacketHandler* pPacketHandler);
	virtual bool			Start								(PokerServerConfig* pConfig, LobbyInfo* pLobbyInfo);
	virtual bool			Stop								();

	PokerCashTableController*	GetGameController				(int nTableId);
	
protected:	
	// Received packet event methods.
	virtual void			OnOpenTable							(PokerOpenTablePacket* p, ClientConnection* pConn);
	virtual void			OnLeaveTable						(PokerLeaveTablePacket* p, ClientConnection* pConn);
	virtual void			OnReserveSeat						(PokerReserveSeatPacket* p, ClientConnection* pConn);
	virtual void			OnCancelReservation					(PokerCancelReservationPacket* p, ClientConnection* pConn);
	virtual void			OnSeatAt							(PokerSeatAtTablePacket* p, ClientConnection* pConn);
	virtual void			OnJoinWaitingList					(PokerJoinWaitingListPacket* p, ClientConnection* pConn);
	virtual void			OnAddChips							(PokerAddChipsPacket* p, ClientConnection* pConn);
	virtual void			OnAction							(PokerActionPacket* p, ClientConnection* pConn);
	virtual void			OnTableInfo							(PokerTableInfoPacket* p, ClientConnection* pConn);
	virtual void			OnReturnBack						(PokerReturnBackPacket* p, ClientConnection* pConn);
	virtual void			OnSitOut							(PokerSitOutPacket* p, ClientConnection* pConn);
	virtual void			OnPostBlind							(PokerPostBlindPacket* p, ClientConnection* pConn);
	virtual void			OnShowMuckCards						(PokerShowMuckCardsPacket* p, ClientConnection* pConn);
	virtual void			OnSetOption							(PokerSetOptionPacket* p, ClientConnection* pConn);
	virtual void			OnChatMessage						(PokerChatMessagePacket* p, ClientConnection* pConn);
	virtual void			OnOpenRandomTable					(PokerOpenRandomTablePacket* p, ClientConnection* pConn);

	virtual void			OnGetGameTables						(PokerGetGameTablesPacket* p, ClientConnection* pConn);
	virtual void			OnCreateGameTable					(PokerCreateGameTablePacket* p, ClientConnection* pConn);
	virtual void			OnDeactivateGameTable				(PokerDeactivateGameTablePacket* p, ClientConnection* pConn);

	// Event methods from GameController.
	virtual bool			PlayerSeated						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, INT_CHIPS nSeatAmount, PokerTableSeatInfo* pSeat);
	virtual void			PlayerLeftSeat						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, PokerTableSeatInfo* pSeat);
	virtual void			PlayerReservedSeat					(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nSeat);
	virtual void			PlayerCanceledReservation			(PokerTableController* pController, void* pParam, PokerSession* pSess, int nUserId, int nTableId, int nSeat);
	virtual void			PlayerAddedInWaitList				(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId);
	virtual void			PlayerRemovedFromWaitList			(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId);
	virtual void			PlayerSeatBalanceChanged			(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nCurrencyId, int nSeat, INT_CHIPS nSeatBalanceNew, INT_CHIPS nSeatBalanceOld);
	virtual bool			HandStarted							(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64& nHandId, int nTableId, int nPlayersCt, UINT nSB, UINT nBB, UINT nAnte);
	virtual bool			HandFinished						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64 nHandId,  INT_CHIPS nTotalPotAmount, UINT nRakeAmount, PokerHandHistory* pHandHistory, _String& sPlayersInfo, _String& sTableCards);

	virtual bool			SetupLobbyInfo						(LobbyInfo* pLobbyInfo);
	virtual bool			UserAddChips						(PokerSession* pSess, PokerTableController* pTable, void* pParam, int nTableId,  int nSeat, INT_CHIPS nAmount, int nCurrencyId);
	virtual bool			GetSeatMinBuyIn						(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS& nAmount, INT_CHIPS& nBalance);

	virtual bool			SeatUser							(PokerSession* pSess, int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount);
	virtual bool			UnseatUser							(PokerSession* pSess, int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount);

protected:
	PokerCashData*			m_pData;						// Poker data manager class.
};
