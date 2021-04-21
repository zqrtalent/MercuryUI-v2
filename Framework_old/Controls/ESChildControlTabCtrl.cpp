// ESChildControlTabCtrl.cpp : implementation file
//


#include "ESChildControlTabCtrl.h"
#include "ESChildTabView.h"
#include "../Path/Path.h"

/////////////////////////////////////////////////////////////////////////////
// ESTabCtrlItemInfo

ESTabCtrlItemInfo::ESTabCtrlItemInfo(){
	// Image instances.
	m_pImageDefault			= NULL;
	m_pImageHover			= NULL;
	m_pImagePressed			= NULL;
	m_pImageActiveDefault	= NULL;
	m_pImageActiveHover		= NULL;
	m_pImageActivePressed	= NULL;
    }

BEGIN_DECLARE_VAR(ESTabCtrlItemInfo, Serializable)
DECLARE_VAR(_T("ESTabCtrlItemInfo"),			_T(""), VariableType::VT_None, 0, false, true, false)
DECLARE_VAR(_T("Title"),					_T("TITLE"), VariableType::VT_String, offsetof(ESTabCtrlItemInfo,m_sTitle), false, false, false)
DECLARE_VAR(_T("Data"),						_T("DATA"), VariableType::VT_Int32, offsetof(ESTabCtrlItemInfo,m_nData), false, false, false)
DECLARE_VAR(_T("Width"),					_T("WIDTH"), VariableType::VT_Int32, offsetof(ESTabCtrlItemInfo,m_nWidth), false, false, false)
DECLARE_VAR(_T("Width Percent"),			_T("WIDTH_PERCENT"), VariableType::VT_Float, offsetof(ESTabCtrlItemInfo,m_fWidthPercent), false, false, false)
DECLARE_VAR(_T("Image Default"),			_T("IMAGE_DEFAULT"), VariableType::VT_String, offsetof(ESTabCtrlItemInfo,m_sImageDefault), false, false, false)
DECLARE_VAR(_T("Image Hover"),				_T("IMAGE_HOVER"), VariableType::VT_String, offsetof(ESTabCtrlItemInfo,m_sImageHover), false, false, false)
DECLARE_VAR(_T("Image Pressed"),			_T("IMAGE_PRESSED"), VariableType::VT_String, offsetof(ESTabCtrlItemInfo,m_sImagePressed), false, false, false)
DECLARE_VAR(_T("Image Active Default"),		_T("IMAGE_ACTIVE_DEFAULT"), VariableType::VT_String, offsetof(ESTabCtrlItemInfo,m_sImageActiveDefault), false, false, false)
DECLARE_VAR(_T("Image Active Hover"),		_T("IMAGE_ACTIVE_HOVER"), VariableType::VT_String, offsetof(ESTabCtrlItemInfo,m_sImageActiveHover), false, false, false)
DECLARE_VAR(_T("Image Active Pressed"),		_T("IMAGE_ACTIVE_PRESSED"), VariableType::VT_String, offsetof(ESTabCtrlItemInfo,m_sImageActivePressed), false, false, false)
END_DECLARE_VAR()

ESTabCtrlItemInfo::~ESTabCtrlItemInfo(){
}

/////////////////////////////////////////////////////////////////////////////
// ESTabItemLinkedInfo

ESTabItemLinkedInfo::ESTabItemLinkedInfo(){
}

BEGIN_DECLARE_VAR(ESTabItemLinkedInfo, Serializable)
DECLARE_VAR(_T("ESTabItemLinkedInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
DECLARE_VAR(_T("ItemIndex"),			_T(""), VariableType::VT_Int32, offsetof(ESTabItemLinkedInfo,m_nItemIndex), false, false, false)
DECLARE_VAR(_T("LinkedControl"),		_T(""), VariableType::VT_String, offsetof(ESTabItemLinkedInfo,m_sLinkedControl), false, false, false)
END_DECLARE_VAR()

ESTabItemLinkedInfo::~ESTabItemLinkedInfo(){
}

/////////////////////////////////////////////////////////////////////////////
// ESTabCtrlItemInfoList

ESTabCtrlItemInfoList::ESTabCtrlItemInfoList(){
}

BEGIN_DECLARE_VAR(ESTabCtrlItemInfoList, Serializable)
DECLARE_VAR(_T("ESTabCtrlItemInfoList"),	_T(""), VariableType::VT_None, 0, false, true, false)
DECLARE_VAR(_T("Infos"),				_T(""), VariableType::VT_None, offsetof(ESTabCtrlItemInfoList,m_listItems), false, true, true)
DECLARE_VAR(_T("LinkedInfos"),			_T(""), VariableType::VT_None, offsetof(ESTabCtrlItemInfoList,m_linkedInfos), false, true, true)
END_DECLARE_VAR()

ESTabCtrlItemInfoList::~ESTabCtrlItemInfoList(){
    }

/////////////////////////////////////////////////////////////////////////////
// ESChildControlTabCtrl

ESChildControlTabCtrl::ESChildControlTabCtrl(){
	m_sClassName			= _T("ESChildControlTabCtrl");
    
	MercuryGUI*	pGUI		= MercuryGUI::GetInstance();
	m_nActiveItem			= -1;
	m_nItemUnderCursor		= -1;
	m_crTabItemText			= pGUI->tabctrl.m_crNormalItemText;
	m_crActiveTabItemText	= pGUI->tabctrl.m_crActiveItemText;
	m_rcItemOmitBorder		= pGUI->tabctrl.m_rcOmitBorder;
    
	m_pImageItemNormal		= NULL;
	m_pImageItemHover		= NULL;
	m_pImageItemActive		= NULL;
    
	m_bUseHandCursor		= true;
    
	pGUI->tabctrl.m_itemTextFont.CopyFont(&m_tabItemFont);
	pGUI->tabctrl.m_activeItemTextFont.CopyFont(&m_tabItemActiveFont);
    }

BEGIN_DECLARE_VAR(ESChildControlTabCtrl, ESChildControl)
DECLARE_VAR(_T("ESChildControlTabCtrl"),		_T(""), VariableType::VT_None, 0, false, true, false)
DECLARE_VAR(_T("Item Image"),				_T(""), VariableType::VT_Image, offsetof(ESChildControlTabCtrl,m_imgItemNormal), false, false, false)
DECLARE_VAR(_T("Hover Item Image"),			_T(""), VariableType::VT_Image, offsetof(ESChildControlTabCtrl,m_imgItemHover), false, false, false)
DECLARE_VAR(_T("Active Item Image"),		_T(""), VariableType::VT_Image, offsetof(ESChildControlTabCtrl,m_imgItemActive), false, false, false)
DECLARE_VAR(_T("Item Image File"),			_T(""), VariableType::VT_String, offsetof(ESChildControlTabCtrl,m_sImgFileItemNormal), false, false, false)
DECLARE_VAR(_T("Hover Item Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildControlTabCtrl,m_sImgFileItemHover), false, false, false)
DECLARE_VAR(_T("Active Item Image File"),	_T(""), VariableType::VT_String, offsetof(ESChildControlTabCtrl,m_sImgFileItemActive), false, false, false)
DECLARE_VAR(_T("OmitBorder"),				_T(""), VariableType::VT_Rect, offsetof(ESChildControlTabCtrl,m_rcItemOmitBorder), false, false, false)
DECLARE_VAR(_T("UseHandCursor"),			_T(""), VariableType::VT_Bool, offsetof(ESChildControlTabCtrl,m_bUseHandCursor), false, false, false)
DECLARE_VAR(_T("Item Font"),				_T(""), VariableType::VT_Font, offsetof(ESChildControlTabCtrl,m_tabItemFont), false, false, false)
DECLARE_VAR(_T("Active Item Font"),			_T(""), VariableType::VT_Font, offsetof(ESChildControlTabCtrl,m_tabItemActiveFont), false, false, false)
DECLARE_VAR(_T("Item Text Color"),			_T(""), VariableType::VT_Color32, offsetof(ESChildControlTabCtrl,m_crTabItemText), false, false, false)
DECLARE_VAR(_T("Active Item Text Color"),	_T(""), VariableType::VT_Color32, offsetof(ESChildControlTabCtrl,m_crActiveTabItemText), false, false, false)
DECLARE_VAR(_T("Active Item"),				_T(""), VariableType::VT_Int32, offsetof(ESChildControlTabCtrl,m_nActiveItem), false, false, false)
DECLARE_VAR(_T("Items"),					_T(""), VariableType::VT_None, offsetof(ESChildControlTabCtrl,m_listItems), false, true, true)
END_DECLARE_VAR()

ESChildControlTabCtrl::~ESChildControlTabCtrl(){
}

bool
ESChildControlTabCtrl::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet && m_pOwner->IsDesignerMode() ){
		// Register variable modify callback methods. {{
		AddVariableModifyCallBack(&m_listItems,	(VariableValueModifyCallback)&ESChildControlTabCtrl::ItemsModify);
		// }}
    }
    
	if( bRet ){
		LoadItemImages();
		OnLocalizeStrings();
    }
	return bRet;
}

bool
ESChildControlTabCtrl::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
}

bool
ESChildControlTabCtrl::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
}

bool
ESChildControlTabCtrl::Destroy(){
	if( ESChildControl::Destroy() ){
		if( !m_imgItemNormal.IsNull() )
			m_imgItemNormal.Destroy();
		if( !m_imgItemHover.IsNull() )
			m_imgItemHover.Destroy();
		if( !m_imgItemActive.IsNull() )
			m_imgItemActive.Destroy();
		
		m_tabItemFont.DeleteObject();
		m_tabItemActiveFont.DeleteObject();
    }
	return true;
}

void
ESChildControlTabCtrl::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
}

UINT
ESChildControlTabCtrl::GetItemData(int nItem){
	ESTabCtrlItemInfo* pItem = m_listItems.GetAt(nItem);
	if( !pItem )
		return 0L;
	return pItem->m_nData;
}

void*
ESChildControlTabCtrl::GetItemParam(int nItem){
	ESTabCtrlItemInfo* pItem = m_listItems.GetAt(nItem);
	if( !pItem )
		return 0L;
	return pItem->m_pParam;
}

bool
ESChildControlTabCtrl::SetItemParam(int nItem, void* pParam){
	ESTabCtrlItemInfo* pItem = m_listItems.GetAt(nItem);
	if( !pItem ) return false;
	pItem->m_pParam = pParam;
	return true;
}

int
ESChildControlTabCtrl::FindItemByItemData(UINT nData){
	for(int i=0; i<m_listItems.GetCount(); i++){
		ESTabCtrlItemInfo* pItem = m_listItems.GetAt(i);
		if( pItem && pItem->m_nData == nData ){
			return i;
        }
    }
	return -1;
}

bool
ESChildControlTabCtrl::RedrawItem(int nItem){
	_Rect rcItem;
	if( !GetItemRect(nItem, rcItem) )
		return false;
	RedrawRect(rcItem, true);
	return true;
}

bool
ESChildControlTabCtrl::GetItemRect(int nItem, _Rect& rcItem){
	if( nItem < 0 || nItem >= GetItemCt() )
		return false;
    
	int nItemCt			= m_listItems.GetCount();
	int nBorderWidth	= 0;
	int nItemHeight		= m_rRect.Height();
	int nItemWidth		= m_rRect.Width() / nItemCt;
	int nLoop = 0, nCt = m_listItems.GetCount();
    
	_Rect rcTabItem;
	rcTabItem.left		= m_rRect.left + nBorderWidth;
	rcTabItem.top		= m_rRect.top + nBorderWidth;
	rcTabItem.right		= 0;
	rcTabItem.bottom	= rcTabItem.top + nItemHeight;
    
	while( nLoop < nCt ){
		ESTabCtrlItemInfo* pItem = m_listItems.GetAt(nLoop);
		rcTabItem.right = rcTabItem.left + nItemWidth;
		if( nLoop == nItem ){
			rcItem = rcTabItem;
			return true;
        }
		rcTabItem.left = rcTabItem.right;
		nLoop ++;
    }
	return false;
}

int
ESChildControlTabCtrl::AddTabItem(_string sTitle, int nWidth, UINT nData, void* pParam /*= NULL*/){
	ESTabCtrlItemInfo* pItem	= new ESTabCtrlItemInfo();
	pItem->m_sTitle				= sTitle;
	pItem->m_sTitleLocal		= sTitle;
	ESLocalizationMan::ParseText(pItem->m_sTitleLocal);
	pItem->m_nData				= nData;
	pItem->m_nWidth				= nWidth;
	pItem->m_fWidthPercent		= 0;
	pItem->m_pParam				= pParam;
	m_listItems.Add(pItem);
    
	pItem->m_pImageDefault			= m_pImageItemNormal;
	pItem->m_pImageHover			= m_pImageItemHover;
	pItem->m_pImageActiveDefault	= m_pImageItemActive;
    
	if( m_nActiveItem == -1 )
		m_nActiveItem = 0;
	return true;
}

bool
ESChildControlTabCtrl::RemoveTabItem(int nIndex){
	bool bRet = m_listItems.RemoveAt(nIndex, true);
	if( nIndex == m_nActiveItem && bRet )
		m_nActiveItem = -1;
	return bRet;
}

bool
ESChildControlTabCtrl::RemoveAllTabItem(){
	m_listItems.RemoveAll();
	m_nActiveItem = -1;
	return true;
}

bool
ESChildControlTabCtrl::ActivateTabItem(int nIndex,  bool bRedraw /*= false*/){
	if( nIndex < 0 || nIndex >= m_listItems.GetCount() )
		return false;
	m_nActiveItem = nIndex;
	if( bRedraw )
		Redraw();
	return true;
}

bool
ESChildControlTabCtrl::SetTabItemTitle(int nItem, _string sTitle, bool bRedraw){
	if( nItem < 0 || nItem >= m_listItems.GetCount() )
		return false;
    
	ESTabCtrlItemInfo* pItem = m_listItems.GetAt(nItem);
	if( !pItem )
		return false;
    
	pItem->m_sTitle			= sTitle;
	pItem->m_sTitleLocal	= sTitle;
	ESLocalizationMan::ParseText(pItem->m_sTitleLocal);
	if( bRedraw )
		Redraw();
	return true;
}

void
ESChildControlTabCtrl::LoadItemImages(){
	MercuryGUI* pGUI = MercuryGUI::GetInstance();
	_string sImage;
	if( m_imgItemNormal.IsNull() ){
		sImage = m_sImgFileItemNormal;
		Path::BuildFullPath(sImage);
		m_pImageItemNormal = pGUI->LoadImage(sImage);
    }
	else
		m_pImageItemNormal = &m_imgItemNormal;
    
	if( m_imgItemHover.IsNull() ){
		sImage = m_sImgFileItemHover;
		Path::BuildFullPath(sImage);
		m_pImageItemHover = pGUI->LoadImage(sImage);
    }
	else
		m_pImageItemHover = &m_imgItemHover;
    
	if( m_imgItemActive.IsNull() ){
		sImage = m_sImgFileItemActive;
		Path::BuildFullPath(sImage);
		m_pImageItemActive = pGUI->LoadImage(sImage);
    }
	else
		m_pImageItemActive = &m_imgItemActive;
    
	for(int i=0; i<m_listItems.GetCount(); i++){
		ESTabCtrlItemInfo* pItem = m_listItems.GetAt(i);
		// Default
		if( pItem->m_sImageDefault.length() ){
			sImage = pItem->m_sImageDefault;
			Path::BuildFullPath(sImage);
			pItem->m_pImageDefault = pGUI->LoadImage(sImage);
        }
		else
			pItem->m_pImageDefault = m_pImageItemNormal;
        
		// Hover
		if( pItem->m_sImageHover.length() ){
			sImage = pItem->m_sImageHover;
			Path::BuildFullPath(sImage);
			pItem->m_pImageHover = pGUI->LoadImage(sImage);
        }
		else
			pItem->m_pImageHover = m_pImageItemHover;
        
		// Pressed
		if( pItem->m_sImagePressed.length() ){
			sImage = pItem->m_sImagePressed;
			Path::BuildFullPath(sImage);
			pItem->m_pImagePressed = pGUI->LoadImage(sImage);
        }
		else
			pItem->m_pImagePressed = NULL;
        
		// Active default
		if( pItem->m_sImageActiveDefault.length() ){
			sImage = pItem->m_sImageActiveDefault;
			Path::BuildFullPath(sImage);
			pItem->m_pImageActiveDefault = pGUI->LoadImage(sImage);
        }
		else
			pItem->m_pImageActiveDefault = m_pImageItemActive;
        
		// Active hover
		if( pItem->m_sImageActiveHover.length() ){
			sImage = pItem->m_sImageActiveHover;
			Path::BuildFullPath(sImage);
			pItem->m_pImageActiveHover = pGUI->LoadImage(sImage);
        }
		else
			pItem->m_pImageActiveHover = NULL;
        
		// Active pressed
		if( pItem->m_sImageActivePressed.length() ){
			sImage = pItem->m_sImageActivePressed;
			Path::BuildFullPath(sImage);
			pItem->m_pImageActivePressed = pGUI->LoadImage(sImage);
        }
		else
			pItem->m_pImageActivePressed = NULL;
    }
}

int
ESChildControlTabCtrl::GetItemByPoint(_Point pt){
	if( !m_listItems.GetCount() )
		return -1;
    
	int nItemCt			= m_listItems.GetCount();
	int nItem			= 0;
	int nBorderWidth	= 0;
	int nItemHeight		= m_rRect.Height();
	int nItemWidth		= m_rRect.Width() / nItemCt;
    
	int nLoop			= 0;
	int nCt				= m_listItems.GetCount();
    
	_Rect rcTabItem;
	rcTabItem.left		= m_rRect.left + nBorderWidth;
	rcTabItem.top		= m_rRect.top + nBorderWidth;
	rcTabItem.right		= 0;
	rcTabItem.bottom	= rcTabItem.top + nItemHeight;
    
	while( nLoop < nCt ){
		ESTabCtrlItemInfo* pItem = m_listItems.GetAt(nLoop);
		rcTabItem.right = rcTabItem.left + nItemWidth;
        
		if( rcTabItem.PtInRect(pt) )
			return nLoop;
		
		rcTabItem.left = rcTabItem.right;
		nLoop ++;
    }
	return -1;
}

bool
ESChildControlTabCtrl::RedrawItems(){
	int nItemCt			= m_listItems.GetCount();
	int nItem			= 0;
	int nBorderWidth	= 0;
	int nItemHeight		= m_rRect.Height();
    
	_Rect rcTabItem;
	rcTabItem.left		= m_rRect.left + nBorderWidth;
	rcTabItem.top		= m_rRect.top + nBorderWidth;
	rcTabItem.right		= m_rRect.right - nBorderWidth;
	rcTabItem.bottom	= rcTabItem.top + nItemHeight;
    
	if( m_pOwner )
		m_pOwner->RedrawRect(rcTabItem, 1);
	return (m_pOwner != NULL);
}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildControlTabCtrl::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	int nItemCt				= m_listItems.GetCount();
	int nItem				= 0;
	int nBorderWidth		= 1;
	int nItemWidth			= nItemCt > 0 ? (m_rRect.Width() / nItemCt) : 0;
	int nItemHeight			= m_rRect.Height();
    
	Image*	pImage			= NULL;
    
	_Rect	rcTabViewDC, rcInvalid;
	rcTabViewDC.left	= (m_rRect.left - pRectClient->left) + pRectDC->left;
	rcTabViewDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top;
	rcTabViewDC.right	= rcTabViewDC.left + m_rRect.Width();
	rcTabViewDC.bottom	= rcTabViewDC.top + m_rRect.Height();
    
	int					nOffsetX	= rcTabViewDC.left;
	ESTabCtrlItemInfo*	pItem		= NULL;
	_Size				szText;
	bool				bActive		= false;
    
	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	m_pOwner->GetZoom(fZoomCX, fZoomCY);
    
	Font fontActive;
	m_tabItemActiveFont.StretchedTextFont(&fontActive, fZoomCX, fZoomCY);
    
	Font fontDefault;
	m_tabItemFont.StretchedTextFont(&fontDefault, fZoomCX, fZoomCY);
    
	while( nItem < nItemCt ){
		pItem				= m_listItems.GetAt(nItem);
		bActive				= (nItem == m_nActiveItem);
        
		_Rect rcTabItemDC;
		rcTabItemDC.left	= nOffsetX;
		rcTabItemDC.top		= rcTabViewDC.top;
		rcTabItemDC.right	=  (nItem == nItemCt-1) ? (rcTabViewDC.right) : rcTabItemDC.left + nItemWidth;
		rcTabItemDC.bottom	= rcTabItemDC.top + nItemHeight;
        
		// Draw tabview item.
		pImage = NULL;
		if( bActive ){
			if( nItem == m_nItemUnderCursor ){
				if( m_bMouseCaptured && pItem->m_pImageActivePressed )
					pImage = pItem->m_pImageActivePressed;
				else
					pImage = pItem->m_pImageActiveHover;
            }
            
			if( !pImage )
				pImage = pItem->m_pImageActiveDefault;
        }
		else{
			if( nItem == m_nItemUnderCursor ){
				if( m_bMouseCaptured && pItem->m_pImagePressed )
					pImage = pItem->m_pImagePressed;
				else
					pImage = pItem->m_pImageHover;
            }
            
			if( !pImage )
				pImage = pItem->m_pImageDefault;
        }
        
		// old
		//pImage		= bActive ? m_pImageItemActive : ((nItem == m_nItemUnderCursor && m_pImageItemHover && !m_pImageItemHover->IsNull()) ? m_pImageItemHover :  m_pImageItemNormal);
		rcInvalid	= rcTabItemDC & *pRectDC;
		if( pImage && !pImage->IsNull() && !rcInvalid.IsRectEmpty() ){
			if( !m_rcItemOmitBorder.IsRectNull() )
				pImage->RenderImage_OmitBorder(pDC, rcInvalid, rcTabItemDC, m_rcItemOmitBorder, FALSE);
			else
				pImage->RenderImage(pDC, rcInvalid, rcTabItemDC, true, (pImage->GetBPP() == 32), ScaleMode::ScaleMode_Bilinier);
        }
        
		// Draw item label. {{
		Font* pFontOld = NULL;
		if( nItem == m_nActiveItem ){
			pFontOld = pDC->SelectObject(&fontActive);
			fontActive.GetTextSize(&pItem->m_sTitleLocal, szText);
        }
		else{
			pFontOld = pDC->SelectObject(&fontDefault);
			fontDefault.GetTextSize(&pItem->m_sTitleLocal, szText);
        }
		
		_Point ptLabel		(rcTabItemDC.left + (rcTabItemDC.Width() - szText.cx)/2, rcTabItemDC.top + (rcTabItemDC.Height() - szText.cy)/2);
		pDC->SetBkMode		(TRANSPARENT);
		pDC->SetTextColor	(bActive ? m_crActiveTabItemText : m_crTabItemText);
        //	ImageHelper::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcTabItemDC, pItem->m_sTitleLocal);
		pDC->ExtTextOut     (ptLabel.x, ptLabel.y, ETO_CLIPPED, rcInvalid, pItem->m_sTitleLocal.c_str());
		pDC->SelectObject	(pFontOld);
		// }}
        
		nOffsetX	+= nItemWidth;
		nItem		++;
    }
    
	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
}

void
ESChildControlTabCtrl::OnMouseHover(_Point pt, UINT nFlags){
	int nOldItemHover = m_nItemUnderCursor;
	m_nItemUnderCursor = GetItemByPoint(pt);
    
	if( /*nOldItemHover != m_nActiveItem &&*/ nOldItemHover > -1 )
		RedrawItem(nOldItemHover);
    
	if( /*m_nItemUnderCursor != m_nActiveItem &&*/ m_nItemUnderCursor > -1 )
		RedrawItem(m_nItemUnderCursor);
}

BOOL
ESChildControlTabCtrl::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	if( !m_bUseHandCursor )
		return 0;
	if( m_nItemUnderCursor == -1 && (m_pImageItemHover && !m_pImageItemHover->IsNull()) )
		return 0;
//	SetCursor(LoadCursor(NULL, IDC_HAND));
	return 1;
}

void
ESChildControlTabCtrl::OnMouseLeave(UINT nFlags){
	if( m_nItemUnderCursor > -1 /*&& m_nItemUnderCursor != m_nActiveItem*/ ){
		int nItem = m_nItemUnderCursor;
		m_nItemUnderCursor = -1;
		RedrawItem(nItem);
    }
	else
		m_nItemUnderCursor = -1;
}

void
ESChildControlTabCtrl::OnMouseEnter(UINT nFlags){
}

void
ESChildControlTabCtrl::OnLButtonDown(_Point pt, UINT nFlags){
}

void
ESChildControlTabCtrl::OnLButtonUp(_Point pt, UINT nFlags){
}

void
ESChildControlTabCtrl::OnLButtonClick(_Point pt, UINT nFlags){
	if( m_pOwner->IsDesignerMode() )
		m_nItemUnderCursor = GetItemByPoint(pt);
	if( m_nItemUnderCursor != -1 ){
		// In designer mode switch tabview's tab.
		if( m_pOwner->IsDesignerMode() && m_pParent && m_pParent->GetClassName() == _T("ESChildTabView")){
			((ESChildTabView*)m_pParent)->ActivateTabItem(m_nItemUnderCursor, true);
			return;
        }
        
		ESTabCtrlItemInfo* pItemOld = m_listItems.GetAt(m_nActiveItem);
		ActivateTabItem(m_nItemUnderCursor, true);
		ESTabCtrlItemInfo* pItem = m_listItems.GetAt(m_nActiveItem);
		if( pItem && pItem != pItemOld )
			SendEvent(this, EventCode_TabItemChange, (void*)m_nActiveItem);
    }
}

void
ESChildControlTabCtrl::OnRButtonClick(_Point pt, UINT nFlags){
    }

void
ESChildControlTabCtrl::OnMButtonClick(_Point pt, UINT nFlags){
    }

void
ESChildControlTabCtrl::OnLButtonDblClick(_Point pt, UINT nFlags){
    }

void
ESChildControlTabCtrl::OnRButtonDblClick(_Point pt, UINT nFlags){
    }

void
ESChildControlTabCtrl::OnMButtonDblClick(_Point pt, UINT nFlags){
    }

void
ESChildControlTabCtrl::OnLocalizeStrings(){
	for(int i=0; i<m_listItems.GetCount(); i++){
		ESTabCtrlItemInfo* pItem = m_listItems.GetAt(i);
		if( pItem ){
			pItem->m_sTitleLocal = pItem->m_sTitle;
			if( !m_pOwner->IsDesignerMode() )
				ESLocalizationMan::ParseText(pItem->m_sTitleLocal);
            }
        }
    }

bool
ESChildControlTabCtrl::ItemsModify(ESChildControlTabCtrl* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->LoadItemImages();
	return true;
}