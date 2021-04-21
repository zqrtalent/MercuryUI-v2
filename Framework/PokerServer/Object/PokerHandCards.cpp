// PokerHandCards.cpp : implementation file
//

#include "stdafx.h"
#include "PokerHandCards.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

_String
PokerHandCards::GetCardNameByIndex(int nCardIndex){
	const TCHAR* arrCardNum[] = {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	const TCHAR* arrCardType[] = {_T("S"), _T("C"), _T("D"), _T("H")};
	
	if( nCardIndex < 0 || nCardIndex > 52 )
		return _T("");

	int nLen1 = sizeof(arrCardType) / sizeof(TCHAR*), nLen2 = sizeof(arrCardNum) / sizeof(TCHAR*);

	_String sCardName;
	sCardName.Format(_T("%s%s"), arrCardNum[nCardIndex%nLen2], arrCardType[nCardIndex/nLen2]);
	return sCardName;
	}

int	
PokerHandCards::GetCardIndex(_String sCardName){
	if( sCardName.GetLength() < 2 )
		return -1;

	sCardName = sCardName.Left(2);
	sCardName.MakeUpper();
	const TCHAR* arrCardNum[] = {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	const TCHAR* arrCardType[] = {_T("S"), _T("C"), _T("D"), _T("H")};

	int nLoop = 0;
	int nLen1 = sizeof(arrCardType) / sizeof(TCHAR*);
	int nLen2 = sizeof(arrCardNum) / sizeof(TCHAR*);

	int		nCardIndex	= -1, nCardType	= 0;
	_String sSymbol		= sCardName.Right(1);

	while( nLoop < nLen1 ){
		if( sSymbol == arrCardType[nLoop] ){
			nCardType = nLoop;
			break;
			}
		nLoop ++;
		}

	nLoop		= 0;
	sSymbol		= sCardName.Left(1);
	while( nLoop < nLen2 ){
		if( sSymbol == arrCardNum[nLoop] ){
			nCardIndex = (nCardType*nLen2) + nLoop;
			return nCardIndex;
			}
		nLoop ++;
		}
	return -1;
	}