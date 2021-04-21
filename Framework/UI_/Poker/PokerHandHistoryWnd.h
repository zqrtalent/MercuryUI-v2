#pragma once

/////////////////////////////////////////////////////////////////////////////
// PokerHandHistoryWnd window

#include "..\..\Controls\MercuryBaseWnd.h"
#include "..\..\Protocol\_PokerDefines.h"
#include "..\..\Array\AutoSortedArrayTempl.h"
#include "..\..\Array\AutoSortedArray.h"
#include "..\..\Array\StringToPtrArray.h"
#include "..\..\PokerServer\Data\Tables\PokerAvatar.h"
#include "..\..\GameClient\PokerClient.h"
#include "..\..\Controls\ESChildControlList.h"
#include "PokerGUI.h"
#include <atlimage.h>

class ChildControl;
class PokerGUI;
class PokerSimpleHistory;

class PokerHandHistoryWnd : public MercuryBaseView
{
// Construction
public:
	PokerHandHistoryWnd();
	virtual ~PokerHandHistoryWnd();

	DECLARE_DYNCREATE(PokerHandHistoryWnd)
	
public: // Operations.
	void					SetTitle			(CString sTitle, bool bRedraw);
	int						AddHistoryInfo		(PokerSimpleHistory* pHistoryInfo, bool bClearExisting = true, bool bRedraw = true);

protected:
	virtual BOOL			Create				(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void			OnPaintClient		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	virtual BOOL			UseDoubleBufferedDC	(){return TRUE;};
	virtual void			OnPrePaint			(CRect& rcRepaint);

protected:
	bool					CreateChildControls	();

protected:
	PokerGUI*				m_pGUI;
	CString					m_sTitle;
	
	// Generated message map functions
protected:
	afx_msg void	OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnLButtonDblClk			(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown			(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp				(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove				(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor				(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT	OnGetDlgCode			();
	afx_msg BOOL	OnMouseActivate			(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg	void	OnSize					(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};