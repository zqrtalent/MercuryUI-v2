// PokerTournamentLobbyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Controls\MercuryGUI.h"
#include "ChildControls\ESChildControlButton.h"
#include "..\..\Controls\ESChildControlImage.h"
#include "..\..\Controls\ESChildControlImageButton.h"
#include "..\..\Controls\ESChildControlLabel.h"
//#include "ChildControls\ESChildControlPreAction.h"
//#include "ChildControls\ESMessageBox.h"

#include "..\..\Image\ImageHelper.h"
#include "..\..\Path\Path.h"
#include "PokerTournamentLobbyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PokerTournamentLobbyWnd

PokerTournamentLobbyWnd::PokerTournamentLobbyWnd() : MercuryBaseView(){
	m_bUseForegroundColor	= 0;
	m_crForeground			= RGB(0, 0, 0);
	m_nAlpha				= 50;
	m_pClient				= NULL;
	}

PokerTournamentLobbyWnd::~PokerTournamentLobbyWnd(){
	}

bool
PokerTournamentLobbyWnd::AttachToFrame(ESFrameBase* pBaseFrameWnd){
	bool bRet = MercuryBaseView::AttachToFrame(pBaseFrameWnd);
	if( bRet ){
		}
	return bRet;
	}