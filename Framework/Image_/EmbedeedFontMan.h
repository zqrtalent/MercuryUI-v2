// EmbedeedFontMan.h header file.
//

#pragma once
#include "..\Array\List.h"
#include "..\Array\StringToIntArray.h"
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

	bool	AddFont					(const CString sFontFolder, const CString sFontName);
	int		GetEmbedeedFontId		(const CString sFontName);
	bool	DrawTextByEmbedeedFont	(int nEmbedeedFontId, CDC* pDC, wchar_t* pwszText, int nLen, int x, int y, const RECT* lpRect);
	bool	GetTextSize				(CDC* pDC, wchar_t* pwszText, int nLen, int nFontId, CSize& szText);

	bool	DumpSystemFont			(const CString sFontName, const CString sFontDumpFolder);

protected:
	List<EmbedeedFont>	m_listEmbedeedFonts;
	StringToIntArray	m_arrFontNameById;		// Embedeed font name by id.
	CFont				m_fontTemp;				// Temporary font object to retrieve font object from DC.
};