// EmbedeedFontMan.cpp source file.
//

#include "EmbedeedFontMan.h"
#include "../Path/Path.h"
#include "../Utility/StringHelper.h"

// Constructor / Destructor
EmbedeedFontMan::EmbedeedFontMan(){
	m_fontTemp.CreateFont(_T("Arial"), 10);
	}

EmbedeedFontMan::~EmbedeedFontMan(){
	}

bool
EmbedeedFontMan::AddFont(const std::string sFontFolder, const std::string sFontName){
	std::string sDir = sFontFolder;
	if( sDir[sDir.length() - 1] != '\\' )
		sDir += _T("\\");

	EmbedeedFont* pFont = new EmbedeedFont();

	// Load characters code map. {{
	BYTE*	lpContent	= NULL;
	int		nSize		= 0;
	if( Path::ReadFileContent(sDir + _T("codes.txt"), lpContent, nSize) && nSize > 0 ){
		std::string sContent;
		sContent.resize(nSize);
		memcpy((void*)sContent.c_str(), lpContent, nSize);
		//memcpy(sContent.GetBufferSetLength(nSize), lpContent, nSize);

		CStringArray arrStrings;
		StringHelper::Split(&sContent, _T(";"), arrStrings);

		std::string sCharCode;
		for(int i=0; i<arrStrings.size(); i++){
			sCharCode		= arrStrings[i];
			int nCharCode	= 0;
			// Hex value
			if( sCharCode[0] == '#' )
				nCharCode = StringHelper::HexStringIntoInt(&((TCHAR*)sCharCode.c_str())[1], sCharCode.length() - 1);
			else
				nCharCode = _ttoi(sCharCode.c_str());
			pFont->m_arrCharacters.Add((void*)nCharCode, (void*)i);
			}
		}
	else{
#ifdef _DEBUG
		std::string sMsg;
		stringEx::Format(sMsg, _T("Couldn't find %s"), (sDir + _T("codes.txt")).c_str());
		::MessageBox(NULL, sMsg.c_str(), _T("Error"), MB_OK);
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
	
	for(int i=0; i<arrFileNames.size(); i++){
		std::string sName = arrFileNames[i];
		std::string sName2 = sName;
		std::string sFile = sDir + sName;

		if( sName.substr(0, sFontName.length()) != sFontName )
			continue; // Skip wrong file name.

		sName.erase(0, sFontName.length());
		stringEx::MakeLower(sName);

		int nIndex = 0;
		EmbedeedFontItemInfo* pInfo = new EmbedeedFontItemInfo();
		pInfo->ZeroInit();

		// Bold
		if( sName[nIndex] == 'b' ){
			pInfo->m_nCharFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_BOLD;
			nIndex ++;
			}
		// Underline
		if( sName[nIndex] == 'u' ){
			pInfo->m_nCharFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_UNDERLINE;
			nIndex ++;
			}
		// Italic
		if( sName[nIndex] == 'i' ){
			pInfo->m_nCharFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_ITALIC;
			nIndex ++;
			}

		// Load characters map image and detect characters left offset and width (in pixels). {{
		Image* pImage = Image::LoadImage(sFile, -1, -1, false);
		if( pImage ){
			int			nWidthLimit				= pImage->GetWidth();
			int			nWidth					= pImage->GetWidth();
			int			nHeight					= pImage->GetHeight();
			int			nWidthBytes				= pImage->GetWidthBytes();
			int			nSymbolCt				= 0;
			BYTE*		pBits					= (BYTE*)pImage->GetBits();
			int			nXOffset				= 0;
			int			nBPP					= pImage->GetBPP()/8;
			int			nSymbolMostLeftOffset	= -1;
			int			nSymbolMostRightOffset	= -1;
			CDWordArray arrSymbolLeftOffsetAndWidth;
			std::string	sSymbolStartPointAndWidth;

			int			nMaxXOffset				= nWidth;
			for(int x=nXOffset; x<nMaxXOffset; x++){
				bool bClearLine = true;
				for(int y=0; y<nHeight; y++){
					BYTE* pLineStartingBits = &pBits[((nHeight - y - 1)*nWidthBytes)];
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
					arrSymbolLeftOffsetAndWidth.push_back(dwCombineVal);

#ifdef _DEBUG
					std::string sFormat;
					stringEx::Format(sFormat, _T("%d:%d;"), nSymbolMostLeftOffset, nSymbolMostRightOffset - nSymbolMostLeftOffset + 1);
					sSymbolStartPointAndWidth += sFormat;
#endif
					nSymbolMostLeftOffset	= -1;
					nSymbolMostRightOffset	= -1;
					}
				}

			pInfo->m_imageCharacters.Attach(pImage->Detach());
			delete pImage;

			if( (arrSymbolLeftOffsetAndWidth.size() == pFont->m_arrCharacters.GetCount()) ){
				pInfo->m_pCharPoints = new Point[arrSymbolLeftOffsetAndWidth.size()];
				for(int j=0; j<arrSymbolLeftOffsetAndWidth.size(); j++){
					DWORD dwCombineVal = arrSymbolLeftOffsetAndWidth.at(j);
					pInfo->m_pCharPoints[j].x = (int)(dwCombineVal>>16);
					pInfo->m_pCharPoints[j].y = (int)(dwCombineVal&0xFFFF);
					}
				}
			else{
#ifdef _DEBUG
				Image bmImg;
				bmImg.CreateDIBBitmap(pInfo->m_imageCharacters.GetBPP(), RGB(0, 0, 0), pInfo->m_imageCharacters.GetWidth(),
					pInfo->m_imageCharacters.GetHeight()*2, 0);

				_DC memDC;
				memDC.CreateCompatibleDC(NULL);
				memDC.SelectObject(bmImg);

				_DC srcDC;
				srcDC.CreateCompatibleDC(NULL);
				srcDC.SelectObject(pInfo->m_imageCharacters);
				memDC.BitBlt(0, 0, pInfo->m_imageCharacters.GetWidth(), pInfo->m_imageCharacters.GetHeight(), srcDC, 0, 0, SRCCOPY);

				int nHeight = pInfo->m_imageCharacters.GetHeight();
				for(int j=0; j<arrSymbolLeftOffsetAndWidth.size(); j++){
					DWORD dwCombineVal = arrSymbolLeftOffsetAndWidth.at(j);
					int nLeft = (int)(dwCombineVal>>16);
					int nWidth = (int)(dwCombineVal&0xFFFF);

					memDC.FillSolidRect(Rect(nLeft, nHeight, nLeft+nWidth, nHeight+nHeight), RGB(255, 255, 0));
					}

				DeleteFile((sDir + _T("__") + sName2).c_str());
				Image img;
				img.Attach((HBITMAP)bmImg.Detach());
//				img.Save(sDir + _T("__") + sName2);
				img.Destroy();
				
				std::string sMsg;
				stringEx::Format(sMsg, _T("Characters count is not the same for font '%s' item '%s'"), sFontName.c_str(), sName.c_str());
				MessageBox(NULL, sMsg.c_str(), _T("Error"), MB_OK);
#endif
				delete pInfo;
				continue;
				}

			pInfo->m_nCharCount = arrSymbolLeftOffsetAndWidth.size();
			}
		// }}

		int nFontVal = _ttoi(&((TCHAR*)sName.c_str())[nIndex]);
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
EmbedeedFontMan::GetEmbedeedFontId(std::string sFontName){
	int nFontId = 0;
	if( m_arrFontNameById.KeyExists(sFontName, &nFontId) )
		return nFontId;
	return -1;
	}

bool
EmbedeedFontMan::DrawTextByEmbedeedFont(int nEmbedeedFontId, _DC* pDC, const wchar_t* pwszText, int nLen, int x, int y, const RECT* lpRect){
	EmbedeedFont* pFont = m_listEmbedeedFonts.GetAt(nEmbedeedFontId);
	if( pFont ){
		COLORREF	crText			= pDC->GetTextColor();
		int			nHeight			= 0;
		bool		bItalic			= false, bUnderLine = false, bBold = false;
	//	pDC->FillSolidRect(lpRect->left, lpRect->top, (lpRect->right - lpRect->left), (lpRect->bottom - lpRect->top), RGB(122, 122, 0));

		FONTDef hfontOld = pDC->SelectObject(m_fontTemp);
		Font	fontOld;
		if( hfontOld ){
			fontOld.Attach(hfontOld);

			LOGFONT lf;
			fontOld.GetLogFont(&lf);

			bItalic			= (lf.lfItalic != 0);
			bBold			= (lf.lfWeight == FW_BOLD);
			bUnderLine		= (lf.lfUnderline != 0);
			if( lf.lfHeight < 0 )
				nHeight =  -1*MulDiv(lf.lfHeight, 72, pDC->GetDeviceCaps(LOGPIXELSY));
			else
				nHeight = lf.lfHeight;
			pDC->SelectObject(hfontOld);
			fontOld.Detach();
			}
		
		if( nHeight > 0 ){
			EmbedeedFontItemInfo*	pFontItem		= pFont->GetFontItem(nHeight, bBold, bItalic, bUnderLine);
			if( !pFontItem ) return false;
			Image*					pImgSymbolsMap	= &pFontItem->m_imageCharacters;

			Rect rcDrawRect(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);

			bool		bBitBlt		= false;
			Image		dcDibImage;
			Rect		rcDestBitBlt(0, 0, 0, 0);
			if( !pDC->GetSelectedDibImageFromDC(dcDibImage) ){
				Image bmImage;
				if( !pDC->GetDibImageFromDC(24, bmImage, lpRect->left, lpRect->top, (lpRect->right - lpRect->left), (lpRect->bottom - lpRect->top), NULL) ){
					ASSERT(FALSE);
					return false;
					}
	
				bBitBlt			= true;
				dcDibImage.Attach(bmImage.Detach());
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

			Rect	rcSymbolImage, rcSymbolDC, rcInvalid;
			rcSymbolDC.left			= x;
			rcSymbolDC.right		= x;
			rcSymbolDC.top			= y;
			rcSymbolDC.bottom		= rcSymbolDC.top + pImgSymbolsMap->GetHeight();

			COLORREF	crBk		= 0;
			bool		bUseBkColor = false;
			if( pDC->GetBkMode() != TRANSPARENT ){
				crBk		= pDC->GetBkColor();
				bUseBkColor = true;
				}

			// Distance between symbols.
			int nDistanceCX	= GetDistanceBetweenSymbols(), nSpaceSymbolCX	= 4, nTabSymbolCX = 6;
			Image bmSymbol;

			for(int i=0; i<nLen; i++){
				int nSymbolCode = pwszText[i];
				if( nSymbolCode == VK_SPACE ){
					rcSymbolDC.right	= rcSymbolDC.left + nSpaceSymbolCX;
					rcInvalid			= rcSymbolDC & rcDrawRect;
					// Fill background.
					if( bUseBkColor && !rcInvalid.IsRectEmpty() )
						pDC->FillSolidRect(Rect(rcInvalid.left, rcInvalid.top, rcInvalid.left + rcInvalid.Width(), rcInvalid.top + rcInvalid.Height()), crBk);
					rcSymbolDC.left		= rcSymbolDC.right;
					continue;
					}

				if( nSymbolCode == VK_TAB ){
					rcSymbolDC.right	= rcSymbolDC.left + nTabSymbolCX;
					rcInvalid			= rcSymbolDC & rcDrawRect;
					// Fill background.
					if( bUseBkColor && !rcInvalid.IsRectEmpty() )
						pDC->FillSolidRect(Rect(rcInvalid.left, rcInvalid.top, rcInvalid.left + rcInvalid.Width(), rcInvalid.top + rcInvalid.Height()), crBk);
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
						pDC->FillSolidRect(Rect(rcInvalid.left, rcInvalid.top, rcInvalid.left + rcInvalid.Width() + nDistanceCX, rcInvalid.top + rcInvalid.Height()), crBk);

					// Biggest(Widest) image to use once for all symbols.
					if( bmSymbol.IsNull() )
						bmSymbol.CreateDIBBitmap(32, crText, rcSymbolImage.Height(), rcSymbolImage.Height(), NULL);
					
					// Copy alpha channel.
					if( pImgSymbolsMap->CopyAlphaChannel(bmSymbol, rcSymbolImage) ){
						int nDrawCX = rcInvalid.Width(), nDrawCY = rcInvalid.Height();
						if( (rcInvalid.left + nDrawCX) > dcDibImage.GetWidth() )
							nDrawCX = (dcDibImage.GetWidth() - rcInvalid.left);
						if( (rcInvalid.top + nDrawCY) > dcDibImage.GetHeight() )
							nDrawCY = (dcDibImage.GetHeight() - rcInvalid.top);

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
							dcDibImage.BitmapOnBitmap(rcInvalid.left, rcInvalid.top, bmSymbol, nImageSrcX, nImageSrcY, nDrawCX, nDrawCY, nDrawCX, nDrawCY, TRUE);
							}
						}
					}

				rcSymbolDC.left		= rcSymbolDC.right + nDistanceCX;
				rcSymbolDC.right	= rcSymbolDC.left;
				}

			if( bBitBlt ){
				_DC srcDC;
				srcDC.CreateCompatibleDC(NULL);
				srcDC.SelectObject(dcDibImage);
				pDC->BitBlt(rcDestBitBlt.left, rcDestBitBlt.top, rcDestBitBlt.Width(), rcDestBitBlt.Height(), srcDC, 0, 0, SRCCOPY);
				dcDibImage.Destroy();
				srcDC.DeleteDC();
				}
			else
				dcDibImage.Detach();
			bmSymbol.Destroy();
			}
		else
			return false;
		}
	return true;
	}

bool
EmbedeedFontMan::GetTextSize(_DC* pDC, wchar_t* pwszText, int nLen, int nFontId, Size& szText){
	EmbedeedFont* pFont = m_listEmbedeedFonts.GetAt(nFontId);
	if( pFont ){
		int		nSymbolHeight	= 0;
		bool		bItalic		= false, bUnderLine	= false, bBold = false;

		// Distance between symbols.
		int nDistanceCX	= GetDistanceBetweenSymbols(), nSpaceSymbolCX	= 4, nTabSymbolCX = 6;

		FONTDef hfontOld	= pDC->SelectObject(m_fontTemp);
		Font fontOld;
		if( hfontOld ){
			fontOld.Attach(hfontOld);

			LOGFONT lf;
			fontOld.GetLogFont(&lf);
			nSymbolHeight	= abs(lf.lfHeight);
			bItalic			= (lf.lfItalic != 0);
			bBold			= (lf.lfWeight == FW_BOLD);
			bUnderLine		= (lf.lfUnderline != 0);
			if( lf.lfHeight < 0 )
				nSymbolHeight =  -1*MulDiv(lf.lfHeight, 72, pDC->GetDeviceCaps(LOGPIXELSY));
			else
				nSymbolHeight = lf.lfHeight;

			fontOld.Detach();
			pDC->SelectObject(hfontOld);
			}

		if( nSymbolHeight > 0 ){
			EmbedeedFontItemInfo* pFontItem = pFont->GetFontItem(nSymbolHeight, bBold, bItalic, bUnderLine);
			if( !pFontItem ) 
				return false;

			Rect rcSymbolImage;
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
EmbedeedFontMan::DumpSystemFont(const std::string sFontName, const std::string sFontDumpFolder){
	int nFontSizesDump[] = {8,9,10,12,14,16};
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	strcpy_s(lf.lfFaceName, sFontName.c_str());
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

	_DC memDC;
	memDC.CreateCompatibleDC(NULL);

	LOGFONT* ppLogFonts[] = {&lfRegular, &lfRegularUnderline, &lfRegularItalic, &lfBold, &lfBoldUnderline, &lfBoldItalic};
	for(int k=0; k<sizeof(ppLogFonts)/sizeof(LOGFONT*); k++){
		LOGFONT* pLogFont = ppLogFonts[k];
		for(int i=0; i<sizeof(nFontSizesDump)/sizeof(int); i++){
			Font font;
			pLogFont->lfHeight =  -MulDiv(nFontSizesDump[i], memDC.GetDeviceCaps(LOGPIXELSY), 72);
			if( !font.CreateFontIndirect(pLogFont) ){
				return false;
				}
			memDC.SelectObject(font);
		
			AutoSortedArray arrSymbolImages;
			Size szSymbols(0, 0);

			// Create symbol images.
			for(int j=0; j<nSymbolsCt; j++){
				SIZE szSymbol;
				if( !::GetTextExtentPoint32W(memDC, wSymbols[j], 1, &szSymbol) )
					continue;

				Image bmSymbol;
				if( !bmSymbol.CreateDIBBitmap(24, RGB(0, 0, 0), szSymbol.cx, szSymbol.cy, NULL) )
					continue;

				memDC.SelectObject	(bmSymbol);
				memDC.SetTextColor	(RGB(255, 255, 255));
				memDC.SetBkMode		(TRANSPARENT);
				::TextOutW		(memDC, 0, 0, wSymbols[j], 1);

				//ImageHelper::SetSoftenFilter((HBITMAP)bmSymbol.m_hObject);

				arrSymbolImages.Add((void*)j, (void*)bmSymbol.Detach());
				szSymbols.cx += (szSymbol.cx + nDistanceBetweenSymbols);
				szSymbols.cy =	max(szSymbols.cy, szSymbol.cy);
				}

			Image bmSymbolsMap;
			if( !bmSymbolsMap.CreateDIBBitmap(24, RGB(0, 0, 0), szSymbols.cx, szSymbols.cy, NULL) )
				continue;

			memDC.SelectObject(bmSymbolsMap);

			_DC srcDC;
			srcDC.CreateCompatibleDC(memDC);

			// Create symbols map image.
			Point ptDraw(0, 0);
			for(int loop=0; loop<arrSymbolImages.GetCount(); loop++){
				Image bmSymbol;
				if( !bmSymbol.Attach((HBITMAP)arrSymbolImages.GetData(loop)) )
					continue;

				ptDraw.y = (szSymbols.cy - bmSymbol.GetHeight())/2;

				srcDC	.SelectObject(bmSymbol);
				memDC	.BitBlt(ptDraw.x, ptDraw.y, bmSymbol.GetWidth(), bmSymbol.GetHeight(), srcDC, 0, 0, SRCCOPY);
				bmSymbol.Destroy();

				ptDraw.x += (bmSymbol.GetWidth() + nDistanceBetweenSymbols);
				}

			std::string sFileName = sFontDumpFolder;
			if( sFileName[sFileName.length() - 1] != '\\' )
				sFileName += _T("\\");

			std::string sName = sFontName;
			if( pLogFont->lfWeight == FW_BOLD )
				sName += _T("b");
			if( pLogFont->lfUnderline == 1 )
				sName += _T("u");
			if( pLogFont->lfItalic == 1 )
				sName += _T("i");

			std::string sTemp;
			stringEx::Format(sTemp, _T("%s%02d.bmp"), sName.c_str(), nFontSizesDump[i]);
			sFileName += sTemp;
			Path::BuildFullPath(sFileName);

			::DeleteFile(sFileName.c_str());

			Image imgSave;
			imgSave.Attach((HBITMAP)bmSymbolsMap.Detach());
			imgSave.Save(sFileName);

			imgSave.Destroy();
			arrSymbolImages.DeleteAll();
			}
	}
	return false;
	}