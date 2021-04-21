// ESChildControl.cpp : implementation file
//

#include "ESEasyMotion.h"
#include <math.h>

#ifndef PI
 #define PI 3.141592653589793f
#endif

/////////////////////////////////////////////////////////////////////////////
// ESEasyMotion

ESEasyMotion::ESEasyMotion(){
	m_hWndSend				= NULL;
	m_nMsgSend				= 0L;
	m_lParam				= 0L;
	m_wParam				= 0L;
	m_bAutoDelete			= false;
	m_nWaitAfterMotionMS	= 0;
	}

ESEasyMotion::~ESEasyMotion(){
	// Stop motion.
	StopMotion();
	}

void
ESEasyMotion::AddMotionObject(ESChildControl* pControl, int nDurationMS, int nFrameCtPerSecond, 
							ESEasyMotionType motionTypeX, ESEasyMotionType motionTypeY,
							float fStartPosX, float fStartPosY, float fEndPosX, float fEndPosY, 
							int nWaitBeforeStartMS /*= 0*/, 
							DWORD_PTR dwMotionStartParam /*= 0*/, 
							DWORD_PTR dwMotionEndParam /*= 0*/, 
							DWORD_PTR dwMotionStopParam /*= 0*/
							)
	{
	m_motionThread.Lock();		// Lock
	MotionObjectInfo* pInfo		= new MotionObjectInfo;
	pInfo->pChild				= pControl;
	pInfo->nDurationMS			= nDurationMS;
	pInfo->nFrameCtPerSec		= nFrameCtPerSecond;
	pInfo->procEquationX		= GetEquationProc(motionTypeX);
	pInfo->procEquationY		= GetEquationProc(motionTypeY);
	pInfo->fStartPosX			= fStartPosX;
	pInfo->fStartPosY			= fStartPosY;
	pInfo->fEndPosX				= fEndPosX;
	pInfo->fEndPosY				= fEndPosY;
	pInfo->fCurrentPosX			= 0.0f;
	pInfo->fCurrentPosY			= 0.0f;
    pInfo->nWaitBeforeStartMS	= max(0, nWaitBeforeStartMS);
	pInfo->clockStart			= 0;
	pInfo->dwMotionStartParam	= dwMotionStartParam;
	pInfo->dwMotionEndParam		= dwMotionEndParam;
	pInfo->dwMotionStopParam	= dwMotionStopParam;
	m_listMotionObjects.Add(pInfo);
	m_motionThread.Unlock();	// Unlock
	}

int
ESEasyMotion::StartMotion(HWND hWndSend, UINT nMsgMotionFinish, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/, int nWaitAfterMotionMS /*= 0*/){
	if( !m_motionThread.IsThreadStopped() )
		return 0;
	if( !m_listMotionObjects.GetCount() )
		return 0;
	if( !m_motionThread.StartThread((LPTHREAD_START_ROUTINE)&ESEasyMotion::MotionThreadProc, this) )
		return 0;

	m_hWndSend				= hWndSend;
	m_nMsgSend				= nMsgMotionFinish;
	m_wParam				= wParam;
	m_lParam				= lParam;
	m_nWaitAfterMotionMS	= nWaitAfterMotionMS;
	return 1;
	}

void
ESEasyMotion::StopMotion(){
	// Stop motion thread.
	m_motionThread.StopThread();
	// Clean up motion objects info.
	m_listMotionObjects.RemoveAll(true);

	m_hWndSend				= 0L;
	m_nMsgSend				= 0;
	m_wParam				= 0;
	m_lParam				= 0;
	m_nWaitAfterMotionMS	= 0;
	}

//////////////////////////////////////////////////////////
//	Motion thread.
//////////////////////////////////////////////////////////
#ifdef _WINDOWS
UINT _stdcall	
ESEasyMotion::MotionThreadProc(WinThread* pThread){
#else
void*
ESEasyMotion::MotionThreadProc(WinThread* pThread){
#endif
	ESEasyMotion* pMotion = (ESEasyMotion*)pThread->GetThreadParameter();
	if( !pMotion ) return 0;
	pMotion->MotionLoop();

	// Deinitialize motion.
	pMotion->StopMotion();
	return 0;
	}

void
ESEasyMotion::MotionLoop(){
	List<MotionObjectInfo> listObject;
	MercuryBaseView* pOwner = NULL;
	m_motionThread.Lock();		// Lock
	for(int i=0; i<m_listMotionObjects.GetCount(); i++){
		MotionObjectInfo* pObject = m_listMotionObjects.GetAt(i);
		if( pObject && pObject->pChild ){
			if( !pOwner )
				pOwner = pObject->pChild->GetOwner();
			pObject->nCurrentFrame	= 0;
			pObject->fCurrentPosX	= 0.0f;
			pObject->fCurrentPosY	= 0.0f;
			pObject->nFrameCt		= (int)((pObject->nDurationMS / 1000.0f)*pObject->nFrameCtPerSec);
			pObject->nDelayPerFrame	= (int)(((float)pObject->nDurationMS) / pObject->nFrameCt);
			listObject.Add(pObject);
			}
		}
	m_listMotionObjects.RemoveAll(false);
	m_motionThread.Unlock();	// Unlock

	BOOL bStop = FALSE;
	while( !bStop ){
		int nMotionCt	= 0, nDelayMSec	= 0, nObjectsMotioned = 0;
		// Start invalid rects caching.
		pOwner->StartCachingInvalidRects();
		for(int i=0; i<listObject.GetCount(); i++){
			MotionObjectInfo*	pObject			= listObject[i];
			ESChildControl* 	pChild			= pObject->pChild;
			int 				nFrameCt		= pObject->nFrameCt;
			int 				nDelayPerFrame	= pObject->nDelayPerFrame;
			int					nLoop			= pObject->nCurrentFrame;
			int					nCurrTimeMS		= nLoop*nDelayPerFrame;

			// Count wait miliseconds before start motion.
			if( pObject->nWaitBeforeStartMS > 0 ){
				if( !pObject->clockStart ){
					pObject->clockStart = clock();
					nMotionCt ++;
					continue;
					}
				else{
					clock_t check = clock();
					long waitMSRemain = (pObject->nWaitBeforeStartMS - (check - pObject->clockStart));
					if( waitMSRemain <= 0 )
						pObject->nWaitBeforeStartMS = 0;
					else{
						nMotionCt ++;
						continue;
						}
					}
				}

			nDelayMSec							= max(nDelayPerFrame, nDelayMSec);
			float				fPosX			= (pObject->nCurrentFrame > 0 ? pObject->fCurrentPosX : pObject->fStartPosX);
			float				fPosY			= (pObject->nCurrentFrame > 0 ? pObject->fCurrentPosY : pObject->fStartPosY);
			float				fChangeInPosX	= (pObject->fEndPosX - pObject->fStartPosX);
			float				fChangeInPosY	= (pObject->fEndPosY - pObject->fStartPosY);

			if( nLoop < nFrameCt ){
				// Start event.
				if( !nLoop ){ 
					pChild->OnMotionStart(pObject->dwMotionStartParam);
					// Call event handler.
					pChild->SendEvent(pObject->pChild, ChildControlEventCode::EventCode_MotionStart, ((void*)pObject->dwMotionStartParam));
					}

				if( m_motionThread.IsStopEventSignaled() ){
					// Signal stop event.
					for(int i=0; i<listObject.GetCount(); i++){
						MotionObjectInfo*	pObject	= listObject[i];
						// If motion is not ended.
						if( pObject->pChild ){
							pObject->pChild->OnMotionStopped();
							// Call event handler.
							pObject->pChild->SendEvent(pObject->pChild, ChildControlEventCode::EventCode_MotionStop, ((void*)pObject->dwMotionStopParam));
							}
						}
					// Clean up motion objects info.
					listObject.RemoveAll(true);
					return;
					}
				
				pChild->OnMotionProgress(nLoop + 1, nCurrTimeMS, nFrameCt, fPosX, fPosY);
				nLoop					++;
				pObject->nCurrentFrame	++;
				nCurrTimeMS				+= nDelayPerFrame;
				fPosX					= (pObject->procEquationX)(nCurrTimeMS, pObject->fStartPosX, fChangeInPosX, pObject->nDurationMS);
				fPosY					= (pObject->procEquationY)(nCurrTimeMS, pObject->fStartPosY, fChangeInPosY, pObject->nDurationMS);
				pObject->fCurrentPosX	= fPosX;
				pObject->fCurrentPosY	= fPosY;

				/*
				nLoop					++;
				pObject->nCurrentFrame	++;
				nCurrTimeMS				+= nDelayPerFrame;
				fPosX					= (pObject->procEquationX)(nCurrTimeMS, pObject->fStartPosX, fChangeInPosX, pObject->nDurationMS);
				fPosY					= (pObject->procEquationY)(nCurrTimeMS, pObject->fStartPosY, fChangeInPosY, pObject->nDurationMS);
				pChild->OnMotionProgress(nLoop, nCurrTimeMS, nFrameCt, fPosX, fPosY);
				*/
				nMotionCt				++;
				nObjectsMotioned		++;

				// End event.
				if( pObject->nCurrentFrame == nFrameCt ){
					pChild->OnMotionEnd();
					// Call event handler.
					pChild->SendEvent(pObject->pChild, ChildControlEventCode::EventCode_MotionEnd, ((void*)pObject->dwMotionEndParam));
					// Stop tracking motion child control.
					pObject->pChild = NULL;
					}
				}
			}

		if( !nMotionCt ){
			bStop = TRUE;
			}
		else{
			//TRACE1(_T("Objects motion ct = %d\r\n"), nObjectsMotioned);
			//ASSERT(pOwner);
			// Redraw cached invalid rects.
			pOwner->EndCachingInvalidRects();
			Sleep(nDelayMSec);
			}
		}

	// Clean up motion objects info.
	listObject.RemoveAll(true);

	if( m_nWaitAfterMotionMS > 0 )
		Sleep(m_nWaitAfterMotionMS);

	// Send motion complete message.
	if( m_hWndSend && m_nMsgSend ){
#ifdef _WINDOWS
		PostMessage(m_hWndSend, m_nMsgSend, m_wParam, m_lParam);
#else
        
#endif
        }

	// Autodelete itself.
	if( m_bAutoDelete )
		delete this;
	}

//////////////////////////////////////////////////////////
//	Motion equations.
//////////////////////////////////////////////////////////


MotionEquationProc
ESEasyMotion::GetEquationProc(ESEasyMotionType motionType){
	switch( motionType ){
		case ESEasyMotionType::MotionLinier:
			return (MotionEquationProc)&MotionEquation_Linier;

		case ESEasyMotionType::MotionQuadraticIn:
			return (MotionEquationProc)&MotionEquation_QuadraticIn;
		case ESEasyMotionType::MotionQuadraticOut:
			return (MotionEquationProc)&MotionEquation_QuadraticOut;
		case ESEasyMotionType::MotionQuadraticInOut:
			return (MotionEquationProc)&MotionEquation_QuadraticInOut;

		case ESEasyMotionType::MotionCubicIn:
			return (MotionEquationProc)&MotionEquation_CubicIn;
		case ESEasyMotionType::MotionCubicOut:
			return (MotionEquationProc)&MotionEquation_CubicOut;
		case ESEasyMotionType::MotionCubicInOut:
			return (MotionEquationProc)&MotionEquation_CubicInOut;

		case ESEasyMotionType::MotionQuarticIn:
			return (MotionEquationProc)&MotionEquation_QuarticIn;
		case ESEasyMotionType::MotionQuarticOut:
			return (MotionEquationProc)&MotionEquation_QuarticOut;
		case ESEasyMotionType::MotionQuarticInOut:
			return (MotionEquationProc)&MotionEquation_QuarticInOut;

		case ESEasyMotionType::MotionQuinticIn:
			return (MotionEquationProc)&MotionEquation_QuinticIn;
		case ESEasyMotionType::MotionQuinticOut:
			return (MotionEquationProc)&MotionEquation_QuinticOut;
		case ESEasyMotionType::MotionQuinticInOut:
			return (MotionEquationProc)&MotionEquation_QuinticInOut;

		case ESEasyMotionType::MotionSinusoidalIn:
			return (MotionEquationProc)&MotionEquation_SinusoidalIn;
		case ESEasyMotionType::MotionSinusoidalOut:
			return (MotionEquationProc)&MotionEquation_SinusoidalOut;
		case ESEasyMotionType::MotionSinusoidalInOut:
			return (MotionEquationProc)&MotionEquation_SinusoidalInOut;

		case ESEasyMotionType::MotionExponentialIn:
			return (MotionEquationProc)&MotionEquation_ExponentialIn;
		case ESEasyMotionType::MotionExponentialOut:
			return (MotionEquationProc)&MotionEquation_ExponentialOut;
		case ESEasyMotionType::MotionExponentialInOut:
			return (MotionEquationProc)&MotionEquation_ExponentialInOut;

		case ESEasyMotionType::MotionCircularIn:
			return (MotionEquationProc)&MotionEquation_CircularIn;
		case ESEasyMotionType::MotionCircularOut:
			return (MotionEquationProc)&MotionEquation_CircularOut;
		case ESEasyMotionType::MotionCircularInOut:
			return (MotionEquationProc)&MotionEquation_CircularInOut;
		};
	return NULL;
	}

float
ESEasyMotion::MotionEquation_Linier(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	return fChangeInValue*(nCurrTimeMS/nMotionDurationMS) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuadraticIn(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return fChangeInValue*dTime*dTime + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuadraticOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return -1*fChangeInValue*dTime*(dTime-2) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuadraticInOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (double)nCurrTimeMS;
	dTime		/= ((double)nMotionDurationMS)/2;
	if( dTime < 1.0 ) return (fChangeInValue/2)*dTime*dTime + fStartValue;

	dTime -= 1.0;
	return -fChangeInValue/2.0 * (dTime*(dTime-2) - 1) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_CubicIn(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return fChangeInValue*dTime*dTime*dTime + fStartValue;
	}

float
ESEasyMotion::MotionEquation_CubicOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	dTime -= 1.0;
	return fChangeInValue*(dTime*dTime*dTime + 1) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_CubicInOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (double)nCurrTimeMS;
	dTime		/= ((double)nMotionDurationMS)/2;
	if( dTime < 1.0 )
        return fChangeInValue*dTime*dTime*dTime + fStartValue;

	dTime -= 2.0;
	return fChangeInValue*(dTime*dTime*dTime + 1) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuarticIn(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return fChangeInValue*dTime*dTime*dTime*dTime + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuarticOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	dTime -= 1.0;
	return -fChangeInValue*(dTime*dTime*dTime*dTime - 1) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuarticInOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (double)nCurrTimeMS;
	dTime		/= ((double)nMotionDurationMS)/2;
	if( dTime < 1.0 ) 
		return fChangeInValue/2*dTime*dTime*dTime*dTime + fStartValue;
	dTime -= 2.0;
	return -fChangeInValue/2 * (dTime*dTime*dTime*dTime - 2) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuinticIn(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return fChangeInValue*dTime*dTime*dTime*dTime*dTime + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuinticOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	dTime -= 1.0;
	return fChangeInValue*(dTime*dTime*dTime*dTime*dTime + 1) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_QuinticInOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (double)nCurrTimeMS;
	dTime		/= ((double)nMotionDurationMS)/2;
	if( dTime < 1.0 ) 
		return fChangeInValue/2*dTime*dTime*dTime*dTime*dTime + fStartValue;

	dTime -= 2.0;
	return fChangeInValue/2*(dTime*dTime*dTime*dTime*dTime + 2) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_SinusoidalIn(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return (-fChangeInValue*cos(dTime * (PI/2)) + fChangeInValue + fStartValue);
    }

float
ESEasyMotion::MotionEquation_SinusoidalOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return (fChangeInValue*sin(dTime * (PI/2)) + fStartValue);
	}

float
ESEasyMotion::MotionEquation_SinusoidalInOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return (-fChangeInValue/2 * (cos(dTime*PI) - 1) + fStartValue);
	}

float
ESEasyMotion::MotionEquation_ExponentialIn(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return fChangeInValue * pow(2, 10 * (dTime - 1.0)) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_ExponentialOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return fChangeInValue * (-pow(2, -10*dTime) + 1) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_ExponentialInOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (double)nCurrTimeMS;
	dTime		/= ((double)nMotionDurationMS)/2;
	if( dTime < 1.0 ) 
		return (fChangeInValue/2)*pow(2,10*(dTime - 1)) + fStartValue;
	dTime -= 1.0;
	return (fChangeInValue/2.0 * (-pow(2,-10*dTime) + 2) + fStartValue);
	}

float				
ESEasyMotion::MotionEquation_CircularIn(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	return -fChangeInValue * (sqrt(1-dTime*dTime) - 1) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_CircularOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (nCurrTimeMS/((double)nMotionDurationMS));
	dTime -= 1.0;
	return fChangeInValue * sqrt(1-dTime*dTime) + fStartValue;
	}

float
ESEasyMotion::MotionEquation_CircularInOut(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS){
	double dTime = (double)nCurrTimeMS;
	dTime		/= ((double)nMotionDurationMS)/2;
	if( dTime < 1.0 ) 
		return  (-fChangeInValue/2)*(sqrt(1-dTime*dTime) - 1) + fStartValue;
	dTime -= 2.0;
	return fChangeInValue * sqrt(1-dTime*dTime) + fStartValue;
	}


/*
t=current time
b=starting value
c=change in value
d=duration


 simple linear tweening - no easing, no acceleration


    Math.linearTween = function (t, b, c, d) {
    	return c*t/d + b;
    };
    		


// quadratic easing in - accelerating from zero velocity


    Math.easeInQuad = function (t, b, c, d) {
    	t /= d;
    	return c*t*t + b;
    };
    		


// quadratic easing out - decelerating to zero velocity


    Math.easeOutQuad = function (t, b, c, d) {
    	t /= d;
    	return -c * t*(t-2) + b;
    };

    		


// quadratic easing in/out - acceleration until halfway, then deceleration


    Math.easeInOutQuad = function (t, b, c, d) {
    	t /= d/2;
    	if (t < 1) return c/2*t*t + b;
    	t--;
    	return -c/2 * (t*(t-2) - 1) + b;
    };


// cubic easing in - accelerating from zero velocity


    Math.easeInCubic = function (t, b, c, d) {
    	t /= d;
    	return c*t*t*t + b;
    };

    		


// cubic easing out - decelerating to zero velocity


    Math.easeOutCubic = function (t, b, c, d) {
    	t /= d;
    	t--;
    	return c*(t*t*t + 1) + b;
    };

    		


// cubic easing in/out - acceleration until halfway, then deceleration


    Math.easeInOutCubic = function (t, b, c, d) {
    	t /= d/2;
    	if (t < 1) return c/2*t*t*t + b;
    	t -= 2;
    	return c/2*(t*t*t + 2) + b;
    };
    	


// quartic easing in - accelerating from zero velocity


    Math.easeInQuart = function (t, b, c, d) {
    	t /= d;
    	return c*t*t*t*t + b;
    };

    		


// quartic easing out - decelerating to zero velocity


    Math.easeOutQuart = function (t, b, c, d) {
    	t /= d;
    	t--;
    	return -c * (t*t*t*t - 1) + b;
    };

    		


// quartic easing in/out - acceleration until halfway, then deceleration


    Math.easeInOutQuart = function (t, b, c, d) {
    	t /= d/2;
    	if (t < 1) return c/2*t*t*t*t + b;
    	t -= 2;
    	return -c/2 * (t*t*t*t - 2) + b;
    };


// quintic easing in - accelerating from zero velocity


    Math.easeInQuint = function (t, b, c, d) {
    	t /= d;
    	return c*t*t*t*t*t + b;
    };

    		


// quintic easing out - decelerating to zero velocity


    Math.easeOutQuint = function (t, b, c, d) {
    	t /= d;
    	t--;
    	return c*(t*t*t*t*t + 1) + b;
    };

    		


// quintic easing in/out - acceleration until halfway, then deceleration


    Math.easeInOutQuint = function (t, b, c, d) {
    	t /= d/2;
    	if (t < 1) return c/2*t*t*t*t*t + b;
    	t -= 2;
    	return c/2*(t*t*t*t*t + 2) + b;
    };
    		


// sinusoidal easing in - accelerating from zero velocity


    Math.easeInSine = function (t, b, c, d) {
    	return -c * Math.cos(t/d * (Math.PI/2)) + c + b;
    };

    		


// sinusoidal easing out - decelerating to zero velocity


    Math.easeOutSine = function (t, b, c, d) {
    	return c * Math.sin(t/d * (Math.PI/2)) + b;
    };

    		


// sinusoidal easing in/out - accelerating until halfway, then decelerating


    Math.easeInOutSine = function (t, b, c, d) {
    	return -c/2 * (Math.cos(Math.PI*t/d) - 1) + b;
    };

    		


// exponential easing in - accelerating from zero velocity


    Math.easeInExpo = function (t, b, c, d) {
    	return c * Math.pow( 2, 10 * (t/d - 1) ) + b;
    };

    		


// exponential easing out - decelerating to zero velocity


    Math.easeOutExpo = function (t, b, c, d) {
    	return c * ( -Math.pow( 2, -10 * t/d ) + 1 ) + b;
    };

    		


// exponential easing in/out - accelerating until halfway, then decelerating


    Math.easeInOutExpo = function (t, b, c, d) {
    	t /= d/2;
    	if (t < 1) return c/2 * Math.pow( 2, 10 * (t - 1) ) + b;
    	t--;
    	return c/2 * ( -Math.pow( 2, -10 * t) + 2 ) + b;
    };
    		


// circular easing in - accelerating from zero velocity


    Math.easeInCirc = function (t, b, c, d) {
    	t /= d;
    	return -c * (Math.sqrt(1 - t*t) - 1) + b;
    };

    		


// circular easing out - decelerating to zero velocity


    Math.easeOutCirc = function (t, b, c, d) {
    	t /= d;
    	t--;
    	return c * Math.sqrt(1 - t*t) + b;
    };

    		


// circular easing in/out - acceleration until halfway, then deceleration


    Math.easeInOutCirc = function (t, b, c, d) {
    	t /= d/2;
    	if (t < 1) return -c/2 * (Math.sqrt(1 - t*t) - 1) + b;
    	t -= 2;
    	return c/2 * (Math.sqrt(1 - t*t) + 1) + b;
    };

 
*/
