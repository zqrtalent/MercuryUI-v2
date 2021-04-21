#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerPotAmountDetail

#include "../../Serialize/Serializable.h"

class PokerPotAmountDetail : public Serializable
{
public:
	PokerPotAmountDetail();
	virtual ~PokerPotAmountDetail();

	INT_CHIPS		m_nAmount;			// [OUT] Pot amount (Rake is included).
	UINT			m_wRakeAmount;		// [OUT] Pot rake amount.
	// 0 indicates main pot all others are side pots by index.
	char			m_cPotIndex;		// [OUT] Pot index.
	BYTE			m_cPlayersCt;		// [OUT] Players ct in pot.


public: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new PokerPotAmountDetail(); };
	INIT_RUNTIME_VARIABLE()
};