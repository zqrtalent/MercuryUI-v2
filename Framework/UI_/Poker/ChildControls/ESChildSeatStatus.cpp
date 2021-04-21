// ESChildControlSeat.cpp : implementation file
//

#include "stdafx.h"
#include "../../../Path\Path.h"
#include "../PokerGUI.h"
#include "ESChildControlTable.h"
#include "ESChildSeatStatus.h"
#include "ESChildControlSeatCards.h"
#include "ESChildControlSeatHiddenCards.h"
#include "../SeatPanelInfo.h"

#include "../../../Controls/ESChildControlLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
ESChildSeatStatus::Create(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		MercuryGUI* pGUI = MercuryGUI::GetInstance();
		_String sImage;
		if( !m_pBgImage ){
			sImage = m_sBgImageFile;
			Path::BuildFullPath(sImage);
			m_pBgImage = pGUI->LoadImage(sImage);
			
			// Save seat status region. {{
			if( !m_pOwner->IsDesignerMode() ){
				CRgn* pSeatStatusImageRgn = ESChildControlTable::GetSeatStatusImageRgn();
				if( !pSeatStatusImageRgn->m_hObject && m_pBgImage && !m_pBgImage->IsNull() ){
					HRGN hRgn = m_pBgImage->GetBitmap32bppRgnByAlphaChannel(200);
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
ESChildSeatStatus::SetUserName(_String sUserName, bool bRedraw){
	_String sName;
	sName.Format(_T("_userName%d"), m_nSeat + 1);
	ESChildControlLabel* pLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.GetBuffer());
	if( pLabel )
		pLabel->SetLabel(sUserName, bRedraw);
	}

void
ESChildSeatStatus::SetUserBalance(_String sBalance, bool bRedraw){
	_String sName;
	sName.Format(_T("_userBalance%d"), m_nSeat + 1);
	ESChildControlLabel* pLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.GetBuffer());
	if( pLabel )
		pLabel->SetLabel(sBalance, bRedraw);
	}

_String
ESChildSeatStatus::GetUserBalanceText(){
	_String sName;
	sName.Format(_T("_userBalance%d"), m_nSeat + 1);
	ESChildControlLabel* pLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.GetBuffer());
	return pLabel ? pLabel->GetLabel() : _T("");
	}

void
ESChildSeatStatus::SetZoomFactor(float fZoom, bool bRedraw){
	m_fZoom = m_fZoom;
	if( bRedraw )
		Redraw(true);
	}

void
ESChildSeatStatus::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect rcClient		= GetClientRect();
	_Rect rcControlDC	= ClientRectToDCRect(pRectDC, pRectClient, &rcClient);
	
	_Rect rcInvalid							= *pRectDC&rcControlDC;
	_Size szClient(rcClient.Width(), rcClient.Height());

	Image* pImageStatus						= m_pBgImage;
	Image* pImageStatusActive				= m_pBgActiveImage;
	Image* pImageStatusCurrent				= m_bActive ? pImageStatusActive : pImageStatus;

	ESChildControlTable*	pTable			= (ESChildControlTable*)GetParent();
	bool					bAllowCaching	= m_pOwner->IsDesignerMode() ? false : true;

	if( bAllowCaching ){
		HBITMAP hImageCached = pTable->GetCachedSeatStatusImage(m_bActive);
		BITMAP	bmInfo;
		memset(&bmInfo, 0, sizeof(BITMAP));
		if( hImageCached )
			GetObject(hImageCached, sizeof(BITMAP), &bmInfo);

		if( bmInfo.bmWidth != szClient.cx || bmInfo.bmHeight != szClient.cy ){
			// Update cached image.
			pTable->FreeCachedSeatStatusImage(m_bActive);
			hImageCached = NULL;
			if( pImageStatusCurrent->ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szClient.cx, szClient.cy, _Rect(0, 0, szClient.cx, szClient.cy), hImageCached, (pImageStatusCurrent->GetBPP() == 32)) )
				pTable->UpdateCachedSeatStatusImage(m_bActive, hImageCached);
			}

		if( hImageCached ){
			// Use cached image.
			Image imgTemp;
			imgTemp.Attach(hImageCached);
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_CentredImage, RGB(0, 0, 0), &imgTemp, _Rect(0, 0, 0, 0), false, (int)ScaleMode::ScaleMode_Bilinier);
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

CRgn*
ESChildSeatStatus::GetControlRgn(_Size& szRgnBound, bool& bCentered){
	if( IsDesignerMode() )
		return NULL;
	CRgn*	pRgn		=  ESChildControlTable::GetSeatStatusImageRgn();
	szRgnBound			= m_pBgImage ? _Size(m_pBgImage->GetWidth(), m_pBgImage->GetHeight()) : _Size(0, 0);
	bCentered			= true;
	return pRgn;
	};