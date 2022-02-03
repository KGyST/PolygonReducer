#pragma once
#ifndef S_POLYLINE_HPP
#define S_POLYLINE_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Segment.hpp"
//#include "S_Polygon.hpp"
#include "Point2DData.h"
#include "Sector2DData.h"

namespace PolygonReducer {
    class S_SubPoly
    {
    private:

    public:
        GS::Array <S::Segment*> m_segments;
        S_SubPoly(GS::Array<API_Coord>* coords, GS::Array<API_PolyArc>* pars, GS::Array<UInt32>* vertexIDs);
        S_SubPoly() {};
        ~S_SubPoly();
    };
}
#endif // !S_POLYLINE_HPP
