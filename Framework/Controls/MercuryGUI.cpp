// MercuryGUI.cpp : implementation file
//

#include "MercuryGUI.h"
#include "../Path/Path.h"

MercuryGUI::MercuryGUI(){
	m_bInited = FALSE;
	}

MercuryGUI::~MercuryGUI(){
	Destroy();
	}

_Image*		
MercuryGUI::LoadImage(const _string sImageFile, int nWidth /*= -1*/, int nHeight /*= -1*/, bool bCache /*= true*/, bool bConvertTo32bpp /*= true*/){
	if( !sImageFile.length() )
		return NULL;

	// Load cached image. {{
	_Image* pImage = (_Image*)m_arrImagesByName.GetValue(sImageFile);
	if( pImage )
		return pImage;
	// }}

#ifdef LOAD_FILES_FROM_DLL
	_string sTemp = sImageFile;
	sTemp.Replace('\\', '@');
	sTemp.MakeUpper();

	_string sResName;
	sResName.Format(_T("~%s~"), sTemp);
	sResName.Replace('~', '\x22');

	HMODULE hResModule		= GetModuleHandle(RESOURCE_MODULE);
	pImage					= LoadPngFromResource(hResModule, sResName.GetBuffer());
	if( !pImage )
		return NULL;

	if( nWidth > 0 && nHeight > 0 ){
		// Not tested yet
		ASSERT(FALSE);
		HBITMAP hbmDest = 0;
		if( ImageHelper::ScaleBitmap_Bilinier(*pImage, nWidth, nHeight, _Rect(0, 0, nWidth, nHeight), hbmDest, (pImage->IsTransparencySupported() == 1), 0) ){
			}	
		}

	// Convert into 32bpp. ################
	if( pImage->GetBPP() != 32 && bConvertTo32bpp ){
		CBitmap bmTemp;
		bmTemp.Attach(pImage->Detach());
		ImageHelper::ConvertToBitmap32bpp(bmTemp);
		pImage->Attach(bmTemp.Detach());

		// Make image opaque
		BITMAP bmImage;
		::GetObject(*pImage, sizeof(BITMAP), &bmImage);
		RGBQUAD* p = (RGBQUAD*)bmImage.bmBits;

		int nLoop	= 0;
		int nCt		= bmImage.bmWidth*bmImage.bmHeight;

		while( nLoop < nCt ){
			p->rgbReserved = 255;
			p		++;
			nLoop	++;
			}
		}
	// #######################################
#else
	pImage = _Image::LoadImage(sImageFile, nWidth, nHeight, bConvertTo32bpp);
	if( !pImage )
		return FALSE;
/*
	pImage = new Image();
	_string sImageFileFullPath = sImageFile;
	Path::BuildFullPath(sImageFileFullPath);
	if( pImage->Load(sImageFileFullPath.GetBuffer()) != 0 ){
		delete pImage;
		return NULL;
		}*/
#endif
	if( pImage && bCache )
		m_arrImagesByName.Add(sImageFile, (void*)pImage);
	return pImage;
	}

_Image*
MercuryGUI::GetCachedImage(const _string sImageFile){
	_Image* pImage = (_Image*)m_arrImagesByName.GetValue(sImageFile);
	if( pImage )
		return pImage;
	return NULL;
	}

bool
MercuryGUI::DeleteCachedImage(const _string sImageFile){
	_Image* pImage = (_Image*)m_arrImagesByName.GetValue(sImageFile);
	if( !pImage )
		return false;
	m_arrImagesByName.Delete(sImageFile);
	delete pImage;
	return true;
	}

_Image*
MercuryGUI::LoadPngFromResource(HMODULE hResModule, LPCTSTR lpszResName){
	return NULL;
	/*
	_Image* pImage	= new Image;
	HRSRC	hRsrc	= ::FindResource(hResModule, lpszResName, _T("PNG"));
	HGLOBAL hRes	= ::LoadResource(hResModule, hRsrc);
	if( hRes == NULL ){
		delete pImage;
		return NULL;
		}

	int		nResDataSize	= (int)::_SizeofResource(hResModule, hRsrc);
	void*	lpResData		= ::LockResource(hRes);
	HGLOBAL hMem			= GlobalAlloc(GMEM_MOVEABLE, nResDataSize);
	void*	lpGMem			= ::GlobalLock(hMem);
	memcpy(lpGMem, lpResData, nResDataSize);
	::GlobalUnlock		(hMem);
	::UnlockResource	(hRes);
	::FreeResource		(hRes);

	LPSTREAM pStream = NULL;
	if( ::CreateStreamOnHGlobal(hMem, FALSE, &pStream) != S_OK ){
		delete pImage;
		return NULL;
		}

	pImage->Load(pStream);

	pStream->Release();
	::GlobalFree(hMem);
	return pImage;*/
	}

BOOL
MercuryGUI::Initialize(){
	if( m_bInited ) return FALSE;
	// TreeGrid {{
	treeGrid.m_crSelItemTextActive			= RGB(255,255,255);
	treeGrid.m_crReadOnlyItemTextActive		= RGB(98, 112, 128);
	treeGrid.m_crNormalItemTextActive		= RGB(0, 0, 0);

	treeGrid.m_crSelItemTextActShadow		= RGB(34, 86, 140);
	treeGrid.m_crReadOnlyItemTextActShadow	= RGB(246, 248, 250);
	treeGrid.m_crNormalItemTextActShadow	= treeGrid.m_crNormalItemTextActive;

	treeGrid.m_crSelItemTextNonActive		= RGB(255,255,255);
	treeGrid.m_crReadOnlyItemTextNonActive	= RGB(112, 112, 112);
	treeGrid.m_crNormalItemTextNonActive	= RGB(0, 0, 0);

	treeGrid.m_crSelItemTextNonActShadow	= RGB(34, 86, 140);
	treeGrid.m_crReadOnlyItemTextNonActShadow= RGB(246, 248, 250);
	treeGrid.m_crNormalItemTextNonActShadow	= treeGrid.m_crNormalItemTextNonActive;

	treeGrid.m_crBkgndActive				= RGB(217, 222, 231);
	treeGrid.m_crBkgndNonActive				= RGB(236, 236, 236);
	treeGrid.m_nItemHeight					= 24;
	treeGrid.m_ptOffset						.SetPoint(9, 0);
	treeGrid.m_dwDefState					= 0x02000000; // Shadow flag for all items.
	treeGrid.m_crBorder						= RGB(185, 185, 185);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight			= 16;
	lf.lfWeight			= 600;
	lf.lfItalic			= 0;
	lf.lfUnderline		= 0;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
    _tcscpy_(lf.lfFaceName, _T("Tahoma"));
	treeGrid.m_fontSelItemActive.CreateFontIndirect(&lf);

	lf.lfHeight			= 16;
	lf.lfWeight			= 600;
	lf.lfItalic			= 0;
	lf.lfUnderline		= 0;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Tahoma"));
	treeGrid.m_fontReadOnlyItemActive.CreateFontIndirect(&lf);

	lf.lfHeight			= 16;
	lf.lfWeight			= 500;
	lf.lfItalic			= 0;
	lf.lfUnderline		= 0;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
    _tcscpy_(lf.lfFaceName, _T("Tahoma"));
	treeGrid.m_fontNormalItemActive.CreateFontIndirect(&lf);

	lf.lfHeight			= 16;
	lf.lfWeight			= 600;
	lf.lfItalic			= 0;
	lf.lfUnderline		= 0;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
    _tcscpy_(lf.lfFaceName, _T("Tahoma"));
	treeGrid.m_fontSelItemNonActive.CreateFontIndirect(&lf);

	lf.lfHeight			= 16;
	lf.lfWeight			= 600;
	lf.lfItalic			= 0;
	lf.lfUnderline		= 0;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
    _tcscpy_(lf.lfFaceName, _T("Tahoma"));
	treeGrid.m_fontReadOnlyItemNonActive.CreateFontIndirect(&lf);

	lf.lfHeight			= 16;
	lf.lfWeight			= 500;
	lf.lfItalic			= 0;
	lf.lfUnderline		= 0;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfQuality		= PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Tahoma"));

	treeGrid.m_fontNormalItemNonActive.CreateFontIndirect(&lf);

	_Image bmImage;
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTreeSelItemCollSignImg[0], 10, 10) ){
		treeGrid.m_imageCollapseSignSel.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTreeSelItemExpSignImg[0], 10, 10) ){
		treeGrid.m_imageExpandSignSel.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTreeItemCollSignImg[0], 10, 10) ){
		treeGrid.m_imageCollapseSign.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTreeItemExpSignImg[0], 10, 10) ){
		treeGrid.m_imageExpandSign.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTreeActiveSelImg[0], 8, 20) ){
		treeGrid.m_imageSelActive.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTreeNonActiveSelImg[0], 8, 20) ){
		treeGrid.m_imageSelNonActive.Attach(bmImage.Detach());
		}
	// TreeGrid }}

	// ScrollBar {{
	scrollBar.m_nVScrollWidth	= 15;
	scrollBar.m_nHScrollHeight	= 15;
	scrollBar.m_nArrowBtnCXCY	= 15;

	scrollBar.m_rcOmitVScrollThumb.SetRect(0, 8, 0, 8);
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nVScrollThumbImg[0], g_szVScrollThumbImg.cx, g_szVScrollThumbImg.cy) ){
		scrollBar.m_imgVScrollThumb.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nVScrollBottomSelImg[0], g_szVScrollBottomSelImg.cx, g_szVScrollBottomSelImg.cy) ){
		scrollBar.m_imgVScrollBottomSel.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nVScrollBottomNormalImg[0], g_szVScrollBottomNormalImg.cx, g_szVScrollBottomNormalImg.cy) ){
		scrollBar.m_imgVScrollBottomNormal.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nVScrollMiddleImg[0], g_szVScrollMiddleImg.cx, g_szVScrollMiddleImg.cy) ){
		scrollBar.m_imgVScrollMiddle.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nVScrollTopSelImg[0], g_szVScrollTopSelImg.cx, g_szVScrollTopSelImg.cy) ){
		scrollBar.m_imgVScrollTopSel.Attach(bmImage.Detach());
		}
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nVScrollTopNormalImg[0], g_szVScrollTopNormalImg.cx, g_szVScrollTopNormalImg.cy) ){
		scrollBar.m_imgVScrollTopNormal.Attach(bmImage.Detach());
		}

	// Horizontal Scroll
	scrollBar.m_imgVScrollBottomNormal.Rotate90Bitmap32bpp(scrollBar.m_imgHScrollLeftNormal);
	scrollBar.m_imgVScrollBottomSel.Rotate90Bitmap32bpp(scrollBar.m_imgHScrollLeftSel);
	scrollBar.m_imgVScrollTopNormal.Rotate90Bitmap32bpp(scrollBar.m_imgHScrollRightNormal);
	scrollBar.m_imgVScrollTopSel.Rotate90Bitmap32bpp(scrollBar.m_imgHScrollRightSel);
	scrollBar.m_imgVScrollThumb.Rotate90Bitmap32bpp(scrollBar.m_imgHScrollThumb);
	scrollBar.m_imgVScrollMiddle.Rotate90Bitmap32bpp(scrollBar.m_imgHScrollMiddle);

	scrollBar.m_rcOmitHScrollThumb.SetRect(8, 0, 8, 0);

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nScrollBarDivider[0], g_szScrollBarDivider.cx, g_szScrollBarDivider.cy) ){
		scrollBar.m_imgScrollBarDivider.Attach(bmImage.Detach());
		}
	// ScrollBar }}

	// Header control
	header.m_nHeight		= 22;
	header.m_nBorderWidth	= 1;
	header.m_crBorder		= RGB(144, 145, 149);
	header.m_crDragItemBkgnd= RGB(136, 136, 136);

	lf.lfHeight				= 16;
	lf.lfWeight				= FW_NORMAL;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfQuality			= PROOF_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));

	header.m_fontItemText.CreateFontIndirect(&lf);

	header.m_crItemText_normal	= RGB(42, 42, 42);
	header.m_crItemText_sel		= RGB(42, 42, 42);

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nHeaderItemNormalImg[0], g_szHeaderItemNormalImg.cx, g_szHeaderItemNormalImg.cy) ){
		header.m_imageItem_normal.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nHeaderItemSelImg[0], g_szHeaderItemSelImg.cx, g_szHeaderItemSelImg.cy) ){
		header.m_imageItem_sel.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nHeaderItemDivNormalImg[0], g_szHeaderItemDivNormalImg.cx, g_szHeaderItemDivNormalImg.cy) ){
		header.m_imageItemDiv_normal.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nHeaderItemDivSelImg[0], g_szHeaderItemDivSelImg.cx, g_szHeaderItemDivSelImg.cy) ){
		header.m_imageItemDiv_sel.Attach(bmImage.Detach());
		}
	// Header control

	// List control.
	list.m_nItemHeight		= 20;

	lf.lfHeight				= 15;
	lf.lfWeight				= FW_NORMAL;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));
	list.m_fontItemTextNormal.CreateFontIndirect(&lf);

	lf.lfHeight				= 15;
	lf.lfWeight				= FW_NORMAL;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));
	list.m_fontItemTextSel.CreateFontIndirect(&lf);

	lf.lfHeight				= 15;
	lf.lfWeight				= FW_NORMAL;
	lf.lfItalic				= 1;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));
	list.m_fontItemTextDisabled.CreateFontIndirect(&lf);

	list.m_crBorder					= RGB(102, 102, 102);

	list.m_crItemNormal				= RGB(0, 0, 0);	
	list.m_crItemSel				= RGB(75, 149, 229);		
	list.m_crItemTextDisabled		= RGB(0, 0, 0);	

	list.m_crItemTextNormal			= RGB(0, 0, 0);	
	list.m_crItemTextSel			= RGB(255, 255, 255);		
	list.m_crItemDisabled			= RGB(127, 127, 127);	

	list.m_crRow1					= RGB(212, 223, 232);
	list.m_crRow2					= RGB(255, 255, 255);

	list.m_crDragItemBkgnd			= RGB(136, 136, 136);
	// List control.

	// SwitchBar Control.
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nSwitchBarActiveImg[0], g_szSwitchBarActiveImg.cx, g_szSwitchBarActiveImg.cy) ){
		switchBar.m_imgItemActive.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nSwitchBarNormalImg[0], g_szSwitchBarNormalImg.cx, g_szSwitchBarNormalImg.cy) ){
		switchBar.m_imgItemNormal.Attach(bmImage.Detach());
		}

	switchBar.m_nHeight			= 38;
	switchBar.m_crBkgnd			= RGB(228, 228, 228);
	switchBar.m_crTextNormal	= RGB(0, 0, 0);
	switchBar.m_crTextActive	= RGB(0, 0, 0);
	switchBar.m_rcOmitBorder	= _Rect(5, 5, 5, 5);
	switchBar.m_crDividerActive	= RGB(51, 83, 108);
	switchBar.m_crDividerNormal	= RGB(144, 144, 144);

	lf.lfHeight				= 16;
	lf.lfWeight				= FW_NORMAL;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfQuality			= PROOF_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Microsoft Sans Serif"));

	switchBar.m_fontItemTextNormal.CreateFontIndirect(&lf);

	lf.lfHeight				= 16;
	lf.lfWeight				= FW_BOLD;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfQuality			= PROOF_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Microsoft Sans Serif"));

	switchBar.m_fontItemTextActive.CreateFontIndirect(&lf);
	// SwitchBar Control.

	// ComboBox control.
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nComboBoxButtonNormalImg[0], g_szComboBoxButtonNormalImg.cx, g_szComboBoxButtonNormalImg.cy) ){
		comboBox.m_imgComboButton.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nComboBoxButtonActiveImg[0], g_szComboBoxButtonActiveImg.cx, g_szComboBoxButtonActiveImg.cy) ){
		comboBox.m_imgComboButtonActive.Attach(bmImage.Detach());
		}
	// ComboBox control.

	// LookupBox control.
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nLookupBoxButtonNormalImg[0], g_szLookupBoxButtonNormalImg.cx, g_szLookupBoxButtonNormalImg.cy) ){
		lookupBox.m_imgLookupButton.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nLookupBoxButtonPushedImg[0], g_szLookupBoxButtonPushedImg.cx, g_szLookupBoxButtonPushedImg.cy) ){
		lookupBox.m_imgLookupButtonPushed.Attach(bmImage.Detach());
		}
	// LookupBox control.

	// ComboLisBox control.
	comboListBox.m_nItemHeight	= 18;
	comboListBox.m_crText		= RGB(0, 0, 0);
	comboListBox.m_crTextSel	= RGB(255, 255, 255);
	comboListBox.m_crSelection	= RGB(51, 153, 255);
	comboListBox.m_crFill		= RGB(255, 255, 255);
	comboListBox.m_crBorder		= RGB(102, 102, 102);

	lf.lfHeight					= 17;
	lf.lfWeight					= FW_MEDIUM;
	lf.lfItalic					= 0;
	lf.lfUnderline				= 0;
	lf.lfStrikeOut				= 0;
	lf.lfCharSet				= ANSI_CHARSET;
	lf.lfQuality				= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily			= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Microsoft Sans Serif"));
	comboListBox.m_textFont.CreateFontIndirect(&lf);
	// ComboLisBox control.

	// TextBox control.
	textbox.m_crText			= RGB(0, 0, 0);
	textbox.m_crSelectedText	= RGB(255, 255, 255);
	textbox.m_crSelectionBg		= RGB(51, 153, 255);
	textbox.m_crBorder			= RGB(102, 102, 102);
	textbox.m_crFill			= RGB(255, 255, 255);
	textbox.m_crAlternateText	= RGB(102, 102, 102);

	lf.lfHeight					= 16;
	lf.lfWeight					= FW_MEDIUM;
	lf.lfItalic					= 0;
	lf.lfUnderline				= 0;
	lf.lfStrikeOut				= 0;
	lf.lfCharSet				= ANSI_CHARSET;
	lf.lfQuality				= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily			= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Microsoft Sans Serif"));
	textbox.m_textFont.CreateFontIndirect(&lf);

	lf.lfHeight					= 16;
	lf.lfWeight					= FW_MEDIUM;
	lf.lfItalic					= 0;
	lf.lfUnderline				= 0;
	lf.lfStrikeOut				= 0;
	lf.lfCharSet				= ANSI_CHARSET;
	lf.lfQuality				= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily			= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Microsoft Sans Serif"));
	textbox.m_alternateTextFont.CreateFontIndirect(&lf);

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTextBoxCloseNormalImg[0], g_szTextBoxCloseNormalImg.cx, g_szTextBoxCloseNormalImg.cy) ){
		textbox.m_imgCloseBoxNormal.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTextBoxCloseHoverImg[0], g_szTextBoxCloseHoverImg.cx, g_szTextBoxCloseHoverImg.cy) ){
		textbox.m_imgCloseBoxHover.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nTextBoxClosePushedImg[0], g_szTextBoxClosePushedImg.cx, g_szTextBoxClosePushedImg.cy) ){
		textbox.m_imgCloseBoxPushed.Attach(bmImage.Detach());
		}
	// TextBox control.

	// Label control.
	label.m_crText				= RGB(255, 255, 255);
	label.m_crFill				= RGB(255, 255, 255);

	lf.lfHeight					= 16;
	lf.lfWeight					= FW_NORMAL;
	lf.lfItalic					= 0;
	lf.lfUnderline				= 0;
	lf.lfStrikeOut				= 0;
	lf.lfCharSet				= ANSI_CHARSET;
	lf.lfQuality				= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily			= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));
	label.m_textFont.CreateFontIndirect(&lf);
	// Label control.

	// Imagebutton control. {{
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nImageButtonImages[0], g_szImageButtonImages.cx, g_szImageButtonImages.cy) ){
		_Image imgFull;
		imgFull.Attach(bmImage.Detach());
		imgFull.GetImageFromImage(_Rect(0, 0, 34, 23), imagebutton.m_imageNomal);
		imgFull.GetImageFromImage(_Rect(0, 23, 34, 46), imagebutton.m_imageHover);
		imgFull.GetImageFromImage(_Rect(0, 46, 34, 69), imagebutton.m_imagePushed);
		imgFull.GetImageFromImage(_Rect(0, 69, 34, 92), imagebutton.m_imageDisabled);
		imgFull.Destroy();
		}

	imagebutton.m_rcOmitBorder.SetRect(13, 11, 12, 11);	

	lf.lfHeight					= 16;
	lf.lfWeight					= FW_NORMAL;
	lf.lfItalic					= 0;
	lf.lfUnderline				= 0;
	lf.lfStrikeOut				= 0;
	lf.lfCharSet				= ANSI_CHARSET;
	lf.lfQuality				= PROOF_QUALITY;
	lf.lfPitchAndFamily			= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));
	imagebutton.m_textFont.CreateFontIndirect(&lf);
	imagebutton.m_crText		= RGB(0, 0, 0);
	// Imagebutton control. }}

	// Tab control. {{
	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nSwitchBarActiveImg[0], g_szSwitchBarActiveImg.cx, g_szSwitchBarActiveImg.cy) ){
		tabctrl.m_imageItemActive.Attach(bmImage.Detach());
		}

	if( bmImage.CreateDIBBitmap(32, (UINT*)&g_nSwitchBarNormalImg[0], g_szSwitchBarNormalImg.cx, g_szSwitchBarNormalImg.cy) ){
		tabctrl.m_imageItemNormal.Attach(bmImage.Detach());
		}
	tabctrl.m_rcOmitBorder.SetRect(6, 10, 6, 10);

	lf.lfHeight					= 16;
	lf.lfWeight					= FW_NORMAL;
	lf.lfItalic					= 0;
	lf.lfUnderline				= 0;
	lf.lfStrikeOut				= 0;
	lf.lfCharSet				= ANSI_CHARSET;
	lf.lfQuality				= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily			= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));
	tabctrl.m_itemTextFont.CreateFontIndirect(&lf);

	lf.lfHeight					= 16;
	lf.lfWeight					= FW_NORMAL;
	lf.lfItalic					= 0;
	lf.lfUnderline				= 0;
	lf.lfStrikeOut				= 0;
	lf.lfCharSet				= ANSI_CHARSET;
	lf.lfQuality				= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily			= DEFAULT_PITCH;
	_tcscpy_(lf.lfFaceName, _T("Verdana"));
	tabctrl.m_activeItemTextFont.CreateFontIndirect(&lf);

	tabctrl.m_crNormalItemText	= RGB(0, 0, 0);
	tabctrl.m_crActiveItemText	= RGB(0, 0, 0);
	// Tab control. }}

	m_bInited = TRUE;
	return TRUE;
	}

void
MercuryGUI::Destroy(){
	m_bInited = FALSE;

	// Delete cached images. {{
	for(int i=0; i<m_arrImagesByName.GetCount(); i++){
		_Image* pImage = (_Image*)m_arrImagesByName.GetValueByIndex(i);
		if( pImage ){
			delete pImage;
			}
		}
	m_arrImagesByName.DeleteAll();
	// }}

	// TreeGrid {{
	treeGrid.m_fontSelItemActive		.DeleteObject();
	treeGrid.m_fontReadOnlyItemActive	.DeleteObject();
	treeGrid.m_fontNormalItemActive		.DeleteObject();
	treeGrid.m_fontSelItemNonActive		.DeleteObject();
	treeGrid.m_fontReadOnlyItemNonActive.DeleteObject();
	treeGrid.m_fontNormalItemNonActive	.DeleteObject();

	treeGrid.m_imageCollapseSignSel.Destroy();
	treeGrid.m_imageExpandSignSel.Destroy();
	treeGrid.m_imageCollapseSign.Destroy();
	treeGrid.m_imageExpandSign.Destroy();
	treeGrid.m_imageSelActive.Destroy();
	treeGrid.m_imageSelNonActive.Destroy();
	// TreeGrid }}

	// ScrollBar {{
	scrollBar.m_imgVScrollThumb.Destroy();
	scrollBar.m_imgVScrollBottomSel.Destroy();
	scrollBar.m_imgVScrollBottomNormal.Destroy();
	scrollBar.m_imgVScrollMiddle.Destroy();
	scrollBar.m_imgVScrollTopSel.Destroy();
	scrollBar.m_imgVScrollTopNormal.Destroy();

	scrollBar.m_imgHScrollThumb.Destroy();
	scrollBar.m_imgHScrollRightSel.Destroy();
	scrollBar.m_imgHScrollRightNormal.Destroy();
	scrollBar.m_imgHScrollMiddle.Destroy();
	scrollBar.m_imgHScrollLeftSel.Destroy();
	scrollBar.m_imgHScrollLeftNormal.Destroy();
	scrollBar.m_imgScrollBarDivider.Destroy();
	// ScrollBar }}

	// ComboBox {{
	comboBox.m_imgComboButton.Destroy();
	comboBox.m_imgComboButtonActive.Destroy();
	// ComboBox }}

	// List header. {{
	header.m_imageItemDiv_normal.Destroy();
	header.m_imageItemDiv_sel.Destroy();
	header.m_imageItem_normal.Destroy();
	header.m_imageItem_sel.Destroy();
	// }}

	// Switch bar. {{
	switchBar.m_imgItemActive.Destroy();
	switchBar.m_imgItemNormal.Destroy();
	// }}

	// lookup Box. {{
	lookupBox.m_imgLookupButton.Destroy();
	lookupBox.m_imgLookupButtonPushed.Destroy();
	// }}

	// Text box. {{
	textbox.m_imgCloseBoxHover.Destroy();
	textbox.m_imgCloseBoxNormal.Destroy();
	textbox.m_imgCloseBoxPushed.Destroy();
	// }}

	// ImageButton. {{
	imagebutton.m_imageNomal.Destroy();
	imagebutton.m_imageHover.Destroy();
	imagebutton.m_imagePushed.Destroy();
	imagebutton.m_imageDisabled.Destroy();
	// ImageButton. }}

	// Tab control. {{
	tabctrl.m_imageItemActive.Destroy();
	tabctrl.m_imageItemNormal.Destroy();
	// Tab control. }}

	// TextBox control. {{
	textbox.m_imgCloseBoxHover.Destroy();
	textbox.m_imgCloseBoxNormal.Destroy();
	textbox.m_imgCloseBoxPushed.Destroy();
	// TextBox control. }}
	}