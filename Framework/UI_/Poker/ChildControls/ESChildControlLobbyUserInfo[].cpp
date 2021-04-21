// ESChildControlLobbyUserInfo.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\Image\ImageHelper.h"
#include "..\..\..\Protocol\Basepacket.h"
#include "..\..\..\Path\Path.h"
#include "..\PokerGUI.h"
#include "ESChildControlLobbyUserInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESChildControlLobbyUserInfo

ESChildControlLobbyUserInfo::ESChildControlLobbyUserInfo(){
	m_sUserName		= _T("---");
	m_sBalanceLabel	= PokerGUI::GetLangValue(_T("BALANCE_LABEL"));
	m_sInPlayLabel	= PokerGUI::GetLangValue(_T("IN_PLAY_LABEL"));
	m_nBalance		= 0;
	m_nInPlay		= 0;
	m_nRankIndex	= 0;
	}

ESChildControlLobbyUserInfo::~ESChildControlLobbyUserInfo(){
	}

bool
ESChildControlLobbyUserInfo::Create(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	return bRet;
	}

bool
ESChildControlLobbyUserInfo::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESChildControlLobbyUserInfo::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

void
ESChildControlLobbyUserInfo::SetLogicalPos(float fPosX, float fPosY){
	ESChildControl::SetLogicalPos(fPosX, fPosY);
	}

void
ESChildControlLobbyUserInfo::SetLogicalSize(float fCX, float fCY){
	ESChildControl::SetLogicalSize(fCX, fCY);
	}

void
ESChildControlLobbyUserInfo::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}

bool
ESChildControlLobbyUserInfo::Destroy(){
	// Clean up avatars info. {{
	for(int i=0; i<m_arrAvatarInfos.GetCount(); i++){
		AvatarInfo* pInfo = (AvatarInfo*)m_arrAvatarInfos.GetData(i);
		if( pInfo ){
			delete pInfo;
			}
		}
	m_arrAvatarInfos.DeleteAll();
	// }}

	ESChildControl::Destroy();
	return true;
	}

void
ESChildControlLobbyUserInfo::OnPaintClient(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI	= PokerGUI::GetInstance();
	CImage*		pImage	= pGUI->m_pUserInfoBkgnd;
	if( !pImage ) return;

	CSize	szImage(pImage->GetWidth(), pImage->GetHeight());
	CRect	rcUserInfoBkDC, rcInvalid, rcAvatar, rcAvatarDC;
	rcUserInfoBkDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcUserInfoBkDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcUserInfoBkDC.right	= rcUserInfoBkDC.left + m_rcRect.Width(); 
	rcUserInfoBkDC.bottom	= rcUserInfoBkDC.top + m_rcRect.Height(); 

	// Make rect center.
	rcUserInfoBkDC.left		= rcUserInfoBkDC.left + ((m_rcRect.Width() - szImage.cx) / 2);
	rcUserInfoBkDC.top		= rcUserInfoBkDC.top + ((m_rcRect.Height() - szImage.cy) / 2);
	rcUserInfoBkDC.right	= rcUserInfoBkDC.left + szImage.cx;
	rcUserInfoBkDC.bottom	= rcUserInfoBkDC.top + szImage.cy;

	// Draw bg image. {{
	rcInvalid	= rcUserInfoBkDC & *pRectDC;
	if( !rcInvalid.IsRectEmpty() )
		PokerGUI::RenderImage(pDC, rcInvalid, rcUserInfoBkDC, pImage, false);
	// }}

	// Draw user avatar.
	rcAvatar				=  pGUI->m_rcUserAvatar; 
	rcAvatarDC.left			=  rcUserInfoBkDC.left + rcAvatar.left;
	rcAvatarDC.top			=  rcUserInfoBkDC.top + rcAvatar.top;
	rcAvatarDC.right		=  rcAvatarDC.left + rcAvatar.Width();
	rcAvatarDC.bottom		=  rcAvatarDC.top + rcAvatar.Height();
	if( !m_avatarImage.IsNull() ){
		rcInvalid	= rcAvatarDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() )
			PokerGUI::RenderImage(pDC, rcInvalid, rcAvatarDC, &m_avatarImage, false);
		}
	
	// Draw user rank indicators.
	DrawRankIndicator(pDC, pRectDC, pRectClient);

	CRect rcLabel, rcLabelDC;
	// User name
	rcLabel				=  pGUI->m_rcUserNameLabel; 
	rcLabelDC.left		=  rcUserInfoBkDC.left + rcLabel.left;
	rcLabelDC.top		=  rcUserInfoBkDC.top + rcLabel.top;
	rcLabelDC.right		=  rcLabelDC.left + rcLabel.Width();
	rcLabelDC.bottom	=  rcLabelDC.top + rcLabel.Height();
	DrawLabel(m_sUserName, pDC, rcLabelDC, pGUI->m_pLobbyUserInfoFont, DT_LEFT);

	// Balance label
	rcLabel				=  pGUI->m_rcUserBalanceLabel; 
	rcLabelDC.left		=  rcUserInfoBkDC.left + rcLabel.left;
	rcLabelDC.top		=  rcUserInfoBkDC.top + rcLabel.top;
	rcLabelDC.right		=  rcLabelDC.left + rcLabel.Width();
	rcLabelDC.bottom	=  rcLabelDC.top + rcLabel.Height();
	DrawLabel(m_sBalanceLabel, pDC, rcLabelDC, pGUI->m_pLobbyUserInfoFont);

	// In-Play label
	rcLabel				=  pGUI->m_rcUserInPlayLabel; 
	rcLabelDC.left		=  rcUserInfoBkDC.left + rcLabel.left;
	rcLabelDC.top		=  rcUserInfoBkDC.top + rcLabel.top;
	rcLabelDC.right		=  rcLabelDC.left + rcLabel.Width();
	rcLabelDC.bottom	=  rcLabelDC.top + rcLabel.Height();
	DrawLabel(m_sInPlayLabel, pDC, rcLabelDC, pGUI->m_pLobbyUserInfoFont);

	CString sLabel;
	// Balance amount.
	sLabel.Format(_T("%d.%02d"), m_nBalance/100, m_nBalance%100);
	rcLabel				=  pGUI->m_rcUserBalanceAmount; 
	rcLabelDC.left		=  rcUserInfoBkDC.left + rcLabel.left;
	rcLabelDC.top		=  rcUserInfoBkDC.top + rcLabel.top;
	rcLabelDC.right		=  rcLabelDC.left + rcLabel.Width();
	rcLabelDC.bottom	=  rcLabelDC.top + rcLabel.Height();
	DrawLabel(sLabel, pDC, rcLabelDC, pGUI->m_pLobbyUserInfoFont);

	// InPlay amount.
	sLabel.Format(_T("%d.%02d"), m_nInPlay/100, m_nInPlay%100);
	rcLabel				=  pGUI->m_rcUserInPlayAmount; 
	rcLabelDC.left		=  rcUserInfoBkDC.left + rcLabel.left;
	rcLabelDC.top		=  rcUserInfoBkDC.top + rcLabel.top;
	rcLabelDC.right		=  rcLabelDC.left + rcLabel.Width();
	rcLabelDC.bottom	=  rcLabelDC.top + rcLabel.Height();
	DrawLabel(sLabel, pDC, rcLabelDC, pGUI->m_pLobbyUserInfoFont);

	ESChildControl::OnPaintClient(pDC, pRectDC, pRectClient);
	}

void
ESChildControlLobbyUserInfo::DrawRankIndicator(CDC* pDC, CRect* pRectDC, CRect* pRectClient){
	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	CImage*		pImage		= pGUI->m_pUserInfoRank;
	CImage*		pImageDis	= pGUI->m_pUserInfoRankDis;
	if( !pImage || !pImageDis ) return;

	CSize	szImage(pImage->GetWidth(), pImage->GetHeight());
	CRect	rcUserInfoBkDC, rcInvalid;
	rcUserInfoBkDC.left		= (m_rcRect.left - pRectClient->left) + pRectDC->left; 
	rcUserInfoBkDC.top		= (m_rcRect.top - pRectClient->top) + pRectDC->top; 
	rcUserInfoBkDC.right	= rcUserInfoBkDC.left + m_rcRect.Width(); 
	rcUserInfoBkDC.bottom	= rcUserInfoBkDC.top + m_rcRect.Height(); 

	CRect rcRankDC;
	rcRankDC.left	= rcUserInfoBkDC.left + pGUI->m_ptUserRankStar.x;
	rcRankDC.top	= rcUserInfoBkDC.top + pGUI->m_ptUserRankStar.y;
	rcRankDC.right	= rcRankDC.left + szImage.cx;
	rcRankDC.bottom = rcRankDC.top + szImage.cy;

	// Draw rank indicator images. {{
	int nLoop		= 0;
	int	nCt			= 5;
	int nSpaceCX	= 2;

	while( nLoop < nCt ){
		rcInvalid	= rcRankDC & *pRectDC;
		if( !rcInvalid.IsRectEmpty() ){
			if( m_nRankIndex > nLoop )
				PokerGUI::RenderImage(pDC, rcInvalid, rcRankDC, pImage, false);
			else
				PokerGUI::RenderImage(pDC, rcInvalid, rcRankDC, pImageDis, false);
			}

		rcRankDC.left	= rcRankDC.right + nSpaceCX;
		rcRankDC.right	= rcRankDC.left + szImage.cx;
		nLoop			++;
		}
	// }}
	}

void
ESChildControlLobbyUserInfo::DrawLabel(CString& sLabel, CDC* pDC, CRect rcLabelDC, CFont* pFont, int nHAlign /*= DT_CENTER*/, int nVAlign /*= DT_VCENTER*/){
	PokerGUI*	pGUI		= PokerGUI::GetInstance();
	CFont*		pFontOld	= pDC->SelectObject(pFont);
	CSize szText;
	ImgHlp::GetTextSize(&sLabel, pFont, szText);

	CPoint ptLabel;
	switch( nHAlign ){
		case DT_LEFT:
			{
				ptLabel.x	= rcLabelDC.left + 2;
				break;
			}
		case DT_RIGHT:
			{
				ptLabel.x	= rcLabelDC.right - szText.cx;
				break;
			}
		case DT_CENTER:
			{
				ptLabel.x	= rcLabelDC.left + (rcLabelDC.Width() - szText.cx)/2;
				break;
			}
		};

	switch( nVAlign ){
		case DT_TOP:
			{
				ptLabel.y	= rcLabelDC.top + 2;
				break;
			}
		case DT_BOTTOM:
			{
				ptLabel.y	= rcLabelDC.bottom- szText.cy;
				break;
			}
		case DT_VCENTER:
			{
				ptLabel.y	= rcLabelDC.top + (rcLabelDC.Height() - szText.cy)/2;
				break;
			}
		};

	pDC->SetBkMode		(TRANSPARENT);
	pDC->SetTextColor	(pGUI->m_crUserInfoText);
	PokerGUI::ExtTextOut(pDC, ptLabel.x, ptLabel.y, ETO_CLIPPED, rcLabelDC, sLabel);
	pDC->SelectObject	(pFontOld);
	}

void	
ESChildControlLobbyUserInfo::OnMouseHover(CPoint pt, UINT nFlags){
	}

BOOL	
ESChildControlLobbyUserInfo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return 1;
	}

void	
ESChildControlLobbyUserInfo::OnMouseLeave(UINT nFlags){
	}

void	
ESChildControlLobbyUserInfo::OnMouseEnter(UINT nFlags){
	}

void
ESChildControlLobbyUserInfo::OnLButtonDown(CPoint pt, UINT nFlags){
	}

void
ESChildControlLobbyUserInfo::OnLButtonUp(CPoint pt, UINT nFlags){

	}

void	
ESChildControlLobbyUserInfo::OnLButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyUserInfo::OnRButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyUserInfo::OnMButtonClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyUserInfo::OnLButtonDblClick(CPoint pt, UINT nFlags){
	OnLButtonDown(pt, nFlags);
	}

void	
ESChildControlLobbyUserInfo::OnRButtonDblClick(CPoint pt, UINT nFlags){
	}

void	
ESChildControlLobbyUserInfo::OnMButtonDblClick(CPoint pt, UINT nFlags){
	}

void
ESChildControlLobbyUserInfo::SetUserName(CString sUserName, bool bRedraw /*= true*/){
	m_sUserName = sUserName;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlLobbyUserInfo::SetUserBalance(int nBalance, bool bRedraw /*= true*/){
	if( m_nBalance == nBalance )
		return;
	m_nBalance = nBalance;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlLobbyUserInfo::SetUserBalanceLabel(CString sLabel, bool bRedraw /*= true*/){
	m_sBalanceLabel = sLabel;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlLobbyUserInfo::SetUserInPlay(int nInPlay, bool bRedraw /*= true*/){
	if( m_nInPlay == nInPlay )
		return;
	m_nInPlay = nInPlay;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlLobbyUserInfo::SetUserInPlayLabel(CString sLabel, bool bRedraw /*= true*/){
	m_sInPlayLabel = sLabel;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlLobbyUserInfo::SetRankIndex(int nRankIndex, bool bRedraw /*= true*/){
	m_nRankIndex = nRankIndex;
	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlLobbyUserInfo::SetAvatarId(int nAvatarId, bool bRedraw /*= true*/){
	m_nAvatarId = nAvatarId;
	m_avatarImage.Destroy();

	AvatarInfo* pAvatarInfo = GetAvatarInfoById(nAvatarId);
	if( !pAvatarInfo ) return;

	CString sAvatarFile = PokerGUI::GetInstance()->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
	CImage* pAvatarImage = PokerGUI::LoadImage(sAvatarFile);
	if( pAvatarImage && !pAvatarImage->IsNull() ){
		m_avatarImage.Attach(pAvatarImage->Detach());
		delete pAvatarImage;
		}

	/*

	CString sAvatarFile = PokerGUI::GetInstance()->m_sAvatarsCacheFolder + _T("\\") + pAvatarInfo->m_sAvatarFileName;
	Path::BuildFullPath(sAvatarFile);
	m_avatarImage.Load(sAvatarFile);*/

	if( bRedraw ){
		Redraw();
		}
	}

void
ESChildControlLobbyUserInfo::SetAvatars(AutoSortedArray* pArrAvatarInfoById){
	// Clean up avatars info. {{
	for(int i=0; i<m_arrAvatarInfos.GetCount(); i++){
		AvatarInfo* pInfo = (AvatarInfo*)m_arrAvatarInfos.GetData(i);
		if( pInfo ){
			delete pInfo;
			}
		}
	m_arrAvatarInfos.DeleteAll();
	// }}
	if( pArrAvatarInfoById )
		m_arrAvatarInfos.Copy(pArrAvatarInfoById);
	}

AvatarInfo*
ESChildControlLobbyUserInfo::GetAvatarInfoById(int nAvatarId){
	int nIndex = m_arrAvatarInfos.IndexOf(nAvatarId);
	if( nIndex == -1 ) return NULL;
	AvatarInfo* pInfo = (AvatarInfo*)m_arrAvatarInfos.GetData(nIndex);
	return pInfo;
	}



