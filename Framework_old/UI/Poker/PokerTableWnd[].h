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
#include "PokerTableWndEventHandler.h"
#include <atlimage.h>

class ChildControl;
class ESChildSeat;		
class ESChildSeatStatus;
class ESChildControlChips;		
class ESChildControlSeatCards;	
class ESChildControlSeatHiddenCards;	
class ESChildControlTableCards;	
class ESChildPreActionControl;
class ESSimpleRenderView;
class ESChildControlImage;			
class PokerHandCards;
class ESChildControlTable;

class PokerTableWnd : public MercuryBaseView
{
// Construction
public:
	PokerTableWnd();
	virtual ~PokerTableWnd();

	friend class ESChildControlSeatHiddenCards;
	friend class PokerTableDlg;
	friend class PokerHandPlayerDlg;

	enum DesignerContextMenuItemIds
		{
		MI_TABLE = 67100,
		MI_SEAT_LABEL_AS_CHILD,
		MI_SEAT_AS_CHILD,
		MI_CHIPS_AS_CHILD,
		MI_HIDDEN_CARDS_AS_CHILD,
		MI_SEAT_CARDS_AS_CHILD,
		MI_TABLE_CARDS_AS_CHILD,
		MI_PREACTION_AS_CHILD
		};
	
public: // Operations.
	void								SetConnectedMode				(bool bConnected = true, bool bRedraw = true);
	void								SetPokerClient					(PokerClient* pClient){m_pClient = pClient;};
	bool								Setup							(CSize szDefault, PokerTableInfo* pTableInfo, EnumerableObject<PokerAvatar>* seatAvatars, 
																		CString	sAmountLabelSufix = _T("GEL"), int nSeatCardsCt = 2, int nRegThinkTimeSec = 0);

	bool								RotateSeatsClockwise			(int nFirstSeatIndexClockwise, bool bRedraw);
	int									GetFirstSeatClockwise			(){return m_nFirstSeatIndexClockwise;};

	bool								SetupSeatTimer					(int nSeat, int nFullSeconds, int nSecondsPassed, bool bInvalidate = false, bool bStart = false);
	bool								StartActionTimer				(int nSeat, bool bStart = true, bool bInvalidate = false);
	bool								IsSeatTimerActive				(int nSeat);

	bool								SetTablePotChips				(int nAmount);
	bool								SetSeatInfo						(int nSeatIndex, PokerTableSeatInfo* pInfo, bool bRedraw = true);
	PokerTableSeatInfo*					GetSeatInfo						(int nSeatIndex){return m_info.m_seats.GetAt(nSeatIndex);};
	void								CopyTableSeats					(PokerTableInfo* pTableInfoInto);

	bool								EmptySeat						(int nSeatIndex, bool bRedraw);
	void								EmptyAllEmptyStatusSeat			(bool bRedraw);
	int									GetEmptySeatCt					();

	bool								UpdateSeat						(int nSeatIndex, bool bRedraw, bool bUpdateOnlySeat = false);
	bool								UpdateSeatStatus				(int nSeatIndex, bool bRedraw);
	bool								UpdateSeatCards					(int nSeatIndex, bool bRedraw);
	bool								UpdateSeatChips					(int nSeatIndex, bool bRedraw);
	bool								UpdateSeatAvatar				(int nSeatIndex, int nAvatarId,  CImage* pAvatarImage, bool bRedraw);

	int									GetMaxPotAmount					();
	bool								SetSeatAvatarInfo				(int nSeat, PokerAvatar* pAvatarInfo, bool bRedraw = true);

	bool								SetActiveSeat					(int nSeatIndex, bool bActivateTimer = false, int nAllSec = 0, int nSecPassed = 0, bool bInvalidate = true); //+	
	int									GetActiveSeat					(){return m_info.m_cActiveSeat;};
	int									GetDealerSeat					(){return m_info.m_cDealerSeat;};
	bool								SetSeatStatus					(int nSeat, PokerSeatStatus status, bool bInvalidate = true);
	PokerSeatStatus						GetSeatStatus					(int nSeat);
	void								SetSeatOffline					(int nSeat, bool bOnline = false, bool bInvalidate = true);
	bool								GetSeatOffline					(int nSeat);
	CString								GetSeatUserName					(int nSeat);
	int									GetMaxSeatCount					(){return m_info.m_seats.GetCount();};
	void								SetRound						(PokerRoundType round, CDWordArray *pListTableCards, bool bInvalidate = false, bool bSetTableCards = true);
	PokerRoundType						GetCurrentRound					(){return m_info.m_round;};
	void								SetGameStatus					(PokerGameStatus gameStatus){m_info.m_gameStatus = gameStatus;};
	void								SetDealerSeat					(int nSeatIndex, bool bInvalidate);
	void								SetTotalPotAmount				(int nTotalPotAmount, bool bRedraw);
	int									GetTotalPotAmount				(){ return m_info.m_nTotalPotAmount; };
	void								SetMainPotAmount				(int nMainPotAmount, EnumerableObject<PokerPotAmountDetail>* pListPots, bool bRedraw, bool bChangeOnlyMainPotChips = false);
	int									GetMainPotAmount				(){return m_info.m_nMainPotAmount;};
	int									GetPotAmount					(int nSeat);
	bool								SetPotAmount					(int nSeat, int nAmount, bool bRedraw = true);
	int									GetSeatTotalPot					(int nSeat);
	bool								SetSeatTotalPot					(int nSeat, int nAmount);
	int									GetBalance						(int nSeat);
	bool								SetBalance						(int nSeat, int nBalance, bool bRedraw = true);
	bool								SetRakeAmount					(int nAmount, bool bRedraw);
	int									GetRakeAmount					(){return m_info.m_nRakeAmount;};

	ActionButtonInfo*					GetActionButtonInfoByIdx		(int nIndex){ return m_listActionButtons.GetAt(nIndex); }
	bool								GetActionButtonByPoint			(CPoint pt, ActionButtonInfo** ppInfo, CRect& rcActionButton);	
	void								ClearPotAmounts					(bool bRedraw = true);

	// Animations.
	virtual bool						CollectChips					(int nMainChipsAmount, EnumerableObject<PokerPotAmountDetail>* pListPots);
	virtual bool						MoveChipsFromPot				(List<PokerHandWinnerInfo>* pListWinners, bool bUseDelayBeforeAnimation);
	virtual bool						DealSeatCardsAnimation			(PokerHandStartedPacket* p);
	virtual bool						DealTableCardsAnimation			(PokerRoundStartedPacket* p);
	void								StopMotions						();

	// Shortcut buttons.
	bool								AddShortcutButton				(CString sLabel, int nAmount, bool bInvalidate = true); 
	bool								AddDisabledShortcutButton		(bool bInvalidate = true);
	bool								ClearShortcutButtons			(bool bInvalidate = true); 
	bool								RedrawShortcutButtons			();

	// Slider.
	bool								ShowSlider						(int nAmount, int nAmountMin, int nAmountMax, int nAmountStep, bool bJumpToClickedPos, bool bInvalidate = true);
	bool								SetSliderAmount					(int nAmount, bool bInvalidate = true);
	bool								HideSlider						(bool bInvalidate = true);
	bool								IsActionSliderVisible			();

	// Action buttons.
	bool								AddActionButton					(ActionButton type, CString sLabel, int nAmountMin, int nAmountMax, bool bInvalidate = true);
	bool								ClearActionButtons				(bool bInvalidate = true);
	bool								RedrawActionButtons				();
	bool								SetActionButtonInfo				(int nIndex, CString sLabel, int nAmount, bool bInvalidate = true);
	bool								RedrawActionButton				(int nIndex);
	bool								IsActionButtonVisible			(ActionButton type);
	ActionButtonInfo*					GetActionButtonInfo				(ActionButton type);

	// Pre-Action buttons.
	bool								AddPreActionButton				(PreActionButton preAction, int nValue, CString sLabel, bool bCheckState = false, bool bInvalidate = true, bool bCalcSize = false);
	bool								RemovePreActionButton			(PreActionButton preAction, bool bInvalidate = true);
	bool								RemoveAllGamePreActions			(bool bInvalidate = true);

	bool								SetPreActionButtonLabel			(PreActionButton preAction, CString sLabel, bool bInvalidate = true);
	bool								ClearPreActions					(bool bInvalidate = true);
	bool								SetPreActionCheckState			(PreActionButton preAction, bool bCheckState, bool bInvalidate = true);
	void								UncheckPreActions				(PreActionButton preActionExcept, bool bInvalidate);
	bool								RedrawPreActions				();
	bool								RedrawPreActionButton			(PreActionButton preAction);

	PreActionButtonInfo*				GetPreActionInfo				(int nId);
	ESChildPreActionControl*			GetPreActionControl				(PreActionButton preAction);
	bool								GetPreActionCheckState			(PreActionButton preAction);

	// Seat action label.
	bool								ClearActionLabels				(bool bRedraw);
	bool								SetSeatActionLabel				(int nSeat, CString sActionName, bool bInvalidate = true);

	void								ClearSeatCards					(bool bRedraw);
	void								ClearSeatHiddenCards			(bool bRedraw);
	void								ClearTableCards					(bool bRedraw);

	// Set seat cards.
	bool								SetSeatCards					(int nSeat, int nCard1, int nCard2, int nCard3, int nCard4, bool bInvalidate = true);
	int									GetSeatCardByIndex				(int nSeat, int nCardIndex);

	// Set table cards.
	void								SetTableCards					(int nCard1, int nCard2, int nCard3, int nCard4, int nCard5, bool bInvalidate = true);

	int									GetTableCard1					(){return m_info.m_cCard1;};
	int									GetTableCard2					(){return m_info.m_cCard2;};
	int									GetTableCard3					(){return m_info.m_cCard3;};
	int									GetTableCard4					(){return m_info.m_cCard4;};
	int									GetTableCard5					(){return m_info.m_cCard5;};

	// Chat
	void								ClearChatMessages				(bool bRedraw);
	void								AddChatMessage					(PokerChatMessage* pMessage);
	int									AddChatMessages					(EnumerableObject<PokerChatMessage>* pListMessage);
	void								AddChatMessage_HandStart		(PokerHandStartedPacket* p);
	void								AddChatMessage_HandFinish		(PokerHandFinishedPacket* p);
	void								AddChatMessage_RoundStart		(PokerRoundStartedPacket* p);
	void								AddChatMessage_SetSeatStatus	(PokerSeatStatusChangedPacket* p);
	void								AddChatMessage_ShowCards		(PokerActionPacket* p);
	void								AddChatMessage_TimeToAct		(int nActTimeSecLeft);

	// Table message label.
	void								SetTableMessage					(CString sMessage, bool bInvalidate);
	void								ClearTableMessage				(bool bInvalidate);

	// Hand combination label.
	void								SetHandCombinationLabel			(CString sLabel, bool bInvalidate);
	void								ClearHandCombinationLabel		(bool bInvalidate);

	// Seat message label.
	void								SetSeatMessage					(int nSeat, CString sMessage, bool bInvalidate);
	void								ClearSeatMessage				(int nSeat, bool bInvalidate);
	void								ClearSeatMessages				(bool bInvalidate);

	// Hand id labels.
	bool								UpdateHandIdLabels				(__int64 nHandId, __int64 nPrevHandId, bool bInvalidate);

	// Set seat hand cards.
	bool								SetBestHandCards				(int nSeat, PokerHandCards* pCards, bool bSetAsSeatMessage = true, CString* psLabelText = NULL, CDWordArray* pArrWinnerCards = NULL);
	// Show winner combination with table cards and table label.		
	bool								SetWinnerTableCards				(PokerHandWinnerInfo* pWinnerInfo, CDWordArray* pArrWinnerCards);
	bool								ClearWinnerTableCards			(bool bRedraw);

	// Player's best hand label.
	void								SetBestHandLabel				(CString sText, bool bInvalidate = true);
	void								ClearBestHandLabel				(bool bInvalidate = true);
	// Set window event handler.
	void								SetWndEventHandler				(PokerTableWndEventHandler* pHandler) {m_pHandler = pHandler;};

	// Set four colored cards.
	bool								SetFourColorCards				(bool bFourColor, bool bRedraw);

	// Set card back index.
	void								SetCardBackIndex				(int nCardBackIndex, bool bRedraw);

	// Set display of folded cards.
	void								SetDisplayFoldedCards			(bool bOn, bool bRedraw);

	// Set hide hole hand cards.
	void								SetHideHoleHandCards			(bool bOn, bool bRedraw);

	void								SetShowPlayerImages				(bool bOn, bool bRedraw);

	// Seat notes.
	bool								SetSeatNoteAndColor				(int nSeat, CString sNote, COLORREF crBorder, bool bRedraw);
	bool								SetSeatNote						(int nSeat, CString sNote, bool bRedraw);
	bool								SetSeatNoteColor				(int nSeat, COLORREF crBorder, bool bRedraw);
	bool								ClearSeatNote					(int nSeat, bool bRedraw);
	bool								ClearSeatNoteColor				(int nSeat, bool bRedraw);

protected:
	int									GetSeatIndex					(PokerTableSeatInfo* pSeat){return m_info.m_seats.IndexOf(pSeat);};
	ESChildSeat*						GetSeatControl					(int nSeat);
	ESChildSeatStatus*					GetSeatStatusControl			(int nSeat);
	ESChildControlChips*				GetChipsControl					(int nSeat);
	ESChildControlSeatCards*			GetSeatCardsControl				(int nSeat);
	ESChildControlSeatHiddenCards*		GetSeatHiddenCardsControl		(int nSeat);
	ESChildControlChips*				GetMainChipsControl				();
	ESChildControlChips*				GetSidePotChipsControl			(int nSidePotIndex);
	ESChildControlTableCards*			GetTableCardsControl			();
	ESChildControlSeatHiddenCards*		GetDeckControl					();
	ESSimpleRenderView*					GetChatViewControl				();
	ESChildControlImage*				GetSeatDealerControl			(int nSeat);
	ESChildControlTable*				GetTableControl					();

	bool								SetLogicalPositionAndSize		(ESChildControl* pControl, CSize szOwner);
	bool								SetLogicalPositionAndSizeByName	(CString sName, CSize szOwner);

	CRect								GetSeatCardsRect				(int nSeat);
	CRect								GetSeatRect						(int nSeat);

	void								HideActionButtons				(bool bShow = false, bool bRedraw = true);
	void								HidePreActionButtons			(bool bShow = false, bool bRedraw = true);
	void								HideShortcutButtons				(bool bShow = false, bool bRedraw = true);

	// Determine weather animation will visible for users.
	bool								AnimationWillBeVisible			(const CRect rcAnimateArea);

public:
	bool								RedrawSeat						(int nSeat, bool bWithCards = false);
	void								RedrawSeats						(bool bWithCards = false);
	bool								RedrawSeatCards					(int nSeat);
	bool								RedrawSeatChips					(int nSeat);
	
protected:
	PokerClient*							m_pClient;						// Poker client class.
	PokerTableInfo							m_info;							// Poker table info.
	CSize									m_szDefaultWindow;
	POINTF									m_arrChipPositions[10];			// Chips positions using default screen dimensions.
	int										m_nSeatCardsCt;					// Seat cards count.
	PokerTableSeatInfo*						m_pTimerActiveSeat;				// Active seat whos timer is set already.
	AutoSortedArray							m_acceptebleMaxPlayersCount;
	int										m_nFirstSeatIndexClockwise;		// First seat index clockwise 0 as default.
	bool									m_bUISetup;						// UI setup flag.
	CString									m_sAmountLabelSuffix;			// Amount label suffix. (GEL, USD, PT or etc)
	bool									m_bConnectedMode;				// Connected mode.
	AutoSortedArray							m_arrPreActionIdByPreAction;	// Preaction button id by preaction type.
	List<PreActionButtonInfo>				m_listPreActions;				// PreAction buttons info list.
	List<ActionButtonInfo>					m_listActionButtons;			// Action button info list.
	List<ShortcutButtonInfo>				m_listShortcutButtons;			// Shortcut button info list.
	ESEasyMotion							m_motionDealSeatCards;
	ESEasyMotion							m_motionDealTableCards;
	ESEasyMotion							m_motionCollectChips;
	ESEasyMotion							m_motionMoveChips;
	PokerTableWndEventHandler*				m_pHandler;						// Event handler class.

protected:
	virtual bool	AttachToFrame					(ESFrameBase* pBaseFrameWnd);
	virtual BOOL	UseDoubleBufferedDC				(){return TRUE;};
	virtual	void	DesignerContextMenu				(CMenu* pPopupMenu, CMenu* pNewControlsMenu, CMenu* pNewChildControlsMenu);

	void			OnSeatClick						(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnActionButtonClick				(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnPreActionButtonClick			(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnShortcutButtonClick			(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnBetSliderPosChanged			(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnBetAmountChanged				(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam);

	void			OnSeatCardsMouseEnter			(ESChildControlSeatCards* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnSeatCardsMouseLeave			(ESChildControlSeatCards* pSender, ChildControlEventCode eventCode, int nParam);

	void			OnSeatMouseEnter				(ESChildSeat* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnSeatMouseLeave				(ESChildSeat* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnSeatTimeBankActivated			(ESChildSeat* pSender, ChildControlEventCode eventCode, int nParam);

	// Table flop card motion events.
	void			OnTableFlopCardDealMotionEnd	(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnTableFlopCardDealMotionStop	(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam);

	void			OnMoveChipsMotionStart			(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnMoveChipsMotionEnd			(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnMoveChipsMotionStop			(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam);

	void			OnCollectChipsMotionStart		(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnCollectChipsMotionEnd			(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnCollectChipsMotionStop		(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam);

	void			OnSeatCardDealMotionEnd			(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam);
	void			OnSeatCardDealMotionStop		(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam);

	// Generated message map functions
protected:	
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
	afx_msg	void	OnCommandPreActionAsChild			();
};

class DealSeatCardsAnimationInfo : public Serializable
{
public:
	DealSeatCardsAnimationInfo();
	virtual ~DealSeatCardsAnimationInfo();

public:
	int			m_nMotionTypeX;
	int			m_nMotionTypeY;
	int			m_nMotionTimeMS;
	int			m_nTimeDistanceBetweenSeatCardAnimation;
	int			m_nTimeDistanceBetweenCardAnimation;
	int			m_nFrameCtPerSecond;
	BYTE		m_btAlphaAtStart;	// Card alpha at start animation.

	virtual Serializable*	CreateSerializableObject	(){return new DealSeatCardsAnimationInfo();};
	INIT_RUNTIME_VARIABLE();
};

class TableCardsAnimationInfo : public Serializable
{
public:
	TableCardsAnimationInfo();
	virtual ~TableCardsAnimationInfo();

public:
	int			m_nMotionTypeX;
	int			m_nMotionTypeY;
	int			m_nMotionTimeMS;
	int			m_nFlopCardsMotionTimeMS;
	int			m_nTimeDistanceBetweenCardAnimation;
	int			m_nFrameCtPerSecond;
	BYTE		m_btAlphaAtStart;			// Card alpha at start animation.
	bool		m_bSlideCardsOnFLop;		// Slide cards animation.

	virtual Serializable*	CreateSerializableObject	(){return new TableCardsAnimationInfo();};
	INIT_RUNTIME_VARIABLE();
};

class CollectChipsAnimationInfo : public Serializable
{
public:
	CollectChipsAnimationInfo();
	virtual ~CollectChipsAnimationInfo();

public:
	int			m_nMotionTypeX;
	int			m_nMotionTypeY;
	int			m_nMotionTimeMS;
	int			m_nFrameCtPerSecond;
	int			m_nDelayAfterMotionMS;

	virtual Serializable*	CreateSerializableObject	(){return new CollectChipsAnimationInfo();};
	INIT_RUNTIME_VARIABLE();
};


class MoveChipsAnimationInfo : public Serializable
{
public:
	MoveChipsAnimationInfo();
	virtual ~MoveChipsAnimationInfo();

public:
	int			m_nMotionTypeX;
	int			m_nMotionTypeY;
	int			m_nMotionTimeMS;
	int			m_nFrameCtPerSecond;

	virtual Serializable*	CreateSerializableObject	(){return new MoveChipsAnimationInfo();};
	INIT_RUNTIME_VARIABLE();
};
