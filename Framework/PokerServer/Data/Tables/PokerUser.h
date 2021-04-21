#pragma once
#include "../../../Serialize/Serializable.h"

class PokerUser : public Serializable
{
public:
	PokerUser();
	virtual ~PokerUser();

public:
	int				m_nId;					// Id.
	_String			m_sUserName;
	_String			m_sPassword;
	int				m_nPermissionId;
	int				m_nAvatarId;
	_String			m_sEmail;
	_String			m_sFirstName;
	_String			m_sLastName;
	_String			m_sPhone;
	int				m_nCurrencyId;
	double			m_dPlayAmount;				// Playmoney amount.
	double			m_dVPPoints;
	short			m_shRankIndex;				// User rank index.
	DateTime		m_dtRegistered;
	bool			m_bIsBlocked;
	DateTime		m_dtAutoUnblockDtime;		// Auto unblock date time.
	bool			m_bIsChatBlocked;
	bool			m_bIsAvatarBlocked;

protected:
	virtual Serializable*						CreateSerializableObject(){return new PokerUser();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerUser>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
