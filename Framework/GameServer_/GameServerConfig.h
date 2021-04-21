#pragma once
#include "..\Serialize\Serializable.h"
#include "..\SocketLibrary\SocketServerOptions.h"
#include "..\Utility\FileModifyMonitor.h"

class GameServerConfig : public Serializable
{
public:
	GameServerConfig();
	virtual ~GameServerConfig();

public:
	SocketServerOptions			m_sockServerOpt;			// Socket server options.
	SockListenOptions			m_adminSocketOpt;			// Administration socket options.
	SockListenOptions			m_policySocketOpt;			// Policy socket options.

	int							m_nSessionExpireTimeSec;	// Session expire time in seconds.
	bool						m_bLog;						// Server log option.
	_String						m_sLogDir;					// Log directory.
	int							m_nMaxLogFileSize;			// Log file size max.
	_String						m_sAvatarsFolder;			// Avatar images folder.

protected:
	FileModifyMonitor			m_modifyMonitor;
	CCriticalSection			m_lock;

protected:
	void			OnFileModified	(HANDLE hFile, DWORD dwSizeNew);

public:
	virtual Serializable* CreateSerializableObject(){return new GameServerConfig();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};