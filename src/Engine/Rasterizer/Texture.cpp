#include <AEEngine.h>
#include "Rasterizer.h"

namespace Rasterizer
{
	Texture::Texture()
		: mWidth(0)
		, mHeight(0)
		, mPixels(0) {
		
		}

	Texture::~Texture()  {

		if (mPixels)
			delete[] mPixels;
	
	}


	// wrapper around utils function that just makes a new texture
	Texture * LoadTextureFromFile(const char * filename) {

		Texture * tex = new Texture();
		AE_ASSERT(tex!=NULL); // out of memory!

		// Load and return 
		if (AEGfxLoadImagePNG(filename, tex->mPixels, tex->mWidth, tex->mHeight))
			return tex;	// done!

		// release unused memory 
		if (tex)
			delete tex;

		// load failed
		return NULL;
	
	}
}