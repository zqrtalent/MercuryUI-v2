// XMLReader.cpp source file.
//
//	ESCL (Elite Soft Class Library)
//	
//	Author: ZqrTalent
//

#include "stdafx.h"
#include "XMLReader.h"
#include "Markup.h"
#include "..\Path\Path.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Constructor / Destructor
XMLReader::XMLReader()
	{
	m_pFirstTag = NULL;
	}

XMLReader::~XMLReader()
	{
	Destroy();
	}

// Public methods

BOOL
XMLReader::Load(const CString sXmlFile)
	{
	CMarkup xmlReader;
	if( !xmlReader.Load(sXmlFile) ){
		return FALSE;
		}

	m_pFirstTag = Recurs_Load(&xmlReader, NULL);
	return 1;
	}

void
XMLReader::Destroy()
	{
	if( m_pFirstTag ){
		Recurs_Destroy(m_pFirstTag);
		m_pFirstTag = NULL;
		}
	}

// Protected methods

void
XMLReader::Recurs_Destroy(XmlTag* pTag)
	{
	while( pTag ){
		if( pTag->child )
			Recurs_Destroy(pTag->child);
		XmlTag* pTemp = pTag->next;
		delete pTag;
		pTag = pTemp;
		}
	}

XmlTag*
XMLReader::Recurs_Load(CMarkup* pXML, XmlTag* pParentTag)
	{
	XmlTag* pTagFirst	= NULL;
	XmlTag* pTagPrev	= NULL;
	CString sTagName;

	while( pXML->FindElem() ){
		XmlTag* pTagNew		= new XmlTag;
		// Check tag name length. ##############
		sTagName			= pXML->GetTagName();
		if( sTagName.GetLength() >= sizeof(pTagNew->strTagName) ){
			sTagName = sTagName.Left(sizeof(pTagNew->strTagName) - 1);
			}
		// #####################################
		pTagNew->parent		= pParentTag;
		pTagNew->child		= NULL;
		pTagNew->next		= NULL;
		pTagNew->prev		= NULL;
		pTagNew->nTagType	= XMLTAG_TYPE_KEY;
		_tcscpy_s(pTagNew->strTagName, sTagName.GetBuffer());

		BOOL bContinue		= TRUE;
		if( pXML->IntoElem() ){
			if( !Recurs_Load(pXML, pTagNew) )
				bContinue	= FALSE;
			pXML->OutOfElem	();
			}

		if( !pTagFirst )
			pTagFirst = pTagNew;
		else
			pTagPrev->next = pTagNew;
		pTagNew->prev	= pTagPrev;
		pTagPrev		= pTagNew;

		if( bContinue )		  continue;	
		CString	sFaceName;
		CString	sData		= pXML->GetData();
		sData.TrimLeft();
		sData.TrimRight();
		if( sData.IsEmpty() ) continue;

		if( sData[0] == '\x22' && sData[sData.GetLength()-1] == '\x22' ){
			// Delete first and last symbol.
			sData.Delete(0);
			sData.Delete(sData.GetLength()-1);
			// #############################

			if( sData.GetLength() > 254 )
				sData = sData.Left(254);

			pTagNew->nTagType	= XMLTAG_TYPE_STRING;
			_tcscpy_s(pTagNew->val.str, sData.GetBuffer());
			continue;
			}

		if( sData[0] > '9' ) // Started with alphabetical symbol.
			{
			if( sData.Left(4) == _T("DTTM") ) // Date time value.
				{
				pTagNew->nTagType = XMLTAG_TYPE_DATETIME;
				COleDateTime dt;
				if( dt.ParseDateTime(CString(&sData.GetBuffer()[4])) )
					pTagNew->val.dt = dt.m_dt;
				else{
					dt = COleDateTime::GetCurrentTime();
					pTagNew->val.dt = dt.m_dt;
					}
				continue;
				}
			else
			if( sData.Left(2) == _T("DT") ) // Date value
				{
				pTagNew->nTagType	= XMLTAG_TYPE_DATE;
				COleDateTime dt;
				if( dt.ParseDateTime(CString(&sData.GetBuffer()[2]), VAR_DATEVALUEONLY) )
					pTagNew->val.dt = dt.m_dt;
				else{
					dt = COleDateTime::GetCurrentTime();
					pTagNew->val.dt = dt.m_dt;
					}
				continue;
				}
			else
			if( sData.Left(2) == _T("TM") ) // Time value
				{
				pTagNew->nTagType = XMLTAG_TYPE_TIME;
				COleDateTime dt;
				if( dt.ParseDateTime(CString(&sData.GetBuffer()[2]), VAR_TIMEVALUEONLY) )
					pTagNew->val.dt = dt.m_dt;
				else{
					dt = COleDateTime::GetCurrentTime();
					pTagNew->val.dt = dt.m_dt;
					}
				continue;
				}			
			}

		int		nStrIdx[64];
		int		nCommaCt	= 0;
		int		nFind		= sData.Find(',');

		while( nFind != -1 ){
			nStrIdx[nCommaCt] = nFind + 1;
			nFind = sData.Find(',', nFind + 1); nCommaCt ++;
			}

		switch( nCommaCt )
			{
			case 0: // Numeric
				{
				sData.MakeLower();
				if( sData.Find('.') != -1 ){
					pTagNew->nTagType	= XMLTAG_TYPE_DOUBLE;
					pTagNew->val.db		= _tstof(sData.GetBuffer());
					}
				else{
					pTagNew->nTagType	= XMLTAG_TYPE_NUMERIC;
					pTagNew->val.num	= _tstoi(sData.GetBuffer());

					// BOOL type check. ####
					if( !pTagNew->val.num ){
						if( sData == "true" ){
							pTagNew->nTagType	= XMLTAG_TYPE_BOOL;
							pTagNew->val.num	= 1;
							}
						else
						if( sData == "false" ){
							pTagNew->nTagType	= XMLTAG_TYPE_BOOL;
							pTagNew->val.num	= 0;
							}
						}
					// ####################
					}
				break;
				}
			case 1: // Point / Size
				{
				pTagNew->nTagType	= XMLTAG_TYPE_POINT;
				pTagNew->val.pt.x   = _tstoi(&((TCHAR*)sData.GetBuffer())[0]);
				pTagNew->val.pt.y   = _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[0]]);
				break;
				}
			case 2: // Color
				{
				pTagNew->nTagType	= XMLTAG_TYPE_COLOR;
				pTagNew->val.cr		= RGB	(_tstoi(&((TCHAR*)sData.GetBuffer())[0]), 
											_tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[0]]),
											_tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[1]]));
				break;
				}
			case 3: // Rect
				{
				pTagNew->nTagType			= XMLTAG_TYPE_RECT;
				pTagNew->val.rect.left		= _tstoi(&((TCHAR*)sData.GetBuffer())[0]);
				pTagNew->val.rect.top		= _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[0]]);
				pTagNew->val.rect.right		= _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[1]]);
				pTagNew->val.rect.bottom	= _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[2]]);
				break;
				}
			case 6: // LOGFONT
				{
				pTagNew->nTagType			= XMLTAG_TYPE_LOGFONT;
				memset(&pTagNew->val.lf, 0, sizeof(LOGFONT));

				pTagNew->val.lf.lfHeight	= _tstoi(&((TCHAR*)sData.GetBuffer())[0]);
				sFaceName					= sData.Mid(nStrIdx[0], nStrIdx[1] - nStrIdx[0] - 1);
				sFaceName.TrimLeft	();
				sFaceName.TrimRight	();
				_tcscpy_s(pTagNew->val.lf.lfFaceName, sFaceName.GetBuffer());
				pTagNew->val.lf.lfWeight	= _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[1]]);
				pTagNew->val.lf.lfCharSet	= _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[2]]);
				pTagNew->val.lf.lfItalic	= _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[3]]);
				pTagNew->val.lf.lfUnderline = _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[4]]);
				pTagNew->val.lf.lfStrikeOut = _tstoi(&((TCHAR*)sData.GetBuffer())[nStrIdx[5]]);
				pTagNew->val.lf.lfQuality	= PROOF_QUALITY;
				pTagNew->val.lf.lfPitchAndFamily = DEFAULT_PITCH;
				break;
				}
			default:
				pTagNew->nTagType	= XMLTAG_TYPE_KEY;
			};
		}

	if( pParentTag )
		pParentTag->child = pTagFirst;
	return pTagFirst;
	}

XmlTag*
XMLReader::Recurs_FindTag(LPTSTR lpszTagPath, XmlTag* pTag)
	{
	if( !pTag ) return NULL;
	CString sTagName;
	LPTSTR	szFind	= _tcschr(lpszTagPath, '/');
	INT_PTR	nFind	= szFind ? szFind - lpszTagPath : -1;
	if( nFind == -1 ){
		sTagName = lpszTagPath;
		}
	else{
		TCHAR cTemp			= lpszTagPath[nFind];
		lpszTagPath[nFind]	= '\0';
		sTagName			= lpszTagPath;
		lpszTagPath[nFind]	= cTemp;
		}

	while( pTag ){
		// Walk through tags.
		if( !_tcsicmp(pTag->strTagName, sTagName.GetBuffer()) ){
			// When tag founded.
			if( nFind == -1 )
				return pTag;
			else{
				XmlTag* pTagFound = Recurs_FindTag(&lpszTagPath[nFind + 1], pTag->child);
				if( pTagFound ) return pTagFound;
				}
			}
		pTag = pTag->next;
		}

	return NULL;
	}

XmlTag* 
XMLReader::FindTag(LPTSTR lpszTagPath)
	{
	CString sPath = lpszTagPath;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), m_pFirstTag);
	return pTag;
	}

BOOL		
XMLReader::GetBool(LPTSTR lpszPathToValue, BOOL bDefault /*= FALSE*/, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);
	if( !pTag || (pTag->nTagType != XMLTAG_TYPE_BOOL && pTag->nTagType != XMLTAG_TYPE_NUMERIC) ) {
		ASSERT(FALSE);
		return FALSE;
		}
	return (BOOL)pTag->val.num;
	}

long		
XMLReader::GetLong(LPTSTR lpszPathToValue, long lDefault /*= 0*/, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);
	if( !pTag || (pTag->nTagType != XMLTAG_TYPE_BOOL && pTag->nTagType != XMLTAG_TYPE_NUMERIC) ) {
		ASSERT(FALSE);
		return 0;
		}
	return pTag->val.num;
	}

COLORREF	
XMLReader::GetColor(LPTSTR lpszPathToValue, COLORREF crDefault /*= 0L*/, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_COLOR ) {
		ASSERT(FALSE);
		return 0;
		}
	return pTag->val.cr;
	}

double		
XMLReader::GetDouble(LPTSTR lpszPathToValue, double dDefault /*= 0.0*/, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_DOUBLE ) {
		ASSERT(FALSE);
		return 0.0;
		}
	return pTag->val.db;
	}

BOOL		
XMLReader::GetSize(LPTSTR lpszPathToValue, CSize& szValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);	
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_POINT ) {
		return FALSE;
		}

	szValue.cx = pTag->val.pt.x;
	szValue.cy = pTag->val.pt.y;
	return 1;
	}

BOOL
XMLReader::GetPoint(LPTSTR lpszPathToValue, CPoint& ptValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);	
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_POINT ) {
		return FALSE;
		}

	ptValue.x = pTag->val.pt.x;
	ptValue.y = pTag->val.pt.y;
	return 1;
	}

BOOL		
XMLReader::GetRect(LPTSTR lpszPathToValue, CRect& rcValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);	
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_RECT ) {
		return FALSE;
		}
	rcValue = pTag->val.rect;
	return 1;
	}

CString		
XMLReader::GetString(LPTSTR lpszPathToValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);	
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_STRING ) {
		return FALSE;
		}
	return pTag->val.str;
	}

BOOL		
XMLReader::GetDate(LPTSTR lpszPathToValue, COleDateTime& dtValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);	
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_DATE ) {
		return FALSE;
		}

	dtValue = pTag->val.dt;
	return TRUE;
	}

BOOL		
XMLReader::GetTime(LPTSTR lpszPathToValue, COleDateTime& dtValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);	
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_TIME ) {
		return FALSE;
		}

	dtValue = pTag->val.dt;
	return TRUE;
	}

BOOL		
XMLReader::GetDateTime(LPTSTR lpszPathToValue, COleDateTime& dtValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);	
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_DATETIME ) {
		return FALSE;
		}

	dtValue = pTag->val.dt;
	return TRUE;
	}

BOOL		
XMLReader::GetLogfont(LPTSTR lpszPathToValue, LOGFONT& lfValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = lpszPathToValue;
	XmlTag* pTag  = Recurs_FindTag(sPath.GetBuffer(), pTagStart?pTagStart:m_pFirstTag);
	if( !pTag || pTag->nTagType != XMLTAG_TYPE_LOGFONT ) {
		ASSERT(FALSE);
		return FALSE;
		}
	memcpy(&lfValue, &pTag->val.lf, sizeof(LOGFONT));
	return 1;
	}

CString
XMLReader::GetFilePath(LPTSTR lpszPathToValue, XmlTag* pTagStart /*= NULL*/)
	{
	CString sPath = GetString(lpszPathToValue, pTagStart);
	Path::BuildFullPath(sPath);
	return sPath;
	}

int
XMLReader::GetStringArr(LPTSTR lpszPathToValue, CStringArray& arr, TCHAR delimiter /*= ';'*/, XmlTag* pTagStart /*= NULL*/)
	{
	CString sStrings	= GetString(lpszPathToValue, pTagStart);
	// Split string to pieces.
	int		nLen		= sStrings.GetLength();
	TCHAR*	pszValue	= sStrings.GetBuffer();
	int		nLoop		= 0;
	sStrings.Replace(delimiter, '\0');

	while( nLoop < nLen ){
		int nLen1 = (int)_tcsclen(&pszValue[nLoop]);
		if( nLen1 > 0 )
			arr.Add(&pszValue[nLoop]);
		nLoop += nLen1 + 1;
		}
	// ########################
	return (int)arr.GetCount();
	}

XmlTag* 
XMLReader::GetStartTag()
	{
	return m_pFirstTag;
	}

void
XMLReader::SetStartTag(XmlTag* pTag)
	{
	ASSERT(pTag);
	if( !pTag ) return;
	m_pFirstTag = pTag;
	}

