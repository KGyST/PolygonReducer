#ifndef S_POLYLINE_HPP
#define S_POLYLINE_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h

#include "Sector2DData.h"

#include "Segment.hpp"
#include "../Gui/SettingsSingleton.hpp"
#include "../Containers/Array.hpp"
//#include "Logger/Logger.hpp"

extern Logger logger;

namespace S {
  class SubPolygon
  {
  private:
    void DeepCopy(const SubPolygon& other);
    double SignedArea() const;
    void PolyToArc();
    void RemoveCollinear();
    // FIXME
    void ArcToPoly();

  public:
    CircularArray <Segment*> m_segments;                // Segments: arcs or edges
    bool 		     m_isHole = false;

    SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs, const bool i_isHole = false);
    SubPolygon(const SubPolygon&);
    SubPolygon(SubPolygon&&) = delete;
    SubPolygon() {}
    ~SubPolygon() = default;

    // Operators
    //SubPolygon& SubPolygon::operator=(const SubPolygon&);
    SubPolygon& operator=(const SubPolygon&);
    SubPolygon& operator=(SubPolygon&&) = delete;

    // Getters only
    Segment* GetShortestSegment() const;
    double GetShortestEdgeLength() const;
    inline bool IsValid() const { return m_segments.GetSize() >= 3; }
    bool IsClockWise() const;

    // Geometry
    void RemoveShortestSegment();
    inline void RemoveSegment(Segment* i_segment) { m_segments.DeleteAll(i_segment); }
    void Preprocess();
    void Postprocess();

    // Converters
    std::string ToString() const;
  };
}
#endif // !S_POLYLINE_HPP

