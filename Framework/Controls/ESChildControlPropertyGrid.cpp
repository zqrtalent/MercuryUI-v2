// ESChildControlPropertyGrid.cpp : implementation file
//

#include "ESChildControlPropertyGrid.h"
#include "ESChildTextBox.h"
#include "ESChildComboBox.h"
#include "ESChildScrollBar.h"
#include "ESChildOpenFileBox.h"
#include "ESChildOpenFontDialogBox.h"
#include "ESChildOpenColorDialogBox.h"
#include "ESChildLookupBox.h"

#include "ESEnumerableModifyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildControlPropertyGrid

ESChildControlPropertyGrid::ESChildControlPropertyGrid() : ESChildScrollView(){
	m_sClassName			= _T("ESChildControlPropertyGrid");
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	_tcscpy_(lf.lfFaceName, _T("Arial"));
	lf.lfWeight				= FW_MEDIUM;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfQuality			= ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	lf.lfHeight				= 12;
	m_fontParentItem.CreateFontIndirect(&lf);
	m_pFontParentItem = &m_fontParentItem;

	lf.lfWeight				= FW_BOLD;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfQuality			= ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	lf.lfHeight				= 12;
	m_fontChildItem.CreateFontIndirect(&lf);
	m_pFontChildItem = &m_fontChildItem;

	_tcscpy_(lf.lfFaceName, _T("Arial"));
	lf.lfWeight				= FW_NORMAL;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfItalic				= 0;
	lf.lfUnderline			= 0;
	lf.lfStrikeOut			= 0;
	lf.lfQuality			= ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily		= DEFAULT_PITCH;
	lf.lfHeight				= 12;
	m_fontChildItemValue.CreateFontIndirect(&lf);
	m_pFontChildItemValue	= &m_fontChildItemValue;

	m_nLineHeight			= 24;
	m_nItemCY				= m_nLineHeight;
	m_crBorder				= RGB(102, 102, 102);
	m_bFillWithColor		= false;
	m_nBorderWidth			= 0;
	m_nPropertyTitleWidth	= 150;

	m_crParentItem			= RGB(190, 190, 190);
	m_crParentItemText		= RGB(0, 0, 0);
	m_crChildItem			= RGB(255, 255, 255);
	m_crChildItemSel		= RGB(200, 200, 200);
	m_crChildItemText		= RGB(0, 0, 0);
	m_crChildItemBottomLine	= RGB(200, 200, 200);
	m_crChildItemDividerLine= RGB(200, 200, 200);

	m_pRootItem				= new ESPropertyGridItem();
	m_pRootItem->pParent	= NULL;
	m_pRootItem->pNext		= NULL;
	m_pRootItem->pPrev		= NULL;
	m_pRootItem->pChild		= NULL;

	m_pSelectedItem			= NULL;
	m_pTopItem				= NULL;
	m_nTopItemOffsetY		= 0;
	m_pValueModifyControl	= NULL;
	}

BEGIN_DECLARE_VAR(ESChildControlPropertyGrid, ESChildControl)
	DECLARE_VAR(_T("ESChildControlPropertyGrid"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Parent Item Font"),					_T(""), VariableType::VT_Font, offsetof(ESChildControlPropertyGrid,m_fontParentItem), false, false, false)
	DECLARE_VAR(_T("Child Item Font"),					_T(""), VariableType::VT_Font, offsetof(ESChildControlPropertyGrid,m_fontChildItem), false, false, false)
	DECLARE_VAR(_T("Child Item Value"),					_T(""), VariableType::VT_Font, offsetof(ESChildControlPropertyGrid,m_fontChildItemValue), false, false, false)	
	DECLARE_VAR(_T("Item Height"),						_T(""), VariableType::VT_Int32, offsetof(ESChildControlPropertyGrid,m_nItemCY), false, false, false)
	DECLARE_VAR(_T("PropetyTitleWidth"),				_T(""), VariableType::VT_Int32, offsetof(ESChildControlPropertyGrid,m_nPropertyTitleWidth), false, false, false)
	DECLARE_VAR(_T("Parent Item Color"),				_T(""), VariableType::VT_Color32, offsetof(ESChildControlPropertyGrid,m_crParentItem), false, false, false)
	DECLARE_VAR(_T("Parent Item Text Color"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlPropertyGrid,m_crParentItemText), false, false, false)
	DECLARE_VAR(_T("Child Item Color"),					_T(""), VariableType::VT_Color32, offsetof(ESChildControlPropertyGrid,m_crChildItem), false, false, false)
	DECLARE_VAR(_T("Child Item Selected Color"),		_T(""), VariableType::VT_Color32, offsetof(ESChildControlPropertyGrid,m_crChildItemSel), false, false, false)
	DECLARE_VAR(_T("Child Item Text Color"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlPropertyGrid,m_crChildItemText), false, false, false)
	DECLARE_VAR(_T("Child Item Bottom Line"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlPropertyGrid,m_crChildItemBottomLine), false, false, false)
	DECLARE_VAR(_T("Child Item Divider Line"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlPropertyGrid,m_crChildItemDividerLine), false, false, false)
END_DECLARE_VAR()

ESChildControlPropertyGrid::~ESChildControlPropertyGrid(){
	if( m_pFontParentItem )
		m_pFontParentItem->DeleteObject();
	if( m_pFontChildItem )
		m_pFontChildItem->DeleteObject();
	if( m_pFontChildItemValue )
		m_pFontChildItemValue->DeleteObject();

	// Delete property items.
	DeletePropertyItem(m_pRootItem);
	}

bool
ESChildControlPropertyGrid::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ESChildScrollView::Create(nId, rRect, pOwner, pParent);
	return true;
	}

bool
ESChildControlPropertyGrid::SetPos(int x, int y, bool bRedraw){
	ESChildScrollView::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlPropertyGrid::SetSize(int cx, int cy, bool bRedraw){
	ESChildScrollView::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESChildControlPropertyGrid::Destroy(){
	ESChildScrollView::Destroy();
	return true;
	}

void
ESChildControlPropertyGrid::OwnerWindowSizeChanged(int cx, int cy){
	ESChildScrollView::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildControlPropertyGrid::SetFocus(){
	return true;
	}

void
ESChildControlPropertyGrid::KillFocus(){
	}

bool
ESChildControlPropertyGrid::SetSerializableObject(Serializable* pObject, bool bRedraw /*= true*/){
	if( m_pObject == pObject )
		return true;

	m_pSelectedItem		= NULL;
	m_pTopItem			= NULL;
	m_nTopItemOffsetY	= 0;

	// Remove modify control.
	RemoveValueModifyControl(false);

	// Delete property items.
	DeletePropertyItem(m_pRootItem->pChild);
	m_pRootItem->pChild = NULL;

	m_pObject	= pObject;
	SetSerializableObject_Rec(pObject, m_pRootItem);

	_Rect rcClient = GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	m_szView.SetSize(rcClient.Width(), GetVisibleItemCt()*m_nLineHeight);
	m_szPage.SetSize(rcClient.Width(), rcClient.Height());
	AdjustScrollBarPos(false);

	_Rect rcOwner;
	m_pOwner->GetClientRectMy(rcOwner);
	OwnerWindowSizeChanged(rcOwner.Width(), rcOwner.Height());

	if( !m_pTopItem ){
		m_pTopItem			= m_pRootItem->pChild;
		m_nTopItemOffsetY	= 0;	
		}

	if( bRedraw )
		Redraw();
	return true;
	}

bool
ESChildControlPropertyGrid::AddSerializableObject(Serializable* pObject, bool bClearBeforeAdd /*= false*/, bool bRedraw /*= true*/){
	if( !pObject )
		return false;

	if( bClearBeforeAdd )
		ClearAll(false);

	SetSerializableObject_Rec(pObject, m_pRootItem);

	_Rect rcClient = GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	m_szView.SetSize(rcClient.Width(), GetVisibleItemCt()*m_nLineHeight);
	m_szPage.SetSize(rcClient.Width(), rcClient.Height());
	AdjustScrollBarPos(false);

	_Rect rcOwner;
	m_pOwner->GetClientRectMy(rcOwner);
	OwnerWindowSizeChanged(rcOwner.Width(), rcOwner.Height());

	if( !m_pTopItem ){
		m_pTopItem			= m_pRootItem->pChild;
		m_nTopItemOffsetY	= 0;	
		}

	if( bRedraw )
		Redraw();
	return true;
	}

void
ESChildControlPropertyGrid::ClearAll(bool bRedraw /*= true*/){
	m_pSelectedItem		= NULL;
	m_pTopItem			= NULL;
	m_nTopItemOffsetY	= 0;

	// Remove modify control.
	RemoveValueModifyControl(false);

	// Delete property items.
	DeletePropertyItem(m_pRootItem->pChild);
	m_pRootItem->pChild = NULL;

	_Rect rcClient = GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	m_szView.SetSize(rcClient.Width(), GetVisibleItemCt()*m_nLineHeight);
	m_szPage.SetSize(rcClient.Width(), rcClient.Height());
	AdjustScrollBarPos(false);

	_Rect rcOwner;
	m_pOwner->GetClientRectMy(rcOwner);
	OwnerWindowSizeChanged(rcOwner.Width(), rcOwner.Height());

	if( bRedraw )
		Redraw();
	}

bool
ESChildControlPropertyGrid::SetListOfObjects(EnumerableObject<Serializable>* pListOfObjects, bool bRedraw /*= true*/){
	if( !pListOfObjects )
		return true;

	m_pSelectedItem		= NULL;
	m_pTopItem			= NULL;
	m_nTopItemOffsetY	= 0;

	// Remove modify control.
	RemoveValueModifyControl(false);

	// Delete property items.
	DeletePropertyItem(m_pRootItem->pChild);
	m_pRootItem->pChild = NULL;

	for(int i=0; i<pListOfObjects->GetCount(); i++){
		Serializable* pObject	= pListOfObjects->GetAt(i);
		SetSerializableObject_Rec(pObject, m_pRootItem);
		}

	_Rect rcClient = GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	m_szView.SetSize	(rcClient.Width(), GetVisibleItemCt()*m_nLineHeight);
	m_szPage.SetSize	(rcClient.Width(), rcClient.Height());
	AdjustScrollBarPos	(false);

	_Rect rcOwner;
	m_pOwner->GetClientRectMy(rcOwner);
	OwnerWindowSizeChanged(rcOwner.Width(), rcOwner.Height());

	if( !m_pTopItem ){
		m_pTopItem			= m_pRootItem->pChild;
		m_nTopItemOffsetY	= 0;	
		}

	if( bRedraw )
		Redraw();
	return true;
	}

bool
ESChildControlPropertyGrid::SetSerializableObject_Rec(Serializable* pObject, ESPropertyGridItem* pParent /*= NULL*/){
	if( !pParent )
		pParent = m_pRootItem;

	VariableInfo*	ppinfo[10];
	int				nMainLoop		= pObject->GetVariableInfos(ppinfo);
	nMainLoop --;

	while( nMainLoop > -1 ){
		VariableInfo*		pVarInfo	= ppinfo[nMainLoop];
		int					nLoop		= 1;
		ESPropertyGridItem* pPrev		= pParent->pChild;
		ESPropertyGridItem* pFirst		= NULL;

		if( pPrev ){
			while( pPrev->pNext )
				pPrev = pPrev->pNext;
			}

		ESPropertyGridItem* pParentNew	= NULL;
		if( pParent == m_pRootItem ){
			pParentNew					= new ESPropertyGridItem();
			pParentNew->bExpanded		= true;
			pParentNew->pParent			= pParent;
			pParentNew->pNext			= NULL;
			pParentNew->pPrev			= pPrev;
			pParentNew->pChild			= NULL;
			pParentNew->sTitle			= pVarInfo[0].m_sAttributeName;
			pParentNew->pObject			= pObject;
			pParentNew->pVarInfo		= pVarInfo;

			if( !m_pRootItem->pChild )
				m_pRootItem->pChild = pParentNew;
			}
		else
			pParentNew = pParent;

		if( pPrev )
			pPrev->pNext = pParentNew;

		while( pVarInfo[nLoop].m_sAttributeName.length() > 0 ){
			ESPropertyGridItem* pItemNew= new ESPropertyGridItem();
			pItemNew->bExpanded			= false;
			pItemNew->pParent			= pParentNew;
			pItemNew->pNext				= NULL;
			pItemNew->pPrev				= pPrev;
			pItemNew->pChild			= NULL;
			pItemNew->sTitle			= pVarInfo[nLoop].m_sAttributeName;
			pItemNew->pObject			= pObject;
			pItemNew->pVarInfo			= &pVarInfo[nLoop];

			if( pVarInfo[nLoop].m_bComplexType ){
				if( pVarInfo[nLoop].m_bEnumerable ){
					}
				else{
					Serializable* pObj = (Serializable*)&((BYTE*)pObject)[pVarInfo[nLoop].m_nOffset];
					SetSerializableObject_Rec(pObj, pItemNew);
					}
				}

			if( !pFirst )
				pFirst = pItemNew;
			else
				pPrev->pNext = pItemNew;
		
			pPrev = pItemNew;
			nLoop ++;
			}

		if( !pParentNew->pChild )
			pParentNew->pChild = pFirst;

		nMainLoop --;
		}
	return true;
	}

ESPropertyGridItem*
ESChildControlPropertyGrid::GetNextVisibleItem(ESPropertyGridItem* pItem, int& nParentCt){
	if( !pItem ) return NULL;

	if( pItem->pChild && pItem->bExpanded ){
		nParentCt ++;
		return pItem->pChild;
		}

	if( pItem->pNext )
		return pItem->pNext;

	if( pItem->pParent ){
		pItem = pItem->pParent;
		nParentCt --;
		return pItem->pNext;
		}

	return NULL;
	}

int	
ESChildControlPropertyGrid::GetParentCt(ESPropertyGridItem* pItem){
	if( !pItem ) return 0;
	int nCt = -1;
	while( pItem->pParent ){
		nCt ++;
		pItem = pItem->pParent;
		}
	return nCt;
	}

bool
ESChildControlPropertyGrid::GetItemByPoint(_Point pt, ESPropertyGridItem* &pItem, _Rect& rcItem, bool& bValueSelection){
	if( !m_pTopItem ) return false;

	_Rect				rcClient	= GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	pt.y							-= (rcClient.top - m_nTopItemOffsetY);
	pItem							= m_pTopItem;
	int					nParentCt	= 0;

	rcItem.left		= rcClient.left;
	rcItem.top		= rcClient.top - m_nTopItemOffsetY;
	rcItem.right	= rcClient.right - (m_pVScroll->IsVisible() ? m_nArrowBtnCX : 0);
	rcItem.bottom	= rcItem.top + m_nItemCY;

	while( pItem ){
		if( pt.y < m_nItemCY ){
			if( pt.x > (rcItem.left + m_nPropertyTitleWidth) )
				bValueSelection = true;
			else
				bValueSelection = false;
			return true;
			}
		
		rcItem.top		+= m_nItemCY;
		rcItem.bottom	+= m_nItemCY;
		pt.y			-= m_nItemCY;
		pItem			= GetNextVisibleItem(pItem, nParentCt);
		}
	return false;
	}

bool
ESChildControlPropertyGrid::GetItemRect(ESPropertyGridItem* pItem, _Rect& rcItem){
	if( !m_pTopItem || !pItem ) return false;

	_Rect				rcClient	= GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	ESPropertyGridItem*	pItemSearch	= m_pTopItem;
	int					nParentCt	= 0;

	rcItem.left		= rcClient.left;
	rcItem.top		= rcClient.top - m_nTopItemOffsetY;
	rcItem.right	= rcClient.right - (m_pVScroll->IsVisible() ? m_nArrowBtnCX : 0);
	rcItem.bottom	= rcItem.top + m_nItemCY;

	while( pItemSearch ){
		if( rcItem.top >= rcClient.bottom )
			return false;
		
		if( pItem == pItemSearch )
			return true;

		rcItem.top		+= m_nItemCY;
		rcItem.bottom	+= m_nItemCY;
		pItemSearch		= GetNextVisibleItem(pItemSearch, nParentCt);
		}
	return false;
	}

bool
ESChildControlPropertyGrid::GetPropertyValue(ESPropertyGridItem* pItem, _string& sValue){
	if( !pItem || !pItem->pObject || pItem->pChild ) return false;
	int nFieldId = pItem->pObject->GetFieldIndex(pItem->pVarInfo->m_sAttributeName);
	return pItem->pObject->GetFieldAsString(nFieldId, sValue);
	}

void*
ESChildControlPropertyGrid::GetPropertyValuePtr(ESPropertyGridItem* pItem){
	if( !pItem || !pItem->pObject || pItem->pChild ) return NULL;
	return pItem->pObject->GetVariablePtr(pItem->pVarInfo->m_sAttributeName);
	}

void
ESChildControlPropertyGrid::DeletePropertyItem(ESPropertyGridItem* pItem){
	while(pItem){
		if( pItem->pChild )
			DeletePropertyItem(pItem->pChild);
		ESPropertyGridItem* pItemNext = pItem->pNext;
		delete pItem;
		pItem = pItemNext;
		}
	}

int
ESChildControlPropertyGrid::GetVisibleItemCt(){
	int					nCt			= 0, nParentCt	= 0;
	ESPropertyGridItem* pItem		= GetNextVisibleItem(m_pRootItem, nParentCt);
	while( pItem ){
		pItem = GetNextVisibleItem(pItem, nParentCt); nCt ++;
		}
	return nCt;
	}

void
ESChildControlPropertyGrid::CalcTopItem(){
	int					nPos		= m_pVScroll->IsVisible() ? m_pVScroll->GetScrollPos() : 0, nCt	= 0, nParentCt = 0;
	ESPropertyGridItem* pItem		= GetNextVisibleItem(m_pRootItem, nParentCt);
	while( pItem ){
		nPos	-= m_nItemCY;
		if( nPos < 0 ){
			m_pTopItem			= pItem;
			m_nTopItemOffsetY	= (m_nItemCY + nPos)%m_nItemCY;
			return;
			}

		pItem	= GetNextVisibleItem(pItem, nParentCt);
		nCt		++;
		}
	}

void
ESChildControlPropertyGrid::ExpandItem(ESPropertyGridItem* pItem, bool bCollapse /*= false*/, bool bRedraw /*= false*/){
	if( !pItem || !pItem->pChild )
		return;
	if( pItem->bExpanded == !bCollapse )
		return;

	pItem->bExpanded = !bCollapse;

	_Rect rcClient = GetClientRect();
	rcClient.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
	m_szView.SetSize(rcClient.Width(), GetVisibleItemCt()*m_nLineHeight);
	m_szPage.SetSize(rcClient.Width(), rcClient.Height());
	AdjustScrollBarPos(false);
	CalcTopItem();

	if( bRedraw )
		Redraw(true);
	}

ESChildControl*
ESChildControlPropertyGrid::CreateValueModifyControl(ESPropertyGridItem* pItem, _Rect rcControl){
	if( !pItem || !pItem->pVarInfo || !pItem->pObject ) return NULL;
	ESChildControl* pControl	= NULL;
	VariableInfo*	pVarInfo	= pItem->pVarInfo;
	void*			lpValuePtr	= &((BYTE*)pItem->pObject)[pVarInfo->m_nOffset];
	_string			sValue;

	if( pVarInfo->m_bEnumerable && pVarInfo->m_bComplexType ){
		ESChildLookupBox* pLookupBox = new ESChildLookupBox();
		pControl = pLookupBox;
		if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
			delete pControl;
			return NULL;
			}
		else{
			pLookupBox->AddEventListener((ESFrameBase*)this, ChildControlEventCode::EventCode_Lookup, (ControlEventListener)&ESChildControlPropertyGrid::OnLookupEnumerable);
			pLookupBox->SetTextFont		(m_pFontChildItemValue);

			EnumerableObject<Serializable>* pEnumerable = (EnumerableObject<Serializable>*)lpValuePtr;
			stringEx::Format(sValue, _T("{%s}"), pEnumerable->GetClassName().c_str());
			pLookupBox->SetText(sValue);
			}
		return pControl;
		}

	switch( pVarInfo->m_type ){
		case VariableType::VT_Bool:
			{
				ESChildComboBox* pComboBox = new ESChildComboBox();
				pControl = pComboBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				else{
					pComboBox->SetTextFont	(m_pFontChildItemValue);
					pComboBox->AddItem		(_T("Yes"), (void*)1);
					pComboBox->AddItem		(_T("No"), (void*)0);
					pComboBox->SetCurSel	((*((bool*)lpValuePtr) ? 0 : 1), false);
					}
				break;
			}
		case VariableType::VT_Enumerable:
			{
				ESChildComboBox* pComboBox = new ESChildComboBox();
				pControl = pComboBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				else{
					pComboBox->SetTextFont	(m_pFontChildItemValue);
					StringToIntArray* pArr = (StringToIntArray*)lpValuePtr;
					for(int i=0; i<pArr->GetCount(); i++){
						pComboBox->AddItem(pArr->GetKey(i), (void*)pArr->GetValueByIndex(i));
						}
					pComboBox->SetCurSel(pArr->GetCurrentIndex(), false);
					}
				break;
			}
		case VariableType::VT_Int8:
		case VariableType::VT_Int16:
		case VariableType::VT_Int32:
		case VariableType::VT_Int64:
			{
				ESChildTextBox* pTextBox = new ESChildTextBox();
				pControl = pTextBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}

				pTextBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pTextBox->SetTextFont			(m_pFontChildItemValue);
				stringEx::Format(sValue, _T("%d"), *((int*)lpValuePtr));
				pTextBox->SetText(sValue);
				break;
			}
		case VariableType::VT_Float:
			{
				ESChildTextBox* pTextBox = new ESChildTextBox();
				pControl = pTextBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				stringEx::Format(sValue, _T("%.4f"), *((float*)lpValuePtr));
				pTextBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pTextBox->SetTextFont			(m_pFontChildItemValue);
				pTextBox->SetText				(sValue);
				break;
			}
		case VariableType::VT_Double:
			{
				ESChildTextBox* pTextBox = new ESChildTextBox();
				pControl = pTextBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				stringEx::Format(sValue, _T("%.4f"), *((double*)lpValuePtr));
				pTextBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pTextBox->SetTextFont			(m_pFontChildItemValue);
				pTextBox->SetText				(sValue);
				break;
			}
		case VariableType::VT_String:
		case VariableType::VT_DateTime:
		case VariableType::VT_Date:
		case VariableType::VT_Time:
			{
				ESChildTextBox* pTextBox = new ESChildTextBox();
				pControl = pTextBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				pTextBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pTextBox->SetTextFont			(m_pFontChildItemValue);
				if( GetPropertyValue(pItem, sValue) )
					pTextBox->SetText(sValue);
				break;
			}
		case VariableType::VT_Size:
		case VariableType::VT_Point:
			{
				ESChildTextBox* pTextBox = new ESChildTextBox();
				pControl = pTextBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				pTextBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pTextBox->SetTextFont			(m_pFontChildItemValue);
				if( GetPropertyValue(pItem, sValue) )
					pTextBox->SetText(sValue);
				break;
			}
		case VariableType::VT_Rect:
			{
				ESChildTextBox* pTextBox = new ESChildTextBox();
				pControl = pTextBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				pTextBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pTextBox->SetTextFont			(m_pFontChildItemValue);
				if( GetPropertyValue(pItem, sValue) )
					pTextBox->SetText(sValue);
				break;
			}
		case VariableType::VT_Image:
			{
				ESChildTextBox* pFileBox = new ESChildOpenFileBox(_T("Image Files|*.png;*.bmp;*.jpg;*.jpeg||"));
				pControl = pFileBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}

				pFileBox->AddEventListener		((ESFrameBase*)this, EventCode_OnClearTextBox, (ControlEventListener)&ESChildControlPropertyGrid::OnImageClear);
				pFileBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pFileBox->SetTextFont			(m_pFontChildItemValue);
				pFileBox->SetAlternateText		(_T("Click to open image"));
				if( GetPropertyValue(pItem, sValue) )
					pFileBox->SetText(sValue);
				break;
			}
		case VariableType::VT_Binary:
			{
				ESChildTextBox* pFileBox = new ESChildOpenFileBox(_T("All Files|*.*||"));
				pControl = pFileBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}

				pFileBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pFileBox->SetTextFont			(m_pFontChildItemValue);
				pFileBox->SetAlternateText		(_T("Click to open file"));
				if( GetPropertyValue(pItem, sValue) )
					pFileBox->SetText(sValue);
				break;
			}
		case VariableType::VT_Font:
			{
				ESChildOpenFontDialogBox* pFontBox = new ESChildOpenFontDialogBox();
				pControl = pFontBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				pFontBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pFontBox->SetTextFont			(m_pFontChildItemValue);
				pFontBox->SetFont				(((_Font*)lpValuePtr), false);
				pFontBox->SetAlternateText		(_T("Click to select font"));
				break;
			}
		case VariableType::VT_Color32:
			{
				ESChildOpenColorDialogBox* pColorBox = new ESChildOpenColorDialogBox();
				pControl = pColorBox;
				if( !m_pOwner->AddChildControl(pControl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this, 1) ){
					delete pControl;
					return NULL;
					}
				pColorBox->SetColor				(*((COLORREF*)lpValuePtr), false);
				pColorBox->SetAlternateTextFont	(m_pFontChildItemValue);
				pColorBox->SetTextFont			(m_pFontChildItemValue);
				pColorBox->SetAlternateText		(_T("Click to select color"));
				break;
			}
		};
	return pControl;
	}

bool
ESChildControlPropertyGrid::RemoveValueModifyControl(bool bRedraw){
	if( m_pValueModifyControl ){
		ModifyPropertyGridItemByType(m_pSelectedItem, m_pValueModifyControl);

		m_pValueModifyControl->SetVisible(false, bRedraw);
		m_pOwner->RemoveChildControl(m_pValueModifyControl);
		m_pValueModifyControl = NULL;
		return true;
		}
	return false;
	}

void
ESChildControlPropertyGrid::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect	rcClient	= GetClientRect();
	_Rect	rcControlDC, rcInvalid;
	rcControlDC.left	= (rcClient.left - pRectClient->left) + pRectDC->left; 
	rcControlDC.top		= (rcClient.top - pRectClient->top) + pRectDC->top; 
	rcControlDC.right	= rcControlDC.left + rcClient.Width() - (m_pVScroll->IsVisible() ? m_nArrowBtnCX : 0); 
	rcControlDC.bottom	= rcControlDC.top + rcClient.Height() - (m_pHScroll->IsVisible() ? m_nArrowBtnCY : 0); 
	rcControlDC.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);

	_Rect				rcItemDC;
	MercuryGUI*			pGUI			= MercuryGUI::GetInstance();	
	ESPropertyGridItem* pItem			= m_pTopItem;
	int					nParentCt		= GetParentCt(pItem);
	int					nOffsetPerChild	= 15;
	_string				sValue;
	_Size				szText(0, 0);
	_Size				szSignImage(pGUI->treeGrid.m_imageExpandSign.GetWidth(), pGUI->treeGrid.m_imageExpandSign.GetHeight());
	_Image*				pImageSignExpanded	= &pGUI->treeGrid.m_imageExpandSign;
	_Image*				pImageSignCollapsed	= &pGUI->treeGrid.m_imageCollapseSign;

	rcItemDC.left	= rcControlDC.left;
	rcItemDC.top	= rcControlDC.top - m_nTopItemOffsetY;
	rcItemDC.right	= rcControlDC.right;
	rcItemDC.bottom	= rcItemDC.top + m_nItemCY;

	while( rcItemDC.top < rcControlDC.bottom ){
		if( !pItem ) break;

		if( pItem->pChild ){
			rcInvalid	= *pRectDC&rcItemDC;
			if( !rcInvalid.IsRectEmpty() ){
				// Fill with color.
				pDC->FillSolidRect(rcInvalid, m_crParentItem);

				// Draw expand/collapse sign image. {{
				_Rect rcSignDC;
				rcSignDC.left	= rcItemDC.left + 2 + nOffsetPerChild*nParentCt;
				rcSignDC.top	= rcItemDC.top + (m_nItemCY - szSignImage.cy)/2;
				rcSignDC.right	= rcSignDC.left + szSignImage.cx;
				rcSignDC.bottom = rcSignDC.top + szSignImage.cy;

				rcInvalid	= *pRectDC&rcSignDC;
				if( !rcInvalid.IsRectEmpty() )
					((pItem->bExpanded ? pImageSignExpanded : pImageSignCollapsed))->RenderImage(pDC, rcInvalid, rcSignDC, false);
				// }}

				if( pItem->sTitle.length() > 0 ){
					m_pFontParentItem->GetTextSize(&pItem->sTitle, szText);

					// Draw text.
					_Point ptText;
					ptText.x = rcSignDC.right + 6; 
					ptText.y = rcItemDC.top + (m_nItemCY - szText.cy) / 2; 
					// }}

					pDC->SelectObject	(m_pFontParentItem);
					pDC->SetBkMode		(TRANSPARENT);
					pDC->SetTextColor	(m_crParentItemText);
					pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, pItem->sTitle.c_str());

					// Draw focus when item is selected. {{
					if( m_pSelectedItem == pItem )
						pDC->DrawFocusRect	(_Rect(ptText.x, ptText.y, ptText.x + szText.cx, ptText.y + szText.cy));
					// }}
					}
				}
			}
		else{
			_Rect rcPropertyValueDC;
			rcPropertyValueDC.left		= rcItemDC.left + m_nPropertyTitleWidth;
			rcPropertyValueDC.top		= rcItemDC.top;
			rcPropertyValueDC.right		= rcItemDC.right;
			rcPropertyValueDC.bottom	= rcItemDC.bottom;

			_Rect rcItemLeftDC		(rcItemDC.left, rcItemDC.top, rcItemDC.left + nParentCt*nOffsetPerChild, rcItemDC.bottom);
			_Rect rcPropertyTitleDC	(rcItemLeftDC.right, rcItemDC.top, rcPropertyValueDC.left - 1, rcItemDC.bottom);

			// Fill left area with color.
			rcInvalid	= *pRectDC&rcItemLeftDC;
			if( !rcInvalid.IsRectEmpty() )
				pDC->FillSolidRect(rcInvalid, m_crParentItem);
			
			// Fill property title with color. {{
			rcInvalid	= *pRectDC&rcPropertyTitleDC;
			if( !rcInvalid.IsRectEmpty() ){
				if( pItem == m_pSelectedItem )
					pDC->FillSolidRect(rcInvalid, m_crChildItemSel);
				else
					pDC->FillSolidRect(rcInvalid, m_crChildItem);
				}
			// }}
			
			// Draw property title.
			if( pItem->sTitle.length() > 0 ){
				m_pFontChildItem->GetTextSize(&pItem->sTitle, szText);

				_Point ptText;
				ptText.x = rcPropertyTitleDC.left + 3; 
				ptText.y = rcPropertyTitleDC.top + (m_nItemCY - szText.cy) / 2;
				
				pDC->SelectObject	(m_pFontChildItem);
				pDC->SetBkMode		(TRANSPARENT);
				pDC->SetTextColor	(m_crChildItemText);
				//ImageHelper::ExtTextOut(pDC, ptText.x, ptText.y, ETO_CLIPPED, &rcPropertyTitleDC, pItem->sTitle);
				pDC->ExtTextOut(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, pItem->sTitle.c_str());
				}
			
			// Draw divider line.
			_Rect rcDividerLineDC;
			rcDividerLineDC.left	= rcPropertyTitleDC.right;
			rcDividerLineDC.top		= rcPropertyTitleDC.top;
			rcDividerLineDC.right	= rcDividerLineDC.left + 1;
			rcDividerLineDC.bottom	= rcPropertyTitleDC.bottom;

			rcInvalid	= *pRectDC&rcDividerLineDC;
			if( !rcInvalid.IsRectEmpty() ){
//				CPen penLine(PS_SOLID, 1, m_crChildItemDividerLine);
//				pDC->SelectObject(&penLine);
//				pDC->MoveTo(rcInvalid.left, rcInvalid.top);
//				pDC->LineTo(rcInvalid.left, rcInvalid.bottom);
				}
			
			// Draw property value.
			rcInvalid	= *pRectDC&rcPropertyValueDC;
			if( !rcInvalid.IsRectEmpty() ){
				// Fill property value area.
				pDC->FillSolidRect(rcInvalid, m_crChildItem);
				// Draw item by type.
				if( !(m_pValueModifyControl && pItem == m_pSelectedItem) )
					DrawPropertyGridItemByType(pItem, pDC, pRectDC, &rcPropertyValueDC, m_crChildItemText, m_pFontChildItemValue);
				}
			
			// Draw bottom line.
			_Rect rcLineDC(rcPropertyTitleDC.left, rcPropertyTitleDC.bottom - 1, rcPropertyTitleDC.right, rcPropertyTitleDC.bottom);
			if( pItem != m_pSelectedItem || !m_pValueModifyControl  )
				rcLineDC.right = rcPropertyValueDC.right;

			rcInvalid	= *pRectDC&rcLineDC;
			if( !rcInvalid.IsRectEmpty() ){
//				CPen penLine(PS_SOLID, 1, m_crChildItemBottomLine);
//				pDC->SelectObject(&penLine);
//				pDC->MoveTo(rcInvalid.left, rcInvalid.top);
//				pDC->LineTo(rcInvalid.right, rcInvalid.top);
				}
			}

		pItem			= GetNextVisibleItem(pItem, nParentCt);
		rcItemDC.top	+= m_nItemCY; rcItemDC.bottom += m_nItemCY;
		}

	// Fill remaining part with color. {{
	if( rcItemDC.top < rcControlDC.bottom ){
		rcControlDC.top = rcItemDC.top;
		rcInvalid	= *pRectDC&rcControlDC;
		if( !rcInvalid.IsRectEmpty() )
			pDC->FillSolidRect(rcInvalid, m_crChildItem);
		}
	// }}

	ESChildScrollView::OnPaintClient(pDC, pRectDC, pRectClient);
	}

BOOL	
ESChildControlPropertyGrid::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	return FALSE;
	}

void
ESChildControlPropertyGrid::OnLButtonDown(_Point pt, UINT nFlags){
	ESPropertyGridItem* pItem			= NULL;
	ESPropertyGridItem* pItemOldSel		= m_pSelectedItem;
	_Rect				rcItem(0, 0, 0, 0);
	bool				bValueSel		= false;

	if( GetItemByPoint(pt, pItem, rcItem, bValueSel) ){
		bool bCreateModifyControl = false;
		if( m_pSelectedItem != pItem ){
			RemoveValueModifyControl(true);

			if( bValueSel && !pItem->pChild )
				bCreateModifyControl = true;

			m_pSelectedItem = pItem;
			RedrawRect(rcItem, true);

			_Rect rcOldItem;
			if( GetItemRect(pItemOldSel, rcOldItem) )
				RedrawRect(rcOldItem, false);
			}
		else{
			if( !m_pValueModifyControl && bValueSel && !pItem->pChild )
				bCreateModifyControl = true;
			}

		if( bCreateModifyControl ){
			_Rect	rcTextBox;
			rcTextBox.left		= rcItem.left + m_nPropertyTitleWidth + 2;
			rcTextBox.top		= rcItem.top + 1;
			rcTextBox.right		= rcItem.right - 2;
			rcTextBox.bottom	= rcItem.bottom - 1;

			ESChildControl* pInPlaceControl = CreateValueModifyControl(pItem, rcTextBox);
			if( pInPlaceControl ){
				m_pValueModifyControl = pInPlaceControl;
				RedrawRect(rcItem, true);
				}
			}

		if( pItem->pChild )
			ExpandItem(pItem, pItem->bExpanded, true);
		else
			SendEvent(this, EventCode_PropertyItemSelChange, (void*)pItem);
		}
	else{
		if( RemoveValueModifyControl(false) ){
			_Rect rcItem;
			if( GetItemRect(m_pSelectedItem, rcItem) )
				RedrawRect(rcItem, false);
			}
		}
	}

void
ESChildControlPropertyGrid::OnLButtonUp(_Point pt, UINT nFlags){
	}

void	
ESChildControlPropertyGrid::OnLButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlPropertyGrid::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlPropertyGrid::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlPropertyGrid::OnLButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlPropertyGrid::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControlPropertyGrid::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void
ESChildControlPropertyGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	ESChildScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

	if( nChar == VK_ESCAPE || nChar == VK_RETURN ){
		if( RemoveValueModifyControl(false) ){
			_Rect rcItem;
			if( GetItemRect(m_pSelectedItem, rcItem) )
				RedrawRect(rcItem, false);
			}
		}
	}

bool
ESChildControlPropertyGrid::OnScrollBarEvent(bool bVScroll, int nCode, ESChildControl* pSender){
	CalcTopItem();
	bool				bRet		= ESChildScrollView::OnScrollBarEvent(bVScroll, nCode, pSender);
	ESChildScrollBar*	pScrollBar	= (ESChildScrollBar*)pSender;
	if( bVScroll ){
		RemoveValueModifyControl(false);
		}
	return bRet;
	}

bool
ESChildControlPropertyGrid::DrawPropertyGridItemByType(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont ){
	if( !pItem || !pItem->pVarInfo ) return false;
	DrawPropertyGridItemMethod method = DrawMethodsByVarType(pItem->pVarInfo->m_type);
	if( method ){
		(method)(pItem, pDC, pRectDC, pItemRectDC, crText, pFont);
		return true;
		}
	else{
		if( pItem->pVarInfo->m_bComplexType && pItem->pVarInfo->m_bEnumerable ){
			DrawPropertyGridItem_String(pItem, pDC, pRectDC, pItemRectDC, crText, pFont);
			return true;
			}
		}
	return false;
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_String(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	_Rect	rcInvalid = *pRectDC&*pItemRectDC;
	_string sValue;
	_Size	szText(0, 0);

	if( GetPropertyValue(pItem, sValue) && !rcInvalid.IsRectEmpty() ){
		pFont->GetTextSize(&sValue, szText);

		_Point ptText;
		ptText.x = pItemRectDC->left + 3; 
		ptText.y = pItemRectDC->top + (pItemRectDC->Height() - szText.cy) / 2;
				
		pDC->SelectObject	(pFont);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);
		//ImageHelper::ExtTextOut(pDC, ptText.x, ptText.y, ETO_CLIPPED, pItemRectDC, sValue);
		pDC->ExtTextOut(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, sValue.c_str());
		}
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Numeric(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	_Rect	rcInvalid = *pRectDC&*pItemRectDC;
	_string sValue;
	_Size	szText(0, 0);

	if( GetPropertyValue(pItem, sValue) && !rcInvalid.IsRectEmpty() ){
		pFont->GetTextSize(&sValue, szText);

		_Point ptText;
		ptText.x = pItemRectDC->left + 3; 
		ptText.y = pItemRectDC->top + (pItemRectDC->Height() - szText.cy) / 2;
				
		pDC->SelectObject	(pFont);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, sValue.c_str());
		}
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Enum(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	_Rect	rcInvalid = *pRectDC&*pItemRectDC;
	_string sValue;
	_Size	szText(0, 0);

	StringToIntArray* pArr = (StringToIntArray*)GetPropertyValuePtr(pItem);
	if( !pArr ) return;

	sValue = pArr->GetCurrentKey();
	if( sValue.length() && !rcInvalid.IsRectEmpty() ){
		pFont->GetTextSize(&sValue, szText);

		_Point ptText;
		ptText.x = pItemRectDC->left + 3; 
		ptText.y = pItemRectDC->top + (pItemRectDC->Height() - szText.cy) / 2;
				
		pDC->SelectObject	(pFont);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, sValue.c_str());
		}
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Color(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	_Rect	rcInvalid	= *pRectDC&*pItemRectDC;
	_string sValue;
	_Size	szText(0, 0);

	COLORREF* pColor	= (COLORREF*)GetPropertyValuePtr(pItem);
	if( !pColor ) return;
	
	BYTE red	= (BYTE)((*pColor)&0xFF);
	BYTE green	= (BYTE)(((*pColor)&0xFF00) >> 8);
	BYTE blue	= (BYTE)(((*pColor)&0xFF0000) >> 16);
	stringEx::Format(sValue, _T("#%02X%02X%02X"), red, green, blue);
	
	if( sValue.length() && !rcInvalid.IsRectEmpty() ){
		pFont->GetTextSize(&sValue, szText);
		pDC->FillSolidRect(&rcInvalid,  *pColor);

		_Point ptText;
		ptText.x = pItemRectDC->left + 3; 
		ptText.y = pItemRectDC->top + (pItemRectDC->Height() - szText.cy) / 2;
				
		pDC->SelectObject	(pFont);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(RGB(255-red, 255-green, 255-blue));
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, sValue.c_str());
		}
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Image(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	_Rect	rcInvalid	= *pRectDC&*pItemRectDC;
	_string sValue;
	_Size	szText(0, 0);

	_Image* pImage	= (_Image*)GetPropertyValuePtr(pItem);
	if( !pImage ) return;
	if( pImage->IsNull() )
		sValue = _T("Image cx=0 cy=0");
	else
		stringEx::Format(sValue, _T("Image cx=%d cy=%d"), pImage->GetWidth(), pImage->GetHeight());
	
	if( sValue.length() && !rcInvalid.IsRectEmpty() ){
		pFont->GetTextSize(&sValue, szText);

		_Point ptText;
		ptText.x = pItemRectDC->left + 3; 
		ptText.y = pItemRectDC->top + (pItemRectDC->Height() - szText.cy) / 2;
				
		pDC->SelectObject	(pFont);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, sValue.c_str());
		}
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Size(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	DrawPropertyGridItem_String(pItem, pDC, pRectDC, pItemRectDC, crText, pFont);
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Point(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	DrawPropertyGridItem_String(pItem, pDC, pRectDC, pItemRectDC, crText, pFont);
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Rect(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	_Rect	rcInvalid = *pRectDC&*pItemRectDC;
	_string sValue;
	_Size	szText(0, 0);
	_Rect* pRect = (_Rect*)GetPropertyValuePtr(pItem);

	if( GetPropertyValue(pItem, sValue) && !rcInvalid.IsRectEmpty() ){
		_string sValueNew;
		stringEx::Format(sValueNew, _T("%s (%dx%d)"), sValue.c_str(), pRect->Width(), pRect->Height());

		pFont->GetTextSize(&sValueNew, szText);

		_Point ptText;
		ptText.x = pItemRectDC->left + 3; 
		ptText.y = pItemRectDC->top + (pItemRectDC->Height() - szText.cy) / 2;
				
		pDC->SelectObject	(pFont);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, sValueNew.c_str());
		}
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Font(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	_Rect	rcInvalid	= *pRectDC&*pItemRectDC;
	_string sValue;
	_Size	szText(0, 0);

	_Font*	pFontValue	= (_Font*)GetPropertyValuePtr(pItem);
	if( !pFontValue ) return;
	
	LOGFONT lf;
	if( !pFontValue->IsNull() && pFontValue->GetLogFont(&lf) != 0 )
		stringEx::Format(sValue, _T("%s"), lf.lfFaceName);
	
	if( sValue.length() && !rcInvalid.IsRectEmpty() ){
		pFontValue->GetTextSize(&sValue, szText);
		
		_Point ptText;
		ptText.x = pItemRectDC->left + 3; 
		ptText.y = pItemRectDC->top + (pItemRectDC->Height() - szText.cy) / 2;
				
		pDC->SelectObject	(pFontValue);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(crText);
		pDC->ExtTextOut		(ptText.x, ptText.y, ETO_CLIPPED, pRectDC, sValue.c_str());
		}
	}

void
ESChildControlPropertyGrid::DrawPropertyGridItem_Binary(ESPropertyGridItem* pItem, _DC* pDC, _Rect* pRectDC, _Rect* pItemRectDC, COLORREF crText, _Font* pFont){
	DrawPropertyGridItem_String(pItem, pDC, pRectDC, pItemRectDC, crText, pFont);
	}

bool
ESChildControlPropertyGrid::ModifyPropertyGridItemByType(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pItem || !pItem->pVarInfo ) return false;
	ModifyPropertyValue method = ModifyPropertyValueByType(pItem->pVarInfo->m_type);
	if( method ){
		(method)(pItem, pControl);
		return true;
		}
	return false;
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Bool(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildComboBox*		pComboBox	= (ESChildComboBox*)pControl;
	bool					bValue		= (pComboBox->GetSelectedParam() ? true : false);
	bool*					pBValue		= (bool*)GetPropertyValuePtr(pItem);
	if( !pBValue ){
		ASSERT(FALSE);
		return;
		}

	if( bValue != *pBValue ){
		bool bTemp = *pBValue;
		*pBValue = bValue;
		if( !pItem->pObject->CallModifyCallback(pBValue, &bValue) ){
			*pBValue = bTemp;
			}
		}
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_String(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildTextBox*	pTextBox	= (ESChildOpenFontDialogBox*)pControl;
	_string						sTextNew		= pTextBox->GetText();
	_string*					pTextOld		= (_string*)GetPropertyValuePtr(pItem);

	if( !pTextOld ){
		ASSERT(FALSE);
		return;
		}

	if( *pTextOld == sTextNew )
		return; // Nothing to modify.
	
	_string sTextTemp = *pTextOld;
	*pTextOld = sTextNew;
	if( !pItem->pObject->CallModifyCallback(pTextOld, &sTextNew) ){
		*pTextOld = sTextTemp;
		}	
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Numeric(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildTextBox*			pTextBox	= (ESChildTextBox*)pControl;
	_string					sValue		= pTextBox->GetText();
	void*					pValueOld	= (void*)GetPropertyValuePtr(pItem);

	switch(pItem->pVarInfo->m_type){
		case VariableType::VT_Float:{
			float fValueNew = (float)std::stof(sValue);
			if( fValueNew != *((float*)pValueOld) ){
				float fValueOld			= *((float*)pValueOld);
				*((float*)pValueOld)	= fValueNew;
				if( !pItem->pObject->CallModifyCallback(pValueOld, &fValueNew) ){
					*((float*)pValueOld) = fValueOld;
					}
				}
			break;
			}

		case VariableType::VT_Double:{
			double dValueNew = std::stod(sValue);
			if( dValueNew != *((double*)pValueOld) ){
				double dValueOld		= *((double*)pValueOld);
				*((double*)pValueOld)	= dValueNew;
				if( !pItem->pObject->CallModifyCallback(pValueOld, &dValueNew) ){
					*((double*)pValueOld) = dValueOld;
					}
				}
			break;
			}

		case VariableType::VT_Int8:{
			char cValueNew = (char)std::stoi(sValue);
			if( cValueNew != *((char*)pValueOld) ){
				char cCharOld			= *((char*)pValueOld);
				*((char*)pValueOld)		= cValueNew;

				if( !pItem->pObject->CallModifyCallback(pValueOld, &cValueNew) ){
					*((char*)pValueOld) = cCharOld;
					}
				}
			break;
			}
		case VariableType::VT_Int16:{
			short shValueNew = (short)std::stoi(sValue);
			if( shValueNew != *((short*)pValueOld) ){
				short shValueOld		= *((short*)pValueOld);
				*((short*)pValueOld)	= shValueNew;
				if( !pItem->pObject->CallModifyCallback(pValueOld, &shValueNew) ){
					*((short*)pValueOld) = shValueOld;
					}
				}
			break;
			}
		case VariableType::VT_Int32:{
			int nValueNew = std::stoi(sValue);
			if( nValueNew != *((int*)pValueOld) ){
				int nValueOld		= *((int*)pValueOld);
				*((int*)pValueOld)	= nValueNew;
				if( !pItem->pObject->CallModifyCallback(pValueOld, &nValueNew) ){
					*((int*)pValueOld) = nValueOld;
					}
				}
			break;
			}
		case VariableType::VT_Int64:{
#ifdef _WINDOWS
			__int64 nnValueNew = _ttoi64(sValue.c_str());
#else
            __int64 nnValueNew = std::stoll(sValue);
#endif
			if( nnValueNew != *((__int64*)pValueOld) ){
				__int64 nnValueOld		= *((__int64*)pValueOld);
				*((__int64*)pValueOld)	= nnValueNew;

				if( !pItem->pObject->CallModifyCallback(pValueOld, &nnValueNew) ){
					*((__int64*)pValueOld) = nnValueOld;
					}
				}
			break;
			}
        default:
            break;
		};
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Enum(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;
/*
	ESChildComboBox*		pComboBox	= (ESChildComboBox*)pControl;
	void*					lpValue		= pComboBox->GetSelectedParam();
	StringToIntArray*		pArrEnum	= (StringToIntArray*)GetPropertyValuePtr(pItem);
	if( !pArrEnum ){
		ASSERT(FALSE);
		return;
		}

	if( pArrEnum->GetCurrentValue() == (int)nValue )
		return; // Nothing to modify.

	void* lpValueOld = pArrEnum->GetCurrentValue();
	pArrEnum->SetCurrentValue(nValue);
	if( !pItem->pObject->CallModifyCallback(pArrEnum, &nValue) ){
		pArrEnum->SetCurrentValue(nValueOld);
		}*/
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Color(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildOpenColorDialogBox*		pColorBox		= (ESChildOpenColorDialogBox*)pControl;
	COLORREF						crValue			= pColorBox->GetColor();
	COLORREF*						pColorValueOld	= (COLORREF*)GetPropertyValuePtr(pItem);
	if( !pColorValueOld ){
		ASSERT(FALSE);
		return;
		}

	if( *pColorValueOld == crValue )
		return; // Nothing to modify.

	COLORREF crValueTemp = *pColorValueOld;
	*pColorValueOld = crValue;
	if( !pItem->pObject->CallModifyCallback(pColorValueOld, &crValue) ){
		*pColorValueOld = crValueTemp;
		}
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Image(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildOpenFileBox*		pImageBox		= (ESChildOpenFileBox*)pControl;
	_string					sImageFile		= pImageBox->GetFilePath();
	_Image*					pImageOld		= (_Image*)GetPropertyValuePtr(pItem);
	if( !pImageOld ){
		ASSERT(FALSE);
		return;
		}

	if( !sImageFile.length() ) return;
	if( !pImageOld->IsNull() ){
		pImageOld->Destroy();
		}

	_Image*	pImageNew = _Image::LoadImage(sImageFile, -1, -1, false);
	if( pImageNew ){
		if( !pImageNew->IsNull() )
			pImageOld->Attach(pImageNew->Detach());
		delete pImageNew;
		}
	pItem->pObject->CallModifyCallback(pImageOld, NULL);
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Size(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildTextBox*		pTextBox		= (ESChildTextBox*)pControl;
	_string				sValue			= pTextBox->GetText();
	_Size*				pSizeValueOld	= (_Size*)GetPropertyValuePtr(pItem);
	if( !pSizeValueOld ){
		ASSERT(FALSE);
		return;
		}

	_Size szValueTemp	= *pSizeValueOld;
	_Size szValueNew;
	if( pItem->pObject->SetVariableFromString(pItem->pVarInfo, sValue) ){
		szValueNew		= *pSizeValueOld;
		if( !pItem->pObject->CallModifyCallback(pSizeValueOld, &szValueNew) ){
			*pSizeValueOld	= szValueTemp;
			}
		}
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Point(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildTextBox*		pTextBox		= (ESChildOpenColorDialogBox*)pControl;
	_string				sValue			= pTextBox->GetText();
	_Point*				pPointValueOld	= (_Point*)GetPropertyValuePtr(pItem);
	if( !pPointValueOld ){
		ASSERT(FALSE);
		return;
		}

	_Point ptValueTemp = *pPointValueOld;
	_Point ptValueNew;
	if( pItem->pObject->SetVariableFromString(pItem->pVarInfo, sValue) ){
		ptValueNew		= *pPointValueOld;
		if( !pItem->pObject->CallModifyCallback(pPointValueOld, &ptValueNew) ){
			*pPointValueOld	= ptValueTemp;
			}
		}
	else{
		}
	}

#include "../Utility/StringHelper.h"

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Rect(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildTextBox*		pTextBox		= (ESChildOpenColorDialogBox*)pControl;
	_string				sValue			= pTextBox->GetText();
	_Rect*				pRectValueOld	= (_Rect*)GetPropertyValuePtr(pItem);
	if( !pRectValueOld ){
		ASSERT(FALSE);
		return;
		}

	_Rect rcValueTemp = *pRectValueOld;
	_Rect rcValueNew;

	// Update width and height of rect.
	CStringArray arrResult;
	if( StringHelper::Split(&sValue, _T(","), arrResult) == 2 ){
		int width = std::stoi(arrResult[0]);
		int height = std::stoi(arrResult[1]);

		if( width > 0 && height > 0 ){
			rcValueNew			= *pRectValueOld;
			rcValueNew.right	= rcValueNew.left + width;
			rcValueNew.bottom	= rcValueNew.top + height;
			*pRectValueOld		= rcValueNew;

			if( !pItem->pObject->CallModifyCallback(pRectValueOld, &rcValueNew) ){
				*pRectValueOld	= rcValueTemp;
				}
			}
		return;
		}

	if( pItem->pObject->SetVariableFromString(pItem->pVarInfo, sValue) ){
		rcValueNew		= *pRectValueOld;		
		if( !pItem->pObject->CallModifyCallback(pRectValueOld, &rcValueNew) ){
			*pRectValueOld	= rcValueTemp;
			}
		}
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Font(ESPropertyGridItem* pItem, ESChildControl* pControl){
	if( !pControl || !pItem->pObject || !pItem->pVarInfo) 
		return;

	ESChildOpenFontDialogBox*	pFontControl	= (ESChildOpenFontDialogBox*)pControl;
	_Font*						pFont			= pFontControl->GetFont();
	_Font*						pFontVal		= (_Font*)GetPropertyValuePtr(pItem);
	if( !pFontVal ){
		ASSERT(FALSE);
		return;
		}

	if( pFontVal )
		pFontVal->DeleteObject();

	if( pFont && !pFont->IsNull() ){
		LOGFONT lf;
		if( pFont->GetLogFont(&lf) != 0 )
			pFontVal->CreateFontIndirect(&lf);
		}

	pItem->pObject->CallModifyCallback(pFontVal, NULL);
	}

void
ESChildControlPropertyGrid::ModifyPropertyGridItem_Binary(ESPropertyGridItem* pItem, ESChildControl* pControl){
	}

void
ESChildControlPropertyGrid::OnComboBoxSelChange(ESChildComboBox* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	}

void
ESChildControlPropertyGrid::OnLookupEnumerable(ESChildLookupBox* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	if( !m_pSelectedItem || !m_pSelectedItem->pObject) return;

	EnumerableObject<Serializable>* pEnumerable = (EnumerableObject<Serializable>*) GetPropertyValuePtr(m_pSelectedItem);
	EnumerableObject<Serializable>*	pCopy		= pEnumerable->Copy();

	ESEnumerableModifyDlg dlg;
	dlg.SetEnumerableObject(pCopy);
	dlg.Create();
	/*
	if( dlg.CreateModal(CWnd::FromHandle(m_pOwner->GetHWND())) == IDOK ){
		}
	*/
	if( m_pSelectedItem->pObject->CallModifyCallback(pEnumerable, pCopy) ){
		// Copy objects from enumerable. {{
		pEnumerable->RemoveAll(true);
		for(int i=0; i<pCopy->GetCount(); i++){
			/*
			Serializable* pObjSrc = pCopy->GetAt(i);
			if( pObjSrc ){
				Serializable* pObjDest = pCopy->CreateSerializableObject();
				pObjSrc->Copy(pObjDest);
				pEnumerable->Add(pObjDest);
				}
			else
				pEnumerable->Add(NULL);*/
			Serializable* pObjSrc = pCopy->GetAt(i);
			pEnumerable->Add(pObjSrc);
			}
		// }}
		pCopy->RemoveAll(false);
		}
	delete pCopy;
	}

void
ESChildControlPropertyGrid::OnImageClear(ESChildTextBox* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	if( !m_pSelectedItem || !m_pSelectedItem->pObject) return;

	_Image* pImgValue = (_Image*)GetPropertyValuePtr(m_pSelectedItem);
	if( pImgValue && !pImgValue->IsNull() ){
		pImgValue->Destroy();

		_string sValue = _T("Image cx=0 cy=0");
		pSender->SetText(sValue);
		}

	if( m_pSelectedItem->pObject )
		m_pSelectedItem->pObject->CallModifyCallback(pImgValue, NULL);
	}
