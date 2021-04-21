#pragma once


/////////////////////////////////////////////////////////////////////////////
// PokerLoginDlg window

#include "ChildControls\ESDialog.h"
#include "..\..\Controls\ESChildControlLabel.h"
#include "..\..\Controls\ESChildTextBox.h"
#include "..\..\..\ESPokerClient\ESPokerClient\ESPokerClient.h"

class PokerLoginDlg : public ESDialog
{
// Construction
public:
	PokerLoginDlg();
	virtual ~PokerLoginDlg();

	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	virtual void	OnLoading				();
	virtual BOOL	OnOK					();
	virtual BOOL	OnCancel				();

protected:

	ESChildTextBox*				m_pUserName;
	ESChildTextBox*				m_pUserPassword;
	ESChildControlImageButton*	m_pLoginButton;
	ESChildControlImageButton*	m_pCancelButton;

	/*
	ESChildControlLabel*		m_pUserNameLabel;
	ESChildControlLabel*		m_pUserPasswordLabel;
	ESChildControlLabel*		m_pErrorStringLabel;

	ESChildControlTextEdit*		m_pUserNameEdit;
	ESChildControlTextEdit*		m_pUserPasswordEdit;
	ESChildControlImageButton*	m_pLoginButton;
	ESChildControlImageButton*	m_pCancelButton;*/

	// Child control events
protected:
	void	OnLogin			(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void	OnCancelButton	(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam = 0);
};