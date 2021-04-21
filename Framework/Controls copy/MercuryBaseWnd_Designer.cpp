// MercuryBaseWnd.cpp : implementation file
//

#include "ESChildControl.h"
#include "MercuryBaseWnd.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"
#include <memory.h>

#include "ESSerializableModifyDlg.h"

#include "ESChildControlLabel.h"
#include "ESChildControlImage.h"
#include "ESChildControlImageButton.h"
#include "ESChildControlList.h"
#include "ESChildTextBox.h"
#include "ESChildControlTabCtrl.h"
#include "ESChildBanner.h"
#include "ESChildControlCheckBox.h"
#include "ESChildScrollBar.h"
#include "ESChildControlPropertyGrid.h"
#include "ESChildTabView.h"
#include "ESChildComboBox.h"
#include "ESSimpleRenderView.h"
#include "ESChildSlider.h"
#include "ESChildProgressCtrl.h"
#include "ESChildAvatar.h"

// ####################################################
//
// class MercuryBaseView
//
// ####################################################

void
MercuryBaseView::OnCommandCommand(UINT_PTR nId){
    CommandProc cmdProc = m_mapManuCommandById.GetAt(nId);
    if(cmdProc != nullptr){
        (this->*cmdProc)();
    }
}

void
MercuryBaseView::OnCommandLabel(){
	ESChildControlLabel*	pControlNew = new ESChildControlLabel();
	if( AddNewControl(pControlNew, NULL) ){
		pControlNew->SetLabel		(_T("New Label"), false);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandImage(){
	ESChildControlImage*	pControlNew = new ESChildControlImage();
	if( AddNewControl(pControlNew, NULL) ){
		pControlNew->SetImage(&MercuryGUI::GetInstance()->switchBar.m_imgItemActive, false);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void	
MercuryBaseView::OnCommandImageButton(){
	ESChildControlImageButton*	pControlNew = new ESChildControlImageButton();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->SetText("Image Button");
		pControlNew->Redraw	(false);
		}
	}

void	
MercuryBaseView::OnCommandTextBox(){
	ESChildTextBox*	pControlNew = new ESChildTextBox();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->SetText(_T("TextBox"), false, true);
		}
	}

void	
MercuryBaseView::OnCommandCheckBox(){
	ESChildControlCheckBox*	pControlNew = new ESChildControlCheckBox();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->SetCheck(true, true);
		}
	}

void	
MercuryBaseView::OnCommandComboBox(){
	ESChildComboBox*	pControlNew = new ESChildComboBox();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void	
MercuryBaseView::OnCommandSlider(){
	ESChildSlider*	pControlNew = new ESChildSlider();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void	
MercuryBaseView::OnCommandProgressCtrl(){
	ESChildProgressCtrl*	pControlNew = new ESChildProgressCtrl();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void	
MercuryBaseView::OnCommandAvatar(){
	ESChildAvatar*	pControlNew = new ESChildAvatar();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void	
MercuryBaseView::OnCommandSimpleRenderView(){
	ESSimpleRenderView*	pControlNew = new ESSimpleRenderView();
	if( AddNewControl(pControlNew, NULL) ){
		/*
		pControlNew->SetBgColor		(RGB(241, 241, 241));
		pControlNew->AddTextStyle	(_T("STYLE1"), _T("Segoe UI"), false, false, true, 18);
		pControlNew->AddTextStyle	(_T("STYLE2"), _T("Segoe UI"), false, false, false, 16);
		pControlNew->AddTextStyle	(_T("STYLE3"), _T("Lucida Calligraphy"), false, false, false, 18);
		pControlNew->AddTextStyle	(_T("STYLE4"), _T("Segoe UI"), false, false, false, 18);
		pControlNew->AddTextStyle	(_T("STYLE5"), _T("Tahoma"), false, false, false, 12);

		_Rect rcListPlayers(0, 0, 200, 40);

		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE1"), _T("Tournament starts at: 1111"), RGB(0, 0, 0), _Size(100, 16), 0.5f, 0.0f, VAlign_Middle, HAlign_Center);
		pControlNew->AddText	(_T("STYLE4"), _T("Tournament starts end: "), RGB(0, 0, 0), _Size(100, 16), 0.4f, 0.6f, VAlign_Middle, HAlign_Center);
		
		
		pControlNew->AddText	(_T("STYLE2"), _T("Feb 01 2013 18:20"), RGB(0, 0, 0), _Size(rcListPlayers.Width(), 16), 0.5f, 0.25f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();

		pControlNew->AddText	(_T("STYLE1"), _T("Registration starts at: "), RGB(0, 0, 0), _Size(rcListPlayers.Width(), 16), 0.5f, 0.25f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE2"), _T("Feb 01 2013 18:20"), RGB(0, 0, 0), _Size(rcListPlayers.Width(), 16), 0.5f, 0.25f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddGotoNextLine();

		pControlNew->AddText	(_T("STYLE2"), _T("Buy-in: 0.12 GEL"), RGB(0, 0, 0), _Size(rcListPlayers.Width(), 16), 0.5f, 0.25f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		pControlNew->AddText	(_T("STYLE2"), _T("Prize pool: 1000 GEL"), RGB(0, 0, 0), _Size(rcListPlayers.Width(), 16), 0.5f, 0.25f, VAlign_Middle, HAlign_Center);
		pControlNew->AddGotoNextLine();
		*/
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void	
MercuryBaseView::OnCommandList(){
	ESChildControlList*	pControlNew = new ESChildControlList();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void	
MercuryBaseView::OnCommandTree(){
	}

void	
MercuryBaseView::OnCommandTab(){
	ESChildControlTabCtrl*	pControlNew = new ESChildControlTabCtrl();
	if( AddNewControl(pControlNew, NULL) ){
		pControlNew->AddTabItem(_T("Item1"), 0);
		pControlNew->AddTabItem(_T("Item2"), 0);
		pControlNew->AddTabItem(_T("Item3"), 0);
		pControlNew->AddTabItem(_T("Item4"), 0);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void	
MercuryBaseView::OnCommandTabView(){
	ESChildTabView*	pControlNew = new ESChildTabView();
	if( AddNewControl(pControlNew, NULL) ){
		pControlNew->AddTabItem(_T("Item1"), 0);
		pControlNew->AddTabItem(_T("Item2"), 0);
		pControlNew->AddTabItem(_T("Item3"), 0);
		pControlNew->AddTabItem(_T("Item4"), 0);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void	
MercuryBaseView::OnCommandTreeView(){
	}

void	
MercuryBaseView::OnCommandBanner(){
	ESChildBanner*	pControlNew = new ESChildBanner();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void	
MercuryBaseView::OnCommandPropertyGrid(){
	ESChildControlPropertyGrid*	pControlNew = new ESChildControlPropertyGrid();
	if( AddNewControl(pControlNew, NULL) ){
		AddInSelectedList(pControlNew);
		pControlNew->SetSerializableObject	(pControlNew, false);
		pControlNew->Redraw					(false);
		}
	}

void
MercuryBaseView::OnCommandLabelAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlLabel*	pControlNew = new ESChildControlLabel();
	if( AddNewControl(pControlNew, pLastSelected) ){
		pControlNew->SetLabel		(_T("New Label"), false);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandImageAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlImage*	pControlNew = new ESChildControlImage();
	if( AddNewControl(pControlNew, pLastSelected) ){
		pControlNew->SetImage(&MercuryGUI::GetInstance()->switchBar.m_imgItemActive, false);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandImageButtonAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlImageButton*	pControlNew = new ESChildControlImageButton();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->SetText("Image Button");
		pControlNew->Redraw	(false);
		}
	}

void
MercuryBaseView::OnCommandTextBoxAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildTextBox*	pControlNew = new ESChildTextBox();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->SetText(_T("TextBox"), false, true);
		}
	}

void
MercuryBaseView::OnCommandCheckBoxAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlCheckBox*	pControlNew = new ESChildControlCheckBox();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandComboBoxAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildComboBox*	pControlNew = new ESChildComboBox();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandSliderAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildSlider*	pControlNew = new ESChildSlider();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandProgressCtrlAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildProgressCtrl*	pControlNew = new ESChildProgressCtrl();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandAvatarAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildAvatar*	pControlNew = new ESChildAvatar();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandSimpleRenderViewAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESSimpleRenderView*	pControlNew = new ESSimpleRenderView();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandListAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlList*	pControlNew = new ESChildControlList();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandTreeAsChild(){
	}

void
MercuryBaseView::OnCommandTabAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;
	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlTabCtrl*	pControlNew = new ESChildControlTabCtrl();
	if( AddNewControl(pControlNew, pLastSelected) ){
		pControlNew->AddTabItem(_T("Item1"), 0);
		pControlNew->AddTabItem(_T("Item2"), 0);
		pControlNew->AddTabItem(_T("Item3"), 0);
		pControlNew->AddTabItem(_T("Item4"), 0);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
MercuryBaseView::OnCommandTabViewAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildTabView*	pControlNew = new ESChildTabView();
	if( AddNewControl(pControlNew, pLastSelected) ){
		pControlNew->AddTabItem(_T("Item1"), 0);
		pControlNew->AddTabItem(_T("Item2"), 0);
		pControlNew->AddTabItem(_T("Item3"), 0);
		pControlNew->AddTabItem(_T("Item4"), 0);
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(false);
		}
	}

void
MercuryBaseView::OnCommandTreeViewAsChild(){
	}

void
MercuryBaseView::OnCommandBannerAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildBanner*	pControlNew = new ESChildBanner();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->Redraw(true);
		}
	}

void
MercuryBaseView::OnCommandPropertyGridAsChild(){
	if( m_arrSelectedControlIds.GetCount() <= 0 )
		return;

	ESChildControl* pLastSelected = GetSelectedControl(m_arrSelectedControlIds.GetCount()-1);
	if( !pLastSelected )
		return;

	ESChildControlPropertyGrid*	pControlNew = new ESChildControlPropertyGrid();
	if( AddNewControl(pControlNew, pLastSelected) ){
		AddInSelectedList(pControlNew);
		pControlNew->SetSerializableObject	(pControlNew, false);
		pControlNew->Redraw					(true);
		}
	}

void	
MercuryBaseView::OnCommandAlignLefts(){
	if( GetSelectedCt() < 2 ) return;
	ESChildControl* pLastSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(m_arrSelectedControlIds.GetCount() - 1).c_str());
	ASSERT(pLastSelected);
	if( !pLastSelected ) return;

	_Rect	rcLastSelected	= pLastSelected->GetClientRect();
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount() - 1;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			_Rect	rcControl = pControl->GetClientRect();
			_Size	szControl	(rcControl.Width(), rcControl.Height());
			pControl->SetPos(rcLastSelected.left, rcControl.top, true);
			}
		nLoop ++;
		}
	}

void	
MercuryBaseView::OnCommandAlignTops(){
	if( GetSelectedCt() < 2 ) return;
	ESChildControl* pLastSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(m_arrSelectedControlIds.GetCount() - 1).c_str());
	ASSERT(pLastSelected);
	if( !pLastSelected ) return;

	_Rect	rcLastSelected	= pLastSelected->GetClientRect();
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount() - 1;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			_Rect	rcControl = pControl->GetClientRect();
			_Size	szControl	(rcControl.Width(), rcControl.Height());
			pControl->SetPos(rcControl.left, rcLastSelected.top, true);
			}
		nLoop ++;
		}
	}

void	
MercuryBaseView::OnCommandAlignRights(){
	if( GetSelectedCt() < 2 ) return;
	ESChildControl* pLastSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(m_arrSelectedControlIds.GetCount() - 1).c_str());
	ASSERT(pLastSelected);
	if( !pLastSelected ) return;

	_Rect	rcLastSelected	= pLastSelected->GetClientRect();
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount() - 1;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			_Rect	rcControl = pControl->GetClientRect();
			_Size	szControl	(rcControl.Width(), rcControl.Height());
			pControl->SetPos((rcLastSelected.right - szControl.cx), rcControl.top, true);
			}
		nLoop ++;
		}
	}

void	
MercuryBaseView::OnCommandAlignBottoms(){
	if( GetSelectedCt() < 2 ) return;
	ESChildControl* pLastSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(m_arrSelectedControlIds.GetCount() - 1).c_str());
	ASSERT(pLastSelected);
	if( !pLastSelected ) return;

	_Rect	rcLastSelected	= pLastSelected->GetClientRect();
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount() - 1;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			_Rect	rcControl = pControl->GetClientRect();
			_Size	szControl	(rcControl.Width(), rcControl.Height());
			pControl->SetPos(rcControl.left, (rcLastSelected.bottom - szControl.cy), true);
			}
		nLoop ++;
		}
	}

void
MercuryBaseView::OnCommandMakeSameSizeWidth(){
	if( GetSelectedCt() < 2 ) return;
	ESChildControl* pLastSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(m_arrSelectedControlIds.GetCount() - 1).c_str());
	ASSERT(pLastSelected);
	if( !pLastSelected ) return;

	_Rect	rcLastSelected	= pLastSelected->GetClientRect();
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount() - 1;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			_Rect	rcControl	= pControl->GetClientRect();
			_Rect	rcBound		= rcControl;
			pControl->SetSize(rcLastSelected.Width(), rcControl.Height(), false);
			rcBound				|= pControl->GetClientRect();
			RedrawRect(rcBound, TRUE);
			}
		nLoop ++;
		}
	}

void
MercuryBaseView::OnCommandMakeSameSizeHeight(){
	if( GetSelectedCt() < 2 ) return;
	ESChildControl* pLastSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(m_arrSelectedControlIds.GetCount() - 1).c_str());
	ASSERT(pLastSelected);
	if( !pLastSelected ) return;

	_Rect	rcLastSelected	= pLastSelected->GetClientRect();
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount() - 1;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			_Rect	rcControl	= pControl->GetClientRect();
			_Rect	rcBound		= rcControl;
			pControl->SetSize(rcControl.Width(), rcLastSelected.Height(), false);
			rcBound				|= pControl->GetClientRect();
			RedrawRect(rcBound, TRUE);
			}
		nLoop ++;
		}
	}

void
MercuryBaseView::OnCommandMakeSameSizeBoth(){
	if( GetSelectedCt() < 2 ) return;
	ESChildControl* pLastSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(m_arrSelectedControlIds.GetCount() - 1).c_str());
	ASSERT(pLastSelected);
	if( !pLastSelected ) return;

	_Rect	rcLastSelected	= pLastSelected->GetClientRect();
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount() - 1;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			_Rect	rcControl	= pControl->GetClientRect();
			_Rect	rcBound		= rcControl;
			pControl->SetSize(rcLastSelected.Width(), rcLastSelected.Height(), false);
			rcBound				|= pControl->GetClientRect();
			RedrawRect(rcBound, TRUE);
			}
		nLoop ++;
		}
	}

void
MercuryBaseView::OnCommandLock(){
	LockSelectedControls(false);
	}

void
MercuryBaseView::OnCommandUnlock(){
	LockSelectedControls(true);
	}

void	
MercuryBaseView::OnCommandCopy(){
	CopySelectedControls(false);
	}

void	
MercuryBaseView::OnCommandCut(){
	CopySelectedControls(true);
	}

void	
MercuryBaseView::OnCommandPaste(){
	PasteControlsFromClipboard();
	}

void	
MercuryBaseView::OnCommandListOfControls(){
	if( m_listOfControlsDlg && m_listOfControlsDlg->IsWindow() ){
		m_listOfControlsDlg->ShowWindow		(SW_SHOW);
		m_listOfControlsDlg->SetActiveWindow();
		}
	else{
		if( !m_listOfControlsDlg )
			m_listOfControlsDlg = new ESSerializableModifyDlg();
            m_listOfControlsDlg->Create();
		//m_listOfControlsDlg->AddEventListener	(this, 1000/*Property Grid control id*/, EventCode_PropertyItemSelChange, (ControlEventListenerTemp)&MercuryBaseView::ListOfControls_PropertyItemSelChange);
		}

	EnumerableObject<ESChildControlAttribute>* pListOfControls = new EnumerableObject<ESChildControlAttribute>();
	for(int i=0; i<m_listChildControlInfos.GetCount(); i++){
		ESChildControlInfo* pInfo = m_listChildControlInfos.GetAt(i);
		if( pInfo ){
			ESChildControlAttribute* pAttrib = new ESChildControlAttribute();
			pAttrib->m_pControl		= pInfo->m_pControl;
			pAttrib->m_bVisible		= pInfo->m_pControl->IsVisible();
			pAttrib->m_nId			= pInfo->m_pControl->GetId();
			pAttrib->m_sClassName	= pInfo->m_pControl->GetClassName();
			pAttrib->m_sName		= pInfo->m_pControl->GetName();

			pAttrib->AddVariableModifyCallBack((void*)&pAttrib->m_bVisible, (VariableValueModifyCallback)MercuryBaseView::ControlAttribute_VisibleModify);
			pAttrib->AddVariableModifyCallBack((void*)&pAttrib->m_sName,	(VariableValueModifyCallback)MercuryBaseView::ControlAttribute_NameModify);
			pListOfControls->Add(pAttrib);
			}
		}

	m_listOfControlsDlg->SetListOfObjects((EnumerableObject<Serializable>*)pListOfControls);

	if( m_pListOfControls ){
		m_pListOfControls->RemoveAll(true);
		delete m_pListOfControls;
		}
	m_pListOfControls = pListOfControls;
	}

void
MercuryBaseView::OnCommandOpenDesignerFile(){
	/*
	_string sFile;
	CFileDialog openFile(TRUE, 0, 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Designer Files(*.des)|*.des||"), m_pBaseWnd); 
	if( openFile.DoModal() == IDOK ){
		sFile = openFile.GetPathName();
		if( LoadFromDesignFile(sFile) ){
			ESFrameBase* pParent = m_pBaseWnd;
			if( pParent ){
				_string sTitle;
				pParent->GetWindowText(sTitle);

				int nIndex = sTitle.Find('[');
				if( nIndex != -1 )
					sTitle = sTitle.Left(nIndex);

				_string sTitleNew;
				sTitleNew.Format(_T("%s[%s]"), sTitle, openFile.GetFileName());
				pParent->SetWindowText(sTitleNew);
				}

			}
		}*/
	}

void
MercuryBaseView::OnCommandSaveDesignerFile(){
	/*
	_string sFile;
	CFileDialog openFile(FALSE, 0, 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Designer Files(*.des)|*.des||"), m_pBaseWnd); 
	if( openFile.DoModal() == IDOK ){
		sFile = openFile.GetPathName();
		SaveToDesignFile(sFile);
		}*/
	}

void	
MercuryBaseView::OnCommandProperties(){
	/*
	if( ::IsWindow(m_propertyDlg) ){
		m_propertyDlg.ShowWindow(SW_SHOW);
		m_propertyDlg.SetActiveWindow();
		}
	else
		m_propertyDlg.Create(m_pBaseWnd, DS_SETFONT|DS_FIXEDSYS|WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME);

	if( GetSelectedCt() > 0 ){
		ESChildControl* pSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(0).c_str());
		if( pSelected )
			m_propertyDlg.SetSerializableObject(pSelected);
		}
	else
		m_propertyDlg.SetSerializableObject(&m_info);
		*/
    
#ifdef __APPLE__
    if(m_propertyDlg == nullptr){
        m_propertyDlg = new ESSerializableModifyDlg();
        m_propertyDlg->Create();
        
    }
    else{
        m_propertyDlg = new ESSerializableModifyDlg();
        m_propertyDlg->Create();
    }

    if( GetSelectedCt() > 0 ){
        ESChildControl* pSelected = GetChildControlByName(m_arrSelectedControlIds.GetKey(0).c_str());
        if( pSelected )
            m_propertyDlg->SetSerializableObject(pSelected);
    }
    else
        m_propertyDlg->SetSerializableObject(&m_info);
    //m_propertyDlg->Invalidate();
    /*
    NSRect contentRect = CGRectMake(0, 0, 200, 200);
    NSUInteger styleMask = NSWindowStyleMaskTitled|NSWindowStyleMaskClosable;
    
    m_pPropertyWindowOS = [[NSWindowFrame alloc] initWithContentRect:contentRect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
    [m_pPropertyWindowOS setReleasedWhenClosed:NO];
    
    NSWindowFrameView* _modalView = [[NSWindowFrameView alloc] init];
    [_modalView setMercuryViewToDisplay:(MercuryBaseView*)m_propertyDlg];

    [m_pPropertyWindowOS setContentView:_modalView];
    [m_pPropertyWindowOS makeKeyAndOrderFront:m_pPropertyWindowOS];*/
#endif
	}

void	
MercuryBaseView::OnCommandBaseViewProperties(){
	/*
	if( ::IsWindow(m_propertyDlg) ){
		m_propertyDlg.ShowWindow(SW_SHOW);
		m_propertyDlg.SetActiveWindow();
		}
	else
		m_propertyDlg.Create(m_pBaseWnd, DS_SETFONT|DS_FIXEDSYS|WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME);

	m_propertyDlg.SetSerializableObject(&m_info);
	*/
	}

int
MercuryBaseView::GetSelectedCt(){
	return m_arrSelectedControlIds.GetCount();
	}

bool
MercuryBaseView::IsControlSelected(ESChildControl* pControl){
	if( !pControl ) return false;
	int nId = m_arrSelectedControlIds.GetValue(pControl->GetName());
	return (nId > 0);
	}

bool
MercuryBaseView::ClearSelections(bool bRedraw /*= true*/){
	CPtrArray	arrControls;
	int			nLoop = 0, nCt = m_arrSelectedControlIds.GetCount();

	_Rect rcClient;
	GetClientRectMy(rcClient);

	if( bRedraw ){
		while( nLoop < nCt ){
			ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
			if( pControl ){
				// Calculate logical position. {{
				if( m_arrSelectedControlIds.GetValueByIndex(nLoop)&0x80000000 ){
					_Point ptPos;
					pControl->GetPosInPixels(ptPos);
					pControl->SetSizeAutoCalculation(true);
					pControl->SetLogicalPos			((ptPos.x / ((float)rcClient.Width())), (ptPos.y / ((float)rcClient.Height())));
					}
				// }}

				arrControls.push_back((void*)pControl);
				}
			nLoop ++;
			}

		m_arrSelectedControlIds.DeleteAll();

		for(int i=0; i<arrControls.size(); i++){
			ESChildControl* pControl = (ESChildControl*)arrControls[i];
			if( pControl )
				pControl->Redraw(true);
			}
		}
	else{
		while( nLoop < nCt ){
			ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
			if( pControl ){
				// Calculate logical position. {{
				if( m_arrSelectedControlIds.GetValueByIndex(nLoop)&0x80000000 ){
					_Point ptPos;
					pControl->GetPosInPixels(ptPos);
					pControl->SetSizeAutoCalculation(true);
					pControl->SetLogicalPos			((ptPos.x / ((float)rcClient.Width())), (ptPos.y / ((float)rcClient.Height())));
					}
				// }}
				}
			nLoop ++;
			}

		m_arrSelectedControlIds.DeleteAll();
		}
	m_rcSelectionBound.SetRect(0, 0, 0, 0);
	return true;
	}

bool
MercuryBaseView::AddInSelectedList(ESChildControl* pControl){
	if( !pControl ) return false;
	if( !m_arrSelectedControlIds.KeyExists(pControl->GetName(), NULL) ){
		if( !m_arrSelectedControlIds.GetCount() )
			m_rcSelectionBound = pControl->GetClientRect();
		else
			m_rcSelectionBound |= pControl->GetClientRect();
		
		if( pControl->HasLogicalPos() ){
			pControl->SetSizeAutoCalculation(false);

			int nValue = (0x80000000 | pControl->GetId());
			m_arrSelectedControlIds.Add(pControl->GetName(), nValue);
			}
		else
			m_arrSelectedControlIds.Add(pControl->GetName(), pControl->GetId());
		}
	return true;
	}

bool
MercuryBaseView::RemoveFromSelectedList	(ESChildControl* pControl){
	if( !pControl )		return false;
	int		nValue		= m_arrSelectedControlIds.GetValue(pControl->GetName());
	BOOL	bAutoCalc	= (nValue&0x80000000) ? 1 : 0;

	if( nValue > 0 ){
		// Calculate logical position. {{
		if( bAutoCalc ){
			_Rect rcClient;
			GetClientRectMy(rcClient);

			_Point ptPos;
			pControl->GetPosInPixels(ptPos);
			pControl->SetSizeAutoCalculation(true);
			pControl->SetLogicalPos			((ptPos.x / ((float)rcClient.Width())), (ptPos.y / ((float)rcClient.Height())));
			}
		// }}

		m_arrSelectedControlIds.Delete(pControl->GetName());

		int		nLoop = 0, nCt	= m_arrSelectedControlIds.GetCount();
		_Rect	rcBound(0, 0, 0, 0);
		while( nLoop < nCt ){
			ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
			if( pControl )
				rcBound |= pControl->GetClientRect();
			nLoop ++;
			}
		m_rcSelectionBound = rcBound;
		return true;
		}
	return false;
	}

int
MercuryBaseView::RemoveSelectedControls(){
	int nSelectedCt = m_arrSelectedControlIds.GetCount();
	if( nSelectedCt <= 0 ) return 0;

	_Rect rcRedraw(0, 0, 0, 0);
	int nLoop		= 0;
	while( nLoop < nSelectedCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			if( pControl->IsVisible() )
				rcRedraw |= pControl->GetClientRect();
			RemoveChildControl(pControl);
			}
		nLoop ++;
		}

	m_arrSelectedControlIds.DeleteAll();
	m_rcSelectionBound.SetRect(0, 0, 0, 0);
	RedrawRect(rcRedraw, 1);
	return nSelectedCt;
	}

bool
MercuryBaseView::MoveSelectedControls(int nOffsetX, int nOffsetY){
	if( m_rcSelectionBound.IsRectEmpty() || ((nOffsetX + nOffsetY) == 0) )
		return false; // Selection rect is invalid.

	_Rect rcClient;
	GetClientRectMy(rcClient);

	int		nLoop = 0, nCt = m_arrSelectedControlIds.GetCount();
	_Rect	rcBound(0, 0, 0, 0), rRectOld;
	_Point	ptPos;

	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl ){
			if( pControl->m_pChild )
				pControl->MoveChildControls(nOffsetX, nOffsetY);

			if( !pControl->IsLocked() ){
				rRectOld	= pControl->GetClientRect();
				pControl->GetPosInPixels(ptPos);
				ptPos.x		+= nOffsetX;
				ptPos.y		+= nOffsetY;

				if( pControl->HasLogicalPos() )
					pControl->SetLogicalPos((ptPos.x / ((float)rcClient.Width())), (ptPos.y / ((float)rcClient.Height())));					
				else
					pControl->SetPos(ptPos.x, ptPos.y, false);
				pControl->OwnerWindowSizeChanged(rcClient.Width(), rcClient.Height());
				rcBound |= (pControl->GetClientRect() | rRectOld);
				}
			}
		nLoop ++;
		}

	RedrawRect(rcBound, 1);
	m_rcSelectionBound = rcBound;
	return true;
	}

int	
MercuryBaseView::CopySelectedControls(bool bCut /*= false*/){
    /*
	if( !GetSelectedCt() )
		return 0;

	int		nLoop		= 0;
	_string	sParent;
	_Rect	rcBound(0, 0, 0, 0);
	if( OpenClipboard(GetHWND()) ){
		int nCt		= m_arrSelectedControlIds.GetCount();
		GrowableMemory mem;
		while( nLoop < nCt ){
			ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
			if( pControl ){
				if( pControl->IsVisible() )
					rcBound |= pControl->GetClientRect();

				// Add class name.
				mem.AddString(pControl->GetClassName());
				// Add parent control name.
				mem.AddString(sParent);
				// Serialize class object.
				pControl->Serialize(&mem);
				}
			nLoop ++;
			}

		UINT	nFormatId	= ::RegisterClipboardFormat(_T("ES_BINARY"));
		HGLOBAL hglbCopy	= ::GetClipboardData(nFormatId);
		if( hglbCopy ){
			EmptyClipboard();
			}
		
		hglbCopy = GlobalAlloc(GMEM_MOVEABLE, mem.GetUsedBufferSize()); 
		if( hglbCopy == NULL ) { 
			CloseClipboard(); 
			return 0; 
			}

		void* lptstrCopy = GlobalLock(hglbCopy); 
		memcpy(lptstrCopy, mem.GetBufferPtr(), mem.GetUsedBufferSize());
		GlobalUnlock(hglbCopy); 
		
		if( !SetClipboardData(nFormatId, hglbCopy) )
			GlobalFree(hglbCopy);
		CloseClipboard();
		}

	if( bCut )
		RemoveSelectedControls();
	return nLoop;*/
    return 0;
	}

int
MercuryBaseView::PasteControlsFromClipboard(){
    /*
	int nControls = 0;
	if( OpenClipboard(GetHWND()) ){
		UINT nFormatId = ::RegisterClipboardFormat(_T("ES_BINARY"));
		HGLOBAL hglbCopy = ::GetClipboardData(nFormatId);
		if( !hglbCopy ) return 0;

		void*	lptstrCopy	= GlobalLock(hglbCopy);
		int		nSize		= (int)::GlobalSize(hglbCopy);

		GrowableMemory mem;
		mem.AddBytes((BYTE*)lptstrCopy, nSize);
		GlobalUnlock(hglbCopy);

		ClearSelections(true);

		_string sClassName, sParentName;
		while( TRUE ){
			if( !mem.ReadString(&sClassName) )
				break;
			if( !mem.ReadString(&sParentName) )
				break;

			ESChildControl* pControl = ESChildControl::GetChildClassInstance(sClassName);
			if( pControl ){
				pControl->Deserialize(&mem);

				_Rect			rcControl			= pControl->GetClientRect();
				ESChildControl* pParent				= NULL;
				ESChildControl* pOriginalControl	= GetChildControl(pControl->GetId());
				if( pOriginalControl ){
					rcControl.left		+= 5;
					rcControl.top		+= 5;
					rcControl.right		+= 5;
					rcControl.bottom	+= 5;
					pParent							= pOriginalControl->GetParent();
					}
				
				//if( !pParent )
				//	pParent = GetChildControlByName(sParentName.c_str());

				if( !AddChildControl(pControl, GetChildControlIdUnused(this), rcControl, pParent, pControl->GetZIndex()) ){
					delete pControl;
					}
				else{
					nControls ++;
					AddInSelectedList(pControl);
					pControl->Redraw();
					}
				}
			}
		CloseClipboard();
		}
	return nControls;*/
    return 0;
	}

bool
MercuryBaseView::LockSelectedControls(bool bUnlock /*= false*/){
	int		nLoop			= 0;
	int		nCt				= m_arrSelectedControlIds.GetCount();
	while( nLoop < nCt ){
		ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).c_str());
		if( pControl )
			pControl->SetLock(!bUnlock);
		nLoop ++;
		}
	return true;
	}

ESChildControl*
MercuryBaseView::GetSelectedControl(int nIndex){
	if( nIndex < 0 || nIndex >= m_arrSelectedControlIds.GetCount() )
		return NULL; 
	int nControlId = (m_arrSelectedControlIds.GetValueByIndex(nIndex)&0x7FFFFFFF);
	return GetChildControl(nControlId);
	}

bool
MercuryBaseView::AddNewControl(ESChildControl* pControlNew, ESChildControl* pParent /*= NULL*/){
    _Point pt = _Cursor::GetCurrentPos(m_hWnd);
    _Size	szDefault	= ((ESChildControl*)pControlNew)->GetControlDefaultSize();

	_Rect rcControl;
	rcControl.left		= pt.x;
	rcControl.top		= pt.y;
	rcControl.right		= rcControl.left + szDefault.cx;
	rcControl.bottom	= rcControl.top + szDefault.cy;

	_Rect rcBound;
	GetClientRectMy(rcBound);
	if( pParent )
		rcBound = pParent->GetClientRect();

	if( rcControl.Width() > rcBound.Width() )
		rcControl.right = rcControl.left + rcBound.Width() - 5;
	if( rcControl.Height() > rcBound.Height() )
		rcControl.bottom = rcControl.top + rcBound.Height() - 5;

	_Rect rcIntersect	= rcBound&rcControl;
	if( rcIntersect.Width() < rcControl.Width() ){
		int nWidth		= rcControl.Width();
		rcControl.left	= rcBound.right - nWidth;
		rcControl.right	= rcControl.left + nWidth;
		}

	if( rcIntersect.Height() < rcControl.Height() ){
		int nHeight		= rcControl.Height();
		rcControl.top	= rcBound.bottom - nHeight;
		rcControl.bottom= rcControl.top + nHeight;
		}

	if( AddChildControl(pControlNew, MercuryBaseView::GetChildControlIdUnused(this), rcControl, pParent) ){
		}
	else{
		delete pControlNew;
		return false;
		}
	return true;
	}

bool
MercuryBaseView::SaveToDesignFile(_string sDesignFile){
	Path::BuildFullPath(sDesignFile);
	AutoSortedArray arrAddedControls;
	arrAddedControls.SetCompareFunction(&AutoSortedArray::CompareEmpty);

	// Delete existed file. {{
	//if( !_access(sDesignFile.c_str(), 0) ){
	//	::DeleteFileA(sDesignFile.c_str());
	//	}
	// }}

	int nSignature = DESIGN_FILE_SIGNATURE;
	m_designInfo.m_dtCreationDateTime = DateTime(time(NULL));
	if( !m_designInfo.m_sAuthor.length() )
		m_designInfo.m_sAuthor = _T("ZqrTalent");

	if( !m_designInfo.m_nVersionMajor )
		m_designInfo.m_nVersionMajor = 1;

	if( !m_designInfo.m_nVersionMinor )
		m_designInfo.m_nVersionMinor = 1;

	GrowableMemory mem;
	// Add signature
	mem.AddBytes((BYTE*)&nSignature, sizeof(int));
	// Add design info.
	m_designInfo.SerializeWithVariableName(&mem);
	m_info.SerializeWithVariableName(&mem);

	bool bRet = SerializeControls(&mem);

	// Save into file. {{
	Path::SaveAsFile(mem.GetBufferPtr(), mem.GetUsedBufferSize(), sDesignFile);
	// }}

	// Call serialization/deserialisation complete method. {{
	int	nLoop = 0, nCt = m_listChildControls.GetCount();
	while( nLoop < nCt ){
		ESChildControl* pControl = m_listChildControls[nLoop];
		if( pControl && pControl->IsCreated() )
			pControl->OnSerializationCompleted();
		nLoop ++;
		}

	if( IsWindow() ){
		// Restore window size. 
		ESFrameBase* pWindow	= this;
		if( pWindow ){
            /*
			DWORD dwStyleOld	= ::GetWindowLongPtr(*pWindow, GWL_STYLE);
			DWORD dwStyleRemove = 0L;
			DWORD dwStyleAdd	= 0L;

			if( !m_info.m_bTitleBar )
				dwStyleRemove |= (WS_CAPTION|WS_THICKFRAME); 
			else
				dwStyleAdd |= (WS_CAPTION|WS_THICKFRAME);

			if( !m_info.m_bMinimizeBox )
				dwStyleRemove |= (WS_MINIMIZEBOX); 
			else
				dwStyleAdd |= (WS_MINIMIZEBOX);

			if( !m_info.m_bMaximizeBox )
				dwStyleRemove |= (WS_MAXIMIZEBOX); 
			else
				dwStyleAdd |= WS_MAXIMIZEBOX;

			pWindow->ModifyStyle(dwStyleRemove, dwStyleAdd);
			pWindow->m_szDefault	= m_info.m_szDefault;
			pWindow->m_szMin		= m_info.m_szMin;
			pWindow->m_szMax		= m_info.m_szMax;*/
			}

        /*
		if( m_info.m_bTitleBar ){
			_Rect rcWindow;
			pWindow->GetWindowRect(rcWindow);
			_Rect rcClient;
			pWindow->GetClientRect(rcClient);
			_Size szAdd((rcWindow.Width() - rcClient.Width()), (rcWindow.Height() - rcClient.Height()));

			pWindow->SetWindowPos(NULL, 0, 0, m_info.m_szDefault.cx + szAdd.cx, m_info.m_szDefault.cy + szAdd.cy, SWP_NOMOVE|SWP_NOREDRAW);
			}
		else
			pWindow->SetWindowPos(NULL, 0, 0, m_info.m_szDefault.cx, m_info.m_szDefault.cy, SWP_NOMOVE|SWP_NOREDRAW);
*/
		_Rect rcClient;
		pWindow->GetClientRect	(rcClient);
		ResizeChildControls		(rcClient.Width(), rcClient.Height());
		Invalidate				();
		}
	return true;
	}

bool
MercuryBaseView::LoadFromDesignFile(_string sDesignFile, bool bAddControls /*= false*/, _string sParentControlName /*= _T("")*/, 
									bool bTranslateControlsRect /*= false*/,  _Size szDefaultWindowSize /*= _Size(0, 0)*/, int nPosX /*= -1*/, int nPosY /*= -1*/, int nControlIdIncrement /*= 0*/){
	Path::BuildFullPath(sDesignFile);
	AutoSortedArray arrAddedControls;
	arrAddedControls.SetCompareFunction(&AutoSortedArray::CompareEmpty);
	
	//if( _access(sDesignFile.c_str(), 0) )
	//	return false; // File not exists.

	/*
	CFile file(sDesignFile.c_str(), CFile::modeRead|CFile::shareDenyWrite);
	DWORD dwFileSize = file.GetLength();
	if( dwFileSize > 0x6400000  ) // 100MB
		return false; // File size is more than allowed to be.

	GrowableMemory mem;
	mem.SetBufferSize		((int)dwFileSize);
	mem.SetUsedBufferSize	((int)dwFileSize);

	DWORD dwRead = dwFileSize;
	if( file.Read(mem.GetBufferPtr(), dwRead) != dwRead )
		return false; // Read file error.
	*/

    //GrowableMemory mem;
	BYTE*	lpFileData	= NULL;
	int		nDataSize	= 0;
	if( !Path::ReadFileContent(sDesignFile, lpFileData, nDataSize) )
		return false; // File reading error !!!
    
	GrowableMemory mem;
	mem.SetReadonlyBuffer(lpFileData, nDataSize);

    /*
	std::unique_ptr<BYTE[]> ptrFileData;
	ptrFileData.reset(lpFileData);
*/
    
	// Deserialize info.
	BOOL	bRet		= FALSE;
	int		nSignature	= 0;
	if( mem.ReadInt32(&nSignature) ){
		bool				bVariableNames = false;
		ESDesignFileInfo	designInfo;
		MercuryBaseViewInfo info;

		if( nSignature == DESIGN_FILE_SIGNATURE ){
			if( !designInfo.DeserializeWithVariableName(&mem) )
				return false;
			bRet = info.DeserializeWithVariableName_Rec(&mem);
			bVariableNames = true;
			}
		else{ // Load old version design file.
			mem.ResetBufferPointerPosition();
			bRet = info.Deserialize(&mem);
			}

		if( !bAddControls ){
			// Copy objects.
			designInfo.Copy(&m_designInfo);
			info.Copy(&m_info);
            
			if( (szDefaultWindowSize.cx >= m_info.m_szMin.cx && szDefaultWindowSize.cx <= m_info.m_szMax.cx) &&
				(szDefaultWindowSize.cy >= m_info.m_szMin.cy && szDefaultWindowSize.cy <= m_info.m_szMax.cy) ){
				if( m_bAllowZooming ){
					m_fRatioCX = ((float)szDefaultWindowSize.cx) / m_info.m_szDefault.cx;
					m_fRatioCY = ((float)szDefaultWindowSize.cy) / m_info.m_szDefault.cy;
					}
				m_info.m_szDefault = szDefaultWindowSize;
				}

			// Clear child controls.
			ClearChildControls(false);
			// Deserialize controls from binary.
			DeserializeControls(&mem, bVariableNames, 0, 0);
			}
		else{
			ESChildControl* pParentControl = !sParentControlName.length() ? NULL : GetChildControlByName(sParentControlName.c_str());
			// Deserialize controls from binary.
			DeserializeControls(&mem, bVariableNames, pParentControl, &arrAddedControls, nControlIdIncrement);
			}
		}
	else{
        delete [] lpFileData;
		return false;
    }
	// Call deserialisation complete method. {{
	if( bAddControls ){
		ESChildControl* pParentControl = !sParentControlName.length() ? NULL : GetChildControlByName(sParentControlName.c_str());
		if( bTranslateControlsRect && pParentControl ){
			_Rect rcParent = pParentControl->GetClientRect();
			for(int i=0; i<arrAddedControls.GetCount(); i++){
				ESChildControl* pControl = (ESChildControl*)arrAddedControls.Get(i);
				if( pControl && pControl->IsCreated() ){
					_Rect rcClient = pControl->GetClientRect();
					int nCX = rcClient.Width(), nCY = rcClient.Height();
					rcClient.left		+= rcParent.left;
					rcClient.top		+= rcParent.top;
					rcClient.right		= rcClient.left + nCX;
					rcClient.bottom		= rcClient.top + nCY;
					pControl->m_rRect	= rcClient;

					pControl->OnDeserializationCompleted();
					}
				}
			}
		else{
			for(int i=0; i<arrAddedControls.GetCount(); i++){
				ESChildControl* pControl = (ESChildControl*)arrAddedControls.Get(i);
				if( pControl && pControl->IsCreated() )
					pControl->OnDeserializationCompleted();
				}
			}
		}
	else{
		int	nLoop = 0, nCt = m_listChildControls.GetCount();
		while( nLoop < nCt ){
			ESChildControl* pControl = m_listChildControls[nLoop];
			if( pControl && pControl->IsCreated() )
				pControl->OnDeserializationCompleted();
			nLoop ++;
			}
		}
	// }}
    
    delete [] lpFileData;
    
    _Rect rcClient;
    GetClientRect      (rcClient);
    ResizeChildControls(rcClient.Width(), rcClient.Height());

    /*
	if( IsWindow() ){
		ESFrameBase* pParentFrame = this;
		// Validate min, default and max rect sizes. {{
		HWND hWndDesk = ::GetDesktopWindow();
		ASSERT(hWndDesk);
		_Rect rcDesk;
		::GetWindowRect(hWndDesk, rcDesk);
		_Size szDesk(rcDesk.Width(), rcDesk.Height());

		if( m_info.m_szMax.cy == -1 )
			m_info.m_szMax.cy = szDesk.cy;
		if( m_info.m_szMax.cx == -1 )
			m_info.m_szMax.cx = szDesk.cx;
			
		if( pParentFrame->m_fHeightAndWidthRatio > 0.0f ){
			if( m_info.m_szMax.cy > szDesk.cy ){
				m_info.m_szMax.cy = szDesk.cy;
				m_info.m_szMax.cx = (((float)m_info.m_szMax.cy) / pParentFrame->m_fHeightAndWidthRatio);
				}
			}
		else{
			if( m_info.m_szDefault.cx > szDesk.cx )
				m_info.m_szDefault.cx = szDesk.cx;
			if( m_info.m_szDefault.cy > szDesk.cy )
				m_info.m_szDefault.cy = szDesk.cy;

			if( m_info.m_szMin.cx > szDesk.cx )
				m_info.m_szMin.cx = szDesk.cx;
			if( m_info.m_szMin.cy > szDesk.cy )
				m_info.m_szMin.cy = szDesk.cy;

			if( m_info.m_szMax.cx > szDesk.cx )
				m_info.m_szMax.cx = szDesk.cx;
			if( m_info.m_szMax.cy > szDesk.cy )
				m_info.m_szMax.cy = szDesk.cy;
			}
		// }}

		// Restore window size. 
		ESFrameBase* pWindow	= this;
		if( pWindow ){
			DWORD dwStyleOld	= ::GetWindowLongPtr(*pWindow, GWL_STYLE);
			DWORD dwStyleRemove = 0L;
			DWORD dwStyleAdd	= 0L;

			if( !m_info.m_bTitleBar ){
				dwStyleRemove |= (WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX); 
				}
			else{
				dwStyleAdd |= (WS_CAPTION|WS_THICKFRAME);

				if( !m_info.m_bMinimizeBox )
					dwStyleRemove |= (WS_MINIMIZEBOX); 
				else
					dwStyleAdd |= (WS_MINIMIZEBOX);

				if( !m_info.m_bMaximizeBox )
					dwStyleRemove |= (WS_MAXIMIZEBOX); 
				else
					dwStyleAdd |= WS_MAXIMIZEBOX;
				}

			pWindow->ModifyStyle(dwStyleRemove, dwStyleAdd);
			pWindow->m_szDefault	= m_info.m_szDefault;
			pWindow->m_szMin		= m_info.m_szMin;
			pWindow->m_szMax		= m_info.m_szMax;
			}
		
		if( !bAddControls ){
//			if( !szDefaultWindowSize.cx || !szDefaultWindowSize.cy )
//				szDefaultWindowSize = m_info.m_szDefault;
			if( m_info.m_bTitleBar ){
				int nCaption	= pWindow->GetCaptionHeight();
				int nBorder		= pWindow->GetBorderWidth();

				if( nPosX >= 0 && nPosY >= 0 )
					pWindow->SetWindowPos(NULL, nPosX, nPosY, m_info.m_szDefault.cx + 2*nBorder, m_info.m_szDefault.cy + nCaption + nBorder, SWP_NOREDRAW);
				else
					pWindow->SetWindowPos(NULL, 0, 0, m_info.m_szDefault.cx + 2*nBorder, m_info.m_szDefault.cy + nCaption + nBorder, SWP_NOMOVE|SWP_NOREDRAW);
				}
			else{
				if( nPosX >= 0 && nPosY >= 0 )
					pWindow->SetWindowPos(NULL, nPosX, nPosY, m_info.m_szDefault.cx, m_info.m_szDefault.cy, SWP_NOREDRAW);
				else
					pWindow->SetWindowPos(NULL, 0, 0, m_info.m_szDefault.cx, m_info.m_szDefault.cy, SWP_NOMOVE|SWP_NOREDRAW);
				}
			}

		_Rect rcClient;
		pWindow->GetClientRect	(rcClient);
		ResizeChildControls		(rcClient.Width(), rcClient.Height());
		Invalidate				();
		}*/
	return true;
	}

bool
MercuryBaseView::LoadControlFromDesign(GrowableMemory* pMem, const _string sClass, const _string sName, ESChildControl*& pControl){
	if( !pMem ) return false;

	// Deserialize info.
	int		nSignature	= 0;
	if( pMem->ReadInt32(&nSignature) ){
		bool				bVariableNames = false;
		ESDesignFileInfo	designInfo;
		MercuryBaseViewInfo info;

		if( nSignature == DESIGN_FILE_SIGNATURE ){
			if( !designInfo.DeserializeWithVariableName(pMem) )
				return FALSE;
			info.DeserializeWithVariableName_Rec(pMem);
			bVariableNames = true;
			}
		else{ // Load old version design file.
			pMem->ResetBufferPointerPosition();
			info.Deserialize(pMem);
			}
		}
	else
		return false;

	_string sClassName;
	int		nParentId	= 0;

	while( pMem->ReadString(&sClassName) && pMem->ReadInt32(&nParentId) ){
		ESChildControl* pControl1 = (ESChildControl*)ESChildControl::GetChildClassInstance(sClassName);
		if( !pControl1 ) break;
		if( !pControl1->DeserializeWithVariableName(pMem) ){
			delete pControl1;
			break;
			}

		//if( !sClassName.CompareNoCase(sClass) && !pControl1->GetName().CompareNoCase(sName) ){
		if( !sClassName.compare(sClass) && !pControl1->GetName().compare(sName) ){
			pControl = pControl1;
			return true;
			}
		delete pControl1;
		}

	return false;
	}

bool
MercuryBaseView::SerializeControls(GrowableMemory* pMem, ESChildControl* pParent /*= NULL*/){
	ASSERT(pMem);
	ESChildControl* pControl = (pParent ? pParent->m_pChild : m_pFirstChild);
	_string			sClassName;
	int				nParentId = 0;

	while( pControl ){
		if( pControl->AllowedToSerialize() ){
			if( pControl->m_pParent )
				nParentId = pControl->m_pParent->GetId();
			else
				nParentId = 0;

			// Add class name.
			pMem->AddString(pControl->m_sClassName);
			// Add parent control id.
			pMem->AddBytes((BYTE*)&nParentId, sizeof(int));
	
			if( !pControl->SerializeWithVariableName(pMem) )
				return false;

			if( pControl->GetChild() ){
				if( !SerializeControls(pMem, pControl) )
					return false;
				}
			}
		pControl = pControl->m_pNext;
		}
	return true;
	}

bool
MercuryBaseView::DeserializeControls(GrowableMemory* pMem, bool bVariableNames /*= false*/, ESChildControl* pParentControl /*= NULL*/, AutoSortedArray* pDeserializedControls /*= NULL*/, int nControlIdIncrement /*= 0*/){
	ASSERT(pMem);
	_string sClassName;
	int		nParentId	= 0;

	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	GetZoom(fZoomCX, fZoomCY);
    
	_Rect rcControl, rcFixed;
	_Size szControl;

	if( bVariableNames ){
		while( pMem->ReadString(&sClassName) && pMem->ReadInt32(&nParentId) ){
			ESChildControl* pControl = (ESChildControl*)ESChildControl::GetChildClassInstance(sClassName);
			if( pControl ){
				if( !pControl->DeserializeWithVariableName(pMem) ){
					delete pControl;
					return false;
					}

				ESChildControl* pParent = pParentControl;
				if( nParentId > 0 ){
					pParent = GetChildControl(nParentId + nControlIdIncrement);
					}

				bool	bVisible	= pControl->IsVisible();
				int		nId			= pControl->GetId() + nControlIdIncrement;
				ESChildControlInfo* pInfo = m_listChildControlInfos.FindOneVariable(_T("Id"), &nId);
				if( pInfo )
					nId = MercuryBaseView::GetChildControlIdUnused(this);

				rcControl = pControl->GetClientRect();
				if( fZoomCX != 1.0f || fZoomCY != 1.0f ){
					szControl.cx		= rcControl.Width();
					szControl.cy		= rcControl.Height();
					rcFixed				= pControl->m_rcFixedPosition;
					if( (rcFixed.left + rcFixed.right + rcFixed.top + rcFixed.bottom) == -4 ){
						rcControl.left	*= fZoomCX;
						rcControl.top	*= fZoomCY;
						rcControl.right	 = rcControl.left + szControl.cx*fZoomCX;
						rcControl.bottom = rcControl.top + szControl.cy*fZoomCY;
						}
					else{
						pControl->CalcClientRectangle(m_info.m_szDefault.cx, m_info.m_szDefault.cy, rcControl, pParent);
						if( rcFixed.left == -1 && rcFixed.right == -1 ){
							rcControl.left	*= fZoomCX;
							rcControl.right	 = rcControl.left + szControl.cx*fZoomCX;
							}

						if( rcFixed.top == -1 && rcFixed.bottom == -1 ){
							rcControl.top	*= fZoomCY;
							rcControl.bottom = rcControl.top + szControl.cy*fZoomCY;
							}
						}
					}

				if( AddChildControl(pControl, nId, rcControl, pParent, pControl->GetZIndex(), pControl->GetName()) ){
					if( !bVisible )
						pControl->SetVisible(false, false);
					if( pDeserializedControls )
						pDeserializedControls->Add((void*)pControl);
					}
				else
					delete pControl;
				}
			else{
#ifdef DEBUG
                /*
				_string sMsg;
				sMsg.Format(_T("Couldn't create control by class '%s'"), sClassName);
				MessageBox(NULL, sMsg, "", MB_OK);
                */
				return false;
#endif
				}
			}
		}
	else{
		while( pMem->ReadString(&sClassName) && pMem->ReadInt32(&nParentId) ){
			ESChildControl* pControl = (ESChildControl*)ESChildControl::GetChildClassInstance(sClassName);
			if( pControl ){
				if( !pControl->Deserialize(pMem) ){
					delete pControl;
					return false;
					}

				ESChildControl* pParent = pParentControl;
				if( nParentId > 0 )
					pParent = GetChildControl(nParentId);

				bool bVisible = pControl->IsVisible();
				if( AddChildControl(pControl, pControl->GetId(), pControl->GetClientRect(), pParent, pControl->GetZIndex(), pControl->GetName()) ){
					if( !bVisible )
						pControl->SetVisible(false, false);

					if( pDeserializedControls )
						pDeserializedControls->Add((void*)pControl);
					}
				else
					delete pControl;
				}
			}
		}
	return true;
	}

bool
MercuryBaseView::ControlAttribute_VisibleModify(ESChildControlAttribute* pObject, VariableInfo* pInfo){
	if( pObject && pObject->m_pControl ){
		pObject->m_pControl->SetVisible(pObject->m_bVisible, true);
		}
	return true;
	}

bool
MercuryBaseView::ControlAttribute_NameModify(ESChildControlAttribute* pObject, VariableInfo* pInfo){
	if( pObject && pObject->m_pControl ){
		bool bExists	= false;
		bool bSuccess	= pObject->m_pControl->m_pOwner->SetChildControlName(pObject->m_pControl, pObject->m_sName, bExists);
		return bSuccess;
		}
	return true;
	}

void
MercuryBaseView::ListOfControls_PropertyItemSelChange(ESChildControl* pSender, int eventCode, int nParam){
	ESPropertyGridItem* pItem = (ESPropertyGridItem*)nParam;
	if( pItem && pItem->pObject ){
		ESChildControlAttribute* pAttrib = (ESChildControlAttribute*)pItem->pObject;
		if( pAttrib && pAttrib->m_pControl ){
			// Select control. {{
			if( pAttrib->m_pControl->IsVisible() && !IsControlSelected(pAttrib->m_pControl) ){
				ClearSelections(true);
				AddInSelectedList(pAttrib->m_pControl);
				pAttrib->m_pControl->Redraw(true);
				}
			// }}
			}
		}
	}
