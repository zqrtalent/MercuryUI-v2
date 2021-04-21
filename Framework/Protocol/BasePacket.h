#pragma once
#include "../Serialize/Serializable.h"

class GrowableMemory;
class BasePacket : public Serializable
{
public:
	BasePacket();
	virtual ~BasePacket();

	void ZeroPacketVariables();

public:
	short			m_nCode;			// Packet code.
	int				m_nPacketSize;		// Packet size in bytes.
	__int64			m_nSessionId;		// Session identifier.
//	bool			m_bIsRequest;		// Request flag, otherwise response.
	_String			m_sError;			// Error string.
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
	_String			m_sPCName;
	_String			m_sOS;
	_String			m_sProcessor;
	
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new HandShakePacket(); };
	INIT_RUNTIME_VARIABLE()
};

class AcquireTableAccessPacket : public BasePacket
{
public:
	AcquireTableAccessPacket();
	virtual ~AcquireTableAccessPacket();

public:
	__int64			m_nSessionIdAcquire;	// [IN] Session id which table you want to gain access.
	int				m_nTableId;				// [IN] Table id.
	int				m_nTournamentId;		// [IN] Tournament id.
	int				m_nUserId;				// [OUT] Acquired session user id.
	bool			m_bChatIsBlocked;		// [OUT] Chat status for user.
	bool			m_bAvatarIsBlocked;		// [OUT] Avatar status for user.
	
protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new AcquireTableAccessPacket(); };
	INIT_RUNTIME_VARIABLE()
};

class AvatarInfo : public Serializable
{
public:
	AvatarInfo();
	virtual ~AvatarInfo();

public:
	int				m_nAvatarId;		// Avatar Id
	_String			m_sAvatarName;		// Avatar Name
	_String			m_sAvatarFileName;	// Avatar file name.

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
	_String			m_sName;				// Tournament name.
	DateTime		m_dtStart;				// Start time.
	BYTE			m_cGameType;			// Game type.
	short			m_shGameSubType;		// Game sub type.
	UINT			m_nBuyinAmount;			// Buyin amount.
	UINT			m_nFeeAmount;			// Fee amount
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
	_String								m_sUserName;			// [IN] User name.
	_String								m_sPassword;			// [IN] User password.
	_String								m_sWebSessionId;		// [IN] Web session id. (Authenticates without username and password)
	INT_CHIPS							m_nBalance;				// [OUT] Balance amount.
	INT_CHIPS							m_nPlayBalance;			// [OUT] Playmoney amount.
	double								m_dVPPoints;			// [OUT] VP points.
	BYTE								m_btRank;				// [OUT] Rank indicator.
	int									m_nUserId;				// [OUT] User id.
	int									m_nAvatarId;			// [OUT] User's avatar id.
	EnumerableObject<OpenedTableInfo>	m_openedTables;			// [OUT] Opened tables list.
	EnumerableObject<UserTournament>	m_listRegTournaments;	// [OUT] Registered tournament list.
	EnumerableObject<AvatarInfo>		m_listAvatars;			// [OUT] Avatars info list.
	bool								m_bPrevSessionRestored; // [OUT] Previous session restored
	bool								m_bChatIsBlocked;		// [OUT] Chat status for user.
	bool								m_bAvatarIsBlocked;		// [OUT] Avatar status for user.

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
	_String	m_sUserName;
	_String	m_sPwd;
	_String	m_sPwdRepeat;
	_String	m_sEmailAddress;
	_String	m_sPhoneNumber;
	_String	m_sNickName;

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new LogoutPacket();};
	INIT_RUNTIME_VARIABLE()
};