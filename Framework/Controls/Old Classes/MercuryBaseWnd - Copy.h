#pragma once


/////////////////////////////////////////////////////////////////////////////
// MercuryBaseWnd window

#include "MercuryGUI.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include "..\Array\AutoSortedArray.h"
#include "..\Array\AutoSortedArrayInt64.h"
#include "..\Array\StringToIntArray.h"
#include "..\Array\List.h"
#include "..\Data\VRowSet.h"
#include "..\PPToolTip\PPToolTip.h"

#include "ESChildControlInfo.h"
#include "MercuryBaseViewInfo.h"
#include "ESSerializableModifyDlg.h"
#include "ESLocalizationMan.h"
#include "MercuryLayerMan.h"

struct DialogItem{
	DWORD		dwHelpId;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	short		shLeftPos;
	short		shTopPos;
	short		shWidth;
	short		shHeight;
	DWORD		dwControlId;

	CString		sClassName;
	WORD		wClassId;
	CString		sText;
	DWORD		dwResID; // Instead of text.
	};

struct DialogTemplate{
	short 		shDialogVer;
	short 		shDialogTemplateType;
	DWORD 		dwHelpId;
	DWORD 		dwExStyle;
	DWORD 		dwStyle;
	short 		shItemCt;
	short 		shLeftPos;
	short 		shTopPos;
	short 		shWidth;
	short 		shHeight;
	short		shMenuExistance;
	short		shMenuId;
	CString		sClassName;
	CString		sCaptionText;
	short		fontHeight;
	short		fontWeight;
	BYTE		fontItalic;
	BYTE		fontCharset;
	CString		sFontFaceName;

	DialogItem*	pItems;
	};

#define CHILD_ControlIdUnused	10000

#define WM_REDRAW_RECT			WM_USER + 20


#define MAKE_RECT(l,t,r,b)\
	(((__int64)l) << 48) | (((__int64)t) << 32) | (((__int64)r) << 16) | ((__int64)b)


///////////////////////////////////////////////////////////////////////
//
//	Fill mode defines.
//
enum FillMode
	{
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
enum VAlignment
	{
	VAlignement_Top = 0,
	VAlignement_Center,
	VAlignement_Bottom
	};

enum HAlignment
	{
	HAlignement_Left = 0,
	HAlignement_Middle,
	HAlignement_Right
	};

#define DESIGN_FILE_SIGNATURE 0x53445345 // ESDS


///////////////////////////////////////////////////////////////////////
//
//	Child control defines.
//

enum ChildControlEventCode
	{
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
typedef void (__thiscall CObject::*ControlEventListener)(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam);

struct ChildEventListenerInfo{
	ControlEventListener	method;
	ESChildControl*			pSender;
	CObject*				pReceiver;
	};

class ESChildControlAttribute : public Serializable
{
public:
	ESChildControlAttribute();
	virtual ~ESChildControlAttribute();

public:
	CString			m_sClassName;		// Child control's class name.
	CString			m_sName;			// Child control's name.
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
	COleDateTime	m_dtCreationDateTime;
	CString			m_sAuthor;
	CString			m_sDescription;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESDesignFileInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESChildControl;
class ESDialogBase;
class MercuryBaseView : public CView
{
public:
	MercuryBaseView();
	virtual ~MercuryBaseView();

	friend ESChildControl;
	friend MercuryBaseViewInfo;
	friend ESFrameBase;

	DECLARE_DYNCREATE(MercuryBaseView)

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
	void					RedrawRect				(CRect rcRedraw, BOOL bRedrawAtOnce = FALSE);
	void					RedrawMultipleRects		(CRect* pRectsRedraw, int nCt, BOOL bRedrawAtOnce = FALSE);
	bool					DoubleBufferedDC		()										{return  (UseDoubleBufferedDC()||m_bUseForegroundColor);};
	void					UseForegroundColor		(BOOL bUse, bool bInvalidate = false)	{m_bUseForegroundColor = bUse; if(bInvalidate){Invalidate(0);}};
	void					SetForegroundColor		(COLORREF crForeground)					{m_crForeground = crForeground;};
	void					SetForegroundColorAlpha	(int nAlpha)							{m_nAlpha = (nAlpha%255);};

	bool					AddEventListener		(CObject* pReceiver, int nControlId, ChildControlEventCode eventCode, ControlEventListener method);
	bool					AddEventListener		(CObject* pReceiver, ESChildControl* pControl, ChildControlEventCode eventCode, ControlEventListener method);
	bool					AddEventListenerByName	(CObject* pReceiver, TCHAR* pszName, ChildControlEventCode eventCode, ControlEventListener method);

	bool					SetControlVisibility	(int nControlId, bool bVisible, bool bRedraw);
	bool					SetControlVisibilityByName(TCHAR* pszName, bool bVisible, bool bRedraw);
	bool					SetupCheckBoxByName		(TCHAR* pszName, bool bVisible, bool bCheckState,  bool bRedraw);
	bool					GetCheckBoxStateByName	(TCHAR* pszName);

	void					DisableImageButton		(int nButtonId, bool bEnable = false, bool bRedraw = false);
	void					DisableImageButtonByName(TCHAR* pszName, bool bEnable = false, bool bRedraw = false);

	ESChildControl*			GetChildControl			(int nId);
	ESChildControl*			GetChildControlByName	(TCHAR* pszName);
	int						GetChildControlIdByName	(TCHAR* pszName);

	// Child control methods.
	bool					AddChildControl			(ESChildControl* pControl, int nId, CRect rcControl, ESChildControl* pParent, int nZIndex = -1, CString sControlName = _T("")); 
	bool					AddChildControlEx		(ESChildControl* pChild, int nId, CRect rcControl, ESChildControl* pParent, CSize szDefWindowSize, CString sControlName = _T(""), int nZIndex = -1);
	bool					IsValidChildControl		(ESChildControl* pControl){ return (m_listChildControls.IndexOf(pControl) != -1); };

	bool					RemoveChildControl		(ESChildControl* pControl, bool bRemoveAllSiblings = false);

	bool					RedrawChildControl		(int nId);
	bool					RedrawChildControlByName(CString sName);
	bool					RemoveChildControlByName(CString sName);

	bool					DrawBackground			(CDC* pDC, CRect* pRectDC, CRect* pRectClient, CRect rcControl, int nMode, COLORREF crFill, 
													CImage* pImageBg, CRect rcOmitBorder, bool bSizeBox = false, int nScaleMode = 1);

	// Invalid rect area caching. Note: this mechanism is usefull to prevent invalidate same areas multiple time. {{
	bool					StartCachingInvalidRects();
	// Use that method to invalidate all cached area.
	bool					EndCachingInvalidRects	(bool bSave = false);
	// }}

	bool					SetChildControlZIndex	(ESChildControl* pChild, int nZIndex);
	bool					SetChildControlName		(ESChildControl* pChild, CString sName, bool& bExists);

	bool					SetChildFocus			(ESChildControl* pControl, bool bFindFocusAcceptable = false);
	ESChildControl*			GetChildFocused			(){ return m_pChildFocused; };

		// Child control's timer
	bool					SetChildControlTimer	(ESChildControl* pChild, UINT nIdEvent, UINT nElapse);
	bool					KillChildControlTimer	(ESChildControl* pChild, UINT nIdEvent);

	MercuryBaseViewInfo*	GetViewInfo				(){return &m_info;};
	BOOL					IsActive				(){return m_bActive;};

	static int				GetChildControlIdUnused	(MercuryBaseView* pBase);
	static	bool			PtInImageRegion			(CPoint ptInOwnerCoords, ESChildControl* pChild, CRgn* pImageRgn, CSize szImage, bool bCentered);

	void					LocalizeStrings			();

	bool					GetZoom					(float& fZoomCX, float& fZoomCY);
	bool					HasFocus				(ESChildControl* pControl)		{return (pControl == m_pChildFocused);};
	float					GetRatioCX				()								{return m_fRatioCX;};
	float					GetRatioCY				()								{return m_fRatioCY;};
	void					SetDesignerMode			(bool bDesignerMode)			{m_bDesignerMode = bDesignerMode;};
	bool					IsDesignerMode			()								{return m_bDesignerMode;};

	// Named parameters. {{
	bool					AddNamedParameter		(LPCTSTR lpszParam, void* lpParam, bool bUpdateIfExists = false);
	bool					RemoveNamedParameter	(LPCTSTR lpszParam, void*& lpParam);
	bool					GetNamedParameter		(LPCTSTR lpszParam, void*& lpParam);
	// }}

	// Load / Save in design file.
	bool					LoadFromDesignFile		(CString sDesignFile, bool bAddControls = false, CString sParentControlName = _T(""), bool bTranslateControlsRect = false, 
													CSize szDefaultWindowSize = CSize(0, 0), int nPosX = -1, int nPosY = -1);
	static bool				LoadControlFromDesign	(GrowableMemory* pMem, const CString sClass, const CString sName, ESChildControl*& pControl);
	bool					SaveToDesignFile		(CString sDesignFile);

	/*
	// Tracking Tooltop control. {{
	bool					AddToolTip				(ESChildControl* pOwner, CString sToolText);
	bool					RemoveToolTip			(ESChildControl* pOwner);
	bool					UpdateToolTipRect		(ESChildControl* pOwner, CRect rcNew);
	bool					UpdateToolTipText		(ESChildControl* pOwner, CString sToolText);
	// }}
	*/

protected:
	void					ScrollWindow			(int nScrollX, int nScrollY);
	BOOL					RegisterWndClass		(LPCTSTR lpszClassName);

	virtual BOOL			UseDoubleBufferedDC		(){return TRUE;};
	virtual void			SetRowSet				(VirtualRowSet* pRowSet){};
	virtual BOOL			OnOK					();
	virtual BOOL			OnCancel				();
	virtual BOOL			OnTabControl			();
	
	bool					DestroyAllChilds		();
	ESChildControl*			ChildControlByPoint		(CPoint pt, ESChildControl* pParent = NULL);

	bool					SetChildSize			(int nId, int nWidth, int nHeight, bool bRedraw = true);
	bool					SetChildPos				(int nId, int x, int y, bool bRedraw = true);
	CRect					GetChildRect			(int nId);
	CPoint					GetChildPos				(int nId);
	bool					IsControlNameUsed		(CString& sControlName);

	// Redraws child controls only in rect area and returns count of it.
	void					RedrawChildControls		(ESChildControl* pChild, CDC* pDC, CRect* pRectDC, CRect* pRectClient);
//	void					GetInvalidChildControls	(ESChildControl* pChild, CRect* pInvalidClient, CDWordArray& arrInvalidControls);
	// Redraw only child control.
	bool					RedrawChildControl		(ESChildControl* pChild, CRect rcRedraw);
	// Resize child controls.
	void					ResizeChildControls		(int cx, int cy);

	// Designer mode. {{
	int						GetSelectedCt			();
	bool					IsControlSelected		(ESChildControl* pControl);
	bool					ClearSelections			(bool bRedraw = true);
	bool					AddInSelectedList		(ESChildControl* pControl);
	bool					RemoveFromSelectedList	(ESChildControl* pControl);
	int						RemoveSelectedControls	();
	bool					MoveSelectedControls	(int nOffsetX, int nOffsetY);
	int						CopySelectedControls	(bool bCut = false);
	int						PasteControlsFromClipboard();
	bool					LockSelectedControls	(bool bUnlock = false);
	ESChildControl*			GetSelectedControl		(int nIndex);

	bool					AddNewControl			(ESChildControl* pControlNew, ESChildControl* pParent = NULL);

	bool					SerializeControls		(GrowableMemory* pMem, ESChildControl* pParent = NULL);
	bool					DeserializeControls		(GrowableMemory* pMem, bool bVariableNames = false, ESChildControl* pParentControl = NULL, AutoSortedArray* pDeserializedControls = NULL);

	void					ClearChildControls		(bool bRedraw);
	void					ClearChildControl		(ESChildControl* pControl);
	bool					Redraw					(CRect rcRedraw, ESChildControl* pChildOnly = NULL);

	// Tracking tooltip.
	bool					CreateTrackingTooltip	();
	bool					DeactivateTooltip		();
	bool					ActivateTooltip			(TCHAR* pszTipText);
	bool					UpdateTooltipRect		(int cx, int cy);

	static void _stdcall	RedrawOptimizeTimer		(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	void					ExcludeIntersectionAreas(List<RECT>* pArrRects);

protected:
	BOOL											m_bActive;						// Active state.
	MercuryGUI*										m_pGUI;
	BOOL											m_bClipBox;
	BOOL											m_bUseForegroundColor;			// Use foreground color.
	COLORREF										m_crForeground;					// Foreground color.
	int												m_nAlpha;						// Foreground color alpha value.
	VirtualRowSet*									m_pRowSet;

	HBITMAP											m_hDoubleBufferImage;			// Double buffer image object.
	BITMAP											m_bmDoubleBufferImage;			// Double buffer image info.

	StringToPtrArray								m_arrNamedParameters;			// Named parameters list.
	MercuryLayerManager								m_layerMan;						// Layer manager class. It manipulates layers of child control by it's z-index.

	// Child control variables.
	ESChildControl*									m_pFirstChild;					// First child control.
	EnumerableObject<ESChildControlInfo>			m_listChildControlInfos;		// List of child control infos.
	List<ESChildControl>							m_listChildControls;			// List child controls.
	AutoSortedArray									m_arrChildControlByTimer;		// <ESChildControl*><Timer event id>
	ESChildControl*									m_pControlUnderCursor;			// Child control under cursor.
	ESChildControl*									m_pControlCaptured;				// Child control captured
	ESChildControl*									m_pChildFocused;				// Focused child control.

	bool											m_bAllowZooming;
	CSize											m_szDefWindowSize;				// Default winow size;
	float											m_fRatioCX;						
	float											m_fRatioCY;

	// Designer mode.
	bool											m_bDesignerMode;				// Designer mode.
	StringToIntArray								m_arrSelectedControlIds;		// Key = Control Name, Value = Control Id (31th bit rect auto-calculated flag).
	
	CRect											m_rcSelectionBound;				// Bound rect of all selected controls.
	CRect											m_rcSelectionBoundDown;			// Bound rect of all selected controls.
	CPoint											m_ptMouseDown;
	bool											m_bMoving;						// Already moving controls.
	bool											m_bSizing;						// Already sizing controls.
	int												m_nSizingHitTest;				// ChildControlHitTest
	CMenu											m_menuContext;

	ESSerializableModifyDlg							m_propertyDlg;

	ESSerializableModifyDlg							m_listOfControlsDlg;
	EnumerableObject<ESChildControlAttribute>*		m_pListOfControls;

	CString											m_sDesignerFile;				// Loaded designer file.
	ESDesignFileInfo								m_designInfo;					// Loaded design info.
	MercuryBaseViewInfo								m_info;							// View info class.

	CPPToolTip										m_toolTipCtrl;
	bool											m_bTooltipIsTracking;
	CString											m_sToolTipText;

private:
	CCriticalSection								m_lockChildControls;
	volatile bool									m_bInvalidRectsCaching;			// Invalid rect area caching is on.
	List<RECT>										m_listCachedInvalidRects;		// List of cached invalid (must redraw) rects.

	// Rect caching for thread other than window thread. {{
	struct RedrawRectCachingStruct{
		List<RECT>	listCachedInvalidRects;
		DWORD		dwThreadId;
		};
	AutoSortedArray									m_arrRectCachingInfoByThreadId;
	CCriticalSection								m_lockRectCachingThread;
	// }}

	CCriticalSection								m_lockRedrawOptimizer;
	List<RECT>										m_listRedrawRects;				// Redraw rects cached to perform at once in sec/30 period.
	volatile bool									m_bRedrawOptimizeTimerIsOn;		// Redraw optimize timer status.
	volatile DWORD_PTR								m_nRedrawOptimizeTimeId;

protected:
	afx_msg int		OnCreate				(LPCREATESTRUCT lcs);
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg void	OnPaint					();
	afx_msg void	OnSetFocus				(CWnd* pOldWnd);
	afx_msg void	OnKillFocus				(CWnd* pNewWnd);

	afx_msg BOOL	OnMouseWheel			(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void	OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnChar					(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	afx_msg void	OnMouseMove				(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown			(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonUp				(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDblClk			(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp				(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor				(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT	OnGetDlgCode			();
	afx_msg void	OnTimer					(UINT_PTR nIDEvent);
	afx_msg LRESULT	OnRedrawRect			(WPARAM wParam, LPARAM lParam);

	// Context menu command events used only in designer mode.
	afx_msg	void	OnCommandLabel				();
	afx_msg	void	OnCommandImage				();
	afx_msg	void	OnCommandImageButton		();
	afx_msg	void	OnCommandTextBox			();
	afx_msg	void	OnCommandCheckBox			();
	afx_msg	void	OnCommandComboBox			();
	afx_msg	void	OnCommandSlider				();
	afx_msg	void	OnCommandAvatar				();
	afx_msg	void	OnCommandProgressCtrl		();
	afx_msg	void	OnCommandSimpleRenderView	();
	afx_msg	void	OnCommandList				();
	afx_msg	void	OnCommandTree				();
	afx_msg	void	OnCommandTab				();
	afx_msg	void	OnCommandTabView			();
	afx_msg	void	OnCommandTreeView			();
	afx_msg	void	OnCommandBanner				();
	afx_msg	void	OnCommandPropertyGrid		();

	afx_msg	void	OnCommandLabelAsChild		();
	afx_msg	void	OnCommandImageAsChild		();
	afx_msg	void	OnCommandImageButtonAsChild	();
	afx_msg	void	OnCommandTextBoxAsChild		();
	afx_msg	void	OnCommandCheckBoxAsChild	();
	afx_msg	void	OnCommandComboBoxAsChild	();
	afx_msg	void	OnCommandSliderAsChild		();
	afx_msg	void	OnCommandProgressCtrlAsChild();
	afx_msg	void	OnCommandAvatarAsChild		();
	afx_msg	void	OnCommandSimpleRenderViewAsChild();
	afx_msg	void	OnCommandListAsChild		();
	afx_msg	void	OnCommandTreeAsChild		();
	afx_msg	void	OnCommandTabAsChild			();
	afx_msg	void	OnCommandTabViewAsChild		();
	afx_msg	void	OnCommandTreeViewAsChild	();
	afx_msg	void	OnCommandBannerAsChild		();
	afx_msg	void	OnCommandPropertyGridAsChild();

	afx_msg	void	OnCommandAlignLefts			();
	afx_msg	void	OnCommandAlignTops			();
	afx_msg	void	OnCommandAlignRights		();
	afx_msg	void	OnCommandAlignBottoms		();
	afx_msg	void	OnCommandMakeSameSizeWidth	();
	afx_msg	void	OnCommandMakeSameSizeHeight	();
	afx_msg	void	OnCommandMakeSameSizeBoth	();
	afx_msg	void	OnCommandLock				();
	afx_msg	void	OnCommandUnlock				();

	afx_msg	void	OnCommandCopy				();
	afx_msg	void	OnCommandCut				();
	afx_msg	void	OnCommandPaste				();
	afx_msg	void	OnCommandListOfControls		();
	afx_msg	void	OnCommandOpenDesignerFile	();
	afx_msg	void	OnCommandSaveDesignerFile	();
	afx_msg	void	OnCommandProperties			();
	afx_msg	void	OnCommandBaseViewProperties	();
	DECLARE_MESSAGE_MAP()

	// pRectDC		- rect in DC coordinates.
	// pRectClient	- rect in client coordinates.
	virtual void	OnPaintClient				(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	// Called when WM_PAINT function is called by framework.
	virtual void	OnPrePaint					(CRect& rcRepaint){};
	virtual void	GetClientRectMy				(CRect& rcClient){ GetClientRect(rcClient); };
	virtual BOOL	OnParentResized				(int cx, int cy){return FALSE;};
	virtual void	OnDraw						(CDC* pDC);
	virtual	void	DesignerContextMenu			(CMenu* pPopupMenu, CMenu* pNewControlsMenu, CMenu* pNewChildControlsMenu){};

	virtual BOOL	PreTranslateMessage			(MSG* pMSG);

	// Child control's virtual methods.
	virtual void	OnChildVisibilityChanged	(ESChildControl* pSender, bool bVisible);
	virtual void	OnChildRectAreaChanged		(ESChildControl* pSender);

	// Variable modify callback
	static bool ControlAttribute_VisibleModify	(ESChildControlAttribute* pObject, VariableInfo* pInfo);
	static bool ControlAttribute_NameModify		(ESChildControlAttribute* pObject, VariableInfo* pInfo);

	// Child control event methods.
	void	ListOfControls_PropertyItemSelChange(ESChildControl* pSender, int eventCode, int nParam);
};