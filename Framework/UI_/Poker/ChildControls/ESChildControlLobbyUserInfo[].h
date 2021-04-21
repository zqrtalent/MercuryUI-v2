#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlLobbyUserInfo window

#include "..\..\..\Controls\ESChildControl.h"

class AvatarInfo;
class ESChildControlLobbyUserInfo : public ESChildControl
{
// Construction
public:
	ESChildControlLobbyUserInfo();
	virtual ~ESChildControlLobbyUserInfo();

	virtual bool		Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		SetPos					(int x, int y, bool bRedraw = true);
	virtual bool		SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool		Destroy					();
	virtual void		SetLogicalPos			(float fPosX, float fPosY);
	virtual	void		SetLogicalSize			(float fCX, float fCY);
	virtual void		OwnerWindowSizeChanged	(int cx, int cy);

public:
	void				SetUserName				(CString sUserName, bool bRedraw = true);
	void				SetUserBalance			(int nBalance, bool bRedraw = true);
	void				SetUserBalanceLabel		(CString sLabel, bool bRedraw = true);
	void				SetUserInPlay			(int nInPlay, bool bRedraw = true);
	void				SetUserInPlayLabel		(CString sLabel, bool bRedraw = true);
	void				SetRankIndex			(int nRankIndex, bool bRedraw = true);
	void				SetAvatarId				(int nAvatarId, bool bRedraw = true);
	void				SetAvatars				(AutoSortedArray* pArrAvatarInfoById);

protected:
	virtual void	OnPaintClient			(CDC* pDC, CRect* pRectDC, CRect* pRectClient);

	void			DrawRankIndicator		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);
	void			DrawLabel				(CString& sLabel, CDC* pDC, CRect rcLabelDC, CFont* pFont, int nHAlign = DT_CENTER, int nVAlign = DT_VCENTER);

	virtual	void	OnMouseHover			(CPoint pt, UINT nFlags);
	virtual void	OnMouseLeave			(UINT nFlags);
	virtual void	OnMouseEnter			(UINT nFlags);
	virtual BOOL	OnSetCursor				(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick			(CPoint pt, UINT nFlags);
	virtual	void	OnRButtonClick			(CPoint pt, UINT nFlags);
	virtual	void	OnMButtonClick			(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonDown			(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonUp				(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonDblClick		(CPoint pt, UINT nFlags);
	virtual	void	OnRButtonDblClick		(CPoint pt, UINT nFlags);
	virtual	void	OnMButtonDblClick		(CPoint pt, UINT nFlags);

protected:
	AvatarInfo*		GetAvatarInfoById		(int nAvatarId);

protected:
	CString			m_sUserName;
	CString			m_sBalanceLabel;
	CString			m_sInPlayLabel;
	int				m_nBalance;
	int				m_nInPlay;
	int				m_nRankIndex;
	int				m_nAvatarId;
	AutoSortedArray	m_arrAvatarInfos;
	CImage			m_avatarImage;
};