#include "Coord.hpp"

double S::Coord::m_eps = 0.01;

S::Coord::Coord(const ::Coord coord) :
	m_x(coord.x),
	m_y(coord.y)
{}


S::Coord::Coord(const API_Coord coord) :
	m_x(coord.x),
	m_y(coord.y)
{}


S::Coord::Coord(const double x, const double y) :
	m_x(x),
	m_y(y)
{}


bool S::Coord::operator==(Coord anotherCoord)
{
	return	m_x - m_eps < anotherCoord.x
		&&	m_x + m_eps > anotherCoord.x
		&&	m_y - m_eps < anotherCoord.y
		&&	m_y + m_eps > anotherCoord.y;
}


::Coord S::Coord::toCoord() const
{
	return ::Coord(m_x, m_y);
}


API_Coord S::Coord::toAPICoord() const
{
	API_Coord res = { m_x, m_y };
	return res;
}

