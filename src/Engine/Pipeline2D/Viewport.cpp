//
//  Viewport.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 08/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include "../Rasterizer/Color.h"
#include "Viewport.h"

namespace Rasterizer {
	
	// ------------------------------------------------------------------------
	/*! Compute Matrix
	*
	*   Computes the Matrix of the Viewport Transformation
	*/ // --------------------------------------------------------------------
	AEMtx33 Viewport::ComputeMatrix() {

		//Computes the transformation concatenation given by: 
		//Offset * Scale * Translation
		return AEMtx33::Translate(mPosition.x, mPosition.y) * 
			AEMtx33::Scale(mDimensions.x, mDimensions.y) *
			AEMtx33::Translate(.5f, .5f);
	
	}

	// ------------------------------------------------------------------------
	/*! Compute Inverse Matrix
	*
	*   Computes the Inverse Matrix of the Viewport Transformation
	*/ // --------------------------------------------------------------------
	AEMtx33 Viewport::ComputeInvMatrix() {

		//Computes the Inverse of the transformation concatenation
		//given by: Translation(Inverse) * Scale(Inverse) * Offset(Inverse)
		return AEMtx33::Translate(-.5f, -.5f) *
			AEMtx33::Scale(1.f / mDimensions.x, 1.f / mDimensions.y) *
			AEMtx33::Translate(-mPosition.x, -mPosition.y);
	
	}
}