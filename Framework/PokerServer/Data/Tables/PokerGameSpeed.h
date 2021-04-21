#pragma once
#include "../../../Serialize/Serializable.h"

class PokerGameSpeed : public Serializable
{
public:
	PokerGameSpeed();
	virtual ~PokerGameSpeed();

public:
	int			m_nId;					// Game speed id.
	_String		m_sName;				// Game speed name.
	short		m_shRegularTimeSec;		// regular time seconds. 
	short		m_shExtraTimeSec;		// Extra time seconds.

	short		m_shHandStartDelay;		// Hand start delay.
	short		m_shPostBlindDelay;		// Post blind delay.
	short		m_shStartRoundDelay;	// Start round delay.

	short		m_shShowMuckDelay;		// Show/Muck delay.
	short		m_shReserveSeatDelay;	// Reserve seat delay.

	short		m_shLeaveSitOutedDelay;	// Leave sutouted seat delay.
	short		m_shLeaveZeroBalancedSeatDelay;	// Leave zero balanced seat delay.

	short		m_shTimeBankSec;		// Time bank seconds.
	short		m_shTimeBankIncSec;		// Timebank increase seconds.
	int			m_nTimeBankIncPerHands;	// Increase timebank per hands.

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerGameSpeed();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerGameSpeed>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
