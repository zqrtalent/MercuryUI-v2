#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildAvatar window

#include "ESChildControl.h"

class ESChildAvatar : public ESChildControl
{
// Construction
public:
	ESChildAvatar();
	virtual ~ESChildAvatar();

	virtual bool		Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		SetPos					(int x, int y, bool bRedraw = true);
	virtual bool		SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool		Destroy					();
	virtual void		SetLogicalPos			(float fPosX, float fPosY);
	virtual	void		SetLogicalSize			(float fCX, float fCY);
	virtual void		OwnerWindowSizeChanged	(int cx, int cy);

	bool				SetAvatarImage			(Image* pImage, bool bRedraw = true);
	void				ClearAvatarImage		(bool bRedraw);

protected:
	virtual void		OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	// Designer virtual methods
	virtual _string     GetControlDefaultName	(){return _T("_avatar");};
	virtual _Size		GetControlDefaultSize	(){return _Size(100, 100);};
	virtual bool		AllowToAddChildControl	(){return true;};

protected:
	_string             m_sBorderImageFile;				// Border image file.
	_string             m_sAvatarBgImageFile;			// Avatar bg image file.
	_string             m_sAvatarImage;					// Avatar image file.

	Image				m_imageAvatarBorder;			// Border image.
	Image				m_imageAvatarBg;				// Avatar bg image.
	Image				m_imageAvatar;					// Avatar image.

	Image*				m_pImageAvatarBorder;
	Image*				m_pImageAvatarBg;
	Image*				m_pImageAvatar;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildAvatar();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
};