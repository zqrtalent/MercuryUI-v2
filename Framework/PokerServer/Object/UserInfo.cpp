// UserInfo.cpp : implementation file
//

#include "stdafx.h"
#include "../../GameServer/Session.h"
#include "UserInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UserInfo::UserInfo(){
	ZeroInit();
	m_pAvatarInfo				= NULL;
	m_pSession					= NULL;
	m_nAvatarId					= 0;
	m_bDestroyAvatarInfo		= true;
	m_bAuthenticatedBySessionId	= false;
	m_bChatIsBlocked			= false;
	m_bAvatarIsBlocked			= false;
	}

BEGIN_DECLARE_VAR(UserInfo, Serializable)
	DECLARE_VAR(_T("UserInfo"),					_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("UserId"),					_T(""),		VariableType::VT_Int32, offsetof(UserInfo,m_nUserId), false, false, false)
	DECLARE_VAR(_T("UserName"),					_T(""),		VariableType::VT_String, offsetof(UserInfo,m_sUserName), false, false, false)
	DECLARE_VAR(_T("AvatarId"),					_T(""),		VariableType::VT_Int32, offsetof(UserInfo,m_nAvatarId), false, false, false)
	DECLARE_VAR(_T("AvatarFileName"),			_T(""),		VariableType::VT_String, offsetof(UserInfo,m_sAvatarFileName), false, false, false)
	DECLARE_VAR(_T("BalanceAmount"),			_T(""),		VariableType::VT_Chips, offsetof(UserInfo,m_nBalanceAmount), false, false, false)
	DECLARE_VAR(_T("InPlayAmount"),				_T(""),		VariableType::VT_Chips, offsetof(UserInfo,m_nInPlayAmount), false, false, false)
	DECLARE_VAR(_T("PlayBalance"),				_T(""),		VariableType::VT_Chips, offsetof(UserInfo,m_nPlayBalance), false, false, false)
	DECLARE_VAR(_T("InPlayPlayBalance"),		_T(""),		VariableType::VT_Chips, offsetof(UserInfo,m_nInPlayPlayBalance), false, false, false)	
	DECLARE_VAR(_T("AuthenticatedBySessionId"),	_T(""),		VariableType::VT_Bool, offsetof(UserInfo,m_bAuthenticatedBySessionId), false, false, false)	
	DECLARE_VAR(_T("VPPoints"),					_T(""),		VariableType::VT_Double, offsetof(UserInfo,m_dVPPoints), false, false, false)
	DECLARE_VAR(_T("RankLevel"),				_T(""),		VariableType::VT_Int16, offsetof(UserInfo,m_shRankLevel), false, false, false)
	DECLARE_VAR(_T("Tickets"),					_T(""),		VariableType::VT_None, offsetof(UserInfo,m_listTickets), false, true, true)
	DECLARE_VAR(_T("TournamentDetails"),		_T(""),		VariableType::VT_None, offsetof(UserInfo,m_listTournamentDetails), false, true, true)
	DECLARE_VAR(_T("PermissionId"),				_T(""),		VariableType::VT_Int32, offsetof(UserInfo,m_nPermissionId), false, false, false)
	DECLARE_VAR(_T("Blocked"),					_T(""),		VariableType::VT_Bool, offsetof(UserInfo,m_bBlocked), false, false, false)
	DECLARE_VAR(_T("ChatIsBlocked"),			_T(""),		VariableType::VT_Bool, offsetof(UserInfo,m_bChatIsBlocked), false, false, false)
	DECLARE_VAR(_T("AvatarIsBlocked"),			_T(""),		VariableType::VT_Bool, offsetof(UserInfo,m_bAvatarIsBlocked), false, false, false)
END_DECLARE_VAR()

UserInfo::~UserInfo(){
	m_pSession = NULL;
	if( m_pAvatarInfo ){
		if( m_bDestroyAvatarInfo )
			delete m_pAvatarInfo;
		m_pAvatarInfo = NULL;
		}
	}
/*
Session*
UserInfo::GetSession(bool bRetain){ 
	if( m_pSession && bRetain )
		return (Session*)SafePointer::Retain(m_pSession);
	return m_pSession; 
	}

void
UserInfo::SetSession(Session* pSess){
	m_pSession = pSess; 
	}*/

bool
UserInfo::SetAvatarInfo(PokerAvatar* pAvatarInfo, bool bCopy){
	if( !pAvatarInfo )
		return false;

	if( m_pAvatarInfo ){
		delete m_pAvatarInfo;
		m_pAvatarInfo = NULL;
		}

	if( bCopy ){
		m_pAvatarInfo = new PokerAvatar();
		m_pAvatarInfo->m_nId				= pAvatarInfo->m_nId;
		m_pAvatarInfo->m_nPrivateUserId		= pAvatarInfo->m_nPrivateUserId;
		m_pAvatarInfo->m_recStatus			= 0;
		m_pAvatarInfo->m_sAvatarFileName	= pAvatarInfo->m_sAvatarFileName;

		if( pAvatarInfo->m_binImage.GetBinarySize() > 0){
			m_pAvatarInfo->m_binImage.SetBinarySize(pAvatarInfo->m_binImage.GetBinarySize(), pAvatarInfo->m_binImage.LockMemory());
			pAvatarInfo->m_binImage.UnlockMemory();
			}
		else
			m_pAvatarInfo->m_binImage.SetBinarySize(0);
		m_bDestroyAvatarInfo = true;
		}
	else{
		m_pAvatarInfo = pAvatarInfo;
		m_bDestroyAvatarInfo = false;
		}
	m_nAvatarId = pAvatarInfo->m_nId;
	return true;
	}

bool
UserInfo::UpdateAvatarInfo(int nAvatarId, void* lpAvatarData, UINT nDataSize){
	ASSERT(lpAvatarData && nAvatarId > 0 && nDataSize > 0);
	if( !m_pAvatarInfo ){
		m_pAvatarInfo = new PokerAvatar();
		m_pAvatarInfo->m_nId				= nAvatarId;
		m_pAvatarInfo->m_nPrivateUserId		= m_nUserId;
		m_pAvatarInfo->m_recStatus			= 0;
		m_pAvatarInfo->m_sAvatarFileName	= _T("");
		m_bDestroyAvatarInfo = true;
		}
	else{
		m_pAvatarInfo->m_nId				= nAvatarId;
		}
	m_nAvatarId = nAvatarId;
	return m_pAvatarInfo->m_binImage.SetBinarySize(nDataSize, lpAvatarData);
	}