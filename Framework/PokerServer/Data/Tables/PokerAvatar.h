#pragma once
#include "..\..\..\Serialize\Serializable.h"

class PokerAvatar : public Serializable
{
public:
	PokerAvatar();
	virtual ~PokerAvatar();

public:
	int			m_nId;					// Id.
	int			m_nPrivateUserId;		// Avatar owner user id. When 0 it is shared to everyone.
	_String		m_sAvatarFileName;		// Avatar file name.
	LongBinary	m_binImage;				// Binary data of image.
	short		m_recStatus;			// Record status.

protected:
	virtual Serializable*						CreateSerializableObject(){return new PokerAvatar();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerAvatar>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
