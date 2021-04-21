#pragma once
#include "..\Serialize\Serializable.h"

class BasePacket : public Serializable
{
public:
	BasePacket();
	virtual ~BasePacket();

public:
	short			m_nCode;			// Packet code.
	__int64			m_nSessionId;		// Session identifier.
	bool			m_bIsRequest;		// Request flag, otherwise response.
	std::string		m_sError;			// Error string.
	short			m_nErrorCode;		// Error code.

public:
	virtual Serializable* CreateSerializableObject(){return new BasePacket();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class HandShakePacket : public BasePacket
{
public:
	HandShakePacket();
	virtual ~HandShakePacket();
public:
	int				m_nProtocolVer;
	int				m_nClientVer;
	bool			m_bPacketCompressionZ;
	std::string		m_sPCName;
	std::string		m_sOS;
	std::string		m_sProcessor;
	
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new HandShakePacket(); };
	INIT_RUNTIME_VARIABLE()
};

class AvatarInfo : public Serializable
{
public:
	AvatarInfo();
	virtual ~AvatarInfo();

public:
	int				m_nAvatarId;		// Avatar Id
	std::string		m_sAvatarName;		// Avatar Name
	std::string		m_sAvatarFileName;	// Avatar file name.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new AvatarInfo(); };
	INIT_RUNTIME_VARIABLE()
};

class OpenedTableInfo : public Serializable
{
public:
	OpenedTableInfo();
	virtual ~OpenedTableInfo();

public:
	int			m_nTableId;			// Table id
	int			m_nTournamentId;	// Tournament id;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new OpenedTableInfo(); };
	INIT_RUNTIME_VARIABLE()
};

class UserTournament : public Serializable
{
public:
	UserTournament();
	virtual ~UserTournament();

public:
	int				m_nTournamentId;		// Tournament id.
	std::string		m_sName;				// Tournament name.
	DateTime		m_dtStart;				// Start time.
	BYTE			m_cGameType;			// Game type.
	short			m_shGameSubType;		// Game sub type.
	int				m_nBuyinAmount;			// Buyin amount.
	int				m_nFeeAmount;			// Fee amount
	BYTE			m_cCurrencyId;			// Currency Id
	BYTE			m_status;				// Tournament status.

public:
	virtual Serializable* CreateSerializableObject(){return new UserTournament();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class LoginPacket : public BasePacket
{
public:
	LoginPacket	();
	virtual ~LoginPacket();

public:
	std::string							m_sUserName;			// [IN] User name.
	std::string							m_sPassword;			// [IN] User password.
	std::string							m_sWebSessionId;		// [IN] Web session id. (Authenticates without username and password)
	int									m_nBalance;				// [OUT] Balance amount.
	int									m_nPlayBalance;			// [OUT] Playmoney amount.
	double								m_dVPPoints;			// [OUT] VP points.
	BYTE								m_btRank;				// [OUT] Rank indicator.
	int									m_nUserId;				// [OUT] User id.
	int									m_nAvatarId;			// [OUT] User's avatar id.
	EnumerableObject<OpenedTableInfo>	m_openedTables;			// [OUT] Opened tables list.
	EnumerableObject<UserTournament>	m_listRegTournaments;	// [OUT] Registered tournament list.
	EnumerableObject<AvatarInfo>		m_listAvatars;			// [OUT] Avatars info list.
	bool								m_bPrevSessionRestored; // [OUT] Previous session restored

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new LoginPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class LogoutPacket : public BasePacket
{
public:
	LogoutPacket();
	virtual ~LogoutPacket();

public:
	__int64								m_nSessionIdNew; // [OUT] New session id.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new LogoutPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class RegisterUserPacket : public BasePacket
{
public:
	RegisterUserPacket();
	virtual ~RegisterUserPacket();

public:
	std::string	m_sUserName;
	std::string	m_sPwd;
	std::string	m_sPwdRepeat;
	std::string	m_sEmailAddress;
	std::string	m_sPhoneNumber;
	std::string	m_sNickName;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new LogoutPacket();};
	INIT_RUNTIME_VARIABLE()
};