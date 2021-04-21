#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlCheckBox window

#include "ESChildControl.h"

class ESChildControlCheckBox : public ESChildControl
{
// Construction
public:
	ESChildControlCheckBox();
	virtual ~ESChildControlCheckBox();

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	void			SetTextFont				(_Font* pFont);
	void			SetTextColor			(COLORREF crText)					{ m_crText = crText; };
	void			SetLabel				(_string sText, bool bRedraw);
	void			SetCheck				(bool bCheck, bool bRedraw = true)	{m_bCheckState = bCheck; if( bRedraw ) Redraw();};
	bool			GetCheck				()									{ return m_bCheckState; };

	bool			SetCheckImages			(_Image* pImageCheck, _Image* pImageNormal, bool bRedraw);

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
	virtual bool	RemainCapturedOutOfClientArea(){return false;};

	virtual void	OnLocalizeStrings	();

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_checkBox");};
	virtual _Size		GetControlDefaultSize	(){return _Size(100, 50);};

	_Image*				GetCheckedImage		();
	_Image*				GetUncheckedImage	();

protected:
	_Font				m_fontText;					// Text font.
	COLORREF			m_crText;					// Text color.
	_string			m_sLabel;					// Label text.
	_string			m_sLabelLocal;				// Localized label text.
	_Image				m_checkedImage;				// Checked image.
	_Image				m_uncheckedImage;			// Unchecked image.
	_string			m_sCheckedImageFile;		// Checked image file.
	_string			m_sUncheckedImageFile;		// Unchecked image file.
	bool				m_bCheckState;				// Check state.

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlCheckBox();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	AttributeModify	(ESChildControlCheckBox* pThis, VariableInfo* pInfo, void* lpValueNew);
};