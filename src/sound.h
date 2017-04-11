//
// Created by Tim on 20/03/2017.
//

#ifndef BLOCKHOP_CLION_SOUND_H
#define BLOCKHOP_CLION_SOUND_H

#include <al.h>

class Sound
{
public:
    Sound();
    ~Sound();
    
    ALuint buffer, source;
    ALint state;
};


#endif //BLOCKHOP_CLION_SOUND_H
