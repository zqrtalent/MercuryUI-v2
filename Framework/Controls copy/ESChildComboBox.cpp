// ESChildComboBox.cpp : implementation file
//

#include "ESChildComboBox.h"
#include "ESChildComboListBox.h"
#include "ESChildScrollBar.h"

//////////////////////////////////////////////////////
//
//	class ESComboBoxItemInfo
//
/////////////////////////////////////////////////////
ESComboBoxItemInfo::ESComboBoxItemInfo(){
	m_nId = 0;
	}

BEGIN_DECLARE_VAR(ESComboBoxItemInfo, Serializable)
	DECLARE_VAR(_T("ESComboBoxItemInfo"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Name"),					_T("NAME"), VariableType::VT_String, offsetof(ESComboBoxItemInfo,m_sName), false, false, false)
	DECLARE_VAR(_T("Id"),					_T("ID"), VariableType::VT_Int32, offsetof(ESComboBoxItemInfo,m_nId), false, false, false)
END_DECLARE_VAR()

ESComboBoxItemInfo::~ESComboBoxItemInfo(){
	}

/////////////////////////////////////////////////////////////////////////////
// ESChildComboBox

ESChildComboBox::ESChildComboBox() : ESChildTextBox(){
	m_sClassName		= _T("ESChildComboBox");
	m_pListBox			= NULL;
	m_nListBoxHeight	= 120;
	m_nSelectedItemIndex=-1;
	m_nComboType		= ComboType::ESChildComboBox_DropList;
	}

BEGIN_DECLARE_VAR(ESChildComboBox, ESChildTextBox)
	DECLARE_VAR(_T("ESChildComboBox"),			_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Items"),					_T(""), VariableType::VT_None, offsetof(ESChildComboBox,m_listItems), false, true, true)
	DECLARE_VAR(_T("Selected Item Index"),		_T(""), VariableType::VT_Int32, offsetof(ESChildComboBox,m_nSelectedItemIndex), false, false, false)
	DECLARE_VAR(_T("ListBox Height"),			_T(""), VariableType::VT_Int32, offsetof(ESChildComboBox,m_nListBoxHeight), false, false, false)
END_DECLARE_VAR()

ESChildComboBox::~ESChildComboBox(){
	}

bool
ESChildComboBox::Create(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	// Translate items into enumerable object. {{
	for(int i=0; i<m_listItems.GetCount(); i++){
		ESComboBoxItemInfo* pItem = m_listItems.GetAt(i);
		if( pItem )
			m_content.Add(pItem->m_sName, (void*)pItem->m_nId);
		}
	SetCurSel(m_nSelectedItemIndex, false);
	// }}

	bool bRet = ESChildTextBox::Create(nId, rRect, pOwner, pParent);
	if( bRet ){
		if( m_nComboType == (int)ComboType::ESChildComboBox_DropList )
			DisableTextBox(true);
		CalcButtonRect();
		m_bReadOnly	= true;
		}
	return bRet;
	}

void
ESChildComboBox::OwnerWindowSizeChanged(int cx, int cy){
	ESChildTextBox::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildComboBox::SetPos(int x, int y, bool bRedraw){
	ESChildTextBox::SetPos(x, y, bRedraw);
	CalcButtonRect();
	return true;
	}

bool
ESChildComboBox::SetSize(int cx, int cy, bool bRedraw){
	bool bRet = ESChildTextBox::SetSize(cx, cy, bRedraw);
	CalcButtonRect();
	return bRet;
	}

bool
ESChildComboBox::Destroy(){
	ShowListBox(true);
	if( ESChildTextBox::Destroy() ){
		}
	return true;
	}

bool
ESChildComboBox::SetFocus(){
	Redraw(false);
	return true;
	}

void
ESChildComboBox::KillFocus(){
	// Hide list box.
	ShowListBox(true);
	ESChildTextBox::KillFocus();
	}

bool
ESChildComboBox::SetCurSel(int nSelIndex, bool bRedraw){
	if( !m_content.GetCount() )
		return false;
	if( nSelIndex < 0 || nSelIndex >= m_content.GetCount() )
		return false;

	m_nSelectedItemIndex = nSelIndex;
	SetText(m_content.GetKey(nSelIndex), true, bRedraw);

	int nIndex = -1;
	if( m_pListBox )
		m_pListBox->SetSelectedItem(nSelIndex, bRedraw, true);
	return true;
	}

bool
ESChildComboBox::SetCurSelByParam(void* pParam, bool bRedraw){
	for(int i=0; i<m_content.GetCount(); i++){
		if( m_content.GetValueByIndex(i) == pParam ){
			return SetCurSel(i, bRedraw);
			}
		}
	return false;
	}

void
ESChildComboBox::CalcButtonRect(){
	MercuryGUI* pGUI		= MercuryGUI::GetInstance();
	int			nCX			= pGUI->comboBox.m_imgComboButton.GetWidth();
	int			nCY			= pGUI->comboBox.m_imgComboButton.GetHeight();
    m_rcBoxButton.SetRect(m_rRect.right - nCX - 2, m_rRect.top + (m_rRect.Height() - nCY)/2, _Size(nCX, nCY));
	}

bool
ESChildComboBox::AddItem(_string sText, void* lpParam /*= NULL*/, bool bRedraw /*= false*/){
	m_content.Add(sText, lpParam);
	if( m_pListBox )
		return m_pListBox->AddItem(sText, lpParam, bRedraw);
	return false;
	}

void
ESChildComboBox::ClearContent(bool bRedraw){
	m_content.DeleteAll();
	ShowListBox(true);
	if( bRedraw )
		Redraw(false);
	}

void*
ESChildComboBox::GetSelectedParam(){
	return m_content.GetValueByIndex(m_nSelectedItemIndex);
//	return m_content.GetValue(GetText());
	}

void
ESChildComboBox::SetDropListType(){
	m_nComboType = (int)ComboType::ESChildComboBox_DropList;
	DisableTextBox(true);
	}

void
ESChildComboBox::SetDropDownType(){
	m_nComboType = (int)ComboType::ESChildComboBox_DropDown;
	DisableTextBox(false);
	}

_string
ESChildComboBox::GetItemText(int nIndex /*= -1*/){
	if( nIndex == -1 )
		return GetText();
	if( !m_content.GetCount() )
		return _T("");
	if( nIndex < 0 || nIndex >= m_content.GetCount() )
		return _T("");
	return m_content.GetKey(nIndex);
	}

// ========================================================
//
//	Event methods.
//
// ========================================================

void
ESChildComboBox::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	MercuryGUI* pMerGUI		= MercuryGUI::GetInstance();
	_Rect		rcBoxDC((m_rcBoxButton.left - pRectClient->left) + pRectDC->left, (m_rcBoxButton.top - pRectClient->top) + pRectDC->top, m_rcBoxButton.Size()), rcInvalid;
	
	_Image*				pImageBox	= m_pListBox ? &pMerGUI->comboBox.m_imgComboButtonActive : &pMerGUI->comboBox.m_imgComboButton;
	ESLocalizationMan*	pMan		=  ESLocalizationMan::GetInstance();

	if( m_arrTextLines.size() == 1 ){
		_string sTextOld = m_arrTextLines.at(0), sTextAlt = m_sAlternateText;
		_string sText = sTextOld;

		pMan->ParseText(sText);
		m_arrTextLines[0] = sText;
		pMan->ParseText(m_sAlternateText);

		// First, draw entire textbox.
		ESChildTextBox::OnPaintClient(pDC, pRectDC, pRectClient);

		m_arrTextLines[0]	= sTextOld;
		m_sAlternateText	= sTextAlt;
		}
	else{
		_string  sTextAlt = m_sAlternateText;
		pMan->ParseText(m_sAlternateText);
		// First, draw entire textbox.
		ESChildTextBox::OnPaintClient(pDC, pRectDC, pRectClient);
		m_sAlternateText	= sTextAlt;
		}

	// Draw box button. {{
	rcInvalid		= rcBoxDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		pImageBox->RenderImage(pDC, rcInvalid, rcBoxDC, false);
	// }}
	}

BOOL	
ESChildComboBox::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
    _Point pt = _Cursor::GetCurrentPos(m_pOwner->GetHWND());
	if( m_rcBoxButton.PtInRect(pt) ){
        _Cursor::SetCursor(_Cursor::Hand);
		return 1;
		}
	return ESChildTextBox::OnSetCursor(hWnd, nHitTest, message);
	}

void
ESChildComboBox::OnLButtonDown(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonDown(pt, nFlags);
	}

void
ESChildComboBox::OnLButtonUp(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonUp(pt, nFlags);
	}

void	
ESChildComboBox::OnLButtonClick(_Point pt, UINT nFlags){
	ESChildTextBox::OnLButtonClick(pt, nFlags);

	if( m_rcBoxButton.PtInRect(pt) )
		ShowListBox(IsListBoxVisible());
	else{
		if( m_nComboType == (int)ComboType::ESChildComboBox_DropList )
			ShowListBox(IsListBoxVisible());
		}
	}

void
ESChildComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	switch( nChar ){
		case VK_RETURN:{
			if( m_pListBox ){
				// Copy selected text from listbox into combobox.{{
				if( m_pListBox->GetSelectedItem() > -1 ){
					m_nSelectedItemIndex = m_pListBox->GetSelectedItem();
					SendEvent(this, EventCode_ComboBoxSelChange, (void*)m_pListBox->GetItemParam(m_nSelectedItemIndex));
					SetText(m_pListBox->GetItemText(m_nSelectedItemIndex), true, true);
					}
				// }}
				ShowListBox	(true);
				return;
				}
			break;
			}
		case VK_ESCAPE:{
			if( m_pListBox ){
				ShowListBox(true);
				return;
				}
			break;
			}
		case VK_DOWN:{
			if( m_pListBox ){
				m_pListBox->SetSelectedItem(m_pListBox->GetSelectedItem() + 1, true);
				return;
				}
			else{
				ShowListBox();
				}
			break;
			}
		case VK_UP:
			{
			if( m_pListBox ){
				m_pListBox->SetSelectedItem(m_pListBox->GetSelectedItem() - 1, true);
				return;
				}
			else{
				ShowListBox();
				}
			break;
			}
		};

	ESChildTextBox::OnKeyDown(nChar, nRepCnt, nFlags);
	}

void
ESChildComboBox::OnMouseWheel(UINT nFlags, short zDelta, _Point pt){
	ESChildTextBox::OnMouseWheel(nFlags, zDelta, pt);
	if( m_pOwner->HasFocus(this) )
		ShowListBox(false);
	}

_Rect
ESChildComboBox::GetTextAreaRect(){
	_Rect rcTextArea = ESChildTextBox::GetTextAreaRect();
	rcTextArea.right -= m_rcBoxButton.Width();
	return rcTextArea;
	}

void
ESChildComboBox::OnListBoxItemClick(ESChildComboListBox* pSender, ChildControlEventCode eventCode, int nParam){
	int nItem	= pSender->GetSelectedItem();
	SetText(pSender->GetItemText(nItem), true, true);
	m_nSelectedItemIndex = nItem;
	// Send selection change.
	SendEvent(this, EventCode_ComboBoxSelChange, (void*)m_pListBox->GetItemParam(m_nSelectedItemIndex));
	}

void
ESChildComboBox::ShowListBox(bool bHide /*= false*/){
	if( bHide ){
		if( m_pListBox ){
			SendEvent(this, EventCode_ComboBoxCloseUp);
			_Rect rcListBox = m_pListBox->GetClientRect();
			m_pListBox->SetVisible(false, false);
			m_pOwner->RemoveChildControl(m_pListBox);
			m_pListBox = NULL;
			RedrawRect	(rcListBox,		true);
			RedrawRect	(m_rcBoxButton, true);
			}
		return;
		}

	if( m_pListBox == NULL ){
		m_pListBox			= new ESChildComboListBox();

		int nItems			= m_content.GetCount();
		int nItemCY			= m_pListBox->GetItemHeight();
		int nListBoxHeight	= m_nListBoxHeight;
		if( nListBoxHeight > nItems*nItemCY ){
			nListBoxHeight = nItems*nItemCY;
			}

		_Rect rcListBox(m_rRect.left, m_rRect.bottom + 1, m_rRect.right, (m_rRect.bottom + 1) + nListBoxHeight);
		_Rect rcOwner;
        
		if( !m_pParent )
			m_pOwner->GetClientRectMy(rcOwner);
		else
			rcOwner = m_pParent->GetClientRect();

		if( rcOwner.bottom < rcListBox.bottom ){
			rcListBox.bottom	= m_rRect.top - 1;
			rcListBox.top		= rcListBox.bottom - nListBoxHeight;
			}

		SendEvent(this, EventCode_ComboBoxDropDown);

		if( m_pOwner->AddChildControl(m_pListBox, MercuryBaseView::GetChildControlIdUnused(m_pOwner), rcListBox, m_pParent, 100) ){
			for(int i=0; i<m_content.GetCount(); i++)
				m_pListBox->AddItem(m_content.GetKey(i), m_content.GetValueByIndex(i), false);

			m_pListBox->GetVScrollBar()->OwnerWindowSizeChanged(rcOwner.Width(), rcOwner.Height());

			int nIndex = -1;
			if( m_content.KeyExists(GetText(), &nIndex) )
				m_pListBox->SetSelectedItem(nIndex, false, true);

			m_pListBox->AddEventListener((ESFrameBase*)this, ChildControlEventCode::EventCode_ListBoxItemClick, (ControlEventListener)&ESChildComboBox::OnListBoxItemClick);
			m_pListBox->SetTextFont		(&m_textFont, false);
			m_pOwner->RedrawRect		(rcListBox, 1);
			RedrawRect					(m_rcBoxButton, true);
			}
		else{
			delete m_pListBox;
			m_pListBox = NULL;
			return;
			}
		}
	else{
		for(int i=0; i<m_content.GetCount(); i++)
			m_pListBox->AddItem(m_content.GetKey(i), m_content.GetValueByIndex(i), false);

		int nIndex = -1;
		if( m_content.KeyExists(GetText(), &nIndex) )
			m_pListBox->SetSelectedItem(nIndex, false, true);
		}
	}

bool
ESChildComboBox::IsListBoxVisible(){
	if( m_pListBox )
		return true;
	return false;
	}
