#pragma once
#include "PokerData.h"
#include "Tables/PokerGameTable.h"

class PokerTableController;
class PokerServer;
class PokerGameBase;

class CashGameTableInfoObject;
class CreateCashTableObject;

class PokerCashData : public PokerData
{
public:
	PokerCashData();
	virtual ~PokerCashData();

	friend class PokerCashGames;
	friend PokerServer;

public:
	virtual bool			LoadData					();
	virtual void			FreeData					(bool bCloseDb = false);
	
	int						CreateGameTableControllers	(PokerServer* pServer, PokerGameBase* pGameBase, List<PokerTableController>* pListControllers, AutoSortedArray* pArrControllerById);
	int						CreateGameTable				(PokerServer* pServer, PokerGameBase* pGameBase, CreateGameTableObject* pTableObject, PokerTableController*& ppTableController);

	bool					GetGameTable				(int nTableId, PokerGameTable* pGameTable);

	bool					UnseatUsers					();

	bool					SeatUser					(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew);
	bool					UnseatUser					(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew);

	bool					SeatUserPlaymoney			(int nUserId, int nTableId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew);
	bool					UnseatUserPlaymoney			(int nUserId, int nTableId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew);

	bool					UserAddChips				(int nUserId, int nTableId,  int nSeat, INT_CHIPS nAmount, int nCurrencyId, INT_CHIPS& nBalanceNew);
	bool					UserAddChipsPlaymoney		(int nUserId, int nTableId,  int nSeat, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew);
	bool					GetSeatMinBuyIn				(int nUserId, int nTableId, int nCurrencyId, INT_CHIPS& nAmount, INT_CHIPS& nBalance);

	bool					GetTableStat				(int nUserId, int nTableId, int& nPlayedHandsCt, 
														int& nWonHandsCt, INT_CHIPS& nSumBetAmount,
														INT_CHIPS& nSumWonAmount, INT_CHIPS& nSumAmountIn, INT_CHIPS& nSumAmountOut);

	bool					UpdateGameTableInfo			(PokerTableInfo* pInfo);

	__int64					StartHand					(int nTableId, int nPlayersCt);
	bool					FinishHand					(__int64 nHandId, int nTableId, int nCurrencyId, INT_CHIPS nTotalPotAmount, UINT nRakeAmount, _String& sTableCards, _String& sPlayersInfo, void* lpHandData, int nSize);
	bool					FinishHandPlaymoney			(__int64 nHandId, int nTableId, INT_CHIPS nTotalPotAmount, UINT nRakeAmount, _String& sTableCards, _String& sPlayersInfo);

	int						GetGameTableInfoObjects		(int nGameType, int nGameSubType, EnumerableObject<PokerGameTableInfoObject>* pListObjects);

protected:
	EnumerableObject<PokerGameTable>		m_listGameTable;
	CCriticalSection						m_lockGameTables;

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerCashData();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};
