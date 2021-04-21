#include "stdafx.h"
#include "PokerSession.h"
#include "../GameServer/ClientConnection.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class PokerSession
///
///////////////////////////////////////////////////////////

PokerSession::PokerSession(GameServer* pServer, ClientConnection* pConn /*= NULL*/) : Session(pServer, pConn) , 
	m_uploadingAvatarImageData(0, 1024, false), 
	m_downloadingAvatarImageData(0, 1024){
	m_userInfo.ZeroInit();
	}

PokerSession::~PokerSession(){
	if( m_userInfo.m_nUserId > 0 ){
		//RemoveUserSession(m_userInfo.m_nUserId);
		SetUserInfo(NULL);
		}
	}

PokerSession*
PokerSession::GetSessionByUserId(int nUserId, bool bRetain){
	LockUserSessionArray(); // Lock
	PokerSession*		pSession			= NULL;
	AutoSortedArray*	pArrSessionByUserId	= GetSessionByUserIdArray();
	int					nIndex				= pArrSessionByUserId->IndexOf((void*)nUserId);
	if( nIndex != -1 ){
		pSession = (PokerSession*)pArrSessionByUserId->GetData(nIndex);
		if( bRetain ) 
			pSession = (PokerSession*)PokerSession::Retain(pSession);
		}
	UnLockUserSessionArray(); // Unlock
	return pSession;
	}

bool
PokerSession::AddUserSession(int nUserId, PokerSession* pSession){
	ASSERT(pSession);
	LockUserSessionArray(); // Lock
	AutoSortedArray*	pArrSessionByUserId	= GetSessionByUserIdArray();
	int					nIndex				= pArrSessionByUserId->IndexOf((void*)nUserId);
	if( nIndex == -1 ){
//		pSession = (PokerSession*)PokerSession::Retain(pSession);
		pArrSessionByUserId->Add((void*)nUserId, (void*)pSession);
		UnLockUserSessionArray(); // Unlock
		return true;
		}
	UnLockUserSessionArray(); // Unlock
	return false;
	}

int
PokerSession::RemoveAllUserSession(){
	LockUserSessionArray();		// Lock
	AutoSortedArray*	pArrSessionByUserId	= GetSessionByUserIdArray();
	int nLoop = 0, nCt = pArrSessionByUserId->GetCount();
	while( nLoop < nCt ){
	//	PokerSession* pSess = (PokerSession*)pArrSessionByUserId->GetData(nLoop);
	//	if( pSess ) SafePointer::Release(pSess);
		nLoop ++;
		}
	pArrSessionByUserId->DeleteAll();
	UnLockUserSessionArray();	// Unlock
	return nCt;
	}

bool
PokerSession::RemoveUserSession(int nUserId){
	LockUserSessionArray(); // Lock
	AutoSortedArray*	pArrSessionByUserId	= GetSessionByUserIdArray();
	int					nIndex				= pArrSessionByUserId->IndexOf((void*)nUserId);
	if( nIndex != -1 ){
		PokerSession* pSess = (PokerSession*)pArrSessionByUserId->GetData(nIndex);
//		if( pSess ) PokerSession::Release(pSess);
		pArrSessionByUserId->Delete(nIndex);
		UnLockUserSessionArray(); // Unlock
		return true;
		}
	UnLockUserSessionArray(); // Unlock
	return false;
	}

bool
PokerSession::GetUserInfoById(int nUserId, UserInfo* pUserInfoCopy){
	ASSERT(pUserInfoCopy);
	PokerSession* pSession = GetSessionByUserId(nUserId, true);
	if( !pSession )
		return false;
	if( pSession->m_userInfo.m_nUserId <= 0 ){
		SafePointer::Release(pSession); // Release session pointer.
		return false;
		}
	if( pUserInfoCopy )
		pSession->m_userInfo.Copy(pUserInfoCopy);
	SafePointer::Release(pSession); // Release session pointer.
	return true;
	}

UserInfo*
PokerSession::GetUserInfo(){
	if( m_userInfo.m_nUserId <= 0 )
		return NULL;
	return &m_userInfo;
	}

void
PokerSession::SetUserInfo(UserInfo* pUserInfo){
	if( m_userInfo.m_nUserId == (pUserInfo ? pUserInfo->m_nUserId : 0) ){
		pUserInfo->Copy(&m_userInfo);
		return;
		}

	// Remove old one.
	if( m_userInfo.m_nUserId > 0 ){
		RemoveUserSession(m_userInfo.m_nUserId);
		m_userInfo.ZeroInit();
		}

	// Set new one.
	if( pUserInfo ){
		pUserInfo->Copy(&m_userInfo);
		AddUserSession(m_userInfo.m_nUserId, this);
		}
	}

void
PokerSession::SetAuthorized(bool bAuthorized){
	ClientConnection* pConn = GetConnection(true, true);
	if( pConn ){
		pConn->SetAuthorized(bAuthorized);
		SafePointer::Release(pConn);
		}

	GetTableConnectionLock()->Lock(); // Lock
	int nLoop = 0, nCt = m_listTableConns.GetCount();
	while(nLoop < nCt){
		pConn = m_listTableConns.GetData(nLoop);
		if( pConn )
			pConn->SetAuthorized(bAuthorized);	
		nLoop ++;
		}
	GetTableConnectionLock()->Unlock(); // Unlock
	}

bool
PokerSession::SetUserBalance(INT_CHIPS nBalanceAmount){
	if( m_userInfo.m_nUserId > 0 ){
		if( nBalanceAmount < 0 ) nBalanceAmount = 0;
		m_userInfo.m_nBalanceAmount = nBalanceAmount;
		return true;
		}
	return false;
	}


bool
PokerSession::AddUserBalance(INT_CHIPS nBalanceAmountAdd){
	if( m_userInfo.m_nUserId > 0 ){
		m_userInfo.m_nBalanceAmount += nBalanceAmountAdd;
		return true;
		}
	return false;
	}

bool
PokerSession::SetUserVPPoints(double dVPPoints){
	if( m_userInfo.m_nUserId > 0 ){
		if( dVPPoints < 0 ) dVPPoints = 0;
		m_userInfo.m_dVPPoints = dVPPoints;
		return true;
		}
	return false;
	}

bool
PokerSession::SetUserInPlay(INT_CHIPS nInPlayAmount){
	if( m_userInfo.m_nUserId > 0 ){
		if( nInPlayAmount < 0 ) nInPlayAmount = 0;
		m_userInfo.m_nInPlayAmount = nInPlayAmount;
		return true;
		}
	return false;
	}

bool
PokerSession::SetUserPlayBalance(INT_CHIPS nPlayBalance){
	if( m_userInfo.m_nUserId > 0 ){
		if( nPlayBalance < 0 ) nPlayBalance = 0;
		m_userInfo.m_nPlayBalance = nPlayBalance;
		return true;
		}
	return false;
	}

bool
PokerSession::SetUserInPlayPlayBalance(INT_CHIPS nInPlayPlayBalance){
	if( m_userInfo.m_nUserId > 0 ){
		if( nInPlayPlayBalance < 0 ) nInPlayPlayBalance = 0;
		m_userInfo.m_nInPlayPlayBalance = nInPlayPlayBalance;
		return true;
		}
	return false;
	}

void
PokerSession::SeatAt(int nTableId, int nSeatIndex, int nTournamentId /*= 0*/){
	__int64 nValue	= MAKEINT64(nTableId, nTournamentId);
	int nIndex = m_arrSeatIndexByTableId.IndexOf(nValue);
	if( nIndex == -1 ){
		m_arrSeatIndexByTableId.Add(nValue, MAKEINT64(nSeatIndex, FALSE));
		}
	else{
		__int64 nData		= m_arrSeatIndexByTableId.GetData(nIndex);
		BOOL	bOpenStatus = (BOOL)(nData >> 32);
		m_arrSeatIndexByTableId.SetData(nIndex, MAKEINT64(nSeatIndex, bOpenStatus));
		}
	}

void
PokerSession::Unseat(int nTableId, int nTournamentId /*= 0*/){
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	int nIndex = m_arrSeatIndexByTableId.IndexOf(nValue);
	if( nIndex != -1 ){
		__int64 nData		= m_arrSeatIndexByTableId.GetData(nIndex);
		BOOL	bOpenStatus = (BOOL)(nData >> 32);
		if( bOpenStatus )
			m_arrSeatIndexByTableId.SetData(nIndex, MAKEINT64(-1, bOpenStatus));
		else
			m_arrSeatIndexByTableId.Delete(nIndex);
		}
	}

void
PokerSession::OpenTable(int nTableId, int nTournamentId /*= 0*/){
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	int nIndex = m_arrSeatIndexByTableId.IndexOf(nValue);
	if( nIndex == -1 ){
		m_arrSeatIndexByTableId.Add(nValue, MAKEINT64(-1, TRUE));
		}
	else{
		__int64 nData		= m_arrSeatIndexByTableId.GetData(nIndex);
		int		nSeatIndex	= (int)(nData);
		m_arrSeatIndexByTableId.SetData(nIndex, MAKEINT64(nSeatIndex, TRUE));
		}
	}

void
PokerSession::CloseTable(int nTableId, int nTournamentId /*= 0*/){
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	int nIndex = m_arrSeatIndexByTableId.IndexOf(nValue);
	if( nIndex != -1 ){
		__int64 nData		= m_arrSeatIndexByTableId.GetData(nIndex);
		int		nSeatIndex	= (int)(nData);
		if( nSeatIndex > -1 )
			m_arrSeatIndexByTableId.SetData(nIndex, MAKEINT64(nSeatIndex, FALSE));
		else
			m_arrSeatIndexByTableId.Delete(nIndex);
		}
	}

void
PokerSession::TableCloseAndUnseat(int nTableId, int nTournamentId){
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	int nIndex = m_arrSeatIndexByTableId.IndexOf(nValue);
	if( nIndex != -1 ){
		m_arrSeatIndexByTableId.Delete(nIndex);
		}
	}

int
PokerSession::GetSeatIndex(int nTableId, bool& bOpened, int nTournamentId /*= 0*/){
	bOpened = false;
	__int64 nValue = MAKEINT64(nTableId, nTournamentId);
	int nIndex = m_arrSeatIndexByTableId.IndexOf(nValue);
	if( nIndex != -1 ){
		__int64 nData		= m_arrSeatIndexByTableId.GetData(nIndex);
		int		nSeatIndex	= (int)(nData);
		BOOL	bOpenStatus = (BOOL)(nData >> 32);
		bOpened				= (bOpenStatus == TRUE);
		return nSeatIndex;
		}
	return -1;
	}

bool
PokerSession::IsTableObserver(int nTableId, int nTournamentId /*= 0*/){
	bool bOpened;
	int nSeat = GetSeatIndex(nTableId, bOpened, nTournamentId);
	return ( nSeat == -1 && bOpened );
	}

int
PokerSession::GetSeatedTableIds(AutoSortedArrayInt64& arrSeatIndexByTableId){
	int nLoop = 0, nCt = m_arrSeatIndexByTableId.GetCount();
	while( nLoop < nCt ){
		__int64 nTableId	= m_arrSeatIndexByTableId.Get(nLoop);
		int nSeatIndex		= (int)m_arrSeatIndexByTableId.GetData(nLoop);
		arrSeatIndexByTableId.Add(nTableId, nSeatIndex);
		nLoop ++;
		}
	return arrSeatIndexByTableId.GetCount();
	}

bool
PokerSession::RegisterInTournament(int nTournamentId, int nRebuyCt, int nAddonCt){
	if( m_userInfo.m_nUserId > 0 ){
		EnumerableObject<UserTournamentDetails>* pListDetails = &m_userInfo.m_listTournamentDetails;
		for(int i=0; i<pListDetails->GetCount(); i++){
			UserTournamentDetails* pDetail = pListDetails->GetAt(i);
			if( pDetail && pDetail->m_nTournamentId == nTournamentId ){
				return false; // Already registered.
				}
			}

		UserTournamentDetails* pDetail	= new UserTournamentDetails();
		pDetail->m_nTournamentId		= nTournamentId;
		pDetail->m_nStake				= 0;
		pDetail->m_nPlace				= -1;
		pDetail->m_cAddonLeft			= (char)nAddonCt;
		pDetail->m_cRebuyLeft			= (char)nRebuyCt;
		pListDetails->Add(pDetail);
		return true;
		}
	return false;
	}

bool
PokerSession::UnregisterFromTournament(int nTournamentId){
	if( m_userInfo.m_nUserId > 0 ){
		EnumerableObject<UserTournamentDetails>* pListDetails = &m_userInfo.m_listTournamentDetails;
		for(int i=0; i<pListDetails->GetCount(); i++){
			UserTournamentDetails* pDetail = pListDetails->GetAt(i);
			if( pDetail && pDetail->m_nTournamentId == nTournamentId ){
				pListDetails->RemoveAt(i, true);
				return true;
				}
			}
		}
	return false;
	}

bool
PokerSession::IsRegisteredInTournament(int nTournamentId){
	if( m_userInfo.m_nUserId > 0 ){
		EnumerableObject<UserTournamentDetails>* pListDetails = &m_userInfo.m_listTournamentDetails;
		for(int i=0; i<pListDetails->GetCount(); i++){
			UserTournamentDetails* pDetail = pListDetails->GetAt(i);
			if( pDetail && pDetail->m_nTournamentId == nTournamentId )
				return true; // Registered !!!
			}
		}
	return false;
	}

bool
PokerSession::SetTournamentUserDetail(int nTournamentId, INT_CHIPS nStake /*= -1*/, int nRebuyCt /*= -1*/, int nAddonCt /*= -1*/, int nPlace /*= -1*/){
	if( m_userInfo.m_nUserId > 0 ){
		EnumerableObject<UserTournamentDetails>* pListDetails = &m_userInfo.m_listTournamentDetails;
		for(int i=0; i<pListDetails->GetCount(); i++){
			UserTournamentDetails* pDetail = pListDetails->GetAt(i);
			if( pDetail && pDetail->m_nTournamentId == nTournamentId ){
				if( nStake != -1  )
					pDetail->m_nStake = nStake;
				if( nRebuyCt != -1 )
					pDetail->m_nPlace = nPlace;
				if( nRebuyCt != -1 )
					pDetail->m_cRebuyLeft = (char)nRebuyCt;
				if( nAddonCt != -1 )
					pDetail->m_cAddonLeft = (char)nAddonCt;
				}
			}
		}
	return false;
	}

bool
PokerSession::GetTournamentUserDetail(int nTournamentId, INT_CHIPS& nStake, int& nRebuyCt, int& nAddonCt, int& nPlace){
	if( m_userInfo.m_nUserId > 0 ){
		EnumerableObject<UserTournamentDetails>* pListDetails = &m_userInfo.m_listTournamentDetails;
		for(int i=0; i<pListDetails->GetCount(); i++){
			UserTournamentDetails* pDetail = pListDetails->GetAt(i);
			if( pDetail && pDetail->m_nTournamentId == nTournamentId ){
				nStake		= pDetail->m_nStake;
				nRebuyCt	= (int)pDetail->m_cRebuyLeft;
				nAddonCt	= (int)pDetail->m_cAddonLeft;
				nPlace		= pDetail->m_nPlace;
				}
			}
		}
	return false;
	}

bool
PokerSession::GetTournamentUserDetail_RebuyAddonCt(int nTournamentId, int& nRebuyCt, int& nAddonCt){
	bool bRet = false;
	if( m_userInfo.m_nUserId > 0 ){
		EnumerableObject<UserTournamentDetails>* pListDetails = &m_userInfo.m_listTournamentDetails;
		for(int i=0; i<pListDetails->GetCount(); i++){
			UserTournamentDetails* pDetail = pListDetails->GetAt(i);
			if( pDetail && pDetail->m_nTournamentId == nTournamentId ){
				nRebuyCt	= (int)pDetail->m_cRebuyLeft;
				nAddonCt	= (int)pDetail->m_cAddonLeft;
				bRet		= true;
				}
			}
		}
	return bRet;
	}

bool
PokerSession::IsWebSessionUser(){
	if( m_userInfo.m_nUserId <= 0 ) 
		return false;
	return m_userInfo.m_bAuthenticatedBySessionId;
	}

_String
PokerSession::GetUserName(){
	if( m_userInfo.m_nUserId <= 0 )
		return _T("");
	return m_userInfo.m_sUserName;
	}

int
PokerSession::GetUserId(){
	return m_userInfo.m_nUserId;
	}

bool
PokerSession::IsChatBlocked(){
	return m_userInfo.m_bChatIsBlocked;
	}

bool
PokerSession::IsAvatarBlocked(){
	return m_userInfo.m_bAvatarIsBlocked;
	}

int
PokerSession::GetAvatarId(){
	if( m_userInfo.m_nUserId <= 0 )
		return 0;
	return m_userInfo.m_nAvatarId;
	}

bool
PokerSession::IsPermissionSet(int nPermissionId){
	if( m_userInfo.m_nUserId <= 0 )
		return false;
	return ((m_userInfo.m_nPermissionId&nPermissionId) == nPermissionId);
	}

INT_CHIPS
PokerSession::GetBalance(){
	if( m_userInfo.m_nUserId <= 0 ) return 0;
	return m_userInfo.m_nBalanceAmount;
	}

double
PokerSession::GetVPPoints(){
	if( m_userInfo.m_nUserId <= 0 ) return 0.0;
	return m_userInfo.m_dVPPoints;
	}

INT_CHIPS
PokerSession::GetInPlayBalance(){
	if( m_userInfo.m_nUserId <= 0 ) return 0;
	return m_userInfo.m_nInPlayAmount;
	}

INT_CHIPS
PokerSession::GetPlayBalance(){
	if( m_userInfo.m_nUserId <= 0 ) return 0;
	return m_userInfo.m_nPlayBalance;
	}

INT_CHIPS
PokerSession::GetInPlayPlayBalance(){
	if( m_userInfo.m_nUserId <= 0 ) return 0;
	return m_userInfo.m_nInPlayPlayBalance;
	}

bool
PokerSession::CopyUserInfo(UserInfo& user){
	if( m_userInfo.m_nUserId <= 0 ) return false;
	m_userInfo.Copy(&user);
	user.SetAvatarInfo(m_userInfo.GetAvatarInfo(), false);
	return true;
	}

// Avatar image uploading/downloading.
bool
PokerSession::IsAvatarUploadingStarted(){
	return (m_uploadingAvatarImageData.GetBufferSize() > 0);
	}

bool
PokerSession::IsAvatarUploadingCompleted(){
	if( m_uploadingAvatarImageData.GetBufferSize() <= 0 )
		return false;
	return (m_uploadingAvatarImageData.GetBufferSize() == m_uploadingAvatarImageData.GetUsedBufferSize());
	}

bool
PokerSession::DiscardAvatarUploading(){
	m_uploadingAvatarImageData.SetBufferSize(0);
	return true;
	}

bool
PokerSession::StartUploadingAvatarImage	(int nImageSize){
	if( IsAvatarUploadingStarted() || nImageSize <= 0 || nImageSize >= 1024*64/*64KB*/ ) 
		return false;
	if( m_uploadingAvatarImageData.SetBufferSize(nImageSize) == nImageSize )
		return true;
	m_uploadingAvatarImageData.SetBufferSize(0);
	return false;
	}

bool
PokerSession::UploadAvatarImageData(BYTE* pBinary, int nBinarySize, int& nFullSize, int& nBytesLeft, bool& bCompleted){
	ASSERT(pBinary && nBinarySize > 0);
	if( m_uploadingAvatarImageData.GetUsedBufferSize() + nBinarySize > m_uploadingAvatarImageData.GetBufferSize() ){
		nBytesLeft	= 0;
		nFullSize	= 0;
		bCompleted	= false;
		DiscardAvatarUploading();
		return false;
		}

	if( !m_uploadingAvatarImageData.AddBytes(pBinary, nBinarySize) ){
		nBytesLeft	= 0;
		nFullSize	= 0;
		bCompleted	= false;
		DiscardAvatarUploading();
		return false;
		}

	nBytesLeft	= (m_uploadingAvatarImageData.GetBufferSize() - m_uploadingAvatarImageData.GetUsedBufferSize());
	bCompleted	= (nBytesLeft == 0);
	nFullSize	= m_uploadingAvatarImageData.GetBufferSize();
	return true;
	}

bool
PokerSession::SaveUploadedAvatar(_String sImageFile, int nAvatarIdNew){
	if( !IsAvatarUploadingCompleted() )
		return false;

	// Set new avatar id.
	m_userInfo.m_nAvatarId = nAvatarIdNew;

	HANDLE hAvatarFile = ::CreateFile(sImageFile.GetBuffer(), GENERIC_WRITE, 0, 0L, CREATE_NEW, 0L, NULL);
	if( hAvatarFile == INVALID_HANDLE_VALUE )
		return false;

	DWORD dwWrite	= m_uploadingAvatarImageData.GetBufferSize();
	DWORD dwWritten = 0L;
	::WriteFile(hAvatarFile, m_uploadingAvatarImageData.GetBufferPtr(), dwWrite, &dwWritten, NULL);

	// Update user info's avatar id.
	m_userInfo.UpdateAvatarInfo(nAvatarIdNew, m_uploadingAvatarImageData.GetBufferPtr(), (UINT)m_uploadingAvatarImageData.GetUsedBufferSize());
	// Discard uploading data.
	DiscardAvatarUploading();

	::CloseHandle(hAvatarFile);
	if( dwWritten == dwWrite )
		return true;

	DeleteFile(sImageFile.GetBuffer());
	return false;
	}

bool
PokerSession::IsAvatarDownloadingStarted(){
	return (m_downloadingAvatarImageData.GetBufferSize() > 0);
	}

bool
PokerSession::IsAvatarDownloadingCompleted(){
	if( m_downloadingAvatarImageData.GetBufferSize() <= 0 )
		return false;
	return (m_downloadingAvatarImageData.GetBufferSize() == m_downloadingAvatarImageData.GetUsedBufferSize());
	}

bool
PokerSession::DiscardAvatarDownloading(){
	m_downloadingAvatarImageData.SetBufferSize(0);
	return true;
	}

bool
PokerSession::StartDownloadingAvatarImage(void* lpImageData, int nDataSize){
	if( IsAvatarDownloadingStarted() || nDataSize <= 0 || nDataSize >= 1024*64/*64KB*/ ) 
		return false;
	// Allocate and copy image data.
	if( m_downloadingAvatarImageData.SetBufferSize(nDataSize) == nDataSize ){
		memcpy(m_downloadingAvatarImageData.GetBufferPtr(), lpImageData, nDataSize);
		return true;
		}
	m_downloadingAvatarImageData.SetBufferSize(0);
	return false;
	}

bool
PokerSession::DownloadAvatarImageData(LongBinary* pBinDownload, int nDownloadPortionSize, int& nFullSize, int& nBytesLeft, bool& bCompleted){
	int		nImageDataSize	= m_downloadingAvatarImageData.GetBufferSize();
	int		nDownloaded		= (int)m_downloadingAvatarImageData.GetCurrentOffset();
	BYTE*	pCurrOffset		= m_downloadingAvatarImageData.GetCurrentOffsetPtr();

	if( (nImageDataSize - nDownloaded) <= 0 ){
		nFullSize	= nImageDataSize;
		nBytesLeft	= 0;
		bCompleted	= true;
		return true;
		}

	// Calculate portion download size.
	nDownloadPortionSize = min((nImageDataSize - nDownloaded), nDownloadPortionSize);
	// Reallocate binary.
	pBinDownload->SetBinarySize(nDownloadPortionSize, pCurrOffset);
	// Change offset of the downloading memory buffer.
	DWORD dwOffsetNew = (DWORD)(m_downloadingAvatarImageData.GetCurrentOffset() + nDownloadPortionSize);
	m_downloadingAvatarImageData.SetCurrentOffset((DWORD)(m_downloadingAvatarImageData.GetCurrentOffset() + nDownloadPortionSize));

	nBytesLeft	= (nImageDataSize - m_downloadingAvatarImageData.GetCurrentOffset());
	bCompleted	= (nBytesLeft == 0);
	nFullSize	= nImageDataSize;
	return true;
	}