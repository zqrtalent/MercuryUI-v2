#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlImage window

#include "ESChildControl.h"

class ESChildControlImage : public ESChildControl
{
// Construction
public:
	ESChildControlImage();
	virtual ~ESChildControlImage();

	virtual bool		Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		SetPos					(int x, int y, bool bRedraw = true);
	virtual bool		SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool		Destroy					();
	virtual void		SetLogicalPos			(float fPosX, float fPosY);
	virtual	void		SetLogicalSize			(float fCX, float fCY);
	virtual void		OwnerWindowSizeChanged	(int cx, int cy);

	bool				SetImage				(Image* pImage, bool bRedraw);
	void				SetHandCursor			(bool bHandCursor){m_bUseHandCursor = bHandCursor;};

protected:
	void				ScanMultiImages			();

protected:
	virtual void		OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);

	virtual	void		OnMouseHover			(_Point pt, UINT nFlags);
	virtual void		OnMouseLeave			(UINT nFlags);
	virtual void		OnMouseEnter			(UINT nFlags);
	virtual BOOL		OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void		OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void		OnRButtonClick			(_Point pt, UINT nFlags);
	virtual	void		OnMButtonClick			(_Point pt, UINT nFlags);
	virtual	void		OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void		OnLButtonUp				(_Point pt, UINT nFlags);
	virtual	void		OnLButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void		OnRButtonDblClick		(_Point pt, UINT nFlags);
	virtual	void		OnMButtonDblClick		(_Point pt, UINT nFlags);

	// Designer virtual methods
	virtual _string     GetControlDefaultName	(){return _T("_image");};
	virtual _Size		GetControlDefaultSize	(){return _Size(100, 50);};
	virtual bool		AllowToAddChildControl	(){return true;};

protected: // Serialization.
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlImage();};	
	INIT_RUNTIME_VARIABLE()

protected:
	Image				m_image;
	_string             m_sImageFile;
	bool				m_bMultiImageMode;
	_string             m_sMultipleImagesFolder;
	_string             m_sMultipleImageNameFormat;
	List<Image>			m_listMultiImages;
	bool				m_bDestroyMultiImages;

	StringToIntArray	m_imageFillMode;
	_Rect				m_rcOmitBorder;
	bool				m_bUseHandCursor;

	// Variable modify callbacks.
protected:
	static bool	AttributeModify	(ESChildControlImage* pThis, VariableInfo* pInfo, void* lpValueNew);
};