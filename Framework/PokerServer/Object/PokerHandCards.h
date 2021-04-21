#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerHandCards window
#include "../../Protocol/_PokerDefines.h"
#include "../../Serialize/Serializable.h"

class PokerHandCards : public Serializable
{
public:
	PokerHandCards();
	~PokerHandCards();

public:
	char				m_btSeat;			// Seat index.
	char				m_btCard1;			// Hand card1.
	char				m_btCard2;			// Hand card2.
	char				m_btCard3;			// Hand card3.
	char				m_btCard4;			// Hand card4.
	char				m_cBestCardsClass;	// Best hand cards class.

	char				m_cBestCard1;		// Best 1 card index.
	char				m_cBestCard2;		// Best 2 card index.
	char				m_cBestCard3;		// Best 3 card index.
	char				m_cBestCard4;		// Best 4 card index.
	char				m_cBestCard5;		// Best 5 card index.


public:
	static int			GetCardIndex			(_String sCardName);
	static _String		GetCardNameByIndex		(int nCardIndex);

public:
	virtual Serializable* CreateSerializableObject(){return new PokerHandCards();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

};