#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildProgressCtrl window

#include "ESChildControl.h"

class ESChildProgressCtrl : public ESChildControl
{
// Construction
public:
	ESChildProgressCtrl();
	virtual ~ESChildProgressCtrl();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();

	bool			SetupProgress			(int nPosMin, int nPosMax, bool bRedraw = true);
	bool			SetProgressPos			(int nPos, bool bRedraw = true);
	int				GetProgressPos			();
	int				GetProgressPosMax		(){return m_nPosMax;};
	int				GetProgressPercentage	();

protected:
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_progress");};
	virtual _Size	GetControlDefaultSize	(){return _Size(150, 20);};

protected:
	// UI parameters. {{
	_Image			m_imageProgressBarFilled;
	_string			m_sImageProgressBarFilled;
	_Rect			m_rcOmitProgressBarFilled;

	_Image			m_imageProgressBarBg;
	_string			m_sImageProgressBarBg;
	_Rect			m_rcOmitProgressBg;
	// }}

	int				m_nPosMax;
	int				m_nPosMin;
	int				m_nCurrPos;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildProgressCtrl();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};