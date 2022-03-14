#include "Segment.hpp"
#include "APICommon.h"

#include <boost/format.hpp>

using namespace Geometry;

void S::Segment::init(int idx, int startIdx, int endIdx, const S::Coord& p_start, const S::Coord& p_end)
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


S::Segment::~Segment()
{
}

const S::Coord S::Segment::MidPoint() const
{
    return Coord((m_start.GetX() + m_end.GetX())/2, (m_start.GetY() + m_end.GetY())/2);
}

const S::Segment S::Segment::MidPerp() const
{
    ::Coord _start = m_start.ToCoord();
    ::Coord _midPoint = MidPoint().ToCoord();
    ::Coord _rotEnd = RotCoord(&_midPoint, &_start, 1.00, 0.00);

    return S::Segment(MidPoint(), Coord(_rotEnd));
}


void S::Segment::SetArc(double angle)
{
    m_angle = angle;

    Coord _start (m_start.GetX(), m_start.GetY());
    Coord _end (m_end.GetX(), m_end.GetY());
    double _halfAngle = angle / 2;

    ::Coord rotEnd = RotCoord(_start.ToCoord(), _end.ToCoord(), sin(_halfAngle), cos(_halfAngle));
    ::Coord rotStart = RotCoord(_end.ToCoord(), _start.ToCoord(), sin(-_halfAngle), cos(-_halfAngle));

    Sector lin1 = SetSector(_start.ToCoord(), rotEnd);
    Sector lin2 = SetSector(_end.ToCoord(), rotStart);

    ::Coord xc (0, 0);

    double eps = 0, radEps = 0;

    XLinesEps(lin1, lin2, &xc, eps, radEps);

    m_center.SetX(xc.x);
    m_center.SetY(xc.y);
}


void S::Segment::SetArc(double angle, S::Coord center)
{
    m_angle = angle;
    m_center.SetX(center.GetX());
    m_center.SetY(center.GetY());
}

void S::Segment::SetStart(const Coord& start)
{
    m_start = start;
}

void S::Segment::SetEnd(const Coord& end)
{
    m_end = end;
}


std::string S::Segment::ToString() const
{
    std::string result = "";

    auto _s = str(boost::format("%-.2f %-.2f \n") % m_start.GetX() % m_start.GetY());

    result += std::string(_s);

    return result;
}

const double S::Segment::GetLength() const
{
    if  (   m_angle > -EPS
        &&  m_angle <  EPS)
        return sqrt(pow((m_end.GetX() - m_start.GetX()), 2) + pow((m_end.GetY() - m_start.GetY()), 2));
    else
        return m_radius * m_angle;
    }
    


//bool S::Segment::operator()(S::Segment *s1, S::Segment *s2) { return s1->GetLength() < s2->GetLength() ; }
