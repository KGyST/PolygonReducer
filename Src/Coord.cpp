#include "Coord.hpp"


namespace S {
	double S::Coord::m_eps = 0.01;

	Coord::Coord(const ::Coord& coord)
		:m_x(coord.x)
		, m_y(coord.y)
	{}


	Coord::Coord(const API_Coord& coord)
		: m_x(coord.x)
		, m_y(coord.y)
	{}


	Coord::Coord(const double x, const double y)
		: m_x(x)
		, m_y(y)
	{}


	bool Coord::operator==(const Coord& otherCoord) const
	{
		return	m_x - m_eps < otherCoord.GetX() 
			&&	m_x + m_eps > otherCoord.GetX()
			&&	m_y - m_eps < otherCoord.GetY()
			&&	m_y + m_eps > otherCoord.GetY();
	}


	::Coord Coord::ToCoord() const
	{
		return ::Coord(m_x, m_y);
	}


	API_Coord Coord::ToAPICoord() const
	{
		API_Coord res = { m_x, m_y };
		return res;
	}
}
