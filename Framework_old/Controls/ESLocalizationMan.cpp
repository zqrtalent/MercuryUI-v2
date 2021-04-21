
#include "ESLocalizationMan.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"
#include "../Utility/StringHelper.h"
#include "MercuryBaseWnd.h"


//////////////////////////////////////////////////////
//
//	class ESLocalizationMan
//
/////////////////////////////////////////////////////
ESLocalizationMan::ESLocalizationMan(){
	}

ESLocalizationMan::~ESLocalizationMan(){
	}

bool
ESLocalizationMan::Load(_string sFile, bool bNotifyAllWindows){
	_string sLangFile = sFile;
	if( !sLangFile.length() ) 
		return false;
	Path::BuildFullPath(sLangFile);

	if( !m_sFile.compare(sFile) )
		return true; // Already loaded.

	BYTE*	lpBuffer	= NULL;
	int		nSize		= 0;
	if( !Path::ReadFileContent(sLangFile, lpBuffer, nSize) || nSize <= 0 )
		return false;

	/*
	if( _access(sLangFile.GetBuffer(), 0) )
		return false; // File not exists.

	CFile file(sLangFile.GetBuffer(), CFile::modeRead|CFile::shareDenyWrite);
	DWORD dwFileSize = (DWORD)file.GetLength();
	if( dwFileSize > 0x6400000  ) // 100MB
		return false; // File size is more than allowed to be.

	GrowableMemory mem;
	mem.SetBufferSize		((int)dwFileSize);
	mem.SetUsedBufferSize	((int)dwFileSize);

	char* pszBuffer = new char[((int)dwFileSize) + 1];
	pszBuffer[((int)dwFileSize)] = '\0';

	DWORD dwRead = dwFileSize;
	if( file.Read(pszBuffer, dwRead) != dwRead ){
		delete [] pszBuffer;
		return false; // Read file error.
		}
*/

	m_sFile = sFile;
	m_arrKeyValues.DeleteAll();
    
#ifndef _MBCS
    wchar_t wszKey[128];
    wchar_t wszValue[256];
#endif

	int nOffset = 0, nCt = (int)nSize;
	char* pszBuffer = (char*)lpBuffer;
	// Is UTF8 signature.
	if( pszBuffer[0] == '\xEF' && pszBuffer[1] == '\xBB' && pszBuffer[2] == '\xBF' )
		nOffset = 3;

	// Load strings.
	char szLine[512];
	int nStartOfLine		= nOffset;
	int nEndOfLine			= nOffset;
	while( nOffset < nCt  ){
		if( pszBuffer[nOffset] == '\r' && pszBuffer[nOffset + 1] == '\n' ){
			nEndOfLine		= nOffset;
			if( nStartOfLine < nEndOfLine ){
				int nLineLen = (nEndOfLine - nStartOfLine);
				szLine[nLineLen] = '\0';
				memcpy(szLine, &pszBuffer[nStartOfLine], nLineLen);

				std::string sLine = szLine, sKey, sValue;
				int nFind = (int)sLine.find('=', 0);
				if( nFind != -1 ){
					sKey = sLine.substr(0, nFind);
//					sKey = stringEx::TrimLeft(sKey);
//					sKey = stringEx::TrimRight(sKey);

					sValue = sLine.substr(nFind + 1, sLine.length() - nFind - 1);
//					sValue = stringEx::TrimLeft(sValue);
//					sValue = stringEx::TrimRight(sValue);

					int nCt = (int)sValue.length();
					for(int i=0; i<(nCt - 1); i++){
						char cSymbol1 = sValue[i];
						char cSymbol2 = sValue[i+1];

						if( cSymbol1 == '\\' ){
							if( cSymbol2 == 'r' ){
								sValue[i] = ' \r';
								sValue.erase(i+1, 1);
								nCt --;
								}
							else
								if( cSymbol2 == 'n' ){
									sValue[i] = '\n';
									sValue.erase(i+1, 1);
									nCt --;
									}
							}
						}
                    
#ifdef _MBCS
                    stringEx::MakeLower(sKey);
					m_arrKeyValues.Add(sKey, sValue);
#else
                    _string sValueW, sKeyW;
                    int nLenKey = StringHelper::UTF8ToUnicode(sKey.c_str(), wszKey, 127);
                    int nLenValue = StringHelper::UTF8ToUnicode(sValue.c_str(), wszValue, 254);
                    stringEx::MakeLower(sKeyW);
					m_arrKeyValues.Add(sKeyW, sValueW);
#endif
					
					}
				}

			nStartOfLine	= (nOffset + 2);
			nEndOfLine		= nStartOfLine;
			}

		nOffset ++;
		}

	_string sFilePath = m_sFile;
	int nFind = sFilePath.rfind('\\');
	if( nFind != -1 )
		m_sLoadedFileName = stringEx::Right(sFilePath, sFilePath.length() - nFind - 1);
	else
		m_sLoadedFileName = sFilePath;

	if( bNotifyAllWindows ){
		#pragma message ("ESLocalizationMan::Load  notify to all windows mechanism is not implemented!!!")
		/*
		ESFrameBase* pWindow = (ESFrameBase*)AfxGetMainWnd();
		if( pWindow ){
			while( pWindow ){
				if( pWindow->IsKindOf(RUNTIME_CLASS(ESFrameBase)) ){
					MercuryBaseView* pView = pWindow->GetMainWindow();
					if( pView ){
						pView->LocalizeStrings	();
						pView->Invalidate		();
						}
					}
				pWindow = (ESFrameBase*)pWindow->GetWindow(GW_HWNDNEXT);
				}
			}*/
		}

	delete [] lpBuffer;
	return true;
	}

bool
ESLocalizationMan::ParseText(_string& sText){
	ESLocalizationMan*		pMan			= ESLocalizationMan::GetInstance();
	StringToStringArray*	pArrKeyValues	= &pMan->m_arrKeyValues;

	_string sKey, sValue;
	int nFind	= 0, nStart, nEnd;
	while( nFind != -1 ){
		nStart	= (int)sText.find(_T("{{"), nFind);
		if( nStart == -1 ) break;
		nEnd	= (int)sText.find(_T("}}"), nFind);
		if( nEnd == -1 ) break;

		sKey = sText.substr(nStart, (nEnd + 2) - nStart);
		stringEx::MakeLower(sKey);
        sValue = pArrKeyValues->GetValue(sKey);
		
		if( sValue.length() ){
			sText.erase(nStart, sKey.length());
			sText.insert(nStart, sValue);
			nFind = nStart + (int)sValue.length() + 1;
			}
		else
			nFind  = nEnd + 2;
		}
	return true;
	}