// ESChildControlSeat.cpp : implementation file
//

#include "ESChildSeatStatus.h"

#include "..\..\..\Path\Path.h"
#include "..\PokerGUI.h"
#include "ESChildControlTable.h"
#include "ESChildControlSeatCards.h"
#include "ESChildControlSeatHiddenCards.h"
#include "..\..\..\Controls\ESChildControlLabel.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildSeatStatus

ESChildSeatStatus::ESChildSeatStatus(){
	m_sClassName		= _T("ESChildSeatStatus");
	m_rcOmitBorder.SetRect(0, 0, 0, 0);
	m_bActive			= false;
	m_nSeat				= -1;
	m_fZoom				= 1.0f;
	m_pBgImage			= NULL;
	m_pBgActiveImage	= NULL;

	m_imageFillMode	.Add(_T("CentredImage"),		FillMode::FillMode_CentredImage);
	m_imageFillMode	.Add(_T("StretchImage"),		FillMode::FillMode_StretchImage);
	m_imageFillMode	.Add(_T("RepeatedImage"),		FillMode::FillMode_RepeatedImage);
	m_imageFillMode	.Add(_T("OmitBorderedImage"),	FillMode::FillMode_OmitBorderedImage);
	m_imageFillMode	.SetCurrentIndex(0);
	}

BEGIN_DECLARE_VAR(ESChildSeatStatus, ESChildControl)
DECLARE_VAR(_T("ESChildSeatStatus"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Bg Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildSeatStatus,m_sBgImageFile), false, false, false)
	DECLARE_VAR(_T("Active Bg Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildSeatStatus,m_sActiveBgImageFile), false, false, false)
	DECLARE_VAR(_T("Image File Mode"),		_T(""), VariableType::VT_Enumerable, offsetof(ESChildSeatStatus,m_imageFillMode), false, false, false)
	DECLARE_VAR(_T("Omitborder Rect"),		_T(""), VariableType::VT_Rect, offsetof(ESChildSeatStatus,m_rcOmitBorder), false, false, false)
	DECLARE_VAR(_T("Active State"),			_T(""), VariableType::VT_Bool, offsetof(ESChildSeatStatus,m_bActive), false, false, false)
END_DECLARE_VAR()

ESChildSeatStatus::~ESChildSeatStatus(){
	}

bool
ESChildSeatStatus::Create(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		MercuryGUI* pGUI = MercuryGUI::GetInstance();
		std::string sImage;
		if( !m_pBgImage ){
			sImage = m_sBgImageFile;
			Path::BuildFullPath(sImage);
			m_pBgImage = pGUI->LoadImage(sImage);
			
			// Save seat status region. {{
			if( !m_pOwner->IsDesignerMode() ){
				Rgn* pSeatStatusImageRgn = ESChildControlTable::GetSeatStatusImageRgn();
				if( pSeatStatusImageRgn->IsNull() && m_pBgImage && !m_pBgImage->IsNull() ){
					RGNDef hRgn = m_pBgImage->GetBitmap32bppRgnByAlphaChannel(200);
					if( hRgn )
						pSeatStatusImageRgn->Attach(hRgn);
					}
				}
			// }}
			}

		if( !m_pBgActiveImage ){
			sImage = m_sActiveBgImageFile;
			Path::BuildFullPath(sImage);
			m_pBgActiveImage = pGUI->LoadImage(sImage);
			}
		}
	return bRet;
	}

void
ESChildSeatStatus::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildSeatStatus::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildSeatStatus::SetSeatActive(bool bActive, bool bRedraw /*= true*/){
	if( bActive == m_bActive )
		return;
	m_bActive = bActive;
	if( bRedraw )
		Redraw();
	}

void
ESChildSeatStatus::SetUserName(std::string sUserName, bool bRedraw){
	std::string sName;
	stringEx::Format(sName, _T("_userName%d"), m_nSeat + 1);
	ESChildControlLabel* pLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.c_str());
	if( pLabel )
		pLabel->SetLabel(sUserName, bRedraw);
	}

void
ESChildSeatStatus::SetUserBalance(std::string sBalance, bool bRedraw){
	std::string sName;
	stringEx::Format(sName, _T("_userBalance%d"), m_nSeat + 1);
	ESChildControlLabel* pLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.c_str());
	if( pLabel )
		pLabel->SetLabel(sBalance, bRedraw);
	}

std::string
ESChildSeatStatus::GetUserBalanceText(){
	std::string sName;
	stringEx::Format(sName, _T("_userBalance%d"), m_nSeat + 1);
	ESChildControlLabel* pLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.c_str());
	return pLabel ? pLabel->GetLabel() : _T("");
	}

void
ESChildSeatStatus::SetZoomFactor(float fZoom, bool bRedraw){
	m_fZoom = m_fZoom;
	if( bRedraw )
		Redraw(true);
	}

void
ESChildSeatStatus::OnPaintClient(_DC* pDC, Rect* pRectDC, Rect* pRectClient){
	Rect rcClient							= GetClientRect();
	Rect rcControlDC;
	rcControlDC.left						= (rcClient.left - pRectClient->left) + pRectDC->left;
	rcControlDC.top							= (rcClient.top - pRectClient->top) + pRectDC->top;
	rcControlDC.right						= rcControlDC.left + rcClient.Width();
	rcControlDC.bottom						= rcControlDC.top + rcClient.Height();

	Rect rcInvalid							= *pRectDC&rcControlDC;
	Size szClient(rcClient.Width(), rcClient.Height());

	Image* pImageStatus						= m_pBgImage;
	Image* pImageStatusActive				= m_pBgActiveImage;
	Image* pImageStatusCurrent				= m_bActive ? pImageStatusActive : pImageStatus;

	ESChildControlTable*	pTable			= (ESChildControlTable*)GetParent();
	bool					bAllowCaching	= m_pOwner->IsDesignerMode() ? false : true;

	if( bAllowCaching ){
		ImageDef hImageCached = pTable->GetCachedSeatStatusImage(m_bActive);
		Image imgCached;
		imgCached.Attach(hImageCached);

		if( imgCached.GetWidth() != szClient.cx || imgCached.GetHeight() != szClient.cy ){
			// Update cached image.
			pTable->FreeCachedSeatStatusImage(m_bActive);
			hImageCached = NULL;
			if( pImageStatusCurrent->ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szClient.cx, szClient.cy, Rect(0, 0, szClient.cx, szClient.cy), hImageCached, (pImageStatusCurrent->GetBPP() == 32)) )
				pTable->UpdateCachedSeatStatusImage(m_bActive, hImageCached);
			}
		imgCached.Detach();

		if( hImageCached ){
			// Use cached image.
			Image imgTemp;
			imgTemp.Attach(hImageCached);
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_CentredImage, RGB(0, 0, 0), &imgTemp, Rect(0, 0, 0, 0), false, (int)ScaleMode::ScaleMode_Bilinier);
			imgTemp.Detach();
			}
		else{
			if( pImageStatusCurrent && !pImageStatusCurrent->IsNull() )
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pImageStatusCurrent, m_rcOmitBorder, false, ScaleMode::ScaleMode_Bilinier);
			}
		}
	else{
		if( pImageStatusCurrent && !pImageStatusCurrent->IsNull() )
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pImageStatusCurrent, m_rcOmitBorder, false, ScaleMode::ScaleMode_Bilinier);
		}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

RGNDef
ESChildSeatStatus::GetControlRgn(Size& szRgnBound, bool& bCentered){
	if( IsDesignerMode() )
		return NULL;
	Rgn*	pRgn		=  ESChildControlTable::GetSeatStatusImageRgn();
	szRgnBound			= m_pBgImage ? Size(m_pBgImage->GetWidth(), m_pBgImage->GetHeight()) : Size(0, 0);
	bCentered			= true;
	return *pRgn;
	};