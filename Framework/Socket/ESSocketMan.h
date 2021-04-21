#pragma once

#ifndef _WINSOCK2API_
	#include <winsock2.h>
#endif

#ifndef __AFXMT_H__
	#include <afxmt.h>
#endif

#include "..\Array\StringToStringArray.h"
#include "..\Array\List.h"

/////////////////////////////////////////
//	class: ESSocketMan
//	programmer:	ZqrTalent
//	Date:	30/6/2009
/////////////////////////////////////////

class ESSocket;

class ESSocketMan
{
friend class ESSocket;

protected:
	ESSocketMan();
	~ESSocketMan();

public:
	static ESSocketMan* GetInstance(){
		static ESSocketMan man;
		return &man;
		}

	ESSocket*	Listen				(short shPort, CString sIpAddress = _T(""));
	ESSocket*	Connect				(short shPort, CString sIpAddress = _T("127.0.0.1"));

public:
	void		OnReceiveData		(ESSocket* pSockSrc, BYTE* lpData, UINT nSize);
	BOOL		OnAccept			(ESSocket* pSockSrc, SOCKET sockAccept);

	BOOL		DestroySocket		(ESSocket* pSock);
	void		DestroyAllSockets	();

protected:
	List<ESSocket>	m_listListenSock;
	List<ESSocket>	m_listReadWriteSock;
};
