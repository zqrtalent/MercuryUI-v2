#include "stdafx.h"
#include "GameServer.h"
#include "ClientConnection.h"
#include "..\Protocol\BasePacket.h"
#include "..\Utility\GrowableMemory.h"
#include "..\Path\Path.h"
#include "DataManageThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class DataManageThread
///
///////////////////////////////////////////////////////////

DataManageThread::DataManageThread(GameServer* pServer){
	ASSERT(pServer);
	m_pServer					= pServer;
	m_threadDataManager			= NULL;
	m_dwDataManagerThreadId		= 0L;
	m_pStopDataManager			= NULL;
	m_pResumeDataManager		= NULL;
	} 

DataManageThread::~DataManageThread(){
	Stop();
	}

///////////////////////////////////////////////////////////
///
///	Public Methods
///
///////////////////////////////////////////////////////////

bool
DataManageThread::Start(){
	if( !m_threadDataManager ){
		m_pStopDataManager		= new CEvent(FALSE, TRUE);
		m_pResumeDataManager	= new CEvent(FALSE, TRUE);
		m_threadDataManager		= ::CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)DataManageThread::DataManage_Thread, 
										this, 0L, &m_dwDataManagerThreadId);
		return true;
		}
	return false;
	}

bool
DataManageThread::Stop(){
	if( m_threadDataManager ){
		m_pStopDataManager		->SetEvent();
		m_pResumeDataManager	->SetEvent();
		WaitForSingleObject(m_threadDataManager, INFINITE);

		delete m_pStopDataManager;
		delete m_pResumeDataManager;
		CloseHandle(m_threadDataManager);

		m_pStopDataManager		= NULL;
		m_pResumeDataManager	= NULL;
		m_threadDataManager		= NULL;

		// Delete list of received data. {{
		int nLoop = 0;
		while( nLoop < m_listReceivedData.GetCount() ){
			ReceivedPacket* pRecv = m_listReceivedData[nLoop];
			if( pRecv && pRecv->packet )
				delete pRecv->packet;
			nLoop ++;
			}
		m_listReceivedData.RemoveAll();
		// }}
		return true;
		}
	return false;
	}

bool
DataManageThread::ManageReceivedPacket(ClientConnection* pConn, SOCKET sockFrom, BasePacket* packet){
	if( m_threadDataManager ){
		ReceivedPacket* pRecv	= new ReceivedPacket;
		pRecv->pConn			= pConn;
		pRecv->sockFrom			= sockFrom;
		pRecv->packet			= packet;
		
		m_lockDataManager.Lock(); // Lock
		m_listReceivedData.Add(pRecv);
		m_lockDataManager.Unlock(); //Unlock

		m_pResumeDataManager->SetEvent();
		return true;
		}
	return false;
	}

UINT  __stdcall 
DataManageThread::DataManage_Thread(DataManageThread* pMan){
	GameServer*				pServer			= pMan->m_pServer;
	ASSERT(pServer);
	CEvent*					pStop			= pMan->m_pStopDataManager;
	CEvent*					pResume			= pMan->m_pResumeDataManager;
	CCriticalSection*		lock			= &pMan->m_lockDataManager;
	List<ReceivedPacket>*	pListData		= &pMan->m_listReceivedData;
	AutoSortedArrayInt64*	pAcceptedSocks	= &pServer->m_arrAcceptedSocks;
	CCriticalSection*		lockConn		= &pServer->m_lockConnections;
	int						nIndex			= -1;

	ClientConnection*		pConn			= NULL;
	BasePacket*				pRecvPacket		= NULL;
	SOCKET					sockFrom		= 0;

	while( TRUE ){
		// Wait while event is not signaled.
		WaitForSingleObject(*pResume, INFINITE);

		// Check if stop event is signaled. {{
		if( WaitForSingleObject(*pStop, 0) == WAIT_OBJECT_0 ){
			break;
			}
		// }}

		// Manage received data. {{
		lock->Lock();			// Lock
		ReceivedPacket* pData	= pListData->GetAt(0);
		if( pData ){
			pConn			= pData->pConn;
			pRecvPacket		= pData->packet;
			sockFrom		= pData->sockFrom;
			}
		pListData->RemoveAt(0); // Remove first element.
		UNUSED(pData);			// No longer available.
		lock->Unlock();			// Unlock

		if( pRecvPacket ){
			// Manage received data only from active connections but not from dead sockets.
			lockConn->Lock();	// Lock
			nIndex = pAcceptedSocks->IndexOf((__int64)sockFrom);
			if( nIndex != -1){
				pConn = (ClientConnection*)pAcceptedSocks->GetData(nIndex);
				if( pConn ){
					lockConn->Unlock(); // Unlock
					pServer->OnPacketReceived(pConn, pRecvPacket);
					}
				else
					lockConn->Unlock(); // Unlock
				}
			else
				lockConn->Unlock(); // Unlock
			
			delete pRecvPacket; // Delete handled packet.
			pConn	= NULL;
			}
		// }}

		lock->Lock();			// Lock
		if( pListData->GetCount() == 0 )
			pResume->ResetEvent(); // Nothing to do.
		lock->Unlock();			// Unlock
		}
	return 0L;
	}
