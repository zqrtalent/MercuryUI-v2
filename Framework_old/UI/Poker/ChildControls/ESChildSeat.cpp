// ESChildControlSeat.cpp : implementation file
//


#include "../../../Path\Path.h"
#include "../../../Utility\StringHelper.h"

#include "ESChildControlTable.h"
#include "ESChildSeat.h"
#include "ESChildSeatStatus.h"
#include "ESChildControlSeatCards.h"
#include "ESChildControlSeatHiddenCards.h"

#include "../../../Controls/ESChildControlLabel.h"
#include "../../../PokerServer/Object/PokerTableSeatInfo.h"
#include "../PokerGUI.h"


/////////////////////////////////////////////////////////////////////////////
// ESChildSeat

ESChildSeat::ESChildSeat(){
	m_sClassName				= _T("ESChildSeat");
	m_rcOmitBorder.SetRect(0, 0, 0, 0);
	m_bActive					= false;
	m_fZoom						= 1.0f;

	m_imageFillMode		.Add(_T("CentredImage"),		FillMode::FillMode_CentredImage);
	m_imageFillMode		.Add(_T("StretchImage"),		FillMode::FillMode_StretchImage);
	m_imageFillMode		.Add(_T("RepeatedImage"),		FillMode::FillMode_RepeatedImage);
	m_imageFillMode		.Add(_T("OmitBorderedImage"),	FillMode::FillMode_OmitBorderedImage);
	m_imageFillMode		.SetCurrentIndex(0);

	m_ptBorderPos		.SetPoint(-1, -1); //Border is around the seat.
	
	m_bColoredBorder			= false;
	m_crBorder					= 0;	

	m_sActionName				= _T("");
	m_nActionClearTimerId		= 0;

	m_pSeatInfo					= NULL;
	m_nSeat						= -1;
	m_bPulseState				= false;
	m_nPulseTimerId				= 0;

	m_nFullTimeSec				= 0;
	m_nRegularTimeSec			= 0;

	m_pBorderImage				= NULL;
	m_pBorderActiveImage		= NULL;
	m_pBorderColoredImage		= NULL;
	m_pBorderColoredBaseImage	= NULL;
	m_pSeatImage				= NULL;
	m_pSeatImageActive			= NULL;
	m_pEmptySeatImage			= NULL;
	m_pAvatarFaceImage			= NULL;
	m_pReservedSeatImage		= NULL;

	m_seatStatus				= PokerSeatStatus::Empty;
	m_nAvatarId					= 0;

	// Do not use empty seat label.
	m_bUseEmptySeatLabel		= false;
	}

BEGIN_DECLARE_VAR(ESChildSeat, ESChildControl)
DECLARE_VAR(_T("ESChildSeat"),					_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Border Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sBorderImageFile), false, false, false)
	DECLARE_VAR(_T("Active Border Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sActiveBorderImageFile), false, false, false)
	DECLARE_VAR(_T("Colored Border Image File"),_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sBorderColoredImageFile), false, false, false)
	DECLARE_VAR(_T("Colored Border Base Image"),_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sBorderColoredBaseImageFile), false, false, false)
	DECLARE_VAR(_T("Colored Border"),			_T(""), VariableType::VT_Bool, offsetof(ESChildSeat,m_bColoredBorder), false, false, false)
	DECLARE_VAR(_T("Border Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildSeat,m_crBorder), false, false, false)
	DECLARE_VAR(_T("Image File Mode"),			_T(""), VariableType::VT_Enumerable, offsetof(ESChildSeat,m_imageFillMode), false, false, false)
	DECLARE_VAR(_T("Omitborder Rect"),			_T(""), VariableType::VT_Rect, offsetof(ESChildSeat,m_rcOmitBorder), false, false, false)
	DECLARE_VAR(_T("Seat Image File"),			_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sSeatImage), false, false, false)
	DECLARE_VAR(_T("Active Seat Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sActiveSeatImage), false, false, false)	
	DECLARE_VAR(_T("Empty Seat Image"),			_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sEmptySeatImage), false, false, false)
	DECLARE_VAR(_T("Avatar face Image"),		_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sAvatarFaceImage), false, false, false)
	DECLARE_VAR(_T("Reserved Seat Image"),		_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sReservedSeatImage), false, false, false)
	DECLARE_VAR(_T("Active State"),				_T(""), VariableType::VT_Bool, offsetof(ESChildSeat,m_bActive), false, false, false)
	DECLARE_VAR(_T("Zoom factor"),				_T(""), VariableType::VT_Float, offsetof(ESChildSeat,m_fZoom), false, false, false)
	DECLARE_VAR(_T("SeatIndex"),				_T(""), VariableType::VT_Int32, offsetof(ESChildSeat,m_nSeat), false, false, false)
	DECLARE_VAR(_T("Border position"),			_T(""), VariableType::VT_Point, offsetof(ESChildSeat,m_ptBorderPos), false, false, false)

	DECLARE_VAR(_T("UseEmptySeatLabel"),		_T(""), VariableType::VT_Bool, offsetof(ESChildSeat,m_bUseEmptySeatLabel), false, false, false)
	DECLARE_VAR(_T("EmptySeatLabel"),			_T(""), VariableType::VT_String, offsetof(ESChildSeat,m_sEmptySeatLabel), false, false, false)
	DECLARE_VAR(_T("EmptySeatLabelFont"),		_T(""), VariableType::VT_Font, offsetof(ESChildSeat,m_fontEmptySeatLabel), false, false, false)
	DECLARE_VAR(_T("EmptySeatLabelColor"),		_T(""), VariableType::VT_Color32, offsetof(ESChildSeat,m_crEmptySeatLabel), false, false, false)
END_DECLARE_VAR()

ESChildSeat::~ESChildSeat(){
	}

bool
ESChildSeat::Create(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		// Register variable modify callback methods. {{
		if( m_pOwner->IsDesignerMode() ){
			AddVariableModifyCallBack(&m_sBorderImageFile,			(VariableValueModifyCallback)&ESChildSeat::ImageModify);
			AddVariableModifyCallBack(&m_sActiveBorderImageFile,	(VariableValueModifyCallback)&ESChildSeat::ImageModify);
			AddVariableModifyCallBack(&m_sBorderColoredImageFile,	(VariableValueModifyCallback)&ESChildSeat::ColorBorderModify);
			AddVariableModifyCallBack(&m_bColoredBorder,			(VariableValueModifyCallback)&ESChildSeat::ColorBorderModify);
			AddVariableModifyCallBack(&m_crBorder,					(VariableValueModifyCallback)&ESChildSeat::ColorBorderModify);
			AddVariableModifyCallBack(&m_sSeatImage,				(VariableValueModifyCallback)&ESChildSeat::ImageModify);
			AddVariableModifyCallBack(&m_sActiveSeatImage,			(VariableValueModifyCallback)&ESChildSeat::ImageModify);
			AddVariableModifyCallBack(&m_sEmptySeatImage,			(VariableValueModifyCallback)&ESChildSeat::ImageModify);
			AddVariableModifyCallBack(&m_sAvatarFaceImage,			(VariableValueModifyCallback)&ESChildSeat::ImageModify);
			AddVariableModifyCallBack(&m_bActive,					(VariableValueModifyCallback)&ESChildSeat::AttributeModify);
			AddVariableModifyCallBack(&m_fZoom,						(VariableValueModifyCallback)&ESChildSeat::AttributeModify);
			}
		// }}
		UpdateImages();
		}

	if( bRet )
		OnLocalizeStrings();
	return bRet;
	}

void			
ESChildSeat::UpdateImages(){
	MercuryGUI* pGUI = MercuryGUI::GetInstance();
	std::string sImage;
	if( !m_pBorderImage ){
		sImage = m_sBorderImageFile;
		Path::BuildFullPath(sImage);
		m_pBorderImage = pGUI->LoadImage(sImage);
		}

	if( !m_pBorderActiveImage ){
		sImage = m_sActiveBorderImageFile;
		Path::BuildFullPath(sImage);
		m_pBorderActiveImage = pGUI->LoadImage(sImage);
		}

	if( !m_pSeatImage ){
		sImage = m_sSeatImage;
		Path::BuildFullPath(sImage);
		m_pSeatImage = pGUI->LoadImage(sImage);
		}

	if( !m_pSeatImageActive ){
		sImage = m_sActiveSeatImage;
		Path::BuildFullPath(sImage);
		m_pSeatImageActive = pGUI->LoadImage(sImage);
		}

	if( !m_pEmptySeatImage ){
		sImage = m_sEmptySeatImage;
		Path::BuildFullPath(sImage);
		m_pEmptySeatImage = pGUI->LoadImage(sImage);
		}

	if( !m_pAvatarFaceImage ){
		sImage = m_sAvatarFaceImage;
		Path::BuildFullPath(sImage);
		m_pAvatarFaceImage = pGUI->LoadImage(sImage);
		}

	if( !m_pReservedSeatImage ){
		sImage = m_sReservedSeatImage;
		Path::BuildFullPath(sImage);
		m_pReservedSeatImage = pGUI->LoadImage(sImage);
		}

	if( !m_pBorderColoredImage ){
		sImage = m_sBorderColoredImageFile;
		Path::BuildFullPath(sImage);
		m_pBorderColoredImage	= pGUI->LoadImage(sImage);
		}

	if( !m_pBorderColoredBaseImage ){
		sImage = m_sBorderColoredBaseImageFile;
		Path::BuildFullPath(sImage);
		m_pBorderColoredBaseImage = pGUI->LoadImage(sImage, -1, -1, true, false);
		}

	// Create seat regions. {{
	if( !m_pOwner->IsDesignerMode() ){
		Rgn* pSeatImageRgn = ESChildControlTable::GetSeatImageRgn();
		if( !pSeatImageRgn->IsNull() && m_pSeatImage && !m_pSeatImage->IsNull() ){
			HRGN hRgn = m_pSeatImage->GetBitmap32bppRgnByAlphaChannel();
			if( hRgn )
				pSeatImageRgn->Attach(hRgn);
			}

		/*
		_DC memDC;
		memDC.CreateCompatibleDC(NULL);
		CBitmap bmImage;
		ImageHelper::CreateDIBBitmap(24, bmImage, RGB(0, 0, 0), 300, 300, NULL, NULL);
		memDC.SelectObject(&bmImage);

		CBrush br(RGB(122, 0, 0));
		memDC.FillRgn(pSeatImageRgn, &br);

		Image img;
		img.Attach((HBITMAP)bmImage.Detach());
		img.Save(_T("d:\\11121212121212.bmp"));
		memDC.DeleteDC();
		*/

		Rgn* pEmptySeatImageRgn = ESChildControlTable::GetEmptySeatImageRgn();
		if( !pEmptySeatImageRgn->IsNull() && m_pEmptySeatImage && !m_pEmptySeatImage->IsNull() ){
			HRGN hRgn = m_pEmptySeatImage->GetBitmap32bppRgnByAlphaChannel(100);
			if( hRgn )
				pEmptySeatImageRgn->Attach(hRgn);
			}
		}
	// }}

	if( m_bColoredBorder )
		SetBorderColor(m_crBorder, false);
	else
		ClearBorderColor(false);
	}

void
ESChildSeat::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildSeat::Destroy(){
	ESChildControl::Destroy();
	// Destroy color border image.
	if( !m_imgBorderColored.IsNull() )
		m_imgBorderColored.Destroy();

	// Destroy avatar image.
	if( !m_avatarImage.IsNull() )
		m_avatarImage.Destroy();
	return true;
	}

void
ESChildSeat::OnTimer(UINT nIdEvent){
	if( nIdEvent == PulseTimerId ){
		std::string sName;
		stringEx::Format(sName, _T("_pokerSeatStatus%d"), m_nSeat + 1);
		ESChildSeatStatus* pSeatStatus = (ESChildSeatStatus*)m_pOwner->GetChildControlByName(sName.c_str());

		if( !m_pSeatInfo )
			return;

		if( !m_pSeatInfo->IncreaseTimer() )
			ActivateTimer(0, false);
		else{/*
			int								nSeat		= m_pTimerActiveSeat->GetSeat();
			ESChildControlSeatProgressBar*	pProgress	= GetSeatProgressControl(nSeat);
			if( pProgress ){
				int nPosMax = m_pTimerActiveSeat->GetAllSeconds();
				int nPos	= nPosMax - m_pTimerActiveSeat->GetPassedSeconds();
				pProgress->SetProgressPos(nPos, nPosMax, true);
				}*/

			bool bTimeBankJustActivated = false;
			if( !m_bTimeBankIsActive && m_pSeatInfo->GetPassedSeconds() >= m_nRegularTimeSec ){
				m_bTimeBankIsActive		= true;
				bTimeBankJustActivated	= true;
				}

			if( m_bTimeBankIsActive ){
				int nTimeBankSecLeft = (m_pSeatInfo->GetAllSeconds() - m_pSeatInfo->GetPassedSeconds());
				std::string sName;
				stringEx::Format(sName, _T("_pokerSeatStatus%d"), m_nSeat + 1);
				ESChildSeatStatus* pSeatStatus = (ESChildSeatStatus*)m_pOwner->GetChildControlByName(sName.c_str());
				if( pSeatStatus ){
					// Save user balance label text.
					if( bTimeBankJustActivated ){
						m_sUserBalanceLabel = pSeatStatus->GetUserBalanceText();
						// Send event.
						SendEvent(this, (ChildControlEventCode)EventCode_SeatTimeBankActivated, nTimeBankSecLeft);
						}
					stringEx::Format(sName, _T("%d {{Sec}}"), nTimeBankSecLeft);
					pSeatStatus->SetUserBalance(sName, false);
					}
				}
			m_bPulseState = !m_bPulseState;
			}

		bool bActive = m_bActive;
		if( m_pSeatInfo && m_pSeatInfo->IsTimerActive() )
			bActive = m_bPulseState;
		if( pSeatStatus )
			pSeatStatus->SetSeatActive(bActive, true);
		Redraw(true);
		}

	// Clear action name.
	if( nIdEvent == m_nActionClearTimerId )
		SetActionName(_T(""), 0, true);
	}

void
ESChildSeat::SetSeatActive(bool bActive, bool bRedraw /*= true*/){
	if( bActive == m_bActive )
		return;
	m_bActive = bActive;
	if( bRedraw )
		Redraw();
	}

bool
ESChildSeat::ActivateTimer(int nFullTimeSec, bool bActivate /*= true*/){
	if( !m_pSeatInfo ) return false;
	if( bActivate ){
		if( !m_pSeatInfo->IsTimerActive() ){
			m_pSeatInfo->SetTimeBankSeconds	(max(0, nFullTimeSec - m_nRegularTimeSec));
//			m_pSeatInfo->SetupTimer			(nFullTimeSec, 0);
			m_pSeatInfo->ActivateTimer		(true);
			m_pOwner->SetChildControlTimer	(this, PulseTimerId, 1000);

			m_bPulseState		= true;
			m_nFullTimeSec		= nFullTimeSec;
			}
		}
	else{
		if( m_pSeatInfo->IsTimerActive() ){
			m_pSeatInfo->ActivateTimer		(false);
			m_pOwner->KillChildControlTimer	(this, PulseTimerId);

			std::string sName;
			stringEx::Format(sName, _T("_pokerSeatStatus%d"), m_nSeat + 1);
			ESChildSeatStatus* pSeatStatus = (ESChildSeatStatus*)m_pOwner->GetChildControlByName(sName.c_str());
			if( pSeatStatus ){
				if( m_bTimeBankIsActive ){
					m_bTimeBankIsActive = false;
					// Restore user balance label text.
					pSeatStatus->SetUserBalance(m_sUserBalanceLabel, false);
					}
				pSeatStatus->SetSeatActive(false);
				}
			}
		m_bTimeBankIsActive = false;
		}
	return true;
	}

bool
ESChildSeat::SetAvatarImage(Image* pImage, int nAvatarId, bool bRedraw){
	if( !pImage || pImage->IsNull() )
		return false;

	Image * pAvatarFaceImage = NULL;
	if( m_pAvatarFaceImage && !m_pAvatarFaceImage->IsNull() )
		pAvatarFaceImage = m_pAvatarFaceImage;
	else
		if( m_pSeatImage && !m_pSeatImage->IsNull() )
			pAvatarFaceImage = m_pSeatImage;
	if( !pAvatarFaceImage )
		return false;

	if( m_nAvatarId == nAvatarId )
		return true; // Already exists such avatar.

	Size	szAvatar(pAvatarFaceImage->GetWidth(), pAvatarFaceImage->GetHeight());
	Image bmAvatar;
	if( !bmAvatar.CreateDIBBitmap(24, RGB(0, 0, 0), szAvatar.cx, szAvatar.cy) )
		return false;

	ImageDef hDest = bmAvatar;
	if( !pImage->ScaleBitmap_Bilinier(szAvatar.cx, szAvatar.cy, Rect(0, 0, szAvatar.cx, szAvatar.cy), hDest, (pImage->GetBPP() == 32), 255) ){
		bmAvatar.Destroy();
		return false;
		}
	
	Image imgBg;
	imgBg.Attach(bmAvatar.Detach());
	/*imgBg.Save("d:\\11111111.bmp");*/

	Image* pImgAvatar = Image::CombineImages(&imgBg, pAvatarFaceImage, true);
	if( !pImgAvatar ){
		imgBg.Destroy();
		return false;
		}
	/*
	GUID arrGUID[] = {
		{0xb96b3caf,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}, // PNG
		{0xb96b3cae,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}, // JPEG
		{0xb96b3cab,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}, // BMP
	};
	pImgAvatar->Save(_T("d:\\qweqweqweq.png"), arrGUID[0]);
	*/
	ESChildControlTable* pTable = (ESChildControlTable*)GetParent();
	if( pTable )
		pTable->FreeCachedSeatSpeImages(GetSeatIndex());

	m_nAvatarId = nAvatarId;
	m_avatarImage.Destroy();
	m_avatarImage.Attach(pImgAvatar->Detach());
	imgBg.Destroy();
	delete pImgAvatar;

	if( bRedraw )
		Redraw();
	return true;
	}

bool
ESChildSeat::ClearAvatarImage(bool bRedraw){
	if( m_avatarImage.IsNull() )
		return false;
	m_nAvatarId = 0;
	m_avatarImage.Destroy();

	ESChildControlTable* pTable = (ESChildControlTable*)GetParent();
	if( pTable )
		pTable->FreeCachedSeatSpeImages(GetSeatIndex());

	if( bRedraw )
		Redraw();
	return true;
	}

void
ESChildSeat::SetActionName(std::string sAction, int nActionDurationMS, bool bRedraw){
	std::string sName;
	stringEx::Format(sName, _T("_seatActionLabel%d"), m_nSeat + 1);
	ESChildControlLabel* pActionLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.c_str());

	if( !sAction.length() ){
		KillTimer(m_nActionClearTimerId);
		m_nActionClearTimerId = 0;
		if( pActionLabel )
			pActionLabel->SetVisible(false, false);
		}
	else{
		if( m_nActionClearTimerId ){
			KillTimer(m_nActionClearTimerId);
			m_nActionClearTimerId = 0;
			}

		if( pActionLabel ){
			pActionLabel->SetLabel		(sAction, false);
			pActionLabel->SetVisible	(true, false);
			}
		m_sActionName = sAction;
		if( nActionDurationMS != -1 ){
			SetTimer(ActionClearTimerId + m_nSeat, nActionDurationMS);
			m_nActionClearTimerId = ActionClearTimerId + m_nSeat;
			}
		else
			m_nActionClearTimerId = 0;
		}
	if( bRedraw )
		Redraw();
	}

void
ESChildSeat::SetBorderColor(COLORREF crBorder, bool bRedraw){
	if( m_bColoredBorder && m_crBorder == crBorder && !m_imgBorderColored.IsNull() )
		return;

	if( !m_imgBorderColored.IsNull() )
		m_imgBorderColored.Destroy();

	if( /*!m_pBorderColoredImage || m_pBorderColoredImage->IsNull() || */
		!m_pBorderColoredBaseImage || m_pBorderColoredBaseImage->IsNull() )
		return;

	Size szImage;
	if( m_pBorderColoredImage )
		szImage.SetSize(m_pBorderColoredImage->GetWidth(), m_pBorderColoredImage->GetHeight());
	else
		szImage.SetSize(m_pBorderColoredBaseImage->GetWidth(), m_pBorderColoredBaseImage->GetHeight());

	Image bmpDest;
	if( bmpDest.CreateDIBBitmap(32, crBorder, szImage.cx, szImage.cy) )
		return;

	m_pBorderColoredBaseImage->CopyAlphaChannel(bmpDest, Rect(0, 0, 0, 0), false, 250, 0);
	// Render alpha image on border image.
	if( m_pBorderColoredImage )
		bmpDest.BitmapOnBitmap(0, 0, *m_pBorderColoredImage, 0, 0, szImage.cx, szImage.cy, szImage.cx, szImage.cy, 1, 255, 0, true);

	m_imgBorderColored.Attach(bmpDest.Detach());

	ESChildControlTable* pTable = (ESChildControlTable*)GetParent();
	if( pTable )
		pTable->FreeCachedSeatSpeImages(GetSeatIndex());

	m_bColoredBorder	= true;
	m_crBorder			= crBorder;
	}

void
ESChildSeat::ClearBorderColor(bool bRedraw){
	if( !m_bColoredBorder )
		return;
	m_bColoredBorder = false;
	if( !m_imgBorderColored.IsNull() )
		m_imgBorderColored.Destroy();

	ESChildControlTable* pTable = (ESChildControlTable*)GetParent();
	if( pTable )
		pTable->FreeCachedSeatSpeImages(GetSeatIndex());

	if( bRedraw )
		Redraw();
	}

void
ESChildSeat::SetUserNote(std::string sNote, bool bRedraw){
	m_sUserNote = sNote;
	if( bRedraw )
		Redraw();
	}

void
ESChildSeat::ClearUserNote(bool bRedraw){
	if( !m_sUserNote.length() )
		return;
	m_sUserNote.empty();
	if( bRedraw )
		Redraw();
	}

bool
ESChildSeat::GetTooltipText(std::string& sTooltipText){
	std::string sName;
	stringEx::Format(sName, _T("_userName%d"), m_nSeat + 1);
	ESChildControlLabel* pUserLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.c_str());

	stringEx::Format(sName, _T("_userBalance%d"), m_nSeat + 1);
	ESChildControlLabel* pBalanceLabel = (ESChildControlLabel*)m_pOwner->GetChildControlByName(sName.c_str());

	stringEx::Format(sName, _T("_pokerSeatStatus%d"), m_nSeat + 1);
	ESChildControl* pSeatStatus = m_pOwner->GetChildControlByName(sName.c_str());

	std::string sLabel;
	if( /*!m_pSeatInfo ||*/ !pSeatStatus || !pSeatStatus->IsVisible() || !pUserLabel || !pBalanceLabel )
		return false;

	std::string sBalance	= pBalanceLabel->GetLabel();
	std::string sUserName	= pUserLabel->GetLabel();
//	sBalance	.Trim();
//	sUserName	.Trim();
	
	if( m_pSeatInfo ){
		DWORD dwIP = m_pSeatInfo->GetIpAddress();
		std::string sIP;
		stringEx::Format(sIP, _T("%d.%d.%d.%d"), (dwIP&0xFF000000)>>24, (dwIP&0xFF0000)>>16, (dwIP&0xFF00)>>8, (dwIP&0xFF));
		stringEx::Format(sTooltipText, _T("%s (%s)\r\nIP %s"), sUserName.c_str(), sBalance.c_str(), sIP.c_str());
		}
	else
		stringEx::Format(sTooltipText, _T("%s (%s)"), sUserName.c_str(), sBalance.c_str());
	return true;
	}

void
ESChildSeat::OnPaintClient(_DC* pDC, Rect* pRectDC, Rect* pRectClient){
	Rect rcClient				= GetClientRect();
	Rect rcControlDC;
	rcControlDC.left			= (rcClient.left - pRectClient->left) + pRectDC->left;
	rcControlDC.top				= (rcClient.top - pRectClient->top) + pRectDC->top;
	rcControlDC.right			= rcControlDC.left + rcClient.Width();
	rcControlDC.bottom			= rcControlDC.top + rcClient.Height();

	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);

	Size szClient(rcClient.Width(), rcClient.Height());
	Rect rcInvalid							= *pRectDC&rcControlDC;

	bool					bAllowCaching	= false;//m_pOwner->IsDesignerMode() ? false : true;
	ESChildControlTable*	pTable			= (ESChildControlTable*)GetParent();

	// Draw empty seat. {{
	if( m_seatStatus == PokerSeatStatus::Empty && !IsDesignerMode() ){
		if( m_pEmptySeatImage && !m_pEmptySeatImage->IsNull() ){
			if( m_pSeatImage && !m_pSeatImage->IsNull() ){
				Size szSeatImg		(m_pSeatImage->GetWidth()*fZoomCX, m_pSeatImage->GetHeight()*fZoomCY);
				Size szEmptySeatImg(m_pEmptySeatImage->GetWidth()*fZoomCX, m_pEmptySeatImage->GetHeight()*fZoomCY);

				rcControlDC.left	= rcControlDC.left + (szSeatImg.cx - szEmptySeatImg.cx)/2;
				rcControlDC.top		= rcControlDC.top + (szSeatImg.cy - szEmptySeatImg.cy)/2;
				rcControlDC.right	= rcControlDC.left + szEmptySeatImg.cx;
				rcControlDC.bottom	= rcControlDC.top + szEmptySeatImg.cy;

				rcClient.left		= rcClient.left + (szSeatImg.cx - szEmptySeatImg.cx)/2;
				rcClient.top		= rcClient.top + (szSeatImg.cy - szEmptySeatImg.cy)/2;
				rcClient.right		= rcClient.left + szEmptySeatImg.cx;
				rcClient.bottom		= rcClient.top + szEmptySeatImg.cy;

				szClient.SetSize(rcClient.Width(), rcClient.Height());
				}

			if( bAllowCaching ){
				HBITMAP hImageCached = pTable->GetCachedEmptySeatImage();
				Image imgCached(hImageCached);

				if( imgCached.GetWidth() != szClient.cx || imgCached.GetHeight() != szClient.cy ){
					// Update cached image.
					pTable->FreeCachedEmptySeatImage();
					if( m_pEmptySeatImage->ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szClient.cx, szClient.cy, Rect(0, 0, szClient.cx, szClient.cy), hImageCached, (m_pEmptySeatImage->GetBPP() == 32)) )
						pTable->UpdateCachedEmptySeatImage(hImageCached);
					else
						hImageCached = 0L;
					}
				imgCached.Detach();

				// Use cached image.
				if( hImageCached ){
					Image imgTemp;
					imgTemp.Attach(hImageCached);
					m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_CentredImage, RGB(0, 0, 0), &imgTemp, Rect(0, 0, 0, 0), false, (int)ScaleMode::ScaleMode_Bilinier);
					imgTemp.Detach();
					}
				else{
					if( m_pEmptySeatImage && m_pEmptySeatImage->IsNull() )
						m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), m_pEmptySeatImage, m_rcOmitBorder, false, ScaleMode::ScaleMode_Bilinier);
					}
				}
			else
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), m_pEmptySeatImage, Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);

			// Draw empty seat label.
			if( m_bUseEmptySeatLabel && m_sEmptySeatLabelLocal.length() ){
				// Calculate lines.
				CStringArray		arrLines;
				std::string			sLine, sLabel = m_sEmptySeatLabelLocal;
				StringHelper::Split(&sLabel, _T("\r\n"), arrLines);

				FONTDef pFontOld	= NULL;
				Font	font;
				Font*	pFont		= &m_fontEmptySeatLabel;
				if( !m_fontEmptySeatLabel.IsNull() ){
					m_fontEmptySeatLabel.StretchedTextFont(&font, fZoomCX, fZoomCY);
					pFontOld		= pDC->SelectObject(font);
					pFont			= &font;
					}
				else
					pFontOld		= pDC->SelectObject(m_fontEmptySeatLabel);

				Size szSymbol;
				sLine					= _T("X");
				pFont->GetTextSize(&sLine, szSymbol);
				int nHeightTextLines	= arrLines.size()*szSymbol.cy + (arrLines.size() - 1);

				// Draw text. {{
				Size	szText;
				int		nOffsetY		= rcClient.top + (szClient.cy - nHeightTextLines)/2;
			
				for( int nLoop=0; nLoop<arrLines.size(); nLoop++ ){
					sLine = arrLines.at(nLoop);
					pFont->GetTextSize(&sLine, szText);

					Rect	rcLabel;
					if( szText.cx <= szClient.cx )
						rcLabel.left = rcClient.left + (szClient.cx - szText.cx)/2;
					else
						rcLabel.left = rcClient.left + 2;

					rcLabel.top		= nOffsetY;
					rcLabel.right	= rcLabel.left + szText.cx;
					rcLabel.bottom	= rcLabel.top + szText.cy;

					// Translate from window pixel to DC pixel.{{
					Point ptLabel;
					ptLabel.x = (rcLabel.left - pRectClient->left) + pRectDC->left; 
					ptLabel.y = (rcLabel.top - pRectClient->top) + pRectDC->top; 
					//}}

					pDC->SetBkMode		(TRANSPARENT);
					pDC->SetTextColor	(m_crEmptySeatLabel);
					pDC->ExtTextOut		(ptLabel.x, ptLabel.y, ETO_CLIPPED, pRectDC, sLine);
					nOffsetY	+=	szText.cy;
					}

				pDC->SelectObject(pFontOld);
				font.DeleteObject();
				// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				}

			ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
			return;
			}
		}
	// }}

	bool bActive = m_bActive;
	if( m_pSeatInfo && m_pSeatInfo->IsTimerActive() )
		bActive = m_bPulseState;

	bool bBorderIsAroundTheSeat = (m_ptBorderPos.x == -1 && m_ptBorderPos.y == -1);
	Image* pImageCurrBorder	= (m_bColoredBorder && !m_imgBorderColored.IsNull()) ? &m_imgBorderColored : (bActive ? m_pBorderActiveImage : m_pBorderImage);
	Image* pImageCurrSeat		= (m_avatarImage.IsNull() || m_nActionClearTimerId > 0) ? m_pSeatImage : &m_avatarImage;
	if( !bBorderIsAroundTheSeat ){
		pImageCurrSeat			= (bActive && m_pSeatImageActive && !m_pSeatImageActive->IsNull()) ? m_pSeatImageActive :  m_pSeatImage;
		pImageCurrBorder		= (bActive && m_pBorderActiveImage && !m_pBorderActiveImage->IsNull()) ? m_pBorderActiveImage : m_pBorderImage;
		}

	if( bAllowCaching ){
		int			nSeatIndex		= GetSeatIndex();
		bool		bAvatar			= !(m_avatarImage.IsNull() || m_nActionClearTimerId > 0), bColorBorder = (m_bColoredBorder && !m_imgBorderColored.IsNull());
		ImageDef	hImageCached	= pTable->GetCachedSeatImage(nSeatIndex, bActive, bColorBorder, bAvatar);
		Image		imgCached(hImageCached);

		// Update cached image.
		if( (imgCached.GetWidth() != szClient.cx || imgCached.GetHeight() != szClient.cy) ){
			pTable->FreeCachedSeatImage(nSeatIndex, bActive, bColorBorder, bAvatar);
			hImageCached = NULL;

			if( pImageCurrSeat && !pImageCurrSeat->IsNull() && pImageCurrBorder && !pImageCurrBorder->IsNull() ){
				ImageDef hCombined			= Image::CombineTwo32bppImages(*pImageCurrSeat, *pImageCurrBorder);
				ImageDef hSeatAvatarScaled	= 0L;
				if( hCombined ){
					Image imgCombined;
					imgCombined.Attach		(hCombined);
					imgCombined.ScaleBitmap	(ScaleMode::ScaleMode_Bilinier, szClient.cx, szClient.cy, Rect(0, 0, szClient.cx, szClient.cy), hSeatAvatarScaled, true);
					imgCombined.Destroy		();
					hImageCached = hSeatAvatarScaled;
					imgCached.Detach();
					imgCached.Attach(hImageCached);
					}

				/*// old
				HBITMAP hSeatAvatarScaled = 0L, hBorderScaled = 0L;
				if( ImageHelper::ScaleBitmap(ScaleMode::ScaleMode_Bilinier, *pImageCurrSeat, szClient.cx, szClient.cy, Rect(0, 0, szClient.cx, szClient.cy), hSeatAvatarScaled, true) &&
					ImageHelper::ScaleBitmap(ScaleMode::ScaleMode_Bilinier, *pImageCurrBorder, szClient.cx, szClient.cy, Rect(0, 0, szClient.cx, szClient.cy), hBorderScaled, true) )
					{
					ImageHelper::BitmapOnBitmap		(hSeatAvatarScaled, 0, 0, hBorderScaled, 0, 0, szClient.cx, szClient.cy, szClient.cx, szClient.cy, 1, 255, 0, true);
					ImageHelper::CopyAlphaChannel	(hSeatAvatarScaled, hBorderScaled, Rect(0, 0, 0, 0), true, 250);

					hImageCached = hSeatAvatarScaled; 
					::DeleteObject(hBorderScaled);
					}
				else{
					if( hSeatAvatarScaled )
						::DeleteObject(hSeatAvatarScaled);
					if( hBorderScaled )
						::DeleteObject(hBorderScaled);
					}*/
				}
			pTable->UpdateCachedSeatImage(nSeatIndex, hImageCached, bActive, bColorBorder, bAvatar);
			}

		// Use cached image.
		if( !imgCached.IsNull() ){
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_CentredImage, RGB(0, 0, 0), &imgCached, Rect(0, 0, 0, 0), false, (int)ScaleMode::ScaleMode_Bilinier);
			imgCached.Detach();
			}
		}
	else{
		Rect rcClient_Border;
		Size szSeatImg			(m_pSeatImage->GetWidth()*fZoomCX, m_pSeatImage->GetHeight()*fZoomCY);
		Size szEmptySeatImg	(m_pEmptySeatImage->GetWidth()*fZoomCX, m_pEmptySeatImage->GetHeight()*fZoomCY);
		Size szSeatBorderImg	(m_pBorderImage->GetWidth()*fZoomCX, m_pBorderImage->GetHeight()*fZoomCY);

		// Seat or avatar image.
		if( pImageCurrSeat && !pImageCurrSeat->IsNull() )
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pImageCurrSeat, Rect(0,0,0,0), false, ScaleMode::ScaleMode_Bilinier);

		if( !bBorderIsAroundTheSeat ){
			rcClient_Border.left		= rcClient.left + m_ptBorderPos.x*fZoomCX;
			rcClient_Border.top			= rcClient.top + m_ptBorderPos.y*fZoomCY;
			rcClient_Border.right		= rcClient_Border.left + szSeatBorderImg.cx;
			rcClient_Border.bottom		= rcClient_Border.top + szSeatBorderImg.cy;

			// Draw avatar.
			if( !m_avatarImage.IsNull() ){
				Rect rcClient_Avatar;
				Size szAvatar(m_avatarImage.GetWidth()*fZoomCX, m_avatarImage.GetHeight()*fZoomCY);
				rcClient_Avatar.left		= rcClient_Border.left + (rcClient_Border.Width() - szAvatar.cx)/2;
				rcClient_Avatar.top			= rcClient_Border.top + (rcClient_Border.Height() - szAvatar.cy)/2;
				rcClient_Avatar.right		= rcClient_Avatar.left + szAvatar.cx;
				rcClient_Avatar.bottom		= rcClient_Avatar.top + szAvatar.cy;
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient_Avatar, FillMode::FillMode_StretchImage, RGB(0, 0, 0), &m_avatarImage, Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
				}

			// Draw border.
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient_Border, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pImageCurrBorder, Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
			// Draw color border.
			if( m_bColoredBorder && !m_imgBorderColored.IsNull() )
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), &m_imgBorderColored, Rect(0, 0, 0, 0), false, ScaleMode::ScaleMode_Bilinier);
			}
		else{
			// Border image.
			if( pImageCurrBorder && !pImageCurrBorder->IsNull() )
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pImageCurrBorder, m_rcOmitBorder, false, ScaleMode::ScaleMode_Bilinier);
			}
		}

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildSeat::OnMouseEnter(UINT nFlags){
	ESChildControl::OnMouseEnter(nFlags);
	}

void
ESChildSeat::OnLButtonClick(Point pt, UINT nFlags){
	SendEvent(this, ChildControlEventCode::EventCode_ControlClick);
	}

void
ESChildSeat::OnRButtonUp(Point pt, UINT nFlags){
	SendEvent(this, ChildControlEventCode::EventCode_OnContextMenu);
	}

void	
ESChildSeat::OnLButtonDblClick(Point pt, UINT nFlags){
	OnLButtonClick(pt, nFlags);
	}

RGNDef
ESChildSeat::GetControlRgn(Size& szRgnBound, bool& bCentered){
	if( IsDesignerMode() )
		return NULL;

	Rgn*	pRgn = NULL;
	if( m_seatStatus == PokerSeatStatus::Empty ){
		pRgn		=  ESChildControlTable::GetEmptySeatImageRgn();
		szRgnBound	= m_pEmptySeatImage ? Size(m_pEmptySeatImage->GetWidth(), m_pEmptySeatImage->GetHeight()) : Size(0, 0);
		}
	else{
		pRgn		=  ESChildControlTable::GetSeatImageRgn();
		szRgnBound	= m_pSeatImage ? Size(m_pSeatImage->GetWidth(), m_pSeatImage->GetHeight()) : Size(0, 0);
		}
	bCentered = true;
	return *pRgn;
	};

void
ESChildSeat::OnDeserializationCompleted(){
	ESChildControl::OnDeserializationCompleted();
	}

void
ESChildSeat::OnLocalizeStrings(){
	m_sEmptySeatLabelLocal = m_sEmptySeatLabel;
	if( !m_pOwner->IsDesignerMode() )
		ESLocalizationMan::ParseText(m_sEmptySeatLabelLocal);
	}

bool
ESChildSeat::ColorBorderModify(ESChildSeat* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->m_pBorderColoredImage		= NULL;
	pThis->m_pBorderColoredBaseImage	= NULL;
	pThis->m_imgBorderColored.Destroy();
	pThis->UpdateImages		();
	pThis->Redraw			();
	return true;
	}

bool
ESChildSeat::ImageModify(ESChildSeat* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->m_pBorderImage			= NULL;
	pThis->m_pBorderActiveImage		= NULL;
	pThis->m_pSeatImage				= NULL;
	pThis->m_pEmptySeatImage		= NULL;
	pThis->UpdateImages	();
	pThis->Redraw		();
	return true;
	}

bool
ESChildSeat::AttributeModify(ESChildSeat* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->Redraw		();
	return true;
	}