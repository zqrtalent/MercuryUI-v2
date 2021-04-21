#pragma once
#include "TaskInfo.h"
#include "TaskParamObj.h"
#include <afxmt.h>

class TaskRunner
{
protected:
	TaskRunner();
	virtual ~TaskRunner();

public:
	bool	Start					(int nWorkerThreadCt = 1);
	void	Stop					();
	bool	IsStarted				(){return (m_hIOCP != NULL);};

	// Register task method. Perform it before start TaskRunner.
	int		RegisterTask			(const CString sName, void* pClassObj, TaskHandlerMethod method);
	bool	PostTask				(const CString sTaskName, TaskParamObj* pParam);
	bool	PostTask				(int nTaskId, TaskParamObj* pParam);

protected:
	bool	GetQueuedTask			(TaskParamObj** pTaskParams);
	void	PostTerminationSignal	();
	bool	AssociateSocketHandle	(SOCKET sock, ULONG_PTR keyObject);

	static unsigned __stdcall		_MainThreadProc		(void* pIOMan);
	void							DoMainThreadJob		();

private:
	HANDLE*						m_phThread;
	DWORD*						m_pdwThreadIds;
	HANDLE						m_hStopEvent;
	UINT						m_nThreadCt;

	CCriticalSection			m_lock;
	EnumerableObject<TaskInfo>	m_listTask;

	HANDLE						m_hIOCP;	// IO completition port handle.
	class DefaultKey{
		public:
			SOCKET _sock;
		};
	DefaultKey*		m_pKey;	
};

