#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerChatMessage 
#include "../../Protocol/_PokerDefines.h"
#include "../../Serialize/Serializable.h"

class PokerChatMessage : public Serializable
{
public:
	PokerChatMessage	();
	~PokerChatMessage	();

public:
	char			m_cSeat;			// Seat index. if -1 then it is dealer.
	_String			m_sUserName;		// User name.
	bool			m_bSystem;			// System message flag.
	char			m_actionType;		// PokerActionTypes
	INT_CHIPS		m_nAmount;			// Action amount.
	_String			m_sMessage;			// Message source.
	DateTime		m_dtTime;			// Message datetime.

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerChatMessage();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};