#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <string>
#include "baseAsset.h"

class ImageAsset : public BaseAsset
{
    public:
        ImageAsset(std::string filename);
        ~ImageAsset();

        //bool loaded;
        int width;
        int height;
        int comp;
        unsigned char* imageData;

        void load();
};

#endif // TEXTURE_H_INCLUDED
