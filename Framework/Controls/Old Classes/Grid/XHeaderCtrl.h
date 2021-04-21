
#define DT_DEFAULT					((UINT)-1)		//+++
#define FLATHEADER_TEXT_MAX			80
#define XHEADERCTRL_NO_IMAGE		0
#define XHEADERCTRL_UNCHECKED_IMAGE	1
#define XHEADERCTRL_CHECKED_IMAGE	2

#define FH_PROPERTY_SPACING			1
#define FH_PROPERTY_ARROW			2
#define FH_PROPERTY_STATICBORDER	3
#define FH_PROPERTY_DONTDROPCURSOR	4
#define FH_PROPERTY_DROPTARGET		5

#define SORT_SIGN_WIDTH				12
#define SORT_SIGN_HEIGHT			6
#define SORT_SIGN_RIGHT_MARGIN		2

///////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl window

class CXHeaderCtrl : public CHeaderCtrl
{
    DECLARE_DYNCREATE(CXHeaderCtrl)

// Construction
public:
	CXHeaderCtrl();
	virtual ~CXHeaderCtrl();

// Attributes
public:
	void		SetListCtrl			(CListCtrl *pListCtrl) { m_pListCtrl = pListCtrl; }
	BOOL		ModifyProperty		(WPARAM wParam, LPARAM lParam);
	int			GetSpacing			() { return m_iSpacing; }
	void		SetSpacing			(int nSpacing) { m_iSpacing = nSpacing; }
	UINT		GetAlignment		() { return m_nFormat; }
	void		SetAlignment		(UINT nFormat) { m_nFormat = nFormat; }
	COLORREF	GetTextColor		() { return m_rgbText; }
	void		SetTextColor		(COLORREF rgbText) { m_rgbText = rgbText; }
	BOOL		GetDividerLines		() { return m_bDividerLines; }
	void		EnableDividerLines	(BOOL bEnable) { m_bDividerLines = bEnable; }
	void		SetGradient			(COLORREF cr1, COLORREF cr2);

	BOOL		DrawSortSign		(CDC* pDC, CRect rect, BOOL bAsc, COLORREF cr);

// Overrides
public:
	virtual void	DrawItem		(LPDRAWITEMSTRUCT);
	virtual void	DrawItem		(CDC* pDC, CRect rect, LPHDITEM lphdi);
	virtual	BOOL	GetColCheckState(int nCol, BOOL& bState){ return FALSE; };
	virtual BOOL	GetColSortStatus(int nCol, BOOL& bState){ return FALSE; };

// Implementation
protected:
	CListCtrl*	m_pListCtrl;
	UINT		m_nFormat;
	COLORREF	m_rgbText;
	BOOL		m_bDividerLines;
	BOOL		m_bDoubleBuffer;
	BOOL		m_bStaticBorder;
	BOOL		m_bResizing;
	int			m_iSpacing;
	SIZE		m_sizeImage;
	SIZE		m_sizeArrow;
	UINT		m_nDontDropCursor;
	UINT		m_nClickFlags;
	CPoint		m_ptClickPoint;

	COLORREF	m_cr3DHighLight;
	COLORREF	m_cr3DShadow;
	COLORREF	m_cr3DFace;
	COLORREF	m_crBtnText;
	
	COLORREF	m_crGradient1;
	COLORREF	m_crGradient2;
	BOOL		m_bGradient;


	HBITMAP		m_hBmpCheckOn;
	HBITMAP		m_hBmpCheckOff;

	void	DrawCtrl	(CDC* pDC);
	int		DrawImage	(CDC* pDC, CRect rect, LPHDITEM hdi, BOOL bRight);
	int		DrawBitmap	(CDC* pDC, CRect rect, LPHDITEM hdi, CBitmap* pBitmap, BITMAP* pBitmapInfo, BOOL bRight);
	int		DrawText	(CDC* pDC, CRect rect, LPHDITEM lphdi);
	

// Generated message map functions
protected:
	afx_msg BOOL	OnEraseBkgnd	(CDC* pDC);
	afx_msg void	OnPaint			();
	afx_msg void	OnSysColorChange();
	afx_msg void	OnLButtonDblClk	(UINT nFlags, CPoint point);
	afx_msg LRESULT OnDeleteItem	(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnInsertItem	(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnLayout		(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetImageList	(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

__inline void
CXHeaderCtrl::SetGradient(COLORREF cr1, COLORREF cr2)
	{
	m_crGradient1	= cr1;
	m_crGradient2	= cr2;
	m_bGradient		= cr1 != cr2 ? TRUE : FALSE;
	}
