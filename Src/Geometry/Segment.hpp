#ifndef S_SEGMENT_HPP
#define S_SEGMENT_HPP

#define ACExtension

#include <string>
#include <optional>
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h

#include "Point2DData.h"
#include "Sector2DData.h"

#include "Coord.hpp"


namespace S {
  class Segment
  {
  private:
    S::Coord           m_start;
    S::Coord           m_end;

    Segment*        m_previous;
    Segment*        m_next;
    bool            m_startPoint;     // Is it a start of a subcontour
    //For arcs:
    double          m_angle;          // Radians

    void Init(int idx, int startIdx, int endIdx, const Coord& p_start, const Coord& p_end);

  public:
    Segment(const Coord& start, const Coord& end) { Init(0, 0, 0, start, end); }
    Segment(const Coord* start, const Coord* end) { Init(0, 0, 0, *start, *end); }
    Segment(int idx, const Coord& start, const Coord& end) { Init(idx, 0, 0, start, end); }
    Segment(int idx, const API_Coord& start, const API_Coord& end) { Init(idx, 0, 0, Coord(start), Coord(end)); }
    Segment(int idx, int startIdx, int endIdx, const Coord& start, const Coord& end) { Init(idx, startIdx, endIdx, start, end); }
    Segment() = default;
    //Segment(Sector sect);

    //Segment& operator=(const Segment&) noexcept;

    // Getters / Setters
    const S::Coord* GetStart() const { return &m_start; }
    void SetStart(const Coord& start) { m_start = start; }

    const S::Coord* GetEnd() const { return &m_end; }
    void SetEnd(const Coord& end) { m_end = end; }

    double  GetAng() const { return m_angle; }
    void SetAng(double angle) { m_angle = angle; }

    Segment* GetPrev() const { return m_previous; }
    void SetPrev(Segment* prev) { m_previous = prev; }

    Segment* GetNext() const { return m_next; }
    void SetNext(Segment* next) { m_next = next; }

    // Getters only
    std::optional<Coord>  GetCenter() const;
    const double GetLength() const;
    std::optional<double>  GetRad() const;

    // Geometry
    const Coord MidPoint() const;
    const Segment MidPerp() const;
    std::optional<Coord> IntersectMidPerp(Segment* io_other);
    bool Intersect(Segment* io_other);
    bool IsCollinear(const Segment*) const;
    bool IsArc() const;

    // Converters
    const Sector ToSector() const { return Sector(GetStart()->ToCoord(), GetEnd()->ToCoord()); }
    std::string ToString() const { return ToString(LogFormat::Default); }
    std::string ToString(LogFormat) const;
  };
}
#endif // !S_SEGMENT_HPP

