//
// Created by Tim on 20/03/2017.
//

#include "sound.h"

Sound::Sound()
{
    
}

Sound::~Sound()
{
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}