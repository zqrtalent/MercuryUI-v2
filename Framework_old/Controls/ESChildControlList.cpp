// ESChildControlList.cpp : implementation file
//

#include "ESChildControlList.h"
#include "MercuryGUI.h"
#include "ESChildScrollBar.h"
#include "ESChildTextBox.h"

/////////////////////////////////////////////////////////////////////////////
// ESListColumnInfo

ESListColumnInfo::ESListColumnInfo(){
	}

BEGIN_DECLARE_VAR(ESListColumnInfo, Serializable)
DECLARE_VAR(_T("ESListColumnInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Field"),			_T("FIELD"), VariableType::VT_String, offsetof(ESListColumnInfo,m_sField), false, false, false)
	DECLARE_VAR(_T("Title"),			_T("TITLE"), VariableType::VT_String, offsetof(ESListColumnInfo,m_sTitle), false, false, false)
	DECLARE_VAR(_T("HAlign"),			_T("ALIGH"), VariableType::VT_Int32, offsetof(ESListColumnInfo,m_nHAlign), false, false, false)
	DECLARE_VAR(_T("Width"),			_T("WIDTH"), VariableType::VT_Int32, offsetof(ESListColumnInfo,m_nWidth), false, false, false)
	DECLARE_VAR(_T("Width Min"),		_T("WIDTH MIN"), VariableType::VT_Int32, offsetof(ESListColumnInfo,m_nWidthMin), false, false, false)
	DECLARE_VAR(_T("Width Percent"),	_T("WIDTH PERCENT"), VariableType::VT_Float, offsetof(ESListColumnInfo,m_fWidthPercent), false, false, false)
END_DECLARE_VAR()

ESListColumnInfo::~ESListColumnInfo(){
	}


/////////////////////////////////////////////////////////////////////////////
// ESChildControlList

ESChildControlList::ESChildControlList(){
	m_sClassName			= _T("ESChildControlList");
	m_pRowSet				= NULL;
	MercuryGUI*	pGUI		= MercuryGUI::GetInstance();	
	m_nBorderWidth			= 1;
	m_nLineHeight			= 20;
	m_nSelectedRow			= -1;
	m_nTopRowVisible		= 0;

	m_bAllowInplaceEdit		= true;
	m_bAllowSort			= true;
	m_nSortCol				= -1;	// Sorted column index
	m_bAsc					= false;	

	m_bAllowInPlaceEdit		= true;
	m_pInPlaceControl		= NULL;
	m_nRow_InPlaceControl	= -1;
	m_nCol_InPlaceControl	= -1;

	m_bUnderColSizing		= false;
	m_nColumnUnder			= -1;
	m_bColumnSizing			= false;
	m_nColumnSizing			= -1;

	m_crBorder				= pGUI->list.m_crBorder;
	m_crRow1				= pGUI->list.m_crRow1;
	m_crRow2				= pGUI->list.m_crRow2;

	m_crSelectedRow			= pGUI->list.m_crItemSel;
	m_crSelectedRowText		= pGUI->list.m_crItemTextSel;
	m_crDefaultRowText		= pGUI->list.m_crItemTextNormal;

	m_nLineHeight			= pGUI->list.m_nItemHeight;
	
	m_nHeaderCY				= pGUI->header.m_nHeight;
	m_pHdrItemDefault		= &pGUI->header.m_imageItem_normal;
	m_pHdrItemSelected		= &pGUI->header.m_imageItem_sel;
	m_pHdrItemDivider		= &pGUI->header.m_imageItemDiv_sel;

	m_bHeader				= true;
	m_crHeaderText			= RGB(0, 0, 0);

	m_callBackMethod		= NULL; 
	m_pCallBackParam		= NULL;

	pGUI->list.m_fontItemTextNormal.CopyFont(&m_normalItemTextFont);
	pGUI->list.m_fontItemTextSel.CopyFont(&m_selectedItemTextFont);
	pGUI->header.m_fontItemText.CopyFont(&m_headerTextFont);
	// ==========================

	/*
	m_pRowSet				= new VirtualRowSet();
	
	m_pRowSet->AddFieldEx(_T("FIELD1"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD2"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD3"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD4"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD5"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD6"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD7"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD8"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD9"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD10"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD11"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD12"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD13"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("FIELD14"), FieldType::FTChar, 64);
	m_pRowSet->Create();

	int nLoop = 0;
	while( nLoop < 20 ){
		if( m_pRowSet->AddRow() ){
			_string sFormat;
			sFormat.Format(_T("Testament %d"), nLoop);

			m_pRowSet->SetString(_T("FIELD1"), sFormat);
			m_pRowSet->SetString(_T("FIELD2"), sFormat);
			m_pRowSet->SetString(_T("FIELD3"), sFormat);
			m_pRowSet->SetString(_T("FIELD7"), sFormat);
			m_pRowSet->SetString(_T("FIELD9"), sFormat);
			m_pRowSet->SetString(_T("FIELD11"), sFormat);
			m_pRowSet->SetString(_T("FIELD13"), sFormat);
			}
		nLoop ++;
		}
		*/
	/*
	AddColumn(_T("FIELD1"), _T("Column1"), HAlignment::HAlignement_Middle, 140, 40);
	AddColumn(_T("FIELD2"), _T("Column2"), HAlignment::HAlignement_Left, 120, 40);
	AddColumn(_T("FIELD3"), _T("Column3"), HAlignment::HAlignement_Left, 150, 40);
	AddColumn(_T("FIELD4"), _T("Column4"), HAlignment::HAlignement_Left, 140, 40);
	AddColumn(_T("FIELD5"), _T("Column5"), HAlignment::HAlignement_Left, 120, 40);*/
/*
	AddColumn(_T("FIELD6"), _T("Column6"), 0, 130, 40);
	AddColumn(_T("FIELD7"), _T("Column7"), 0, 80, 40);
	AddColumn(_T("FIELD8"), _T("Column8"), 0, 80, 40);
	AddColumn(_T("FIELD9"), _T("Column9"), 0, 80, 40);*/
//	AddColumn(_T("FIELD10"), _T("Column10"), 0, 80, 40);
//	AddColumn(_T("FIELD11"), _T("Column11"), 0, 80, 40);
//	AddColumn(_T("FIELD12"), _T("Column12"), 0, 80, 40);
//	AddColumn(_T("FIELD13"), _T("Column13"), 0, 80, 40);
//	AddColumn(_T("FIELD14"), _T("Column14"), 0, 80, 40);
	// ==========================
	}

BEGIN_DECLARE_VAR(ESChildControlList, ESChildScrollView)
	DECLARE_VAR(_T("ESChildControlList"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Border Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlList,m_crBorder), false, false, false)
	DECLARE_VAR(_T("Row1 Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlList,m_crRow1), false, false, false)
	DECLARE_VAR(_T("Row2 Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlList,m_crRow2), false, false, false)
	DECLARE_VAR(_T("Selected Row"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlList,m_crSelectedRow), false, false, false)
	DECLARE_VAR(_T("Selected Row Text"),		_T(""), VariableType::VT_Color32, offsetof(ESChildControlList,m_crSelectedRowText), false, false, false)
	DECLARE_VAR(_T("Default Row Text"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlList,m_crDefaultRowText), false, false, false)
	DECLARE_VAR(_T("TextFontNormal"),			_T(""), VariableType::VT_Font, offsetof(ESChildControlList,m_normalItemTextFont), false, false, false)
	DECLARE_VAR(_T("TextFontSelected"),			_T(""), VariableType::VT_Font, offsetof(ESChildControlList,m_selectedItemTextFont), false, false, false)
	DECLARE_VAR(_T("Allow Sort"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlList,m_bAllowSort), false, false, false)
	DECLARE_VAR(_T("Columns"),					_T(""), VariableType::VT_None, offsetof(ESChildControlList,m_listColumns), false, true, true)
	DECLARE_VAR(_T("IsHeader"),					_T(""), VariableType::VT_Bool, offsetof(ESChildControlList,m_bHeader), false, false, false)
	DECLARE_VAR(_T("HeaderText Color"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlList,m_crHeaderText), false, false, false)
	DECLARE_VAR(_T("HeaderTextFont"),			_T(""), VariableType::VT_Font, offsetof(ESChildControlList,m_headerTextFont), false, false, false)
	DECLARE_VAR(_T("HeaderCY"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlList,m_nHeaderCY), false, false, false)
	DECLARE_VAR(_T("HeaderImageDefault"),		_T(""), VariableType::VT_Image, offsetof(ESChildControlList,m_imgHeaderDefault), false, false, false)
END_DECLARE_VAR()

ESChildControlList::~ESChildControlList(){
	if( m_pRowSet ){
		delete m_pRowSet;
		m_pRowSet = NULL;
		}

	m_callBackMethod = NULL; 
	m_pCallBackParam = NULL;

	m_normalItemTextFont.DeleteObject();
	m_selectedItemTextFont.DeleteObject();
	m_headerTextFont.DeleteObject();
	}

bool
ESChildControlList::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	m_szView.SetSize(GetColumnsWidth(), m_pRowSet ? m_pRowSet->GetRowCount()*m_nLineHeight : rRect.Height());

	bool bRet = ESChildScrollView::Create(nId, rRect, pOwner, pParent);
	if( bRet && m_pOwner->IsDesignerMode() ){
		// Register variable modify callback methods. {{
		AddVariableModifyCallBack(&m_listColumns,	(VariableValueModifyCallback)&ESChildControlList::ColumnsModify);
		// }}
		}
	if( bRet ){
		if( !m_imgHeaderDefault.IsNull() )
			m_pHdrItemDefault = &m_imgHeaderDefault;
		OnLocalizeStrings();
		}
	return bRet;
	}

bool
ESChildControlList::SetPos(int x, int y, bool bRedraw){
	ESChildScrollView::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlList::SetSize(int cx, int cy, bool bRedraw){
	int nLoop		= 0;
	int nColCt		= GetColumnCount();
	while( nLoop < nColCt ){
		ESListColumnInfo* pCol = m_listColumns.GetAt(nLoop);
		if( !pCol ) {
			nLoop ++;
			continue;
			}
		if( pCol->m_fWidthPercent > 0.0f )
			pCol->m_nWidth = (int)(pCol->m_fWidthPercent*cx);
		nLoop ++;
		}
	ESChildScrollView::SetSize(cx, cy, bRedraw);
	return true;
	}

void
ESChildControlList::SetLogicalPos(float fPosX, float fPosY){
	ESChildScrollView::SetLogicalPos(fPosX, fPosY);
	}

void
ESChildControlList::SetLogicalSize(float fCX, float fCY){
	ESChildScrollView::SetLogicalSize(fCX, fCY);
	}

void
ESChildControlList::OwnerWindowSizeChanged(int cx, int cy){
	_Rect rcView;
	GetItemsViewRect(rcView);
	//m_szView.SetSize(rcView.Width(), m_pRowSet ? m_pRowSet->GetRowCount()*m_nLineHeight : rcView.Height());
	m_szView.SetSize(GetColumnsWidth(), m_pRowSet ? m_pRowSet->GetRowCount()*m_nLineHeight : rcView.Height());
	m_szPage.SetSize(rcView.Width(), rcView.Height());
	ESChildScrollView::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildControlList::SetFocus(){
	return true;
	}

void
ESChildControlList::KillFocus(){
	}

bool
ESChildControlList::Destroy(){
	ESChildScrollView::Destroy();
	return true;
	}

void
ESChildControlList::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect	rcListDC;
	rcListDC.left		= (m_rRect.left - pRectClient->left) + pRectDC->left; 
	rcListDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top; 
	rcListDC.right		= rcListDC.left + m_rRect.Width(); 
	rcListDC.bottom		= rcListDC.top + m_rRect.Height(); 

	// Draw border. {{
	/*
	CPen	penBorder(PS_SOLID, m_nBorderWidth, m_crBorder);
	CPen*	pPenOld	= pDC->SelectObject(&penBorder);
	pDC->MoveTo(rcListDC.left, rcListDC.top);
	pDC->LineTo(rcListDC.right - m_nBorderWidth, rcListDC.top);
	pDC->LineTo(rcListDC.right - m_nBorderWidth, rcListDC.bottom - m_nBorderWidth);
	pDC->LineTo(rcListDC.left, rcListDC.bottom - m_nBorderWidth);
	pDC->LineTo(rcListDC.left, rcListDC.top);
	pDC->SelectObject(pPenOld);
	*/
	pDC->DrawPath(rcListDC, m_nBorderWidth, m_crBorder);
	// }}

	DrawView		(pDC, pRectDC, pRectClient);
	DrawHeader		(pDC, pRectDC, pRectClient);

	// Draw corner when both of scrolbars are visible. {{
	if( m_pVScroll && m_pVScroll->IsVisible() && 
		m_pHScroll && m_pHScroll->IsVisible() ){
		Image* pImgCorner		= &MercuryGUI::GetInstance()->scrollBar.m_imgScrollBarDivider;

		_Rect rcCornerDC;
		rcCornerDC.left		= rcListDC.right - pImgCorner->GetWidth() - m_nBorderWidth;
		rcCornerDC.top		= rcListDC.bottom - pImgCorner->GetHeight() - m_nBorderWidth;
		rcCornerDC.right	= rcCornerDC.left + pImgCorner->GetWidth();
		rcCornerDC.bottom	= rcCornerDC.top + pImgCorner->GetHeight();

		_Rect rcInvalid		= *pRectDC&rcCornerDC;
		if( !rcInvalid.IsRectEmpty() )
			pImgCorner->RenderImage(pDC, rcInvalid, rcCornerDC, false, false);
		}
	// }}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void
ESChildControlList::DrawView(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect	rcView, rcInvalid;
	GetItemsViewRect(rcView);

	_Size	szText;
	_Rect	rcViewDC;
	rcViewDC.left		= (rcView.left - pRectClient->left) + pRectDC->left; 
	rcViewDC.top		= (rcView.top - pRectClient->top) + pRectDC->top; 
	rcViewDC.right		= rcViewDC.left + rcView.Width(); 
	rcViewDC.bottom		= rcViewDC.top + rcView.Height(); 

	int nVScrollPos		= m_pVScroll->GetScrollPos();
	int	nTopRow			= nVScrollPos / m_nLineHeight;

	// Send top row changed event. {{
	if( m_nTopRowVisible != nTopRow ){
		SendEvent(this, ChildControlEventCode::EventCode_ListTopRowChanged, (void*)m_nTopRowVisible/*old top row*/);
		m_nTopRowVisible = nTopRow;
		}
	// }}

	int nLeftCol		= 0;
	int nLeftColOffset	= 0;
	nLeftCol			= CalculateLeftCol(nLeftColOffset);

	int	nLoop			= nTopRow;
	int nCt				= GetColumnCount() > 0 ?  GetRowCount() : 0;
	int nWidth			= 0;
	int nOffsetY		= (rcViewDC.top - nVScrollPos%m_nLineHeight);
	int nOffsetX		= (rcViewDC.left - nLeftColOffset);

	if( !nCt && m_nSelectedRow > -1 )
		m_nSelectedRow = -1;

	_string sItem;
	COLORREF*	pCOlor[]		= {&m_crRow1, &m_crRow2};

	COLORREF	crText			= RGB(0, 0, 0);
	COLORREF	crRowBg			= RGB(0, 0, 0);
	COLORREF*	pCurrColor		= NULL;
	/*
	CFont	fontText;
	PokerGUI::StretchedTextFont(pGUI->m_pLobbyTabFont, &fontText, 1.0f, 1.0f, false);

	CFont	fontTextBold;
	PokerGUI::StretchedTextFont(pGUI->m_pLobbyTabFont, &fontTextBold, 1.0f, 1.0f, false, true);
	*/

	Font*	pFontNormal		= &m_normalItemTextFont;
	Font*	pFontSelected	= &m_selectedItemTextFont;
	Font	fontNormalBold, fontSelectedBold;

	LOGFONT lf;
	pFontNormal->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	fontNormalBold.CreateFontIndirect(&lf);

	pFontSelected->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	fontSelectedBold.CreateFontIndirect(&lf);

	while( nOffsetY < rcViewDC.bottom ){
		_Rect rcItemDC;
		rcItemDC.left		= nOffsetX;
		rcItemDC.top		= nOffsetY;
		rcItemDC.right		= rcViewDC.right;
		rcItemDC.bottom		= std::min(rcItemDC.top + m_nLineHeight, rcViewDC.bottom);

		rcInvalid = rcItemDC & rcViewDC;
		if( rcInvalid.IsRectEmpty() ){
			nOffsetY	+= m_nLineHeight;
			nLoop ++;
			continue;
			}

		Font*	pFont	 = NULL;
		if( nLoop == m_nSelectedRow ){
			crText		= m_crSelectedRowText;
			pCurrColor	= &m_crSelectedRow;
			crRowBg		= m_crSelectedRow;
			pFont		= pFontSelected;
			}
		else{
			crText		= m_crDefaultRowText;
			pCurrColor	= pCOlor[(nLoop /*- nTopRow*/) % 2];
			crRowBg		= (nLoop /*- nTopRow*/) % 2 == 0 ? m_crRow1 : m_crRow2;
			pFont		= pFontNormal;
			}

		// Skip cell rendering.
		if( !m_pRowSet || !m_pRowSet->SetCurrentRow(nLoop) ){
			// Fill row with color.
			pDC->FillSolidRect(rcInvalid, *pCurrColor);

			nOffsetY	+= m_nLineHeight;
			nLoop		++;
			continue;
			}

		bool bBoldText = false;
		// Call row customize callback method.
		if( m_callBackMethod && (m_callBackMethod)(m_pCallBackParam, m_pRowSet, nLoop, (nLoop == m_nSelectedRow), crRowBg, crText, bBoldText) ){
			// Fill row with custom color.
			pDC->FillSolidRect(rcInvalid, crRowBg);

			// Choose bold font object.
			if( bBoldText )
				pFont = (nLoop == m_nSelectedRow) ? &fontSelectedBold : &fontNormalBold;
			}
		else{
			// Fill row with color.
			pDC->FillSolidRect(rcInvalid, *pCurrColor);
			}

		// Render row cell. {{
		int nItem = nLeftCol, nItemCt	= m_listColumns.GetCount();

		_Rect rcCellDC;		
		rcCellDC.left	= nOffsetX;
		rcCellDC.top	= rcItemDC.top;
		rcCellDC.bottom	= rcItemDC.bottom;

//		bool	bBold	= IsItemBold(nLoop);
		
		Font*	pFontOld = pDC->SelectObject(pFont);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);

		while( nItem < nItemCt ){
			ESListColumnInfo*	pItem = m_listColumns.GetAt(nItem);
			if( !pItem ){
				nItem ++;
				break;
				}

			rcCellDC.right		= std::min(rcCellDC.left + pItem->m_nWidth, rcItemDC.right);
			m_pRowSet->GetString((TCHAR*)pItem->m_sField.c_str(), sItem);
			if( !m_pOwner->IsDesignerMode() && sItem.length() )
				ESLocalizationMan::ParseText(sItem);

			// Draw item label. {{
			pFont->GetTextSize(&sItem, szText);
			_Point ptLabel;
			ptLabel.x	= rcCellDC.left + 2;
			ptLabel.y	= rcCellDC.top + (m_nLineHeight - szText.cy)/2;

			if( pItem->m_nHAlign == HAlignment::HAlignement_Middle )
				ptLabel.x = std::max(ptLabel.x, rcCellDC.left + (rcCellDC.Width() - szText.cx)/2);
			else
			if( pItem->m_nHAlign == HAlignment::HAlignement_Right )
				ptLabel.x = std::max(ptLabel.x, (rcCellDC.right - szText.cx) - 3);

			pDC->ExtTextOut(ptLabel.x, ptLabel.y, ETO_CLIPPED, (rcCellDC&*pRectDC), sItem.c_str());
			// }}

			rcCellDC.left	+= pItem->m_nWidth;
			nItem			++;
			}
		pDC->SelectObject	(pFontOld);
		// }}

		nOffsetY	+= m_nLineHeight;
		nLoop ++;
		}

	fontNormalBold	.DeleteObject();
	fontSelectedBold.DeleteObject();

	/*
	// Delete font objects.
	fontText	.DeleteObject();
	fontTextBold.DeleteObject();*/
	}

void
ESChildControlList::DrawHeader(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect	rcHeader, rcInvalid;
	if( !GetHeaderRect(rcHeader) )
		return;

	_Size		szText;
	_Rect	rcHeaderDC;
	rcHeaderDC.left		= (rcHeader.left - pRectClient->left) + pRectDC->left; 
	rcHeaderDC.top		= (rcHeader.top - pRectClient->top) + pRectDC->top; 
	rcHeaderDC.right	= rcHeaderDC.left + rcHeader.Width(); 
	rcHeaderDC.bottom	= rcHeaderDC.top + rcHeader.Height(); 

	int nLeftCol		= 0;
	int nLeftColOffset	= 0;
	nLeftCol			= CalculateLeftCol(nLeftColOffset);

	int	nLoop			= nLeftCol;
	int nCt				= m_listColumns.GetCount();
	int nWidth			= 0;
	int nOffset			= (rcHeaderDC.left - nLeftColOffset);
	int	nWidthDiv		= m_pHdrItemDivider->GetWidth();
	bool bActive		= false;

	_string sTitle;
	Font*		pFont	= &m_headerTextFont;
	/*
	PokerGUI::StretchedTextFont(pGUI->m_pLobbyTabFont, &fontText, 1.0f, 1.0f, false, false);
	PokerGUI::StretchedTextFont(pGUI->m_pLobbyTabFont, &fontTextBold, 1.0f, 1.0f, false, true);
	*/
	_Rect rcOmitRect(0, 0, 0, 0);
	_Rect rcItem	(rcHeader.left, rcHeader.top, rcHeader.left, rcHeader.bottom);

	while( nLoop < nCt ){
		ESListColumnInfo*	pItem = m_listColumns.GetAt(nLoop);
		if( !pItem ){
			nLoop ++;
			continue;
			}
		if( nOffset >= rcHeaderDC.right || nOffset >= pRectDC->right ) 
			break;
		rcItem.right		= std::min(rcItem.left + pItem->m_nWidth - nWidthDiv, rcHeader.right);

		_Rect rcItemDC;
		rcItemDC.left		= nOffset;
		rcItemDC.top		= rcHeaderDC.top;
		rcItemDC.right		= std::min(rcItemDC.left + pItem->m_nWidth - nWidthDiv, rcHeaderDC.right);
		rcItemDC.bottom		= rcHeaderDC.bottom;

		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, &rcItem, FillMode::FillMode_RepeatedImage, RGB(0, 0, 0), m_pHdrItemDefault, _Rect(0, 0, 0, 0), false);

		// Draw item label. {{
//		pFont = (m_nSortCol == nLoop) ? &fontTextBold : &fontText;
		Font* pFontOld = pDC->SelectObject(pFont);
		sTitle = pItem->m_sTitle;
		if( !m_pOwner->IsDesignerMode() && sTitle.length() )
			ESLocalizationMan::ParseText(sTitle);

		pFont->GetTextSize(&sTitle, szText);
		
		_Point ptLabel;
		ptLabel.y = rcItemDC.top + (rcItemDC.Height() - szText.cy)/2;
		ptLabel.x = rcItemDC.left + 3;
		if( pItem->m_nHAlign == HAlignment::HAlignement_Middle )
			ptLabel.x = std::max(ptLabel.x, rcItemDC.left + (rcItemDC.Width() - szText.cx)/2);
		else
		if( pItem->m_nHAlign == HAlignment::HAlignement_Right )
			ptLabel.x = std::max(ptLabel.x, (rcItemDC.right - szText.cx) - 3);
		
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(m_crHeaderText);
		//ImageHelper::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcItemDC, sTitle);
		pDC->ExtTextOut     (ptLabel.x, ptLabel.y, ETO_CLIPPED, (rcItemDC&*pRectDC), sTitle.c_str());
		pDC->SelectObject	(pFontOld);
		// }}

		_Rect rcItemDividerDC;
		rcItemDividerDC.left		= rcItemDC.right;
		rcItemDividerDC.top			= rcItemDC.top;
		rcItemDividerDC.right		= rcItemDC.right + nWidthDiv;
		rcItemDividerDC.bottom		= rcItemDC.bottom;

		rcInvalid = rcItemDividerDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() )
			m_pHdrItemDivider->RenderImage_OmitBorder(pDC, rcInvalid, rcItemDividerDC, rcOmitRect);

		nOffset		+= pItem->m_nWidth;
		nLoop		++;
		rcItem.left = rcItem.right + nWidthDiv;
		}

	rcHeaderDC.left	= nOffset;
	rcInvalid		= rcHeaderDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		m_pHdrItemDefault->RenderImage_OmitBorder(pDC, rcInvalid, rcHeaderDC, rcOmitRect);
	/*
	// Delete font objects.
	fontText	.DeleteObject();
	fontTextBold.DeleteObject();*/
	}

void	
ESChildControlList::OnMouseHover(_Point pt, UINT nFlags){
	m_nColumnUnder = GetHeaderColByPoint(pt, m_bUnderColSizing);
	}

BOOL	
ESChildControlList::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	if( m_bUnderColSizing ){
		//SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		//return 1;
		}
	return 0;
	}

void	
ESChildControlList::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlList::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlList::OnMouseWheel(UINT nFlags, short zDelta, _Point pt){
	ESChildScrollView::OnMouseWheel(nFlags, zDelta, pt);
	/*
	int nOffset = (zDelta / 120)*3;
	int nTopRow = m_nTopRow - nOffset;

	if( nTopRow < 0 ){
		if( m_nTopRowOffset )
			m_nTopRowOffset = 0;
		SetTopRow(0);
		}
	else
		SetTopRow(nTopRow);*/
	}

void
ESChildControlList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	switch( nChar ){
		case VK_UP:{
			if( GotoRow(m_nSelectedRow - 1, true) ){
				SendEvent(this, ChildControlEventCode::EventCode_ListRowChange, (void*)m_nSelectedRow);
				}
			break;
			}
		case VK_DOWN:{
			if( GotoRow(m_nSelectedRow + 1, true) ){
				SendEvent(this, ChildControlEventCode::EventCode_ListRowChange, (void*)m_nSelectedRow);
				}
			break;
			}
		};
	}

void
ESChildControlList::OnLocalizeStrings(){
	}

void
ESChildControlList::OnDeserializationCompleted(){
	ESChildScrollView::OnDeserializationCompleted();

	int nWidth = GetClientWidth();
	for(int i=0; i<m_listColumns.GetCount(); i++){
		ESListColumnInfo* pColInfo = m_listColumns.GetAt(i);
		if( !pColInfo || pColInfo->m_fWidthPercent > 0.0f )
			continue;
		if( pColInfo->m_nWidth > 0 )
			pColInfo->m_fWidthPercent = pColInfo->m_nWidth / ((float)nWidth);
		}
	}

void
ESChildControlList::OnLButtonDown(_Point pt, UINT nFlags){
	int nCol = -1;
	int nRow = GetItemByPoint(pt, nCol);

	if( m_bAllowInPlaceEdit ){
		if( m_pInPlaceControl ){
			if( m_pInPlaceControl->IsVisible() ){
				OnInPlaceControlOk(m_pInPlaceControl, ChildControlEventCode::EventCode_OnOK);
				}
			else
				RemoveInPlaceControl(true);
			}

		CreateInPlaceControl(nRow, nCol);
		}

	if( SelectRow(nRow) ){
		
/*
		if( m_pOwner )
			m_pOwner->SendMessage(WM_LIST_ROW_CHANGED, (WPARAM)GetId(), (LPARAM)m_nSelectedRow);
*/
		}
	else{
		
		}
	}

void
ESChildControlList::OnLButtonUp(_Point pt, UINT nFlags){
	// Sort by column.
	bool	bOverSizing = false;
	int		nCol		= GetHeaderColByPoint(pt, bOverSizing);
	if( !bOverSizing && nCol > -1 && m_bAllowSort ){
		SortByColumnIndex(nCol, true, true);
		}
	}

void	
ESChildControlList::OnLButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlList::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlList::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlList::OnLButtonDblClick(_Point pt, UINT nFlags){
	int nCol	= -1;
	int nItem	= GetItemByPoint(pt, nCol);

	if( m_pOwner && m_nSelectedRow == nItem && nItem != -1 ){
		SendEvent(this, ChildControlEventCode::EventCode_ListRowDblClk, (void*)nItem);
		//m_pOwner->SendMessage(WM_LIST_ROW_DBLCLICK, (WPARAM)GetId(), (LPARAM)m_nSelectedRow);
		}
	}

void	
ESChildControlList::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlList::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

bool
ESChildControlList::AddColumn(const _string sField, const _string sTitle, int nAlign, int nWidth, int nWidthMin, float fWidthPercent){
	ESListColumnInfo* pColumn	= new ESListColumnInfo();
	pColumn->m_sTitle			= sTitle;
	pColumn->m_sField			= sField;
	pColumn->m_nHAlign			= nAlign;
	pColumn->m_nWidth			= nWidth;
	pColumn->m_nWidthMin		= nWidthMin;
	pColumn->m_fWidthPercent	= fWidthPercent;
	m_listColumns.Add(pColumn);

	_Rect rcView;
	GetItemsViewRect(rcView);
	m_szView.SetSize(GetColumnsWidth(), m_pRowSet ? m_pRowSet->GetRowCount()*m_nLineHeight : rcView.Height());
	m_szPage.SetSize(rcView.Width(), rcView.Height());
	AdjustScrollBarPos(false);
	return true;
	}

bool
ESChildControlList::SetColumnWidth(int nCol, int nWidth, bool bRedraw){
	ESListColumnInfo* pItem = m_listColumns.GetAt(nCol);
	if( !pItem )
		return false;
	
	if( nWidth < pItem->m_nWidthMin )
		return false;

	pItem->m_nWidth = nWidth;
	if( bRedraw ){
		RedrawColumn(nCol);
		}
	return true;
	}

bool
ESChildControlList::SelectRow(int nRow){
	if( nRow < 0 || nRow >= GetRowCount() ){
		return false;
		}

	if( nRow == m_nSelectedRow )
		return true;

	int nItemOld	= m_nSelectedRow;
	m_nSelectedRow	= nRow;
	RedrawItem	(nItemOld);
	RedrawItem	(m_nSelectedRow);
	SendEvent(this, ChildControlEventCode::EventCode_ListRowChange, (void*)nRow);
	return true;
	}

bool
ESChildControlList::GotoRow(int nRow, bool bRedraw /*= true*/){
	if( nRow >= GetRowCount() || nRow < 0 )
		return false;

	int nOffsetY	= 0;
	int nVisRowCt	= GetVisibleRowCt(&nOffsetY);
	if( nVisRowCt <= 0 )
		return false;

	int	nTopRow			= m_pVScroll->GetScrollPos() / m_nLineHeight;
	if( nRow >= nTopRow && nRow < (nTopRow + nVisRowCt) ){
		m_nSelectedRow	= nRow;
		if( bRedraw )
			RedrawItemsView();

		SendEvent(this, EventCode_ListRowChange);
		/*
		if( m_pOwner )
			m_pOwner->SendMessage(WM_LIST_ROW_CHANGED, (WPARAM)GetId(), (LPARAM)m_nSelectedRow);
*/
		return true;
		}

	m_pVScroll->SetScrollPos(nRow*m_nLineHeight, true);
	SendEvent(this, EventCode_ListRowChange);
	return true;
	}

int
ESChildControlList::GetVisibleRowCt(int* pOffsetY){
	int nRowCount	= GetRowCount();
	_Rect rcView;
	GetItemsViewRect(rcView);
	
	int nRowCt		= (rcView.Height() / m_nLineHeight);
	int nOffsetY	= rcView.Height() % m_nLineHeight;

	if( nOffsetY > 0 )
		nRowCt		++;

	if( pOffsetY )
		*pOffsetY	=  nOffsetY;
	nRowCt			= std::min(nRowCt, nRowCount);
	return nRowCt;
	}

int
ESChildControlList::GetHeaderColByPoint(_Point pt, bool& bOverSizing){
	_Rect rcHeaderItem;
	bOverSizing		= false;
	if( !GetHeaderRect(rcHeaderItem) ) return -1;

	if( pt.y < rcHeaderItem.top || pt.y > rcHeaderItem.bottom )
		return -1;

	int nLeftCol		= 0;
	int nLeftColOffset	= 0;
	nLeftCol			= CalculateLeftCol(nLeftColOffset);

	int nLoop			= nLeftCol;
	int nCt				= m_listColumns.GetCount();
	int nOffset			= m_rRect.left + m_nBorderWidth;
	int nLeftOffset		= nLeftColOffset;

	while( nLoop < nCt ){
		ESListColumnInfo* pItem = m_listColumns.GetAt(nLoop);
		if( !pItem ){
			nLoop ++;
			continue;
			}

		if( nOffset >= (m_rRect.right - m_nBorderWidth) )
			return -1;

		rcHeaderItem.left	= nOffset;
		rcHeaderItem.right	= rcHeaderItem.left + (pItem->m_nWidth - nLeftOffset);
		if( pt.x >= rcHeaderItem.left && pt.x <= rcHeaderItem.right ){
			if( pt.x > (rcHeaderItem.right - 3) )
				bOverSizing = true;
			return nLoop;
			}

		nLeftOffset	= 0;
		nOffset		= rcHeaderItem.right;
		nLoop		++;
		}
	return -1;
	}

int
ESChildControlList::GetItemByPoint(_Point pt, int& nCol){
	_Rect rcItem;
	GetItemsViewRect(rcItem);
	if( !rcItem.PtInRect(pt) )
		return -1;

	int nTopRow		= m_pVScroll->GetScrollPos()/m_nLineHeight;
	int nTopOffset	= m_pVScroll->GetScrollPos()%m_nLineHeight;
	int nItem		= (pt.y - rcItem.top + nTopOffset) / m_nLineHeight + nTopRow;
	if( nItem >= GetRowCount() )
		return -1;

	int nLeftColOffset	= 0, nLeftCol = CalculateLeftCol(nLeftColOffset);
	int nColWidth		= 0, nOffset = (pt.x - rcItem.left) + nLeftColOffset;
	int nLoop			= nLeftCol;
	while( nLoop < GetColumnCount() ){
		nColWidth	= GetColumnWidth(nLoop);
		if( nOffset < nColWidth ){
			nCol = nLoop;
			return nItem;
			}
		nOffset -= nColWidth;
		nLoop ++;
		}
	nCol = -1;
	return nItem;
	}

void
ESChildControlList::RedrawColumn(int nCol){
	_Rect rcColumn;
	if( GetColumnRect(nCol, rcColumn) ){
		if( m_pOwner )
			RedrawRect(rcColumn, true);
		}
	}

void
ESChildControlList::RedrawItem(int nItem){
	_Rect rcItem;
	if( GetItemRect(nItem, rcItem) ){
		if( m_pOwner )
			RedrawRect(rcItem, true);
		}
	}

void
ESChildControlList::RedrawItemsView(){
	_Rect rcView;
	GetItemsViewRect(rcView);

	if( m_pOwner ){
		RedrawRect(rcView, true);
		}
	}

bool
ESChildControlList::GetHeaderRect(_Rect& rRect){
	if( !m_bHeader ) return false;
	rRect.left		= m_rRect.left + m_nBorderWidth;
	rRect.top		= m_rRect.top + m_nBorderWidth;
	rRect.right	= m_rRect.right - m_nBorderWidth;
	rRect.bottom	= rRect.top + m_nHeaderCY;
	return true;
	}

bool
ESChildControlList::GetItemsViewRect(_Rect& rRect){
	rRect			= GetClientRect();
	rRect.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	if( m_bHeader )
		rRect.top	+= m_nHeaderCY;

	rRect.bottom	= (m_pHScroll && m_pHScroll->IsVisible()) ? rRect.bottom - m_nArrowBtnCY : rRect.bottom;
	rRect.right	= (m_pVScroll && m_pVScroll->IsVisible()) ? rRect.right - m_nArrowBtnCX : rRect.right;
	return true;
	}

int
ESChildControlList::GetColumnWidth(int nCol){
	if( nCol > m_listColumns.GetCount() || nCol < 0 )
		return 0;
	ESListColumnInfo*	pCol = m_listColumns.GetAt(nCol);
	if( pCol )
		return pCol->m_nWidth;
	return 0;
	}

int
ESChildControlList::GetColumnsWidth(){
	ESListColumnInfo*	pCol = NULL;
	int					nRet = 0;

	for(int i=0; i<m_listColumns.GetCount(); i++){
		pCol = m_listColumns.GetAt(i);
		if( pCol )
			nRet += pCol->m_nWidth;
		}
	return nRet;
	}

int
ESChildControlList::CalculateLeftCol(int& nLeftColOffset){
	if( !m_pHScroll || !m_pHScroll->IsVisible() ){
		nLeftColOffset = 0;
		return 0;
		}

	int nHScrollPos	= m_pHScroll->GetScrollPos();
	int nLeftCol	= 0;
	int nCt			= m_listColumns.GetCount();

	while( nLeftCol < nCt ){
		int nWidth = GetColumnWidth(nLeftCol);
		if( nHScrollPos < nWidth ){
			nLeftColOffset = nHScrollPos;
			return nLeftCol;
			}

		nHScrollPos -= nWidth;
		nLeftCol	++;
		}

	nLeftColOffset = 0;
	return 0;
	}

bool
ESChildControlList::GetColumnRect(int nCol, _Rect& rRect){
	_Rect rcColumn;
	rcColumn.left		= m_rRect.left + m_nBorderWidth;
	rcColumn.top		= m_rRect.top + m_nBorderWidth;
	rcColumn.right		= m_rRect.right - m_nBorderWidth;
	rcColumn.bottom		= m_rRect.bottom - m_nBorderWidth;

	int nLeftCol		= 0;
	int nLeftColOffset	= 0;
	nLeftCol			= CalculateLeftCol(nLeftColOffset);

	int nLoop			= nLeftCol;
	int nCt				= m_listColumns.GetCount();
	int nOffset			= m_rRect.left + m_nBorderWidth;
	int nLeftOffset		= nLeftColOffset;

	if( nCol < nLoop )
		return false; // invisible column.

	while( nLoop < nCt ){
		ESListColumnInfo* pItem = m_listColumns.GetAt(nLoop);
		if( !pItem ){
			nLoop ++;
			continue;
			}

		if( nOffset >= (m_rRect.right - m_nBorderWidth) )
			return false;

		rcColumn.left	= nOffset;
		rcColumn.right	= rcColumn.left + (pItem->m_nWidth - nLeftOffset);
		if( nCol == nLoop ){
			rRect = rcColumn;
			return true;
			}

		nLeftOffset	= 0;
		nOffset		= rcColumn.right;
		nLoop		++;
		}
	return false;
	}

bool
ESChildControlList::GetItemRect(int nItem, _Rect& rRect){
	_Rect rcItem;
	GetItemsViewRect(rcItem);

	int nHeaderCY	= m_bHeader ? m_nHeaderCY : 0;
	rcItem.left		= m_rRect.left;
	rcItem.top		= m_rRect.top + nHeaderCY;

	int nTopRow		= m_pVScroll->GetScrollPos()/m_nLineHeight;
	int nTopOffset	= m_pVScroll->GetScrollPos()%m_nLineHeight;

	if( nItem < nTopRow )
		return false;

	int nOffsetY	= rcItem.top + (nItem - nTopRow)*m_nLineHeight;
	if( nOffsetY < rcItem.bottom && nOffsetY >= rcItem.top ){
		rcItem.top		= (nOffsetY - nTopOffset);
		rcItem.bottom	= std::min(rcItem.top + m_nLineHeight + 1,  m_rRect.bottom);
		rcItem.top		= std::max(m_rRect.top + nHeaderCY, rcItem.top);

		rRect			= rcItem;
		return true;
		}
	return false;
	}

bool
ESChildControlList::GetCellRect(int nRow, int nCol, _Rect& rcCell){
	_Rect rcItem;
	_Rect rcClient	= GetClientRect();
	int nHeaderCY	= m_bHeader ? m_nHeaderCY : 0;

	rcItem.left		= rcClient.left;
	rcItem.top		= rcClient.top + nHeaderCY;
	rcItem.bottom	= rcClient.bottom - m_nBorderWidth;
	rcItem.right	= rcClient.right - m_nBorderWidth;

	int nTopRow		= m_pVScroll->GetScrollPos()/m_nLineHeight;
	int nTopOffset	= m_pVScroll->GetScrollPos()%m_nLineHeight;
	if( nRow < nTopRow ) return false;

	int nOffsetY	= rcItem.top + (nRow - nTopRow)*m_nLineHeight;
	if( nOffsetY < rcItem.bottom && nOffsetY >= rcItem.top ){
		rcItem.top			= (nOffsetY - nTopOffset);
		rcItem.bottom		= std::min(rcItem.top + m_nLineHeight,  rcClient.bottom);
		rcItem.top			= std::max(rcClient.top + nHeaderCY, rcItem.top);

		int	nLeftColOffset	= 0;
		int nLeftCol		= CalculateLeftCol(nLeftColOffset);
		int nColCt			= GetColumnCount();
		int nColWidth		= 0;

		if( nCol < nLeftCol || nCol >= nColCt )
			return false;

		int nLoop			= nLeftCol;
		while( nLoop < nColCt ){
			if( nLoop == nCol ){
				rcItem.right	= std::min(rcItem.left + GetColumnWidth(nCol), rcClient.right);
				rcCell			= rcItem;
				return true;
				}

			nColWidth	= GetColumnWidth(nCol);
			if( nLeftColOffset > 0 ){
				nColWidth		-= nLeftColOffset;
				nLeftColOffset	= 0;
				}
			rcItem.left		+= nColWidth;
			nLoop ++;
			}
		}
	return false;
	}

bool
ESChildControlList::SetRowSet(VirtualRowSet* pRowSet, bool bRedraw, bool bClearSort /*= true*/){
	if( !pRowSet ) return false;	
	int nRowCtOld	= m_pRowSet ? m_pRowSet->GetRowCount() : 0;
	int nRowCtNew	= pRowSet ? pRowSet->GetRowCount() : 0;
	if( m_pRowSet ){
		delete m_pRowSet;
		m_pRowSet	= NULL;
		}
	m_pRowSet		= pRowSet;

	if( bClearSort ){
		m_nSortCol	= -1;	// Sorted column index
		m_bAsc		= false;	
		}
	else{
		// To keep same order.
		m_bAsc = !m_bAsc;
		SortByColumnIndex(m_nSortCol, false, false);
		}

	if( nRowCtOld != nRowCtNew ){
		if( m_nSelectedRow != -1 ){
			// Clear selection.
			m_nSelectedRow	= -1;
			// Post event.
			SendEvent(this, ChildControlEventCode::EventCode_ListRowChange, (void*)m_nSelectedRow);
			}
		else // Clear selection.
			m_nSelectedRow	= -1;
		
		// Setup vertical scroll.
		if( m_pVScroll ){
			_Rect rcView;
			GetItemsViewRect(rcView);
			m_pVScroll->SetScrollPos(0, false);
			//m_szView.SetSize(rcView.Width(), pRowSet->GetRowCount()*m_nLineHeight);
			m_szView.SetSize(GetColumnsWidth(), pRowSet->GetRowCount()*m_nLineHeight);
			if( !m_pVScroll->SetScrollInfo(0, m_szView.cy, rcView.Height(), false) )
				m_pVScroll->SetVisible(false, m_pVScroll->IsVisible());
			else{
				if( !m_pVScroll->IsVisible() && IsVisible() )
					m_pVScroll->SetVisible(true, false);
				}
			}
		}
	
	if( bRedraw ){
		Redraw(true);
		if( m_pVScroll && m_pVScroll->IsVisible() )
			m_pVScroll->Redraw(true);
		}
	return true;
	}

void
ESChildControlList::UpdateList(bool bRedraw){
	if( m_nSelectedRow >= m_pRowSet->GetRowCount() )
		m_nSelectedRow = (m_pRowSet->GetRowCount() > 0 ? 0 : -1);

	SendEvent(this, ChildControlEventCode::EventCode_ListRowChange, (void*)m_nSelectedRow);

	// Setup vertical scroll.
	if( m_pVScroll ){
		int nPosOld = m_pVScroll->GetScrollPos();

		_Rect rcView;
		GetItemsViewRect(rcView);
		m_pVScroll->SetScrollPos(0, false);
		//m_szView.SetSize(rcView.Width(), m_pRowSet->GetRowCount()*m_nLineHeight);
		m_szView.SetSize(GetColumnsWidth(), m_pRowSet->GetRowCount()*m_nLineHeight);
		m_szPage.cx = rcView.Width();
		m_szPage.cy = rcView.Height();

		if( !m_pVScroll->SetScrollInfo(0, m_szView.cy, rcView.Height(), false) )
			m_pVScroll->SetVisible(false, m_pVScroll->IsVisible());
		else{
			if( !m_pVScroll->IsVisible() )
				m_pVScroll->SetVisible(true, false);

			int nMaxPos = m_pVScroll->GetLimitPos();
			if( nPosOld > nMaxPos )
				m_pVScroll->SetScrollPos(nMaxPos, false);
			else
				m_pVScroll->SetScrollPos(nPosOld, false);
			}
		}

	if( bRedraw ){
		Redraw(true);
		if( m_pVScroll && m_pVScroll->IsVisible() )
			m_pVScroll->Redraw(true);
		if( m_pHScroll && m_pHScroll->IsVisible() )
			m_pHScroll->Redraw(true);
		}
	}

int
ESChildControlList::GetTopRowIndex(){
	if( !m_pVScroll ) return 0;
	int nVScrollPos		= m_pVScroll->GetScrollPos();
	int	nTopRow			= nVScrollPos / m_nLineHeight;
	return nTopRow;
	}

int
ESChildControlList::GetRowCountPerPage(){
	_Rect rcView;
	GetItemsViewRect(rcView);
	float fRowCtPerPage = rcView.Height() / (float)m_nLineHeight;
	int	nRowCtPerPage	= rcView.Height() / m_nLineHeight;
	if( (fRowCtPerPage - (float)nRowCtPerPage) > 0.09 )
		nRowCtPerPage ++;
	return nRowCtPerPage;
	}

bool
ESChildControlList::EmptyList(bool bRedraw){
	if( !m_pRowSet ) return false;
	int nRowCt = GetRowCount();
	if( m_pRowSet ){
		delete m_pRowSet;
		m_pRowSet = NULL;
		}

	m_nSelectedRow	= -1;
	m_nSortCol		= -1;	// Sorted column index
	m_bAsc			= false;	

	if( nRowCt > 0 && bRedraw ){
		Redraw(false);
		if( m_pVScroll && m_pVScroll->IsVisible() )
			m_pVScroll->Redraw(false);

		if( m_pHScroll && m_pHScroll->IsVisible() )
			m_pHScroll->Redraw(false);
		}
	return true;
	}

bool
ESChildControlList::DeleteRow(int nRow, bool bRedraw){
	if( !m_pRowSet ) return false;
	if( nRow < 0 || nRow >= m_pRowSet->GetRowCount() )
		return false;

	RemoveInPlaceControl(false);

	if( !m_pRowSet->DeleteRow(nRow) )
		return false;

	_Rect rcView;
	GetItemsViewRect(rcView);
	m_szView.SetSize(GetColumnsWidth(), m_pRowSet->GetRowCount()*m_nLineHeight);
	//m_szView.SetSize(rcView.Width(), m_pRowSet->GetRowCount()*m_nLineHeight);
	m_szPage.SetSize(rcView.Width(), rcView.Height());
	AdjustScrollBarPos(bRedraw);
	
	if( m_nSelectedRow < m_pRowSet->GetRowCount() && m_nSelectedRow > -1 ){
		m_nSelectedRow = (m_pRowSet->GetRowCount() - 1);
		if( bRedraw ){
			RedrawViewArea();
			}
		}
	return true;
	}

bool
ESChildControlList::AddEmptyRow(bool bRedraw){
	if( !m_pRowSet ) return false;

	// Add empty row.
	m_pRowSet->AddRow();

	_Rect rcView;
	GetItemsViewRect(rcView);
	m_szView.SetSize(GetColumnsWidth(), m_pRowSet->GetRowCount()*m_nLineHeight);
	//m_szView.SetSize(rcView.Width(), m_pRowSet->GetRowCount()*m_nLineHeight);
	m_szPage.SetSize(rcView.Width(), rcView.Height());
	AdjustScrollBarPos(bRedraw);

	GotoRow(GetRowCount() - 1, bRedraw);
	return true;
	}

int
ESChildControlList::GetCurrentFieldAsInt(const TCHAR* sField, int nDefault /*= 0*/){
	if( !m_pRowSet || m_nSelectedRow == -1 ) 
		return nDefault;
	if( m_pRowSet->SetCurrentRow(m_nSelectedRow) ){
		return m_pRowSet->GetInt32((TCHAR*)sField);
		}
	return nDefault;
	}

_string
ESChildControlList::GetCurrentFieldAsString(const TCHAR* sField){
	if( !m_pRowSet || m_nSelectedRow == -1 ) 
		return _T("");
	if( m_pRowSet->SetCurrentRow(m_nSelectedRow) ){
		return m_pRowSet->GetString((TCHAR*)sField);
		}
	return _T("");
	}	

int
ESChildControlList::GetFieldId(int nCol){
	if( nCol < 0 || nCol >= m_listColumns.GetCount() )
		return -1;
	ESListColumnInfo* pInfo = m_listColumns.GetAt(nCol);
	if( !pInfo )
		return -1;
	if( !m_pRowSet )
		return -1;
	return m_pRowSet->GetOrdinalIndex((TCHAR*)pInfo->m_sField.c_str());
	}

int
ESChildControlList::FindRowByFieldValue_Int(_string sField, int nFieldValue){
	if( !m_pRowSet || !m_pRowSet->GetRowCount() ) 
		return -1;

	int nFieldId = m_pRowSet->GetOrdinalIndex((TCHAR*)sField.c_str());
	if( nFieldId == -1 )
		return -1;

	int nLoop = 0, nCt = m_pRowSet->GetRowCount();
	while( nLoop < nCt ){
		m_pRowSet->SetCurrentRow(nLoop);
		if( nFieldValue == m_pRowSet->GetInt32(nFieldId) )
			return nLoop;
		nLoop ++;
		}
	return -1;
	}

bool
ESChildControlList::SortByColumnIndex(int nCol, bool bGotoSelectedRow, bool bRedraw){
	ESListColumnInfo* pCol = m_listColumns.GetAt(nCol);
	if( !pCol )
		return false;

	bool bAsc = true;
	if( nCol == m_nSortCol ){
		bAsc = !m_bAsc;
		}

	int nCurrRow = m_nSelectedRow;
	if( m_pRowSet && m_pRowSet->SortByField((TCHAR*)pCol->m_sField.c_str(), nCurrRow, bAsc) ){
		m_nSortCol	= nCol;	// Sorted column index
		m_bAsc		= bAsc;	

		if( nCurrRow != -1 ){
			m_nSelectedRow = nCurrRow;
			if( bGotoSelectedRow )
				GotoRow(nCurrRow, false);
			}
		if( bRedraw ){
			Redraw(false);
			if( m_pVScroll && m_pVScroll->IsVisible() )
				m_pVScroll->Redraw(false);
			}
		return true;
		}
	return false;
	}

bool
ESChildControlList::IsItemBold(int nItem){
	if( !m_pRowSet ) return false;
	if( !m_pRowSet->SetCurrentRow(nItem) ) return false;
	DWORD dwState = m_pRowSet->GetInt32(_T("_STATE"));
	return ((dwState&ITEM_STATE_BOLD) == ITEM_STATE_BOLD) ? true : false;
	}

bool
ESChildControlList::IsItemUnderline(int nItem){
	if( !m_pRowSet ) return false;
	if( !m_pRowSet->SetCurrentRow(nItem) ) return false;
	DWORD dwState = m_pRowSet->GetInt32(_T("_STATE"));
	return ((dwState&ITEM_STATE_UNDERLINE) == ITEM_STATE_UNDERLINE) ? true : false;
	}

bool
ESChildControlList::IsItemItalic(int nItem){
	if( !m_pRowSet ) return false;
	if( !m_pRowSet->SetCurrentRow(nItem) ) return false;
	DWORD dwState = m_pRowSet->GetInt32(_T("_STATE"));
	return ((dwState&ITEM_STATE_ITALIC) == ITEM_STATE_ITALIC) ? true : false;
	}

bool
ESChildControlList::SetItemBold(int nItem, bool bBold){
	if( !m_pRowSet ) return false;
	if( !m_pRowSet->SetCurrentRow(nItem) ) return false;
	DWORD dwState = bBold ? (m_pRowSet->GetInt32(_T("_STATE")) | ITEM_STATE_BOLD) : (m_pRowSet->GetInt32(_T("_STATE"))&(~ITEM_STATE_BOLD));
	m_pRowSet->SetInt32(_T("_STATE"), dwState);
	return true;
	}

bool
ESChildControlList::SetItemUnderline(int nItem, bool bUnderLine){
	if( !m_pRowSet ) return false;
	if( !m_pRowSet->SetCurrentRow(nItem) ) return false;
	DWORD dwState = bUnderLine ? (m_pRowSet->GetInt32(_T("_STATE")) | ITEM_STATE_UNDERLINE) : (m_pRowSet->GetInt32(_T("_STATE"))&(~ITEM_STATE_UNDERLINE));
	m_pRowSet->SetInt32(_T("_STATE"), dwState);
	return true;
	}

bool
ESChildControlList::SetItemItalic(int nItem, bool bItalic){
	if( !m_pRowSet ) return false;
	if( !m_pRowSet->SetCurrentRow(nItem) ) return false;
	DWORD dwState = bItalic ? (m_pRowSet->GetInt32(_T("_STATE")) | ITEM_STATE_ITALIC) : (m_pRowSet->GetInt32(_T("_STATE"))&(~ITEM_STATE_ITALIC));
	m_pRowSet->SetInt32(_T("_STATE"), dwState);
	return true;
	}

bool
ESChildControlList::CreateInPlaceControl(int nRow, int nCol){
	RemoveInPlaceControl(false);

	_Rect rcCell;
	bool bRet = false;
	if( nCol > -1 && GetCellRect(nRow, nCol, rcCell) ){
		ESChildTextBox* pTextBox = new ESChildTextBox();
		if( !m_pOwner->AddChildControl(pTextBox, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcCell, this, 1) ){
			delete pTextBox;
			}
		else{
			_string sValue;
			GetCellValueAsText(nRow, nCol, sValue);

			m_nRow_InPlaceControl	= nRow;
			m_nCol_InPlaceControl	= nCol;

			pTextBox->AddEventListener		((ESFrameBase*)this, ChildControlEventCode::EventCode_OnOK, (ControlEventListener)&ESChildControlList::OnInPlaceControlOk);
			pTextBox->AddEventListener		((ESFrameBase*)this, ChildControlEventCode::EventCode_OnCancel, (ControlEventListener)&ESChildControlList::OnInPlaceControlCancel);
			pTextBox->SetTextFont			(&m_normalItemTextFont);
			pTextBox->SetText				(sValue);
			m_pInPlaceControl		= pTextBox;
			bRet					= true;
			}
		}
	return bRet;
	}

bool
ESChildControlList::RemoveInPlaceControl(bool bRedraw){
	if( m_pInPlaceControl ){
		//ModifyPropertyGridItemByType(m_pSelectedItem, m_pValueModifyControl);
		m_pInPlaceControl->SetVisible(false, bRedraw);
		m_pOwner->RemoveChildControl(m_pInPlaceControl);
		m_pInPlaceControl = NULL;

		m_nRow_InPlaceControl	= -1;
		m_nCol_InPlaceControl	= -1;
		return true;
		}
	return false;
	}

bool
ESChildControlList::GetCellValueAsText(int nRow, int nCol, _string& sValue){
	if( !m_pRowSet ) return false;

	ESListColumnInfo*	pCol = m_listColumns.GetAt(nCol);
	if( !pCol ) return false;

	int nCurRow = m_pRowSet->GetCurrentRow();
	if( !m_pRowSet->SetCurrentRow(nRow) ){
		m_pRowSet->SetCurrentRow(nCurRow);
		return false;
		}
	m_pRowSet->GetString	((TCHAR*)pCol->m_sField.c_str(), sValue);
	m_pRowSet->SetCurrentRow(nCurRow);
	return true;
	}

/////////////////////////////////////////////////////////////////////////////
// Control events.

void
ESChildControlList::OnInPlaceControlOk(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam){
	ESChildTextBox* pTextBox = (ESChildTextBox*)pSender;
	if( pTextBox ){
		_string sText		= pTextBox->GetText();
		int		nCurrRow	= m_pRowSet->GetCurrentRow();

		if( m_pRowSet->SetCurrentRow(m_nRow_InPlaceControl) ){
			int nFieldId = GetFieldId(m_nCol_InPlaceControl);
			m_pRowSet->SetString(nFieldId, sText);
			}
		m_pRowSet->SetCurrentRow(nCurrRow);
		}
	RemoveInPlaceControl(true);
	}

void
ESChildControlList::OnInPlaceControlCancel(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam){
	RemoveInPlaceControl(true);
	}

/////////////////////////////////////////////////////////////////////////////
// Variable modify callbacks.

bool
ESChildControlList::ColumnsModify(ESChildControlList* pThis, VariableInfo* pInfo, void* lpValueNew){
	_Rect rcView;
	pThis->GetItemsViewRect(rcView);
	//pThis->m_szView.SetSize(rcView.Width(), pThis->m_pRowSet ? pThis->m_pRowSet->GetRowCount()*pThis->m_nLineHeight : 0);
	pThis->m_szView.SetSize(pThis->GetColumnsWidth(), pThis->m_pRowSet ? pThis->m_pRowSet->GetRowCount()*pThis->m_nLineHeight : 0);
	pThis->m_szPage.SetSize(rcView.Width(), rcView.Height());
	pThis->AdjustScrollBarPos(true);

	pThis->Redraw(true);
	return true;
	}