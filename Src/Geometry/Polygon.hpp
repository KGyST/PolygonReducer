#pragma once
#pragma once
#ifndef S_POLYGON_HPP
#define S_POLYGON_HPP

#define ACExtension
#include "ACAPinc.h"					            // also includes APIdefs.h
#include "SubPolygon.hpp"
#include "Segment.hpp"
#include "../PolygonReducer.hpp"
#include "Array.hpp"
#include <string>


namespace S {
    class Polygon
    {
        // Representing a polygon which can have multiple subpolygons
        // (first representing the contour, others inner holes; all non-intersecting)
        // of straight edges and circular arcs 
    private:
        UINT m_pointCount;
        void removeShortestEdge();
        void removeSegment(Segment* i_segment);

        void SetUserdata();                               //TODO

    public:
        Polygon(const API_ElementMemo* p_memo);
        ~Polygon();

        std::string getGDLcode();
        API_ElementMemo getMemo();

        void setPointCount(const unsigned int i_count);     // The main purpose of the addon
        USize getPointCount();                              // 

        void MoveAllPoints();                               // For testing

        bool m_isPolygon;                                   // False if polyline

        void intersectSegments( Segment* io_prev,  Segment* io_next);  // Intersect two segments


        Array <SubPolygon*> m_subpolys;           // Subpolygons, like contour or holes
        Array <Segment*> m_segments;             // Segments: arcs or edges
    };
}
#endif // !S_POLYGON_HPP