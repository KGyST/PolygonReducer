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
        Array <Segment*> m_segments;

        SubPolygon(UInt32 iStart, UInt32 iEnd, Array<API_Coord>& coords, Array<API_PolyArc>& parcs, Array<UInt32>& vertexIDs, Array<Segment*>& o_segments);
        SubPolygon() {};
        ~SubPolygon();

        inline void RemoveSegment(Segment* i_segment) { m_segments.DeleteAll(i_segment); }
        void CreateArcsFromPolys();
        void CreatePolysFromArcs();

    };
}
#endif // !S_POLYLINE_HPP
