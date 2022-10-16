#ifndef _TEXTURE_SAMPLER_H_
#define _TEXTURE_SAMPLER_H_

namespace Rasterizer
{
	// !! forward declare Texture to be able to use it in our class!!
	struct Texture;

	/// TODO
	/// -----------------------------------------------------------------------
	/// \class	TextureSampler
	///	\brief	Class responsible of sampling the texture color based on some
	///			internal parameters like the border mode and user-provided 
	///			texture coordinates.
	class TextureSampler
	{
	public:

		///	PROVIDED
		/// -------------------------------------------------------------------
		/// \enum	EBorderMode
		///	\brief	Defines the different border mode implemented by this 
		///			sampler class. 
		enum EBorderMode {
			eBorderColor, eRepeat, eClamp, eMirrorWidthOnly,
			eMirrorHeightOnly, eMirrorWidthHeight, eCOUNT
		};

		///	PROVIDED
		/// -------------------------------------------------------------------
		///	\fn		TextureSampler
		/// \brief	Custom constructor
		///	\param	pTex -> Pointer to texture to sample.
		///	\param	borderMode -> Bordermode to use. 
		TextureSampler(Texture* pTex = NULL, EBorderMode borderMode = eBorderColor)
			: mpTexture(pTex)
			, mBorderMode(borderMode) {
			
		}


		/// TODO
		/// -------------------------------------------------------------------
		/// \fn		Sample
		/// \brief	Generic. Uses the stored border mode to decide which of the
		///			functions below to use.
		Color Sample(const AEVec2& texCoord);

		/// TODO
		/// -------------------------------------------------------------------
		/// \fn		SampleBorderColor
		/// \brief	Samples normally, but returns the border color if beyond 
		///			the range [0,1]
		Color SampleBorderColor(const AEVec2& texCoord);

		/// TODO
		/// -------------------------------------------------------------------
		/// \fn		SampleRepeat
		/// \brief	Samples a texture using the repeat border mode described 
		///			in class. 
		///	\param	texCoord -> texture coordinates to sample
		Color SampleRepeat(const AEVec2& texCoord);

		/// TODO
		/// -------------------------------------------------------------------
		/// \fn		SampleClamp
		/// \brief	Samples a texture using the clamp border mode described 
		///			in class. 
		///	\param	texCoord -> texture coordinates to sample
		Color SampleClamp(const AEVec2& texCoord);

		/// TODO
		/// -------------------------------------------------------------------
		/// \fn		SampleMirror
		/// \brief	Samples a texture using the mirroring border mode described 
		///			in class. 
		///	\param	texCoord -> texture coordinates to sample
		///	\param	horizontal	-> specifies whether horizontal mirroring should
		///			be applied.
		///	\param	vertical	-> specifies whether vertical mirroring should
		///			be applied.
		Color SampleMirror(const AEVec2& texCoord, bool horizontal, bool vertical);

		/// PROVIDED
		/// -------------------------------------------------------------------
#pragma region// GETTERS/SETTERS

		void SetTexture(Texture* pTex) { mpTexture = pTex; }
		Texture* GetTexture() { return mpTexture; }

		void SetBorderMode(EBorderMode borderMode) { mBorderMode = borderMode; }
		EBorderMode GetBorderMode() { return mBorderMode; }

		void SetBorderColor(const Color& c) { mBorderColor = c; }
		Color GetBorderColor() { return mBorderColor; }

#pragma endregion

	private:
		Texture* mpTexture;
		EBorderMode		mBorderMode;
		Color			mBorderColor;
	};
}

#endif