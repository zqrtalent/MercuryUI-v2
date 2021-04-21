#include "stdafx.h"
#include "Logger.h"
#include "../Path/Path.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_LOG_FILE_SIZE_DEFAULT 1024*1024 // 1MB

Logger::Logger(){
	m_hLogFile				= INVALID_HANDLE_VALUE;
	m_nMaxFileLength		= MAX_LOG_FILE_SIZE_DEFAULT; // 10MB
	m_nLogFileSize			= 0;
	} 

Logger::~Logger(){
	m_lock.Lock();	// Lock
	if( m_hLogFile != INVALID_HANDLE_VALUE )
		CloseHandle(m_hLogFile);

	m_hLogFile				= INVALID_HANDLE_VALUE;
	m_nLogFileSize			= 0;
	m_nMaxFileLength		= 0;
	m_lock.Lock();	// Unlock
	}

bool
Logger::InitLogging(_String sLogDir, int nMaxFileLength){
	Logger* pLog = Logger::GetLoggerInstance();
	if( !pLog ) return false;
	return pLog->InitializeLogging(sLogDir, nMaxFileLength);
	}

void
Logger::DestroyLogging(){
	Logger* pLog = Logger::GetLoggerInstance();
	if( pLog ){
		pLog->m_lock.Lock();	// Lock
		if( pLog->m_hLogFile != INVALID_HANDLE_VALUE )
			CloseHandle(pLog->m_hLogFile);

		pLog->m_hLogFile			= INVALID_HANDLE_VALUE;
		pLog->m_nMaxFileLength		= 0;
		pLog->m_nLogFileSize		= 0;
		pLog->m_lock.Unlock();	// Unlock
		}
	}

bool
Logger::InitializeLogging(_String sLogDir, int nMaxFileLength){
	Path::BuildFullPath(sLogDir);
	if( m_hLogFile == INVALID_HANDLE_VALUE ){
		if( sLogDir.Right(1) != _T("\\") )
			sLogDir += _T("\\");

		_String	 sLogFile;
		sLogFile.Format(_T("%scurrent.log"), sLogDir.GetBuffer());
		m_sCurrentLogFileName = _T("current.log");
		HANDLE	hFile = ::CreateFile(sLogFile.GetBuffer(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0L, NULL);;
		if( hFile != INVALID_HANDLE_VALUE ){
			if( GetFileSize(hFile, NULL) >= (DWORD)nMaxFileLength ){
				CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;

				// Rename current.log to formated name.
				DateTime	dtCurrent(true);
				_String		sLogFileNew;
				sLogFileNew.Format(_T("%s%02d-%02d-%04d %02d-%02d-%02d_%d.log"), sLogDir.GetBuffer(), dtCurrent.GetDay(), dtCurrent.GetMonth(), dtCurrent.GetYear(), 
					dtCurrent.GetHour(), dtCurrent.GetMinute(), dtCurrent.GetSecond(),  rand());
				MoveFile(sLogFile.GetBuffer(), sLogFileNew.GetBuffer());
				}
			}

		if( hFile == INVALID_HANDLE_VALUE ){
			srand((UINT)time(NULL));
			int nRetryCt			= 0;
			m_sCurrentLogFileName	= _T("current.log");
retry:
			sLogFile				= sLogDir + m_sCurrentLogFileName;

			hFile = ::CreateFile(sLogFile.GetBuffer(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_NEW, 0L, NULL);
			if( hFile == INVALID_HANDLE_VALUE && nRetryCt < 10 ){
				m_sCurrentLogFileName.Format(_T("%scurrent_%d.log"), rand());
				nRetryCt ++;
				goto retry;
				}
			}
		else{
			// Set file pointer to the end of file.
			SetFilePointer(hFile, 0, NULL, FILE_END);
			}
		
		m_nMaxFileLength	= nMaxFileLength;
		m_sLogDir			= sLogDir;
		m_hLogFile			= hFile;
		return true;
		}
	return false;
	}

bool
Logger::WriteLogEntry(char* pszLogEntry){
	DateTime dtCurrent(true);
	_String sEntryLine;
	sEntryLine.Format(_T("%02d-%02d-%04d %02d:%02d:%02d    %s\r\n"), dtCurrent.GetDay(), dtCurrent.GetMonth(), dtCurrent.GetYear(), dtCurrent.GetHour(), dtCurrent.GetMinute(), dtCurrent.GetSecond(), pszLogEntry);

	m_lock.Lock();		// Lock
	if( m_hLogFile == INVALID_HANDLE_VALUE ) {
		m_lock.Unlock();		// Lock
		return false;
		}

	if( m_nLogFileSize + sEntryLine.GetLength() > m_nMaxFileLength && m_nLogFileSize > 0 ){
		// Close file handle.
		CloseHandle(m_hLogFile);

		srand((UINT)time(NULL));
		// Rename current.log to formated name.
		_String sLogFileNameNew;
		sLogFileNameNew.Format(_T("%02d-%02d-%04d %02d-%02d-%02d_%d.log"), dtCurrent.GetDay(), dtCurrent.GetMonth(), dtCurrent.GetYear(), 
			dtCurrent.GetHour(), dtCurrent.GetMinute(), dtCurrent.GetSecond(),  rand());
		_String sLogFileNew		= m_sLogDir + sLogFileNameNew;
		_String sLogFile		= m_sLogDir + m_sCurrentLogFileName;
		MoveFile(sLogFile.GetBuffer(), sLogFileNew.GetBuffer());

		// Create current.log file.
		int nRetryCt			= 0;
		m_sCurrentLogFileName	= _T("current.log");

retry:
		sLogFile				= m_sLogDir + m_sCurrentLogFileName;
		m_nLogFileSize			= 0;

		HANDLE hFile = ::CreateFile(sLogFile.GetBuffer(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_NEW, 0L, NULL);
		if( hFile == INVALID_HANDLE_VALUE && nRetryCt < 10 ){
			m_sCurrentLogFileName.Format(_T("current_%d.log"), rand());
			nRetryCt ++;
			goto retry;
			}
		m_hLogFile = hFile;
		}

	DWORD dwWritten = 0L;
	BOOL bRet		= WriteFile(m_hLogFile, sEntryLine.GetBuffer(), sEntryLine.GetLength(), &dwWritten, NULL);
	m_nLogFileSize	+= dwWritten;

	m_lock.Unlock();	// Unlock
	return (bRet != FALSE);
	}

bool
Logger::CreateLogEntry(char* pszLogEntry){
	Logger* pLog = Logger::GetLoggerInstance();
	if( !pLog || !pszLogEntry ) return false;
	return pLog->WriteLogEntry(pszLogEntry);
	}