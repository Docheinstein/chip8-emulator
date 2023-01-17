#ifdef SOLOUD_ENABLED

#include "soloudspeaker.h"
#include "440.h"

void SoLoudSpeaker::play() {
    if (!playing) {
        playing = true;
        soloud.setPauseAll(false);
    }
}

void SoLoudSpeaker::stop() {
    if (playing) {
        playing = false;
        soloud.setPauseAll(true);
    }
}

SoLoudSpeaker::SoLoudSpeaker() :
        playing() {
    soloud.init();
    wav.loadMem(sound_440_wav, sound_440_wav_len, false, false);
    wav.setLooping(true);
    soloud.play(wav, 1.0f, 0.0f, true /* paused */);
}

SoLoudSpeaker::~SoLoudSpeaker() {
    soloud.deinit();
}

#endif // #ifdef SOLOUD_ENABLED
