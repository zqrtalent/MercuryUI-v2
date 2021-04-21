#include "stdafx.h"
#include "Session.h"
#include "ClientConnection.h"
#include "GameServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class Session
///
///////////////////////////////////////////////////////////

Session::Session(GameServer* pServer, ClientConnection* pConn /*= NULL*/){
	m_pServer		= pServer;
	if( pConn ){
		pConn = (ClientConnection*)SafePointer::Retain(pConn); // Retain connection pointer.
		m_bActive	= true;
		}
	else
		m_bActive	= false;
	m_pConnection	= pConn;
	m_sessionId		= GenerateNewSessionId(this);
	}

Session::~Session(){
	// Release connection.
	if( m_pConnection ) 
		SafePointer::Release(m_pConnection);
	// Release table connections.
	ReleaseTableConnections();

	m_mapKeyValue.DeleteAll(true);
	}

ClientConnection*
Session::GetConnection(bool bRetain, bool bNullWhenInactive /*= true*/){
	if( bNullWhenInactive && !IsActive() )
		return NULL;
	if( bRetain && m_pConnection)
		return (ClientConnection*)SafePointer::Retain(m_pConnection); 
	return m_pConnection;
	}

void
Session::SetConnection(ClientConnection* pConn, bool bDestroyOldConn){
	if( m_pConnection == pConn ) return;
	if( bDestroyOldConn && m_pConnection )
		SafePointer::Release(m_pConnection); // Release connection pointer.
	if( pConn )
		pConn = (ClientConnection*)SafePointer::Retain(pConn); // Retain connection pointer.
	m_pConnection = pConn;
	}

ClientConnection*
Session::GetTableConnection(int nTableId, int nTournamentId, bool bRetain, bool bNullWhenInactive){
	ASSERT(nTableId > 0 && nTournamentId >= 0);
	if( bNullWhenInactive && !IsActive() )
		return NULL;

	ClientConnection* pConn = NULL;
	__int64 nKey = MAKEINT64(nTableId, nTournamentId);
	GetTableConnectionLock()->Lock(); // Lock
	int nIndex = m_listTableConns.FindValue(nKey);
	if( nIndex != -1 )
		pConn = m_listTableConns.GetData(nIndex);
	GetTableConnectionLock()->Unlock(); // Unlock

	if( bRetain && pConn)
		return (ClientConnection*)SafePointer::Retain(pConn); 

	// Retrieve main connection if table connection not exists.
	if( !pConn )
		return GetConnection(bRetain, bNullWhenInactive);
	return pConn;
	}

void
Session::SetTableConnection(int nTableId, int nTournamentId, ClientConnection* pConn, bool bDestroyOldConn){
	ASSERT(nTableId > 0 && nTournamentId >= 0);
	ClientConnection* pConnOld = NULL;
	__int64 nKey = MAKEINT64(nTableId, nTournamentId);
	GetTableConnectionLock()->Lock();	// Lock
	int nIndex = m_listTableConns.FindValue(nKey);
	if( nIndex != -1 ){
		pConnOld = m_listTableConns.GetData(nIndex);
		if( pConnOld != pConn )
			m_listTableConns.SetData(nIndex, pConn);
		}
	else
		m_listTableConns.Add(nKey, pConn);
	GetTableConnectionLock()->Unlock(); // Unlock

	if( pConnOld != pConn ){
		if( bDestroyOldConn && pConnOld )
			SafePointer::Release(pConnOld); // Release pointer.
		if( pConn )
			pConn = (ClientConnection*)SafePointer::Retain(pConn); // Retain pointer.
		}
	}

void*
Session::Get(const _String sKeyName){
	return m_mapKeyValue[sKeyName];
	}

bool
Session::Set(const _String sKeyName, void* lpValue){
	int nIndexKey = -1;
	if( !m_mapKeyValue.KeyExists(sKeyName, &nIndexKey) ){
		m_mapKeyValue.Add(sKeyName, lpValue);
		}
	else{
		m_mapKeyValue.SetValueByIndex(nIndexKey, lpValue);
		}
	return true;
	}

bool
Session::Remove(const _String sKeyName, void** lppValue /*= NULL*/){
	int nIndexKey = -1;
	if( m_mapKeyValue.KeyExists(sKeyName, &nIndexKey) ){
		if( lppValue != NULL )
			*lppValue = m_mapKeyValue.GetValueByIndex(nIndexKey);
		m_mapKeyValue.DeleteByIndex(nIndexKey);
		return TRUE;
		}
	return FALSE;
	}

int
Session::RemoveAllGetValues(CPtrVector& m_arrValues){
	int nLoop = 0, nCt = m_mapKeyValue.GetCount();
	m_arrValues.resize(nCt);
	while( nLoop < nCt ){
		m_arrValues.push_back(m_mapKeyValue.GetValueByIndex(nLoop));
		nLoop ++;
		}

	m_mapKeyValue.DeleteAll();
	return nCt;
	}

int
Session::RemoveAll(bool bDestoryObjects){
	int nLoop = 0, nCt = m_mapKeyValue.GetCount();
	if( bDestoryObjects ){
		while( nLoop < nCt ){
			void* lpValue = m_mapKeyValue.GetValueByIndex(nLoop);
			if( lpValue )
				delete lpValue;
			nLoop ++;
			}
		}
	m_mapKeyValue.DeleteAll();
	return nCt;
	}

__int64
Session::GenerateNewSessionId(Session* pSess){
	LockGlobalSession(); // Lock
	srand((UINT)time(NULL));
	__int64 nLow		= rand();
	__int64 nHi			= rand();
	__int64 idNew		= nLow | (nHi << 32);

	AutoSortedArrayInt64* pArrIds = GetArraySessionIds();
	while( pArrIds->IndexOf(idNew) != -1 ){
		nLow	= rand();
		nHi		= rand();
		idNew	= nLow | (nHi << 32);
		}
	AddSessionIdIntoList(idNew, pSess);
	UnLockGlobalSession(); // Unlock
	return idNew;
	}

bool
Session::RemoveSessionIdFromList(__int64 nSessionId){
	bool bRet = false;
	AutoSortedArrayInt64* pArrIds = GetArraySessionIds();
	LockGlobalSession(); // Lock

	int nIndex = pArrIds->IndexOf(nSessionId);
	if( nIndex != -1 ){
		Session* pSess = (Session*)pArrIds->GetData(nIndex);
		if( pSess && pSess->IsActive() ) {
			//pSess->SetDeletingDelegate(NULL);	// Zero delegate function to prevent the same operation on array. see method 'OnSessionDeleting'
			pSess->SetActive(false);			// Inactive session and remain session in list until it is not released.
			//pSess->SetDeletingDelegate((SafePointerDeletingProc)OnSessionDeleting);

			bool bReleased = SafePointer::Release(pSess);	// Release session pointer.			
			if( !bReleased ){		
				}
			UnLockGlobalSession();			// Unlock
			return true;
			}
		pArrIds->Delete(nIndex);
		bRet = true;
		}
	UnLockGlobalSession(); // Unlock
	return bRet;
	}

bool
Session::AddSessionIdIntoList(__int64 nSessionId, Session* pSess){
	bool bRet = false;
	AutoSortedArrayInt64* pArrIds = GetArraySessionIds();
	LockGlobalSession(); // Lock
	int nIndex = pArrIds->IndexOf(nSessionId);
	if( nIndex == -1 ){
		pArrIds->Add(nSessionId, (__int64)pSess);
		if( pSess->IsActive() )				// Retain session pointer only when session is active.
			if( pSess ) 
				pSess = (Session*)SafePointer::Retain(pSess);	// Retain session pointer.
		bRet = true;
		}
	UnLockGlobalSession(); // Unlock
	return bRet;
	}

Session*
Session::CreateNewSession(GameServer* pServer, ClientConnection* pConn, bool bRetain){
	ASSERT(pServer);
	Session* pNew = pServer->CreateSession(pConn);
	if( pNew ){
		if( pConn ) pConn->SetSession(pNew);
		pNew->SetDeletingDelegate((SafePointerDeletingProc)OnSessionDeleting);
		if( bRetain )
			return (Session*)SafePointer::Retain(pNew); // Retain session pointer.
		}
	return pNew;
	}

Session*
Session::RestoreInactiveSession(__int64 nSessionId, ClientConnection* pConn, bool bRetain){
	Session* pSess = NULL;
	LockGlobalSession();	// Lock
	AutoSortedArrayInt64* pArrIds = GetArraySessionIds();
	int nIndex = pArrIds->IndexOf(nSessionId);
	if( nIndex != -1 ){
		pSess = (Session*)pArrIds->GetData(nIndex);
		ASSERT(pSess);
		if( !pSess->IsActive() ){
			pSess->SetConnection(pConn, true);				// Set connection.
			pSess = (Session*)SafePointer::Retain(pSess);	// Retain session pointer.
			pSess->SetActive(true);							// Make session active.
			if( bRetain ) 
				pSess = (Session*)SafePointer::Retain(pSess);
			}
		else
			pSess = NULL;
		}
	UnLockGlobalSession();	// Unlock
	return pSess;
	}

bool
Session::DestroySession(Session* pSess){
	if( !pSess ) return false;
	bool bRet = Session::RemoveSessionIdFromList(pSess->GetSessionId());
	return bRet;
	}

Session*
Session::GetSessionById(__int64 nSessionId, bool bRetain){
	Session*				pSess	= NULL;
	AutoSortedArrayInt64*	pArrIds = GetArraySessionIds();
	LockGlobalSession(); // Lock

	int nIndex = pArrIds->IndexOf(nSessionId);
	if( nIndex != -1 ){
		pSess = (Session*)pArrIds->GetData(nIndex);
		if( pSess && bRetain )
			pSess = (Session*)SafePointer::Retain(pSess);
		}
	UnLockGlobalSession(); // Unlock
	return pSess;
	}

void
Session::DestroyAllSessions(){
	AutoSortedArrayInt64*	pArrIds = GetArraySessionIds();
	LockGlobalSession(); // Lock
	int nLoop = 0, nCt = pArrIds->GetCount();

	while( nLoop < nCt ){
		Session* pSess = (Session*)pArrIds->GetData(nLoop);
		if( pSess ){
			pSess->SetDeletingDelegate(NULL);	// No need to call delegate.
			SafePointer::Release(pSess);		// Release session pointer.
			}
		nLoop ++;
		}
	pArrIds->DeleteAll();
	UnLockGlobalSession(); // Unlock
	}

bool
Session::OnSessionDeleting(Session* pSess){
	bool bActive = pSess->IsActive();
	LockGlobalSession();	// Lock
	// Remove session id from array.
	AutoSortedArrayInt64*	pArrIds = GetArraySessionIds();
	int nIndex = pArrIds->IndexOf(pSess->GetSessionId());
	if( nIndex != -1 ){
		// Session status changed so we should not delete session pointer.
		if( !bActive && pSess->IsActive() ){
			UnLockGlobalSession();	// Unlock
			return false;
			}
		pSess->m_pServer->OnSessionExpired(pSess);
		pArrIds->Delete(nIndex);
		}
	UnLockGlobalSession();	// Unlock
	return true;
	}

int	
Session::ReleaseTableConnections(){
	AutoSortedArrayTempl<__int64, ClientConnection*>	listTableConnsTemp;
	GetTableConnectionLock()->Lock(); // Lock
	m_listTableConns.Copy(&listTableConnsTemp);
	m_listTableConns.DeleteAll();
	GetTableConnectionLock()->Unlock(); // Unlock
	
	int nRet = 0, nCt = listTableConnsTemp.GetCount();
	for(int i=0; i<nCt; i++){
		ClientConnection* pTableConn = listTableConnsTemp.GetData(i);
		if( pTableConn ){
			SafePointer::Release(pTableConn);
			nRet ++;
			}
		}
	return nRet;
	}