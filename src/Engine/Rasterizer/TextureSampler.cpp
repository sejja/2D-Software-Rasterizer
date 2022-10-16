//
//  TextureSample.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 26/06/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include "Rasterizer.h"

#define U	0
#define V	U + 1
#define UV	V + 1
#define R		0
#define G		1
#define B		2
#define A		3
#define RGBA	4

namespace Rasterizer {

	// ------------------------------------------------------------------------
	/*! FS Texture VS Color Modulation
	*
	*   Modulates the Texture Colors with the Background Colors
	*/ // --------------------------------------------------------------------
	Color TextureSampler::SampleBorderColor(const AEVec2& texCoord) {

		//calculate the UV axis values
		const unsigned long uv_[UV] = {static_cast<unsigned long>(Floor(texCoord.x * (mpTexture->mWidth - 1))), 
			static_cast<unsigned long>(Floor(texCoord.y * (mpTexture->mHeight - 1)))};

		//if we are withtin image borders
		if (uv_[U] < mpTexture->mWidth && uv_[V] < mpTexture->mHeight) {

			//Calculate the pixel index
			const unsigned char* pixel_ = mpTexture->mPixels +
				(uv_[V] * mpTexture->mWidth + uv_[U]) * RGBA;

			// Return the Color (Normalized)
			return { static_cast<float>(pixel_[R]) / UCHAR_MAX,
				static_cast<float>(pixel_[G]) / UCHAR_MAX,
				static_cast<float>(pixel_[B]) / UCHAR_MAX,
				static_cast<float>(pixel_[A]) / UCHAR_MAX };

		}

		//return the Border Color
		return mBorderColor;
	
	}

	// ------------------------------------------------------------------------
	/*! Sample Repeat
	*
	*   Repeats an image until the shader is filled
	*/ // --------------------------------------------------------------------
	Color TextureSampler::SampleRepeat(const AEVec2& texCoord) {

		//find the UV axis coordinates (in normalized values)
		unsigned long uv_[UV] = {static_cast<unsigned long>(Floor((texCoord.x - 
			static_cast<int>(texCoord.x)) * (mpTexture->mWidth - 1))),
			static_cast<unsigned long>(Floor((texCoord.y - 
			static_cast<int>(texCoord.y))* (mpTexture->mHeight - 1))) };

		//If we are within the positive values of U and V
		if (uv_[U] < mpTexture->mWidth && uv_[V] < mpTexture->mHeight) {

			//Calculate the pixel index
			const unsigned char* pixel_ = mpTexture->mPixels +
				(uv_[V] * mpTexture->mWidth + uv_[U]) * RGBA;

			//Return the Pixel Color (Color is Normalized)
			return { static_cast<float>(pixel_[R]) / UCHAR_MAX,
				static_cast<float>(pixel_[G]) / UCHAR_MAX,
				static_cast<float>(pixel_[B]) / UCHAR_MAX,
				static_cast<float>(pixel_[A]) / UCHAR_MAX };

		}

		//Obtain the offset of the images (in image count) for U
		const unsigned long base_U_ = abs(Floor(texCoord.x));

		//Obtain the offset of the images (in image count) for V
		const unsigned long base_V_ = abs(Floor(texCoord.y)); 

		//Get the new U and V coordinates (if either U or V are negative) using the new offset
		uv_[U] = Floor(((base_U_ + texCoord.x) - static_cast<int>(base_U_ + texCoord.x))* (mpTexture->mWidth - 1));
		uv_[V] = Floor(((base_V_ + texCoord.y) - static_cast<int>(base_V_ + texCoord.y))* (mpTexture->mHeight - 1));

		//calculate the new pixel index
		const unsigned char* pixel_ = mpTexture->mPixels +
			(uv_[V] * mpTexture->mWidth + uv_[U]) * RGBA;

		//Return the Pixel Color (Color is Normalized)
		return { static_cast<float>(pixel_[R]) / UCHAR_MAX,
			static_cast<float>(pixel_[G]) / UCHAR_MAX,
			static_cast<float>(pixel_[B]) / UCHAR_MAX,
			static_cast<float>(pixel_[A]) / UCHAR_MAX };

	}

	// ------------------------------------------------------------------------
	/*! Clamp
	*
	*   Clamps a value between a minimun and a maximun
	*/ // --------------------------------------------------------------------
	constexpr const float inline Clamp(const float n, const float low, const float up) {

		//if the upper value is lower than n
		if(up < n)
			
			//return the upper value
			return up;

		//if the lower value is higher than n
		if(low > n)

			//return the lower value
			return low;

		//return n
		return n;

	}

	// ------------------------------------------------------------------------
	/*! Sample Clamp
	*
	*   Repeats an image until the shader is filled
	*/ // --------------------------------------------------------------------
	Color TextureSampler::SampleClamp(const AEVec2& texCoord) {

		//get the pixel index
		const unsigned char* pixel_ = mpTexture->mPixels +
			(Floor(Clamp(texCoord.y, 0.f, 1.f) * (mpTexture->mHeight - 1)) * 
			mpTexture->mWidth +
			Floor(Clamp(texCoord.x, 0.f, 1.f) * (mpTexture->mWidth - 1))) * RGBA;

		//Return the Pixel Color (Color is Normalized)
		return { static_cast<float>(pixel_[R]) / UCHAR_MAX,
			static_cast<float>(pixel_[G]) / UCHAR_MAX,
			static_cast<float>(pixel_[B]) / UCHAR_MAX,
			static_cast<float>(pixel_[A]) / UCHAR_MAX };
	
	}

	// ------------------------------------------------------------------------
	/*! Sample Mirror
	*
	*   Repeats an image mirrowed in the directions given
	*/ // --------------------------------------------------------------------
	Color TextureSampler::SampleMirror(const AEVec2& texCoord, bool horizontal, bool vertical) {

		//find the UV axis coordinates (in normalized values)
		unsigned long uv_[UV] = {static_cast<unsigned long>(Floor((texCoord.x - static_cast<int>(texCoord.x)) * (mpTexture->mWidth - 1))),
			static_cast<unsigned long>(Floor((texCoord.y - static_cast<int>(texCoord.y)) * (mpTexture->mHeight - 1)))};

		//if we are in positive values of U and V
		if (uv_[U] < mpTexture->mWidth && uv_[V] < mpTexture->mHeight) {

			//if we are in an odd number of picture (counting horizontaly) and we want a horizontal reflection
			if(horizontal && Floor(texCoord.x) & 1)

				//invert the U coordinate (locally)
				uv_[U] = Floor((1 - (texCoord.x - static_cast<int>(texCoord.x))) * (mpTexture->mWidth - 1));

			//if we are in an odd number of picture (counting verticaly) and we want a vertical reflection
			if(vertical && Floor(texCoord.y) & 1)
				
				//invert the V coordinate (locally)
				uv_[V] = Floor((1 - (texCoord.y - static_cast<int>(texCoord.y))) * (mpTexture->mHeight - 1));

			//compute the pixel index
			const unsigned char* pixel = mpTexture->mPixels +
				(uv_[V] * mpTexture->mWidth + uv_[U]) * RGBA;

			//Return the Pixel Color (Color is Normalized)
			return { static_cast<float>(pixel[R]) / UCHAR_MAX,
				static_cast<float>(pixel[G]) / UCHAR_MAX,
				static_cast<float>(pixel[B]) / UCHAR_MAX,
				static_cast<float>(pixel[A]) / UCHAR_MAX };

		}

		//Obtain the offset of the images (in image count) for U
		const unsigned base_U_ = abs(Floor(texCoord.x));

		//Obtain the offset of the images (in image count) for V
		const unsigned base_V_ = abs(Floor(texCoord.y));

		//if we are in an odd number of picture (counting horizontaly) and we want a horizontal reflection
		if(horizontal && (base_U_ & 1))

			//invert the U coordinate (locally)
			uv_[U] = Floor((1 - ((base_U_ + texCoord.x) - static_cast<int>(base_U_ + texCoord.x))) * (mpTexture->mWidth - 1));

		//else
		else 

			//Get the new U oordinate (if either U or V are negative) using the new offset
			uv_[U] = Floor(((base_U_ + texCoord.x) - static_cast<int>(base_U_ + texCoord.x)) * (mpTexture->mWidth - 1));

		//if we are in an odd number of picture (counting verticaly) and we want a vertical reflection
		if(vertical && (base_V_ & 1))

			//invert the V coordinate (locally)
			uv_[V] = Floor((1 - ((base_V_ + texCoord.y) - static_cast<int>(base_V_ + texCoord.y)))* (mpTexture->mHeight - 1));
		
		//else
		else

			//Get the new V oordinate (if either U or V are negative) using the new offset
			uv_[V] = Floor(((base_V_ + texCoord.y) - static_cast<int>(base_V_ + texCoord.y))* (mpTexture->mHeight - 1));

		//Return the Pixel Color (Color is Normalized)
		const unsigned char* pixel = mpTexture->mPixels +
			(uv_[V] * mpTexture->mWidth + uv_[U]) * RGBA;

		//Return the Pixel Color (Color is Normalized)
		return { static_cast<float>(pixel[R]) / UCHAR_MAX ,
			static_cast<float>(pixel[G]) / UCHAR_MAX ,
			static_cast<float>(pixel[B]) / UCHAR_MAX ,
			static_cast<float>(pixel[A]) / UCHAR_MAX };
	
	}

	// ------------------------------------------------------------------------
	/*! Sample
	*
	*   Sample an image given the texture coordinates
	*/ // --------------------------------------------------------------------
	Color TextureSampler::Sample(const AEVec2 & texCoord) {

		//if we have a texture to sample
		if(mpTexture)

			//switch by the bordemode that we are using
			switch (mBorderMode) {

			//case using repeating border
			case Rasterizer::TextureSampler::eRepeat:

				//retunr a Repeat Case Sampler
				return SampleRepeat(texCoord);

			//case using clamp border
			case Rasterizer::TextureSampler::eClamp:

				//return a Clamp Case Sampler
				return SampleClamp(texCoord);

			//case using horizontal border
			case Rasterizer::TextureSampler::eMirrorWidthOnly:

				//retunr a Horizontal Mirror Case Sampler
				return SampleMirror(texCoord, true, false);
				
			//case using vertical border
			case Rasterizer::TextureSampler::eMirrorHeightOnly:
				
				//return a Vertical Mirror Case Sampler
				return SampleMirror(texCoord, false, true);

			//case using horizontal and vertical border
			case Rasterizer::TextureSampler::eMirrorWidthHeight:
				
				//return a Horizontal-Vertial Mirror Case Sampler
				return SampleMirror(texCoord, true, true);
				
			//??
			case Rasterizer::TextureSampler::eCOUNT:
				
				//Quit and return a Blank color
				break;

			//Default Case
			default:

				//return a Border Color Case Sampler
				return SampleBorderColor(texCoord);

			}

		//return a blank color
		return Color();

	}
}