
#include "Ini.h"

// ==========================
// item state values.********
// ==========================
#define ITEM_STATE_NORMAL   0
#define ITEM_STATE_HOT      1
#define ITEM_STATE_PRESSED  2
#define ITEM_STATE_DISABLED 3
#define ITEM_STATE_DEFAULT  4

// Top Header Item State values. ======================================
#define THI_STATE_NOFOCUS_NORMAL      0
#define THI_STATE_FOCUS_NORMAL        THI_STATE_NOFOCUS_NORMAL      + 1
#define THI_STATE_PRESSED_NORMAL      THI_STATE_FOCUS_NORMAL        + 1
#define THI_STATE_NOFOCUS_SELECT_ASC  THI_STATE_PRESSED_NORMAL      + 1
#define THI_STATE_FOCUS_SELECT_ASC    THI_STATE_NOFOCUS_SELECT_ASC  + 1
#define THI_STATE_PRESSED_SELECT_ASC  THI_STATE_FOCUS_SELECT_ASC    + 1
#define THI_STATE_NOFOCUS_SELECT_DESC THI_STATE_PRESSED_SELECT_ASC  + 1
#define THI_STATE_FOCUS_SELECT_DESC   THI_STATE_NOFOCUS_SELECT_DESC + 1
#define THI_STATE_PRESSED_SELECT_DESC THI_STATE_FOCUS_SELECT_DESC   + 1

typedef struct DrawScroll_tag
{
	UINT   nPage;			// [in]
	UINT   nTrackPos;		// [in]
	UINT   nTrackPosMax;	// [out]
	int    nMin;			// [in]
	int    nMax;			// [in]
	CRect  rcThumbBtn;		// [out]
	CRect  rcScroll;		// [in]
	CRect  rcInvalidate;	// [in]
} DrawScroll, *PDrawScroll;


class iSkinManager : public CObject
{
	friend class iScrollCtrl;
	friend class iButton;

// Construction
public:
	iSkinManager ();
	~iSkinManager();

	BOOL LoadSkin			(CString strSkinPath);
	void LoadSkinDefault	();
	void FreeSkinRes		();


	static iSkinManager* SkinManager();

	// **********************************************************************
	//							Static Functions.						   //
	// **********************************************************************
	static HBITMAP CreateBitmap32bpp	 (UINT* pColorTable, int nWidth, int nHeight, BITMAP* pBmp = NULL);          
	static BOOL    RegionFromAlphaData   (HBITMAP hBmpAlpha, CRgn& rgnMask);
	static BOOL    SaveBitmap32bppFile   (LPCTSTR lpszFile, HBITMAP hBmp);
	static BOOL    ConvertInBitmap32bpp  (HBITMAP& hBmp);
	static void    SaveMaskDataInBasicImg(HBITMAP hbmpBasic, HBITMAP hbmpMask);
	static HBITMAP GetBitmap32bppFromDC  (CDC* pSrc, int xSrc, int ySrc, int nWidth, int nHeight);
	static HBITMAP Bitmap32bppFromBitmap (HBITMAP hBmpSrc, int xSrc, int ySrc, int nWidth, int nHeight);
	static BOOL	   BitmapOnBitmap		 (HBITMAP hBmpDest, int xDest, int yDest, HBITMAP hBmpSrc,
										  int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
										  int nDrawWidth, int nDrawHeight, BOOL bAlpha = FALSE);
	static BOOL	   BitmapOnBitmap		 (BITMAP* pbmpDest, int xDest, int yDest, BITMAP* pbmpSrc, 
										  int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, 
										  int nDrawWidth, int nDrawHeight, BOOL bAlpha = FALSE);
	// **********************************************************************

public:
	// scroll drawing functions. ===========================================
	BOOL DrawHScrollOnBitmap(CDC* pDC, HBITMAP hbmpBK, UINT nLeftArrowState, 
							 UINT nRightArrowState, DrawScroll& draw);
	void DrawThumbHorzOnDib (CDC* pDC, BITMAP& bmpBK, DrawScroll& draw);
	BOOL DrawVScrollOnBitmap(CDC* pDC, HBITMAP hbmpBK, UINT nTopArrowState, 
							 UINT nBottomArrowState, DrawScroll& draw);
	void DrawThumbVertOnDib (CDC* pDC, BITMAP& bmpBK, DrawScroll& draw);
	// ======================================================================
	// top header drawing functions. ========================================
	BOOL DrawTopHeaderItem  (CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState, 
							 UINT nItemWidth, LPCTSTR lpszText, int nMargin, int nAlign,
							 BOOL bSortedSign = TRUE);
	// ======================================================================
	// button drawing functions. ============================================
	BOOL DrawPushButton	    (CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState,
							 LPCTSTR lpszText, int nMargin, int nAlign);
	BOOL DrawCheckButton	(CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState,
							 LPCTSTR lpszText, int nMargin, int nAlign);
	BOOL DrawRadioButton	(CDC* pDC, HBITMAP hbmpBK, int x, int y, UINT nItemState,
							 LPCTSTR lpszText, int nMargin, int nAlign);
	// ======================================================================

public:
	UINT	m_nCX_VScroll;			// Width of horizontal scroll bar.
	UINT	m_nCY_HScroll;		    // Height of horizontal scroll bar.
	int     m_nCY_TopHeader;		// height of top header.

	UINT	m_nPushBtnDrawOp;		// draw option of push-button's image.
	UINT	m_nPushBtnArcLeft;
	UINT	m_nPushBtnArcRight; 
	UINT	m_nPushBtnArcTop; 
	UINT	m_nPushBtnArcBottom;
	UINT	m_nPushBtnTransClr;		// transparent color.

	UINT	m_nCheckBtnDrawOp;		// draw option of Check-button's image.
	UINT	m_nCheckBtnArcLeft;
	UINT	m_nCheckBtnArcRight; 
	UINT	m_nCheckBtnArcTop; 
	UINT	m_nCheckBtnArcBottom;
	UINT	m_nCheckBtnTransClr;	// transparent color.

	UINT	m_nRadioBtnDrawOp;		// draw option of Radio-button's image.
	UINT	m_nRadioBtnArcLeft;
	UINT	m_nRadioBtnArcRight; 
	UINT	m_nRadioBtnArcTop; 
	UINT	m_nRadioBtnArcBottom;
	UINT	m_nRadioBtnTransClr;	// transparent color.

	CFont   m_btnFont;				// button's font object.

public:
	// ========================================================
	// Horizontal Scroll Resources. ***************************
	// ========================================================
	HBITMAP m_hArrowLeftH[4];			// Left arrow images.
	HBITMAP m_hThumbLeftH[4];			// Left part of thumb button images.
	HBITMAP m_hThumbCenterH[4];			// Center part of thumb button images.
	HBITMAP m_hThumbRightH[4];			// Right part of thumb button images.
	HBITMAP m_hArrowRightH[4];			// Right arrow images.
	HBITMAP m_hBackgroundH[4];			// Background images.
	BITMAP  m_bmpArrowLeftH[4];			// BITMAP structures of Left arrow images.
	BITMAP  m_bmpThumbLeftH[4];			// BITMAP structures of Left part of thumb button images.
	BITMAP  m_bmpThumbCenterH[4];		// BITMAP structures of Center part of thumb button images.
	BITMAP  m_bmpThumbRightH[4];		// BITMAP structures of Right part of thumb button images.
	BITMAP  m_bmpArrowRightH[4];		// BITMAP structures of Right arrow images.
	BITMAP  m_bmpBackgroundH[4];		// BITMAP structures of Background images.
	CRgn    m_rgnArrowLeft;				// region of arrow left button.
	CRgn    m_rgnArrowRight;			// region of arrow right button.
	UINT    m_nCY_ThumbMin;				// minimal height of thumb button.
	UINT    m_nCX_HArrowMin;			// minimal Width of the arrow button in pixels.
	UINT    m_nCX_HArrowMax;			// maximum Width of the arrow button in pixels.
	// ********************************************************

	// ========================================================
	// Vertical Scroll Resources. *****************************
	// ========================================================
	HBITMAP m_hArrowTopV[4];			// Top arrow images.
	HBITMAP m_hThumbTopV[4];			// Top part of thumb button images.
	HBITMAP m_hThumbCenterV[4];			// Center part of thumb button images.
	HBITMAP m_hThumbBottomV[4];			// Bottom part of thumb button images.
	HBITMAP m_hArrowBottomV[4];			// Bottom arrow images.
	HBITMAP m_hBackgroundV[4];			// Background images.
	BITMAP  m_bmpArrowTopV[4];			// BITMAP structures of Top arrow images.
	BITMAP  m_bmpThumbTopV[4];			// BITMAP structures of Top part of thumb button images.
	BITMAP  m_bmpThumbCenterV[4];		// BITMAP structures of Center part of thumb button images.
	BITMAP  m_bmpThumbBottomV[4];		// BITMAP structures of Bottom part of thumb button images.
	BITMAP  m_bmpArrowBottomV[4];		// BITMAP structures of Bottom arrow images.
	BITMAP  m_bmpBackgroundV[4];		// BITMAP structures of Background images.
	CRgn    m_rgnArrowTop;				// region of arrow top button.
	CRgn    m_rgnArrowBottom;			// region of arrow bottom button.
	UINT    m_nCX_ThumbMin;				// minimal thumb width for horz scrolls.
	UINT    m_nCY_VArrowMin;			// minimal Height of the arrow button in pixels.
	UINT    m_nCY_VArrowMax;			// minimal Height of the arrow button in pixels.
	// ********************************************************
	
	// ========================================================
	// Top Header Resources. **********************************
	// ========================================================
	HBITMAP m_hTH_NoFocusNormal;		// image of Not focused item in normal state.
	HBITMAP m_hTH_FocusNormal;			// image of focused item in normal state.
	HBITMAP m_hTH_PressedNormal;		// image of pressed item in normal state.
	HBITMAP m_hTH_NoFocusSelectAsc;		// image of Not focused item in selected state and ascending order.
	HBITMAP m_hTH_FocusSelectAsc;		// image of focused item in selected state and ascending order.
	HBITMAP m_hTH_PressedSelectAsc;		// image of pressed item in selected state and ascending order.
	HBITMAP m_hTH_NoFocusSelectDesc;	// image of Not focused item in selected state and descending order.
	HBITMAP m_hTH_FocusSelectDesc;		// image of focused item in selected state and descending order.
	HBITMAP m_hTH_PressedSelectDesc;	// image of pressed item in selected state and descending order.
	BITMAP  m_bmpTH_NoFocusNormal;		// BITMAP of image of Not focused item in normal state.
	BITMAP  m_bmpTH_FocusNormal;		// BITMAP of image of focused item in normal state.
	BITMAP  m_bmpTH_PressedNormal;		// BITMAP of image of pressed item in normal state.
	BITMAP  m_bmpTH_NoFocusSelectAsc;	// BITMAP of image of Not focused item in selected state and ascending order.
	BITMAP  m_bmpTH_FocusSelectAsc;		// BITMAP of image of focused item in selected state and ascending order.
	BITMAP  m_bmpTH_PressedSelectAsc;	// BITMAP of image of pressed item in selected state and ascending order.
	BITMAP  m_bmpTH_NoFocusSelectDesc;	// BITMAP of image of Not focused item in selected state and descending order.
	BITMAP  m_bmpTH_FocusSelectDesc;	// BITMAP of image of focused item in selected state and descending order.
	BITMAP  m_bmpTH_PressedSelectDesc;	// BITMAP of image of pressed item in selected state and descending order.
	// ********************************************************
	
	// ========================================================
	// Push Button Resources. *********************************
	// ========================================================
	// don't change order of members. !!! <Zqr Specific>
	HBITMAP m_hPushBtnNormal;			// Image for normal button.
	HBITMAP m_hPushBtnHot;				// Image for hovered button.
	HBITMAP m_hPushBtnPressed;			// Image for pressed button.
	HBITMAP m_hPushBtnDisabled;			// Image for disabled button.
	HBITMAP m_hPushBtnDefault;			// Image for default button.
	BITMAP  m_bmpPushBtnNormal;			// BITMAP of normal button image.
	BITMAP  m_bmpPushBtnHot;			// BITMAP of hovered button image.
	BITMAP  m_bmpPushBtnPressed;		// BITMAP of pressed button image.
	BITMAP  m_bmpPushBtnDisabled;		// BITMAP of desabled button image.
	BITMAP  m_bmpPushBtnDefault;		// BITMAP of default button image.
	// ********************************************************

	// ========================================================
	// Check Box Resources. ***********************************
	// ========================================================
	// don't change order of members. !!! <Zqr Specific>
	HBITMAP m_hCheckBtnNormal0;			// image of normal item in unchecked state.
	HBITMAP m_hCheckBtnHot0;			// image of normal item in unchecked state.
	HBITMAP m_hCheckBtnPressed0;		// image of normal item in unchecked state.
	HBITMAP m_hCheckBtnDisabled0;		// image of normal item in unchecked state.
	HBITMAP m_hCheckBtnNormal1;			// image of normal item in checked state.
	HBITMAP m_hCheckBtnHot1;			// image of normal item in checked state.
	HBITMAP m_hCheckBtnPressed1;		// image of normal item in checked state.
	HBITMAP m_hCheckBtnDisabled1;		// image of normal item in checked state.
	HBITMAP m_hCheckBtnNormal2;			// image of normal item in indetermine state.
	HBITMAP m_hCheckBtnHot2;			// image of normal item in indetermine state.
	HBITMAP m_hCheckBtnPressed2;		// image of normal item in indetermine state.
	HBITMAP m_hCheckBtnDisabled2;		// image of normal item in indetermine state.
	BITMAP  m_bmpCheckBtnNormal0;		// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpCheckBtnHot0;			// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpCheckBtnPressed0;		// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpCheckBtnDisabled0;		// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpCheckBtnNormal1;		// BITMAP of image of normal item in checked state.
	BITMAP  m_bmpCheckBtnHot1;			// BITMAP of image of normal item in checked state.
	BITMAP  m_bmpCheckBtnPressed1;		// BITMAP of image of normal item in checked state.
	BITMAP  m_bmpCheckBtnDisabled1;		// BITMAP of image of normal item in checked state.
	BITMAP  m_bmpCheckBtnNormal2;		// BITMAP of image of normal item in indetermine state.
	BITMAP  m_bmpCheckBtnHot2;			// BITMAP of image of normal item in indetermine state.
	BITMAP  m_bmpCheckBtnPressed2;		// BITMAP of image of normal item in indetermine state.
	BITMAP  m_bmpCheckBtnDisabled2;		// BITMAP of image of normal item in indetermine state.
	// ********************************************************

	// ========================================================
	// Radio Button Resources. ********************************
	// ========================================================
	// don't change order of members. !!! <Zqr Specific>
	HBITMAP m_hRadioBtnNormal0;			// image of normal item in unchecked state.
	HBITMAP m_hRadioBtnHot0;			// image of normal item in unchecked state.
	HBITMAP m_hRadioBtnPressed0;		// image of normal item in unchecked state.
	HBITMAP m_hRadioBtnDisabled0;		// image of normal item in unchecked state.
	HBITMAP m_hRadioBtnNormal1;			// image of normal item in checked state.
	HBITMAP m_hRadioBtnHot1;			// image of normal item in checked state.
	HBITMAP m_hRadioBtnPressed1;		// image of normal item in checked state.
	HBITMAP m_hRadioBtnDisabled1;		// image of normal item in checked state.
	BITMAP  m_bmpRadioBtnNormal0;		// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpRadioBtnHot0;			// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpRadioBtnPressed0;		// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpRadioBtnDisabled0;		// BITMAP of image of normal item in unchecked state.
	BITMAP  m_bmpRadioBtnNormal1;		// BITMAP of image of normal item in checked state.
	BITMAP  m_bmpRadioBtnHot1;			// BITMAP of image of normal item in checked state.
	BITMAP  m_bmpRadioBtnPressed1;		// BITMAP of image of normal item in checked state.
	BITMAP  m_bmpRadioBtnDisabled1;		// BITMAP of image of normal item in checked state.
	// ********************************************************
	
protected:
	Ini				m_ini;
	CString			m_strSkinPath;
};

/////////////////////////////////////////////////////////////////////////////
