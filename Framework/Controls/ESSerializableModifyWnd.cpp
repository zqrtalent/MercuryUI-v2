// ESSerializableModifyWnd.cpp : implementation file
//

#include "ESSerializableModifyWnd.h"
#include "ESChildControlPropertyGrid.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/////////////////////////////////////////////////////////////////////////////
// PokerLobbyWnd

ESSerializableModifyWnd::ESSerializableModifyWnd() : MercuryBaseView(){
	m_bUseForegroundColor	= 0;
	m_crForeground			= RGB(0, 0, 0);
	m_nAlpha				= 50;
	m_bDesignerMode			= false;
	}

ESSerializableModifyWnd::~ESSerializableModifyWnd(){
	}

void
ESSerializableModifyWnd::SetSerializableObject(Serializable* pObject){
	ESChildControlPropertyGrid* pControl = (ESChildControlPropertyGrid*)GetChildControl(1000);
	if( pControl ){
		pControl->SetSerializableObject(pObject);
		}
	}

void
ESSerializableModifyWnd::SetListOfObjects(EnumerableObject<Serializable>* pListOfObjects){
	ESChildControlPropertyGrid* pControl = (ESChildControlPropertyGrid*)GetChildControl(1000);
	if( pControl )
		pControl->SetListOfObjects(pListOfObjects);
	}

bool
ESSerializableModifyWnd::AttachToWindow(ESFrameBase* pBaseFrameWnd){
	bool bRet = false;// MercuryBaseView::AttachToWindow(pBaseFrameWnd);
	if( bRet ){
		_Rect rcClient;
		GetClientRectMy(rcClient);
		// Create child controls.
		CreateChildControls();
		}
	return bRet;
	}

void
ESSerializableModifyWnd::OnPrePaint(_Rect& rcRepaint){
	}

void 
ESSerializableModifyWnd::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect rcClipDC = *pRectDC;
//	_Rect rcDestDC(-1*pRectClient->left, -1*pRectClient->top, pRectDC->Width(), pRectDC->Height());
//	DrawBackground		(pDC, &rcDestDC, &rcClipDC);
	RedrawChildControls	(m_pFirstChild, pDC, pRectDC, pRectClient);
	}

// ######################################################
//
//	Operations.
//
// ######################################################

bool
ESSerializableModifyWnd::CreateChildControls(){
	_Rect rcClient;
	GetClientRectMy(rcClient);

	ESChildControlPropertyGrid *pGridControl = new ESChildControlPropertyGrid();
	if( !AddChildControl(pGridControl, 1000, rcClient, NULL) ){
		delete pGridControl;
		ESChildControl* pControl = GetChildControl(1000);
		if( pControl ){
			pControl->SetPos(rcClient.left, rcClient.top, false);
			pControl->SetSize(rcClient.Width(), rcClient.Height(), false);
			}
		}
	// }}

	return true;
	}