// PokerTableWnd.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Controls\MercuryGUI.h"
#include "..\..\Log\Logger.h"

#include "ChildControls\ESChildControlTableCards.h"
#include "ChildControls\ESChildControlTable.h"
#include "ChildControls\ESChildControlChips.h"
#include "ChildControls\ESChildControlSeatCards.h"
#include "ChildControls\ESChildControlSeatHiddenCards.h"

#include "..\..\Image\ImageHelper.h"
#include "..\..\UI\Poker\PokerGUI.h"
#include "..\..\Path\Path.h"
#include "..\..\Protocol\PokerPacket.h"
#include "..\..\PokerServer\Object\PokerHandCards.h"
#include "PokerTableWnd.h"
#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DealSeatCardsAnimationInfo

DealSeatCardsAnimationInfo::DealSeatCardsAnimationInfo(){
	}

BEGIN_DECLARE_VAR(DealSeatCardsAnimationInfo, Serializable)
DECLARE_VAR(_T("DealSeatCardsAnimationInfo"),				_T(""), VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("MotionTypeX"),							_T(""), VariableType::Int32, offsetof(DealSeatCardsAnimationInfo,m_nMotionTypeX), false, false, false)
	DECLARE_VAR(_T("MotionTypeY"),							_T(""), VariableType::Int32, offsetof(DealSeatCardsAnimationInfo,m_nMotionTypeY), false, false, false)
	DECLARE_VAR(_T("MotionTimeMS"),							_T(""), VariableType::Int32, offsetof(DealSeatCardsAnimationInfo,m_nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("TimeDistanceBetweenSeatCardAnimation"),	_T(""), VariableType::Int32, offsetof(DealSeatCardsAnimationInfo,m_nTimeDistanceBetweenSeatCardAnimation), false, false, false)
	DECLARE_VAR(_T("TimeDistanceBetweenCardAnimation"),		_T(""), VariableType::Int32, offsetof(DealSeatCardsAnimationInfo,m_nTimeDistanceBetweenCardAnimation), false, false, false)
	DECLARE_VAR(_T("FrameCtPerSecond"),						_T(""), VariableType::Int32, offsetof(DealSeatCardsAnimationInfo,m_nFrameCtPerSecond), false, false, false)
	DECLARE_VAR(_T("AlphaAtStart"),							_T(""), VariableType::Int8, offsetof(DealSeatCardsAnimationInfo,m_btAlphaAtStart), false, false, false)
END_DECLARE_VAR()

DealSeatCardsAnimationInfo::~DealSeatCardsAnimationInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// TableCardsAnimationInfo

TableCardsAnimationInfo::TableCardsAnimationInfo(){
	}

BEGIN_DECLARE_VAR(TableCardsAnimationInfo, Serializable)
DECLARE_VAR(_T("TableCardsAnimationInfo"),					_T(""), VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("MotionTypeX"),							_T(""), VariableType::Int32, offsetof(TableCardsAnimationInfo,m_nMotionTypeX), false, false, false)
	DECLARE_VAR(_T("MotionTypeY"),							_T(""), VariableType::Int32, offsetof(TableCardsAnimationInfo,m_nMotionTypeY), false, false, false)
	DECLARE_VAR(_T("MotionTimeMS"),							_T(""), VariableType::Int32, offsetof(TableCardsAnimationInfo,m_nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("FlopCardsMotionTimeMS"),				_T(""), VariableType::Int32, offsetof(TableCardsAnimationInfo,m_nFlopCardsMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("TimeDistanceBetweenCardAnimation"),		_T(""), VariableType::Int32, offsetof(TableCardsAnimationInfo,m_nTimeDistanceBetweenCardAnimation), false, false, false)
	DECLARE_VAR(_T("FrameCtPerSecond"),						_T(""), VariableType::Int32, offsetof(TableCardsAnimationInfo,m_nFrameCtPerSecond), false, false, false)
	DECLARE_VAR(_T("AlphaAtStart"),							_T(""), VariableType::Int8, offsetof(TableCardsAnimationInfo,m_btAlphaAtStart), false, false, false)
	DECLARE_VAR(_T("SlideCardsOnFLop"),						_T(""), VariableType::Bool, offsetof(TableCardsAnimationInfo,m_bSlideCardsOnFLop), false, false, false)
END_DECLARE_VAR()

TableCardsAnimationInfo::~TableCardsAnimationInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// CollectChipsAnimationInfo

CollectChipsAnimationInfo::CollectChipsAnimationInfo(){
	}

BEGIN_DECLARE_VAR(CollectChipsAnimationInfo, Serializable)
DECLARE_VAR(_T("CollectChipsAnimationInfo"),				_T(""), VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("MotionTypeX"),							_T(""), VariableType::Int32, offsetof(CollectChipsAnimationInfo,m_nMotionTypeX), false, false, false)
	DECLARE_VAR(_T("MotionTypeY"),							_T(""), VariableType::Int32, offsetof(CollectChipsAnimationInfo,m_nMotionTypeY), false, false, false)
	DECLARE_VAR(_T("MotionTimeMS"),							_T(""), VariableType::Int32, offsetof(CollectChipsAnimationInfo,m_nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("FrameCtPerSecond"),						_T(""), VariableType::Int32, offsetof(CollectChipsAnimationInfo,m_nFrameCtPerSecond), false, false, false)
	DECLARE_VAR(_T("DelayAfterMotionMS"),					_T(""), VariableType::Int32, offsetof(CollectChipsAnimationInfo,m_nDelayAfterMotionMS), false, false, false)
END_DECLARE_VAR()

CollectChipsAnimationInfo::~CollectChipsAnimationInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// MoveChipsAnimationInfo

MoveChipsAnimationInfo::MoveChipsAnimationInfo(){
	}

BEGIN_DECLARE_VAR(MoveChipsAnimationInfo, Serializable)
DECLARE_VAR(_T("MoveChipsAnimationInfo"),					_T(""), VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("MotionTypeX"),							_T(""), VariableType::Int32, offsetof(MoveChipsAnimationInfo,m_nMotionTypeX), false, false, false)
	DECLARE_VAR(_T("MotionTypeY"),							_T(""), VariableType::Int32, offsetof(MoveChipsAnimationInfo,m_nMotionTypeY), false, false, false)
	DECLARE_VAR(_T("MotionTimeMS"),							_T(""), VariableType::Int32, offsetof(MoveChipsAnimationInfo,m_nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("FrameCtPerSecond"),						_T(""), VariableType::Int32, offsetof(MoveChipsAnimationInfo,m_nFrameCtPerSecond), false, false, false)
END_DECLARE_VAR()

MoveChipsAnimationInfo::~MoveChipsAnimationInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// PokerTableWnd Animations

#ifdef DEBUG
#define LOAD_ANIMATION_INFO_FROM_XML
#endif

bool
PokerTableWnd::CollectChips(int nMainChipsAmount, EnumerableObject<PokerPotAmountDetail>* pListPots){
	EnumerableObject<PokerPotAmountDetail>* pListPotsCopy = NULL;
	if( pListPots ){
		pListPotsCopy = new EnumerableObject<PokerPotAmountDetail>();
		pListPots->Copy_Into(pListPotsCopy);
		}
	
	CRect rcClient;
	GetClientRectMy(rcClient);
	if( PokerTableSettings::GetInstance()->m_nAnimationType != TableAnimationType::TableAnimationType_Full || !AnimationWillBeVisible(rcClient) ){
		// Clear seat pot amounts.
		for(int i=0; i<m_info.m_seats.GetCount(); i++){
			PokerTableSeatInfo* pSeat = m_info.m_seats.GetAt(i);
			if( !pSeat || pSeat->GetPotAmount() <= 0 ) continue;
			SetPotAmount(i, 0, true);
			}

		::PostMessage(GetHWND(), WM_COLLECT_CHIPS_FINISHED, (WPARAM)nMainChipsAmount, (LPARAM)pListPotsCopy);
		return true;
		}

	ESEasyMotion*			pMotion				= &m_motionCollectChips;
	pMotion->SetAutoDelete(false);
	ESChildControlChips*	pMainChips			= GetMainChipsControl();
	ESChildControlTable*	pTable				= GetTableControl();
	CRect					rcMainChips			= pMainChips->GetClientRect();

	ESEasyMotionType		motionTypeX			= pTable->GetCollectChipsMotionTypeX();
	ESEasyMotionType		motionTypeY			= pTable->GetCollectChipsMotionTypeY();
	int						nMotionimeMS		= pTable->GetCollectChipsMotionTimeMS();
	int						nFrameCt			= pTable->GetCollectChipsFrameCt();
	int						nDelayAfterMotion	= pTable->GetCollectChipsDelayAfterMotion();
	int						nObjectIndex		= 0;

#ifdef LOAD_ANIMATION_INFO_FROM_XML
	CollectChipsAnimationInfo info;
	if( !info.DeserializeAsXML(_T("CollectChipsAnimationInfo.xml")) ){
		info.m_nMotionTypeX								= motionTypeX;
		info.m_nMotionTypeY								= motionTypeY;
		info.m_nMotionTimeMS							= nMotionimeMS;
		info.m_nFrameCtPerSecond						= nFrameCt;
		info.m_nDelayAfterMotionMS						= nDelayAfterMotion;
		info.SerializeAsXML(_T("CollectChipsAnimationInfo.xml"), FALSE);
		}
	else{
		motionTypeX								= (ESEasyMotionType)info.m_nMotionTypeX;
		motionTypeY								= (ESEasyMotionType)info.m_nMotionTypeY;
		nMotionimeMS							= info.m_nMotionTimeMS;
		nFrameCt								= info.m_nFrameCtPerSecond;
		nDelayAfterMotion						= info.m_nDelayAfterMotionMS;
		}
#endif

	for(int i=0; i<m_info.m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = m_info.m_seats.GetAt(i);
		if( !pSeat || pSeat->GetPotAmount() <= 0 ) 
			continue;

		ESChildControlChips*	pChip		= GetChipsControl(i);
		CRect					rcControl	= pChip->GetClientRect();
		float 					fPosX		= pChip->GetLogicalPosX();
		float 					fPosY		= pChip->GetLogicalPosY();
		float 					fPosEndX	= ((rcMainChips.left + (rcMainChips.Width() /*- rcControl.Width()*/)/2) / ((float)rcClient.Width()));
		float 					fPosEndY	= ((rcMainChips.top + (rcMainChips.Height() - rcControl.Height())/2) / ((float)rcClient.Height()));

		ESChildControlChips*	pChipTemp	= new  ESChildControlChips();
		pChip->Copy(pChipTemp);
			
		int		nZIndex = pChipTemp->GetZIndex();
		CString sName;
		sName.Format(_T("_collectChip%d"),i + 1);
		if( !AddChildControlEx(pChipTemp, 0, pChipTemp->GetClientRect(), pTable, m_szDefaultWindow, sName, nZIndex) ){
			delete pChipTemp;
			continue;
			}

		pChipTemp->SetVisible			(false, false);
		pChipTemp->SetSizeAutoCalculation(true);
		pChipTemp->SetLogicalPos		(pChip->GetLogicalPosX(), pChip->GetLogicalPosY());
		pChipTemp->SetLogicalSize		(pChip->GetLogicalSizeCX(), pChip->GetLogicalSizeCY());
		pChipTemp->AddEventListener		(this, ChildControlEventCode::EventCode_MotionStart, (ControlEventListener)&PokerTableWnd::OnCollectChipsMotionStart);
		pChipTemp->AddEventListener		(this, ChildControlEventCode::EventCode_MotionEnd, (ControlEventListener)&PokerTableWnd::OnCollectChipsMotionEnd);
		pChipTemp->AddEventListener		(this, ChildControlEventCode::EventCode_MotionStop, (ControlEventListener)&PokerTableWnd::OnCollectChipsMotionStop);

		if( fPosX > fPosEndX )
			fPosEndX = ((rcMainChips.left + rcMainChips.Width()/2 - (rcControl.Width() - pChip->GetWidthDrawn()/2)) / ((float)rcClient.Width()));
		else
			fPosEndX = ((rcMainChips.left + rcMainChips.Width()/2 - pChip->GetWidthDrawn()/2) / ((float)rcClient.Width()));

		pChipTemp->SetSizeAutoCalculation(true);
		pMotion->AddMotionObject(pChipTemp, nMotionimeMS, nFrameCt, motionTypeX, motionTypeY, fPosX, fPosY, fPosEndX, fPosEndY, 0, 
			(DWORD)i/*Chip seat index*/, (DWORD)nObjectIndex, (DWORD)nObjectIndex);
		nObjectIndex ++;
		}

	if( pMotion->StartMotion(GetHWND(), WM_COLLECT_CHIPS_FINISHED, nMainChipsAmount/*Main pot chips amount*/, (LPARAM)pListPotsCopy/*Pots detail info*/) ){
		if( m_pHandler )
			m_pHandler->OnCollectChipsStarted();
		}
	else
		return false;
	return true;
	}

bool
PokerTableWnd::MoveChipsFromPot(List<PokerHandWinnerInfo>* pListWinners, bool bUseDelayBeforeAnimation){	
	ASSERT(pListWinners);
	if( !pListWinners || !pListWinners->GetCount() )
		return false;

	List<PokerHandWinnerInfo>* pListWinnersCopy = new List<PokerHandWinnerInfo>();
	AutoSortedArray arrWinnersBySeat;
	for(int i=0; i<pListWinners->GetCount(); i++){
		PokerHandWinnerInfo* pInfo =  pListWinners->GetAt(i);
		if( !pInfo ) continue;
		arrWinnersBySeat.Add((void*)pInfo->m_cSeat, pInfo);

		PokerHandWinnerInfo* pInfoCopy = new PokerHandWinnerInfo();
		pInfo->Copy(pInfoCopy);
		pListWinnersCopy->Add(pInfoCopy);
		}

	CRect rcClient;
	GetClientRectMy(rcClient);
	if( PokerTableSettings::GetInstance()->m_nAnimationType != TableAnimationType::TableAnimationType_Full || !AnimationWillBeVisible(rcClient) ){		
		for(int i=0; i<m_info.m_seats.GetCount(); i++){
			int nIndex = arrWinnersBySeat.IndexOf((void*)i);
			if( nIndex == -1 )
				SetPotAmount(i, 0, true);
			else{
				PokerHandWinnerInfo* pInfo = (PokerHandWinnerInfo*)arrWinnersBySeat.GetData(nIndex);
				if( pInfo )
					SetPotAmount(i, max(pInfo->m_nWonAmount + pInfo->m_nReturnBackAmount, 0), true);
				}
			}

		SetMainPotAmount(0, NULL, true);
		::PostMessage(GetHWND(), WM_MOVE_CHIPS_FROM_POT_FINISHED, 0L, 0L);
		return true;
		}

	ESChildControlChips*	pMainChips			= GetMainChipsControl();
	ESChildControlTable*	pTable				= GetTableControl();
	CRect					rcMainChips			= pMainChips->GetClientRect();
	ESEasyMotion*			pMotion				= &m_motionMoveChips;
	pMotion->SetAutoDelete(false);

	ESEasyMotionType		motionTypeX			= pTable->GetMoveChipsMotionTypeX();
	ESEasyMotionType		motionTypeY			= pTable->GetMoveChipsMotionTypeY();
	int						nMotionimeMS		= pTable->GetMoveChipsMotionTimeMS();
	int						nFrameCt			= pTable->GetMoveChipsFrameCt();
	int						nDelayAfterMotionMS	= pTable->GetCollectChipsDelayAfterMotion();
	int						nObjectIndex		= 0;
	int						nPotIndex			= 0; // Zero is main pot.
	
#ifdef LOAD_ANIMATION_INFO_FROM_XML
	MoveChipsAnimationInfo info;
	if( !info.DeserializeAsXML(_T("MoveChipsAnimationInfo.xml")) ){
		info.m_nMotionTypeX								= motionTypeX;
		info.m_nMotionTypeY								= motionTypeY;
		info.m_nMotionTimeMS							= nMotionimeMS;
		info.m_nFrameCtPerSecond						= nFrameCt;
		info.SerializeAsXML(_T("MoveChipsAnimationInfo.xml"), FALSE);
		}
	else{
		motionTypeX								= (ESEasyMotionType)info.m_nMotionTypeX;
		motionTypeY								= (ESEasyMotionType)info.m_nMotionTypeY;
		nMotionimeMS							= info.m_nMotionTimeMS;
		nFrameCt								= info.m_nFrameCtPerSecond;
		}
#endif

	int nMSWaitBeforeMotion = (bUseDelayBeforeAnimation ? nDelayAfterMotionMS : 0);
	for(int i=0; i<pListWinnersCopy->GetCount(); i++){
		PokerHandWinnerInfo*	pInfo	=  (PokerHandWinnerInfo*)pListWinnersCopy->GetAt(i);
		int						nAmount	= pInfo ? (pInfo->m_nWonAmount + pInfo->m_nReturnBackAmount) : 0;
		if( nAmount <= 0 ) continue;
		
		ESChildControlChips*	pChip	= GetChipsControl(pInfo->m_cSeat);
		if( !pChip ) continue;
		pChip->SetVisible				(false, false);
		pChip->SetSizeAutoCalculation	(true);

		int nPotAmountToSet				= 0;
		for(char j=0; j<pInfo->m_listWonAmountDetail.GetCount(); j++){
			PokerWonAmountDetail* pDetail = pInfo->m_listWonAmountDetail[j];
			if( !pDetail ) continue;

			ESChildControlChips*	pSidePotChips	= !pDetail->m_cPotIndex ? GetMainChipsControl() : GetSidePotChipsControl(pDetail->m_cPotIndex - 1);
			if( !pSidePotChips )
				continue;

			CRect					rcSidePotChips	= pSidePotChips->GetClientRect();
			CRect					rcControl		= pChip->GetClientRect();
			float 					fPosEndX		= pChip->GetLogicalPosX();
			float 					fPosEndY		= pChip->GetLogicalPosY();
			float 					fPosX			= ((rcSidePotChips.left + (rcSidePotChips.Width() - rcControl.Width())/2) / ((float)rcClient.Width()));
			float 					fPosY			= ((rcSidePotChips.top + (rcSidePotChips.Height() - rcControl.Height())/2) / ((float)rcClient.Height()));

			ESChildControlChips*	pChipTemp		= new  ESChildControlChips();
			pChip->Copy(pChipTemp);
			
			int		nZIndex = pChipTemp->GetZIndex();
			CString sName;
			sName.Format(_T("_moveChip%d"),nObjectIndex + 1);
			if( !AddChildControlEx(pChipTemp, 0, pChipTemp->GetClientRect(), pTable, m_szDefaultWindow, sName, nZIndex) ){
				delete pChipTemp;
				continue;
				}

			pChipTemp->SetAmount				(pDetail->m_nWonAmount - (int)pDetail->m_wRakeAmount);
			pChipTemp->SetVisible				(false, true);
			pChipTemp->SetSizeAutoCalculation	(true);
			pChipTemp->SetLogicalPos			(pChip->GetLogicalPosX(), pChip->GetLogicalPosY());
			pChipTemp->SetLogicalSize			(pChip->GetLogicalSizeCX(), pChip->GetLogicalSizeCY());
			pChipTemp->AddEventListener			(this, ChildControlEventCode::EventCode_MotionStart, (ControlEventListener)&PokerTableWnd::OnMoveChipsMotionStart);
			pChipTemp->AddEventListener			(this, ChildControlEventCode::EventCode_MotionEnd, (ControlEventListener)&PokerTableWnd::OnMoveChipsMotionEnd);
			pChipTemp->AddEventListener			(this, ChildControlEventCode::EventCode_MotionStop, (ControlEventListener)&PokerTableWnd::OnMoveChipsMotionStop);

			struct MoveChipsMotionEndParam{
				int						nSeatIndex;
				int						nPotAmountToSet;
				};

			nPotAmountToSet					+= (pDetail->m_nWonAmount - (int)pDetail->m_wRakeAmount);

			MoveChipsMotionEndParam* pParam = new MoveChipsMotionEndParam;
			pParam->nSeatIndex				= (int)pInfo->m_cSeat;
			pParam->nPotAmountToSet			= (j == pInfo->m_listWonAmountDetail.GetCount() - 1) ? pInfo->m_nWonAmount : nPotAmountToSet;

			pMotion->AddMotionObject(pChipTemp, nMotionimeMS, nFrameCt, motionTypeX, motionTypeY, fPosX, fPosY, fPosEndX, fPosEndY, 
				(nMSWaitBeforeMotion + pDetail->m_cPotIndex*(nMotionimeMS + 200)),
				(DWORD)pDetail->m_cPotIndex/*Pot index 0 is main pot other are side pots*/, (DWORD)pParam/*MoveChipsMotionEndParam*/, 0
				);

			nObjectIndex ++;
			}

		//nMSWaitBeforeMotion += (nMotionimeMS + 20);
		}

	/*
	for(int i=0; i<m_info.m_seats.GetCount(); i++){
		int nIndex = arrWinnersBySeat.IndexOf((void*)i);
		if( nIndex == -1 ){
			SetPotAmount(i, 0, false);
			continue;
			}

		PokerHandWinnerInfo*	pInfo	=  (PokerHandWinnerInfo*)arrWinnersBySeat.GetData(nIndex);
		int						nAmount	= pInfo ? (pInfo->m_nWonAmount + pInfo->m_nReturnBackAmount) : 0;
		if( nAmount <= 0 ) {
			SetPotAmount(i, 0, false);
			continue;
			}

		ESChildControlChips*	pChip	= GetChipsControl(i);
		if( !pChip ) continue;
		pChip->SetVisible				(false, false);
		pChip->SetSizeAutoCalculation	(true);

		for(char j=0; j<pInfo->m_listWonAmountDetail.GetCount(); j++){
			PokerWonAmountDetail* pDetail = pInfo->m_listWonAmountDetail[j];
			if( !pDetail ) continue;

			ESChildControlChips*	pSidePotChips	= !pDetail->m_cPotIndex ? GetMainChipsControl() : GetSidePotChipsControl(pDetail->m_cPotIndex - 1);
			if( !pSidePotChips )
				continue;

			CRect					rcSidePotChips	= pSidePotChips->GetClientRect();
			CRect					rcControl		= pChip->GetClientRect();
			float 					fPosEndX		= pChip->GetLogicalPosX();
			float 					fPosEndY		= pChip->GetLogicalPosY();
			float 					fPosX			= ((rcSidePotChips.left + (rcSidePotChips.Width() - rcControl.Width())/2) / ((float)rcClient.Width()));
			float 					fPosY			= ((rcSidePotChips.top + (rcSidePotChips.Height() - rcControl.Height())/2) / ((float)rcClient.Height()));

			ESChildControlChips*	pChipTemp		= new  ESChildControlChips();
			pChip->Copy(pChipTemp);
			
			int		nZIndex = pChipTemp->GetZIndex();
			CString sName;
			sName.Format(_T("_moveChip%d"),nObjectIndex + 1);
			if( !AddChildControlEx(pChipTemp, 0, pChipTemp->GetClientRect(), pTable, m_szDefaultWindow, sName, nZIndex) ){
				delete pChipTemp;
				continue;
				}

			pChipTemp->SetAmount				(pDetail->m_nWonAmount);
			pChipTemp->SetVisible				(false, true);
			pChipTemp->SetSizeAutoCalculation	(true);
			pChipTemp->SetLogicalPos			(pChip->GetLogicalPosX(), pChip->GetLogicalPosY());
			pChipTemp->SetLogicalSize			(pChip->GetLogicalSizeCX(), pChip->GetLogicalSizeCY());
			pChipTemp->AddEventListener			(this, ChildControlEventCode::EventCode_MotionStart, (ControlEventListener)&PokerTableWnd::OnMoveChipsMotionStart);
			pChipTemp->AddEventListener			(this, ChildControlEventCode::EventCode_MotionEnd, (ControlEventListener)&PokerTableWnd::OnMoveChipsMotionEnd);
			pChipTemp->AddEventListener			(this, ChildControlEventCode::EventCode_MotionStop, (ControlEventListener)&PokerTableWnd::OnMoveChipsMotionStop);

			nMSWaitBeforeMotion	+= j*(nMotionimeMS*4);

			pMotion->AddMotionObject(pChipTemp, nMotionimeMS, nFrameCt, motionTypeX, motionTypeY, fPosX, fPosY, fPosEndX, fPosEndY, 
				nMSWaitBeforeMotion,
				(DWORD)pDetail->m_cPotIndex, //Pot index 0 is main pot other are side pots
				(DWORD)i,//Chip seat index
				0
				);

			nObjectIndex ++;
			}

		nMSWaitBeforeMotion += nMotionimeMS*4;
		}*/

	pListWinnersCopy->RemoveAll(true);
	delete pListWinnersCopy;
	
	if( pMotion->StartMotion(GetHWND(), WM_MOVE_CHIPS_FROM_POT_FINISHED, 0L, 0L) ){
		if( m_pHandler )
			m_pHandler->OnMoveChipsFromPotStarted();
		}
	else
		return false;
	return true;
	}

bool
PokerTableWnd::DealSeatCardsAnimation(PokerHandStartedPacket* p){
	if( !p || !p->m_handCards.GetCount() ) return false;
	ESChildControlSeatHiddenCards*	pDeck	= GetDeckControl();
	ESChildControlTable*			pTable	= GetTableControl();
	if( !pDeck ){
		ASSERT(FALSE);
		return false;
		}

	CRect rcClient;
	GetClientRectMy(rcClient);
	if( PokerTableSettings::GetInstance()->m_nAnimationType != TableAnimationType::TableAnimationType_Full || !AnimationWillBeVisible(rcClient) ){
		for(int i=0; i<p->m_handCards.GetCount(); i++){
			PokerHandCards* pCards = p->m_handCards.GetAt(i);
			if( pCards )
				SetSeatCards(((int)pCards->m_btSeat), ((int)pCards->m_btCard1), ((int)pCards->m_btCard2), ((int)pCards->m_btCard3), ((int)pCards->m_btCard4), true); 
			}
		::PostMessage(GetHWND(), WM_SEAT_CARDS_DEAL_FINISHED, 0L, 0L);
		return true;
		}

	ESEasyMotion*	pMotion	= &m_motionDealSeatCards;
	pMotion->SetAutoDelete(false);

	// Dealer seat should receive card last. {{
	AutoSortedArray arrHandCardsByOrder;
	for(int i=0; i<min(p->m_handCards.GetCount(), m_info.m_seats.GetCount()); i++){
		PokerHandCards* pCards = p->m_handCards.GetAt(i);
		if( !pCards ) continue;
		int nSeat = (int)pCards->m_btSeat;
		if( nSeat <= (int)p->m_cDealer )
			arrHandCardsByOrder.Add((void*)(nSeat + p->m_handCards.GetCount()), (void*)(pCards));
		else
			arrHandCardsByOrder.Add((void*)(nSeat - (((int)p->m_cDealer) + 1)), (void*)(pCards));
		}
	// }}
	
	int nWaitMSBeforeStartAnimation				= 0;
	int nMotionimeMS							= pTable->GetMotionTimeMS();
	int nTimeDistanceBetweenSeatCardAnimation	= pTable->GetDistanceSeatCardAnimation();
	int nTimeDistanceBetweenCardAnimation		= pTable->GetDistanceCardAnimation();
	int nCardsCtForEachSeat						= m_nSeatCardsCt;
	int nFrameCtPerSecond						= pTable->GetFrameCt();
	int nSeatCt									= arrHandCardsByOrder.GetCount();
	BYTE btAlpha								= pTable->GetCardAlphaAtStart();

	ESEasyMotionType motionTypeX				= pTable->GetSeatCardsDealMotionTypeX();
	ESEasyMotionType motionTypeY				= pTable->GetSeatCardsDealMotionTypeY();

#ifdef LOAD_ANIMATION_INFO_FROM_XML
	DealSeatCardsAnimationInfo info;
	if( !info.DeserializeAsXML(_T("DealSeatCardsAnimation.xml")) ){
		info.m_nMotionTypeX								= motionTypeX;
		info.m_nMotionTypeY								= motionTypeY;
		info.m_nMotionTimeMS							= nMotionimeMS;
		info.m_nTimeDistanceBetweenSeatCardAnimation	= nTimeDistanceBetweenSeatCardAnimation;
		info.m_nTimeDistanceBetweenCardAnimation		= nTimeDistanceBetweenCardAnimation;
		info.m_nFrameCtPerSecond						= nFrameCtPerSecond;
		info.m_btAlphaAtStart							= btAlpha;
		info.SerializeAsXML(_T("DealSeatCardsAnimation.xml"), FALSE);
		}
	else{
		motionTypeX								= (ESEasyMotionType)info.m_nMotionTypeX;
		motionTypeY								= (ESEasyMotionType)info.m_nMotionTypeY;
		nMotionimeMS							= info.m_nMotionTimeMS;
		nTimeDistanceBetweenSeatCardAnimation	= info.m_nTimeDistanceBetweenSeatCardAnimation;
		nTimeDistanceBetweenCardAnimation		= info.m_nTimeDistanceBetweenCardAnimation;
		nFrameCtPerSecond						= info.m_nFrameCtPerSecond;
		btAlpha									= info.m_btAlphaAtStart;
		}
#endif
	
	for(int j=0; j<nCardsCtForEachSeat; j++){
		nWaitMSBeforeStartAnimation += nTimeDistanceBetweenCardAnimation;
		for(int i=0; i<arrHandCardsByOrder.GetCount(); i++){
			PokerHandCards* pCards	= (PokerHandCards*)arrHandCardsByOrder.GetData(i);
			int				nSeat	= (int)pCards->m_btSeat;

			ESChildControlSeatHiddenCards* pHiddenCards = GetSeatHiddenCardsControl(nSeat);
			char nArrCards[] = {pCards->m_btCard1, pCards->m_btCard2, pCards->m_btCard3, pCards->m_btCard4};
			if( ((int)nArrCards[j]) == -1 )
				continue;

			ESChildControlSeatHiddenCards* pDeckTemp = new  ESChildControlSeatHiddenCards();
			pDeck->Copy(pDeckTemp);

			int		nZIndex = pDeck->GetZIndex() + i + 1;
			CString sName;
			sName.Format(_T("_deck%d%d"),j + 1, nSeat + 1);
			if( !AddChildControlEx(pDeckTemp, 0, pDeckTemp->GetClientRect(), pTable, m_szDefaultWindow, sName, nZIndex) ){
				delete pDeckTemp;
				continue;
				}
			if( j == 0 && i == 0 )
				pDeckTemp->SetVisible(true, true);
			else
				pDeckTemp->SetVisible(false, false);

			pDeckTemp->SetAnimationAlpha	(btAlpha);
			pDeckTemp->SetSizeAutoCalculation(true);
			pDeckTemp->SetLogicalPos		(pDeck->GetLogicalPosX(), pDeck->GetLogicalPosY());
			pDeckTemp->SetLogicalSize		(pDeck->GetLogicalSizeCX(), pDeck->GetLogicalSizeCY());
			pDeckTemp->SetupForAnimation	(nSeat, j, (int)nArrCards[j]);
			pDeckTemp->AddEventListener		(this, ChildControlEventCode::EventCode_MotionEnd, (ControlEventListener)&PokerTableWnd::OnSeatCardDealMotionEnd);
			pDeckTemp->AddEventListener		(this, ChildControlEventCode::EventCode_MotionStop, (ControlEventListener)&PokerTableWnd::OnSeatCardDealMotionStop);

			CRect rcClient;
			GetClientRectMy(rcClient);

			float fStartCX	= pDeckTemp->GetLogicalPosX();
			float fStartCY	= pDeckTemp->GetLogicalPosY();
			CPoint ptCard;
			float fEndCX	= 0.0f;
			float fEndCY	= 0.0f;

			if( !pHiddenCards || nArrCards[j] > -1 ){
				ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(nSeat);
				if( pSeatCards ){
	//				CPoint ptCard = pSeatCards->GetCardPointByIndex(j);
					CPoint ptCard = pSeatCards->GetCardCenterByIndex(j);
					if( ptCard != CPoint(0, 0) ){
						fEndCX	= ((float)ptCard.x) / rcClient.Width();
						fEndCY	= ((float)ptCard.y) / rcClient.Height();
						}
					}
				}
			else{
				ptCard	= pHiddenCards->GetCardPointByIndex(j);
				fEndCX	= ((float)ptCard.x) / rcClient.Width();
				fEndCY	= ((float)ptCard.y) / rcClient.Height();
				}

			pMotion->AddMotionObject(pDeckTemp, nMotionimeMS, nFrameCtPerSecond, 
				motionTypeX, motionTypeY, fStartCX, fStartCY, fEndCX, fEndCY, nWaitMSBeforeStartAnimation);
			nWaitMSBeforeStartAnimation += nTimeDistanceBetweenSeatCardAnimation;
			}
		}

	if( pMotion->StartMotion(GetHWND(), WM_SEAT_CARDS_DEAL_FINISHED) ){
		if( m_pHandler )
			m_pHandler->OnSeatCardsDealAnimationStarted();
		}
	else
		return false;
	return true;
	}

bool
PokerTableWnd::DealTableCardsAnimation(PokerRoundStartedPacket* p){
	if( !p ) return false;
	
	CRect rcClient, rcCard;
	GetClientRectMy(rcClient);

	ESChildControlTableCards*	pTableCards		= GetTableCardsControl();
	ESChildControlTable*		pTable			= GetTableControl();
	CRect						rcControl		= pTableCards->GetClientRect();
	ESChildControlSeatHiddenCards*	pDeck		= GetDeckControl();

	if( PokerTableSettings::GetInstance()->m_nAnimationType != TableAnimationType::TableAnimationType_Full || !AnimationWillBeVisible(rcControl) ){		
		SetTableCards(((int)p->m_btTableCard1), ((int)p->m_btTableCard2), ((int)p->m_btTableCard3), ((int)p->m_btTableCard4), ((int)p->m_btTableCard5), true);
		::PostMessage(GetHWND(), WM_TABLE_CARDS_DEAL_FINISHED, 0L, 0L);
		return true;
		}

	CSize						szCard			= pTableCards->GetCardDefaultSize();
	float						fCX				= szCard.cx / ((float)m_szDefaultWindow.cx);
	float						fCY				= szCard.cy / ((float)m_szDefaultWindow.cy);

	float 						fSizeStartCX	= 0.0f, fSizeEndCX = 0.0f;
	float 						fSizeStartCY	= fCY;
	float 						fSizeEndCY		= fCY;

	int							nTimeDistanceBetweenCardAnimation = pTable->GetTableCardsTimeDistBetweenCards();
	BYTE						btAlphaStart	= pTable->GetTableCardsAlphaAtStart();
	bool						bSlideFlop		= pTable->GetTableCardsSlideFlopAnim();
	int							nFrameCt		= pTable->GetTableCardsDealFrameCt();
	int							nMotionMS		= pTable->GetTableCardsDealMotionTimeMS();
	int							nFlopCardsMotionMS = pTable->GetTableFlopCardsDealMotionTimeMS();
	ESEasyMotionType			motionTypeX		= pTable->GetTableCardsDealMotionTypeX();
	ESEasyMotionType			motionTypeY		= pTable->GetTableCardsDealMotionTypeY();

#ifdef LOAD_ANIMATION_INFO_FROM_XML
	TableCardsAnimationInfo info;
	if( !info.DeserializeAsXML(_T("TableCardsAnimationInfo.xml")) ){
		info.m_nMotionTypeX								= motionTypeX;
		info.m_nMotionTypeY								= motionTypeY;
		info.m_nMotionTimeMS							= nMotionMS;
		info.m_nTimeDistanceBetweenCardAnimation		= nTimeDistanceBetweenCardAnimation;
		info.m_nFrameCtPerSecond						= nFrameCt;
		info.m_btAlphaAtStart							= btAlphaStart;
		info.m_bSlideCardsOnFLop						= true;
		info.m_nFlopCardsMotionTimeMS					= nFlopCardsMotionMS;
		info.SerializeAsXML(_T("TableCardsAnimationInfo.xml"), FALSE);
		}
	else{
		motionTypeX								= (ESEasyMotionType)info.m_nMotionTypeX;
		motionTypeY								= (ESEasyMotionType)info.m_nMotionTypeY;
		nMotionMS								= info.m_nMotionTimeMS;
		nTimeDistanceBetweenCardAnimation		= info.m_nTimeDistanceBetweenCardAnimation;
		nFrameCt								= info.m_nFrameCtPerSecond;
		btAlphaStart							= info.m_btAlphaAtStart;
		bSlideFlop								= info.m_bSlideCardsOnFLop;
		nFlopCardsMotionMS						= info.m_nFlopCardsMotionTimeMS;
		}
#endif

	float	fZoomCX			= 1.0f, fZoomCY = 1.0f;
	GetZoom(fZoomCX, fZoomCY);
	ESEasyMotion*	pMotion	= &m_motionDealTableCards;
	pMotion->SetAutoDelete(false);

	switch( ((int)p->m_cRoundType) ){
		case PokerRoundType::Flop:{
			// Slide flop cards.
			if( bSlideFlop ){
				fSizeStartCX	= 1.0*fCX;
				fSizeEndCX		= (pTableCards->GetWidthCards(3)) / ((float)rcClient.Width());

				pTableCards->SetCard1(((char)p->m_btTableCard1));
				pTableCards->SetCard2(((char)p->m_btTableCard2));
				pTableCards->SetCard3(((char)p->m_btTableCard3));
				pTableCards->SetCard4(((char)p->m_btTableCard4));
				pTableCards->SetCard5(((char)p->m_btTableCard5));

				pMotion->AddMotionObject(pTableCards, nFlopCardsMotionMS, nFrameCt, motionTypeX, motionTypeY, fSizeStartCX, fSizeStartCY, fSizeEndCX, fSizeEndCY);
				if( !pMotion->StartMotion(GetHWND(), WM_TABLE_CARDS_DEAL_FINISHED) ){
					pTableCards->Redraw();
					return false;
					}
				}
			else{
				CString sName;
				int nWaitMSBeforeStartAnimation = 0;
				for(int i=0; i<3/*flop cards count*/; i++){
					ESChildControlSeatHiddenCards* pDeckTemp	= new  ESChildControlSeatHiddenCards();
					pDeck->Copy(pDeckTemp);
			
					int	nZIndex = pDeck->GetZIndex() + i + 1;
					sName.Format(_T("_deck1_%d"), i);
					if( !AddChildControlEx(pDeckTemp, 0, pDeckTemp->GetClientRect(), pTable, m_szDefaultWindow, sName, nZIndex) ){
						delete pDeckTemp;
						continue;
						}

					pDeckTemp->SetAnimationAlpha	(btAlphaStart);
					pDeckTemp->SetVisible			(false, false);
					pDeckTemp->SetSizeAutoCalculation(true);
					pDeckTemp->SetLogicalPos		(pDeck->GetLogicalPosX(), pDeck->GetLogicalPosY());
					pDeckTemp->SetLogicalSize		(pDeck->GetLogicalSizeCX(), pDeck->GetLogicalSizeCY());
					pDeckTemp->AddEventListener		(this, ChildControlEventCode::EventCode_MotionEnd, (ControlEventListener)&PokerTableWnd::OnTableFlopCardDealMotionEnd);
					pDeckTemp->AddEventListener		(this, ChildControlEventCode::EventCode_MotionStop, (ControlEventListener)&PokerTableWnd::OnTableFlopCardDealMotionStop);

					GetClientRectMy(rcClient);
					rcCard			= pTableCards->GetCardRect(i);
					float fStartCX	= pDeckTemp->GetLogicalPosX();
					float fStartCY	= pDeckTemp->GetLogicalPosY();
					float fEndCX	= ((float)rcCard.left) / rcClient.Width();
					float fEndCY	= ((float)rcCard.top) / rcClient.Height();

					// BYTE order <CARD INDEX><Card1 value><Card2 value><Card3 value>
					DWORD dwParam = ((DWORD)i) << 24 | ((((DWORD)p->m_btTableCard1)&0xFF)<<16) | ((((DWORD)p->m_btTableCard2)&0xFF)<<8) | (((DWORD)p->m_btTableCard3)&0xFF);
					pMotion->AddMotionObject(pDeckTemp, nMotionMS, nFrameCt, motionTypeX, motionTypeY, fStartCX, fStartCY, fEndCX, fEndCY, nWaitMSBeforeStartAnimation,
						0, dwParam, dwParam);
					nWaitMSBeforeStartAnimation += nTimeDistanceBetweenCardAnimation;
					}
					
				if( !pMotion->StartMotion(GetHWND(), WM_TABLE_CARDS_DEAL_FINISHED) ){
					SetTableCards(((char)p->m_btTableCard1), ((char)p->m_btTableCard2), ((char)p->m_btTableCard3), ((char)p->m_btTableCard4), ((char)p->m_btTableCard5), true);
					return false;
					}
				}
			break;
			}
		case PokerRoundType::Turn:{
			CRect	rcCard4	= pTableCards->GetCardRect(3);
			fSizeStartCX	= pDeck->GetLogicalPosX();
			fSizeStartCY	= pDeck->GetLogicalPosY();
			fSizeEndCX		= (rcCard4.left / ((float)rcClient.Width()));
			fSizeEndCY		= (rcCard4.top / ((float)rcClient.Height()));

			pTableCards->SetCard1(((char)p->m_btTableCard1));
			pTableCards->SetCard2(((char)p->m_btTableCard2));
			pTableCards->SetCard3(((char)p->m_btTableCard3));
			pTableCards->SetCard4(((char)p->m_btTableCard4));
			pTableCards->SetCard5(((char)p->m_btTableCard5));

			pMotion->AddMotionObject(pTableCards, nMotionMS, nFrameCt, motionTypeX, motionTypeY, fSizeStartCX, fSizeStartCY, fSizeEndCX, fSizeEndCY);
			if( !pMotion->StartMotion(GetHWND(), WM_TABLE_CARDS_DEAL_FINISHED) ){
				pTableCards->Redraw();
				return false;
				}
			break;
			}
		case PokerRoundType::River:{
			CRect	rcCard5	= pTableCards->GetCardRect(4);
			fSizeStartCX	= pDeck->GetLogicalPosX();
			fSizeStartCY	= pDeck->GetLogicalPosY();
			fSizeEndCX		= (rcCard5.left / ((float)rcClient.Width()));
			fSizeEndCY		= (rcCard5.top / ((float)rcClient.Height()));

			pTableCards->SetCard1(((char)p->m_btTableCard1));
			pTableCards->SetCard2(((char)p->m_btTableCard2));
			pTableCards->SetCard3(((char)p->m_btTableCard3));
			pTableCards->SetCard4(((char)p->m_btTableCard4));
			pTableCards->SetCard5(((char)p->m_btTableCard5));

			pMotion->AddMotionObject(pTableCards, nMotionMS, nFrameCt, motionTypeX, motionTypeY, fSizeStartCX, fSizeStartCY, fSizeEndCX, fSizeEndCY);
			if( !pMotion->StartMotion(GetHWND(), WM_TABLE_CARDS_DEAL_FINISHED) ){
				pTableCards->Redraw();
				return false;
				}
			break;
			}
		}
	return true;
	}

void
PokerTableWnd::StopMotions(){
	m_motionDealSeatCards.StopMotion();
	m_motionDealTableCards.StopMotion();
	m_motionCollectChips.StopMotion();
	m_motionMoveChips.StopMotion();
	}

bool
PokerTableWnd::AnimationWillBeVisible(const CRect rcAnimateArea){
	if( !IsWindow() || !::IsWindowVisible(GetHWND()) )
		return false;
	ESFrameBase* pParentFrame = m_pBaseWnd;
	if( !pParentFrame )
		return false;
	DWORD dwStyle = pParentFrame->GetStyle();
	if( (WS_MINIMIZE&dwStyle) == WS_MINIMIZE )
		return false; // Parent frame is minimized.

	CRect rcAnimateAreaScreen = rcAnimateArea, rcWindow;
	m_pBaseWnd->ClientToScreen(&rcAnimateAreaScreen);

	HWND hWndWalkThrough = ::GetWindow(*pParentFrame, GW_HWNDPREV);
	while(hWndWalkThrough){
		dwStyle = ::GetWindowLongPtrW(hWndWalkThrough, GWL_STYLE);
		if( (dwStyle&(WS_CAPTION|WS_VISIBLE|WS_MINIMIZE)) == (WS_CAPTION|WS_VISIBLE) ){
			if( ::GetWindowRect(hWndWalkThrough, rcWindow) && (rcWindow&rcAnimateArea) == rcAnimateArea ){
//				MessageBeep(-1);
				return false; // Animate area is covered by other window.
				}
			}
		hWndWalkThrough = ::GetWindow(hWndWalkThrough, GW_HWNDPREV);
		}
	return true;
	}

/////////////////////////////////////////////////////////////////////////////
// PokerTableWnd control motion event handlers.

void
PokerTableWnd::OnTableFlopCardDealMotionEnd(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam){
	char cCardIndex		= (char)(nParam>>24);
	char cTableCard1	= (char)((nParam&0xFF0000)>>16);
	char cTableCard2	= (char)((nParam&0xFF00)>>8);
	char cTableCard3	= (char)(nParam&0xFF);
	int nArrTableCards[]={-1, -1, -1, -1, -1};

	// Last flop card.
	if( cCardIndex == 2 ){// Open flop cards.
		SetTableCards(cTableCard1, cTableCard2, cTableCard3, -1, -1, false);
		// Redraw table cards. {{
		ESChildControlTableCards*	pTableCards		= GetTableCardsControl();
		CRect rcRedrawTableCards, rcCard;
		for(int i=0; i<=cCardIndex; i++){
			rcCard = pTableCards->GetCardRect(i);
			rcRedrawTableCards |= rcCard;
			}
		RedrawRect(rcRedrawTableCards, 1);
		// }}
		}
	else{
		for(int i=0; i<=cCardIndex; i++)
			nArrTableCards[i] = -2;
		SetTableCards(nArrTableCards[0], nArrTableCards[1], nArrTableCards[2], nArrTableCards[3], nArrTableCards[4], false);

		ESChildControlTableCards*	pTableCards		= GetTableCardsControl();
		CRect rcCard = pTableCards->GetCardRect(cCardIndex);
		RedrawRect(rcCard, 0);
		}

	// Destroy motion card control.
	RemoveChildControl(pSender);
	}

void
PokerTableWnd::OnTableFlopCardDealMotionStop(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam){
	// Destroy motion card control.
	RemoveChildControl(pSender);
	}

void
PokerTableWnd::OnSeatCardDealMotionEnd(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam){
	if( m_pHandler )
		m_pHandler->OnSeatCardDealMotionEnd(pSender);
	}

void
PokerTableWnd::OnSeatCardDealMotionStop(ESChildControlSeatHiddenCards* pSender, ChildControlEventCode eventCode, int nParam){
	}

void
PokerTableWnd::OnMoveChipsMotionStart(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam){
	// Pot index 0 is main pot index.
	int nPotIndex = nParam;
	if( !nPotIndex ){
		// Call event handler.
		if( m_pHandler && GetMainPotAmount() > 0 /*First time*/ )
			m_pHandler->OnMoveChipsFromPotMotionStart();

		// Set only main pot chips.
		SetMainPotAmount(0, NULL, true, true);
		}
	else{
		ESChildControlChips* pSidePot = GetSidePotChipsControl(nPotIndex - 1);
		if( pSidePot && pSidePot->GetAmount() )
			pSidePot->SetAmount(0, true);
		}

	// Make motion chip visible.
	pSender->SetVisible(true, false);
	}

void
PokerTableWnd::OnMoveChipsMotionEnd(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam){
	struct MoveChipsMotionEndParam{
		int						nSeatIndex;
		int						nPotAmountToSet;
		};
	MoveChipsMotionEndParam* pParam = (MoveChipsMotionEndParam*)nParam;
	
	// Make motion chip invisible.
	pSender->SetVisible(false, false);
	
	if( pParam ){
		// Update seat chip amount.
		int nSeatIndex		= pParam->nSeatIndex;
		int nPotAmountNew	= pParam->nPotAmountToSet;
		SetPotAmount(nSeatIndex, nPotAmountNew, true);
		
		// Clean up.
		delete pParam;
		}
	}

void
PokerTableWnd::OnMoveChipsMotionStop(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam){
	// Destroy motion chip control.
	RemoveChildControl(pSender);
	}

void
PokerTableWnd::OnCollectChipsMotionStart(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam){
	// Update seat chip amount. {{
	int nChipSeatIndex = nParam;
	SetPotAmount(nChipSeatIndex, 0, true);
	// }}

	// Make motion chip visible.
	pSender->SetVisible(true, false);
	}

void
PokerTableWnd::OnCollectChipsMotionEnd(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam){
	}

void
PokerTableWnd::OnCollectChipsMotionStop(ESChildControlChips* pSender, ChildControlEventCode eventCode, int nParam){
	// Destroy motion chip control.
	RemoveChildControl(pSender);
	}

/////////////////////////////////////////////////////////////////////////////
// PokerTableWnd Animation Finish message handlers.

LRESULT	
PokerTableWnd::OnCollectChipsFinished(WPARAM wParam, LPARAM lParam){
	int										nMainPotAmount	= (int)wParam;
	EnumerableObject<PokerPotAmountDetail>* pListPots		= (EnumerableObject<PokerPotAmountDetail>*)lParam;

	// Destroy chip controls. {{
	CRect rcChipsRedraw, rcChip;
	for(int i=0; i<m_info.m_seats.GetCount(); i++){
		CString sName;
		sName.Format(_T("_collectChip%d"), i+1);
		ESChildControl* pChip = GetChildControlByName(sName.GetBuffer());
		if( pChip ){
			rcChip			= pChip->GetClientRect();
			rcChipsRedraw	|= rcChip;
			}
		RemoveChildControl(pChip);
		}
	// }}

	// Update main pot chips. {{
	SetMainPotAmount(nMainPotAmount, pListPots, true);
	if( !rcChipsRedraw.IsRectEmpty() )
		RedrawRect(rcChipsRedraw, 1);
	// }}

	if( pListPots ) delete pListPots;

	if( m_pHandler )
		m_pHandler->OnCollectChipsFinished();
	return 0L;
	}

LRESULT
PokerTableWnd::OnMoveChipsFinished(WPARAM wParam, LPARAM lParam){
	// Destroy chip controls. {{
	CRect rcChipsRedraw, rcChip;
	for(int i=0; i<m_info.m_seats.GetCount(); i++){
		CString sName;
		sName.Format(_T("_moveChip%d"), i+1);
		ESChildControl* pChip = GetChildControlByName(sName.GetBuffer());
		if( pChip ){
			rcChip			= pChip->GetClientRect();
			rcChipsRedraw	|= rcChip;
			RemoveChildControl(pChip);
			}
		else
			break;
		}
	// }}

	if( m_pHandler )
		m_pHandler->OnMoveChipsFromPotFinished();
	return 0L;
	}

LRESULT
PokerTableWnd::OnSeatCardsDealFinished(WPARAM wParam, LPARAM lParam){
	// Destroy hidden cards controls. {{
	for(int i=0; i<m_info.m_seats.GetCount(); i++){
		CString sName;
		sName.Format(_T("_deck1%d"), i+1);
		RemoveChildControlByName(sName);

		sName.Format(_T("_deck2%d"), i+1);
		RemoveChildControlByName(sName);

		sName.Format(_T("_deck3%d"), i+1);
		RemoveChildControlByName(sName);

		sName.Format(_T("_deck4%d"), i+1);
		RemoveChildControlByName(sName);
		}
	// }}
	if( m_pHandler )
		m_pHandler->OnSeatCardsDealAnimationFinished();
	return 0L;
	}

LRESULT
PokerTableWnd::OnTableCardsDealFinished(WPARAM wParam, LPARAM lParam){
	if( m_pHandler )
		m_pHandler->OnTableCardsDealAnimationFinished();

	// Update table cards. {{
	ESChildControlTableCards* pTableCards = GetTableCardsControl();
	if( pTableCards ){
		m_info.m_cCard1 = 	pTableCards->GetCard1();
		m_info.m_cCard2 = 	pTableCards->GetCard2();
		m_info.m_cCard3 = 	pTableCards->GetCard3();
		m_info.m_cCard4 = 	pTableCards->GetCard4();
		m_info.m_cCard5 = 	pTableCards->GetCard5();
		}
	// }}
	return 0L;
	}