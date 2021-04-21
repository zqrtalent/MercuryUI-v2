#pragma once
#include "afxwin.h"



class AppBase : public CWinApp
{
public:
	AppBase(void);
	virtual ~AppBase(void);


public:
	virtual BOOL InitInstance();
	virtual int	 ExitInstance(); // return app exit code

};
