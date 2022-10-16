#include <AEEngine.h>
#include "Rasterizer.h"
#include <math.h>

namespace Rasterizer
{
	static ERoundMethod sRoundMethod = eRM_TRUNCATE;

	/// -----------------------------------------------------------------------
	///	\fn		GetRoundMethod
	///	\brief	Returns the current round method of the rasterizer
	ERoundMethod	GetRoundMethod()
	{
		return sRoundMethod;
	}

	/// -----------------------------------------------------------------------
	///	\fn		SetRoundMethod
	///	\brief	Sets the current round method of the rasterizer
	void SetRoundMethod(ERoundMethod rm)
	{	
		sRoundMethod = rm;
	}

	/// -----------------------------------------------------------------------
	/// \fn		Ceiling
	/// \brief	Ceil function. 
	int Ceiling(const float &p)
	{
		return (int)ceilf(p);
	}

	/// -----------------------------------------------------------------------
	/// \fn		Floor
	/// \brief	Floor function. 
	int Floor(const float &p) {

		return (int)floorf(p);
	
	}

	/// -----------------------------------------------------------------------
	/// \fn		Truncate
	/// \brief	Truncate rounding method (uses floor). 
	int Truncate(const float &p)
	{
		return Floor(p);
	}

	/// -----------------------------------------------------------------------
	/// \fn		TruncateShift
	/// \brief	Truncate then shift by 0.5f rounding method. 
	int TruncateShift(const float &p)
	{
		return Floor(p + 0.5f);
	}

	/// -----------------------------------------------------------------------
	/// \fn		Round
	/// \brief	Round method (wrapper around either Truncate or Truncate Shift
	int Round(const float &p)
	{
		switch(sRoundMethod)
		{
		case eRM_TRUNCATE_SHIFT:
			return TruncateShift(p);
		case eRM_TRUNCATE:
		default:
			return Truncate(p);
		}
	}
}