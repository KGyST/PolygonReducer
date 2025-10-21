#include "Segment.hpp"
#include "APICommon.h"

#include <boost/format.hpp>

using namespace Geometry;

namespace S {
    void Segment::init(int idx, int startIdx, int endIdx, const Coord& p_start, const Coord& p_end)
    {
        m_start = p_start;
        m_end = p_end;
        m_idx = idx;
        m_startIdx = startIdx;
        m_endIdx = endIdx;
        m_status1 = 0;
        m_status2 = 0;
        m_angle = 0.00;
    }

    //Segment::Segment(int idx, int startIdx, int endIdx, const Coord& start, const Coord& end, Segment* prev, Segment* next)
    //    :m_idx(idx)
    //    ,m_startIdx(startIdx)
    //    ,m_endIdx(endIdx)
    //    ,m_start(start)
    //    ,m_end(end)
    //    ,m_previous(prev)
    //    ,m_next(next)


    Segment::~Segment()
    {
    }

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

#if ACVER == 19
        ::Coord _rotEnd = RotCoord(&_midPoint, &_start, 1.00, 0.00);
#else
        ::Coord _rotEnd = RotCoord((Point2D) _midPoint, (Point2D) _start, 1.00, 0.00);
#endif

        return Segment(MidPoint(), Coord(_rotEnd));
    }


    void Segment::SetArc(double angle)
    {
        m_angle = angle;

        Coord _start(m_start.GetX(), m_start.GetY());
        Coord _end(m_end.GetX(), m_end.GetY());
        double _halfAngle = angle / 2;

        ::Coord rotEnd = RotCoord(_start.ToCoord(), _end.ToCoord(), sin(_halfAngle), cos(_halfAngle));
        ::Coord rotStart = RotCoord(_end.ToCoord(), _start.ToCoord(), sin(-_halfAngle), cos(-_halfAngle));

#if ACVER == 19
        Sector lin1 = SetSector(_start.ToCoord(), rotEnd);
        Sector lin2 = SetSector(_end.ToCoord(), rotStart);
#else
		Sector lin1{ _start.ToCoord(), rotEnd };
		Sector lin2{ _end.ToCoord(), rotStart };
#endif

        ::Coord xc(0, 0);

        double eps = 0, radEps = 0;

        XLinesEps(lin1, lin2, &xc, eps, radEps);

        m_center.SetX(xc.x);
        m_center.SetY(xc.y);
        m_radius = (float)Dist(xc, _start.ToCoord());
    }


    void Segment::SetArc(double angle, Coord center)
    {
        m_angle = angle;
        m_center.SetX(center.GetX());
        m_center.SetY(center.GetY());
        //TODO m_radius
    }

    //void Segment::SetStart(const Coord& start)
    //{
    //    m_start = start;
    //}
    //
    //void Segment::SetEnd(const Coord& end)
    //{
    //    m_end = end;
    //}

    std::string Segment::ToString(LogFormat i_format) const
    {
        std::string result = "";

		switch (i_format) {
		case LogFormat::Default:
            result = str(boost::format("%-.2f %-.2f") % m_start.GetX() % m_start.GetY());
			return result;
        case LogFormat::Index:
			result = str(boost::format("[%d]") % m_idx);
            return result;
		case LogFormat::Short:
            result = str(boost::format("[%d]: [%d] %-.2f %-.2f -> [%d] %-.2f %-.2f") % m_idx % m_startIdx % m_start.GetX() % m_start.GetY() % m_endIdx % m_end.GetX() % m_end.GetY());
			return result;
		case LogFormat::Detailed:
			result += str(boost::format("Segment Idx: %d\n") % m_idx);
			result += str(boost::format(" StartIdx: %d, EndIdx: %d\n") % m_startIdx % m_endIdx);
			result += str(boost::format(" Start: %-.2f %-.2f\n") % m_start.GetX() % m_start.GetY());
			result += str(boost::format(" End: %-.2f %-.2f\n") % m_end.GetX() % m_end.GetY());
			result += str(boost::format(" Length: %-.2f\n") % GetLength());
			result += str(boost::format(" Arc Angle: %-.2f\n") % m_angle);
			result += str(boost::format(" Radius: %-.2f\n") % m_radius);
			return result;
		}

        return result;
    }

    const double Segment::GetLength() const
    {
        if (m_angle > -EPS
            && m_angle < EPS)
            return sqrt(pow((m_end.GetX() - m_start.GetX()), 2) + pow((m_end.GetY() - m_start.GetY()), 2));
        else
            return abs(m_radius * m_angle);
    }

    const Sector Segment::toSector() const
    {
#if ACVER == 19
        return SetSector(m_start, m_end);
#else
		return Sector{ m_start, m_end };
#endif
    }



    //bool Segment::operator()(Segment *s1, Segment *s2) { return s1->GetLength() < s2->GetLength() ; }

}
