#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <vector>
#include <string>
#include "baseAsset.h"

class ImageAsset : public BaseAsset
{
    public:
        //ImageAsset();
        //ImageAsset(std::string filename, bool loadNow);
        ImageAsset(std::string filename);
        //ImageAsset(std::vector<std::string> inFilenames, bool loadNow);
        //ImageAsset(std::vector<std::string> inFilenames);
        ~ImageAsset();

        //bool loaded;
        std::vector<std::string> filenames;
        std::vector<int> widths;
        std::vector<int> heights;
        std::vector<int> comps;
        std::vector<unsigned char*> imageData;
        //GLuint textureID;

        void load();
        //bool load(std::vector<std::string> filenames);
};

#endif // TEXTURE_H_INCLUDED
