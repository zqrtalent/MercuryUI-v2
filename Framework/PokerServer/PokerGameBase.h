#pragma once
#include "Protocol/PokerPacket.h"
#include "Protocol/PacketHandler.h"
#include "PokerServerConfig.h"

class PokerServer;
class PokerGameBase : public Serializable
{
public:
	friend class PokerTableController;
	friend class PokerCashTableController;
	friend class TournamentTableController;
	
	PokerGameBase();
	virtual ~PokerGameBase();

	virtual void			Initialize							(PokerServer* pServer, PacketHandler* pPacketHandler){};
	virtual bool			Start								(PokerServerConfig* pConfig, LobbyInfo* pLobbyInfo){ASSERT(FALSE); return false;};
	virtual bool			Stop								(){ASSERT(FALSE); return false;};

	PokerTableController*	GetTableController					(int nTableId);	

protected:
	// Received packet event methods.
	virtual void			OnOpenTable							(PokerOpenTablePacket* p, ClientConnection* pConn){};
	virtual void			OnLeaveTable						(PokerLeaveTablePacket* p, ClientConnection* pConn){};
	virtual void			OnReserveSeat						(PokerReserveSeatPacket* p, ClientConnection* pConn){};
	virtual void			OnCancelReservation					(PokerCancelReservationPacket* p, ClientConnection* pConn){};
	virtual void			OnSeatAt							(PokerSeatAtTablePacket* p, ClientConnection* pConn){};
	virtual void			OnJoinWaitingList					(PokerJoinWaitingListPacket* p, ClientConnection* pConn){};
	virtual void			OnAddChips							(PokerAddChipsPacket* p, ClientConnection* pConn){};
	virtual void			OnAction							(PokerActionPacket* p, ClientConnection* pConn){};
	virtual void			OnTableInfo							(PokerTableInfoPacket* p, ClientConnection* pConn){};
	virtual void			OnReturnBack						(PokerReturnBackPacket* p, ClientConnection* pConn){};
	virtual void			OnSitOut							(PokerSitOutPacket* p, ClientConnection* pConn){};
	virtual void			OnPostBlind							(PokerPostBlindPacket* p, ClientConnection* pConn){};
	virtual void			OnGetLobbyInfo						(PokerGetLobbyInfoPacket* p, ClientConnection* pConn){};
	virtual void			OnShowMuckCards						(PokerShowMuckCardsPacket* p, ClientConnection* pConn){};
	virtual void			OnSetOption							(PokerSetOptionPacket* p, ClientConnection* pConn){};
	virtual void			OnChatMessage						(PokerChatMessagePacket* p, ClientConnection* pConn){};
	virtual void			OnOpenRandomTable					(PokerOpenRandomTablePacket* p, ClientConnection* pConn){};

	virtual void			OnGetGameTables						(PokerGetGameTablesPacket* p, ClientConnection* pConn){};
	virtual void			OnGetTournaments					(PokerGetTournamentsPacket* p, ClientConnection* pConn){};
	virtual void			OnCreateGameTable					(PokerCreateGameTablePacket* p, ClientConnection* pConn){};
	virtual void			OnDeactivateGameTable				(PokerDeactivateGameTablePacket* p, ClientConnection* pConn){};
	virtual void			OnCreateTournament					(PokerCreateTournamentPacket* p, ClientConnection* pConn){};
	virtual void			OnCancelTournament					(PokerCancelTournamentPacket* p, ClientConnection* pConn){};
	virtual void			OnTournamentCustomInfo				(PokerTournamentCustomInfoPacket* p, ClientConnection* pConn){};

	virtual void			OnRegister							(PokerRegisterInTournamentPacket* p, ClientConnection* pConn){};
	virtual void			OnUnregister						(PokerUnregisterFromTournamentPacket* p, ClientConnection* pConn){};
	virtual void			OnOpenTable_Tournament				(PokerOpenTable_TournamentPacket* p, ClientConnection* pConn){};
	virtual void			OnLeaveTable_Tournament				(PokerLeaveTable_TournamentPacket* p, ClientConnection* pConn){};

	// Event methods from TableController.
	virtual bool			PlayerSeated						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, INT_CHIPS nSeatAmount, PokerTableSeatInfo* pSeat){return false;};
	virtual void			PlayerLeftSeat						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, PokerTableSeatInfo* pSeat){};
	virtual void			PlayerReservedSeat					(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nSeat){};
	virtual void			PlayerCanceledReservation			(PokerTableController* pController, void* pParam, PokerSession* pSess, int nUserId, int nTableId, int nSeat){};
	virtual void			PlayerAddedInWaitList				(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId){};
	virtual void			PlayerRemovedFromWaitList			(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId){};
	virtual void			PlayerSeatBalanceChanged			(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nCurrencyId, int nSeat, INT_CHIPS nSeatBalanceNew, INT_CHIPS nSeatBalanceOld){};
	virtual bool			HandStarted							(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64& nHandId, int nTableId, int nPlayersCt, UINT nSB, UINT nBB, UINT nAnte){return false;};
	virtual bool			HandFinished						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64 nHandId,  INT_CHIPS nTotalPotAmount, UINT nRakeAmount, PokerHandHistory* pHandHistory, _String& sPlayersInfo, _String& sTableCards){return false;};

	virtual bool			SetupLobbyInfo						(LobbyInfo* pLobbyInfo){return false;};
	virtual bool			UserAddChips						(PokerSession* pSess, PokerTableController* pTable, void* pParam, int nTableId,  int nSeat, INT_CHIPS nAmount, int nCurrencyId){return false;};
	virtual bool			UserAddRebuyChips					(PokerSession* pSess, PokerTableController* pTable, void* pParam, int nTableId, PokerTableSeatInfo* pSeat, INT_CHIPS nAmount, bool bUseVPPoints){return false;};
	virtual bool			UserAddAddonChips					(PokerSession* pSess, PokerTableController* pTable, void* pParam, int nTableId, PokerTableSeatInfo* pSeat, INT_CHIPS nAmount, bool bUseVPPoints){return false;};

	virtual bool			GetSeatMinBuyIn						(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS& nAmount, INT_CHIPS& nBalance){return false;};
	virtual bool			GetTableStatInfoByUser				(int nUserId, int nTableId, PokerOpenTablePacket* p){return false;};

	virtual bool			SeatUser							(UserInfo* pUserInfo, int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount){return false;};
	virtual bool			UnseatUser							(UserInfo* pUserInfo, int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount){return false;};

protected:
	bool						m_bInited;
	PokerServer*				m_pServer;						// Poker server instance.
	PacketHandler*				m_pPacketHandler;				// Poker packet handler.
	LobbyInfo*					m_pLobbyInfo;					// Lobby info class.

	List<PokerTableController>	m_listActiveGames;				// List of active game controllers.
	AutoSortedArray				m_arrActiveGameById;			// [TableId][PokerTableController*]
	CCriticalSection			m_lockGames;					// Lock game array objects.
};
