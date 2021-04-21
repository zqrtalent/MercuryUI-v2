//// ESDialogBase.cpp : implementation file
//

#include "stdafx.h"
#include "ESDialogBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESDialogBase

ESDialogBase::ESDialogBase(){
	}

ESDialogBase::~ESDialogBase(){
	}

bool
ESDialogBase::Create(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent){
	bool bRet = ESChildControl::Create(nId, rcRect, pOwner, pParent);
	if( bRet ){
		}
	return bRet;
	}

bool
ESDialogBase::SetPos(int x, int y, bool bRedraw){
	ESChildControl::SetPos(x, y, bRedraw);
	return true;
	}

bool
ESDialogBase::SetSize(int cx, int cy, bool bRedraw){
	ESChildControl::SetSize(cx, cy, bRedraw);
	return true;
	}

bool
ESDialogBase::Destroy(){
	ESChildControl::Destroy();
	return true;
	}

void
ESDialogBase::OwnerWindowSizeChanged(int cx, int cy){
	ESChildControl::OwnerWindowSizeChanged(cx, cy);
	}