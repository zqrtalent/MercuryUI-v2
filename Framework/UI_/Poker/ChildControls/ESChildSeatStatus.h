#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeat window

#include "../../../Controls/ESChildControl.h"

class ESChildSeatStatus : public ESChildControl
{
// Construction
public:
	ESChildSeatStatus();
	virtual ~ESChildSeatStatus();

	virtual bool		Create					(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		Destroy					();
	virtual void		OwnerWindowSizeChanged	(int cx, int cy);

	bool				IsSeatActive			(){return m_bActive;};
	void				SetSeatActive			(bool bActive, bool bRedraw = true);
	void				SetSeatIndex			(int nSeat){m_nSeat = nSeat;}

	void				SetUserName				(_String sUserName, bool bRedraw);
	void				SetUserBalance			(_String sBalance, bool bRedraw);
	_String				GetUserBalanceText		();
	void				SetZoomFactor			(float fZoom, bool bRedraw);

protected:
	virtual void	OnPaintClient					(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	// Remain control captured out of client area.
	virtual bool	RemainCapturedOutOfClientArea	(){return false;};
	// Retrieve region, bound size and centered flag.
	virtual CRgn*	GetControlRgn					(_Size& szRgnBound, bool& bCentered);
	// Designer virtual methods
	virtual _String	GetControlDefaultName			(){return _T("_pokerSeatStatus");};
	virtual _Size	GetControlDefaultSize			(){return _Size(150, 100);};
	virtual bool	AllowToAddChildControl			(){return true;};

protected:
	_String						m_sBgImageFile;			// Background image file.
	_String						m_sActiveBgImageFile;	// Active background image file.
	StringToIntArray			m_imageFillMode;		// Bg image fill mode.
	_Rect						m_rcOmitBorder;			// Bg image omitborder rect.
	bool						m_bActive;				// Active state.
	int							m_nSeat;				// Seat index.

	float						m_fZoom;				// Zoom factor.

	Image*						m_pBgImage;
	Image*						m_pBgActiveImage;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildSeatStatus();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};