#include "soundemitter.h"

SoundEmitter::SoundEmitter()
{
    //ctor
}

SoundEmitter::~SoundEmitter()
{
    //dtor
}

void SoundEmitter::emitSound(std::string soundKey)
{
    auto weak_ptr_snd_buff = global::sound_manager.getResptrFromKey(soundKey);
    sf::SoundBuffer* ptr_snd_buff = shared_ptr<sf::SoundBuffer>(weak_ptr_snd_buff).get();

    sound.setBuffer(*ptr_snd_buff);
    sound.play();
}
