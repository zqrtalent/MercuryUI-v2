#pragma once
#include "../Serialize/Serializable.h"
#include "../_platformCompat/PlatformImage.h"

class ChildControlAttribute : public Serializable
{
public:
	ChildControlAttribute();
	virtual ~ChildControlAttribute();

	int		m_nControlId;			// Control id.
	bool	m_bLock;				// Lock status.

public:
	virtual Serializable*	CreateSerializableObject	(){return new ChildControlAttribute();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class MercuryBaseView;
class MercuryBaseViewInfo : public Serializable
{
public:
	MercuryBaseViewInfo();
	virtual ~MercuryBaseViewInfo();

	friend MercuryBaseView;

public:
	_Size									m_szDefault;			// Default window size.
	_Size									m_szMin;				// Minimal window size.
	_Size									m_szMax;				// Maximum window size

	Font									m_font;

	StringToIntArray						m_bgFillMode;			// Background fill mode.
	COLORREF								m_crFill;				// Background fill color.
	Image									m_bgImage;				// Fill image binary data.
	_Rect									m_rcOmitBorder;			// Omit border rect.
	_string								m_sTitle;				// Window title.
	bool									m_bTitleBar;			// Titlebar
	bool									m_bMinimizeBox;			// Maximize box.
	bool									m_bMaximizeBox;			// Minimize box.
	Image									m_sizeBoxImage;			// _Size box image
	Image									m_borderImage;			// Border image.

	EnumerableObject<ChildControlAttribute>	m_listAttributes;		// Child control attributes.

protected:
	MercuryBaseView*	m_pView;

	// Variable modify callbacks.
protected:
	static bool	DefaultSizeModify	(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	MinSizeModify		(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	MaxSizeModify		(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	BgFillModeModify	(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	FillColorModeModify	(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	BgImageModeModify	(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	OmitBorderModeModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	OnTitlebarModify	(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	OnSizeBoxImageModify(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	OnMinimizeBoxModify	(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);
	static bool	OnMaximizeBoxModify	(MercuryBaseViewInfo* pThis, VariableInfo* pInfo);

public:
	virtual Serializable*	CreateSerializableObject	(){return new MercuryBaseViewInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};
