#include "Segment.hpp"
#include "APICommon.h"

#include <boost/format.hpp>
#include "GeometryHelpers.hpp"

using namespace Geometry;

namespace S {
    void Segment::init(int idx, int startIdx, int endIdx, const Coord& p_start, const Coord& p_end)
    {
        m_start = p_start;
        m_end = p_end;
        m_status1 = 0;
        m_status2 = 0;
    }

    //Segment::Segment(int idx, int startIdx, int endIdx, const Coord& start, const Coord& end, Segment* prev, Segment* next)
    //    :m_idx(idx)
    //    ,m_startIdx(startIdx)
    //    ,m_endIdx(endIdx)
    //    ,m_start(start)
    //    ,m_end(end)
    //    ,m_previous(prev)
    //    ,m_next(next)

  //  Segment& Segment::operator=(const Segment& i_other) noexcept
  //  {
  //      if (this == &i_other)
  //          return *this;

  //      m_previous = i_other.m_previous;
  //      m_next = i_other.m_next;

		//return *this;
  //  }

    const Coord Segment::MidPoint() const
    {
        //TODO arc
        return Coord((m_start.GetX() + m_end.GetX()) / 2, (m_start.GetY() + m_end.GetY()) / 2);
    }

    const Segment Segment::MidPerp() const
    {
        //TODO arc

        ::Coord _start = m_start.ToCoord();
        ::Coord _midPoint = MidPoint().ToCoord();

        ::Coord _rotEnd = RotCoord((Point2D) _midPoint, (Point2D) _start, 1.00, 0.00);

        return Segment(MidPoint(), Coord(_rotEnd));
    }

    void Segment::SetAng(double angle)
    {
        Coord _start(m_start.GetX(), m_start.GetY());
        Coord _end(m_end.GetX(), m_end.GetY());
        double _halfAngle = angle / 2;

        ::Coord rotEnd = RotCoord(_start.ToCoord(), _end.ToCoord(), sin(_halfAngle), cos(_halfAngle));
        ::Coord rotStart = RotCoord(_end.ToCoord(), _start.ToCoord(), sin(-_halfAngle), cos(-_halfAngle));

		Sector lin1{ _start.ToCoord(), rotEnd };
		Sector lin2{ _end.ToCoord(), rotStart };

        ::Coord xc(0, 0);

        double eps = 0, radEps = 0;

        XLinesEps(lin1, lin2, &xc, eps, radEps);

		m_center = Coord(xc.x, xc.y);
    }

    //void Segment::SetArc(double angle, Coord center)
    //{
    //    //m_angle = angle;
    //    m_center->SetX(center.GetX());
    //    m_center->SetY(center.GetY());
    //    //TODO m_radius
    //}

    std::string Segment::ToString(LogFormat i_format) const
    {
        std::string result = "";

		switch (i_format) {
		case LogFormat::Default:
            result = str(boost::format("%-.2f %-.2f") % m_start.GetX() % m_start.GetY());
			return result;
            return result;
		case LogFormat::Short:
            result = str(boost::format("%-.2f %-.2f -> %-.2f %-.2f") % m_start.GetX() % m_start.GetY() % m_end.GetX() % m_end.GetY());
			return result;
		case LogFormat::Detailed:
			result += str(boost::format(" Start: %-.2f %-.2f\n") % m_start.GetX() % m_start.GetY());
			result += str(boost::format(" End: %-.2f %-.2f\n") % m_end.GetX() % m_end.GetY());
			result += str(boost::format(" Length: %-.2f\n") % GetLength());
			return result;
		}

        return result;
    }

    const double Segment::GetLength() const
    {
        if (!m_center)
            return sqrt(pow((m_end.GetX() - m_start.GetX()), 2) + pow((m_end.GetY() - m_start.GetY()), 2));
        else
            return abs(*GetRad() * *GetAng());
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

    std::optional<double> Segment::GetRad() const
    {
        if (m_center)
            return Dist(m_start.ToCoord(), m_center->ToCoord());
        else
			return std::nullopt;
    }

    std::optional<double> Segment::GetAng() const
    {
        if (!m_center)
            return std::nullopt;

        Coord v1 = *m_center - m_start;
        Coord v2 = *m_center - m_end;

        double angle = std::atan2(v2.GetY(), v2.GetX()) - std::atan2(v1.GetY(), v1.GetX());

        if (angle < -PI - EPS)
            angle += 2 * PI;

        if (angle > PI + EPS)
            angle -= 2 * PI;

        return angle;
    }       
}
