// SeatPanelInfo.h : header file
//

#pragma once
#include "..\..\Serialize\Serializable.h"

enum PositionDef
{
	Left, Right, Top, Bottom
};

enum ChipGrowDirection
{
	LeftToRight = 0,
	RightToLeft
};

class SeatPanelInfo : public Serializable
{
public:
	SeatPanelInfo();
	~SeatPanelInfo();

public:
	CPoint				m_ptPos;				// Seat panel position.
	CPoint				m_ptChipPos;			// Placed chip position in seat panel coordinates.
	CPoint				m_ptDealerButtonPos;	// Dealer button position.

	CRect				m_rcSeatLabel;			// Seat label rect.
	CRect				m_rcSeatInfoLabel;		// Seat info label rect.
	CRect				m_rcChips;				// Chips rect.

	PositionDef			m_chipLabelTextPos;		// Chip label text position.
	ChipGrowDirection	m_chipGrowDir;			// Chip grow direction

	CPoint				m_ptOpenCard1;			// Open card1 position.
	CPoint				m_ptOpenCard2;			// Open card2 position.
	CPoint				m_ptOpenCard3;			// Open card3 position.
	CPoint				m_ptOpenCard4;			// Open card4 position.

	CPoint				m_ptHiddenCard1;		// Hidden card1 position.
	CPoint				m_ptHiddenCard2;		// Hidden card2 position.
	CPoint				m_ptHiddenCard3;		// Hidden card3 position.
	CPoint				m_ptHiddenCard4;		// Hidden card4 position.

protected: // Serialization.
	virtual Serializable* CreateSerializableObject(){return new SeatPanelInfo();};
	INIT_RUNTIME_VARIABLE()
};