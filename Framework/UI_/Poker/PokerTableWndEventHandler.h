#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerTableWndEventHandler window
#include "..\..\Controls\ESFrameBase.h"
#include "..\..\UI\Poker\PokerGUI.h"

class PokerTableSeatInfo;
class ESChildControlSeatHiddenCards;
class PokerTableWndEventHandler : public ESFrameBase
{
// Construction
public:
	PokerTableWndEventHandler();
	virtual ~PokerTableWndEventHandler();

	friend class PokerTableWnd;

protected:
	virtual void	OnActionButtonClicked				(ActionButtonInfo* pActionInfo);
	virtual void	OnLeaveTableClicked					(){};
	virtual void	OnViewLobbyClicked					(){};
	virtual void	OnBuyChipsClicked					(){};
	virtual void	OnAddChipsButtonClicked				(int nAmount);
	virtual void	OnPreActionButtonChecked			(PreActionButtonInfo* pActionInfo, bool bCheckState);
	virtual void	OnSeatPanelClick					(int nSeatIndex, PokerTableSeatInfo* pInfo);
	virtual void	OnAddChipsControlClose				(){};
	
	virtual void	OnCollectChipsFinished				(){};
	virtual void	OnCollectChipsStarted				(){};
	virtual void	OnCollectChipsMotionStart			(){};

	virtual void	OnMoveChipsFromPotFinished			(){};
	virtual void	OnMoveChipsFromPotStarted			(){};
	virtual void	OnMoveChipsFromPotMotionStart		(){};

	virtual void	OnTableCardsDealAnimationStarted	(){};
	virtual void	OnTableCardsDealAnimationFinished	(){};
	virtual void	OnSeatCardsDealAnimationStarted		(){};
	virtual void	OnSeatCardsDealAnimationFinished	(){};
	virtual void	OnSeatCardDealMotionEnd				(ESChildControlSeatHiddenCards* pCards){};
	
	virtual void	OnSeatTimeBankActivated				(int nSeatIndex, int nTimeBankSec){};
	virtual bool	SeatOccupied						(){return false;};
};