#pragma once
#ifndef S_POLYLINE_HPP
#define S_POLYLINE_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Segment.hpp"
#include "Sector2DData.h"
#include "../Containers/Array.hpp"
#include "Logger/Logger.hpp"

extern Logger logger;

namespace S {
  class SubPolygon
  {
  private:
    void PolyToArc();
    void RemoveCollinear();
    void ArcToPoly();

  public:
    CircularArray <Segment*> m_segments;                // Segments: arcs or edges
    bool 		     m_isHole = false;

    SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs, const bool i_isHole = false);
    SubPolygon(const SubPolygon&);
    SubPolygon() {}
    ~SubPolygon() = default;

    void Preprocess();
    void Postprocess();

    //SubPolygon& SubPolygon::operator=(const SubPolygon&);
    SubPolygon& operator=(const SubPolygon&) = delete;
    SubPolygon& operator=(SubPolygon&&) = delete;

    inline void RemoveSegment(Segment* i_segment) { m_segments.DeleteAll(i_segment); }
    Segment* GetShortestSegment() const;
    void RemoveShortestSegment();
    double GetShortestEdgeLength() const;

    inline bool IsValid() const { return m_segments.GetSize() >= 3; }
    // Geometry

    // Converters
    std::string ToString() const;
  };
}
#endif // !S_POLYLINE_HPP

