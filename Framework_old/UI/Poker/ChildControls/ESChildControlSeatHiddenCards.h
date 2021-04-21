#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeatHiddenCards window

#include "..\..\..\Controls\ESChildControl.h"

class ESChildControlSeatHiddenCards : public ESChildControl
{
// Construction
public:
	ESChildControlSeatHiddenCards();
	virtual ~ESChildControlSeatHiddenCards();

	virtual bool	Create					(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetCards				(bool bCard1, bool bCard2, bool bCard3, bool bCard4, bool bRedraw);
	// Deal seat cards animation.
	void			SetupForAnimation		(int nSeat, int nCardIndex, int nCardValue);
	int				GetCardOffsetX			(int nCardIndex);
	int				GetCardOffsetY			(int nCardIndex);
	Point			GetCardPointByIndex		(int nCardIndex);
	void			SetCardBackIndex		(int nBackIndex, bool bRedraw);

	void			SetAnimationAlpha		(BYTE btAlpha){m_btAnimationAlpha = btAlpha;};

protected:
	virtual void	OnPaintClient			(_DC* pDC, Rect* pRectDC, Rect* pRectClient);
	virtual void	OnLButtonClick			(Point pt, UINT nFlags);

	// Motion
	virtual void	OnMotionStart			(DWORD_PTR dwParam);
	virtual void	OnMotionProgress		(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY);
	virtual void	OnMotionEnd				();
	virtual void	OnMotionStopped			();

	// Designer virtual methods
	virtual std::string	GetControlDefaultName	(){return _T("_seatHiddenCards");};
	virtual Size	GetControlDefaultSize	(){return Size(80, 80);};
	
protected:
	Point			m_ptCard1Offset;
	bool			m_bCard1;
	Point			m_ptCard2Offset;
	int				m_bCard2;
	Point			m_ptCard3Offset;
	int				m_bCard3;
	Point			m_ptCard4Offset;
	bool			m_bCard4;

	BYTE			m_btAnimationAlpha;
	int				m_nAlphaConstant;
	int				m_nBackIndex;		// Card back index.
	Size			m_szCardBack;
	float			m_fZoom;

	// Deal seat cards animation.
	int				m_nSeatIndex;
	int				m_nCardIndex;
	int				m_nCardValue;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlSeatHiddenCards();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};