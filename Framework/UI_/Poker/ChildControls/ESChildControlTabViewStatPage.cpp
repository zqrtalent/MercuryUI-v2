// ESChildControlTabViewStatPage.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\..\..\Protocol\PokerPacket.h"
#include "..\PokerGUI.h"
#include "ESChildControlTabViewStatPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabViewStatPage

ESChildControlTabViewStatPage::ESChildControlTabViewStatPage() : ESChildControlTabViewChatPage(){
	m_crFill			= RGB(255, 255, 255);
	m_pOpenTablePacket	= NULL;
	m_bChatEdit			= false;
	m_bCenterAlign		= true;
	m_nItemHeight		= 22;
	}

ESChildControlTabViewStatPage::~ESChildControlTabViewStatPage(){
	if( m_pOpenTablePacket ){
		delete m_pOpenTablePacket;
		m_pOpenTablePacket = NULL;
		}
	}

bool
ESChildControlTabViewStatPage::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControlTabViewChatPage::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlTabViewStatPage::SetPos(int x, int y, bool bRedraw){
	ESChildControlTabViewChatPage::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlTabViewStatPage::SetSize(int cx, int cy, bool bRedraw){
	ESChildControlTabViewChatPage::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlTabViewStatPage::Destroy(){
	ESChildControlTabViewChatPage::Destroy();
	return true;
	}

bool
ESChildControlTabViewStatPage::SetStatInfo(PokerOpenTablePacket* p){
	if( !p ) return false;
	if( !m_pOpenTablePacket )
		m_pOpenTablePacket = (PokerOpenTablePacket*)p->CreateSerializableObject();
	p->Copy(m_pOpenTablePacket);

	CreateTextFormat(_T("FMT1"), _T("Segoe UI"), false, false, false, 18);

	int nLine = 0;
	CString strLine;
	strLine = _T("Played Hands: ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d"), m_pOpenTablePacket->m_nPlayedHandsCt);
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);

	strLine = _T("Won Hands: ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d"), m_pOpenTablePacket->m_nWonHandsCt);
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);

	strLine = _T("Bets: ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d.%02d GEL"), m_pOpenTablePacket->m_nSumBetsAmount/100, m_pOpenTablePacket->m_nSumBetsAmount%100);
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);

	strLine = _T("Won: ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d.%02d GEL"), m_pOpenTablePacket->m_nSumWonAmount/100, abs(m_pOpenTablePacket->m_nSumWonAmount%100));
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================
/*
void
ESChildControlTabViewStatPage::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	pDC->FillSolidRect(*pRectDC, m_crFill);
	}*/

void	
ESChildControlTabViewStatPage::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlTabViewStatPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlTabViewStatPage::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlTabViewStatPage::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlTabViewStatPage::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlTabViewStatPage::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewStatPage::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewStatPage::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewStatPage::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewStatPage::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewStatPage::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewStatPage::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}