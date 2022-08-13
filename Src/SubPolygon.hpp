#pragma once
#ifndef S_POLYLINE_HPP
#define S_POLYLINE_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Segment.hpp"
//#include "S_Polygon.hpp"
#include "Point2DData.h"
#include "Sector2DData.h"

namespace S {
    class SubPolygon
    {
    private:

    public:
        S::Array <Segment*> m_segments;

        SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs);
        SubPolygon() {};
        ~SubPolygon();
    };
}
#endif // !S_POLYLINE_HPP
