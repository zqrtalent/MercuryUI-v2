#pragma once
#include "BitmapEx.h"

#define ITEM_STATE_NORMAL   0
#define ITEM_STATE_HOT      1
#define ITEM_STATE_PRESSED  2
#define ITEM_STATE_DISABLED 3
#define ITEM_STATE_DEFAULT  4

#pragma pack(1)
struct JumpOperation
	{
	BYTE bJmpCode;
	long lJmpOffset;
	};

//
//	class CGridCtrlTheme
//

class CGridCtrlTheme : public CObject
{
public:
	CGridCtrlTheme	();
	~CGridCtrlTheme	();

	// < Public virtual methods. >
public:
	virtual void LoadTheme();

	// < Protected methods. >
protected:
	void FreeThemeResources();
	
	// < Public attributes. >
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
	UINT	m_nCY_HScroll;				// Height of horizontal scroll bar.
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
	UINT	m_nCX_VScroll;				// Width of vertical scroll bar.
	// ********************************************************

	UINT	m_crSizeGripper;			// Size gripper color;
	UINT	m_crGridBorder;				// Color of grid border.
};