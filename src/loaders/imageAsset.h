#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>
#include "baseAsset.h"

class ImageAsset : public BaseAsset
{
	friend class ImageLoader;
	bool load() override;
	explicit ImageAsset(const std::string &filename);
	~ImageAsset() override;
public:
	//bool loaded;
	int width;
	int height;
	int comp;
	unsigned char* imageData;
};

#endif // TEXTURE_H_INCLUDED
