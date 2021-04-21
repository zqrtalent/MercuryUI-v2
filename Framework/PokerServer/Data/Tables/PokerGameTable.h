#pragma once
#include "../../../Protocol/_PokerDefines.h"
#include "../../../Serialize/Serializable.h"
#include "PokerGameSpeed.h"

class PokerGameTable : public Serializable
{
public:
	PokerGameTable();
	virtual ~PokerGameTable();

public:
	// Data variables. {{
	int					m_nTableId;					// Table id
	int					m_nAdminUserId;				// Table creator user id.
	_String				m_sName;					// Table name.

	int					m_nCurrencyId;				// Currecny id.
	int					m_nMoneyType;				// Money type.
	int					m_nGameType;				// Game type.
	int					m_nGameSubType;				// Game sub type.
	int					m_nGameSpeedId;				// Game speed id.

	int					m_nPlayedHandsCt;			// Played hand count.

//	double				m_dMinBet;					// Min bet.
//	double				m_dMaxBet;					// Max bet.
	double				m_dMinBuyin;				// Min buy in.
	double				m_dMaxBuyin;				// Max buyin.
	double				m_dSmallBlind;				// Small blind.
	double				m_dBigBlind;				// Big blind
	double				m_dAnte;					// Ante.

	int					m_nBBJackpotId;				// BBJackpot id.

	double				m_dRakePercent;				// Rake percent.
	double				m_dRakeAmountMax;			// Rake amount max.

	bool				m_bOffline;					// Is offline table
	bool				m_bPrivate;					// Is private table.
	_String				m_sPwd;						// Password hash for table.
	short				m_shMaxPlayerCt;			// Max players count.
	short				m_shStatus;					// Status.
	// }}

	PokerGameSpeed		m_speed;					// Speed parameters.

public:
	virtual Serializable* CreateSerializableObject(){return new PokerGameTable();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<PokerGameTable>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class TestTable : public Serializable
{
public:
	TestTable();
	virtual ~TestTable();

public:
	// Data variables. {{
	int				m_nId;
	_String			m_str;
	DateTime		m_date;
	DateTime		m_datetime;
	LongBinary		m_binary;
	double			m_float;
	double			m_real;
	__int64			m_big;
	double			m_dec;
	double			m_num;
	double			m_money;
	// }}

protected:
	virtual Serializable* CreateObject(){return new TestTable();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};
