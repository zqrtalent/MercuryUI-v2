// PokerLobbyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Controls\MercuryGUI.h"
#include "..\..\Controls\ESChildControlLabel.h"
#include "..\..\Controls\ESChildControlCheckBox.h"
#include "..\..\Controls\ESChildControlImageButton.h"
#include "..\..\Controls\ESChildControlImage.h"
#include "..\..\Controls\ESChildControlTabCtrl.h"
#include "..\..\Controls\ESChildAvatar.h"

#include "..\..\Image\ImageHelper.h"
#include "..\..\Path\Path.h"
#include "PokerLobbyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerLobbyWnd

PokerLobbyWnd::PokerLobbyWnd() : MercuryBaseView(){
	m_bUseForegroundColor	= 0;
	m_crForeground			= RGB(0, 0, 0);
	m_nAlpha				= 50;

	m_bDesignerMode			= false;

	m_pClient				= NULL;
	m_bConnectedMode		= false;
	m_bLoginMode			= false;
	m_bOpenTableAllowed		= true;
	}

PokerLobbyWnd::~PokerLobbyWnd(){
	}

void
PokerLobbyWnd::UpdateUI(){
	}

bool
PokerLobbyWnd::AttachToFrame(ESFrameBase* pBaseFrameWnd){
	bool bRet = MercuryBaseView::AttachToFrame(pBaseFrameWnd);

	if( bRet ){
		}

	return bRet;
	}

// ######################################################
//
//	Operations.
//
// ######################################################

void
PokerLobbyWnd::SetUserName(CString sUserName, bool bRedraw){
	ESChildControlLabel* pUser = (ESChildControlLabel*)GetChildControlByName(_T("_userNameLabel"));
	if( !pUser ) return;
	pUser->SetLabel(sUserName, bRedraw);
	}

void
PokerLobbyWnd::SetUserBalance(int nBalance, bool bRedraw){
	ESChildControlLabel* pUserBalance = (ESChildControlLabel*)GetChildControlByName(_T("_userBalanceLabel"));
	if( !pUserBalance ) return;

	CString sBalance;
	sBalance.Format(_T("%d.%02d GEL"), nBalance/100, nBalance%100);
	pUserBalance->SetLabel(sBalance, bRedraw);
	}

void
PokerLobbyWnd::SetUserInPlayAmount(int nInPlayAmount, bool bRedraw){
	ESChildControlLabel* pInPlayBalance = (ESChildControlLabel*)GetChildControlByName(_T("_inPlayBalanceLabel"));
	if( !pInPlayBalance ) return;

	CString sBalance;
	sBalance.Format(_T("%d.%02d GEL"), nInPlayAmount/100, nInPlayAmount%100);
	pInPlayBalance->SetLabel(sBalance, bRedraw);
	}

void
PokerLobbyWnd::SetUserRank(int nRank, bool bRedraw){
	/*
	ESChildControlLobbyUserInfo* pUserInfo = (ESChildControlLobbyUserInfo*)GetChildControl(CHILD_LobbyUserInfo);
	if( !pUserInfo ) return;
	pUserInfo->SetRankIndex(nRank, bRedraw);*/
	}

void
PokerLobbyWnd::SetUserAvatarImage(CImage* pImageAvatar, bool bRedraw){
	ESChildAvatar* pAvatar = (ESChildAvatar*)GetChildControlByName(_T("_imageAvatar"));
	ASSERT(pAvatar);
	if( !pImageAvatar || pImageAvatar->IsNull() )
		pAvatar->ClearAvatarImage(bRedraw);
	else
		pAvatar->SetAvatarImage(pImageAvatar, bRedraw);
	}

void
PokerLobbyWnd::SetAvatars(AutoSortedArray* pArrAvatarInfoById){
	/*
	ESChildControlLobbyUserInfo* pUserInfo = (ESChildControlLobbyUserInfo*)GetChildControl(CHILD_LobbyUserInfo);
	if( !pUserInfo ) return;
	pUserInfo->SetAvatars(pArrAvatarInfoById);*/
	}

void
PokerLobbyWnd::SetLobbyInfo(int nTablesCt, int nTournamentsCt, int nPlayersCt, bool bRedraw){
	ESChildControlLabel* pLobbyInfoLabel	= (ESChildControlLabel*)GetChildControlByName(_T("_lobbyInfoLabel"));
	if( !pLobbyInfoLabel ) return;

	CString sLabel;
	sLabel.Format(_T("{{Player}} : %d {{Table}} : %d {{Tournament}} : %d"), nPlayersCt, nTablesCt, nTournamentsCt);
	pLobbyInfoLabel->SetLabel(sLabel, bRedraw);
	}

void
PokerLobbyWnd::SetConnectedMode	(bool bConnected /*= true*/,  bool bRedraw /*= true*/){
	m_bUseForegroundColor	= !bConnected;
	m_bConnectedMode		= bConnected;
	if( bRedraw && IsWindow() )
		Invalidate();
	}

ESChildControlList*
PokerLobbyWnd::GetListOfCashTables(){
	return (ESChildControlList*)GetChildControlByName(_T("_cashTablesList"));
	}

ESChildControlList*
PokerLobbyWnd::GetListOfCashTableInfo(){
	return (ESChildControlList*)GetChildControlByName(_T("_cashTableInfoList"));
	}

ESChildControlList*
PokerLobbyWnd::GetListOfTournaments(){
	return (ESChildControlList*)GetChildControlByName(_T("_tournamentsList"));
	}

bool
PokerLobbyWnd::GetHideEmptyTables(){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(_T("_checkHideEmptyTables"));
	if( !pCheck ) return false;
	return pCheck->GetCheck();
	}

bool
PokerLobbyWnd::GetHideFullTables(){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(_T("_checkHideFullTables"));
	if( !pCheck ) return false;
	return pCheck->GetCheck();
	}

bool
PokerLobbyWnd::GetUpcommingTournaments(){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(_T("_upComingTournamentsCheck"));
	if( !pCheck ) return false;
	return pCheck->GetCheck();
	}

void
PokerLobbyWnd::SetHideEmptyTableCt(int nCt){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(_T("_checkHideEmptyTables"));
	if( !pCheck ) return;
	CString sText;
	if( nCt < 0 )
		sText = _T("{{Hide Empty Tables}}");
	else
		sText.Format(_T("{{Hide Empty Tables}} (%d)"), nCt);
	pCheck->SetLabel(sText, true);
	}

void
PokerLobbyWnd::SetHideFullTableCt(int nCt){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(_T("_checkHideFullTables"));
	if( !pCheck ) return;
	CString sText;
	if( nCt < 0 )
		sText = _T("{{Hide Full tables}}");
	else
		sText.Format(_T("{{Hide Full tables}} (%d)"), nCt);
	pCheck->SetLabel(sText, true);
	}

void
PokerLobbyWnd::SetFinishedTournamentsCt(int nCt){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(_T("_upComingTournamentsCheck"));
	if( !pCheck ) return;
	CString sText;
	if( nCt < 0 )
		sText = _T("{{Show Upcoming Tournaments}}");
	else
		sText.Format(_T("{{Show Upcoming Tournaments}} (%d)"), nCt);
	pCheck->SetLabel(sText, true);
	}

int
PokerLobbyWnd::GetCurrentGameType(){
	ESChildControlTabCtrl*	pTab		= (ESChildControlTabCtrl*)GetChildControlByName(_T("_cashGamesTab"));
	if( !pTab ) return 0;
	int nGameType = (int)pTab->GetItemData(pTab->GetActiveItem());

	ESChildControlTabCtrl*	pTabSubTypes= (ESChildControlTabCtrl*)GetChildControlByName(_T("_cashLimitsTab"));
	if( pTabSubTypes ) {
		int nCurrSubType = (int)pTabSubTypes->GetItemData(pTabSubTypes->GetActiveItem());
		// Playmoney.
		if( nCurrSubType == -1 )
			nGameType |= GameType::Playmoney;
		}
	return nGameType;
	}

int
PokerLobbyWnd::GetCurrentSubType(){
	ESChildControlTabCtrl*	pTab	= (ESChildControlTabCtrl*)GetChildControlByName(_T("_cashLimitsTab"));
	if( !pTab ) return 0;
	int nSubType = (int)pTab->GetItemData(pTab->GetActiveItem());

	// Playmoney.
	if( nSubType == -1 ){
		ESChildControlTabCtrl*	pTabGameTypes = (ESChildControlTabCtrl*)GetChildControlByName(_T("_cashGamesTab"));
		if( pTabGameTypes ){
			int nGameType = (int)pTabGameTypes->GetItemData(pTabGameTypes->GetActiveItem());
			nSubType = PokerHelper::GetSubTypesByGameType(nGameType);
			}
		else
			nSubType = 0;
		}
	return nSubType;
	}

void
PokerLobbyWnd::SetLoginMode(bool bOn /*= true*/){
	m_bLoginMode = bOn;

	if( !bOn ){
		// Hide cashier button.
		SetControlVisibilityByName(_T("_cashierButton"), false, true);
		// Hide logout button.
		SetControlVisibilityByName(_T("_logoutButton"), false, true);
		// Hide seat at button.
		SetControlVisibilityByName(_T("_seatOrWaitListButton"), false, true);
		
		/*
		// Hide avatar image.
		SetControlVisibilityByName(_T("_imageAvatar"), false, true);
		// Hide user name.
		SetControlVisibilityByName(_T("_userNameLabel"), false, true);
		// Hide user balance.
		SetControlVisibilityByName(_T("_userBalanceLabel"), false, true);
		// Hide user inplay balance.
		SetControlVisibilityByName(_T("_inPlayBalanceLabel"), false, true);
		*/
		// Hide avatar image.
		SetControlVisibilityByName(_T("_imageAvatar"), false, true);
		// Hide container.
		SetControlVisibilityByName(_T("_image2"), false, true);
		
		// Show login button.
		SetControlVisibilityByName(_T("_loginButton"), true, true);
		}
	else{
		// Hide login button.
		SetControlVisibilityByName(_T("_loginButton"), false, true);

		// Show cashier button.
		SetControlVisibilityByName(_T("_cashierButton"), true, true);
		// Show logout button.
		SetControlVisibilityByName(_T("_logoutButton"), true, true);
		// Show avatar image.
		SetControlVisibilityByName(_T("_imageAvatar"), true, true);
		// Show seat at button.
		SetControlVisibilityByName(_T("_seatOrWaitListButton"), true, true);

		/*
		// Show user name.
		SetControlVisibilityByName(_T("_userNameLabel"), true, true);
		// Show user balance.
		SetControlVisibilityByName(_T("_userBalanceLabel"), true, true);
		// Show user inplay balance.
		SetControlVisibilityByName(_T("_inPlayBalanceLabel"), true, true);
		*/
		// Show container.
		SetControlVisibilityByName(_T("_image2"), true, true);
		}
	}