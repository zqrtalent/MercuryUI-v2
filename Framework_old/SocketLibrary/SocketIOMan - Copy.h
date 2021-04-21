#pragma once

class SocketIOEventHandler
{
public:
	SocketIOEventHandler(){};
	virtual ~SocketIOEventHandler(){};

public:
	virtual void		OnDisconnect	(DWORD dwLastError){};
	virtual void		OnDataReceived	(DWORD dwSize){};
	virtual void		OnDataSent		(DWORD dwSize){};
};

class SocketContext;
class SocketIOMan
{
protected:
	SocketIOMan();
	virtual ~SocketIOMan();

public:
	static SocketIOMan* GetInstance(){
		static SocketIOMan ioMan;
		return &ioMan;
		}

	bool		Start(UINT nWorkerThreadCt = 1);
	void		Stop();

protected:
	static unsigned __stdcall	 _MainThreadProc(void* pIOMan);
	void						DoMainThreadJob	();

private:
	HANDLE*					m_phThread;
	DWORD*					m_pdwThreadIds;
	HANDLE					m_hStopEvent;
	UINT					m_nThreadCt;
};

