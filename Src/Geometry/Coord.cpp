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

	bool Coord::operator==(const Coord &i_other) const
	{
    return	abs(m_x - i_other.m_x) < m_eps
			&&		abs(m_y - i_other.m_y) < m_eps;
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

	double len1 = hypot(v1.x, v1.y);
	double len2 = hypot(v2.x, v2.y);

	if (len1 < EPS || len2 < EPS)
		return std::nullopt;

	double dot = v1.x * v2.x + v1.y * v2.y;
	double cosang = dot / (len1 * len2);

	cosang = max(min(cosang, 1.0), -1.0);

	if (cosang > -1.0 + EPS)
		return acos(cosang);
	else
    return -acos(cosang);
}

