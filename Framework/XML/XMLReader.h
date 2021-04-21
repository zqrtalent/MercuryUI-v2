// UIXml.h header file.
//
//	ESCL (Elite Soft Class Library)
//	
//	Author: ZqrTalent
//

#pragma once

#define XMLTAG_TYPE_UNKNOWN 0
#define XMLTAG_TYPE_KEY 1
#define XMLTAG_TYPE_DATE 2
#define XMLTAG_TYPE_TIME 3
#define XMLTAG_TYPE_DATETIME 4
#define XMLTAG_TYPE_NUMERIC 5
#define XMLTAG_TYPE_DOUBLE 6
#define XMLTAG_TYPE_BOOL 7
#define XMLTAG_TYPE_POINT 8
#define XMLTAG_TYPE_RECT 9
#define XMLTAG_TYPE_LOGFONT 10
#define XMLTAG_TYPE_COLOR 11
#define XMLTAG_TYPE_STRING 12

typedef struct _XmlTag
	{
	_XmlTag* parent;
	_XmlTag* child;
	_XmlTag* next;
	_XmlTag* prev;
	TCHAR	strTagName[128];

	union Value{
		DATE	dt;
		TCHAR	str[255];
		long	num;
		POINT	pt;
		RECT	rect;
		double	db;
		LOGFONT	lf;
		COLORREF cr;
		};

	UINT	nTagType;
	Value	val;
	} XmlTag, *PXmlTag;

class CMarkup;
class XMLReader
{
public:
	// Constructor / Destructor
	XMLReader();
	virtual ~XMLReader();

	// Public attributes
public:

	// Public methods
public:

	BOOL	Load		(const CString sXmlFile);
	void	Destroy		();
	XmlTag* FindTag		(LPTSTR lpszTagPath);
	XmlTag* GetStartTag	();
	void	SetStartTag	(XmlTag* pTag);

	// Methods to retrieve values from XML.
	// lpszPathToValue is string value and includes full path to value just like below.
	// Path string isn't case sensitive.
	// UI/UIView/bkcolor

	BOOL		GetBool		(LPTSTR lpszPathToValue, BOOL bDefault = FALSE, XmlTag* pTagStart = NULL);
	long		GetLong		(LPTSTR lpszPathToValue, long lDefault = 0, XmlTag* pTagStart = NULL);
	COLORREF	GetColor	(LPTSTR lpszPathToValue, COLORREF crDefault = 0L, XmlTag* pTagStart = NULL);
	double		GetDouble	(LPTSTR lpszPathToValue, double dDefault = 0.0, XmlTag* pTagStart = NULL);
	BOOL		GetSize		(LPTSTR lpszPathToValue, CSize& szValue, XmlTag* pTagStart = NULL);
	BOOL		GetPoint	(LPTSTR lpszPathToValue, CPoint& ptValue, XmlTag* pTagStart = NULL);
	BOOL		GetRect		(LPTSTR lpszPathToValue, CRect& rcValue, XmlTag* pTagStart = NULL);
	CString		GetString	(LPTSTR lpszPathToValue, XmlTag* pTagStart = NULL);
	BOOL		GetDate		(LPTSTR lpszPathToValue, COleDateTime& dtValue, XmlTag* pTagStart = NULL);
	BOOL		GetTime		(LPTSTR lpszPathToValue, COleDateTime& dtValue, XmlTag* pTagStart = NULL);
	BOOL		GetDateTime	(LPTSTR lpszPathToValue, COleDateTime& dtValue, XmlTag* pTagStart = NULL);
	BOOL		GetLogfont	(LPTSTR lpszPathToValue, LOGFONT& lfValue, XmlTag* pTagStart = NULL);
	CString		GetFilePath	(LPTSTR lpszPathToValue, XmlTag* pTagStart = NULL);
	int			GetStringArr(LPTSTR lpszPathToValue, CStringArray& arr, TCHAR delimiter = ';', XmlTag* pTagStart = NULL);


	// Protected attributes
protected:
	XmlTag*	m_pFirstTag;

	// Protected methods
protected:
	void	Recurs_Destroy	(XmlTag* pTag);
	XmlTag*	Recurs_Load		(CMarkup* pXML, XmlTag* pParentTag);
	XmlTag* Recurs_FindTag	(LPTSTR lpszTagPath, XmlTag* pTag);

};