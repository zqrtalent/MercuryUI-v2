#pragma once
#include "GameClient.h"
#include "..\Protocol\PokerPacket.h"
#include "..\Utility\GrowableMemory.h"

typedef void (__stdcall *UploadProgressProc)(CObject* pReceiver, PokerUploadAvatarPacket* p); 
typedef void (__stdcall *AvatarDownloadProgressProc)(CObject* pReceiver, PokerDownloadAvatarPacket* p); 
typedef void (__stdcall *AvatarDownloadCompleteProc)(CObject* pReceiver, PokerDownloadAvatarPacket* p);
typedef void (__stdcall *AvatarUpdateProc)(CObject* pReceiver, int nUserId, int nAvatarId);

class PokerClient : public GameClient
{
public:
	PokerClient(HWND hOwnerWnd = NULL, _String sIpAddress = _T("127.0.0.1"), int nPort = 20001, int nReconnectTime = 5000);
	virtual ~PokerClient();

	enum {
	AvatarUploadChunkSize = 512
	};

public: // Operations.
	void				OpenTable				(int nTableId, bool bSeatAtOnce = false, int nSeatIndex = -1);
	void				LeaveTable				(int nTableId);
	void				SeatAt					(int nTableId, int nSeatIndex, INT_CHIPS nAmount);
	void				ReserveSeat				(int nTableId, int nSeatIndex);
	void				CancelReservation		(int nTableId, int nSeatIndex);
	void				JoinWaitingList			(int nTableId);
	void				Request_AddChips		(int nTableId);
	void				AddChips				(int nTableId, INT_CHIPS nChipAmount);
	void				Action					(int nTableId, int nTournamentId, int nSeat, PokerActionTypes action, INT_CHIPS nAmount);
	void				GetTableInfo			(int nTableId, int nTournamentId);
	void				GetLobbyInfo			(int nMoneyType, int nGameType, int nSubType, int nRequestFlags = PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesStaticInfo|PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesDynamicInfo);
	void				GetTablePlayers			(int nTableId);
	void				ShowMuckCards			(int nTableId, int nTournamentId, bool bShowCards);
	void				SetOption				(int nTableId, int nTournamentId, PokerTableOption option, bool bValue, INT_CHIPS nValue = 0, int nSeatIndex = 0);
	void				ReturnBack				(int nTableId, int nTournamentId, int nSeat);
	void				GetHandHistory			(__int64 nHandId, int nTournamentId);

	// Uploading avatar image. {{
	bool				UploadAvatarImageFile	(_String sImageFile, CObject* pReceiver, UploadProgressProc procProgress, UploadProgressProc procComplete, UploadProgressProc procError);
	bool				UploadAvatarImage		(Image* pImgAvatar, CObject* pReceiver, UploadProgressProc procProgress, UploadProgressProc procComplete, UploadProgressProc procError);
	bool				IsAvatarUploadStarted	();
	bool				IsAvatarUploadCompleted	();
	bool				DiscardUploadingAvatar	();
	// }}

	// Download avatar image. {{
	bool				DownloadMultipleAvatars (AutoSortedArray* pArrUserIdByAvatarId);
	bool				AvatarExists			(int nAvatarId);
	bool				DownloadAvatarImageFile	(int nUserId, int nAvatarId);
	bool				IsAvatarDownloadStarted	();
	bool				IsAvatarDownloadCompleted();
	bool				DiscardDownloadingAvatar ();
	// }}

	bool				Rebuy					(int nTableId, int nTournamentId, bool bByVPPAmount = false, bool b2xRebuy = false);
	bool				Addon					(int nTableId, int nTournamentId, bool bByVPPAmount = false);

	bool				SendChatMessage			(int nTableId, int nTournamentId, _String sMessage);
	bool				TransferMoney			(int nAccountFrom, int nAccountTo, INT_CHIPS nAmount, bool bRefreshBalances);
	bool				ResetPlayBalance		();
	bool				RefreshBalance			();
	bool				FindPlayer				(_String sUserName);
	bool				FindPlayerByUserId		(int nUserId);
	bool				FindTournament			(_String sTournamentID);
	bool				RefreshLobbyInfo		(int nGameType, int nGameSubType, int nSelectedTableId, int nSecRefreshAfter = -1);

protected: // Lobby info Cache
	EnumerableObject<LobbyCashTableStaticInfo>*	GetCachedTables			(int nGameType, int nGameSubType, int& nIndex);
	EnumerableObject<LobbyTournamentInfo>*		GetCachedTournaments	(int nGameType, int nGameSubType, int& nIndex);

	bool										UpdateTablesCache		(PokerGetLobbyInfoPacket* p);
	bool										UpdateTournamentsCache	(PokerGetLobbyInfoPacket* p);

	bool										IsTablesCached			(int nGameType, int nGameSubType, int& nTablesCt, int& nGameSubTypeNotCached);
	bool										IsTournamentsCached		(int nGameType, int nGameSubType, int& nTournamentsCt, int& nGameSubTypeNotCached);
	void										ClearCache				();

public:
	int											CopyCachedTables		(int nGameType, int nGameSubType, EnumerableObject<LobbyCashTableStaticInfo>* pListTables);
	int											CopyCachedTournaments	(int nGameType, int nGameSubType, EnumerableObject<LobbyTournamentInfo>* pListTournaments);
	bool										GetTournamentInfo		(int nTournamentId, LobbyTournamentInfo* pInfo);

public:
	// Tournament
	bool				GetTournamentLobbyInfo	(int nTournamentId, int nFlags, 
												int nGameTableIndex = 0, int nGameTableCt = 0 /*Tournament game tables*/,
												int nParticipantIndex = 0, int nParticipantCt = 0, /*Tournament Particpants*/
												int nGameTableIndexPlayers = 0 /*Table index for players*/);

	void				OpenTable_Tournament	(int nTournamentId, int nTableId);
	void				LeaveTable_Tournament	(int nTournamentId, int nTableId);
	bool				OpenMyTableInTournament	(int nTournamentId);
	void				RegisterInTournament	(int nTournamentId, bool bByBalanceAmount = true, bool bByPoints = false, bool bByTicket = false);
	void				UnregisterFromTournament(int nTournamentId);
	int					CopyMyTournaments		(EnumerableObject<UserTournament>* pListTournaments);

	// Attributes.
	bool				IsLoggedIn				()								{return m_bLoggedIn;};
	bool				isRegisteredInTournament(int nTournamentId);
	_String				GetUserName				()								{return m_sUserName;};
	INT_CHIPS			GetBalance				()								{return m_nBalance;};
	INT_CHIPS			GetInPlay				()								{return m_nInPlay;};
	INT_CHIPS			GetPlayBalance			()								{return m_nPlayBalance;};
	double				GetVPPoints				()								{return m_dVPPPoints;};
	int					GetRank					()								{return (int)m_btRank;};
	int					GetUserId				()								{return (IsLoggedIn() ? m_nUserId : 0);};

	bool				SetAvatarId				(int nAvatarId);
	int					GetAvatarId				(){return m_nAvatarId;};
	Image*				GetAvatarImage			(){return (m_avatarImage.IsNull() ? NULL : &m_avatarImage);};
	_String				GetAvatarImageFileById	(int nAvatarId);
	void				SetAvatarCacheFolder	(_String sFolder){m_sAvatarsCacheFolder = sFolder;}
	void				SetAvatarUpdaterProc	(CObject* pReceiver, AvatarUpdateProc proc){ m_pAvatarUpdateReceiver = pReceiver; m_avatarUpdateProc = proc; };

protected: // Virtual events.

	// Server response events.
public:
	void				OnLogin						(LoginPacket* p, void* pParam);
	void				OnLogout					(LogoutPacket* p, void* pParam);

protected:
	void				OnOpenTable					(PokerOpenTablePacket* p, void* pParam);
	void				OnLeaveTable				(PokerLeaveTablePacket* p, void* pParam);
	void				OnSeatAt					(PokerSeatAtTablePacket* p, void* pParam);
	void				OnAddChips					(PokerAddChipsPacket* p, void* pParam);
	void				OnReserveSeat				(PokerReserveSeatPacket* p, void* pParam);
	void				OnJoinWaitingList			(PokerJoinWaitingListPacket* p, void* pParam);
	void				OnAction					(PokerActionPacket* p, void* pParam);
	void				OnTableInfo					(PokerTableInfoPacket* p, void* pParam);

	void				OnReturnBack				(PokerReturnBackPacket* p, void* pParam);
	void				OnSitOut					(PokerSitOutPacket* p, void* pParam);
	void				OnPostBlind					(PokerPostBlindPacket* p, void* pParam);

	void				OnSetActiveSeat				(PokerSetActivePacket* p, void* pParam);
	void				OnSeatStatusChanged			(PokerSeatStatusChangedPacket* p, void* pParam);

	void				OnHandStarted				(PokerHandStartedPacket* p, void* pParam);
	void				OnHandFinished				(PokerHandFinishedPacket* p, void* pParam);
	void				OnRoundStarted				(PokerRoundStartedPacket* p, void* pParam);
	void				OnAct						(PokerActPacket* p, void* pParam);

	void				OnGetLobbyInfo				(PokerGetLobbyInfoPacket* p, void* pParam);

	void				OnShowMuckCards				(PokerShowMuckCardsPacket* p, void* pParam);
	void				OnSetOption					(PokerSetOptionPacket* p, void* pParam);
	void				OnSeatOnlineChanged			(PokerSeatOnlineChangedPacket* p, void* pParam);
	void				OnGetHandHistory			(PokerGetHandHistoryPacket* p, void* pParam);
	void				OnQuitClient				(PokerQuitClientPacket* p, void* pParam);
	void				OnSetAvatar					(PokerSetAvatarPacket* p, void* pParam);
	void				OnUploadAvatar				(PokerUploadAvatarPacket* p, void* pParam);
	void				OnDownloadAvatar			(PokerDownloadAvatarPacket* p, void* pParam);
	void				OnChatMessage				(PokerChatMessagePacket* p, void* pParam);
	void				OnMoneyTransfer				(PokerMoneyTransferPacket* p, void* pParam);
	void				OnResetPlaymoney			(PokerResetPlaymoneyPacket* p, void* pParam);
	void				OnRefreshBalance			(PokerRefreshBalancePacket* p, void* pParam);
	void				OnFindPlayer				(PokerFindPlayerPacket* p, void* pParam);
	void				OnFindTournament			(PokerFindTournamentPacket* p, void* pParam);

	// Tournament
//	void				OnCreateTournament			(PokerCreateTournamentPacket* p, void* pParam);
//	void				OnCancelTournament			(PokerCancelTournamentPacket* p, void* pParam);
	void				OnTournamentCustomInfo		(PokerTournamentCustomInfoPacket* p, void* pParam);
	void				OnRegisterInTournament		(PokerRegisterInTournamentPacket* p, void* pParam);
	void				OnUnregisterFromTournament	(PokerUnregisterFromTournamentPacket* p, void* pParam);
	void				OnOpenTable_Tournament		(PokerOpenTable_TournamentPacket* p, void* pParam);
	void				OnLeaveTableTournament		(PokerLeaveTable_TournamentPacket* p, void* pParam);
	void				OnWaitFor					(PokerWaitForPacket* p, void* pParam);
	void				OnTournamentFinished		(PokerTournamentFinishedPacket* p, void* pParam);
	void				OnRebuy						(PokerRebuyPacket* p, void* pParam);
	void				OnAddon						(PokerAddonPacket* p, void* pParam);

protected:
	volatile INT_CHIPS								m_nBalance;							// User's balance amount.
	volatile INT_CHIPS								m_nPlayBalance;						// User's Play balance amount.
	volatile INT_CHIPS								m_nPlayMoneyInPlayBalance;			// User's playmoney inplay balance.
	volatile INT_CHIPS								m_nInPlay;							// User's InPlay amount.
	volatile double									m_dVPPPoints;						// User's VPP points.
	volatile int									m_nTicketsCt;						// Tournament tickets count.
	BYTE											m_btRank;							// User rank index.
	EnumerableObject<UserTournament>				m_listUserTournaments;				// User tournaments.
	_String											m_sAvatarsCacheFolder;				// Avatars cache folder.
	int												m_nAvatarId;						// User avatar id.
	Image											m_avatarImage;						// Avatar image.

	// Avatar uploading.
	CCriticalSection								m_lockUpload;
	GrowableMemory									m_avatarUploading;					// Avatar uploading.
	CObject*										m_pUploadEventReceiver;				// Upload event receiver object.
	UploadProgressProc								m_procProgress;						// Upload progress proc;
	UploadProgressProc								m_procComplete;						// Upload complete proc;
	UploadProgressProc								m_procError;						// Upload error proc;

	// Avatar downloading attributes.
	CCriticalSection								m_lockDownload;						// Avatar downloading.
	GrowableMemory									m_avatarDownloading;				// Downloading avatar data.
	AutoSortedArray									m_arrUserIdAvatarDownload;			// User id's to download avatars. <UserId><AvatarId>

	AvatarUpdateProc								m_avatarUpdateProc;					// Avatar update proc.
	CObject*										m_pAvatarUpdateReceiver;			// Avatar update receiver.

	// Lobby info cache.
	AutoSortedArrayInt64							m_arrStaticCashTablesByGameType;	// EnumerableObject<LobbyCashTableStaticInfo>* by ((GameType << 32)|GameSubType)					
	AutoSortedArrayInt64							m_arrTournamentsByGameType;			// EnumerableObject<LobbyTournamentInfo>* by ((GameType << 32)|GameSubType)					
	PokerGetLobbyInfoPacket							m_lastResponse;						// Last getlobby info response. 
	CCriticalSection								m_lockCache;
};
