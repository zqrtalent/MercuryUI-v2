// ESChildControlSeatCards.cpp : implementation file
//

#include "stdafx.h"
#include "ESChildControlSeatCards.h"
#include "ESChildControlTable.h"

#include "../PokerGUI.h"

#ifdef _DEBUGW
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeatCards

ESChildControlSeatCards::ESChildControlSeatCards(){
	m_sClassName		= _T("ESChildControlSeatCards");
	m_ptCard1Offset.SetPoint(4, 4);
	m_ptCard2Offset.SetPoint(4, 4);
	m_ptCard3Offset.SetPoint(4, 4);
	m_ptCard4Offset.SetPoint(4, 4);
	m_nCard1			= m_nCard2 = m_nCard3 = m_nCard4 = -1;
	m_bMark1			= m_bMark2 = m_bMark3 = m_bMark4 = false;
	m_fZoom				= 1.0f;
	m_szCard.SetSize(50, 70);
	m_bHideCards		= false;
	m_bUseCardOffsets	= false;
	m_bCardsCentered	= false;
	m_nCardsCtMax		= 4;
	}

BEGIN_DECLARE_VAR(ESChildControlSeatCards, ESChildControl)
DECLARE_VAR(_T("ESChildControlSeatCards"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Card 1"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlSeatCards,m_nCard1), false, false, false)
	DECLARE_VAR(_T("Card 1 Offset"),			_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatCards,m_ptCard1Offset), false, false, false)
	DECLARE_VAR(_T("Card 1 mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatCards,m_bMark1), false, false, false)
	DECLARE_VAR(_T("Card 2"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlSeatCards,m_nCard2), false, false, false)
	DECLARE_VAR(_T("Card 2 Offset"),			_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatCards,m_ptCard2Offset), false, false, false)
	DECLARE_VAR(_T("Card 2 mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatCards,m_bMark2), false, false, false)
	DECLARE_VAR(_T("Card 3"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlSeatCards,m_nCard3), false, false, false)
	DECLARE_VAR(_T("Card 3 Offset"),			_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatCards,m_ptCard3Offset), false, false, false)
	DECLARE_VAR(_T("Card 3 mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatCards,m_bMark3), false, false, false)
	DECLARE_VAR(_T("Card 4"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlSeatCards,m_nCard4), false, false, false)
	DECLARE_VAR(_T("Card 4 Offset"),			_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatCards,m_ptCard4Offset), false, false, false)
	DECLARE_VAR(_T("Card 4 mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatCards,m_bMark4), false, false, false)
	DECLARE_VAR(_T("Card default size"),		_T(""), VariableType::VT_Size, offsetof(ESChildControlSeatCards,m_szCard), false, false, false)
	DECLARE_VAR(_T("Zoom Factor"),				_T(""), VariableType::VT_Float, offsetof(ESChildControlSeatCards,m_fZoom), false, false, false)
	DECLARE_VAR(_T("UseCardOffsets"),			_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatCards,m_bUseCardOffsets), false, false, false)
	DECLARE_VAR(_T("CardsCentered"),			_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatCards,m_bCardsCentered), false, false, false)
END_DECLARE_VAR()

ESChildControlSeatCards::~ESChildControlSeatCards(){
	}

bool
ESChildControlSeatCards::Create(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlSeatCards::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlSeatCards::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

int
ESChildControlSeatCards::GetSeatIndex(){
	int nSeat = atoi(m_sName.Right(1).GetBuffer());
	if( !nSeat ){
		nSeat = atoi(m_sName.Right(2).GetBuffer());
		if( !nSeat )
			return -1;
		}

	if( nSeat > 0 )
		return (nSeat - 1);
	return -1;
	}

void
ESChildControlSeatCards::SetCards(int nCard1, int nCard2, int nCard3, int nCard4, bool bRedraw){
	m_nCard1 = nCard1;
	m_nCard2 = nCard2;
	m_nCard3 = nCard3;
	m_nCard4 = nCard4;
	if( bRedraw )
		Redraw();
	}

void
ESChildControlSeatCards::SetCardsMark(bool bMark1, bool bMark2, bool bMark3, bool bMark4, bool bRedraw){
	if( m_bMark1 != bMark1 ||
		m_bMark2 != bMark2 ||
		m_bMark3 != bMark3 ||
		m_bMark4 != bMark4 ){
		m_bMark1 = bMark1;
		m_bMark2 = bMark2;
		m_bMark3 = bMark3;
		m_bMark4 = bMark4;
		if( bRedraw )
			Redraw();
		}
	}

_Point
ESChildControlSeatCards::GetCardPointByIndex(int nCardIndex){
	if( nCardIndex < 0 || nCardIndex > 3 )
		return _Point(0, 0);

	float fZoomCX = m_fZoom;
	float fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Image*		pImage		= NULL;
	_Size szCard(m_szCard.cx*fZoomCX, m_szCard.cy*fZoomCY);
	CardsGroup* pGroup = NULL;
	int			nCardSuitWidth = 0;
	if( !ESChildControlTable::GetNearestCardDimension(szCard, &pGroup, &nCardSuitWidth) )
		ASSERT(FALSE);

	_Rect rcCard, rcCardsContainer, rcRect = GetClientRect();
	rcCard.left					= rcRect.left;
	rcCard.top					= rcRect.top;
	rcCard.right				= rcCard.left + szCard.cx;
	rcCard.bottom				= rcCard.top + szCard.cy;

	POINT	ptArrCardOffsets[]	= {m_ptCard1Offset, m_ptCard2Offset, m_ptCard3Offset, m_ptCard4Offset};
	POINT	ptArrCalcCardOffsets[4];
	int		nLoop = 0, nCardsCt = m_nCardsCtMax;

	for(int i=0; i<nCardsCt; i++){
		int nOffsetX	= ptArrCardOffsets[nLoop].x*fZoomCX;
		int nOffsetY	= ptArrCardOffsets[nLoop].y*fZoomCY;
		// Make card suit visible;
		if( !m_bUseCardOffsets )
			nOffsetX	= nCardSuitWidth + 3*fZoomCX;

		rcCard.left		+= nOffsetX;
		rcCard.right	+= nOffsetX;
		rcCard.top		+= nOffsetY;
		rcCard.bottom	+= nOffsetY;

		ptArrCalcCardOffsets[i].x = rcCard.left;
		ptArrCalcCardOffsets[i].y = rcCard.top;

		if( m_bUseCardOffsets ){
			rcCard.left		+= szCard.cx;
			rcCard.right	+= szCard.cx;
			}
		}

	if( m_bCardsCentered ){
		rcCardsContainer.SetRect(ptArrCalcCardOffsets[0].x, ptArrCalcCardOffsets[0].y, ptArrCalcCardOffsets[nCardsCt-1].x + szCard.cx, ptArrCalcCardOffsets[nCardsCt-1].y + szCard.cy);
		if( rcCardsContainer.Width() > rcRect.Width() ){
			int nPartCX = ((nCardsCt > 1) ? (rcRect.Width() - szCard.cx)/(nCardsCt-1) : 0);
			ptArrCalcCardOffsets[0].x = rcCardsContainer.left;
			for(int i=1; i<nCardsCt; i++)
				ptArrCalcCardOffsets[i].x = ptArrCalcCardOffsets[i-1].x + nPartCX;
			}
		else{
			int x = (rcRect.Width() - rcCardsContainer.Width())/2 - (ptArrCalcCardOffsets[0].x - rcRect.left);
			for(int i=0; i<nCardsCt; i++)
				ptArrCalcCardOffsets[i].x += x;
			}
		}

	do
		{
		rcCard.left		= ptArrCalcCardOffsets[nLoop].x;
		rcCard.right	= rcCard.left + szCard.cx;
		rcCard.top		= ptArrCalcCardOffsets[nLoop].y;
		rcCard.bottom	= rcCard.top + szCard.cy;
		if( nLoop >= nCardIndex ) break;
		nLoop ++;
		}
	while(TRUE);


	return _Point(rcCard.left, rcCard.top);
	}

_Point
ESChildControlSeatCards::GetCardCenterByIndex(int nCardIndex){
	if( nCardIndex < 0 || nCardIndex > 3 )
		return _Point(0, 0);

	float fZoomCX = m_fZoom;
	float fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Image*		pImage		= NULL;
	_Size szCard(m_szCard.cx*fZoomCX, m_szCard.cy*fZoomCY);
	CardsGroup* pGroup = NULL;
	int			nCardSuitWidth = 0;
	if( !ESChildControlTable::GetNearestCardDimension(szCard, &pGroup, &nCardSuitWidth) )
		ASSERT(FALSE);

	_Rect rcCard, rcCardsContainer, rcRect = GetClientRect();
	rcCard.left					= m_rcRect.left;
	rcCard.top					= m_rcRect.top;
	rcCard.right				= rcCard.left + szCard.cx;
	rcCard.bottom				= rcCard.top + szCard.cy;

	POINT	ptArrCardOffsets[]	= {m_ptCard1Offset, m_ptCard2Offset, m_ptCard3Offset, m_ptCard4Offset};
	POINT	ptArrCalcCardOffsets[4];
	int		nLoop = 0, nCardsCt = m_nCardsCtMax;

	for(int i=0; i<nCardsCt; i++){
		int nOffsetX	= ptArrCardOffsets[nLoop].x*fZoomCX;
		int nOffsetY	= ptArrCardOffsets[nLoop].y*fZoomCY;
		// Make card suit visible;
		if( !m_bUseCardOffsets )
			nOffsetX	= nCardSuitWidth + 3*fZoomCX;

		rcCard.left		+= nOffsetX;
		rcCard.right	+= nOffsetX;
		rcCard.top		+= nOffsetY;
		rcCard.bottom	+= nOffsetY;

		ptArrCalcCardOffsets[i].x = rcCard.left;
		ptArrCalcCardOffsets[i].y = rcCard.top;

		if( m_bUseCardOffsets ){
			rcCard.left		+= szCard.cx;
			rcCard.right	+= szCard.cx;
			}
		}

	if( m_bCardsCentered ){
		rcCardsContainer.SetRect(ptArrCalcCardOffsets[0].x, ptArrCalcCardOffsets[0].y, ptArrCalcCardOffsets[nCardsCt-1].x + szCard.cx, ptArrCalcCardOffsets[nCardsCt-1].y + szCard.cy);
		if( rcCardsContainer.Width() > m_rcRect.Width() ){
			int nPartCX = ((nCardsCt > 1) ? (rcRect.Width() - szCard.cx)/(nCardsCt-1) : 0);
			ptArrCalcCardOffsets[0].x = rcCardsContainer.left;
			for(int i=1; i<nCardsCt; i++)
				ptArrCalcCardOffsets[i].x = ptArrCalcCardOffsets[i-1].x + nPartCX;
			}
		else{
			int x = (rcRect.Width() - rcCardsContainer.Width())/2 - (ptArrCalcCardOffsets[0].x - rcRect.left);
			for(int i=0; i<nCardsCt; i++)
				ptArrCalcCardOffsets[i].x += x;
			}
		}

	do
		{
		rcCard.left		= ptArrCalcCardOffsets[nLoop].x;
		rcCard.right	= rcCard.left + szCard.cx;
		rcCard.top		= ptArrCalcCardOffsets[nLoop].y;
		rcCard.bottom	= rcCard.top + szCard.cy;
		if( nLoop >= nCardIndex ) break;
		nLoop ++;
		}
	while(TRUE);

	return _Point(rcCard.left + rcCard.Width()/2, rcCard.top + rcCard.Height()/2);
	}

void
ESChildControlSeatCards::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcCardDC, rcInvalid;
	_Rect		rcControlDC	= ClientRectToDCRect(pRectDC, pRectClient, &m_rcRect);
	
	float fZoomCX = m_fZoom, fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Image*		pImage		= NULL;
	_Size szCard(m_szCard.cx*fZoomCX, m_szCard.cy*fZoomCY);
	CardsGroup* pGroup = NULL;
	int			nCardSuitWidth = 0;
	if( !ESChildControlTable::GetNearestCardDimension(szCard, &pGroup, &nCardSuitWidth) )
		ASSERT(FALSE);

	_Rect rcCard, rcCardsContainer, rcRect = GetClientRect();
	rcCard.left					= rcRect.left;
	rcCard.top					= rcRect.top;
	rcCard.right				= rcCard.left + szCard.cx;
	rcCard.bottom				= rcCard.top + szCard.cy;

	POINT	ptArrCardOffsets[]	= {m_ptCard1Offset, m_ptCard2Offset, m_ptCard3Offset, m_ptCard4Offset};
	POINT	ptArrCalcCardOffsets[4];
	int		nLoop				= 0;
	int		nCardsCt			= m_nCardsCtMax;

	for(int i=0; i<nCardsCt; i++){
		int nOffsetX	= ptArrCardOffsets[nLoop].x*fZoomCX;
		int nOffsetY	= ptArrCardOffsets[nLoop].y*fZoomCY;
		// Make card suit visible;
		if( !m_bUseCardOffsets )
			nOffsetX	= nCardSuitWidth + 3*fZoomCX;

		rcCard.left		+= nOffsetX;
		rcCard.right	+= nOffsetX;
		rcCard.top		+= nOffsetY;
		rcCard.bottom	+= nOffsetY;

		ptArrCalcCardOffsets[i].x = rcCard.left;
		ptArrCalcCardOffsets[i].y = rcCard.top;

		if( m_bUseCardOffsets ){
			rcCard.left		+= szCard.cx;
			rcCard.right	+= szCard.cx;
			}
		}

	if( m_bCardsCentered ){
		rcCardsContainer.SetRect(ptArrCalcCardOffsets[0].x, ptArrCalcCardOffsets[0].y, ptArrCalcCardOffsets[nCardsCt-1].x + szCard.cx, ptArrCalcCardOffsets[nCardsCt-1].y + szCard.cy);
		if( rcCardsContainer.Width() > rcRect.Width() ){
			int nPartCX = ((nCardsCt > 1) ? (rcRect.Width() - szCard.cx)/(nCardsCt-1) : 0);
			ptArrCalcCardOffsets[0].x = rcCardsContainer.left;
			for(int i=1; i<nCardsCt; i++)
				ptArrCalcCardOffsets[i].x = ptArrCalcCardOffsets[i-1].x + nPartCX;
			}
		else{
			int x = (rcRect.Width() - rcCardsContainer.Width())/2 - (ptArrCalcCardOffsets[0].x - rcRect.left);
			for(int i=0; i<nCardsCt; i++)
				ptArrCalcCardOffsets[i].x += x;
			}
		}

	nLoop					= 0;
	bool		arrMarks[]	= {m_bMark1, m_bMark2, m_bMark3, m_bMark4};	
	int			nArrCards[]	= {m_nCard1, m_nCard2, m_nCard3, m_nCard4, -1};
	/*
	// Correct card offsets to make card suit and rank visible.
	if( pGroup && !pGroup->m_imgCardSuit1.IsNull() ){
		_Size szSuit(pGroup->m_imgCardSuit1.GetWidth(), pGroup->m_imgCardSuit1.GetHeight());
		ptArrCardOffsets[1].x = -1*( szCard.cx - szSuit.cx);
		ptArrCardOffsets[2].x = ptArrCardOffsets[1].x;
		ptArrCardOffsets[3].x = ptArrCardOffsets[1].x;
		}*/

	PokerTableSettings* pSettings			= PokerTableSettings::GetInstance();
	bool				bFourColoredCards	= pSettings->m_b4ColoredCards;
	int					nCardsBackIndex		= pSettings->m_nCardsBackIndex;
	int					nOffsetCYWhenMark	= (int)(5*fZoomCY);
	while( nArrCards[nLoop] != -1 ){
		rcCard.left		= ptArrCalcCardOffsets[nLoop].x;
		rcCard.right	= rcCard.left + szCard.cx;
		rcCard.top		= ptArrCalcCardOffsets[nLoop].y;
		rcCard.bottom	= rcCard.top + szCard.cy;

		if( arrMarks[nLoop] ){
			rcCard.top		+= nOffsetCYWhenMark;
			rcCard.bottom	+= nOffsetCYWhenMark;
			}
		
		rcCardDC	= ClientRectToDCRect(pRectDC, pRectClient, &rcCard);
		rcInvalid	= rcCardDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() ){	
			if( m_bHideCards || nArrCards[nLoop] == -2 )
				ESChildControlTable::DrawCardBackByIndex(m_pOwner, pDC, pRectDC, &rcCardDC, szCard, nCardsBackIndex, (arrMarks[nLoop] ? 122 : 255), false);
			else
				ESChildControlTable::DrawCardByIndex(m_pOwner, pDC, pRectDC, &rcCardDC, szCard, nArrCards[nLoop], bFourColoredCards, (arrMarks[nLoop] ? 122 : 255));
			}

		if( arrMarks[nLoop] ){
			rcCard.top		-= nOffsetCYWhenMark;
			rcCard.bottom	-= nOffsetCYWhenMark;
			}
		nLoop ++;
		}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}