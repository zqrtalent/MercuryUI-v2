// ESChildTextBox.cpp : implementation file
//

#include "ESChildTextBox.h"
#include "../Utility/StringHelper.h"
//#include "../EmpbedeedFont/EmbedeedFontMan.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildTextBox

ESChildTextBox::ESChildTextBox(){
	m_sClassName			= _T("ESChildTextBox");
	m_bPassword				= false;
	m_bReadOnly				= false;
	m_bLogicalPos			= false;
	m_bMultiLine			= false;
	m_nLimitSymbolCt		= 512;
	m_bDisabled				= false;

	m_nSymbolCt				= 0;
	m_bCalcSymbolCt			= true;

	m_nCursorPos			= -1;
	m_nCursorWidth			= 1;

	m_rcOmitBorder.SetRect(0, 0, 0, 0);

	MercuryGUI* pGUI		= MercuryGUI::GetInstance();
	m_crBorder				= pGUI->textbox.m_crBorder;
	m_crFill				= pGUI->textbox.m_crFill;
	m_crAlternateText		= pGUI->textbox.m_crAlternateText;

	m_nTextLineHeight		= 0;
	m_nTopLine				= 0;
	m_nTopLineOffsetY		= 0;
	m_nLeftIndex			= 0;
	m_nLeftOffsetX			= 0;

	m_nSelStart				= -1;
	m_nSelEnd				= -1;
	m_crText				= pGUI->textbox.m_crText;
	m_crSelectedText		= pGUI->textbox.m_crSelectedText;
	m_crSelectedTextBg		= pGUI->textbox.m_crSelectionBg;

	m_fZoomCX				= 1.0f;
	m_fZoomCY				= 1.0f;	

	SetTextFont			(&pGUI->textbox.m_textFont, false);
	SetAlternateTextFont(&pGUI->textbox.m_alternateTextFont);

	m_bCloseBox				= 0;
	m_pImageCloseBoxNormal	= &pGUI->textbox.m_imgCloseBoxNormal;
	m_pImageCloseBoxHover	= &pGUI->textbox.m_imgCloseBoxHover;
	m_pImageCloseBoxPushed	= &pGUI->textbox.m_imgCloseBoxPushed;
	}

BEGIN_DECLARE_VAR(ESChildTextBox, ESChildControl)
	DECLARE_VAR(_T("ESChildTextBox"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Text"),						_T(""), VariableType::VT_String, offsetof(ESChildTextBox,m_sText), false, false, false)
	DECLARE_VAR(_T("Regex string"),				_T(""), VariableType::VT_String, offsetof(ESChildTextBox,m_sRegex), false, false, false)
	DECLARE_VAR(_T("Text Font"),				_T(""), VariableType::VT_Font, offsetof(ESChildTextBox,m_textFont), false, false, false)
	DECLARE_VAR(_T("Text Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildTextBox,m_crText), false, false, false)
	DECLARE_VAR(_T("Selected Text Color"),		_T(""), VariableType::VT_Color32, offsetof(ESChildTextBox,m_crSelectedText), false, false, false)
	DECLARE_VAR(_T("Selected Text Bg"),			_T(""), VariableType::VT_Color32, offsetof(ESChildTextBox,m_crSelectedTextBg), false, false, false)
	DECLARE_VAR(_T("Alternate Text Font"),		_T(""), VariableType::VT_Font, offsetof(ESChildTextBox,m_alternateTextFont), false, false, false)
	DECLARE_VAR(_T("Alternate Text"),			_T(""), VariableType::VT_String, offsetof(ESChildTextBox,m_sAlternateText), false, false, false)
	DECLARE_VAR(_T("Alternate Text Color"),		_T(""), VariableType::VT_Color32, offsetof(ESChildTextBox,m_crAlternateText), false, false, false)
	DECLARE_VAR(_T("Bg Image"),					_T(""), VariableType::VT_Image, offsetof(ESChildTextBox,m_imageBg), false, false, false)
	DECLARE_VAR(_T("OmitBorder"),				_T(""), VariableType::VT_Rect, offsetof(ESChildTextBox,m_rcOmitBorder), false, false, false)
	DECLARE_VAR(_T("Border Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildTextBox,m_crBorder), false, false, false)
	DECLARE_VAR(_T("Fill Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildTextBox,m_crFill), false, false, false)
	DECLARE_VAR(_T("Read Only"),				_T(""), VariableType::VT_Bool, offsetof(ESChildTextBox,m_bReadOnly), false, false, false)
	DECLARE_VAR(_T("Password Mode"),			_T(""), VariableType::VT_Bool, offsetof(ESChildTextBox,m_bPassword), false, false, false)
	DECLARE_VAR(_T("MultiLine"),				_T(""), VariableType::VT_Bool, offsetof(ESChildTextBox,m_bMultiLine), false, false, false)
	DECLARE_VAR(_T("Disabled"),					_T(""), VariableType::VT_Bool, offsetof(ESChildTextBox,m_bDisabled), false, false, false)
	DECLARE_VAR(_T("LimitSymbolCt"),			_T(""), VariableType::VT_Int32, offsetof(ESChildTextBox,m_nLimitSymbolCt), false, false, false)
END_DECLARE_VAR()

ESChildTextBox::~ESChildTextBox(){
	}

bool
ESChildTextBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		// Register variable modify callback methods. {{
		AddVariableModifyCallBack(&m_sText,			(VariableValueModifyCallback)&ESChildTextBox::TextModify);
		AddVariableModifyCallBack(&m_bMultiLine,	(VariableValueModifyCallback)&ESChildTextBox::TextModify);
		AddVariableModifyCallBack(&m_textFont,		(VariableValueModifyCallback)&ESChildTextBox::TextFontModify);
		// }}

		if( !m_textFont.IsNull() )
			SetTextFont(&m_textFont, false);
		
		if( m_sText.length() > 0 )
			SetText(m_sText, false, false);

		/*if( SetTimer(CURSOR_BLINK_TIMER, 100) )
			m_nCursorTimerId = CURSOR_BLINK_TIMER;*/
		}
	return bRet;
	}

void
ESChildTextBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildTextBox::SetFocus(){
	if( !SetSel(0, -1, true, true) )
		m_nCursorPos = 0;
	Redraw(false);
	return true;
	}

void
ESChildTextBox::KillFocus(){
	ClearSelection	(false);
	Redraw			(false);
	}

BOOL
ESChildTextBox::OnOK(){
	if( m_bMultiLine && !m_bReadOnly )
		return 1;
	return 0;
	}

BOOL
ESChildTextBox::OnCancel(){
	return 0; // Not accept
	}

bool
ESChildTextBox::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildTextBox::SetSize(int cx, int cy, bool bRedraw){
	int nCXOld = m_rRect.Width();
	int nCYOld = m_rRect.Height();
	ESChildControl::SetSize(cx, cy, bRedraw);
	if( nCYOld != cy || nCXOld != cx ){
	//	m_nRightIndex = GetRightSymbolIndex();
		}
	GetTextAreaRect();
	return true;
	}

bool
ESChildTextBox::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildTextBox::SetBgImage(_Image* pImage, _Rect rcOmitBorder){
	if( !m_imageBg.IsNull() )
		m_imageBg.Destroy();
	pImage->CopyImage(&m_imageBg);
	m_rcOmitBorder	= rcOmitBorder;
	}

int
ESChildTextBox::GetTextAsInt(){
	if( m_bMultiLine )
		return 0;
	_string sLine;
	GetLineString(0, sLine);
	return _ttoi(sLine.c_str());
	}

double
ESChildTextBox::GetTextAsDouble(){
	if( m_bMultiLine )
		return 0;
	_string sLine;
	GetLineString(0, sLine);
	return _ttof(sLine.c_str());
	}

DateTime
ESChildTextBox::GetTextAsDateTime(){
	DateTime dt(time(NULL));
	return dt;
	}

void
ESChildTextBox::SetTextFont(_Font* pFont, bool bRedraw /*= false*/){
	if( !pFont ) return;

    if( pFont != &m_textFont ){
		pFont->CopyFont(&m_textFont);
    }

	if( !pFont->IsNull() ){
		CalcTextDimensions(&m_sText);

		_string sSymbol		= _T("X");
		_Size szText(0, 0);
		pFont->GetTextSize(&sSymbol, szText);
		m_nTextLineHeight	= szText.cy;

		if( bRedraw )
			Redraw(false);
		}
	}

bool
ESChildTextBox::SetText(_string sText, bool bSelectAll /*= false*/, bool bRedraw /*= true*/){
	m_nCursorPos	= m_nLeftIndex	= m_nLeftOffsetX = 0;
	m_nSelStart		= m_nSelEnd	= -1;
	
	if( !IsMultiLine() ){
		stringEx::Remove(sText, '\r');
		stringEx::Remove(sText, '\n');
		sText		+= _T("\r"); // At the end of single line.
		}
	
	m_sText			= sText;
	m_bCalcSymbolCt	= true;
	CalcTextDimensions	(&sText);
	CalcTextMaxOffsets	();

	if( bSelectAll )
		SetSel(0, -1, false, false);

//	SetCursorPos		(m_sText.length(), true);

	if( bRedraw )
		Redraw(false);
	return true;
	}

bool
ESChildTextBox::SetInt(int nValue, bool bRedraw /*= true*/){
	_string sVal;
	stringEx::Format(sVal, _T("%d"), nValue);
	return SetText(sVal, false, bRedraw);
	}

bool
ESChildTextBox::SetDouble(double dValue, bool bRedraw /*= true*/){
	_string sVal;
	stringEx::Format(sVal, _T("%.4f"), dValue);
	return SetText(sVal, false, bRedraw);
	}

bool
ESChildTextBox::SetFloat(float fValue, bool bRedraw /*= true*/){
	_string sVal;
	stringEx::Format(sVal, _T("%.2f"), fValue);
	return SetText(sVal, false, bRedraw);
	}

bool
ESChildTextBox::SetDateTime(DateTime dtValue, bool bRedraw /*= true*/){
	return false;
	}

_string	
ESChildTextBox::GetText(){
	_string sRet, sLine;
	if( m_bMultiLine ){
		for(int i=0; i<m_arrTextLines.size(); i++){
			sLine = m_arrTextLines.at(i);
			stringEx::Remove(sLine, '\r');
			stringEx::Remove(sLine, '\n');
			sRet += sLine;
			if( i != m_arrTextLines.size() - 1 )
				sRet += _T("\r\n");
			}
		}
	else{
		if( m_arrTextLines.size() > 0 ){
			sRet = m_arrTextLines.at(0);
			stringEx::Remove(sRet, '\r');
			stringEx::Remove(sRet, '\n');
			}
		}
	return sRet;
	}

int
ESChildTextBox::GetCount(){
	if( m_bCalcSymbolCt ){
		m_nSymbolCt = 0;
		for(int i=0; i<m_arrTextLines.size(); i++){
			_string sLine = m_arrTextLines.at(i);
			if( sLine.length() > 0 )
				m_nSymbolCt	+= (sLine.length() - 1);
			}
		m_bCalcSymbolCt = false;
		}
	return m_nSymbolCt;
	}

bool
ESChildTextBox::SetSel(int nSelStart, int nSelEnd, bool bChangeCursorPos, bool bRedraw){
	// Clear old selection.
	ClearSelection(false);

	int nCt	= GetCount() + GetLineCount();
	if( nSelEnd >= nCt || nSelStart >= nCt )
		return false;

	int nCursorPosStart	= GetCursorPosByCharIndex(nSelStart);
	int nCursorPosEnd	= GetCursorPosByCharIndex(nSelEnd > -1 ? nSelEnd :  nCt - 1);
	m_nSelStart			= nCursorPosStart;
	m_nSelEnd			= nCursorPosEnd;

	if( bChangeCursorPos )
		m_nCursorPos = nCursorPosEnd;
	if( bRedraw ) Redraw(false);
	return true;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildTextBox::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcControlDC((m_rRect.left - pRectClient->left) + pRectDC->left, (m_rRect.top - pRectClient->top) + pRectDC->top, m_rRect.Size()), rcInvalid;

	// Zoom text font. {{
	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	if( m_pOwner && m_bLogicalPos )
		m_pOwner->GetZoom(fZoomCX, fZoomCY);

	if( fZoomCX != m_fZoomCX || fZoomCY != m_fZoomCY ){
		if( m_textFontOriginal.IsNull() )
			m_textFont.CopyFont(&m_textFontOriginal);

		_Font fontScaled;
		m_textFontOriginal.StretchedTextFont(&fontScaled, fZoomCX, fZoomCY );
		SetTextFont(&fontScaled, false);

		m_fZoomCX = fZoomCX;
		m_fZoomCY = fZoomCY;
		}
	// }}

	if( !m_imageBg.IsNull() ){
		// Draw bg image. {{
		rcInvalid	= rcControlDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() )
			m_imageBg.RenderImage_OmitBorder(pDC, rcInvalid, rcControlDC, m_rcOmitBorder);
		// }}
		}
	else{
		// Fill background.
		rcInvalid = *pRectDC&rcControlDC;
        if( !rcInvalid.IsRectEmpty() ){
			//pDC->FillSolidRect(rcInvalid, m_crFill);
            pDC->FillRectWithBorderRadius(rcControlDC, 6, 6, m_crFill, &m_crBorder, 1);
        }
    }

	// Draw text. {{
	_Rect	rcText = GetTextAreaRect();
	_Rect	rcTextAreaDC((rcText.left - pRectClient->left) + pRectDC->left, (rcText.top - pRectClient->top) + pRectDC->top, rcText.Size());
	
	_Point	ptText(0, 0);
	_Size	szText(0, 0);

	int 	nCt						= GetCount();
	int		nLineCt					= GetLineCount();
    int		nSelLineStart			= (m_nSelStart >> 16);
    int		nSelLineEnd				= (m_nSelEnd >> 16);
    int		nSelSymbolIndexStart	= (m_nSelStart & 0xFFFF);
    int		nSelSymbolIndexEnd		= (m_nSelEnd & 0xFFFF);
	_string	sLine, sBeforeSel, sSel, sAfterSel;

	if( m_bDisabled )
		nSelLineStart = nSelLineEnd = nSelSymbolIndexStart = nSelSymbolIndexEnd = -1;

	// When only return symbol is in textbox. {{
	if( nLineCt == 1 && nCt == 1 ){
		GetLineString(0, sLine);
		if( sLine == _T("\r") )
			nCt = 0;
		}
	// }}

	if( nCt > 0 ){
		_Font*	pFontOld				= pDC->SelectObject(&m_textFont);
		_Rect	rcLineDC				= rcTextAreaDC;
		int		nCurrentLine			= (m_nCursorPos >> 16);
		bool	bEmbedeedFont			= m_textFont.IsEmbedeedFontAvailable();
		int		nDistanceBetweenSymbols = 0;// bEmbedeedFont ? EmbedeedFontMan::GetDistanceBetweenSymbols() : 0;
		int		nOffsetX				= GetTextWidth(nCurrentLine, 0, m_nLeftIndex) + m_nLeftOffsetX;

		if( nOffsetX > 0 )
			nOffsetX += nDistanceBetweenSymbols;

		if( m_bMultiLine ){
			rcLineDC.top		-= m_nTopLineOffsetY;
			rcLineDC.bottom		-= m_nTopLineOffsetY;
			}

		for(int i=m_nTopLine; i<nLineCt; i++){
			rcLineDC.bottom		= rcLineDC.top + m_nTextLineHeight;
			rcLineDC.right		= rcLineDC.left + m_arrLineWidth.at(i);

			if( rcLineDC.top > rcTextAreaDC.bottom )
				break; // End of the visible line.

			_Rect rcInvalid		= rcTextAreaDC&rcLineDC;
			if( rcInvalid.IsRectEmpty() ){
				rcLineDC.top	= rcLineDC.bottom;
				continue;
				}

			sLine = m_arrTextLines.at(i);
			if( i >= nSelLineStart && i <= nSelLineEnd ){
				int nSelStart	= -1, nSelEnd = -1;
				// Selection at single line.
				if( nSelLineStart == nSelLineEnd ){
					nSelStart	= nSelSymbolIndexStart;
					nSelEnd		= nSelSymbolIndexEnd;
					}
				else{
					// First selection line
					if( i == nSelLineStart ){
						nSelStart	= nSelSymbolIndexStart;
						nSelEnd		= sLine.length() - 1; // Till the end of line.
						}
					else{
						nSelStart	= 0;
						// End selection line.
						if( i == nSelLineEnd )
							nSelEnd	= nSelSymbolIndexEnd;
						else
							nSelEnd	= sLine.length() - 1;
						}
					}

				if( nSelStart > -1 && nSelStart < nSelEnd ){
					if( nSelStart > 0 )
						sBeforeSel = sLine.substr(0, nSelStart);
					sSel		= sLine.substr(nSelStart, (nSelEnd - nSelStart));
					sAfterSel	= sLine.substr(nSelEnd, sLine.length() - nSelEnd);
					}
				else
					sAfterSel = sLine;
				}
			else // No selection.
				sBeforeSel = sLine;

			int nLeft = rcLineDC.left;
			if( GetTextSize(&sBeforeSel, &m_textFont, szText) ){
				pDC->SetBkMode		(TRANSPARENT);
				ptText.SetPoint		(rcLineDC.left - nOffsetX, rcLineDC.top);
				pDC->SetTextColor	(m_crText);
				if( ExtTextOutMy(pDC, ptText.x, ptText.y, ETO_CLIPPED, (rcTextAreaDC&*pRectDC), sBeforeSel) )
					rcLineDC.left	+= (szText.cx + nDistanceBetweenSymbols);
				sBeforeSel.erase();
				}

			if( GetTextSize(&sSel, &m_textFont, szText) ){
				pDC->SetBkMode		(OPAQUE);
				ptText.SetPoint(rcLineDC.left - nOffsetX, rcLineDC.top);
				pDC->SetTextColor	(m_crSelectedText);
				pDC->SetBkColor		(m_crSelectedTextBg);
				if( ExtTextOutMy(pDC, ptText.x, ptText.y, ETO_CLIPPED, (rcTextAreaDC&*pRectDC), sSel) )
					rcLineDC.left	+= (szText.cx + nDistanceBetweenSymbols);
				sSel.erase();
				}

			if( GetTextSize(&sAfterSel, &m_textFont, szText) ){
				pDC->SetBkMode		(TRANSPARENT);
				ptText.SetPoint		(rcLineDC.left - nOffsetX, rcLineDC.top);
				pDC->SetTextColor	(m_crText);
				if( ExtTextOutMy(pDC, ptText.x, ptText.y, ETO_CLIPPED, (rcTextAreaDC&*pRectDC), sAfterSel) )
					rcLineDC.left += (szText.cx + nDistanceBetweenSymbols);
				sAfterSel.erase();
				}

			rcLineDC.left	= nLeft;
			rcLineDC.top	= rcLineDC.bottom;
			}
		pDC->SelectObject(pFontOld);
		}
	else{
		// Draw alternate text. {{
		if( m_sAlternateText.length() > 0 ){
			_Font* pFontOld = pDC->SelectObject(&m_alternateTextFont);
			if( m_alternateTextFont.GetTextSize(&m_sAlternateText, szText) ){
				pDC->SetBkMode		(TRANSPARENT);
				ptText.x = rcTextAreaDC.left;
				ptText.y = rcTextAreaDC.top + (rcTextAreaDC.Height() - szText.cy)/2;
				pDC->SetTextColor	(m_crAlternateText);
				pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, (rcTextAreaDC&*pRectDC), m_sAlternateText.c_str());
				rcTextAreaDC.left += szText.cx;
				}
			pDC->SelectObject(pFontOld);
			}
		}
	
	// Draw cursor. {{
	_Rect rcCursor;
	if( !IsDisabled() && m_pOwner->HasFocus(this) && GetCursorRect(rcCursor) ){
		_Rect		rcCursorDC((rcCursor.left - pRectClient->left) + pRectDC->left, (rcCursor.top - pRectClient->top) + pRectDC->top, rcCursor.Size());
        rcInvalid = *pRectDC&rcCursorDC;
		if( !rcInvalid.IsRectEmpty() /*&& m_bCursorIsVisible*/ )
			pDC->FillSolidRect(rcInvalid, RGB(0, 0, 0));
		}
	// }}
	
	// Draw close box. {{
	if( m_bCloseBox ){
		_Point pt = _Cursor::GetCurrentPos(m_pOwner->GetHWND());
        _Image* pImageBox = m_pImageCloseBoxNormal;
		if( m_rcCloseBox.PtInRect(pt) ){
			if( m_bMouseCaptured )
				pImageBox = m_pImageCloseBoxPushed;
			else
				pImageBox = m_pImageCloseBoxHover;
			}

		// Draw box button. {{
		_Rect		rcBoxDC((m_rcCloseBox.left - pRectClient->left) + pRectDC->left, (m_rcCloseBox.top - pRectClient->top) + pRectDC->top, m_rcCloseBox.Size()), rcInvalid;
        rcInvalid = rcBoxDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() )
			pImageBox->RenderImage(pDC, rcInvalid, rcBoxDC, false);
		}
	// }}
	
	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

BOOL	
ESChildTextBox::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	if( !m_bCloseBox ){
        _Cursor::SetCursor(_Cursor::Beam);
		return 1;
		}

	_Point pt = _Cursor::GetCurrentPos(m_pOwner->GetHWND());
    if( m_rcCloseBox.PtInRect(pt) )
        _Cursor::SetCursor(_Cursor::Hand);
	else
		_Cursor::SetCursor(_Cursor::Beam);
    return 1;
	}

void	
ESChildTextBox::OnMouseLeave(UINT nFlags){
	if( m_bCloseBox && m_rcCloseBox.PtInRect(m_ptMouseDown) )
		RedrawRect(m_rcCloseBox, false);
	}

void	
ESChildTextBox::OnMouseHover(_Point pt, UINT nFlags){
	if( nFlags&MK_LBUTTON && m_bMouseCaptured ){
		_Rect rcTextArea = GetTextAreaRect();
		if( pt.y < rcTextArea.top )
			pt.y = rcTextArea.top + 1;
		if( pt.y > rcTextArea.bottom )
			pt.y = rcTextArea.bottom - 1;

		// Make selection of text.
		int nLine		= 0;
		int nCharIndex	= GetCursorIndexByPoint(pt, nLine);
		int nCursorPos	= ((nLine << 16) | (nCharIndex&0xFFFF));

		if( nCharIndex != -1 ){
			int nCharStart	= GetCharIndexByCursorPos(m_nCursorPos);
			int nCharEnd	= GetCharIndexByCursorPos(nCursorPos);
			if( nCharEnd > nCharStart )
				SetSel(nCharStart, nCharEnd, false, true);
			else{
				if( nCharIndex == m_nCursorPos )
					ClearSelection(true);
				else
					SetSel(nCharEnd, nCharStart, false, true);
				}
			}
		}
	else{
		if( m_bCloseBox && m_rcCloseBox.PtInRect(m_ptHoverPos) )
			RedrawRect(m_rcCloseBox, false);
		m_ptHoverPos = pt;
		}
	}

void
ESChildTextBox::OnLButtonDown(_Point pt, UINT nFlags){
	ESChildControl::OnLButtonDown(pt, nFlags);
	if( m_bDisabled ) return;
	
	if( m_bCloseBox && m_rcCloseBox.PtInRect(pt) ){
		RedrawRect(m_rcCloseBox, false);
		return;
		}

	int nLine		= 0;
	int nCharIndex	= GetCursorIndexByPoint(pt, nLine);
	if( nCharIndex > -1){
		int		nCursorPos		= (nLine << 16)|(nCharIndex&0xFFFF);
		int		nSymbolIndex	= GetCharIndexByCursorPos(nCursorPos);
		// Clear selection.
		bool bCleared = ClearSelection(false);
		if( bCleared ){
			SetCursorPos(nSymbolIndex, false);
			Redraw(false);
			}
		else
			SetCursorPos(nSymbolIndex, true);
		}
	}

void
ESChildTextBox::OnLButtonUp(_Point pt, UINT nFlags){
	if( m_bDisabled ){
		ESChildControl::OnLButtonUp(pt, nFlags);
		return;
		}
	if( m_bCloseBox && m_rcCloseBox.PtInRect(m_ptMouseDown) )
		RedrawRect(m_rcCloseBox, false);
	ESChildControl::OnLButtonUp(pt, nFlags);
	}

void	
ESChildTextBox::OnLButtonClick(_Point pt, UINT nFlags){
	if( m_bDisabled )
		return;

	if( m_bCloseBox && m_rcCloseBox.PtInRect(m_ptMouseDown) ){
		RedrawRect(m_rcCloseBox, false);
		// Send event.
		SendEvent(this, EventCode_OnClearTextBox);
		}
	}

void	
ESChildTextBox::OnLButtonDblClick(_Point pt, UINT nFlags){
	ESChildControl::OnLButtonDblClick(pt, nFlags);
	if( m_bDisabled ) return;

	if( m_bCloseBox && m_rcCloseBox.PtInRect(m_ptMouseDown) ){
		RedrawRect(m_rcCloseBox, false);
		// Send event.
		SendEvent(this, EventCode_OnClearTextBox);
		return;
		}
	
	// Select complete word. {{
	int nLine	= 0;
	int nIndex	= GetCursorIndexByPoint(pt, nLine);
	_string sWord;
	int nStartIndex = GetCompleteWordByIndex(GetCharIndexByCursorPos(((nLine<<16)|(nIndex&0xFFFF))), sWord);
	if( nStartIndex > -1 && sWord.length() > 0 ){
		SetSel(nStartIndex, (nStartIndex + sWord.length()), true, true);
		}
	// }}
	}

void
ESChildTextBox::OnTimer(UINT nIdEvent){
	m_bCursorIsVisible = !m_bCursorIsVisible;
	_Rect rcCursor;
	if( GetCursorRect(rcCursor) )
		RedrawRect(rcCursor, false);
	}

void
ESChildTextBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	if( m_bDisabled ) return;
    bool bShiftIsPressed	= false;//((GetKeyState(VK_LSHIFT)&0x8000) == 0x8000) || ((GetKeyState(VK_RSHIFT)&0x8000) == 0x8000);
    bool bControlIsPressed	= false;//((GetKeyState(VK_LCONTROL)&0x8000) == 0x8000) || ((GetKeyState(VK_RCONTROL)&0x8000) == 0x8000);
	
	if( bControlIsPressed ){
		switch( nChar ){
			case 'A':
			case 'a':{
					SetSel(0, GetCount(), true, true);
					//SetSel(0, size() - 1, true, true);
					return;
				}
			case 'v':
			case 'V':{
					PasteString(true);
					return;
				}
			case 'c':
			case 'C':{
					CopySelectedString();
					return;
				}
			case 'x':
			case 'X':{
					CutSelectedString(false, true);
					return;
				}
			};
		}

	int nLen, nCurrentSymbolIndex;
	switch( nChar ){
		case VK_LEFT:{
			nLen				= GetCount();
			nCurrentSymbolIndex = GetCharIndexByCursorPos(m_nCursorPos);
			if( bShiftIsPressed ){
				if( !SetCursorPos(nCurrentSymbolIndex - 1, false) )
					return;
				nCurrentSymbolIndex --;
				if( m_nSelStart == -1 )
					SetSel(nCurrentSymbolIndex, nCurrentSymbolIndex + 1, false, true);
				else{
					int nSelStart	= GetCharIndexByCursorPos(m_nSelStart);
					int nSelEnd		= GetCharIndexByCursorPos(m_nSelEnd);

					if( nSelStart < nCurrentSymbolIndex )
						nSelEnd --;
					else{
						if( nSelStart > nCurrentSymbolIndex )
							nSelStart --;
						else{
							// No selection.
							nSelStart = nSelEnd = nCurrentSymbolIndex;
							}
						}

					if( nSelStart != nSelEnd )
						SetSel(nSelStart, nSelEnd, false, true);
					else
						ClearSelection(true);
					}
				}
			else{
				SetCursorPos	(nCurrentSymbolIndex - 1, true);
				ClearSelection	(true);
				}
			break;
			}
		case VK_RIGHT:{
			nLen				= GetCount();
			nCurrentSymbolIndex = GetCharIndexByCursorPos(m_nCursorPos);
			if( bShiftIsPressed ){
				if( !SetCursorPos(nCurrentSymbolIndex + 1, false) )
					return;
				nCurrentSymbolIndex ++;
				if( m_nSelStart == -1 )
					SetSel(nCurrentSymbolIndex-1, nCurrentSymbolIndex, false, true);
				else{
					int nSelStart	= GetCharIndexByCursorPos(m_nSelStart);
					int nSelEnd		= GetCharIndexByCursorPos(m_nSelEnd);
					if( nSelEnd < nCurrentSymbolIndex )
						nSelEnd ++;
					else{
						if( nSelEnd > nCurrentSymbolIndex )
							nSelStart ++;
						else{
							// No selection.
							nSelStart = nSelEnd = nCurrentSymbolIndex;
							}
						}

					if( nSelStart != nSelEnd )
						SetSel(nSelStart, nSelEnd, false, true);
					else
						ClearSelection(true);
					}
				}
			else{
				SetCursorPos	(nCurrentSymbolIndex + 1, true);
				ClearSelection	(true);
				}
			break;
			}
		case VK_DOWN:{
				if( m_bMultiLine ){
					int nLine		= (m_nCursorPos>>16);
					int nCharIndex	= (m_nCursorPos&0xFFFF);
					if( nLine == m_arrTextLines.size() - 1 || m_arrTextLines.size() <= 1 )
						break;
					_Rect rcCursor;
					if( !GetCursorRect(rcCursor) )
						break;

					int nLineNew		= nLine + 1;
					int nCharIndexNew	= GetCursorIndexByPoint(_Point(rcCursor.right, rcCursor.bottom + m_nTextLineHeight/2), nLineNew);
					if( nCharIndexNew == -1 ){
						int nLineSymbolCt = GetLineSymbolCt(nLineNew);
						if( nCharIndexNew >= nLineSymbolCt )
							nCharIndexNew = nLineSymbolCt - 1;
						}
					else{
						int nLineSymbolCt = GetLineSymbolCt(nLineNew);
						nCharIndexNew = nCharIndex;
						if( nCharIndexNew >= nLineSymbolCt )
							nCharIndexNew = nLineSymbolCt - 1;
						}

					if( bShiftIsPressed ){
						//SetSel(0, GetCharIndexByCursorPos(m_nCursorPos), true, true);
						}
					else{
						
						int nCursorPos = (nLineNew << 16)|(nCharIndexNew&0xFFFF);
						SetCursorPos	(GetCharIndexByCursorPos(nCursorPos), true);
						ClearSelection	(true);
						}
					}
				break;
				}
		case VK_UP:{
				if( m_bMultiLine ){
					int nLine		= (m_nCursorPos>>16);
					int nCharIndex	= (m_nCursorPos&0xFFFF);
					if( nLine <= 0 || m_arrTextLines.size() <= 1 )
						break;
					nLine --;
					if( bShiftIsPressed ){
						//SetSel(0, GetCharIndexByCursorPos(m_nCursorPos), true, true);
						}
					else{
						int nLineSymbolCt = GetLineSymbolCt(nLine);
						if( nCharIndex >= nLineSymbolCt )
							nCharIndex = nLineSymbolCt - 1;
						int nCursorPos = (nLine << 16)|(nCharIndex&0xFFFF);
						SetCursorPos	(GetCharIndexByCursorPos(nCursorPos), true);
						ClearSelection	(true);
						}
					}
				break;
				}
		case VK_HOME:{
				if( bShiftIsPressed )
					SetSel(0, GetCharIndexByCursorPos(m_nCursorPos), true, true);
				else{
					SetCursorPos	(0, true);
					ClearSelection	(true);
					}
				break;
				}
		case VK_END:{
				if( bShiftIsPressed && m_bCursorIsVisible && m_nCursorPos > -1 )
					SetSel(GetCharIndexByCursorPos(m_nCursorPos), GetCount() - 1, true, true);
				else{
					SetCursorPos	(0, true);
					ClearSelection	(true);
					}
				break;
				}
		case VK_DELETE:{
				if( m_bReadOnly ) return;
				_string sSel;
				if( GetSelectedString(sSel) && sSel.length() > 0 ){
					CutSelectedString(false, true);
					}
				else{
					if( DeleteCharAtIndex(GetCharIndexByCursorPos(m_nCursorPos), true) )
						SendEvent(this, ChildControlEventCode::EventCode_EditTextChange);
					}
				break;
				}
		case VK_BACK:{
				if( m_bReadOnly ) return;
				_string sSel;
				if( GetSelectedString(sSel) && sSel.length() > 0 ){
					CutSelectedString(false, true);
					}
				else{
					if( SetCursorPos(GetCharIndexByCursorPos(m_nCursorPos) - 1, true) ){
						if( DeleteCharAtIndex(GetCharIndexByCursorPos(m_nCursorPos), true) )
							SendEvent(this, ChildControlEventCode::EventCode_EditTextChange);
						}
					}
				break;
				}
		case VK_RETURN:{
				if( m_bMultiLine && !m_bReadOnly && !m_bPassword ){
					// Insert return cariage.
					_string sInsert = _T("\r\n");
					int nSymbolIndex = GetCharIndexByCursorPos(m_nCursorPos);
					if( InsertStringAtIndex(nSymbolIndex, sInsert, true) ){
						SetCursorPos(nSymbolIndex + 1, true);
						SendEvent(this, ChildControlEventCode::EventCode_EditTextChange);
						}
					}
				else
					SendEvent(this, ChildControlEventCode::EventCode_OnOK);
				break;
				}
		case VK_ESCAPE:{
				SendEvent(this, ChildControlEventCode::EventCode_OnCancel);
				break;
				}
		};
	}

void
ESChildTextBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags){
    /*
	if( !IsEditable() ){
		::MessageBeep(-1);
		return;
		}
	if( nChar < ' ' )	return;

	_string sSel;
	if( GetSelectedString(sSel) && sSel.length() > 0 ){
		// Delete selected string.
		CutSelectedString(false, false);
		}
	
	// Insert char.
	_string sInsert;
	sInsert.insert(sInsert.begin(), (TCHAR)nChar);

	if( m_arrTextLines.size() <= 0 )
		m_nCursorPos = 0;

	int nSymbolIndex = GetCharIndexByCursorPos(m_nCursorPos);
	if( InsertStringAtIndex(nSymbolIndex, sInsert, true) ){
		SetCursorPos(nSymbolIndex + sInsert.length(), true);
		// Send event.
		SendEvent(this, ChildControlEventCode::EventCode_EditTextChange);
		}*/
	}

_Rect
ESChildTextBox::GetTextAreaRect(){
	_Rect rcTextArea	= m_rRect;
	rcTextArea.DeflateRect(1, 1, 1, 1);

	int nTextHeight		= min(rcTextArea.Height(), m_nTextLineHeight);
	if( m_bMultiLine )
		rcTextArea.left		+= 2;
	else{ // Vertical centered text.
		rcTextArea.top		= rcTextArea.top + (rcTextArea.Height() - nTextHeight)/2;
		rcTextArea.bottom	= rcTextArea.top + nTextHeight;
		rcTextArea.left		+= 2;
		}

	if( m_bCloseBox ){
		MercuryGUI* pGUI	= MercuryGUI::GetInstance();
		_Size szImage(pGUI->textbox.m_imgCloseBoxNormal.GetWidth(), pGUI->textbox.m_imgCloseBoxNormal.GetHeight());
		
        m_rcCloseBox.SetRect(rcTextArea.right - szImage.cx, rcTextArea.top + (rcTextArea.Height() - szImage.cy) / 2, szImage);
        rcTextArea.right	-= szImage.cx;
		}
	return rcTextArea;
	}

int
ESChildTextBox::GetCursorIndexByPoint(_Point pt, int& nLineIndex){
	_Rect rcTextArea = GetTextAreaRect();
	if( !rcTextArea.PtInRect(pt) ){
		nLineIndex = 0;
		return -1;
		}

	if( m_arrTextLines.size() == 0 && !GetCount() ){
		nLineIndex = 0;
		return 0;
		}

	int nCursorIndex = 0;
	pt.y			+=	m_nTopLineOffsetY;
	int nOffsetY	=   rcTextArea.top + m_nTextLineHeight;
	nLineIndex		= -1;

	// Determine line index by point.
	for(int i=m_nTopLine; i<m_arrTextLines.size(); i++){
		if( pt.y <= nOffsetY ){
			nLineIndex	= i;
			break;
			}
		nOffsetY += m_nTextLineHeight;
		}

	if( nLineIndex == -1 ){
		nLineIndex = 0;
		return -1;
		}

	// Determine symbol index in certain line.
	_string	sLine		= m_arrTextLines.at(nLineIndex);
	pt.x				+= m_nLeftOffsetX;
	int		nOffsetX	=  rcTextArea.left;
	_string strVisible	= sLine.substr(m_nLeftIndex, sLine.length() - m_nLeftIndex);
	nCursorIndex		= GetSymbolIndexByXOffset(&strVisible, &m_textFont, nOffsetX, pt.x) + m_nLeftIndex;
    return nCursorIndex == -1 ? max((m_nLeftIndex - 1), -1) : nCursorIndex;
	}

bool
ESChildTextBox::ClearSelection(bool bRedraw){
	int nLen	= GetCount();
	int nSelCt	= (m_nSelEnd == -1) ? (nLen - 1) : (m_nSelEnd - m_nSelStart);

	if( m_nSelStart < 0 || nSelCt <= 0 )
		return false; // Selection is not available.
	if( m_nLeftIndex >= (m_nSelStart + nSelCt) )
		return false; // Selection is not visible.

	m_nSelStart	= -1;
	m_nSelEnd	= -1;
	if( bRedraw )
		Redraw(false);
	return true;
	}

void
ESChildTextBox::CalcTextDimensions(_string* pStrText){
	m_arrTextLines	.clear();
	m_arrLineWidth	.clear();
	if( !pStrText || !pStrText->length() )
		return;

	StringHelper::Split(pStrText, _T("\r\n"), m_arrTextLines);

	_Size	szText;
	_string	sLine;
	for(int i=0; i<m_arrTextLines.size(); i++){
		sLine = m_arrTextLines[i];
		if( m_bMultiLine && sLine.length() > 0 && sLine[sLine.length() - 1] != '\r' ){
			sLine += _T("\r");
			m_arrTextLines[i] = sLine;
			}
		// Add line width.
		if( GetTextSize(&sLine, &m_textFont, szText) )
			m_arrLineWidth.push_back(szText.cx);
		else
			m_arrLineWidth.push_back(0);
		}
	}

void
ESChildTextBox::CalcTextMaxOffsets(){
	_Rect	rcTextArea = GetTextAreaRect();
	_Size	szTextView(0, 0);
	int		nLongestLineIndex = 0, nLongestLineCX	= 0;

	for(int i=0; i<m_arrTextLines.size(); i++){
		if( nLongestLineCX < m_arrLineWidth[i] ){
			nLongestLineCX		= m_arrLineWidth[i];
			nLongestLineIndex	= i;
			}
        szTextView.cx = _max(szTextView.cx, m_arrLineWidth.at(i));
		}

	szTextView.cy = m_arrLineWidth.size()*m_nTextLineHeight;
	if( rcTextArea.Height() <= szTextView.cy )
		m_nMaxTopLine = m_nMaxTopLineOffsetY = 0;
	else{
		// Calc max top line offset Y.
		int nTopOffset = m_nTextLineHeight;
		for(int i=0; i<m_arrTextLines.size(); i++){
			if( nTopOffset >= (rcTextArea.Height() - szTextView.cy) ){
				m_nMaxTopLine			= i;
				m_nMaxTopLineOffsetY	= nTopOffset - (rcTextArea.Height() - szTextView.cy);
				break;
				}
			nTopOffset += m_nTextLineHeight;
			}
		}

	if( rcTextArea.Width() <= szTextView.cx ){
		m_nMaxLeftIndex		= 0;
		m_nMaxLeftOffsetX	= 0;
		}
	else
		m_nMaxLeftIndex		= CalcMaxLeftOffsetOfLine(m_nLeftIndex, m_nMaxLeftOffsetX);
	}

int
ESChildTextBox::CalcMaxLeftOffsetOfLine(int nLine, int& nLeftSymbolOffsetX){
	if( nLine >= m_arrTextLines.size() ){
		nLeftSymbolOffsetX = 0;
		return 0;
		}

	_Rect	rcTextArea	= GetTextAreaRect();
	int		nLineWidth	= m_arrLineWidth.at(nLine);
	_string sLine	= m_arrTextLines.at(nLine);

	if( nLineWidth <= rcTextArea.Width() ){
		nLeftSymbolOffsetX = 0;
		return (sLine.length() - 1);
		}
	else{
		int		nWidth	= 0;
		_string sSymbol = _T("");
		_Size	szText(0, 0);

		for(int i=sLine.length()-1; i>=0; i--){
			sSymbol = sLine.substr(i, 1);
			if( GetTextSize(&sSymbol, &m_textFont, szText) )
				nWidth += szText.cx;
		
			if( /*(nLineWidth - nWidth) <= rcTextArea.Width() ||*/ nWidth >= rcTextArea.Width() ){
				nLeftSymbolOffsetX = (nWidth - rcTextArea.Width());
				/*
				if( (nLineWidth - nWidth) <= rcTextArea.Width() )
					nLeftSymbolOffsetX = (rcTextArea.Width() - (nLineWidth - nWidth));
				else
					nLeftSymbolOffsetX = (nWidth - rcTextArea.Width());*/
				return  i;
				}
			}
		}

	nLeftSymbolOffsetX = 0;
	return 0;
	}

int
ESChildTextBox::GetTextWidth(int nLine, int nStartIndex, int nEndIndex){
	if( nLine >= m_arrTextLines.size() || nLine < 0 ) 
		return 0;
	_string	sLine = m_arrTextLines.at(nLine);
	if( nEndIndex == -1 )
		nEndIndex = (sLine.length() - 1);
	_string	sText = sLine.substr(nStartIndex, (nEndIndex - nStartIndex)); 
	if( sText.length() <= 0 ) 
		return 0;
	_Size szText(0, 0);
	GetTextSize(&sText, &m_textFont, szText);
	return szText.cx;
	}

bool
ESChildTextBox::GetTextRect(int nLine, int nStartIndex, int nEndIndex, _Rect& rcText){
	if( nLine >= m_arrTextLines.size() || nLine < 0 )
		return false;
	_string sLine	= m_arrTextLines.at(nLine);
	if( nStartIndex >= sLine.length() || nStartIndex < 0 )
		return false;
	_string	sTextLeft	= sLine.substr(0, nStartIndex);
    _string	sText	= sLine.substr(nStartIndex, (nEndIndex - nStartIndex));
	if( sText.length() <= 0 || nLine < m_nTopLine || nStartIndex >= nEndIndex )
		return false;

	_Size szText(0, 0), szTextLeft(0,0);
	GetTextSize(&sText, &m_textFont, szText);
    GetTextSize(&sTextLeft, &m_textFont, szTextLeft);

	_Rect rcTextArea	= GetTextAreaRect();
	rcText				= rcTextArea;
	rcText.left			+= (szTextLeft.cx - m_nLeftOffsetX);
	rcText.right		= rcText.left;
	rcText.top			+= m_nTopLineOffsetY;
	rcText.bottom		= rcText.top + m_nTextLineHeight;

	for(int i=m_nTopLine; i<m_arrTextLines.size(); i++){
		if( i == nLine )
			break;
		if( rcText.top >= rcTextArea.bottom )
			return false;
		rcText.top		= rcText.bottom;
		rcText.bottom	+= m_nTextLineHeight;
		}

    /*
	CDWordArray arrSymbolWidth;
	GetSymbolWidthArray(&sLine, &m_textFont, arrSymbolWidth);
	for(int i=m_nLeftIndex; i<arrSymbolWidth.size(); i++){
		rcText.right += arrSymbolWidth[i];
		if( rcText.left >= rcTextArea.right )
			break;
		if( i == nEndIndex )
			break;
		if( i < nStartIndex )
			rcText.left = rcText.right;
		}*/

	if( !rcText.IsRectEmpty() ){
		rcText &= rcTextArea;
		return true;
		}
	else 
		if( sText == _T("\r") ){
			rcText.left -= m_nCursorWidth;
			return true;
			}
	return false;
	}

int
ESChildTextBox::GetTextSymbolCount(_string& sText, int& nLineCt){
	nLineCt = 0;
	if( sText.length() <= 0 )
		return 0;

	// Calculate lines. {{
	_Size szText;
	_string	s = sText, sLine;
	int nSymbolCt	= 0;
	int nFind		= s.find(_T("\r\n"), 0);
	if( nFind == -1 && s.length() > 0 )
		nFind = s.length() - 1;
		
	do{
		sLine = s.substr(0, nFind + 1);
		s.erase(0, nFind + 2);

		nSymbolCt += sLine.length();
		nLineCt ++;
		
		nFind =	s.find(_T("\r\n"), 0);
		if( nFind == -1 && s.length() > 0 )
			nFind = s.length() - 1;
		}
	while( nFind != -1 );
	// }}

	return nSymbolCt;
	}

int
ESChildTextBox::GetCursorPosByCharIndex(int nCharIndex){
	if( nCharIndex < 0 ) return -1;
	for(int i=0; i<m_arrTextLines.size(); i++){
		_string sLine = m_arrTextLines.at(i);
		if( nCharIndex < sLine.length() )
			return ((i << 16)|(0xFFFF&nCharIndex));
		nCharIndex -= sLine.length();
		}
	return -1;
	}

int
ESChildTextBox::GetCharIndexByCursorPos	(int nCursorPos){
	int nLine			= (nCursorPos >> 16);
	int nSymbolIndex	= (nCursorPos & 0xFFFF);
	int	nCharIndex		= nSymbolIndex;

	for(int i=0; i<nLine; i++){
		_string sLine = m_arrTextLines.at(i);
		nCharIndex += sLine.length();
		}
	return nCharIndex;
	}

int
ESChildTextBox::GetLineSymbolCt(int nLine){
	_string sLine;
	GetLineString(nLine, sLine);
	return sLine.length();
	}

bool
ESChildTextBox::GetLineString(int nLine, _string& sLine){
	if( nLine < 0 || nLine > m_arrTextLines.size() ){
		sLine.erase();
		return false;
		}
	if( nLine == m_arrTextLines.size() && !nLine )
		return true;
	sLine = m_arrTextLines.at(nLine);
	return true;
	}

bool
ESChildTextBox::EnsureCursorVisibility(bool& bRedrawAll){
	bRedrawAll = false;
	if( m_nCursorPos < 0 ) 
		return false;

	int		nLine			= (m_nCursorPos >> 16);
	int		nCharIndex		= (m_nCursorPos & 0xFFFF);
	_Rect	rcText			= GetTextAreaRect();
	_string	sLine;
	GetLineString(nLine, sLine);

	_Rect rcFullText	(0, rcText.top, GetTextWidth(nLine, 0, -1), rcText.bottom);
	_Rect rcVisibleText;
	rcVisibleText.left		= GetTextWidth(nLine, 0, m_nLeftIndex) + m_nLeftOffsetX;
	rcVisibleText.top		= rcText.top;
	rcVisibleText.right		= rcVisibleText.left + rcText.Width();
	rcVisibleText.bottom	= rcText.bottom;

	_Rect rcCursor;
	rcCursor.top			= rcText.top;
	rcCursor.bottom			= rcText.bottom;
	if( nCharIndex == sLine.length() - 1 ){
		rcCursor.left		= rcFullText.right - m_nCursorWidth;
		rcCursor.right		= rcFullText.right;
		}
	else{
		if( !nCharIndex )
			rcCursor.left	= rcFullText.left;
		else
			rcCursor.left	= GetTextWidth(nLine, 0, nCharIndex);
		rcCursor.right		= rcCursor.left + m_nCursorWidth;
		}

	if( rcCursor.left >= rcVisibleText.left && rcCursor.right <= rcVisibleText.right )
		return true; // Cursor is visible.

	// Scroll left
	if( rcCursor.left < rcVisibleText.left ){
		int		nWidth	= rcVisibleText.left + m_nLeftOffsetX;
		_string sSymbol = _T("");
		_Size	szText(0, 0);

		for(int i=m_nLeftIndex; i>=0; i--){
			if( nWidth <= rcCursor.left ){
				m_nLeftIndex	= i;
				m_nLeftOffsetX	= 0;
				break;
				}

			sSymbol = sLine.substr(i, 1);
			GetTextSize(&sSymbol, &m_textFont, szText);
			nWidth -= szText.cx;
			}
		bRedrawAll = true;
		}
	else{ // Scroll right
		int			nWidth		= m_nLeftOffsetX + (rcCursor.right - rcVisibleText.right);
		_string sSymbol		= _T("");
		_Size		szText(0, 0);

		for(int i=m_nLeftIndex; i<sLine.length(); i++){
			sSymbol = sLine.substr(i, 1);
			GetTextSize(&sSymbol, &m_textFont, szText);	
			nWidth -= szText.cx;
			
			if( nWidth <= 0 ){
				m_nLeftIndex	= i;
				m_nLeftOffsetX	= nWidth ? (szText.cx + nWidth) : 0;
				break;
				}
			}
		bRedrawAll = true;
		}
	return true;
	/*
	if( m_nRightIndex == -1 )
		m_nRightIndex = GetRightSymbolIndex(m_nLeftSymbolOffsetX);

	if( m_nLeftIndex <= m_nCursorPos && m_nRightIndex >= m_nCursorPos  )
		return true; // Is visible.

	int nLoop			= m_nCursorPos;
	int nSymbosWidth	= 0;
	while( nLoop < m_arrSymbolWidth.size() ){
		nSymbosWidth += m_arrSymbolWidth.at(nLoop);
		if( nSymbosWidth >= rcText.Width() ){
			m_nRightIndex			= nLoop;
			m_nLeftIndex			= m_nCursorPos;
			m_nLeftSymbolOffsetX	= 0;
			return true;
			}
		nLoop ++;
		}

	nLoop			= m_nCursorPos < m_arrSymbolWidth.size() ? m_nCursorPos : m_arrSymbolWidth.size() - 1;
	nSymbosWidth	= 0;
	while( nLoop >= 0 ){
		nSymbosWidth += m_arrSymbolWidth.at(nLoop);
		if( nSymbosWidth >= rcText.Width() ){
			m_nRightIndex			= m_nCursorPos - 1;
			m_nLeftIndex			= nLoop;
			m_nLeftSymbolOffsetX	= (nSymbosWidth - rcText.Width());
			return true;
			}
		nLoop --;
		}

	m_nRightIndex			= m_arrSymbolWidth.size();
	m_nLeftIndex			= 0;
	m_nLeftSymbolOffsetX	= 0;*/
	return true;
	}

bool
ESChildTextBox::GetCursorRect(_Rect& rcCursor){
	if( m_nCursorPos <= -1 )
		return false;
	_Rect rcText			= GetTextAreaRect();
	rcCursor.left			= rcText.left - m_nLeftOffsetX;
	rcCursor.top			= rcText.top - m_nTopLineOffsetY;
	rcCursor.right			= rcCursor.left + m_nCursorWidth;
	rcCursor.bottom			= rcCursor.top + m_nTextLineHeight;

	int		nLine			= (m_nCursorPos >> 16);
	int		nSymbolIndex	= (m_nCursorPos & 0xFFFF);
	int		nSymbolCt		= GetLineSymbolCt(nLine);
	
	_Rect rRect;
	// If textbox is empty. {{
	if( !nLine && !nSymbolCt ){
		rcCursor.left	= rcText.left - m_nCursorWidth;
		rcCursor.top	= rcText.top;
		rcCursor.right	= rcCursor.left + m_nCursorWidth;
		rcCursor.bottom = rcText.top + m_nTextLineHeight;
		return true;
		}
	// }}

	if( GetTextRect(nLine, nSymbolIndex, nSymbolIndex + 1, rRect) ){
		//if( nSymbolIndex >= nSymbolCt - 1 )
            rcCursor.SetRect(rRect.left, rRect.top, rRect.left + m_nCursorWidth, rRect.bottom);
		//else
        //    rcCursor.SetRect(rRect.left - m_nCursorWidth, rRect.top, rRect.left, rRect.bottom);
		return true;
		}

	rcCursor.SetRect(0, 0, 0, 0);
	return false;
	}

bool
ESChildTextBox::SetCursorPos(int nSymbolIndex, bool bRedraw){
	int nCursorPos = GetCursorPosByCharIndex(nSymbolIndex);
	if( nCursorPos < 0 )				return false;
	if( m_nCursorPos == nCursorPos )	return true;

	_Rect rcOldCursor;
	GetCursorRect(rcOldCursor);
	m_nCursorPos		= nCursorPos;

	bool bRedrawAll		= false;
	EnsureCursorVisibility(bRedrawAll);

	_Rect rcCursor;
	GetCursorRect(rcCursor);

	m_bCursorIsVisible	= true;
	if( bRedraw ){
		if( bRedrawAll )
			Redraw(false);
		else{
			RedrawRect(rcOldCursor, false);
			RedrawRect(rcCursor, false);
			}
		}
	return true;
	}

bool
ESChildTextBox::CopySelectedString(){
    /*
	_string sSelString;
	GetSelectedString(sSelString);
	if( !sSelString.length() )
		return false;

	if( ::OpenClipboard(*m_pOwner) ){
		if( ::IsClipboardFormatAvailable(CF_TEXT) ){
			HANDLE	hClipBoardData	= GetClipboardData(CF_TEXT);
			BOOL	bFreeLocal		= FALSE;

			if( hClipBoardData ){
				char*	lpszText	= (char*)hClipBoardData;
				::EmptyClipboard();

				HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (sSelString.length() + 1)*sizeof(char)); 
				if( hglbCopy == NULL ){ 
					CloseClipboard(); 
					return false; 
					}

				void* lptstrCopy = GlobalLock(hglbCopy); 
				memcpy(lptstrCopy, sSelString.c_str(), (sSelString.length() + 1)*sizeof(char)); 
				GlobalUnlock(hglbCopy); 

				// Place the handle on the clipboard. 
				SetClipboardData(CF_TEXT, hglbCopy);
				}
			}
		else{
			HGLOBAL hglbClipboard = GlobalAlloc(GMEM_MOVEABLE, (sSelString.length() + 1)*sizeof(char)); 
			if( !hglbClipboard ){ 
				CloseClipboard(); 
				return false; 
				}

			void* lptstrCopy = GlobalLock(hglbClipboard); 
			memcpy(lptstrCopy, sSelString.c_str(), (sSelString.length() + 1)*sizeof(char)); 
			GlobalUnlock(hglbClipboard);

			// Place the handle on the clipboard. 
			SetClipboardData(CF_TEXT, hglbClipboard);
			}
		// Close the clipboard. 
		::CloseClipboard();
		return true;
		}*/
	return false;
	}

bool
ESChildTextBox::CutSelectedString(bool bCopyInClipboard, bool bRedraw){
	_string sText;
	if( !GetSelectedString(sText) || sText.length() == 0 )
		return false;

	int		nLineStart		= (m_nSelStart>>16);
	int		nLineEnd		= (m_nSelEnd>>16);
	int		nCharStart		= (m_nSelStart&0xFFFF);
	int		nCharEnd		= (m_nSelEnd&0xFFFF);
	_Size	szText(0, 0);
	_string sLine			= m_arrTextLines.at(nLineStart);

	// Single line.
	if( nLineStart == nLineEnd ){
		sLine.erase(nCharStart, nCharEnd - nCharStart);
		m_arrTextLines[nLineStart] = sLine;
		// Set line width.
		if( GetTextSize(&sLine, &m_textFont, szText) )
			m_arrLineWidth[nLineStart] = szText.cx;
		}
	else{
		// First selection line. {{
		sLine.erase(nCharStart, sLine.length() - nCharStart);
		if( sLine.length() > 0 && sLine.at(sLine.length() - 1) != '\r' )
			sLine += _T("\r");

		int nLinesDeleted = 0;
		if( !sLine.length() ){
			m_arrTextLines.erase(m_arrTextLines.begin() + nLineStart);
			m_arrLineWidth.erase(m_arrLineWidth.begin() + nLineStart);
			nLinesDeleted ++;
			}
		else{
			m_arrTextLines[nLineStart] = sLine;
			if( GetTextSize(&sLine, &m_textFont, szText) )
				m_arrLineWidth[nLineStart] = szText.cx;
			}
		// }}
		
		for(int i=nLineStart+1; i<nLineEnd; i++){
			m_arrTextLines.erase(m_arrTextLines.begin() + (i - nLinesDeleted));
			m_arrLineWidth.erase(m_arrLineWidth.begin() + (i - nLinesDeleted));
			nLinesDeleted ++;
			}

		// Last selection line. {{
		sLine = m_arrTextLines.at(nLineEnd - nLinesDeleted);
		sLine.erase(0, nCharEnd);
		m_arrTextLines[nLineEnd - nLinesDeleted] = sLine;
		if( GetTextSize(&sLine, &m_textFont, szText) )
			m_arrLineWidth[nLineEnd - nLinesDeleted] = szText.cx;
		// }}
		}

	m_bCalcSymbolCt = true;
	m_nCursorPos	= ((nLineStart << 16) | ((nCharStart)&0xFFFF));
	ClearSelection	(false);

	// Copy in clipboard.
	if( bCopyInClipboard ){
		}

	SendEvent(this, ChildControlEventCode::EventCode_EditTextChange);

	if( bRedraw )
		Redraw(false);
	return true;
	}

bool
ESChildTextBox::PasteString(bool bRedraw){
    /*
	if( ::OpenClipboard(*m_pOwner) ){
		if( ::IsClipboardFormatAvailable(CF_TEXT) ){
			HANDLE	hClipBoardData	= GetClipboardData(CF_TEXT);
			BOOL	bFreeLocal		= FALSE;
			if( !hClipBoardData ){
		//		hClipBoardData = GetClipboardDataAlloc(CF_TEXT);
		//		if( hClipBoardData )
		//			bFreeLocal = TRUE;
				}

			if( hClipBoardData ){
				void*	lptstrCopy		= GlobalLock(hClipBoardData);
				int		nSize			= (int)::GlobalSize(hClipBoardData);

				char*	lpszText		= (char*)lptstrCopy;
				_string sTextPaste		= lpszText;

				if( !m_bMultiLine ){
					stringEx::Remove(sTextPaste, '\r');
					stringEx::Remove(sTextPaste, '\n');
					}

				CutSelectedString(false, false);

				int		nLineCt			= 0;
				int		nPasteSymbols	= GetTextSymbolCount(sTextPaste, nLineCt);
				int		nSymbolIndex	= GetCharIndexByCursorPos(m_nCursorPos);
				if( InsertStringAtIndex(nSymbolIndex, sTextPaste, true) ){
					SetCursorPos((nSymbolIndex + nPasteSymbols), true);
					SendEvent(this, ChildControlEventCode::EventCode_EditTextChange);
					}

				::GlobalUnlock(hClipBoardData);
				if( bFreeLocal )
					::LocalFree((HLOCAL)hClipBoardData);
				}
			}
		::CloseClipboard();
		}*/
	return false;
	}

bool
ESChildTextBox::GetSelectedString(_string& sText){
	int nLineStart		= (m_nSelStart>>16);
	int nLineEnd		= (m_nSelEnd>>16);
	int nCharStart		= (m_nSelStart&0xFFFF);
	int nCharEnd		= (m_nSelEnd&0xFFFF);

	_string	sLine;
	if( !(nLineStart >= 0 && nLineEnd <= m_arrTextLines.size() - 1) || (nLineEnd - nLineStart) < 0 )
		return false;

	// Single line.
	if( nLineStart == nLineEnd ){
		if( nCharStart >= nCharEnd )
			return false;
		sLine = m_arrTextLines.at(nLineStart);
		sText = sLine.substr(nCharStart, nCharEnd - nCharStart);
		}
	else{
		sLine = m_arrTextLines.at(nLineStart);
		sText = sLine.substr(nCharStart, sLine.length() - 1);
		sText += _T("\r\n");
		for(int i=nLineStart+1; i<nLineEnd; i++){
			sLine = m_arrTextLines.at(i);
			sText += sLine.substr(0, sLine.length() - 1);
			sText += _T("\r\n");
			}
		sLine = m_arrTextLines.at(nLineEnd);
		sText += sLine.substr(0, nCharEnd);
		}
	return true;
	}

int
ESChildTextBox::GetCompleteWordByIndex(int nSymbolIndex, _string& sWord){
	if( m_bPassword ) return -1;
	int nLen	= GetCount() + GetLineCount();
	if( nSymbolIndex < 0 || nSymbolIndex > nLen ){
		sWord = _T("");
		return -1;
		}

	int		nStartIndex = 0;
	int		nEndIndex	= (nLen - 1);
	_string sText		= GetText();
//	sText.Remove('\n');
	for(int i=nSymbolIndex; i<sText.length(); i++){
		if( sText.at(i) == ' ' || sText.at(i) == '\r'  ){
			nEndIndex = i;
			break;
			}
		}

	if( nSymbolIndex >= sText.length() )
		nSymbolIndex = (sText.length() - 1);
	for(int i=nSymbolIndex; i>=0; i--){
		if( sText.at(i) == ' ' || sText.at(i) == '\r' ){
			nStartIndex = i;
			break;
			}
		}

	sWord = sText.substr(nStartIndex, nEndIndex - nStartIndex);
	return nStartIndex;
	}

bool
ESChildTextBox::DeleteCharAtIndex(int nSymbolIndex, bool bRedraw){
	if( nSymbolIndex < 0 || nSymbolIndex >= GetCount() )
		return false;

	int		nCursorPos	= GetCursorPosByCharIndex(nSymbolIndex);
	int		nLine		= (nCursorPos >> 16);
	int		nCharIndex	= (nCursorPos & 0xFFFF);
	int		nSymbolCt	= GetLineSymbolCt(nLine);
	_string sLine		= m_arrTextLines.at(nLine);
	int		nLineCt		= m_arrTextLines.size();
	_Size	szText;

	// Last symbol at the line.
	if( nCharIndex == nSymbolCt - 1 ){
		if( !nCharIndex ){
			m_arrTextLines.erase(m_arrTextLines.begin() + nLine);
			m_arrLineWidth.erase(m_arrLineWidth.begin() + nLine);
			}
		else{
			if( nLine == nLineCt - 1 )
				return false;
			// Join next line.
			if( sLine.at(sLine.length()-1) == '\r' )
				sLine.erase(sLine.length()-1);
			sLine += m_arrTextLines.at(nLine + 1);
			if( sLine.at(sLine.length()-1) != '\r' )
				sLine += '\r';

			m_arrTextLines[nLine] = sLine;
			m_arrLineWidth[nLine] = m_arrLineWidth.at(nLine) + m_arrLineWidth.at(nLine+1);
			m_arrTextLines.erase(m_arrTextLines.begin() + nLine + 1);
			m_arrLineWidth.erase(m_arrLineWidth.begin() + nLine + 1);
			}
		}
	else{
		sLine.erase(nCharIndex);
		m_arrTextLines[nLine] = sLine;
		// Set line width.
		if( GetTextSize(&sLine, &m_textFont, szText) )
			m_arrLineWidth[nLine] = szText.cx;
		}

	bool bRedrawAll		= false;
	EnsureCursorVisibility(bRedrawAll);
	m_bCalcSymbolCt		= true;

	if( bRedraw )
		Redraw(false);
	return true;
	}

bool
ESChildTextBox::InsertStringAtIndex(int nSymbolIndex, _string sInsert, bool bRedraw){
	int nLen = GetCount();
	if( nSymbolIndex < 0 || nSymbolIndex > nLen || !sInsert.length() )
		return false;
	if( m_nLimitSymbolCt > -1 && m_nLimitSymbolCt < (nLen + sInsert.length()) )
		return false; // Limited symbol count.

	int nCursorPos		= GetCursorPosByCharIndex(nSymbolIndex);
	int nLine			= nCursorPos >> 16;
	int nCharIndex		= nCursorPos & 0xFFFF;
	int nLineSymbolCt	= GetLineSymbolCt(nLine);

	// When arrays are empty. {{
	if( nLine == -1 && !m_arrTextLines.size() ){
		m_arrTextLines.push_back(_T("\r"));
		m_arrLineWidth.push_back(0);
		nLine			= 0;
		nCharIndex		= 0;
		}
	// }}

	// Calculate lines. {{
	_Size	szText;
	_string	sLine, sText = sInsert;

	CStringArray arrLines;
	StringHelper::Split(&sText, _T("\r\n"), arrLines);

	if( arrLines.size() == 1 ){
		// Set line string.
		_string sLineNew;
		GetLineString(nLine, sLineNew);
		sLineNew.insert(nCharIndex, sText);
		m_arrTextLines[nLine] = sLineNew;
		// Set line width.
		if( GetTextSize(&sText, &m_textFont, szText) )
			m_arrLineWidth[nLine] = m_arrLineWidth.at(nLine) + szText.cx;
		}
	else{
		bool	bAppend = true;
		_string sAppendAtLastLine;

		for(int i=0; i<arrLines.size(); i++){
			sLine = arrLines[i];
			if( bAppend && sLine != _T("\r") ){
				_string sLineNew	= m_arrTextLines.at(nLine);
				// Set line string.
				sAppendAtLastLine	= sLineNew.substr(nCharIndex, sLineNew.length() - nCharIndex - 1);
				sLineNew			= sLineNew.substr(0, nCharIndex + 1) + sLine;
				if( !sLineNew.length() )
					sLineNew = _T("\r");
				if( sLineNew.at(sLineNew.length()-1) != '\r' )
					sLineNew += _T("\r");
				m_arrTextLines[nLine] = sLineNew;
				// Set line width.
				if( GetTextSize(&sLineNew, &m_textFont, szText) )
					m_arrLineWidth[nLine] = szText.cx;
				}
			else{
				int nLineInsert = nLine;
				if( sLine == _T("\r") ){
					_string sLineNew	= m_arrTextLines.at(nLine);
					_string sInsertLine	= sLineNew.substr(nCharIndex, sLineNew.length() - nCharIndex - 1);
					sLineNew			= sLineNew.substr(0, nCharIndex);
					if( !sLineNew.length() )
						sLineNew = _T("\r");
					if( sLineNew.at(sLineNew.length()-1) != '\r' )
						sLineNew += _T("\r");
					m_arrTextLines[nLine] = sLineNew;
					// Set line width.
					if( GetTextSize(&sLineNew, &m_textFont, szText) )
						m_arrLineWidth[nLine] = szText.cx;
					sLine = sInsertLine;
					nLineInsert ++;
					}

				if( !sLine.length() )
					sLine = _T("\r");
				if( sLine.at(sLine.length()-1) != '\r' )
					sLine += _T("\r");

				m_arrTextLines.insert(m_arrTextLines.begin() + nLineInsert, sLine);
				// Insert line width.
				if( GetTextSize(&sLine, &m_textFont, szText) )
					m_arrLineWidth.insert(m_arrLineWidth.begin() + nLineInsert, szText.cx);
				}

			bAppend = false;
			nLine	++;
			}

		// Append last inserted line.
		if( sAppendAtLastLine.length() ){
			nLine	--;
			// Set line string.
			_string sLineNew	= m_arrTextLines.at(nLine) + sAppendAtLastLine;
			m_arrTextLines[nLine] = sLineNew;
			// Set line width.
			if( GetTextSize(&sAppendAtLastLine, &m_textFont, szText) )
				m_arrLineWidth[nLine] = m_arrLineWidth.at(nLine) + szText.cx;
			}
		}
	// }}

	bool bRedrawAll		= false;
	EnsureCursorVisibility(bRedrawAll);
	m_bCalcSymbolCt		= true;

	if( bRedraw )
		Redraw(false);
	return true;
	}

bool
ESChildTextBox::GetTextSize(_string* pStr, _Font* pFont, _Size& szText){
	bool bRet = false;
	bool bEmbedeedFont = pFont->IsEmbedeedFontAvailable();
	int nCt = 0;

	for(int i=0; i<pStr->length(); i++){
		if( pStr->at(i) != '\r' && pStr->at(i) != '\n' )
			nCt ++;
		}
    if(nCt == 0)
        return bRet;

	if( m_bPassword ){
		_Size	szSymbol(0, 0);
		_string sSymbol					= _T("*");
		int		nDistanceBetweenSymbols = 0;// EmbedeedFontMan::GetDistanceBetweenSymbols();
		pFont->GetTextSize(&sSymbol, szSymbol);

		if( bEmbedeedFont )
			szText.cx = nCt*szSymbol.cx + (nCt > 0 ? ((nCt -1)*nDistanceBetweenSymbols) : 0);
		else
			szText.cx = nCt*szSymbol.cx;
		bRet = true;
		}
	else{
		bRet = pFont->GetTextSize(pStr, szText);
		}
	return bRet;
	}

bool
ESChildTextBox::ExtTextOutMy(_DC* pDC, int x, int y, UINT nOptions, RECT* lpRect, _string sText){
	bool	bRet	= false;
	int		nCt		= 0;
	if( m_bPassword ){
		for(int i=0; i<sText.length(); i++){
			if( sText.at(i) != '\r' && sText.at(i) != '\n' ){
				sText[i] = '*';
				nCt ++;
				}
			}
		}
	bRet = pDC->ExtTextOut(x, y, nOptions, lpRect, sText.c_str());
	return bRet;
	}

int
ESChildTextBox::GetSymbolIndexByXOffset(_string* pStr, _Font* pFont, int nXTextStart,  int nXSymbolAt){
	int		nRet			= -1;
	bool	bEmbedeedFont	= pFont->IsEmbedeedFontAvailable();

	if( m_bPassword ){
		_Size	szSymbol(0, 0);
		_string sSymbol				= _T("*");
		int		nDistanceBetweenSymbols = 0;// EmbedeedFontMan::GetDistanceBetweenSymbols();
		int		nCt						= 0;
		pFont->GetTextSize(&sSymbol, szSymbol);

		for(int i=0; i<pStr->length(); i++){
			if( pStr->at(i) == '\r' || pStr->at(i) == '\n' )
				continue;

			if( bEmbedeedFont )
				nXTextStart += (szSymbol.cx + nDistanceBetweenSymbols);
			else
				nXTextStart += szSymbol.cx;

			if( nXSymbolAt <= nXTextStart ){
				if( nXSymbolAt <= (nXTextStart - szSymbol.cx/2) )
					nRet = i;
				else
					nRet = i + 1;
				break;
				}
			nCt ++;
			}
		if( nXSymbolAt > nXTextStart )
			nRet = nCt;
		}
	else
		nRet = pFont->GetSymbolIndexByXOffset(pStr, nXTextStart, nXSymbolAt);
	return nRet;
	}

int
ESChildTextBox::GetSymbolWidthArray(_string* pStr, _Font* pFont, CDWordArray& arrSymbolWidth){
	int		nRet = 0, nCt = 0;
	bool	bEmbedeedFont	= pFont->IsEmbedeedFontAvailable();

	if( m_bPassword ){
		for(int i=0; i<pStr->length(); i++){
			if( pStr->at(i) != '\r' && pStr->at(i) != '\n' )
				nCt ++;
			}

		_Size	szSymbol(0, 0);
		_string sSymbol				= _T("*");
		int		nDistanceBetweenSymbols = 0;// EmbedeedFontMan::GetDistanceBetweenSymbols();
		pFont->GetTextSize(&sSymbol, szSymbol);

		for(int i=0; i<nCt; i++)
			arrSymbolWidth.push_back(szSymbol.cx + (bEmbedeedFont ? nDistanceBetweenSymbols : 0));

		nRet = nCt;
		}
	else
		nRet = pFont->GetSymbolWidthArray(pStr, arrSymbolWidth);
	return nRet;
	}


/////////////////////////////////////////////////////////////////////////////
// Variable modify callbacks.

bool
ESChildTextBox::TextModify(ESChildTextBox* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->SetText(pThis->m_sText, false, true);
	return true;
	}

bool
ESChildTextBox::TextFontModify(ESChildTextBox* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->SetTextFont(&pThis->m_textFont, true);
	return true;
	}


