// EmbedeedFontMan.cpp source file.
//

#include "stdafx.h"
#include "EmbedeedFontMan.h"
#include "..\Path\Path.h"
#include "..\Utility\StringHelper.h"
#include "ImageHelper.h"

// Constructor / Destructor
EmbedeedFontMan::EmbedeedFontMan(){
	m_fontTemp.CreatePointFont(10, _T("Arial"));
	}

EmbedeedFontMan::~EmbedeedFontMan(){
	}

bool
EmbedeedFontMan::AddFont(const CString sFontFolder, const CString sFontName){
	CString sDir = sFontFolder;
	if( sDir.GetAt(sDir.GetLength() - 1) != '\\' )
		sDir += _T("\\");

	EmbedeedFont* pFont = new EmbedeedFont();

	// Load characters code map. {{
	BYTE*	lpContent	= NULL;
	int		nSize		= 0;
	if( Path::ReadFileContent(sDir + _T("codes.txt"), lpContent, nSize) && nSize > 0 ){
		CString sContent;
		memcpy(sContent.GetBufferSetLength(nSize), lpContent, nSize);

		CStringArray arrStrings;
		StringHelper::Split(&sContent, _T(";"), arrStrings);

		CString sCharCode;
		for(int i=0; i<arrStrings.GetCount(); i++){
			sCharCode		= arrStrings.GetAt(i);
			int nCharCode	= 0;
			// Hex value
			if( sCharCode.GetAt(0) == '#' )
				nCharCode = StringHelper::HexStringIntoInt(&((TCHAR*)sCharCode.GetBuffer())[1], sCharCode.GetLength() - 1);
			else
				nCharCode = _ttoi(sCharCode.GetBuffer());
			pFont->m_arrCharacters.Add((void*)nCharCode, (void*)i);
			}
		}
	else{
#ifdef _DEBUG
		CString sMsg;
		sMsg.Format(_T("Couldn't find %s"), sDir + _T("codes.txt"));
		AfxMessageBox(sMsg);
#endif
		if( lpContent )
			delete [] lpContent;
		delete pFont;
		return false;
		}
	// }}

	if( lpContent ){
		delete [] lpContent;
		lpContent = NULL;
		}

	CStringArray arrFileNames;
	if( Path::GetFilesByExtention(sDir, _T("*.bmp"), arrFileNames, true) == 0 ){
		delete pFont;
		return false;
		}
	
	for(int i=0; i<arrFileNames.GetCount(); i++){
		CString sName = arrFileNames.GetAt(i);
		CString sName2 = sName;
		CString sFile = sDir + sName;

		if( sName.Left(sFontName.GetLength()) != sFontName )
			continue; // Skip wrong file name.

		sName.Delete	(0, sFontName.GetLength());
		sName.MakeLower	();

		int nIndex = 0;
		EmbedeedFontItemInfo* pInfo = new EmbedeedFontItemInfo();
		pInfo->ZeroInit();

		// Bold
		if( sName.GetAt(nIndex) == 'b' ){
			pInfo->m_nCharFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_BOLD;
			nIndex ++;
			}
		// Underline
		if( sName.GetAt(nIndex) == 'u' ){
			pInfo->m_nCharFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_UNDERLINE;
			nIndex ++;
			}
		// Italic
		if( sName.GetAt(nIndex) == 'i' ){
			pInfo->m_nCharFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_ITALIC;
			nIndex ++;
			}

		// Load characters map image and detect characters left offset and width (in pixels). {{
		CImage* pImage = ImageHelper::LoadImage(sFile, -1, -1, false);
		if( pImage ){
			BITMAP bmImage;
			GetObject(*pImage, sizeof(BITMAP), &bmImage);

			int			nWidthLimit				= bmImage.bmWidth;
			int			nSymbolCt				= 0;
			BYTE*		pBits					= (BYTE*)bmImage.bmBits;
			int			nXOffset				= 0;
			int			nBPP					= bmImage.bmBitsPixel/8;
			int			nSymbolMostLeftOffset	= -1;
			int			nSymbolMostRightOffset	= -1;
			CDWordArray arrSymbolLeftOffsetAndWidth;
			CString		sSymbolStartPointAndWidth;

			int nMaxXOffset = bmImage.bmWidth;
			for(int x=nXOffset; x<nMaxXOffset; x++){
				bool bClearLine = true;
				for(int y=0; y<bmImage.bmHeight; y++){
					BYTE* pLineStartingBits = &pBits[((bmImage.bmHeight - y - 1)*bmImage.bmWidthBytes)];
					BYTE* pPixel			= &pLineStartingBits[x*nBPP];

					// Is pixel visible.
					if( pPixel[0] > 10 ){
						bClearLine = false;
						if( nSymbolMostLeftOffset > -1 )
							nSymbolMostRightOffset = x;
						else
							nSymbolMostLeftOffset = x;
						continue;
						}
		
					pPixel = &pPixel[nBPP];
					}

				if( bClearLine && nSymbolMostLeftOffset > -1 ){
					if( nSymbolMostRightOffset == -1 )
						nSymbolMostRightOffset = nSymbolMostLeftOffset;
					DWORD dwCombineVal = (nSymbolMostLeftOffset << 16) | ((nSymbolMostRightOffset - nSymbolMostLeftOffset + 1)&0xFFFF);
					arrSymbolLeftOffsetAndWidth.Add(dwCombineVal);

#ifdef _DEBUG
					CString sFormat;
					sFormat.Format(_T("%d:%d;"), nSymbolMostLeftOffset, nSymbolMostRightOffset - nSymbolMostLeftOffset + 1);
					sSymbolStartPointAndWidth += sFormat;
#endif
					nSymbolMostLeftOffset	= -1;
					nSymbolMostRightOffset	= -1;
					}
				}

			pInfo->m_imageCharacters.Attach(pImage->Detach());
			delete pImage;

			if( (arrSymbolLeftOffsetAndWidth.GetCount() == pFont->m_arrCharacters.GetCount()) ){
				pInfo->m_pCharPoints = new CPoint[arrSymbolLeftOffsetAndWidth.GetCount()];
				for(int j=0; j<arrSymbolLeftOffsetAndWidth.GetCount(); j++){
					DWORD dwCombineVal = arrSymbolLeftOffsetAndWidth.GetAt(j);
					pInfo->m_pCharPoints[j].x = (int)(dwCombineVal>>16);
					pInfo->m_pCharPoints[j].y = (int)(dwCombineVal&0xFFFF);
					}
				}
			else{
#ifdef _DEBUG
				CBitmap bmImg;
				ImageHelper::CreateDIBBitmap(pInfo->m_imageCharacters.GetBPP(), bmImg, RGB(0, 0, 0), pInfo->m_imageCharacters.GetWidth(),
					pInfo->m_imageCharacters.GetHeight()*2, 0);

				CDC memDC;
				memDC.CreateCompatibleDC(NULL);
				::SelectObject(memDC, bmImg.m_hObject);

				CDC srcDC;
				srcDC.CreateCompatibleDC(NULL);
				::SelectObject(srcDC, pInfo->m_imageCharacters);
				memDC.BitBlt(0, 0, pInfo->m_imageCharacters.GetWidth(), pInfo->m_imageCharacters.GetHeight(), &srcDC, 0, 0, SRCCOPY);

				int nHeight = pInfo->m_imageCharacters.GetHeight();
				for(int j=0; j<arrSymbolLeftOffsetAndWidth.GetCount(); j++){
					DWORD dwCombineVal = arrSymbolLeftOffsetAndWidth.GetAt(j);
					int nLeft = (int)(dwCombineVal>>16);
					int nWidth = (int)(dwCombineVal&0xFFFF);

					memDC.FillSolidRect(nLeft, nHeight, nWidth, nHeight, RGB(255, 255, 0));
					}

				DeleteFile(sDir + _T("__") + sName2);
				CImage img;
				img.Attach((HBITMAP)bmImg.Detach());
				img.Save(sDir + _T("__") + sName2);
				img.Destroy();
				
				CString sMsg;
				sMsg.Format(_T("Characters count is not the same for font '%s' item '%s'"), sFontName, sName);
				AfxMessageBox(sMsg);
#endif
				delete pInfo;
				continue;
				}

			pInfo->m_nCharCount = arrSymbolLeftOffsetAndWidth.GetCount();
			}
		// }}

		int nFontVal = _ttoi(&((TCHAR*)sName.GetBuffer())[nIndex]);
		if( nFontVal > 0 ){
			pInfo->m_nFontVal = nFontVal;
			}
		else{
			delete pInfo;
			continue;
			}

		// Add embedeed font item.
		pFont->m_listItems.Add(pInfo);
		}

	// Finalize embedeed font loading.
	pFont->m_sFontName	= sFontName;
	pFont->m_nFontId	= m_listEmbedeedFonts.GetCount();
	m_listEmbedeedFonts.Add(pFont);
	m_arrFontNameById.Add(sFontName, pFont->m_nFontId);
	return true;
	}

int
EmbedeedFontMan::GetEmbedeedFontId(const CString sFontName){
	int nFontId = 0;
	if( m_arrFontNameById.KeyExists(sFontName, &nFontId) )
		return nFontId;
	return -1;
	}

bool
EmbedeedFontMan::DrawTextByEmbedeedFont(int nEmbedeedFontId, CDC* pDC, wchar_t* pwszText, int nLen, int x, int y, const RECT* lpRect){
	EmbedeedFont* pFont = m_listEmbedeedFonts.GetAt(nEmbedeedFontId);
	if( pFont ){
		COLORREF	crText			= pDC->GetTextColor();
		int			nHeight			= 0;
		bool		bItalic			= false, bUnderLine = false, bBold = false;
	//	pDC->FillSolidRect(lpRect->left, lpRect->top, (lpRect->right - lpRect->left), (lpRect->bottom - lpRect->top), RGB(122, 122, 0));

		CFont* pFontOld = pDC->SelectObject(&m_fontTemp);
		if( pFontOld ){
			LOGFONT lf;
			pFontOld->GetLogFont(&lf);
			bItalic			= (lf.lfItalic != 0);
			bBold			= (lf.lfWeight == FW_BOLD);
			bUnderLine		= (lf.lfUnderline != 0);
			if( lf.lfHeight < 0 )
				nHeight =  -1*MulDiv(lf.lfHeight, 72, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY));
			else
				nHeight = lf.lfHeight;
			pDC->SelectObject(pFontOld);
			}
		
		if( nHeight > 0 ){
			EmbedeedFontItemInfo*	pFontItem		= pFont->GetFontItem(nHeight, bBold, bItalic, bUnderLine);
			if( !pFontItem ) return false;
			CImage*					pImgSymbolsMap	= &pFontItem->m_imageCharacters;

			CRect rcDrawRect(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);

			bool	bBitBlt		= false;
			HBITMAP hDcDibImage = NULL;
			CRect	rcDestBitBlt(0, 0, 0, 0);
			if( !ImageHelper::GetSelectedDibImageFromDC(pDC, hDcDibImage) ){
				CBitmap bmImage;
				if( !ImageHelper::GetBitmapFromDC(24, bmImage, pDC, lpRect->left, lpRect->top, (lpRect->right - lpRect->left), (lpRect->bottom - lpRect->top), NULL) ){
					ASSERT(FALSE);
					return false;
					}
	
				bBitBlt			= true;
				hDcDibImage		= (HBITMAP)bmImage.Detach();
				int nWidth		= (lpRect->right - lpRect->left);
				int nHeight		= (lpRect->bottom - lpRect->top);

				if( lpRect->left < 0 )
					rcDestBitBlt.left = 0;
				else
					rcDestBitBlt.left = lpRect->left;

				if( lpRect->top < 0 )
					rcDestBitBlt.top = 0;
				else
					rcDestBitBlt.top = lpRect->top;
				rcDestBitBlt.right	= rcDestBitBlt.left + nWidth;
				rcDestBitBlt.bottom = rcDestBitBlt.top + nHeight;

				x					-= lpRect->left;
				y					-= lpRect->top;
				rcDrawRect.left		= 0;
				rcDrawRect.top		= 0;
				rcDrawRect.right	= nWidth;
				rcDrawRect.bottom	= nHeight;
				}

			CRect	rcSymbolImage, rcSymbolDC, rcInvalid;
			rcSymbolDC.left			= x;
			rcSymbolDC.right		= x;
			rcSymbolDC.top			= y;
			rcSymbolDC.bottom		= rcSymbolDC.top + pImgSymbolsMap->GetHeight();
			
			BITMAP bmDibImage;
			GetObject(hDcDibImage, sizeof(BITMAP), &bmDibImage);

			COLORREF	crBk		= 0;
			bool		bUseBkColor = false;
			if( pDC->GetBkMode() != TRANSPARENT ){
				crBk		= pDC->GetBkColor();
				bUseBkColor = true;
				}

			// Distance between symbols.
			int nDistanceCX	= GetDistanceBetweenSymbols(), nSpaceSymbolCX	= 4, nTabSymbolCX = 6;
			CBitmap bmSymbol;

			for(int i=0; i<nLen; i++){
				int nSymbolCode = pwszText[i];
				if( nSymbolCode == VK_SPACE ){
					rcSymbolDC.right	= rcSymbolDC.left + nSpaceSymbolCX;
					rcInvalid			= rcSymbolDC & rcDrawRect;
					// Fill background.
					if( bUseBkColor && !rcInvalid.IsRectEmpty() )
						pDC->FillSolidRect(rcInvalid.left, rcInvalid.top, rcInvalid.Width(), rcInvalid.Height(), crBk);
					rcSymbolDC.left		= rcSymbolDC.right;
					continue;
					}

				if( nSymbolCode == VK_TAB ){
					rcSymbolDC.right	= rcSymbolDC.left + nTabSymbolCX;
					rcInvalid			= rcSymbolDC & rcDrawRect;
					// Fill background.
					if( bUseBkColor && !rcInvalid.IsRectEmpty() )
						pDC->FillSolidRect(rcInvalid.left, rcInvalid.top, rcInvalid.Width(), rcInvalid.Height(), crBk);
					rcSymbolDC.left		= rcSymbolDC.right;
					continue;
					}

				if( !pFont->GetSymbolRectFromImage(pFontItem, nSymbolCode, rcSymbolImage) )
					continue;
				rcSymbolDC.right	+= rcSymbolImage.Width();

				rcInvalid = rcSymbolDC & rcDrawRect;
				// Skip symbol.
				if( rcInvalid.right <= 0 || rcInvalid.bottom <= 0 ){
					rcSymbolDC.left		= rcSymbolDC.right + nDistanceCX;
					rcSymbolDC.right	= rcSymbolDC.left;
					continue;
					}

				if( !rcInvalid.IsRectEmpty() ){
					// Fill background.
					if( bUseBkColor )
						pDC->FillSolidRect(rcInvalid.left, rcInvalid.top, rcInvalid.Width() + nDistanceCX, rcInvalid.Height(), crBk);

					// Biggest(Widest) image to use once for all symbols.
					if( !bmSymbol.m_hObject )
						ImageHelper::CreateDIBBitmap(32, bmSymbol, crText, rcSymbolImage.Height(), rcSymbolImage.Height(), NULL);
					
					// Copy alpha channel.
					if( ImageHelper::CopyAlphaChannel((HBITMAP)bmSymbol.m_hObject, *pImgSymbolsMap, rcSymbolImage) ){
						int nDrawCX = rcInvalid.Width(), nDrawCY = rcInvalid.Height();
						if( (rcInvalid.left + nDrawCX) > bmDibImage.bmWidth )
							nDrawCX = (bmDibImage.bmWidth - rcInvalid.left);
						if( (rcInvalid.top + nDrawCY) > bmDibImage.bmHeight )
							nDrawCY = (bmDibImage.bmHeight - rcInvalid.top);

						//int nImageSrcX = (rcSymbolImage.Width() - nDrawCX), nImageSrcY = (rcSymbolImage.Height() - nDrawCY);
						int nImageSrcX = abs(rcSymbolDC.left - rcInvalid.left), nImageSrcY = abs(rcSymbolDC.top - rcInvalid.top);
						if( rcInvalid.top < 0 ){
							nImageSrcY		= -1*rcInvalid.top;
							rcInvalid.top	= 0;
							}

						if( rcInvalid.left < 0 ){
							nImageSrcX		= -1*rcInvalid.left;
							rcInvalid.left	= 0;
							}
						
						if( nDrawCX > 0 && nDrawCY > 0 ){
							ImageHelper::BitmapOnBitmap(hDcDibImage, rcInvalid.left, rcInvalid.top, 
								(HBITMAP)bmSymbol.m_hObject, nImageSrcX, nImageSrcY, nDrawCX, nDrawCY, nDrawCX, nDrawCY, TRUE);
							}
						}
					}

				rcSymbolDC.left		= rcSymbolDC.right + nDistanceCX;
				rcSymbolDC.right	= rcSymbolDC.left;
				}

			if( bBitBlt ){
				CDC srcDC;
				srcDC.CreateCompatibleDC(NULL);
				::SelectObject(srcDC, hDcDibImage);
				
				BitBlt(pDC->m_hDC, rcDestBitBlt.left, rcDestBitBlt.top, 
					rcDestBitBlt.Width(), rcDestBitBlt.Height(), srcDC, 0, 0, SRCCOPY);
					
				srcDC.DeleteDC();
				::DeleteObject(hDcDibImage);
				hDcDibImage = NULL;
				}
			
			if( bmSymbol.m_hObject )
				bmSymbol.DeleteObject();
			}
		else
			return false;
		}
	return true;
	}

bool
EmbedeedFontMan::GetTextSize(CDC* pDC, wchar_t* pwszText, int nLen, int nFontId, CSize& szText){
	EmbedeedFont* pFont = m_listEmbedeedFonts.GetAt(nFontId);
	if( pFont ){
		int		nSymbolHeight	= 0;
		bool		bItalic		= false, bUnderLine	= false, bBold = false;

		// Distance between symbols.
		int nDistanceCX	= GetDistanceBetweenSymbols(), nSpaceSymbolCX	= 4, nTabSymbolCX = 6;

		CFont*	pFontOld	= pDC->SelectObject(&m_fontTemp);
		if( pFontOld ){
			LOGFONT lf;
			pFontOld->GetLogFont(&lf);
			nSymbolHeight	= abs(lf.lfHeight);
			bItalic			= (lf.lfItalic != 0);
			bBold			= (lf.lfWeight == FW_BOLD);
			bUnderLine		= (lf.lfUnderline != 0);
			if( lf.lfHeight < 0 )
				nSymbolHeight =  -1*MulDiv(lf.lfHeight, 72, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY));
			else
				nSymbolHeight = lf.lfHeight;
			pDC->SelectObject(pFontOld);
			}

		if( nSymbolHeight > 0 ){
			EmbedeedFontItemInfo* pFontItem = pFont->GetFontItem(nSymbolHeight, bBold, bItalic, bUnderLine);
			if( !pFontItem ) 
				return false;

			CRect rcSymbolImage;
			szText.SetSize(0, pFontItem->m_imageCharacters.GetHeight());
			for(int i=0; i<nLen; i++){
				int nSymbolCode = pwszText[i];
				if( nSymbolCode == VK_SPACE ){
					szText.cx += nSpaceSymbolCX;
					continue;
					}

				if( nSymbolCode == VK_TAB ){
					szText.cx += nTabSymbolCX;
					continue;
					}

				if( !pFont->GetSymbolRectFromImage(pFontItem, nSymbolCode, rcSymbolImage) )
					continue;
				szText.cx	+= (rcSymbolImage.Width() + nDistanceCX);
				}
			szText.cx -= nDistanceCX;
			return true;
			}
		}
	return false;
	}

bool
EmbedeedFontMan::DumpSystemFont(const CString sFontName, const CString sFontDumpFolder){
	int nFontSizesDump[] = {8,9,10,12,14,16};
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	_tcscpy_s(lf.lfFaceName, sFontName);
	lf.lfWeight				= FW_MEDIUM;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfQuality			= DRAFT_QUALITY;
	lf.lfPitchAndFamily		= 34;
	lf.lfOutPrecision		= 3;
	lf.lfClipPrecision		= 2;

	LOGFONT lfRegular;
	memcpy(&lfRegular, &lf, sizeof(LOGFONT));

	LOGFONT lfRegularUnderline;
	memcpy(&lfRegularUnderline, &lfRegular, sizeof(LOGFONT));
	lfRegularUnderline.lfUnderline = 1;

	LOGFONT lfRegularItalic;
	memcpy(&lfRegularItalic, &lfRegular, sizeof(LOGFONT));
	lfRegularItalic.lfItalic = 1;

	LOGFONT lfBold;
	memcpy(&lfBold, &lf, sizeof(LOGFONT));
	lfBold.lfWeight = FW_BOLD;

	LOGFONT lfBoldUnderline;
	memcpy(&lfBoldUnderline, &lfBold, sizeof(LOGFONT));
	lfBoldUnderline.lfUnderline = 1;

	LOGFONT lfBoldItalic;
	memcpy(&lfBoldItalic, &lfBold, sizeof(LOGFONT));
	lfBoldItalic.lfItalic = 1;

	wchar_t* wSymbols[] = 
	{ 
		L"!", L"\x22", L"\x23", L"$", L"%", L"&", L"'", L"(", L")", L"*", L"+", L",", L"-", L".", L"/", 
		L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L":", L";", L"<", L"=", L">", L"?", L"@", 
		L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", 
		L"[", L"\x5c", L"]", L"^", L"_", L"`", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o", L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L"{", L"|", L"}", L"~",
		L"ა", L"ბ", L"გ", L"დ", L"ე", L"ვ", L"ზ", L"თ", L"ი", L"კ", L"ლ", L"მ", L"ნ", L"ო", L"პ", 
		L"ჟ", L"რ", L"ს", L"ტ", L"უ", L"ფ", L"ქ", L"ღ", L"ყ", L"შ", L"ჩ", L"ც", L"ძ", L"წ", L"ჭ", L"ხ", L"ჴ", L"ჯ", L"ჰ",
		L"Ё", L"А", L"Б", L"В", L"Г", L"Д", L"Е", L"Ж", L"З", L"И", L"Й", L"К", L"Л", L"М", L"Н", 
		L"О", L"П", L"Р", L"С", L"Т", L"У", L"Ф", L"Х", L"Ц", L"Ч", L"Ш", L"Щ", L"Ъ", L"Ы", L"Ь", 
		L"Э", L"Ю", L"Я", L"а", L"б", L"в", L"г", L"д", L"е", L"ж", L"з", L"и", L"й", L"к", L"л", 
		L"м", L"н", L"о", L"п", L"р", L"с", L"т", L"у", L"ф", L"х", L"ц", L"ч", L"ш", L"щ", L"ъ", L"ы", L"ь", L"э", L"ю",L"я", L"ё"
	};

	int nSymbolsCt				= sizeof(wSymbols)/sizeof(wchar_t*);
	int nDistanceBetweenSymbols = 2;

	CDC memDC;
	memDC.CreateCompatibleDC(NULL);

	LOGFONT* ppLogFonts[] = {&lfRegular, &lfRegularUnderline, &lfRegularItalic, &lfBold, &lfBoldUnderline, &lfBoldItalic};
	for(int k=0; k<sizeof(ppLogFonts)/sizeof(LOGFONT*); k++){
		LOGFONT* pLogFont = ppLogFonts[k];
		for(int i=0; i<sizeof(nFontSizesDump)/sizeof(int); i++){
			CFont font;
			pLogFont->lfHeight =  -MulDiv(nFontSizesDump[i], GetDeviceCaps(memDC.m_hDC, LOGPIXELSY), 72);
			if( !font.CreateFontIndirect(pLogFont) ){
				return false;
				}
			memDC.SelectObject(&font);
		
			AutoSortedArray arrSymbolImages;
			CSize szSymbols(0, 0);

			// Create symbol images.
			for(int j=0; j<nSymbolsCt; j++){
				SIZE szSymbol;
				if( !::GetTextExtentPoint32W(memDC.m_hAttribDC, wSymbols[j], 1, &szSymbol) )
					continue;

				CBitmap bmSymbol;
				if( !ImageHelper::CreateDIBBitmap(24, bmSymbol, RGB(0, 0, 0), szSymbol.cx, szSymbol.cy, NULL) )
					continue;

				::SelectObject	(memDC.m_hDC, bmSymbol.m_hObject);
				::SetTextColor	(memDC, RGB(255, 255, 255));
				::SetBkMode		(memDC, TRANSPARENT);
				::TextOutW		(memDC, 0, 0, wSymbols[j], 1);

				//ImageHelper::SetSoftenFilter((HBITMAP)bmSymbol.m_hObject);

				arrSymbolImages.Add((void*)j, (void*)bmSymbol.Detach());

				szSymbols.cx += (szSymbol.cx + nDistanceBetweenSymbols);
				szSymbols.cy =	max(szSymbols.cy, szSymbol.cy);
				}

			CBitmap bmSymbolsMap;
			if( !ImageHelper::CreateDIBBitmap(24, bmSymbolsMap, RGB(0, 0, 0), szSymbols.cx, szSymbols.cy, NULL) )
				continue;

			::SelectObject(memDC, bmSymbolsMap.m_hObject);

			CDC srcDC;
			srcDC.CreateCompatibleDC(&memDC);

			// Create symbols map image.
			CPoint ptDraw(0, 0);
			for(int loop=0; loop<arrSymbolImages.GetCount(); loop++){
				CBitmap bmSymbol;
				if( !bmSymbol.Attach((HBITMAP)arrSymbolImages.GetData(loop)) )
					continue;

				BITMAP bmpSymbol;
				bmSymbol.GetBitmap(&bmpSymbol);

				ptDraw.y = (szSymbols.cy - bmpSymbol.bmHeight)/2;

				::SelectObject(srcDC, bmSymbol.m_hObject);
				::BitBlt(memDC, ptDraw.x, ptDraw.y, bmpSymbol.bmWidth, bmpSymbol.bmHeight, srcDC, 0, 0, SRCCOPY);
				::DeleteObject(bmSymbol.Detach());

				ptDraw.x += (bmpSymbol.bmWidth + nDistanceBetweenSymbols);
				}

			CString sFileName = sFontDumpFolder;
			if( sFileName.Right(1) != _T("\\") )
				sFileName += _T("\\");

			CString sName = sFontName;
			if( pLogFont->lfWeight == FW_BOLD )
				sName += _T("b");
			if( pLogFont->lfUnderline == 1 )
				sName += _T("u");
			if( pLogFont->lfItalic == 1 )
				sName += _T("i");

			CString sTemp;
			sTemp.Format(_T("%s%02d.bmp"), sName, nFontSizesDump[i]);
			sFileName += sTemp;
			Path::BuildFullPath(sFileName);

			::DeleteFile(sFileName.GetBuffer());
			CImage imgSave;
			imgSave.Attach((HBITMAP)bmSymbolsMap.Detach());
			imgSave.Save(sFileName);

			imgSave.Destroy();
			arrSymbolImages.DeleteAll();
			}
	}
	return false;
	}