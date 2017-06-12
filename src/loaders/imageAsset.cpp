#include "imageAsset.h"
#include "../openGLFunctions.h"
#include "../logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/*ImageAsset::ImageAsset()
{
    loaded = false;
    glGenTextures(1, &textureID);
}
ImageAsset::ImageAsset(std::string inFilename, bool loadNow)
{
    filenames.push_back(inFilename);
    glGenTextures(1, &textureID);
    if(loadNow)
        load();
}
ImageAsset::ImageAsset(std::string inFilename) : ImageAsset(inFilename, true) {}

ImageAsset::ImageAsset(std::vector<std::string> inFilenames, bool loadNow)
{
    filenames.swap(inFilenames);
    glGenTextures(1, &textureID);
    if(loadNow)
        load();
}
ImageAsset::ImageAsset(std::vector<std::string> inFilenames) : ImageAsset(inFilenames, true) {}*/

ImageAsset::~ImageAsset()
{
    for(auto singleImageData : imageData)
        stbi_image_free(singleImageData);
    //glDeleteTextures(1, &textureID);
}

/*bool ImageAsset::load(std::vector<std::string> inFilenames)
{
    filenames.swap(inFilenames);
    return load();
}*/

void ImageAsset::load()
{
    stbi_set_flip_vertically_on_load(true);
    int maxW = 0;
    int maxH = 0;
    for(unsigned int i = 0; i < filenames.size(); i++)
    {
        int tempWidth, tempHeight, tempComp;
        imageData.push_back(stbi_load(filenames[i].c_str(), &tempWidth, &tempHeight, &tempComp, STBI_rgb_alpha));

        if(tempWidth > maxW)
            maxW = tempWidth;
        if(tempHeight > maxH)
            maxH = tempHeight;

        widths.push_back(tempWidth);
        heights.push_back(tempHeight);
        comps.push_back(tempComp);

        if(!imageData[i])
        {
            //loaded = false;
            Logger(1) << "Image not loaded: " << filenames[i] << " - " << stbi_failure_reason();
        }
    }
/*
    glSetBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, imageData.size(), GL_RGBA8, maxW,maxH, imageData.size());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    for(unsigned int i = 0; i < imageData.size(); i++)
    {
        if(imageData[i])
        {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,0,0, i, maxW,maxH, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData[i]);

            GLenum err = glGetError();
            if(!err)
            {
                loaded = true;
                Logger(1) << "Texture loaded: " << filenames[i];
            }
            else
            {
                Logger(1) << "Texture load failed: " << filenames[i] << " - " << std::hex << err;
            }
        }
    }
    return true;*/
}
