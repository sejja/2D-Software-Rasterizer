#include "Rasterizer.h"
#include <AEEngine.h>

#ifndef _VERTEX_H_
#define _VERTEX_H_

namespace Rasterizer
{

	namespace VertexDefs
	{
		// PROVIDED
		/// -----------------------------------------------------------------------
		/// \class	Vertex
		///	\brief	Simple structure used to define a triangle vertex with both position
		///			and color attribute. 
		struct VtxPosColor
		{
			AEVec2  mPosition;	// x,y
			Color	mColor;		// r,g,b,a
		};

		/// \class	Vertex
		///	\brief	Represents a vertex with position, texture coordinates and color
		struct VtxPosTexCol
		{
			VtxPosTexCol()
				:mPosition(AEVec2(0)), mTexCoord(AEVec2(0)), mColor(Color(0, 0, 0, 1)) {}
			VtxPosTexCol(AEVec2 pos, AEVec2 tex, Color col)
				:mPosition(pos), mTexCoord(tex), mColor(col) {}
			AEVec2 mPosition;	//! x,y
			AEVec2 mTexCoord;	//! u,v
			Color  mColor;		//! r,g,b,a
		};
	}

	// backwards compatibility with assignment 3
	typedef VertexDefs::VtxPosColor Vertex;
}
// ----------------------------------------------------------------------------
#endif