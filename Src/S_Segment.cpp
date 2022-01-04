#include "S_Segment.hpp"
#include "APICommon.h"

#include <boost/format.hpp>

using namespace Geometry;
namespace PolygonReducer {

    void S_Segment::init(int idx, int startIdx, int endIdx, API_Coord& p_start, API_Coord& p_end)
    {
        m_start = p_start;
        m_end = p_end;
        m_idx = idx;
        m_startIdx = startIdx;
        m_endIdx = endIdx;
        m_length = sqrt(pow((m_end.x - m_start.x), 2) + pow((m_end.y - m_start.y), 2));
        m_status1 = 0;
        m_status2 = 0;
    }


    S_Segment::~S_Segment()
    {
    }

    void S_Segment::SetArc(double angle)
    {
        angle = angle;

        Coord *_start = new Coord(m_start.x, m_start.y);
        Coord *_end = new Coord(m_end.x, m_end.y);
        double _halfAngle = PI / 4 - angle / 2;

        Coord rotEnd = RotCoord(_start, _end, sin(_halfAngle), cos(_halfAngle));
        Coord rotStart = RotCoord(_end, _start, sin(_halfAngle), cos(_halfAngle));

        Sector lin1 = SetSector(*_start, rotEnd);
        Sector lin2 = SetSector(*_end, rotStart);

        Coord* xc = new Coord(0, 0);

        double eps = 0, radEps = 0;

        XLinesEps(lin1, lin2, xc, eps, radEps);

        m_center->x = xc->x;
        m_center->y = xc->y;
    }

    void S_Segment::SetArc(double angle, Coord center)
    {
        m_angle = angle;
        m_center->x = center.x;
        m_center->y = center.y;
    }

    //using namespace std;
    std::string S_Segment::toString()
    {
        std::string result = "";

        auto _s = str(boost::format("%-.2f %-.2f \n") % m_start.x % m_start.y);

        result += std::string(_s);

        return result;
    }
}
