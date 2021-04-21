#pragma once
#include <afxmt.h>

class Logger
{
protected:
	Logger();
	

public:
	virtual ~Logger();

	static bool	CreateLogEntry		(char* pszLogEntry);
	static bool	InitLogging			(_String sLogDir, int nMaxFileLength);
	static void	DestroyLogging		();
	
protected:
	static Logger*	GetLoggerInstance(){
		static Logger log;
		return &log;
		}

	bool	InitializeLogging	(_String sLogDir, int nMaxFileLength);
	bool	WriteLogEntry		(char* pszLogEntry);

private:
	HANDLE				m_hLogFile;
	int					m_nMaxFileLength;
	int					m_nLogFileSize;
	_String				m_sLogDir;
	_String				m_sCurrentLogFileName;
	CCriticalSection	m_lock;
};
