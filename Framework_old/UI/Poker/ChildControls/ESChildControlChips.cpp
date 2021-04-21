// ESChildControlChips.cpp : implementation file
//

#include "ESChildControlChips.h"
#include "ESChildControlTable.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildControlChips

ESChildControlChips::ESChildControlChips(){
	m_sClassName			= _T("ESChildControlChips");
	m_nAmount				= 0;
	m_sFormat				= _T("%d.%02d");
	m_crLabel				= RGB(255, 255, 255);
	m_fZoom					= 1.0f;
	m_szChipDefault.SetSize(22, 20);
	m_bGroupSameChips		= true;
	m_bDrawLabelText		= true;
	m_nWidthDrawn			= false;

	m_textPosition	.Add(_T("Left"),	PositionDef::Left);
	m_textPosition	.Add(_T("Top"),		PositionDef::Top);
	m_textPosition	.Add(_T("Right"),	PositionDef::Right);
	m_textPosition	.Add(_T("Bottom"),	PositionDef::Bottom);
	m_textPosition	.SetCurrentIndex(3);

	m_chipHorzAlignment.Add(_T("Left"),		HorzAlignmentDef::Align_Left);
	m_chipHorzAlignment.Add(_T("Right"),	HorzAlignmentDef::Align_Right);
	m_chipHorzAlignment.Add(_T("Middle"),	HorzAlignmentDef::Align_Middle);
	m_chipHorzAlignment.SetCurrentIndex(0);

	m_nDistanceCYCurrent	= 0;
	m_nDrawChipPartCY		= 9;
	m_fLabelTextMaxZoom		= 1.1f;

	m_bShadow				= false;
	m_crShadow				= RGB(0, 0, 0);

	m_bShadow				= true;
	m_crShadow				= RGB(40, 40, 40);
	m_ptShadowPos.SetPoint(2, 1);
	}

BEGIN_DECLARE_VAR(ESChildControlChips, ESChildControl)
DECLARE_VAR(_T("ESChildControlChips"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Label Font"),				_T(""), VariableType::VT_Font, offsetof(ESChildControlChips,m_labelFont), false, false, false)
	DECLARE_VAR(_T("Label Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlChips,m_crLabel), false, false, false)
	DECLARE_VAR(_T("Distance between chips"),	_T(""), VariableType::VT_Int32, offsetof(ESChildControlChips,m_nDistanceCY), false, false, false)
	DECLARE_VAR(_T("Label Format String"),		_T(""), VariableType::VT_String, offsetof(ESChildControlChips,m_sFormat), false, false, false)
	DECLARE_VAR(_T("Chip default size"),		_T(""), VariableType::VT_Size, offsetof(ESChildControlChips,m_szChipDefault), false, false, false)
	DECLARE_VAR(_T("Amount"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControlChips,m_nAmount), false, false, false)
	DECLARE_VAR(_T("GroupChips"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControlChips,m_bGroupSameChips), false, false, false)
	DECLARE_VAR(_T("Label Visibility"),			_T(""), VariableType::VT_Bool, offsetof(ESChildControlChips,m_bDrawLabelText), false, false, false)
	DECLARE_VAR(_T("Text Position"),			_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlChips,m_textPosition), false, false, false)
	DECLARE_VAR(_T("Zoom"),						_T(""), VariableType::VT_Float, offsetof(ESChildControlChips,m_fZoom), false, false, false)
	DECLARE_VAR(_T("DrawChipPartCY"),			_T(""), VariableType::VT_Int32, offsetof(ESChildControlChips,m_nDrawChipPartCY), false, false, false)
	DECLARE_VAR(_T("ChipHorzAlignment"),		_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlChips,m_chipHorzAlignment), false, false, false)
	DECLARE_VAR(_T("Shadow"),					_T(""), VariableType::VT_Bool, offsetof(ESChildControlChips,m_bShadow), false, false, false)
	DECLARE_VAR(_T("ShadowColor"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlChips,m_crShadow), false, false, false)
	DECLARE_VAR(_T("ShadowPos"),				_T(""), VariableType::VT_Point, offsetof(ESChildControlChips,m_ptShadowPos), false, false, false)
END_DECLARE_VAR()

ESChildControlChips::~ESChildControlChips(){
	}

bool
ESChildControlChips::SetAmount(int nAmount, bool bInvalidate /*= true*/){
	int nAmountOld	= m_nAmount;
	m_nAmount		= nAmount;
	if( nAmountOld == m_nAmount )
		return true;

	UpdateLabelText	();

	Rect rcLabel = m_rcLabel, rcChips = m_rcChips;
	RenderChipsAndLabel(m_rcLabel, m_rcChips);

	if( bInvalidate ){
		RedrawRect(m_rcLabel|rcLabel, true);
		RedrawRect(m_rcChips|rcChips, true);
		//Redraw();
		}
	else{
		m_rcLabel |= rcLabel;
		m_rcChips |= rcChips;
		}
	return true;
	}
bool
ESChildControlChips::AddAmount(int nAmountAdd, bool bInvalidate /*= true*/){
	int nAmountOld	= m_nAmount;
	m_nAmount		+= nAmountAdd;
	UpdateLabelText	();

	Rect rcLabel = m_rcLabel, rcChips = m_rcChips;
	RenderChipsAndLabel(m_rcLabel, m_rcChips);

	if( bInvalidate ){
		RedrawRect(m_rcLabel|rcLabel, true);
		RedrawRect(m_rcChips|rcChips, true);
		//Redraw();
		}
	else{
		m_rcLabel |= rcLabel;
		m_rcChips |= rcChips;
		}
	return true;
	}

bool
ESChildControlChips::GetTooltipText(std::string& sTooltipText){
	if( m_nAmount <= 0 )
		return false;
	sTooltipText = m_sLabel;
	return true;
	}

void
ESChildControlChips::Redraw(bool bLayersToo){
	if( !m_pOwner || m_pOwner->IsDesignerMode() || !IsVisible() || !m_nAmount ){
		ESChildControl::Redraw(bLayersToo);
		return;
		}

	Rect rcLabel, rcChips;
	if( RenderChipsAndLabel(rcLabel, rcChips) ){
		rcLabel |= m_rcLabel;
		if( !rcLabel.IsRectEmpty() )
			ESChildControl::RedrawRect(rcLabel, bLayersToo);

		rcChips |= m_rcChips;
		if( !rcChips.IsRectEmpty() )
			ESChildControl::RedrawRect(rcChips, bLayersToo);
		return;
		}

	ESChildControl::Redraw(bLayersToo);
	}

bool
ESChildControlChips::PtInClientArea(Point pt){
	if( !m_pOwner || !IsVisible() || !m_nAmount )
		return false;
	if( m_pOwner->IsDesignerMode() )
		return ESChildControl::PtInClientArea(pt);
	return (m_rcChips.PtInRect(pt) == TRUE);
	}

void
ESChildControlChips::UpdateLabelText(){
	std::string sAmount;
	if( m_nAmount <= 0 ){
		m_sLabel.empty();
		return;
		}
	if( m_nAmount%100 > 0 )
		stringEx::Format(sAmount, _T("%d.%02d"), m_nAmount/100, m_nAmount%100);
	else
		stringEx::Format(sAmount, _T("%d"), m_nAmount/100);
	stringEx::Format(m_sLabel, m_sFormatLocal.c_str(), sAmount.c_str());
	}

bool
ESChildControlChips::RenderChipsAndLabel(Rect& rcLabel, Rect& rcBoundChips, _DC* pDC /*= NULL*/, Rect* pRectDC /*= NULL*/, Rect* pRectClient /*= NULL*/){
	Rect	rcControl		= m_rRect, rcChip, rcChips, rcInvalid;
	bool	bRender			= (pDC && pRectDC && pRectClient);
	int		nLabelPosition	= m_textPosition.GetCurrentValue();
	float	fZoomCX			= m_fZoom, fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);
	rcBoundChips.SetRectEmpty();

	Font	font;
	m_labelFont.StretchedTextFont(&font, min(fZoomCX, m_fLabelTextMaxZoom), min(fZoomCY, m_fLabelTextMaxZoom));

	Size	szLabelText(0, 0);
	if( m_bDrawLabelText )
		font.GetTextSize(&m_sLabel, szLabelText);
	else
		nLabelPosition	= PositionDef::Right;

	Size szChip((int)(m_szChipDefault.cx*fZoomCY), (int)(m_szChipDefault.cy*fZoomCY));
	CPtrArray arrCurrChipImages;
	if( ESChildControlTable::GetChipImagesByAmount(szChip, m_nAmount, arrCurrChipImages, szChip) <= 0 )
		return false;

	Image* pChipImage = (Image*)arrCurrChipImages[0];
	if( pChipImage && !pChipImage->IsNull() )
		szChip.SetSize(pChipImage->GetWidth(), pChipImage->GetHeight());

	switch(nLabelPosition){
		case PositionDef::Left:{
			rcLabel.left		= rcControl.left;
			rcLabel.top			= rcControl.bottom - szLabelText.cy;
			rcLabel.right		= rcLabel.left + szLabelText.cx;
			rcLabel.bottom		= rcLabel.top + szLabelText.cy;

			rcChips.left		= rcLabel.right + 2;
			rcChips.top			= rcControl.top;
			rcChips.right		= rcControl.right;
			rcChips.bottom		= rcControl.bottom;
			break;
			}
		case PositionDef::Right:{
			rcLabel.left		= (rcControl.right - szLabelText.cx);
			rcLabel.top			= rcControl.bottom - szLabelText.cy;
			rcLabel.right		= rcLabel.left + szLabelText.cx;
			rcLabel.bottom		= rcLabel.top + szLabelText.cy;

			rcChips.left		= rcControl.left;
			rcChips.top			= rcControl.top;
			rcChips.right		= rcLabel.left - 2;
			rcChips.bottom		= rcControl.bottom;
			break;
			}
		case PositionDef::Top:{
			rcLabel.left		= rcControl.left + (rcControl.Width() - szLabelText.cx)/2;
			rcLabel.top			= rcControl.top;
			rcLabel.right		= rcLabel.left + szLabelText.cx;
			rcLabel.bottom		= rcLabel.top + szLabelText.cy;

			rcChips.left		= rcControl.left;
			rcChips.top			= rcLabel.top + 2;
			rcChips.right		= rcControl.right;
			rcChips.bottom		= rcControl.bottom;
			break;
			}
		case PositionDef::Bottom:{
			rcLabel.left		= rcControl.left + (rcControl.Width() - szLabelText.cx)/2;
			rcLabel.top			= rcControl.bottom - szLabelText.cy;
			rcLabel.right		= rcLabel.left + szLabelText.cx;
			rcLabel.bottom		= rcLabel.top + szLabelText.cy;

			rcChips.left		= rcControl.left;
			rcChips.top			= rcControl.top;
			rcChips.right		= rcControl.right;
			rcChips.bottom		= rcLabel.top + 2;
			break;
			}
		}
	// }}

	int		nHorzAlign			= m_chipHorzAlignment.GetCurrentValue();
	int		nChipsCt			= arrCurrChipImages.size();
	int		nMaxChipsCt_InRow	= (rcChips.Width() / (szChip.cx + 2));
	int		nMaxChipsCt_InCol	= 0;

	if( rcChips.Height() >= szChip.cy ){
		if( !m_nDistanceCYCurrent ){
			nMaxChipsCt_InCol	= ((rcChips.Height() - szChip.cy) / 3) + 1;
			nMaxChipsCt_InCol	= min(nMaxChipsCt_InCol, arrCurrChipImages.size());
			}
		else{
			nMaxChipsCt_InCol	= ((rcChips.Height() - szChip.cy) / m_nDistanceCYCurrent) + 1;
			nMaxChipsCt_InCol	= min(nMaxChipsCt_InCol, arrCurrChipImages.size());
			}
		}

	Image* pImageChip	= NULL, *pImagePrev = NULL;
	int		nImage		= 0, nChipsColCt = 0;
	Rect	rcChipPrev;
	bool    bGroupSameChipsInRow = m_bGroupSameChips;

	// Calculate chip columns.
	Rect rcChip_save = rcChip;
	for(int nCol=0; nCol<nMaxChipsCt_InRow; nCol ++){
		rcChip.top			= (rcChips.bottom - szChip.cy);
		rcChip.bottom		= rcChips.bottom;

		int nChipsCtInRow	= 0;
		int nImageIndexOld	= nImage;
		for(int nRow=0; nRow<nMaxChipsCt_InCol; nRow ++){
			if( nImage >= nChipsCt ) break;
			pImagePrev	= pImageChip;
			pImageChip	= (Image*)arrCurrChipImages[nImage];

			// Diferent chip in different column.
			if( bGroupSameChipsInRow ){
				if( pImagePrev != pImageChip && pImagePrev && nMaxChipsCt_InRow > 1 ){
					pImageChip = pImagePrev;
					break;
					}
				}
			nChipsCtInRow	++;
			nImage			++;
			}
		nImage = nImageIndexOld;

		for(int nRow=0; nRow<nChipsCtInRow; nRow++){
			pImageChip	= (Image*)arrCurrChipImages[nImage];
			nImage ++;
			}

		// Skip same chip images.
		if( bGroupSameChipsInRow ){
			while( nImage < nChipsCt ){
				if( pImageChip != (Image*)arrCurrChipImages[nImage] ){
					pImageChip = NULL;
					break;
					}
				nImage ++;
				}
			}

		nChipsColCt ++;
		if( nImage >= nChipsCt ) break;
		}
	rcChip			= rcChip_save;
	int nChipsCX	= (szChip.cx + 2)*nChipsColCt;
	m_nWidthDrawn	= nChipsCX;
	if( nHorzAlign == Align_Middle && rcChips.Width() > nChipsCX )
		rcChips.left = rcChips.left + (rcChips.Width() - nChipsCX)/2;
	// }}

	pImageChip		= pImagePrev = NULL;
	nImage			= 0;
	rcChipPrev.SetRect(0, 0, 0, 0);
	for(int nCol=0; nCol<nMaxChipsCt_InRow; nCol ++){
		rcChip.top	= (rcChips.bottom - szChip.cy);
		rcChip.bottom = rcChips.bottom;
		if( nHorzAlign == Align_Right ){
			rcChip.right	= rcChips.right - (szChip.cx + 2)*nCol;
			rcChip.left		= rcChip.right - szChip.cx;
			}
		else{
			rcChip.left		= rcChips.left + (szChip.cx + 2)*nCol;
			rcChip.right	= rcChip.left + szChip.cx;
			}

		int nChipsCtInRow	= 0, nImageIndexOld	= nImage;
		for(int nRow=0; nRow<nMaxChipsCt_InCol; nRow ++){
			if( nImage >= nChipsCt ) break;
			pImagePrev	= pImageChip;
			pImageChip	= (Image*)arrCurrChipImages[nImage];

			// Diferent chip in different column.
			if( bGroupSameChipsInRow ){
				if( pImagePrev != pImageChip && pImagePrev && nMaxChipsCt_InRow > 1 ){
					pImageChip = pImagePrev;
					break;
					}
				}
			nChipsCtInRow	++;
			nImage			++;
			}

		nImage = nImageIndexOld;
		for(int nRow=0; nRow<nChipsCtInRow; nRow++){
			pImageChip	= (Image*)arrCurrChipImages[nImage];
			if( bRender ){
				// Translate to dc coords.
				Rect	rcChipDC;
				rcChipDC.left	= (rcChip.left - pRectClient->left) + pRectDC->left; 
				rcChipDC.top	= (rcChip.top - pRectClient->top) + pRectDC->top; 
				rcChipDC.right	= rcChipDC.left + rcChip.Width(); 
				rcChipDC.bottom	= rcChipDC.top + rcChip.Height();
			
				// Render whole chip.
				if( nRow == nChipsCtInRow - 1 ){
					Rect rcInvalid		= rcChipDC & *pRectDC;
					if( pImageChip && !rcInvalid.IsRectEmpty() )
						pImageChip->RenderImage(pDC, rcInvalid, rcChipDC, false, true);
					}
				else{// Render part chip image.
					Rect rcChipDCPart	= rcChipDC;
					rcChipDCPart.top	= rcChipDCPart.bottom - m_nDrawChipPartCYCurrent;
					Rect rcInvalid		= rcChipDCPart&*pRectDC;
					if( pImageChip && !rcInvalid.IsRectEmpty() )
						pImageChip->RenderImage(pDC, rcInvalid, rcChipDC, false, true);
					}
				}

			rcBoundChips |= rcChip;

			rcChipPrev		= rcChip;
			rcChip.top		-= m_nDistanceCYCurrent;
			rcChip.bottom	-= m_nDistanceCYCurrent;
			nImage			++;
			}

		// Skip same chip images.
		if( bGroupSameChipsInRow ){
			while( nImage < nChipsCt ){
				if( pImageChip != (Image*)arrCurrChipImages[nImage] ){
					pImageChip = NULL;
					break;
					}
				nImage ++;
				}
			}

		if( nImage >= nChipsCt ) break;
		}

	if( nHorzAlign == Align_Right ){
		if( nLabelPosition == PositionDef::Left ){
			int nCX			= rcLabel.Width();
			rcLabel.right	= rcChip.left - 2;
			rcLabel.left	= rcLabel.right - nCX;
			}
		}

	// Adjust label position. {{
	if( nLabelPosition == PositionDef::Right && rcChip.right + 2 < rcLabel.left ){
		int nCX			= rcLabel.Width();
		rcLabel.left	= rcChip.right + 2;
		rcLabel.right	= rcLabel.right + nCX;
		}
	// }}

	if( m_bDrawLabelText && bRender ){
		FONTDef pFontOld = pDC->SelectObject(font);
		// Translate from window pixel to DC pixel.{{
		Point ptLabel;
		ptLabel.x = (rcLabel.left - pRectClient->left) + pRectDC->left;  
		ptLabel.y = (rcLabel.top - pRectClient->top) + pRectDC->top;
		//}}

		pDC->SetBkMode	(TRANSPARENT);
		if( m_bShadow ){
			pDC->SetTextColor	(m_crShadow);
			pDC->ExtTextOut		(ptLabel.x + m_ptShadowPos.x, ptLabel.y + m_ptShadowPos.y, ETO_CLIPPED, pRectDC, m_sLabel);
			}

		pDC->SetTextColor	(m_crLabel);
		pDC->ExtTextOut		(ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, m_sLabel);
		pDC->SelectObject	(pFontOld);
		}

	font.DeleteObject();
	return true;
	}

bool
ESChildControlChips::Create(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet )
		OnLocalizeStrings();
	return bRet;
	}

bool
ESChildControlChips::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlChips::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);

	float fZoomCX				= m_fZoom, fZoomCY = m_fZoom;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);
	m_nDistanceCYCurrent		= fZoomCY*m_nDistanceCY;
	m_nDrawChipPartCYCurrent	= fZoomCY*m_nDrawChipPartCY;
	}

void
ESChildControlChips::SetFormatString(std::string sFormat){
	m_sFormat = sFormat;
	OnLocalizeStrings();
	}

void
ESChildControlChips::OnPaintClient(_DC* pDC, Rect* pRectDC, Rect* pRectClient){
	if( m_nAmount <= 0 ){ 
		ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
		return;
		}

//	m_bGroupSameChips = false;
//	pDC->FillSolidRect(*pRectDC, RGB(0, 122, 23));
	RenderChipsAndLabel(m_rcLabel, m_rcChips, pDC, pRectDC, pRectClient);

/*
	// Soften filter.
	CBitmap bmpSoften;
	if( ImageHelper::GetBitmap32bppFromDC(bmpSoften, pDC, rcChipsDC.left, rcChipsDC.top, rcChipsDC.Width(), rcChipsDC.Height(), NULL) ){
		if( ImageHelper::SetSoftenFilter(bmpSoften) ){
			_DC memDC;
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&bmpSoften);
			pDC->BitBlt(rcChipsDC.left, rcChipsDC.top, rcChipsDC.Width(), rcChipsDC.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.DeleteDC();
			}
		bmpSoften.DeleteObject();
		}
*/

/*
	// Draw chips label.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if( m_bDrawLabelText ){
		std::string sLabel;
		sLabel.Format(m_sFormat, m_nAmount/100, m_nAmount%100);
		Font	font;
		PokerGUI::StretchedTextFont(pGUI->m_pChipLabelFont, &font, fZoomChips, fZoomChips);
		Font* pFontOld = pDC->SelectObject(&font);

		Size szText;
		ImageHelper::GetTextSize(&sLabel, &font, szText);
		Rect rcLabel;
		rcLabel.left	= m_rcLabel.left + (m_rcLabel.Width() - szText.cx)/2;
		rcLabel.right	= rcLabel.left + szText.cx;
		rcLabel.top		= m_rcLabel.top;
		rcLabel.bottom	= rcLabel.top + szText.cy;
		
		// Translate from window pixel to DC pixel.{{
		Point ptLabel;
		ptLabel.x = (rcLabel.left - pRectClient->left) + pRectDC->left; 
		ptLabel.y = (rcLabel.top - pRectClient->top - 3) + pRectDC->top; 
		//}}

		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(pGUI->m_crChipLabel);
		PokerGUI::ExtTextOut(pDC,ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, sLabel);
		pDC->SelectObject	(pFontOld);
		font.DeleteObject	();
		}
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void
ESChildControlChips::OnMotionStart(DWORD_PTR dwParam){
//	if( dwParam != 0 )
//		SetAmount((int)dwParam, false);

	//m_bGroupSameChips	= false;
	//m_bDrawLabelText = false;
	RenderChipsAndLabel(m_rcLabel, m_rcChips);
	}

void
ESChildControlChips::OnMotionProgress(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY){
	if( !m_pOwner->IsWindow() ) 
		return;

	Rect rcRedraw			= (m_rcLabel.IsRectEmpty() ?  m_rcChips : m_rcLabel|m_rcChips);
	Rect rRectNoChange	= rcRedraw;
	Rect rcOwner;

	m_pOwner->GetClientRectMy(rcOwner);
	SetLogicalPos			(fPosX, fPosY);
	OwnerWindowSizeChanged	(rcOwner.Width(), rcOwner.Height());

	Rect rcRedrawAfter;
	RenderChipsAndLabel(m_rcLabel, m_rcChips);
	if( !m_rcLabel.IsRectEmpty() )
		rcRedrawAfter |= m_rcLabel;
	if( !m_rcChips.IsRectEmpty() )
		rcRedrawAfter |= m_rcChips;

	if( rcRedrawAfter != rRectNoChange ){
		m_pOwner->RedrawRect(rcRedraw, 0);
		m_pOwner->RedrawRect(m_rcLabel, 0);
		m_pOwner->RedrawRect(m_rcChips, 0);
		}
	}

void
ESChildControlChips::OnMotionEnd(){
	//m_bDrawLabelText	= true;
	//m_bGroupSameChips	= true;
	}

void
ESChildControlChips::OnMotionStopped(){
	//m_bDrawLabelText = true;
	}

void
ESChildControlChips::OnLocalizeStrings(){
	m_sFormatLocal = m_sFormat;
	if( !m_pOwner->IsDesignerMode() )
		ESLocalizationMan::ParseText(m_sFormatLocal);
	UpdateLabelText();
	}