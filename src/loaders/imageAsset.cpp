#include "imageAsset.h"
#include "../logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


ImageAsset::ImageAsset(std::string filename): BaseAsset(filename) {}

ImageAsset::~ImageAsset()
{
    stbi_image_free(imageData);
}

bool ImageAsset::load()
{
    stbi_set_flip_vertically_on_load(true);
    int tempWidth, tempHeight, tempComp;
    imageData = stbi_load(filename.c_str(), &tempWidth, &tempHeight, &tempComp, STBI_rgb_alpha);

    width = tempWidth;
    height = tempHeight;
    comp = tempComp;

    if(!imageData)
    {
        Logger(1) << "Could not load image: " << filename << " - " << stbi_failure_reason();
        return false;
    }
    return true;
}
