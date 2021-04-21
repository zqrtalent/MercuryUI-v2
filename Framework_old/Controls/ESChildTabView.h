#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildTabView window

#include "ESChildControlTabCtrl.h"

class ESChildTabView : public ESChildControl
{
// Construction
public:
	ESChildTabView();
	virtual ~ESChildTabView();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	VisibilityChanged		(bool bVisible);

	int				AddTabItem				(_string sTitle, int nWidth = 0, UINT nData = 0);
	bool			RemoveTabItem			(int nIndex);
	bool			RemoveAllTabItem		();
	bool			ActivateTabItem			(int nIndex, bool bRedraw = false);
	bool			SetTabItemTitle			(int nItem, _string sTitle, bool bRedraw);
	int				GetActiveItem			(){return m_pTabCtrl ?  m_pTabCtrl->GetActiveItem() : -1;};
	UINT			GetActiveItemData		(){return GetItemData(GetActiveItem());}
	UINT			GetItemData				(int nItem);

protected:
	virtual void	OnPaintClient           (_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_tabview");};
	virtual _Size	GetControlDefaultSize	(){return _Size(200, 250);};
	virtual bool	AllowToAddChildControl	(){return true;};

	virtual BOOL	Serialize					(GrowableMemory* pMem);
	virtual BOOL	SerializeWithVariableName	(GrowableMemory* pMem);

	virtual BOOL	Deserialize					(GrowableMemory* pMem);
	virtual BOOL	DeserializeWithVariableName	(GrowableMemory* pMem);

	virtual void	OnDeserializationCompleted();

protected:
	ESChildControlTabCtrl*	m_pTabCtrl;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildTabView();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

protected:
	int							m_nBorderWidth;
	COLORREF					m_crBorder;
	int							m_nSelectedTabIndex;
	ESTabCtrlItemInfoList		m_itemInfos;

	// Variable modify callbacks.
protected:
	static bool	ActiveTabItemModify	(ESChildTabView* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	ItemsModify			(ESChildControlTabCtrl* pThis, VariableInfo* pInfo, void* lpValueNew);

	// Child control event methods.
protected:
	void		OnTabItemChange		(ESChildControlTabCtrl* pSender, ChildControlEventCode eventCode, int nParam = 0);

};