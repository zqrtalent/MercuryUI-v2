#if !defined(PLATFORMFONT_H_INCLUDED)
#define      PLATFORMFONT_H_INCLUDED

#include "PlatformCompat.h"
#include "PlatformSize_Win.h"

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

	BOOL	GetTextSize				(_String* pStr, _Size& szText);
	int		GetSymbolIndexByXOffset	(_String* pStr, int nXTextStart, int nXSymbolAt);
	int		GetSymbolWidthArray		(_String* pStr, CDWordVector& arrSymbolWidth);

	// Creates scaled font object.
	BOOL	StretchedTextFont		(Font* pFontOut, float fStretchCX, float fStretchCY, bool bUnderLine = false, bool bBold = false);

	bool	IsEmbedeedFontAvailable	();

protected:
	FONTDef	font_;
};

#endif // !defined(PLATFORMFONT_H_INCLUDED)

