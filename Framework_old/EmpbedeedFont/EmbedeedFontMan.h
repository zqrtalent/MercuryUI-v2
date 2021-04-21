// EmbedeedFontMan.h header file.
//

#pragma once
#include "../_platformCompat/PlatformDeviceContext.h"
#include "../Array/List.h"
#include "../Array/StringToIntArray.h"
#include "EmbedeedFont.h"

class EmbedeedFontMan
{
protected:
	EmbedeedFontMan();
public:
	virtual ~EmbedeedFontMan();

	static EmbedeedFontMan* GetInstance(){
		static EmbedeedFontMan instance;
		return &instance;
		}

	static int GetDistanceBetweenSymbols(){return 1;}

	bool	AddFont					(const _string sFontFolder, const _string sFontName);
	int		GetEmbedeedFontId		(_string sFontName);
	bool	DrawTextByEmbedeedFont	(int nEmbedeedFontId, _DC* pDC, const wchar_t* pwszText, int nLen, int x, int y, const RECTDef* lpRect);
	bool	GetTextSize				(_DC* pDC, wchar_t* pwszText, int nLen, int nFontId, _Size& szText);

	bool	DumpSystemFont			(const _string sFontName, const _string sFontDumpFolder);

protected:
	List<EmbedeedFont>	m_listEmbedeedFonts;
	StringToIntArray	m_arrFontNameById;		// Embedeed font name by id.
	Font				m_fontTemp;				// Temporary font object to retrieve font object from DC.
};