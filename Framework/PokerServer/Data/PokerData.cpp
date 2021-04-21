#include "stdafx.h"
#include "../PokerTableController.h"
#include "PokerData.h"
#include "../../Utility/GrowableMemory.h"
#include "../../Path/Path.h"
#include "../../MD5/MD5Checksum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerData::PokerData(){
	m_pDataMan		= NULL;
//	m_sConnString	= _T("Driver={SQL Server};Server=localhost;Database=ESPokerDB; Trusted_Connection=yes;");
	m_sConnString	= _T("Driver={SQL Server Native Client 10.0};Server=localhost;Database=ESPokerDB; Trusted_Connection=yes;");

	m_listAvatars	.CreateVariableIndex(_T("Id"), Serializable::Compare_Int32);
	m_listGameSpeed	.CreateVariableIndex(_T("Id"), Serializable::Compare_Int32);
	}

BEGIN_DECLARE_VAR(PokerData, Serializable)
	DECLARE_VAR(_T("PokerData"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("PokerAvatar"),	_T(""), VariableType::VT_None, offsetof(PokerData,m_listAvatars), false, true, true)
	DECLARE_VAR(_T("GameSpeed"),	_T(""), VariableType::VT_None, offsetof(PokerData,m_listGameSpeed), false, true, true)
	DECLARE_VAR(_T("GameType"),		_T(""), VariableType::VT_None, offsetof(PokerData,m_listGameType), false, true, true)
	DECLARE_VAR(_T("GameSubType"),	_T(""), VariableType::VT_None, offsetof(PokerData,m_listGameSubType), false, true, true)
	DECLARE_VAR(_T("MoneyType"),	_T(""), VariableType::VT_None, offsetof(PokerData,m_listMoneyType), false, true, true)
END_DECLARE_VAR()

PokerData::~PokerData(){
	FreeData(true); // Free data close database.
	}

bool
PokerData::LoadTable(EnumerableObject<Serializable>* pRecordObjects, const _String sWhereClause /*= _T("")*/, const _String sOrderBy /*= _T("")*/){
	if( !m_pDataMan ) return false;
	return m_pDataMan->LoadTable(pRecordObjects, sWhereClause, sOrderBy);
	}

bool
PokerData::RefreshRecord(Serializable* pRecord){
	if( !m_pDataMan ) return false;
	return m_pDataMan->RefreshRecord(pRecord);
	}

bool
PokerData::SaveRecord(Serializable* pRecord){
	if( !m_pDataMan ) return false;
	return m_pDataMan->SaveRecord(pRecord);
	}

bool
PokerData::DeleteRecord	(Serializable* pRecord){
	if( !m_pDataMan ) return false;
	return m_pDataMan->DeleteRecord(pRecord);
	}

bool
PokerData::ExecuteProc(Serializable* pProcDesc){
	if( !m_pDataMan ) return false;
	return m_pDataMan->ExecuteProc(pProcDesc);
	}

bool
PokerData::LoadData(){
	FreeData(true);

	m_pDataMan = new DataManager(m_sConnString);
	if( !m_pDataMan->Open() ){
		delete m_pDataMan;
		m_pDataMan = NULL;
		return false;
		}
/*
	PokerAvatar avatar;
	avatar.m_nId = 2;
	avatar.m_recStatus = 0;
	Serializable::LoadImageData(_T("d:\\avatar_1.png"), avatar.m_binData);
	m_pDataMan->SaveRecord(&avatar);*/

	/*
	PokerAvatar avatar;
	avatar.m_nId = 0;
	avatar.m_recStatus = 0;
	Serializable::LoadImageData(_T("d:\\avatar1.png"), avatar.m_binData);
	m_pDataMan->SaveRecord(&avatar);
*/

	/*
	CreateUserProc* pCreateUser = new CreateUserProc();
	pCreateUser->m_sUserName = _T("zack");
	pCreateUser->m_sPassword = _T("zack");
	m_pDataMan->ExecuteProc(pCreateUser);
	delete pCreateUser;
	*/
/*
	TestTableProc* pTestProc = new TestTableProc();
	pTestProc->m_str = "test";
	pTestProc->m_date = COleDateTime::GetCurrentTime();
	pTestProc->m_datetime = COleDateTime::GetCurrentTime();
	pTestProc->m_big = 5555555555555;
	pTestProc->m_float = 123.89f;
	pTestProc->m_real = 1234444.89;
	pTestProc->m_dec = 1234441.89;
	pTestProc->m_num = 1234440.89;
	pTestProc->m_money = 123.89988;

	Serializable::GlobalReAlloc(&pTestProc->m_binary, 20);


	BYTE* lp = (BYTE*)GlobalLock(pTestProc->m_binary.m_hData);
	lp[0] = 'z';
	lp[1] = 'a';
	lp[2] = 'q';
	lp[3] = 'q';
	lp[4] = 'q';
	lp[5] = 'q';
	lp[6] = 'q';
	lp[7] = 'z';
	GlobalUnlock(pTestProc->m_binary.m_hData);

	m_pDataMan->ExecuteProc(pTestProc);
	delete pTestProc;
	*/

	/*
	EnumerableObject<TestTable>* pTest = new EnumerableObject<TestTable>();
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)pTest);
	if( pTest->GetCount() > 0 ){
		TestTable* pRec = pTest->GetAt(0);
	//	pRec->m_nId = 0;
		pRec->m_str = "zaqro [changed]";
		//Serializable::FreeBinary(&pRec->m_binary);
		//m_pDataMan->DeleteRecord(pRec);
		//m_pDataMan->SaveRecord(pRec);
		}
	delete pTest;
	*/
	m_lockAttributeTables.Lock(); // Lock
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listGameType,			_T(""), _T(""));	// Game type
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listGameSubType,		_T(""), _T(""));	// Game sub type
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listMoneyType,		_T(""), _T(""));	// Money type
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listGameSpeed,		_T(""), _T(""));	// Game speed
	m_lockAttributeTables.Unlock(); // Unlock
	return true;
	}

bool
PokerData::LoadAvatars(){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	// load default avatars.
	m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&m_listAvatars, _T("where [STATUS]=0 and [PRIVATE_USER_ID]=0"), _T(""));	// Avatar table

	_String sAvatars = m_sAvatarsFolder;
	Path::BuildFullPath(sAvatars);

	for( int nLoop=0; nLoop<m_listAvatars.GetCount(); nLoop++ ){
		PokerAvatar* pAvatar = m_listAvatars[nLoop];
		ASSERT( pAvatar );
		if( pAvatar ){
			// Read avatar image data from file to binary.{{
			_String sFileName = sAvatars + _T("\\") + pAvatar->m_sAvatarFileName;
			HANDLE hFile = CreateFile(sFileName.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0L, NULL);
			if( hFile == INVALID_HANDLE_VALUE ) continue;
			DWORD dwFileSize	= ::GetFileSize(hFile, NULL);
			if( pAvatar->m_binImage.SetBinarySize(dwFileSize) > 0 ){
				void* lpData = pAvatar->m_binImage.LockMemory();
				DWORD dwRead;
				::ReadFile(hFile, lpData, dwFileSize, &dwRead, NULL);
				pAvatar->m_binImage.UnlockMemory();
				}
			CloseHandle(hFile);
			// }}	
			}
		}
	return true;
	}

void
PokerData::FreeData(bool bCloseDb /*= false*/){
	if( bCloseDb ){
		if( m_pDataMan ){
			delete m_pDataMan;
			m_pDataMan = NULL;
			}
		}
		
	m_lockAttributeTables.Lock(); // Lock
	m_listGameType		.RemoveAll(true);
	m_listGameSubType	.RemoveAll(true);
	m_listMoneyType		.RemoveAll(true);
	m_listAvatars		.RemoveAll(true);
	m_listGameSpeed		.RemoveAll(true);
	m_lockAttributeTables.Unlock(); // Unlock
	}

PokerAvatar*
PokerData::LoadAvatarInfo(int nAvatarId){
	_String sAvatarImageFile;
	sAvatarImageFile.Format(_T("%s\\%s"), m_sAvatarsFolder.GetBuffer(), GetAvatarFileName(nAvatarId).GetBuffer());
	Path::BuildFullPath(sAvatarImageFile);

	int		nSize = 0;
	BYTE*	lpContent = NULL;
	if( !Path::ReadFileContent(sAvatarImageFile, lpContent, nSize) )
		return NULL;

	PokerAvatar* pAvatarNew			= new PokerAvatar();
	pAvatarNew->m_nId				= nAvatarId;
	pAvatarNew->m_nPrivateUserId	= 0;
	pAvatarNew->m_recStatus			= 0;
	pAvatarNew->m_binImage.SetBinarySize(nSize, lpContent);

	delete [] lpContent;
	return pAvatarNew;
	}

int
PokerData::GetUserAvatarId(int nUserId){
	int nAvatarId = 0;
	_String sWhereSQL;
	sWhereSQL.Format(_T("WHERE ID=%d"), nUserId);

	EnumerableObject<PokerUser> listUsers;
	if( m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&listUsers, sWhereSQL) && listUsers.GetCount() > 0 ){
		PokerUser* pUserData = listUsers.GetAt(0);
		if( pUserData )
			nAvatarId = pUserData->m_nAvatarId;
		listUsers.RemoveAll(true);
		}
	return nAvatarId;
	}

bool
PokerData::LoadAvatarData(int nAvatarId, GrowableMemory* pImageData){
	ASSERT(pImageData && nAvatarId > 0);
	_String sAvatarImageFile;
	sAvatarImageFile.Format(_T("%s\\%s"), m_sAvatarsFolder.GetBuffer(), GetAvatarFileName(nAvatarId).GetBuffer());
	Path::BuildFullPath(sAvatarImageFile);

	int		nSize = 0;
	BYTE*	lpContent = NULL;
	if( !Path::ReadFileContent(sAvatarImageFile, lpContent, nSize) )
		return NULL;

	pImageData->SetBufferSize(nSize);
	pImageData->SetUsedBufferSize(0);
	pImageData->AddBytes(lpContent, nSize);
	pImageData->SetUsedBufferSize(0);
	
	delete [] lpContent;
	return true;
	}

_String
PokerData::GetAvatarFileName(int nAvatarId){
	_String sNameCrypt, sAvatarFielName;
	sNameCrypt.Format(_T("avatar_id=%d"), nAvatarId);
	_String sMD5		= CMD5Checksum::GetMD5((BYTE*)sNameCrypt.GetBuffer(), sNameCrypt.GetLength());
	sAvatarFielName.Format(_T("%s.jpg"), sMD5.GetBuffer());
	return sAvatarFielName;
	}

bool
PokerData::ExchangeVPPoints(int nUserId, int nCurrencyId, double dVPPoints, INT_CHIPS& nBalanceNew, double& dVPPointsNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	if( nUserId <= 0 || dVPPoints <= 0 )
		return false;

	ConvertVPPointsToMoneyProc proc;
	proc.m_nSuccess			= 0;
	proc.m_nUserId			= nUserId;
	proc.m_nCurrencyId		= nCurrencyId;
	proc.m_dExactAmount		= 0.0;
	proc.m_dVPPointsConvert	= dVPPoints;
	proc.m_dBalanceNew		= 0.0;
	proc.m_dVPPointsNew		= 0.0;

	if( !m_pDataMan->ExecuteProc(&proc) && !proc.m_nSuccess ){
		nBalanceNew		= Serializable::ConvertMoney(proc.m_dBalanceNew);
		dVPPointsNew	= proc.m_dVPPointsNew;
		return true;
		}
	return false;
	}

bool
PokerData::ExchangeVPPointsToGetAmount(int nUserId, int nCurrencyId, INT_CHIPS nAmount, INT_CHIPS& nBalanceNew, double& dVPPointsNew){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	if( nUserId <= 0 || nAmount <= 0 )
		return false;

	ConvertVPPointsToMoneyProc proc;
	proc.m_nSuccess			= 0;
	proc.m_nUserId			= nUserId;
	proc.m_nCurrencyId		= nCurrencyId;
	proc.m_dExactAmount		= (double)(nAmount/100.0f);
	proc.m_dVPPointsConvert	= 0.0;
	proc.m_dBalanceNew		= 0.0;
	proc.m_dVPPointsNew		= 0.0;

	if( !m_pDataMan->ExecuteProc(&proc) && !proc.m_nSuccess ){
		nBalanceNew		= Serializable::ConvertMoney(proc.m_dBalanceNew);
		dVPPointsNew	= proc.m_dVPPointsNew;
		return true;
		}
	return false;
	}

int
PokerData::CreateUser(int nUserId, _String sUserName, _String sPassword, _String sEmail, _String sFirstName, _String sLastName, _String sPhone, int nPlayBalanceChips){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return -1;

	CreateUserProc proc;
	proc.m_sUserName = sUserName;
	proc.m_sPassword = sPassword;
	proc.m_sFirstName = sFirstName;
	proc.m_sLastName = sLastName;
	proc.m_sPhone = sPhone;
	proc.m_sEMail = sEmail;
	proc.m_nUserId = nUserId;
	proc.m_dPlayAmount = nPlayBalanceChips;

	if( !m_pDataMan->ExecuteProc(&proc) )
		return -1;
	return proc.m_nRetUserId;
	}

UserInfo*
PokerData::LoginUser(const _String sUserName, const _String sPassword, bool& bUserNotFound, bool& bUserBlocked, bool& bWrongPassword){
	bUserNotFound	= false;
	bUserBlocked	= false;
	bWrongPassword	= false;
	if( !m_pDataMan || !m_pDataMan->IsOpen() || sUserName.IsEmpty() || sPassword.IsEmpty() )
		return NULL;

	AuthenticateUserProc proc;
	proc.m_sUserName = sUserName;
	proc.m_sPassword = sPassword;
	if( !m_pDataMan->ExecuteProc(&proc) ){
		return NULL; // Procedure execution problem.
		}

	if( proc.m_nUserId <= 0 ){
		bUserNotFound	= true;
		bWrongPassword	= proc.m_bUserExists; 
		return NULL;
		}

	if( proc.m_bIsBlocked ){
		bUserBlocked = true;
		return NULL;
		}

	UserInfo* pUser				= new UserInfo();
	pUser->m_sUserName			= sUserName;
	pUser->m_nUserId			= proc.m_nUserId;
	pUser->m_nBalanceAmount		= ConvertMoney(proc.m_dBalance);
	pUser->m_nInPlayAmount		= 0;
	pUser->m_nPlayBalance		= ConvertMoney(proc.m_dPlayBalance);
	pUser->m_shRankLevel		= proc.m_nUserLevelId;
	pUser->m_dVPPoints			= proc.m_dVPPoints;
	pUser->m_nPermissionId		= proc.m_nPermissionId;
	pUser->m_pAvatarInfo		= LoadAvatarInfo(proc.m_nAvatarId);
	pUser->m_nAvatarId			= proc.m_nAvatarId;
	pUser->m_sAvatarFileName	= pUser->m_pAvatarInfo ? pUser->m_pAvatarInfo->m_sAvatarFileName : _T("");
	pUser->m_bAvatarIsBlocked	= proc.m_bAvatarStatus;
	pUser->m_bChatIsBlocked		= proc.m_bChatBlockStatus;
	return pUser;
	}

UserInfo*
PokerData::LoginUserByWebSessionId(const _String sSessionId, bool& bSessionIsAlreadyActive, bool& bUserBlocked){
	bSessionIsAlreadyActive	= false;
	bUserBlocked			= false;
	if( !m_pDataMan || !m_pDataMan->IsOpen() || sSessionId.GetLength() == 0 )
		return NULL;

	AuthenticateUserBySessionIdProc proc;
	proc.m_sSessionId = sSessionId;
	if( !m_pDataMan->ExecuteProc(&proc) )
		return NULL; // Procedure execution problem.

	if( proc.m_nUserId <= 0 ){
		bSessionIsAlreadyActive	= proc.m_bSessionIsAlreadyActive; 
		return NULL;
		}

	if( proc.m_bIsBlocked ){
		bUserBlocked = true;
		return NULL;
		}

	proc.m_sUserName.Trim();
	UserInfo* pUser						= new UserInfo();
	pUser->m_sUserName					= proc.m_sUserName;
	pUser->m_nUserId					= proc.m_nUserId;
	pUser->m_nBalanceAmount				= ConvertMoney(proc.m_dBalance);
	pUser->m_nInPlayAmount				= 0;
	pUser->m_nPlayBalance				= ConvertMoney(proc.m_dPlayBalance);
	pUser->m_shRankLevel				= proc.m_nUserLevelId;
	pUser->m_dVPPoints					= proc.m_dVPPoints;
	pUser->m_nPermissionId				= proc.m_nPermissionId;
	pUser->m_pAvatarInfo				= LoadAvatarInfo(proc.m_nAvatarId);
	pUser->m_nAvatarId					= proc.m_nAvatarId;
	pUser->m_sAvatarFileName			= pUser->m_pAvatarInfo ? pUser->m_pAvatarInfo->m_sAvatarFileName : _T("");
	pUser->m_bAvatarIsBlocked			= proc.m_bAvatarStatus;
	pUser->m_bChatIsBlocked				= proc.m_bChatBlockStatus;
	pUser->m_bAuthenticatedBySessionId	= true; // Set session authentication flag.
	return pUser;
	}

bool
PokerData::LogoutUser(UserInfo* pUserInfo){
	ASSERT(pUserInfo);
	if( !pUserInfo ) return false;
	return false;
	}

bool
PokerData::LogoutWebSessionUser(int nUserId){
	if( !m_pDataMan || !m_pDataMan->IsOpen() || nUserId == 0 )
		return NULL;
	DeauthenticateUserSessionProc proc;
	proc.m_nUserId = nUserId;
	if( !m_pDataMan->ExecuteProc(&proc) )
		return NULL; // Procedure execution problem.
	// Success.
	if( proc.m_nSuccess == 0 )
		return true;
	// User web session not existed or already deactivated !!!
	return false;
	}

int
PokerData::GetUserIdByName(_String sUserName){
	if( !m_pDataMan || !m_pDataMan->IsOpen() || sUserName.IsEmpty() )
		return 0;
	_String sSQL;
	sSQL.Format(_T("SELECT ID from dbo.TB_USER where [USER_NAME] = '%s'"), sUserName.GetBuffer());
	EnumerableObject<PokerUser> listUsers;
	if( m_pDataMan->LoadTableBySql((EnumerableObject<Serializable>*)&listUsers, sSQL, true) ){
		PokerUser* pUser = listUsers.GetAt(0);
		return pUser->m_nId; // UserId
		}
	return 0;
	}

bool
PokerData::GetUserNameById(int nUserId, _String& sUserName){
	if( nUserId <= 0 || !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	_String sSQL;
	sSQL.Format(_T("SELECT USER_NAME from dbo.TB_USER where [ID] = %d"), nUserId);
	EnumerableObject<PokerUser> listUsers;
	if( m_pDataMan->LoadTableBySql((EnumerableObject<Serializable>*)&listUsers, sSQL, true) ){
		PokerUser* pUser = listUsers.GetAt(0);
		sUserName =  pUser->m_sUserName;
		return true;
		}
	return false;
	}

bool
PokerData::LoadUserTournaments(int nUserId, LoginPacket* p){
	if( nUserId < 0 || !p ) return false;
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	UserRegTournamentsProc proc;
	proc.m_nUserId = nUserId;
	if( !m_pDataMan->ExecuteProc(&proc) )
		return false; // Procedure execution problem.

	p->m_listRegTournaments.RemoveAll(true);
	for(int i=0; i<proc.m_listRegTournaments.GetCount(); i++){
		UserRegTournament* pRegTournament = proc.m_listRegTournaments.GetAt(i);
		if( !pRegTournament ) continue;

		UserTournament* pNew	= new UserTournament();
		pNew->m_nTournamentId	= pRegTournament->m_nTournamentId;
		pNew->m_sName			= pRegTournament->m_sName;
		pNew->m_dtStart			= pRegTournament->m_dtStartDTime;
		pNew->m_cGameType		= pRegTournament->m_nGameTypeId;
		pNew->m_shGameSubType	= pRegTournament->m_nSubTypeId;
		pNew->m_nBuyinAmount	= (int)Serializable::ConvertMoney(pRegTournament->m_dBuyIn);
		pNew->m_nFeeAmount		= (int)Serializable::ConvertMoney(pRegTournament->m_dBuyInFee);
		pNew->m_cCurrencyId		= pRegTournament->m_nCurrencyId;
		pNew->m_status			= pRegTournament->m_nStatus;
		p->m_listRegTournaments.Add(pNew);
		}
	return true;
	}

UserInfo*
PokerData::LoadUserInfo(int nUserId){
	if( nUserId <= 0 ) return NULL;

	_String sWhereSQL;
	sWhereSQL.Format(_T("WHERE ID=%d"), nUserId);
	EnumerableObject<PokerUser> listUsers;
	if( m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&listUsers, sWhereSQL) && listUsers.GetCount() > 0 ){
		PokerUser* pUserData = listUsers.GetAt(0);
		listUsers.RemoveAll(false);

		UserInfo* pUser				= new UserInfo();
		pUser->m_sUserName			= pUserData->m_sUserName;
		pUser->m_nUserId			= nUserId;
		pUser->m_nBalanceAmount		= 0;
		pUser->m_nInPlayAmount		= 0;
		pUser->m_nPlayBalance		= 0;
		pUser->m_shRankLevel		= 0;
		pUser->m_dVPPoints			= pUserData->m_dVPPoints;
		pUser->m_nPermissionId		= pUserData->m_nPermissionId;
		pUser->m_pAvatarInfo		= LoadAvatarInfo(pUserData->m_nAvatarId);
		pUser->m_nAvatarId			= pUserData->m_nAvatarId;
		pUser->m_bAvatarIsBlocked	= pUserData->m_bIsAvatarBlocked;
		pUser->m_bChatIsBlocked		= pUserData->m_bIsChatBlocked;

		delete pUserData;
		return pUser;
		}
	return NULL;
	}

INT_CHIPS
PokerData::GetUserBalance(int nUserId, int nCurrencyId){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return 0;
	GetUserBalanceProc proc;
	proc.m_nUserId		= nUserId;
	proc.m_nCurrencyId	= nCurrencyId;
	proc.m_dBalance		= 0.0;
	if( !m_pDataMan->ExecuteProc(&proc) ){
		return 0; // Procedure execution problem.
		}
	return ConvertMoney(proc.m_dBalance);
	}

bool
PokerData::SetUserPlayBalance(int nUserId, INT_CHIPS nPlayBalance){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;
	SetUserPlayBalanceProc proc;
	proc.m_nUserId		= nUserId;
	proc.m_dBalance		= ((double)(nPlayBalance / 100.0));
	if( !m_pDataMan->ExecuteProc(&proc) ){
		return false; // Procedure execution problem.
		}
	return (proc.m_nSuccess == 0);
	}

int				
PokerData::SetUserAvatar(int nUserId, _String &sAvatarFileName, int nAvatarId /*= -1*/, bool bIsPrivateAvatar /*= true*/){
	if( nUserId <= 0 ) return 0;
	SetUserAvatarProc proc;
	proc.m_nAvatarId		= nAvatarId;
	proc.m_nUserId			= nUserId;
	proc.m_bIsPrivateAvatar	= bIsPrivateAvatar;

	int nRet = 0;
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		int nAvatarIdNew	= proc.m_nAvatarIdNew;
		nRet				= nAvatarIdNew;
		if( nAvatarId != nAvatarIdNew ){
			sAvatarFileName = GetAvatarFileName(nAvatarIdNew);

			// Update new avatars file name. {{
			PokerAvatar avatarNew;
			avatarNew.m_nId				= nAvatarIdNew;
			avatarNew.m_sAvatarFileName = sAvatarFileName;
			CStringVector arrDirtyFields;
			arrDirtyFields.push_back(_T("AVATAR_FILE_NAME"));
			if( m_pDataMan->SaveRecord(&avatarNew, &arrDirtyFields) ){
				/*
				PokerAvatar* pAvatarInfoNew			= new PokerAvatar();
				pAvatarInfoNew->m_nId				= proc.m_nAvatarIdNew;
				pAvatarInfoNew->m_nPrivateUserId	= nUserId;
				pAvatarInfoNew->m_sAvatarFileName	= sAvatarFileName;
				pAvatarInfoNew->m_recStatus			= 0;
				m_listAvatars.Add(pAvatarInfoNew);
				*/
				}
			// }}
			}
		else
			sAvatarFileName = GetAvatarFileName(nAvatarId);
		nAvatarIdNew = proc.m_nAvatarIdNew;
		}
	return nRet;
	}

bool
PokerData::TransferMoneyFromMainBalance(int nUserId, INT_CHIPS nAmount, int nCurrencyId, double dMainBalance, int nMainBalanceEntryId, INT_CHIPS& nBalanceNew){
	if( nUserId <= 0 || nAmount <= 0 )
		return false;

	TransferFromMainBalanceProc proc;
	proc.m_nUserId				= nUserId;
	proc.m_dAmount				= (double)(nAmount / 100.0);
	proc.m_nCurrencyId			= nCurrencyId;
	proc.m_nMainBalanceEntryId	= nMainBalanceEntryId;
	proc.m_dMainBalance			= dMainBalance;
	proc.m_dBalanceNew			= 0.0;
	nBalanceNew					= 0;
	
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerData::TransferMoneyToMainBalance(int nUserId, INT_CHIPS nAmount, int nCurrencyId, double dMainBalance, int nMainBalanceEntryId, INT_CHIPS& nBalanceNew){
	if( nUserId <= 0 || nAmount <= 0 )
		return false;

	TransferToMainBalanceProc proc;
	proc.m_nUserId				= nUserId;
	proc.m_dAmount				= (double)(nAmount / 100.0);
	proc.m_nCurrencyId			= nCurrencyId;
	proc.m_nMainBalanceEntryId	= nMainBalanceEntryId;
	proc.m_dMainBalance			= dMainBalance;
	proc.m_dBalanceNew			= 0.0;
	nBalanceNew					= 0;
	
	if( m_pDataMan->ExecuteProc(&proc) && proc.m_nSuccess == 0 ){
		nBalanceNew = ConvertMoney(proc.m_dBalanceNew);
		return true;
		}
	return false;
	}

bool
PokerData::GetHandHistoryInfo(__int64 nHandId, PokerHandHistory& handInfo){
	if( !m_pDataMan || !m_pDataMan->IsOpen() )
		return false;

	_String sWhereSQL;
	sWhereSQL.Format(_T("where [ID]=%d"), nHandId);

	EnumerableObject<PokerHand> resultList;
	bool bRet = m_pDataMan->LoadTable((EnumerableObject<Serializable>*)&resultList, sWhereSQL, _T(""));
	if( !bRet || resultList.GetCount() == 0 )
		return false;

	PokerHand* pHandInfo = resultList.GetAt(0);
	if( !pHandInfo )
		return false;

	bRet = (handInfo.Deserialize(&pHandInfo->m_binHandInfo) == TRUE);
	return bRet;
	}
