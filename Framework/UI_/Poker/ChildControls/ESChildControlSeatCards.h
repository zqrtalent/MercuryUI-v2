#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeatCards window

#include "../../../Controls/ESChildControl.h"

class ESChildControlSeatCards : public ESChildControl
{
// Construction
public:
	ESChildControlSeatCards();
	virtual ~ESChildControlSeatCards();

	virtual bool	Create					(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetCards				(int nCard1, int nCard2, int nCard3, int nCard4, bool bRedraw);
	void			SetCardsMark			(bool bMark1, bool bMark2, bool bMark3, bool bMark4, bool bRedraw);
	void			SetMaxCardsCount		(int nMaxCardsCt){m_nCardsCtMax = nMaxCardsCt;};

	int				GetCard1				(){return m_nCard1;};
	int				GetCard2				(){return m_nCard2;};
	int				GetCard3				(){return m_nCard3;};
	int				GetCard4				(){return m_nCard4;};
	_Point			GetCard1Offset			(){return m_ptCard1Offset;};
	_Point			GetCard2Offset			(){return m_ptCard2Offset;};
	_Point			GetCard3Offset			(){return m_ptCard3Offset;};
	_Point			GetCard4Offset			(){return m_ptCard4Offset;};
	_Size			GetCardDefaultSize		(){return m_szCard;};
	void			SetCard1Offset			(_Point ptOffset){m_ptCard1Offset = ptOffset;};
	void			SetCard2Offset			(_Point ptOffset){m_ptCard2Offset = ptOffset;};
	void			SetCard3Offset			(_Point ptOffset){m_ptCard3Offset = ptOffset;};
	void			SetCard4Offset			(_Point ptOffset){m_ptCard4Offset = ptOffset;};
	void			HideCards				(bool bHide){m_bHideCards = bHide;};
	int				GetSeatIndex			();

	_Point			GetCardPointByIndex		(int nCardIndex);
	_Point			GetCardCenterByIndex	(int nCardIndex);

protected:
	virtual void	OnPaintClient			(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	// Designer virtual methods
	virtual _String	GetControlDefaultName	(){return _T("_seatCards");};
	virtual _Size	GetControlDefaultSize	(){return _Size(80, 80);};
	
protected:
	bool			m_bUseCardOffsets;
	bool			m_bCardsCentered;
	_Point			m_ptCard1Offset;
	int				m_nCard1;
	bool			m_bMark1;
	_Point			m_ptCard2Offset;
	int				m_nCard2;
	bool			m_bMark2;
	_Point			m_ptCard3Offset;
	int				m_nCard3;
	bool			m_bMark3;
	_Point			m_ptCard4Offset;
	int				m_nCard4;
	bool			m_bMark4;

	_Size			m_szCard;
	float			m_fZoom;
	bool			m_bHideCards;
	int				m_nCardsCtMax;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlSeatCards();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};