#include "stdafx.h"
#include "..\Utility\GrowableMemory.h"
#include "PokerBot.h"

///////////////////////////////////////////////////////////
///
///	class PokerBot
///
///////////////////////////////////////////////////////////

PokerBot::PokerBot(HWND hOwnerWnd /*= NULL*/, _String sIpAddress /*= _T("127.0.0.1")*/, int nPort /*= 20001*/, int nReconnectTime /*= 5000*/) :  
	PokerClient(hOwnerWnd, sIpAddress, nPort, nReconnectTime) {

	m_nTournamentId = 0;

	// Initialize packet listener methods. ###############################
	PacketListenerMethod method;

	__asm{
		lea eax, PokerBot::OnLogin;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Login, this, method);

		__asm{
		lea eax, PokerBot::OnLogout;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_Logout, this, method);

	__asm{
		lea eax, PokerBot::OnOpenTable;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerOpenTable, this, method);

	__asm{
		lea eax, PokerBot::OnLeaveTable;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerLeaveTable, this, method);

	__asm{
		lea eax, PokerBot::OnReserveSeat;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerReserveSeat, this, method);

	__asm{
		lea eax, PokerBot::OnSeatAt;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatAt, this, method);

	__asm{
		lea eax, PokerBot::OnAddChips;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAddChips, this, method);

	__asm{
		lea eax, PokerBot::OnJoinWaitingList;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerJoinWaitingList, this, method);

	__asm{
		lea eax, PokerBot::OnAddChips;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAddChips, this, method);

	__asm{
		lea eax, PokerBot::OnAction;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAction, this, method);

	__asm{
		lea eax, PokerBot::OnTableInfo;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerTableInfo, this, method);

	__asm{
		lea eax, PokerBot::OnReturnBack;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerReturnBack, this, method);

	__asm{
		lea eax, PokerBot::OnSitOut;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSitOut, this, method);

	__asm{
		lea eax, PokerBot::OnPostBlind;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerPostBlind, this, method);

	__asm{
		lea eax, PokerBot::OnSetActiveSeat;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetActive, this, method);

	__asm{
		lea eax, PokerBot::OnSeatStatusChanged;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatStatusChanged, this, method);

	__asm{
		lea eax, PokerBot::OnHandStarted;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerHandStarted, this, method);

	__asm{
		lea eax, PokerBot::OnRoundStarted;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerRoundStarted, this, method);

	__asm{
		lea eax, PokerBot::OnAct;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerAct, this, method);

	__asm{
		lea eax, PokerBot::OnHandFinished;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerHandFinished, this, method);

	__asm{
		lea eax, PokerBot::OnGetLobbyInfo;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetLobbyInfo, this, method);

	__asm{
		lea eax, PokerBot::OnShowMuckCards;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerShowMuckCards, this, method);

	__asm{
		lea eax, PokerBot::OnSetOption;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetOption, this, method);

	__asm{
		lea eax, PokerBot::OnSeatOnlineChanged;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatOnlineChanged, this, method);

	__asm{
		lea eax, PokerBot::OnSeatOnlineChanged;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSeatOnlineChanged, this, method);

	__asm{
		lea eax, PokerBot::OnGetHandHistory;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerGetHandHistoryPacket, this, method);

	__asm{
		lea eax, PokerBot::OnQuitClient;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerQuitClient, this, method);

	__asm{
		lea eax, PokerBot::OnSetAvatar;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerSetAvatar, this, method);

	__asm{
		lea eax, PokerBot::OnChatMessage;
		mov method, eax;
		};
	m_pPacketHandler->AddPacketHandler(PacketCodes::_PokerChatMessage, this, method);
	// ###################################################################
	} 

PokerBot::~PokerBot(){
	StopBot();
	}

bool
PokerBot::StartBot(PokerBotInfo* pInfo){
	if( !pInfo ) return false;
	pInfo->Copy(&m_info);

	// Connect. {{
	if( !Connect() ){
		return false;
		}
	// }}

	// Handshake. {{
	HandShakePacket* p = NULL;
	HandShake();
	if( WaitForResponse(PacketCodes::_HandShake, (BasePacket**)&p, 4) ){
		// Manually call packet handler method(s)
		CallPacketHandler(p, (void*)this);
		if( p ) delete p;
		}
	else
		return false;
	// }}

	// User Authentication. {{
	LoginPacket* p1 = NULL;
	Login(m_info.m_sUserName, m_info.m_sPassword);
	if( WaitForResponse(PacketCodes::_Login, (BasePacket**)&p1, 4) ){
		// Manually call packet handler method(s)
		CallPacketHandler(p1, (void*)this);
		if( !p1 || p1->m_nErrorCode != ES_ERROR_SUCCESS ){
			if( p1 ) delete p1;
			return false;
			}
		else
			delete p1;
		}
	else
		return false;
	// }}

	// Open random table.
	PokerOpenTablePacket* p2 = NULL;
	OpenRandomTable(m_info.m_nGameTypeId, m_info.m_nSubGameTypeId, true);
	if( WaitForResponse(PacketCodes::_PokerOpenTable, (BasePacket**)&p2, 60) ){
		// Manually call packet handler method(s)
		if( p2 && p2->m_info.GetCount() > 0 )
			CallPacketHandler(p2, (void*)this);
		if( p2 ) delete p2;
		}
	// }}
	return true;
	}

bool
PokerBot::StopBot(){
	if( IsLoggedIn() && IsConnected() ){
		LogoutPacket* p = NULL;
		Logout();
		if( WaitForResponse(PacketCodes::_Logout, (BasePacket**)&p, 4) ){
			// Manually call packet handler method(s)
			CallPacketHandler(p, (void*)this);
			if( p ) delete p;
			}
		}

	Disconnect();

	m_listTableInfos		.RemoveAll(true);
	m_arrPokerTableInfoById	.DeleteAll();
	return true;
	}

void
PokerBot::AddTableInfo(int nTableId, PokerTableInfo* pInfo){
	if( !pInfo ) return;
	PokerTableInfo* pInfoNew = new PokerTableInfo();
	pInfo->Copy(pInfoNew);

	int nIndex = m_arrPokerTableInfoById.IndexOf((void*)nTableId);
	if( nIndex != -1 ){
		PokerTableInfo* pInfoOld = (PokerTableInfo*)m_arrPokerTableInfoById.GetData(nIndex);
		if( pInfoOld )
			m_listTableInfos.Remove(pInfoOld);

		m_arrPokerTableInfoById.Set(nIndex, (void*)nTableId, (void*)pInfoNew);
		m_listTableInfos.Add(pInfoNew);
		}
	else{
		m_listTableInfos.Add(pInfoNew);
		m_arrPokerTableInfoById.Add((void*)nTableId, (void*)pInfoNew);
		}
	}

bool
PokerBot::RemoveTableInfo(int nTableId){
	int nIndex = m_arrPokerTableInfoById.IndexOf((void*)nTableId);
	if( nIndex != -1 ){
		PokerTableInfo* pInfoOld = (PokerTableInfo*)m_arrPokerTableInfoById.GetData(nIndex);
		if( pInfoOld )
			m_listTableInfos.Remove(pInfoOld);
		m_arrPokerTableInfoById.Delete(nIndex);
		return true;
		}
	return false;
	}

PokerTableInfo*	
PokerBot::GetTableInfo(int nTableId){
	int nIndex = m_arrPokerTableInfoById.IndexOf((void*)nTableId);
	if( nIndex != -1 ){
		PokerTableInfo* pInfo = (PokerTableInfo*)m_arrPokerTableInfoById.GetData(nIndex);
		return pInfo;
		}
	return NULL;
	}


bool
PokerBot::OpenRandomTable(int nGameType, int nGameSubType, bool bReserveAnySeat){
	if( !IsConnected() )
		return false;
	PokerOpenRandomTablePacket* packet	= new PokerOpenRandomTablePacket();
	packet->m_nGameType			= nGameType;
	packet->m_nGameSubType		= nGameSubType;
	packet->m_bReserveSeat		= bReserveAnySeat;
	SendPacket(packet);
	delete packet;
	return true;
	}

///////////////////////////////////////////////////////////
///
///	Virtual events.
///
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
///
///	Public server response events.
///
///////////////////////////////////////////////////////////

void
PokerBot::OnLogin(LoginPacket* p, void* pParam){
	}

void
PokerBot::OnLogout(LogoutPacket* p, void* pParam){
	}

void	
PokerBot::OnOpenTable(PokerOpenTablePacket* p, void* pParam){
	if( p->m_nErrorCode != ES_ERROR_SUCCESS ) return;
	AddTableInfo(p->m_nTableId, p->m_info[0]);

	if( p->m_bSeatOrJoinWaitList ){
		// Seat at table.
		PokerTableInfo* pTableInfo = p->m_info[0];
		int nLoop = 0;
		while( nLoop < pTableInfo->m_cMaxPlayersCt ){
			// Empty seat.
			if( pTableInfo->m_seats.GetAt(nLoop) == NULL ){
				//SeatAt(p->m_nTableId, nLoop, pTableInfo->m_nMinBuyIn);
				break;
				}
			nLoop ++;
			}
		}
	}

void
PokerBot::OnLeaveTable(PokerLeaveTablePacket* p, void* pParam){
	RemoveTableInfo(p->m_nTableId);
	}

void
PokerBot::OnReserveSeat(PokerReserveSeatPacket* p, void* pParam){
	if (p->m_nErrorCode != ES_ERROR_SUCCESS)
		return;
	PokerTableInfo* pTableInfo = GetTableInfo(p->m_nTableId);
	if (pTableInfo){

		TRACE(" OnReserveSeat Amount = %08X  \r\n", p->m_nAmountMin);

		SeatAt(p->m_nTableId, (int)p->m_cSeat, p->m_nAmountMin);
		}
	}

void
PokerBot::OnSeatAt(PokerSeatAtTablePacket* p, void* pParam){
	}

void
PokerBot::OnAddChips(PokerAddChipsPacket* p, void* pParam){
	}

void
PokerBot::OnJoinWaitingList(PokerJoinWaitingListPacket* p, void* pParam){
	}

void
PokerBot::OnAction(PokerActionPacket* p, void* pParam){
	}

void
PokerBot::OnTableInfo(PokerTableInfoPacket* p, void* pParam){
	if( p->m_nErrorCode != ES_ERROR_SUCCESS ) return;
	AddTableInfo(p->m_nTableId, p->m_info[0]);

 	// Return back at table.
	PokerTableInfo* pTableInfo = p->m_info[0];
	if( pTableInfo ){
		int nLoop = 0;
		while( nLoop < pTableInfo->m_cMaxPlayersCt ){
			PokerTableSeatInfo* pSeat = pTableInfo->m_seats[nLoop];
			if( pSeat && pSeat->GetUserId() == m_nUserId ){
				if( pSeat->GetStatus() == PokerSeatStatus::SittedOut ){
					ReturnBack(p->m_nTableId, 0, pSeat->GetSeat());
					}
				break;
				}
			nLoop ++;
			}
		}
	}

void
PokerBot::OnReturnBack(PokerReturnBackPacket* p, void* pParam){
	}

void
PokerBot::OnSitOut(PokerSitOutPacket* p, void* pParam){
	}

void
PokerBot::OnPostBlind(PokerPostBlindPacket* p, void* pParam){
	Sleep(1000);

	PokerTableInfo* pInfo = GetTableInfo(p->m_nTableId);
	if( !pInfo ) return;

	Action(p->m_nTableId, m_nTournamentId, -1, 
		(p->m_cPostBlind == 0) ? PokerActionTypes::PostSB : (p->m_cPostBlind == 1 ? PokerActionTypes::PostBB : PokerActionTypes::PostBB_Plus_SB), 
		(p->m_cPostBlind == 0) ? pInfo->m_nSmallBlindAmount : (p->m_cPostBlind == 1 ? pInfo->m_nBigBlindAmount : (pInfo->m_nBigBlindAmount + pInfo->m_nSmallBlindAmount)));
}

void
PokerBot::OnSetActiveSeat(PokerSetActivePacket* p, void* pParam){
	}

void
PokerBot::OnSeatStatusChanged(PokerSeatStatusChangedPacket* p, void* pParam){
	}

void
PokerBot::OnHandStarted(PokerHandStartedPacket* p, void* pParam){
	}

void
PokerBot::OnRoundStarted(PokerRoundStartedPacket* p, void* pParam){
	}

void
PokerBot::OnAct(PokerActPacket* p, void* pParam){
	Sleep(1500);
	
	int		nLoop			= 0;
	int		nCt				= p->m_actions.GetCount();
	bool	bActivateSeat	= true;
	int		nTableId		= p->m_nTableId;
	while( nLoop < nCt ){
		PokerAction* action = p->m_actions.GetAt(nLoop);
		if( !action ){
			nLoop ++;
			continue;
			}

		_String sLabel;
		switch( action->m_cActionType ){
			case PokerActionTypes::Call:{
				Action(nTableId, m_nTournamentId, -1, PokerActionTypes::Call, action->m_nAmountMin);
				/*
				sLabel.Format(_T("Call %d.%02d"), action->m_nAmountMin/100, action->m_nAmountMin%100);
				pTableWnd->AddActionButton(ActionButton::ButtonCall, sLabel, 
					action->m_nAmountMin, action->m_nAmountMax, true);*/
				break;
				}
			case PokerActionTypes::Check:{
				Action(nTableId, m_nTournamentId, -1, PokerActionTypes::Check, action->m_nAmountMin);
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonCheck, _T("Check"), 
					action->m_nAmountMin, action->m_nAmountMax, true);
					*/
				break;
				}
		    case PokerActionTypes::Bet:{
				/*
				// Show slider. {{
				if( action->m_nAmountMin < action->m_nAmountMax ){
					pTableWnd->ShowSlider(action->m_nAmountMin, action->m_nAmountMin, action->m_nAmountMax, m_tableInfo.m_nBigBlindAmount, true);

					int nTotalPot	= pTableWnd->GetTotalPotAmount();
					int nPot12		= (int)(0.5f*nTotalPot);
					int nPot23		= (int)(0.66f*nTotalPot);
					int nPot		= nTotalPot;

					// Shortcut buttons.
					if( action->m_nAmountMax >= nPot12 )
						pTableWnd->AddShortcutButton(_T("1/2POT"), nPot12, true);
					if( action->m_nAmountMax >= nPot23 )
						pTableWnd->AddShortcutButton(_T("2/3POT"), nPot23, true);
					if( action->m_nAmountMax >= nPot )
						pTableWnd->AddShortcutButton(_T("POT"), nPot, true);
					pTableWnd->AddShortcutButton(_T("ALLIN"), -1, true);
					}
				// }}

				sLabel.Format(_T("Bet %d.%02d"), action->m_nAmountMin/100, action->m_nAmountMin%100);
				pTableWnd->AddActionButton(ActionButton::ButtonBet, sLabel, 
					action->m_nAmountMin, action->m_nAmountMax, true);
					*/
				break;
				}
			case PokerActionTypes::Fold:{
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonFold, _T("Fold"), 
					action->m_nAmountMin, action->m_nAmountMax, true);
				*/
				break;
				}
			case PokerActionTypes::Raise:{
				/*
				// Show slider. {{
				if( action->m_nAmountMin < action->m_nAmountMax ){
					pTableWnd->ShowSlider(action->m_nAmountMin, action->m_nAmountMin, action->m_nAmountMax, m_tableInfo.m_nBigBlindAmount, true);

					// Shortcut buttons.
					int nBB = m_tableInfo.m_nBigBlindAmount;
					if( action->m_nAmountMax >= nBB )
						pTableWnd->AddShortcutButton(_T("BB"), nBB, true);
					if( action->m_nAmountMax >= 2*nBB )
						pTableWnd->AddShortcutButton(_T("2BB"), 2*nBB, true);
					if( action->m_nAmountMax >= 3*nBB )
						pTableWnd->AddShortcutButton(_T("3BB"), 3*nBB, true);
					if( action->m_nAmountMax >= 5*nBB )
						pTableWnd->AddShortcutButton(_T("5BB"), 5*nBB, true);
					pTableWnd->AddShortcutButton(_T("ALLIN"), -1, true);
					}
				// }}
				sLabel.Format(_T("Raise %d.%02d"), action->m_nAmountMin/100, action->m_nAmountMin%100);
				pTableWnd->AddActionButton(ActionButton::ButtonRaise, sLabel, 
					action->m_nAmountMin, action->m_nAmountMax, true);
					*/
				break;
				}
			case PokerActionTypes::Muck:{
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonMuck, _T("Muck"), 
					action->m_nAmountMin, action->m_nAmountMax, true);
					*/
				break;
				}
		    case PokerActionTypes::ShowCards:{
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonShowCards, _T("Show Cards"), 
					action->m_nAmountMin, action->m_nAmountMax, true);*/
				break;
				}
			case PokerActionTypes::PostSB:{
				Action(nTableId, m_nTournamentId, -1, PokerActionTypes::PostBB, action->m_nAmountMin);
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonPostSB, _T("Post SB"), 
					action->m_nAmountMin, action->m_nAmountMax, true);*/
				break;
				}
			case PokerActionTypes::PostBB:{
				Action(nTableId, m_nTournamentId, -1, PokerActionTypes::PostBB, action->m_nAmountMin);
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonPostBB, _T("Post BB"), 
					action->m_nAmountMin, action->m_nAmountMax, true);*/
				break;
				}
			case PokerActionTypes::SitOut:{
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonSitOut, _T("SitOut"), 
					action->m_nAmountMin, action->m_nAmountMax, true);*/
				break;
				}
		    case PokerActionTypes::ReturnBack:{
				/*
				pTableWnd->AddActionButton(ActionButton::ButtonReturnBack, _T("I'm Back"), 
					action->m_nAmountMin, action->m_nAmountMax, true);*/
				break;
				}
			case PokerActionTypes::AllIn:{
				/*
				sLabel.Format(_T("All-In %d.%02d"), action->m_nAmountMin/100, action->m_nAmountMin%100);
				pTableWnd->AddActionButton(ActionButton::ButtonAllIn, sLabel, 
					action->m_nAmountMin, action->m_nAmountMax, true);*/
				break;
				}
			};
		nLoop ++;
		}
	}

void
PokerBot::OnHandFinished(PokerHandFinishedPacket* p, void* pParam){
	}

void
PokerBot::OnGetLobbyInfo(PokerGetLobbyInfoPacket* p, void* pParam){
	}

void
PokerBot::OnShowMuckCards(PokerShowMuckCardsPacket* p, void* pParam){
	}

void
PokerBot::OnSetOption(PokerSetOptionPacket* p, void* pParam){
	}

void
PokerBot::OnSeatOnlineChanged(PokerSeatOnlineChangedPacket* p, void* pParam){
	}

void
PokerBot::OnGetHandHistory(PokerGetHandHistoryPacket* p, void* pParam){
	}

void
PokerBot::OnQuitClient(PokerQuitClientPacket* p, void* pParam){
	}

void
PokerBot::OnSetAvatar(PokerSetAvatarPacket* p, void* pParam){
	}

void
PokerBot::OnChatMessage(PokerChatMessagePacket* p, void* pParam){
	}
