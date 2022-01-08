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
}


S::Segment::~Segment()
{
}

S::Coord S::Segment::midPoint()
{
    return Coord((m_start.getX() + m_end.getX())/2, (m_start.getY() + m_end.getY())/2);
}

S::Segment S::Segment::midPerp()
{
    ::Coord _start = m_start.toCoord();
    ::Coord _midPoint = midPoint().toCoord();
    ::Coord _rotEnd = RotCoord(&_midPoint, &_start, 1.00, 0.00);

    return S::Segment(midPoint(), Coord(_rotEnd));
}


void S::Segment::SetArc(double angle)
{
    m_angle = angle;

    Coord *_start = new Coord(m_start.getX(), m_start.getY());
    Coord *_end = new Coord(m_end.getX(), m_end.getY());
    double _halfAngle = PI / 4 - angle / 2;

    ::Coord rotEnd = RotCoord(_start->toCoord(), _end->toCoord(), sin(_halfAngle), cos(_halfAngle));
    ::Coord rotStart = RotCoord(_end->toCoord(), _start->toCoord(), sin(_halfAngle), cos(_halfAngle));

    Sector lin1 = SetSector(_start->toCoord(), rotEnd);
    Sector lin2 = SetSector(_end->toCoord(), rotStart);

    ::Coord* xc = new ::Coord(0, 0);

    double eps = 0, radEps = 0;

    XLinesEps(lin1, lin2, xc, eps, radEps);

    m_center.setX(xc->x);
    m_center.setY(xc->y);
}


void S::Segment::SetArc(double angle, S::Coord center)
{
    m_angle = angle;
    m_center.setX(center.getX());
    m_center.setY(center.getY());
}


std::string S::Segment::toString()
{
    std::string result = "";

    auto _s = str(boost::format("%-.2f %-.2f \n") % m_start.getX() % m_start.getY());

    result += std::string(_s);

    return result;
}

