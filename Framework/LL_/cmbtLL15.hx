/**** C/C++ run-time linkage constants and function definitions for LL15.DLL ****/
/****  (c) 1991,..,1999,2000,..,06,... combit GmbH, Konstanz, Germany  ****/
/****  [build of 2009-10-22 14:10:16] ****/

#ifndef _LL15_H
#define _LL15_H
#define _LL15_HX

#ifndef _LL15_DONT_INCLUDE_OLE2_H
  #if defined(WIN32)
    #include <ole2.h>
  #endif
#endif

#ifndef EXPLICIT_TYPES
  #define EXPLICIT_TYPES
  #ifndef INT
    typedef int INT; /* you should comment this out if you have any problems with INT */
  #endif
  #ifndef CHAR
    typedef char CHAR; /* you should comment this out if you have any problems with CHAR */
  #endif
  typedef unsigned char UINT8;
  typedef unsigned short UINT16;
  typedef signed char INT8;
  typedef signed short INT16;
  #ifndef _BASETSD_H_ /* MSVC6 defines these itself in basetsd.h */
    typedef unsigned long UINT32;
    typedef signed long INT32;
  #endif
#endif

#ifndef DLLPROC
  #define DLLPROC WINAPI
#endif
#ifndef DLLCPROC
  #define DLLCPROC WINAPI
#endif


#ifndef CMBTLANG_DEFAULT
 #define CMBTLANG_DEFAULT    -1
 #define CMBTLANG_GERMAN      0
 #define CMBTLANG_ENGLISH     1
 #define CMBTLANG_ARABIC      2
 #define CMBTLANG_AFRIKAANS   3
 #define CMBTLANG_ALBANIAN    4
 #define CMBTLANG_BASQUE      5
 #define CMBTLANG_BULGARIAN   6
 #define CMBTLANG_BYELORUSSIAN 7
 #define CMBTLANG_CATALAN     8
 #define CMBTLANG_CHINESE     9
 #define CMBTLANG_CROATIAN    10
 #define CMBTLANG_CZECH       11
 #define CMBTLANG_DANISH      12
 #define CMBTLANG_DUTCH       13
 #define CMBTLANG_ESTONIAN    14
 #define CMBTLANG_FAEROESE    15
 #define CMBTLANG_FARSI       16
 #define CMBTLANG_FINNISH     17
 #define CMBTLANG_FRENCH      18
 #define CMBTLANG_GREEK       19
 #define CMBTLANG_HEBREW      20
 #define CMBTLANG_HUNGARIAN   21
 #define CMBTLANG_ICELANDIC   22
 #define CMBTLANG_INDONESIAN  23
 #define CMBTLANG_ITALIAN     24
 #define CMBTLANG_JAPANESE    25
 #define CMBTLANG_KOREAN      26
 #define CMBTLANG_LATVIAN     27
 #define CMBTLANG_LITHUANIAN  28
 #define CMBTLANG_NORWEGIAN   29
 #define CMBTLANG_POLISH      30
 #define CMBTLANG_PORTUGUESE  31
 #define CMBTLANG_ROMANIAN    32
 #define CMBTLANG_RUSSIAN     33
 #define CMBTLANG_SLOVAK      34
 #define CMBTLANG_SLOVENIAN   35
 #define CMBTLANG_SERBIAN     36
 #define CMBTLANG_SPANISH     37
 #define CMBTLANG_SWEDISH     38
 #define CMBTLANG_THAI        39
 #define CMBTLANG_TURKISH     40
 #define CMBTLANG_UKRAINIAN   41
 #define CMBTLANG_CHINESE_TRADITIONAL   48
 #define CMBTLANG_PORTUGUESE_BRAZILIAN   	129
 #define CMBTLANG_SPANISH_COLOMBIA  		130
 #define CMBTLANG_UNSPECIFIED	255
#endif


/*--- type declarations ---*/

#ifndef HJOB
  #define HJOB                           INT
#endif
#ifndef HLLJOB
  #define HLLJOB                         INT
#endif
#ifndef HLLDOMOBJ
  #define HLLDOMOBJ                      LPVOID
#endif
#ifndef PHLLDOMOBJ
  #define PHLLDOMOBJ                     LPVOID FAR *
#endif
#ifndef HLLEXPR
  #define HLLEXPR                        LPVOID
#endif
#ifndef HLLINTERF
  #define HLLINTERF                      LPVOID
#endif
#ifndef HSTG
  #define HSTG                           UINT_PTR
#endif
#ifndef HLLSTG
  #define HLLSTG                         UINT_PTR
#endif
#ifndef HLLRTFOBJ
  #define HLLRTFOBJ                      UINT_PTR
#endif
#ifndef _PRECT
  #define _PRECT                         RECT FAR *
#endif
#ifndef _PCRECT
  #define _PCRECT                        const RECT FAR *
#endif
#ifndef HLISTPOS
  #define HLISTPOS                       UINT_PTR
#endif
#ifndef _LPHANDLE
  #define _LPHANDLE                      HANDLE FAR *
#endif
#ifndef _LPINT
  #define _LPINT                         INT FAR *
#endif
#ifndef _LPUINT
  #define _LPUINT                        UINT FAR *
#endif
#ifndef _LCID
  #define _LCID                          UINT
#endif
#ifndef PHGLOBAL
  #define PHGLOBAL                       HGLOBAL FAR *
#endif
#ifndef LLPUINT
  #define LLPUINT                        UINT FAR *
#endif
#ifndef _PDEVMODE
  #define _PDEVMODE                      DEVMODE FAR *
#endif
#ifndef _PDEVMODEA
  #define _PDEVMODEA                     DEVMODEA FAR *
#endif
#ifndef _PDEVMODEW
  #define _PDEVMODEW                     DEVMODEW FAR *
#endif
#ifndef _PCDEVMODE
  #define _PCDEVMODE                     const DEVMODE FAR *
#endif
#ifndef _PCDEVMODEA
  #define _PCDEVMODEA                    const DEVMODEA FAR *
#endif
#ifndef _PCDEVMODEW
  #define _PCDEVMODEW                    const DEVMODEW FAR *
#endif
#ifndef PSCLLCOLUMN
  #define PSCLLCOLUMN                    scLlColumn FAR *
#endif
#ifndef PVARIANT
  #define PVARIANT                       VARIANT FAR *
#endif
#ifndef CTL_GUID
  #define CTL_GUID                       const GUID
#endif
#ifndef CTL_PUNK
  #define CTL_PUNK                       IUnknown FAR *
#endif
#ifndef CTL_PPUNK
  #define CTL_PPUNK                      IUnknown FAR * FAR *
#endif
#ifndef _PSYSTEMTIME
  #define _PSYSTEMTIME                   SYSTEMTIME FAR *
#endif
#ifndef _PCSYSTEMTIME
  #define _PCSYSTEMTIME                  const SYSTEMTIME FAR *
#endif
#ifndef PSCLLPROJECTUSERDATA
  #define PSCLLPROJECTUSERDATA           scLlProjectUserData FAR *
#endif
#ifndef _LPINTJAVADUMMY
  #define _LPINTJAVADUMMY                INT FAR *
#endif
#ifndef _PBSTR
  #define _PBSTR                         BSTR FAR *
#endif
#ifndef PILlXOBStatJob
  #define PILlXOBStatJob                 ILlXOBStatJob FAR *
#endif
#ifndef PPILlXOBStatJob
  #define PPILlXOBStatJob                ILlXOBStatJob FAR * FAR *
#endif
#ifndef _PUINT8
  #define _PUINT8                        UINT8 FAR *
#endif
#ifndef _PCUINT8
  #define _PCUINT8                       const UINT8 FAR *
#endif
#ifndef _PISTREAM
  #define _PISTREAM                      IStream FAR *
#endif
#ifndef _PHGLOBAL
  #define _PHGLOBAL                      HGLOBAL FAR *
#endif

/*--- constant declarations ---*/

#define LL_LINK_HPOS_NONE              (0x0000              )
#define LL_LINK_HPOS_START             (0x0001              )
#define LL_LINK_HPOS_END               (0x0002              )
#define LL_LINK_HPOS_ABS               (0x0003              )
#define LL_LINK_HPOS_MASK              (0x0007              )
#define LL_LINK_VPOS_NONE              (0x0000              )
#define LL_LINK_VPOS_START             (0x0010              )
#define LL_LINK_VPOS_END               (0x0020              )
#define LL_LINK_VPOS_ABS               (0x0030              )
#define LL_LINK_VPOS_MASK              (0x0070              )
#define LL_LINK_HSIZE_NONE             (0x0000              )
#define LL_LINK_HSIZE_PROP             (0x0100              )
#define LL_LINK_HSIZE_INV              (0x0200              )
#define LL_LINK_HSIZE_MASK             (0x0300              )
#define LL_LINK_VSIZE_NONE             (0x0000              )
#define LL_LINK_VSIZE_PROP             (0x1000              )
#define LL_LINK_VSIZE_INV              (0x2000              )
#define LL_LINK_VSIZE_MASK             (0x3000              )
#define LL_LINK_SIZEPOS_MASK           (0x3377              )
#define LL_LINK_SIZEOFPARENT           (0x4000              )
#define LL_DESIGNERPRINTCALLBACK_PREVIEW_START (1                   )
#define LL_DESIGNERPRINTCALLBACK_PREVIEW_ABORT (2                   )
#define LL_DESIGNERPRINTCALLBACK_PREVIEW_FINALIZE (3                   )
#define LL_DESIGNERPRINTCALLBACK_PREVIEW_QUEST_JOBSTATE (4                   )
#define LL_DESIGNERPRINTCALLBACK_EXPORT_START (5                   )
#define LL_DESIGNERPRINTCALLBACK_EXPORT_ABORT (6                   )
#define LL_DESIGNERPRINTCALLBACK_EXPORT_FINALIZE (7                   )
#define LL_DESIGNERPRINTCALLBACK_EXPORT_QUEST_JOBSTATE (8                   )
#define LL_DESIGNERPRINTTHREAD_STATE_STOPPED (0                   )
#define LL_DESIGNERPRINTTHREAD_STATE_SUSPENDED (1                   )
#define LL_DESIGNERPRINTTHREAD_STATE_RUNNING (2                   )
#define LL_DRILLDOWN_START             (1                   )
#define LL_DRILLDOWN_FINALIZE          (2                   )
#define LL_JOBOPENFLAG_NOLLXPRELOAD    (0x00001000          )
#define LL_JOBOPENFLAG_ONLYEXACTLANGUAGE (0x00002000          ) /* do not look for '@@' LNG file */
#define LL_DEBUG_CMBTLL                (0x0001              ) /* debug CMBTLLnn.DLL */
#define LL_DEBUG_CMBTDWG               (0x0002              ) /* debug CMBTDWnn.DLL */
#define LL_DEBUG_CMBTLS                (0x0080              ) /* debug CMBTLSnn.DLL */
#define LL_DEBUG_PRNINFO               (0x0100              ) /* debug basic printer operations */
#define LL_DEBUG_CMBTLL_NOCALLBACKS    (0x0004              )
#define LL_DEBUG_CMBTLL_NOSTORAGE      (0x0008              )
#define LL_DEBUG_CMBTLL_NOWAITDLG      (0x0010              )
#define LL_DEBUG_CMBTLL_NOSYSINFO      (0x0020              )
#define LL_DEBUG_CMBTLL_LOGTOFILE      (0x0040              )
#define LL_VERSION_MAJOR               (1                   ) /* direct return of major version (f.ex. 1) */
#define LL_VERSION_MINOR               (2                   ) /* direct return of minor version (f.ex. 13) */
#define LL_VERSION_SERNO_LO            (3                   ) /* LOWORD(serial number) */
#define LL_VERSION_SERNO_HI            (4                   ) /* HIWORD(serial number) */
#define LL_VERSION_OEMNO               (5                   ) /* OEM number */
#define LL_VERSION_RESMAJOR            (11                  ) /* internal, for LlRCGetVersion: resource version */
#define LL_VERSION_RESMINOR            (12                  ) /* internal, for LlRCGetVersion: resource version */
#define LL_VERSION_RESLANGUAGE         (14                  ) /* internal, for LlRCGetVersion: resource language */
#define LL_CMND_DRAW_USEROBJ           (0                   ) /* callback for LL_DRAWING_USEROBJ */
#define LL_CMND_EDIT_USEROBJ           (1                   ) /* callback for LL_DRAWING_USEROBJ_DLG */
#define LL_CMND_GETSIZE_USEROBJ        (2                   )
#define LL_CMND_GETSIZE_USEROBJ_SCM    (3                   )
#define LL_CMND_GETSIZE_USEROBJ_PIXEL  (4                   )
#define LL_CMND_TABLELINE              (10                  ) /* callback for LL_CB_TABLELINE */
#define LL_TABLE_LINE_HEADER           (0                   )
#define LL_TABLE_LINE_BODY             (1                   )
#define LL_TABLE_LINE_FOOTER           (2                   )
#define LL_TABLE_LINE_FILL             (3                   )
#define LL_TABLE_LINE_GROUP            (4                   )
#define LL_TABLE_LINE_GROUPFOOTER      (5                   )
#define LL_CMND_TABLEFIELD             (11                  ) /* callback for LL_CB_TABLEFIELD */
#define LL_TABLE_FIELD_HEADER          (0                   )
#define LL_TABLE_FIELD_BODY            (1                   )
#define LL_TABLE_FIELD_FOOTER          (2                   )
#define LL_TABLE_FIELD_FILL            (3                   )
#define LL_TABLE_FIELD_GROUP           (4                   )
#define LL_TABLE_FIELD_GROUPFOOTER     (5                   )
#define LL_CMND_EVALUATE               (12                  ) /* callback for "External$" function */
#define LL_CMND_OBJECT                 (20                  ) /* callback of LL_CB_OBJECT */
#define LL_CMND_PAGE                   (21                  ) /* callback of LL_CB_PAGE */
#define LL_CMND_PROJECT                (22                  ) /* callback of LL_CB_PROJECT */
#define LL_CMND_DRAW_GROUP_BEGIN       (23                  ) /* callback for LlPrintBeginGroup */
#define LL_CMND_DRAW_GROUP_END         (24                  ) /* callback for LlPrintEndGroup */
#define LL_CMND_DRAW_GROUPLINE         (25                  ) /* callback for LlPrintGroupLine */
#define LL_RSP_GROUP_IMT               (0                   )
#define LL_RSP_GROUP_NEXTPAGE          (1                   )
#define LL_RSP_GROUP_OK                (2                   )
#define LL_RSP_GROUP_DRAWFOOTER        (3                   )
#define LL_CMND_HELP                   (30                  ) /* lParam: HIWORD=HELP_xxx, LOWORD=Context # */
#define LL_CMND_ENABLEMENU             (31                  ) /* undoc: lParam/LOWORD(lParam) = HMENU */
#define LL_CMND_MODIFYMENU             (32                  ) /* undoc: lParam/LOWORD(lParam) = HMENU */
#define LL_CMND_SELECTMENU             (33                  ) /* undoc: lParam=ID (return TRUE if processed) */
#define LL_CMND_GETVIEWERBUTTONSTATE   (34                  ) /* HIWORD(lParam)=ID, LOWORD(lParam)=state */
#define LL_CMND_VARHELPTEXT            (35                  ) /* lParam=LPSTR(Name), returns LPSTR(Helptext) */
#define LL_INFO_METER                  (37                  ) /* lParam = addr(scLlMeterInfo) */
#define LL_METERJOB_LOAD               (1                   )
#define LL_METERJOB_SAVE               (2                   )
#define LL_METERJOB_CONSISTENCYCHECK   (3                   )
#define LL_METERJOB_PASS2              (4                   )
#define LL_METERJOB_PASS3              (5                   )
#define LL_NTFY_FAILSFILTER            (1000                ) /* data set fails filter expression */
#define LL_NTFY_VIEWERBTNCLICKED       (38                  ) /* user presses a preview button (action will be done). lParam=ID. result: 0=allowed, 1=not allowed */
#define LL_CMND_DLGEXPR_VARBTN         (39                  ) /* lParam: @scLlDlgExprVarExt, return: IDOK for ok */
#define LL_CMND_HOSTPRINTER            (40                  ) /* lParam: scLlPrinter */
#define LL_PRN_CREATE_DC               (1                   ) /* scLlPrinter._nCmd values */
#define LL_PRN_DELETE_DC               (2                   )
#define LL_PRN_SET_ORIENTATION         (3                   )
#define LL_PRN_GET_ORIENTATION         (4                   )
#define LL_PRN_EDIT                    (5                   ) /* unused */
#define LL_PRN_GET_DEVICENAME          (6                   )
#define LL_PRN_GET_DRIVERNAME          (7                   )
#define LL_PRN_GET_PORTNAME            (8                   )
#define LL_PRN_RESET_DC                (9                   )
#define LL_PRN_COMPARE_PRINTER         (10                  )
#define LL_PRN_GET_PHYSPAGE            (11                  )
#define LL_PRN_SET_PHYSPAGE            (12                  )
#define LL_PRN_GET_PAPERFORMAT         (13                  ) /* fill _nPaperFormat */
#define LL_PRN_SET_PAPERFORMAT         (14                  ) /* _nPaperFormat, _xPaperSize, _yPaperSize */
#define LL_OEM_TOOLBAR_START           (41                  )
#define LL_OEM_TOOLBAR_END             (50                  )
#define LL_NTFY_EXPRERROR              (51                  ) /* lParam = LPCSTR(error text) */
#define LL_CMND_CHANGE_DCPROPERTIES_CREATE (52                  ) /* lParam = addr(scLlPrinter), _hDC is valid */
#define LL_CMND_CHANGE_DCPROPERTIES_DOC (53                  ) /* lParam = addr(scLlPrinter), _hDC is valid */
#define LL_CMND_CHANGE_DCPROPERTIES_PAGE (54                  ) /* lParam = addr(scLlPrinter), _hDC is valid */
#define LL_CMND_CHANGE_DCPROPERTIES_PREPAGE (56                  ) /* lParam = addr(scLlPrinter), _hDC and _pszBuffer( DEVMODE* ) are valid */
#define LL_CMND_MODIFY_METAFILE        (57                  ) /* lParam = handle of metafile (32 bit: enh. metafile) */
#define LL_INFO_PRINTJOBSUPERVISION    (58                  ) /* lParam = addr(scLlPrintJobInfo) */
#define LL_CMND_DELAYEDVALUE           (59                  ) /* lParam = addr(scLlDelayedValue) */
#define LL_CMND_SUPPLY_USERDATA        (60                  ) /* lParam = addr(scLlProjectUserData) */
#define LL_CMND_SAVEFILENAME           (61                  ) /* lParam = LPCTSTR(Filename) */
#define LL_QUERY_IS_VARIABLE_OR_FIELD  (62                  ) /* lParam = addr(scLlDelayDefineFieldOrVariable), must be enabled by CB mask. If returns TRUE, the var must be defined in the callback... */
#define LL_NTFY_PROJECTLOADED          (63                  ) /* lParam = 0 */
#define LL_QUERY_DESIGNERACTIONSTATE   (64                  )
#define LL_NTFY_DESIGNERREADY          (65                  ) /* lParam = 0 */
#define LL_NTFY_DESIGNERPRINTJOB       (66                  )
#define LL_NTFY_VIEWERDRILLDOWN        (67                  )
#define LL_NTFY_QUEST_DRILLDOWNDENIED  (68                  ) /* see LS_VIEWERCONTROL_QUEST_DRILLDOWNDENIED */
#define LL_QUERY_DRILLDOWN_ADDITIONAL_BASETABLES_FOR_VARIABLES (69                  ) /* lParam = scLlDDFilterInfo* */
#define LL_QUERY_DRILLDOWN_ADDITIONAL_TABLES (70                  ) /* lParam = scLlDDFilterInfo* */
#define OBJECT_LABEL                   (1                   ) /* old - please do not use any more */
#define OBJECT_LIST                    (2                   )
#define OBJECT_CARD                    (3                   )
#define LL_PROJECT_LABEL               (1                   ) /* new names... */
#define LL_PROJECT_LIST                (2                   )
#define LL_PROJECT_CARD                (3                   )
#define LL_PROJECT_MASK                (0x000f              )
#define LL_OBJ_MARKER                  (0                   ) /* internal use only */
#define LL_OBJ_TEXT                    (1                   ) /* the following are used in the object callback */
#define LL_OBJ_RECT                    (2                   )
#define LL_OBJ_LINE                    (3                   )
#define LL_OBJ_BARCODE                 (4                   )
#define LL_OBJ_DRAWING                 (5                   )
#define LL_OBJ_TABLE                   (6                   )
#define LL_OBJ_TEMPLATE                (7                   )
#define LL_OBJ_ELLIPSE                 (8                   )
#define LL_OBJ_GROUP                   (9                   ) /* internal use only */
#define LL_OBJ_RTF                     (10                  )
#define LL_OBJ_LLX                     (11                  )
#define LL_OBJ_INPUT                   (12                  )
#define LL_OBJ_LAST                    (12                  ) /* last object type (for loops as upper bound) - if this is changed, change the contants in object.c too! */
#define LL_OBJ_PROJECT                 (254                 ) /* for exporter */
#define LL_OBJ_PAGE                    (255                 ) /* for exporter */
#define LL_DELAYEDVALUE                (0x80000000          )
#define LL_TYPEMASK                    (0x7ff00000          )
#define LL_TABLE_FOOTERFIELD           (0x00008000          ) /* 'or'ed for footline-only fields // reserved also for Variables (see "$$xx$$")!!!! */
#define LL_TABLE_GROUPFIELD            (0x00004000          ) /* 'or'ed for groupline-only fields */
#define LL_TABLE_HEADERFIELD           (0x00002000          ) /* 'or'ed for headline-only fields */
#define LL_TABLE_BODYFIELD             (0x00001000          ) /* 'or'ed for headline-only fields */
#define LL_TABLE_GROUPFOOTERFIELD      (0x00000800          ) /* 'or'ed for group-footer-line-only fields */
#define LL_TABLE_FIELDTYPEMASK         (0x0000f800          ) /* internal use */
#define LL_BARCODE                     (0x40000000          )
#define LL_BARCODE_EAN13               (0x40000000          )
#define LL_BARCODE_EAN8                (0x40000001          )
#define LL_BARCODE_GTIN13              (0x40000000          )
#define LL_BARCODE_GTIN8               (0x40000001          )
#define LL_BARCODE_UPCA                (0x40000002          )
#define LL_BARCODE_UPCE                (0x40000003          )
#define LL_BARCODE_3OF9                (0x40000004          )
#define LL_BARCODE_25INDUSTRIAL        (0x40000005          )
#define LL_BARCODE_25INTERLEAVED       (0x40000006          )
#define LL_BARCODE_25DATALOGIC         (0x40000007          )
#define LL_BARCODE_25MATRIX            (0x40000008          )
#define LL_BARCODE_POSTNET             (0x40000009          )
#define LL_BARCODE_FIM                 (0x4000000A          )
#define LL_BARCODE_CODABAR             (0x4000000B          )
#define LL_BARCODE_EAN128              (0x4000000C          )
#define LL_BARCODE_GS1_128             (0x4000000C          )
#define LL_BARCODE_CODE128             (0x4000000D          )
#define LL_BARCODE_DP_LEITCODE         (0x4000000E          )
#define LL_BARCODE_DP_IDENTCODE        (0x4000000F          )
#define LL_BARCODE_GERMAN_PARCEL       (0x40000010          )
#define LL_BARCODE_CODE93              (0x40000011          )
#define LL_BARCODE_MSI                 (0x40000012          )
#define LL_BARCODE_CODE11              (0x40000013          )
#define LL_BARCODE_MSI_10_CD           (0x40000014          )
#define LL_BARCODE_MSI_10_10           (0x40000015          )
#define LL_BARCODE_MSI_11_10           (0x40000016          )
#define LL_BARCODE_MSI_PLAIN           (0x40000017          )
#define LL_BARCODE_EAN14               (0x40000018          )
#define LL_BARCODE_GTIN14              (0x40000018          )
#define LL_BARCODE_UCC14               (0x40000019          )
#define LL_BARCODE_CODE39              (0x4000001A          )
#define LL_BARCODE_CODE39_CRC43        (0x4000001B          )
#define LL_BARCODE_PZN                 (0x4000001C          )
#define LL_BARCODE_CODE39_EXT          (0x4000001D          )
#define LL_BARCODE_JAPANESE_POSTAL     (0x4000001E          )
#define LL_BARCODE_RM4SCC              (0x4000001F          )
#define LL_BARCODE_RM4SCC_CRC          (0x40000020          )
#define LL_BARCODE_SSCC                (0x40000021          )
#define LL_BARCODE_ISBN                (0x40000022          )
#define LL_BARCODE_GS1                 (0x40000023          )
#define LL_BARCODE_GS1_TRUNCATED       (0x40000024          )
#define LL_BARCODE_GS1_STACKED         (0x40000025          )
#define LL_BARCODE_GS1_STACKED_OMNI    (0x40000026          )
#define LL_BARCODE_GS1_LIMITED         (0x40000027          )
#define LL_BARCODE_GS1_EXPANDED        (0x40000028          )
#define LL_BARCODE_LLXSTART            (0x40000040          )
#define LL_BARCODE_PDF417              (0x40000040          )
#define LL_BARCODE_MAXICODE            (0x40000041          )
#define LL_BARCODE_MAXICODE_UPS        (0x40000042          )
#define LL_BARCODE_DATAMATRIX          (0x40000044          )
#define LL_BARCODE_AZTEC               (0x40000045          )
#define LL_BARCODE_QRCODE              (0x40000046          )
#define LL_BARCODE_METHODMASK          (0x000000ff          )
#define LL_BARCODE_WITHTEXT            (0x00000100          )
#define LL_BARCODE_WITHOUTTEXT         (0x00000200          )
#define LL_BARCODE_TEXTDONTCARE        (0x00000000          )
#define LL_DRAWING                     (0x20000000          )
#define LL_DRAWING_HMETA               (0x20000001          )
#define LL_DRAWING_USEROBJ             (0x20000002          )
#define LL_DRAWING_USEROBJ_DLG         (0x20000003          )
#define LL_DRAWING_HBITMAP             (0x20000004          )
#define LL_DRAWING_HICON               (0x20000005          )
#define LL_DRAWING_HEMETA              (0x20000006          )
#define LL_DRAWING_HDIB                (0x20000007          ) /* global handle to BITMAPINFO and bits */
#define LL_DRAWING_METHODMASK          (0x000000ff          )
#define LL_META_MAXX                   (10000               )
#define LL_META_MAXY                   (10000               )
#define LL_TEXT                        (0x10000000          )
#define LL_TEXT_ALLOW_WORDWRAP         (0x10000000          )
#define LL_TEXT_DENY_WORDWRAP          (0x10000001          )
#define LL_TEXT_FORCE_WORDWRAP         (0x10000002          )
#define LL_NUMERIC                     (0x08000000          )
#define LL_NUMERIC_LOCALIZED           (0x08000001          )
#define LL_NUMERIC_INTEGER             (0x08000002          ) /* flag */
#define LL_DATE                        (0x04000000          ) /* LL's own julian */
#define LL_DATE_DELPHI_1               (0x04000001          )
#define LL_DATE_DELPHI                 (0x04000002          ) /* DELPHI 2, 3, 4: OLE DATE */
#define LL_DATE_MS                     (0x04000002          ) /* MS C/Basic: OLE DATE */
#define LL_DATE_OLE                    (0x04000002          ) /* generic: OLE DATE */
#define LL_DATE_VFOXPRO                (0x04000003          ) /* nearly LL's own julian, has an offset of 1! */
#define LL_DATE_DMY                    (0x04000004          ) /* <d><sep><m><sep><yyyy>. Year MUST be 4 digits! */
#define LL_DATE_MDY                    (0x04000005          ) /* <m><sep><d><sep><yyyy>. Year MUST be 4 digits! */
#define LL_DATE_YMD                    (0x04000006          ) /* <yyyy><sep><m><sep><d>. Year MUST be 4 digits! */
#define LL_DATE_YYYYMMDD               (0x04000007          ) /* <yyyymmdd> */
#define LL_DATE_LOCALIZED              (0x04000008          ) /* localized (automatic VariantConversion) */
#define LL_DATE_JULIAN                 (0x04000009          ) /* variant 'date' is a julian date */
#define LL_DATE_CLARION                (0x0400000a          ) /* days since 1800-12-28 (what's so special about that day?) */
#define LL_DATE_YMD_AUTO               (0x04000010          ) /* wither DMY, MDY or YMD, automatically detected */
#define LL_DATE_METHODMASK             (0x000000ff          )
#define LL_BOOLEAN                     (0x02000000          )
#define LL_RTF                         (0x01000000          )
#define LL_HTML                        (0x00800000          )
#define LL_LLXOBJECT                   (0x00100000          ) /* internal use only */
#define LL_FIXEDNAME                   (0x8000              )
#define LL_NOSAVEAS                    (0x4000              )
#define LL_EXPRCONVERTQUIET            (0x1000              ) /* convert to new expressions without warning box */
#define LL_NONAMEINTITLE               (0x0800              ) /* no file name appended to title */
#define LL_PRVOPT_PRN_USEDEFAULT       (0x00000000          )
#define LL_PRVOPT_PRN_ASKPRINTERIFNEEDED (0x00000001          )
#define LL_PRVOPT_PRN_ASKPRINTERALWAYS (0x00000002          )
#define LL_PRVOPT_PRN_ALWAYSUSEDEFAULT (0x00000003          )
#define LL_PRVOPT_PRN_ASSIGNMASK       (0x00000003          ) /* used by L&L */
#define LL_OPTION_COPIES               (0                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_OPTION_STARTPAGE            (1                   ) /* compatibility only, please use LL_PRNOPT_PAGE */
#define LL_OPTION_PAGE                 (1                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_OPTION_OFFSET               (2                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_OPTION_COPIES_SUPPORTED     (3                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_OPTION_FIRSTPAGE            (5                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_OPTION_LASTPAGE             (6                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_OPTION_JOBPAGES             (7                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_OPTION_PRINTORDER           (8                   ) /* compatibility only, please use LL_PRNOPT_... */
#define LL_PRNOPT_COPIES               (0                   )
#define LL_COPIES_HIDE                 (-32768              ) /* anything negative... */
#define LL_PRNOPT_STARTPAGE            (1                   )
#define LL_PRNOPT_PAGE                 (1                   ) /* alias; please do not use STARTPAGE any more... */
#define LL_PAGE_HIDE                   (-32768              ) /* must be exactly this value! */
#define LL_PRNOPT_OFFSET               (2                   )
#define LL_PRNOPT_COPIES_SUPPORTED     (3                   )
#define LL_PRNOPT_UNITS                (4                   ) /* r/o */
#define LL_UNITS_MM_DIV_10             (0                   ) /* for LL_PRNOPT_UNITS, LL_OPTION_UNITS and LL_OPTION_UNITS_DEFAULT */
#define LL_UNITS_INCH_DIV_100          (1                   )
#define LL_UNITS_INCH_DIV_1000         (2                   )
#define LL_UNITS_SYSDEFAULT_LORES      (3                   ) /* mm/10, in/100 (depending on regional settings of the system) */
#define LL_UNITS_SYSDEFAULT            (4                   ) /* mm/100, in/1000 (depending on regional settings of the system) */
#define LL_UNITS_MM_DIV_100            (5                   )
#define LL_UNITS_MM_DIV_1000           (6                   )
#define LL_UNITS_SYSDEFAULT_HIRES      (7                   ) /* mm/100, in/1000 (depending on regional settings of the system) */
#define LL_PRNOPT_FIRSTPAGE            (5                   )
#define LL_PRNOPT_LASTPAGE             (6                   )
#define LL_PRNOPT_JOBPAGES             (7                   )
#define LL_PRNOPT_PRINTORDER           (8                   )
#define LL_PRINTORDER_HORZ_LTRB        (0                   )
#define LL_PRINTORDER_VERT_LTRB        (1                   )
#define LL_PRINTORDER_HORZ_RBLT        (2                   )
#define LL_PRINTORDER_VERT_RBLT        (3                   )
#define LL_PRNOPT_DEFPRINTERINSTALLED  (11                  ) /* returns 0 for no default printer, 1 for default printer present */
#define LL_PRNOPT_PRINTDLG_DESTMASK    (12                  ) /* any combination of the ones below... Default: all. Outdated, please use LL_OPTIONSTR_EXPORTS_ALLOWED */
#define LL_DESTINATION_PRN             (1                   )
#define LL_DESTINATION_PRV             (2                   )
#define LL_DESTINATION_FILE            (4                   )
#define LL_DESTINATION_EXTERN          (8                   )
#define LL_DESTINATION_MSFAX           (16                  ) /* reserved */
#define LL_PRNOPT_PRINTDLG_DEST        (13                  ) /* default destination; outdated, please use LL_PRNOPTSTR_EXPORT */
#define LL_PRNOPT_PRINTDLG_ONLYPRINTERCOPIES (14                  ) /* show copies option in dialog only if they are supported by the printer. default: FALSE */
#define LL_PRNOPT_JOBID                (17                  )
#define LL_PRNOPT_PAGEINDEX            (18                  )
#define LL_PRNOPT_USES2PASS            (19                  ) /* r/o */
#define LL_PRNOPT_PAGERANGE_USES_ABSOLUTENUMBER (20                  ) /* default: FALSE */
#define LL_PRNOPT_USEMEMORYMETAFILE    (22                  ) /* default: false */
#define LL_PRNOPT_PARTIALPREVIEW       (23                  ) /* default: false */
#define LL_PRNOPTSTR_PRINTDST_FILENAME (0                   ) /* print to file: default filename (LlGet/SetPrintOptionString) */
#define LL_PRNOPTSTR_EXPORTDESCR       (1                   ) /* r/o, returns the description of the export chosen */
#define LL_PRNOPTSTR_EXPORT            (2                   ) /* sets default exporter to use / returns the name of the export chosen */
#define LL_PRNOPTSTR_PRINTJOBNAME      (3                   ) /* set name to be given to StartDoc() (lpszMessage of LlPrintWithBoxStart() */
#define LL_PRNOPTSTR_PRESTARTDOCESCSTRING (4                   ) /* sent before StartDoc() */
#define LL_PRNOPTSTR_POSTENDDOCESCSTRING (5                   ) /* sent after EndDoc() */
#define LL_PRNOPTSTR_PRESTARTPAGEESCSTRING (6                   ) /* sent before StartPage() */
#define LL_PRNOPTSTR_POSTENDPAGEESCSTRING (7                   ) /* sent after EndPage() */
#define LL_PRNOPTSTR_PRESTARTPROJECTESCSTRING (8                   ) /* sent before first StartPage() of project */
#define LL_PRNOPTSTR_POSTENDPROJECTESCSTRING (9                   ) /* sent after last EndPage() of project */
#define LL_PRNOPTSTR_PAGERANGES        (10                  )
#define LL_PRNOPTSTR_ISSUERANGES       (11                  )
#define LL_PRNOPTSTR_PREVIEWTITLE      (12                  ) /* default: language dependent */
#define LL_PRINT_V1POINTX              (0x00000000          )
#define LL_PRINT_NORMAL                (0x00000100          )
#define LL_PRINT_PREVIEW               (0x00000200          )
#define LL_PRINT_STORAGE               (0x00000200          ) /* same as LL_PRINT_PREVIEW */
#define LL_PRINT_FILE                  (0x00000400          )
#define LL_PRINT_USERSELECT            (0x00000800          )
#define LL_PRINT_EXPORT                (0x00000800          ) /* same as LL_PRINT_USERSELECT */
#define LL_PRINT_MODEMASK              (0x00000f00          )
#define LL_PRINT_MULTIPLE_JOBS         (0x00001000          )
#define LL_PRINT_KEEPJOB               (0x00002000          )
#define LL_PRINT_IS_DOM_CALLER         (0x00004000          ) /* internal */
#define LL_PRINT_DOM_NOCREATEDC        (0x00010000          ) /* internal */
#define LL_PRINT_DOM_NOOBJECTLOAD      (0x00020000          ) /* internal */
#define LL_PRINT_REMOVE_UNUSED_VARS    (0x00008000          ) /* optimization flag */
#define LL_BOXTYPE_NONE                (-1                  )
#define LL_BOXTYPE_NORMALMETER         (0                   )
#define LL_BOXTYPE_BRIDGEMETER         (1                   )
#define LL_BOXTYPE_NORMALWAIT          (2                   )
#define LL_BOXTYPE_BRIDGEWAIT          (3                   )
#define LL_BOXTYPE_EMPTYWAIT           (4                   )
#define LL_BOXTYPE_EMPTYABORT          (5                   )
#define LL_BOXTYPE_STDWAIT             (6                   )
#define LL_BOXTYPE_STDABORT            (7                   )
#define LL_BOXTYPE_MAX                 (7                   )
#define LL_FILE_ALSONEW                (0x8000              )
#define LL_FCTPARATYPE_DOUBLE          (0x01                )
#define LL_FCTPARATYPE_DATE            (0x02                )
#define LL_FCTPARATYPE_STRING          (0x04                )
#define LL_FCTPARATYPE_BOOL            (0x08                )
#define LL_FCTPARATYPE_DRAWING         (0x10                )
#define LL_FCTPARATYPE_BARCODE         (0x20                )
#define LL_FCTPARATYPE_ALL             (0x3f                )
#define LL_FCTPARATYPE_PARA1           (0x8001              )
#define LL_FCTPARATYPE_PARA2           (0x8002              )
#define LL_FCTPARATYPE_PARA3           (0x8003              )
#define LL_FCTPARATYPE_PARA4           (0x8004              )
#define LL_FCTPARATYPE_SAME            (0x803f              )
#define LL_FCTPARATYPE_MASK            (0x8fff              )
#define LL_FCTPARATYPEFLAG_NONULLCHECK (0x10000             )
#define LL_EXPRTYPE_DOUBLE             (1                   )
#define LL_EXPRTYPE_DATE               (2                   )
#define LL_EXPRTYPE_STRING             (3                   )
#define LL_EXPRTYPE_BOOL               (4                   )
#define LL_EXPRTYPE_DRAWING            (5                   )
#define LL_EXPRTYPE_BARCODE            (6                   )
#define LL_OPTION_NEWEXPRESSIONS       (0                   ) /* default: TRUE */
#define LL_OPTION_ONLYONETABLE         (1                   ) /* default: FALSE */
#define LL_OPTION_TABLE_COLORING       (2                   ) /* default: LL_COLORING_LL */
#define LL_COLORING_LL                 (0                   )
#define LL_COLORING_PROGRAM            (1                   )
#define LL_COLORING_DONTCARE           (2                   )
#define LL_OPTION_SUPERVISOR           (3                   ) /* default: FALSE */
#define LL_OPTION_UNITS                (4                   ) /* default: see LL_OPTION_METRIC */
#define LL_OPTION_TABSTOPS             (5                   ) /* default: LL_TABS_DELETE */
#define LL_TABS_DELETE                 (0                   )
#define LL_TABS_EXPAND                 (1                   )
#define LL_OPTION_CALLBACKMASK         (6                   ) /* default: 0x00000000 */
#define LL_CB_PAGE                     (0x40000000          ) /* callback for each page */
#define LL_CB_PROJECT                  (0x20000000          ) /* callback for each label */
#define LL_CB_OBJECT                   (0x10000000          ) /* callback for each object */
#define LL_CB_HELP                     (0x08000000          ) /* callback for HELP (F1/Button) */
#define LL_CB_TABLELINE                (0x04000000          ) /* callback for table line */
#define LL_CB_TABLEFIELD               (0x02000000          ) /* callback for table field */
#define LL_CB_QUERY_IS_VARIABLE_OR_FIELD (0x01000000          ) /* callback for delayload (LL_QUERY_IS_VARIABLE_OR_FIELD) */
#define LL_OPTION_CALLBACKPARAMETER    (7                   ) /* default: 0 */
#define LL_OPTION_HELPAVAILABLE        (8                   ) /* default: TRUE */
#define LL_OPTION_SORTVARIABLES        (9                   ) /* default: TRUE */
#define LL_OPTION_SUPPORTPAGEBREAK     (10                  ) /* default: TRUE */
#define LL_OPTION_SHOWPREDEFVARS       (11                  ) /* default: TRUE */
#define LL_OPTION_USEHOSTPRINTER       (13                  ) /* default: FALSE // use host printer via callback */
#define LL_OPTION_EXTENDEDEVALUATION   (14                  ) /* allows expressions in chevrons (amwin mode) */
#define LL_OPTION_TABREPRESENTATIONCODE (15                  ) /* default: 247 (0xf7) */
#define LL_OPTION_METRIC               (18                  ) /* default: depends on Windows defaults */
#define LL_OPTION_ADDVARSTOFIELDS      (19                  ) /* default: FALSE */
#define LL_OPTION_MULTIPLETABLELINES   (20                  ) /* default: TRUE */
#define LL_OPTION_CONVERTCRLF          (21                  ) /* default: FALSE */
#define LL_OPTION_WIZ_FILENEW          (22                  ) /* default: FALSE */
#define LL_OPTION_RETREPRESENTATIONCODE (23                  ) /* default: LL_CHAR_NEWLINE (182) */
#define LL_OPTION_PRVZOOM_PERC         (25                  ) /* initial preview zoom */
#define LL_OPTION_PRVRECT_LEFT         (26                  ) /* initial preview position */
#define LL_OPTION_PRVRECT_TOP          (27                  )
#define LL_OPTION_PRVRECT_WIDTH        (28                  )
#define LL_OPTION_PRVRECT_HEIGHT       (29                  )
#define LL_OPTION_STORAGESYSTEM        (30                  ) /* DEPRECATED. Do not change. 0=LX4-compatible, 1=STORAGE (default) */
#define LL_STG_COMPAT4                 (0                   )
#define LL_STG_STORAGE                 (1                   )
#define LL_OPTION_COMPRESSSTORAGE      (31                  ) /* 0, 1, 10..17 */
#define LL_STG_COMPRESS_THREADED       (0x00008000          )
#define LL_STG_COMPRESS_UNTHREADED     (0x00010000          )
#define LL_OPTION_NOPARAMETERCHECK     (32                  ) /* you need a bit more speed? */
#define LL_OPTION_NONOTABLECHECK       (33                  ) /* don't check on "NO_TABLEOBJECT" error. Default TRUE (don't check) */
#define LL_OPTION_DRAWFOOTERLINEONPRINT (34                  ) /* delay footerline printing to LlPrint(). Default FALSE */
#define LL_OPTION_PRVZOOM_LEFT         (35                  ) /* initial preview position in percent of screen */
#define LL_OPTION_PRVZOOM_TOP          (36                  )
#define LL_OPTION_PRVZOOM_WIDTH        (37                  )
#define LL_OPTION_PRVZOOM_HEIGHT       (38                  )
#define LL_OPTION_SPACEOPTIMIZATION    (40                  ) /* default: TRUE */
#define LL_OPTION_REALTIME             (41                  ) /* default: FALSE */
#define LL_OPTION_AUTOMULTIPAGE        (42                  ) /* default: TRUE */
#define LL_OPTION_USEBARCODESIZES      (43                  ) /* default: FALSE */
#define LL_OPTION_MAXRTFVERSION        (44                  ) /* default: 0xff00 */
#define LL_OPTION_VARSCASESENSITIVE    (46                  ) /* default: FALSE */
#define LL_OPTION_DELAYTABLEHEADER     (47                  ) /* default: TRUE */
#define LL_OPTION_OFNDIALOG_EXPLORER   (48                  ) /* DEPRECATED. Do not change. */
#define LL_OPTION_OFN_NOPLACESBAR      (0x40000000          )
#define LL_OPTION_EMFRESOLUTION        (49                  ) /* DEPRECATED. Do not change. */
#define LL_OPTION_SETCREATIONINFO      (50                  ) /* default: TRUE */
#define LL_OPTION_XLATVARNAMES         (51                  ) /* default: TRUE */
#define LL_OPTION_LANGUAGE             (52                  ) /* returns current language (r/o) */
#define LL_OPTION_PHANTOMSPACEREPRESENTATIONCODE (54                  ) /* default: LL_CHAR_PHANTOMSPACE */
#define LL_OPTION_LOCKNEXTCHARREPRESENTATIONCODE (55                  ) /* default: LL_CHAR_LOCK */
#define LL_OPTION_EXPRSEPREPRESENTATIONCODE (56                  ) /* default: LL_CHAR_EXPRSEP */
#define LL_OPTION_DEFPRINTERINSTALLED  (57                  ) /* r/o */
#define LL_OPTION_CALCSUMVARSONINVISIBLELINES (58                  ) /* default: FALSE - only default value if no preferences in project */
#define LL_OPTION_NOFOOTERPAGEWRAP     (59                  ) /* default: FALSE - only default value if no preferences in project */
#define LL_OPTION_IMMEDIATELASTPAGE    (64                  ) /* default: FALSE */
#define LL_OPTION_LCID                 (65                  ) /* default: LOCALE_USER_DEFAULT */
#define LL_OPTION_TEXTQUOTEREPRESENTATIONCODE (66                  ) /* default: 1 */
#define LL_OPTION_SCALABLEFONTSONLY    (67                  ) /* default: TRUE */
#define LL_OPTION_NOTIFICATIONMESSAGEHWND (68                  ) /* default: NULL (parent window handle) */
#define LL_OPTION_DEFDEFFONT           (69                  ) /* default: GetStockObject(ANSI_VAR_FONT) */
#define LL_OPTION_CODEPAGE             (70                  ) /* default: CP_ACP; set codepage to use for conversions. */
#define LL_OPTION_FORCEFONTCHARSET     (71                  ) /* default: FALSE; set font's charset to the codepage according to LL_OPTION_LCID. Default: FALSE */
#define LL_OPTION_COMPRESSRTF          (72                  ) /* default: TRUE; compress RTF text > 1024 bytes in project file */
#define LL_OPTION_ALLOW_LLX_EXPORTERS  (74                  ) /* default: TRUE; allow ILlXExport extensions */
#define LL_OPTION_SUPPORTS_PRNOPTSTR_EXPORT (75                  ) /* default: FALSE: hides "set to default" button in "export option" tab in designer */
#define LL_OPTION_DEBUGFLAG            (76                  )
#define LL_OPTION_SKIPRETURNATENDOFRTF (77                  ) /* default: FALSE */
#define LL_OPTION_INTERCHARSPACING     (78                  ) /* default: FALSE: allows character interspacing in case of block justify */
#define LL_OPTION_INCLUDEFONTDESCENT   (79                  ) /* default: TRUE */
#define LL_OPTION_RESOLUTIONCOMPATIBLETO9X (80                  ) /* DEPRECATED. default: FALSE */
#define LL_OPTION_USECHARTFIELDS       (81                  ) /* default: FALSE */
#define LL_OPTION_OFNDIALOG_NOPLACESBAR (82                  ) /* default: FALSE; do not use "Places" bar in NT2K? */
#define LL_OPTION_SKETCH_COLORDEPTH    (83                  ) /* default: 1 */
#define LL_OPTION_FINAL_TRUE_ON_LASTPAGE (84                  ) /* default: FALSE: internal use */
#define LL_OPTION_INTERCHARSPACING_FORCED (86                  ) /* default: FALSE: forces character interspacing calculation in TEXT objects (possibly dangerous and slow) */
#define LL_OPTION_RTFAUTOINCREMENT     (87                  ) /* default: FALSE, to increment RTF char pointer if nothing can be printed */
#define LL_OPTION_UNITS_DEFAULT        (88                  ) /* default: LL_OPTION_UNITS_SYSDEFAULT. Use for contols that query the units, where we need to return "sysdefault" also */
#define LL_OPTION_NO_MAPI              (89                  ) /* default: FALSE. Inhibit MAPI load for preview */
#define LL_OPTION_TOOLBARSTYLE         (90                  ) /* default: LL_OPTION_TOOLBARSTYLE_STANDARD|LL_OPTION_TOOLBARSTYLEFLAG_DOCKABLE */
#define LL_OPTION_TOOLBARSTYLE_STANDARD (0                   ) /* OFFICE97 alike style */
#define LL_OPTION_TOOLBARSTYLE_OFFICEXP (1                   ) /* DOTNET/OFFICE_XP alike style */
#define LL_OPTION_TOOLBARSTYLE_OFFICE2003 (2                   )
#define LL_OPTION_TOOLBARSTYLEMASK     (0x0f                )
#define LL_OPTION_TOOLBARSTYLEFLAG_GRADIENT (0x80                ) /* starting with XP, use gradient style */
#define LL_OPTION_TOOLBARSTYLEFLAG_DOCKABLE (0x40                ) /* dockable toolbars? */
#define LL_OPTION_TOOLBARSTYLEFLAG_CANCLOSE (0x20                ) /* internal use only */
#define LL_OPTION_MENUSTYLE            (91                  ) /* default: LL_OPTION_MENUSTYLE_STANDARD */
#define LL_OPTION_MENUSTYLE_STANDARD_WITHOUT_BITMAPS (0                   ) /* values: see CTL */
#define LL_OPTION_MENUSTYLE_STANDARD   (1                   )
#define LL_OPTION_MENUSTYLE_OFFICEXP   (2                   )
#define LL_OPTION_MENUSTYLE_OFFICE2003 (3                   )
#define LL_OPTION_RULERSTYLE           (92                  ) /* default: LL_OPTION_RULERSTYLE_FLAT */
#define LL_OPTION_RULERSTYLE_FLAT      (0x10                )
#define LL_OPTION_RULERSTYLE_GRADIENT  (0x80                )
#define LL_OPTION_STATUSBARSTYLE       (93                  )
#define LL_OPTION_STATUSBARSTYLE_STANDARD (0                   )
#define LL_OPTION_STATUSBARSTYLE_OFFICEXP (1                   )
#define LL_OPTION_STATUSBARSTYLE_OFFICE2003 (2                   )
#define LL_OPTION_TABBARSTYLE          (94                  )
#define LL_OPTION_TABBARSTYLE_STANDARD (0                   )
#define LL_OPTION_TABBARSTYLE_OFFICEXP (1                   )
#define LL_OPTION_TABBARSTYLE_OFFICE2003 (2                   )
#define LL_OPTION_DROPWINDOWSTYLE      (95                  )
#define LL_OPTION_DROPWINDOWSTYLE_STANDARD (0                   )
#define LL_OPTION_DROPWINDOWSTYLE_OFFICEXP (1                   )
#define LL_OPTION_DROPWINDOWSTYLE_OFFICE2003 (2                   )
#define LL_OPTION_DROPWINDOWSTYLEMASK  (0x0f                )
#define LL_OPTION_DROPWINDOWSTYLEFLAG_CANCLOSE (0x20                )
#define LL_OPTION_INTERFACEWRAPPER     (96                  ) /* returns IL<n>* */
#define LL_OPTION_FONTQUALITY          (97                  ) /* LOGFONT.lfQuality, default: DEFAULT_QUALITY */
#define LL_OPTION_FONTPRECISION        (98                  ) /* LOGFONT.lfOutPrecision, default: OUT_STRING_PRECIS */
#define LL_OPTION_UISTYLE              (99                  ) /* UI collection, w/o */
#define LL_OPTION_UISTYLE_STANDARD     (0                   ) /* 90=0x40, 91=1, 92=0x10, 93=0, 94=0, 95=0x20 */
#define LL_OPTION_UISTYLE_OFFICEXP     (1                   ) /* 90=0x41, 91=2, 92=0x10, 93=1, 94=1, 95=0x21 */
#define LL_OPTION_UISTYLE_OFFICE2003   (2                   ) /* 90=0x42, 91=3, 92=0x10, 93=2, 94=2, 95=0x22 */
#define LL_OPTION_NOFILEVERSIONUPGRADEWARNING (100                 ) /* default: FALSE */
#define LL_OPTION_UPDATE_FOOTER_ON_DATALINEBREAK_AT_FIRST_LINE (101                 ) /* default: FALSE */
#define LL_OPTION_ESC_CLOSES_PREVIEW   (102                 ) /* shall ESC close the preview window (default: FALSE) */
#define LL_OPTION_VIEWER_ASSUMES_TEMPFILE (103                 ) /* shall the viewer assume that the file is a temporary file (and not store values in it)? default TRUE */
#define LL_OPTION_CALC_USED_VARS       (104                 ) /* default: TRUE */
#define LL_OPTION_BOTTOMALIGNMENT_WIN9X_UNLIKE_NT (105                 ) /* default: TRUE */
#define LL_OPTION_NOPRINTJOBSUPERVISION (106                 ) /* default: TRUE */
#define LL_OPTION_CALC_SUMVARS_ON_PARTIAL_LINES (107                 ) /* default: FALSE */
#define LL_OPTION_BLACKNESS_SCM        (108                 ) /* default: 0 */
#define LL_OPTION_PROHIBIT_USERINTERACTION (109                 ) /* default: FALSE */
#define LL_OPTION_PERFMON_INSTALL      (110                 ) /* w/o, TRUE to install, FALSE to uninstall */
#define LL_OPTION_RESERVED111          (111                 )
#define LL_OPTION_VARLISTBUCKETCOUNT   (112                 ) /* applied to future jobs only, default 1000 */
#define LL_OPTION_MSFAXALLOWED         (113                 ) /* global flag - set at start of LL! Will allow/prohibit fax detection. Default: TRUE */
#define LL_OPTION_AUTOPROFILINGTICKS   (114                 ) /* global flag - set at start of LL! Activates LL's thread profiling */
#define LL_OPTION_PROJECTBACKUP        (115                 ) /* default: TRUE */
#define LL_OPTION_ERR_ON_FILENOTFOUND  (116                 ) /* default: FALSE */
#define LL_OPTION_NOFAXVARS            (117                 ) /* default: FALSE */
#define LL_OPTION_NOMAILVARS           (118                 ) /* default: FALSE */
#define LL_OPTION_PATTERNRESCOMPATIBILITY (119                 ) /* default: FALSE */
#define LL_OPTION_NODELAYEDVALUECACHING (120                 ) /* default: FALSE */
#define LL_OPTION_FEATURE              (1000                )
#define LL_OPTION_FEATURE_CLEARALL     (0                   )
#define LL_OPTION_FEATURE_SUPPRESS_JPEG_DISPLAY (1                   )
#define LL_OPTION_FEATURE_SUPPRESS_JPEG_CREATION (2                   )
#define LL_OPTION_VARLISTDISPLAY       (121                 ) /* default: LL_OPTION_VARLISTDISPLAY_VARSORT_DECLARATIONORDER | LL_OPTION_VARLISTDISPLAY_FOLDERPOS_DECLARATIONORDER, see also LL_OPTION_SORTVARIABLES */
#define LL_OPTION_VARLISTDISPLAY_VARSORT_DECLARATIONORDER (0x0000              )
#define LL_OPTION_VARLISTDISPLAY_VARSORT_ALPHA (0x0001              )
#define LL_OPTION_VARLISTDISPLAY_VARSORT_MASK (0x000f              )
#define LL_OPTION_VARLISTDISPLAY_FOLDERPOS_DECLARATIONORDER (0x0000              )
#define LL_OPTION_VARLISTDISPLAY_FOLDERPOS_ALPHA (0x0010              ) /* only if LL_OPTION_VARLISTDISPLAY_VARSORT_ALPHA is set */
#define LL_OPTION_VARLISTDISPLAY_FOLDERPOS_TOP (0x0020              )
#define LL_OPTION_VARLISTDISPLAY_FOLDERPOS_BOTTOM (0x0030              )
#define LL_OPTION_VARLISTDISPLAY_FOLDERPOS_MASK (0x00f0              )
#define LL_OPTION_WORKAROUND_RTFBUG_EMPTYFIRSTPAGE (122                 )
#define LL_OPTION_FORMULASTRINGCOMPARISONS_CASESENSITIVE (123                 ) /* default: TRUE */
#define LL_OPTION_FIELDS_IN_PROJECTPARAMETERS (124                 ) /* default: FALSE */
#define LL_OPTION_CHECKWINDOWTHREADEDNESS (125                 ) /* default: FALSE */
#define LL_OPTION_ISUSED_WILDCARD_AT_START (126                 ) /* default: FALSE */
#define LL_OPTION_ROOT_MUST_BE_MASTERTABLE (127                 ) /* default: FALSE */
#define LL_OPTION_DLLTYPE              (128                 ) /* r/o */
#define LL_OPTION_DLLTYPE_32BIT        (0x0001              )
#define LL_OPTION_DLLTYPE_64BIT        (0x0002              )
#define LL_OPTION_DLLTYPE_BITMASK      (0x000f              )
#define LL_OPTION_DLLTYPE_SDBCS        (0x0010              )
#define LL_OPTION_DLLTYPE_UNICODE      (0x0020              )
#define LL_OPTION_DLLTYPE_CHARSET      (0x00f0              )
#define LL_OPTION_HLIBRARY             (129                 ) /* r/o */
#define LL_OPTION_INVERTED_PAGEORIENTATION (130                 ) /* default: FALSE */
#define LL_OPTION_ENABLE_STANDALONE_DATACOLLECTING_OBJECTS (131                 ) /* default: FALSE */
#define LL_OPTION_USERVARS_ARE_CODESNIPPETS (132                 ) /* default: FALSE */
#define LL_OPTION_STORAGE_ADD_SUMMARYINFORMATION (133                 ) /* default: FALSE */
#define LL_OPTION_INCREMENTAL_PREVIEW  (135                 ) /* default: TRUE */
#define LL_OPTION_RELAX_AT_SHUTDOWN    (136                 ) /* default: FALSE */
#define LL_OPTION_NOPRINTERPATHCHECK   (137                 ) /* default: FALSE */
#define LL_OPTION_SUPPORT_HUGESTORAGEFS (138                 ) /* default: FALSE */
#define LL_OPTION_NOAUTOPROPERTYCORRECTION (139                 ) /* default: FALSE */
#define LL_OPTION_NOVARLISTRESET_ON_RESETPROJECTSTATE (140                 ) /* default: FALSE; */
#define LL_OPTION_DESIGNERPREVIEWPARAMETER (141                 ) /* default: NULL */
#define LL_OPTION_RESERVED142          (142                 )
#define LL_OPTION_DESIGNEREXPORTPARAMETER (143                 ) /* default: NULL */
#define LL_OPTION_DESIGNERPRINT_SINGLETHREADED (144                 ) /* default: FALSE */
#define LL_OPTION_ALLOW_COMMENTS_IN_FORMULA (145                 ) /* default: TRUE */
#define LL_OPTION_USE_MLANG_LINEBREAKALGORITHM (146                 ) /* default: FALSE (would use MLANG to calculate the line break algorithm) */
#define LL_OPTION_USE_JPEG_OPTIMIZATION (147                 ) /* default: TRUE */
#define LL_OPTION_ENABLE_IMAGESMOOTHING (148                 ) /* default: TRUE (uses GDIPLUS - no smoothing on Win2000/98 if not GDIPLUS installed! Right now, applies only to JPEG.) */
#define LL_OPTION_MAXRTFVERSION_AVAILABLE (159                 ) /* r/o */
#define LL_OPTION_CONDREPRESENTATIONCODES_LIKE_ANSI (160                 ) /* default: FALSE */
#define LL_OPTION_NULL_IS_NONDESTRUCTIVE (161                 ) /* default: FALSE */
#define LL_OPTION_DRILLDOWNPARAMETER   (162                 ) /* default: NULL */
#define LL_OPTION_ROUNDINGSTRATEGY     (163                 ) /* default: LL_ROUNDINGSTRATEGY_ARITHMETIC_SYMMETRIC */
#define LL_ROUNDINGSTRATEGY_BANKERSROUNDING (0                   )
#define LL_ROUNDINGSTRATEGY_ARITHMETIC_SYMMETRIC (1                   )
#define LL_OPTION_RESERVED164          (164                 )
#define LL_OPTION_RESERVED165          (165                 )
#define LL_OPTION_PICTURE_TRANSPARENCY_IS_WHITE (166                 ) /* default: false (transparent) */
#define LL_OPTION_FLOATPRECISION       (167                 ) /* global (not job specific!). Default: 0 (192 bit mantissa, 32 bit exponent) */
#define LL_OPTION_SUPPRESS_LRUENTRY    (168                 )
#define LL_OPTION_FORCEFIRSTGROUPHEADER (169                 ) /* default: false (group match string is an empty string) */
#define LL_OPTION_SUPPORT_PDFINPUTFIELDS (170                 ) /* PDF 3.0 supports text objects and check boxes as input objects - default: true */
#define LL_OPTION_ENHANCED_SKIPRETURNATENDOFRTF (171                 ) /* default: false. */
#define LL_OPTION_HIERARCHICALDATASOURCE (172                 ) /* default: false */
#define LL_OPTION_FORCE_HEADER_EVEN_ON_LARGE_FOOTERLINES (173                 ) /* default: false */
#define LL_OPTION_PRINTERDEVICEOPTIMIZATION (174                 ) /* default: false */
#define LL_OPTION_RTFHEIGHTSCALINGPERCENTAGE (175                 ) /* default: 100 */
#define LL_OPTION_FORCE_DEFAULT_PRINTER_IN_PREVIEW (176                 ) /* default: false */
#define LL_OPTION_SAVE_PROJECT_IN_UTF8 (178                 ) /* BOOL, default 0 (meaning: project is saved as UNICODE if A API is not used) */
#define LL_DRILLDOWNFILTERSTRATEGY_ALLOW_ONLY_SUBTABLES (0                   )
#define LL_DRILLDOWNFILTERSTRATEGY_ALLOW_ALL_TABLES (1                   )
#define LL_DRILLDOWNFILTERSTRATEGY_ALLOW_SUBTABLES_AND_UNRELATED (2                   )
#define LL_DRILLDOWNFILTERSTRATEGY_ALLOW_SUBTABLES_AND_USERDEFINED (3                   )
#define LL_DRILLDOWNFILTERSTRATEGY_MASK (0x0f                )
#define LL_DRILLDOWNFILTERFLAG_OFFER_BASERECORD_AS_VARIABLES (0x10                )
#define LL_OPTION_DRILLDOWN_DATABASEFILTERING (179                 ) /* default: 0 (filter all except subtables of the base table: LL_DRILLDOWNFILTERSTRATEGY_ALLOW_ONLY_SUBTABLES) */
#define LL_OPTION_SUPPRESS_TASKBARBUTTON_PROGRESSSTATE (180                 ) /* default: false */
#define LL_OPTION_PRINTDLG_DEVICECHANGE_KEEPS_DEVMODESETTINGS (181                 ) /* default: true */
#define LL_OPTION_DRILLDOWN_SUPPORTS_EMBEDDING (182                 ) /* default: true */
#define LL_OPTIONSTR_LABEL_PRJEXT      (0                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_LABEL_PRVEXT      (1                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_LABEL_PRNEXT      (2                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_CARD_PRJEXT       (3                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_CARD_PRVEXT       (4                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_CARD_PRNEXT       (5                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_LIST_PRJEXT       (6                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_LIST_PRVEXT       (7                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_LIST_PRNEXT       (8                   ) /* internal... (compatibility to L6) */
#define LL_OPTIONSTR_LLXPATHLIST       (12                  )
#define LL_OPTIONSTR_SHORTDATEFORMAT   (13                  )
#define LL_OPTIONSTR_DECIMAL           (14                  ) /* decimal point, default: system */
#define LL_OPTIONSTR_THOUSAND          (15                  ) /* thousands separator, default: system */
#define LL_OPTIONSTR_CURRENCY          (16                  ) /* currency symbol, default: system */
#define LL_OPTIONSTR_EXPORTS_AVAILABLE (17                  ) /* r/o */
#define LL_OPTIONSTR_EXPORTS_ALLOWED   (18                  )
#define LL_OPTIONSTR_DEFDEFFONT        (19                  ) /* in "{(r,g,b),size,<logfont>}" */
#define LL_OPTIONSTR_EXPORTFILELIST    (20                  )
#define LL_OPTIONSTR_VARALIAS          (21                  ) /* "<local>=<global>" */
#define LL_OPTIONSTR_MAILTO            (24                  ) /* default TO: address for mailing from viewer */
#define LL_OPTIONSTR_MAILTO_CC         (25                  ) /* default CC: address for mailing from viewer */
#define LL_OPTIONSTR_MAILTO_BCC        (26                  ) /* default BCC: address for mailing from viewer */
#define LL_OPTIONSTR_MAILTO_SUBJECT    (27                  ) /* default subject for mailing from viewer */
#define LL_OPTIONSTR_SAVEAS_PATH       (28                  ) /* default filename for saving the LL file from viewer */
#define LL_OPTIONSTR_LABEL_PRJDESCR    (29                  ) /* "Etikett" ... */
#define LL_OPTIONSTR_CARD_PRJDESCR     (30                  )
#define LL_OPTIONSTR_LIST_PRJDESCR     (31                  )
#define LL_OPTIONSTR_LLFILEDESCR       (32                  ) /* "Vorschau-Datei" */
#define LL_OPTIONSTR_PROJECTPASSWORD   (33                  ) /* w/o, of course :) */
#define LL_OPTIONSTR_FAX_RECIPNAME     (34                  )
#define LL_OPTIONSTR_FAX_RECIPNUMBER   (35                  )
#define LL_OPTIONSTR_FAX_QUEUENAME     (36                  )
#define LL_OPTIONSTR_FAX_SENDERNAME    (37                  )
#define LL_OPTIONSTR_FAX_SENDERCOMPANY (38                  )
#define LL_OPTIONSTR_FAX_SENDERDEPT    (39                  )
#define LL_OPTIONSTR_FAX_SENDERBILLINGCODE (40                  )
#define LL_OPTIONSTR_FAX_AVAILABLEQUEUES (42                  ) /* r/o (Tab-separated) [job can be -1 or a valid job] */
#define LL_OPTIONSTR_LOGFILEPATH       (43                  )
#define LL_OPTIONSTR_LICENSINGINFO     (44                  ) /* w/o, SERNO to define licensing state */
#define LL_OPTIONSTR_PRINTERALIASLIST  (45                  ) /* multiple "PrnOld=PrnNew1[;PrnNew2[;...]]", erase with NULL or "" */
#define LL_OPTIONSTR_PREVIEWFILENAME   (46                  ) /* path of preview file (directory will be overridden by LlPreviewSetTempPath(), if given) */
#define LL_OPTIONSTR_EXPORTS_ALLOWED_IN_PREVIEW (47                  ) /* set in preview file */
#define LL_OPTIONSTR_HELPFILENAME      (48                  )
#define LL_OPTIONSTR_NULLVALUE         (49                  ) /* string which represents the NULL value */
#define LL_OPTIONSTR_DEFAULT_EXPORT    (50                  ) /* default export medium for new projects */
#define LL_OPTIONSTR_ORIGINALPROJECTFILENAME (51                  ) /* fixup project path for relative paths in realdata preview/export in designer */
#define LL_OPTIONSTR_HIERARCHICALDATASOURCE_ROOT (52                  ) /* internal use only */
#define LL_SYSCOMMAND_MINIMIZE         (-1                  )
#define LL_SYSCOMMAND_MAXIMIZE         (-2                  )
#define LL_DLGBOXMODE_3DBUTTONS        (0x8000              ) /* 'or'ed */
#define LL_DLGBOXMODE_3DFRAME2         (0x4000              ) /* 'OR'ed */
#define LL_DLGBOXMODE_3DFRAME          (0x1000              ) /* 'OR'ed */
#define LL_DLGBOXMODE_NOBITMAPS        (0x2000              ) /* 'or'ed */
#define LL_DLGBOXMODE_DONTCARE         (0x0000              ) /* load from INI */
#define LL_DLGBOXMODE_SAA              (0x0001              )
#define LL_DLGBOXMODE_ALT1             (0x0002              )
#define LL_DLGBOXMODE_ALT2             (0x0003              )
#define LL_DLGBOXMODE_ALT3             (0x0004              )
#define LL_DLGBOXMODE_ALT4             (0x0005              )
#define LL_DLGBOXMODE_ALT5             (0x0006              )
#define LL_DLGBOXMODE_ALT6             (0x0007              )
#define LL_DLGBOXMODE_ALT7             (0x0008              )
#define LL_DLGBOXMODE_ALT8             (0x0009              ) /* Win95 */
#define LL_DLGBOXMODE_ALT9             (0x000A              ) /* Win98 */
#define LL_DLGBOXMODE_ALT10            (0x000B              ) /* Win98 with gray/color button bitmaps like IE4 */
#define LL_DLGBOXMODE_TOOLTIPS98       (0x0800              ) /* DEPRECATED. Do not change. */
#define LL_CTL_ADDTOSYSMENU            (0x00000004          ) /* from CTL */
#define LL_CTL_ALSOCHILDREN            (0x00000010          )
#define LL_CTL_CONVERTCONTROLS         (0x00010000          )
#define LL_GROUP_ALWAYSFOOTER          (0x40000000          )
#define LL_PRINTERCONFIG_SAVE          (1                   )
#define LL_PRINTERCONFIG_RESTORE       (2                   )
#define LL_RTFTEXTMODE_RTF             (0x0000              )
#define LL_RTFTEXTMODE_PLAIN           (0x0001              )
#define LL_RTFTEXTMODE_EVALUATED       (0x0000              )
#define LL_RTFTEXTMODE_RAW             (0x0002              )
#define LL_ERR_BAD_JOBHANDLE           (-1                  ) /* bad jobhandle */
#define LL_ERR_TASK_ACTIVE             (-2                  ) /* LlDefineLayout() only once in a job */
#define LL_ERR_BAD_OBJECTTYPE          (-3                  ) /* nObjType must be one of the allowed values (obsolete constant) */
#define LL_ERR_BAD_PROJECTTYPE         (-3                  ) /* nObjType must be one of the allowed values */
#define LL_ERR_PRINTING_JOB            (-4                  ) /* print job not opened, no print object */
#define LL_ERR_NO_BOX                  (-5                  ) /* LlPrintSetBoxText(...) called when no abort box exists! */
#define LL_ERR_ALREADY_PRINTING        (-6                  ) /* the current operation cannot be performed while a print job is open */
#define LL_ERR_NOT_YET_PRINTING        (-7                  ) /* LlPrintGetOptionString... */
#define LL_ERR_NO_PROJECT              (-10                 ) /* object with requested name does not exist (former ERR_NO_OBJECT) */
#define LL_ERR_NO_PRINTER              (-11                 ) /* printer couldn't be opened */
#define LL_ERR_PRINTING                (-12                 ) /* error while printing */
#define LL_ERR_EXPORTING               (-13                 ) /* error while exporting */
#define LL_ERR_NEEDS_VB                (-14                 ) /* '11...' needs VB.EXE */
#define LL_ERR_BAD_PRINTER             (-15                 ) /* PrintOptionsDialog(): no printer available */
#define LL_ERR_NO_PREVIEWMODE          (-16                 ) /* Preview functions: not in preview mode */
#define LL_ERR_NO_PREVIEWFILES         (-17                 ) /* PreviewDisplay(): no file found */
#define LL_ERR_PARAMETER               (-18                 ) /* bad parameter (usually NULL pointer) */
#define LL_ERR_BAD_EXPRESSION          (-19                 ) /* bad expression in LlExprEvaluate() and LlExprType() */
#define LL_ERR_BAD_EXPRMODE            (-20                 ) /* bad expression mode (LlSetExpressionMode()) */
#define LL_ERR_NO_TABLE                (-21                 ) /* not used */
#define LL_ERR_CFGNOTFOUND             (-22                 ) /* on LlPrintStart(), LlPrintWithBoxStart() [not found] */
#define LL_ERR_EXPRESSION              (-23                 ) /* on LlPrintStart(), LlPrintWithBoxStart() */
#define LL_ERR_CFGBADFILE              (-24                 ) /* on LlPrintStart(), LlPrintWithBoxStart() [read error, bad format] */
#define LL_ERR_BADOBJNAME              (-25                 ) /* on LlPrintEnableObject() - not a ':' at the beginning */
#define LL_ERR_NOOBJECT                (-26                 ) /* on LlPrintEnableObject() - "*" and no object in project */
#define LL_ERR_UNKNOWNOBJECT           (-27                 ) /* on LlPrintEnableObject() - object with that name not existing */
#define LL_ERR_NO_TABLEOBJECT          (-28                 ) /* LlPrint...Start() and no list in Project, or: */
#define LL_ERR_NO_OBJECT               (-29                 ) /* LlPrint...Start() and no object in project */
#define LL_ERR_NO_TEXTOBJECT           (-30                 ) /* LlPrintGetTextCharsPrinted() and no printable text in Project! */
#define LL_ERR_UNKNOWN                 (-31                 ) /* LlPrintIsVariableUsed(), LlPrintIsFieldUsed() */
#define LL_ERR_BAD_MODE                (-32                 ) /* LlPrintFields(), LlPrintIsFieldUsed() called on non-OBJECT_LIST */
#define LL_ERR_CFGBADMODE              (-33                 ) /* on LlDefineLayout(), LlPrint...Start(): file is in wrong expression mode */
#define LL_ERR_ONLYWITHONETABLE        (-34                 ) /* on LlDefinePageSeparation(), LlDefineGrouping() */
#define LL_ERR_UNKNOWNVARIABLE         (-35                 ) /* on LlGetVariableContents() */
#define LL_ERR_UNKNOWNFIELD            (-36                 ) /* on LlGetFieldContents() */
#define LL_ERR_UNKNOWNSORTORDER        (-37                 ) /* on LlGetFieldContents() */
#define LL_ERR_NOPRINTERCFG            (-38                 ) /* on LlPrintCopyPrinterConfiguration() - no or bad file */
#define LL_ERR_SAVEPRINTERCFG          (-39                 ) /* on LlPrintCopyPrinterConfiguration() - file could not be saved */
#define LL_ERR_RESERVED                (-40                 ) /* function not yet implemeted */
#define LL_ERR_NOVALIDPAGES            (-41                 ) /* could also be that 16 bit Viewer tries to open 32bit-only storage */
#define LL_ERR_NOTINHOSTPRINTERMODE    (-42                 ) /* cannot be done in Host Printer Mode (LlSetPrinterInPrinterFile()) */
#define LL_ERR_NOTFINISHED             (-43                 ) /* appears when a project reset() is done, but the table not finished */
#define LL_ERR_BUFFERTOOSMALL          (-44                 ) /* LlXXGetOptionStr() */
#define LL_ERR_BADCODEPAGE             (-45                 ) /* LL_OPTION_CODEPAGE */
#define LL_ERR_CANNOTCREATETEMPFILE    (-46                 ) /* cannot create temporary file */
#define LL_ERR_NODESTINATION           (-47                 ) /* no valid export destination */
#define LL_ERR_NOCHART                 (-48                 ) /* no chart control present */
#define LL_ERR_TOO_MANY_CONCURRENT_PRINTJOBS (-49                 ) /* WebServer: not enough print process licenses */
#define LL_ERR_BAD_WEBSERVER_LICENSE   (-50                 ) /* WebServer: bad license file */
#define LL_ERR_NO_WEBSERVER_LICENSE    (-51                 ) /* WebServer: no license file */
#define LL_ERR_INVALIDDATE             (-52                 ) /* LlSystemTimeFromLocaleString(): date not valid! */
#define LL_ERR_DRAWINGNOTFOUND         (-53                 ) /* only if LL_OPTION_ERR_ON_FILENOTFOUND set */
#define LL_ERR_NOUSERINTERACTION       (-54                 ) /* a call is used which would show a dialog, but LL is in Webserver mode */
#define LL_ERR_BADDATABASESTRUCTURE    (-55                 ) /* the project that is loading has a table that is not supported by the database */
#define LL_ERR_UNKNOWNPROPERTY         (-56                 )
#define LL_ERR_INVALIDOPERATION        (-57                 )
#define LL_ERR_PROPERTY_ALREADY_DEFINED (-58                 )
#define LL_ERR_CFGFOUND                (-59                 ) /* on LlPrjOpen() with CREATE_NEW disposition, or of project file is r/o and access flag is r/w */
#define LL_ERR_SAVECFG                 (-60                 ) /* error while saving (LlProjectSave()) */
#define LL_ERR_WRONGTHREAD             (-61                 ) /* internal (.NET) */
#define LL_ERR_USER_ABORTED            (-99                 ) /* user aborted printing */
#define LL_ERR_BAD_DLLS                (-100                ) /* DLLs not up to date (CTL, DWG, UTIL) */
#define LL_ERR_NO_LANG_DLL             (-101                ) /* no or out-of-date language resource DLL */
#define LL_ERR_NO_MEMORY               (-102                ) /* out of memory */
#define LL_ERR_EXCEPTION               (-104                ) /* there was a GPF during the API execution. Any action that follows might cause problems! */
#define LL_ERR_LICENSEVIOLATION        (-105                ) /* your license does not allow this call (see LL_OPTIONSTR_LICENSINGINFO) */
#define LL_ERR_NOT_SUPPORTED_IN_THIS_OS (-106                ) /* the OS does not support this function */
#define LL_WRN_ISNULL                  (-995                ) /* LlExprEvaluate[Var]() */
#define LL_WRN_TABLECHANGE             (-996                )
#define LL_WRN_PRINTFINISHED           (-997                ) /* LlRTFDisplay() */
#define LL_WRN_REPEAT_DATA             (-998                ) /* notification: page is full, prepare for next page */
#define LL_CHAR_TEXTQUOTE              (1                   )
#define LL_CHAR_PHANTOMSPACE           (2                   )
#define LL_CHAR_LOCK                   (3                   )
#define LL_CHAR_NEWLINE                (182                 ) /* "¶" */
#define LL_CHAR_EXPRSEP                (164                 ) /* "¤" */
#define LL_CHAR_TAB                    (247                 ) /* "÷" */
#define LL_CHAR_EAN128NUL              (255                 )
#define LL_CHAR_EAN128FNC1             (254                 )
#define LL_CHAR_EAN128FNC2             (253                 )
#define LL_CHAR_EAN128FNC3             (252                 )
#define LL_CHAR_EAN128FNC4             (251                 )
#define LL_CHAR_CODE93NUL              (255                 )
#define LL_CHAR_CODE93EXDOLLAR         (254                 )
#define LL_CHAR_CODE93EXPERC           (253                 )
#define LL_CHAR_CODE93EXSLASH          (252                 )
#define LL_CHAR_CODE93EXPLUS           (251                 )
#define LL_CHAR_CODE39NUL              (255                 )
#define LL_DLGEXPR_VAREXTBTN_ENABLE    (0x00000001          ) /* callback for simple Wizard extension */
#define LL_DLGEXPR_VAREXTBTN_DOMODAL   (0x00000002          )
#define LL_LLX_EXTENSIONTYPE_EXPORT    (1                   )
#define LL_LLX_EXTENSIONTYPE_BARCODE   (2                   )
#define LL_LLX_EXTENSIONTYPE_OBJECT    (3                   ) /* nyi */
#define LL_LLX_EXTENSIONTYPE_WIZARD    (4                   ) /* nyi */
#define LL_DECLARECHARTROW_FOR_OBJECTS (0x00000001          )
#define LL_DECLARECHARTROW_FOR_TABLECOLUMNS (0x00000002          ) /* body only */
#define LL_DECLARECHARTROW_FOR_TABLECOLUMNS_FOOTERS (0x00000004          )
#define LL_GETCHARTOBJECTCOUNT_CHARTOBJECTS (1                   )
#define LL_GETCHARTOBJECTCOUNT_CHARTOBJECTS_BEFORE_TABLE (2                   )
#define LL_GETCHARTOBJECTCOUNT_CHARTCOLUMNS (3                   ) /* body only */
#define LL_GETCHARTOBJECTCOUNT_CHARTCOLUMNS_FOOTERS (4                   )
#define LL_VARIANTFLAG_NEUTRAL         (0x00000000          )
#define LL_VARIANTFLAG_USE_JULIAN_DATE (0x00000001          )
#define LL_GRIPT_DIM_SCM               (1                   )
#define LL_GRIPT_DIM_PERC              (2                   )
#define LL_PARAMETERFLAG_PUBLIC        (0x00000000          )
#define LL_PARAMETERFLAG_SAVEDEFAULT   (0x00010000          )
#define LL_PARAMETERFLAG_PRIVATE       (0x40000000          )
#define LL_PARAMETERFLAG_FORMULA       (0x00000000          )
#define LL_PARAMETERFLAG_VALUE         (0x20000000          )
#define LL_PARAMETERFLAG_GLOBAL        (0x00000000          )
#define LL_PARAMETERFLAG_LOCAL         (0x10000000          )
#define LL_PARAMETERFLAG_MASK          (0xffff0000          )
#define LL_PARAMETERTYPE_USER          (0                   )
#define LL_PARAMETERTYPE_FAX           (1                   )
#define LL_PARAMETERTYPE_MAIL          (2                   )
#define LL_PARAMETERTYPE_LLINTERNAL    (4                   )
#define LL_PARAMETERTYPE_MASK          (0x0000000f          )
#define LL_LOCCONVERSION_LCID          (0                   )
#define LL_LOCCONVERSION_COUNTRYPREFIX (1                   )
#define LL_LOCCONVERSION_COUNTRYISONAME (2                   )
#define LL_LOCCONVERSION_DIALPREFIX    (3                   ) /* Not yet implemented */
#define LL_PRJOPEN_AM_READWRITE        (0x40000000          )
#define LL_PRJOPEN_AM_READONLY         (0x00000000          ) /* default */
#define LL_PRJOPEN_AM_MASK             (0x40000000          )
#define LL_PRJOPEN_CD_OPEN_EXISTING    (0x00000000          ) /* fails if it does not exist - default */
#define LL_PRJOPEN_CD_CREATE_ALWAYS    (0x10000000          ) /* open (but do not read contents) if exists, create if not */
#define LL_PRJOPEN_CD_CREATE_NEW       (0x20000000          ) /* fails if already exists */
#define LL_PRJOPEN_CD_OPEN_ALWAYS      (0x30000000          ) /* open (and load) if exists, create if not */
#define LL_PRJOPEN_CD_MASK             (0x30000000          ) /* fails if it does not exist */
#define LL_PRJOPEN_EM_IGNORE_FORMULAERRORS (0x08000000          )
#define LL_PRJOPEN_EM_MASK             (0x08000000          )
#define LL_PRJOPEN_FLG_NOINITPRINTER   (0x04000000          )
#define LL_PRJOPEN_FLG_NOOBJECTLOAD    (0x02000000          )
#define LL_ASSOCIATEPREVIEWCONTROLFLAG_DELETE_ON_CLOSE (0x0001              )
#define LL_ASSOCIATEPREVIEWCONTROLFLAG_HANDLE_IS_ATTACHINFO (0x0002              )
#define LL_ASSOCIATEPREVIEWCONTROLFLAG_PRV_REPLACE (0x0000              )
#define LL_ASSOCIATEPREVIEWCONTROLFLAG_PRV_ADD_TO_CONTROL_STACK (0x0004              )
#define LL_ASSOCIATEPREVIEWCONTROLFLAG_PRV_ADD_TO_CONTROL_IN_TAB (0x0008              )
#define LL_ASSOCIATEPREVIEWCONTROLMASK_ATTACHLOCATION (0x003c              )
#define LL_DESFILEOPEN_OPEN_EXISTING   (0x00000000          ) /* fails if it does not exist - default */
#define LL_DESFILEOPEN_CREATE_ALWAYS   (0x10000000          ) /* open (but do not read contents) if exists, create if not */
#define LL_DESFILEOPEN_CREATE_NEW      (0x20000000          ) /* fails if already exists */
#define LL_DESFILEOPEN_OPEN_ALWAYS     (0x30000000          ) /* open (and load) if exists, create if not */
#define LL_DESFILEOPEN_OPEN_IMPORT     (0x40000000          ) /* fails if it does not exist - only imports data */
#define LL_DESFILEOPEN_OPENMASK        (0x70000000          )
#define LL_DESFILEOPENFLAG_SUPPRESS_SAVEDIALOG (0x00000001          )
#define LL_DESFILEOPENFLAG_SUPPRESS_SAVE (0x00000002          )
#define LL_DESFILESAVE_DEFAULT         (0x00000000          ) /* default */
#define LLDESADDACTIONFLAG_ADD_TO_TOOLBAR (0x20000000          )
#define LLDESADDACTION_MENUITEM_APPEND (0x00000000          )
#define LLDESADDACTION_MENUITEM_INSERT (0x10000000          )
#define LLDESADDACTION_MENUITEM_POSITIONMASK (0x10000000          )
#define LLDESADDACTION_ACCEL_CONTROL   (0x00010000          )
#define LLDESADDACTION_ACCEL_SHIFT     (0x00020000          )
#define LLDESADDACTION_ACCEL_ALT       (0x00040000          )
#define LLDESADDACTION_ACCEL_VIRTKEY   (0x00080000          )
#define LLDESADDACTION_ACCEL_KEYMODIFIERMASK (0x000f0000          )
#define LLDESADDACTION_ACCEL_KEYCODEMASK (0x0000ffff          )
#define LL_DESIGNEROPTSTR_PROJECTFILENAME (1                   )
#define LL_DESIGNEROPTSTR_WORKSPACETITLE (2                   )
#define LL_DESIGNEROPTSTR_PROJECTDESCRIPTION (3                   )
#define LL_USEDIDENTIFIERSFLAG_VARIABLES (0x0001              )
#define LL_USEDIDENTIFIERSFLAG_FIELDS  (0x0002              )
#define LL_USEDIDENTIFIERSFLAG_CHARTFIELDS (0x0004              )
#define LL_USEDIDENTIFIERSFLAG_TABLES  (0x0008              )
#define LL_USEDIDENTIFIERSFLAG_RELATIONS (0x0010              )
#define LL_TEMPFILENAME_DEFAULT        (0x0000              )
#define LL_TEMPFILENAME_ENSURELONGPATH (0x0001              )

/*--- function declaration ---*/

#if !defined(_RC_INVOKED_) && !defined(RC_INVOKED)

#if defined(_WIN64)
#  pragma pack(push,8)
# else // _WIN64
#  if __WATCOMC__ > 1000 || _MSC_VER >= 1400 /* Watcom C++ >= 10.x or MSC >= 2005 */
#    pragma pack(push,4)
#   elif __BORLANDC__ /* Borland C++ */
#    pragma option -a4
#   else
#    pragma pack(4) /* MS, Watcom <= 10.0, ... */
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IMPLEMENTATION
  #define extern /* */
  static HINSTANCE g_hDLLLL15 = NULL;
  static INT       g_nDLLLL15Counter = 0;
#endif


 #ifndef IMPLEMENTATION
 typedef struct
  {
  UINT      _nSize;     // size of the structure
  LPARAM     _lParam;    // parameter (most likely address of structure)
  LPARAM     _lReply;    // reply (defaults to 0)
   UINT_PTR    _lUserParameter;        // user parameter
  } scLlCallback, *PSCLLCALLBACK;
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  HWND _hWnd;     // L&L mainframe wnd
  UINT _nTotal;    // total count of objects
  UINT _nCurrent;    // current object number (0=start,...,total=end)
  UINT _nJob;     // LL_METERINFO_... constants
  } scLlMeterInfo, *PSCLLMETERINFO;
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  LPCSTR _pszContents;   // contents of the parameter string (do not overwrite!!!)
  BOOL _bEvaluate;    // TRUE on evaluation, FALSE on syntax check (fill _bError, _szError)!
   CHAR _szNewValue[0x4000+1]; // new value
   UINT _bError;    // FALSE
  CHAR _szError[128];   // error text
  } scLlExtFctA;
 
 typedef struct
   {
   UINT    _nSize;
   LPCWSTR _pszContents;
   BOOL   _bEvaluate;
   WCHAR   _szNewValue[0x4000+1];
   UINT   _bError;
   WCHAR   _szError[128];
   } scLlExtFctW;
 #if defined(UNICODE)
   typedef scLlExtFctW scLlExtFct, *PSCLLEXTFCT;
 #else
   typedef scLlExtFctA scLlExtFct, *PSCLLEXTFCT;
 #endif
 
 typedef struct     // internal use (same struct as BASIC struct)
  {
  UINT  _nSize;     // size of the structure
  LPVOID _pszContents;   // contents of the parameter string (do not overwrite!!!)
  BOOL _bEvaluate;    // TRUE on evaluation, FALSE on syntax check (fill _bError, _szError)!
   LPVOID _szNewValue;   // new value
   UINT _bError;    // FALSE
  LPVOID _szError;    // error text
  } scLlExtFctXXX, *PSCLLEXTFCTXXX;
 
 #if defined(USE_OCX)
  typedef struct     // internal use (same struct as BASIC struct)
  {
  UINT  _nSize;     // size of the structure
  LPVOID _pszContents;   // contents of the parameter string (do not overwrite!!!)
  BOOL _bEvaluate;    // TRUE on evaluation, FALSE on syntax check (fill _bError, _szError)!
  BSTR _szNewValue;   // new value
  UINT _bError;    // FALSE
  BSTR _szError;    // error text
  } scLlExtFctXXX_B, *PSCLLEXTFCTXXX_B;
 #endif
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  LPCSTR _pszName;    // name of the object
  INT  _nType;     // see LL_OBJ_... values
  BOOL _bPreDraw;    // TRUE on call before draw, FALSE on call afterwards
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // RECT in paint units (mm/10 or inch/100)
  } scLlObjectA;
 typedef struct
   {
   UINT  _nSize;
   LPCWSTR _pszName;
   INT _nType;
   BOOL _bPreDraw;
   HDC _hRefDC;
   HDC _hPaintDC;
   RECT _rcPaint;
   } scLlObjectW;
 #if defined(UNICODE)
   typedef scLlObjectW scLlObject, *PSCLLOBJECT;
 #else
   typedef scLlObjectA scLlObject, *PSCLLOBJECT;
 #endif
 
 #if defined(USE_OCX)
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  BSTR _pszName;    // name of the object
  INT  _nType;     // see LL_OBJ_... values
  BOOL _bPreDraw;    // TRUE on call before draw, FALSE on call afterwards
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // RECT in paint units (mm/10 or inch/100)
  } scLlObject_B, *PSCLLOBJECT_B;
 #endif
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  BOOL _bDesignerPreview;  // flag if in Preview Mode or not
  BOOL _bPreDraw;    // TRUE on call before draw, FALSE on call afterwards
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
   } scLlPage, *PSCLLPAGE;
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  BOOL _bDesignerPreview;  // flag if in Preview Mode or not
  BOOL _bPreDraw;    // TRUE on call before draw, FALSE on call afterwards
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // RECT in paint units (mm/10 or inch/100)
   } scLlProject, *PSCLLPROJECT;
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  LPCSTR _pszName;    // name of the variable
  LPCSTR _pszContents;   // contents of the variable (valid if def´d by VariableExt())
  INT _lPara;     // lPara of the variable
  LPVOID _lpPtr;     // lpPtr of the variable
  HANDLE _hPara;     // hContents of the variable (valid if def´d by VariableExtHandle())
  BOOL _bIsotropic;   // "Isotropic" flag
  LPCSTR _pszParameters;   // for editable USERDWG: parameters
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // RECT in paint units (mm/10 or inch/100)
   INT  _nPaintMode;   // 0: Preview, 1: FastPreview, 2: Workspace (NYI)
  } scLlDrawUserObjA;
 typedef struct
   {
   UINT   _nSize;
   LPCWSTR _pszName;
   LPCWSTR _pszContents;
   INT  _lPara;
   LPVOID _lpPtr;
   HANDLE _hPara;
   BOOL  _bIsotropic;
   LPCWSTR _pszParameters;
   HDC  _hRefDC;
   HDC  _hPaintDC;
   RECT  _rcPaint;
   INT  _nPaintMode;
   } scLlDrawUserObjW;
 #if defined(UNICODE)
   typedef scLlDrawUserObjW scLlDrawUserObj, *PSCLLDRAWUSEROBJECT;
 #else
   typedef scLlDrawUserObjA scLlDrawUserObj, *PSCLLDRAWUSEROBJECT;
 #endif
 
 #if defined(USE_OCX)
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  BSTR _pszName;    // name of the variable
  BSTR _pszContents;   // contents of the variable (valid if def´d by VariableExt())
  INT _lPara;     // lPara of the variable
  LPVOID _lpPtr;     // lpPtr of the variable
  HANDLE _hPara;     // hContents of the variable (valid if def´d by VariableExtHandle())
  BOOL _bIsotropic;   // "Isotropic" flag
  BSTR _pszParameters;  // for editable USERDWG: parameters
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // RECT in paint units (mm/10 or inch/100)
   INT  _nPaintMode;   // 0: Preview, 1: FastPreview, 2: Workspace (NYI)
  } scLlDrawUserObj_B, *PSCLLDRAWUSEROBJ_B;
 #endif
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  LPCSTR _pszName;    // name of the variable
  INT_PTR _lPara;     // lPara of the variable
  LPVOID _lpPtr;     // lpPtr of the variable
  HANDLE _hPara;     // hContents of the variable (valid if def´d by VariableExtHandle())
  BOOL _bIsotropic;   // "Isotropic" flag
  HWND _hWnd;     // parent window for dialog
  LPSTR _pszParameters;   // parameter buffer
  UINT _nParaBufSize;   // max size of buffer (1024+1)
  } scLlEditUserObjA;
 typedef struct
   {
   UINT     _nSize;
   LPCWSTR _pszName;
   INT_PTR _lPara;
   LPVOID _lpPtr;
   HANDLE _hPara;
   BOOL  _bIsotropic;
   HWND  _hWnd;
   LPWSTR _pszParameters;
   UINT  _nParaBufSize;
   } scLlEditUserObjW;
 #if defined(UNICODE)
   typedef scLlEditUserObjW scLlEditUserObj, *PSCLLEDITUSEROBJECT;
 #else
   typedef scLlEditUserObjA scLlEditUserObj, *PSCLLEDITUSEROBJECT;
 #endif
 
 #if defined(USE_OCX)
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  BSTR _pszName;    // name of the variable
  INT_PTR _lPara;     // lPara of the variable
  LPVOID _lpPtr;     // lpPtr of the variable
  HANDLE _hPara;     // hContents of the variable (valid if def´d by VariableExtHandle())
  BOOL _bIsotropic;   // "Isotropic" flag
  HWND _hWnd;     // parent window for dialog
  BSTR _pszParameters;   // parameter buffer
  INT  _nParaBufSize;   // max size of buffer (1024+1)
  } scLlEditUserObj_B, *PSCLLEDITUSEROBJ_B;
 #endif
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  INT  _nType;     // LL_TABLE_LINE_xxx constant
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // RECT in paint units (mm/10 or inch/100)
  INT  _nPageLine;       // number of line on page
  INT  _nLine;     // absolute number of line in table
  INT  _nLineDef;    // table line definition
  BOOL _bZebra;    // zebra mode selected by user (only body line!)
  RECT _rcSpacing;    // spaces around the frame
  } scLlTableLine, *PSCLLTABLELINE;
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  INT  _nType;     // LL_TABLE_FIELD_xxx constant
  HDC  _hRefDC;      // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // RECT in paint units (mm/10 or inch/100)
  INT  _nLineDef;    // table line definition
  INT  _nIndex;       // column index (0..)
  RECT _rcSpacing;    // spaces around the frame
  } scLlTableField, *PSCLLTABLEFIELD;
 
 typedef struct
   {
  UINT  _nSize;     // size of the structure
  INT  _xPos;     // column position (paint units)
  INT  _nWidth;    // width (paint units)
  HFONT _fntColumnX;   // column font (NULL)
   } scLlColumn;
 
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  HDC  _hRefDC;    // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // suggested RECT in paint units (mm/10 or inch/100), change .bottom value
   HFONT _fntHeaderX;   // header default font
   HFONT _fntBodyX;    // body   default font
   HFONT _fntFooterX;   // footer default font
  INT  _nHeight;    // height of one body line (incl. spacing)
  BOOL _bPaint;    // do paint, or do just calculate?
  INT _lParam;    // function argument: user parameter 1
  LPVOID _lpParam;    // function argument: user parameter 2
  INT  _nColumns;    // number of columns in this table
  PSCLLCOLUMN _lpColumns;      // pointer to an array of column info
  } scLlGroup, *PSCLLGROUP;
 
 #if defined(USE_OCX)
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  HDC  _hRefDC;    // HDC to get information from
  HDC  _hPaintDC;     // HDC to paint on
  RECT _rcPaint;    // suggested RECT in paint units (mm/10 or inch/100), change .bottom value
   HFONT _fntHeader;    // header default font
   HFONT _fntBody;    // body   default font
   HFONT _fntFooter;    // footer default font
  INT  _nHeight;    // height of one body line (incl. spacing)
  BOOL _bPaint;    // do paint, or do just calculate?
  BOOL _bFrameLeft;   // frame on left side selected?
  BOOL _bFrameRight;   // frame on right side selected?
  INT _lParam;    // function argument: user parameter 1
  BSTR _lpParam;    // function argument: user parameter 2
  INT  _nColumns;    // number of columns in this table
  PSCLLCOLUMN _lpColumns;      // pointer to an array of column info
  } scLlGroup_B, *PSCLLGROUP_B;
 #endif
 
 typedef struct
  {
  UINT _nSize;     // size of the structure
  BOOL _bFirst;    // first or second printer?
   INT  _nCmd;     // i command
   HWND    _hWnd;     // i
   HDC  _hDC;     // i/o
   INT16 _nOrientation;   // i/o
  BOOL _bPhysPage;    // i/o, new L5
   UINT    _nBufSize;    // i
   LPSTR   _pszBuffer;    // i, fill
   INT     _nUniqueNumber;   // i
   INT  _nUniqueNumberCompare;  // i
   INT  _nPaperFormat;   // i/o (JobID for LL_CMND_CHANGE_DCPROPERTIES_DOC)
   INT    _xPaperSize;   // i/o, mm/10
   INT    _yPaperSize;   // i/o, mm/10
  } scLlPrinterA;
 typedef struct
   {
   UINT  _nSize;
   BOOL  _bFirst;
   INT  _nCmd;
   HWND     _hWnd;
   HDC  _hDC;
   INT16  _nOrientation;
   BOOL  _bPhysPage;
   UINT     _nBufSize;
   LPWSTR    _pszBuffer;
   INT      _nUniqueNumber;
   INT  _nUniqueNumberCompare;
   INT  _nPaperFormat;   // i/o (JobID for LL_CMND_CHANGE_DCPROPERTIES_DOC)
   INT    _xPaperSize;   // i/o, mm/10
   INT    _yPaperSize;   // i/o, mm/10
   } scLlPrinterW;
 #if defined(UNICODE)
   typedef scLlPrinterW scLlPrinter, *PSCLLPRINTER;
 #else
   typedef scLlPrinterA scLlPrinter, *PSCLLPRINTER;
 #endif
 
 typedef LRESULT (WINAPI *LLNOTIFICATION)(UINT nMessage, LPARAM lParam, UINT_PTR lUserParam);
 
 typedef struct                    // for VC++
   {
   UINT   FAR *ppStruct;
  LPVOID nIndex;
   } PTRPARAM;
 typedef struct                    // for VC++
   {
  INT  FAR *ppnID;
  INT  FAR *ppnType;
  LPVOID nIndex;
   } HELPPARAM;
 typedef struct                    // for VC++
   {
   INT     FAR *ppnFunction;
  LPVOID nIndex;
   } NTFYBTNPARAM;
 typedef struct
  {
  UINT  _nSize;     // size of the structure
  UINT _nFunction;    // function code (LL_DLGEXPR_VAREXTBTN_xxx)
   HWND _hWndDialog;   // dialog handle
  LPCSTR _pszPage;    // "CondDlgVar"
   CHAR _szValue[0x4000+1];  // new value
   UINT _bFields;    // FALSE
  UINT _nMask;     // LL_TEXT, ... OR LL_FOOTERFIELD...
  } scLlDlgExprVarExtA;
 typedef struct
   {
   UINT     _nSize;
   UINT  _nFunction;
   HWND  _hWndDialog;
   LPCWSTR _pszPage;
   WCHAR  _szValue[0x4000+1];
   UINT  _bFields;
   UINT _nMask;
   } scLlDlgExprVarExtW;
 #if defined(UNICODE)
   typedef scLlDlgExprVarExtW scLlDlgExprVarExt, *PSCLLDLGEXPRVAREXT;
 #else
   typedef scLlDlgExprVarExtA scLlDlgExprVarExt, *PSCLLDLGEXPRVAREXT;
 #endif
 
 typedef struct
  {
  UINT _nSize;
  HWND _hWnd;
  UINT _nType;
  HMENU _hMenu;
  UINT _nParam;
  } scLlToolbar, *PSCLLTOOLBAR;
 
 typedef struct
   {
   INT   _nSize;
   BOOL  _bEnableAskString;
  } scLlDlgEditLineExParams;
 typedef struct
   {
   INT   _nSize;
   BOOL  _bEnableAskString;
   UINT  _bIncludeChartFields;
  } scLlDlgEditLineExParams8;
 typedef struct
   {
   INT   _nSize;
   BOOL  _bEnableAskString;
   UINT  _bIncludeChartFields;
   LPCWSTR  _pszAllowedSourceTables;
  } scLlDlgEditLineExParams12;
 typedef struct
   {
   INT   _nSize;
   BOOL  _bEnableAskString;
   UINT  _bIncludeChartFields;
   LPCWSTR  _pszAllowedSourceTables;
  UINT  _nFlags;
  } scLlDlgEditLineExParams12a;
 #define LL_SCDLGEDITLINEPARAMS_FLAG_NO_ALIASTRANSLATION 0x00000001
 #define LL_SCDLGEDITLINEPARAMS_FLAG_RESERVED    0x80000000
 typedef struct
  {
  INT   _nSize;
  HLLJOB  _hLlJob; // the LL job that issued the job
  CHAR  _szDevice[80]; // printer device name
  DWORD  _dwJobID; // an internal ID, NOT THE QUEUE ID (as different queues could have different IDs)
  DWORD  _dwState; // state (JOB_STATUS_xxx, see Windows API)
  } scLlPrintJobInfoA;
 typedef struct
  {
  INT   _nSize;
  HLLJOB  _hLlJob; // the LL job that issued the job
  WCHAR  _szDevice[80]; // printer device name
  DWORD  _dwJobID; // an internal ID, NOT THE QUEUE ID (as different queues could have different IDs)
  DWORD  _dwState; // state (JOB_STATUS_xxx, see Windows API)
  } scLlPrintJobInfoW;
 #if defined(UNICODE)
   typedef scLlPrintJobInfoW scLlPrintJobInfo, *PSCLLPRINTJOBINFO;
 #else
   typedef scLlPrintJobInfoA scLlPrintJobInfo, *PSCLLPRINTJOBINFO;
 #endif
 
 typedef struct
  {
  INT   _nSize;
  BOOL  _bAlsoField;
  LPCSTR  _pszVarName;
  } scLlDelayDefineFieldOrVariableA;
 typedef struct
  {
  INT   _nSize;
  BOOL  _bAlsoField;
  LPCWSTR  _pszVarName;
  } scLlDelayDefineFieldOrVariableW;
 #if defined(UNICODE)
   typedef scLlDelayDefineFieldOrVariableW scLlDelayDefineFieldOrVariable, *PSCLLDELAYDEFINEFIELDORVARIABLE;
 #else
   typedef scLlDelayDefineFieldOrVariableA scLlDelayDefineFieldOrVariable, *PSCLLDELAYDEFINEFIELDORVARIABLE;
 #endif
 
 typedef struct
  {
  INT   _nSize;
  LPCSTR  _pszVarName;
  LPSTR  _pszContents;
  UINT  _nBufSize; // size of buffer '_pszContents' points to. If a larger buffer is needed, _pszContents must be changed to point to that (static) buffer in the host application
  HANDLE  _hContents;
  } scLlDelayedValueA;
 typedef struct
  {
  INT   _nSize;
  LPCWSTR  _pszVarName;
  LPWSTR  _pszContents;
  UINT  _nBufSize; // size of buffer '_pszContents' points to. If a larger buffer is needed, _pszContents must be changed to point to that (static) buffer in the host application
  HANDLE  _hContents;
  } scLlDelayedValueW;
 #if defined(UNICODE)
   typedef scLlDelayedValueW scLlDelayedValue, *PSCLLDELAYEDVALUE;
 #else
   typedef scLlDelayedValueA scLlDelayedValue, *PSCLLDELAYEDVALUE;
 #endif
 
 typedef struct
  {
  INT   _nSize;
  BOOL  _bStoring;
  IStream* _pContentStream;
  } scLlProjectUserData;
 
 typedef struct
  {
  UINT        _nSize;
  UINT_PTR       _nUserParam;
  LPCSTR        _pszProjectFileName;
  LPCSTR        _pszOriginalProjectFileName;
  UINT        _nPages; // 0 for 'unlimited'
  UINT        _nFunction;
  HWND        _hWnd;
  HANDLE        _hEvent;
  } scLlDesignerPrintJobA;
 typedef struct
  {
  UINT        _nSize;
  UINT_PTR       _nUserParam;
  LPCWSTR        _pszProjectFileName;
  LPCWSTR        _pszOriginalProjectFileName;
  UINT        _nPages; // 0 for 'unlimited'
  UINT        _nFunction;
  HWND        _hWnd;
  HANDLE        _hEvent;
  } scLlDesignerPrintJobW;
 #if defined(UNICODE)
   typedef scLlDesignerPrintJobW scLlDesignerPrintJob, *PSCLLDESIGNERPRINTJOB;
 #else
   typedef scLlDesignerPrintJobA scLlDesignerPrintJob, *PSCLLDESIGNERPRINTJOB;
 #endif
 
 typedef struct
 {
  UINT        _nSize;        // [in]
  UINT        _nFunction;       // [in]
  UINT_PTR       _nUserParameter;     // [in]
  LPCSTR        _pszTableID;      // [in]
  LPCSTR        _pszRelationID;      // [in]
  LPCSTR        _pszSubreportTableID;    // [in]
  LPCSTR        _pszKeyField;      // [in]
  LPCSTR        _pszSubreportKeyField;    // [in]
  LPCSTR        _pszKeyValue;      // [in]
  LPCSTR        _pszProjectFileName;    // [in]
  LPCSTR        _pszPreviewFileName;    // [in] NULL for interactive mode
  LPCSTR        _pszTooltipText;     // [in]
  LPCSTR        _pszTabText;      // [in] right now, same as _pszTooltipText
  HWND        _hWnd;        // [in]
  UINT_PTR       _nID;        // [in] unique ID of job (important for ABORT/FINISH)
   HANDLE        _hAttachInfo;      // [in]
 } scLlDrillDownJobA;
 typedef struct
 {
  UINT        _nSize;        // [in]
  UINT        _nFunction;       // [in]
  UINT_PTR       _nUserParameter;     // [in]
  LPCWSTR        _pszTableID;      // [in]
  LPCWSTR        _pszRelationID;      // [in]
  LPCWSTR        _pszSubreportTableID;    // [in]
  LPCWSTR        _pszKeyField;      // [in]
  LPCWSTR        _pszSubreportKeyField;    // [in]
  LPCWSTR        _pszKeyValue;      // [in]
  LPCWSTR        _pszProjectFileName;    // [in]
  LPCWSTR        _pszPreviewFileName;    // [in] NULL for interactive mode
  LPCWSTR        _pszTooltipText;     // [in]
  LPCWSTR        _pszTabText;      // [in] right now, same as _pszTooltipText
  HWND        _hWnd;        // [in]
  UINT_PTR       _nID;        // [in] unique ID of job (important for ABORT/FINISH)
   HANDLE        _hAttachInfo;      // [in]
 } scLlDrillDownJobW;
 #if defined(UNICODE)
   typedef scLlDrillDownJobW scLlDrillDownJob, *PSCLLDRILLDOWNJOB;
 #else
   typedef scLlDrillDownJobA scLlDrillDownJob, *PSCLLDRILLDOWNJOB;
 #endif
 
 typedef struct
  {
  INT   _nSize;
  INT   _hLlJob;
  LPCSTR  _pszParentTableID;
  LPCSTR  _pszSubreportTableID;
  LPCSTR  _pszRelationID;
  LPSTR  _pszAllowedTables;
  UINT  _nBufSize; // size of buffer '_pszAllowedTables' points to. If a larger buffer is needed, _pszAllowedTables must be changed to point to that (static) buffer in the host application
  } scLlDDFilterInfoA;
 typedef struct
  {
  INT   _nSize;
  INT   _hLlJob;
  LPCWSTR  _pszParentTableID;
  LPCWSTR  _pszSubreportTableID;
  LPCWSTR  _pszRelationID;
  LPWSTR  _pszAllowedTables;
  UINT  _nBufSize; // size of buffer '_pszAllowedTables' points to. If a larger buffer is needed, _pszAllowedTables must be changed to point to that (static) buffer in the host application
  } scLlDDFilterInfoW;
 #if defined(UNICODE)
   typedef scLlDDFilterInfoW scLlDDFilterInfo, *PSCLLDDFILTERINFO;
 #else
   typedef scLlDDFilterInfoA scLlDDFilterInfo, *PSCLLDDFILTERINFO;
 #endif
 
 #endif // ifndef IMPLEMENTATION
 
 
 #ifdef _DEF_LLXINTERFACE // LL internal
   #include "lxoem.h"
 #endif // _DEF_LLXINTERFACE
 #ifndef IMPLEMENTATION
 typedef struct
  {
   INT  _nSize;
   INT  _nProjectTypeCount;
   UINT* _pProjectType;
  } scLlSelectFileDlgTitleExParams;
 #endif
 #ifndef IMPLEMENTATION
 interface ILlXOBStatCB
   : public IUnknown
  {
    public:
           enum enConfigEntryType
            {
            LLXOBSCB_CET_ENTRY,
            LLXOBSCB_CET_CLEAR,
            };
           enum enPropertyBag
            {
            LLXOBSCB_PB_GLOBAL,
            LLXOBSCB_PB_USER,
            };
     public:
   STDMETHOD      (QueryInterface)(REFIID riid, VOID** ppv) PURE;
   STDMETHOD_      (ULONG,AddRef)(void) PURE;
   STDMETHOD_      (ULONG,Release)(void) PURE;
 
   STDMETHOD_      (ULONG,GetVersion)(void) PURE;
   STDMETHOD      (GetParentHWND)(HWND* phWnd) PURE;
   STDMETHOD      (GetParentLCID)(LCID* pnLCID) PURE;
   STDMETHOD      (GetParentViewName)(BSTR* pbsName) PURE;
   STDMETHOD      (GetField)(int nIndex, BSTR* pbsName, BSTR* pbsAlias, UINT* pnType) PURE;    // S_FALSE for 'no more field', LL field types
   STDMETHOD      (SetAdditionalFields)(HLLJOB hLlJob) PURE;
 
   STDMETHOD      (GetConfigEntry)(enPropertyBag nBag, const BSTR bsName, BSTR* pbsValue) PURE;    // S_FALSE for 'no entry'
   STDMETHOD      (SetConfigEntry)(enPropertyBag nBag, const BSTR bsName, BSTR bsValue, enConfigEntryType nType) PURE; // please ignore empty names (only evaluate nType in this case)
 
   STDMETHOD      (GetOption)(int nOption, VARIANT* pVal) PURE;
            #define LLXOBSCB_OPT_MAY_MODIFY_GLOBAL   1     // BSTR, if <> EMPTY -> show message
            #define LLXOBSCB_OPT_MAY_MODIFY_USER   2     // BSTR, if <> EMPTY -> show message
            #define LLXOBSCB_OPT_DEFAULTPROJECT    3     // BSTR
            #define LLXOBSCB_OPT_HELPPRESSED    4     // notification: help button pressed
            #define LLXOBSCB_OPT_DEFAULT_TEMPLATEFILENAME 5     // default "CRM"
 
   STDMETHOD      (TableGetRowcount)(DWORD* pdwCount) PURE;       // S_FALSE for 'no data'
   STDMETHOD      (TableGotoTop)(void) PURE;           // S_FALSE for 'no data'
   STDMETHOD      (TableNextRow)(void) PURE;           // S_FALSE for 'no more data'
   STDMETHOD      (GetFieldContents)(const BSTR bsName, VARIANT* pvContents) PURE;    //
   STDMETHOD      (QueryContinue)() PURE;                                            // S_FALSE for ABORT, S_OK for continue
  };
 
 interface ILlXOBStatJob
   : public IUnknown
  {
     public:
       enum   enLlXOBStatFileType
           {
           STATFILETYPE_EMF = 0,
           STATFILETYPE_JPG = 1,
           STATFILETYPE_BMP = 2,
           };
       struct scLlXOBStatJobFileInfo
           {
           UINT                _nSize;
           BSTR                _bsFileName;
           enLlXOBStatFileType _nFileType;
           SIZE                _szDimensions;
           };
     public:
   STDMETHOD      (QueryInterface)(REFIID riid, VOID** ppv) PURE;
   STDMETHOD_      (ULONG,AddRef)(void) PURE;
   STDMETHOD_      (ULONG,Release)(void) PURE;
 
   STDMETHOD_      (ULONG,GetVersion)(void) PURE;
   STDMETHOD      (SetNtfySink)(ILlXOBStatCB* pNtfySink) PURE;
 
   STDMETHOD      (Edit)(void) PURE;
   STDMETHOD      (Execute)(BSTR bsProject) PURE;          // S_FALSE if project not found
 
   STDMETHOD      (GetLastError)(BSTR* pbsLastError) PURE;
   STDMETHOD      (EnumProjects)(IEnumString** ppEnum) PURE;
   STDMETHOD      (CreateFile)(BSTR bsProject, const scLlXOBStatJobFileInfo& FileInfo) PURE;
  };
 #endif



/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef   HLLJOB ( DLLPROC *PFNLLJOBOPEN)(
	INT                  nLanguage);
#endif /* IMPLEMENTATION */

extern PFNLLJOBOPEN LlJobOpen;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef   HLLJOB ( DLLPROC *PFNLLJOBOPENLCID)(
	_LCID                nLCID);
#endif /* IMPLEMENTATION */

extern PFNLLJOBOPENLCID LlJobOpenLCID;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLJOBCLOSE)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLJOBCLOSE LlJobClose;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLSETDEBUG)(
	INT                  nOnOff);
#endif /* IMPLEMENTATION */

extern PFNLLSETDEBUG LlSetDebug;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     UINT ( DLLPROC *PFNLLGETVERSION)(
	INT                  nCmd);
#endif /* IMPLEMENTATION */

extern PFNLLGETVERSION LlGetVersion;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     UINT ( DLLPROC *PFNLLGETNOTIFICATIONMESSAGE)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLGETNOTIFICATIONMESSAGE LlGetNotificationMessage;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSETNOTIFICATIONMESSAGE)(
	HLLJOB               hLlJob,
	UINT                 nMessage);
#endif /* IMPLEMENTATION */

extern PFNLLSETNOTIFICATIONMESSAGE LlSetNotificationMessage;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef  FARPROC ( DLLPROC *PFNLLSETNOTIFICATIONCALLBACK)(
	HLLJOB               hLlJob,
	FARPROC              lpfnNotify);
#endif /* IMPLEMENTATION */

extern PFNLLSETNOTIFICATIONCALLBACK LlSetNotificationCallback;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineFieldO LlDefineFieldA
    #else
      #define LlDefineField LlDefineFieldA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	LPCSTR               lpbufContents);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDA LlDefineFieldA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineField LlDefineFieldW
    #else
      #define LlDefineFieldO LlDefineFieldW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	LPCWSTR              lpbufContents);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDW LlDefineFieldW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineFieldExtO LlDefineFieldExtA
    #else
      #define LlDefineFieldExt LlDefineFieldExtA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDEXTA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	LPCSTR               lpbufContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDEXTA LlDefineFieldExtA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineFieldExt LlDefineFieldExtW
    #else
      #define LlDefineFieldExtO LlDefineFieldExtW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDEXTW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	LPCWSTR              lpbufContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDEXTW LlDefineFieldExtW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineFieldExtHandleO LlDefineFieldExtHandleA
    #else
      #define LlDefineFieldExtHandle LlDefineFieldExtHandleA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDEXTHANDLEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	HANDLE               hContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDEXTHANDLEA LlDefineFieldExtHandleA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineFieldExtHandle LlDefineFieldExtHandleW
    #else
      #define LlDefineFieldExtHandleO LlDefineFieldExtHandleW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDEXTHANDLEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	HANDLE               hContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDEXTHANDLEW LlDefineFieldExtHandleW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLDEFINEFIELDSTART)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDSTART LlDefineFieldStart;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableO LlDefineVariableA
    #else
      #define LlDefineVariable LlDefineVariableA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	LPCSTR               lpbufContents);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEA LlDefineVariableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariable LlDefineVariableW
    #else
      #define LlDefineVariableO LlDefineVariableW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	LPCWSTR              lpbufContents);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEW LlDefineVariableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableExtO LlDefineVariableExtA
    #else
      #define LlDefineVariableExt LlDefineVariableExtA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEEXTA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	LPCSTR               lpbufContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEEXTA LlDefineVariableExtA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableExt LlDefineVariableExtW
    #else
      #define LlDefineVariableExtO LlDefineVariableExtW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEEXTW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	LPCWSTR              lpbufContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEEXTW LlDefineVariableExtW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableExtHandleO LlDefineVariableExtHandleA
    #else
      #define LlDefineVariableExtHandle LlDefineVariableExtHandleA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEEXTHANDLEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	HANDLE               hContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEEXTHANDLEA LlDefineVariableExtHandleA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableExtHandle LlDefineVariableExtHandleW
    #else
      #define LlDefineVariableExtHandleO LlDefineVariableExtHandleW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEEXTHANDLEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	HANDLE               hContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEEXTHANDLEW LlDefineVariableExtHandleW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableNameO LlDefineVariableNameA
    #else
      #define LlDefineVariableName LlDefineVariableNameA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLENAMEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLENAMEA LlDefineVariableNameA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableName LlDefineVariableNameW
    #else
      #define LlDefineVariableNameO LlDefineVariableNameW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLENAMEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLENAMEW LlDefineVariableNameW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLDEFINEVARIABLESTART)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLESTART LlDefineVariableStart;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineSumVariableO LlDefineSumVariableA
    #else
      #define LlDefineSumVariable LlDefineSumVariableA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINESUMVARIABLEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	LPCSTR               lpbufContents);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINESUMVARIABLEA LlDefineSumVariableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineSumVariable LlDefineSumVariableW
    #else
      #define LlDefineSumVariableO LlDefineSumVariableW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINESUMVARIABLEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	LPCWSTR              lpbufContents);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINESUMVARIABLEW LlDefineSumVariableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineLayoutO LlDefineLayoutA
    #else
      #define LlDefineLayout LlDefineLayoutA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINELAYOUTA)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCSTR               pszTitle,
	UINT                 nObjType,
	LPCSTR               pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINELAYOUTA LlDefineLayoutA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineLayout LlDefineLayoutW
    #else
      #define LlDefineLayoutO LlDefineLayoutW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINELAYOUTW)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCWSTR              pszTitle,
	UINT                 nObjType,
	LPCWSTR              pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINELAYOUTW LlDefineLayoutW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDlgEditLineO LlDlgEditLineA
    #else
      #define LlDlgEditLine LlDlgEditLineA
  #endif
typedef      INT ( DLLPROC *PFNLLDLGEDITLINEA)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPSTR                lpBuf,
	INT                  nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDLGEDITLINEA LlDlgEditLineA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDlgEditLine LlDlgEditLineW
    #else
      #define LlDlgEditLineO LlDlgEditLineW
  #endif
typedef      INT ( DLLPROC *PFNLLDLGEDITLINEW)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPWSTR               lpBuf,
	INT                  nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDLGEDITLINEW LlDlgEditLineW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDlgEditLineExO LlDlgEditLineExA
    #else
      #define LlDlgEditLineEx LlDlgEditLineExA
  #endif
typedef      INT ( DLLPROC *PFNLLDLGEDITLINEEXA)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPSTR                pszBuffer,
	UINT                 nBufSize,
	UINT                 nParaTypes,
	LPCSTR               pszTitle,
	BOOL                 bTable,
	LPVOID               pvReserved);
#endif /* IMPLEMENTATION */

extern PFNLLDLGEDITLINEEXA LlDlgEditLineExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDlgEditLineEx LlDlgEditLineExW
    #else
      #define LlDlgEditLineExO LlDlgEditLineExW
  #endif
typedef      INT ( DLLPROC *PFNLLDLGEDITLINEEXW)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPWSTR               pszBuffer,
	UINT                 nBufSize,
	UINT                 nParaTypes,
	LPCWSTR              pszTitle,
	BOOL                 bTable,
	LPVOID               pvReserved);
#endif /* IMPLEMENTATION */

extern PFNLLDLGEDITLINEEXW LlDlgEditLineExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewSetTempPathO LlPreviewSetTempPathA
    #else
      #define LlPreviewSetTempPath LlPreviewSetTempPathA
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWSETTEMPPATHA)(
	HLLJOB               hLlJob,
	LPCSTR               pszPath);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWSETTEMPPATHA LlPreviewSetTempPathA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewSetTempPath LlPreviewSetTempPathW
    #else
      #define LlPreviewSetTempPathO LlPreviewSetTempPathW
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWSETTEMPPATHW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszPath);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWSETTEMPPATHW LlPreviewSetTempPathW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewDeleteFilesO LlPreviewDeleteFilesA
    #else
      #define LlPreviewDeleteFiles LlPreviewDeleteFilesA
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWDELETEFILESA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjName,
	LPCSTR               pszPath);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWDELETEFILESA LlPreviewDeleteFilesA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewDeleteFiles LlPreviewDeleteFilesW
    #else
      #define LlPreviewDeleteFilesO LlPreviewDeleteFilesW
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWDELETEFILESW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjName,
	LPCWSTR              pszPath);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWDELETEFILESW LlPreviewDeleteFilesW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewDisplayO LlPreviewDisplayA
    #else
      #define LlPreviewDisplay LlPreviewDisplayA
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWDISPLAYA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjName,
	LPCSTR               pszPath,
	HWND                 Wnd);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWDISPLAYA LlPreviewDisplayA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewDisplay LlPreviewDisplayW
    #else
      #define LlPreviewDisplayO LlPreviewDisplayW
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWDISPLAYW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjName,
	LPCWSTR              pszPath,
	HWND                 Wnd);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWDISPLAYW LlPreviewDisplayW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewDisplayExO LlPreviewDisplayExA
    #else
      #define LlPreviewDisplayEx LlPreviewDisplayExA
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWDISPLAYEXA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjName,
	LPCSTR               pszPath,
	HWND                 Wnd,
	UINT                 nOptions,
	LPVOID               pOptions);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWDISPLAYEXA LlPreviewDisplayExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPreviewDisplayEx LlPreviewDisplayExW
    #else
      #define LlPreviewDisplayExO LlPreviewDisplayExW
  #endif
typedef      INT ( DLLPROC *PFNLLPREVIEWDISPLAYEXW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjName,
	LPCWSTR              pszPath,
	HWND                 Wnd,
	UINT                 nOptions,
	LPVOID               pOptions);
#endif /* IMPLEMENTATION */

extern PFNLLPREVIEWDISPLAYEXW LlPreviewDisplayExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINT)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINT LlPrint;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTABORT)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTABORT LlPrintAbort;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     BOOL ( DLLPROC *PFNLLPRINTCHECKLINEFIT)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTCHECKLINEFIT LlPrintCheckLineFit;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTEND)(
	HLLJOB               hLlJob,
	INT                  nPages);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTEND LlPrintEnd;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTFIELDS)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTFIELDS LlPrintFields;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTFIELDSEND)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTFIELDSEND LlPrintFieldsEnd;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTGETCURRENTPAGE)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETCURRENTPAGE LlPrintGetCurrentPage;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTGETITEMSPERPAGE)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETITEMSPERPAGE LlPrintGetItemsPerPage;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTGETITEMSPERTABLE)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETITEMSPERTABLE LlPrintGetItemsPerTable;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetRemainingItemsPerTableO LlPrintGetRemainingItemsPerTableA
    #else
      #define LlPrintGetRemainingItemsPerTable LlPrintGetRemainingItemsPerTableA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETREMAININGITEMSPERTABLEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszField);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETREMAININGITEMSPERTABLEA LlPrintGetRemainingItemsPerTableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetRemainingItemsPerTable LlPrintGetRemainingItemsPerTableW
    #else
      #define LlPrintGetRemainingItemsPerTableO LlPrintGetRemainingItemsPerTableW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETREMAININGITEMSPERTABLEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszField);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETREMAININGITEMSPERTABLEW LlPrintGetRemainingItemsPerTableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetRemItemsPerTableO LlPrintGetRemItemsPerTableA
    #else
      #define LlPrintGetRemItemsPerTable LlPrintGetRemItemsPerTableA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETREMITEMSPERTABLEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszField);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETREMITEMSPERTABLEA LlPrintGetRemItemsPerTableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetRemItemsPerTable LlPrintGetRemItemsPerTableW
    #else
      #define LlPrintGetRemItemsPerTableO LlPrintGetRemItemsPerTableW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETREMITEMSPERTABLEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszField);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETREMITEMSPERTABLEW LlPrintGetRemItemsPerTableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef  INT_PTR ( DLLPROC *PFNLLPRINTGETOPTION)(
	HLLJOB               hLlJob,
	INT                  nIndex);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETOPTION LlPrintGetOption;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetPrinterInfoO LlPrintGetPrinterInfoA
    #else
      #define LlPrintGetPrinterInfo LlPrintGetPrinterInfoA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETPRINTERINFOA)(
	HLLJOB               hLlJob,
	LPSTR                pszPrn,
	UINT                 nPrnLen,
	LPSTR                pszPort,
	UINT                 nPortLen);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETPRINTERINFOA LlPrintGetPrinterInfoA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetPrinterInfo LlPrintGetPrinterInfoW
    #else
      #define LlPrintGetPrinterInfoO LlPrintGetPrinterInfoW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETPRINTERINFOW)(
	HLLJOB               hLlJob,
	LPWSTR               pszPrn,
	UINT                 nPrnLen,
	LPWSTR               pszPort,
	UINT                 nPortLen);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETPRINTERINFOW LlPrintGetPrinterInfoW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintOptionsDialogO LlPrintOptionsDialogA
    #else
      #define LlPrintOptionsDialog LlPrintOptionsDialogA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTOPTIONSDIALOGA)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCSTR               pszText);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTOPTIONSDIALOGA LlPrintOptionsDialogA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintOptionsDialog LlPrintOptionsDialogW
    #else
      #define LlPrintOptionsDialogO LlPrintOptionsDialogW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTOPTIONSDIALOGW)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCWSTR              pszText);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTOPTIONSDIALOGW LlPrintOptionsDialogW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTSELECTOFFSETEX)(
	HLLJOB               hLlJob,
	HWND                 hWnd);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSELECTOFFSETEX LlPrintSelectOffsetEx;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintSetBoxTextO LlPrintSetBoxTextA
    #else
      #define LlPrintSetBoxText LlPrintSetBoxTextA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSETBOXTEXTA)(
	HLLJOB               hLlJob,
	LPCSTR               szText,
	INT                  nPercentage);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSETBOXTEXTA LlPrintSetBoxTextA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintSetBoxText LlPrintSetBoxTextW
    #else
      #define LlPrintSetBoxTextO LlPrintSetBoxTextW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSETBOXTEXTW)(
	HLLJOB               hLlJob,
	LPCWSTR              szText,
	INT                  nPercentage);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSETBOXTEXTW LlPrintSetBoxTextW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTSETOPTION)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	INT_PTR              nValue);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSETOPTION LlPrintSetOption;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTUPDATEBOX)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTUPDATEBOX LlPrintUpdateBox;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintStartO LlPrintStartA
    #else
      #define LlPrintStart LlPrintStartA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSTARTA)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCSTR               pszObjName,
	INT                  nPrintOptions,
	INT                  nReserved);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSTARTA LlPrintStartA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintStart LlPrintStartW
    #else
      #define LlPrintStartO LlPrintStartW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSTARTW)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCWSTR              pszObjName,
	INT                  nPrintOptions,
	INT                  nReserved);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSTARTW LlPrintStartW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintWithBoxStartO LlPrintWithBoxStartA
    #else
      #define LlPrintWithBoxStart LlPrintWithBoxStartA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTWITHBOXSTARTA)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCSTR               pszObjName,
	INT                  nPrintOptions,
	INT                  nBoxType,
	HWND                 hWnd,
	LPCSTR               pszTitle);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTWITHBOXSTARTA LlPrintWithBoxStartA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintWithBoxStart LlPrintWithBoxStartW
    #else
      #define LlPrintWithBoxStartO LlPrintWithBoxStartW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTWITHBOXSTARTW)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCWSTR              pszObjName,
	INT                  nPrintOptions,
	INT                  nBoxType,
	HWND                 hWnd,
	LPCWSTR              pszTitle);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTWITHBOXSTARTW LlPrintWithBoxStartW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrinterSetupO LlPrinterSetupA
    #else
      #define LlPrinterSetup LlPrinterSetupA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTERSETUPA)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	UINT                 nObjType,
	LPCSTR               pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTERSETUPA LlPrinterSetupA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrinterSetup LlPrinterSetupW
    #else
      #define LlPrinterSetupO LlPrinterSetupW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTERSETUPW)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	UINT                 nObjType,
	LPCWSTR              pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTERSETUPW LlPrinterSetupW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSelectFileDlgTitleExO LlSelectFileDlgTitleExA
    #else
      #define LlSelectFileDlgTitleEx LlSelectFileDlgTitleExA
  #endif
typedef      INT ( DLLPROC *PFNLLSELECTFILEDLGTITLEEXA)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCSTR               pszTitle,
	UINT                 nObjType,
	LPSTR                pszObjName,
	UINT                 nBufSize,
	LPVOID               pReserved);
#endif /* IMPLEMENTATION */

extern PFNLLSELECTFILEDLGTITLEEXA LlSelectFileDlgTitleExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSelectFileDlgTitleEx LlSelectFileDlgTitleExW
    #else
      #define LlSelectFileDlgTitleExO LlSelectFileDlgTitleExW
  #endif
typedef      INT ( DLLPROC *PFNLLSELECTFILEDLGTITLEEXW)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCWSTR              pszTitle,
	UINT                 nObjType,
	LPWSTR               pszObjName,
	UINT                 nBufSize,
	LPVOID               pReserved);
#endif /* IMPLEMENTATION */

extern PFNLLSELECTFILEDLGTITLEEXW LlSelectFileDlgTitleExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLSETDLGBOXMODE)(
	UINT                 nMode);
#endif /* IMPLEMENTATION */

extern PFNLLSETDLGBOXMODE LlSetDlgboxMode;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     UINT ( DLLPROC *PFNLLGETDLGBOXMODE)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLLGETDLGBOXMODE LlGetDlgboxMode;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprParseO LlExprParseA
    #else
      #define LlExprParse LlExprParseA
  #endif
typedef  HLLEXPR ( DLLPROC *PFNLLEXPRPARSEA)(
	HLLJOB               hLlJob,
	LPCSTR               lpExprText,
	BOOL                 bIncludeFields);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRPARSEA LlExprParseA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprParse LlExprParseW
    #else
      #define LlExprParseO LlExprParseW
  #endif
typedef  HLLEXPR ( DLLPROC *PFNLLEXPRPARSEW)(
	HLLJOB               hLlJob,
	LPCWSTR              lpExprText,
	BOOL                 bIncludeFields);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRPARSEW LlExprParseW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLEXPRTYPE)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRTYPE LlExprType;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprErrorO LlExprErrorA
    #else
      #define LlExprError LlExprErrorA
  #endif
typedef     void ( DLLPROC *PFNLLEXPRERRORA)(
	HLLJOB               hLlJob,
	LPSTR                pszBuf,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRERRORA LlExprErrorA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprError LlExprErrorW
    #else
      #define LlExprErrorO LlExprErrorW
  #endif
typedef     void ( DLLPROC *PFNLLEXPRERRORW)(
	HLLJOB               hLlJob,
	LPWSTR               pszBuf,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRERRORW LlExprErrorW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLEXPRFREE)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRFREE LlExprFree;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprEvaluateO LlExprEvaluateA
    #else
      #define LlExprEvaluate LlExprEvaluateA
  #endif
typedef      INT ( DLLPROC *PFNLLEXPREVALUATEA)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr,
	LPSTR                pszBuf,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLEXPREVALUATEA LlExprEvaluateA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprEvaluate LlExprEvaluateW
    #else
      #define LlExprEvaluateO LlExprEvaluateW
  #endif
typedef      INT ( DLLPROC *PFNLLEXPREVALUATEW)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr,
	LPWSTR               pszBuf,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLEXPREVALUATEW LlExprEvaluateW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprGetUsedVarsO LlExprGetUsedVarsA
    #else
      #define LlExprGetUsedVars LlExprGetUsedVarsA
  #endif
typedef      INT ( DLLPROC *PFNLLEXPRGETUSEDVARSA)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRGETUSEDVARSA LlExprGetUsedVarsA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprGetUsedVars LlExprGetUsedVarsW
    #else
      #define LlExprGetUsedVarsO LlExprGetUsedVarsW
  #endif
typedef      INT ( DLLPROC *PFNLLEXPRGETUSEDVARSW)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRGETUSEDVARSW LlExprGetUsedVarsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSETOPTION)(
	HLLJOB               hLlJob,
	INT                  nMode,
	INT_PTR              nValue);
#endif /* IMPLEMENTATION */

extern PFNLLSETOPTION LlSetOption;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef  INT_PTR ( DLLPROC *PFNLLGETOPTION)(
	HLLJOB               hLlJob,
	INT                  nMode);
#endif /* IMPLEMENTATION */

extern PFNLLGETOPTION LlGetOption;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetOptionStringO LlSetOptionStringA
    #else
      #define LlSetOptionString LlSetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLSETOPTIONSTRINGA)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPCSTR               pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLSETOPTIONSTRINGA LlSetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetOptionString LlSetOptionStringW
    #else
      #define LlSetOptionStringO LlSetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLSETOPTIONSTRINGW)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPCWSTR              pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLSETOPTIONSTRINGW LlSetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetOptionStringO LlGetOptionStringA
    #else
      #define LlGetOptionString LlGetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLGETOPTIONSTRINGA)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETOPTIONSTRINGA LlGetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetOptionString LlGetOptionStringW
    #else
      #define LlGetOptionStringO LlGetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLGETOPTIONSTRINGW)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETOPTIONSTRINGW LlGetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintSetOptionStringO LlPrintSetOptionStringA
    #else
      #define LlPrintSetOptionString LlPrintSetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSETOPTIONSTRINGA)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPCSTR               pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSETOPTIONSTRINGA LlPrintSetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintSetOptionString LlPrintSetOptionStringW
    #else
      #define LlPrintSetOptionStringO LlPrintSetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSETOPTIONSTRINGW)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPCWSTR              pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSETOPTIONSTRINGW LlPrintSetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetOptionStringO LlPrintGetOptionStringA
    #else
      #define LlPrintGetOptionString LlPrintGetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETOPTIONSTRINGA)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETOPTIONSTRINGA LlPrintGetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetOptionString LlPrintGetOptionStringW
    #else
      #define LlPrintGetOptionStringO LlPrintGetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETOPTIONSTRINGW)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETOPTIONSTRINGW LlPrintGetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDESIGNERPROHIBITACTION)(
	HLLJOB               hLlJob,
	INT                  nMenuID);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERPROHIBITACTION LlDesignerProhibitAction;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerProhibitFunctionO LlDesignerProhibitFunctionA
    #else
      #define LlDesignerProhibitFunction LlDesignerProhibitFunctionA
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERPROHIBITFUNCTIONA)(
	HLLJOB               hLlJob,
	LPCSTR               pszFunction);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERPROHIBITFUNCTIONA LlDesignerProhibitFunctionA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerProhibitFunction LlDesignerProhibitFunctionW
    #else
      #define LlDesignerProhibitFunctionO LlDesignerProhibitFunctionW
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERPROHIBITFUNCTIONW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszFunction);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERPROHIBITFUNCTIONW LlDesignerProhibitFunctionW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintEnableObjectO LlPrintEnableObjectA
    #else
      #define LlPrintEnableObject LlPrintEnableObjectA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTENABLEOBJECTA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjectName,
	BOOL                 bEnable);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTENABLEOBJECTA LlPrintEnableObjectA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintEnableObject LlPrintEnableObjectW
    #else
      #define LlPrintEnableObjectO LlPrintEnableObjectW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTENABLEOBJECTW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjectName,
	BOOL                 bEnable);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTENABLEOBJECTW LlPrintEnableObjectW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetFileExtensionsO LlSetFileExtensionsA
    #else
      #define LlSetFileExtensions LlSetFileExtensionsA
  #endif
typedef      INT ( DLLPROC *PFNLLSETFILEEXTENSIONSA)(
	HLLJOB               hLlJob,
	INT                  nObjType,
	LPCSTR               pszObjectExt,
	LPCSTR               pszPrinterExt,
	LPCSTR               pszSketchExt);
#endif /* IMPLEMENTATION */

extern PFNLLSETFILEEXTENSIONSA LlSetFileExtensionsA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetFileExtensions LlSetFileExtensionsW
    #else
      #define LlSetFileExtensionsO LlSetFileExtensionsW
  #endif
typedef      INT ( DLLPROC *PFNLLSETFILEEXTENSIONSW)(
	HLLJOB               hLlJob,
	INT                  nObjType,
	LPCWSTR              pszObjectExt,
	LPCWSTR              pszPrinterExt,
	LPCWSTR              pszSketchExt);
#endif /* IMPLEMENTATION */

extern PFNLLSETFILEEXTENSIONSW LlSetFileExtensionsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetTextCharsPrintedO LlPrintGetTextCharsPrintedA
    #else
      #define LlPrintGetTextCharsPrinted LlPrintGetTextCharsPrintedA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETTEXTCHARSPRINTEDA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjectName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETTEXTCHARSPRINTEDA LlPrintGetTextCharsPrintedA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetTextCharsPrinted LlPrintGetTextCharsPrintedW
    #else
      #define LlPrintGetTextCharsPrintedO LlPrintGetTextCharsPrintedW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETTEXTCHARSPRINTEDW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjectName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETTEXTCHARSPRINTEDW LlPrintGetTextCharsPrintedW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetFieldCharsPrintedO LlPrintGetFieldCharsPrintedA
    #else
      #define LlPrintGetFieldCharsPrinted LlPrintGetFieldCharsPrintedA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETFIELDCHARSPRINTEDA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjectName,
	LPCSTR               pszField);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETFIELDCHARSPRINTEDA LlPrintGetFieldCharsPrintedA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetFieldCharsPrinted LlPrintGetFieldCharsPrintedW
    #else
      #define LlPrintGetFieldCharsPrintedO LlPrintGetFieldCharsPrintedW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETFIELDCHARSPRINTEDW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjectName,
	LPCWSTR              pszField);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETFIELDCHARSPRINTEDW LlPrintGetFieldCharsPrintedW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintIsVariableUsedO LlPrintIsVariableUsedA
    #else
      #define LlPrintIsVariableUsed LlPrintIsVariableUsedA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTISVARIABLEUSEDA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTISVARIABLEUSEDA LlPrintIsVariableUsedA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintIsVariableUsed LlPrintIsVariableUsedW
    #else
      #define LlPrintIsVariableUsedO LlPrintIsVariableUsedW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTISVARIABLEUSEDW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTISVARIABLEUSEDW LlPrintIsVariableUsedW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintIsFieldUsedO LlPrintIsFieldUsedA
    #else
      #define LlPrintIsFieldUsed LlPrintIsFieldUsedA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTISFIELDUSEDA)(
	HLLJOB               hLlJob,
	LPCSTR               pszFieldName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTISFIELDUSEDA LlPrintIsFieldUsedA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintIsFieldUsed LlPrintIsFieldUsedW
    #else
      #define LlPrintIsFieldUsedO LlPrintIsFieldUsedW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTISFIELDUSEDW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszFieldName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTISFIELDUSEDW LlPrintIsFieldUsedW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintOptionsDialogTitleO LlPrintOptionsDialogTitleA
    #else
      #define LlPrintOptionsDialogTitle LlPrintOptionsDialogTitleA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTOPTIONSDIALOGTITLEA)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCSTR               pszTitle,
	LPCSTR               pszText);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTOPTIONSDIALOGTITLEA LlPrintOptionsDialogTitleA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintOptionsDialogTitle LlPrintOptionsDialogTitleW
    #else
      #define LlPrintOptionsDialogTitleO LlPrintOptionsDialogTitleW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTOPTIONSDIALOGTITLEW)(
	HLLJOB               hLlJob,
	HWND                 hWnd,
	LPCWSTR              pszTitle,
	LPCWSTR              pszText);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTOPTIONSDIALOGTITLEW LlPrintOptionsDialogTitleW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetPrinterToDefaultO LlSetPrinterToDefaultA
    #else
      #define LlSetPrinterToDefault LlSetPrinterToDefaultA
  #endif
typedef      INT ( DLLPROC *PFNLLSETPRINTERTODEFAULTA)(
	HLLJOB               hLlJob,
	INT                  nObjType,
	LPCSTR               pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLSETPRINTERTODEFAULTA LlSetPrinterToDefaultA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetPrinterToDefault LlSetPrinterToDefaultW
    #else
      #define LlSetPrinterToDefaultO LlSetPrinterToDefaultW
  #endif
typedef      INT ( DLLPROC *PFNLLSETPRINTERTODEFAULTW)(
	HLLJOB               hLlJob,
	INT                  nObjType,
	LPCWSTR              pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLSETPRINTERTODEFAULTW LlSetPrinterToDefaultW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDEFINESORTORDERSTART)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINESORTORDERSTART LlDefineSortOrderStart;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineSortOrderO LlDefineSortOrderA
    #else
      #define LlDefineSortOrder LlDefineSortOrderA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINESORTORDERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszIdentifier,
	LPCSTR               pszText);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINESORTORDERA LlDefineSortOrderA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineSortOrder LlDefineSortOrderW
    #else
      #define LlDefineSortOrderO LlDefineSortOrderW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINESORTORDERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszIdentifier,
	LPCWSTR              pszText);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINESORTORDERW LlDefineSortOrderW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetSortOrderO LlPrintGetSortOrderA
    #else
      #define LlPrintGetSortOrder LlPrintGetSortOrderA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETSORTORDERA)(
	HLLJOB               hLlJob,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETSORTORDERA LlPrintGetSortOrderA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetSortOrder LlPrintGetSortOrderW
    #else
      #define LlPrintGetSortOrderO LlPrintGetSortOrderW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETSORTORDERW)(
	HLLJOB               hLlJob,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETSORTORDERW LlPrintGetSortOrderW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineGroupingO LlDefineGroupingA
    #else
      #define LlDefineGrouping LlDefineGroupingA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEGROUPINGA)(
	HLLJOB               hLlJob,
	LPCSTR               pszSortorder,
	LPCSTR               pszIdentifier,
	LPCSTR               pszText);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEGROUPINGA LlDefineGroupingA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineGrouping LlDefineGroupingW
    #else
      #define LlDefineGroupingO LlDefineGroupingW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEGROUPINGW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszSortorder,
	LPCWSTR              pszIdentifier,
	LPCWSTR              pszText);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEGROUPINGW LlDefineGroupingW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetGroupingO LlPrintGetGroupingA
    #else
      #define LlPrintGetGrouping LlPrintGetGroupingA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETGROUPINGA)(
	HLLJOB               hLlJob,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETGROUPINGA LlPrintGetGroupingA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetGrouping LlPrintGetGroupingW
    #else
      #define LlPrintGetGroupingO LlPrintGetGroupingW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETGROUPINGW)(
	HLLJOB               hLlJob,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETGROUPINGW LlPrintGetGroupingW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlAddCtlSupportO LlAddCtlSupportA
    #else
      #define LlAddCtlSupport LlAddCtlSupportA
  #endif
typedef      INT ( DLLPROC *PFNLLADDCTLSUPPORTA)(
	HWND                 hWnd,
	UINT                 nFlags,
	LPCSTR               pszInifile);
#endif /* IMPLEMENTATION */

extern PFNLLADDCTLSUPPORTA LlAddCtlSupportA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlAddCtlSupport LlAddCtlSupportW
    #else
      #define LlAddCtlSupportO LlAddCtlSupportW
  #endif
typedef      INT ( DLLPROC *PFNLLADDCTLSUPPORTW)(
	HWND                 hWnd,
	UINT                 nFlags,
	LPCWSTR              pszInifile);
#endif /* IMPLEMENTATION */

extern PFNLLADDCTLSUPPORTW LlAddCtlSupportW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTBEGINGROUP)(
	HLLJOB               hLlJob,
	LPARAM               lParam,
	LPVOID               lpParam);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTBEGINGROUP LlPrintBeginGroup;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTENDGROUP)(
	HLLJOB               hLlJob,
	LPARAM               lParam,
	LPVOID               lpParam);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTENDGROUP LlPrintEndGroup;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTGROUPLINE)(
	HLLJOB               hLlJob,
	LPARAM               lParam,
	LPVOID               lpParam);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGROUPLINE LlPrintGroupLine;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTGROUPHEADER)(
	HLLJOB               hLlJob,
	LPARAM               lParam);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGROUPHEADER LlPrintGroupHeader;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetFilterExpressionO LlPrintGetFilterExpressionA
    #else
      #define LlPrintGetFilterExpression LlPrintGetFilterExpressionA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETFILTEREXPRESSIONA)(
	HLLJOB               hLlJob,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETFILTEREXPRESSIONA LlPrintGetFilterExpressionA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetFilterExpression LlPrintGetFilterExpressionW
    #else
      #define LlPrintGetFilterExpressionO LlPrintGetFilterExpressionW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETFILTEREXPRESSIONW)(
	HLLJOB               hLlJob,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETFILTEREXPRESSIONW LlPrintGetFilterExpressionW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTWILLMATCHFILTER)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTWILLMATCHFILTER LlPrintWillMatchFilter;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTDIDMATCHFILTER)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDIDMATCHFILTER LlPrintDidMatchFilter;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetFieldContentsO LlGetFieldContentsA
    #else
      #define LlGetFieldContents LlGetFieldContentsA
  #endif
typedef      INT ( DLLPROC *PFNLLGETFIELDCONTENTSA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETFIELDCONTENTSA LlGetFieldContentsA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetFieldContents LlGetFieldContentsW
    #else
      #define LlGetFieldContentsO LlGetFieldContentsW
  #endif
typedef      INT ( DLLPROC *PFNLLGETFIELDCONTENTSW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETFIELDCONTENTSW LlGetFieldContentsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetVariableContentsO LlGetVariableContentsA
    #else
      #define LlGetVariableContents LlGetVariableContentsA
  #endif
typedef      INT ( DLLPROC *PFNLLGETVARIABLECONTENTSA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETVARIABLECONTENTSA LlGetVariableContentsA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetVariableContents LlGetVariableContentsW
    #else
      #define LlGetVariableContentsO LlGetVariableContentsW
  #endif
typedef      INT ( DLLPROC *PFNLLGETVARIABLECONTENTSW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETVARIABLECONTENTSW LlGetVariableContentsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetSumVariableContentsO LlGetSumVariableContentsA
    #else
      #define LlGetSumVariableContents LlGetSumVariableContentsA
  #endif
typedef      INT ( DLLPROC *PFNLLGETSUMVARIABLECONTENTSA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETSUMVARIABLECONTENTSA LlGetSumVariableContentsA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetSumVariableContents LlGetSumVariableContentsW
    #else
      #define LlGetSumVariableContentsO LlGetSumVariableContentsW
  #endif
typedef      INT ( DLLPROC *PFNLLGETSUMVARIABLECONTENTSW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETSUMVARIABLECONTENTSW LlGetSumVariableContentsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUserVariableContentsO LlGetUserVariableContentsA
    #else
      #define LlGetUserVariableContents LlGetUserVariableContentsA
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSERVARIABLECONTENTSA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSERVARIABLECONTENTSA LlGetUserVariableContentsA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUserVariableContents LlGetUserVariableContentsW
    #else
      #define LlGetUserVariableContentsO LlGetUserVariableContentsW
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSERVARIABLECONTENTSW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSERVARIABLECONTENTSW LlGetUserVariableContentsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetVariableTypeO LlGetVariableTypeA
    #else
      #define LlGetVariableType LlGetVariableTypeA
  #endif
typedef      INT ( DLLPROC *PFNLLGETVARIABLETYPEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName);
#endif /* IMPLEMENTATION */

extern PFNLLGETVARIABLETYPEA LlGetVariableTypeA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetVariableType LlGetVariableTypeW
    #else
      #define LlGetVariableTypeO LlGetVariableTypeW
  #endif
typedef      INT ( DLLPROC *PFNLLGETVARIABLETYPEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName);
#endif /* IMPLEMENTATION */

extern PFNLLGETVARIABLETYPEW LlGetVariableTypeW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetFieldTypeO LlGetFieldTypeA
    #else
      #define LlGetFieldType LlGetFieldTypeA
  #endif
typedef      INT ( DLLPROC *PFNLLGETFIELDTYPEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName);
#endif /* IMPLEMENTATION */

extern PFNLLGETFIELDTYPEA LlGetFieldTypeA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetFieldType LlGetFieldTypeW
    #else
      #define LlGetFieldTypeO LlGetFieldTypeW
  #endif
typedef      INT ( DLLPROC *PFNLLGETFIELDTYPEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName);
#endif /* IMPLEMENTATION */

extern PFNLLGETFIELDTYPEW LlGetFieldTypeW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetColumnInfoO LlPrintGetColumnInfoA
    #else
      #define LlPrintGetColumnInfo LlPrintGetColumnInfoA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETCOLUMNINFOA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjectName,
	INT                  nCol,
	PSCLLCOLUMN          pCol);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETCOLUMNINFOA LlPrintGetColumnInfoA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetColumnInfo LlPrintGetColumnInfoW
    #else
      #define LlPrintGetColumnInfoO LlPrintGetColumnInfoW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETCOLUMNINFOW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjectName,
	INT                  nCol,
	PSCLLCOLUMN          pCol);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETCOLUMNINFOW LlPrintGetColumnInfoW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetPrinterDefaultsDirO LlSetPrinterDefaultsDirA
    #else
      #define LlSetPrinterDefaultsDir LlSetPrinterDefaultsDirA
  #endif
typedef      INT ( DLLPROC *PFNLLSETPRINTERDEFAULTSDIRA)(
	HLLJOB               hLlJob,
	LPCSTR               pszDir);
#endif /* IMPLEMENTATION */

extern PFNLLSETPRINTERDEFAULTSDIRA LlSetPrinterDefaultsDirA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetPrinterDefaultsDir LlSetPrinterDefaultsDirW
    #else
      #define LlSetPrinterDefaultsDirO LlSetPrinterDefaultsDirW
  #endif
typedef      INT ( DLLPROC *PFNLLSETPRINTERDEFAULTSDIRW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszDir);
#endif /* IMPLEMENTATION */

extern PFNLLSETPRINTERDEFAULTSDIRW LlSetPrinterDefaultsDirW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlCreateSketchO LlCreateSketchA
    #else
      #define LlCreateSketch LlCreateSketchA
  #endif
typedef      INT ( DLLPROC *PFNLLCREATESKETCHA)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCSTR               lpszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLCREATESKETCHA LlCreateSketchA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlCreateSketch LlCreateSketchW
    #else
      #define LlCreateSketchO LlCreateSketchW
  #endif
typedef      INT ( DLLPROC *PFNLLCREATESKETCHW)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCWSTR              lpszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLCREATESKETCHW LlCreateSketchW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLVIEWERPROHIBITACTION)(
	HLLJOB               hLlJob,
	INT                  nMenuID);
#endif /* IMPLEMENTATION */

extern PFNLLVIEWERPROHIBITACTION LlViewerProhibitAction;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintCopyPrinterConfigurationO LlPrintCopyPrinterConfigurationA
    #else
      #define LlPrintCopyPrinterConfiguration LlPrintCopyPrinterConfigurationA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTCOPYPRINTERCONFIGURATIONA)(
	HLLJOB               hLlJob,
	LPCSTR               lpszFilename,
	INT                  nFunction);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTCOPYPRINTERCONFIGURATIONA LlPrintCopyPrinterConfigurationA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintCopyPrinterConfiguration LlPrintCopyPrinterConfigurationW
    #else
      #define LlPrintCopyPrinterConfigurationO LlPrintCopyPrinterConfigurationW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTCOPYPRINTERCONFIGURATIONW)(
	HLLJOB               hLlJob,
	LPCWSTR              lpszFilename,
	INT                  nFunction);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTCOPYPRINTERCONFIGURATIONW LlPrintCopyPrinterConfigurationW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetPrinterInPrinterFileO LlSetPrinterInPrinterFileA
    #else
      #define LlSetPrinterInPrinterFile LlSetPrinterInPrinterFileA
  #endif
typedef      INT ( DLLPROC *PFNLLSETPRINTERINPRINTERFILEA)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCSTR               pszObjName,
	INT                  nPrinterIndex,
	LPCSTR               pszPrinter,
	_PCDEVMODEA          pDevMode);
#endif /* IMPLEMENTATION */

extern PFNLLSETPRINTERINPRINTERFILEA LlSetPrinterInPrinterFileA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetPrinterInPrinterFile LlSetPrinterInPrinterFileW
    #else
      #define LlSetPrinterInPrinterFileO LlSetPrinterInPrinterFileW
  #endif
typedef      INT ( DLLPROC *PFNLLSETPRINTERINPRINTERFILEW)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCWSTR              pszObjName,
	INT                  nPrinterIndex,
	LPCWSTR              pszPrinter,
	_PCDEVMODEW          pDevMode);
#endif /* IMPLEMENTATION */

extern PFNLLSETPRINTERINPRINTERFILEW LlSetPrinterInPrinterFileW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef HLLRTFOBJ ( DLLPROC *PFNLLRTFCREATEOBJECT)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLRTFCREATEOBJECT LlRTFCreateObject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLRTFDELETEOBJECT)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF);
#endif /* IMPLEMENTATION */

extern PFNLLRTFDELETEOBJECT LlRTFDeleteObject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlRTFSetTextO LlRTFSetTextA
    #else
      #define LlRTFSetText LlRTFSetTextA
  #endif
typedef      INT ( DLLPROC *PFNLLRTFSETTEXTA)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	LPCSTR               pszText);
#endif /* IMPLEMENTATION */

extern PFNLLRTFSETTEXTA LlRTFSetTextA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlRTFSetText LlRTFSetTextW
    #else
      #define LlRTFSetTextO LlRTFSetTextW
  #endif
typedef      INT ( DLLPROC *PFNLLRTFSETTEXTW)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	LPCWSTR              pszText);
#endif /* IMPLEMENTATION */

extern PFNLLRTFSETTEXTW LlRTFSetTextW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     UINT ( DLLPROC *PFNLLRTFGETTEXTLENGTH)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	INT                  nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLRTFGETTEXTLENGTH LlRTFGetTextLength;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlRTFGetTextO LlRTFGetTextA
    #else
      #define LlRTFGetText LlRTFGetTextA
  #endif
typedef      INT ( DLLPROC *PFNLLRTFGETTEXTA)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	INT                  nFlags,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLRTFGETTEXTA LlRTFGetTextA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlRTFGetText LlRTFGetTextW
    #else
      #define LlRTFGetTextO LlRTFGetTextW
  #endif
typedef      INT ( DLLPROC *PFNLLRTFGETTEXTW)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	INT                  nFlags,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLRTFGETTEXTW LlRTFGetTextW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLRTFEDITOBJECT)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	HWND                 hWnd,
	HDC                  hPrnDC,
	INT                  nProjectType,
	BOOL                 bModal);
#endif /* IMPLEMENTATION */

extern PFNLLRTFEDITOBJECT LlRTFEditObject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLRTFCOPYTOCLIPBOARD)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF);
#endif /* IMPLEMENTATION */

extern PFNLLRTFCOPYTOCLIPBOARD LlRTFCopyToClipboard;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLRTFDISPLAY)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	HDC                  hDC,
	_PRECT               pRC,
	BOOL                 bRestart,
	LLPUINT              pnState);
#endif /* IMPLEMENTATION */

extern PFNLLRTFDISPLAY LlRTFDisplay;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLRTFEDITORPROHIBITACTION)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	INT                  nControlID);
#endif /* IMPLEMENTATION */

extern PFNLLRTFEDITORPROHIBITACTION LlRTFEditorProhibitAction;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLRTFEDITORINVOKEACTION)(
	HLLJOB               hLlJob,
	HLLRTFOBJ            hRTF,
	INT                  nControlID);
#endif /* IMPLEMENTATION */

extern PFNLLRTFEDITORINVOKEACTION LlRTFEditorInvokeAction;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDebugOutputO LlDebugOutputA
    #else
      #define LlDebugOutput LlDebugOutputA
  #endif
typedef     void ( DLLPROC *PFNLLDEBUGOUTPUTA)(
	INT                  nIndent,
	LPCSTR               pszText);
#endif /* IMPLEMENTATION */

extern PFNLLDEBUGOUTPUTA LlDebugOutputA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDebugOutput LlDebugOutputW
    #else
      #define LlDebugOutputO LlDebugOutputW
  #endif
typedef     void ( DLLPROC *PFNLLDEBUGOUTPUTW)(
	INT                  nIndent,
	LPCWSTR              pszText);
#endif /* IMPLEMENTATION */

extern PFNLLDEBUGOUTPUTW LlDebugOutputW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef HLISTPOS ( DLLPROC *PFNLLENUMGETFIRSTVAR)(
	HLLJOB               hLlJob,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLENUMGETFIRSTVAR LlEnumGetFirstVar;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef HLISTPOS ( DLLPROC *PFNLLENUMGETFIRSTFIELD)(
	HLLJOB               hLlJob,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLENUMGETFIRSTFIELD LlEnumGetFirstField;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef HLISTPOS ( DLLPROC *PFNLLENUMGETNEXTENTRY)(
	HLLJOB               hLlJob,
	HLISTPOS             nPos,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLENUMGETNEXTENTRY LlEnumGetNextEntry;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlEnumGetEntryO LlEnumGetEntryA
    #else
      #define LlEnumGetEntry LlEnumGetEntryA
  #endif
typedef      INT ( DLLPROC *PFNLLENUMGETENTRYA)(
	HLLJOB               hLlJob,
	HLISTPOS             nPos,
	LPSTR                pszNameBuf,
	UINT                 nNameBufSize,
	LPSTR                pszContBuf,
	UINT                 nContBufSize,
	_LPHANDLE            pHandle,
	_LPINT               pType);
#endif /* IMPLEMENTATION */

extern PFNLLENUMGETENTRYA LlEnumGetEntryA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlEnumGetEntry LlEnumGetEntryW
    #else
      #define LlEnumGetEntryO LlEnumGetEntryW
  #endif
typedef      INT ( DLLPROC *PFNLLENUMGETENTRYW)(
	HLLJOB               hLlJob,
	HLISTPOS             nPos,
	LPWSTR               pszNameBuf,
	UINT                 nNameBufSize,
	LPWSTR               pszContBuf,
	UINT                 nContBufSize,
	_LPHANDLE            pHandle,
	_LPINT               pType);
#endif /* IMPLEMENTATION */

extern PFNLLENUMGETENTRYW LlEnumGetEntryW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTRESETOBJECTSTATES)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTRESETOBJECTSTATES LlPrintResetObjectStates;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXSetParameterO LlXSetParameterA
    #else
      #define LlXSetParameter LlXSetParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLXSETPARAMETERA)(
	HLLJOB               hLlJob,
	INT                  nExtensionType,
	LPCSTR               pszExtensionName,
	LPCSTR               pszKey,
	LPCSTR               pszValue);
#endif /* IMPLEMENTATION */

extern PFNLLXSETPARAMETERA LlXSetParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXSetParameter LlXSetParameterW
    #else
      #define LlXSetParameterO LlXSetParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLXSETPARAMETERW)(
	HLLJOB               hLlJob,
	INT                  nExtensionType,
	LPCWSTR              pszExtensionName,
	LPCWSTR              pszKey,
	LPCWSTR              pszValue);
#endif /* IMPLEMENTATION */

extern PFNLLXSETPARAMETERW LlXSetParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXGetParameterO LlXGetParameterA
    #else
      #define LlXGetParameter LlXGetParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLXGETPARAMETERA)(
	HLLJOB               hLlJob,
	INT                  nExtensionType,
	LPCSTR               pszExtensionName,
	LPCSTR               pszKey,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLXGETPARAMETERA LlXGetParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXGetParameter LlXGetParameterW
    #else
      #define LlXGetParameterO LlXGetParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLXGETPARAMETERW)(
	HLLJOB               hLlJob,
	INT                  nExtensionType,
	LPCWSTR              pszExtensionName,
	LPCWSTR              pszKey,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLXGETPARAMETERW LlXGetParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTRESETPROJECTSTATE)(
	HLLJOB               hJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTRESETPROJECTSTATE LlPrintResetProjectState;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLDEFINECHARTFIELDSTART)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINECHARTFIELDSTART LlDefineChartFieldStart;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineChartFieldExtO LlDefineChartFieldExtA
    #else
      #define LlDefineChartFieldExt LlDefineChartFieldExtA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINECHARTFIELDEXTA)(
	HLLJOB               hLlJob,
	LPCSTR               pszVarName,
	LPCSTR               pszContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINECHARTFIELDEXTA LlDefineChartFieldExtA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineChartFieldExt LlDefineChartFieldExtW
    #else
      #define LlDefineChartFieldExtO LlDefineChartFieldExtW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINECHARTFIELDEXTW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszVarName,
	LPCWSTR              pszContents,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINECHARTFIELDEXTW LlDefineChartFieldExtW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTDECLARECHARTROW)(
	HLLJOB               hLlJob,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDECLARECHARTROW LlPrintDeclareChartRow;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTGETCHARTOBJECTCOUNT)(
	HLLJOB               hLlJob,
	UINT                 nType);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETCHARTOBJECTCOUNT LlPrintGetChartObjectCount;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintIsChartFieldUsedO LlPrintIsChartFieldUsedA
    #else
      #define LlPrintIsChartFieldUsed LlPrintIsChartFieldUsedA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTISCHARTFIELDUSEDA)(
	HLLJOB               hLlJob,
	LPCSTR               pszFieldName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTISCHARTFIELDUSEDA LlPrintIsChartFieldUsedA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintIsChartFieldUsed LlPrintIsChartFieldUsedW
    #else
      #define LlPrintIsChartFieldUsedO LlPrintIsChartFieldUsedW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTISCHARTFIELDUSEDW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszFieldName);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTISCHARTFIELDUSEDW LlPrintIsChartFieldUsedW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetChartFieldContentsO LlGetChartFieldContentsA
    #else
      #define LlGetChartFieldContents LlGetChartFieldContentsA
  #endif
typedef      INT ( DLLPROC *PFNLLGETCHARTFIELDCONTENTSA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETCHARTFIELDCONTENTSA LlGetChartFieldContentsA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetChartFieldContents LlGetChartFieldContentsW
    #else
      #define LlGetChartFieldContentsO LlGetChartFieldContentsW
  #endif
typedef      INT ( DLLPROC *PFNLLGETCHARTFIELDCONTENTSW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETCHARTFIELDCONTENTSW LlGetChartFieldContentsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef HLISTPOS ( DLLPROC *PFNLLENUMGETFIRSTCHARTFIELD)(
	HLLJOB               hLlJob,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLENUMGETFIRSTCHARTFIELD LlEnumGetFirstChartField;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef  FARPROC ( DLLPROC *PFNLLSETNOTIFICATIONCALLBACKEXT)(
	HLLJOB               hLlJob,
	INT                  nEvent,
	FARPROC              lpfnNotify);
#endif /* IMPLEMENTATION */

extern PFNLLSETNOTIFICATIONCALLBACKEXT LlSetNotificationCallbackExt;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLEXPREVALUATEVAR)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr,
	PVARIANT             pVar,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLEXPREVALUATEVAR LlExprEvaluateVar;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLEXPRTYPEVAR)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRTYPEVAR LlExprTypeVar;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetPrinterFromPrinterFileO LlGetPrinterFromPrinterFileA
    #else
      #define LlGetPrinterFromPrinterFile LlGetPrinterFromPrinterFileA
  #endif
typedef      INT ( DLLPROC *PFNLLGETPRINTERFROMPRINTERFILEA)(
	HLLJOB               hJob,
	UINT                 nObjType,
	LPCSTR               pszObjectName,
	INT                  nPrinter,
	LPSTR                pszPrinter,
	LLPUINT              pnPrinterBufSize,
	_PDEVMODEA           pDevMode,
	LLPUINT              pnDevModeBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETPRINTERFROMPRINTERFILEA LlGetPrinterFromPrinterFileA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetPrinterFromPrinterFile LlGetPrinterFromPrinterFileW
    #else
      #define LlGetPrinterFromPrinterFileO LlGetPrinterFromPrinterFileW
  #endif
typedef      INT ( DLLPROC *PFNLLGETPRINTERFROMPRINTERFILEW)(
	HLLJOB               hJob,
	UINT                 nObjType,
	LPCWSTR              pszObjectName,
	INT                  nPrinter,
	LPWSTR               pszPrinter,
	LLPUINT              pnPrinterBufSize,
	_PDEVMODEW           pDevMode,
	LLPUINT              pnDevModeBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETPRINTERFROMPRINTERFILEW LlGetPrinterFromPrinterFileW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetRemainingSpacePerTableO LlPrintGetRemainingSpacePerTableA
    #else
      #define LlPrintGetRemainingSpacePerTable LlPrintGetRemainingSpacePerTableA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETREMAININGSPACEPERTABLEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszField,
	INT                  nDimension);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETREMAININGSPACEPERTABLEA LlPrintGetRemainingSpacePerTableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetRemainingSpacePerTable LlPrintGetRemainingSpacePerTableW
    #else
      #define LlPrintGetRemainingSpacePerTableO LlPrintGetRemainingSpacePerTableW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETREMAININGSPACEPERTABLEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszField,
	INT                  nDimension);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETREMAININGSPACEPERTABLEW LlPrintGetRemainingSpacePerTableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLDRAWTOOLBARBACKGROUND)(
	HDC                  hDC,
	_PRECT               pRC,
	BOOL                 bHorz,
	INT                  nTBMode);
#endif /* IMPLEMENTATION */

extern PFNLLDRAWTOOLBARBACKGROUND LlDrawToolbarBackground;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetDefaultProjectParameterO LlSetDefaultProjectParameterA
    #else
      #define LlSetDefaultProjectParameter LlSetDefaultProjectParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLSETDEFAULTPROJECTPARAMETERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszParameter,
	LPCSTR               pszValue,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLSETDEFAULTPROJECTPARAMETERA LlSetDefaultProjectParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlSetDefaultProjectParameter LlSetDefaultProjectParameterW
    #else
      #define LlSetDefaultProjectParameterO LlSetDefaultProjectParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLSETDEFAULTPROJECTPARAMETERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszParameter,
	LPCWSTR              pszValue,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLSETDEFAULTPROJECTPARAMETERW LlSetDefaultProjectParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetDefaultProjectParameterO LlGetDefaultProjectParameterA
    #else
      #define LlGetDefaultProjectParameter LlGetDefaultProjectParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLGETDEFAULTPROJECTPARAMETERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszParameter,
	LPSTR                pszBuffer,
	INT                  nBufSize,
	_LPUINT              pnFlags);
#endif /* IMPLEMENTATION */

extern PFNLLGETDEFAULTPROJECTPARAMETERA LlGetDefaultProjectParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetDefaultProjectParameter LlGetDefaultProjectParameterW
    #else
      #define LlGetDefaultProjectParameterO LlGetDefaultProjectParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLGETDEFAULTPROJECTPARAMETERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszParameter,
	LPWSTR               pszBuffer,
	INT                  nBufSize,
	_LPUINT              pnFlags);
#endif /* IMPLEMENTATION */

extern PFNLLGETDEFAULTPROJECTPARAMETERW LlGetDefaultProjectParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintSetProjectParameterO LlPrintSetProjectParameterA
    #else
      #define LlPrintSetProjectParameter LlPrintSetProjectParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSETPROJECTPARAMETERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszParameter,
	LPCSTR               pszValue,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSETPROJECTPARAMETERA LlPrintSetProjectParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintSetProjectParameter LlPrintSetProjectParameterW
    #else
      #define LlPrintSetProjectParameterO LlPrintSetProjectParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTSETPROJECTPARAMETERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszParameter,
	LPCWSTR              pszValue,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTSETPROJECTPARAMETERW LlPrintSetProjectParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetProjectParameterO LlPrintGetProjectParameterA
    #else
      #define LlPrintGetProjectParameter LlPrintGetProjectParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETPROJECTPARAMETERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszParameter,
	BOOL                 bEvaluated,
	LPSTR                pszBuffer,
	INT                  nBufSize,
	_LPUINT              pnFlags);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETPROJECTPARAMETERA LlPrintGetProjectParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintGetProjectParameter LlPrintGetProjectParameterW
    #else
      #define LlPrintGetProjectParameterO LlPrintGetProjectParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTGETPROJECTPARAMETERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszParameter,
	BOOL                 bEvaluated,
	LPWSTR               pszBuffer,
	INT                  nBufSize,
	_LPUINT              pnFlags);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTGETPROJECTPARAMETERW LlPrintGetProjectParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     BOOL ( DLLPROC *PFNLLCREATEOBJECT)(
	CTL_GUID             pIID,
	CTL_PPUNK            ppI);
#endif /* IMPLEMENTATION */

extern PFNLLCREATEOBJECT LlCreateObject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprContainsVariableO LlExprContainsVariableA
    #else
      #define LlExprContainsVariable LlExprContainsVariableA
  #endif
typedef      INT ( DLLPROC *PFNLLEXPRCONTAINSVARIABLEA)(
	HLLJOB               hLlJob,
	HLLEXPR              hExpr,
	LPCSTR               pszVariable);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRCONTAINSVARIABLEA LlExprContainsVariableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprContainsVariable LlExprContainsVariableW
    #else
      #define LlExprContainsVariableO LlExprContainsVariableW
  #endif
typedef      INT ( DLLPROC *PFNLLEXPRCONTAINSVARIABLEW)(
	HLLJOB               hLlJob,
	HLLEXPR              hExpr,
	LPCWSTR              pszVariable);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRCONTAINSVARIABLEW LlExprContainsVariableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLEXPRISCONSTANT)(
	HLLJOB               hLlJob,
	HLLEXPR              hExpr);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRISCONSTANT LlExprIsConstant;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlLocSystemTimeFromLocaleStringO LlLocSystemTimeFromLocaleStringA
    #else
      #define LlLocSystemTimeFromLocaleString LlLocSystemTimeFromLocaleStringA
  #endif
typedef      INT ( DLLPROC *PFNLLLOCSYSTEMTIMEFROMLOCALESTRINGA)(
	LCID                 nLCID,
	LPCSTR               pszDateTime,
	_PSYSTEMTIME         pST,
	BOOL                 bAddCentury);
#endif /* IMPLEMENTATION */

extern PFNLLLOCSYSTEMTIMEFROMLOCALESTRINGA LlLocSystemTimeFromLocaleStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlLocSystemTimeFromLocaleString LlLocSystemTimeFromLocaleStringW
    #else
      #define LlLocSystemTimeFromLocaleStringO LlLocSystemTimeFromLocaleStringW
  #endif
typedef      INT ( DLLPROC *PFNLLLOCSYSTEMTIMEFROMLOCALESTRINGW)(
	LCID                 nLCID,
	LPCWSTR              pszDateTime,
	_PSYSTEMTIME         pST,
	BOOL                 bAddCentury);
#endif /* IMPLEMENTATION */

extern PFNLLLOCSYSTEMTIMEFROMLOCALESTRINGW LlLocSystemTimeFromLocaleStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlLocSystemTimeToLocaleStringO LlLocSystemTimeToLocaleStringA
    #else
      #define LlLocSystemTimeToLocaleString LlLocSystemTimeToLocaleStringA
  #endif
typedef      INT ( DLLPROC *PFNLLLOCSYSTEMTIMETOLOCALESTRINGA)(
	LCID                 nLCID,
	_PCSYSTEMTIME        pST,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLLOCSYSTEMTIMETOLOCALESTRINGA LlLocSystemTimeToLocaleStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlLocSystemTimeToLocaleString LlLocSystemTimeToLocaleStringW
    #else
      #define LlLocSystemTimeToLocaleStringO LlLocSystemTimeToLocaleStringW
  #endif
typedef      INT ( DLLPROC *PFNLLLOCSYSTEMTIMETOLOCALESTRINGW)(
	LCID                 nLCID,
	_PCSYSTEMTIME        pST,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLLOCSYSTEMTIMETOLOCALESTRINGW LlLocSystemTimeToLocaleStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlLocConvertCountryToO LlLocConvertCountryToA
    #else
      #define LlLocConvertCountryTo LlLocConvertCountryToA
  #endif
typedef      INT ( DLLPROC *PFNLLLOCCONVERTCOUNTRYTOA)(
	HLLJOB               hJob,
	LPCSTR               pszCountryID,
	INT                  nIndex,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLLOCCONVERTCOUNTRYTOA LlLocConvertCountryToA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlLocConvertCountryTo LlLocConvertCountryToW
    #else
      #define LlLocConvertCountryToO LlLocConvertCountryToW
  #endif
typedef      INT ( DLLPROC *PFNLLLOCCONVERTCOUNTRYTOW)(
	HLLJOB               hJob,
	LPCWSTR              pszCountryID,
	INT                  nIndex,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLLOCCONVERTCOUNTRYTOW LlLocConvertCountryToW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlProfileStartO LlProfileStartA
    #else
      #define LlProfileStart LlProfileStartA
  #endif
typedef      INT ( DLLPROC *PFNLLPROFILESTARTA)(
	HANDLE               hThread,
	LPCSTR               pszDescr,
	LPCSTR               pszFilename,
	INT                  nTicksMS);
#endif /* IMPLEMENTATION */

extern PFNLLPROFILESTARTA LlProfileStartA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlProfileStart LlProfileStartW
    #else
      #define LlProfileStartO LlProfileStartW
  #endif
typedef      INT ( DLLPROC *PFNLLPROFILESTARTW)(
	HANDLE               hThread,
	LPCWSTR              pszDescr,
	LPCWSTR              pszFilename,
	INT                  nTicksMS);
#endif /* IMPLEMENTATION */

extern PFNLLPROFILESTARTW LlProfileStartW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLPROFILEEND)(
	HANDLE               hThread);
#endif /* IMPLEMENTATION */

extern PFNLLPROFILEEND LlProfileEnd;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLDUMPMEMORY)(
	BOOL                 bDumpAll);
#endif /* IMPLEMENTATION */

extern PFNLLDUMPMEMORY LlDumpMemory;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableO LlDbAddTableA
    #else
      #define LlDbAddTable LlDbAddTableA
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLEA)(
	HLLJOB               hJob,
	LPCSTR               pszTableID,
	LPCSTR               pszDisplayName);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLEA LlDbAddTableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTable LlDbAddTableW
    #else
      #define LlDbAddTableO LlDbAddTableW
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLEW)(
	HLLJOB               hJob,
	LPCWSTR              pszTableID,
	LPCWSTR              pszDisplayName);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLEW LlDbAddTableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableRelationO LlDbAddTableRelationA
    #else
      #define LlDbAddTableRelation LlDbAddTableRelationA
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLERELATIONA)(
	HLLJOB               hJob,
	LPCSTR               pszTableID,
	LPCSTR               pszParentTableID,
	LPCSTR               pszRelationID,
	LPCSTR               pszRelationDisplayName);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLERELATIONA LlDbAddTableRelationA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableRelation LlDbAddTableRelationW
    #else
      #define LlDbAddTableRelationO LlDbAddTableRelationW
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLERELATIONW)(
	HLLJOB               hJob,
	LPCWSTR              pszTableID,
	LPCWSTR              pszParentTableID,
	LPCWSTR              pszRelationID,
	LPCWSTR              pszRelationDisplayName);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLERELATIONW LlDbAddTableRelationW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableSortOrderO LlDbAddTableSortOrderA
    #else
      #define LlDbAddTableSortOrder LlDbAddTableSortOrderA
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLESORTORDERA)(
	HLLJOB               hJob,
	LPCSTR               pszTableID,
	LPCSTR               pszSortOrderID,
	LPCSTR               pszSortOrderDisplayName);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLESORTORDERA LlDbAddTableSortOrderA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableSortOrder LlDbAddTableSortOrderW
    #else
      #define LlDbAddTableSortOrderO LlDbAddTableSortOrderW
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLESORTORDERW)(
	HLLJOB               hJob,
	LPCWSTR              pszTableID,
	LPCWSTR              pszSortOrderID,
	LPCWSTR              pszSortOrderDisplayName);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLESORTORDERW LlDbAddTableSortOrderW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintDbGetCurrentTableO LlPrintDbGetCurrentTableA
    #else
      #define LlPrintDbGetCurrentTable LlPrintDbGetCurrentTableA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTDBGETCURRENTTABLEA)(
	HLLJOB               hJob,
	LPSTR                pszTableID,
	UINT                 nTableIDLength,
	BOOL                 bCompletePath);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDBGETCURRENTTABLEA LlPrintDbGetCurrentTableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintDbGetCurrentTable LlPrintDbGetCurrentTableW
    #else
      #define LlPrintDbGetCurrentTableO LlPrintDbGetCurrentTableW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTDBGETCURRENTTABLEW)(
	HLLJOB               hJob,
	LPWSTR               pszTableID,
	UINT                 nTableIDLength,
	BOOL                 bCompletePath);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDBGETCURRENTTABLEW LlPrintDbGetCurrentTableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintDbGetCurrentTableRelationO LlPrintDbGetCurrentTableRelationA
    #else
      #define LlPrintDbGetCurrentTableRelation LlPrintDbGetCurrentTableRelationA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTDBGETCURRENTTABLERELATIONA)(
	HLLJOB               hJob,
	LPSTR                pszRelationID,
	UINT                 nRelationIDLength);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDBGETCURRENTTABLERELATIONA LlPrintDbGetCurrentTableRelationA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintDbGetCurrentTableRelation LlPrintDbGetCurrentTableRelationW
    #else
      #define LlPrintDbGetCurrentTableRelationO LlPrintDbGetCurrentTableRelationW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTDBGETCURRENTTABLERELATIONW)(
	HLLJOB               hJob,
	LPWSTR               pszRelationID,
	UINT                 nRelationIDLength);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDBGETCURRENTTABLERELATIONW LlPrintDbGetCurrentTableRelationW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintDbGetCurrentTableSortOrderO LlPrintDbGetCurrentTableSortOrderA
    #else
      #define LlPrintDbGetCurrentTableSortOrder LlPrintDbGetCurrentTableSortOrderA
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTDBGETCURRENTTABLESORTORDERA)(
	HLLJOB               hJob,
	LPSTR                pszSortOrderID,
	UINT                 nSortOrderIDLength);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDBGETCURRENTTABLESORTORDERA LlPrintDbGetCurrentTableSortOrderA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlPrintDbGetCurrentTableSortOrder LlPrintDbGetCurrentTableSortOrderW
    #else
      #define LlPrintDbGetCurrentTableSortOrderO LlPrintDbGetCurrentTableSortOrderW
  #endif
typedef      INT ( DLLPROC *PFNLLPRINTDBGETCURRENTTABLESORTORDERW)(
	HLLJOB               hJob,
	LPWSTR               pszSortOrderID,
	UINT                 nSortOrderIDLength);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDBGETCURRENTTABLESORTORDERW LlPrintDbGetCurrentTableSortOrderW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDBDUMPSTRUCTURE)(
	HLLJOB               hJob);
#endif /* IMPLEMENTATION */

extern PFNLLDBDUMPSTRUCTURE LlDbDumpStructure;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPRINTDBGETROOTTABLECOUNT)(
	HLLJOB               hJob);
#endif /* IMPLEMENTATION */

extern PFNLLPRINTDBGETROOTTABLECOUNT LlPrintDbGetRootTableCount;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbSetMasterTableO LlDbSetMasterTableA
    #else
      #define LlDbSetMasterTable LlDbSetMasterTableA
  #endif
typedef      INT ( DLLPROC *PFNLLDBSETMASTERTABLEA)(
	HLLJOB               hJob,
	LPCSTR               pszTableID);
#endif /* IMPLEMENTATION */

extern PFNLLDBSETMASTERTABLEA LlDbSetMasterTableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbSetMasterTable LlDbSetMasterTableW
    #else
      #define LlDbSetMasterTableO LlDbSetMasterTableW
  #endif
typedef      INT ( DLLPROC *PFNLLDBSETMASTERTABLEW)(
	HLLJOB               hJob,
	LPCWSTR              pszTableID);
#endif /* IMPLEMENTATION */

extern PFNLLDBSETMASTERTABLEW LlDbSetMasterTableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbGetMasterTableO LlDbGetMasterTableA
    #else
      #define LlDbGetMasterTable LlDbGetMasterTableA
  #endif
typedef      INT ( DLLPROC *PFNLLDBGETMASTERTABLEA)(
	HLLJOB               hJob,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDBGETMASTERTABLEA LlDbGetMasterTableA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbGetMasterTable LlDbGetMasterTableW
    #else
      #define LlDbGetMasterTableO LlDbGetMasterTableW
  #endif
typedef      INT ( DLLPROC *PFNLLDBGETMASTERTABLEW)(
	HLLJOB               hJob,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDBGETMASTERTABLEW LlDbGetMasterTableW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXSetExportParameterO LlXSetExportParameterA
    #else
      #define LlXSetExportParameter LlXSetExportParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLXSETEXPORTPARAMETERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszExtensionName,
	LPCSTR               pszKey,
	LPCSTR               pszValue);
#endif /* IMPLEMENTATION */

extern PFNLLXSETEXPORTPARAMETERA LlXSetExportParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXSetExportParameter LlXSetExportParameterW
    #else
      #define LlXSetExportParameterO LlXSetExportParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLXSETEXPORTPARAMETERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszExtensionName,
	LPCWSTR              pszKey,
	LPCWSTR              pszValue);
#endif /* IMPLEMENTATION */

extern PFNLLXSETEXPORTPARAMETERW LlXSetExportParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXGetExportParameterO LlXGetExportParameterA
    #else
      #define LlXGetExportParameter LlXGetExportParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLXGETEXPORTPARAMETERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszExtensionName,
	LPCSTR               pszKey,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLXGETEXPORTPARAMETERA LlXGetExportParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXGetExportParameter LlXGetExportParameterW
    #else
      #define LlXGetExportParameterO LlXGetExportParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLXGETEXPORTPARAMETERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszExtensionName,
	LPCWSTR              pszKey,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLXGETEXPORTPARAMETERW LlXGetExportParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXlatNameO LlXlatNameA
    #else
      #define LlXlatName LlXlatNameA
  #endif
typedef      INT ( DLLPROC *PFNLLXLATNAMEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLXLATNAMEA LlXlatNameA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlXlatName LlXlatNameW
    #else
      #define LlXlatNameO LlXlatNameW
  #endif
typedef      INT ( DLLPROC *PFNLLXLATNAMEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLXLATNAMEW LlXlatNameW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableVarO LlDefineVariableVarA
    #else
      #define LlDefineVariableVar LlDefineVariableVarA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEVARA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	const PVARIANT       pValue,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEVARA LlDefineVariableVarA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineVariableVar LlDefineVariableVarW
    #else
      #define LlDefineVariableVarO LlDefineVariableVarW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEVARIABLEVARW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	const PVARIANT       pValue,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEVARIABLEVARW LlDefineVariableVarW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineFieldVarO LlDefineFieldVarA
    #else
      #define LlDefineFieldVar LlDefineFieldVarA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDVARA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	const PVARIANT       pValue,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDVARA LlDefineFieldVarA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineFieldVar LlDefineFieldVarW
    #else
      #define LlDefineFieldVarO LlDefineFieldVarW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINEFIELDVARW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	const PVARIANT       pValue,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINEFIELDVARW LlDefineFieldVarW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineChartFieldVarO LlDefineChartFieldVarA
    #else
      #define LlDefineChartFieldVar LlDefineChartFieldVarA
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINECHARTFIELDVARA)(
	HLLJOB               hLlJob,
	LPCSTR               pszName,
	const PVARIANT       pValue,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINECHARTFIELDVARA LlDefineChartFieldVarA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDefineChartFieldVar LlDefineChartFieldVarW
    #else
      #define LlDefineChartFieldVarO LlDefineChartFieldVarW
  #endif
typedef      INT ( DLLPROC *PFNLLDEFINECHARTFIELDVARW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszName,
	const PVARIANT       pValue,
	INT                  lPara,
	LPVOID               lpPtr);
#endif /* IMPLEMENTATION */

extern PFNLLDEFINECHARTFIELDVARW LlDefineChartFieldVarW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerProhibitEditingObjectO LlDesignerProhibitEditingObjectA
    #else
      #define LlDesignerProhibitEditingObject LlDesignerProhibitEditingObjectA
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERPROHIBITEDITINGOBJECTA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObject);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERPROHIBITEDITINGOBJECTA LlDesignerProhibitEditingObjectA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerProhibitEditingObject LlDesignerProhibitEditingObjectW
    #else
      #define LlDesignerProhibitEditingObjectO LlDesignerProhibitEditingObjectW
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERPROHIBITEDITINGOBJECTW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObject);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERPROHIBITEDITINGOBJECTW LlDesignerProhibitEditingObjectW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUsedIdentifiersO LlGetUsedIdentifiersA
    #else
      #define LlGetUsedIdentifiers LlGetUsedIdentifiersA
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSEDIDENTIFIERSA)(
	HLLJOB               hLlJob,
	LPCSTR               pszProjectName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSEDIDENTIFIERSA LlGetUsedIdentifiersA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUsedIdentifiers LlGetUsedIdentifiersW
    #else
      #define LlGetUsedIdentifiersO LlGetUsedIdentifiersW
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSEDIDENTIFIERSW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszProjectName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSEDIDENTIFIERSW LlGetUsedIdentifiersW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLINTERNALATTACHAPP)(
	HLLJOB               hLlJob,
	HLLJOB               hLlJobToAttach);
#endif /* IMPLEMENTATION */

extern PFNLLINTERNALATTACHAPP LlInternalAttachApp;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLINTERNALDETACHAPP)(
	HLLJOB               hLlJob,
	HLLJOB               hLlJobToDetach);
#endif /* IMPLEMENTATION */

extern PFNLLINTERNALDETACHAPP LlInternalDetachApp;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUserSectionDataO LlGetUserSectionDataA
    #else
      #define LlGetUserSectionData LlGetUserSectionDataA
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSERSECTIONDATAA)(
	HLLJOB               hLlJob,
	LPCSTR               pszProjectName,
	PSCLLPROJECTUSERDATA pPUD);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSERSECTIONDATAA LlGetUserSectionDataA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUserSectionData LlGetUserSectionDataW
    #else
      #define LlGetUserSectionDataO LlGetUserSectionDataW
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSERSECTIONDATAW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszProjectName,
	PSCLLPROJECTUSERDATA pPUD);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSERSECTIONDATAW LlGetUserSectionDataW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLEXPRUPDATECOLLECTIONFORLLX)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRUPDATECOLLECTIONFORLLX LlExprUpdateCollectionForLlX;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprGetUsedVarsExO LlExprGetUsedVarsExA
    #else
      #define LlExprGetUsedVarsEx LlExprGetUsedVarsExA
  #endif
typedef      INT ( DLLPROC *PFNLLEXPRGETUSEDVARSEXA)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr,
	LPSTR                pszBuffer,
	UINT                 nBufSize,
	BOOL                 OrgName);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRGETUSEDVARSEXA LlExprGetUsedVarsExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlExprGetUsedVarsEx LlExprGetUsedVarsExW
    #else
      #define LlExprGetUsedVarsExO LlExprGetUsedVarsExW
  #endif
typedef      INT ( DLLPROC *PFNLLEXPRGETUSEDVARSEXW)(
	HLLJOB               hLlJob,
	HLLEXPR              lpExpr,
	LPWSTR               pszBuffer,
	UINT                 nBufSize,
	BOOL                 OrgName);
#endif /* IMPLEMENTATION */

extern PFNLLEXPRGETUSEDVARSEXW LlExprGetUsedVarsExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDOMGETPROJECT)(
	HLLJOB               hLlJob,
	PHLLDOMOBJ           phDOMObj);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETPROJECT LlDomGetProject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomGetPropertyO LlDomGetPropertyA
    #else
      #define LlDomGetProperty LlDomGetPropertyA
  #endif
typedef      INT ( DLLPROC *PFNLLDOMGETPROPERTYA)(
	HLLDOMOBJ            hDOMObj,
	LPCSTR               pszName,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETPROPERTYA LlDomGetPropertyA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomGetProperty LlDomGetPropertyW
    #else
      #define LlDomGetPropertyO LlDomGetPropertyW
  #endif
typedef      INT ( DLLPROC *PFNLLDOMGETPROPERTYW)(
	HLLDOMOBJ            hDOMObj,
	LPCWSTR              pszName,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETPROPERTYW LlDomGetPropertyW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomSetPropertyO LlDomSetPropertyA
    #else
      #define LlDomSetProperty LlDomSetPropertyA
  #endif
typedef      INT ( DLLPROC *PFNLLDOMSETPROPERTYA)(
	HLLDOMOBJ            hDOMObj,
	LPCSTR               pszName,
	LPCSTR               pszValue);
#endif /* IMPLEMENTATION */

extern PFNLLDOMSETPROPERTYA LlDomSetPropertyA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomSetProperty LlDomSetPropertyW
    #else
      #define LlDomSetPropertyO LlDomSetPropertyW
  #endif
typedef      INT ( DLLPROC *PFNLLDOMSETPROPERTYW)(
	HLLDOMOBJ            hDOMObj,
	LPCWSTR              pszName,
	LPCWSTR              pszValue);
#endif /* IMPLEMENTATION */

extern PFNLLDOMSETPROPERTYW LlDomSetPropertyW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomGetObjectO LlDomGetObjectA
    #else
      #define LlDomGetObject LlDomGetObjectA
  #endif
typedef      INT ( DLLPROC *PFNLLDOMGETOBJECTA)(
	HLLDOMOBJ            hDOMObj,
	LPCSTR               pszName,
	PHLLDOMOBJ           phDOMSubObj);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETOBJECTA LlDomGetObjectA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomGetObject LlDomGetObjectW
    #else
      #define LlDomGetObjectO LlDomGetObjectW
  #endif
typedef      INT ( DLLPROC *PFNLLDOMGETOBJECTW)(
	HLLDOMOBJ            hDOMObj,
	LPCWSTR              pszName,
	PHLLDOMOBJ           phDOMSubObj);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETOBJECTW LlDomGetObjectW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDOMGETSUBOBJECTCOUNT)(
	HLLDOMOBJ            hDOMObj,
	_LPINTJAVADUMMY      pnCount);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETSUBOBJECTCOUNT LlDomGetSubobjectCount;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDOMGETSUBOBJECT)(
	HLLDOMOBJ            hDOMObj,
	INT                  nPosition,
	PHLLDOMOBJ           phDOMSubObj);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETSUBOBJECT LlDomGetSubobject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomCreateSubobjectO LlDomCreateSubobjectA
    #else
      #define LlDomCreateSubobject LlDomCreateSubobjectA
  #endif
typedef      INT ( DLLPROC *PFNLLDOMCREATESUBOBJECTA)(
	HLLDOMOBJ            hDOMObj,
	INT                  nPosition,
	LPCSTR               pszType,
	PHLLDOMOBJ           phDOMSubObj);
#endif /* IMPLEMENTATION */

extern PFNLLDOMCREATESUBOBJECTA LlDomCreateSubobjectA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDomCreateSubobject LlDomCreateSubobjectW
    #else
      #define LlDomCreateSubobjectO LlDomCreateSubobjectW
  #endif
typedef      INT ( DLLPROC *PFNLLDOMCREATESUBOBJECTW)(
	HLLDOMOBJ            hDOMObj,
	INT                  nPosition,
	LPCWSTR              pszType,
	PHLLDOMOBJ           phDOMSubObj);
#endif /* IMPLEMENTATION */

extern PFNLLDOMCREATESUBOBJECTW LlDomCreateSubobjectW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDOMDELETESUBOBJECT)(
	HLLDOMOBJ            hDOMObj,
	INT                  nPosition);
#endif /* IMPLEMENTATION */

extern PFNLLDOMDELETESUBOBJECT LlDomDeleteSubobject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlProjectOpenO LlProjectOpenA
    #else
      #define LlProjectOpen LlProjectOpenA
  #endif
typedef      INT ( DLLPROC *PFNLLPROJECTOPENA)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCSTR               pszObjName,
	UINT                 nOpenMode);
#endif /* IMPLEMENTATION */

extern PFNLLPROJECTOPENA LlProjectOpenA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlProjectOpen LlProjectOpenW
    #else
      #define LlProjectOpenO LlProjectOpenW
  #endif
typedef      INT ( DLLPROC *PFNLLPROJECTOPENW)(
	HLLJOB               hLlJob,
	UINT                 nObjType,
	LPCWSTR              pszObjName,
	UINT                 nOpenMode);
#endif /* IMPLEMENTATION */

extern PFNLLPROJECTOPENW LlProjectOpenW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlProjectSaveO LlProjectSaveA
    #else
      #define LlProjectSave LlProjectSaveA
  #endif
typedef      INT ( DLLPROC *PFNLLPROJECTSAVEA)(
	HLLJOB               hLlJob,
	LPCSTR               pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLPROJECTSAVEA LlProjectSaveA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlProjectSave LlProjectSaveW
    #else
      #define LlProjectSaveO LlProjectSaveW
  #endif
typedef      INT ( DLLPROC *PFNLLPROJECTSAVEW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszObjName);
#endif /* IMPLEMENTATION */

extern PFNLLPROJECTSAVEW LlProjectSaveW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLPROJECTCLOSE)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLPROJECTCLOSE LlProjectClose;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDOMGETPROPERTYBSTR)(
	HLLDOMOBJ            hDOMObj,
	LPCWSTR              pszName,
	_PBSTR               pbsValue);
#endif /* IMPLEMENTATION */

extern PFNLLDOMGETPROPERTYBSTR LlDomGetPropertyBSTR;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLASSOCIATEPREVIEWCONTROL)(
	HLLJOB               hLlJob,
	HWND                 hWndControl,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLASSOCIATEPREVIEWCONTROL LlAssociatePreviewControl;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetErrortextO LlGetErrortextA
    #else
      #define LlGetErrortext LlGetErrortextA
  #endif
typedef      INT ( DLLPROC *PFNLLGETERRORTEXTA)(
	INT                  nError,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETERRORTEXTA LlGetErrortextA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetErrortext LlGetErrortextW
    #else
      #define LlGetErrortextO LlGetErrortextW
  #endif
typedef      INT ( DLLPROC *PFNLLGETERRORTEXTW)(
	INT                  nError,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETERRORTEXTW LlGetErrortextW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef  HRESULT ( DLLPROC *PFNLLXOBSTATJOBOPEN)(
	HLLJOB               hJob,
	PPILlXOBStatJob      pStatJob);
#endif /* IMPLEMENTATION */

extern PFNLLXOBSTATJOBOPEN LlXOBStatJobOpen;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSETPREVIEWOPTION)(
	HLLJOB               hLlJob,
	INT                  nOption,
	UINT                 nValue);
#endif /* IMPLEMENTATION */

extern PFNLLSETPREVIEWOPTION LlSetPreviewOption;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLGETPREVIEWOPTION)(
	HLLJOB               hLlJob,
	INT                  nOption,
	_LPUINT              pnValue);
#endif /* IMPLEMENTATION */

extern PFNLLGETPREVIEWOPTION LlGetPreviewOption;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDESIGNERINVOKEACTION)(
	HLLJOB               hLlJob,
	INT                  nMenuID);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERINVOKEACTION LlDesignerInvokeAction;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLDESIGNERREFRESHWORKSPACE)(
	HLLJOB               hLlJob);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERREFRESHWORKSPACE LlDesignerRefreshWorkspace;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerFileOpenO LlDesignerFileOpenA
    #else
      #define LlDesignerFileOpen LlDesignerFileOpenA
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERFILEOPENA)(
	HLLJOB               hLlJob,
	LPCSTR               pszFilename,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERFILEOPENA LlDesignerFileOpenA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerFileOpen LlDesignerFileOpenW
    #else
      #define LlDesignerFileOpenO LlDesignerFileOpenW
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERFILEOPENW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszFilename,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERFILEOPENW LlDesignerFileOpenW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerFileSaveO LlDesignerFileSaveA
    #else
      #define LlDesignerFileSave LlDesignerFileSaveA
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERFILESAVEA)(
	HLLJOB               hLlJob,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERFILESAVEA LlDesignerFileSaveA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerFileSave LlDesignerFileSaveW
    #else
      #define LlDesignerFileSaveO LlDesignerFileSaveW
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERFILESAVEW)(
	HLLJOB               hLlJob,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERFILESAVEW LlDesignerFileSaveW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerAddActionO LlDesignerAddActionA
    #else
      #define LlDesignerAddAction LlDesignerAddActionA
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERADDACTIONA)(
	HLLJOB               hLlJob,
	UINT                 nID,
	UINT                 nFlags,
	LPCSTR               pszMenuText,
	LPCSTR               pszMenuHierarchy,
	LPCSTR               pszTooltipText,
	UINT                 nIcon,
	LPVOID               pvReserved);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERADDACTIONA LlDesignerAddActionA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerAddAction LlDesignerAddActionW
    #else
      #define LlDesignerAddActionO LlDesignerAddActionW
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERADDACTIONW)(
	HLLJOB               hLlJob,
	UINT                 nID,
	UINT                 nFlags,
	LPCWSTR              pszMenuText,
	LPCWSTR              pszMenuHierarchy,
	LPCWSTR              pszTooltipText,
	UINT                 nIcon,
	LPVOID               pvReserved);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERADDACTIONW LlDesignerAddActionW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerGetOptionStringO LlDesignerGetOptionStringA
    #else
      #define LlDesignerGetOptionString LlDesignerGetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERGETOPTIONSTRINGA)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERGETOPTIONSTRINGA LlDesignerGetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerGetOptionString LlDesignerGetOptionStringW
    #else
      #define LlDesignerGetOptionStringO LlDesignerGetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERGETOPTIONSTRINGW)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERGETOPTIONSTRINGW LlDesignerGetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerSetOptionStringO LlDesignerSetOptionStringA
    #else
      #define LlDesignerSetOptionString LlDesignerSetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERSETOPTIONSTRINGA)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPCSTR               pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERSETOPTIONSTRINGA LlDesignerSetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDesignerSetOptionString LlDesignerSetOptionStringW
    #else
      #define LlDesignerSetOptionStringO LlDesignerSetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLDESIGNERSETOPTIONSTRINGW)(
	HLLJOB               hLlJob,
	INT                  nIndex,
	LPCWSTR              pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLDESIGNERSETOPTIONSTRINGW LlDesignerSetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef   HLLJOB ( DLLPROC *PFNLLJOBOPENCOPY)(
	HLLJOB               hJob);
#endif /* IMPLEMENTATION */

extern PFNLLJOBOPENCOPY LlJobOpenCopy;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetProjectParameterO LlGetProjectParameterA
    #else
      #define LlGetProjectParameter LlGetProjectParameterA
  #endif
typedef      INT ( DLLPROC *PFNLLGETPROJECTPARAMETERA)(
	HLLJOB               hLlJob,
	LPCSTR               pszProjectName,
	LPCSTR               pszParameter,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETPROJECTPARAMETERA LlGetProjectParameterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetProjectParameter LlGetProjectParameterW
    #else
      #define LlGetProjectParameterO LlGetProjectParameterW
  #endif
typedef      INT ( DLLPROC *PFNLLGETPROJECTPARAMETERW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszProjectName,
	LPCWSTR              pszParameter,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETPROJECTPARAMETERW LlGetProjectParameterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertBLOBToStringO LlConvertBLOBToStringA
    #else
      #define LlConvertBLOBToString LlConvertBLOBToStringA
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTBLOBTOSTRINGA)(
	_PCUINT8             pBytes,
	UINT                 nBytes,
	LPSTR                pszBuffer,
	UINT                 nBufSize,
	BOOL                 bWithCompression);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTBLOBTOSTRINGA LlConvertBLOBToStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertBLOBToString LlConvertBLOBToStringW
    #else
      #define LlConvertBLOBToStringO LlConvertBLOBToStringW
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTBLOBTOSTRINGW)(
	_PCUINT8             pBytes,
	UINT                 nBytes,
	LPWSTR               pszBuffer,
	UINT                 nBufSize,
	BOOL                 bWithCompression);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTBLOBTOSTRINGW LlConvertBLOBToStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStringToBLOBO LlConvertStringToBLOBA
    #else
      #define LlConvertStringToBLOB LlConvertStringToBLOBA
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTRINGTOBLOBA)(
	LPCSTR               pszText,
	_PUINT8              pBytes,
	UINT                 nBytes);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTRINGTOBLOBA LlConvertStringToBLOBA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStringToBLOB LlConvertStringToBLOBW
    #else
      #define LlConvertStringToBLOBO LlConvertStringToBLOBW
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTRINGTOBLOBW)(
	LPCWSTR              pszText,
	_PUINT8              pBytes,
	UINT                 nBytes);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTRINGTOBLOBW LlConvertStringToBLOBW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStreamToStringO LlConvertStreamToStringA
    #else
      #define LlConvertStreamToString LlConvertStreamToStringA
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTREAMTOSTRINGA)(
	_PISTREAM            pStream,
	LPSTR                pszBuffer,
	UINT                 nBufSize,
	BOOL                 bWithCompression);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTREAMTOSTRINGA LlConvertStreamToStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStreamToString LlConvertStreamToStringW
    #else
      #define LlConvertStreamToStringO LlConvertStreamToStringW
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTREAMTOSTRINGW)(
	_PISTREAM            pStream,
	LPWSTR               pszBuffer,
	UINT                 nBufSize,
	BOOL                 bWithCompression);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTREAMTOSTRINGW LlConvertStreamToStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStringToStreamO LlConvertStringToStreamA
    #else
      #define LlConvertStringToStream LlConvertStringToStreamA
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTRINGTOSTREAMA)(
	LPCSTR               pszText,
	_PISTREAM            pStream);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTRINGTOSTREAMA LlConvertStringToStreamA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStringToStream LlConvertStringToStreamW
    #else
      #define LlConvertStringToStreamO LlConvertStringToStreamW
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTRINGTOSTREAMW)(
	LPCWSTR              pszText,
	_PISTREAM            pStream);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTRINGTOSTREAMW LlConvertStringToStreamW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertHGLOBALToStringO LlConvertHGLOBALToStringA
    #else
      #define LlConvertHGLOBALToString LlConvertHGLOBALToStringA
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTHGLOBALTOSTRINGA)(
	HGLOBAL              hMemory,
	LPSTR                pszBuffer,
	UINT                 nBufSize,
	BOOL                 bWithCompression);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTHGLOBALTOSTRINGA LlConvertHGLOBALToStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertHGLOBALToString LlConvertHGLOBALToStringW
    #else
      #define LlConvertHGLOBALToStringO LlConvertHGLOBALToStringW
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTHGLOBALTOSTRINGW)(
	HGLOBAL              hMemory,
	LPWSTR               pszBuffer,
	UINT                 nBufSize,
	BOOL                 bWithCompression);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTHGLOBALTOSTRINGW LlConvertHGLOBALToStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStringToHGLOBALO LlConvertStringToHGLOBALA
    #else
      #define LlConvertStringToHGLOBAL LlConvertStringToHGLOBALA
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTRINGTOHGLOBALA)(
	LPCSTR               pszText,
	_PHGLOBAL            phMemory);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTRINGTOHGLOBALA LlConvertStringToHGLOBALA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlConvertStringToHGLOBAL LlConvertStringToHGLOBALW
    #else
      #define LlConvertStringToHGLOBALO LlConvertStringToHGLOBALW
  #endif
typedef      INT ( DLLPROC *PFNLLCONVERTSTRINGTOHGLOBALW)(
	LPCWSTR              pszText,
	_PHGLOBAL            phMemory);
#endif /* IMPLEMENTATION */

extern PFNLLCONVERTSTRINGTOHGLOBALW LlConvertStringToHGLOBALW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLAZTECENCODE)(
	LPVOID               in,
	LPVOID               out,
	INT                  fill);
#endif /* IMPLEMENTATION */

extern PFNLLAZTECENCODE LlAztecEncode;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableRelationExO LlDbAddTableRelationExA
    #else
      #define LlDbAddTableRelationEx LlDbAddTableRelationExA
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLERELATIONEXA)(
	HLLJOB               hJob,
	LPCSTR               pszTableID,
	LPCSTR               pszParentTableID,
	LPCSTR               pszRelationID,
	LPCSTR               pszRelationDisplayName,
	LPCSTR               pszKeyField,
	LPCSTR               pszParentKeyField);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLERELATIONEXA LlDbAddTableRelationExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableRelationEx LlDbAddTableRelationExW
    #else
      #define LlDbAddTableRelationExO LlDbAddTableRelationExW
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLERELATIONEXW)(
	HLLJOB               hJob,
	LPCWSTR              pszTableID,
	LPCWSTR              pszParentTableID,
	LPCWSTR              pszRelationID,
	LPCWSTR              pszRelationDisplayName,
	LPCWSTR              pszKeyField,
	LPCWSTR              pszParentKeyField);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLERELATIONEXW LlDbAddTableRelationExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableSortOrderExO LlDbAddTableSortOrderExA
    #else
      #define LlDbAddTableSortOrderEx LlDbAddTableSortOrderExA
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLESORTORDEREXA)(
	HLLJOB               hJob,
	LPCSTR               pszTableID,
	LPCSTR               pszSortOrderID,
	LPCSTR               pszSortOrderDisplayName,
	LPCSTR               pszField);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLESORTORDEREXA LlDbAddTableSortOrderExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlDbAddTableSortOrderEx LlDbAddTableSortOrderExW
    #else
      #define LlDbAddTableSortOrderExO LlDbAddTableSortOrderExW
  #endif
typedef      INT ( DLLPROC *PFNLLDBADDTABLESORTORDEREXW)(
	HLLJOB               hJob,
	LPCWSTR              pszTableID,
	LPCWSTR              pszSortOrderID,
	LPCWSTR              pszSortOrderDisplayName,
	LPCWSTR              pszField);
#endif /* IMPLEMENTATION */

extern PFNLLDBADDTABLESORTORDEREXW LlDbAddTableSortOrderExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUsedIdentifiersExO LlGetUsedIdentifiersExA
    #else
      #define LlGetUsedIdentifiersEx LlGetUsedIdentifiersExA
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSEDIDENTIFIERSEXA)(
	HLLJOB               hLlJob,
	LPCSTR               pszProjectName,
	UINT                 nIdentifierTypes,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSEDIDENTIFIERSEXA LlGetUsedIdentifiersExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetUsedIdentifiersEx LlGetUsedIdentifiersExW
    #else
      #define LlGetUsedIdentifiersExO LlGetUsedIdentifiersExW
  #endif
typedef      INT ( DLLPROC *PFNLLGETUSEDIDENTIFIERSEXW)(
	HLLJOB               hLlJob,
	LPCWSTR              pszProjectName,
	UINT                 nIdentifierTypes,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLGETUSEDIDENTIFIERSEXW LlGetUsedIdentifiersExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetTempFileNameO LlGetTempFileNameA
    #else
      #define LlGetTempFileName LlGetTempFileNameA
  #endif
typedef      INT ( DLLPROC *PFNLLGETTEMPFILENAMEA)(
	LPCSTR               pszPrefix,
	LPCSTR               pszExt,
	LPSTR                pszBuffer,
	UINT                 nBufSize,
	UINT                 nOptions);
#endif /* IMPLEMENTATION */

extern PFNLLGETTEMPFILENAMEA LlGetTempFileNameA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlGetTempFileName LlGetTempFileNameW
    #else
      #define LlGetTempFileNameO LlGetTempFileNameW
  #endif
typedef      INT ( DLLPROC *PFNLLGETTEMPFILENAMEW)(
	LPCWSTR              pszPrefix,
	LPCWSTR              pszExt,
	LPWSTR               pszBuffer,
	UINT                 nBufSize,
	UINT                 nOptions);
#endif /* IMPLEMENTATION */

extern PFNLLGETTEMPFILENAMEW LlGetTempFileNameW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     UINT ( DLLPROC *PFNLLGETDEBUG)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLLGETDEBUG LlGetDebug;

INT LL15xLoad(void);
INT LL15xLoadEx(LPCTSTR pszModuleName);
void LL15xUnload(void);
LPCTSTR LL15xModuleName(void);

#ifdef IMPLEMENTATION
  #if defined(__WATCOMC__)
    #define PROCIMPLL15(p) TEXT(#p)
    #define LOADDLLPROCLL15(fn,pfn,n) \
      fn = (pfn)GetProcAddress(g_hDLLLL15,PROCIMPLL15(fn));
    #define LOADDLLCPROCLL15(fn,pfn,n) \
      fn = (pfn)GetProcAddress(g_hDLLLL15,PROCIMPLL15(fn));
   #else
    #define PROCIMPLL15(p) #p
    #define LOADDLLPROCLL15(fn,pfn,n) \
      *(FARPROC*)&fn = GetProcAddress(g_hDLLLL15,PROCIMPLL15(fn));
    #define LOADDLLCPROCLL15(fn,pfn,n) \
      *(FARPROC*)&fn = GetProcAddress(g_hDLLLL15,PROCIMPLL15(fn));
  #endif

LPCTSTR LL15xModuleName(void)
{
  #if defined(_WIN64)
    return(_T("cxLL15.dll"));
  #else
    return(_T("cmLL15.dll"));
  #endif
}

INT LL15xLoad(void)
{
  return(LL15xLoadEx(LL15xModuleName()));
}

INT LL15xLoadEx(LPCTSTR pszModuleName)
{
  if (g_nDLLLL15Counter++ > 0)
    {
    return(0);
    }

  INT nError = SetErrorMode(SEM_NOOPENFILEERRORBOX);
  g_hDLLLL15 = LoadLibrary(pszModuleName);
  SetErrorMode(nError);

  if (g_hDLLLL15 == NULL)
    {
    g_nDLLLL15Counter = 0;
    return(-1);
    }

  LOADDLLPROCLL15(LlJobOpen,PFNLLJOBOPEN,4);
  LOADDLLPROCLL15(LlJobOpenLCID,PFNLLJOBOPENLCID,4);
  LOADDLLPROCLL15(LlJobClose,PFNLLJOBCLOSE,4);
  LOADDLLPROCLL15(LlSetDebug,PFNLLSETDEBUG,4);
  LOADDLLPROCLL15(LlGetVersion,PFNLLGETVERSION,4);
  LOADDLLPROCLL15(LlGetNotificationMessage,PFNLLGETNOTIFICATIONMESSAGE,4);
  LOADDLLPROCLL15(LlSetNotificationMessage,PFNLLSETNOTIFICATIONMESSAGE,8);
  LOADDLLPROCLL15(LlSetNotificationCallback,PFNLLSETNOTIFICATIONCALLBACK,8);
  LOADDLLPROCLL15(LlDefineFieldA,PFNLLDEFINEFIELDA,12);
  LOADDLLPROCLL15(LlDefineFieldW,PFNLLDEFINEFIELDW,12);
  LOADDLLPROCLL15(LlDefineFieldExtA,PFNLLDEFINEFIELDEXTA,20);
  LOADDLLPROCLL15(LlDefineFieldExtW,PFNLLDEFINEFIELDEXTW,20);
  LOADDLLPROCLL15(LlDefineFieldExtHandleA,PFNLLDEFINEFIELDEXTHANDLEA,20);
  LOADDLLPROCLL15(LlDefineFieldExtHandleW,PFNLLDEFINEFIELDEXTHANDLEW,20);
  LOADDLLPROCLL15(LlDefineFieldStart,PFNLLDEFINEFIELDSTART,4);
  LOADDLLPROCLL15(LlDefineVariableA,PFNLLDEFINEVARIABLEA,12);
  LOADDLLPROCLL15(LlDefineVariableW,PFNLLDEFINEVARIABLEW,12);
  LOADDLLPROCLL15(LlDefineVariableExtA,PFNLLDEFINEVARIABLEEXTA,20);
  LOADDLLPROCLL15(LlDefineVariableExtW,PFNLLDEFINEVARIABLEEXTW,20);
  LOADDLLPROCLL15(LlDefineVariableExtHandleA,PFNLLDEFINEVARIABLEEXTHANDLEA,20);
  LOADDLLPROCLL15(LlDefineVariableExtHandleW,PFNLLDEFINEVARIABLEEXTHANDLEW,20);
  LOADDLLPROCLL15(LlDefineVariableNameA,PFNLLDEFINEVARIABLENAMEA,8);
  LOADDLLPROCLL15(LlDefineVariableNameW,PFNLLDEFINEVARIABLENAMEW,8);
  LOADDLLPROCLL15(LlDefineVariableStart,PFNLLDEFINEVARIABLESTART,4);
  LOADDLLPROCLL15(LlDefineSumVariableA,PFNLLDEFINESUMVARIABLEA,12);
  LOADDLLPROCLL15(LlDefineSumVariableW,PFNLLDEFINESUMVARIABLEW,12);
  LOADDLLPROCLL15(LlDefineLayoutA,PFNLLDEFINELAYOUTA,20);
  LOADDLLPROCLL15(LlDefineLayoutW,PFNLLDEFINELAYOUTW,20);
  LOADDLLPROCLL15(LlDlgEditLineA,PFNLLDLGEDITLINEA,16);
  LOADDLLPROCLL15(LlDlgEditLineW,PFNLLDLGEDITLINEW,16);
  LOADDLLPROCLL15(LlDlgEditLineExA,PFNLLDLGEDITLINEEXA,32);
  LOADDLLPROCLL15(LlDlgEditLineExW,PFNLLDLGEDITLINEEXW,32);
  LOADDLLPROCLL15(LlPreviewSetTempPathA,PFNLLPREVIEWSETTEMPPATHA,8);
  LOADDLLPROCLL15(LlPreviewSetTempPathW,PFNLLPREVIEWSETTEMPPATHW,8);
  LOADDLLPROCLL15(LlPreviewDeleteFilesA,PFNLLPREVIEWDELETEFILESA,12);
  LOADDLLPROCLL15(LlPreviewDeleteFilesW,PFNLLPREVIEWDELETEFILESW,12);
  LOADDLLPROCLL15(LlPreviewDisplayA,PFNLLPREVIEWDISPLAYA,16);
  LOADDLLPROCLL15(LlPreviewDisplayW,PFNLLPREVIEWDISPLAYW,16);
  LOADDLLPROCLL15(LlPreviewDisplayExA,PFNLLPREVIEWDISPLAYEXA,24);
  LOADDLLPROCLL15(LlPreviewDisplayExW,PFNLLPREVIEWDISPLAYEXW,24);
  LOADDLLPROCLL15(LlPrint,PFNLLPRINT,4);
  LOADDLLPROCLL15(LlPrintAbort,PFNLLPRINTABORT,4);
  LOADDLLPROCLL15(LlPrintCheckLineFit,PFNLLPRINTCHECKLINEFIT,4);
  LOADDLLPROCLL15(LlPrintEnd,PFNLLPRINTEND,8);
  LOADDLLPROCLL15(LlPrintFields,PFNLLPRINTFIELDS,4);
  LOADDLLPROCLL15(LlPrintFieldsEnd,PFNLLPRINTFIELDSEND,4);
  LOADDLLPROCLL15(LlPrintGetCurrentPage,PFNLLPRINTGETCURRENTPAGE,4);
  LOADDLLPROCLL15(LlPrintGetItemsPerPage,PFNLLPRINTGETITEMSPERPAGE,4);
  LOADDLLPROCLL15(LlPrintGetItemsPerTable,PFNLLPRINTGETITEMSPERTABLE,4);
  LOADDLLPROCLL15(LlPrintGetRemainingItemsPerTableA,PFNLLPRINTGETREMAININGITEMSPERTABLEA,8);
  LOADDLLPROCLL15(LlPrintGetRemainingItemsPerTableW,PFNLLPRINTGETREMAININGITEMSPERTABLEW,8);
  LOADDLLPROCLL15(LlPrintGetRemItemsPerTableA,PFNLLPRINTGETREMITEMSPERTABLEA,8);
  LOADDLLPROCLL15(LlPrintGetRemItemsPerTableW,PFNLLPRINTGETREMITEMSPERTABLEW,8);
  LOADDLLPROCLL15(LlPrintGetOption,PFNLLPRINTGETOPTION,8);
  LOADDLLPROCLL15(LlPrintGetPrinterInfoA,PFNLLPRINTGETPRINTERINFOA,20);
  LOADDLLPROCLL15(LlPrintGetPrinterInfoW,PFNLLPRINTGETPRINTERINFOW,20);
  LOADDLLPROCLL15(LlPrintOptionsDialogA,PFNLLPRINTOPTIONSDIALOGA,12);
  LOADDLLPROCLL15(LlPrintOptionsDialogW,PFNLLPRINTOPTIONSDIALOGW,12);
  LOADDLLPROCLL15(LlPrintSelectOffsetEx,PFNLLPRINTSELECTOFFSETEX,8);
  LOADDLLPROCLL15(LlPrintSetBoxTextA,PFNLLPRINTSETBOXTEXTA,12);
  LOADDLLPROCLL15(LlPrintSetBoxTextW,PFNLLPRINTSETBOXTEXTW,12);
  LOADDLLPROCLL15(LlPrintSetOption,PFNLLPRINTSETOPTION,12);
  LOADDLLPROCLL15(LlPrintUpdateBox,PFNLLPRINTUPDATEBOX,4);
  LOADDLLPROCLL15(LlPrintStartA,PFNLLPRINTSTARTA,20);
  LOADDLLPROCLL15(LlPrintStartW,PFNLLPRINTSTARTW,20);
  LOADDLLPROCLL15(LlPrintWithBoxStartA,PFNLLPRINTWITHBOXSTARTA,28);
  LOADDLLPROCLL15(LlPrintWithBoxStartW,PFNLLPRINTWITHBOXSTARTW,28);
  LOADDLLPROCLL15(LlPrinterSetupA,PFNLLPRINTERSETUPA,16);
  LOADDLLPROCLL15(LlPrinterSetupW,PFNLLPRINTERSETUPW,16);
  LOADDLLPROCLL15(LlSelectFileDlgTitleExA,PFNLLSELECTFILEDLGTITLEEXA,28);
  LOADDLLPROCLL15(LlSelectFileDlgTitleExW,PFNLLSELECTFILEDLGTITLEEXW,28);
  LOADDLLPROCLL15(LlSetDlgboxMode,PFNLLSETDLGBOXMODE,4);
  LOADDLLPROCLL15(LlGetDlgboxMode,PFNLLGETDLGBOXMODE,0);
  LOADDLLPROCLL15(LlExprParseA,PFNLLEXPRPARSEA,12);
  LOADDLLPROCLL15(LlExprParseW,PFNLLEXPRPARSEW,12);
  LOADDLLPROCLL15(LlExprType,PFNLLEXPRTYPE,8);
  LOADDLLPROCLL15(LlExprErrorA,PFNLLEXPRERRORA,12);
  LOADDLLPROCLL15(LlExprErrorW,PFNLLEXPRERRORW,12);
  LOADDLLPROCLL15(LlExprFree,PFNLLEXPRFREE,8);
  LOADDLLPROCLL15(LlExprEvaluateA,PFNLLEXPREVALUATEA,16);
  LOADDLLPROCLL15(LlExprEvaluateW,PFNLLEXPREVALUATEW,16);
  LOADDLLPROCLL15(LlExprGetUsedVarsA,PFNLLEXPRGETUSEDVARSA,16);
  LOADDLLPROCLL15(LlExprGetUsedVarsW,PFNLLEXPRGETUSEDVARSW,16);
  LOADDLLPROCLL15(LlSetOption,PFNLLSETOPTION,12);
  LOADDLLPROCLL15(LlGetOption,PFNLLGETOPTION,8);
  LOADDLLPROCLL15(LlSetOptionStringA,PFNLLSETOPTIONSTRINGA,12);
  LOADDLLPROCLL15(LlSetOptionStringW,PFNLLSETOPTIONSTRINGW,12);
  LOADDLLPROCLL15(LlGetOptionStringA,PFNLLGETOPTIONSTRINGA,16);
  LOADDLLPROCLL15(LlGetOptionStringW,PFNLLGETOPTIONSTRINGW,16);
  LOADDLLPROCLL15(LlPrintSetOptionStringA,PFNLLPRINTSETOPTIONSTRINGA,12);
  LOADDLLPROCLL15(LlPrintSetOptionStringW,PFNLLPRINTSETOPTIONSTRINGW,12);
  LOADDLLPROCLL15(LlPrintGetOptionStringA,PFNLLPRINTGETOPTIONSTRINGA,16);
  LOADDLLPROCLL15(LlPrintGetOptionStringW,PFNLLPRINTGETOPTIONSTRINGW,16);
  LOADDLLPROCLL15(LlDesignerProhibitAction,PFNLLDESIGNERPROHIBITACTION,8);
  LOADDLLPROCLL15(LlDesignerProhibitFunctionA,PFNLLDESIGNERPROHIBITFUNCTIONA,8);
  LOADDLLPROCLL15(LlDesignerProhibitFunctionW,PFNLLDESIGNERPROHIBITFUNCTIONW,8);
  LOADDLLPROCLL15(LlPrintEnableObjectA,PFNLLPRINTENABLEOBJECTA,12);
  LOADDLLPROCLL15(LlPrintEnableObjectW,PFNLLPRINTENABLEOBJECTW,12);
  LOADDLLPROCLL15(LlSetFileExtensionsA,PFNLLSETFILEEXTENSIONSA,20);
  LOADDLLPROCLL15(LlSetFileExtensionsW,PFNLLSETFILEEXTENSIONSW,20);
  LOADDLLPROCLL15(LlPrintGetTextCharsPrintedA,PFNLLPRINTGETTEXTCHARSPRINTEDA,8);
  LOADDLLPROCLL15(LlPrintGetTextCharsPrintedW,PFNLLPRINTGETTEXTCHARSPRINTEDW,8);
  LOADDLLPROCLL15(LlPrintGetFieldCharsPrintedA,PFNLLPRINTGETFIELDCHARSPRINTEDA,12);
  LOADDLLPROCLL15(LlPrintGetFieldCharsPrintedW,PFNLLPRINTGETFIELDCHARSPRINTEDW,12);
  LOADDLLPROCLL15(LlPrintIsVariableUsedA,PFNLLPRINTISVARIABLEUSEDA,8);
  LOADDLLPROCLL15(LlPrintIsVariableUsedW,PFNLLPRINTISVARIABLEUSEDW,8);
  LOADDLLPROCLL15(LlPrintIsFieldUsedA,PFNLLPRINTISFIELDUSEDA,8);
  LOADDLLPROCLL15(LlPrintIsFieldUsedW,PFNLLPRINTISFIELDUSEDW,8);
  LOADDLLPROCLL15(LlPrintOptionsDialogTitleA,PFNLLPRINTOPTIONSDIALOGTITLEA,16);
  LOADDLLPROCLL15(LlPrintOptionsDialogTitleW,PFNLLPRINTOPTIONSDIALOGTITLEW,16);
  LOADDLLPROCLL15(LlSetPrinterToDefaultA,PFNLLSETPRINTERTODEFAULTA,12);
  LOADDLLPROCLL15(LlSetPrinterToDefaultW,PFNLLSETPRINTERTODEFAULTW,12);
  LOADDLLPROCLL15(LlDefineSortOrderStart,PFNLLDEFINESORTORDERSTART,4);
  LOADDLLPROCLL15(LlDefineSortOrderA,PFNLLDEFINESORTORDERA,12);
  LOADDLLPROCLL15(LlDefineSortOrderW,PFNLLDEFINESORTORDERW,12);
  LOADDLLPROCLL15(LlPrintGetSortOrderA,PFNLLPRINTGETSORTORDERA,12);
  LOADDLLPROCLL15(LlPrintGetSortOrderW,PFNLLPRINTGETSORTORDERW,12);
  LOADDLLPROCLL15(LlDefineGroupingA,PFNLLDEFINEGROUPINGA,16);
  LOADDLLPROCLL15(LlDefineGroupingW,PFNLLDEFINEGROUPINGW,16);
  LOADDLLPROCLL15(LlPrintGetGroupingA,PFNLLPRINTGETGROUPINGA,12);
  LOADDLLPROCLL15(LlPrintGetGroupingW,PFNLLPRINTGETGROUPINGW,12);
  LOADDLLPROCLL15(LlAddCtlSupportA,PFNLLADDCTLSUPPORTA,12);
  LOADDLLPROCLL15(LlAddCtlSupportW,PFNLLADDCTLSUPPORTW,12);
  LOADDLLPROCLL15(LlPrintBeginGroup,PFNLLPRINTBEGINGROUP,12);
  LOADDLLPROCLL15(LlPrintEndGroup,PFNLLPRINTENDGROUP,12);
  LOADDLLPROCLL15(LlPrintGroupLine,PFNLLPRINTGROUPLINE,12);
  LOADDLLPROCLL15(LlPrintGroupHeader,PFNLLPRINTGROUPHEADER,8);
  LOADDLLPROCLL15(LlPrintGetFilterExpressionA,PFNLLPRINTGETFILTEREXPRESSIONA,12);
  LOADDLLPROCLL15(LlPrintGetFilterExpressionW,PFNLLPRINTGETFILTEREXPRESSIONW,12);
  LOADDLLPROCLL15(LlPrintWillMatchFilter,PFNLLPRINTWILLMATCHFILTER,4);
  LOADDLLPROCLL15(LlPrintDidMatchFilter,PFNLLPRINTDIDMATCHFILTER,4);
  LOADDLLPROCLL15(LlGetFieldContentsA,PFNLLGETFIELDCONTENTSA,16);
  LOADDLLPROCLL15(LlGetFieldContentsW,PFNLLGETFIELDCONTENTSW,16);
  LOADDLLPROCLL15(LlGetVariableContentsA,PFNLLGETVARIABLECONTENTSA,16);
  LOADDLLPROCLL15(LlGetVariableContentsW,PFNLLGETVARIABLECONTENTSW,16);
  LOADDLLPROCLL15(LlGetSumVariableContentsA,PFNLLGETSUMVARIABLECONTENTSA,16);
  LOADDLLPROCLL15(LlGetSumVariableContentsW,PFNLLGETSUMVARIABLECONTENTSW,16);
  LOADDLLPROCLL15(LlGetUserVariableContentsA,PFNLLGETUSERVARIABLECONTENTSA,16);
  LOADDLLPROCLL15(LlGetUserVariableContentsW,PFNLLGETUSERVARIABLECONTENTSW,16);
  LOADDLLPROCLL15(LlGetVariableTypeA,PFNLLGETVARIABLETYPEA,8);
  LOADDLLPROCLL15(LlGetVariableTypeW,PFNLLGETVARIABLETYPEW,8);
  LOADDLLPROCLL15(LlGetFieldTypeA,PFNLLGETFIELDTYPEA,8);
  LOADDLLPROCLL15(LlGetFieldTypeW,PFNLLGETFIELDTYPEW,8);
  LOADDLLPROCLL15(LlPrintGetColumnInfoA,PFNLLPRINTGETCOLUMNINFOA,16);
  LOADDLLPROCLL15(LlPrintGetColumnInfoW,PFNLLPRINTGETCOLUMNINFOW,16);
  LOADDLLPROCLL15(LlSetPrinterDefaultsDirA,PFNLLSETPRINTERDEFAULTSDIRA,8);
  LOADDLLPROCLL15(LlSetPrinterDefaultsDirW,PFNLLSETPRINTERDEFAULTSDIRW,8);
  LOADDLLPROCLL15(LlCreateSketchA,PFNLLCREATESKETCHA,12);
  LOADDLLPROCLL15(LlCreateSketchW,PFNLLCREATESKETCHW,12);
  LOADDLLPROCLL15(LlViewerProhibitAction,PFNLLVIEWERPROHIBITACTION,8);
  LOADDLLPROCLL15(LlPrintCopyPrinterConfigurationA,PFNLLPRINTCOPYPRINTERCONFIGURATIONA,12);
  LOADDLLPROCLL15(LlPrintCopyPrinterConfigurationW,PFNLLPRINTCOPYPRINTERCONFIGURATIONW,12);
  LOADDLLPROCLL15(LlSetPrinterInPrinterFileA,PFNLLSETPRINTERINPRINTERFILEA,24);
  LOADDLLPROCLL15(LlSetPrinterInPrinterFileW,PFNLLSETPRINTERINPRINTERFILEW,24);
  LOADDLLPROCLL15(LlRTFCreateObject,PFNLLRTFCREATEOBJECT,4);
  LOADDLLPROCLL15(LlRTFDeleteObject,PFNLLRTFDELETEOBJECT,8);
  LOADDLLPROCLL15(LlRTFSetTextA,PFNLLRTFSETTEXTA,12);
  LOADDLLPROCLL15(LlRTFSetTextW,PFNLLRTFSETTEXTW,12);
  LOADDLLPROCLL15(LlRTFGetTextLength,PFNLLRTFGETTEXTLENGTH,12);
  LOADDLLPROCLL15(LlRTFGetTextA,PFNLLRTFGETTEXTA,20);
  LOADDLLPROCLL15(LlRTFGetTextW,PFNLLRTFGETTEXTW,20);
  LOADDLLPROCLL15(LlRTFEditObject,PFNLLRTFEDITOBJECT,24);
  LOADDLLPROCLL15(LlRTFCopyToClipboard,PFNLLRTFCOPYTOCLIPBOARD,8);
  LOADDLLPROCLL15(LlRTFDisplay,PFNLLRTFDISPLAY,24);
  LOADDLLPROCLL15(LlRTFEditorProhibitAction,PFNLLRTFEDITORPROHIBITACTION,12);
  LOADDLLPROCLL15(LlRTFEditorInvokeAction,PFNLLRTFEDITORINVOKEACTION,12);
  LOADDLLPROCLL15(LlDebugOutputA,PFNLLDEBUGOUTPUTA,8);
  LOADDLLPROCLL15(LlDebugOutputW,PFNLLDEBUGOUTPUTW,8);
  LOADDLLPROCLL15(LlEnumGetFirstVar,PFNLLENUMGETFIRSTVAR,8);
  LOADDLLPROCLL15(LlEnumGetFirstField,PFNLLENUMGETFIRSTFIELD,8);
  LOADDLLPROCLL15(LlEnumGetNextEntry,PFNLLENUMGETNEXTENTRY,12);
  LOADDLLPROCLL15(LlEnumGetEntryA,PFNLLENUMGETENTRYA,32);
  LOADDLLPROCLL15(LlEnumGetEntryW,PFNLLENUMGETENTRYW,32);
  LOADDLLPROCLL15(LlPrintResetObjectStates,PFNLLPRINTRESETOBJECTSTATES,4);
  LOADDLLPROCLL15(LlXSetParameterA,PFNLLXSETPARAMETERA,20);
  LOADDLLPROCLL15(LlXSetParameterW,PFNLLXSETPARAMETERW,20);
  LOADDLLPROCLL15(LlXGetParameterA,PFNLLXGETPARAMETERA,24);
  LOADDLLPROCLL15(LlXGetParameterW,PFNLLXGETPARAMETERW,24);
  LOADDLLPROCLL15(LlPrintResetProjectState,PFNLLPRINTRESETPROJECTSTATE,4);
  LOADDLLPROCLL15(LlDefineChartFieldStart,PFNLLDEFINECHARTFIELDSTART,4);
  LOADDLLPROCLL15(LlDefineChartFieldExtA,PFNLLDEFINECHARTFIELDEXTA,20);
  LOADDLLPROCLL15(LlDefineChartFieldExtW,PFNLLDEFINECHARTFIELDEXTW,20);
  LOADDLLPROCLL15(LlPrintDeclareChartRow,PFNLLPRINTDECLARECHARTROW,8);
  LOADDLLPROCLL15(LlPrintGetChartObjectCount,PFNLLPRINTGETCHARTOBJECTCOUNT,8);
  LOADDLLPROCLL15(LlPrintIsChartFieldUsedA,PFNLLPRINTISCHARTFIELDUSEDA,8);
  LOADDLLPROCLL15(LlPrintIsChartFieldUsedW,PFNLLPRINTISCHARTFIELDUSEDW,8);
  LOADDLLPROCLL15(LlGetChartFieldContentsA,PFNLLGETCHARTFIELDCONTENTSA,16);
  LOADDLLPROCLL15(LlGetChartFieldContentsW,PFNLLGETCHARTFIELDCONTENTSW,16);
  LOADDLLPROCLL15(LlEnumGetFirstChartField,PFNLLENUMGETFIRSTCHARTFIELD,8);
  LOADDLLPROCLL15(LlSetNotificationCallbackExt,PFNLLSETNOTIFICATIONCALLBACKEXT,12);
  LOADDLLPROCLL15(LlExprEvaluateVar,PFNLLEXPREVALUATEVAR,16);
  LOADDLLPROCLL15(LlExprTypeVar,PFNLLEXPRTYPEVAR,8);
  LOADDLLPROCLL15(LlGetPrinterFromPrinterFileA,PFNLLGETPRINTERFROMPRINTERFILEA,32);
  LOADDLLPROCLL15(LlGetPrinterFromPrinterFileW,PFNLLGETPRINTERFROMPRINTERFILEW,32);
  LOADDLLPROCLL15(LlPrintGetRemainingSpacePerTableA,PFNLLPRINTGETREMAININGSPACEPERTABLEA,12);
  LOADDLLPROCLL15(LlPrintGetRemainingSpacePerTableW,PFNLLPRINTGETREMAININGSPACEPERTABLEW,12);
  LOADDLLPROCLL15(LlDrawToolbarBackground,PFNLLDRAWTOOLBARBACKGROUND,16);
  LOADDLLPROCLL15(LlSetDefaultProjectParameterA,PFNLLSETDEFAULTPROJECTPARAMETERA,16);
  LOADDLLPROCLL15(LlSetDefaultProjectParameterW,PFNLLSETDEFAULTPROJECTPARAMETERW,16);
  LOADDLLPROCLL15(LlGetDefaultProjectParameterA,PFNLLGETDEFAULTPROJECTPARAMETERA,20);
  LOADDLLPROCLL15(LlGetDefaultProjectParameterW,PFNLLGETDEFAULTPROJECTPARAMETERW,20);
  LOADDLLPROCLL15(LlPrintSetProjectParameterA,PFNLLPRINTSETPROJECTPARAMETERA,16);
  LOADDLLPROCLL15(LlPrintSetProjectParameterW,PFNLLPRINTSETPROJECTPARAMETERW,16);
  LOADDLLPROCLL15(LlPrintGetProjectParameterA,PFNLLPRINTGETPROJECTPARAMETERA,24);
  LOADDLLPROCLL15(LlPrintGetProjectParameterW,PFNLLPRINTGETPROJECTPARAMETERW,24);
  LOADDLLPROCLL15(LlCreateObject,PFNLLCREATEOBJECT,8);
  LOADDLLPROCLL15(LlExprContainsVariableA,PFNLLEXPRCONTAINSVARIABLEA,12);
  LOADDLLPROCLL15(LlExprContainsVariableW,PFNLLEXPRCONTAINSVARIABLEW,12);
  LOADDLLPROCLL15(LlExprIsConstant,PFNLLEXPRISCONSTANT,8);
  LOADDLLPROCLL15(LlLocSystemTimeFromLocaleStringA,PFNLLLOCSYSTEMTIMEFROMLOCALESTRINGA,16);
  LOADDLLPROCLL15(LlLocSystemTimeFromLocaleStringW,PFNLLLOCSYSTEMTIMEFROMLOCALESTRINGW,16);
  LOADDLLPROCLL15(LlLocSystemTimeToLocaleStringA,PFNLLLOCSYSTEMTIMETOLOCALESTRINGA,16);
  LOADDLLPROCLL15(LlLocSystemTimeToLocaleStringW,PFNLLLOCSYSTEMTIMETOLOCALESTRINGW,16);
  LOADDLLPROCLL15(LlLocConvertCountryToA,PFNLLLOCCONVERTCOUNTRYTOA,20);
  LOADDLLPROCLL15(LlLocConvertCountryToW,PFNLLLOCCONVERTCOUNTRYTOW,20);
  LOADDLLPROCLL15(LlProfileStartA,PFNLLPROFILESTARTA,16);
  LOADDLLPROCLL15(LlProfileStartW,PFNLLPROFILESTARTW,16);
  LOADDLLPROCLL15(LlProfileEnd,PFNLLPROFILEEND,4);
  LOADDLLPROCLL15(LlDumpMemory,PFNLLDUMPMEMORY,4);
  LOADDLLPROCLL15(LlDbAddTableA,PFNLLDBADDTABLEA,12);
  LOADDLLPROCLL15(LlDbAddTableW,PFNLLDBADDTABLEW,12);
  LOADDLLPROCLL15(LlDbAddTableRelationA,PFNLLDBADDTABLERELATIONA,20);
  LOADDLLPROCLL15(LlDbAddTableRelationW,PFNLLDBADDTABLERELATIONW,20);
  LOADDLLPROCLL15(LlDbAddTableSortOrderA,PFNLLDBADDTABLESORTORDERA,16);
  LOADDLLPROCLL15(LlDbAddTableSortOrderW,PFNLLDBADDTABLESORTORDERW,16);
  LOADDLLPROCLL15(LlPrintDbGetCurrentTableA,PFNLLPRINTDBGETCURRENTTABLEA,16);
  LOADDLLPROCLL15(LlPrintDbGetCurrentTableW,PFNLLPRINTDBGETCURRENTTABLEW,16);
  LOADDLLPROCLL15(LlPrintDbGetCurrentTableRelationA,PFNLLPRINTDBGETCURRENTTABLERELATIONA,12);
  LOADDLLPROCLL15(LlPrintDbGetCurrentTableRelationW,PFNLLPRINTDBGETCURRENTTABLERELATIONW,12);
  LOADDLLPROCLL15(LlPrintDbGetCurrentTableSortOrderA,PFNLLPRINTDBGETCURRENTTABLESORTORDERA,12);
  LOADDLLPROCLL15(LlPrintDbGetCurrentTableSortOrderW,PFNLLPRINTDBGETCURRENTTABLESORTORDERW,12);
  LOADDLLPROCLL15(LlDbDumpStructure,PFNLLDBDUMPSTRUCTURE,4);
  LOADDLLPROCLL15(LlPrintDbGetRootTableCount,PFNLLPRINTDBGETROOTTABLECOUNT,4);
  LOADDLLPROCLL15(LlDbSetMasterTableA,PFNLLDBSETMASTERTABLEA,8);
  LOADDLLPROCLL15(LlDbSetMasterTableW,PFNLLDBSETMASTERTABLEW,8);
  LOADDLLPROCLL15(LlDbGetMasterTableA,PFNLLDBGETMASTERTABLEA,12);
  LOADDLLPROCLL15(LlDbGetMasterTableW,PFNLLDBGETMASTERTABLEW,12);
  LOADDLLPROCLL15(LlXSetExportParameterA,PFNLLXSETEXPORTPARAMETERA,16);
  LOADDLLPROCLL15(LlXSetExportParameterW,PFNLLXSETEXPORTPARAMETERW,16);
  LOADDLLPROCLL15(LlXGetExportParameterA,PFNLLXGETEXPORTPARAMETERA,20);
  LOADDLLPROCLL15(LlXGetExportParameterW,PFNLLXGETEXPORTPARAMETERW,20);
  LOADDLLPROCLL15(LlXlatNameA,PFNLLXLATNAMEA,16);
  LOADDLLPROCLL15(LlXlatNameW,PFNLLXLATNAMEW,16);
  LOADDLLPROCLL15(LlDefineVariableVarA,PFNLLDEFINEVARIABLEVARA,20);
  LOADDLLPROCLL15(LlDefineVariableVarW,PFNLLDEFINEVARIABLEVARW,20);
  LOADDLLPROCLL15(LlDefineFieldVarA,PFNLLDEFINEFIELDVARA,20);
  LOADDLLPROCLL15(LlDefineFieldVarW,PFNLLDEFINEFIELDVARW,20);
  LOADDLLPROCLL15(LlDefineChartFieldVarA,PFNLLDEFINECHARTFIELDVARA,20);
  LOADDLLPROCLL15(LlDefineChartFieldVarW,PFNLLDEFINECHARTFIELDVARW,20);
  LOADDLLPROCLL15(LlDesignerProhibitEditingObjectA,PFNLLDESIGNERPROHIBITEDITINGOBJECTA,8);
  LOADDLLPROCLL15(LlDesignerProhibitEditingObjectW,PFNLLDESIGNERPROHIBITEDITINGOBJECTW,8);
  LOADDLLPROCLL15(LlGetUsedIdentifiersA,PFNLLGETUSEDIDENTIFIERSA,16);
  LOADDLLPROCLL15(LlGetUsedIdentifiersW,PFNLLGETUSEDIDENTIFIERSW,16);
  LOADDLLPROCLL15(LlInternalAttachApp,PFNLLINTERNALATTACHAPP,8);
  LOADDLLPROCLL15(LlInternalDetachApp,PFNLLINTERNALDETACHAPP,8);
  LOADDLLPROCLL15(LlGetUserSectionDataA,PFNLLGETUSERSECTIONDATAA,12);
  LOADDLLPROCLL15(LlGetUserSectionDataW,PFNLLGETUSERSECTIONDATAW,12);
  LOADDLLPROCLL15(LlExprUpdateCollectionForLlX,PFNLLEXPRUPDATECOLLECTIONFORLLX,8);
  LOADDLLPROCLL15(LlExprGetUsedVarsExA,PFNLLEXPRGETUSEDVARSEXA,20);
  LOADDLLPROCLL15(LlExprGetUsedVarsExW,PFNLLEXPRGETUSEDVARSEXW,20);
  LOADDLLPROCLL15(LlDomGetProject,PFNLLDOMGETPROJECT,8);
  LOADDLLPROCLL15(LlDomGetPropertyA,PFNLLDOMGETPROPERTYA,16);
  LOADDLLPROCLL15(LlDomGetPropertyW,PFNLLDOMGETPROPERTYW,16);
  LOADDLLPROCLL15(LlDomSetPropertyA,PFNLLDOMSETPROPERTYA,12);
  LOADDLLPROCLL15(LlDomSetPropertyW,PFNLLDOMSETPROPERTYW,12);
  LOADDLLPROCLL15(LlDomGetObjectA,PFNLLDOMGETOBJECTA,12);
  LOADDLLPROCLL15(LlDomGetObjectW,PFNLLDOMGETOBJECTW,12);
  LOADDLLPROCLL15(LlDomGetSubobjectCount,PFNLLDOMGETSUBOBJECTCOUNT,8);
  LOADDLLPROCLL15(LlDomGetSubobject,PFNLLDOMGETSUBOBJECT,12);
  LOADDLLPROCLL15(LlDomCreateSubobjectA,PFNLLDOMCREATESUBOBJECTA,16);
  LOADDLLPROCLL15(LlDomCreateSubobjectW,PFNLLDOMCREATESUBOBJECTW,16);
  LOADDLLPROCLL15(LlDomDeleteSubobject,PFNLLDOMDELETESUBOBJECT,8);
  LOADDLLPROCLL15(LlProjectOpenA,PFNLLPROJECTOPENA,16);
  LOADDLLPROCLL15(LlProjectOpenW,PFNLLPROJECTOPENW,16);
  LOADDLLPROCLL15(LlProjectSaveA,PFNLLPROJECTSAVEA,8);
  LOADDLLPROCLL15(LlProjectSaveW,PFNLLPROJECTSAVEW,8);
  LOADDLLPROCLL15(LlProjectClose,PFNLLPROJECTCLOSE,4);
  LOADDLLPROCLL15(LlDomGetPropertyBSTR,PFNLLDOMGETPROPERTYBSTR,12);
  LOADDLLPROCLL15(LlAssociatePreviewControl,PFNLLASSOCIATEPREVIEWCONTROL,12);
  LOADDLLPROCLL15(LlGetErrortextA,PFNLLGETERRORTEXTA,12);
  LOADDLLPROCLL15(LlGetErrortextW,PFNLLGETERRORTEXTW,12);
  LOADDLLPROCLL15(LlXOBStatJobOpen,PFNLLXOBSTATJOBOPEN,8);
  LOADDLLPROCLL15(LlSetPreviewOption,PFNLLSETPREVIEWOPTION,12);
  LOADDLLPROCLL15(LlGetPreviewOption,PFNLLGETPREVIEWOPTION,12);
  LOADDLLPROCLL15(LlDesignerInvokeAction,PFNLLDESIGNERINVOKEACTION,8);
  LOADDLLPROCLL15(LlDesignerRefreshWorkspace,PFNLLDESIGNERREFRESHWORKSPACE,4);
  LOADDLLPROCLL15(LlDesignerFileOpenA,PFNLLDESIGNERFILEOPENA,12);
  LOADDLLPROCLL15(LlDesignerFileOpenW,PFNLLDESIGNERFILEOPENW,12);
  LOADDLLPROCLL15(LlDesignerFileSaveA,PFNLLDESIGNERFILESAVEA,8);
  LOADDLLPROCLL15(LlDesignerFileSaveW,PFNLLDESIGNERFILESAVEW,8);
  LOADDLLPROCLL15(LlDesignerAddActionA,PFNLLDESIGNERADDACTIONA,32);
  LOADDLLPROCLL15(LlDesignerAddActionW,PFNLLDESIGNERADDACTIONW,32);
  LOADDLLPROCLL15(LlDesignerGetOptionStringA,PFNLLDESIGNERGETOPTIONSTRINGA,16);
  LOADDLLPROCLL15(LlDesignerGetOptionStringW,PFNLLDESIGNERGETOPTIONSTRINGW,16);
  LOADDLLPROCLL15(LlDesignerSetOptionStringA,PFNLLDESIGNERSETOPTIONSTRINGA,12);
  LOADDLLPROCLL15(LlDesignerSetOptionStringW,PFNLLDESIGNERSETOPTIONSTRINGW,12);
  LOADDLLPROCLL15(LlJobOpenCopy,PFNLLJOBOPENCOPY,4);
  LOADDLLPROCLL15(LlGetProjectParameterA,PFNLLGETPROJECTPARAMETERA,20);
  LOADDLLPROCLL15(LlGetProjectParameterW,PFNLLGETPROJECTPARAMETERW,20);
  LOADDLLPROCLL15(LlConvertBLOBToStringA,PFNLLCONVERTBLOBTOSTRINGA,20);
  LOADDLLPROCLL15(LlConvertBLOBToStringW,PFNLLCONVERTBLOBTOSTRINGW,20);
  LOADDLLPROCLL15(LlConvertStringToBLOBA,PFNLLCONVERTSTRINGTOBLOBA,12);
  LOADDLLPROCLL15(LlConvertStringToBLOBW,PFNLLCONVERTSTRINGTOBLOBW,12);
  LOADDLLPROCLL15(LlConvertStreamToStringA,PFNLLCONVERTSTREAMTOSTRINGA,16);
  LOADDLLPROCLL15(LlConvertStreamToStringW,PFNLLCONVERTSTREAMTOSTRINGW,16);
  LOADDLLPROCLL15(LlConvertStringToStreamA,PFNLLCONVERTSTRINGTOSTREAMA,8);
  LOADDLLPROCLL15(LlConvertStringToStreamW,PFNLLCONVERTSTRINGTOSTREAMW,8);
  LOADDLLPROCLL15(LlConvertHGLOBALToStringA,PFNLLCONVERTHGLOBALTOSTRINGA,16);
  LOADDLLPROCLL15(LlConvertHGLOBALToStringW,PFNLLCONVERTHGLOBALTOSTRINGW,16);
  LOADDLLPROCLL15(LlConvertStringToHGLOBALA,PFNLLCONVERTSTRINGTOHGLOBALA,8);
  LOADDLLPROCLL15(LlConvertStringToHGLOBALW,PFNLLCONVERTSTRINGTOHGLOBALW,8);
  LOADDLLPROCLL15(LlAztecEncode,PFNLLAZTECENCODE,12);
  LOADDLLPROCLL15(LlDbAddTableRelationExA,PFNLLDBADDTABLERELATIONEXA,28);
  LOADDLLPROCLL15(LlDbAddTableRelationExW,PFNLLDBADDTABLERELATIONEXW,28);
  LOADDLLPROCLL15(LlDbAddTableSortOrderExA,PFNLLDBADDTABLESORTORDEREXA,20);
  LOADDLLPROCLL15(LlDbAddTableSortOrderExW,PFNLLDBADDTABLESORTORDEREXW,20);
  LOADDLLPROCLL15(LlGetUsedIdentifiersExA,PFNLLGETUSEDIDENTIFIERSEXA,20);
  LOADDLLPROCLL15(LlGetUsedIdentifiersExW,PFNLLGETUSEDIDENTIFIERSEXW,20);
  LOADDLLPROCLL15(LlGetTempFileNameA,PFNLLGETTEMPFILENAMEA,20);
  LOADDLLPROCLL15(LlGetTempFileNameW,PFNLLGETTEMPFILENAMEW,20);
  LOADDLLPROCLL15(LlGetDebug,PFNLLGETDEBUG,0);
  return(0);
}

void LL15xUnload(void)
{
  if (--g_nDLLLL15Counter > 0)
    return;

  if (g_hDLLLL15 != NULL)
    {
    FreeLibrary(g_hDLLLL15);
    g_hDLLLL15 = NULL;
    LlJobOpen = NULL;
    LlJobOpenLCID = NULL;
    LlJobClose = NULL;
    LlSetDebug = NULL;
    LlGetVersion = NULL;
    LlGetNotificationMessage = NULL;
    LlSetNotificationMessage = NULL;
    LlSetNotificationCallback = NULL;
    LlDefineFieldA = NULL;
    LlDefineFieldW = NULL;
    LlDefineFieldExtA = NULL;
    LlDefineFieldExtW = NULL;
    LlDefineFieldExtHandleA = NULL;
    LlDefineFieldExtHandleW = NULL;
    LlDefineFieldStart = NULL;
    LlDefineVariableA = NULL;
    LlDefineVariableW = NULL;
    LlDefineVariableExtA = NULL;
    LlDefineVariableExtW = NULL;
    LlDefineVariableExtHandleA = NULL;
    LlDefineVariableExtHandleW = NULL;
    LlDefineVariableNameA = NULL;
    LlDefineVariableNameW = NULL;
    LlDefineVariableStart = NULL;
    LlDefineSumVariableA = NULL;
    LlDefineSumVariableW = NULL;
    LlDefineLayoutA = NULL;
    LlDefineLayoutW = NULL;
    LlDlgEditLineA = NULL;
    LlDlgEditLineW = NULL;
    LlDlgEditLineExA = NULL;
    LlDlgEditLineExW = NULL;
    LlPreviewSetTempPathA = NULL;
    LlPreviewSetTempPathW = NULL;
    LlPreviewDeleteFilesA = NULL;
    LlPreviewDeleteFilesW = NULL;
    LlPreviewDisplayA = NULL;
    LlPreviewDisplayW = NULL;
    LlPreviewDisplayExA = NULL;
    LlPreviewDisplayExW = NULL;
    LlPrint = NULL;
    LlPrintAbort = NULL;
    LlPrintCheckLineFit = NULL;
    LlPrintEnd = NULL;
    LlPrintFields = NULL;
    LlPrintFieldsEnd = NULL;
    LlPrintGetCurrentPage = NULL;
    LlPrintGetItemsPerPage = NULL;
    LlPrintGetItemsPerTable = NULL;
    LlPrintGetRemainingItemsPerTableA = NULL;
    LlPrintGetRemainingItemsPerTableW = NULL;
    LlPrintGetRemItemsPerTableA = NULL;
    LlPrintGetRemItemsPerTableW = NULL;
    LlPrintGetOption = NULL;
    LlPrintGetPrinterInfoA = NULL;
    LlPrintGetPrinterInfoW = NULL;
    LlPrintOptionsDialogA = NULL;
    LlPrintOptionsDialogW = NULL;
    LlPrintSelectOffsetEx = NULL;
    LlPrintSetBoxTextA = NULL;
    LlPrintSetBoxTextW = NULL;
    LlPrintSetOption = NULL;
    LlPrintUpdateBox = NULL;
    LlPrintStartA = NULL;
    LlPrintStartW = NULL;
    LlPrintWithBoxStartA = NULL;
    LlPrintWithBoxStartW = NULL;
    LlPrinterSetupA = NULL;
    LlPrinterSetupW = NULL;
    LlSelectFileDlgTitleExA = NULL;
    LlSelectFileDlgTitleExW = NULL;
    LlSetDlgboxMode = NULL;
    LlGetDlgboxMode = NULL;
    LlExprParseA = NULL;
    LlExprParseW = NULL;
    LlExprType = NULL;
    LlExprErrorA = NULL;
    LlExprErrorW = NULL;
    LlExprFree = NULL;
    LlExprEvaluateA = NULL;
    LlExprEvaluateW = NULL;
    LlExprGetUsedVarsA = NULL;
    LlExprGetUsedVarsW = NULL;
    LlSetOption = NULL;
    LlGetOption = NULL;
    LlSetOptionStringA = NULL;
    LlSetOptionStringW = NULL;
    LlGetOptionStringA = NULL;
    LlGetOptionStringW = NULL;
    LlPrintSetOptionStringA = NULL;
    LlPrintSetOptionStringW = NULL;
    LlPrintGetOptionStringA = NULL;
    LlPrintGetOptionStringW = NULL;
    LlDesignerProhibitAction = NULL;
    LlDesignerProhibitFunctionA = NULL;
    LlDesignerProhibitFunctionW = NULL;
    LlPrintEnableObjectA = NULL;
    LlPrintEnableObjectW = NULL;
    LlSetFileExtensionsA = NULL;
    LlSetFileExtensionsW = NULL;
    LlPrintGetTextCharsPrintedA = NULL;
    LlPrintGetTextCharsPrintedW = NULL;
    LlPrintGetFieldCharsPrintedA = NULL;
    LlPrintGetFieldCharsPrintedW = NULL;
    LlPrintIsVariableUsedA = NULL;
    LlPrintIsVariableUsedW = NULL;
    LlPrintIsFieldUsedA = NULL;
    LlPrintIsFieldUsedW = NULL;
    LlPrintOptionsDialogTitleA = NULL;
    LlPrintOptionsDialogTitleW = NULL;
    LlSetPrinterToDefaultA = NULL;
    LlSetPrinterToDefaultW = NULL;
    LlDefineSortOrderStart = NULL;
    LlDefineSortOrderA = NULL;
    LlDefineSortOrderW = NULL;
    LlPrintGetSortOrderA = NULL;
    LlPrintGetSortOrderW = NULL;
    LlDefineGroupingA = NULL;
    LlDefineGroupingW = NULL;
    LlPrintGetGroupingA = NULL;
    LlPrintGetGroupingW = NULL;
    LlAddCtlSupportA = NULL;
    LlAddCtlSupportW = NULL;
    LlPrintBeginGroup = NULL;
    LlPrintEndGroup = NULL;
    LlPrintGroupLine = NULL;
    LlPrintGroupHeader = NULL;
    LlPrintGetFilterExpressionA = NULL;
    LlPrintGetFilterExpressionW = NULL;
    LlPrintWillMatchFilter = NULL;
    LlPrintDidMatchFilter = NULL;
    LlGetFieldContentsA = NULL;
    LlGetFieldContentsW = NULL;
    LlGetVariableContentsA = NULL;
    LlGetVariableContentsW = NULL;
    LlGetSumVariableContentsA = NULL;
    LlGetSumVariableContentsW = NULL;
    LlGetUserVariableContentsA = NULL;
    LlGetUserVariableContentsW = NULL;
    LlGetVariableTypeA = NULL;
    LlGetVariableTypeW = NULL;
    LlGetFieldTypeA = NULL;
    LlGetFieldTypeW = NULL;
    LlPrintGetColumnInfoA = NULL;
    LlPrintGetColumnInfoW = NULL;
    LlSetPrinterDefaultsDirA = NULL;
    LlSetPrinterDefaultsDirW = NULL;
    LlCreateSketchA = NULL;
    LlCreateSketchW = NULL;
    LlViewerProhibitAction = NULL;
    LlPrintCopyPrinterConfigurationA = NULL;
    LlPrintCopyPrinterConfigurationW = NULL;
    LlSetPrinterInPrinterFileA = NULL;
    LlSetPrinterInPrinterFileW = NULL;
    LlRTFCreateObject = NULL;
    LlRTFDeleteObject = NULL;
    LlRTFSetTextA = NULL;
    LlRTFSetTextW = NULL;
    LlRTFGetTextLength = NULL;
    LlRTFGetTextA = NULL;
    LlRTFGetTextW = NULL;
    LlRTFEditObject = NULL;
    LlRTFCopyToClipboard = NULL;
    LlRTFDisplay = NULL;
    LlRTFEditorProhibitAction = NULL;
    LlRTFEditorInvokeAction = NULL;
    LlDebugOutputA = NULL;
    LlDebugOutputW = NULL;
    LlEnumGetFirstVar = NULL;
    LlEnumGetFirstField = NULL;
    LlEnumGetNextEntry = NULL;
    LlEnumGetEntryA = NULL;
    LlEnumGetEntryW = NULL;
    LlPrintResetObjectStates = NULL;
    LlXSetParameterA = NULL;
    LlXSetParameterW = NULL;
    LlXGetParameterA = NULL;
    LlXGetParameterW = NULL;
    LlPrintResetProjectState = NULL;
    LlDefineChartFieldStart = NULL;
    LlDefineChartFieldExtA = NULL;
    LlDefineChartFieldExtW = NULL;
    LlPrintDeclareChartRow = NULL;
    LlPrintGetChartObjectCount = NULL;
    LlPrintIsChartFieldUsedA = NULL;
    LlPrintIsChartFieldUsedW = NULL;
    LlGetChartFieldContentsA = NULL;
    LlGetChartFieldContentsW = NULL;
    LlEnumGetFirstChartField = NULL;
    LlSetNotificationCallbackExt = NULL;
    LlExprEvaluateVar = NULL;
    LlExprTypeVar = NULL;
    LlGetPrinterFromPrinterFileA = NULL;
    LlGetPrinterFromPrinterFileW = NULL;
    LlPrintGetRemainingSpacePerTableA = NULL;
    LlPrintGetRemainingSpacePerTableW = NULL;
    LlDrawToolbarBackground = NULL;
    LlSetDefaultProjectParameterA = NULL;
    LlSetDefaultProjectParameterW = NULL;
    LlGetDefaultProjectParameterA = NULL;
    LlGetDefaultProjectParameterW = NULL;
    LlPrintSetProjectParameterA = NULL;
    LlPrintSetProjectParameterW = NULL;
    LlPrintGetProjectParameterA = NULL;
    LlPrintGetProjectParameterW = NULL;
    LlCreateObject = NULL;
    LlExprContainsVariableA = NULL;
    LlExprContainsVariableW = NULL;
    LlExprIsConstant = NULL;
    LlLocSystemTimeFromLocaleStringA = NULL;
    LlLocSystemTimeFromLocaleStringW = NULL;
    LlLocSystemTimeToLocaleStringA = NULL;
    LlLocSystemTimeToLocaleStringW = NULL;
    LlLocConvertCountryToA = NULL;
    LlLocConvertCountryToW = NULL;
    LlProfileStartA = NULL;
    LlProfileStartW = NULL;
    LlProfileEnd = NULL;
    LlDumpMemory = NULL;
    LlDbAddTableA = NULL;
    LlDbAddTableW = NULL;
    LlDbAddTableRelationA = NULL;
    LlDbAddTableRelationW = NULL;
    LlDbAddTableSortOrderA = NULL;
    LlDbAddTableSortOrderW = NULL;
    LlPrintDbGetCurrentTableA = NULL;
    LlPrintDbGetCurrentTableW = NULL;
    LlPrintDbGetCurrentTableRelationA = NULL;
    LlPrintDbGetCurrentTableRelationW = NULL;
    LlPrintDbGetCurrentTableSortOrderA = NULL;
    LlPrintDbGetCurrentTableSortOrderW = NULL;
    LlDbDumpStructure = NULL;
    LlPrintDbGetRootTableCount = NULL;
    LlDbSetMasterTableA = NULL;
    LlDbSetMasterTableW = NULL;
    LlDbGetMasterTableA = NULL;
    LlDbGetMasterTableW = NULL;
    LlXSetExportParameterA = NULL;
    LlXSetExportParameterW = NULL;
    LlXGetExportParameterA = NULL;
    LlXGetExportParameterW = NULL;
    LlXlatNameA = NULL;
    LlXlatNameW = NULL;
    LlDefineVariableVarA = NULL;
    LlDefineVariableVarW = NULL;
    LlDefineFieldVarA = NULL;
    LlDefineFieldVarW = NULL;
    LlDefineChartFieldVarA = NULL;
    LlDefineChartFieldVarW = NULL;
    LlDesignerProhibitEditingObjectA = NULL;
    LlDesignerProhibitEditingObjectW = NULL;
    LlGetUsedIdentifiersA = NULL;
    LlGetUsedIdentifiersW = NULL;
    LlInternalAttachApp = NULL;
    LlInternalDetachApp = NULL;
    LlGetUserSectionDataA = NULL;
    LlGetUserSectionDataW = NULL;
    LlExprUpdateCollectionForLlX = NULL;
    LlExprGetUsedVarsExA = NULL;
    LlExprGetUsedVarsExW = NULL;
    LlDomGetProject = NULL;
    LlDomGetPropertyA = NULL;
    LlDomGetPropertyW = NULL;
    LlDomSetPropertyA = NULL;
    LlDomSetPropertyW = NULL;
    LlDomGetObjectA = NULL;
    LlDomGetObjectW = NULL;
    LlDomGetSubobjectCount = NULL;
    LlDomGetSubobject = NULL;
    LlDomCreateSubobjectA = NULL;
    LlDomCreateSubobjectW = NULL;
    LlDomDeleteSubobject = NULL;
    LlProjectOpenA = NULL;
    LlProjectOpenW = NULL;
    LlProjectSaveA = NULL;
    LlProjectSaveW = NULL;
    LlProjectClose = NULL;
    LlDomGetPropertyBSTR = NULL;
    LlAssociatePreviewControl = NULL;
    LlGetErrortextA = NULL;
    LlGetErrortextW = NULL;
    LlXOBStatJobOpen = NULL;
    LlSetPreviewOption = NULL;
    LlGetPreviewOption = NULL;
    LlDesignerInvokeAction = NULL;
    LlDesignerRefreshWorkspace = NULL;
    LlDesignerFileOpenA = NULL;
    LlDesignerFileOpenW = NULL;
    LlDesignerFileSaveA = NULL;
    LlDesignerFileSaveW = NULL;
    LlDesignerAddActionA = NULL;
    LlDesignerAddActionW = NULL;
    LlDesignerGetOptionStringA = NULL;
    LlDesignerGetOptionStringW = NULL;
    LlDesignerSetOptionStringA = NULL;
    LlDesignerSetOptionStringW = NULL;
    LlJobOpenCopy = NULL;
    LlGetProjectParameterA = NULL;
    LlGetProjectParameterW = NULL;
    LlConvertBLOBToStringA = NULL;
    LlConvertBLOBToStringW = NULL;
    LlConvertStringToBLOBA = NULL;
    LlConvertStringToBLOBW = NULL;
    LlConvertStreamToStringA = NULL;
    LlConvertStreamToStringW = NULL;
    LlConvertStringToStreamA = NULL;
    LlConvertStringToStreamW = NULL;
    LlConvertHGLOBALToStringA = NULL;
    LlConvertHGLOBALToStringW = NULL;
    LlConvertStringToHGLOBALA = NULL;
    LlConvertStringToHGLOBALW = NULL;
    LlAztecEncode = NULL;
    LlDbAddTableRelationExA = NULL;
    LlDbAddTableRelationExW = NULL;
    LlDbAddTableSortOrderExA = NULL;
    LlDbAddTableSortOrderExW = NULL;
    LlGetUsedIdentifiersExA = NULL;
    LlGetUsedIdentifiersExW = NULL;
    LlGetTempFileNameA = NULL;
    LlGetTempFileNameW = NULL;
    LlGetDebug = NULL;
    }
}
#endif

#ifdef __cplusplus
}
#endif

#if __WATCOMC__ > 1000 || _MSC_VER >= 1400 /* Watcom C++ >= 10.x or MSC >= 2005 */
#pragma pack(pop)
#elif __BORLANDC__ /* Borland C++ */
#pragma option -a.
#else
#pragma pack() /* others */
#endif

#ifdef IMPLEMENTATION
#undef extern
#endif

#endif  /* #ifndef _RC_INVOKED_ */

#endif  /* #ifndef _LL15_H */

