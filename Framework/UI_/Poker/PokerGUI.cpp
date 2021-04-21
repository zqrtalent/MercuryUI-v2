// PokerGUI.cpp : implementation file
//

#include "stdafx.h"
#include "PokerGUI.h"
#include "../../Protocol/_PokerDefines.h"

//================================================
//
//	Static pokerhelper methods.
//
//================================================

_String	
PokerHelper::GetCurrencyName(int nCurrencyId){
	const TCHAR* arrCurrencyNames[] = {_T("{{GEL}}"), _T("{{USD}}"), _T("{{EUR}}"), _T("{{PT}}")};
	if( nCurrencyId < 1 || nCurrencyId > sizeof(arrCurrencyNames)/sizeof(TCHAR*) )  
		return _T("");
	return CString((TCHAR*)arrCurrencyNames[nCurrencyId - 1]);
	}

_String	
PokerHelper::GetCurrencyNameEng(int nCurrencyId){
	const TCHAR* arrCurrencyNames[] = {_T("GEL"), _T("USD"), _T("EUR"), _T("PT")};
	if( nCurrencyId < 1 || nCurrencyId > sizeof(arrCurrencyNames)/sizeof(TCHAR*) )  
		return _T("");
	return CString((TCHAR*)arrCurrencyNames[nCurrencyId - 1]);
	}

_String
PokerHelper::GetFormattedAmount(int nCurrencyId, INT_CHIPS nAmount){
	_String sAmount;
	if( !(nAmount%100) ){
		if( nCurrencyId <= 0 )
			sAmount.Format(_T("%d"), (UINT)(nAmount/100));
		else
			sAmount.Format(_T("%d %s"), (UINT)(nAmount/100), GetCurrencyName(nCurrencyId).GetBuffer());
		}
	else{
		if( nCurrencyId <= 0 )
			sAmount.Format(_T("%d.%02d"), (UINT)(nAmount/100), (UINT)(nAmount%100));
		else
			sAmount.Format(_T("%d.%02d %s"), (UINT)(nAmount/100), (UINT)(nAmount%100), GetCurrencyName(nCurrencyId).GetBuffer());
		}
	return sAmount;
	}

_String
PokerHelper::GetGameName(int nGameType, int nGameSubType){
	_String sRet;
	bool bPlayMoney = ((nGameType&GameType::Playmoney) != 0);
	nGameType &= ~GameType::Playmoney;
	switch( nGameType ){
		case GameType::Omaha:{
			if( nGameSubType == Omaha_Omaha )
				sRet = _T("{{Omaha}}");
			else
			if( nGameSubType == Omaha_OmahaHiLo )
				sRet = _T("{{Omaha Hi/Lo}}");
			break;
			}
		case GameType::TexasHoldem:{
			if( nGameSubType == Holdem_NoLimit )
				sRet = _T("{{NL Hold'em}}");
			else
			if( nGameSubType == Holdem_FixedLimit )
				sRet = _T("{{FL Hold'em}}");
			else
			if( nGameSubType == Holdem_PotLimit )
				sRet = _T("{{PL Hold'em}}");
			break;
			}
		case GameType::Tournament:{
			if( nGameSubType == Tournament_Freeroll )
				sRet = _T("{{Freeroll}}");
			else
			if( nGameSubType == Tournament_Guaranteed )
				sRet = _T("{{Guaranteed}}");
			else
			if( nGameSubType == Tournament_Regular )
				sRet = _T("{{Regular}}");
			else
			if( nGameSubType == Tournament_Satellite )
				sRet = _T("{{Satellite}}");
			else
			if( nGameSubType == Tournament_SitNGo )
				sRet = _T("{{SitNGo}}");
			break;
			}
		}
	/*
	if( bPlayMoney )
		sRet += _T(" ({{PlayMoney}})");*/
	return sRet;
	}

_String
PokerHelper::GetGameNameEng(int nGameType, int nGameSubType){
	_String sRet;
	bool bPlayMoney = ((nGameType&GameType::Playmoney) != 0);
	nGameType &= ~GameType::Playmoney;
	switch( nGameType ){
		case GameType::Omaha:{
			if( nGameSubType == Omaha_Omaha )
				sRet = _T("Omaha");
			else
			if( nGameSubType == Omaha_OmahaHiLo )
				sRet = _T("Omaha Hi/Lo");
			break;
			}
		case GameType::TexasHoldem:{
			if( nGameSubType == Holdem_NoLimit )
				sRet = _T("NL Hold'em");
			else
			if( nGameSubType == Holdem_FixedLimit )
				sRet = _T("FL Hold'em");
			else
			if( nGameSubType == Holdem_PotLimit )
				sRet = _T("PL Hold'em");
			break;
			}
		case GameType::Tournament:{
			if( nGameSubType == Tournament_Freeroll )
				sRet = _T("Freeroll");
			else
			if( nGameSubType == Tournament_Guaranteed )
				sRet = _T("Guaranteed");
			else
			if( nGameSubType == Tournament_Regular )
				sRet = _T("Regular");
			else
			if( nGameSubType == Tournament_Satellite )
				sRet = _T("Satellite");
			else
			if( nGameSubType == Tournament_SitNGo )
				sRet = _T("Sit&Go");
			break;
			}
		}
	if( bPlayMoney )
		sRet += _T(" (Play Money)");
	return sRet;
	}

int
PokerHelper::GetSubTypesByGameType(int nGameType){
	switch( nGameType ){
		case GameType::Omaha:{
			return (int)(GameSubType::Omaha_Omaha|GameSubType::Omaha_OmahaHiLo);
			}
		case GameType::TexasHoldem:{
			return (int)(GameSubType::Holdem_NoLimit|GameSubType::Holdem_PotLimit|GameSubType::Holdem_FixedLimit);
			}
		case GameType::Tournament:{
			return (int)(GameSubType::Tournament_All);
			}
		}
	return 0;
	}

_String
PokerHelper::GetTournamentStatus(int nStatus){
	const _String sArrStatus[] = {_T("{{Announced}}"), _T("{{Seating}}"), _T("{{Registering}}"), _T("{{Starting}}"), _T("{{InProgress}}"), _T("{{Completed}}"), _T("{{Canceled}}")};
	if( nStatus < 0 || nStatus > sizeof(sArrStatus)/sizeof(CString) )
		return _T("");
	return sArrStatus[nStatus - ((int)TournamentStatus::Tournament_Announced)];
	}

_String
PokerHelper::GetTournamentDateAsString(DateTime dtTime){
	if( dtTime.GetStatus() == DateTime::Status::valid ){
		const TCHAR* sArrMonth[] = {_T("{{Jan}}"), _T("{{Feb}}"), _T("{{Mar}}"), _T("{{Apr}}"), _T("{{May}}"), _T("{{Jun}}"), _T("{{Jul}}"), _T("{{Aug}}"), _T("{{Sep}}"), _T("{{Oct}}"), _T("{{Nov}}"), _T("{{Dec}}")};
		_String sDate;
		sDate.Format(_T("%02d %s %02d:%02d"), dtTime.GetDay(), sArrMonth[dtTime.GetMonth()-1], dtTime.GetHour(), dtTime.GetMinute());
		return sDate;
		}
	return _T("");
	}

_String
PokerHelper::GetTournamentBuyin(int nBuyinAmount, int nBuyinFeeAmount, int nCurrencyId){
	_String sValue;
	if( nBuyinAmount > 0 ){
		if( nBuyinFeeAmount > 0 )
			sValue.Format(_T("%d.%02d%s+%d.%02d%s"), nBuyinAmount/100, nBuyinAmount%100, GetCurrencyName(nCurrencyId).GetBuffer(), nBuyinFeeAmount/100, nBuyinFeeAmount%100, GetCurrencyName(nCurrencyId).GetBuffer());
		else
			sValue.Format(_T("%d.%02d%s"), nBuyinAmount/100, nBuyinAmount%100, GetCurrencyName(nCurrencyId).GetBuffer());
		}
	else
		sValue = _T("{{Freeroll}}");
	return sValue;
	}

_String	
PokerHelper::CardNameByIndex(int nIndex){
	const _String arrCardNum[] = {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	const _String arrCardType[] = {_T("S"), _T("C"), _T("D"), _T("H")};
	if( nIndex < 0 || nIndex > 52 )
		return _T("");
	return CString(arrCardNum[nIndex%13] + arrCardType[nIndex/13]);
	}