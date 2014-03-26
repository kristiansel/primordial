#ifndef SOUND_H
#define SOUND_H

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

class Sound : public sf::SoundBuffer
{
    public:
        Sound();
        virtual ~Sound();

        void fromFile(std::string sound_key);

    protected:
    private:
};

#endif // SOUND_H
