#pragma once
#include "Vertex.h"

struct AEVec2;

namespace Rasterizer
{

	struct Color;

	// TODO
	/// -----------------------------------------------------------------------
	/// \fn		FillTriangleNaive
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2 using the naive 
	///			algorithm described in class. Each pixel in the triangle share
	///			the same color.
	/// \param	v0	First triangle vertex position.
	///	\param	v1	Second triangle vertex position.
	///	\param	v2	Third triangle vertex position.
	//	\param	c	Color to fill the triangle with.
	void FillTriangleNaive(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c);

	/// -----------------------------------------------------------------------
	/// \fn		FillTriangleTopLeft
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2, using the top
	///			left method described in class. Each pixel in the triangle share
	///			the same color. 
	/// \param	v0	First triangle vertex position.
	///	\param	v1	Second triangle vertex position.
	///	\param	v2	Third triangle vertex position.
	//	\param	c	Color to fill the triangle with.	
	void FillTriangleTopLeft(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c);

	// PROVIDED
	/// -----------------------------------------------------------------------
	/// \enum	EDrawTriangleMethod
	/// \brief	Enumeration of all the internal algorithms used for rasterizing 
	///			with interpolation of color.
	enum EDrawTriangleMethod {eDT_BILINEAR, eDT_PLANE_NORMAL, eDT_BARYCENTRIC };

	// TODO
	/// -----------------------------------------------------------------------
	/// \fn		GetDrawTriangleMethod
	/// \brief	Getter for the current method used when DrawTriangle is called. 
	EDrawTriangleMethod GetDrawTriangleMethod();

	// TODO
	/// -----------------------------------------------------------------------
	/// \fn		SetDrawTriangleMethod
	/// \brief	Setter for the current method used when DrawTriangle is called. 
	void SetDrawTriangleMethod(EDrawTriangleMethod dtm);


	// TODO
	/// -----------------------------------------------------------------------
	/// \fn		DrawTriangle
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2. It delegates
	///			the call to one of the methods implemented internally. See
	///			enum EDrawTriangleMethod above. 
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	/// ------------------------------------------------------------------------
	/// \fn		DrawTriangleBiLinear
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2, using the top
	///			left method and the bi-linear interpolation method for color 
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangleBiLinear(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	/// ------------------------------------------------------------------------
	/// \fn		DrawTrianglePlaneNormal
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2, using the top
	///			left method and the color interpolation method using the normal
	///			plane equation (you cannot use the bi-linear method).
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTrianglePlaneNormal(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	/// ------------------------------------------------------------------------
	/// \fn		DrawTriangleBarycentric
	/// \brief	Same method as the function above but computes the 
	///			This method uses computes the barycentric coordinates INCREMENTALLY.
	///			
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangleBarycentric(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	// ------------------------------------------------------------------------
	// ASSIGNMENT 4 
	// ------------------------------------------------------------------------

	/// -----------------------------------------------------------------------
	// typedef the shader function 
	// Shader functions take as their first argument the fragment
	// data related to the pixel we are writing, that is its attributes. 
	// Additionally the shader func accepts a second argument that 
	// represents user data and that is set on the pipeline. 
	typedef Color(*ShaderFunc)(float*, void*);

	/// TODO
	/// -----------------------------------------------------------------------
	/// \class	AttributeInterpolator
	/// \brief	Interpolates any vertex attributes (including x and y) accross
	///			the surface of a triangle using the plane normal method. 
	///			this class is meant to be used in conjunction with the 
	///			DrawTriangleShaded function below. 
	class AttributeInterpolator
	{
	public:
		// Separate arrays for the attribute interpolator
		float* mvCurrentValues;	// current values for the attributes.
		float* mvStartValues;	// values at the start of the scan line.
		float* mvDiffX;			// differentials on the x axis.
		float* mvDiffY;			// differentials on the y axis.

		// Count of attributes
		unsigned long mAttributeCount;
	public:

		/// TODO
		/// -----------------------------------------------------------------------
		/// \fn		AttributeInterpolator
		///	\brief	Constructor. Sets all data to zero (NULL for pointers). This function
		///			does NOT allocate any memory.
		AttributeInterpolator();

		/// TODO
		/// -----------------------------------------------------------------------
		///	\brief	Destructor. Deletes all data that was allocated (if any).
		~AttributeInterpolator();

		/// TODO
		/// -----------------------------------------------------------------------
		///	\fn		Init
		///	\brief	Initializes the attribute interpolator to use vertices are 
		///			assumed to be given in the following order top->middle->bot. 
		///			This function should allocate memory.
		void Init(const float* topVtx,
			const float* midVtx,
			const float* botVtx,
			const unsigned& vertexSize);

		/// TODO
		/// -----------------------------------------------------------------------
		///	\fn		StartScanLine
		///	\brief	Sets the current values to start values for every attributes. 
		///			Thus preparing them to start the scan line. 
		void StartScanLine();

		/// TODO
		/// -----------------------------------------------------------------------
		///	\fn		StepX
		///	\brief	Steps one positive unit along the X axis. This should update
		///			the current values.
		void StepX();

		/// TODO
		/// -----------------------------------------------------------------------
		///	\fn		StepY
		///	\brief	Steps one negative unit along the Y axis. This should update
		///			the start values based on the invSlope.
		void StepY(float invSlope);

		/// TODO
		/// -----------------------------------------------------------------------
		///	\fn		GetCurrentValues
		///	\brief	Returns the current value array. 
		float* GetCurrentValues();
	};

	/// TODO
	/// -----------------------------------------------------------------------
	/// \fn		DrawTriangleShaded
	/// \brief	Draws a triangle using the AttributeInterpolator class defined
	///			above. The traversal method should be Topleft and the 
	///			interpolation method should be the plane normal method. 
	///			(note: this is implemented via the AttributeInterpolator class)
	///			Additionally, this function determines the fragment color to 
	///			write to the frame buffer using the shader function provided
	///			and then blends it with the current color in the frame buffer.
	///			(Note: You should use FrameBuffer::GetPixel(x,y) to retrieve 
	///			the pixel color in the frame buffer at pixel position (x, y)
	/// \param	vertexData	-> 3 vertices provided into f32 array.
	/// \param	vertexSize	-> number of float elements in the vertex (includes x,y)
	/// \param	shader		-> shader function to call.
	/// \param	shaderData	-> shader data to pass. 
	void DrawTriangleShaded(const float* vertexData,
		const unsigned& vertexSize,
		ShaderFunc shader,
		void* shaderData);
}