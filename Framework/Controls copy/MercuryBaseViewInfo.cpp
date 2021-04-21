#include "MercuryBaseViewInfo.h"
#include "MercuryBaseWnd.h"
#include "ESFrameBase.h"
#include "../Utility/GrowableMemory.h"

//////////////////////////////////////////////////////
//
//	class ChildControlAttribute
//
/////////////////////////////////////////////////////
ChildControlAttribute::ChildControlAttribute(){
	}

BEGIN_DECLARE_VAR(ChildControlAttribute, Serializable)
	DECLARE_VAR(_T("ChildControlAttribute"),	_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Control Id"),				_T(""), VariableType::VT_Size, offsetof(ChildControlAttribute,m_nControlId), false, false, false)
	DECLARE_VAR(_T("Lock"),						_T(""), VariableType::VT_Size, offsetof(ChildControlAttribute,m_bLock), false, false, false)
END_DECLARE_VAR()

ChildControlAttribute::~ChildControlAttribute(){
	}


//////////////////////////////////////////////////////
//
//	class MercuryBaseViewInfo
//
/////////////////////////////////////////////////////
MercuryBaseViewInfo::MercuryBaseViewInfo(){
	m_pView			= NULL;
	m_szDefault		.SetSize(0, 0);
	m_szMin			.SetSize(0, 0);
	m_szMax			.SetSize(0, 0);
	m_rcOmitBorder	.SetRect(0, 0, 0, 0);

	m_bgFillMode	.Add(_T("None"),				_FillMode::FillMode_None);
	m_bgFillMode	.Add(_T("Color"),				_FillMode::FillMode_Color);
	m_bgFillMode	.Add(_T("CentredImage"),		_FillMode::FillMode_CentredImage);
	m_bgFillMode	.Add(_T("StretchImage"),		_FillMode::FillMode_StretchImage);
	m_bgFillMode	.Add(_T("RepeatedImage"),		_FillMode::FillMode_RepeatedImage);
	m_bgFillMode	.Add(_T("OmitBorderedImage"),	_FillMode::FillMode_OmitBorderedImage);
	m_bgFillMode	.SetCurrentIndex(0);

	m_crFill		= 0;
	m_bTitleBar		= true;
	m_bMinimizeBox	= true;
	m_bMaximizeBox	= true;
	m_sTitle		= _T("Window 1");

	AddVariableModifyCallBack(&m_szDefault,			(VariableValueModifyCallback)&MercuryBaseViewInfo::DefaultSizeModify);
	AddVariableModifyCallBack(&m_szMin,				(VariableValueModifyCallback)&MercuryBaseViewInfo::MinSizeModify);
	AddVariableModifyCallBack(&m_szMax,				(VariableValueModifyCallback)&MercuryBaseViewInfo::MaxSizeModify);
	AddVariableModifyCallBack(&m_bgFillMode,		(VariableValueModifyCallback)&MercuryBaseViewInfo::BgFillModeModify);
	AddVariableModifyCallBack(&m_crFill,			(VariableValueModifyCallback)&MercuryBaseViewInfo::FillColorModeModify);
	AddVariableModifyCallBack(&m_bgImage,			(VariableValueModifyCallback)&MercuryBaseViewInfo::BgImageModeModify);
	AddVariableModifyCallBack(&m_rcOmitBorder,		(VariableValueModifyCallback)&MercuryBaseViewInfo::OmitBorderModeModify);
	AddVariableModifyCallBack(&m_bTitleBar,			(VariableValueModifyCallback)&MercuryBaseViewInfo::OnTitlebarModify);
	AddVariableModifyCallBack(&m_sizeBoxImage,		(VariableValueModifyCallback)&MercuryBaseViewInfo::OnSizeBoxImageModify);
	AddVariableModifyCallBack(&m_bMinimizeBox,		(VariableValueModifyCallback)&MercuryBaseViewInfo::OnMinimizeBoxModify);
	AddVariableModifyCallBack(&m_bMaximizeBox,		(VariableValueModifyCallback)&MercuryBaseViewInfo::OnMaximizeBoxModify);
	}

BEGIN_DECLARE_VAR(MercuryBaseViewInfo, Serializable)
	DECLARE_VAR(_T("MercuryBaseViewInfo"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Default Size"),				_T(""), VariableType::VT_Size, offsetof(MercuryBaseViewInfo,m_szDefault), false, false, false)
	DECLARE_VAR(_T("Min Size"),					_T(""), VariableType::VT_Size, offsetof(MercuryBaseViewInfo,m_szMin), false, false, false)
	DECLARE_VAR(_T("Max Size"),					_T(""), VariableType::VT_Size, offsetof(MercuryBaseViewInfo,m_szMax), false, false, false)
	DECLARE_VAR(_T("Font"),						_T(""), VariableType::VT_Font, offsetof(MercuryBaseViewInfo,m_font), false, false, false)
	DECLARE_VAR(_T("Background Fill Mode"),		_T(""), VariableType::VT_Enumerable, offsetof(MercuryBaseViewInfo,m_bgFillMode), false, false, false)
	DECLARE_VAR(_T("Fill Color"),				_T(""), VariableType::VT_Color32, offsetof(MercuryBaseViewInfo,m_crFill), false, false, false)
	DECLARE_VAR(_T("BgImage"),					_T(""), VariableType::VT_Image, offsetof(MercuryBaseViewInfo,m_bgImage), false, false, false)
	DECLARE_VAR(_T("OmitBorder"),				_T(""), VariableType::VT_Rect, offsetof(MercuryBaseViewInfo,m_rcOmitBorder), false, false, false)
	DECLARE_VAR(_T("Title"),					_T(""), VariableType::VT_String, offsetof(MercuryBaseViewInfo,m_sTitle), false, false, false)
	DECLARE_VAR(_T("Titlebar"),					_T(""), VariableType::VT_Bool, offsetof(MercuryBaseViewInfo,m_bTitleBar), false, false, false)
	DECLARE_VAR(_T("MinimizeBox"),				_T(""), VariableType::VT_Bool, offsetof(MercuryBaseViewInfo,m_bMinimizeBox), false, false, false)
	DECLARE_VAR(_T("MaximizeBox"),				_T(""), VariableType::VT_Bool, offsetof(MercuryBaseViewInfo,m_bMaximizeBox), false, false, false)
	DECLARE_VAR(_T("SizeBoxImage"),				_T(""), VariableType::VT_Image, offsetof(MercuryBaseViewInfo,m_sizeBoxImage), false, false, false)
	DECLARE_VAR(_T("BorderImage"),				_T(""), VariableType::VT_Image, offsetof(MercuryBaseViewInfo,m_borderImage), false, false, false)
	DECLARE_VAR(_T("Child Control Attributes"),	_T(""), VariableType::VT_None, offsetof(MercuryBaseViewInfo,m_listAttributes), false, true, true)
END_DECLARE_VAR()

MercuryBaseViewInfo::~MercuryBaseViewInfo(){
	}

bool
MercuryBaseViewInfo::DefaultSizeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;

	#pragma message ("Incomplete: MercuryBaseViewInfo::DefaultSizeModify")
	/*
	ESFrameBase* pWindow = (ESFrameBase*)pThis->m_pView->m_pBaseWnd;
	if( pWindow && pWindow->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
		pWindow->m_szDefault = pThis->m_szDefault;

		if( pThis->m_bTitleBar ){
			_Rect rcWindow;
			pWindow->GetWindowRect(rcWindow);
			_Rect rcClient;
			pWindow->GetClientRect(rcClient);

			_Size szAdd((rcWindow.Width() - rcClient.Width()), (rcWindow.Height() - rcClient.Height()));
			pWindow->SetWindowPos(NULL, 0, 0, pThis->m_szDefault.cx + szAdd.cx, pThis->m_szDefault.cy + szAdd.cy, SWP_NOMOVE);
			}
		else
			pWindow->SetWindowPos(NULL, 0, 0, pThis->m_szDefault.cx, pThis->m_szDefault.cy, SWP_NOMOVE);
		return true;
		}*/
	return false;
	}

bool
MercuryBaseViewInfo::MinSizeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;

	#pragma message ("Incomplete: MercuryBaseViewInfo::MinSizeModify")
	/*
	ESFrameBase* pWindow = (ESFrameBase*)pThis->m_pView->m_pBaseWnd;
	if( pWindow && pWindow->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
		pWindow->m_szMin = pThis->m_szMin;
		if( pThis->m_bTitleBar ){
			_Rect rcWindow;
			pWindow->GetWindowRect(rcWindow);
			_Rect rcClient;
			pWindow->GetClientRect(rcClient);

			_Size szAdd((rcWindow.Width() - rcClient.Width()), (rcWindow.Height() - rcClient.Height()));
			pWindow->SetWindowPos(NULL, 0, 0, pThis->m_szDefault.cx + szAdd.cx, pThis->m_szDefault.cy + szAdd.cy, SWP_NOMOVE);
			}
		else
			pWindow->SetWindowPos(NULL, 0, 0, pThis->m_szDefault.cx, pThis->m_szDefault.cy, SWP_NOMOVE);
		return true;
		}*/
	return false;
	}

bool
MercuryBaseViewInfo::MaxSizeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;
#ifdef _WINDOWS
	ESFrameBase* pWindow = (ESFrameBase*)pThis->m_pView;
	if( pWindow /*&& pWindow->IsKindOf(RUNTIME_CLASS(ESFrameBase))*/ ){
		pWindow->m_szMax = pThis->m_szMax;

		if( pThis->m_bTitleBar ){
			_Rect rcWindow;
			pWindow->GetWindowRect(rcWindow);
			_Rect rcClient;
			pWindow->GetClientRect(rcClient);

			_Size szAdd((rcWindow.Width() - rcClient.Width()), (rcWindow.Height() - rcClient.Height()));
			pWindow->SetWindowPos(NULL, 0, 0, pThis->m_szDefault.cx + szAdd.cx, pThis->m_szDefault.cy + szAdd.cy, SWP_NOMOVE);
			}
		else
			pWindow->SetWindowPos(NULL, 0, 0, pThis->m_szDefault.cx, pThis->m_szDefault.cy, SWP_NOMOVE);
		return true;
		}
#else
    return false;
#endif
	return false;
	}

bool
MercuryBaseViewInfo::BgFillModeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;
	pThis->m_pView->Invalidate();
	return true;
	}

bool
MercuryBaseViewInfo::FillColorModeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;
	pThis->m_pView->Invalidate();
	return true;
	}

bool
MercuryBaseViewInfo::BgImageModeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;
	pThis->m_pView->Invalidate();
	return true;
	}

bool
MercuryBaseViewInfo::OmitBorderModeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;
	if( pThis->m_bgFillMode.GetCurrentValue() == FillMode_OmitBorderedImage && !pThis->m_bgImage.IsNull() )
		pThis->m_pView->Invalidate();
	return true;
	}

bool
MercuryBaseViewInfo::OnTitlebarModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;

	ESFrameBase* pWindow = (ESFrameBase*)pThis->m_pView;
	if( pWindow ){
#ifdef _WINDOWS
		DWORD dwStyleRemove = 0L;
		DWORD dwStyleAdd	= 0L;

		if( pThis->m_bTitleBar )
			dwStyleAdd |= (WS_CAPTION|WS_THICKFRAME);
		else
			dwStyleRemove |= (WS_CAPTION|WS_THICKFRAME);

		pWindow->ModifyStyle(dwStyleRemove, dwStyleAdd);

		if( !pThis->m_bTitleBar ){
			_Rect rcView;
			pThis->m_pView->GetClientRectMy(rcView);
			pWindow->SetWindowPos(NULL, 0, 0, rcView.Width(), rcView.Height(), SWP_NOMOVE|SWP_FRAMECHANGED);
			}
#else
        return false;
#endif
		return true;
		}
	return true;
	}

bool
MercuryBaseViewInfo::OnSizeBoxImageModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;
	pThis->m_pView->Invalidate();
	return true;
	}

bool
MercuryBaseViewInfo::OnMinimizeBoxModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;

	ESFrameBase* pWindow = (ESFrameBase*)pThis->m_pView;
	if( pWindow ){
#ifdef _WINDOWS
		DWORD dwStyleRemove = 0L;
		DWORD dwStyleAdd	= 0L;

		if( pThis->m_bMinimizeBox )
			dwStyleAdd |= (WS_MINIMIZEBOX);
		else
			dwStyleRemove |= (WS_MINIMIZEBOX);

		pWindow->ModifyStyle(dwStyleRemove, dwStyleAdd);
		return true;
#else
        return false;
#endif
		}
	return true;
	}

bool
MercuryBaseViewInfo::OnMaximizeBoxModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo){
	ASSERT(pThis && pThis->m_pView);
	if( !pThis || !pThis->m_pView) return false;

	ESFrameBase* pWindow = (ESFrameBase*)pThis->m_pView;
	if( pWindow ){
#ifdef _WINDOWS
		DWORD dwStyleRemove = 0L;
		DWORD dwStyleAdd	= 0L;

		if( pThis->m_bMaximizeBox )
			dwStyleAdd |= (WS_MAXIMIZEBOX);
		else
			dwStyleRemove |= (WS_MAXIMIZEBOX);
		pWindow->ModifyStyle(dwStyleRemove, dwStyleAdd);
		return true;
#else
        return false;
#endif
		}
	return true;
	}