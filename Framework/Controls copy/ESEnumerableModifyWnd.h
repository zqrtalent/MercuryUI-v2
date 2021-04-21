#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESEnumerableModifyWnd window

#include "MercuryBaseWnd.h"

class ESChildControlImageButton;
class ESChildControlList;
class VirtualRowSet;

class ESEnumerableModifyWnd : public MercuryBaseView
{
// Construction
public:
	ESEnumerableModifyWnd();
	virtual ~ESEnumerableModifyWnd();

	friend class ESSerializableModifyDlg;
	
public: // Operations.
	void					SetEnumerableObject	(EnumerableObject<Serializable>* pEnumerable);

protected:
	virtual bool			AttachToWindow		(ESFrameBase* pBaseFrameWnd);
	virtual void			OnPaintClient		(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual BOOL			UseDoubleBufferedDC	(){return TRUE;};
	virtual void			OnPrePaint			(_Rect& rcRepaint);

protected:
	bool					CreateChildControls	();


protected:
	ESChildControlList*					m_pList;
	ESChildControlImageButton*			m_pAddButton;
	ESChildControlImageButton*			m_pDeleteButton;
	ESChildControlImageButton*			m_pMoveUpButton;
	ESChildControlImageButton*			m_pMoveDownButton;

	VirtualRowSet*						m_pListRowSet;
	EnumerableObject<Serializable>*		m_pEnumerable;

	// Child control events.
protected:
	void			OnListRowChange		(ESChildControlList* pSender, ChildControlEventCode eventCode, void* lpParam = 0);
	void			OnAdd				(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam = 0);
	void			OnDelete			(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam = 0);
	void			OnMoveUp			(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam = 0);
	void			OnMoveDown			(ESChildControlImageButton* pSender, ChildControlEventCode eventCode, void* lpParam = 0);
};