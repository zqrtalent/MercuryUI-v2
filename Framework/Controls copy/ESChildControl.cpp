// ESChildControl.cpp : implementation file
//

#include "ESChildControl.h"
#include "ESChildControlInfo.h"
//#include "..\Image\ImageHelper.h"

/////////////////////////////////////////////////////////////////////////////
// ESChildControl

ESChildControl::ESChildControl(){
	m_sClassName		= _T("ESChildControl");
	m_pControlInfo		= NULL;
	m_nId				= 0;
	m_pOwner			= NULL;
	m_pParent			= NULL;
	m_pNext				= NULL;
	m_pChild			= NULL;
	m_rRect.SetRect(0, 0, 0, 0);
	m_bVisible			= false;
	m_bCreated			= false;
	m_nZIndex			= 0;

	m_bLocked			= false;
	m_bModal			= false;
	m_bAllowToSaveScreen= true;
	m_bLogicalPos		= false;
	m_bSerialize		= true;

	m_pArrChildIdsHidden= NULL;

	m_fPosX				= 0.0f;
	m_fPosY				= 0.0f;
	m_fCX				= 0.0f;
	m_fCY				= 0.0f;
	m_rcFixedPosition.SetRect(-1, -1, -1, -1);

	m_pTopRelativeControl		= NULL;		
	m_pBottomRelativeControl	= NULL;
	m_pLeftRelativeControl		= NULL;
	m_pRightRelativeControl		= NULL;

	m_bMouseCaptured			= FALSE;
	m_ptMouseDown.SetPoint(0, 0);
	}

BEGIN_DECLARE_VAR(ESChildControl, Serializable)
	DECLARE_VAR(_T("ESChildControl"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),					_T(""), VariableType::VT_Int32, offsetof(ESChildControl,m_nId), false, false, false)
	DECLARE_VAR(_T("Name"),					_T(""), VariableType::VT_String, offsetof(ESChildControl,m_sName), false, false, false)
	DECLARE_VAR(_T("ZIndex"),				_T(""), VariableType::VT_Int32, offsetof(ESChildControl,m_nZIndex), false, false, false)
	DECLARE_VAR(_T("Visible"),				_T(""), VariableType::VT_Bool, offsetof(ESChildControl,m_bVisible), false, false, false)
	DECLARE_VAR(_T("Rect"),					_T(""), VariableType::VT_Rect, offsetof(ESChildControl,m_rRect), false, false, false)
	DECLARE_VAR(_T("FixedPositions"),		_T(""), VariableType::VT_Rect, offsetof(ESChildControl,m_rcFixedPosition), false, false, false)
	DECLARE_VAR(_T("AllowMouseEvents"),		_T(""), VariableType::VT_Bool, offsetof(ESChildControl,m_bAllowMouseEvents), false, false, false)
	DECLARE_VAR(_T("Lock"),					_T(""), VariableType::VT_Bool, offsetof(ESChildControl,m_bLocked), false, false, false)
END_DECLARE_VAR()

ESChildControl::~ESChildControl(){
	if( m_bCreated )
		Destroy();
	}

bool
ESChildControl::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	ASSERT(pOwner);
	if( m_bCreated ){
		ASSERT(FALSE);
		return false;
		}

	m_nId			= nId;		
	m_pOwner		= pOwner;	
	m_pParent		= pParent;	
	m_pNext			= NULL;
	m_rRect		= rRect;	
	m_bVisible		= true;	
	m_bCreated		= true;	
	
	if( m_pOwner->IsDesignerMode() ){
		// Register variable modify callback methods. {{
		AddVariableModifyCallBack(&m_sName,					(VariableValueModifyCallback)&ESChildControl::NameModify);
		AddVariableModifyCallBack(&m_nZIndex,				(VariableValueModifyCallback)&ESChildControl::ZIndexModify);
		AddVariableModifyCallBack(&m_bVisible,				(VariableValueModifyCallback)&ESChildControl::VisibilityModify);
		AddVariableModifyCallBack(&m_rRect,				(VariableValueModifyCallback)&ESChildControl::ControlRectModify);
		AddVariableModifyCallBack(&m_bAllowMouseEvents,		(VariableValueModifyCallback)&ESChildControl::AllowMouseEventsModify);
		AddVariableModifyCallBack(&m_rcFixedPosition,		(VariableValueModifyCallback)&ESChildControl::FixedPositionModify);
		// }}
		}
	return true;
	}

bool
ESChildControl::SetPos(int x, int y, bool bRedraw){
	int cx	= m_rRect.Width(), cy	= m_rRect.Height();
	_Rect rRectOld = m_rRect;

	m_rRect.left	= x;
	m_rRect.top	= y;
	m_rRect.right	= m_rRect.left + cx;
	m_rRect.bottom	= m_rRect.top + cy;

	if( m_pControlInfo )
		m_pControlInfo->m_rRect = m_rRect;

	if( bRedraw ){
		// Redraw old rect.
		RedrawRect(rRectOld, true);
		// Redraw new rect.
		Redraw();
		}
	return true;
	}

bool
ESChildControl::SetSize	(int cx, int cy, bool bRedraw){
	m_rRect.right	= m_rRect.left + cx;
	m_rRect.bottom = m_rRect.top + cy;

	if( m_pControlInfo )
		m_pControlInfo->m_rRect = m_rRect;

	if( bRedraw )
		Redraw();
	return true;
	}

bool
ESChildControl::Destroy	(){
	m_bCreated = false;
	RemoveAllEvents();
	if( m_pArrChildIdsHidden ){
		delete m_pArrChildIdsHidden;
		m_pArrChildIdsHidden = NULL;
		}

	m_pTopRelativeControl		= NULL;
	m_pBottomRelativeControl	= NULL;
	m_pLeftRelativeControl		= NULL;
	m_pRightRelativeControl		= NULL;
	return true;
	}

void
ESChildControl::SetZIndex(int nZIndex){
	m_nZIndex = nZIndex; 
	}

void
ESChildControl::AddChild(ESChildControl* pChild){
	if( !m_pChild ){
		m_pChild = pChild;
		}
	else{
		ESChildControl* pChildLast = m_pChild;
		while( pChildLast->m_pNext )
			pChildLast = pChildLast->m_pNext;
		pChildLast->m_pNext = pChild;
		}
	}

ESChildControl*
ESChildControl::GetPrev(){
	if( !m_pParent || !m_pParent->m_pChild || m_pParent->m_pChild == this ) 
		return NULL;

	ESChildControl* pPrev = m_pParent->m_pChild;
	while( pPrev ){
		if( pPrev->m_pNext == this )
			return pPrev;
		pPrev = pPrev->m_pNext;
		}
	return NULL;
	}

void
ESChildControl::Redraw(bool bLayersToo /*= true*/){
	if( !m_pOwner || !IsVisible() ) return;
	if( m_bCreated && !m_rRect.IsRectEmpty() ){
#ifdef __APPLE__
        bLayersToo = true;
#endif
        if( bLayersToo )
			m_pOwner->RedrawRect(m_rRect, 1);
		else
			m_pOwner->RedrawChildControl(this, m_rRect);
		}
	}

void
ESChildControl::RedrawRect(_Rect rcRedraw, bool bLayersToo /*= true*/){
	if( !m_pOwner ) return;
	if( m_bCreated && !rcRedraw.IsRectEmpty() ){
#ifdef __APPLE__
        bLayersToo = true;
#endif
		if( bLayersToo ){
			m_pOwner->RedrawRect(rcRedraw, 1);
			}
		else{
			m_pOwner->RedrawChildControl(this, rcRedraw);
			}
		}
	}

void
ESChildControl::SetVisible(bool bVisible, bool bRedraw, bool bRememberRestoreHiddenChilds /*= false*/){
	if( bRememberRestoreHiddenChilds ){
		if( !bVisible && IsVisible() && GetChild() ){
			if( m_pArrChildIdsHidden )
				delete m_pArrChildIdsHidden;
			m_pArrChildIdsHidden = new CDWordArray();
			//Old code dont guess why ???????
			//GetChild()->GetChildsByVisibility(false, m_pArrChildIdsHidden);
			GetChildsByVisibility(false, m_pArrChildIdsHidden);
			}
		}

	if( bVisible ){
		// Do not allow to show when parent control is hidden
		if( GetParent() && !GetParent()->IsVisible() ){
			/*
			CDWordArray* pArrChildIdsHidden = GetParent()->m_pArrChildIdsHidden;
			if( pArrChildIdsHidden ){
				for(int i=0; i<pArrChildIdsHidden->GetCount(); i++){
					if( pArrChildIdsHidden->GetAt(i) == GetId() )
						return; // Not allow to make control visible.
					}
				}*/
			return;
			}
		}
	
	SetVisible_Rec(this, bVisible); 

	if( bRememberRestoreHiddenChilds && bVisible ){
		if( m_pArrChildIdsHidden ){
			for(int i=0; i<m_pArrChildIdsHidden->size(); i++){
				ESChildControl* pChild = m_pOwner->GetChildControl((int)m_pArrChildIdsHidden->at(i));
				if( pChild )
					pChild->SetVisible(false, false);
				}
			delete m_pArrChildIdsHidden;
			m_pArrChildIdsHidden = NULL;
			}
		}
	

	if( bRedraw )
		RedrawRect(m_rRect, true);
	}

void
ESChildControl::SetVisible_Rec(ESChildControl* pControl, bool bVisible){
	if( pControl ){
		if( pControl->VisibilityChanged(bVisible) ){
			pControl->m_bVisible = bVisible;
			pControl->SendEvent(this, EventCode_VisibilityChanged);
			if( m_pOwner )
				m_pOwner->OnChildVisibilityChanged(pControl, bVisible);

			ESChildControl* pChild = pControl->m_pChild;
			while( pChild ){
				SetVisible_Rec(pChild, bVisible);
				pChild = pChild->m_pNext;
				}
			}
		}
	}

bool
ESChildControl::GetPosInPixels(_Point& ptPos){
	if( !m_pOwner ) return false;
	if( m_bLogicalPos ){
		_Rect rcOwner;
		m_pOwner->GetClientRectMy(rcOwner);

		ptPos.x = (int)(rcOwner.Width() * m_fPosX);
		ptPos.y = (int)(rcOwner.Height() * m_fPosY);
		}
	else{
		ptPos.x = m_rRect.left;
		ptPos.y = m_rRect.top;
		}
	return true;
	}

bool
ESChildControl::GetSizeInPixels	(_Size& szSize){
	if( !m_pOwner ) return false;
	_Rect rcOwner;
	m_pOwner->GetClientRectMy(rcOwner);

	szSize.cx = (int)(rcOwner.Width() * m_fCX);
	szSize.cy = (int)(rcOwner.Height() * m_fCY);
	return true;
	}

void
ESChildControl::MoveChildControls(int nOffsetX, int nOffsetY){
	ESChildControl* pChild = m_pChild;
	_Point			ptPos(0, 0);
	_Rect			rcClient;
	m_pOwner->GetClientRectMy(rcClient);

	while( pChild ){	
		/*
		if( pChild->m_pChild )
			pChild->MoveChildControls(nOffsetX, nOffsetY);
		*/
		if( !m_pOwner->IsDesignerMode() || !pChild->IsLocked() ){
			pChild->GetPosInPixels(ptPos);
			ptPos.x		+= nOffsetX;
			ptPos.y		+= nOffsetY;

			if( pChild->HasLogicalPos() ){
				//pChild->SetLogicalPos((ptPos.x / ((float)rcClient.Width())), (ptPos.y / ((float)rcClient.Height())));
				}
			else
				pChild->SetPos(ptPos.x, ptPos.y, false);
			pChild->OwnerWindowSizeChanged	(rcClient.Width(), rcClient.Height());
			}

		if( pChild->m_pChild )
			pChild->MoveChildControls(nOffsetX, nOffsetY);

		pChild = pChild->m_pNext;
		}
	}

bool
ESChildControl::SetTimer(UINT nIdEvent, UINT nElapse){
	if( !m_pOwner ) return false;
	return m_pOwner->SetChildControlTimer(this, nIdEvent, nElapse);
	}

bool
ESChildControl::KillTimer(UINT nIdEvent){
	if( !m_pOwner ) return false;
	return m_pOwner->KillChildControlTimer(this, nIdEvent);
	}

int
ESChildControl::GetChildsByVisibility(bool bVisibility, CDWordArray* pArrControlIds){
	GetChildsByVisibility_Rec(bVisibility, pArrControlIds);
	return (int)pArrControlIds->size();
	}

void
ESChildControl::GetChildsByVisibility_Rec(bool bVisibility, CDWordArray* pArrControlIds){
	ESChildControl* pControl = GetChild();
	while( pControl ){
		if( pControl->IsVisible() == bVisibility )
			pArrControlIds->push_back((DWORD)pControl->GetId());
		if( pControl->GetChild() )
			pControl->GetChild()->GetChildsByVisibility_Rec(bVisibility, pArrControlIds);
		pControl = pControl->GetNext();
		}
	}

int
ESChildControl::_SizeBoxHitTest(_Point pt){
	if( !m_bVisible ) return -1;
	if( !m_rRect.PtInRect(pt) )
		return -1;

	int			nBoxCXCY	= 6;
	_Rect		rcSign;
	rcSign.left		= m_rRect.left;
	rcSign.top		= m_rRect.top;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::LeftTop;

	rcSign.left		= m_rRect.left + (m_rRect.Width() - nBoxCXCY) / 2;
	rcSign.top		= m_rRect.top;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::MiddleTop;

	rcSign.left		= m_rRect.right - nBoxCXCY;
	rcSign.top		= m_rRect.top;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::RightTop;

	rcSign.left		= m_rRect.right - nBoxCXCY;
	rcSign.top		= m_rRect.top + (m_rRect.Height() - nBoxCXCY) / 2;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::RightCenter;

	rcSign.left		= m_rRect.right - nBoxCXCY;
	rcSign.top		= m_rRect.bottom - nBoxCXCY;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::RightBottom;

	rcSign.left		= m_rRect.left + (m_rRect.Width() - nBoxCXCY) / 2;
	rcSign.top		= m_rRect.bottom - nBoxCXCY;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::MiddleBottom;

	rcSign.left		= m_rRect.left;
	rcSign.top		= m_rRect.bottom - nBoxCXCY;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::LeftBottom;

	rcSign.left		= m_rRect.left;
	rcSign.top		= m_rRect.top + (m_rRect.Height() - nBoxCXCY) / 2;
	rcSign.right	= rcSign.left + nBoxCXCY;
	rcSign.bottom	= rcSign.top + nBoxCXCY;
	if( rcSign.PtInRect(pt) )
		return ChildControlHitTest::LeftCenter;

	return -1;
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

bool
ESChildControl::CalcClientRectangle(int nOwnerCX, int nOwnerCY, _Rect& rcClient, ESChildControl* pParent){
	if( m_bLogicalPos ){
		_Point ptPos;
		ptPos.x = (int)(nOwnerCX * m_fPosX);
		ptPos.y = (int)(nOwnerCY * m_fPosY);

		_Size szSize;
		szSize.cx = (int)(nOwnerCX * m_fCX);
		szSize.cy = (int)(nOwnerCY * m_fCY);
		rcClient.SetRect(ptPos.x, ptPos.y, ptPos.x + szSize.cx, ptPos.y + szSize.cy);
		}
	else{
		rcClient			= m_rRect;
		_Rect rcFixedPos	= m_rcFixedPosition;
		_Rect rcRelative;

		if( m_pTopRelativeControl /*&& m_pTopRelativeControl->IsVisible()*/ ){
			rcRelative			= m_pTopRelativeControl->GetClientRect();
			rcClient.top		= (rcRelative.bottom + rcFixedPos.top);
			rcFixedPos.top		= -1;
			}

		if( m_pBottomRelativeControl /*&& m_pBottomRelativeControl->IsVisible()*/ ){
			rcRelative			= m_pBottomRelativeControl->GetClientRect();
			rcClient.bottom		= (rcRelative.top - rcFixedPos.bottom);
			rcFixedPos.bottom		= -1;
			}

		if( m_pLeftRelativeControl /*&& m_pLeftRelativeControl->IsVisible()*/ ){
			rcRelative			= m_pLeftRelativeControl->GetClientRect();
			rcClient.left		= (rcRelative.right + rcFixedPos.left);
			rcFixedPos.left		= -1;
			}

		if( m_pRightRelativeControl /*&& m_pRightRelativeControl->IsVisible()*/ ){
			rcRelative			= m_pRightRelativeControl->GetClientRect();
			rcClient.right		= (rcRelative.left - rcFixedPos.left);
			rcFixedPos.right	= -1;
			}

		if( !pParent ){
			if( rcFixedPos.left > -1 && rcFixedPos.right > -1 ){
				rcClient.left	= rcFixedPos.left;
				rcClient.right = (nOwnerCX - rcFixedPos.right);
				}
			else{
				if( rcFixedPos.left > -1 ){
					rcClient.left	= rcFixedPos.left;
					rcClient.right = rcClient.left + m_rRect.Width();
					}
				else
				if( rcFixedPos.right > -1 ){
					rcClient.right = (nOwnerCX - rcFixedPos.right);
					rcClient.left = rcClient.right - m_rRect.Width();
					}
				}

			if( rcFixedPos.top > -1 && rcFixedPos.bottom > -1 ){
				rcClient.top	= rcFixedPos.top;
				rcClient.bottom	= (nOwnerCY - rcFixedPos.bottom);
				}
			else{
				if( rcFixedPos.top > -1 ){
					rcClient.top	= rcFixedPos.top;
					rcClient.bottom	= rcClient.top + m_rRect.Height();
					}
				else
				if( rcFixedPos.bottom > -1 ){
					rcClient.bottom	= (nOwnerCY - rcFixedPos.bottom);
					rcClient.top	= rcClient.bottom - m_rRect.Height();
					}
				}
			}
		else{
			_Rect rcParent = pParent->GetClientRect();
			if( rcFixedPos.left > -1 && rcFixedPos.right > -1 ){
				rcClient.left	= (rcParent.left + rcFixedPos.left);
				rcClient.right = (rcParent.right - rcFixedPos.right);
				}
			else{
				if( rcFixedPos.left > -1 ){
					rcClient.left	= (rcParent.left + rcFixedPos.left);
					rcClient.right = rcClient.left + m_rRect.Width();
					}
				else
				if( rcFixedPos.right > -1 ){
					rcClient.right = (rcParent.right - rcFixedPos.right);
					rcClient.left = rcClient.right - m_rRect.Width();
					}
				}

			if( rcFixedPos.top > -1 && rcFixedPos.bottom > -1 ){
				rcClient.top		= (rcParent.top + rcFixedPos.top);
				rcClient.bottom	= (rcParent.bottom - rcFixedPos.bottom);
				}
			else{
				if( rcFixedPos.top > -1 ){
					rcClient.top		= (rcParent.top + rcFixedPos.top);
					rcClient.bottom		= rcClient.top + m_rRect.Height();
					}
				else
				if( rcFixedPos.bottom > -1 ){
					rcClient.bottom		= (rcParent.bottom - rcFixedPos.bottom);
					rcClient.top		= rcClient.bottom - m_rRect.Height();
					}
				}
			}
		}
	return true;
	}

void
ESChildControl::OwnerWindowSizeChanged(int cx, int cy){
	_Rect rRectOld = GetClientRect(), rRectNew;
	CalcClientRectangle(cx, cy, rRectNew, m_pParent);

	if( rRectOld.Width() != rRectNew.Width() || rRectOld.Height() != rRectNew.Height() )
		SetSize(rRectNew.Width(), rRectNew.Height(), false);

	if( !GetParent() ){
		// Move child controls.
		MoveChildControls((rRectNew.left - rRectOld.left), (rRectNew.top - rRectOld.top));
		}

	/*
	if( rRectOld.Width() != rRectNew.Width() || rRectOld.Height() != rRectNew.Height() )
		SetSize(rRectNew.Width(), rRectNew.Height(), false);
	*/

	m_rRect = rRectNew;
	if( rRectOld != m_rRect ){
		SendEvent(this, ChildControlEventCode::EventCode_ClientRectChanged, 0);
		m_pOwner->OnChildRectAreaChanged(this);
		}
/*
	if( m_bLogicalPos ){
		CalcClientRectangle(cx, cy, m_rRect, m_pParent);
		rRectNew = GetClientRect();

		// Move child controls.
		//MoveChildControls((rRectNew.left - rRectOld.left), (rRectNew.top - rRectOld.top));
		if( rRectOld != m_rRect ){
			SendEvent(this, ChildControlEventCode::EventCode_ClientRectChanged, 0);
			m_pOwner->OnChildRectAreaChanged(this);
			}
		}
	else{
		CalcClientRectangle(cx, cy, rRectNew, m_pParent);
		// Move child controls.
		MoveChildControls((rRectNew.left - m_rRect.left), (rRectNew.top - m_rRect.top));
		if( m_rRect.Width() != rRectNew.Width() || m_rRect.Height() != rRectNew.Height() )
			SetSize(rRectNew.Width(), rRectNew.Height(), false);
		m_rRect = rRectNew;
		if( rRectOld != m_rRect ){
			SendEvent(this, ChildControlEventCode::EventCode_ClientRectChanged, 0);
			m_pOwner->OnChildRectAreaChanged(this);
			}
		}*/
	}

void
ESChildControl::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	if( m_pOwner->IsDesignerMode() && m_pOwner->IsControlSelected(this) ){
		_Rect	rcControlDC, rcInvalid;
		rcControlDC.left	= (m_rRect.left - pRectClient->left) + pRectDC->left; 
		rcControlDC.top		= (m_rRect.top - pRectClient->top) + pRectDC->top; 
		rcControlDC.right	= rcControlDC.left + m_rRect.Width(); 
		rcControlDC.bottom	= rcControlDC.top + m_rRect.Height();
		
		COLORREF	crSelection = RGB(63, 84, 122);
		int			nBoxCXCY	= 6;

		_Rect		rcFill;
		rcFill.left		= rcControlDC.left;
		rcFill.top		= rcControlDC.top;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);

		rcFill.left		= rcControlDC.left + (rcControlDC.Width() - nBoxCXCY) / 2;
		rcFill.top		= rcControlDC.top;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);

		rcFill.left		= rcControlDC.right - nBoxCXCY;
		rcFill.top		= rcControlDC.top;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);

		rcFill.left		= rcControlDC.right - nBoxCXCY;
		rcFill.top		= rcControlDC.top + (rcControlDC.Height() - nBoxCXCY) / 2;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);

		rcFill.left		= rcControlDC.right - nBoxCXCY;
		rcFill.top		= rcControlDC.bottom - nBoxCXCY;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);

		rcFill.left		= rcControlDC.left + (rcControlDC.Width() - nBoxCXCY) / 2;
		rcFill.top		= rcControlDC.bottom - nBoxCXCY;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);

		rcFill.left		= rcControlDC.left;
		rcFill.top		= rcControlDC.bottom - nBoxCXCY;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);

		rcFill.left		= rcControlDC.left;
		rcFill.top		= rcControlDC.top + (rcControlDC.Height() - nBoxCXCY) / 2;
		rcFill.right	= rcFill.left + nBoxCXCY;
		rcFill.bottom	= rcFill.top + nBoxCXCY;
		pDC->FillSolidRect(rcFill, crSelection);
		}
	}

void	
ESChildControl::OnMouseHover(_Point pt, UINT nFlags){
	}

void	
ESChildControl::OnMouseLeave(UINT nFlags){
	SendEvent(this, EventCode_ControlMouseLeave, (void*)nFlags);
	}

void	
ESChildControl::OnMouseEnter(UINT nFlags){
	SendEvent(this, EventCode_ControlMouseEnter, (void*)nFlags);
	}

BOOL
ESChildControl::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
	return FALSE;
	}

void
ESChildControl::OnLButtonDown(_Point pt, UINT nFlags){
	m_bMouseCaptured	= 1;
	m_ptMouseDown		= pt;
	}

void
ESChildControl::OnLButtonUp(_Point pt, UINT nFlags){
	m_bMouseCaptured	= 0;
	m_ptMouseDown		= pt;
	}

void	
ESChildControl::OnLButtonClick(_Point pt, UINT nFlags){
	}

void
ESChildControl::OnRButtonDown(_Point pt, UINT nFlags){
	}

void
ESChildControl::OnRButtonUp(_Point pt, UINT nFlags){
	}

void	
ESChildControl::OnRButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControl::OnMButtonClick(_Point pt, UINT nFlags){
	}

void	
ESChildControl::OnLButtonDblClick(_Point pt, UINT nFlags){
	m_bMouseCaptured	= 1;
	m_ptMouseDown		= pt;
	}

void	
ESChildControl::OnRButtonDblClick(_Point pt, UINT nFlags){
	}

void	
ESChildControl::OnMButtonDblClick(_Point pt, UINT nFlags){
	}

void
ESChildControl::AddListenEventControl(ESChildControl* pControlListen){
	if( m_arrControlsEventListening.IndexOf((void*)pControlListen) == -1 )
		m_arrControlsEventListening.Add((void*)pControlListen);
	}

bool
ESChildControl::PtInClientArea(_Point pt){
	_Size	szRgnBound;
	bool	bCentered	= false;
	RGNDef	hRgn		= GetControlRgn(szRgnBound, bCentered);
	bool	bRet		= false;

	if( hRgn ){
		if( GetClientRect().PtInRect(pt) )
			bRet = MercuryBaseView::PtInImageRegion(pt, this, hRgn, szRgnBound, bCentered);
		}
	else
		bRet = (GetClientRect().PtInRect(pt) == TRUE);
	return bRet;
	}

bool
ESChildControl::AddEventListener(ESFrameBase* pReceiver, int eventCode, ControlEventListener method){
	if( !m_bCreated ){
		ASSERT(FALSE);
		return false;
		}

	List<ChildEventListenerInfo>* pListeners = NULL;
	int nIndex = m_arrListenerByEventCode.IndexOf((void*)eventCode);
	if( nIndex == -1 ){
		pListeners = new List<ChildEventListenerInfo>();
		m_arrListenerByEventCode.Add((void*)eventCode, (void*)pListeners);
		}
	else{
		pListeners = (List<ChildEventListenerInfo>*)m_arrListenerByEventCode.GetData(nIndex);
		if( !pListeners ){
			pListeners = new List<ChildEventListenerInfo>();
			m_arrListenerByEventCode.SetData(nIndex, (void*)pListeners);
			}
		}

	// Check for existance.
	for(int i=0; i<pListeners->GetCount(); i++){
		ChildEventListenerInfo* pInfo = pListeners->GetAt(i);
		if( pInfo && pInfo->method == method && pInfo->pReceiver == pReceiver )
			return true;
		}

	ChildEventListenerInfo* pInfo	= new ChildEventListenerInfo();
	pInfo->method					= method;
	pInfo->pSender					= this;
	pInfo->pReceiver				= pReceiver;
	pListeners->Add(pInfo);

	if( m_pOwner->IsValidChildControl((ESChildControl*)pReceiver) )
		((ESChildControl*)pReceiver)->AddListenEventControl(this);
	return true;
	}

bool
ESChildControl::SendEvent(ESChildControl* pSender, int eventCode, void* lpParam /*= 0*/){
	List<ChildEventListenerInfo>* pListeners = NULL;
	int nIndex = m_arrListenerByEventCode.IndexOf((void*)eventCode);
	if( nIndex == -1 ){
		return false;
		}
	else{
		pListeners = (List<ChildEventListenerInfo>*)m_arrListenerByEventCode.GetData(nIndex);
		if( !pListeners ) return false;
		}

	bool	bRet = false;
	int		nLoop = 0, nCt = pListeners->GetCount();

	while( nLoop < nCt ){
		ChildEventListenerInfo* pInfo = pListeners->GetAt(nLoop);
		if( pInfo && pInfo->pSender == pSender && pInfo->method != NULL ){
			void*   pThis	= pInfo->pReceiver;
			ControlEventListener	method	= pInfo->method;
            (((ESFrameBase*)pThis)->*method)(pInfo->pSender, (ChildControlEventCode)eventCode, lpParam);
		/*
			// Call thiscall method. {{
			__asm{
				push eax;
				push ecx;
				mov eax, pThis;
				mov ecx, eax;				// Place this pointer
				mov eax, nParam;
				push eax;
				mov eax, eventCode;
				push eax;
				mov eax, pSender;
				push eax;
				mov eax, method;
				call eax;
				pop ecx;
				pop eax;
				};
			// }}
*/
			bRet = true;
			}
		nLoop ++;
		}
	return bRet;
	}

bool
ESChildControl::RemoveEvents(ESChildControl* pReceiver){
	int nLoop = 0, nCt = m_arrListenerByEventCode.GetCount();
	while( nLoop < nCt ){
		List<ChildEventListenerInfo>* pListeners = (List<ChildEventListenerInfo>*)m_arrListenerByEventCode.GetData(nLoop);
		if( pListeners ){
			int nLoop1 = 0, nCt1 = pListeners->GetCount();
			while( nLoop1 < nCt1 ){
				ChildEventListenerInfo* pInfo = pListeners->GetAt(nLoop1);
				if( pInfo && (pInfo->pReceiver == (void*)pReceiver) ){
					pListeners->RemoveAt(nLoop1);
					nCt1 --;
					continue;
					}
				nLoop1 ++;
				}
			}
		nLoop ++;
		}

	int nIndex = m_arrControlsEventListening.IndexOf((void*)pReceiver);
	if( nIndex != -1 )
		m_arrControlsEventListening.Delete(nIndex);
	return true;
	}

void
ESChildControl::RemoveAllEvents(){
	int nLoop = 0, nCt = m_arrListenerByEventCode.GetCount();
	// Remove all provided events.
	while( nLoop < nCt ){
		List<ChildEventListenerInfo>* pListeners = (List<ChildEventListenerInfo>*)m_arrListenerByEventCode.GetData(nLoop);
		if( pListeners ){
			for(int i=0; i<pListeners->GetCount(); i++){
				ChildEventListenerInfo* pInfo = pListeners->GetAt(i);
				if( pInfo && pInfo->pReceiver && m_pOwner->IsValidChildControl((ESChildControl*)pInfo->pReceiver) )
					((ESChildControl*)pInfo->pReceiver)->RemoveEvents(this);
				}

			pListeners->RemoveAll(true);
			delete pListeners;
			}
		nLoop ++;
		}
	m_arrListenerByEventCode	.DeleteAll();
	m_arrControlsEventListening	.DeleteAll();
	}

/////////////////////////////////////////////////////////////////////////////
// Variable modify callbacks.

bool
ESChildControl::NameModify(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew){
	return true;
	}

bool
ESChildControl::ZIndexModify(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew){
	return true;
	}

bool
ESChildControl::VisibilityModify(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew){
	if( pThis->m_pOwner )
		pThis->m_pOwner->OnChildVisibilityChanged(pThis, *((bool*)lpValueNew));
	return true;
	}

bool
ESChildControl::ControlRectModify(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew){
	return true;
	}

bool
ESChildControl::AllowMouseEventsModify(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew){
	return true;
	}

bool
ESChildControl::FixedPositionModify(ESChildControl* pThis, VariableInfo* pInfo, void* lpValueNew){
	MercuryBaseView* pOwner = pThis->m_pOwner;
	_Rect rcOwner;
	pOwner->GetClientRectMy(rcOwner);

	_Rect rcClientOld = pThis->GetClientRect();
	pThis->OwnerWindowSizeChanged(rcOwner.Width(), rcOwner.Height());
	_Rect rcClientNew = pThis->GetClientRect();

	if( rcClientOld != rcClientNew ){
		pOwner->RedrawRect(rcClientOld, 1);
		pOwner->RedrawRect(rcClientNew, 1);
		}
	return true;
	}
