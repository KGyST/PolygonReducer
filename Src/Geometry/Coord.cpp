#include "Coord.hpp"
#include <boost/format.hpp>
#include "Segment.hpp"


namespace S {

	//-----------------------   Utility functions //-----------------------

	std::optional<double> AngleBetween(const S::Coord* i_pStart, const S::Coord* i_pEnd, const S::Coord* i_P3) {
		Segment s1(i_pStart, i_P3);
		Segment s2(i_P3, i_pEnd);
		Coord cen = *s1.IntersectMidPerp(&s2);

		Coord v1 = *i_pStart - cen;
		Coord v2 = *i_pEnd - cen;

		double len1 = std::hypot(v1.GetX(), v1.GetY());
		double len2 = std::hypot(v2.GetX(), v2.GetY());

		if (len1 < EPS || len2 < EPS)
			return std::nullopt;

		v1.SetX(v1.GetX() / len1);
		v1.SetY(v1.GetY() / len1);
		v2.SetX(v2.GetX() / len2);
		v2.SetY(v2.GetY() / len2);

		double dot = v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY();
		double det = v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX();

		double angle = std::atan2(det, dot);

		Coord dirVec = *i_pEnd - *i_pStart;
		Coord dir3 = *i_P3 - *i_pStart;
		double d3t = dirVec.GetY() * (dir3.GetX()) - dirVec.GetX() * (dir3.GetY());

		if (d3t > 0)
		{
			if (angle < 0)
				angle += 2 * PI;
		}
		else
		{
			if (angle > 0)
				angle -= 2 * PI;
		}

		return angle;
	}
	
	//----------------------- / Utility functions //-----------------------

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

	// Operators

	bool Coord::operator==(const Coord &i_other) const
	{
    return	abs(m_x - i_other.m_x) < m_eps
			&&		abs(m_y - i_other.m_y) < m_eps;
	}

	// Converters

	::Coord Coord::ToCoord() const
	{
		return ::Coord(m_x, m_y);
	}

	API_Coord Coord::ToAPICoord() const
	{
		API_Coord res = { m_x, m_y };
		return res;
	}

	std::string Coord::ToString(LogFormat i_format) const
	{
		switch (i_format) {
		case LogFormat::JSON:
			return str(boost::format("{ \"x\": %-.2f, \"y\": %-.2f }") % m_x % m_y);
		default:
			return str(boost::format("%.2f, %.2f") % m_x % m_y);
		}
  }
}

