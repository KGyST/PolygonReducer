#pragma once
#ifndef S_POLYLINE_HPP
#define S_POLYLINE_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Segment.hpp"
#include "Sector2DData.h"
#include "../Containers/Array.hpp"

namespace S {
    class SubPolygon
    {
    private:

    public:
        Array <Segment*> m_segments;

        SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs);
        //SubPolygon(const SubPolygon&);
        SubPolygon() {};
        ~SubPolygon() {};

        inline void RemoveSegment(Segment* i_segment) { m_segments.DeleteAll(i_segment); }
    };
}
#endif // !S_POLYLINE_HPP
