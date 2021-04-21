// PokerTableWnd.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "..\..\Controls\MercuryGUI.h"

#include "ChildControls\ESChildControlButton.h"
#include "ChildControls\ESChildControlShortcutBtn.h"
#include "ChildControls\ESChildControlSlider.h"
#include "ChildControls\ESChildControlPreAction.h"
#include "ChildControls\ESChildControlActionPanel.h"
#include "ChildControls\ESChildControlTableCards.h"
#include "ChildControls\ESChildControlTable.h"
#include "ChildControls\ESChildControlSeat.h"
#include "ChildControls\ESChildControlSeatCards.h"
#include "ChildControls\ESChildControlChips.h"
#include "ChildControls\ESChildControlDealer.h"
#include "ChildControls\ESChildControlSeatProgressBar.h"
#include "ChildControls\ESChildControlTabView.h"
#include "ChildControls\ESChildControlTabViewStatPage.h"
#include "ChildControls\ESChildControlMoneyEdit.h"
#include "..\..\Controls\ESChildControlLabel.h"
#include "ChildControls\ESChildControlAddChips.h"
#include "ChildControls\ESChildControlFlatButton.h"
#include "ChildControls\ESChildControlFlyingCover.h"

#include "..\..\Image\ImageHelper.h"
#include "..\..\Path\Path.h"
#include "..\..\Protocol\PokerPacket.h"
#include "PokerTableWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerTableWnd

//=====================================================================
//
// Animation thread.
//
// ====================================================================

UINT	_stdcall	
PokerTableWnd::AnimationProc(WinThread* pThread){
	PokerTableWnd*				pTableWnd		= (PokerTableWnd*)pThread->GetThreadParameter();
	if( !pTableWnd ) return 0L;
	List<PokerAnimationInfo>*	pListAnimations = &pTableWnd->m_listAnimations;

	while( TRUE ){
		if( pThread->IsStopEventSignaled() ){
			break; // Stop animation.
			}

		pThread->Lock(); // Lock
		PokerAnimationInfo* pInfo = pListAnimations->GetAt(0);
		pListAnimations->RemoveAt(0, false);
		if( pInfo ){
			pThread->Unlock(); // Unlock
			switch( pInfo->m_animationType ){
				case AnimationType::CollectChips:
					{
						pTableWnd->AnimateCollectChips(pThread, (CollectChipsAnimationInfo*)pInfo);
						break;
					}
				case AnimationType::ChipsMovement:{
						pTableWnd->AnimateMoveChipsFromPot(pThread, (ChipsMovementAnimationInfo*)pInfo);
						break;
					}
				case AnimationType::DealSeatCards:
					{
						pTableWnd->AnimateStartHand(pThread, (DealSeatCardsAnimationInfo*)pInfo);
						break;
					}
				case AnimationType::DealTableCards:
					{
						pTableWnd->AnimateDealTableCards(pThread, (DealTableCardsAnimationInfo*)pInfo);
						break;
					}
				};
			delete pInfo;
			}
		else{
			pThread->Unlock(); // Unlock

			Sleep(50); // Sleep some time.
			}
		}
	return 0L;
	}

//=====================================================================
//
// Collect chips animation.
//
// ====================================================================

bool	
PokerTableWnd::AnimateCollectChips(WinThread* pThread, CollectChipsAnimationInfo* pAnimationInfo){
	int 			nSecAnimate	= pAnimationInfo->m_nAnimationSec;
	int 			nFrameCt	= 15;
	int				nSkipFrames	= 0;
	int 			nLoop		= 0;
	int 			nCt			= m_nMaxPlayerCt;
	int 			nMainChips	= pAnimationInfo->m_nMainChipsAmount;
	int				nChipsCt	= 0;

	float			fArrPosX[]	= {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	float			fArrPosY[]	= {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	CRect			rcChips	[]	= {CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0),
								   CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0)};

	// Collect chips controls.
	ESChildControlChips* pMainChips = GetMainChipsControl();
	ESChildControlChips* ppChips[]	= {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

// Crush
//	pMainChips = NULL;

	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = GetSeatInfo(nLoop);
		if( !pSeat || pSeat->GetPotAmount() <= 0 ){
			nLoop ++;
			continue;
			}

		ppChips[nLoop] = GetChipsControl(nLoop);
		if( ppChips[nLoop] ){
			if( ppChips[nLoop]->GetAmount() <= 0 ){
				ppChips[nLoop] = NULL;
				}
			else{
				rcChips[nLoop]	= ppChips[nLoop]->GetClientRect();
				fArrPosX[nLoop] = ppChips[nLoop]->GetLogicalPosX();
				fArrPosY[nLoop] = ppChips[nLoop]->GetLogicalPosY();
				nChipsCt		++;
				}
			}
		
		nLoop ++;
		}
	// }}

	if( !nChipsCt ){
		PostMessage(WM_COLLECT_CHIPS_FINISHED, 0, 0);
		return true; // Stop animation.
		}

	// Collect chips sound.
	PokerGUI::GetInstance()->PlaySoundByType(SoundTypes::Sound_ChipsMovedToPot, 1, 0);

	// Animation movement(s)
	int		nFrame = 0;
	CRect	rcChipsBound;
	float	fDelay		= 1000.0f / ((float)(nFrameCt / ((float)nSecAnimate)));
	float   fAccelerate = 0.97f;

	while( nFrame < nFrameCt ){
		int nLoop1 = 0;
		if( (nFrame + nSkipFrames) == nFrameCt )
			break;

		rcChipsBound.SetRect(MAXINT,MAXINT,MAXINT,MAXINT);

		CPtrArray* pArrRects = new CPtrArray();
		while( nLoop1 < nCt ){
			if( !ppChips[nLoop1] ){
				nLoop1 ++;
				continue;
				}

			if( pThread->IsStopEventSignaled() ){
				// Finished.
				PostMessage(WM_COLLECT_CHIPS_FINISHED, 0, 0);
				return true; // Stop animation.
				}

			CRect	rcMainChips	= pMainChips->GetClientRect();
			CPoint	ptMiddle(rcMainChips.left + rcMainChips.Width()/2, rcMainChips.top + rcMainChips.Height()/2);

			ESChildControlChips*	pChips		= ppChips[nLoop1];
			CRect					rcRectChips	= pChips->GetClientRect();
			CRect					rcChipOrig	= rcChips[nLoop1];
			CPoint					ptMiddleChips (rcChipOrig.left + rcChipOrig.Width()/2, rcChipOrig.top + rcChipOrig.Height()/2);
			int						nMoveCY		= ptMiddle.y - ptMiddleChips.y;
			int						nMoveCX		= ptMiddle.x - ptMiddleChips.x;
			int						nOffsetY	= rcChipOrig.top + (nMoveCY / nFrameCt)*(nFrame + 1);
			int						nOffsetX	= rcChipOrig.left + (nMoveCX / nFrameCt)*(nFrame + 1);

			CRect rcTableWnd;
			GetClientRect(rcTableWnd);

			float fY = (nOffsetY*1.0f) / ((float)rcTableWnd.Height());
			float fX = (nOffsetX*1.0f) / ((float)rcTableWnd.Width());

			pChips->SetLogicalPos			(fX, fY);
			pChips->OwnerWindowSizeChanged	(rcTableWnd.Width(), rcTableWnd.Height());
			
			rcRectChips |= pChips->GetClientRect();
			pThread->Lock(); // Lock
			pArrRects->Add(new CRect(rcRectChips.left, rcRectChips.top, rcRectChips.right, rcRectChips.bottom));
			pThread->Unlock(); // Unlock

			rcChipsBound |= rcRectChips;
			nLoop1 ++;
			}

		if( rcChipsBound.Width() < 300 ){
			for(int i=0; i<pArrRects->GetCount(); i++){
				CRect* pRect = (CRect*)pArrRects->GetAt(i);
				delete pRect;
				}
			pArrRects->RemoveAll();
			CRect* pRectBound = new CRect(rcChipsBound);
			pArrRects->Add(pRectBound);
			::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
			}
		else{
			::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
			}
		
//		int nSleep = (int)(fDelay*fAccelerate);
		Sleep(30);
//		fAccelerate *= fAccelerate;
		nFrame		++;
		}

	// Wait
	Sleep(10);

	// Clear seat pot amounts.
	int nLoop1 = 0;
	while( nLoop1 < nCt ){
		if( !ppChips[nLoop1] ){
			nLoop1 ++;
			continue;
			}

		ESChildControlChips*	pChips		= ppChips[nLoop1];
		CRect					rcRectChips	= pChips->GetClientRect();

		CRect rcTableWnd;
		GetClientRect(rcTableWnd);
		
		pChips->SetAmount				(0, false);
		pChips->SetLogicalPos			(fArrPosX[nLoop1], fArrPosY[nLoop1]);
		pChips->OwnerWindowSizeChanged	(rcTableWnd.Width(), rcTableWnd.Height());
		nLoop1 ++;
		}

	if( pThread->IsStopEventSignaled() ){
		PostMessage(WM_COLLECT_CHIPS_FINISHED, 0, 0);
		return true; // Stop animation.
		}

	pThread->Lock(); // Lock
	// Redraw all chips area at once.
	CPtrArray* pArrRects = new CPtrArray();
	pArrRects->Add(new CRect(rcChipsBound.left, rcChipsBound.top, rcChipsBound.right, rcChipsBound.bottom));
	CRect	rcMainChips	= pMainChips->GetClientRect();
	pArrRects->Add(new CRect(rcMainChips.left, rcMainChips.top, rcMainChips.right, rcMainChips.bottom));
	SetMainPotAmount	(nMainChips, false);
	::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
	pThread->Unlock(); // Unlock

	Sleep(100);

	PostMessage(WM_COLLECT_CHIPS_FINISHED, 0, 0);
	return true;
	}



//=====================================================================
//
// Move chips from pot animation.
//
// ====================================================================

bool	
PokerTableWnd::AnimateMoveChipsFromPot(WinThread* pThread, ChipsMovementAnimationInfo* pAnimationInfo){
	int 			nSecAnimate		= pAnimationInfo->m_nAnimationSec;
	int 			nFrameCt		= 15;
	int 			nLoop			= 0;
	int 			nCt				= m_nMaxPlayerCt;
	int*			pArrSeatAmounts	= pAnimationInfo->m_arrMoveChipsFromPotAmounts;

	float			fArrPosX[]		= {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	float			fArrPosY[]		= {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	CRect			rcChips	[]		= {CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0),
									   CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0)};
	// Collect chips controls.
	ESChildControlChips* pMainChips = GetMainChipsControl();
	ESChildControlChips* ppChips[]	= {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

	while( nLoop < nCt ){
		PokerTableSeatInfo* pSeat = GetSeatInfo(nLoop);
		if( !pSeat || pArrSeatAmounts[nLoop] <= 0 ){
			nLoop ++;
			continue;
			}

		ppChips[nLoop] = GetChipsControl(nLoop);
		if( ppChips[nLoop] ){
			rcChips[nLoop] = ppChips[nLoop]->GetClientRect();
			ppChips[nLoop]->SetAmount(pArrSeatAmounts[nLoop], false);
			}
		
		if( ppChips[nLoop] ){
			fArrPosX[nLoop] = ppChips[nLoop]->GetLogicalPosX();
			fArrPosY[nLoop] = ppChips[nLoop]->GetLogicalPosY();
			}
		nLoop ++;
		}
	// }}

	// Animation movement(s)
	int		nFrame = 0;
	CRect	rcChipsBound;
	float	fDelay		= 1000.0f / ((float)(nFrameCt / ((float)nSecAnimate)));
	float   fAccelerate = 0.97f;

	while( nFrame < nFrameCt ){
		int nLoop1 = 0;
		rcChipsBound.SetRect(0, 0, 0, 0);

		CPtrArray* pArrRects = new CPtrArray();
		while( nLoop1 < nCt ){
			if( !ppChips[nLoop1] ){
				nLoop1 ++;
				continue;
				}

			if( pThread->IsStopEventSignaled() ){
				// Post finish message.
				::PostMessage(GetSafeHwnd(), WM_MOVE_CHIPS_FROM_POT_FINISHED, 0, 0);
				return true; // Stop animation.
				}

			CRect	rcMainChips	= pMainChips->GetClientRect();
			CPoint	ptMiddle(rcMainChips.left + rcMainChips.Width()/2, rcMainChips.top + rcMainChips.Height()/2);

			ESChildControlChips*	pChips		= ppChips[nLoop1];
			CRect					rcRectChips	= pChips->GetClientRect();
			CRect					rcChipOrig	= rcChips[nLoop1];
			CPoint					ptMiddleChips (rcChipOrig.left + rcChipOrig.Width()/2, rcChipOrig.top + rcChipOrig.Height()/2);
			int						nMoveCY		= ptMiddle.y - ptMiddleChips.y;
			int						nMoveCX		= ptMiddle.x - ptMiddleChips.x;
			int						nOffsetY	= rcChipOrig.top + (nMoveCY / nFrameCt)*(nFrameCt - (nFrame + 1));
			int						nOffsetX	= rcChipOrig.left + (nMoveCX / nFrameCt)*(nFrameCt - (nFrame + 1));

			CRect rcTableWnd;
			GetClientRect(rcTableWnd);

			float fY = (nOffsetY*1.0f) / ((float)rcTableWnd.Height());
			float fX = (nOffsetX*1.0f) / ((float)rcTableWnd.Width());

			pChips->SetLogicalPos			(fX, fY);
			pChips->OwnerWindowSizeChanged	(rcTableWnd.Width(), rcTableWnd.Height());
			
			rcRectChips |= pChips->GetClientRect();
			pThread->Lock(); // Lock
			pArrRects->Add(new CRect(rcRectChips.left, rcRectChips.top, rcRectChips.right, rcRectChips.bottom));
			pThread->Unlock(); // Unlock

			rcChipsBound |= pChips->GetClientRect();
			nLoop1 ++;
			}

		::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
		int nSleep = (int)(fDelay*fAccelerate);
		Sleep(30);

		fAccelerate *= fAccelerate;
		nFrame		++;
		}

	// Post finish message.
	PostMessage(WM_MOVE_CHIPS_FROM_POT_FINISHED, 0, 0);
	return true;
	}

bool
PokerTableWnd::AnimateStartHand(WinThread* pThread, DealSeatCardsAnimationInfo* pAnimationInfo){
	PokerHandStartedPacket*	p				= &pAnimationInfo->m_handStartedPacket;
	int 					nLoop			= 0;
	int 					nCt				= m_nMaxPlayerCt;
	int						nDealer			= -1;
	int						nFirstDealSeat	= 0;

	AutoSortedArray			arrHandCards;
	for(int i=0; i<p->m_handCards.GetCount(); i++){
		PokerHandCards* pHand = p->m_handCards.GetAt(i);
		if( !pHand ) continue;
		if( (int)p->m_cDealer == ((long)pHand->m_btSeat) )
			nDealer = ((long)pHand->m_btSeat);
		arrHandCards.Add(((long)pHand->m_btSeat), (long)pHand);
		}

	// Dealer seat is unavailable.
	if( nDealer == -1 ){
		nDealer = (int)p->m_cDealer;
		bool bExists = false;
		int nIndex = arrHandCards.FindValuePlace(nDealer + 1, bExists);
		if( bExists ){
			nFirstDealSeat = arrHandCards.Get(nIndex);
			}
		else{
			if( nIndex >= arrHandCards.GetCount() )
				nFirstDealSeat = arrHandCards.Get(0);
			else
				nFirstDealSeat = arrHandCards.Get(nIndex);
			}
		}
	else{
		int nIndex = arrHandCards.IndexOf(nDealer);
		if( (nIndex + 1) >= arrHandCards.GetCount() )
			nIndex = (nIndex + 1) - arrHandCards.GetCount();
		else
			nIndex ++;
		nFirstDealSeat = arrHandCards.Get(nIndex);
		}

	// Create flying cover control. {{
	CRect rcCover;
	rcCover.left	= m_pGUI->m_ptDealerCardPos.x;
	rcCover.top		= m_pGUI->m_ptDealerCardPos.y;
	rcCover.right	= rcCover.left + m_pGUI->m_szCardDimension.cx/2;
	rcCover.bottom	= rcCover.top + m_pGUI->m_szCardDimension.cy/2;

	ESChildControlFlyingCover* pCover = new ESChildControlFlyingCover();
	AddChildControlEx(pCover, CHILD_Table_FlyingCover, rcCover, GetChildControl(CHILD_PokerTable), m_pGUI->m_szDefWindowSize);
	// }}

	CPtrArray*	pArrRects	= NULL;
	nLoop = 0;
//	PokerGUI::GetInstance()->PlaySoundByType(SoundTypes::Sound_CardsDealt);
	while( nLoop < 4 ){
		int nIndexOffset = arrHandCards.IndexOf(nFirstDealSeat);
		for(int i=0; i<arrHandCards.GetCount(); i++){
			int nSeatIndex = i + nIndexOffset;
			if( nSeatIndex >= arrHandCards.GetCount() )
				nSeatIndex -= arrHandCards.GetCount();

			if( pThread->IsStopEventSignaled() ){
				goto quit_label;
				}

			PokerHandCards* pHand = (PokerHandCards*)arrHandCards.GetData(nSeatIndex);
			if( !pHand ) continue;
			int nSeat		= ((int)pHand->m_btSeat);
			int nCards[]	= {-1, -1, -1, -1};
			nCards[0]		= (nLoop >= 0) ? ((int)pHand->m_btCard1) : -1;
			nCards[1]		= (nLoop >= 1) ? ((int)pHand->m_btCard2) : -1;
			nCards[2]		= (nLoop >= 2) ? ((int)pHand->m_btCard3) : -1;
			nCards[3]		= (nLoop >= 3) ? ((int)pHand->m_btCard4) : -1;

			if( nCards[nLoop] == -1 )
				continue;
				
			// Flying cover animation.
			int			nFrame		= 0;
			int 		nFrameCtMin	= 3;
			int 		nFrameCt	= 4;
			int			nSpeed		= 40;
			bool		bCalcFrame	= true;

			while( nFrame <= nFrameCt ){
				CRect rcClient;
				GetClientRect(rcClient);

				float fStretchCX	= rcClient.Width() / ((float)m_pGUI->m_szDefWindowSize.cx);
				float fStretchCY	= rcClient.Height() / ((float)m_pGUI->m_szDefWindowSize.cy);

				CSize szFlyingCover(pCover->GetClientWidth(), pCover->GetClientHeight());

				CRect rcStartPos;
				rcStartPos.left		= m_pGUI->m_ptDealerCardPos.x*fStretchCX;
				rcStartPos.top		= m_pGUI->m_ptDealerCardPos.y*fStretchCY;
				rcStartPos.right	= rcStartPos.left + szFlyingCover.cx;
				rcStartPos.bottom	= rcStartPos.top + szFlyingCover.cy;

				CPoint	ptMiddle	(rcStartPos.left + rcStartPos.Width()/2, rcStartPos.top + rcStartPos.Height()/2);

				CRect rcSeat		= GetSeatRect(nSeat);
				CRect rcFinishPos;
				if( ptMiddle.x > rcSeat.right ){
					rcFinishPos.left	= rcSeat.right;
					rcFinishPos.top		= rcSeat.bottom - szFlyingCover.cy;
					rcFinishPos.right	= rcFinishPos.left + szFlyingCover.cx;
					rcFinishPos.bottom	= rcSeat.bottom;
					}
				else{
					rcFinishPos.left	= rcSeat.left - szFlyingCover.cx;
					rcFinishPos.top		= rcSeat.bottom - szFlyingCover.cy;
					rcFinishPos.right	= rcFinishPos.left + szFlyingCover.cx;
					rcFinishPos.bottom	= rcSeat.bottom;
					}

				CPoint	ptMiddleFly (rcFinishPos.left + rcFinishPos.Width()/2, rcFinishPos.top + rcFinishPos.Height()/2);
				int		nMoveCY		= ptMiddleFly.y - ptMiddle.y;
				int		nMoveCX		= ptMiddleFly.x - ptMiddle.x;

				if( bCalcFrame ){
					if( abs(nMoveCY) > abs(nMoveCX) )
						nFrameCt = ((abs(nMoveCY) / nSpeed*fStretchCY) + 1);
					else
						nFrameCt = ((abs(nMoveCX) / nSpeed*fStretchCX) + 1);
					nFrameCt	= min(nFrameCt, nFrameCtMin);
					bCalcFrame	= false;
					}

				int		nOffsetY	= rcStartPos.top + (nMoveCY / nFrameCt)*(nFrame);
				int		nOffsetX	= rcStartPos.left + (nMoveCX / nFrameCt)*(nFrame);

				CRect rcCoverFly;
				rcCoverFly.left		= m_pGUI->m_ptDealerCardPos.x*fStretchCX;
				rcCoverFly.top		= m_pGUI->m_ptDealerCardPos.y*fStretchCY;
				rcCoverFly.right	= rcCoverFly.left + szFlyingCover.cx;
				rcCoverFly.bottom	= rcCoverFly.top + szFlyingCover.cy;

				CRect rcCoverOld	= pCover->GetClientRect();

				float fY = (nOffsetY*1.0f) / ((float)rcClient.Height());
				float fX = (nOffsetX*1.0f) / ((float)rcClient.Width());
				pCover->SetLogicalPos			(fX, fY);
				pCover->OwnerWindowSizeChanged	(rcClient.Width(), rcClient.Height());

				pThread->Lock(); // Lock
				pArrRects = new CPtrArray();
				pArrRects->Add(new CRect(rcCoverOld));
				pArrRects->Add(new CRect(pCover->GetClientRect()));
				pThread->Unlock(); // Unlock
				::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);

				// Play deal card sound.
				PokerGUI::GetInstance()->PlaySoundByType(SoundTypes::Sound_CardsDealt);

				Sleep(25);

				nFrame		++;
				}
			// }}

			SetSeatCards(nSeat, nCards[0], nCards[1], nCards[2], nCards[3], false);
			// Redraw seat cards. {{
			pArrRects				= new CPtrArray();
			CRect* pRect			= new CRect();
			*pRect					= GetSeatCardsRect(nSeat);
			pArrRects->Add(pRect);
			::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
			// }}
			}
		nLoop ++;
		}

quit_label:
	// Redraw cover control. {{
	pCover->SetVisible(false, false);
	pThread->Lock(); // Lock
	pArrRects = new CPtrArray();
	pArrRects->Add(new CRect(pCover->GetClientRect()));
	pThread->Unlock(); // Unlock
	::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
	// }}

	RemoveChildControl(pCover, true);
	// Post finish message.
	PostMessage(WM_SEAT_CARDS_DEAL_FINISHED, 0, 0);
	return true;
	}

bool
PokerTableWnd::AnimateDealTableCards(WinThread* pThread, DealTableCardsAnimationInfo* pAnimationInfo){
	PokerRoundStartedPacket*	p			= &pAnimationInfo->m_roundStartedPacket;
	ESChildControlTableCards*	pTableCards	= GetTableCardsControl();
	int							nFrameCt	= 15;

	int nCard1			= (int)p->m_btTableCard1;
	int nCard2			= (int)p->m_btTableCard2;
	int nCard3			= (int)p->m_btTableCard3;
	int nCard4			= (int)p->m_btTableCard4;
	int nCard5			= (int)p->m_btTableCard5;

	if( p->m_cRoundType == PokerRoundType::Flop ){
		int nOffsetXCard1	= -1;
		int nOffsetXCard2	= -1;
		int nOffsetXCard3	= -1;

		SetTableCards(nCard1, nCard2, nCard3, -1, -1, false);
		for(int nFrame=0; nFrame<nFrameCt; nFrame++){
			CRect	rcTableCards	= pTableCards->GetClientRect();
			CRect	rcCard			= pTableCards->GetCard1Rect();
			CRect	rcCard1			= pTableCards->GetCard1Rect();
			CRect	rcCard2			= pTableCards->GetCard2Rect();
			CRect	rcCard3			= pTableCards->GetCard3Rect();
			float	f				= (float)((rcCard3.right - rcCard1.left) / ((float)nFrameCt));
			float	fOffsetX		= f * (nFrame + 1);

			nOffsetXCard1	= ((int)fOffsetX) - rcCard.Width();
			if( nOffsetXCard1 <= 0 )
				pTableCards->SetCard1OffsetX(nOffsetXCard1);
			else
				pTableCards->SetCard1OffsetX(0);

			nOffsetXCard2	= (rcCard1.left - rcCard2.right) + (int)(f*2) + (int)(fOffsetX);
			if( nOffsetXCard2 <= 0 )
				pTableCards->SetCard2OffsetX(nOffsetXCard2);
			else
				pTableCards->SetCard2OffsetX(0);

			nOffsetXCard3	= (rcCard1.left - rcCard3.right) + (int)(f*4) + (int)(fOffsetX);
			if( nOffsetXCard3 <= 0 )
				pTableCards->SetCard3OffsetX(nOffsetXCard3);
			else
				pTableCards->SetCard3OffsetX(0);

			if( pThread->IsStopEventSignaled() ){
				// Post finish message.
				PostMessage(WM_TABLE_CARDS_DEAL_FINISHED, 0, 0);
				return true;
				}

			// Redraw seat cards. {{
			CPtrArray* pArrRects	= new CPtrArray();
			CRect* pRect			= new CRect();
			*pRect					= pTableCards->GetClientRect();
			pArrRects->Add(pRect);
			::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
			// }}

			Sleep(40);
			}
		}
	else
	if( p->m_cRoundType == PokerRoundType::Turn ){
		int nOffsetXCard4	= -1;
		SetTableCards(nCard1, nCard2, nCard3, nCard4, -1, false);

		for(int nFrame=0; nFrame<nFrameCt; nFrame++){
			CRect	rcTableCards	= pTableCards->GetClientRect();
			CRect	rcCard			= pTableCards->GetCard4Rect();
			float	f				= (float)(rcCard.Width() / ((float)nFrameCt));
			float	fOffsetX		= f * (nFrame + 1);

			nOffsetXCard4	= ((int)fOffsetX) - rcCard.Width();
			if( nOffsetXCard4 <= 0 )
				pTableCards->SetCard4OffsetX(nOffsetXCard4);
			else
				pTableCards->SetCard4OffsetX(0);

			if( pThread->IsStopEventSignaled() ){
				// Post finish message.
				PostMessage(WM_TABLE_CARDS_DEAL_FINISHED, 0, 0);
				return true;
				}

			// Redraw seat cards. {{
			CPtrArray* pArrRects	= new CPtrArray();
			CRect* pRect			= new CRect();
			*pRect					= pTableCards->GetClientRect();
			pArrRects->Add(pRect);
			::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
			// }}

			Sleep(30);
			}
		}
	else
	if( p->m_cRoundType == PokerRoundType::River ){
		int nOffsetXCard5	= -1;
		SetTableCards(nCard1, nCard2, nCard3, nCard4, nCard5, false);

		for(int nFrame=0; nFrame<nFrameCt; nFrame++){
			CRect	rcTableCards	= pTableCards->GetClientRect();
			CRect	rcCard			= pTableCards->GetCard5Rect();
			float	f				= (float)(rcCard.Width() / ((float)nFrameCt));
			float	fOffsetX		= f * (nFrame + 1);

			nOffsetXCard5	= ((int)fOffsetX) - rcCard.Width();
			if( nOffsetXCard5 <= 0 )
				pTableCards->SetCard5OffsetX(nOffsetXCard5);
			else
				pTableCards->SetCard5OffsetX(0);

			if( pThread->IsStopEventSignaled() ){
				// Post finish message.
				PostMessage(WM_TABLE_CARDS_DEAL_FINISHED, 0, 0);
				return true;
				}

			// Redraw seat cards. {{
			CPtrArray* pArrRects	= new CPtrArray();
			CRect* pRect			= new CRect();
			*pRect					= pTableCards->GetClientRect();
			pArrRects->Add(pRect);
			::PostMessage(GetSafeHwnd(), WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
			// }}

			Sleep(30);
			}
		}
	
	// Post finish message.
	PostMessage(WM_TABLE_CARDS_DEAL_FINISHED, 0, 0);
	return 0L;
	}

