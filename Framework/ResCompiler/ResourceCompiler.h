#pragma once
#include "..\Array\AutoSortedArray.h"

#ifndef DefineStruct_def 
typedef struct DefineStruct_def{
	CString sDefineName;
	UINT	nDefineValue;
	} DefineStruct;
#endif

const DefineStruct g_defines[] = 
{
"IDOK", 1, "IDCANCEL", 2,
"IDABORT", 3, "IDRETRY", 4,
"IDIGNORE", 5, "IDYES", 6,
"IDNO", 7, "IDCLOSE", 8, "IDHELP", 9,

"WS_OVERLAPPED",			0x00000000L,	"WS_POPUP",				0x80000000L,
"WS_CHILD",					0x40000000L,	"WS_MINIMIZE",			0x20000000L,
"WS_VISIBLE",				0x10000000L,	"WS_DISABLED",			0x08000000L,
"WS_CLIPSIBLINGS",			0x04000000L,	"WS_CLIPCHILDREN",		0x02000000L,
"WS_MAXIMIZE",				0x01000000L,	"WS_CAPTION",			0x00C00000L,
"WS_BORDER",				0x00800000L,	"WS_DLGFRAME",			0x00400000L,
"WS_VSCROLL",				0x00200000L,	"WS_HSCROLL",			0x00100000L,
"WS_SYSMENU",				0x00080000L,	"WS_THICKFRAME",		0x00040000L,
"WS_GROUP",					0x00020000L,	"WS_TABSTOP",			0x00010000L,
"WS_MINIMIZEBOX",			0x00020000L,	"WS_MAXIMIZEBOX",		0x00010000L,
"WS_TILED",					WS_OVERLAPPED,	"WS_ICONIC",			WS_MINIMIZE,
"WS_SIZEBOX",				WS_THICKFRAME,	"WS_TILEDWINDOW",		WS_OVERLAPPEDWINDOW,
"WS_OVERLAPPEDWINDOW",		WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX, 
"WS_POPUPWINDOW",			WS_POPUP|WS_BORDER|WS_SYSMENU, 
"WS_CHILDWINDOW",			WS_CHILD,	"WS_EX_DLGMODALFRAME",		0x00000001L,
"WS_EX_NOPARENTNOTIFY",		0x00000004L, "WS_EX_TOPMOST",			0x00000008L,
"WS_EX_ACCEPTFILES",		0x00000010L, "WS_EX_TRANSPARENT",		0x00000020L,
"WS_EX_MDICHILD",			0x00000040L, "WS_EX_TOOLWINDOW",		0x00000080L,
"WS_EX_WINDOWEDGE",			0x00000100L, "WS_EX_CLIENTEDGE",		0x00000200L,
"WS_EX_CONTEXTHELP",		0x00000400L, "WS_EX_RIGHT",				0x00001000L,
"WS_EX_LEFT",				0x00000000L, "WS_EX_RTLREADING",		0x00002000L,
"WS_EX_LTRREADING",			0x00000000L, "WS_EX_LEFTSCROLLBAR",		0x00004000L,
"WS_EX_RIGHTSCROLLBAR",		0x00000000L, "WS_EX_CONTROLPARENT",		0x00010000L,
"WS_EX_STATICEDGE",			0x00020000L, "WS_EX_APPWINDOW",			0x00040000L,
"WS_EX_OVERLAPPEDWINDOW",	WS_EX_WINDOWEDGE|WS_EX_CLIENTEDGE, 
"WS_EX_PALETTEWINDOW",		WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW|WS_EX_TOPMOST,
"WS_EX_LAYERED",			0x00080000, "WS_EX_NOINHERITLAYOUT",	0x00100000L,
"WS_EX_LAYOUTRTL",			0x00400000L, "WS_EX_COMPOSITED",		0x02000000L,
"WS_EX_NOACTIVATE",			0x08000000L,
"BS_PUSHBUTTON",			0x00000000L, "BS_DEFPUSHBUTTON",	0x00000001L,
"BS_CHECKBOX",				0x00000002L, "BS_AUTOCHECKBOX",		0x00000003L,
"BS_RADIOBUTTON",			0x00000004L, "BS_3STATE",			0x00000005L,
"BS_AUTO3STATE",			0x00000006L, "BS_GROUPBOX",			0x00000007L,
"BS_USERBUTTON",			0x00000008L, "BS_AUTORADIOBUTTON",	0x00000009L,
"BS_PUSHBOX",				0x0000000AL, "BS_OWNERDRAW",		0x0000000BL,
"BS_TYPEMASK",				0x0000000FL, "BS_LEFTTEXT",			0x00000020L,

"BS_TEXT",					0x00000000L, "BS_BITMAP",			0x00000080L,
"BS_ICON",					0x00000040L, "BS_LEFT",				0x00000100L,
"BS_RIGHT",					0x00000200L, "BS_CENTER",			0x00000300L,
"BS_TOP",					0x00000400L, "BS_BOTTOM",			0x00000800L,
"BS_VCENTER",				0x00000C00L, "BS_PUSHLIKE",			0x00001000L,
"BS_MULTILINE",				0x00002000L, "BS_NOTIFY",			0x00004000L,
"BS_FLAT",					0x00008000L, "BS_RIGHTBUTTON",		BS_LEFTTEXT,

"DS_ABSALIGN",         0x01L,
"DS_SYSMODAL",         0x02L,
"DS_LOCALEDIT",        0x20L,
"DS_SETFONT",          0x40L,
"DS_MODALFRAME",       0x80L,
"DS_NOIDLEMSG",        0x100L,
"DS_SETFOREGROUND",    0x200L,
"DS_3DLOOK",           0x0004L,
"DS_FIXEDSYS",         0x0008L,
"DS_NOFAILCREATE",     0x0010L,
"DS_CONTROL",          0x0400L,
"DS_CENTER",           0x0800L,
"DS_CENTERMOUSE",      0x1000L,
"DS_CONTEXTHELP",      0x2000L,
"DS_SHELLFONT",        (DS_SETFONT | DS_FIXEDSYS),

"ES_LEFT",					0x0000L,	"ES_CENTER",			0x0001L,
"ES_RIGHT",					0x0002L,	"ES_MULTILINE",			0x0004L,
"ES_UPPERCASE",				0x0008L,	"ES_LOWERCASE",			0x0010L,
"ES_PASSWORD",				0x0020L,	"ES_AUTOVSCROLL",		0x0040L,
"ES_AUTOHSCROLL",			0x0080L,	"ES_NOHIDESEL",			0x0100L,
"ES_OEMCONVERT",			0x0400L,	"ES_READONLY",			0x0800L,
"ES_WANTRETURN",			0x1000L,	"ES_NUMBER",			0x2000L,

"CBS_SIMPLE",				0x0001L,	"CBS_DROPDOWN",			0x0002L,
"CBS_DROPDOWNLIST",			0x0003L,	"CBS_OWNERDRAWFIXED",	0x0010L,
"CBS_OWNERDRAWVARIABLE",	0x0020L,	"CBS_AUTOHSCROLL",		0x0040L,
"CBS_OEMCONVERT",			0x0080L,	"CBS_SORT",				0x0100L,
"CBS_HASSTRINGS",			0x0200L,	"CBS_NOINTEGRALHEIGHT",	0x2000L,
"CBS_DISABLENOSCROLL",		0x0800L,	"CBS_UPPERCASE",		0x2000L,
"CBS_LOWERCASE",			0x4000L,

"LBS_NOTIFY",				0x0001L,	"LBS_SORT",				0x0002L,
"LBS_NOREDRAW",				0x0004L,	"LBS_MULTIPLESEL",		0x0008L,
"LBS_OWNERDRAWFIXED",		0x0010L,	"LBS_OWNERDRAWVARIABLE", 0x0020L,
"LBS_HASSTRINGS",			0x0040L,	"LBS_USETABSTOPS",		0x0080L,
"LBS_NOINTEGRALHEIGHT",		0x0100L,	"LBS_MULTICOLUMN",		0x0200L,
"LBS_WANTKEYBOARDINPUT",	0x0400L,	"LBS_EXTENDEDSEL",		0x0800L,
"LBS_DISABLENOSCROLL",		0x1000L,	"LBS_NODATA",			0x2000L,
"LBS_NOSEL",				0x4000L, "LBS_COMBOBOX", 0x8000L,
"LBS_STANDARD",				LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER,
"SS_LEFT",					0x00000000L,
"SS_CENTER",				0x00000001L,
"SS_RIGHT",					0x00000002L,
"SS_ICON",             		0x00000003L,
"SS_BLACKRECT",        		0x00000004L,
"SS_GRAYRECT",         		0x00000005L,
"SS_WHITERECT",        		0x00000006L,
"SS_BLACKFRAME",       		0x00000007L,
"SS_GRAYFRAME",        		0x00000008L,
"SS_WHITEFRAME",       		0x00000009L,
"SS_USERITEM",         		0x0000000AL,
"SS_SIMPLE",           		0x0000000BL,
"SS_LEFTNOWORDWRAP",   		0x0000000CL,
"SS_OWNERDRAW",        		0x0000000DL,
"SS_BITMAP",           		0x0000000EL,
"SS_ENHMETAFILE",      		0x0000000FL,
"SS_ETCHEDHORZ",       		0x00000010L,
"SS_ETCHEDVERT",       		0x00000011L,
"SS_ETCHEDFRAME",      		0x00000012L,
"SS_TYPEMASK",         		0x0000001FL,
"SS_REALSIZECONTROL",  		0x00000040L,
"SS_NOPREFIX",         		0x00000080L,
"SS_NOTIFY",           		0x00000100L,
"SS_CENTERIMAGE",      		0x00000200L,
"SS_RIGHTJUST",        		0x00000400L,
"SS_REALSIZEIMAGE",    		0x00000800L,
"SS_SUNKEN",           		0x00001000L,
"SS_EDITCONTROL",      		0x00002000L,
"SS_ENDELLIPSIS",      		0x00004000L,
"SS_PATHELLIPSIS",     		0x00008000L,
"SS_WORDELLIPSIS",			0x0000C000L,
"SS_ELLIPSISMASK",			0x0000C000L,

"SBS_HORZ",						0x0000L,
"SBS_VERT",						0x0001L,
"SBS_TOPALIGN",					0x0002L,
"SBS_LEFTALIGN",				0x0002L,
"SBS_BOTTOMALIGN",				0x0004L,
"SBS_RIGHTALIGN",				0x0004L,
"SBS_SIZEBOXTOPLEFTALIGN",		0x0002L,
"SBS_SIZEBOXBOTTOMRIGHTALIGN",	0x0004L,
"SBS_SIZEBOX",					0x0008L,
"SBS_SIZEGRIP",					0x0010L,

"TBS_AUTOTICKS",           0x0001,
"TBS_VERT",                0x0002,
"TBS_HORZ",                0x0000,
"TBS_TOP",                 0x0004,
"TBS_BOTTOM",              0x0000,
"TBS_LEFT",                0x0004,
"TBS_RIGHT",               0x0000,
"TBS_BOTH",                0x0008,
"TBS_NOTICKS",             0x0010,
"TBS_ENABLESELRANGE",      0x0020,
"TBS_FIXEDLENGTH",         0x0040,
"TBS_NOTHUMB",             0x0080,
"TBS_TOOLTIPS",            0x0100,
"TBS_REVERSED",            0x0200,
"TBS_DOWNISLEFT",          0x0400,

"UDS_WRAP",                0x0001,
"UDS_SETBUDDYINT",         0x0002,
"UDS_ALIGNRIGHT",          0x0004,
"UDS_ALIGNLEFT",           0x0008,
"UDS_AUTOBUDDY",           0x0010,
"UDS_ARROWKEYS",           0x0020,
"UDS_HORZ",                0x0040,
"UDS_NOTHOUSANDS",         0x0080,
"UDS_HOTTRACK",            0x0100,

"PBS_SMOOTH",              0x01,
"PBS_VERTICAL",            0x04,

"LVS_ICON",                0x0000,
"LVS_REPORT",              0x0001,
"LVS_SMALLICON",           0x0002,
"LVS_LIST",                0x0003,
"LVS_TYPEMASK",            0x0003,
"LVS_SINGLESEL",           0x0004,
"LVS_SHOWSELALWAYS",       0x0008,
"LVS_SORTASCENDING",       0x0010,
"LVS_SORTDESCENDING",      0x0020,
"LVS_SHAREIMAGELISTS",     0x0040,
"LVS_NOLABELWRAP",         0x0080,
"LVS_AUTOARRANGE",         0x0100,
"LVS_EDITLABELS",          0x0200,
"LVS_OWNERDATA",           0x1000,
"LVS_NOSCROLL",            0x2000,
"LVS_TYPESTYLEMASK",       0xfc00,
"LVS_ALIGNTOP",            0x0000,
"LVS_ALIGNLEFT",           0x0800,
"LVS_ALIGNMASK",           0x0c00,
"LVS_OWNERDRAWFIXED",      0x0400,
"LVS_NOCOLUMNHEADER",      0x4000,
"LVS_NOSORTHEADER",        0x8000,

"LVS_EX_GRIDLINES",        0x00000001,
"LVS_EX_SUBITEMIMAGES",    0x00000002,
"LVS_EX_CHECKBOXES",       0x00000004,
"LVS_EX_TRACKSELECT",      0x00000008,
"LVS_EX_HEADERDRAGDROP",   0x00000010,
"LVS_EX_FULLROWSELECT",    0x00000020,
"LVS_EX_ONECLICKACTIVATE", 0x00000040,
"LVS_EX_TWOCLICKACTIVATE", 0x00000080,
"LVS_EX_FLATSB",           0x00000100,
"LVS_EX_REGIONAL",         0x00000200,
"LVS_EX_INFOTIP",          0x00000400,
"LVS_EX_UNDERLINEHOT",     0x00000800,
"LVS_EX_UNDERLINECOLD",    0x00001000,
"LVS_EX_MULTIWORKAREAS",   0x00002000,
"LVS_EX_LABELTIP",         0x00004000,
"LVS_EX_BORDERSELECT",     0x00008000,
"LVS_EX_DOUBLEBUFFER",     0x00010000,
"LVS_EX_HIDELABELS",       0x00020000,
"LVS_EX_SINGLEROW",        0x00040000,
"LVS_EX_SNAPTOGRID",       0x00080000,
"LVS_EX_SIMPLESELECT",     0x00100000,

"TVS_HASBUTTONS",          0x0001,
"TVS_HASLINES",            0x0002,
"TVS_LINESATROOT",         0x0004,
"TVS_EDITLABELS",          0x0008,
"TVS_DISABLEDRAGDROP",     0x0010,
"TVS_SHOWSELALWAYS",       0x0020,
"TVS_RTLREADING",          0x0040,
"TVS_NOTOOLTIPS",          0x0080,
"TVS_CHECKBOXES",          0x0100,
"TVS_TRACKSELECT",         0x0200,
"TVS_SINGLEEXPAND",        0x0400,
"TVS_INFOTIP",             0x0800,
"TVS_FULLROWSELECT",       0x1000,
"TVS_NOSCROLL",            0x2000,
"TVS_NONEVENHEIGHT",       0x4000,
"TVS_NOHSCROLL",           0x8000,

"TCS_SCROLLOPPOSITE",      0x0001,
"TCS_BOTTOM",              0x0002,
"TCS_RIGHT",               0x0002,
"TCS_MULTISELECT",         0x0004,
"TCS_FLATBUTTONS",         0x0008,
"TCS_FORCEICONLEFT",       0x0010,
"TCS_FORCELABELLEFT",      0x0020,
"TCS_HOTTRACK",            0x0040,
"TCS_VERTICAL",            0x0080,
"TCS_TABS",                0x0000,
"TCS_BUTTONS",             0x0100,
"TCS_SINGLELINE",          0x0000,
"TCS_MULTILINE",           0x0200,
"TCS_RIGHTJUSTIFY",        0x0000,
"TCS_FIXEDWIDTH",          0x0400,
"TCS_RAGGEDRIGHT",         0x0800,
"TCS_FOCUSONBUTTONDOWN",   0x1000,
"TCS_OWNERDRAWFIXED",      0x2000,
"TCS_TOOLTIPS",            0x4000,
"TCS_FOCUSNEVER",          0x8000,
"TCS_EX_FLATSEPARATORS",   0x00000001,
"TCS_EX_REGISTERDROP",     0x00000002,

"DTS_UPDOWN",          0x0001,
"DTS_SHOWNONE",        0x0002,
"DTS_SHORTDATEFORMAT", 0x0000,
"DTS_LONGDATEFORMAT",  0x0004,
"DTS_SHORTDATECENTURYFORMAT", 0x000C,
"DTS_TIMEFORMAT",      0x0009,
"DTS_APPCANPARSE",     0x0010,
"DTS_RIGHTALIGN",      0x0020
};

#include <pshpack1.h>
typedef struct { 
  WORD dlgVer; 
  WORD signature; 
  DWORD helpID; 
  DWORD exStyle; 
  DWORD style; 
  WORD cDlgItems; 
  short x; 
  short y; 
  short cx; 
  short cy;
  WORD menu_existance;
  WORD menu;
  WORD wndclass_existance;
  WORD	wndClassName[260];
  WORD title[260]; 
  WORD pointsize; 
  WORD weight; 
  BYTE bItalic; 
  BYTE charset;
  WCHAR typeface[260];
} DLGTEMPLATEEX;


typedef struct {
    DWORD helpID;
    DWORD exStyle;
    DWORD style;
    short x;
    short y;
    short cx;
    short cy;
    WORD id;
    WORD wndclass_existance;
	WORD wndClassName[260];
    WORD title[260];
    WORD extraCount;
} DLGITEMTEMPLATEEX;

struct DialogData
	{
	UINT	nID;
	UINT	nVer;
	_Point	ptDlg;
	_Size	szDlg;
	UINT	nStyle;
	UINT	nStyleEx;
	_String strCaption;
	_String strWndClassName;
	UINT	nItemCount;
	UINT	nMenuId;
	UINT	nFontSize;
	_String strFaceName;
	BOOL	bItalic;
	BYTE	btCharset;
	UINT	nWeight;
	};

struct DlgItemData
	{ 
	UINT	helpID; 
	UINT	nStyleEx; 
	UINT	nStyle; 
	_Point	ptItem;
	_Size	szItem;
	UINT	nID; 
	_String strWndClass; 
	_String strTitle; 
	UINT	nExtraCount; 
	}; 
#include <poppack.h>


class ResourceCompiler : public CObject
{
public:
	ResourceCompiler	();
	virtual ~ResourceCompiler();
	static ResourceCompiler* GetCompiler();

public:
	void SetResourceHeaderFile(_String strResourceHdr);

protected: // attributes.
	AutoSortedArray m_arrStyleName_Value;		// Exampl: value = DecodeString(_T("WS_VISIBLE")).
												//		   data  = WS_VISIBLE.
	AutoSortedArray m_arrDefineName_Value;		// Exampl: value = IDD_DIALOG1
												//		   data  = 1001
	_String			m_strLastError;				// Last error string value.

	AutoSortedArray m_arrDlgItemType_Func;		// Exampl: value = IDD_DIALOG1
	CMapStringToPtr m_mapDialogToTempl;

	UINT			m_nDefineValue;

public:

	BOOL		AddDefine					(_String sDefineName, UINT nValue);
	BOOL		CompileDialogScriptFile		(_String strDlgScriptFile, void*& lpBuffer, UINT& nBuffSize);
	BOOL		CompileDialogScript			(_String strDlgScript, void*& lpBuffer, UINT& nBuffSize, DWORD dwStyleAdd = 0L, DWORD dwStyleExAdd = 0L);

protected: // methods.
	BOOL		ReadDefinesFromFile			(_String strHdrFile, AutoSortedArray& arrDefines);
	BOOL		ReadDefines					(_String& strHdrFileContent, AutoSortedArray& arrDefines);
	void		AddStyleDefine				(_String sDefName, UINT nValue);
	
	BOOL		GetNextWord					(LPTSTR& lpszScript, _String& strScriptWord);
	BOOL		GetDefineValue				(LPTSTR& lpszScript, UINT& nDefineValue, AutoSortedArray& arrDefines);
	BOOL		GetNumericValue				(LPTSTR& lpszScript, long& nNumericValue);
	BOOL		GetStringValue				(LPTSTR& lpszScript, _String& sStringValue);
	BOOL		GetRectValue				(LPTSTR& lpszScript, _Rect& rcValue);
	BOOL		GetPointValue				(LPTSTR& lpszScript, _Point& ptValue);
	BOOL		GetLogFontValue				(LPTSTR& lpszScript, LOGFONT& lfValue);
	BOOL		DefineStringToValue			(_String& strDefineString, UINT& nDefineValue, AutoSortedArray& arrDefines);
	
	BOOL		ReadDialogScriptHeader		(DialogData* pDlgData, LPTSTR& lpszScript);

	static BOOL BuildDlgTemplate			(DialogData* pDlgData, void*& lpBuffer, UINT& nBuffSize);
	static BOOL BuildItemTemplate			(DlgItemData* pItemData, void*& lpBuffer, UINT& nBuffSize);

	static BOOL Parse_DlgItem_PUSHBUTTON	(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
	static BOOL Parse_DlgItem_EDITTEXT		(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
	static BOOL Parse_DlgItem_COMBOBOX		(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
	static BOOL Parse_DlgItem_LISTBOX		(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
	static BOOL Parse_DlgItem_GROUPBOX		(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
	static BOOL Parse_DlgItem_LTEXT			(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
	static BOOL Parse_DlgItem_SCROLLBAR		(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
	static BOOL Parse_DlgItem_CONTROL		(ResourceCompiler* pCompiler, LPTSTR& lpszScript, void*& lpBuffer, UINT& nBuffSize, _String& strError);
};

/*
DEFPUSHBUTTON   "OK",IDOK,7,7,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,7,24,50,14,BS_FLAT,WS_EX_RIGHT
    CONTROL         "Check1",IDC_CHECK1,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,241,33,39,10
    EDITTEXT        IDC_EDIT1,211,121,40,14,ES_AUTOHSCROLL
    COMBOBOX        IDC_COMBO1,171,94,48,30,CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | WS_TABSTOP
    LISTBOX         IDC_LIST1,244,63,48,40,LBS_SORT | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP
    GROUPBOX        "Static",IDC_STATIC,224,158,48,40
    CONTROL         "Radio1",IDC_RADIO1,"Button",BS_AUTORADIOBUTTON,66,79,38,10
    LTEXT           "Static",IDC_STATIC,71,145,19,8
    CONTROL         "",IDC_STATIC,"Static",SS_BLACKFRAME,114,108,20,20
    SCROLLBAR       IDC_SCROLLBAR1,156,156,40,11
    SCROLLBAR       IDC_SCROLLBAR2,161,30,10,40,SBS_VERT
    CONTROL         "",IDC_SLIDER1,"msctls_trackbar32",TBS_BOTH | TBS_NOTICKS | WS_TABSTOP,161,10,100,15
    CONTROL         "",IDC_SPIN1,"msctls_updown32",UDS_ARROWKEYS,305,138,10,14
    CONTROL         "",IDC_PROGRESS1,"msctls_progress32",WS_BORDER,7,183,80,14
    CONTROL         "",IDC_HOTKEY1,"msctls_hotkey32",WS_BORDER | WS_TABSTOP,101,193,80,14
    CONTROL         "",IDC_LIST2,"SysListView32",LVS_ALIGNLEFT | WS_BORDER | WS_TABSTOP,81,7,60,50
    CONTROL         "",IDC_TREE1,"SysTreeView32",WS_BORDER | WS_TABSTOP,299,158,60,50
    CONTROL         "",IDC_TAB1,"SysTabControl32",0x0,311,41,50,30

    CONTROL         "",IDC_ANIMATE1,"SysAnimate32",WS_BORDER | WS_TABSTOP,319,92,20,20
    CONTROL         "",IDC_DATETIMEPICKER1,"SysDateTimePick32",DTS_RIGHTALIGN | WS_TABSTOP,269,15,100,15
    CONTROL         "",IDC_COMBOBOXEX1,"ComboBoxEx32",CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | WS_TABSTOP,196,217,48,30
*/
