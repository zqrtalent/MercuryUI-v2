// ESChildControlTable.cpp : implementation file
//

#include "ESChildControlTable.h"
#include "ESChildControlTableCards.h"
#include "ESChildControlSeatCards.h"
#include "ESChildSeat.h"
#include "../../../Path/Path.h"
#include "../PokerGUI.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTable

ESChildControlTable::ESChildControlTable(){
	m_sClassName								= _T("ESChildControlTable");
	m_nMotionTimeMS								= 250;
	m_nTimeDistanceBetweenSeatCardAnimation		= 20;
	m_nTimeDistanceBetweenCardAnimation			= 50;
	m_nFrameCtPerSecond							= 20;
	m_btCardAlphaAtStart						= 255;

	// Supports two type of animation of flop cards, slide and card motion.
	m_tableCardsAnimation.nMotionTimeMS			= 200;						
	m_tableCardsAnimation.nFrameCtPerSecond		= 20;	
	m_tableCardsAnimation.nFlopCardsMotionTimeMS= 300;
	m_tableCardsAnimation.nTimeDistanceBetweenCardAnimation= 90;
	m_tableCardsAnimation.btAlphaAtStart		= 255;
	m_tableCardsAnimation.bSlideCardsOnFlop		= true;

	m_collectChipsAnimation.nMotionTimeMS		= 200;						
	m_collectChipsAnimation.nFrameCtPerSecond	= 20;	
	m_collectChipsAnimation.nDelayAfterMotionMS	= 500;

	m_moveChipsAnimation.nMotionTimeMS			= 200;						
	m_moveChipsAnimation.nFrameCtPerSecond		= 20;		

	StringToIntArray* pMotions[]= {&m_seatCardsDealMotionTypeX, &m_seatCardsDealMotionTypeY, 
	&m_tableCardsAnimation.motionTypeX, &m_tableCardsAnimation.motionTypeY,
	&m_collectChipsAnimation.motionTypeX, &m_collectChipsAnimation.motionTypeY,
	&m_moveChipsAnimation.motionTypeX, &m_moveChipsAnimation.motionTypeY};

	for(int i=0; i<sizeof(pMotions)/sizeof(StringToIntArray*); i++){
		StringToIntArray* pMotionTypes = pMotions[i];
		pMotionTypes->Add(_T("MotionCircularIn"),	 ESEasyMotionType::MotionCircularIn);
		pMotionTypes->Add(_T("MotionCircularInOut"), ESEasyMotionType::MotionCircularInOut);
		pMotionTypes->Add(_T("MotionCircularOut"), ESEasyMotionType::MotionCircularOut);
		pMotionTypes->Add(_T("MotionCubicIn"), ESEasyMotionType::MotionCubicIn);
		pMotionTypes->Add(_T("MotionCubicInOut"), ESEasyMotionType::MotionCubicInOut);
		pMotionTypes->Add(_T("MotionCubicOut"), ESEasyMotionType::MotionCubicOut);
		pMotionTypes->Add(_T("MotionExponentialIn"), ESEasyMotionType::MotionExponentialIn);
		pMotionTypes->Add(_T("MotionExponentialInOut"), ESEasyMotionType::MotionExponentialInOut);
		pMotionTypes->Add(_T("MotionExponentialOut"), ESEasyMotionType::MotionExponentialOut);
		pMotionTypes->Add(_T("MotionLinier"), ESEasyMotionType::MotionLinier);
		pMotionTypes->Add(_T("MotionQuadraticIn"), ESEasyMotionType::MotionQuadraticIn);
		pMotionTypes->Add(_T("MotionQuadraticInOut"), ESEasyMotionType::MotionQuadraticInOut);
		pMotionTypes->Add(_T("MotionQuadraticOut"), ESEasyMotionType::MotionQuadraticOut);
		pMotionTypes->Add(_T("MotionQuarticIn"), ESEasyMotionType::MotionQuarticIn);
		pMotionTypes->Add(_T("MotionQuarticInOut"), ESEasyMotionType::MotionQuarticInOut);
		pMotionTypes->Add(_T("MotionQuarticOut"), ESEasyMotionType::MotionQuarticOut);
		pMotionTypes->Add(_T("MotionQuinticIn"), ESEasyMotionType::MotionQuinticIn);
		pMotionTypes->Add(_T("MotionQuinticInOut"), ESEasyMotionType::MotionQuinticInOut);
		pMotionTypes->Add(_T("MotionQuinticOut"), ESEasyMotionType::MotionQuinticOut);
		pMotionTypes->Add(_T("MotionSinusoidalIn"), ESEasyMotionType::MotionSinusoidalIn);
		pMotionTypes->Add(_T("MotionSinusoidalInOut"), ESEasyMotionType::MotionSinusoidalInOut);
		pMotionTypes->Add(_T("MotionSinusoidalOut"), ESEasyMotionType::MotionSinusoidalOut);

		pMotionTypes->SetCurrentValue(ESEasyMotionType::MotionQuadraticInOut);
		}
	}

BEGIN_DECLARE_VAR(ESChildControlTable, ESChildControl)
DECLARE_VAR(_T("ESChildControlTable"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Background Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sBgImageFile), false, false, false)
	DECLARE_VAR(_T("Border Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sBorderImageFile), false, false, false)
	DECLARE_VAR(_T("Foreground Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sFgImageFile), false, false, false)
	DECLARE_VAR(_T("Foreground Color"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlTable,m_crFgColor), false, false, false)
	DECLARE_VAR(_T("LogoImage"),				_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sLogoImage), false, false, false)

	DECLARE_VAR(_T("Layer 1 Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sLayer1ImageFile), false, false, false)
	DECLARE_VAR(_T("Layer 2 Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sLayer2ImageFile), false, false, false)
	DECLARE_VAR(_T("Layer 3 Image File"),		_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sLayer3ImageFile), false, false, false)
	DECLARE_VAR(_T("Chip images folder"),		_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sChipImagesFolder), false, false, false)
	DECLARE_VAR(_T("Card images folder"),		_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sCardImagesFolder), false, false, false)
	DECLARE_VAR(_T("Card Name Using"),			_T(""), VariableType::VT_String, offsetof(ESChildControlTable,m_sCardName), false, false, false)
	
	// Seat cards deal animation.
	DECLARE_VAR(_T("SeatCardsDealMotionCX"),	_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_seatCardsDealMotionTypeX), false, false, false)
	DECLARE_VAR(_T("SeatCardsDealMotionCY"),	_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_seatCardsDealMotionTypeY), false, false, false)
	DECLARE_VAR(_T("MotionTimeMS"),				_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("TimeDistanceSeatCardAnim"),	_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_nTimeDistanceBetweenSeatCardAnimation), false, false, false)
	DECLARE_VAR(_T("TimeDistanceCardAnim"),		_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_nTimeDistanceBetweenCardAnimation), false, false, false)
	DECLARE_VAR(_T("FrameCtPerSecond"),			_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_nFrameCtPerSecond), false, false, false)	
	DECLARE_VAR(_T("CardAlphaAtStart"),			_T(""), VariableType::VT_Int8, offsetof(ESChildControlTable,m_btCardAlphaAtStart), false, false, false)

	// Table cards animation.
	DECLARE_VAR(_T("TableCardsDealMotionX"),	_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_tableCardsAnimation.motionTypeX), false, false, false)
	DECLARE_VAR(_T("TableCardsDealMotionY"),	_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_tableCardsAnimation.motionTypeY), false, false, false)
	DECLARE_VAR(_T("TableCardsMotionTimeMS"),	_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_tableCardsAnimation.nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("TableFlopCardsMotionTimeMS"),_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_tableCardsAnimation.nFlopCardsMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("TableCardsMotionFrameCt"),	_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_tableCardsAnimation.nFrameCtPerSecond), false, false, false)
	DECLARE_VAR(_T("TableAnimDistBetweenCards"),_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_tableCardsAnimation.nTimeDistanceBetweenCardAnimation), false, false, false)
	DECLARE_VAR(_T("TableAnimCardAlphaAtStart"),_T(""), VariableType::VT_Int8, offsetof(ESChildControlTable,m_tableCardsAnimation.btAlphaAtStart), false, false, false)
	DECLARE_VAR(_T("SlideFlopCardsAnim"),		_T(""), VariableType::VT_Bool, offsetof(ESChildControlTable,m_tableCardsAnimation.bSlideCardsOnFlop), false, false, false)
	
	// Collect chips animation.
	DECLARE_VAR(_T("CollectChipsMotionX"),		_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_collectChipsAnimation.motionTypeX), false, false, false)
	DECLARE_VAR(_T("CollectChipsMotionY"),		_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_collectChipsAnimation.motionTypeY), false, false, false)
	DECLARE_VAR(_T("CollectChipsMotionTimeMS"),	_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_collectChipsAnimation.nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("CollectChipsMotionFrameCt"),_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_collectChipsAnimation.nFrameCtPerSecond), false, false, false)
	DECLARE_VAR(_T("CollectChipsDelayAfterMotionMS"),_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_collectChipsAnimation.nDelayAfterMotionMS), false, false, false)

	// Move chips animation.
	DECLARE_VAR(_T("MoveChipsMotionX"),			_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_moveChipsAnimation.motionTypeX), false, false, false)
	DECLARE_VAR(_T("MoveChipsMotionY"),			_T(""), VariableType::VT_Enumerable, offsetof(ESChildControlTable,m_moveChipsAnimation.motionTypeY), false, false, false)
	DECLARE_VAR(_T("MoveChipsMotionTimeMS"),	_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_moveChipsAnimation.nMotionTimeMS), false, false, false)
	DECLARE_VAR(_T("MoveChipsMotionFrameCt"),	_T(""), VariableType::VT_Int32, offsetof(ESChildControlTable,m_moveChipsAnimation.nFrameCtPerSecond), false, false, false)
END_DECLARE_VAR()

ESChildControlTable::~ESChildControlTable(){
	}

bool
ESChildControlTable::SetTableColor(COLORREF crTable, bool bUpdate){
	if( m_crFgColor == crTable )
		return false;
	m_crFgColor = crTable;
	if( bUpdate )
		UpdateTableImage(true);
	return true;
	}

bool
ESChildControlTable::SetTableBgImage(const std::string sBgImageFIle, bool bUpdate){
	MercuryGUI* pGUI = MercuryGUI::GetInstance();
	std::string sBgImageOld = m_sBgImageFile;
//	if( !sBgImageOld.CompareNoCase(sBgImageFIle) ) 
	if( !sBgImageOld.compare(sBgImageFIle) ) 
		return false;
	m_sBgImageFile = sBgImageFIle;

	if( bUpdate ){
		UpdateTableImage(true);

		Image* pBgImageOldCached = pGUI->GetCachedImage(sBgImageOld);
		if( pBgImageOldCached )
			pGUI->DeleteCachedImage(sBgImageOld);
		}
	return true;
	}

void
ESChildControlTable::UpdateTableImage(bool bUpdateExisting){
	MercuryGUI* pGUI			= MercuryGUI::GetInstance();
	Image*		pTableImageDef	= GetTableImageDefault();
	if( !bUpdateExisting && pTableImageDef && !pTableImageDef->IsNull() ){
		return;
		}

	*GetTableColorApplied()			= m_crFgColor;
	*GetTableBgImageFileApplied()	= m_sBgImageFile;

	std::string sImage		= m_sBgImageFile;
	Image* pBgImage			= pGUI->LoadImage(sImage, -1, -1, true, false);
	sImage = m_sBorderImageFile;
	Image* pBorderImage		= pGUI->LoadImage(sImage);
	sImage = m_sFgImageFile;
	Image* pFgImage			= pGUI->LoadImage(sImage);
	sImage = m_sLogoImage;
	Image* pLogoImage		= pGUI->LoadImage(sImage);
	sImage = m_sLayer1ImageFile;
	Image* pLayer1Image		= pGUI->LoadImage(sImage);
	sImage = m_sLayer2ImageFile;
	Image* pLayer2Image		= pGUI->LoadImage(sImage);
	sImage = m_sLayer3ImageFile;
	Image* pLayer3Image		= pGUI->LoadImage(sImage);

	// Save table fg image rgn. {{
	if( !m_pOwner || !m_pOwner->IsDesignerMode() ){
		Rgn* pTableImageRgn = GetTableImageRgn();
		if( pTableImageRgn->IsNull() && pFgImage && !pFgImage->IsNull() ){
			RGNDef hRgn = pFgImage->GetBitmap32bppRgnByAlphaChannel();
			if( hRgn )
				pTableImageRgn->Attach(hRgn);
			}
		}
	// }}

	Image	imageFgMultiply;
	if( pFgImage ){
		pFgImage->CopyImage(&imageFgMultiply);

		if( pLogoImage && !pLogoImage->IsNull() ){
			Point ptDest;
			ptDest.x = (imageFgMultiply.GetWidth() - pLogoImage->GetWidth()) / 2;
			ptDest.y = (imageFgMultiply.GetHeight() - pLogoImage->GetHeight()) / 2;
			imageFgMultiply.BitmapOnBitmap(ptDest.x, ptDest.y, *pLogoImage, 0, 0, 
				pLogoImage->GetWidth(), pLogoImage->GetHeight(), pLogoImage->GetWidth(), pLogoImage->GetHeight(), (pLogoImage->GetBPP() == 32), 255, 0, true);
			}

		imageFgMultiply.MultiplyBlendImageBpp32(m_crFgColor);
		}

	Image* pImage	= Image::CombineImages(pBgImage, pBorderImage, false);
	if( pImage ){
		Image* pImageTable = Image::CombineImages(pImage, &imageFgMultiply, false);
		if( pLayer1Image && !pLayer1Image->IsNull() ){
			Image* pImageNew	= Image::CombineImages(pImageTable, pLayer1Image, false);
			delete pImageTable;
			pImageTable			= pImageNew;
			}
		
		/*
		if( pLayer2Image && !pLayer2Image->IsNull() ){
			Image* pImageNew	= PokerGUI::CombineImages(pImageTable, pLayer2Image, false);
			delete pImageTable;
			pImageTable			= pImageNew;
			}
		*/
		if( pLayer3Image && !pLayer3Image->IsNull() ){
			Image* pImageNew	= Image::CombineImages(pImageTable, pLayer3Image, false);
			delete pImageTable;
			pImageTable			= pImageNew;
			}

		if( !pTableImageDef->IsNull() )
			pTableImageDef->Destroy();
		pTableImageDef->Attach(pImageTable->Detach());
		/*
		GUID guidPng = {0xb96b3caf,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e};
		HBITMAP hBmStretched = NULL;
		if( ImageHelper::ScaleBitmap(ScaleMode::ScaleMode_NearestNeghbore, m_imageScreen, 0, 0, 396, 273, 660, 455, hBmStretched) ){
			Image imgSave;
			imgSave.Attach(hBmStretched);
			imgSave.Save("d:\\1_xxx_2.png", guidPng);
			}
			*/
		delete pImage;
		}
	imageFgMultiply.Destroy();

	// Free cached table image.
	FreeCachedTableImage();
	}

bool
ESChildControlTable::LoadCardImages(std::string sImageFolder, std::string sCardName, bool bUpdateExisting){
	std::string sCardImagesFolder = sImageFolder;
	if( sCardImagesFolder[sCardImagesFolder.length() - 1] == '\\'  ){
		sCardImagesFolder	.erase(sCardImagesFolder.length() - 1, 1);
		sImageFolder		.erase(sImageFolder.length() - 1, 1);
		}
	if( m_sCardName.length() )
		sCardImagesFolder += _T("\\") + sCardName;

	Path::BuildFullPath(sCardImagesFolder);
	Path::BuildFullPath(sImageFolder);

	EnumerableObject<CardsInfo>* pListCardInfos = GetCardsInfoInstance();
	if( !bUpdateExisting && pListCardInfos && pListCardInfos->GetCount() > 0 )
		return true;
	pListCardInfos->RemoveAll(true);

	CStringArray arrGroupFolders;
	if( Path::EnumerateFolders(sCardImagesFolder, arrGroupFolders) <= 0 )
		return false;

	MercuryGUI* pGUI		= MercuryGUI::GetInstance();
	CardsInfo*	pCardsInfo	= new CardsInfo();
	pListCardInfos->Add(pCardsInfo);

	// Load card back images. {{
	std::string		sImage;
	std::string		sImageNames[]		= {_T("smback"), _T("back"), _T("medback"), _T("lgback"), (!sCardName.length() ? _T("deck") : sCardName + _T("\\deck"))};
	Image*			ppImages[]			= {&pCardsInfo->m_imgBackSm, &pCardsInfo->m_imgBack, &pCardsInfo->m_imgBackMed, &pCardsInfo->m_imgBackLg, &pCardsInfo->m_imgDeck};
	Size*			ppImgSizes[]		= {&pCardsInfo->m_szBackSm, &pCardsInfo->m_szBack, &pCardsInfo->m_szBackMed, &pCardsInfo->m_szBackLg, NULL};
	List<Image>*	ppCardBackImages[]	= {&pCardsInfo->m_listBackImagesSm, &pCardsInfo->m_listBackImages, &pCardsInfo->m_listBackImagesMed, &pCardsInfo->m_listBackImagesLg};

	for(int i=0; i<sizeof(sImageNames)/sizeof(std::string); i++){
		stringEx::Format(sImage, _T("%s\\%s.png"), sImageFolder.c_str(), sImageNames[i].c_str());
		Image* pImage = pGUI->LoadImage(sImage, -1, -1, false);
		if( !pImage ){
			stringEx::Format(sImage, _T("%s\\%s.bmp"), sImageFolder.c_str(), sImageNames[i].c_str());
			Image* pImageBmp = pGUI->LoadImage(sImage, -1, -1, false);

			stringEx::Format(sImage, _T("%s\\%s.a.bmp"), sImageFolder.c_str(), sImageNames[i].c_str());
			Image* pImageAlphaBmp = pGUI->LoadImage(sImage, -1, -1, false);

			if( pImageBmp && pImageAlphaBmp ){
				pImage = Image::CombineImages(pImageBmp, pImageAlphaBmp, true);
				if( pImage )
					ppImages[i]->Attach(pImage->Detach());
				}
			else{
				if( pImageBmp )
					ppImages[i]->Attach(pImageBmp->Detach());
				}
			if( pImageBmp )
				delete pImageBmp;
			if( pImageAlphaBmp )
				delete pImageAlphaBmp;
			}
		else
			ppImages[i]->Attach(pImage->Detach());

		if( ppImgSizes[i] && ppImages[i] && !ppImages[i]->IsNull() ){
			Size szImage(ppImages[i]->GetWidth() / 18, ppImages[i]->GetHeight());

			Rect rcBackImg;
			rcBackImg.top				= 0;
			rcBackImg.bottom			= rcBackImg.top + szImage.cy;
			for(int j=0; j<(ppImages[i]->GetWidth() / szImage.cx); j++){
				rcBackImg.left			= szImage.cx*j;
				rcBackImg.right			= rcBackImg.left + szImage.cx;

				Image* pImageBack		= new Image();
				if( ppImages[i]->GetImageFromImage(rcBackImg, *pImageBack) ){
					ppCardBackImages[i]->Add(pImageBack);
					}
				else{
					ppCardBackImages[i]->Add(NULL);
					delete pImageBack;
					}
				}
			*ppImgSizes[i] = szImage;

			// Add back image list. {{
			if( ppCardBackImages[i]->GetCount() > 0 ){
				int nCombine = szImage.cx;
				nCombine	<<= 16;
				nCombine	|= szImage.cy;
				pCardsInfo->m_arrBackImagesOrderedByDimensions.Add((void*)nCombine, (void*)ppCardBackImages[i]);
				}
			// }}
			}
		}
	// }}

	// Load card groups. {{
	Image imgCardBase, imgCardFace;
	std::string sImageNamesGroup[]	= {_T("cardbase"), _T("cardface"), _T("cardrank"), _T("cardsuit"), _T("deck")};
	for(int i=0; i<arrGroupFolders.size(); i++){
		std::string		sFolder		= arrGroupFolders.at(i);
		CardsGroup*		pCardsGroup = new CardsGroup();
		pCardsInfo->m_listCards.Add(pCardsGroup);

		Image*	ppImagesGroup[]		= {&imgCardBase, &imgCardFace, &pCardsGroup->m_imgCardRank, &pCardsGroup->m_imgCardSuit, &pCardsGroup->m_imgCardDeck};
		for(int j=0; j<sizeof(sImageNamesGroup)/sizeof(std::string); j++){
			stringEx::Format(sImage, _T("%s\\%s.png"), sFolder, sImageNamesGroup[j]);
			Image* pImage = pGUI->LoadImage(sImage, -1, -1, false, false);
			if( !pImage ){
				stringEx::Format(sImage, _T("%s\\%s.bmp"), sFolder, sImageNamesGroup[j]);
				Image* pImageBmp = pGUI->LoadImage(sImage, -1, -1, false, false);

				stringEx::Format(sImage, _T("%s\\%s.a.bmp"), sFolder, sImageNamesGroup[j]);
				Image* pImageAlphaBmp = pGUI->LoadImage(sImage, -1, -1, false, false);

				if( pImageBmp && pImageAlphaBmp ){
					pImage = Image::CombineImages(pImageBmp, pImageAlphaBmp, true);
					if( pImage ){
						ppImagesGroup[j]->Attach(pImage->Detach());
						delete pImage;
						}
					}
				else{
					if( pImageBmp )
						ppImagesGroup[j]->Attach(pImageBmp->Detach());
					}
			
				if( pImageBmp )
					delete pImageBmp;
				if( pImageAlphaBmp )
					delete pImageAlphaBmp;
				}
			else
				ppImagesGroup[j]->Attach(pImage->Detach());
			}

		if( !pCardsGroup->m_imgCardSuit.IsNull() ){
			Size		szSuit(pCardsGroup->m_imgCardSuit.GetWidth() / 6, pCardsGroup->m_imgCardSuit.GetHeight());
			Image*		ppSuitImages[] = {&pCardsGroup->m_imgCardSuit1, &pCardsGroup->m_imgCardSuit2, &pCardsGroup->m_imgCardSuit3, &pCardsGroup->m_imgCardSuit4, &pCardsGroup->m_imgCardSuit5, &pCardsGroup->m_imgCardSuit6};
			for(int i=0; i<6; i++){
				Rect rcSuitImg;
				rcSuitImg.left			= szSuit.cx*i;
				rcSuitImg.top			= 0;
				rcSuitImg.right			= rcSuitImg.left + szSuit.cx;
				rcSuitImg.bottom		= rcSuitImg.top + szSuit.cy;

				Image imgSuit;
				if( pCardsGroup->m_imgCardSuit.GetImageFromImage(rcSuitImg, imgSuit) )
					ppSuitImages[i]->Attach(imgSuit.Detach());
				}
			}

		if( !imgCardBase.IsNull() ){
			// Multi suit card bg is available.
			if( imgCardBase.GetWidth() > 2*imgCardBase.GetHeight() ){
				pCardsGroup->m_szCard.SetSize(imgCardBase.GetWidth()/6, imgCardBase.GetHeight());
				pCardsGroup->m_bMultiSuitBg = true;
				Size		szSuitBg(imgCardBase.GetWidth()/6, imgCardBase.GetHeight());
				Image*		ppSuitImages[] = {&pCardsGroup->m_imgCardBg1, &pCardsGroup->m_imgCardBg2, &pCardsGroup->m_imgCardBg3, &pCardsGroup->m_imgCardBg4, &pCardsGroup->m_imgCardBg5, &pCardsGroup->m_imgCardBg6};
				for(int i=0; i<6; i++){
					Rect rcSuitBgImg;
					rcSuitBgImg.left		= szSuitBg.cx*i;
					rcSuitBgImg.top			= 0;
					rcSuitBgImg.right		= rcSuitBgImg.left + szSuitBg.cx;
					rcSuitBgImg.bottom		= rcSuitBgImg.top + szSuitBg.cy;

					Image imgSuitBg;
					if( imgCardBase.GetImageFromImage(rcSuitBgImg, imgSuitBg) )
						ppSuitImages[i]->Attach(imgSuitBg.Detach());
					}
				}
			else
				pCardsGroup->m_szCard.SetSize(imgCardBase.GetWidth(), imgCardBase.GetHeight());

			if( !imgCardFace.IsNull() ){
				Image* pImage = Image::CombineImages(&imgCardBase, &imgCardFace, false, 32);
				if( pImage )
					pCardsGroup->m_imgCardBg.Attach(pImage->Detach());
				}
			else
				pCardsGroup->m_imgCardBg.Attach(imgCardBase.Detach());
			}

		if( !imgCardFace.IsNull() ){
			Rect		rcFaceRect(-1, -1, -1, -1);
			int			nCardFaceWidth = imgCardFace.GetWidth(), nCardFaceHeight = imgCardFace.GetHeight();
			const void*	lpBits	= imgCardFace.GetBits();
			RGBQUAD*	pPixel	= (RGBQUAD*)lpBits;
			for(int y=0; y<imgCardFace.GetHeight(); y++){
				pPixel = (RGBQUAD*)(((long)lpBits) + (nCardFaceHeight - y - 1)*imgCardFace.GetWidthBytes());
				int nStart = -1, nEnd = -1;
				for(int x=0; x<nCardFaceWidth; x++){
					if( pPixel->rgbReserved > 0 ){
						if( nStart == -1 ){
							nStart	= x;
							nEnd	= nStart;
							}
						else
							nEnd = x;
						}
					pPixel ++;
					}

				if( nStart < nEnd ){
					if( rcFaceRect.left == -1 ){
						rcFaceRect.left		= nStart;
						rcFaceRect.top		= y;
						rcFaceRect.right	= nEnd;
						}
					else
						rcFaceRect.bottom	= y;
					}
				}

			if( !rcFaceRect.IsRectEmpty() ){
				rcFaceRect.DeflateRect(1, 1, 1, 1);
				pCardsGroup->m_rcDeck = rcFaceRect;
				}
			else
				pCardsGroup->m_rcDeck.SetRect(0, 0, 0, 0);
			}
		else
			pCardsGroup->m_rcDeck.SetRect(0, 0, 0, 0);

		if( !imgCardBase.IsNull() )
			imgCardBase.Destroy();
		if( !imgCardFace.IsNull() )
			imgCardFace.Destroy();
		}
	// }}
	return true;
	}

bool
ESChildControlTable::LoadChipImages(std::string sImageFolder, bool bUpdateExisting){
	Path::BuildFullPath(sImageFolder);
	EnumerableObject<ChipsGroupInfo>* pListGroups = GetChipsGroupInstance();
	if( !bUpdateExisting && pListGroups && pListGroups->GetCount() > 0 )
		return true;
	pListGroups->RemoveAll(true);

	CStringArray arrGroupFolders;
	if( Path::EnumerateFolders(sImageFolder, arrGroupFolders) <= 0 )
		return false;

	std::string arrChipImages[] = 
		{
		_T("chip000001"), _T("chip000005"), _T("chip000025"), _T("chip0001"),
		_T("chip0005"), _T("chip0025"), _T("chip0100"), _T("chip0500"),
		_T("chip1000"), _T("chip5000"), _T("chip25000"), _T("chip100000"),
		_T("chip500000"), _T("chip1000000"), _T("chip5000000"), _T("chip25000000"),
		_T("chip100000000"), _T("chip500000000"), _T("chip1000000000"), _T("chip5000000000"),
		_T("chip25000000000")
		};

	__int64 nChipAmounts[] = 
		{
		1, 5, 25, 100, 500, 2500, 10000, 50000,
		100000, 500000, 2500000, 10000000, 
		50000000, 100000000, 500000000, 2500000000,
		10000000000, 50000000000, 100000000000, 
		500000000000, 2500000000000
		};

	MercuryGUI* pGUI = MercuryGUI::GetInstance();
	for(int i=0; i<arrGroupFolders.size(); i++){
		std::string sFolder = arrGroupFolders.at(i);
		std::string sImage;

		ChipsGroupInfo* pGroup = new ChipsGroupInfo();
		pListGroups->Add(pGroup);

		for(int j=0; j<sizeof(nChipAmounts)/sizeof(__int64); j++){
			stringEx::Format(sImage, _T("%s\\%s.png"), sFolder.c_str(), arrChipImages[j].c_str());
			Image* pImage = pGUI->LoadImage(sImage, -1, -1, false);
			if( !pImage ){
				stringEx::Format(sImage, _T("%s\\%s.bmp"), sFolder.c_str(), arrChipImages[j].c_str());
				Image* pImageBmp = pGUI->LoadImage(sImage, -1, -1, false);

				stringEx::Format(sImage, _T("%s\\%s.a.bmp"), sFolder.c_str(), arrChipImages[j].c_str());
				Image* pImageAlphaBmp = pGUI->LoadImage(sImage, -1, -1, false, false);

				if( pImageBmp && pImageAlphaBmp )
					pImage = Image::CombineImages(pImageBmp, pImageAlphaBmp, true);
			
				delete pImageBmp;
				delete pImageAlphaBmp;
				}

			if( pImage && !pImage->IsNull() ){
				ChipInfo* pInfo		= new ChipInfo();
				pGroup->m_szChips.SetSize(pImage->GetWidth(), pImage->GetHeight());
				pGroup->m_listChips.Add(pInfo);

				pInfo->m_chipImage.Attach(pImage->Detach());
//				pInfo->m_chipImage.SetHasAlphaChannel(true);
				pInfo->m_szImage	= pGroup->m_szChips;
				stringEx::Format(pInfo->m_sImageFile, _T("%s\\%s.png"), sFolder.c_str(), arrChipImages[j].c_str());
				pInfo->m_nAmount	= (int)nChipAmounts[j];
				delete pImage;
				}
			}
		}

	return true;
	}

int
ESChildControlTable::GetChipImagesByAmount(Size szChip, int nAmount, CPtrArray& arrChipImages, Size& szChipImage){
	arrChipImages.empty();
	szChipImage.SetSize(0, 0);
	if( nAmount <= 0 )
		return 0;

	EnumerableObject<ChipsGroupInfo>*	pListGroups = GetChipsGroupInstance();
	ChipsGroupInfo*						pGroup		= NULL;

	int nLoop = 0;
	while(nLoop < pListGroups->GetCount() ){
		pGroup = pListGroups->GetAt(nLoop);
		if( (pGroup->m_szChips.cx >= szChip.cx || pGroup->m_szChips.cy >= szChip.cy) )
			break;
		nLoop ++;
		}
	if( !pGroup ) return 0;

	nLoop	= (pGroup->m_listChips.GetCount() - 1);
	while( nLoop >= 0 ){
		ChipInfo*	pChipInfo	= pGroup->m_listChips.GetAt(nLoop);
		int			nDiv		= (nAmount / pChipInfo->m_nAmount);
		if( nDiv > 0 ){
			nAmount = nAmount % pChipInfo->m_nAmount;
			// Add chip image. {{
			Image* pChipImage = &pChipInfo->m_chipImage;
			int i = 0;
			while( i < nDiv){
				arrChipImages.push_back(pChipImage);
				i ++;
				}
			// }}
			}
		nLoop --;
		}

	if( arrChipImages.size() > 0 )
		szChipImage = pGroup->m_listChips[0]->m_szImage;
	return (int)arrChipImages.size();
	}

bool
ESChildControlTable::DrawCardByIndex(MercuryBaseView* pBaseView, _DC* pDC, Rect* pRectDC, Rect* pCardDC, Size szCard, int nCard, bool bFourColored /*= true*/, int nConstantAlpha /*= 255*/){
	if( nCard < 0 || nCard >= 52 )
		return false;

	Rect rcInvalid	= *pCardDC & *pRectDC;
	if( rcInvalid.IsRectEmpty() )
		return false;

	EnumerableObject<CardsInfo>* pListCardInfos = GetCardsInfoInstance();
	if( pListCardInfos->GetCount() <= 0 ) 
		return false;
	CardsInfo*	pCardsInfo = pListCardInfos->GetAt(0);
	if( !pCardsInfo || pCardsInfo->m_listCards.GetCount() <= 0) 
		return false;
	CardsGroup* pGroup = NULL;
	for(int i=pCardsInfo->m_listCards.GetCount() - 1; i>=0; i--){
		pGroup = pCardsInfo->m_listCards.GetAt(i);
		if( szCard.cx >= pGroup->m_szCard.cx || 
			szCard.cy >= pGroup->m_szCard.cy )
			break;
		}
	if( !pGroup ) return false;

	Image		bmMemCard;
	_DC			memDC;
	_DC*		pDrawDC		= pDC;
	Rect		rcCardDC	= *pCardDC, r_DC = *pRectDC;
	bool		bMemDC		= (nConstantAlpha < 255);

	if( bMemDC ){
		bmMemCard.CreateDIBBitmap(32, (COLORREF)0, rcInvalid.Width(), rcInvalid.Height());
		memDC.CreateCompatibleDC(*pDC);
		memDC.SelectObject		(bmMemCard);

		// Translate rects.
		Rect rcCardDCTrans;
		rcCardDCTrans.left		= (rcCardDC.left - rcInvalid.left);
		rcCardDCTrans.top		= (rcCardDC.top - rcInvalid.top);
		rcCardDCTrans.right		= rcCardDCTrans.left + rcCardDC.Width();
		rcCardDCTrans.bottom	= rcCardDCTrans.top + rcCardDC.Height();

		Rect rRectDCTrans;
		rRectDCTrans.left		= 0;
		rRectDCTrans.top		= 0;
		rRectDCTrans.right		= rcInvalid.Width();
		rRectDCTrans.bottom	= rcInvalid.Height();

		r_DC					= rRectDCTrans;
		rcCardDC				= rcCardDCTrans;
		pDrawDC					= &memDC;
		}

	float		fZoomCX			= ((float)szCard.cx) / pGroup->m_szCard.cx;
	float		fZoomCY			= ((float)szCard.cy) / pGroup->m_szCard.cy;
	Point		ptCardRank(2*fZoomCX, 4*fZoomCY);
	Point		ptCardSuit(2*fZoomCX, 4*fZoomCY);

	int			nSuitIndex		= nCard/13;
	int			nRankIndex		= nCard%13;
	COLORREF	crRankBySuit[]	= {RGB(0, 0, 0), RGB(0, 0, 0), RGB(200, 8, 8), RGB(200, 8, 8)};
	Image*		ppSuitImages[]	= {&pGroup->m_imgCardSuit1, &pGroup->m_imgCardSuit2, &pGroup->m_imgCardSuit3, &pGroup->m_imgCardSuit4};
	Image*		ppSuitBgImages[]= {&pGroup->m_imgCardBg1, &pGroup->m_imgCardBg2, &pGroup->m_imgCardBg3, &pGroup->m_imgCardBg4};

	if( bFourColored ){
		crRankBySuit[0]		= RGB(0, 0, 0);
		crRankBySuit[1]		= RGB(32, 128, 0);
		crRankBySuit[2]		= RGB(0, 0, 237);
		crRankBySuit[3]		= RGB(200, 8, 8);

		ppSuitImages[0]		= &pGroup->m_imgCardSuit1;
		ppSuitImages[1]		= &pGroup->m_imgCardSuit5;
		ppSuitImages[2]		= &pGroup->m_imgCardSuit6;
		ppSuitImages[3]		= &pGroup->m_imgCardSuit4;

		ppSuitBgImages[0]	= &pGroup->m_imgCardBg1;
		ppSuitBgImages[1]	= &pGroup->m_imgCardBg5;
		ppSuitBgImages[2]	= &pGroup->m_imgCardBg6;
		ppSuitBgImages[3]	= &pGroup->m_imgCardBg4;
		}

	COLORREF	crRank			= crRankBySuit[nSuitIndex];
	HBITMAP		hCardImage		= NULL;
	// Render card bg image. {{
	rcInvalid		= rcCardDC & r_DC;
	if( !pGroup->m_imgCardBg.IsNull() ){
		if( pGroup->m_bMultiSuitBg )
			ppSuitBgImages[nSuitIndex]->RenderImage(pDrawDC, rcInvalid, rcCardDC, true, true, ScaleMode::ScaleMode_Bilinier);
		else
			pGroup->m_imgCardBg.RenderImage(pDrawDC, rcInvalid, rcCardDC, true, true, ScaleMode::ScaleMode_Bilinier);
		}
	else
		return false;
	// }}
	
	// Render card deck. {{
	if( !pCardsInfo->m_imgDeck.IsNull() ){
		Size szCardDeck(pCardsInfo->m_imgDeck.GetWidth() / 78, pCardsInfo->m_imgDeck.GetHeight());
		Rect rcDeck;
		rcDeck.left				= pGroup->m_rcDeck.left*fZoomCX;
		rcDeck.top				= pGroup->m_rcDeck.top*fZoomCY;
		rcDeck.right			= rcDeck.left + pGroup->m_rcDeck.Width()*fZoomCX;
		rcDeck.bottom			= rcDeck.top + pGroup->m_rcDeck.Height()*fZoomCY;
		
		// Restore saved deck rect value. {{
		static AutoSortedArrayInt64 arrDeckRectByCardSize;
		int nCardSizeAsInt	= (((int)szCard.cx) << 16) | (((int)szCard.cy)&0xFFFF);
		int nIndex			= arrDeckRectByCardSize.IndexOf((__int64)nCardSizeAsInt);
		if( nIndex != -1 ){
			__int64 nData	= arrDeckRectByCardSize.GetData(nIndex);
			rcDeck.bottom = ((int)nData&0xFFFF);
			nData >>= 16;
			rcDeck.right = ((int)nData&0xFFFF);
			nData >>= 16;
			rcDeck.top	= ((int)nData&0xFFFF);
			nData >>= 16;
			rcDeck.left	= ((int)nData&0xFFFF);
			}
		else{
			__int64 nData	= rcDeck.left;
			nData <<= 16;
			nData |= (0xFFFF&rcDeck.top);
			nData <<= 16;
			nData |= (0xFFFF&rcDeck.right);
			nData <<= 16;
			nData |= (0xFFFF&rcDeck.bottom);
			arrDeckRectByCardSize.Add(nCardSizeAsInt, nData);
			}
			
		rcDeck.left				+= rcCardDC.left;
		rcDeck.top				+= rcCardDC.top;
		rcDeck.right			+= rcCardDC.left;
		rcDeck.bottom			+= rcCardDC.top;
		// }}

		int nCardDeckIndex		= nCard;
		if( bFourColored ){
			// Spade
			if( nSuitIndex == 1 )
				nCardDeckIndex = 52 + nCardDeckIndex % 13;
			// Diamond
			if( nSuitIndex == 2 )
				nCardDeckIndex = 65 + nCardDeckIndex % 13;
			}

		Rect rcDeckImg;
		rcDeckImg.left			= szCardDeck.cx*nCardDeckIndex;
		rcDeckImg.top			= 0;
		rcDeckImg.right			= rcDeckImg.left + szCardDeck.cx;
		rcDeckImg.bottom		= rcDeckImg.top + szCardDeck.cy;

		rcInvalid = rcDeck & r_DC;
		if( !rcInvalid.IsRectEmpty() ){
			Image imgDeck;
			if( pCardsInfo->m_imgDeck.GetImageFromImage(rcDeckImg, imgDeck) )
				imgDeck.RenderImage(pDrawDC, rcInvalid, rcDeck, true, false, ScaleMode::ScaleMode_Bilinier, 0, 255, true);
			}
		}
	else
		if( !pGroup->m_imgCardDeck.IsNull() ){
			Size szCardDeck(pGroup->m_imgCardDeck.GetWidth() / 13, pGroup->m_imgCardDeck.GetHeight());
			Rect rcDeckImg;
			rcDeckImg.left			= szCardDeck.cx*nRankIndex;
			rcDeckImg.top			= 0;
			rcDeckImg.right			= rcDeckImg.left + szCardDeck.cx;
			rcDeckImg.bottom		= rcDeckImg.top + szCardDeck.cy;

			szCardDeck.cx			*= fZoomCX;
			szCardDeck.cy			*= fZoomCY;

			Rect rcDeck;
			rcDeck.left				= rcCardDC.left + (rcCardDC.Width() - szCardDeck.cx)/2;
			rcDeck.right			= rcDeck.left + szCardDeck.cx;
			rcDeck.bottom			= rcCardDC.bottom;
			rcDeck.top				= rcDeck.bottom - szCardDeck.cy;

			rcInvalid				= rcDeck & r_DC;
			if( !rcInvalid.IsRectEmpty() ){
				Image imgDeck;
				if( pGroup->m_imgCardDeck.GetImageFromImage(rcDeckImg, imgDeck) )
					imgDeck.RenderImage(pDrawDC, rcInvalid, rcDeck, true, true, ScaleMode::ScaleMode_Bilinier, 0, 255, true);
				}
			}
	// }}

	// Render rank image. {{
	if( !pGroup->m_imgCardRank.IsNull() ){
		Size szRank(pGroup->m_imgCardRank.GetWidth() / 13, pGroup->m_imgCardRank.GetHeight());
		Rect rcRank;
		rcRank.left			= rcCardDC.left + ptCardRank.x;
		rcRank.top			= rcCardDC.top + ptCardRank.y;
		rcRank.right		= rcRank.left + szRank.cx;
		rcRank.bottom		= rcRank.top + szRank.cy;

		ptCardSuit.y		+= szRank.cy;

		Rect rcRankImg;	
		rcRankImg.left		= szRank.cx*nRankIndex;
		rcRankImg.top		= 0;
		rcRankImg.right		= rcRankImg.left + szRank.cx;
		rcRankImg.bottom	= rcRankImg.top + szRank.cy;

		Image bmRank;
		// Use card rank image as alpha channel.
		if( pGroup->m_imgCardRank.GetBPP() == 8 ){
			if( bmRank.CreateDIBBitmap(32, crRank, szRank.cx, szRank.cy, NULL) ){
				Image imgRank;
				imgRank.Attach((HBITMAP)bmRank.Detach());
				Image* pImgRank = Image::CombineImages(&imgRank, &pGroup->m_imgCardRank, true, 24, rcRankImg);
				if( pImgRank ){
					rcInvalid = rcRank & r_DC;
					if( !rcInvalid.IsRectEmpty() )
						pImgRank->RenderImage(pDrawDC, rcInvalid, rcRank, true, true, ScaleMode::ScaleMode_Bilinier, 0, 255, true);
					delete pImgRank;
					}
				imgRank.Destroy();
				}
			}
		else{
			rcInvalid = rcRank & r_DC;
			if( !rcInvalid.IsRectEmpty() ){
				Image imgRank;
				if( pGroup->m_imgCardRank.GetImageFromImage(rcRankImg, imgRank) )
					imgRank.RenderImage(pDrawDC, rcInvalid, rcRank, true, true, ScaleMode::ScaleMode_Bilinier, 0, 255, true);
				}
			}
		}
	// }}

	// Render card suit image. {{
	if( !pGroup->m_imgCardSuit.IsNull() ){
		Size		szSuit(pGroup->m_imgCardSuit.GetWidth() / 6, pGroup->m_imgCardSuit.GetHeight());
		Rect rcSuit;
		rcSuit.left				= rcCardDC.left + ptCardSuit.x;
		rcSuit.top				= rcCardDC.top + ptCardSuit.y;
		rcSuit.right			= rcSuit.left + szSuit.cx;
		rcSuit.bottom			= rcSuit.top + szSuit.cy;

		rcInvalid = rcSuit & r_DC;
		if( !rcInvalid.IsRectEmpty() && !ppSuitImages[nSuitIndex]->IsNull() )
			ppSuitImages[nSuitIndex]->RenderImage(pDrawDC, rcInvalid, rcSuit, true, true, ScaleMode::ScaleMode_Bilinier, 0, 255, true);
		}
	// }}

	if( bMemDC ){
		rcInvalid = *pRectDC&*pCardDC;
		if( !rcInvalid.IsRectEmpty() ){
			Image imgCard;
			imgCard.Attach(bmMemCard.Detach());
			// In this case clip destination rect and destination rect are the same.
			imgCard.RenderImage(pDC, rcInvalid, rcInvalid, true, (imgCard.GetBPP() == 32), ScaleMode_Bilinier, 0, nConstantAlpha);
			imgCard.Destroy();
			}
		memDC		.DeleteDC	();
		bmMemCard	.Destroy	();
		}
	return true;
	}

bool
ESChildControlTable::DrawCardBackByIndex(MercuryBaseView* pBaseView, _DC* pDC, Rect* pRectDC, Rect* pCardDC, Size szCard, int nIndex, int nAlphaConstant, bool bSmallCards){
	Image*	pBackImage = GetNearestCardBackImage(szCard, nIndex);
	if( !pBackImage ) return false;

	Rect	rcInvalid;
	float	fZoom		= ((float)szCard.cx) / pBackImage->GetWidth();
	bool bAllowChaching = pBaseView->IsDesignerMode() ? false : true;
	if( bAllowChaching ){
		ESChildControlTable* pTable = (ESChildControlTable*)pBaseView->GetChildControlByName(_T("_pokerTable1"));
		ASSERT(pTable);

		ImageDef	hImageCached = pTable->GetCachedCardBackImage(bSmallCards);
		Image		imageCached;
		imageCached.Attach(hImageCached);

		Size szDestCard(pCardDC->Width(), pCardDC->Height());
		if( imageCached.GetWidth() != szDestCard.cx || imageCached.GetHeight() != szDestCard.cy ){
			// Update cached image.
			pTable->FreeCachedCardBackImage(bSmallCards);
			hImageCached = NULL;
			if( pBackImage->ScaleBitmap(ScaleMode::ScaleMode_Bilinier, szDestCard.cx, szDestCard.cy, Rect(0, 0, szDestCard.cx, szDestCard.cy), hImageCached, (pBackImage->GetBPP() == 32)) )
				pTable->UpdateCachedCardBackImage(bSmallCards, hImageCached);
			}
		imageCached.Detach();

		if( hImageCached ){
			// Use cached image.
			rcInvalid = *pCardDC & *pRectDC;
			if( !rcInvalid.IsRectEmpty() ){
				Image imgTemp;
				imgTemp.Attach		(hImageCached);
				imgTemp.RenderImage	(pDC, rcInvalid, pCardDC, true, true, ScaleMode::ScaleMode_Bilinier, NULL, nAlphaConstant);
				imgTemp.Detach		();
				}
			return true;
			}
		}
	else{
		rcInvalid = *pCardDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() ){
			if( !rcInvalid.IsRectEmpty() )
				pBackImage->RenderImage(pDC, rcInvalid, pCardDC, true, true, ScaleMode::ScaleMode_Bilinier, NULL, nAlphaConstant);
			}
		}
	return true;
	}

Size
ESChildControlTable::GetCardDimensionMin(){
	EnumerableObject<CardsInfo>* pListCardInfos = GetCardsInfoInstance();
	if( pListCardInfos->GetCount() <= 0 ) 
		return Size(0, 0);

	CardsInfo*	pCardsInfo = pListCardInfos->GetAt(0);
	if( !pCardsInfo || pCardsInfo->m_listCards.GetCount() <= 0) 
		return Size(0, 0);

	CardsGroup* pGroup = pCardsInfo->m_listCards.GetAt(0);
	if( pGroup )
		return pGroup->m_szCard;
	return Size(0, 0);
	}

Size
ESChildControlTable::GetCardDimensionMax(){
	EnumerableObject<CardsInfo>* pListCardInfos = GetCardsInfoInstance();
	if( pListCardInfos->GetCount() <= 0 ) 
		return Size(0, 0);

	CardsInfo*	pCardsInfo = pListCardInfos->GetAt(0);
	if( !pCardsInfo || pCardsInfo->m_listCards.GetCount() <= 0) 
		return Size(0, 0);

	CardsGroup* pGroup = pCardsInfo->m_listCards.GetAt(pCardsInfo->m_listCards.GetCount() - 1);
	if( pGroup )
		return pGroup->m_szCard;
	return Size(0, 0);
	}

BOOL
ESChildControlTable::GetNearestCardDimension(Size& szCard, CardsGroup** ppGroup /*= NULL*/, int* pCardSuitWidth /*= NULL*/){
	EnumerableObject<CardsInfo>* pListCardInfos = GetCardsInfoInstance();
	if( pListCardInfos->GetCount() <= 0 ) 
		return FALSE;

	CardsInfo*	pCardsInfo = pListCardInfos->GetAt(0);
	if( !pCardsInfo || pCardsInfo->m_listCards.GetCount() <= 0) 
		return FALSE;

	CardsGroup* pGroupPrev = NULL;
	for(int i=pCardsInfo->m_listCards.GetCount()-1; i>=0; i--){
		CardsGroup* pGroup = pCardsInfo->m_listCards.GetAt(i);
		if( pGroup->m_szCard.cx <= szCard.cx && pGroup->m_szCard.cx <= szCard.cy ){
			if( pGroupPrev && ((pGroupPrev->m_szCard.cx - szCard.cx) < (szCard.cx - pGroup->m_szCard.cx) || (pGroupPrev->m_szCard.cy - szCard.cy) < (szCard.cy - pGroup->m_szCard.cy)) ){
				szCard = pGroupPrev->m_szCard;
				if( ppGroup )
					*ppGroup = pGroupPrev; 
				if( pCardSuitWidth && !pGroupPrev->m_imgCardSuit.IsNull() )
					*pCardSuitWidth = (pGroupPrev->m_imgCardSuit.GetWidth() / 6);
				}
			else{
				szCard = pGroup->m_szCard;
				if( ppGroup )
					*ppGroup = pGroup; 
				if( pCardSuitWidth && !pGroup->m_imgCardSuit.IsNull() )
					*pCardSuitWidth = (pGroup->m_imgCardSuit.GetWidth() / 6);
				}
			return TRUE;
			}
		pGroupPrev = pGroup;
		}

	// Min card dimensions.
	szCard = pCardsInfo->m_listCards.GetAt(0)->m_szCard;
	if( ppGroup )
		*ppGroup = pCardsInfo->m_listCards.GetAt(0);

	if( pCardSuitWidth ){
		Image* pImageCardSuit = &pCardsInfo->m_listCards.GetAt(0)->m_imgCardSuit;
		if( (pImageCardSuit && !pImageCardSuit->IsNull()) )
			*pCardSuitWidth = (pImageCardSuit->GetWidth() / 6);
		}
	return TRUE;
	}

Image*
ESChildControlTable::GetNearestCardBackImage(Size& szCard, int nCardBackIndex){
	EnumerableObject<CardsInfo>* pListCardInfos = GetCardsInfoInstance();
	if( pListCardInfos->GetCount() <= 0 )  return NULL;
	CardsInfo* pCardsInfo = pListCardInfos->GetAt(0);
	if( !pCardsInfo ) return NULL;
	AutoSortedArray* pArr	= &pCardsInfo->m_arrBackImagesOrderedByDimensions;
	if( !pArr->GetCount() ) return NULL;

	int nFindValue			=	szCard.cx;
	nFindValue				<<= 16;
	nFindValue				|=	szCard.cy;

	List<Image>* pImages	= NULL;
	bool bExists			= false;
	int nIndex				= pArr->FindValuePlace((void*)nFindValue, bExists);
	
	if( bExists ){
		pImages = (List<Image>*)pArr->GetData(nIndex);
		}
	else{
		if( nIndex < pArr->GetCount() ){
			if( !nIndex )
				pImages = (List<Image>*)pArr->GetData(0);
			else{
				List<Image>*	pImagesNext			= (List<Image>*)pArr->GetData(nIndex);
				List<Image>*	pImagesPrev			= (List<Image>*)pArr->GetData(nIndex - 1);
				Size			szImagePrev(pImagesPrev->GetAt(0)->GetWidth(), pImagesPrev->GetAt(0)->GetHeight());
				Size			szImageNext(pImagesNext->GetAt(0)->GetWidth(), pImagesNext->GetAt(0)->GetHeight()); 

				/*
				if( (szCard.cx*szCard.cy) < (szImageNext.cx*szImageNext.cy) )
					pImages = pImagesPrev;c
				else
					pImages = pImagesNext;*/

				float			fZoomTillNext		= ((float)szImageNext.cx) / szCard.cx;
				float			fReduceTillPrev		= ((float)szCard.cx) / szImagePrev.cx;
				if( fZoomTillNext <= fReduceTillPrev )
					pImages = pImagesNext;
				else
					pImages = pImagesPrev;
				}
			}
		else
			pImages = (List<Image>*)pArr->GetData(pArr->GetCount() - 1);
		}

	return (pImages ?pImages->GetAt(nCardBackIndex) : NULL);
	}

bool
ESChildControlTable::FreeGlobalResources(){
	// Free card resources.
	EnumerableObject<CardsInfo>* pListCardInfos = GetCardsInfoInstance();
	pListCardInfos->RemoveAll(true);

	// Free chip resources.
	EnumerableObject<ChipsGroupInfo>* pChips = GetChipsGroupInstance();
	pChips->RemoveAll(true);

	Image* pDefTable = GetTableImageDefault();
	pDefTable->Destroy();

	// Free regions. {{
	Rgn* pRgn = GetTableImageRgn();
	if( pRgn )
		pRgn->DeleteObject();

	pRgn = GetEmptySeatImageRgn();
	if( pRgn )
		pRgn->DeleteObject();

	pRgn = GetSeatImageRgn();
	if( pRgn )
		pRgn->DeleteObject();

	pRgn = GetSeatStatusImageRgn();
	if( pRgn )
		pRgn->DeleteObject();
	// }}
	return true;
	}

void
ESChildControlTable::OnDeserializationCompleted(){
	std::string		sBgImageFileApplied = *GetTableBgImageFileApplied();
	COLORREF	crTableFgApplied	= *GetTableColorApplied();
	if( sBgImageFileApplied.length() )
		m_sBgImageFile = sBgImageFileApplied;
	if( crTableFgApplied )
		m_crFgColor = crTableFgApplied;
	
	UpdateTableImage(false);
	LoadChipImages	(m_sChipImagesFolder, false);
	LoadCardImages	(m_sCardImagesFolder, m_sCardName, false);
	}

bool
ESChildControlTable::Create(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	if( ESChildControl::Create(nId, rRect, pOwner, pParent) ){
		// Register variable modify callback methods. {{
		if( m_pOwner->IsDesignerMode() ){
			AddVariableModifyCallBack(&m_sBgImageFile,				(VariableValueModifyCallback)&ESChildControlTable::AttributeModify);
			AddVariableModifyCallBack(&m_sBorderImageFile,			(VariableValueModifyCallback)&ESChildControlTable::AttributeModify);
			AddVariableModifyCallBack(&m_sFgImageFile,				(VariableValueModifyCallback)&ESChildControlTable::AttributeModify);
			AddVariableModifyCallBack(&m_crFgColor,					(VariableValueModifyCallback)&ESChildControlTable::AttributeModify);
			AddVariableModifyCallBack(&m_sLayer1ImageFile,			(VariableValueModifyCallback)&ESChildControlTable::AttributeModify);
			AddVariableModifyCallBack(&m_sLayer2ImageFile,			(VariableValueModifyCallback)&ESChildControlTable::AttributeModify);
			AddVariableModifyCallBack(&m_sLayer3ImageFile,			(VariableValueModifyCallback)&ESChildControlTable::AttributeModify);
			}
		// }}

		/*
		MercuryGUI* pGUI	= MercuryGUI::GetInstance();
		std::string		sImage	= _T("design\\black\\label\\status.png");
		Path::BuildFullPath(sImage);
		Image* pBgImage			= pGUI->LoadImage(sImage);
		m_hSeatRgn = ImageHelper::GetBitmap32bppRgnByAlphaChannel(*pBgImage);

		*/
		}
	return true;
	}

bool
ESChildControlTable::Destroy(){
	ESChildControl::Destroy();

	// Free cached images.
	FreeCachedTableImage		();
	FreeCachedEmptySeatImage	();
	FreeCachedSeatStatusImage	(true);
	FreeCachedSeatStatusImage	(false);
	FreeCachedCardBackImage		(true);
	FreeCachedCardBackImage		(false);

	for(int i=0; i<10; i++)
		FreeCachedSeatSpeImages(i);

	// Avtive border no avatar.
	FreeCachedSeatImage(-1, true, false, false);
	// No active border no avatar.
	FreeCachedSeatImage(-1, false, false, false);

	return true;
	}

void
ESChildControlTable::OnPaintClient(_DC* pDC, Rect* pRectDC, Rect* pRectClient){
	MercuryGUI* pGUI			= MercuryGUI::GetInstance();

	Rect rcTableDC;
	rcTableDC.left				= (m_rRect.left - pRectClient->left) + pRectDC->left;
	rcTableDC.top				= (m_rRect.top - pRectClient->top) + pRectDC->top;
	rcTableDC.right				= rcTableDC.left + m_rRect.Width();
	rcTableDC.bottom			= rcTableDC.top + m_rRect.Height();

	Rect rcClient				= GetClientRect();
	bool bAllowCaching			= m_pOwner->IsDesignerMode() ? false : true;

	// Table image.
	Image*		pTableImageDef	= GetTableImageDefault();
	if( pTableImageDef && !pTableImageDef->IsNull() ){
		if( bAllowCaching ){
			ImageDef	hImageCached = GetCachedTableImage();
			Image		imgCached(hImageCached);

			if( imgCached.GetWidth() != rcClient.Width() || imgCached.GetHeight() != rcClient.Height() ){
				// Update cached image.
				if( pRectDC->Width() >= rcClient.Width() && pRectDC->Height() >= rcClient.Height() ){
					Image imgMemory;
					if( pDC->GetSelectedDibImageFromDC(imgMemory) ){
						m_szDefTableImage.SetSize(pTableImageDef->GetWidth(), pTableImageDef->GetHeight());
						m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pTableImageDef, Rect(0, 0, 0, 0), false, (int)ScaleMode::ScaleMode_Bilinier);

						FreeCachedTableImage();
						imgCached.Detach	();

						if( imgCached.CreateDIBBitmap(24, RGB(0, 0, 0), rcClient.Width(), rcClient.Height(), NULL) ){
							_DC memDC;
							memDC.CreateCompatibleDC(*pDC);
							memDC.SelectObject(imgCached);
							memDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), *pDC, rcTableDC.left, rcTableDC.top, SRCCOPY);
							memDC.DeleteDC();

							hImageCached = imgCached.Detach();
							UpdateCachedTableImage(hImageCached);
							}
						}
					imgMemory.Detach();
					}
				}
			else{// Use cached image.
				Image imgTemp;
				imgTemp.Attach(hImageCached);
				m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_CentredImage, RGB(0, 0, 0), &imgTemp, Rect(0, 0, 0, 0), false, (int)ScaleMode::ScaleMode_Bilinier);
				imgTemp.Detach();
				}

			imgCached.Detach();
			}
		else{
			m_szDefTableImage.SetSize(pTableImageDef->GetWidth(), pTableImageDef->GetHeight());
			m_pOwner->DrawBackground(pDC, pRectDC, pRectClient, rcClient, FillMode::FillMode_StretchImage, RGB(0, 0, 0), pTableImageDef, Rect(0, 0, 0, 0), false, (int)ScaleMode::ScaleMode_Bilinier);
			}
		}
	else
		m_szDefTableImage.SetSize(0, 0);
	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void	
ESChildControlTable::OnRButtonUp(Point pt, UINT nFlags){
	Rgn* pRgn = ESChildControlTable::GetTableImageRgn();	
	if( MercuryBaseView::PtInImageRegion(pt, this, *pRgn, m_szDefTableImage, true) )
		SendEvent(this, ChildControlEventCode::EventCode_OnContextMenu, 1/*Table foreground options menu*/);
	else
		SendEvent(this, ChildControlEventCode::EventCode_OnContextMenu, 0/*Table background options menu*/);
	}

bool
ESChildControlTable::AttributeModify(ESChildControlTable* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->UpdateTableImage	(true);
	pThis->Redraw			();
	return true;
	}


/////////////////////////////////////////////////////////////////////////////
//
// Caching images
//

HBITMAP
ESChildControlTable::GetCachedTableImage(){
	return GetCachedImage(_T("__cache_TableImage"));
	}

bool
ESChildControlTable::FreeCachedTableImage(){
	return FreeCachedImage(_T("__cache_TableImage"));
	}

bool
ESChildControlTable::UpdateCachedTableImage(HBITMAP hImage){
	return UpdateCachedImage(_T("__cache_TableImage"), hImage);
	}

HBITMAP
ESChildControlTable::GetCachedEmptySeatImage(){
	return GetCachedImage(_T("__cache_EmptySeatImage"));
	}

bool
ESChildControlTable::FreeCachedEmptySeatImage(){
	return FreeCachedImage(_T("__cache_EmptySeatImage"));
	}

bool
ESChildControlTable::UpdateCachedEmptySeatImage(HBITMAP hImage){
	return UpdateCachedImage(_T("__cache_EmptySeatImage"), hImage);
	}

HBITMAP	
ESChildControlTable::GetCachedSeatImage(int nSeat, bool bActive, bool bColorBorder, bool bAvatar){
	std::string sName;
	if( !bColorBorder && !bAvatar )
		sName = bActive ? _T("__cache_SeatImageActive") : _T("__cache_SeatImage");
	else{
		if( bColorBorder )
			stringEx::Format(sName, bAvatar ? _T("__cache_SeatImageClrBorderAvatar%d") :  _T("__cache_SeatImageClrBorder%d"), nSeat + 1);
		else
			stringEx::Format(sName, bActive ? _T("__cache_SeatImageAvatarActive%d") : _T("__cache_SeatImageAvatar%d"), nSeat + 1);
		}
	return GetCachedImage(sName.c_str());
	}

bool
ESChildControlTable::FreeCachedSeatImage(int nSeat, bool bActive, bool bColorBorder, bool bAvatar){
	std::string sName;
	if( !bColorBorder && !bAvatar )
		sName = bActive ? _T("__cache_SeatImageActive") : _T("__cache_SeatImage");
	else{
		if( bColorBorder )
			stringEx::Format(sName, bAvatar ? _T("__cache_SeatImageClrBorderAvatar%d") :  _T("__cache_SeatImageClrBorder%d"), nSeat + 1);
		else
			stringEx::Format(sName, bActive ? _T("__cache_SeatImageAvatarActive%d") : _T("__cache_SeatImageAvatar%d"), nSeat + 1);
		}
	return FreeCachedImage(sName.c_str());
	}

bool
ESChildControlTable::FreeCachedSeatSpeImages(int nSeat){
	std::string sName;
	// Avatar + Color border
	stringEx::Format(sName, _T("__cache_SeatImageClrBorderAvatar%d"), nSeat + 1);
	FreeCachedImage(sName.c_str());

	// No avatar + Color border
	stringEx::Format(sName, _T("__cache_SeatImageClrBorder%d"), nSeat + 1);
	FreeCachedImage(sName.c_str());

	// Avatar + Active border
	stringEx::Format(sName, _T("__cache_SeatImageAvatarActive%d"), nSeat + 1);
	FreeCachedImage(sName.c_str());

	// Avatar + Normal border
	stringEx::Format(sName, _T("__cache_SeatImageAvatar%d"), nSeat + 1);
	FreeCachedImage(sName.c_str());
	return true;
	}

bool
ESChildControlTable::UpdateCachedSeatImage(int nSeat, HBITMAP hImage, bool bActive, bool bColorBorder, bool bAvatar){
	std::string sName;
	if( !bColorBorder && !bAvatar )
		sName = bActive ? _T("__cache_SeatImageActive") : _T("__cache_SeatImage");
	else{
		if( bColorBorder )
			stringEx::Format(sName, bAvatar ? _T("__cache_SeatImageClrBorderAvatar%d") :  _T("__cache_SeatImageClrBorder%d"), nSeat + 1);
		else
			stringEx::Format(sName, bActive ? _T("__cache_SeatImageAvatarActive%d") : _T("__cache_SeatImageAvatar%d"), nSeat + 1);
		}
	return UpdateCachedImage(sName.c_str(), hImage);
	}

HBITMAP
ESChildControlTable::GetCachedSeatStatusImage(bool bActive){
	return GetCachedImage(!bActive ? _T("__cache_SeatStatusImage") : _T("__cache_SeatStatusActiveImage"));
	}

bool
ESChildControlTable::FreeCachedSeatStatusImage(bool bActive){
	return FreeCachedImage(!bActive ? _T("__cache_SeatStatusImage") : _T("__cache_SeatStatusActiveImage"));
	}

bool
ESChildControlTable::UpdateCachedSeatStatusImage(bool bActive, HBITMAP hImage){
	return UpdateCachedImage(!bActive ? _T("__cache_SeatStatusImage") : _T("__cache_SeatStatusActiveImage"), hImage);
	}

HBITMAP
ESChildControlTable::GetCachedCardBackImage(bool bSmall){
	return GetCachedImage(!bSmall ? _T("__cache_CardBackImage") : _T("__cache_CardBackImageSm"));
	}

bool
ESChildControlTable::FreeCachedCardBackImage(bool bSmall){
	return FreeCachedImage(!bSmall ? _T("__cache_CardBackImage") : _T("__cache_CardBackImageSm"));
	}

bool
ESChildControlTable::UpdateCachedCardBackImage(bool bSmall, HBITMAP hImage){
	return UpdateCachedImage(!bSmall ? _T("__cache_CardBackImage") : _T("__cache_CardBackImageSm"), hImage);
	}

HBITMAP
ESChildControlTable::GetCachedImage(LPCTSTR lpszName){
	if( !m_pOwner ) return NULL;
	void*		lpParam			= 0L;
	ImageDef	hImageCached	= NULL;
	if( !m_pOwner->GetNamedParameter(lpszName, lpParam) || !lpParam )
		return NULL;
	hImageCached = (ImageDef)lpParam;
	return  hImageCached;
	}

bool
ESChildControlTable::FreeCachedImage(LPCTSTR lpszName){
	if( !m_pOwner ) return false;
	void*		lpParam			= 0L;
	ImageDef	hImageCached	= NULL;
	if( !m_pOwner->RemoveNamedParameter(lpszName, lpParam) || !lpParam )
		return false;
	hImageCached = (ImageDef)lpParam;
	::DeleteObject(hImageCached);
	return  true;
	}

bool
ESChildControlTable::UpdateCachedImage(LPCTSTR lpszName, HBITMAP hImage){
	if( !m_pOwner ) return false;
	void*		lpParam			= 0L;
	ImageDef	hImageCached	= NULL;
	if( !m_pOwner->GetNamedParameter(lpszName, lpParam) ){
		if( hImage ){
			lpParam = (void*)hImage;
			return m_pOwner->AddNamedParameter(lpszName, lpParam, true);
			}
		return false;
		}

	hImageCached = (ImageDef)lpParam;
	if( hImageCached )
		::DeleteObject(hImageCached);
	lpParam = (void*)hImage;
	return m_pOwner->AddNamedParameter(lpszName, lpParam, true);
	}

/////////////////////////////////////////////////////////////////////////////
// ChipInfo

ChipInfo::ChipInfo(){
	}

BEGIN_DECLARE_VAR(ChipInfo, Serializable)
	DECLARE_VAR(_T("ChipInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Image File"),	_T(""), VariableType::VT_String, offsetof(ChipInfo,m_sImageFile), false, false, false)
	DECLARE_VAR(_T("Amount"),		_T(""), VariableType::VT_Int32, offsetof(ChipInfo,m_nAmount), false, false, false)
END_DECLARE_VAR()

ChipInfo::~ChipInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// ChipsGroupInfo

ChipsGroupInfo::ChipsGroupInfo(){
	m_listChips.CreateVariableIndex(_T("Amount"), Serializable::Compare_Int32);
	}

BEGIN_DECLARE_VAR(ChipsGroupInfo, Serializable)
	DECLARE_VAR(_T("ChipsGroupInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Chips dimension"),		_T(""), VariableType::VT_Size, offsetof(ChipsGroupInfo,m_szChips), false, false, false)
	DECLARE_VAR(_T("Chips"),				_T(""), VariableType::VT_None, offsetof(ChipsGroupInfo,m_listChips), false, true, true)
END_DECLARE_VAR()

ChipsGroupInfo::~ChipsGroupInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// CardsGroup

CardsGroup::CardsGroup(){
	m_bMultiSuitBg = false;
	}

BEGIN_DECLARE_VAR(CardsGroup, Serializable)
	DECLARE_VAR(_T("CardsGroup"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Card bg"),			_T(""), VariableType::VT_Image, offsetof(CardsGroup,m_imgCardBg), false, false, false)
	DECLARE_VAR(_T("Card rank"),		_T(""), VariableType::VT_Image, offsetof(CardsGroup,m_imgCardRank), false, false, false)
	DECLARE_VAR(_T("Card suit"),		_T(""), VariableType::VT_Image, offsetof(CardsGroup,m_imgCardSuit), false, false, false)
END_DECLARE_VAR()

CardsGroup::~CardsGroup(){
	}

/////////////////////////////////////////////////////////////////////////////
// CardsInfo

CardsInfo::CardsInfo(){
	}

BEGIN_DECLARE_VAR(CardsInfo, Serializable)
	DECLARE_VAR(_T("CardsInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Back small"),		_T(""), VariableType::VT_Size, offsetof(CardsInfo,m_szBackSm), false, false, false)
	DECLARE_VAR(_T("Back"),				_T(""), VariableType::VT_Size, offsetof(CardsInfo,m_szBack), false, false, false)
	DECLARE_VAR(_T("Back medium"),		_T(""), VariableType::VT_Size, offsetof(CardsInfo,m_szBackMed), false, false, false)
	DECLARE_VAR(_T("Back large"),		_T(""), VariableType::VT_Size, offsetof(CardsInfo,m_szBackLg), false, false, false)
	DECLARE_VAR(_T("Back image small"),	_T(""), VariableType::VT_Image, offsetof(CardsInfo,m_imgBackSm), false, false, false)
	DECLARE_VAR(_T("Back image"),		_T(""), VariableType::VT_Image, offsetof(CardsInfo,m_imgBack), false, false, false)
	DECLARE_VAR(_T("Back image medium"),_T(""), VariableType::VT_Image, offsetof(CardsInfo,m_imgBackMed), false, false, false)
	DECLARE_VAR(_T("Back image large"),	_T(""), VariableType::VT_Image, offsetof(CardsInfo,m_imgBackLg), false, false, false)
	DECLARE_VAR(_T("Deck image"),		_T(""), VariableType::VT_Image, offsetof(CardsInfo,m_imgDeck), false, false, false)
	DECLARE_VAR(_T("Cards group"),		_T(""), VariableType::VT_None, offsetof(CardsInfo,m_listCards), false, true, true)
END_DECLARE_VAR()

CardsInfo::~CardsInfo(){
	}