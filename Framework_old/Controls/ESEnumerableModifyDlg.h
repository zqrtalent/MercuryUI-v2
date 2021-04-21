// ESEnumerableModifyDlg.h : header file
//

#pragma once
#include "MercuryBaseWnd.h"

class ESChildControlImageButton;
class ESChildControlList;
class VirtualRowSet;

// ESEnumerableModifyDlg dialog
class ESEnumerableModifyDlg : public MercuryBaseView
{
// Construction
public:
	ESEnumerableModifyDlg();	// standard constructor

	void			SetEnumerableObject		(EnumerableObject<Serializable>* pEnumerable);

protected:
	virtual void	SetTitle				();
	virtual void	Setup					();
	virtual BOOL	UseDoubleBufferedDC		(){return TRUE;};

	virtual	void	OnClose					();
	virtual	void	OnDestroy				();

protected:
	bool			CreateChildControls		();

protected:
	_string							m_sTitle;
	EnumerableObject<Serializable>*		m_pListObjects;

	ESChildControlList*					m_pList;
	ESChildControlImageButton*			m_pAddButton;
	ESChildControlImageButton*			m_pDeleteButton;
	ESChildControlImageButton*			m_pMoveUpButton;
	ESChildControlImageButton*			m_pMoveDownButton;

	VirtualRowSet*						m_pListRowSet;
	EnumerableObject<Serializable>*		m_pEnumerable;

// Implementation
protected:
	void			OnListRowChange			(ESChildControlList* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void			OnAdd					(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void			OnDelete				(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void			OnMoveUp				(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, int nParam = 0);
	void			OnMoveDown				(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, int nParam = 0);
};
