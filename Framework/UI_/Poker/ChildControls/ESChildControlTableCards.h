#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlTableCards window

#include "../../../Controls/ESChildControl.h"

class ESChildControlTableCards : public ESChildControl
{
// Construction
public:
	ESChildControlTableCards();
	virtual ~ESChildControlTableCards();

	virtual bool	Create				(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy				();
	virtual void	OwnerWindowSizeChanged(int cx, int cy);

	void			SetCard1			(int nCard1){m_nCard1 = nCard1;};
	void			SetCard2			(int nCard2){m_nCard2 = nCard2;};
	void			SetCard3			(int nCard3){m_nCard3 = nCard3;};
	void			SetCard4			(int nCard4){m_nCard4 = nCard4;};
	void			SetCard5			(int nCard5){m_nCard5 = nCard5;};
	int				GetCard1			(){return m_nCard1;};
	int				GetCard2			(){return m_nCard2;};
	int				GetCard3			(){return m_nCard3;};
	int				GetCard4			(){return m_nCard4;};
	int				GetCard5			(){return m_nCard5;};
	void			SetCard1OffsetX		(int nOffsetX){m_nCard1OffsetX = nOffsetX;};
	void			SetCard2OffsetX		(int nOffsetX){m_nCard2OffsetX = nOffsetX;};
	void			SetCard3OffsetX		(int nOffsetX){m_nCard3OffsetX = nOffsetX;};
	void			SetCard4OffsetX		(int nOffsetX){m_nCard4OffsetX = nOffsetX;};
	void			SetCard5OffsetX		(int nOffsetX){m_nCard5OffsetX = nOffsetX;};

	void			CalcCardRects		();
	void			ClearMarks			();
	void			SetMarkCards		(bool bMarkCard1, bool bMarkCard2, bool bMarkCard3, bool bMarkCard4, bool bMarkCard5, bool bRedraw = true);

	_Size			GetCardDefaultSize	(){return m_szCard;};
	int				GetWidthCards		(int nCardsCt);
	_Rect			GetCardRect			(int nCardIndex);

protected:
	virtual void	OnPaintClient		(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	// Motion
	virtual void	OnMotionStart		(DWORD_PTR dwParam);
	virtual void	OnMotionProgress	(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY);
	virtual void	OnMotionEnd			();
	virtual void	OnMotionStopped		();

	// Designer virtual methods
	virtual _String	GetControlDefaultName	(){return _T("_tableCards");};
	virtual _Size	GetControlDefaultSize	(){return _Size(80, 80);};

protected:
	int				m_nCard1;
	int				m_nCard2;
	int				m_nCard3;
	int				m_nCard4;
	int				m_nCard5;

	int				m_nCard4Temp;
	int				m_nCard5Temp;
	float			m_fDeckPosXSave;
	float			m_fDeckPosYSave;

	int				m_nCard1OffsetX;
	int				m_nCard2OffsetX;
	int				m_nCard3OffsetX;
	int				m_nCard4OffsetX;
	int				m_nCard5OffsetX;

	int				m_nCard1OffsetXSave;
	int				m_nCard2OffsetXSave;
	int				m_nCard3OffsetXSave;
	int				m_nCard4OffsetXSave;
	int				m_nCard5OffsetXSave;

	bool			m_bMarkCard1;			// Mark flag for card1.
	bool			m_bMarkCard2;			// Mark flag for card2.
	bool			m_bMarkCard3;			// Mark flag for card3.
	bool			m_bMarkCard4;			// Mark flag for card4.
	bool			m_bMarkCard5;			// Mark flag for card5.

	int				m_nDistanceCX;			// Distance between cards.
	float			m_fZoom;				// Zoom factor.

	_Size			m_szCard;				// Default card dimensions.
	_Size			m_szCardCurrent;		// Current card dimensions.
	bool			m_bCentred;				// Cards make centred.

	// Card rects.
	_Rect			m_rcCard1;
	_Rect			m_rcCard2;
	_Rect			m_rcCard3;
	_Rect			m_rcCard4;
	_Rect			m_rcCard5;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlTableCards();};
	virtual bool			AllowToAddChildControl		(){return true;};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
//	static bool	AttributeModify(ESChildControlTableCards* pThis, VariableInfo* pInfo, void* lpValueNew);
};