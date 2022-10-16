#ifndef _TEXTURE_H_
#define _TEXTURE_H_

namespace Rasterizer
{
	struct Texture
	{
		u8	   *mPixels;
		u32		mWidth;
		u32		mHeight;

		Texture();
		~Texture();
	};

	Texture * LoadTextureFromFile(const char * filename); // wrapper around utils function that just makes a new texture
}
#endif