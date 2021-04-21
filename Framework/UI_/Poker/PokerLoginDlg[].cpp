//// PokerLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PokerGUI.h"
#include "..\..\Controls\ESChildControlImageButton.h"
#include "PokerLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerLoginDlg

PokerLoginDlg::PokerLoginDlg(){
	}

PokerLoginDlg::~PokerLoginDlg(){
	}

bool
PokerLoginDlg::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESDialog::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		}
	return bRet;
	}

void
PokerLoginDlg::OnLoading(){
	ESDialog::OnLoading();

	PokerGUI* pGUI = PokerGUI::GetInstance();

	// User name edit.
	CRect rcUserName(10, 10, 300, 36);
	m_pUserName = new ESChildTextBox();
	m_pUserName->SetFont					(PokerGUI::GetInstance()->m_pDialogFont);
	m_pUserName->SetAlternateTextColor		(RGB(153, 153, 153));
	m_pUserName->SetAlternateText			(_T("User"));
	m_pUserName->SetAlternateTextFont		(PokerGUI::GetInstance()->m_pDialogFont);
	m_pUserName->SetBgImage					(pGUI->m_pImageTextBoxBg, CRect(10, 10, 10, 10));

	AddChild(m_pUserName, rcUserName, false);

	// User password edit.
	CRect rcUserPassword(10, 50, 300, 76);
	m_pUserPassword = new ESChildTextBox();
	m_pUserPassword->SetFont				(PokerGUI::GetInstance()->m_pDialogFont);
	m_pUserPassword->SetPassword			(true);

	m_pUserPassword->SetAlternateTextColor	(RGB(153, 153, 153));
	m_pUserPassword->SetAlternateText		(_T("User"));
	m_pUserPassword->SetAlternateTextFont	(PokerGUI::GetInstance()->m_pDialogFont);
	m_pUserPassword->SetBgImage				(pGUI->m_pImageTextBoxBg, CRect(10, 10, 10, 10));
	AddChild(m_pUserPassword, rcUserPassword, false);

	// Login button.
	CRect rcLoginButton(10, 100, 150, 130);
	m_pLoginButton = new ESChildControlImageButton();
	m_pLoginButton->SetDefaultImage			(pGUI->m_pPushButtonNormal);
	m_pLoginButton->SetHoverImage			(pGUI->m_pPushButtonHover);
	m_pLoginButton->SetSelectedImage		(pGUI->m_pPushButtonPushed);
	m_pLoginButton->SetDisabledImage		(pGUI->m_pPushButtonDisabled);
	m_pLoginButton->SetSizeAutoCalculation	(false);
	m_pLoginButton->SetOmitBorder			(pGUI->m_rcPushButtonOmitBorder);
	m_pLoginButton->SetFont					(m_pFont);
	m_pLoginButton->SetTextColor			(pGUI->m_crPushButtonText);
	m_pLoginButton->SetText					(PokerGUI::GetLangValue(_T("LOGIN_DIALOG_LOGIN_BUTTON")));
	AddChild(m_pLoginButton, rcLoginButton, false);

	// Cancel button.
	CRect rcCancelButton(160, 100, 300, 130);
	m_pCancelButton = new ESChildControlImageButton();
	m_pCancelButton->SetDefaultImage		(pGUI->m_pPushButtonNormal);
	m_pCancelButton->SetHoverImage			(pGUI->m_pPushButtonHover);
	m_pCancelButton->SetSelectedImage		(pGUI->m_pPushButtonPushed);
	m_pCancelButton->SetDisabledImage		(pGUI->m_pPushButtonDisabled);
	m_pCancelButton->SetSizeAutoCalculation	(false);
	m_pCancelButton->SetOmitBorder			(pGUI->m_rcPushButtonOmitBorder);
	m_pCancelButton->SetFont				(m_pFont);
	m_pCancelButton->SetTextColor			(pGUI->m_crPushButtonText);
	m_pCancelButton->SetText				(PokerGUI::GetLangValue(_T("LOGIN_DIALOG_CANCEL_BUTTON")));
	AddChild(m_pCancelButton, rcCancelButton, false);

	m_pLoginButton->AddEventListener(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&PokerLoginDlg::OnLogin);	
	m_pCancelButton->AddEventListener(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&PokerLoginDlg::OnCancelButton);	

	CString sUserName = theApp.GetProfileString(_T("LOGIN"), _T("UserName"), _T(""));
	if( !sUserName.IsEmpty() ){
		m_pUserName->SetText(sUserName, true);
		SetFocus(m_pUserPassword);
		}
	else
		SetFocus(m_pUserPassword);

	m_sTitle = PokerGUI::GetLangValue(_T("LOGIN_DIALOG_TITLE"));
	}

bool
PokerLoginDlg::SetPos(int x, int y, bool bRedraw){
	ESDialog::SetPos(x, y, bRedraw);
	return true;
	}

bool
PokerLoginDlg::SetSize(int cx, int cy, bool bRedraw){
	ESDialog::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
PokerLoginDlg::Destroy(){
	ESDialog::Destroy();
	return true;
	}

void
PokerLoginDlg::OwnerWindowSizeChanged(int cx, int cy){
	ESDialog::OwnerWindowSizeChanged(cx, cy);
	}

BOOL
PokerLoginDlg::OnOK(){
	CString sUserName = m_pUserName->GetText();
	CString sPassword = m_pUserPassword->GetText();

	if( sUserName.IsEmpty() ){
		//m_pUserNameEdit->SetFocus();
		return FALSE;
		}

	if( sPassword.IsEmpty() ){
		//m_pUserPasswordEdit->SetFocus();
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
		/*		switch( nErrorCode ){
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
					}*/
				}
			}
		}
	// }}

	theApp.EndWaitCursor();
	m_nRetCode = IDOK;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, false);
	return FALSE;
	}

BOOL
PokerLoginDlg::OnCancel(){
	m_nRetCode = IDCANCEL;
	SetVisible(false, true);
	m_pOwner->RemoveChildControl(this, false);
	return FALSE;
	}

void
PokerLoginDlg::OnLogin(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	OnOK();
	}

void
PokerLoginDlg::OnCancelButton(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	OnCancel();
	}


