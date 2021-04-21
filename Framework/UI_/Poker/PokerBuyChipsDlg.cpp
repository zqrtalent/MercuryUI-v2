//// PokerBuyChipsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PokerGUI.h"
#include "..\..\Controls\ESChildControlImageButton.h"
#include "PokerBuyChipsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerBuyChipsDlg

PokerBuyChipsDlg::PokerBuyChipsDlg(){
	m_nMinAmount		= 1;
	m_nMaxAmount		= 2000;
	m_nBalanceAmount	= 4300;
	m_nStepAmount		= 10;
	m_nAmountAdd		= 34;
	}

PokerBuyChipsDlg::~PokerBuyChipsDlg(){
	}

bool
PokerBuyChipsDlg::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESDialog::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		}
	return bRet;
	}

void
PokerBuyChipsDlg::OnInitDialog(){
	ESDialog::OnInitDialog();

	m_pMinLabel			= (ESChildControlLabel*)m_pOwner->GetChildControl(IDC_MIN_LABEL);
	m_pMaxLabel			= (ESChildControlLabel*)m_pOwner->GetChildControl(IDC_MAX_LABEL);
	m_pBalanceLabel		= (ESChildControlLabel*)m_pOwner->GetChildControl(IDC_BALANCE_LABEL);
	m_pAmountAddEdit	= (ESChildControlMoneyEdit*)m_pOwner->GetChildControl(IDC_EDIT_ADD_AMOUNT);
	m_pAddChipsButton	= (ESChildControlImageButton*)m_pOwner->GetChildControl(IDC_BUTTON_ADD_CHIPS);

	m_pAddChipsButton	->AddEventListener(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&PokerBuyChipsDlg::OnAddChips);	
	m_pAmountAddEdit	->AddEventListener(this, ChildControlEventCode::EventCode_EditTextChange, (ControlEventListener)&PokerBuyChipsDlg::OnEditAmountChange);

	SetFocus(m_pAmountAddEdit);
	m_pAmountAddEdit->Setup(m_nMinAmount, m_nMaxAmount, m_nAmountAdd);

/*
	m_sTitle				= PokerGUI::GetLangValue(_T("LOGIN_DIALOG_TITLE"));
	m_pLoginButton		->SetText(PokerGUI::GetLangValue(_T("LOGIN_DIALOG_LOGIN_BUTTON")));
	m_pCancelButton		->SetText(PokerGUI::GetLangValue(_T("LOGIN_DIALOG_CANCEL_BUTTON")));
	m_pUserNameLabel	->SetLabel(PokerGUI::GetLangValue(_T("LOGIN_DIALOG_USERNAME_LABEL")), false);
	m_pUserPasswordLabel->SetLabel(PokerGUI::GetLangValue(_T("LOGIN_DIALOG_PASSWORD_LABEL")), false);
	m_pErrorStringLabel	->SetLabel(_T(""), false);
	m_pErrorStringLabel->SetTextColor(RGB(200, 0, 0)); // Red color.

	CString sUserName = theApp.GetProfileString(_T("LOGIN"), _T("UserName"), _T(""));
	if( !sUserName.IsEmpty() ){
		m_pUserNameEdit->SetText		(sUserName);
		SetFocus(m_pUserPasswordEdit);
		}
	else
		SetFocus(m_pUserNameEdit);*/
	}

bool
PokerBuyChipsDlg::SetPos(int x, int y, bool bRedraw){
	ESDialog::SetPos(x, y, bRedraw);
	return true;
	}

bool
PokerBuyChipsDlg::SetSize(int cx, int cy, bool bRedraw){
	ESDialog::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
PokerBuyChipsDlg::Destroy(){
	ESDialog::Destroy();
	return true;
	}

void
PokerBuyChipsDlg::OwnerWindowSizeChanged(int cx, int cy){
	ESDialog::OwnerWindowSizeChanged(cx, cy);
	}

BOOL
PokerBuyChipsDlg::OnOK(){
	/*
	CString sUserName = m_pUserNameEdit->GetText();
	CString sPassword = m_pUserPasswordEdit->GetText();

	if( sUserName.IsEmpty() ){
		m_pUserNameEdit->SetFocus();
		return FALSE;
		}

	if( sPassword.IsEmpty() ){
		m_pUserPasswordEdit->SetFocus();
		return FALSE;
		}

	// Make server call. {{
	if( theApp.m_client.IsConnected() ){
		LoginPacket* p1 = NULL;
		theApp.m_client.Login(sUserName, sPassword);

		theApp.BeginWaitCursor();
		if( theApp.m_client.WaitForResponse(PacketCodes::_Login, (BasePacket**)&p1, 10) ){
			theApp.m_client.CallPacketHandler(p1, (void*)&theApp.m_client);

			int nErrorCode = p1->m_nErrorCode;
			delete p1;
			if( nErrorCode == ES_ERROR_SUCCESS ){
				theApp.WriteProfileString(_T("LOGIN"), _T("UserName"), sUserName);
				}
			else{
				theApp.EndWaitCursor();
				switch( nErrorCode ){
					case ES_ERROR_USER_NOTFOUND_OR_WRONG_PASSWORD:{
						m_pErrorStringLabel->SetLabel(_T("User not found or wrong password !!!"), true);
						return FALSE;
						}
					case ES_ERROR_USER_ALREADY_LOGGED_IN:{
						m_pErrorStringLabel->SetLabel(_T("User already logged in !!!"), true);
						return FALSE;
						}
					case ES_ERROR_USER_BLOCKED:{
						m_pErrorStringLabel->SetLabel(_T("User is blocked !!!"), true);
						return FALSE;
						}
					}
				}
			}
		}
	// }}

	theApp.EndWaitCursor();*/
	m_nRetCode = IDOK;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, false);
	return FALSE;
	}

BOOL
PokerBuyChipsDlg::OnCancel(){
	m_nRetCode = IDCANCEL;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, false);
	return FALSE;
	}

void
PokerBuyChipsDlg::OnAddChips(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	OnOK();
	}

void
PokerBuyChipsDlg::OnEditAmountChange(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	//int nAmount = 
	}

void
PokerBuyChipsDlg::SetMinAmount(int nAmountMin){
	}

void
PokerBuyChipsDlg::SetMaxAmount(int nAmountMax){
	}

void
PokerBuyChipsDlg::SetBalanceAmount(int nAmountBalance){
	}


