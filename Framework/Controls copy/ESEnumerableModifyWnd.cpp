// ESEnumerableModifyWnd.cpp : implementation file
//

#include "ESEnumerableModifyWnd.h"
#include "ESChildControlList.h"
#include "ESChildControlImageButton.h"

#include "../Data/VRowSet.h"
#include "../Data/DataFieldCopier.h"

/////////////////////////////////////////////////////////////////////////////
// ESEnumerableModifyWnd

ESEnumerableModifyWnd::ESEnumerableModifyWnd() : MercuryBaseView(){
	m_bUseForegroundColor	= 0;
	m_crForeground			= RGB(0, 0, 0);
	m_nAlpha				= 50;
	m_bDesignerMode			= false;

	m_pAddButton			= NULL;
	m_pDeleteButton			= NULL;
	m_pMoveUpButton			= NULL;
	m_pMoveDownButton		= NULL;

	m_pListRowSet			= NULL;
	m_pEnumerable			= NULL;
	}

ESEnumerableModifyWnd::~ESEnumerableModifyWnd(){
	// Copy from list rowset into enumerable object. {{
	if( m_pList && m_pListRowSet && m_pEnumerable ){
		Serializable* pObject = m_pEnumerable->CreateSerializableObject();
		DataFieldCopier copier(m_pListRowSet, pObject, true); 
		m_pEnumerable->RemoveAll(true);
		copier.CopyFromVRowIntoEnumerable(m_pListRowSet, m_pEnumerable);
		}
	// }}
	}

void
ESEnumerableModifyWnd::SetEnumerableObject(EnumerableObject<Serializable>* pEnumerable){
	ASSERT(pEnumerable);
	if( !pEnumerable ) return;

	ESChildControlList* pList = (ESChildControlList*)GetChildControl(1000);
	if( pList ){
		Serializable* pObject = pEnumerable->CreateSerializableObject();
		ASSERT(pObject);
		VariableInfo* pInfo = pObject->GetThisInfo();

		MercuryGUI*	pGUI		= MercuryGUI::GetInstance();
		_Size		szText(0, 0);
		_string		sTitle;

		int nLoop = 1;
		while( pInfo[nLoop].m_sAttributeName.length() > 0 ){
			if( !pInfo[nLoop].m_bComplexType ){
				sTitle = pInfo[nLoop].m_sDbFieldName;
				pGUI->header.m_fontItemText.GetTextSize(&sTitle, szText);
				if( szText.cx <= 0 )
					szText.cx = 80;
				pList->AddColumn(pInfo[nLoop].m_sDbFieldName, pInfo[nLoop].m_sAttributeName, HAlignment::HAlignement_Left, szText.cx + 5, szText.cx + 5);
				szText.cx = 0;
				}
			nLoop ++;
			}

		VirtualRowSet* pRowSet	= Serializable::SerializableObjectToVRowSet(pEnumerable);
		m_pListRowSet			= pRowSet;
		m_pEnumerable			= pEnumerable;
		pList->SetRowSet(pRowSet, true, true);
		delete pObject;
		}
	}

bool
ESEnumerableModifyWnd::AttachToWindow(ESFrameBase* pBaseFrameWnd){
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
ESEnumerableModifyWnd::OnPrePaint(_Rect& rcRepaint){
	}

void 
ESEnumerableModifyWnd::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	RedrawChildControls	(m_pFirstChild, pDC, pRectDC, pRectClient);
	}

// ######################################################
//
//	Operations.
//
// ######################################################

bool
ESEnumerableModifyWnd::CreateChildControls(){
	_Rect rcClient;
	GetClientRectMy(rcClient);

	m_info.m_bgFillMode.SetCurrentValue(_FillMode::FillMode_Color);
	m_info.m_crFill	= RGB(255, 255, 255);

	_Rect rcList;
	rcList.left		= rcClient.left;
	rcList.top		= rcClient.top;
	rcList.right	= rcClient.right;
	rcList.bottom	= rcClient.bottom - 50;

	ESChildControlList *pList = new ESChildControlList();
	if( !AddChildControl(pList, 1000, rcList, NULL) ){
		delete pList;
		}
	else{
		m_pList = pList;
		pList->AddEventListener	(this, ChildControlEventCode::EventCode_ListRowChange, (ControlEventListener)&ESEnumerableModifyWnd::OnListRowChange);
		pList->SetBorder		(FALSE);
		pList->AllowColumnSort	(false);
		pList->SetFixedPosition	(_Rect(0, 0, 0, 50));
		}
	// }}

	_Size szButton(100, 34);

	_Rect rcAddImageButton;
	rcAddImageButton.left	= 5;
	rcAddImageButton.top	= rcList.bottom + 5;
	rcAddImageButton.right	= rcAddImageButton.left + szButton.cx;
	rcAddImageButton.bottom = rcAddImageButton.top + szButton.cy;

	ESChildControlImageButton *pAddButton = new ESChildControlImageButton();
	if( !AddChildControl(pAddButton, 1001, rcAddImageButton, NULL) ){
		delete pAddButton;
		}
	else{
		m_pAddButton = pAddButton;
		pAddButton->AddEventListener	(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&ESEnumerableModifyWnd::OnAdd);
		pAddButton->SetFixedPosition	(_Rect(rcAddImageButton.left, -1, -1, (rcClient.bottom - rcAddImageButton.bottom)));
		pAddButton->SetText				(_T("Add"));
		}

	_Rect rcDeleteImageButton;
	rcDeleteImageButton.left	= rcAddImageButton.right + 5;
	rcDeleteImageButton.top		= rcList.bottom + 5;
	rcDeleteImageButton.right	= rcDeleteImageButton.left + szButton.cx;
	rcDeleteImageButton.bottom	= rcDeleteImageButton.top + szButton.cy;

	ESChildControlImageButton *pDelButton = new ESChildControlImageButton();
	if( !AddChildControl(pDelButton, 1002, rcDeleteImageButton, NULL) ){
		delete pDelButton;
		}
	else{
		m_pDeleteButton = pDelButton;
		pDelButton->AddEventListener	(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&ESEnumerableModifyWnd::OnDelete);
		pDelButton->SetFixedPosition	(_Rect(rcDeleteImageButton.left, -1, -1, (rcClient.bottom - rcDeleteImageButton.bottom)));
		pDelButton->DisableButton		(false);
		pDelButton->SetText				(_T("Delete"));
		}

	_Rect rcMoveUpImageButton;
	rcMoveUpImageButton.left	= rcDeleteImageButton.right + 5;
	rcMoveUpImageButton.top		= rcList.bottom + 5;
	rcMoveUpImageButton.right	= rcMoveUpImageButton.left + szButton.cx;
	rcMoveUpImageButton.bottom	= rcMoveUpImageButton.top + szButton.cy;

	ESChildControlImageButton *pMoveUpButton = new ESChildControlImageButton();
	if( !AddChildControl(pMoveUpButton, 1003, rcMoveUpImageButton, NULL) ){
		delete pMoveUpButton;
		}
	else{
		m_pMoveUpButton	= pMoveUpButton;
		pMoveUpButton->AddEventListener	(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&ESEnumerableModifyWnd::OnMoveUp);
		pMoveUpButton->SetFixedPosition	(_Rect(rcMoveUpImageButton.left, -1, -1, (rcClient.bottom - rcMoveUpImageButton.bottom)));
		pMoveUpButton->DisableButton	(false);
		pMoveUpButton->SetText			(_T("Move Up"));
		}

	_Rect rcMoveDownImageButton;
	rcMoveDownImageButton.left		= rcMoveUpImageButton.right + 5;
	rcMoveDownImageButton.top		= rcList.bottom + 5;
	rcMoveDownImageButton.right		= rcMoveDownImageButton.left + szButton.cx;
	rcMoveDownImageButton.bottom	= rcMoveDownImageButton.top + szButton.cy;

	ESChildControlImageButton *pMoveDownButton = new ESChildControlImageButton();
	if( !AddChildControl(pMoveDownButton, 1004, rcMoveDownImageButton, NULL) ){
		delete pMoveDownButton;
		}
	else{
		m_pMoveDownButton			= pMoveDownButton;
		pMoveDownButton->AddEventListener	(this, ChildControlEventCode::EventCode_ImageButtonClick, (ControlEventListener)&ESEnumerableModifyWnd::OnMoveDown);
		pMoveDownButton->SetFixedPosition	(_Rect(rcMoveDownImageButton.left, -1, -1, (rcClient.bottom - rcMoveDownImageButton.bottom)));
		pMoveDownButton->SetText			(_T("Move Down"));
		pMoveDownButton->DisableButton		(false);
		}
	return true;
	}

void
ESEnumerableModifyWnd::OnListRowChange(ESChildControlList* pSender, ChildControlEventCode eventCode, void* lpParam /*= 0*/){
	int nRow = (int)((UINT_PTR)lpParam);
	if( m_pDeleteButton ){
		if( nRow > -1 ){
			if( m_pDeleteButton->IsDisabled() )
				m_pDeleteButton->EnableButton(true);
			}
		else{
			if( !m_pDeleteButton->IsDisabled() )
				m_pDeleteButton->DisableButton(true);
			}
		}

	if( m_pMoveUpButton ){
		if( nRow > 0  ){
			if( m_pMoveUpButton->IsDisabled() )
				m_pMoveUpButton->EnableButton(true);
			}
		else{
			if( !m_pMoveUpButton->IsDisabled() )
				m_pMoveUpButton->DisableButton(true);
			}
		}

	if( m_pMoveDownButton ){
		if( nRow < (pSender->GetRowCount() - 1) && nRow > -1  ){
			if( m_pMoveDownButton->IsDisabled() )
				m_pMoveDownButton->EnableButton(true);
			}
		else{
			if( !m_pMoveDownButton->IsDisabled() )
				m_pMoveDownButton->DisableButton(true);
			}
		}

	}

void
ESEnumerableModifyWnd::OnAdd(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam /*= 0*/){
	if( m_pList ){
		m_pList->AddEmptyRow(true);
		}
	}

void
ESEnumerableModifyWnd::OnDelete(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam /*= 0*/){
	if( m_pList ){
		m_pList->DeleteRow(m_pList->GetSelectedRow(), true);
		}
	}

void
ESEnumerableModifyWnd::OnMoveUp(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam /*= 0*/){
	}

void
ESEnumerableModifyWnd::OnMoveDown(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam /*= 0*/){
	}