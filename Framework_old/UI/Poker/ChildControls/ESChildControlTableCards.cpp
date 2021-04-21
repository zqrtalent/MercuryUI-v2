// ESChildControlTableCards.cpp : implementation file
//

#include "ESChildControlTableCards.h"
#include "ESChildControlSeatHiddenCards.h"
#include "ESChildControlTable.h"

#include "../PokerGUI.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTableCards

ESChildControlTableCards::ESChildControlTableCards(){
	m_sClassName	= _T("ESChildControlTableCards");
	m_nCard1		= -1;
	m_nCard2		= -1;
	m_nCard3		= -1;
	m_nCard4		= -1;
	m_nCard5		= -1;

	m_nCard4Temp	= -1;
	m_nCard5Temp	= -1;

	m_nCard1OffsetX	= 0;
	m_nCard2OffsetX	= 0;
	m_nCard3OffsetX	= 0;
	m_nCard4OffsetX	= 0;
	m_nCard5OffsetX	= 0;

	m_bMarkCard1	= false;
	m_bMarkCard2	= false;
	m_bMarkCard3	= false;
	m_bMarkCard4	= false;
	m_bMarkCard5	= false;

	m_bCentred		= true;

	m_nDistanceCX	= 6;
	m_szCard.SetSize(50, 70);
	m_fZoom			= 1.0f;
	}

BEGIN_DECLARE_VAR(ESChildControlTableCards, ESChildControl)
DECLARE_VAR(_T("ESChildControlTableCards"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Card 1"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard1), false, false, false)
	DECLARE_VAR(_T("Card 2"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard2), false, false, false)
	DECLARE_VAR(_T("Card 3"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard3), false, false, false)
	DECLARE_VAR(_T("Card 4"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard4), false, false, false)
	DECLARE_VAR(_T("Card 5"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard5), false, false, false)
	DECLARE_VAR(_T("Card1 OffsetX"),			_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard1OffsetX), false, false, false)
	DECLARE_VAR(_T("Card2 OffsetX"),			_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard2OffsetX), false, false, false)
	DECLARE_VAR(_T("Card3 OffsetX"),			_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard3OffsetX), false, false, false)
	DECLARE_VAR(_T("Card4 OffsetX"),			_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard4OffsetX), false, false, false)
	DECLARE_VAR(_T("Card5 OffsetX"),			_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nCard5OffsetX), false, false, false)
	DECLARE_VAR(_T("Card1 Mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlTableCards,m_bMarkCard1), false, false, false)
	DECLARE_VAR(_T("Card2 Mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlTableCards,m_bMarkCard2), false, false, false)
	DECLARE_VAR(_T("Card3 Mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlTableCards,m_bMarkCard3), false, false, false)
	DECLARE_VAR(_T("Card4 Mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlTableCards,m_bMarkCard4), false, false, false)
	DECLARE_VAR(_T("Card5 Mark"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlTableCards,m_bMarkCard5), false, false, false)
	DECLARE_VAR(_T("Distance CX"),				_T(""), VariableType::VT_Int32, offsetof(ESChildControlTableCards,m_nDistanceCX), false, false, false)
	DECLARE_VAR(_T("Zoom Factor"),				_T(""), VariableType::VT_Float, offsetof(ESChildControlTableCards,m_fZoom), false, false, false)
END_DECLARE_VAR()

ESChildControlTableCards::~ESChildControlTableCards(){
	}

bool
ESChildControlTableCards::Create(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlTableCards::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlTableCards::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

void
ESChildControlTableCards::OnPaintClient(_DC* pDC, Rect* pRectDC, Rect* pRectClient){
	Rect		rcCardDC, rcInvalid;
	Rect		rcControlDC;
	rcControlDC.left		= (m_rRect.left - pRectClient->left) + pRectDC->left; 
	rcControlDC.top			= (m_rRect.top - pRectClient->top) + pRectDC->top; 
	rcControlDC.right		= rcControlDC.left + m_rRect.Width(); 
	rcControlDC.bottom		= rcControlDC.top + m_rRect.Height();

	float fZoomCX = m_fZoom, fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Image*		pImage		= NULL;
	Size szCard(m_szCard.cx*fZoomCX, m_szCard.cy*fZoomCY);
	int			nDistanceCX = m_nDistanceCX*fZoomCX;

	if( !ESChildControlTable::GetNearestCardDimension(szCard) ){
		//ASSERT(FALSE);
		return;
		}

	m_szCardCurrent = szCard;

	Rect rcCard;
	rcCard.left		= m_rRect.left + (m_bCentred ? (m_rRect.Width() - GetWidthCards(5))/2 : 0) /*Make cards in the middle of control*/;
	rcCard.top		= m_rRect.top;
	rcCard.right	= rcCard.left + szCard.cx;
	rcCard.bottom	= rcCard.top + szCard.cy;

	int			nLoop				= 0;
	bool		arrMarks[]			= {m_bMarkCard1, m_bMarkCard2, m_bMarkCard3, m_bMarkCard4, m_bMarkCard5};	
	int			nCardOffsets[]		= {m_nCard1OffsetX*fZoomCX, m_nCard2OffsetX*fZoomCX, m_nCard3OffsetX*fZoomCX, m_nCard4OffsetX*fZoomCX, m_nCard5OffsetX*fZoomCX};
	int			nArrCards[]			= {m_nCard1, m_nCard2, m_nCard3, m_nCard4, m_nCard5, -1};
	bool		bFourColoredCards	= PokerTableSettings::GetInstance()->m_b4ColoredCards;
	int			nCardBackIndex		= PokerTableSettings::GetInstance()->m_nCardsBackIndex;

	while( nArrCards[nLoop] != -1 ){
		if( arrMarks[nLoop] ){
			rcCard.top		+= 5;
			rcCard.bottom	+= 5;
			}

		rcCardDC.left		= (rcCard.left - pRectClient->left) + pRectDC->left + nCardOffsets[nLoop]; 
		rcCardDC.top		= (rcCard.top - pRectClient->top) + pRectDC->top; 
		rcCardDC.right		= rcCardDC.left + rcCard.Width(); 
		rcCardDC.bottom		= rcCardDC.top + rcCard.Height(); 

		rcInvalid = rcCardDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() ){
			if( nArrCards[nLoop] == -2 )
				ESChildControlTable::DrawCardBackByIndex(m_pOwner, pDC, pRectDC, &rcCardDC, szCard, nCardBackIndex, (arrMarks[nLoop] ? 122 : 255), false);
			else
				ESChildControlTable::DrawCardByIndex(m_pOwner, pDC, pRectDC, &rcCardDC, szCard, nArrCards[nLoop], bFourColoredCards, (arrMarks[nLoop] ? 122 : 255));
			}

		if( arrMarks[nLoop] ){
			rcCard.top		-= 5;
			rcCard.bottom	-= 5;
			}

		rcCard.left		+= (szCard.cx + nDistanceCX);
		rcCard.right	+= (szCard.cx + nDistanceCX);
		nLoop ++;
		}
/*
	// Soften filter.
	CBitmap bmpSoften;
	if( ImageHelper::GetBitmap32bppFromDC(bmpSoften, pDC, rcControlDC.left, rcControlDC.top, rcControlDC.Width(), rcControlDC.Height(), NULL) ){
		if( ImageHelper::SetSoftenFilter(bmpSoften) ){
			_DC memDC;
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&bmpSoften);
			pDC->BitBlt(rcControlDC.left, rcControlDC.top, rcControlDC.Width(), rcControlDC.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.DeleteDC();
			}
		bmpSoften.DeleteObject();
		}*/

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void
ESChildControlTableCards::CalcCardRects(){
	float		fZoomCX = m_fZoom;
	float		fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Rect rcCard;
	rcCard.left		= m_rRect.left + (m_bCentred ? (m_rRect.Width() - GetWidthCards(5))/2 : 0) /*Make cards in the middle of control*/;
//	rcCard.left		= m_rRect.left;
	rcCard.top		= m_rRect.top;
	rcCard.right	= rcCard.left + m_szCardCurrent.cx;
	rcCard.bottom	= rcCard.top + m_szCardCurrent.cy;

	m_rcCard1		= rcCard;
	m_rcCard1.left	= rcCard.left + /*m_nDistanceCX*fZoomCX +*/ m_nCard1OffsetX*fZoomCX;
	m_rcCard1.right	= m_rcCard1.left + m_szCardCurrent.cx;

	m_rcCard2		= rcCard;
	m_rcCard2.left	= m_rcCard1.right + m_nDistanceCX*fZoomCX + m_nCard2OffsetX*fZoomCX;
	m_rcCard2.right	= m_rcCard2.left + m_szCardCurrent.cx;

	m_rcCard3		= rcCard;
	m_rcCard3.left	= m_rcCard2.right + m_nDistanceCX*fZoomCX + m_nCard3OffsetX*fZoomCX;
	m_rcCard3.right	= m_rcCard3.left + m_szCardCurrent.cx;

	m_rcCard4		= rcCard;
	m_rcCard4.left	= m_rcCard3.right + m_nDistanceCX*fZoomCX + m_nCard4OffsetX*fZoomCX;
	m_rcCard4.right	= m_rcCard4.left + m_szCardCurrent.cx;

	m_rcCard5		= rcCard;
	m_rcCard5.left	= m_rcCard4.right + m_nDistanceCX*fZoomCX + m_nCard5OffsetX*fZoomCX;
	m_rcCard5.right	= m_rcCard5.left + m_szCardCurrent.cx;
	}

void
ESChildControlTableCards::ClearMarks(){
	m_bMarkCard1 = m_bMarkCard2	= m_bMarkCard3	= m_bMarkCard4	= m_bMarkCard5	= false;
	}

void
ESChildControlTableCards::SetMarkCards(bool bMarkCard1, bool bMarkCard2, bool bMarkCard3, bool bMarkCard4, bool bMarkCard5, bool bRedraw){
	if( m_bMarkCard1 != bMarkCard1 ||
		m_bMarkCard2 != bMarkCard2 ||
		m_bMarkCard3 != bMarkCard3 ||
		m_bMarkCard4 != bMarkCard4 ||
		m_bMarkCard5 != bMarkCard5){
		m_bMarkCard1	= bMarkCard1;
		m_bMarkCard2	= bMarkCard2;
		m_bMarkCard3	= bMarkCard3;
		m_bMarkCard4	= bMarkCard4;
		m_bMarkCard5	= bMarkCard5;
		if( bRedraw )
			Redraw(true);
		}
	}

int
ESChildControlTableCards::GetWidthCards(int nCardsCt){
	float		fZoomCX = m_fZoom;
	float		fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	int nWidthCard1	= m_szCardCurrent.cx  + m_nCard1OffsetX*fZoomCX;
	int nWidthCard2	= nWidthCard1 + m_szCardCurrent.cx + m_nDistanceCX*fZoomCX + m_nCard2OffsetX*fZoomCX;
	int nWidthCard3	= nWidthCard2 + m_szCardCurrent.cx + m_nDistanceCX*fZoomCX + m_nCard3OffsetX*fZoomCX;
	int nWidthCard4	= nWidthCard3 + m_szCardCurrent.cx +  m_nDistanceCX*fZoomCX + m_nCard4OffsetX*fZoomCX;
	int nWidthCard5	= nWidthCard4 + m_szCardCurrent.cx + m_nDistanceCX*fZoomCX + m_nCard5OffsetX*fZoomCX;

	int nArrWidth[] = {nWidthCard1, nWidthCard2, nWidthCard3, nWidthCard4, nWidthCard5};
	return nArrWidth[nCardsCt - 1];
	}

Rect
ESChildControlTableCards::GetCardRect(int nCardIndex){
	CalcCardRects();
	Rect* rcArrCards[] = {&m_rcCard1, &m_rcCard2, &m_rcCard3, &m_rcCard4, &m_rcCard5};
	return *rcArrCards[nCardIndex];
	}

void
ESChildControlTableCards::OnMotionStart(DWORD_PTR dwParam){
	m_nCard1OffsetXSave	= m_nCard1OffsetX;
	m_nCard2OffsetXSave	= m_nCard2OffsetX;
	m_nCard3OffsetXSave = m_nCard3OffsetX;
	m_nCard4OffsetXSave = m_nCard4OffsetX;
	m_nCard5OffsetXSave = m_nCard5OffsetX;
	}

void
ESChildControlTableCards::OnMotionProgress(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY){
	Rect rcOwner;
	m_pOwner->GetClientRectMy(rcOwner);

	float fZoomCX = m_fZoom, fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Size	szCard		= m_szCardCurrent;
	int		nDistanceCX = m_nDistanceCX*fZoomCX;
	CalcCardRects();

	// Flop cards animation.
	if( m_nCard1 != -1 && m_nCard2 != -1 && m_nCard3 != -1 && m_nCard4 == -1 && m_nCard4Temp == -1 && m_nCard5Temp == -1 ){
		int		nCardsWidth		= (rcOwner.Width()*fPosX);
		float	fDistanceEachCX	= (nCardsWidth - m_rcCard1.Width()) / 2.0f;
		m_nCard2OffsetX			= (m_rcCard1.left + fDistanceEachCX) - (m_rcCard1.right + nDistanceCX);
		m_nCard3OffsetX			= (m_rcCard1.left + 2*fDistanceEachCX) - (m_rcCard1.right + szCard.cx + 2*nDistanceCX);
	
		m_nCard2OffsetX			= (int)((float)m_nCard2OffsetX / fZoomCX);
		m_nCard3OffsetX			= (int)((float)m_nCard3OffsetX / fZoomCX);

		Rect rcRedraw			= m_rRect;
		m_pOwner->RedrawRect(rcRedraw, 1);
		}
	else{
		// 4th card animation.
		if( (m_nCard4 != -1  || m_nCard4Temp != -1) && m_nCard5 == -1 && m_nCard5Temp == -1 ){
			ESChildControlSeatHiddenCards*	pDeck = (ESChildControlSeatHiddenCards*)m_pOwner->GetChildControlByName(_T("_deck"));
			Rect							rcRedraw		= pDeck->GetClientRect();
			Rect							rRectNoChange	= rcRedraw;
			if( nFrame == 1 ){
				m_nCard4Temp	= m_nCard4;
				m_nCard4		= -1;
				pDeck->SetSizeAutoCalculation	(true);
				pDeck->SetVisible				(true, false);
				m_fDeckPosXSave	= pDeck->GetLogicalPosX();
				m_fDeckPosYSave	= pDeck->GetLogicalPosY();
				}
			else{
				if( nFrame == nFrameCt ){
					rRectNoChange.SetRect(0, 0, 0, 0);
					m_nCard4		= m_nCard4Temp;
					m_nCard4Temp	= -1;
					pDeck->SetVisible				(false, false);
					pDeck->SetLogicalPos			(m_fDeckPosXSave, m_fDeckPosYSave);
					pDeck->OwnerWindowSizeChanged	(rcOwner.Width(), rcOwner.Height());
					m_pOwner->RedrawRect			(m_rcCard4, 1);
					}
				else{
					pDeck->SetLogicalPos			(fPosX, fPosY);
					pDeck->OwnerWindowSizeChanged	(rcOwner.Width(), rcOwner.Height());
					rcRedraw |= pDeck->GetClientRect();
					}
				}

			if( rcRedraw != rRectNoChange )
				m_pOwner->RedrawRect(rcRedraw, 1);
			}
		// 5th card animation.
		else{
			ESChildControlSeatHiddenCards*	pDeck = (ESChildControlSeatHiddenCards*)m_pOwner->GetChildControlByName(_T("_deck"));
			Rect							rcRedraw		= pDeck->GetClientRect();
			Rect							rRectNoChange	= rcRedraw;

			if( nFrame == 1 ){
				m_nCard5Temp	= m_nCard5;
				m_nCard5		= -1;
				pDeck->SetSizeAutoCalculation	(true);
				pDeck->SetVisible				(true, false);
				m_fDeckPosXSave	= pDeck->GetLogicalPosX();
				m_fDeckPosYSave	= pDeck->GetLogicalPosY();
				}
			else{
				if( nFrame == nFrameCt ){
					rRectNoChange.SetRect(0, 0, 0, 0);
					m_nCard5		= m_nCard5Temp;
					m_nCard5Temp	= -1;
					pDeck->SetVisible				(false, false);
					pDeck->SetLogicalPos			(m_fDeckPosXSave, m_fDeckPosYSave);
					pDeck->OwnerWindowSizeChanged	(rcOwner.Width(), rcOwner.Height());
					m_pOwner->RedrawRect(m_rcCard5, 1);
					}
				else{
					pDeck->SetLogicalPos			(fPosX, fPosY);
					pDeck->OwnerWindowSizeChanged	(rcOwner.Width(), rcOwner.Height());
					rcRedraw |= pDeck->GetClientRect();
					}
				}

			if( rcRedraw != rRectNoChange )
				m_pOwner->RedrawRect(rcRedraw, 1);
			}
		}
	}

void
ESChildControlTableCards::OnMotionEnd(){
	m_nCard1OffsetX	= m_nCard1OffsetXSave;
	m_nCard2OffsetX	= m_nCard2OffsetXSave;
	m_nCard3OffsetX = m_nCard3OffsetXSave;
	m_nCard4OffsetX = m_nCard4OffsetXSave;
	m_nCard5OffsetX = m_nCard5OffsetXSave;
	}

void
ESChildControlTableCards::OnMotionStopped(){
	m_nCard1OffsetX	= m_nCard1OffsetXSave;
	m_nCard2OffsetX	= m_nCard2OffsetXSave;
	m_nCard3OffsetX = m_nCard3OffsetXSave;
	m_nCard4OffsetX = m_nCard4OffsetXSave;
	m_nCard5OffsetX = m_nCard5OffsetXSave;
	}