#include "S_Segment.hpp"
#include "APICommon.h"

using namespace Geometry;
namespace PolygonReducer {

    void S_Segment::init(int idx, int startIdx, int endIdx, API_Coord start, API_Coord end) {
        start = start;
        end = end;
        idx = idx;
        startIdx = startIdx;
        endIdx = endIdx;
        length = sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2));
        status1 = 0;
        status2 = 0;
    }


    S_Segment::~S_Segment()
    {
    }

    void S_Segment::SetArc(double angle)
    {
        angle = angle;

        Coord _start = *new Coord(start.x, start.y);
        Coord _end = *new Coord(end.x, end.y);
        double _halfAngle = PI / 4 - angle / 2;

        Coord rotEnd = RotCoord(_start, _end, sin(_halfAngle), cos(_halfAngle));
        Coord rotStart = RotCoord(_end, _start, sin(_halfAngle), cos(_halfAngle));

        Sector lin1 = SetSector(_start, rotEnd);
        Sector lin2 = SetSector(_end, rotStart);

        Coord* xc = new Coord(0, 0);

        double eps = 0, radEps = 0;

        XLinesEps(lin1, lin2, xc, eps, radEps);

        center.x = xc->x;
        center.y = xc->y;
    }

    //using namespace std;
    std::string S_Segment::toString()
    {
        std::string result = "";

        char* _s = "";

        sprintf(_s, "%d %d \n", start.x, start.y);

        result += std::string(_s);

        return result;
    }
}
