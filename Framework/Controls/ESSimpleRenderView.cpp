// ESSimpleRenderView.cpp : implementation file
//


#include "ESSimpleRenderView.h"
#include "ESChildScrollBar.h"
#include "../String/StringDecoder.h"
#include "../Utility/StringHelper.h"

/////////////////////////////////////////////////////////////////////////////
// ESSimpleRenderView_TextStyle

ESSimpleRenderView_TextStyle::ESSimpleRenderView_TextStyle(){
	}

BEGIN_DECLARE_VAR(ESSimpleRenderView_TextStyle, Serializable)
	DECLARE_VAR(_T("ESSimpleRenderView_TextStyle"),		_T(""),					VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("StyleName"),						_T("STYLE_NAME"),		VariableType::VT_String, offsetof(ESSimpleRenderView_TextStyle,m_sStyleName), false, false, false)
	DECLARE_VAR(_T("FontName"),							_T("FONTHEIGHT"),		VariableType::VT_String, offsetof(ESSimpleRenderView_TextStyle,m_sFontName), false, false, false)
	DECLARE_VAR(_T("FontHeight"),						_T("HEIGHT"),			VariableType::VT_Int32, offsetof(ESSimpleRenderView_TextStyle,m_nFontHeight), false, false, false)
	DECLARE_VAR(_T("Italic"),							_T("IS_ITALIC"),		VariableType::VT_Int8, offsetof(ESSimpleRenderView_TextStyle,m_bItalic), false, false, false)
	DECLARE_VAR(_T("UnderLine"),						_T("IS_UNDERLINE"),		VariableType::VT_Int8, offsetof(ESSimpleRenderView_TextStyle,m_bUnderLine), false, false, false)
	DECLARE_VAR(_T("Bold"),								_T("IS_BOLD"),			VariableType::VT_Int8, offsetof(ESSimpleRenderView_TextStyle,m_bBold), false, false, false)
END_DECLARE_VAR()

ESSimpleRenderView_TextStyle::~ESSimpleRenderView_TextStyle(){
	m_font.DeleteObject();
	}

/////////////////////////////////////////////////////////////////////////////
// ESSimpleRenderView_ImageStyle

ESSimpleRenderView_ImageStyle::ESSimpleRenderView_ImageStyle(){
	m_pImage = NULL;
	m_szImage.SetSize(0, 0);
	}

BEGIN_DECLARE_VAR(ESSimpleRenderView_ImageStyle, Serializable)
	DECLARE_VAR(_T("ESSimpleRenderView_ImageStyle"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("StyleName"),						_T("STYLE_NAME"), VariableType::VT_String, offsetof(ESSimpleRenderView_ImageStyle,m_sStyleName), false, false, false)
	DECLARE_VAR(_T("Image File"),						_T("IMAGE_FILE"), VariableType::VT_String, offsetof(ESSimpleRenderView_ImageStyle,m_sImageFile), false, false, false)
END_DECLARE_VAR()

ESSimpleRenderView_ImageStyle::~ESSimpleRenderView_ImageStyle(){
	}

/////////////////////////////////////////////////////////////////////////////
// ESSimpleRenderView

ESSimpleRenderView::ESSimpleRenderView(){
	m_sClassName		= _T("ESSimpleRenderView");
	m_crFill			= RGB(122, 0, 122);
	m_crBorder			= RGB(102, 102, 102);
	m_nBorderWidth		= 1;
	m_pRootLine			= new RenderLine();
	m_pLastLine			= NULL;
	m_pTopLine			= NULL;
	m_nLineHeight		= 15;
	m_nLineHeightMin	= 0;
	m_nTopOffsetY		= 0;
	m_nTopFullOffsetY	= 0;
	m_nContentHeight	= 0;

	m_bLineColors		= false;
	m_crLine1			= RGB(0, 0, 0);
	m_crLine2			= RGB(0, 0, 0);
	m_crLineBorder		= RGB(0, 0, 0);

	m_nTextMarginLeftRight	= 2;
	m_nTextMarginTopBottom	= 2;

	/*
	// Temp
	m_bLineColors		= true;
	m_crLine1			= RGB(213, 213, 213);
	m_crLine2			= RGB(204, 204, 204);
	m_crLineBorder		= RGB(181, 181, 181);
	// Temp
	*/
    
	// Create variables. {{
    // Problem in MacOSX
	m_listTextStyles.CreateVariableIndex(_T("StyleName"), Serializable::Compare_String);
	m_listImageStyles.CreateVariableIndex(_T("StyleName"), Serializable::Compare_String);
	// }}
	}

BEGIN_DECLARE_VAR(ESSimpleRenderView, ESChildControl)
	DECLARE_VAR(_T("ESSimpleRenderView"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Fill Color"),			_T(""), VariableType::VT_Color32, offsetof(ESSimpleRenderView,m_crFill), false, false, false)
	DECLARE_VAR(_T("Border Color"),			_T(""), VariableType::VT_Color32, offsetof(ESSimpleRenderView,m_crBorder), false, false, false)

	DECLARE_VAR(_T("Line Colors"),			_T(""), VariableType::VT_Bool, offsetof(ESSimpleRenderView,m_bLineColors), false, false, false)
	DECLARE_VAR(_T("Line Color1"),			_T(""), VariableType::VT_Color32, offsetof(ESSimpleRenderView,m_crLine1), false, false, false)
	DECLARE_VAR(_T("Line Color2"),			_T(""), VariableType::VT_Color32, offsetof(ESSimpleRenderView,m_crLine2), false, false, false)
	DECLARE_VAR(_T("Line Border"),			_T(""), VariableType::VT_Color32, offsetof(ESSimpleRenderView,m_crLineBorder), false, false, false)

	DECLARE_VAR(_T("Text Styles"),			_T(""), VariableType::VT_None, offsetof(ESSimpleRenderView,m_listTextStyles), false, true, true)
	DECLARE_VAR(_T("Image Styles"),			_T(""), VariableType::VT_None, offsetof(ESSimpleRenderView,m_listImageStyles), false, true, true)
END_DECLARE_VAR()

ESSimpleRenderView::~ESSimpleRenderView(){
	ClearContent(false);
	delete m_pRootLine;
	}

bool
ESSimpleRenderView::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	m_szPage.SetSize(rRect.Width(), rRect.Height());
	m_szView.SetSize(rRect.Width(), m_nContentHeight);
	bool bRet = ESChildScrollView::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		}
	return bRet;
	}

bool
ESSimpleRenderView::SetPos(int x, int y, bool bRedraw){
	ESChildScrollView::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESSimpleRenderView::SetSize(int cx, int cy, bool bRedraw){
	ESChildScrollView::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESSimpleRenderView::Destroy(){
	ESChildScrollView::Destroy();
	return true;
	}

void
ESSimpleRenderView::OwnerWindowSizeChanged(int cx, int cy){
	_Rect rcClient = GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	m_szPage.SetSize(rcClient.Width(), rcClient.Height());
	m_szView.SetSize(rcClient.Width(), m_nContentHeight);
	ESChildScrollView::OwnerWindowSizeChanged(cx, cy);

	m_nTopFullOffsetY	= 0;
	m_pTopLine			= NULL;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESSimpleRenderView::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	ESChildScrollView::OnPaintClient(pDC, pRectDC, pRectClient);

	_Rect	rcControlDC, rcInvalid;
	_Rect	rRect		= GetClientRect();
	rcControlDC.left	= (rRect.left - pRectClient->left) + pRectDC->left; 
	rcControlDC.top		= (rRect.top - pRectClient->top) + pRectDC->top; 
	rcControlDC.right	= rcControlDC.left + rRect.Width(); 
	rcControlDC.bottom	= rcControlDC.top + rRect.Height();
	rcControlDC.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);

	if( m_pVScroll && m_pVScroll->IsVisible() )
		rcControlDC.right -= m_pVScroll->GetClientWidth();

	_Point	ptOffset(rcControlDC.left, rcControlDC.top);
	_Rect	rcLineDC, rcDrawDC;

	int nScrollPos		= GetVScrollPos();
	m_pTopLine			= GetLineByOffset(nScrollPos);
	if( m_pTopLine )
		m_nTopOffsetY	= nScrollPos;
	else
		m_nTopOffsetY	= 0;

	RenderLine* pLine	= m_pTopLine;
	if( !pLine )
		pLine = m_pRootLine->m_pNextLine;

	int		nLineIndex	= 0;
//	CPen	penBorder(PS_SOLID, 1, m_crLineBorder);
//	CPen*	penOld		= pDC->SelectObject(&penBorder);
//	CBrush	brushClr1(m_crLine1);
//	CBrush	brushClr2(m_crLine2);

	while( pLine ){
		rcLineDC.left	= ptOffset.x;
		rcLineDC.top	= ptOffset.y - m_nTopOffsetY;
		rcLineDC.right	= rcControlDC.right;
		rcLineDC.bottom	= rcLineDC.top +  pLine->m_szLine.cy;

		rcDrawDC		= rcControlDC&rcLineDC;
		if( !rcDrawDC.IsRectEmpty() ){
			if( m_bLineColors ){
				_Rect rcFillLine = rcDrawDC;
				if( rcLineDC.top == rcDrawDC.top && nLineIndex ){
//					pDC->MoveTo(rcDrawDC.left, rcDrawDC.top);
//					pDC->LineTo(rcDrawDC.right, rcDrawDC.top);
					rcFillLine.top ++;
					}

				// Completely visible last line.
				if( !pLine->m_pNextLine && rcLineDC.bottom == rcDrawDC.bottom ){
//					pDC->MoveTo(rcDrawDC.left,	rcDrawDC.bottom-1);
//					pDC->LineTo(rcDrawDC.right, rcDrawDC.bottom-1);
					rcFillLine.bottom --;
					}

				pDC->FillSolidRect(rcFillLine, ((nLineIndex%2) ? m_crLine2 : m_crLine1));
				}
			pLine->Draw(pDC, rcLineDC, rcDrawDC, this);
			}

		pLine			= pLine->m_pNextLine;
		ptOffset.y		+= rcLineDC.Height();
		if( ptOffset.y >= (rcControlDC.bottom + m_nTopOffsetY) )
			break;

		nLineIndex ++;
		}

//	penBorder.DeleteObject();
//	brushClr1.DeleteObject();
//	brushClr2.DeleteObject();
	}

void
RenderLine::Draw(_DC* pDC, _Rect rcLine, _Rect rcDraw, ESChildControl* pControl){
	RenderItem* pItem			= m_pFirstItem;
	_Rect		rcItem, rcInvalid;
	int			nOffsetX		= rcLine.left;
	int			nClientWidth	= pControl->GetClientWidth();
	while( pItem ){
		if( pItem->m_fPosX > 0.0f )
			nOffsetX = (pItem->m_fPosX*((float)nClientWidth)) + rcLine.left;

		rcItem.left			= nOffsetX + pItem->m_ptOffset.x;
		rcItem.top			= rcLine.top + pItem->m_ptOffset.y;
		if( pItem->m_fCX > 0.0f ){
			rcItem.right	= rcItem.left + (pItem->m_fCX*((float)nClientWidth));
			rcItem.bottom	= rcItem.top + pItem->m_szItem.cy;
			}
		else{
			rcItem.right	= rcItem.left + pItem->m_szItem.cx;
			rcItem.bottom	= rcItem.top + pItem->m_szItem.cy;
			}

		rcInvalid = rcItem&rcDraw;
		if( !rcInvalid.IsRectEmpty() )
			pItem->Draw(pDC, rcItem, rcInvalid, pControl);

		if( pItem->m_bBreakLine )
			nOffsetX = rcLine.left;
		else
			nOffsetX += rcItem.Width();
		pItem = pItem->m_pNext;
		}
	}

void
RenderItemText::Draw(_DC* pDC, _Rect rcItem, _Rect rcDraw, ESChildControl* pControl){
	if( !m_pStyle ) return;
	_Font*	pFontOld	= pDC->SelectObject(&m_pStyle->m_font);
	int		nBkModeOld	= pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crText);

	_Rect rcText;
	if( m_nHAlign == Alignment::HAlign_Center ){
		rcText.left		= rcItem.left + (rcItem.Width() - m_szText.cx)/2;
		rcText.right	= rcText.left + m_szText.cx;
		}
	else{
		if( m_nHAlign == Alignment::HAlign_Right ){
			rcText.left		= rcItem.right - m_szText.cx;
			rcText.right	= rcItem.right;
			}
		else{
			rcText.left		= rcItem.left;
			rcText.right	= rcItem.left + m_szText.cx;
			}
		}

	if( m_nVAlign == Alignment::VAlign_Middle ){
		rcText.top		= rcItem.top + (rcItem.Height() - m_szText.cy)/2;
		rcText.bottom	= rcText.top + m_szItem.cy;
		}
	else{
		if( m_nVAlign == Alignment::VAlign_Bottom ){
			rcText.top		= rcItem.bottom - m_szText.cy;
			rcText.bottom	= rcItem.bottom;
			}
		else{
			rcText.top		= rcItem.top;
			rcText.bottom	= rcItem.top + m_szText.cy;
			}
		}
		
	_string sText = m_sText;
	if( sText.length() )
		ESLocalizationMan::ParseText(sText);
	pDC->ExtTextOut     (rcText.left, rcText.top, ETO_CLIPPED, rcDraw, sText.c_str());
	pDC->SetBkMode		(nBkModeOld);
	pDC->SelectObject	(pFontOld);
	}

void
RenderItemImage::Draw(_DC* pDC, _Rect rcItem, _Rect rcDraw, ESChildControl* pControl){
	}

Serializable*
ESSimpleRenderView::GetTextStyleByName(const TCHAR* lpszName){
	_string sVal = lpszName;
	ESSimpleRenderView_TextStyle* pStyle = m_listTextStyles.FindOneVariable(_T("StyleName"), (void*)&sVal);
	return pStyle;
	}

Serializable*
ESSimpleRenderView::GetImageStyleByName(const TCHAR* lpszName){
	_string sVal = lpszName;
	ESSimpleRenderView_ImageStyle* pStyle = m_listImageStyles.FindOneVariable(_T("StyleName"), (void*)&sVal);
	return pStyle;
	}

bool
ESSimpleRenderView::CalcLineSize(RenderLine* pLine){
	if( !pLine ) return false;
	_Size		szLine(0, 0);
	RenderItem* pItem			= pLine->m_pFirstItem;
	int			nLineWidthMax	= -1;
	bool		bBreakLine		= false;

	while( pItem ){
		if( bBreakLine ){
			szLine.cy	+= max(szLine.cy, pItem->m_szItem.cy);
			szLine.cx	= pItem->m_szItem.cx;
			}
		else{
			szLine.cy	= max(szLine.cy, pItem->m_szItem.cy);
			szLine.cx	+= pItem->m_szItem.cx;
			}

		if( pItem->m_bBreakLine ){
			bBreakLine = true;
			if( nLineWidthMax < szLine.cx )
				nLineWidthMax = szLine.cx;
			szLine.cx = 0;
			}
		else
			bBreakLine = false;
		pItem = pItem->m_pNext;
		}

	// In case when line break occures.
	if( nLineWidthMax != -1 )
		szLine.cx = nLineWidthMax;
	pLine->m_szLine	= szLine;
	return true;
	}

RenderLine*
ESSimpleRenderView::GetLineByOffset(int& nOffsetY){
	if( nOffsetY == m_nTopFullOffsetY ){
		nOffsetY = m_nTopOffsetY;
		return m_pTopLine;
		}

	RenderLine* pLine = m_pRootLine->m_pNextLine;
	if( nOffsetY <= 0 ){
		nOffsetY = 0;
		return pLine;
		}

	m_nTopFullOffsetY = nOffsetY;
	while( pLine ){
		if( pLine->m_szLine.cy > nOffsetY ){
			return pLine;
			}
		nOffsetY	-= pLine->m_szLine.cy;
		pLine		= pLine->m_pNextLine;
		}
	m_nTopFullOffsetY = 0;
	return NULL;
	}

bool
ESSimpleRenderView::IsLineVisible(RenderLine* pLine, _Rect& rcLine){
	if( !m_pTopLine ){ 
		int nOffsetY = 0;
		if( m_pVScroll && m_pVScroll->IsVisible() ){
			nOffsetY = m_pVScroll->GetScrollPos();
			GetLineByOffset(nOffsetY);
			}
		else
			m_pTopLine = m_pRootLine->m_pNextLine;
		}
	if( !m_pTopLine ) return false;

	_Rect rcClient	= GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	if( m_pVScroll && m_pVScroll->IsVisible() )
		rcClient.right -= m_pVScroll->GetClientWidth();

	_Rect rcLineLoop;
	rcLineLoop.left		= rcClient.left;
	rcLineLoop.right	= rcClient.right;
	rcLineLoop.top		= rcClient.top;
	rcLineLoop.bottom	= rcLineLoop.top + (pLine->m_szLine.cy - m_nTopOffsetY);

	RenderLine* pLineLoop = m_pTopLine;
	while( 1 ){
		if( rcLineLoop.top >= rcClient.bottom )
			break;
		if( pLine == pLineLoop ){
			rcLine = rcLineLoop;
			rcLine.bottom = min(rcLine.bottom, rcClient.bottom);
			return true;
			}
		pLineLoop = pLineLoop->m_pNextLine;
		if( !pLineLoop )
			break;
		rcLineLoop.top		= rcLineLoop.bottom;
		rcLineLoop.bottom	+= pLine->m_szLine.cy;
		}
	return false;
	}

bool
ESSimpleRenderView::AddTextStyle(_string sName, const _string sFontName, bool bItalic, bool bUnderLine, bool bBold, int nFontHeight){
	ESSimpleRenderView_TextStyle* pStyle	= new ESSimpleRenderView_TextStyle();
	pStyle->m_bItalic						= bItalic;
	pStyle->m_bUnderLine					= bUnderLine;
	pStyle->m_bBold							= bBold;
	pStyle->m_nFontHeight					= nFontHeight;
	pStyle->m_sStyleName					= sName;
	pStyle->m_sFontName						= sFontName;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight			= nFontHeight;
	_tcscpy_(lf.lfFaceName, sFontName.c_str());
	lf.lfWeight			= bBold ? FW_BOLD : FW_NORMAL;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfItalic			= bItalic;
	lf.lfUnderline		= bUnderLine;
	lf.lfStrikeOut		= 0;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	pStyle->m_font.CreateFontIndirect(&lf);

	m_listTextStyles.Add(pStyle);
	return true;
	}

bool
ESSimpleRenderView::AddImageStyle(_string sName, _string sImageFile){
	ESSimpleRenderView_ImageStyle* pStyle	= new ESSimpleRenderView_ImageStyle();
	pStyle->m_sImageFile					= sImageFile;
	pStyle->m_pImage						= MercuryGUI::GetInstance()->LoadImage(sImageFile);
	/*
	if( pStyle->m_pImage )
		pStyle->m_szImage.SetSize(pStyle->m_pImage->GetWidth(), pStyle->m_pImage->GetHeight());
*/
	m_listImageStyles.Add(pStyle);
	return true;
	}

void
ESSimpleRenderView::AddText(_string sStyleName, _string sText, bool bLineBreak, COLORREF crText, _Size szText, float fCX, float fPosX /*= 0.0f*/, int nVAlign, int nHAlign, bool bRedraw){
	ESSimpleRenderView_TextStyle* pStyle = (ESSimpleRenderView_TextStyle*)GetTextStyleByName(sStyleName.c_str());
	if( !pStyle ) return;
	/*
	if( !m_pLastLine ){
		m_pLastLine					= new RenderLine();
		m_pRootLine->m_pNextLine	= m_pLastLine;
		m_pLastLine->m_bLineBreak	= bLineBreak;
		bLineBreak					= false;
		}*/

	CStringArray arrStrings;
	StringHelper::Split(&sText, _T("\r\n"), arrStrings);

	RenderItemText* pItemNew	= NULL;
	RenderItem*		pItemPrev	= NULL;
	RenderLine*		pLine		= m_pLastLine;
	bool			bNewLine	= false;

	if( !pLine || pLine->m_bLineBreak ){
		RenderLine* pNewLine	= new RenderLine();
		pNewLine->m_pPrevLine	= pLine;

		if( !pLine ){
			m_pLastLine					= pNewLine;
			m_pRootLine->m_pNextLine	= m_pLastLine;
			m_pLastLine->m_bLineBreak	= bLineBreak;
			}
		else{
			pLine->m_pNextLine			= pNewLine;
			}

		pLine					= pNewLine;
		bNewLine				= true;
		}

	pItemPrev = pLine->m_pFirstItem;
	if( pItemPrev ){
		while(pItemPrev->m_pNext)
			pItemPrev = pItemPrev->m_pNext;
		}

	int nHeightOld	= pLine->m_szLine.cy; 
	for(int i=0; i<arrStrings.size(); i++){
		RenderItemText* pItemNew	= new RenderItemText();
		pItemNew->m_pLine			= pLine;
		pItemNew->m_pStyle			= pStyle;
		pItemNew->m_sText			= arrStrings.at(i);
		pItemNew->m_sTextLocal		= pItemNew->m_sText;
		ESLocalizationMan::ParseText(pItemNew->m_sTextLocal);
		pItemNew->m_nHAlign			= nHAlign;
		pItemNew->m_nVAlign			= nVAlign;
		pItemNew->m_crText			= crText;
		pItemNew->m_fPosX			= fPosX;
		pItemNew->m_fCX				= fCX;

		pStyle->m_font.GetTextSize(&pItemNew->m_sTextLocal, pItemNew->m_szText);
		pItemNew->m_szItem			= pItemNew->m_szText;
		pItemNew->m_szItem.cy		+= 2*m_nTextMarginTopBottom;
		
		if( i > 0 ){
			// Break line on previous item.
			pItemPrev->m_bBreakLine	= true;
			// Set offset of item.
			pItemNew->m_ptOffset.SetPoint(0, pLine->m_szLine.cy);
			// Set size of line height.
			pLine->m_szLine.cy		+= pItemNew->m_szText.cy;
			}
		else{
			pLine->m_szLine.cx		+= pItemNew->m_szItem.cx;
			pLine->m_szLine.cy		= max(pLine->m_szLine.cy, pItemNew->m_szItem.cy);
			}
		
		if( pItemPrev )
			pItemPrev->m_pNext = pItemNew;
		else
			pLine->m_pFirstItem = pItemNew;
		pItemNew->m_pPrev = pItemPrev;
		pItemPrev = pItemNew;		
		}

	CalcLineSize(pLine);
	if( bNewLine ){
		m_nContentHeight	+= pLine->m_szLine.cy;
		m_pLastLine			= pLine;
		}
	else
		m_nContentHeight	+= (pLine->m_szLine.cy - nHeightOld);

	m_nTopFullOffsetY	= 0;
	if( m_szView.cy != m_nContentHeight ){
		m_szView.cy		= m_nContentHeight;
		if( IsCreated() )
			AdjustScrollBarPos(bRedraw);
		}

	pLine->m_bLineBreak = bLineBreak;
	if( bRedraw ){
		_Rect rcLine;
		if( IsLineVisible(pLine, rcLine) )
			RedrawRect(rcLine, false);
		//Redraw(true);
		}
	}

bool
ESSimpleRenderView::UpdateText(void* pText, _string sText, COLORREF crText, bool bRedraw){
	return false;
	}

bool
ESSimpleRenderView::AddImage(_string sStyleName, _Size szImage, float fCX, float fPosX, int nVAlign, int nHAlign, bool bRedraw){
	ESSimpleRenderView_ImageStyle* pStyle = (ESSimpleRenderView_ImageStyle*)GetImageStyleByName(sStyleName.c_str());
	if( !pStyle ) return false;
	if( !m_pLastLine ){
		m_pLastLine				= new RenderLine();
		m_pRootLine->m_pNextLine= m_pLastLine;
		}

	RenderItemImage* pItemNew	= new RenderItemImage();
	pItemNew->m_pLine			= m_pLastLine;
	pItemNew->m_pStyle			= pStyle;
	pItemNew->m_nVAlign			= nVAlign;
	pItemNew->m_nHAlign			= nHAlign;
	pItemNew->m_szItem			= szImage;
	pItemNew->m_fPosX			= fPosX;
	pItemNew->m_fCX				= fCX;

	if( szImage.cx <= 0 || szImage.cy <= 0 )
		pItemNew->m_szItem	= pStyle->m_szImage;
	else
		pItemNew->m_szItem	= szImage;

	RenderItem* pItem = m_pLastLine->m_pFirstItem;
	if( pItem ){
		while(pItem->m_pNext)
			pItem = pItem->m_pNext;
		pItem->m_pNext		= pItemNew;
		pItemNew->m_pPrev	= pItem;
		}
	else
		m_pLastLine->m_pFirstItem = pItemNew;

	int nHeightOld		= m_pLastLine->m_szLine.cy; 
	CalcLineSize(m_pLastLine);
	m_nContentHeight	+= m_pLastLine->m_szLine.cy;
	m_nTopFullOffsetY	= 0;
	m_szView.cy			= m_nContentHeight;
	if( IsCreated() )
		AdjustScrollBarPos(false);
	return true;
	}

void
ESSimpleRenderView::ClearContent(bool bRedraw){
	RenderLine* pLine = m_pRootLine->m_pNextLine;
	while( pLine ){
		RenderItem* pItem = pLine->m_pFirstItem;
		while( pItem ){
			RenderItem* pTemp = pItem->m_pNext;
			delete pItem;
			pItem = pTemp;
			}
		RenderLine* pTempLine = pLine->m_pNextLine;
		delete pLine;
		pLine = pTempLine;
		}

	m_pRootLine->m_pNextLine	= NULL;
	m_pLastLine					= NULL;
	m_pTopLine					= NULL;
	m_nContentHeight			= 0;
	m_szView.cy					= 0;
	if( IsCreated() )
		AdjustScrollBarPos(false);

	if( bRedraw )
		Redraw(true);
	}

void	
ESSimpleRenderView::OnMouseHover(_Point pt, UINT nFlags){
	}

BOOL	
ESSimpleRenderView::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	return 0;
	}

void	
ESSimpleRenderView::OnMouseLeave(UINT nFlags){
	}

void	
ESSimpleRenderView::OnMouseEnter(UINT nFlags){
	}

void
ESSimpleRenderView::OnLButtonDown(_Point pt, UINT nFlags){
	}

void
ESSimpleRenderView::OnLButtonUp(_Point pt, UINT nFlags){
	}

void	
ESSimpleRenderView::OnLButtonClick(_Point pt, UINT nFlags){
	}

void	
ESSimpleRenderView::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESSimpleRenderView::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESSimpleRenderView::OnLButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESSimpleRenderView::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESSimpleRenderView::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void
ESSimpleRenderView::OnDeserializationCompleted(){
	// Create font and load images. {{
	for(int i=0; i<m_listTextStyles.GetCount(); i++){
		ESSimpleRenderView_TextStyle* pStyle = m_listTextStyles.GetAt(i);
		if( pStyle && pStyle->m_sFontName.length() ){
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			lf.lfHeight			= pStyle->m_nFontHeight;
			_tcscpy_(lf.lfFaceName, pStyle->m_sFontName.c_str());
			lf.lfWeight			= pStyle->m_bBold ? FW_BOLD : FW_NORMAL;
			lf.lfCharSet		= ANSI_CHARSET;
			lf.lfItalic			= pStyle->m_bItalic;
			lf.lfUnderline		= pStyle->m_bUnderLine;
			lf.lfStrikeOut		= 0;
			lf.lfQuality		= PROOF_QUALITY;
			lf.lfPitchAndFamily = DEFAULT_PITCH;
			pStyle->m_font.CreateFontIndirect(&lf);
			}
		}

	MercuryGUI* pGUI = MercuryGUI::GetInstance();
	for(int i=0; i<m_listTextStyles.GetCount(); i++){
		ESSimpleRenderView_ImageStyle* pStyle = m_listImageStyles.GetAt(i);
		if( pStyle && pStyle->m_sImageFile.length() ){
			pStyle->m_pImage = pGUI->LoadImage(pStyle->m_sImageFile);
			if( pStyle->m_pImage )
				pStyle->m_szImage.SetSize(pStyle->m_pImage->GetWidth(), pStyle->m_pImage->GetHeight());
			}
		}
	// }}
	}

void
ESSimpleRenderView::OnLocalizeStrings(){
	}