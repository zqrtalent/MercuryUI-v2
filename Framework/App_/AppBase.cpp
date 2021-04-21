#include "StdAfx.h"
#include "AppBase.h"

AppBase::AppBase(void)
	{
	}

AppBase::~AppBase(void)
	{
	}

BOOL 
AppBase::InitInstance()
	{
	BOOL bRet = CWinApp::InitInstance();
	return bRet;
	}

int
AppBase::ExitInstance()
	{
	int nRet = CWinApp::ExitInstance();
	return nRet;
	}
