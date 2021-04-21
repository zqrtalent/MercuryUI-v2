#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabCtrl window

#include "ESChildControl.h"

class ESTabCtrlItemInfo : public Serializable
{
public:
	ESTabCtrlItemInfo();
	virtual ~ESTabCtrlItemInfo();

public:
	_string				m_sTitle;
	_string				m_sTitleLocal;	// Localized title text.
	int						m_nData;
	int						m_nWidth;
	float					m_fWidthPercent;
	void*					m_pParam;

	// Item image resources.
	_string				m_sImageDefault;
	_string				m_sImageHover;
	_string				m_sImagePressed;

	_string				m_sImageActiveDefault;
	_string				m_sImageActiveHover;
	_string				m_sImageActivePressed;

	// Image instances.
	Image*					m_pImageDefault;
	Image*					m_pImageHover;
	Image*					m_pImagePressed;

	Image*					m_pImageActiveDefault;
	Image*					m_pImageActiveHover;
	Image*					m_pImageActivePressed;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESTabCtrlItemInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESTabItemLinkedInfo : public Serializable
{
public:
	ESTabItemLinkedInfo();
	virtual ~ESTabItemLinkedInfo();

public:
	int				m_nItemIndex;
	_string		m_sLinkedControl; // Linked control name. (TabView)
public:
	virtual Serializable*	CreateSerializableObject	(){return new ESTabItemLinkedInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESTabCtrlItemInfoList : public Serializable
{
public:
	ESTabCtrlItemInfoList();
	virtual ~ESTabCtrlItemInfoList();

public:
	EnumerableObject<ESTabCtrlItemInfo>		m_listItems;
	EnumerableObject<ESTabItemLinkedInfo>	m_linkedInfos;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESTabCtrlItemInfoList();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESChildControlTabCtrl : public ESChildControl
{
// Construction
public:
	ESChildControlTabCtrl();
	virtual ~ESChildControlTabCtrl();

	friend class ESChildTabView;

	virtual bool	Create						(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos						(int x, int y, bool bRedraw = true);
	virtual bool	SetSize						(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy						();
	virtual void	OwnerWindowSizeChanged		(int cx, int cy);
	virtual bool	MouseUpDownEventsInDesigner	(){return true;};

	int				AddTabItem					(_string sTitle, int nWidth = 0, UINT nData = 0, void* pParam = NULL);
	bool			RemoveTabItem				(int nIndex);
	bool			RemoveAllTabItem			();
	bool			ActivateTabItem				(int nIndex, bool bRedraw = false);
	bool			SetTabItemTitle				(int nItem, _string sTitle, bool bRedraw);
	int				GetActiveItem				(){return m_nActiveItem;};
	UINT			GetActiveItemData			(){return GetItemData(GetActiveItem());}
	UINT			GetItemData					(int nItem);
	void*			GetItemParam				(int nItem);
	bool			SetItemParam				(int nItem, void* pParam);
	int				GetItemCt					(){return m_listItems.GetCount();};
	bool			RedrawItem					(int nItem);
	int				FindItemByItemData			(UINT nData);
	bool			GetItemRect					(int nItem, _Rect& rcItem);

protected:
	virtual void	OnPaintClient				(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

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
	virtual bool	RemainCapturedOutOfClientArea(){return false;};

	virtual void	OnLocalizeStrings			();

	// Designer virtual methods
	virtual _string	GetControlDefaultName       (){return _T("_tabctrl");};
	virtual _Size	GetControlDefaultSize		(){return _Size(200, 30);};

protected:
	void			LoadItemImages				();
	int				GetItemByPoint				(_Point pt);
	bool			RedrawItems					();

protected:
	int									m_nActiveItem;
	int									m_nItemUnderCursor;
	EnumerableObject<ESTabCtrlItemInfo>	m_listItems;
	bool								m_bUseHandCursor;

	// Custom item images. {{
	Image								m_imgItemNormal;
	Image								m_imgItemHover;
	Image								m_imgItemActive;
	// }}

	// Custom item image files. {{
	_string							m_sImgFileItemNormal;
	_string							m_sImgFileItemHover;
	_string							m_sImgFileItemActive;
	// }}

	Image*								m_pImageItemNormal;
	Image*								m_pImageItemHover;
	Image*								m_pImageItemActive;

	_Rect								m_rcItemOmitBorder;
	Font								m_tabItemFont;
	Font								m_tabItemActiveFont;

	COLORREF							m_crTabItemText;
	COLORREF							m_crActiveTabItemText;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlTabCtrl();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	ItemsModify				(ESChildControlTabCtrl* pThis, VariableInfo* pInfo, void* lpValueNew);
};