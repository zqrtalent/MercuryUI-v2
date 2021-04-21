#pragma once

/////////////////////////////////////////
//	class: CSocketListen
//	programmer:	ZqrTalent
//	Date:	30/6/2009
/////////////////////////////////////////

class ESSocket;
class ESDBXMLRequestMan;
class CDatabase;

#define ESDBXMLServer_PORT 1112

class ESDBXMLServer
{
protected:
	ESDBXMLServer();
	~ESDBXMLServer();

	enum Instance{
		Unknown = -1,
		Server = 0,
		Client = 1
	};

public:
	static ESDBXMLServer* GetServerInstance(){
		return GetInstance(Server); 
		}

	static ESDBXMLServer* GetClientInstance(){
		return GetInstance(Client); 
		}

	static ESDBXMLServer* GetInstance(Instance inst = Server){
		static ESDBXMLServer server[2];
		if( inst == Server ){
			if( server[0].m_instanceType == Unknown )
				server[0].m_instanceType = Server;
			return &server[0];
			}
		else
		if( inst == Client ){
			if( server[1].m_instanceType == Unknown )
				server[1].m_instanceType = Client;
			return &server[1];
			}
		return NULL;
		}

public: // Server side methods.
	bool	Start				();
	bool	Stop				();
	bool	Restart				();

public:	// Client side methods.
	bool	OpenDatabase		(const CString sDbName, const CString sConnString, CString& sErrorString);
	bool	CloseDatabase		(const CString sDbName, CString& sErrorString);
	bool	QuerySQL			(const CString sDbName, const CString sQuerySQL, CString& sErrorString);
	bool	ExecuteSQL			(const CString sDbName, const CString sExecuteSQL, CString& sErrorString);

	bool	Connect				();	// Connect to server
	bool	Disconnect			();	// Disconnect from server.
	bool	IsConnected			();

public:
	ESDBXMLRequestMan*	GetRequestMan(){return m_pRequestMan;};

protected:
	void	Destroy				();

	// Notify methods.
	void	OnAcceptConnection	(ESSocket* pAccepted, BOOL& bAccept);
	void	OnReceiveRequest	(ESSocket* pSockFrom, BYTE* lpData, UINT nSize);
	void	OnReceiveReply		(ESSocket* pSockFrom, BYTE* lpData, UINT nSize);
	void	OnDisconnected		(ESSocket* pSock);


protected:

protected:
	ESSocket*			m_pSockListen;			// Port listened socket. 
	ESSocket*			m_pSockConnect;			// Connected socket.
	short				m_shPort;				// DBXML server port.
	CString				m_sIpAddress;
	Instance			m_instanceType;
	ESDBXMLRequestMan*	m_pRequestMan;
	DWORD				m_dwMainThreadId;		// Main process thread id.
};
