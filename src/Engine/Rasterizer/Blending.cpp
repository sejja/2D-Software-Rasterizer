//
//  Blending.cpp
//  2D Rasterizer
//
//  Created by Diego Revilla on 27/06/2020
//  Copyright � 2020 . All rights reserved.
//

#include "Blending.h"
#include "Color.h"

namespace Rasterizer {

	/**********************************************************

		Global Variables

		-BlendFunc
			Stores a global Blend Function

		-SrcParm
			Stores a Source Parameter

		-DstParm
			Stores a Destiny Parameter

	**********************************************************/
	static EBlendFunction BlendFunc = EBlendFunction::eBF_ADD;
	static EBlendParam SrcParm = EBlendParam::eBP_Count;
	static EBlendParam DstParm = EBlendParam::eBP_Count;

	// ------------------------------------------------------------------------
	/*! Set Blend Function
	*
	*   Sets the Blend Function to the given
	*/ // --------------------------------------------------------------------
	void SetBlendFunction(EBlendFunction fn) {
	
		//Blending Function =  Given Function
		BlendFunc = fn;

	}
 
	// ------------------------------------------------------------------------
	/*! Get Blend Function
	*
	*   Returns the Current Blending Function
	*/ // --------------------------------------------------------------------
	EBlendFunction GetBlendFunction() {

		//Gives the Blending Function
		return BlendFunc;
	
	}

	// ------------------------------------------------------------------------
	/*! Set Blend Parameters
	*
	*   Sets the new Blending Parameters
	*/ // --------------------------------------------------------------------
	void SetBlendParameters(EBlendParam srcParam, EBlendParam dstParam) {

		//Sets the new Source Parameter
		SrcParm = srcParam;

		//Sets the new Destiny Parameter
		DstParm = dstParam;

	}

	// ------------------------------------------------------------------------
	/*! Get Blend Parameters
	*
	*   Gets the Blending Parameters
	*/ // --------------------------------------------------------------------
	void GetBlendParameters(EBlendParam& outSrcParam, EBlendParam& outDstParam) {

		//Rewrites the out Source Parameter
		outSrcParam = SrcParm;

		//Rewrites the out Destiny Parameter
		outDstParam = DstParm;

	}

	// ------------------------------------------------------------------------
	/*! Blend Color
	*
	*   Blends a new Color
	*/ // --------------------------------------------------------------------
	Color BlendColor(Color srcColor, Color dstColor) {

		//call the explicit version of Blend Color
		return BlendColor(srcColor, dstColor, BlendFunc, SrcParm, DstParm);
	
	}

	// ------------------------------------------------------------------------
	/*! Get Color From Parameter
	*
	*   Returns a Color given the Blending Parameter
	*/ // --------------------------------------------------------------------
	const Color GetColorFromParam(const Color& srcColor, const Color& dstColor, const EBlendParam Param) {

		//Switch by the Blending Parameter
		switch (Param) {

		//case One
		case Rasterizer::eBP_ONE:
			
			//return a white Color
			return {1, 1, 1, 1};

		//case Source Color
		case Rasterizer::eBP_SRC_COLOR:
			
			//return the Source Color
			return srcColor;

		//case Destiny Color
		case Rasterizer::eBP_DST_COLOR:
			
			//return the Destiny Color
			return dstColor;

		//case Source Alpha
		case Rasterizer::eBP_SRC_ALPHA:
			
			//return the Source Alpha as a Color
			return {srcColor.a, srcColor.a, srcColor.a, srcColor.a };

		//case Destiny Alpha
		case Rasterizer::eBP_DST_ALPHA:
			
			//retun the destiny alpha as a color
			return {dstColor.a, dstColor.a, dstColor.a, dstColor.a };

		//Case inverse Source
		case Rasterizer::eBP_INV_SRC_COLOR:

			//return the Inverse of the Source Color
			return { 1 - srcColor.r, 1 - srcColor.g, 1 - srcColor.b, 1 - srcColor.a };
			
		//case inverse Destination
		case Rasterizer::eBP_INV_DST_COLOR:
			
			//return the inverse of the destination color
			return { 1 - dstColor.r, 1 - dstColor.g, 1 - dstColor.b, 1 - dstColor.a };
			
		//case Inverse Source Alpha
		case Rasterizer::eBP_INV_SRC_ALPHA:
			
			//return the inverse of the source's alpha as a color
			return { 1 - srcColor.a, 1 - srcColor.a, 1 - srcColor.a, 1 - srcColor.a };

		//case inverse destiny Alpha
		case Rasterizer::eBP_INV_DST_ALPHA:

			//return the inverse of the destinty's alpha as a color
			return { 1 - dstColor.a, 1 - dstColor.a, 1 - dstColor.a, 1 - dstColor.a };

		//default case
		default:

			//Return Black Color
			return {0.f, 0.f, 0.f, 0.f};
			
		}
	}

	// ------------------------------------------------------------------------
	/*! Max
	*
	*   Returns the highest floating point value betwen two 
	*/ // --------------------------------------------------------------------
	constexpr const float inline max(const float x, const float y) {

		//return the highest value variable
		return x > y ? x : y;

	}

	// ------------------------------------------------------------------------
	/*! Min
	*
	*   Returns the lowest floating point value betwen two
	*/ // --------------------------------------------------------------------
	constexpr const float inline min(const float x, const float y) {

		//retun the lowest value variable
		return x > y ? y : x;

	}

	// ------------------------------------------------------------------------
	/*! Max
	*
	*   Returns the highest floating point value betwen two
	*/ // --------------------------------------------------------------------
	Color BlendColor(Color srcColor, Color dstColor, EBlendFunction fn, EBlendParam srcParam, EBlendParam dstParam) {

		//Switch by the Blending Function
		switch (fn) {
		
		//Case substract
		case Rasterizer::eBF_SUBTRACT:

			//perform the substract formula
			return srcColor * GetColorFromParam(srcColor, dstColor, srcParam) - 
				dstColor * GetColorFromParam(srcColor, dstColor, dstParam);

		//Case reverse substract
		case Rasterizer::eBF_REVERSE_SUBTRACT:

			//perform the reverse substract formula
			return (const Color(0, 0, 0, 0) - srcColor) * GetColorFromParam(srcColor, dstColor, srcParam) - 
				dstColor * GetColorFromParam(srcColor, dstColor, dstParam);

		//Case minimun
		case Rasterizer::eBF_MIN:
			{
			
				//Calculate Destiny Color
				const Color D = dstColor * GetColorFromParam(srcColor, dstColor, dstParam),
					S = srcColor * GetColorFromParam(srcColor, dstColor, srcParam);

				//get always the minimun value
				return {min(D.r, S.r), min(D.g, S.g), min(D.b, S.b), min(D.a, S.a)};

			}

		//Case maximun
		case Rasterizer::eBF_MAX:
			{

				//Calculate the Destiny Color
				const Color D = dstColor * GetColorFromParam(srcColor, dstColor, dstParam),
					S = srcColor * GetColorFromParam(srcColor, dstColor, srcParam);

				//get always the maximun value
				return { max(D.r, S.r), max(D.g, S.g), max(D.b, S.b), max(D.a, S.a) };

			}

		//Default case
		default:
		
			//perform the default Formula
			return (srcColor * GetColorFromParam(srcColor, dstColor, srcParam)) + 
				(dstColor * GetColorFromParam(srcColor, dstColor, dstParam));

		}
	}
}