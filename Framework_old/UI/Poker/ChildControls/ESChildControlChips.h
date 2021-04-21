#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlChips window

#include "..\..\..\Controls\ESChildControl.h"

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

	virtual bool	Create					(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetFormatString			(std::string sFormat);
	bool			SetAmount				(int nAmount, bool bInvalidate = true);
	bool			AddAmount				(int nAmountAdd, bool bInvalidate = true);

	int				GetAmount				()					{return m_nAmount;};
	void			SetDrawLabelText		(bool bDraw)		{m_bDrawLabelText = bDraw;};
	void			SetLabelTextPosition	(int nLabelTextPos)	{m_textPosition.SetCurrentValue(nLabelTextPos);};
	int				GetChipHAlign			()					{m_chipHorzAlignment.GetCurrentValue();};
	int				GetWidthDrawn			()					{return m_nWidthDrawn;};

	virtual void	Redraw					(bool bLayersToo = true);
	virtual bool	PtInClientArea			(Point pt);

protected:
	virtual void	OnPaintClient			(_DC* pDC, Rect* pRectDC, Rect* pRectClient);
	// Motion
	virtual void	OnMotionStart			(DWORD_PTR dwParam);
	virtual void	OnMotionProgress		(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY);
	virtual void	OnMotionEnd				();
	virtual void	OnMotionStopped			();

	virtual void	OnLocalizeStrings		();
	virtual bool	GetTooltipText			(std::string& sTooltipText);

	// Designer virtual methods
	virtual std::string	GetControlDefaultName	(){return _T("_pokerChip");};
	virtual Size	GetControlDefaultSize	(){return Size(80, 80);};

protected:
	void			UpdateLabelText			();
	bool			RenderChipsAndLabel		(Rect& rcLabel, Rect& rcBoundChips, _DC* pDC = NULL, Rect* pRectDC = NULL, Rect* pRectClient = NULL);

protected:
	int					m_nAmount;					// Chip amount.
	std::string				m_sLabel;					// Chips label text.

	int					m_nDistanceCY;				// Chip distance 
	std::string				m_sFormat;					// Chip label text format.
	std::string				m_sFormatLocal;				// Localized chip label text format.
	Font				m_labelFont;				// Label font.
	COLORREF			m_crLabel;					// Label color.

	bool				m_bGroupSameChips;			// Group chips.
	StringToIntArray	m_textPosition;
	bool				m_bDrawLabelText;			// Label text visibility.
	float				m_fZoom;
	Size				m_szChipDefault;			// Chip size default.
	StringToIntArray	m_chipHorzAlignment;		// Chips horizontal alignment.
	int					m_nDrawChipPartCY;			// Partially draw chip when it is not on top and mose part of it is covered by another chip.

	bool				m_bShadow;					// Shadow.
	COLORREF			m_crShadow;					// Shadow color.
	Point				m_ptShadowPos;				// Shadow offset position.

	int					m_nDistanceCYCurrent;		// Current CY distance.
	int					m_nDrawChipPartCYCurrent;
	int					m_nWidthDrawn;				// Drawn width of chips.

private:
	Rect				m_rcLabel;
	Rect				m_rcChips;
	float				m_fLabelTextMaxZoom;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlChips();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};