#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlLobbyBanner window

#include "..\..\..\Controls\ESEasyMotion.h"
#include "..\..\..\Controls\FlashWindow.h"
#include <afxmt.h>

class ESChildControlLobbyBanner : public ESChildControl
{
// Construction
public:
	ESChildControlLobbyBanner();
	virtual ~ESChildControlLobbyBanner();

	virtual bool		Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		SetPos					(int x, int y, bool bRedraw = true);
	virtual bool		SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool		Destroy					();
	virtual void		SetLogicalPos			(float fPosX, float fPosY);
	virtual	void		SetLogicalSize			(float fCX, float fCY);
	virtual void		OwnerWindowSizeChanged	(int cx, int cy);

	void				AddBannerImage			(CImage* pImage);
	void				ClearBannerImages		();

	void				SetBannerButtonImages	(CImage* pImageDef, CImage* pImageActive);

	void				SetFlashMoviePath		(CString sPath) {m_sFlashMovie = sPath;};

protected:
	virtual void	OnPaintClient		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);

	virtual	void	OnMouseHover		(CPoint pt, UINT nFlags);
	virtual void	OnMouseLeave		(UINT nFlags);
	virtual void	OnMouseEnter		(UINT nFlags);
	virtual BOOL	OnSetCursor			(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick		(CPoint pt, UINT nFlags);
	virtual	void	OnRButtonClick		(CPoint pt, UINT nFlags);
	virtual	void	OnMButtonClick		(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonDown		(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonUp			(CPoint pt, UINT nFlags);
	virtual	void	OnLButtonDblClick	(CPoint pt, UINT nFlags);
	virtual	void	OnRButtonDblClick	(CPoint pt, UINT nFlags);
	virtual	void	OnMButtonDblClick	(CPoint pt, UINT nFlags);
	virtual void	OnTimer				(UINT nIdEvent);

	virtual void	OnMotionStart		();
	virtual void	OnMotionProgress	(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY);
	virtual void	OnMotionEnd			();
	virtual void	OnMotionStopped		();

protected:
	CFlashWindow*		m_pFlash;
	CString				m_sFlashMovie;

	CImage*				m_pBannerButtonImageDef;
	CImage*				m_pBannerButtonImageActive;
	CSize				m_szBannerButton;

	int					m_nChangeImageMS;			// Change image in miliseconds.
	int					m_nDistanceBetweenImages;	// Distance between images.
	bool				m_bVerticalSlide;			// Vertical slide or horizontal.

	ESEasyMotion		m_motion;
	CSize				m_szImage;
	int					m_nCurrentImage;
	int					m_nImageOffset;				// Image offset of current image. It depends on slide type.
	List<CImage>		m_listBannerImages;
	CCriticalSection	m_lock;
};