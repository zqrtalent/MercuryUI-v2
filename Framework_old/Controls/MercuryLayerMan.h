#pragma once
#include "../Serialize/Serializable.h"
#include "../_platformCompat/PlatformCriticalSection.h"

class MercuryBaseView;
class ESChildControl;
class MercuryLayer;

class MercuryLayerManager
{
public:
	MercuryLayerManager(MercuryBaseView* pBaseView);
	virtual ~MercuryLayerManager();

public:
	bool				Add							(ESChildControl* pChild);
	bool				Remove						(ESChildControl* pChild);
	void				RemoveAll					();

	ESChildControl*		GetTopMostChildByPoint		(_Point pt);
	int					EnumChildControlsByPoint	(_Point pt, CPtrArray& arrControls);
	int					EnumChildControlsByRect		(_Rect rc, CPtrArray& arrControls);

	void				ViewSizeChanged				(int cx, int cy);
	void				ChildSizeChanged			(ESChildControl* pChild);
	void				ChildVisibilityChanged		(ESChildControl* pChild, bool bVisible);

protected:
	MercuryLayer*		FindLayerByIndex			(int nLayerIndex, bool bCreateIfNotExists);

protected:
	MercuryBaseView*								m_pView;					// Base view control.
	AutoSortedArray									m_arrLayerByIndex;			// Layer by index.
	AutoSortedArray									m_arrExcludedControl;		// <ESChildControl*>
	CriticalSection                                 m_lock;
    void*   m_pTemp;
};

class MercuryLayer
{
public:
	MercuryLayer(int nLayerIndex);
	virtual ~MercuryLayer();

	bool				Add				(ESChildControl* pChild);
	bool				Remove			(ESChildControl* pChild);
	int					GetChildsByPoint(_Point& pt, CPtrArray& arrControls);
	int					GetChildsByRect	(_Rect& rect, CPtrArray& arrControls);
	int					GetChildCount	(){return m_arrChildControlById.GetCount();};

protected:
	int					m_nLayerIndex;
	AutoSortedArray		m_arrChildControlById;
    void* m_pTemp;
};
