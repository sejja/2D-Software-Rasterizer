#ifndef _FRAGMENT_SHADERS_H_
#define _FRAGMENT_SHADERS_H_
// ----------------------------------------------------------------------------

#include <AEEngine.h>

namespace Rasterizer
{
	struct Color;
	class TextureSampler;

	namespace Shaders
	{

		struct TexTransformShaderData {
			Rasterizer::TextureSampler* pSampler;
			AEMtx33 texTransform;
		};


		/// TODO
		// ------------------------------------------------------------------------
		/// \fn		FSFragmentColor
		/// \brief	Returns the fragment color.  
		///	\param	fragData float array representing the fragment. The memory
		///			layout of this array is expected to math the one of 
		///			Raterizer::VertexDefs::VtxPosColor
		///	\param	userData	Data specific to the shader. Expected to be NULL
		Color FSFragmentColor(float* fragData, void* userData = 0);

		/// TODO
		// ------------------------------------------------------------------------
		/// \fn		FSFragmentColorInv
		/// \brief	Returns the fragment inverse color
		///	\param	fragData float array representing the fragment. The memory
		///			layout of this array is expected to math the one of 
		///			Raterizer::VertexDefs::VtxPosColor.  
		///	\param	userData	Data specific to the shader. Expected to be NULL
		Color FSFragmentColorInv(float* fragData, void* userData = 0);


		// TODO
		// ------------------------------------------------------------------------
		/// \fn		FSAlphaBlend
		/// \brief	Returns the resulting color of performing alpha blending 
		///			of the fragment color (RGBA) and the frame buffer color (RGB).
		Color FSAlphaBlend(float* fragData, void* userData = 0);

		/// TODO
		// ------------------------------------------------------------------------
		/// \fn		FSTextureMap
		/// \brief	Returns the color from the texture sampler
		///	\param	fragData float array representing the fragment. The memory
		///			layout of this array is expected to math the one of 
		///			Raterizer::VertexDefs::VtxPosTexCol.  
		///	\param	userData	Data specific to the shader. Expected to point
		///			to a TextureSampler class. 
		Color FSTextureMap(float* fragData, void* userData = 0);

		/// TODO
		// ------------------------------------------------------------------------
		/// \fn		FSTextureDecal
		/// \brief	returns the resulting color of performing the decal blending 
		///			described in the class notes, between the fragment color and the
		///			texture color.
		///	\param	fragData float array representing the fragment. The memory
		///			layout of this array is expected to math the one of 
		///			Raterizer::VertexDefs::VtxPosTexCol.  
		///	\param	userData	Data specific to the shader. Expected to point
		///			to a TextureSampler class. 
		Color FSTextureDecal(float* fragData, void* userData = 0);

		/// TODO
		// ------------------------------------------------------------------------
		/// \fn		FSTextureVSColorMod
		/// \brief	Returns the resulting color of performing color modulation 
		///			as described in the class notes, between the fragment color
		///			and the texture color.
		///	\param	fragData float array representing the fragment. The memory
		///			layout of this array is expected to math the one of 
		///			Raterizer::VertexDefs::VtxPosTexCol.  
		///	\param	userData	Data specific to the shader. Expected to point
		///			to a TextureSampler class. 
		Color FSTextureVSColorMod(float* fragData, void* userData = 0);

		/// TODO
		// ------------------------------------------------------------------------
		/// \fn		FSTextureTransform
		/// \brief	Samples a texture using transformed coordinates
		///	\param	fragData float array representing the fragment. The memory
		///			layout of this array is expected to math the one of 
		///			Raterizer::VertexDefs::VtxPosTexCol.  
		///	\param	userData	Pointer to the TexTransformShaderData below
		Color FSTextureTransform(float* fragData, void* userData = 0);

	} // namespace Shaders
} // namespace Ratsterizer


// ----------------------------------------------------------------------------
#endif