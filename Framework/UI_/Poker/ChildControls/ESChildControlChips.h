#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlChips window

#include "../../../Controls/ESChildControl.h"

enum PositionDef{
	Left, Right, Top, Bottom
	};

enum HorzAlignmentDef{
	Align_Left, Align_Right, Align_Middle
	};

enum ChipGrowDirection{
	LeftToRight = 0,
	RightToLeft
	};

class ESChildControlChips : public ESChildControl
{
// Construction
public:
	ESChildControlChips();
	virtual ~ESChildControlChips();

	virtual bool	Create					(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetFormatString			(_String sFormat);
	bool			SetAmount				(INT_CHIPS nAmount, bool bInvalidate = true);
	bool			AddAmount				(INT_CHIPS nAmountAdd, bool bInvalidate = true);
	bool			SetCaching				(bool bAllowCaching) { m_bAllowCaching = bAllowCaching; };

	INT_CHIPS		GetAmount				()					{return m_nAmount;};
	void			SetDrawLabelText		(bool bDraw)		{m_bDrawLabelText = bDraw;};
	void			SetLabelTextPosition	(int nLabelTextPos)	{m_textPosition.SetCurrentValue(nLabelTextPos);};
	int				GetChipHAlign			()					{m_chipHorzAlignment.GetCurrentValue();};
	int				GetWidthDrawn			()					{return m_nWidthDrawn;};

	virtual void	Redraw					(bool bLayersToo = true);
	virtual bool	PtInClientArea			(_Point pt);

protected:
	virtual void	OnPaintClient			(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	// Motion
	virtual void	OnMotionStart			(DWORD_PTR dwParam);
	virtual void	OnMotionProgress		(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY);
	virtual void	OnMotionEnd				();
	virtual void	OnMotionStopped			();

	virtual void	OnLocalizeStrings		();
	virtual bool	GetTooltipText			(_String& sTooltipText);

	// Designer virtual methods
	virtual _String	GetControlDefaultName	(){return _T("_pokerChip");};
	virtual _Size	GetControlDefaultSize	(){return _Size(80, 80);};

protected:
	void			UpdateLabelText			();
	bool			RenderChipsAndLabel		(_Rect& rcLabel, _Rect& rcBoundChips, _DC* pDC = NULL, _Rect* pRectDC = NULL, _Rect* pRectClient = NULL);

protected:
	INT_CHIPS			m_nAmount;					// Chip amount.
	_String				m_sLabel;					// Chips label text.

	int					m_nDistanceCY;				// Chip distance 
	_String				m_sFormat;					// Chip label text format.
	_String				m_sFormatLocal;				// Localized chip label text format.
	Font				m_labelFont;				// Label font.
	COLORREF			m_crLabel;					// Label color.

	bool				m_bGroupSameChips;			// Group chips.
	StringToIntArray	m_textPosition;
	bool				m_bDrawLabelText;			// Label text visibility.
	float				m_fZoom;
	_Size				m_szChipDefault;			// Chip size default.
	StringToIntArray	m_chipHorzAlignment;		// Chips horizontal alignment.
	int					m_nDrawChipPartCY;			// Partially draw chip when it is not on top and mose part of it is covered by another chip.

	bool				m_bShadow;					// Shadow.
	COLORREF			m_crShadow;					// Shadow color.
	_Point				m_ptShadowPos;				// Shadow offset position.

	int					m_nDistanceCYCurrent;		// Current CY distance.
	int					m_nDrawChipPartCYCurrent;
	int					m_nWidthDrawn;				// Drawn width of chips.

	bool				m_bAllowCaching;			// Chips image caching.
private:
	_Rect				m_rcLabel;
	_Rect				m_rcChips;
	float				m_fLabelTextMaxZoom;

	// Chips image caching.
	Image				m_imgChips_Cached;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlChips();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};