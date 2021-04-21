#include "PokerClient.h"
#include "..\MD5\MD5Checksum.h"
#include "..\Path\Path.h"
#include "..\Utility\GrowableMemory.h"


///////////////////////////////////////////////////////////
///
///	class PokerClient
///
///////////////////////////////////////////////////////////

PokerClient::PokerClient(HWND hOwnerWnd /*= NULL*/, std::string sIpAddress /*= _T("127.0.0.1")*/, int nPort /*= 20001*/, int nReconnectTime /*= 5000*/) :  
	GameClient(hOwnerWnd, sIpAddress, nPort, nReconnectTime), m_avatarUploading(0, 0, false), m_avatarDownloading(0, 1024) {

	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTableInfo,			new PokerTableInfoPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerOpenTable,			new PokerOpenTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerLeaveTable,		new PokerLeaveTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTableClosed,		new PokerTableClosedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSeatAt,			new PokerSeatAtTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAddChips,			new PokerAddChipsPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerReserveSeat,		new PokerReserveSeatPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerCancelReservation,	new PokerCancelReservationPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerJoinWaitingList,	new PokerJoinWaitingListPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAction,			new PokerActionPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTableInfo,			new PokerTableInfoPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSitOut,			new PokerSitOutPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerPostBlind,			new PokerPostBlindPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerReturnBack,		new PokerReturnBackPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSetActive,			new PokerSetActivePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSeatStatusChanged,	new PokerSeatStatusChangedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerHandStarted,		new PokerHandStartedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRoundStarted,		new PokerRoundStartedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAct,				new PokerActPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerHandFinished,		new PokerHandFinishedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerGetLobbyInfo,		new PokerGetLobbyInfoPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerShowMuckCards,		new PokerShowMuckCardsPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSetOption,			new PokerSetOptionPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerReturnBack,		new PokerReturnBackPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSeatOnlineChanged,	new PokerSeatOnlineChangedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerGetHandHistoryPacket,	new PokerGetHandHistoryPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerQuitClient,		new PokerQuitClientPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSetAvatar,			new PokerSetAvatarPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerUploadAvatar,		new PokerUploadAvatarPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerDownloadAvatar,	new PokerDownloadAvatarPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerChatMessage,		new PokerChatMessagePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerChatMessage,		new PokerChatMessagePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerMoneyTransfer,		new PokerMoneyTransferPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerResetPlaymoney,	new PokerResetPlaymoneyPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRefreshBalance,	new PokerRefreshBalancePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerFindPlayer,		new PokerFindPlayerPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerFindTournament,	new PokerFindTournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerOpenRandomTable,	new PokerOpenRandomTablePacket()); // Not needs reply method

	// Tournament
//	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerCreateTournament,			new PokerCreateTournamentPacket()); 
//	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerCancelTournament,			new PokerCancelTournamentPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTournamentCustomInfo,		new PokerTournamentCustomInfoPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRegisterInTournament,		new PokerRegisterInTournamentPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerUnregisterFromTournament,	new PokerUnregisterFromTournamentPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerOpenTable_Tournament,		new PokerOpenTable_TournamentPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerLeaveTable_Tournament,		new PokerLeaveTable_TournamentPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRedirect,					new PokerRedirectPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerLevelChanged,				new PokerLevelChangedPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerBreak,						new PokerBreakPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerWaitFor,					new PokerWaitForPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTournamentFinished,		new PokerTournamentFinishedPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRebuy,						new PokerRebuyPacket()); 
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAddon,						new PokerAddonPacket()); 

	// Initialize packet listener methods. ###############################
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Login, this, (PacketListenerMethod)&PokerClient::OnLogin);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Logout, this, (PacketListenerMethod)&PokerClient::OnLogout);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerOpenTable, this, (PacketListenerMethod)&PokerClient::OnOpenTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerLeaveTable, this, (PacketListenerMethod)&PokerClient::OnLeaveTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerReserveSeat, this, (PacketListenerMethod)&PokerClient::OnReserveSeat);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatAt, this, (PacketListenerMethod)&PokerClient::OnSeatAt);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAddChips, this, (PacketListenerMethod)&PokerClient::OnAddChips);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerJoinWaitingList, this, (PacketListenerMethod)&PokerClient::OnJoinWaitingList);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAddChips, this, (PacketListenerMethod)&PokerClient::OnAddChips);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAction, this, (PacketListenerMethod)&PokerClient::OnAction);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerTableInfo, this, (PacketListenerMethod)&PokerClient::OnTableInfo);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerReturnBack, this, (PacketListenerMethod)&PokerClient::OnReturnBack);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSitOut, this, (PacketListenerMethod)&PokerClient::OnSitOut);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerPostBlind, this, (PacketListenerMethod)&PokerClient::OnPostBlind);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetActive, this, (PacketListenerMethod)&PokerClient::OnSetActiveSeat);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatStatusChanged, this, (PacketListenerMethod)&PokerClient::OnSeatStatusChanged);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerHandStarted, this, (PacketListenerMethod)&PokerClient::OnHandStarted);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRoundStarted, this, (PacketListenerMethod)&PokerClient::OnRoundStarted);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAct, this, (PacketListenerMethod)&PokerClient::OnAct);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerHandFinished, this, (PacketListenerMethod)&PokerClient::OnHandFinished);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetLobbyInfo, this, (PacketListenerMethod)&PokerClient::OnGetLobbyInfo);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerShowMuckCards, this, (PacketListenerMethod)&PokerClient::OnShowMuckCards);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetOption, this, (PacketListenerMethod)&PokerClient::OnSetOption);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatOnlineChanged, this, (PacketListenerMethod)&PokerClient::OnSeatOnlineChanged);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetHandHistoryPacket, this, (PacketListenerMethod)&PokerClient::OnGetHandHistory);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerQuitClient, this, (PacketListenerMethod)&PokerClient::OnQuitClient);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetAvatar, this, (PacketListenerMethod)&PokerClient::OnSetAvatar);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerUploadAvatar, this, (PacketListenerMethod)&PokerClient::OnUploadAvatar);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerDownloadAvatar, this, (PacketListenerMethod)&PokerClient::OnDownloadAvatar);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerChatMessage, this, (PacketListenerMethod)&PokerClient::OnChatMessage);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerMoneyTransfer, this, (PacketListenerMethod)&PokerClient::OnMoneyTransfer);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerResetPlaymoney, this, (PacketListenerMethod)&PokerClient::OnResetPlaymoney);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRefreshBalance, this, (PacketListenerMethod)&PokerClient::OnRefreshBalance);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerFindPlayer, this, (PacketListenerMethod)&PokerClient::OnFindPlayer);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerFindTournament, this, (PacketListenerMethod)&PokerClient::OnFindTournament);

	// Tournament packet handlers.
//	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerCreateTournament, this, (PacketListenerMethod)&PokerClient::OnCreateTournament);
//	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerCancelTournament, this, (PacketListenerMethod)&PokerClient::OnCancelTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerTournamentCustomInfo, this, (PacketListenerMethod)&PokerClient::OnTournamentCustomInfo);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRegisterInTournament, this, (PacketListenerMethod)&PokerClient::OnRegisterInTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerUnregisterFromTournament, this, (PacketListenerMethod)&PokerClient::OnUnregisterFromTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerOpenTable_Tournament, this, (PacketListenerMethod)&PokerClient::OnOpenTable_Tournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerLeaveTable_Tournament, this, (PacketListenerMethod)&PokerClient::OnLeaveTableTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerWaitFor, this, (PacketListenerMethod)&PokerClient::OnWaitFor);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerTournamentFinished, this, (PacketListenerMethod)&PokerClient::OnTournamentFinished);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRebuy, this, (PacketListenerMethod)&PokerClient::OnRebuy);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAddon, this, (PacketListenerMethod)&PokerClient::OnAddon);
	// ###################################################################

	m_bLoggedIn				= false;
	m_sUserName				= _T("");
	m_nBalance				= 0;
	m_nPlayBalance			= 0;
	m_nInPlay				= 0;
	m_dVPPPoints			= 0.0;
	m_btRank				= 0;
	m_nAvatarId				= 0;

	m_pUploadEventReceiver	= NULL;
	m_procProgress			= NULL;
	m_procComplete			= NULL;
	m_procError				= NULL;

	m_pAvatarUpdateReceiver = NULL; 
	m_avatarUpdateProc		= NULL;

	// Create variable index.
	m_listUserTournaments.CreateVariableIndex(_T("TournamentId"), Serializable::Compare_Int32);
	} 

PokerClient::~PokerClient(){
	ClearCache();
	// Discard uploading.
	DiscardUploadingAvatar	();
	// Discard downloading.
	DiscardDownloadingAvatar();
	m_avatarImage.Destroy();
	}

///////////////////////////////////////////////////////////
///
///	Public server operations.
///
///////////////////////////////////////////////////////////

void
PokerClient::OpenTable(int nTableId, bool bSeatAtOnce /*= false*/, int nSeatIndex /*= -1*/){
	PokerOpenTablePacket* packet	= new PokerOpenTablePacket();
	int nCode						= packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode					= nCode;
	packet->m_nTableId				= nTableId;
	packet->m_bSeatOrJoinWaitList	= bSeatAtOnce;

	if(bSeatAtOnce && nSeatIndex > -1 && nSeatIndex < 10 )
		packet->m_cSeat	= (char)nSeatIndex;

	SendPacket(packet);
	delete packet;
	}

void
PokerClient::LeaveTable(int nTableId){
	PokerLeaveTablePacket* packet	= new PokerLeaveTablePacket();
	int nCode						= packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode					= nCode;
	packet->m_nTableId				= nTableId;
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::ReserveSeat(int nTableId, int nSeatIndex){
	PokerReserveSeatPacket* packet = new PokerReserveSeatPacket();
	int nCode				= packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode			= nCode;
	packet->m_nTableId		= nTableId;
	packet->m_cSeat			= (char)nSeatIndex;
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::CancelReservation(int nTableId, int nSeatIndex){
	PokerCancelReservationPacket* packet = new PokerCancelReservationPacket();
	int nCode				= packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode			= nCode;
	packet->m_nTableId		= nTableId;
	packet->m_cSeat			= (char)nSeatIndex;
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::SeatAt(int nTableId, int nSeatIndex, int nAmount){
	PokerSeatAtTablePacket* packet = new PokerSeatAtTablePacket();
	int nCode				= packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode			= nCode;
	packet->m_nTableId		= nTableId;
	packet->m_cSeatIndex	= (char)nSeatIndex;
	packet->m_nChipAmount	= nAmount;
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::JoinWaitingList(int nTableId){
	PokerJoinWaitingListPacket* packet = new PokerJoinWaitingListPacket();
	int nCode				= packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode			= nCode;
	packet->m_nTableId		= nTableId;
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::Request_AddChips(int nTableId){
	PokerAddChipsPacket* packet = new PokerAddChipsPacket();
	int nCode					= packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode				= nCode;
	packet->m_nTableId			= nTableId;
	packet->m_bPrepareToAdd		= true; // Add chips
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::AddChips(int nTableId, int nChipAmount){
	PokerAddChipsPacket* packet = new PokerAddChipsPacket();
	int nCode = packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode				= nCode;
	packet->m_nTableId			= nTableId;
	packet->m_nAmount			= nChipAmount;
	packet->m_bPrepareToAdd		= false; // Add chips
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::Action(int nTableId, int nTournamentId, int nSeat, PokerActionTypes action, int nAmount){
	PokerActionPacket* packet = new PokerActionPacket();
	int nCode = packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode			= nCode;
	packet->m_nTableId		= nTableId;
	packet->m_nTournamentId	= nTournamentId;
	packet->m_cSeat			= (char)nSeat;
	packet->m_cActionType	= (char)action;
	packet->m_nAmount		= nAmount;
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::GetTableInfo(int nTableId, int nTournamentId){
	if( !nTournamentId ){
		PokerTableInfoPacket* p = new PokerTableInfoPacket();
		int nCode = p->m_nCode;
		p->ZeroInit();
		p->m_nCode		= nCode;
		p->m_nTableId	= nTableId;
		SendPacket(p);
		delete p;
		}
	else{
		PokerTournamentTableInfoPacket* p = new PokerTournamentTableInfoPacket();
		int nCode			= p->m_nCode;
		p->ZeroInit();
		p->m_nCode			= nCode;
		p->m_nTableId		= nTableId;
		p->m_nTournamentId	= nTournamentId;
		SendPacket(p);
		delete p;
		}
	}

void
PokerClient::GetLobbyInfo(int nMoneyType, int nGameType, int nSubType, int nRequestFlags /*= PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesStaticInfo|PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesDynamicInfo*/){
	PokerGetLobbyInfoPacket* packet = new PokerGetLobbyInfoPacket();
	int nCode = packet->m_nCode;
	packet->ZeroInit();
	packet->m_nCode			= nCode;
	packet->m_cRequestFlag	= (char)nRequestFlags;
	packet->m_cMoneyType	= (char)nMoneyType;
	packet->m_cGameType		= (BYTE)nGameType;
	packet->m_shSubType		= (short)nSubType;
	SendPacket(packet);
	delete packet;
	}

void
PokerClient::ShowMuckCards(int nTableId, int nTournamentId, bool bShowCards){
	PokerShowMuckCardsPacket* p = new PokerShowMuckCardsPacket();
	int nCode = p->m_nCode;
	p->ZeroInit();
	p->m_nCode					= nCode;
	p->m_nTableId				= nTableId;
	p->m_nTournamentId			= nTournamentId;
	p->m_bShowCards				= bShowCards;
	SendPacket(p);
	delete p;
	}

void
PokerClient::SetOption(int nTableId, int nTournamentId, PokerTableOption option, bool bValue, int nValue /*= 0*/, int nSeatIndex /*= 0*/){
	PokerSetOptionPacket* p		= new PokerSetOptionPacket();
	int nCode = p->m_nCode;
	p->ZeroInit();
	p->m_nCode					= nCode;
	p->m_nTableId				= nTableId;
	p->m_nTournamentId			= nTournamentId;
	p->m_cOption				= (char)option;
	p->m_bValue					= bValue;
	p->m_nValue					= nValue;
	p->m_cSeat					= (char)nSeatIndex;
	SendPacket(p);
	delete p;
	}

void
PokerClient::ReturnBack(int nTableId, int nTournamentId, int nSeat){
	PokerReturnBackPacket* p	= new PokerReturnBackPacket();
	int nCode = p->m_nCode;
	p->ZeroInit();
	p->m_nCode					= nCode;
	p->m_nTableId				= nTableId;
	p->m_nTournamentId			= nTournamentId;
	p->m_cSeat					= (char)nSeat;
	p->m_bReturnBack			= true;
	SendPacket(p);
	delete p;
	}

void
PokerClient::GetHandHistory(__int64 nHandId, int nTournamentId){
	PokerGetHandHistoryPacket* p	= new PokerGetHandHistoryPacket();
	int nCode = p->m_nCode;
	p->ZeroInit();
	p->m_nCode						= nCode;
	p->m_nHandId					= nHandId;
	p->m_nTournamentId				= nTournamentId;
	SendPacket(p);
	delete p;
	}

bool
PokerClient::UploadAvatarImageFile(std::string sImageFile, void* pReceiver, UploadProgressProc procProgress, UploadProgressProc procComplete, UploadProgressProc procError){
	Image image;
	if( IsAvatarUploadStarted() )
		return false;
	if( !image.LoadImage32bpp(sImageFile) )
		return false;
	/*if( image.GetWidth() != 100 && image.GetHeight() != 64 )
		return false;*/
	return UploadAvatarImage(&image, pReceiver, procProgress, procComplete, procError);
	}

bool
PokerClient::UploadAvatarImage(Image* pImgAvatar, void* pReceiver, UploadProgressProc procProgress, UploadProgressProc procComplete, UploadProgressProc procError){
	LongBinary imageData;
	if( !pImgAvatar || pImgAvatar->IsNull()  )
		return false;

	if( !pImgAvatar->EncodeImageData(&imageData, Image::ImageAsJPG) )
		return false;

	int nSize = (int)imageData.GetBinarySize();
	void* lpImageBuffer = imageData.LockMemory();
	m_avatarUploading.SetBufferSize(nSize);
	BYTE* pBuffer = m_avatarUploading.GetBufferPtr();
	memcpy(pBuffer, lpImageBuffer, nSize);
	imageData.UnlockMemory();

	m_lockUpload.Lock(); // Lock
	m_pUploadEventReceiver	= pReceiver;
	m_procProgress			= procProgress;
	m_procComplete			= procComplete;
	m_procError				= procError;

	// Start uploading avatar image. {{
	PokerUploadAvatarPacket packet;
	int nCode = packet.m_nCode;
	packet.ZeroInit();
	packet.m_nCode = nCode;

	packet.m_shSize				= (short)min(PokerClient::AvatarUploadChunkSize, nSize);
	packet.m_nFullBinarySize	= nSize;
	packet.m_nAvatarId			= 0;
	packet.m_nUploaded			= 0;
	m_lockUpload.Unlock(); // Unlock
	packet.m_imageData.SetBinarySize((UINT)packet.m_shSize, pBuffer);
	SendPacket(&packet);
	// }}
	return true;
	}

bool
PokerClient::IsAvatarUploadStarted(){
	m_lockUpload.Lock(); // Lock
	bool bRet = (m_avatarUploading.GetBufferSize() > 0);
	m_lockUpload.Unlock(); // Unlock
	return bRet;
	}

bool
PokerClient::IsAvatarUploadCompleted(){
	m_lockUpload.Lock(); // Lock
	bool bRet = false;
	if( m_avatarUploading.GetBufferSize() )
		bRet = (m_avatarUploading.GetBufferSize() == m_avatarUploading.GetUsedBufferSize());
	m_lockUpload.Unlock(); // Unlock
	return bRet;
	}

bool
PokerClient::DiscardUploadingAvatar(){
	m_lockUpload.Lock(); // Lock
	m_avatarUploading.SetBufferSize(0);
	m_pUploadEventReceiver	= NULL;
	m_procProgress			= NULL;
	m_procComplete			= NULL;
	m_procError				= NULL;
	m_lockUpload.Unlock(); // Unlock
	return true;
	}

bool
PokerClient::DownloadAvatarImageFile(int nUserId, int nAvatarId){
	if( IsAvatarDownloadStarted() )
		return true;

	m_lockDownload.Lock(); // Lock
	PokerDownloadAvatarPacket p;
	p.m_nUserId			= nUserId;
	p.m_nAvatarId		= nAvatarId;
	p.m_shSize			= 0;
	p.m_nDownloaded		= false;
	p.m_nFullBinarySize = 0;
	p.m_bCompleted		= false;
	bool bRet = SendPacket(&p);
	m_avatarDownloading.SetBufferSize(0);
	m_lockDownload.Unlock(); // Unlock
	return bRet;
	}

bool
PokerClient::IsAvatarDownloadStarted(){
	m_lockDownload.Lock(); // Lock
	bool bRet = (m_avatarDownloading.GetBufferSize() > 0);
	m_lockDownload.Unlock(); // Unlock
	return bRet;
	}

bool
PokerClient::IsAvatarDownloadCompleted(){
	m_lockDownload.Lock(); // Lock
	bool bRet = false;
	if( m_avatarDownloading.GetBufferSize() )
		bRet = (m_avatarDownloading.GetBufferSize() == m_avatarDownloading.GetUsedBufferSize());
	m_lockDownload.Unlock(); // Unlock
	return bRet;
	}

bool
PokerClient::DiscardDownloadingAvatar(){
	m_lockDownload.Lock(); // Lock
	m_avatarDownloading.SetBufferSize(0);
	m_lockDownload.Unlock(); // Unlock
	return true;
	}

bool
PokerClient::Rebuy(int nTableId, int nTournamentId){
	if( nTableId <=0 || nTournamentId <= 0 || !IsLoggedIn() )
		return false;
	PokerRebuyPacket* p		= new PokerRebuyPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	p->m_nTableId			= nTableId;
	p->m_nTournamentId		= nTournamentId;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::Addon(int nTableId, int nTournamentId){
	if( nTableId <=0 || nTournamentId <= 0 || !IsLoggedIn() )
		return false;
	PokerAddonPacket* p		= new PokerAddonPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	p->m_nTableId			= nTableId;
	p->m_nTournamentId		= nTournamentId;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::SendChatMessage(int nTableId, int nTournamentId, std::string sMessage){
	if( !IsConnected() || !IsLoggedIn() || !sMessage.length() )
		return false;

	PokerChatMessagePacket* p	= new PokerChatMessagePacket();
	int nCode					= p->m_nCode;
	p->ZeroInit();
	p->m_nCode					= nCode;
	p->m_nTableId				= nTableId;
	p->m_nTournamentId			= nTournamentId;
	p->m_nDestUserId			= -1;
	p->m_message.m_bSystem		= false;
	p->m_message.m_actionType	= PokerActionTypes::NoAction;
	p->m_message.m_cSeat		= (char)-1; // Is not neccesary when client sends packet to server.
	p->m_message.m_nAmount		= 0;
	p->m_message.m_sMessage		= sMessage;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::TransferMoney(int nAccountFrom, int nAccountTo, int nAmount, bool bRefreshBalances){
	PokerMoneyTransferPacket* p = new PokerMoneyTransferPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	p->m_nTransAmount		= nAmount;
	p->m_nAccountFrom		= nAccountFrom;
	p->m_nAccountTo			= nAccountTo;
	p->m_nBalanceFrom		= 0;
	p->m_nBalanceTo			= 0;
	p->m_bRefreshBalances	= bRefreshBalances;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::ResetPlayBalance(){
	PokerResetPlaymoneyPacket* p = new PokerResetPlaymoneyPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::RefreshBalance(){
	PokerRefreshBalancePacket* p = new PokerRefreshBalancePacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::FindPlayer(std::string sUserName){
	PokerFindPlayerPacket* p = new PokerFindPlayerPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	p->m_sUserName			= sUserName;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::FindPlayerByUserId(int nUserId){
	PokerFindPlayerPacket* p = new PokerFindPlayerPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	p->m_nUserId			= nUserId;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::FindTournament(std::string sTournamentID){
	PokerFindTournamentPacket* p = new PokerFindTournamentPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	p->m_sTournamentID		= sTournamentID;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

bool
PokerClient::RefreshLobbyInfo(int nGameType, int nGameSubType, int nSelectedTableId, int nSecRefreshAfter /*= -1*/){
	PokerGetLobbyInfoPacket* p	= new PokerGetLobbyInfoPacket();
	int nCode					= p->m_nCode;
	p->ZeroInit();
	p->m_nCode					= nCode;

	p->m_cMoneyType				= MoneyType::Real;
	p->m_cGameType				= (BYTE)nGameType;
	p->m_shSubType				= (short)nGameSubType;
	// Playmoney tables flag.
	if( (nGameType&GameType::Playmoney) != 0 )
		p->m_cMoneyType = MoneyType::Play;

	int nGameTypeSent			= (int)m_lastResponse.m_cGameType;
	int nGameSubTypeSent		= (int)m_lastResponse.m_shSubType;
	int nTablesCtByReq			= (int)m_lastResponse.m_shTablesCtByRequest;
	int nTournamentsCtByReq		= (int)m_lastResponse.m_shTournamentsCtByRequest;

	if( nSelectedTableId > 0 ){
		p->m_cRequestFlag		|= PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablePlayers;
		p->m_nTableId			= nSelectedTableId;
		}
	else
		p->m_nTableId			= 0;

	if( nGameType&GameType::Tournament ){
		// Request for complete tournament info. {{
		int nTournamentsCt = 0, nSubTypesNotCached = 0;
		IsTournamentsCached(nGameType, nGameSubType, nTournamentsCt, nSubTypesNotCached);
		if( nSubTypesNotCached )
			p->m_cRequestFlag	|= PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentInfos; 
		// }}
		// Tournaments count is changed.
		if( nGameTypeSent&GameType::Tournament && nGameSubTypeSent == nGameSubType && nTournamentsCt != nTournamentsCtByReq )
			p->m_cRequestFlag	|= PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentInfos; 
		// Tournament dynamic info request.
		if( !p->m_cRequestFlag )
			p->m_cRequestFlag	|= PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentDynamicInfos;
		}
	
	if( nGameType&(GameType::Omaha|GameType::TexasHoldem)  ){
		p->m_cRequestFlag		|= 	PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesDynamicInfo /*| PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablePlayers*/;
		// Refresh static info of cash tables. {{
		int nTableCt = 0, nSubTypesNotCached = 0;
		IsTablesCached(nGameType, nGameSubType, nTableCt, nSubTypesNotCached);
		if( nSubTypesNotCached )
			p->m_cRequestFlag	|= PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesStaticInfo; 
		// }}
		// Tables count changed. {{
		if( nGameTypeSent&(GameType::Omaha|GameType::TexasHoldem) && nGameSubTypeSent == nGameSubType && nTableCt != nTablesCtByReq )
			p->m_cRequestFlag	|= PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesStaticInfo; 
		// }}
		}

	if( !p->m_cRequestFlag ){
		delete p;
		return false;
		}

	SendPacket(p);
	delete p;
	return true;
	}

EnumerableObject<LobbyCashTableStaticInfo>*
PokerClient::GetCachedTables(int nGameType, int nGameSubType, int& nIndex){
	EnumerableObject<LobbyCashTableStaticInfo>* pStaticTablesCash = NULL;
	__int64 nValue = MAKEINT64(nGameSubType, nGameType);
	nIndex = m_arrStaticCashTablesByGameType.IndexOf(nValue);
	if( nIndex != -1 ){
		pStaticTablesCash = (EnumerableObject<LobbyCashTableStaticInfo>*)m_arrStaticCashTablesByGameType.GetData(nIndex);
		if( !pStaticTablesCash )
			m_arrStaticCashTablesByGameType.Delete(nIndex);
		}
	return pStaticTablesCash;
	}

EnumerableObject<LobbyTournamentInfo>*
PokerClient::GetCachedTournaments(int nGameType, int nGameSubType, int& nIndex){
	EnumerableObject<LobbyTournamentInfo>* pTournaments = NULL;
	__int64 nValue = MAKEINT64(nGameSubType, nGameType);
	nIndex = m_arrTournamentsByGameType.IndexOf(nValue);
	if( nIndex != -1 ){
		pTournaments = (EnumerableObject<LobbyTournamentInfo>*)m_arrTournamentsByGameType.GetData(nIndex);
		if( !pTournaments )
			m_arrTournamentsByGameType.Delete(nIndex);
		}
	return pTournaments;
	}

bool
PokerClient::UpdateTablesCache(PokerGetLobbyInfoPacket* p){
	if( !p )  return false;
	int nReqFlags = (int)p->m_cRequestFlag;
	if( !(nReqFlags&PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesStaticInfo) )
		return false;

	m_lockCache.Lock(); // Lock
	int	nGameType = (int)p->m_cGameType, nGameSubType = (int)p->m_shSubType, nIndex = -1;
	
	const int nArrGameTypes[]			= {GameType::Omaha, GameType::TexasHoldem};
	const int nArrPlaymoneyGameTypes[]	= {(GameType::Omaha|GameType::Playmoney), (GameType::TexasHoldem|GameType::Playmoney)};
	const int nArrGameSubTypes[][3]		= {{GameSubType::Omaha_Omaha, GameSubType::Omaha_OmahaHiLo, 0}, {GameSubType::Holdem_NoLimit, GameSubType::Holdem_PotLimit, GameSubType::Holdem_FixedLimit}};

	const int* pArrGameTypes			= (p->m_cMoneyType == MoneyType::Play) ? nArrPlaymoneyGameTypes : nArrGameTypes;
	const int nGameTypesCt				= sizeof(nArrGameTypes)/sizeof(int);

	EnumerableObject<LobbyCashTableStaticInfo>* pStaticTablesCash = NULL;

	for(int i=0; i<nGameTypesCt; i++){
		if( (nGameType&pArrGameTypes[i]) != pArrGameTypes[i] ) continue;
		for(int j=0; j<3; j++){
			if( nGameSubType&nArrGameSubTypes[i][j] ){
				int nGameSubTypeFilter	= (nGameSubType&nArrGameSubTypes[i][j]);
				pStaticTablesCash		= GetCachedTables(pArrGameTypes[i], nGameSubTypeFilter, nIndex);
				if( pStaticTablesCash )
					pStaticTablesCash->RemoveAll(true);
				else{
					pStaticTablesCash = new EnumerableObject<LobbyCashTableStaticInfo>();
					m_arrStaticCashTablesByGameType.Add(MAKEINT64(nGameSubTypeFilter, pArrGameTypes[i]), (__int64)pStaticTablesCash);
					}

				for(int nLoop=0; nLoop<p->m_listCashTablesStaticInfo.GetCount(); nLoop++){
					LobbyCashTableStaticInfo* pInfo = p->m_listCashTablesStaticInfo.GetAt(nLoop);
					if( pInfo && (pInfo->m_cGameType == (BYTE)pArrGameTypes[i]) && (pInfo->m_shGameSubType == nGameSubTypeFilter) ){
						LobbyCashTableStaticInfo* pInfoNew = new LobbyCashTableStaticInfo();
						pInfo->Copy(pInfoNew);
#ifndef _SERVER
						LobbyCashTableDynamicInfo* pDynamicInfo = p->m_listCashTablesDynamicInfo.FindOneVariable(_T("TableId"), &pInfoNew->m_nTableId);
						if( pDynamicInfo ){
							pInfo->m_cPlayerCt			= pDynamicInfo->m_cPlayerCt;
							pInfo->m_cPlayerFlopPercent = pDynamicInfo->m_cPlayerFlopPercent;
							pInfo->m_cWaitedPlayersCt	= pDynamicInfo->m_cWaitedPlayersCt;
							pInfo->m_nAvgPotAmount		= pDynamicInfo->m_nAvgPotAmount;
							pInfo->m_shHandPerHour		= pDynamicInfo->m_shHandPerHour;
							}
						else{
							pInfo->m_cPlayerCt			= 0;
							pInfo->m_cPlayerFlopPercent = 0;
							pInfo->m_cWaitedPlayersCt	= 0;
							pInfo->m_nAvgPotAmount		= 0;
							pInfo->m_shHandPerHour		= 0;
							}
#endif
						pStaticTablesCash->Add(pInfoNew);
						}
					}
				}
			}
		}
	m_lockCache.Unlock(); //Unlock
	return true;
	}

bool
PokerClient::UpdateTournamentsCache(PokerGetLobbyInfoPacket* p){
	if( !p )  return false;
	int nReqFlags		= (int)p->m_cRequestFlag;
	if( !(nReqFlags&(PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentInfos | PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentDynamicInfos)) )
		return false;

	m_lockCache.Lock(); //Lock
	int				nGameType		= (int)p->m_cGameType;
	int				nGameSubType	= (int)p->m_shSubType;
	int				nIndex			= -1;
	EnumerableObject<LobbyTournamentInfo>* pTournaments = NULL;
	bool			bDynamicInfo	= (nReqFlags & PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentDynamicInfos) > 0 ? true : false;

	const int nArrGameTypes[]		= {GameType::Tournament};
	const int nArrGameSubTypes[][4] = {{GameSubType::Tournament_Freeroll, GameSubType::Tournament_Guaranteed, GameSubType::Tournament_Regular, GameSubType::Tournament_SitNGo}};

	for(int i=0; i<sizeof(nArrGameTypes)/sizeof(int); i++){
		if( !(nGameType&nArrGameTypes[i]) ) continue;

		// Update tournament dynamic info.
		if( bDynamicInfo ){
			for(int j=0; j<4; j++){
				if( nGameSubType&nArrGameSubTypes[i][j] ){
					pTournaments = GetCachedTournaments(nArrGameTypes[i], nArrGameSubTypes[i][j], nIndex);
					if( !pTournaments )
						continue;

					for(int nLoop=0; nLoop<p->m_listTournamentDynamicInfo.GetCount(); nLoop++){
						LobbyTournamentDynamicInfo* pDynInfo = p->m_listTournamentDynamicInfo.GetAt(nLoop);
						if( !pDynInfo ) continue;

						LobbyTournamentInfo* pInfo = pTournaments->FindOneVariable(_T("TournamentId"), &pDynInfo->m_nTournamentId);
						if( pInfo ){
							pInfo->m_status						= pDynInfo->m_status;
							pInfo->m_dtTimeTournamentFinished	= pDynInfo->m_dtTimeTournamentFinished;
							pInfo->m_nPlayersCt					= pDynInfo->m_nPlayersCt;
							pInfo->m_nRegisteredCt				= pDynInfo->m_nRegisteredCt;
							pInfo->m_shTableCt					= pDynInfo->m_shTableCt;
							}
						}
					}
				}
			}
		else{
			for(int j=0; j<4; j++){
				if( nGameSubType&nArrGameSubTypes[i][j] ){
					pTournaments = GetCachedTournaments(nArrGameTypes[i], nArrGameSubTypes[i][j], nIndex);
					if( pTournaments )
						pTournaments->RemoveAll(true);
					else{
						pTournaments = new EnumerableObject<LobbyTournamentInfo>();
						pTournaments->CreateVariableIndex(_T("TournamentId"), &Serializable::Compare_Int32);
						m_arrTournamentsByGameType.Add(MAKEINT64(nArrGameSubTypes[i][j], nArrGameTypes[i]), (__int64)pTournaments);
						}

					for(int nLoop=0; nLoop<p->m_listTournamentInfo.GetCount(); nLoop++){
						LobbyTournamentInfo* pInfo = p->m_listTournamentInfo.GetAt(nLoop);
						if( !pInfo ) continue;
						int nSubTypeTournament = (pInfo->m_shSubType&(GameSubType::Tournament_Freeroll|GameSubType::Tournament_Guaranteed|GameSubType::Tournament_Regular|GameSubType::Tournament_SitNGo));
						if( nSubTypeTournament == nArrGameSubTypes[i][j]){
							LobbyTournamentInfo* pInfoNew = new LobbyTournamentInfo();
							pInfo->Copy(pInfoNew);
							pTournaments->Add(pInfoNew);
							}
						}
					}
				}
			}
		}
	m_lockCache.Unlock(); //Unlock
	return true;
	}

bool
PokerClient::IsTablesCached(int nGameType, int nGameSubType, int& nTablesCt, int& nGameSubTypeNotCached){
	m_lockCache.Lock(); //Lock
	nTablesCt							= 0;
	const int nArrGameTypes[]			= {GameType::Omaha, GameType::TexasHoldem};
	const int nArrPlaymoneyGameTypes[]	= {(GameType::Omaha|GameType::Playmoney), (GameType::TexasHoldem|GameType::Playmoney)};
	const int nArrGameSubTypes[][3]		= { {GameSubType::Omaha_Omaha, GameSubType::Omaha_OmahaHiLo, 0}, 
											{GameSubType::Holdem_NoLimit, GameSubType::Holdem_PotLimit, GameSubType::Holdem_FixedLimit}};
	int nIndex							= 0;
	nGameSubTypeNotCached				= nGameSubType;

	const int* pArrGameTypes			= (nGameType&GameType::Playmoney) ? nArrPlaymoneyGameTypes : nArrGameTypes;
	const int  nGameTypesCt				= sizeof(nArrGameTypes)/sizeof(int);

	for(int i=0; i<nGameTypesCt; i++){
		if( (nGameType&pArrGameTypes[i]) != pArrGameTypes[i] ) continue;
		for(int j=0; j<3; j++){
			if( nGameSubType&nArrGameSubTypes[i][j] ){
				EnumerableObject<LobbyCashTableStaticInfo>* pStaticTablesCash = GetCachedTables(pArrGameTypes[i], nArrGameSubTypes[i][j], nIndex);
				if( pStaticTablesCash ){
					nGameSubTypeNotCached &= (~nArrGameSubTypes[i][j]);
					nTablesCt += pStaticTablesCash->GetCount();
					}
				}
			}
		}
	m_lockCache.Unlock(); //Unlock
	return (nGameSubTypeNotCached == 0);
	}

bool
PokerClient::IsTournamentsCached(int nGameType, int nGameSubType, int& nTournamentsCt, int& nGameSubTypeNotCached){
	m_lockCache.Lock(); //Lock
	nTournamentsCt					= 0;
	const int nArrGameTypes[]		= {GameType::Tournament};
	const int nArrGameSubTypes[][4] = {{GameSubType::Tournament_Freeroll, GameSubType::Tournament_Guaranteed, GameSubType::Tournament_Regular, GameSubType::Tournament_SitNGo}};
	int nIndex						= 0;
	nGameSubTypeNotCached			= nGameSubType;

	for(int i=0; i<sizeof(nArrGameTypes)/sizeof(int); i++){
		if( !(nGameType&nArrGameTypes[i]) ) continue;
		for(int j=0; j<4; j++){
			if( nGameSubType&nArrGameSubTypes[i][j] ){
				EnumerableObject<LobbyTournamentInfo>* pTournaments = GetCachedTournaments(nArrGameTypes[i], nArrGameSubTypes[i][j], nIndex);
				if( pTournaments ){
					nGameSubTypeNotCached &= (~nArrGameSubTypes[i][j]);
					nTournamentsCt += pTournaments->GetCount();
					}
				}
			}
		}
	m_lockCache.Unlock(); //Unlock
	return (nGameSubTypeNotCached == 0);
	}

void
PokerClient::ClearCache(){
	// Clear cache. {{
	m_lockCache.Lock(); // Lock
	for(int i=0; i<m_arrStaticCashTablesByGameType.GetCount(); i++){
		EnumerableObject<LobbyCashTableStaticInfo>*pTables = (EnumerableObject<LobbyCashTableStaticInfo>*)m_arrStaticCashTablesByGameType.GetData(i);
		if( pTables ){
			pTables->RemoveAll(true);
			delete pTables;
			}
		}
	m_arrStaticCashTablesByGameType.DeleteAll();

	for(int i=0; i<m_arrTournamentsByGameType.GetCount(); i++){
		EnumerableObject<LobbyTournamentInfo>*pTournaments = (EnumerableObject<LobbyTournamentInfo>*)m_arrTournamentsByGameType.GetData(i);
		if( pTournaments ){
			pTournaments->RemoveAll(true);
			delete pTournaments;
			}
		}
	m_arrTournamentsByGameType.DeleteAll();
	m_lockCache.Unlock(); // Unlock
	// }}
	}

int
PokerClient::CopyCachedTables(int nGameType, int nGameSubType, EnumerableObject<LobbyCashTableStaticInfo>* pListTables){
	if( !pListTables ) return 0;
	const int nArrGameTypes[]			= {GameType::Omaha, GameType::TexasHoldem};
	const int nArrPlaymoneyGameTypes[]	= {(GameType::Omaha|GameType::Playmoney), (GameType::TexasHoldem|GameType::Playmoney)};
	const int nArrGameSubTypes[][3]		= { {GameSubType::Omaha_Omaha, GameSubType::Omaha_OmahaHiLo, 0}, 
											{GameSubType::Holdem_NoLimit, GameSubType::Holdem_PotLimit, GameSubType::Holdem_FixedLimit}};
	int nIndex							= 0;
	const int* pArrGameTypes			= (nGameType&GameType::Playmoney) ? nArrPlaymoneyGameTypes : nArrGameTypes;
	const int  nGameTypesCt				= sizeof(nArrGameTypes)/sizeof(int);

	m_lockCache.Lock(); //Lock
	for(int i=0; i<nGameTypesCt; i++){
		if( (nGameType&pArrGameTypes[i]) != pArrGameTypes[i] ) continue;
		for(int j=0; j<3; j++){
			if( nGameSubType&nArrGameSubTypes[i][j] ){
				EnumerableObject<LobbyCashTableStaticInfo>* pStaticTablesCash = GetCachedTables(pArrGameTypes[i], nArrGameSubTypes[i][j], nIndex);
				if( pStaticTablesCash ){
					for(int nLoop=0; nLoop<pStaticTablesCash->GetCount(); nLoop++){
						LobbyCashTableStaticInfo* pInfo = pStaticTablesCash->GetAt(nLoop);
						if( pInfo ){
							LobbyCashTableStaticInfo* pInfoNew = new LobbyCashTableStaticInfo();
							pInfo->Copy(pInfoNew);
							pListTables->Add(pInfoNew);
							}
						}
					}
				}
			}
		}
	m_lockCache.Unlock(); //Unlock
	return pListTables->GetCount();
	}

int
PokerClient::CopyCachedTournaments(int nGameType, int nGameSubType, EnumerableObject<LobbyTournamentInfo>* pListTournaments){
	if( !pListTournaments ) return 0;

	const int nArrGameTypes[]		= {GameType::Tournament};
	const int nArrGameSubTypes[][4] = {{GameSubType::Tournament_Freeroll, GameSubType::Tournament_Guaranteed, GameSubType::Tournament_Regular, GameSubType::Tournament_SitNGo}};
	int nIndex						= 0;

	m_lockCache.Lock(); //Lock
	for(int i=0; i<sizeof(nArrGameTypes)/sizeof(int); i++){
		if( !(nGameType&nArrGameTypes[i]) ) continue;
		for(int j=0; j<4; j++){
			if( nGameSubType&nArrGameSubTypes[i][j] ){
				EnumerableObject<LobbyTournamentInfo>* pTournamentsCached = GetCachedTournaments(nArrGameTypes[i], nArrGameSubTypes[i][j], nIndex);
				if( pTournamentsCached ){
					for(int nLoop=0; nLoop<pTournamentsCached->GetCount(); nLoop++){
						LobbyTournamentInfo* pInfo = pTournamentsCached->GetAt(nLoop);
						if( pInfo ){
							LobbyTournamentInfo* pInfoNew = new LobbyTournamentInfo();
							pInfo->Copy(pInfoNew);
							pListTournaments->Add(pInfoNew);
							}
						}
					}
				}
			}
		}
	m_lockCache.Unlock(); //Unlock
	return pListTournaments->GetCount();
	}

bool
PokerClient::GetTournamentInfo(int nTournamentId, LobbyTournamentInfo* pInfo){
	m_lockCache.Lock(); //Lock
	for(int i=0; i<m_arrTournamentsByGameType.GetCount(); i++){
		EnumerableObject<LobbyTournamentInfo>* pTournamentsCached = (EnumerableObject<LobbyTournamentInfo>*)m_arrTournamentsByGameType.GetData(i);
		if( pTournamentsCached && pTournamentsCached->GetCount() > 0 ){
			for(int j=0; j<pTournamentsCached->GetCount(); j++){
				LobbyTournamentInfo* pTournamentInfo = pTournamentsCached->GetAt(j);
				if( pTournamentInfo && pTournamentInfo->m_nTournamentId == nTournamentId ){
					pTournamentInfo->Copy(pInfo);
					m_lockCache.Unlock(); //Unlock
					return true;
					}
				}
			}
		}
	m_lockCache.Unlock(); //Unlock
	return false;
	}

bool
PokerClient::GetTournamentLobbyInfo(int nTournamentId, int nFlags, int nGameTableIndex, int nGameTableCt, int nParticipantIndex, int nParticipantCt, int nGameTableIndexPlayers){
	if( !IsConnected() || nTournamentId <= 0 || !nFlags )
		return false;

	PokerTournamentCustomInfoPacket* p	= new PokerTournamentCustomInfoPacket();
	int nCode							= p->m_nCode;
	p->ZeroInit();
	p->m_nCode							= nCode;
	p->m_nTournamentId					= nTournamentId;
	p->m_nFlags							= nFlags;
	p->m_shGameTableIndex				= (short)nGameTableIndex;
	p->m_shGameTableCt					= (short)nGameTableCt;
	p->m_shParticipantIndex				= (short)nParticipantIndex;
	p->m_shParticipantCt				= (short)nParticipantCt;
	p->m_shGameTableIndexPlayers		= (short)nGameTableIndexPlayers;

	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

void
PokerClient::OpenTable_Tournament(int nTournamentId, int nTableId){
	PokerOpenTable_TournamentPacket* p = new PokerOpenTable_TournamentPacket();
	int nCode			= p->m_nCode;
	p->ZeroInit();
	p->m_nCode			= nCode;
	p->m_nTournamentId	= nTournamentId;
	p->m_nTableId		= nTableId;
	p->m_bLookForMyTable= false;
	SendPacket(p);
	delete p;
	}

void
PokerClient::LeaveTable_Tournament(int nTournamentId, int nTableId){
	PokerLeaveTable_TournamentPacket* p = new PokerLeaveTable_TournamentPacket();
	int nCode			= p->m_nCode;
	p->ZeroInit();
	p->m_nCode			= nCode;
	p->m_nTournamentId	= nTournamentId;
	p->m_nTableId		= nTableId;
	SendPacket(p);
	delete p;
	}

bool
PokerClient::OpenMyTableInTournament(int nTournamentId){
	PokerOpenTable_TournamentPacket* p = new PokerOpenTable_TournamentPacket();
	int nCode				= p->m_nCode;
	p->ZeroInit();
	p->m_nCode				= nCode;
	p->m_nTournamentId		= nTournamentId;
	p->m_nTableId			= 0;
	p->m_bLookForMyTable	= true;
	bool bRet = SendPacket(p);
	delete p;
	return bRet;
	}

void
PokerClient::RegisterInTournament(int nTournamentId, bool bByBalanceAmount /*= true*/, bool bByPoints /*= false*/, bool bByTicket /*= false*/){
	PokerRegisterInTournamentPacket* p = new PokerRegisterInTournamentPacket();
	int nCode			= p->m_nCode;
	p->ZeroInit();
	p->m_nCode			= nCode;
	p->m_nTournamentId	= nTournamentId;
	p->m_cRegisterBy	= 0;
	p->m_nBalance		= 0;
	p->m_dPoint			= 0.0;
	p->m_cTicketCt		= 0;
	if( bByBalanceAmount )
		p->m_cRegisterBy = PokerRegisterInTournamentPacket::RegisterBy_BalanceAmount;
	if( bByPoints )
		p->m_cRegisterBy = PokerRegisterInTournamentPacket::RegisterBy_Points;
	if( bByTicket )
		p->m_cRegisterBy = PokerRegisterInTournamentPacket::RegisterBy_Ticket;
	SendPacket(p);
	delete p;
	}

void
PokerClient::UnregisterFromTournament(int nTournamentId){
	PokerUnregisterFromTournamentPacket* p = new PokerUnregisterFromTournamentPacket();
	int nCode			= p->m_nCode;
	p->ZeroInit();
	p->m_nCode			= nCode;
	p->m_nTournamentId	= nTournamentId;
	p->m_nBalance		= 0;
	p->m_dPoints		= 0.0;
	SendPacket(p);
	delete p;
	}

///////////////////////////////////////////////////////////
///
///	Public server response events.
///
///////////////////////////////////////////////////////////

void
PokerClient::OnLogin(LoginPacket* p, void* pParam){
	ClientConnection* pConn = (ClientConnection*)pParam;
	if( p->m_nErrorCode != ES_ERROR_SUCCESS )
		return;

	// Save new session.
	if( p->m_bPrevSessionRestored )
		m_nSessionId = p->m_nSessionId;

	m_secLock1.Lock(); // Lock
	
	m_listUserTournaments.RemoveAll(true);
	p->m_listRegTournaments.Copy_Into(&m_listUserTournaments);
	m_secLock1.Unlock(); // Unlock

	m_bLoggedIn		= true;
	m_nUserId		= p->m_nUserId;
	m_sUserName		= p->m_sUserName;
	m_nBalance		= p->m_nBalance;
	m_nPlayBalance	= p->m_nPlayBalance;
	m_nAvatarId		= p->m_nAvatarId;
	m_nInPlay		= 0;
	m_dVPPPoints	= p->m_dVPPoints;
	m_btRank		= p->m_btRank;
	SetAvatarId(p->m_nAvatarId);
	}

void
PokerClient::OnLogout(LogoutPacket* p, void* pParam){
	ClientConnection* pConn = (ClientConnection*)pParam;
	m_secLock1.Lock();		// Lock
	m_listUserTournaments.RemoveAll(true);
	m_secLock1.Unlock();	// Unlock

	m_bLoggedIn		= false;
	m_sUserName		= _T("");
	m_nBalance		= 0;
	m_nPlayBalance	= 0;
	m_nInPlay		= 0;
	m_dVPPPoints	= 0.0;
	m_btRank		= 0;
	m_nAvatarId		= 0;
	m_avatarImage.Destroy();
	}

void	
PokerClient::OnOpenTable(PokerOpenTablePacket* p, void* pParam){
	}

void
PokerClient::OnLeaveTable(PokerLeaveTablePacket* p, void* pParam){
	}

void
PokerClient::OnReserveSeat(PokerReserveSeatPacket* p, void* pParam){
	}

void
PokerClient::OnSeatAt(PokerSeatAtTablePacket* p, void* pParam){
	}

void
PokerClient::OnAddChips(PokerAddChipsPacket* p, void* pParam){
	}

void
PokerClient::OnJoinWaitingList(PokerJoinWaitingListPacket* p, void* pParam){
	}

void
PokerClient::OnAction(PokerActionPacket* p, void* pParam){
	}

void
PokerClient::OnTableInfo(PokerTableInfoPacket* p, void* pParam){	
	}

void
PokerClient::OnReturnBack(PokerReturnBackPacket* p, void* pParam){
	}

void
PokerClient::OnSitOut(PokerSitOutPacket* p, void* pParam){
	}

void
PokerClient::OnPostBlind(PokerPostBlindPacket* p, void* pParam){
	}

void
PokerClient::OnSetActiveSeat(PokerSetActivePacket* p, void* pParam){
	}

void
PokerClient::OnSeatStatusChanged(PokerSeatStatusChangedPacket* p, void* pParam){
	}

void
PokerClient::OnHandStarted(PokerHandStartedPacket* p, void* pParam){
	}

void
PokerClient::OnRoundStarted(PokerRoundStartedPacket* p, void* pParam){
	}

void
PokerClient::OnAct(PokerActPacket* p, void* pParam){
	}

void
PokerClient::OnHandFinished(PokerHandFinishedPacket* p, void* pParam){
	}

void
PokerClient::OnGetLobbyInfo(PokerGetLobbyInfoPacket* p, void* pParam){
	// Update cache.
	UpdateTablesCache		(p);
	UpdateTournamentsCache	(p);

	m_nBalance		= p->m_nBalance;
	m_nPlayBalance	= p->m_nPlayBalance;
	m_nInPlay		= p->m_nInPlayAmount;
	
	m_lastResponse.ZeroInit();
	m_lastResponse.m_cGameType					= p->m_cGameType;
	m_lastResponse.m_shSubType					= p->m_shSubType;
	m_lastResponse.m_cRequestFlag				= p->m_cRequestFlag;
	m_lastResponse.m_shTournamentsCtByRequest	= p->m_shTournamentsCtByRequest;
	m_lastResponse.m_shTablesCtByRequest		= p->m_shTablesCtByRequest;
	}

void
PokerClient::OnShowMuckCards(PokerShowMuckCardsPacket* p, void* pParam){
	}

void
PokerClient::OnSetOption(PokerSetOptionPacket* p, void* pParam){
	}

void
PokerClient::OnSeatOnlineChanged(PokerSeatOnlineChangedPacket* p, void* pParam){
	}

void
PokerClient::OnGetHandHistory(PokerGetHandHistoryPacket* p, void* pParam){
	}

void
PokerClient::OnQuitClient(PokerQuitClientPacket* p, void* pParam){
	}

void
PokerClient::OnSetAvatar(PokerSetAvatarPacket* p, void* pParam){
	}

void
PokerClient::OnUploadAvatar(PokerUploadAvatarPacket* p, void* pParam){
	if( !IsAvatarUploadStarted() )
		return;

	m_lockUpload.Lock(); // Lock
	if( !p->m_bCompleted ){
		if( m_procProgress )
			(m_procProgress)(m_pUploadEventReceiver, p);
		}
	else{
		if( m_procComplete )
			(m_procComplete)(m_pUploadEventReceiver, p);

		// Call update method.
		if( m_avatarUpdateProc )
			(m_avatarUpdateProc)(m_pAvatarUpdateReceiver, GetUserId(), p->m_nAvatarId);
		}
	m_lockUpload.Unlock(); // Unlock

	if( p->m_bCompleted )
		DiscardUploadingAvatar();
	else{ // Upload next part.
		m_lockUpload.Lock(); // Lock
		BYTE*	pBuffer		= m_avatarUploading.GetCurrentOffsetPtr();
		int		nBufferSize = m_avatarUploading.GetBufferSize();
		if( nBufferSize != p->m_nFullBinarySize || p->m_nUploaded >= nBufferSize ){
			if( m_procError )
				(m_procError)(m_pUploadEventReceiver, p);
			DiscardUploadingAvatar();
			return;
			}

		BYTE* pBufferUpload			= &pBuffer[p->m_nUploaded];
		// Continue uploading avatar image. {{
		PokerUploadAvatarPacket packet;
		int nCode = packet.m_nCode;
		packet.ZeroInit();
		packet.m_nCode				= nCode;
		packet.m_shSize				= (short)min(PokerClient::AvatarUploadChunkSize, (p->m_nFullBinarySize - p->m_nUploaded));
		packet.m_nFullBinarySize	= p->m_nFullBinarySize;
		packet.m_nAvatarId			= 0;
		packet.m_nUploaded			= p->m_nUploaded;
		m_lockUpload.Unlock(); // Unlock
		packet.m_imageData.SetBinarySize((UINT)packet.m_shSize, pBufferUpload);
		SendPacket(&packet);
		// }}
		}
	}

bool
PokerClient::DownloadMultipleAvatars(AutoSortedArray* pArrUserIdByAvatarId){
	m_lockDownload.Lock(); // Lock
	int nCtAvatarsToDownload = 0;
	for(int i=0; i<pArrUserIdByAvatarId->GetCount(); i++){
		int nAvatarId	= (int)pArrUserIdByAvatarId->Get(i);
		int nUserId		= (int)pArrUserIdByAvatarId->GetData(i);
		if( nAvatarId > 0 && nUserId > 0 && !AvatarExists(nAvatarId) ){
			if( m_arrUserIdAvatarDownload.IndexOf(pArrUserIdByAvatarId->GetData(i)) == -1 ){
				m_arrUserIdAvatarDownload.Add(pArrUserIdByAvatarId->GetData(i), pArrUserIdByAvatarId->Get(i));
				nCtAvatarsToDownload ++;
				}
			}
		}
	m_lockDownload.Unlock(); // Unlock

	// Start download avatars.
	if( nCtAvatarsToDownload > 0 && !IsAvatarDownloadStarted() ){
		int nAvatarId	= (int)m_arrUserIdAvatarDownload.GetData(0);
		int nUserId		= (int)m_arrUserIdAvatarDownload.Get(0);
		DownloadAvatarImageFile(nUserId, nAvatarId);
		}
	return true;
	}

bool
PokerClient::AvatarExists(int nAvatarId){
	std::string sAvatarFile = GetAvatarImageFileById(nAvatarId);
	return Path::FileExists(sAvatarFile.c_str());
	}

void
PokerClient::OnDownloadAvatar(PokerDownloadAvatarPacket* p, void* pParam){
	m_lockDownload.Lock();		// Lock
	if( m_avatarDownloading.GetBufferSize() )
		m_avatarDownloading.SetBufferSize(p->m_nFullBinarySize);

	// Wromng avatar id has been sent so update avatar id and send download request again.
	if( p->m_bCompleted && !p->m_nFullBinarySize ){
		int nIndex = m_arrUserIdAvatarDownload.IndexOf((void*)p->m_nUserId);
		if( nIndex != -1 ){
			int nAvatarIdSent = (int)m_arrUserIdAvatarDownload.GetData(nIndex);
			if( nAvatarIdSent != p->m_nAvatarId ){
				// Update avatar id.
				m_arrUserIdAvatarDownload.SetData(nIndex, (void*)p->m_nAvatarId);

				bool bAvatarExists = AvatarExists(p->m_nAvatarId);
				if( !bAvatarExists ){
					m_lockDownload.Unlock();	// Unlock
					SendPacket(p);
					}
				else{
					// Call update method.
					if( m_avatarUpdateProc )
						(m_avatarUpdateProc)(m_pAvatarUpdateReceiver, p->m_nUserId, p->m_nAvatarId);

					m_arrUserIdAvatarDownload.Delete(nIndex);

					// Download next one.
					if( m_arrUserIdAvatarDownload.GetCount() > 0 ){
						int nAvatarId = (int)m_arrUserIdAvatarDownload.GetData(0);
						int nUserId = (int)m_arrUserIdAvatarDownload.Get(0);
						DownloadAvatarImageFile(nUserId, nAvatarId);
						}
					m_lockDownload.Unlock();	// Unlock
					}
				return;
				}
			}
		}

	// Add image data bytes.
	if( p->m_imageData.GetBinarySize() > 0 ){
		BYTE* lpImageData = (BYTE*)p->m_imageData.LockMemory();
		m_avatarDownloading.AddBytes(lpImageData, (int)p->m_imageData.GetBinarySize());
		p->m_imageData.UnlockMemory();
		}

	if( !p->m_bCompleted ){
		m_lockDownload.Unlock();	// Unlock
		p->m_imageData.SetBinarySize(0);
		p->m_shSize = 0;
		SendPacket(p);
		}
	else{
		// Save avatar image.
		std::string sImageFile = GetAvatarImageFileById(p->m_nAvatarId);
		Path::SaveAsFile(m_avatarDownloading.GetBufferPtr(), (DWORD)m_avatarDownloading.GetBufferSize(), sImageFile);	

		// Call update method.
		if( m_avatarUpdateProc )
			(m_avatarUpdateProc)(m_pAvatarUpdateReceiver, p->m_nUserId, p->m_nAvatarId);

		m_avatarDownloading.SetBufferSize(0);
		int nIndex = m_arrUserIdAvatarDownload.IndexOf((void*)p->m_nUserId);
		if( nIndex != -1 )
			m_arrUserIdAvatarDownload.Delete(nIndex);

		// Download next one.
		if( m_arrUserIdAvatarDownload.GetCount() > 0 ){
			int nAvatarId = (int)m_arrUserIdAvatarDownload.GetData(0);
			int nUserId = (int)m_arrUserIdAvatarDownload.Get(0);
			DownloadAvatarImageFile(nUserId, nAvatarId);
			}
		m_lockDownload.Unlock();	// Unlock
		}
	}

void
PokerClient::OnChatMessage(PokerChatMessagePacket* p, void* pParam){
	}

void
PokerClient::OnMoneyTransfer(PokerMoneyTransferPacket* p, void* pParam){
	}

void
PokerClient::OnResetPlaymoney(PokerResetPlaymoneyPacket* p, void* pParam){
	m_nPlayBalance = p->m_nPlayMoneyAmount;
	}

void
PokerClient::OnRefreshBalance(PokerRefreshBalancePacket* p, void* pParam){
	m_nPlayBalance = p->m_nPlayMoneyAmount;
	m_nBalance = p->m_nBalance;
	}

void
PokerClient::OnFindPlayer(PokerFindPlayerPacket* p, void* pParam){
	}

void
PokerClient::OnFindTournament(PokerFindTournamentPacket* p, void* pParam){
	}

/////////////////////////////////////////////////////////////////////////
//
// Tournament
//

void
PokerClient::OnTournamentCustomInfo(PokerTournamentCustomInfoPacket* p, void* pParam){
	}

void
PokerClient::OnRegisterInTournament(PokerRegisterInTournamentPacket* p, void* pParam){
	if( p->m_nErrorCode == ES_ERROR_SUCCESS ){
		// Add registered tournament id.
		m_secLock1.Lock();		// Lock
		UserTournament* pTournament = m_listUserTournaments.FindOneVariable(_T("TournamentId"), &p->m_nTournamentId);
		if( !pTournament ){
			if( p->m_listRegTournament.GetCount() > 0 ){
				pTournament = p->m_listRegTournament.GetAt(0);
				if( pTournament ){
					m_listUserTournaments.Add(pTournament);
					p->m_listRegTournament.RemoveAt(0, false);
					}
				}
			}
		m_secLock1.Unlock();	// Unlock
		}
	}

void
PokerClient::OnUnregisterFromTournament(PokerUnregisterFromTournamentPacket* p, void* pParam){
	if( p->m_nErrorCode == ES_ERROR_SUCCESS ){
		// Remove from registered list.
		m_secLock1.Lock();		// Lock
		UserTournament* pTournament = m_listUserTournaments.FindOneVariable(_T("TournamentId"), &p->m_nTournamentId);
		if( pTournament )
			m_listUserTournaments.Remove(pTournament, true);
		m_secLock1.Unlock();	// Unlock
		}
	}

void
PokerClient::OnOpenTable_Tournament(PokerOpenTable_TournamentPacket* p, void* pParam){
	}

void
PokerClient::OnLeaveTableTournament(PokerLeaveTable_TournamentPacket* p, void* pParam){
	}

void
PokerClient::OnWaitFor(PokerWaitForPacket* p, void* pParam){
	}

void
PokerClient::OnTournamentFinished(PokerTournamentFinishedPacket* p, void* pParam){
	// Remove from registered list.
	m_secLock1.Lock();		// Lock
	UserTournament* pTournament = m_listUserTournaments.FindOneVariable(_T("TournamentId"), &p->m_nTournamentId);
	if( pTournament )
		m_listUserTournaments.Remove(pTournament, true);
	m_secLock1.Unlock();	// Unlock
	}

void
PokerClient::OnRebuy(PokerRebuyPacket* p, void* pParam){
	if( p->m_nErrorCode == ES_ERROR_SUCCESS ){
		m_nBalance = p->m_nBalanceNew;
		}
	}

void
PokerClient::OnAddon(PokerAddonPacket* p, void* pParam){
	if( p->m_nErrorCode == ES_ERROR_SUCCESS ){
		m_nBalance = p->m_nBalanceNew;
		}
	}

bool
PokerClient::SetAvatarId(int nAvatarId){
	int nAvatarOld = m_nAvatarId;
	m_nAvatarId = nAvatarId;
	m_avatarImage.Destroy();

	std::string sAvatarFile;
	if( nAvatarOld != m_nAvatarId){
		// Remove old avatar image file. {{
		sAvatarFile = GetAvatarImageFileById(nAvatarOld);
		DeleteFile(sAvatarFile.c_str());
		// }}
		}
	sAvatarFile = GetAvatarImageFileById(nAvatarId);
	return (m_avatarImage.LoadImage32bpp(sAvatarFile)?true:false);
	}

std::string
PokerClient::GetAvatarImageFileById(int nAvatarId){
	std::string sNameCrypt;
	stringEx::Format(sNameCrypt, _T("avatar_id=%d"), nAvatarId);
	std::string sMD5 = CMD5Checksum::GetMD5((BYTE*)sNameCrypt.c_str(), sNameCrypt.length());
	std::string sAvatarFile;
	stringEx::Format(sAvatarFile, _T("%s\\%s.jpg"),m_sAvatarsCacheFolder.c_str(), sMD5.c_str());
	Path::BuildFullPath(sAvatarFile);
	return sAvatarFile;
	}

int
PokerClient::CopyMyTournaments(EnumerableObject<UserTournament>* pListTournaments){
	if( !pListTournaments ) return 0;
	m_secLock1.Lock();		// Lock
	m_listUserTournaments.Copy_Into(pListTournaments);
	int nRet = m_listUserTournaments.GetCount();
	m_secLock1.Unlock();	// Unlock
	return nRet;
	}

bool
PokerClient::isRegisteredInTournament(int nTournamentId){
	bool bRet = false;
	m_secLock1.Lock();		// Lock
	bRet = (m_listUserTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId) != NULL);
	m_secLock1.Unlock();	// Unlock
	return bRet;
	}