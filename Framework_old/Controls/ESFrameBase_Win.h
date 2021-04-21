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

// ESFrameBase dialog
class ESFrameBase
{
// Construction
public:
	ESFrameBase();	// standard constructor
	virtual ~ESFrameBase();

	friend class MercuryBaseView;
	friend class MercuryBaseViewInfo;

	operator HWND		() const {return m_hWnd;};
	bool				IsWindow				(){ return ::IsWindow(m_hWnd) ? true : false; };
	bool				IsWindowVisible			(){ return ::IsWindowVisible(m_hWnd) ? true : false; };
	HWND				GetHWND					(){return m_hWnd;};

	void				ShowWindow				(int nCmd);
	void				SetActiveWindow			();

	void				CloseWindow				();

	void				SetOwner				(ESFrameBase* pOwner);
	ESFrameBase*		GetOwner				();

	BOOL				GetClientRect			(Rect& rcClient);
	BOOL				GetWindowRect			(Rect& rcWindow);
	BOOL				ScreenToClient			(Point& pt);
	BOOL				ScreenToClient			(Rect& rect);
	BOOL				ClientToScreen			(Point& pt);
	BOOL				ClientToScreen			(Rect& rect);

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
	Size				GetSizeMin				();
	Size				GetSizeMax				();
	Size				GetSizeDefault			();
	float				GetHaightWidthRatio		(){return m_fHeightAndWidthRatio;};
	int					GetBorderWidth			(){return m_nBorderWidth;};
	int					GetCaptionHeight		(){return m_nCaptionHeight;};
	Size				GetCurrentSizeOmitBorder(Point* pWindowPos = NULL);

protected:
	virtual BOOL		OnInitDialog			();
//	virtual	void		OnCancel				();
//	virtual	void		OnOK					();

	virtual bool		OnRegisterWindowClass	(HINSTANCE hInst, std::string& wndClassName);

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
	static LRESULT CALLBACK	_WndProc				(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void			OnPaint					(_DC *DC);
	virtual BOOL			OnEraseBkgnd			(_DC *pDC);
	virtual void			OnSysCommand			(UINT nID, LPARAM lParam);
	virtual void			OnDestroy				();
	virtual void			OnSize					(UINT nType, int cx, int cy);
	virtual void			OnClose					();
	virtual void			OnGetMinMaxInfo			(MINMAXINFO* lpMMI);
	virtual LRESULT			OnNcHitTest				(_Point pt);
	virtual void			OnWindowPosChanging		(LPWINDOWPOS lpWndPos);
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
	HICON					m_hIcon;
	HWND					m_hWnd;	
	std::string				m_sWndClass;			// Window class name.

	ESFrameBase*			m_pOwner;

	Size					m_szMin;
	Size					m_szMax;
	Size					m_szDefault;			// Default window size omit bordered.
	Point					m_ptSetupPos;			// Default position of window.

	int						m_nBorderWidth;
	int						m_nCaptionHeight;

	float					m_fHeightAndWidthRatio;
	bool					m_bResizingFrame;
	UINT					m_nHitTestCode;
	Rect					m_rcRestoredRect;
};
