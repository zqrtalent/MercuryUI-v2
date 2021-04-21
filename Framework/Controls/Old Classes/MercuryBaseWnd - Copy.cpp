// MercuryBaseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ESChildControl.h"
#include "MercuryBaseWnd.h"
#include "ESDialogBase.h"
#include "..\Image\ImageHelper.h"
#include "..\Utility\GrowableMemory.h"
#include "..\Path\Path.h"
#include <io.h>

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ####################################################
//
// class ESChildControlAttribute
//
// ####################################################
ESChildControlAttribute::ESChildControlAttribute(){
	m_nId			= 0;
	m_bVisible		= false;
	m_pControl		= NULL;
	}

BEGIN_DECLARE_VAR(ESChildControlAttribute, Serializable)
	DECLARE_VAR(_T("ESChildControlAttribute"),		_T(""), VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("ClassName"),					_T(""), VariableType::String, offsetof(ESChildControlAttribute,m_sClassName), false, false, false)
	DECLARE_VAR(_T("Name"),							_T(""), VariableType::String, offsetof(ESChildControlAttribute,m_sName), false, false, false)
	DECLARE_VAR(_T("Id"),							_T(""), VariableType::Int32, offsetof(ESChildControlAttribute,m_nId), false, false, false)
	DECLARE_VAR(_T("Visibility"),					_T(""), VariableType::Bool, offsetof(ESChildControlAttribute,m_bVisible), false, false, false)
END_DECLARE_VAR()

ESChildControlAttribute::~ESChildControlAttribute(){
	}

// ####################################################
//
// class ESDesignFileInfo
//
// ####################################################
ESDesignFileInfo::ESDesignFileInfo(){
	}

BEGIN_DECLARE_VAR(ESDesignFileInfo, Serializable)
	DECLARE_VAR(_T("ESDesignFileInfo"),		_T(""), VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("VersionMajor"),			_T(""), VariableType::Int32, offsetof(ESDesignFileInfo,m_nVersionMajor), false, false, false)
	DECLARE_VAR(_T("VersionMinor"),			_T(""), VariableType::Int32, offsetof(ESDesignFileInfo,m_nVersionMinor), false, false, false)
	DECLARE_VAR(_T("CreationDateTime"),		_T(""), VariableType::DateTime, offsetof(ESDesignFileInfo,m_dtCreationDateTime), false, false, false)
	DECLARE_VAR(_T("Author"),				_T(""), VariableType::String, offsetof(ESDesignFileInfo,m_sAuthor), false, false, false)
	DECLARE_VAR(_T("Description"),			_T(""), VariableType::String, offsetof(ESDesignFileInfo,m_sDescription), false, false, false)
END_DECLARE_VAR()

ESDesignFileInfo::~ESDesignFileInfo(){
	}

// ####################################################
//
// class MercuryBaseView
//
// ####################################################

MercuryBaseView::MercuryBaseView() : m_layerMan(this) {
	// Register child classes. {{
	ESChildControl::RegisterChildClass(_T("ESChildControlLabel"),		new ESChildControlLabel);
	ESChildControl::RegisterChildClass(_T("ESChildControlImage"),		new ESChildControlImage);
	ESChildControl::RegisterChildClass(_T("ESChildControlImageButton"), new ESChildControlImageButton);
	ESChildControl::RegisterChildClass(_T("ESChildControlList"),		new ESChildControlList);
	ESChildControl::RegisterChildClass(_T("ESChildTextBox"),			new ESChildTextBox);
	ESChildControl::RegisterChildClass(_T("ESChildControlTabCtrl"),		new ESChildControlTabCtrl);
	ESChildControl::RegisterChildClass(_T("ESChildBanner"),				new ESChildBanner);
	ESChildControl::RegisterChildClass(_T("ESChildControlCheckBox"),	new ESChildControlCheckBox);	
	ESChildControl::RegisterChildClass(_T("ESChildScrollBar"),			new ESChildScrollBar);
	ESChildControl::RegisterChildClass(_T("ESChildTabView"),			new ESChildTabView);
	ESChildControl::RegisterChildClass(_T("ESChildComboBox"),			new ESChildComboBox);
	ESChildControl::RegisterChildClass(_T("ESSimpleRenderView"),		new ESSimpleRenderView);
	ESChildControl::RegisterChildClass(_T("ESChildSlider"),				new ESChildSlider);
	ESChildControl::RegisterChildClass(_T("ESChildControlPropertyGrid"),new ESChildControlPropertyGrid);
	ESChildControl::RegisterChildClass(_T("ESChildProgressCtrl"),		new ESChildProgressCtrl);
	ESChildControl::RegisterChildClass(_T("ESChildAvatar"),				new ESChildAvatar);
	// }}

	m_designInfo.ZeroInit();
	m_bActive				= TRUE;
	m_pGUI					= MercuryGUI::GetInstance();
	m_pRowSet				= NULL;
	m_bUseForegroundColor	= FALSE;
	m_crForeground			= RGB(0, 10, 30);
	m_nAlpha				= 150;
	m_pFirstChild			= NULL;
	m_pControlUnderCursor	= NULL;
	m_pControlCaptured		= NULL;
	m_pChildFocused			= NULL;
	m_bDesignerMode			= false;
	m_pListOfControls		= NULL;
	m_bInvalidRectsCaching	= false;

	m_hDoubleBufferImage	= NULL;
	memset(&m_bmDoubleBufferImage, sizeof(BITMAP), 0);

	m_info.m_pView			= this;
	m_bTooltipIsTracking	= false;

	// Create variable indexes. {{
	m_listChildControlInfos.CreateVariableIndex(_T("Id"), Serializable::Compare_Int32);
	m_listChildControlInfos.CreateVariableIndex(_T("Name"), Serializable::Compare_String);
	//m_listChildControlInfos.CreateVariableIndex(_T("ZIndex"), Serializable::Compare_Int32);
	// }}

	m_bAllowZooming				= false;
	m_fRatioCX					= 0.0f;
	m_fRatioCY					= 0.0f;
	m_szDefWindowSize.SetSize(0, 0);

	m_bMoving					= false;
	m_bSizing					= false;
	m_nSizingHitTest			= -1;

	m_bRedrawOptimizeTimerIsOn	= false;
	m_nRedrawOptimizeTimeId		= 0;
	}

IMPLEMENT_DYNCREATE(MercuryBaseView, CView)

MercuryBaseView::~MercuryBaseView(){
	if( m_pRowSet && m_pRowSet->GetOwner() == this ){
		delete m_pRowSet;
		m_pRowSet = NULL;
		}
	m_listCachedInvalidRects.RemoveAll(true);

	// Destroy attributes of thread rect caching mechanism. {{
	m_lockRectCachingThread.Lock(); // Lock
	for(int i=0; i<m_arrRectCachingInfoByThreadId.GetCount(); i++){
		RedrawRectCachingStruct* pInfo = (RedrawRectCachingStruct*)m_arrRectCachingInfoByThreadId.GetData(i);
		if( pInfo ) delete pInfo;
		}
	m_arrRectCachingInfoByThreadId.DeleteAll();
	m_lockRectCachingThread.Unlock(); // Unlock
	// }}
	
	// Empty clipboard.
	if( m_bDesignerMode ){
		UINT	nFormatId	= ::RegisterClipboardFormat(_T("ES_BINARY"));
		HGLOBAL hglbCopy	= ::GetClipboardData(nFormatId);

		if( hglbCopy ){
			GlobalFree(hglbCopy);
			SetClipboardData(nFormatId, NULL);
			}
		}

	if( m_toolTipCtrl.m_hWnd )
		m_toolTipCtrl.DestroyWindow();

	// Free double buffer image objects. {{
	if( m_hDoubleBufferImage ){
		::DeleteObject(m_hDoubleBufferImage);
		m_hDoubleBufferImage = 0;
		}
	memset(&m_bmDoubleBufferImage, sizeof(BITMAP), 0);
	// }}

	// Destroy child controls.
	DestroyAllChilds();

	// Destroy property dialog.
	if( IsWindow(m_propertyDlg) )
		::DestroyWindow(m_propertyDlg);

	// Destroy list of controls dialog.
	if( IsWindow(m_listOfControlsDlg) )
		::DestroyWindow(m_listOfControlsDlg);

	// Delete list of controls object.
	if( m_pListOfControls ){
		m_pListOfControls->RemoveAll(true);
		delete m_pListOfControls;
		}
	}

BEGIN_MESSAGE_MAP(MercuryBaseView, CView)
	ON_WM_CREATE		()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_SETFOCUS		()
	ON_WM_KILLFOCUS		()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONDBLCLK	()
	ON_WM_LBUTTONUP		()
	ON_WM_RBUTTONUP		()
	ON_WM_SETCURSOR		()
	ON_WM_GETDLGCODE	()
	ON_WM_MOUSEWHEEL	()
	ON_WM_KEYDOWN		()
	ON_WM_CHAR			()
	ON_WM_TIMER			()
	ON_MESSAGE			(WM_REDRAW_RECT, OnRedrawRect)
	ON_WM_SIZE			()

	ON_COMMAND			(MI_LABEL,						OnCommandLabel)
	ON_COMMAND			(MI_IMAGE,						OnCommandImage)
	ON_COMMAND			(MI_IMAGEBUTTON,				OnCommandImageButton)
	ON_COMMAND			(MI_TEXTBOX,					OnCommandTextBox)
	ON_COMMAND			(MI_CHECKBOX,					OnCommandCheckBox)
	ON_COMMAND			(MI_COMBOBOX,					OnCommandComboBox)
	ON_COMMAND			(MI_SLIDER,						OnCommandSlider)
	ON_COMMAND			(MI_PROGRESSCTRL,				OnCommandProgressCtrl)
	ON_COMMAND			(MI_AVATAR,						OnCommandAvatar)
	ON_COMMAND			(MI_SIMPLERENDERVIEW,			OnCommandSimpleRenderView)
	ON_COMMAND			(MI_LIST,						OnCommandList)
	ON_COMMAND			(MI_TREE,						OnCommandTree)
	ON_COMMAND			(MI_TAB,						OnCommandTab)
	ON_COMMAND			(MI_TABVIEW,					OnCommandTabView)
	ON_COMMAND			(MI_TREEVIEW,					OnCommandTreeView)
	ON_COMMAND			(MI_BANNER,						OnCommandBanner)
	ON_COMMAND			(MI_PROPERTYGRID,				OnCommandPropertyGrid)

	ON_COMMAND			(MI_LABEL_AS_CHILD,				OnCommandLabelAsChild)
	ON_COMMAND			(MI_IMAGE_AS_CHILD,				OnCommandImageAsChild)
	ON_COMMAND			(MI_IMAGEBUTTON_AS_CHILD,		OnCommandImageButtonAsChild)
	ON_COMMAND			(MI_TEXTBOX_AS_CHILD,			OnCommandTextBoxAsChild)
	ON_COMMAND			(MI_CHECKBOX_AS_CHILD,			OnCommandCheckBoxAsChild)
	ON_COMMAND			(MI_COMBOBOX_AS_CHILD,			OnCommandComboBoxAsChild)
	ON_COMMAND			(MI_SLIDER_AS_CHILD,			OnCommandSliderAsChild)
	ON_COMMAND			(MI_PROGRESSCTRL_AS_CHILD,		OnCommandProgressCtrlAsChild)
	ON_COMMAND			(MI_AVATAR_AS_CHILD,			OnCommandAvatarAsChild)
	ON_COMMAND			(MI_SIMPLERENDERVIEW_AS_CHILD,	OnCommandSimpleRenderViewAsChild)
	ON_COMMAND			(MI_LIST_AS_CHILD,				OnCommandListAsChild)
	ON_COMMAND			(MI_TREE_AS_CHILD,				OnCommandTreeAsChild)
	ON_COMMAND			(MI_TAB_AS_CHILD,				OnCommandTabAsChild)
	ON_COMMAND			(MI_TABVIEW_AS_CHILD,			OnCommandTabViewAsChild)
	ON_COMMAND			(MI_TREEVIEW_AS_CHILD,			OnCommandTreeViewAsChild)
	ON_COMMAND			(MI_BANNER_AS_CHILD,			OnCommandBannerAsChild)
	ON_COMMAND			(MI_PROPERTYGRID_AS_CHILD,		OnCommandPropertyGridAsChild)

	ON_COMMAND			(MI_ALIGN_LEFTS,				OnCommandAlignLefts)
	ON_COMMAND			(MI_ALIGN_TOPS,					OnCommandAlignTops)
	ON_COMMAND			(MI_ALIGN_RIGHTS,				OnCommandAlignRights)
	ON_COMMAND			(MI_ALIGN_BOTTOMS,				OnCommandAlignBottoms)

	ON_COMMAND			(MI_MAKE_SAME_SIZE_WIDTH,		OnCommandMakeSameSizeWidth)
	ON_COMMAND			(MI_MAKE_SAME_SIZE_HEIGHT,		OnCommandMakeSameSizeHeight)
	ON_COMMAND			(MI_MAKE_SAME_SIZE_BOTH,		OnCommandMakeSameSizeBoth)
	ON_COMMAND			(MI_LOCK,						OnCommandLock)
	ON_COMMAND			(MI_UNLOCK,						OnCommandUnlock)

	ON_COMMAND			(MI_COPY,						OnCommandCopy)
	ON_COMMAND			(MI_CUT,						OnCommandCut)
	ON_COMMAND			(MI_PASTE,						OnCommandPaste)
	ON_COMMAND			(MI_LIST_OF_CONTROLS,			OnCommandListOfControls)
	ON_COMMAND			(MI_OPEN_DESIGNER_FILE,			OnCommandOpenDesignerFile)
	ON_COMMAND			(MI_SAVE_DESIGNER_FILE,			OnCommandSaveDesignerFile)
	ON_COMMAND			(MI_PROPERTIES,					OnCommandProperties)
	ON_COMMAND			(MI_BASEVIEW_PROPERTIES,		OnCommandBaseViewProperties)
END_MESSAGE_MAP()

int
MercuryBaseView::OnCreate(LPCREATESTRUCT lcs){
	int nRet = CView::OnCreate(lcs);
	if( nRet != -1 ){
		ESFrameBase* pWindow	= (ESFrameBase*)GetParent();
		if( pWindow && pWindow->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
			m_info.m_szDefault	= pWindow->m_szDefault;
			m_info.m_szMin		= pWindow->m_szMin;
			m_info.m_szMax		= pWindow->m_szMax;
			}
		}
	return nRet;
	}

BOOL 
MercuryBaseView::OnEraseBkgnd(CDC* pDC){
	return TRUE;
	}

void
MercuryBaseView::OnDraw(CDC* pDC){
	}

BOOL 
MercuryBaseView::PreTranslateMessage(MSG* pMsg){
	if( m_toolTipCtrl.m_hWnd )
		m_toolTipCtrl.RelayEvent(pMsg);
	return CView::PreTranslateMessage(pMsg);
	}

void 
MercuryBaseView::OnPaint() {
	CPaintDC	dc(this);
	CRect		rcClient, rcClipBox;
	GetClientRectMy	(rcClient);
	if( rcClient.IsRectEmpty() )
		return;

	// Retrieve clipbox. ####################
	int nResult = dc.GetClipBox(&rcClipBox);
	if( rcClipBox.IsRectEmpty() )
		rcClipBox = rcClient;
	else
		rcClipBox &= rcClient;

	if( nResult == COMPLEXREGION )
		rcClipBox = rcClient;

//	float fZoom =  !UseDoubleBufferedDC() ? 1.0f : GetZoomFactor();
	float fZoom =  1.0f;
	CRect rcRectNonZoom	(0, 0, rcClient.Width(), rcClient.Height());
	CRect rcRectZoom	(0, 0, rcRectNonZoom.Width()*fZoom, rcRectNonZoom.Height()*fZoom);

	// Call PrePaint event.
	OnPrePaint(rcClipBox);

	if( rcClipBox.IsRectEmpty() ) return;
	// #####################################

	Redraw(rcClipBox);
	}

void
MercuryBaseView::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	RedrawChildControls(m_pFirstChild, pDC, pRectDC, pRectClient);
	}

bool
MercuryBaseView::Redraw(CRect rcRedraw, ESChildControl* pChildOnly /*= NULL*/){
	CRect	rcClipBox	= rcRedraw, rcClient;
	GetClientRect(rcClient);
	rcClient.SetRect(0, 0, rcClient.Width(), rcClient.Height());

	if( UseDoubleBufferedDC() == FALSE && m_bUseForegroundColor == FALSE ){
		CDC*	pDC			= GetDC();
		ASSERT(pDC);
		if( !pDC )  return false;

		OnPaintClient(pDC, &rcClipBox, &rcClipBox);
		ReleaseDC(pDC);
		return true;
		}

	// Invalid rects caching. {{
	if( m_bInvalidRectsCaching ){
		RECT* pRectNew		= new RECT;
		pRectNew->left		= rcRedraw.left;
		pRectNew->top		= rcRedraw.top;
		pRectNew->right		= rcRedraw.right;
		pRectNew->bottom	= rcRedraw.bottom;
		m_listCachedInvalidRects.Add(pRectNew);
		return true;
		}
	// }}
	
	CDC*	pDC			= GetDC();
	ASSERT(pDC);
	if( !pDC )  
		return false;

	CDC		memDC;
	memDC.CreateCompatibleDC(pDC);
	if( (!m_hDoubleBufferImage && rcClient.Width() > 0 && rcClient.Height() > 0) || 
		rcClient.Width() > m_bmDoubleBufferImage.bmWidth || 
		rcClient.Height() > m_bmDoubleBufferImage.bmHeight ){

		if( m_hDoubleBufferImage ){
			::DeleteObject(m_hDoubleBufferImage);
			m_hDoubleBufferImage = NULL;
			memset(&m_bmDoubleBufferImage, sizeof(BITMAP), 0);
			}

		CBitmap	memBmp;
		ImageHelper::CreateDIBBitmap(24, memBmp, (COLORREF)0, rcClient.Width(), rcClient.Height(), &m_bmDoubleBufferImage, &memDC);
		m_hDoubleBufferImage = (HBITMAP)memBmp.Detach();
		}
	/*
	// Temp
	BITMAPINFOHEADER	bmpInfo32;
	memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo32.biBitCount		= 24;
	bmpInfo32.biCompression		= BI_RGB;
	bmpInfo32.biPlanes			= 1;
	bmpInfo32.biHeight			= 5;
	bmpInfo32.biSize			= sizeof(BITMAPINFOHEADER);
	bmpInfo32.biWidth			= 5;

	UINT*	lpMap1				= NULL;
	HBITMAP hDib1				= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, (void**)&lpMap1, NULL, 0);
	ASSERT( lpMap1 );

	UINT*	lpMap2				= NULL;
	HBITMAP hDib2				= CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, (void**)&lpMap2, NULL, 0);
	ASSERT( lpMap2 );

	::SelectObject(memDC, m_hDoubleBufferImage);
	if( (HBITMAP)SelectObject(memDC, hDib2) == m_hDoubleBufferImage ){
		AfxMessageBox("sdfsdfs");
		}

	::DeleteObject(hDib1);
	::DeleteObject(hDib2);
	// Temp
	*/

	/*
	CBitmap bmpTemp;
	bmpTemp.CreateCompatibleBitmap(&memDC, 10, 10);

	CBitmap bmpTemp1;
	bmpTemp1.CreateCompatibleBitmap(&memDC, 10, 10);

	::SelectObject(memDC, bmpTemp.m_hObject);
	if( (HBITMAP)SelectObject(memDC, bmpTemp1.m_hObject) == bmpTemp.m_hObject ){
		AfxMessageBox("sdfsdfs");
		}*/


	::SelectObject(memDC.m_hDC, m_hDoubleBufferImage);

	/*
	if( m_hDoubleBufferImage ){
		::DeleteObject(m_hDoubleBufferImage);
		m_hDoubleBufferImage = 0;
		}
	memset(&m_bmDoubleBufferImage, sizeof(BITMAP), 0);
	*/

	/*
	bool	bDetach = false;
	CBitmap	memBmp;

	if( !ImageHelper::CreateDIBBitmap(24, memBmp, (COLORREF)0, rcClipBox.Width(), rcClipBox.Height()) )
		memBmp.CreateCompatibleBitmap(pDC, rcClipBox.Width(), rcClipBox.Height());
	memDC.SelectObject(&memBmp);
	*/
	// ######################################

	// Call virtual function 'OnPaintClient'.
	CRect rcRectDC(0, 0, rcClipBox.Width(), rcClipBox.Height());
	// Draw background first.
	DrawBackground(&memDC, &rcRectDC, &rcClipBox, rcClient, m_info.m_bgFillMode.GetCurrentValue(), m_info.m_crFill, &m_info.m_bgImage, m_info.m_rcOmitBorder, true);

	if( pChildOnly ){ // Redraw only one child control.
		CRect rcControl	= pChildOnly->GetClientRect() & rcClipBox;
		if( !rcControl.IsRectEmpty() && pChildOnly->m_bVisible ){
			rcRectDC.left	= (rcControl.left - rcClipBox.left);
			rcRectDC.top	= (rcControl.top - rcClipBox.top);
			rcRectDC.right	= (rcRectDC.left + rcControl.Width());
			rcRectDC.bottom = (rcRectDC.top + rcControl.Height());
			pChildOnly->OnPaintClient(&memDC, &rcRectDC, &rcControl);
			}
		}
	else// Redraw all by rect region.
		OnPaintClient(&memDC, &rcRectDC, &rcClipBox);
	// ######################################
	
	// Copy from memory DC to real window DC (small parts).
	if( m_bUseForegroundColor )
		ImgHlp::AlphaColorOnBitmap(m_hDoubleBufferImage, 0, 0, rcClipBox.Width(), rcClipBox.Height(), m_crForeground, (BYTE)m_nAlpha);
	pDC->BitBlt(rcClipBox.left, rcClipBox.top, rcClipBox.Width(), rcClipBox.Height(), &memDC, 0, 0, SRCCOPY);
	// ######################################

	/*
	// Destroy memory DC and bitmap. ########
	if( !bDetach )
		memBmp.DeleteObject();
	else
		memBmp.Detach();
	memDC	.DeleteDC();
	// ######################################
	*/

	memDC.DeleteDC	();
	ReleaseDC		(pDC);
	return true;
	}

bool
MercuryBaseView::GetZoom(float& fZoomCX, float& fZoomCY){
	if( m_bAllowZooming ){
		if( !m_fRatioCX || !m_fRatioCY )
			return false;
		fZoomCX = m_fRatioCX;
		fZoomCY = m_fRatioCY;
		return true;
		}
	return false;
	}

bool
MercuryBaseView::AddNamedParameter(LPCTSTR lpszParam, void* lpParam, bool bUpdateIfExists /*= false*/){
	if( m_arrNamedParameters.KeyExists(lpszParam) ){
		if( bUpdateIfExists ){
			m_arrNamedParameters.SetValue(lpszParam, lpParam);
			return true;
			}
		return false; // Key exists!!!
		}
	m_arrNamedParameters.Add(lpszParam, lpParam);
	return true;
	}

bool
MercuryBaseView::RemoveNamedParameter(LPCTSTR lpszParam, void*& lpParam){
	if( !m_arrNamedParameters.KeyExists(lpszParam) )
		return false;
	lpParam = m_arrNamedParameters.GetValue(lpszParam);
	m_arrNamedParameters.Delete(lpszParam, false);
	return true;
	}

bool
MercuryBaseView::GetNamedParameter(LPCTSTR lpszParam, void*& lpParam){
	if( !m_arrNamedParameters.KeyExists(lpszParam) )
		return false;
	lpParam = m_arrNamedParameters.GetValue(lpszParam);
	return true;
	}

bool
MercuryBaseView::PtInImageRegion(CPoint ptInOwnerCoords, ESChildControl* pChild, CRgn* pImageRgn, CSize szImage, bool bCentered){
	if( !pChild || !pImageRgn ) return false;
	MercuryBaseView* pOwner = pChild->GetOwner();
	if( !pOwner || !pImageRgn || !pImageRgn->m_hObject ) 
		return false;

	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	pOwner->GetZoom(fZoomCX, fZoomCY);
	CRect	rcRgnImage, rcClient = pChild->GetClientRect();

	if( bCentered ){
		rcRgnImage.left		= rcClient.left + (rcClient.Width() - szImage.cx*fZoomCX)/2;
		rcRgnImage.top		= rcClient.top + (rcClient.Height() - szImage.cy*fZoomCY)/2;
		rcRgnImage.right	= rcRgnImage.left + szImage.cx*fZoomCX;
		rcRgnImage.bottom	= rcRgnImage.top + szImage.cy*fZoomCY;
		}
	else{
		rcRgnImage.left		= rcClient.left;
		rcRgnImage.top		= rcClient.top;
		rcRgnImage.right	= rcRgnImage.left + szImage.cx*fZoomCX;
		rcRgnImage.bottom	= rcRgnImage.top + szImage.cy*fZoomCY;
		}

	CPoint	ptClient((ptInOwnerCoords.x - rcRgnImage.left)/fZoomCX, (ptInOwnerCoords.y - rcRgnImage.top)/fZoomCY);
	if( pImageRgn->PtInRegion(ptClient.x, ptClient.y) )
		return true;	
	return false;
	}

bool
MercuryBaseView::DrawBackground(CDC* pDC, CRect* pRectDC, CRect* pRectClient, CRect rcControl, int nMode, COLORREF crFill, CImage* pImageBg, 
								CRect rcOmitBorder, bool bSizeBox /*= false*/, int nScaleMode /*= 1*/){
	MercuryGUI* pMerGUI		= MercuryGUI::GetInstance();
	CRect		rcBackgroundDC;
	rcBackgroundDC.left		= (rcControl.left - pRectClient->left) + pRectDC->left; 
	rcBackgroundDC.top		= (rcControl.top - pRectClient->top) + pRectDC->top; 
	rcBackgroundDC.right	= rcBackgroundDC.left + rcControl.Width(); 
	rcBackgroundDC.bottom	= rcBackgroundDC.top + rcControl.Height();

	CRect		rcInvalid	= rcBackgroundDC & *pRectDC;
	if( rcInvalid.IsRectEmpty() )
		return false;

	int		nFillMode		= nMode;
	bool	bRet			= false;

	switch( nFillMode ){
		case FillMode::FillMode_Color:{
			bRet = true;
			CBrush brush(crFill);
			pDC->FillRect(rcInvalid, &brush);
			break;
			}
		case FillMode::FillMode_StretchImage:{
			if( pImageBg && !pImageBg->IsNull()){
				bRet = true;
				ImageHelper::RenderImage(pDC, rcInvalid, rcBackgroundDC, pImageBg, true, (pImageBg->GetBPP() == 32), (ScaleMode)nScaleMode);
				}
			break;
			}
		case FillMode::FillMode_OmitBorderedImage:{
			if( pImageBg && !pImageBg->IsNull()){
				bRet = true;
				ImageHelper::RenderImage_OmitBorder(pDC, rcInvalid, rcBackgroundDC, pImageBg, rcOmitBorder, (pImageBg->GetBPP() == 32));
				}
			break;
			}
		case FillMode::FillMode_RepeatedImage:{
			if( pImageBg && !pImageBg->IsNull() ){
				bRet = true;
				ImageHelper::RenderImage(pDC, rcInvalid, rcBackgroundDC, pImageBg, false, false);
				}
			break;
			}
		case FillMode::FillMode_CentredImage:{
			if( pImageBg && !pImageBg->IsNull() ){
				CRect rcImageCenter;
				rcImageCenter.left		= rcControl.left + (rcControl.Width() - pImageBg->GetWidth())/2;
				rcImageCenter.top		= rcControl.top + (rcControl.Height() - pImageBg->GetHeight())/2;
				rcImageCenter.right		= rcImageCenter.left + pImageBg->GetWidth();
				rcImageCenter.bottom	= rcImageCenter.top + pImageBg->GetHeight();

				CRect rcImageCenterDC;
				rcImageCenterDC.left	= (rcImageCenter.left - pRectClient->left) + pRectDC->left; 
				rcImageCenterDC.top		= (rcImageCenter.top - pRectClient->top) + pRectDC->top;
				rcImageCenterDC.right	= rcImageCenterDC.left + pImageBg->GetWidth();
				rcImageCenterDC.bottom	= rcImageCenterDC.top + pImageBg->GetHeight();

				rcInvalid = *pRectDC&rcImageCenterDC;
				if( !rcInvalid.IsRectEmpty() ){
					ImageHelper::RenderImage(pDC, rcInvalid, rcImageCenterDC, pImageBg, false, (pImageBg->GetBPP() == 32));
					bRet = true;
					}
				}
			break;
			}
		};

	// Draw sizebox image. {{
	CImage* pImageSizeBox = &m_info.m_sizeBoxImage;
	if( bSizeBox && m_info.m_szMax != m_info.m_szMin && !pImageSizeBox->IsNull() ){
		CRect rcImage;
		rcImage.left		= rcControl.right - pImageSizeBox->GetWidth();
		rcImage.top			= rcControl.bottom - pImageSizeBox->GetHeight();
		rcImage.right		= rcImage.left + pImageSizeBox->GetWidth();
		rcImage.bottom		= rcImage.top + pImageSizeBox->GetHeight();

		CRect rcImageDC;
		rcImageDC.left		= (rcImage.left - pRectClient->left) + pRectDC->left; 
		rcImageDC.top		= (rcImage.top - pRectClient->top) + pRectDC->top;
		rcImageDC.right		= rcImageDC.left + pImageSizeBox->GetWidth();
		rcImageDC.bottom	= rcImageDC.top + pImageSizeBox->GetHeight();

		rcInvalid = *pRectDC&rcImageDC;
		if( !rcInvalid.IsRectEmpty() )
			ImageHelper::RenderImage(pDC, rcInvalid, rcImageDC, pImageSizeBox, false,  (pImageSizeBox->GetBPP() == 32));
		}
	// }}
	return bRet;
	}

void
MercuryBaseView::LocalizeStrings(){
	int nLoop = 0, nCt = m_listChildControlInfos.GetCount();
	while(nLoop < nCt){
		ESChildControlInfo* pInfo = m_listChildControlInfos.GetAt(nLoop);
		if( pInfo && pInfo->m_pControl )
			pInfo->m_pControl->OnLocalizeStrings();
		nLoop ++;
		}
	}

/*
bool
MercuryBaseView::AddToolTip(ESChildControl* pOwner, CString sToolText){
	if( !pOwner || sToolText.IsEmpty() ) 
		return false;

	if( !m_pToolTipControl ){
		m_pToolTipControl = new CPPToolTip();
		m_pToolTipControl->Create	(this, TRUE);
		m_pToolTipControl->SetNotify();
		}
	// Localize string.
	ESLocalizationMan::ParseText(sToolText);
	
//	m_pToolTipControl->AddTool(this, _T("<center><h2>ToolTip in ScrollView</h2><br><hr color=blue></center><br>ToolTip for <font color=red>RED</font> rectangle in View"), pOwner->GetClientRect(), pOwner->GetId());
	m_pToolTipControl->AddTool(this, sToolText.GetBuffer(), pOwner->GetClientRect(), pOwner->GetId());
	return true;
	}

bool
MercuryBaseView::RemoveToolTip(ESChildControl* pOwner){
	if( !m_pToolTipControl || !pOwner || !IsWindow(m_pToolTipControl->GetSafeHwnd()) )
		return false;
	m_pToolTipControl->DelTool(this, pOwner->GetId());
	return true;
	}

bool
MercuryBaseView::UpdateToolTipRect(ESChildControl* pOwner, CRect rcNew){
	if( !m_pToolTipControl || !pOwner || !IsWindow(m_pToolTipControl->GetSafeHwnd()) )
		return false;
	m_pToolTipControl->SetToolRect(this, pOwner->GetId(), &rcNew);
	return true;
	}

bool
MercuryBaseView::UpdateToolTipText(ESChildControl* pOwner, CString sToolText){
	if( !m_pToolTipControl || !pOwner || !IsWindow(m_pToolTipControl->GetSafeHwnd()) )
		return false;
		// Localize string.
	ESLocalizationMan::ParseText(sToolText);
	m_pToolTipControl->UpdateTipText(sToolText.GetBuffer(), this, pOwner->GetId());
	return true;
	}
*/

void
MercuryBaseView::OnTimer(UINT_PTR nIDEvent){
	int nIndex = m_arrChildControlByTimer.IndexOf((void*)nIDEvent);
	if( nIndex != -1 ){
		ESChildControl* pChild = (ESChildControl*)m_arrChildControlByTimer.GetData(nIndex);
		if( pChild )
			pChild->OnTimer((UINT)nIDEvent);
		}
	CView::OnTimer(nIDEvent);
	}

LRESULT
MercuryBaseView::OnRedrawRect(WPARAM wParam, LPARAM lParam){
	CPtrArray*	pArrRects	= (CPtrArray*)lParam;
	BOOL		bAtOnce		= (BOOL)wParam;

	int nLoop = 0;
	while( nLoop < pArrRects->GetCount() ){
		CRect* pRect = (CRect*)pArrRects->GetAt(nLoop);
		if( pRect ){
			RedrawRect(*pRect, bAtOnce);
			delete pRect;
			}
		nLoop ++;
		}
	delete pArrRects;
	return 0L;
	}

void _stdcall 
MercuryBaseView::RedrawOptimizeTimer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){
	MercuryBaseView* pView = (MercuryBaseView*)CWnd::FromHandle(hWnd);
	ASSERT(pView);
		
	pView->m_lockRedrawOptimizer.Lock();	// Lock
	pView->m_bRedrawOptimizeTimerIsOn	= false;
	::KillTimer(hWnd, pView->m_nRedrawOptimizeTimeId);
	pView->m_nRedrawOptimizeTimeId		= 0;
	pView->ExcludeIntersectionAreas(&pView->m_listRedrawRects);
	CPtrArray*	pArrRects	= new CPtrArray();
	for(int i=0; i<pView->m_listRedrawRects.GetCount(); i++){
		CRect*	pRectBound	= new CRect(pView->m_listRedrawRects[i]);
		if( !pRectBound->IsRectEmpty() )
			pArrRects->Add(pRectBound);
		else
			delete pRectBound;
		}

	TRACE2(_T("RedrawOptimizeTimer rectct=%d clock=%d \r\n"), pArrRects->GetCount(), clock());

	bool bSave = false;
	if( bSave ){
		CDC memDC;
		memDC.CreateCompatibleDC(NULL);

		CRect rcClient;
		pView->GetClientRect(rcClient);

		CBitmap bmImage;
		//ImageHelper::CreateDIBBitmap(32, bmImage, RGB(255, 255, 255), rcClient.Width(), rcClient.Height(), NULL);

		CDC* pDC = pView->GetDC();
		ImageHelper::GetBitmapFromDC(32, bmImage, pDC, 0, 0, rcClient.Width(), rcClient.Height(), NULL);
		pView->ReleaseDC(pDC);
		memDC.SelectObject(&bmImage);

		List<RECT>	listPartRects;
		CRect rc1, rc2, rc3;
		
		CString s, s2;
		int nRectsCt = 0;

		for(int i=0; i<pView->m_listRedrawRects.GetCount(); i++){
			RECT* pRect = pView->m_listRedrawRects.GetAt(i);
			rc1.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
			if( rc1.IsRectEmpty() ) continue;

			s.Format(_T("%d,%d,%d,%d\r\n"), pRect->left, pRect->top, pRect->right, pRect->bottom);
			s2 += s;
			//TRACE3(_T("%d %d %d"), pRect->left, pRect->top, pRect->right); 

			s.Format(_T("%d"), i);
			memDC.FillSolidRect(pRect->left, pRect->top, rc1.Width(), rc1.Height(), RGB(20 + i*10, 0, 0));
			nRectsCt ++;
			}

		memDC.SetTextColor(RGB(255, 255, 255));
		memDC.SetBkColor(RGB(0, 0, 0));
		for(int i=0; i<pView->m_listRedrawRects.GetCount(); i++){
			RECT* pRect = pView->m_listRedrawRects.GetAt(i);
			rc1.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
			if( rc1.IsRectEmpty() ) continue;

			s.Format(_T("%d (%d,%d,%d,%d)"), i, pRect->left, pRect->top, pRect->right, pRect->bottom);
		//	memDC.TextOut(pRect->left, pRect->top, s);
			}

		CPen pen(PS_DASHDOT, 1, RGB(0, 0, 30));
		memDC.SelectObject(&pen);
		CBrush br(RGB(255, 150, 255));
		memDC.SelectObject(&br);

		for(int i=0; i<listPartRects.GetCount(); i++){
			RECT* pRect = listPartRects.GetAt(i);
			rc1.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
			if( rc1.IsRectEmpty() ) continue;
			memDC.Rectangle(rc1);
			//memDC.FillSolidRect(rc1.left, rc1.top, rc1.Width(), rc1.Height(), RGB(255, 0, 255));
			//ImageHelper::AlphaColorOnBitmap(bmImage, rc1.left, rc1.top, rc1.Width(), rc1.Height(), RGB(0, 200, 0), 122);
			}

		CString sFileName;
		sFileName.Format(_T("d:\\111\\caching_rects%d (rects_%d).bmp"), clock(), nRectsCt);
		DeleteFile(sFileName.GetBuffer());
		ImageHelper::SaveBitmap(sFileName.GetBuffer(), bmImage, 32);

		bmImage.DeleteObject();
		memDC.DeleteDC();
		}

	pView->m_listRedrawRects.RemoveAll(true);
	pView->m_lockRedrawOptimizer.Unlock();	// Unlock

	pView->OnRedrawRect(1, (LPARAM)pArrRects);
	}

void
MercuryBaseView::RedrawRect(CRect rcRedraw, BOOL bRedrawAtOnce /*= FALSE*/){
	if( rcRedraw.top < 0 )
		rcRedraw.top = 0;
	if( rcRedraw.left < 0 )
		rcRedraw.left = 0;

	if( rcRedraw.Width() <= 0 || rcRedraw.Height() <= 0 || !::IsWindow(m_hWnd) )
		return;
	DWORD dwThreadId = ::GetCurrentThreadId();
	if( dwThreadId == ::GetWindowThreadProcessId(m_hWnd, NULL) ){
		if( bRedrawAtOnce )
			Redraw(rcRedraw);
		else
			InvalidateRect(rcRedraw, FALSE);
		}
	else{
		/*
		CPtrArray*	pArrRects	= new CPtrArray();
		CRect*		pRectBound	= new CRect(rcRedraw);
		pArrRects->Add(pRectBound);
		::PostMessage(m_hWnd, WM_REDRAW_RECT, bRedrawAtOnce, (LPARAM)pArrRects);
	*/
		/*
		m_lockRedrawOptimizer.Lock(); // Lock
		RECT* pRect		= new RECT;
		pRect->left		= rcRedraw.left;
		pRect->top		= rcRedraw.top;
		pRect->right	= rcRedraw.right;
		pRect->bottom	= rcRedraw.bottom;
		m_listRedrawRects.Add(pRect);
		if( !m_bRedrawOptimizeTimerIsOn )
			m_nRedrawOptimizeTimeId = SetTimer(20000, 10, RedrawOptimizeTimer);
		m_lockRedrawOptimizer.Unlock(); // Unlock
		*/

		m_lockRectCachingThread.Lock();		//Lock
		int nIndex = m_arrRectCachingInfoByThreadId.IndexOf((void*)dwThreadId);
		if( nIndex != -1 ){
			RedrawRectCachingStruct* pInfo = (RedrawRectCachingStruct*)m_arrRectCachingInfoByThreadId.GetData(nIndex);
			if( !pInfo )
				m_arrRectCachingInfoByThreadId.Delete(nIndex);
			else{
				RECT* pRect		= new RECT;
				pRect->left		= rcRedraw.left;
				pRect->top		= rcRedraw.top;
				pRect->right	= rcRedraw.right;
				pRect->bottom	= rcRedraw.bottom;
				pInfo->listCachedInvalidRects.Add(pRect);
				}
			m_lockRectCachingThread.Unlock();	//Unlock
			}
		else{
			m_lockRectCachingThread.Unlock();	//Unlock

			CPtrArray*	pArrRects	= new CPtrArray();
			CRect*		pRectBound	= new CRect(rcRedraw);
			pArrRects->Add(pRectBound);
			::PostMessage(m_hWnd, WM_REDRAW_RECT, bRedrawAtOnce, (LPARAM)pArrRects);
			}
		}
	}

void
MercuryBaseView::RedrawMultipleRects(CRect* pRectsRedraw, int nCt, BOOL bRedrawAtOnce /*= FALSE*/){
	if( !pRectsRedraw || nCt <= 0 || !::IsWindow(m_hWnd) )
		return;

	DWORD dwThreadId = ::GetCurrentThreadId();
	if( dwThreadId == ::GetWindowThreadProcessId(m_hWnd, NULL) ){
		if( bRedrawAtOnce ){
			for(int i=0; i<nCt; i++)
				Redraw(pRectsRedraw[i]);
			}
		else{
			for(int i=0; i<nCt; i++)
				InvalidateRect(pRectsRedraw[i], FALSE);
			}
		}
	else{
		/*
		CPtrArray*	pArrRects	= new CPtrArray();
		for(int i=0; i<nCt; i++){
			CRect*	pRect	= new CRect(pRectsRedraw[i]);
			if( pRect->IsRectEmpty() ){
				delete pRect;
				continue;
				}
			pArrRects->Add(pRect);
			}
		::PostMessage(m_hWnd, WM_REDRAW_RECT, bRedrawAtOnce, (LPARAM)pArrRects);
	*/
		
		/*
		m_lockRedrawOptimizer.Lock(); // Lock
		for(int i=0; i<nCt; i++){
			if( !pRectsRedraw[i].IsRectEmpty() ){
				CRect	rect		= pRectsRedraw[i];
				RECT* pRect			= new RECT;
				pRect->left			= rect.left;
				pRect->top			= rect.top;
				pRect->right		= rect.right;
				pRect->bottom		= rect.bottom;
				m_listRedrawRects.Add(pRect);
				}
			}
		
		if( !m_bRedrawOptimizeTimerIsOn )
			m_nRedrawOptimizeTimeId = SetTimer(20000, 10, RedrawOptimizeTimer);
		m_lockRedrawOptimizer.Unlock(); // Unlock
		*/

		m_lockRectCachingThread.Lock();		//Lock
		int nIndex = m_arrRectCachingInfoByThreadId.IndexOf((void*)dwThreadId);
		if( nIndex != -1 ){
			RedrawRectCachingStruct* pInfo = (RedrawRectCachingStruct*)m_arrRectCachingInfoByThreadId.GetData(nIndex);
			if( !pInfo )
				m_arrRectCachingInfoByThreadId.Delete(nIndex);
			else{
				for(int i=0; i<nCt; i++){
					if( !pRectsRedraw[i].IsRectEmpty() ){
						CRect	rect		= pRectsRedraw[i];
						RECT* pRect			= new RECT;
						pRect->left			= rect.left;
						pRect->top			= rect.top;
						pRect->right		= rect.right;
						pRect->bottom		= rect.bottom;
						pInfo->listCachedInvalidRects.Add(pRect);
						}
					}
				}
			}
		m_lockRectCachingThread.Unlock();	//Unlock
		}
	}

void
MercuryBaseView::ScrollWindow(int nScrollX, int nScrollY){
	CRect rcClient;
	GetClientRectMy(rcClient);
	if( rcClient.IsRectEmpty() )
		return;

	if( nScrollX != 0 ){
		CDC* pDC = GetDC();
		pDC->ScrollDC(nScrollX, 0, rcClient, rcClient, NULL, rcClient);
		ReleaseDC(pDC);
		}
	else
	if( nScrollY != 0 ){
		CDC* pDC = GetDC();
		pDC->ScrollDC(0, nScrollY, rcClient, rcClient, NULL, rcClient);
		ReleaseDC(pDC);

		if( nScrollY < 0 ){
			CRect rcInvalidate(rcClient.left, rcClient.bottom + nScrollY, rcClient.right, rcClient.bottom);
			RedrawRect(rcInvalidate, TRUE);
			}
		else{
			CRect rcInvalidate(rcClient.left, rcClient.top, rcClient.right, rcClient.top + nScrollY);
			RedrawRect(rcInvalidate, TRUE);
			}
		}
	}

BOOL 
MercuryBaseView::RegisterWndClass(LPCTSTR lpszClassName){
	WNDCLASS wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASS));

	HINSTANCE hInst = AfxGetInstanceHandle();
	if( !GetClassInfo(hInst, lpszClassName, &wndClass) ){
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hbrBackground	= (HBRUSH)NULL;
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon			= (HICON)NULL;
		wndClass.hInstance		= hInst;
		wndClass.lpfnWndProc	= ::DefWindowProc;
		wndClass.lpszClassName	= lpszClassName;
		wndClass.lpszMenuName	= NULL;
		wndClass.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
		return AfxRegisterClass(&wndClass);
		}
	return TRUE;
	}

bool
MercuryBaseView::SetChildControlTimer(ESChildControl* pChild, UINT nIdEvent, UINT nElapse){
	int nIndex = m_arrChildControlByTimer.IndexOf((void*)nIdEvent);
	if( nIndex != -1 ){
		m_arrChildControlByTimer.Delete(nIndex);
		::KillTimer(m_hWnd, nIdEvent);
		}

	if( !::IsWindow(m_hWnd) )
		return false;

	UINT nRet = SetTimer(nIdEvent, nElapse, NULL);
	if( nRet ){ // Success
		m_arrChildControlByTimer.Add((void*)nIdEvent, (void*)pChild);
		return true;
		}
	return false;
	}

bool
MercuryBaseView::KillChildControlTimer(ESChildControl* pChild, UINT nTimerId){
	int nIndex = m_arrChildControlByTimer.IndexOf((void*)nTimerId);
	if( nIndex != -1 ){
		m_arrChildControlByTimer.Delete(nIndex);
		::KillTimer(m_hWnd, nTimerId);
		return true;
		}
	return false;
	}

void 
MercuryBaseView::OnSetFocus(CWnd* pOldWnd){
	CView::OnSetFocus(pOldWnd);
	m_bActive = TRUE;
	/*
	// Redraw client area. #######
	CRect rcClient;
	GetClientRect(rcClient);
//	RedrawRect(rcClient, TRUE);
	// ###########################
	*/
	}

void 
MercuryBaseView::OnKillFocus(CWnd* pNewWnd){
	CView::OnKillFocus(pNewWnd);
	m_bActive = FALSE;
	/*
	// Redraw client area. #######
	CRect rcClient;
	GetClientRect(rcClient);
//	RedrawRect(rcClient, TRUE);
	// ###########################
	*/
	}

BOOL
MercuryBaseView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	BOOL bRet = CView::OnMouseWheel(nFlags, zDelta, pt);
	if( m_pControlUnderCursor )
		m_pControlUnderCursor->OnMouseWheel(nFlags, zDelta, pt);
	return bRet;
	}

void
MercuryBaseView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// Designer mode. {{
	if( m_bDesignerMode ){
		if( GetSelectedCt() > 0 ){
			switch( nChar ){
				case VK_DOWN:{
						MoveSelectedControls(0, 1);
						break;
					}
				case VK_UP:{
						MoveSelectedControls(0, -1);
						break;
					}
				case VK_LEFT:{
						MoveSelectedControls(-1, 0);
						break;
					}
				case VK_RIGHT:{
						MoveSelectedControls(1, 0);
						break;
					}
				case VK_DELETE:
				case VK_BACK:{
						RemoveSelectedControls();
					}
				};
			}

		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
		}
	// }}

	if( m_pChildFocused && m_pChildFocused->IsVisible() ){
		m_pChildFocused->OnKeyDown(nChar, nRepCnt, nFlags);
		}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

void
MercuryBaseView::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags){
	if( m_pChildFocused && m_pChildFocused->IsVisible() ){
		m_pChildFocused->OnChar(uChar, nRepCnt, nFlags);
		}
	CView::OnChar(uChar, nRepCnt, nFlags);
	}

void
MercuryBaseView::OnSize(UINT nType, int cx, int cy){
	if( cx > 0 && cy > 0 ){
		if( m_bAllowZooming ){
			m_fRatioCX = (cx / (float)m_szDefWindowSize.cx);
			m_fRatioCY = (cy / (float)m_szDefWindowSize.cy);
			}
		// Resize child controls.
		ResizeChildControls(cx, cy);
		}

	CView::OnSize(nType, cx, cy);
	}

void
MercuryBaseView::OnMouseMove(UINT nFlags, CPoint point){
	if( m_bDesignerMode ){
		// Sizing undermouse control.
		if( m_pControlUnderCursor && m_nSizingHitTest != -1 && (nFlags&MK_LBUTTON) ){
			CRect rcControlOld	= m_pControlUnderCursor->GetClientRect();
			CRect rcControlNew;
			m_bSizing			= true;

			switch( m_nSizingHitTest ){
				case ESChildControl::ChildControlHitTest::LeftCenter:{
					rcControlNew.left	= point.x;
					rcControlNew.top	= rcControlOld.top;
					rcControlNew.right	= rcControlOld.right;
					rcControlNew.bottom = rcControlOld.bottom;
					break;
					}
				case ESChildControl::ChildControlHitTest::RightCenter:{
					rcControlNew.left	= rcControlOld.left;
					rcControlNew.top	= rcControlOld.top;
					rcControlNew.right	= point.x;
					rcControlNew.bottom = rcControlOld.bottom;
					break;
					}
				case ESChildControl::ChildControlHitTest::MiddleTop:{
					rcControlNew.left	= rcControlOld.left;
					rcControlNew.top	= point.y;
					rcControlNew.right	= rcControlOld.right;
					rcControlNew.bottom = rcControlOld.bottom;
					break;
					}
				case ESChildControl::ChildControlHitTest::MiddleBottom:{
					rcControlNew.left	= rcControlOld.left;
					rcControlNew.top	= rcControlOld.top;
					rcControlNew.right	= rcControlOld.right;
					rcControlNew.bottom = point.y;
					break;
					}
				case ESChildControl::ChildControlHitTest::LeftTop:{
					rcControlNew.left	= point.x;
					rcControlNew.top	= point.y;
					rcControlNew.right	= rcControlOld.right;
					rcControlNew.bottom = rcControlOld.bottom;
					break;
					}
				case ESChildControl::ChildControlHitTest::RightBottom:{
					rcControlNew.left	= rcControlOld.left;
					rcControlNew.top	= rcControlOld.top;
					rcControlNew.right	= point.x;
					rcControlNew.bottom = point.y;
					break;
					}
				case ESChildControl::ChildControlHitTest::RightTop:{
					rcControlNew.left	= rcControlOld.left;
					rcControlNew.top	= point.y;
					rcControlNew.right	= point.x;
					rcControlNew.bottom = rcControlOld.bottom;
					break;
					}
				case ESChildControl::ChildControlHitTest::LeftBottom:{
					rcControlNew.left	= point.x;
					rcControlNew.top	= rcControlOld.top;
					rcControlNew.right	= rcControlOld.right;
					rcControlNew.bottom = point.y;
					break;
					}
				};

			if( rcControlNew != rcControlOld ){
				CRect rcClient;
				GetClientRect(rcClient);
				rcClient.SetRect(0, 0, rcClient.Width(), rcClient.Height());
				m_pControlUnderCursor->SetPos					(rcControlNew.left, rcControlNew.top, false);
				m_pControlUnderCursor->SetSize					(rcControlNew.Width(), rcControlNew.Height(), false);
				m_pControlUnderCursor->OwnerWindowSizeChanged	(rcClient.Width(), rcClient.Height());
				RedrawRect(rcControlNew|rcControlOld, 1);
				}

			CView::OnMouseMove(nFlags, point);
			return;
			}

		m_bSizing			= false;
		m_nSizingHitTest	= -1;

		// Move selected controls.
		if( (nFlags&MK_LBUTTON) ){
			m_bMoving		= true;
			int nOffsetX	= (point.x - m_ptMouseDown.x);
			int nOffsetY	= (point.y - m_ptMouseDown.y);
			if( MoveSelectedControls(nOffsetX, nOffsetY) ){
				m_ptMouseDown = point;
				}
			CView::OnMouseMove(nFlags, point);
			return;
			}

		m_bMoving = false;

//		m_pControlUnderCursor = ChildControlByPoint(point);
		m_pControlUnderCursor = m_layerMan.GetTopMostChildByPoint(point);
		CView::OnMouseMove(nFlags, point);
		return;
		}

	if( nFlags&MK_LBUTTON && m_pControlUnderCursor ){
		if( m_pControlUnderCursor->PtInClientArea(point) )
			m_pControlUnderCursor->OnMouseHover(point, nFlags);
		else{
			if( m_pControlUnderCursor->RemainCapturedOutOfClientArea() )
				m_pControlUnderCursor->OnMouseHover(point, nFlags);
			else{
				m_pControlUnderCursor->OnMouseLeave(nFlags);
				m_pControlUnderCursor = m_layerMan.GetTopMostChildByPoint(point);

				if( m_pControlCaptured ){
					ReleaseCapture();
					m_pControlCaptured = NULL;
					}
				}			
			}
		CView::OnMouseMove(nFlags, point);
		return;
		}

//	ESChildControl* pControl = ChildControlByPoint(point);
	ESChildControl* pControl = m_layerMan.GetTopMostChildByPoint(point);
	if( m_pControlUnderCursor != pControl ){
		if( pControl ){
			if( m_pControlUnderCursor )
				m_pControlUnderCursor->OnMouseLeave(nFlags);
			
			CString sTipText;
			if( pControl->GetTooltipText(sTipText) )
				ActivateTooltip(sTipText.GetBuffer());
			else
				DeactivateTooltip();
			
			m_pControlUnderCursor = pControl;
			m_pControlUnderCursor->OnMouseEnter(nFlags);
			}
		else{
			if( m_pControlUnderCursor )
				m_pControlUnderCursor->OnMouseLeave(nFlags);
			
			DeactivateTooltip();
			m_pControlUnderCursor = NULL;
			}
		CView::OnMouseMove(nFlags, point);
		return;
		}
	else{
		if( pControl ){
			if( m_bTooltipIsTracking ){
				CString sTipText;
				if( pControl->GetTooltipText(sTipText) )
					ActivateTooltip(sTipText.GetBuffer());
				}

			m_pControlUnderCursor->OnMouseHover(point, nFlags);
			CView::OnMouseMove(nFlags, point);
			return;
			}
		}
	CView::OnMouseMove(nFlags, point);
	}

BOOL
MercuryBaseView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	// Designer mode. {{
	if( m_bDesignerMode ){
		if( !m_pControlUnderCursor || !IsControlSelected(m_pControlUnderCursor) )
			return CView::OnSetCursor(pWnd, nHitTest, message);

		if( m_bMoving ){
			::SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			return 1;
			}

		int nHitTest = -1;
		if( !m_bSizing ){
			CPoint pt;
			::GetCursorPos(&pt);
			ScreenToClient(&pt);

			nHitTest			= m_pControlUnderCursor->SizeBoxHitTest(pt);
			m_nSizingHitTest	= nHitTest;
			}
		else
			nHitTest			= m_nSizingHitTest;

		if( nHitTest != -1 ){
			switch( nHitTest ){
				case ESChildControl::ChildControlHitTest::LeftCenter:
				case ESChildControl::ChildControlHitTest::RightCenter:
					{
						::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
						return 1;
					}
				case ESChildControl::ChildControlHitTest::MiddleTop:
				case ESChildControl::ChildControlHitTest::MiddleBottom:
					{
						::SetCursor(LoadCursor(NULL, IDC_SIZENS));
						return 1;
					}
				case ESChildControl::ChildControlHitTest::LeftTop:
				case ESChildControl::ChildControlHitTest::RightBottom:
					{
						::SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
						return 1;
					}
				case ESChildControl::ChildControlHitTest::RightTop:
				case ESChildControl::ChildControlHitTest::LeftBottom:
					{
						::SetCursor(LoadCursor(NULL, IDC_SIZENESW));
						return 1;
					}
				default:
					::SetCursor(LoadCursor(NULL, IDC_ARROW));
				};
			return CView::OnSetCursor(pWnd, nHitTest, message);
			}
		else
			return CView::OnSetCursor(pWnd, nHitTest, message);
		}
	// }}

	if( m_pControlUnderCursor ){
		BOOL bRet = m_pControlUnderCursor->OnSetCursor(pWnd, nHitTest, message);
		if( bRet )
			return 1;
		}
	return CView::OnSetCursor(pWnd, nHitTest, message);
	}

UINT 
MercuryBaseView::OnGetDlgCode(){
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	}

void
MercuryBaseView::OnLButtonDblClk(UINT nFlags, CPoint point){
	if( m_pControlUnderCursor )
		m_pControlUnderCursor->OnLButtonDblClick(point, nFlags);

	// Set control focus.
	if( m_pControlUnderCursor != m_pChildFocused ){
		if( m_pControlUnderCursor && m_pControlUnderCursor->SetFocus() ){
			ESChildControl* pFocusOld = m_pChildFocused;
			m_pChildFocused = m_pControlUnderCursor;
			if( pFocusOld )
				pFocusOld->KillFocus();
			}
		if( !m_pControlUnderCursor )
			m_pChildFocused = NULL;
		}
	CView::OnLButtonDblClk(nFlags, point);
	}

void
MercuryBaseView::OnLButtonDown(UINT nFlags, CPoint point){
//	CRect rcRedraw (point.x - 40, point.y - 40, point.x + 40, point.y + 40);
//	RedrawRect(rcRedraw, 1);
	// Designer mode. {{
	if( m_bDesignerMode ){
//		m_pControlUnderCursor	= ChildControlByPoint(point);
		m_pControlUnderCursor	= m_layerMan.GetTopMostChildByPoint(point);

		// MouseUpDown events allowed in designer mode.
		if( m_pControlUnderCursor && m_pControlUnderCursor->MouseUpDownEventsInDesigner() && IsControlSelected(m_pControlUnderCursor) ){
			if( m_pControlUnderCursor ){
				m_pControlCaptured = m_pControlUnderCursor;
				SetCapture();
				m_pControlUnderCursor->OnLButtonDown(point, nFlags);
				}

			m_ptMouseDown			= point;
			CView::OnLButtonDown(nFlags, point);
			return;
			}

		if( (nFlags&MK_CONTROL) != MK_CONTROL )
			ClearSelections(true);
		if( AddInSelectedList(m_pControlUnderCursor) )
			m_pControlUnderCursor->Redraw();

		m_ptMouseDown			= point;
		m_rcSelectionBoundDown	= m_rcSelectionBound;

		if( ::IsWindow(m_propertyDlg) && ::IsWindowVisible(m_propertyDlg) ){
			if( m_pControlUnderCursor )
				m_propertyDlg.SetSerializableObject(m_pControlUnderCursor);
			else
				if( !GetSelectedCt() )
					m_propertyDlg.SetSerializableObject(&m_info);
			}
			
		CView::OnLButtonDown(nFlags, point);
		return;
		}
	// }}

	if( m_pControlUnderCursor ){
		m_pControlCaptured = m_pControlUnderCursor;
		SetCapture();
		m_pControlUnderCursor->OnLButtonDown(point, nFlags);
		}
	else{
		if( !m_info.m_bTitleBar ){
			CRect rcClient;
			GetClientRect(rcClient);
			CRect rcCaption(0, 0, rcClient.right, m_info.m_rcOmitBorder.top);
			if( rcCaption.PtInRect(point) ){
				GetParent()->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELONG(point.x, point.y));
				}
			}
		}
	// Set control focus.
	SetChildFocus(m_pControlUnderCursor);

	CView::OnLButtonDown(nFlags, point);
	}

void
MercuryBaseView::OnRButtonUp(UINT nFlags, CPoint point){
	// Designer mode. {{
	if( m_bDesignerMode ){
		// Destroy old menu. {{
		if( ::IsMenu(m_menuContext) )
			::DestroyMenu(m_menuContext.Detach());
		// }}

		CMenu	newControl;
		CMenu	newChildControl;

		m_menuContext.CreatePopupMenu();
		int nSelectedCt = GetSelectedCt();
		if( nSelectedCt > 0 ){
			ESChildControl* pLastSel = GetSelectedControl(m_arrSelectedControlIds.GetCount() - 1);
			if( pLastSel && pLastSel->AllowToAddChildControl() ){
				
				newChildControl.CreatePopupMenu();
				newChildControl.AppendMenu(MF_STRING, MI_LABEL_AS_CHILD,				(LPCTSTR)_T("Label"));
				newChildControl.AppendMenu(MF_STRING, MI_IMAGE_AS_CHILD,				(LPCTSTR)_T("Image"));
				newChildControl.AppendMenu(MF_STRING, MI_IMAGEBUTTON_AS_CHILD,			(LPCTSTR)_T("ImageButton"));
				newChildControl.AppendMenu(MF_STRING, MI_TEXTBOX_AS_CHILD,				(LPCTSTR)_T("TextBox"));
				newChildControl.AppendMenu(MF_STRING, MI_CHECKBOX_AS_CHILD,				(LPCTSTR)_T("CheckBox"));
				newChildControl.AppendMenu(MF_STRING, MI_COMBOBOX_AS_CHILD,				(LPCTSTR)_T("ComboBox"));
				newChildControl.AppendMenu(MF_STRING, MI_SLIDER_AS_CHILD,				(LPCTSTR)_T("Slider"));
				newChildControl.AppendMenu(MF_STRING, MI_PROGRESSCTRL_AS_CHILD,			(LPCTSTR)_T("ProgressCtrl"));
				newChildControl.AppendMenu(MF_STRING, MI_AVATAR_AS_CHILD,				(LPCTSTR)_T("Avatar"));
				newChildControl.AppendMenu(MF_STRING, MI_SIMPLERENDERVIEW_AS_CHILD,		(LPCTSTR)_T("Simple Render View"));
				newChildControl.AppendMenu(MF_STRING, MI_LIST_AS_CHILD,					(LPCTSTR)_T("List"));
//				newChildControl.AppendMenu(MF_STRING, MI_TREE_AS_CHILD,					(LPCTSTR)_T("Tree"));
				newChildControl.AppendMenu(MF_STRING, MI_TAB_AS_CHILD,					(LPCTSTR)_T("Tab"));
				newChildControl.AppendMenu(MF_STRING, MI_TABVIEW_AS_CHILD,				(LPCTSTR)_T("TabView"));
//				newChildControl.AppendMenu(MF_STRING, MI_TREEVIEW_AS_CHILD,				(LPCTSTR)_T("TreeView"));
				newChildControl.AppendMenu(MF_STRING, MI_BANNER_AS_CHILD,				(LPCTSTR)_T("Banner"));
				newChildControl.AppendMenu(MF_STRING, MI_PROPERTYGRID_AS_CHILD,			(LPCTSTR)_T("PropertyGrid"));
				m_menuContext.AppendMenu(MF_POPUP, (UINT)newChildControl.m_hMenu,		(LPCTSTR)"Add Child");
				}

			m_menuContext.AppendMenu(MF_SEPARATOR, 0, _T(""));
			if( nSelectedCt > 1 ){
				m_menuContext.AppendMenu(MF_STRING, MI_ALIGN_LEFTS,						(LPCTSTR)_T("Align Lefts"));
				m_menuContext.AppendMenu(MF_STRING, MI_ALIGN_TOPS,						(LPCTSTR)_T("Align Tops"));
				m_menuContext.AppendMenu(MF_STRING, MI_ALIGN_RIGHTS,					(LPCTSTR)_T("Align Rights"));
				m_menuContext.AppendMenu(MF_STRING, MI_ALIGN_BOTTOMS,					(LPCTSTR)_T("Align Bottoms"));
				m_menuContext.AppendMenu(MF_STRING, MI_MAKE_SAME_SIZE_WIDTH,			(LPCTSTR)_T("Make Same Width"));
				m_menuContext.AppendMenu(MF_STRING, MI_MAKE_SAME_SIZE_HEIGHT,			(LPCTSTR)_T("Make Same Height"));
				m_menuContext.AppendMenu(MF_STRING, MI_MAKE_SAME_SIZE_BOTH,				(LPCTSTR)_T("Make Same Size Both"));
				}

			BOOL	bLock	= FALSE;
			int		nLoop	= 0;
			int		nCt		= m_arrSelectedControlIds.GetCount();
			while( nLoop < nCt ){
				ESChildControl* pControl = GetChildControlByName(m_arrSelectedControlIds.GetKey(nLoop).GetBuffer());
				if( pControl && !pControl->IsLocked() )
					bLock = TRUE;
				nLoop ++;
				}

			if( bLock )
				m_menuContext.AppendMenu(MF_STRING|MF_UNCHECKED, MI_LOCK,		(LPCTSTR)_T("Lock"));
			else
				m_menuContext.AppendMenu(MF_STRING|MF_CHECKED, MI_UNLOCK,		(LPCTSTR)_T("Unlock"));

			m_menuContext.AppendMenu(MF_STRING, MI_COPY,			(LPCTSTR)_T("Copy"));
			m_menuContext.AppendMenu(MF_STRING, MI_CUT,				(LPCTSTR)_T("Cut"));
			}
		else{
			newControl.CreatePopupMenu();
			newControl.AppendMenu(MF_STRING, MI_LABEL,			(LPCTSTR)_T("Label"));
			newControl.AppendMenu(MF_STRING, MI_IMAGE,			(LPCTSTR)_T("Image"));
			newControl.AppendMenu(MF_STRING, MI_IMAGEBUTTON,	(LPCTSTR)_T("ImageButton"));
			newControl.AppendMenu(MF_STRING, MI_TEXTBOX,		(LPCTSTR)_T("TextBox"));
			newControl.AppendMenu(MF_STRING, MI_CHECKBOX,		(LPCTSTR)_T("CheckBox"));
			newControl.AppendMenu(MF_STRING, MI_COMBOBOX,		(LPCTSTR)_T("ComboBox"));
			newControl.AppendMenu(MF_STRING, MI_SLIDER,			(LPCTSTR)_T("Slider"));
			newControl.AppendMenu(MF_STRING, MI_PROGRESSCTRL,	(LPCTSTR)_T("ProgressCtrl"));
			newControl.AppendMenu(MF_STRING, MI_AVATAR,			(LPCTSTR)_T("Avatar"));
			newControl.AppendMenu(MF_STRING, MI_SIMPLERENDERVIEW,(LPCTSTR)_T("Simple Render View"));
			newControl.AppendMenu(MF_STRING, MI_LIST,			(LPCTSTR)_T("List"));
//			newControl.AppendMenu(MF_STRING, MI_TREE,			(LPCTSTR)_T("Tree"));
			newControl.AppendMenu(MF_STRING, MI_TAB,			(LPCTSTR)_T("Tab"));
			newControl.AppendMenu(MF_STRING, MI_TABVIEW,		(LPCTSTR)_T("TabView"));
//			newControl.AppendMenu(MF_STRING, MI_TREEVIEW,		(LPCTSTR)_T("TreeView"));
			newControl.AppendMenu(MF_STRING, MI_BANNER,			(LPCTSTR)_T("Banner"));
			newControl.AppendMenu(MF_STRING, MI_PROPERTYGRID,	(LPCTSTR)_T("PropetyGrid"));
			m_menuContext.AppendMenu(MF_POPUP, (UINT)newControl.m_hMenu, (LPCTSTR)"Create New");
			}

		m_menuContext.AppendMenu(MF_STRING, MI_PASTE,					(LPCTSTR)_T("Paste"));
		m_menuContext.AppendMenu(MF_STRING, MI_OPEN_DESIGNER_FILE,		(LPCTSTR)_T("Open"));
		m_menuContext.AppendMenu(MF_STRING, MI_SAVE_DESIGNER_FILE,		(LPCTSTR)_T("Save"));

		m_menuContext.AppendMenu(MF_STRING, MI_LIST_OF_CONTROLS,		(LPCTSTR)_T("List of controls"));
		m_menuContext.AppendMenu(MF_STRING, MI_PROPERTIES,				(LPCTSTR)_T("Properties"));

		if( nSelectedCt > 0 )
			m_menuContext.AppendMenu(MF_STRING, MI_BASEVIEW_PROPERTIES,	(LPCTSTR)_T("Base View Properties"));

		DesignerContextMenu(&m_menuContext, &newControl, &newChildControl);

		CPoint ptScreen = point;
		ClientToScreen(&ptScreen);
		m_menuContext.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);

		CView::OnRButtonUp(nFlags, point);
		return;
		}
	// }}

	if( m_pControlUnderCursor )
		m_pControlUnderCursor->OnRButtonUp(point, nFlags);
	CView::OnRButtonUp(nFlags, point);
	}

void
MercuryBaseView::OnLButtonUp(UINT nFlags, CPoint point){
	m_bMoving			= false;
	m_bSizing			= false;
	m_nSizingHitTest	= -1;

	CView::OnLButtonUp(nFlags, point);

	if( m_pControlCaptured ){
		ReleaseCapture();
		m_pControlCaptured->OnLButtonUp(point, nFlags);
		m_pControlCaptured->OnLButtonClick(point, nFlags);
		m_pControlCaptured = NULL;
		}
	}

BOOL
MercuryBaseView::OnOK(){
	if( m_pChildFocused && m_pChildFocused->IsVisible() )
		return m_pChildFocused->OnOK();
	return FALSE;
	}

BOOL
MercuryBaseView::OnCancel(){
	if( m_pChildFocused && m_pChildFocused->IsVisible() )
		return m_pChildFocused->OnCancel();
	return FALSE;
	}

BOOL
MercuryBaseView::OnTabControl(){
	ESChildControl* pNextFocus = m_pChildFocused;
	if( !pNextFocus )
		pNextFocus = m_pFirstChild;
	else
		pNextFocus = pNextFocus->GetNext();
	SetChildFocus(pNextFocus, true);
	return 1;
	}

bool
MercuryBaseView::AddEventListener(CObject* pReceiver, int nControlId, ChildControlEventCode eventCode, ControlEventListener method){
	if( !pReceiver ) return false;
	ESChildControl* pChild = GetChildControl(nControlId);
	return AddEventListener(pReceiver, pChild, eventCode, method);
	}

bool
MercuryBaseView::AddEventListener(CObject* pReceiver, ESChildControl* pControl, ChildControlEventCode eventCode, ControlEventListener method){
	if( !pReceiver ) return false;
	ASSERT(pControl);
	if( pControl )
		return pControl->AddEventListener(pReceiver, eventCode, method);
	return false;
	}

bool
MercuryBaseView::AddEventListenerByName(CObject* pReceiver, TCHAR* pszName, ChildControlEventCode eventCode, ControlEventListener method){
	ESChildControl* pControl = GetChildControlByName(pszName);
	if( pControl )
		return AddEventListener(pReceiver, pControl, eventCode, method);
	return false;
	}

bool
MercuryBaseView::SetControlVisibility(int nControlId, bool bVisible, bool bRedraw){
	ESChildControl* pControl = GetChildControl(nControlId);
	if( pControl ){
		if( pControl->IsVisible() != bVisible ){
			pControl->SetVisible(bVisible, bRedraw);
			return true;
			}
		}
	return false;
	}

bool
MercuryBaseView::SetControlVisibilityByName(TCHAR* pszName, bool bVisible, bool bRedraw){
	ESChildControl* pControl = GetChildControlByName(pszName);
	if( pControl ){
		if( pControl->IsVisible() != bVisible ){
			pControl->SetVisible(bVisible, bRedraw);
			return true;
			}
		}
	return false;
	}

bool
MercuryBaseView::SetupCheckBoxByName(TCHAR* pszName, bool bVisible, bool bCheckState, bool bRedraw){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(pszName);
	if( pCheck ){
		CRect rcCheck = pCheck->GetClientRect();
		if( pCheck->IsVisible() != bVisible )
			pCheck->SetVisible(bVisible, false);
		pCheck->SetCheck(bCheckState, false);
		if( bRedraw )
			RedrawRect(rcCheck, 1);
		return true;
		}
	return false;
	}

bool
MercuryBaseView::GetCheckBoxStateByName(TCHAR* pszName){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(pszName);
	if( pCheck )
		return pCheck->GetCheck();
	return false;
	}

void
MercuryBaseView::DisableImageButton(int nButtonId, bool bEnable /*= false*/, bool bRedraw){
	ESChildControlImageButton* pButton = (ESChildControlImageButton*)GetChildControl(nButtonId);
	if( pButton ){
		if( bEnable )
			pButton->EnableButton(bRedraw);
		else
			pButton->DisableButton(bRedraw);
		}
	}

void
MercuryBaseView::DisableImageButtonByName(TCHAR* pszName, bool bEnable /*= false*/, bool bRedraw){
	ESChildControlImageButton* pButton = (ESChildControlImageButton*)GetChildControlByName(pszName);
	if( pButton ){
		if( bEnable )
			pButton->EnableButton(bRedraw);
		else
			pButton->DisableButton(bRedraw);
		}
	}

bool
MercuryBaseView::RedrawChildControl(int nId){
	ESChildControl* pControl = GetChildControl(nId);
	if( !pControl ) return false;
	pControl->Redraw();
	return true;
	}

bool
MercuryBaseView::RedrawChildControlByName(CString sName){
	ESChildControl* pControl = GetChildControlByName(sName.GetBuffer());
	if( !pControl ) return false;
	pControl->Redraw();
	return true;
	}

bool
MercuryBaseView::RemoveChildControlByName(CString sName){
	ESChildControl* pControl = GetChildControlByName(sName.GetBuffer());
	if( !pControl ) return false;
	RemoveChildControl(pControl);
	return true;
	}

bool
MercuryBaseView::StartCachingInvalidRects(){
	if( !IsWindow(m_hWnd) ) return false;
	DWORD dwThreadId = ::GetCurrentThreadId();
	if( dwThreadId == ::GetWindowThreadProcessId(m_hWnd, NULL) ){
		if( !m_bInvalidRectsCaching ){
			m_bInvalidRectsCaching = true;
			m_listCachedInvalidRects.RemoveAll(true);
			return true;
			}
		}
	else{
		m_lockRectCachingThread.Lock();		//Lock
		int nIndex = m_arrRectCachingInfoByThreadId.IndexOf((void*)dwThreadId);
		if( nIndex != -1 ){
			RedrawRectCachingStruct* pInfo = (RedrawRectCachingStruct*)m_arrRectCachingInfoByThreadId.GetData(nIndex);
			if( pInfo ){
				pInfo->listCachedInvalidRects.RemoveAll(true);
				pInfo->dwThreadId = dwThreadId;
				}
			else{
				RedrawRectCachingStruct* pInfo = new RedrawRectCachingStruct;
				pInfo->dwThreadId = dwThreadId;
				m_arrRectCachingInfoByThreadId.Add((void*)dwThreadId, (void*)pInfo);
				}
			}
		else{
			RedrawRectCachingStruct* pInfo = new RedrawRectCachingStruct;
			pInfo->dwThreadId = dwThreadId;
			m_arrRectCachingInfoByThreadId.Add((void*)dwThreadId, (void*)pInfo);
			}
		m_lockRectCachingThread.Unlock();	//Unlock
		return true;
		}
	return false;
	}

bool
MercuryBaseView::EndCachingInvalidRects(bool bSave){
	if( !IsWindow(m_hWnd) ) return false;
	DWORD dwThreadId = ::GetCurrentThreadId();
	if( GetWindowThreadProcessId(m_hWnd, NULL) == dwThreadId ){
		if( !m_bInvalidRectsCaching ) 
			return false;
		m_bInvalidRectsCaching = false;
		if( m_listCachedInvalidRects.GetCount() == 0 ) 
			return true;
		/*
		if( bSave ){
			CDC memDC;
			memDC.CreateCompatibleDC(NULL);

			CRect rcClient;
			GetClientRect(rcClient);

			CBitmap bmImage;
			ImageHelper::CreateDIBBitmap(32, bmImage, RGB(255, 255, 255), rcClient.Width(), rcClient.Height(), NULL);
			memDC.SelectObject(&bmImage);

			List<RECT>	listPartRects;
		
			CString s, s2;
			for(int i=0; i<m_listCachedInvalidRects.GetCount(); i++){
				RECT* pRect = m_listCachedInvalidRects.GetAt(i);
				rc1.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
				if( rc1.IsRectEmpty() ) continue;

				s.Format(_T("%d,%d,%d,%d\r\n"), pRect->left, pRect->top, pRect->right, pRect->bottom);
				s2 += s;
				TRACE3(_T("%d %d %d"), pRect->left, pRect->top, pRect->right); 

				s.Format(_T("%d"), i);
				memDC.FillSolidRect(pRect->left, pRect->top, rc1.Width(), rc1.Height(), RGB(20 + i*10, 0, 0));
				}

			memDC.SetTextColor(RGB(255, 255, 255));
			memDC.SetBkColor(RGB(0, 0, 0));
			for(int i=0; i<m_listCachedInvalidRects.GetCount(); i++){
				RECT* pRect = m_listCachedInvalidRects.GetAt(i);
				rc1.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
				if( rc1.IsRectEmpty() ) continue;

				s.Format(_T("%d (%d,%d,%d,%d)"), i, pRect->left, pRect->top, pRect->right, pRect->bottom);
				memDC.TextOut(pRect->left, pRect->top, s);
				}

			CPen pen(PS_DASHDOT, 1, RGB(0, 0, 30));
			memDC.SelectObject(&pen);
			CBrush br(RGB(255, 150, 255));
			memDC.SelectObject(&br);

			for(int i=0; i<listPartRects.GetCount(); i++){
				RECT* pRect = listPartRects.GetAt(i);
				rc1.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
				if( rc1.IsRectEmpty() ) continue;
				memDC.Rectangle(rc1);
				//memDC.FillSolidRect(rc1.left, rc1.top, rc1.Width(), rc1.Height(), RGB(255, 0, 255));
				//ImageHelper::AlphaColorOnBitmap(bmImage, rc1.left, rc1.top, rc1.Width(), rc1.Height(), RGB(0, 200, 0), 122);
				}

			DeleteFile(_T("d:\\caching_rects.bmp"));
			ImageHelper::SaveBitmap(_T("d:\\caching_rects.bmp"), bmImage, 32);

			bmImage.DeleteObject();
			memDC.DeleteDC();
			}
			*/

		ExcludeIntersectionAreas(&m_listCachedInvalidRects);
		CRect rcDraw;
		for(int i=0; i<m_listCachedInvalidRects.GetCount(); i++){
			RECT* pRect = m_listCachedInvalidRects.GetAt(i);
			rcDraw.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
			if( rcDraw.IsRectEmpty() ) continue;
			Redraw(rcDraw, NULL);
			}
		m_listCachedInvalidRects.RemoveAll(true);
		return true;
		}
	else{
		m_lockRectCachingThread.Lock();		// Lock
		int nIndex = m_arrRectCachingInfoByThreadId.IndexOf((void*)dwThreadId);
		if( nIndex != -1 ){
			RedrawRectCachingStruct* pInfo = (RedrawRectCachingStruct*)m_arrRectCachingInfoByThreadId.GetData(nIndex);
			if( pInfo ){
				CPtrArray*	pArrRects	= new CPtrArray();
				for(int i=0; i<pInfo->listCachedInvalidRects.GetCount(); i++){
					RECT*	pRect			= pInfo->listCachedInvalidRects[i];
					CRect*	pRectInvalid	= new CRect(pRect);
					pArrRects->Add(pRectInvalid);
					}
				pInfo->listCachedInvalidRects.RemoveAll(true);
				delete pInfo;
				m_arrRectCachingInfoByThreadId.Delete(nIndex);
				m_lockRectCachingThread.Unlock();	// Unlock
				::PostMessage(m_hWnd, WM_REDRAW_RECT, 1/*Redraw at once*/, (LPARAM)pArrRects);
				return true;
				}
			}
		m_lockRectCachingThread.Unlock();	// Unlock
		}
	return false;
	}

void
MercuryBaseView::ExcludeIntersectionAreas(List<RECT>* pArrRects){
	// Exclude intersections to speed up performance. {{
	CRect rc1, rc2, rc3;
	for(int i=0; i<pArrRects->GetCount(); i++){
		RECT* pRect1 = pArrRects->GetAt(i);
		rc1.SetRect(pRect1->left,pRect1->top,pRect1->right,pRect1->bottom);
		if( rc1.IsRectEmpty() ) continue;
		for(int j=i+1; j<pArrRects->GetCount(); j++){
			RECT* pRect2 = pArrRects->GetAt(j);
			rc2.SetRect(pRect2->left,pRect2->top,pRect2->right,pRect2->bottom);
			if( rc2.IsRectEmpty() ) continue;

			rc3 = rc1 & rc2;
			if( !rc3.IsRectEmpty() ){
				int		nSquareRect1		= rc1.Width()*rc1.Height();
				int		nSquareRect2		= rc2.Width()*rc2.Height();
				int		nSquareIntersection	= rc3.Width()*rc3.Height();

				// These are parts of over rects.
				CRect	rcLeft, rcTop, rcRight, rcBottom;
				CRect*	ppRects[]			= {&rcLeft, &rcTop, &rcRight, &rcBottom};
				bool	bRect1IsOverRect2	= false;
				bool	bRects[]			= {false, false, false, false};

				CRect	rcRectOver, rcRect;
				// Rect1 is over rect2
				if( (nSquareRect1 - nSquareIntersection) < (nSquareRect2 - nSquareIntersection) ){
					rcRectOver			= rc1;
					rcRect				= rc2;
					bRect1IsOverRect2	= true;
					}
				else{
					rcRectOver			= rc2;
					rcRect				= rc1;
					bRect1IsOverRect2	= false;
					}

				// Top left
				if( rcRect.PtInRect(CPoint(rcRectOver.left, rcRectOver.top)) ){
					rcTop	.SetRect(0, 0, 0, 0);
					rcLeft	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rcRectOver.top < rcRect.top ){
						rcTop.top		= rcRectOver.top;
						rcTop.bottom	= rcRect.top;
						rcTop.left		= rcRectOver.left;
						rcTop.right		= rcRectOver.right;
						bRects[1] = true; // Top rect
						}

					if( rcRectOver.left < rcRect.left ){
						rcLeft.left		= rcRectOver.left;
						rcLeft.right	= rcRect.left;
						rcLeft.top		= bRects[1] ? rcTop.bottom : rcRectOver.top;
						rcLeft.bottom	= rcRectOver.bottom;
						bRects[0] = true; // Left rect
						}
					}

				// top right
				if( rcRect.PtInRect(CPoint(rcRectOver.right, rcRectOver.top)) ){
					rcTop	.SetRect(0, 0, 0, 0);
					rcRight	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rcRectOver.top < rcRect.top ){
						rcTop.top		= rcRectOver.top;
						rcTop.bottom	= rcRect.top;
						rcTop.left		= rcRectOver.left;
						rcTop.right		= rcRectOver.right;
						bRects[1] = true; // Top rect
						}

					if( rcRectOver.right > rcRect.right ){
						rcRight.left	= rcRect.right;
						rcRight.right	= rcRectOver.right;
						rcRight.top		= bRects[1] ? rcTop.bottom : rcRectOver.top;
						rcRight.bottom	= rcRectOver.bottom;
						bRects[2] = true; // Right rect
						}
					}

				// bottom left
				if( rcRect.PtInRect(CPoint(rcRectOver.left, rcRectOver.bottom)) ){
					rcBottom.SetRect(0, 0, 0, 0);
					rcLeft	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rcRectOver.bottom > rcRect.bottom ){
						rcBottom.top	= rcRect.bottom;
						rcBottom.bottom	= rcRectOver.bottom;
						rcBottom.left	= rcRectOver.left;
						rcBottom.right	= rcRectOver.right;
						bRects[3] = true; // Bottom rect
						}

					if( rcRectOver.left < rcRect.left ){
						rcLeft.left		= rcRectOver.left;
						rcLeft.right	= rcRect.left;
						rcLeft.top		= rcRectOver.top;
						rcLeft.bottom	= bRects[3] ? rcBottom.top : rcRectOver.bottom;
						bRects[0] = true; // Left rect
						}
					}

				// bottom right
				if( rcRect.PtInRect(CPoint(rcRectOver.right, rcRectOver.bottom)) ){
					rcBottom.SetRect(0, 0, 0, 0);
					rcRight	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rcRectOver.bottom > rcRect.bottom ){
						rcBottom.top	= rcRect.bottom;
						rcBottom.bottom	= rcRectOver.bottom;
						rcBottom.left	= rcRectOver.left;
						rcBottom.right	= rcRectOver.right;
						bRects[3] = true; // Bottom rect
						}

					if( rcRectOver.right > rcRect.right ){
						rcRight.left	= rcRect.right;
						rcRight.right	= rcRectOver.right;
						rcRight.top		= rcRectOver.top;
						rcRight.bottom	= bRects[3] ? rcBottom.top : rcRectOver.bottom;
						bRects[2] = true; // Right rect
						}
					}


				for(int nLoop=0; nLoop<4; nLoop++){
					CRect* pRectPart = ppRects[nLoop];
					if( !pRectPart->IsRectEmpty() ){
						RECT* pRect		= new RECT;
						pRect->left		= pRectPart->left;
						pRect->top		= pRectPart->top;
						pRect->right	= pRectPart->right;
						pRect->bottom	= pRectPart->bottom;
						pArrRects->Add(pRect);
						}
					}

				if( bRect1IsOverRect2 ){
					pRect1->left = pRect1->top = pRect1->right = pRect1->bottom = 0;
					break;
					}
				else
					pRect2->left = pRect2->top = pRect2->right = pRect2->bottom = 0;
				}
			}
		}
	}

bool
MercuryBaseView::SetChildControlZIndex(ESChildControl* pChild, int nZIndex){
	ASSERT(pChild);
	if( !pChild ) return false;
	ESChildControl* pControl = GetChildControl(pChild->GetId());
	if( !pControl ) return false;

	int nZIndexOld = pControl->GetZIndex();
	m_listChildControlInfos.ModifyIndexVariable(_T("ZIndex"), pControl->m_pControlInfo, &nZIndex);
	pControl->SetZIndex(nZIndex);
	return true;
	}

bool
MercuryBaseView::SetChildControlName(ESChildControl* pChild, CString sName, bool& bExists){
	ASSERT(pChild);
	if( !pChild ) return false;
	ESChildControl* pControl = GetChildControl(pChild->GetId());
	if( !pControl ) return false;

	if( m_listChildControlInfos.FindOneVariable(_T("Name"), &sName) ){
		bExists = true;
		return false;
		}

	CString				sNameOld	= pControl->GetName();
	ESChildControlInfo* pInfo		= m_listChildControlInfos.FindOneVariable(_T("Name"), (void*)&sNameOld);
	if( !pInfo )
		return false;
	
	pControl->SetName(sName);
// There is no need to set variable ModifyIndexVariable method does it self.
//	pInfo->m_sName	= sName;
	m_listChildControlInfos.ModifyIndexVariable(_T("Name"), pInfo, (void*)&sName);
	return true;
	}

bool
MercuryBaseView::SetChildFocus(ESChildControl* pControl, bool bFindFocusAcceptable /*= false*/){
	// Set control focus.
	if( pControl != m_pChildFocused ){
		ESChildControl* pFocusOld	= m_pChildFocused;
		m_pChildFocused				= pControl;

		if( pControl ){
			if( m_pChildFocused->SetFocus() ){
				if( pFocusOld )
					pFocusOld->KillFocus();
				}
			else{
				// Find focus acceptable child control.
				if( bFindFocusAcceptable ){

					ESChildControl* pTemp		= pControl;
					ESChildControl* pFirstChild = pControl->GetParent() ? pControl->GetParent()->GetChild() : m_pFirstChild;

					// Loop till last child.
					while( pControl ){
						m_pChildFocused	= pControl;
						if( pControl->SetFocus() ){
							if( pFocusOld )
								pFocusOld->KillFocus();
							return true;
							}
						pControl = pControl->GetNext();
						}

					// Loop from first child.
					pControl = pFirstChild;
					while( pControl != pTemp ){
						m_pChildFocused	= pControl;
						if( pControl->SetFocus() ){
							if( pFocusOld )
								pFocusOld->KillFocus();
							return true;
							}
						pControl = pControl->GetNext();
						}
					}
				m_pChildFocused = pFocusOld;
				}
			}
		else{
			if( pFocusOld )
				pFocusOld->KillFocus();
			}
		return true;
		}
	return false;
	}

bool
MercuryBaseView::AddChildControlEx(ESChildControl* pChild, int nId, CRect rcControl, ESChildControl* pParent, CSize szDefWindowSize, CString sControlName /*= _T("")*/, int nZIndex /*= -1*/){
	float fPosX				= (rcControl.left / ((float)szDefWindowSize.cx));
	float fPosY				= (rcControl.top / ((float)szDefWindowSize.cy));
	float fCX				= rcControl.Width() / ((float)szDefWindowSize.cx);
	float fCY				= rcControl.Height() / ((float)szDefWindowSize.cy);

//	if( AddChildControl(pChild, nId, CRect(0, 0, 1, 1), pParent, nZIndex, sControlName) ){
	if( AddChildControl(pChild, nId, rcControl, pParent, nZIndex, sControlName) ){
		CRect rcPokerWnd;
		GetClientRect(rcPokerWnd);
//		pChild->SetVisible		(true, false);
		pChild->SetLogicalPos	(fPosX, fPosY);
		pChild->SetLogicalSize	(fCX, fCY);
		((ESChildControl*)pChild)->OwnerWindowSizeChanged(rcPokerWnd.Width(), rcPokerWnd.Height());
		return true;
		}
	return false;
	}

bool
MercuryBaseView::AddChildControl(ESChildControl* pControl, int nId, CRect rcControl, ESChildControl* pParent, int nZIndex /*= -1*/, CString sControlName /*= _T("")*/){
	if( !pControl) return false;
	if( nId < 1 )
		nId = MercuryBaseView::GetChildControlIdUnused(this);
			
	ESChildControlInfo* pInfo = m_listChildControlInfos.FindOneVariable(_T("Id"), &nId);
	if( pInfo ){
		TRACE(_T("MercuryBaseView::AddChildControl : control with that id already exists"));
		return false;
		}

	if( pParent )
		pParent->AddChild(pControl);
	else{
		if( !m_pFirstChild )
			m_pFirstChild = pControl;
		else{
			ESChildControl* last = m_pFirstChild;
			while( last->m_pNext )
				   last = last->m_pNext;
			last->m_pNext = pControl;
			}
		}

	ESChildControlInfo* pChildInfo	= new ESChildControlInfo();
	pChildInfo->m_nId				= nId;
	pChildInfo->m_rcRect			= rcControl;

	CString sUniqueName				= sControlName;
	bool	bSetUniqueName			= true;
	if( !sUniqueName.IsEmpty() ){
		if( !m_listChildControlInfos.FindOneVariable(_T("Name"), &sUniqueName) )
			bSetUniqueName			= false;
		}

	if( bSetUniqueName ){
		int		nLoop		= 1;
		CString	sDefName	= pControl->GetControlDefaultName();
		sUniqueName.Format(_T("%s%d"), sDefName, nLoop);
		while( m_listChildControlInfos.FindOneVariable(_T("Name"), &sUniqueName) ){
			nLoop ++;
			sUniqueName.Format(_T("%s%d"), sDefName, nLoop);
			}
		}
	
	pChildInfo->m_sName				= sUniqueName;
	pChildInfo->m_sClassName		= _T("");
	pChildInfo->m_pControl			= pControl;
	if( nZIndex < (pParent ? (pParent->m_nZIndex + 1) : 0) )
		pChildInfo->m_nZIndex	= pParent ? (pParent->m_nZIndex + 1) : 0;
	else
		pChildInfo->m_nZIndex	= nZIndex;

	pControl->SetZIndex	(pChildInfo->m_nZIndex);
	pControl->SetName	(pChildInfo->m_sName);
	m_listChildControlInfos	.Add(pChildInfo);
	m_listChildControls		.Add(pControl);

	// Create child control. {{
	if( !pControl->Create(nId, rcControl, this, pParent) ){
		TRACE(_T("MercuryBaseView::AddChildControl : fail during child control creation"));
		m_listChildControlInfos	.Remove(pChildInfo, true);
		m_listChildControls		.Remove(pControl, true);
		return false;
		}
	// }}

	/*
	// Set visibility change listener method..
	pControl->AddEventListener(this, ChildControlEventCode::EventCode_VisibilityChanged, (ControlEventListener)&MercuryBaseView::OnChildControlVisibilityChanged);
	// Set rect area change event listener.
	pControl->AddEventListener(this, ChildControlEventCode::EventCode_ClientRectChanged, (ControlEventListener)&MercuryBaseView::OnChildControlRectAreaChanged);
	*/
	// Add child control to layer.
	m_layerMan.Add(pControl);

	if( IsWindow(m_hWnd) ){
		CPoint pt;
		::GetCursorPos(&pt);
		ScreenToClient(&pt);
		::SendMessage(m_hWnd, WM_MOUSEMOVE, 0L, MAKELONG((short)pt.x, (short)pt.y));
		}
	return true;
	}

int
MercuryBaseView::GetChildControlIdUnused(MercuryBaseView* pBase){
	int nIdUnused = CHILD_ControlIdUnused;
	while( pBase->GetChildControl(nIdUnused) )
		nIdUnused ++;
	return nIdUnused;
	}

ESChildControl*
MercuryBaseView::GetChildControl(int nId){
	ESChildControlInfo* pInfo = m_listChildControlInfos.FindOneVariable(_T("Id"), &nId);
	if( !pInfo )
		return false;
	return pInfo->m_pControl;
	}

ESChildControl*
MercuryBaseView::GetChildControlByName(TCHAR* pszName){
	CString				sName	= pszName;
	ESChildControlInfo* pInfo	= m_listChildControlInfos.FindOneVariable(_T("Name"), &sName);
	if( !pInfo )
		return NULL;
	return pInfo->m_pControl;
	}

int
MercuryBaseView::GetChildControlIdByName(TCHAR* pszName){
	CString				sName	= pszName;
	ESChildControlInfo* pInfo	= m_listChildControlInfos.FindOneVariable(_T("Name"), &sName);
	if( !pInfo )
		return 0;
	return pInfo->m_nId;
	}

bool
MercuryBaseView::SetChildSize(int nId, int nWidth, int nHeight, bool bRedraw){
	ESChildControl* pChild = GetChildControl(nId);
	if( !pChild )
		return false;
	pChild->SetSize(nWidth, nHeight, bRedraw);
	return true;
	}

bool
MercuryBaseView::SetChildPos(int nId, int x, int y, bool bRedraw){
	ESChildControl* pChild = GetChildControl(nId);
	if( !pChild )
		return false;
	pChild->SetPos(x, y, bRedraw);
	return true;
	}

CRect
MercuryBaseView::GetChildRect(int nId){
	ESChildControl* pChild = GetChildControl(nId);
	if( !pChild )
		return CRect(0, 0, 0, 0);
	return pChild->GetClientRect();
	}

CPoint
MercuryBaseView::GetChildPos(int nId){
	ESChildControl* pChild = GetChildControl(nId);
	if( !pChild )
		return CPoint(0, 0);
	return pChild->GetClientPos();
	}

bool
MercuryBaseView::IsControlNameUsed(CString& sControlName){
	ESChildControlInfo* pInfo = m_listChildControlInfos.FindOneVariable(_T("Name"), &sControlName);
	return (pInfo == NULL);
	}

void
MercuryBaseView::ClearChildControls(bool bRedraw){
	ClearChildControl(m_pFirstChild);

	m_pChildFocused			= NULL;
	m_pFirstChild			= NULL;
	m_pControlUnderCursor	= NULL;

	if( m_pControlCaptured ){
		m_pControlCaptured	= NULL;
		ReleaseCapture();
		}

	m_layerMan					.RemoveAll();
	m_listChildControlInfos		.RemoveAll(true);
	m_listChildControls			.RemoveAll(false);
	m_arrChildControlByTimer	.DeleteAll();
	// Selected controls.
	m_arrSelectedControlIds		.DeleteAll();

	if( bRedraw )
		Invalidate(FALSE);
	}

void
MercuryBaseView::ClearChildControl(ESChildControl* pControl){
	if( !pControl ) return;
	while( pControl ){
		if( pControl->m_pChild )
			ClearChildControl(pControl->m_pChild);

		ESChildControl* pNext = pControl->m_pNext;
		if( pControl->IsCreated() )
			pControl->Destroy();
		delete pControl;
		pControl = pNext;
		}
	}

bool
MercuryBaseView::RemoveChildControl(ESChildControl* pControl, bool bRemoveAllSiblings){
	if( !pControl ) return false;

	m_lockChildControls.Lock(); // Lock
	ESChildControl* pParent		= pControl->m_pParent;
	ESChildControl* pChild		= pControl;
	ESChildControl* pNext		= NULL;
	int				nId			= 0;

	while( pChild ){
		if( pChild->m_pChild )
			RemoveChildControl(pChild->m_pChild, true);

		if( pChild == m_pFirstChild )
			m_pFirstChild = m_pFirstChild->m_pNext;

		if( pChild == m_pControlUnderCursor )
			m_pControlUnderCursor = NULL;

		if( pChild == m_pControlCaptured ){
			m_pControlCaptured = NULL;
			ReleaseCapture();
			}

		pChild->Destroy();

		ESChildControl* pTemp = pChild->m_pNext;
		// Remove control from arrays. {{
		nId	= pChild->GetId();
		ESChildControlInfo* pControlInfo = m_listChildControlInfos.FindOneVariable(_T("Id"), &nId);
		if( pControlInfo )
			m_listChildControlInfos.Remove(pControlInfo, true);
		if( m_pChildFocused == pChild ){
			// When focused child window dies someone should take focus from it. {{
			if( pTemp && pTemp->SetFocus() )
				m_pChildFocused = pTemp;
			else{
				ESChildControl* pPrev = pChild->GetPrev();
				if( pPrev && pPrev->SetFocus() )
					m_pChildFocused = pPrev;
				else
					m_pChildFocused = pChild->GetParent();
				}
			// }}
			}

		// Remove child control from layer.
		m_layerMan.Remove(pChild);
		// Destroy child control object.
		m_listChildControls.Remove(pChild);
		// }}

		if( !bRemoveAllSiblings ) {
			pNext = pTemp;
			break;
			}
		pChild = pTemp;
		}

	ESChildControl* pPrev = pParent ? pParent->m_pChild : m_pFirstChild;
	if( pPrev == pControl ){
		if( pParent )
			pParent->m_pChild = pNext;
		}
	else{
		if( pPrev ){
			while( pPrev->m_pNext ){
				if( pPrev->m_pNext == pControl ){
					pPrev->m_pNext = pNext;
					break;
					}
				pPrev = pPrev->m_pNext;
				}
			}
		}
	m_lockChildControls.Unlock(); // Unlock
	return true;
	}

bool
MercuryBaseView::DestroyAllChilds(){
	int nLoop = 0, nCt = m_listChildControls.GetCount();
	while( nLoop < nCt ){
		ESChildControl* pControl = m_listChildControls[nLoop];
		if( pControl )
			pControl->Destroy();
		nLoop ++;
		}

	// Remove all controls from layer.
	m_layerMan.RemoveAll();

	m_pChildFocused	= NULL;
	m_listChildControlInfos	.RemoveAll(true);
	m_listChildControls		.RemoveAll(true);
	return true;
	}

/*
ESChildControl*	
MercuryBaseView::ChildControlByPoint(CPoint pt, ESChildControl* pParent ){
	if( !m_pFirstChild ) return NULL;

	if( !pParent )
		pParent = m_pFirstChild;

	ESChildControl* pControl = pParent;
	while( pControl ){
		CRect rcControl = pControl->GetClientRect();
		if( pControl->IsVisible() && 
			//pControl->m_bAllowMouseEvents && 
			rcControl.PtInRect(pt) ){
			if( pControl->m_pChild ){
				ESChildControl* pChild = ChildControlByPoint(pt, pControl->m_pChild);
				if( pChild ) return pChild;
				}
			return pControl;
			}
		pControl = pControl->m_pNext;
		}
	return NULL;
	}*/
/*
void
MercuryBaseView::GetInvalidChildControls(ESChildControl* pChild, CRect* pInvalidClient, CDWordArray& arrInvalidControls){
	CRect rcControl;
	while( pChild ){
		if( pChild->m_bVisible ){
			rcControl	= pChild->GetClientRect();
			rcControl &= *pInvalidClient;
			if( !rcControl.IsRectEmpty() )
				arrInvalidControls.Add((long)pChild);
			// Call method recursevly.
			GetInvalidChildControls(pChild->m_pChild, pInvalidClient, arrInvalidControls);
			}
		pChild = pChild->m_pNext;
		}
	}*/

void
MercuryBaseView::RedrawChildControls(ESChildControl* pChild, CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	CRect rcControl, rcRectDC;
	CPtrArray arrControls;
	m_lockChildControls.Lock(); // Lock
	m_layerMan.EnumChildControlsByRect(*pRectClient, arrControls);

	if( arrControls.GetCount() > 0 ){
		int nLoop = 0, nCt = arrControls.GetCount();
		while( nLoop < nCt ){
			ESChildControl* pChild	= (ESChildControl*)arrControls.GetAt(nLoop);
			rcControl				= pChild->GetClientRect()&*pRectClient;
			rcRectDC.left			= (rcControl.left - pRectClient->left);
			rcRectDC.top			= (rcControl.top - pRectClient->top);
			rcRectDC.right			= (rcRectDC.left + rcControl.Width());
			rcRectDC.bottom			= (rcRectDC.top + rcControl.Height());
			pChild->OnPaintClient(pDC, &rcRectDC, &rcControl);
			nLoop ++;
			}
		}
	m_lockChildControls.Unlock(); // Unlock
	}

bool
MercuryBaseView::RedrawChildControl(ESChildControl* pChild, CRect rcRedraw){
	if( !pChild || !pChild->IsVisible() )
		return false;

	CRect rcChild = rcRedraw;
	if( rcChild.IsRectEmpty() )
		rcChild = pChild->GetClientRect();
	if( rcChild.IsRectEmpty() )
		return false;

	// Redraw only child control.
	Redraw(rcChild, pChild);
	return true;
	}

void
MercuryBaseView::ResizeChildControls(int cx, int cy){
	/*
	int	nLoop = 0, nCt = m_listChildControls.GetCount();
	while( nLoop < nCt ){
		ESChildControl* pControl = m_listChildControls[nLoop];
		if( pControl && pControl->IsCreated() )
			pControl->OwnerWindowSizeChanged(cx, cy);
		nLoop ++;
		}*/

	// Control resizes it's child controls.
	ESChildControl* pControl =  m_pFirstChild;
	while( pControl ){
		if( pControl && pControl->IsCreated() )
			pControl->OwnerWindowSizeChanged(cx, cy);
		pControl = pControl->GetNext();
		}

	// Update tool tip rect.
	UpdateTooltipRect(cx, cy);
	}

void
MercuryBaseView::OnChildVisibilityChanged(ESChildControl* pSender, bool bVisible){
	m_layerMan.ChildVisibilityChanged(pSender, bVisible);
	}

void
MercuryBaseView::OnChildRectAreaChanged(ESChildControl* pSender){
	m_layerMan.ChildSizeChanged(pSender);
	}

bool 
MercuryBaseView::CreateTrackingTooltip(){
	if( m_toolTipCtrl.m_hWnd ) 
		return false;
	if( !m_toolTipCtrl.Create(this, 0) )
		return false;
	CRect rcWindow;
	GetClientRect(rcWindow);
	m_toolTipCtrl.AddTool(this, _T(""), CRect(0, 0, rcWindow.Width(), rcWindow.Height()), 1000);
	m_toolTipCtrl.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW);
	m_toolTipCtrl.SetDelayTime(2000, 0);
	m_bTooltipIsTracking= false;
	return true;
	}

bool
MercuryBaseView::DeactivateTooltip(){
	if( m_toolTipCtrl.m_hWnd && m_bTooltipIsTracking ){
		m_toolTipCtrl.ActivateToolTip(false);
		m_bTooltipIsTracking = false;		
		return true;
		}
	return false;
	}

#include "..\Utility\StringHelper.h"
bool
MercuryBaseView::ActivateTooltip(TCHAR* pszTipText){
	if( !m_toolTipCtrl.m_hWnd ){
		if( !CreateTrackingTooltip() )
			return false;
		}

	CString sTip = pszTipText;
	CStringArray arrStrings;
	StringHelper::Split(&sTip, _T("\\r\\n"), arrStrings);
	sTip.Empty();
	for(int i=0; i<arrStrings.GetCount(); i++){
		sTip += arrStrings.GetAt(i);
		if( i != arrStrings.GetCount() -1 ){
			sTip += _T("\r\n");
			}
		}
	ESLocalizationMan::ParseText(sTip);
	m_sToolTipText = sTip;
	m_toolTipCtrl.UpdateTipText(m_sToolTipText, this, 1000);

	
	if( !m_bTooltipIsTracking ){
		m_toolTipCtrl.ActivateToolTip(true);
		m_bTooltipIsTracking = true;
		/*
		CPoint pt;
		GetCursorPos(&pt);
		// Position the tooltip. The coordinates are adjusted so that the tooltip does not overlap the mouse pointer.
		::SendMessage(m_toolTipCtrl.m_hWnd, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x + 10, pt.y - 20));
		*/
		}
	return true;
	}

bool
MercuryBaseView::UpdateTooltipRect(int cx, int cy){
	if( !m_toolTipCtrl.m_hWnd )
		return false;
	CRect rcBound(0, 0, cx, cy);
	//GetClientRect(rcBound);
//	m_toolTipCtrl.AdjustRect(rcBound);
	m_toolTipCtrl.SetToolRect(this, 1000, rcBound);
	return true;
	}