#pragma once
#include "PokerData.h"
#include "Tables/TournamentInfo.h"
#include "Tables/TournamentParticipant.h"
#include "Tables/TournamentPayoutProfile.h"
#include "Tables/TournamentBlindStructureProfile.h"

class PokerTournaments;
class PokerTournamentManager;
class PokerServer;
class PokerGameBase;
class TournamentTableController;
class LobbyInfo;
class TournamentInfoObject;
class PokerCreateTournamentPacket;

class PokerTournamentData : public PokerData
{
public:
	PokerTournamentData();
	virtual ~PokerTournamentData();

	friend class PokerCashGames;
	friend PokerTournaments;
	friend PokerServer;

public:
	virtual bool						LoadData						();
	virtual void						FreeData						(bool bCloseDb = false);

	int									CreatTournament					(PokerCreateTournamentPacket* p);
	PokerTournamentManager*				CreateTournamentManager			(int nTournamentId, PokerServer* pServer, PokerTournaments* pGameBase);
	int									CreateTournamentManagers		(PokerServer* pServer, PokerTournaments* pGameBase, EnumerableObject<PokerTournamentManager>* pListMan);
	int									CreateTournamentGameTables		(int nTournamentId, PokerServer* pServer, PokerGameBase* pGameBase, PokerTournamentManager* pManager, List<TournamentTableController>* pListTables, AutoSortedArray* pArrGameTableById);
	bool								CancelTournament				(int nTournamentId, int nUserId, EnumerableObject<CancelTournamentProc_Return>& refundPlayers);
	// Refund amount of canceled tournament.
	bool								CancelTournamentRefund			(int nTournamentId, int nUserId, double dRefundAmount);
	bool								HideTournament					(int nTournamentId);

	int									GetTournamentInfoObjects		(int nTournamentType, EnumerableObject<TournamentInfoObject>* pListObjects);

	// Payout needs to be changed when tournament registrants count is changing.
	bool								RebuildTournamentPayouts		(int nTournamentId, LobbyInfo* pLobby);

	bool								Rebuy							(int nUserId, int nTournamentId, int nRebuyCtAtOnce, double& dBalance, double& dVPPoints, bool bUseVPPoints = false);
	bool								Addon							(int nUserId, int nTournamentId, double& dBalance, double& dVPPoints, bool bUseVPPoints = false);

	__int64								StartHand						(int nTournamentId, int nTableId, int nLevelIndex, int nPlayersCt, UINT nSB, UINT nBB, UINT nAnte);
	bool								FinishHand						(__int64 nHandId, int nTableId, INT_CHIPS nTotalPotAmount, _String& sTableCards, _String& sPlayersInfo, void* lpHandData, int nSize);

	bool								FinishTournament				(int nTournamentId);

	bool								PlayerFinishedTournament		(int nTournamentId, int nUserId, int& nPlace, double& dAmountWon, bool& bHandForHand, bool& bFinishTournament, int& nTournamentTicketId);

	bool								SetTournamentStatus				(int nTournamentId, TournamentStatus statusNew);
	bool								SetTournamentLevel				(int nTournamentId, int nLevelIndex);

	bool								RegisterUserInTournament		(int nUserId, int nTournamentId, _String sIp, int nIp, INT_CHIPS& nBalanceNew, double& dVPPointsNew, bool bByPoints = false);
	bool								RegisterUserInTournamentByTicket(int nUserId, int nTournamentId, _String sIp, int nIp, INT_CHIPS& nBalanceNew);
	bool								UnregisterUserFromTournament	(int nUserId, int nTournamentId, INT_CHIPS& nBalanceNew, double& dVPPointsNew);
	
protected:
	const TournamentInfo*				TournamentInfoById				(int nTournamentId);
	bool								IsBreakAfterLevel				(int nTournamentId, int nLevelIndex);
	const TournamentBlindStructure*		GetTournamentLevelInfo			(int nTournamentId, int nLevelIndex);

private:
	TournamentPayoutProfile*			GetPayoutProfileById			(int nProfileId);
	TournamentBlindStructureProfile*	GetBlindStructProfileById		(int nProfileId);

protected:
	EnumerableObject<TournamentInfo>					m_listTournaments;
	CCriticalSection									m_lockTournamentObjects;

private:
	EnumerableObject<TournamentPayoutProfile>			m_listPayoutProfiles;
	EnumerableObject<TournamentBlindStructureProfile>	m_listBlindStructProfiles;

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerTournamentData();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
