#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "ElSound.h"
#include "args.h"
#include "decoder.h"

// Header mask definition
#define HDRCMPMASK 0xfffffd00

// Maximum frame size
#define MAXFRAMESIZE 1792

#define MPG_MD_STEREO           0
#define MPG_MD_JOINT_STEREO     1
#define MPG_MD_DUAL_CHANNEL     2
#define MPG_MD_MONO             3

extern LPCTSTR NameString;
extern LPCTSTR ExtsString;

class SoundPlayer 
	{
public:
    SoundPlayer();
    ~SoundPlayer();

	bool					Open				();

	bool					Play				();
    bool					Pause				();
    bool					Resume				();
    bool					Stop				();
    bool					Close				();

	bool					InitStream			(); // initializes input stream record

    bool					SetInName			(const char *FileName);
    bool					SetOutName			(const char *FileName);
    bool					SetInMode			(esInputMode InMode);
    bool					SetOutMode			(esOutputMode OutMode);
    bool					SetOutputDevice		(unsigned long DevNum);
   
    unsigned long			GetPos				();
    unsigned long			GetSize				();
    bool					CanSetPos			();
    bool					SetPos				(unsigned long NewPos);
    esPlayerMode			PlayerMode			();
    bool					SetAudioBuffers		(unsigned long buffers, unsigned long size);
    bool					SetLimits			(unsigned long buffers, unsigned long size);
    bool					SetPriority			(long Priority);
    bool					SetOutCB			(WaveOutCB OutCB, WaveOutActionCB OutActionCB, unsigned long UserData);
    bool					SetInCB				(InFileOpenCB openCB, InFileCloseCB closeCB, InFileGetSizeCB getsizeCB, InFileSeekCB seekCB, InFileReadCB readCB, unsigned long UserData1);
    bool					CanSetVolume		(bool *Separate); 
    bool					SetVolume			(unsigned long Volume);
    bool					GetVolume			(unsigned long *Volume);
    bool					SetDevNum			(unsigned long DevNum);
    bool					SetWindow			(HWND Wnd);
    bool					UseOutCB			(bool UseCB);

	 esPlayerError			ESGetLastError		();

protected:
	static DWORD __stdcall	PlayThread			(SoundPlayer* pParam);
	DWORD					PlayThreadProc		();

	void					SynthesisFilterInit	();
	void					HardAudioReset		(SoundPlayer *ptr);

protected:
    esPlayerMode	FPlayerMode;

    HANDLE			StopEvent;
    bool			PauseFlag;
    bool			ThreadSync;
	bool			UserBreak;

	MPArgs			*Args;
	struct frame	fr;
	esPlayerError	LastError;
	HANDLE			ThreadQueueEvent;
	HANDLE			SeekEvent;
	HANDLE			SeekEvent2;
	HANDLE			FinishEvent;
	unsigned long	NewSeekPosition;

private:
	HANDLE			m_hThreadQueueEvent;

};

extern esPlayerError LastError;
extern DWORD OLEInit;

#endif