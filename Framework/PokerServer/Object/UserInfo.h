#pragma once

/////////////////////////////////////////////////////////////////////////////
// UserInfo

#include "../../Serialize/Serializable.h"
#include "../Data/Tables/PokerAvatar.h"
#include "../../SafePointer/SafePointer.h"

#include "UserTournamentTicket.h"
#include "UserTournamentDetails.h"

enum UserPermissions{
	Permission_ManageCashTables				= 1,
	Permission_ManageTournaments			= 2,
	Permission_ViewCashTablesStatistic		= 4,
	Permission_ViewTournamentsStatistic		= 8,
	Permission_ManageUsers					= 16,
	Permission_ManageUserPermissions		= 32
	};

class Session;
class UserInfo : public Serializable, public SafePointer
{
public:
	// Access avatar info.
	friend class PokerCashTableController;
	friend class PokerSession;
	friend class PokerData;
	friend class PokerTournamentData;
	friend class TournamentTableController;
	friend class PokerServer;

	UserInfo();
	~UserInfo();
/*
	Session*		GetSession		(bool bRetain);
	void			SetSession		(Session* pSess);
	*/
protected:
	bool			SetAvatarInfo	(PokerAvatar* pAvatarInfo, bool bCopy);
	PokerAvatar*	GetAvatarInfo	(){return m_pAvatarInfo;}
	bool			UpdateAvatarInfo(int nAvatarId, void* lpAvatarData, UINT nDataSize);

public:
	int										m_nUserId;
	int										m_nAvatarId;
	_String									m_sUserName;
	_String									m_sAvatarFileName;

	volatile INT_CHIPS						m_nBalanceAmount;
	volatile INT_CHIPS						m_nInPlayAmount;
	volatile INT_CHIPS						m_nPlayBalance;
	volatile INT_CHIPS						m_nInPlayPlayBalance;
	bool									m_bAuthenticatedBySessionId;	// User authenticated by session.

	volatile double							m_dVPPoints;
	volatile char							m_shRankLevel;
	volatile bool							m_bChatIsBlocked;
	volatile bool							m_bAvatarIsBlocked;

	EnumerableObject<UserTournamentTicket>	m_listTickets;
	EnumerableObject<UserTournamentDetails>	m_listTournamentDetails;

	int										m_nPermissionId;
	bool									m_bBlocked;

private:
	// Session pointer.
	Session*								m_pSession;

protected:
	// Avatar info.
	PokerAvatar*							m_pAvatarInfo;
	bool									m_bDestroyAvatarInfo;

protected:
	virtual Serializable* CreateSerializableObject(){return new UserInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};