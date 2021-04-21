#pragma once
#include "../../../Serialize/Serializable.h"
#include "../Tables/TournamentParticipant.h"

// ====================================================
//	How to create procedure class without mistake:
//		
//	Use 'IN' when parameter is input.
//	Use 'OUT' when parameter is output.
//  Use 'RET' when parameter is returnable.
//
//  DBField name attribute is used to type parameter purpose. (Input, Output, Return)
//
// ====================================================

class CreateUserProc : public Serializable
{
public:
	CreateUserProc();
	virtual ~CreateUserProc();

public:
	_String		m_sUserName;	// [IN] User name.
	_String		m_sPassword;	// [IN] User password.
	_String		m_sEMail;		// [IN] User mail address.
	_String		m_sFirstName;	// [IN] User first name.
	_String		m_sLastName;	// [IN] User last name.
	_String		m_sPhone;		// [IN] User phone.
	int			m_nUserId;		// [IN] User id.
	double		m_dPlayAmount;	// [IN] Play money amount.

	int			m_nRetUserId;	// [RET] returned user id.

protected:
	virtual Serializable* CreateSerializableObject(){return new CreateUserProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class AuthenticateUserProc : public Serializable
{
public:
	AuthenticateUserProc();
	virtual ~AuthenticateUserProc();

public:
	_String		m_sUserName;		// [IN] User name.
	_String		m_sPassword;		// [IN] User password.
	int			m_nUserId;			// [RET] Authenticated user id.
	double		m_dBalance;			// [OUT] Balance amount.
	double		m_dPlayBalance;		// [OUT] Play balance amount.
	double		m_dVPPoints;		// [OUT] VPP points.
	bool		m_bIsBlocked;		// [OUT] Blocked status.
	int			m_nAvatarId;		// [OUT] Avatar id.
	int			m_nPermissionId;	// [OUT] Permission id.
	int			m_nUserLevelId;		// [OUT] User level id.
	bool		m_bUserExists;		// [OUT] User name existance.
	bool		m_bChatBlockStatus;	// [OUT] User chat status.
	bool		m_bAvatarStatus;	// [OUT] User avatar status.

protected:
	virtual Serializable* CreateSerializableObject(){return new AuthenticateUserProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class AuthenticateUserBySessionIdProc : public Serializable
{
public:
	AuthenticateUserBySessionIdProc();
	virtual ~AuthenticateUserBySessionIdProc();

public:
	_String		m_sSessionId;					// [IN] Session id.
	int			m_nUserId;						// [RET] Authenticated user id.
	_String		m_sUserName;					// [OUT] User name.
	double		m_dBalance;						// [OUT] Balance amount.
	double		m_dPlayBalance;					// [OUT] Play balance amount.
	double		m_dVPPoints;					// [OUT] VPP points.
	bool		m_bIsBlocked;					// [OUT] Blocked status.
	int			m_nAvatarId;					// [OUT] Avatar id.
	int			m_nPermissionId;				// [OUT] Permission id.
	int			m_nUserLevelId;					// [OUT] User level id.
	bool		m_bSessionIsAlreadyActive;		// [OUT] User session is already active status.
	bool		m_bChatBlockStatus;				// [OUT] User chat status.
	bool		m_bAvatarStatus;				// [OUT] User avatar status.

protected:
	virtual Serializable* CreateSerializableObject(){return new AuthenticateUserBySessionIdProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class DeauthenticateUserSessionProc : public Serializable
{
public:
	DeauthenticateUserSessionProc();
	virtual ~DeauthenticateUserSessionProc();

public:
	int			m_nSuccess;					// [RET] returned integer. 0 = success
	int			m_nUserId;					// [IN] User id.

protected:
	virtual Serializable* CreateSerializableObject(){return new DeauthenticateUserSessionProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class CreateUserSessionProc : public Serializable
{
public:
	CreateUserSessionProc();
	virtual ~CreateUserSessionProc();

public:
	int			m_nSuccess;						// [RET] returned integer. 0 = success
	int			m_nUserId;						// [IN] User id.
	_String		m_sSessionId;					// [OUT] User session id.
	bool		m_bSessionIsAlreadyActive;		// [OUT] User session is already active status.

protected:
	virtual Serializable* CreateSerializableObject(){return new CreateUserSessionProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class StartHandProc : public Serializable
{
public:
	StartHandProc();
	virtual ~StartHandProc();

public:
	__int64		m_nHandId;				// [RET] Hand id.
	int			m_nTableId;				// [IN] Table id.
	int			m_nPlayersCt;			// [IN] Players count.
	int			m_nSeatCt;				// [IN] Authenticated user id.
	double		m_dBigBlindAmount;		// [IN] Big blind amount.
	double		m_dSmallBlindAmount;	// [IN] Small blind amount.
	double		m_dAnteAmount;			// [IN] Ante amount.
	double		m_dRakePercent;		// [IN] Rake percent.

protected:
	virtual Serializable* CreateSerializableObject(){return new StartHandProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class FinishHandProc : public Serializable
{
public:
	FinishHandProc();
	virtual ~FinishHandProc();

public:
	bool			m_bFinishStatus;		// [RET] 
	__int64			m_nHandId;				// [IN] Hand id.
	int				m_nTableId;				// [IN] Table id.
	int				m_nCurrencyId;			// [IN] Currency id.
	_String			m_sPlayersInfo;			// [IN] Players info string. '12,12.34,234.56,789,4,2J4C' UserId,SeatAmount,BetAmount,WonAmount,BetCardsRank,HandCards
	_String			m_sTableCards;			// [IN] Table cards. '2J4CAS'
	LongBinary		m_binHandData;			// [IN] Hand info data binary.
	double			m_dTotalPotAmount;		// [IN] Total pot amount of hand.
	double			m_dRakeAmount;			// [IN] Rake amount of hand.

protected:
	virtual Serializable* CreateSerializableObject(){return new FinishHandProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class FinishHandPlaymoneyProc : public Serializable
{
public:
	FinishHandPlaymoneyProc();
	virtual ~FinishHandPlaymoneyProc();

public:
	bool			m_bFinishStatus;		// [RET] 
	__int64			m_nHandId;				// [IN] Hand id.
	int				m_nTableId;				// [IN] Table id.
	_String			m_sPlayersInfo;			// [IN] Players info string. '12,12.34,234.56,789,4,2J4C' UserId,SeatAmount,BetAmount,WonAmount,BetCardsRank,HandCards
	_String			m_sTableCards;			// [IN] Table cards. '2J4CAS'
	double			m_dTotalPotAmount;		// [IN] Total pot amount of hand.
	double			m_dRakeAmount;			// [IN] Rake amount of hand.

protected:
	virtual Serializable* CreateSerializableObject(){return new FinishHandPlaymoneyProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class SeatUserProc : public Serializable
{
public:
	SeatUserProc();
	virtual ~SeatUserProc();

public:
	int			m_nSuccess;		// [RET] returned integer. 0 = success
	int			m_nUserId;		// [IN] User id.
	int			m_nTableId;		// [IN] Table id
	double		m_dAmount;		// [IN] Amount
	int			m_nCurrencyId;	// [IN] Currency id
	double		m_dBalanceNew;	// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new SeatUserProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class SeatUserPlaymoneyProc : public Serializable
{
public:
	SeatUserPlaymoneyProc();
	virtual ~SeatUserPlaymoneyProc();

public:
	int			m_nSuccess;		// [RET] returned integer. 0 = success
	int			m_nUserId;		// [IN] User id.
	int			m_nTableId;		// [IN] Table id
	double		m_dAmount;		// [IN] Amount
	double		m_dBalanceNew;	// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new SeatUserPlaymoneyProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UnseatUserProc : public Serializable
{
public:
	UnseatUserProc();
	virtual ~UnseatUserProc();

public:
	int			m_nSuccess;		// [RET] returned integer. 0 = success
	int			m_nUserId;		// [IN] User id.
	int			m_nTableId;		// [IN] Table id
	double		m_dAmount;		// [IN] Amount
	int			m_nCurrencyId;	// [IN] Currency id
	double		m_dBalanceNew;	// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new UnseatUserProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UnseatUserPleymoneyProc : public Serializable
{
public:
	UnseatUserPleymoneyProc();
	virtual ~UnseatUserPleymoneyProc();

public:
	int			m_nSuccess;		// [RET] returned integer. 0 = success
	int			m_nUserId;		// [IN] User id.
	int			m_nTableId;		// [IN] Table id
	double		m_dAmount;		// [IN] Amount
	double		m_dBalanceNew;	// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new UnseatUserPleymoneyProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UserAddChipsProc : public Serializable
{
public:
	UserAddChipsProc();
	virtual ~UserAddChipsProc();

public:
	int			m_nSuccess;		// [RET] returned integer. 0 = success
	int			m_nUserId;		// [IN] User id.
	int			m_nTableId;		// [IN] Table id
	double		m_dAmount;		// [IN] Amount
	int			m_nCurrencyId;	// [IN] Currency id
	double		m_dBalanceNew;	// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new UserAddChipsProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UserAddChipsPlaymoneyProc : public Serializable
{
public:
	UserAddChipsPlaymoneyProc();
	virtual ~UserAddChipsPlaymoneyProc();

public:
	int			m_nSuccess;		// [RET] returned integer. 0 = success
	int			m_nUserId;		// [IN] User id.
	int			m_nTableId;		// [IN] Table id
	double		m_dAmount;		// [IN] Amount
	double		m_dBalanceNew;	// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new UserAddChipsPlaymoneyProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TransferFromMainBalanceProc : public Serializable
{
public:
	TransferFromMainBalanceProc();
	virtual ~TransferFromMainBalanceProc();

public:
	int			m_nSuccess;				// [RET] returned integer. 0 = success
	int			m_nUserId;				// [IN] User id.
	double		m_dAmount;				// [IN] Amount
	int			m_nCurrencyId;			// [IN] Currency id
	int			m_nMainBalanceEntryId;	// [IN] Main balance specified  entry id.
	double		m_dMainBalance;			// [IN] Main balance.
	double		m_dBalanceNew;			// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new TransferFromMainBalanceProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TransferToMainBalanceProc : public Serializable
{
public:
	TransferToMainBalanceProc();
	virtual ~TransferToMainBalanceProc();

public:
	int			m_nSuccess;				// [RET] returned integer. 0 = success
	int			m_nUserId;				// [IN] User id.
	double		m_dAmount;				// [IN] Amount
	int			m_nCurrencyId;			// [IN] Currency id
	int			m_nMainBalanceEntryId;	// [IN] Main balance specified  entry id.
	double		m_dMainBalance;			// [IN] Main balance.
	double		m_dBalanceNew;			// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new TransferToMainBalanceProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class GetUserBalanceProc : public Serializable
{
public:
	GetUserBalanceProc();
	virtual ~GetUserBalanceProc();

public:
	int			m_nUserId;				// [IN] User id.
	int			m_nCurrencyId;			// [IN] Currency id
	double		m_dBalance;				// [OUT] User balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new GetUserBalanceProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class SetUserPlayBalanceProc : public Serializable
{
public:
	SetUserPlayBalanceProc();
	virtual ~SetUserPlayBalanceProc();

public:
	int			m_nUserId;				// [IN] User id.
	double		m_dBalance;				// [IN] User balance amount.
	int			m_nSuccess;				// [RET]

protected:
	virtual Serializable* CreateSerializableObject(){return new SetUserPlayBalanceProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class RetrieveMinSeatBuyInProc : public Serializable
{
public:
	RetrieveMinSeatBuyInProc();
	virtual ~RetrieveMinSeatBuyInProc();

public:
	int			m_nSuccess;		// [RET] returned integer. 0 = success
	int			m_nUserId;		// [IN] User id.
	int			m_nTableId;		// [IN] Table id
	int			m_nCurrencyId;	// [IN] Currency id
	double		m_dAmount;		// [OUT] Min amount
	double		m_dBalance;		// [OUT] Balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new RetrieveMinSeatBuyInProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class SetUserAvatarProc : public Serializable
{
public:
	SetUserAvatarProc();
	virtual ~SetUserAvatarProc();

public:
	int			m_nSuccess;			// [RET] returned integer. 0 = success
	int			m_nUserId;			// [IN] User id.
	int			m_nAvatarId;		// [IN] Avatar id
	bool		m_bIsPrivateAvatar;	// [IN] Private avatar.
	_String		m_sAvatarFileName;	// [IN] Avatar image file name.
	int			m_nAvatarIdNew;		// [OUT] Avatar id.

protected:
	virtual Serializable* CreateSerializableObject(){return new SetUserAvatarProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UnseatUsersProc : public Serializable
{
public:
	UnseatUsersProc();
	virtual ~UnseatUsersProc();

public:
	int			m_nSuccess;			// [RET] returned integer. 0 = success

protected:
	virtual Serializable* CreateSerializableObject(){return new UnseatUsersProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class GetTableStatProc : public Serializable
{
public:
	GetTableStatProc();
	virtual ~GetTableStatProc();

public:
	int			m_nUserId;			// [IN] User id.
	int			m_nTableId;			// [IN] Table id
	int			m_nHandsPlayed;		// [OUT] Played hands count.
	int			m_nHandsWon;		// [OUT] Won hands count.
	double		m_dSumBetAmount;	// [OUT] Sum bet amount.
	double		m_dSumWonAmount;	// [OUT] Sum won amount.
	double		m_dSumAmountIn;		// [OUT] Sum amount in.
	double		m_dSumAmountOut;	// [OUT] Sum amount out.

protected:
	virtual Serializable* CreateSerializableObject(){return new GetTableStatProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class CancelTournamentProc_Return : public Serializable
{
public:
	CancelTournamentProc_Return();
	virtual ~CancelTournamentProc_Return();

public:
	int		m_nUserId;			// User identifier.
	double	m_dRefundAmount;	// Refund amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new CancelTournamentProc_Return();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class CancelTournamentProc : public Serializable
{
public:
	CancelTournamentProc();
	virtual ~CancelTournamentProc();

public:
	EnumerableObject<CancelTournamentProc_Return>	m_refundInfo;		// [RET] return refund info list.
	int												m_nTournamentId;	// [IN] Tournament id.

protected:
	virtual Serializable* CreateSerializableObject(){return new CancelTournamentProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class CancelTournamentRefundProc : public Serializable
{
public:
	CancelTournamentRefundProc();
	virtual ~CancelTournamentRefundProc();

public:
	int												m_nRetCode;			// [RET] return code. 0 - success.
	int												m_nTournamentId;	// [IN] Tournament id.
	int												m_nUserId;			// [IN] User id.
	double											m_dRefundAmount;	// [IN] Refund amount.
	double											m_dBalanceNew;		// [OUT] Balance new.

protected:
	virtual Serializable* CreateSerializableObject(){return new CancelTournamentRefundProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class HideTournamentProc : public Serializable
{
public:
	HideTournamentProc();
	virtual ~HideTournamentProc();

public:
	int												m_nRetCode;			// [RET] return code. 0 - success.
	int												m_nTournamentId;	// [IN] Tournament id.

protected:
	virtual Serializable* CreateSerializableObject(){return new HideTournamentProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class RegisterInTournamentProc : public Serializable
{
public:
	RegisterInTournamentProc();
	virtual ~RegisterInTournamentProc();

public:
	EnumerableObject<TournamentParticipant>	m_return;			// [RET] returned result set.
	int										m_nUserId;			// [IN] User id.
	_String									m_sIp;				// [IN] IP address
	int										m_nIP;				// [IN] IP address as string.
	int										m_nTournamentId;	// [IN] Tournament id.
	int										m_nCurrencyId;		// [IN] Currency id.
	bool									m_bUseVPPoints;		// [IN] Use vppoints instead of balance.
	double									m_dBalanceNew;		// [OUT] Balance amount.
	double									m_dVPPointsNew;		// [OUT] VPPoints amount.
	int										m_nParticipantId;	// [OUT] Participant id.
	int										m_nErrorCode;		// [OUT] Error code.

protected:
	virtual Serializable* CreateSerializableObject(){return new RegisterInTournamentProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class RegisterInTournamentByTicketProc : public Serializable
{
public:
	RegisterInTournamentByTicketProc();
	virtual ~RegisterInTournamentByTicketProc();

public:
	EnumerableObject<TournamentParticipant>	m_return;			// [RET] returned result set.
	int										m_nUserId;			// [IN] User id.
	_String									m_sIp;				// [IN] IP address
	int										m_nIP;				// [IN] IP address as string.
	int										m_nTournamentId;	// [IN] Tournament id.
	int										m_nCurrencyId;		// [IN] Currency id.
	double									m_dBalanceNew;		// [OUT] Balance amount.
	int										m_nParticipantId;	// [OUT] Participant id.
	int										m_nErrorCode;		// [OUT] Error code.

protected:
	virtual Serializable* CreateSerializableObject(){return new RegisterInTournamentByTicketProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UnRegisterFromTournamentProc : public Serializable
{
public:
	UnRegisterFromTournamentProc();
	virtual ~UnRegisterFromTournamentProc();

public:
	int			m_nSuccess;			// [RET] returned integer. 0 = success
	int			m_nUserId;			// [IN] User id.
	int			m_nTournamentId;	// [IN] Tournament id.
	double		m_dBalanceNew;		// [OUT] Balance amount.	
	double		m_dVPPointsNew;		// [OUT] VPPoints amount.	
	int			m_nParticipantId;	// [OUT] Participant id.	

protected:
	virtual Serializable* CreateSerializableObject(){return new UnRegisterFromTournamentProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UserRegTournament : public Serializable
{
public:
	UserRegTournament();
	virtual ~UserRegTournament();

public:
	int				m_nParticipantId;		// Reg participant id.
	int				m_nTournamentId;		// Tournament id.
	_String			m_sName;				// Tournament name.

	DateTime		m_dtStartDTime;			// Start date time.
	int				m_nStatus;				// Tournament status
	double			m_dBuyIn;				// Buyin amount.
	double			m_dBuyInFee;			// Buyin fee amount.

	int				m_nGameTypeId;			// Game type id.
	int				m_nSubTypeId;			// Game sub type id.
	int				m_nCurrencyId;			// Currency id.
protected:
	virtual Serializable* CreateSerializableObject(){return new UserRegTournament();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class UserRegTournamentsProc : public Serializable
{
public:
	UserRegTournamentsProc();
	virtual ~UserRegTournamentsProc();

public:
	EnumerableObject<UserRegTournament>		m_listRegTournaments;		// [RET] list registered tournaments.
	int										m_nUserId;					// [IN] User id.

protected:
	virtual Serializable* CreateSerializableObject(){return new UserRegTournamentsProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ConvertVPPointsToMoneyProc : public Serializable
{
public:
	ConvertVPPointsToMoneyProc();
	virtual ~ConvertVPPointsToMoneyProc();

public:
	int			m_nSuccess;			// [RET] returned integer. 0 = success
	int			m_nUserId;			// [IN] User id.
	int			m_nCurrencyId;		// [IN] Tournament id.

	double		m_dVPPointsConvert;	// [IN] VPPoints amount to convert.	
	double		m_dExactAmount;		// [IN] Exact amount to convert 0.0 by default.	
	double		m_dBalanceNew;		// [OUT] Balance amount.	
	double		m_dVPPointsNew;		// [OUT] VPPoints amount.	
	
protected:
	virtual Serializable* CreateSerializableObject(){return new ConvertVPPointsToMoneyProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TournamentRebuyProc : public Serializable
{
public:
	TournamentRebuyProc();
	virtual ~TournamentRebuyProc();

public:
	int										m_nSuccess;					// [RET] Ret code.
	int										m_nUserId;					// [IN] User id.
	int										m_nTournamentId;			// [IN] Tournament id.
	bool									m_bByPoints;				// [IN] By points.
	short									m_shRebuyCtAtOnce;			// [IN] Rebuy count to add at once x2.
	double									m_dBalance;					// [OUT] balance amount.
	double									m_dPoints;					// [OUT] Points amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new TournamentRebuyProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TournamentAddonProc : public Serializable
{
public:
	TournamentAddonProc();
	virtual ~TournamentAddonProc();

public:
	int										m_nSuccess;					// [RET] Ret code.
	int										m_nUserId;					// [IN] User id.
	int										m_nTournamentId;			// [IN] Tournament id.
	bool									m_bByPoints;				// [IN] By points.
	double									m_dBalance;					// [OUT] balance amount.
	double									m_dPoints;					// [OUT] Points amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new TournamentAddonProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class PlayerFinishedTournamentProc : public Serializable
{
public:
	PlayerFinishedTournamentProc();
	virtual ~PlayerFinishedTournamentProc();

public:
	int								m_nSuccess;					// [RET] Ret code.
	int								m_nParticipantId;			// [IN] Participant id.
	int								m_nTournamentId;			// [IN] Tournament id.
	int								m_nUserId;					// [IN] User id.
	int								m_nPlace;					// [IN] User place in tournament.
	int								m_nTournamentTicketIdWon;	// [IN] Tournament ticket id won.
	double							m_dAmountWon;				// [IN] Won amount.
	int								m_nCurrencyId;				// [IN] Currency id.
	double							m_dBalance;					// [OUT] balance amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new PlayerFinishedTournamentProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class StartTournamentHandProc : public Serializable
{
public:
	StartTournamentHandProc();
	virtual ~StartTournamentHandProc();

public:
	__int64		m_nHandId;				// [RET] Hand id.
	int			m_nTournamentId;		// [IN] Tournament id.
	int			m_nTableId;				// [IN] Table id.
	int			m_nLevelIndex;			// [IN] Level index.
	int			m_nPlayersCt;			// [IN] Player count.
	double		m_dBigBlindAmount;		// [IN] Big blind amount.
	double		m_dSmallBlindAmount;	// [IN] Small blind amount.
	double		m_dAnteAmount;			// [IN] Ante amount.

protected:
	virtual Serializable* CreateSerializableObject(){return new StartTournamentHandProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class FinishTournamentHandProc : public Serializable
{
public:
	FinishTournamentHandProc();
	virtual ~FinishTournamentHandProc();

public:
	bool			m_bFinishStatus;		// [RET] 
	__int64			m_nHandId;				// [IN] Hand id.
	int				m_nTableId;				// [IN] Table id.
	_String			m_sPlayersInfo;			// [IN] Players info string. '12,12.34,234.56,789,4,2J4C' UserId,SeatAmount,BetAmount,WonAmount,BetCardsRank,HandCards
	_String			m_sTableCards;			// [IN] Table cards. '2J4CAS'
	LongBinary		m_binHandData;			// [IN] Hand info data binary.
	double			m_dTotalPotAmount;		// [IN] Total pot amount of hand.

protected:
	virtual Serializable* CreateSerializableObject(){return new FinishTournamentHandProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class FinishTournamentProc : public Serializable
{
public:
	FinishTournamentProc();
	virtual ~FinishTournamentProc();

public:
	int				m_nSuccess;			// [RET] Return code.
	int				m_nTournamentId;	// [IN] Tournament id.
	int				m_nPlayersCt;		// [IN] Players count played.
	DateTime		m_dtFinished;		// [IN] Finished datetime.

protected:
	virtual Serializable* CreateSerializableObject(){return new FinishTournamentProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TestTableProc : public Serializable
{
public:
	TestTableProc();
	virtual ~TestTableProc();

public:
	// Data variables. {{
	int				m_nId;
	_String			m_str;
	DateTime		m_date;
	DateTime		m_datetime;
	LongBinary		m_binary;
	double			m_float;
	double			m_real;
	__int64			m_big;
	double			m_dec;
	double			m_num;
	double			m_money;
	// }}

protected:
	virtual Serializable* CreateSerializableObject(){return new TestTableProc();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
