#pragma once
#ifndef S_POLYGON_HPP
#define S_POLYGON_HPP

#define ACExtension
#include "ACAPinc.h"					            // also includes APIdefs.h
#include "S_SubPoly.hpp"
#include "Segment.hpp"
#include "PolygonReducer.hpp"
#include "Array.hpp"
#include <string>

namespace PolygonReducer {
    class S_Polygon
    {
        // Representing a polygon which can have multiple subpolygons
        // (first representing the contour, others inner holes; all non-intersecting)
        // of straight edges and circular arcs 
    private:
        UINT m_pointCount;
        void removeShortestEdge();

        //void SetUserdata();

    public:
        S_Polygon(const API_ElementMemo* p_memo);
        ~S_Polygon();

        std::string getGDLcode();
        API_ElementMemo getMemo();

        void setPointCount(const unsigned int i_count);     // The main purpose of the addon
        USize getPointCount();                              // 

        void MoveAllPoints();                               // For testing

        bool m_isPolygon;                                   // False if polyline

        void intersectSegments( S::Segment* io_prev,  S::Segment* io_next);  // Intersect two segments


        GS::Array <S_SubPoly> m_subpolys;           // Subpolygons, like contour or holes TODO: to S::Array
        S::Array <S::Segment*> m_segments;          // Segments: arcs or edges
    };
}
#endif // !S_POLYGON_HPP