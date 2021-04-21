#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESChildComboBox window

#include "ESChildTextBox.h"
#include "../Array/StringToPtrArray.h"

class ESComboBoxItemInfo : public Serializable
{
public:
	ESComboBoxItemInfo();
	virtual ~ESComboBoxItemInfo();

public:
	_string		m_sName;
	int			m_nId;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESComboBoxItemInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESChildComboListBox;
class ESChildComboBox : public ESChildTextBox
{
// Construction
public:
	ESChildComboBox();
	virtual ~ESChildComboBox();

	enum ComboType
	{
		ESChildComboBox_DropDown = 1,
		ESChildComboBox_DropList
	};

	virtual bool	Create					(int nId, _Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);
	virtual bool	SetFocus				();
	virtual void	KillFocus				();

	bool			SetCurSel				(int nSelIndex, bool bRedraw);
	bool			SetCurSelByParam		(void* pParam, bool bRedraw);
	int				GetCurSel				(){return m_nSelectedItemIndex;};
	bool			AddItem					(_string sText, void* lpParam = NULL, bool bRedraw = false);
	void			ClearContent			(bool bRedraw);
	_string		GetItemText				(int nIndex = -1);
	void*			GetSelectedParam		();

	void			SetDropListType			();
	void			SetDropDownType			();


protected:
	virtual void	OnPaintClient			(_DC* pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual BOOL	OnSetCursor				(HWND hWnd, UINT nHitTest, UINT message);
	virtual	void	OnLButtonClick			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonDown			(_Point pt, UINT nFlags);
	virtual	void	OnLButtonUp				(_Point pt, UINT nFlags);
	virtual void	OnKeyDown				(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual	void	OnMouseWheel			(UINT nFlags, short zDelta, _Point pt);

	virtual _Rect	GetTextAreaRect			();

	// Designer virtual methods
	virtual _string	GetControlDefaultName	(){return _T("_combobox");};
	virtual _Size	GetControlDefaultSize	(){return _Size(100, 20);};

protected: // Serialization.
	virtual Serializable*	CreateSerializableObject	(){return new ESChildComboBox();};	
	INIT_RUNTIME_VARIABLE()

protected:
	void			CalcButtonRect			();
	void			ShowListBox				(bool bHide = false);
	bool			IsListBoxVisible		();

protected:
	_Rect									m_rcBoxButton;
	StringToPtrArray						m_content;
	EnumerableObject<ESComboBoxItemInfo>	m_listItems;
	ESChildComboListBox*					m_pListBox;
	int										m_nSelectedItemIndex;
	int										m_nListBoxHeight;

	int										m_nComboType;
	

	// Variable modify callbacks.
protected:

	// Control events.
protected:
	void			OnListBoxItemClick		(ESChildComboListBox* pSender, ChildControlEventCode eventCode, int nParam = 0);
};