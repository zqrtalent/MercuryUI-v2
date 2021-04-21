#include "stdafx.h"
#include "PokerGameTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerGameTable::PokerGameTable(){
	}

BEGIN_DECLARE_VAR(PokerGameTable, Serializable)
	DECLARE_VAR(_T("PokerTable"),			_T("TB_GAME_TABLE"),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),				_T("ID"),					VariableType::VT_Int32,	offsetof(PokerGameTable,m_nTableId), false, false, false)
	DECLARE_VAR(_T("AdminUserId"),			_T("ADMIN_USER_ID"),		VariableType::VT_Int32,	offsetof(PokerGameTable,m_nAdminUserId), false, false, false)
	DECLARE_VAR(_T("Name"),					_T("NAME"),					VariableType::VT_String,	offsetof(PokerGameTable,m_sName), false, false, false)

	DECLARE_VAR(_T("CurrencyId"),			_T("CURRENCY_ID"),			VariableType::VT_Int32,	offsetof(PokerGameTable,m_nCurrencyId), false, false, false)
	DECLARE_VAR(_T("MoneyType"),			_T("MONEY_TYPE_ID"),		VariableType::VT_Int32,	offsetof(PokerGameTable,m_nMoneyType), false, false, false)
	DECLARE_VAR(_T("GameType"),				_T("GAME_TYPE_ID"),			VariableType::VT_Int32,	offsetof(PokerGameTable,m_nGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),			_T("GAME_SUB_TYPE_ID"),		VariableType::VT_Int32,	offsetof(PokerGameTable,m_nGameSubType), false, false, false)
	DECLARE_VAR(_T("GameSpeedId"),			_T("GAME_SPEED_ID"),		VariableType::VT_Int32,	offsetof(PokerGameTable,m_nGameSpeedId), false, false, false)
	DECLARE_VAR(_T("PlayedHandsCt"),		_T("PLAYED_HANDS_COUNT"),	VariableType::VT_Int32,	offsetof(PokerGameTable,m_nPlayedHandsCt), false, false, false)
	
//	DECLARE_VAR(_T("MinBet"),				_T("MIN_BET"),				VariableType::VT_Double,	offsetof(PokerGameTable,m_dMinBet), false, false, false)
//	DECLARE_VAR(_T("MaxBet"),				_T("MAX_BET"),				VariableType::VT_Double,	offsetof(PokerGameTable,m_dMaxBet), false, false, false)
	DECLARE_VAR(_T("MinBuyin"),				_T("MIN_BUYIN"),			VariableType::VT_Double,	offsetof(PokerGameTable,m_dMinBuyin), false, false, false)
	DECLARE_VAR(_T("MaxBuyin"),				_T("MAX_BUYIN"),			VariableType::VT_Double,	offsetof(PokerGameTable,m_dMaxBuyin), false, false, false)
	DECLARE_VAR(_T("SmallBlind"),			_T("SMALLBLIND"),			VariableType::VT_Double,	offsetof(PokerGameTable,m_dSmallBlind), false, false, false)
	DECLARE_VAR(_T("BigBlind"),				_T("BIGBLIND"),				VariableType::VT_Double,	offsetof(PokerGameTable,m_dBigBlind), false, false, false)
	DECLARE_VAR(_T("Ante"),					_T("ANTE"),					VariableType::VT_Double,	offsetof(PokerGameTable,m_dAnte), false, false, false)

	DECLARE_VAR(_T("BBJackpotId"),			_T("BB_JACKPOT_ID"),		VariableType::VT_Int32,	offsetof(PokerGameTable,m_nBBJackpotId), false, false, false)

	DECLARE_VAR(_T("RakePercent"),			_T("RAKE_PERCENT"),			VariableType::VT_Double,	offsetof(PokerGameTable,m_dRakePercent), false, false, false)
	DECLARE_VAR(_T("RakeAmountMax"),		_T("RAKE_AMOUNT_MAX"),		VariableType::VT_Double,	offsetof(PokerGameTable,m_dRakeAmountMax), false, false, false)

	DECLARE_VAR(_T("Offline"),				_T("IS_OFFLINE"),			VariableType::VT_Bool,		offsetof(PokerGameTable,m_bOffline), false, false, false)
	DECLARE_VAR(_T("Private"),				_T("IS_PRIVATE"),			VariableType::VT_Bool,		offsetof(PokerGameTable,m_bPrivate), false, false, false)
	DECLARE_VAR(_T("MaxPlayerCt"),			_T("MAX_PLAYER_CT"),		VariableType::VT_Int16,	offsetof(PokerGameTable,m_shMaxPlayerCt), false, false, false)
	DECLARE_VAR(_T("Pwd"),					_T("PASSWORD"),				VariableType::VT_String,	offsetof(PokerGameTable,m_sPwd), false, false, false)
	DECLARE_VAR(_T("Status"),				_T("STATUS"),				VariableType::VT_Int16,	offsetof(PokerGameTable,m_shStatus), false, false, false)
END_DECLARE_VAR()

PokerGameTable::~PokerGameTable(){
	}

TestTable::TestTable(){
	} 

TestTable::~TestTable(){
	}

BEGIN_DECLARE_VAR(TestTable, Serializable)
	DECLARE_VAR(_T("TestTable"),	_T("TB_TEST"),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Var0"),			_T("Id"),		VariableType::VT_Int32, offsetof(TestTable,m_nId), false, false, false)
	DECLARE_VAR(_T("Var1"),			_T("STR"),		VariableType::VT_String, offsetof(TestTable,m_str), false, false, false)
	DECLARE_VAR(_T("Var2"),			_T("DT"),		VariableType::VT_DateTime, offsetof(TestTable,m_date), false, false, false)
	DECLARE_VAR(_T("Var3"),			_T("DTTIME"),	VariableType::VT_DateTime, offsetof(TestTable,m_datetime), false, false, false)
	DECLARE_VAR(_T("Var5"),			_T("FL"),		VariableType::VT_Double, offsetof(TestTable,m_float), false, false, false)
	DECLARE_VAR(_T("Var6"),			_T("RL"),		VariableType::VT_Double, offsetof(TestTable,m_real), false, false, false)
	DECLARE_VAR(_T("Var7"),			_T("BIG"),		VariableType::VT_Int64, offsetof(TestTable,m_big), false, false, false)
	DECLARE_VAR(_T("Var8"),			_T("DEC"),		VariableType::VT_Double, offsetof(TestTable,m_dec), false, false, false)
	DECLARE_VAR(_T("Var9"),			_T("NUM"),		VariableType::VT_Double, offsetof(TestTable,m_num), false, false, false)
	DECLARE_VAR(_T("Var10"),		_T("MON"),		VariableType::VT_Double, offsetof(TestTable,m_money), false, false, false)
	DECLARE_VAR(_T("Var4"),			_T("BIN"),		VariableType::VT_Binary, offsetof(TestTable,m_binary), false, false, false)
END_DECLARE_VAR()