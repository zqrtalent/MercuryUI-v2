// LobbyCashTable.cpp : implementation file
//

#include "stdafx.h"
#include "LobbyCashTableInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
//
//	class LobbyCashTablePlayerInfo
//
LobbyCashTablePlayerInfo::LobbyCashTablePlayerInfo(){
	}

BEGIN_DECLARE_VAR(LobbyCashTablePlayerInfo, Serializable)
	DECLARE_VAR(_T("LobbyCashTablePlayerInfo"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),					_T(""),		VariableType::VT_Int32, offsetof(LobbyCashTablePlayerInfo,m_nUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),					_T(""),		VariableType::VT_String, offsetof(LobbyCashTablePlayerInfo,m_sUserName), false, false, false)
	DECLARE_VAR(_T("SeatAmount"),				_T(""),		VariableType::VT_Chips, offsetof(LobbyCashTablePlayerInfo,m_nSeatAmount), false, false, false)
	DECLARE_VAR(_T("VIPLevel"),					_T(""),		VariableType::VT_Int8, offsetof(LobbyCashTablePlayerInfo,m_cVIPLevel), false, false, false)
	DECLARE_VAR(_T("SeatIndex"),				_T(""),		VariableType::VT_Int8, offsetof(LobbyCashTablePlayerInfo,m_cSeatIndex), false, false, false)
END_DECLARE_VAR()

LobbyCashTablePlayerInfo::~LobbyCashTablePlayerInfo(){
	}

///////////////////////////////////////////////////////////
//
//	class LobbyCashTableStaticInfo
//
LobbyCashTableStaticInfo::LobbyCashTableStaticInfo(){
#ifndef _SERVER
	m_cPlayerCt = -1;
#endif
	}

BEGIN_DECLARE_VAR(LobbyCashTableStaticInfo, Serializable)
	DECLARE_VAR(_T("LobbyCashTableStaticInfo"),	_T(""),						VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),					_T("TABLE_ID"),				VariableType::VT_Int32, offsetof(LobbyCashTableStaticInfo,m_nTableId), false, false, false)
	DECLARE_VAR(_T("TableName"),				_T("TABLE_NAME"),			VariableType::VT_String, offsetof(LobbyCashTableStaticInfo,m_sTableName), false, false, false)
	DECLARE_VAR(_T("GameType"),					_T("GAME_TYPE_ID"),			VariableType::VT_Int8, offsetof(LobbyCashTableStaticInfo,m_cGameType), false, false, false)
	DECLARE_VAR(_T("GameSubType"),				_T("GAME_SUB_TYPE_ID"),		VariableType::VT_Int16, offsetof(LobbyCashTableStaticInfo,m_shGameSubType), false, false, false)
	DECLARE_VAR(_T("CurrencyId"),				_T("CURRENCY_ID"),			VariableType::VT_Int8, offsetof(LobbyCashTableStaticInfo,m_btCurrencyId), false, false, false)
	DECLARE_VAR(_T("SBAmount"),					_T("SBAMOUNT"),				VariableType::VT_Int32, offsetof(LobbyCashTableStaticInfo,m_nSBAmount), false, false, false)
	DECLARE_VAR(_T("BBAmount"),					_T("BBAMOUNT"),				VariableType::VT_Int32, offsetof(LobbyCashTableStaticInfo,m_nBBAmount), false, false, false)
	DECLARE_VAR(_T("AnteAmount"),				_T("ANTE_AMOUNT"),			VariableType::VT_Int32, offsetof(LobbyCashTableStaticInfo,m_nAnteAmount), false, false, false)
	DECLARE_VAR(_T("MaxPlayerCt"),				_T("PLAYER_CT_MAX"),		VariableType::VT_Int8, offsetof(LobbyCashTableStaticInfo,m_cMaxPlayerCt), false, false, false)
	DECLARE_VAR(_T("MinBuyIn"),					_T("MIN_BUYIN"),			VariableType::VT_Int32, offsetof(LobbyCashTableStaticInfo,m_nMinBuyIn), false, false, false)
	DECLARE_VAR(_T("MaxBuyIn"),					_T("MAX_BUYIN"),			VariableType::VT_Int32, offsetof(LobbyCashTableStaticInfo,m_nMaxBuyIn), false, false, false)
END_DECLARE_VAR()

LobbyCashTableStaticInfo::~LobbyCashTableStaticInfo(){
	}

///////////////////////////////////////////////////////////
//
//	class LobbyCashTableDynamicInfo
//
LobbyCashTableDynamicInfo::LobbyCashTableDynamicInfo(){
	}

BEGIN_DECLARE_VAR(LobbyCashTableDynamicInfo, Serializable)
	DECLARE_VAR(_T("LobbyCashTableDynamicInfo"),	_T(""),						VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("TableId"),						_T("TABLE_ID"),				VariableType::VT_Int32, offsetof(LobbyCashTableDynamicInfo,m_nTableId), false, false, false)
	DECLARE_VAR(_T("PlayerCt"),						_T("PLAYER_CT"),			VariableType::VT_Int8, offsetof(LobbyCashTableDynamicInfo,m_cPlayerCt), false, false, false)
	DECLARE_VAR(_T("PlayerFlopPercent"),			_T("PLAYER_FLOP"),			VariableType::VT_Int8, offsetof(LobbyCashTableDynamicInfo,m_cPlayerFlopPercent), false, false, false)
	DECLARE_VAR(_T("WaitedPlayersCt"),				_T("PLAYER_CT_WAITING"),	VariableType::VT_Int8, offsetof(LobbyCashTableDynamicInfo,m_cWaitedPlayersCt), false, false, false)
	DECLARE_VAR(_T("AvgPotAmount"),					_T("AVG_POT_AMOUNT"),		VariableType::VT_Chips, offsetof(LobbyCashTableDynamicInfo,m_nAvgPotAmount), false, false, false)
	DECLARE_VAR(_T("HandPerHour"),					_T("HAND_PER_HOUR"),		VariableType::VT_Int16, offsetof(LobbyCashTableDynamicInfo,m_shHandPerHour), false, false, false)
END_DECLARE_VAR()

LobbyCashTableDynamicInfo::~LobbyCashTableDynamicInfo(){
	}

void
LobbyCashTableDynamicInfo::AddPlayerInfo(LobbyCashTablePlayerInfo* pInfo, bool bCopyObject /*= false*/){
	if( !pInfo ) return;
	// Update existing info. {{
	int nLoop = 0, nCt = m_listPlayers.GetCount(), nUserId = pInfo->m_nUserId;

	// Copy object. {{
	if( bCopyObject ){
		LobbyCashTablePlayerInfo* pCopy = new LobbyCashTablePlayerInfo();
		pInfo->Copy(pCopy);
		}
	// }}

	while( nLoop < nCt ){
		LobbyCashTablePlayerInfo* p = m_listPlayers.GetAt(nLoop);
		if( p && p->m_nUserId == nUserId ){
			m_listPlayers.SetAt(nLoop, pInfo);
			return;
			}
		nLoop ++;
		}
	// }}

	if( pInfo->m_cSeatIndex == -1 )
		m_cWaitedPlayersCt ++;
	else
		m_cPlayerCt ++;

	m_listPlayers.Add(pInfo);
	}

bool
LobbyCashTableDynamicInfo::RemovePlayerInfoBySeat(int nSeat){
	int nLoop = 0, nCt = m_listPlayers.GetCount();
	while( nLoop < nCt ){
		LobbyCashTablePlayerInfo* pInfo = m_listPlayers.GetAt(nLoop);
		if( !pInfo ){
			nLoop ++;
			continue;
			}
		if( pInfo->m_cSeatIndex == (char)nSeat ){
			if( pInfo->m_cSeatIndex == -1 )
				m_cWaitedPlayersCt = max(m_cWaitedPlayersCt-1, 0);
			else
				m_cPlayerCt = max(m_cPlayerCt-1, 0);
			return m_listPlayers.RemoveAt(nLoop);
			}
		nLoop ++;
		}
	return false;
	}

bool
LobbyCashTableDynamicInfo::RemovePlayerInfoById(int nUserId){
	int nLoop = 0, nCt = m_listPlayers.GetCount();
	while( nLoop < nCt ){
		LobbyCashTablePlayerInfo* pInfo = m_listPlayers.GetAt(nLoop);
		if( !pInfo ){
			nLoop ++;
			continue;
			}
		if( pInfo->m_nUserId == nUserId ){
			if( pInfo->m_cSeatIndex == -1 )
				m_cWaitedPlayersCt = max(--m_cWaitedPlayersCt, 0);
			else
				m_cPlayerCt = max(m_cPlayerCt-1, 0);
			return m_listPlayers.RemoveAt(nLoop);
			}
		nLoop ++;
		}
	return false;
	}

LobbyCashTablePlayerInfo*
LobbyCashTableDynamicInfo::GetPlayerInfo(int nUserId){
	int nLoop = 0, nCt = m_listPlayers.GetCount();
	while( nLoop < nCt ){
		LobbyCashTablePlayerInfo* pInfo = m_listPlayers.GetAt(nLoop);
		if( pInfo && pInfo->m_nUserId == nUserId )
			return m_listPlayers.GetAt(nLoop);
		nLoop ++;
		}
	return NULL;
	}

LobbyCashTablePlayerInfo*
LobbyCashTableDynamicInfo::GetPlayerInfoBySeat(int nSeat){
	int nLoop = 0, nCt = m_listPlayers.GetCount();
	while( nLoop < nCt ){
		LobbyCashTablePlayerInfo* pInfo = m_listPlayers.GetAt(nLoop);
		if( pInfo && pInfo->m_cSeatIndex == (char)nSeat )
			return m_listPlayers.GetAt(nLoop);
		nLoop ++;
		}
	return NULL;
	}