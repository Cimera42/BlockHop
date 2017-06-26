#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <string>
#include "baseAsset.h"

class ImageAsset : public BaseAsset
{
    friend class ImageLoader;
    bool load();
    ImageAsset(std::string filename);
    ~ImageAsset();
public:
    //bool loaded;
    int width;
    int height;
    int comp;
    unsigned char* imageData;
};

#endif // TEXTURE_H_INCLUDED
