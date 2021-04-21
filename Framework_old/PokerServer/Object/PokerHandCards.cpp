// PokerHandCards.cpp : implementation file
//

#include "PokerHandCards.h"

PokerHandCards::PokerHandCards(){
	m_btSeat			= -1;
	m_btCard1			= -1;
	m_btCard2			= -1;
	m_btCard3			= -1;
	m_btCard4			= -1;
	m_cBestCard1		= m_cBestCard2 = m_cBestCard3 = m_cBestCard4 = m_cBestCard5 = -1;
	m_cBestCardsClass	= 0;
	}

BEGIN_DECLARE_VAR(PokerHandCards, Serializable)
	DECLARE_VAR(_T("PokerHandCards"),	_T(""),		VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Seat"),				_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_btSeat), false, false, false)
	DECLARE_VAR(_T("Card1"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_btCard1), false, false, false)
	DECLARE_VAR(_T("Card2"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_btCard2), false, false, false)
	DECLARE_VAR(_T("Card3"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_btCard3), false, false, false)
	DECLARE_VAR(_T("Card4"),			_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_btCard4), false, false, false)
	DECLARE_VAR(_T("BestCardsClass"),	_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_cBestCardsClass), false, false, false)

	DECLARE_VAR(_T("cBestCard1"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_cBestCard1), false, false, false)
	DECLARE_VAR(_T("cBestCard2"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_cBestCard2), false, false, false)
	DECLARE_VAR(_T("cBestCard3"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_cBestCard3), false, false, false)
	DECLARE_VAR(_T("cBestCard4"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_cBestCard4), false, false, false)
	DECLARE_VAR(_T("cBestCard5"),		_T(""),		VariableType::VT_Int8, offsetof(PokerHandCards,m_cBestCard5), false, false, false)
END_DECLARE_VAR()

PokerHandCards::~PokerHandCards(){
	}