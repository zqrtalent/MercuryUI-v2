/**** C/C++ run-time linkage constants and function definitions for LS15.DLL ****/
/****  (c) 1991,..,1999,2000,..,06,... combit GmbH, Konstanz, Germany  ****/
/****  [build of 2009-10-15 14:10:27] ****/

#ifndef _LS15_H
#define _LS15_H
#define _LS15_HX

#ifndef _LS15_DONT_INCLUDE_OLE2_H
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

#ifndef _PCRECT
  #define _PCRECT                        const RECT FAR *
#endif
#ifndef _PRECT
  #define _PRECT                         RECT FAR *
#endif
#ifndef HLLSTG
  #define HLLSTG                         UINT_PTR
#endif
#ifndef HSTG
  #define HSTG                           UINT_PTR
#endif
#ifndef PHGLOBAL
  #define PHGLOBAL                       HGLOBAL FAR *
#endif
#ifndef HLSCNVJOB
  #define HLSCNVJOB                      UINT_PTR
#endif
#ifndef HLSMAILJOB
  #define HLSMAILJOB                     UINT_PTR
#endif
#ifndef _LPCWORD
  #define _LPCWORD                       const UINT16 FAR *
#endif
#ifndef PSTREAM
  #define PSTREAM                        IStream FAR *
#endif
#ifndef PCSCDIOOPTIONS
  #define PCSCDIOOPTIONS                 const scDIOOptions FAR *
#endif
#ifndef _PIEnumString
  #define _PIEnumString                  IEnumString FAR *
#endif
#ifndef _PPIEnumString
  #define _PPIEnumString                 IEnumString FAR * FAR *
#endif
#ifndef _PILLPreviewFileSystem
  #define _PILLPreviewFileSystem         ILLPreviewFileSystem FAR *
#endif
#ifndef PSCSTORAGEOPENINFO
  #define PSCSTORAGEOPENINFO             const scStorageOpenInfo FAR *
#endif

/*--- constant declarations ---*/

#define LL_ERR_STG_NOSTORAGE           (-1000               )
#define LL_ERR_STG_BADVERSION          (-1001               )
#define LL_ERR_STG_READ                (-1002               )
#define LL_ERR_STG_WRITE               (-1003               )
#define LL_ERR_STG_UNKNOWNSYSTEM       (-1004               )
#define LL_ERR_STG_BADHANDLE           (-1005               )
#define LL_ERR_STG_ENDOFLIST           (-1006               )
#define LL_ERR_STG_BADJOB              (-1007               )
#define LL_ERR_STG_ACCESSDENIED        (-1008               )
#define LL_ERR_STG_BADSTORAGE          (-1009               )
#define LL_ERR_STG_CANNOTGETMETAFILE   (-1010               )
#define LL_ERR_STG_OUTOFMEMORY         (-1011               )
#define LL_ERR_STG_SEND_FAILED         (-1012               )
#define LL_ERR_STG_DOWNLOAD_PENDING    (-1013               )
#define LL_ERR_STG_DOWNLOAD_FAILED     (-1014               )
#define LL_ERR_STG_WRITE_FAILED        (-1015               )
#define LL_ERR_STG_UNEXPECTED          (-1016               )
#define LL_ERR_STG_CANNOTCREATEFILE    (-1017               )
#define LL_ERR_STG_UNKNOWN_CONVERTER   (-1018               )
#define LL_ERR_STG_INET_ERROR          (-1019               )
#define LL_WRN_STG_UNFAXED_PAGES       (-1100               )
#define LS_OPTION_HAS16BITPAGES        (200                 ) /* has job 16 bit pages? */
#define LS_OPTION_BOXTYPE              (201                 ) /* wait meter box type */
#define LS_OPTION_UNITS                (203                 ) /* LL_UNITS_INCH_DIV_100 or LL_UNITS_MM_DIV_10 */
#define LS_OPTION_PRINTERCOUNT         (204                 ) /* number of printers (1 or 2) */
#define LS_OPTION_ISSTORAGE            (205                 ) /* returns whether file is STORAGE or COMPAT4 */
#define LS_OPTION_EMFRESOLUTION        (206                 ) /* EMFRESOLUTION used to print the file */
#define LS_OPTION_JOB                  (207                 ) /* returns current job number */
#define LS_OPTION_TOTALPAGES           (208                 ) /* differs from GetPageCount() if print range in effect */
#define LS_OPTION_PAGESWITHFAXNUMBER   (209                 )
#define LS_OPTION_HASINPUTOBJECTS      (210                 )
#define LS_OPTION_HASFORCEDINPUTOBJECTS (211                 )
#define LS_OPTION_INPUTOBJECTSFINISHED (212                 )
#define LS_OPTION_HASHYPERLINKS        (213                 )
#define LS_OPTION_PAGENUMBER           (0                   ) /* page number of current page */
#define LS_OPTION_COPIES               (1                   ) /* number of copies (same for all pages at the moment) */
#define LS_OPTION_PRN_ORIENTATION      (2                   ) /* orientation (DMORIENT_LANDSCAPE, DMORIENT_PORTRAIT) */
#define LS_OPTION_PHYSPAGE             (3                   ) /* is page "physical page" oriented? */
#define LS_OPTION_PRN_PIXELSOFFSET_X   (4                   ) /* this and the following values are */
#define LS_OPTION_PRN_PIXELSOFFSET_Y   (5                   ) /* values of the printer that the preview was */
#define LS_OPTION_PRN_PIXELS_X         (6                   ) /* created on! */
#define LS_OPTION_PRN_PIXELS_Y         (7                   )
#define LS_OPTION_PRN_PIXELSPHYSICAL_X (8                   )
#define LS_OPTION_PRN_PIXELSPHYSICAL_Y (9                   )
#define LS_OPTION_PRN_PIXELSPERINCH_X  (10                  )
#define LS_OPTION_PRN_PIXELSPERINCH_Y  (11                  )
#define LS_OPTION_PRN_INDEX            (12                  ) /* printer index of the page (0/1) */
#define LS_OPTION_PRN_PAPERTYPE        (13                  )
#define LS_OPTION_PRN_PAPERSIZE_X      (14                  )
#define LS_OPTION_PRN_PAPERSIZE_Y      (15                  )
#define LS_OPTION_PRN_FORCE_PAPERSIZE  (16                  )
#define LS_OPTION_STARTNEWSHEET        (17                  )
#define LS_OPTION_ISSUEINDEX           (18                  )
#define LS_OPTION_PROJECTNAME          (100                 ) /* name of the original project (not page dependent) */
#define LS_OPTION_JOBNAME              (101                 ) /* name of the job (WindowTitle of LlPrintWithBoxStart()) (not page dependent) */
#define LS_OPTION_PRTNAME              (102                 ) /* deprecated! */
#define LS_OPTION_PRTDEVICE            (103                 ) /* printer device ("HP Laserjet 4L") */
#define LS_OPTION_PRTPORT              (104                 ) /* deprecated! */
#define LS_OPTION_USER                 (105                 ) /* user string (not page dependent) */
#define LS_OPTION_CREATION             (106                 ) /* creation date (not page dependent) */
#define LS_OPTION_CREATIONAPP          (107                 ) /* creation application (not page dependent) */
#define LS_OPTION_CREATIONDLL          (108                 ) /* creation DLL (not page dependent) */
#define LS_OPTION_CREATIONUSER         (109                 ) /* creation user and computer name (not page dependent) */
#define LS_OPTION_FAXPARA_QUEUE        (110                 ) /* NYI */
#define LS_OPTION_FAXPARA_RECIPNAME    (111                 ) /* NYI */
#define LS_OPTION_FAXPARA_RECIPNUMBER  (112                 ) /* NYI */
#define LS_OPTION_FAXPARA_SENDERNAME   (113                 ) /* NYI */
#define LS_OPTION_FAXPARA_SENDERCOMPANY (114                 ) /* NYI */
#define LS_OPTION_FAXPARA_SENDERDEPT   (115                 ) /* NYI */
#define LS_OPTION_FAXPARA_SENDERBILLINGCODE (116                 ) /* NYI */
#define LS_OPTION_FAX_AVAILABLEQUEUES  (118                 ) /* NYI, nPageIndex=1 */
#define LS_OPTION_PRINTERALIASLIST     (119                 ) /* alternative printer list (taken from project) */
#define LS_OPTION_PRTDEVMODE           (120                 ) /* r/o, DEVMODEW structure, to be used with the LlConvertXxxx API */
#define LS_OPTION_USED_PRTDEVICE       (121                 ) /* r/o, printer name that would actually be used */
#define LS_OPTION_USED_PRTDEVMODE      (122                 ) /* r/o, DEVMODEW structure, to be used with the LlConvertXxxx API */
#define LS_PRINTFLAG_FIT               (0x00000001          )
#define LS_PRINTFLAG_STACKEDCOPIES     (0x00000002          ) /* n times page1, n times page2, ... (else n times (page 1...x)) */
#define LS_PRINTFLAG_TRYPRINTERCOPIES  (0x00000004          ) /* first try printer copies, then simulated ones... */
#define LS_PRINTFLAG_SHOWDIALOG        (0x00000008          )
#define LS_PRINTFLAG_METER             (0x00000010          )
#define LS_PRINTFLAG_ABORTABLEMETER    (0x00000020          )
#define LS_PRINTFLAG_METERMASK         (0x00000070          ) /* allows 7 styles of abort boxes... */
#define LS_PRINTFLAG_USEDEFPRINTERIFNULL (0x00000080          )
#define LS_PRINTFLAG_FAX               (0x00000100          )
#define LS_VIEWERCONTROL_QUERY_CHARWIDTH (1                   ) /* sent in wParam using LsGetViewerControlDefaultMessage() (return: 1 for SBCS, 2 for Unicode) */
#define LS_VIEWERCONTROL_CLEAR         (WM_USER+1           )
#define LS_VIEWERCONTROL_SET_HANDLE_EX (WM_USER+2           ) /* wParam = HANDLE (NULL for RELEASE), lParam = internal struct handle; */
#define LS_VIEWERCONTROL_SET_HANDLE    (WM_USER+3           ) /* wParam = HANDLE (NULL for RELEASE) */
#define LS_VIEWERCONTROLSETHANDLEFLAG_ADD (0x0100              )
#define LS_VIEWERCONTROLSETHANDLEFLAG_DELETE_ON_CLOSE (0x0200              )
#define LS_VIEWERCONTROL_GET_HANDLE    (WM_USER+4           ) /* lParam = HANDLE (NULL for none) */
#define LS_VIEWERCONTROL_SET_FILENAME  (WM_USER+5           ) /* lParam = LPCTSTR pszFilename (NULL for RELEASE), wParam = options */
#define LS_STGFILEOPEN_READONLY        (0x00000000          )
#define LS_STGFILEOPEN_READWRITE       (0x00000001          )
#define LS_STGFILEOPEN_FORCE_NO_READWRITE (0x00000002          ) /* never open read-write, even if formula elements are present! */
#define LS_STGFILEOPEN_DELETE_ON_CLOSE (0x00000004          )
#define LS_STGFILEOPENFLAG_ADD         (0x00000100          )
#define LS_VIEWERCONTROL_SET_OPTION    (WM_USER+6           )
#define LS_OPTION_MESSAGE              (0                   ) /* communication message */
#define LS_OPTION_PRINTERASSIGNMENT    (1                   ) /* set BEFORE setting the storage handle/filename! */
#define LS_PRNASSIGNMENT_USEDEFAULT    (0x00000000          )
#define LS_PRNASSIGNMENT_ASKPRINTERIFNEEDED (0x00000001          )
#define LS_PRNASSIGNMENT_ASKPRINTERALWAYS (0x00000002          )
#define LS_PRNASSIGNMENT_ALWAYSUSEDEFAULT (0x00000003          ) /* default */
#define LS_OPTION_TOOLBAR              (2                   ) /* TRUE to force viewer control to display a toolbar, FALSE otherwise (def: FALSE) */
#define LS_OPTION_SKETCHBAR            (3                   ) /* TRUE to force viewer control to display a sketch bar (def: TRUE) */
#define LS_OPTION_SKETCHBARWIDTH       (4                   ) /* TRUE to force viewer control to display a sketch bar (def: 50) */
#define LS_OPTION_TOOLBARSTYLE         (5                   ) /* default: LS_OPTION_TOOLBARSTYLE_STANDARD, set BEFORE LS_OPTION_TOOLBAR to TRUE! */
#define LS_OPTION_TOOLBARSTYLE_STANDARD (0                   ) /* OFFICE97 alike style */
#define LS_OPTION_TOOLBARSTYLE_OFFICEXP (1                   ) /* DOTNET/OFFICE_XP alike style */
#define LS_OPTION_TOOLBARSTYLE_OFFICE2003 (2                   )
#define LS_OPTION_TOOLBARSTYLEMASK     (0x0f                )
#define LS_OPTION_TOOLBARSTYLEFLAG_GRADIENT (0x80                ) /* starting with XP, use gradient style */
#define LS_OPTION_CODEPAGE             (7                   ) /* lParam = codepage for MBCS aware string operations - set it if the system default is not applicable */
#define LS_OPTION_SAVEASFILEPATH       (8                   ) /* w/o, lParam = "SaveAs" default filename (LPCTSTR!) */
#define LS_OPTION_USERDATA             (9                   ) /* for LS_VIEWERCONTROL_SET_NTFYCALLBACK */
#define LS_OPTION_BGCOLOR              (10                  ) /* background color */
#define LS_OPTION_ASYNC_DOWNLOAD       (11                  ) /* download is ASYNC (def: TRUE) */
#define LS_OPTION_LANGUAGE             (12                  ) /* CMBTLANG_xxx or -1 for ThreadLocale */
#define LS_OPTION_ASSUME_TEMPFILE      (13                  ) /* viewer assumes that the LL file is a temp file, so data can not be saved into it */
#define LS_OPTION_IOLECLIENTSITE       (14                  ) /* internal use */
#define LS_OPTION_TOOLTIPS             (15                  ) /* lParam = flag value */
#define LS_OPTION_AUTOSAVE             (16                  ) /* lParam = (BOOL)bAutoSave */
#define LS_OPTION_CHANGEDFLAG          (17                  ) /* lParam = flag value */
#define LS_OPTION_SHOW_UNPRINTABLE_AREA (18                  ) /* lParam = flags, default: TRUE */
#define LS_OPTION_NOUIRESET            (19                  ) /* lParam = flags, default: TRUE */
#define LS_OPTION_NAVIGATIONBAR        (20                  ) /* TRUE to force viewer control to display a sketch bar (def: TRUE) */
#define LS_OPTION_NAVIGATIONBARWIDTH   (21                  ) /* TRUE to force viewer control to display a sketch bar (def: 50) */
#define LS_OPTION_IN_PREVIEWPANE       (22                  ) /* TRUE to disable unneeded message boxes */
#define LS_OPTION_IN_LLVIEWER          (23                  ) /* internal */
#define LS_OPTION_TABBARSTYLE          (24                  )
#define LS_OPTION_TABBARSTYLE_STANDARD (0                   )
#define LS_OPTION_TABBARSTYLE_OFFICEXP (1                   )
#define LS_OPTION_TABBARSTYLE_OFFICE2003 (2                   )
#define LS_OPTION_DESIGNERPREVIEW      (25                  )
#define LS_VIEWERCONTROL_GET_OPTION    (WM_USER+7           )
#define LS_VIEWERCONTROL_QUERY_ENDSESSION (WM_USER+8           )
#define LS_VIEWERCONTROL_GET_ZOOM      (WM_USER+9           )
#define LS_VIEWERCONTROL_SET_ZOOM      (WM_USER+10          ) /* wParam = factor (lParam = 1 if in percent) */
#define LS_VIEWERCONTROL_GET_ZOOMED    (WM_USER+11          ) /* TRUE if zoomed */
#define LS_VIEWERCONTROL_POP_ZOOM      (WM_USER+12          )
#define LS_VIEWERCONTROL_RESET_ZOOM    (WM_USER+13          )
#define LS_VIEWERCONTROL_SET_ZOOM_TWICE (WM_USER+14          )
#define LS_VIEWERCONTROL_SET_PAGE      (WM_USER+20          ) /* wParam = page# (0..n-1) */
#define LS_VIEWERCONTROL_GET_PAGE      (WM_USER+21          )
#define LS_VIEWERCONTROL_GET_PAGECOUNT (WM_USER+22          )
#define LS_VIEWERCONTROL_GET_PAGECOUNT_FAXPAGES (WM_USER+23          )
#define LS_VIEWERCONTROL_GET_JOB       (WM_USER+24          )
#define LS_VIEWERCONTROL_GET_JOBPAGEINDEX (WM_USER+25          )
#define LS_VIEWERCONTROL_GET_METAFILE  (WM_USER+26          ) /* wParam = page#, for IMMEDIATE use (will be released by LS DLL at some undefined time!) */
#define LS_VIEWERCONTROL_GET_ENABLED   (WM_USER+27          ) /* wParam = ID */
#define LS_VCITEM_SEARCH_FIRST         (0                   )
#define LS_VCITEM_SEARCH_NEXT          (1                   )
#define LS_VCITEM_SEARCH_PREV          (2                   )
#define LS_VCITEM_SEARCHFLAG_CASEINSENSITIVE (0x8000              )
#define LS_VCITEM_SAVE_AS_FILE         (3                   )
#define LS_VCITEM_SEND_AS_MAIL         (4                   )
#define LS_VCITEM_SEND_AS_FAX          (5                   )
#define LS_VCITEM_PRINT_ONE            (6                   )
#define LS_VCITEM_PRINT_ALL            (7                   )
#define LS_VCITEM_PAGENUMBER           (8                   )
#define LS_VCITEM_ZOOM                 (9                   )
#define LS_VCITEM_THEATERMODE          (10                  )
#define LS_VCITEM_PREVSTG              (11                  )
#define LS_VCITEM_NEXTSTG              (12                  )
#define LS_VIEWERCONTROL_GET_SEARCHSTATE (WM_USER+28          ) /* returns TRUE if search in progress */
#define LS_VIEWERCONTROL_SEARCH        (WM_USER+29          ) /* wParam = BOOL(bCaseSens), lParam=SearchText (NULL to stop) */
#define LS_VIEWERCONTROL_GET_ENABLED_SEARCHPREV (WM_USER+30          )
#define LS_VIEWERCONTROL_PRINT_CURRENT (WM_USER+31          ) /* wParam = 0 (default printer), 1 (with printer selection) */
#define LS_VIEWERCONTROL_PRINT_ALL     (WM_USER+32          ) /* wParam = 0 (default printer), 1 (with printer selection) */
#define LS_VIEWERCONTROL_PRINT_TO_FAX  (WM_USER+33          )
#define LS_VIEWERCONTROL_UPDATE_TOOLBAR (WM_USER+35          ) /* if LS_OPTION_TOOLBAR is TRUE */
#define LS_VIEWERCONTROL_GET_TOOLBAR   (WM_USER+36          ) /* if LS_OPTION_TOOLBAR is TRUE, returns window handle of toolbar */
#define LS_VIEWERCONTROL_SAVE_TO_FILE  (WM_USER+37          )
#define LS_VIEWERCONTROL_SEND_AS_MAIL  (WM_USER+39          )
#define LS_VIEWERCONTROL_SET_OPTIONSTR (WM_USER+40          ) /* see docs, wParam = (LPCTSTR)key, lParam = (LPCTSTR)value */
#define LS_VIEWERCONTROL_GET_OPTIONSTR (WM_USER+41          ) /* see docs, wParam = (LPCTSTR)key, lParam = (LPCTSTR)value */
#define LS_VIEWERCONTROL_GET_OPTIONSTRLEN (WM_USER+42          ) /* see docs, wParam = (LPCTSTR)key (returns size in TCHARs) */
#define LS_VIEWERCONTROL_SET_NTFYCALLBACK (WM_USER+43          ) /* lParam = LRESULT ( WINAPI fn* )(UINT nMsg, LPARAM lParam, UINT nUserParameter); */
#define LS_VIEWERCONTROL_GET_NTFYCALLBACK (WM_USER+44          ) /* LRESULT ( WINAPI fn* )(UINT nMsg, LPARAM lParam, UINT nUserParameter); */
#define LS_VIEWERCONTROL_GET_TOOLBARBUTTONSTATE (WM_USER+45          ) /* wParam=nID -> -1=hidden, 1=enabled, 2=disabled (only when toobar present, to sync menu state) */
#define LS_VIEWERCONTROL_SET_FOCUS     (WM_USER+46          )
#define LS_VCSF_PREVIEW                (1                   )
#define LS_VCSF_SKETCHLIST             (2                   )
#define LS_VCSF_TOC                    (3                   )
#define LS_VIEWERCONTROL_ADDTOOLBARITEM (WM_USER+47          )
#define LS_VIEWERCONTROL_INTERNAL_CHECKERRORLIST (WM_USER+48          )
#define LS_VIEWERCONTROL_SET_THEATERMODE (WM_USER+49          ) /* 0=non-theater, 1=with frame, 2=without frame */
#define LS_VIEWERCONTROL_SET_THEATERFLIPDELAY (WM_USER+50          ) /* ms for each page */
#define LS_VIEWERCONTROL_SET_THEATERFLIPMODE (WM_USER+51          ) /* wParam = mode */
#define LS_VCTFM_NONE                  (0                   )
#define LS_VCTFM_LINEAR                (1                   ) /* lParam = (LPCTSTR)ProgID */
#define LS_VCTFM_FADE                  (2                   )
#define LS_VCTFM_WHEEL                 (3                   )
#define LS_VIEWERCONTROL_SELECT_THEATERXFORM (WM_USER+52          )
#define LS_VIEWERCONTROL_NTFY_PRVFSCHANGED (WM_USER+53          ) /* wParam = ILLPreviewFileSystemChangeNotifier::enPrvFSChange.. */
#define LS_VIEWERCONTROL_SET_PROGRESSINFO (WM_USER+54          ) /* wParam = nPercentage (-1=finished...), lParam = (LPCTSTR)sText */
#define LS_VIEWERCONTROL_GET_FILENAME  (WM_USER+55          ) /* lParam = LPTSTR pszFilename, wParam = sizeofTSTR(pszBuffer). Returns size needed if NULL filename */
#define LS_VIEWERCONTROL_QUERY_PRVFS_COMPLETE (WM_USER+56          ) /* indicates whether the STGSYS file is complete (1=complete, 2=finished, but incomplete) */
#define LS_VIEWERCONTROL_ONSIZEMOVE    (WM_USER+57          ) /* wParam = 0 (ENTER), 1 (EXIT) */
#define LS_VIEWERCONTROL_NTFY_SHOW     (WM_USER+58          ) /* internal use */
#define LS_VIEWERCONTROL_GET_IDEVICEINFO (WM_USER+59          ) /* internal use */
#define LS_VIEWERCONTROL_REMOVEFAILURETOOLTIPS (WM_USER+60          ) /* internal use */
#define LS_VIEWERCONTROL_SET_LLNTFYSINK (WM_USER+61          ) /* internal use */
#define LS_VIEWERCONTROL_OPEN_PREVSTG  (WM_USER+62          )
#define LS_VIEWERCONTROL_OPEN_NEXTSTG  (WM_USER+63          )
#define LS_VIEWERCONTROL_NTFY_PAGELOADED (1                   ) /* lParam = page# */
#define LS_VIEWERCONTROL_NTFY_UPDATETOOLBAR (2                   ) /* called when control does NOT have an own toolbar. lParam = 1 if count of pages did change */
#define LS_VIEWERCONTROL_NTFY_PRINT_START (3                   ) /* lParam = &scViewerControlPrintData, return 1 to abort print */
#define LS_VIEWERCONTROL_NTFY_PRINT_PAGE (4                   ) /* lParam = &scViewerControlPrintData, return 1 to abort loop */
#define LS_VIEWERCONTROL_NTFY_PRINT_END (5                   ) /* lParam = &scViewerControlPrintData */
#define LS_VIEWERCONTROL_NTFY_TOOLBARUPDATE (6                   ) /* lParam = toolbar handle, called when control has an own toolbar */
#define LS_VIEWERCONTROL_NTFY_EXITBTNPRESSED (7                   )
#define LS_VIEWERCONTROL_NTFY_BTNPRESSED (8                   ) /* lParam = control ID */
#define LS_VIEWERCONTROL_QUEST_BTNSTATE (9                   ) /* lParam = control ID, -1 to hide, 1 to show, 2 to disable (0 to use default) */
#define LS_VIEWERCONTROL_NTFY_ERROR    (10                  ) /* lParam = &scVCError. Return != 0 to suppress error mbox from control. */
#define LS_VIEWERCONTROL_NTFY_MAIL_SENT (11                  ) /* lParam = Stream* of EML mail contents */
#define LS_VIEWERCONTROL_NTFY_DOWNLOADFINISHED (12                  ) /* lParam = 0 (failed), 1 (ok) */
#define LS_VIEWERCONTROL_NTFY_KEYBOARDMESSAGE (13                  ) /* lParam = const MSG*. Return TRUE if message should be taken out of the input queue */
#define LS_VIEWERCONTROL_NTFY_VIEWCHANGED (14                  ) /* lParam = const scViewChangedInfo */
#define LS_VIEWERCONTROL_CMND_SAVEDATA (15                  ) /* return: 0 = OK, -1 = failure, 1 = save in LL file too [event used only if AUTOSAVE is TRUE] */
#define LS_VIEWERCONTROL_NTFY_DATACHANGED (16                  )
#define LS_VIEWERCONTROL_NTFY_PROGRESS (17                  ) /* lParam = percentage (-1=finished). return: 1 if internal progress bar shall be suppressed */
#define LS_VIEWERCONTROL_QUEST_SUPPORTCONTINUATION (18                  ) /* return: 1 if continuation button () should be displayed */
#define LS_VIEWERCONTROL_CMND_CONTINUE (19                  ) /* continue report! */
#define LS_VIEWERCONTROL_NTFY_VIEWERDRILLDOWN (20                  ) /* lParam:  */
#define LS_VIEWERCONTROL_QUEST_DRILLDOWNSUPPORT (21                  ) /* 1 to allow (default), 0 to deny (if provider cannot handle multiple threads or so) */
#define LS_VIEWERCONTROL_OPEN_STORAGE_IN_NEW_WINDOW (22                  ) /* lParam = &scOpenStorageInNewWindow. Reply with 1 if done. */
#define LS_MAILCONFIG_GLOBAL           (0x0001              )
#define LS_MAILCONFIG_USER             (0x0002              )
#define LS_MAILCONFIG_PROVIDER         (0x0004              )
#define LS_DIO_CHECKBOX                (0                   )
#define LS_DIO_PUSHBUTTON              (1                   )
#define LS_GOTFG_FLAG_REORDER          (0x00000001          )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_NONE (0x0000              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_NEW_WINDOW (0x0001              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_NAVIGATION (0x0002              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_JAVA (0x0004              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_SCRIPTING (0x0008              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_ACTIVEX (0x0010              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_ONLINE (0x0020              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_BROWSERCONTEXTMENU (0x0040              )
#define LSMAILVIEW_HTMLRIGHT_ALLOW_PRINT (0x0080              )

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
  static HINSTANCE g_hDLLLS15 = NULL;
  static INT       g_nDLLLS15Counter = 0;
#endif


  #define LS_MAKE_NTFY_PRVFSCHANGED_WPARAM(nCmd,nSerialID,nJob,nPage) MAKELONG(nCmd,nSerialID)
  #define LS_MAKE_NTFY_PRVFSCHANGED_LPARAM(nCmd,nSerialID,nJob,nPage) MAKELONG(nPage,nJob)
  #define LS_NTFY_PRVFSCHANGED_CMD(wParam,lParam)      LOWORD(wParam)
  #define LS_NTFY_PRVFSCHANGED_SERIALID(wParam,lParam)     HIWORD(wParam)
  #define LS_NTFY_PRVFSCHANGED_JOB(wParam,lParam)      HIWORD(lParam)
  #define LS_NTFY_PRVFSCHANGED_PAGE(wParam,lParam)      LOWORD(lParam)
 #ifndef IMPLEMENTATION
 
 struct scOpenStorageInNewWindow
   {
   UINT        _nSize;
   LPCWSTR     _pszStgPath;
   BOOL  _bDeleteOnClose;
   };
 
 #endif // IMPLEMENTATION
 #ifndef IMPLEMENTATION
 struct scViewerControlPrintDataA
  {
  UINT  _nSize;
  UINT  _nPages;  // total count of pages
  UINT  _nABCABCCopies; // total count of ABCABC copies
  UINT  _nPage;   // current page (0.._nPages-1)
  UINT  _nCopy;   // current copy (0.._nABCABCCopies-1)
  UINT  _nABCABCCopy;
  LPCSTR  _pszDevice;  // LS_VIEWERCONTROL_NTFY_PRINT_START: "FAX" or "Printer"; LS_VIEWERCONTROL_NTFY_PRINT_PAGE: device name; NULL else
  LPCSTR  _pszProject; // dito
  LPCSTR  _pszJobName; // dito
  UINT  _nAAABBBCCCCopies; // total amount
  };
 struct scViewerControlPrintDataW
  {
  UINT  _nSize;
  UINT  _nPages;  // total count of pages
  UINT  _nABCABCCopies; // total count of ABCABC copies
  UINT  _nPage;   // current page (0.._nPages-1)
  UINT  _nCopy;   // current copy (0.._nABCABCCopies-1)
  UINT  _nABCABCCopy;
  LPCWSTR  _pszDevice;  // LS_VIEWERCONTROL_NTFY_PRINT_START: "FAX" or "Printer"; LS_VIEWERCONTROL_NTFY_PRINT_PAGE: device name; NULL else
  LPCWSTR  _pszProject; // dito
  LPCWSTR  _pszJobName; // dito
  UINT  _nAAABBBCCCCopies; // total amount
  };
 #if defined(UNICODE)
     #define scViewerControlPrintData scViewerControlPrintDataW
   #else
     #define scViewerControlPrintData scViewerControlPrintDataA
 #endif
 
 struct scVCErrorW
  {
  UINT  _nSize;
  UINT  _nErrorCode;
  LPCWSTR  _pszErrortext;
  };
 struct scVCErrorA
  {
  UINT  _nSize;
  UINT  _nErrorCode;
  LPCSTR  _pszErrortext;
  };
 #if defined(UNICODE)
     #define scVCError scVCErrorW
   #else
     #define scVCError scVCErrorA
 #endif
 
 struct scViewChangedInfo
  {
  UINT  _nSize;
  double  _fZoom;
  double  _fOffsetX;
  double  _fOffsetY;
   BOOL  _bPresentationMode;
  };
 #endif // IMPLEMENTATION
 #if !defined(IMPLEMENTATION)
 struct scDIOOptions
  {
  int  _nSize;
  int  _nFlags; // reserved
  };
 #if defined(__cplusplus)
 struct scDIOOptionsCheckbox
  : public scDIOOptions
  {
  int  _nChecked;
  int  _nStyle;
  BOOL _bFocus;
   LPCWSTR _pszText; // NULL for now, reserved...
  };
 struct scDIOOptionsPushbutton
  : public scDIOOptions
  {
  BOOL _bPressed;
  int  _nStyle;
  BOOL _bFocus;
   LPCWSTR _pszText;
   HFONT   _hFont;
  };
 #endif /* __cplusplus */
 #endif /* IMPLEMENTATION */
 interface ILLPreviewFileSystem;
 #ifndef IMPLEMENTATION
 
 struct scStorageOpenInfo
   {
   UINT        _nSize;
   LPCWSTR     _pszTempPath;
   void*       _pApp;
   bool        _bReadOnly;
   bool        _bOneJobTranslation;
   };
 
 #endif // IMPLEMENTATION
 
 



/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageOpenO LlStgsysStorageOpenA
    #else
      #define LlStgsysStorageOpen LlStgsysStorageOpenA
  #endif
typedef   HLLSTG ( DLLPROC *PFNLLSTGSYSSTORAGEOPENA)(
	LPCSTR               lpszFilename,
	LPCSTR               pszTempPath,
	BOOL                 bReadOnly,
	BOOL                 bOneJobTranslation);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGEOPENA LlStgsysStorageOpenA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageOpen LlStgsysStorageOpenW
    #else
      #define LlStgsysStorageOpenO LlStgsysStorageOpenW
  #endif
typedef   HLLSTG ( DLLPROC *PFNLLSTGSYSSTORAGEOPENW)(
	LPCWSTR              lpszFilename,
	LPCWSTR              pszTempPath,
	BOOL                 bReadOnly,
	BOOL                 bOneJobTranslation);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGEOPENW LlStgsysStorageOpenW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLLSTGSYSSTORAGECLOSE)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGECLOSE LlStgsysStorageClose;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETAPIVERSION)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETAPIVERSION LlStgsysGetAPIVersion;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETFILEVERSION)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETFILEVERSION LlStgsysGetFileVersion;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetFilenameO LlStgsysGetFilenameA
    #else
      #define LlStgsysGetFilename LlStgsysGetFilenameA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETFILENAMEA)(
	HLLSTG               hStg,
	INT                  nJob,
	INT                  nFile,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETFILENAMEA LlStgsysGetFilenameA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetFilename LlStgsysGetFilenameW
    #else
      #define LlStgsysGetFilenameO LlStgsysGetFilenameW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETFILENAMEW)(
	HLLSTG               hStg,
	INT                  nJob,
	INT                  nFile,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETFILENAMEW LlStgsysGetFilenameW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETJOBCOUNT)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETJOBCOUNT LlStgsysGetJobCount;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSSETJOB)(
	HLLSTG               hStg,
	INT                  nJob);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSETJOB LlStgsysSetJob;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETJOB)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETJOB LlStgsysGetJob;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETPAGECOUNT)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETPAGECOUNT LlStgsysGetPageCount;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETJOBOPTIONVALUE)(
	HLLSTG               hStg,
	INT                  nOption);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETJOBOPTIONVALUE LlStgsysGetJobOptionValue;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETPAGEOPTIONVALUE)(
	HLLSTG               hStg,
	INT                  nPageIndex,
	INT                  nOption);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETPAGEOPTIONVALUE LlStgsysGetPageOptionValue;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetPageOptionStringO LlStgsysGetPageOptionStringA
    #else
      #define LlStgsysGetPageOptionString LlStgsysGetPageOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETPAGEOPTIONSTRINGA)(
	HLLSTG               hStg,
	INT                  nPageIndex,
	INT                  nOption,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETPAGEOPTIONSTRINGA LlStgsysGetPageOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetPageOptionString LlStgsysGetPageOptionStringW
    #else
      #define LlStgsysGetPageOptionStringO LlStgsysGetPageOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETPAGEOPTIONSTRINGW)(
	HLLSTG               hStg,
	INT                  nPageIndex,
	INT                  nOption,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETPAGEOPTIONSTRINGW LlStgsysGetPageOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysSetPageOptionStringO LlStgsysSetPageOptionStringA
    #else
      #define LlStgsysSetPageOptionString LlStgsysSetPageOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSETPAGEOPTIONSTRINGA)(
	HLLSTG               hStg,
	INT                  nPageIndex,
	INT                  nOption,
	LPCSTR               pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSETPAGEOPTIONSTRINGA LlStgsysSetPageOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysSetPageOptionString LlStgsysSetPageOptionStringW
    #else
      #define LlStgsysSetPageOptionStringO LlStgsysSetPageOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSETPAGEOPTIONSTRINGW)(
	HLLSTG               hStg,
	INT                  nPageIndex,
	INT                  nOption,
	LPCWSTR              pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSETPAGEOPTIONSTRINGW LlStgsysSetPageOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSAPPEND)(
	HLLSTG               hStg,
	HLLSTG               hStgToAppend);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSAPPEND LlStgsysAppend;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSDELETEJOB)(
	HLLSTG               hStg,
	INT                  nPageIndex);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSDELETEJOB LlStgsysDeleteJob;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSDELETEPAGE)(
	HLLSTG               hStg,
	INT                  nPageIndex);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSDELETEPAGE LlStgsysDeletePage;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef   HANDLE ( DLLPROC *PFNLLSTGSYSGETPAGEMETAFILE)(
	HLLSTG               hStg,
	INT                  nPageIndex);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETPAGEMETAFILE LlStgsysGetPageMetafile;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSDESTROYMETAFILE)(
	HANDLE               hMF);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSDESTROYMETAFILE LlStgsysDestroyMetafile;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSDRAWPAGE)(
	HLLSTG               hStg,
	HDC                  hDC,
	HDC                  hPrnDC,
	BOOL                 bAskPrinter,
	_PCRECT              pRC,
	INT                  nPageIndex,
	BOOL                 bFit,
	LPVOID               pReserved);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSDRAWPAGE LlStgsysDrawPage;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETLASTERROR)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETLASTERROR LlStgsysGetLastError;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSDELETEFILES)(
	HLLSTG               hStg);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSDELETEFILES LlStgsysDeleteFiles;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysPrintO LlStgsysPrintA
    #else
      #define LlStgsysPrint LlStgsysPrintA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSPRINTA)(
	HLLSTG               hStg,
	LPCSTR               pszPrinterName1,
	LPCSTR               pszPrinterName2,
	INT                  nStartPageIndex,
	INT                  nEndPageIndex,
	INT                  nCopies,
	UINT                 nFlags,
	LPCSTR               pszMessage,
	HWND                 hWndParent);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSPRINTA LlStgsysPrintA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysPrint LlStgsysPrintW
    #else
      #define LlStgsysPrintO LlStgsysPrintW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSPRINTW)(
	HLLSTG               hStg,
	LPCWSTR              pszPrinterName1,
	LPCWSTR              pszPrinterName2,
	INT                  nStartPageIndex,
	INT                  nEndPageIndex,
	INT                  nCopies,
	UINT                 nFlags,
	LPCWSTR              pszMessage,
	HWND                 hWndParent);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSPRINTW LlStgsysPrintW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStoragePrintO LlStgsysStoragePrintA
    #else
      #define LlStgsysStoragePrint LlStgsysStoragePrintA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSTORAGEPRINTA)(
	LPCSTR               lpszFilename,
	LPCSTR               pszTempPath,
	LPCSTR               pszPrinterName1,
	LPCSTR               pszPrinterName2,
	INT                  nStartPageIndex,
	INT                  nEndPageIndex,
	INT                  nCopies,
	UINT                 nFlags,
	LPCSTR               pszMessage,
	HWND                 hWndParent);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGEPRINTA LlStgsysStoragePrintA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStoragePrint LlStgsysStoragePrintW
    #else
      #define LlStgsysStoragePrintO LlStgsysStoragePrintW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSTORAGEPRINTW)(
	LPCWSTR              lpszFilename,
	LPCWSTR              pszTempPath,
	LPCWSTR              pszPrinterName1,
	LPCWSTR              pszPrinterName2,
	INT                  nStartPageIndex,
	INT                  nEndPageIndex,
	INT                  nCopies,
	UINT                 nFlags,
	LPCWSTR              pszMessage,
	HWND                 hWndParent);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGEPRINTW LlStgsysStoragePrintW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetPagePrinterO LlStgsysGetPagePrinterA
    #else
      #define LlStgsysGetPagePrinter LlStgsysGetPagePrinterA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETPAGEPRINTERA)(
	HLLSTG               hStg,
	INT                  nPageIndex,
	LPSTR                pszDeviceName,
	UINT                 nDeviceNameSize,
	PHGLOBAL             phDevMode);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETPAGEPRINTERA LlStgsysGetPagePrinterA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetPagePrinter LlStgsysGetPagePrinterW
    #else
      #define LlStgsysGetPagePrinterO LlStgsysGetPagePrinterW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETPAGEPRINTERW)(
	HLLSTG               hStg,
	INT                  nPageIndex,
	LPWSTR               pszDeviceName,
	UINT                 nDeviceNameSize,
	PHGLOBAL             phDevMode);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETPAGEPRINTERW LlStgsysGetPagePrinterW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLSSETDEBUG)(
	BOOL                 bOn);
#endif /* IMPLEMENTATION */

extern PFNLSSETDEBUG LsSetDebug;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsGetViewerControlClassNameO LsGetViewerControlClassNameA
    #else
      #define LsGetViewerControlClassName LsGetViewerControlClassNameA
  #endif
typedef   LPCSTR ( DLLPROC *PFNLSGETVIEWERCONTROLCLASSNAMEA)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLSGETVIEWERCONTROLCLASSNAMEA LsGetViewerControlClassNameA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsGetViewerControlClassName LsGetViewerControlClassNameW
    #else
      #define LsGetViewerControlClassNameO LsGetViewerControlClassNameW
  #endif
typedef  LPCWSTR ( DLLPROC *PFNLSGETVIEWERCONTROLCLASSNAMEW)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLSGETVIEWERCONTROLCLASSNAMEW LsGetViewerControlClassNameW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     UINT ( DLLPROC *PFNLSGETVIEWERCONTROLDEFAULTMESSAGE)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLSGETVIEWERCONTROLDEFAULTMESSAGE LsGetViewerControlDefaultMessage;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSCREATEVIEWERCONTROLOVERPARENT)(
	HSTG                 hStg,
	HWND                 hParentControl);
#endif /* IMPLEMENTATION */

extern PFNLSCREATEVIEWERCONTROLOVERPARENT LsCreateViewerControlOverParent;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetJobOptionStringExO LlStgsysGetJobOptionStringExA
    #else
      #define LlStgsysGetJobOptionStringEx LlStgsysGetJobOptionStringExA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETJOBOPTIONSTRINGEXA)(
	HLLSTG               hStg,
	LPCSTR               pszKey,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETJOBOPTIONSTRINGEXA LlStgsysGetJobOptionStringExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysGetJobOptionStringEx LlStgsysGetJobOptionStringExW
    #else
      #define LlStgsysGetJobOptionStringExO LlStgsysGetJobOptionStringExW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSGETJOBOPTIONSTRINGEXW)(
	HLLSTG               hStg,
	LPCWSTR              pszKey,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETJOBOPTIONSTRINGEXW LlStgsysGetJobOptionStringExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysSetJobOptionStringExO LlStgsysSetJobOptionStringExA
    #else
      #define LlStgsysSetJobOptionStringEx LlStgsysSetJobOptionStringExA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSETJOBOPTIONSTRINGEXA)(
	HLLSTG               hStg,
	LPCSTR               pszKey,
	LPCSTR               pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSETJOBOPTIONSTRINGEXA LlStgsysSetJobOptionStringExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysSetJobOptionStringEx LlStgsysSetJobOptionStringExW
    #else
      #define LlStgsysSetJobOptionStringExO LlStgsysSetJobOptionStringExW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSETJOBOPTIONSTRINGEXW)(
	HLLSTG               hStg,
	LPCWSTR              pszKey,
	LPCWSTR              pszBuffer);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSETJOBOPTIONSTRINGEXW LlStgsysSetJobOptionStringExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionJobOpenO LsConversionJobOpenA
    #else
      #define LsConversionJobOpen LsConversionJobOpenA
  #endif
typedef HLSCNVJOB ( DLLPROC *PFNLSCONVERSIONJOBOPENA)(
	HWND                 hWndParent,
	INT                  nLanguage,
	LPCSTR               pszFormat);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONJOBOPENA LsConversionJobOpenA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionJobOpen LsConversionJobOpenW
    #else
      #define LsConversionJobOpenO LsConversionJobOpenW
  #endif
typedef HLSCNVJOB ( DLLPROC *PFNLSCONVERSIONJOBOPENW)(
	HWND                 hWndParent,
	INT                  nLanguage,
	LPCWSTR              pszFormat);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONJOBOPENW LsConversionJobOpenW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSCONVERSIONJOBCLOSE)(
	HLSCNVJOB            hCnvJob);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONJOBCLOSE LsConversionJobClose;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSCONVERSIONCONVERTEMFTOSTREAM)(
	HLSCNVJOB            hCnvJob,
	HENHMETAFILE         hEMF,
	PSTREAM              pStream);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONCONVERTEMFTOSTREAM LsConversionConvertEMFToStream;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSCONVERSIONCONVERTSTGTOSTREAM)(
	HLSCNVJOB            hCnvJob,
	HLLSTG               hStg,
	PSTREAM              pStream);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONCONVERTSTGTOSTREAM LsConversionConvertStgToStream;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionPrintO LsConversionPrintA
    #else
      #define LsConversionPrint LsConversionPrintA
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONPRINTA)(
	HLSCNVJOB            hCnvJob,
	PSTREAM              pStream,
	LPSTR                pszBufFilename,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONPRINTA LsConversionPrintA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionPrint LsConversionPrintW
    #else
      #define LsConversionPrintO LsConversionPrintW
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONPRINTW)(
	HLSCNVJOB            hCnvJob,
	PSTREAM              pStream,
	LPWSTR               pszBufFilename,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONPRINTW LsConversionPrintW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSCONVERSIONCONFIGURATIONDLG)(
	HLSCNVJOB            hCnvJob,
	HWND                 hWndParent);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONCONFIGURATIONDLG LsConversionConfigurationDlg;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionSetOptionStringO LsConversionSetOptionStringA
    #else
      #define LsConversionSetOptionString LsConversionSetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONSETOPTIONSTRINGA)(
	HLSCNVJOB            hCnvJob,
	LPCSTR               pszKey,
	LPCSTR               pszData);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONSETOPTIONSTRINGA LsConversionSetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionSetOptionString LsConversionSetOptionStringW
    #else
      #define LsConversionSetOptionStringO LsConversionSetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONSETOPTIONSTRINGW)(
	HLSCNVJOB            hCnvJob,
	LPCWSTR              pszKey,
	LPCWSTR              pszData);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONSETOPTIONSTRINGW LsConversionSetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionGetOptionStringO LsConversionGetOptionStringA
    #else
      #define LsConversionGetOptionString LsConversionGetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONGETOPTIONSTRINGA)(
	HLSCNVJOB            hCnvJob,
	LPCSTR               pszKey,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONGETOPTIONSTRINGA LsConversionGetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionGetOptionString LsConversionGetOptionStringW
    #else
      #define LsConversionGetOptionStringO LsConversionGetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONGETOPTIONSTRINGW)(
	HLSCNVJOB            hCnvJob,
	LPCWSTR              pszKey,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONGETOPTIONSTRINGW LsConversionGetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionConvertEMFToFileO LsConversionConvertEMFToFileA
    #else
      #define LsConversionConvertEMFToFile LsConversionConvertEMFToFileA
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONCONVERTEMFTOFILEA)(
	HLSCNVJOB            hCnvJob,
	HENHMETAFILE         hEMF,
	LPCSTR               pszFilename);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONCONVERTEMFTOFILEA LsConversionConvertEMFToFileA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionConvertEMFToFile LsConversionConvertEMFToFileW
    #else
      #define LsConversionConvertEMFToFileO LsConversionConvertEMFToFileW
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONCONVERTEMFTOFILEW)(
	HLSCNVJOB            hCnvJob,
	HENHMETAFILE         hEMF,
	LPCWSTR              pszFilename);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONCONVERTEMFTOFILEW LsConversionConvertEMFToFileW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionConvertStgToFileO LsConversionConvertStgToFileA
    #else
      #define LsConversionConvertStgToFile LsConversionConvertStgToFileA
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONCONVERTSTGTOFILEA)(
	HLSCNVJOB            hCnvJob,
	HLLSTG               hStg,
	LPCSTR               pszFilename);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONCONVERTSTGTOFILEA LsConversionConvertStgToFileA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsConversionConvertStgToFile LsConversionConvertStgToFileW
    #else
      #define LsConversionConvertStgToFileO LsConversionConvertStgToFileW
  #endif
typedef      INT ( DLLPROC *PFNLSCONVERSIONCONVERTSTGTOFILEW)(
	HLSCNVJOB            hCnvJob,
	HLLSTG               hStg,
	LPCWSTR              pszFilename);
#endif /* IMPLEMENTATION */

extern PFNLSCONVERSIONCONVERTSTGTOFILEW LsConversionConvertStgToFileW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageConvertO LlStgsysStorageConvertA
    #else
      #define LlStgsysStorageConvert LlStgsysStorageConvertA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSTORAGECONVERTA)(
	LPCSTR               pszStgFilename,
	LPCSTR               pszDstFilename,
	LPCSTR               pszFormat);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGECONVERTA LlStgsysStorageConvertA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageConvert LlStgsysStorageConvertW
    #else
      #define LlStgsysStorageConvertO LlStgsysStorageConvertW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSSTORAGECONVERTW)(
	LPCWSTR              pszStgFilename,
	LPCWSTR              pszDstFilename,
	LPCWSTR              pszFormat);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGECONVERTW LlStgsysStorageConvertW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysConvertO LlStgsysConvertA
    #else
      #define LlStgsysConvert LlStgsysConvertA
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSCONVERTA)(
	HLLSTG               hStg,
	LPCSTR               pszDstFilename,
	LPCSTR               pszFormat);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSCONVERTA LlStgsysConvertA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysConvert LlStgsysConvertW
    #else
      #define LlStgsysConvertO LlStgsysConvertW
  #endif
typedef      INT ( DLLPROC *PFNLLSTGSYSCONVERTW)(
	HLLSTG               hStg,
	LPCWSTR              pszDstFilename,
	LPCWSTR              pszFormat);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSCONVERTW LlStgsysConvertW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailConfigurationDialogO LsMailConfigurationDialogA
    #else
      #define LsMailConfigurationDialog LsMailConfigurationDialogA
  #endif
typedef      INT ( DLLPROC *PFNLSMAILCONFIGURATIONDIALOGA)(
	HWND                 hWndParent,
	LPCSTR               pszSubkey,
	UINT                 nFlags,
	INT                  nLanguage);
#endif /* IMPLEMENTATION */

extern PFNLSMAILCONFIGURATIONDIALOGA LsMailConfigurationDialogA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailConfigurationDialog LsMailConfigurationDialogW
    #else
      #define LsMailConfigurationDialogO LsMailConfigurationDialogW
  #endif
typedef      INT ( DLLPROC *PFNLSMAILCONFIGURATIONDIALOGW)(
	HWND                 hWndParent,
	LPCWSTR              pszSubkey,
	UINT                 nFlags,
	INT                  nLanguage);
#endif /* IMPLEMENTATION */

extern PFNLSMAILCONFIGURATIONDIALOGW LsMailConfigurationDialogW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef HLSMAILJOB ( DLLPROC *PFNLSMAILJOBOPEN)(
	INT                  nLanguage);
#endif /* IMPLEMENTATION */

extern PFNLSMAILJOBOPEN LsMailJobOpen;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSMAILJOBCLOSE)(
	HLSMAILJOB           hJob);
#endif /* IMPLEMENTATION */

extern PFNLSMAILJOBCLOSE LsMailJobClose;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailSetOptionStringO LsMailSetOptionStringA
    #else
      #define LsMailSetOptionString LsMailSetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLSMAILSETOPTIONSTRINGA)(
	HLSMAILJOB           hJob,
	LPCSTR               pszKey,
	LPCSTR               pszValue);
#endif /* IMPLEMENTATION */

extern PFNLSMAILSETOPTIONSTRINGA LsMailSetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailSetOptionString LsMailSetOptionStringW
    #else
      #define LsMailSetOptionStringO LsMailSetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLSMAILSETOPTIONSTRINGW)(
	HLSMAILJOB           hJob,
	LPCWSTR              pszKey,
	LPCWSTR              pszValue);
#endif /* IMPLEMENTATION */

extern PFNLSMAILSETOPTIONSTRINGW LsMailSetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailGetOptionStringO LsMailGetOptionStringA
    #else
      #define LsMailGetOptionString LsMailGetOptionStringA
  #endif
typedef      INT ( DLLPROC *PFNLSMAILGETOPTIONSTRINGA)(
	HLSMAILJOB           hJob,
	LPCSTR               pszKey,
	LPSTR                pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLSMAILGETOPTIONSTRINGA LsMailGetOptionStringA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailGetOptionString LsMailGetOptionStringW
    #else
      #define LsMailGetOptionStringO LsMailGetOptionStringW
  #endif
typedef      INT ( DLLPROC *PFNLSMAILGETOPTIONSTRINGW)(
	HLSMAILJOB           hJob,
	LPCWSTR              pszKey,
	LPWSTR               pszBuffer,
	UINT                 nBufSize);
#endif /* IMPLEMENTATION */

extern PFNLSMAILGETOPTIONSTRINGW LsMailGetOptionStringW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSMAILSENDFILE)(
	HLSMAILJOB           hJob,
	HWND                 hWndParent);
#endif /* IMPLEMENTATION */

extern PFNLSMAILSENDFILE LsMailSendFile;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLSDRAWINPUTOBJECT)(
	HWND                 hWnd,
	HDC                  hDC,
	_PRECT               pRC,
	INT                  nType,
	PCSCDIOOPTIONS       pOptions);
#endif /* IMPLEMENTATION */

extern PFNLSDRAWINPUTOBJECT LsDrawInputObject;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSGETLLFILENAME)(
	LPCTSTR              pszFilename,
	LPCTSTR              pszTempPath,
	LPTSTR               pszBuffer,
	UINT                 nSize);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSGETLLFILENAME LlStgsysGetLLFilename;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageCreateO LlStgsysStorageCreateA
    #else
      #define LlStgsysStorageCreate LlStgsysStorageCreateA
  #endif
typedef   HLLSTG ( DLLPROC *PFNLLSTGSYSSTORAGECREATEA)(
	LPCSTR               lpszFilename,
	LPCSTR               pszTempPath,
	HDC                  hRefDC,
	_PCRECT              prcArea,
	BOOL                 bPhysicalPage);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGECREATEA LlStgsysStorageCreateA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageCreate LlStgsysStorageCreateW
    #else
      #define LlStgsysStorageCreateO LlStgsysStorageCreateW
  #endif
typedef   HLLSTG ( DLLPROC *PFNLLSTGSYSSTORAGECREATEW)(
	LPCWSTR              lpszFilename,
	LPCWSTR              pszTempPath,
	HDC                  hRefDC,
	_PCRECT              prcArea,
	BOOL                 bPhysicalPage);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGECREATEW LlStgsysStorageCreateW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLLSTGSYSAPPENDEMF)(
	HLLSTG               hStg,
	HENHMETAFILE         hEMFToAppend,
	HDC                  hRefDC,
	_PCRECT              prcArea,
	BOOL                 bPhysicalPage);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSAPPENDEMF LlStgsysAppendEMF;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsProfileStartO LsProfileStartA
    #else
      #define LsProfileStart LsProfileStartA
  #endif
typedef      INT ( DLLPROC *PFNLSPROFILESTARTA)(
	HANDLE               hThread,
	LPCSTR               pszDescr,
	LPCSTR               pszFilename,
	INT                  nTicksMS);
#endif /* IMPLEMENTATION */

extern PFNLSPROFILESTARTA LsProfileStartA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsProfileStart LsProfileStartW
    #else
      #define LsProfileStartO LsProfileStartW
  #endif
typedef      INT ( DLLPROC *PFNLSPROFILESTARTW)(
	HANDLE               hThread,
	LPCWSTR              pszDescr,
	LPCWSTR              pszFilename,
	INT                  nTicksMS);
#endif /* IMPLEMENTATION */

extern PFNLSPROFILESTARTW LsProfileStartW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLSPROFILEEND)(
	HANDLE               hThread);
#endif /* IMPLEMENTATION */

extern PFNLSPROFILEEND LsProfileEnd;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSGETORGTEXTFROMGLYPHSW)(
	HDC                  hDC,
	_LPCWORD             pszGlyphs,
	INT                  nGlyphs,
	LPWSTR               pszOrgTextBuffer,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLSGETORGTEXTFROMGLYPHSW LsGetOrgTextFromGlyphsW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailViewO LsMailViewA
    #else
      #define LsMailView LsMailViewA
  #endif
typedef      INT ( DLLPROC *PFNLSMAILVIEWA)(
	HWND                 hWndParent,
	LPCSTR               pszMailFile,
	UINT                 nRights,
	INT                  nLanguage);
#endif /* IMPLEMENTATION */

extern PFNLSMAILVIEWA LsMailViewA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LsMailView LsMailViewW
    #else
      #define LsMailViewO LsMailViewW
  #endif
typedef      INT ( DLLPROC *PFNLSMAILVIEWW)(
	HWND                 hWndParent,
	LPCWSTR              pszMailFile,
	UINT                 nRights,
	INT                  nLanguage);
#endif /* IMPLEMENTATION */

extern PFNLSMAILVIEWW LsMailViewW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSGETDDXFORMS)(
	_PPIEnumString       ppEnumString);
#endif /* IMPLEMENTATION */

extern PFNLSGETDDXFORMS LsGetDDXForms;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef   HLLSTG ( DLLPROC *PFNLLSTGSYSSTORAGEOPEN13)(
	_PILLPreviewFileSystem                     pFileSys);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGEOPEN13 LlStgsysStorageOpen13;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef      INT ( DLLPROC *PFNLSINTERNALCREATEVIEWERCONTROLOVERPARENT13)(
	HWND                 hParentControl,
	UINT                 nFlags);
#endif /* IMPLEMENTATION */

extern PFNLSINTERNALCREATEVIEWERCONTROLOVERPARENT13 LsInternalCreateViewerControlOverParent13;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     HWND ( DLLPROC *PFNLSINTERNALGETVIEWERCONTROLFROMPARENT13)(
	HWND                 hParentControl);
#endif /* IMPLEMENTATION */

extern PFNLSINTERNALGETVIEWERCONTROLFROMPARENT13 LsInternalGetViewerControlFromParent13;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     void ( DLLPROC *PFNLSSETDLGBOXMODE)(
	UINT                 nMode);
#endif /* IMPLEMENTATION */

extern PFNLSSETDLGBOXMODE LsSetDlgboxMode;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     UINT ( DLLPROC *PFNLSGETDLGBOXMODE)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLSGETDLGBOXMODE LsGetDlgboxMode;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageOpenExO LlStgsysStorageOpenExA
    #else
      #define LlStgsysStorageOpenEx LlStgsysStorageOpenExA
  #endif
typedef   HLLSTG ( DLLPROC *PFNLLSTGSYSSTORAGEOPENEXA)(
	LPCSTR               lpszFilename,
	PSCSTORAGEOPENINFO   pSOI);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGEOPENEXA LlStgsysStorageOpenExA;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
  #ifdef UNICODE
      #define LlStgsysStorageOpenEx LlStgsysStorageOpenExW
    #else
      #define LlStgsysStorageOpenExO LlStgsysStorageOpenExW
  #endif
typedef   HLLSTG ( DLLPROC *PFNLLSTGSYSSTORAGEOPENEXW)(
	LPCWSTR              lpszFilename,
	PSCSTORAGEOPENINFO   pSOI);
#endif /* IMPLEMENTATION */

extern PFNLLSTGSYSSTORAGEOPENEXW LlStgsysStorageOpenExW;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef  LPCWSTR ( DLLPROC *PFNLSGETVIEWERCONTROLCLASSNAMEEX)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLSGETVIEWERCONTROLCLASSNAMEEX LsGetViewerControlClassNameEx;
/* ------------------------------------------------------------------ */
#ifndef IMPLEMENTATION
typedef     BOOL ( DLLPROC *PFNLSGETDEBUG)(
	void);
#endif /* IMPLEMENTATION */

extern PFNLSGETDEBUG LsGetDebug;

INT LS15xLoad(void);
INT LS15xLoadEx(LPCTSTR pszModuleName);
void LS15xUnload(void);
LPCTSTR LS15xModuleName(void);

#ifdef IMPLEMENTATION
  #if defined(__WATCOMC__)
    #define PROCIMPLS15(p) TEXT(#p)
    #define LOADDLLPROCLS15(fn,pfn,n) \
      fn = (pfn)GetProcAddress(g_hDLLLS15,PROCIMPLS15(fn));
    #define LOADDLLCPROCLS15(fn,pfn,n) \
      fn = (pfn)GetProcAddress(g_hDLLLS15,PROCIMPLS15(fn));
   #else
    #define PROCIMPLS15(p) #p
    #define LOADDLLPROCLS15(fn,pfn,n) \
      *(FARPROC*)&fn = GetProcAddress(g_hDLLLS15,PROCIMPLS15(fn));
    #define LOADDLLCPROCLS15(fn,pfn,n) \
      *(FARPROC*)&fn = GetProcAddress(g_hDLLLS15,PROCIMPLS15(fn));
  #endif

LPCTSTR LS15xModuleName(void)
{
  #if defined(_WIN64)
    return(_T("cxLS15.dll"));
  #else
    return(_T("cmLS15.dll"));
  #endif
}

INT LS15xLoad(void)
{
  return(LS15xLoadEx(LS15xModuleName()));
}

INT LS15xLoadEx(LPCTSTR pszModuleName)
{
  if (g_nDLLLS15Counter++ > 0)
    {
    return(0);
    }

  INT nError = SetErrorMode(SEM_NOOPENFILEERRORBOX);
  g_hDLLLS15 = LoadLibrary(pszModuleName);
  SetErrorMode(nError);

  if (g_hDLLLS15 == NULL)
    {
    g_nDLLLS15Counter = 0;
    return(-1);
    }

  LOADDLLPROCLS15(LlStgsysStorageOpenA,PFNLLSTGSYSSTORAGEOPENA,16);
  LOADDLLPROCLS15(LlStgsysStorageOpenW,PFNLLSTGSYSSTORAGEOPENW,16);
  LOADDLLPROCLS15(LlStgsysStorageClose,PFNLLSTGSYSSTORAGECLOSE,4);
  LOADDLLPROCLS15(LlStgsysGetAPIVersion,PFNLLSTGSYSGETAPIVERSION,4);
  LOADDLLPROCLS15(LlStgsysGetFileVersion,PFNLLSTGSYSGETFILEVERSION,4);
  LOADDLLPROCLS15(LlStgsysGetFilenameA,PFNLLSTGSYSGETFILENAMEA,20);
  LOADDLLPROCLS15(LlStgsysGetFilenameW,PFNLLSTGSYSGETFILENAMEW,20);
  LOADDLLPROCLS15(LlStgsysGetJobCount,PFNLLSTGSYSGETJOBCOUNT,4);
  LOADDLLPROCLS15(LlStgsysSetJob,PFNLLSTGSYSSETJOB,8);
  LOADDLLPROCLS15(LlStgsysGetJob,PFNLLSTGSYSGETJOB,4);
  LOADDLLPROCLS15(LlStgsysGetPageCount,PFNLLSTGSYSGETPAGECOUNT,4);
  LOADDLLPROCLS15(LlStgsysGetJobOptionValue,PFNLLSTGSYSGETJOBOPTIONVALUE,8);
  LOADDLLPROCLS15(LlStgsysGetPageOptionValue,PFNLLSTGSYSGETPAGEOPTIONVALUE,12);
  LOADDLLPROCLS15(LlStgsysGetPageOptionStringA,PFNLLSTGSYSGETPAGEOPTIONSTRINGA,20);
  LOADDLLPROCLS15(LlStgsysGetPageOptionStringW,PFNLLSTGSYSGETPAGEOPTIONSTRINGW,20);
  LOADDLLPROCLS15(LlStgsysSetPageOptionStringA,PFNLLSTGSYSSETPAGEOPTIONSTRINGA,16);
  LOADDLLPROCLS15(LlStgsysSetPageOptionStringW,PFNLLSTGSYSSETPAGEOPTIONSTRINGW,16);
  LOADDLLPROCLS15(LlStgsysAppend,PFNLLSTGSYSAPPEND,8);
  LOADDLLPROCLS15(LlStgsysDeleteJob,PFNLLSTGSYSDELETEJOB,8);
  LOADDLLPROCLS15(LlStgsysDeletePage,PFNLLSTGSYSDELETEPAGE,8);
  LOADDLLPROCLS15(LlStgsysGetPageMetafile,PFNLLSTGSYSGETPAGEMETAFILE,8);
  LOADDLLPROCLS15(LlStgsysDestroyMetafile,PFNLLSTGSYSDESTROYMETAFILE,4);
  LOADDLLPROCLS15(LlStgsysDrawPage,PFNLLSTGSYSDRAWPAGE,32);
  LOADDLLPROCLS15(LlStgsysGetLastError,PFNLLSTGSYSGETLASTERROR,4);
  LOADDLLPROCLS15(LlStgsysDeleteFiles,PFNLLSTGSYSDELETEFILES,4);
  LOADDLLPROCLS15(LlStgsysPrintA,PFNLLSTGSYSPRINTA,36);
  LOADDLLPROCLS15(LlStgsysPrintW,PFNLLSTGSYSPRINTW,36);
  LOADDLLPROCLS15(LlStgsysStoragePrintA,PFNLLSTGSYSSTORAGEPRINTA,40);
  LOADDLLPROCLS15(LlStgsysStoragePrintW,PFNLLSTGSYSSTORAGEPRINTW,40);
  LOADDLLPROCLS15(LlStgsysGetPagePrinterA,PFNLLSTGSYSGETPAGEPRINTERA,20);
  LOADDLLPROCLS15(LlStgsysGetPagePrinterW,PFNLLSTGSYSGETPAGEPRINTERW,20);
  LOADDLLPROCLS15(LsSetDebug,PFNLSSETDEBUG,4);
  LOADDLLPROCLS15(LsGetViewerControlClassNameA,PFNLSGETVIEWERCONTROLCLASSNAMEA,0);
  LOADDLLPROCLS15(LsGetViewerControlClassNameW,PFNLSGETVIEWERCONTROLCLASSNAMEW,0);
  LOADDLLPROCLS15(LsGetViewerControlDefaultMessage,PFNLSGETVIEWERCONTROLDEFAULTMESSAGE,0);
  LOADDLLPROCLS15(LsCreateViewerControlOverParent,PFNLSCREATEVIEWERCONTROLOVERPARENT,8);
  LOADDLLPROCLS15(LlStgsysGetJobOptionStringExA,PFNLLSTGSYSGETJOBOPTIONSTRINGEXA,16);
  LOADDLLPROCLS15(LlStgsysGetJobOptionStringExW,PFNLLSTGSYSGETJOBOPTIONSTRINGEXW,16);
  LOADDLLPROCLS15(LlStgsysSetJobOptionStringExA,PFNLLSTGSYSSETJOBOPTIONSTRINGEXA,12);
  LOADDLLPROCLS15(LlStgsysSetJobOptionStringExW,PFNLLSTGSYSSETJOBOPTIONSTRINGEXW,12);
  LOADDLLPROCLS15(LsConversionJobOpenA,PFNLSCONVERSIONJOBOPENA,12);
  LOADDLLPROCLS15(LsConversionJobOpenW,PFNLSCONVERSIONJOBOPENW,12);
  LOADDLLPROCLS15(LsConversionJobClose,PFNLSCONVERSIONJOBCLOSE,4);
  LOADDLLPROCLS15(LsConversionConvertEMFToStream,PFNLSCONVERSIONCONVERTEMFTOSTREAM,12);
  LOADDLLPROCLS15(LsConversionConvertStgToStream,PFNLSCONVERSIONCONVERTSTGTOSTREAM,12);
  LOADDLLPROCLS15(LsConversionPrintA,PFNLSCONVERSIONPRINTA,16);
  LOADDLLPROCLS15(LsConversionPrintW,PFNLSCONVERSIONPRINTW,16);
  LOADDLLPROCLS15(LsConversionConfigurationDlg,PFNLSCONVERSIONCONFIGURATIONDLG,8);
  LOADDLLPROCLS15(LsConversionSetOptionStringA,PFNLSCONVERSIONSETOPTIONSTRINGA,12);
  LOADDLLPROCLS15(LsConversionSetOptionStringW,PFNLSCONVERSIONSETOPTIONSTRINGW,12);
  LOADDLLPROCLS15(LsConversionGetOptionStringA,PFNLSCONVERSIONGETOPTIONSTRINGA,16);
  LOADDLLPROCLS15(LsConversionGetOptionStringW,PFNLSCONVERSIONGETOPTIONSTRINGW,16);
  LOADDLLPROCLS15(LsConversionConvertEMFToFileA,PFNLSCONVERSIONCONVERTEMFTOFILEA,12);
  LOADDLLPROCLS15(LsConversionConvertEMFToFileW,PFNLSCONVERSIONCONVERTEMFTOFILEW,12);
  LOADDLLPROCLS15(LsConversionConvertStgToFileA,PFNLSCONVERSIONCONVERTSTGTOFILEA,12);
  LOADDLLPROCLS15(LsConversionConvertStgToFileW,PFNLSCONVERSIONCONVERTSTGTOFILEW,12);
  LOADDLLPROCLS15(LlStgsysStorageConvertA,PFNLLSTGSYSSTORAGECONVERTA,12);
  LOADDLLPROCLS15(LlStgsysStorageConvertW,PFNLLSTGSYSSTORAGECONVERTW,12);
  LOADDLLPROCLS15(LlStgsysConvertA,PFNLLSTGSYSCONVERTA,12);
  LOADDLLPROCLS15(LlStgsysConvertW,PFNLLSTGSYSCONVERTW,12);
  LOADDLLPROCLS15(LsMailConfigurationDialogA,PFNLSMAILCONFIGURATIONDIALOGA,16);
  LOADDLLPROCLS15(LsMailConfigurationDialogW,PFNLSMAILCONFIGURATIONDIALOGW,16);
  LOADDLLPROCLS15(LsMailJobOpen,PFNLSMAILJOBOPEN,4);
  LOADDLLPROCLS15(LsMailJobClose,PFNLSMAILJOBCLOSE,4);
  LOADDLLPROCLS15(LsMailSetOptionStringA,PFNLSMAILSETOPTIONSTRINGA,12);
  LOADDLLPROCLS15(LsMailSetOptionStringW,PFNLSMAILSETOPTIONSTRINGW,12);
  LOADDLLPROCLS15(LsMailGetOptionStringA,PFNLSMAILGETOPTIONSTRINGA,16);
  LOADDLLPROCLS15(LsMailGetOptionStringW,PFNLSMAILGETOPTIONSTRINGW,16);
  LOADDLLPROCLS15(LsMailSendFile,PFNLSMAILSENDFILE,8);
  LOADDLLPROCLS15(LsDrawInputObject,PFNLSDRAWINPUTOBJECT,20);
  LOADDLLPROCLS15(LlStgsysGetLLFilename,PFNLLSTGSYSGETLLFILENAME,16);
  LOADDLLPROCLS15(LlStgsysStorageCreateA,PFNLLSTGSYSSTORAGECREATEA,20);
  LOADDLLPROCLS15(LlStgsysStorageCreateW,PFNLLSTGSYSSTORAGECREATEW,20);
  LOADDLLPROCLS15(LlStgsysAppendEMF,PFNLLSTGSYSAPPENDEMF,20);
  LOADDLLPROCLS15(LsProfileStartA,PFNLSPROFILESTARTA,16);
  LOADDLLPROCLS15(LsProfileStartW,PFNLSPROFILESTARTW,16);
  LOADDLLPROCLS15(LsProfileEnd,PFNLSPROFILEEND,4);
  LOADDLLPROCLS15(LsGetOrgTextFromGlyphsW,PFNLSGETORGTEXTFROMGLYPHSW,20);
  LOADDLLPROCLS15(LsMailViewA,PFNLSMAILVIEWA,16);
  LOADDLLPROCLS15(LsMailViewW,PFNLSMAILVIEWW,16);
  LOADDLLPROCLS15(LsGetDDXForms,PFNLSGETDDXFORMS,4);
  LOADDLLPROCLS15(LlStgsysStorageOpen13,PFNLLSTGSYSSTORAGEOPEN13,4);
  LOADDLLPROCLS15(LsInternalCreateViewerControlOverParent13,PFNLSINTERNALCREATEVIEWERCONTROLOVERPARENT13,8);
  LOADDLLPROCLS15(LsInternalGetViewerControlFromParent13,PFNLSINTERNALGETVIEWERCONTROLFROMPARENT13,4);
  LOADDLLPROCLS15(LsSetDlgboxMode,PFNLSSETDLGBOXMODE,4);
  LOADDLLPROCLS15(LsGetDlgboxMode,PFNLSGETDLGBOXMODE,0);
  LOADDLLPROCLS15(LlStgsysStorageOpenExA,PFNLLSTGSYSSTORAGEOPENEXA,8);
  LOADDLLPROCLS15(LlStgsysStorageOpenExW,PFNLLSTGSYSSTORAGEOPENEXW,8);
  LOADDLLPROCLS15(LsGetViewerControlClassNameEx,PFNLSGETVIEWERCONTROLCLASSNAMEEX,0);
  LOADDLLPROCLS15(LsGetDebug,PFNLSGETDEBUG,0);
  return(0);
}

void LS15xUnload(void)
{
  if (--g_nDLLLS15Counter > 0)
    return;

  if (g_hDLLLS15 != NULL)
    {
    FreeLibrary(g_hDLLLS15);
    g_hDLLLS15 = NULL;
    LlStgsysStorageOpenA = NULL;
    LlStgsysStorageOpenW = NULL;
    LlStgsysStorageClose = NULL;
    LlStgsysGetAPIVersion = NULL;
    LlStgsysGetFileVersion = NULL;
    LlStgsysGetFilenameA = NULL;
    LlStgsysGetFilenameW = NULL;
    LlStgsysGetJobCount = NULL;
    LlStgsysSetJob = NULL;
    LlStgsysGetJob = NULL;
    LlStgsysGetPageCount = NULL;
    LlStgsysGetJobOptionValue = NULL;
    LlStgsysGetPageOptionValue = NULL;
    LlStgsysGetPageOptionStringA = NULL;
    LlStgsysGetPageOptionStringW = NULL;
    LlStgsysSetPageOptionStringA = NULL;
    LlStgsysSetPageOptionStringW = NULL;
    LlStgsysAppend = NULL;
    LlStgsysDeleteJob = NULL;
    LlStgsysDeletePage = NULL;
    LlStgsysGetPageMetafile = NULL;
    LlStgsysDestroyMetafile = NULL;
    LlStgsysDrawPage = NULL;
    LlStgsysGetLastError = NULL;
    LlStgsysDeleteFiles = NULL;
    LlStgsysPrintA = NULL;
    LlStgsysPrintW = NULL;
    LlStgsysStoragePrintA = NULL;
    LlStgsysStoragePrintW = NULL;
    LlStgsysGetPagePrinterA = NULL;
    LlStgsysGetPagePrinterW = NULL;
    LsSetDebug = NULL;
    LsGetViewerControlClassNameA = NULL;
    LsGetViewerControlClassNameW = NULL;
    LsGetViewerControlDefaultMessage = NULL;
    LsCreateViewerControlOverParent = NULL;
    LlStgsysGetJobOptionStringExA = NULL;
    LlStgsysGetJobOptionStringExW = NULL;
    LlStgsysSetJobOptionStringExA = NULL;
    LlStgsysSetJobOptionStringExW = NULL;
    LsConversionJobOpenA = NULL;
    LsConversionJobOpenW = NULL;
    LsConversionJobClose = NULL;
    LsConversionConvertEMFToStream = NULL;
    LsConversionConvertStgToStream = NULL;
    LsConversionPrintA = NULL;
    LsConversionPrintW = NULL;
    LsConversionConfigurationDlg = NULL;
    LsConversionSetOptionStringA = NULL;
    LsConversionSetOptionStringW = NULL;
    LsConversionGetOptionStringA = NULL;
    LsConversionGetOptionStringW = NULL;
    LsConversionConvertEMFToFileA = NULL;
    LsConversionConvertEMFToFileW = NULL;
    LsConversionConvertStgToFileA = NULL;
    LsConversionConvertStgToFileW = NULL;
    LlStgsysStorageConvertA = NULL;
    LlStgsysStorageConvertW = NULL;
    LlStgsysConvertA = NULL;
    LlStgsysConvertW = NULL;
    LsMailConfigurationDialogA = NULL;
    LsMailConfigurationDialogW = NULL;
    LsMailJobOpen = NULL;
    LsMailJobClose = NULL;
    LsMailSetOptionStringA = NULL;
    LsMailSetOptionStringW = NULL;
    LsMailGetOptionStringA = NULL;
    LsMailGetOptionStringW = NULL;
    LsMailSendFile = NULL;
    LsDrawInputObject = NULL;
    LlStgsysGetLLFilename = NULL;
    LlStgsysStorageCreateA = NULL;
    LlStgsysStorageCreateW = NULL;
    LlStgsysAppendEMF = NULL;
    LsProfileStartA = NULL;
    LsProfileStartW = NULL;
    LsProfileEnd = NULL;
    LsGetOrgTextFromGlyphsW = NULL;
    LsMailViewA = NULL;
    LsMailViewW = NULL;
    LsGetDDXForms = NULL;
    LlStgsysStorageOpen13 = NULL;
    LsInternalCreateViewerControlOverParent13 = NULL;
    LsInternalGetViewerControlFromParent13 = NULL;
    LsSetDlgboxMode = NULL;
    LsGetDlgboxMode = NULL;
    LlStgsysStorageOpenExA = NULL;
    LlStgsysStorageOpenExW = NULL;
    LsGetViewerControlClassNameEx = NULL;
    LsGetDebug = NULL;
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

#endif  /* #ifndef _LS15_H */

