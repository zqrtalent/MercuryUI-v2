// LobbyInfo.cpp : implementation file
//

#include "stdafx.h"
#include "LobbyInfo.h"
#include "PokerTableInfo.h"
#include "PokerTableSeatInfo.h"

#include "../../Utility/GrowableMemory.h"
#include "../../GameServer/ClientConnection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LobbyInfo::LobbyInfo(){
	m_nPlayersCt						= 0;
	m_nTablesCt							= 0;
	m_nTournamentsCt					= 0;
	m_dtLastUpdate						= COleDateTime::GetCurrentTime();

	m_arrStaticTableInfoByGameType		.OptimizeForLargeData(TRUE);
	m_arrLobbyTournamentInfoByGameType	.OptimizeForLargeData(TRUE);

	m_pMemLobbyInfoSerializer			= new GrowableMemory();

	// Create variable indexes.
	m_listTableStaticInfos				.CreateVariableIndex(_T("TableId"),			Serializable::Compare_Int32);
	m_listTableDynamicInfos				.CreateVariableIndex(_T("TableId"),			Serializable::Compare_Int32);
	m_listTournaments					.CreateVariableIndex(_T("TournamentId"),	Serializable::Compare_Int32);
	}

BEGIN_DECLARE_VAR(LobbyInfo, Serializable)
	DECLARE_VAR(_T("LobbyInfo"),					_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("PlayersCt"),					_T(""),		VariableType::VT_Int32, offsetof(LobbyInfo,m_nPlayersCt), false, false, false)
	DECLARE_VAR(_T("TablesCt"),						_T(""),		VariableType::VT_Int32, offsetof(LobbyInfo,m_nTablesCt), false, false, false)
	DECLARE_VAR(_T("TournamentsCt"),				_T(""),		VariableType::VT_Int32, offsetof(LobbyInfo,m_nTournamentsCt), false, false, false)
	DECLARE_VAR(_T("LastUpdate"),					_T(""),		VariableType::VT_DateTime, offsetof(LobbyInfo,m_dtLastUpdate), false, false, false)
	DECLARE_VAR(_T("ListOfStaticInfos"),			_T(""),		VariableType::VT_None, offsetof(LobbyInfo,m_listTableStaticInfos), false, true, true)
	DECLARE_VAR(_T("ListOfDynamicInfos"),			_T(""),		VariableType::VT_None, offsetof(LobbyInfo,m_listTableDynamicInfos), false, true, true)
	DECLARE_VAR(_T("ListOfTournamentsInfo"),		_T(""),		VariableType::VT_None, offsetof(LobbyInfo,m_listTournaments), false, true, true)
END_DECLARE_VAR()

LobbyInfo::~LobbyInfo(){
	EmptyInfo(); // Empty all

	m_lockLobbyInfoSend.Lock();		// Lock
	if( m_pMemLobbyInfoSerializer ){
		delete m_pMemLobbyInfoSerializer;
		m_pMemLobbyInfoSerializer = NULL;
		}
	m_lockLobbyInfoSend.Unlock();	// Unlock
	}

bool
LobbyInfo::SetTable(PokerGameTable* pTable){
	ASSERT( pTable );
	if( !pTable ) return false;

	int							nTableId	= pTable->m_nTableId;
	LobbyCashTableStaticInfo*	pStaticInfo	= m_listTableStaticInfos.FindOneVariable(_T("TableId"), &nTableId);
	if( !pStaticInfo ){
		pStaticInfo					= new LobbyCashTableStaticInfo();
		pStaticInfo->m_nTableId		= pTable->m_nTableId;
		pStaticInfo->m_sTableName	= pTable->m_sName;
		pStaticInfo->m_cGameType	= (char)(pTable->m_nGameType | (pTable->m_nMoneyType == MoneyType::Play ? GameType::Playmoney : 0));
		pStaticInfo->m_shGameSubType= (short)pTable->m_nGameSubType;
		pStaticInfo->m_btCurrencyId	= (BYTE)pTable->m_nCurrencyId;
		pStaticInfo->m_nSBAmount	= (UINT)Serializable::ConvertMoney(pTable->m_dSmallBlind);
		pStaticInfo->m_nBBAmount	= (UINT)Serializable::ConvertMoney(pTable->m_dBigBlind);
		pStaticInfo->m_nAnteAmount	= (UINT)Serializable::ConvertMoney(pTable->m_dAnte);
		pStaticInfo->m_cMaxPlayerCt = (char)pTable->m_shMaxPlayerCt;
		pStaticInfo->m_nMinBuyIn	= (UINT)Serializable::ConvertMoney(pTable->m_dMinBuyin);
		pStaticInfo->m_nMaxBuyIn	= (UINT)Serializable::ConvertMoney(pTable->m_dMaxBuyin);
		m_listTableStaticInfos.Add(pStaticInfo);

		m_nTablesCt					++;
		// Add table static info in list to search easily by gametype and subtype.
		__int64 nValue = ((((__int64)pStaticInfo->m_cGameType)&0xFF) << 32) | ((int)pStaticInfo->m_shGameSubType);
		m_arrStaticTableInfoByGameType.Add(nValue, (__int64)pStaticInfo);
		}
	else{
		// Update
		}
	return true;
	}

void
LobbyInfo::EmptyInfo(){
	m_lockCashGames.Lock();			// Lock
	m_listTableStaticInfos				.RemoveAll(true);
	m_listTableDynamicInfos				.RemoveAll(true);
	m_arrStaticTableInfoByGameType		.DeleteAll();
	m_lockCashGames.Unlock();		// Unlock

	m_lockTournaments.Lock();		// Lock
	m_listTournaments					.RemoveAll(true);
	m_arrLobbyTournamentInfoByGameType	.DeleteAll();
	m_lockTournaments.Unlock();		// Unlock

	m_nTablesCt			= 0;
	m_nTournamentsCt	= 0;
	m_nPlayersCt		= 0;
	}

bool
LobbyInfo::FindPlayerTables(AutoSortedArray* pArrTableIds, EnumerableObject<FindPlayerTableInfo>* pFindPlayerInfo){
	if( !pArrTableIds || !pFindPlayerInfo ) return false;
	m_lockCashGames.Lock();		// Lock
	int nTableId = 0, nCt = pArrTableIds->GetCount();
	for(int i=0; i<nCt; i++){
		nTableId = (int)pArrTableIds->Get(i);
		LobbyCashTableStaticInfo* pTableInfo = m_listTableStaticInfos.FindOneVariable(_T("TableId"), &nTableId);
		if( pTableInfo ){
			FindPlayerTableInfo* pInfo	= new FindPlayerTableInfo();
			pInfo->m_nTableId			= pTableInfo->m_nTableId;
			pInfo->m_nTournamentId		= 0;
			pInfo->m_sTableName			= pTableInfo->m_sTableName;
			pInfo->m_cGameType			= pTableInfo->m_cGameType;
			pInfo->m_shGameSubType		= pTableInfo->m_shGameSubType;
			pInfo->m_cMaxSeatCt			= pTableInfo->m_cMaxPlayerCt;
			pInfo->m_nSBAmount			= pTableInfo->m_nSBAmount;
			pInfo->m_nBBAmount			= pTableInfo->m_nBBAmount;
			pInfo->m_nAnteAmount		= pTableInfo->m_nAnteAmount;
			pFindPlayerInfo->Add(pInfo);
			}
		}
	m_lockCashGames.Unlock();		// Unlock
	return true;
	}

bool
LobbyInfo::FindTournament(int nTournamentId, LobbyTournamentInfo* pFindInfo){
	if( nTournamentId <= 0 || !pFindInfo ) return false;
	bool bRet = false;
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( pTournamentInfo ){
		pTournamentInfo->Copy(pFindInfo);
		bRet = true;
		}
	m_lockTournaments.Unlock();		// Unlock
	return bRet;
	}

bool
LobbyInfo::SendLobbyInfoByRequest(ClientConnection* pConn, PokerGetLobbyInfoPacket* p){
	if( !p ) 
		return false;

	const GameType		nCashGameTypes[]			= { GameType::TexasHoldem, GameType::Omaha};
	const GameType		nPlayMoneyGameTypes[]		= { (GameType)(GameType::Playmoney|GameType::Omaha), (GameType)(GameType::Playmoney|GameType::TexasHoldem)};
	const GameSubType	nGameSubTypes[]				= { GameSubType::Holdem_FixedLimit, GameSubType::Holdem_NoLimit, GameSubType::Holdem_PotLimit, 
														GameSubType::Omaha_Omaha, GameSubType::Omaha_OmahaHiLo};

	const GameType*		pArrGameTypes				= (p->m_cMoneyType == MoneyType::Real) ? nCashGameTypes : nPlayMoneyGameTypes;
	int					nGameTypesCt				= (sizeof(nCashGameTypes) / sizeof(int));
	int					nGameSubTypesCt				= (sizeof(nGameSubTypes) / sizeof(int));

	const GameType		nTournamentGameTypes[]		= { GameType::Tournament };
	const GameSubType	nTournamentSubTypes[]		= { GameSubType::Tournament_Freeroll, GameSubType::Tournament_Guaranteed, GameSubType::Tournament_Regular, GameSubType::Tournament_Satellite, GameSubType::Tournament_SitNGo};
	const int			nTournamentTypesCt			= (sizeof(nTournamentGameTypes) / sizeof(int));
	const int			nTournamentSubTypesCt		= (sizeof(nTournamentSubTypes) / sizeof(int));
	
	int					nSubType					= (int)p->m_shSubType;
	int					nGameType					= (p->m_cMoneyType == MoneyType::Real) ? (int)p->m_cGameType : ((int)p->m_cGameType|GameType::Playmoney);
	int					nReqFlags					= (int)p->m_cRequestFlag;
	bool				bReqStaticTableInfos		= (nReqFlags&((BYTE)PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesStaticInfo)) > 0 ? true : false;
	bool				bReqDynamicTableInfos		= (nReqFlags&((BYTE)PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablesDynamicInfo)) > 0 ? true : false;

	p->m_shTablesCtByRequest						= 0;
	p->m_shTournamentsCtByRequest					= 0;
	int					nDivide						= 1;
	int					nTablesCt					= 0, nResultItemCt = 0;

	// These members should be empty.
	p->m_listCashTablesStaticInfo	.RemoveAll(true);
	p->m_listCashTablesDynamicInfo	.RemoveAll(true);
	p->m_listCashTablePlayersInfo	.RemoveAll(true);
	p->m_listTournamentInfo			.RemoveAll(true);
	p->m_listTournamentDynamicInfo	.RemoveAll(true);

	GrowableMemory memListObjects(64, 1024, true);

	#pragma pack(1)
	// Read header info. {{
	struct VarHdr{
		char cType;
		int nLength;
	};

	BYTE	btDefaultValue	= (BYTE)(VariableType::VT_Array | 0x80);
	VarHdr	hdrInfo			= {VariableType::VT_Array, 0};
	int*	pArrayLen		= NULL;
	int		nArrayLenOffset	= 0;

	// Cash table static info.
	if( bReqStaticTableInfos ){
		// Add serialize object list header.
		memListObjects.AddBytes((BYTE*)&hdrInfo.cType, sizeof(BYTE));
		nArrayLenOffset	= memListObjects.GetUsedBufferSize();
		memListObjects.AddBytes((BYTE*)&hdrInfo.nLength, sizeof(int));

		for(int i=0; i<nGameTypesCt; i++){
			if( (pArrGameTypes[i]&nGameType) != pArrGameTypes[i] ) continue;
			for(int j=0; j<nGameSubTypesCt; j++){
				if( (nGameSubTypes[j]&nSubType) != 0 ){
					nResultItemCt += SerializeCashTables(&memListObjects, NULL, (GameType)(((int)pArrGameTypes[i])&nGameType), (GameSubType)(((int)nGameSubTypes[j])&nSubType), nTablesCt);
					p->m_shTablesCtByRequest += (short)nTablesCt;
					}
				}
			}

		// Update array length in serialized buffer.
		if( nResultItemCt > 0 ){
			pArrayLen		= (int*)memListObjects.GetOffsetPtr(nArrayLenOffset);
			*pArrayLen		= nResultItemCt;
			nResultItemCt	= 0;
			}

		nDivide = 1;
		}
	else{
		// Add empty object list header (m_listCashTablesStaticInfo).
		memListObjects.AddBytes((BYTE*)&btDefaultValue, sizeof(BYTE));
		}

	// Cash table dynamic info.
	if( bReqDynamicTableInfos ){
		// Add serialize object list header.
		memListObjects.AddBytes((BYTE*)&hdrInfo.cType, sizeof(BYTE));
		nArrayLenOffset	= memListObjects.GetUsedBufferSize();
		memListObjects.AddBytes((BYTE*)&hdrInfo.nLength, sizeof(int));

		for(int i=0; i<nGameTypesCt; i++){
			if( (pArrGameTypes[i]&nGameType) != pArrGameTypes[i] ) continue;
			for(int j=0; j<nGameSubTypesCt; j++){
				if( (nGameSubTypes[j]&nSubType) != 0 ){
					nResultItemCt += SerializeCashTables(NULL, &memListObjects, (GameType)(((int)pArrGameTypes[i])&nGameType), (GameSubType)(((int)nGameSubTypes[j])&nSubType), nTablesCt);
					p->m_shTablesCtByRequest += (short)nTablesCt;
					}
				}
			}

		// Update array length in serialized buffer.
		if( nResultItemCt > 0 ){
			pArrayLen		= (int*)memListObjects.GetOffsetPtr(nArrayLenOffset);
			*pArrayLen		= nResultItemCt;
			nResultItemCt	= 0;
			}

		nDivide ++;
		}
	else{
		// Add empty object list header (m_listCashTablesDynamicInfo).
		memListObjects.AddBytes((BYTE*)&btDefaultValue, sizeof(BYTE));
		}
	p->m_shTablesCtByRequest /= nDivide;
	
	// Cash table players info.
	if( nReqFlags&PokerGetLobbyInfoPacket::LobbyInfoReq_CashTablePlayers && p->m_nTableId > 0 ){
		// Add serialize object list header.
		memListObjects.AddBytes((BYTE*)&hdrInfo.cType, sizeof(BYTE));
		nArrayLenOffset	= memListObjects.GetUsedBufferSize();
		memListObjects.AddBytes((BYTE*)&hdrInfo.nLength, sizeof(int));

		m_lockCashGames.Lock();		// Lock
		LobbyCashTableDynamicInfo*	pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &p->m_nTableId);
		if( pDynamicInfo ){
			for(int i=0; i<pDynamicInfo->m_listPlayers.GetCount(); i++){
				LobbyCashTablePlayerInfo* pPlayerInfo =	pDynamicInfo->m_listPlayers.GetAt(i);
				if( pPlayerInfo )
					pPlayerInfo->Serialize(&memListObjects);
				else{
					// Add null element.
					char nullTypeDef	= (char)VariableType::VT_None;
					memListObjects.AddBytes((BYTE*)&nullTypeDef, sizeof(char));
					}
				nResultItemCt ++;
				}
			}
		m_lockCashGames.Unlock();	// Unlock

		// Update array length in serialized buffer.
		if( nResultItemCt > 0 ){
			pArrayLen		= (int*)memListObjects.GetOffsetPtr(nArrayLenOffset);
			*pArrayLen		= nResultItemCt;
			nResultItemCt	= 0;
			}
		}
	else{
		// Add empty object list header (m_listCashTablePlayersInfo).
		memListObjects.AddBytes((BYTE*)&btDefaultValue, sizeof(BYTE));
		}

	// Tournament infos.
	if( nReqFlags&(PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentInfos|PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentDynamicInfos) ){
		if( nReqFlags&PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentInfos ){
			// Add serialize object list header.
			memListObjects.AddBytes((BYTE*)&hdrInfo.cType, sizeof(BYTE));
			nArrayLenOffset	= memListObjects.GetUsedBufferSize();
			memListObjects.AddBytes((BYTE*)&hdrInfo.nLength, sizeof(int));

			for(int i=0; i<nTournamentTypesCt; i++){
				if( (nTournamentGameTypes[i]&nGameType) == 0 ) continue;
				for(int j=0; j<nTournamentSubTypesCt; j++){
					if( (nTournamentSubTypes[j]&nSubType) != 0 )
						nResultItemCt += SerializeTournaments(&memListObjects, (GameType)(((int)nTournamentGameTypes[i])&nGameType), (GameSubType)(((int)nTournamentSubTypes[j])&nSubType));
					}
				}
			p->m_shTournamentsCtByRequest = nResultItemCt;
			// Add empty object list header. (m_listTournamentDynamicInfo)
			memListObjects.AddBytes((BYTE*)&btDefaultValue, sizeof(BYTE));

			// Update array length in serialized buffer.
			if( nResultItemCt > 0 ){
				pArrayLen		= (int*)memListObjects.GetOffsetPtr(nArrayLenOffset);
				*pArrayLen		= nResultItemCt;
				nResultItemCt	= 0;
				}
			}
		else{
			// Tournament dynamic infos.
			if( nReqFlags&PokerGetLobbyInfoPacket::LobbyInfoReq_TournamentDynamicInfos ){
				// Add empty object list header. (m_listTournamentInfo)
				memListObjects.AddBytes((BYTE*)&btDefaultValue, sizeof(BYTE));
				// Add serialize object list header.
				memListObjects.AddBytes((BYTE*)&hdrInfo.cType, sizeof(BYTE));
				nArrayLenOffset	= memListObjects.GetUsedBufferSize();
				pArrayLen		= (int*)memListObjects.GetOffsetPtr(memListObjects.GetUsedBufferSize());
				memListObjects.AddBytes((BYTE*)&hdrInfo.nLength, sizeof(int));

				for(int i=0; i<nTournamentTypesCt; i++){
					if( (nTournamentGameTypes[i]&nGameType) == 0 ) continue;
					for(int j=0; j<nTournamentSubTypesCt; j++){
						if( (nTournamentSubTypes[j]&nSubType) != 0 )
							nResultItemCt += SerializeTournamentDynamicInfos(&memListObjects, (GameType)(((int)nTournamentGameTypes[i])&nGameType), (GameSubType)(((int)nTournamentSubTypes[j])&nSubType));
						}
					}

				p->m_shTournamentsCtByRequest = nResultItemCt;

				// Update array length in serialized buffer.
				if( nResultItemCt > 0 ){
					pArrayLen		= (int*)memListObjects.GetOffsetPtr(nArrayLenOffset);
					*pArrayLen		= nResultItemCt;
					nResultItemCt	= 0;
					}
				}
			}
		}
	else{
		// Add empty object list header. (m_listTournamentInfo)
		memListObjects.AddBytes((BYTE*)&btDefaultValue, sizeof(BYTE));
		// Add empty object list header. (m_listTournamentDynamicInfo)
		memListObjects.AddBytes((BYTE*)&btDefaultValue, sizeof(BYTE));
		}

	p->m_nPlayersCt			=  m_nPlayersCt;
	p->m_shTablesCt			=  (short)m_nTablesCt;
	p->m_shTournamentsCt	=  (short)m_nTournamentsCt;

	m_lockLobbyInfoSend.Lock();		// Lock
	if( m_pMemLobbyInfoSerializer ){
		m_pMemLobbyInfoSerializer->ZeroUsedMemory();

		p->m_nPacketSize	= 0xFFFF;
		p->Serialize(m_pMemLobbyInfoSerializer);
		
		// We have 5 list array's which we need to add at the end of serialized binary.
		int nSerializedEmptyArraysByteSize = 5;
		m_pMemLobbyInfoSerializer->SetUsedBufferSize(m_pMemLobbyInfoSerializer->GetUsedBufferSize() - nSerializedEmptyArraysByteSize);
		m_pMemLobbyInfoSerializer->AddBytes(memListObjects.GetBufferPtr(), memListObjects.GetUsedBufferSize());

		// Set packet size.
		*(int*)(&(m_pMemLobbyInfoSerializer->GetBufferPtr()[4])) = m_pMemLobbyInfoSerializer->GetUsedBufferSize(); 

		pConn->SendBytes(m_pMemLobbyInfoSerializer->GetBufferPtr(), (UINT)m_pMemLobbyInfoSerializer->GetUsedBufferSize());
		/*
		PokerGetLobbyInfoPacket iii;
		m_pMemLobbyInfoSerializer->ResetBufferPointerPosition();
		iii.Deserialize(m_pMemLobbyInfoSerializer);*/
		}
	m_lockLobbyInfoSend.Unlock();	// Unlock

	/*
	SendPacket(pConn, p); // Send reply 
	p->m_listCashTablePlayersInfo	.RemoveAll(false);
	p->m_listCashTablesDynamicInfo	.RemoveAll(false);
	p->m_listCashTablesStaticInfo	.RemoveAll(false);
	p->m_listTournamentInfo			.RemoveAll(false);
	*/
	return true;
	}

int
LobbyInfo::SerializeCashTables(GrowableMemory* pMemTableStaticInfo, GrowableMemory* pMemTableDynamicInfo, GameType type, GameSubType subType, int& nTablesCt){
	m_lockCashGames.Lock(); // Lock
	__int64 nFindValue	= (((__int64)type) << 32) | ((int)subType);
	int		nIndex		= 0, nRet = 0;
	nTablesCt			= 0;

	if( pMemTableStaticInfo ){
		nIndex	= m_arrStaticTableInfoByGameType.FindFirst(nFindValue);
		if( !pMemTableDynamicInfo ){
			while( nIndex != -1 ){
				LobbyCashTableStaticInfo* pInfo = (LobbyCashTableStaticInfo*)m_arrStaticTableInfoByGameType.GetData(nIndex);
				if( pInfo ){
					pInfo->Serialize(pMemTableStaticInfo);
					nRet ++;
					}
				nIndex = m_arrStaticTableInfoByGameType.FindNext();
				}
			}
		else{
			while( nIndex != -1 ){
				LobbyCashTableStaticInfo* pInfo = (LobbyCashTableStaticInfo*)m_arrStaticTableInfoByGameType.GetData(nIndex);
				if( pInfo ){
					LobbyCashTableDynamicInfo* pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &pInfo->m_nTableId);
					if( pDynamicInfo )
						pDynamicInfo->Serialize(pMemTableDynamicInfo);
					pInfo->Serialize(pMemTableStaticInfo);
					nRet ++;
					}
				nIndex = m_arrStaticTableInfoByGameType.FindNext();
				}
			}
		nTablesCt = nRet;
		}
	else{
		if( pMemTableDynamicInfo ){
			nIndex	= m_arrStaticTableInfoByGameType.FindFirst(nFindValue);
			while( nIndex != -1 ){
				LobbyCashTableStaticInfo* pInfo = (LobbyCashTableStaticInfo*)m_arrStaticTableInfoByGameType.GetData(nIndex);
				if( pInfo ){
					LobbyCashTableDynamicInfo* pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &pInfo->m_nTableId);
					if( pDynamicInfo ){
						pDynamicInfo->Serialize(pMemTableDynamicInfo);
						nRet ++;
						}
					nTablesCt ++;
					}
				nIndex = m_arrStaticTableInfoByGameType.FindNext();
				}
			}
		}
	m_lockCashGames.Unlock(); // Unlock
	return nRet;
	}

int
LobbyInfo::SerializeTournaments(GrowableMemory* pMemInto, GameType type, GameSubType subType){
	m_lockTournaments.Lock(); // Lock
	__int64 nFindValue	= (((__int64)type) << 32) | ((int)subType);
	int		nIndex		= m_arrLobbyTournamentInfoByGameType.FindFirst(nFindValue), nRet = 0;

	while( nIndex != -1 ){
		LobbyTournamentInfo* pInfo = (LobbyTournamentInfo*)m_arrLobbyTournamentInfoByGameType.GetData(nIndex);
		if( pInfo ){
			pInfo->Serialize(pMemInto);
			nRet ++;
			}
		nIndex = m_arrLobbyTournamentInfoByGameType.FindNext();
		}
	m_lockTournaments.Unlock(); // Unlock
	return nRet;
	}

int
LobbyInfo::SerializeTournamentDynamicInfos(GrowableMemory* pMemInto, GameType type, GameSubType subType){
	m_lockTournaments.Lock(); // Lock
	__int64 nFindValue	= (((__int64)type) << 32) | ((int)subType);
	int		nIndex		= m_arrLobbyTournamentInfoByGameType.FindFirst(nFindValue), nRet = 0;
	LobbyTournamentDynamicInfo dynInfo;

	while( nIndex != -1 ){
		LobbyTournamentInfo* pInfo = (LobbyTournamentInfo*)m_arrLobbyTournamentInfoByGameType.GetData(nIndex);
		if( pInfo ){
			if( pInfo->m_status != TournamentStatus::Tournament_Completed )
				dynInfo.m_dtTimeTournamentFinished.SetStatusNull();
			else
				dynInfo.m_dtTimeTournamentFinished = pInfo->m_dtTimeTournamentFinished;
			dynInfo.m_nTournamentId			= pInfo->m_nTournamentId;
			dynInfo.m_status				= pInfo->m_status;
			dynInfo.m_bRegistrationIsOpen	= pInfo->m_bRegistrationIsOpen;
			dynInfo.m_nRegisteredCt			= pInfo->m_nRegisteredCt;
			dynInfo.m_nPlayersCt			= pInfo->m_nPlayersCt;
			dynInfo.m_shTableCt				= pInfo->m_shTableCt;
			dynInfo.Serialize(pMemInto);
			nRet ++;
			}
		nIndex = m_arrLobbyTournamentInfoByGameType.FindNext();
		}
	m_lockTournaments.Unlock(); // Unlock
	return nRet;
	}

bool
LobbyInfo::UpdateTableInfo(PokerTableInfo* pTableInfo){
	ASSERT(pTableInfo);
	if( !pTableInfo ) return false;

	m_lockCashGames.Lock();		// Lock
	int							nTableId		= pTableInfo->m_nTableId;
	LobbyCashTableDynamicInfo*	pDynamicInfo	= m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &nTableId);
	if( !pDynamicInfo ){
		/*
		pDynamicInfo						= new LobbyCashTableDynamicInfo();
		pDynamicInfo->m_nTableId			= nTableId;
		pDynamicInfo->m_nAvgPotAmount		= (int)(pTableInfo->m_dAvgPotAmount*100);
		pDynamicInfo->m_cPlayerCt			= 0;
		pDynamicInfo->m_cPlayerFlopPercent	= 0;
		pDynamicInfo->m_cWaitedPlayersCt	= 0;
		pDynamicInfo->m_shHandPerHour		= (short)(pTableInfo->m_dAvgHandsPerHour);
		m_listTableDynamicInfos.Add(pDynamicInfo);
		*/
		}
	else{
		pDynamicInfo->m_nAvgPotAmount		= (int)(pTableInfo->m_dAvgPotAmount*100);
		pDynamicInfo->m_cPlayerFlopPercent	= (char)(pTableInfo->m_dPlayerFlop*100);
		pDynamicInfo->m_shHandPerHour		= (short)(pTableInfo->m_dAvgHandsPerHour);
		}
	m_lockCashGames.Unlock();	// Unlock
	return true;
	}

int
LobbyInfo::GetTableId(int nGameType, int nGameSubType, bool bIgnoreFullTable, AutoSortedArray& notIntableIds){
	m_lockCashGames.Lock(); // Lock
	__int64 nFindValue	= (((__int64)nGameType) << 32) | ((int)nGameSubType);
	int		nIndex		= m_arrStaticTableInfoByGameType.FindFirst(nFindValue);

	while( nIndex != -1 ){
		LobbyCashTableStaticInfo* pInfo = (LobbyCashTableStaticInfo*)m_arrStaticTableInfoByGameType.GetData(nIndex);
		if( pInfo ){
			if( bIgnoreFullTable ){
				LobbyCashTableDynamicInfo*	pDynamicInfo	= m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &pInfo->m_nTableId);
				if( pDynamicInfo && pDynamicInfo->m_cPlayerCt == pInfo->m_cMaxPlayerCt ){
					nIndex = m_arrStaticTableInfoByGameType.FindNext();
					continue;
					}
				}

			if( notIntableIds.IndexOf((void*)pInfo->m_nTableId) == -1 ){
				m_lockCashGames.Unlock(); // Unlock
				return pInfo->m_nTableId;
				}
			}
		nIndex = m_arrStaticTableInfoByGameType.FindNext();
		}
	m_lockCashGames.Unlock(); // Unlock
	return 0;
	}

bool
LobbyInfo::SeatTablePlayer(int nTableId, int nSeat, PokerTableSeatInfo* pSeatInfo){
	ASSERT(pSeatInfo);
	if( !pSeatInfo ) return false;
	
	m_lockCashGames.Lock();		// Lock
	LobbyCashTableDynamicInfo*	pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &nTableId);
	if( !pDynamicInfo ){
		pDynamicInfo						= new LobbyCashTableDynamicInfo();
		pDynamicInfo->m_nTableId			= nTableId;
		pDynamicInfo->m_nAvgPotAmount		= 0;
		pDynamicInfo->m_cPlayerCt			= 0;
		pDynamicInfo->m_cWaitedPlayersCt	= 0;
		pDynamicInfo->m_shHandPerHour		= 0;
		pDynamicInfo->m_cPlayerFlopPercent	= 0;
		m_listTableDynamicInfos.Add(pDynamicInfo);
		}
	else{
		}
	
	LobbyCashTablePlayerInfo* pPlayerInfo	= new LobbyCashTablePlayerInfo();
	pPlayerInfo->m_cSeatIndex				= (char)nSeat;
	pPlayerInfo->m_cVIPLevel				= (char)pSeatInfo->GetRankLevel();
	pPlayerInfo->m_nSeatAmount				= pSeatInfo->GetBalance();
	pPlayerInfo->m_nUserId					= pSeatInfo->GetUserId();
	pPlayerInfo->m_sUserName				= pSeatInfo->GetUserName();
	pDynamicInfo->AddPlayerInfo(pPlayerInfo, false);
	m_lockCashGames.Unlock();	// Unlock
	return true;
	}

bool
LobbyInfo::UnseatTablePlayer(int nTableId, int nUserId){
	m_lockCashGames.Lock(); // Lock
	LobbyCashTableDynamicInfo*	pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &nTableId);
	if( !pDynamicInfo ){
		m_lockCashGames.Unlock(); // Unlock
		return false;
		}
	bool bRet = pDynamicInfo->RemovePlayerInfoById(nUserId);
	// Empty cash table dynamic info. {{
	if( !pDynamicInfo->m_cPlayerCt && !pDynamicInfo->m_cWaitedPlayersCt ){
		m_listTableDynamicInfos.Remove(pDynamicInfo, true);
		}
	else{
		pDynamicInfo->m_cWaitedPlayersCt = 0;
		}
	// }}
	m_lockCashGames.Unlock(); // Unlock
	return bRet;
	}

bool
LobbyInfo::SetTableDynamicInfo(int nTableId, int nWaitedPlayersCt /*= -1*/, int nAvaragePotAmount /*= -1*/, int nHandPerHour /*= -1*/){
	m_lockCashGames.Lock(); // Lock
	LobbyCashTableDynamicInfo*	pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &nTableId);
	if( !pDynamicInfo ){
		m_lockCashGames.Unlock(); // Unlock
		return false;
		}
	if( nWaitedPlayersCt > -1 )
		pDynamicInfo->m_cWaitedPlayersCt = (char)nWaitedPlayersCt;
	if( nAvaragePotAmount > -1 )
		pDynamicInfo->m_nAvgPotAmount = nAvaragePotAmount;
	if( nHandPerHour > -1 )
		pDynamicInfo->m_shHandPerHour = (short)nHandPerHour;
	m_lockCashGames.Unlock(); // Unlock
	return true;
	}

bool
LobbyInfo::SetTableSeatAmount(int nTableId, int nSeat, INT_CHIPS nAmount){
	m_lockCashGames.Lock(); // Lock
	LobbyCashTableDynamicInfo*	pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &nTableId);
	if( !pDynamicInfo ){
		m_lockCashGames.Unlock(); // Unlock
		return false;
		}

	bool bRet = false;
	LobbyCashTablePlayerInfo* pPlayerInfo = pDynamicInfo->GetPlayerInfoBySeat(nSeat);
	if( pPlayerInfo ){
		pPlayerInfo->m_nSeatAmount = nAmount;
		bRet = true;
		}
	m_lockCashGames.Unlock(); // Unlock
	return bRet;
	}

bool
LobbyInfo::GetTableAttributes(int nTableId, int& nPlayersCt, int& nWaitingPlayersCt, float& fAvgPot, int& nHandPerHour){
	m_lockCashGames.Lock(); // Lock
	LobbyCashTableDynamicInfo*	pDynamicInfo = m_listTableDynamicInfos.FindOneVariable(_T("TableId"), &nTableId);
	if( !pDynamicInfo ){
		m_lockCashGames.Unlock(); // Unlock
		return false;
		}

	nPlayersCt			= (int)pDynamicInfo->m_cPlayerCt;
	nWaitingPlayersCt	= (int)pDynamicInfo->m_cWaitedPlayersCt;
	fAvgPot				= (float)(pDynamicInfo->m_nAvgPotAmount / 100.0f);
	nHandPerHour		= (int)pDynamicInfo->m_shHandPerHour;
	m_lockCashGames.Unlock(); // Unlock
	return true;
	}

//=================================================================================
//
//	Tournament lobby infos.
//

bool
LobbyInfo::SetTournamentInfo(TournamentInfo* pInfo){
	ASSERT(pInfo);
	if( !pInfo ) return false;

	m_lockTournaments.Lock(); // Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &pInfo->m_nId);
	if( !pTournamentInfo ){
		pTournamentInfo								= new LobbyTournamentInfo();
		pTournamentInfo->m_nTournamentId			= pInfo->m_nId;
		pTournamentInfo->m_sName					= pInfo->m_sName;
		pTournamentInfo->m_sDescription				= pInfo->m_sDescription;

		if( (pInfo->m_nSubTypeId & (int)GameSubType::Tournament_SitNGo) == 0 ){
			pTournamentInfo->m_dtTimeTournamentStart	= pInfo->m_dtStartTime;
			pTournamentInfo->m_dtTimeRegistrationStart	= pInfo->m_dtRegStartTime;
			pTournamentInfo->m_dtTimeRegistrationEnd	= pInfo->m_dtRegEndTime;
			}
		else{
			pTournamentInfo->m_dtTimeTournamentStart	.SetStatusNull();
			pTournamentInfo->m_dtTimeRegistrationStart	.SetStatusNull();
			pTournamentInfo->m_dtTimeRegistrationEnd	.SetStatusNull();
			}
		
		pTournamentInfo->m_dtTimeTournamentFinished	= pInfo->m_dtFinished;

		pTournamentInfo->m_cGameType				= (char)pInfo->m_nGameTypeId;
		pTournamentInfo->m_shSubType				= (short)pInfo->m_nSubTypeId;
		pTournamentInfo->m_cCurrencyId				= (char)pInfo->m_nCurrencyId;

		pTournamentInfo->m_shMinPlayers				= pInfo->m_nMinPlayers;
		pTournamentInfo->m_nMaxPlayers				= pInfo->m_nMaxPlayers;

		pTournamentInfo->m_shTableCt				= 0;
		pTournamentInfo->m_nPlayersCt				= 0;
		pTournamentInfo->m_nRegisteredCt			= pInfo->m_listParticipants.GetCount();

		pTournamentInfo->m_nRebuyAmount				= (int)Serializable::ConvertMoney(pInfo->m_dRebuy);
		pTournamentInfo->m_nAddonAmount				= (int)Serializable::ConvertMoney(pInfo->m_dAddon);
		pTournamentInfo->m_shAddonChips				= pInfo->m_nAddonChips;
		pTournamentInfo->m_shRebuyChips				= pInfo->m_nRebuyChips;
		pTournamentInfo->m_cRebuyCt					= ((char)pInfo->m_nRebuysCt);
		pTournamentInfo->m_cAddonCt					= ((char)pInfo->m_nAddonsCt);
		pTournamentInfo->m_shStartingChips			= ((short)pInfo->m_nBuyInChips);

		pTournamentInfo->m_status					= (TournamentStatus)pInfo->m_nStatus;
		pTournamentInfo->m_bRegistrationIsOpen		= ((pTournamentInfo->m_status == TournamentStatus::Tournament_Registering) ? true : false);
		pTournamentInfo->m_bAllowedToUseVPPoints	= pInfo->m_bAllowToUseVPPoints;

		pTournamentInfo->m_nBuyinAmount				= (int)Serializable::ConvertMoney(pInfo->m_dBuyIn);
		pTournamentInfo->m_nFeeAmount				= (int)Serializable::ConvertMoney(pInfo->m_dBuyInFee);
		pTournamentInfo->m_nPrizeAmount				= (int)Serializable::ConvertMoney(pInfo->m_dPrizeAmount);

		pTournamentInfo->m_nSatelliteToId			= pInfo->m_nSatelliteToId;
		if( pTournamentInfo->m_nSatelliteToId > 0 ){
			LobbyTournamentInfo* pSatelliteTo = m_listTournaments.FindOneVariable(_T("TournamentId"), &pTournamentInfo->m_nSatelliteToId);
			if( pSatelliteTo ){
				pTournamentInfo->m_sSatelliteToName		= pSatelliteTo->m_sName;
				pTournamentInfo->m_dtSatelliteToStart	= pSatelliteTo->m_dtTimeTournamentStart;
				}
			else
				pTournamentInfo->m_dtSatelliteToStart.SetStatusNull();
			}
		else
			pTournamentInfo->m_dtSatelliteToStart.SetStatusNull();

		m_listTournaments.Add(pTournamentInfo);

//		__int64 nValue = (((__int64)pTournamentInfo->m_nGameType) << 32) | ((int)pTournamentInfo->m_nSubType);
		// Exclude tournament type bits. // ZqrTalent
		__int64 nValue = (((__int64)GameType::Tournament) << 32) | ((int)(pTournamentInfo->m_shSubType & (GameSubType::Tournament_All)));
		m_arrLobbyTournamentInfoByGameType.Add(nValue, (__int64)pTournamentInfo);

		m_nTournamentsCt ++; // Increase tournament count.
		}
	else{
		}

	// Copy tournament participants. {{
	int nLoop = 0;
	pTournamentInfo->m_listParticipants.RemoveAll(true);
	while( nLoop < pInfo->m_listParticipants.GetCount() ){
		TournamentParticipant* pParticipant = pInfo->m_listParticipants.GetAt(nLoop);
		if( !pParticipant ){
			nLoop ++;
			continue;
			}

		TournamentPlayerInfo* pPlayerInfo	= new TournamentPlayerInfo();
		if( pParticipant->m_bIsActive ){
			pPlayerInfo->m_nChips			= (int)(pParticipant->m_dAmountChips*100);
			pPlayerInfo->m_shPlace			= -1;
			pPlayerInfo->m_nOrder			= (pPlayerInfo->m_nChips);
			}
		else{
			pPlayerInfo->m_nChips			= (int)(pParticipant->m_dPrizeWon*100);
			pPlayerInfo->m_shPlace			= (short)pParticipant->m_nPlace;
			pPlayerInfo->m_nOrder			= (-1*pParticipant->m_nPlace);
			}

		pPlayerInfo->m_cLevel				= (char)0;
		pPlayerInfo->m_nUserId				= pParticipant->m_nUserId;
		pPlayerInfo->m_sUserName			= pParticipant->m_sUserName;
		pTournamentInfo->m_listParticipants.Add(pPlayerInfo);
		nLoop ++;
		}
/*
	// Copy tournament game tables. {{
	pTournamentInfo->m_listTables.RemoveAll(true);
	nLoop = 0;
	while( nLoop < pInfo->m_listGameTables.GetCount() ){
		TournamentGameTable* pTable = pInfo->m_listGameTables.GetAt(nLoop);
		if( !pTable ){
			nLoop ++;
			continue;
			}
		TournamentTableInfo* pTableInfo = new TournamentTableInfo();
		pTableInfo->m_nTableId			= 0;
		pTableInfo->m_cPlayerCt			= (char)0;
		pTableInfo->m_nStakeLarge		= 0;
		pTableInfo->m_nStakeSmall		= 0;

		pTournamentInfo->m_listTables.Add(pTableInfo);
		nLoop ++;
		}
	// }}
*/
	// Copy blind structrure info. {{
	pTournamentInfo->m_listBlindStructure.RemoveAll(true);
	nLoop = 0;

	LobbyTournamentBlindStructureInfo* pLobbyBlindStruct	= new LobbyTournamentBlindStructureInfo();
	pLobbyBlindStruct->m_cLevelDuarationMin					= (char)pInfo->m_shLevelDuration;
	pLobbyBlindStruct->m_cBreakDurationMin					= (char)pInfo->m_shBreakDurationMin;
	pLobbyBlindStruct->m_cBreakPerLevel						= (char)pInfo->m_shBreakPerLevel;
	pLobbyBlindStruct->m_cAddonBreakIndex					= (char)pInfo->m_shAddonBreak;
	pLobbyBlindStruct->m_cRebuyCt							= (char)pInfo->m_nRebuysCt;
	pLobbyBlindStruct->m_cAddonCt							= (char)pInfo->m_nAddonsCt;
	pTournamentInfo->m_listBlindStructure.Add(pLobbyBlindStruct);

	while(nLoop < pInfo->m_listBlindStructure.GetCount() ){
		TournamentBlindStructure* pBlindInfo = pInfo->m_listBlindStructure.GetAt(nLoop);
		if( !pBlindInfo ){
			nLoop ++;
			continue;
			}
		LobbyTournamentLevelInfo* pNewLevel = new LobbyTournamentLevelInfo();
		pNewLevel->m_cLevelIndex			= ((char)pLobbyBlindStruct->m_listLevels.GetCount());
		pNewLevel->m_nSmallBlind			= ((int)pBlindInfo->m_dSmallBlind);
		pNewLevel->m_nBigBlind				= ((int)pBlindInfo->m_dBigBlind);
		pNewLevel->m_nAnte					= ((int)pBlindInfo->m_dAnte);
		pLobbyBlindStruct->m_listLevels.Add(pNewLevel);
		nLoop ++;
		}
	// }}

	// Rebuild tournament payouts.
	RebuildTournamentPayouts(pInfo);

	pTournamentInfo->m_nRegisteredCt		= pInfo->m_listParticipants.GetCount();
	pTournamentInfo->m_nPlayersCt			= 0;
	pTournamentInfo->m_shTableCt			= pInfo->m_listGameTables.GetCount();

	m_lockTournaments.Unlock(); // Unlock
	return true;
	}

bool
LobbyInfo::SetTournamentTable(int nTournamentId, PokerTableInfo* pTableInfo){
	ASSERT(pTableInfo);
	if( !pTableInfo ) return false;

	m_lockTournaments.Lock(); // Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	TournamentTableInfo* pTournamentTableInfo = pTournamentInfo->m_listTables.FindOneVariable(_T("TableId"), &pTableInfo->m_nTableId);
	if( !pTournamentTableInfo ){
		pTournamentTableInfo				= new TournamentTableInfo();
		pTournamentTableInfo->m_nTableId	= pTableInfo->m_nTableId;
		pTournamentInfo->m_listTables.Add(pTournamentTableInfo);

		// Increase tournament tables count.
		pTournamentInfo->m_shTableCt		++;
		}
	else{
		}

	pTournamentTableInfo->m_cPlayerCt	= 0;
	pTournamentTableInfo->m_nStakeLarge = 0;
	pTournamentTableInfo->m_nStakeSmall = 0;
	pTournamentTableInfo->m_nTableId	= pTableInfo->m_nTableId;
	pTournamentTableInfo->m_nStakeSmall = MAXINT;
	pTournamentTableInfo->m_nStakeLarge = 0;

	pTournamentTableInfo->m_players.RemoveAll(true);
	EnumerableObject<TournamentPlayerInfo>* pPlayers = &pTournamentTableInfo->m_players;
	for(int i=0; i<pTableInfo->m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = pTableInfo->m_seats[i];
		if( pSeat && pSeat->GetBalance() > 0 ){
			TournamentPlayerInfo* pPlayer	= new TournamentPlayerInfo();
			pPlayer->m_nChips				= pSeat->GetBalance();
			pPlayer->m_nUserId				= pSeat->GetUserId();
			pPlayer->m_sUserName			= pSeat->GetUserName();
			pPlayer->m_cLevel				= 0;
			pPlayer->m_shPlace				= 0;
			pPlayer->m_nOrder				= 0; // Useless because there is no need to order game table players.
			pPlayers->Add(pPlayer);

			// Set participant balance.
			pTournamentInfo->SetParticipantChipsAmountAndPlace(pPlayer->m_nUserId, pPlayer->m_nChips, -1);
			// Update table chips stat.
			pTournamentTableInfo->m_nStakeSmall = min(pTournamentTableInfo->m_nStakeSmall, pPlayer->m_nChips);
			pTournamentTableInfo->m_nStakeLarge = max(pTournamentTableInfo->m_nStakeLarge, pPlayer->m_nChips);
			pTournamentTableInfo->m_cPlayerCt	++;
			}
		}

	if( pTournamentTableInfo->m_nStakeSmall == MAXINT )
		pTournamentTableInfo->m_nStakeSmall = 0;

	m_lockTournaments.Unlock(); // Unlock
	return true;
	}

bool
LobbyInfo::SetTournamentParticipant(int nTournamentId, TournamentPlayerInfo* pPlayerInfo){
	if( !pPlayerInfo ) return false;
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	bool bRet = false;
	if( !pTournamentInfo->m_listParticipants.FindOneVariable(_T("UserId"), &pPlayerInfo->m_nUserId) ){
		TournamentPlayerInfo* pNew	= new TournamentPlayerInfo();
		pPlayerInfo->m_nChips		= (pTournamentInfo->m_shStartingChips*100);

		pPlayerInfo->Copy(pNew);
		pTournamentInfo->m_listParticipants.Add(pNew);
		// Update registered players count.
		pTournamentInfo->m_nRegisteredCt = pTournamentInfo->m_listParticipants.GetCount();
		// Update players count.
		if( pTournamentInfo->m_status == TournamentStatus::Tournament_InProgress )
			pTournamentInfo->m_nPlayersCt ++;
		bRet = true;
		} 
	m_lockTournaments.Unlock();		// Unlock
	return bRet;
	}

bool
LobbyInfo::SetTournamentStatus(int nTournamentId, TournamentStatus statusNew){
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}
	if( pTournamentInfo->m_status != statusNew ){
		pTournamentInfo->m_status = statusNew;

		// Change tournament registration status.
		if( pTournamentInfo->m_status == TournamentStatus::Tournament_Completed || 
			pTournamentInfo->m_status == TournamentStatus::Tournament_Canceled ){
			pTournamentInfo->m_bRegistrationIsOpen = false;
			}
		}
	m_lockTournaments.Unlock();		// Unlock
	return true;
	}

bool
LobbyInfo::SetTournamentPlayersCt(int nTournamentId, int nPlayersCt, int nRegistrantsCt){
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}
	pTournamentInfo->m_nPlayersCt		= nPlayersCt;
	pTournamentInfo->m_nRegisteredCt	= nRegistrantsCt;
	m_lockTournaments.Unlock();		// Unlock
	return true;
	}

bool
LobbyInfo::PlayerFinishedTournament(int nTournamentId, int nUserId, int nPlace, INT_CHIPS nWinAmount){
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}
	// 
	pTournamentInfo->m_nPlayersCt --;
	// Set participant balance.
	pTournamentInfo->SetParticipantChipsAmountAndPlace(nUserId, nWinAmount, nPlace);
	m_lockTournaments.Unlock();		// Unlock
	return true;
	}

int
LobbyInfo::GetPlayersPlaceInTournament(int nTournamentId, int nUserId){
	if( nTournamentId <= 0 || nUserId <= 0 )
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo || 
		pTournamentInfo->m_status < TournamentStatus::Tournament_InProgress ||
		pTournamentInfo->m_status == TournamentStatus::Tournament_Canceled ||
		pTournamentInfo->m_status == TournamentStatus::Tournament_Completed ){
		m_lockTournaments.Unlock(); // Unlock
		return -1;
		}

	int nResult = pTournamentInfo->GetPlayersPlace(nUserId);
	m_lockTournaments.Unlock();		// Unlock
	return nResult;
	}

bool
LobbyInfo::SetTournamentRegistrationStatus(int nTournamentId, bool bOpenStatus){
	if( nTournamentId <= 0 ) return false;
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}
	pTournamentInfo->m_bRegistrationIsOpen = bOpenStatus;
	m_lockTournaments.Unlock();		// Unlock
	return true;
	}

bool
LobbyInfo::OnTournamentRebuy(TournamentInfo* pInfo, int nUserId){
	if( !pInfo || pInfo->m_nId <= 0 ) 
		return false;
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pLobbyInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &pInfo->m_nId);
	if( !pLobbyInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}
	LobbyTournamentPayoutInfo* pPayoutInfo = pLobbyInfo->m_listPayoutInfo.GetAt(0);
	if( pInfo ){
		pPayoutInfo->m_nRebuysCt	++;
		RebuildTournamentPayouts(pInfo);
		}
	m_lockTournaments.Unlock();		// Unlock
	return true;
	}

bool
LobbyInfo::OnTournamentAddon(TournamentInfo* pInfo, int nUserId){
	if( !pInfo || pInfo->m_nId <= 0 ) 
		return false;
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pLobbyInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &pInfo->m_nId);
	if( !pLobbyInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}
	LobbyTournamentPayoutInfo* pPayoutInfo = pLobbyInfo->m_listPayoutInfo.GetAt(0);
	if( pInfo ){
		pPayoutInfo->m_nAddonsCt	++;
		RebuildTournamentPayouts(pInfo);
		}
	m_lockTournaments.Unlock();		// Unlock
	return true;
	}

bool
LobbyInfo::FillCustomInfoPacket(PokerTournamentCustomInfoPacket* p){
	if( !p || p->m_nTournamentId <= 0 )
		return false;

	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &p->m_nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	int							nFlags			= p->m_nFlags;
	LobbyTournamentSummaryInfo* pSummaryInfo	= p->m_listResult.GetAt(0);
	if( !pSummaryInfo ){
		pSummaryInfo = new LobbyTournamentSummaryInfo();
		if( p->m_listResult.GetCount() > 0 )
			p->m_listResult.SetAt(0, pSummaryInfo, false);
		else
			p->m_listResult.Add(pSummaryInfo);
		}

	// Tournament info.
	if( nFlags&(PokerTournamentCustomInfoPacket::TournamentInfoCustom_TournamentInfo) ){
		LobbyTournamentInfo* pTournamentInfoCopy = new LobbyTournamentInfo();
		pTournamentInfo->Copy(pTournamentInfoCopy);
		pSummaryInfo->m_listInfo.Add(pTournamentInfoCopy);
		}

	// Blind structure info.
	if( nFlags&(PokerTournamentCustomInfoPacket::TournamentInfoCustom_BlindStructure) )
		pTournamentInfo->m_listBlindStructure.Copy_Into(&pSummaryInfo->m_listBlindStructure);

	// Payout (Prize) info.
	if( nFlags&(PokerTournamentCustomInfoPacket::TournamentInfoCustom_PrizeInfo) )
		pTournamentInfo->m_listPayoutInfo.Copy_Into(&pSummaryInfo->m_listPayoutInfo);

	int nLoop = 0, nCt = 0, nRealCt = 0;
	// Game tables info.
	if( nFlags&(PokerTournamentCustomInfoPacket::TournamentInfoCustom_GameTables) ){
		nLoop	= (int)p->m_shGameTableIndex;
		nCt		= ((int)p->m_shGameTableCt);
		nRealCt	= pTournamentInfo->m_listTables.GetCount();
		
		// Avoid item index problems when item count has changed eventually.
		if( (nLoop + nCt) > nRealCt ){
			nLoop = (nRealCt - nCt - nLoop);
			if( nLoop < 0 ){
				nLoop	= 0;
				nCt		= nRealCt;
				}
			}

		nCt						+= nLoop;
		p->m_shGameTableIndex	= (short)nLoop;
		p->m_shGameTableCt		= (short)(nCt - nLoop);
		while(nLoop < nCt){
			TournamentTableInfo* pTableInfo = pTournamentInfo->m_listTables.GetAt(nLoop);
			if( pTableInfo ){
				TournamentTableInfo* pNew	= new TournamentTableInfo();
				pNew->m_nTableId			= pTableInfo->m_nTableId;
				pNew->m_cPlayerCt			= pTableInfo->m_cPlayerCt;
				pNew->m_nStakeSmall			= pTableInfo->m_nStakeSmall;
				pNew->m_nStakeLarge			= pTableInfo->m_nStakeLarge;
				pNew->m_nTableId			= pTableInfo->m_nTableId;
				pSummaryInfo->m_listTables.Add(pNew);
				}
			else
				pSummaryInfo->m_listTables.Add(NULL);
			nLoop ++;
			}
		}

	// Game table players info.
	if( nFlags&(PokerTournamentCustomInfoPacket::TournamentInfoCustom_GameTablePlayers) ){
		nLoop	= (int)p->m_shGameTableIndexPlayers;
		TournamentTableInfo* pTableInfo = pTournamentInfo->m_listTables.GetAt(nLoop);
		if( pTableInfo )
			pTableInfo->m_players.Copy_Into(&pSummaryInfo->m_listTablePlayers);
		}

	// Tournament participants.
	if( nFlags&(PokerTournamentCustomInfoPacket::TournamentInfoCustom_Participants) ){
		nLoop	= (int)p->m_shParticipantIndex;
		nCt		= ((int)p->m_shParticipantCt);
		nRealCt	= pTournamentInfo->m_listParticipants.GetCount();
		
		// Avoid item index problems when item count is changing eventually.
		if( (nLoop + nCt) > nRealCt ){
			nLoop = (nRealCt - nCt - nLoop);
			if( nLoop < 0 ){
				nLoop	= 0;
				nCt		= nRealCt;
				}
			}

		nCt						+= nLoop;
		p->m_shParticipantIndex	= (short)nLoop;
		p->m_shParticipantCt	= (short)(nCt - nLoop);

		if( pTournamentInfo->m_status >= TournamentStatus::Tournament_InProgress ){
			// get list of participants ordered.
			pTournamentInfo->m_listParticipants.GetObjectsByVariableIndex(_T("Order"), nLoop, nCt, pSummaryInfo->m_listParticipants, true);
			}
		else{
			// get list of participants unordered.
			while(nLoop < nCt){
				TournamentPlayerInfo* pPlayerInfo = pTournamentInfo->m_listParticipants.GetAt(nLoop);
				if( pPlayerInfo ){
					TournamentPlayerInfo* pNew = new TournamentPlayerInfo();
					pPlayerInfo->Copy(pNew);
					pSummaryInfo->m_listParticipants.Add(pNew);
					}
				else
					pSummaryInfo->m_listParticipants.Add(NULL);
				nLoop ++;
				}
			}
		}
	m_lockTournaments.Unlock();		// Unlock
	return true;
	}

bool
LobbyInfo::RebuildTournamentPayouts(TournamentInfo* pInfo){
	if( !pInfo ) return false;
	m_lockTournaments.Lock();		// Lock
	LobbyTournamentInfo* pLobbyInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &pInfo->m_nId);
	if( !pLobbyInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	int		nEntryCt		= pInfo->m_listParticipants.GetCount();
	int		nPlacesPaid		= 0;
	double	dPrizePool		= pInfo->CalcPrizePool();//pInfo->m_dPrizeAmount;

	LobbyTournamentPayoutInfo* pPayoutInfo	= pLobbyInfo->m_listPayoutInfo.GetAt(0);
	if( !pPayoutInfo ){
		pLobbyInfo->m_listPayoutInfo.RemoveAll();
		pPayoutInfo = new LobbyTournamentPayoutInfo();
		pLobbyInfo->m_listPayoutInfo.Add(pPayoutInfo);
		}
	
	pPayoutInfo->m_nPrizePool	= (int)Serializable::ConvertMoney(dPrizePool);	
	pPayoutInfo->m_nAddonsCt	= pInfo->m_nAddonsCt;
	pPayoutInfo->m_nEntriesCt	= nEntryCt;
	pPayoutInfo->m_nRebuysCt	= pInfo->m_nRebuysCt;
	pPayoutInfo->m_shPlacesPaid = 0;
	//pPayoutInfo->m_listDetail.RemoveAll();

	if( nEntryCt > 0 ){
		// Sattelite tournament.
		if( (pInfo->m_nSubTypeId&GameSubType::Tournament_Satellite) > 0 ){
			nPlacesPaid	= max(pInfo->m_shGuaranteedTicketCt, (int)(dPrizePool / pInfo->m_dTicketPrice));
			if( nPlacesPaid > 0 ){
				int nAmountLessThanTicketPrice = (int)Serializable::ConvertMoney(max(0.0, (dPrizePool - (pInfo->m_dTicketPrice*nPlacesPaid))));
				pPayoutInfo->m_listDetail.SetCount((nAmountLessThanTicketPrice > 1 ? 2 : 1), true);

				// Ticket winners.
				LobbyTournamentPayoutDetail* pLobbyDetail = pPayoutInfo->m_listDetail[0];
				if( !pLobbyDetail ){
					pLobbyDetail = new LobbyTournamentPayoutDetail();
					pPayoutInfo->m_listDetail.SetAt(0, pLobbyDetail, false);
					}
				pLobbyDetail->m_shPlaceFirst	= 1;
				pLobbyDetail->m_shPlaceLast		= nPlacesPaid;
				pLobbyDetail->m_nAmount			= 0; // Tournament ticket.

				// Remnant money winner.
				if( nAmountLessThanTicketPrice > 1 ){
					LobbyTournamentPayoutDetail* pLobbyDetail = pPayoutInfo->m_listDetail[nPlacesPaid];
					if( !pLobbyDetail ){
						pLobbyDetail = new LobbyTournamentPayoutDetail();
						pPayoutInfo->m_listDetail.SetAt(nPlacesPaid, pLobbyDetail, false);
						}

					nPlacesPaid ++;
					pLobbyDetail->m_shPlaceFirst	= nPlacesPaid;
					pLobbyDetail->m_shPlaceLast		= nPlacesPaid;
					pLobbyDetail->m_nAmount			= (UINT)nAmountLessThanTicketPrice;
					}
				}
			}
		else{
			TournamentPayout* pPayout = pInfo->GetPayout(nEntryCt);
			if( pPayout ){
				pPayoutInfo->m_listDetail.SetCount(pPayout->m_listDetail.GetCount(), true);
				for(int j=0; j<pPayout->m_listDetail.GetCount(); j++){
					TournamentPayoutDetail* pDetail = pPayout->m_listDetail.GetAt(j);
					if( !pDetail ) continue;
					LobbyTournamentPayoutDetail* pLobbyDetail = pPayoutInfo->m_listDetail[j];
					if( !pLobbyDetail ){
						pLobbyDetail = new LobbyTournamentPayoutDetail();
						pPayoutInfo->m_listDetail.SetAt(j, pLobbyDetail, false);
						}
					pLobbyDetail->m_shPlaceFirst	= pDetail->m_nFinishingStart;
					pLobbyDetail->m_shPlaceLast		= pDetail->m_nFinishingEnd;
					pLobbyDetail->m_nAmount			= (int)Serializable::ConvertMoney(dPrizePool*pDetail->m_dPercentWin/100.0f);
					nPlacesPaid						= max(nPlacesPaid, pLobbyDetail->m_shPlaceLast);
					}
				}
			}
		}
	else
		pPayoutInfo->m_listDetail.RemoveAll();

	// Set paid places count.
	pPayoutInfo->m_shPlacesPaid = (short)nPlacesPaid;

	m_lockTournaments.Unlock(); // Unlock
	return true;
	}

bool
LobbyInfo::RemoveTournamentInfo(int nTournamentId){
	m_lockTournaments.Lock(); // Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	// Decrease tournament count.
	m_nTournamentsCt = max(--m_nTournamentsCt, 0); 
	// Delete tournament info object.
	m_listTournaments.Remove(pTournamentInfo, true);

	// Delete from specific array.
	bool bRet = false;
	for(int i=0; i<m_arrLobbyTournamentInfoByGameType.GetCount(); i++){
		if( ((LobbyTournamentInfo*)m_arrLobbyTournamentInfoByGameType.GetData(i)) == pTournamentInfo ){
			bRet = true;
			m_arrLobbyTournamentInfoByGameType.Delete(i);
			break;
			}
		}
	m_lockTournaments.Unlock(); // Unlock
	return bRet;
	}

bool
LobbyInfo::RemoveTournamentTable(int nTournamentId, int nTableId){
	m_lockTournaments.Lock(); // Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	TournamentTableInfo* pTournamentTableInfo = pTournamentInfo->m_listTables.FindOneVariable(_T("TableId"), &nTableId);
	if( !pTournamentTableInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	pTournamentInfo->m_listTables.Remove(pTournamentTableInfo, true);
	pTournamentInfo->m_shTableCt = (short)pTournamentInfo->m_listTables.GetCount();
	m_lockTournaments.Unlock(); // Unlock
	return true;
	}

bool
LobbyInfo::RemoveTournamentParticipant(int nTournamentId, int nUserId){
	m_lockTournaments.Lock(); // Lock
	LobbyTournamentInfo* pTournamentInfo = m_listTournaments.FindOneVariable(_T("TournamentId"), &nTournamentId);
	if( !pTournamentInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}

	TournamentPlayerInfo* pPlayerInfo = pTournamentInfo->m_listParticipants.FindOneVariable(_T("UserId"), &nUserId);
	if( !pPlayerInfo ){
		m_lockTournaments.Unlock(); // Unlock
		return false;
		}
	pTournamentInfo->m_listParticipants.Remove(pPlayerInfo, true);
	pTournamentInfo->m_nRegisteredCt  = pTournamentInfo->m_listParticipants.GetCount();
	m_lockTournaments.Unlock(); // Unlock
	return true;
	}