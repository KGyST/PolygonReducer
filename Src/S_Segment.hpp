#pragma once

#ifndef S_SEGMENT_HPP
#define S_SEGMENT_HPP
#endif // !S_SEGMENT_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Point2DData.h"
#include "Sector2DData.h"


class S_Segment
{
private:
    unsigned int idx;
    API_Coord start;
    API_Coord end;
    double length;

    unsigned int status1;
    unsigned int status2;
    //unsigned int prvIdx;
    //unsigned int nxtIdx;
    unsigned int startIdx;  // vertexID
    unsigned int endIdx;    // vertexID
    S_Segment* previous;
    S_Segment* next;
    bool startPoint;        // Is it a start of a subcontour

    API_Coord center;
    float angle;
    float radius;

    //DELETE
    void init(int idx, int startIdx, int endIdx, API_Coord start, API_Coord end);


public:
    S_Segment() {};
    inline S_Segment(API_Coord start, API_Coord end)                                { init(0, 0, 0,start, end); };
    S_Segment(int idx, API_Coord start, API_Coord end)                              { init(idx, 0, 0, start, end); };
    S_Segment(int idx, int startIdx, int endIdx, API_Coord start, API_Coord end)    { init(idx, startIdx, endIdx, start, end); };
    ~S_Segment();

    void SetArc(double angle);
};