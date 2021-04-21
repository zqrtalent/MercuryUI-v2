#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerTableSeatInfo window
#include "../../Protocol/_PokerDefines.h"
#include "../../Serialize/Serializable.h"

// Seat flags.
#define FLAG_SEAT_IS_OFFLINE		1
#define FLAG_SEAT_AUTOPOSTBLIND		2
#define FLAG_SEAT_AUTOMUCK			4
#define FLAG_SEAT_SITOUTNEXTHAND	8
#define FLAG_SEAT_SITOUTNEXTBB		16
#define FLAG_SEAT_ISTIMERACTIVE		32
#define FLAG_SEAT_AUTOREBUY			64
#define FLAG_SEAT_AUTOADDON			128

class Session;
class ClientConnection;
class PokerTableSeatInfo : public Serializable
{
public:
	PokerTableSeatInfo();
	~PokerTableSeatInfo();

	friend class PokerTableInfo;

	void				SetUserName			(std::string sUserName){m_sUserName = sUserName;};
	std::string			GetUserName			(){return m_sUserName;};
	void				SetUserId			(int nUserId){m_nUserId = nUserId;};
	int					GetUserId			(){return m_nUserId;};

	void				SetAvatarId			(int nAvatarId){m_nAvatarId = nAvatarId;};
	int					GetAvatarId			(){return m_nAvatarId;};

	void				SetBalance			(int nBalance){m_nBalance = nBalance;};
	int					GetBalance			(){return m_nBalance;};

	void				SetHandWonState		(bool bWon){m_bWonState = bWon;};

	void				SetAddedAmount		(int nAmount){m_nAddedAmount = nAmount;};
	int					GetAddedAmount		(){return m_nAddedAmount;};
	void				Add_AddedAmount		(int nAmount){m_nAddedAmount += nAmount;};

	void				SetPotAmount		(int nPotAmount){m_nPotAmount = nPotAmount;};
	int					GetPotAmount		(){return m_nPotAmount;};

	bool				SetAvatarData		(LongBinary* pBinImage);

	// Total pot amount of current seat.
	void				SetTotalPot			(int nTotalPot){m_nPotAmountTotal = nTotalPot;};
	void				AddTotalPot			(int nTotalPot){m_nPotAmountTotal += nTotalPot;};
	int					GetTotalPot			(){return m_nPotAmountTotal;};

	void				SetStatus			(PokerSeatStatus status){m_seatStatus = status;};
	PokerSeatStatus		GetStatus			(){return m_seatStatus;};

	int					GetSeat				(){return (int)m_cSeat;};
	void				SetSeat				(int nSeat){m_cSeat = (char)nSeat;};

	void				SetCards			(int nCard1, int nCard2, int nCard3 = -1, int nCard4 = -1)
											{m_cCard1 = (char)nCard1; m_cCard2 = (char)nCard2; m_cCard3 = (char)nCard3; m_cCard4 = (char)nCard4;};
	int					GetCard1			(){return (int)m_cCard1;};
	int					GetCard2			(){return (int)m_cCard2;};
	int					GetCard3			(){return (int)m_cCard3;};
	int					GetCard4			(){return (int)m_cCard4;};

	void				SetCard1			(int nCard){m_cCard1 = (int)nCard;};
	void				SetCard2			(int nCard){m_cCard2 = (int)nCard;};
	void				SetCard3			(int nCard){m_cCard3 = (int)nCard;};
	void				SetCard4			(int nCard){m_cCard4 = (int)nCard;};

	int					GetRankLevel		(){return m_cRankLevel;};
	void				SetRankLevel		(int nRankLevel){m_cRankLevel = nRankLevel;};

	std::string			GetActionName		(){return m_sAction;};
	void				SetActionName		(std::string sAction){m_sAction = sAction;};

	void				SetupTimer			(int nFullTimeSec, int nSecPassed);
	void				ActivateTimer		(bool bActivate){SetSeatFlag(FLAG_SEAT_ISTIMERACTIVE, bActivate);};
	bool				IsTimerActive		() {return ((m_cSeatFlags&FLAG_SEAT_ISTIMERACTIVE) == FLAG_SEAT_ISTIMERACTIVE);};
	int					GetAllSeconds		(){return (int)m_cTimerSeconds;};
	int					GetPassedSeconds	(){return (int)m_cSecondsPassed;};
	bool				IncreaseTimer		();

	Session*			GetSession			(bool bRetain);
	void				SetSession			(Session* pSes);
	ClientConnection*	GetConnection		();

	bool				GetAutoPostBlind	(){return ((m_cSeatFlags&FLAG_SEAT_AUTOPOSTBLIND) == FLAG_SEAT_AUTOPOSTBLIND);};
	void				SetAutoPostBlind	(bool bAutoPostBlind){SetSeatFlag(FLAG_SEAT_AUTOPOSTBLIND, bAutoPostBlind);};

	bool				GetSitOutNextHand	(){return ((m_cSeatFlags&FLAG_SEAT_SITOUTNEXTHAND) == FLAG_SEAT_SITOUTNEXTHAND);};
	void				SetSitOutNextHand	(bool bSitOutNextHand){SetSeatFlag(FLAG_SEAT_SITOUTNEXTHAND, bSitOutNextHand);};

	bool				GetSitOutNextBB		(){return ((m_cSeatFlags&FLAG_SEAT_SITOUTNEXTBB) == FLAG_SEAT_SITOUTNEXTBB);};
	void				SetSitOutNextBB		(bool bSitOutNextBB){SetSeatFlag(FLAG_SEAT_SITOUTNEXTBB, bSitOutNextBB);};

	bool				GetAutoMuck			(){return ((m_cSeatFlags&FLAG_SEAT_AUTOMUCK) == FLAG_SEAT_AUTOMUCK);};
	void				SetAutoMuck			(bool bAutoMuck){SetSeatFlag(FLAG_SEAT_AUTOMUCK, bAutoMuck);};

	bool				GetAutoRebuy		(){return ((m_cSeatFlags&FLAG_SEAT_AUTOREBUY) == FLAG_SEAT_AUTOREBUY);};
	void				SetAutoRebuy		(bool bAutoRebuy){SetSeatFlag(FLAG_SEAT_AUTOREBUY, bAutoRebuy);};

	bool				GetAutoAddon		(){return ((m_cSeatFlags&FLAG_SEAT_AUTOADDON) == FLAG_SEAT_AUTOADDON);};
	void				SetAutoAddon		(bool bAutoAddon){SetSeatFlag(FLAG_SEAT_AUTOADDON, bAutoAddon);};

	PokerActionTypes	GetLastAction		(){return m_lastAction;};
	void				SetLastAction		(PokerActionTypes action){m_lastAction = action;};

	bool				IsWaitingForBB		(){return m_bWaitingBB;};
	void				SetWaitingBB		(bool bWaitingBB) {m_bWaitingBB = bWaitingBB;};

	bool				IsOffline			(){return ((m_cSeatFlags&FLAG_SEAT_IS_OFFLINE) == FLAG_SEAT_IS_OFFLINE);};
	void				SetOffline			(bool bOffline){SetSeatFlag(FLAG_SEAT_IS_OFFLINE, bOffline);};

	void				SetSeatFlag			(BYTE cFlag, bool bOn){
		if( bOn )
			m_cSeatFlags |= cFlag;
		else
			m_cSeatFlags &= ~cFlag;
		}

	void				FalsePreActions		(){
						m_bPreAction_Fold			= false;	
						m_bPreAction_CheckFold		= false;	
						m_bPreAction_Check			= false;	
						m_bPreAction_Call			= false;	
						m_bPreAction_CallAny		= false;	
						m_bPreAction_CallAnyCheck	= false;	
						m_nPreAction_CallAmount		= 0;
		};

	int					GetTimeBankSeconds	(){return m_cTimeBankSec;};
	void				SetTimeBankSeconds	(int nTimeBankSec) {m_cTimeBankSec = (char)nTimeBankSec;};
	float				GetCollectedTimeBank(){return m_fTimeBankSec;};
	void				SetCollectedTimeBank(float fTimeBankSec){m_fTimeBankSec = fTimeBankSec;}

	int					GetHandCardsClass	(){return (int)m_cHandCardsClass;};
	void				SetHandCardsClass	(int nHandCardsClass){m_cHandCardsClass = (char)nHandCardsClass;}

	DWORD				GetIpAddress		(){ return m_dwIpAddress;}
	void				SetIpAddress		(DWORD dwIpAddress){m_dwIpAddress = dwIpAddress;}

	bool				GetCardsShowStatus	(){return m_bCardsShown;}
	void				SetCardsShowStatus	(bool bStatus){m_bCardsShown = bStatus;}

	// Statistic
	int					GetHandsDealed()	{return m_nHandsDealed;};
	int					GetHandsPlayed()	{return m_nHandsPlayed;};
	int					GetHandsWon()		{return m_nHandsWon;};
	int					GetHandsPreflop()	{return m_nHandsWonPreflop;};
	int					GetHandsPostflop()	{return m_nHandsWonPostflop;};
	int					GetHandsShowdown()	{return m_nHandsWonShowdown;};
protected:
	Session*			m_pSes;				// Session of the client connection.
	std::string			m_sUserName;		// User name.
	int					m_nUserId;			// User id.
	int					m_nAvatarId;		// User avatar id.
	char				m_cSeatFlags;		// Seat flags.
//	bool				m_bOffline;			// Offline state.
	char				m_cSeat;			// Seat index.
	int					m_nBalance;			// Balance amount (without pot amount).
	int					m_nAddedAmount;		// Amount added from user's balance.
	int					m_nPotAmount;		// Current round pot amount.
	int					m_nPotAmountTotal;	// Total pot amount.
	char				m_cCard1;			// Hand card 1.
	char				m_cCard2;			// Hand card 2.
	char				m_cCard3;			// Hand card 3.
	char				m_cCard4;			// Hand card 4.
	std::string				m_sRankName;		// Rank name
	char				m_cRankLevel;		// Rank level value. [0...6]
	std::string				m_sAction;			// Last action name.
	PokerActionTypes	m_lastAction;		// Last action. 
//	bool				m_bTimerActive;		// Timer progress active status.
	char				m_cTimerSeconds;	// Full timer in seconds
	char				m_cSecondsPassed;	// Seconds passed through.
	char				m_cTimeBankSec;		// Time bank seconds.
//	MipmapImage			*m_pAvatarImage;	// Avatar image.
	PokerSeatStatus		m_seatStatus;		// Seat status. 
//	bool				m_bAutoPostBlind;	// Auto post blind.
//	bool				m_bAutoMuck;		// Auto muck.
//	bool				m_bSitOutNextHand;	// Sit out next hand.
//	bool				m_bSitOutNextBB;	// Sit out next big blind.
	bool				m_bWaitingBB;		// Waiting for BB.
	char				m_cHandCardsClass;	// Hand cards class.

	DWORD				m_dwIpAddress;		// Ip Address of connection.
	float				m_fTimeBankSec;		// Collected time bank milli seconds.
	bool				m_bCardsShown;		// Cards shown status.
	bool				m_bWonState;		// Hand won state.

	// Statistic info.
	int					m_nHandsDealed;			// Hands count dealed.
	int					m_nHandsPlayed;			// Hands count played.
	int					m_nHandsWon;			// Hands count won.
	int					m_nHandsWonPreflop;		// Hands count won preflop.
	int					m_nHandsWonPostflop;	// Hands count won postflop.
	int					m_nHandsWonShowdown;	// Hands count won show down.

#ifdef _SERVER
	int					m_nUserPlaceIndex;		// User place index of tournament.
public:
	void	SetUserPlaceIndex(int nPlaceIndex){ m_nUserPlaceIndex = nPlaceIndex; };
	int		GetUserPlaceIndex(){ return m_nUserPlaceIndex; };
#endif

public:
	// PreActions
	bool				m_bPreAction_Fold;	
	bool				m_bPreAction_CheckFold;	
	bool				m_bPreAction_Check;	
	bool				m_bPreAction_Call;	
	bool				m_bPreAction_CallAny;	
	bool				m_bPreAction_CallAnyCheck;	
	int					m_nPreAction_CallAmount;

public:
	virtual bool			Copy		(Serializable* pInto);
protected:
	virtual BOOL			Serialize	(GrowableMemory* pMem);
public:
	virtual Serializable*	CreateSerializableObject(){return new PokerTableSeatInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};