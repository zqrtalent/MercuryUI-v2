// MercuryBaseWnd.cpp : implementation file
//

#include "ESChildControl.h"
#include "MercuryBaseWnd.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"

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
// class ESChildControlAttribute
//
// ####################################################
ESChildControlAttribute::ESChildControlAttribute(){
	m_nId			= 0;
	m_bVisible		= false;
	m_pControl		= NULL;
	}

BEGIN_DECLARE_VAR(ESChildControlAttribute, Serializable)
	DECLARE_VAR(_T("ESChildControlAttribute"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("ClassName"),					_T(""), VariableType::VT_String, offsetof(ESChildControlAttribute,m_sClassName), false, false, false)
	DECLARE_VAR(_T("Name"),							_T(""), VariableType::VT_String, offsetof(ESChildControlAttribute,m_sName), false, false, false)
	DECLARE_VAR(_T("Id"),							_T(""), VariableType::VT_Int32, offsetof(ESChildControlAttribute,m_nId), false, false, false)
	DECLARE_VAR(_T("Visibility"),					_T(""), VariableType::VT_Bool, offsetof(ESChildControlAttribute,m_bVisible), false, false, false)
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
	DECLARE_VAR(_T("ESDesignFileInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("VersionMajor"),			_T(""), VariableType::VT_Int32, offsetof(ESDesignFileInfo,m_nVersionMajor), false, false, false)
	DECLARE_VAR(_T("VersionMinor"),			_T(""), VariableType::VT_Int32, offsetof(ESDesignFileInfo,m_nVersionMinor), false, false, false)
	DECLARE_VAR(_T("CreationDateTime"),		_T(""), VariableType::VT_DateTime, offsetof(ESDesignFileInfo,m_dtCreationDateTime), false, false, false)
	DECLARE_VAR(_T("Author"),				_T(""), VariableType::VT_String, offsetof(ESDesignFileInfo,m_sAuthor), false, false, false)
	DECLARE_VAR(_T("Description"),			_T(""), VariableType::VT_String, offsetof(ESDesignFileInfo,m_sDescription), false, false, false)
END_DECLARE_VAR()

ESDesignFileInfo::~ESDesignFileInfo(){
	}

// ####################################################
//
// class MercuryBaseView
//
// ####################################################

MercuryBaseView::MercuryBaseView() : ESFrameBase(), m_layerMan(this) {
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
//	m_bTooltipIsTracking	= false;
    
    /*
     int nFullSize = sizeof(EnumerableObject<ESChildControlInfo>);
     int intSize = sizeof(int);
     int valSize = sizeof(ESChildControlInfo*);
     int valPtrSize = sizeof(ESChildControlInfo**);
     int indexManSize = sizeof(EnumerableObject_IndexExtension*);
     */
    
    /*
    int									m_nCount;
    VALUE**								m_objects;
    VALUE*								m_pValueObject; // Needed to create object for deserialization.
    EnumerableObject_IndexExtension*	m_pIndexMan;
*/
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

	m_propertyDlg				= NULL;
	m_listOfControlsDlg			= NULL;
	}

MercuryBaseView::~MercuryBaseView(){
	//if( m_pRowSet && m_pRowSet->GetOwner() == this ){
	//	delete m_pRowSet;
	//	m_pRowSet = NULL;
	//	}
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
    /*
	if( m_bDesignerMode ){
		UINT	nFormatId	= ::RegisterClipboardFormat(_T("ES_BINARY"));
		HGLOBAL hglbCopy	= ::GetClipboardData(nFormatId);

		if( hglbCopy ){
			GlobalFree(hglbCopy);
			SetClipboardData(nFormatId, NULL);
			}
		}*/

//	if( m_toolTipCtrl.m_hWnd )
//		m_toolTipCtrl.DestroyWindow();

	// Free double buffer image objects. {{
    /*
	if( m_hDoubleBufferImage ){
		::DeleteObject(m_hDoubleBufferImage);
		m_hDoubleBufferImage = 0;
		}
	memset(&m_bmDoubleBufferImage, sizeof(BITMAP), 0);*/
	// }}

	// Destroy child controls.
	DestroyAllChilds();
	
	//// Destroy property dialog.
	//if( ::IsWindow(m_propertyDlg) )
	//	::DestroyWindow(m_propertyDlg);

	//// Destroy list of controls dialog.
	//if( ::IsWindow(m_listOfControlsDlg) )
	//	::DestroyWindow(m_listOfControlsDlg);

	if( m_propertyDlg ){
		delete m_propertyDlg;
		m_propertyDlg = NULL;
		}

	if( m_listOfControlsDlg ){
		delete m_listOfControlsDlg;
		m_listOfControlsDlg = NULL;
		}

	// Delete list of controls object.
	if( m_pListOfControls ){
		m_pListOfControls->RemoveAll(true);
		delete m_pListOfControls;
		}
	}

void 
MercuryBaseView::OnPaint(DCDef dc, _Rect rcRedraw) {
	_Rect		rcClient, rcClipBox;
	GetClientRectMy	(rcClient);
	if( rcClient.IsRectEmpty() )
		return;

    /*
	// Retrieve clipbox. ####################
	int nResult = pDC->GetClipBox(rcClipBox);
	if( rcClipBox.IsRectEmpty() )
		rcClipBox = rcClient;
	else
		rcClipBox &= rcClient;

	if( nResult == COMPLEXREGION )
		rcClipBox = rcClient;
*/
    
    if( rcRedraw.IsRectEmpty() )
        rcRedraw = rcClient;
    else
        rcRedraw &= rcClient;
    
    rcClipBox = rcRedraw;
//	float fZoom =  !UseDoubleBufferedDC() ? 1.0f : GetZoomFactor();
	float fZoom =  1.0f;
	_Rect rRectNonZoom	(0, 0, rcClient.Width(), rcClient.Height());
	_Rect rRectZoom		(0, 0, rRectNonZoom.Width()*fZoom, rRectNonZoom.Height()*fZoom);

	// Call PrePaint event.
	OnPrePaint(rcClipBox);

	if( rcClipBox.IsRectEmpty() ) 
		return;
	// #####################################

	Redraw(dc, rcClipBox);
	}

BOOL 
MercuryBaseView::OnEraseBkgnd(_DC *pDC){
	return TRUE;
	}

void
MercuryBaseView::OnPaintClient(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	RedrawChildControls(m_pFirstChild, pDC, pRectDC, pRectClient);
	}

bool
MercuryBaseView::Redraw(DCDef dc, _Rect rcRedraw, ESChildControl* pChildOnly /*= NULL*/){
	_Rect	rcClipBox	= rcRedraw, rcClient;
	GetClientRectMy(rcClient);
	rcClient.SetRect(0, 0, rcClient.Width(), rcClient.Height());
	if( rcClient.IsRectNull() ) return false;

	if( UseDoubleBufferedDC() == FALSE && m_bUseForegroundColor == FALSE ){
		_DC		dc;
		dc.Attach(GetDC());
		_DC*	pDC	= &dc;

		OnPaintClient	(pDC, &rcClipBox, &rcClipBox);
		ReleaseDC		(*pDC);
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
    
    _DC memDC;
    memDC.CreateMemoryBitmapDC(32, rcClipBox.Width(), rcClipBox.Height());
    
    /*
    _Rect rcFill1(0, 0, 200, 200);
    _Rect rcFill2(25, 25, 75, 75);
    
    //memDC.CreateMemoryBitmapDC(32, 200, 200);
    memDC.FillSolidRect(rcFill1, ARGB(0, 255, 255, 255));
    memDC.FillSolidRect(rcFill2, ARGB(200, 200, 100, 255));
    
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    strcpy(lf.lfFaceName, "Zapfino");
    lf.lfHeight = 14;
    lf.lfItalic = 1;
    
    Font font;
    font.CreateFontIndirect(&lf);
    memDC.SelectObject(&font);
    memDC.SetTextColor(ARGB(0, 0, 0, 255));
    
    
    NSString* drawStr = @"სუპ ბოი სუპ ბოი სუპ ბოი სუპ ბოი სუპ ბოი სუპ ბოი";
    memDC.ExtTextOut(0, 0, 0, _Rect(0, 0, 200, 200), [drawStr cStringUsingEncoding:NSUTF8StringEncoding]);
*/
	// Call virtual function 'OnPaintClient'.
	_Rect rRectDC(0, 0, rcClipBox.Width(), rcClipBox.Height());
	// Draw background first.
	DrawBackground(&memDC, &rRectDC, &rcClipBox, rcClient, m_info.m_bgFillMode.GetCurrentValue(), m_info.m_crFill, &m_info.m_bgImage, m_info.m_rcOmitBorder, true);
	
	if( pChildOnly ){ // Redraw only one child control.
		_Rect rcControl	= pChildOnly->GetClientRect() & rcClipBox;
		if( !rcControl.IsRectEmpty() && pChildOnly->m_bVisible ){
			rRectDC.left	= (rcControl.left - rcClipBox.left);
			rRectDC.top	= (rcControl.top - rcClipBox.top);
			rRectDC.right	= (rRectDC.left + rcControl.Width());
			rRectDC.bottom = (rRectDC.top + rcControl.Height());
			pChildOnly->OnPaintClient(&memDC, &rRectDC, &rcControl);
			}
		}
	else// Redraw all by rect region.
		OnPaintClient(&memDC, &rRectDC, &rcClipBox);
	// ######################################
	
    _DC::DrawDC(dc, rcClipBox.left, rcClipBox.top, rcClipBox.Width(), rcClipBox.Height(), &memDC, 0, 0, 0);
    // Win
	//pDC->BitBlt(rcClipBox.left, rcClipBox.top, rcClipBox.Width(), rcClipBox.Height(), memDC, 0, 0, SRCCOPY);
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
	//ReleaseDC		(*pDC);
	//dc.Detach();
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
	m_arrNamedParameters.Delete(lpszParam);
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
MercuryBaseView::PtInImageRegion(_Point ptInOwnerCoords, ESChildControl* pChild, RGNDef imageRgn, _Size szImage, bool bCentered){
	if( !pChild || !imageRgn ) return false;
	MercuryBaseView* pOwner = pChild->GetOwner();
	if( !pOwner ) 
		return false;

	float fZoomCX = 1.0f, fZoomCY = 1.0f;
	pOwner->GetZoom(fZoomCX, fZoomCY);
	_Rect	rcRgnImage, rcClient = pChild->GetClientRect();

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
#ifdef _WINDOWS
	_Point	ptClient((ptInOwnerCoords.x - rcRgnImage.left)/fZoomCX, (ptInOwnerCoords.y - rcRgnImage.top)/fZoomCY);
	if( ::PtInRegion(imageRgn, ptClient.x, ptClient.y) )
		return true;
#endif
	return false;
	}

bool
MercuryBaseView::DrawBackground(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient, _Rect rcControl, int nMode, COLORREF crFill, Image* pImageBg, 
								_Rect rcOmitBorder, bool bSizeBox /*= false*/, int nScaleMode /*= 1*/){
	MercuryGUI* pMerGUI		= MercuryGUI::GetInstance();
	_Rect		rcBackgroundDC;
	rcBackgroundDC.left		= (rcControl.left - pRectClient->left) + pRectDC->left; 
	rcBackgroundDC.top		= (rcControl.top - pRectClient->top) + pRectDC->top; 
	rcBackgroundDC.right	= rcBackgroundDC.left + rcControl.Width(); 
	rcBackgroundDC.bottom	= rcBackgroundDC.top + rcControl.Height();

	_Rect		rcInvalid	= rcBackgroundDC & *pRectDC;
	if( rcInvalid.IsRectEmpty() )
		return false;

	int		nFillMode		= nMode;
	bool	bRet			= false;

	switch( nFillMode ){
		case FillMode::FillMode_Color:{
			bRet = true;
			pDC->FillSolidRect(rcInvalid, crFill);
			break;
			}
		case FillMode::FillMode_StretchImage:{
			if( pImageBg && !pImageBg->IsNull()){
				bRet = true;
				pImageBg->RenderImage(pDC, rcInvalid, rcBackgroundDC, true, (pImageBg->GetBPP() == 32), (ScaleMode)nScaleMode);
				}
			break;
			}
		case FillMode::FillMode_OmitBorderedImage:{
			if( pImageBg && !pImageBg->IsNull()){
				bRet = true;
				pImageBg->RenderImage_OmitBorder(pDC, rcInvalid, rcBackgroundDC, rcOmitBorder, (pImageBg->GetBPP() == 32));
				}
			break;
			}
		case FillMode::FillMode_RepeatedImage:{
			if( pImageBg && !pImageBg->IsNull() ){
				bRet = true;
				pImageBg->RenderImage(pDC, rcInvalid, rcBackgroundDC, false, false);
				}
			break;
			}
		case FillMode::FillMode_CentredImage:{
			if( pImageBg && !pImageBg->IsNull() ){
				_Rect rImageCenter;
				rImageCenter.left		= rcControl.left + (rcControl.Width() - pImageBg->GetWidth())/2;
				rImageCenter.top		= rcControl.top + (rcControl.Height() - pImageBg->GetHeight())/2;
				rImageCenter.right		= rImageCenter.left + pImageBg->GetWidth();
				rImageCenter.bottom		= rImageCenter.top + pImageBg->GetHeight();

				_Rect rImageCenterDC;
				rImageCenterDC.left		= (rImageCenter.left - pRectClient->left) + pRectDC->left; 
				rImageCenterDC.top		= (rImageCenter.top - pRectClient->top) + pRectDC->top;
				rImageCenterDC.right	= rImageCenterDC.left + pImageBg->GetWidth();
				rImageCenterDC.bottom	= rImageCenterDC.top + pImageBg->GetHeight();

				rcInvalid = *pRectDC&rImageCenterDC;
				if( !rcInvalid.IsRectEmpty() ){
					pImageBg->RenderImage(pDC, rcInvalid, rImageCenterDC, false, (pImageBg->GetBPP() == 32));
					bRet = true;
					}
				}
			break;
			}
		};

	// Draw sizebox image. {{
	Image* pImageSizeBox = &m_info.m_sizeBoxImage;
	if( bSizeBox && m_info.m_szMax != m_info.m_szMin && !pImageSizeBox->IsNull() ){
		_Rect rImage;
		rImage.left		= rcControl.right - pImageSizeBox->GetWidth();
		rImage.top			= rcControl.bottom - pImageSizeBox->GetHeight();
		rImage.right		= rImage.left + pImageSizeBox->GetWidth();
		rImage.bottom		= rImage.top + pImageSizeBox->GetHeight();

		_Rect rImageDC;
		rImageDC.left		= (rImage.left - pRectClient->left) + pRectDC->left; 
		rImageDC.top		= (rImage.top - pRectClient->top) + pRectDC->top;
		rImageDC.right		= rImageDC.left + pImageSizeBox->GetWidth();
		rImageDC.bottom	= rImageDC.top + pImageSizeBox->GetHeight();

		rcInvalid = *pRectDC&rImageDC;
		if( !rcInvalid.IsRectEmpty() )
			pImageSizeBox->RenderImage(pDC, rcInvalid, rImageDC, false,  (pImageSizeBox->GetBPP() == 32));
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

bool
MercuryBaseView::AddToolTip(ESChildControl* pOwner, _string sToolText){
	if( !pOwner || !sToolText.length() ) 
		return false;
	/*
	if( !m_pToolTipControl ){
		m_pToolTipControl = new CPPToolTip();
		m_pToolTipControl->Create	(this, TRUE);
		m_pToolTipControl->SetNotify();
		}*/
	// Localize string.
	ESLocalizationMan::ParseText(sToolText);
	
//	m_pToolTipControl->AddTool(this, _T("<center><h2>ToolTip in ScrollView</h2><br><hr color=blue></center><br>ToolTip for <font color=red>RED</font> rectangle in View"), pOwner->GetClientRect(), pOwner->GetId());
//	m_pToolTipControl->AddTool(this, sToolText.c_str(), pOwner->GetClientRect(), pOwner->GetId());
	return true;
	}

bool
MercuryBaseView::RemoveToolTip(ESChildControl* pOwner){
	/*if( !m_pToolTipControl || !pOwner || !IsWindow(m_pToolTipControl->GetSafeHwnd()) )
		return false;
	m_pToolTipControl->DelTool(this, pOwner->GetId());*/
	return true;
	}

bool
MercuryBaseView::UpdateToolTipRect(ESChildControl* pOwner, _Rect rcNew){
	/*
	if( !m_pToolTipControl || !pOwner || !IsWindow(m_pToolTipControl->GetSafeHwnd()) )
		return false;
	m_pToolTipControl->SetToolRect(this, pOwner->GetId(), &rcNew);*/
	return true;
	}

bool
MercuryBaseView::UpdateToolTipText(ESChildControl* pOwner, _string sToolText){
	/*
	if( !m_pToolTipControl || !pOwner || !IsWindow(m_pToolTipControl->GetSafeHwnd()) )
		return false;
		// Localize string.
	ESLocalizationMan::ParseText(sToolText);
	m_pToolTipControl->UpdateTipText(sToolText.GetBuffer(), this, pOwner->GetId());
	*/
	return true;
	}

void
MercuryBaseView::OnTimer(UINT_PTR nIDEvent){
	ESFrameBase::OnTimer(nIDEvent);

	int			nIndex		= m_arrChildControlByTimer.IndexOf((void*)nIDEvent);
	LRESULT		lRet		= 0;
	if( nIndex != -1 ){
		ESChildControl* pChild = (ESChildControl*)m_arrChildControlByTimer.GetData(nIndex);
		if( pChild )
			pChild->OnTimer((UINT)nIDEvent);
		}
	}

void
MercuryBaseView::OnRedrawRect(CPtrArray* pArrRects, BOOL bAtOnce){
	ESFrameBase::OnRedrawRect(pArrRects, bAtOnce);

	int	nLoop = 0;
	while( nLoop < pArrRects->size() ){
		_Rect* pRect = (_Rect*)pArrRects->at(nLoop);
		if( pRect ){
			RedrawRect(*pRect, bAtOnce);
			delete pRect;
			}
		nLoop ++;
		}
	delete pArrRects;
	}

void
MercuryBaseView::RedrawRect(_Rect rcRedraw, BOOL bRedrawAtOnce /*= FALSE*/){
	if( rcRedraw.top < 0 )
		rcRedraw.top = 0;
	if( rcRedraw.left < 0 )
		rcRedraw.left = 0;

	if( rcRedraw.Width() <= 0 || rcRedraw.Height() <= 0 || !IsWindow() )
		return;
	//DWORD dwThreadId = ::GetCurrentThreadId();
	if( /*dwThreadId == ::GetWindowThreadProcessId(GetHWND(), NULL)*/TRUE ){
        if( bRedrawAtOnce )
			Redraw((CGContextRef)[[NSGraphicsContext currentContext] graphicsPort], rcRedraw);
		else
			InvalidateRect(rcRedraw, FALSE);
		}
	else{
		/*
		CPtrArray*	pArrRects	= new CPtrArray();
		_Rect*		pRectBound	= new _Rect(rcRedraw);
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

        
        /*
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
			_Rect*		pRectBound	= new _Rect(rcRedraw);
			pArrRects->push_back(pRectBound);
            
			::PostMessage(GetHWND(), WM_REDRAW_RECT, bRedrawAtOnce, (LPARAM)pArrRects);
			}*/
		}
	}

void
MercuryBaseView::RedrawMultipleRects(_Rect* pRectsRedraw, int nCt, BOOL bRedrawAtOnce /*= FALSE*/){
	if( !pRectsRedraw || nCt <= 0 || !IsWindow() )
		return;

	//DWORD dwThreadId = ::GetCurrentThreadId();
	if( /*dwThreadId == ::GetWindowThreadProcessId(GetHWND(), NULL)*/TRUE ){
		if( bRedrawAtOnce ){
            CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
			for(int i=0; i<nCt; i++)
				Redraw(context, pRectsRedraw[i]);
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
			_Rect*	pRect	= new _Rect(pRectsRedraw[i]);
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
				_Rect	rect		= pRectsRedraw[i];
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

        
        /*
		m_lockRectCachingThread.Lock();		//Lock
		int nIndex = m_arrRectCachingInfoByThreadId.IndexOf((void*)dwThreadId);
		if( nIndex != -1 ){
			RedrawRectCachingStruct* pInfo = (RedrawRectCachingStruct*)m_arrRectCachingInfoByThreadId.GetData(nIndex);
			if( !pInfo )
				m_arrRectCachingInfoByThreadId.Delete(nIndex);
			else{
				for(int i=0; i<nCt; i++){
					if( !pRectsRedraw[i].IsRectEmpty() ){
						_Rect	rect		= pRectsRedraw[i];
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
         */
		}
	}

void
MercuryBaseView::ScrollWindow(int nScrollX, int nScrollY){
	_Rect rcClient;
	GetClientRectMy(rcClient);
	if( rcClient.IsRectEmpty() )
		return;

	if( nScrollX != 0 ){
		DCDef pDC = GetDC();
		//pDC->ScrollDC(nScrollX, 0, rcClient, rcClient, NULL, rcClient);
		ReleaseDC(pDC);
		}
	else
	if( nScrollY != 0 ){
		DCDef pDC = GetDC();
		//pDC->ScrollDC(0, nScrollY, rcClient, rcClient, NULL, rcClient);
		ReleaseDC(pDC);

		if( nScrollY < 0 ){
			_Rect rcInvalidate(rcClient.left, rcClient.bottom + nScrollY, rcClient.right, rcClient.bottom);
			RedrawRect(rcInvalidate, TRUE);
			}
		else{
			_Rect rcInvalidate(rcClient.left, rcClient.top, rcClient.right, rcClient.top + nScrollY);
			RedrawRect(rcInvalidate, TRUE);
			}
		}
	}

bool
MercuryBaseView::SetChildControlTimer(ESChildControl* pChild, UINT nIdEvent, UINT nElapse){
    /*
	int nIndex = m_arrChildControlByTimer.IndexOf((void*)nIdEvent);
	if( nIndex != -1 ){
		m_arrChildControlByTimer.Delete(nIndex);
		::KillTimer(GetHWND(), nIdEvent);
		}

	if( !::IsWindow(GetHWND()) )
		return false;

	UINT nRet = ::SetTimer(GetHWND(), nIdEvent, nElapse, NULL);
	if( nRet ){ // Success
		m_arrChildControlByTimer.Add((void*)nIdEvent, (void*)pChild);
		return true;
		}*/
	return false;
	}

bool
MercuryBaseView::KillChildControlTimer(ESChildControl* pChild, UINT nTimerId){
    /*
	int nIndex = m_arrChildControlByTimer.IndexOf((void*)nTimerId);
	if( nIndex != -1 ){
		m_arrChildControlByTimer.Delete(nIndex);
		::KillTimer(m_hWnd, nTimerId);
		return true;
		}*/
	return false;
	}

void 
MercuryBaseView::OnSetFocus(HWND hWndFocusLost){
	ESFrameBase::OnSetFocus(hWndFocusLost);
	m_bActive = TRUE;
	}

void 
MercuryBaseView::OnKillFocus(HWND hWndGainFocus){
	ESFrameBase::OnKillFocus(hWndGainFocus);
	m_bActive = FALSE;
	
	// Redraw client area. #######
	//_Rect rcClient;
	//GetClientRect(rcClient);
	//RedrawRect(rcClient, TRUE);
	// ###########################
	}

BOOL
MercuryBaseView::OnMouseWheel(UINT nFlags, short zDelta, _Point pt){
	ESFrameBase::OnMouseWheel(nFlags, zDelta, pt);
	/*
	UINT	nFlags = GET_KEYSTATE_WPARAM(wParam);
	short	zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	_Point	pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	*/
	if( m_pControlUnderCursor )
		m_pControlUnderCursor->OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
	}

BOOL
MercuryBaseView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	ESFrameBase::OnKeyDown(nChar, nRepCnt, nFlags);

    /*
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
		return 1;
		}
	// }}
*/
	if( m_pChildFocused && m_pChildFocused->IsVisible() ){
		m_pChildFocused->OnKeyDown(nChar, nRepCnt, nFlags);
		}
	return 1;
	}

BOOL
MercuryBaseView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags){
	ESFrameBase::OnChar(nChar, nRepCnt, nFlags);

	if( m_pChildFocused && m_pChildFocused->IsVisible() )
		m_pChildFocused->OnChar(nChar, nRepCnt, nFlags);
	return TRUE;
	}

void
MercuryBaseView::OnSize(UINT nType, int cx, int cy){
	ESFrameBase::OnSize(nType, cx, cy);

	//if( nType == SIZE_MINIMIZED )
	//	return;

	if( cx > 0 && cy > 0 ){
		if( m_bAllowZooming ){
			m_fRatioCX = (cx / (float)m_szDefWindowSize.cx);
			m_fRatioCY = (cy / (float)m_szDefWindowSize.cy);
			}
		// Resize child controls.
		ResizeChildControls(cx, cy);
		// Redraw child controls.
		//Redraw((CGContextRef)[[NSGraphicsContext currentContext] graphicsPort], _Rect(0, 0, cx, cy));
		}
	}

void
MercuryBaseView::OnMouseMove(UINT nFlags, _Point point){
	ESFrameBase::OnMouseMove(nFlags, point);

	if( m_bDesignerMode ){
		// Sizing undermouse control.
		if( m_pControlUnderCursor && m_nSizingHitTest != -1 && (nFlags&MK_LBUTTON) ){
			_Rect rcControlOld	= m_pControlUnderCursor->GetClientRect();
			_Rect rcControlNew;
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
				_Rect rcClient;
				GetClientRectMy(rcClient);
				rcClient.SetRect(0, 0, rcClient.Width(), rcClient.Height());
				m_pControlUnderCursor->SetPos					(rcControlNew.left, rcControlNew.top, false);
				m_pControlUnderCursor->SetSize					(rcControlNew.Width(), rcControlNew.Height(), false);
				m_pControlUnderCursor->OwnerWindowSizeChanged	(rcClient.Width(), rcClient.Height());
				RedrawRect(rcControlNew|rcControlOld, 1);
				}
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
			return;
			}

		m_bMoving = false;

//		m_pControlUnderCursor = ChildControlByPoint(point);
		m_pControlUnderCursor = m_layerMan.GetTopMostChildByPoint(point);
		//CView::OnMouseMove(nFlags, point);
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
					//ReleaseCapture();
					m_pControlCaptured = NULL;
					}
				}			
			}
		return;
		}

//	ESChildControl* pControl = ChildControlByPoint(point);
	ESChildControl* pControl = m_layerMan.GetTopMostChildByPoint(point);
	if( m_pControlUnderCursor != pControl ){
		if( pControl ){
			if( m_pControlUnderCursor )
				m_pControlUnderCursor->OnMouseLeave(nFlags);
			
			_string sTipText;
			if( pControl->GetTooltipText(sTipText) )
				ActivateTooltip(sTipText.c_str());
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
		//CView::OnMouseMove(nFlags, point);
		return;
		}
	else{
		if( pControl ){
			/*if( m_bTooltipIsTracking ){
				_string sTipText;
				if( pControl->GetTooltipText(sTipText) )
					ActivateTooltip(sTipText.c_str());
				}*/

			m_pControlUnderCursor->OnMouseHover(point, nFlags);
			//CView::OnMouseMove(nFlags, point);
			return;
			}
		}
	//CView::OnMouseMove(nFlags, point);
	}

BOOL
MercuryBaseView::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message){
    /*
	// Designer mode. {{
	if( m_bDesignerMode ){
		if( !m_pControlUnderCursor || !IsControlSelected(m_pControlUnderCursor) )
			return 0;

		if( m_bMoving ){
			::SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			return 1;
			}

		int nHitTest = -1;
		if( !m_bSizing ){
			_Point pt;
			::GetCursorPos(&pt);
			::ScreenToClient(m_hWnd, &pt);

			nHitTest			= m_pControlUnderCursor->_SizeBoxHitTest(pt);
			m_nSizingHitTest	= nHitTest;
			}
		else
			nHitTest			= m_nSizingHitTest;

		if( nHitTest != -1 ){
			switch( nHitTest ){
				case ESChildControl::ChildControlHitTest::LeftCenter:
				case ESChildControl::ChildControlHitTest::RightCenter:{
					::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					return 1;
					}
				case ESChildControl::ChildControlHitTest::MiddleTop:
				case ESChildControl::ChildControlHitTest::MiddleBottom:{
					::SetCursor(LoadCursor(NULL, IDC_SIZENS));
					return 1;
					}
				case ESChildControl::ChildControlHitTest::LeftTop:
				case ESChildControl::ChildControlHitTest::RightBottom:{
					::SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
					return 1;
					}
				case ESChildControl::ChildControlHitTest::RightTop:
				case ESChildControl::ChildControlHitTest::LeftBottom:{
					::SetCursor(LoadCursor(NULL, IDC_SIZENESW));
					return 1;
					}
				default:
					::SetCursor(LoadCursor(NULL, IDC_ARROW));
				};
			return 1;
			}
		else
			return 1;
		}
	// }}

	if( m_pControlUnderCursor ){
		BOOL bRet = m_pControlUnderCursor->OnSetCursor(hWnd, nHitTest, message);
		if( bRet )
			return 1;
		}
*/
	return ESFrameBase::OnSetCursor(hWnd, nHitTest, message);
	}

void
MercuryBaseView::OnLButtonDblClk(UINT nFlags, _Point point){
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

	ESFrameBase::OnLButtonDblClk(nFlags, point);
	}

void
MercuryBaseView::OnLButtonDown(UINT nFlags, _Point point){
//	_Rect rcRedraw (point.x - 40, point.y - 40, point.x + 40, point.y + 40);
//	RedrawRect(rcRedraw, 1);
    
    /*
	// Designer mode. {{
	if( m_bDesignerMode ){
//		m_pControlUnderCursor	= ChildControlByPoint(point);
		m_pControlUnderCursor	= m_layerMan.GetTopMostChildByPoint(point);

		// MouseUpDown events allowed in designer mode.
		if( m_pControlUnderCursor && m_pControlUnderCursor->MouseUpDownEventsInDesigner() && IsControlSelected(m_pControlUnderCursor) ){
			if( m_pControlUnderCursor ){
				m_pControlCaptured = m_pControlUnderCursor;
				::SetCapture(m_hWnd);
				m_pControlUnderCursor->OnLButtonDown(point, nFlags);
				}

			m_ptMouseDown = point;
			ESFrameBase::OnLButtonDown(nFlags, point);
			return;
			}

		if( (nFlags&MK_CONTROL) != MK_CONTROL )
			ClearSelections(true);
		if( AddInSelectedList(m_pControlUnderCursor) )
			m_pControlUnderCursor->Redraw();

		m_ptMouseDown			= point;
		m_rcSelectionBoundDown	= m_rcSelectionBound;

		if( m_propertyDlg && m_propertyDlg->IsWindow() && m_propertyDlg->IsWindowVisible() ){
			if( m_pControlUnderCursor )
				m_propertyDlg->SetSerializableObject(m_pControlUnderCursor);
			else
				if( !GetSelectedCt() )
					m_propertyDlg->SetSerializableObject(&m_info);
			}
			
		ESFrameBase::OnLButtonDown(nFlags, point);
		return;
		}
	// }}

	if( m_pControlUnderCursor ){
		m_pControlCaptured = m_pControlUnderCursor;
		::SetCapture(m_hWnd);
		m_pControlUnderCursor->OnLButtonDown(point, nFlags);
		}
	else{
		if( !m_info.m_bTitleBar ){
			_Rect rcClient;
			GetClientRectMy(rcClient);
			_Rect rcCaption(0, 0, rcClient.right, m_info.m_rcOmitBorder.top);
			if( rcCaption.PtInRect(point) ){
				::SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELONG(point.x, point.y));
				}
			}
		}
	// Set control focus.
	SetChildFocus(m_pControlUnderCursor);
*/
	ESFrameBase::OnLButtonDown(nFlags, point);
	}

void
MercuryBaseView::OnRButtonUp(UINT nFlags, _Point point){
	/*
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
				m_menuContext.AppendMenu(MF_STRING, MI_MAKE_SAME_SIZE_BOTH,				(LPCTSTR)_T("Make Same _Size Both"));
				}

			BOOL	bLock	= FALSE;
			int		nLoop	= 0, nCt = m_arrSelectedControlIds.GetCount();
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

		_Point ptScreen = point;
		::ClientToScreen(GetHWND(), &ptScreen);
		m_menuContext.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, m_pBaseWnd);

		ESFrameBase::OnRButtonUp(nFlags, point);
		return;
		}
	// }}

	if( m_pControlUnderCursor )
		m_pControlUnderCursor->OnRButtonUp(point, nFlags);
	*/
	ESFrameBase::OnRButtonUp(nFlags, point);
	}

void
MercuryBaseView::OnLButtonUp(UINT nFlags, _Point point){
	m_bMoving			= false;
	m_bSizing			= false;
	m_nSizingHitTest	= -1;

	ESFrameBase::OnLButtonUp(nFlags, point);

	if( m_pControlCaptured ){
		//ReleaseCapture();
		m_pControlCaptured->OnLButtonUp		(point, nFlags);
		m_pControlCaptured->OnLButtonClick	(point, nFlags);
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

void
MercuryBaseView::SetTitle(){
	//if( IsWindow() )
		//::SetWindowText(m_hWnd, m_info.m_sTitle.c_str());
	}

void
MercuryBaseView::Setup(){
	ESFrameBase::Setup();


//	m_fHeightAndWidthRatio = 0.689f;
//	LoadFromDesignFile(_T("design\\table_base.des"), false, _T(""), false, m_szDefault, m_ptSetupPos.x, m_ptSetupPos.y);
	//LoadFromDesignFile(_T("design\\table_base.des"), false, _T(""), false, m_szDefault, m_ptSetupPos.x, m_ptSetupPos.y);
	}

bool
MercuryBaseView::AddEventListener(ESFrameBase* pReceiver, int nControlId, ChildControlEventCode eventCode, ControlEventListener method){
	if( !pReceiver ) return false;
	ESChildControl* pChild = GetChildControl(nControlId);
	return AddEventListener(pReceiver, pChild, eventCode, method);
	}

bool
MercuryBaseView::AddEventListener(ESFrameBase* pReceiver, ESChildControl* pControl, ChildControlEventCode eventCode, ControlEventListener method){
	if( !pReceiver ) return false;
	ASSERT(pControl);
	if( pControl )
		return pControl->AddEventListener(pReceiver, eventCode, method);
	return false;
	}

bool
MercuryBaseView::AddEventListenerByName(ESFrameBase* pReceiver, const TCHAR* pszName, ChildControlEventCode eventCode, ControlEventListener method){
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
MercuryBaseView::SetControlVisibilityByName(const TCHAR* pszName, bool bVisible, bool bRedraw){
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
MercuryBaseView::SetupCheckBoxByName(const TCHAR* pszName, bool bVisible, bool bCheckState, bool bRedraw){
	ESChildControlCheckBox* pCheck = (ESChildControlCheckBox*)GetChildControlByName(pszName);
	if( pCheck ){
		_Rect rcCheck = pCheck->GetClientRect();
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
MercuryBaseView::GetCheckBoxStateByName(const TCHAR* pszName){
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
MercuryBaseView::RedrawChildControlByName(const TCHAR* pszName){
	ESChildControl* pControl = GetChildControlByName(pszName);
	if( !pControl ) return false;
	pControl->Redraw();
	return true;
	}

bool
MercuryBaseView::RemoveChildControlByName(const TCHAR* pszName){
	ESChildControl* pControl = GetChildControlByName(pszName);
	if( !pControl ) return false;
	RemoveChildControl(pControl);
	return true;
	}

bool
MercuryBaseView::StartCachingInvalidRects(){
    /*
	if( !IsWindow() ) return false;
	DWORD dwThreadId = ::GetCurrentThreadId();
	if( dwThreadId == ::GetWindowThreadProcessId(GetHWND(), NULL) ){
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
		}*/
	return false;
	}

bool
MercuryBaseView::EndCachingInvalidRects(bool bSave){
    /*
	if( !IsWindow() ) 
		return false;
	DWORD dwThreadId = ::GetCurrentThreadId();
	if( GetWindowThreadProcessId(GetHWND(), NULL) == dwThreadId ){
		if( !m_bInvalidRectsCaching ) 
			return false;
		m_bInvalidRectsCaching = false;
		if( m_listCachedInvalidRects.GetCount() == 0 ) 
			return true;
		ExcludeIntersectionAreas(&m_listCachedInvalidRects);
		_Rect rcDraw;
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
					_Rect*	pRectInvalid	= new _Rect(pRect);
					pArrRects->push_back(pRectInvalid);
					}
				pInfo->listCachedInvalidRects.RemoveAll(true);
				delete pInfo;
				m_arrRectCachingInfoByThreadId.Delete(nIndex);
				m_lockRectCachingThread.Unlock();	// Unlock
				::PostMessage(m_hWnd, WM_REDRAW_RECT, 1, (LPARAM)pArrRects);
				return true;
				}
			}
		m_lockRectCachingThread.Unlock();	// Unlock
		}*/
	return false;
	}

void
MercuryBaseView::ExcludeIntersectionAreas(List<RECT>* pArrRects){
	// Exclude intersections to speed up performance. {{
	_Rect rc1, rc2, rc3;
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
				_Rect	rcLeft, rcTop, rcRight, rcBottom;
				_Rect*	ppRects[]			= {&rcLeft, &rcTop, &rcRight, &rcBottom};
				bool	bRect1IsOverRect2	= false;
				bool	bRects[]			= {false, false, false, false};

				_Rect	rRectOver, rRect;
				// _Rect1 is over rect2
				if( (nSquareRect1 - nSquareIntersection) < (nSquareRect2 - nSquareIntersection) ){
					rRectOver			= rc1;
					rRect				= rc2;
					bRect1IsOverRect2	= true;
					}
				else{
					rRectOver			= rc2;
					rRect				= rc1;
					bRect1IsOverRect2	= false;
					}

				// Top left
				if( rRect.PtInRect(_Point(rRectOver.left, rRectOver.top)) ){
					rcTop	.SetRect(0, 0, 0, 0);
					rcLeft	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rRectOver.top < rRect.top ){
						rcTop.top		= rRectOver.top;
						rcTop.bottom	= rRect.top;
						rcTop.left		= rRectOver.left;
						rcTop.right		= rRectOver.right;
						bRects[1] = true; // Top rect
						}

					if( rRectOver.left < rRect.left ){
						rcLeft.left		= rRectOver.left;
						rcLeft.right	= rRect.left;
						rcLeft.top		= bRects[1] ? rcTop.bottom : rRectOver.top;
						rcLeft.bottom	= rRectOver.bottom;
						bRects[0] = true; // Left rect
						}
					}

				// top right
				if( rRect.PtInRect(_Point(rRectOver.right, rRectOver.top)) ){
					rcTop	.SetRect(0, 0, 0, 0);
					rcRight	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rRectOver.top < rRect.top ){
						rcTop.top		= rRectOver.top;
						rcTop.bottom	= rRect.top;
						rcTop.left		= rRectOver.left;
						rcTop.right		= rRectOver.right;
						bRects[1] = true; // Top rect
						}

					if( rRectOver.right > rRect.right ){
						rcRight.left	= rRect.right;
						rcRight.right	= rRectOver.right;
						rcRight.top		= bRects[1] ? rcTop.bottom : rRectOver.top;
						rcRight.bottom	= rRectOver.bottom;
						bRects[2] = true; // Right rect
						}
					}

				// bottom left
				if( rRect.PtInRect(_Point(rRectOver.left, rRectOver.bottom)) ){
					rcBottom.SetRect(0, 0, 0, 0);
					rcLeft	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rRectOver.bottom > rRect.bottom ){
						rcBottom.top	= rRect.bottom;
						rcBottom.bottom	= rRectOver.bottom;
						rcBottom.left	= rRectOver.left;
						rcBottom.right	= rRectOver.right;
						bRects[3] = true; // Bottom rect
						}

					if( rRectOver.left < rRect.left ){
						rcLeft.left		= rRectOver.left;
						rcLeft.right	= rRect.left;
						rcLeft.top		= rRectOver.top;
						rcLeft.bottom	= bRects[3] ? rcBottom.top : rRectOver.bottom;
						bRects[0] = true; // Left rect
						}
					}

				// bottom right
				if( rRect.PtInRect(_Point(rRectOver.right, rRectOver.bottom)) ){
					rcBottom.SetRect(0, 0, 0, 0);
					rcRight	.SetRect(0, 0, 0, 0);
					}
				else{
					if( rRectOver.bottom > rRect.bottom ){
						rcBottom.top	= rRect.bottom;
						rcBottom.bottom	= rRectOver.bottom;
						rcBottom.left	= rRectOver.left;
						rcBottom.right	= rRectOver.right;
						bRects[3] = true; // Bottom rect
						}

					if( rRectOver.right > rRect.right ){
						rcRight.left	= rRect.right;
						rcRight.right	= rRectOver.right;
						rcRight.top		= rRectOver.top;
						rcRight.bottom	= bRects[3] ? rcBottom.top : rRectOver.bottom;
						bRects[2] = true; // Right rect
						}
					}


				for(int nLoop=0; nLoop<4; nLoop++){
					_Rect* pRectPart = ppRects[nLoop];
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
MercuryBaseView::SetChildControlName(ESChildControl* pChild, _string sName, bool& bExists){
	ASSERT(pChild);
	if( !pChild ) return false;
	ESChildControl* pControl = GetChildControl(pChild->GetId());
	if( !pControl ) return false;

	if( m_listChildControlInfos.FindOneVariable(_T("Name"), &sName) ){
		bExists = true;
		return false;
		}

	_string				sNameOld	= pControl->GetName();
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
MercuryBaseView::AddChildControlEx(ESChildControl* pChild, int nId, _Rect rcControl, ESChildControl* pParent, _Size szDefWindowSize, _string sControlName /*= _T("")*/, int nZIndex /*= -1*/){
	float fPosX	= (rcControl.left / ((float)szDefWindowSize.cx));
	float fPosY	= (rcControl.top / ((float)szDefWindowSize.cy));
	float fCX	= rcControl.Width() / ((float)szDefWindowSize.cx);
	float fCY	= rcControl.Height() / ((float)szDefWindowSize.cy);

//	if( AddChildControl(pChild, nId, _Rect(0, 0, 1, 1), pParent, nZIndex, sControlName) ){
	if( AddChildControl(pChild, nId, rcControl, pParent, nZIndex, sControlName) ){
		_Rect rcPokerWnd;
		GetClientRectMy(rcPokerWnd);
//		pChild->SetVisible		(true, false);
		pChild->SetLogicalPos	(fPosX, fPosY);
		pChild->SetLogicalSize	(fCX, fCY);
		((ESChildControl*)pChild)->OwnerWindowSizeChanged(rcPokerWnd.Width(), rcPokerWnd.Height());
		return true;
		}
	return false;
	}

bool
MercuryBaseView::AddChildControl(ESChildControl* pControl, int nId, _Rect rcControl, ESChildControl* pParent, int nZIndex /*= -1*/, _string sControlName /*= _T("")*/){
	if( !pControl) return false;
	if( nId < 1 )
		nId = MercuryBaseView::GetChildControlIdUnused(this);
			
	ESChildControlInfo* pInfo = m_listChildControlInfos.FindOneVariable(_T("Id"), &nId);
	if( pInfo ){
		//TRACE(_T("MercuryBaseView::AddChildControl : control with that id already exists"));
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
	pChildInfo->m_rRect			= rcControl;

	_string sUniqueName				= sControlName;
	bool	bSetUniqueName			= true;
	if( sUniqueName.length() ){
		if( !m_listChildControlInfos.FindOneVariable(_T("Name"), &sUniqueName) )
			bSetUniqueName			= false;
		}

	if( bSetUniqueName ){
		int		nLoop		= 1;
		_string	sDefName	= pControl->GetControlDefaultName();
		stringEx::Format(sUniqueName, _T("%s%d"), sDefName.c_str(), nLoop);
		while( m_listChildControlInfos.FindOneVariable(_T("Name"), &sUniqueName) ){
			nLoop ++;
			stringEx::Format(sUniqueName, _T("%s%d"), sDefName.c_str(), nLoop);
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
		//TRACE(_T("MercuryBaseView::AddChildControl : fail during child control creation"));
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

    /*
	if( IsWindow() ){
		_Point pt;
		::GetCursorPos	(&pt);
		::ScreenToClient(m_hWnd, &pt);
		::SendMessage	(GetHWND(), WM_MOUSEMOVE, 0L, MAKELONG((short)pt.x, (short)pt.y));
		}*/
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
		return NULL;
	return pInfo->m_pControl;
	}

ESChildControl*
MercuryBaseView::GetChildControlByName(const TCHAR* pszName){
	_string				sName	= pszName;
	ESChildControlInfo* pInfo	= m_listChildControlInfos.FindOneVariable(_T("Name"), &sName);
	if( !pInfo )
		return NULL;
	return pInfo->m_pControl;
	}

int
MercuryBaseView::GetChildControlIdByName(const TCHAR* pszName){
	_string				sName	= pszName;
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

_Rect
MercuryBaseView::GetChildRect(int nId){
	ESChildControl* pChild = GetChildControl(nId);
	if( !pChild )
		return _Rect(0, 0, 0, 0);
	return pChild->GetClientRect();
	}

_Point
MercuryBaseView::GetChildPos(int nId){
	ESChildControl* pChild = GetChildControl(nId);
	if( !pChild )
		return _Point(0, 0);
	return pChild->GetClientPos();
	}

bool
MercuryBaseView::IsControlNameUsed(_string& sControlName){
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
		//ReleaseCapture();
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
			//ReleaseCapture();
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

void
MercuryBaseView::RedrawChildControls(ESChildControl* pChild, _DC *pDC, _Rect* pRectDC, _Rect* pRectClient){
	_Rect rcControl, rRectDC;
	CPtrArray arrControls;
	m_lockChildControls.Lock(); // Lock
	m_layerMan.EnumChildControlsByRect(*pRectClient, arrControls);
	// Causes problems when removing child control !!!
	//LeaveCriticalSection(&m_lockChildControls); // Unlock

	if( arrControls.size() > 0 ){
		int nLoop = 0, nCt = (int)arrControls.size();
		while( nLoop < nCt ){
			ESChildControl* pChild	= (ESChildControl*)arrControls.at(nLoop);
			rcControl				= pChild->GetClientRect()&*pRectClient;
			rRectDC.left			= (rcControl.left - pRectClient->left);
			rRectDC.top				= (rcControl.top - pRectClient->top);
			rRectDC.right			= (rRectDC.left + rcControl.Width());
			rRectDC.bottom			= (rRectDC.top + rcControl.Height());
			pChild->OnPaintClient(pDC, &rRectDC, &rcControl);
			nLoop ++;
			}
		}
	m_lockChildControls.Unlock(); // Unlock
	}

bool
MercuryBaseView::RedrawChildControl(ESChildControl* pChild, _Rect rcRedraw){
	if( !pChild || !pChild->IsVisible() )
		return false;

	_Rect rcChild = rcRedraw;
	if( rcChild.IsRectEmpty() )
		rcChild = pChild->GetClientRect();
	if( rcChild.IsRectEmpty() )
		return false;

	// Redraw only child control.
	Redraw((CGContextRef)[[NSGraphicsContext currentContext] graphicsPort], rcChild, pChild);
	return true;
	}

void
MercuryBaseView::ResizeChildControls(int cx, int cy){
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
	/*
	if( m_toolTipCtrl.m_hWnd ) 
		return false;
	if( !m_toolTipCtrl.Create(m_pBaseWnd, 0) )
		return false;
	_Rect rcWindow;
	GetClientRectMy(rcWindow);
	m_toolTipCtrl.AddTool		(m_pBaseWnd, _T(""), _Rect(0, 0, rcWindow.Width(), rcWindow.Height()), 1000);
	m_toolTipCtrl.SetBehaviour	(PPTOOLTIP_MULTIPLE_SHOW);
	m_toolTipCtrl.SetDelayTime	(PPTOOLTIP_TIME_INITIAL, 100);
	m_toolTipCtrl.SetDelayTime	(PPTOOLTIP_TIME_ANIMATION, 5000);
	m_bTooltipIsTracking= false;*/
	return true;
	}

bool
MercuryBaseView::DeactivateTooltip(){
	/*
	if( m_toolTipCtrl.m_hWnd && m_bTooltipIsTracking ){
		m_toolTipCtrl.ActivateToolTip(false);
		m_bTooltipIsTracking = false;		
		return true;
		}*/
	return false;
	}

//#include "..\Utility\StringHelper.h"
bool
MercuryBaseView::ActivateTooltip(const TCHAR* pszTipText){
	/*
	if( !m_toolTipCtrl.m_hWnd ){
		if( !CreateTrackingTooltip() )
			return false;
		}

	_string sTip = pszTipText;
	CStringArray arrStrings;
	StringHelper::Split(&sTip, _T("\\r\\n"), arrStrings);
	sTip.empty();
	for(int i=0; i<arrStrings.size(); i++){
		sTip += arrStrings.at(i);
		if( i != arrStrings.size() -1 ){
			sTip += _T("\r\n");
			}
		}
	ESLocalizationMan::ParseText(sTip);
	m_sToolTipText = sTip;
	m_toolTipCtrl.UpdateTipText(m_sToolTipText, m_pBaseWnd, 1000);
	
	if( !m_bTooltipIsTracking ){
		m_toolTipCtrl.ActivateToolTip(true);
		m_bTooltipIsTracking = true;
		}*/
	return true;
	}

bool
MercuryBaseView::UpdateTooltipRect(int cx, int cy){
	/*
	if( !m_toolTipCtrl.m_hWnd )
		return false;
	_Rect rcBound(0, 0, cx, cy);
	//GetClientRect(rcBound);
//	m_toolTipCtrl.AdjustRect(rcBound);
	m_toolTipCtrl.SetToolRect(this, 1000, rcBound);
	*/
	return true;
	}