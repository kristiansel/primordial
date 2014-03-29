#ifndef SOUNDEMITTER_H
#define SOUNDEMITTER_H

#include "SFML/Audio.hpp"
#include "global.h"


class SoundEmitter
{
    public:
        SoundEmitter();
        virtual ~SoundEmitter();

        void emitSound(std::string soundKey);

    protected:
    private:
        sf::Sound sound;

};

#endif // SOUNDEMITTER_H
