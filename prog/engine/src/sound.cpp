#include "sound.h"

Sound::Sound()
{
    //ctor
}

Sound::~Sound()
{
    //dtor
}

void Sound::fromFile(std::string soundKey)
{
    std::string filepath = "assets/sound/"+soundKey;

    if (!sf::SoundBuffer::loadFromFile(filepath))
        std::cerr << "error: unable to load sound file " << filepath << "\n";
}
