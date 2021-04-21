// ESChildProgressCtrl.cpp : implementation file
//

#include "ESChildProgressCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildProgressCtrl

ESChildProgressCtrl::ESChildProgressCtrl(){
	m_sClassName	= _T("ESChildProgressCtrl");

	m_rcOmitProgressBarFilled	.SetRect(0, 0, 0, 0);
	m_rcOmitProgressBg			.SetRect(0, 0, 0, 0);

	m_nPosMax		= 0;
	m_nPosMin		= 0;
	m_nCurrPos		= 0;

	SetupProgress(1, 100, false);
	SetProgressPos(33, false);
	}

BEGIN_DECLARE_VAR(ESChildProgressCtrl, ESChildControl)
	DECLARE_VAR(_T("ESChildProgressCtrl"),				_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ProgressBarFilled Image"),			_T(""), VariableType::VT_Image, offsetof(ESChildProgressCtrl,m_imageProgressBarFilled), false, false, false)
	DECLARE_VAR(_T("ProgressBarFilled Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildProgressCtrl,m_sImageProgressBarFilled), false, false, false)
	DECLARE_VAR(_T("OmitProgressBarFilled"),			_T(""), VariableType::VT_Rect, offsetof(ESChildProgressCtrl,m_rcOmitProgressBarFilled), false, false, false)

	DECLARE_VAR(_T("imageProgressBarBg Image"),			_T(""), VariableType::VT_Image, offsetof(ESChildProgressCtrl,m_imageProgressBarBg), false, false, false)
	DECLARE_VAR(_T("ImageProgressBarBg Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildProgressCtrl,m_sImageProgressBarBg), false, false, false)
	DECLARE_VAR(_T("OmitProgressBg"),					_T(""), VariableType::VT_Rect, offsetof(ESChildProgressCtrl,m_rcOmitProgressBg), false, false, false)

	DECLARE_VAR(_T("PosMax"),							_T(""), VariableType::VT_Int32, offsetof(ESChildProgressCtrl,m_nPosMax), false, false, false)
	DECLARE_VAR(_T("PosMin"),							_T(""), VariableType::VT_Int32, offsetof(ESChildProgressCtrl,m_nPosMin), false, false, false)
	DECLARE_VAR(_T("CurrPos"),							_T(""), VariableType::VT_Int32, offsetof(ESChildProgressCtrl,m_nCurrPos), false, false, false)	
END_DECLARE_VAR()

ESChildProgressCtrl::~ESChildProgressCtrl(){
	}

bool
ESChildProgressCtrl::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	if( ESChildControl::Create(nId, rRect, pOwner, pParent) ){
		}
	return true;
	}

bool
ESChildProgressCtrl::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildProgressCtrl::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildProgressCtrl::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESChildProgressCtrl::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildProgressCtrl::SetupProgress(int nPosMin, int nPosMax, bool bRedraw /*= true*/){
	m_nPosMin	= nPosMin;
	m_nPosMax	= nPosMax;
	m_nCurrPos	= nPosMin;

	if( bRedraw )
		Redraw();
	return true;
	}

bool
ESChildProgressCtrl::SetProgressPos(int nPos, bool bRedraw /*= true*/){
	if( nPos > m_nPosMax )
		nPos = m_nPosMax;
	else
		if( nPos < m_nPosMin )
			nPos = m_nPosMin;

	if( nPos == m_nCurrPos )
		return true;
	m_nCurrPos = nPos;
	if( bRedraw )
		Redraw();
	return true;
	}

int
ESChildProgressCtrl::GetProgressPos(){
	if( m_nPosMin > m_nPosMax )
		return 0;
	return m_nCurrPos;
	}

int
ESChildProgressCtrl::GetProgressPercentage(){
	float fPercentage = ((float)m_nCurrPos) / m_nPosMax;
	return (int)(fPercentage * 100.0f);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildProgressCtrl::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcControlDC, rcInvalid;
	rcControlDC.left		= (m_rRect.left - pRectClient->left) + pRectDC->left; 
	rcControlDC.top			= (m_rRect.top - pRectClient->top) + pRectDC->top; 
	rcControlDC.right		= rcControlDC.left + m_rRect.Width(); 
	rcControlDC.bottom		= rcControlDC.top + m_rRect.Height();

	_Rect		rcControl	= GetClientRect();
	_Image*		pImageBg	= &m_imageProgressBarBg;
	if( pImageBg->IsNull() ){
		MercuryGUI* pGUI = MercuryGUI::GetInstance();
		pImageBg = pGUI->LoadImage(m_sImageProgressBarBg);
		}

	if( pImageBg && !pImageBg->IsNull() ){
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcControl, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), pImageBg, m_rcOmitProgressBg);
		}

	_Image*		pImageFilled	= &m_imageProgressBarFilled;
	if( pImageFilled->IsNull() ){
		MercuryGUI* pGUI = MercuryGUI::GetInstance();
		pImageFilled = pGUI->LoadImage(m_sImageProgressBarFilled);
		}

	if( pImageFilled && !pImageFilled->IsNull() ){
		_Rect		rcFill;
		rcFill.left		= rcControl.left;
		rcFill.top		= rcControl.top;
		rcFill.right	= rcControl.left + ((((float)m_nCurrPos)/(m_nPosMax - m_nPosMin))*rcControl.Width());
		rcFill.bottom	= rcControl.bottom;
		m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcFill, _FillMode::FillMode_OmitBorderedImage, RGB(0, 0, 0), pImageFilled, m_rcOmitProgressBarFilled);
		}
	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}