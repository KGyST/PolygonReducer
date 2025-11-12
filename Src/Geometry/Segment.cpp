#include "Segment.hpp"
#include "APICommon.h"

#include <boost/format.hpp>
#include "GeometryHelpers.hpp"


using namespace Geometry;


namespace S {
  //  Private methods

  void Segment::Init(int idx, int startIdx, int endIdx, const Coord& p_start, const Coord& p_end)
  {
    m_start = p_start;
    m_end = p_end;
    m_angle = 0;
  }

  // /Private methods

  // Getters only

  std::optional<Coord> Segment::GetCenter() const
  {
    if (IsArc())
    {
      double angle = PI / 2.00 - m_angle / 2.00;
      double radius = *GetRad();
      double halfDist = Dist(m_start.ToCoord(), m_end.ToCoord()) / 2;
      double distToCenter = radius * cos(angle);
      ::Coord dirVec = (m_end - m_start).ToCoord();
      double vecLength = sqrt(dirVec.x * dirVec.x + dirVec.y * dirVec.y);
      ::Coord unitVec = ::Coord(dirVec.x / vecLength, dirVec.y / vecLength);
      ::Coord center = ::Coord(m_start.GetX() + unitVec.x * halfDist - unitVec.y * distToCenter,
        m_start.GetY() + unitVec.y * halfDist + unitVec.x * distToCenter);
      return Coord(center.x, center.y);
    }
    else
      return std::nullopt;
  }

  const double Segment::GetLength() const
  {
    if (IsArc())
      return abs(*GetRad() * m_angle);
    else
      return sqrt(pow((m_end.GetX() - m_start.GetX()), 2) + pow((m_end.GetY() - m_start.GetY()), 2));
  }

  std::optional<double> Segment::GetRad() const
  {
    if (IsArc())
    {
      double halfDist = Dist(m_start.ToCoord(), m_end.ToCoord()) / 2;
      return halfDist / sin(m_angle / 2);
    }
    else
      return std::nullopt;
  }

  // Geometry

  const Coord Segment::MidPoint() const
  {
    if (abs(m_angle) < EPS)
      return Coord((m_start.GetX() + m_end.GetX()) / 2, (m_start.GetY() + m_end.GetY()) / 2);
    else
    {
      // Arc
      double halfAngle = GetAng() / 2.00;
      double radius = *GetRad();
      ::Coord center = GetCenter()->ToCoord();
      ::Coord start = m_start.ToCoord();
      ::Coord midPoint = RotCoord(center, start, sin(halfAngle), cos(halfAngle));
      return Coord(midPoint.x, midPoint.y);
    }
  }

  const Segment Segment::MidPerp() const
  {
    if (abs(m_angle) < EPS)
    {
      ::Coord _start = m_start.ToCoord();
      ::Coord _midPoint = MidPoint().ToCoord();

      ::Coord _rotEnd = RotCoord((Point2D)_midPoint, (Point2D)_start, 1.00, 0.00);

      return Segment(MidPoint(), Coord(_rotEnd));
    }
    else
    {
      // Arc
      double halfAngle = GetAng() / 2.00;
      double radius = *GetRad();
      ::Coord center = GetCenter()->ToCoord();
      ::Coord start = m_start.ToCoord();
      ::Coord midPoint = RotCoord(center, start, sin(halfAngle), cos(halfAngle));
      ::Coord _rotEnd = RotCoord((Point2D)midPoint, (Point2D)start, 1.00, 0.00);
      return Segment(MidPoint(), Coord(_rotEnd));
    }
  }

  std::optional<Coord> Segment::IntersectMidPerp(Segment* io_other)
  {
    using namespace Geometry;

    Sector lin1 = MidPerp().ToSector();
    Sector lin2 = io_other->MidPerp().ToSector();

    ::Coord xc(0, 0);

    double eps = 0, radEps = 0;

    if (XLinesEps(lin1, lin2, &xc, eps, radEps))
      return Coord(xc.x, xc.y);
    else
      return std::nullopt;
  }

  bool Segment::Intersect(Segment* io_other)
  {
    using namespace Geometry;

    Sector lin1 = ToSector();
    Sector lin2 = io_other->ToSector();

    ::Coord xc(0, 0);

    double eps = 0, radEps = 0;

    if (XLinesEps(lin1, lin2, &xc, eps, radEps))
    {
      SetEnd(xc);
      io_other->SetStart(xc);
      SetNext(io_other);
      io_other->SetPrev(this);
      return true;
    }
    else
    {
      return false;
    }
  }

  bool Segment::IsCollinear(const Segment* i_other) const
  {
    using namespace Geometry;

    Sector lin1 = ToSector();
    Sector lin2 = i_other->ToSector();

    ::Coord xc(0, 0);

    double eps = 0, radEps = 0;

    if (XLinesEps(lin1, lin2, &xc, eps, radEps))
      return false;
    else
      return true;
  }

  bool Segment::IsArc() const
  {
    return (abs(m_angle) > EPS);
  }

  // Converters
  
  std::string Segment::ToString(LogFormat i_format) const
  {
    std::string result = "";

    switch (i_format) {
    case LogFormat::Default:
      result = str(boost::format("%-.2f %-.2f") % m_start.GetX() % m_start.GetY());
      return result;
    case LogFormat::Short:
      if (m_angle)
        result = m_start.ToString(i_format) + " -> " + str(boost::format("%-.2f  (%-.2f %-.2f) -> ") % m_angle % GetCenter()->GetX() % GetCenter()->GetY()) + m_end.ToString(i_format) + "\n";
      else
        result = m_start.ToString(i_format) + " -> " + m_end.ToString(i_format) + "\n";
      return result;
    case LogFormat::Detailed:
      result += str(boost::format(" Start: %-.2f %-.2f\n") % m_start.GetX() % m_start.GetY());
      result += str(boost::format(" End: %-.2f %-.2f\n") % m_end.GetX() % m_end.GetY());
      result += str(boost::format(" Length: %-.2f\n") % GetLength());
      return result;
    case LogFormat::JSON:
      result += "{ ";
      result += "\"start\": " + m_start.ToString(i_format) + ", ";
      result += "\"end\": " + m_end.ToString(i_format) + ", ";
      if (m_angle)
        result += str(boost::format("\"angle\": %-.2f, \"center\": {%-.2f, %-.2f}") % m_angle % GetCenter()->GetX() % GetCenter()->GetY() );
      result += " }";
      return result;
    }

    return result;
  }
}

