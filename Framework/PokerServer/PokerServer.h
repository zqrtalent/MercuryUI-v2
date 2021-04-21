#pragma once
#include "../GameServer/GameServer.h"
#include "Data/PokerData.h"
#include "../Protocol/PokerPacket.h"
#include "../Protocol/PokerAdministrationPacket.h"
#include "../Serialize/Serializable.h"
#include "../Array/AutoSortedArrayTempl.h"
#include "Object/LobbyInfo.h"

#include "PokerCashTableController.h"
#include "TournamentTableController.h"

#include "PokerSession.h"
#include "PokerServerConfig.h"

#include "PokerCashGames.h"
#include "PokerTournaments.h"

class PokerServer : public GameServer
{
public:
	friend class PokerCashGames;
	friend class PokerTableController;
	friend class PokerCashTableController;

	friend class PokerTournaments;
	friend class PokerTournamentManager;
	friend class TournamentTableController;
	friend class PokerTournamentData;

	PokerServer();
	virtual ~PokerServer();

	virtual bool				Start								();
	virtual bool				Stop								();

	bool						SendPacketByTable					(int nTableId, int nTournamentId, BasePacket* p, bool bOnlyObservers = false);
	int							QuitAllClients						();
	LobbyInfo*					GetLobbyInfo						(){return &m_lobbyInfo;};

	int							GetTablesCt							();
	int							GetTournamentsCt					();
	int							GetPlayersCt						();

protected:
	int							LoadAvatars							(_String sFolder);
	void						FillWithAvatarsList					(LoginPacket* p);

protected:
	virtual GameServerConfig*	LoadConfiguration			(){
		PokerServerConfig* pConf = new PokerServerConfig();
		if( !pConf->DeserializeAsXML(_T("Config.xml")) ){
			delete pConf;
			return NULL;
			}
		return pConf;
		};

	// Event methods of client connection.
	virtual Session*			CreateSession						(ClientConnection* pConn){return new PokerSession(this, pConn);};
	virtual void				OnDisconnected						(ClientConnection* pConn, DWORD dwLastError);
	virtual void				OnConnected							(ClientConnection* pConn, ConnectedSocketInfo* pSockInfo);
	virtual void				OnPacketReceived					(ClientConnection* pConn, BasePacket* packet);
	virtual void				OnSessionExpired					(Session* pSess);
	virtual	void				OnSessionRestored					(Session* pSess, ClientConnection* pConn);

	// Received packet event methods.
	void						OnLogin								(LoginPacket* p, ClientConnection* pConn);
	void						OnLogout							(LogoutPacket* p, ClientConnection* pConn);
	void						OnGetLobbyInfo						(PokerGetLobbyInfoPacket* p, ClientConnection* pConn);
	void						OnGetHandHistory					(PokerGetHandHistoryPacket* p, ClientConnection* pConn);
	void						OnSetAvatar							(PokerSetAvatarPacket* p, ClientConnection* pConn);
	void						OnChatMessage						(PokerChatMessagePacket* p, ClientConnection* pConn);
	void						OnMoneyTransfer						(PokerMoneyTransferPacket* p, ClientConnection* pConn);
	void						OnResetPlaymoney					(PokerResetPlaymoneyPacket* p, ClientConnection* pConn);
	void						OnRefreshBalance					(PokerRefreshBalancePacket* p, ClientConnection* pConn);
	void						OnFindPlayer						(PokerFindPlayerPacket* p, ClientConnection* pConn);
	void						OnFindTournament					(PokerFindTournamentPacket* p, ClientConnection* pConn);

	// Poker game table packets.
	void						OnOpenTable							(PokerOpenTablePacket* p, ClientConnection* pConn);
	void						OnLeaveTable						(PokerLeaveTablePacket* p, ClientConnection* pConn);
	void						OnReserveSeat						(PokerReserveSeatPacket* p, ClientConnection* pConn);
	void						OnCancelReservation					(PokerCancelReservationPacket* p, ClientConnection* pConn);
	void						OnSeatAt							(PokerSeatAtTablePacket* p, ClientConnection* pConn);
	void						OnJoinWaitingList					(PokerJoinWaitingListPacket* p, ClientConnection* pConn);
	void						OnAddChips							(PokerAddChipsPacket* p, ClientConnection* pConn);
	void						OnAction							(PokerActionPacket* p, ClientConnection* pConn);
	void						OnTableInfo							(PokerTableInfoPacket* p, ClientConnection* pConn);
	void						OnReturnBack						(PokerReturnBackPacket* p, ClientConnection* pConn);
	void						OnSitOut							(PokerSitOutPacket* p, ClientConnection* pConn);
	void						OnPostBlind							(PokerPostBlindPacket* p, ClientConnection* pConn);
	void						OnShowMuckCards						(PokerShowMuckCardsPacket* p, ClientConnection* pConn);
	void						OnSetOption							(PokerSetOptionPacket* p, ClientConnection* pConn);
	void						OnOpenRandomTable					(PokerOpenRandomTablePacket* p, ClientConnection* pConn);

	// Tournament packet methods.
	void						OnTournamentCustomInfo				(PokerTournamentCustomInfoPacket* p, ClientConnection* pConn);
	void						OnRegister							(PokerRegisterInTournamentPacket* p, ClientConnection* pConn);
	void						OnUnregister						(PokerUnregisterFromTournamentPacket* p, ClientConnection* pConn);
	void						OnOpenTable_Tournament				(PokerOpenTable_TournamentPacket* p, ClientConnection* pConn);
	void						OnLeaveTable_Tournament				(PokerLeaveTable_TournamentPacket* p, ClientConnection* pConn);
	void						OnRebuy								(PokerRebuyPacket* p, ClientConnection* pConn);
	void						OnAddon								(PokerAddonPacket* p, ClientConnection* pConn);

	// Upload / Download methods.
	void						OnUploadAvatar						(PokerUploadAvatarPacket* p, ClientConnection* pConn);
	void						OnDownloadAvatar					(PokerDownloadAvatarPacket* p, ClientConnection* pConn);

	// Administration methods.
	void						OnCreateTournament					(PokerCreateTournamentPacket* p, ClientConnection* pConn);
	void						OnCancelTournament					(PokerCancelTournamentPacket* p, ClientConnection* pConn);
	void						OnGetGameTables						(PokerGetGameTablesPacket* p, ClientConnection* pConn);
	void						OnGetTournaments					(PokerGetTournamentsPacket* p, ClientConnection* pConn);
	void						OnGetClientConnections				(PokerGetClientConnectionsPacket* p, ClientConnection* pConn);
	void						OnSendAdminMessageByTable			(PokerSendAdminMessageByTablePacket* p, ClientConnection* pConn);
	void						OnSendAdminMessageByUser			(PokerSendAdminMessageByUserPacket* p, ClientConnection* pConn);
	void						OnCreateGameTable					(PokerCreateGameTablePacket* p, ClientConnection* pConn);
	void						OnForceClientSessionToQuit			(PokerForceClientSessionToQuitPacket* p, ClientConnection* pConn);
	void						OnDisableServerConnect				(PokerDisableServerConnectPacket* p, ClientConnection* pConn);
	void						OnDeactivateGameTable				(PokerDeactivateGameTablePacket* p, ClientConnection* pConn);
	void						OnBlockUser							(PokerBlockUserPacket* p, ClientConnection* pConn);
	void						OnRegisterUserInTournament			(PokerRegisterUserInTournamentPacket* p, ClientConnection* pConn);
	void						OnHideShowTournament				(PokerHideShowTournamentPacket* p, ClientConnection* pConn);

	// Seat user session on table
	PokerSession*				SeatUserSession						(int nUserId, int nTableId, int nSeatIndex, int nTournamentId = 0, INT_CHIPS nSeatAmount = 0, int nRebuyCt = 0, int nAddonCt = 0);
	// Game controllers, tables and connections.
	PokerCashTableController*	GetCashGameController				(int nTableId);
	TournamentTableController*	GetTournamentGameController			(int nTournament, int nTableId);
	// ClientConnection by table id.
	bool						AddTableConnection					(int nTableId, int nTournamentId, ClientConnection* pConn, PokerTableController* pController);
	bool						RemoveTableConnection				(int nTableId, int nTournamentId, ClientConnection* pConn, PokerTableController* pController);
	bool						RemoveConnectionsByTable			(int nTableId, int nTournamentId, PokerTableController* pController);
	// Retrieves table connections.
	bool						GetTableConnections					(int nTableId, int nTournamentId, List<ClientConnection>*& pListConn);
	// Ratrieves tables opened by connection.
	bool						GetConnectionTables					(ClientConnection* pConn, List<PokerTableController>*& pListControllers);
	bool						IsTableOpenedByConn					(int nTableId, int nTournamentId, ClientConnection* pConn);
	// Remove ClientConnection* from all dependant lists.
	bool						RemoveConnection					(ClientConnection* pConn, bool bUnseatFromTable = true);

	bool						RemoveAllGameControllersByConnection();
	bool						RemoveAllConnectionsByTableId		();

	bool						GetBalanceByAccount					(int nAccountType, int nUserId, int nCurrencyId, INT_CHIPS& nBalance);
	bool						TransferMoneyFromMainBalance		(int nUserId, INT_CHIPS nAmount, int nCurrencyId, INT_CHIPS& nBalanceNew, INT_CHIPS& nMainBalance);
	bool						TransferMoneyToMainBalance			(int nUserId, INT_CHIPS nAmount, int nCurrencyId, INT_CHIPS& nBalanceNew, INT_CHIPS& nMainBalance);

	bool						IsAllowedMultiUserIp				(DWORD dwIp);
	bool						IsDontRestrictIPUser				(_String sUserName);

protected:
	AutoSortedArrayTempl<__int64, List<ClientConnection>*>					m_connByTableId;				// Game table connections.
	AutoSortedArrayTempl<ClientConnection*, List<PokerTableController>*>	m_tableControllerByConn;		// Game controllers by connection.

	CCriticalSection														m_lockTableObj;					// Lock table objects.
	CCriticalSection														m_lockUserObj;					// Lock user info objects.
	PokerData*																m_pData;						// Poker data manager class.
	LobbyInfo																m_lobbyInfo;					// Lobby info class.

	PokerServerConfig*														m_pPokerGameConfig;

	PokerCashGames															m_cashGames;					// Poker cash game.
	PokerTournaments														m_tournamentGames;				// Poker tournaments.
};
