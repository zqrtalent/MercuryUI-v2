// PokerLobbyDlg.h : header file
//

#pragma once
#include "Controls/MercuryBaseWnd.h"
#include "Controls/ESChildComboBox.h"

#if !defined(MAKEINT64)
#define MAKEINT64(low,high) \
        ((((__int64)low)&0xFFFFFFFF) | (((__int64)high)<<32))
#endif

class ESChildControlTabCtrl;
class ESChildControlList;
class ESChildControlImage;
class ESChildControlCheckBox;
class UserSettingsMan;

// PokerLobbyDlg dialog
class PokerLobbyDlg : public MercuryBaseView
{
// Construction
public:
	PokerLobbyDlg();	// standard constructor

protected:
	//virtual void		DoDataExchange				(CDataExchange* pDX);	// DDX/DDV support
	virtual void		SetTitle					();
	virtual void		Setup						();
	//virtual void		OnOK						();
	//virtual void		OnCancel					();

// Implementation
protected:
	MercuryBaseView*			m_pLobbyWnd;
protected:
	// Generated message map functions
	virtual	void	OnClose							();
	virtual void	OnDestroy						();
	//virtual void	OnTimer							(UINT_PTR nIDEvent);
};
