#ifndef _DRAW_LINE_H_
#define _DRAW_LINE_H_

namespace Rasterizer
{
	/// @TODO
	// ------------------------------------------------------------------------
	/// enum	EDrawLineMethod
	///	\brief	Specifies which method should be used when drawing a line. 
	enum EDrawLineMethod { eDL_NAIVE, eDL_DDA, eDL_BRESENHAM, eDL_Count };

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		GetDrawLineMethod
	/// \brief	Sets the line rendering method.
	EDrawLineMethod GetDrawLineMethod();

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	SetDrawLineMethod	
	/// \brief	Sets the line rendering method.
	void SetDrawLineMethod(EDrawLineMethod dlm);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawLine
	/// \brief	Wrapper function, draws a line using the current method. 
	void DrawLine(const AEVec2& p1, const AEVec2& p2, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineNaive
	/// \brief	Draws a line using the naive algorithm presented in class, based on the explicit line equation
	void DrawLineNaive(const AEVec2& p1, const AEVec2& p2, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineDDA
	/// \brief	Draws a line using the DDA algorithm presented in class.
	void DrawLineDDA(const AEVec2& p1, const AEVec2& p2, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineBresenham
	/// \brief	Draws a line using the Bresenham algorithm presented in class.
	void DrawLineBresenham(const AEVec2& p1, const AEVec2& p2, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawRect
	/// \brief	Wrapper function, draws a rectangle using the above line method. 
	void DrawRect(const AEVec2& r, const AEVec2& size, const Color& c);

	/// -----------------------------------------------------------------------
	///	LINE ALGORITHM IMPLEMENTATIONS

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawHorizontalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawHorizontalLine(const AEVec2& p1, const AEVec2& p2, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawVerticalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawVerticalLine(const AEVec2& p1, const AEVec2& p2, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawDiagonalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawDiagonalLine(const AEVec2& p1, const AEVec2& p2, const Color& c);

	#pragma region COLOR INTERPOLATION

	/// @LAB
	// ------------------------------------------------------------------------
	/// \fn		DrawLine
	/// \brief	Function that draws a line and interpolates the color from its
	///			two vertices
	void DrawLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawHorizontalLine
	/// \brief	Helper function that draws a horizontal line from left to right.
	///			and interpolates the color from its two vertices
	void DrawHorizontalLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawVerticalLine
	/// \brief	Helper function that draws a horizontal line from left to right.
	///			and interpolates the color from its two vertices
	void DrawVerticalLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2);

	#pragma endregion

}

#endif