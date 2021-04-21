#pragma once
#include "PokerGameBase.h"
#include "PokerTournamentManager.h"

class PokerServer;
class PokerTournamentData;
class PokerTournaments : public PokerGameBase
{
public:
	PokerTournaments();
	virtual ~PokerTournaments();

	friend class PokerServer;
	friend class PokerTournamentManager;

	virtual void				Initialize					(PokerServer* pServer, PacketHandler* pPacketHandler);
	virtual bool				Start						(PokerServerConfig* pConfig, LobbyInfo* pLobbyInfo);
	virtual bool				Stop						();
	
	TournamentTableController*	GetTableController			(int nTournamentId, int nTableId);
	PokerTournamentManager*		GetTournamentManager		(int nTournamentId);

protected:
	int							CreateTournamentGameTables	(PokerTournamentManager* pMan, int nTournamentId, List<TournamentTableController>* pListTables, AutoSortedArray* pArrGameTableById);
	TournamentInfo*				GetTournamentInfo			(int nTournamentId);
	int							RegisterUserInTournament	(PokerSession* pSess, int nUserId, int nTournamentId, bool bByPoints, bool bUnregister = false, bool bByTicket = false);

protected:
	// Received packet event methods.
	virtual void			OnAction							(PokerActionPacket* p, ClientConnection* pConn);
	virtual void			OnReturnBack						(PokerReturnBackPacket* p, ClientConnection* pConn);
	virtual void			OnSitOut							(PokerSitOutPacket* p, ClientConnection* pConn);
	virtual void			OnPostBlind							(PokerPostBlindPacket* p, ClientConnection* pConn);
	virtual void			OnShowMuckCards						(PokerShowMuckCardsPacket* p, ClientConnection* pConn);
	virtual void			OnSetOption							(PokerSetOptionPacket* p, ClientConnection* pConn);
	virtual void			OnChatMessage						(PokerChatMessagePacket* p, ClientConnection* pConn);
	virtual void			OnOpenRandomTable					(PokerOpenRandomTablePacket* p, ClientConnection* pConn);
	
	virtual void			OnCreateTournament					(PokerCreateTournamentPacket* p, ClientConnection* pConn);
	virtual void			OnCancelTournament					(PokerCancelTournamentPacket* p, ClientConnection* pConn);
	virtual void			OnTournamentCustomInfo				(PokerTournamentCustomInfoPacket* p, ClientConnection* pConn);
	virtual void			OnRegister							(PokerRegisterInTournamentPacket* p, ClientConnection* pConn);
	virtual void			OnUnregister						(PokerUnregisterFromTournamentPacket* p, ClientConnection* pConn);
	virtual void			OnOpenTable_Tournament				(PokerOpenTable_TournamentPacket* p, ClientConnection* pConn);
	virtual void			OnLeaveTable_Tournament				(PokerLeaveTable_TournamentPacket* p, ClientConnection* pConn);
	virtual void			OnRebuy								(PokerRebuyPacket* p, ClientConnection* pConn);
	virtual void			OnAddon								(PokerAddonPacket* p, ClientConnection* pConn);

	virtual void			OnGetTournaments					(PokerGetTournamentsPacket* p, ClientConnection* pConn);
	virtual void			OnRegisterUserInTournament			(PokerRegisterUserInTournamentPacket* p, ClientConnection* pConn);
	virtual void			OnHideShowTournament				(PokerHideShowTournamentPacket* p, ClientConnection* pConn);

protected:
	// Event methods from TableController.
	virtual bool			PlayerSeated						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, INT_CHIPS nSeatAmount, PokerTableSeatInfo* pSeat);
	virtual void			PlayerLeftSeat						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, int nSeat, PokerTableSeatInfo* pSeat);
	virtual void			PlayerSeatBalanceChanged			(PokerTableController* pController, void* pParam, PokerSession* pSess, int nTableId, int nCurrencyId, int nSeat, INT_CHIPS nSeatBalanceNew, INT_CHIPS nSeatBalanceOld);
	virtual bool			HandStarted							(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64& nHandId, int nTableId, int nPlayersCt, UINT nSB, UINT nBB, UINT nAnte);
	virtual bool			HandFinished						(PokerTableController* pController, void* pParam, PokerTableInfo* pInfo, __int64 nHandId,  INT_CHIPS nTotalPotAmount, UINT nRakeAmount, PokerHandHistory* pHandHistory, _String& sPlayersInfo, _String& sTableCards);

	virtual bool			UserAddRebuyChips					(PokerSession* pSess, PokerTableController* pTable, void* pParam, int nTableId, PokerTableSeatInfo* pSeat, INT_CHIPS nAmount, bool bUseVPPoints);
	virtual bool			UserAddAddonChips					(PokerSession* pSess, PokerTableController* pTable, void* pParam, int nTableId, PokerTableSeatInfo* pSeat, INT_CHIPS nAmount, bool bUseVPPoints);

	// Event methods from Tournament manager.
	virtual void			TournamentStarted					(int nTournamentId, void* pParam);
	virtual void			TournamentFinished					(int nTournamentId, void* pParam);
	virtual void			TournamentVisibilityChanged			(int nTournamentId, void* pParam, bool bVisible);
	virtual void			TournamentStatusChanged				(int nTournamentId, void* pParam, TournamentStatus statusNew);
	virtual void			TournamentRegistrationStatusChanged	(int nTournamentId, void* pParam, bool bRegIsOpen);
	virtual void			TournamentLevelChanged				(int nLevelIndex, void* pParam);
	virtual void			TournamentTableCreated				(int nTournamentId, int nTableId, void* pParam, TournamentTableController* pController);
	virtual void			GameTableClosed						(int nTournamentId, int nTableId, void* pParam, TournamentTableController* pController);

protected:
	PokerTournamentData*						m_pData;
	EnumerableObject<PokerTournamentManager>	m_listTournaments;
	CCriticalSection							m_lockTournamentsList;
};
