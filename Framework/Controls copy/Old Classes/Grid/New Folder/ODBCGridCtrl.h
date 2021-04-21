
#pragma once
#include "GridCtrl\GridCtrl.h"

class AutoSortedArray;
class iODBCRst;
class CRecord;

//
//	class ODBCGridCtrl
//

typedef struct ODBCGridCol_tag
	{
	CString				sFieldName;			// Specifies field name.
	int					nFieldID;			// Specifies field index.
	int					nAlign;				// Specifies column align.
	BOOL				bRowIndex;			// Indicates if this column is row index column.
	int					nDecimalPlaces;		// Decimal places.
	PROC				pGetAsTextFunc;		// Function to retrieve field as text.
	int					nPrecision;
	int					nScale;
	ODBCGridCol_tag*	pNextCol;			// Next DBColumn pointer.
	} ODBCGridCol, *PODBCGridCol;

typedef void (*GetFieldAsText)		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);

class ODBCGridCtrl: public CGridCtrl
{
	// < Constructor/Destructor >
public:
	ODBCGridCtrl();

	// < Public methods >
public:
	void			SetRowId			(const CString sUIDField);
	iODBCRst*		SetRst				(iODBCRst* pRst);
	void			InitCommands		(UINT nCmdDelete, UINT nCmdAddNew);
	void			DefineColumn		(int nIndex, LPCTSTR lpszTitle, LPCTSTR lpszDBField, int nAlignment, int nWidth);
	BOOL			Load				();
	BOOL			ReLoad				(CStringArray* pArrFields = NULL);
	CString			GetLastError		();
	BOOL			IsCmdHandled		(UINT nCode, UINT nCmdId);
	void			DetachOnDestroy		(BOOL bDetach, CWnd* pWndAttach);


	// < Protected methods >
protected:
	virtual bool	OnGetCellColors		(IN int Row, IN int Col, OUT COLORREF& TextColor, OUT COLORREF& BkColor, OUT UINT8& TextStyle) const;
	virtual bool	OnChooseGridFont	(OUT LOGFONT* plf);
	virtual void	OnSetup				();
	virtual bool	OnGetCellText		(IN int Row, IN int Col, OUT CString& strCellText, IN LONG lRowParam);
	virtual bool	OnGetCellTextAlign	(IN int Row, IN int Col, OUT int& nAlign);
	virtual void	OnDestroy			();

	virtual BOOL	OnDeleteRecord		(CRecord* pRecDelete);
	virtual	BOOL	OnAddNewRecord		(CRecord* pNewRec	);

	// < Private methods >
private:
	void			AdjustBrightness		(IN OUT COLORREF& Color, IN INT16 Amount) const;
	void			BuildODBCColumnsArray	();
	void			FreeRecordDataArray		();

	// Functions to retrieve field value as text. ####################################################
	static void		GetBool_FieldAsText		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetInt_FieldAsText		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDouble_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDateTime_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDate_FieldAsText		(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetString_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	static void		GetDecimal_FieldAsText	(ODBCGridCol* pDBCol, CString& sFieldText, CRecord* pRec);
	// ###############################################################################################


	// < Protected attributes >
protected:

	CString				m_sSelectSQL;		// Select SQL query.
	CString				m_sUIDField;		// Unique field name.
	int					m_nUIDField;		// Unique field id.
	iODBCRst*			m_pRstSelect;		// Select recordset.
	CRecord*			m_pRecord;			// Record object pointer.
	AutoSortedArray*	m_pArrRecDataById;	// Array record UID by Record data pointer.
	ODBCGridCol*		m_pCols;			// Chain of ODBCGridCol structures.
	ODBCGridCol**		m_pColByIndex;		// Array of ODBCGridCol structure pointers used currently.
	long				m_nNumberCols;		// Number of columns.
	CFont*				m_pFont;			// Font object for list control.
	BOOL				m_bDetach;
	CWnd*				m_pWndAttach;
	CString				m_strLastError;
	
	// < Protected message handlers >
protected:

	afx_msg void	OnDelete		();
	afx_msg void	OnAddNew		();
	afx_msg void	OnUpdateDelete	(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateAddNew	(CCmdUI* pCmdUI);
	afx_msg void	OnItemChanged	(LPNMHDR pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

__inline CString
ODBCGridCtrl::GetLastError()
	{
	return m_strLastError;
	}