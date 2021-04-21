#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildSlider window

#include "ESChildControl.h"

class ESChildSlider : public ESChildControl
{
// Construction
public:
	ESChildSlider();
	virtual ~ESChildSlider();

	virtual bool	Create			(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos			(int x, int y, bool bRedraw = true);
	virtual bool	SetSize			(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy			();

	bool			SetupSlider		(int nMin, int nMax, int nStep, bool bInvalidate = true);
	bool			SetSliderPos	(int nPos, bool bInvalidate = true, bool bReflectEvent = true);
	int				GetSliderPos	() { return m_nPos; };
	int				GetSliderPosMin	() { return m_nMin; };
	int				GetSliderPosMax	() { return m_nMax; };
	int				GetSliderStep	() { return m_nStep;};

	void			AllowCursorDrag	(bool bAllowDrag){m_bAllowDrag = bAllowDrag;};
	void			AllowJumpToClickedPos(bool bAllow){m_bJumpToClickedPos = bAllow;}

	enum SliderHitTest
	{
		NoSign = 0,
		PlusSign = 1,
		MinusSign,
		CursorSign
	};

	enum 
	{
		MinusSlideTimerId = 13000,
		PlusSlideTimerId
	};

protected:
	virtual void	OwnerWindowSizeChanged(int cx, int cy);
	virtual void	OnPaintClient		(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	virtual	void	OnMouseHover		(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave		(UINT nFlags);
	virtual void	OnMouseEnter		(UINT nFlags);
	virtual BOOL	OnSetCursor			(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick		(_Point pt, UINT nFlags);
	virtual	void	OnRButtonClick		(_Point pt, UINT nFlags);
	virtual	void	OnMButtonClick		(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown		(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDblClick	(_Point pt, UINT nFlags);
	virtual	void	OnRButtonDblClick	(_Point pt, UINT nFlags);
	virtual	void	OnMButtonDblClick	(_Point pt, UINT nFlags);
	virtual void	OnTimer				(UINT nIDEvent);

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_slider");};
	virtual _Size	GetControlDefaultSize	(){return _Size(150, 30);};

protected:
	void			CalRects				();
	bool			CalcCursorPosByPoint	(_Point pt);
	bool			CalcCursorRect			(_Rect rcBar, _Rect& rcCursor);
	void			AdjustAttributesByVerticalMode(bool bVerticalMode);

protected:
	// UI parameters. {{
	Image			m_imageSliderBarFilled;
	_Rect			m_rcOmitBarFilled;
	Image			m_imageSliderBarNotFilled;
	_Rect			m_rcOmitBarNotFilled;

	Image			m_imageSliderBarFilled_Scale;
	_Rect			m_rcOmitBarFilled_Scale;
	Image			m_imageSliderBarNotFilled_Scale;
	_Rect			m_rcOmitBarNotFilled_Scale;

	Image			m_imageCursor;
	Image			m_imageCursorPushed;
	Image			m_imagePlusButtonNormal;
	Image			m_imagePlusButtonPushed;
	Image			m_imageMinusButtonNormal;
	Image			m_imageMinusButtonPushed;

	int				m_nDistBetweenBarAndButton;
	bool			m_bVerticalMode;
	float			m_fZoom;
	// }}

	bool			m_bMouseDown;
	int				m_nPos;
	int				m_nMin;
	int				m_nMax;
	int				m_nStep;
	_Point			m_ptCursorPos;

	bool			m_bJumpToClickedPos;

	bool			m_bHandCursor;
	SliderHitTest	m_hitTest;
	bool			m_bCursorCaptured;

	bool			m_bAllowDrag;

	_Rect			m_rcBar;
	_Rect			m_rcMinus;
	_Rect			m_rcPlus;
	_Rect			m_rcCursor;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildSlider();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	VerticalModeModify	(ESChildSlider* pThis, VariableInfo* pInfo, void* lpValueNew);
};