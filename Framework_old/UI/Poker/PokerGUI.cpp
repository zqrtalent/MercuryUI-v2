// PokerGUI.cpp : implementation file
//

#include "PokerGUI.h"
#include "..\..\Protocol\_PokerDefines.h"

//================================================
//
//	Static pokerhelper methods.
//
//================================================

std::string	
PokerHelper::GetCurrencyName(int nCurrencyId){
	const TCHAR* arrCurrencyNames[] = {_T("{{GEL}}"), _T("{{USD}}"), _T("{{EUR}}"), _T("{{PT}}")};
	if( nCurrencyId < 1 || nCurrencyId > sizeof(arrCurrencyNames)/sizeof(TCHAR*) )  
		return _T("");
	return std::string((TCHAR*)arrCurrencyNames[nCurrencyId - 1]);
	}

std::string
PokerHelper::GetFormattedAmount(int nCurrencyId, int nAmount){
	std::string sAmount;
	if( !(nAmount%100) ){
		if( nCurrencyId <= 0 )
			stringEx::Format(sAmount, _T("%d"), nAmount/100);
		else
			stringEx::Format(sAmount, _T("%d %s"), nAmount/100, GetCurrencyName(nCurrencyId).c_str());
		}
	else{
		if( nCurrencyId <= 0 )
			stringEx::Format(sAmount, _T("%d.%02d"), nAmount/100, nAmount%100);
		else
			stringEx::Format(sAmount, _T("%d.%02d %s"), nAmount/100, nAmount%100, GetCurrencyName(nCurrencyId).c_str());
		}
	return sAmount;
	}

std::string
PokerHelper::GetGameName(int nGameType, int nGameSubType){
	std::string sRet;
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
			break;
			}
		}
	/*
	if( bPlayMoney )
		sRet += _T(" ({{PlayMoney}})");*/
	return sRet;
	}

std::string
PokerHelper::GetGameNameEng(int nGameType, int nGameSubType){
	std::string sRet;
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
			return (int)(GameSubType::Tournament_Freeroll|GameSubType::Tournament_Guaranteed|GameSubType::Tournament_Regular);
			}
		}
	return 0;
	}

std::string
PokerHelper::GetTournamentStatus(int nStatus){
	const std::string sArrStatus[] = {_T("{{Announced}}"), _T("{{Seating}}"), _T("{{Registering}}"), _T("{{Starting}}"), _T("{{InProgress}}"), _T("{{Completed}}"), _T("{{Canceled}}")};
	if( nStatus < 0 || nStatus > sizeof(sArrStatus)/sizeof(std::string) )
		return _T("");
	return sArrStatus[nStatus - ((int)TournamentStatus::Tournament_Announced)];
	}

std::string
PokerHelper::GetTournamentDateAsString(DateTime dtTime){
	if( dtTime.GetStatus() != DateTime::Status::valid )
		return _T("");
	const std::string sArrMonth[] = {_T("{{Jan}}"), _T("{{Feb}}"), _T("{{Mar}}"), _T("{{Apr}}"), _T("{{May}}"), _T("{{Jun}}"), _T("{{Jul}}"), _T("{{Aug}}"), _T("{{Sep}}"), _T("{{Oct}}"), _T("{{Nov}}"), _T("{{Dec}}")};
	std::string sDate;
	stringEx::Format(sDate, _T("%02d %s %02d:%02d"), dtTime.GetDay(), sArrMonth[dtTime.GetMonth()-1].c_str(), dtTime.GetHour(), dtTime.GetMinute());
	return sDate;
	}

std::string
PokerHelper::GetTournamentBuyin(int nBuyinAmount, int nBuyinFeeAmount, int nCurrencyId){
	std::string sValue;
	if( nBuyinAmount > 0 ){
		if( nBuyinFeeAmount > 0 )
			stringEx::Format(sValue, _T("%d.%02d %s + %d.%02d %s"), nBuyinAmount/100, nBuyinAmount%100, GetCurrencyName(nCurrencyId).c_str(), nBuyinFeeAmount/100, nBuyinFeeAmount%100, GetCurrencyName(nCurrencyId).c_str());
		else
			stringEx::Format(sValue, _T("%d.%02d %s"), nBuyinAmount/100, nBuyinAmount%100, GetCurrencyName(nCurrencyId).c_str());
		}
	else
		sValue = _T("{{Freeroll}}");
	return sValue;
	}

std::string	
PokerHelper::CardNameByIndex(int nIndex){
	const std::string arrCardNum[] = {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	const std::string arrCardType[] = {_T("S"), _T("C"), _T("D"), _T("H")};
	if( nIndex < 0 || nIndex > 52 )
		return _T("");
	return std::string(arrCardNum[nIndex%13] + arrCardType[nIndex/13]);
	}