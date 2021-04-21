// ESChildControlTabViewInfoPage.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\..\..\Protocol\PokerPacket.h"
#include "..\PokerGUI.h"
#include "ESChildControlTabViewInfoPage.h"
#include "..\PokerLobbyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabViewInfoPage

ESChildControlTabViewInfoPage::ESChildControlTabViewInfoPage() : ESChildControlTabViewChatPage(){
	m_crFill		= RGB(255, 255, 255);
	m_pTableInfo	= new PokerTableInfo();
	m_bChatEdit		= false;
	m_bCenterAlign	= true;
	m_nItemHeight	= 22;
	}

ESChildControlTabViewInfoPage::~ESChildControlTabViewInfoPage(){
	delete m_pTableInfo;
	}

bool
ESChildControlTabViewInfoPage::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildControlTabViewChatPage::Create(nId, rcRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlTabViewInfoPage::SetPos(int x, int y, bool bRedraw){
	ESChildControlTabViewChatPage::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlTabViewInfoPage::SetSize(int cx, int cy, bool bRedraw){
	ESChildControlTabViewChatPage::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlTabViewInfoPage::Destroy(){
	ESChildControlTabViewChatPage::Destroy();
	return true;
	}

bool
ESChildControlTabViewInfoPage::SetTableInfo(PokerTableInfo* pTableInfo){
	/*
	if( !pTableInfo ) return false;
	pTableInfo->Copy(m_pTableInfo);
	// Clear previous content.
	ClearContent(true);
	CreateTextFormat(_T("FMT1"), _T("Segoe UI"), false, false, false, 18);

	int nLine = 0;
	CString strLine;
	strLine.Format(_T(" %s #%d"), m_pTableInfo->m_sTableName, m_pTableInfo->m_nTableId);
	AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);

	strLine.Format(_T("%s - %s"), PokerLobbyWnd::GetGameName(m_pTableInfo->m_nGameTypeId, m_pTableInfo->m_nSubTypeId),
									PokerLobbyWnd::GetCurrencyName(m_pTableInfo->m_nCurrencyId));
	AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);

	strLine = _T("Stakes - ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d.%02d / %d.%02d GEL"), m_pTableInfo->m_nSmallBlindAmount/100, m_pTableInfo->m_nSmallBlindAmount%100, 
		m_pTableInfo->m_nBigBlindAmount/100, m_pTableInfo->m_nBigBlindAmount%100);
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);

	strLine = _T("Min Buy In - ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d.%02d GEL"), m_pTableInfo->m_nMinBuyIn/100, m_pTableInfo->m_nMinBuyIn%100);
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);

	strLine = _T("Max Buy In - ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d.%02d GEL"), m_pTableInfo->m_nMaxBuyIn/100, m_pTableInfo->m_nMaxBuyIn%100);
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);

	int nHandId = 100;
	strLine = _T("Hand - ");
	nLine = AddTextToLine(-1, _T("FMT1"), strLine, RGB(0, 0, 0), false);
	strLine.Format(_T("%d"), nHandId);
	AddTextToLine(nLine, _T("FMT1"), strLine, RGB(122, 0, 0), false);
	*/
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================
/*
void
ESChildControlTabViewInfoPage::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	pDC->FillSolidRect(*pRectDC, m_crFill);
	}*/

void	
ESChildControlTabViewInfoPage::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlTabViewInfoPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlTabViewInfoPage::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlTabViewInfoPage::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlTabViewInfoPage::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlTabViewInfoPage::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewInfoPage::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewInfoPage::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewInfoPage::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewInfoPage::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewInfoPage::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewInfoPage::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}