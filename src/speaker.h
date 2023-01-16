#ifndef SPEAKER_H
#define SPEAKER_H

class Speaker {
public:
    virtual ~Speaker() = default;
    virtual void play() = 0;
    virtual void stop() = 0;
};

#endif // SPEAKER_H