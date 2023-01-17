#ifndef DUMMYSPEAKER_H
#define DUMMYSPEAKER_H

#include "speaker.h"

class DummySpeaker : public Speaker {
public:
    DummySpeaker();
    ~DummySpeaker() override;

    void play() override;
    void stop() override;
};

#endif // DUMMYSPEAKER_H