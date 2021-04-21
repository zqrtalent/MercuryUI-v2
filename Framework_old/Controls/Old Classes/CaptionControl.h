#pragma once


/////////////////////////////////////////////////////////////////////////////
// CaptionControl window

#include "MercuryBaseWnd.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

class CaptionControl : public MercuryBaseWnd
{
// Construction
public:
	CaptionControl();
	virtual ~CaptionControl();

public:
	virtual BOOL			Create				(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
												UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL			RegisterWndClass	(LPCTSTR lpszClassName);

protected:
	BOOL	DrawiTunesCaption	(CDC* pDC, CRect rcGradient);
	BOOL	DrawVGradient		(CDC* pDC, CRect rcGradient, COLORREF cr1, COLORREF cr2);
	void	DestroyClassMembers	();

protected:
	// pRectDC - rect in DC coordinates.
	// pRectClient - rect in client coordinates.
	virtual void	OnPaintClient	(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual void	OnPrePaint		(CRect rcRepaint);
	
	// Generated message map functions
protected:
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()
};