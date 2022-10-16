#ifndef _ROUNDING_H_
#define _ROUNDING_H_


namespace Rasterizer
{
	/// -----------------------------------------------------------------------
	/// \fn		Ceiling
	/// \brief	Ceil function. 
	int Ceiling(const float &p);

	/// -----------------------------------------------------------------------
	/// \fn		Floor
	/// \brief	Floor function. 
	int Floor(const float &p);

	/// -----------------------------------------------------------------------
	/// \fn		Truncate
	/// \brief	Truncate rounding method (uses floor). 
	int Truncate(const float &p);

	/// -----------------------------------------------------------------------
	/// \fn		TruncateShift
	/// \brief	Truncate then shift by 0.5f rounding method. 
	int TruncateShift(const float &p);

	/// -----------------------------------------------------------------------
	/// \fn		Round
	/// \brief	Round method (wrapper around either Truncate or Truncate Shift
	int Round(const float &p);

	/// -----------------------------------------------------------------------
	/// enum	ERoundMethod	
	/// \brief	Used to determine which rounding method to use.
	enum ERoundMethod {eRM_TRUNCATE, eRM_TRUNCATE_SHIFT, eRM_Count};

	/// -----------------------------------------------------------------------
	///	\fn		GetRoundMethod
	///	\brief	Returns the current round method of the rasterizer
	ERoundMethod	GetRoundMethod();

	/// -----------------------------------------------------------------------
	///	\fn		SetRoundMethod
	///	\brief	Sets the current round method of the rasterizer
	void SetRoundMethod(ERoundMethod rm);
}

#endif