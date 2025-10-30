#include "Coord.hpp"

namespace S {
	double S::Coord::m_eps = 0.01;

	Coord::Coord(const ::Coord coord)
		:m_x(coord.x)
		, m_y(coord.y)
	{}


	Coord::Coord(const API_Coord coord)
		: m_x(coord.x)
		, m_y(coord.y)
	{}


	Coord::Coord(const double x, const double y)
		: m_x(x)
		, m_y(y)
	{}


	bool Coord::operator==(const Coord anotherCoord) const
	{
		return	m_x - m_eps < anotherCoord.m_x
			&&	m_x + m_eps > anotherCoord.m_x
			&&	m_y - m_eps < anotherCoord.m_y
			&&	m_y + m_eps > anotherCoord.m_y;
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


std::optional<double> AngleBetween(const Coord& i_p1, const Coord& i_cen, const Coord& i_p2) {
	Coord v1{ i_p1.x - i_cen.x, i_p1.y - i_cen.y };
	Coord v2{ i_p2.x - i_cen.x, i_p2.y - i_cen.y };

	double len1 = std::hypot(v1.x, v1.y);
	double len2 = std::hypot(v2.x, v2.y);
	if (len1 < EPS || len2 < EPS)
		return std::nullopt;

	double dot = v1.x * v2.x + v1.y * v2.y;
	double cosang = dot / (len1 * len2);

	cosang = std::max(std::min(cosang, 1.0), -1.0);

	return std::acos(cosang);
}


