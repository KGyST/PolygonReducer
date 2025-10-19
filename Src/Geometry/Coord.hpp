#pragma once

#ifndef S_COORD_HPP
#define S_COORD_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Point2D.hpp"


namespace S {
	class Coord : public ::Coord
	{
	private:
		static double m_eps;
		double m_x;
		double m_y;
	public:
		Coord()
			:	m_x(0.00)
			,	m_y(0.00) {};
		Coord(const ::Coord coord);
		Coord(const API_Coord coord);
		Coord(const double x, const double y);
		Coord(const Coord& c)
			:	m_x(c.GetX())
			,	m_y(c.GetY()) {};;
		~Coord() {};

		void SetX(const double x) { m_x = x; };
		void SetY(const double y) { m_y = y; };
		void SetEps(const double eps) { m_eps = eps; };

		const double GetX(void) const { return m_x; };
		const double GetY(void) const { return m_y; };
		const double GetEps(void) const { return m_eps; };

		bool operator== (Coord anotherCoord) const;

		::Coord ToCoord() const;
		API_Coord ToAPICoord() const;
	};
}

#endif // !S_COORD_HPP

