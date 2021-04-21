// ESFrameBase.h : header file
//
#pragma once

class MercuryBaseView;
class ESChildControl;

#include "../_platformCompat/PlatformCompat.h"
#include "../_platformCompat/PlatformDeviceContext.h"
#include "../_platformCompat/PlatformPoint.h"
#include "../_platformCompat/PlatformSize.h"
#include "../Array/AutoSortedArray.h"

#ifndef WM_REDRAW_RECT
	#define WM_REDRAW_RECT	WM_USER + 20
#endif

@protocol OSXWindowEvents

@optional
-(void)         OnPaint:(_DC*)pDC;
-(BOOL)         OnEraseBkgnd:(_DC*)pDC;
-(void)         OnSysCommand:(UINT)nID param:(LPARAM)lParam;
-(void)			OnDestroy;
-(void)			OnSize:(UINT)nType width:(int)cx height:(int)cy;
-(void)			OnClose;
-(LRESULT)      OnNcHitTest:(_Point) pt;
-(BOOL)			OnSetCursor:(HWND) hWnd hitTest:(UINT)nHitTest message:(UINT)message;
-(void)			OnSetFocus:(HWND) hWndFocusLost;
-(void)			OnKillFocus:(HWND) hWndGainFocus;
-(BOOL)			OnMouseWheel:(UINT)nFlags Delta:(short)zDelta Pt:(_Point)pt;
-(BOOL)			OnKeyDown:(UINT)nChar RepCnt:(UINT)nRepCnt flags:(UINT)nFlags;

-(BOOL)			OnChar:(UINT)nChar RepCnt:(UINT) nRepCnt flags:(UINT)nFlags;
-(void)			OnMouseMove:(UINT)nFlags Points:(_Point)point;
-(void)			OnLButtonDown:(UINT)nFlags Points:(_Point)point;
-(void)			OnLButtonDblClk:(UINT)nFlags Points:(_Point)point;
-(void)			OnLButtonUp:(UINT)nFlags Points:(_Point)point;
-(void)			OnRButtonUp:(UINT)nFlags Points:(_Point)point;
-(void)			OnTimer:(UINT_PTR)nIDEvent;
-(void)			OnRedrawRect:(CPtrArray*)pArrRects AtOnce:(BOOL)bAtOnce;
@end

// ESFrameBase dialog
class ESFrameBase
{
// Construction
public:
	ESFrameBase();	// standard constructor
	virtual ~ESFrameBase();

	friend class MercuryBaseView;
	friend class MercuryBaseViewInfo;

//	operator HWND		() const {return m_hWnd;};
	bool				IsWindow				(){ return /*::IsWindow(m_hWnd) ? true : */false; };
	bool				IsWindowVisible			(){ return /*::IsWindowVisible(m_hWnd) ? true : */false; };
    void				SetHWND					(NSWindow* pWindow){m_hWnd = pWindow;};

	void				ShowWindow				(int nCmd);
	void				SetActiveWindow			();

	void				CloseWindow				();

	void				SetOwner				(ESFrameBase* pOwner);
	ESFrameBase*		GetOwner				();

	BOOL				GetClientRect			(_Rect& rcClient);
	BOOL				GetWindowRect			(_Rect& rcWindow);
	BOOL				ScreenToClient			(_Point& pt);
	BOOL				ScreenToClient			(_Rect& rect);
	BOOL				ClientToScreen			(_Point& pt);
	BOOL				ClientToScreen			(_Rect& rect);

	BOOL				SetWindowPos			(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT nFlags);
	BOOL				ModifyStyleEx			(DWORD dwStyleRemove, DWORD dwStyleAdd);
	BOOL				ModifyStyle				(DWORD dwStyleRemove, DWORD dwStyleAdd);
	DWORD				GetStyle				();
	DWORD				GetStyleEx				();

	UINT				SendMessage				(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	UINT				PostMessage				(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

	BOOL				SetTimer				(UINT_PTR nIDEvent, UINT nElapse);
	BOOL				KillTimer				(UINT_PTR nIDEvent);

	int					Create					();
	void				CloseDialog				(int nRetCode);
	static ESFrameBase*	FromHandle				(HWND hWnd);
	void				Invalidate				(BOOL eraseBkgnd = FALSE);
	void				InvalidateRect			(LPCRECTDef lpRect, BOOL eraseBkgnd = FALSE);

	// Get/Release device context.
	DCDef				GetDC					();
	void				ReleaseDC				(DCDef dc);

	bool				LoadWindowPlacement		(_Point ptPos, _Size szWindow);
	_Size				GetSizeMin				();
	_Size				GetSizeMax				();
	_Size				GetSizeDefault			();
	float				GetHaightWidthRatio		(){return m_fHeightAndWidthRatio;};
	int					GetBorderWidth			(){return m_nBorderWidth;};
	int					GetCaptionHeight		(){return m_nCaptionHeight;};
	_Size				GetCurrentSizeOmitBorder(_Point* pWindowPos = NULL);

protected:
	virtual BOOL		OnInitDialog			();
//	virtual	void		OnCancel				();
//	virtual	void		OnOK					();

	virtual bool		OnRegisterWindowClass	(HINSTANCE hInst, _string& wndClassName);

	virtual void		SetTitle				(){};
	virtual void		Setup					(){};

//	void				UpdatePopupMenuItems	(CMenu* pPopupMenu);
	void				CalcCaptionBorderSize	();

protected:
	static AutoSortedArray*	GetArrayWNDAttachments(){
		static AutoSortedArray arrWNDAttachments_;
		return &arrWNDAttachments_;
		}

	static bool				AttachWND				(HWND hWnd, ESFrameBase* pFrame);
	static ESFrameBase*		DetachWND				(HWND hWnd);

public:
	virtual void			OnPaint					(_DC *DC);
	virtual BOOL			OnEraseBkgnd			(_DC *pDC);
	virtual void			OnSysCommand			(UINT nID, LPARAM lParam);
	virtual void			OnDestroy				();
	virtual void			OnSize					(UINT nType, int cx, int cy);
	virtual void			OnClose					();
//	virtual void			OnGetMinMaxInfo			(MINMAXINFO* lpMMI);
	virtual LRESULT			OnNcHitTest				(_Point pt);
//	virtual void			OnWindowPosChanging		(LPWINDOWPOS lpWndPos);
	virtual BOOL			OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);

	virtual void			OnSetFocus				(HWND hWndFocusLost){};
	virtual void			OnKillFocus				(HWND hWndGainFocus){};
	virtual BOOL			OnMouseWheel			(UINT nFlags, short zDelta, _Point pt){return TRUE;};
	virtual BOOL			OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags){return TRUE;};

	virtual BOOL			OnChar					(UINT nChar, UINT nRepCnt, UINT nFlags){return TRUE;};
	virtual void			OnMouseMove				(UINT nFlags, _Point point){};
	virtual void			OnLButtonDown			(UINT nFlags, _Point point){};
	virtual void			OnLButtonDblClk			(UINT nFlags, _Point point){};
	virtual void			OnLButtonUp				(UINT nFlags, _Point point){};
	virtual void			OnRButtonUp				(UINT nFlags, _Point point){};
	virtual void			OnTimer					(UINT_PTR nIDEvent){};
	virtual void			OnRedrawRect			(CPtrArray*	pArrRects, BOOL	bAtOnce){};

protected:
//	HICON					m_hIcon;
	NSWindow*				m_hWnd;
	_string                 m_sWndClass;			// Window class name.

	ESFrameBase*			m_pOwner;

	_Size					m_szMin;
	_Size					m_szMax;
	_Size					m_szDefault;			// Default window size omit bordered.
	_Point					m_ptSetupPos;			// Default position of window.

	int						m_nBorderWidth;
	int						m_nCaptionHeight;

	float					m_fHeightAndWidthRatio;
	bool					m_bResizingFrame;
	UINT					m_nHitTestCode;
	_Rect					m_rcRestoredRect;
public:
};
