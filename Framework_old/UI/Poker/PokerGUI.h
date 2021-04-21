// PokerGUI.h : header file
//

#pragma once
#include "../../_platformCompat/PlatformCompat.h"
#include "../../_platformCompat/PlatformDateTime.h"

// Defines.
#define POKER_TABLE_HAND_FINISH_TIMER		5000
#define POKER_TABLE_ROUND_START_TIMER		5001
#define POKER_SEAT_ACTION_TIMER				5002
#define POKER_OPEN_LOGIN_DIALOG_TIMER		5003
#define POKER_CLEAR_TABLE_MESSAGE_TIMER		5004
#define POKER_CLOSE_TABLE_TIMER				5005
#define POKER_TOURNAMENT_LEVEL_COUNTDOWN	5006

// Lobby timer id's
#define POKER_LOBBY_LOAD_TABLES				5104
#define POKER_LOBBY_LOAD_TABLE_PLAYERS		5105
#define POKER_LOBBY_REFRESH_ALL				5106

// Message defines.
#define WM_SERVER_CONNECTION_CHANGE			WM_USER + 99
#define WM_SEAT_CLICK						WM_USER + 100
#define WM_ACTION_BUTTON_CLICK				WM_USER + 101
#define WM_SHORTCUT_BUTTON_CLICK			WM_USER + 102
#define WM_SLIDER_POS_CHANGED				WM_USER + 103
#define WM_PREACTION_CHECKED				WM_USER + 104
#define WM_SHOW_ADD_CHIPS_DIALOG			WM_USER + 105
#define WM_LOBBY_TAB_ITEM_CHANGED			WM_USER + 106
#define WM_IMAGE_BUTTON_CLICK				WM_USER + 107
#define WM_LIST_ROW_CHANGED					WM_USER + 108
#define WM_LIST_ROW_DBLCLICK				WM_USER + 109
#define WM_LOBBY_OPEN_TABLE					WM_USER + 110
#define WM_LOBBY_LOGIN						WM_USER + 111
#define WM_LOBBY_LOGOUT						WM_USER + 112
#define WM_LOBBY_CASHIER					WM_USER + 113
#define WM_PACKET_RECEIVED					WM_USER + 114
#define WM_SLIDER_MONEY_CHANGED				WM_USER + 115
#define WM_FLAT_BUTTON_CLICK				WM_USER + 116
#define WM_ADD_CHIPS_CLOSE_BUTTON_CLICK		WM_USER + 117
#define WM_HIDE_EMPTY_TABLES				WM_USER + 119
#define WM_HIDE_FULL_TABLES					WM_USER + 120
#define WM_OPEN_DIALOG						WM_USER + 121
#define WM_SET_TABLE_FELT_COLOR				WM_USER + 122
#define WM_SET_TABLE_BG_IMAGE				WM_USER + 123


// Animations.
#define WM_COLLECT_CHIPS_FINISHED			WM_USER + 200
#define WM_MOVE_CHIPS_FROM_POT_FINISHED		WM_USER + 201
#define WM_SEAT_CARDS_DEAL_FINISHED			WM_USER + 202
#define WM_TABLE_CARDS_DEAL_FINISHED		WM_USER + 203

enum AnimationType{
	CollectChips = 1,
	ChipsMovement,
	DealSeatCards,
	DealTableCards
	};

// Sound types.
enum SoundTypes{
	Sound_SystemMessage = 0,
	Sound_Attention,
	Sound_CardsDealt,
	Sound_ChipsMovedToPot,
	Sound_ChipsMovedFromPot,
	Sound_Fold,
	Sound_Check,
	Sound_Bet,
	Sound_Raise,
	Sound_Call,
	Sound_AllIn,
	Sound_TimeWarning,
	Sound_ChatMessage
	};

enum PreActionButton{
	PreAction_Fold,
	PreAction_CheckFold,
	PreAction_Check,
	PreAction_Call,
	PreAction_CallAny,
	PreAction_CallAnyCheck,
	PreAction_WaitForBB_PostBB
	};

enum ActionButton{
	ButtonCheck = 1,
	ButtonCall,
	ButtonFold,
	ButtonBet,
	ButtonRaise,
	ButtonPostSB,
	ButtonPostBB,
	ButtonSitOut,
	ButtonReturnBack,
	ButtonShowCards,
	ButtonMuck,
	ButtonAllIn,
	ButtonPayBB,
	ButtonWaitingForBB,
	ButtonJoinWaitingList,
	ButtonFindSeat
	};

enum TableAnimationType{
	TableAnimationType_Disabled = 1,
	TableAnimationType_Simplified,
	TableAnimationType_Full
	};

enum PushButtonState{
	Normal = 0,
	Hover,
	Pushed,
	Disabled
	};

struct ActionButtonInfo{
	ActionButton	type;
	std::string		sLabel;
	int				nAmountMin;
	int				nAmount;
	int				nAmountMax;
	int				nButtonId;
	};

struct ShortcutButtonInfo{
	std::string		sLabel;
	int				nAmount;
	int				nButtonId;
	};

struct PreActionButtonInfo{
	PreActionButton	type;
	int				nButtonId;
	int				nValue;
	};

class PokerTableSettings
{
protected:
	PokerTableSettings()
	{
		m_b4ColoredCards			= false;
		m_bShowPlayerAvatarImage	= true;
		m_bDisplayFoldedCards		= true;
		m_bHideHoleCards			= false;
		m_bDisplayeEmoticons		= true;
		m_nAnimationType			= 0;
		m_nCardsBackIndex			= 3;
	};
	~PokerTableSettings(){};

public:
	static PokerTableSettings* GetInstance(){
		static PokerTableSettings settings;
		return &settings;
		}

public:
	bool		m_b4ColoredCards;
	bool		m_bShowPlayerAvatarImage;
	bool		m_bDisplayFoldedCards;
	bool		m_bHideHoleCards;
	bool		m_bDisplayeEmoticons;
	int			m_nAnimationType;
	int			m_nCardsBackIndex;
};

class PokerHelper
{
public:
	PokerHelper(){}
	virtual ~PokerHelper(){};

	// Static methods.
	static	std::string			GetGameName					(int nGameType, int nGameSubType);
	static	std::string			GetGameNameEng				(int nGameType, int nGameSubType);
	static  int					GetSubTypesByGameType		(int nGameType);		
	static	std::string			GetCurrencyName				(int nCurrencyId);
	static	std::string			GetFormattedAmount			(int nCurrencyId, int nAmount);
	static	std::string			GetTournamentStatus			(int nStatus);
	static	std::string			GetTournamentDateAsString	(DateTime dtTime);
	static	std::string			GetTournamentBuyin			(int nBuyinAmount, int nBuyinFeeAmount, int nCurrencyId);
	static	std::string			CardNameByIndex				(int nIndex);
};
