// EmbedeedFont.h header file.
//

#pragma once
#include "../Array/AutoSortedArray.h"
#include "../_platformCompat/PlatformImage.h"
#include "../Serialize/Serializable.h"

class EmbedeedFontItemInfo : public Serializable
{
public:
	EmbedeedFontItemInfo();
	virtual ~EmbedeedFontItemInfo();

	friend class EmbedeedFontMan;
		friend class EmbedeedFont;

	enum {
		EMBEDEED_FONT_FLAG_BOLD = 1,
		EMBEDEED_FONT_FLAG_UNDERLINE = 2,
		EMBEDEED_FONT_FLAG_ITALIC = 4
		};

protected:
	int				m_nFontVal;			// Font value.
	Image			m_imageCharacters;	// Characters map image.
	_Point*			m_pCharPoints;		// x is left offset, y is width of character.
	int				m_nCharCount;		// Count of characters.
	int				m_nCharFlags;		// Flags italic, bold or underline

public:
	virtual Serializable*	CreateSerializableObject	(){return new EmbedeedFontItemInfo();};
protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class EmbedeedFont : public Serializable
{
public:
	EmbedeedFont();
	virtual ~EmbedeedFont();

	friend class EmbedeedFontMan;

protected:
	EmbedeedFontItemInfo*	GetFontItem					(int nFontHeight, bool bBold, bool bItalic, bool bUnderLine);
	bool					GetSymbolRectFromImage		(EmbedeedFontItemInfo* pItem, int nSymbolCode, _Rect& rcSymbol);

protected:
	AutoSortedArray							m_arrCharacters;	// <character code><index in map>
	std::string								m_sFontName;
	int										m_nFontId;
	EnumerableObject<EmbedeedFontItemInfo>	m_listItems;

private:
	// <[font height][font item flags(bold, italic, underline)]><EmbedeedFontItemInfo*>
	AutoSortedArray							m_arrFontItemInfoByHeight;
	bool									m_bUpdateItemInfoArray;

public:
	virtual Serializable*	CreateSerializableObject	(){return new EmbedeedFont();};
protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};