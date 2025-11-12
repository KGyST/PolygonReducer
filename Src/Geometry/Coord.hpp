#ifndef S_COORD_HPP
#define S_COORD_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Point2D.hpp"
#include "Vector2D.hpp"
#include "GeometryHelpers.hpp"
#include <optional>


// FIXME Move to somewhere else
enum class LogFormat {
	Default,
	Short,
	Detailed,
	JSON,
	GDL
};


namespace S {
	std::optional<double> AngleBetween(const Coord& i_pStart, const Coord& i_pEnd, const Coord& i_P3);

	class Coord : public ::Coord
	{
	private:
		static double m_eps;
		double m_x;
		double m_y;
	public:
		Coord()
			: m_x(0.00)
			, m_y(0.00) {
		}
		Coord(const ::Coord coord);
		Coord(const API_Coord coord);
		Coord(const double x, const double y);
		Coord(const Coord& c)
			: m_x(c.GetX())
			, m_y(c.GetY()) {
		}
		Coord(const Geometry::Vector2<double>& p)
			: m_x(p.x)
			, m_y(p.y) {} 
    ~Coord() = default;

    // Operators
		bool operator== (const Coord& i_other) const;
		Coord operator- (const Coord& anotherCoord) const { return this->ToCoord() - anotherCoord.ToCoord(); };

		// Getters / Setters
		const double GetX(void) const { return m_x; }
		void SetX(const double x) { m_x = x; }

		const double GetY(void) const { return m_y; }
		void SetY(const double y) { m_y = y; }

		const double GetEps(void) const { return m_eps; }
		void SetEps(const double eps) { m_eps = eps; }

		// Converters
		::Coord ToCoord() const;
		API_Coord ToAPICoord() const;
		std::string ToString(LogFormat i_format = LogFormat::Short) const;
	};
}
#endif // !S_COORD_HPP

