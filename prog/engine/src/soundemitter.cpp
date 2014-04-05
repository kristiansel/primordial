#include "soundemitter.h"

SoundChannel::SoundChannel()
{

}

SoundChannel::~SoundChannel()
{

}

void SoundChannel::playSound(std::string soundKey)
{
    auto weak_ptr_snd_buff = global::sound_manager.getResptrFromKey(soundKey);
    sf::SoundBuffer* ptr_snd_buff = shared_ptr<sf::SoundBuffer>(weak_ptr_snd_buff).get();

    sound.setBuffer(*ptr_snd_buff);
    sound.play();
}

// SOUND EMITTER
SoundChannel SoundEmitter::s_soundChannels[NUM_FX_CHANNELS];
unsigned int SoundEmitter::current_sound_channel = 0;


SoundEmitter::SoundEmitter()
{
    //ctor
}

SoundEmitter::~SoundEmitter()
{
    //dtor
}

SoundChannel* SoundEmitter::getFreeSoundChannel()
{
    return ( &s_soundChannels[(current_sound_channel++)%(NUM_FX_CHANNELS-1)] );
}

void SoundEmitter::emitSound(std::string soundKey)
{
    SoundChannel* snd_chan = getFreeSoundChannel();
    snd_chan->playSound(soundKey);
}
