#ifndef _COLOR_H_
#define _COLOR_H_

namespace Rasterizer
{
#pragma warning (disable:4201) // nameless struct warning
	struct Color
	{
		union
		{
			struct
			{
				float r, g, b, a;
			};
			float v[4];
		};

		Color();
		Color(float rr, float gg, float bb, float aa = 1.0f);
		
		Color	operator *(const float & sc) const;
		Color&	operator *=(const float & sc);
		Color	operator +(const Color& rhs) const;
		Color&	operator += (const Color& rhs);
		Color	operator *(const Color& rhs) const;
		Color&	operator *= (const Color& rhs);
		Color& FromU32(unsigned long colorU32);
		Color   operator-(const Color& rhs) const;
		Color&  operator-=(const Color& rhs);
		unsigned long    ToU32()const;
	};
#pragma warning (default:4201) // nameless struct warning
}

#endif