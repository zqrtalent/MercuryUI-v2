#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESEasyMotion window

#include "ESChildControl.h"
#include "../Thread/WinThread.h"

enum ESEasyMotionType
{
	MotionLinier = 0,
	MotionQuadraticIn,
	MotionQuadraticOut,
	MotionQuadraticInOut,

	MotionCubicIn,
	MotionCubicOut,
	MotionCubicInOut,

	MotionQuarticIn,
	MotionQuarticOut,
	MotionQuarticInOut,

	MotionQuinticIn,
	MotionQuinticOut,
	MotionQuinticInOut,

	MotionSinusoidalIn,
	MotionSinusoidalOut,
	MotionSinusoidalInOut,

	MotionExponentialIn,
	MotionExponentialOut,
	MotionExponentialInOut,

	MotionCircularIn,
	MotionCircularOut,
	MotionCircularInOut
};


typedef float (*MotionEquationProc)(int, float, float, int);

struct MotionObjectInfo
{
	ESChildControl*		pChild;
	int					nDurationMS;
	int					nFrameCtPerSec;

	MotionEquationProc	procEquationX;
	MotionEquationProc	procEquationY;

	clock_t				clockStart;
	int					nWaitBeforeStartMS;		// Wait time before start in MS.

	float				fStartPosX;
	float				fStartPosY;
	float				fEndPosX;
	float				fEndPosY;
	float				fCurrentPosX;
	float				fCurrentPosY;

	int					nCurrentFrame;
	int					nFrameCt;
	int					nDelayPerFrame;

	DWORD_PTR			dwMotionStartParam;
	DWORD_PTR			dwMotionEndParam;
	DWORD_PTR			dwMotionStopParam;
};

class ESEasyMotion
{
// Construction
public:
	ESEasyMotion();
	virtual ~ESEasyMotion();

	void	AddMotionObject	(ESChildControl* pControl, int nDurationMS, int nFrameCtPerSecond, 
							ESEasyMotionType motionTypeX, 
							ESEasyMotionType motionTypeY,
							float fStartPosX, float fStartPosY, float fEndPosX, float fEndPosY, int nWaitBeforeStartMS = 0, 
							DWORD_PTR dwMotionStartParam = 0, 
							DWORD_PTR dwMotionEndParam = 0, 
							DWORD_PTR dwMotionStopParam = 0);

	int		StartMotion		(HWND hWndSend, UINT nMsgMotionFinish, WPARAM wParam = 0L, LPARAM lParam = 0L, int nWaitAfterMotionMS = 0);
	void	StopMotion		();
	bool	IsStarted		(){return !m_motionThread.IsThreadStopped();}
	void	SetAutoDelete	(bool bAutoDelete){m_bAutoDelete = bAutoDelete;}

	DWORD	GetMotionThreadID(){return IsStarted() ? m_motionThread.GetThreadId() : 0L;};

protected:
	void						MotionLoop						();
#ifdef _WINDOWS
	static UINT _stdcall		MotionThreadProc				(WinThread* pThread);
#else
    static void*                MotionThreadProc				(WinThread* pThread);
#endif

	static MotionEquationProc	GetEquationProc					(ESEasyMotionType motionType);
	static float				MotionEquation_Linier			(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuadraticIn		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuadraticOut		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuadraticInOut	(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_CubicIn			(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_CubicOut			(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_CubicInOut		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuarticIn		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuarticOut		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuarticInOut		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuinticIn		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuinticOut		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_QuinticInOut		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_SinusoidalIn		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_SinusoidalOut	(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_SinusoidalInOut	(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_ExponentialIn	(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_ExponentialOut	(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_ExponentialInOut	(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_CircularIn		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_CircularOut		(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);
	static float				MotionEquation_CircularInOut	(int nCurrTimeMS, float fStartValue, float fChangeInValue, int nMotionDurationMS);

protected:
	WinThread						m_motionThread;
	List<MotionObjectInfo>			m_listMotionObjects;
	HWND							m_hWndSend;
	UINT							m_nMsgSend;
	WPARAM							m_wParam;
	LPARAM							m_lParam;
	int								m_nWaitAfterMotionMS;
	bool							m_bAutoDelete;
};

/*
Math.linearTween = function (t, b, c, d) {
	return c*t/d + b;
};*/


/*

t = current time
b = start value
c = change value
d = duration

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