#include "stdafx.h"
#include "..\Utility\GrowableMemory.h"
#include "..\String\StringDecoder.h"
#include "..\Log\Logger.h"
#include "GameServerConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
///
///	class GameServerConfig
///
///////////////////////////////////////////////////////////

GameServerConfig::GameServerConfig(){
	ZeroInit();
	}

BEGIN_DECLARE_VAR(GameServerConfig, Serializable)
	DECLARE_VAR(_T("GameServerConfig"),		_T(""), VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("SessionExpireTimeSec"),	_T(""), VariableType::VT_Int32, offsetof(GameServerConfig,m_nSessionExpireTimeSec), false, false, false)
	DECLARE_VAR(_T("Log"),					_T(""), VariableType::VT_Bool, offsetof(GameServerConfig,m_bLog), false, false, false)
	DECLARE_VAR(_T("LogDir"),				_T(""), VariableType::VT_String, offsetof(GameServerConfig,m_sLogDir), false, false, false)
	DECLARE_VAR(_T("MaxLogFileSize"),		_T(""), VariableType::VT_Int32, offsetof(GameServerConfig,m_nMaxLogFileSize), false, false, false)
	DECLARE_VAR(_T("AvatarsFolder"),		_T(""), VariableType::VT_String, offsetof(GameServerConfig,m_sAvatarsFolder), false, false, false)
	DECLARE_VAR(_T("SocketServer"),			_T(""), VariableType::VT_None, offsetof(GameServerConfig,m_sockServerOpt), false, true, false)
	DECLARE_VAR(_T("AdminSocketOpt"),		_T(""), VariableType::VT_None, offsetof(GameServerConfig,m_adminSocketOpt), false, true, false)
	DECLARE_VAR(_T("PolicySocketOpt"),		_T(""), VariableType::VT_None, offsetof(GameServerConfig,m_policySocketOpt), false, true, false)
END_DECLARE_VAR()

GameServerConfig::~GameServerConfig(){
	}

void
GameServerConfig::OnFileModified(HANDLE hFile, DWORD dwSizeNew){
	DWORD dwRead = 0;
	char* pszBuffer = new char[dwSizeNew + 1];
	pszBuffer[dwSizeNew] = '\0';

	if( ReadFile(hFile, pszBuffer, dwSizeNew, &dwRead, NULL) || dwRead == dwSizeNew ){
		_String sXML = pszBuffer;
		delete [] pszBuffer;
		DeserializeAsXML(&sXML);
		}
	else
		delete [] pszBuffer;
	}