#ifndef SOUNDEMITTER_H
#define SOUNDEMITTER_H

#include "SFML/Audio.hpp"
#include "global.h"


class SoundChannel
{
public:
    SoundChannel();
    virtual ~SoundChannel();

    void playSound(std::string soundKey);
private:
    sf::Sound sound;
};

class SoundEmitter
{
private:
    static const unsigned int NUM_FX_CHANNELS = 200;
    static SoundChannel s_soundChannels[NUM_FX_CHANNELS]; // 200/255 sound channels are for sound effects
    static unsigned int current_sound_channel;

public:
    SoundEmitter();
    virtual ~SoundEmitter();

    void emitSound(std::string soundKey);

protected:
private:
    //sf::Sound sound;

private:
    SoundChannel* getFreeSoundChannel();

};

#endif // SOUNDEMITTER_H
