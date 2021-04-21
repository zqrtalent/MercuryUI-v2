// ESChildControlSeatHiddenCards.cpp : implementation file
//

#include "ESChildControlSeatHiddenCards.h"
#include "ESChildControlSeatCards.h"
#include "ESChildControlTable.h"
//#include "../PokerTableWnd.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeatHiddenCards

ESChildControlSeatHiddenCards::ESChildControlSeatHiddenCards(){
	m_sClassName	= _T("ESChildControlSeatHiddenCards");
	m_ptCard1Offset.SetPoint(4, 4);
	m_ptCard2Offset.SetPoint(4, 4);
	m_ptCard3Offset.SetPoint(4, 4);
	m_ptCard4Offset.SetPoint(4, 4);
	
	m_bCard1		= true;
	m_bCard2		= true;
	m_bCard3		= false;
	m_bCard4		= false;

	m_nAlphaConstant	= 255;
	m_btAnimationAlpha	= 255;

	m_fZoom			= 1.0f;
	m_szCardBack.SetSize(20, 28);
	m_nBackIndex	= 7;

	m_nSeatIndex	= -1;
	m_nCardIndex	= -1;
	m_nCardValue	= -1;
	}

BEGIN_DECLARE_VAR(ESChildControlSeatHiddenCards, ESChildControl)
DECLARE_VAR(_T("ESChildControlSeatHiddenCards"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Card 1"),							_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatHiddenCards,m_bCard1), false, false, false)
	DECLARE_VAR(_T("Card 1 Offset"),					_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatHiddenCards,m_ptCard1Offset), false, false, false)
	DECLARE_VAR(_T("Card 2"),							_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatHiddenCards,m_bCard2), false, false, false)
	DECLARE_VAR(_T("Card 2 Offset"),					_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatHiddenCards,m_ptCard2Offset), false, false, false)
	DECLARE_VAR(_T("Card 3"),							_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatHiddenCards,m_bCard3), false, false, false)
	DECLARE_VAR(_T("Card 3 Offset"),					_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatHiddenCards,m_ptCard3Offset), false, false, false)
	DECLARE_VAR(_T("Card 4"),							_T(""), VariableType::VT_Bool, offsetof(ESChildControlSeatHiddenCards,m_bCard4), false, false, false)
	DECLARE_VAR(_T("Card 4 Offset"),					_T(""), VariableType::VT_Point, offsetof(ESChildControlSeatHiddenCards,m_ptCard4Offset), false, false, false)
	DECLARE_VAR(_T("Back image index"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlSeatHiddenCards,m_nBackIndex), false, false, false)
	DECLARE_VAR(_T("Card default size"),				_T(""), VariableType::VT_Size, offsetof(ESChildControlSeatHiddenCards,m_szCardBack), false, false, false)
	DECLARE_VAR(_T("Zoom Factor"),						_T(""), VariableType::VT_Float, offsetof(ESChildControlSeatHiddenCards,m_fZoom), false, false, false)
END_DECLARE_VAR()

ESChildControlSeatHiddenCards::~ESChildControlSeatHiddenCards(){
	}

bool
ESChildControlSeatHiddenCards::Create(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControl::Create(nId, rRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlSeatHiddenCards::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlSeatHiddenCards::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

void
ESChildControlSeatHiddenCards::SetupForAnimation(int nSeat, int nCardIndex, int nCardValue){
	m_nSeatIndex	= nSeat;
	m_nCardIndex	= nCardIndex;
	m_nCardValue	= nCardValue;
	}

void
ESChildControlSeatHiddenCards::SetCards(bool bCard1, bool bCard2, bool bCard3, bool bCard4, bool bRedraw){
	if( m_bCard1 == bCard1 &&  
		m_bCard2 == bCard2 &&  
		m_bCard3 == bCard3 &&  
		m_bCard4 == bCard4 ){
		return;
		}

	m_bCard1	= bCard1;
	m_bCard2	= bCard2;
	m_bCard3	= bCard3;
	m_bCard4	= bCard4;
	if( bRedraw )
		Redraw(true);
	}

int
ESChildControlSeatHiddenCards::GetCardOffsetX(int nCardIndex){
	if( nCardIndex < 0 || nCardIndex > 3 ) return 0;
	POINT		ptArrCardOffsets[]	= {m_ptCard1Offset, m_ptCard2Offset, m_ptCard3Offset, m_ptCard4Offset};
	int			nLoop = 0, nOffset = 0;
	while(nLoop <= nCardIndex){
		int nOffsetX		= ptArrCardOffsets[nLoop].x;
		if( !nLoop )
			nOffset			+= (nOffsetX);
		else
			nOffset			+= (m_szCardBack.cx + nOffsetX);
		nLoop				++;
		}
	return nOffset;
	}

int
ESChildControlSeatHiddenCards::GetCardOffsetY(int nCardIndex){
	if( nCardIndex < 0 || nCardIndex > 3 ) return 0;
	POINT		ptArrCardOffsets[]	= {m_ptCard1Offset, m_ptCard2Offset, m_ptCard3Offset, m_ptCard4Offset};
	int			nLoop = 0, nOffset = 0;
	while(nLoop <= nCardIndex){
		int nOffsetY		= ptArrCardOffsets[nLoop].y;
		nOffset				+= (nOffsetY);
		nLoop				++;
		}
	return nOffset;
	}

Point
ESChildControlSeatHiddenCards::GetCardPointByIndex(int nCardIndex){
	if( nCardIndex < 0 || nCardIndex > 3 ) 
		return Point(m_rRect.left, m_rRect.top);

	float fZoomCX = m_fZoom, fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);
	Point	ptCard;
	ptCard.x = m_rRect.left + GetCardOffsetX(nCardIndex)*fZoomCX;
	ptCard.y = m_rRect.top + GetCardOffsetY(nCardIndex)*fZoomCY;
	return ptCard;
	}

void
ESChildControlSeatHiddenCards::SetCardBackIndex(int nBackIndex, bool bRedraw){
	if( m_nBackIndex = nBackIndex )
		return;
	m_nBackIndex = nBackIndex;
	if( bRedraw )
		Redraw(true);
	}

void
ESChildControlSeatHiddenCards::OnPaintClient(_DC* pDC, Rect* pRectDC, Rect* pRectClient){
	Rect		rcCardDC, rcInvalid;
	Rect		rcControlDC;
	rcControlDC.left		= (m_rRect.left - pRectClient->left) + pRectDC->left; 
	rcControlDC.top			= (m_rRect.top - pRectClient->top) + pRectDC->top; 
	rcControlDC.right		= rcControlDC.left + m_rRect.Width(); 
	rcControlDC.bottom		= rcControlDC.top + m_rRect.Height();

	float fZoomCX = m_fZoom, fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Image*		pImage		= NULL;
	Size szCard(m_szCardBack.cx*fZoomCX, m_szCardBack.cy*fZoomCY);
	
	Rect rcCard;
	rcCard.left		= m_rRect.left;
	rcCard.top		= m_rRect.top;
	rcCard.right	= rcCard.left + szCard.cx;
	rcCard.bottom	= rcCard.top + szCard.cy;

	int			nLoop				= 0;
	POINT		ptArrCardOffsets[]	= {m_ptCard1Offset, m_ptCard2Offset, m_ptCard3Offset, m_ptCard4Offset};
	bool		bArrCards[]			= {m_bCard1, m_bCard2, m_bCard3, m_bCard4, false};

	while( bArrCards[nLoop] ){
		int nOffsetX		= ptArrCardOffsets[nLoop].x*fZoomCX;
		int nOffsetY		= ptArrCardOffsets[nLoop].y*fZoomCY;

		rcCardDC.left		= (rcCard.left - pRectClient->left) + pRectDC->left + nOffsetX; 
		rcCardDC.top		= (rcCard.top - pRectClient->top) + pRectDC->top + nOffsetY; 
		rcCardDC.right		= rcCardDC.left + rcCard.Width(); 
		rcCardDC.bottom		= rcCardDC.top + rcCard.Height(); 

		rcInvalid = rcCardDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() ){
	//		ESChildControlTable::DrawCardBackByIndex(m_pOwner, pDC, pRectDC, &rcCardDC, szCard, m_nBackIndex, m_nAlphaConstant);
			// Use the smallest back image.
			ESChildControlTable::DrawCardBackByIndex(m_pOwner, pDC, pRectDC, &rcCardDC, m_szCardBack, m_nBackIndex, m_nAlphaConstant, true);
			}
		
		rcCard.left		+= (szCard.cx + nOffsetX);
		rcCard.right	+= (szCard.cx + nOffsetX);
		rcCard.top		+= nOffsetY;
		rcCard.bottom	+= nOffsetY;
		nLoop			++;
		}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void
ESChildControlSeatHiddenCards::OnMotionStart(DWORD_PTR dwParam){
	SetVisible(true, true);
	//TRACE2(_T("%s Motion Start clock=%d \r\n"), GetName(), clock());
	/*
	m_nCard1OffsetXSave	= m_nCard1OffsetX;
	m_nCard2OffsetXSave	= m_nCard2OffsetX;
	m_nCard3OffsetXSave = m_nCard3OffsetX;
	m_nCard4OffsetXSave = m_nCard4OffsetX;
	m_nCard5OffsetXSave = m_nCard5OffsetX;*/
	}

void
ESChildControlSeatHiddenCards::OnMotionProgress(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY){
	if( !m_pOwner->IsWindow() ) 
		return;

	if( !IsVisible() )
		SetVisible(true, false);

	Rect rcOwner;
	m_pOwner->GetClientRectMy(rcOwner);

	Rect	rcRedraw		= GetClientRect();
	Rect	rRectOld		= rcRedraw;

	SetLogicalPos			(fPosX, fPosY);
	OwnerWindowSizeChanged	(rcOwner.Width(), rcOwner.Height());

	Rect rcTemp = GetClientRect();
	/*
	if( nFrame > 1 )
		rcRedraw |= GetClientRect();
	else*/
		rcRedraw = GetClientRect();

	float fPercentageCompleted	= (nFrame / ((float)nFrameCt));
	m_nAlphaConstant			= m_btAnimationAlpha + (255 - m_btAnimationAlpha)*fPercentageCompleted;//(255*fPercentageCompleted);

	if( rcRedraw != rRectOld ){
		const Rect pRects[] = {&rcRedraw, &rRectOld};
//		m_pOwner->RedrawRect(rcRedraw, 1);
		m_pOwner->RedrawMultipleRects((Rect*)pRects, 2, TRUE);

		//TRACE3(_T("%s frame=%d clock=%d \r\n"), GetName(), nFrame, clock());
		}
	}

void
ESChildControlSeatHiddenCards::OnMotionEnd(){
	if( !m_pOwner->IsWindow() ) 
		return;
#pragma message ("Incomplete code ...")
	/*
	PokerTableWnd*					pWnd			= (PokerTableWnd*)m_pOwner;
	ESChildControlSeatHiddenCards*	pHiddenCards	= pWnd->GetSeatHiddenCardsControl(m_nSeatIndex);
	ESChildControlSeatCards*		pSeatCards		= pWnd->GetSeatCardsControl(m_nSeatIndex);

	// Alpha constant.
	m_nAlphaConstant	= 255;

	// Hide animated card.
	SetVisible(false, true);

	//
	//if( !pHiddenCards || !pSeatCards ){
	//	ASSERT(FALSE);
	//	return;
	//	}

	int nCard1 = pWnd->GetSeatCardByIndex(m_nSeatIndex, 0);
	int nCard2 = pWnd->GetSeatCardByIndex(m_nSeatIndex, 1);
	int nCard3 = pWnd->GetSeatCardByIndex(m_nSeatIndex, 2);
	int nCard4 = pWnd->GetSeatCardByIndex(m_nSeatIndex, 3);

	switch( m_nCardIndex ){
		case 0:{
				pWnd->SetSeatCards(m_nSeatIndex, m_nCardValue, -1, -1, -1, true);
				break;
			}
		case 1:{
				pWnd->SetSeatCards(m_nSeatIndex, nCard1, m_nCardValue, -1, -1, true);
				break;
			}
		case 2:{
				pWnd->SetSeatCards(m_nSeatIndex, nCard1, nCard2, m_nCardValue, -1, true);
				break;
			}
		case 3:{
				pWnd->SetSeatCards(m_nSeatIndex, nCard1, nCard2, nCard3, m_nCardValue, true);
				break;
			}
		}*/
	}

void
ESChildControlSeatHiddenCards::OnMotionStopped(){/*
	m_nCard1OffsetX	= m_nCard1OffsetXSave;
	m_nCard2OffsetX	= m_nCard2OffsetXSave;
	m_nCard3OffsetX = m_nCard3OffsetXSave;
	m_nCard4OffsetX = m_nCard4OffsetXSave;
	m_nCard5OffsetX = m_nCard5OffsetXSave;*/
	}

void	
ESChildControlSeatHiddenCards::OnLButtonClick(Point pt, UINT nFlags){
	//SendEvent(this, ChildControlEventCode::EventCode_ControlClick, (int)0);
	}