#pragma once
#include "../Array/StringToPtrArray.h"
#include "../Array/AutoSortedArray.h"
#include "../Array/AutoSortedArrayInt64.h"
#include "../Utility/GrowableMemory.h"
#include "../GameServer/Session.h"
#include "Object/UserInfo.h"

class ClientConnection;
class GameServer;

class PokerSession : public Session
{
public:
	PokerSession(GameServer* pServer, ClientConnection* pConn = NULL);
	virtual ~PokerSession();

	friend class PokerServer;
	friend class PokerTournaments;

protected:
	static PokerSession*	GetSessionByUserId			(int nUserId, bool bRetain);
	static bool				GetUserInfoById				(int nUserId, UserInfo* pUserInfoCopy);

	static bool				AddUserSession				(int nUserId, PokerSession* pSession);
	static int				RemoveAllUserSession		();
	static bool				RemoveUserSession			(int nUserId);

public:
	UserInfo*				GetUserInfo					();
	void					SetUserInfo					(UserInfo* pUserInfo);
	void					SetAuthorized				(bool bAuthorized);

	bool					CopyUserInfo				(UserInfo& user);
	bool					IsWebSessionUser			();
	_String					GetUserName					();
	virtual int				GetUserId					();
	virtual bool			IsChatBlocked				();
	virtual bool			IsAvatarBlocked				();
	int						GetAvatarId					();

	bool					IsPermissionSet				(int nPermissionId);

	bool					SetUserBalance				(INT_CHIPS nBalanceAmount);
	bool					AddUserBalance				(INT_CHIPS nBalanceAmountAdd);
	bool					SetUserVPPoints				(double dVPPoints);
	bool					SetUserInPlay				(INT_CHIPS nInPlayAmount);
	bool					SetUserPlayBalance			(INT_CHIPS nPlayBalance);
	bool					SetUserInPlayPlayBalance	(INT_CHIPS nInPlayPlayBalance);

	INT_CHIPS				GetBalance					();
	double					GetVPPoints					();
	INT_CHIPS				GetInPlayBalance			();
	INT_CHIPS				GetPlayBalance				();
	INT_CHIPS				GetInPlayPlayBalance		();
	
	void					OpenTable					(int nTableId, int nTournamentId);
	void					CloseTable					(int nTableId, int nTournamentId);
	void					TableCloseAndUnseat			(int nTableId, int nTournamentId);
	void					SeatAt						(int nTableId, int nSeatIndex, int nTournamentId);
	void					Unseat						(int nTableId, int nTournamentId);
	int						GetSeatIndex				(int nTableId, bool& bOpened, int nTournamentId);
	bool					IsTableObserver				(int nTableId, int nTournamentId);
	int						GetSeatedTableIds			(AutoSortedArrayInt64& arrSeatIndexByTableId);

	bool					RegisterInTournament		(int nTournamentId, int nRebuyCt, int nAddonCt);
	bool					UnregisterFromTournament	(int nTournamentId);
	bool					IsRegisteredInTournament	(int nTournamentId);

	bool					SetTournamentUserDetail		(int nTournamentId, INT_CHIPS nStake = -1, int nRebuyCt = -1, int nAddonCt = -1, int nPlace = -1);
	bool					GetTournamentUserDetail		(int nTournamentId, INT_CHIPS& nStake, int& nRebuyCt, int& nAddonCt, int& nPlace);
	bool					GetTournamentUserDetail_RebuyAddonCt(int nTournamentId, int& nRebuyCt, int& nAddonCt);

	// Avatar image uploading/downloading.
	bool					IsAvatarUploadingStarted	();
	bool					IsAvatarUploadingCompleted	();
	bool					DiscardAvatarUploading		();
	bool					StartUploadingAvatarImage	(int nImageSize);
	bool					UploadAvatarImageData		(BYTE* pBinary, int nBinarySize, int& nFullSize, int& nBytesLeft, bool& bCompleted);
	bool					SaveUploadedAvatar			(_String sImageFile, int nAvatarIdNew);

	bool					IsAvatarDownloadingStarted	();
	bool					IsAvatarDownloadingCompleted();
	bool					DiscardAvatarDownloading	();
	bool					StartDownloadingAvatarImage	(void* lpImageData, int nDataSize);
	bool					DownloadAvatarImageData		(LongBinary* pBinDownload, int nDownloadPortionSize, int& nFullSize, int& nBytesLeft, bool& bCompleted);

protected:
	static AutoSortedArray*	GetSessionByUserIdArray(){
		static AutoSortedArray arrSessionByUserId;
		return &arrSessionByUserId;
		}

	static CCriticalSection* GetUserSessionLock(){
		static CCriticalSection lock;
		return &lock;
		}

	static void	LockUserSessionArray(){
		GetUserSessionLock()->Lock();
		}

	static void	UnLockUserSessionArray(){
		GetUserSessionLock()->Unlock();
		}

private:
	UserInfo				m_userInfo;
	AutoSortedArrayInt64	m_arrSeatIndexByTableId;		// <(int32)(table Id) + (int32)(tournament id)><(int32)(table open status) + (int32)(seat index)>

	GrowableMemory			m_uploadingAvatarImageData;		// Avatar uploading image data.
	GrowableMemory			m_downloadingAvatarImageData;	// Avatar downloading image data.
};