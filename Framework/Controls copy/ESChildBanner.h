#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildBanner window

#include "ESEasyMotion.h"

class ESBannerImageInfo : public Serializable
{
public:
	ESBannerImageInfo();
	virtual ~ESBannerImageInfo();

public:
	_string				m_sImage;
	_string				m_sActionName;
	_string				m_sURL;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESBannerImageInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESChildBanner : public ESChildControl
{
// Construction
public:
	ESChildBanner();
	virtual ~ESChildBanner();

	virtual bool		Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		SetPos					(int x, int y, bool bRedraw = true);
	virtual bool		SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool		Destroy					();
	virtual void		SetLogicalPos			(float fPosX, float fPosY);
	virtual	void		SetLogicalSize			(float fCX, float fCY);
	virtual void		OwnerWindowSizeChanged	(int cx, int cy);

	bool				AddBannerImage			(_string sImageFile, _string sActionName = _T(""), _string sURL = _T(""));
	int					ClearBannerImages		();
	bool				UpdateBannerImages		();
	bool				UpdateSwitchButtons		();

protected:
	int					GetSwitchButtonByPoint	(_Point pt);

protected:
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	virtual	void	OnMouseHover			(_Point pt, UINT nFlags);
	virtual void	OnMouseLeave			(UINT nFlags);
	virtual void	OnMouseEnter			(UINT nFlags);
	virtual BOOL	OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnRButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnMButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp				(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void	OnRButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void	OnMButtonDblClick		(_Point pt, UINT nFlags);
	virtual void	OnTimer					(UINT nIdEvent);

	virtual void	OnMotionStart			(DWORD_PTR dwParam);
	virtual void	OnMotionProgress		(int nFrame, int nCurrTimeMS, int nFrameCt, float fPosX, float fPosY);
	virtual void	OnMotionEnd				();
	virtual void	OnMotionStopped			();

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_banner");};
	virtual _Size	GetControlDefaultSize	(){return _Size(300, 180);};

protected:
	EnumerableObject<ESBannerImageInfo>	m_imageInfos;

	volatile int						m_nImageChangeMS;			// Change image in miliseconds.
	int									m_nDistanceBetweenImages;	// Distance between images.
	bool								m_bVerticalSlide;			// Vertical slide or horizontal.
	_Size								m_szBannerImage;			// Banner image size.

	// Banner switch button. {{
	_Image								m_imageSwitchButtonNormal;	// Switch image normal.
	_Image								m_imageSwitchButtonClick;	// Switch image click.

	_Size								m_szSwitchButton; 
	_Rect								m_rcSwitchButtonsBound;
	int									m_nDistanceBetweenButtons;
	// }}
	
	ESEasyMotion						m_motion;
	int									m_nCurrentImage;
	int									m_nImageOffset;				// Image offset of current image. It depends on slide type.
	List<_Image>						m_listBannerImages;
	CriticalSection						m_lock;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildBanner();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	BannerImagesModify	(ESChildBanner* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	BannerSwitchModify	(ESChildBanner* pThis, VariableInfo* pInfo, void* lpValueNew);
};