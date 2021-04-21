// ESItemContainer.cpp : implementation file
//

#include "stdafx.h"
#include "MercuryGUI.h"
#include "ESControlContainer.h"
#include "ESItemsList.h"
#include "..\Image\ImageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESItemContainer

ESControlContainer::ESControlContainer() : MercuryBaseView(){
	/*
	m_pView			= NULL;
	m_bView			= FALSE;
	m_pHeader		= NULL;
	m_bHeader		= FALSE;
	m_pVScroll		= NULL;
	m_bVScroll		= FALSE;
	m_pHScroll		= NULL;
	m_bHScroll		= FALSE;
	m_pSwitchBar	= NULL;
	m_bSwitchBar	= FALSE;
	m_pTree			= NULL;
	m_bTree			= FALSE;
	m_nScrollsCt	= 0;
	*/
	}

IMPLEMENT_DYNCREATE(ESControlContainer, MercuryBaseView)

ESControlContainer::~ESControlContainer(){
	//DestroyClassMembers();
	}

BEGIN_MESSAGE_MAP(ESControlContainer, MercuryBaseView)
	/*
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_CHAR			()
	ON_WM_KEYDOWN		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_SETCURSOR		()
	ON_WM_MOUSEWHEEL	()
	ON_WM_GETDLGCODE	()
	ON_WM_SHOWWINDOW	()
	ON_WM_MOUSEACTIVATE	()
	ON_MESSAGE			(WM_VSCROLL, OnVScroll)
	ON_MESSAGE			(WM_HSCROLL, OnHScroll)
	ON_WM_GETMINMAXINFO	()
	ON_WM_SIZE			()
	ON_WM_MOUSEWHEEL	()
	ON_WM_KEYDOWN		()*/
END_MESSAGE_MAP()

BOOL 
ESControlContainer::Create(LPCTSTR lpszClassName,
						LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect,
						CWnd* pParentWnd, UINT nID,
						CCreateContext* pContext /*= NULL*/){
							/*
	BOOL bRet =  CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if( bRet ){
		CRect rcEmpty(0, 0, 0, 0);
		m_pView			= new ESItemsList();
		m_pHeader		= new ESHeaderCtrl();
		m_pVScroll		= new MercuryScrollBar();
		m_pHScroll		= new MercuryScrollBar();
		m_pSwitchBar	= new ESSwitchBar();
		m_pTree			= new MercuryTreeGrid();

		m_pView->m_pRoot		= this;
		m_pHeader->m_pRoot		= this;
		m_pSwitchBar->m_pRoot	= this;


	m_pRowSet				= new VirtualRowSet;
	m_pRowSet->SetOwner(this);

	m_pRowSet->AddField(_T("ID"), FieldType::FTInt32);
	m_pRowSet->AddFieldEx(_T("NAME"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME1"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME2"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME3"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME4"), FieldType::FTChar, 64);
	m_pRowSet->AddFieldEx(_T("NAME5"), FieldType::FTChar, 64);
	m_pRowSet->AddField(_T("_STATE"), FieldType::FTInt32);

	if( m_pRowSet->Create() > 0 ){

		CString sTitle;
		int nLoop = 0, nCt = 1000;
		while( nLoop < nCt ){

			m_pRowSet->AddRow();
			m_pRowSet->SetInt32(_T("ID"), nLoop);
			sTitle.Format(_T("sdfsdfsdfsd - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME1"), sTitle);
			sTitle.Format(_T("ghfghfghfg - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME2"), sTitle);
			sTitle.Format(_T("cvbcvbcvbcv - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME3"), sTitle);
			sTitle.Format(_T("tyutyuuioui - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME"), sTitle);
			sTitle.Format(_T("kl;khjhjkhj - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME4"), sTitle);
			sTitle.Format(_T("bnmbnmbnmbnm - %d"), nLoop);
			m_pRowSet->SetString(_T("NAME5"), sTitle);

			nLoop ++;
			}

		m_pRowSet->SetCurrentRow(0);
		}
	// ----------------

		m_bView	= m_bHeader = m_bVScroll = m_bHScroll = m_bSwitchBar = m_bTree = TRUE;

		UINT nID = 1000;
		m_pView		->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pHeader	->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pVScroll	->Create(WS_CHILD|WS_VSCROLL, rcEmpty, this, nID++, NULL);
		m_pHScroll	->Create(WS_CHILD|WS_HSCROLL, rcEmpty, this, nID++, NULL);
		m_pSwitchBar->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pTree		->Create(WS_VISIBLE|WS_CHILD, rcEmpty, this, nID++, NULL);
		m_pTree->SetBorders	(FALSE, TRUE, TRUE, FALSE);

		// TEMP_CODE
		m_pHeader->AddItem(_T("item1"), _T("ID"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item2"), _T("NAME1"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item3"), _T("NAME2"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item4"), _T("NAME3"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item5"), _T("NAME4"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item6"), _T("NAME5"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item7"), _T("NAME6"), TEXT_HALIGN_LEFT, 120, 40);
		m_pHeader->AddItem(_T("item8"), _T("NAME7"), TEXT_HALIGN_LEFT, 120, 40);
		// TEMP_CODE

		m_pHeader	->SetRowSet(m_pRowSet);
		m_pView		->SetRowSet(m_pRowSet);

		int nImg = m_pTree->AddItemImage(_T("d:\\c++\\folder_24.png"));
		int nImg2 = m_pTree->AddItemImage(_T("d:\\c++\\documents.png"));

		MerTreeItem* pRoot = m_pTree->AddItem(_T("registracia"));
		MerTreeItem* pItem = NULL;
		pRoot->iImage = nImg;
		pRoot->iSelImage = nImg;
		

		pItem = m_pTree->AddItem(_T("pacienti"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;

		pItem = m_pTree->AddItem(_T("servisi"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;

		pItem = m_pTree->AddItem(_T("personali"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;

		pRoot = m_pTree->AddItem(_T("Root2"));
		pRoot->iImage = nImg;
		pRoot->iSelImage = nImg;
		for(int i=0; i<2; i++)
		{
		pItem = m_pTree->AddItem(_T("Item1"), pRoot);
		pItem->iImage = nImg2;
		pItem->iSelImage = nImg2;
		}

		m_controlRects.Add(VScrolBar, CRect(0, 0, 0, 0));
		m_controlRects.Add(HScrolBar, CRect(0, 0, 0, 0));
		m_controlRects.Add(ItemsListCtrl, CRect(0, 0, 0, 0));
		m_controlRects.Add(HeaderCtrl, CRect(0, 0, 0, 0));
		m_controlRects.Add(SwitchBar, CRect(0, 0, 0, 0));
		m_controlRects.Add(TreeControl, CRect(0, 0, 0, 0));

		if( !CRect(rect).IsRectEmpty() ){
			RecalcChildWndRects	(CRect(rect).Width(), CRect(rect).Height());
			SizeAllChildWindows	();
			AdjustScrollBars	();
			}
		}
	return bRet;*/return FALSE;
	}

