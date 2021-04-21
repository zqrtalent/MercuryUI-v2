#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeatCards window

#include "..\..\..\Controls\ESChildControl.h"

class ESChildControlSeatCards : public ESChildControl
{
// Construction
public:
	ESChildControlSeatCards();
	virtual ~ESChildControlSeatCards();

	virtual bool	Create					(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetCards				(int nCard1, int nCard2, int nCard3, int nCard4, bool bRedraw);
	void			SetCardsMark			(bool bMark1, bool bMark2, bool bMark3, bool bMark4, bool bRedraw);

	int				GetCard1				(){return m_nCard1;};
	int				GetCard2				(){return m_nCard2;};
	int				GetCard3				(){return m_nCard3;};
	int				GetCard4				(){return m_nCard4;};
	Point			GetCard1Offset			(){return m_ptCard1Offset;};
	Point			GetCard2Offset			(){return m_ptCard2Offset;};
	Point			GetCard3Offset			(){return m_ptCard3Offset;};
	Point			GetCard4Offset			(){return m_ptCard4Offset;};
	Size			GetCardDefaultSize		(){return m_szCard;};
	void			SetCard1Offset			(Point ptOffset){m_ptCard1Offset = ptOffset;};
	void			SetCard2Offset			(Point ptOffset){m_ptCard2Offset = ptOffset;};
	void			SetCard3Offset			(Point ptOffset){m_ptCard3Offset = ptOffset;};
	void			SetCard4Offset			(Point ptOffset){m_ptCard4Offset = ptOffset;};
	void			HideCards				(bool bHide){m_bHideCards = bHide;};
	int				GetSeatIndex			();

	Point			GetCardPointByIndex		(int nCardIndex);
	Point			GetCardCenterByIndex	(int nCardIndex);

protected:
	virtual void	OnPaintClient			(_DC* pDC, Rect* pRectDC, Rect* pRectClient);
	// Designer virtual methods
	virtual std::string	GetControlDefaultName	(){return _T("_seatCards");};
	virtual Size	GetControlDefaultSize	(){return Size(80, 80);};
	
protected:
	Point			m_ptCard1Offset;
	int				m_nCard1;
	bool			m_bMark1;
	Point			m_ptCard2Offset;
	int				m_nCard2;
	bool			m_bMark2;
	Point			m_ptCard3Offset;
	int				m_nCard3;
	bool			m_bMark3;
	Point			m_ptCard4Offset;
	int				m_nCard4;
	bool			m_bMark4;

	Size			m_szCard;
	float			m_fZoom;
	bool			m_bHideCards;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlSeatCards();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};