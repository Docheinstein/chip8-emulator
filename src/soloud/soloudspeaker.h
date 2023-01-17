#ifdef SOLOUD_ENABLED

#ifndef SOLOUDSPEAKER_H
#define SOLOUDSPEAKER_H

#include "speaker.h"
#include "soloud.h"
#include "soloud_wav.h"

class SoLoudSpeaker : public Speaker {
public:
    SoLoudSpeaker();
    ~SoLoudSpeaker() override;

    void play() override;
    void stop() override;

private:
    SoLoud::Soloud soloud;
    SoLoud::Wav wav;
    bool playing;
};

#endif // SOLOUDSPEAKER_H

#endif // #ifdef SOLOUD_ENABLED
