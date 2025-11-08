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
#include "Logger/Logger.hpp"

extern Logger logger;

namespace S {
  class Polygon
  {
    // Representing a polygon which can have multiple subpolygons (of class SubPolygon)
    // (first representing the contour, others inner holes; all non-intersecting)
// of straight edges and circular arcs (of class Segment)
  private:
    //UINT m_pointCount;
    void RemoveShortestSegment();
    SubPolygon* GetSPolyHavingShortestSegment() const;

    void SetUserdata();                                 //TODO
    void _Init(const API_ElementMemo*);

  public:
    bool m_isPolyline;                                   // False if polyline, true if hatch
    Array <SubPolygon*> m_subpolys;                     // Subpolygons, like contour or holes

    Polygon(const API_ElementMemo* p_memo);
    Polygon(const API_ElementMemo& p_memo) : Polygon(&p_memo) {}
    Polygon(const API_Neig*);
    Polygon(const API_Guid*);
    Polygon(const Polygon&);
    Polygon();
    ~Polygon();

    Polygon& operator=(const Polygon&);

    // Getters
    std::string GetGDLcode() const;
    void GetMemo(API_ElementMemo&) const;

    void SetPointCount(unsigned int i_count);     // The main purpose of the addon
    USize GetPointCount() const;                              // 

    API_Polygon ToPoly() const;

    double GetShortestEdgeLength() const;               // Length of the shortest edge
    void SetShortestEdgeLength(double i_length);

    void IntersectSegments(Segment* io_prev, Segment* io_next);  // Intersect two segments

    void MoveAllPoints();                               // For testing
    // Converters
    std::string ToString() const;
  };
}
#endif // !S_POLYGON_HPP

