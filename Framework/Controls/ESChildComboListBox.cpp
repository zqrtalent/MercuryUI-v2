// ESChildComboListBox.cpp : implementation file
//

#include "ESChildComboListBox.h"
#include "ESChildScrollBar.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildComboListBox

ESChildComboListBox::ESChildComboListBox() : ESChildScrollView(){
	m_sClassName		= _T("ESChildComboListBox");

	MercuryGUI* pGUI	= MercuryGUI::GetInstance();
	m_nItemHeight		= pGUI->comboListBox.m_nItemHeight;
	m_nLineHeight		= m_nItemHeight;
	m_crText			= pGUI->comboListBox.m_crText;
	m_crTextSel			= pGUI->comboListBox.m_crTextSel;
	m_crSelection		= pGUI->comboListBox.m_crSelection;
	m_nTopIndex			= 0;
	m_nSelIndex			= -1;

	m_crBorder			= pGUI->comboListBox.m_crBorder;
	m_nBorderWidth		= 1;

	m_bFillWithColor	= true;
	m_crFill			= pGUI->comboListBox.m_crFill;

	pGUI->comboListBox.m_textFont.CopyFont(&m_textFont);

	m_nTopOffset		= 0;
	m_szView.SetSize	(0, 0);
	}

ESChildComboListBox::~ESChildComboListBox(){
	ClearContent(false);
	}

bool
ESChildComboListBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildScrollView::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		}
	return bRet;
	}

void
ESChildComboListBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildScrollView::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildComboListBox::SetFocus(){
	return true;
	}

void
ESChildComboListBox::KillFocus(){
	}

bool
ESChildComboListBox::VisibilityChanged(bool bVisible){	
	return true;
	}

bool
ESChildComboListBox::SetPos(int x, int y, bool bRedraw){
	ESChildScrollView::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildComboListBox::SetSize(int cx, int cy, bool bRedraw){
	ESChildScrollView::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildComboListBox::Destroy(){
	ESChildScrollView::Destroy();
	return true;
	}

bool
ESChildComboListBox::AddItem(_string sText, void* lpParam /*= NULL*/, bool bRedraw /*= false*/){
	m_content.Add(sText, lpParam);
	m_szView.SetSize(GetClientWidth() - 2*m_nBorderWidth, m_nItemHeight*m_content.GetCount());
	m_szView.cy		+= GetClientHeight()%m_nItemHeight;

	_Rect rcClient = GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	m_szPage.SetSize(rcClient.Width(), rcClient.Height());
	AdjustScrollBarPos(bRedraw);
	return true;
	}

void
ESChildComboListBox::ClearContent(bool bRedraw){
	m_nTopIndex = 0;
	m_content.DeleteAll();

	if( bRedraw )
		Redraw(false);
	}

_string
ESChildComboListBox::GetItemText(int nItem){
	if( nItem < 0 || nItem >= m_content.GetCount() )
		return _T("");
	return m_content.GetKey(nItem);
	}

void*
ESChildComboListBox::GetItemParam(int nItem){
	if( nItem < 0 || nItem >= m_content.GetCount() )
		return NULL;
	return m_content.GetValueByIndex(nItem);
	}

bool
ESChildComboListBox::SetSelectedItem(int nItem, bool bRedraw, bool bAdjustScroll /*= true*/){
	if( nItem < 0 || nItem >= m_content.GetCount() )
		return false;
	if( nItem == m_nSelIndex )
		return true;

	int nSelOld = m_nSelIndex;
	m_nSelIndex = nItem;

	if( bRedraw ){
		_Rect rcItemOld;
		if( GetItemRect(nSelOld, rcItemOld) )
			RedrawRect(rcItemOld, false);

		_Rect rcItem;
		if( GetItemRect(nItem, rcItem) )
			RedrawRect(rcItem, false);
		}

	if( bAdjustScroll )
		EnsureItemVisibility(nItem, bRedraw);
	return true;
	}

void
ESChildComboListBox::SetTextFont(_Font* pTextFont, bool bRedraw){
	if( !pTextFont )
		return;
	pTextFont->CopyFont(&m_textFont);
	if( bRedraw )
		Redraw(true);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildComboListBox::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
    _Rect rcControlDC((m_rRect.left - pRectClient->left) + pRectDC->left, (m_rRect.top - pRectClient->top) + pRectDC->top, m_rRect.Size()), rcInvalid;
    
    ESChildScrollView::OnPaintClient(pDC, pRectDC, pRectClient);
    
    _Rect rcFrameDC(rcControlDC.left + m_nBorderWidth, rcControlDC.top + m_nBorderWidth, rcControlDC.right - m_nBorderWidth, rcControlDC.bottom - m_nBorderWidth);
    m_nTopOffset		= 0;//m_pVScroll->IsVisible() ? m_pVScroll->GetScrollPos()%m_nItemHeight : 0;
    m_nTopIndex			= m_pVScroll->IsVisible() ? m_pVScroll->GetScrollPos()/m_nItemHeight : 0;
    
    _Rect rcItemDC(rcFrameDC.left, rcFrameDC.top - m_nTopOffset, rcFrameDC.right, (rcFrameDC.top - m_nTopOffset) + m_nItemHeight);
    
    int		nLoop = m_nTopIndex, nCt = m_content.GetCount();
    _Size	szText;
    _Font*	pFontOld	= pDC->SelectObject(&m_textFont);
    
    ESLocalizationMan* pMan =  ESLocalizationMan::GetInstance();
    
    while( nLoop < nCt ){
        if( rcItemDC.top > rcFrameDC.bottom ) break;
        _Rect rcItemDCClip = rcItemDC&rcFrameDC;
        if( nLoop == m_nSelIndex ){
            // Fill selection item color.
            rcInvalid = *pRectDC&rcItemDCClip;
            if( !rcInvalid.IsRectEmpty() )
                pDC->FillSolidRect(rcInvalid, m_crSelection);
            pDC->SetTextColor(m_crTextSel);
        }
        else
            pDC->SetTextColor(m_crText);
        
        // Draw item text.
        _string sText = m_content.GetKey(nLoop);
        pMan->ParseText(sText);
        if( m_textFont.GetTextSize(&sText, szText) ){
            pDC->SetBkMode(TRANSPARENT);
            
            _Point ptText;
            ptText.x = rcItemDC.left + 3;
            ptText.y = rcItemDC.top + (m_nItemHeight - szText.cy)/2;
            pDC->ExtTextOut(ptText.x, ptText.y, ETO_CLIPPED, rcItemDCClip, sText.c_str());
        }
        
        rcItemDC.top	+= m_nItemHeight;
        rcItemDC.bottom += m_nItemHeight;
        nLoop ++;
    }
    pDC->SelectObject(pFontOld);
	}

void	
ESChildComboListBox::OnMouseHover(_Point pt, UINT nFlags){
	ESChildScrollView::OnMouseHover(pt, nFlags);

	int nSel = GetItemByPoint(pt);
	SetSelectedItem(nSel, true);
	}

void
ESChildComboListBox::OnLButtonDown(_Point pt, UINT nFlags){
	ESChildScrollView::OnLButtonDown(pt, nFlags);

	if( m_nSelIndex > -1 )
		SendEvent(this, EventCode_ListBoxItemClick);
	}

void
ESChildComboListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
//	bool bShiftIsPressed	= ((GetKeyState(VK_LSHIFT)&0x8000) == 0x8000) || ((GetKeyState(VK_RSHIFT)&0x8000) == 0x8000);
//	bool bControlIsPressed	= ((GetKeyState(VK_LCONTROL)&0x8000) == 0x8000) || ((GetKeyState(VK_RCONTROL)&0x8000) == 0x8000);

	switch( nChar ){
		case VK_UP:{
			break;
			}
		case VK_DOWN:{
			break;
			}
		};
	ESChildScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

bool
ESChildComboListBox::OnScrollBarEvent(bool bVScroll, int nCode, ESChildControl* pSender){
	bool bRet = ESChildScrollView::OnScrollBarEvent(bVScroll, nCode, pSender);
	if( bRet ){
		}
	return bRet;
	}

int
ESChildComboListBox::GetItemByPoint(_Point pt){
	if( m_content.GetCount() <= 0 ) 
		return -1;

	pt.y -= m_rRect.top - m_nBorderWidth /*border width*/ - m_nTopOffset;

	int nItem = m_nTopIndex;
	while( nItem < m_content.GetCount() ){
		if( pt.y < m_nItemHeight )
			return nItem;
		pt.y -= m_nItemHeight;
		nItem ++;
		}

	return -1;
	}

bool
ESChildComboListBox::GetItemRect(int nItem, _Rect& rcItem){
	if( nItem < m_nTopIndex || nItem >= m_content.GetCount() )
		return false;

    rcItem.SetRect(m_rRect.left + m_nBorderWidth, m_rRect.top + m_nBorderWidth - m_nTopOffset, m_rRect.right - m_nBorderWidth - (m_pVScroll->IsVisible() ? m_pVScroll->GetClientWidth() : 0), (m_rRect.top + m_nBorderWidth - m_nTopOffset) + m_nItemHeight);

	int nLoop = m_nTopIndex;
	while( nLoop < m_content.GetCount() ){

		if( nLoop == nItem ){
			if( rcItem.bottom > m_rRect.bottom )
				rcItem.bottom = m_rRect.bottom - m_nBorderWidth - (m_pHScroll->IsVisible() ? m_pHScroll->GetClientHeight() : 0);
			return true;
			}

		rcItem.top		+= m_nItemHeight;
		rcItem.bottom	+= m_nItemHeight;
		nLoop			++;
		}
	return false;
	}

