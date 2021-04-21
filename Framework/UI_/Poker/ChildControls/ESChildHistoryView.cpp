// ESChildHistoryView.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\PokerGUI.h"
#include "ESChildHistoryView.h"
#include "..\..\..\Controls\ESChildScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildHistoryView

ESChildHistoryView::ESChildHistoryView() : ESChildScrollView() {
	m_crRow1		= RGB(212, 223, 232);
	m_crRow2		= RGB(255, 255, 255);
	m_nLineHeight	= 28;
	m_nLimitItems	= 10;
	m_szView.SetSize(0, 0);
	// No indexing needed.
	m_arrItems.SetCompareFunction(AutoSortedArray::CompareEmptyReverse);

	/*
	m_history.Deserialize(_T("c:\\history.ser"));

	int nLoop = 0;
	while( nLoop < m_history.m_itemHeaders.GetCount() ){
		PokerSimpleHistoryItemHeader* pHeader = m_history.m_itemHeaders.GetAt(nLoop);
		// Header item.
		m_arrItems.Add((long)pHeader, (long)HistoryItemHeader);

		int nLoop1 = 0;
		while( nLoop1 < pHeader->m_items.GetCount() ){
			PokerSimpleHistoryItem* pItem = pHeader->m_items.GetAt(nLoop1);
			// Player item.
			m_arrItems.Add((long)pItem, (long)HistoryItemPlayer);
			nLoop1 ++;
			}

		// Separator item.
		m_arrItems.Add(0, (long)HistoryItemSeparator);
		// Empty item.
		m_arrItems.Add(0, (long)HistoryItemEmpty);
		nLoop ++;
		}

	m_szView.SetSize(120, m_nLineHeight*m_arrItems.GetCount());*/
	}

ESChildHistoryView::~ESChildHistoryView(){
	}

bool
ESChildHistoryView::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildScrollView::Create(nId, rcRect, pOwner, pParent);
	return bRet;
	}

bool
ESChildHistoryView::SetPos(int x, int y, bool bRedraw){
	ESChildScrollView::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildHistoryView::SetSize(int cx, int cy, bool bRedraw){
	ESChildScrollView::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildHistoryView::Destroy(){
	ESChildScrollView::Destroy();
	return true;
	}

void
ESChildHistoryView::OwnerWindowSizeChanged(int cx, int cy){
	ESChildScrollView::OwnerWindowSizeChanged(cx, cy);
	}

int
ESChildHistoryView::AddHistoryInfo(PokerSimpleHistory* pHistoryInfo, bool bClearExisting /*= true*/, bool bRedraw /*= true*/){
	int nCt	= 0;
	m_pVScroll->SetScrollPos(0, false);
	// Clear existing info.
	if( bClearExisting ){
		// Setup scroll info.
		m_pVScroll->SetScrollInfo	(0, 0, 0, false);
		m_history.ZeroInit();
		m_arrItems.DeleteAll();
		}

	nCt = m_history.m_itemHeaders.GetCount();
	// Copy.
	pHistoryInfo->Copy(&m_history);

	int nLoop	= (m_history.m_itemHeaders.GetCount() - 1);
	int nMin	= nCt;
	while( nLoop >= nMin ){
		PokerSimpleHistoryItemHeader* pHeader = m_history.m_itemHeaders.GetAt(nLoop);
		// Header item.
		m_arrItems.Add((long)pHeader, (long)HistoryItemHeader);

		int nLoop1 = 0;
		while( nLoop1 < pHeader->m_items.GetCount() ){
			PokerSimpleHistoryItem* pItem = pHeader->m_items.GetAt(nLoop1);
			// Player item.
			m_arrItems.Add((long)pItem, (long)HistoryItemPlayer);
			nLoop1 ++;
			}

		// Separator item.
		m_arrItems.Add(0, (long)HistoryItemSeparator);
		// Empty item.
		m_arrItems.Add(0, (long)HistoryItemEmpty);
		nLoop --;
		}

	m_szView.SetSize(0, m_nLineHeight*m_arrItems.GetCount());
	// Setup scroll info.
	PokerGUI*		pGUI		= PokerGUI::GetInstance();
	if( !m_pVScroll->SetScrollInfo(0, m_szView.cy, m_rcRect.Height(), bRedraw) ){
		if( m_pVScroll->IsVisible() )
			m_pVScroll->SetVisible(false, false);
		}
	else{
		if( !m_pVScroll->IsVisible() )
			m_pVScroll->SetVisible(true, false);
		}

	if( bRedraw ){
		CRect	rcView;
		rcView.left			= m_rcRect.left;
		rcView.top			= m_rcRect.top;
		rcView.right		= m_rcRect.right;
		rcView.bottom		= rcView.top + m_rcRect.Height();
		m_pOwner->RedrawRect(rcView, 1);
		}
	return m_arrItems.GetCount();
	}

bool
ESChildHistoryView::AddHistoryItem(PokerSimpleHistoryItemHeader* pItemHearder, bool bRedraw /*= true*/){
	return false;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

bool
ESChildHistoryView::RenderSmallCard(int nCard, CDC* pDC, CRect* pRectDC, int& xPos, CRect rcItemDC){
	if( nCard < 0 || nCard > 52 )
		return false;
	PokerGUI*		pGUI		= PokerGUI::GetInstance();
	// Render card image.
	int nCardType				= nCard / 13;
	int nCardRank				= nCard % 13;
	const CString sCardRank[]	= {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	CImage* pImages[]			= {pGUI->m_pClubLogo_Small, pGUI->m_pDiamondLogo_Small, pGUI->m_pSpadeLogo_Small, pGUI->m_pHeartLogo_Small};
	CImage* pImage				= pImages[nCardType];
	CRect	rcInvalid			= *pRectDC & rcItemDC;

	CRect rcImageDC;
	rcImageDC.left		= xPos; 
	rcImageDC.top		= rcItemDC.top + (m_nLineHeight - pImage->GetHeight()); 
	rcImageDC.right		= rcImageDC.left + pImage->GetWidth(); 
	rcImageDC.bottom	= rcImageDC.top + pImage->GetHeight(); 

	CRect rcInvalidImageDC = rcImageDC & *pRectDC;
	if( !rcInvalidImageDC.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalidImageDC, rcImageDC, pImage, false);
	xPos += (pImage->GetWidth());

	CPoint	pt;
	CSize	szText;
	// Render card rank name.
	CFont*	pFontOld	= pDC->SelectObject(pGUI->m_pHistoryWindowFont);
	CString str			= sCardRank[nCardRank];
	ImgHlp::GetTextSize(&str, pGUI->m_pHistoryWindowFont, szText);
	pt.x = xPos;
	pt.y = rcItemDC.top + (m_nLineHeight - szText.cy);
	pDC->SetBkMode		(TRANSPARENT);
	pDC->SetTextColor	(pGUI->m_crHistoryListText);
	PokerGUI::ExtTextOut(pDC, pt.x, pt.y, ETO_CLIPPED, rcInvalid, str);
	pDC->SelectObject	(pFontOld);
	xPos += szText.cx;
	return true;
	}

bool
ESChildHistoryView::RenderText(CDC* pDC, CRect* pRectDC, CRect rcItemDC, CString sText, COLORREF crText, int& xPos){
	if( sText.GetLength() <= 0 )
		return false;
	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	CRect		rcInvalid	= *pRectDC & rcItemDC;
	CPoint		pt;
	CSize		szText;

	// Render text.
	CFont*	pFontOld	= pDC->SelectObject(pGUI->m_pHistoryWindowFont);
	ImgHlp::GetTextSize(&sText, pGUI->m_pHistoryWindowFont, szText);
	pt.x = xPos;
	pt.y = rcItemDC.top + (m_nLineHeight - szText.cy);
	pDC->SetBkMode		(TRANSPARENT);
	pDC->SetTextColor	(crText);
	PokerGUI::ExtTextOut(pDC, pt.x, pt.y, ETO_CLIPPED, rcInvalid, sText);
	pDC->SelectObject	(pFontOld);
	xPos += szText.cx;
	return true;
	}

void
ESChildHistoryView::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	int nScrollCXCY	= ESChildScrollBar::GetScrollCXCY();
	CRect	rcView;
	rcView.left			= m_rcRect.left;
	rcView.top			= m_rcRect.top;
	rcView.right		= m_pVScroll->IsVisible() ? m_rcRect.right - nScrollCXCY : m_rcRect.right;
	rcView.bottom		= rcView.top + m_rcRect.Height();

	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	CFont*		pFont	= pGUI->m_pHistoryWindowFont;
	COLORREF	crText	= pGUI->m_crHistoryListText;
	CString		str;
	CPoint		pt;
	CSize		szText;

	CBrush brush1(m_crRow1);
	CBrush brush2(m_crRow2);
	CBrush* pBrush[]		= {&brush1, &brush2};

	CRect	rcViewDC, rcInvalid;
	rcViewDC.left		= (rcView.left - pRectClient->left) + pRectDC->left; 
	rcViewDC.top		= (rcView.top - pRectClient->top) + pRectDC->top; 
	rcViewDC.right		= rcViewDC.left + rcView.Width(); 
	rcViewDC.bottom		= rcViewDC.top + rcView.Height(); 

	int		nVScrollPos	= m_pVScroll->GetScrollPos();
	int		nLoop		= (nVScrollPos / m_nLineHeight);
	int		nTopRow		= nLoop;
	int		nCt			= m_arrItems.GetCount();
	int		nWidth		= 0;
	int		nOffsetY	= nVScrollPos % m_nLineHeight;
	int		nOffsetX	= 0;

	CRect rcItemDC;
	rcItemDC.left		= rcViewDC.left;
	rcItemDC.top		= rcViewDC.top - nOffsetY;
	rcItemDC.right		= rcViewDC.right;
	rcItemDC.bottom		= rcItemDC.top + m_nLineHeight;

	while( rcItemDC.top < rcViewDC.bottom ){
		rcInvalid = rcItemDC & rcViewDC;
		if( rcInvalid.IsRectEmpty() ){
			rcItemDC.top		+= m_nLineHeight;
			rcItemDC.bottom		= min(rcItemDC.top + m_nLineHeight, rcViewDC.bottom);
			nOffsetY			+= m_nLineHeight;
			nLoop				++;
			continue;
			}

		// Fill row.
	//	pDC->FillRect(rcInvalid, pBrush[(nLoop - nTopRow) % 2]);
		pDC->FillRect(rcInvalid, pBrush[0]);

		// Skip row rendering.
		if( nLoop >= nCt ){
			nOffsetY			+= m_nLineHeight;
			rcItemDC.top		+= m_nLineHeight;
			rcItemDC.bottom		= min(rcItemDC.top + m_nLineHeight, rcViewDC.bottom);
			nLoop				++;
			continue;
			}

		int nHandCardsPosX	= 120;
		int	nBestHandPosX	= 200;
		int nWonAmount		= 350;

		nOffsetX = rcItemDC.left;
		HistoryItemType itemType = (HistoryItemType)m_arrItems.GetData(nLoop);
		switch( itemType ){
			case HistoryItemType::HistoryItemHeader:
				{
					PokerSimpleHistoryItemHeader* pHeader = (PokerSimpleHistoryItemHeader*)m_arrItems.Get(nLoop);
					if( pHeader->m_dtTime.GetStatus() == 0 ) // Valid
						{
						str.Format(_T("[%02d:%02d:%02d] Hand %d :"), pHeader->m_dtTime.GetHour(), 
							pHeader->m_dtTime.GetMinute(), 
							pHeader->m_dtTime.GetSecond(), 
							pHeader->m_nHandId);
						}
					else
						str.Format(_T("Hand %d : "), pHeader->m_nHandId);

					if( RenderText(pDC, pRectDC, rcItemDC, str, RGB(122, 0, 0), nOffsetX) )
						nOffsetX += 10;

					// table cards. {{
					if( RenderSmallCard((int)(int)pHeader->m_cTableCard1, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;

					if( RenderSmallCard((int)(int)pHeader->m_cTableCard2, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;

					if( RenderSmallCard((int)(int)pHeader->m_cTableCard3, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;

					if( RenderSmallCard((int)(int)pHeader->m_cTableCard4, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;

					if( RenderSmallCard((int)(int)pHeader->m_cTableCard5, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;
					// }}
					break;
				}
			case HistoryItemType::HistoryItemPlayer:
				{
					PokerSimpleHistoryItem* pItem = (PokerSimpleHistoryItem*)m_arrItems.Get(nLoop);

					// User name. {{
					str.Format(_T("%s"), pItem->m_sUserName);
					str = str.Left(20);
					RenderText(pDC, pRectDC, rcItemDC, str, RGB(122, 122, 0), nOffsetX);
					// }}

					// hand cards.
					nOffsetX = rcItemDC.left + nHandCardsPosX;
					if( RenderSmallCard((int)(int)pItem->m_cCard1, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;
					if( RenderSmallCard((int)(int)pItem->m_cCard2, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;
					if( RenderSmallCard((int)(int)pItem->m_cCard3, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;
					if( RenderSmallCard((int)(int)pItem->m_cCard4, pDC, pRectDC, nOffsetX, rcItemDC) )
						nOffsetX += 5;

					// Hand card class.
					nOffsetX = rcItemDC.left + nBestHandPosX;
					const CString sClassNames[] = {_T("----"), _T("Straight Flush"), _T("Four Of A Kind"), _T("Full House"), _T("Flush"), _T("Straight"), 
						_T("Three Of A Kind"), _T("Two Pairs"), _T("Pair"), _T("HighCard")};		
					int nHandCards = (int)pItem->m_cBestHandCardsClass;
					if( nHandCards > -1 && 
						nHandCards < sizeof(sClassNames)/sizeof(CString) ){
						str = sClassNames[nHandCards];
						}
					else
						str = _T("----");
					RenderText(pDC, pRectDC, rcItemDC, str, pGUI->m_crHistoryListText, nOffsetX);

					// Won amounts.
					nOffsetX = rcItemDC.left + nWonAmount;
					str.Format(_T("%d.%02d"), pItem->m_nWonAmount/100, pItem->m_nWonAmount%100);
					RenderText(pDC, pRectDC, rcItemDC, str, pGUI->m_crHistoryListText, nOffsetX);

					break;
				}
			case HistoryItemType::HistoryItemSeparator:
				{
					str = _T("==============================");
					RenderText(pDC, pRectDC, rcItemDC, str, RGB(122, 0, 0), nOffsetX);
					break;
				}

			case HistoryItemType::HistoryItemEmpty:
				{
					break;
				}
			};

		rcItemDC.top		+= m_nLineHeight;
		rcItemDC.bottom		= min(rcItemDC.top + m_nLineHeight, rcViewDC.bottom);
		nLoop				++;
		}

	brush1.DeleteObject();
	brush2.DeleteObject();
	}
