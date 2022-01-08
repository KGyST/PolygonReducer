#pragma once

#ifndef S_COORD_HPP
#define S_COORD_HPP

#define ACExtension
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
		Coord(const ::Coord coord);
		Coord(const API_Coord coord);
		Coord(const double x, const double y);
		Coord() : m_x(0.00), m_y(0.00) {};
		~Coord() {};

		void setX(const double x) { m_x = x; };
		void setY(const double y) { m_y = y; };
		void setEps(const double eps) { m_eps = eps; };

		const double getX(void) { return m_x; };
		const double getY(void) { return m_y; };
		const double getEps(void) { return m_eps; };

		bool operator== (Coord anotherCoord);

		::Coord toCoord();
		API_Coord toAPICoord();
	};
}

#endif // !S_COORD_HPP