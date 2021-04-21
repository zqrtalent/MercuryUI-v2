#include "MercuryLayerMan.h"
#include "MercuryBaseWnd.h"
#include "ESChildControl.h"

//////////////////////////////////////////////////////
//
//	class MercuryLayerManager
//
/////////////////////////////////////////////////////

MercuryLayerManager::MercuryLayerManager(MercuryBaseView* pBaseView){
	m_pView = pBaseView;
	}

MercuryLayerManager::~MercuryLayerManager(){
	RemoveAll();
	}

bool
MercuryLayerManager::Add(ESChildControl* pChild){
	if( !pChild ) return false;
	m_lock.Lock(); // Lock
	ASSERT( pChild->IsCreated() );
	_Rect rcChild = pChild->GetClientRect();
	if( !pChild->IsVisible() || rcChild.IsRectEmpty() ){
		if( m_arrExcludedControl.IndexOf((void*)pChild) == -1 ){
			m_arrExcludedControl.Add((void*)pChild);
			m_lock.Unlock(); // Unlock
			return true;
			}
		else{
			m_lock.Unlock(); // Unlock
			return false;
			}
		}
	else{
		MercuryLayer* pLayer = FindLayerByIndex(pChild->GetZIndex(), true);
		bool bRet = pLayer->Add(pChild);
		m_lock.Unlock(); // Unlock
		return bRet;
		}
	m_lock.Unlock(); // Unlock
	return false;
	}

bool
MercuryLayerManager::Remove(ESChildControl* pChild){
	m_lock.Lock(); // Lock
	int nIndex = m_arrExcludedControl.IndexOf((void*)pChild);
	if( nIndex != -1 ){
		m_arrExcludedControl.Delete(nIndex);
		m_lock.Unlock(); // Unlock
		return true;
		}
	else{
		MercuryLayer* pLayer = FindLayerByIndex(pChild->GetZIndex(), false);
		if( pLayer ){
			bool bRet = pLayer->Remove(pChild);
			m_lock.Unlock(); // Unlock
			return bRet;
			}
		}
	m_lock.Unlock(); // Unlock
	return false;
	}

void
MercuryLayerManager::RemoveAll(){
	m_lock.Lock(); // Lock
	m_arrExcludedControl.DeleteAll();
	// Destroy MercuryLayer objects.
	for(int i=0; i<m_arrLayerByIndex.GetCount(); i++){
		MercuryLayer* pLayer = (MercuryLayer*)m_arrLayerByIndex.GetData(i);
		if( pLayer )
			delete pLayer;
		}
	m_arrLayerByIndex	.DeleteAll();
	m_lock.Unlock(); // Unlock
	}

ESChildControl*
MercuryLayerManager::GetTopMostChildByPoint(_Point pt){
	ASSERT(m_pView);
	_Rect rcView;
	m_pView->GetClientRectMy(rcView);
	if( !rcView.PtInRect(pt) )
		return NULL;

	m_lock.Lock(); // Lock
	for(int i=m_arrLayerByIndex.GetCount()-1; i>=0; i--){
		MercuryLayer* pLayer = (MercuryLayer*)m_arrLayerByIndex.GetData(i);
		if( pLayer ){
			CPtrArray arrChilds;
			if( pLayer->GetChildsByPoint(pt, arrChilds) > 0 ){
				ESChildControl* pRet = (ESChildControl*)arrChilds.at(0);
				m_lock.Unlock(); // Unlock
				return pRet;
				}
			}
		}
	m_lock.Unlock(); // Unlock
	return NULL;
	}

int
MercuryLayerManager::EnumChildControlsByPoint(_Point pt, CPtrArray& arrControls){
	ASSERT(m_pView);
	_Rect rcView;
	m_pView->GetClientRectMy(rcView);
	if( !rcView.PtInRect(pt) )
		return 0;

	m_lock.Lock(); // Lock
	int nCtOld = (int)arrControls.size();
	for(int i=0; i<m_arrLayerByIndex.GetCount(); i++){
		MercuryLayer* pLayer = (MercuryLayer*)m_arrLayerByIndex.GetData(i);
		if( pLayer )
			pLayer->GetChildsByPoint(pt, arrControls);
		}
	int nRet = ((int)arrControls.size() - nCtOld);
	m_lock.Unlock(); // Unlock
	return nRet;
	}

int
MercuryLayerManager::EnumChildControlsByRect(_Rect rc, CPtrArray& arrControls){
	ASSERT(m_pView);
	_Rect rcView;
	m_pView->GetClientRectMy(rcView);
	if( !(rcView&rc) ) return 0;

	m_lock.Lock(); // Lock
	int nCtOld = (int)arrControls.size();
	for(int i=0; i<m_arrLayerByIndex.GetCount(); i++){
		MercuryLayer* pLayer = (MercuryLayer*)m_arrLayerByIndex.GetData(i);
		if( pLayer )
			pLayer->GetChildsByRect(rc, arrControls);
		}
	int nRet = ((int)arrControls.size() - nCtOld);
	m_lock.Unlock(); // Unlock
	return nRet;
	}

void
MercuryLayerManager::ViewSizeChanged(int cx, int cy){
	}

void
MercuryLayerManager::ChildSizeChanged(ESChildControl* pChild){
	m_lock.Lock(); // Lock
	_Rect rcNew = pChild->GetClientRect();
	if( !rcNew.IsRectEmpty() ){
		int nIndex = m_arrExcludedControl.IndexOf((void*)pChild);
		if( nIndex != -1 ){
			m_arrExcludedControl.Delete(nIndex);
			Add(pChild);
			}
		}
	else{
		Remove(pChild);
		int nIndex = m_arrExcludedControl.IndexOf((void*)pChild);
		if( nIndex == -1 )
			m_arrExcludedControl.Add((void*)pChild);
		}
	m_lock.Unlock(); // Unlock
	}

void
MercuryLayerManager::ChildVisibilityChanged(ESChildControl* pChild, bool bVisible){
	m_lock.Lock(); // Lock
	if( bVisible ){
		int nIndex = m_arrExcludedControl.IndexOf((void*)pChild);
		if( nIndex != -1 ){
			m_arrExcludedControl.Delete(nIndex);
			Add(pChild);
			}
		}
	else{
		Remove(pChild);
		int nIndex = m_arrExcludedControl.IndexOf((void*)pChild);
		if( nIndex == -1 )
			m_arrExcludedControl.Add((void*)pChild);
		}
	m_lock.Unlock(); // Unlock
	}

MercuryLayer*
MercuryLayerManager::FindLayerByIndex(int nLayerIndex, bool bCreateIfNotExists){
	m_lock.Lock(); // Lock
	int nIndex = m_arrLayerByIndex.IndexOf((void*)nLayerIndex);
	if( nIndex == -1 ){
		if( bCreateIfNotExists ){
			MercuryLayer* pNewLayer = new MercuryLayer(nLayerIndex);
			m_arrLayerByIndex.Add((void*)nLayerIndex, (void*)pNewLayer);
			m_lock.Unlock(); // Unlock
			return pNewLayer;
			}
		m_lock.Unlock(); // Unlock
		return NULL;
		}
	MercuryLayer* pLayer = (MercuryLayer*)m_arrLayerByIndex.GetData(nIndex);
	m_lock.Unlock(); // Unlock
	return pLayer;
	}

//////////////////////////////////////////////////////
//
//	class MercuryLayer
//
/////////////////////////////////////////////////////

MercuryLayer::MercuryLayer(int nLayerIndex){
	m_nLayerIndex = nLayerIndex;
	}

MercuryLayer::~MercuryLayer(){
	}

bool 
MercuryLayer::Add(ESChildControl* pChild){
	_Rect rcChild = pChild->GetClientRect();
	if( rcChild.IsRectEmpty() || pChild->GetZIndex() != m_nLayerIndex )
		return false;
	if( m_arrChildControlById.IndexOf((void*)pChild->GetId()) == -1 ){
		m_arrChildControlById.Add((void*)pChild->GetId(), (void*)pChild);
		return true;
		}
	return false;
	}

bool 
MercuryLayer::Remove(ESChildControl* pChild){
	int nIndex = m_arrChildControlById.IndexOf((void*)pChild->GetId());
	if( nIndex != -1 ){
		m_arrChildControlById.Delete(nIndex);
		return true;
		}
	return false;
	}

int 
MercuryLayer::GetChildsByPoint(_Point& pt, CPtrArray& arrControls){
//	_Rect	rcChild;
	int		nRet = 0;
	for(int i=0; i<m_arrChildControlById.GetCount(); i++){
		ESChildControl* pChild = (ESChildControl*)m_arrChildControlById.GetData(i);
		if( pChild && (pChild->IsMouseEventsAllowed() || pChild->IsDesignerMode()) ){
		//	rcChild = pChild->GetClientRect();
		//	if( rcChild.PtInRect(pt) ){
			if( pChild->PtInClientArea(pt) ){
				arrControls.push_back(pChild);
				nRet ++;
				}
			}
		}
	return nRet;
	}

int 
MercuryLayer::GetChildsByRect(_Rect& rect, CPtrArray& arrControls){
	_Rect	rcChild;
	int		nRet = 0;
	for(int i=0; i<m_arrChildControlById.GetCount(); i++){
		ESChildControl* pChild = (ESChildControl*)m_arrChildControlById.GetData(i);
		if( pChild ){
			rcChild = pChild->GetClientRect();
			rcChild &= rect;
			if( !rcChild.IsRectEmpty() ){
				arrControls.push_back(pChild);
				nRet ++;
				}
			}
		}
	return nRet;
	}