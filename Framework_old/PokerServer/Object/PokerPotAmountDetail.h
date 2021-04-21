#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerPotAmountDetail

#include "..\..\Serialize\Serializable.h"

class PokerPotAmountDetail : public Serializable
{
public:
	PokerPotAmountDetail();
	virtual ~PokerPotAmountDetail();

	int				m_nAmount;			// [OUT] Pot amount (Rake is included).
	WORD			m_wRakeAmount;		// [OUT] Pot rake amount.
	// 0 indicates main pot all others are side pots by index.
	char			m_cPotIndex;		// [OUT] Pot index.

public: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerPotAmountDetail(); };
	INIT_RUNTIME_VARIABLE()
};