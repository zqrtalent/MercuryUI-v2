#pragma once

/////////////////////////////////////////////////////////////////////////////
// ESSerializableModifyWnd window

#include "MercuryBaseWnd.h"

class ESSerializableModifyWnd : public MercuryBaseView
{
// Construction
public:
	ESSerializableModifyWnd();
	virtual ~ESSerializableModifyWnd();

	friend class ESSerializableModifyDlg;
	
public: // Operations.
	void					SetSerializableObject	(Serializable* pObject);
	void					SetListOfObjects		(EnumerableObject<Serializable>* pListOfObjects);

protected:
	virtual bool			AttachToWindow		(ESFrameBase* pBaseFrameWnd);
	virtual void			OnPaintClient		(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual BOOL			UseDoubleBufferedDC	(){return TRUE;};
	virtual void			OnPrePaint			(_Rect& rcRepaint);

protected:
	bool					CreateChildControls	();

	// Child control events.
protected:
	
};