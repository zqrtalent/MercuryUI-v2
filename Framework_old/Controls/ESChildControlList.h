#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlList window

#include "ESChildScrollView.h"
#include "../Data/VRowSet.h"

#define ITEM_STATE_BOLD			1
#define ITEM_STATE_UNDERLINE	2
#define ITEM_STATE_ITALIC		4

class ESListColumnInfo : public Serializable
{
public:
	ESListColumnInfo();
	virtual ~ESListColumnInfo();

public:
	_string				m_sField;
	_string				m_sTitle;
	int					m_nHAlign;
	int					m_nWidth;
	int					m_nWidthMin;
	float				m_fWidthPercent;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESListColumnInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

typedef bool (__cdecl *CustomizeRowCallback)(void* lpCallbackParam, VirtualRowSet* pRowSet, int nRow, bool bRowIsSelected, COLORREF& crBkColor, COLORREF& crText, bool& bBold);

class ESChildTextBox;
class ESChildControlList : public ESChildScrollView
{
// Construction
public:
	ESChildControlList();
	virtual ~ESChildControlList();

	virtual bool	Create						(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos						(int x, int y, bool bRedraw = true);
	virtual bool	SetSize						(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy						();
	virtual void	SetLogicalPos				(float fPosX, float fPosY);
	virtual	void	SetLogicalSize				(float fCX, float fCY);
	virtual void	OwnerWindowSizeChanged		(int cx, int cy);
	virtual bool	SetFocus					();
	virtual void	KillFocus					();

public:
	bool			AddColumn					(const _string sField, const _string sTitle, int nAlign, int nWidth, int nWidthMin, float fWidthPercent = 0.0f);
	bool			SetColumnWidth				(int nCol, int nWidth, bool bRedraw);
	bool			GotoRow						(int nRow, bool bRedraw = true);
	int				GetRowCount					(){ return m_pRowSet ? m_pRowSet->GetRowCount() : 0; };
	int				GetColumnCount				(){ return m_listColumns.GetCount(); };
	bool			SelectRow					(int nRow);
	int				GetCurrentRow				(){return m_nSelectedRow;};
	void			AllowColumnSort				(bool bAllow) { m_bAllowSort = bAllow; };
	int				GetSelectedRow				(){ return m_nSelectedRow; };

	int				FindRowByFieldValue_Int		(_string sField, int nFieldValue);

	// Cutomize row callback.
	void			SetRowCustomizeCallback		(CustomizeRowCallback callbackMethod, void* pCallBackParam){ m_callBackMethod = callbackMethod; m_pCallBackParam = pCallBackParam; };

	bool			SetRowSet					(VirtualRowSet* pRowSet, bool bRedraw, bool bClearSort = true);
	void			UpdateList					(bool bRedraw);

	int				GetTopRowIndex				();
	int				GetRowCountPerPage			();

	bool			EmptyList					(bool bRedraw);
	bool			DeleteRow					(int nRow, bool bRedraw = false);
	bool			AddEmptyRow					(bool bRedraw = false);

	int				GetCurrentFieldAsInt		(const TCHAR* sField, int nDefault = 0);			
	_string		GetCurrentFieldAsString		(const TCHAR* sField);
	int				GetFieldId					(int nCol);

	bool			SetItemBold					(int nItem, bool bBold);
	bool			SetItemUnderline			(int nItem, bool bUnderLine);
	bool			SetItemItalic				(int nItem, bool bItalic);

	void			AllowInPlaceEdit			(bool bAllow){m_bAllowInPlaceEdit = bAllow;};

	bool			SortByColumnIndex			(int nCol, bool bGotoSelectedRow, bool bRedraw);
	void			RedrawItem					(int nItem);

protected:
	virtual void	OnPaintClient				(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	void			DrawView					(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	void			DrawHeader					(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	int				GetHeaderColByPoint			(_Point pt, bool& bOverSizing);
	int				GetItemByPoint				(_Point pt, int& nCol);
	void			RedrawColumn				(int nCol);
	void			RedrawItemsView				();
	bool			GetColumnRect				(int nCol, _Rect& rRect);
	bool			GetItemRect					(int nItem, _Rect& rRect);
	bool			GetCellRect					(int nRow, int nCol, _Rect& rcCell);
	int				GetVisibleRowCt				(int* pOffsetY);
	bool			GetHeaderRect				(_Rect& rRect);
	bool			GetItemsViewRect			(_Rect& rRect);
	int				GetColumnWidth				(int nCol);
	int				GetColumnsWidth				();
	int				CalculateLeftCol			(int& nLeftColOffset);

	bool			IsItemBold					(int nItem);
	bool			IsItemUnderline				(int nItem);
	bool			IsItemItalic				(int nItem);

	virtual	void	OnMouseHover				(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave				(UINT nFlags);
	virtual void	OnMouseEnter				(UINT nFlags);
	virtual BOOL	OnSetCursor					(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick				(_Point pt, UINT nFlags);
	virtual	void	OnRButtonClick				(_Point pt, UINT nFlags);
	virtual	void	OnMButtonClick				(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown				(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp					(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDblClick			(_Point pt, UINT nFlags);
	virtual	void	OnRButtonDblClick			(_Point pt, UINT nFlags);
	virtual	void	OnMButtonDblClick			(_Point pt, UINT nFlags);
	virtual	void	OnMouseWheel				(UINT nFlags, short zDelta, _Point pt);
	virtual	void	OnKeyDown					(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void	OnLocalizeStrings			();
	virtual void	OnDeserializationCompleted	();

	bool			CreateInPlaceControl		(int nRow, int nCol);
	bool			RemoveInPlaceControl		(bool bRedraw);
	bool			GetCellValueAsText			(int nRow, int nCol, _string& sValue);

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_listctrl");};
	virtual _Size		GetControlDefaultSize	(){return _Size(250, 150);};

protected:
	VirtualRowSet*						m_pRowSet;
	EnumerableObject<ESListColumnInfo>	m_listColumns;

	int									m_nTopRowVisible;	// Visible top row index.

	ESChildControl*						m_pInPlaceControl;
	int									m_nRow_InPlaceControl;
	int									m_nCol_InPlaceControl;

	COLORREF							m_crBorder;
	COLORREF							m_crView;
	COLORREF							m_crRow1;
	COLORREF							m_crRow2;
	COLORREF							m_crSelectedRow;
	COLORREF							m_crSelectedRowText;
	COLORREF							m_crDefaultRowText;

	bool								m_bHeader;
	COLORREF							m_crHeaderText;

	int									m_nHeaderCY;
	Image								m_imgHeaderDefault;
	Image*								m_pHdrItemDefault;
	Image*								m_pHdrItemSelected;
	Image*								m_pHdrItemDivider;

	Font								m_normalItemTextFont;
	Font								m_selectedItemTextFont;
	Font								m_headerTextFont;

	// ============================================
	int									m_nSelectedRow;

	bool								m_bAllowSort;
	bool								m_bAllowInplaceEdit;

	bool								m_bColumnSizing;
	int									m_nColumnSizing;
	bool								m_bUnderColSizing;
	int									m_nColumnUnder;
	
	bool								m_bAllowInPlaceEdit;
	int									m_nSortCol;	// Sorted column index
	bool								m_bAsc;

	// Row customization.
	CustomizeRowCallback				m_callBackMethod;
	void*								m_pCallBackParam;

protected:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlList();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Control events.
protected:
	void			OnInPlaceControlOk		(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void			OnInPlaceControlCancel	(ESChildControl* pSender, ChildControlEventCode eventCode, int nParam = 0);

	// Variable modify callbacks.
protected:
	static bool	ColumnsModify				(ESChildControlList* pThis, VariableInfo* pInfo, void* lpValueNew);
};