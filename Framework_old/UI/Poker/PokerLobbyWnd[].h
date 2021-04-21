#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerLobbyWnd window

#include "..\..\Controls\ESChildControl.h"
#include "..\..\Controls\MercuryBaseWnd.h"
#include "..\..\Protocol\_PokerDefines.h"
#include "..\..\Array\AutoSortedArrayTempl.h"
#include "..\..\Array\AutoSortedArray.h"
#include "..\..\Array\StringToPtrArray.h"
#include "..\..\PokerServer\Data\Tables\PokerAvatar.h"
#include "..\..\GameClient\PokerClient.h"
#include "..\..\Controls\ESChildControlList.h"
#include <atlimage.h>

class ESChildControlImageButton;

class PokerLobbyWnd : public MercuryBaseView
{
// Construction
public:
	PokerLobbyWnd();
	virtual ~PokerLobbyWnd();

	friend class PokerLobbyDlg;
	
public: // Operations.
	void					SetUserName			(CString sUserName, bool bRedraw);
	void					SetUserBalance		(int nBalance, bool bRedraw);
	void					SetUserInPlayAmount	(int nInPlayAmount, bool bRedraw);
	void					SetUserRank			(int nRank, bool bRedraw);
	void					SetUserAvatarImage	(CImage* pImageAvatar, bool bRedraw);
	void					SetAvatars			(AutoSortedArray* pArrAvatarInfoById);
	void					SetLobbyInfo		(int nTablesCt, int nTournamentsCt, int nPlayersCt, bool bRedraw); // 	ESChildControlLabel* pLobbyInfoLabel	= (ESChildControlLabel*)GetChildControl(CHILD_LobbyInfoLabel);

	void					SetConnectedMode	(bool bConnected = true, bool bRedraw = true);
	void					SetLoginMode		(bool bOn = true);

	ESChildControlList*		GetListOfCashTables		();
	ESChildControlList*		GetListOfCashTableInfo	();
	ESChildControlList*		GetListOfTournaments	();

	void					UpdateUI				();
	bool					GetHideEmptyTables		();
	bool					GetHideFullTables		();
	bool					GetUpcommingTournaments	();
	void					SetHideEmptyTableCt		(int nCt);
	void					SetHideFullTableCt		(int nCt);
	void					SetFinishedTournamentsCt(int nCt);

	// Retrieves current game type. (Game type which is switched by tab)
	int						GetCurrentGameType	();
	// Retrieves current game sub type. (Game sub type which is switched by tab)
	int						GetCurrentSubType	();

	void					AllowOpenTable		(bool bAllow){m_bOpenTableAllowed = bAllow;};
	int						GetSelectedTableId	(int nDefault);
	int						GetSelectedListItem	(int nListControlId);

protected:
	virtual bool			AttachToFrame		(ESFrameBase* pBaseFrameWnd);
	virtual BOOL			UseDoubleBufferedDC	(){return TRUE;};

protected:
	PokerClient*		m_pClient;

	bool				m_bOpenTableAllowed;	// Permition to open table.
	bool				m_bLoginMode;			// Login mode.
	bool				m_bConnectedMode;		// Connected mode.

	// Child control events.
protected:
};