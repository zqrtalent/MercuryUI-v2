// PokerTableWnd.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Controls\MercuryGUI.h"
#include "..\..\Log\Logger.h"

#include "ChildControls\ESChildControlPreAction.h"
#include "ChildControls\ESChildControlTableCards.h"
#include "ChildControls\ESChildControlTable.h"
#include "ChildControls\ESChildSeatStatus.h"
#include "ChildControls\ESChildSeat.h"
#include "ChildControls\ESChildControlChips.h"
#include "ChildControls\ESChildControlSeatCards.h"
#include "ChildControls\ESChildControlSeatHiddenCards.h"
#include "ChildControls\ESChildPreActionControl.h"

#include "..\..\Controls\ESChildControlImage.h"
#include "..\..\Controls\ESChildControlImageButton.h"
#include "..\..\Controls\ESChildControlLabel.h"
#include "..\..\Controls\ESChildSlider.h"
#include "..\..\Controls\ESChildProgressCtrl.h"
#include "..\..\Controls\ESChildTextBox.h"
#include "..\..\Controls\ESSimpleRenderView.h"

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
// PokerTableWnd

PokerTableWnd::PokerTableWnd() : MercuryBaseView(){
	m_szDefWindowSize			= CSize(792, 546);
	m_bAllowZooming				= true;
	m_fRatioCX					= 1.0f;
	m_fRatioCY					= 1.0f;
	for(int i=0; i<10; i++){
		m_arrChipPositions[0].x = 0.0f;
		m_arrChipPositions[0].y = 0.0f;
		}
	m_nSeatCardsCt				= 2;
	m_pClient					= NULL;

	m_bUseForegroundColor		= 0;
	m_crForeground				= RGB(0, 0, 0);
	m_nAlpha					= 50;

	m_pHandler					= NULL;
	m_pTimerActiveSeat			= NULL;
	m_bConnectedMode			= false;

	m_bDesignerMode				= true;
	m_bUISetup					= true;
	m_nFirstSeatIndexClockwise	= 0;

	// Register child classes. {{
	ESChildControl::RegisterChildClass(_T("ESChildControlTable"),			new ESChildControlTable);
	ESChildControl::RegisterChildClass(_T("ESChildSeatStatus"),				new ESChildSeatStatus);
	ESChildControl::RegisterChildClass(_T("ESChildSeat"),					new ESChildSeat);
	ESChildControl::RegisterChildClass(_T("ESChildControlChips"),			new ESChildControlChips);
	ESChildControl::RegisterChildClass(_T("ESChildControlTableCards"),		new ESChildControlTableCards);
	ESChildControl::RegisterChildClass(_T("ESChildControlSeatCards"),		new ESChildControlSeatCards);
	ESChildControl::RegisterChildClass(_T("ESChildControlSeatHiddenCards"),	new ESChildControlSeatHiddenCards);
	ESChildControl::RegisterChildClass(_T("ESChildPreActionControl"),		new ESChildPreActionControl);
	// }}

	// Acceptable max players count. ####
	m_acceptebleMaxPlayersCount.Add((void*)2);
	m_acceptebleMaxPlayersCount.Add((void*)4);
	m_acceptebleMaxPlayersCount.Add((void*)6);
	m_acceptebleMaxPlayersCount.Add((void*)9);
//	m_acceptebleMaxPlayersCount.Add((void*)10);
	// #################################
	}

PokerTableWnd::~PokerTableWnd(){
	// Stop animations.
	StopMotions();
	}

bool
PokerTableWnd::AttachToFrame(ESFrameBase* pBaseFrameWnd){
	bool bRet = MercuryBaseView::AttachToFrame(pBaseFrameWnd);
	if( bRet ){
		pBaseFrameWnd->AddWndMsgListener(this, WM_COLLECT_CHIPS_FINISHED,					(PMSGMethod)&PokerTableWnd::OnCollectChipsFinished);
		pBaseFrameWnd->AddWndMsgListener(this, WM_MOVE_CHIPS_FROM_POT_FINISHED,				(PMSGMethod)&PokerTableWnd::OnMoveChipsFinished);
		pBaseFrameWnd->AddWndMsgListener(this, WM_SEAT_CARDS_DEAL_FINISHED,					(PMSGMethod)&PokerTableWnd::OnSeatCardsDealFinished);
		pBaseFrameWnd->AddWndMsgListener(this, WM_TABLE_CARDS_DEAL_FINISHED,				(PMSGMethod)&PokerTableWnd::OnTableCardsDealFinished);

		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_TABLE,					(PCMDMethod)&PokerTableWnd::OnCommandTable);
		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_SEAT_LABEL_AS_CHILD,		(PCMDMethod)&PokerTableWnd::OnCommandSeatStatusAsChild);
		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_SEAT_AS_CHILD,			(PCMDMethod)&PokerTableWnd::OnCommandSeatAsChild);
		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_CHIPS_AS_CHILD,			(PCMDMethod)&PokerTableWnd::OnCommandChipsAsChild);
		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_HIDDEN_CARDS_AS_CHILD,	(PCMDMethod)&PokerTableWnd::OnCommandHiddenCardsAsChild);
		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_SEAT_CARDS_AS_CHILD,		(PCMDMethod)&PokerTableWnd::OnCommandSeatCardsAsChild);
		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_TABLE_CARDS_AS_CHILD,		(PCMDMethod)&PokerTableWnd::OnCommandTableCardsAsChild);
		pBaseFrameWnd->AddCommandMsgListener(this, CN_COMMAND, MI_PREACTION_AS_CHILD,		(PCMDMethod)&PokerTableWnd::OnCommandPreActionAsChild);
		}

	return bRet;
	}

void
PokerTableWnd::DesignerContextMenu(CMenu* pPopupMenu, CMenu* pNewControlsMenu, CMenu* pNewChildControlsMenu){
	if( pNewControlsMenu && ::IsMenu(*pNewControlsMenu) ){
		pNewControlsMenu->AppendMenu(MF_SEPARATOR, 0, _T(""));
		pNewControlsMenu->AppendMenu(MF_STRING, MI_TABLE,		(LPCTSTR)_T("Poker Table Control"));
		}

	if( pNewChildControlsMenu && ::IsMenu(*pNewChildControlsMenu) ){
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_SEAT_LABEL_AS_CHILD,	(LPCTSTR)_T("Seat Status Control"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_SEAT_AS_CHILD,			(LPCTSTR)_T("Seat Control"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_CHIPS_AS_CHILD,			(LPCTSTR)_T("Chips Control"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_HIDDEN_CARDS_AS_CHILD,	(LPCTSTR)_T("Hidden Cards"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_TABLE_CARDS_AS_CHILD,	(LPCTSTR)_T("Table Cards"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_SEAT_CARDS_AS_CHILD,	(LPCTSTR)_T("Seat Cards"));
		pNewChildControlsMenu->AppendMenu(MF_STRING, MI_PREACTION_AS_CHILD,		(LPCTSTR)_T("Pre-Action"));
		}
	}

// ######################################################
//
//	Operations.
//
// ######################################################

void
PokerTableWnd::SetConnectedMode(bool bConnected /*= true*/, bool bRedraw /*= true*/){
	m_bConnectedMode		= bConnected;
	m_bUseForegroundColor	= !bConnected;
	if( bRedraw ) Invalidate();
	}

bool
PokerTableWnd::Setup(CSize szDefault, PokerTableInfo* pTableInfo, EnumerableObject<PokerAvatar>* seatAvatars,
					std::string	sAmountLabelSufix /*= _T("GEL")*/, int nSeatCardsCt /*= 2*/, int nRegThinkTimeSec /*= 0*/)
	{
	m_szDefaultWindow							= szDefault;
	EnumerableObject<PokerTableSeatInfo>* seats	= &pTableInfo->m_seats;
	int nMaxPlayerCt							= seats->GetCount();

	if( IsDesignerMode() )
		return false;
	/*
	ESChildControl* pTable = GetChildControlByName(_T("_pokerTable1"));
	CDWordArray arrIds;
	pTable->GetChildsByVisibility(true, &arrIds);

	for(int i=0; i<arrIds.GetCount(); i++){
		ESChildControl* pChild = GetChildControl(arrIds[i]);
		if( pChild ){
			pChild->SetVisible(false, false, false);
			}
		}
	return true;
	*/

	PokerTableSettings *pSettings = PokerTableSettings::GetInstance();
	std::string sFormat;
	if( m_bUISetup ){
		for(int i=0; i<10; i++){
			m_arrChipPositions[0].x = 0.0f;
			m_arrChipPositions[0].y = 0.0f;
			}
		std::string sName;
		for(int i=1; i<=nMaxPlayerCt;  i++){
			sName.Format(_T("_pokerSeatStatus%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_pokerSeat%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_seatCards%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_seatHiddenCards%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_pokerChip%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_seatCards%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_seatActionLabel%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_userName%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_userBalance%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			sName.Format(_T("_dealer%d"), i);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			SetLogicalPositionAndSizeByName(sName, m_szDefaultWindow);

			// add seat events. {{
			sName.Format(_T("_pokerSeat%d"), i);
			ESChildSeat* pSeat = (ESChildSeat*)GetChildControlByName(sName.GetBuffer());
			if( pSeat ){
				pSeat->AddEventListener(this, ChildControlEventCode::EventCode_ControlMouseEnter, (ControlEventListener)&PokerTableWnd::OnSeatMouseEnter);
				pSeat->AddEventListener(this, ChildControlEventCode::EventCode_ControlMouseLeave, (ControlEventListener)&PokerTableWnd::OnSeatMouseLeave);
				// Timebank activate event.
				pSeat->AddEventListener(this, EventCode_SeatTimeBankActivated, (ControlEventListener)&PokerTableWnd::OnSeatTimeBankActivated);
				}
			// }}

			// Correction of seat cards and hidden cards depending on seat cards count. {{
			sName.Format(_T("_seatHiddenCards%d"), i);
			ESChildControlSeatHiddenCards* pSeatHiddenCards = (ESChildControlSeatHiddenCards*)GetChildControlByName(sName.GetBuffer());
			if( pSeatHiddenCards ){
				if( nSeatCardsCt == 2 )
					pSeatHiddenCards->SetCards(true, true, false, false, false);
				else
					if( nSeatCardsCt == 4 ){
						pSeatHiddenCards->SetCards(true, true, true, true, false);
						}

				// Set cardback index.	
				pSeatHiddenCards->SetCardBackIndex(pSettings->m_nCardsBackIndex, true);
				}
			
			sName.Format(_T("_seatCards%d"), i);
			ESChildControlSeatCards* pSeatCards = (ESChildControlSeatCards*)GetChildControlByName(sName.GetBuffer());
			if( pSeatCards ){
				pSeatCards->AddEventListener(this, ChildControlEventCode::EventCode_ControlMouseEnter, (ControlEventListener)&PokerTableWnd::OnSeatCardsMouseEnter);
				pSeatCards->AddEventListener(this, ChildControlEventCode::EventCode_ControlMouseLeave, (ControlEventListener)&PokerTableWnd::OnSeatCardsMouseLeave);

				if( nSeatCardsCt == 2 ){
					// Make 1 and 2 card in the middle of cards.
					CSize	szCard		= pSeatCards->GetCardDefaultSize();
					CPoint	ptOffset1	= pSeatCards->GetCard1Offset();
					CPoint	ptOffset2	= pSeatCards->GetCard2Offset();
					CPoint	ptOffset3	= pSeatCards->GetCard3Offset();

					ptOffset1.x			= ptOffset1.x + (szCard.cx + ptOffset2.x);
			//		pSeatCards->SetCard1Offset(ptOffset1);
					// Each card is depending on prevous one so we dont need to change second cards offset.
//					ptOffset2.x			= ptOffset1.x + (szCard.cx + ptOffset3.x);
//					pSeatCards->SetCard2Offset(ptOffset2);
					}
				else
					if( nSeatCardsCt == 4 ){
						}
				}
			// }}
			}

		// Set deck's card back index. {{
		ESChildControlSeatHiddenCards* pDeck = GetDeckControl();
		if( pDeck )
			pDeck->SetCardBackIndex(pSettings->m_nCardsBackIndex, false);
		// }}

		// Setup preactions. {{
		const std::string sArrPreActions[] = 
			{_T("_preactCallAnyCheck"), _T("_preactCheckFold"), _T("_preactCheck"), _T("_preactFold"), _T("_preactCall"), _T("_preactCallAny")};

		const int nArrPreActions[] = 
			{PreActionButton::PreAction_CallAnyCheck, PreActionButton::PreAction_CheckFold, PreActionButton::PreAction_Check, 
			PreActionButton::PreAction_Fold, PreActionButton::PreAction_Call, PreActionButton::PreAction_CallAny};

		if( !m_arrPreActionIdByPreAction.GetCount() ){
			for(int i=0; i<(sizeof(sArrPreActions)/sizeof(std::string)); i++){
				sName = sArrPreActions[i];
				int nId = GetChildControlIdByName(sName.GetBuffer());
				if( nId ){
					AddEventListenerByName(this, sName.GetBuffer(), ChildControlEventCode::EventCode_ControlClick, (ControlEventListener)&PokerTableWnd::OnPreActionButtonClick);
					m_arrPreActionIdByPreAction.Add((void*)nArrPreActions[i], (void*)nId);
					}
				}
			}
		// }}

		StringToIntArray arrControlNamesAndVisibility;
		arrControlNamesAndVisibility.Add(_T("_viewLobbyButton"), -1);
		arrControlNamesAndVisibility.Add(_T("_settingsButton"), -1);
		arrControlNamesAndVisibility.Add(_T("_leaveTableButton"), -1);
		arrControlNamesAndVisibility.Add(_T("_replayButton"), -1);
		arrControlNamesAndVisibility.Add(_T("_viewoptButton"), -1);

		arrControlNamesAndVisibility.Add(_T("_labelHandInfo"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_labelPreviousHand"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_shortcutButton1"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_shortcutButton2"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_shortcutButton3"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_shortcutButton4"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_actionButton1"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_actionButton2"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_actionButton3"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_betSlider"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_betAmount"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_tableCards"), (int)FALSE);
		// Main pot chips. 
		arrControlNamesAndVisibility.Add(_T("_pokerMainChips"), (int)FALSE);

		// Side pot chips.
		arrControlNamesAndVisibility.Add(_T("_sidePot1"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_sidePot2"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_sidePot3"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_sidePot4"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_sidePot5"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_sidePot6"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_sidePot7"), (int)FALSE);

		arrControlNamesAndVisibility.Add(_T("_deck"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_labelTotalPot"), (int)TRUE);
		arrControlNamesAndVisibility.Add(_T("_btnOptions"), (int)TRUE);
		arrControlNamesAndVisibility.Add(_T("_labelTableMessage"), -1);
		arrControlNamesAndVisibility.Add(_T("_labelCardsCombination"), (int)FALSE);

		arrControlNamesAndVisibility.Add(_T("_preactCallAnyCheck"), (int)0);
		arrControlNamesAndVisibility.Add(_T("_preactCheckFold"), (int)0);
		arrControlNamesAndVisibility.Add(_T("_preactCheck"), (int)0);
		arrControlNamesAndVisibility.Add(_T("_preactFold"), (int)0);
		arrControlNamesAndVisibility.Add(_T("_preactCall"), (int)0);
		arrControlNamesAndVisibility.Add(_T("_preactCallAny"), (int)0);

		arrControlNamesAndVisibility.Add(_T("_checkSitOutNextBB"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_checkSitOutNextHand"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_checkAutoPostBB"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_checkAutoMuck"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_checkAutoRebuy"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_checkAutoAddon"), (int)FALSE);
		arrControlNamesAndVisibility.Add(_T("_pokerChipRakeAmount"), (int)TRUE);
		arrControlNamesAndVisibility.Add(_T("_addonRebuyChips"), (int)TRUE);
		arrControlNamesAndVisibility.Add(_T("_addChipsButton"), (int)TRUE);
		arrControlNamesAndVisibility.Add(_T("_checkPostBBAndPlay"), (int)FALSE);

		// Chat page
//		arrControlNamesAndVisibility.Add(_T("_textChat"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_tabviewChat"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_tabctrl1"), (int)-1);
//		arrControlNamesAndVisibility.Add(_T("_renderviewChat"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_imageChatIcons"), (int)-1);

		// Notes
		arrControlNamesAndVisibility.Add(_T("_comboNoteUsers"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_userNoteDesciption"), (int)-1);

		// Table info.
		arrControlNamesAndVisibility.Add(_T("_tableInfoLabel1"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_tableInfoLabel2"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_tableInfoLabel3"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_tableInfoLabel4"), (int)-1);

		// Statistic
		arrControlNamesAndVisibility.Add(_T("_label3"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_label4"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_label5"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_label6"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_labelHandsPlayed"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_labelHandsPlayedPreFlop"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_labelHandsPlayedPostFlop"), (int)-1);
		arrControlNamesAndVisibility.Add(_T("_labelHandsPlayedShowdown"), (int)-1);
	
		for(int i=0; i<arrControlNamesAndVisibility.GetCount(); i++){
			sName = arrControlNamesAndVisibility.GetKey(i);
			int nVisible = arrControlNamesAndVisibility.GetValueByIndex(i);
			if( nVisible != -1 )
				SetControlVisibilityByName	(sName.GetBuffer(), (bool)(nVisible != 0), false);
			SetLogicalPositionAndSizeByName	(sName, m_szDefaultWindow);
			}

		// Add event listener methods. {{
		for(int i=1; i<=4; i++){
			sName.Format(_T("_actionButton%d"), i);
			AddEventListenerByName(this, sName.GetBuffer(), ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&PokerTableWnd::OnActionButtonClick);
			sName.Format(_T("_shortcutButton%d"), i);
			AddEventListenerByName(this, sName.GetBuffer(), ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&PokerTableWnd::OnShortcutButtonClick);
			}

		AddEventListenerByName(this, _T("_betSlider"), ChildControlEventCode::EventCode_SliderPosChanged, (ControlEventListener)&PokerTableWnd::OnBetSliderPosChanged);
		AddEventListenerByName(this, _T("_betAmount"), ChildControlEventCode::EventCode_EditTextChange, (ControlEventListener)&PokerTableWnd::OnBetAmountChanged);
		// }}

		// Set main chips pot label format.
		ESChildControlChips* pMainPotChips = GetMainChipsControl();
		if( pMainPotChips ){
	//		sFormat.Format(_T("%s"), PokerGUI::GetLangValue(_T("LABEL_MAIN_POT")));
	//		sFormat += _T(": %s ") + sAmountLabelSufix; 
			//sFormat = _T("Main Pot: %s ") + sAmountLabelSufix; 
			sFormat = _T("%s ") + sAmountLabelSufix; 
			pMainPotChips->SetFormatString(sFormat);
			}

		// Set max player count. {{
		int nIndex = m_acceptebleMaxPlayersCount.IndexOf((void*)nMaxPlayerCt);
		if( nIndex == -1 ){
			nMaxPlayerCt	= 2;
			nIndex			= 0;
			}
		// }}
		}

//	sFormat = _T("%d.%02d ") + sAmountLabelSufix;
	sFormat = _T("%s");
	// Set seat info. #############
	m_tableInfo.m_seats.RemoveAll();
	if( seats && seats->GetCount() > 0 ){
		for(int i=0; i<seats->GetCount(); i++){
			PokerTableSeatInfo* pSeatInfo = seats->GetAt(i);
			// Copy PokerTableSeatInfo class into new instance.
			if( pSeatInfo ){
				PokerTableSeatInfo* pSeatCopy = new PokerTableSeatInfo();
				pSeatInfo->Copy(pSeatCopy);
				pSeatInfo = pSeatCopy;
				}

			if( pSeatInfo ){
				PokerAvatar* pAvatarInfo = seatAvatars ? seatAvatars->GetAt(i) : NULL;
				if( pAvatarInfo ){
					if( pAvatarInfo->m_binImage.m_hData != NULL && pAvatarInfo->m_binImage.m_dwDataLength > 0 ){
						pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
						}
					else{
						/*
						std::string sAvatarFile = m_pGUI->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
						CImage* pAvatarImage = PokerGUI::LoadImage(sAvatarFile);
						if( pAvatarImage ){
							if( Serializable::BinaryFromImage(pAvatarImage, &pAvatarInfo->m_binImage) )
								pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
							delete pAvatarImage;
							}
*/
//						Path::BuildFullPath(sAvatarFile);
//						CImage avatar_image;
//						if( avatar_image.Load(sAvatarFile) == S_OK && Serializable::BinaryFromImage(&avatar_image, &pAvatarInfo->m_binImage) )
//							pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
//						avatar_image.Destroy();
						}
					}
				}

			ESChildSeat*			pSeat		= GetSeatControl(i);
			ESChildSeatStatus*		pSeatStatus	= GetSeatStatusControl(i);
			ESChildControlChips*	pChips		= GetChipsControl(i);
			if( !pSeat ) continue;

			// Seat
			pSeat->SetSeatIndex				(i);
			pSeat->SetSeatStatus			(pSeatInfo ? pSeatInfo->GetStatus() : PokerSeatStatus::Empty);
			pSeat->SetSeatActive			(false, false);
			pSeat->SetVisible				(true, false);
			pSeat->SetRegularThinkTimeSec	(nRegThinkTimeSec);
			pChips->SetFormatString			(sFormat);

			if( m_bUISetup ){
				SetLogicalPositionAndSize(pSeat,		szDefault);
				SetLogicalPositionAndSize(pSeatStatus,	szDefault);
				SetLogicalPositionAndSize(pChips,		szDefault);
				// Save chips position. {{
				CRect rcChips = pChips->GetClientRect();
				m_arrChipPositions[i].x	= (((float)rcChips.left) / szDefault.cx);
				m_arrChipPositions[i].y	= (((float)rcChips.top) / szDefault.cy);
				// }}
				pSeat->AddEventListener	(this, ChildControlEventCode::EventCode_ControlClick, (ControlEventListener)&PokerTableWnd::OnSeatClick);
				}

			if( pSeatStatus )
				pSeatStatus->SetSeatIndex(i);

			m_tableInfo.m_seats.Add(pSeatInfo);
			UpdateSeat(i, false);

			// Set seat hand cards class.
//			if( pSeatInfo )
//				SetHandCards(i, (PokerHandCardsClass)pSeatInfo->GetHandCardsClass());
			}
		}

	// Set current round. #########
	m_tableInfo.m_round	= pTableInfo->m_round;
	m_tableInfo.m_cCard1 = pTableInfo->m_cCard1; 
	m_tableInfo.m_cCard2 = pTableInfo->m_cCard2;
	m_tableInfo.m_cCard3 = pTableInfo->m_cCard3;
	m_tableInfo.m_cCard4 = pTableInfo->m_cCard4;
	m_tableInfo.m_cCard5 = pTableInfo->m_cCard5;

	ESChildControlTableCards* pTableCards = GetTableCardsControl();
	if( pTableCards ){
		pTableCards->SetVisible(true, false);
		pTableCards->SetCard1(m_tableInfo.m_cCard1);
		pTableCards->SetCard2(m_tableInfo.m_cCard2);
		pTableCards->SetCard3(m_tableInfo.m_cCard3);
		pTableCards->SetCard4(m_tableInfo.m_cCard4);
		pTableCards->SetCard5(m_tableInfo.m_cCard5);
		}
	// ############################

	// Setup chat view. {{
	ESSimpleRenderView* pRenderView = (ESSimpleRenderView*)GetChildControlByName(_T("_renderviewChat"));
	if( pRenderView ){
		pRenderView->ClearContent(false);

//Temp
//		std::string sText;
//		for(int i=0; i<20; i++){
//			AddChatMessage_TimeToAct(i + 20);

//			sText.Format(_T("You have %d sec to act\r\nfgdfgdgd"), 11111);
//			pRenderView->AddText(_T("LABEL1"), _T("Dealer: "), false, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
//			pRenderView->AddText(_T("MSG1"), sText, true, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
//			}
//Temp
		}
	// }}
	m_nSeatCardsCt			= nSeatCardsCt;
	m_bUISetup				= false;
	m_sAmountLabelSuffix	= sAmountLabelSufix;
	return true;
	}

bool
PokerTableWnd::RotateSeatsClockwise(int nFirstSeatIndexClockwise, bool bRedraw){
	// ZqrTalent.
	/* We should rename controls and change some parameters to look differently and remain logic the same as it is.

	_pokerSeat1 -> seat index.
	_seatCards1
	_seatActionLabel1
	_pokerSeatStatus1 -> seat index.
	_userName1
	_userBalance1
	_seatHiddenCards1
	_pokerChip1
	*/
	int nMaxSeatCt = m_tableInfo.m_seats.GetCount();
	if( !nMaxSeatCt ) return false;
	nFirstSeatIndexClockwise %= nMaxSeatCt;
//	if( nFirstSeatIndexClockwise == m_nFirstSeatIndexClockwise )
//		return true;

	bool	bExists = false;
	std::string sControlName, sControlNameNew;
	int		nSeat = 0;
	ESChildControl* pControl = NULL;

	const std::string sArrControlNames[] = 
	{_T("_pokerSeat"), _T("_seatCards"), _T("_seatActionLabel"), _T("_pokerSeatStatus"), _T("_userName"), _T("_userBalance"), _T("_seatHiddenCards"), _T("_pokerChip"), _T("_dealer")};
	
	int nActiveSeat			= m_tableInfo.m_cActiveSeat;
	m_tableInfo.m_cActiveSeat	= -1;

	nSeat = nMaxSeatCt;
	for(int i=nFirstSeatIndexClockwise-1; i>=0; i--){
		int nLoop = 0, nCt = (sizeof(sArrControlNames) / sizeof(std::string));
		while(nLoop < nCt){
			sControlName.Format(_T("%s%d"), sArrControlNames[nLoop],  nSeat);
			sControlNameNew.Format(_T("%s%d_"), sArrControlNames[nLoop], i+1);
			pControl = GetChildControlByName(sControlName.GetBuffer());
			if( pControl ){
				SetChildControlName(pControl, sControlNameNew, bExists);

				if( nLoop == 0 ){ // Seat.
					ESChildSeat* pSeat = ((ESChildSeat*)pControl);
					PokerTableSeatInfo* pSeatInfo = GetSeatInfo(i);

					// Copy border color and user note string.
					sControlNameNew.Format(_T("%s%d"), sArrControlNames[nLoop], i+1);
					ESChildSeat* pSeatRenameAs = ((ESChildSeat*)GetChildControlByName(sControlNameNew.GetBuffer()));
					if( pSeatRenameAs ){
						COLORREF crBorder;
						if( pSeatRenameAs->GetBorderColor(crBorder) ){
							pSeatRenameAs->ClearBorderColor(false);
							pSeat->SetBorderColor(crBorder, false);
							}
						else
							pSeat->ClearBorderColor(false);

						pSeat->SetUserNote(pSeatRenameAs->GetUserNote(), false);
						pSeatRenameAs->ClearUserNote(false);
						}

					// Remove active state and stop action timer. {{
					pSeat->SetSeatActive(false, false);
					pSeat->ActivateTimer(0, false);
					// }}

					pSeat->SetSeatIndex	(i);
					pSeat->SetSeatInfo	(pSeatInfo);
					}
				else
				if( nLoop == 3 ){ // SeatStatus.
					((ESChildSeatStatus*)pControl)->SetSeatIndex(i);
					}
				}
			nLoop ++;
			}
		nSeat --;
		}

	nSeat = 1;
	for(int i=nFirstSeatIndexClockwise; i<nMaxSeatCt; i++){
		int nLoop = 0, nCt = (sizeof(sArrControlNames) / sizeof(std::string));
		while(nLoop < nCt){
			sControlName.Format(_T("%s%d"), sArrControlNames[nLoop], nSeat);
			sControlNameNew.Format(_T("%s%d_"), sArrControlNames[nLoop], i+1);
			pControl = GetChildControlByName(sControlName.GetBuffer());
			if( pControl ){
				SetChildControlName(pControl, sControlNameNew, bExists);
				if( nLoop == 0 ){ // Seat.
					ESChildSeat* pSeat = ((ESChildSeat*)pControl);
					PokerTableSeatInfo* pSeatInfo = GetSeatInfo(i);

					// Copy border color and user note string.
					sControlNameNew.Format(_T("%s%d"), sArrControlNames[nLoop], i+1);
					ESChildSeat* pSeatRenameAs = ((ESChildSeat*)GetChildControlByName(sControlNameNew.GetBuffer()));
					if( pSeatRenameAs ){
						COLORREF crBorder;
						if( pSeatRenameAs->GetBorderColor(crBorder) ){
							pSeatRenameAs->ClearBorderColor(false);
							pSeat->SetBorderColor(crBorder, false);
							}
						else
							pSeat->ClearBorderColor(false);

						pSeat->SetUserNote(pSeatRenameAs->GetUserNote(), false);
						pSeatRenameAs->ClearUserNote(false);
						}

					// Remove active state and stop action timer. {{
					pSeat->SetSeatActive(false, false);
					pSeat->ActivateTimer(0, false);
					// }}

					pSeat->SetSeatIndex	(i);
					pSeat->SetSeatInfo	(pSeatInfo);
					}
				else
				if( nLoop == 3 ){ // SeatStatus.
					((ESChildSeatStatus*)pControl)->SetSeatIndex(i);
					}
				}
			nLoop ++;
			}
		nSeat ++;
		}

	// Rename controls. {{
	CRect rcBoundRect(0, 0, 0, 0);
	for(int i=0; i<nMaxSeatCt; i++){
		int nLoop = 0, nCt = (sizeof(sArrControlNames) / sizeof(std::string));
		while(nLoop < nCt){
			sControlName.Format(_T("%s%d_"), sArrControlNames[nLoop],  i+1);
			sControlNameNew.Format(_T("%s%d"), sArrControlNames[nLoop], i+1);
			pControl = GetChildControlByName(sControlName.GetBuffer());
			if( pControl ){
				SetChildControlName(pControl, sControlNameNew, bExists);
				rcBoundRect |= pControl->GetClientRect();
				}
			nLoop ++;
			}

		// Save chips position. {{
		ESChildControlChips* pChips = GetChipsControl(i);
		if( pChips ){
			/*
			float fZoomCX = 1.0f, fZoomCY = 1.0f;
			GetZoom(fZoomCX, fZoomCY);

			CRect rcChips = pChips->GetClientRect();
			m_arrChipPositions[i].x	= rcChips.left / fZoomCX;
			m_arrChipPositions[i].y	= rcChips.top / fZoomCY;
			*/
			m_arrChipPositions[i].x	= pChips->GetLogicalPosX();
			m_arrChipPositions[i].y	= pChips->GetLogicalPosY();
			}
		// }}

		UpdateSeat		(i, false, false);
		UpdateSeatStatus(i, false);
		}
	// }}

	// Activate seat.
	PokerTableSeatInfo* pInfo = GetSeatInfo(nActiveSeat);
	if( pInfo && pInfo->GetStatus() != PokerSeatStatus::Empty )
		SetActiveSeat(nActiveSeat, true, false);
	m_nFirstSeatIndexClockwise = nFirstSeatIndexClockwise;

	// Update dealer seat.
	SetDealerSeat(m_tableInfo.m_cDealerSeat, false);

	if( bRedraw )
		RedrawRect(rcBoundRect, 1);
	return true;
	}

int
PokerTableWnd::GetMaxPotAmount(){
	int nMaxPotAmount = 0;
	for( int i=0; i<m_tableInfo.m_seats.GetCount(); i++ ){
		PokerTableSeatInfo* pSeatInfo = m_tableInfo.m_seats.GetAt(i);
		if( pSeatInfo )
			nMaxPotAmount = max(nMaxPotAmount, pSeatInfo->GetPotAmount());
		}
	return nMaxPotAmount;
	}

bool
PokerTableWnd::SetSeatAvatarInfo(int nSeat, PokerAvatar* pAvatarInfo, bool bRedraw /*= true*/){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;

	if( pAvatarInfo ){
		if( pAvatarInfo->m_binImage.m_hData != NULL && pAvatarInfo->m_binImage.m_dwDataLength > 0 ){
			pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
			}
		else{
			/*
			std::string sAvatarFile = m_pGUI->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
			CImage* pAvatarImage = PokerGUI::LoadImage(sAvatarFile);
			if( pAvatarImage ){
				if( Serializable::BinaryFromImage(pAvatarImage, &pAvatarInfo->m_binImage) )
					pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
				delete pAvatarImage;
				}*/
/*
			std::string sAvatarFile = m_pGUI->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
			Path::BuildFullPath(sAvatarFile);
			CImage avatar_image;
			if( avatar_image.Load(sAvatarFile) == S_OK && Serializable::BinaryFromImage(&avatar_image, &pAvatarInfo->m_binImage) ){
				pSeatInfo->SetAvatarData(&pAvatarInfo->m_binImage);
				}
			avatar_image.Destroy();*/
			}
		}
	
	if( bRedraw )
		RedrawSeat(nSeat);
	return true;
	}

bool
PokerTableWnd::SetSeatInfo(int nSeatIndex, PokerTableSeatInfo* pInfo, bool bRedraw /*= true*/){
	if( nSeatIndex < 0 || nSeatIndex >= m_tableInfo.m_seats.GetCount() )
		return false;
/*
	// Keep avatar image of existing seat.
	PokerTableSeatInfo* pSeatExisting = m_tableInfo.m_seats.GetAt(nSeatIndex);
	if( pSeatExisting && pInfo && pSeatExisting->GetUserId() == pInfo->GetUserId() ){
		pInfo->SetAvatarImage(pSeatExisting->GetAvatarImage());
		pSeatExisting->SetAvatarImage(NULL); // Clear avatar image of old one.
		}
*/
	if( !m_tableInfo.m_seats.SetAt(nSeatIndex, pInfo, true) )
		return false;
	
	UpdateSeat(nSeatIndex, bRedraw);
	return true;
	}

void
PokerTableWnd::CopyTableSeats(PokerTableInfo* pTableInfoInto){
	if( !pTableInfoInto ) return;
	pTableInfoInto->m_seats.RemoveAll(true);
	m_tableInfo.m_seats.Copy_Into(&pTableInfoInto->m_seats);
	}

bool
PokerTableWnd::EmptySeat(int nSeatIndex, bool bRedraw){
	ClearSeatNote		(nSeatIndex, false);
	ClearSeatNoteColor	(nSeatIndex, false);
	if( GetActiveSeat() == nSeatIndex )
		SetActiveSeat(-1, false, false);
	SetSeatStatus	(nSeatIndex, (PokerSeatStatus)PokerSeatStatus::Empty, false);
	SetSeatInfo		(nSeatIndex, NULL, bRedraw);
	SetSeatCards	(nSeatIndex, -1, -1, -1, -1, bRedraw);
	return false;
	}

void
PokerTableWnd::EmptyAllEmptyStatusSeat(bool bRedraw){
	for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = GetSeatInfo(i);
		if( pSeat && pSeat->GetStatus() == PokerSeatStatus::Empty )
			EmptySeat(i, bRedraw);
		}
	}

int
PokerTableWnd::GetEmptySeatCt(){
	int nEmptySeatCt = 0;
	for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
		PokerTableSeatInfo* pSeat = GetSeatInfo(i);
		if( !pSeat || pSeat->GetStatus() == PokerSeatStatus::Empty )
			nEmptySeatCt ++;
		}
	return nEmptySeatCt;
	}

bool
PokerTableWnd::UpdateSeat(int nSeatIndex, bool bRedraw, bool bUpdateOnlySeat /*= false*/){
	PokerTableSeatInfo*				pInfo				= m_tableInfo.m_seats.GetAt(nSeatIndex);
	ESChildSeat*					pSeat				= GetSeatControl(nSeatIndex);
	ESChildControlSeatCards*		pSeatCards			= GetSeatCardsControl(nSeatIndex);
	ESChildControlSeatHiddenCards*	pSeatHiddenCards	= GetSeatHiddenCardsControl(nSeatIndex);
	ESChildSeatStatus*				pSeatStatus			= GetSeatStatusControl(nSeatIndex);
	PokerTableSettings*				pSettings			= PokerTableSettings::GetInstance();

	// Update action label.
	if( pSeat ){
		if( pInfo && pInfo->GetStatus() != PokerSeatStatus::Empty )
			pSeat->SetActionName(pInfo->GetActionName(), 3000, bRedraw);
		else
			pSeat->SetActionName(_T("{{Sit Here}}"), -1, bRedraw);
		}

	if( bUpdateOnlySeat )
		return true;

	bool							bCardsAvailable			= !pInfo ? false : (pInfo->GetCard1() > -1 || pInfo->GetCard2() > -1 || pInfo->GetCard3() > -1 || pInfo->GetCard4() > -1);
	bool							bHiddenCardsAvailable	= !pInfo ? false : (pInfo->GetCard1() != -2 || pInfo->GetCard2() != -2 || pInfo->GetCard3() != -2 || pInfo->GetCard4() != -2);
	if( pSeatCards ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty || !bCardsAvailable /*|| pInfo->GetCard1() < 0*/ ){
			if( !pSeatHiddenCards && bHiddenCardsAvailable ){
				pSeatCards->SetCards(pInfo->GetCard1(), pInfo->GetCard2(), pInfo->GetCard3(), pInfo->GetCard4(), false);
				pSeatCards->SetVisible(true, bRedraw);
				}
			else{
				pSeatCards->SetVisible		(false, bRedraw);
				pSeatCards->HideCards		(false);
				pSeatCards->AllowMouseEvents(false);
				}
			}
		else{
			if( pInfo->GetStatus() != PokerSeatStatus::All_In && pInfo->GetStatus() != PokerSeatStatus::ShownCards && pInfo->GetStatus() != PokerSeatStatus::Folded ){
				pSeatCards->AllowMouseEvents(true);
				pSeatCards->HideCards		(pSettings->m_bHideHoleCards);
				}
			else{
				pSeatCards->HideCards		(false);
				pSeatCards->AllowMouseEvents(false);
				}
			pSeatCards->SetCards(pInfo->GetCard1(), pInfo->GetCard2(), pInfo->GetCard3(), pInfo->GetCard4(), false);
			pSeatCards->SetVisible(true, bRedraw);
			}
		}

	if( pSeatHiddenCards ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty || pInfo->GetCard1() != -2 )
			pSeatHiddenCards->SetVisible(false, bRedraw);
		else{
			pSeatHiddenCards->SetCards((pInfo->GetCard1() == -2), (pInfo->GetCard2() == -2), (pInfo->GetCard3() == -2), (pInfo->GetCard4() == -2), false);
			pSeatHiddenCards->SetVisible(true, bRedraw);
			}
		}

	/*
	if( pSeatCards ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty || pInfo->GetStatus() == PokerSeatStatus::Folded || pInfo->GetCard1() < 0 ){
			pSeatCards->HideCards		(false);
			pSeatCards->AllowMouseEvents(false);
			pSeatCards->SetVisible		(false, bRedraw);
			}
		else{
			if( pInfo->GetStatus() != PokerSeatStatus::All_In && pInfo->GetStatus() != PokerSeatStatus::ShownCards && pInfo->GetStatus() != PokerSeatStatus::Folded ){
				pSeatCards->AllowMouseEvents(true);
				pSeatCards->HideCards		(pSettings->m_bHideHoleCards);
				}
			else{
				pSeatCards->HideCards		(false);
				pSeatCards->AllowMouseEvents(false);
				}

			pSeatCards->SetCards(pInfo->GetCard1(), pInfo->GetCard2(), pInfo->GetCard3(), pInfo->GetCard4(), false);
			pSeatCards->SetVisible(true, bRedraw);
			}
		}

	if( pSeatHiddenCards ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty  || pInfo->GetStatus() == PokerSeatStatus::Folded || pInfo->GetCard1() != -2 )
			pSeatHiddenCards->SetVisible(false, bRedraw);
		else{
			pSeatCards->SetCards((pInfo->GetCard1() == -2), (pInfo->GetCard2() == -2), (pInfo->GetCard3() == -2), (pInfo->GetCard4() == -2), false);
			pSeatHiddenCards->SetVisible(true, bRedraw);
			}
		}
	*/

	pSeat->SetSeatInfo	(pInfo);
	if( pInfo && pInfo->GetStatus() != PokerSeatStatus::Empty ){
		pSeat->SetSeatStatus(pInfo->GetStatus());
		// Update seat avatar. {{
		if( pInfo->GetUserId() == m_pClient->GetUserId() ){
			CImage* pAvatarImage = m_pClient->GetAvatarImage();
			if( pAvatarImage && !pAvatarImage->IsNull() )
				pSeat->SetAvatarImage(pAvatarImage, pInfo->GetAvatarId(), false);
			else
				pSeat->ClearAvatarImage(false);
			}
		else{
			if( pInfo->GetAvatarId() != pSeat->GetAvatarId() ){
				CImage avatarImage;
				if( Serializable::LoadJpgImage(&avatarImage, m_pClient->GetAvatarImageFileById(pInfo->GetAvatarId())) ){
					pSeat->SetAvatarImage(&avatarImage, pInfo->GetAvatarId(), false);
					}
				else
					pSeat->ClearAvatarImage(false);
				}
			}
		// }}
		
		/*
		if( pSeat->GetSeatIndex() == GetActiveSeat() ){
			pSeat->SetSeatActive(true, false);
			pSeat->ActivateTimer(pInfo->IsTimerActive());
			}*/
		}
	else{
		pSeat->SetSeatStatus	(PokerSeatStatus::Empty);
		pSeat->ClearAvatarImage	(false);
		}

	UpdateSeatStatus	(nSeatIndex, bRedraw);
	UpdateSeatChips		(nSeatIndex, bRedraw);
	return true;
	}

bool
PokerTableWnd::UpdateSeatStatus(int nSeatIndex, bool bRedraw){
	PokerTableSeatInfo*		pInfo		= m_tableInfo.m_seats.GetAt(nSeatIndex);
	ESChildSeatStatus*		pSeatStatus	= GetSeatStatusControl(nSeatIndex);

	if( pSeatStatus ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty ){
			std::string sName;
			sName.Format(_T("_userName%d"), nSeatIndex + 1);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);
			sName.Format(_T("_userBalance%d"), nSeatIndex + 1);
			SetControlVisibilityByName(sName.GetBuffer(), false, false);

			pSeatStatus->SetVisible(false, bRedraw);
			}
		else{
			std::string sBalance;
			int		nBalance = pInfo->GetBalance();
			if( nBalance%100 > 0 )
				sBalance.Format(_T("%d.%02d %s"), nBalance/100, nBalance%100, m_sAmountLabelSuffix);
			else
				sBalance.Format(_T("%d %s"), nBalance/100, m_sAmountLabelSuffix);
			pSeatStatus->SetVisible		(true, false);

			if( pInfo->GetStatus() == PokerSeatStatus::SittedOut )
				sBalance  = _T("{{Sitted Out}}");
			else
				if( pInfo->GetStatus() == PokerSeatStatus::Reserved )
					sBalance  = _T("{{Sitting}}");
				else
					if( pInfo->IsOffline() )
						sBalance = _T("{{Disconnected}}");

			std::string sName;
			sName.Format(_T("_userName%d"), nSeatIndex + 1);
			SetControlVisibilityByName(sName.GetBuffer(), true, false);
			sName.Format(_T("_userBalance%d"), nSeatIndex + 1);
			SetControlVisibilityByName(sName.GetBuffer(), true, false);

			pSeatStatus->SetUserBalance	(sBalance, false);
			pSeatStatus->SetUserName	(pInfo->GetUserName(), false);
			if( bRedraw )
				pSeatStatus->Redraw();
			}
		}
	return true;
	}

bool
PokerTableWnd::UpdateSeatCards(int nSeatIndex, bool bRedraw){
	PokerTableSeatInfo*				pInfo				= m_tableInfo.m_seats.GetAt(nSeatIndex);
	ESChildControlSeatCards*		pSeatCards			= GetSeatCardsControl(nSeatIndex);
	ESChildControlSeatHiddenCards*	pSeatHiddenCards	= GetSeatHiddenCardsControl(nSeatIndex);
	PokerTableSettings*				pSettings			= PokerTableSettings::GetInstance();
	bool							bCardsAvailable			= !pInfo ? false : (pInfo->GetCard1() > -1 || pInfo->GetCard2() > -1 || pInfo->GetCard3() > -1 || pInfo->GetCard4() > -1);
	bool							bHiddenCardsAvailable	= !pInfo ? false : (pInfo->GetCard1() != -2 || pInfo->GetCard2() != -2 || pInfo->GetCard3() != -2 || pInfo->GetCard4() != -2);

	if( pSeatCards ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty || !bCardsAvailable /*|| pInfo->GetCard1() < 0*/ ){
			if( !pSeatHiddenCards && bHiddenCardsAvailable ){
				pSeatCards->SetCards(pInfo->GetCard1(), pInfo->GetCard2(), pInfo->GetCard3(), pInfo->GetCard4(), false);
				pSeatCards->SetVisible(true, bRedraw);
				}
			else{
				pSeatCards->SetVisible		(false, bRedraw);
				pSeatCards->HideCards		(false);
				pSeatCards->AllowMouseEvents(false);
				}
			}
		else{
			if( pInfo->GetStatus() != PokerSeatStatus::All_In && pInfo->GetStatus() != PokerSeatStatus::ShownCards && pInfo->GetStatus() != PokerSeatStatus::Folded ){
				pSeatCards->AllowMouseEvents(true);
				pSeatCards->HideCards		(pSettings->m_bHideHoleCards);
				}
			else{
				pSeatCards->HideCards		(false);
				pSeatCards->AllowMouseEvents(false);
				}
			pSeatCards->SetCards(pInfo->GetCard1(), pInfo->GetCard2(), pInfo->GetCard3(), pInfo->GetCard4(), false);
			pSeatCards->SetVisible(true, bRedraw);
			}
		}

	if( pSeatHiddenCards ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty || pInfo->GetCard1() != -2 )
			pSeatHiddenCards->SetVisible(false, bRedraw);
		else{
			pSeatHiddenCards->SetCards((pInfo->GetCard1() == -2), (pInfo->GetCard2() == -2), (pInfo->GetCard3() == -2), (pInfo->GetCard4() == -2), false);
			pSeatHiddenCards->SetVisible(true, bRedraw);
			}
		}

	if( pSeatHiddenCards ){
		if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty || pInfo->GetCard1() != -2 )
			pSeatHiddenCards->SetVisible(false, bRedraw);
		else{
			pSeatHiddenCards->SetCards((pInfo->GetCard1() == -2), (pInfo->GetCard2() == -2), (pInfo->GetCard3() == -2), (pInfo->GetCard4() == -2), false);
			pSeatHiddenCards->SetVisible(true, bRedraw);
			}
		}
	return true;
	}

bool
PokerTableWnd::UpdateSeatChips(int nSeatIndex, bool bRedraw){
	PokerTableSeatInfo*		pInfo	= m_tableInfo.m_seats.GetAt(nSeatIndex);
	ESChildControlChips*	pChips	= GetChipsControl(nSeatIndex);
	if( !pChips ) return false;
	pChips->SetAmount(pInfo ? pInfo->GetPotAmount() : 0, false);
	if( !pChips->IsVisible() )
		pChips->SetVisible(true, false);
	if( bRedraw )
		pChips->Redraw();
	return true;
	}

bool
PokerTableWnd::UpdateSeatAvatar(int nSeatIndex, int nAvatarId,  CImage* pAvatarImage, bool bRedraw){
	PokerTableSeatInfo*				pInfo	= GetSeatInfo(nSeatIndex);
	ESChildSeat*					pSeat	= GetSeatControl(nSeatIndex);
	if( !pSeat ) return false;

	if( pSeat->GetAvatarId() == nAvatarId )
		return true;
	if( pAvatarImage && !pAvatarImage->IsNull() )
		pSeat->SetAvatarImage(pAvatarImage, nAvatarId, bRedraw);
	else
		pSeat->ClearAvatarImage(bRedraw);
	return true;
	}

void
PokerTableWnd::SetRound(PokerRoundType round, CDWordArray *pListTableCards, bool bInvalidate /*= false*/, bool bSetTableCards /*= true*/){
	if( round == PokerRoundType::PreFlop )
		m_tableInfo.m_nRakeAmount = 0;

	m_tableInfo.m_round = round;
	if( !pListTableCards ){
		if( bSetTableCards )
			SetTableCards(-1, -1, -1, -1, -1, bInvalidate);
		return;
		}

	char *pCards[]	= {&m_tableInfo.m_cCard1, &m_tableInfo.m_cCard2, &m_tableInfo.m_cCard3, &m_tableInfo.m_cCard4, &m_tableInfo.m_cCard5};
	int nLoop		= 0, nCt = min(pListTableCards->GetCount(), 5);

	while( nLoop < nCt ){
		*pCards[nLoop] = (char)pListTableCards->GetAt(nLoop);
		nLoop ++;
		}

	if( bSetTableCards )
		SetTableCards(m_tableInfo.m_cCard1, m_tableInfo.m_cCard2, m_tableInfo.m_cCard3, m_tableInfo.m_cCard4, m_tableInfo.m_cCard5, bInvalidate);
	}

bool
PokerTableWnd::SetupSeatTimer(int nSeat, int nFullSeconds, int nSecondsPassed, bool bInvalidate /*= false*/, bool bStart /*= false*/){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return false;
	pSeat->SetupTimer(nFullSeconds, nSecondsPassed);
	// Start or stop action time.
	StartActionTimer(nSeat, bStart, bInvalidate);
	return true;
	}

void
PokerTableWnd::SetDealerSeat(int nSeatIndex, bool bInvalidate){
	int nLoop = 0;
	while( nLoop < m_tableInfo.m_seats.GetCount() ){
		ESChildControlImage* pDealer = GetSeatDealerControl(nLoop);
		if( pDealer ){
			if( nSeatIndex == nLoop )
				pDealer->SetVisible(true, bInvalidate);
			else{
				if( pDealer->IsVisible() )
					pDealer->SetVisible(false, bInvalidate);
				}
			}
		nLoop ++;
		}
	m_tableInfo.m_cDealerSeat = nSeatIndex;
	}

void		
PokerTableWnd::SetTableCards(int nCard1, int nCard2, int nCard3, int nCard4 /*= -1*/, int nCard5 /*= -1*/, bool bInvalidate /*= true*/){
	m_tableInfo.m_cCard1 = nCard1;
	m_tableInfo.m_cCard2 = nCard2;
	m_tableInfo.m_cCard3 = nCard3;
	m_tableInfo.m_cCard4 = nCard4;
	m_tableInfo.m_cCard5 = nCard5;

	ESChildControlTableCards* pTableCards = GetTableCardsControl();
	if( pTableCards ){
		pTableCards->SetCard1(m_tableInfo.m_cCard1);
		pTableCards->SetCard2(m_tableInfo.m_cCard2);
		pTableCards->SetCard3(m_tableInfo.m_cCard3);
		pTableCards->SetCard4(m_tableInfo.m_cCard4);
		pTableCards->SetCard5(m_tableInfo.m_cCard5);
		pTableCards->SetMarkCards(false, false, false, false, false, false);

		if( bInvalidate )
			pTableCards->Redraw();
		}
	}

bool
PokerTableWnd::SetSeatCards(int nSeat, int nCard1, int nCard2, int nCard3, int nCard4, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ){
		UpdateSeatCards(nSeat, bInvalidate);
		return false;
		}
	pSeat->SetCard1(nCard1);
	pSeat->SetCard2(nCard2);
	pSeat->SetCard3(nCard3);
	pSeat->SetCard4(nCard4);
	UpdateSeatCards(nSeat, bInvalidate);
	return true;
	}

int
PokerTableWnd::GetSeatCardByIndex(int nSeat, int nCardIndex){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat || nCardIndex < 0 || nCardIndex > 3 )
		return -1;
	int nArrCards[] = {pSeat->GetCard1(), pSeat->GetCard2(), pSeat->GetCard3(), pSeat->GetCard4()};
	return nArrCards[nCardIndex];
	}

void		
PokerTableWnd::SetTotalPotAmount(int nTotalPotAmount, bool bRedraw){
	int nAmountOld				= m_tableInfo.m_nTotalPotAmount;
	m_tableInfo.m_nTotalPotAmount	= nTotalPotAmount;
	ESChildControlLabel* pLabel = (ESChildControlLabel*)GetChildControlByName(_T("_labelTotalPot"));
	if( pLabel ){
		if( m_tableInfo.m_nTotalPotAmount > 0 ){
			std::string sLabel;
			if( m_tableInfo.m_nTotalPotAmount%100 > 0 )
				sLabel.Format(_T("{{Pot}}:%d.%02d %s"), m_tableInfo.m_nTotalPotAmount/100, m_tableInfo.m_nTotalPotAmount%100, m_sAmountLabelSuffix);
			else
				sLabel.Format(_T("{{Pot}}:%d %s"), m_tableInfo.m_nTotalPotAmount/100, m_sAmountLabelSuffix);
			pLabel->SetLabel(sLabel, bRedraw);
			}
		else
			pLabel->SetLabel(_T(""), bRedraw);
		}
	}

void		
PokerTableWnd::SetMainPotAmount(int nMainPotAmount, EnumerableObject<PokerPotAmountDetail>* pListPots, bool bRedraw, bool bChangeOnlyMainPotChips /*= false*/){
//	if( m_tableInfo.m_nMainPotAmount == nMainPotAmount )
//		return;
	ESChildControlChips*	pMainChips	= GetMainChipsControl();
	int						nAmountOld	= m_tableInfo.m_nMainPotAmount;
	m_tableInfo.m_nMainPotAmount				= nMainPotAmount;

	if( nMainPotAmount <= 0 ){
		for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
			// Main pot amount.
			if( !i ){
				if( pMainChips ){
					pMainChips->SetAmount(0, false);
					if( pMainChips->IsVisible() )
						pMainChips->SetVisible(false, bRedraw);
					}
				if( bChangeOnlyMainPotChips ) break;
				}
			// Side pot.
			else{
				ESChildControlChips* pSidePotChips = GetSidePotChipsControl(i-1);
				if( pSidePotChips ){
					pSidePotChips->SetAmount(0, false);
					if( pSidePotChips->IsVisible() )
						pSidePotChips->SetVisible(false, bRedraw);
					}
				}
			}
		return;
		}

	if( pListPots && pListPots->GetCount() > 0 ){
		for(int i=0; i<pListPots->GetCount(); i++){
			PokerPotAmountDetail*	pPotInfo		=  pListPots->GetAt(i);
			int						nAmountRakeFree = (pPotInfo->m_nAmount - pPotInfo->m_wRakeAmount);

			// Main pot amount.
			if( !i ){
				if( pMainChips ){
					if( !pMainChips->IsVisible() )
						pMainChips->SetVisible(true, false);
					if( pMainChips->GetAmount() != nAmountRakeFree )
						pMainChips->SetAmount(nAmountRakeFree, bRedraw);
					}
				if( bChangeOnlyMainPotChips ) break;
				}
			// Side pot.
			else{
				ESChildControlChips* pSidePotChips = GetSidePotChipsControl(i-1);
				if( pSidePotChips ){
					if( !pSidePotChips->IsVisible() )
						pSidePotChips->SetVisible(true, false);
					if( pSidePotChips->GetAmount() != nAmountRakeFree )
						pSidePotChips->SetAmount(nAmountRakeFree, bRedraw);
					}
				}
			}
		}
	else{
		if( pMainChips ){
			if( !pMainChips->IsVisible() )
				pMainChips->SetVisible(true, false);
			if( pMainChips->GetAmount() != nMainPotAmount )
				pMainChips->SetAmount(nMainPotAmount, bRedraw);
			}
		}
	}

int
PokerTableWnd::GetPotAmount(int nSeat){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return 0;
	return pSeat->GetPotAmount();
	}

bool
PokerTableWnd::SetPotAmount(int nSeat, int nAmount, bool bRedraw /*= true*/){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return false;
	pSeat->SetPotAmount	(nAmount);
	UpdateSeatChips		(nSeat, bRedraw);
	return true;
	}

int
PokerTableWnd::GetSeatTotalPot(int nSeat){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return 0;
	return pSeat->GetTotalPot();
	}

bool
PokerTableWnd::SetSeatTotalPot(int nSeat, int nAmount){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return false;
	int nAmountOld = pSeat->GetTotalPot();
	pSeat->SetTotalPot(nAmount);
	return true;
	}

int
PokerTableWnd::GetBalance(int nSeat){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return 0;
	return pSeat->GetBalance();
	}

bool
PokerTableWnd::SetBalance(int nSeat, int nBalance, bool bRedraw /*= true*/){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return false;
	pSeat->SetBalance	(nBalance);
	UpdateSeatStatus	(nSeat, bRedraw);
	return true;
	}

bool
PokerTableWnd::SetRakeAmount(int nAmount, bool bRedraw){
	m_tableInfo.m_nRakeAmount = nAmount;
	ESChildControlChips* pRakeChips = (ESChildControlChips*)GetChildControlByName(_T("_pokerChipRakeAmount"));
	if( !pRakeChips )
		return false;
	pRakeChips->SetAmount(nAmount, bRedraw);
	return true;
	}

bool
PokerTableWnd::StartActionTimer(int nSeat, bool bStart /*= true*/, bool bInvalidate /*= false*/){
	PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
	if( !pSeat ) return false;
	if( bStart ){ // Start
		// Stop recently active seat timer.
		if( m_pTimerActiveSeat ){
			int				nSeatRecentlyActive = m_pTimerActiveSeat->GetSeat();
			ESChildSeat*	pSeatControl		= GetSeatControl(nSeatRecentlyActive);
			if( pSeatControl )
				pSeatControl->ActivateTimer(0, false);
			}

		ESChildSeat*	pSeatControl = GetSeatControl(nSeat);
		if( pSeatControl )
			pSeatControl->ActivateTimer(pSeat->GetAllSeconds(), true);
		}
	else{ // Stop
		ESChildSeat*	pSeatControl = GetSeatControl(nSeat);
		if( pSeatControl )
			pSeatControl->ActivateTimer(0, false);
		}
	return true;
	}

void
PokerTableWnd::ClearPotAmounts(bool bRedraw /*= true*/){
	int nSeat = 0, nCt = m_tableInfo.m_seats.GetCount(), nSumChips	= 0;
	while( nSeat < nCt ){
		PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
		if( !pSeat ){
			nSeat ++;
			continue;
			}
		int nAmountOld = pSeat->GetPotAmount();
		pSeat->SetTotalPot	(0);
		pSeat->SetPotAmount	(0);
		UpdateSeatChips		(nSeat, bRedraw);
		nSeat ++;
		}
	}

void
PokerTableWnd::ClearSeatCards(bool bRedraw){
	int nSeat = 0, nCt = m_tableInfo.m_seats.GetCount(), nSumChips	= 0;
	while( nSeat < nCt ){
		PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
		if( !pSeat ){
			nSeat ++;
			continue;
			}

		// Remove cards mark.
		ESChildControlSeatCards* pSeatCards	= GetSeatCardsControl(nSeat);
		if( pSeatCards )
			pSeatCards->SetCardsMark(false, false, false, false, false);
		SetSeatCards(nSeat, -1, -1, -1, -1, bRedraw);
		nSeat ++;
		}
	}

void
PokerTableWnd::ClearSeatHiddenCards(bool bRedraw){
	int nSeat = 0, nCt = m_tableInfo.m_seats.GetCount(), nSumChips	= 0;
	while( nSeat < nCt ){
		PokerTableSeatInfo* pSeat = m_tableInfo.m_seats.GetAt(nSeat);
		if( !pSeat ){
			nSeat ++;
			continue;
			}

		// Remove cards mark.
		ESChildControlSeatHiddenCards* pSeatHiddenCards	= GetSeatHiddenCardsControl(nSeat);
		if( pSeatHiddenCards ){
			pSeatHiddenCards->SetCards	(false, false, false, false, false);
			if( pSeatHiddenCards->IsVisible() )
				pSeatHiddenCards->SetVisible(false, bRedraw);

			PokerTableSeatInfo* pInfo = GetSeatInfo(nSeat);
			if( pInfo ){
				pInfo->SetCard1(-1);
				pInfo->SetCard2(-1);
				pInfo->SetCard3(-1);
				pInfo->SetCard4(-1);
				}
			}
		nSeat ++;
		}
	}

void
PokerTableWnd::ClearTableCards(bool bRedraw){
	SetTableCards(-1, -1, -1, -1, -1, bRedraw);
	}

bool
PokerTableWnd::IsSeatTimerActive(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) return false;
	return pSeat->IsTimerActive();
	}

bool
PokerTableWnd::SetActiveSeat(int nSeatIndex, bool bActivateTimer /*= false*/, int nAllSec /*= 0*/, int nSecPassed /*= 0*/, bool bInvalidate){
	int nActiveSeatOld		= m_tableInfo.m_cActiveSeat;
	m_tableInfo.m_cActiveSeat	= nSeatIndex;
	ESChildSeat* pSeatCtrl	= GetSeatControl(nSeatIndex);
	if( pSeatCtrl )
		pSeatCtrl->SetSeatActive(true, bInvalidate);

	if( nSeatIndex != nActiveSeatOld ){
		ESChildSeat* pSeatCtrlOld = GetSeatControl(nActiveSeatOld);
		if( pSeatCtrlOld )
			pSeatCtrlOld->SetSeatActive(false, bInvalidate);
		// Stop action timer on old seat.
		if( IsSeatTimerActive(nActiveSeatOld) )
			StartActionTimer(nActiveSeatOld, false, bInvalidate);
		}
	
	if( bActivateTimer )
		SetupSeatTimer(nSeatIndex, nAllSec, nSecPassed, bInvalidate, true);
	return true;
	}

bool
PokerTableWnd::AddActionButton(ActionButton type, std::string sLabel, int nAmountMin, int nAmountMax, bool bInvalidate /*= true*/){
	int	nActionButtonCt		= m_listActionButtons.GetCount();
	std::string	sName;
	sName.Format(_T("_actionButton%d"), nActionButtonCt + 1);
	ESChildControl*	pActionButton = GetChildControlByName(sName.GetBuffer());
	if( !pActionButton )
		return false;
	
	ActionButtonInfo* pNew	= new ActionButtonInfo();
	pNew->nAmountMin		= nAmountMin;
	pNew->nAmount			= min(nAmountMin, nAmountMax);
	pNew->nAmountMax		= nAmountMax;
	pNew->sLabel			= sLabel;
	pNew->type				= type;
	pNew->nButtonId			= pActionButton->GetId();
	m_listActionButtons.Add(pNew);

	((ESChildControlImageButton*)pActionButton)->SetText(sLabel);

	if( !pActionButton->IsVisible() )
		pActionButton->SetVisible(true, false);
	if( bInvalidate )
		pActionButton->Redraw();
	return true;
	}

bool
PokerTableWnd::IsActionButtonVisible(ActionButton type){
	int nLoop = 0, nCt = m_listActionButtons.GetCount();
	while( nLoop < nCt ){
		ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
		if( pInfo && pInfo->type == type ){
			ESChildControl*	pActionButton = GetChildControl(pInfo->nButtonId);
			if( pActionButton )
				return pActionButton->IsVisible();
			return false;
			}
		nLoop ++;
		}
	return false;
	}

ActionButtonInfo*	
PokerTableWnd::GetActionButtonInfo(ActionButton type){
	int nLoop	= 0, nCt = m_listActionButtons.GetCount();
	while( nLoop < nCt ){
		ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
		if( pInfo && pInfo->type == type )
			return pInfo;
		nLoop ++;
		}
	return NULL;
	}

bool
PokerTableWnd::ClearActionButtons(bool bInvalidate /*= true*/){
	int nLoop	= 0, nCt = m_listActionButtons.GetCount();
	while( nLoop < nCt ){
		ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
		if( !pInfo ){
			nLoop ++;
			continue;
			}
		ESChildControl*	pActionButton = GetChildControl(pInfo->nButtonId);
		if( pActionButton )
			pActionButton->SetVisible(false, bInvalidate);
		nLoop ++;
		}
	m_listActionButtons.RemoveAll();
	return true;
	}

bool
PokerTableWnd::RedrawActionButtons(){
	int nLoop	= 0, nCt = m_listActionButtons.GetCount();
	while( nLoop < nCt ){
		ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
		if( !pInfo ){
			nLoop ++;
			continue;
			}
		ESChildControl*	pActionButton = GetChildControl(pInfo->nButtonId);
		if( pActionButton )
			pActionButton->Redraw();
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::SetActionButtonInfo(int nIndex, std::string sLabel, int nAmount, bool bInvalidate){
	ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nIndex);
	if( !pInfo ) return false;

	pInfo->sLabel	= sLabel;
	pInfo->nAmount	= nAmount;
	
	ESChildControlImageButton*	pActionButton = (ESChildControlImageButton*)GetChildControl(pInfo->nButtonId);
	if( pActionButton ){
		pActionButton->SetText(sLabel);
		if( bInvalidate )
			pActionButton->Redraw();
		}
	return true;
	}

bool
PokerTableWnd::RedrawActionButton(int nIndex){
	ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nIndex);
	if( !pInfo ) return false;
	return RedrawChildControl(pInfo->nButtonId);
	}

bool
PokerTableWnd::ClearActionLabels(bool bRedraw){
	int nLoop	= 0, nCt = m_tableInfo.m_seats.GetCount();
	while( nLoop < nCt ){
		SetSeatActionLabel(nLoop, _T(""), bRedraw);
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::SetSeatActionLabel(int nSeat, std::string sActionName, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeatInfo = GetSeatInfo(nSeat);
	if( !pSeatInfo ) return false;
	pSeatInfo->SetActionName(sActionName);
	UpdateSeat(nSeat, bInvalidate, true);
	return true;
	}

bool
PokerTableWnd::SetSeatStatus(int nSeat, PokerSeatStatus status, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) return false;
	pSeat->SetStatus(status);
	if( bInvalidate )
		RedrawSeat(nSeat);
	return true;
	}

PokerSeatStatus	
PokerTableWnd::GetSeatStatus(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) return PokerSeatStatus::Empty;
	return pSeat->GetStatus();
	}

void
PokerTableWnd::SetSeatOffline(int nSeat, bool bOnline /*= false*/, bool bInvalidate /*= true*/){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) return;
	pSeat->SetOffline(!bOnline);
	UpdateSeatStatus(nSeat, bInvalidate);
	}

bool
PokerTableWnd::GetSeatOffline(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) return false;
	return pSeat->IsOffline();
	}

std::string
PokerTableWnd::GetSeatUserName(int nSeat){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat ) return _T("");
	return pSeat->GetUserName();
	}

void
PokerTableWnd::OnSeatClick(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	ESChildSeat*	pSeat = (ESChildSeat*)pSender;
	int				nSeat = pSeat->GetSeatIndex();
	if( nSeat < 0 || nSeat >= m_tableInfo.m_seats.GetCount() ) 
		return;
	if( m_pHandler )
		m_pHandler->OnSeatPanelClick(nSeat, GetSeatInfo(nSeat));
	}

void
PokerTableWnd::OnActionButtonClick(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	ActionButtonInfo* pInfo = NULL;
	for(int i=0; i<m_listActionButtons.GetCount(); i++){
		pInfo = m_listActionButtons.GetAt(i);
		if( pInfo && pInfo->nButtonId == pSender->GetId() )
			break;
		}
	
	if( !pInfo ) return;
	if( m_pHandler )
		m_pHandler->OnActionButtonClicked(pInfo);
	}

void
PokerTableWnd::OnPreActionButtonClick(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	int							nId			= pSender->GetId();
	ESChildPreActionControl*	pPreAction	= (ESChildPreActionControl*)pSender;
	bool						bCheckState = pPreAction->GetActive();
	PreActionButtonInfo*		pInfo		= GetPreActionInfo(nId);
	if( pInfo && m_pHandler )
		m_pHandler->OnPreActionButtonChecked(pInfo, bCheckState);
	}

void
PokerTableWnd::OnShortcutButtonClick(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	ShortcutButtonInfo* pInfo = NULL;
	for(int i=0; i<m_listShortcutButtons.GetCount(); i++){
		pInfo = m_listShortcutButtons.GetAt(i);
		if( pInfo && pInfo->nButtonId == pSender->GetId() )
			break;
		}
	if( !pInfo ) return;
	ESChildSlider* pSlider = (ESChildSlider*)GetChildControlByName(_T("_betSlider"));
	if( !pSlider ) return;

	int nAmount = pInfo->nAmount, nAmountMax = pSlider->GetSliderPosMax();
	if( nAmount == -1 ) // All-in shortcut button
		nAmount = nAmountMax;
		
	if( SetSliderAmount(nAmount, true) ){
		// Set raise/bet button info. {{
		std::string sLabel, sActionName;
		if( nAmount == pSlider->GetSliderPosMax() )
			sActionName = _T("{{All-in}}\r\n");
		else{
			if( m_tableInfo.m_round == PokerRoundType::PreFlop )
				sActionName = _T("{{Raise}}\r\n");
			else
				sActionName = _T("{{Bet}}\r\n");
			}
		sLabel.Format(_T("%s %s"),sActionName,  PokerHelper::GetFormattedAmount(0, nAmount));
		SetActionButtonInfo(2, sLabel, nAmount, true);
		// }}
		}
	}

void
PokerTableWnd::OnBetSliderPosChanged(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	int				nAmount		= nParam;
	ESChildTextBox* pBetAmount	= (ESChildTextBox*)GetChildControlByName(_T("_betAmount"));
	if( pBetAmount ){
		std::string sText = PokerHelper::GetFormattedAmount(0, nAmount);
		pBetAmount->SetText(sText, true, true);
		}

	// Set raise button info. {{
	std::string sLabel;
	if( m_tableInfo.m_round == PokerRoundType::PreFlop )
		sLabel.Format(_T("{{Raise}}\r\n %s"), PokerHelper::GetFormattedAmount(0, nAmount));
	else
		sLabel.Format(_T("{{Bet}}\r\n %s"), PokerHelper::GetFormattedAmount(0, nAmount));
	SetActionButtonInfo(2, sLabel, nAmount, true);
	// }}
	}

void
PokerTableWnd::OnBetAmountChanged(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	std::string			sText	= ((ESChildTextBox*)pSender)->GetText();
	double			dAmount = atof(sText.GetBuffer());
	int				nAmount = Serializable::ConvertMoney(dAmount);
	ESChildSlider*	pSlider = (ESChildSlider*)GetChildControlByName(_T("_betSlider"));
	if( !pSlider )	return;
	if( nAmount == pSlider->GetSliderPos() )
		return;

	int nMin = pSlider->GetSliderPosMin();
	int nMax = pSlider->GetSliderPosMax();
	if( nAmount > nMax )
		nAmount = nMax;
	if( nAmount < nMin )
		nAmount = nMin;

	// Set slider pos.
	pSlider->SetSliderPos(nAmount, true, false);
	// Set raise/bet button info. {{
	std::string sLabel;
	if( m_tableInfo.m_round == PokerRoundType::PreFlop )
		sLabel.Format(_T("{{Raise}}\r\n %s"), PokerHelper::GetFormattedAmount(0, nAmount));
	else
		sLabel.Format(_T("{{Bet}}\r\n %s"), PokerHelper::GetFormattedAmount(0, nAmount));
	SetActionButtonInfo(2, sLabel, nAmount, true);
	// }}
	}

void
PokerTableWnd::OnSeatCardsMouseEnter(ESChildControlSeatCards* pSender, ChildControlEventCode eventCode, int nParam){
	int					nSeatIndex	= pSender->GetSeatIndex();
	PokerTableSettings* pSettings	= PokerTableSettings::GetInstance();
	PokerTableSeatInfo* pInfo		= GetSeatInfo(nSeatIndex);
	if( !pInfo || pInfo->GetCard1() < 0 ) return;

	if( pInfo->GetStatus() != PokerSeatStatus::Folded && pInfo->GetStatus() != PokerSeatStatus::All_In && pSettings->m_bHideHoleCards ){
		ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(nSeatIndex);
		if( pSeatCards ){
			pSeatCards->HideCards	(false);
			pSeatCards->Redraw		();
			}
		}
	}

void
PokerTableWnd::OnSeatCardsMouseLeave(ESChildControlSeatCards* pSender, ChildControlEventCode eventCode, int nParam){
	int					nSeatIndex	= pSender->GetSeatIndex();
	PokerTableSettings* pSettings	= PokerTableSettings::GetInstance();
	PokerTableSeatInfo* pInfo		= GetSeatInfo(nSeatIndex);
	if( !pInfo || pInfo->GetCard1() < 0 ) return;

	if( pInfo->GetStatus() != PokerSeatStatus::Folded && pInfo->GetStatus() != PokerSeatStatus::All_In && pSettings->m_bHideHoleCards ){
		ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(nSeatIndex);
		if( pSeatCards ){
			pSeatCards->HideCards	(true);
			pSeatCards->Redraw		();
			}
		}
	}

void
PokerTableWnd::OnSeatMouseEnter(ESChildSeat* pSender, ChildControlEventCode eventCode, int nParam){
	int					nSeatIndex	= pSender->GetSeatIndex();
	PokerTableSettings* pSettings	= PokerTableSettings::GetInstance();
	PokerTableSeatInfo* pInfo		= GetSeatInfo(nSeatIndex);
	if( !pInfo || pInfo->GetCard1() < 0 ) return;

	if( pInfo->GetStatus() == PokerSeatStatus::Folded && pSettings->m_bDisplayFoldedCards ){
		ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(nSeatIndex);
		if( pSeatCards && !pSeatCards->IsVisible() ){
			pSeatCards->SetVisible(true, true);
			}
		}
	}

void
PokerTableWnd::OnSeatMouseLeave(ESChildSeat* pSender, ChildControlEventCode eventCode, int nParam){
	int					nSeatIndex	= pSender->GetSeatIndex();
	PokerTableSettings* pSettings	= PokerTableSettings::GetInstance();
	PokerTableSeatInfo* pInfo		= GetSeatInfo(nSeatIndex);
	if( !pInfo || pInfo->GetCard1() < 0 ) return;

	if( pInfo->GetStatus() == PokerSeatStatus::Folded && pSettings->m_bDisplayFoldedCards ){
		ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(nSeatIndex);
		if( pSeatCards && pSeatCards->IsVisible() ){
			pSeatCards->SetVisible(false, true);
			}
		}
	}

void
PokerTableWnd::OnSeatTimeBankActivated(ESChildSeat* pSender, ChildControlEventCode eventCode, int nParam){
	int nTimeBankSec = nParam;
	if( m_pHandler )
		m_pHandler->OnSeatTimeBankActivated(pSender->GetSeatIndex(), nTimeBankSec);
	}

bool
PokerTableWnd::AddShortcutButton(std::string sLabel, int nAmount, bool bInvalidate /*= true*/){
	std::string	 sControlName;
	sControlName.Format(_T("_shortcutButton%d"), m_listShortcutButtons.GetCount() + 1);
	ESChildControl*	pShortcutButton = GetChildControlByName(sControlName.GetBuffer());
	if( !pShortcutButton )
		return false;

	ShortcutButtonInfo* pInfo = new ShortcutButtonInfo();
	pInfo->sLabel		= sLabel;
	pInfo->nAmount		= nAmount;
	pInfo->nButtonId	= pShortcutButton->GetId();
	m_listShortcutButtons.Add(pInfo);

	// Set label.
	((ESChildControlImageButton*)pShortcutButton)->SetText(sLabel);
	if( pShortcutButton->IsVisible() )
		pShortcutButton->Redraw(true);
	else
		pShortcutButton->SetVisible(true, bInvalidate);
	return true;
	} 

bool
PokerTableWnd::AddDisabledShortcutButton(bool bInvalidate /*= true*/){
	std::string	 sControlName;
	sControlName.Format(_T("_shortcutButton%d"), m_listShortcutButtons.GetCount() + 1);
	ESChildControl*	pShortcutButton = GetChildControlByName(sControlName.GetBuffer());
	if( !pShortcutButton )
		return false;

	m_listShortcutButtons.Add(NULL);
	if( pShortcutButton->IsVisible() )
		pShortcutButton->SetVisible(false, bInvalidate);
	return true;
	}

void
PokerTableWnd::HideShortcutButtons(bool bShow /*= false*/, bool bRedraw /*= true*/){
	int nLoop = 0, nCt	= m_listShortcutButtons.GetCount();
	while( nLoop < nCt ){
		ShortcutButtonInfo* pInfo = m_listShortcutButtons.GetAt(nLoop);
		if( pInfo ){
			ESChildControl*	pShortcutButton = GetChildControl(pInfo->nButtonId);
			if( pShortcutButton )
				pShortcutButton->SetVisible(bShow, bRedraw);
			}
		nLoop ++;
		}
	}

bool
PokerTableWnd::ClearShortcutButtons(bool bInvalidate /*= true*/){
	int nLoop = 0, nCt = m_listShortcutButtons.GetCount();
	while( nLoop < nCt ){
		ShortcutButtonInfo* pInfo = m_listShortcutButtons.GetAt(nLoop);
		if( pInfo ){
			ESChildControl*	pShortcutButton = GetChildControl(pInfo->nButtonId);
			if( pShortcutButton ){
				if( pShortcutButton->IsVisible() )
					pShortcutButton->SetVisible(false, bInvalidate);
				}
			}
		nLoop ++;
		}
	m_listShortcutButtons.RemoveAll();
	return true;
	}

bool
PokerTableWnd::RedrawShortcutButtons(){
	int nLoop = 0, nCt = m_listShortcutButtons.GetCount();
	while( nLoop < nCt ){
		ShortcutButtonInfo* pInfo = m_listShortcutButtons.GetAt(nLoop);
		if( pInfo ){
			nLoop ++;
			continue;
			}
		ESChildControl*	pShortcutButton = GetChildControl(pInfo->nButtonId);
		if( pShortcutButton ){
			if( pShortcutButton->IsVisible() )
				pShortcutButton->Redraw(true);
			}
		nLoop ++;
		}
	return true;
	}

void
PokerTableWnd::HideActionButtons(bool bShow /*= false*/, bool bRedraw /*= true*/){
	int nCt	= m_listActionButtons.GetCount(), nLoop = 0;
	while( nLoop < nCt ){
		ActionButtonInfo* pInfo = m_listActionButtons.GetAt(nLoop);
		if( !pInfo ){
			nLoop ++;
			continue;
			}
		ESChildControl*	pActionButton = GetChildControl(pInfo->nButtonId);
		if( pActionButton )
			pActionButton->SetVisible(bShow, bRedraw);
		nLoop ++;
		}
	}

void
PokerTableWnd::HidePreActionButtons(bool bShow /*= false*/, bool bRedraw /*= true*/){
	int nCt		= m_listPreActions.GetCount(), nLoop = 0;
	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions.GetAt(nLoop);
		if( pInfo ){
			ESChildControl*	 pPreAction	= GetChildControl(pInfo->nButtonId);
			if( pPreAction )
				pPreAction->SetVisible(bShow, bRedraw);
			}
		nLoop ++;
		}
	}

bool
PokerTableWnd::SetSliderAmount(int nAmount, bool bInvalidate /*= true*/){
	ESChildSlider*	pSlider	= (ESChildSlider*)GetChildControlByName(_T("_betSlider"));
	if( !pSlider ) return false;
	bool bRet = pSlider->SetSliderPos(nAmount, bInvalidate);
	if( bRet ){
		ESChildTextBox* pBetAmount = (ESChildTextBox*)GetChildControlByName(_T("_betAmount"));
		if( pBetAmount )
			pBetAmount->SetText(PokerHelper::GetFormattedAmount(0, nAmount), false, bInvalidate);
		}
	return bRet;
	}

bool
PokerTableWnd::ShowSlider(int nAmount, int nAmountMin, int nAmountMax, int nAmountStep, bool bJumpToClickedPos, bool bInvalidate /*= true*/){
	ESChildSlider*		pSlider			= (ESChildSlider*)GetChildControlByName(_T("_betSlider"));
	ESChildTextBox*		pBetAmount		= (ESChildTextBox*)GetChildControlByName(_T("_betAmount"));

	if( pSlider ){
		if( !pSlider->IsVisible() )
			pSlider->SetVisible(true, false);
		pSlider->AllowJumpToClickedPos	(bJumpToClickedPos);
		pSlider->SetupSlider			(nAmountMin, nAmountMax, nAmountStep, false);
		pSlider->SetSliderPos			(nAmount, false);
		if( bInvalidate )
			pSlider->Redraw();
		}

	if( pBetAmount ){
		if( !pBetAmount->IsVisible() )
			pBetAmount->SetVisible(true, false);
//		pBetAmount->Setup		(nAmount, nAmountMin, nAmountMax);
		SetChildFocus(pBetAmount, false);
		pBetAmount->SetText	(PokerHelper::GetFormattedAmount(0, nAmount), true, false);

		if( bInvalidate )
			pBetAmount->Redraw();
		}
	return true;
	}

bool
PokerTableWnd::HideSlider(bool bInvalidate /*= true*/){
	SetControlVisibilityByName(_T("_betSlider"), false, bInvalidate);
	SetControlVisibilityByName(_T("_betAmount"), false, bInvalidate);
	return true;
	}

bool
PokerTableWnd::IsActionSliderVisible(){
	ESChildControl*		pSlider	= GetChildControlByName(_T("_betSlider"));
	if( !pSlider ) return false;
	return pSlider->IsVisible();
	}

bool
PokerTableWnd::AddPreActionButton(PreActionButton preAction, int nValue, std::string sLabel, bool bCheckState, bool bInvalidate, bool bCalcSize /*= false*/){
	ESChildPreActionControl*	pPreAction	= GetPreActionControl(preAction);
	if( !pPreAction ) return false;

	PreActionButtonInfo* pInfo	= new PreActionButtonInfo;
	pInfo->type					= preAction;
	pInfo->nButtonId			= pPreAction->GetId();
	pInfo->nValue				= nValue;	
	m_listPreActions.Add(pInfo);

	pPreAction->SetActive	(bCheckState, false);
	pPreAction->SetLabel	(sLabel, false);
	pPreAction->SetVisible	(true, bInvalidate);
	return true;
	}

bool
PokerTableWnd::RemovePreActionButton(PreActionButton preAction, bool bInvalidate){
	ESChildPreActionControl*	pPreAction	= GetPreActionControl(preAction);
	if( !pPreAction ) return false;

	int nLoop = 0, nCt = m_listPreActions.GetCount(), nId = pPreAction->GetId();
	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo && pInfo->nButtonId == nId ){
			m_listPreActions.RemoveAt(nLoop);
			break;
			}
		nLoop ++;
		}
	pPreAction->SetVisible(false, bInvalidate);
	return true;
	}

bool
PokerTableWnd::RemoveAllGamePreActions(bool bInvalidate /*= true*/){
	RemovePreActionButton(PreActionButton::PreAction_Call, bInvalidate);
	RemovePreActionButton(PreActionButton::PreAction_CallAny, bInvalidate);
	RemovePreActionButton(PreActionButton::PreAction_CallAnyCheck, bInvalidate);
	RemovePreActionButton(PreActionButton::PreAction_Fold, bInvalidate);
	RemovePreActionButton(PreActionButton::PreAction_Check, bInvalidate);
	RemovePreActionButton(PreActionButton::PreAction_CheckFold, bInvalidate);
	RemovePreActionButton(PreActionButton::PreAction_WaitForBB_PostBB, bInvalidate);
	return true;
	}

bool
PokerTableWnd::SetPreActionButtonLabel(PreActionButton preAction, std::string sLabel, bool bInvalidate /*= true*/){
	ESChildPreActionControl*	pPreAction	= GetPreActionControl(preAction);
	if( !pPreAction ) return false;
	pPreAction->SetLabel(sLabel, bInvalidate);
	return true;
	}

bool
PokerTableWnd::SetPreActionCheckState(PreActionButton preAction, bool bCheckState, bool bInvalidate /*= true*/){
	ESChildPreActionControl*	pPreAction	= GetPreActionControl(preAction);
	if( !pPreAction ) return false;
	pPreAction->SetActive(bCheckState, bInvalidate);
	return true;
	}

void
PokerTableWnd::UncheckPreActions(PreActionButton preActionExcept, bool bInvalidate){
	int nLoop = 0, nCt = m_listPreActions.GetCount();
	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo && pInfo->type != preActionExcept ){
			ESChildPreActionControl*	pPreAction	= (ESChildPreActionControl*)GetChildControl(pInfo->nButtonId);
			ASSERT(pPreAction);
			if( pPreAction->GetActive() )
				pPreAction->SetActive(false, bInvalidate);
			}
		nLoop ++;
		}
	}

bool
PokerTableWnd::GetPreActionCheckState(PreActionButton preAction){
	ESChildPreActionControl*	pPreAction	= GetPreActionControl(preAction);
	if( !pPreAction ) return false;
	return pPreAction->GetActive();
	}

bool
PokerTableWnd::ClearPreActions(bool bInvalidate /*= true*/){
	int nLoop = 0, nCt = m_listPreActions.GetCount();
	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo ){
			ESChildPreActionControl*	pPreAction	= (ESChildPreActionControl*)GetChildControl(pInfo->nButtonId);
			if( pPreAction )
				pPreAction->SetVisible(false, bInvalidate);
			}
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::RedrawPreActions(){
	int nLoop = 0, nCt = m_listPreActions.GetCount();
	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo ){
			ESChildControlPreAction* pPreAction	= (ESChildControlPreAction*)GetChildControl(pInfo->nButtonId);
			if( pPreAction )
				pPreAction->Redraw();
			}
		nLoop ++;
		}
	return true;
	}

bool
PokerTableWnd::RedrawPreActionButton(PreActionButton preAction){
	ESChildPreActionControl*	pPreAction	= GetPreActionControl(preAction);
	if( !pPreAction ) 
		return false;
	pPreAction->Redraw();
	return true;
	}

PreActionButtonInfo*
PokerTableWnd::GetPreActionInfo(int nId){
	int nLoop	= 0, nCt = m_listPreActions.GetCount();
	while( nLoop < nCt ){
		PreActionButtonInfo* pInfo = m_listPreActions[nLoop];
		if( pInfo && pInfo->nButtonId == nId )
			return pInfo;
		nLoop ++;
		}
	return NULL;
	}

ESChildPreActionControl*
PokerTableWnd::GetPreActionControl(PreActionButton preAction){
	int nIndex	= m_arrPreActionIdByPreAction.IndexOf((void*)preAction);
	if( nIndex == -1 ) 
		return NULL; // Control have not been found !
	int							nId			= (int)m_arrPreActionIdByPreAction.GetData(nIndex);
	return (ESChildPreActionControl*)GetChildControl(nId);
	}

ESChildSeat*
PokerTableWnd::GetSeatControl(int nSeat){
	std::string	sName;
	sName.Format(_T("_pokerSeat%d"), nSeat + 1);
	ESChildSeat*	pSeat = (ESChildSeat*)GetChildControlByName(sName.GetBuffer());
	return pSeat;
	}

ESChildSeatStatus*
PokerTableWnd::GetSeatStatusControl(int nSeat){
	std::string	sName;
	sName.Format(_T("_pokerSeatStatus%d"), nSeat + 1);
	ESChildSeatStatus*	pSeatStatus = (ESChildSeatStatus*)GetChildControlByName(sName.GetBuffer());
	return pSeatStatus;
	}

ESChildControlSeatCards*
PokerTableWnd::GetSeatCardsControl(int nSeat){
	std::string	sName;
	sName.Format(_T("_seatCards%d"), nSeat + 1);
	ESChildControlSeatCards*	pSeatCards = (ESChildControlSeatCards*)GetChildControlByName(sName.GetBuffer());
	return pSeatCards;
	}

ESChildControlSeatHiddenCards*
PokerTableWnd::GetSeatHiddenCardsControl(int nSeat){
	std::string	sName;
	sName.Format(_T("_seatHiddenCards%d"), nSeat + 1);
	ESChildControlSeatHiddenCards*	pSeatHiddenCards = (ESChildControlSeatHiddenCards*)GetChildControlByName(sName.GetBuffer());
	return pSeatHiddenCards;
	}

CRect
PokerTableWnd::GetSeatCardsRect(int nSeat){
	ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(nSeat);
	if( !pSeatCards ) return CRect(0, 0, 0, 0);
	return pSeatCards->GetClientRect();
	}

CRect
PokerTableWnd::GetSeatRect(int nSeat){
	ESChildSeat* pSeat = GetSeatControl(nSeat);
	if( !pSeat ) return CRect(0, 0, 0, 0);
	return pSeat->GetClientRect();
	}

ESChildControlChips*
PokerTableWnd::GetChipsControl(int nSeat){
	std::string	sName;
	sName.Format(_T("_pokerChip%d"), nSeat + 1);
	ESChildControlChips*	pSeatChips = (ESChildControlChips*)GetChildControlByName(sName.GetBuffer());
	return pSeatChips;
	}

ESChildControlChips*
PokerTableWnd::GetMainChipsControl(){
	ESChildControlChips*	pMainChips	= (ESChildControlChips*)GetChildControlByName(_T("_pokerMainChips"));
	return pMainChips;
	}

ESChildControlChips*
PokerTableWnd::GetSidePotChipsControl(int nSidePotIndex){
	std::string sName;
	sName.Format(_T("_sidePot%d"), nSidePotIndex + 1);
	return (ESChildControlChips*)GetChildControlByName(sName.GetBuffer());
	}

ESChildControlImage*
PokerTableWnd::GetSeatDealerControl(int nSeat){
	std::string	sName;
	sName.Format(_T("_dealer%d"), nSeat + 1);
	ESChildControlImage*	pDealer = (ESChildControlImage*)GetChildControlByName(sName.GetBuffer());
	return pDealer;
	}

ESChildControlTableCards*
PokerTableWnd::GetTableCardsControl(){
	ESChildControlTableCards*	pTableCards	= (ESChildControlTableCards*)GetChildControlByName(_T("_tableCards"));
	return pTableCards;
	}

ESChildControlSeatHiddenCards*
PokerTableWnd::GetDeckControl(){
	ESChildControlSeatHiddenCards*	pDeck	= (ESChildControlSeatHiddenCards*)GetChildControlByName(_T("_deck"));
	return pDeck;
	}

ESSimpleRenderView*
PokerTableWnd::GetChatViewControl(){
	ESSimpleRenderView* pRenderView = (ESSimpleRenderView*)GetChildControlByName(_T("_renderviewChat"));
	return pRenderView;
	}

ESChildControlTable*
PokerTableWnd::GetTableControl(){
	return (ESChildControlTable*)GetChildControlByName(_T("_pokerTable1"));
	}

bool
PokerTableWnd::SetLogicalPositionAndSize(ESChildControl* pControl, CSize szOwner){
	if( !pControl ) return false;
	CRect rcControl			= pControl->GetClientRect();
	float fPosX				= (rcControl.left / ((float)szOwner.cx));
	float fPosY				= (rcControl.top / ((float)szOwner.cy));
	float fCX				= rcControl.Width() / ((float)szOwner.cx);
	float fCY				= rcControl.Height() / ((float)szOwner.cy);

	CRect rcClient;
	GetClientRectMy(rcClient);
	pControl->SetSizeAutoCalculation(true);
	pControl->SetLogicalPos			(fPosX, fPosY);
	pControl->SetLogicalSize		(fCX, fCY);
	pControl->OwnerWindowSizeChanged(rcClient.Width(), rcClient.Height());
	return true;
	}

bool
PokerTableWnd::SetLogicalPositionAndSizeByName(std::string sName, CSize szOwner){
	return SetLogicalPositionAndSize(GetChildControlByName(sName.GetBuffer()), szOwner);
	}

bool
PokerTableWnd::RedrawSeat(int nSeat, bool bWithCards /*= false*/){
	ESChildSeat*				pSeatCtrl	= GetSeatControl(nSeat);
	if( !pSeatCtrl ) return false;
	CRect						rcSeat		= pSeatCtrl->GetClientRect();
	ESChildControlSeatCards*	pSeatCards	=  GetSeatCardsControl(nSeat);
	if( pSeatCards && bWithCards /*&& pSeatCards->IsVisible()*/ )
		rcSeat |= pSeatCards->GetClientRect();
	ESChildSeatStatus*			pSeatStatus	=  GetSeatStatusControl(nSeat);
	if( pSeatStatus && pSeatStatus->IsVisible() )
		rcSeat |= pSeatStatus->GetClientRect();
	RedrawRect(rcSeat);
	return true;
	}

void
PokerTableWnd::RedrawSeats(bool bWithCards /*= false*/){
	for( int i=0; i<m_tableInfo.m_seats.GetCount(); i++)
		RedrawSeat(i, bWithCards);
	}

bool
PokerTableWnd::RedrawSeatCards(int nSeat){
	ESChildControlSeatCards* pSeatCards	= (ESChildControlSeatCards*)GetSeatCardsControl(nSeat);
	if( !pSeatCards ) return false;
	pSeatCards->Redraw();
	return true;
	}

bool
PokerTableWnd::RedrawSeatChips(int nSeat){
	ESChildControlChips* pChipsCtrl = GetChipsControl(nSeat);
	if( !pChipsCtrl ) return false;
	pChipsCtrl->Redraw();
	return true;
	}

void
PokerTableWnd::ClearChatMessages(bool bRedraw){
	ESSimpleRenderView* pChat = GetChatViewControl();
	if( !pChat ) return;
	pChat->ClearContent(bRedraw);
	}

void
PokerTableWnd::AddChatMessage(PokerChatMessage* pMessage){
	ESSimpleRenderView* pChat = GetChatViewControl();
	if( !pChat ) return;

	std::string sText;
	if( pMessage->m_bSystem ){
		sText = pMessage->m_sMessage;
		pChat->AddText(_T("LABEL2"), _T("Admin: "), false, RGB(204, 51, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
		pChat->AddText(_T("MSG1"), sText, true, RGB(204, 51, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
		}
	else{
		if( ((PokerActionTypes)pMessage->m_actionType) == PokerActionTypes::NoAction  ){
			sText = pMessage->m_sUserName + _T(": ");
			pChat->AddText(_T("LABEL1"), sText, false, RGB(0, 51, 153), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
			sText = pMessage->m_sMessage;
			pChat->AddText(_T("MSG1"), sText, true, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
			}
		else{

			}
		}

	/*
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !pMessage )
		return;

	std::string sFormat;
	COleDateTime dtAction = pMessage->m_dtTime;//COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	std::string	sUserName	= pMessage->m_sUserName;

	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	if( pMessage->m_bSystem ){
		pChatPage->AddTextToLine(nLine, _T("UserName"), _T("SYSTEM: "), m_pGUI->m_crChatUserName, false);
		pChatPage->AddTextToLine(nLine, _T("System"), pMessage->m_sMessage, m_pGUI->m_crChatSystem, false);
		}
	else{
		if( ((PokerActionTypes)pMessage->m_actionType) != PokerActionTypes::NoAction ){
			pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);
			PokerActionTypes	acType		= (PokerActionTypes)pMessage->m_actionType;
			switch( acType){
				case PokerActionTypes::AllIn:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_ALLIN_FORMAT")), sUserName);
					//sFormat.Format(_T("%s acts AllIn"), sUserName);
					break;
					}
				case PokerActionTypes::Bet:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_BET_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s acts Bet to %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				case PokerActionTypes::Call:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_CALL_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s Calls %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				case PokerActionTypes::Check:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_CHECK_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Checks"), sUserName);
					break;
					}
				case PokerActionTypes::Fold:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_FOLD_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Folds"), sUserName);
					break;
					}
				case PokerActionTypes::Muck:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_MUCK_CARDS_FORMAT")), sUserName);
					//sFormat.Format(_T("%s muck cards"), sUserName);
					break;
					}
				case PokerActionTypes::PostBB:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_POST_BB_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Posts Big Blind"), sUserName);
					break;
					}
				case PokerActionTypes::PostSB:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_POST_SB_FORMAT")), sUserName);
					//sFormat.Format(_T("%s Posts Small Blind"), sUserName);
					break;
					}
				case PokerActionTypes::Raise:
					{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_RAISE_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s acts Raise to %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				case PokerActionTypes::ShowCards:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SHOW_CARDS_FORMAT")), sUserName);
					//sFormat.Format(_T("%s show cards"), sUserName);
					break;
					}
				case PokerActionTypes::SitOut:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SITOUT_FORMAT")), sUserName);
					//sFormat.Format(_T("%s sitted out"), sUserName);
					break;
					}
				case PokerActionTypes::ReturnBack:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_RETURN_BACK_FORMAT")), sUserName);
					//sFormat.Format(_T("%s returned back"), sUserName);
					break;
					}
				case PokerActionTypes::AddChips:{
					sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_ADD_CHIPS_FORMAT")), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					//sFormat.Format(_T("%s added chips %d.%02d %s"), sUserName, pMessage->m_nAmount/100, pMessage->m_nAmount%100, m_sAmountLabelSuffix);
					break;
					}
				};
			pChatPage->AddTextToLine(nLine, _T("Message"), sFormat, m_pGUI->m_crChatMessage, false);
			}
		else{
			sFormat = sUserName + _T(": ");
			pChatPage->AddTextToLine(nLine, _T("UserName"), sFormat, m_pGUI->m_crChatUserName, false);
			pChatPage->AddTextToLine(nLine, _T("Message"), pMessage->m_sMessage, m_pGUI->m_crChatMessage, false);
			}
		}

	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	*/
	}

void
PokerTableWnd::AddChatMessage_HandStart(PokerHandStartedPacket* p){
	ESSimpleRenderView* pChat = GetChatViewControl();
	if( !pChat ) return;

	std::string sText;
	sText.Format(_T("Hand #%d Started"), p->m_nHandId);
	pChat->AddText(_T("LABEL1"), _T("Dealer: "), false, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
	pChat->AddText(_T("MSG1"), sText, true, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);

	/*
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;

	std::string sFormat;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);

	std::string sMessage;
	sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_HAND_STARTED_FORMAT")), p->m_nHandId);
	//sMessage.Format(_T("********* Hand Started %d *******"), p->m_nHandId);
	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	sMessage.Format(_T("Dealer: %d"), (int)p->m_cDealer);
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	sMessage.Format(_T("Small Blind: %d"), (int)p->m_cSmallBlind);
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	sMessage.Format(_T("Big Blind: %d"), (int)p->m_cBigBlind);
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	*/
	}

void
PokerTableWnd::AddChatMessage_HandFinish(PokerHandFinishedPacket* p){
	ESSimpleRenderView* pChat = GetChatViewControl();
	if( !pChat ) return;

	std::string sText, sFormat;
	sText.Format(_T("Hand #%d Finished\r\n"), p->m_nHandId);

	int	nLoop = 0, nCt = p->m_winners.GetCount();
	while( nLoop < nCt ){
		PokerHandWinnerInfo* pWinner = p->m_winners.GetAt(nLoop);
		if( !pWinner ){
			nLoop ++;
			continue;
			}
		if( pWinner->m_bWinner )
			sFormat.Format(_T("\t\t%s won (%d.%02d %s) from pot\r\n"), GetSeatUserName((int)pWinner->m_cSeat), pWinner->m_nWonAmount/100, pWinner->m_nWonAmount%100, m_sAmountLabelSuffix);
		else
			sFormat.Format(_T("\t\t%s returned (%d.%02d %s)\r\n"), GetSeatUserName((int)pWinner->m_cSeat), pWinner->m_nReturnBackAmount/100, pWinner->m_nReturnBackAmount%100, m_sAmountLabelSuffix);
		sText += sFormat;
		nLoop ++;
		}
	pChat->AddText(_T("LABEL1"), _T("Dealer: "), false, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
	pChat->AddText(_T("MSG1"), sText, true, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);

	
	/*
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;
	
	std::string sFormat, sMessage;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);

	sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_HAND_FINISHED_FORMAT")), p->m_nHandId);
	//sMessage.Format(_T("************ Hand Finished %d ***********"), p->m_nHandId);
	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);

	int		nLoop = 0;
	int		nCt		= p->m_winners.GetCount();
	while( nLoop < nCt ){
		PokerHandWinnerInfo* pWinner = p->m_winners.GetAt(nLoop);
		if( !pWinner || !pWinner->m_bWinner ){
			nLoop ++;
			continue;
			}

		sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_WON_FORMAT")), GetSeatUserName((int)pWinner->m_cSeat), pWinner->m_nWonAmount/100, pWinner->m_nWonAmount%100, m_sAmountLabelSuffix);
		//sMessage.Format(_T("%s won %d.%02d %s"), GetSeatUserName((int)pWinner->m_cSeat), pWinner->m_nWonAmount/100, pWinner->m_nWonAmount%100, m_sAmountLabelSuffix);
		pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
		nLoop ++;
		}
	sMessage = _T("***********************");
	pChatPage->AddTextToLine(-1, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);
	*/
	}

void
PokerTableWnd::AddChatMessage_RoundStart(PokerRoundStartedPacket* p){
	/*
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;
	
	std::string sFormat, sMessage;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);


	PokerRoundType round = (PokerRoundType)p->m_cRoundType;
	switch( round ){
		case PokerRoundType::Flop:
			{	
				sMessage.Format(_T("%s: ^%s ^%s ^%s"), PokerGUI::GetLangValue(_T("CHAT_FLOP_DEALT")), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3));

//				sMessage.Format(_T("Flop dealt: ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
//					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3));
				break;
			}
		case PokerRoundType::Turn:
			{	
				sMessage.Format(_T("%s: ^%s ^%s ^%s ^%s"), PokerGUI::GetLangValue(_T("CHAT_TURN_DEALT")), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard4));

//				sMessage.Format(_T("Turn dealt: ^%s ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
//					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
//					PokerGUI::CardNameByIndex((int)p->m_btTableCard4));
				break;
			}
		case PokerRoundType::River:
			{	
				sMessage.Format(_T("%s: ^%s ^%s ^%s ^%s ^%s"), PokerGUI::GetLangValue(_T("CHAT_RIVER_DEALT")), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
					PokerGUI::CardNameByIndex((int)p->m_btTableCard4), PokerGUI::CardNameByIndex((int)p->m_btTableCard5));
				
//				sMessage.Format(_T("River dealt: ^%s ^%s ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)p->m_btTableCard1),
//					PokerGUI::CardNameByIndex((int)p->m_btTableCard2), PokerGUI::CardNameByIndex((int)p->m_btTableCard3),
//					PokerGUI::CardNameByIndex((int)p->m_btTableCard4), PokerGUI::CardNameByIndex((int)p->m_btTableCard5));
				break;
			}
		}

	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);*/
	}

void
PokerTableWnd::AddChatMessage_ShowCards(PokerActionPacket* p){
	/*
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;

	std::string sFormat;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	std::string	sUserName	= GetSeatUserName((int)p->m_cSeat);

	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);
	sFormat.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SHOW_CARDS_FORMAT")), sUserName);

	CDWordArray arrCards;
	for(int i=0; i<p->m_listCardsShown.GetCount(); i++){
		PokerHandCards* pCards = p->m_listCardsShown.GetAt(i);
		if( pCards ){
			arrCards.Add((DWORD)pCards->m_btCard1);
			arrCards.Add((DWORD)pCards->m_btCard2);

			if( pCards->m_btCard3 != (char)-1 )
				arrCards.Add((DWORD)pCards->m_btCard3);

			if( pCards->m_btCard4 != (char)-1 )
				arrCards.Add((DWORD)pCards->m_btCard4);
			break;
			}
		}

	if( arrCards.GetCount() == 2 ){
		std::string sMessage;
		sMessage.Format(_T(": ^%s ^%s"), PokerGUI::CardNameByIndex((int)arrCards[0]), PokerGUI::CardNameByIndex((int)arrCards[1]));
		sFormat += sMessage;
		}
	else
	if( arrCards.GetCount() == 4 ){
		std::string sMessage;
		sMessage.Format(_T(": ^%s ^%s ^%s ^%s"), PokerGUI::CardNameByIndex((int)arrCards[0]), PokerGUI::CardNameByIndex((int)arrCards[1]),
						PokerGUI::CardNameByIndex((int)arrCards[2]), PokerGUI::CardNameByIndex((int)arrCards[3]));
		sFormat += sMessage;
		}

	pChatPage->AddTextToLine(nLine, _T("Message"), sFormat, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);*/
	}

void
PokerTableWnd::AddChatMessage_TimeToAct(int nActTimeSecLeft){
	ESSimpleRenderView* pChat = GetChatViewControl();
	if( !pChat ) return;
	std::string sText;
	sText.Format(_T("You have %d sec to act"), nActTimeSecLeft);
	pChat->AddText(_T("LABEL1"), _T("Dealer: "), false, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, false);
	pChat->AddText(_T("MSG1"), sText, true, RGB(0, 0, 0), CSize(0, 0), 0.0f, 0.0f, VAlign_Middle, HAlignement_Left, true);
	}

void
PokerTableWnd::AddChatMessage_SetSeatStatus(PokerSeatStatusChangedPacket* p){
	/*
	ESChildControlTabViewChatPage* pChatPage = (ESChildControlTabViewChatPage*)GetChildControl(CHILD_TableChatPage);
	if( !pChatPage || !p )
		return;
	
	std::string sFormat, sMessage;
	COleDateTime dtAction = COleDateTime::GetCurrentTime();
	sFormat.Format(_T("{%02d:%02d:%02d} "), dtAction.GetHour(), dtAction.GetMinute(), dtAction.GetSecond());
	int nLine = pChatPage->AddTextToLine(-1, _T("Time"), sFormat, m_pGUI->m_crChatTime, false);
	pChatPage->AddTextToLine(nLine, _T("Dealer"), _T("D: "), m_pGUI->m_crChatDealer, false);

	PokerSeatStatus status = (PokerSeatStatus)p->m_status;
	switch( status ){
		case PokerSeatStatus::Sitted:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SIT_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s seated at table"), p->m_sUserName);
			break;
			}
		case PokerSeatStatus::SittedOut:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_SITOUT_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s seated out"), p->m_sUserName);
			break;
			}
		case PokerSeatStatus::Reserved:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_JOIN_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s joined the table"), p->m_sUserName);
			break;
			}
		case PokerSeatStatus::Empty:{	
			sMessage.Format(PokerGUI::GetLangValue(_T("CHAT_SOMEONE_ACT_LEAVE_FORMAT")), p->m_sUserName);
			//sMessage.Format(_T("%s left the table"), p->m_sUserName);
			break;
			}
		}

	pChatPage->AddTextToLine(nLine, _T("Message"), sMessage, m_pGUI->m_crChatMessage, false);
	pChatPage->GotoLine(pChatPage->GetLineCount() - 1, true);*/
	}

int
PokerTableWnd::AddChatMessages(EnumerableObject<PokerChatMessage>* pListMessage){
	/*
	if( !pListMessage || !pListMessage->GetCount() )
		return 0;

	ClearChatMessages(false);

	int nLoop	= 0;
	int nCt		= pListMessage->GetCount();

	while( nLoop < nCt ){
		PokerChatMessage* pMsg = pListMessage->GetAt(nLoop);
		AddChatMessage(pMsg);
		nLoop ++;
		}
	return nLoop;*/
	return 0;
	}

void
PokerTableWnd::SetTableMessage(std::string sMessage, bool bInvalidate){
	ESChildControlLabel* pMessageLabel = (ESChildControlLabel*)GetChildControlByName(_T("_labelTableMessage"));
	if( !pMessageLabel ) return;
	pMessageLabel->SetVisible(true, false);
	pMessageLabel->SetLabel(sMessage, bInvalidate);
	}

void
PokerTableWnd::ClearTableMessage(bool bInvalidate){
	ESChildControlLabel* pMessageLabel = (ESChildControlLabel*)GetChildControlByName(_T("_labelTableMessage"));
	if( pMessageLabel && pMessageLabel->IsVisible() ){
		pMessageLabel->SetVisible(false, bInvalidate);
		}
	}

void
PokerTableWnd::SetHandCombinationLabel(std::string sLabel, bool bInvalidate){
	ESChildControlLabel* pCombLabel = (ESChildControlLabel*)GetChildControlByName(_T("_labelCardsCombination"));
	if( !pCombLabel ) return;
	pCombLabel->SetVisible	(true, false);
	pCombLabel->SetLabel	(sLabel, bInvalidate);
	}

void
PokerTableWnd::ClearHandCombinationLabel(bool bInvalidate){
	ESChildControlLabel* pCombLabel = (ESChildControlLabel*)GetChildControlByName(_T("_labelCardsCombination"));
	if( pCombLabel && pCombLabel->IsVisible() )
		pCombLabel->SetVisible	(false, bInvalidate);
	}

void
PokerTableWnd::SetSeatMessage(int nSeat, std::string sMessage, bool bInvalidate){
	/*
	if( nSeat < 0 || nSeat >= m_tableInfo.m_seats.GetCount() )
		return;

	int						nId				= CHILD_Seat_Message + nSeat;
	ESChildControlSeat*		pSeatCtrl		= GetSeatControl(nSeat);
	if( !pSeatCtrl ) return;
	ESChildControlLabel*	pMessageLabel	= (ESChildControlLabel*)GetChildControl(nId);
	CRect					rcSeat			= pSeatCtrl->GetClientRect();

	if( !pMessageLabel ){
		pMessageLabel = new ESChildControlLabel();
		// Seat message label.
		CRect rcSeatMessage;
		rcSeatMessage.left		= rcSeat.left;
		rcSeatMessage.top		= rcSeat.top - 14;
		rcSeatMessage.right		= rcSeat.right;
		rcSeatMessage.bottom	= rcSeatMessage.top + 28;

		CRect rcClient;
		GetClientRect(rcClient);

		if( AddChildControlEx(pMessageLabel, nId, rcSeatMessage, NULL, CSize(rcClient.Width(), rcClient.Height())) ){
//			pMessageLabel->SetZIndex	(2);
			pMessageLabel->SetTextColor	(m_pGUI->m_crTMessageText);
			pMessageLabel->SetFont		(m_pGUI->m_pTMessageFont);
			pMessageLabel->SetBkImage	(m_pGUI->m_pTableMessageBk, m_pGUI->m_rcTableMessageOmit);
			pMessageLabel->SetCentred	(false);
			pMessageLabel->SetVisible	(true, false);
			}
		else{
			delete pMessageLabel;
			pMessageLabel = NULL;
			return;
			}
		}
	if( !pMessageLabel->IsVisible() ){
		pMessageLabel->SetVisible(true, false);
		}
	pMessageLabel->SetLabel(sMessage, bInvalidate);*/
	}

void
PokerTableWnd::ClearSeatMessage(int nSeat, bool bInvalidate){
	/*
	int						nId				= CHILD_Seat_Message + nSeat;
	ESChildControlLabel*	pMessageLabel	= (ESChildControlLabel*)GetChildControl(nId);
	if( pMessageLabel ){
		if( bInvalidate && pMessageLabel->IsVisible() ){
			pMessageLabel->SetVisible(false, true);
			}
		RemoveChildControl(pMessageLabel, false);
		}*/
	}

void
PokerTableWnd::ClearSeatMessages(bool bInvalidate){
	/*
	int nLoop	= 0;
	int nCt		= m_tableInfo.m_seats.GetCount();

	while( nLoop < nCt ){
		ClearSeatMessage(nLoop, bInvalidate);
		nLoop ++;
		}*/
	}

bool
PokerTableWnd::UpdateHandIdLabels(__int64 nHandId, __int64 nPrevHandId, bool bInvalidate){
	ESChildControlLabel * pHandIdLabel		= (ESChildControlLabel*)GetChildControlByName(_T("_labelHandInfo"));
	ESChildControlLabel * pPrevHandIdLabel	= (ESChildControlLabel*)GetChildControlByName(_T("_labelPreviousHand"));
	bool bRet = false;
	std::string sFormat;
	if( pHandIdLabel ){
		bRet = true;
		if( nHandId <= 0 )
			pHandIdLabel->SetVisible(false, bInvalidate);
		else{
			pHandIdLabel->SetVisible(true, false);
			sFormat.Format(_T("{{Hand}} #%I64d"), nHandId);
			pHandIdLabel->SetLabel(sFormat, bInvalidate);
			}
		}

	if( pPrevHandIdLabel ){
		bRet = true;
		if( nPrevHandId <= 0 )
			pPrevHandIdLabel->SetVisible(false, bInvalidate);
		else{
			pPrevHandIdLabel->SetVisible(true, false);
			sFormat.Format(_T("{{Previous Hand}} #%d"), nPrevHandId);
			pPrevHandIdLabel->SetLabel(sFormat, bInvalidate);
			}
		}
	return bRet;
	}

bool
PokerTableWnd::SetBestHandCards(int nSeat, PokerHandCards* pCards, bool bSetAsSeatMessage /*= true*/, std::string* psLabelText /*= NULL*/, CDWordArray* pArrWinnerCards /*= NULL*/){
	PokerTableSeatInfo* pSeat = GetSeatInfo(nSeat);
	if( !pSeat || pSeat->GetCard1() < 0 || pSeat->GetCard2() < 0 || !pCards )
		return false;

	if( m_tableInfo.m_cCard1 < 0 || m_tableInfo.m_cCard2 < 0 || m_tableInfo.m_cCard3 < 0 )
		return false; // We don't have table cards.

	int nCards[] = {(int)pCards->m_btCard1, (int)pCards->m_btCard2, (int)pCards->m_btCard3, 
					(int)pCards->m_btCard4, GetTableCard1(), GetTableCard2(), GetTableCard3(), GetTableCard4(), GetTableCard5()};
		
	int nArrCards[5];
	memset(nArrCards, 0, sizeof(nArrCards)); // Fill with zero.

	if( pCards->m_cBestCard1 > -1 )
		nArrCards[0] = nCards[pCards->m_cBestCard1];
	if( pCards->m_cBestCard2 > -1 )
		nArrCards[1] = nCards[pCards->m_cBestCard2];
	if( pCards->m_cBestCard3 > -1 )
		nArrCards[2] = nCards[pCards->m_cBestCard3];
	if( pCards->m_cBestCard4 > -1 )
		nArrCards[3] = nCards[pCards->m_cBestCard4];
	if( pCards->m_cBestCard5 > -1 )
		nArrCards[4] = nCards[pCards->m_cBestCard5];

//	 For testing :)
//	int nArrCards[] = {2, 3, 26, 27, 29, 34, 30};
//	handCards = PokerHandCardsClass::CardsClass_Flush;
//	CDWordArray arr;
//	pArrWinnerCards = &arr;

	AutoSortedArray arrCards;

	std::string sCardKind[]			= {_T("{{Cross}}"), _T("{{Diamond}}"), _T("{{Spade}}"), _T("{{Heart}}")};
	std::string sCardKind_Pl[]		= {_T("{{Crosses}}"), _T("{{Diamonds}}"), _T("{{Spades}}"), _T("{{Hearts}}")};
	std::string sCardNames[]		= {_T("{{Two}}"), _T("{{Three}}"), _T("{{Four}}"), _T("{{Five}}"), _T("{{Six}}"), _T("{{Seven}}"), _T("{{Eight}}"),
								_T("{{Nine}}"), _T("{{Ten}}"), _T("{{Jack}}"), _T("{{Queen}}"), _T("{{King}}"), _T("{{Ace}}")};
	std::string sCardNames_Pl[]		= {_T("{{Twos}}"), _T("{{Threes}}"), _T("{{Fours}}"), _T("{{Fives}}"), _T("{{Sixes}}"), _T("{{Sevens}}"), _T("{{Eights}}"),
								_T("{{Nines}}"), _T("{{Tens}}"), _T("{{Jacks}}"), _T("{{Queens}}"), _T("{{Kings}}"), _T("{{Aces}}")};

	std::string sHandCards			= _T("");
	int		nCardsCt			= sizeof(nArrCards)/sizeof(int);
	int		nCard1, nCard2;
	switch( pCards->m_cBestCardsClass ){
		case CardsClass_StraightFlush:{
			if( pArrWinnerCards ){
				pArrWinnerCards->Add(nArrCards[0]);
				pArrWinnerCards->Add(nArrCards[1]);
				pArrWinnerCards->Add(nArrCards[2]);
				pArrWinnerCards->Add(nArrCards[3]);
				pArrWinnerCards->Add(nArrCards[4]);
				}
			sHandCards = _T("{{StraightFlush}}");
			break;
			}
		case CardsClass_FourOfAKind:{
			sHandCards = _T("{{FourOfAKind}}");

			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add((void*)(nArrCards[i]%13), ((void*)nArrCards[i]));
				}

			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				// Found out starting point.
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 4 ){
					if( pArrWinnerCards ){
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i));
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i+1));
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i+2));
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i+3));
						}
					sHandCards.Format(_T("{{ThreeOfAKind}}, %s"), sCardNames_Pl[(int)arrCards[i]]);
					break;
					}
				}
			break;
			}
		case CardsClass_FullHouse:{
			sHandCards = _T("{{FullHouse}}");
			
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add((void*)(nArrCards[i]%13), ((void*)nArrCards[i]));
				}

			// Find out pair cards.
			nCard1 = nCard2 = -1;
			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 3 ){
					nCard1 = (int)arrCards.GetData(i);
					}
				else
				if( nCt == 2 ){
					if( (nCard2 == -1)|| (nCard2 != -1 && ((int)arrCards[i]) > nCard2%13) )
						nCard2 = (int)arrCards.GetData(i);
					}
				// Skip the same card values.
				if( nCt > 1 ){
					i += (nCt - 1);
					}
				}

			if( nCard1 != -1 && nCard2 != -1 ){
				if( pArrWinnerCards ){
					int nIndex = arrCards.FindFirst((void*)(nCard1%13));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex+1));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex+2));

					nIndex = arrCards.FindFirst((void*)(nCard2%13));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex+1));
					}

				sHandCards.Format(_T("{{FullHouse}}, %s, %s"), sCardNames_Pl[nCard1%13], sCardNames_Pl[nCard2%13]);
				}
			break;
			}
		case CardsClass_Flush:{
			if( pArrWinnerCards ){
				pArrWinnerCards->Add(nArrCards[0]);
				pArrWinnerCards->Add(nArrCards[1]);
				pArrWinnerCards->Add(nArrCards[2]);
				pArrWinnerCards->Add(nArrCards[3]);
				pArrWinnerCards->Add(nArrCards[4]);
				}
			sHandCards.Format(_T("{{Flush}}, %s"), sCardKind_Pl[nArrCards[0]/13]);
			break;
			}
		case CardsClass_Straight:{
			if( pArrWinnerCards ){
				pArrWinnerCards->Add(nArrCards[0]);
				pArrWinnerCards->Add(nArrCards[1]);
				pArrWinnerCards->Add(nArrCards[2]);
				pArrWinnerCards->Add(nArrCards[3]);
				pArrWinnerCards->Add(nArrCards[4]);
				}
			sHandCards = _T("{{Straight}}");
			break;
			}
		case CardsClass_ThreeOfAKind:{
			sHandCards	= _T("ThreeOfAKind");
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add((void*)(nArrCards[i]%13), ((void*)nArrCards[i]));
				}

			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				// Found out starting point.
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 3 ){
					if( pArrWinnerCards ){
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i));
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i+1));
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i+2));
						}
					sHandCards.Format(_T("{{ThreeOfAKind}}, %s"), sCardNames_Pl[(int)arrCards[i]]);
					break;
					}
				}
			break;
			}
		case CardsClass_TwoPairs:{
			sHandCards = _T("{{TwoPairs}}");
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add((void*)(nArrCards[i]%13), ((void*)nArrCards[i]));
				}

			// Find out pair cards.
			nCard1 = nCard2 = -1;
			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				int nCt = arrCards.GetValueCount(arrCards[i]);
				if( nCt == 2 ){
					if( nCard1 == -1 ){
						nCard1 = (int)arrCards.GetData(i);
						}
					else
					if( (nCard2 == -1)|| (nCard2 != -1 && ((int)arrCards[i]) > nCard2%13) )
						nCard2 = (int)arrCards.GetData(i);
					}
				// Skip the same card values.
				if( nCt > 1 ){
					i += (nCt - 1);
					}
				}

			if( nCard1 != -1 && nCard2 != -1 ){
				if( pArrWinnerCards ){
					int nIndex = arrCards.FindFirst((void*)(nCard1%13));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex+1));

					nIndex = arrCards.FindFirst((void*)(nCard2%13));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex));
					pArrWinnerCards->Add((DWORD)arrCards.GetData(nIndex+1));
					}

				sHandCards.Format(_T("{{TwoPairs}}, %s, %s"), sCardNames_Pl[nCard1%13], sCardNames_Pl[nCard2%13]);
				}
			break;
			}
		case CardsClass_Pair:{
			sHandCards = _T("{{Pair}}");
			// Add cards into list by value
			for( int i=0; i<nCardsCt; i++ ){
				if( nArrCards[i] < 0 ) continue;
				// Card with the same value but different kind.
				arrCards.Add((void*)(nArrCards[i]%13), ((void*)nArrCards[i]));
				}

			for( int i=0; i<arrCards.GetCount() - 1; i++ ){
				// Found out starting point.
				if( arrCards[i] == arrCards[i + 1] ){
					if( (arrCards.GetCount() - i) < 2 ){
						return false; // Not Pair.
						}
					if( pArrWinnerCards ){
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i));
						pArrWinnerCards->Add((DWORD)arrCards.GetData(i+1));
						}
					sHandCards.Format(_T("{{Pair}}, %s"), sCardNames_Pl[(int)arrCards[i]]);
					break;
					}
				}
			break;
			}
		case CardsClass_HighCard:{
			sHandCards = _T("{{HighCard}}");
			break;
			}
		};

	ESLocalizationMan::ParseText(sHandCards);
	if( psLabelText )
		*psLabelText = sHandCards;
	if( bSetAsSeatMessage )
		SetSeatMessage(nSeat, sHandCards, true);
	return true;
	}

bool
PokerTableWnd::SetWinnerTableCards(PokerHandWinnerInfo* pWinnerInfo, CDWordArray* pArrWinnerCards){
	std::string sCardKind[]			= {_T("{{Cross}}"), _T("{{Diamond}}"), _T("{{Spade}}"), _T("{{Heart}}")};
	std::string sCardKind_Pl[]		= {_T("{{Crosses}}"), _T("{{Diamonds}}"), _T("{{Spades}}"), _T("{{Hearts}}")};
	std::string sCardNames[]		= {_T("{{Two}}"), _T("{{Three}}"), _T("{{Four}}"), _T("{{Five}}"), _T("{{Six}}"), _T("{{Seven}}"), _T("{{Eight}}"),
								_T("{{Nine}}"), _T("{{Ten}}"), _T("{{Jack}}"), _T("{{Queen}}"), _T("{{King}}"), _T("{{Ace}}")};
	std::string sCardNames_Pl[]		= {_T("{{Twos}}"), _T("{{Threes}}"), _T("{{Fours}}"), _T("{{Fives}}"), _T("{{Sixes}}"), _T("{{Sevens}}"), _T("{{Eights}}"),
								_T("{{Nines}}"), _T("{{Tens}}"), _T("{{Jacks}}"), _T("{{Queens}}"), _T("{{Kings}}"), _T("{{Aces}}")};

	int nCard1 = pArrWinnerCards->GetCount() > 0 ? pArrWinnerCards->GetAt(0) : -1;
	int nCard2 = pArrWinnerCards->GetCount() > 1 ? pArrWinnerCards->GetAt(1) : -1;
	int nCard3 = pArrWinnerCards->GetCount() > 2 ? pArrWinnerCards->GetAt(2) : -1;
	int nCard4 = pArrWinnerCards->GetCount() > 3 ? pArrWinnerCards->GetAt(3) : -1;
	int nCard5 = pArrWinnerCards->GetCount() > 4 ? pArrWinnerCards->GetAt(4) : -1;

	AutoSortedArray arrTableCards;
	arrTableCards.Add((void*)m_tableInfo.m_cCard1);
	arrTableCards.Add((void*)m_tableInfo.m_cCard2);
	arrTableCards.Add((void*)m_tableInfo.m_cCard3);
	arrTableCards.Add((void*)m_tableInfo.m_cCard4);
	arrTableCards.Add((void*)m_tableInfo.m_cCard5);

	PokerHandCardsClass handCards = (PokerHandCardsClass)pWinnerInfo->m_cHandCardClass;
	std::string sHandCards	= _T("");
	switch( handCards ){
		case CardsClass_StraightFlush:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 &&
				nCard5 != -1){
				sHandCards = _T("{{StraightFlush}}");
				}
			break;
			}
		case CardsClass_FourOfAKind:{
			if( nCard1 != -1 )
				sHandCards.Format(_T("{{FourOfAKind}}, %s"), sCardNames_Pl[nCard1%13]);
			break;
			}
		case CardsClass_FullHouse:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 ){
				sHandCards.Format(_T("{{FullHouse}}, %s, %s"), sCardNames_Pl[nCard1%13], sCardNames_Pl[nCard4%13]);
				}
			break;
			}
		case CardsClass_Flush:{
			if( nCard1 != -1 )
				sHandCards.Format(_T("{{Flush}}, %s"), sCardKind_Pl[nCard1/13]);
			break;
			}
		case CardsClass_Straight:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 &&
				nCard5 != -1){
				sHandCards = _T("{{Straight}}");
				}
			break;
			}
		case CardsClass_ThreeOfAKind:{
			if( nCard1 != -1 )
				sHandCards.Format(_T("{{ThreeOfAKind}}, %s"), sCardNames_Pl[nCard1%13]);
			break;
			}
		case CardsClass_TwoPairs:{
			if( nCard1 != -1 &&
				nCard2 != -1 &&
				nCard3 != -1 &&
				nCard4 != -1 ){
				sHandCards.Format(_T("{{TwoPairs}}, %s, %s"), sCardNames_Pl[nCard1%13], sCardNames_Pl[nCard3%13]);
				}
			break;
			}
		case CardsClass_Pair:{
			if( nCard1 != -1 &&
				nCard2 != -1 ){
				sHandCards.Format(_T("{{Pair}}, %s"), sCardNames_Pl[nCard1%13]);
				}			
			break;
			}
		case CardsClass_HighCard:{
			sHandCards =_T("{{HighCard}}");
			break;
			}
		default:
			return false;
		};

	ESChildControlTableCards* pTableCards = GetTableCardsControl();
	if( pTableCards ){
		int nBastHandCardsIdx[]	= {pWinnerInfo->m_cWinnerCard1, pWinnerInfo->m_cWinnerCard2, pWinnerInfo->m_cWinnerCard3, 
									pWinnerInfo->m_cWinnerCard4, pWinnerInfo->m_cWinnerCard5};
		bool bSeatCardsMarks[]	= {true, true, true, true};
		bool bTableCardsMarks[] = {true, true, true, true, true};

		for(int i=0; i<sizeof(nBastHandCardsIdx)/sizeof(int); i++){
			// Unmark seat card.
			if( nBastHandCardsIdx[i] < 4 ){
				bSeatCardsMarks[nBastHandCardsIdx[i]] = false;
				}
			else{// Unmark table card.
				bTableCardsMarks[nBastHandCardsIdx[i] - 4] = false;
				}
			}

		ESChildControlSeatCards* pSeatCards = GetSeatCardsControl((int)pWinnerInfo->m_cSeat);
		if( pSeatCards )
			pSeatCards->SetCardsMark(bSeatCardsMarks[0], bSeatCardsMarks[1], bSeatCardsMarks[2], bSeatCardsMarks[3], true);

		if( pTableCards )
			pTableCards->SetMarkCards(bTableCardsMarks[0], bTableCardsMarks[1], bTableCardsMarks[2], bTableCardsMarks[3], bTableCardsMarks[4], true);

		/*
		AutoSortedArray arrCards;
		for( int i=0; i<pArrWinnerCards->GetCount(); i++ )
			arrCards.Add((void*)pArrWinnerCards->GetAt(i));

		pTableCards->SetMarkCards(
			(arrCards.IndexOf((void*)m_tableInfo.m_cCard1) == -1),
			(arrCards.IndexOf((void*)m_tableInfo.m_cCard2) == -1),
			(arrCards.IndexOf((void*)m_tableInfo.m_cCard3) == -1),
			(arrCards.IndexOf((void*)m_tableInfo.m_cCard4) == -1),
			(arrCards.IndexOf((void*)m_tableInfo.m_cCard5) == -1));
			*/
		ESLocalizationMan::ParseText(sHandCards);
		SetHandCombinationLabel(sHandCards, true);
		return true;
		}
	return false;
	}

bool
PokerTableWnd::ClearWinnerTableCards(bool bRedraw){
	ESChildControlTableCards* pTableCards = GetTableCardsControl();
	if( pTableCards ){
		bool bSeatCardsMarks[]	= {false, false, false, false};
		bool bTableCardsMarks[] = {false, false, false, false, false};

		for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
			ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(i);
			if( pSeatCards )
				pSeatCards->SetCardsMark(bSeatCardsMarks[0], bSeatCardsMarks[1], bSeatCardsMarks[2], bSeatCardsMarks[3], bRedraw);
			}

		if( pTableCards )
			pTableCards->SetMarkCards(bTableCardsMarks[0], bTableCardsMarks[1], bTableCardsMarks[2], bTableCardsMarks[3], bTableCardsMarks[4], bRedraw);

		ClearHandCombinationLabel(bRedraw);
		return true;
		}
	return false;
	}

void
PokerTableWnd::SetBestHandLabel(std::string sText, bool bInvalidate /*= true*/){
	SetHandCombinationLabel(sText, bInvalidate);
	/*
	ESChildControlLabel* pLabel = (ESChildControlLabel*)GetChildControl(CHILD_Table_Best_Hand_Label);
	if( !pLabel ){
		pLabel = new ESChildControlLabel();
		if( AddChildControlEx(pLabel, CHILD_Table_Best_Hand_Label, m_pGUI->m_rcPlayerBestHandLabel, NULL, m_pGUI->m_szDefWindowSize) ){
//			pLabel->SetZIndex	(1);
			pLabel->SetTextColor(RGB(255, 255, 255));
			pLabel->SetFont		(m_pGUI->m_pTMessageFont);
			pLabel->SetCentred	(true);
			}
		else{
			ASSERT(FALSE);
			return;
			}
		}

	if( !pLabel->IsVisible() )
		pLabel->SetVisible(true, false);
	pLabel->SetLabel(sText, bInvalidate);*/
	}

void
PokerTableWnd::ClearBestHandLabel(bool bInvalidate /*= true*/){
	ClearHandCombinationLabel(bInvalidate);
	/*
	ESChildControlLabel* pLabel = (ESChildControlLabel*)GetChildControl(CHILD_Table_Best_Hand_Label);
	if( pLabel ){
		pLabel->SetLabel	(_T(""), false);
		pLabel->SetVisible	(false, bInvalidate);
		}*/
	}

bool
PokerTableWnd::SetFourColorCards(bool bFourColor, bool bRedraw){
	// Update seat cards.
	std::string sName;
	for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
		ESChildControlSeatCards* pSeatCards = GetSeatCardsControl(i);
		if( pSeatCards ){
			if( bRedraw && pSeatCards->IsVisible() )
				pSeatCards->Redraw();
			}
		}

	// Update table cards.
	ESChildControlTableCards* pTableCards = GetTableCardsControl();
	if( pTableCards ){
		if( bRedraw && pTableCards->IsVisible() && 
			(pTableCards->GetCard1() != -1 || pTableCards->GetCard2() != -1 || pTableCards->GetCard3() != -1 || pTableCards->GetCard4() != -1 || pTableCards->GetCard5() != -1) ){
			pTableCards->Redraw();
			}
		}
	return true;
	}

void
PokerTableWnd::SetCardBackIndex	(int nCardBackIndex, bool bRedraw){
	std::string sName;
	PokerTableSettings *pSettings = PokerTableSettings::GetInstance();

	for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
		ESChildControlSeatHiddenCards* pSeatHiddenCards = GetSeatHiddenCardsControl(i);
		if( pSeatHiddenCards )
			pSeatHiddenCards->SetCardBackIndex(pSettings->m_nCardsBackIndex, bRedraw);
		}

	ESChildControlSeatHiddenCards* pSeatHiddenCards = GetDeckControl();
	if( pSeatHiddenCards )
		pSeatHiddenCards->SetCardBackIndex(pSettings->m_nCardsBackIndex, bRedraw);
	}

void
PokerTableWnd::SetDisplayFoldedCards(bool bOn, bool bRedraw){
	for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
		PokerTableSeatInfo*			pInfo		= GetSeatInfo(i);
		if( !pInfo || pInfo->GetCard1() < 0 ) continue;
		ESChildControlSeatCards*	pSeatCards	= GetSeatCardsControl(i);
		if( pSeatCards ){
			if( pInfo->GetStatus() == PokerSeatStatus::Folded ){
				pSeatCards->SetVisible(bOn, bRedraw);
				}
			}
		}
	}

void
PokerTableWnd::SetHideHoleHandCards(bool bOn, bool bRedraw){
	for(int i=0; i<m_tableInfo.m_seats.GetCount(); i++){
		PokerTableSeatInfo*			pInfo		= GetSeatInfo(i);
		ESChildControlSeatCards*	pSeatCards	= GetSeatCardsControl(i);
		if( pSeatCards ){
			if( !pInfo || pInfo->GetStatus() == PokerSeatStatus::Empty || pInfo->GetStatus() == PokerSeatStatus::Folded || pInfo->GetCard1() < 0 ){
				pSeatCards->HideCards		(false);
				pSeatCards->AllowMouseEvents(false);
				}
			else{
				if( pInfo->GetStatus() != PokerSeatStatus::All_In && pInfo->GetStatus() != PokerSeatStatus::ShownCards && pInfo->GetStatus() != PokerSeatStatus::Folded ){
					pSeatCards->AllowMouseEvents(true);
					pSeatCards->HideCards		(bOn);
					}
				else{
					pSeatCards->HideCards		(false);
					pSeatCards->AllowMouseEvents(false);
					}
				if( bRedraw )
					pSeatCards->Redraw();
				}
			}
		}
	}

void
PokerTableWnd::SetShowPlayerImages(bool bOn, bool bRedraw){
	}

bool
PokerTableWnd::SetSeatNoteAndColor(int nSeat, std::string sNote, COLORREF crBorder, bool bRedraw){
	std::string sName;
	sName.Format(_T("_pokerSeat%d"), nSeat+1);
	ESChildSeat* pSeat = (ESChildSeat*)GetChildControlByName(sName.GetBuffer());
	if( !pSeat ) return false;

	pSeat->SetBorderColor	(crBorder, false);
	pSeat->SetUserNote		(sNote, false);
	if( bRedraw )
		pSeat->Redraw();
	return true;
	}

bool
PokerTableWnd::SetSeatNote(int nSeat, std::string sNote, bool bRedraw){
	std::string sName;
	sName.Format(_T("_pokerSeat%d"), nSeat+1);
	ESChildSeat* pSeat = (ESChildSeat*)GetChildControlByName(sName.GetBuffer());
	if( !pSeat ) return false;
	pSeat->SetUserNote		(sNote, false);
	if( bRedraw )
		pSeat->Redraw();
	return true;
	}

bool
PokerTableWnd::SetSeatNoteColor(int nSeat, COLORREF crBorder, bool bRedraw){
	std::string sName;
	sName.Format(_T("_pokerSeat%d"), nSeat+1);
	ESChildSeat* pSeat = (ESChildSeat*)GetChildControlByName(sName.GetBuffer());
	if( !pSeat ) return false;
	pSeat->SetBorderColor	(crBorder, false);
	if( bRedraw )
		pSeat->Redraw();
	return true;
	}

bool
PokerTableWnd::ClearSeatNote(int nSeat, bool bRedraw){
	std::string sName;
	sName.Format(_T("_pokerSeat%d"), nSeat+1);
	ESChildSeat* pSeat = (ESChildSeat*)GetChildControlByName(sName.GetBuffer());
	if( !pSeat ) return false;
	pSeat->ClearUserNote	(bRedraw);
	return true;
	}

bool
PokerTableWnd::ClearSeatNoteColor(int nSeat, bool bRedraw){
	std::string sName;
	sName.Format(_T("_pokerSeat%d"), nSeat+1);
	ESChildSeat* pSeat = (ESChildSeat*)GetChildControlByName(sName.GetBuffer());
	if( !pSeat ) return false;
	pSeat->ClearBorderColor	(bRedraw);
	return true;
	}

//=======================================================
//
//	Designer context menu commands.
//

void
PokerTableWnd::OnCommandTable(){
	ESChildControlTable*	pControlNew = new ESChildControlTable();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandSeatStatusAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 ) return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected ) return;

	ESChildSeatStatus*	pControlNew = new ESChildSeatStatus();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandSeatAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 ) return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected ) return;

	ESChildSeat*	pControlNew = new ESChildSeat();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandChipsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 ) return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected ) return;

	ESChildControlChips*	pControlNew = new ESChildControlChips();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandHiddenCardsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 ) return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected ) return;

	ESChildControlSeatHiddenCards*	pControlNew = new ESChildControlSeatHiddenCards();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandSeatCardsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 ) return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected ) return;

	ESChildControlSeatCards*	pControlNew = new ESChildControlSeatCards();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandTableCardsAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 ) return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected ) return;

	ESChildControlTableCards*	pControlNew = new ESChildControlTableCards();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
PokerTableWnd::OnCommandPreActionAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 ) return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected ) return;

	ESChildPreActionControl*	pControlNew = new ESChildPreActionControl();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}