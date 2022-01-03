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
        m_angle = angle;

        Coord _start(m_start.x, m_start.y);
        Coord _end(m_end.x, m_end.y);
        double _halfAngle = angle / 2;

        Coord rotEnd = RotCoord(_start, _end, sin(_halfAngle), cos(_halfAngle));
        Coord rotStart = RotCoord(_end, _start, -sin(_halfAngle), cos(_halfAngle));

        Sector lin1 = SetSector(_start, rotEnd);
        Sector lin2 = SetSector(_end, rotStart);

        Coord xc((m_start.x + m_end.x) / 2, (m_start.y + m_end.y) / 2);

        double eps = 0, radEps = 0;

        XLinesEps(lin1, lin2, &xc, eps, radEps);

        m_center.x = xc.x;
        m_center.y = xc.y;

        m_radius = Dist(xc, _start);
    }

    using namespace std;
    string S_Segment::toString()
    {
        const std::string NDIGITS = "3";
        const UINT8 NTABS = 2;
        const string STATUS = "1";

        string result = "";
        string _tabs = "";
        for (UINT8 i = 0; i < NTABS; i++) _tabs += "\t";
        
        string _number = _tabs + "%-." + NDIGITS +"f,";

        result += str(boost::format(_number + _number + _tabs + STATUS) % m_start.x % m_start.y);

        if  (m_radius >  EPS)
        {
            result += str(boost::format(",\n" + _tabs + "0," + _number + _tabs + "400" + STATUS) % m_angle);
        }

        return result;
    }
}
