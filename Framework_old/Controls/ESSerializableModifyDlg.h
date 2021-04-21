// ESSerializableModifyDlg.h : header file
//

#pragma once
#include "MercuryBaseWnd.h"

class ESChildControlPropertyGrid;

// ESSerializableModifyDlg dialog
class ESSerializableModifyDlg : public MercuryBaseView
{
// Construction
public:
	ESSerializableModifyDlg();

	bool			SetSerializableObject	(Serializable* pObject);
	bool			SetListOfObjects		(EnumerableObject<Serializable>* pListOfObjects);

protected:
	bool			CreateChildControls		();

protected:
	virtual void	Setup		();
	virtual	void	OnClose		();
	virtual void	OnDestroy	();

protected:
	ESChildControlPropertyGrid*			m_pGrid;
	Serializable*						m_pObject;
	EnumerableObject<Serializable>*		m_pListOfObjects;
};
