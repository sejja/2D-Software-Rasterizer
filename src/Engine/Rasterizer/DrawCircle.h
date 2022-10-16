#pragma once

struct AEVec2;
namespace Rasterizer {
	struct Color;

#pragma region CIRCLE
	
	// config data
	enum EDrawCircleMethod {eDC_ALGEBRAIC, eDC_PARAMETRIC, eDC_PARAMETRIC_INCREMENTAL, eDC_MIDPOINT};
	EDrawCircleMethod GetDrawCircleMethod();
	void SetDrawCircleMethod(EDrawCircleMethod dcm);
	int GetCircleParametricPrecision();
	void SetCircleParametricPrecision(int prec);

	// Lab
	void DrawCircle(const AEVec2& center, float radius, const Color& c);
	void SetPixelEightWay(unsigned long cX, unsigned long cY, unsigned long x, unsigned long y,
		const Color& c);
	void DrawCircleAlgebraic(const AEVec2& center, float radius, const Color& c);
	void DrawCircleParametric(const AEVec2& center, float radius, const Color& c);

	// Challenge 1
	void DrawCircleParametricInc(const AEVec2& center, float radius, const Color& c);

	// Challenge 2
	void FillCircle(const AEVec2& center, float radius, const Color& c);

	// Challenge 3
	void FillRing(const AEVec2 & center, float outerRadius, float innerRadius, const Color & c);
	
	// Challenge 4
	void FillPie(const AEVec2 & center, float radius, float minAngle, float maxAngle, const Color & c);

	// Extra Credit
	void DrawCircleMidpoint(const AEVec2& center, float radius, const Color& c);
#pragma endregion

#pragma region ELLIPSE
	enum EDrawEllipseMethod { eDE_ALGEBRAIC, eDE_PARAMETRIC, eDE_PARAMETRIC_INCREMENTAL, eDE_MIDPOINT };
	EDrawEllipseMethod GetDrawEllipseMethod();
	void SetDrawEllipseMethod(EDrawEllipseMethod dem);

	// Lab
	void DrawEllipse(const AEVec2& center, float A, float B, const Color& c);
	void SetPixelFourWay(unsigned long cX, unsigned long cY, unsigned long x, unsigned long y,
	const Color& c);
	void DrawEllipseAlgebraic(const AEVec2& center, float A, float B, const Color& c);
	void DrawEllipseParametric(const AEVec2& center, float A, float B, const Color& c);

	// Challenge 1
	void DrawEllipseParametricInc(const AEVec2& center, float A, float B, const Color& c);

	// Challenge 2
	void FillEllipse(const AEVec2& center, float A, float B, const Color& c);

	// Extra Credit
	void DrawEllipseMidpoint(const AEVec2& center, float A, float B, const Color& c);
#pragma endregion

}