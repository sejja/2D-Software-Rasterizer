#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "../Rasterizer/Color.h"

namespace Rasterizer {

	class Viewport {
	private:
		AEVec2	mPosition;		// position in the framebuffer (this will eventual
		AEVec2	mDimensions;	// Width and height of the viewport in the frame buffer
		Color	mColor;			// Color of the viewport
	public:

		// PROVIDED
		// ------------------------------------------------------------------------
		/// \fn		Viewport (default constructor)
		/// \brief	Constructs a viewport at (0,0), size of the screen and black color
		Viewport(): mPosition(0.0f, 0.0f), mDimensions((f32)gAESysWinWidth, (f32)gAESysWinHeight)	
		{}


		// PROVIDED
		// ------------------------------------------------------------------------
		/// \fn		Viewport (custom constructor)
		/// \brief	Constructs a viewport at the specified position and dimensions.
		/// \param	position	position of the viewport (bottom left) in the framebuffer 
		/// \param	dimensions	dimensions of the viewport in the frame buffer.
		Viewport(AEVec2 position, AEVec2 dimensions): mPosition(position), mDimensions(dimensions)
		{}
		

		// TODO
		// ------------------------------------------------------------------------
		/// \fn		ComputeMatrix
		/// \brief	Computes the camera to viewport matrix using the method seen in
		///			class. After this transform, the points should be in frame-buffer
		///			coordinates and ready to be sent to the Rasterizer. 
		AEMtx33 ComputeMatrix();


		// TODO
		// ------------------------------------------------------------------------
		/// \fn		ComputeInvMatrix
		/// \brief	Computes the viewport to camera  matrix. After this transform, 
		///			the points should be in NDC coordinates. You cannot use 
		///			AEMtx33Inverse to compute the matrix.
		AEMtx33 ComputeInvMatrix();

		// ------------------------------------------------------------------------
		#pragma region // GETTERS & SETTERS (PROVIDED)
		
		void SetPosition(const AEVec2 & newPos)	{ mPosition = newPos;	}
		AEVec2 GetPosition()const				{ return mPosition; }
		void SetRect(const AEVec2 & rect)		{ mDimensions = rect; }
		AEVec2 GetRect()const					{ return mDimensions; }
		void SetColor(const Color & col)		{ mColor = col; }
		Color GetColor()const					{ return mColor; }

		#pragma endregion

	};
}

#endif