#pragma once
#include "../../../Serialize/Serializable.h"

class PokerHand : public Serializable
{
public:
	PokerHand();
	virtual ~PokerHand();

public:
	__int64			m_nId;					// Id.
	int				m_nGameTableId;			// GAME_TABLE_ID
	short			m_shPlayersCt;			// PLAYERS_CT
	short			m_shSeatsCt;			// SEATS_CT
	DateTime		m_dtStart;				// START_DTIME
	DateTime		m_dtFinish;				// FINISH_DTIME
	double			m_dBigBlind;			// BIG_BLIND
	double			m_dSmallBlind;			// SMALL_BLIND
	double			m_dAnte;				// ANTE
	double			m_dTotalPot;			// TOTAL_POT
	double			m_dRakeAmount;			// RAKE_AMOUNT
	short			m_dRakePercent;			// RAKE_PERCENT
	LongBinary		m_binHandInfo;			// HANDINFO_BIN

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerHand();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerHand>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
