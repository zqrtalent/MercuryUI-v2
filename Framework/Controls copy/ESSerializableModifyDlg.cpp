// ESSerializableModifyDlg.cpp : implementation file
//

#include "ESSerializableModifyDlg.h"
#include "ESChildControlPropertyGrid.h"

//////////////////////////////////////////////////////
//
//	class ESSerializableModifyDlg dialog
//
/////////////////////////////////////////////////////

ESSerializableModifyDlg::ESSerializableModifyDlg() : MercuryBaseView(){
	//m_hIcon					= ::LoadIconA(NULL, IDI_APPLICATION);
	m_fHeightAndWidthRatio	= 0.0f;

	m_szDefault	.SetSize(370, 450);
	m_szMin					= m_szDefault;
	m_szMax					= m_szDefault;

	m_pGrid					= NULL;
	m_pObject				= NULL;
	m_pListOfObjects		= NULL;
	}

bool
ESSerializableModifyDlg::SetSerializableObject(Serializable* pObject){
	bool bRet = false;
	if( m_pGrid ){
		m_pGrid->SetSerializableObject(pObject);
		bRet = true;
		}
	else
		m_pObject = pObject;

	stringEx::Format(m_info.m_sTitle, _T("%s"), pObject->GetClassName().c_str());
	if( IsWindow() ){
		//SetWindowText(m_hWnd, m_info.m_sTitle.c_str());
		}
	return bRet;
	}

bool
ESSerializableModifyDlg::SetListOfObjects(EnumerableObject<Serializable>* pListOfObjects){
	if( !pListOfObjects || !pListOfObjects->GetCount() )
		return false;

	m_info.m_sTitle = _T("List of objects");
	//if( IsWindow() )
	//	SetWindowText(m_hWnd, m_info.m_sTitle.c_str());

	if( m_pGrid ){
		m_pGrid->SetListOfObjects(pListOfObjects);
		return true;
		}
	else
		m_pListOfObjects = pListOfObjects;
	return false;
	}

// ESSerializableModifyDlg message handlers
void
ESSerializableModifyDlg::Setup(){
	// Create child window.
	MercuryBaseView::Setup();
    
    SetDesignerMode(false);

	_Rect rcClient;
	GetClientRectMy(rcClient);
	// Create child controls.
	CreateChildControls();

	if( m_pListOfObjects )
		SetListOfObjects(m_pListOfObjects);

	if( m_pObject )
		SetSerializableObject(m_pObject);
	}

void
ESSerializableModifyDlg::OnDestroy(){
	MercuryBaseView::OnDestroy();
	}


void
ESSerializableModifyDlg::OnClose(){
	//ShowWindow(SW_HIDE);
	//MercuryBaseView::OnClose();
	}

// ######################################################
//
//	Operations.
//
// ######################################################

bool
ESSerializableModifyDlg::CreateChildControls(){
	_Rect rcClient;
	GetClientRectMy(rcClient);

	m_pGrid = new ESChildControlPropertyGrid();
    m_pGrid->AllowMouseEvents(true);
    
	if( !AddChildControl(m_pGrid, 1000, rcClient, NULL) ){
		delete m_pGrid;
		ESChildControl* pControl = GetChildControl(1000);
		if( pControl ){
			pControl->SetPos(rcClient.left, rcClient.top, false);
			pControl->SetSize(rcClient.Width(), rcClient.Height(), false);
			}
		}
	// }}
	return true;
	}
