#pragma once
#include "PokerClient.h"
#include "PokerBotInfo.h"

class PokerBot : public PokerClient
{
public:
	PokerBot(HWND hOwnerWnd = NULL, _String sIpAddress = _T("127.0.0.1"), int nPort = 20001, int nReconnectTime = 5000);
	virtual ~PokerBot();

	friend class PokerBotController;

	bool	StartBot(PokerBotInfo* pInfo);
	bool	StopBot	();

	void				AddTableInfo	(int nTableId, PokerTableInfo* pInfo);
	bool				RemoveTableInfo	(int nTableId);
	PokerTableInfo*		GetTableInfo	(int nTableId);
	bool				OpenRandomTable	(int nGameType, int nGameSubType, bool bReserveAnySeat);

protected: // Virtual events.

	// Server response events.
protected:
	void				OnLogin				(LoginPacket* p, void* pParam);
	void				OnLogout			(LogoutPacket* p, void* pParam);

protected:
	void				OnOpenTable			(PokerOpenTablePacket* p, void* pParam);
	void				OnLeaveTable		(PokerLeaveTablePacket* p, void* pParam);
	void				OnSeatAt			(PokerSeatAtTablePacket* p, void* pParam);
	void				OnAddChips			(PokerAddChipsPacket* p, void* pParam);
	void				OnReserveSeat		(PokerReserveSeatPacket* p, void* pParam);
	void				OnJoinWaitingList	(PokerJoinWaitingListPacket* p, void* pParam);
	void				OnAction			(PokerActionPacket* p, void* pParam);
	void				OnTableInfo			(PokerTableInfoPacket* p, void* pParam);

	void				OnReturnBack		(PokerReturnBackPacket* p, void* pParam);
	void				OnSitOut			(PokerSitOutPacket* p, void* pParam);
	void				OnPostBlind			(PokerPostBlindPacket* p, void* pParam);

	void				OnSetActiveSeat		(PokerSetActivePacket* p, void* pParam);
	void				OnSeatStatusChanged	(PokerSeatStatusChangedPacket* p, void* pParam);

	void				OnHandStarted		(PokerHandStartedPacket* p, void* pParam);
	void				OnHandFinished		(PokerHandFinishedPacket* p, void* pParam);
	void				OnRoundStarted		(PokerRoundStartedPacket* p, void* pParam);
	void				OnAct				(PokerActPacket* p, void* pParam);

	void				OnGetLobbyInfo		(PokerGetLobbyInfoPacket* p, void* pParam);

	void				OnShowMuckCards		(PokerShowMuckCardsPacket* p, void* pParam);
	void				OnSetOption			(PokerSetOptionPacket* p, void* pParam);
	void				OnSeatOnlineChanged	(PokerSeatOnlineChangedPacket* p, void* pParam);
	void				OnGetHandHistory	(PokerGetHandHistoryPacket* p, void* pParam);
	void				OnQuitClient		(PokerQuitClientPacket* p, void* pParam);
	void				OnSetAvatar			(PokerSetAvatarPacket* p, void* pParam);
	void				OnChatMessage		(PokerChatMessagePacket* p, void* pParam);

protected:
	PokerBotInfo			m_info;
	int						m_nTournamentId;
	List<PokerTableInfo>	m_listTableInfos;
	AutoSortedArray			m_arrPokerTableInfoById;	// <int - table id><PokerTableInfo* pInfo>
};
