#pragma once


class DataRow
{
protected:
	DataRow();
	~DataRow();

public:
	// Navigation methods.
	virtual BOOL			MoveNext();
	virtual BOOL			MovePrev();
	virtual BOOL			MoveFirst();
	virtual BOOL			MoveLast();
	virtual BOOL			MoveTo(int nPos);

	// Refresh record.
	virtual BOOL			RefreshCurrent();
	// Retrieves refreshed record count.
	virtual int				RefreshByField(int nField, CString sValue);
	// Retrieves refreshed record count.
	virtual int				RefreshByField(int nField, long lValue);

	// Update record.
	virtual BOOL			UpdateCurrent();
	// Retrieves updated record count.
	virtual int				UpdateByField(int nField, CString sValue);
	// Retrieves updated record count.
	virtual int				UpdateByField(int nField, long lValue);

	// Add new record.
	virtual BOOL			AddNew();

	// Delete record.
	virtual BOOL			DeleteCurrent();
	// Retrieves status.
	virtual BOOL			DeleteByField(int nField, CString sValue);
	// Retrieves status.
	virtual int				DeleteByField(int nField, long lValue);

	// Field value manipulation methods.
	virtual BOOL			GetString(int nField, CString& sValue);
	virtual CString			GetString(int nField);
	virtual BOOL			GetString(TCHAR* pszFieldName, CString& sValue);
	virtual CString			GetString(TCHAR* pszFieldName);
	virtual BOOL			SetString(int nField, CString sValueNew);
	virtual BOOL			SetString(TCHAR* pszFieldName, CString sValueNew);

	virtual BOOL			GetShort(int nField, short& shValue);
	virtual short			GetShort(int nField);
	virtual BOOL			GetShort(TCHAR* pszFieldName, short& shValue);
	virtual short			GetShort(TCHAR* pszFieldName);
	virtual BOOL			SetShort(TCHAR* pszFieldName, short shValueNew);
	virtual BOOL			SetShort(int nField, short shValueNew);

	virtual BOOL			GetLong(int nField, long& lValue);
	virtual long			GetLong(int nField);
	virtual BOOL			GetLong(TCHAR* pszFieldName, long& lValue);
	virtual long			GetLong(TCHAR* pszFieldName);
	virtual BOOL			SetLong(TCHAR* pszFieldName, long lValueNew);
	virtual BOOL			SetLong(int nField, long lValueNew);

	virtual BOOL			GetBigint(int nField, __int64& nValue);
	virtual __int64			GetBigint(int nField);
	virtual BOOL			GetBigint(TCHAR* pszFieldName, __int64& nValue);
	virtual __int64			GetBigint(TCHAR* pszFieldName);
	virtual BOOL			SetBigint(TCHAR* pszFieldName, __int64 nValueNew);
	virtual BOOL			SetBigint(int nField, __int64 nValueNew);

	virtual BOOL			GetDouble(int nField, double& dValue);
	virtual double			GetDouble(int nField);
	virtual BOOL			GetDouble(TCHAR* pszFieldName, double& dValue);
	virtual double			GetDouble(TCHAR* pszFieldName);
	virtual BOOL			SetDouble(TCHAR* pszFieldName, double dValueNew);
	virtual BOOL			SetDouble(int nField, double dValueNew);

	virtual BOOL			GetFloat(int nField, float& fValue);
	virtual float			GetFloat(int nField);
	virtual BOOL			GetFloat(TCHAR* pszFieldName, float& fValue);
	virtual float			GetFloat(TCHAR* pszFieldName);
	virtual BOOL			SetFloat(TCHAR* pszFieldName, float fValueNew);
	virtual BOOL			SetFloat(int nField, float fValueNew);

	virtual BOOL			GetDate(int nField, DATE& dtValue);
	virtual DATE			GetDate(int nField);
	virtual BOOL			GetDate(TCHAR* pszFieldName, DATE& dtValue);
	virtual DATE			GetDate(TCHAR* pszFieldName);
	virtual BOOL			SetDate(TCHAR* pszFieldName, DATE dtValue);
	virtual BOOL			SetDate(int nField, DATE dtValue);

	virtual BOOL			GetTime(int nField, CTime& tmValue);
	virtual CTime			GetTime(int nField);
	virtual BOOL			GetTime(TCHAR* pszFieldName, CTime& tmValue);
	virtual CTime			GetTime(TCHAR* pszFieldName);
	virtual BOOL			SetTime(TCHAR* pszFieldName, CTime tmValue);
	virtual BOOL			SetTime(int nField, CTime tmValue);

	virtual BOOL			GetDateTime(int nField, COleDateTime& dtmValue);
	virtual COleDateTime	GetDateTime(int nField);
	virtual BOOL			GetDateTime(TCHAR* pszFieldName, COleDateTime& dtmValue);
	virtual COleDateTime	GetDateTime(TCHAR* pszFieldName);
	virtual BOOL			SetDateTime(TCHAR* pszFieldName, COleDateTime dtmValue);
	virtual BOOL			SetDateTime(int nField, COleDateTime dtmValue);

	protected:
		CString	m_sDSName; // DataSet name.
};