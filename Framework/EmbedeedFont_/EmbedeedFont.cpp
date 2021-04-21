// EmbedeedFont.cpp source file.
//
#include "stdafx.h"
#include "EmbedeedFont.h"

//////////////////////////////////////////////////////
//	class EmbedeedFont
//
EmbedeedFont::EmbedeedFont(){
	m_bUpdateItemInfoArray = true;
	}

BEGIN_DECLARE_VAR(EmbedeedFont, Serializable)
	DECLARE_VAR(_T("EmbedeedFont"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("FontName"),				_T(""), VariableType::VT_String, offsetof(EmbedeedFont,m_sFontName), false, false, false)
	DECLARE_VAR(_T("FontId"),				_T(""), VariableType::VT_Int32, offsetof(EmbedeedFont,m_nFontId), false, false, false)
	DECLARE_VAR(_T("ListItems"),			_T(""), VariableType::VT_Array, offsetof(EmbedeedFont,m_listItems), false, true, true)
END_DECLARE_VAR()

EmbedeedFont::~EmbedeedFont(){
	}

EmbedeedFontItemInfo*
EmbedeedFont::GetFontItem(int nFontHeight, bool bBold, bool bItalic, bool bUnderLine){
	if( m_bUpdateItemInfoArray ){
		m_arrFontItemInfoByHeight.DeleteAll();
		for(int i=0; i<m_listItems.GetCount(); i++){
			EmbedeedFontItemInfo* pItem = m_listItems.GetAt(i);
			if( pItem ){
				int nKey = pItem->m_nFontVal;
				nKey		<<= 16;
				nKey		|= (pItem->m_nCharFlags&0xFFFF);
				m_arrFontItemInfoByHeight.Add((void*)nKey, (void*)pItem);
				}
			}
		m_bUpdateItemInfoArray = false;
		}

	if( !m_arrFontItemInfoByHeight.GetCount() )
		return NULL;

	bool	bExists = false;
	int		nIndex	= m_arrFontItemInfoByHeight.FindValuePlace((void*)(nFontHeight << 16), bExists);
	if( nIndex == m_arrFontItemInfoByHeight.GetCount() )
		nIndex = m_arrFontItemInfoByHeight.GetCount() - 1;
	else{
		int nFlags = 0;
		if( bBold )
			nFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_BOLD;
		if( bUnderLine )
			nFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_UNDERLINE;
		if( bItalic )
			nFlags |= EmbedeedFontItemInfo::EMBEDEED_FONT_FLAG_ITALIC;

		int nLoop = nIndex, nCt = m_arrFontItemInfoByHeight.GetCount();
		int nFontHeightFound = (((int)m_arrFontItemInfoByHeight.Get(nIndex)) >> 16);
		while(nLoop < nCt){
			int nKey = (int)m_arrFontItemInfoByHeight[nLoop];
			if( (nKey>>16) != nFontHeightFound )
				break; 
			// Found font with flag settings.
			if( (nKey&0xFFFF) == nFlags ){
				nIndex = nLoop;
				break;
				}
			nLoop ++;
			}
		}
	return (EmbedeedFontItemInfo*)m_arrFontItemInfoByHeight.GetData(nIndex);
	}

bool
EmbedeedFont::GetSymbolRectFromImage(EmbedeedFontItemInfo* pItem, int nSymbolCode, _Rect& rcSymbol){
	int nIndex = m_arrCharacters.IndexOf((void*)nSymbolCode);
	if( nIndex == -1 )
		return false;

	// <Symbol Code><Symbol index>
	nIndex = (int)m_arrCharacters.GetData(nIndex);

	_Point pt = pItem->m_pCharPoints[nIndex];
	rcSymbol.left	= pt.x;
	rcSymbol.top	= 0;
	rcSymbol.right	= rcSymbol.left + pt.y;
	rcSymbol.bottom = pItem->m_imageCharacters.GetHeight();
	return true;
	}

//////////////////////////////////////////////////////
//	class EmbedeedFontItemInfo
//

EmbedeedFontItemInfo::EmbedeedFontItemInfo(){
	m_pCharPoints = NULL;
	}

BEGIN_DECLARE_VAR(EmbedeedFontItemInfo, Serializable)
	DECLARE_VAR(_T("EmbedeedFontItemInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("FontVal"),				_T(""), VariableType::VT_Int32, offsetof(EmbedeedFontItemInfo,m_nFontVal), false, false, false)
	DECLARE_VAR(_T("imageCharacters"),		_T(""), VariableType::VT_Image, offsetof(EmbedeedFontItemInfo,m_imageCharacters), false, false, false)
	DECLARE_VAR(_T("CharCount"),			_T(""), VariableType::VT_Int32, offsetof(EmbedeedFontItemInfo,m_nCharCount), false, false, false)
	DECLARE_VAR(_T("CharFlags"),			_T(""), VariableType::VT_Int32, offsetof(EmbedeedFontItemInfo,m_nCharFlags), false, false, false)
END_DECLARE_VAR()

EmbedeedFontItemInfo::~EmbedeedFontItemInfo(){
	if( m_pCharPoints ){
		delete [] m_pCharPoints;
		m_pCharPoints = NULL;
		}
	}