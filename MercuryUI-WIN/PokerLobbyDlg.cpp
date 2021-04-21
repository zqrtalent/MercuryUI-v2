// PokerLobbyDlg.cpp : implementation file
//

#include "Array/List.h"
#include "Path/Path.h"
#include "PokerLobbyDlg.h"
#include "Data/DataFieldCopier.h"

#include "Controls/ESChildControlTabCtrl.h"
#include "Controls/ESChildControlList.h"
#include "Controls/ESChildControlImage.h"
#include "Controls/ESChildControlImageButton.h"
#include "Controls/ESChildControlCheckBox.h"
#include "Controls/ESSimpleRenderView.h"

//////////////////////////////////////////////////////
//
//	class PokerLobbyDlg dialog
//
/////////////////////////////////////////////////////
PokerLobbyDlg::PokerLobbyDlg() : MercuryBaseView(){
	m_hIcon = NULL;// AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pLobbyWnd						= NULL;
	m_fHeightAndWidthRatio			= 0.0f;

	m_szDefault	.SetSize(1000, 600);
	m_szMin		.SetSize(800, 500);
	m_szMax		.SetSize(1100, 700);
	}

// PokerLobbyDlg message handlers
void
PokerLobbyDlg::Setup(){
	//m_pMainWnd				= m_pLobbyWnd;
	//m_pLobbyWnd->SetConnectedMode	(m_client->IsConnected(), false);

	// Load window placement.
	//LoadWindowPlacement(_T("Lobby"), this);
	
	// Create child window.
	MercuryBaseView::Setup();

	// Load from file. {{
	if(this->LoadFromDesignFile(_T("design\\lobby.des"), false, _T(""), false, m_szDefault, m_ptSetupPos.x, m_ptSetupPos.y) ){
		MercuryBaseViewInfo* pViewInfo = GetViewInfo();
		m_szDefault		= pViewInfo->m_szDefault;
		m_szMin			= pViewInfo->m_szMin;	
		m_szMax			= pViewInfo->m_szMax;

		ESChildControlList* pListOfTables		= (ESChildControlList*)GetChildControlByName(_T("_cashTablesList"));
		if( pListOfTables ){
			pListOfTables->AllowInPlaceEdit(false);
			//pListOfTables->AddEventListener(this, EventCode_ListRowChange, (ControlEventListener)&PokerLobbyDlg::OnCashTableList_ItemChange);
			//pListOfTables->AddEventListener(this, EventCode_ListRowDblClk, (ControlEventListener)&PokerLobbyDlg::OnCashTableList_ItemDblClk);
			/*
			m_pRowSetCashTables	= new VirtualRowSet();
			m_pRowSetCashTables->AddField	(_T("ID"), FieldType::FTInt32);
			m_pRowSetCashTables->AddFieldEx	(_T("NAME"), FieldType::FTChar, 64);
			m_pRowSetCashTables->AddFieldEx	(_T("GAME_NAME"), FieldType::FTChar, 64);
			m_pRowSetCashTables->AddFieldEx	(_T("STAKES"), FieldType::FTChar, 32);
			m_pRowSetCashTables->AddFieldEx	(_T("PLAYERS"), FieldType::FTChar, 32);
			m_pRowSetCashTables->AddFieldEx	(_T("AVGPOT"), FieldType::FTChar, 32);
			m_pRowSetCashTables->AddFieldEx	(_T("PLAYER_FLOP"), FieldType::FTChar, 32);
			m_pRowSetCashTables->AddFieldEx	(_T("WAIT_PLAYERS"), FieldType::FTChar, 32);
			m_pRowSetCashTables->AddFieldEx	(_T("HAND_HOUR"), FieldType::FTChar, 32);
			m_pRowSetCashTables->AddField	(_T("MAX_PLAYERS_CT"), FieldType::FTInt32);
			m_pRowSetCashTables->AddField	(_T("PLAYERS_CT"), FieldType::FTInt32);
			m_pRowSetCashTables->AddField	(_T("_STATE"), FieldType::FTInt32);
			m_pRowSetCashTables->AddField	(_T("BUYIN_MIN"), FieldType::FTInt32);
			m_pRowSetCashTables->AddField	(_T("BUYIN_MAX"), FieldType::FTInt32);
			m_pRowSetCashTables->AddField	(_T("CURRENCY_ID"), FieldType::FTInt32);
			m_pRowSetCashTables->AddField	(_T("BLIND"), FieldType::FTInt32);
			m_pRowSetCashTables->AddField	(_T("BIG_BLIND"), FieldType::FTInt32);
			m_pRowSetCashTables->Create		();
			m_pRowSetCashTablesCopy			= m_pRowSetCashTables->GetInstanceCopy();
			pListOfTables->SetRowSet		(m_pRowSetCashTables, false, false);*/
			}

		ESChildControlList* pListOfTournaments	= (ESChildControlList*)GetChildControlByName(_T("_tournamentsList"));
		if( pListOfTournaments ){
			pListOfTournaments->AllowInPlaceEdit(false);
			//pListOfTournaments->AddEventListener(this, EventCode_ListRowChange, (ControlEventListener)&PokerLobbyDlg::OnTournamentsList_ItemChange);
			//pListOfTournaments->AddEventListener(this, EventCode_ListRowDblClk, (ControlEventListener)&PokerLobbyDlg::OnTournamentsList_ItemDblClk);

			// Row customize callback method.
			//pListOfTournaments->SetRowCustomizeCallback(&PokerLobbyDlg::TournamentListRowCustomize, this);
			/*
			m_pRowSetTournaments	= new VirtualRowSet();
			m_pRowSetTournaments->AddField	(_T("TOURNAMENT_ID"), FieldType::FTInt32);
			m_pRowSetTournaments->AddFieldEx	(_T("NAME"), FieldType::FTChar, 64);
			m_pRowSetTournaments->AddFieldEx	(_T("DESC"), FieldType::FTChar, 255);
			m_pRowSetTournaments->AddFieldEx	(_T("GAME"), FieldType::FTChar, 64);
			m_pRowSetTournaments->AddFieldEx	(_T("DATE"), FieldType::FTChar, 32);
			m_pRowSetTournaments->AddFieldEx	(_T("BUYIN"), FieldType::FTChar, 32);
			m_pRowSetTournaments->AddFieldEx	(_T("PLAYERS"), FieldType::FTChar, 32);
			m_pRowSetTournaments->AddFieldEx	(_T("TOURNAMENT_STATUS"), FieldType::FTChar, 64);
			m_pRowSetTournaments->AddField		(_T("T_STATUS"), FieldType::FTInt32);
			m_pRowSetTournaments->AddField		(_T("REGOPEN_STATUS"), FieldType::FTBool);
			m_pRowSetTournaments->AddField		(_T("_STATE"), FieldType::FTInt32);
			m_pRowSetTournaments->Create		();
			m_pRowSetTournamentsCopy = m_pRowSetTournaments->GetInstanceCopy();
			pListOfTournaments->SetRowSet		(m_pRowSetTournaments, false, false);*/
			}
		/*
		ESChildControlList* pListOfTableInfo	= (ESChildControlList*)m_pMainWnd->GetChildControlByName(_T("_cashTableInfoList"));
		if( pListOfTableInfo ){
			pListOfTableInfo->AllowInPlaceEdit(false);

			m_pRowSetCashTablePlayers			= new VirtualRowSet();
			m_pRowSetCashTablePlayers->AddField		(_T("USER_ID"), FieldType::FTInt32);
			m_pRowSetCashTablePlayers->AddFieldEx	(_T("USER_NAME"), FieldType::FTChar, 64);
			m_pRowSetCashTablePlayers->AddFieldEx	(_T("SEAT_AMOUNT"), FieldType::FTChar, 32);
			m_pRowSetCashTablePlayers->AddField		(_T("_STATE"), FieldType::FTInt32);
			m_pRowSetCashTablePlayers->Create		();
			pListOfTableInfo->SetRowSet				(m_pRowSetCashTablePlayers, false, false);
			}

		ESChildControlImageButton* pOpenTableButton	= (ESChildControlImageButton*)m_pMainWnd->GetChildControlByName(_T("_openTableButton"));
		if( pOpenTableButton ){
			pOpenTableButton->AddEventListener	(this, EventCode_ImageButtonClick, (ControlEventListener)&PokerLobbyDlg::OnOpenTableButton_Click);
			pOpenTableButton->DisableButton		(false);
			}
		
		m_pMainWnd->AddEventListenerByName(this, _T("_cashGamesTab"),				EventCode_TabItemChange, (ControlEventListener)&PokerLobbyDlg::OnGameTypeTab_ItemChange);
		m_pMainWnd->AddEventListenerByName(this, _T("_cashLimitsTab"),				EventCode_TabItemChange, (ControlEventListener)&PokerLobbyDlg::OnGameCashLimitsTab_ItemChange);
		m_pMainWnd->AddEventListenerByName(this, _T("_tabTableInfoSwitch"),			EventCode_TabItemChange, (ControlEventListener)&PokerLobbyDlg::OnTableInfoSwitchTabActive);

		m_pMainWnd->AddEventListenerByName(this, _T("_openTournamentLobby"),		EventCode_ImageButtonClick, (ControlEventListener)&PokerLobbyDlg::OnOpenTournamentButton_Click);
		m_pMainWnd->AddEventListenerByName(this, _T("_tournamentRegisterUnreg"),	EventCode_ImageButtonClick, (ControlEventListener)&PokerLobbyDlg::OnTournamentRegUnregButton_Click);
		m_pMainWnd->AddEventListenerByName(this, _T("_logoutButton"),				EventCode_ImageButtonClick, (ControlEventListener)&PokerLobbyDlg::OnLogoutButton_Click);
		m_pMainWnd->AddEventListenerByName(this, _T("_cashierButton"),				EventCode_ImageButtonClick, (ControlEventListener)&PokerLobbyDlg::OnCashierButton_Click);
		m_pMainWnd->AddEventListenerByName(this, _T("_loginButton"),				EventCode_ImageButtonClick, (ControlEventListener)&PokerLobbyDlg::OnLoginButton_Click);
		m_pMainWnd->AddEventListenerByName(this, _T("_imageAvatar"),				EventCode_ImageClick, (ControlEventListener)&PokerLobbyDlg::OnAvatarImage_Click);
		m_pMainWnd->AddEventListenerByName(this, _T("_checkHideEmptyTables"),		EventCode_CheckBoxClick, (ControlEventListener)&PokerLobbyDlg::OnHideEmptyTables_Check);
		m_pMainWnd->AddEventListenerByName(this, _T("_checkHideFullTables"),		EventCode_CheckBoxClick, (ControlEventListener)&PokerLobbyDlg::OnHideFullTables_Check);
		m_pMainWnd->AddEventListenerByName(this, _T("_upComingTournamentsCheck"),	EventCode_CheckBoxClick, (ControlEventListener)&PokerLobbyDlg::OnUpcommingTournaments_Check);
		*/}
	// }}
	
	
	}

void
PokerLobbyDlg::SetTitle(){
	//Config* pConf = Config::GetInstance();
	_string sTitle;
	//sTitle.Format(_T("Maximum Poker Client v%s build:%d"), pConf->m_sAppVer.GetBuffer(), pConf->m_nBuildNumber);
	//sTitle.Format(_T("... Poker Client v%s build:%d"), pConf->m_sAppVer.GetBuffer(), pConf->m_nBuildNumber);
	//SetWindowText(sTitle);
	}
/*
void
PokerLobbyDlg::OnOK(){
	// Do not allow to quit on OK.
	}

void
PokerLobbyDlg::OnCancel(){
	PostMessage(WM_CLOSE, 0L, 0L);
	}*/

void
PokerLobbyDlg::OnDestroy(){
	ESFrameBase::OnDestroy();
	}

void
PokerLobbyDlg::OnClose(){
	//m_pLobbyWnd->UseForegroundColor(1, true);
	MercuryBaseView::OnClose();
	}