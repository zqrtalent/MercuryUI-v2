// ESChildTabView.cpp : implementation file
//

#include "ESChildTabView.h"
#include "ESChildControlImage.h"
#include "../Utility/GrowableMemory.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildTabView

ESChildTabView::ESChildTabView(){
	m_sClassName			= _T("ESChildTabView");
	m_nBorderWidth			= 1;
	m_crBorder				= RGB(102, 102, 102);
	m_nSelectedTabIndex		= -1;
	m_pTabCtrl				= new ESChildControlTabCtrl();
}

BEGIN_DECLARE_VAR(ESChildTabView, ESChildControl)
DECLARE_VAR(_T("ESChildTabView"),		_T(""), VariableType::VT_None, 0, false, true, false)
DECLARE_VAR(_T("Border Color"),		_T(""), VariableType::VT_Color32, offsetof(ESChildTabView,m_crBorder), false, false, false)
DECLARE_VAR(_T("Border Width"),		_T(""), VariableType::VT_Int32, offsetof(ESChildTabView,m_nBorderWidth), false, false, false)
DECLARE_VAR(_T("Selected Tab"),		_T(""), VariableType::VT_Int32, offsetof(ESChildTabView,m_nSelectedTabIndex), false, false, false)
END_DECLARE_VAR()

ESChildTabView::~ESChildTabView(){
	if( m_pTabCtrl ){
		if( m_pOwner && m_pOwner->GetChildControl(m_pTabCtrl->GetId()) == m_pTabCtrl ){
			m_pOwner->RemoveChildControl(m_pTabCtrl, false);
        }
		else
			delete m_pTabCtrl;
    }
}

bool
ESChildTabView::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		if( m_pOwner->IsDesignerMode() ){
			// Register variable modify callback methods. {{
			AddVariableModifyCallBack(&m_nSelectedTabIndex,	(VariableValueModifyCallback)&ESChildTabView::ActiveTabItemModify);
			// }}
        }
        
		int nTabCtrlHeight = m_pTabCtrl->GetClientHeight();
		if( nTabCtrlHeight <= 0 )
			nTabCtrlHeight = 20;
        
		float fCX = 1.0f;
		float fCY = 1.0f;
		pOwner->GetZoom(fCX, fCY);
        
		nTabCtrlHeight *= fCY;
        
		if( pOwner->AddChildControl(m_pTabCtrl, MercuryBaseView::GetChildControlIdUnused(m_pOwner), _Rect(0, 0, 0, nTabCtrlHeight), this) ){
			_Rect rcFixedPos(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, -1);
            
			m_pTabCtrl->SetSerializeFlag(false);
			m_pTabCtrl->SetFixedPosition(rcFixedPos);
			m_pTabCtrl->SetVisible		(true, false);
			m_pTabCtrl->AddEventListener((ESFrameBase*)this, EventCode_TabItemChange, (ControlEventListener)&ESChildTabView::OnTabItemChange);
            
			// Register variable modify callback methods. {{
			m_pTabCtrl->AddVariableModifyCallBack(&m_pTabCtrl->m_listItems,	(VariableValueModifyCallback)&ESChildTabView::ItemsModify);
			// }}
        }
    }
	return bRet;
}

bool
ESChildTabView::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
}

bool
ESChildTabView::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
}

bool
ESChildTabView::Destroy(){
	if( ESChildControl::Destroy() ){
		if( m_pTabCtrl && !m_pTabCtrl->IsCreated() ){
			delete m_pTabCtrl;
			m_pTabCtrl = NULL;
        }
		else
			m_pTabCtrl = NULL;
    }
	return true;
}

void
ESChildTabView::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
}

bool
ESChildTabView::VisibilityChanged(bool bVisible){
	// Hide child controls belonging to hidden tab.
	if( bVisible ){
		for(int i=0; i<m_pTabCtrl->GetItemCt(); i++){
			ESChildControl* pLinkedPageControl = (ESChildControl*)m_pTabCtrl->GetItemParam(i);
			if( !pLinkedPageControl )
				continue;
            
			if( i != m_nSelectedTabIndex )
				pLinkedPageControl->SetVisible(false, false);
			else
				pLinkedPageControl->SetVisible(true, false);
        }
		// Make tab control visible.{{
		if( m_pTabCtrl && !m_pTabCtrl->IsVisible() )
			m_pTabCtrl->SetVisible(true, false);
		// }}
        
		m_bVisible = bVisible;
		SendEvent(this, EventCode_VisibilityChanged);
		// Prevent visiblity change operation on child controls by returning false.
		return false;
    }
	return true;
}

void
ESChildTabView::OnDeserializationCompleted(){
	// Load tab items. {{
	for(int i=0; i<m_itemInfos.m_listItems.GetCount(); i++){
		ESTabCtrlItemInfo* pItem = m_itemInfos.m_listItems.GetAt(i);
		if( pItem )
			AddTabItem(pItem->m_sTitle, pItem->m_nWidth, pItem->m_nData);
    }
	// }}
    
	// Link controls to tab item. {{
	for(int i=0; i<m_itemInfos.m_linkedInfos.GetCount(); i++){
		ESTabItemLinkedInfo* pInfo = m_itemInfos.m_linkedInfos.GetAt(i);
		if( pInfo ){
			ESChildControl* pLinkedControl = m_pOwner->GetChildControlByName(pInfo->m_sLinkedControl.c_str());
			m_pTabCtrl->SetItemParam(pInfo->m_nItemIndex, (void*)pLinkedControl);
			if( pLinkedControl )
				pLinkedControl->SetVisible(false, false);
        }
    }
	// }}
    
	// Activate tab item.
	ActivateTabItem(m_nSelectedTabIndex, false);
}

UINT
ESChildTabView::GetItemData(int nItem){
	if( !m_pTabCtrl )
		return 0L;
	return m_pTabCtrl->GetItemData(nItem);
}

int
ESChildTabView::AddTabItem(_string sTitle, int nWidth, UINT nData){
	if( !m_pTabCtrl )
		return 0L;
	return m_pTabCtrl->AddTabItem(sTitle, nWidth, nData);
}

bool
ESChildTabView::RemoveTabItem(int nIndex){
	if( !m_pTabCtrl )
		return false;
    
	ESChildControlImage*	pTabControlsOwnerOld	= (ESChildControlImage*)m_pTabCtrl->GetItemParam(nIndex);
	if( m_pTabCtrl->RemoveTabItem(nIndex) ){
		if( pTabControlsOwnerOld ){
			for(int i=0; i<m_itemInfos.m_listItems.GetCount(); i++){
				if( i == nIndex )
					m_itemInfos.m_listItems.RemoveAt(i, true);
            }
            
			for(int i=0; i<m_itemInfos.m_linkedInfos.GetCount(); i++){
				if( i == nIndex )
					m_itemInfos.m_linkedInfos.RemoveAt(i, true);
            }
			m_pOwner->RemoveChildControl(pTabControlsOwnerOld, false);
        }
		return true;
    }
    
	return false;
}

bool
ESChildTabView::RemoveAllTabItem(){
	if( !m_pTabCtrl )
		return false;
    
	for(int i=0; i<m_pTabCtrl->GetItemCt(); i++){
		ESChildControlImage*	pTabControlsOwnerOld	= (ESChildControlImage*)m_pTabCtrl->GetItemParam(i);
		if( pTabControlsOwnerOld )
			m_pOwner->RemoveChildControl(pTabControlsOwnerOld, false);
    }
    
	bool bRet = m_pTabCtrl->RemoveAllTabItem();
	m_itemInfos.m_listItems.RemoveAll(true);
	m_itemInfos.m_linkedInfos.RemoveAll(true);
	return bRet;
}

bool
ESChildTabView::ActivateTabItem(int nIndex,  bool bRedraw /*= false*/){
	if( !m_pTabCtrl )
		return false;
    
	ESChildControlImage*	pTabControlsOwnerOld	= (ESChildControlImage*)m_pTabCtrl->GetItemParam(m_nSelectedTabIndex);
	bool					bRet					= m_pTabCtrl->ActivateTabItem(nIndex, false);
	if( bRet ){
		if( pTabControlsOwnerOld ){
			pTabControlsOwnerOld->SetVisible(false, false, true);
        }
        
		ESChildControlImage* pTabControlsOwner = (ESChildControlImage*)m_pTabCtrl->GetItemParam(nIndex);
		if( !pTabControlsOwner ){
			pTabControlsOwner	= new ESChildControlImage();
			_Rect rcControl		= GetClientRect();
			rcControl.DeflateRect(m_nBorderWidth, m_nBorderWidth, m_nBorderWidth, m_nBorderWidth);
			if( m_pOwner->AddChildControl(pTabControlsOwner, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcControl, this) ){
				pTabControlsOwner->SetFixedPosition	(_Rect(m_nBorderWidth));
				pTabControlsOwner->SetVisible		(true, false);
				m_pTabCtrl->SetItemParam			(nIndex, pTabControlsOwner);
            }
        }
		else
			pTabControlsOwner->SetVisible(true, false, true);
        
		m_nSelectedTabIndex = nIndex;
		if( bRedraw )
			pTabControlsOwner->Redraw(true);
    }
	return bRet;
}

bool
ESChildTabView::SetTabItemTitle(int nItem, _string sTitle, bool bRedraw){
	if( !m_pTabCtrl )
		return false;
	return m_pTabCtrl->SetTabItemTitle(nItem, sTitle, bRedraw);
}

BOOL
ESChildTabView::Serialize(GrowableMemory* pMem){
	BOOL bRet = ESChildControl::Serialize(pMem);
	if( bRet && m_pTabCtrl ){
		ESTabCtrlItemInfoList					itemsInfo;
		EnumerableObject<ESTabCtrlItemInfo>*	pItems	= &m_pTabCtrl->m_listItems;
		int										nCt		= pItems->GetCount();
		for(int i=0; i<nCt; i++){
			ESTabCtrlItemInfo* pInfo = pItems->GetAt(i);
			itemsInfo.m_listItems.Add(pInfo);
            
			if( pInfo && pInfo->m_pParam ){
				ESTabItemLinkedInfo* pLinkedInfo	= new ESTabItemLinkedInfo();
				pLinkedInfo->m_nItemIndex			= i;
				pLinkedInfo->m_sLinkedControl		= ((ESChildControl*)pInfo->m_pParam)->GetName();
				itemsInfo.m_linkedInfos.Add(pLinkedInfo);
            }
        }
        
		bRet = itemsInfo.Serialize(pMem);
		itemsInfo.m_listItems	.RemoveAll(false);
		itemsInfo.m_linkedInfos	.RemoveAll(false);
        
		// Serialize tabcontrol attributes. {{
		m_pTabCtrl->Serialize(pMem);
		// }}
    }
	return bRet;
}

BOOL
ESChildTabView::SerializeWithVariableName(GrowableMemory* pMem){
	BOOL bRet = ESChildControl::SerializeWithVariableName(pMem);
	if( bRet && m_pTabCtrl ){
		ESTabCtrlItemInfoList					itemsInfo;
		EnumerableObject<ESTabCtrlItemInfo>*	pItems	= &m_pTabCtrl->m_listItems;
		int										nCt		= pItems->GetCount();
		for(int i=0; i<nCt; i++){
			ESTabCtrlItemInfo* pInfo = pItems->GetAt(i);
			itemsInfo.m_listItems.Add(pInfo);
            
			if( pInfo && pInfo->m_pParam ){
				ESTabItemLinkedInfo* pLinkedInfo	= new ESTabItemLinkedInfo();
				pLinkedInfo->m_nItemIndex			= i;
				pLinkedInfo->m_sLinkedControl		= ((ESChildControl*)pInfo->m_pParam)->GetName();
				itemsInfo.m_linkedInfos.Add(pLinkedInfo);
            }
        }
        
		bRet = itemsInfo.SerializeWithVariableName(pMem);
		itemsInfo.m_listItems	.RemoveAll(false);
		itemsInfo.m_linkedInfos	.RemoveAll(false);
        
		// Serialize tabcontrol attributes. {{
		m_pTabCtrl->SerializeWithVariableName(pMem);
		// }}
    }
	return bRet;
}

BOOL
ESChildTabView::DeserializeWithVariableName(GrowableMemory* pMem){
	BOOL bRet = ESChildControl::DeserializeWithVariableName(pMem);
	if( bRet ){
		bRet = m_itemInfos.DeserializeWithVariableName(pMem);
		if( bRet ){
			bRet = m_pTabCtrl->DeserializeWithVariableName(pMem);
			if( bRet ){
				m_pTabCtrl->m_listItems.RemoveAll(true);
				m_pTabCtrl->m_nActiveItem	= -1;
            }
        }
    }
	return bRet;
}

BOOL
ESChildTabView::Deserialize(GrowableMemory* pMem){
	BOOL bRet = ESChildControl::Deserialize(pMem);
	if( bRet ){
		bRet = m_itemInfos.Deserialize(pMem);
		if( bRet ){
			bRet = m_pTabCtrl->Deserialize(pMem);
			if( bRet ){
				m_pTabCtrl->m_listItems.RemoveAll(true);
				m_pTabCtrl->m_nActiveItem	= -1;
            }
        }
    }
	return bRet;
}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildTabView::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect		rcControlDC, rcInvalid;
	rcControlDC.left	= (m_rRect.left - pRectClient->left) + pRectDC->left;
	rcControlDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top;
	rcControlDC.right	= rcControlDC.left + m_rRect.Width();
	rcControlDC.bottom	= rcControlDC.top + m_rRect.Height();
    
	if( m_nBorderWidth > 0 )
		pDC->DrawPath(rcControlDC, m_nBorderWidth, m_crBorder);
    
	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
}

bool
ESChildTabView::ActiveTabItemModify(ESChildTabView* pThis, VariableInfo* pInfo, void* lpValueNew){
	pThis->ActivateTabItem(pThis->m_nSelectedTabIndex, true);
	return true;
}

bool
ESChildTabView::ItemsModify(ESChildControlTabCtrl* pThis, VariableInfo* pInfo, void* lpValueNew){
	EnumerableObject<ESTabCtrlItemInfo>*	pItemsNew	= (EnumerableObject<ESTabCtrlItemInfo>*)lpValueNew;
	EnumerableObject<ESTabCtrlItemInfo>*	pItemsOld	= (EnumerableObject<ESTabCtrlItemInfo>*)&pThis->m_listItems;
	ESChildTabView*							pTabView	= (ESChildTabView*)pThis->GetParent();
	if( !pTabView || !pItemsNew )
		return true;
    
	for(int i=0; i<pItemsNew->GetCount(); i++){
		ESTabCtrlItemInfo* pItem = pItemsNew->GetAt(i);
		if( pItem ){
			// Copy param value from old to new item instance.
			ESTabCtrlItemInfo* pItemOld = pItemsOld->GetAt(i);
			if( pItemOld )
				pItem->m_pParam = pItemOld->m_pParam;
			else
				pItem->m_pParam = NULL;
        }
    }
	return true;
}


void
ESChildTabView::OnTabItemChange(ESChildControlTabCtrl* pSender, ChildControlEventCode eventCode, int nParam /*= 0*/){
	ActivateTabItem(nParam, true);
}
