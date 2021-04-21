#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlPropertyGrid window

#include "ESChildScrollView.h"

typedef struct ESPropertyGridItemTag{
	ESPropertyGridItemTag*	pParent;
	ESPropertyGridItemTag*	pNext;
	ESPropertyGridItemTag*	pPrev;
	ESPropertyGridItemTag*	pChild;
	_string				sTitle;
	Serializable*			pObject;
	VariableInfo*			pVarInfo;
	bool					bExpanded;
	} ESPropertyGridItem;

typedef void (__cdecl *DrawPropertyGridItemMethod) (ESPropertyGridItem*, _DC*, _Rect*, _Rect*, COLORREF, _Font*);
typedef void (__cdecl *ModifyPropertyValue) (ESPropertyGridItem*, ESChildControl*);

class ESChildComboBox;
class ESChildLookupBox;
class ESChildTextBox;

class ESChildControlPropertyGrid : public ESChildScrollView
{
// Construction
public:
	ESChildControlPropertyGrid();
	virtual ~ESChildControlPropertyGrid();

	virtual bool			Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool			SetPos					(int x, int y, bool bRedraw = true);
	virtual bool			SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool			Destroy					();
	virtual void			OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool			SetFocus				();
	virtual void			KillFocus				();

	void					SetPropertyTitleWidth	(int nWidth){m_nPropertyTitleWidth = nWidth;};
	bool					SetSerializableObject	(Serializable* pObject, bool bRedraw = true);
	bool					AddSerializableObject	(Serializable* pObject, bool bClearBeforeAdd = false, bool bRedraw = true);
	void					ClearAll				(bool bRedraw = true);
	bool					SetListOfObjects		(EnumerableObject<Serializable>* pListOfObjects, bool bRedraw = true);

protected:
	bool					SetSerializableObject_Rec(Serializable* pObject, ESPropertyGridItem*	pParent = NULL);	
	ESPropertyGridItem*		GetNextVisibleItem		(ESPropertyGridItem* pItem, int& nParentCt);
	int						GetParentCt				(ESPropertyGridItem* pItem);
	bool					GetItemByPoint			(_Point pt, ESPropertyGridItem* &pItem, _Rect& rcItem, bool& bValueSelection);
	bool					GetItemRect				(ESPropertyGridItem* pItem, _Rect& rcItem);
	static bool				GetPropertyValue		(ESPropertyGridItem* pItem, _string& sValue);
	static void*			GetPropertyValuePtr		(ESPropertyGridItem* pItem);
	void					DeletePropertyItem		(ESPropertyGridItem* pItem);
	int						GetVisibleItemCt		();
	void					CalcTopItem				();
	void					ExpandItem				(ESPropertyGridItem* pItem, bool bCollapse = false, bool bRedraw = false);
	ESChildControl*			CreateValueModifyControl(ESPropertyGridItem* pItem, _Rect rcControl);
	bool					RemoveValueModifyControl(bool bRedraw);

protected:
	virtual void			OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual BOOL			OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void			OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void			OnRButtonClick			(_Point pt, UINT nFlags);
	virtual	void			OnMButtonClick			(_Point pt, UINT nFlags);
	virtual	void			OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void			OnLButtonUp				(_Point pt, UINT nFlags);
	virtual	void			OnLButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void			OnRButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void			OnMButtonDblClick		(_Point pt, UINT nFlags);
	virtual void			OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool			OnScrollBarEvent		(bool bVScroll, int nCode, ESChildControl* pSender);

	// Designer virtual methods
	virtual _string		GetControlDefaultName	(){return _T("_propertyGrid");};
	virtual _Size			GetControlDefaultSize	(){return _Size(250, 250);};

	static	DrawPropertyGridItemMethod	DrawMethodsByVarType(int nVarType)
	{
		static  AutoSortedArray arrDrawMethodByType;
		if( !arrDrawMethodByType.GetCount() ){
			arrDrawMethodByType.Add((void*)VariableType::VT_Bool, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_String);
			arrDrawMethodByType.Add((void*)VariableType::VT_Int8, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Numeric);
			arrDrawMethodByType.Add((void*)VariableType::VT_Int16, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Numeric);
			arrDrawMethodByType.Add((void*)VariableType::VT_Int32, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Numeric);
			arrDrawMethodByType.Add((void*)VariableType::VT_Int64, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Numeric);
			arrDrawMethodByType.Add((void*)VariableType::VT_Float, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Numeric);
			arrDrawMethodByType.Add((void*)VariableType::VT_Double, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Numeric);
			arrDrawMethodByType.Add((void*)VariableType::VT_Enumerable, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Enum);
			arrDrawMethodByType.Add((void*)VariableType::VT_String, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_String);
			arrDrawMethodByType.Add((void*)VariableType::VT_Time, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_String);
			arrDrawMethodByType.Add((void*)VariableType::VT_Date, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_String);
			arrDrawMethodByType.Add((void*)VariableType::VT_DateTime, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_String);
			arrDrawMethodByType.Add((void*)VariableType::VT_Color32, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Color);
			arrDrawMethodByType.Add((void*)VariableType::VT_Point, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Point);
			arrDrawMethodByType.Add((void*)VariableType::VT_Size, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Size);
			arrDrawMethodByType.Add((void*)VariableType::VT_Rect, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Rect);
			arrDrawMethodByType.Add((void*)VariableType::VT_Font, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Font);
			arrDrawMethodByType.Add((void*)VariableType::VT_Image, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Image);
			arrDrawMethodByType.Add((void*)VariableType::VT_Binary, (void*)&ESChildControlPropertyGrid::DrawPropertyGridItem_Binary);
			}

		int nIndex = arrDrawMethodByType.IndexOf((void*)nVarType);
		if( nIndex == -1 )
			return NULL;
		return (DrawPropertyGridItemMethod)arrDrawMethodByType.GetData(nIndex);
	};

	bool					DrawPropertyGridItemByType	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	// Property grid item drawing methods by its type.
	static void				DrawPropertyGridItem_String	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Numeric(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Enum	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Color	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Image	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Size	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Point	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Rect	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Font	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);
	static void				DrawPropertyGridItem_Binary	(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont);


	static	ModifyPropertyValue	ModifyPropertyValueByType(int nVarType)
	{
		static  AutoSortedArray arrModifyPropertyValueByType;
		if( !arrModifyPropertyValueByType.GetCount() ){
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Bool, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Bool);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Int8, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Numeric);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Int16, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Numeric);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Int32, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Numeric);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Int64, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Numeric);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Float, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Numeric);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Double, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Numeric);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Enumerable, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Enum);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_String, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_String);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Time, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_String);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Date, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_String);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_DateTime, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_String);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Color32, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Color);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Point, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Point);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Size, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Size);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Rect, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Rect);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Font, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Font);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Image, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Image);
			arrModifyPropertyValueByType.Add((void*)VariableType::VT_Binary, (void*)&ESChildControlPropertyGrid::ModifyPropertyGridItem_Binary);
			}

		int nIndex = arrModifyPropertyValueByType.IndexOf((void*)nVarType);
		if( nIndex == -1 )
			return NULL;
		return (ModifyPropertyValue)arrModifyPropertyValueByType.GetData(nIndex);
	};

	bool					ModifyPropertyGridItemByType(ESPropertyGridItem* pItem, ESChildControl* pControl);
	// Property grid item value modify methods.
	static void				ModifyPropertyGridItem_Bool		(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_String	(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Numeric	(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Enum		(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Color	(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Image	(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Size		(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Point	(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Rect		(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Font		(ESPropertyGridItem* pItem, ESChildControl* pControl);
	static void				ModifyPropertyGridItem_Binary	(ESPropertyGridItem* pItem, ESChildControl* pControl);

	// Events
protected:
	void					OnComboBoxSelChange		(ESChildComboBox* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void					OnLookupEnumerable		(ESChildLookupBox* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void					OnImageClear			(ESChildTextBox* pSender, ChildControlEventCode eventCode, int nParam = 0);

protected:
	_Font*					m_pFontParentItem;
	_Font*					m_pFontChildItem;
	_Font*					m_pFontChildItemValue;

	_Font					m_fontParentItem;
	_Font					m_fontChildItem;
	_Font					m_fontChildItemValue;

	int						m_nItemCY;

	COLORREF				m_crParentItem;
	COLORREF				m_crParentItemText;
	COLORREF				m_crChildItem;
	COLORREF				m_crChildItemSel;
	COLORREF				m_crChildItemText;
	COLORREF				m_crChildItemBottomLine;
	COLORREF				m_crChildItemDividerLine;

	int						m_nPropertyTitleWidth;

	Serializable*			m_pObject;
	ESChildControl*			m_pValueModifyControl;

	ESPropertyGridItem*		m_pRootItem;
	ESPropertyGridItem*		m_pSelectedItem;
	ESPropertyGridItem*		m_pTopItem;
	int						m_nTopItemOffsetY;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlPropertyGrid();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};