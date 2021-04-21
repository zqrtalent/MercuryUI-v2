#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerTableWnd window

#include "..\..\Controls\ESEasyMotion.h"
#include "..\..\Controls\MercuryBaseWnd.h"
#include "..\..\Protocol\_PokerDefines.h"
#include "..\..\Array\AutoSortedArrayTempl.h"
#include "..\..\Array\AutoSortedArray.h"
#include "..\..\Array\StringToPtrArray.h"
#include "..\..\PokerServer\Object\PokerTableSeatInfo.h"
#include "..\..\PokerServer\Object\PokerChatMessage.h"
#include "..\..\PokerServer\Data\Tables\PokerAvatar.h"
#include "..\..\Thread\WinThread.h"
#include "PokerGUI.h"
#include "PokerTableWndEventHandler.h"
#include "PokerTableWnd_AnimationInfo.h"
#include <atlimage.h>

class ChildControl;
class ESChildControlSeat;				
class ESChildControlSeatCards;		
class ESChildControlSeatProgressBar;	
class ESChildControlCards;		
class ESChildControlDealer;			
class ESChildControlTableCards;
class ESChildControlChips;
class SeatPanelInfo;
class PokerGUI;
class PokerHandCards;

class PokerTableWnd : public MercuryBaseView
{
// Construction
public:
	PokerTableWnd();
	virtual ~PokerTableWnd();

	DECLARE_DYNCREATE(PokerTableWnd)

	enum DesignerContextMenuItemIds
	{
	   MI_TABLE = 67100,
	   MI_SEAT_LABEL_AS_CHILD,
	   MI_SEAT_AS_CHILD,
	   MI_CHIPS_AS_CHILD,
	   MI_HIDDEN_CARDS_AS_CHILD,
	   MI_SEAT_CARDS_AS_CHILD,
	   MI_TABLE_CARDS_AS_CHILD
	};
	
public: // Operations.
	void				SetConnectedMode		(bool bConnected = true, bool bRedraw = true);
	bool				Setup					(int nMaxPlayerCt,
												EnumerableObject<PokerTableSeatInfo>* seats, 
												EnumerableObject<PokerAvatar>* seatAvatars,
												PokerRoundType round, 
												CDWordArray *pListTableCards,
												CString	sAmountLabelSufix = _T("GEL"));

	bool				SetTableStatistic		(PokerOpenTablePacket* p);
	bool				SetTableInfo			(PokerTableInfo* pInfo);

	bool				SetupSeatTimer			(int nSeat, int nFullSeconds, int nSecondsPassed, bool bInvalidate = false, bool bStart = false);
	bool				StartActionTimer		(int nSeat, bool bStart = true, bool bInvalidate = false);

	bool				SetTablePotChips		(int nAmount);
	bool				SetSeatInfo				(int nSeatIndex, PokerTableSeatInfo* pInfo, bool bRedraw = true);
	PokerTableSeatInfo*	GetSeatInfo				(int nSeatIndex){return m_seats.GetAt(nSeatIndex);};

	int					GetMaxPotAmount			();
	bool				SetSeatAvatarInfo		(int nSeat, PokerAvatar* pAvatarInfo, bool bRedraw = true);

	bool				SetActiveSeat			(int nSeatIndex, bool bActivateTimer = false, bool bInvalidate = true); //+	
	int					GetActiveSeat			(){return m_nActiveSeat;};
	int					GetDealerSeat			(){return m_nDealerSeat;};
	bool				SetSeatStatus			(int nSeat, PokerSeatStatus status, bool bInvalidate = true);
	PokerSeatStatus		GetSeatStatus			(int nSeat);
	void				SetSeatOffline			(int nSeat, bool bOnline = false, bool bInvalidate = true);
	bool				GetSeatOffline			(int nSeat);
	CString				GetSeatUserName			(int nSeat);
	int					GetMaxSeatCount			(){return m_nMaxPlayerCt;};
	void				SetRound				(PokerRoundType round, CDWordArray *pListTableCards, bool bRedrawCards = false);
	void				SetGameStatus			(PokerGameStatus gameStatus){m_gameStatus = gameStatus;};
	void				SetDealerSeat			(int nSeatIndex, bool bInvalidate);
	void				SetTotalPotAmount		(int nTotalPotAmount, bool bRedraw);
	int					GetTotalPotAmount		(){ return m_nTotalPotAmount; };
	void				SetMainPotAmount		(int nMainPotAmount, bool bRedraw);
	int					GetMainPotAmount		(){return m_nMainPotAmount;};
	int					GetPotAmount			(int nSeat);
	bool				SetPotAmount			(int nSeat, int nAmount, bool bRedraw = true);
	int					GetSeatTotalPot			(int nSeat);
	bool				SetSeatTotalPot			(int nSeat, int nAmount, bool bRedraw = true);
	int					GetBalance				(int nSeat);
	bool				SetBalance				(int nSeat, int nBalance, bool bRedraw = true);
	bool				IsSeatTimerActive		(int nSeat);

	ActionButtonInfo*	GetActionButtonInfoByIdx(int nIndex){ return m_listActionButtons.GetAt(nIndex); }
	bool				GetActionButtonByPoint	(CPoint pt, ActionButtonInfo** ppInfo, CRect& rcActionButton);
	bool				GetSeatPanelRect		(int nSeat, CRect& rcSeat);		
	void				ClearPotAmounts			(bool bRedraw = true);

	// Animations.
	bool				CollectChips			(int nAnimationSec, int nMainChipsAmount);
	bool				MoveChipsFromPot		(int nAnimationSec, CDWordArray& arrSeatChipAmounts);
	bool				DealSeatCardsAnimation	(PokerHandStartedPacket* p);
	bool				DealTableCardsAnimation	(PokerRoundStartedPacket* p);

	// Flat buttons.
	void				AddFlatButton			(int nIndex, int nId, CString sTitle, bool bRedraw);
	// Shortcut buttons.
	bool				AddShortcutButton		(CString sLabel, int nAmount, bool bInvalidate = true); 
	bool				ClearShortcutButtons	(bool bInvalidate = true); 
	bool				RedrawShortcutButtons	();

	// Add chips control.
	bool				ShowAddChipsControl		(int nAmount, int nAmountMin, int nAmountMax, int nAmountStep, int nBalance, bool bInvalidate = true);
	bool				HideAddChipsControl		(bool bInvalidate = true);
	bool				IsAddChipsControlVisible();
	int					GetAddChipsAmount		();

	// Slider.
	bool				ShowSlider				(int nAmount, int nAmountMin, int nAmountMax, int nAmountStep, bool bInvalidate = true);
	bool				SetSliderAmount			(int nAmount, bool bInvalidate = true);
	bool				HideSlider				(bool bInvalidate = true);
	bool				IsActionSliderVisible	();

	// Action buttons.
	bool				AddActionButton			(ActionButton type, CString sLabel, int nAmountMin, int nAmountMax, bool bInvalidate = true);
	bool				ClearActionButtons		(bool bInvalidate = true);
	bool				RedrawActionButtons		();
	bool				SetActionButtonInfo		(int nIndex, CString sLabel, int nAmount, bool bInvalidate = true);
	bool				RedrawActionButton		(int nIndex);
	bool				IsActionButtonVisible	(ActionButton type);
	ActionButtonInfo*	GetActionButtonInfo		(ActionButton type);

	// Pre-Action buttons.
	bool				AddPreActionButton		(PreActionButton preAction, int nValue, CString sLabel, bool bCheckState = false, bool bInvalidate = true, bool bCalcSize = false);
	bool				RemovePreActionButton	(PreActionButton preAction, bool bInvalidate = true);
	bool				SetPreActionButtonLabel	(PreActionButton preAction, CString sLabel, bool bInvalidate = true);
	bool				ClearPreActions			(bool bInvalidate = true);
	bool				SetPreActionCheckState	(PreActionButton preAction, bool bCheckState, bool bInvalidate = true);
	bool				RedrawPreActions		();
	bool				RedrawPreActionButton	(PreActionButton preAction);
	PreActionButtonInfo* GetPreActionInfo		(int nId);
	bool				GetPreActionCheckState	(PreActionButton preAction);

	// Seat action label.
	bool				ClearActionLabels		(bool bRedraw);
	bool				SetSeatActionLabel		(int nSeat, CString sActionName, bool bInvalidate = true);
	bool				RedrawSeatActionLabel	(int nSeat);

	void				ClearSeatCards			(bool bRedraw);
	void				ClearTableCards			(bool bRedraw);

	// Set seat cards.
	bool				SetSeatCards			(int nSeat, int nCard1, int nCard2, int nCard3, int nCard4, bool bInvalidate = true);

	// Set table cards.
	void				SetTableCards			(int nCard1, int nCard2, int nCard3, int nCard4, int nCard5, bool bInvalidate = true);

	int					GetTableCard1			(){return m_nCard1;};
	int					GetTableCard2			(){return m_nCard2;};
	int					GetTableCard3			(){return m_nCard3;};
	int					GetTableCard4			(){return m_nCard4;};
	int					GetTableCard5			(){return m_nCard5;};

	// Chat
	void				ClearChatMessages			(bool bRedraw);
	void				AddChatMessage				(PokerChatMessage* pMessage);
	int					AddChatMessages				(EnumerableObject<PokerChatMessage>* pListMessage);
	void				AddChatMessage_HandStart	(PokerHandStartedPacket* p);
	void				AddChatMessage_HandFinish	(PokerHandFinishedPacket* p);
	void				AddChatMessage_RoundStart	(PokerRoundStartedPacket* p);
	void				AddChatMessage_SetSeatStatus(PokerSetSeatStatusPacket* p);
	void				AddChatMessage_ShowCards	(PokerActionPacket* p);

	// Table message label.
	void				SetTableMessage			(CString sMessage, bool bInvalidate);
	void				ClearTableMessage		(bool bInvalidate);

	// Seat message label.
	void				SetSeatMessage			(int nSeat, CString sMessage, bool bInvalidate);
	void				ClearSeatMessage		(int nSeat, bool bInvalidate);
	void				ClearSeatMessages		(bool bInvalidate);

	// Set seat hand cards.
	bool				SetBestHandCards		(int nSeat, PokerHandCards* pCards, 
												bool bSetAsSeatMessage = true, CString* psLabelText = NULL, CDWordArray* pArrWinnerCards = NULL);
	// Show winner combination with table cards and table label.
	bool				SetWinnerTableCards		(PokerHandCardsClass handCards, CDWordArray* pArrWinnerCards);

	// Player's best hand label.
	void				SetBestHandLabel		(CString sText, bool bInvalidate = true);
	void				ClearBestHandLabel		(bool bInvalidate = true);
	// Set window event handler.
	void				SetWndEventHandler		(PokerTableWndEventHandler* pHandler) {m_pHandler = pHandler;};
	// Stop animation.
	void				StopAnimationThread		();

protected:
	bool				DrawFloor				(CDC* pDC, CRect* pRectDC, CRect* pClipRect);

protected:
	// Animation methods.
	static UINT	_stdcall	AnimationProc					(WinThread* pThread);

	bool				AnimateCollectChips					(WinThread* pThread, CollectChipsAnimationInfo* pAnimationInfo);
	bool				AnimateMoveChipsFromPot				(WinThread* pThread, ChipsMovementAnimationInfo* pAnimationInfo);
	bool				AnimateStartHand					(WinThread* pThread, DealSeatCardsAnimationInfo* pAnimationInfo);
	bool				AnimateDealTableCards				(WinThread* pThread, DealTableCardsAnimationInfo* pAnimationInfo);
			


protected:
	int									GetSeatIndex			(PokerTableSeatInfo* pSeat){return m_seats.IndexOf(pSeat);};
	SeatPanelInfo*						GetSeatPanelInfo		(int nSeat);
	bool								CreateChildControls		();

	ESChildControlSeat*					GetSeatControl			(int nSeat);
	ESChildControlSeatCards*			GetSeatCardsControl		(int nSeat);
	ESChildControlSeatProgressBar*		GetSeatProgressControl	(int nSeat);
	ESChildControlChips*				GetChipsControl			(int nSeat);
	ESChildControlChips*				GetMainChipsControl		();
	ESChildControlDealer*				GetSeatDealerControl	(int nSeat);
	ESChildControlTableCards*			GetTableCardsControl	();
	bool								SetChildControlVisible	(int nId, bool bVisible, bool bRedraw);
	bool								SetSeatChips			(int nSeat, int nAmount);

	CRect								GetSeatCardsRect		(int nSeat);
	CRect								GetSeatRect				(int nSeat);

	void								HideActionButtons		(bool bShow = false, bool bRedraw = true);
	void								HidePreActionButtons	(bool bShow = false, bool bRedraw = true);
	void								HideShortcutButtons		(bool bShow = false, bool bRedraw = true);

public:
	bool								RedrawSeat				(int nSeat, bool bWithCards = false);
	void								RedrawSeats				(bool bWithCards = false);
	bool								RedrawSeatCards			(int nSeat);
	bool								RedrawSeatChips			(int nSeat);
	bool								RedrawChildControl		(int nId);

protected:
	float									m_fZoomFactor;
	PokerGUI*								m_pGUI;
	int										m_nMaxPlayerCt;
	PokerGameStatus							m_gameStatus;
	PokerRoundType							m_currentRound;
	AutoSortedArray							m_acceptebleMaxPlayersCount;
	EnumerableObject<PokerTableSeatInfo>	m_seats;
	EnumerableObject<SeatPanelInfo>*		m_seatPanelInfos;
	CString									m_sAmountLabelSuffix;			// Amount label suffix. (GEL, USD, PT or etc)
	int										m_nRegularThinkTimeSec;			// Regular think time seconds.

	bool									m_bConnectedMode;				// Connected mode.

	PokerTableSeatInfo*						m_pTimerActiveSeat;				// Active seat whos timer is ser already.

	int										m_nTotalPotAmount;				// Total pot amount.
	int										m_nMainPotAmount;				// Main pot amount.
	int										m_nActiveSeat;					// Active seat index.
	int										m_nDealerSeat;					// Dealer seat index.

	int										m_nCard1;						// Table card1.
	int										m_nCard2;						// Table card2.
	int										m_nCard3;						// Table card3.
	int										m_nCard4;						// Table card4.
	int										m_nCard5;						// Table card5.

	List<ActionButtonInfo>					m_listActionButtons;			// Action button info list.
	List<ShortcutButtonInfo>				m_listShortcutButtons;			// Shortcut button info list.
	List<PreActionButtonInfo>				m_listPreActions;				// PreAction buttons info list.

	PokerTableWndEventHandler*				m_pHandler;						// Event handler class.

	ESEasyMotion							m_motion;
	
	// Animation.	
	WinThread								m_animationThread;				// Animation thread.
	List<PokerAnimationInfo>				m_listAnimations;				// Animation queue.
	CCriticalSection						m_lock;

protected:
	virtual BOOL	Create					(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
//	virtual void	OnPaintClient			(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual BOOL	UseDoubleBufferedDC		(){return TRUE;};
	virtual float	GetZoomFactor			(){return m_fZoomFactor;};
	virtual void	OnPrePaint				(CRect& rcRepaint);

	virtual	void	DesignerContextMenu		(CMenu* pPopupMenu, CMenu* pNewControlsMenu, CMenu* pNewChildControlsMenu);
	
	// Generated message map functions
protected:
	afx_msg void	OnShowWindow						(BOOL bShow, UINT nStatus);
	afx_msg void	OnChar								(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnKeyDown							(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnLButtonDblClk						(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown						(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp							(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove							(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor							(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL	OnMouseWheel						(UINT nFlags, short zDelta, CPoint pt);
	afx_msg UINT	OnGetDlgCode						();
	afx_msg BOOL	OnMouseActivate						(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void	OnTimer								(UINT_PTR nIDEvent);
	afx_msg void	OnDestroy							();
	afx_msg LRESULT	OnSeatClick							(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnFlatButtonClick					(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnActionButtonClick					(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnShortcutButtonClick				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnSliderPosChanged					(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnSliderMoneyChanged				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnPreActionChecked					(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnAddChipsClose						(WPARAM wParam, LPARAM lParam);
	// Animations.
	afx_msg LRESULT	OnCollectChipsFinished				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMoveChipsFinished					(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnSeatCardsDealFinished				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnTableCardsDealFinished			(WPARAM wParam, LPARAM lParam);

	// Designer.
	afx_msg	void	OnCommandTable						();
	afx_msg	void	OnCommandSeatStatusAsChild			();
	afx_msg	void	OnCommandSeatAsChild				();
	afx_msg	void	OnCommandChipsAsChild				();
	afx_msg	void	OnCommandHiddenCardsAsChild			();
	afx_msg	void	OnCommandSeatCardsAsChild			();
	afx_msg	void	OnCommandTableCardsAsChild			();
	DECLARE_MESSAGE_MAP()
};