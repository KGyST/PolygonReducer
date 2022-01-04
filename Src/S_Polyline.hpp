#pragma once
#ifndef S_POLYLINE_HPP
#define S_POLYLINE_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "S_Segment.hpp"
//#include "S_Polygon.hpp"
#include "Point2DData.h"
#include "Sector2DData.h"

namespace PolygonReducer {
    class S_Polyline
    {
    private:
        GS::Array <S_Segment*>* segments;

    public:
        S_Polyline(GS::Array<API_Coord>* coords, GS::Array<API_PolyArc>* pars, GS::Array<UInt32>* vertexIDs);
        //S_Polyline(S_Polygon* p_owner, int p_iStart, int p_iEnd);
        ~S_Polyline();
    };
}
#endif // !S_POLYLINE_HPP
