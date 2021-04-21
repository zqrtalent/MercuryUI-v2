#pragma once
#include "BasePacket.h"
#include "_PokerDefines.h"

class PokerBlockUserPacket : public BasePacket
{
public:
	PokerBlockUserPacket();
	virtual ~PokerBlockUserPacket();
public:
	int				m_nUserId;	// [IN] User id
	bool			m_bBlock;	// [IN] Block/Unblock flag.

	virtual Serializable* CreateSerializableObject(){return new PokerBlockUserPacket();};
protected: // Serialization.
	INIT_RUNTIME_VARIABLE()
};

class PokerGameTableInfoObject : public Serializable
{
public:
	PokerGameTableInfoObject();
	virtual ~PokerGameTableInfoObject();

public:
	int				m_nId;
	std::string		m_sName;
	int				m_nGameType;
	int				m_nGameSubType;
	int				m_nCurrencyId;
	float			m_fSmallBlind;
	float			m_fBigBlind;
	float			m_fAnte;
	float			m_fRakePercent;
	float			m_fMaxRakeAmount;
	float			m_fMinBuyin;
	float			m_fMaxBuyin;
	float			m_fMinBet;
	float			m_fMaxBet;
	BYTE			m_cMaxSeatCt;
	std::string		m_sPassword;
	BYTE			m_cPlayersCt;
	BYTE			m_cWaitingPlayersCt;
	float			m_fAvgPotAmount;
	short			m_shHandPerHour;
	bool			m_bActiveStatus;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerGameTableInfoObject();};
	INIT_RUNTIME_VARIABLE()
};

class PokerGetGameTablesPacket : public BasePacket
{
public:
	PokerGetGameTablesPacket();
	virtual ~PokerGetGameTablesPacket();

public:
	BYTE											m_cGameType;		// [IN] Game type.
	short											m_shSubType;		// [IN]	Game sub type
	EnumerableObject<PokerGameTableInfoObject>		m_listResult;		// [OUT] Result list of cash game table info.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerGetGameTablesPacket();};
	INIT_RUNTIME_VARIABLE()
};

class TournamentInfoObject : public Serializable
{
public:
	TournamentInfoObject();
	virtual ~TournamentInfoObject();

public:
	int					m_nId;
	std::string			m_sName;
	std::string			m_sDescription;
	int					m_nTournamentType;
	BYTE				m_cGameType;
	short				m_nGameSubType;
	BYTE				m_cStatus;
	int					m_nRegisteredCt;
	int					m_nPlayersCt;
	int					m_nTablesCt;
	DateTime			m_dtStartTime;
	DateTime			m_dtRegStartTime;
	DateTime			m_dtRegEndTime;
	DateTime			m_dtFinishedTime;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new TournamentInfoObject();};
	INIT_RUNTIME_VARIABLE()
};

class PokerGetTournamentsPacket : public BasePacket
{
public:
	PokerGetTournamentsPacket();
	virtual ~PokerGetTournamentsPacket();

public:
	int												m_nTournamentType;				// [IN] Tournament type.
	EnumerableObject<TournamentInfoObject>			m_listResult;					// [OUT] Result list of tournament info.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerGetTournamentsPacket();};
	INIT_RUNTIME_VARIABLE()
};

class ClientConnectionInfoObject : public Serializable
{
public:
	ClientConnectionInfoObject();
	virtual ~ClientConnectionInfoObject();

public:
	int					m_nClientIP;
	int					m_nClientPort;
	int					m_nServerPort;
	__int64				m_nSessionId;
	int					m_nUserId;
	std::string			m_sUserName;
	DateTime			m_dtTimeConnected;
	int					m_nReceivedBytesCt;
	int					m_nSentBytesCt;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new ClientConnectionInfoObject();};
	INIT_RUNTIME_VARIABLE()
};

class PokerGetClientConnectionsPacket : public BasePacket
{
public:
	PokerGetClientConnectionsPacket();
	virtual ~PokerGetClientConnectionsPacket();

public:
	int													m_nMaxConnectionCt;				// [IN] Maximum connection object count to retrieve.
	EnumerableObject<ClientConnectionInfoObject>		m_listResult;					// [OUT] Result list of client connection info objects.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerGetClientConnectionsPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSendAdminMessageByTablePacket : public BasePacket
{
public:
	PokerSendAdminMessageByTablePacket();
	virtual ~PokerSendAdminMessageByTablePacket();

public:
	int								m_nTableId;						// [IN] Table id.
	int								m_nTournamentId;				// [IN] Tournament id.
	std::string						m_sMessage;						// [IN] Message text.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSendAdminMessageByTablePacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerSendAdminMessageByUserPacket : public BasePacket
{
public:
	PokerSendAdminMessageByUserPacket();
	virtual ~PokerSendAdminMessageByUserPacket();

public:
	int								m_nUserId;						// [IN] User id.
	__int64							m_nSessionId;					// [IN] Session id.
	std::string						m_sMessage;						// [IN] Message text.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerSendAdminMessageByUserPacket();};
	INIT_RUNTIME_VARIABLE()
};

class CreateGameTableObject : public Serializable
{
public:
	CreateGameTableObject();
	virtual ~CreateGameTableObject();

public:
	BYTE				m_cGameType;
	short				m_shGameSubType;
	int					m_nGameSpeedId;
	int					m_nCurrencyId;

	std::string			m_sName;
	float				m_fSmallBlind;
	float				m_fBigBlind;
	float				m_fAnte;
	float				m_fRakePercent;
	float				m_fMaxRakeAmount;

	float				m_fMinBuyin;
	float				m_fMaxBuyin;
	float				m_fMinBet;
	float				m_fMaxBet;

	BYTE				m_cMaxSeatCt;
	std::string			m_sPassword;
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new CreateGameTableObject();};
	INIT_RUNTIME_VARIABLE()
};

class PokerCreateGameTablePacket : public BasePacket
{
public:
	PokerCreateGameTablePacket();
	virtual ~PokerCreateGameTablePacket();

public:
	int											m_nTableId;						// [OUT] Created game table's id.
	EnumerableObject<CreateGameTableObject>		m_tableInfo;					// [IN] table parameters.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerCreateGameTablePacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerForceClientSessionToQuitPacket : public BasePacket
{
public:
	PokerForceClientSessionToQuitPacket();
	virtual ~PokerForceClientSessionToQuitPacket();

public:
	__int64										m_nSessionId;					// [IN] Client session identifier.
	bool										m_bQuitAll;						// [IN] Quit all client sessions.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerForceClientSessionToQuitPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerDisableServerConnectPacket : public BasePacket
{
public:
	PokerDisableServerConnectPacket();
	virtual ~PokerDisableServerConnectPacket();

public:
	bool												m_bDisable;					// [IN] Disable/Enable flag.
	std::string											m_sMessageDescription;		// [IN] Message description send to connected client and then drop connection.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerDisableServerConnectPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerDeactivateGameTablePacket : public BasePacket
{
public:
	PokerDeactivateGameTablePacket();
	virtual ~PokerDeactivateGameTablePacket();

public:
	int													m_nTableId;					// [IN] Table id.
	bool												m_bDeactivate;				// [IN] Deactivate/Activate flag.
	bool												m_bAllTables;				// [IN] All tables flag.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerDeactivateGameTablePacket();};
	INIT_RUNTIME_VARIABLE()
};


//=====================================================================================================================
//
//	Poker tournament packets
//
//=====================================================================================================================

class PokerCreateTournamentPacket : public BasePacket
{
public:
	PokerCreateTournamentPacket();
	virtual ~PokerCreateTournamentPacket();

public:
	int						m_nTournamentId;			// [OUT] Tournament id.
	std::string				m_sResponseLog;				// [OUT] Response log string.
	int						m_nGameType;				// [IN] One of the GameType.
	int						m_nGameSubType;				// [IN] One of the GameSubType. (it includes also tournament type)
	int						m_nGameSpeedId;				// [IN] Game speed id.		

	std::string				m_sName;					// [IN] Tournament name.
	std::string				m_sDescription;				// [IN] Tournament description.

	DateTime				m_dtTimeStart;				// [IN] Tournament start datetime.
	DateTime				m_dtTimeShow;				// [IN] Tournament show datetime.
	DateTime				m_dtTimeRegStart;			// [IN] Tournament registration start datetime.
	DateTime				m_dtTimeRegEnd;				// [IN] Tournament registration end datetime.

	int						m_nMinPlayers;				// [IN] Min players count.
	int						m_nMaxPlayers;				// [IN] Max players count.

	short					m_shActionTimeSec;			// [IN] Regular action time sec.
	short					m_shTBInitialSec;			// [IN] Time bank initial seconds.
	short					m_shTBIncLevel;				// [IN] Time bank increase per level.
	short					m_shTBIncSec;				// [IN] Time bank increase seconds.
	short					m_shMaxSeatCt;				// [IN] Table seat count.
	short					m_shLevelDurationMin;		// [IN] Level duration in minutes.

	int						m_nBuyinAmount;				// [IN]	Buyin amount.
	int						m_nBuyinVPP;				// [IN]	Buyin VPP amount.
	int						m_nBuyinFeeAmount;			// [IN] Buyin fee amount.
	int						m_nBuyinChips;				// [IN] Buyin chips.

	short					m_shBreakPerLevel;			// [IN] Break per level.
	short					m_shBreakDurationMin;		// [IN] Break duration in minutes.

	int						m_nRebuyAmount;				// [IN]	Rebuy amount.
	int						m_nRebuyVPP;				// [IN]	Rebuy VPP amount.
	int						m_nRebuyFeeAmount;			// [IN] Rebuy fee amount.
	int						m_nRebuyChips;				// [IN] Rebuy chips.

	int						m_nAddonAmount;				// [IN]	Addon amount.
	int						m_nAddonVPP;				// [IN]	Addon VPP amount.
	int						m_nAddonFeeAmount;			// [IN] Addon fee amount.
	int						m_nAddonChips;				// [IN] Addon chips.
	short					m_shAddonBreak;				// [IN] Addon break level number.
	double					m_dPrizeAmount;				// [IN] Prize amount guaranteed.
	int						m_nPrizeTicketId;			// [IN] Prize ticket id. (tournament id)

	int						m_nBlindStructProfileId;	// [IN] Blind structure profile id.
	int						m_nPayoutProfileId;			// [IN] Payout profile id.
	BYTE					m_btRebuyCt;				// [IN] Rebuy count.
	BYTE					m_btAddonCt;				// [IN] Addon count.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerCreateTournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerCancelTournamentPacket : public BasePacket
{
public:
	PokerCancelTournamentPacket();
	virtual ~PokerCancelTournamentPacket();

public:
	int						m_nTournamentId;	// [IN] Tournament id.
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerCancelTournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerHideShowTournamentPacket : public BasePacket
{
public:
	PokerHideShowTournamentPacket();
	virtual ~PokerHideShowTournamentPacket();

public:
	int						m_nTournamentId;	// [IN] Tournament id.
	bool					m_bHide;			// [IN] Hide/Show flag.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerHideShowTournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};

class PokerRegisterUserInTournamentPacket : public BasePacket
{
public:
	PokerRegisterUserInTournamentPacket();
	virtual ~PokerRegisterUserInTournamentPacket();

public:
	int						m_nTournamentId;	// [IN] Tournament id.
	bool					m_bRegister;		// [IN] Register/Unregister flag.
	int						m_nUserId;			// [IN] User id to register/unregister.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerRegisterUserInTournamentPacket();};
	INIT_RUNTIME_VARIABLE()
};