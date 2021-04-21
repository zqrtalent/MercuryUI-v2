#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESDialogBase window

#include "ESChildControl.h"

class ESDialogBase : public ESChildControl
{
// Construction
public:
	ESDialogBase();
	virtual ~ESDialogBase();

	virtual bool			Create					(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool			SetPos					(int x, int y, bool bRedraw = true);
	virtual bool			SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool			Destroy					();
	virtual void			OwnerWindowSizeChanged	(int cx, int cy);

	virtual void			OnInitDialog			(){};

	virtual BOOL			OnOK					(){return TRUE;};
	virtual BOOL			OnCancel				(){return TRUE;};
	virtual BOOL			OnTabControl			(){return TRUE;};

	virtual ESChildControl*	InitChildControl		(DialogItem* pItem){return NULL;};
	virtual _Rect			GetDialogOmitBorder		(){return _Rect(0, 0, 0, 0);};

	virtual void			SetTitleBar				(bool bTitleBar){};
	virtual void			SetFont					(Font* pFont, bool bDestroy = false){};
	virtual void			SetTextColor			(COLORREF crText) {};
	virtual void			SetTitle				(_String sText, bool bRedraw){};
	virtual void			SetCentred				(bool bCentred){};
	virtual void			SetSizeTextFont			(bool bSizeTextFont){};
	virtual void			SetFillColor			(COLORREF crFill){};
	virtual int				GetCloseButtonId		(){return 0;};
};