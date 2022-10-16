//
//  DrawCircle.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 30/05/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include "Rasterizer.h"

namespace Rasterizer {
#pragma region CIRCLE

	// ---------------------------------------------------------------------------

	static EDrawCircleMethod sDrawCircleMethod = eDC_ALGEBRAIC;
	EDrawCircleMethod GetDrawCircleMethod() { return sDrawCircleMethod; }
	void SetDrawCircleMethod(EDrawCircleMethod dcm) { sDrawCircleMethod = dcm; }

	// ---------------------------------------------------------------------------

	static int sCircleParametricPrecision = 50;
	int GetCircleParametricPrecision() { return sCircleParametricPrecision; }
	void SetCircleParametricPrecision(int prec) { sCircleParametricPrecision = prec; }

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Draw Circle
	*
	*   Draws a Circle
	*/ // --------------------------------------------------------------------
	void DrawCircle(const AEVec2& center, float radius, const Color& c) {

		//If we have a radius bigger than 0
		if (Round(radius))

			//switch by drawing method
			switch (sDrawCircleMethod) {

				//in case it's algebraic
			case eDC_ALGEBRAIC:

				//draw a circle in algebraic form
				DrawCircleAlgebraic(center, radius, c);

				//break
				break;

				//in case we want the parametric form
			case eDC_PARAMETRIC:

				//draw a circle in parametric form
				DrawCircleParametric(center, radius, c);

				//break
				break;

				//in case we want the incremental form
			case eDC_PARAMETRIC_INCREMENTAL:

				//draw the parametric incremental form
				DrawCircleParametricInc(center, radius, c);

				//break
				break;

				//default 
			default:

				//Draw using the midpoint form
				DrawCircleMidpoint(center, radius, c);

			}

		//else
		else

			//Draw using the Set Pixel
			FrameBuffer::SetPixel(Round(center.x), Round(center.y), c);

	}

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Set Pixel Eight Way
	*
	*   Sets eight pixel given an ammount of coordinates
	*/ // --------------------------------------------------------------------
	void SetPixelEightWay(u32 cX, u32 cY, u32 x, u32 y, const Color& c) {

		//Calculate the colors
		const u8 r = static_cast<u8>(c.r * 255),
			g = static_cast<u8>(c.g * 255),
			b = static_cast<u8>(c.b * 255),
			a = static_cast<u8>(c.a * 255);

		//Set Pixel Way 1
		FrameBuffer::SetPixel(cX + x, cY + y, r, g, b, a);

		//Set Pixel Way 2
		FrameBuffer::SetPixel(cX - x, cY + y, r, g, b, a);

		//Set Pixel Way 3
		FrameBuffer::SetPixel(cX + x, cY - y, r, g, b, a);

		//Set Pixel Way 4
		FrameBuffer::SetPixel(cX - x, cY - y, r, g, b, a);

		//Set Pixel Way 5
		FrameBuffer::SetPixel(cX + y, cY + x, r, g, b, a);

		//Set Pixel Way 6
		FrameBuffer::SetPixel(cX - y, cY + x, r, g, b, a);

		//Set Pixel Way 7
		FrameBuffer::SetPixel(cX + y, cY - x, r, g, b, a);

		//Set Pixel Way 8
		FrameBuffer::SetPixel(cX - y, cY - x, r, g, b, a);

	}

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Draw Circle Algebraic
	*
	*   Draws a circle using the Algebraic method
	*/ // --------------------------------------------------------------------
	void DrawCircleAlgebraic(const AEVec2& center, float radius, const Color& c) {

		//Calculate The Radius Squared
		const float R2 = radius * radius;

		//Get standard varivles
		const int Xc = Round(center.x), Yc = Round(center.y),
			q = Round(sqrtf(2.f) / 2 * radius);

		//Loop over x from negative q to q
		for (int x = -q; x != q; x++)

			//Set right pixels
			SetPixelEightWay(Xc, Yc, x, Round(sqrtf(R2 - x * x)), c);

	}

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Draw Circle Parametric
	*
	*   Draws a Circle using parametric equation
	*/ // --------------------------------------------------------------------
	void DrawCircleParametric(const AEVec2& center, float radius, const Color& c) {

		//Set the Step
		const float StepAngle_ = TWO_PI / sCircleParametricPrecision;

		//Set initial points
		AEVec2 p1 = { radius, 0.f }, p2;

		//Loop for every single step
		for (float alpha = StepAngle_; alpha <= TWO_PI + StepAngle_;
			alpha += StepAngle_, p1 = p2)

			//Draw the line
			DrawLine(p1 + center, (p2 = { cosf(alpha) * radius, sinf(alpha) * radius })
				+ center, c);

	}

	// ---------------------------------------------------------------------------
	// Challenge 1

	// ------------------------------------------------------------------------
	/*! Draw Circle Parametric Inc
	*
	*   Draws a Circle using parametric equation (incremential)
	*/ // --------------------------------------------------------------------
	void DrawCircleParametricInc(const AEVec2& center, float radius, const Color& c) {

		//set the step
		const float StepAngle_ = TWO_PI / sCircleParametricPrecision,
			Cos_ = cosf(StepAngle_), Sin_ = sinf(StepAngle_);

		//set the initial poinst
		AEVec2 p1{ 0, radius }, p2{ 0, radius };

		//Loop for every single step
		for (float alpha = StepAngle_; alpha <= TWO_PI + StepAngle_; alpha += StepAngle_,
			p1 = p2)

			//Draw the line
			DrawLine(p1 + center, (p2 = { p2.x * Cos_ - p2.y * Sin_ ,
				p2.x * Sin_ + p2.y * Cos_ }) + center, c);

	}

	// ---------------------------------------------------------------------------
	// Challenge 2

	// ------------------------------------------------------------------------
	/*! Fill Circle
	*
	*   Draws a filled Circle
	*/ // --------------------------------------------------------------------
	void FillCircle(const AEVec2& center, float radius, const Color& c) {

		//Precompute recursive variables
		const float R2 = radius * radius;
		const int Xc = Round(center.x), Yc = Round(center.y), RC = Round(radius);

		//for every single line that could fit in a circle
		for (int x = Round(-radius); x <= RC; x++) {

			//find the Rounded Y
			const int y = Round(sqrtf(R2 - x * x));

			//Draw a line
			DrawLine({ static_cast<float>(Xc + x), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc + x), static_cast<float>(Yc - y) }, c);

		}
	}

	// ------------------------------------------------------------------------
/*! Fill Ring
*
*   Draws Ring
*/ // --------------------------------------------------------------------
	void FillRing(const AEVec2& center, float outerRadius, float innerRadius, const Color& c) {

		//Precalculate the double radiouses
		const float R2 = outerRadius * outerRadius,
			RI2 = innerRadius * innerRadius;

		//Precalculate the Roundinh
		const int Xc = Round(center.x), Yc = Round(center.y);

		//Draw right border
		for (int x = Round(-outerRadius); x <= -innerRadius; x++) {

			//find the Rounded Y
			const int y = Round(sqrtf(R2 - x * x));

			//Draw a line
			DrawLine({ static_cast<float>(Xc + x), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc + x), static_cast<float>(Yc - y) }, c);

		}

		//Draw left border
		for (int x = Round(innerRadius); x <= outerRadius; x++) {

			//find the Rounded Y
			const int y = Round(sqrtf(R2 - x * x));

			//Draw a line
			DrawLine({ static_cast<float>(Xc + x), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc + x), static_cast<float>(Yc - y) }, c);

		}

		//for every single line that could fit in a circle
		for (int xi = Round(-innerRadius); xi <= 0; xi++) {

			//find the Rounded Y
			const int y = Round(sqrtf(R2 - xi * xi));

			//find the Rounded Y
			const int yi = Round(sqrtf(RI2 - xi * xi));

			//Draw a line
			DrawLine({ static_cast<float>(Xc + xi), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc + xi), static_cast<float>(Yc + yi) }, c);

			//Draw a line
			DrawLine({ static_cast<float>(Xc + xi), static_cast<float>(Yc - y) },
				{ static_cast<float>(Xc + xi), static_cast<float>(Yc - yi) }, c);

			//Draw a line
			DrawLine({ static_cast<float>(Xc - xi), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc - xi), static_cast<float>(Yc + yi) }, c);

			//Draw a line
			DrawLine({ static_cast<float>(Xc - xi), static_cast<float>(Yc - y) },
				{ static_cast<float>(Xc - xi), static_cast<float>(Yc - yi) }, c);

		}
	}

#define TWEAKED_VALUE 0.0004f

	// ------------------------------------------------------------------------
	/*! Fill Pie
	*
	*   Draws a Pie Ring
	*/ // --------------------------------------------------------------------
	void FillPie(const AEVec2& center, float radius, float minAngle, float maxAngle, const Color& c) {

		//Get the method used beafore
		ERoundMethod preferredmethod = GetRoundMethod();

		//This only does only work with Trucate Shift
		SetRoundMethod(ERoundMethod::eRM_TRUNCATE_SHIFT);

		//calculate the angles in radians
		const float mAngle = 360 / maxAngle, miAngle = 360 / minAngle;

		//Set the stepangle
		const float StepAngle_ = TWO_PI / (360);

		//Set initial points
		AEVec2 p1 = { cosf(TWO_PI / miAngle) * radius, sinf(TWO_PI / miAngle) * radius }, p2 = p1;

		//Loop for every single step
		for (float alpha = TWO_PI / miAngle; alpha <= (TWO_PI / mAngle) + 2 * StepAngle_;
			alpha += TWEAKED_VALUE, p1 = p2) {

			//Draw the line
			DrawLine(p1 + center, (p2 = { cosf(alpha) * radius, sinf(alpha) * radius })
				+ center, c);
			//Draw the line
			DrawLine(center, p2 + center, c);

		}

		//go back to the preferred method
		SetRoundMethod(preferredmethod);

	}

	// ---------------------------------------------------------------------------
	// Extra Credit

	// ------------------------------------------------------------------------
	/*! Draw Circle Midpoint
	*
	*   Draws a Circle using the Midpoint Algorithm
	*/ // --------------------------------------------------------------------
	void DrawCircleMidpoint(const AEVec2& center, float radius, const Color& c) {

		/*Set the initial Y to the total Radius,
		we will start at the top*/
		int y = static_cast<int>(radius);

		//Precompute the Rounded Center
		const int Rcx_ = Round(center.x), Rcy_ = Round(center.y);

		//Compute the initial Error Magnitude/Decision parameter
		float ErrMg_ = 5/4 - radius;
		
		/*Lets loop and calculate positions just for a quarter of a
		semicircle. Using Set Pixel Eight way, we will be able to 
		replicate the positions adjusted for the rest of the circle
		with ease*/
		for (int x = 0; y >= x; x++) {
		
			/*If the Error Magnitude still allows us to 
				draw within the circle perimeter*/
			if (ErrMg_ < 0)
				
				//Update the Decision Parameter (East)
				ErrMg_ += 2 * x + 1;

			//else, if we are outside the perimeter
			else

				//Update the Decision Paramenter (South East)
				ErrMg_ += 2 * x - 2 * y-- + 1;

			//Draw the pixels (eight way is more optimized)
			SetPixelEightWay(Rcx_, Rcy_, x, y, c);

		}
	}

#pragma endregion

#pragma region ELLIPSE

	// ---------------------------------------------------------------------------

	EDrawEllipseMethod sDrawEllipseMethod = eDE_ALGEBRAIC;
	EDrawEllipseMethod GetDrawEllipseMethod() { return sDrawEllipseMethod; }
	void SetDrawEllipseMethod(EDrawEllipseMethod dem) { sDrawEllipseMethod = dem; }

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Draw Ellipse
	*
	*   Draws a Ellipse
	*/ // --------------------------------------------------------------------
	void DrawEllipse(const AEVec2& center, float A, float B, const Color& c) {

		//if we have proper radious
		if (Round(A) || Round(B))

			//if just the first radius is not valid
			if (!Round(A))

				//Draw a vertical line
				DrawVerticalLine(center + AEVec2(0, B),
					center + AEVec2(0, -B), c);

		//if it's the second radius is not valid
			else if (!Round(B))

				//Draws a horizontal line
				DrawHorizontalLine(center + AEVec2(A, 0),
					center + AEVec2(-A, 0), c);

		//else
			else

				//switch by the choosen elipse method
				switch (sDrawEllipseMethod) {

					//if the Algebraic method is used
				case eDE_ALGEBRAIC:

					//Draw an Algebraic Elpise
					DrawEllipseAlgebraic(center, A, B, c);

					//break
					break;

					//if the parametric method is used
				case eDE_PARAMETRIC:

					//Draw the parametric elipse
					DrawEllipseParametric(center, A, B, c);

					//break
					break;

					//If the incremental method is used
				case eDE_PARAMETRIC_INCREMENTAL:

					//Draw the paramtric increased elipse
					DrawEllipseParametricInc(center, A, B, c);

					//break
					break;

					//Else
				default:

					//Draw the midpoint eplise
					DrawEllipseMidpoint(center, A, B, c);

				}

		//else
		else

			//Set a Pixel
			FrameBuffer::SetPixel(Round(center.x), Round(center.y), c);

	}

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Set Pixel Four Way
	*
	*   Sets a Four Way Pixel
	*/ // --------------------------------------------------------------------
	void SetPixelFourWay(u32 cX, u32 cY, u32 x, u32 y, const Color& c) {

		//Calculate the colors
		const u8 r = static_cast<u8>(c.r * 255),
			g = static_cast<u8>(c.g * 255),
			b = static_cast<u8>(c.b * 255),
			a = static_cast<u8>(c.a * 255);

		//Set Pixel Way 1
		FrameBuffer::SetPixel(cX + x, cY + y, r, g, b, a);

		//Set Pixel Way 2
		FrameBuffer::SetPixel(cX - x, cY + y, r, g, b, a);

		//Set Pixel Way 3
		FrameBuffer::SetPixel(cX + x, cY - y, r, g, b, a);

		//Set Pixel Way 4
		FrameBuffer::SetPixel(cX - x, cY - y, r, g, b, a);

	}

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Draw Ellipse Algebraic
	*
	*   Draws an Ellipse in Algebraic form
	*/ // --------------------------------------------------------------------
	void DrawEllipseAlgebraic(const AEVec2& center, float A, float B, const Color& c) {

		//Precalculate recursive data
		const float aob = A / B, boa = B / A, A2 = A * A, B2 = B * B;

		//Round centers and extremes
		const int Xc = Round(center.x), Yc = Round(center.y),
			x0 = Round(A2 / sqrtf(A2 + B2)),
			y0 = Round(B2 / sqrtf(A2 + B2));

		//loop over x
		for (int x = 0; x <= x0;)

			//Draw the pixels
			SetPixelFourWay(Xc, Yc, x++, Round(boa * sqrtf(A2 - x * x)), c);

		//loop over y
		for (int y = y0; y >= 0;)

			//Draw the pixels
			SetPixelFourWay(Xc, Yc, Round(aob * sqrtf(B2 - y * y)), y--, c);

	}

	// ---------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	/*! Draw Ellipse Parametric
	*
	*   Draws a Elipse using the Parametric method
	*/ // --------------------------------------------------------------------
	void DrawEllipseParametric(const AEVec2& center, float A, float B, const Color& c) {

		//Calculate Staged Angle
		const float StepAngle_ = TWO_PI / sCircleParametricPrecision;

		//Set p1 and p2
		AEVec2 p1 = { A, 0.f }, p2;

		//loop until completed an entire loop
		for (float alpha = StepAngle_; alpha <= TWO_PI + StepAngle_;
			alpha += StepAngle_, p1 = p2)

			//Draw the line
			DrawLine(p1 + center, (p2 = { cosf(alpha) * A , sinf(alpha) * B }) + center, c);

	}

	// ---------------------------------------------------------------------------
	// Challenge 1

	// ------------------------------------------------------------------------
	/*! Draw Ellipse Parametric Incremental
	*
	*   Draws a Elipse using the Parametric Incremental method
	*/ // --------------------------------------------------------------------
	void DrawEllipseParametricInc(const AEVec2& center, float A, float B, const Color& c) {

		const float StepAngle_ = TWO_PI / sCircleParametricPrecision,
			Cos_ = cosf(PI / 180.f), aobSin_ = (A / B) * sinf(PI / 180), boaSin_ =
			(B / A) * sinf(PI / 180);
		const int Xc = Round(center.x), Yc = Round(center.y);
		AEVec2 p1{ A, 0 }, p2{ A, 0 };

		//Loop for every single step
		for (float alpha = StepAngle_; alpha <= 360 + StepAngle_; alpha += StepAngle_,
			p1 = p2)

			//Draws a line
			DrawLine(p1 + center, (p2 = { Cos_ * p2.x - aobSin_ * p2.y, Cos_ * p2.y +
				boaSin_ * p2.x }) + center, c);

	}

	// ---------------------------------------------------------------------------
	// Challenge 2

	// ------------------------------------------------------------------------
	/*! Fill Ellipse
	*
	*   Draws a Filled Ellipse
	*/ // --------------------------------------------------------------------
	void FillEllipse(const AEVec2& center, float A, float B, const Color& c) {

		//Pre calculate recursive variables
		const float aob = A / B, boa = B / A, A2 = A * A, B2 = B * B;

		//Find Extremes and limits
		const int Xc = Round(center.x), Yc = Round(center.y),
			x0 = Round(A2 / sqrtf(A2 + B2)),
			y0 = Round(B2 / sqrtf(A2 + B2));

		//Traverse through x
		for (int x = 0; x <= x0; x++) {

			//Find y
			const int y = Round(boa * sqrtf(A2 - x * x));

			//Draw the line
			DrawLine({ static_cast<float>(Xc + x), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc + x), static_cast<float>(Yc - y) }, c);

			//Draw it's reflected line	
			DrawLine({ static_cast<float>(Xc - x), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc - x), static_cast<float>(Yc - y) }, c);

		}

		//tracerse through y
		for (int y = y0; y >= 0; y--) {

			//Find x
			const int x = Round(aob * sqrtf(B2 - y * y));

			//Draw the line
			DrawLine({ static_cast<float>(Xc + x), static_cast<float>(Yc + y) },
				{ static_cast<float>(Xc + x), static_cast<float>(Yc - y) }, c),

				//Draw it's reflected line	
				DrawLine({ static_cast<float>(Xc - x), static_cast<float>(Yc + y) },
					{ static_cast<float>(Xc - x), static_cast<float>(Yc - y) }, c);

		}
	}

	// ---------------------------------------------------------------------------
	// Extra Credit

	// ------------------------------------------------------------------------
	/*! Draw Ellipse Midpoint
	*
	*   Draws a Ellipse using the Midpoint Agorythm
	*/ // --------------------------------------------------------------------
	void DrawEllipseMidpoint(const AEVec2& center, float A, float B, const Color& c) {

		//Create the initial variables
		int x = 0, y = Round(B);

		//Save the Rounded versions of the center
		const int Rcx_ = Round(center.x), Rcy_ = Round(center.y);
		
		//Precompute common operations
		const float BB_ = B * B, AA_ = A * A, twoBB_ = 2 * BB_,
		twoAA_ = 2 * AA_, threeBB_ = 3 * BB_, threeAA_ = 3 * AA_;
		
		//Declares Dx, Dy and the Error Magnitude
		float Dx_ = twoBB_ * x, Dy_ = twoAA_ * y,
		ErrMg_ = BB_ - AA_ * (B + 0.25f);
 
		//Until the slope is -1
		for (;Dx_ < Dy_; x++) { 

			//If the error magnitude is below 0, draw the East Pixel
			if (ErrMg_ < 0)
				
				//Update the Error Magnitude and Dx
				ErrMg_ += Dx_ = twoBB_ * x + threeBB_;
			
			//Else, draw the South East Pixel
			else
				
				//Update the Error Magnitude, Dx and Dy
				ErrMg_ += (Dx_ = twoBB_ * x + threeBB_) 
				- (Dy_ = twoAA_ * y-- + twoAA_);
			
			//Draw 4 pixels in a row (better that drawing every pixel)
			SetPixelFourWay(Rcx_, Rcy_, x, y, c);

		}

		//Corrects a visual artifact (2 pixels offset) 
		x -= 2;

		//Sets the new error magnitude
		ErrMg_ = BB_ * x - twoAA_ * y + AA_ + BB_ / 4;

		//From slope = -1, to y = 0
		for (; y >= 0; y--) {

			//if the error magnutude is less than 0
			if (ErrMg_ < 0)

				//Update the error magnitude
				ErrMg_ += twoBB_ * x++ + twoBB_ - twoAA_ * y + threeAA_;

			//else
			else

				//Update the Error Magnitude
				ErrMg_ -= twoAA_ * y + threeAA_;

			
			//Draw 4 pixels in a row (better that drawing every pixel)
			SetPixelFourWay(Rcx_, Rcy_, x, y, c);

		}
	}

#pragma endregion

}