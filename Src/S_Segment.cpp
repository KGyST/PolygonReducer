#include "S_Segment.hpp"

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
    //FIXME
}