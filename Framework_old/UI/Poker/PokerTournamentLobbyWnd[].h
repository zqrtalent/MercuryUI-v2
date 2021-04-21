#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerTournamentLobbyWnd window

#include "..\..\Controls\ESChildControl.h"
#include "..\..\Controls\MercuryBaseWnd.h"
#include "..\..\Protocol\_PokerDefines.h"
#include "..\..\Array\AutoSortedArrayTempl.h"
#include "..\..\Array\AutoSortedArray.h"
#include "..\..\Array\StringToPtrArray.h"
#include "..\..\PokerServer\Data\Tables\PokerAvatar.h"
#include "..\..\GameClient\PokerClient.h"
#include "..\..\Controls\ESChildControlList.h"
#include "PokerGUI.h"
#include <atlimage.h>

class PokerGUI;
class PokerTournamentLobbyWnd : public MercuryBaseView
{
// Construction
public:
	PokerTournamentLobbyWnd();
	virtual ~PokerTournamentLobbyWnd();

	friend class PokerLobbyDlg;
	
public: // Operations.
	void					SetPokerClient		(PokerClient* pClient) {m_pClient = pClient;};

protected:
	virtual bool			AttachToFrame		(ESFrameBase* pBaseFrameWnd);
	virtual BOOL			UseDoubleBufferedDC	(){return TRUE;};

protected:
	PokerClient*			m_pClient;
};