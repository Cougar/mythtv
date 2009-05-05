#ifndef AUDIOOUTPUT_H_
#define AUDIOOUTPUT_H_
/*
	audiooutput.h

	(c) 2003 Thor Sigvaldason and Isaac Richards
	Part of the mythTV project
	
	Headers for actual device audio output
	
	Very closely based on work by Brad Hughes
	Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>

*/


class MMAudioOutput;

#include "output.h"

//  #if defined( Q_OS_WIN32 )
//  #include <dsound.h>
//  #include "constants.h"
//  #endif

struct ReSampleContext;

class AudioPlugin;

class MMAudioOutput : public Output
{
public:
    MMAudioOutput(AudioPlugin *owner, unsigned int, const QString &);
    virtual ~MMAudioOutput();

    bool initialized() const { return inited; }

    bool initialize();
    void configure(long, int, int, int);
    void stop();
    void pause();
    long written();
    long latency();
    void seek(long);
    void resetTime();
    bool isPaused() { return paus; }
    void dispatch(OutputEvent&);

private:
    // thread run function
    void run();

    // helper functions
    void reset();
    void resetDSP();
    void status();
    void post();
    void sync();

    

 
    QString     audio_device;
    AudioPlugin *parent;

    bool inited,paus,play,user_stop;
    long total_written, current_seconds,bps;
    int stat;
    int lr, lf, lc, lp;

    bool needresample;
    ReSampleContext *resampctx;

#if defined(_OS_UNIX_) || defined(Q_OS_UNIX)
    bool do_select;
    int audio_fd;

#elif defined( _OS_WIN32_ ) || defined( Q_OS_WIN32 )
    LPDIRECTSOUND lpds;
    LPDIRECTSOUNDBUFFER lpdsb;
    LPDIRECTSOUNDNOTIFY lpdsn;
    PCMWAVEFORMAT pcmwf;
    HANDLE hNotifyEvent;
    DSBPOSITIONNOTIFY notifies[ BUFFERBLOCKS ];
    DWORD currentLatency;
/* Not sure we need this
    DSCAPS dsCaps;
*/
#endif

};


#endif // _audiooutout_h_