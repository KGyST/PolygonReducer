#pragma once

#ifndef S_SEGMENT_HPP
#define S_SEGMENT_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Point2DData.h"
#include "Sector2DData.h"
#include <string>
#include "Coord.hpp"
#include <optional>


namespace S {
  class Segment
  {
  private:
    Coord           m_start;
    Coord           m_end;

    //unsigned int    m_status1;
    //unsigned int    m_status2;
    Segment*        m_previous;
    Segment*        m_next;
    bool            m_startPoint;       // Is it a start of a subcontour
    //For arcs:
    double          m_angle;          // Radians

    //DELETE
    void init(int idx, int startIdx, int endIdx, const Coord& p_start, const Coord& p_end);


  public:
    Segment() {}
    Segment(const Coord& start, const Coord& end) { init(0, 0, 0, start, end); }
    Segment(int idx, const Coord& start, const Coord& end) { init(idx, 0, 0, start, end); }
    Segment(int idx, const API_Coord& start, const API_Coord& end) { init(idx, 0, 0, Coord(start), Coord(end)); }
    Segment(int idx, int startIdx, int endIdx, const Coord& start, const Coord& end) { init(idx, startIdx, endIdx, start, end); }
    //Segment(Sector sect);

    //Segment& operator=(const Segment&) noexcept;

    // Getters
    const Coord* GetStart() const { return &m_start; }
    const Coord* GetEnd() const { return &m_end; }
    Segment* GetPrev() const { return m_previous; }
    Segment* GetNext() const { return m_next; }
    std::optional<double>  GetRad() const;
    double  GetAng() const { return m_angle; }
    std::optional<Coord>  GetCenter() const;
    const double GetLength() const;

    // Setters
    void SetAng(double angle) { m_angle = angle; }
    void SetStart(const Coord& start) { m_start = start; }
    void SetEnd(const Coord& end) { m_end = end; }
    void SetPrev(Segment* prev) { m_previous = prev; }
    void SetNext(Segment* next) { m_next = next; }
    //void SetCenter(const Coord i_center) { m_center = i_center; }

    // Geometry
    const Coord MidPoint() const;
    const Segment MidPerp() const;
    bool Intersect(Segment* io_other);
    bool IsCollinear(const Segment*) const;
    std::optional<Coord> IntersectMidPerp(Segment* io_other);
    bool IsArc() const;

    // Converters
    const Sector ToSector() const { return Sector(GetStart()->ToCoord(), GetEnd()->ToCoord()); }
    std::string ToString() const { return ToString(LogFormat::Default); }
    std::string ToString(LogFormat) const;
  };
}


#endif // !S_SEGMENT_HPP

