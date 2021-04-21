#pragma once


/////////////////////////////////////////////////////////////////////////////
// ControlContainer window

#include "MercuryBaseWnd.h"
#include "..\Array\AutoSortedArrayTempl.h"
#include <atlimage.h>

class ControlContainer : public MercuryBaseWnd
{
// Construction
public:
	ControlContainer();
	virtual ~ControlContainer();

public:
	virtual BOOL			Create				(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
												UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL			RegisterWndClass	(LPCTSTR lpszClassName);

protected:
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