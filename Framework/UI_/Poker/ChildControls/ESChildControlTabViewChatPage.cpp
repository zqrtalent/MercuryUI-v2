// ESChildControlTabViewChatPage.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\..\..\String\StringDecoder.h"
#include "..\PokerGUI.h"
#include "ESChildControlTextEdit.h"
#include "ESChildControlTabViewChatPage.h"
#include "..\..\..\Controls\ESChildScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabViewChatPage

ESChildControlTabViewChatPage::ESChildControlTabViewChatPage() : ESChildControlTabViewPage(){
	m_crFill		= RGB(255, 255, 255);
	m_nItemHeight	= 16;
	m_nTopLine		= 0;
	m_pVScroll		= NULL;
	m_pEdit			= NULL;
	m_bChatEdit		= true;
	m_bCenterAlign	= false;
	}

ESChildControlTabViewChatPage::~ESChildControlTabViewChatPage(){
	DestroyObjects();
	}

bool
ESChildControlTabViewChatPage::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){/*
		PokerGUI* pGUI = PokerGUI::GetInstance();
		if( m_bChatEdit ){
			CRect rcTextEdit;
			rcTextEdit.left		= m_rcRect.left;
			rcTextEdit.top		= m_rcRect.bottom - pGUI->m_nChatEditHeight;
			rcTextEdit.right	= m_rcRect.right;
			rcTextEdit.bottom	= rcTextEdit.top + pGUI->m_nChatEditHeight;
			m_pEdit				= new ESChildControlTextEdit();
			m_pEdit->SetSizeAutoCalculation	(false);
			m_pEdit->SetFont				(pGUI->m_pTMessageFont);
			pOwner->AddChildControl(m_pEdit, CHILD_Table_Chat_Edit, rcTextEdit, this); 
			rcRect.bottom		-= pGUI->m_nChatEditHeight;
			}

		int nScrollCXCY = ESChildScrollBar::GetScrollCXCY();
		CRect rcVScroll;
		rcVScroll.left		= rcRect.right - nScrollCXCY;
		rcVScroll.top		= rcRect.top;
		rcVScroll.right		= rcVScroll.left + nScrollCXCY;
		rcVScroll.bottom	= rcRect.bottom;

		m_pVScroll = new ESChildScrollBar();
		pOwner->AddChildControl(m_pVScroll, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcVScroll, this);
		if( !m_pVScroll->SetScrollInfo(0, m_listChatItems.GetCount()*m_nItemHeight, rcRect.Height(), false) )
			m_pVScroll->SetVisible(false, false);
		else{
			m_pVScroll->SetVisible(IsVisible(), false);
			}*/
		}
	return bRet;
	}

bool
ESChildControlTabViewChatPage::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlTabViewChatPage::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlTabViewChatPage::Destroy(){
	ESChildControl::Destroy();
	m_pVScroll = NULL;
	return true;
	}

void
ESChildControlTabViewChatPage::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);

	PokerGUI* pGUI		= PokerGUI::GetInstance();
	int nScrollCXCY		= ESChildScrollBar::GetScrollCXCY();;
	
	CRect rcVScroll;
	rcVScroll.left		= m_rcRect.right - nScrollCXCY;
	rcVScroll.top		= m_rcRect.top;
	rcVScroll.right		= rcVScroll.left + nScrollCXCY;
	rcVScroll.bottom	= m_rcRect.bottom -  (m_bChatEdit ? pGUI->m_nChatEditHeight : 0);
	
	m_pVScroll->SetPos	(rcVScroll.left, rcVScroll.top, false);
	m_pVScroll->SetSize	(rcVScroll.Width(), rcVScroll.Height(), false);
	RecalcVScroll(false);

	if( m_pEdit ){
		CRect rcTextEdit;
		rcTextEdit.left		= m_rcRect.left;
		rcTextEdit.top		= m_rcRect.bottom - pGUI->m_nChatEditHeight;
		rcTextEdit.right	= m_rcRect.right;
		rcTextEdit.bottom	= rcTextEdit.top + pGUI->m_nChatEditHeight;
		m_pEdit->SetPos		(rcTextEdit.left, rcTextEdit.top, false);
		m_pEdit->SetSize	(rcTextEdit.Width(), rcTextEdit.Height(), false);
		}
	}

bool
ESChildControlTabViewChatPage::IsScrollBarVisible(ESChildControl* pScrollBar){
	if( pScrollBar == m_pVScroll && m_pVScroll ){
		PokerGUI* pGUI	= PokerGUI::GetInstance();
		return m_pVScroll->SetScrollInfo(0, m_listChatItems.GetCount()*m_nItemHeight, (m_rcRect.Height() - (m_bChatEdit ? pGUI->m_nChatEditHeight : 0)), false);
		}
	return false;
	}

void
ESChildControlTabViewChatPage::RecalcVScroll(bool bInvalidate){
	if( !m_pVScroll ) return;
	PokerGUI* pGUI	= PokerGUI::GetInstance();
	if( !m_pVScroll->SetScrollInfo(0, m_listChatItems.GetCount()*m_nItemHeight, (m_rcRect.Height() - (m_bChatEdit ? pGUI->m_nChatEditHeight : 0)), false) ){
		m_pVScroll->SetVisible(false, false);
		}
	else{
		if( !m_pVScroll->IsVisible() ){
			m_pVScroll->SetVisible(IsVisible(), bInvalidate);
			}
		}
	if( m_pVScroll->IsVisible() && bInvalidate )
		m_pVScroll->Redraw(true);
	}

int
ESChildControlTabViewChatPage::SplitText(CString& sText, CStringArray& arrStrings){
	const CString sCardTypes[]	= {_T("C"), _T("D"), _T("S"), _T("H")};
	const CString sCardRank[]	= {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
	int nFind	= 0;
	int nStart	= 0;
	int nLastSavedIdx = 0;
	CString str;
	while( nFind != -1 ){
		nFind = sText.Find('^', nStart);
		if( nFind != -1 && (nFind + 3) <= sText.GetLength() ){
			str	= sText.Mid(nFind, 3);
			str.MakeUpper();
			char cCardType = str.GetAt(2);
			char cCardRank = str.GetAt(1);

			bool bSuccess = false;
			int nLoop = 0;
			while( nLoop < sizeof(sCardRank)/sizeof(CString) ){
				if( cCardRank == sCardRank[nLoop] ){
					str.SetAt(0, '^');
					str.SetAt(1, '0' + (char)nLoop);
					nLoop = 0;
					while( nLoop < sizeof(sCardTypes)/sizeof(CString) ){
						if( cCardType == sCardTypes[nLoop] ){
							bSuccess = true;
							str.SetAt(2, '0' + (char)nLoop);
							break;
							}
						nLoop ++;
						}
					break;
					}
				nLoop ++;
				}

			if( bSuccess ){
				arrStrings.Add(sText.Mid(nLastSavedIdx, nFind - nLastSavedIdx));
				arrStrings.Add(str);
				nStart			= nFind + 3;
				nLastSavedIdx	= nStart;
				}
			else
				nStart = nFind + 1;
			}
		else{
			nFind	= -1;
			str		= sText.Mid(nStart, sText.GetLength() - nStart);
			arrStrings.Add(str);
			}
		}
	return (int)arrStrings.GetCount();
	}

void
ESChildControlTabViewChatPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	int nOffset = (zDelta / 120)*m_nItemHeight;
	if( m_pVScroll->IsVisible() ){
		m_pVScroll->SetScrollPos(m_pVScroll->GetScrollPos() - nOffset, true);
		}
	}

bool
ESChildControlTabViewChatPage::OnScrollBarEvent(bool bVScroll, int nCode, ESChildControl* pSender){
	ESChildScrollBar* pScrollBar = (ESChildScrollBar*)pSender;
	bool bRet = true;
	switch( nCode ) {
		case SB_LINEDOWN:
			{
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + m_nItemHeight, true);
				break;
			}
		case SB_LINEUP:
			{
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - m_nItemHeight, true);
				break;
			}
		case SB_PAGEDOWN:
			{
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + pScrollBar->GetScrollPage(), true);
				break;
			}
		case SB_PAGEUP:
			{
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - pScrollBar->GetScrollPage(), true);
				break;
			}
		case SB_THUMBTRACK:
			{
				break;
			}
		};

	if( bRet ){
		int nScrollPos = pScrollBar->GetScrollPos();
		m_nTopLine	= nScrollPos / m_nItemHeight;
		PokerGUI* pGUI		= PokerGUI::GetInstance();

		int		nScrollCXCY = ESChildScrollBar::GetScrollCXCY();;
		CRect	rcRedraw;
		rcRedraw.left		= m_rcRect.left;
		rcRedraw.top		= m_rcRect.top;
		rcRedraw.right		= m_pVScroll->IsVisible() ? m_rcRect.right - nScrollCXCY : m_rcRect.right;
		rcRedraw.bottom		= rcRedraw.top + m_rcRect.Height() - (m_bChatEdit ? pGUI->m_nChatEditHeight : 0);
		m_pOwner->RedrawRect(rcRedraw, 1);
		}
	return bRet;
	}

bool
ESChildControlTabViewChatPage::CreateTextFormat(CString sName, 
												const CString sFontName, bool bItalic, bool bUnderLine,
												bool bBold, int nFontHeight)
	{
	sName.MakeLower();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight			= nFontHeight;
	_tcscpy_s(lf.lfFaceName, sFontName);
	lf.lfWeight			= bBold ? FW_BOLD : FW_NORMAL;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfItalic			= bItalic;
	lf.lfUnderline		= bUnderLine;
	lf.lfStrikeOut		= 0;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	CFont* pFont		= new CFont();
	pFont->CreateFontIndirect(&lf);

	long nTextCode = (long)DecodeString(sName.GetBuffer());
	if( m_arrFormatNames.IndexOf(nTextCode) != -1 ){
		delete pFont;
		return false;
		}

	m_arrFormatNames.Add(nTextCode, (long)pFont);
	m_listFonts		.Add(pFont);
	return true;
	}

int
ESChildControlTabViewChatPage::AddTextToLine(int nLine, CString sFormatName, CString sText, COLORREF crText, bool bRedraw){
	sFormatName.MakeLower();
	long nTextCode = (long)DecodeString(sFormatName.GetBuffer());
	if( m_arrFormatNames.IndexOf(nTextCode) == -1 ){
		return -1; // Format not found.
		}

	ESChatTextTag* pNewTag	= new ESChatTextTag;
	pNewTag->dwChatType		= ESCHAT_TAG_TEXT;
	pNewTag->pFirst			= NULL;
	pNewTag->pNext			= NULL;
	pNewTag->dwFormatId		= nTextCode;
	pNewTag->sText			= sText;
	pNewTag->crText			= crText;

	int nResult = m_listChatItems.GetCount();
	if( nLine == -1 ){	// Add new line.
		ESChatItem* pItem	= new ESChatItem;
		pItem->pTag			= pNewTag;
		m_listChatItems.Add(pItem);
		}
	else{				// Append existed line.
		ESChatItem* pItem = m_listChatItems.GetAt(nLine);
		if( !pItem || !pItem->pTag ) return -1;

		ESChatTag* pPrevTag = pItem->pTag;
		while( pPrevTag->pNext ){
			pPrevTag = pPrevTag->pNext;
			}

		pPrevTag->pNext			= pNewTag;
		pNewTag->pFirst			= pItem->pTag;
		pNewTag->pNext			= NULL;
		nResult					= nLine;
		}
	RecalcVScroll(bRedraw);
	return nResult;
	}

int
ESChildControlTabViewChatPage::AddImageToLine(int nLine, CString sImageFile, int nWidth, int nHeight, bool bRedraw){
	sImageFile.MakeLower();
	long	nCode	= (long)DecodeString(sImageFile.GetBuffer());
	int		nIndex	= m_arrImageNames.IndexOf(nCode);
	CImage* pImage	= (nIndex == -1) ? PokerGUI::LoadImage(sImageFile) : GetImageById(nIndex);
	if( !pImage )
		return -1; // Image not found.
	m_listImages.Add(pImage);

	ESChatImageTag* pNewTag	= new ESChatImageTag;
	pNewTag->dwChatType		= ESCHAT_TAG_IMAGE;
	pNewTag->pFirst			= NULL;
	pNewTag->pNext			= NULL;
	pNewTag->nImageId		= (m_listImages.GetCount() - 1);
	pNewTag->szImage.SetSize(nWidth, nHeight);
	pNewTag->sImageFile		= sImageFile;

	int nResult = m_listChatItems.GetCount();
	if( nLine == -1 ){	// Add new line.
		ESChatItem* pItem	= new ESChatItem;
		pItem->pTag			= pNewTag;
		m_listChatItems.Add(pItem);
		}
	else{				// Append existed line.
		ESChatItem* pItem = m_listChatItems.GetAt(nLine);
		if( !pItem || !pItem->pTag ) return -1;

		ESChatTag* pPrevTag = pItem->pTag;
		while( pPrevTag->pNext ){
			pPrevTag = pPrevTag->pNext;
			}

		pPrevTag->pNext			= pNewTag;
		pNewTag->pFirst			= pItem->pTag;
		pNewTag->pNext			= NULL;
		nResult					= nLine;
		}
	RecalcVScroll(bRedraw);
	return nResult;
	}

CFont*
ESChildControlTabViewChatPage::GetFormatFont(int nFormatId){
	int nIndex = m_arrFormatNames.IndexOf(nFormatId);
	if( nIndex == -1 )
		return NULL;
	return (CFont*)m_arrFormatNames.GetData(nIndex);
	}

CImage*
ESChildControlTabViewChatPage::GetImageById(int nId){
	return m_listImages.GetAt(nId);
	}

void
ESChildControlTabViewChatPage::DestroyObjects(){
	int nLoop = 0;
	while( nLoop < m_listChatItems.GetCount() ){
		ESChatItem* pItem = m_listChatItems[nLoop];
		if( pItem && pItem->pTag ){
			ESChatTag* pTag = pItem->pTag;
			while( pTag ){
				ESChatTag* pNextTag = pTag->pNext;
				delete pTag;
				pTag = pNextTag;
				}
			}
		nLoop ++;
		}

	m_nTopLine	= 0;
	m_listChatItems	.RemoveAll();
	m_listImages	.RemoveAll();
	m_listFonts		.RemoveAll();
	m_arrImageNames	.DeleteAll();
	m_arrFormatNames.DeleteAll();
	RecalcVScroll(true);
	}

bool
ESChildControlTabViewChatPage::ClearContent(bool bRedraw){
	int nLoop = 0;
	while( nLoop < m_listChatItems.GetCount() ){
		ESChatItem* pItem = m_listChatItems[nLoop];
		if( pItem && pItem->pTag ){
			ESChatTag* pTag = pItem->pTag;
			while( pTag ){
				ESChatTag* pNextTag = pTag->pNext;
				delete pTag;
				pTag = pNextTag;
				}
			}
		nLoop ++;
		}
	m_listChatItems	.RemoveAll();
	m_nTopLine	= 0;
	m_pVScroll->SetScrollInfo	(0, 0, 0, false);
	m_pVScroll->SetVisible		(false, bRedraw);

	if( bRedraw ){
		Redraw(false);
		}
	return true;
	}

bool
ESChildControlTabViewChatPage::GotoLine(int nLine, bool bRedraw){
	if( nLine < 0 || nLine >= GetLineCount() )
		return false;

	if( m_rcRect.Height() <= 0 )
		return false;
	PokerGUI* pGUI = PokerGUI::GetInstance();

	int nLineCtPerPage = ((m_rcRect.Height() - (m_bChatEdit ? pGUI->m_nChatEditHeight : 0)) / m_nItemHeight);
	m_nTopLine = GetLineCount() - nLineCtPerPage;
	if( m_nTopLine < 0 )
		m_nTopLine = 0;

	if( bRedraw ){
		if( m_pVScroll->IsVisible() ){
			m_pVScroll->SetScrollPos(m_nItemHeight*m_nTopLine, false);
			m_pVScroll->Redraw(false);
			}
		else{
			Redraw();
			}
		}
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlTabViewChatPage::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI* pGUI = PokerGUI::GetInstance();
	int nScrollCXCY = ESChildScrollBar::GetScrollCXCY();
	CRect	rcPageDC, rcInvalid;
	rcPageDC.left	= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcPageDC.top	= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcPageDC.right	= rcPageDC.left + m_rcRect.Width(); 
	rcPageDC.bottom	= rcPageDC.top + (m_rcRect.Height() - (m_bChatEdit ? pGUI->m_nChatEditHeight : 0)); 

	CFont	fontCardRank;
	LOGFONT lf;
	pGUI->m_pTMessageFont->GetLogFont(&lf);
	lf.lfHeight = 17;
	lf.lfWeight	= FW_BOLD;
	fontCardRank.CreateFontIndirect(&lf);

	// Fill background.
	rcInvalid = *pRectDC&rcPageDC;
	if( !rcInvalid.IsRectEmpty() )
		pDC->FillSolidRect(rcInvalid, m_crFill);

	int		nLoop		= m_nTopLine;
	int		nOffsetX	= 0;
	int		nOffsetY	= (m_pVScroll && m_pVScroll->IsVisible()) ? m_pVScroll->GetScrollPos()%m_nItemHeight : 0;
	CSize	szText;

	bool	bCenter		= m_bCenterAlign;
	bool	bCalcSize	= false;
	bool	bSizeCalculated	= false;

	pDC->SetBkMode(TRANSPARENT);
	while( nLoop < m_listChatItems.GetCount() ){
		ESChatItem* pItem = m_listChatItems.GetAt(nLoop);
		if( !pItem || !pItem->pTag ){
			nLoop ++;
			continue;
			}

_render:
		CRect rcItemDC;
		rcItemDC.left			= rcPageDC.left;
		rcItemDC.top			= rcPageDC.top + ((nLoop - m_nTopLine)*m_nItemHeight) - nOffsetY;
		rcItemDC.right			= rcPageDC.right;
		rcItemDC.bottom			= rcItemDC.top + m_nItemHeight;

		if( rcItemDC.top >= rcPageDC.bottom )
			break;

		if( bCenter ){
			if( bSizeCalculated ){
				bSizeCalculated	= false;
				bCalcSize		= false;
				int nLeft		= rcItemDC.left;
				int nRight		= rcPageDC.right;
				nOffsetX		= nLeft + (nRight - nOffsetX)/2;
				}
			else{
				bCalcSize = true;
				nOffsetX =  rcItemDC.left;
				}
			}
		else
			nOffsetX =  rcItemDC.left;

		ESChatTag* pChatTag		= pItem->pTag;
		while( pChatTag ){
			CRect rcItemTagDC;
			rcItemTagDC.left	= nOffsetX;
			rcItemTagDC.top		= rcItemDC.top;
			rcItemTagDC.right	= rcItemDC.right;
			rcItemTagDC.bottom	= rcItemDC.bottom;

			if( pChatTag->dwChatType == ESCHAT_TAG_TEXT ){
				CString		sText		= ((ESChatTextTag*)pChatTag)->sText;
				CFont*		pFont		= GetFormatFont(((ESChatTextTag*)pChatTag)->dwFormatId);
				COLORREF	crText		= ((ESChatTextTag*)pChatTag)->crText;
				CPoint		ptLabel(rcItemTagDC.left, 0);

				rcInvalid = *pRectDC&rcItemTagDC;
				if( pFont ){
					CStringArray arrStrings;
					SplitText(sText, arrStrings);
												
					for(int i=0; i<arrStrings.GetCount(); i++){
						sText = arrStrings[i];
						if( sText.GetLength() == 3 && sText[0] == '^' ){	// Render card.
							int nCardType	= (int)sText[2] - (int)'0';
							int nCardRank	= (int)sText[1] - (int)'0';
							const CString sCardRank[]	= {_T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("T"), _T("J"), _T("Q"), _T("K"), _T("A")};
							CImage* pImages[] = {pGUI->m_pClubLogo_Small, pGUI->m_pDiamondLogo_Small, pGUI->m_pSpadeLogo_Small, pGUI->m_pHeartLogo_Small};
							CImage* pImage = pImages[nCardType];
							
							if( pImage ){
								// Render card image.
								CRect rcImageDC;
								rcImageDC.left		= ptLabel.x; 
								rcImageDC.top		= rcItemTagDC.top + (rcItemTagDC.Height() - pImage->GetHeight()); 
								rcImageDC.right		= rcImageDC.left + pImage->GetWidth(); 
								rcImageDC.bottom	= rcImageDC.top + pImage->GetHeight(); 

								CRect rcInvalidImageDC = rcImageDC & *pRectDC;
								if( !rcInvalidImageDC.IsRectEmpty() && !bCalcSize )
									PokerGUI::RenderImage(pDC, rcInvalidImageDC, rcImageDC, pImage, false);
								ptLabel.x += (pImage->GetWidth());

								// Render card rank name.
								CFont*	pFontOld = pDC->SelectObject(&fontCardRank);
								sText  = sCardRank[nCardRank];
								ImgHlp::GetTextSize(&sText, pFont, szText);
								ptLabel.y = rcItemTagDC.top + (rcItemTagDC.Height() - szText.cy)/2;
								if( !bCalcSize ){
									pDC->SetTextColor	(pGUI->m_crTMessageText);
									pDC->ExtTextOut		(ptLabel.x, ptLabel.y, ETO_CLIPPED, rcInvalid, sText, NULL);
									}
								pDC->SelectObject	(pFontOld);
								ptLabel.x += szText.cx;
								}
							}
						else{	// Render text.
							// Render text. {{
							CFont*	pFontOld = pDC->SelectObject	(pFont);
							ImgHlp::GetTextSize(&sText, pFont, szText);
							ptLabel.y = rcItemTagDC.top + (rcItemTagDC.Height() - szText.cy)/2;
							pDC->SetTextColor	(crText);
							if( !bCalcSize ){
								PokerGUI::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcInvalid, sText);
	//							pDC->ExtTextOut		(ptLabel.x, ptLabel.y, ETO_CLIPPED, rcInvalid, sText, NULL);
								}
							pDC->SelectObject	(pFontOld);
							// }}
							ptLabel.x += szText.cx;
							}
						}	
					rcItemTagDC.right = ptLabel.x;
					
					}
				}
			else
			if( pChatTag->dwChatType == ESCHAT_TAG_IMAGE ){
				CSize		szImage	= ((ESChatImageTag*)pChatTag)->szImage;
				CImage*		pImage	= GetImageById(((ESChatImageTag*)pChatTag)->nImageId);

				if( pImage && szImage.cx > 0 && szImage.cy > 0 ){
					// Render image.
					rcItemTagDC.right	= rcItemTagDC.left + szImage.cx;

					CRect rcImageDC;
					rcImageDC.left		= rcItemTagDC.left; 
					rcImageDC.top		= rcItemTagDC.top + (rcItemTagDC.Height() - szImage.cy); 
					rcImageDC.right		= rcImageDC.left + szImage.cx; 
					rcImageDC.bottom	= rcImageDC.top + szImage.cy; 

					rcInvalid = rcImageDC & *pRectDC;
					if( !rcInvalid.IsRectEmpty() && !bCalcSize )
						PokerGUI::RenderImage(pDC, rcInvalid, rcImageDC, pImage);
					// }}
					}
				}

			nOffsetX	 = rcItemTagDC.right;
			pChatTag	 = pChatTag->pNext;
			}

		if( bCalcSize ){
			bSizeCalculated = true;
			goto _render;
			}
		nLoop		++;
		}
	fontCardRank.DeleteObject();
	}

void	
ESChildControlTabViewChatPage::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlTabViewChatPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESChildControlTabViewChatPage::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlTabViewChatPage::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlTabViewChatPage::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlTabViewChatPage::OnLButtonUp(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewChatPage::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewChatPage::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewChatPage::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewChatPage::OnLButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewChatPage::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlTabViewChatPage::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}