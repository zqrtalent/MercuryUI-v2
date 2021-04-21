// PokerTableSeatInfo.cpp : implementation file
//

#include "stdafx.h"
#ifdef _SERVER
#include "../../GameServer/ClientConnection.h"
#include "../../GameServer/Session.h"
#endif
#include "PokerTableSeatInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerTableSeatInfo::PokerTableSeatInfo(){
	m_pSes				= NULL;
	m_nUserId			= 0;
	m_nAvatarId			= 0;
	m_cSeatFlags		= 0;
//	m_bOffline			= false;
	m_cSeat				= (char)-1;
	m_nBalance			= 0;
	m_nAddedAmount		= 0;
	m_nPotAmount		= 0;
	m_nPotAmountTotal	= 0;
	m_seatStatus		= PokerSeatStatus::Empty;
	m_lastAction		= PokerActionTypes::NoAction;
//	m_pAvatarImage		= NULL;
	m_cCard1			= (char)-1;
	m_cCard2			= (char)-1;
	m_cCard3			= (char)-1;
	m_cCard4			= (char)-1;
	m_cRankLevel		= 0;
//	m_bTimerActive		= false;
	m_cTimeBankSec		= 0;
	SetAutoPostBlind(true);
	SetAutoMuck		(true);
//	m_bAutoPostBlind	= true;
//	m_bAutoMuck			= true;
//	m_bSitOutNextHand	= false;
//	m_bSitOutNextBB		= false;
	m_bWaitingBB		= false;
	m_cHandCardsClass	= 0;
	m_fTimeBankSec		= 0.0f;
	m_bCardsShown		= false;
	m_dwIpAddress		= m_dwIpAddress;
	FalsePreActions();

	m_bWonState			= 0;
	m_nHandsDealed		= 0;
	m_nHandsPlayed		= 0;
	m_nHandsWon			= 0;
	m_nHandsWonPreflop	= 0;
	m_nHandsWonPostflop	= 0;
	m_nHandsWonShowdown	= 0;

#ifdef _SERVER
	m_nUserPlaceIndex	= -1;
	m_nTableId			= 0;
	m_nTournamentId		= 0;
	m_bSitOutButInPlay	= false;
	m_bMissedSB			= false;
	m_bMissedBB			= false;
	m_statusBeforeSitout= PokerSeatStatus::Empty;
#endif
	}

BEGIN_DECLARE_VAR(PokerTableSeatInfo, Serializable)
	DECLARE_VAR(_T("PokerTableSeatInfo"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserName"),				_T(""),		VariableType::VT_String, offsetof(PokerTableSeatInfo,m_sUserName), false, false, false)
	DECLARE_VAR(_T("UserId"),				_T(""),		VariableType::VT_Int32, offsetof(PokerTableSeatInfo,m_nUserId), false, false, false)
	DECLARE_VAR(_T("AvatarId"),				_T(""),		VariableType::VT_Int32, offsetof(PokerTableSeatInfo,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("Flags"),				_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cSeatFlags), false, false, false)
//	DECLARE_VAR(_T("Offline"),				_T(""),		VariableType::VT_Bool, offsetof(PokerTableSeatInfo,m_bOffline), false, false, false)
	DECLARE_VAR(_T("Seat"),					_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cSeat), false, false, false)
	DECLARE_VAR(_T("Balance"),				_T(""),		VariableType::VT_Chips, offsetof(PokerTableSeatInfo,m_nBalance), false, false, false)
	DECLARE_VAR(_T("PotAmount"),			_T(""),		VariableType::VT_Chips, offsetof(PokerTableSeatInfo,m_nPotAmount), false, false, false)
	DECLARE_VAR(_T("PotAmountTotal"),		_T(""),		VariableType::VT_Chips, offsetof(PokerTableSeatInfo,m_nPotAmountTotal), false, false, false)
	DECLARE_VAR(_T("Card1"),				_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cCard1), false, false, false)
	DECLARE_VAR(_T("Card2"),				_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cCard2), false, false, false)
	DECLARE_VAR(_T("Card3"),				_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cCard3), false, false, false)
	DECLARE_VAR(_T("Card4"),				_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cCard4), false, false, false)
	DECLARE_VAR(_T("RankLevel"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cRankLevel), false, false, false)
	DECLARE_VAR(_T("ActionType"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_lastAction), false, false, false)
//	DECLARE_VAR(_T("TimerActive"),			_T(""),		VariableType::VT_Bool, offsetof(PokerTableSeatInfo,m_bTimerActive), false, false, false)
	DECLARE_VAR(_T("TimerSeconds"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cTimerSeconds), false, false, false)
	DECLARE_VAR(_T("SecondsPassed"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cSecondsPassed), false, false, false)
	DECLARE_VAR(_T("TimeBankSec"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cTimeBankSec), false, false, false)	
	DECLARE_VAR(_T("seatStatus"),			_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_seatStatus), false, false, false)
//	DECLARE_VAR(_T("AutoPostBlind"),		_T(""),		VariableType::VT_Bool, offsetof(PokerTableSeatInfo,m_bAutoPostBlind), false, false, false)
//	DECLARE_VAR(_T("AutoMuck"),				_T(""),		VariableType::VT_Bool, offsetof(PokerTableSeatInfo,m_bAutoMuck), false, false, false)
//	DECLARE_VAR(_T("SitOutNextHand"),		_T(""),		VariableType::VT_Bool, offsetof(PokerTableSeatInfo,m_bSitOutNextHand), false, false, false)
//	DECLARE_VAR(_T("SitOutNextBB"),			_T(""),		VariableType::VT_Bool, offsetof(PokerTableSeatInfo,m_bSitOutNextBB), false, false, false)	
	DECLARE_VAR(_T("HandCardsClass"),		_T(""),		VariableType::VT_Int8, offsetof(PokerTableSeatInfo,m_cHandCardsClass), false, false, false)
	DECLARE_VAR(_T("IpAddress"),			_T(""),		VariableType::VT_Int32, offsetof(PokerTableSeatInfo,m_dwIpAddress), false, false, false)
END_DECLARE_VAR()

PokerTableSeatInfo::~PokerTableSeatInfo(){
	SetSession(NULL, 0, 0);
	/*
	if( m_pAvatarImage ) {
		delete m_pAvatarImage;
		m_pAvatarImage = NULL;
		}*/
	}

bool
PokerTableSeatInfo::Copy(Serializable* pInto){
	bool bRet = Serializable::Copy(pInto);
#ifdef _SERVER
	if( bRet ){

		PokerTableSeatInfo* pSeatInfoInto = (PokerTableSeatInfo*)pInto;
		pSeatInfoInto->SetSession(m_pSes, m_nTableId, m_nTournamentId);
		// Copy offline flag.
//		pSeatInfoInto->m_bOffline = m_bOffline;
		}
#endif
	return bRet;
	}

#ifdef _SERVER
BOOL
PokerTableSeatInfo::Serialize(GrowableMemory* pMem, AutoSortedArray* pArrVarOffsets /*= NULL*/){
	int nTempCard1 = m_cCard1, nTempCard2 = m_cCard2, nTempCard3 = m_cCard3, nTempCard4 = m_cCard4;
	// Note: Hide cards if user is permited to see that.
	if( !GetConnection() || (!GetConnection()->GetSendingStatus() && !m_bCardsShown) || IsOffline() ){
		if( m_cCard1 > -1 )
			m_cCard1 = -2; // Card cover would be visible.
		if( m_cCard2 > -1 )
			m_cCard2 = -2; // Card cover would be visible.
		if( m_cCard3 > -1 )
			m_cCard3 = -2; // Card cover would be visible.
		if( m_cCard4 > -1 )
			m_cCard4 = -2; // Card cover would be visible.
		}

	BOOL bRet = Serializable::Serialize(pMem, pArrVarOffsets);
	m_cCard1 = nTempCard1;
	m_cCard2 = nTempCard2;
	m_cCard3 = nTempCard3;
	m_cCard4 = nTempCard4;
	return bRet;
	}

Session*
PokerTableSeatInfo::GetSession(bool bRetain) {
	if( m_pSes && bRetain )
		return (Session*)SafePointer::Retain(m_pSes);
	return m_pSes;
	}

void
PokerTableSeatInfo::SetSession(Session* pSes, int nTableId, int nTournamentId){
	if( m_pSes == pSes ) return;
	if( m_pSes )
		SafePointer::Release(m_pSes);
	if( pSes ){
		pSes = (Session*)SafePointer::Retain(pSes);
		}

	m_nTableId		= nTableId;
	m_nTournamentId = nTournamentId;
	m_pSes			= pSes;
	}

ClientConnection*
PokerTableSeatInfo::GetConnection(){
	if( IsOffline() ) 
		return NULL;
	if( m_nTableId > 0 )
		return (m_pSes ? m_pSes->GetTableConnection(m_nTableId, m_nTournamentId, false, false) : NULL);
	return (m_pSes ? m_pSes->GetConnection(false, false) : NULL);
	}
/*
ClientConnection*
PokerTableSeatInfo::GetTableConnection(int nTableId, int nTournamentId){
	if( IsOffline() ) return NULL;
	return (m_pSes ? m_pSes->GetTableConnection(nTableId, nTournamentId, false, false) : NULL);
	}*/
#else

BOOL
PokerTableSeatInfo::Serialize(GrowableMemory* pMem, AutoSortedArray* pArrVarOffsets /*= NULL*/){
	BOOL bRet = Serializable::Serialize(pMem, pArrVarOffsets);
	return bRet;
	}

ClientConnection*
PokerTableSeatInfo::GetConnection(){
	return NULL;
	}

Session*
PokerTableSeatInfo::GetSession(bool bRetain) {
	return NULL;
	}

void
PokerTableSeatInfo::SetSession(Session* pSes, int nTableId, int nTournamentId){
	}
#endif

bool
PokerTableSeatInfo::SetAvatarData(LongBinary* pBinImage){
	/*
	if( !pBinImage || pBinImage->m_hData == NULL ){
		if( m_pAvatarImage )
			delete m_pAvatarImage;
		m_pAvatarImage = NULL;
		return true;
		}

	Image* pAvatar = new Image();
	int nImageDataSize = 0;
	if( !Serializable::ImageFromBinary(pBinImage, pAvatar, nImageDataSize) ){
		delete pAvatar;
		return false;
		}

	if( !m_pAvatarImage ){
		m_pAvatarImage = new MipmapImage();
		}
	m_pAvatarImage->SetImage(pAvatar);
	delete pAvatar;
	return true;*/
	return false;
	}

void
PokerTableSeatInfo::SetupTimer(int nFullTimeSec, int nSecPassed){
	m_cTimerSeconds		= (char)nFullTimeSec;
	m_cSecondsPassed	= (char)nSecPassed;
	}

bool
PokerTableSeatInfo::IncreaseTimer(){
	if( m_cSecondsPassed < m_cTimerSeconds ){
		m_cSecondsPassed ++;
		return true;
		}
	return false;
	}