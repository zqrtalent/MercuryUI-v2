#include "stdafx.h"
#include "../GameServer/ClientConnection.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"
#include "../MD5/MD5Checksum.h"
#include "../MD5/McbDES2.hpp"
#include "../WebService/HTTPClient.h"
#include "MaximumWebService.h"

#include "Log/Logger.h"
#include "PokerServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerServer
///
///////////////////////////////////////////////////////////

PokerServer::PokerServer() : GameServer()
	{	
	m_connByTableId			.OptimizeForLargeData(TRUE);
	m_tableControllerByConn	.OptimizeForLargeData(TRUE);
	m_pData					= NULL;
	m_pPokerGameConfig		= NULL;

	// Add packet by code to be known by system.
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerGetLobbyInfo,				new PokerGetLobbyInfoPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerGetHandHistoryPacket,		new PokerGetHandHistoryPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerQuitClient,				new PokerQuitClientPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSetAvatar,					new PokerSetAvatarPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerUploadAvatar,				new PokerUploadAvatarPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerDownloadAvatar,			new PokerDownloadAvatarPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerChatMessage,				new PokerChatMessagePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerMoneyTransfer,				new PokerMoneyTransferPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerResetPlaymoney,			new PokerResetPlaymoneyPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRefreshBalance,			new PokerRefreshBalancePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerOpenTable,					new PokerOpenTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerLeaveTable,				new PokerLeaveTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSeatAt,					new PokerSeatAtTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAddChips,					new PokerAddChipsPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerReserveSeat,				new PokerReserveSeatPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerCancelReservation,			new PokerCancelReservationPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerJoinWaitingList,			new PokerJoinWaitingListPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAction,					new PokerActionPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTableInfo,					new PokerTableInfoPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSitOut,					new PokerSitOutPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerPostBlind,					new PokerPostBlindPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerReturnBack,				new PokerReturnBackPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSetActive,					new PokerSetActivePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSeatStatusChanged,			new PokerSeatStatusChangedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerHandStarted,				new PokerHandStartedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRoundStarted,				new PokerRoundStartedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAct,						new PokerActPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerHandFinished,				new PokerHandFinishedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerShowMuckCards,				new PokerShowMuckCardsPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSetOption,					new PokerSetOptionPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSeatOnlineChanged,			new PokerSeatOnlineChangedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerOpenRandomTable,			new PokerOpenRandomTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTournamentCustomInfo,		new PokerTournamentCustomInfoPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRegisterInTournament,		new PokerRegisterInTournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerUnregisterFromTournament,	new PokerUnregisterFromTournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerOpenTable_Tournament,		new PokerOpenTable_TournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerLeaveTable_Tournament,		new PokerLeaveTable_TournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRebuy,						new PokerRebuyPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerAddon,						new PokerAddonPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRedirect,					new PokerRedirectPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerFindPlayer,				new PokerFindPlayerPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerFindTournament,			new PokerFindTournamentPacket());

	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerTableClosed,				new PokerTableClosedPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerWaitFor,					new PokerWaitForPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerBreak,						new PokerBreakPacket());

	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerGetGameTablesByType,		new PokerGetGameTablesPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerGetTournamentsByType,		new PokerGetTournamentsPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerGetClientConnections,		new PokerGetClientConnectionsPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSendAdminMessageByTable,	new PokerSendAdminMessageByTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerSendAdminMessageByUser,	new PokerSendAdminMessageByUserPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerCreateGameTable,			new PokerCreateGameTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerForceClientSessionToQuit,	new PokerForceClientSessionToQuitPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerDisableServerConnect,		new PokerDisableServerConnectPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerDeactivateGameTable,		new PokerDeactivateGameTablePacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerBlockUnblockUser,			new PokerBlockUserPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerRegisterUserInTournament,	new PokerRegisterUserInTournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerCreateTournament,			new PokerCreateTournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerCancelTournament,			new PokerCancelTournamentPacket());
	m_pPacketHandler->AddPacketObject(PacketCodes::_PokerHideShowTournament,		new PokerHideShowTournamentPacket());

	// Set packet listener methods. ###############################
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Login, this, (PacketListenerMethod)&PokerServer::OnLogin);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Logout, this, (PacketListenerMethod)&PokerServer::OnLogout);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetLobbyInfo, this, (PacketListenerMethod)&PokerServer::OnGetLobbyInfo);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetHandHistoryPacket, this, (PacketListenerMethod)&PokerServer::OnGetHandHistory);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetAvatar, this, (PacketListenerMethod)&PokerServer::OnSetAvatar);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerUploadAvatar, this, (PacketListenerMethod)&PokerServer::OnUploadAvatar);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerDownloadAvatar, this, (PacketListenerMethod)&PokerServer::OnDownloadAvatar);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerChatMessage, this, (PacketListenerMethod)&PokerServer::OnChatMessage);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerMoneyTransfer, this, (PacketListenerMethod)&PokerServer::OnMoneyTransfer);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerResetPlaymoney, this, (PacketListenerMethod)&PokerServer::OnResetPlaymoney);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRefreshBalance, this, (PacketListenerMethod)&PokerServer::OnRefreshBalance);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerOpenRandomTable, this, (PacketListenerMethod)&PokerServer::OnOpenRandomTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerFindPlayer, this, (PacketListenerMethod)&PokerServer::OnFindPlayer);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerFindTournament, this, (PacketListenerMethod)&PokerServer::OnFindTournament);

	// Game table packets.
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerOpenTable, this, (PacketListenerMethod)&PokerServer::OnOpenTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerLeaveTable, this, (PacketListenerMethod)&PokerServer::OnLeaveTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatAt, this, (PacketListenerMethod)&PokerServer::OnSeatAt);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAddChips, this, (PacketListenerMethod)&PokerServer::OnAddChips);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerReserveSeat, this, (PacketListenerMethod)&PokerServer::OnReserveSeat);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerCancelReservation, this, (PacketListenerMethod)&PokerServer::OnCancelReservation);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerJoinWaitingList, this, (PacketListenerMethod)&PokerServer::OnJoinWaitingList);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAction, this, (PacketListenerMethod)&PokerServer::OnAction);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerTableInfo, this, (PacketListenerMethod)&PokerServer::OnTableInfo);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerReturnBack, this, (PacketListenerMethod)&PokerServer::OnReturnBack);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSitOut, this, (PacketListenerMethod)&PokerServer::OnSitOut);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerPostBlind, this, (PacketListenerMethod)&PokerServer::OnPostBlind);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerShowMuckCards, this, (PacketListenerMethod)&PokerServer::OnShowMuckCards);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetOption, this, (PacketListenerMethod)&PokerServer::OnSetOption);

	// Tournaments only
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerTournamentCustomInfo, this, (PacketListenerMethod)&PokerServer::OnTournamentCustomInfo);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRegisterInTournament, this, (PacketListenerMethod)&PokerServer::OnRegister);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerUnregisterFromTournament, this, (PacketListenerMethod)&PokerServer::OnUnregister);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerOpenTable_Tournament, this, (PacketListenerMethod)&PokerServer::OnOpenTable_Tournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerLeaveTable_Tournament, this, (PacketListenerMethod)&PokerServer::OnLeaveTable_Tournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRebuy, this, (PacketListenerMethod)&PokerServer::OnRebuy);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAddon, this, (PacketListenerMethod)&PokerServer::OnAddon);

	// Administration
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerCreateTournament, this, (PacketListenerMethod)&PokerServer::OnCreateTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerCancelTournament, this, (PacketListenerMethod)&PokerServer::OnCancelTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetGameTablesByType, this, (PacketListenerMethod)&PokerServer::OnGetGameTables);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetTournamentsByType, this, (PacketListenerMethod)&PokerServer::OnGetTournaments);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetClientConnections, this, (PacketListenerMethod)&PokerServer::OnGetClientConnections);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSendAdminMessageByTable, this, (PacketListenerMethod)&PokerServer::OnSendAdminMessageByTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSendAdminMessageByUser, this, (PacketListenerMethod)&PokerServer::OnSendAdminMessageByUser);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerCreateGameTable, this, (PacketListenerMethod)&PokerServer::OnCreateGameTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerForceClientSessionToQuit, this, (PacketListenerMethod)&PokerServer::OnForceClientSessionToQuit);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerDisableServerConnect, this, (PacketListenerMethod)&PokerServer::OnDisableServerConnect);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerDeactivateGameTable, this, (PacketListenerMethod)&PokerServer::OnDeactivateGameTable);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerHideShowTournament, this, (PacketListenerMethod)&PokerServer::OnHideShowTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRegisterUserInTournament, this, (PacketListenerMethod)&PokerServer::OnRegisterUserInTournament);
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerBlockUnblockUser, this, (PacketListenerMethod)&PokerServer::OnBlockUser);
	// ############################################################
/*
	// Basic poker server packets handler thread. 
	DataManageThread* pManageThread_Server = m_listDataManageThreads.GetAt(0);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_Login);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_Logout);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_PokerGetLobbyInfo);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_PokerTournamentCustomInfo);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_PokerGetHandHistoryPacket);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_PokerSetAvatar);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_PokerChatMessage);
	SetDataManageThreadPacket(pManageThread_Server, PacketCodes::_PokerOpenRandomTable);

	// Poker game table data packet handle thread.
	DataManageThread* pManageThread_GameTable = new DataManageThread(this);
	AddDataManageThread(pManageThread_GameTable);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerOpenTable);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerLeaveTable);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerSeatAt);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerAddChips);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerReserveSeat);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerCancelReservation);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerJoinWaitingList);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerAction);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerTableInfo);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerReturnBack);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerSitOut);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerPostBlind);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerShowMuckCards);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerSetOption);
	SetDataManageThreadPacket(pManageThread_GameTable, PacketCodes::_PokerSeatOnlineChanged);

	// Poker tournament packets handler thread.
	DataManageThread* pManageThread_Tournament = new DataManageThread(this);
	AddDataManageThread(pManageThread_Tournament);
	SetDataManageThreadPacket(pManageThread_Tournament, PacketCodes::_PokerCreateTournament);
	SetDataManageThreadPacket(pManageThread_Tournament, PacketCodes::_PokerCancelTournament);
	SetDataManageThreadPacket(pManageThread_Tournament, PacketCodes::_PokerRegisterInTournament);
	SetDataManageThreadPacket(pManageThread_Tournament, PacketCodes::_PokerUnregisterFromTournament);
	SetDataManageThreadPacket(pManageThread_Tournament, PacketCodes::_PokerOpenTable_Tournament);
	SetDataManageThreadPacket(pManageThread_Tournament, PacketCodes::_PokerLeaveTable_Tournament);

	// Money transfer handler thread.
	DataManageThread* pManageThread_MoneyTransfer = new DataManageThread(this);
	AddDataManageThread(pManageThread_MoneyTransfer);
	SetDataManageThreadPacket(pManageThread_MoneyTransfer, PacketCodes::_PokerMoneyTransfer);

	// Upload/Download handler thread.
	DataManageThread* pManageThread_UploadDownload = new DataManageThread(this);
	AddDataManageThread(pManageThread_UploadDownload);
	SetDataManageThreadPacket(pManageThread_UploadDownload, PacketCodes::_PokerUploadAvatar);
	SetDataManageThreadPacket(pManageThread_UploadDownload, PacketCodes::_PokerDownloadAvatar);

	// Administration packets handler thread.
	DataManageThread* pManageThread_Administration = new DataManageThread(this);
	AddDataManageThread(pManageThread_Administration);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerGetCashGameTablesByType);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerGetTournamentsByType);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerGetClientConnections);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerSendAdminMessageByTable);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerSendAdminMessageByUser);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerCreateCashTable);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerForceClientSessionToQuit);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerDisableServerConnect);
	SetDataManageThreadPacket(pManageThread_Administration, PacketCodes::_PokerDeactivateCashTable);
*/
	// Initialize cash game and tournament controllers.
	m_cashGames			.Initialize(this, m_pPacketHandler);
	m_tournamentGames	.Initialize(this, m_pPacketHandler);
	} 

PokerServer::~PokerServer(){
	//Stop();
	}

int
PokerServer::GetTablesCt(){return m_lobbyInfo.GetTablesCt();}

int
PokerServer::GetTournamentsCt(){return m_lobbyInfo.GetTournamentsCt();}

int
PokerServer::GetPlayersCt(){return m_lobbyInfo.GetPlayersCt();}

#include "Data/Tables/GMSMarket/GMSMarketDataExporter.h"

BYTE*
DataBlockFromString(_String sDataBlock, int& nLen){
	LPTSTR	lpsz	= sDataBlock.GetBuffer();
	int		nCount	= sDataBlock.GetLength() / 2;
	BYTE*	pData	= nCount > 0 ? new BYTE[nCount] : NULL;
	int		nLoop	= 0;
	_String sz		= "00";

	while( nLoop < nCount )
		{
		sz.SetAt	(0, lpsz[2*nLoop]);
		sz.SetAt	(1, lpsz[2*nLoop + 1]);
		sz.MakeUpper();

		// Convert heximal value to decimal. ######
		int nValue;
		if( sz[0] >= '0' && sz[0] <= '9' )
			nValue = (sz[0] - '0')*16;
		else
		if( sz[0] >= 'A' && sz[0] <= 'F' )
			nValue = (sz[0] - 'A' + 10)*16;
		else
			break;

		if( sz[1] >= '0' && sz[1] <= '9' )
			nValue += sz[1] - '0';
		else
		if( sz[1] >= 'A' && sz[1] <= 'F' )
			nValue += sz[1] - 'A' + 10;
		else
			break;
		// ########################################
		
		pData[nLoop]= (BYTE)nValue;
		nLoop ++;
		}

	nLen = nLoop;
	return pData;
	}

bool
PokerServer::Start(){
	//PokerServerConfig config;
	/*config.SerializeAsXML(_T("Config11.xml"), TRUE);
	///*config.m_sIp			= _T("192.168.1.124");
	config.m_nPort			= 20001;
	config.m_sSQLConnect	= _T("Driver={SQL Server Native Client 10.0};Server=localhost;Database=ESPokerDB; Trusted_Connection=yes;");
	config.SerializeAsXML(_T("Config.xml"), TRUE);*/
/*
	// Temp
	GMSMarketDataExporter exp;
	exp.ExportInExcell();
	return false;
	// Temp
*/
	/*
	int nLen = 0;
	_String sData = "CE57213B0E0299D4F7E3C7636CBACEF3CF5A335FB1CC80010482CA421424905E17CFD53057133CF7";
	sData = "CE57213B0E0299D4D49A07898825F3725F36CD76F2E0A23C4A727308E4D9519E34B60DE0B0189A69";
	//CE57213B0E0299D495FB5B12793DC83DFE3FF4C6118992A4E3C275885B1C73D983EB57DC6CE36495
	BYTE * bb = DataBlockFromString(sData, nLen);
	
	McbDES desDecrypt;
	desDecrypt.McbSetKey1((unsigned char*)"teoeligiviko");
	desDecrypt.McbSetKey2((unsigned char*)"1cf2fefd"); // Old
	//desDecrypt.McbSetKey2((unsigned char*)"80a442d2");
	_String sPwd = "";

	//80a442d2
	// PWD=1cf2fefd;UID=SYSDBA;

	//desDecrypt.McbEncrypt("PWD=80a442d2;UID=SYSDBA;");


	if( desDecrypt.McbDecrypt(bb, nLen) )
	{
		sPwd = desDecrypt.McbGetPlainText();
	}*/
	
	/*
	HTTPClient cl;
	_String sResponse;
	_String sGet = "http://xml.oddservice.com/OS/OddsWebService.svc/GetCountries?email=kid@ucoz.com&password=Newoddservice!&guid=f023f6be-934a-4dbb-8c41-81256b260ffd";
//	_String sGet = "http://xml.oddservice.com/OS/OddsWebService.svc/GetSportEvents?email=kid@ucoz.com&password=Newoddservice!&guid=f023f6be-934a-4dbb-8c41-81256b260ffd";
	cl.GetResponse(sGet, 80, sResponse);
*/
/*
	GrowableMemory mem;
	PokerGetLobbyInfoPacket p;
	p.m_dtLastUpdate.SetDateTime(2011, 9, 11, 23, 30, 15);
	p.Serialize(&mem);

	PokerGetLobbyInfoPacket p1;
	mem.ResetBufferPointerPosition();
	p1.Deserialize(&mem);

	_String s;
	s.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), 
		p1.m_dtLastUpdate.GetYear(),
		p1.m_dtLastUpdate.GetMonth(),
		p1.m_dtLastUpdate.GetDay(),
		p1.m_dtLastUpdate.GetHour(),
		p1.m_dtLastUpdate.GetMinute(),
		p1.m_dtLastUpdate.GetSecond());
		*/

/*
	if( m_config.DeserializeAsXML(_T("Config.xml")) ){
		SetIpAddress(m_config.m_sIp);
		SetPort(m_config.m_nPort);
		m_sockServer.SetMaxConnectionCt(m_config.m_nMaxConnection);
		m_nReceiveDataSizeMax	= m_config.m_nReceiveDataSizeMax;
//		m_nSessionExpireTime	= m_config.m_nSessionExpireTimeSec;
		m_sAvatarsFolder		= m_config.m_sAvatarsFolder;

		// Initialize logging.
		if( m_config.m_bLog ){
			Logger::InitLogging(m_config.m_sLogDir, m_config.m_nMaxLogFileSize);
			}
		}
*/

	/*
	MultiUserIp* p = new MultiUserIp();
	p->m_sIp = _T("192.168.1.124");
	m_config.m_listMultiUserIps.Add(p);
	m_config.SerializeAsXML(_T("111.xml"), TRUE);*/

	/*
	TournamentPlayerInfo* pPlayer = new TournamentPlayerInfo();
	pPlayer->m_cLevel		= 0;
	pPlayer->m_nUserId		= 0;
	pPlayer->m_sUserName.Format(_T("bot%d"), 1);
	pPlayer->m_nChips		= 0xFF11112222;
	pPlayer->m_shPlace		= -1;
	pPlayer->m_nOrder		= 0xFF33332222;	// Need to order participants.

	GrowableMemory mem;
	pPlayer->Serialize(&mem);
	pPlayer->Deserialize(&mem);
	*/

	bool bRet = GameServer::Start();
	if( bRet ){
		m_pPokerGameConfig = (PokerServerConfig*)m_pConfig;

		if( !m_pData )
			m_pData = new PokerData();
		else
			m_pData->FreeData(true);

		// Set connection string.
		if( !m_pPokerGameConfig->m_sSQLConnect.IsEmpty() )
			m_pData->SetConnString(m_pPokerGameConfig->m_sSQLConnect);
		// Set avatars folder.
		m_pData->SetAvatarsFolder(m_pPokerGameConfig->m_sAvatarsFolder);
		// Load data from database.
		m_pData->LoadData();
		// Load avatars.
		m_pData->LoadAvatars();

		
		//Temp
		/*
		EnumerableObject<TournamentPlayerInfo>	listParticipants;			// List of tournament participant players.
		listParticipants	.CreateVariableIndex(_T("UserId"),	Serializable::Compare_Int32);
		listParticipants	.CreateVariableIndex(_T("Order"),	Serializable::Compare_Int32_Desc);

		for(int i=0; i<100; i++){
			int nUserId = 10000 + i;
			TournamentPlayerInfo* pPlayer = new TournamentPlayerInfo();
			pPlayer->m_cLevel		= 0;
			pPlayer->m_nUserId		= nUserId;
			pPlayer->m_sUserName.Format(_T("bot%d"), i +1);
			pPlayer->m_nChips		= 100000;
			pPlayer->m_shPlace		= -1;
			pPlayer->m_nOrder		= 100000;	// Need to order participants.
			listParticipants.Add(pPlayer);
			}


		//listParticipants.Get
		TournamentPlayerInfo* pPlayer = listParticipants.GetAt(0);
		int nOrder = 99000;
		listParticipants.ModifyIndexVariable(_T("Order"), pPlayer, &nOrder);
		*/
		//Temp

		/*
		PokerHandHistory handInfo;
		m_pData->GetHandHistoryInfo(187254, handInfo);
		_String sFile = _T("187254.bin");
		Path::BuildFullPath(sFile);
		handInfo.Serialize(sFile, true);

		CDWordArray arr;
		for(int i=0; i<handInfo.m_listSeats.GetCount(); i++){
			PokerTableSeatInfo* pp = handInfo.m_listSeats[i];
			if( pp ){
				arr.Add(pp->GetCard1());
				arr.Add(pp->GetCard2());
				}
			}
		
		_String s;
		s.Format("{%d, %d, %d, %d, %d} {%d, %d} {%d, %d}", 
			handInfo.m_cTableCard1,
			handInfo.m_cTableCard2,
			handInfo.m_cTableCard3,
			handInfo.m_cTableCard4,
			handInfo.m_cTableCard5,

			arr[0],
			arr[1],
			arr[2],
			arr[3]
			);
		AfxMessageBox(s);
		*/
		/*
		PokerHandHistory h;
		m_pData->GetHandHistoryInfo(41486, h);
		_String sFile = _T("41486.bin");
		Path::BuildFullPath(sFile);
		h.Serialize(sFile, true);
		*/

		// Setup lobby info.
		//SetupLobbyInfo();
		// Load avatars from folder.
		//LoadAvatars(m_sAvatarsFolder);

		m_cashGames			.Start(m_pPokerGameConfig, &m_lobbyInfo);	// Stop cash games controller.
		m_tournamentGames	.Start(m_pPokerGameConfig, &m_lobbyInfo);	// Stop tournaments controller.

		// Temp
		/*
		SeatUserSession(53594, 4, 0, 0, 200); // testmax
		SeatUserSession(3, 4, 1, 0, 100); // zack1
		SeatUserSession(4, 4, 2, 0, 100); // zack2
		SeatUserSession(5, 4, 3, 0, 100); // zack3
	*/
		// Temp
		}
	return bRet;
	}

bool
PokerServer::Stop(){
	m_cashGames			.Stop();	// Stop cash games controller.
	m_tournamentGames	.Stop();	// Stop tournaments controller.

	RemoveAllGameControllersByConnection	();
	RemoveAllConnectionsByTableId			();

	bool bRet = GameServer::Stop();

	// Free data objects.
	if( m_pData ){
		m_pData->FreeData(true);
		delete m_pData;
		m_pData = NULL;
		}

	m_pPokerGameConfig = NULL;

	// Remove user info.
	PokerSession::RemoveAllUserSession();
	// Empty lobby info.
	m_lobbyInfo.EmptyInfo();	
	return bRet;
	}

bool
PokerServer::SendPacketByTable(int nTableId, int nTournamentId, BasePacket* p, bool bOnlyObservers /*= false*/){
	m_lockTableObj.Lock(); // Lock
	List<ClientConnection>* pListConn = NULL;
	GetTableConnections(nTableId, nTournamentId, pListConn);
	if( !pListConn || pListConn->GetCount() <= 0 ){
		m_lockTableObj.Unlock(); // Unlock
		return false;
		}

	if( bOnlyObservers ){		// Send only to observers.
		int		nLoop	= 0;
		bool	bOpened = false;
		while( nLoop < pListConn->GetCount() ){
			ClientConnection*	pConn = pListConn->GetAt(nLoop);
			if( pConn ){
				PokerSession*	pSess		= (PokerSession*)pConn->GetSession(false);
				bool			bObserver	= pSess ? pSess->IsTableObserver(nTableId, nTournamentId) : false;
				if( bObserver )
					SendPacket(pConn, p);
				}
			nLoop ++;
			}
		}
	else{				// Send to everyone.
		int nLoop = 0;
		while( nLoop < pListConn->GetCount() ){
			SendPacket(pListConn->GetAt(nLoop), p);
			nLoop ++;
			}
		}
	
	m_lockTableObj.Unlock();	// Unlock
	return true;
	}

int
PokerServer::QuitAllClients(){
	/*
	m_secSessionLock.Lock(); // Lock
	int nLoop	= 0;
	int nCt		= m_arrSessions.GetCount();
	int nResult = 0;

	PokerQuitClientPacket* p	= new PokerQuitClientPacket();
	p->m_dtServerTime			= COleDateTime::GetCurrentTime();

	while( nLoop < nCt ){
		Session* pSess	= (Session*)m_arrSessions.GetData(nLoop);
		if( pSess ){
			pSess->Retain(); // Retain pointer.
			ClientConnection* pConn = pSess->GetConnection(true, false);
			if( pConn ){
				nResult ++;
				pConn->Release(); // Release pointer.
				pSess->Release(); // Release pointer.
				SendPacket(pConn, p);
				}
			else
				pSess->Release(); // Release pointer.
			}
		nLoop ++;
		}
	m_secSessionLock.Unlock(); // Unlock
	delete p;
	return nResult;*/ return 0;
	}

PokerCashTableController*
PokerServer::GetCashGameController(int nTableId){
	return m_cashGames.GetGameController(nTableId);
	}

TournamentTableController*
PokerServer::GetTournamentGameController(int nTournamentId, int nTableId){
	return m_tournamentGames.GetTableController(nTournamentId, nTableId);
	}

int
PokerServer::LoadAvatars(_String sFolder){
	int nAvatarsCt = 0;
	CStringVector arrAvatarImages;
	if( Path::GetFilesByExtention(sFolder, _String(_T("*.png")), arrAvatarImages) > 0 ){
		PokerAvatar* pAvatar = new PokerAvatar();
		_String sFileNew, sFile;
		Path::BuildFullPath(sFolder);

		for(int i=0; i<arrAvatarImages.size(); i++){
			pAvatar->m_nId				= 0;
			pAvatar->m_nPrivateUserId	= 0;
			pAvatar->m_sAvatarFileName	= _T("TEMP");
			pAvatar->m_recStatus		= 0;
			m_pData->SaveRecord(pAvatar); // Add new

			sFile				= arrAvatarImages.at(i);
			_String sNameCrypt;
			sNameCrypt.Format(_T("avatar_id=%d"), pAvatar->m_nId);
			_String sMD5		= CMD5Checksum::GetMD5((BYTE*)sNameCrypt.GetBuffer(), sNameCrypt.GetLength());

			sFileNew.Format(_T("%s\\%s.png"), sFolder.GetBuffer(), sMD5.GetBuffer());
			if( ::CopyFileA(sFile, sFileNew, FALSE) ){
				nAvatarsCt ++;
				pAvatar->m_sAvatarFileName = sMD5 + _T(".png");
				m_pData->SaveRecord(pAvatar);
				}
			else
				m_pData->DeleteRecord(pAvatar);
			}
		delete pAvatar;
		}
	return nAvatarsCt;
	}

void
PokerServer::FillWithAvatarsList(LoginPacket* p){
	if( !p ) return;
	p->m_listAvatars.RemoveAll();
	EnumerableObject<PokerAvatar>* pAvatars = &m_pData->m_listAvatars;

	for(int i=0; i<pAvatars->GetCount(); i++){
		PokerAvatar* pAvatar = pAvatars->GetAt(i);
		if( !pAvatar ) continue;
		AvatarInfo* pInfo			= new AvatarInfo();
		pInfo->m_nAvatarId			= pAvatar->m_nId;
		pInfo->m_sAvatarFileName	= pAvatar->m_sAvatarFileName;
		pInfo->m_sAvatarName		= _T("");
		p->m_listAvatars.Add(pInfo);
		}
	}

///////////////////////////////////////////////////////////
///
///	Virtual events.
///
///////////////////////////////////////////////////////////

void
PokerServer::OnDisconnected(ClientConnection* pConn, DWORD dwLastError){
	GameServer::OnDisconnected(pConn, dwLastError);

	AutoSortedArrayInt64 arrSeatIndexByTableId;
	_String			sUserName;
	PokerSession*	pSess = (PokerSession*)pConn->GetSession(false);
	if( pSess ){
		sUserName = pSess->GetUserName();
		pSess->GetSeatedTableIds(arrSeatIndexByTableId);
		/*
		// Deauthentcate web session user.
		if( !dwLastError && pSess->IsWebSessionUser() )
			m_pData->LogoutWebSessionUser(pSess->GetUserId());
			*/
		}

	int		nLoop	= 0, nCt = arrSeatIndexByTableId.GetCount();
	__int64	nValue	= 0;
	while( nLoop < nCt ){
		nValue	= arrSeatIndexByTableId.Get(nLoop);
		int						nTournamentId	= (int)(nValue >> 32);
		int						nTableId		= (int)(nValue&0xFFFFFFFF);
		int						nSeat			= arrSeatIndexByTableId.GetData(nLoop);
		PokerTableController*	pController		= (nTournamentId > 0) ? GetTournamentGameController(nTournamentId, nTableId) : GetCashGameController(nTableId);
		if( pController )
			pController->OnConnectionDropped(pSess, pConn, nSeat);
		nLoop ++;
		}

	// Remove connection.
	RemoveConnection(pConn, false);

	// Update players count.
	m_lobbyInfo.SetPlayersCt(max(GetConnectionCt() - 1, 0));
	
	// Log entry.
	_String sLogEntry;
	sLogEntry.Format(_T("Disconnected Ip=%s Port=%d UserName=%s Errcode=%d"), pConn->GetRemoteIpAddress().GetBuffer(), pConn->GetRemotePort(), sUserName.GetBuffer(), dwLastError);
	Logger::CreateLogEntry(sLogEntry.GetBuffer());
	}

void
PokerServer::OnConnected(ClientConnection* pConn, ConnectedSocketInfo* pSockInfo){
	GameServer::OnConnected(pConn, pSockInfo);

	// Update players count.
	m_lobbyInfo.SetPlayersCt(GetConnectionCt());

	// Log entry.
	_String sLogEntry; 
	sLogEntry.Format(_T("Connected Ip=%s Port=%d"), pSockInfo->m_sRemoteIp.GetBuffer(), pSockInfo->m_nRemotePort);
	Logger::CreateLogEntry(sLogEntry.GetBuffer());
	}

void
PokerServer::OnPacketReceived(ClientConnection* pConn, BasePacket* packet){
	pConn						= (ClientConnection*)SafePointer::Retain(pConn); // Retain connection pointer.
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(true);

	GameServer::OnPacketReceived(pConn, packet);

	if( pSess ) 
		SafePointer::Release(pSess);	// Release session pointer.
	SafePointer::Release(pConn);
	}

void
PokerServer::OnSessionExpired(Session* pSess){
	ClientConnection*	pConn		= pSess->GetConnection(false, false);
	_String				sUserName	= ((PokerSession*)pSess)->GetUserName();
	int					nUserId		= ((PokerSession*)pSess)->GetUserId();
	_String				sIp			= pConn ? pConn->GetRemoteIpAddress() : _T("");
	int					nPort		= pConn ? pConn->GetRemotePort() : 0;

	// Deauthentcate web session user.
	if( ((PokerSession*)pSess)->IsWebSessionUser() && nUserId > 0 )
		m_pData->LogoutWebSessionUser(nUserId);

	_String sLog;
	sLog.Format(_T("Session Expired Ip=%s Port=%d User=%s"), sIp.GetBuffer(), nPort, sUserName.GetBuffer());
	if( pConn ){
		// Remove connection and other params.
		RemoveConnection(pConn, false);
		}
	// Log entry.
	Logger::CreateLogEntry(sLog.GetBuffer());
	}

void
PokerServer::OnSessionRestored(Session* pSess, ClientConnection* pConn){
	ASSERT(pSess);
	GameServer::OnSessionRestored(pSess, pConn);

	m_lockUserObj.Lock();		// Lock
	int			nUserId	= ((PokerSession*)pSess)->GetUserId();
	if( nUserId <= 0 ){
		m_lockUserObj	.Unlock();	// Unlock
		return;
		}

	_String sLogEntry;
	sLogEntry.Format(_T("Session Restored Ip=%s Port=%d UserName=%s"), pConn->GetRemoteIpAddress().GetBuffer(), pConn->GetRemotePort(), ((PokerSession*)pSess)->GetUserName().GetBuffer());
	m_lockUserObj.Unlock();		// Unlock
	
	AutoSortedArrayInt64 arrSeatIndexByTableId;
	((PokerSession*)pSess)->GetSeatedTableIds(arrSeatIndexByTableId);
			
	int		nLoop	= 0;
	int		nCt		= arrSeatIndexByTableId.GetCount();
	__int64	nValue	= 0;
	while( nLoop < nCt ){
		nValue									= arrSeatIndexByTableId.Get(nLoop);
		int						nTableId		= (int)(nValue&0xFFFFFFFF);
		int						nTournamentId	= (int)(nValue >> 32);
		int						nSeat			= arrSeatIndexByTableId.GetData(nLoop);
		PokerTableController*	pController		= NULL;

		if( nTournamentId > 0 )
			pController = GetTournamentGameController(nTournamentId, nTableId);
		else
			pController = GetCashGameController(nTableId);

		if( pController && nSeat != -1 ){
			// Attach connection to table.
			// AddTableConnection(nTableId, nTournamentId, pConn, pController);
			// Restore game table session.
			pController->OnGameSessionRestored((PokerSession*)pSess, pConn, nUserId);
			}
		nLoop ++;
		}
	// Log entry.
	Logger::CreateLogEntry(sLogEntry.GetBuffer());
	}

///////////////////////////////////////////////////////////
///
///	Packet handler methods.
///
///////////////////////////////////////////////////////////

void	
PokerServer::OnLogin(LoginPacket* p, ClientConnection* pConn){
	if( pConn->IsAuthorized() ) return;
	_String			sLog;
	PokerSession*	pSess			= (PokerSession*)pConn->GetSession(false);
	if( !pSess ) return;
	__int64			nSessionId		= pSess->GetSessionId();
	int				nUserId			= ((PokerSession*)pSess)->GetUserId();
	if( nUserId <= 0 ){
repeat_login:
		bool		bUserNotFound	= false, bUserBlocked = false, bWrongPassword = false, bSessionIsAlreadyActive = false;
		UserInfo*	pUser			= NULL;

		if( p->m_sUserName.IsEmpty() && p->m_sPassword.IsEmpty() && !p->m_sWebSessionId.IsEmpty() ){
			pUser	= m_pData->LoginUserByWebSessionId(p->m_sWebSessionId, bSessionIsAlreadyActive, bUserBlocked);
			if( pUser )
				p->m_sUserName = pUser->m_sUserName;
			}
		else
			pUser	= m_pData->LoginUser(p->m_sUserName, p->m_sPassword, bUserNotFound, bUserBlocked, bWrongPassword);
		
		if( bUserBlocked ){
			if( pUser ) delete pUser;
			p->m_nErrorCode = ES_ERROR_USER_BLOCKED;
			SendPacket(pConn, p); // Reply 
			return;
			}

		if( bWrongPassword ){
			if( pUser ) delete pUser;
			p->m_nErrorCode = ES_ERROR_USER_NOTFOUND_OR_WRONG_PASSWORD;
			SendPacket(pConn, p); // Reply 
			return;
			}

		if( bSessionIsAlreadyActive ){
			if( pUser ) delete pUser;
			p->m_nErrorCode = ES_ERROR_SESSION_IS_ALREADY_ACTIVE;
			SendPacket(pConn, p); // Reply 
			return;
			}

		p->m_bPrevSessionRestored	= false;
		if( pUser ){
			p->m_bAvatarIsBlocked	= pUser->m_bAvatarIsBlocked;
			p->m_bChatIsBlocked		= pUser->m_bChatIsBlocked;

			PokerSession*	pSessByUser		= PokerSession::GetSessionByUserId(pUser->m_nUserId, true);
			if( !pSessByUser ){
				p->m_nBalance		= pUser->m_nBalanceAmount;
				p->m_nPlayBalance	= pUser->m_nPlayBalance;
				p->m_dVPPoints		= pUser->m_dVPPoints;
				p->m_btRank			= (BYTE)pUser->m_shRankLevel;
				p->m_nAvatarId		= pUser->m_nAvatarId;
				p->m_nUserId		= pUser->m_nUserId;

				m_lockUserObj.Lock();				// Lock
				pSess->SetUserInfo(pUser);			// Save in session.
				m_lockUserObj.Unlock();				// Unlock

				//pConn->SetAuthorized(true);		// Connection is authenticated.
				pSess->SetAuthorized(true);			// Make all connections authorized.

				// Load user tournaments.
				m_pData->LoadUserTournaments(pUser->m_nUserId, p);
				}
			else{
				if( pSessByUser ){
					UserInfo*			pUserExisted	= pSessByUser->GetUserInfo();
					ClientConnection*	pConnOld		= pSessByUser->GetConnection(true, false);
					if( pSessByUser->IsActive() ){	// Already logged in !!!
						ASSERT(pConnOld);
						// Session attach to new user info class.
						pSessByUser->SetUserInfo(pUser);							// Save in session.
					
						SafePointer::Release(pConnOld);								// Release old connection.
						SafePointer::Release(pUserExisted);							// Release user info pointer.
						p->m_nErrorCode = ES_ERROR_USER_ALREADY_LOGGED_IN;
						}
					else{							// Restore previous session of the same user !!!!
						m_lockConnections.Lock();	// Lock
						pSessByUser->SetUserInfo	(pUser);						// Update userinfo.
						pConn->SetSession			(pSessByUser);
						m_lockConnections.Unlock();	// Unlock

						Session::DestroySession			(pSess);										// Destroy current session.
						Session::RestoreInactiveSession	(pSessByUser->GetSessionId(), pConn, false);	// Restore previous session.

						pSessByUser->SetAuthorized	(true);												// Make all connections authorized.
						//pConn->SetAuthorized		(true);												// Connection is authenticated.

						// Create list of opened tables.
						AutoSortedArrayInt64 arrSeatedTables;
						if( pSessByUser->GetSeatedTableIds(arrSeatedTables) > 0 ){
							for(int i=0; i<arrSeatedTables.GetCount(); i++){
								OpenedTableInfo* pTableInfo = new OpenedTableInfo();
								__int64 nValue				= arrSeatedTables[i];
								pTableInfo->m_nTableId		= (int)nValue;
								pTableInfo->m_nTournamentId = (int)(nValue >> 32);
								p->m_openedTables.Add(pTableInfo);
								}
							}
						
						// Load user tournaments.
						m_pData->LoadUserTournaments(pUser->m_nUserId, p);
						
						// Login reply packet.
						p->m_nSessionId				= pSessByUser->GetSessionId();
						p->m_bPrevSessionRestored	= true;
						p->m_nBalance				= pUser->m_nBalanceAmount;
						p->m_nPlayBalance			= pUser->m_nPlayBalance;
						p->m_dVPPoints				= pUser->m_dVPPoints;
						p->m_btRank					= (BYTE)pUser->m_shRankLevel;
						p->m_nUserId				= pUser->m_nUserId;
						p->m_nAvatarId				= pUser->m_nAvatarId;
						p->m_nErrorCode				= ES_ERROR_SUCCESS;
						FillWithAvatarsList(p);
						m_lockUserObj.Unlock();								// Unlock
						SendPacket(pConn, p);								// Reply login packet

						OnSessionRestored(pSessByUser, pConn);

						// Cleanup
						SafePointer::Release(pSessByUser);					// Release session pointer.
						if( pConnOld ) 
							SafePointer::Release(pConnOld);					// Release old connection.
						if( pUser ) delete pUser;

						// Log entry.
						sLog.Format(_T("OnLogin UserName=%s Ip=%s SessionRestored=true"), p->m_sUserName.GetBuffer(), pConn->GetRemoteIpAddress());
						Logger::CreateLogEntry(sLog.GetBuffer());
						return;
						}
					}
				else{
					p->m_nErrorCode = ES_ERROR_USER_ALREADY_LOGGED_IN;
					}
				SafePointer::Release(pSessByUser); // Release session pointer.
				}
			if( pUser ) delete pUser;
			}
		else{
			// Authenticate through web service. {{
			MaximumWebService webService;
			webService.SetServicePort(m_pPokerGameConfig->m_nWebServicePort);
			webService.SetServiceURL(m_pPokerGameConfig->m_sWebServiceURL);
			int nUserId = webService.Authenticate(p->m_sUserName, p->m_sPassword);
			if( nUserId > 0 ){
				// Import user info from web service.
				MaximumUserDetails userDetails;
				if( webService.GetUserDetails(p->m_sUserName, userDetails) ){
					// Create poker user.
					int nRet = m_pData->CreateUser(nUserId, 
						userDetails.userName, p->m_sPassword, 
						userDetails.email, userDetails.firstName, 
						userDetails.lastName, userDetails.phoneNumber, m_pPokerGameConfig->m_nPlayMoneyChips);

					// User info export completed.
					if( nRet == nUserId ){
						goto repeat_login;
						}
					}
				}
			// }}
			p->m_nErrorCode = ES_ERROR_USER_NOTFOUND_OR_WRONG_PASSWORD;
			}
		}

	if( p->m_nErrorCode == ES_ERROR_SUCCESS )
		FillWithAvatarsList(p);

	SendPacket(pConn, p); // Reply 

	// Log entry.
	if( p->m_nErrorCode == ES_ERROR_SUCCESS ){
		sLog.Format(_T("OnLogin UserName=%s Ip=%s"), p->m_sUserName.GetBuffer(), pConn->GetRemoteIpAddress().GetBuffer());
		Logger::CreateLogEntry(sLog.GetBuffer());
		}
	}

void
PokerServer::OnLogout(LogoutPacket* p, ClientConnection* pConn){
	if( !pConn->IsAuthorized() ){ 
		p->m_nSessionIdNew	= 0;
		p->m_nErrorCode		= ES_ERROR_USER_ALREADY_LOGGED_OUT;
		SendPacket(pConn, p); // Reply 
		return;
		}

	PokerSession*	pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess ){
		pConn->SetAuthorized(false);
		return;
		}
	pSess->SetAuthorized(false);							// Make all connections deauthorized.

	// Deauthentcate web session user.
	if( pSess->IsWebSessionUser() )
		m_pData->LogoutWebSessionUser(pSess->GetUserId());
		
	_String	sUserName = pSess->GetUserName();
	// Drop connection from active game tables. {{
	AutoSortedArrayInt64 arrSeatIndexByTableId;
	if( pSess && ((PokerSession*)pSess)->GetSeatedTableIds(arrSeatIndexByTableId) ){
		if( arrSeatIndexByTableId.GetCount() > 0 ){
			int nLoop	= 0;
			int nCt		= arrSeatIndexByTableId.GetCount();
			__int64	nValue = 0;
			while( nLoop < nCt ){
				nValue									= arrSeatIndexByTableId.Get(nLoop);
				int						nTableId		= (int)nValue;
				int						nTournamentId	= (int)(nValue >> 32);
				int						nSeat			= (int)arrSeatIndexByTableId.GetData(nLoop);
				PokerTableController*	pController		= NULL;
				bool					bTournament		= (nTournamentId > 0);

				if( bTournament )
					pController	= GetTournamentGameController(nTournamentId, nTableId);
				else
					pController	= GetCashGameController(nTableId);

				if( pController ){
					pController->OnConnectionDropped((PokerSession*)pSess, pConn, nSeat);

					// Remove table connection.
					//if( bTournament )
						RemoveTableConnection(nTableId, nTournamentId, pConn, pController);
						
					// Close table.
					// pSess->CloseTable(nTableId, nTournamentId);
					}
				nLoop ++;
				}
			}
		}
	// }}
	m_lockConnections.Lock();	// Lock
	PokerSession*		pSessNew	= (PokerSession*)Session::CreateNewSession(this, pConn, true);
	ASSERT(pSessNew);
	pSess->SetConnection(NULL, true);			// Make current session connectionless.
	pConn->SetSession	(pSessNew);				// Attach current connection to new session.

	// Connection dropped during the logout operation.
	if( !pSess->IsActive() )
		Session::DestroySession(pSessNew);
	else
		Session::DestroySession(pSess);
	m_lockConnections.Unlock(); // Unlock

	p->m_nSessionIdNew	= pSessNew->GetSessionId();
	p->m_nErrorCode		= ES_ERROR_SUCCESS;

	SendPacket(pConn, p);	// Reply 

	_String sLog;
	sLog.Format(_T("OnLogout UserName=%s Ip=%s"), sUserName.GetBuffer(), pConn->GetRemoteIpAddress().GetBuffer());
	Logger::CreateLogEntry(sLog.GetBuffer());

	if( pSessNew ) 
		SafePointer::Release(pSessNew); // Release
	}

void
PokerServer::OnGetLobbyInfo(PokerGetLobbyInfoPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() ) return;

	// Only authenticated user's count.
	p->m_nPlayersCt			= m_sockServer.GetConnectionCt();
	if( pSess ){
		p->m_nBalance		= pSess->GetBalance();
		p->m_nInPlayAmount	= pSess->GetInPlayBalance();
		p->m_nPlayBalance	= pSess->GetPlayBalance();
		}
	else{
		p->m_nBalance		= 0;
		p->m_nInPlayAmount	= 0;
		p->m_nPlayBalance	= 0;
		}
	m_lobbyInfo.SendLobbyInfoByRequest(pConn, p);

	/*
	SendPacket(pConn, p); // Send reply 

	p->m_listCashTablePlayersInfo	.RemoveAll(false);
	p->m_listCashTablesDynamicInfo	.RemoveAll(false);
	p->m_listCashTablesStaticInfo	.RemoveAll(false);
	p->m_listTournamentInfo			.RemoveAll(false);
	*/
	}

void
PokerServer::OnGetHandHistory(PokerGetHandHistoryPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() ) return;
		
	PokerHandHistory handInfo;
	if( !m_pData->GetHandHistoryInfo(p->m_nHandId, handInfo) ){
		SendPacket(pConn, p); // Empty info.
		}
	else{
		p->m_handData.Add(&handInfo);
		SendPacket(pConn, p); // With hand info.
		p->m_handData.RemoveAll(false);
		}
	}

void
PokerServer::OnSetAvatar(PokerSetAvatarPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() ) return;

	int nUserId = 0;
	m_lockUserObj.Lock();	// Lock
	nUserId = pSess->GetUserId();
	m_lockUserObj.Unlock(); // Unlock

	if( nUserId <= 0 ) return; // Invalid user id.

	/*
	m_lockUserObj.Lock(); // Lock.
	int nAvatarIdNew = 0;
	if( m_pData->SetUserAvatar(pUserInfo, p->m_nAvatarId, &p->m_imageData, nAvatarIdNew, false) ){
		m_lockUserObj.Unlock(); // Unlock

		// Free binary data.
		Serializable::GlobalReAlloc(&p->m_imageData, 0);
		
		p->m_nAvatarId = nAvatarIdNew;
		p->m_imageData.m_dwDataLength = 0;

		// Reply packet.
		SendPacket(pConn, p);
		}
	else
		m_lockUserObj.Unlock(); // Unlock
		*/
	}

void
PokerServer::OnChatMessage(PokerChatMessagePacket* p, ClientConnection* pConn){
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() ) return;
	if( !pConn->IsAuthorized() || pSess->IsTableObserver(p->m_nTableId, p->m_nTournamentId) ) return; // Need authorization and must be seated already.

	/*
	PokerRedirectPacket packet;
	packet.m_nTableId				= 2;
	packet.m_nTournamentId			= 0;
	packet.m_nTableId_Redirect		= 5;
	packet.m_nTournamentId_Redirect = 0;
	SendPacket(pConn, &packet);*/

	if( !p->m_nTournamentId )
		m_cashGames.OnChatMessage(p, pConn);
	else
		m_tournamentGames.OnChatMessage(p, pConn);
	}

void
PokerServer::OnMoneyTransfer(PokerMoneyTransferPacket* p, ClientConnection* pConn){
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() ) return;

	m_lockUserObj.Lock(); // Lock
	int					nUserId		= pSess->GetUserId();
	_String				sUserName	= pSess->GetUserName();
	_String				sIp			= pConn->GetRemoteIpAddress();
	m_lockUserObj.Unlock(); // Unlock

	bool		bRet			= false;
	INT_CHIPS	nBalanceNew		= 0;
	INT_CHIPS	nMainBalance	= 0;
	int		nCurrencyId		= CurrencyType::Currency_GEL;
	_String	sAccountFrom;
	_String	sAccountTo;

	if( !p->m_bRefreshBalances ){
		if( p->m_nAccountFrom == AccountType::MainBalance && p->m_nAccountTo == AccountType::PokerBalance ){
			bRet = TransferMoneyFromMainBalance(nUserId, p->m_nTransAmount, nCurrencyId, nBalanceNew, nMainBalance);
			p->m_nBalanceTo		= nBalanceNew;
			p->m_nBalanceFrom	= nMainBalance;
			sAccountFrom		= _T("MainBalance");
			sAccountTo			= _T("PokerBalance");
			}

		if( p->m_nAccountFrom == AccountType::PokerBalance && p->m_nAccountTo == AccountType::MainBalance ){
			bRet = TransferMoneyToMainBalance(nUserId, p->m_nTransAmount, nCurrencyId, nBalanceNew, nMainBalance);
			p->m_nBalanceFrom	= nBalanceNew;
			p->m_nBalanceTo		= nMainBalance;
			sAccountFrom		= _T("PokerBalance");
			sAccountTo			= _T("MainBalance");
			}

		// Change user balance.
		if( bRet ){
			pSess->SetUserBalance(nBalanceNew);
			}
		}
	else{ // Reply account balance amounts.
		GetBalanceByAccount(p->m_nAccountFrom, nUserId, nCurrencyId, p->m_nBalanceFrom);
		GetBalanceByAccount(p->m_nAccountTo, nUserId, nCurrencyId, p->m_nBalanceTo);
		bRet = true;
		}

	// Send reply.
	p->m_nErrorCode		= bRet ? ES_ERROR_SUCCESS : ES_ERROR_UNKNOWN;
	SendPacket(pConn, p);

	// Create log entry.
	if( bRet && !p->m_bRefreshBalances ){
		_String sLog;
		sLog.Format(_T("OnMoneyTransfer  UserName=%s Ip=%s Amount=%d.%02d From=%s To=%s"), sUserName.GetBuffer(), sIp.GetBuffer(), p->m_nTransAmount/100, p->m_nTransAmount%100, sAccountFrom.GetBuffer(), sAccountTo.GetBuffer());
		Logger::CreateLogEntry(sLog.GetBuffer());
		}
	}

void
PokerServer::OnResetPlaymoney(PokerResetPlaymoneyPacket* p, ClientConnection* pConn){
	PokerSession*		pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() ) return;

	m_lockUserObj.Lock();	// Lock
	INT_CHIPS			nPlayBalance		= pSess->GetPlayBalance();
	INT_CHIPS			nInPlayPlayBalance	= pSess->GetInPlayPlayBalance();
	int					nUserId				= pSess->GetUserId();
	m_lockUserObj.Unlock(); // Unlock

	INT_CHIPS nPlayBalanceDefault = (m_pPokerGameConfig->m_nPlayMoneyChips*100);
	if( (nPlayBalance + nInPlayPlayBalance) < nPlayBalanceDefault ){
		if( m_pData->SetUserPlayBalance(nUserId, nPlayBalanceDefault) ){
			nPlayBalance = nPlayBalanceDefault;
			pSess->SetUserPlayBalance(nPlayBalance);
			}
		}

	// Send replay.
	p->m_nPlayMoneyAmount = nPlayBalance;
	SendPacket(pConn, p);
	}

void
PokerServer::OnRefreshBalance(PokerRefreshBalancePacket* p, ClientConnection* pConn){
	PokerSession* pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() ) return;

	m_lockUserObj.Lock(); // Lock
	p->m_nPlayMoneyAmount	= pSess->GetPlayBalance();
	p->m_nBalance			= pSess->GetBalance();
	p->m_nCashInPlay		= pSess->GetInPlayBalance();
	p->m_nPlayMoneyAmount	= pSess->GetPlayBalance();
	p->m_nPlayMoneyInPlay	= pSess->GetInPlayPlayBalance();
	p->m_dVPPoints			= 0.0;
	p->m_cTicketsCt			= 0;
	m_lockUserObj.Unlock(); // Unlock

	// Send replay.
	SendPacket(pConn, p);
	}

void
PokerServer::OnFindPlayer(PokerFindPlayerPacket* p, ClientConnection* pConn){
	PokerSession* pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() ) return;
	p->m_listTables.RemoveAll(true);

	int nUserId = p->m_nUserId <= 0 ? m_pData->GetUserIdByName(p->m_sUserName) : p->m_nUserId;
	if( nUserId > 0 ){
		PokerSession* pSessUser = PokerSession::GetSessionByUserId(nUserId, true);
		if( pSessUser ){
			AutoSortedArrayInt64 arrSeatIndexByTableId;
			if( pSessUser->GetSeatedTableIds(arrSeatIndexByTableId) > 0 ){
				AutoSortedArray arrTableIds;
				arrTableIds.SetSize(arrSeatIndexByTableId.GetCount());

				__int64 nValue = 0;
				for(int i=0; i<arrSeatIndexByTableId.GetCount(); i++){
					nValue = arrSeatIndexByTableId.Get(i);
					if( (int)(nValue >> 32) > 0 ){
						// Tournament table.
						}
					else{
						int nTableId = (int)(nValue & 0xFFFFFFFF);
						arrTableIds.Add((void*)nTableId);
						}
					}
				m_lobbyInfo.FindPlayerTables(&arrTableIds, &p->m_listTables);
				}
			SafePointer::Release(pSessUser);
			}
		}

	// Send replay.
	SendPacket(pConn, p);
	}

void
PokerServer::OnFindTournament(PokerFindTournamentPacket* p, ClientConnection* pConn){
	PokerSession* pSess	= (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() ) return;
	p->m_listTournaments.RemoveAll(true);

	LobbyTournamentInfo info;
	if( p->m_sTournamentID.GetLength() > 0 && p->m_sTournamentID.GetAt(0) == 'T' ){
		TCHAR* pszTournamentID = p->m_sTournamentID.GetBuffer();
		int nTournamentId = _ttoi(&pszTournamentID[1]);
		if( nTournamentId > 0 ){
			if( m_lobbyInfo.FindTournament(nTournamentId, &info) ){
				p->m_listTournaments.Add(&info);
				}
			}
		}

	// Send replay.
	SendPacket(pConn, p);

	p->m_listTournaments.RemoveAll(false);
	}

// ======================================================================
//
//	Poker game table packet methods.
//
// ======================================================================

void
PokerServer::OnOpenTable(PokerOpenTablePacket* p, ClientConnection* pConn){
	m_cashGames.OnOpenTable(p, pConn); // Only for cash game.
	}

void
PokerServer::OnLeaveTable(PokerLeaveTablePacket* p, ClientConnection* pConn){
	m_cashGames.OnLeaveTable(p, pConn);
	}

void
PokerServer::OnReserveSeat(PokerReserveSeatPacket* p, ClientConnection* pConn){
	m_cashGames.OnReserveSeat(p, pConn);
	}

void
PokerServer::OnCancelReservation(PokerCancelReservationPacket* p, ClientConnection* pConn){
	m_cashGames.OnCancelReservation(p, pConn);
	}

void
PokerServer::OnSeatAt(PokerSeatAtTablePacket* p, ClientConnection* pConn){
	m_cashGames.OnSeatAt(p, pConn);

	//Temp
	/*
	PokerChatMessagePacket welcome;
	welcome.m_bIsRequest = true;
	welcome.m_nDestUserId = 0;
	welcome.m_nTableId = p->m_nTableId;
	welcome.m_nSessionId = p->m_nSessionId;
	welcome.m_nErrorCode = 0;
	welcome.m_nTournamentId = 0;
	welcome.m_nPacketSize = 0xFFFF;

	welcome.m_message.m_bSystem		= false;
	welcome.m_message.m_actionType = 0;
	welcome.m_message.m_cSeat		= -1; // Is not neccesary when client sends packet to server.
	welcome.m_message.m_nAmount		= 0;
	welcome.m_message.m_sMessage	= "Hello there !!!";

	GrowableMemory mem(1024, 1024, false);
	AutoSortedArray arr;
	arr.Add((void*)offsetof(BasePacket, m_nPacketSize), NULL);
	welcome.Serialize(&mem, &arr);
	// Set packet size.
	*(int*)(&(mem.GetBufferPtr()[arr.GetDataAsLong(0)])) = mem.GetUsedBufferSize(); 
	
	int nEachPacketSize = mem.GetUsedBufferSize();
	BYTE* lpSend = new BYTE[nEachPacketSize*3];
	memcpy(&lpSend[0*nEachPacketSize], mem.GetBufferPtr(), nEachPacketSize);
	memcpy(&lpSend[1*nEachPacketSize], mem.GetBufferPtr(), nEachPacketSize);
	memcpy(&lpSend[2*nEachPacketSize], mem.GetBufferPtr(), nEachPacketSize);

	pConn->SendBytes(&lpSend[0*nEachPacketSize], (2*nEachPacketSize) + 10);
	pConn->SendBytes(&lpSend[2*nEachPacketSize + 10], nEachPacketSize - 10);
	*/
	//Temp
	
	//Temp
	//zack3 - 5
	SeatUserSession(4, p->m_nTableId, 1, 0, p->m_nChipAmount - 30);
	SeatUserSession(5, p->m_nTableId, 2, 0, p->m_nChipAmount);
	SeatUserSession(3, p->m_nTableId, 3, 0, p->m_nChipAmount - 20);
	SeatUserSession(6, p->m_nTableId, 4, 0, p->m_nChipAmount - 40);
	SeatUserSession(7, p->m_nTableId, 5, 0, p->m_nChipAmount - 50);
	SeatUserSession(8, p->m_nTableId, 6, 0, p->m_nChipAmount);
	/*
	SeatUserSession(3, p->m_nTableId, 1, 0, 140);
	SeatUserSession(4, p->m_nTableId, 2, 0, 150);
	SeatUserSession(6, p->m_nTableId, 3, 0, 160);
	SeatUserSession(7, p->m_nTableId, 4, 0, 170);
	SeatUserSession(8, p->m_nTableId, 5, 0, 180);
	*/

	/*
	SeatUserSession(5, p->m_nTableId, 0, 0, p->m_nChipAmount);
	SeatUserSession(3, p->m_nTableId, 1, 0, p->m_nChipAmount);
	SeatUserSession(4, p->m_nTableId, 2, 0, p->m_nChipAmount);
	SeatUserSession(6, p->m_nTableId, 3, 0, p->m_nChipAmount);
	SeatUserSession(7, p->m_nTableId, 4, 0, p->m_nChipAmount);
	SeatUserSession(8, p->m_nTableId, 5, 0, p->m_nChipAmount);
	*/
	//Temp
	}

void
PokerServer::OnJoinWaitingList(PokerJoinWaitingListPacket* p, ClientConnection* pConn){
	m_cashGames.OnJoinWaitingList(p, pConn);
	}

void
PokerServer::OnAddChips(PokerAddChipsPacket* p, ClientConnection* pConn){
	m_cashGames.OnAddChips(p, pConn);
	}

void
PokerServer::OnAction(PokerActionPacket* p, ClientConnection* pConn){
	if( p->m_nTournamentId > 0 )
		m_tournamentGames.OnAction(p, pConn);
	else
		m_cashGames.OnAction(p, pConn);
	}

void
PokerServer::OnTableInfo(PokerTableInfoPacket* p, ClientConnection* pConn){
	m_cashGames.OnTableInfo(p, pConn);
	}

void
PokerServer::OnReturnBack(PokerReturnBackPacket* p, ClientConnection* pConn){
	if( p->m_nTournamentId > 0 )
		m_tournamentGames.OnReturnBack(p, pConn);
	else
		m_cashGames.OnReturnBack(p, pConn);
	}

void
PokerServer::OnSitOut(PokerSitOutPacket* p, ClientConnection* pConn){
	if( p->m_nTournamentId > 0 )
		m_tournamentGames.OnSitOut(p, pConn);
	else
		m_cashGames.OnSitOut(p, pConn);
	}

void
PokerServer::OnPostBlind(PokerPostBlindPacket* p, ClientConnection* pConn){
	m_cashGames.OnPostBlind(p, pConn);
	}

void
PokerServer::OnShowMuckCards(PokerShowMuckCardsPacket* p, ClientConnection* pConn){
	if( p->m_nTournamentId > 0 )
		m_tournamentGames.OnShowMuckCards(p, pConn);
	else
		m_cashGames.OnShowMuckCards(p, pConn);
	}

void
PokerServer::OnSetOption(PokerSetOptionPacket* p, ClientConnection* pConn){
	if( p->m_nTournamentId > 0 )
		m_tournamentGames.OnSetOption(p, pConn);
	else
		m_cashGames.OnSetOption(p, pConn);
	}

void
PokerServer::OnOpenRandomTable(PokerOpenRandomTablePacket* p, ClientConnection* pConn){
/*
	if( p->m_nTournamentId > 0 )
		m_tournamentGames.OnOpenRandomTable(p, pParam);
	else
		m_cashGames.OnOpenRandomTable(p, pParam);*/

	m_cashGames.OnOpenRandomTable(p, pConn);
	}

// ======================================================================
//
//	Tournament packet methods.
//
// ======================================================================

void
PokerServer::OnCreateTournament(PokerCreateTournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnCreateTournament(p, pConn);
	}

void
PokerServer::OnCancelTournament(PokerCancelTournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnCancelTournament(p, pConn);
	}

void
PokerServer::OnTournamentCustomInfo(PokerTournamentCustomInfoPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnTournamentCustomInfo(p, pConn);
	}

void
PokerServer::OnRegister(PokerRegisterInTournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnRegister(p, pConn);
	}

void
PokerServer::OnUnregister(PokerUnregisterFromTournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnUnregister(p, pConn);
	}

void
PokerServer::OnOpenTable_Tournament(PokerOpenTable_TournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnOpenTable_Tournament(p, pConn);
	}

void
PokerServer::OnLeaveTable_Tournament(PokerLeaveTable_TournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnLeaveTable_Tournament(p, pConn);
	}

void
PokerServer::OnRebuy(PokerRebuyPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnRebuy(p, pConn);
	}

void
PokerServer::OnAddon(PokerAddonPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnAddon(p, pConn);
	}

// Upload / Download
void
PokerServer::OnUploadAvatar(PokerUploadAvatarPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() || !pConn->IsAuthorized() || pSess->IsAvatarBlocked() ) 
		return;

	int		nUserId	= pSess->GetUserId();
	int		nSize	= p->m_imageData.GetBinarySize();
	BYTE*	pBinary = (BYTE*)p->m_imageData.LockMemory();
	if( nSize == (int)p->m_shSize && nSize > 0 && pBinary ){
		if( !pSess->IsAvatarUploadingStarted() ){
			// Image size limit.
			if( p->m_nFullBinarySize > 64*1024/*64KB*/ ){
				p->m_imageData.SetBinarySize(0);
				return;
				}
			pSess->StartUploadingAvatarImage(p->m_nFullBinarySize);
			}

		int nBytesLeft = 0, nFullSize = 0;
		if( pSess->UploadAvatarImageData(pBinary, nSize, nFullSize, nBytesLeft, p->m_bCompleted) ){
			p->m_imageData.SetBinarySize(0);
			
			p->m_shSize						= 0;
			p->m_nAvatarId					= 0;
			p->m_nUploaded					= (nFullSize - nBytesLeft);
			p->m_nFullBinarySize			= nFullSize;

			if( p->m_bCompleted ){
				_String sImageFile, sAvatarFileName;
				int nAvatarIdOld = pSess->GetAvatarId();
				int nAvatarIdNew = m_pData->SetUserAvatar(nUserId, sAvatarFileName, -1, true);
				if( nAvatarIdNew > 0 ){
					// Delete old avatar file. {{
					if( nAvatarIdOld > 0 ){
						sImageFile.Format(_T("%s\\%s"), m_pPokerGameConfig->m_sAvatarsFolder.GetBuffer(), m_pData->GetAvatarFileName(nAvatarIdOld).GetBuffer());
						Path::BuildFullPath(sImageFile);
						::DeleteFile(sImageFile);
						}
					// }}

					// Save uploaded avatar file. {{
					sImageFile.Format(_T("%s\\%s"), m_pPokerGameConfig->m_sAvatarsFolder.GetBuffer(), sAvatarFileName.GetBuffer());
					Path::BuildFullPath(sImageFile);
					if( pSess->SaveUploadedAvatar(sImageFile, nAvatarIdNew) ){
						p->m_nAvatarId	= nAvatarIdNew;
						p->m_nErrorCode = ES_ERROR_SUCCESS;
						SendPacket(pConn, p);
						return;
						}
					// }}
					}
				p->m_nErrorCode = ES_ERROR_UNKNOWN;
				SendPacket(pConn, p);
				return;
				}
			p->m_nErrorCode = ES_ERROR_SUCCESS;
			SendPacket(pConn, p);
			return;
			}
		}

	if( pBinary ){
		pSess->DiscardAvatarUploading();
		p->m_imageData.SetBinarySize(0);
		}
	}

void
PokerServer::OnDownloadAvatar(PokerDownloadAvatarPacket* p, ClientConnection* pConn){
	PokerSession*		pSess = (PokerSession*)pConn->GetSession(false);
	if( !pSess || !pSess->IsActive() /*|| !pConn->IsAuthorized()*/ ) 
		return;

	p->m_imageData.SetBinarySize(0);
	int	nUserId		= pSess->GetUserId();
	int nAvatarId	= p->m_nAvatarId;
	int nAvatarUserId = p->m_nUserId;
	if( !pSess->IsAvatarDownloadingStarted() ){
		GrowableMemory memImageData(0);
		PokerSession*	pSessAvatarUser = PokerSession::GetSessionByUserId(nAvatarUserId, true);
		UserInfo*		pUserInfoAvatar = pSessAvatarUser ? pSessAvatarUser->GetUserInfo() : NULL;

		if( pUserInfoAvatar && pUserInfoAvatar->m_nAvatarId && pUserInfoAvatar->GetAvatarInfo() ){	
			PokerAvatar*	pAvatarInfo = pUserInfoAvatar->GetAvatarInfo();
			bool			bSuccess	= true;

			// Wrong avatar id received so send correct avatar id.
			if( nAvatarId != pAvatarInfo->m_nId ){
				p->m_nDownloaded		= 0;
				p->m_nFullBinarySize	= 0;
				p->m_bCompleted			= true;
				p->m_nAvatarId			= pAvatarInfo->m_nId;
				SendPacket(pConn, p);
				if( pSessAvatarUser ) SafePointer::Release(pSessAvatarUser);
				return;
				}

			if( pAvatarInfo->m_binImage.GetBinarySize() > 0 ){
				void* lpImageData	= pAvatarInfo->m_binImage.LockMemory();
				bSuccess			= pSess->StartDownloadingAvatarImage(lpImageData, ((int)pAvatarInfo->m_binImage.GetBinarySize()));
				pAvatarInfo->m_binImage.UnlockMemory();
				}
			else
				bSuccess = false;

			UserInfo::Release(pUserInfoAvatar);
			if( !bSuccess ){
				p->m_nAvatarId	= 0;
				p->m_nErrorCode = ES_ERROR_UNKNOWN;
				SendPacket(pConn, p);
				if( pSessAvatarUser ) SafePointer::Release(pSessAvatarUser);
				return;
				}
			else
				p->m_nAvatarId = pAvatarInfo->m_nId;
			}
		else{
			nAvatarId = m_pData->GetUserAvatarId(nAvatarUserId);
			// Wrong avatar id received so send correct avatar id.
			if( p->m_nAvatarId != nAvatarId && nAvatarId > 0 ){
				p->m_nDownloaded		= 0;
				p->m_nFullBinarySize	= 0;
				p->m_bCompleted			= true;
				p->m_nAvatarId			= nAvatarId;
				SendPacket(pConn, p);
				if( pSessAvatarUser ) SafePointer::Release(pSessAvatarUser);
				return;
				}

			if( !nAvatarId || 
				!m_pData->LoadAvatarData(nAvatarId, &memImageData) || 
				!pSess->StartDownloadingAvatarImage(memImageData.GetBufferPtr(), memImageData.GetBufferSize()) ){
				p->m_nErrorCode = ES_ERROR_UNKNOWN;
				SendPacket(pConn, p);
				if( pSessAvatarUser ) SafePointer::Release(pSessAvatarUser);
				return;
				}
			}
		if( pSessAvatarUser ) SafePointer::Release(pSessAvatarUser);
		}

	int nFullSize = 0, nBytesLeft = 0;
	bool bCompleted = false;
	if( pSess->DownloadAvatarImageData(&p->m_imageData, 512, nFullSize, nBytesLeft, bCompleted) ){
		p->m_shSize				= (short)p->m_imageData.GetBinarySize(); // Downloading binary size.
		p->m_nFullBinarySize	= nFullSize;							// full binary size.
		p->m_bCompleted			= bCompleted;
		p->m_nDownloaded		= (nFullSize - nBytesLeft);

		if( bCompleted )
			pSess->DiscardAvatarDownloading();

		p->m_nErrorCode			= ES_ERROR_SUCCESS;
		SendPacket(pConn, p);
		}
	else{
		// Error occured.
		pSess->DiscardAvatarDownloading();
		p->m_shSize				= 0;
		p->m_nFullBinarySize	= 0;
		p->m_bCompleted			= true;
		p->m_nDownloaded		= 0;

		p->m_nErrorCode = ES_ERROR_UNKNOWN;
		SendPacket(pConn, p);
		}
	}

// ======================================================================
//
//	Administration packet methods.
//
// ======================================================================

void
PokerServer::OnGetGameTables(PokerGetGameTablesPacket* p, ClientConnection* pConn){
	m_cashGames.OnGetGameTables(p, pConn);
	}

void
PokerServer::OnGetTournaments(PokerGetTournamentsPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnGetTournaments(p, pConn);
	}

void
PokerServer::OnGetClientConnections(PokerGetClientConnectionsPacket* p, ClientConnection* pConn){
	p->m_listResult.RemoveAll(true);
	int						nCt			= 0;
	ConnectedSocketInfo*	pArrConn	=	m_sockServer.GetConnectionInfos(nCt);
	if( nCt > 0 && pArrConn ){
		for(int i=0; i<nCt; i++){
			ClientConnectionInfoObject* pNew	= new ClientConnectionInfoObject();
			pNew->m_nClientIP					= (int)pArrConn[i].m_dwRemoteIp;
			pNew->m_nClientPort					= pArrConn[i].m_nRemotePort;
			pNew->m_nServerPort					= pArrConn[i].m_nLocalPort;
			pNew->m_nSessionId					= 0;
			pNew->m_nUserId						= 0;
			pNew->m_sUserName					= _T("");
			pNew->m_dtTimeConnected				= pArrConn[i].m_dtTimeConnected;
			pNew->m_nReceivedBytesCt			= (int)pArrConn[i].m_dwBytesReceived;
			pNew->m_nSentBytesCt				= (int)pArrConn[i].m_dwBytesSent;
			p->m_listResult.Add(pNew);

			SocketContext*	pSockConn		= pArrConn[i].m_pSockConn;
			if( pSockConn ){
				ClientConnection*	pClientConn	= (ClientConnection*)SafePointer::Retain((ClientConnection*)pSockConn->GetParam());
				if( pClientConn ){
					pClientConn->GetSessionId();
					PokerSession* pSess = (PokerSession*)pClientConn->GetSession(false);
					if( pSess ){
						pNew->m_nSessionId			= pSess->GetSessionId();
						pNew->m_nUserId				= pSess->GetUserId();
						pNew->m_sUserName			= pSess->GetUserName();
						}
					SafePointer::Release(pClientConn);
					}
				}
			}
		}

	if( pArrConn )
		delete [] pArrConn;

	p->m_nErrorCode = PokerErrorCodes::ES_ERROR_SUCCESS;
	SendPacket(pConn, p);
	}

void
PokerServer::OnSendAdminMessageByTable(PokerSendAdminMessageByTablePacket* p, ClientConnection* pConn){
	}

void
PokerServer::OnSendAdminMessageByUser(PokerSendAdminMessageByUserPacket* p, ClientConnection* pConn){
	}

void
PokerServer::OnCreateGameTable(PokerCreateGameTablePacket* p, ClientConnection* pConn){
	m_cashGames.OnCreateGameTable(p, pConn);
	}

void
PokerServer::OnForceClientSessionToQuit(PokerForceClientSessionToQuitPacket* p, ClientConnection* pConn){
	}

void
PokerServer::OnDisableServerConnect(PokerDisableServerConnectPacket* p, ClientConnection* pConn){
	}

void
PokerServer::OnDeactivateGameTable(PokerDeactivateGameTablePacket* p, ClientConnection* pConn){
	m_cashGames.OnDeactivateGameTable(p, pConn);
	}

void
PokerServer::OnBlockUser(PokerBlockUserPacket* p, ClientConnection* pConn){
	}

void
PokerServer::OnRegisterUserInTournament(PokerRegisterUserInTournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnRegisterUserInTournament(p, pConn);
	}

void
PokerServer::OnHideShowTournament(PokerHideShowTournamentPacket* p, ClientConnection* pConn){
	m_tournamentGames.OnHideShowTournament(p, pConn);
	}

// ======================================================================
//
//	Other usefull methods.
//
// ======================================================================

bool
PokerServer::AddTableConnection(int nTableId, int nTournamentId, ClientConnection* pConn, PokerTableController* pController){
	if( !pController ) return false;
	int	nIndex	= -1;
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	m_lockTableObj.Lock(); // Lock
	
	// Add connection into list. {{
	// < table id >< List<ClientConnection>* >
	List<ClientConnection>* pListConn = NULL;
	if( m_connByTableId.GetData(nValue, pListConn, nIndex) ){ // Found
		if( !pListConn )
			pListConn = new List<ClientConnection>();
		m_connByTableId.Set(nIndex, nValue, pListConn);
		}
	else{
		pListConn = new List<ClientConnection>();
		m_connByTableId.Add(nValue, pListConn);
		}
	pListConn->SetDestroyObjects(false);
	pListConn->Add				(pConn);
	// }}

	// Add table controller into list. {{
	List<PokerTableController>*	pListControllers = NULL;
	if( m_tableControllerByConn.GetData(pConn, pListControllers, nIndex) ){ // Found
		if( !pListControllers ){
			pListControllers = new List<PokerTableController>;
			pListControllers->SetDestroyObjects(false);
			m_tableControllerByConn.SetData(nIndex, pListControllers);
			}
		pListControllers->Add(pController);
		}
	else{
		pListControllers = new List<PokerTableController>;
		pListControllers->SetDestroyObjects(false);
		pListControllers->Add(pController);
		m_tableControllerByConn.Add(pConn, pListControllers);
		}
	// }}
	m_lockTableObj.Unlock(); // Unlock

	// Register table as opened by connection
	PokerSession* pSess = (PokerSession*)pConn->GetSession(true); // Retain
	if( pSess ){
		pSess->OpenTable(nTableId, nTournamentId);
		SafePointer::Release(pSess); // Release
		}
	return true;
	}

bool
PokerServer::RemoveTableConnection(int nTableId, int nTournamentId, ClientConnection* pConn, PokerTableController* pController){
	if( !pController ) return false;
	int		nIndex = -1;
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);

	// Unregister table as opened by connection
	PokerSession* pSess = (PokerSession*)pConn->GetSession(true); // Retain session pointer.
	if( pSess ){
		//pSess->TableCloseAndUnseat(nTableId, nTournamentId);
		pSess->CloseTable(nTableId, nTournamentId);
		SafePointer::Release(pSess); // Release session pointer.
		}
	
	m_lockTableObj.Lock(); // Lock
	// Remove connection. {{
	List<ClientConnection>* pListConnections = NULL;
	if( m_connByTableId.GetData(nValue, pListConnections, nIndex) ){
		if( pListConnections )
			pListConnections->Remove(pConn);
		}
	// }}

	// Remove game controller class. {{
	List<PokerTableController>* pListControlers = NULL;
	if( m_tableControllerByConn.GetData(pConn, pListControlers, nIndex) ){
		if( pListControlers )
			pListControlers->Remove(pController);
		}
	// }}
	m_lockTableObj.Unlock(); // Unlock
	return true;
	}

bool
PokerServer::RemoveConnectionsByTable(int nTableId, int nTournamentId, PokerTableController* pController){
	int nIndex = -1, nIndex1 = -1;
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	m_lockTableObj.Lock(); // Lock

	// Remove connections by table id. {{
	List<ClientConnection>* pListConnections = NULL;
	if( m_connByTableId.GetData(nValue, pListConnections, nIndex) ){
		if( pListConnections ){
			for(int i=0; i<pListConnections->GetCount(); i++){
				ClientConnection* pConn = pListConnections->GetAt(i);
				if( pConn ){
					// Close table and unseat. {{
					PokerSession* pSess = (PokerSession*)pConn->GetSession(true);
					if( pSess ){
						pSess->TableCloseAndUnseat(nTableId, nTournamentId);
						SafePointer::Release(pSess);
						}
					// }}
					List<PokerTableController>* pListControlers = NULL;
					if( m_tableControllerByConn.GetData(pConn, pListControlers, nIndex1) && pListControlers )
						pListControlers->Remove(pController, false);
					}
				}
			pListConnections->RemoveAll(false);
			delete pListConnections;
			}
		m_connByTableId.Delete(nIndex);
		}
	// }}
	m_lockTableObj.Unlock(); // Unlock
	return true;
	}

bool
PokerServer::RemoveConnection(ClientConnection* pConn, bool bUnseatFromTable /*= true*/){
	int nIndex = -1;
	if( !pConn ) return false;

	PokerSession* pSess = (PokerSession*)pConn->GetSession(true); // Retain session pointer.
	AutoSortedArrayInt64	arrSeatIndexByTableId;
	if( pSess ){
		pSess->GetSeatedTableIds(arrSeatIndexByTableId);
		SafePointer::Release(pSess); // Release session pointer.
		}
	
	List<PokerTableController>* pListControllers = NULL;
	PokerTableController* pController = NULL;
	m_lockTableObj.Lock(); // Lock
	if( m_tableControllerByConn.GetData(pConn, pListControllers, nIndex) ){ // List of game controllers (tables)
		if( pListControllers ){
			__int64 nValue = 0;
			int nIndex1 = -1, nLoop = 0, nTableId = 0, nTournamentId = 0, nSeat = 0;
			while( nLoop < pListControllers->GetCount() ){
				List<ClientConnection>* pListConn = NULL;
				pController		= pListControllers->GetAt(nLoop);
				nTableId		= pController->GetTableId();
				nTournamentId	= pController->GetTournamentId();
				nValue			= MAKEINT64(nTableId, nTournamentId);
				if( m_connByTableId.GetData(nValue, pListConn, nIndex1) ){
					if( pListConn ){ // Unseat from table.
						if( bUnseatFromTable ){
							int nIndex = arrSeatIndexByTableId.IndexOf(nValue);
							nSeat		= (nIndex != -1) ? (int)arrSeatIndexByTableId.GetData(nIndex) : -1;
							if( nSeat != -1 ){ // Empty seat
								pController->EmptySeat(nSeat);
								}
							}
						pListConn->Remove(pConn, false); // Remove connection.
						}
					}
				nLoop ++;
				}
			if( pListControllers ){
				pListControllers->RemoveAll(false);
				delete pListControllers;
				}
			m_tableControllerByConn.Delete(nIndex); // Remove connection.
			}
		}
	m_lockTableObj.Unlock(); // Unlock
	return true;
	}

bool
PokerServer::GetTableConnections(int nTableId, int nTournamentId, List<ClientConnection>*& pListConn){
	int nIndex = -1;
	if( m_connByTableId.GetData(MAKEINT64(nTableId, nTournamentId), pListConn, nIndex) )
		return true;
	return false;
	}

bool
PokerServer::GetConnectionTables(ClientConnection* pConn, List<PokerTableController>*& pListControllers){
	int nIndex = -1;
	if( m_tableControllerByConn.GetData(pConn, pListControllers, nIndex) ){
		return true;
		}
	return false;
	}

bool
PokerServer::IsTableOpenedByConn(int nTableId, int nTournamentId, ClientConnection* pConn){
	m_lockTableObj.Lock(); // Lock
	int		nIndex = -1;
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	List<ClientConnection>* pListConn = NULL;
	if( m_connByTableId.GetData(nValue, pListConn, nIndex) ){
		if( pListConn && pListConn->IndexOf(pConn) != -1 ){
			m_lockTableObj.Unlock(); // Unlock
			return true;
			}
		}
	m_lockTableObj.Unlock(); // Unlock
	return false;
	}

bool
PokerServer::RemoveAllGameControllersByConnection(){
	m_lockTableObj.Lock(); // Lock
	int nLoop = 0, nCt = m_tableControllerByConn.GetCount();
	while( nLoop < nCt ){
		List<PokerTableController>* pListControllers = m_tableControllerByConn.GetData(nLoop);
		if( pListControllers ){
			pListControllers->SetDestroyObjects(false);
			delete pListControllers;
			}
		nLoop ++;
		}
	m_tableControllerByConn.DeleteAll();
	m_lockTableObj.Unlock(); // Unlock
	return true;
	}

bool
PokerServer::RemoveAllConnectionsByTableId(){
	m_lockTableObj.Lock(); // Lock
	int nLoop = 0, nCt = m_connByTableId.GetCount();
	while( nLoop < nCt ){
		List<ClientConnection>* pListConn = m_connByTableId.GetData(nLoop);
		if( pListConn ){
			pListConn->SetDestroyObjects(false);
			delete pListConn;
			}
		nLoop ++;
		}
	m_connByTableId.DeleteAll();
	m_lockTableObj.Unlock(); // Unlock
	return true;
	}

bool
PokerServer::GetBalanceByAccount(int nAccountType, int nUserId, int nCurrencyId, INT_CHIPS& nBalance){
	switch( nAccountType ){
		case AccountType::MainBalance:{
			MaximumWebService webService;
			webService.SetServicePort(m_pPokerGameConfig->m_nWebServicePort);
			webService.SetServiceURL(m_pPokerGameConfig->m_sWebServiceURL);
			double dBalance = webService.GetUserBalance(nUserId);
			nBalance = Serializable::ConvertMoney(dBalance);
			return true;
			}
		case AccountType::PokerBalance:{
			nBalance = m_pData->GetUserBalance(nUserId, nCurrencyId);
			return true;
			}
		}
	return false;
	}

bool
PokerServer::TransferMoneyFromMainBalance(int nUserId, INT_CHIPS nAmount, int nCurrencyId, INT_CHIPS& nBalanceNew, INT_CHIPS& nMainBalance){
	/*
	if( m_pData ){
		int nEntryId = 0;
		MaximumWebService webService;
		webService.SetServicePort(m_pPokerGameConfig->m_nWebServicePort);
		webService.SetServiceURL(m_pPokerGameConfig->m_sWebServiceURL);
		if( !webService.WithdrawMoney(nUserId, nAmount, nMainBalance, nEntryId) ){
			return false; // WebService::WithdrawMoney not completed successfully.
			}

		double dMainBalance = (double)(nMainBalance/100.0);
		bool bRet = m_pData->TransferMoneyFromMainBalance(nUserId, nAmount, nCurrencyId, dMainBalance, nEntryId, nBalanceNew);
		if( !bRet ){
			// Delete transfer entry.
			webService.DeleteTransferEntry(nEntryId);
			}
		return bRet;
		}*/
	return false;
	}

bool
PokerServer::TransferMoneyToMainBalance(int nUserId, INT_CHIPS nAmount, int nCurrencyId, INT_CHIPS& nBalanceNew, INT_CHIPS& nMainBalance){
	/*
	if( m_pData ){
		int nBalance = m_pData->GetUserBalance(nUserId, nCurrencyId);
		if( nBalance < nAmount ){
			return false;
			}

		int nEntryId = 0;
		MaximumWebService webService;
		webService.SetServicePort(m_pPokerGameConfig->m_nWebServicePort);
		webService.SetServiceURL(m_pPokerGameConfig->m_sWebServiceURL);
		if( !webService.DepositMoney(nUserId, nAmount, nMainBalance, nEntryId) ){
			return false; // WebService::DepositMoney not completed successfully.
			}

		double dMainBalance = (double)(nMainBalance/100.0);
		bool bRet = m_pData->TransferMoneyToMainBalance(nUserId, nAmount, nCurrencyId, dMainBalance, nEntryId, nBalanceNew);
		if( !bRet ){
			// Delete transfer entry.
			webService.DeleteTransferEntry(nEntryId);
			}
		return bRet;
		}*/
	return false;
	}

bool
PokerServer::IsAllowedMultiUserIp(DWORD dwIp){
	return m_pPokerGameConfig->IsAllowedMultiUser(dwIp);
	}

bool
PokerServer::IsDontRestrictIPUser(_String sUserName){
	return m_pPokerGameConfig->IsDontRestrictIPUser(sUserName);
	}

PokerSession*
PokerServer::SeatUserSession(int nUserId, int nTableId, int nSeatIndex, int nTournamentId /*= 0*/, INT_CHIPS nSeatAmount /*= 0*/, int nRebuyCt /*= 0*/, int nAddonCt /*= 0*/){
	if( nUserId < 0 ) return NULL;

	PokerSession* pSessByUser = PokerSession::GetSessionByUserId(nUserId, true);
	if( pSessByUser ){
		bool bOpened = false;
		if( pSessByUser->GetSeatIndex(nTableId, bOpened, nTournamentId) > -1 )
			return NULL; // Already seated on that table.

		if( nTournamentId == 0 ){
			PokerSeatAtTablePacket packet;
			packet.m_cSeatIndex		= (char)nSeatIndex;
			packet.m_nChipAmount	= (nSeatAmount <= 0 ? 100 : nSeatAmount);
			packet.m_nSeatSessionId = 0;
			packet.m_nSessionId		= pSessByUser->GetSessionId();
			packet.m_nTableId		= nTableId;
			packet.m_nTournamentId = 0;
			if( m_cashGames.GetGameController(nTableId) )
				m_cashGames.GetGameController(nTableId)->OnSeatAt(pSessByUser, NULL, &packet);
			}

		// Register session in tournament.
		if( nTournamentId > 0 )
			pSessByUser->RegisterInTournament(nTournamentId, nRebuyCt, nAddonCt);

		pSessByUser->SeatAt(nTableId, nSeatIndex, nTournamentId); // Set user session
		return pSessByUser;
		}

	UserInfo*			pUser		= m_pData->LoadUserInfo(nUserId);
	if( !pUser ) return NULL;
	PokerSession*		pSessNew	= (PokerSession*)Session::CreateNewSession(this, NULL, true);
	ASSERT(pSessNew);
	pSessNew->SetUserInfo	(pUser);
	delete pUser;

	if( nTournamentId == 0 ){
		PokerSeatAtTablePacket packet;
		packet.m_cSeatIndex		= (char)nSeatIndex;
		packet.m_nChipAmount	= (nSeatAmount <= 0 ? 100 : nSeatAmount);
		packet.m_nSeatSessionId = 0;
		packet.m_nSessionId		= pSessNew->GetSessionId();
		packet.m_nTableId		= nTableId;
		packet.m_nTournamentId	= 0;
		if( m_cashGames.GetGameController(nTableId) )
			m_cashGames.GetGameController(nTableId)->OnSeatAt(pSessNew, NULL, &packet);
		}

	pSessNew->SeatAt		(nTableId, nSeatIndex, nTournamentId); // Set user session

	// Register session in tournament.
	if( nTournamentId > 0 )
		pSessNew->RegisterInTournament(nTournamentId, nRebuyCt, nAddonCt);
	return pSessNew;
	}
