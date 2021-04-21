#pragma once

/////////////////////////////////////////////////////////////////////////////
// MercuryBaseWnd window

#include "../_platformCompat/PlatformCompat.h"
#include "../_platformCompat/PlatformDeviceContext.h"
#include "../_platformCompat/PlatformImage.h"
#include "../_platformCompat/PlatformRect.h"
#include "../_platformCompat/PlatformSize.h"
#include "../_platformCompat/PlatformPoint.h"
#include "../_platformCompat/PlatformDateTime.h"
#include "../_platformCompat/PlatformLongBinary.h"
#include "../_platformCompat/PlatformCursor.h"
#include "../_platformCompat/PlatformUIMenu.h"

#include "MercuryGUI.h"
#include "ESFrameBase.h"
#include "../Array/AutoSortedArrayTempl.h"
#include "../Array/AutoSortedArray.h"
#include "../Array/AutoSortedArrayInt64.h"
#include "../Array/StringToIntArray.h"
#include "../Array/List.h"
#include "../Data/VRowSet.h"
//#include "../PPToolTip/PPToolTip.h"

#include "ESChildControlInfo.h"
#include "MercuryBaseViewInfo.h"
#include "ESLocalizationMan.h"
#include "MercuryLayerMan.h"

#define CHILD_ControlIdUnused	10000

#define MAKE_RECT(l,t,r,b)\
	(((__int64)l) << 48) | (((__int64)t) << 32) | (((__int64)r) << 16) | ((__int64)b)

///////////////////////////////////////////////////////////////////////
//
//	Fill mode defines.
//
enum _FillMode{
	FillMode_None = 0,
	FillMode_Color,
	FillMode_CentredImage,
	FillMode_StretchImage,
	FillMode_RepeatedImage,
	FillMode_OmitBorderedImage
	};

///////////////////////////////////////////////////////////////////////
//
//	Alignement defines.
//
enum VAlignment{
	VAlignement_Top = 0,
	VAlignement_Center,
	VAlignement_Bottom
	};

enum HAlignment{
	HAlignement_Left = 0,
	HAlignement_Middle,
	HAlignement_Right
	};

#define DESIGN_FILE_SIGNATURE 0x53445345 // ESDS


///////////////////////////////////////////////////////////////////////
//
//	Child control defines.
//

enum ChildControlEventCode{
	EventCode_VisibilityChanged = 100,
	EventCode_ClientRectChanged,
	EventCode_OnCancel,
	EventCode_OnOK,
	EventCode_ImageButtonClick,
	EventCode_ImageClick,
	EventCode_EditTextChange,
	EventCode_TabItemChange,
	EventCode_ListBoxItemClick,
	EventCode_ControlClick,
	EventCode_ControlMouseEnter,
	EventCode_ControlMouseLeave,
	EventCode_OnContextMenu,

	// Combobox
	EventCode_ComboBoxCloseUp,
	EventCode_ComboBoxDropDown,
	EventCode_ComboBoxSelChange,

	// LookupBox
	EventCode_Lookup,

	// List
	EventCode_ListRowChange,
	EventCode_ListRowDblClk,
	EventCode_ListTopRowChanged,

	// TextBox
	EventCode_OnClearTextBox,

	// CheckBox
	EventCode_CheckBoxClick,

	// PropertyGrid
	EventCode_PropertyItemSelChange,

	// Slider
	EventCode_SliderPosChanged,

	// Motion events.
	EventCode_MotionStart,
	EventCode_MotionStop,
	EventCode_MotionEnd
	};

#define CHILD_CONTROL_EVENT_CODE_CUSTOM 1000

class ESChildControl;
typedef void (__thiscall ESFrameBase::*ControlEventListener)(ESChildControl* pSender, ChildControlEventCode eventCode, void* lpParam);
typedef void (__thiscall MercuryBaseView::*CommandProc)();

struct ChildEventListenerInfo{
	ControlEventListener	method;
	ESChildControl*			pSender;
	ESFrameBase*			pReceiver;
	};

class ESChildControlAttribute : public Serializable
{
public:
	ESChildControlAttribute();
	virtual ~ESChildControlAttribute();

public:
	_string         m_sClassName;		// Child control's class name.
	_string         m_sName;			// Child control's name.
	int				m_nId;				// Control id.
	bool			m_bVisible;			// Contol's visibility.

	ESChildControl*	m_pControl;			// Child control.

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlAttribute();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESDesignFileInfo : public Serializable
{
public:
	ESDesignFileInfo();
	virtual ~ESDesignFileInfo();

public:
	int				m_nVersionMajor;
	int				m_nVersionMinor;
	DateTime		m_dtCreationDateTime;
	_string		m_sAuthor;
	_string		m_sDescription;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESDesignFileInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESChildControl;
class ESSerializableModifyDlg;

class MercuryBaseView : public ESFrameBase
{
public:
	MercuryBaseView();
	virtual ~MercuryBaseView();

	friend ESChildControl;
	friend MercuryBaseViewInfo;

	enum MenuItemIds
	{
	   MI_LABEL = 65100,
	   MI_IMAGE,
	   MI_BUTTON,
	   MI_IMAGEBUTTON,
	   MI_TEXTBOX,
	   MI_CHECKBOX,
	   MI_COMBOBOX,
	   MI_SLIDER,
	   MI_PROGRESSCTRL,
	   MI_AVATAR,
	   MI_SIMPLERENDERVIEW,
	   MI_LIST,
	   MI_TREE,
	   MI_TAB,
	   MI_TABVIEW,
	   MI_TREEVIEW,
	   MI_BANNER,
	   MI_PROPERTYGRID,

	   MI_ALIGN_LEFTS,
	   MI_ALIGN_TOPS,
	   MI_ALIGN_RIGHTS,
	   MI_ALIGN_BOTTOMS,
	   MI_MAKE_SAME_SIZE_WIDTH,
	   MI_MAKE_SAME_SIZE_HEIGHT,
	   MI_MAKE_SAME_SIZE_BOTH,
	   MI_LOCK,
	   MI_UNLOCK,
	   MI_COPY,
	   MI_CUT,
	   MI_PASTE,
	   MI_LIST_OF_CONTROLS,
	   MI_OPEN_DESIGNER_FILE,
	   MI_SAVE_DESIGNER_FILE,
	   MI_HIDE_CONTROL,
	   MI_PROPERTIES,
	   MI_BASEVIEW_PROPERTIES,

	   MI_LABEL_AS_CHILD,
	   MI_IMAGE_AS_CHILD,
	   MI_IMAGEBUTTON_AS_CHILD,
	   MI_TEXTBOX_AS_CHILD,
	   MI_CHECKBOX_AS_CHILD,
	   MI_COMBOBOX_AS_CHILD,
	   MI_SLIDER_AS_CHILD,
	   MI_PROGRESSCTRL_AS_CHILD,
	   MI_AVATAR_AS_CHILD,
	   MI_SIMPLERENDERVIEW_AS_CHILD,
	   MI_LIST_AS_CHILD,
	   MI_TREE_AS_CHILD,
	   MI_TAB_AS_CHILD,
	   MI_BANNER_AS_CHILD,
	   MI_TABVIEW_AS_CHILD,
	   MI_TREEVIEW_AS_CHILD,
	   MI_PROPERTYGRID_AS_CHILD
	};

public:
	void					RedrawRect					(_Rect rcRedraw, BOOL bRedrawAtOnce = FALSE);
	void					RedrawMultipleRects			(_Rect* pRectsRedraw, int nCt, BOOL bRedrawAtOnce = FALSE);
	bool					DoubleBufferedDC			()										{return  (UseDoubleBufferedDC()||m_bUseForegroundColor);};
	void					UseForegroundColor			(BOOL bUse, bool bInvalidate = false)
    {
        m_bUseForegroundColor = bUse; if(bInvalidate && IsWindow()){ Invalidate(); /*::InvalidateRect(m_hWnd, NULL, 0);*/}
    };
	void					SetForegroundColor			(COLORREF crForeground)					{m_crForeground = crForeground;};
	void					SetForegroundColorAlpha		(int nAlpha)							{m_nAlpha = (nAlpha%255);};

	bool					AddEventListener			(ESFrameBase* pReceiver, int nControlId, ChildControlEventCode eventCode, ControlEventListener method);
	bool					AddEventListener			(ESFrameBase* pReceiver, ESChildControl* pControl, ChildControlEventCode eventCode, ControlEventListener method);
	bool					AddEventListenerByName		(ESFrameBase* pReceiver, const TCHAR* pszName, ChildControlEventCode eventCode, ControlEventListener method);

	bool					SetControlVisibility		(int nControlId, bool bVisible, bool bRedraw);
	bool					SetControlVisibilityByName	(const TCHAR* pszName, bool bVisible, bool bRedraw);
	bool					SetupCheckBoxByName			(const TCHAR* pszName, bool bVisible, bool bCheckState,  bool bRedraw);
	bool					GetCheckBoxStateByName		(const TCHAR* pszName);

	void					DisableImageButton			(int nButtonId, bool bEnable = false, bool bRedraw = false);
	void					DisableImageButtonByName	(TCHAR* pszName, bool bEnable = false, bool bRedraw = false);

	ESChildControl*			GetChildControl				(int nId);
	ESChildControl*			GetChildControlByName		(const TCHAR* pszName);
	int						GetChildControlIdByName		(const TCHAR* pszName);

	// Child control methods.
	bool					AddChildControl				(ESChildControl* pControl, int nId, _Rect rcControl, ESChildControl* pParent, int nZIndex = -1, _string sControlName = _T(""));
	bool					AddChildControlEx			(ESChildControl* pChild, int nId, _Rect rcControl, ESChildControl* pParent, _Size szDefWindowSize, _string sControlName = _T(""), int nZIndex = -1);
	bool					IsValidChildControl			(ESChildControl* pControl){ return (m_listChildControls.IndexOf(pControl) != -1); };

	bool					RemoveChildControl			(ESChildControl* pControl, bool bRemoveAllSiblings = false);

	bool					RedrawChildControl			(int nId);
	bool					RedrawChildControlByName	(const TCHAR* pszName);
	bool					RemoveChildControlByName	(const TCHAR* pszName);

	bool					DrawBackground				(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient, _Rect rcControl, int nMode, COLORREF crFill, 
														_Image* pImageBg, _Rect rcOmitBorder, bool bSizeBox = false, int nScaleMode = 1);

	// Invalid rect area caching. Note: this mechanism is usefull to prevent invalidate same areas multiple time. {{
	bool					StartCachingInvalidRects	();
	// Use that method to invalidate all cached area.
	bool					EndCachingInvalidRects		(bool bSave = false);
	// }}

	bool					SetChildControlZIndex		(ESChildControl* pChild, int nZIndex);
	bool					SetChildControlName			(ESChildControl* pChild, _string sName, bool& bExists);

	bool					SetChildFocus				(ESChildControl* pControl, bool bFindFocusAcceptable = false);
	ESChildControl*			GetChildFocused				(){ return m_pChildFocused; };

		// Child control's timer
	bool					SetChildControlTimer		(ESChildControl* pChild, UINT nIdEvent, UINT nElapse);
	bool					KillChildControlTimer		(ESChildControl* pChild, UINT nIdEvent);

	MercuryBaseViewInfo*	GetViewInfo					(){return &m_info;};
	BOOL					IsActive					(){return m_bActive;};

	static int				GetChildControlIdUnused		(MercuryBaseView* pBase);
	static	bool			PtInImageRegion				(_Point ptInOwnerCoords, ESChildControl* pChild, RGNDef imageRgn, _Size szImage, bool bCentered);

	void					LocalizeStrings				();

	bool					GetZoom						(float& fZoomCX, float& fZoomCY);
	bool					HasFocus					(ESChildControl* pControl)		{return (pControl == m_pChildFocused);};
	float					GetRatioCX					()								{return m_fRatioCX;};
	float					GetRatioCY					()								{return m_fRatioCY;};
	void					SetDesignerMode				(bool bDesignerMode)			{m_bDesignerMode = bDesignerMode;};
	bool					IsDesignerMode				()								{return m_bDesignerMode;};

	// Named parameters. {{
	bool					AddNamedParameter			(LPCTSTR lpszParam, void* lpParam, bool bUpdateIfExists = false);
	bool					RemoveNamedParameter		(LPCTSTR lpszParam, void*& lpParam);
	bool					GetNamedParameter			(LPCTSTR lpszParam, void*& lpParam);
	// }}

	// Load / Save in design file.
	bool					LoadFromDesignFile			(_string sDesignFile, bool bAddControls = false, _string sParentControlName = _T(""), bool bTranslateControlsRect = false,
														_Size szDefaultWindowSize = _Size(0, 0), int nPosX = -1, int nPosY = -1, int nControlIdIncrement = 0);
	static bool				LoadControlFromDesign		(GrowableMemory* pMem, const _string sClass, const _string sName, ESChildControl*& pControl);
	bool					SaveToDesignFile			(_string sDesignFile);

	// Tracking Tooltop control. {{
	bool					AddToolTip					(ESChildControl* pOwner, _string sToolText);
	bool					RemoveToolTip				(ESChildControl* pOwner);
	bool					UpdateToolTipRect			(ESChildControl* pOwner, _Rect rcNew);
	bool					UpdateToolTipText			(ESChildControl* pOwner, _string sToolText);
	// }}

protected:
	void					ScrollWindow				(int nScrollX, int nScrollY);

	virtual BOOL			UseDoubleBufferedDC			(){return TRUE;};
	virtual void			SetRowSet					(VirtualRowSet* pRowSet){};
	virtual BOOL			OnOK						();
	virtual BOOL			OnCancel					();
	virtual BOOL			OnTabControl				();

	virtual void			SetTitle					();
	virtual void			Setup						();
	
	bool					DestroyAllChilds			();
	ESChildControl*		ChildControlByPoint			(_Point pt, ESChildControl* pParent = NULL);

	bool					SetChildSize				(int nId, int nWidth, int nHeight, bool bRedraw = true);
	bool					SetChildPos					(int nId, int x, int y, bool bRedraw = true);
	_Rect					GetChildRect				(int nId);
	_Point					GetChildPos					(int nId);
	bool					IsControlNameUsed			(_string& sControlName);

	// Redraws child controls only in rect area and returns count of it.
	void					RedrawChildControls			(ESChildControl* pChild, _DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
//	void					GetInvalidChildControls		(ESChildControl* pChild, _Rect* pInvalidClient, CDWordArray& arrInvalidControls);
	// Redraw only child control.
	bool					RedrawChildControl			(ESChildControl* pChild, _Rect rcRedraw);
	// Resize child controls.
	void					ResizeChildControls			(int cx, int cy);

	// Designer mode. {{
	int						GetSelectedCt				();
	bool					IsControlSelected			(ESChildControl* pControl);
	bool					ClearSelections				(bool bRedraw = true);
	bool					AddInSelectedList			(ESChildControl* pControl);
	bool					RemoveFromSelectedList		(ESChildControl* pControl);
	int						RemoveSelectedControls		();
	bool					MoveSelectedControls		(int nOffsetX, int nOffsetY);
	int						CopySelectedControls		(bool bCut = false);
	int						PasteControlsFromClipboard	();
	bool					LockSelectedControls		(bool bUnlock = false);
	ESChildControl*			GetSelectedControl			(int nIndex);

	bool					AddNewControl				(ESChildControl* pControlNew, ESChildControl* pParent = NULL);

	bool					SerializeControls			(GrowableMemory* pMem, ESChildControl* pParent = NULL);
	bool					DeserializeControls			(GrowableMemory* pMem, bool bVariableNames = false, ESChildControl* pParentControl = NULL, AutoSortedArray* pDeserializedControls = NULL, int nControlIdIncrement = 0);

	void					ClearChildControls			(bool bRedraw);
	void					ClearChildControl			(ESChildControl* pControl);
	bool					Redraw						(_Rect rcRedraw, ESChildControl* pChildOnly = NULL);

	// Tracking tooltip.
	bool					CreateTrackingTooltip		();
	bool					DeactivateTooltip			();
	bool					ActivateTooltip				(const TCHAR* pszTipText);
	bool					UpdateTooltipRect			(int cx, int cy);

//	static void _stdcall	RedrawOptimizeTimer			(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	void					ExcludeIntersectionAreas	(List<RECT>* pArrRects);

public:
	virtual void			GetClientRectMy				(_Rect& rcClient){ GetClientRect(rcClient); /*if( m_hWnd )::GetClientRect(m_hWnd, rcClient);*/ };

protected:
	BOOL											m_bActive;						// Active state.
	MercuryGUI*										m_pGUI;
	BOOL											m_bClipBox;
	BOOL											m_bUseForegroundColor;			// Use foreground color.
	COLORREF										m_crForeground;					// Foreground color.
	int												m_nAlpha;						// Foreground color alpha value.
	VirtualRowSet*									m_pRowSet;

	ImageDef										m_hDoubleBufferImage;			// Double buffer image object.
	BITMAP											m_bmDoubleBufferImage;			// Double buffer image info.
    
    
	StringToPtrArray								m_arrNamedParameters;			// Named parameters list.
	MercuryLayerManager								m_layerMan;						// Layer manager class. It manipulates layers of child control by it's z-index.

    ESChildControl*									m_pControlUnderCursor;			// Child control under cursor.
    
	// Child control variables.
	ESChildControl*									m_pFirstChild;					// First child control.
	EnumerableObject<ESChildControlInfo>			m_listChildControlInfos;		// List of child control infos.
	List<ESChildControl>							m_listChildControls;			// List child controls.
	AutoSortedArray									m_arrChildControlByTimer;		// <ESChildControl*><Timer event id>

	//ESChildControl*									m_pControlUnderCursor;			// Child control under cursor.
	ESChildControl*									m_pControlCaptured;				// Child control captured
	ESChildControl*									m_pChildFocused;				// Focused child control.

	bool											m_bAllowZooming;
	_Size											m_szDefWindowSize;				// Default winow size;
	float											m_fRatioCX;						
	float											m_fRatioCY;

	// Designer mode.
	bool											m_bDesignerMode;				// Designer mode.
	StringToIntArray								m_arrSelectedControlIds;		// Key = Control Name, Value = Control Id (31th bit rect auto-calculated flag).
	
	_Rect											m_rcSelectionBound;				// Bound rect of all selected controls.
	_Rect											m_rcSelectionBoundDown;			// Bound rect of all selected controls.
	_Point											m_ptMouseDown;
	bool											m_bMoving;						// Already moving controls.
	bool											m_bSizing;						// Already sizing controls.
	int												m_nSizingHitTest;				// ChildControlHitTest
//	CMenu											m_menuContext;
    _UIMenu                                         m_menuContext;
    AutoSortedArrayTempl<UINT_PTR, CommandProc>     m_mapManuCommandById;

	ESSerializableModifyDlg*						m_propertyDlg;
	ESSerializableModifyDlg*						m_listOfControlsDlg;
	EnumerableObject<ESChildControlAttribute>*		m_pListOfControls;

	_string                                         m_sDesignerFile;				// Loaded designer file.
	ESDesignFileInfo								m_designInfo;					// Loaded design info.
	MercuryBaseViewInfo								m_info;							// View info class.

	//CPPToolTip									m_toolTipCtrl;
	bool											m_bTooltipIsTracking;
	_string                                         m_sToolTipText;

private:
	CriticalSection									m_lockChildControls;
	volatile bool									m_bInvalidRectsCaching;			// Invalid rect area caching is on.
	List<RECT>										m_listCachedInvalidRects;		// List of cached invalid (must redraw) rects.

	// _Rect caching for thread other than window thread. {{
	struct RedrawRectCachingStruct{
		List<RECT>	listCachedInvalidRects;
		DWORD		dwThreadId;
		};
	AutoSortedArray									m_arrRectCachingInfoByThreadId;
	CriticalSection									m_lockRectCachingThread;
	// }}

	CriticalSection									m_lockRedrawOptimizer;
	List<RECT>										m_listRedrawRects;				// Redraw rects cached to perform at once in sec/30 period.
	volatile bool									m_bRedrawOptimizeTimerIsOn;		// Redraw optimize timer status.
	volatile UINT_PTR								m_nRedrawOptimizeTimeId;
    
public:
	virtual BOOL	OnEraseBkgnd				(_DC *pDC);
	virtual void	OnPaint						(DCDef dc, _Rect rcRedraw);

    virtual void	OnSetFocus					(HWND hWndFocusLost);
	virtual void	OnKillFocus					(HWND hWndGainFocus);
	virtual BOOL	OnMouseWheel				(UINT nFlags, short zDelta, _Point pt);
	virtual BOOL	OnKeyDown					(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnChar						(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void	OnSize						(UINT nType, int cx, int cy);
	virtual void	OnMouseMove					(UINT nFlags, _Point point);
	virtual void	OnLButtonDown				(UINT nFlags, _Point point);
	virtual void	OnLButtonDblClk				(UINT nFlags, _Point point);
	virtual void	OnLButtonUp					(UINT nFlags, _Point point);
	virtual void	OnRButtonUp					(UINT nFlags, _Point point);
	virtual BOOL	OnSetCursor					(HWND hWnd, UINT nHitTest, UINT message);
	virtual void	OnTimer						(UINT_PTR nIDEvent);
	virtual void	OnRedrawRect				(CPtrArray*	pArrRects, BOOL	bAtOnce);

protected:
    void    OnCommandCommand            (UINT_PTR nId);
	// Context menu command events used only in designer mode.
	void	OnCommandLabel				();
	void	OnCommandImage				();
	void	OnCommandImageButton		();
	void	OnCommandTextBox			();
	void	OnCommandCheckBox			();
	void	OnCommandComboBox			();
	void	OnCommandSlider				();
	void	OnCommandAvatar				();
	void	OnCommandProgressCtrl		();
	void	OnCommandSimpleRenderView	();
	void	OnCommandList				();
	void	OnCommandTree				();
	void	OnCommandTab				();
	void	OnCommandTabView			();
	void	OnCommandTreeView			();
	void	OnCommandBanner				();
	void	OnCommandPropertyGrid		();

	void	OnCommandLabelAsChild		();
	void	OnCommandImageAsChild		();
	void	OnCommandImageButtonAsChild	();
	void	OnCommandTextBoxAsChild		();
	void	OnCommandCheckBoxAsChild	();
	void	OnCommandComboBoxAsChild	();
	void	OnCommandSliderAsChild		();
	void	OnCommandProgressCtrlAsChild();
	void	OnCommandAvatarAsChild		();
	void	OnCommandSimpleRenderViewAsChild();
	void	OnCommandListAsChild		();
	void	OnCommandTreeAsChild		();
	void	OnCommandTabAsChild			();
	void	OnCommandTabViewAsChild		();
	void	OnCommandTreeViewAsChild	();
	void	OnCommandBannerAsChild		();
	void	OnCommandPropertyGridAsChild();

	void	OnCommandAlignLefts			();
	void	OnCommandAlignTops			();
	void	OnCommandAlignRights		();
	void	OnCommandAlignBottoms		();
	void	OnCommandMakeSameSizeWidth	();
	void	OnCommandMakeSameSizeHeight	();
	void	OnCommandMakeSameSizeBoth	();
	void	OnCommandLock				();
	void	OnCommandUnlock				();

	void	OnCommandCopy				();
	void	OnCommandCut				();
	void	OnCommandPaste				();
	void	OnCommandListOfControls		();
	void	OnCommandOpenDesignerFile	();
	void	OnCommandSaveDesignerFile	();
	void	OnCommandProperties			();
	void	OnCommandBaseViewProperties	();

	// pRectDC		- rect in DC coordinates.
	// pRectClient	- rect in client coordinates.
	virtual void	OnPaintClient						(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	// Called when WM_PAINT function is called by framework.
	virtual void	OnPrePaint							(_Rect& rcRepaint){};
//	virtual	void	DesignerContextMenu					(CMenu* pPopupMenu, CMenu* pNewControlsMenu, CMenu* pNewChildControlsMenu){};

	// Child control's virtual methods.
	virtual void	OnChildVisibilityChanged			(ESChildControl* pSender, bool bVisible);
	virtual void	OnChildRectAreaChanged				(ESChildControl* pSender);

	// Variable modify callback
	static bool		ControlAttribute_VisibleModify		(ESChildControlAttribute* pObject, VariableInfo* pInfo);
	static bool		ControlAttribute_NameModify			(ESChildControlAttribute* pObject, VariableInfo* pInfo);

	// Child control event methods.
	void			ListOfControls_PropertyItemSelChange(ESChildControl* pSender, int eventCode, int nParam);
};
