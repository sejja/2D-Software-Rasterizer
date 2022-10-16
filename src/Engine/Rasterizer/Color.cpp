#include <AEEngine.h>
#include "Color.h"
namespace Rasterizer
{
	// ------------------------------------------------------------------------
	/// \fn		Default & custom
	/// \brief	
	Color::Color()
		: r(0.0f)
		, g(0.0f)
		, b(0.0f)
		, a(1.0f)
	{}

	// ------------------------------------------------------------------------
	/// \fn		Default & custom
	/// \brief	
	Color::Color(f32 rr, f32 gg, f32 bb, f32 aa)
		: r(rr)
		, g(gg)
		, b(bb)
		, a(aa)
	{}

	// ------------------------------------------------------------------------
	/// \fn		FromU32
	/// \brief	Set the color to the u32 color in format ARGB hex
	Color & Color::FromU32(u32 color) // custom constructor
	{
		// decompose
		u32 aa = (color & (0x000000FF << 24)) >> 24;
		u32 rr = (color & (0x000000FF << 16)) >> 16;
		u32 gg = (color & (0x000000FF << 8)) >> 8;
		u32 bb = color & 0x000000FF;

		// float
		r = (f32)rr / 255.0f;
		g = (f32)gg / 255.0f;
		b = (f32)bb / 255.0f;
		a = (f32)aa / 255.0f;

		// return the modified color
		return *this;
	}

	// ------------------------------------------------------------------------
	/// \fn		ToU32
	/// \brief	returns u32 color from this color.
	u32 Color::ToU32()	const // conversion operator
	{
		// convert each element to range [0,255]
		u32 aa = (a < 0.0f) ? (0) : ((a > 1.0f) ? (255) : u32(a*255.0f));
		u32 rr = (r < 0.0f) ? (0) : ((r > 1.0f) ? (255) : u32(r*255.0f));
		u32 gg = (g < 0.0f) ? (0) : ((g > 1.0f) ? (255) : u32(g*255.0f));
		u32 bb = (b < 0.0f) ? (0) : ((b > 1.0f) ? (255) : u32(b*255.0f));

		// pack
		u32 c = (aa << 24) | (rr << 16) | (gg << 8) | bb;
		return c;
	}

	Color Color::operator *(const float & sc)const 
	{
		return Color(r*sc, g*sc, b*sc, a*sc); 
	}
	Color& Color::operator *=(const float & sc) 
	{
		return *this = *this * sc; 
	}
	Color Color::operator +(const Color & rhs)const  
	{
		return Color(
			r + rhs.r,
			g + rhs.g,
			b + rhs.b,
			a + rhs.a);
	}
	Color& Color::operator += (const Color & rhs)
	{
		return *this = *this + rhs;
	}
	Color Color::operator-(const Color& rhs) const
	{
		return Color(
			r - rhs.r,
			g - rhs.g,
			b - rhs.b,
			a - rhs.a);
	}
	Color& Color::operator-=(const Color& rhs)
	{
		return *this = *this - rhs;
	}
	Color Color::operator*(const Color& rhs) const
	{
		return Color(
			r * rhs.r,
			g * rhs.g,
			b * rhs.b,
			a * rhs.a);
	}
	Color& Color::operator*=(const Color& rhs)
	{
		return *this = *this * rhs;
	}
}// namespace Rasterizer