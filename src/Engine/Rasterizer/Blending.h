#ifndef _BLENDING_H_
#define _BLENDING_H_
// ----------------------------------------------------------------------------

namespace Rasterizer
{

	struct Color;

	/// PROVIDED
	/// -----------------------------------------------------------------------
	/// \enum	EBlendParam
	/// \brief	Define the Blend function type used in the blending equation. 
	///			These values mimic the ones in OpenGL. 
	enum EBlendFunction
	{
		eBF_ADD,				// O = sS + dD (DEFAULT)
		eBF_SUBTRACT,			// O = sS - dD
		eBF_REVERSE_SUBTRACT,	// O = dD - sS
		eBF_MIN,				// O = (min(S.r, D.r), min(S.g, D.g), min(S.b, D.b), min(S.a, D.a))
		eBF_MAX,				// O = (max(S.r, D.r), max(S.g, D.g), max(S.b, D.b), max(S.a, D.a))
		eBF_Count
	};

	/// PROVIDED
	/// -----------------------------------------------------------------------
	/// \enum	EBlendParam
	/// \brief	Defines all possbile values for the blending parameters of the
	///			blend equation. These values mimic the ones in OpenGL. 
	enum EBlendParam
	{
		eBP_ONE,			// P = (1,1,1,1)	(DEFAULT for source parameter)
		eBP_ZERO,			// P = (0,0,0,0)	(DEDAULT for destination parameter)
		eBP_SRC_COLOR,		// P = S
		eBP_DST_COLOR,		// P = D
		eBP_SRC_ALPHA,		// P = (S.a, S.a, S.a, S.a)
		eBP_DST_ALPHA,		// P = (D.a, D.a, D.a, D.a)
		eBP_INV_SRC_COLOR,	// P = (1,1,1,1) - S
		eBP_INV_DST_COLOR,	// P = (1,1,1,1) - D
		eBP_INV_SRC_ALPHA,	// P = (1,1,1,1) - (S.a, S.a, S.a, S.a)
		eBP_INV_DST_ALPHA,	// P = (1,1,1,1) - (D.a, D.a, D.a, D.a)
		eBP_Count
	};

	/// TODO
	/// -----------------------------------------------------------------------
	/// \fn		SetBlendFunction
	/// \brief	Sets the current blend function.
	///	\param	fn -> The new blend function.
	void SetBlendFunction(EBlendFunction fn);

	/// TODO
	/// -----------------------------------------------------------------------
	/// \fn		GetBlendFunction
	///	\brief	Returns the current blend function.
	EBlendFunction GetBlendFunction();

	/// TODO
	// ------------------------------------------------------------------------
	/// \fn		SetBledSetBlendParameters
	/// \brief	Sets the current blend parameters used by the rasterizer.
	/// \param	srcParam -> Blend parameter for the source color.
	/// \param	dstParam -> Blend parameter for the destination color.
	void SetBlendParameters(EBlendParam srcParam, EBlendParam dstParam);

	/// TODO
	/// -----------------------------------------------------------------------
	/// \fn		SetBledSetBlendParameters
	/// \brief	Sets the current blend parameters used by the rasterizer.
	/// \param	outSrcParam -> Output variable to store the blend parameter for
	///			the source color.
	/// \param	outDstParam -> Output variable to store the blend parameter for
	///			the destination color.
	void GetBlendParameters(EBlendParam & outSrcParam, EBlendParam & outDstParam);

	/// TODO
	/// -----------------------------------------------------------------------
	///	\fn		BlendColor
	/// \brief	Perfoms blending equation using the current values for the blend
	///			equation (function, source and destination parameters). 
	/// \param	srcColor -> Source color as input to the blend equation.
	/// \param	dstColor -> Destination color as input to the blend equation.
	Color BlendColor(Color srcColor, Color dstColor);

	/// TODO
	/// -----------------------------------------------------------------------
	///	\fn		BlendColor
	/// \brief	Perfoms blending equation using the provided values for the blend
	///			equation (function, source and destination parameters). 
	/// \param	srcColor -> Source color as input to the blend equation.
	/// \param	dstColor -> Destination color as input to the blend equation.
	/// \param	fn		 -> Blend function to use in the blend equation.
	/// \param	srcParam -> Source param to use in the blend equation.
	/// \param	dstParam -> Destination param to use in the blend equation.
	Color BlendColor(Color srcColor, Color dstColor, EBlendFunction fn, EBlendParam srcParam, EBlendParam dstParam);

}// namespace Raterizer

// ----------------------------------------------------------------------------
#endif