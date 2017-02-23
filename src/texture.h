#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <vector>
#include <string>

class Texture
{
    public:
        Texture();
        Texture(std::string filename, bool loadNow);
        Texture(std::string filename);
        Texture(std::vector<std::string> inFilenames, bool loadNow);
        Texture(std::vector<std::string> inFilenames);
        ~Texture();

        bool loaded;
        std::vector<std::string> filenames;
        std::vector<int> widths;
        std::vector<int> heights;
        std::vector<int> comps;
        std::vector<unsigned char*> imageData;
        GLuint textureID;

        bool load();
        bool load(std::vector<std::string> filenames);
};

#endif // TEXTURE_H_INCLUDED
