// PokerGUI.h : header file
//

#pragma once

#include "..\..\Array\List.h"
#include "..\..\Array\AutoSortedArrayTempl.h"
#include "..\..\Array\AutoSortedArrayInt64.h"
#include "..\..\Serialize\Serializable.h"
#include "..\..\Image\MipMapImage.h"
#include <atlimage.h>

//#define LOAD_FILES_FROM_DLL

class ESChildControl;
class ESChildControlBase;

#pragma comment(lib, "Winmm.lib")

// Defines.
#define POKER_TABLE_HAND_FINISH_TIMER		1000
#define POKER_TABLE_ROUND_START_TIMER		1001
#define POKER_SEAT_ACTION_TIMER				1002
#define POKER_OPEN_LOGIN_DIALOG_TIMER		1003
#define POKER_CLEAR_TABLE_MESSAGE_TIMER		1004
#define POKER_CLOSE_TABLE_TIMER				1005
#define POKER_TOURNAMENT_LEVEL_COUNTDOWN	1006

// Lobby timer id's
#define POKER_LOBBY_LOAD_TABLES				1104
#define POKER_LOBBY_LOAD_TABLE_PLAYERS		1105
#define POKER_LOBBY_REFRESH_ALL				1106

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

// Animations.
#define WM_COLLECT_CHIPS_FINISHED			WM_USER + 121
#define WM_MOVE_CHIPS_FROM_POT_FINISHED		WM_USER + 122
#define WM_SEAT_CARDS_DEAL_FINISHED			WM_USER + 123
#define WM_TABLE_CARDS_DEAL_FINISHED		WM_USER + 124

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

enum PushButtonState{
	Normal = 0,
	Hover,
	Pushed,
	Disabled
	};

// Lobby child control names. {{
#define _LobbyLogo						_T("LobbyLogo")
#define _LobbyBanner					_T("LobbyBanner")
#define _LobbyInfoLabel					_T("LobbyInfo")
#define _LobbyUserName					_T("LobbyUserName")
#define _LobbyUserBalanceLabel			_T("LobbyUserBalanceLabel")
#define _LobbyUserBalance				_T("LobbyUserBalance")
#define _LobbyUserInPlayLabel			_T("LobbyUserInPlayLabel")
#define _LobbyUserInPlay				_T("LobbyUserInPlay")
#define _LobbyUserAvatar				_T("LobbyUserAvatar")
#define _LobbyUserPoints				_T("LobbyUserPoints")
#define _LobbyGamesTab					_T("LobbyGamesTab")
#define _LobbySubGamesTab				_T("LobbySubGamesTab")
#define _LobbyTab1						_T("LobbyTab1")
#define _LobbyTablesList				_T("LobbyTablesList")
#define _LobbyPlayersList				_T("LobbyPlayersList")
#define _LobbyTournamentsList			_T("LobbyTournamentsList")
#define _LobbyTournamentInfo			_T("LobbyTournamentInfo")
#define _LobbyLoginButton				_T("LobbyLoginButton")
#define _LobbyLogoutButton				_T("LobbyLogoutButton")
#define _LobbyCashierButton				_T("LobbyCashierButton")
#define _LobbyOpenTableButton			_T("LobbyOpenTableButton")
#define _LobbyJoinTableButton			_T("LobbyJoinTableButton")
#define _LobbyOpenTournamentButton		_T("LobbyOpenTournamentButton")
// }}

enum ChildControls{
	CHILD_ActionButton			= 200,
	CHILD_ShortcutButton		= 300,
	CHILD_Slider				= 400,
	CHILD_PreAction				= 500,
	/*
	CHILD_PokerActionPanel		= 100,
	CHILD_PokerTableCards		= 101,
	CHILD_PokerTable			= 150,
	CHILD_Seat					= 600,
	CHILD_SeatCards				= 700,
	CHILD_SeatHiddenCards		= 720,
	CHILD_SeatOpenCards			= 750,
	CHILD_SeatChips				= 800,
	CHILD_SeatMainPotChips		= CHILD_SeatChips + 10,
	CHILD_SeatDealer			= 900,
	CHILD_SeatProgressBar		= 1000,
	CHILD_Seat_Message			= 1050,
	CHILD_TableTabView			= 1100,
	CHILD_TableChatPage			= 1101,
	CHILD_TableInfoPage			= 1102,
	CHILD_TableStatPage			= 1103,
	CHILD_TableWaitPage			= 1104,
	CHILD_TotalPotLabel			= 1105,
	CHILD_AddChipsControl		= 1106,
	CHILD_AddChipsButton		= 1107,
	CHILD_AddChipsMoneyEdit		= 1108,
	CHILD_AddChipsMinLabel		= 1109,
	CHILD_AddChipsMaxLabel		= 1110,
	CHILD_AddChipsBalanceLabel	= 1111,

	CHILD_Button_LeaveTable		= 1140,
	CHILD_Button_ViewLobby		= 1141,
	CHILD_Button_BuyChips		= 1142,
	CHILD_Button_Rebuy			= 1143,
	CHILD_Button_Addon			= 1144,
	CHILD_Button_History		= 1145,
	CHILD_Table_Message			= 1146,
	CHILD_Table_Best_Hand_Label	= 1147,
	CHILD_Table_Chat_Edit		= 1148,
	CHILD_Table_FlyingCover		= 1149,

	CHILD_LobbyLogo				= 2000,
	CHILD_LobbyTabGames,
	CHILD_LobbyTabGameLimits,
	CHILD_LobbyTopBanner,
	CHILD_LobbyUserInfo,
	CHILD_LobbyTabPlayers,

	CHILD_LobbyListPlayers,
	CHILD_LobbyListTables,
	CHILD_LobbyListTournaments,
	CHILD_LobbyTournamentDescLabel,

	CHILD_LobbyCashier,
	CHILD_LobbyLogin,
	CHILD_LobbyLogout,
	CHILD_LobbyOpenTable,
	CHILD_LobbyOpenTournamentLobby,
	CHILD_SliderMoneyEdit,
	CHILD_LobbyInfoLabel,

	CHILD_LobbyFilterFullTables,
	CHILD_LobbyFilterEmptyTables,

	CHILD_HistoryView			= 3001,
	CHILD_HistoryTitle,


	CHILD_LoginDialog	= 4000,*/


	};

struct ActionButtonInfo{
	ActionButton	type;
	CString			sLabel;
	int				nAmountMin;
	int				nAmount;
	int				nAmountMax;
	int				nButtonId;
	};

struct ShortcutButtonInfo{
	CString			sLabel;
	int				nAmount;
	int				nButtonId;
	};

struct PreActionButtonInfo{
	PreActionButton	type;
	int				nButtonId;
	int				nValue;
	};

class MercuryBaseView;
class GUILanguage;
class PokerGUI : public Serializable
{
public:
	PokerGUI();
	~PokerGUI();

	static PokerGUI* GetInstance(){
		static PokerGUI pokerGUI;
		if( !pokerGUI.m_bInited )
			pokerGUI.Initialize();
		return &pokerGUI;
		}

	// Retrieves chip images by amount.
	int				GetChipImagesByAmount	(int nAmount, CPtrArray& arrChipImages);

	static CString	CardNameByIndex				(int nIndex);
	static CString	GetLangValue				(TCHAR* pszKey);

protected:
	BOOL			Initialize					();
	void			Destroy						();
	bool			LoadCardImages				();
	bool			LoadChipImages				();
	CImage*			LoadTableImage				(CString sTableImage, CString sFloorImage);
	void			CreateAlphaChannelImage		(CImage* pImage);

public:

	CImage*			GetCardImageByIndex			(int nIndex);
	bool			GetPushButtonItemRect		(PushButtonState state, CRect& rcSrcImage);
	bool			GetPreActionPosition		(PreActionButton preAction, CPoint& pos);
	bool			AddPreActionPosition		(PreActionButton preAction, CPoint pos);

	void			PlaySoundByType				(SoundTypes soundType, int nPlayCt = 1, int nDelayBetween = 0);

public:
	static CImage*	LoadImage					(CString sImageFile, int nWidth = -1, int nHeight = -1);
	static CString	LoadXML						(CString sXMLFile);

	static CImage*	LoadPngFromResource			(HMODULE hResModule, LPCTSTR lpszResName);

	static bool		GetImageFromImage			(CImage* pDestImage, CRect rcDest, CImage& image);


	static bool		RenderMipmapImage			(CDC* pDC,						/*Destination DC*/
												const CRect rcClipDestDC,		/*Destination clip rect in DC coordinates*/
												const CRect rcDestDC,			/*Destination rect in DC coordinates */
												MipmapImage* pImageSrc,  bool bStretch = true, bool bMemDC = false );

	static	void	DrawText					(CDC* pDC, CFont* pFont, int x, int y, int flag, CRect rcClipRect, TCHAR* pszText, float fStretchCX, float fStretchCY);
	static	bool	StretchedTextFont			(CFont* pFontIn, CFont* pFontOut, float fStretchCX, float fStretchCY, bool bUnderLine = false, bool bBold = false);
	static  bool	ExtTextOut					(CDC* pDC, int x, int y, UINT nOptions, const RECT* lpRect, CString sText);

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerGUI();};
	INIT_RUNTIME_VARIABLE()

public:
	HMODULE								m_hResModule;
	BOOL								m_bInited;
	List<CImage>						m_listUsedImages;
	List<CFont>							m_listUsedFonts;
	AutoSortedArray						m_arrAlphaChannel;			// <CImage* original image><CImage* alpha channel image.>

	CImage*								m_pImageBg;
	CImage*								m_pImageFg;
	CImage*								m_pImageFgBorder;
	CImage*								m_pImageTable;

	CImage*								m_pImageBannerButtonDef;
	CImage*								m_pImageBannerButtonActive;

	CImage*								m_pImageResizeBar;
	CImage*								m_pImageCheckBox_Checked;
	CImage*								m_pImageCheckBox_NoCheck;
	CImage*								m_pImageTextBoxBg;



	CImage								*m_pTableBgImg;
	CImage								*m_pFloorBgImg;
	CImage								*m_pCardBackImg;
	CImage								*m_pSeatPanelBgImg;
	CImage								*m_pSeatPanelBgDisImg;		// Disabled seat panel background image.
	CImage								*m_pSeatPanelActBgImg;		// Active seat panel background image.
	CImage								*m_pRankImg;
	CImage								*m_pDealerBtnImg;
	CImage								*m_pPBarBgImg;
	CImage								*m_pPBarFillImg;
	CImage								*m_pEmptySeatLogo;
	CImage								*m_pActionPanelBg;
	CImage								*m_pActionButtonClicked;
	CImage								*m_pActionButtonDefault;
	CImage								*m_pShortcutButtonDefault;
	CImage								*m_pShortcutButtonClicked;
	CImage								*m_pSliderMinus;
	CImage								*m_pSliderPlus;
	CImage								*m_pSliderCursor;
	CImage								*m_pSliderBarBg;
	CImage								*m_pSliderBg;
	CImage								*m_pPreActionBg;
	CImage								*m_pPreActionChecked;
	CImage								*m_pPreActionUnchecked;
	CImage								*m_pTabViewItemActive;
	CImage								*m_pTabViewItemDefault;
	CImage								*m_pAddChipskgnd;
	CImage								*m_pTableMessageBk;

	CImage								*m_pLobbyBkgndImage;
	CImage								*m_pLobbyLogoImage;

	CImage								*m_pLobbyTabDefaultImage;
	CImage								*m_pLobbyTabActiveImage;
	CImage								*m_pLobbyTabHoverImage;

	CImage								*m_pCashierImageDef;
	CImage								*m_pCashierImageSel;
	CImage								*m_pCashierImageHover;
	CImage								*m_pCashierImageDis;

	CImage								*m_pLogInImageDef;
	CImage								*m_pLogInImageSel;
	CImage								*m_pLogInImageHover;
	CImage								*m_pLogInImageDis;
	/*
	CImage								*m_pLobbyButtonDef;
	CImage								*m_pLobbyButtonSel;
	CImage								*m_pLobbyButtonHover;
	CImage								*m_pLobbyButtonDis;
	*/
	CImage								*m_pUserInfoBkgnd;
	CImage								*m_pUserInfoRank;
	CImage								*m_pUserInfoRankDis;
	CImage								*m_pVScrollBar;

	CImage								*m_pCloseButton;

	CImage								*m_pDiamondLogo_Small;
	CImage								*m_pHeartLogo_Small;
	CImage								*m_pSpadeLogo_Small;
	CImage								*m_pClubLogo_Small;
	CImage								*m_pMoneyTransferArrow;
	CImage								*m_pDialogBk;
	CImage								*m_pDialogCloseNormal;
	CImage								*m_pDialogCloseHover;
	CImage								*m_pDialogClosePushed;

	// Lobby images. {{
	CImage								*m_pLobbyLogo;
	CImage								*m_pLobbyTabItemActive2x;
	CImage								*m_pLobbyTabItem2x;
	CImage								*m_pLobbyTabItemActive;
	CImage								*m_pLobbyTabItem;
	
	CRect								m_rcTabItemOmitBorder2x;
	CRect								m_rcActiveTabItemOmitBorder2x;

	CRect								m_rcTabItemOmitBorder;
	CRect								m_rcActiveTabItemOmitBorder;

	CImage								*m_pLobbyCashierButtonClick;
	CImage								*m_pLobbyCashierButtonDef;
	CImage								*m_pLobbyCashierButtonHover;

	CImage								*m_pLobbyLoginButtonClick;
	CImage								*m_pLobbyLoginButtonDef;
	CImage								*m_pLobbyLoginButtonHover;

	CImage								*m_pLobbyButtonClick;
	CImage								*m_pLobbyButtonDef;
	CImage								*m_pLobbyButtonHover;
	CImage								*m_pLobbyButtonDisable;

	CFont								*m_pLobbyTabItemFont;
	CFont								*m_pLobbyActiveTabItemFont;

	// }}

	float								m_fMinStretch;				// Minimal stretch koef for text.

	CString								m_sFloorBgImage;			// Floor background image.
	CString								m_sTableBgImage;			// Table background image.
	CString								m_sCardBackImage;			// Card background image.
	CString								m_sSeatPanelBgImage;		// Seat panel background image.
	CString								m_sSeatPanelBgDisImage;		// Disabled seat panel background image.
	CString								m_sSeatPanelActBgImage;		// Active seat panel background image.
	CString								m_sCardsFolder;				// Cards folder.
	CString								m_sChipsFolder;				// Chips folder.
	CString								m_sRankImage;				// Rank image.
	CString								m_sDealerBtnImage;			// Dealer button image.
	CString								m_sPBarBgImage;				// Progress bar background image.
	CString								m_sPBarFillImage;			// Progress bar fill image.
	CString								m_sEmptySeatLogoImage;		// Empty seat logo image.
	CString								m_sActionPanelImage;		// Action panel background image.
	CString								m_sActionBtnImageClk;		// Clicked action button image.
	CString								m_sActionBtnImageDef;		// Default action button image.
	CString								m_sShortcutBtnImageDef;		// Shortcut button default background image.
	CString								m_sShortcutBtnImageClicked;	// Shortcut button clicked background image.
	CString								m_sSliderMinusImage;		// Slider minus image.
	CString								m_sSliderPlusImage;			// Slider plus image.
	CString								m_sSliderCursorImage;		// Slider cursor image.
	CString								m_sSliderBarBgImage;		// Slider bar background image.
	CString								m_sSliderBgImage;			// Slider background image.
	CString								m_sPreActionBg;				// PreAction background image.
	CString								m_sPreActionChecked;		// PreAction check state image.
	CString								m_sPreActionUnchecked;		// PreAction uncheck state image.
	CString								m_sTabViewItemActive;		// TabView  active item image.
	CString								m_sTabViewItemDefault;		// TabView  default item image.
	CString								m_sAddChipsBkgnd;			// Add chips control background image.
	CString								m_sTableMessageBk;			// Table message background.

	CString								m_sLobbyBkgnd;				// Lobby background image.
	CString								m_sLogoImage;				// Lobby logo image.

	CString								m_sLobbyTabDefaultImage;	// Lobby tab default item image.
	CString								m_sLobbyTabActiveImage;		// Lobby tab active item image.
	CString								m_sLobbyTabHoverImage;		// Lobby tab hover item image.

	CString								m_sUserInfoRankImage;		// Lobby user info rank indicator start image.
	CString								m_sUserInfoRankImageDis;	// Lobby user info rank indicator start image disabled.

	CString								m_sVScrollBar;				// Vertical scroll bar image.
	CString								m_sDialogImage;				// Dialog image.
	CString								m_sDialogCloseNormalImage;	// Dialog close normal image.
	CString								m_sDialogCloseHoverImage;	// Dialog close hover image.
	CString								m_sDialogClosePushedImage;	// Dialog close pushed image.

	CString								m_sCloseButtonImage;
	CString								m_sDiamondLogoSmall;		
	CString								m_sHeartLogoSmall;			
	CString								m_sSpadeLogoSmall;			
	CString								m_sClubLogoSmall;	

	CString								m_sMoneyTransferArrow;		// Money transfer arrow.

	CString								m_sAvatarsCacheFolder;		// Avatars cache folder.

	// Lobby cashier image.
	CString								m_sCashierImageDefault;	
	CString								m_sCashierImageSelected;
	CString								m_sCashierImageHover;	
	CString								m_sCashierImageDisabled;

	// Lobby login image.
	CString								m_sLogInImageDefault;	
	CString								m_sLogInImageSelected;
	CString								m_sLogInImageHover;	
	CString								m_sLogInImageDisabled;

	// Lobby open table image.
	CString								m_sLobbyButtonDefault;	
	CString								m_sLobbyButtonSelected;
	CString								m_sLobbyButtonHover;	
	CString								m_sLobbyButtonDisabled;

	// Lobby button font.
	CString								m_sLobbyButtonFont;			
	int									m_nLobbyButtonFontHeight;		
	int									m_nLobbyButtonBigFontHeight;		
	int									m_nLobbyButtonFontWeight;		
	CFont								*m_pLobbyButtonFont;			
	CFont								*m_pLobbyButtonFontBig;			
	COLORREF							m_crLobbyButton;				

	CString								m_sUserInfoBkgnd;			// User info background.


	List<CImage>						m_chipImages;				// All chip images.
	AutoSortedArrayTempl<int, CImage*>	m_chipImageByAmount;		// ChipImage by amount.
	List<CImage>						m_cardImages;

	CPoint								m_ptTablePos;				// Table window position on the floor.
	CSize								m_szTable;					// Table window size.
	CSize								m_szDefWindowSize;			// Default table window size.
	CSize								m_szCardDimension;			// Card dimensions.
	CSize								m_szHiddenCardDimension;	// Hidden card dimensions.
	int									m_nSeatCardsPanelCX;		// Seat card panel width.
	CSize								m_szSeatPanel;				// Seat panel dimensions.

	CPoint								m_ptSeatCard1Pos;			// Seat card 1 position depending on seat panel position.
	CPoint								m_ptSeatCard2Pos;			// Seat card 2 position depending on seat panel position.
	CPoint								m_ptSeatCard3Pos;			// Seat card 3 position depending on seat panel position.
	CPoint								m_ptSeatCard4Pos;			// Seat card 4 position depending on seat panel position.

	CPoint								m_ptTableCard1;				// Table card 1 position.
	CPoint								m_ptTableCard2;				// Table card 2 position.
	CPoint								m_ptTableCard3;				// Table card 3 position.
	CPoint								m_ptTableCard4;				// Table card 4 position.
	CPoint								m_ptTableCard5;				// Table card 5 position.

/*	EnumerableObject<SeatPanelInfo>		m_seatInfo2;				// Seat panel info for 2 player.
	EnumerableObject<SeatPanelInfo>		m_seatInfo4;				// Seat panel info for 4 player.
	EnumerableObject<SeatPanelInfo>		m_seatInfo6;				// Seat panel info for 6 player.
	EnumerableObject<SeatPanelInfo>		m_seatInfo9;				// Seat panel info for 9 player.
	EnumerableObject<SeatPanelInfo>		m_seatInfo10;				// Seat panel info for 10 player.
	*/
	CString								m_sChipLabelFont;			// Chip label font name.
	int									m_nChipLabelFontHeight;		// Chip label font height.
	CFont								*m_pChipLabelFont;			// Chip label font object.
	COLORREF							m_crChipLabel;				// Chip label text color.
	CSize								m_szChipImage;				// Chip image dimensions.
	int									m_nChipCountInCol;			// Chip count in one column.
	int									m_nChipsDistanceCY;			// Distance between chips.
	float								m_fChipsZoomMax;			// Max zoom of chip and chip container.
	float								m_fChipsZoomMin;			// Min zoom of chip and chip container.
	CSize								m_szChipContainer;			// Chip container size;
	CSize								m_szDealer;					// Dealer button image.
	CPoint								m_ptDealerCardPos;			// Dealer cards position to deal it to seats.

	CString								m_sSeatLabelFont;			// Seat label font name.
	int									m_nSeatLabelFontHeight;		// Seat label font height.
	CFont								*m_pSeatLabelFont;			// Seat label font object.
	CFont								*m_pSeatLabelFontItalic;	// Seat label italic font object.

	COLORREF							m_crSeatUserNameLabel;		// User name label color on seat.
	COLORREF							m_crSeatBalanceLabel;		// Balance label color on seat.
	COLORREF							m_crSeatActionLabel;		// Action label color on seat.

	CPoint								m_ptSeatUserNameLabel;		// User name label 
	CPoint								m_ptSeatBalanceLabel;		// Balance label 
	CPoint								m_ptSeatActionLabel;		// Action label 

	CString								m_sEmptySeatLabelFont;		// Empty seat label font name.
	int									m_nEmptySeatLabelFontHeight;// Empty seat label font height.
	int									m_nEmptySeatLabelFontWeight;// Empty seat label font weight.
	COLORREF							m_crEmptySeatLabel;			// Empty seat label color.
	CFont								*m_pEmptySeatLabelFont;		// Empty seat label font object.

	CSize								m_szAvatar;					// Avatar image dimensions.
	CPoint								m_ptAvatarPos;				// Avatar image position.
	CPoint								m_ptRankIndicatorPos;		// Rank indicator position.

	CPoint								m_ptMainPotChipsPos;		// Main pot chips position.
	CString								m_sMainPotLabelFont;		// Main label font name.
	int									m_nMainPotLabelFontHeight;	// Main label font height.
	CFont								*m_pMainPotLabelFont;		// Main label font object.
	COLORREF							m_crMainPotLabel;			// Main label text color.

	CPoint								m_ptTotalPotLabelPos;		// Total pot label position.
	CSize								m_szTotalPotLabel;			// Total pot label size.
	CString								m_sTotalPotLabelFont;		// Total pot label font name.
	int									m_nTotalPotLabelFontHeight;	// Total pot label font height.
	CFont								*m_pTotalPotLabelFont;		// Total pot label font object.
	COLORREF							m_crTotalPotLabel;			// Total pot label text color.

	CSize								m_szProgressBar;			// Progress bar dimensions.
	int									m_nMinProgressBarCY;		// Min height of progress bar.
	int									m_nMaxProgressBarCY;		// Max height of progress bar.
	CPoint								m_ptProgressBarPos;			// Progress bar position depending on seat position.

	CImage*								m_pPushButton;				// Pushbutton image.

	CImage*								m_pPushButtonNormal;		// Pushbutton normal image.
	CImage*								m_pPushButtonHover;			// Pushbutton hover image.
	CImage*								m_pPushButtonPushed;		// Pushbutton pushed image.
	CImage*								m_pPushButtonDisabled;		// Pushbutton disabled image.


	CString								m_sPushButtonImage;			// Pushbutton image file.
	CRect								m_rcPushButtonOmitBorder;	// Pushbutton image omit border rect.
	CSize								m_szPushButtonItem;			// Pushbutton item size.
	COLORREF							m_crPushButtonText;			// Pushbutton text color.
	CString								m_sPushButtonFont;			// Pushbutton font name.
	int									m_nPushButtonFontHeight;	// Pushbutton font height.
	int									m_nPushButtonFontWeight;	// Pushbutton font weight.
	CFont								*m_pPushButtonFont;			// Pushbutton font.
	CSize								m_szEmptySeatLogo;			// Empty seat logo dimensions.

	CRect								m_rcPlayerBestHandLabel;	// Player best hand label rect.

	// Action panel 
	CRect								m_rcActionPanel;			// Action panel rect.

	// Add chips control. 
	CRect								m_rcAddChips;				// Add chips panel rect.

	// Table message.
	CRect								m_rcTableMessageOmit;		// Table message bk omit border rect.
	CString								m_sTMessageFont;			// Table message font name.
	int									m_nTMessageFontHeight;		// Table message font height.
	int									m_nTMessageFontWeight;		// Table message font weight.
	CFont								*m_pTMessageFont;			// Table message font.
	COLORREF							m_crTMessageText;			// Table message text color.

	// Action button.
	CPoint								m_ptActionButton;			// Action button starting offset.
	CSize								m_szActionButton;			// Action button dimensions.
	CString								m_sActionButtonFont;		// Action button font name.
	int									m_nActionButtonFontHeight;	// Action button font height.
	int									m_nActionButtonFontWeight;	// Action button font weight.
	CFont								*m_pActionButtonFont;		// Action button font.
	COLORREF							m_crActionButtonText;		// Action button text color.

	// Shortcut button(s)
	CPoint								m_ptShortcutButton;			// Starting point of shortcut buttons.
	CSize								m_szShortcutButton;			// Shortcut button dimensions.
	CString								m_sShortcutButtFont;		// Shortcut button font name.
	int									m_nShortcutButtFontHeight;	// Shortcut button font height.
	int									m_nShortcutButtFontWeight;	// Shortcut button font weight.
	CFont*								m_pShortcutButtFont;		// Shortcut button font object pointer.
	COLORREF							m_crShortcutButtText;		// Shortcut button label text color.
	COLORREF							m_crShortcutButtTextClicked;// Clicked shortcut button label text color.

	// Poker table window money slider
	CPoint								m_ptSlider;					// Slider position.
	CSize								m_szSlider;					// Slider dimensions.
	CRect								m_rcSliderMoneyEdit;		// Slider money edit.
	CString								m_sSliderLabelFont;			// Slider label font name.
	int									m_nSliderLabelFontHeight;	// Slider label font height.
	int									m_nSliderLabelFontWeight;	// Slider label font weight.
	CFont*								m_pSliderFont;				// Slider font object pointer.
	COLORREF							m_crSliderLabel;			// Slider label color.

	CSize								m_szSliderMinus;			// Slider minus sign.
	CSize								m_szSliderPlus;				// Slider plus sign.
	CSize								m_szSliderBar;				// Slider bar.
	CSize								m_szSliderCursor;			// Slider cursor.

	// Pre-Action checkbox
	CSize								m_szPreAction;				// Pre-Action checkbox dimensions.
	CString								m_sPreActionLabelFont;		// Pre-Action label font name.
	int									m_nPreActionLabelFontHeight;// Pre-Action label font height.
	int									m_nPreActionLabelFontWeight;// Pre-Action label font weight.
	CFont*								m_pPreActionFont;			// Pre-Action font object pointer.
	COLORREF							m_crPreActionLabel;			// Pre-Action label color.
	AutoSortedArrayInt64				m_arrPreActionPosByType;	// Pre-Action position by type. <PreActionButton><POINT>	

	// TabView control.
	CSize								m_szTabView;				// TabView control dimensions.
	CPoint								m_ptTabView;				// TabView control position.
	COLORREF							m_crTabViewBorder;			// TabView border color.
	int									m_nTabViewItemCY;			// TabView item height.
	int									m_nTabViewItemLabelHeight;	// TabView item label font height.
	CString								m_sTabViewItemLabelFont;	// TabView item label font.
	CFont*								m_pTabViewItemFont;			// TabView item label font.
	COLORREF							m_crTabViewItemLabelAct;	// Active Tabviw item label color.
	COLORREF							m_crTabViewItemLabel;		// Tabviw item label color.

	// Chat control.
	COLORREF							m_crChatTime;				// Chat time color.
	COLORREF							m_crChatUserName;			// Chat user name color.
	COLORREF							m_crChatMessage;			// Chat message color.
	COLORREF							m_crChatSystem;				// Chat system message color.
	COLORREF							m_crChatDealer;				// Chat dealer message color.
	CString								m_sChatFont;				// Chat font name.
	int									m_nChatTimeFontHeight;		// Chat time font height.
	int									m_nChatUserNameFontHeight;	// Chat user name font height.
	int									m_nChatSystemFontHeight;	// Chat system message font height.
	int									m_nChatMessageFontHeight;	// Chat message font height.
	int									m_nChatDealerFontHeight;	// Chat dealer message font height.
	int									m_nChatEditHeight;

	// Lobby
	int									m_nLobbyHeaderCY;			// Lobby header height.

	// Lobby logo.
	CSize								m_szLogo;					// Logo size
	CPoint								m_ptLogo;					// Logo position.

	// Lobby tab.
	int									m_nLobbyTabCY;				// Lobby tab height.
	int									m_nLobbyTabLabelHeight;		// Lobby tab item label font height.
	CString								m_sLobbyTabLabelFont;		// Lobby tab item label font.
	CFont*								m_pLobbyTabFont;			// Lobby tab item label font.
	COLORREF							m_crLobbyTabItemLabelAct;	// Active Lobby tab item label color.
	COLORREF							m_crLobbyTabItemLabel;		// Lobby tab item label color.

	CPoint								m_ptLobbyGamesTab;			// Lobby games tab position.
	int									m_nLobbyGamesTabCX;			// Lobby games tab width.
	CPoint								m_ptLobbyGameLimitTab;		// Lobby game limits tab position.
	int									m_nLobbyGameLimitsTabCX;	// Lobby game limits tab width.

	// Lobby user info.
	CSize								m_szUserInfo;				// Lobby user info size.
	CRect								m_rcUserNameLabel;			// Lobby user name label rect.
	CRect								m_rcUserBalanceLabel;		// Lobby user balance label rect.
	CRect								m_rcUserBalanceAmount;		// Lobby user balance amount rect.
	CRect								m_rcUserInPlayLabel;		// Lobby user in-play label rect.
	CRect								m_rcUserInPlayAmount;		// Lobby user in-play amount label rect.
	CRect								m_rcUserAvatar;				// Lobby user avatar rect.
	COLORREF							m_crUserInfoText;			// Lobby user info text color.
	CPoint								m_ptUserRankStar;			// Lobby user info rank star position.
	int									m_nUserInfoFontHeight;		// Lobby user info font height.
	CString								m_sUserInfoFont;			// Lobby user info font face name.
	CFont*								m_pLobbyUserInfoFont;		// Lobby userinfo font.

	// Hand history window.
	COLORREF							m_crHistoryTitleLabel;		// Hand history window title color.
	COLORREF							m_crHistoryTitleLabelFill;	// Hand history window title fill color.
	int									m_nHistoryTitleHeight;
	CString								m_sHistoryTitleFont;
	int									m_nHistoryTitleFontHeight;
	int									m_nHistoryTitleFontWeight;
	CFont*								m_pHistoryTitleFont;
	CString								m_sHistoryWindowFont;
	int									m_nHistoryWindowFontHeight;
	int									m_nHistoryWindowFontWeight;
	CFont*								m_pHistoryWindowFont;
	COLORREF							m_crHistoryListText;		// History list text color.		

	// Dialog control.
	CString								m_sDialogFont;				// Dialog font name.
	int									m_nDialogFontHeight;		// Dialog font height.
	int									m_nDialogFontWeight;		// Dialog font weight.
	CFont								*m_pDialogFont;				// Dialog font.
	COLORREF							m_crDialogText;				// Dialog text color.
	COLORREF							m_crDialogFill;				// Dialog fill color.
	CRect								m_rcDialogBkOmitBorder;		// Dialog bk omit border.
	CSize								m_szDialogCloseButton;		// Dialog close buttion size.

	// Sound files.
	CString								m_sSystemMessageSnd;
	CString								m_sAttentionSnd;
	CString								m_sCardsDealtSnd;
	CString								m_sChipsMovedToPotSnd;
	CString								m_sChipsMovedFromPotSnd;
	CString								m_sFoldSnd;
	CString								m_sCheckSnd;
	CString								m_sBetSnd;
	CString								m_sRaiseSnd;
	CString								m_sCallSnd;
	CString								m_sAllInSnd;
	CString								m_sTimeWarningSnd;
	CString								m_sChatMessageSnd;

	GUILanguage*						m_pLang;
};

class GUILaguageItem : public Serializable
{
public:
	GUILaguageItem();
	virtual ~GUILaguageItem();

public:
	CString		m_sKey;
	CString		m_sValue;

protected:
	virtual Serializable* CreateSerializableObject(){return new GUILaguageItem();};
	INIT_RUNTIME_VARIABLE()
};

class GUILanguage : public Serializable
{
public:
	GUILanguage();
	virtual ~GUILanguage();

	CString		GetValue(TCHAR* pszKey);

public:
	EnumerableObject<GUILaguageItem>	m_listItems;
	AutoSortedArray						m_arrKeyArray;	// <Decoded(Upper(Value))><GUILaguageItem*>

protected:
	virtual BOOL			DeserializeAsXML(CString sXmlFile);
	virtual BOOL			DeserializeAsXML(CString* psXmlSource);
	virtual Serializable*	CreateSerializableObject	(){return new GUILanguage();};
	INIT_RUNTIME_VARIABLE()
};
