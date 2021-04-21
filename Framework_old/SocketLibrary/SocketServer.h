#pragma once

#include "SocketServerEventHandler.h"
#include "..\Array\List.h"
#include "..\Array\AutoSortedArrayInt64.h"
#include "..\Array\AutoSortedArray.h"

#include "SocketLibraryDef.h"
#include "SocketConnectionListener.h"
#include "SocketContext.h"
#include "SSLContext.h"

class SocketServer : public SocketIOEventHandler
{
public:
	SocketServer			();
	virtual ~SocketServer	();

	friend SocketConnectionListener;
	friend SocketContext;
	friend class SocketServerContext;

public:
	virtual bool					Start				(SockListenOptions* pInfo, SocketServerEventHandler* pHandler, LPCTSTR lpszPemFile /*= NULL*/);
	virtual bool					Stop				();

	void							SetPolicyXML		(const std::string sPolicyXML){m_sPolicyXML = sPolicyXML;};
	int								GetConnectionCt		(){ return m_nConnectionCt; };
	ConnectedSocketInfo*			GetConnectionInfos	(int& nCt);
	static bool						GetIpAddress		(SOCKET sock, std::string& sIpLocal, int& nPortLocal, std::string& sIpRemote, int& nPortRemote, DWORD* pIpRemote = NULL);

protected:
	static AutoSortedArray* GetArrSocketServers(){
		static AutoSortedArray _arrSocketServers;
		return &_arrSocketServers;
		};

	static int AddSocketServer(SocketServer* pSockServer){
		AutoSortedArray* pArrSocketServer = GetArrSocketServers();
		if( pArrSocketServer->IndexOf((void*)pSockServer) == -1 )
			pArrSocketServer->Add((void*)pSockServer);
		return pArrSocketServer->GetCount();
		};

	static int RemoveSocketServer(SocketServer* pSockServer){
		AutoSortedArray* pArrSocketServer = GetArrSocketServers();
		int nIndex = pArrSocketServer->IndexOf((void*)pSockServer);
		if( nIndex != -1 )
			pArrSocketServer->Delete(nIndex);
		return pArrSocketServer->GetCount();
		};

	void								AddConnectedSocketInfo		(SOCKET sock, ConnectedSocketInfo* pSockInfo);
	bool								DeleteConnectedSocketInfo	(SOCKET sock);

protected: // Events
	virtual void						OnAcceptConnection			(SOCKET sock, const std::string sRemoteIp, int nRemotePort, bool& bAccept);
	virtual void						OnConnected					(SOCKET sock, ConnectedSocketInfo* pSockInfo);
	virtual void						OnDataReceived				(SocketContext* pConn, BYTE* pData, DWORD dwSize, bool bIsFirstData);
	virtual void						OnDisconnected				(SocketContext* pConn, DWORD dwLastError);

private:
	bool								m_bStarted;						// Server status.
	SocketConnectionListener*			m_pConnListener;
	SocketServerEventHandler*			m_pHandler;						// Event handler.
	SockListenOptions					m_options;
	CriticalSection						m_lock;

	std::string							m_sPolicyXML;					// Policy xml content.

	SSLContext*							m_pSSL_CTX;
	AutoSortedArray						m_arrSocketInfoBySocket;		// <SOCKET><ConnectedSocketInfo*>
	CriticalSection						m_lockListOfSockets;
	volatile int						m_nConnectionCt;
};
