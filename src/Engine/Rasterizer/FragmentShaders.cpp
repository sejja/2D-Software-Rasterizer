//
//  Fragment Shaders.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 04/06/2020
//  Copyright � 2020 . All rights reserved.
//

#include "FragmentShaders.h"
#include "Rasterizer.h"

#define X	0
#define Y	X + 1
#define R	Y + 1
#define G	R + 1
#define B	G + 1
#define A	B + 1

namespace Rasterizer {

	// ------------------------------------------------------------------------
	/*! FS Fragment Color
	*
	*   Returns the Color as is
	*/ // --------------------------------------------------------------------
	Color Shaders::FSFragmentColor(float* fragData, void* userData) {

		//returns the Fragment Values as a Color
		return *reinterpret_cast<Color*>(fragData + R);

	}

	// ------------------------------------------------------------------------
	/*! FS Fragment Color Inverse
	*
	*   Returns the Color inverted
	*/ // --------------------------------------------------------------------
	Color Shaders::FSFragmentColorInv(float* fragData, void* userData) {

		//inverts the fragment values and convert them into a color
		return Color(1 - fragData[R], 1 - fragData[G], 1 - fragData[B], 
			1 - fragData[A]);
	 
	}

	// ------------------------------------------------------------------------
	/*! FS Alpha Blend
	*
	*   Performs an Alpha Blend with the Previous FrameBuffer Color
	*/ // --------------------------------------------------------------------
	Color Shaders::FSAlphaBlend(float* fragData, void* userData) {

		//Gets the previous color at the position given
		const Color PrevCol_ = FrameBuffer::GetPixel(reinterpret_cast<unsigned*>(fragData)[X], 
			reinterpret_cast<unsigned*>(fragData)[Y]);

		//Performs a blend with the previous color and the new color
		return Color(fragData[R] + fragData[A] * (PrevCol_.r - fragData[R]),
			fragData[G] + fragData[A] * (PrevCol_.g - fragData[G]),
			fragData[B] + fragData[A] * (PrevCol_.b - fragData[B]),
			fragData[A] + fragData[A] * (PrevCol_.a - fragData[A]));
	
	}

	// ------------------------------------------------------------------------
	/*! FS Texture Map
	*
	*   Returns the Color of a texture given a Map
	*/ // --------------------------------------------------------------------
	Color Shaders::FSTextureMap(float* fragData, void* userData) {

		//Sampñe the Texture given the coordinates
		return reinterpret_cast<TextureSampler*>(userData)->
			Sample(reinterpret_cast<VertexDefs::VtxPosTexCol*>(fragData)->mTexCoord);

	}

	#define PTC_COL_IDX	4

	// ------------------------------------------------------------------------
	/*! FS Texture Decal
	*
	*   Returns the Color that is achieved using the texture as a Decal
	*/ // --------------------------------------------------------------------
	Color Shaders::FSTextureDecal(float* fragData, void* userData) {

		//Get the Texture and Background Color
		const Color CS(*reinterpret_cast<Color*>(fragData + PTC_COL_IDX)),
			CL(reinterpret_cast<TextureSampler*>(userData)->
			Sample(reinterpret_cast<VertexDefs::VtxPosTexCol*>(fragData)->mTexCoord));

		//Lerp both based on Texture alpha
		return {CS.r + CL.a * (CL.r - CS.r), CS.g + CL.a * (CL.g - CS.g), 
			CS.b + CL.a * (CL.b - CS.b), CS.r + CL.a * (CL.a - CS.a)};

	}
	
	// ------------------------------------------------------------------------
	/*! FS Texture VS Color Modulation
	*
	*   Modulates the Texture Colors with the Background Colors
	*/ // --------------------------------------------------------------------
	Color Shaders::FSTextureVSColorMod(float* fragData, void* userData) {

		//Return the modulation between both colors
		return *reinterpret_cast<Color*>(fragData + PTC_COL_IDX) * 
			reinterpret_cast<TextureSampler*>(userData)->
			Sample(reinterpret_cast<VertexDefs::VtxPosTexCol*>(fragData)->mTexCoord);

	}

	// ------------------------------------------------------------------------
	/*! FS Texture Transform
	*
	*   Apply a Transform to the Texture
	*/ // --------------------------------------------------------------------
	Color Shaders::FSTextureTransform(float* fragData, void* userData) {

		/*Compute the Sample Color, transforming the UV's coordinates with the texture 
			transform*/
		return reinterpret_cast<TexTransformShaderData*>(userData)->pSampler->Sample(
			reinterpret_cast<TexTransformShaderData*>(userData)->texTransform *
			reinterpret_cast<VertexDefs::VtxPosTexCol*>(fragData)->mTexCoord);
	
	}
}