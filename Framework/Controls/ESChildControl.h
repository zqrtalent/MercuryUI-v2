#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControl window

#include "MercuryBaseWnd.h"
#include "MercuryGUI.h"
#include "../Array/AutoSortedArrayTempl.h"
#include "../Array/List.h"
#include "../Data/VRowSet.h"
#include "../Serialize/Serializable.h"
#include "../String/StringDecoder.h"
#include "../_platformCompat/PlatformRgn.h"
using namespace std;

class ESChildControlInfo;
class ESChildControl : public Serializable
{
// Construction
public:
	ESChildControl();
	virtual ~ESChildControl();

	friend class MercuryBaseView;
	friend class ESEasyMotion;

	virtual bool	Create						(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos						(int x, int y, bool bRedraw);
	virtual bool	SetSize						(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy						();
	virtual void	OwnerWindowSizeChanged		(int cx, int cy);
	virtual void	SetLogicalPos				(float fPosX, float fPosY){m_fPosX = fPosX; m_fPosY = fPosY;};
	virtual	void	SetLogicalSize				(float fCX, float fCY){m_fCX = fCX; m_fCY = fCY;};
	virtual bool	VisibilityChanged			(bool bVisible){return true;};
	virtual bool	OnScrollBarEvent			(bool bVScroll, int nCode, ESChildControl* pSender){return false;};
	virtual bool	IsScrollBarVisible			(ESChildControl* pScrollBar){return false;};
	virtual bool	MoveControl					(int x, int y, bool bRedraw){return false;};
	virtual bool	SetFocus					(){return false;};
	virtual void	KillFocus					(){};
	virtual void	SetFont						(_Font* pFont){};
	virtual bool	MouseUpDownEventsInDesigner	(){return false;};

	// Remain control captured out of client area.
	virtual bool	RemainCapturedOutOfClientArea(){return true;};
	// Retrieve region, bound size and centered flag.
	virtual RGNDef	GetControlRgn				(_Size& szRgnBound, bool& bCentered){return NULL;};
	// Retrieve tool tip text.
	virtual bool	GetTooltipText				(_string& sTooltipText){return false;};

	virtual BOOL	OnOK						(){return FALSE;};
	virtual BOOL	OnCancel					(){return FALSE;};
	virtual BOOL	OnTabControl				(){return FALSE;};

	// Event listener. {{
	bool			AddEventListener			(ESFrameBase* pReceiver, int eventCode, ControlEventListener method);
	bool			SendEvent					(ESChildControl* pSender, int eventCode, void* lpParam = 0);
	bool			RemoveEvents				(ESChildControl* pReceiver);
	void			RemoveAllEvents				();
	void			AddListenEventControl		(ESChildControl* pControlListen);
	// }}

	virtual _Rect	GetClientRect				(){return m_rRect;};
	int				GetClientWidth				(){return m_rRect.Width();};
	int				GetClientHeight				(){return m_rRect.Height();};
	_Point			GetClientPos				(){return _Point(m_rRect.left, m_rRect.top);};
	int				GetId						(){return m_nId;};
	int				GetZIndex					(){return m_nZIndex;};
	bool			IsCreated					(){return m_bCreated;};
	_string         GetName						(){return m_sName;};

	void			SetFixedPosition			(_Rect rcFixedPos){m_rcFixedPosition = rcFixedPos;};
	_Rect			GetFixedPosition			(){return m_rcFixedPosition;};
	void			SetRelativeControls			(ESChildControl* pLeft, ESChildControl* pRight, ESChildControl* pTop, ESChildControl* pBottom){
						m_pTopRelativeControl		= pTop;
						m_pBottomRelativeControl	= pBottom;
						m_pLeftRelativeControl		= pLeft;
						m_pRightRelativeControl		= pRight;
						};

	virtual bool	PtInClientArea				(_Point pt);

	ESChildControl*	GetChild					(){return m_pChild;};
	ESChildControl*	GetParent					(){return m_pParent;};
	ESChildControl*	GetNext						(){return m_pNext;};
	ESChildControl*	GetPrev						();

	virtual void	Redraw						(bool bLayersToo = true);
	virtual void	RedrawRect					(_Rect rcRedraw, bool bLayersToo = true);

	bool			IsVisible					(){return m_bVisible;};
	void			SetVisible					(bool bVisible, bool bRedraw, bool bRememberRestoreHiddenChilds = false);
	bool			IsMouseEventsAllowed		(){return m_bAllowMouseEvents;};
	void			AllowMouseEvents			(bool bAllow){
        m_bAllowMouseEvents = bAllow;
        
    }

	bool			IsDesignerMode				(){return (m_pOwner ? m_pOwner->IsDesignerMode() : false);};

	void			SetSizeAutoCalculation		(bool bCalc){m_bLogicalPos = bCalc;};

	float			GetLogicalPosX				() {return m_fPosX;};
	float			GetLogicalPosY				() {return m_fPosY;};
	float			GetLogicalSizeCX			() {return m_fCX;};
	float			GetLogicalSizeCY			() {return m_fCY;};

	bool			HasLogicalPos				() {return m_bLogicalPos;};

	bool			SetTimer					(UINT nIdEvent, UINT nElapse);
	bool			KillTimer					(UINT nIdEvent);

	void			AllowToSaveScreenImage		(bool bAllow)				{m_bAllowToSaveScreen = bAllow;};
	bool			IsLocked					()							{return m_bLocked;};
	void			SetLock						(bool bLock)				{m_bLocked = bLock;};
	bool			AllowedToSerialize			()							{return m_bSerialize;};
	void			SetSerializeFlag			(bool bSerialize)			{m_bSerialize = bSerialize;};
	void			SetParent					(ESChildControl* pParent)	{m_pParent = pParent;};
	MercuryBaseView*GetOwner					()							{return m_pOwner;};
	void			SetOwner					(MercuryBaseView* pOwner)	{m_pOwner = pOwner;};

	int				GetChildsByVisibility		(bool bVisibility, CDWordArray* pArrControlIds);
	
protected:
	void			GetChildsByVisibility_Rec	(bool bVisibility, CDWordArray* pArrControlIds);

	void			SetZIndex					(int nZIndex);
	void			SetName						(_string sName){m_sName = sName;};

	void			AddChild					(ESChildControl* pChild);
	void			SetVisible_Rec				(ESChildControl* pControl, bool bVisible);
	bool			GetPosInPixels				(_Point& ptPos);
	bool			GetSizeInPixels				(_Size& szSize);

	void			MoveChildControls			(int nOffsetX, int nOffsetY);

	bool			CalcClientRectangle			(int nOwnerCX, int nOwnerCY, _Rect& rcClient, ESChildControl* pParent);

	// Designer methods. {{
	enum ChildControlHitTest{
		LeftTop = 0, MiddleTop, RightTop, RightCenter, RightBottom, MiddleBottom, LeftBottom, LeftCenter
		};
	int				_SizeBoxHitTest				(_Point pt);
	// }}

protected:
	virtual void	OnPaintClient				(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);

	virtual	void	OnMouseHover				(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave				(UINT nFlags);
	virtual void	OnMouseEnter				(UINT nFlags);
	virtual BOOL	OnSetCursor					(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonDown				(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp					(_Point pt, UINT nFlags);
	virtual	void	OnRButtonDown				(_Point pt, UINT nFlags);
	virtual	void	OnRButtonUp					(_Point pt, UINT nFlags);
	virtual	void	OnLButtonClick				(_Point pt, UINT nFlags);
	virtual	void	OnRButtonClick				(_Point pt, UINT nFlags);
	virtual	void	OnMButtonClick				(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDblClick			(_Point pt, UINT nFlags);
	virtual	void	OnRButtonDblClick			(_Point pt, UINT nFlags);
	virtual	void	OnMButtonDblClick			(_Point pt, UINT nFlags);

	virtual	void	OnMouseWheel				(UINT nFlags, short zDelta, _Point pt){};
	virtual	void	OnKeyDown					(UINT nChar, UINT nRepCnt, UINT nFlags){};
	virtual	void	OnChar						(UINT nChar, UINT nRepCnt, UINT nFlags){};
	virtual void	OnTimer						(UINT nIdEvent){};

	virtual void	OnMotionStart				(DWORD_PTR dwParam){};
	virtual void	OnMotionProgress			(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY){};
	virtual void	OnMotionEnd					(){};
	virtual void	OnMotionStopped				(){};
	virtual void	OnLocalizeStrings			(){};

public:
	virtual void	OnSerializationCompleted	(){};
	virtual void	OnDeserializationCompleted	(){};

protected:
	// Designer virtual methods
	virtual _string     GetControlDefaultName	(){return _T("_control");};
	virtual _Size		GetControlDefaultSize	(){return _Size(100, 50);};
	virtual bool		AllowToAddChildControl	(){return false;};

public:
	static bool	RegisterChildClass(_string sClassName, ESChildControl* pClassInstance){
		AutoSortedArrayTempl<DWORD, ESChildControl*>*	pArrClassInstanceByName = GetArrayClassInstanceByName();
		List<ESChildControl>*							pListClassInstances = GetListClassInstances();
		if( !sClassName.length() || !pClassInstance )
			return false;

		DWORD	dwCode	= DecodeString(sClassName.c_str());
		if( !dwCode ){
			delete pClassInstance;
			return false;
			}

		if( pArrClassInstanceByName->IndexOf(dwCode) == -1 ){
			pArrClassInstanceByName->Add(dwCode, pClassInstance);
			pListClassInstances->Add(pClassInstance);
			}
		else
			delete pClassInstance;
		return true;
		}

	static bool	DestroyChildClassObjects(){
		AutoSortedArrayTempl<DWORD, ESChildControl*>*	pArrClassInstanceByName = GetArrayClassInstanceByName();
		List<ESChildControl>*							pListClassInstances = GetListClassInstances();
		
		pArrClassInstanceByName->DeleteAll();
		pArrClassInstanceByName->FreeExtra();
		pListClassInstances->RemoveAll(true);
		return true;
		}
	
	static ESChildControl* GetChildClassInstance(_string sClassName){
		DWORD	dwCode	= DecodeString(sClassName.c_str());
		if( !dwCode ) return NULL;

		AutoSortedArrayTempl<DWORD, ESChildControl*>*	pArrClassInstanceByName = GetArrayClassInstanceByName();
		int nIndex = pArrClassInstanceByName->IndexOf(dwCode);
		if( nIndex == -1 )
			return NULL;

		ESChildControl* pControlInstance = pArrClassInstanceByName->GetData(nIndex);
		if( !pControlInstance )
			return NULL;

		ESChildControl* pNewControl = (ESChildControl*)pControlInstance->CreateSerializableObject();
		return pNewControl;
		}

protected:
	static AutoSortedArrayTempl<DWORD, ESChildControl*>*	GetArrayClassInstanceByName(){
		static  AutoSortedArrayTempl<DWORD, ESChildControl*> arrClassInstanceByName;
		return &arrClassInstanceByName;
		}

	static List<ESChildControl>*	GetListClassInstances(){
		static  List<ESChildControl> listClassInstances;
		listClassInstances.SetDestroyObjects(TRUE);
		return &listClassInstances;
		}

protected:
	int								m_nId;						// Control's identifier.
	_string                         m_sName;					// Control's unique name.
	ESChildControlInfo*				m_pControlInfo;				// Control info class.
	int								m_nZIndex;					// Z-Index of control.
	bool							m_bVisible;					// Controls visibility.
	bool							m_bCreated;					// Created status.
	bool							m_bLocked;					// Lock state of control (Used only in designer mode).
	bool							m_bSerialize;				// Serialize control in designer file.

	CDWordArray*					m_pArrChildIdsHidden;		// Hidden child control id's. Need to restore state of child controls when its parent became visible again.

protected:
	_string                         m_sClassName;				// Control's class name.
	_Rect							m_rRect;					// _Rect in owner view coordinates.
	MercuryBaseView*				m_pOwner;					// Owner base view.
	ESChildControl*					m_pParent;					// Parent child control.
	ESChildControl*					m_pChild;					// First child control.
	ESChildControl*					m_pNext;					// Next child control.

	AutoSortedArray					m_arrListenerByEventCode;	// <ChildControlEventCode><List<ChildEventListenerInfo>*>
	AutoSortedArray					m_arrControlsEventListening;// Event listening controls (To determine whos events are this control listening). <ESChildControl><???>

	bool							m_bAllowToSaveScreen;		// Allow to save screen image to restore later.

	bool							m_bModal;					// Indicates modal child control.

	bool							m_bAllowMouseEvents;		// Mouse events allowed.
    
	_Rect							m_rcFixedPosition;			// Fixed position of lef, top, right and bottom sides depending on parent control.
	// Relative controls.
	ESChildControl*					m_pTopRelativeControl;		
	ESChildControl*					m_pBottomRelativeControl;
	ESChildControl*					m_pLeftRelativeControl;
	ESChildControl*					m_pRightRelativeControl;

	BOOL							m_bMouseCaptured;
	_Point							m_ptMouseDown;

	bool							m_bLogicalPos;				
	float							m_fPosX;
	float							m_fPosY;
	float							m_fCX;
	float							m_fCY;
public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControl();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	NameModify					(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	ZIndexModify				(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	VisibilityModify			(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	ControlRectModify			(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	AllowMouseEventsModify		(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	FixedPositionModify			(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew);
};
