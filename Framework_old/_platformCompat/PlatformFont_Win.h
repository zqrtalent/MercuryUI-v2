#if !defined(PLATFORMFONT_H_INCLUDED)
#define      PLATFORMFONT_H_INCLUDED

#include "PlatformCompat.h"
#include "PlatformSize.h"

class Font
{
public:
	Font();
	virtual ~Font();
	operator FONTDef(){return font_;};

	BOOL	Attach					(FONTDef font);
	FONTDef Detach					();
	bool	IsNull					(){ return (font_ == NULL); };
	BOOL	DeleteObject			();

	BOOL	CopyFont				(Font* pCopyInto);

	BOOL	CreateFont				(const TCHAR* pszFontName, int nHeight);

	int		GetLogFont				(LOGFONT* pLogFont);
	BOOL	CreateFontIndirect		(LOGFONT* pLogFont);

	BOOL	GetTextSize				(std::string* pStr, Size& szText);
	int		GetSymbolIndexByXOffset	(std::string* pStr, int nXTextStart, int nXSymbolAt);
	int		GetSymbolWidthArray		(std::string* pStr, CDWordArray& arrSymbolWidth);

	// Creates scaled font object.
	BOOL	StretchedTextFont		(Font* pFontOut, float fStretchCX, float fStretchCY, bool bUnderLine = false, bool bBold = false);

	bool	IsEmbedeedFontAvailable	();

protected:
	FONTDef	font_;
};

#endif // !defined(PLATFORMFONT_H_INCLUDED)

