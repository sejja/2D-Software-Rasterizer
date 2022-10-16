//
//  DrawTriangle.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 04/06/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include "DrawTriangle.h"
#include "Rounding.h"
#include "FrameBuffer.h"
#include "Color.h"
#include "Vertex.h"

namespace Rasterizer {

	//Global Variable - Defines Drawing Method
	static EDrawTriangleMethod DrawingMethod = eDT_BILINEAR;

	// ------------------------------------------------------------------------
	/*! Vertex Inf
	*
	*   Determines the case in which the vertices are ordered
	*/ // --------------------------------------------------------------------
	template<typename Ty_>
	struct VertexInf {

		Ty_*Top, *Middle, *Bottom;
		bool MiddleIsLeft;
		
		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Default Vertex information (For AEVec2)
		*/ // --------------------------------------------------------------------
		VertexInf(const AEVec2& a, const AEVec2& b, const AEVec2& c) {

			//If the vertex 0 greater than 1
			if (a.y > b.y) {

				//If the vertex 1 greater than 2	
				if (b.y > c.y) {

					//Set the top to the first Vertex
					Top = const_cast<AEVec2*>(&a);

					//Set the middle to the second Vertex
					Middle = const_cast<AEVec2*>(&b);

					//Set the bottom to the third Vertex
					Bottom = const_cast<AEVec2*>(&c);

					//The middle is on the left?
					MiddleIsLeft = true;

				//If the vertex 0 greater than 2
				} else if (a.y > c.y) {

					//Set the top to the first Vertex
					Top = const_cast<AEVec2*>(&a);

					//Set the middle to the third Vertex
					Middle = const_cast<AEVec2*>(&c);

					//Set the bottom to the second Vertex
					Bottom = const_cast<AEVec2*>(&b);

					//The middle is on the left?
					MiddleIsLeft = false;

				//else
				} else {

					//Set the top to the third Vertex
					Top = const_cast<AEVec2*>(&c);

					//Set the middle to the first Vertex
					Middle = const_cast<AEVec2*>(&a);

					//Set the bottom to the second Vertex
					Bottom = const_cast<AEVec2*>(&b);

					//The middle is on the left?
					MiddleIsLeft = true;

				}

			//else
			} else {

				//If the vertex 0 greater than 2
				if (a.y > c.y) {

					//Set the top to the second Vertex
					Top = const_cast<AEVec2*>(&b);

					//Set the middle to the first Vertex
					Middle = const_cast<AEVec2*>(&a);

					//Set the bottom to the third Vertex
					Bottom = const_cast<AEVec2*>(&c);

					//The middle is on the left?
					MiddleIsLeft = false;

				//If the vertex 1 greater than 2
				} else if (b.y > c.y) {

					//Set the top to the second Vertex
					Top = const_cast<AEVec2*>(&b);

					//Set the middle to the third Vertex
					Middle = const_cast<AEVec2*>(&c);

					//Set the bottom to the first Vertex
					Bottom = const_cast<AEVec2*>(&a);

					//The middle is on the left?
					MiddleIsLeft = true;

				//else
				} else {

					//Set the top to the third Vertex
					Top = const_cast<AEVec2*>(&c);

					//Set the middle to the second Vertex
					Middle = const_cast<AEVec2*>(&b);

					//Set the bottom to the first Vertex
					Bottom = const_cast<AEVec2*>(&a);

					//The middle is on the left?
					MiddleIsLeft = false;

				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Default Vertex information (with Vertex)
		*/ // --------------------------------------------------------------------
		VertexInf(const Vertex& a, const Vertex& b, const Vertex& c) {

			//If the vertex 0 greater than 1
			if (a.mPosition.y > b.mPosition.y) {

				//If the vertex 1 greater than 2	
				if (b.mPosition.y > c.mPosition.y) {

					//Set the top to the first Vertex
					Top = const_cast<Vertex*>(&a);

					//Set the middle to the second Vertex
					Middle = const_cast<Vertex*>(&b);

					//Set the bottom to the third Vertex
					Bottom = const_cast<Vertex*>(&c);

					//The middle is on the left?
					MiddleIsLeft = true;

				//If the vertex 0 greater than 2
				} else if (a.mPosition.y > c.mPosition.y) {

					//Set the top to the first Vertex
					Top = const_cast<Vertex*>(&a);

					//Set the middle to the third Vertex
					Middle = const_cast<Vertex*>(&c);

					//Set the bottom to the second Vertex
					Bottom = const_cast<Vertex*>(&b);

					//The middle is on the left?
					MiddleIsLeft = false;

					//else
				} else {

					//Set the top to the third Vertex
					Top = const_cast<Vertex*>(&c);

					//Set the middle to the first Vertex
					Middle = const_cast<Vertex*>(&a);

					//Set the bottom to the second Vertex
					Bottom = const_cast<Vertex*>(&b);

					//The middle is on the left?
					MiddleIsLeft = true;

				}

			//else
			} else {

				//If the vertex 0 greater than 2
				if (a.mPosition.y > c.mPosition.y) {

					//Set the top to the second Vertex
					Top = const_cast<Vertex*>(&b);

					//Set the middle to the first Vertex
					Middle = const_cast<Vertex*>(&a);

					//Set the bottom to the third Vertex
					Bottom = const_cast<Vertex*>(&c);

					//The middle is on the left?
					MiddleIsLeft = false;

				//If the vertex 1 greater than 2
				} else if (b.mPosition.y > c.mPosition.y) {

					//Set the top to the second Vertex
					Top = const_cast<Vertex*>(&b);

					//Set the middle to the third Vertex
					Middle = const_cast<Vertex*>(&c);

					//Set the bottom to the first Vertex
					Bottom = const_cast<Vertex*>(&a);

					//The middle is on the left?
					MiddleIsLeft = true;

				//else
				} else {

					//Set the top to the third Vertex
					Top = const_cast<Vertex*>(&c);

					//Set the middle to the second Vertex
					Middle = const_cast<Vertex*>(&b);

					//Set the bottom to the first Vertex
					Bottom = const_cast<Vertex*>(&a);

					//The middle is on the left?
					MiddleIsLeft = false;

				}
			}
		}
	};

	// ------------------------------------------------------------------------
	/*! Vertex Inf B
	*
	*   Determines the case in which the vertices are ordered 
	*   (Optimized for Barycentric Coordinates)
	*/ // --------------------------------------------------------------------
	struct VertexInfB {

		Vertex* Top, * Bottom;

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Default Vertex information (with Vertex)
		*/ // --------------------------------------------------------------------
		VertexInfB(const Vertex& a, const Vertex& b, const Vertex& c) {

			//If the vertex 0 greater than 1
			if (a.mPosition.y > b.mPosition.y) {

				//If the vertex 1 greater than 2	
				if (b.mPosition.y > c.mPosition.y) {

					//Set the top to the first Vertex
					Top = const_cast<Vertex*>(&a);

					//Set the bottom to the third Vertex
					Bottom = const_cast<Vertex*>(&c);

				//If the vertex 0 greater than 2
				} else if (a.mPosition.y > c.mPosition.y) {

					//Set the top to the first Vertex
					Top = const_cast<Vertex*>(&a);

					//Set the bottom to the second Vertex
					Bottom = const_cast<Vertex*>(&b);

				//else
				} else {

					//Set the top to the third Vertex
					Top = const_cast<Vertex*>(&c);

					//Set the bottom to the second Vertex
					Bottom = const_cast<Vertex*>(&b);

				}

			//else
			} else {

				//If the vertex 0 greater than 2
				if (a.mPosition.y > c.mPosition.y) {

					//Set the top to the second Vertex
					Top = const_cast<Vertex*>(&b);

					//Set the bottom to the third Vertex
					Bottom = const_cast<Vertex*>(&c);

					//If the vertex 1 greater than 2
				} else if (b.mPosition.y > c.mPosition.y) {

					//Set the top to the second Vertex
					Top = const_cast<Vertex*>(&b);

					//Set the bottom to the first Vertex
					Bottom = const_cast<Vertex*>(&a);

				//else
				} else {

					//Set the top to the third Vertex
					Top = const_cast<Vertex*>(&c);

					//Set the bottom to the first Vertex
					Bottom = const_cast<Vertex*>(&a);

				}
			}
		}
	};

#define R				 0
#define	G				 1
#define B				 2
#define A				 3
#define CFLOAT_TO_U8(a)	 (a * 255.f)
#define RGBA			 4
#define LERP(a, b, c, d) (a - b) / (c - d)

	// ------------------------------------------------------------------------
	/*! Fill Triangle Naive
	*
	*   Fill a triangle in the naive way
	*/ // --------------------------------------------------------------------
	void FillTriangleNaive(const AEVec2& v0, const AEVec2& v1, 
		const AEVec2& v2, const Color& c) {

		//Crate a Vertex information Structure
		VertexInf<AEVec2> vinf_(v0, v1, v2);

		//Precalculate the colors
		u8 Col_[RGBA] = {static_cast<u8>(CFLOAT_TO_U8(c.r)),
			static_cast<u8>(CFLOAT_TO_U8(c.g)), static_cast<u8>(CFLOAT_TO_U8(c.b)),
			static_cast<u8>(CFLOAT_TO_U8(c.a))};

		//Get the ending Y value for the first section and it's begining
		unsigned yEnd_ = Round(vinf_.Middle->y), y_ = Round(vinf_.Top->y), cXR_;

		//Declare constraints and slopes
		float xL_ = vinf_.Top->x, xR_ = xL_, slopeLeft_ = vinf_.MiddleIsLeft ?
			LERP(vinf_.Middle->x, vinf_.Top->x, vinf_.Middle->y, vinf_.Top->y) :
			LERP(vinf_.Bottom->x, vinf_.Top->x, vinf_.Bottom->y, vinf_.Top->y),
		slopeRight_ = vinf_.MiddleIsLeft ? 
			LERP(vinf_.Bottom->x, vinf_.Top->x, vinf_.Bottom->y, vinf_.Top->y) :
			LERP(vinf_.Middle->x, vinf_.Top->x, vinf_.Middle->y, vinf_.Top->y);

		//Traverse through the first section of the triangle
		for (; y_ >= yEnd_; y_--) {

			//Get the rounded version of XR
			cXR_ = Round(xR_);
			
			//Traverse through the line 
			for (unsigned x = Round(xL_); x <= cXR_; x++)
				
				//Set all Pixels
				FrameBuffer::SetPixel(x, y_, Col_[R], Col_[G], Col_[B], Col_[A]);

			//Decrement by the slope left
			xL_ -= slopeLeft_;

			//DEcrement by the slope right
			xR_ -= slopeRight_;

		}

		//Is the middle is on the left
		if (vinf_.MiddleIsLeft) {

			//Set the new xL
			xL_ = vinf_.Middle->x;

			//Update the Left Slope
			slopeLeft_ = LERP(vinf_.Bottom->x, vinf_.Middle->x, 
				vinf_.Bottom->y, vinf_.Middle->y);

		//else
		} else {

			//Set the new xR
			xR_ = vinf_.Middle->x;

			//Update the Right Slope
			slopeRight_ = LERP(vinf_.Bottom->x, vinf_.Middle->x,
				vinf_.Bottom->y, vinf_.Middle->y);

		}

		//Set the new End (on Y)
		yEnd_ = Round(vinf_.Bottom->y);

		//go till the end
		for (; y_ >= yEnd_; y_--) {

			//ROund XR
			cXR_ = Round(xR_);

			//Traverse through the line
			for (unsigned x = Round(xL_); x <= cXR_; x++)

				//Set the right pixels
				FrameBuffer::SetPixel(x, y_, Col_[R], Col_[G], Col_[B], Col_[A]);

			//Update the limits
			xL_ -= slopeLeft_;

			//Update the limits
			xR_ -= slopeRight_;

		}
	}

	// ------------------------------------------------------------------------
	/*! Fill Triangle Top Left
	*
	*   Fill a triangle using the top left method
	*/ // --------------------------------------------------------------------
	void FillTriangleTopLeft(const AEVec2& v0, const AEVec2& v1, 
		const AEVec2& v2, const Color& c) {

		//Get the Vertex information
		VertexInf<AEVec2> vinf_(v0, v1, v2);

		//Precalculate the colors
		u8 Col_[RGBA] = {static_cast<u8>(CFLOAT_TO_U8(c.r)),
			static_cast<u8>(CFLOAT_TO_U8(c.g)), static_cast<u8>(CFLOAT_TO_U8(c.b)),
			static_cast<u8>(CFLOAT_TO_U8(c.a))};

		//Calculate first part's end Y and Start Y
		unsigned yEnd_ = Ceiling(vinf_.Middle->y), y_ = Ceiling(vinf_.Top->y), cXR;
		
		//Let's find the slopes and the boundaries
		float xL_ = vinf_.Top->x, xR_ = xL_, 
		slopeLeft_ = vinf_.MiddleIsLeft ?
			LERP(vinf_.Middle->x, vinf_.Top->x, vinf_.Middle->y, vinf_.Top->y) :
			LERP(vinf_.Bottom->x, vinf_.Top->x, vinf_.Bottom->y, vinf_.Top->y),
		slopeRight_ = vinf_.MiddleIsLeft ?
			LERP(vinf_.Bottom->x, vinf_.Top->x, vinf_.Bottom->y, vinf_.Top->y) :
			LERP(vinf_.Middle->x, vinf_.Top->x, vinf_.Middle->y, vinf_.Top->y);

		//Traverse through the first part
		for (; y_ >= yEnd_; y_--) {

			//Find the Rounded XR
			cXR = Floor(xR_);

			//Draw a line
			for (unsigned x = Floor(xL_); x < cXR; x++)
				
				//Set the individual pixel
				FrameBuffer::SetPixel(x, y_, Col_[R], Col_[G], Col_[B], Col_[A]);

			//Extend Boundaries (Left)
			xL_ -= slopeLeft_;

			//Extended Boundaries (Right)
			xR_ -= slopeRight_;

		}

		//If the middle is on the left side
		if (vinf_.MiddleIsLeft) {

			//Update the Boundaries
			xL_ = vinf_.Middle->x;

			//Update the Slope
			slopeLeft_ = LERP(vinf_.Bottom->x, vinf_.Middle->x,
				vinf_.Bottom->y, vinf_.Middle->y);

		//else
		} else {

			//Update the Boundaries
			xR_ = vinf_.Middle->x;

			//Update the Slope
			slopeRight_ = LERP(vinf_.Bottom->x, vinf_.Middle->x,
				vinf_.Bottom->y, vinf_.Middle->y);

		}

		//Set the End Y to the ending of the second part
		yEnd_ = Ceiling(vinf_.Bottom->y) + 1;

		//Traverse through the second part
		for (; y_ >= yEnd_; y_--) {

			//Round the X Boundary
			cXR = Floor(xR_);

			//Draw a line
			for (unsigned x = Floor(xL_); x < cXR; x++)
				
				//Set the pixels
				FrameBuffer::SetPixel(x, y_, Col_[R], Col_[G], Col_[B], Col_[A]);

			//Extend the Boundaries (Left)
			xL_ -= slopeLeft_;

			//Extend the Boundaries (Right)
			xR_ -= slopeRight_;

		}
	}

	// ------------------------------------------------------------------------
	/*! Get Draw Triangle Method
	*
	*   Get the Draw Triangle Method
	*/ // --------------------------------------------------------------------
	EDrawTriangleMethod GetDrawTriangleMethod() {

		//Return the used drawing method
		return DrawingMethod;
	
	}

	// ------------------------------------------------------------------------
	/*! Set Draw Triangle Method
	*
	*   Sets the Drawing Method for Triangles
	*/ // --------------------------------------------------------------------
	void SetDrawTriangleMethod(EDrawTriangleMethod dtm) {
		
		//Set the drawing method
		DrawingMethod = dtm;

	}

	// ------------------------------------------------------------------------
	/*! Draw Triangle
	*
	*   Draws a triangle
	*/ // --------------------------------------------------------------------
	void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {

		//switcg by the preferred Drawing Method
		switch (DrawingMethod) {

		//case Bi-linear
		case Rasterizer::eDT_BILINEAR:
			
			//Draw a Triangle using Bi-linear interpolation
			DrawTriangleBiLinear(v0, v1, v2);

			//break
			break;

		//case Plane Normal
		case Rasterizer::eDT_PLANE_NORMAL:
			
			//Draw a Triangle using the Plane Normal Method
			DrawTrianglePlaneNormal(v0, v1, v2);

			//break;
			break;

		//Any other case
		default:

			//Draw a Triangle using Barycentric Coordinates
			DrawTriangleBarycentric(v0, v1, v2);

		}
	}

	// ------------------------------------------------------------------------
	/*! Draw Triangle Bilinear
	*
	*   Draws a triangle using Bilinear interpolation
	*/ // --------------------------------------------------------------------
	void DrawTriangleBiLinear(const Vertex& v0, const Vertex& v1, const Vertex& v2) {

		//Get the Vertex information
		const VertexInf<Vertex> vinf_(v0, v1, v2);

		//find the Ending Y and the Starting Y
		unsigned yEnd_ = Ceiling(vinf_.Middle->mPosition.y) ,
		y_ = Ceiling(vinf_.Top->mPosition.y), cXR_;

		/*Calculate the triangle's borders, as well as it's current slopes and
		it's pixel gradient steps and some precomputed data*/
		float xL_ = vinf_.Top->mPosition.x, xR_ = xL_,
		SlopeLeft_ = vinf_.MiddleIsLeft ? 
		LERP(vinf_.Middle->mPosition.x, vinf_.Top->mPosition.x, 
		vinf_.Middle->mPosition.y, vinf_.Top->mPosition.y)
		: LERP(vinf_.Bottom->mPosition.x, vinf_.Top->mPosition.x, 
		vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y),
		SlopeRight_ = vinf_.MiddleIsLeft ? 
		LERP(vinf_.Bottom->mPosition.x, vinf_.Top->mPosition.x,
		vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y)
		: LERP(vinf_.Middle->mPosition.x, vinf_.Top->mPosition.x, 
		vinf_.Middle->mPosition.y, vinf_.Top->mPosition.y),
			Step_[RGBA], Xdiff_;

		//Find The Slope of every component of every color
		const float StepTB_[RGBA] = { 
			LERP(vinf_.Bottom->mColor.r, vinf_.Top->mColor.r,
			vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y),
			LERP(vinf_.Bottom->mColor.g, vinf_.Top->mColor.g,
			vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y) ,
			LERP(vinf_.Bottom->mColor.b, vinf_.Top->mColor.b,
			vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y) ,
			LERP(vinf_.Bottom->mColor.a, vinf_.Top->mColor.a,
			vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y) },
		StepTM_[RGBA] = { 
			LERP(vinf_.Top->mColor.r, vinf_.Middle->mColor.r,
			vinf_.Top->mPosition.y, vinf_.Middle->mPosition.y),
			LERP(vinf_.Top->mColor.g, vinf_.Middle->mColor.g,
			vinf_.Top->mPosition.y, vinf_.Middle->mPosition.y) ,
			LERP(vinf_.Top->mColor.b, vinf_.Middle->mColor.b,
			vinf_.Top->mPosition.y, vinf_.Middle->mPosition.y) ,
			LERP(vinf_.Top->mColor.a, vinf_.Middle->mColor.a,
			vinf_.Top->mPosition.y, vinf_.Middle->mPosition.y) },
		StepMB_[RGBA] = { 
			LERP(vinf_.Bottom->mColor.r, vinf_.Middle->mColor.r,
			vinf_.Bottom->mPosition.y, vinf_.Middle->mPosition.y),
			LERP(vinf_.Bottom->mColor.g, vinf_.Middle->mColor.g,
			vinf_.Bottom->mPosition.y, vinf_.Middle->mPosition.y) ,
			LERP(vinf_.Bottom->mColor.b, vinf_.Middle->mColor.b,
			vinf_.Bottom->mPosition.y, vinf_.Middle->mPosition.y) ,
			LERP(vinf_.Bottom->mColor.a, vinf_.Middle->mColor.a,
			vinf_.Bottom->mPosition.y, vinf_.Middle->mPosition.y) };

		//Declare the Left and Right interpolant colors as well as it's gradient
		Color ColL_ = vinf_.Top->mColor, ColR_ = vinf_.Top->mColor,
		Gradient_ = vinf_.Top->mColor;

		//traverse through the first part of the triangle
		for (; y_ > yEnd_; y_--) {

			//Round the Right Boundary
			cXR_ = Floor(xR_);	

			//Calculate the Red Gradient Step
			Step_[R] = (ColR_.r - ColL_.r) / (Xdiff_ = (xR_ - xL_));
			
			//Calculate the Green Gradient Step
			Step_[G] = (ColR_.g - ColL_.g) / (Xdiff_);
			
			//Calculate the Blue Gradient Step
			Step_[B] = (ColR_.b - ColL_.b) / (Xdiff_);
			
			//Calculate the Alpha Gradient Step
			Step_[A] = (ColR_.a - ColL_.a) / (Xdiff_);

			//Set the gradient initial value to the left color
			memcpy(&Gradient_, &ColL_, sizeof(Color));

			//Draw a line
			for (unsigned x = Floor(xL_); x < cXR_; x++) {

				//Set the pixel
				FrameBuffer::SetPixel(x, y_, Gradient_);
				
				//Update the gradient (red)
				Gradient_.r += Step_[R];
				
				//Update the gradient (green)
				Gradient_.g += Step_[G];

				//Update the gradient (blue)
				Gradient_.b += Step_[B];

				//Update the gradient (alpha)
				Gradient_.a += Step_[A];

			}

			//Extend the Boundaries (Left)
			xL_ -= SlopeLeft_;

			//Extend the Boundaries (Right)
			xR_ -= SlopeRight_;

			//If the middle is on the left
			if (vinf_.MiddleIsLeft) {
				
				//Update the Left color(Red)
				ColL_.r -= StepTM_[R];

				//Update the Left color(Green)
				ColL_.g -= StepTM_[G];

				//Update the Left color(Blue)
				ColL_.b -= StepTM_[B];

				//Update the Left color(Alpha)
				ColL_.a -= StepTM_[A];

				//Update the Right color(Red)
				ColR_.r -= StepTB_[R];

				//Update the Right color(Green)
				ColR_.g -= StepTB_[G];

				//Update the Right color(Blue)
				ColR_.b -= StepTB_[B];

				//Update the Right color(Alpha)
				ColR_.a -= StepTB_[A];

			//else
			} else {
				
				//Update the Left color(Red)
				ColL_.r -= StepTB_[R];

				//Update the Left color(Green)
				ColL_.g -= StepTB_[G];

				//Update the Left color(Blue)
				ColL_.b -= StepTB_[B];

				//Update the Left color(Alpha)
				ColL_.a -= StepTB_[A];

				//Update the Right color(Red)
				ColR_.r -= StepTM_[R];

				//Update the Right color(Green)
				ColR_.g -= StepTM_[G];

				//Update the Right color(Blue)
				ColR_.b -= StepTM_[B];

				//Update the Right color(Alpha)
				ColR_.a -= StepTM_[A];

			}
		}

		//If the middle is on the left
		if (vinf_.MiddleIsLeft) {

			//Update the Left Boundaries
			xL_ = vinf_.Middle->mPosition.x;

			//Adjust the new slope
			SlopeLeft_ = (vinf_.Bottom->mPosition.x - vinf_.Middle->mPosition.x)
				/ (vinf_.Bottom->mPosition.y - vinf_.Middle->mPosition.y);

			//Update the Color
			memcpy(&ColL_, &vinf_.Middle->mColor, sizeof(Color));

		//else
		} else {

			//Update the Right Boundaries
			xR_ = vinf_.Middle->mPosition.x;

			//Adjust the new slope
			SlopeRight_ = (vinf_.Bottom->mPosition.x - vinf_.Middle->mPosition.x)
				/ (vinf_.Bottom->mPosition.y - vinf_.Middle->mPosition.y);

			//Update the Color
			memcpy(&ColR_, &vinf_.Middle->mColor, sizeof(Color));

		}

		//Set the new ending
		yEnd_ = Ceiling(vinf_.Bottom->mPosition.y) + 1;

		//Traverse through the second part of the triangle
		for (; y_ >= yEnd_; y_--) {

			//Round the Right Boundaries
			cXR_ = Floor(xR_);

			//Calculate the pixel's gradient step (Red)
			Step_[R] = (ColR_.r - ColL_.r) / (Xdiff_ = (xR_ - xL_));
			
			//Calculate the pixel's gradient step (Green)
			Step_[G] = (ColR_.g - ColL_.g) / (Xdiff_);

			//Calculate the pixel's gradient step (Blue)
			Step_[B] = (ColR_.b - ColL_.b) / (Xdiff_);

			//Calculate the pixel's gradient step (Alpha)
			Step_[A] = (ColR_.a - ColL_.a) / (Xdiff_);

			//Set the Gradient initial value to the left color
			memcpy(&Gradient_, &ColL_, sizeof(Color));

			//Draw a line
			for (unsigned x = Floor(xL_); x < cXR_; x++) {

				//Set the pixel
				FrameBuffer::SetPixel(x, y_, Gradient_);

				//Update the new gradient Color (Red)
				Gradient_.r += Step_[R];
				
				//Update the new gradient Color (Green)
				Gradient_.g += Step_[G];

				//Update the new gradient Color (Blue)
				Gradient_.b += Step_[B];

				//Update the new gradient Color (Alpha)
				Gradient_.a += Step_[A];

			}

			//Extend the triangle boundaries (Left)
			xL_ -= SlopeLeft_;

			//Extend the triangle Boundaries (Right)
			xR_ -= SlopeRight_;

			//If the middle is on the left
			if (vinf_.MiddleIsLeft) {

				//Update the Left color(Red)
				ColL_.r -= StepMB_[R];

				//Update the Left color(Green)
				ColL_.g -= StepMB_[G];

				//Update the Left color(Blue)
				ColL_.b -= StepMB_[B];

				//Update the Left color(Alpha)
				ColL_.a -= StepMB_[A];

				//Update the Right color(Red)
				ColR_.r -= StepTB_[R];

				//Update the Right color(Green)
				ColR_.g -= StepTB_[G];

				//Update the Right color(Blue)
				ColR_.b -= StepTB_[B];

				//Update the Right color(Alpha)
				ColR_.a -= StepTB_[A];

			//else
			} else {

				//Update the Left color(Red)
				ColL_.r -= StepTB_[R];

				//Update the Left color(Green)
				ColL_.g -= StepTB_[G];

				//Update the Left color(Blue)
				ColL_.b -= StepTB_[B];

				//Update the Left color(Alpha)
				ColL_.a -= StepTB_[A];

				//Update the Right color(Red)
				ColR_.r -= StepMB_[R];

				//Update the Right color(Green)
				ColR_.g -= StepMB_[G];

				//Update the Right color(Blue)
				ColR_.b -= StepMB_[B];

				//Update the Right color(Alpha)
				ColR_.a -= StepMB_[A];

			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Draw Triangle Plane Normal
	*
	*   Draws a Triangle using the Plane Normal method
	*/ // --------------------------------------------------------------------
	void DrawTrianglePlaneNormal(const Vertex& v0, const Vertex& v1, const Vertex& v2) {

		//Get the Vertex information
		const VertexInf<Vertex> vinf_(v0, v1, v2);

		//find the Ending Y and the Starting Y
		unsigned yEnd_ = Ceiling(vinf_.Middle->mPosition.y),
			y_ = Ceiling(vinf_.Top->mPosition.y), cXR_;

		/*Calculate the triangle's borders, as well as it's current slopes and
		it's pixel gradient steps and some precomputed data*/
		float xL_ = vinf_.Top->mPosition.x, xR_ = xL_,
			SlopeLeft_ = vinf_.MiddleIsLeft ?
			LERP(vinf_.Middle->mPosition.x, vinf_.Top->mPosition.x,
			vinf_.Middle->mPosition.y, vinf_.Top->mPosition.y)
			: LERP(vinf_.Bottom->mPosition.x, vinf_.Top->mPosition.x,
			vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y),
			SlopeRight_ = vinf_.MiddleIsLeft ?
			LERP(vinf_.Bottom->mPosition.x, vinf_.Top->mPosition.x,
			vinf_.Bottom->mPosition.y, vinf_.Top->mPosition.y)
			: LERP(vinf_.Middle->mPosition.x, vinf_.Top->mPosition.x,
			vinf_.Middle->mPosition.y, vinf_.Top->mPosition.y);

		//Calculate Plane Vertices (Z axis correspond to each color)
		const AEVec3 VertexT_[RGBA] = { AEVec3(v0.mPosition.x - v1.mPosition.x,
			v0.mPosition.y - v1.mPosition.y, v0.mColor.r - v1.mColor.r).
			Cross(AEVec3(v0.mPosition.x - v2.mPosition.x, v0.mPosition.y -
			v2.mPosition.y, v0.mColor.r - v2.mColor.r)), AEVec3(v0.mPosition.x - v1.mPosition.x,
			v0.mPosition.y - v1.mPosition.y, v0.mColor.g - v1.mColor.g).
			Cross(AEVec3(v0.mPosition.x - v2.mPosition.x,
			v0.mPosition.y - v2.mPosition.y, v0.mColor.g - v2.mColor.g)), 
			AEVec3(v0.mPosition.x - v1.mPosition.x,
			v0.mPosition.y - v1.mPosition.y, v0.mColor.b - v1.mColor.b).
			Cross(AEVec3(v0.mPosition.x - v2.mPosition.x,
			v0.mPosition.y - v2.mPosition.y, v0.mColor.b - v2.mColor.b)), 
			AEVec3(v0.mPosition.x - v1.mPosition.x,
			v0.mPosition.y - v1.mPosition.y, v0.mColor.a - v1.mColor.a).
			Cross(AEVec3(v0.mPosition.x - v2.mPosition.x,
			v0.mPosition.y - v2.mPosition.y, v0.mColor.a - v2.mColor.a)) };
		
		//Calculate Increments
		const float dx_[RGBA] = { (-VertexT_[R].x) / VertexT_[R].z,
			(-VertexT_[G].x) / VertexT_[G].z , (-VertexT_[B].x) / VertexT_[B].z ,
			(-VertexT_[A].x) / VertexT_[A].z },
			dy_[RGBA] = { (-VertexT_[R].y) / VertexT_[R].z,
			(-VertexT_[G].y) / VertexT_[G].z , (-VertexT_[B].y) / VertexT_[B].z ,
			(-VertexT_[A].y) / VertexT_[A].z };

		//Declare the Left and Right interpolant colors as well as it's gradient
		Color ColL_ = vinf_.Top->mColor, Gradient_ = vinf_.Top->mColor;
		
		//Calcualte steps
		float Step[RGBA] = { dy_[R] + SlopeLeft_ * dx_[R] ,
			dy_[G] + SlopeLeft_ * dx_[G] , dy_[B] + SlopeLeft_ * dx_[B] ,
			dy_[A] + SlopeLeft_ * dx_[A] };

		//traverse through the first part of the triangle
		for (; y_ > yEnd_; y_--) {

			//Round the Right Boundary
			cXR_ = Floor(xR_);

			//Set the gradient initial value to the left color
			memcpy(&Gradient_, &ColL_, sizeof(Color));

			//Draw a line
			for (unsigned x = Floor(xL_); x < cXR_; x++) {

				//Set the pixel
				FrameBuffer::SetPixel(x, y_, Gradient_);

				//Increment the gradient (Red)
				Gradient_.r += dx_[R];
				
				//Increment the gradient (Green)
				Gradient_.g += dx_[G];

				//Increment the gradient (Blue)
				Gradient_.b += dx_[B];

				//Increment the gradient (Alpha)
				Gradient_.a += dx_[A];

			}

			//Update the Red Channel
			ColL_.r -= Step[R];
			
			//Update the Green Channel
			ColL_.g -= Step[G];
			
			//Update the Blue Channel
			ColL_.b -= Step[B];
			
			//Update the Alpha Channel
			ColL_.a -= Step[A];

			//Extend the Boundaries (Left)
			xL_ -= SlopeLeft_;

			//Extend the Boundaries (Right)
			xR_ -= SlopeRight_;

		}

		//If the middle is on the left
		if (vinf_.MiddleIsLeft) {

			//Update the Left Boundaries
			xL_ = vinf_.Middle->mPosition.x;

			//Adjust the new slope
			SlopeLeft_ = (vinf_.Bottom->mPosition.x - vinf_.Middle->mPosition.x)
				/ (vinf_.Bottom->mPosition.y - vinf_.Middle->mPosition.y);

			//Update the Color
			memcpy(&ColL_, &vinf_.Middle->mColor, sizeof(Color));

		//else
		} else {

			//Update the Right Boundaries
			xR_ = vinf_.Middle->mPosition.x;

			//Adjust the new slope
			SlopeRight_ = (vinf_.Bottom->mPosition.x - vinf_.Middle->mPosition.x)
				/ (vinf_.Bottom->mPosition.y - vinf_.Middle->mPosition.y);

		}

		//Set the new color increment(Red)
		Step[R] = dy_[R] + SlopeLeft_ * dx_[R];

		//Set the new color increment(Green)
		Step[G] = dy_[G] + SlopeLeft_ * dx_[G];

		//Set the new color increment(Blue)
		Step[B] = dy_[B] + SlopeLeft_ * dx_[B];

		//Set the new color increment(Alpha)
		Step[A] = dy_[A] + SlopeLeft_ * dx_[A];

		//Set the new ending
		yEnd_ = Ceiling(vinf_.Bottom->mPosition.y) + 1;

		//Traverse through the second part of the triangle
		for (; y_ >= yEnd_; y_--) {

			//Round the Right Boundaries
			cXR_ = Floor(xR_);

			//Copy the Left color to the gradient
			memcpy(&Gradient_, &ColL_, sizeof(Color));

			//Draw a line
			for (unsigned x = Floor(xL_); x < cXR_; x++) {

				//Set the pixel
				FrameBuffer::SetPixel(x, y_, Gradient_);

				//Increment the gradient (Red)
				Gradient_.r += dx_[R];

				//Increment the gradient (Green)
				Gradient_.g += dx_[G];

				//Increment the gradient (Blue)
				Gradient_.b += dx_[B];

				//Increment the gradient (Alpha)
				Gradient_.a += dx_[A];

			}

			//Update the Red Channel
			ColL_.r -= Step[R];

			//Update the Green Channel
			ColL_.g -= Step[G];

			//Update the Blue Channel
			ColL_.b -= Step[B];

			//Update the Alpha Channel
			ColL_.a -= Step[A];

			//Extend the Boundaries (Left)
			xL_ -= SlopeLeft_;

			//Extend the Boundaries (Right)
			xR_ -= SlopeRight_;

		}
	}
	
	// ------------------------------------------------------------------------
	/*! Smallest Three Way
	*
	*   Gets the smallest of three floats
	*/ // --------------------------------------------------------------------
	constexpr float inline Smallestofthree(const float x, const float y, const float z) {

		//Create a temporary smallest float
		float Temp_ = x;

		//If the second float is smaller
		if (y < Temp_)
			
			//Set the temporary value to the second float
			Temp_ = y;

		//If the third float is smaller
		if (z < Temp_)

			//Set the temporary value to the third float
			Temp_ = z;

		//return the temporary value
		return Temp_;

	}

	// ------------------------------------------------------------------------
	/*! Highest Three Way
	*
	*   Gets the highest of three floats
	*/ // --------------------------------------------------------------------
	constexpr float inline Highestofthree(const float x, const float y, const float z) {

		//Create a temporal highest vector
		float Temp_ = x;

		//if the second float is higher than the temporal value
		if (y > Temp_)
			
			//Assign the temporal value to the second float
			Temp_ = y;

		//If the third float is higher than the temporal values
		if (z > Temp_)

			//Assign the temporal value to the third float
			Temp_ = z;

		//Return the temporal value
		return Temp_;

	}

	// ------------------------------------------------------------------------
	/*! Draw Triangle Barycentric
	*
	*   Draws a Triangle using Barycentric Coordinates
	*/ // --------------------------------------------------------------------
	void DrawTriangleBarycentric(const Vertex& v0, const Vertex& v1, const Vertex& v2) {

		//Get the Vertex information
		const VertexInfB vinf_(v0, v1, v2);

		//Store the Maximun Values for the Traversal
		const unsigned Xmax_ = Floor(Highestofthree(v0.mPosition.x, v1.mPosition.x, 
			v2.mPosition.x)), Ymax_ = Ceiling(vinf_.Top->mPosition.y);

		//Compute the Normal, the Barycentric coordinates increments and the minimun X
		const float n = AEVec2(v1.mPosition - v0.mPosition).
			CrossMag(AEVec2(v2.mPosition - v0.mPosition)),
			dL1dx_ = (v2.mPosition.y - v0.mPosition.y) / n,
			dL1dy_ = (v0.mPosition.x - v2.mPosition.x) / n,
			dL2dx_ = (-v1.mPosition.y + v0.mPosition.y) / n,
			dL2dy_ = (-v0.mPosition.x + v1.mPosition.x) / n,
			Xmin_ = Smallestofthree(v0.mPosition.x, v1.mPosition.x, v2.mPosition.x);

		//Calculate the Starting Point and the vector from it to the first vertex
		AEVec2 StartPoint_(Xmin_, vinf_.Bottom->mPosition.y), PV0_ = StartPoint_ - v0.mPosition;

		//Calculate the initial lambdas
		float Lambda1start_ = (AEVec2(StartPoint_ - v2.mPosition).CrossMag(PV0_)) / n,
			Lambda2start_ = (PV0_.CrossMag(AEVec2(StartPoint_ - v1.mPosition))) / n;

		//From the Bottom Y to the Top Y
		for (unsigned y = Ceiling(vinf_.Bottom->mPosition.y); y <= Ymax_; y++,
			Lambda1start_ += dL1dy_, Lambda2start_ += dL2dy_) {

			//Reset the Lambdas plus an Y axis increment
			float Lambda1_ = Lambda1start_, 
				Lambda2_ = Lambda2start_;

			//From the Left X to the Right X
			for (unsigned x = Floor(Xmin_); x < Xmax_; x++, Lambda1_ += dL1dx_,
				Lambda2_ += dL2dx_) {

				//Calculate final Lambda (Using L1 + L2 + L0 = 0)
				const float Lambda0_ = 1 - Lambda1_ - Lambda2_;

				//if this point is inside our triangle
				if (Lambda0_ > -EPSILON && Lambda1_ > -EPSILON && Lambda2_ > -EPSILON)

					//Draw the Pixel
					FrameBuffer::SetPixel(x, y,
					(v0.mColor * Lambda0_) + (v1.mColor * Lambda1_) +
						(v2.mColor * Lambda2_));

			}
		}
	}

#define AXIS_SIZE	2
#define VERTEX_NUM	3
#define X			0
#define Y			(X + 1)
#define COL_PTR_IDX	AXIS_SIZE
#define BOOL_SIZE	1
#define FLOAT_SIZE	1
#define CURRV_ID_XL 0
#define CURRV_ID_XR (CURRV_ID_XL + 1)
#define CURRV_ID_SL	(CURRV_ID_XR + 1)
#define CURRV_ID_SR	(CURRV_ID_SL + 1)
#define STAV_ID_MDB	0	
#define STAV_ID_TOP	FLOAT_SIZE - 1
#define STAV_ID_MID	(STAV_ID_TOP + AXIS_SIZE)
#define STAV_ID_BOT	(STAV_ID_MID + AXIS_SIZE)
#define STAV_ID_CLL (STAV_ID_BOT + AXIS_SIZE)
#define CUR_OFFSET	(CURRV_ID_SR + 1)
#define CUR_STP_MTL	0
#define CUR_GDR_MTL	(CUR_STP_MTL + 1)

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs the Attribute Interpolator Class
	*/ // --------------------------------------------------------------------
	AttributeInterpolator::AttributeInterpolator() :
		mvCurrentValues(nullptr), mvStartValues(nullptr), mvDiffX(nullptr), mvDiffY(nullptr),
		mAttributeCount(0) {

	}
	
	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys the Attribute Interpolator
	*/ // --------------------------------------------------------------------
	AttributeInterpolator::~AttributeInterpolator() {

		//Delete the Current Values
		if (mvCurrentValues) 
			delete [] mvCurrentValues;

		//Delete the Start Values
		if(mvStartValues)
			delete [] mvStartValues;

		//Delete the X Differentials
		if(mvDiffX)
			delete [] mvDiffX;

		//Delete the Y Differentials
		if(mvDiffY)
			delete [] mvDiffY;

	}

	/*************************************************************************
	
		HOW I STRUCTURATE MEMORY

		-> Current Values:
			4 floats -> XR, XL, Slope Right, Slope Left (In Order)
			n * 2 floats -> Gradient Attributes and the Attribute Slopes 
				(in order)
			 _   _    _
			|4f||GA||AS|
			 

		-> Start Values:
			1 float -> Is the middle vertex on the left?
			5 floats -> To store the vertex positions in order except for the Top X
			n floats -> To store the base increments of each attribute
			 _   _    _
			|1f||5f||BA|

		-> Increments (both X and Y):
			n floats -> One float for each attribute
	
	**************************************************************************/

	// ------------------------------------------------------------------------
	/*! Init
	*
	*   Initializes the Attribute Interpolator
	*/ // --------------------------------------------------------------------
	void inline AttributeInterpolator::Init(const float* topVtx, const float* midVtx, 
		const float* botVtx, const unsigned& vertexSize) {

		//Gather the Vertex information
		const VertexInf<AEVec2> vinf_(*reinterpret_cast<const AEVec2*>(topVtx), 
			*reinterpret_cast<const AEVec2*>(midVtx), 
			*reinterpret_cast<const AEVec2*>(botVtx));

		//Set the Attribute Count
		mAttributeCount = vertexSize - AXIS_SIZE;

		//Allocate the array of Current Values (One attributes copy for each vertex)
		mvCurrentValues = new float[(2 * mAttributeCount) + 4 + 2];

		//Allocate the X axis Differentials
		mvDiffX = new float[mAttributeCount];

		//Allocate the Y axis Differentials
		mvDiffY = new float[mAttributeCount];

		/*
			-Allocate the Scan-line Starting Values:
				Is middle?
				Vertex Positions Ordered
				Attributes

				MICRO-OPTIMIZATION:
				We actually never use Top.X, so we can substract 1 to the variable count

		*/
		mvStartValues = new float[(mAttributeCount) + BOOL_SIZE + (VERTEX_NUM * AXIS_SIZE) - 1];

		//Set the XR and XL line constraints
		mvCurrentValues[CURRV_ID_XL] = mvCurrentValues[CURRV_ID_XR] = vinf_.Top->x;
		
		//Store Middle Is Left
		mvStartValues[STAV_ID_MDB] = vinf_.MiddleIsLeft;

		//Store the Top Y Value
		mvStartValues[STAV_ID_TOP + Y] = vinf_.Top->y;
		
		//Store the Mid Position
		memcpy(mvStartValues + STAV_ID_MID, vinf_.Middle, sizeof(float) * AXIS_SIZE);
		
		//Store the Bot Position
		memcpy(mvStartValues + STAV_ID_BOT, vinf_.Bottom, sizeof(float) * AXIS_SIZE);
		
		//Calculate the Left Slope
		mvCurrentValues[CURRV_ID_SL] = mvStartValues[STAV_ID_MDB] ?
			LERP(vinf_.Middle->x, vinf_.Top->x, vinf_.Middle->y, vinf_.Top->y) : 
			LERP(vinf_.Bottom->x, vinf_.Top->x, vinf_.Bottom->y, vinf_.Top->y);
		
		//Calculate the Right Slope
		mvCurrentValues[CURRV_ID_SR] = mvStartValues[STAV_ID_MDB] ?
			LERP(vinf_.Bottom->x, vinf_.Top->x, vinf_.Bottom->y, vinf_.Top->y) : 
			LERP(vinf_.Middle->x, vinf_.Top->x, vinf_.Middle->y, vinf_.Top->y);

		//Calculate the Initial Base Color
		memcpy(mvStartValues + STAV_ID_CLL, topVtx + COL_PTR_IDX, sizeof(float) * mAttributeCount);

		//For each Attribute
		for (size_t i = 0; i < mAttributeCount; i++) {

			const AEVec3 VertexT_ = AEVec3(topVtx[X] - midVtx[X],
				topVtx[Y] - midVtx[Y], topVtx[COL_PTR_IDX + i] - midVtx[COL_PTR_IDX + i]).
				Cross(AEVec3(topVtx[X] - botVtx[X],
					topVtx[Y] - botVtx[Y], topVtx[COL_PTR_IDX + i] - botVtx[COL_PTR_IDX + i]));

			//Calculate the X Differential
			mvDiffX[i] = -VertexT_.x / VertexT_.z;

			//Calculate the Y Differential
			mvDiffY[i] = -VertexT_.y / VertexT_.z;

			//Calculate the Step
			mvCurrentValues[CUR_OFFSET + i] =
			mvDiffY[i] + mvCurrentValues[CURRV_ID_SL] * mvDiffX[i];

		}
	}
	
	// ------------------------------------------------------------------------
	/*! Start Scan Line
	*
	*   Function which is called whenever a Scan Line has started
	*/ // --------------------------------------------------------------------
	void inline AttributeInterpolator::StartScanLine() {

		//Copy the Base Color to the Gradient Color
		memcpy(mvCurrentValues + CUR_OFFSET + (CUR_GDR_MTL * mAttributeCount) + AXIS_SIZE, mvStartValues + STAV_ID_CLL,
			sizeof(float) * mAttributeCount);

	}
	
	// ------------------------------------------------------------------------
	/*! Step X
	*
	*   Function which is called everytime we Step on X
	*/ // --------------------------------------------------------------------
	void inline AttributeInterpolator::StepX() {

		//For each attribute
		for (size_t i = 0; i < mAttributeCount; i++)

			//Increment said attribute
			mvCurrentValues[AXIS_SIZE + CUR_OFFSET + (CUR_GDR_MTL *  mAttributeCount) + i] += mvDiffX[i];

	}
	
	// ------------------------------------------------------------------------
	/*! Step Y
	*
	*   Function which is called everytime we Step on X
	*/ // --------------------------------------------------------------------
	void inline AttributeInterpolator::StepY(float invSlope) {

		//For each attribute
		for (size_t i = 0; i < mAttributeCount; i++)

			//Increment said attribute
			mvStartValues[STAV_ID_CLL + i] -= 
				mvCurrentValues[CUR_OFFSET + (CUR_STP_MTL * mAttributeCount) + i];


		//Extend the Boundaries (Left)
		mvCurrentValues[CURRV_ID_XL] -= invSlope;

		//Extend the Boundaries (Right)
		mvCurrentValues[CURRV_ID_XR] -= mvCurrentValues[CURRV_ID_SR];

	}
	
	// ------------------------------------------------------------------------
	/*! Get Current Values
	*
	*   Returns the values of the Attribute Interpolator's current values
	*/ // --------------------------------------------------------------------
	float* AttributeInterpolator::GetCurrentValues() {

		//return the Current Values
		return mvCurrentValues;
	
	}

	// ------------------------------------------------------------------------
	/*! Draw Triangle Shaded
	*
	*   Draws a Shaded Triangle
	*/ // --------------------------------------------------------------------
	void DrawTriangleShaded(const float* vertexData, const unsigned& vertexSize, 
		ShaderFunc shader, void* shaderData) {

		//Create an Attribute Interpolator
		AttributeInterpolator Ai;

		//Init the Attribute Interpolator
		Ai.Init(vertexData, vertexData + vertexSize, vertexData + 2 * vertexSize, vertexSize);

		int* const Pos[AXIS_SIZE] = { reinterpret_cast<int*>(Ai.mvCurrentValues + 
			CUR_OFFSET + (CUR_GDR_MTL * Ai.mAttributeCount) + X),  Pos[0] + 1};

		float* const ShaderData_idx = Ai.mvCurrentValues + CUR_OFFSET +
					(CUR_GDR_MTL * Ai.mAttributeCount);

		//find the Ending Y and the Starting Y
		int yEnd_ = Ceiling(Ai.mvStartValues[STAV_ID_MID + Y]), cXR_;

		//traverse through the first part of the triangle
		for (*Pos[Y] = Ceiling(Ai.mvStartValues[STAV_ID_TOP + Y]); *Pos[Y] > yEnd_;
			(*Pos[Y])--, Ai.StepY(Ai.mvCurrentValues[CURRV_ID_SL])) {

			//Round the Right Boundary
			cXR_ = Floor(Ai.mvCurrentValues[CURRV_ID_XR]);

			//Start the Atribute Line
			Ai.StartScanLine();

			//Draw a line
			for (*Pos[X] = Floor(Ai.mvCurrentValues[CURRV_ID_XL]);
				*Pos[X] < cXR_; Ai.StepX(), (*Pos[X])++)
			
				//Calculate the Pixel Color with the shader
				FrameBuffer::SetPixel(*Pos[X], *Pos[Y], BlendColor(shader(ShaderData_idx, shaderData), FrameBuffer::GetPixel(*Pos[X], *Pos[Y])));

		}

		//If the middle is on the left
		if (Ai.mvStartValues[STAV_ID_MDB]) {

			//Update the Left Boundaries
			Ai.mvCurrentValues[CURRV_ID_XL] = Ai.mvStartValues[STAV_ID_MID + X];

			//Adjust the new slope
			Ai.mvCurrentValues[CURRV_ID_SL] =  LERP(Ai.mvStartValues[STAV_ID_BOT + X], 
				Ai.mvStartValues[STAV_ID_MID + X], Ai.mvStartValues[STAV_ID_BOT + Y], 
				Ai.mvStartValues[STAV_ID_MID + Y]);

			//Update the Color
			memcpy(Ai.mvStartValues + STAV_ID_CLL, vertexData + COL_PTR_IDX, sizeof(float) * 
				Ai.mAttributeCount);

		//else
		} else {

			//Update the Right Boundaries
			Ai.mvCurrentValues[CURRV_ID_XR] = Ai.mvStartValues[STAV_ID_MID + X];

			//Adjust the new slope
			Ai.mvCurrentValues[CURRV_ID_SR] = LERP(Ai.mvStartValues[STAV_ID_BOT + X],
				Ai.mvStartValues[STAV_ID_MID + X], Ai.mvStartValues[STAV_ID_BOT + Y],
				Ai.mvStartValues[STAV_ID_MID + Y]);

		}

		//For every single attribute
		for (size_t i = 0; i < Ai.mAttributeCount; i++)

			//Recalculate it's step for the new half of the triangle
			Ai.mvCurrentValues[CUR_STP_MTL * Ai.mAttributeCount + CUR_OFFSET + i] =
				Ai.mvDiffY[i] + Ai.mvCurrentValues[CURRV_ID_SL] * Ai.mvDiffX[i];

		//Set the new ending
		yEnd_ = Ceiling(Ai.mvStartValues[STAV_ID_BOT + Y]);

		//Traverse through the second part of the triangle
		for (; *Pos[Y] >= yEnd_; (*Pos[Y])--, Ai.StepY(Ai.mvCurrentValues[CURRV_ID_SL])) {

			//cXR_ = 0;
			//Round the Right Boundaries
			cXR_ = Floor(Ai.mvCurrentValues[CURRV_ID_XR]);

			//Set the gradient initial value to the left color
			Ai.StartScanLine();

			//Draw a line
			for (*Pos[X] = Floor(Ai.mvCurrentValues[CURRV_ID_XL]);
				*Pos[X] < cXR_; Ai.StepX(), (*Pos[X])++)
				
				//Calculate the Pixel Color with the shader
				FrameBuffer::SetPixel(*Pos[X], *Pos[Y], BlendColor(shader(ShaderData_idx, shaderData), FrameBuffer::GetPixel(*Pos[X], *Pos[Y])));

		}
	}
}