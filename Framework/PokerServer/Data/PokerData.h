#pragma once
#include "../../Serialize/Serializable.h"
#include "../../Data/DataManager.h" 
#include "../../Array/AutoSortedArray.h"
#include "../../Array/AutoSortedArrayInt64.h"
#include "../Object/UserInfo.h"
#include "../Object/PokerHandHistory.h"

#include "Tables/PokerGameSpeed.h"
#include "Tables/PokerAvatar.h"
#include "Tables/PokerUser.h"

#include "Tables/PokerGameType.h"
#include "Tables/PokerGameSubType.h"
#include "Tables/PokerMoneyType.h"
#include "Tables/PokerHand.h"
#include "Procedures/DataProcedures.h"

class PokerGameController;
class PokerServer;
class PokerGameBase;
class LoginPacket;

class PokerData : public Serializable
{
public:
	PokerData();
	virtual ~PokerData();

	friend class PokerCashGames;
	friend PokerServer;

public:
	bool					LoadTable					(EnumerableObject<Serializable>* pRecordObjects,const _String sWhereClause = _T(""), const _String sOrderBy = _T(""));
	bool					RefreshRecord				(Serializable* pRecord);
	bool					SaveRecord					(Serializable* pRecord);
	bool					DeleteRecord				(Serializable* pRecord);
	bool					ExecuteProc					(Serializable* pProcDesc);

public:
	virtual bool			LoadData					();
	virtual void			FreeData					(bool bCloseDb = false);

	bool					LoadAvatars					();
	void					SetConnString				(_String sConnString) {m_sConnString = sConnString;};
	void					SetAvatarsFolder			(_String sAvatarsFolder){m_sAvatarsFolder = sAvatarsFolder;}

	int						CreateUser					(int nUserId, _String sUserName, _String sPassword, _String sEmail, _String sFirstName, _String sLastName, _String sPhone, int nPlayBalanceChips);
	bool					GetHandHistoryInfo			(__int64 nHandId, PokerHandHistory& handInfo);

	UserInfo*				LoginUser					(const _String sUserName, const _String sPassword, bool& bUserNotFound, bool& bUserBlocked, bool& bWrongPassword);
	UserInfo*				LoginUserByWebSessionId		(const _String sSessionId, bool& bSessionIsAlreadyActive, bool& bUserBlocked);
	bool					LogoutUser					(UserInfo* pUserInfo);
	bool					LogoutWebSessionUser		(int nUserId);

	int						GetUserIdByName				(_String sUserName);
	bool					GetUserNameById				(int nUserId, _String& sUserName);

	bool					LoadUserTournaments			(int nUserId, LoginPacket* p);

	UserInfo*				LoadUserInfo				(int nUserId);
	INT_CHIPS				GetUserBalance				(int nUserId, int nCurrencyId);
	bool					SetUserPlayBalance			(int nUserId, INT_CHIPS nPlayBalance);
	int						SetUserAvatar				(int nUserId, _String &sAvatarFileName, int nAvatarId = -1, bool bIsPrivateAvatar = true);

	bool					TransferMoneyFromMainBalance(int nUserId, INT_CHIPS nAmount, int nCurrencyId, double dMainBalance, int nMainBalanceEntryId, INT_CHIPS& nBalanceNew);
	bool					TransferMoneyToMainBalance	(int nUserId, INT_CHIPS nAmount, int nCurrencyId, double dMainBalance, int nMainBalanceEntryId, INT_CHIPS& nBalanceNew);

	int						GetUserAvatarId				(int nUserId);
	bool					LoadAvatarData				(int nAvatarId, GrowableMemory* pImageData);
	PokerAvatar*			LoadAvatarInfo				(int nAvatarId);
	_String					GetAvatarFileName			(int nAvatarId);

	// Exchange vppoints.
	bool					ExchangeVPPoints			(int nUserId, int nCurrencyId, double dVPPoints, INT_CHIPS& nBalanceNew, double& dVPPointsNew);
	bool					ExchangeVPPointsToGetAmount	(int nUserId, int nCurrencyId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew, double& dVPPointsNew);

protected:
	DataManager*						m_pDataMan;			// Data manager class.
	_String								m_sConnString;		// Database connection string.
	_String								m_sAvatarsFolder;	// Folder of avatar images.

	CCriticalSection					m_lockAttributeTables;
	EnumerableObject<PokerGameType>		m_listGameType;
	EnumerableObject<PokerGameSubType>	m_listGameSubType;
	EnumerableObject<PokerMoneyType>	m_listMoneyType;
	EnumerableObject<PokerGameSpeed>	m_listGameSpeed;

private:
	EnumerableObject<PokerAvatar>		m_listAvatars; // List of default avatar objects.

protected:
	virtual Serializable* CreateSerializableObject(){return new PokerData();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};
