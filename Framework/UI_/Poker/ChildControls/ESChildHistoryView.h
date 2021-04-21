#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildHistoryView window

#include "..\..\..\PokerServer\Object\PokerHandHistory.h"
#include "..\..\..\Controls\ESChildScrollView.h"

class ESChildHistoryView : public ESChildScrollView
{
// Construction
public:
	ESChildHistoryView();
	virtual ~ESChildHistoryView();

	enum HistoryItemType { HistoryItemHeader,  HistoryItemPlayer, HistoryItemSeparator, HistoryItemEmpty};

	virtual bool	Create					(int nId, CRect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool	SetPos					(int x, int y, bool bRedraw = true);
	virtual bool	SetSize					(int cx, int cy, bool bRedraw = true);
	virtual bool	Destroy					();
	virtual void	OwnerWindowSizeChanged	(int cx, int cy);

	int				AddHistoryInfo			(PokerSimpleHistory* pHistoryInfo, bool bClearExisting = true, bool bRedraw = true);
	bool			AddHistoryItem			(PokerSimpleHistoryItemHeader* pItemHearder, bool bRedraw = true);

protected:
	virtual void	OnPaintClient		(CDC* pDC, CRect* pRectDC, CRect* pRectClient);

protected:
	bool			RenderSmallCard		(int nCard, CDC* pDC, CRect* pRectDC, int& xPos, CRect rcItemDC);
	bool			RenderText			(CDC* pDC, CRect* pRectDC, CRect rcItemDC, CString sText, COLORREF crText, int& xPos);

protected:
	COLORREF			m_crRow1;
	COLORREF			m_crRow2;

	PokerSimpleHistory	m_history;		// Simple history holder class object.
	AutoSortedArray		m_arrItems;		// History items. <PokerSimpleHistoryItemHeader*, PokerSimpleHistoryItem*><HistoryItemType>
	int					m_nLimitItems;	// Limit of history items.


};