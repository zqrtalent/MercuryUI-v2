#pragma once


/////////////////////////////////////////////////////////////////////////////
// PokerBuyChipsDlg window

#include "ChildControls\ESDialog.h"
#include "..\..\Controls\ESChildControlLabel.h"
#include "ChildControls\ESChildControlTextEdit.h"
#include "ChildControls\ESChildControlMoneyEdit.h"
#include "..\..\Controls\ESChildControlImageButton.h"
#include "..\..\..\ESPokerClient\ESPokerClient\ESPokerClient.h"

class PokerBuyChipsDlg : public ESDialog
{
// Construction
public:
	PokerBuyChipsDlg();
	virtual ~PokerBuyChipsDlg();

	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	virtual void	OnInitDialog			();

	virtual BOOL	OnOK					();
	virtual BOOL	OnCancel				();

	void			SetMinAmount			(int nAmountMin);
	void			SetMaxAmount			(int nAmountMax);
	void			SetBalanceAmount		(int nAmountBalance);

protected:
	ESChildControlLabel*		m_pMinLabel;
	ESChildControlLabel*		m_pMaxLabel;
	ESChildControlLabel*		m_pBalanceLabel;

	ESChildControlMoneyEdit*	m_pAmountAddEdit;	// IDC_BUTTON_ADD_CHIPS
	ESChildControlImageButton*	m_pAddChipsButton;
/*
	ESChildControlMoneyEdit*	m_pAmountAddEdit;
	ESChildControlTextEdit*		m_pUserPasswordEdit;
	ESChildControlImageButton*	m_pLoginButton;
	ESChildControlImageButton*	m_pCancelButton;*/

	int							m_nMinAmount;
	int							m_nMaxAmount;
	int							m_nBalanceAmount;
	int							m_nStepAmount;
	int							m_nAmountAdd;

	// Child control events
protected:
	void	OnAddChips			(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void	OnEditAmountChange	(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam = 0);
};